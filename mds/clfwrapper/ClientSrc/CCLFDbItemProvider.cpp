/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*
*/


// INCLUDE FILES
#include    "CCLFDbItemProvider.h"
#include    "CCLFItemImpl.h"
#include    "MCLFItemProviderObserver.h"
#include    "CCLFDbItemContainer.h"
#include    "CLFPanics.h"
#include    "MGDebugPrint.h"
#include    "CCLFQueryAdapter.h"
#include    "CLFUtils.h"
#include    <mdeobjectquery.h>
#include    <mdeconstants.h>

const TInt KCLFItemArrayGranularity( 25 );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::CCLFDbItemProvider
// -----------------------------------------------------------------------------
//
CCLFDbItemProvider::CCLFDbItemProvider( CCLFDbItemContainer& aDbItemContainer,
                                               CCLFQueryAdapter& aQueryAdapter, CMdESession* aMdESession )
    : CActive( EPriorityStandard ),
      iProviderState( EItemsReady ),
      iObserver( NULL ),
      iItemArray( KCLFItemArrayGranularity ),
      iDbItemContainer( aDbItemContainer ),
      iQueryAdapter( aQueryAdapter ),
      iObjectQuery( NULL ),
      iObjDefStr( &KNullDesC ),
      iMdESession( aMdESession )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::ConstructL
// -----------------------------------------------------------------------------
//
void CCLFDbItemProvider::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::NewL
// -----------------------------------------------------------------------------
//
CCLFDbItemProvider* CCLFDbItemProvider::NewL( CCLFDbItemContainer& aDbItemContainer,
                                              CCLFQueryAdapter& aQueryAdapter, CMdESession* aMdESession )
    {
    CCLFDbItemProvider* self = new( ELeave ) CCLFDbItemProvider( aDbItemContainer, 
                                                                                                       aQueryAdapter, 
                                                                                                       aMdESession );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::~CCLFDbItemProvider
// Destructor
// -----------------------------------------------------------------------------
//
CCLFDbItemProvider::~CCLFDbItemProvider()
    {
    Cancel();
    delete iObjectQuery;
    iObjectQuery = NULL;
    iItemArray.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::DoCancel
// -----------------------------------------------------------------------------
//
void CCLFDbItemProvider::DoCancel()
    {
    if ( iObjectQuery )
        {
        iObjectQuery->Cancel();
        }

    iObjDefStr = &KNullDesC;
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::RunL
// -----------------------------------------------------------------------------
//
void CCLFDbItemProvider::RunL()
    {
    MG_DEBUG2( r1, "[CLF]\t CCLFDbItemProvider::RunL %d", iStatus.Int() );

    TInt error( iStatus.Int() );
    if ( error == KErrNone )
        {
        DoRunL();
        }
    if ( EItemsReady == iProviderState )
        {
        iObserver->OperationCompleteL( KErrNone );
        }
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::DoRunL
// -----------------------------------------------------------------------------
//
void CCLFDbItemProvider::DoRunL()
    {
    switch ( iProviderState )
        {
        case EPrepareItemIds:
            {
            DoFetchMdSIdDataCachedL();
            break;
            }
        case EPrepareItems:
            {
            DoFetchMdSDataL();
            break;
            }
        default:
            {
            User::Leave( ECLFIncorrectCommand );
            }
        }
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::RunError
// -----------------------------------------------------------------------------
//
TInt CCLFDbItemProvider::RunError( const TInt aError )
    {
    MG_DEBUG2( re1, "[CLF]\t CCLFDbItemProvider::RunError: %d", aError ); 

    if ( (EItemsReady == iProviderState || aError) && iObserver )
    	{
        TRAP_IGNORE( iObserver->OperationCompleteL( aError ) );
        }
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::PrepareItemsL
// -----------------------------------------------------------------------------
//
void CCLFDbItemProvider::PrepareItemsL( const MDesCArray& aMimeTypeArray,
                                        const TArray< TInt >& aMediaTypeArray,
                                        MCLFItemProviderObserver& aObserver )
    {
    MG_DEBUG1( PI1, "[CLF]\t CCLFDbItemProvider::PrepareItemsL 1" ); 

    Cancel();
    iObserver = &aObserver;
    iItemArray.ResetAndDestroy();

    TBool image = EFalse;
    TBool video = EFalse;
    TBool audio = EFalse;
    TBool other = EFalse;
    
    // find all mediatypes
    const TInt typeCount( aMediaTypeArray.Count() );
    for ( TInt i = 0; i < typeCount; i++ )
        {
        iObjDefStr = &KNullDesC;
        iObjDefStr = &CLFUtils::MapClfType( aMediaTypeArray[ i ] );
        
        if (iObjDefStr->Compare(MdeConstants::Image::KImageObject) == 0)
            {
            image = ETrue;
            }
        else if (iObjDefStr->Compare(MdeConstants::Video::KVideoObject) == 0)
            {
            video = ETrue;
            }
        else if (iObjDefStr->Compare(MdeConstants::Audio::KAudioObject) == 0)
            {
            audio = ETrue;
            }
        else
            {
            other = ETrue;
            }
        }
    
    // images & videos (gallery)
    if (image && video && !audio)
        {
        iObjDefStr = &MdeConstants::Object::KBaseObject;
        }
    // images
    else if(image && !video && !audio && !other)
        {
        iObjDefStr = &MdeConstants::Image::KImageObject;
        }
    // videos
    else if (!image && video && !audio && !other)
        {
        iObjDefStr = &MdeConstants::Video::KVideoObject;
        }
    // audio
    else if (!image && !video && audio && !other)
        {
        iObjDefStr = &MdeConstants::Audio::KAudioObject;
        }
    // other
    else
        {
        iObjDefStr = &MdeConstants::Object::KBaseObject;
        }
    
    iProviderState = EPrepareItemIds;

    delete iObjectQuery;
    iObjectQuery = NULL;
    iQueryAdapter.QueryMdEObjectsL( aMimeTypeArray, aMediaTypeArray, iObjectQuery, iStatus );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::PrepareItemsL
// -----------------------------------------------------------------------------
//
void CCLFDbItemProvider::PrepareItemsL( const TArray< TCLFItemId >& aItemIDArray,
                                        MCLFItemProviderObserver& aObserver )
    {
    MG_DEBUG1( PI1, "[CLF]\t CCLFDbItemProvider::PrepareItemsL 2" ); 

    Cancel();
    iObserver = &aObserver;
    iItemArray.ResetAndDestroy();
    iProviderState = EPrepareItems;

    const TDesC& objDefStr = CLFUtils::MapClfType( ECLFMediaTypeUnknown );
    delete iObjectQuery;
    iObjectQuery = NULL;
    iQueryAdapter.QueryMdEObjectsL( aItemIDArray, objDefStr, iObjectQuery, iStatus );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::GetItems
// -----------------------------------------------------------------------------
//
TInt CCLFDbItemProvider::GetItems( RPointerArray< MCLFItem >& aItemArray )
    {
    MG_DEBUG1( df1, "[CLF]\t CCLFDbItemProvider::GetItems" ); 

    const TInt KCLFItemIndex( 0 ); // do not change

    while ( iItemArray.Count() )
        {
        const TInt error( aItemArray.Append(
                            iItemArray[ KCLFItemIndex ] ) ); // takes ownership
        if ( error )
            {
            return error;
            }
        iItemArray.Remove( KCLFItemIndex );
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::DoFetchMdSIdDataCachedL
// -----------------------------------------------------------------------------
//
void CCLFDbItemProvider::DoFetchMdSIdDataCachedL()
    {
    MG_DEBUG1( df1, "[CLF]\t CCLFDbItemProvider::DoFetchMdSIdDataCachedL" ); 

    RArray< TCLFItemId > itemIdArray;
    CleanupClosePushL( itemIdArray );
    DoFetchMdSIdDataL( itemIdArray );

    // Check if items are in cache
    TInt idCount( itemIdArray.Count() );
    for ( TInt i = 0 ; i < idCount ; ++i )
        {
        CCLFContainerItem* containerItem = 
                                iDbItemContainer.ItemById( itemIdArray[ i ] );
        if ( containerItem )
            {
            // found in cache
            CreateCLFItemL( *containerItem ); // reserve item for this model

            // remove handled item ID
            itemIdArray.Remove( i );
            --i;
            --idCount;
            }
        }
    iProviderState = EPrepareItems;

    delete iObjectQuery;
    iObjectQuery = NULL;
    iQueryAdapter.QueryMdEObjectsL( itemIdArray.Array(), *iObjDefStr, iObjectQuery, iStatus );
    SetActive();
    CleanupStack::PopAndDestroy( &itemIdArray );
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::DoFetchMdSDataL
// -----------------------------------------------------------------------------
//
void CCLFDbItemProvider::DoFetchMdSDataL()
    {
#ifdef _DEBUG
    MG_DEBUG1( df1, "[CLF]\t CCLFDbItemProvider::DoFetchDataL" ); 
    
    _LIT( KCLFWIPPanicText, "DoFetchMdSDataL");
    __ASSERT_DEBUG( iObjectQuery, User::Panic( KCLFWIPPanicText, KErrNotReady ));
#endif

    const TInt count( iObjectQuery->Count() );
    for ( TInt index( 0 ); index < count; index++ )
        {
        const TInt64 id = (TInt64)iObjectQuery->ResultId( index );
        CMdEObject* object = iMdESession->GetFullObjectL( id );
        
        if( !object )
            {
            continue;
            }

        // check if other model is already put this item to container
        CCLFContainerItem* containerItem = 
                                iDbItemContainer.ItemById( id );
        if ( containerItem )
            {
            delete object;
            object = NULL;
            continue;
            }
        else
            {
            // create container item
            containerItem = CCLFContainerItem::NewLC( object );

            // append container item to container
            // container takes ownership of container item
            iDbItemContainer.AddL( containerItem );
            CleanupStack::Pop( containerItem );
            }
        CreateCLFItemL( *containerItem );
        }
    iProviderState = EItemsReady;
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::DoFetchMdSIdDataL
// -----------------------------------------------------------------------------
//
void CCLFDbItemProvider::DoFetchMdSIdDataL( RArray< TCLFItemId >& aItemIDArray )
    {
#ifdef _DEBUG
    MG_DEBUG1( df1, "[CLF]\t CCLFDbItemProvider::DoFetchDataL" ); 

    _LIT( KCLWIPPanicString, "DoFetchMdSIdDataL" );
    __ASSERT_DEBUG( iObjectQuery,
                     User::Panic( KCLWIPPanicString, KErrAbort ));
#endif

    const TInt count( iObjectQuery->Count() );
    for ( TInt index( 0 ); index < count; index++ )
        {
        aItemIDArray.AppendL( (TInt64)iObjectQuery->ResultId( index ));
        }
    }

// -----------------------------------------------------------------------------
// CCLFDbItemProvider::CreateCLFItemL
// -----------------------------------------------------------------------------
//
void CCLFDbItemProvider::CreateCLFItemL( CCLFContainerItem& aCItem )
    {
    MG_DEBUG1( cci1, "[CLF]\t CCLFDbItemProvider::CreateCLFItemL" ); 

    // CLF item releases reference in destructor
    CCLFItemImpl* item = CCLFItemImpl::NewLC( aCItem, iDbItemContainer );
    iItemArray.AppendL( item ); // array takes ownership of the item
    CleanupStack::Pop( item );
    }

//  End of File
