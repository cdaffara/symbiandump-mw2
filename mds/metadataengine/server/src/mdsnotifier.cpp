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
* Description:  Notifier engine / server side*
*/

#include "mdsnotifier.h"

#include "mdcresult.h"
#include "mdcitem.h"
#include "mdsserversession.h"
#include "mdsnotifycomparator.h"
#include "mdslogger.h"
#include "mdcserializationbuffer.h"
#include "mdsmanipulationengine.h"
#include "mdccommon.pan"

// for CleanupResetAndDestroyPushL
#include <mmf/common/mmfcontrollerpluginresolver.h>

__USES_LOGGER

// ------------------------------------------------
// NewL
// ------------------------------------------------
//
CMdSNotifier* CMdSNotifier::NewL()
    {
    CMdSNotifier* that = CMdSNotifier::NewLC();
    CleanupStack::Pop( that );
    return that;
    }


// ------------------------------------------------
// NewLC
// ------------------------------------------------
//
CMdSNotifier* CMdSNotifier::NewLC()
    {
    CMdSNotifier* that = new(ELeave) CMdSNotifier();
    CleanupStack::PushL( that );
    that->ConstructL();
    return that;
    }


// ------------------------------------------------
// Constructor
// ------------------------------------------------
//
CMdSNotifier::CMdSNotifier()
    {
    
    }

// ------------------------------------------------
// ConstructL
// ------------------------------------------------
//
void CMdSNotifier::ConstructL()
    {
    iComparator = CMdSNotifyComparator::NewL();
    }

// ------------------------------------------------
// Destructor
// ------------------------------------------------
//
CMdSNotifier::~CMdSNotifier()
    {
    delete iComparator;
    iComparator = NULL;
    
    const TInt count = iEntries.Count();
    
    for ( TInt i = 0; i < count; ++i )
        {
        TEntry& e = iEntries[i];
            
        if ( e.iSerializedCondition )
            {
            delete e.iSerializedCondition;
            e.iSerializedCondition = NULL;
            }
        if ( e.iDataBuffer )
            {
            delete e.iDataBuffer;
            e.iDataBuffer = NULL;
            }
        }
    
    iEntries.Close();
    }

// ------------------------------------------------
// Constructor
// ------------------------------------------------
//
CMdSNotifier::TEntry::TEntry( TInt aId,
    TConditionType aType,
    CMdCSerializationBuffer* aSerializedBuffer,
    TDefId aNamespaceDefId, 
    CMdSServerSession& aSession, 
    TBool aConfidential)
    : iId( aId )
    , iType( aType )
    , iNamespaceDefId(aNamespaceDefId)
    , iSerializedCondition( aSerializedBuffer )
    , iSession( aSession )
    , iConfidential(aConfidential)    
    {
    iDataBuffer = NULL;
    iRemoteSizeMsgSlot = KErrNotFound;
    }

// ------------------------------------------------
// TriggerL completes the client message and sends the data size
// ------------------------------------------------
//
void CMdSNotifier::TEntry::TriggerL(
	TUint32 aCompleteCode,
    const RArray<TItemId>& aIdArray,
    RPointerArray<HBufC>& aUriArray )
    {
    const TInt remoteSizeMsgSlot = iRemoteSizeMsgSlot;
    iRemoteSizeMsgSlot = KErrNotFound;

    __ASSERT_DEBUG( !iDataBuffer, MMdCCommon::Panic( KErrCorrupt ) );

    if( iMessage.IsNull() )
        {
        return;
        }
    
    if( aIdArray.Count() )
		{
		if( iType & ( EObjectNotifyAddWithUri | EObjectNotifyModifyWithUri | EObjectNotifyRemoveWithUri  ) )
		    {
		    iDataBuffer = CopyToBufferL( aIdArray, aUriArray );
		    }
		else
		    {
		    iDataBuffer = CopyToBufferL( aIdArray );
		    }
	    iSession.SizeToRemoteL( iMessage, remoteSizeMsgSlot, iDataBuffer->Size());
		}
	else
		{
	    iSession.SizeToRemoteL( iMessage, remoteSizeMsgSlot, 0 );
		}

    __LOG2( ELogServer, "<- Notify trigger %d (%d)", iId, aCompleteCode );
    iMessage.Complete( aCompleteCode );
    }

// ------------------------------------------------
// TriggerRelationItemsL completes the client
//                  message and sends the data size
// ------------------------------------------------
//
void CMdSNotifier::TEntry::TriggerRelationItemsL(
	TUint32 aCompleteCode,
	CMdCSerializationBuffer& aBuffer,
	const RArray<TItemId>& aRelationIdArray)
    {
    const TInt remoteSizeMsgSlot = iRemoteSizeMsgSlot;
    iRemoteSizeMsgSlot = KErrNotFound;

    __ASSERT_DEBUG( !iDataBuffer, MMdCCommon::Panic( KErrCorrupt ) );

    if( iMessage.IsNull() )
        {
        return;
        }
    
    if(aRelationIdArray.Count())
		{
		iDataBuffer = CopyItemsToBufferL( aBuffer, aRelationIdArray );
	    iSession.SizeToRemoteL( iMessage, remoteSizeMsgSlot, iDataBuffer->Size());
		}
	else
		{
	    iSession.SizeToRemoteL( iMessage, remoteSizeMsgSlot, 0);
		}

    __LOG2( ELogServer, "<- Notify trigger %d (%d)", iId, aCompleteCode );
    iMessage.Complete( aCompleteCode );
    }

// ------------------------------------------------
// TriggerSchemaL sends a schema notification
// ------------------------------------------------
//
void CMdSNotifier::TEntry::TriggerSchema()
    {    
    if( iMessage.IsNull() )
        {
        return;
        }
    
    iRemoteSizeMsgSlot = KErrNotFound;
    iMessage.Complete( ESchemaModify );
    }

// ------------------------------------------------
// TriggerError send a error message to the client
// ------------------------------------------------
//
void CMdSNotifier::TEntry::TriggerError( TInt aErrorCode )
    {
    iRemoteSizeMsgSlot = KErrNotFound;
	delete iDataBuffer;
    iDataBuffer = NULL;
    __LOG2( ELogServer, "<- Notify trigger %d (%d)", iId, aErrorCode );

    if( !iMessage.IsNull() )
    	{
    	iMessage.Complete( aErrorCode );
    	}
    }

// ------------------------------------------------
// CopyToBufferL copies id to buffer
// ------------------------------------------------
//
CMdCSerializationBuffer* CMdSNotifier::TEntry::CopyToBufferL(const RArray<TItemId>& aIdArray)
	{
	// IDs are always stored in object ID, 
	// even if those are actually relation or event IDs

	const TUint32 count = aIdArray.Count();

	CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC(
			sizeof( TMdCItemIds )
			+ count * CMdCSerializationBuffer::KRequiredSizeForTItemId );

	TMdCItemIds itemIds;
	itemIds.iNamespaceDefId = NamespaceDefId();
	itemIds.iObjectIds.iPtr.iCount = count;
	itemIds.iObjectIds.iPtr.iOffset = sizeof(TMdCItemIds);
	itemIds.SerializeL( *buffer );

	for( TInt i = 0; i < count; ++i )
		{
		buffer->InsertL( aIdArray[i] );
		}

	CleanupStack::Pop( buffer );
	return buffer;	
	}

// ------------------------------------------------
// CopyToBufferL copies ids and uris to buffer
// ------------------------------------------------
//
CMdCSerializationBuffer* CMdSNotifier::TEntry::CopyToBufferL(const RArray<TItemId>& aIdArray,
                                                                                                  const RPointerArray<HBufC>& aUriArray)
    {
    const TUint32 count = aIdArray.Count();
    const TUint32 uriCount = aUriArray.Count();

    TInt urisSize = CMdCSerializationBuffer::KRequiredSizeForTUint32;

    for( TInt i = uriCount - 1; i >=0; i-- )
        {
        urisSize += CMdCSerializationBuffer::RequiredSize( *aUriArray[i] );
        }
    
    TInt idsSize( sizeof( TMdCItemIds ) + count * CMdCSerializationBuffer::KRequiredSizeForTItemId );
    
    CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC(
            urisSize + idsSize );
    
    TMdCItemIds itemIds;
    itemIds.iNamespaceDefId = NamespaceDefId();
    itemIds.iObjectIds.iPtr.iCount = count;
    itemIds.iObjectIds.iPtr.iOffset = sizeof(TMdCItemIds);
    itemIds.SerializeL( *buffer );

    // Insert IDs
    for( TInt i = 0; i < count; ++i )
        {
        buffer->InsertL( aIdArray[i] );
        }
    
    // Insert uri count
    buffer->InsertL( uriCount );
    
    // Insert uris
    for( TInt i = 0; i < uriCount; ++i )
        {
        // add uri
        const TDesC& uri =  *aUriArray[i];
        HBufC* lcUri = HBufC::NewLC( uri.Length() );
        lcUri->Des().Append( uri );
        buffer->InsertL( *lcUri );
        CleanupStack::PopAndDestroy( lcUri ); 
        lcUri = NULL;
        }

    CleanupStack::Pop( buffer );
    return buffer;  
    }

// ------------------------------------------------
// CopyItemsToBufferL copies relation items to buffer
// ------------------------------------------------
//
CMdCSerializationBuffer* CMdSNotifier::TEntry::CopyItemsToBufferL(
		CMdCSerializationBuffer& aRelationItemsBuffer, 
		const RArray<TItemId>& aIdArray)
	{
	const TUint32 count = aIdArray.Count();
	aRelationItemsBuffer.PositionL( KNoOffset );
	const TMdCItems& items = TMdCItems::GetFromBufferL( aRelationItemsBuffer );

	CMdCSerializationBuffer* buffer = NULL;
	if ( items.iRelations.iPtr.iCount == count )
		{
		buffer = CMdCSerializationBuffer::NewLC( aRelationItemsBuffer );
		}
	else
		{
		buffer = CMdCSerializationBuffer::NewLC( sizeof(TMdCItems)
				+ count * sizeof(TMdCRelation) );

		TMdCItems returnItems;
		returnItems.iNamespaceDefId = items.iNamespaceDefId;
		returnItems.iRelations.iPtr.iCount = count;
		returnItems.iRelations.iPtr.iOffset = sizeof(TMdCItems);
		buffer->PositionL( sizeof(TMdCItems) );

		for( TInt i = 0; i < items.iRelations.iPtr.iCount; ++i )
			{
			TMdCRelation relation;
			relation.DeserializeL( aRelationItemsBuffer );

			if ( aIdArray.Find( relation.iId ) >= 0 )
				{
				relation.SerializeL( *buffer );
				}
			}
		buffer->PositionL( KNoOffset );
		returnItems.SerializeL( *buffer );
		}
	
	CleanupStack::Pop( buffer );
	return buffer;
	}

// ------------------------------------------------
// CacheL caches the notification
// ------------------------------------------------
//
void CMdSNotifier::TEntry::CacheL(TUint32 aCompleteCode, 
                                                         const RArray<TItemId>& aIdArray, 
                                                         const RPointerArray<HBufC>& aUriArray)
    {
    if ( aIdArray.Count() <= 0 )
    	{
    	return;
    	}

    CMdCSerializationBuffer* data( NULL );
    
    if( iType & ( EObjectNotifyAddWithUri | EObjectNotifyModifyWithUri | EObjectNotifyRemoveWithUri  ) )
        {
        data = CopyToBufferL( aIdArray, aUriArray );
        }
    else
        {
        data = CopyToBufferL( aIdArray );
        }
    iSession.CacheNotificationL( iId, aCompleteCode, data );
    }

// ------------------------------------------------
// CacheRelationItemsL caches the notification
// ------------------------------------------------
//
void CMdSNotifier::TEntry::CacheRelationItemsL(TUint32 aCompleteCode,
		CMdCSerializationBuffer& aBuffer, 
		const RArray<TItemId>& aRelationIdArray )
    {
    CMdCSerializationBuffer* data = CopyItemsToBufferL( aBuffer, 
    		aRelationIdArray );
    iSession.CacheNotificationL(iId, aCompleteCode, data);
    }

// ------------------------------------------------
// CacheL for schema mods
// ------------------------------------------------
//
void CMdSNotifier::TEntry::CacheL(TUint32 aCompleteCode)
    {
    iSession.CacheNotificationL(iId, aCompleteCode, NULL);
    }
    
// ------------------------------------------------
// TriggerCachedL triggers a previously cached notification
// ------------------------------------------------
//
void CMdSNotifier::TEntry::TriggerCachedL(TUint32 aCompleteCode, 
		CMdCSerializationBuffer* aData)
    {
    const TInt remoteSizeMsgSlot = iRemoteSizeMsgSlot;
    iRemoteSizeMsgSlot = KErrNotFound;

    __ASSERT_DEBUG( !iDataBuffer, MMdCCommon::Panic( KErrCorrupt ) );

    if( iMessage.IsNull() )
        {
        return;
        }
    
    if( aData )
    	{
    	iSession.SizeToRemoteL( iMessage, remoteSizeMsgSlot, aData->Size());
    	}

	iDataBuffer = aData;

    __LOG2( ELogServer, "<- Notify trigger %d (%d)", iId, aCompleteCode );
    iMessage.Complete( aCompleteCode );
    }

// ------------------------------------------------
// SetupForCallback
// ------------------------------------------------
//
void CMdSNotifier::TEntry::SetupForCallback(
    RMessage2 aMessage, TInt aRemoteSizeMsgSlot )
    {
    __ASSERT_DEBUG( !IsPending(), MMdCCommon::Panic( KErrCorrupt ) );
    iMessage = aMessage;
    iRemoteSizeMsgSlot = aRemoteSizeMsgSlot;
    }

// ------------------------------------------------
// GetDataBuffer
// ------------------------------------------------
//
CMdCSerializationBuffer* CMdSNotifier::TEntry::GetDataBuffer()
    {
    CMdCSerializationBuffer* data = iDataBuffer;
    iDataBuffer = NULL;
    return data;
    }

// ------------------------------------------------
// CreateEntry creates a new notifier entry
// ------------------------------------------------
//
CMdSNotifier::TEntry& CMdSNotifier::CreateEntryL( TInt aId,
    TConditionType aType, CMdCSerializationBuffer* aSerializedBuffer,
    TDefId aNamespaceDefId, CMdSServerSession& aSession, TBool aConfidential )
    {
    TEntry entry = TEntry( aId, aType, aSerializedBuffer, aNamespaceDefId, aSession, aConfidential ); 
    User::LeaveIfError( iEntries.InsertInOrderAllowRepeats( entry, TLinearOrder<TEntry>(CMdSNotifier::Compare) ) ); 
    
    return FindEntryL( aId );
    }

// ------------------------------------------------
// FindEntry
// ------------------------------------------------
//
CMdSNotifier::TEntry& CMdSNotifier::FindEntryL( TInt aId )
    {
    CMdSNotifier::TEntry* entry = NULL;

    TInt low( 0 );
    TInt high( iEntries.Count() );
    
    while( low < high )
        {
        TInt mid( (low+high)>>1 );
        
        const TInt compare( aId - iEntries[mid].Id() );
        if( compare == 0 )
            {
            entry = &iEntries[mid];
            break;
            }
        else if( compare > 0 )
            {
            low = mid + 1;
            }
        else
            {
            high = mid;
            }
        }    

    if( !entry )
    	{
    	User::Leave( KErrNotFound );
    	}
    
    return *entry;
    }

// ------------------------------------------------
// RemoveEntryL
// ------------------------------------------------
//
void CMdSNotifier::RemoveEntryL( TInt aId )
    {
    CMdSNotifier::TEntry* e = NULL;

    TInt low( 0 );
    TInt mid( 0 );
    TInt high( iEntries.Count() );
    
    while( low < high )
        {
        mid = (low+high)>>1;
        
        const TInt compare( aId - iEntries[mid].Id() );
        if( compare == 0 )
            {
            e = &iEntries[mid];
            break;
            }
        else if( compare > 0 )
            {
            low = mid + 1;
            }
        else
            {
            high = mid;
            }
        }
    
    if( e )
        {
        if ( e->IsPending() )
            {
            e->TriggerError( KErrCancel );
            }
        if ( e->iSerializedCondition )
            {
            delete e->iSerializedCondition;
            e->iSerializedCondition = NULL;
            }
        if ( e->iDataBuffer )
            {
            delete e->iDataBuffer;
            e->iDataBuffer = NULL;
            }
        iEntries.Remove( mid );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    }

// ------------------------------------------------
// RemoveEntriesBySession
// ------------------------------------------------
//
void CMdSNotifier::RemoveEntriesBySession(
    const CMdSServerSession& aSession )
    {
    const TInt count = iEntries.Count();
    
    for ( TInt i = count; --i >= 0; )
        {
        TEntry& e = iEntries[i];
        if ( &e.iSession == &aSession ) // pointer comparision
            {
            if ( e.IsPending() )
                {
                e.TriggerError( KErrCancel );
                }
            
            delete e.iSerializedCondition;
            e.iSerializedCondition = NULL;
            delete e.iDataBuffer;
            e.iDataBuffer = NULL;
            iEntries.Remove( i );
            }
        }
    }

// ------------------------------------------------
// NotifyAdded
// ------------------------------------------------
//
void CMdSNotifier::NotifyAddedL(CMdCSerializationBuffer& aSerializedItems, 
							    CMdCSerializationBuffer& aSerializedItemIds)
    {
    const TBool uriNotify = CheckForNotifier(EObjectNotifyAddWithUri);
    TBool allMatched( EFalse );
    TBool allItemsFetched( EFalse );
    
    RArray<TItemId> allItemsIdArray;
    CleanupClosePushL( allItemsIdArray );
    RPointerArray<HBufC> allItemsUriArray;
    CleanupResetAndDestroyPushL( allItemsUriArray );
    
    for( TInt i = iEntries.Count() - 1; i >=0; i-- )
        {
        TEntry& e = iEntries[i];

        if ( ! (e.iType & ( EObjectNotifyAdd | ERelationNotifyAdd | EEventNotifyAdd | EObjectNotifyAddWithUri ) ) )
        	{
        	continue;
        	}
        
        RArray<TItemId> matchingItemIdArray;
        RPointerArray<HBufC> matchingItemUriArray;
   		CleanupClosePushL( matchingItemIdArray );
   		CleanupResetAndDestroyPushL( matchingItemUriArray );

		aSerializedItems.PositionL( KNoOffset );
		aSerializedItemIds.PositionL( KNoOffset );

		const TBool someMatches = iComparator->MatchL( e.NamespaceDefId(), e.iType, e.Condition(), 
										    		 aSerializedItems, aSerializedItemIds, 
											    	 matchingItemIdArray,
											    	 allItemsIdArray,
												     matchingItemUriArray,
												     allItemsUriArray,
						     						 e.AllowConfidential(),
							     					 uriNotify,
								    				 allMatched,
								    				 allItemsFetched );

        if( someMatches ) // check if there is some matches
            {
            if( e.IsPending() )
            	{
            	TInt err( KErrNone );
            	// match found. trigger notifier entry !
            	if( allMatched )
            	    {
                    TRAP( err, e.TriggerL( EObjectNotifyAdd | ERelationNotifyAdd | EEventNotifyAdd | EObjectNotifyAddWithUri,
                            allItemsIdArray, allItemsUriArray ) );  
            	    }
            	else
            	    {
                    TRAP( err, e.TriggerL( EObjectNotifyAdd | ERelationNotifyAdd | EEventNotifyAdd | EObjectNotifyAddWithUri,
                            matchingItemIdArray, matchingItemUriArray ) );  
            	    }
                if( err != KErrNone )
                    {
                    e.TriggerError( err );
                    }
            	}
            else
            	{
                if( allMatched )
                    {
                    TRAP_IGNORE( e.CacheL( EObjectNotifyAdd | ERelationNotifyAdd | EEventNotifyAdd | EObjectNotifyAddWithUri,
                             allItemsIdArray, allItemsUriArray ) );    
                    }
                else
                    {
                    TRAP_IGNORE( e.CacheL( EObjectNotifyAdd | ERelationNotifyAdd | EEventNotifyAdd | EObjectNotifyAddWithUri,
                             matchingItemIdArray, matchingItemUriArray ) );    
                    }
            	}
            }

   		CleanupStack::PopAndDestroy( 2, &matchingItemIdArray ); // matchingItemIdArray, matchingItemUriArray
        }
    CleanupStack::PopAndDestroy( 2, &allItemsIdArray ); // allItemsIdArray, allItemsUriArray
    }

// ------------------------------------------------
// NotifyRemoved
// ------------------------------------------------
//
void CMdSNotifier::NotifyRemovedL(CMdCSerializationBuffer& aSerializedItemIds, 
								  TBool aItemIsConfidential,
								  RPointerArray<HBufC>& aRemovedItemUriArray,
								  CMdSManipulationEngine* aMEngine )
    {
	aSerializedItemIds.PositionL( KNoOffset );

	const TMdCItemIds& itemIds = TMdCItemIds::GetFromBufferL( aSerializedItemIds );

    RArray<TItemId> objectIdArray;
	CleanupClosePushL( objectIdArray );
    RArray<TItemId> eventIdArray;
	CleanupClosePushL( eventIdArray );
    RArray<TItemId> relationIdArray;
	CleanupClosePushL( relationIdArray );

    //get removed item IDs
	if( itemIds.iObjectIds.iPtr.iCount > 0 )
		{
		aSerializedItemIds.PositionL( itemIds.iObjectIds.iPtr.iOffset );

    	objectIdArray.ReserveL( itemIds.iObjectIds.iPtr.iCount );
    	for( TUint32 i = 0; i < itemIds.iObjectIds.iPtr.iCount; i++ )
    		{
    		TItemId objectId;
    		aSerializedItemIds.ReceiveL( objectId );
    		if ( objectId != KNoId )
    			{
    			objectIdArray.Append( objectId );
    			}
    		}
		}
	if( itemIds.iEventIds.iPtr.iCount > 0 )
		{
		aSerializedItemIds.PositionL( itemIds.iEventIds.iPtr.iOffset );

    	eventIdArray.ReserveL( itemIds.iEventIds.iPtr.iCount );
    	for( TUint32 i = 0; i < itemIds.iEventIds.iPtr.iCount; i++ )
    		{
    		TItemId eventId;
    		aSerializedItemIds.ReceiveL( eventId );
    		if ( eventId != KNoId )
    			{
    			eventIdArray.Append( eventId );
    			}
    		}
		}
	if( itemIds.iRelationIds.iPtr.iCount > 0 )
		{
		aSerializedItemIds.PositionL( itemIds.iRelationIds.iPtr.iOffset );

    	relationIdArray.ReserveL( itemIds.iRelationIds.iPtr.iCount );
    	for( TUint32 i = 0; i < itemIds.iRelationIds.iPtr.iCount; i++ )
    		{
    		TItemId relationId;
    		aSerializedItemIds.ReceiveL( relationId );
    		if ( relationId != KNoId )
    			{
    			relationIdArray.Append( relationId );
    			}
    		}
		}

	const TInt objectCount( objectIdArray.Count() );
	const TInt eventCount( eventIdArray.Count() );
	const TInt relationCount( relationIdArray.Count() );
	if( objectCount != 0 
			|| eventCount != 0 
			|| relationCount != 0 )
		{
		for( TInt i = iEntries.Count() - 1; i >=0; i-- )
	        {
	        TEntry& e = iEntries[i];
	        
	        // if namespace definition IDs don't match skip listener entry
	        if( e.NamespaceDefId() != itemIds.iNamespaceDefId )
	        	{
	        	continue;
	        	}

	        if(aItemIsConfidential && !e.AllowConfidential())
	        	{
	        	continue;	
	        	}

	        RPointerArray<HBufC> uriArray;
	        CleanupResetAndDestroyPushL( uriArray );
	        
	        if( e.iType & EObjectNotifyRemove && objectCount > 0 )
	            {
	            // collect matching object IDs
	            RArray<TItemId> matchingObjectIdArray;
				CleanupClosePushL( matchingObjectIdArray );
	
	            const TBool allMatches = iComparator->MatchObjectIdsL( e.Condition(),
	            		objectIdArray, matchingObjectIdArray );
	
				// check is there any matches
				if( allMatches || matchingObjectIdArray.Count() > 0 )
	            	{
	            	if(e.IsPending())
	            		{
		            	// Match found. Trigger notifier entry.
		            	TInt err( KErrNone );
		            	
		            	if( allMatches )
		            		{
		            		// all matches so send whole object ID array
		            		TRAP( err, e.TriggerL( EObjectNotifyRemove, 
		            				objectIdArray, uriArray ) );
		            		}
		            	else
		            		{
		            		TRAP( err, e.TriggerL( EObjectNotifyRemove, 
		            				matchingObjectIdArray, uriArray ) );
		            		}
	
		            	if( err != KErrNone )
			            	{
			            	e.TriggerError( err );
		    	        	}
	            		}
	            	else
	            		{
						if( allMatches )
		            		{
		            		// all matches so send whole object ID array
	            			TRAP_IGNORE( e.CacheL( EObjectNotifyRemove, 
	            					objectIdArray, uriArray ) );
		            		}
		            	else
		            		{
		            		TRAP_IGNORE( e.CacheL( EObjectNotifyRemove, 
		            				matchingObjectIdArray, uriArray ) );
		            		}
	            		}
	            	}
	
				CleanupStack::PopAndDestroy( &matchingObjectIdArray );
				}
	        else if( e.iType & EObjectNotifyRemoveWithUri && objectCount > 0 )
                {
                // collect matching object IDs
                RArray<TItemId> matchingObjectIdArray;
                CleanupClosePushL( matchingObjectIdArray );
    
                const TBool allMatches = iComparator->MatchObjectIdsL( e.Condition(),
                        objectIdArray, matchingObjectIdArray );
    
                // check is there any matches
                if( allMatches || matchingObjectIdArray.Count() > 0 )
                    {
                    if(e.IsPending())
                        {
                        // Match found. Trigger notifier entry.
                        TInt err( KErrNone );
                        
                        if( allMatches )
                            {
                            // all matches so send whole object ID array
                            TRAP( err, e.TriggerL( EObjectNotifyRemoveWithUri, 
                                    objectIdArray, aRemovedItemUriArray ) );
                            }
                        else
                            {
                            aMEngine->GetObjectUrisByIdsL( objectIdArray, uriArray );
                            TRAP( err, e.TriggerL( EObjectNotifyRemoveWithUri, 
                                    matchingObjectIdArray, uriArray ) );
                            }
    
                        if( err != KErrNone )
                            {
                            e.TriggerError( err );
                            }
                        }
                    else
                        {
                        if( allMatches )
                            {
                            // all matches so send whole object ID array
                            TRAP_IGNORE( e.CacheL( EObjectNotifyRemoveWithUri, 
                                    objectIdArray, aRemovedItemUriArray ) );
                            }
                        else
                            {
                            aMEngine->GetObjectUrisByIdsL( objectIdArray, uriArray );
                            TRAP_IGNORE( e.CacheL( EObjectNotifyRemoveWithUri, 
                                    matchingObjectIdArray, uriArray ) );
                            }
                        }
                    }
    
                CleanupStack::PopAndDestroy( &matchingObjectIdArray );
                }
	        else if( ( e.iType & EEventNotifyRemove ) 
	        		&& eventCount > 0 )
            	{
				// event condition can't contain ID conditions, 
            	// so get all IDs
	        	if(e.IsPending())
	        		{
	            	// Match found. Trigger notifier entry.
	            	TRAPD( err, e.TriggerL( EEventNotifyRemove, 
	            			eventIdArray, uriArray ) );
	            	if( err != KErrNone )
		            	{
		            	e.TriggerError( err );
	    	        	}
	        		}
	        	else
	        		{
	        		TRAP_IGNORE( e.CacheL( EEventNotifyRemove, 
	        				eventIdArray, uriArray ) );
	        		}
            	}
	        else if( ( e.iType & ERelationNotifyRemove ) 
	        		&& relationCount > 0 )
            	{
	            // relation condition can't contain ID conditions, 
            	// so get all IDs
	        	if(e.IsPending())
	        		{
	            	// Match found. Trigger notifier entry.
	            	TRAPD( err, e.TriggerL( ERelationNotifyRemove, 
	            			relationIdArray, uriArray ) );
	            	if( err != KErrNone )
		            	{
		            	e.TriggerError( err );
	    	        	}
	        		}
	        	else
	        		{
	        		TRAP_IGNORE( e.CacheL( ERelationNotifyRemove, 
	        				relationIdArray, uriArray ) );
	        		}
            	}
	        CleanupStack::PopAndDestroy( &uriArray );
	        }
		}
	CleanupStack::PopAndDestroy( 3, &objectIdArray ); // relationIdArray, eventIdArray, objectIdArray
    }

// ------------------------------------------------
// NotifyModified
// ------------------------------------------------
//
void CMdSNotifier::NotifyModifiedL(CMdCSerializationBuffer& aSerializedItems, 
							       CMdCSerializationBuffer& aSerializedItemIds)
    {
    const TBool uriNotify = CheckForNotifier(EObjectNotifyModifyWithUri);
    TBool allMatched( EFalse );
    TBool allItemsFetched( EFalse );

    RArray<TItemId> allItemsIdArray;
    CleanupClosePushL( allItemsIdArray );
    RPointerArray<HBufC> allItemsUriArray;
    CleanupResetAndDestroyPushL( allItemsUriArray );
    
    for( TInt i = iEntries.Count() - 1; i >=0; i-- )
        {
        TEntry& e = iEntries[i];

        if ( ! (e.iType & ( EObjectNotifyModify | ERelationNotifyModify | EObjectNotifyModifyWithUri ) ) )
        	{
        	continue;
        	}
  
        RArray<TItemId> matchingObjectIdArray;
        RPointerArray<HBufC> matchingItemUriArray;
		CleanupClosePushL( matchingObjectIdArray );
		CleanupResetAndDestroyPushL( matchingItemUriArray );

		aSerializedItems.PositionL( KNoOffset );
		aSerializedItemIds.PositionL( KNoOffset );

		const TBool someMatches = iComparator->MatchL( e.NamespaceDefId(), e.iType, e.Condition(), 
                                          aSerializedItems, aSerializedItemIds, 
                                          matchingObjectIdArray,
                                          allItemsIdArray,
                                          matchingItemUriArray,
                                          allItemsUriArray,
                                          e.AllowConfidential(),
                                          uriNotify,
                                          allMatched,
                                          allItemsFetched );

        if( someMatches ) // check if there is some matches
            {
            if( e.IsPending() )
            	{
            	TInt err( KErrNone );
                // match found. trigger notifier entry !
                if( allMatched )
                    {
                    TRAP( err, e.TriggerL( EObjectNotifyModify | ERelationNotifyModify | EObjectNotifyModifyWithUri,
                            allItemsIdArray, allItemsUriArray ) );  
                    }
                else
                    {
                    TRAP( err, e.TriggerL( EObjectNotifyModify | ERelationNotifyModify | EObjectNotifyModifyWithUri,
                            matchingObjectIdArray, matchingItemUriArray ) );  
                    }
                if( err != KErrNone )
                    {
                    e.TriggerError( err );
                    }         
            	}
            else
            	{
                if( allMatched )
                    {
                    TRAP_IGNORE( e.CacheL( EObjectNotifyModify | ERelationNotifyModify | EObjectNotifyModifyWithUri,
                             allItemsIdArray, allItemsUriArray ) );    
                    }
                else
                    {
                    TRAP_IGNORE( e.CacheL( EObjectNotifyModify | ERelationNotifyModify | EObjectNotifyModifyWithUri,
                            matchingObjectIdArray, matchingItemUriArray ) );    
                    }
                }
            }
		CleanupStack::PopAndDestroy( 2, &matchingObjectIdArray ); // matchingItemIdArray, matchingItemUriArray
        }
    CleanupStack::PopAndDestroy( 2, &allItemsIdArray ); // allItemsIdArray, allItemsUriArray
    }

// ------------------------------------------------
// NotifyModified
// ------------------------------------------------
//
void CMdSNotifier::NotifyModifiedL(const RArray<TItemId>& aObjectIds,
                                                         CMdSManipulationEngine* aMEngine)
	{
	if (aObjectIds.Count() == 0)
    	{
    	return;
    	}

    TBool allUrisFetched( EFalse );
    RPointerArray<HBufC> allUrisArray;
    CleanupResetAndDestroyPushL( allUrisArray );

    for( TInt i = iEntries.Count() - 1; i >=0; i-- )
        {
        TEntry& e = iEntries[i];

        if( e.iType & (EObjectNotifyModify | EObjectNotifyModifyWithUri) )
            {
            // collect matching object IDs
            RArray<TItemId> matchingObjectIdArray;
			CleanupClosePushL( matchingObjectIdArray );
		    RPointerArray<HBufC> uriArray;
		    CleanupResetAndDestroyPushL( uriArray );

            const TBool allMatches = iComparator->MatchObjectIdsL( e.Condition(), 
            		aObjectIds, matchingObjectIdArray );

			// check is there any matches
			if( allMatches || matchingObjectIdArray.Count() > 0 )
            	{
            	if(e.IsPending())
            		{
	            	// Match found. Trigger notifier entry.
	            	TInt err( KErrNone );

	            	if( allMatches )
	            		{
	            		// all matches so send whole object ID array
	            		if( e.iType & EObjectNotifyModifyWithUri )
	            		    {
	            		    if( !allUrisFetched )
	            		        {
	            		        aMEngine->GetObjectUrisByIdsL( aObjectIds, allUrisArray );
	            		        allUrisFetched = ETrue;
	            		        }
	                        TRAP( err, e.TriggerL( EObjectNotifyModifyWithUri, 
	                                 aObjectIds, allUrisArray ) );
	            		    }
	            		else
	            		    {
                            TRAP( err, e.TriggerL( EObjectNotifyModify, 
                                     aObjectIds, uriArray ) );
	            		    }
	            		}
	            	else
	            		{
                        if( e.iType & EObjectNotifyModifyWithUri )
                            {
                            aMEngine->GetObjectUrisByIdsL( matchingObjectIdArray, uriArray );
                            TRAP( err, e.TriggerL( EObjectNotifyModifyWithUri, 
                                    matchingObjectIdArray, uriArray ) );
                            }
                        else
                            {
                            TRAP( err, e.TriggerL( EObjectNotifyModify, 
                                    matchingObjectIdArray, uriArray ) );
                            }
	            		}
	            	if( err != KErrNone )
		            	{
		            	e.TriggerError( err );
	    	        	}
            		}
            	else
            		{
					if( allMatches )
	            		{
                        // all matches so send whole object ID array
                        if( e.iType & EObjectNotifyModifyWithUri )
                            {
                            if( !allUrisFetched )
                                {
                                aMEngine->GetObjectUrisByIdsL( aObjectIds, allUrisArray );
                                allUrisFetched = ETrue;
                                }
                            TRAP_IGNORE( e.CacheL( EObjectNotifyModifyWithUri, 
                                     aObjectIds, allUrisArray ) );
                            }
                        else
                            {
                            TRAP_IGNORE( e.CacheL( EObjectNotifyModify, 
                                     aObjectIds, uriArray ) );
                            }
	            		}
	            	else
	            		{
                        if( e.iType & EObjectNotifyModifyWithUri )
                            {
                            aMEngine->GetObjectUrisByIdsL( matchingObjectIdArray, uriArray );
                            TRAP_IGNORE( e.CacheL( EObjectNotifyModifyWithUri, 
                                    matchingObjectIdArray, uriArray ) );
                            }
                        else
                            {
                            TRAP_IGNORE( e.CacheL( EObjectNotifyModify, 
                                    matchingObjectIdArray, uriArray ) );
                            }
	            		}
            		}
            	}
			CleanupStack::PopAndDestroy( 2, &matchingObjectIdArray ); // matchingObjectIdArray, uriArray
            }
        }
    CleanupStack::PopAndDestroy( &allUrisArray );
	}

// ------------------------------------------------
// NotifyRemoved
// ------------------------------------------------
//
void CMdSNotifier::NotifyRemovedL(const RArray<TItemId>& aItemIdArray,
                                                          CMdSManipulationEngine* aMEngine)
	{
    if (aItemIdArray.Count() == 0)
        {
        return;
        }
    
    RPointerArray<HBufC> uriArray;
    CleanupResetAndDestroyPushL( uriArray );
    TBool urisFetched( EFalse );
	
    for( TInt i = iEntries.Count()- 1; i >=0; i-- )
        {
        TEntry& e = iEntries[i];

        if( e.iType & EObjectNotifyRemove )
        	{
        	RPointerArray<HBufC> nullArray; // For when uris are not needed
        	CleanupResetAndDestroyPushL( nullArray );
            if( e.IsPending() )
            	{
	            TRAPD( err, e.TriggerL( EObjectNotifyRemove, aItemIdArray, nullArray ) );
	            if( err != KErrNone )
	            	{
	            	e.TriggerError( err );
	            	}
            	}
            else
            	{
            	TRAP_IGNORE( e.CacheL( EObjectNotifyRemove, aItemIdArray, nullArray ) );
            	}
            CleanupStack::PopAndDestroy( &nullArray );
        	}
        else if( e.iType & EObjectNotifyRemoveWithUri )
            {
            if( !urisFetched && aMEngine )
                {
                aMEngine->GetObjectUrisByIdsL( aItemIdArray, uriArray );
                urisFetched = ETrue;
                }
            if( e.IsPending() )
                {
                TRAPD( err, e.TriggerL( EObjectNotifyRemoveWithUri, aItemIdArray, uriArray ) );
                if( err != KErrNone )
                    {
                    e.TriggerError( err );
                    }
                }
            else
                {
                TRAP_IGNORE( e.CacheL( EObjectNotifyRemoveWithUri, aItemIdArray, uriArray ) );
                }
            }
        }
    CleanupStack::PopAndDestroy( &uriArray );
	}

// ------------------------------------------------
// NotifyObjectPresent
// ------------------------------------------------
//
void CMdSNotifier::NotifyObjectPresent(TBool aPresent, const RArray<TItemId>& aObjectIds)
    {
    if (aObjectIds.Count() == 0)
    	{
    	return;
    	}

    for( TInt i = iEntries.Count() - 1; i >=0; i-- )
        {
        TEntry& e = iEntries[i];

        // No condition matching, object present changes
        // are always notified to object present observers
        if( e.iType & ( EObjectNotifyPresent | EObjectNotifyNotPresent )  )
            {
            const TMdSObserverNotificationType objectState = 
            	aPresent ? EObjectNotifyPresent : EObjectNotifyNotPresent;

            RPointerArray<HBufC> nullArray; // For when uris are not needed
            if( e.IsPending() )
            	{
            	// match found. trigger notifier entry !
	            TRAPD( err, e.TriggerL( objectState, aObjectIds, nullArray ) );
	            if( err != KErrNone )
	            	{
	            	e.TriggerError( err );
	            	}
            	}
            else
            	{
            	TRAP_IGNORE( e.CacheL( objectState, aObjectIds, nullArray ) );
            	}
            nullArray.Close();
            }
        }
    }

// ------------------------------------------------
// NotifyRelationPresent
// ------------------------------------------------
//
void CMdSNotifier::NotifyRelationPresent(TBool aPresent, const RArray<TItemId>& aRelationIds)
    {
    if (aRelationIds.Count() == 0)
    	{
    	return;
    	}

    for( TInt i = iEntries.Count() - 1; i >=0; i-- )
        {
        TEntry& e = iEntries[i];

        // No condition matching, relation present changes
        // are always notified to relation present observers
        if( e.iType & ( ERelationNotifyPresent | ERelationNotifyNotPresent ) )
            {
            const TMdSObserverNotificationType relationState = 
            	aPresent ? ERelationNotifyPresent : ERelationNotifyNotPresent;

            RPointerArray<HBufC> nullArray; // For when uris are not needed
            if( e.IsPending() )
            	{
            	// match found. trigger notifier entry !
	            TRAPD( err, e.TriggerL( relationState, aRelationIds, nullArray ) );
	            if( err != KErrNone )
	            	{
	            	e.TriggerError( err );
	            	}
            	}
            else
            	{
            	TRAP_IGNORE( e.CacheL( relationState, aRelationIds, nullArray ) );
            	}
            nullArray.Close();
            }
        }
    }


// ------------------------------------------------
// NotifySchemaAdded
// ------------------------------------------------
//
void CMdSNotifier::NotifySchemaAddedL()
    {
    for( TInt i = iEntries.Count() - 1; i >=0; i-- )
        {
        TEntry& e = iEntries[i];

        // No condition matching, schema additions 
        // are always notified to schema observers
        if( e.iType == ESchemaModify )
            {
            if( e.IsPending() )
            	{
	            // match found. trigger notifier entry
	            e.TriggerSchema();
            	}
            else
            	{
            	TRAP_IGNORE( e.CacheL( ESchemaModify ) );
            	}
            }
        }
    }


// ------------------------------------------------
// CheckForNotifier
// ------------------------------------------------
//
TBool CMdSNotifier::CheckForNotifier( TUint32 aNotifyTypes )
    {
    for( TInt i = iEntries.Count() - 1; i >=0; i-- )
        {
        if ( iEntries[i].iType & aNotifyTypes )
        	{
        	return ETrue;
        	}
        }
    return EFalse;
    }

void CMdSNotifier::NotifyRemovedRelationItemsL( 
		CMdCSerializationBuffer& aBuffer )
	{
	aBuffer.PositionL( KNoOffset );

	const TMdCItems& items = TMdCItems::GetFromBufferL( aBuffer );

	if( items.iRelations.iPtr.iCount )
		{
		for( TInt i = iEntries.Count() - 1; i >=0; i-- )
	        {
	        TEntry& e = iEntries[i];
	        
	        // if namespace definition IDs don't match skip listener entry
	        if( e.NamespaceDefId() != items.iNamespaceDefId )
	        	{
	        	continue;
	        	}
	        
	        if( e.iType & ERelationItemNotifyRemove )
            	{
            	aBuffer.PositionL( items.iRelations.iPtr.iOffset );
	            // check relations condition
            	RArray<TItemId> matchedRelations;
            	CleanupClosePushL( matchedRelations );
            	TBool matches = iComparator->MatchRelationItemsL( 
            			e.Condition(), aBuffer, matchedRelations );

            	if ( matches )
	        		{
	        		if(e.IsPending())
	        			{
		            	// Match found. Trigger notifier entry.
		            	TRAPD( err, e.TriggerRelationItemsL( 
		            			ERelationItemNotifyRemove, aBuffer, 
		            			matchedRelations ) );
		            	if( err != KErrNone )
			            	{
			            	e.TriggerError( err );
		    	        	}
		        		}
		        	else
		        		{
		        		TRAP_IGNORE( e.CacheRelationItemsL( 
		        				ERelationItemNotifyRemove, aBuffer, 
		        				matchedRelations ) );
		        		}
	        		}
            	CleanupStack::PopAndDestroy( &matchedRelations );
            	}
	        }
		}
	}

TInt CMdSNotifier::Compare( const TEntry& aFirst, const TEntry& aSecond )
    {
    return aFirst.Id() - aSecond.Id();
    }

