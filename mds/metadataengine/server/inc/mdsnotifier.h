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
* Description:  Notifier engine*
*/

#ifndef __MDSNOTIFIER_H__
#define __MDSNOTIFIER_H__

#include <e32base.h>

#include "mdscommoninternal.h"


// forward declarations
class CMdSServerSession;
class CMdSNotifyComparator;
class CMdSSchema;
class CMdCSerializationBuffer;
class CMdSManipulationEngine;

/**
* A class that contains notification profiles for clients.
* The class compares events against each profile and
* triggers the client side which then calls back the observer.
*/
class CMdSNotifier : public CBase
    {
    public: // public construction

        /** static construction */
        static CMdSNotifier* NewL();
        static CMdSNotifier* NewLC();

        /** destructor */
        virtual ~CMdSNotifier();

    private: // private construction

        /** constructor */        
        CMdSNotifier();

        /** 2nd phase constructor */
        void ConstructL();

    public: // methods

        /**
        * An inner class for an entry in the array of listeners to
        * notify for different notification profiles.Each entry
        * contains the notification profiles against which to
        * compare the event and the parameters required to perform
        * the asynchronous notification.
        */
        class TEntry
            {
            friend class CMdSNotifier; // accessible by host

            public:

                /**
                * sets the entry up for asynchronous callback
                * @param aMessage the asynchronous message
                * @param aRemoteSize the pointer to remote message size buffer
                */
                void SetupForCallback( RMessage2 aMessage, TInt aRemoteSizeMsgSlot );

                /**
                * Whether or not this entry has an asynchronous message
                * pending
                */
                TBool IsPending() { return ( iRemoteSizeMsgSlot!=KErrNotFound ); }

                /** the criteria folder */
                CMdCSerializationBuffer& Condition() 
                	{ 
                	return *iSerializedCondition; 
                	}

				/**
				*	Does this entry allow confidential data
				*	@return EFalse if not allowed
				*/
				TBool AllowConfidential()
					{
					return iConfidential;
					}
				
				/**
				*	Return the id of this entry
				*	@return The id
				*/
				TInt Id() const
					{
					return iId;
					}

				/**
				*	Return the id of this entry
				*	@return The id
				*/
				TDefId NamespaceDefId()
					{
					return iNamespaceDefId;
					}
					
				/**
				*	Trigger a previously cached notification
				*	@param aCompleteCode The code to complete the message
				*	@param aData The data hierarchy to send to the client,
				*                ownership changes only if method doesn't leave
				*	@return none
				*/			
				void TriggerCachedL(TUint32 aCompleteCode, CMdCSerializationBuffer* aData);

				/**
				*	Get the data buffer
				*	@return the serialised data buffer
				*/
				CMdCSerializationBuffer* GetDataBuffer();

            private: // methods for host class

                /** constructor */
                TEntry( TInt aId, TConditionType aType,
                    CMdCSerializationBuffer* aSerializedBuffer,
                    TDefId aNamespaceDefId, 
                    CMdSServerSession& aSession, TBool aConfidential );

                /**
                * Triggers the notifier causing the observer to be
                * notifier with the given code.
                * @param aCompleteCode either added/removed/modified
                * @param aIdArray ids of matching results
                * @param aUriArray uris of matching results 
                */
                void TriggerL( TUint32 aCompleteCode,
                    const RArray<TItemId>& aIdArray,
                    RPointerArray<HBufC>& aUriArray );

                /**
                 * Triggers the notifier causing the observer to be
                 * notifier with the given code.
                 * 
                 * @param aCompleteCode either added/removed/modified
                 * @param aBuffer relation items of the event
                 * @param aRelationIdArray matched relation ids
                 */
                void TriggerRelationItemsL( TUint32 aCompleteCode,
                		CMdCSerializationBuffer& aBuffer,
                		const RArray<TItemId>& aRelationIdArray );

				/**
				 * Trigger a notification of a schema change
				 */
				void TriggerSchema();

				/**
				 *	The entry has currently no message active, 
				 *	wait until one is available.
				 */
                void CacheL(TUint32 aCompleteCode, 
                                    const RArray<TItemId>& aIdArray,
                                    const RPointerArray<HBufC>& aUriArray);

                /**
				 *	The entry has currently no message active, 
				 *	wait until one is available.
				 */
                void CacheRelationItemsL(TUint32 aCompleteCode,
                		CMdCSerializationBuffer& aBuffer,
                		const RArray<TItemId>& aRelationIdArray);

                /**
				 *	cache schema notification
				 */
                void CacheL(TUint32 aCompleteCode);
				
                /**
                 * Triggers the notifier with an error code
                 * @param aErrorCode one of system-wide error codes
                 */
                void TriggerError( TInt aErrorCode );

				/**
				 * Copy all IDs from the array in to the buffer
				 * 
				 * @param aIdArray ID array
				 * @return new buffer
				 */
				CMdCSerializationBuffer* CopyToBufferL(
						const RArray<TItemId>& aIdArray);
				
                /**
                 * Copy all URIs from the array in to the buffer
                 * 
                 * @param aUriArray URI array
                 * @return new buffer
                 */
                CMdCSerializationBuffer* CopyToBufferL(const RArray<TItemId>& aIdArray,
                                                                              const RPointerArray<HBufC>& aUriArray);

				/**
				 * Copy matched relation IDs to a buffer
				 * 
				 * @param aRelationItemsBuffer serialized items
				 * @param aIdArray matched IDs
				 * @return new buffer
				 */
				CMdCSerializationBuffer* CopyItemsToBufferL(
						CMdCSerializationBuffer& aRelationItemsBuffer, 
						const RArray<TItemId>& aIdArray);
            private:

                /** id */
                TInt iId;

                /** type of the notification */
                TUint32 iType;

				/** namespace definition ID */
                TDefId iNamespaceDefId;

                /** actual serialized condition */
                CMdCSerializationBuffer* iSerializedCondition;

				/**
				* serialised id or relation items buffer.
				* Used instead of iDataObject when 
				* sending object ids to the clients
				*/
                CMdCSerializationBuffer* iDataBuffer;
				
                /** asynchronous message */
                RMessage2 iMessage;

                /** pointer to remote size buffer */
                TInt iRemoteSizeMsgSlot;

                /** the session */
                CMdSServerSession& iSession;

                TBool iConfidential;
            };

       /**
        * creates a notifier entry
        * @param aId id of the entry to be created
        * @param aType type of the notifier (object/relation/event)
        * @param aCriteria the serialized criteria to compare against
        * @param aSession the session that owns the notificant
        * @param aConfidential Does the session get notifications of confidential items 
        * @return reference to the created entry
        */
        TEntry& CreateEntryL( TInt aId, TConditionType aType,
            CMdCSerializationBuffer* aSerializedBuffer, TDefId aNamespaceDefId,
            CMdSServerSession& aSession, TBool aConfidential);

       /**
        * finds an entry by its id, if not found leaves with KErrNotFound
        * @param aId id of the entry
        * @return reference to the entry found
        */
        TEntry& FindEntryL( TInt aId );

       /**
        * removes a notifier entry, if not found leaves with KErrNotFound
        * @param aId id of the entry to be removed
        */
        void RemoveEntryL( TInt aId );

       /**
        * removes all notifier entries that belong to a certain session
        * @param aSession session who's notify entries are to be removed
        */
        void RemoveEntriesBySession(const CMdSServerSession& aSession );

       /**
        * notifiers that items has been added
        * @param aSerializedItems serialized items
        * @param aSerializedItemIds serialized item IDs
        */
        void NotifyAddedL(CMdCSerializationBuffer& aSerializedItems, 
        				  CMdCSerializationBuffer& aSerializedItemIds);

       /**
        * notifiers that items has been modified
        * @param aSerializedItems serialized items
        * @param aSerializedItemIds serialized item IDs
        */
        void NotifyModifiedL(CMdCSerializationBuffer& aSerializedItems, 
							 CMdCSerializationBuffer& aSerializedItemIds);

       /**
        * notifiers that items has been modified
        * @param aObjectIds object IDs
        * @param aMEngine pointer to manipulator class for utility functions
        */
        void NotifyModifiedL( const RArray<TItemId>& aObjectIds,
                                           CMdSManipulationEngine* aMEngine );

	   /**
        * notifiers that objects has been set to present or not present state
        * @param aPresent are object present state changed to present or not present
        * @param aObjectIds object IDs
		*/
		void NotifyObjectPresent(TBool aPresent, const RArray<TItemId>& aObjectIds);

	   /**
        * notifiers that objects has been set to present or not present state
        * @param aPresent are object present state changed to present or not present
        * @param aObjectIds object IDs
		*/
		void NotifyRelationPresent(TBool aPresent, const RArray<TItemId>& aObjectIds);

	   /**
		* notify about removed relation items
		* 
		* @param aRemovedRelations removed relations
		* @param aAdditionalRemovedRelations possible additional removed relations
		*/
		void NotifyRemovedRelationItemsL( CMdCSerializationBuffer& aBuffer );
		
		/**
		 * Notifiers that items has been removed. 
		 * Can be only used for default namespace.
		 * 
		 * @param aItemIdArray modified item IDs
		 * @param aMEngine pointer to manipulator class for utility functions
		 */
		void NotifyRemovedL( const RArray<TItemId>& aItemIdArray,
		                                    CMdSManipulationEngine* aMEngine );
		
		/**
        * notifiers that items has been removed
        * @param aSerializedItemIds serialized item IDs
        * @param aItemIsConfidential are items confidential
        * @param aRemovedItemUriArray uri array if uris are present
        * @param aMEngine pointer to manipulator class for utility functions
        */
        void NotifyRemovedL(CMdCSerializationBuffer& aSerializedItemIds, TBool aItemIsConfidential,
                                           RPointerArray<HBufC>& aRemovedItemUriArray,
                                           CMdSManipulationEngine* aMEngine );

		void NotifySchemaAddedL();
		
		TBool CheckForNotifier( TUint32 aNotifyTypes );

    private:
		
		static TInt Compare( const TEntry& aFirst, const TEntry& aSecond );
		
	private: // data members

        /** the entry array */
        RArray<TEntry> iEntries;

        /** the comparator */
        CMdSNotifyComparator* iComparator;
        
        TInt iDataAccessCount;
    };

#endif	// __MDSNOTIFIER_H__
