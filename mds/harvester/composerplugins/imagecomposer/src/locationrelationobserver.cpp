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
* Description:  Image Composer's listener for changes in db location relations
*
*/


#include <mdelogiccondition.h>
#include <mdequery.h>

#include "locationrelationobserver.h"
#include "harvesterlog.h"
#include "mdeconstants.h"
#include "composerimageplugin.h"

using namespace MdeConstants;


CLocationRelationObserver::CLocationRelationObserver() :
    iDefaultNamespace( NULL ), iSession( NULL ), iComposer( NULL ),
    iQuery( NULL ), iQueryStarted( EFalse )
	{
	// No implementation required
	}

CLocationRelationObserver::~CLocationRelationObserver()
	{
	iRelationIdArray.Close();
	
	if( iSession )
		{
		TRAP_IGNORE( iSession->RemoveRelationObserverL( *this ) );
		}
	
	if( iQuery )
	    {
	    iQuery->Cancel();
	    delete iQuery;
	    iQuery = NULL;
	    }
	}

CLocationRelationObserver* CLocationRelationObserver::NewL( CMdESession* aSession, CComposerImagePlugin* aComposer )
	{
	WRITELOG( "CLocationRelationObserver::NewL start" );
	CLocationRelationObserver* self = new (ELeave)CLocationRelationObserver();
	CleanupStack::PushL( self );
	self->ConstructL( aSession, aComposer );
	CleanupStack::Pop( self ); 
	WRITELOG( "CLocationRelationObserver::NewL end" );
	return self;
	}

void CLocationRelationObserver::ConstructL( CMdESession* aSession, CComposerImagePlugin* aComposer )
	{
    WRITELOG( "CLocationRelationObserver::ConstructL start" );
    User::LeaveIfNull( iSession = aSession );
    User::LeaveIfNull( iComposer = aComposer );
    
    iDefaultNamespace = &iSession->GetDefaultNamespaceDefL();

    // start listening to mde location object changes
    CMdELogicCondition* condition = CMdELogicCondition::NewL( ELogicConditionOperatorAnd );
    CleanupStack::PushL( condition );
    
    CMdERelationDef& relationDef = iDefaultNamespace->GetRelationDefL( 
    		MdeConstants::Relations::KContainsLocation );
    
    // Create relation condition
    CMdERelationCondition* relCond = &condition->AddRelationConditionL( relationDef );

    // Remove relation condition from logic condition
    condition->Remove( 0 );
    // and delete logic condition
    CleanupStack::PopAndDestroy( condition );

    iSession->AddRelationObserverL( *this, relCond, ENotifyAdd | ENotifyModify | ENotifyRemove, iDefaultNamespace );

    WRITELOG( "CLocationRelationObserver::ConstructL end" );
	}

void CLocationRelationObserver::HandleRelationNotification(
		CMdESession& /*aSession*/, TObserverNotificationType aType,
		const RArray<TItemId>& aRelationIdArray)
	{
#ifdef _DEBUG
	WRITELOG2( "CLocationRelationObserver::HandleObjectModified() objects: %d, aType: %d", aRelationIdArray.Count(), aType );
#endif
	
	if ( aType == ENotifyAdd || aType == ENotifyModify )
    	{
#ifdef _DEBUG
        WRITELOG1( "CLocationRelationObserver::HandleObjectModified() objects: %d", aRelationIdArray.Count() );
#endif

        // get images 
        const TInt count = aRelationIdArray.Count();
        for( TInt i = 0; i < count; i ++ )
        	{
        	iRelationIdArray.Append( aRelationIdArray[i] );
        	}
        
        if( !iQueryStarted )
        	{
        	TRAP_IGNORE( QueryRelationItemsL() );
        	}
    	}	
	}

// ---------------------------------------------------------------------------
// QueryImageObjectIdsL
// ---------------------------------------------------------------------------
//
void CLocationRelationObserver::QueryRelationItemsL()
    {
    if ( iRelationIdArray.Count() > 0 )
    	{
    	iQueryStarted = ETrue;
    	}
    else
    	{ 
    	iQueryStarted = EFalse;
    	return;
    	}
    	
    delete iQuery; iQuery = NULL;
    
    RArray<TItemId> objectIds;
    CleanupClosePushL( objectIds );
    const TInt count = iRelationIdArray.Count();
    for ( TInt i = 0; i < count; i++ )
    	{
    	objectIds.Append( iRelationIdArray[i] );
    	}
    iRelationIdArray.Reset();
    
    iQuery = iSession->NewRelationQueryL( *iDefaultNamespace, this );
    User::LeaveIfNull( iQuery );

    iQuery->SetResultMode( EQueryResultModeId );
    iQuery->Conditions().SetOperator( ELogicConditionOperatorAnd );
    
    // Relation id must be in array
    iQuery->Conditions().AddRelationConditionL( objectIds );
    
    CMdERelationCondition& filterCondLeft = iQuery->Conditions().
    		AddRelationConditionL( ERelationConditionSideLeft );

    // Left object in relation must be an image object.
    CMdEObjectDef& imageObjDef = iDefaultNamespace->GetObjectDefL(
    		MdeConstants::Image::KImageObject );
    filterCondLeft.LeftL().AddObjectConditionL( imageObjDef );
    
    CMdERelationCondition& filterCondRight = iQuery->Conditions().
	AddRelationConditionL( ERelationConditionSideRight );
    
    // Right object in relation must be a location object.
    CMdEObjectDef& locationObjDef = iDefaultNamespace->GetObjectDefL(
    		Location::KLocationObject );
    filterCondRight.RightL().AddObjectConditionL( locationObjDef );
    
    iQuery->FindL(); // results to a call to HandleQueryCompleted()
    
    CleanupStack::PopAndDestroy( &objectIds );
    }

// ---------------------------------------------------------------------------
// HandleQueryCompleted
// From MMdEQueryObserver.
// ---------------------------------------------------------------------------
//
void CLocationRelationObserver::HandleQueryNewResults( CMdEQuery& /*aQuery*/,
    TInt /*aFirstNewItemIndex*/, TInt /*aNewItemCount*/ )
    {
    }

// ---------------------------------------------------------------------------
// HandleQueryCompleted
// From MMdEQueryObserver.
// ---------------------------------------------------------------------------
//
void CLocationRelationObserver::HandleQueryCompleted( CMdEQuery& aQuery, TInt /*aError*/ )
    {
    if ( aQuery.Count() == 0 )
        {
        WRITELOG( "CLocationObjectObserver::HandleQueryCompleted() - no items found" );
        }
    else 
    	{
	    TRAP_IGNORE(
	    const TInt count = aQuery.Count();
	    for ( TInt i = 0; i < count; i++ )
	    	{
	    	CMdERelation* relation = iSession->GetRelationL( aQuery.ResultId(i) );
	        if ( iComposer && relation )
	            {
	            iComposer->WriteGPSTagsL( relation->LeftObjectId(), relation->RightObjectId() );
	            }
	    	
	    	delete relation;
	    	relation = NULL;
	    	}
	    	)
    	}
    
   	TRAP_IGNORE( QueryRelationItemsL() );
    }
