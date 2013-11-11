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
* Description:  This is Metadata engine server session file*
*/


#ifndef __MDESERVERSESSION_H__
#define __MDESERVERSESSION_H__

// INCLUDE FILES
#include <e32base.h>

#include "mdscommoninternal.h"
#include "mdcserializationbuffer.h"

// FORWARD DECLARATIONS
class CMdSServer;
class CMdSFindEngine;
class CMdCSerializationBuffer;

// CLASS DECLARATION
/**
* CServerSession.
*  An instance of class CServerSession is created for each client.
*/
class CMdSServerSession : public CSession2
    {
    public: // Constructors and destructors

        /**
        * NewL.
        * Two-phased constructor.
        * @param aClient The Client's thread.
        * @param aServer The server.
        * @return Pointer to created CMdEServerSession object.
        */
        static CMdSServerSession* NewL( CMdSServer& aServer );

        /**
        * NewLC.
        * Two-phased constructor.
        * @param aClient The Client's thread.
        * @param aServer The server.
        * @return Pointer to created CMdEServerSession object.
        */
        static CMdSServerSession* NewLC( CMdSServer& aServer );

        /**
        * ~CMdEServerSession.
        * Destructor.
        */
        virtual ~CMdSServerSession();

		/**
		* Cache a notification for an entry;
		* @param aId notification ID
		* @param aCompleteCode complete code
		* @param aData serialized notification data, ownership of data changes 
		*              even if method leaves
		*/
		void CacheNotificationL(TInt aId, TUint32 aCompleteCode, CMdCSerializationBuffer* aData);
		
		CMdSServer& GetServer() const;
	
	private: // Internal helper methods
		/**
		* Check that all queries are complete.
		*/ 
		void QueriesCompleteL();

    protected: // Methods for own use

        /**
        * Adds items to database.
        * @param aMessage Message from client
        */
        void AddL( const RMessage2& aMsg );

		/**
		* Adds relation to database.
		* @param aMessage message from client
		*/
		void AddRelationDefL(const RMessage2 &aMsg);

		/**
		* Adds event to database.
		* @param aMessage message from client
		*/		
		void AddEventDefL(const RMessage2 &aMsg);

        /**
        * Removes item from database.
        * @param aMessage Message from client
        */
        void RemoveL( const RMessage2& aMsg );

        /**
        * Removes item from database.
        * @param aMessage Message from client
        */
        void RemoveByUriL( const RMessage2& aMsg, TBool aRemoveEvents );
 
        /**
        * Finds item from database.
        * @param aMessage Message from client
        */
        void FindL( const RMessage2& aMsg );

        /**
        * Finds item from database.
        * @param aMessage Message from client
        */
        void FindAsyncL( const RMessage2& aMsg );

        /**
        * Finds item from database.
        * @param aMessage Message from client
        */
        void FindContinueAsyncL( const RMessage2& aMsg );

        /**
        * Finds item from database and completes message.
        * @param aMessage Message from client
        */
        void FindCancel( const RMessage2& aMsg, TInt aError );

        /**
         * Get object's "base" values from database and completes message.
         * @param aMessage Message from client
         */
        void CheckObjectL( const RMessage2& aMsg );
        
        /**
        * Transfers data from server to client.
        * @param aMessage Message from client
        */
        void GetDataL( const RMessage2& aMsg );

        /**
        * Cancels the modifying of object and unlocks the object.
        * @param aMessage Message from client
        */
        void CancelObjectL( const RMessage2& aMsg );

        /**
        * Commits the modified items to database.
        * @param aMessage Message from client
        */
        void UpdateL( const RMessage2& aMsg );

        /**
        * Registers the client for notifications.
        * @param aMessage Message from client
        */
        void RegisterL( const RMessage2& aMsg );

        /**
        * Continues registration for an existing client.
        * @param aMessage Message from client
        */
        void ListenL( const RMessage2& aMsg );

        /**
        * Unregisters a client for notifications.
        * @param aMessage Message from client
        */
        void UnregisterL( const RMessage2& aMsg );

        /**
        * Executes session shutdown.
        * @param aMessage Message from client
        */
        void ShutdownL( const RMessage2& aMsg );

        /**
        * Import metadata.
        * @param aMessage Message from client
        */
        void ImportMetadataL( const RMessage2& aMsg );

        /**
        * Export metadata
        * @param aMessage Message from client
        */
        void ExportMetadataL( const RMessage2& aMsg );

        /**
        * Import schema
        */
        void ImportSchemaL( const RMessage2& aMsg );

        /**
        * Add memory card media ID
        * @param aMessage Message from client where first argument is media ID
        */
		void AddMemoryCardL(const RMessage2& aMessage);
        
        /**
        * Get the latest memory card's media ID
        *
        * @param aMessage Message from client where first argument is returned 
        *                 Media ID
		* @leave KErrNotFound Not memory cards exist at all 
        */
        void GetMemoryCardL(const RMessage2& aMessage);
        
        /**
         * Check if the memory card's media ID exist in DB
         * @param aMessage Message from client where first argument is media ID
         *                 and second is exists boolean value
         */
        void CheckMemoryCardL(const RMessage2& aMessage);

        /**
         * Set media's media ID, drive and present state to DB.
         * 
         * @param aMessage Message from client where first argument is media ID
         *                 and second is drive and third is present state
         */
        void SetMediaL(const RMessage2& aMessage);

        /**
         * Check if the media's with given media ID exist in DB and return 
         * drive and present state.
         * 
         * @param aMessage Message from client where first argument is media 
         * 				   ID, second is media ID, third is drive and fourth 
         *                 is exists boolean value
         */
        void GetMediaL(const RMessage2& aMessage);

        /**
         * Get present medias' media IDs and drives.
         * 
         * @param aMessage Message from client where first argument is media 
         * 				   count and second is media infos
         */
        void GetPresentMediasL(const RMessage2& aMessage);
        
        /**
         * Sets files object to present state.
         * @param aMessage Message from client where first argument is media ID 
         *                 and file count, second argument is URIs, third 
         *                 argument is TMdSFileInfos and fourth argument is
         *                 result buffer, which are sent back to client
         */
        void SetFilesToPresentL(const RMessage2& aMessage);
        
        /**
        * Sets all file objects which has given media ID to not present state.
        *
        * @param aMessage Message from client where first argument is media ID
        */
        void SetFilesToNotPresentL(const RMessage2& aMessage);
        
        /**
        * Removes all file objects which has given media ID and are in not 
        * present.
        *
        * @param aMessage Message from client where first argument is media ID
        */
        void RemoveFilesNotPresentL(const RMessage2& aMessage);
        
        /**
         * Gets schema version.
         *
         * @param aMessage message from client where first argument is major 
         *                 version number and second is minor version number
         */
        void GetSchemaVersionL(const RMessage2& aMessage);
        
        /**
         * Set object to "present" state by GUID.
         *
         * @param aMessage message from client where first argument is GUID 
         *                 high and second is GUID low
         * 
		 * @leave KErrNotFound MdE can't find object in "not present" state 
		 *                     with matching GUID
         */
        void SetObjectToPresentByGuidL(const RMessage2& aMessage);
        
        /**
         * Change path of objects, which match to the old path, to the new 
         * path.
         * 
         * @param aMessage message from client where first argument is the old 
         *                 path and second is the new path
         */
        void ChangePathL(const RMessage2& aMessage);
        
        /**
         * Change C-drive media id to MdS_Medias table and update all objects that have the old id.
         */
        void ChangeMediaIdL( const RMessage2& aMessage );
        
        /**
         * Check media id of internal mass storage to DB and update items accordingly
         */
        void CheckMassStorageMediaIdL( const RMessage2& aMessage );
        
        /**
         * Set "binary composing to file" pending flag to objects.
         * 
         * @param aMessage message from client where first argument is 
         *                 serialized array of object IDs
         */
        void SetPendingL(const RMessage2& aMessage);

        /**
         * Reset "binary composing to file" pending flag to objects.
         * 
         * @param aMessage message from client where first argument is 
         *                 serialized array of object IDs
         */
        void ResetPendingL(const RMessage2& aMessage);        
        
        /**
		 * Get count of objects with "binary composing to file" pending flag
		 *
         * @param aMessage message from client where first argument is 
         *                 serialized object def ID
         * 
		 * @return if >= 0, object count is returned, otherwise error code is 
		 *         returned
		 */
		TInt GetPendingCountL(const RMessage2& aMessage);
		
		/**
		 * Get IDs from objects with "binary composing to file" pending flag
		 *
         * @param aMessage message from client where first argument is 
         *                 serialized object def ID and second argument is
         *                 returned object ID count and third is serialized 
         *                 array for object IDs
		 *
		 * @return if > 0, buffer is not large enough for objects ID from DB,
		 *         otherwise error code is returned
		 */
		TInt GetPendingL(const RMessage2& aMessage);
        
    public: // Functions from base classes

        /**
         * From CSession2, ServiceL.
         * Service request from client.
         * @param aMessage Message from client
         *                 (containing requested operation and any data).
         */
        void ServiceL( const RMessage2& aMessage );

        /**
         * SizeToRemoteL.
         * Writes size to slave process.
         * @param aMessage Message from client
         * @param aRemote, pointer to client's variable.
         * @param aHierarchy hierarchy that's size is to be transmitted
         */
        static void SizeToRemoteL( const RMessage2& aMsg, TInt aMessageSlot, TInt aSize);

    private: // Constructors and destructors

        /**
        * CMdSServerSession.
        * C++ default constructor.
        * @param aClient The Client's thread.
        * @param aServer The server.
        */
        CMdSServerSession( CMdSServer& aServer );

        /**
        * ConstructL.
        * 2nd phase constructor.
        */
        void ConstructL();

        /**
        * Internal function to handle service request from client.
        * @param aMessage Message from client
        *                 (containing requested operation and any data).
        */
        void ServiceFunctionL( const RMessage2& aMessage );        
        
        CMdCSerializationBuffer* CombineBuffersL(
        		CMdCSerializationBuffer& aLeftBuffer, CMdCSerializationBuffer& aRightBuffer );
        
        CMdCSerializationBuffer* CombineUriNotificationsBuffersL(
                CMdCSerializationBuffer& aLeftBuffer, CMdCSerializationBuffer& aRightBuffer );

        CMdCSerializationBuffer* CombineItemBuffersL(
        		CMdCSerializationBuffer& aLeftBuffer, CMdCSerializationBuffer& aRightBuffer );
        
    private: // Data

        /**
        * iServer, reference to the server.
        */
        CMdSServer& iServer;
        
       /**
        * Find engine
        */
        RPointerArray<CMdSFindEngine> iFindEngines;

	    class CNotificationCacheItem : public CBase
	    	{
	    	public:
	    		CNotificationCacheItem( TInt aId, TUint32 aCode, 
	    			CMdCSerializationBuffer* aData ) : 
		    		iId( aId ), 
		    		iCode( aCode ),
		    		iData( aData )
		    		{
		    		}
		    	
		    	~CNotificationCacheItem()
			    	{
			    	delete iData;
			    	iData = NULL;
			    	}

	    	public:
		    	const TInt iId;
		    	const TUint32 iCode;
		    	CMdCSerializationBuffer* iData;
	    	};

       /**
        * Cache of notification events
        */        
    	RPointerArray<CNotificationCacheItem> iNotificationCache;
    	
	private: // Compare methods
    	
        static TInt ComparePropertiesCacheItem( const CNotificationCacheItem& aFirst, const CNotificationCacheItem& aSecond );
    };

#endif // __MDESERVERSESSION_H__
