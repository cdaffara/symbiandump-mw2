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
* Description:  This is Metadata engine client session file
*
*/


#ifndef __MDEENGINESESSION_H__
#define __MDEENGINESESSION_H__

// INCLUDE FILES
#include <e32std.h>
#include <e32base.h>
#include <badesca.h>

#include "mdscommoninternal.h"
#include "mdequery.h"
#include "mdeharvestersession.h"

// CONSTANTS
// Number of message slots to reserve for this client server session.
// 27 = 1 asynchronous find + max.25 pending notifier hooks + 1 extra
const TUint KMetadataMessageSlots = 27;

// FORWARD DECLARATIONS
class CMdESessionImpl;
class CMdESchemaItem;
class CMdEInstanceItem;
class CMdEQuery;
class CMdEQueryCriteriaSerialization;
class CMdCSerializationBuffer;
class TMdEObject;

// CLASS DECLARATION
/**
* RMdEEngineSession
*  This class provides the client-side interface to the server session
*/
class RMdEEngineSession : public RSessionBase
    {
    public: // Constructors and destructors

        /**
        * RMdEEngineSession.
        * Constructs the object.
        */
        RMdEEngineSession(CMdESessionImpl& aSession);

    public: // New functions

        /**
        * Open server.
        * Connects to the server.
        * @return Error code.
        */
        void OpenL();
        
        /**
        * Cancel server opening.
        * Cancels server opening.
        * @return Error code.
        */
       void OpenCancel();

        /**
        * Connect session to server.
        * Create a session.
        * @return Error code.
        */
        void ConnectL();

        /**
        * Shut down session
        * Performs session shutdown activities in server end
        */
        void Shutdown();

        /**
        * Version.
        * Gets the version number.
        * @return The version.
        */
        TVersion Version() const;

        /**
         * Adds an event definition to the database.
         * @param aNameSpaceId id of the namespace to add the definition into
         * @param aName name of the event
         */
		void DoAddEventDefL(const TDefId aNameSpaceId, const TDesC &aName );

		/**
		 * Adds a relation definition to the database.
		 * @param aNameSpaceId id of the namespace to add the definition into
		 * @param aName name of the relation
		 */
		void DoAddRelationDefL(const TDefId aNameSpaceId, const TDesC &aName );

		/**
		 * Adds items to the database.
		 * @param aSerializedBuffer items serialized in a buffer
		 * @param aResultBuffer on return contains serialized results
		 */
        void DoAddItemsL( const CMdCSerializationBuffer& aSerializedBuffer, CMdCSerializationBuffer& aResultBuffer );

        /**
		 * Adds items to the database asynchronously.
		 * @param aSerializedBuffer items serialized in a buffer
		 * @param aResultBuffer on return contains serialized results
		 * @param aStatus the asynchronous response status
		 */
        void DoAddItemsAsync( const CMdCSerializationBuffer& aSerializedBuffer, CMdCSerializationBuffer& aResultBuffer, TRequestStatus& aStatus );

        /**
		 * Removes items from the database.
		 * @param aRemoveId item ids serialized in a buffer
		 * @param aSuccessfulId on return contains ids of successfully remove items in a serialization buffer.
		 */
        void DoRemoveItemsL( const CMdCSerializationBuffer& aRemoveId, CMdCSerializationBuffer& aSuccessfulId );

        /**
		 * Removes items from the database asynchronously.
		 * @param aRemoveId item ids serialized in a buffer
		 * @param aSuccessfulId on return contains ids of successfully remove items in a serialization buffer.
		 * @param aStatus the asynchronous response status
		 */
        void DoRemoveItemsAsync( const CMdCSerializationBuffer& aRemoveId, CMdCSerializationBuffer& aSuccessfulId, TRequestStatus& aStatus );

        /**
         * Updates items in the database.
         * @param aSerializedBuffer serialized buffer of updated items
         * @param aResultBuffer on return contains serialized results
         */
        void DoUpdateItemsL( const CMdCSerializationBuffer& aSerializedBuffer, CMdCSerializationBuffer& aResultBuffer );

        /**
         * Updates items in the database asynchronously.
         * @param aSerializedBuffer serialized buffer of updated items
         * @param aResultBuffer on return contains serialized results
         * @param aStatus the asynchronous response status
         */
        void DoUpdateItemsAsync( const CMdCSerializationBuffer& aSerializedBuffer, CMdCSerializationBuffer& aResultBuffer, TRequestStatus& aStatus );

        /**
         * Finds objects synchronously.
         * @param aQuery the query
         * @param aSerializationBuffer query criterias serialiazed in a buffer
         * @param aLocktype how the object should be locked during the query
         * @param aNotifyCount count indicating how often the client should be notified
         */
        CMdCSerializationBuffer* DoFindSyncLC( 
        	const CMdEQuery* aQuery, 
        	const CMdEQueryCriteriaSerialization& aSerializationBuffer,
            TMdCQueryLockType aLocktype,TUint32 aNotifyCount);
            
        /**
        * Finds objects asynchronously
        * @param 
        * @param 
        * @param 
        * @param aAsyncStatus the asynchronous response status
        */
        void DoFindAsync(
        	TUint32 aQueryId, 
            const CMdEQueryCriteriaSerialization& aSerializationBuffer,
            TPckgBuf<TInt>* aSizeBuf,
            TRequestStatus& aAsyncStatus, 
            TUint32 aNotifyCount = KMdEQueryDefaultMaxCount);

        /**
        * Continues a step in an existing asynchronous find
        * @param 
        * @param 
        * @param aAsyncStatus the asynchronous response status
        */
        void DoFindContinue( 
        	const CMdEQuery* aQuery, 
            TPckgBuf<TInt>* aSizeBuf,
            TRequestStatus& aAsyncStatus );

        /**
        * Cancels ongoing find
        */
        void DoFindCancelL( const CMdEQuery* aQuery );

        /**
        * Registers for notifications
        */
        void DoRegisterL( TInt aId,
            TUint32 aType,
            CMdCSerializationBuffer& aBuffer,
            TDefId aNamespaceDefId );

        /**
        * Continues registration for more notifications
        */
        void DoListen( TInt aId,
            TPckgBuf<TInt>* aSizeBuf,
            TRequestStatus& aAsyncStatus );

        /**
        * Unregisters observer for no more notifications
        */
        void DoUnregisterL( TInt aId );

        /**
        * DoLoadSchemaL.
        * Gets find results to client 
        * @param aHandle handle to shared memory chunk
        */
        void DoLoadSchemaL( TInt& aHandle );

        /**
        * DoGetDataL.
        * Gets data to client.
        * @param aQuery the query ID
        * @param aSize number of bytes to reserve for hierarchy
        * @param aRelatedCommand command related to which the data is obtained.
        * @param aExtraData an optional id specifying the identity of the data to obtain.
        */
		void DoGetDataL( const CMdEQuery* aQuery, 
			CMdCSerializationBuffer* aResultBuffer, 
			TUint32 aRelatedCommand, TUint32 aExtraData = 0 );
        /**
        * DoGetDataL.
        * Gets data from the server for a notifier AO
        * @param aBuffer The data buffer to read to from the server
        * @param aId The id of the observer requesting the data
        */
		void DoGetDataL( CMdCSerializationBuffer& aBuffer, TUint32 aId );

		void DoCheckObjectL( CMdCSerializationBuffer& aObjectBuffer, 
				const TDesC& aUri, TDefId aNamespaceDefId );

		void DoCheckObjectL( CMdCSerializationBuffer& aObjectBuffer, 
				TItemId aId, TDefId aNamespaceDefId );

		void DoCheckObjectL( CMdCSerializationBuffer& aObjectsBuffer, 
				const CMdCSerializationBuffer& aObjectIdsBuffer, 
				TDefId aNamespaceDefId );

        /**
        * Commits changes to objects to database
        * @param aCodedObjects objects in coded form
        */
        void DoCommitObjectsL( const CMdCSerializationBuffer& aCodedObjects );

        /**
        * Cancels changes to object to database
        * @param aCodedId id of locked object in coded form
        */
        void DoCancelObjectL( CMdCSerializationBuffer& aRemoveId );

        /**
         * Imports schema from a file.
         * @param aFilename schema file name
         */
        void DoImportSchemaL( const TDesC& aFileName );
    
        /**
         * Imports metadata from a file.
         * @param aFilename metadata file name
         */
        TInt DoImportMetadataL( const TDesC& aFileName );

        /**
         * Imports metadata from a file asynchronously.
         * @param aFilename metadata file name
         * @param aResult packaged error code
         * @param aStatus asynchronous request status
         */
        void DoImportMetadata( const TDesC& aFileName, TPckgBuf<TInt>& aResult, TRequestStatus& aStatus );

        /**
         * Exports metadata into a file.
         * @param aFilename metadata file name
         * @param aItems items to export in a serialized buffer
         */
        void DoExportMetadataL( const TDesC& aFileName, const CMdCSerializationBuffer& aItems );

        /**
         * Exports metadata into a file asynchronously.
         * @param aFilename metadata file name
         * @param aItems items to export in a serialized buffer
         * @param aStatus asynchronous request status
         */
        void DoExportMetadataL( const TDesC& aFileName, const CMdCSerializationBuffer& aItems,
								TRequestStatus& aStatus);

        /**
    	 * Add memory card with given media ID and current time. If memory card 
    	 * already exist only time is updated to current time.
    	 * @param memory card's media ID
    	 */
		void DoAddMemoryCard(TUint32 aMediaId);

		/**
		 * Get latest memory card's media ID.
		 * @param returned memory card's media ID
		 * @return EFalse if there is no any memory cards added
		 */
		TBool DoGetMemoryCard(TUint32& aMediaId);
		
	    /**
		 * Check if there is memory card with given media ID.
		 * @param memory card's media ID
		 * @param returned boolean if memory card exists or not
		 * @return EFalse if there was error
		 */
		TBool DoCheckMemoryCard(TUint32 aMediaId, TBool& aExist);

		/**
		 * Set media's media ID, drive and present state.
		 * 
		 * @param media's media ID
		 * @param media's drive
		 * @param media's present state
		 */
		void DoSetMediaL(TUint32 aMediaId, TChar aDrive, TBool aPresentState);

		/**
		 * Get media's drive and present state by media ID.
		 * 
		 * @param aMediaId media's media ID
		 * @param aDrive returned media's drive
		 * @param aPresent returned media's present state
		 * @return EFalse if there is no media with matching media ID in DB
		 */
		TBool DoGetMediaL(TUint32 aMediaId, TChar& aDrive, 
				TBool& aPresentState);

	    /**
		 * Get present medias' media IDs and drives.
		 * 
		 * @param aMedias returned present medias' media IDs and drives
		 */
		void DoGetPresentMediasL(RArray<TMdEMediaInfo>& aMedias);
		
		/**
	     * Set file to present state.
	     *
	     * @param aMediaId file's media ID
	     * @param aUri file's URI
	     * @param aFileInfo file's modified date and byte size
	     *
	     * @return ETrue if file existed and changing to present state was 
	     *               successful
	     */
		TBool DoSetFileToPresent(TUint32 aMediaId, const TDesC& aUri, 
				TMdSFileInfo& aFileInfo);

		/**
		 * Set files to present state.
		 * 
		 * @param aMediaIdAndCount files' media ID and file count
		 * @param aUris serialized files' URIs
		 * @param aFileInfos serialized files' modified dates and byte sizes
		 * @param aResults serialized values from TFilePresentStates
		 * 
		 * @return Error code
		 */
		TInt DoSetFilesToPresent(TMdSMediaIdAndCount aMediaIdAndCount, 
				const TDes8& aUris, const TDes8& aFileInfos, TDes8& aResults);		
		
		
		/**
		 * Set all files, which media ID is given, to not present state.
		 *
		 * @param aMediaId media ID
		 * @param aStartUp is start up or normal unmount
		 */		
		void DoSetFilesToNotPresent(TUint32 aMediaId, TBool aStartUp);

		/**
	     * Remove all objects, which are in not present state, with given media ID
	     * 
		 * @param aMediaId media ID
		 * @param aStartUp is start up or normal mount
	     */
		void DoRemoveFilesNotPresent(TUint32 aMediaId, TBool aStartUp);

		/**
		 * Get schema version's major and minor version.
		 *
		 * @param aMajorVersion returned major version
		 * @param aMinorVersion returned minor version
		 */
		void DoGetSchemaVersionL(TInt& aMajorVersion, TInt& aMinorVersion);

		/**
		 * Set object to "present" state by GUID.
		 *
		 * @param aGuidHigh Guid's high part
		 * @param aGuidLow Guid's low part
		 * 
		 * @leave KErrNotFound MdE can't find object in "not present" state 
		 *                     with matching GUID
		 * 
		 */
		void DoSetObjectToPresentByGuidL( 
				const TInt64& aGuidHigh, const TInt64& aGuidLow );
		
		/**
		 * Set global harvester prioritization chunk to MdE Server. There can 
		 * be only one chunk and observer at once. If MdE Server already 
		 * contains active observer, old observer completes with KErrAbort, 
		 * chunk is always changed to this new one. Observer is added using 
		 * DoAddHarvestingPrioritizationObserver.
         * @param aHandle the handle of global chunk that will receive object 
         *                URIs which need to be prioritize
         */
		void DoSetHarvestingPrioritizationChunkL( RHandleBase aHandle );

		/**
		 * Add harvester prioritization observer to MdE Server. There can be 
		 * only one chunk and observer at once. If MdE Server already contains 
		 * active observer, aStatus completes with KErrAlreadyExists. When MdE 
		 * Server needs prioritize object, aStatus completes with KErrNone.
         * @param aStatus the asynchronous response status which is changed 
         *                when there is object URIs to be prioritize
		 */
		void DoAddHarvestingPrioritizationObserver( TRequestStatus& aStatus );

		/**
		 * Cancel harvester prioritization observer from MdE Server. 
		 * @return If MdE Server doesn't have outstanding observer method 
		 *         returns KErrNotFound, otherwise KErrNone is returned.
		 */
		TInt DoCancelHarvestingPrioritizationObserver();
		
		/**
		 * Change path of objects.
		 * 
		 * @param aOldPath old path
		 * @param aNewPath new path
	     * @param aStatus the asynchronous response status which is changed when 
	     *                URIs are changed
		 */
		void DoChangePath( const TDesC& aOldPath, const TDesC& aNewPath,
				TRequestStatus& aStatus );
		
		/**
		 * Change C-drive media id to MdS_Medias table and update it to all objects.
		 */
		void DoChangeCDriveMediaId();
		
        /**
         * Change intenal mass storage media id in the DB and update objects accordingly
         */
        void DoCheckMassStorageMediaId( const TUint32 aMediaId );
		
		/**
		 * Set "binary composing to file" pending flag to objects
		 *
		 * @param aObjectId object IDs
		 */
		void DoSetPendingL( const RArray<TItemId>& aObjectIds );

		/**
		 * Reset "binary composing to file" pending flag from objects
		 *
		 * @param aObjectId object IDs
		 */
		void DoResetPendingL( const RArray<TItemId>& aObjectIds );

		/**
		 * Get count of objects with "binary composing to file" pending flag
		 *
		 * @param aObjectDef object definition which limits what objects are
		 *                   returned or NULL when all objects are returned
		 * 
		 * @return count of objects
		 */
		TInt DoGetPendingCount( const CMdEObjectDef* aObjectDef );
		
		/**
		 * Get IDs from objects with "binary composing to file" pending flag
		 *
		 * @param aObjectDef object definition which limits what objects are
		 *                   returned or NULL when all objects are returned
		 * @param aObjectIdCount count of returned object IDs
		 * @param aObjectIdBuffer serialized buffer of returned object IDs
		 * 
		 * @return If returned value is > 0, buffer is not large enough for 
		 * 		    objects ID from DB, otherwise error code is returned.
		 */
		TInt DoGetPending( const CMdEObjectDef* aObjectDef, 
				TInt& aObjectIdCount, 
				CMdCSerializationBuffer& aObjectIdBuffer );
		
		/**
		 * Resets DB
		 * only for internal purpose and DEBUG ONLY!!
		 */
		void DoResetDBL();
		
		/**
		 * Check if there was error in client-server communication and notify
		 * session if needed.
		 */
		void NotifySessionError( TInt aError );
		
    private: // Data
    	CMdESessionImpl& iSession;
    
		RProcess iServerProcess;

		TBool iIsSessionOpen;
		TBool iIsSessionProcessOpen;
    };

#endif // __MDEENGINESESSION_H__
