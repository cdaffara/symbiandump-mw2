/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Metadata engine client session implementation*
*/

#include <etel3rdparty.h>
#include <e32property.h> 

#include "mdesessionimpl.h"
#include "mdesessionstartupao.h"

#include "mdcdef.h"
#include "mdcitem.h"
#include "mdcresult.h"
#include "mdeobjectdef.h"
#include "mderelationdef.h"
#include "mdeeventdef.h"
#include "mdeobject.h"
#include "mderelation.h"
#include "mdeevent.h"
#include "mdepanic.h"
#include "mdequeryimpl.h"
#include "mdenotifierao.h"
#include "mdeobjectdef.h"
#include "mdenamespacedef.h"
#include "mdccommon.pan"
#include "mdedatabuffer.h"
#include "mdcserializationbuffer.h"
#include "mdequerycriteriaserialization.h"
#include "mdelogiccondition.h"
#include "mdeobjectcondition.h"
#include "mdscommoninternal.h"

// for CleanupResetAndDestroyPushL
#include <mmf/common/mmfcontrollerpluginresolver.h>

RMdESessionAsyncRequest::RMdESessionAsyncRequest( TRequestType aRequestType, 
	CMdCSerializationBuffer* aBuffer, CMdCSerializationBuffer& aResultBuffer,
	TRequestStatus& aRequestStatus) : 
	iRequestType(aRequestType), iBuffer(aBuffer), iResultBuffer(&aResultBuffer), 
	iRequestStatus(&aRequestStatus)
	{
	*iRequestStatus = KRequestPending;
	}

void RMdESessionAsyncRequest::Close()
	{
	if (iBuffer)
		{
		delete iBuffer;
		iBuffer = NULL;
		}
	}

void CMdESessionAsyncHandler::AddRequest( CMdCSerializationBuffer* aBuffer,
			CMdCSerializationBuffer& aResultBuffer, 
			TRequestStatus& aRequestStatus )
	{
	RMdESessionAsyncRequest request = RMdESessionAsyncRequest( 
			RMdESessionAsyncRequest::EAddRequest, 
			aBuffer, aResultBuffer, aRequestStatus );

	const TInt error = iRequests.Append(request);
    if( error != KErrNone )
        {
        return;
        }

	if( !IsActive() )
		{
		iEngineSession.DoAddItemsAsync( *aBuffer, aResultBuffer, 
				iStatus );
		SetActive();
		}
	}

void CMdESessionAsyncHandler::UpdateRequest( CMdCSerializationBuffer * aBuffer,
			CMdCSerializationBuffer& aResultBuffer,
			TRequestStatus& aRequestStatus )
	{
	RMdESessionAsyncRequest request = RMdESessionAsyncRequest( 
			RMdESessionAsyncRequest::EUpdateRequest,
			aBuffer, aResultBuffer, aRequestStatus);

	const TInt error = iRequests.Append(request);
    if( error != KErrNone )
        {
        return;
        }

	if( !IsActive() )
		{
		iEngineSession.DoUpdateItemsAsync(*aBuffer, aResultBuffer, 
				iStatus);
		SetActive();
		}
	}

void CMdESessionAsyncHandler::RemoveRequest( CMdCSerializationBuffer* aBuffer,
		CMdCSerializationBuffer& aResultBuffer,
		TRequestStatus& aRequestStatus )
	{
	RMdESessionAsyncRequest request = RMdESessionAsyncRequest( 
			RMdESessionAsyncRequest::ERemoveRequest, 
			aBuffer, aResultBuffer, aRequestStatus);

	const TInt error = iRequests.Append(request);
	if( error != KErrNone )
	    {
	    return;
	    }

	if( !IsActive() )
		{
		iEngineSession.DoRemoveItemsAsync( *aBuffer, aResultBuffer, 
				iStatus );	
		SetActive();
		}
	}

CMdESessionAsyncHandler* CMdESessionAsyncHandler::NewL(CMdESessionImpl& aSession, 
		RMdEEngineSession &aEngineSession)
	{
	CMdESessionAsyncHandler* self = CMdESessionAsyncHandler::NewLC(
			aSession, aEngineSession);
	CleanupStack::Pop(self);
	return self;
	}

CMdESessionAsyncHandler* CMdESessionAsyncHandler::NewLC( CMdESessionImpl& aSession, 
		RMdEEngineSession &aEngineSession )
	{
	CMdESessionAsyncHandler *self = new (ELeave) CMdESessionAsyncHandler(
			aSession, aEngineSession);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CMdESessionAsyncHandler::~CMdESessionAsyncHandler()
	{
	Cancel();

	const TInt count = iRequests.Count();

	for (TInt i = 0; i < count; i++)
		{
		iRequests[i].Close();
		}
	iRequests.Close();		
	}		

void CMdESessionAsyncHandler::RunL()
	{
	//Remove first from array	
	RMdESessionAsyncRequest& request = iRequests[0];
	iRequests.Remove(0);

	const TInt status = iStatus.Int();

	User::RequestComplete( request.iRequestStatus, status );
	request.Close();
	
	if (iRequests.Count() > 0)
		{
		request = iRequests[0];

		iStatus = KRequestPending;
		
		switch( request.iRequestType )
			{
			case RMdESessionAsyncRequest::EAddRequest:
				iEngineSession.DoAddItemsAsync( *(request.iBuffer),
					*(request.iResultBuffer), iStatus);
				break;
			case RMdESessionAsyncRequest::EUpdateRequest:
				iEngineSession.DoUpdateItemsAsync( *(request.iBuffer),
					*(request.iResultBuffer), iStatus);
				break;
			case RMdESessionAsyncRequest::ERemoveRequest:
				iEngineSession.DoRemoveItemsAsync( *(request.iBuffer),
					*(request.iResultBuffer), iStatus);
				break;
			default:
			    break;
			}
		SetActive();			
		}						
	}

TInt CMdESessionAsyncHandler::RunError(TInt aError)
	{
	if( aError == KErrServerTerminated )
		{
		iSession.NotifyError( aError );
		return KErrNone;
		}
	else
		{
		return aError;
		}
	}

void CMdESessionAsyncHandler::DoCancel()
	{
	}

CMdESessionAsyncHandler::CMdESessionAsyncHandler(CMdESessionImpl& aSession, 
		RMdEEngineSession &aEngineSession)
	: CActive( CActive::EPriorityStandard ), iSession( aSession ), 
	iEngineSession(aEngineSession)
	{
	}

void CMdESessionAsyncHandler::ConstructL()		
	{
	CActiveScheduler::Add(this);
	}

CMdESessionImpl::CMdESessionImpl(MMdESessionObserver& aObserver)
	: iSessionStartupAO( NULL ), iSessionObserver(&aObserver), 
	  iSchemaBuffer( NULL ), iAsyncHandler(NULL), iNextQueryId( 0 ),
	  iSession( *this )
	{
	}

CMdESessionImpl::~CMdESessionImpl()
	{
    // No session errors should be sent during deconstruction to avoid possible double deletion
    iSessionObserver = NULL;

	delete iSchemaBuffer;
	iSchemaBuffer = NULL;

	iNotifiers.ResetAndDestroy();
	iNotifiers.Close();

	iSession.Shutdown();
	iSession.Close();
	
	iSchemaChunk.Close();
	
	iNamespaceDefs.ResetAndDestroy();
	iNamespaceDefs.Close();
	
	delete iSessionStartupAO;
	iSessionStartupAO = NULL;
		
	delete iAsyncHandler;
	iAsyncHandler = NULL;
	}

void CMdESessionImpl::ConstructL()
	{
    iSessionStartupAO = CMdESessionStartupAO::NewL( *this, iSession );
    iAsyncHandler = CMdESessionAsyncHandler::NewL( *this, iSession );    
    }

void CMdESessionImpl::Close()
	{
	}

TInt CMdESessionImpl::NamespaceDefCount() const
	{
	return iNamespaceDefs.Count();
	}

CMdENamespaceDef& CMdESessionImpl::NamespaceDefL( TInt aIndex )
	{
	return *iNamespaceDefs[aIndex];
	}

CMdENamespaceDef& CMdESessionImpl::GetNamespaceDefL( const TDesC& aName )
	{
	const TInt KNamespaceCount = iNamespaceDefs.Count();
	for ( TInt i = 0; i < KNamespaceCount; ++i )
		{
		if ( !aName.Compare( iNamespaceDefs[i]->Name() ) )
			{
			return NamespaceDefL( i );
			}
		}

	User::Leave( KErrNotFound );
	return NamespaceDefL( -1 ); // never reached
	}

CMdENamespaceDef& CMdESessionImpl::GetNamespaceDefL(TDefId aId)
	{
	const TInt KNamespaceCount = iNamespaceDefs.Count();
	for ( TInt i = 0; i < KNamespaceCount; ++i )
		{
		if ( iNamespaceDefs[i]->Id() == aId )
			{
			return NamespaceDefL( i );
			}
		}

	User::Leave( KErrNotFound );
	return NamespaceDefL( -1 ); // never reached
	}

CMdENamespaceDef& CMdESessionImpl::GetDefaultNamespaceDefL()
	{
	return GetNamespaceDefL( KDefaultNamespaceDefId );
	}

CMdEObject* CMdESessionImpl::NewObjectL( CMdEObjectDef& aDef, const TDesC& aUri, TUint32 aMediaId )
	{
	CMdEObject* object = NewObjectLC( aDef, aUri, aMediaId );
	CleanupStack::Pop(object);
	return object;
	}

CMdEObject* CMdESessionImpl::NewObjectLC( CMdEObjectDef& aDef, const TDesC& aUri, TUint32 aMediaId )
	{
	CMdEObject* object = CMdEObject::NewLC( aDef, aUri, aMediaId );
	return object;
	}

void CMdESessionImpl::CommitObjectL(CMdEObject& aObject)
    {
	// check state
	// check that object is open for modifications
	if (!aObject.OpenForModifications())
		{
		User::Leave( KErrMdENotLocked );
		}

	RPointerArray<CMdEInstanceItem> items;
	CleanupClosePushL( items );
	items.AppendL( &aObject );
	UpdateItemsL( items );
	CleanupStack::PopAndDestroy( &items );	
    }

void CMdESessionImpl::CommitObjectsL(RPointerArray<CMdEObject>& aObjects)
    {
	// check state
	// check that object is open for modifications

    RPointerArray<CMdEInstanceItem> items;
    CleanupClosePushL( items );

    const TInt objectsCount = aObjects.Count();
    items.ReserveL( objectsCount );    
    for (TInt i = 0; i < objectsCount; ++i)
    	{
    	CMdEObject* obj = aObjects[i];
    	if ( !obj->OpenForModifications() )
    		{
    		User::Leave( KErrMdENotLocked );
    		}
    	
    	items.AppendL( obj );
    	}

    UpdateItemsL(items);

    items.Reset();
    CleanupStack::PopAndDestroy( &items );
    }

TItemId CMdESessionImpl::CancelObjectL(CMdEObject& aObject)
    {
    // check that object is open for modifications
    if( !aObject.OpenForModifications() || !aObject.BelongsToSession() )
    	{
    	User::Leave( KErrMdENotLocked );
    	}
    
	CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( 
			sizeof(TMdCItemIds) + 
			CMdCSerializationBuffer::KRequiredSizeForTItemId );
	
	TMdCItemIds itemIds;
	itemIds.iNamespaceDefId = aObject.Def().NamespaceDef().Id();
	itemIds.iObjectIds.iPtr.iCount = 1;
	itemIds.iObjectIds.iPtr.iOffset = sizeof(TMdCItemIds);
	itemIds.SerializeL( *buffer );

	buffer->InsertL( aObject.Id() );

    iSession.DoCancelObjectL( *buffer );

    TItemId result;
    buffer->PositionL( KNoOffset );
    itemIds.DeserializeL( *buffer );
	buffer->PositionL( itemIds.iObjectIds.iPtr.iOffset );
	buffer->ReceiveL( result );

	CleanupStack::PopAndDestroy( buffer ); // buffer

	if (result == KNoId)
		{
		User::Leave( KErrNotFound );
		}

	aObject.SetNotOpenForModifications();
	return result;
    }

CMdERelation* CMdESessionImpl::NewRelationLC( CMdERelationDef& aDef, TItemId aLeftObjectId, 
										      TItemId aRightObjectId, TInt32 aParameter )
	{
	return CMdERelation::NewLC(aDef, aLeftObjectId, aRightObjectId, aParameter);
	}

CMdERelation* CMdESessionImpl::NewRelationL( CMdERelationDef& aDef, TItemId aLeftObjectId, 
										     TItemId aRightObjectId, TInt32 aParameter )
	{
	CMdERelation* rel = NewRelationLC( aDef, aLeftObjectId, aRightObjectId, aParameter );
	CleanupStack::Pop( rel );
	return rel;
	}


CMdEEvent* CMdESessionImpl::NewEventLC(CMdEEventDef& aDef, TItemId aObjectId, TTime aTime, const TDesC* aSource, const TDesC* aParticipant)
	{
	return CMdEEvent::NewLC(aDef, aObjectId, aTime, aSource, aParticipant );
	}

CMdEEvent* CMdESessionImpl::NewEventL(CMdEEventDef& aDef, TItemId aObjectId, TTime aTime, const TDesC* aSource, const TDesC* aParticipant)
	{
	CMdEEvent* event = NewEventLC( aDef, aObjectId, aTime, aSource, aParticipant );
	CleanupStack::Pop( event );
	return event;
	}
	
void CMdESessionImpl::AddSchemaObserverL(MMdESchemaObserver& aObserver)
	{
	CheckOpened();

	CMdENamespaceDef& defaultNamespaceDef = GetDefaultNamespaceDefL();

	TInt err = FindNotifier( ESchemaModify, &aObserver, defaultNamespaceDef );
	
    if ( err != KErrNotFound )
    	{
    	if ( err >= 0 )
    		{
    		return;
    		}
    	User::LeaveIfError( err );
    	}
    
    CMdENotifierAO* notifier = CMdENotifierAO::NewLC( *this, iSession );
    notifier->RegisterL( ESchemaModify, &aObserver, NULL, defaultNamespaceDef );
    User::LeaveIfError(iNotifiers.Append( notifier ));
    CleanupStack::Pop( notifier );
	}


void CMdESessionImpl::RemoveSchemaObserverL(MMdESchemaObserver& aObserver)
	{
	CheckOpened();

	CMdENamespaceDef& defaultNamespaceDef = GetDefaultNamespaceDefL();

    const TInt index = FindNotifier( ESchemaModify, &aObserver, defaultNamespaceDef );
    if ( index != KErrNotFound )
        {
	    iNotifiers[index]->Cancel();
	    delete iNotifiers[index];
	    iNotifiers[index] = NULL;
	    iNotifiers.Remove( index );
        }
    else
    	{
       	User::Leave( KErrNotFound );
    	}
	}

void CMdESessionImpl::NotifySessionOpened(TInt aError)
	{
	__ASSERT_DEBUG(iSessionObserver != 0,
				    TMdEPanic::Panic(TMdEPanic::EInternal));
	if(!aError)
		{
		iSessionState = EMdESessionOpen;
		}

    if( iSessionObserver )
        {
        iSessionObserver->HandleSessionOpened(*this, aError);
        }
	}


void CMdESessionImpl::NotifyError(TInt aError)
	{
	if(iSessionObserver)
		{
		iSessionObserver->HandleSessionError(*this, aError);
		}
	iSessionState = EMdESessionClosed;
	}

void CMdESessionImpl::LoadSchemaL()
	{
	DoLoadSchemaL();
	}

RMdEEngineSession& CMdESessionImpl::EngineSession()
	{
	return iSession;
	}

void CMdESessionImpl::DoLoadSchemaL()
    {
    TInt handle( 0 );
    iSession.DoLoadSchemaL( handle );
    
   	TBuf<32> name( KSchemaChunkName );
	name.AppendNum( handle );
	iSchemaChunk.Close();
	User::LeaveIfError( iSchemaChunk.OpenGlobal( name, ETrue ) );

	CMdCSerializationBuffer* schemaBuffer = 
		CMdCSerializationBuffer::NewLC( iSchemaChunk.Base(), iSchemaChunk.Size() );

	if ( schemaBuffer->Size() == 0 )
		{
		User::Leave( KErrNotFound );
		}
	else
		{
		CleanupStack::Pop( schemaBuffer );
		}

	delete iSchemaBuffer;
	iSchemaBuffer = NULL;

	iSchemaBuffer = schemaBuffer;
	
	iNamespaceDefs.ResetAndDestroy();
	
	// initialize namespacedefs
	const TMdCSchema& schema = TMdCSchema::GetFromBufferL(*iSchemaBuffer);
	const TUint32 namespaceCount = schema.iNamespaceDefs.iPtr.iCount;
	const TMdCOffset namespaceOffset = schema.iNamespaceDefs.iPtr.iOffset;

	iNamespaceDefs.ReserveL( namespaceCount );	
	for ( TUint32 i = 0; i < namespaceCount; ++i )
		{
		iSchemaBuffer->PositionL( namespaceOffset + i * sizeof(TMdCNamespaceDef) );
		const TMdCNamespaceDef& namespaceDef = TMdCNamespaceDef::GetFromBufferL(*iSchemaBuffer);
		iNamespaceDefs.AppendL( CMdENamespaceDef::NewL( *this, namespaceDef, *iSchemaBuffer ) );
		}
    }

void CMdESessionImpl::AddRelationDefL( const CMdENamespaceDef &aNamespaceDef, const TDesC &aName )
	{
	iSession.DoAddRelationDefL(aNamespaceDef.Id(), aName);
	DoLoadSchemaL();
	}

void CMdESessionImpl::AddEventDefL( const CMdENamespaceDef &aNamespaceDef, const TDesC &aName )
	{
	iSession.DoAddEventDefL(aNamespaceDef.Id(), aName);
	DoLoadSchemaL();
	}

/**
* Get methods
*/
CMdEObject* CMdESessionImpl::GetObjectL(CMdEObjectDef& aObjectDef, 
	const TItemId aId, const TInt64 aGuidHigh, const TInt64 aGuidLow, const TDesC& aUri, 
	TMdCQueryLockType aLocktype, TBool aIncludeFreetexts )
    {
    if(aUri == KNullDesC && aGuidHigh == 0 && aGuidLow == 0 && aId == KNoId)
    	{
    	User::Leave(KErrNotSupported);
    	}
        
    CMdENamespaceDef &namespacedef = aObjectDef.NamespaceDef();    
    
    CMdEObjectQuery* query = NewObjectQueryL(namespacedef,aObjectDef,NULL);
    CleanupStack::PushL(query);
    
    query->SetResultMode(EQueryResultModeItem);
        
    if(aId != KNoId)
    	{
    	CMdEObjectCondition& cond = query->Conditions().AddObjectConditionL( aId );
    	cond.SetConfidentialityLevel( EObjectConditionLevelIgnoreConfidentiality );
    	}
    else if(aGuidHigh != 0 && aGuidLow != 0)
    	{
    	CMdEObjectCondition& cond = query->Conditions().AddObjectConditionL( aGuidHigh, aGuidLow );
    	cond.SetConfidentialityLevel( EObjectConditionLevelIgnoreConfidentiality );
    	}
    else if(aUri != KNullDesC)
    	{
    	CMdEObjectCondition& cond = query->Conditions().AddObjectConditionL( EObjectConditionCompareUri, aUri );
    	cond.SetConfidentialityLevel( EObjectConditionLevelIgnoreConfidentiality );
    	}
    else
    	{
    	User::Leave( KErrArgument );
    	}
	
	TUint32 optimizationFlags = EContainsObjectCondition;
	
	if( aIncludeFreetexts )
		{
		optimizationFlags |= EContainsFreetextCondition;
		}
	
	if( aLocktype == ELock )
		{
		optimizationFlags |= EContainsObjectLocking;
		}
	
	CMdEQueryCriteriaSerialization* buf = CMdEQueryCriteriaSerialization::NewLC( 
											   query->ResultMode(),
											   query->Type(), 
    										   query->NamespaceDef(), 
    										   &aObjectDef, 
    										   NULL, 
    										   1, //Max 1  
    										   0, // 0 offset because it's not used now
    										   optimizationFlags , 
    										   query->Conditions(), 
    										   query->OrderRules(),
    										   NULL);    

	CMdCSerializationBuffer* resbuf = iSession.DoFindSyncLC( 
		query, 
		*buf, aLocktype, 
		KMdEQueryDefaultMaxCount );

	RPointerArray<CMdEInstanceItem> items;
	CleanupClosePushL( items );	

	DeserializeQueryResultL( *resbuf, items );

	CleanupStack::Pop( &items );

   	CleanupStack::PopAndDestroy( resbuf );
	CleanupStack::PopAndDestroy( buf );
	CleanupStack::PopAndDestroy( query );

	const TInt itemsCount( items.Count() );
	if( itemsCount== 1 )
		{
		CMdEInstanceItem* item = items[0];
		
#ifdef _DEBUG
		if( !item || item->InstanceType() != EMdETypeObject )
			{
			User::Leave( KErrCorrupt );
			}
#endif
		
		items.Close();

		return (CMdEObject*)item;
		}
   	else if( itemsCount == 0 )
   		{
   		items.Close();

   		return NULL;
   		}

#ifdef _DEBUG
   	else
   		{
   		items.ResetAndDestroy();
   		items.Close();

   		User::Leave( KErrCorrupt );
   		}
#endif

	return NULL; // <-- just to stop compile warnings!!
    }

CMdEObject* CMdESessionImpl::GetObjectL( const TItemId aId, CMdENamespaceDef* aNamespaceDef )
	{
	CMdENamespaceDef* namespaceDef = GetNamespaceDefL( aNamespaceDef );
	
	CMdEObjectDef* objectDef = namespaceDef->GetObjectDefL( KBaseObjectDefId );
	if ( !objectDef )
		{
		User::Leave( KErrNotFound );
		}
	return GetObjectL(*objectDef, aId, 0, 0, KNullDesC, EGet, EFalse);	
	}
	
CMdEObject* CMdESessionImpl::GetFullObjectL( const TItemId aId, CMdENamespaceDef* aNamespaceDef )
	{
	TMdEObject object;

	CheckObjectL( object, aId, aNamespaceDef );

	if( object.NotPresent() || object.Removed() )
		{
		User::Leave( KErrNotFound );
		}
	
	const CMdEObjectDef& objectDef = object.DefL();
	
	return GetObjectL( CONST_CAST( CMdEObjectDef&, objectDef ), aId, 0, 0, KNullDesC, EGet, ETrue );
	}

CMdEObject* CMdESessionImpl::GetObjectL( const TItemId aId, CMdEObjectDef& aObjectDef )
	{
	return GetObjectL(aObjectDef, aId, 0, 0, KNullDesC, EGet, EFalse);	
	}

CMdEObject* CMdESessionImpl::GetObjectL( const TInt64 aGuidHigh, const TInt64 aGuidLow, CMdENamespaceDef* aNamespaceDef )
	{
	CMdENamespaceDef* namespaceDef = GetNamespaceDefL( aNamespaceDef );

	CMdEObjectDef* objectDef = namespaceDef->GetObjectDefL( KBaseObjectDefId );
	if ( !objectDef )
		{
		User::Leave( KErrNotFound );
		}
	return GetObjectL(*objectDef, KNoId, aGuidHigh, aGuidLow, KNullDesC, EGet, EFalse);	
	}

CMdEObject* CMdESessionImpl::GetFullObjectL( const TInt64 aGuidHigh, const TInt64 aGuidLow, CMdENamespaceDef* aNamespaceDef )
	{
	CMdEObject* object = NULL;

	object = GetObjectL( aGuidHigh, aGuidLow, aNamespaceDef );

	if ( object )
		{
		CMdEObjectDef& objectDef = object->Def();
		TItemId objId = object->Id();

		delete object;
		object = NULL;		
		object = GetObjectL( objectDef, objId, 0, 0, KNullDesC, EGet, ETrue );
		}

	return object;
	}

CMdEObject* CMdESessionImpl::GetObjectL( const TInt64 aGuidHigh, const TInt64 aGuidLow, CMdEObjectDef& aObjectDef )
	{
	return GetObjectL(aObjectDef, KNoId, aGuidHigh, aGuidLow, KNullDesC, EGet, EFalse);	
	}

CMdEObject* CMdESessionImpl::OpenObjectL( const TItemId aId, CMdENamespaceDef* aNamespaceDef )
	{
	CMdENamespaceDef* namespaceDef = GetNamespaceDefL( aNamespaceDef );
	
	CMdEObjectDef* objectDef = namespaceDef->GetObjectDefL( KBaseObjectDefId );
	if ( !objectDef )
		{
		User::Leave( KErrNotFound );
		}

	CMdEObject* object = GetObjectL(*objectDef, aId, 0, 0, KNullDesC, ELock, EFalse);
	
	if ( object && !object->OpenForModifications() )
		{
		delete object;
		object = NULL;
		User::Leave( KErrLocked );
		}

	return object;
	}

CMdEObject* CMdESessionImpl::OpenFullObjectL( const TItemId aId, CMdENamespaceDef* aNamespaceDef )
	{
	TMdEObject object;

	CheckObjectL( object, aId, aNamespaceDef );

	if( object.NotPresent() || object.Removed() )
		{
		User::Leave( KErrNotFound );
		}

	const CMdEObjectDef& objectDef = object.DefL();
	
	return GetObjectL( CONST_CAST( CMdEObjectDef&, objectDef ), aId, 0, 0, KNullDesC, ELock, ETrue );
	}

CMdEObject* CMdESessionImpl::OpenObjectL( const TItemId aId, CMdEObjectDef& aObjectDef )
    {
    CMdEObject* object = GetObjectL(aObjectDef, aId, 0, 0, KNullDesC, ELock, EFalse);

	if( object && !object->OpenForModifications() )
		{
		delete object;
		object = NULL;
		User::Leave( KErrLocked );
		}

	return object;    
    }

CMdEObject* CMdESessionImpl::OpenObjectL( const TInt64 aGuidHigh, const TInt64 aGuidLow, CMdENamespaceDef* aNamespaceDef )
	{
	CMdENamespaceDef* namespaceDef = GetNamespaceDefL( aNamespaceDef );
	
	CMdEObjectDef* objectDef = namespaceDef->GetObjectDefL( KBaseObjectDefId );
	if ( !objectDef )
		{
		User::Leave( KErrNotFound );
		}

	CMdEObject* object = GetObjectL(*objectDef, KNoId, aGuidHigh, aGuidLow, KNullDesC, ELock, EFalse);

	if( object && !object->OpenForModifications() )
		{
		delete object;
		object = NULL;
		User::Leave( KErrLocked );
		}

	return object;
	}

CMdEObject* CMdESessionImpl::OpenFullObjectL( const TInt64 aGuidHigh, const TInt64 aGuidLow, CMdENamespaceDef* aNamespaceDef )
	{
	CMdEObject* object = NULL;
	
	object = GetObjectL( aGuidHigh,  aGuidLow, aNamespaceDef );
	
	if( object )
		{
		CMdEObjectDef& objectDef = object->Def();
		TItemId objId = object->Id();

		delete object;
		object = NULL;

		object = GetObjectL( CONST_CAST( CMdEObjectDef&, objectDef ), objId, 0, 0, KNullDesC, ELock, ETrue );
		}

	return object;
	}

CMdEObject* CMdESessionImpl::OpenObjectL( const TInt64 aGuidHigh, const TInt64 aGuidLow, CMdEObjectDef& aObjectDef )
    {
    CMdEObject* object = GetObjectL(aObjectDef, KNoId, aGuidHigh, aGuidLow, KNullDesC, ELock, EFalse);

	if( object && !object->OpenForModifications() )
		{
		delete object;
		object = NULL;
		User::Leave( KErrLocked );
		}

	return object;    
    }

EXPORT_C CMdEObject* CMdESessionImpl::GetObjectL( const TDesC& aUri, CMdENamespaceDef* aNamespaceDef )
	{
	CMdENamespaceDef* namespaceDef = aNamespaceDef;

	if ( !aNamespaceDef )
		{
		namespaceDef = &GetDefaultNamespaceDefL();
		}

	CMdEObjectDef* objectDef = namespaceDef->GetObjectDefL( KBaseObjectDefId );
	if ( !objectDef )
		{
		User::Leave( KErrNotFound );
		}

	return GetObjectL(*objectDef, KNoId, 0, 0, aUri, EGet, EFalse);
	}
    
CMdEObject* CMdESessionImpl::GetFullObjectL( const TDesC& aUri, CMdENamespaceDef* aNamespaceDef )
	{
	TMdEObject object;
	
	CheckObjectL( object, aUri, aNamespaceDef );
	
	if( object.NotPresent() || object.Removed() )
		{
		User::Leave( KErrNotFound );
		}
	
	const CMdEObjectDef& objectDef = object.DefL();
	TItemId objId = object.Id();
	
	return GetObjectL( CONST_CAST( CMdEObjectDef&, objectDef ), objId, 0, 0, KNullDesC, EGet, ETrue );
	}
    
CMdEObject* CMdESessionImpl::GetObjectL( const TDesC& aUri, CMdEObjectDef& aObjectDef )
    {
    return GetObjectL( aObjectDef, KNoId, 0, 0, aUri, EGet, EFalse );
    }

    
CMdEObject* CMdESessionImpl::OpenObjectL( const TDesC& aUri, CMdENamespaceDef* aNamespaceDef )
	{
	CMdENamespaceDef* namespaceDef = aNamespaceDef;
	
	if ( !aNamespaceDef )
		{
		namespaceDef = &GetDefaultNamespaceDefL();
		}

	CMdEObjectDef* objectDef = namespaceDef->GetObjectDefL( KBaseObjectDefId );
	if ( !objectDef )
		{
		User::Leave( KErrNotFound );
		}

	CMdEObject* object = GetObjectL(*objectDef, KNoId, 0, 0, aUri, ELock, EFalse);

	if( object && !object->OpenForModifications() )
		{
		delete object;
		object = NULL;
		User::Leave( KErrLocked );
		}

	return object;
	}

CMdEObject* CMdESessionImpl::OpenFullObjectL( const TDesC& aUri, CMdENamespaceDef* aNamespaceDef )
	{
	TMdEObject object;
	
	CheckObjectL( object, aUri, aNamespaceDef );
	
	if( object.NotPresent() || object.Removed() )
		{
		User::Leave( KErrNotFound );
		}
	
	const CMdEObjectDef& objectDef = object.DefL();
	TItemId objId = object.Id();

	return GetObjectL( CONST_CAST( CMdEObjectDef&, objectDef ), objId, 0, 0, KNullDesC, ELock, ETrue );
	}

CMdEObject* CMdESessionImpl::OpenObjectL( const TDesC& aUri, CMdEObjectDef& aObjectDef )
    {
    CMdEObject* object = GetObjectL(aObjectDef, KNoId, 0, 0, aUri, ELock, EFalse);

	if( object && !object->OpenForModifications() )
		{
		delete object;
		object = NULL;
		User::Leave( KErrLocked );
		}

	return object;
    }

void CMdESessionImpl::CheckObjectL( TMdEObject& aObject, const TDesC& aUri, 
		CMdENamespaceDef* aNamespaceDef )
	{
	CMdENamespaceDef& namespaceDef = *GetNamespaceDefL( aNamespaceDef );
	
	CMdCSerializationBuffer* object = 
		CMdCSerializationBuffer::NewLC( aObject.RequiredBufferSize() );

	iSession.DoCheckObjectL( *object, aUri, namespaceDef.Id() );

	object->PositionL( KNoOffset );
	aObject.DeSerializeL( *object, namespaceDef );

	CleanupStack::PopAndDestroy( object );
	}

void CMdESessionImpl::CheckObjectL( TMdEObject& aObject, TItemId aId, 
		CMdENamespaceDef* aNamespaceDef )
	{
	CMdENamespaceDef& namespaceDef = *GetNamespaceDefL( aNamespaceDef );

	CMdCSerializationBuffer* object = 
		CMdCSerializationBuffer::NewLC( aObject.RequiredBufferSize() );

	iSession.DoCheckObjectL( *object, aId, namespaceDef.Id() );
	
	object->PositionL( KNoOffset );
	aObject.DeSerializeL( *object, namespaceDef );
	
	CleanupStack::PopAndDestroy( object );
	}

void CMdESessionImpl::CheckObjectL( RArray<TMdEObject>& aObjects, 
		const RArray<TItemId>& aIds, CMdENamespaceDef* aNamespaceDef )
	{
	CMdENamespaceDef& namespaceDef = *GetNamespaceDefL( aNamespaceDef );

	const TUint32 idCount = (TUint32)aIds.Count();
	
	CMdCSerializationBuffer* objects = 
		CMdCSerializationBuffer::NewLC( 
				CMdCSerializationBuffer::KRequiredSizeForTUint32 + 
				TMdEObject::RequiredBufferSize() * idCount );

	CMdCSerializationBuffer* ids = 
		CMdCSerializationBuffer::NewLC( 
				CMdCSerializationBuffer::KRequiredSizeForTUint32 + 
				CMdCSerializationBuffer::KRequiredSizeForTItemId * idCount );

	ids->InsertL( idCount );
	for( TUint32 i = 0; i < idCount; i++ )
		{
		ids->InsertL( aIds[i] );
		}

	iSession.DoCheckObjectL( *objects, *ids, namespaceDef.Id() );

	objects->PositionL( KNoOffset );

	TUint32 objectCount = 0;
	objects->ReceiveL( objectCount );

	aObjects.ReserveL( objectCount );

	for( TUint32 i = 0; i < objectCount; i++ )
		{
		aObjects.AppendL( TMdEObject() );
		aObjects[i].DeSerializeL( *objects, namespaceDef );
		}
	
	CleanupStack::PopAndDestroy( ids );
	CleanupStack::PopAndDestroy( objects );
	}

CMdERelation* CMdESessionImpl::GetRelationL(TItemId aId, CMdENamespaceDef* aNamespaceDef)
    {
    CMdERelationQuery* query = NewRelationQueryL( *GetNamespaceDefL( aNamespaceDef ), NULL );
    query->SetResultMode( EQueryResultModeItem );
    CleanupStack::PushL( query );

    query->Conditions().AddRelationConditionL( aId );

	CMdEQueryCriteriaSerialization* buf = CMdEQueryCriteriaSerialization::NewLC( 
											   query->ResultMode(),
											   query->Type(), 
    										   query->NamespaceDef(), 
    										   NULL, 
    										   NULL, 
    										   1, //Max 1  
    										   0, // 0 offset because it's not used now
    										   EContainsRelationCondition, 
    										   query->Conditions(), 
    										   query->OrderRules(),
    										   NULL);


	CMdCSerializationBuffer* resbuf = iSession.DoFindSyncLC(
		query, 
		*buf, EGet, 
		KMdEQueryDefaultMaxCount);

	RPointerArray<CMdEInstanceItem> items;
	CleanupClosePushL( items );

	DeserializeQueryResultL( *resbuf, items );

	CleanupStack::Pop( &items );

   	CleanupStack::PopAndDestroy(resbuf);
	CleanupStack::PopAndDestroy( buf );
	CleanupStack::PopAndDestroy( query );

	const TInt itemsCount( items.Count() );
	if( itemsCount== 1 )
		{
		CMdEInstanceItem* item = items[0];

#ifdef _DEBUG
		if ( !item || item->InstanceType() != EMdETypeRelation )
			{
			User::Leave( KErrCorrupt );
			}
#endif	
		
		items.Close();
			
		return (CMdERelation*)item;
		}
   	else if( itemsCount == 0 )
   		{
   		items.Close();
   		return NULL;
   		}
#ifdef _DEBUG
   	else
   		{
   		items.ResetAndDestroy();
   		items.Close();
   		
   		User::Leave( KErrCorrupt );
   		}
#endif

   	return NULL; // <-- just to stop compile warnings!!
    }

CMdEEvent* CMdESessionImpl::GetEventL(TItemId aId, 
		CMdENamespaceDef* aNamespaceDef)
    {
	CMdENamespaceDef* namespaceDef = aNamespaceDef;
	
	if ( !aNamespaceDef )
		{
		namespaceDef = &GetDefaultNamespaceDefL();
		}
    
	CMdEEventQuery * query = NewEventQueryL(*namespaceDef,NULL);
    query->SetResultMode(EQueryResultModeItem);
    CleanupStack::PushL(query);

	query->Conditions().AddEventConditionL(aId);
	
	CMdEQueryCriteriaSerialization* buf = CMdEQueryCriteriaSerialization::NewLC( 
											   query->ResultMode(),
											   query->Type(), 
    										   query->NamespaceDef(), 
    										   NULL, 
    										   NULL, 
    										   1, //Max 1  
    										   0, // 0 offset because it's not used now
    										   EContainsEventCondition,
    										   query->Conditions(), 
    										   query->OrderRules(),
    										   NULL);    

	CMdCSerializationBuffer* resbuf = iSession.DoFindSyncLC(
			query, 
			*buf, EGet,
			KMdEQueryDefaultMaxCount);

	RPointerArray<CMdEInstanceItem> items;
	CleanupClosePushL( items );

	DeserializeQueryResultL( *resbuf, items );

	CleanupStack::Pop( &items );

   	CleanupStack::PopAndDestroy(resbuf);
	CleanupStack::PopAndDestroy( buf );
	CleanupStack::PopAndDestroy( query );

	const TInt itemsCount( items.Count() );
	if( itemsCount == 1 )
		{
		CMdEInstanceItem* item = items[0];
		
#ifdef _DEBUG
		if ( !item || item->InstanceType() != EMdETypeEvent )
			{
			User::Leave( KErrCorrupt );
			}
#endif
		
		items.Close();
			
		return (CMdEEvent*)item;
		}
   	else if( itemsCount == 0 )
   		{
   		items.Close();
   		
   		return NULL;
   		}
#ifdef _DEBUG
	else
   		{
   		items.ResetAndDestroy();
   		items.Close();
   		
   		User::Leave( KErrCorrupt );
   		}
#endif
	
	return NULL; // <-- just to stop compile warnings!!
    }

/**
* Remove methods
*/
CMdCSerializationBuffer* CMdESessionImpl::RemoveCommonL( 
		CMdENamespaceDef& aNamespaceDef, const RArray<TItemId>* aObjects,
		const RArray<TItemId>* aEvents, const RArray<TItemId>* aRelations )
	{
	if ( !( (aObjects && aObjects->Count()) || 
			(aEvents && aEvents->Count()) || 
			(aRelations && aRelations->Count()) ) )
		{
		User::Leave( KErrArgument );
		}

	TMdCItemIds itemIds;
	itemIds.iNamespaceDefId = aNamespaceDef.Id();
	itemIds.iObjectUris.iPtr.iCount = 0;
	itemIds.iObjectUris.iPtr.iOffset = KNoOffset;

	// headerSize
	TUint32 bufferSize = sizeof(TMdCItemIds);
	
	if ( aObjects )
		{
		bufferSize += aObjects->Count() * CMdCSerializationBuffer::KRequiredSizeForTItemId;
		}
	if ( aEvents )
		{
		bufferSize += aEvents->Count() * CMdCSerializationBuffer::KRequiredSizeForTItemId;
		}
	if ( aRelations )
		{
		bufferSize += aRelations->Count() * CMdCSerializationBuffer::KRequiredSizeForTItemId;
		}

	CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( bufferSize );

	buffer->PositionL( sizeof(TMdCItemIds) );

	// insert objects
	if ( aObjects )
		{
		const TInt count = aObjects->Count();
		itemIds.iObjectIds.iPtr.iCount = count;
		itemIds.iObjectIds.iPtr.iOffset = buffer->Position();

		for ( TInt i = 0; i < count; ++i )
			{
			buffer->InsertL( (*aObjects)[i] );
			}
		}
	else
		{
		itemIds.iObjectIds.iPtr.iCount = 0;
		itemIds.iObjectIds.iPtr.iOffset = KNoOffset;
		}
	
	// insert events
	if ( aEvents )
		{
		const TInt count = aEvents->Count();
		itemIds.iEventIds.iPtr.iCount = count;
		itemIds.iEventIds.iPtr.iOffset = buffer->Position();
		
		for ( TInt i = 0; i < count; ++i )
			{
			buffer->InsertL( (*aEvents)[i] );
			}
		}
	else
		{
		itemIds.iEventIds.iPtr.iCount = 0;
		itemIds.iEventIds.iPtr.iOffset = KNoOffset;
		}

	// insert relations
	if ( aRelations )
		{
		const TInt count = aRelations->Count();
		itemIds.iRelationIds.iPtr.iCount = count;
		itemIds.iRelationIds.iPtr.iOffset = buffer->Position();
		
		for ( TInt i = 0; i < count; ++i )
			{
			buffer->InsertL( (*aRelations)[i] );
			}
		}
	else
		{
		itemIds.iRelationIds.iPtr.iCount = 0;
		itemIds.iRelationIds.iPtr.iOffset = KNoOffset;
		}

	// set up header correctly
	buffer->PositionL( KNoOffset );
	itemIds.SerializeL( *buffer );

	CleanupStack::Pop( buffer );
	return buffer;
	}

CMdCSerializationBuffer* CMdESessionImpl::RemoveCommonL( 
		CMdENamespaceDef& aNamespaceDef, 
		const RPointerArray<TDesC16>* aObjects,
		const RArray<TItemId>* aEvents, const RArray<TItemId>* aRelations )
	{
	if ( !( (aObjects && aObjects->Count()) || 
			(aEvents && aEvents->Count()) || 
			(aRelations && aRelations->Count()) ) )
		{
		User::Leave( KErrArgument );
		}

	TMdCItemIds itemIds;
	itemIds.iNamespaceDefId = aNamespaceDef.Id();
	itemIds.iObjectIds.iPtr.iCount = 0;
	itemIds.iObjectIds.iPtr.iOffset = KNoOffset;

	// headerSize
	TUint32 bufferSize = sizeof(TMdCItemIds);

	if ( aObjects )
		{
		const TInt count = aObjects->Count();
		for ( TInt i = 0; i < count; ++i )
			{
			bufferSize += CMdCSerializationBuffer::RequiredSize( *((*aObjects)[i]) );
			}
		}
	if ( aEvents )
		{
		bufferSize += aEvents->Count() * CMdCSerializationBuffer::KRequiredSizeForTItemId;
		}
	if ( aRelations )
		{
		bufferSize += aRelations->Count() * CMdCSerializationBuffer::KRequiredSizeForTItemId;
		}

	CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( bufferSize );

	buffer->PositionL( sizeof(TMdCItemIds) );

	// insert objects
	if ( aObjects )
		{
		const TInt count = aObjects->Count();
		itemIds.iObjectUris.iPtr.iCount = count;
		itemIds.iObjectUris.iPtr.iOffset = buffer->Position();

		for ( TInt i = 0; i < count; ++i )
			{
			const TDesC& uri = *((*aObjects)[i]);
			HBufC* lcUri = HBufC::NewLC( uri.Length() );
			// Note: CopyLC doesn't push anything to cleanup stack
			lcUri->Des().CopyLC( uri );
			buffer->InsertL( *lcUri );
			CleanupStack::PopAndDestroy( lcUri );
			}
		}
	else
		{
		itemIds.iObjectUris.iPtr.iCount = 0;
		itemIds.iObjectUris.iPtr.iOffset = KNoOffset;
		}
	
	// insert events
	if ( aEvents )
		{
		const TInt count = aEvents->Count();
		itemIds.iEventIds.iPtr.iCount = count;
		itemIds.iEventIds.iPtr.iOffset = buffer->Position();
		
		for ( TInt i = 0; i < count; ++i )
			{
			buffer->InsertL( (*aEvents)[i] );
			}
		}
	else
		{
		itemIds.iEventIds.iPtr.iCount = 0;
		itemIds.iEventIds.iPtr.iOffset = KNoOffset;
		}

	// insert relations
	if ( aRelations )
		{
		const TInt count = aRelations->Count();
		itemIds.iRelationIds.iPtr.iCount = count;
		itemIds.iRelationIds.iPtr.iOffset = buffer->Position();
		
		for ( TInt i = 0; i < count; ++i )
			{
			buffer->InsertL( (*aRelations)[i] );
			}
		}
	else
		{
		itemIds.iRelationIds.iPtr.iCount = 0;
		itemIds.iRelationIds.iPtr.iOffset = KNoOffset;
		}

	// set up header correctly
	buffer->PositionL( KNoOffset );
	itemIds.SerializeL( *buffer );

	CleanupStack::Pop( buffer );
	return buffer;
	}

TInt CMdESessionImpl::DeserializeIdsL( RMdEDataBuffer& aSerializedItemIds, 
		RArray<TItemId>* aResultObjects, RArray<TItemId>* aResultEvents, 
		RArray<TItemId>* aResultRelations )
	{
	CMdCSerializationBuffer* buffer = aSerializedItemIds.GetBufferLC();
	
	const TMdCItemIds& itemIds = TMdCItemIds::GetFromBufferL( *buffer );

    if ( itemIds.iObjectIds.iPtr.iCount > 0 && aResultObjects )
    	{
    	buffer->PositionL( itemIds.iObjectIds.iPtr.iOffset );
    	TItemId objectId;
    	aResultObjects->ReserveL( itemIds.iObjectIds.iPtr.iCount );
    	for (TUint32 i = 0; i < itemIds.iObjectIds.iPtr.iCount; ++i)
    		{
    		buffer->ReceiveL( objectId );
    		aResultObjects->AppendL( objectId );
    		}
    	}

    if ( itemIds.iEventIds.iPtr.iCount > 0 && aResultEvents )
    	{
    	buffer->PositionL( itemIds.iEventIds.iPtr.iOffset );
    	TItemId eventId;
    	aResultEvents->ReserveL( itemIds.iEventIds.iPtr.iCount );
    	for (TUint32 i = 0; i < itemIds.iEventIds.iPtr.iCount; ++i)
    		{
    		buffer->ReceiveL( eventId );
    		aResultEvents->AppendL( eventId );
    		}
    	}

    if ( itemIds.iRelationIds.iPtr.iCount > 0 && aResultRelations )
    	{
    	buffer->PositionL( itemIds.iRelationIds.iPtr.iOffset );
    	TItemId relationId;
    	aResultRelations->ReserveL( itemIds.iRelationIds.iPtr.iCount );
    	for (TUint32 i = 0; i < itemIds.iRelationIds.iPtr.iCount; ++i)
    		{
    		buffer->ReceiveL( relationId );
    		aResultRelations->AppendL( relationId );
    		}
    	}
    
    const TInt errorCode = itemIds.iErrorCode;
    CleanupStack::PopAndDestroy( buffer );
    
    return errorCode;
	}

TItemId CMdESessionImpl::RemoveObjectL( TItemId aId, 
		CMdENamespaceDef* aNamespaceDef )
    {
	CMdENamespaceDef* namespaceDef = aNamespaceDef;

	if ( !aNamespaceDef )
		{
		namespaceDef = &GetDefaultNamespaceDefL();
		}    

	RArray<TItemId> removeIdArray;
	CleanupClosePushL( removeIdArray );
	RArray<TItemId> resultObjectArray;
	CleanupClosePushL( resultObjectArray );
	removeIdArray.AppendL( aId );
	User::LeaveIfError( RemoveObjectsL( removeIdArray, resultObjectArray, 
			namespaceDef ) );
	TItemId result = KNoId;
	if ( resultObjectArray.Count() )
		{
		result = resultObjectArray[0];
		}
	CleanupStack::PopAndDestroy( 2, &removeIdArray ); // resultObjectArray, removeIdArray
	return result;
    }

TItemId CMdESessionImpl::RemoveObjectL( const TDesC& aUri, 
		CMdENamespaceDef* aNamespaceDef )
    {
	CMdENamespaceDef* namespaceDef = aNamespaceDef;

	if ( !aNamespaceDef )
		{
		namespaceDef = &GetDefaultNamespaceDefL();
		}

	RPointerArray<TDesC16> removeUriArray;
	CleanupClosePushL( removeUriArray );
	RArray<TItemId> resultObjectArray;
	CleanupClosePushL( resultObjectArray );
	removeUriArray.AppendL( &aUri );
	User::LeaveIfError( RemoveObjectsL( removeUriArray, resultObjectArray, 
			namespaceDef ) );
	TItemId result = KNoId;
	if ( resultObjectArray.Count() )
		{
		result = resultObjectArray[0];
		}
	CleanupStack::PopAndDestroy( 2, &removeUriArray ); // resultObjectArray, removeUriArray
	return result;
    }

TInt CMdESessionImpl::RemoveObjectsL( const RArray<TItemId>& aId, 
		RArray<TItemId>& aResult, CMdENamespaceDef* aNamespaceDef )
	{
	CMdENamespaceDef* namespaceDef = aNamespaceDef;
	
	if ( !aNamespaceDef )
		{
		namespaceDef = &GetDefaultNamespaceDefL();
		}

	CMdCSerializationBuffer* buffer = RemoveCommonL( *namespaceDef, &aId, 
			NULL, NULL  );
	CleanupStack::PushL( buffer );
	CMdCSerializationBuffer* resultBuffer = CMdCSerializationBuffer::NewLC( 
			buffer->Size() );
	RMdEDataBuffer dataBuffer;
	dataBuffer.SetBufferL( resultBuffer );
	CleanupStack::Pop( resultBuffer );
	CleanupClosePushL( dataBuffer );
	
    iSession.DoRemoveItemsL( *buffer, *resultBuffer );
    TInt32 firstItemError = DeserializeIdsL( dataBuffer, &aResult );
    CleanupStack::PopAndDestroy( 2, buffer ); // successfulBuffer, buffer
    return firstItemError;
	}

TInt CMdESessionImpl::RemoveObjectsL( const RPointerArray<TDesC>& aUri, 
		RArray<TItemId>& aResult, CMdENamespaceDef* aNamespaceDef )
	{
	CMdENamespaceDef* namespaceDef = aNamespaceDef;
	
	if ( !aNamespaceDef )
		{
		namespaceDef = &GetDefaultNamespaceDefL();
		}    

	CMdCSerializationBuffer* buffer = RemoveCommonL( *namespaceDef, &aUri, NULL, NULL  );
	CleanupStack::PushL( buffer );
    const TUint32 rbs = sizeof( TMdCItemIds )
                        + ( aUri.Count() + 2 ) * CMdCSerializationBuffer::KRequiredSizeForTItemId;

	CMdCSerializationBuffer* resultBuffer = CMdCSerializationBuffer::NewLC( rbs );
	RMdEDataBuffer dataBuffer;
	dataBuffer.SetBufferL( resultBuffer );
	CleanupStack::Pop( resultBuffer );
	CleanupClosePushL( dataBuffer );
	
    iSession.DoRemoveItemsL( *buffer, *resultBuffer );
    TInt32 firstItemError = DeserializeIdsL( dataBuffer, &aResult );
    CleanupStack::PopAndDestroy( &dataBuffer );
    CleanupStack::PopAndDestroy( buffer );
    return firstItemError;
	}

void CMdESessionImpl::RemoveObjectsAsyncL( 
		const RArray<TItemId>& aId, TRequestStatus& aStatus, 
		RMdEDataBuffer& aSerializedObjectIds, 
		CMdENamespaceDef* aNamespaceDef )
	{
	CMdENamespaceDef* namespaceDef = aNamespaceDef;

	if ( !aNamespaceDef )
		{
		namespaceDef = &GetDefaultNamespaceDefL();
		}

	CMdCSerializationBuffer* buffer = RemoveCommonL( *namespaceDef, &aId, 
			NULL, NULL  );
	CleanupStack::PushL( buffer );
	CMdCSerializationBuffer* resultBuffer = CMdCSerializationBuffer::NewLC( 
			buffer->Size() );
	aSerializedObjectIds.SetBufferL( resultBuffer );
    CleanupStack::Pop( resultBuffer );
	
    CleanupStack::Pop( buffer );

    iAsyncHandler->RemoveRequest( buffer, *resultBuffer, aStatus );
	}
	
void CMdESessionImpl::RemoveObjectsAsyncL( 
		const RPointerArray<TDesC>& aUri, TRequestStatus& aStatus, 
		RMdEDataBuffer& aSerializedObjectIds, 
		CMdENamespaceDef* aNamespaceDef )
	{
	CMdENamespaceDef* namespaceDef = aNamespaceDef;
	
	if ( !aNamespaceDef )
		{
		namespaceDef = &GetDefaultNamespaceDefL();
		}    

	CMdCSerializationBuffer* buffer = RemoveCommonL( *namespaceDef, &aUri, 
			NULL, NULL  );
	CleanupStack::PushL( buffer );
    const TUint32 rbs = sizeof( TMdCItemIds )
                        + ( aUri.Count() + 2 ) * CMdCSerializationBuffer::KRequiredSizeForTItemId;

	CMdCSerializationBuffer* resultBuffer = CMdCSerializationBuffer::NewLC( rbs );
	aSerializedObjectIds.SetBufferL( resultBuffer );
	CleanupStack::Pop( resultBuffer );
	CleanupStack::Pop( buffer );

    iAsyncHandler->RemoveRequest( buffer, *resultBuffer, aStatus );
	}

TItemId CMdESessionImpl::RemoveRelationL(TItemId aId, 
		CMdENamespaceDef* aNamespaceDef)
    {
    RArray<TItemId> items;
    CleanupClosePushL( items );
    RArray<TItemId> successful;
    CleanupClosePushL( successful );

    items.AppendL( aId );
    User::LeaveIfError( RemoveRelationsL( items, successful, aNamespaceDef ) );

    TItemId result = KNoId;
    if ( successful.Count() )
    	{
    	result = successful[0];
    	}
    CleanupStack::PopAndDestroy( 2, &items );
    return result;
    }

TInt CMdESessionImpl::RemoveRelationsL(const RArray<TItemId>& aId, 
		RArray<TItemId>& aSuccessful, CMdENamespaceDef* aNamespaceDef)
	{
	CMdENamespaceDef* namespaceDef = aNamespaceDef;

	if ( !aNamespaceDef )
		{
		namespaceDef = &GetDefaultNamespaceDefL();
		}

	CMdCSerializationBuffer* buffer = RemoveCommonL( *namespaceDef, 
			(RArray<TItemId>*)NULL, NULL, &aId  );
	CleanupStack::PushL( buffer );
	CMdCSerializationBuffer* successfulBuffer = CMdCSerializationBuffer::NewLC(
			buffer->Size() );
    iSession.DoRemoveItemsL( *buffer, *successfulBuffer );
    
	RMdEDataBuffer dataBuffer;
	dataBuffer.SetBufferL( successfulBuffer );
	CleanupStack::Pop( successfulBuffer );
	CleanupClosePushL( dataBuffer );
    
    const TInt firstItemError = DeserializeIdsL( dataBuffer, NULL, NULL, 
    		&aSuccessful );
    CleanupStack::PopAndDestroy( &dataBuffer ); // successfulBuffer, buffer
    CleanupStack::PopAndDestroy( buffer ); // successfulBuffer, buffer
    return firstItemError;
	}

void CMdESessionImpl::RemoveRelationsAsyncL(
		const RArray<TItemId>& aId, TRequestStatus& aStatus, 
		RMdEDataBuffer& aSerializedRelationIds, 
		CMdENamespaceDef* aNamespaceDef)
	{
	CMdENamespaceDef* namespaceDef = aNamespaceDef;

	if ( !aNamespaceDef )
		{
		namespaceDef = &GetDefaultNamespaceDefL();
		}

	CMdCSerializationBuffer* buffer = RemoveCommonL( *namespaceDef, 
			(RArray<TItemId>*)NULL, NULL, &aId  );
	CleanupStack::PushL( buffer );
	CMdCSerializationBuffer* resultBuffer = CMdCSerializationBuffer::NewLC( 
			buffer->Size() );
	aSerializedRelationIds.SetBufferL( resultBuffer );
    CleanupStack::Pop( resultBuffer );    
    CleanupStack::Pop( buffer );

    iAsyncHandler->RemoveRequest( buffer, *resultBuffer, aStatus );    
	}

/**
* Add methods
*/
TItemId CMdESessionImpl::AddItemL( CMdEInstanceItem& aItem )
	{
	RPointerArray<CMdEInstanceItem> items;
	CleanupClosePushL( items );
	items.AppendL( &aItem );
	User::LeaveIfError( AddItemsL( items ) );
	CleanupStack::PopAndDestroy( &items );
	return aItem.Id();
	}

CMdCSerializationBuffer* CMdESessionImpl::SerializeItemsL( 
		RPointerArray<CMdEInstanceItem>& aItems )
    {
    const TInt itemsCount = aItems.Count();
	if ( itemsCount == 0 )
		{
		User::Leave(KErrArgument);
		return NULL;
		}

    TInt requiredBufferSize = sizeof(TMdCItems);

	// counting items and required buffer size
    TMdCItems items;
    items.iNamespaceDefId = KNoDefId;
    items.iErrorCode = KErrNone;
    items.iObjects.iPtr.iCount = 0;
    items.iObjects.iPtr.iOffset = 0;
    items.iRelations.iPtr.iCount = 0;
    items.iRelations.iPtr.iOffset = 0;
    items.iEvents.iPtr.iCount = 0;
    items.iEvents.iPtr.iOffset = 0;

    for ( TInt i = 0; i < itemsCount; ++i )
		{
        switch (aItems[i]->InstanceType())
        	{
			case EMdETypeObject:
	        	{
	            requiredBufferSize += static_cast<CMdEObject*>(aItems[i])->RequiredBufferSize();
				++items.iObjects.iPtr.iCount;
				const TDefId nmspId = static_cast<CMdEObject*>(aItems[i])->Def().NamespaceDef().Id();
				if (items.iNamespaceDefId == KNoDefId)
					{
					items.iNamespaceDefId = nmspId;
					}
				else if ( items.iNamespaceDefId != nmspId )
					{
					User::Leave(KErrArgument);
					}
	            break;
	            }
	        case EMdETypeRelation:
	        	{
	            requiredBufferSize += static_cast<CMdERelation*>(aItems[i])->RequiredBufferSize();
				++items.iRelations.iPtr.iCount;
				const TDefId nmspId = static_cast<CMdERelation*>(aItems[i])->Def().NamespaceDef().Id();
				if (items.iNamespaceDefId == KNoDefId)
					{
					items.iNamespaceDefId = nmspId;
					}
				else if ( items.iNamespaceDefId != nmspId )
					{
					User::Leave(KErrArgument);
					}
	            break;
	            }
	        case EMdETypeEvent:
	        	{
	            requiredBufferSize += static_cast<CMdEEvent*>(aItems[i])->RequiredBufferSize();
				++items.iEvents.iPtr.iCount;
				const TDefId nmspId = static_cast<CMdEEvent*>(aItems[i])->Def().NamespaceDef().Id();
				if (items.iNamespaceDefId == KNoDefId)
					{
					items.iNamespaceDefId = nmspId;
					}
				else if ( items.iNamespaceDefId != nmspId )
					{
					User::Leave(KErrArgument);
					}
	            break;
	            }
	        default:
	        	{
	        	User::Leave(KErrArgument);
	        	}
        	}
		}

	CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( requiredBufferSize );

    // move after main header
	TMdCOffset freespaceOffset = sizeof( TMdCItems );

	if (items.iObjects.iPtr.iCount)
		{
		// add objects header
		items.iObjects.iPtr.iOffset = freespaceOffset;
		freespaceOffset += items.iObjects.iPtr.iCount * sizeof( TMdCObject );
		}
	if (items.iEvents.iPtr.iCount)
		{
		// add events header
		items.iEvents.iPtr.iOffset = freespaceOffset;
		freespaceOffset += items.iEvents.iPtr.iCount * sizeof( TMdCEvent );
		}
	if (items.iRelations.iPtr.iCount)
		{
		// add relations header
		items.iRelations.iPtr.iOffset = freespaceOffset;
		freespaceOffset += items.iRelations.iPtr.iCount * sizeof( TMdCRelation );
		}

	TUint32 objectCtr = 0;
    TUint32 relationCtr = 0;
    TUint32 eventCtr = 0;
	for ( TInt i = 0; i < itemsCount; ++i )
		{
        const TUint32 actualPosition = buffer->Position();
        switch (aItems[i]->InstanceType())
        	{
			case EMdETypeObject:
	        	{
	            CMdEObject* object = static_cast<CMdEObject*>(aItems[i]);
	            // set right offset
	            buffer->PositionL( items.iObjects.iPtr.iOffset + objectCtr * sizeof(TMdCObject) );
	            freespaceOffset = object->SerializeL( *buffer, freespaceOffset );
	            ++objectCtr;
	            break;
	            }
	        case EMdETypeRelation:
	        	{
	            CMdERelation* relation = static_cast<CMdERelation*>(aItems[i]);
	            // set right offset
	            buffer->PositionL( items.iRelations.iPtr.iOffset + relationCtr * sizeof(TMdCRelation) );
	            freespaceOffset = relation->SerializeL( *buffer, freespaceOffset );
	            ++relationCtr;
	            break;
	            }
	        case EMdETypeEvent:
	        	{
	            CMdEEvent* event = static_cast<CMdEEvent*>(aItems[i]);
	            // set right offset
	            buffer->PositionL( items.iEvents.iPtr.iOffset + eventCtr * sizeof(TMdCEvent) );
	            freespaceOffset = event->SerializeL( *buffer, freespaceOffset );
	            ++eventCtr;
				break;
	            }
	        default:
	        	{
	        	User::Leave(KErrArgument);
	        	}
        	}
		}

    // insert namespaceid
	buffer->PositionL( KNoOffset );
	items.SerializeL( *buffer );

    CleanupStack::Pop( buffer );
    return buffer;
    }

void CMdESessionImpl::DeserializeQueryResultL( 
		CMdCSerializationBuffer& aBuffer, 
		RPointerArray<CMdEInstanceItem>& aItems )
	{
    CleanupResetAndDestroyPushL( aItems );
    
	const TMdCItems& items = TMdCItems::GetFromBufferL( aBuffer );

    CMdENamespaceDef& namespaceDef = GetNamespaceDefL( items.iNamespaceDefId );

    aItems.ReserveL( items.iObjects.iPtr.iCount + items.iEvents.iPtr.iCount 
    		+ items.iRelations.iPtr.iCount );
    
	if ( items.iObjects.iPtr.iCount > 0 )
		{
		for ( TUint32 i = 0; i < items.iObjects.iPtr.iCount; ++i )
			{
			aBuffer.PositionL( items.iObjects.iPtr.iOffset + i * sizeof(TMdCObject) );
			CMdEObject* object = CMdEObject::NewLC( this, aBuffer, 
					namespaceDef );
			aItems.AppendL( object );
			CleanupStack::Pop( object );
			}
		}
		
	if ( items.iEvents.iPtr.iCount > 0 )
		{
		for ( TUint32 i = 0; i < items.iEvents.iPtr.iCount; ++i )
			{
			aBuffer.PositionL( items.iEvents.iPtr.iOffset + i * sizeof(TMdCEvent) );
			CMdEEvent* event = CMdEEvent::NewLC( this, aBuffer, namespaceDef );
			aItems.AppendL( event );
			CleanupStack::Pop( event );
			}
		}

	if ( items.iRelations.iPtr.iCount > 0 )
		{
		for ( TUint32 i = 0; i < items.iRelations.iPtr.iCount; ++i )
			{
			aBuffer.PositionL( items.iRelations.iPtr.iOffset + i * sizeof(TMdCRelation) );
			CMdERelation* relation = CMdERelation::NewLC( this, aBuffer, 
					namespaceDef );
			aItems.AppendL( relation );
			CleanupStack::Pop( relation );
			}
		}
	
	CleanupStack::Pop( &aItems );
	}

TItemId CMdESessionImpl::AddObjectL( CMdEObject& aObject )
    {
    AddItemL(aObject);
    return aObject.Id();
    }

TInt CMdESessionImpl::AddObjectsL( RPointerArray<CMdEObject>& aObjects )
    {
    const TInt firstObjectError = AddItemsL( 
    		(RPointerArray<CMdEInstanceItem>&)aObjects );

    return firstObjectError;
    }

TItemId CMdESessionImpl::AddRelationL( CMdERelation& aRelation ) 
    {
	return AddItemL( aRelation );
    }

TItemId CMdESessionImpl::UpdateRelationL( CMdERelation& aRelation ) 
    {
	RPointerArray<CMdEInstanceItem> items;
	CleanupClosePushL( items );
	items.AppendL( &aRelation );
	User::LeaveIfError( UpdateItemsL( items ) );
	CleanupStack::PopAndDestroy( &items );
	return aRelation.Id();
    }

TInt CMdESessionImpl::AddItemsL( RPointerArray<CMdEInstanceItem>& aItems )
    {    
    CMdCSerializationBuffer *buffer = SerializeItemsL( aItems );
    CleanupStack::PushL( buffer );
    CMdCSerializationBuffer* resultBuf = CMdCSerializationBuffer::NewLC( 
    		+ sizeof(TMdCItemIds)
    		+ aItems.Count() * CMdCSerializationBuffer::KRequiredSizeForTItemId );
	RMdEDataBuffer dataBuffer;
	dataBuffer.SetBufferL( resultBuf );
	CleanupStack::Pop( resultBuf );
	CleanupClosePushL( dataBuffer );    
    
    iSession.DoAddItemsL( *buffer, *resultBuf );
    
	const TInt firstItemError = DeserializeItemsL( dataBuffer, aItems );

	CleanupStack::PopAndDestroy( &dataBuffer );
    CleanupStack::PopAndDestroy( buffer );
    
    return firstItemError;
    }

void CMdESessionImpl::AddItemsAsyncL( 
		RPointerArray<CMdEInstanceItem>& aItems, TRequestStatus& aStatus, 
		RMdEDataBuffer& aSerializedItemIds )
    {    
    CMdCSerializationBuffer* buffer = SerializeItemsL( aItems );
    CleanupStack::PushL( buffer );

    CMdCSerializationBuffer* resultBuf = CMdCSerializationBuffer::NewLC( 
    		+ sizeof(TMdCItemIds)
    		+ aItems.Count() * CMdCSerializationBuffer::KRequiredSizeForTItemId );
    aSerializedItemIds.SetBufferL( resultBuf );
    CleanupStack::Pop( resultBuf );    
    CleanupStack::Pop( buffer );

    iAsyncHandler->AddRequest( buffer, *resultBuf, aStatus );
    }

TInt CMdESessionImpl::UpdateItemsL( RPointerArray<CMdEInstanceItem>& aItems )
    {    
    CMdCSerializationBuffer *buffer = SerializeItemsL( aItems );
    CleanupStack::PushL( buffer );

    CMdCSerializationBuffer* resultBuf = CMdCSerializationBuffer::NewLC( 
    		+ sizeof(TMdCItemIds)
    		+ aItems.Count() * CMdCSerializationBuffer::KRequiredSizeForTItemId );

	RMdEDataBuffer dataBuffer;
	dataBuffer.SetBufferL( resultBuf );
	CleanupStack::Pop( resultBuf );
	CleanupClosePushL( dataBuffer );

    iSession.DoUpdateItemsL( *buffer, *resultBuf );

	TInt firstItemError = DeserializeItemsL( dataBuffer, aItems );

	CleanupStack::PopAndDestroy( &dataBuffer );
    CleanupStack::PopAndDestroy( buffer );
    return firstItemError;
    }      

void CMdESessionImpl::UpdateItemsAsyncL( 
		RPointerArray<CMdEInstanceItem>& aItems, TRequestStatus& aStatus,
		RMdEDataBuffer& aSerializedItemIds )
    {
    CMdCSerializationBuffer *buffer = SerializeItemsL( aItems );
    CleanupStack::PushL( buffer );
    CMdCSerializationBuffer* resultBuf = CMdCSerializationBuffer::NewLC( 
    		+ sizeof(TMdCItemIds)
    		+ aItems.Count() * CMdCSerializationBuffer::KRequiredSizeForTItemId );
    aSerializedItemIds.SetBufferL( resultBuf );
	CleanupStack::Pop( resultBuf );
	CleanupStack::Pop( buffer );

    iAsyncHandler->UpdateRequest( buffer, *resultBuf, aStatus );
    }

TInt CMdESessionImpl::DeserializeItemsL( RMdEDataBuffer& aSerializedItems, 
		RPointerArray<CMdEInstanceItem>& aItems )
	{
	CMdCSerializationBuffer* buffer = aSerializedItems.GetBufferLC();

	const TMdCItemIds& itemIds = TMdCItemIds::GetFromBufferL( *buffer );

    TUint32 objectsIndex = 0;
    TUint32 eventsIndex = 0;
    TUint32 relationsIndex = 0;
    TItemId id = 0;

    const TInt count = aItems.Count();
    
    if ( count != itemIds.iObjectIds.iPtr.iCount
    		+ itemIds.iRelationIds.iPtr.iCount + itemIds.iEventIds.iPtr.iCount )
    	{
    	User::Leave( KErrArgument );
    	}

    for ( TInt i = 0; i < count; ++i )
    	{
    	switch ( aItems[i]->InstanceType() )
    		{
    		case EMdETypeObject:
    			buffer->PositionL( itemIds.iObjectIds.iPtr.iOffset
				        + objectsIndex * CMdCSerializationBuffer::KRequiredSizeForTItemId );
				buffer->ReceiveL( id );
				aItems[i]->SetId( id );
				aItems[i]->SetSession( *this );
				static_cast<CMdEObject*>(aItems[i])->ClearObject();
				++objectsIndex;
    			break;

    		case EMdETypeEvent:
    			buffer->PositionL( itemIds.iEventIds.iPtr.iOffset
    					+ eventsIndex * CMdCSerializationBuffer::KRequiredSizeForTItemId );
				buffer->ReceiveL( id );
				aItems[i]->SetId( id );
				aItems[i]->SetSession( *this );
				++eventsIndex;
    			break;

    		case EMdETypeRelation:
    			buffer->PositionL( itemIds.iRelationIds.iPtr.iOffset
    					+ relationsIndex * CMdCSerializationBuffer::KRequiredSizeForTItemId );
    			buffer->ReceiveL( id );
				aItems[i]->SetId( id );
				aItems[i]->SetSession( *this );
				++relationsIndex;
    			break;

    		default:
    			User::Leave( KErrArgument );
    			break;
    		}
    	}
    
    const TInt errorCode = itemIds.iErrorCode;
    CleanupStack::PopAndDestroy( buffer );
    
    return errorCode;
	}

TItemId CMdESessionImpl::AddEventL( CMdEEvent& aEvent )
    {
	return AddItemL( aEvent );
    }

TItemId CMdESessionImpl::RemoveEventL( TItemId aId, 
		CMdENamespaceDef* aNamespaceDef )
    {
    RArray<TItemId> items;
    CleanupClosePushL( items );
    RArray<TItemId> successful;
    CleanupClosePushL( successful );

    items.AppendL( aId );
    User::LeaveIfError( RemoveEventsL( items, successful, aNamespaceDef ) );

    TItemId result = KNoId;
    if ( successful.Count() > 0 )
    	{
    	result = successful[0];
    	}

    CleanupStack::PopAndDestroy( &successful );
    CleanupStack::PopAndDestroy( &items );

    return result;
    }

TInt CMdESessionImpl::RemoveEventsL( const RArray<TItemId>& aId, 
		RArray<TItemId>& aSuccessful, CMdENamespaceDef* aNamespaceDef )
	{
	CMdENamespaceDef* namespaceDef = aNamespaceDef;

	if ( !aNamespaceDef )
		{
		namespaceDef = &GetDefaultNamespaceDefL();
		}

	CMdCSerializationBuffer* buffer = RemoveCommonL( *namespaceDef, 
			(RArray<TItemId>*)NULL, &aId, NULL  );
	CleanupStack::PushL( buffer );
	CMdCSerializationBuffer* successfulBuffer = CMdCSerializationBuffer::NewLC(
			buffer->Size() );
	
	RMdEDataBuffer dataBuffer;
	dataBuffer.SetBufferL( successfulBuffer );
	CleanupStack::Pop( successfulBuffer );
	CleanupClosePushL( dataBuffer );
	
    iSession.DoRemoveItemsL( *buffer, *successfulBuffer );
    const TInt firstItemError = DeserializeIdsL( dataBuffer, NULL, 
    		&aSuccessful );

    CleanupStack::PopAndDestroy( &dataBuffer );
    CleanupStack::PopAndDestroy( buffer );
    
    return firstItemError;
	}    
	
void CMdESessionImpl::RemoveEventsAsyncL( 
		const RArray<TItemId>& aId, TRequestStatus& aStatus, 
		RMdEDataBuffer& aSerializedEventIds, 
		CMdENamespaceDef* aNamespaceDef )
	{
	CMdENamespaceDef* namespaceDef = aNamespaceDef;
	
	if ( !aNamespaceDef )
		{
		namespaceDef = &GetDefaultNamespaceDefL();
		}

	CMdCSerializationBuffer* buffer = RemoveCommonL( *namespaceDef, 
			(RArray<TItemId>*)NULL, &aId, NULL  );
	CleanupStack::PushL( buffer );
	CMdCSerializationBuffer* resultBuffer = CMdCSerializationBuffer::NewLC( 
			buffer->Size() );
	aSerializedEventIds.SetBufferL( resultBuffer );
    CleanupStack::Pop( resultBuffer );    
	
    CleanupStack::Pop( buffer );

    iAsyncHandler->RemoveRequest( buffer, *resultBuffer, aStatus );    
	}
	

// Query 

CMdEObjectQuery* CMdESessionImpl::NewObjectQueryL(
		CMdENamespaceDef& aNamespaceDef, CMdEObjectDef& aObjectDef, 
		MMdEQueryObserver* aObserver)
    {
    CMdEObjectQueryImpl* query = CMdEObjectQueryImpl::NewLC( *this, 
    		aNamespaceDef, aObjectDef, NULL, iSession );
    if( aObserver )
        {
        query->AddObserverL( *aObserver );
        }
    CleanupStack::Pop( query );

    query->SetQueryId( iNextQueryId );
    iNextQueryId++;
    
    return query;
    }

CMdEObjectQuery* CMdESessionImpl::NewObjectQueryL(
		CMdEObjectDef& aObjectDef, RPointerArray<CMdEObjectDef>* aObjectDefs, 
		MMdEQueryObserver* aObserver)
	{
	CleanupStack::PushL( aObjectDefs );

	if( !aObjectDefs || ( aObjectDefs->Count() <= 0 ) )
		{
		User::Leave( KErrArgument );
		}
	
    CMdEObjectQueryImpl* query = CMdEObjectQueryImpl::NewLC( *this, 
    		aObjectDef.NamespaceDef(), aObjectDef, aObjectDefs, iSession );
    if( aObserver )
        {
        query->AddObserverL( *aObserver );
        }
    CleanupStack::Pop( query );

    CleanupStack::Pop( aObjectDefs );

    query->SetQueryId( iNextQueryId );
    iNextQueryId++;
    
    return query;
	}

CMdERelationQuery* CMdESessionImpl::NewRelationQueryL(
		CMdENamespaceDef& aNamespaceDef, MMdEQueryObserver* aObserver)
    {
    CMdERelationQueryImpl* query = CMdERelationQueryImpl::NewLC( *this,
    		aNamespaceDef, iSession );

    if( aObserver )
        {
        query->AddObserverL( *aObserver );
        }
    CleanupStack::Pop( query );

    query->SetQueryId( iNextQueryId );
    iNextQueryId++;
    
    return query;
    }

CMdEEventQuery* CMdESessionImpl::NewEventQueryL(
		CMdENamespaceDef& aNamespaceDef, MMdEQueryObserver* aObserver)
    {
    CMdEEventQueryImpl* query = CMdEEventQueryImpl::NewLC( *this,
    		aNamespaceDef, iSession );

    if( aObserver )
        {
        query->AddObserverL( *aObserver );
        }
    CleanupStack::Pop( query );

    query->SetQueryId( iNextQueryId );
    iNextQueryId++;
    
    return query;
    }


// Observer handling
void CMdESessionImpl::AddObjectObserverL( MMdEObjectObserver& aObserver,
    									  CMdELogicCondition* aCondition, 
     									  TUint32 aNotificationType,
    									  CMdENamespaceDef* aNamespaceDef )
    {
    CleanupStack::PushL( aCondition );
    
    // if condition is given, check that it is correct type
	if( aCondition && ( EConditionTypeLogic != aCondition->Type() ) )
		{
		User::Leave( KErrArgument );
		}

	// if namespace is not given get default namespace definition
    CMdENamespaceDef* namespaceDef = NULL;
    if ( !aNamespaceDef )
    	{
    	namespaceDef = &GetDefaultNamespaceDefL();
    	}
    else
    	{
    	namespaceDef = aNamespaceDef;
    	}

	TUint32 type = 0;
	if ( aNotificationType & ENotifyAdd )
		{
		type |= EObjectNotifyAdd;
		}
	if ( aNotificationType & ENotifyModify )
		{
		type |= EObjectNotifyModify;
		}
	if ( aNotificationType & ENotifyRemove )
		{
		type |= EObjectNotifyRemove;
		}

    TInt err = FindNotifier( type, &aObserver, *namespaceDef );
    
    if ( err != KErrNotFound )
    	{
    	if ( err >= 0 )
    		{
    		err = KErrAlreadyExists;
    		}
    	User::LeaveIfError( err );
    	}

    CMdENotifierAO* notifier = CMdENotifierAO::NewLC( *this, iSession );
    notifier->RegisterL( type, &aObserver, aCondition, *namespaceDef );

    iNotifiers.AppendL( notifier );
    CleanupStack::Pop( notifier );
    
    CleanupStack::PopAndDestroy( aCondition );
    }
    
void CMdESessionImpl::AddObjectPresentObserverL(
		MMdEObjectPresentObserver& aObserver)
	{
    CMdENamespaceDef& namespaceDef = GetDefaultNamespaceDefL();

    TInt err = FindNotifier( 
    		EObjectNotifyPresent, &aObserver, namespaceDef );
    
    if ( err != KErrNotFound )
    	{
    	if ( err >= 0 )
    		{
    		err = KErrAlreadyExists;
    		}
    	User::LeaveIfError( err );
    	}

    CMdENotifierAO* notifier = CMdENotifierAO::NewLC( *this, iSession );
    notifier->RegisterL( EObjectNotifyPresent | EObjectNotifyNotPresent,
    		&aObserver, NULL, namespaceDef );

    iNotifiers.AppendL( notifier );
    CleanupStack::Pop( notifier );
	}

void CMdESessionImpl::AddRelationObserverL( MMdERelationObserver& aObserver,
    										CMdECondition* aCondition, 
    										TUint32 aNotificationType,
    										CMdENamespaceDef* aNamespaceDef )
    {
    CleanupStack::PushL( aCondition );
    
    // if condition is given, check that it is correct type
	if( aCondition && ( EConditionTypeRelation != aCondition->Type() ) )
		{
		User::Leave( KErrArgument );
		}

	// if namespace is not given get default namespace definition
    CMdENamespaceDef* namespaceDef = NULL;
    if ( !aNamespaceDef )
    	{
    	namespaceDef = &GetDefaultNamespaceDefL();
    	}
    else
    	{
    	namespaceDef = aNamespaceDef;
    	}

	TUint32 type = 0;
	if ( aNotificationType & ENotifyAdd )
		{
		type |= ERelationNotifyAdd;
		}
	if ( aNotificationType & ENotifyModify )
		{
		type |= ERelationNotifyModify;
		}
	if ( aNotificationType & ENotifyRemove )
		{
		type |= ERelationNotifyRemove;
		}

	TInt err = FindNotifier( type, &aObserver, *namespaceDef );

    if ( err != KErrNotFound )
    	{
    	if ( err >= 0 )
    		{
    		err = KErrAlreadyExists;
    		}
    	User::LeaveIfError( err );
    	}
    
    CMdENotifierAO* notifier = CMdENotifierAO::NewLC( *this, iSession );
    notifier->RegisterL( type, &aObserver, aCondition, *namespaceDef );

    iNotifiers.AppendL( notifier );
    CleanupStack::Pop( notifier );
    
    CleanupStack::PopAndDestroy( aCondition );
    }

void CMdESessionImpl::AddRelationItemObserverL( 
		MMdERelationItemObserver& aObserver, CMdECondition* aCondition, 
		TUint32 aNotificationType, CMdENamespaceDef* aNamespaceDef )
	{
    CleanupStack::PushL( aCondition );
    
	// if condition is given, check that it is correct type
	if( aCondition && ( EConditionTypeRelation != aCondition->Type() ) )
		{
		User::Leave( KErrArgument );
		}
	
	// if namespace is not given get default namespace definition
	CMdENamespaceDef* namespaceDef = NULL;
	if ( !aNamespaceDef )
		{
		namespaceDef = &GetDefaultNamespaceDefL();
		}
	else
		{
		namespaceDef = aNamespaceDef;
		}
	
	TUint32 type = 0;
	if ( aNotificationType & ENotifyAdd )
		{		
		User::Leave( KErrNotSupported );
		}
	if ( aNotificationType & ENotifyModify )
		{		
		User::Leave( KErrNotSupported );
		}
	if ( aNotificationType & ENotifyRemove )
		{
		type |= ERelationItemNotifyRemove;
		}
	
	TInt err = FindNotifier( type, &aObserver, *namespaceDef );
	
	if ( err != KErrNotFound )
		{
		if ( err >= 0 )
			{
			err = KErrAlreadyExists;
			}
		User::LeaveIfError( err );
		}
	
	CMdENotifierAO* notifier = CMdENotifierAO::NewLC( *this, iSession );
	notifier->RegisterL( type, &aObserver, aCondition, *namespaceDef );
	
	iNotifiers.AppendL( notifier );
	CleanupStack::Pop( notifier );
	
    CleanupStack::PopAndDestroy( aCondition );
	}


void CMdESessionImpl::AddRelationPresentObserverL(
		MMdERelationPresentObserver& aObserver)
	{
    CMdENamespaceDef& namespaceDef = GetDefaultNamespaceDefL();

    TInt err = FindNotifier( 
    		ERelationNotifyPresent | ERelationNotifyNotPresent,
    		&aObserver, namespaceDef );
    
    if ( err != KErrNotFound )
    	{
    	if ( err >= 0 )
    		{
    		err = KErrAlreadyExists;
    		}
    	User::LeaveIfError( err );
    	}
    
    CMdENotifierAO* notifier = CMdENotifierAO::NewLC( *this, iSession );
    notifier->RegisterL( ERelationNotifyPresent | ERelationNotifyNotPresent,
    		&aObserver, NULL, namespaceDef );

    iNotifiers.AppendL( notifier );
    CleanupStack::Pop( notifier );
	}

void CMdESessionImpl::AddEventObserverL( MMdEEventObserver& aObserver,
    									 CMdECondition* aCondition, 
    									 TUint32 aNotificationType,
    									 CMdENamespaceDef* aNamespaceDef )
    {
    CleanupStack::PushL( aCondition );
    
    // if condition is given, check that it is correct type
	if( aCondition && ( EConditionTypeEvent != aCondition->Type() ) )
		{
		User::Leave( KErrArgument );
		}

	if ( aNotificationType & ENotifyModify )
		{
		User::Leave( KErrNotSupported );
		}

	// if namespace is not given get default namespace definition
    CMdENamespaceDef* namespaceDef = NULL;
    if ( !aNamespaceDef )
    	{
    	namespaceDef = &GetDefaultNamespaceDefL();
    	}
    else
    	{
    	namespaceDef = aNamespaceDef;
    	}

	TUint32 type = 0;
	if ( aNotificationType & ENotifyAdd )
		{
		type |= EEventNotifyAdd;
		}
	if ( aNotificationType & ENotifyRemove )
		{
		type |= EEventNotifyRemove;
		}
	
    TInt err = FindNotifier( type, &aObserver, *namespaceDef );
    
    if ( err != KErrNotFound )
    	{
    	if ( err >= 0 )
    		{
    		err = KErrAlreadyExists;
    		}
    	User::LeaveIfError( err );
    	}

    CMdENotifierAO* notifier = CMdENotifierAO::NewLC( *this, iSession );
    notifier->RegisterL( type, &aObserver, aCondition, *namespaceDef );

    iNotifiers.AppendL( notifier );
    CleanupStack::Pop( notifier );
    
    CleanupStack::PopAndDestroy( aCondition );
    }

void CMdESessionImpl::RemoveObjectObserverL( 
		MMdEObjectObserver& aObserver, CMdENamespaceDef* aNamespaceDef )
    {
	// if namespace is not given get default namespace definition
    CMdENamespaceDef* namespaceDef = NULL;
    if ( !aNamespaceDef )
    	{
    	namespaceDef = &GetDefaultNamespaceDefL();
    	}
    else
    	{
    	namespaceDef = aNamespaceDef;
    	}

    const TInt index = FindNotifier( 
    		EObjectNotifyAdd | EObjectNotifyModify | EObjectNotifyRemove,
    		&aObserver, *namespaceDef );
    if ( index != KErrNotFound )
        {
    	delete iNotifiers[index];
    	iNotifiers[index] = NULL;
    	iNotifiers.Remove( index );
    	iNotifiers.Compress();
        }
    else
    	{
    	User::Leave( KErrNotFound );
    	}
    }

void CMdESessionImpl::RemoveObjectPresentObserverL(
		MMdEObjectPresentObserver& aObserver)
	{
	// if namespace is not given get default namespace definition
    CMdENamespaceDef& namespaceDef = GetDefaultNamespaceDefL();

    const TInt index = FindNotifier( EObjectNotifyPresent | EObjectNotifyNotPresent,
    		&aObserver, namespaceDef );
    if ( index != KErrNotFound )
        {
    	delete iNotifiers[index];
    	iNotifiers[index] = NULL;
    	iNotifiers.Remove( index );
    	iNotifiers.Compress();
        }
    else
    	{
    	User::Leave( KErrNotFound );
    	}
	}

void CMdESessionImpl::RemoveRelationObserverL( 
		MMdERelationObserver& aObserver, CMdENamespaceDef* aNamespaceDef )
    {
	// if namespace is not given get default namespace definition
    CMdENamespaceDef* namespaceDef = NULL;
    if ( !aNamespaceDef )
    	{
    	namespaceDef = &GetDefaultNamespaceDefL();
    	}
    else
    	{
    	namespaceDef = aNamespaceDef;
    	}

    const TInt index = FindNotifier( 
    		ERelationNotifyAdd | ERelationNotifyModify | ERelationNotifyRemove,
    		&aObserver, *namespaceDef );
    if ( index != KErrNotFound )
        {
    	delete iNotifiers[index];
    	iNotifiers[index] = NULL;
    	iNotifiers.Remove( index );
    	iNotifiers.Compress();
        }
    else
    	{
    	User::Leave( KErrNotFound );
    	}
   }

void CMdESessionImpl::RemoveRelationItemObserverL( 
		MMdERelationItemObserver& aObserver, CMdENamespaceDef* aNamespaceDef )
    {
	// if namespace is not given get default namespace definition
    CMdENamespaceDef* namespaceDef = NULL;
    if ( !aNamespaceDef )
    	{
    	namespaceDef = &GetDefaultNamespaceDefL();
    	}
    else
    	{
    	namespaceDef = aNamespaceDef;
    	}

    const TInt index = FindNotifier( 
    		/*ERelationItemNotifyAdd | ERelationItemNotifyModify |*/ 
    		ERelationItemNotifyRemove,
    		&aObserver, *namespaceDef );
    if ( index != KErrNotFound )
        {
    	delete iNotifiers[index];
    	iNotifiers[index] = NULL;
    	iNotifiers.Remove( index );
        }
    else
    	{
    	User::Leave( KErrNotFound );
    	}
   }

void CMdESessionImpl::RemoveRelationPresentObserverL(
		MMdERelationPresentObserver& aObserver)
	{
	// if namespace is not given get default namespace definition
    CMdENamespaceDef& namespaceDef = GetDefaultNamespaceDefL();

    const TInt index = FindNotifier( 
    		ERelationNotifyPresent | ERelationNotifyNotPresent,
    		&aObserver, namespaceDef );
    if ( index != KErrNotFound )
        {
    	delete iNotifiers[index];
    	iNotifiers[index] = NULL;
    	iNotifiers.Remove( index );
    	iNotifiers.Compress();
        }
    else
    	{
    	User::Leave( KErrNotFound );
    	}
	}

void CMdESessionImpl::RemoveEventObserverL( 
		MMdEEventObserver& aObserver, CMdENamespaceDef* aNamespaceDef )
    {
	// if namespace is not given get default namespace definition
    CMdENamespaceDef* namespaceDef = NULL;
    if ( !aNamespaceDef )
    	{
    	namespaceDef = &GetDefaultNamespaceDefL();
    	}
    else
    	{
    	namespaceDef = aNamespaceDef;
    	}
    
    const TInt index = FindNotifier( EEventNotifyAdd | EEventNotifyRemove,
    		&aObserver, *namespaceDef );
    if ( index != KErrNotFound )
        {
	    delete iNotifiers[index];
	    iNotifiers[index] = NULL;
	    iNotifiers.Remove( index );
        }
    else
    	{
    	User::Leave( KErrNotFound );
    	}
    }

TInt CMdESessionImpl::FindNotifier( TUint32 aNotifyType, TAny* aObserver, 
									CMdENamespaceDef& aNamespaceDef )
    {
    for( TInt i = iNotifiers.Count() - 1; i >=0; i-- )
        {
        if ( iNotifiers[i]->Match( aNotifyType, aObserver, aNamespaceDef ) )
        	{
        	return i;
        	}
        }
    return KErrNotFound;
    }

void CMdESessionImpl::NotifierInError( CMdENotifierAO* aNotifier )
    {
    const TInt index = iNotifiers.Find( aNotifier );
    delete aNotifier;
    aNotifier = NULL;
    iNotifiers.Remove( index );
    }

void CMdESessionImpl::ImportSchemaL( const TDesC& aFileName )
    {
    iSession.DoImportSchemaL( aFileName );
    DoLoadSchemaL();  
    }
    
TInt CMdESessionImpl::ImportMetadataL( const TDesC& aFileName )
    {
    return iSession.DoImportMetadataL( aFileName );
    }

void CMdESessionImpl::ImportMetadata( const TDesC& aFileName, 
		TPckgBuf<TInt>& aResult, TRequestStatus& aStatus )
	{
	return iSession.DoImportMetadata( aFileName, aResult, aStatus );
	}

CMdCSerializationBuffer* CMdESessionImpl::ExportCommonL( 
		const CMdENamespaceDef* aNamespaceDef,
		const RPointerArray<CMdEObjectDef>* aObjectDefs,
        const RPointerArray<CMdERelationDef>* aRelationDefs,
        const RPointerArray<CMdEEventDef>* aEventDefs )
	{
	// headerSize
	TUint32 bufferSize = sizeof(TMdCItemIds);
	
	TMdCItemIds itemIds;
	if ( aNamespaceDef )
		{
		itemIds.iNamespaceDefId = aNamespaceDef->Id();
		}
	else
		{
		itemIds.iNamespaceDefId = KNoDefId;
		}
	itemIds.iObjectUris.iPtr.iCount = 0;
	itemIds.iObjectUris.iPtr.iOffset = KNoOffset;
	itemIds.iObjectIds.iPtr.iCount = 0;
	itemIds.iObjectIds.iPtr.iOffset = KNoOffset;
	itemIds.iEventIds.iPtr.iCount = 0;
	itemIds.iEventIds.iPtr.iOffset = KNoOffset;
	itemIds.iRelationIds.iPtr.iCount = 0;
	itemIds.iRelationIds.iPtr.iOffset = KNoOffset;

	if ( !aNamespaceDef || (!aObjectDefs && !aRelationDefs && !aEventDefs) )
		{
		CMdCSerializationBuffer* buffer = 
				CMdCSerializationBuffer::NewLC( bufferSize );
		itemIds.SerializeL( *buffer );
		CleanupStack::Pop( buffer );
		return buffer;
		}

	if ( aObjectDefs )
		{
		bufferSize += aObjectDefs->Count() * CMdCSerializationBuffer::KRequiredSizeForTItemId;
		}
	if ( aEventDefs )
		{
		bufferSize += aEventDefs->Count() * CMdCSerializationBuffer::KRequiredSizeForTItemId;
		}
	if ( aRelationDefs )
		{
		bufferSize += aRelationDefs->Count() * CMdCSerializationBuffer::KRequiredSizeForTItemId;
		}

	CMdCSerializationBuffer* buffer = 
			CMdCSerializationBuffer::NewLC( bufferSize );

	buffer->PositionL( sizeof(TMdCItemIds) );

	// insert objects
	if ( aObjectDefs )
		{
		itemIds.iObjectIds.iPtr.iOffset = buffer->Position();
		itemIds.iObjectIds.iPtr.iCount = aObjectDefs->Count();
		
		for ( TInt i = 0; i < itemIds.iObjectIds.iPtr.iCount; ++i )
			{
			buffer->InsertL( (*aObjectDefs)[i]->Id() );
			}
		}

	// insert events
	if ( aEventDefs )
		{
		itemIds.iEventIds.iPtr.iOffset = buffer->Position();
		itemIds.iEventIds.iPtr.iCount = aEventDefs->Count();

		for ( TInt i = 0; i < itemIds.iEventIds.iPtr.iCount; ++i )
			{
			buffer->InsertL( (*aEventDefs)[i]->Id() );
			}
		}

	// insert relations
	if ( aRelationDefs )
		{
		itemIds.iRelationIds.iPtr.iOffset = buffer->Position();
		itemIds.iRelationIds.iPtr.iCount = aRelationDefs->Count();

		for ( TInt i = 0; i < itemIds.iRelationIds.iPtr.iCount; ++i )
			{
			buffer->InsertL( (*aRelationDefs)[i]->Id() );
			}
		}

	// set up header correctly
	buffer->PositionL( KNoOffset );
	itemIds.SerializeL( *buffer );

	CleanupStack::Pop( buffer );
	return buffer;
	}


void CMdESessionImpl::ExportMetadataL( const TDesC& aFileName,
    	const CMdENamespaceDef* aNamespaceDef,
	    const RPointerArray<CMdEObjectDef>* aObjectDefs,
        const RPointerArray<CMdERelationDef>* aRelationDefs,
        const RPointerArray<CMdEEventDef>* aEventDefs )
    {
	CMdCSerializationBuffer* buffer = ExportCommonL( 
			aNamespaceDef, aObjectDefs, aRelationDefs, aEventDefs );
	CleanupStack::PushL( buffer );

    // Export
    iSession.DoExportMetadataL( aFileName, *buffer );
       
    // Cleanup
	CleanupStack::PopAndDestroy( buffer );
    }

void CMdESessionImpl::ExportMetadataL( const TDesC& aFileName, 
		TRequestStatus& aStatus, RMdEDataBuffer& aBuffer, 
        const CMdENamespaceDef* aNamespaceDef, 
	    const RPointerArray<CMdEObjectDef>* aObjectDefs, 
        const RPointerArray<CMdERelationDef>* aRelationDefs, 
        const RPointerArray<CMdEEventDef>* aEventDefs )
    {
	CMdCSerializationBuffer* buffer = ExportCommonL( 
			aNamespaceDef, aObjectDefs, aRelationDefs, aEventDefs );
	CleanupStack::PushL( buffer );

    // Export
    iSession.DoExportMetadataL( aFileName, *buffer, aStatus );

    aBuffer.SetBufferL( buffer );
    CleanupStack::Pop( buffer );
    }

void CMdESessionImpl::GetSchemaVersionL(
		TInt& aMajorVersion, TInt& aMinorVersion)
	{
	return iSession.DoGetSchemaVersionL( aMajorVersion, aMinorVersion );
	}

void CMdESessionImpl::SetObjectToPresentByGuidL( 
		const TInt64& aGuidHigh, const TInt64& aGuidLow )
	{
	return iSession.DoSetObjectToPresentByGuidL( aGuidHigh, aGuidLow );
	}

void CMdESessionImpl::CheckOpened() const
	{
	__ASSERT_ALWAYS(iSessionState == EMdESessionOpen,
				    TMdEPanic::Panic(TMdEPanic::ESessionOpenInProgress));
	}

void CMdESessionImpl::GetCountL( CMdCSerializationBuffer* aBuffer, 
		TUint32& aResult )
	{
	const TMdCItemCounts& itemCounts = TMdCItemCounts::GetFromBufferL( *aBuffer );

	if( itemCounts.iObjects ) 
	    {
	    aResult = itemCounts.iObjects;
	    __ASSERT_DEBUG( ( itemCounts.iEvents == 0 ) && ( itemCounts.iRelations == 0 ), MMdCCommon::Panic( KErrCorrupt ) );
        }
	
	if( itemCounts.iEvents ) 
	    {
	    aResult = itemCounts.iEvents;
	    __ASSERT_DEBUG( ( itemCounts.iObjects == 0 ) && ( itemCounts.iRelations == 0 ), MMdCCommon::Panic( KErrCorrupt ) );
	    }

	if( itemCounts.iRelations ) 
	    {
	    aResult = itemCounts.iRelations;
	    __ASSERT_DEBUG( ( itemCounts.iObjects == 0 ) && ( itemCounts.iEvents == 0 ), MMdCCommon::Panic( KErrCorrupt ) );
	    }
	}

void CMdESessionImpl::GetItemIdL( CMdCSerializationBuffer* aBuffer, 
		RArray<TItemId>& aIdArray )
	{
    CleanupClosePushL( aIdArray );
    
	const TMdCItemIds& itemIds = TMdCItemIds::GetFromBufferL( *aBuffer );

	if( itemIds.iObjectIds.iPtr.iCount > 0 ) 
	    {
	    aBuffer->PositionL( itemIds.iObjectIds.iPtr.iOffset );
	    }
	
	if( itemIds.iRelationIds.iPtr.iCount > 0 ) 
	    {
	    aBuffer->PositionL( itemIds.iRelationIds.iPtr.iOffset );
	    }

	if( itemIds.iEventIds.iPtr.iCount > 0 ) 
	    {
	    aBuffer->PositionL( itemIds.iEventIds.iPtr.iOffset );
	    }	

	const TInt count = itemIds.iObjectIds.iPtr.iCount + itemIds.iRelationIds.iPtr.iCount
		+ itemIds.iEventIds.iPtr.iCount;
    aIdArray.ReserveL( count );
	for( TUint32 i = 0; i < count; i++ )
	    {
	    TItemId id;
	    aBuffer->ReceiveL( id );
	    aIdArray.AppendL( id );
	    }
	
	CleanupStack::Pop( &aIdArray );
	}

void CMdESessionImpl::GetDistinctValuesL( CMdCSerializationBuffer& aBuffer, 
		CDesCArray& aResults )
	{
	const TMdCItemIds& itemIds = TMdCItemIds::GetFromBufferL( aBuffer );
	aBuffer.PositionL( itemIds.iObjectUris.iPtr.iOffset );
	for ( TUint32 i = 0; i < itemIds.iObjectUris.iPtr.iCount; ++i )
		{
		TPtrC16 value = aBuffer.ReceivePtr16L();
		aResults.AppendL( value );
		}
	}

CMdENamespaceDef* CMdESessionImpl::GetNamespaceDefL( 
		CMdENamespaceDef* aNamespaceDef )
	{
    if ( aNamespaceDef )
    	{
    	return aNamespaceDef;
    	}
    else
    	{
    	return &GetDefaultNamespaceDefL();
    	}
	}
	
void CMdESessionImpl::AddObjectObserverWithUriL( MMdEObjectObserverWithUri& aObserver,
                                          CMdELogicCondition* aCondition, 
                                          TUint32 aNotificationType,
                                          CMdENamespaceDef* aNamespaceDef )
    {
    CleanupStack::PushL( aCondition );
    
    // if condition is given, check that it is correct type
    if( aCondition && ( EConditionTypeLogic != aCondition->Type() ) )
        {
        User::Leave( KErrArgument );
        }

    // if namespace is not given get default namespace definition
    CMdENamespaceDef* namespaceDef = NULL;
    if ( !aNamespaceDef )
        {
        namespaceDef = &GetDefaultNamespaceDefL();
        }
    else
        {
        namespaceDef = aNamespaceDef;
        }

    TUint32 type = 0;
    if ( aNotificationType & ENotifyAdd )
        {
        type |= EObjectNotifyAddWithUri;
        }
    if ( aNotificationType & ENotifyModify )
        {
        type |= EObjectNotifyModifyWithUri;
        }
    if ( aNotificationType & ENotifyRemove )
        {
        type |= EObjectNotifyRemoveWithUri;
        }
        
    TInt err = FindNotifier( type, &aObserver, *namespaceDef );
    
    if ( err != KErrNotFound )
        {
        if ( err >= 0 )
            {
            err = KErrAlreadyExists;
            }
        User::LeaveIfError( err );
        }

    CMdENotifierAO* notifier = CMdENotifierAO::NewLC( *this, iSession );
    notifier->RegisterL( type, &aObserver, aCondition, *namespaceDef );

    iNotifiers.AppendL( notifier );
    CleanupStack::Pop( notifier );
    
    CleanupStack::PopAndDestroy( aCondition );
    }

void CMdESessionImpl::RemoveObjectObserverWithUriL( 
        MMdEObjectObserverWithUri& aObserver, CMdENamespaceDef* aNamespaceDef )
    {
    // if namespace is not given get default namespace definition
    CMdENamespaceDef* namespaceDef = NULL;
    if ( !aNamespaceDef )
        {
        namespaceDef = &GetDefaultNamespaceDefL();
        }
    else
        {
        namespaceDef = aNamespaceDef;
        }

    const TInt index = FindNotifier( 
            EObjectNotifyAddWithUri | EObjectNotifyModifyWithUri | EObjectNotifyRemoveWithUri,
            &aObserver, *namespaceDef );
    if ( index != KErrNotFound )
        {
        delete iNotifiers[index];
        iNotifiers[index] = NULL;
        iNotifiers.Remove( index );
        iNotifiers.Compress();
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    }

