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
* Description:  Add, update and remove items in DB.*
*/

#ifndef __MDSSQLOBJECTMANIPULATE_H__
#define __MDSSQLOBJECTMANIPULATE_H__

// INCLUDE FILES
#include <e32base.h>
#include <hash.h>

#include "mdscommoninternal.h"

#include "mdsclausebuffer.h"

// FORWARD DECLARATION
class CMdSSqLiteConnection;
class CMdsSchema;
class CMdCFolder;
class RMdsStatement;
class CMdsNamespaceDef;
class CMdsObjectDef;
class CMdsPropertyDef;
class CMdCSerializationBuffer;
class CMdsClauseBuffer;
class RRowData;
class TColumn;
class CMdSObjectLockList;
class CMdSServerSession;

// CLASS DECLARATION
/**
 *
 */
class CMdSIdentifierGenerator: public CBase
	{
    public: // Constructors and destructor
	    /**
	    * Two-phased constructor.
	    * @param aPool, reference to pool for reserve database connection.
        * @return Pointer to CMdSIdentifierGenerator object.
	    */
        static CMdSIdentifierGenerator* NewL();

	    /**
	    * Two-phased constructor.
	    * @param aPool, reference to pool for reserve database connection.
	    * @return Pointer to CMdSIdentifierGenerator object.
	    */
        static CMdSIdentifierGenerator* NewLC();

        /**
        * Destructor.
        */
        virtual ~CMdSIdentifierGenerator();

		/**
		 * Set guid to generated ones if both are 0
		 */
		void GenerateGuid( TInt64& aGuidHigh, TInt64& aGuidLow );

		/**
		 * Return unique uri
		 * method require unique guid's
		 */
		HBufC* GenerateUriL( const CMdsObjectDef& aObjectDef, 
				const TInt64& aGuidHigh, const TInt64& aGuidLow ) const;

    private: // Private constructors
        /**
        * CMdSIdentifierGenerator.
        * C++ default constructor.
        */        
        CMdSIdentifierGenerator( )
        	{
        	iDigest = NULL;
        	}

        /**
        * ConstructL.
        * 2nd phase constructor.
        */        
        void ConstructL( );

	private: //Internal variables
		TInt64 iImei;
		TInt64 iLastTime;
		
		CMessageDigest* iDigest;
	};


/**
 * CMdSSqlObjectManipulate converts object handling  
 * instructions to an SQL clause.
 */
class CMdSSqlObjectManipulate: public CBase
	{
    public: // Constructors and destructor

	    /**
	    * Two-phased constructor.
	    * @param aPool, reference to pool for reserve database connection.
        * @return Pointer to CMdSSqlObjectManipulate object.
	    */
        static CMdSSqlObjectManipulate* NewL( const CMdsSchema& aSchema, 
        		CMdSObjectLockList& aLockList );

	    /**
	    * Two-phased constructor.
	    * @param aPool, reference to pool for reserve database connection.
	    * @return Pointer to CMdSSqlObjectManipulate object.
	    */
        static CMdSSqlObjectManipulate* NewLC( const CMdsSchema& aSchema, 
        		CMdSObjectLockList& aLockList );

        /**
        * Destructor.
        */
        virtual ~CMdSSqlObjectManipulate();


    private: // Private constructors
    
        /**
        * CSqlObjectManipulate.
        * C++ default constructor.
        */        
        CMdSSqlObjectManipulate( const CMdsSchema& aSchema, 
        		CMdSObjectLockList& aLockList );
        
        /**
        * ConstructL.
        * 2nd phase constructor.
        */        
        void ConstructL( );


    public: // Public functions

		/**
		 * Add new object to DB (not using any transaction system)
		 *
		 * @param aBuffer serialized object with properties and freeText
		 * @return new added object id
		 */
		TItemId AddObjectL( CMdSSqLiteConnection& aConnection, 
				CMdCSerializationBuffer& aBuffer, 
				RMdsStatement& aMdsBaseObjStatement, RMdsStatement& aMdsObjStatement,
				const CMdSServerSession* aServerSession = NULL );

		/**
		 * Really add freetext to DB
		 * 
		 * @param aWord word to add
		 * @param aObjectId owner of freetext object id
		 * @param aPosition position
		 * @param aSearch search for existing freetext in DB
		 */
		TUint32 AddFreeTextToDBL( TPtrC16& aWord, TItemId aObjectId, 
				TInt32 aPosition, TBool aSearch = ETrue );

		/**
		 * Add new freeText to DB (not using any transaction system)
		 *  (check if FreeText is in DB and in this case don't add it)
		 *
		 * @param aBuffer serialized object with FreeText
		 * @param aFreeTextCount freetext count
		 * @param aObjectId freetext owner object ID
		 * @return how many freeText was added to DB
		 */
		TInt AddFreeTextL( CMdCSerializationBuffer& aBuffer, TInt aFreeTextCount, TItemId aObjectId );

		/**
		 *	remove object by URI - just for internal use!
		 */
		void RemoveObjectForceL( const TDesC16& aUri, TItemId aObjectId );

		/**
		 * USE RemoveObjectForceL WITH CAUTION !!!
		 * THIS FUNCTION REMOVES OBJECT WITHOUT ANY CHECKING
		 * ONLY FOR INTERNAL USE !!!
		 */
		void RemoveObjectForceL( TItemId aObjectId );

		CMdCSerializationBuffer* CheckObjectL( TInt aResultBufferSize, 
				const TDesC& aUri, TDefId aNamespaceDefId );

		CMdCSerializationBuffer* CheckObjectL( TInt aResultBufferSize, 
				TItemId aId, TDefId aNamespaceDefId );

		CMdCSerializationBuffer* CheckObjectL( TInt aResultBufferSize, 
				CMdCSerializationBuffer& aIds, TDefId aNamespaceDefId );

		void CollectRemovedItemsL( RArray<TItemId>& aRemoveIds, 
				RArray<TItemId>& aObjectIds, RArray<TItemId>& aRelationIds, 
				RArray<TItemId>& aEventIds );

		/**
		 * Remove object using it's id
		 *
		 * @param aBuffer id's to remove
		 * @param aCount id's count
		 * @param aIdArray result id's
         * @param aRelationIds result relation id's
         * @param aEventIds result event id's
         * @param aUrisRequired determines if uris are to be fetched
         * @param aRemovedItemUriArray result uris
		 */
		void RemoveObjectsByIdL( CMdCSerializationBuffer& aBuffer, TInt aCount,
				RArray<TItemId>& aIdArray, RArray<TItemId>& aRelationIds, 
				RArray<TItemId>& aEventIds,
				TBool aUrisRequired, 
				RPointerArray<HBufC>& aRemovedItemUriArray);

		/**
		 * Remove object using it's URI's
		 *
		 * @param aBuffer URI's to remove
		 * @param aCount URI's count
		 * @param aIdArray result object id's
         * @param aRelationIds result relation id's
         * @param aEventIds result event id's
         * @param aRemovedItemUriArray result uris
         */
		void RemoveObjectsByUriL( CMdCSerializationBuffer& aBuffer, TInt aCount,
				RArray<TItemId>& aIdArray, RArray<TItemId>& aRelationIds, 
				RArray<TItemId>& aEventIds,
				TBool aUrisRequired,
				RPointerArray<HBufC>& aRemovedItemUriArray );

		/**
		 * search for object uri
		 *
		 * @param aUri object uri to search for
		 * @param aFlags return found object flags
		 * @return object id
		 */
		TItemId SearchObjectByUriL( const TDesC16& aUri, TUint32& aFlags );

        /**
         * search for object uri
         *
         * @param aId object id to search uri for
         * @param aFlags return found object flags
         * @return reference to object uri
         */
		HBufC*& SearchObjectUriByIdL( const TItemId aId, TUint32& aFlags );
		
		/**
		 * update freetext (add and remove)
		 */
		void UpdateFreeTextL( CMdCSerializationBuffer& aBuffer, TInt aCount,
				TItemId aObjectId );

		/**
		 * Update object
		 *
		 * @param aBuffer objects to update
		 * @param aLockList list of locked objects
		 */
		TItemId UpdateObjectL( CMdSSqLiteConnection& aConnection, 
				CMdCSerializationBuffer& aBuffer );
		
 	    /**
 	    * Runs garbage collection to database
 		* @param aDb The database connection
 		* @return next garbage collection start is needed?
 	    */
        TBool GarbageCollectionL();
        
		/**
		 * Set namespace for processing functions
		 */
        void SetNamespace( const CMdsNamespaceDef* aNamespaceDef );

        TItemId AddEventL( CMdSSqLiteConnection& aConnection, 
        		CMdCSerializationBuffer& aBuffer );

        TItemId AddRelationL( CMdSSqLiteConnection& aConnection, 
        		CMdCSerializationBuffer& aBuffer );

        void RemoveEventsL( CMdCSerializationBuffer& aBuffer, TInt aCount,
        		RArray<TItemId>& aIdArray );

        void RemoveRelationsL( CMdCSerializationBuffer& aBuffer, TInt aCount,
        		RArray<TItemId>& aIdArray );

        TItemId UpdateRelationsL( CMdSSqLiteConnection& aConnection, 
        		CMdCSerializationBuffer& aBuffer );

		/**
        * Add memory card media ID
		* @param aDb The database connection
        * @param aMediaId Memory card's media ID
        */
		void AddMemoryCardL(TUint32 aMediaId);

        /**
         * Get the latest memory card's media ID
 		 * @param aDb The database connection
         * @param aMediaId Returns the latest memory card's media ID
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
         * Search file object which is in not present state.
 		 *
 		 * @param aDb The database connection
         * @param aMediaId Memory card's media ID
         * @param aUri File's URI
         * @param aFileInfo File information
         *
 		 * @leave KErrNotFound If not present state file object doesn't exist 
 		 *                     with given URI and media ID
         */
		TItemId SearchNotPresentFileL(TUint32 aMediaId, TDesC16& aUri, 
									  TMdSFileInfo& aFileInfo,
									  TFilePresentStates& aPlaceHolder, TBool& aNotPresentState);

        /** from MMdSObjectManipulate */
        void SetFilesToPresentL(const RArray<TItemId>& aObjectIds);
		
        /**
         * Sets object's relation to present state.
         * @param aObjectId object ID
         * @param aIdArray marked relations ID's
         */
        void SetRelationsToPresentL(TItemId aObjectId, 
        		RArray<TItemId>& aIdArray);

        /**
         * Sets all file objects from last memory card to not present state.
 		 * @param aMediaId Memory card's media ID
         * @param aObjectIds Object IDs of those object which are set to not 
         *                   present state
         */
        void SetFilesToNotPresentL(TUint32 aMediaId, TBool aStartUp, RArray<TItemId>& aObjectIds);

        /**
         * Sets file objects relations from last memory card to not present 
         * state.
         *
         * @param aMediaId Memory card's media ID
         * @param aRelationIds changed relation id's
         */
        void SetRelationsToNotPresentL(TUint32 aMediaId, 
        		RArray<TItemId>& aRelationIds);

        /**
         * Removes all file objects which has given media ID and are in not 
         * present.
 		 *
         * @param aMediaId Memory card's media ID
         */
        void RemoveFilesNotPresentL(TUint32 aMediaId, RArray<TItemId>* aObjectIds);

		/**
		 * Get schema version from DB
		 * @param aConnection The database connection
		 * @param aMajorVersion schema major version
		 * @param aMinorVersion schema minor version
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
         * Change path of objects, which match to the old path, to the new 
         * path.
         *
         * @param aOldPath the old path 
         * @param aNewPath the new path
         * @param aObjectIds IDs of the change objects
         */
		void ChangePathL(const TDesC& aOldPath, const TDesC& aNewPath, 
				RArray<TItemId>& aObjectIds);
		
		/**
		 * Change C-drive media id and update it to all objects that have the old id.
		 */
		void ChangeMediaIdL();
		
		/**
		 * Get removed relation items
		 * 
		 * @param aBuffer buffer to fill with returned items
		 * @param aRemovedRelations removed relations
		 * @param aAdditionalRemovedRelations possible additional removed relations
		 */
		void GetRemovedRelationItemsL( CMdCSerializationBuffer& aBuffer,
				const RArray<TItemId>& aRemovedRelations,
				const RArray<TItemId>& aAdditionalRemovedRelations );

		/**
		 * Set/reset "binary composing to file" pending flag to objects
		 * 
		 * @param aObjectIds object IDs
		 * @param aReset when EFalse flags are setted and when ETrue flags are 
		 *               resetted
		 */
		void SetPendingL(const RArray<TItemId>& aObjectIds, TBool aReset);
		
		/**
		 * Get count of objects with "binary composing to file" pending flag
		 * 
		 * @param aObjectDefId object def ID
		 * 
		 * @return if >= 0, object count is returned, otherwise error code is 
		 *         returned
		 */
		TInt GetPendingCountL(TDefId aObjectDefId);
		
		/**
		 * Get objects with "binary composing to file" pending flag
		 * 
		 * @param aObjectDefId object def ID
		 * @param aBufferSize maximum ID count of buffer
		 * @param aObjectIds returned object IDs
		 * 
		 * @return if > 0, there was not enough space in buffer, otherwise 
		 *         error code is returned
		 */
		TInt GetPendingL(TDefId aObjectDefId, TInt aBufferSize, 
				RArray<TItemId>& aObjectIds);

		void CheckMassStorageMediaIdL( const TUint32 aMediaId );

        /**
        * Analyze the  database after enough items have been added or modified
        */    
        void AnalyzeL();
		
    protected: // personal methods

		/**
		 * Add single property from aBuffer and adds it to proper clause and 
		 * row data.
		 *
		 * @param aBuffer serialized property
		 * @param aObjectDef definition of current object
		 * @param aBaseObjectClause clause describing base object
		 * @param aObjectClause clause describing current object
		 * @param aBaseObjectDataRow data row contains base object values
		 * @param aObjectDataRow data row contains current object values
		 * @return new property definition
		*/		 
		const CMdsPropertyDef& ReadPropertyL( 
				CMdCSerializationBuffer& aBuffer, 
				const CMdsObjectDef& aObjectDef, 
				CMdsClauseBuffer& aBaseObjectClause, 
				CMdsClauseBuffer& aObjectClause,
                RRowData& aBaseObjectDataRow, RRowData& aObjectDataRow, 
                TUint8& aFlags );

		/**
		 * Search in DB for freeText
		 *
		 * @param aFreeText freetext to search for
		 * @return if find ETrue, else EFalse
		 */
		TItemId FindFreeTextInDbL( TPtrC16 aFreeText );
		
        /**
	    * Deletes BaseObject.
	    */	   	    
        TBool DoGarbageCollectionL();    

#ifdef MDS_PLAYLIST_HARVESTING_ENABLED
    private:
        
        /**
        * Deletes BaseObject.
        */          
        TInt CleanPlaylistsL();
#endif
        
	private: // Member data
		struct TLockBuffer
			{
			CMdsClauseBuffer* iBuffer;
			TBool iLock;
			};

    	TRequestStatus iTextSearcherDeleteFreeTextStatus;

    	TItemId iTextSearcherDeleteFreeTextObjectId;

    	TRequestStatus iTextSearcherAddFreeTextStatus;

    	TItemId iTextSearcherAddFreeTextObjectId;

		TItemId iCreationEventId;

		TItemId iDeletionEventId;

        /**
         * pointer to actual processing namespace
         */
        const CMdsNamespaceDef* iNamespaceDef;
        
        /**
         * common usage buffers (initial size = 1024)
         */
        RArray<TLockBuffer> iBuffers;

		/**
		 * BaseObject definition, used in addobject and addproperty
		 */
		const CMdsObjectDef* iBaseObjectDef;
	
	    /**
        * Store every namespace read from schema file.
        */
	    const CMdsSchema& iSchema;
	    
	    /**
	     * guid generator
	     */
	    CMdSIdentifierGenerator* iGenerator;

	    CMdSObjectLockList& iLockList;
	    
	    // Last handled uri.
	    HBufC* iUri;
	
	    TBuf<256> iLastAddedObjName;
	    
	    TBool iDictionaryToBeCleaned;
	    
	private:
		class RClauseBuffer
			{
			public:
				RClauseBuffer(CMdSSqlObjectManipulate& aSOM, TInt aSize = 1024);
				
				CMdsClauseBuffer& BufferL();

				void Close();

			private:
				RArray<TLockBuffer>& iBuffers;
				CMdsClauseBuffer* iBuffer;
				TInt iNr;
				const TInt iSize;
			};
	};

#include "mdssqlobjectmanipulate.inl"

#endif	// __MDSSQLOBJECTMANIPULATE_H__
