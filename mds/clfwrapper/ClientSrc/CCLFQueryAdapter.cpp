/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  QueryAdapter implementation for CLF
*
*/


// Includes
#include "CCLFQueryAdapter.h"
#include "CLFConsts.h"
#include "CLFPanics.h"
#include "CLF2MdEConsts.h"
#include "CLFUtils.h"
#include <e32base.h>
#include <e32std.h>
#include <e32debug.h>
#include "MGDebugPrint.h"
#include "MGTracePrint.h"
#include <mdecondition.h>
#include <mdeconstants.h>
#include <mdetextpropertycondition.h>
#include <mdeobject.h>
#include <mdeproperty.h>
#include <mdequery.h>
#include <mdenamespacedef.h>

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// CCLFQueryAdapter::CCLFQueryAdapter
// Default constructor
// ---------------------------------------------------------------------------
//
CCLFQueryAdapter::CCLFQueryAdapter( CMdESession& aMdESession )
    : iMdESession( aMdESession ), iDefaultNamespaceDef( NULL )
    {
    // pass
    }

// ---------------------------------------------------------------------------
// CCLFQueryAdapter::ConstructL
// Two-phase constructor
// ---------------------------------------------------------------------------
//
void CCLFQueryAdapter::ConstructL()
    {
    iDefaultNamespaceDef = &iMdESession.GetDefaultNamespaceDefL();
    }

// ---------------------------------------------------------------------------
// CCLFQueryAdapter::NewL
// Two-phase API constructor
// ---------------------------------------------------------------------------
//
CCLFQueryAdapter* CCLFQueryAdapter::NewL( CMdESession& aMdESession )
    {
    CCLFQueryAdapter* self = new( ELeave ) CCLFQueryAdapter( aMdESession );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CCLFQueryAdapter::~CCLFQueryAdapter
// Destructor
// ---------------------------------------------------------------------------
//
CCLFQueryAdapter::~CCLFQueryAdapter()
    {
    iQueryArray.Reset();
    iStatusArray.Reset();
    }

// ---------------------------------------------------------------------------
// CCLFQueryAdapter::QueryMdEObjectsL
// Queries the objects based on CLF media and mime types
// ---------------------------------------------------------------------------
//
void CCLFQueryAdapter::QueryMdEObjectsL( const MDesCArray& aMimeTypeArray,
                                         const TArray<TInt>& aMediaTypeArray,
                                         CMdEObjectQuery*& aQuery,
                                         TRequestStatus& aStatus )
    {
    // hard-coded mime types for music
    const TDesC* clfwrappermusictypes[] =
            {
            &KCLFWrapperMusicTypeMpeg,
            &KCLFWrapperMusicTypeAac,
            &KCLFWrapperMusicTypeMp3,
            &KCLFWrapperMusicTypeX_Mp3,
            &KCLFWrapperMusicTypeMp4,
            &KCLFWrapperMusicType3gpp,
            &KCLFWrapperMusicTypeM4a,
            &KCLFWrapperMusicType3gpp2,
            &KCLFWrapperMusicTypeMpeg4,
            &KCLFWrapperMusicTypeX_Pn_RealAudio,
            &KCLFWrapperMusicTypeX_Ms_Wma,
            };
    
    CMdEObjectDef& def = iDefaultNamespaceDef->GetObjectDefL( MdeConstants::Object::KBaseObject );
    __ASSERT_DEBUG( !aQuery, User::Panic( KCLFPanicText, KErrNotReady ));
    aQuery = iMdESession.NewObjectQueryL( *iDefaultNamespaceDef, def, this );
    aQuery->SetResultMode( EQueryResultModeId );

    CMdELogicCondition& rootCondition = aQuery->Conditions();

    if ( aMimeTypeArray.MdcaCount() > 0 )
        {
        rootCondition.SetOperator( ELogicConditionOperatorOr );
        CMdEPropertyDef& propDef = def.GetPropertyDefL( MdeConstants::Object::KItemTypeProperty );
        
        // Add object conditions based on mime types.
        for ( TInt index( 0 ); index < aMimeTypeArray.MdcaCount(); index++ )
            {
            CMdELogicCondition& logicCond = rootCondition.AddLogicConditionL( ELogicConditionOperatorOr );
            AddMimeTypeConditionL(logicCond, propDef, aMimeTypeArray.MdcaPoint( index ));
            }
        }
    else
        {
        TBool music = EFalse;
        TBool sounds = EFalse;

        // Add mime conditions for audio types
        CMdEObjectDef* objDef = NULL;
        CMdEPropertyDef* propDef = NULL;

        const TInt typeCount( aMediaTypeArray.Count() );
        
        // Add object conditions based on CLF media types.
        if ( typeCount > 0 )
            {
            rootCondition.SetOperator( ELogicConditionOperatorOr );

            for ( TInt index( 0 ); index < typeCount; index++ )
               {
               const TDesC& type = CLFUtils::MapClfType( aMediaTypeArray[ index ] );
               if ( type != KNullDesC )
                   {
                   CMdEObjectDef& objDef = iDefaultNamespaceDef->GetObjectDefL( type );
                   if( aMediaTypeArray[ index ] == ECLFMediaTypeSound )
                       {
                       sounds = ETrue;
                       }
                   else if( aMediaTypeArray[ index ] == ECLFMediaTypeMusic )
                       {
                       music = ETrue;
                       }
                   else
                       {
                       rootCondition.AddObjectConditionL( objDef );     
                       }
                   }
               }
            if( sounds && music )
                {
                CMdEObjectDef& objDef = iDefaultNamespaceDef->GetObjectDefL( CLFUtils::MapClfType( ECLFMediaTypeMusic ) );
                rootCondition.AddObjectConditionL( objDef );    
                }
            else if( sounds )
                {
                objDef = &iDefaultNamespaceDef->GetObjectDefL( CLFUtils::MapClfType( ECLFMediaTypeSound ) );
                propDef = &objDef->GetPropertyDefL( MdeConstants::Object::KItemTypeProperty );
                CMdELogicCondition& soundCond = rootCondition.AddLogicConditionL( ELogicConditionOperatorAnd );

                // add all necessary mime types to query conditions
                const TInt num = sizeof ( clfwrappermusictypes ) / sizeof( TDesC* );
                for ( TInt i = 0; i < num; ++i )
                    {
                    CMdELogicCondition& mimeCond = soundCond.AddLogicConditionL( ELogicConditionOperatorOr );
                    mimeCond.AddPropertyConditionL( *propDef, ETextPropertyConditionCompareEquals, *clfwrappermusictypes[i] );
                    mimeCond.SetNegate( ETrue );
                    }  
                soundCond.AddObjectConditionL( *objDef );
                }
            else if( music )
                {
                objDef = &iDefaultNamespaceDef->GetObjectDefL( CLFUtils::MapClfType( ECLFMediaTypeMusic ) );
                propDef = &objDef->GetPropertyDefL( MdeConstants::Object::KItemTypeProperty );
                CMdELogicCondition& musicCond = rootCondition.AddLogicConditionL( ELogicConditionOperatorAnd );

                // add all necessary mime types to query conditions
                const TInt num = sizeof ( clfwrappermusictypes ) / sizeof( TDesC* );
                CMdELogicCondition& mimeCond = musicCond.AddLogicConditionL( ELogicConditionOperatorOr );
                for ( TInt i = 0; i < num; ++i )
                    {
                    mimeCond.AddPropertyConditionL( *propDef, ETextPropertyConditionCompareEquals, *clfwrappermusictypes[i] );
                    }  
                musicCond.AddObjectConditionL( *objDef );
                }
            }
        }

    aStatus = KRequestPending;
    iStatusArray.AppendL( &aStatus );
    iQueryArray.AppendL( aQuery );

    // Check that we can actually execute the query. We can if we have at least one query condition.
    if ( rootCondition.Count() > 0 )
        {
        MG_DEBUG1( QMO5, "[CLF]\t CCLFQueryAdapter::QueryMdEObjectsL execute query");
        aQuery->FindL();
        }
    else
        {
        MG_DEBUG1( QMO6, "[CLF]\t CCLFQueryAdapter::QueryMdEObjectsL unknown conditions");
        CompleteRequest( *aQuery, KErrNone );
        }
    }

// ---------------------------------------------------------------------------
// CCLFQueryAdapter::QueryMdEObjectsL
// Queries the objects based on CLF ids
// ---------------------------------------------------------------------------
//
void CCLFQueryAdapter::QueryMdEObjectsL( const TArray< TCLFItemId >& aItemIDArray,
                                         const TDesC& aObjDefStr,
                                         CMdEObjectQuery*& aQuery,
                                         TRequestStatus& aStatus )
    {
    CMdEObjectDef& objDef = iDefaultNamespaceDef->GetObjectDefL( aObjDefStr );
    __ASSERT_DEBUG( !aQuery, User::Panic( KCLFPanicText, KErrNotReady ));
    aQuery = iMdESession.NewObjectQueryL( *iDefaultNamespaceDef, objDef, this );
    aQuery->SetResultMode( EQueryResultModeId );

    CMdELogicCondition& rootCondition = aQuery->Conditions();
    rootCondition.SetOperator( ELogicConditionOperatorOr );

    const TInt idCount( aItemIDArray.Count()  );
    if ( idCount > 0 )
        {
        RArray< TItemId > objectIds;

        // Add object conditions based on CLF ids.
        for ( TInt index( 0 ); index < idCount; index++ )
            {
            objectIds.Append( aItemIDArray[ index ] );
            }
        CleanupClosePushL( objectIds );
        rootCondition.AddObjectConditionL( objectIds );
        CleanupStack::PopAndDestroy( &objectIds );
        }

    aStatus = KRequestPending;
    iStatusArray.AppendL( &aStatus );
    iQueryArray.AppendL( aQuery );

    // Check that we can actually execute the query. We can if we have at least one query condition.
    if ( rootCondition.Count() > 0 )
        {
        MG_DEBUG1( QMO4, "[CLF]\t CCLFQueryAdapter::QueryMdEObjectsL execute query" );
        aQuery->FindL();
        }
    else
        {
        MG_DEBUG1( QMO5, "[CLF]\t CCLFQueryAdapter::QueryMdEObjectsL unknown conditions" );
        CompleteRequest( *aQuery, KErrNone );
        }
    }

// ---------------------------------------------------------------------------
// CCLFQueryAdapter::HandleQueryNewResults
// Called to notify the observer that new results have been received 
// ---------------------------------------------------------------------------
//
#ifdef MG_ENABLE_TRACE_PRINT
void CCLFQueryAdapter::HandleQueryNewResults( CMdEQuery& /* aQuery */,
                                              const TInt aFirstNewItemIndex,
                                              const TInt aNewItemCount )
#else
void CCLFQueryAdapter::HandleQueryNewResults( CMdEQuery& /* aQuery */,
                                              const TInt /* aFirstNewItemIndex */,
                                              const TInt /* aNewItemCount */ )
#endif
    {
    MG_DEBUG3( HQN1, "[CLF]\t HandleQueryNewResults, aFirstNewItemIndex == %d, aNewItemCount == %d",
               aFirstNewItemIndex, aNewItemCount );

    /* This method could be used to receive metadata query results in batches.
     * This feature is currently unimplemented. */
    }

// ---------------------------------------------------------------------------
// CCLFQueryAdapter::HandleQueryCompleted
// Called to notify the observer that the query has been completed
// ---------------------------------------------------------------------------
//
void CCLFQueryAdapter::HandleQueryCompleted( CMdEQuery& aQuery, const TInt aError )
    {
    MG_DEBUG2( HQC1, "[CLF]\t CCLFQueryAdapter::HandleQueryCompleted %d", aError );
    MG_DEBUG2( HQC2, "[CLF]\t aQuery.Count() returns: %d", aQuery.Count() );
    MG_DEBUG2( HQC3, "[CLF]\t Class instance %d", this );

    CompleteRequest( aQuery, aError );
    }

// ---------------------------------------------------------------------------
// CCLFQueryAdapter::CompleteRequest
// ---------------------------------------------------------------------------
//
void CCLFQueryAdapter::CompleteRequest( CMdEQuery& aQuery, const TInt aError )
    {
    MG_DEBUG1( HQC4, "[CLF]\t CCLFQueryAdapter::CompleteRequest" );
    const TInt count = iQueryArray.Count();
    TInt i( 0 );

    for ( i = 0; i < count; ++i )
        {
        if ( iQueryArray[ i ] == &aQuery )
            {
            User::RequestComplete( iStatusArray[ i ], aError );
            iStatusArray.Remove( i );
            iQueryArray.Remove( i );
            break;
            }
        }
#ifdef _DEBUG
    _LIT( KCLFWQAPanicText, "HandleQueryCompleted");
    __ASSERT_DEBUG( i < count, User::Panic( KCLFWQAPanicText, KErrAbort ));
#endif
    }

// ---------------------------------------------------------------------------
// CCLFQueryAdapter::AddMimeTypeConditionL
// ---------------------------------------------------------------------------
//
void CCLFQueryAdapter::AddMimeTypeConditionL( CMdELogicCondition& aLogicCondition,
                                              const CMdEPropertyDef& aPropDef, 
                                              const TPtrC& aMimeType ) 
    {
    HBufC16* result = HBufC16::NewLC( aMimeType.Length() );
    TPtr16 modResult = result->Des();
    modResult.Copy( aMimeType );

    // strip possible left asterisk
    if ( modResult.Locate( '*' ) == 0 )
        {
        modResult.Copy( modResult.Right( modResult.Length() - 1 ));
        }

    if ( modResult.Length() > 0 )
        {
        // strip possible right asterisk
        if ( modResult.LocateReverse( '*' ) == modResult.Length() - 1 )
            {
            modResult.Copy( modResult.Left( modResult.Length() - 1 ));
            }

        // add type condition
        aLogicCondition.AddPropertyConditionL( aPropDef, ETextPropertyConditionCompareContains, modResult );
        }
    CleanupStack::PopAndDestroy( result );
    }

//  End of File
