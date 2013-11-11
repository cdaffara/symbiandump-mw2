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
* Description:  This is Manipulation engine to manage adding,*
*/

#ifndef __MDSMANIPULATIONENGINE_H__
#define __MDSMANIPULATIONENGINE_H__

#include <e32base.h>

#include "mdscommoninternal.h"
#include "mdsgarbagecollector.h"

// FORWARD DECLARATIONS
class CMdSSchema;
class CMdsSchema;
class CMdSNotifier;
class CMdSServerSession;
class CMdSObjectLockList;
class CMdCSerializationBuffer;
class CMdSSqlObjectManipulate;

// CONSTANT DECLARATION
const TInt KGarbageCollectionDelay = 80; // 1 minute, 20 seconds

// CLASS DECLARATION

/**
* CMdSManipulationEngine.
*  Class for manipulating metadata database entries.
*/
class CMdSManipulationEngine : public CBase, public MMdSGarbageCollectorObserver
    {
    public: // Constructors and destructors

        /**
        * NewL.
        * Two-phased constructor.
        * @param aObjectManipulate pointer to object manipulator
        * @return Pointer to created CMdSManipulationEngine object.
        */
        static CMdSManipulationEngine* NewL( CMdsSchema& aSchema, 
            CMdSNotifier& aNotifier, CMdSObjectLockList& aLockList );

        /**
         * NewLC.
         * Two-phased constructor.
         * @param aObjectManipulate pointer to object manipulator
         * @return Pointer to created CMdSManipulationEngine object.
         */
         static CMdSManipulationEngine* NewLC( CMdsSchema& aSchema,
             CMdSNotifier& aNotifier, CMdSObjectLockList& aLockList );

        /**
        * ~CMdSManipulationEngine.
        * Destructor.
        */
        virtual ~CMdSManipulationEngine();

    public:

		/** 
		 * Adds object to DB from serialized buffer and put a result item IDs 
		 * to result buffer 
		 */
		void AddL( CMdCSerializationBuffer& aBuffer, 
				CMdCSerializationBuffer& aResultBuffer,
				const CMdSServerSession* aServerSession );

	    /** 
	     * Removes items from the database
	     * @param aBuffer serialized TMdCItemIds object
	     * @param aSuccessfulBuffer successfully removed objects
	     */
	    void RemoveL( CMdCSerializationBuffer& aBuffer, 
	    		CMdCSerializationBuffer& aSuccessfulBuffer );
	    
	    /** 
	     * Updates items to database
	     */
	    void UpdateL( CMdCSerializationBuffer& aBuffer, 
	    		CMdCSerializationBuffer& aResultBuffer );

	    /**
	     * Gets object id, object def and flags in serialization buffer.
	     * @param aResultBufferSize size of result buffer
	     * @param aUri object URI
	     * @param aNamespaceDefId namespace id
	     * 
	     * @return serialization buffer that contains object id, object def id and object flags
	     */
		CMdCSerializationBuffer* CheckObjectL( TInt aResultBufferSize, 
				const TDesC& aUri, TDefId aNamespaceDefId );

		/**
	     * Gets object def and flags in serialization buffer.
	     * @param aResultBufferSize size of result buffer
	     * @param aId object id
	     * @param aNamespaceDefId namespace id
	     * 
	     * @return serialization buffer that contains object def id and object flags
	     */
		CMdCSerializationBuffer* CheckObjectL( TInt aResultBufferSize, 
				TItemId aId, TDefId aNamespaceDefId );

		/**
	     * Gets object def and flags in serialization buffer.
	     * @param aResultBufferSize size of result buffer
	     * @param aIds list of object ids in serialization buffer
	     * @param aNamespaceDefId namespace id
	     * 
	     * @return serialization buffer that contains object id, object def id and object flags for
	     * all objects
	     */
		CMdCSerializationBuffer* CheckObjectL( TInt aResultBufferSize, 
				CMdCSerializationBuffer& aIds, TDefId aNamespaceDefId );

	    /**
         * Give a reference to the manipulator object
         */
		CMdSSqlObjectManipulate& Manipulate();

	    /**
	     * Give a reference to the manipulator object
	     */
        CMdSGarbageCollector& GarbageCollector();

		/**
        * Add memory card media ID
        * @param aMediaId Memory card's media ID
        */
		void AddMemoryCardL(TUint32 aMediaId);

        /**
        * Get the lastest memory card's media ID
        * @param aMediaId Returned the latest memory card's media ID
		* @leave KErrNotFound Not memory cards exist at all
        */
        void GetMemoryCardL(TUint32& aMediaId);

        /**
         * Checks if memory card's with given media ID exists
         * @param aMediaId memory card's media ID
         * @return memory card exists or not
         */
        TBool CheckMemoryCardL(TUint32 aMediaId);

        /**
         * Set media's media ID, drive and present state to DB.
         * 
         * @param aMediaId media ID
         * @param aMediaId drive
         * @param aMediaId present state
         */
        void SetMediaL(TUint32 aMediaId, TChar aDrive, TBool aPresentState);

        /**
         * Check if the media's with given media ID exist in DB and return 
         * drive and present state.
         * 
         * @param aMediaId given media ID
         * @param aMediaId drive
         * @param aMediaId present state
         * 
         * @return does media exist
         */
        TBool GetMediaL(TUint32 aMediaId, TChar& aDrive, TBool& aPresentState);

        /**
         * Get present medias' media IDs and drives.
         * 
         * @param aMediaInfoBuffer buffer where media infos are written
         * 
         * @return present media count
         */
        TInt32 GetPresentMediasL(TDes8& aMediaInfoBuffer);

        /**
         * Sets files object to present state.
         * @param aMediaId Memory card's media ID
         * @param aFileCount File count
         * @param aUris Files' URIs
         * @param aFileInfos Files' file infos
         * @param aResults TFilePresentStates results (casted to TUint8)
         */
        void SetFilesToPresentL(TUint32 aMediaId, TUint32 aFileCount, 
        		CMdCSerializationBuffer& aUris, CMdCSerializationBuffer& aFileInfos,
        		CMdCSerializationBuffer& aResults);
        
        
        /**
        * Sets all file objects which has given media ID to not present state.
        * @param aMediaId Memory card's media ID
        * @param aStartUp start up not present
        */
        void SetFilesToNotPresentL(TUint32 aMediaId, TBool aStartUp);
        
        /**
        * Removes all file objects which has given media ID and are in not 
        * present.
        *
        * @param aMediaId Memory card's Media ID
        * @param aStartUp start up not present
        */
        void RemoveFilesNotPresentL(TUint32 aMediaId, TBool aStartUp);

		/**
		 * Get schema version
		 */
		void GetSchemaVersionL(TInt& aMajorVersion, TInt& aMinorVersion);

		/**
		 * Set object to "present" state bu GUID
		 * 
		 * @leave KErrNotFound MdE can't find object in "not present" state 
		 *                     with matching GUID
		 */
		void SetObjectToPresentByGuidL( 
				const TInt64& aGuidHigh, const TInt64& aGuidLow );

		/**
		 * Change a path of a media object to another.
		 * @param aOldPath old path
		 * @param aNewPath new path
		 */
		void ChangePathL(const TDesC& aOldPath, const TDesC& aNewPath);
		
		/**
		 * Updates C drive media id for all objects with the old media id. 
		 */
		void ChangeMediaIdL();

        /**
         * Updates internal mass storage media id for all objects. 
         */
        void CheckMassStorageMediaIdL( const TUint32 aMediaId );
        
		/**
		 * Adds new relation definition to the DB.
		 * @param aNamespaceId namespace id
		 * @param aRelationDefName name of the new relation def
		 */
		void AddRelationDefL( TDefId aNamespaceId, const TDesC& aRelationDefName );
		
		/**
		 * Adds new event definition to the DB.
		 * @param aNamespaceId namespace id
		 * @param aEventDefName name of the new event def
		 */
		void AddEventDefL( TDefId aNamespaceId, const TDesC& aEventDefName );

		/**
		 * Set the "pending" flag for objects. The pending flag means that composing to binary is 
		 * pending for the object.
		 * @param aObjectIds list of object ids
		 */
		void SetPendingL(const RArray<TItemId>& aObjectIds);
		
		/**
		 * Resets the "pending" flag for objects.
		 * @param aObjectIds list of object ids.
		 */
		void ResetPendingL(const RArray<TItemId>& aObjectIds);
		
		/**
		 * Gets count of objects pending.
		 * @param aObjectDefId type of objects
		 * @return count of pending objects
		 */
		TInt GetPendingCountL( TDefId aObjectDefId );
		
		/**
		 * Gets ids of pending objects of type defined with aObjectDefId.
		 * @param aObjectDefId objects definition id
		 * @param aBufferSize max count of object ids in aObjectIds
		 * @param aObjectIds on return contains ids of pending objects
		 * 
		 * @return 1 if there were more pending objects than could be appended to the array,
		 * 	0 otherwise
		 */
		TInt GetPendingL( TDefId aObjectDefId, TInt aBufferSize, 
				RArray<TItemId>& aObjectIds );
		
	      /**
	         * Fetches object uris based on id array
	         * @param aObjectIds list of object ids.
	         * @param aObjectUris array for object uris.
	         */
		void GetObjectUrisByIdsL( const RArray<TItemId>& aObjectIds, 
		                                         RPointerArray<HBufC>& aUriArray );
		
	protected:
		TBool StartGarbageCollectionL();

    private: // Constructors and destructors

        /**
        * CMdSManipulationEngine.
        * C++ default constructor.
        * @param .
        */
        CMdSManipulationEngine( CMdsSchema& aSchema, CMdSNotifier& aNotifier, 
        		CMdSObjectLockList& aLockList );

        /**
        * ConstructL.
        * 2nd phase constructor.
        */
        void ConstructL();

    private: // Methods for own use

    private: // Data

       /**
        * Object adding, removing and modification.
        * Provides DB coding.
        */
    	CMdSSqlObjectManipulate* iManipulate;

        /**
        * Schema
        */
        CMdsSchema& iSchema;

        /**
        * Notifier
        */
        CMdSNotifier& iNotifier;

		CMdSGarbageCollector* iGarbageCollector;
		
		CMdSObjectLockList& iLockList;
		
		//For DB maintenance
		TInt iAddedObjectsCount;
		TInt iModifiedObjectsCount;
	};

#endif //__MDSMANIPULATIONENGINE_H__
