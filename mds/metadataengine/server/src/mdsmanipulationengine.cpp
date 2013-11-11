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
* Description:  This is Manipulation engine to manage adding,
*                deleting and modifying metadata database entries
*
*/

// INCLUDE FILES
#include "mdsmanipulationengine.h"

#include "mdcresult.h"
#include "mdcitem.h"
#include "mdsschema.h"
#include "mdsnotifier.h"
#include "mdsdbconnectionpool.h"
#include "mdslogger.h"
#include "mdssqlobjectmanipulate.h"
#include "mdssqliteconnection.h"
#include "mdsnamespacedef.h"
#include "mdcserializationbuffer.h"
#include "mdeinternalerror.h"
#include "mdeerror.h"
#include "mdscommoninternal.h"

// for CleanupResetAndDestroyPushL
#include <mmf/common/mmfcontrollerpluginresolver.h>

__USES_LOGGER

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CMdSManipulationEngine* CMdSManipulationEngine::NewL( CMdsSchema& aSchema,
    CMdSNotifier& aNotifier, CMdSObjectLockList& aLockList )
    {
    CMdSManipulationEngine* self = CMdSManipulationEngine::NewLC( aSchema, 
    		aNotifier, aLockList );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// NewLC
// ---------------------------------------------------------------------------
//
CMdSManipulationEngine* CMdSManipulationEngine::NewLC( CMdsSchema& aSchema,
    CMdSNotifier& aNotifier, CMdSObjectLockList& aLockList )
    {
    CMdSManipulationEngine* self = new (ELeave) CMdSManipulationEngine(
        aSchema, aNotifier, aLockList );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CMdSManipulationEngine::CMdSManipulationEngine( CMdsSchema& aSchema,
    CMdSNotifier& aNotifier, CMdSObjectLockList& aLockList )
    : iManipulate( NULL ), iSchema( aSchema ), iNotifier( aNotifier ), 
    iGarbageCollector( NULL ), iLockList( aLockList )
    {
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CMdSManipulationEngine::ConstructL()
    {
    iManipulate = CMdSSqlObjectManipulate::NewL( iSchema, iLockList );
    iGarbageCollector = CMdSGarbageCollector::NewL(*this);
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMdSManipulationEngine::~CMdSManipulationEngine()
    {
    delete iManipulate;
    iManipulate = NULL;
    
    delete iGarbageCollector;
    iGarbageCollector = NULL;
    }

// ---------------------------------------------------------------------------
// Manipulate
// ---------------------------------------------------------------------------
//
CMdSSqlObjectManipulate& CMdSManipulationEngine::Manipulate()
    {
    return *iManipulate;
    }

// ---------------------------------------------------------------------------
// GarbageCollector
// ---------------------------------------------------------------------------
//
CMdSGarbageCollector& CMdSManipulationEngine::GarbageCollector()
	{
	return *iGarbageCollector;
	}

// ---------------------------------------------------------------------------
// AddL adds objects from serialized buffer
// ---------------------------------------------------------------------------
//
void CMdSManipulationEngine::AddL( CMdCSerializationBuffer& aBuffer, 
		CMdCSerializationBuffer& aResultBuffer, 
		const CMdSServerSession* aServerSession )
	{
	const TMdCItems& items = TMdCItems::GetFromBufferL( aBuffer );

	const CMdsNamespaceDef* namespaceDef = iSchema.GetNamespaceByIdL( 
			items.iNamespaceDefId );
	if ( !namespaceDef )
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}
	iManipulate->SetNamespace( namespaceDef );

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	TMdCItemIds resultIds;
	resultIds.iNamespaceDefId = items.iNamespaceDefId;
	resultIds.iErrorCode = KErrNone;
	aResultBuffer.PositionL( sizeof(TMdCItemIds) );

	// add objects
	const TInt KObjectCount = items.iObjects.iPtr.iCount;
	if ( KObjectCount > 0 )
		{
		resultIds.iObjectIds.iPtr.iOffset = aResultBuffer.Position();
		resultIds.iObjectIds.iPtr.iCount = items.iObjects.iPtr.iCount;

        RMdsStatement baseObjStmt;
        CleanupClosePushL(baseObjStmt);
        RMdsStatement objStmt;
        CleanupClosePushL(objStmt);
		
        RMdSTransaction transaction( connection );
        CleanupClosePushL (transaction );
        const TInt beginError( transaction.Error() );
        if( beginError != KErrNone )
            {
            CleanupStack::PopAndDestroy( &transaction );
            }
    
        for ( TInt i = 0; i < KObjectCount; ++i )
            {
            aBuffer.PositionL( items.iObjects.iPtr.iOffset + i * sizeof(TMdCObject) );
            TItemId id = KNoId;
            TRAPD( err, id = iManipulate->AddObjectL( connection, aBuffer, 
                    baseObjStmt, objStmt, aServerSession ) );
            if (err == KErrNone)
                {
                aResultBuffer.InsertL( id );
                iAddedObjectsCount++;
                }
            else
                {
                aResultBuffer.InsertL( KNoId );
                if(resultIds.iErrorCode == KErrNone)
                    {
                    resultIds.iErrorCode = err;
                    }
                }
            }
        if( beginError == KErrNone )
            {
            transaction.CommitL();
            CleanupStack::PopAndDestroy( &transaction );
            }

        CleanupStack::PopAndDestroy(&objStmt);
        CleanupStack::PopAndDestroy(&baseObjStmt);
		}
	else
		{
		resultIds.iObjectIds.iPtr.iOffset = KNoOffset;
		resultIds.iObjectIds.iPtr.iCount = 0;
		}

	 // add events
    const TInt KEventCount = items.iEvents.iPtr.iCount;
    if ( KEventCount > 0 )
		{
		resultIds.iEventIds.iPtr.iOffset = aResultBuffer.Position();
		resultIds.iEventIds.iPtr.iCount = KEventCount;
	    
        RMdSTransaction transaction( connection );
        CleanupClosePushL(transaction);
        const TInt beginError( transaction.Error() );
        if( beginError != KErrNone )
            {
            CleanupStack::PopAndDestroy( &transaction );
            }

		for ( TInt i = 0; i < KEventCount; ++i )
			{
			aBuffer.PositionL( items.iEvents.iPtr.iOffset + 
					i * sizeof(TMdCEvent) );

			TItemId id = KNoId;
			TRAPD( err, id = iManipulate->AddEventL( connection, aBuffer ) );
			if (err == KErrNone)
				{
				aResultBuffer.InsertL( id );
				}
			else
				{
				aResultBuffer.InsertL( KNoId );
				if(resultIds.iErrorCode == KErrNone)
					{
					resultIds.iErrorCode = err;
					}
				}
			}

        if( beginError == KErrNone )
            {
            transaction.CommitL();
            CleanupStack::PopAndDestroy( &transaction );
            }
		}
	else
		{
		resultIds.iEventIds.iPtr.iOffset = KNoOffset;
		resultIds.iEventIds.iPtr.iCount = 0;
		}

	// add relations
    const TInt KRelationCount = items.iRelations.iPtr.iCount;
    if ( KRelationCount > 0 )
		{
		resultIds.iRelationIds.iPtr.iOffset = aResultBuffer.Position();
		resultIds.iRelationIds.iPtr.iCount = KRelationCount;
	    
        RMdSTransaction transaction( connection );
        CleanupClosePushL(transaction);
        const TInt beginError( transaction.Error() );
        if( beginError != KErrNone )
            {
            CleanupStack::PopAndDestroy( &transaction );
            }

	    for ( TInt i = 0; i < KRelationCount; ++i )
			{
			aBuffer.PositionL( items.iRelations.iPtr.iOffset + 
					i * sizeof(TMdCRelation) );

			TItemId id = KNoId;
			TRAPD( err, id = iManipulate->AddRelationL( connection, aBuffer ) );
			if (err == KErrNone)
				{
				aResultBuffer.InsertL( id );
				}
			else
				{
				aResultBuffer.InsertL( KNoId );
				if(resultIds.iErrorCode == KErrNone)
					{
					resultIds.iErrorCode = err;
					}
				}
			}

        if( beginError == KErrNone )
            {
            transaction.CommitL();
            CleanupStack::PopAndDestroy( &transaction );
            }
		}
	else
		{
		resultIds.iRelationIds.iPtr.iOffset = KNoOffset;
		resultIds.iRelationIds.iPtr.iCount = 0;
		}

	// set up result header
	aResultBuffer.PositionL( KNoOffset );
	resultIds.SerializeL( aResultBuffer );

	iManipulate->SetNamespace( NULL );
	iNotifier.NotifyAddedL( aBuffer, aResultBuffer );
	
    if( iAddedObjectsCount > KTriggerDbMaintenanceTreasholdValue )
	   {
	   RMdSTransaction transaction( connection );
	   CleanupClosePushL(transaction);
	   const TInt beginError( transaction.Error() );
	       
	   if( beginError != KErrNone )
	       {
	       CleanupStack::PopAndDestroy( &transaction );
	       }
	       
	   iManipulate->AnalyzeL();
	    
	   if( beginError == KErrNone )
	       {
	       transaction.CommitL();
	       CleanupStack::PopAndDestroy( &transaction );
	       }
	       
	   iAddedObjectsCount = 0;
	   }
	}

// ---------------------------------------------------------------------------
// RemoveL
// ---------------------------------------------------------------------------
//
void CMdSManipulationEngine::RemoveL( CMdCSerializationBuffer& aBuffer, 
		CMdCSerializationBuffer& aResultBuffer )
    {
    // Read item ids from buffer.
    const TMdCItemIds& itemIds = TMdCItemIds::GetFromBufferL( aBuffer );
    
	const CMdsNamespaceDef* namespaceDef = iSchema.GetNamespaceByIdL( 
			itemIds.iNamespaceDefId );
	if ( !namespaceDef )
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}
	iManipulate->SetNamespace( namespaceDef );

    TMdCItemIds resultIds;
	resultIds.iNamespaceDefId = itemIds.iNamespaceDefId;
	resultIds.iErrorCode = KErrNone;
	resultIds.iObjectUris.iPtr.iCount = 0;
	resultIds.iObjectUris.iPtr.iOffset = KNoOffset;

	aResultBuffer.PositionL( sizeof(TMdCItemIds) );

	RArray<TItemId> idArray;
	CleanupClosePushL( idArray );
	RArray<TItemId> removedRelations;
	CleanupClosePushL( removedRelations );
	RArray<TItemId> removedEvents;
	CleanupClosePushL( removedEvents );
	RPointerArray<HBufC> removedItemUriArray;
	CleanupResetAndDestroyPushL( removedItemUriArray );

    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    
    const TBool uriNotify = iNotifier.CheckForNotifier(EObjectNotifyRemoveWithUri);

    // Remove objects by id or URI.
    if (itemIds.iObjectIds.iPtr.iCount + itemIds.iObjectUris.iPtr.iCount > 0)
		{
	    RMdSTransaction transaction( connection );
	    CleanupClosePushL(transaction);
	    User::LeaveIfError( transaction.Error() );

		if (itemIds.iObjectUris.iPtr.iCount > 0)
			{
			aBuffer.PositionL( itemIds.iObjectUris.iPtr.iOffset );
			iManipulate->RemoveObjectsByUriL( aBuffer, itemIds.iObjectUris.iPtr.iCount,
					idArray, removedRelations, removedEvents, uriNotify, removedItemUriArray );
			}
		else
			{
		    // pause garbage collector so it does not remove the objects
			// before uris are read
		    if( iGarbageCollector && uriNotify )
		        {
		        iGarbageCollector->Pause();
		        }
			aBuffer.PositionL( itemIds.iObjectIds.iPtr.iOffset );
			iManipulate->RemoveObjectsByIdL( aBuffer, itemIds.iObjectIds.iPtr.iCount,
					idArray, removedRelations, removedEvents, uriNotify, removedItemUriArray );
			}

	    transaction.CommitL();
		CleanupStack::PopAndDestroy( &transaction );

        // write it to the buffer
        const TInt count = idArray.Count();
		resultIds.iObjectIds.iPtr.iOffset = aResultBuffer.Position();
        resultIds.iObjectIds.iPtr.iCount = count;
        
        for ( TInt i = 0; i < count; ++i )
        	{
        	aResultBuffer.InsertL( idArray[i] );
        	if (idArray[i] == KNoId && resultIds.iErrorCode==KErrNone)
        		{
        		resultIds.iErrorCode = KErrNotFound;
        		}
        	}
        idArray.Reset();
		}
    else
    	{
    	resultIds.iObjectIds.iPtr.iCount = 0;
    	resultIds.iObjectIds.iPtr.iOffset = KNoOffset;
    	}

    // Remove events by id.
	if (itemIds.iEventIds.iPtr.iCount > 0)
		{
		// process events
		aBuffer.PositionL( itemIds.iEventIds.iPtr.iOffset );

	    RMdSTransaction transaction( connection );
	    CleanupClosePushL(transaction);
	    User::LeaveIfError( transaction.Error() );

		iManipulate->RemoveEventsL( aBuffer, itemIds.iEventIds.iPtr.iCount, idArray );

	    transaction.CommitL();
		CleanupStack::PopAndDestroy( &transaction );

        const TInt count = idArray.Count();
        resultIds.iEventIds.iPtr.iOffset = aResultBuffer.Position();
        resultIds.iEventIds.iPtr.iCount = count;

        for ( TInt i = 0; i < count; ++i )
        	{
        	aResultBuffer.InsertL( idArray[i] );
        	if (idArray[i] == KNoId && resultIds.iErrorCode==KErrNone)
        		{
        		resultIds.iErrorCode = KErrNotFound;
        		}
        	}
		idArray.Reset();
		}
	else
		{
        resultIds.iEventIds.iPtr.iOffset = KNoOffset;
        resultIds.iEventIds.iPtr.iCount = 0;
		}

	// Remove relations by id.
	if (itemIds.iRelationIds.iPtr.iCount > 0)
		{
		// process relations
		aBuffer.PositionL( itemIds.iRelationIds.iPtr.iOffset );

	    RMdSTransaction transaction( connection );
	    CleanupClosePushL(transaction);
	    User::LeaveIfError( transaction.Error() );

		iManipulate->RemoveRelationsL( aBuffer, itemIds.iRelationIds.iPtr.iCount, idArray );

	    transaction.CommitL();
		CleanupStack::PopAndDestroy( &transaction );

        const TInt count = idArray.Count();
        resultIds.iRelationIds.iPtr.iOffset = aResultBuffer.Position();
        resultIds.iRelationIds.iPtr.iCount = count;

        for ( TInt i = 0; i < count; ++i )
        	{
        	aResultBuffer.InsertL( idArray[i] );
        	if (idArray[i] == KNoId && resultIds.iErrorCode==KErrNone)
        		{
        		resultIds.iErrorCode = KErrNotFound;
        		}
        	}
		}
	else
		{
        resultIds.iRelationIds.iPtr.iOffset = KNoOffset;
        resultIds.iRelationIds.iPtr.iCount = 0;
		}

	aResultBuffer.PositionL( KNoOffset );
	resultIds.SerializeL( aResultBuffer );

	TBool notify = iNotifier.CheckForNotifier(EObjectNotifyRemove|EEventNotifyRemove|ERelationNotifyRemove);
	
	// notify about items removed 
	if( uriNotify || notify )
	    {
	    iNotifier.NotifyRemovedL( aResultBuffer, EFalse, removedItemUriArray, this );
		notify = ETrue;
	    }
	
	// notify about additional items removed
	const TInt KRemovedItemsCount = removedRelations.Count() + removedEvents.Count();
    if ( notify && KRemovedItemsCount > 0 )
    	{
    	const TInt32 bufferSize = sizeof(TMdCItemIds)
							+ KRemovedItemsCount * CMdCSerializationBuffer::KRequiredSizeForTItemId;

		CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( bufferSize );

		buffer->PositionL( sizeof(TMdCItemIds) );
		
		TMdCItemIds additResultIds;
		additResultIds.iNamespaceDefId = itemIds.iNamespaceDefId;
		additResultIds.iErrorCode = KErrNone;
		additResultIds.iObjectIds.iPtr.iOffset = KNoOffset;
		additResultIds.iObjectIds.iPtr.iCount = 0;
		additResultIds.iObjectUris.iPtr.iOffset = KNoOffset;
		additResultIds.iObjectUris.iPtr.iCount = 0;

		// Insert list of removed events to the serialization buffer.
		const TInt KRemovedEventsCount = removedEvents.Count();
		additResultIds.iEventIds.iPtr.iCount = KRemovedEventsCount;
		if ( KRemovedEventsCount > 0 )
			{
			additResultIds.iEventIds.iPtr.iOffset = buffer->Position();
			for ( TInt i = 0; i < KRemovedEventsCount; ++i )
				{
				buffer->InsertL( removedEvents[i] );
				}
			}
		else
			{
			additResultIds.iEventIds.iPtr.iOffset = KNoOffset;
			}

		// Insert list of removed relations to the serialization buffer.
		const TInt KRemovedRelationsCount = removedRelations.Count();
		additResultIds.iRelationIds.iPtr.iCount = KRemovedRelationsCount;
		if ( KRemovedRelationsCount > 0 )
			{
			additResultIds.iRelationIds.iPtr.iOffset = buffer->Position();
			for ( TInt i = 0; i < KRemovedRelationsCount; ++i )
				{
				buffer->InsertL( removedRelations[i] );
				}
			}
		else
			{
			additResultIds.iRelationIds.iPtr.iOffset = KNoOffset;
			}

		buffer->PositionL( KNoOffset );
		additResultIds.SerializeL( *buffer );
		const TBool notifyEnabled = iNotifier.CheckForNotifier(EEventNotifyRemove|ERelationNotifyRemove);
		if (notifyEnabled)
		    {
	        RPointerArray<HBufC> nullArray; // For when uris are not available or needed
	        CleanupResetAndDestroyPushL( nullArray );
		    iNotifier.NotifyRemovedL( *buffer, EFalse, nullArray, this );
		    CleanupStack::PopAndDestroy( &nullArray );
		    }
		CleanupStack::PopAndDestroy( buffer );
    	}

    // notify about removed relation items
	const TInt relationsCount = removedRelations.Count() + idArray.Count();
	
    if ( relationsCount > 0 && iNotifier.CheckForNotifier(ERelationItemNotifyRemove) )
    	{
		CMdCSerializationBuffer* itemsBuffer = CMdCSerializationBuffer::NewLC( 
				sizeof( TMdCItems ) + 
				sizeof( TMdCRelation ) * relationsCount );

		iManipulate->GetRemovedRelationItemsL( *itemsBuffer, 
				removedRelations, idArray );

		iNotifier.NotifyRemovedRelationItemsL( *itemsBuffer );

		CleanupStack::PopAndDestroy( itemsBuffer );
    	}
    
	// start garbage collector
	if( iGarbageCollector )
		{
		iGarbageCollector->Start( KGarbageCollectionDelay );
		}

	// removedItemUriArray, removedEvents, removedRelations, idArray
    CleanupStack::PopAndDestroy( 4, &idArray );
    }

void CMdSManipulationEngine::UpdateL( CMdCSerializationBuffer& aBuffer, 
		CMdCSerializationBuffer& aResultBuffer )
	{
	const TMdCItems& items = TMdCItems::GetFromBufferL( aBuffer );

	const CMdsNamespaceDef* namespaceDef = iSchema.GetNamespaceByIdL( 
			items.iNamespaceDefId );
	if ( !namespaceDef )
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}
	iManipulate->SetNamespace( namespaceDef );
    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	TMdCItemIds resultIds;
	resultIds.iNamespaceDefId = items.iNamespaceDefId;
	resultIds.iErrorCode = KErrNone;
	resultIds.iObjectUris.iPtr.iOffset = KNoOffset;
	resultIds.iObjectUris.iPtr.iCount = 0;
	resultIds.iEventIds.iPtr.iOffset = KNoOffset;
	resultIds.iEventIds.iPtr.iCount = 0;

	aResultBuffer.PositionL( sizeof(TMdCItemIds) );

	// update objects
	if ( items.iObjects.iPtr.iCount > 0 )
		{
		resultIds.iObjectIds.iPtr.iOffset = aResultBuffer.Position();
		resultIds.iObjectIds.iPtr.iCount = items.iObjects.iPtr.iCount;

        RMdSTransaction transaction( connection );
        CleanupClosePushL(transaction);
        const TInt beginError( transaction.Error() );
        if( beginError != KErrNone )
            {
            CleanupStack::PopAndDestroy( &transaction );
            }
		
		for ( TInt i = 0; i < items.iObjects.iPtr.iCount; ++i )
			{
			aBuffer.PositionL( items.iObjects.iPtr.iOffset + i * sizeof(TMdCObject) );

			TItemId id = KNoId;
			TRAPD( err, id = iManipulate->UpdateObjectL( connection, aBuffer ) );
			if (err == KErrNone)
				{
				aResultBuffer.InsertL( id );
				iModifiedObjectsCount++;
				}
			else
				{
				aResultBuffer.InsertL( KNoId );
				if ( resultIds.iErrorCode == KErrNone )
					{
					resultIds.iErrorCode = err;
					}
				}
			}
        if( beginError == KErrNone )
            {
            transaction.CommitL();
            CleanupStack::PopAndDestroy( &transaction );
            }
		}
	else
		{
		resultIds.iObjectIds.iPtr.iOffset = KNoOffset;
		resultIds.iObjectIds.iPtr.iCount = 0;
		}

	// update relations
	if ( items.iRelations.iPtr.iCount > 0 )
		{
		resultIds.iRelationIds.iPtr.iOffset = aResultBuffer.Position();
		resultIds.iRelationIds.iPtr.iCount = items.iRelations.iPtr.iCount;

        RMdSTransaction transaction( connection );
        CleanupClosePushL(transaction);
        const TInt beginError( transaction.Error() );
        if( beginError != KErrNone )
            {
            CleanupStack::PopAndDestroy( &transaction );
            }
		
		for ( TInt i = 0; i < items.iRelations.iPtr.iCount; ++i )
			{
			aBuffer.PositionL( items.iRelations.iPtr.iOffset + i * sizeof(TMdCRelation) );

			TItemId id = KNoId;
			TRAPD( err, id = iManipulate->UpdateRelationsL( connection, aBuffer ) );
			if (err == KErrNone)
				{
				aResultBuffer.InsertL( id );
				}
			else
				{
				aResultBuffer.InsertL( KNoId );
				if ( resultIds.iErrorCode == KErrNone )
					{
					resultIds.iErrorCode = err;
					}
				}
			}
        if( beginError == KErrNone )
            {
            transaction.CommitL();
            CleanupStack::PopAndDestroy( &transaction );
            }
		}
	else
		{
		resultIds.iRelationIds.iPtr.iOffset = KNoOffset;
		resultIds.iRelationIds.iPtr.iCount = 0;
		}

	if ( items.iEvents.iPtr.iCount > 0 )
		{
		// events cannot be updated
		// so just ignore it, but if possible return error code
		if ( resultIds.iErrorCode == KErrNone )
			{
			resultIds.iErrorCode = KErrArgument;
			}
		}

	iManipulate->SetNamespace( NULL );

	// set up result header
	aResultBuffer.PositionL( KNoOffset );
	resultIds.SerializeL( aResultBuffer );

	iNotifier.NotifyModifiedL( aBuffer, aResultBuffer );
	
    if( iModifiedObjectsCount > KTriggerDbMaintenanceTreasholdValue )
       {
       RMdSTransaction transaction( connection );
       CleanupClosePushL(transaction);
       const TInt beginError( transaction.Error() );
           
       if( beginError != KErrNone )
           {
           CleanupStack::PopAndDestroy( &transaction );
           }
           
       iManipulate->AnalyzeL();
        
       if( beginError == KErrNone )
           {
           transaction.CommitL();
           CleanupStack::PopAndDestroy( &transaction );
           }
           
       iModifiedObjectsCount = 0;
       }
    }

CMdCSerializationBuffer* CMdSManipulationEngine::CheckObjectL( 
		TInt aResultBufferSize, const TDesC& aUri, TDefId aNamespaceDefId )
	{
    return iManipulate->CheckObjectL( 
    		aResultBufferSize, aUri, aNamespaceDefId );
	}

CMdCSerializationBuffer* CMdSManipulationEngine::CheckObjectL( 
		TInt aResultBufferSize, TItemId aId, TDefId aNamespaceDefId )
	{
    return iManipulate->CheckObjectL( 
    		aResultBufferSize, aId, aNamespaceDefId );
	}

CMdCSerializationBuffer* CMdSManipulationEngine::CheckObjectL( 
		TInt aResultBufferSize, CMdCSerializationBuffer& aIds, 
		TDefId aNamespaceDefId )
	{
	return iManipulate->CheckObjectL( 
			aResultBufferSize, aIds, aNamespaceDefId );
	}

// ---------------------------------------------------------------------------
// AddMemoryCardL
// ---------------------------------------------------------------------------
//
void CMdSManipulationEngine::AddMemoryCardL(TUint32 aMediaId)
	{
	iManipulate->AddMemoryCardL( aMediaId );
	}

// ---------------------------------------------------------------------------
// GetMemoryCardL
// ---------------------------------------------------------------------------
//
void CMdSManipulationEngine::GetMemoryCardL(TUint32& aMediaId)
	{
	iManipulate->GetMemoryCardL( aMediaId );
	}

// ---------------------------------------------------------------------------
// CheckMemoryCardL
// ---------------------------------------------------------------------------
//
TBool CMdSManipulationEngine::CheckMemoryCardL(TUint32 aMediaId)
	{
	return iManipulate->CheckMemoryCardL( aMediaId );
	}

// ---------------------------------------------------------------------------
// SetMediaL
// ---------------------------------------------------------------------------
//
void CMdSManipulationEngine::SetMediaL(TUint32 aMediaId, TChar aDrive, 
		TBool aPresentState)
	{
	return iManipulate->SetMediaL( aMediaId, aDrive, aPresentState );
	}
	
// ---------------------------------------------------------------------------
// GetMediaL
// ---------------------------------------------------------------------------
//
TBool CMdSManipulationEngine::GetMediaL(TUint32 aMediaId, TChar& aDrive, 
		TBool& aPresentState)
	{
	return iManipulate->GetMediaL( aMediaId, aDrive, aPresentState );
	}

// ---------------------------------------------------------------------------
// GetPresentMediasL
// ---------------------------------------------------------------------------
//
TInt32 CMdSManipulationEngine::GetPresentMediasL(TDes8& aMediaInfoBuffer)
	{
	return iManipulate->GetPresentMediasL( aMediaInfoBuffer );
	}

// ---------------------------------------------------------------------------
// SetFilesToPresentL
// ---------------------------------------------------------------------------
//
void CMdSManipulationEngine::SetFilesToPresentL(TUint32 aMediaId, TUint32 aFileCount, 
		CMdCSerializationBuffer& aUris, CMdCSerializationBuffer& aFileInfos,
		CMdCSerializationBuffer& aResults)
	{

    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    RMdSTransaction transaction( connection );
	CleanupClosePushL( transaction );
	User::LeaveIfError( transaction.Error() );

    RArray<TItemId> itemIds;
	CleanupClosePushL( itemIds );
    itemIds.ReserveL( aFileCount );

    RArray<TItemId> notifyItemIds;
	CleanupClosePushL( notifyItemIds );
	notifyItemIds.ReserveL( aFileCount );

    for( TUint32 i = 0; i < aFileCount; i++ )
    	{
    	TPtrC16 uri = aUris.ReceivePtr16L();
    	TPtr16 uriLC( CONST_CAST( TUint16*, uri.Ptr() ), uri.Length(), uri.Length() );
    	uriLC.LowerCase();

    	TMdSFileInfo fileInfo;
    	aFileInfos.ReceiveL(fileInfo.iModifiedTime);
    	aFileInfos.ReceiveL(fileInfo.iSize);

#ifdef _DEBUG    	
    	const TInt64 time = fileInfo.iModifiedTime;
        RDebug::Print( _L("CMdSManipulationEngine::SetFilesToPresentL: (%d) uri %S, iSize %u, iModified %Ld"),
        		i,
        		&uri,
        		fileInfo.iSize,
        		time );
#endif

    	TFilePresentStates placeHolder;
    	TBool notPresentState( EFalse );
    	const TItemId objectId = iManipulate->SearchNotPresentFileL( 
    			aMediaId, uri, fileInfo, placeHolder, notPresentState );
    	if ( placeHolder != EMdsNotFound )
    		{
    		itemIds.Append( objectId );

    		if( notPresentState )
    			{
    			notifyItemIds.Append( objectId );
    			}
    		}

    	aResults.InsertL( (TUint8)placeHolder );
    	}

	iManipulate->SetFilesToPresentL( itemIds );

	// only notify about objects in not present state and
	// modify and notify relations related to those
	if( notifyItemIds.Count() > 0 )
		{
		iNotifier.NotifyObjectPresent( ETrue, notifyItemIds );

		RArray<TItemId> relationIds;
		CleanupClosePushL( relationIds );

	    const TInt itemIdCount = notifyItemIds.Count();
		for( TUint32 i = 0; i < itemIdCount; i++ )
			{
			iManipulate->SetRelationsToPresentL( notifyItemIds[i], relationIds );
			}

		iNotifier.NotifyRelationPresent( ETrue, relationIds );

		CleanupStack::PopAndDestroy( &relationIds );
		}

	transaction.CommitL();

	CleanupStack::PopAndDestroy( &notifyItemIds );
	CleanupStack::PopAndDestroy( &itemIds );

	CleanupStack::PopAndDestroy( &transaction );
	}

// ---------------------------------------------------------------------------
// SetFilesToNotPresentL
// ---------------------------------------------------------------------------
//
void CMdSManipulationEngine::SetFilesToNotPresentL(TUint32 aMediaId, TBool aStartUp)
	{
    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    RMdSTransaction transaction( connection );
	CleanupClosePushL( transaction );
	User::LeaveIfError( transaction.Error() );
	
	RArray<TItemId> objectIds;
	CleanupClosePushL( objectIds );

	iManipulate->SetFilesToNotPresentL( aMediaId, aStartUp, objectIds );
	
	// if start up no need for object notifications, 
	// setting relations to not present and relation notifications
	if( !aStartUp )
		{
		iNotifier.NotifyObjectPresent( EFalse, objectIds );
		
		RArray<TItemId> relationIds;
		CleanupClosePushL( relationIds );
	
        iManipulate->SetRelationsToNotPresentL( aMediaId, relationIds );
		iNotifier.NotifyRelationPresent( EFalse, relationIds );
		
		CleanupStack::PopAndDestroy( &relationIds );
		}
    
    transaction.CommitL();

    CleanupStack::PopAndDestroy( &objectIds );
    CleanupStack::PopAndDestroy( &transaction );
	}

// ---------------------------------------------------------------------------
// RemoveFilesNotPresentL
// ---------------------------------------------------------------------------
//
void CMdSManipulationEngine::RemoveFilesNotPresentL(TUint32 aMediaId, TBool aStartUp)
	{
    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    RMdSTransaction transaction( connection );
	CleanupClosePushL( transaction );
	User::LeaveIfError( transaction.Error() );
	
	if( aStartUp )
		{
		RArray<TItemId> objectIds;
		CleanupClosePushL( objectIds );

		iManipulate->RemoveFilesNotPresentL( aMediaId, &objectIds );
		
		if( objectIds.Count() > 0 )
			{
			iNotifier.NotifyRemovedL( objectIds, this );
			}
		
		CleanupStack::PopAndDestroy( &objectIds );
		}
	else
		{
		iManipulate->RemoveFilesNotPresentL( aMediaId, NULL );
		}

    transaction.CommitL();
    CleanupStack::PopAndDestroy( &transaction );

	// start garbage collector
	if( iGarbageCollector )
		{
		iGarbageCollector->Start( KGarbageCollectionDelay );
		}
	}

// ---------------------------------------------------------------------------
// StartGarbageCollection
// ---------------------------------------------------------------------------
//
TBool CMdSManipulationEngine::StartGarbageCollectionL()
	{
#ifdef _DEBUG
	RDebug::Print( _L("CMdSManipulationEngine::StartGarbageCollection()") );
#endif

    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    RMdSTransaction transaction( connection );
	CleanupClosePushL( transaction );
	User::LeaveIfError( transaction.Error() );
	
    TBool again = EFalse;
#ifdef _DEBUG
    TRAPD( err, again = iManipulate->GarbageCollectionL() );
#else
    TRAP_IGNORE( again = iManipulate->GarbageCollectionL() );
#endif

    transaction.CommitL();
    CleanupStack::PopAndDestroy( &transaction ); 
    
	#ifdef _DEBUG
	if( err != KErrNone )
		{
		_LIT( KErrLog, "CMdSManipulationEngine::StartGarbageCollection error: %d" );
		RDebug::Print( KErrLog, err );		
		}
	#endif
	
	return again;
	}

// ---------------------------------------------------------------------------
// GetSchemaVersionL
// ---------------------------------------------------------------------------
//
void CMdSManipulationEngine::GetSchemaVersionL(
		TInt& aMajorVersion, TInt& aMinorVersion)
	{
	iManipulate->GetSchemaVersionL( aMajorVersion, aMinorVersion );
	}

void CMdSManipulationEngine::SetObjectToPresentByGuidL( 
		const TInt64& aGuidHigh, const TInt64& aGuidLow )
	{
	iManipulate->SetObjectToPresentByGuidL( aGuidHigh, aGuidLow );
	}

void CMdSManipulationEngine::ChangePathL(const TDesC& aOldPath, const TDesC& aNewPath)
	{
	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    RMdSTransaction transaction( connection );
	CleanupClosePushL( transaction );
	User::LeaveIfError( transaction.Error() );
	
	RArray<TItemId> objectIds;
	CleanupClosePushL( objectIds );

	iManipulate->ChangePathL( aOldPath, aNewPath, objectIds );

	iNotifier.NotifyModifiedL( objectIds, this );
	
	CleanupStack::PopAndDestroy( &objectIds );
	
	transaction.CommitL();
    CleanupStack::PopAndDestroy( &transaction );
	}

void CMdSManipulationEngine::ChangeMediaIdL()
	{
	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    RMdSTransaction transaction( connection );
	CleanupClosePushL( transaction );
	User::LeaveIfError( transaction.Error() );
	
	iManipulate->ChangeMediaIdL();
	
	transaction.CommitL();

	CleanupStack::PopAndDestroy( &transaction );
	}

void CMdSManipulationEngine::CheckMassStorageMediaIdL( const TUint32 aMediaId )
    {
    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    RMdSTransaction transaction( connection );
    CleanupClosePushL( transaction );
    const TInt beginError( transaction.Error() );
    if( beginError != KErrNone )
        {
        CleanupStack::PopAndDestroy( &transaction );
        }
    
    iManipulate->CheckMassStorageMediaIdL( aMediaId );
    
    if( beginError == KErrNone )
        {
        transaction.CommitL();
        CleanupStack::PopAndDestroy( &transaction );
        }
    }

void CMdSManipulationEngine::AddRelationDefL( TDefId aNamespaceId, const TDesC& aRelationDefName )
	{
	CMdsNamespaceDef* namespaceDef = CONST_CAST( CMdsNamespaceDef*, iSchema.GetNamespaceByIdL( aNamespaceId ) );
	if ( !namespaceDef )
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}
	if ( namespaceDef->GetReadOnly() )
		{
		User::Leave( KErrLocked );
		}

	namespaceDef->AddRelationDefL( aRelationDefName );
	iSchema.StoreToDBL();
	}

void CMdSManipulationEngine::AddEventDefL( TDefId aNamespaceId, const TDesC& aEventDefName )
	{
	CMdsNamespaceDef* namespaceDef = CONST_CAST( CMdsNamespaceDef*, iSchema.GetNamespaceByIdL( aNamespaceId ) );
	if ( !namespaceDef )
		{
		User::Leave( KErrMdEUnknownNamespaceDef );
		}
	if ( namespaceDef->GetReadOnly() )
		{
		User::Leave( KErrLocked );
		}

	namespaceDef->AddEventDefL( aEventDefName, 1 );
	iSchema.StoreToDBL();
	}

void CMdSManipulationEngine::SetPendingL(const RArray<TItemId>& aObjectIds)
	{
	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
	RMdSTransaction transaction( connection );
	CleanupClosePushL( transaction );
	User::LeaveIfError( transaction.Error() );

	iManipulate->SetPendingL( aObjectIds, EFalse );

	transaction.CommitL();
    CleanupStack::PopAndDestroy( &transaction );
	}

void CMdSManipulationEngine::ResetPendingL(const RArray<TItemId>& aObjectIds)
	{
	iManipulate->SetPendingL( aObjectIds, ETrue );
	}

TInt CMdSManipulationEngine::GetPendingCountL( TDefId aObjectDefId )
	{
	return iManipulate->GetPendingCountL( aObjectDefId );
	}

TInt CMdSManipulationEngine::GetPendingL( TDefId aObjectDefId, 
		TInt aBufferSize, RArray<TItemId>& aObjectIds )
	{
	return iManipulate->GetPendingL( aObjectDefId, aBufferSize, aObjectIds );
	}

void CMdSManipulationEngine::GetObjectUrisByIdsL( const RArray<TItemId>& aObjectIds, 
                                                                                   RPointerArray<HBufC>& aUriArray )
    {
    const TInt count( aObjectIds.Count() );
    for( TInt i( 0 ); i < count; i++ )
        {
        TUint32 flags;
        HBufC* uri = iManipulate->SearchObjectUriByIdL( aObjectIds[i], flags );
        aUriArray.AppendL( uri );
        uri = NULL;
        }
    }

