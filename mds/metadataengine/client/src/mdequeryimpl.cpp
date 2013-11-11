/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of object, relation and event query
*
*/


#include "mdequeryimpl.h"

#include "mdesessionimpl.h"
#include "mdeasynchronousfindao.h"
#include "mdeobject.h"
#include "mderelation.h"
#include "mdeevent.h"



CMdEObjectQueryImpl* CMdEObjectQueryImpl::NewL( CMdESessionImpl& aSessionImpl,
		CMdENamespaceDef& aNamespaceDef, CMdEObjectDef& aObjectDef,
		RPointerArray<CMdEObjectDef>* aObjectDefs, RMdEEngineSession& aSession )
	{
	CMdEObjectQueryImpl* self = CMdEObjectQueryImpl::NewLC( aSessionImpl, aNamespaceDef, 
			aObjectDef, aObjectDefs, aSession );
    CleanupStack::Pop( self );
    return self;
	}

CMdEObjectQueryImpl* CMdEObjectQueryImpl::NewLC( CMdESessionImpl& aSessionImpl,
		CMdENamespaceDef& aNamespaceDef, CMdEObjectDef& aObjectDef,
		RPointerArray<CMdEObjectDef>* aObjectDefs, RMdEEngineSession& aSession  )
	{
	CMdEObjectQueryImpl* self = new ( ELeave ) CMdEObjectQueryImpl( aSessionImpl, aNamespaceDef, 
			aObjectDef, aObjectDefs );
    CleanupStack::PushL( self );
    self->ConstructL( aSessionImpl, aSession );
    return self;
	}

CMdEObjectQueryImpl::CMdEObjectQueryImpl( 
		CMdESessionImpl& aSessionImpl, CMdENamespaceDef& aNamespaceDef, 
		CMdEObjectDef& aObjectDef, RPointerArray<CMdEObjectDef>* aObjectDefs )
    : CMdEObjectQuery( aSessionImpl, aNamespaceDef, aObjectDef, aObjectDefs )
    {
    }


void CMdEObjectQueryImpl::ConstructL( CMdESessionImpl& aSessionImpl,
    RMdEEngineSession& aSession )
    {
    ObjectQueryConstructL();
    iAsyncFind = CMdEAsynchronousFindAO::NewL( *this, aSessionImpl, aSession );
    }


CMdEObjectQueryImpl::~CMdEObjectQueryImpl()
    {
    if( iAsyncFind )
    	{    	
    	iAsyncFind->Cancel();
    	
    	delete iAsyncFind;
    	iAsyncFind = NULL;
    	}
    }


void CMdEObjectQueryImpl::DoFindL( TUint aMaxCount, TUint aNotifyCount )
    {
    iAsyncFind->DoFindL( aMaxCount, aNotifyCount );
    SetState( EStateSearching );
    }


void CMdEObjectQueryImpl::DoCancel()
    {
    // Stop searching.
    iAsyncFind->Cancel();
    SetState( EStateError );
    }


void CMdEObjectQueryImpl::DoNotifyResultsL( RPointerArray<CMdEInstanceItem>& aResultList )
    {
    NotifyNewResultsL( aResultList );
    }


void CMdEObjectQueryImpl::DoNotifyResultsL( RArray<TItemId>& aResultIdList )
    {
    NotifyNewResultsL( aResultIdList );
    }

void CMdEObjectQueryImpl::DoNotifyResultsL( CDesCArray& aResults )
	{
	NotifyNewResultsL( aResults );
	}

void CMdEObjectQueryImpl::DoNotifyResults( TInt aResultCount )
    {
    NotifyNewResults( aResultCount );
    }

void CMdEObjectQueryImpl::DoNotifyCompleted( TInt aError )
    {
    SetState( aError ? EStateError : EStateCompleted );
    NotifyCompleted( aError );
    }


CMdERelationQueryImpl* CMdERelationQueryImpl::NewL( CMdESessionImpl& aSessionImpl,
		CMdENamespaceDef& aNamespaceDef, RMdEEngineSession& aSession )
	{
	CMdERelationQueryImpl* self = CMdERelationQueryImpl::NewLC( aSessionImpl,
			aNamespaceDef, aSession );
    CleanupStack::Pop( self );
    return self;
	}

CMdERelationQueryImpl* CMdERelationQueryImpl::NewLC( CMdESessionImpl& aSessionImpl,
		CMdENamespaceDef& aNamespaceDef, RMdEEngineSession& aSession )
	{
	CMdERelationQueryImpl* self = new ( ELeave ) CMdERelationQueryImpl( aSessionImpl, aNamespaceDef );
    CleanupStack::PushL( self );
    self->ConstructL( aSessionImpl, aSession );
    return self;
	}

CMdERelationQueryImpl::CMdERelationQueryImpl( CMdESessionImpl& aSessionImpl, CMdENamespaceDef& aNamespaceDef )
    : CMdERelationQuery( aSessionImpl, aNamespaceDef )
    {
    }


void CMdERelationQueryImpl::ConstructL( CMdESessionImpl& aSessionImpl,
    RMdEEngineSession& aSession )
    {
    RelationQueryConstructL();
    iAsyncFind = CMdEAsynchronousFindAO::NewL( *this, aSessionImpl, aSession );
    }


CMdERelationQueryImpl::~CMdERelationQueryImpl()
    {
	delete iAsyncFind;
	iAsyncFind = NULL;
    }


void CMdERelationQueryImpl::DoFindL( TUint aMaxCount, TUint aNotifyCount )
    {
    iAsyncFind->DoFindL( aMaxCount, aNotifyCount );
    SetState( EStateSearching );
    }

    
void CMdERelationQueryImpl::DoCancel()
    {
    // Stop searching.
    iAsyncFind->Cancel();
    SetState( EStateError );
    }

void CMdERelationQueryImpl::DoNotifyResultsL( RPointerArray<CMdEInstanceItem>& aResultList )
    {
    NotifyNewResultsL( aResultList );
    }

void CMdERelationQueryImpl::DoNotifyResultsL( RArray<TItemId>& aResultIdList )
    {
    NotifyNewResultsL( aResultIdList );
    }

void CMdERelationQueryImpl::DoNotifyResults( TUint32 aResultCount )
    {
    NotifyNewResults( aResultCount );
    }

void CMdERelationQueryImpl::DoNotifyCompleted( TInt aError )
    {
    SetState( aError ? EStateError : EStateCompleted );
    NotifyCompleted( aError );
    }


CMdEEventQueryImpl::CMdEEventQueryImpl( CMdESessionImpl& aSessionImpl, CMdENamespaceDef& aNamespaceDef )
    : CMdEEventQuery( aSessionImpl, aNamespaceDef )
    {
    }


CMdEEventQueryImpl* CMdEEventQueryImpl::NewL( CMdESessionImpl& aSessionImpl,
		CMdENamespaceDef& aNamespaceDef, RMdEEngineSession& aSession )
	{
	CMdEEventQueryImpl* self = CMdEEventQueryImpl::NewLC( aSessionImpl,
			aNamespaceDef, aSession );
    CleanupStack::Pop( self );
    return self;
	}

CMdEEventQueryImpl* CMdEEventQueryImpl::NewLC( CMdESessionImpl& aSessionImpl,
		CMdENamespaceDef& aNamespaceDef, RMdEEngineSession& aSession )
	{
	CMdEEventQueryImpl* self = new ( ELeave ) CMdEEventQueryImpl( aSessionImpl, aNamespaceDef );
    CleanupStack::PushL( self );
    self->ConstructL( aSessionImpl, aSession );
    return self;
	}

void CMdEEventQueryImpl::ConstructL( CMdESessionImpl& aSessionImpl,
    RMdEEngineSession& aSession )
    {
    EventQueryConstructL();
    iAsyncFind = CMdEAsynchronousFindAO::NewL( *this, aSessionImpl, aSession );
    }


CMdEEventQueryImpl::~CMdEEventQueryImpl()
    {
	delete iAsyncFind;
	iAsyncFind = NULL;
    }


void CMdEEventQueryImpl::DoFindL( TUint aMaxCount, TUint aNotifyCount )
    {
    iAsyncFind->DoFindL( aMaxCount, aNotifyCount );
    SetState( EStateSearching );
    }


void CMdEEventQueryImpl::DoCancel()
    {
    // Stop searching.
    iAsyncFind->Cancel();
    SetState( EStateError );
    }

void CMdEEventQueryImpl::DoNotifyResultsL( RPointerArray<CMdEInstanceItem>& aResultList )
    {
    NotifyNewResultsL( aResultList );
    }

void CMdEEventQueryImpl::DoNotifyResultsL( RArray<TItemId>& aResultIdList )
    {
    NotifyNewResultsL( aResultIdList );
    }

void CMdEEventQueryImpl::DoNotifyResults( TUint32 aResultCount )
    {
    NotifyNewResults( aResultCount );
    }

void CMdEEventQueryImpl::DoNotifyCompleted( TInt aError )
    {
    SetState( aError ? EStateError : EStateCompleted );
    NotifyCompleted( aError );
    }
