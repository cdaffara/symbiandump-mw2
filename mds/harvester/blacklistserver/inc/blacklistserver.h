/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:
*
*/
#ifndef __BLACKLISTSERVER_H__
#define __BLACKLISTSERVER_H__

// SYSTEM INCLUDE
#include <e32base.h>    // CTrapCleanup
#include <f32file.h>

// FORWARD DECLERATIONS
class CMdSSqLiteConnection;
class CBlacklistItem;
class RRowData;

/*!
  @class CBlacklistServer
  @discussion An instance of the CBlacklistServer object 
  */
class CBlacklistServer : public CPolicyServer
    {
    public:
         /*!
        @function NewL
        @discussion factory function.
        */
        static CBlacklistServer* NewL();

        /*!
        @function NewLC
        @discussion factory function, pointer is in cleanupstack.
        */
        static CBlacklistServer* NewLC();      

        /*!
        @function ~CBlacklistServer
        @discussion standard C++ destructor.
        */
        virtual ~CBlacklistServer();

        /*!
        @function AddSession
        @discussion adds new session.
        */
        void AddSession();
        /*!
        @function RemoveSession
        @discussion removes session.
        */
        void RemoveSession();

        /*!
        @function MemoryHandle
        @discussion returns memory handle.
        @result integer value to indicate memory handle.
        */
        TInt MemoryHandle();

        /*!
        @function OpenDatabaseL
        @discussion Opens server database.
        */
        void OpenDatabaseL();
 
        /*!
        @function AddL
        @discussion Add object to server's list.
        @param aMediaId - drive id where object is located (E,C ...).
        @param aUri - object uri.
        @param aLastModifiedTime - time when object is modified.
        */
        void AddL( const TUint32& mediaId, const TDesC& aUri, const TTime& aLastModifiedTime );

        /*!
        @function RemoveL
        @discussion Removes object from server's list.
        @param aUri - object uri.
        @param aMediaId - drive id where object is located (E,C ...).
        */
        void RemoveL( const TUint32& mediaId, const TDesC& aUri );

        /*!
        @function RemoveFromDBL
        @discussion removes object from server's database.
        @param aUri - object uri.
        @param aMediaId - drive id where object is located (E,C ...).
        */
        void RemoveFromDBL( const TDesC& aUri, const TUint32 aMediaId );

        /*!
        @function AddToDBL
        @discussion Adds object to server's database.
        @param aUri - object uri.
        @param aMediaId - drive id where object is located (E,C ...).
        @param aLastModifiedTime - time when object is modified.
        */
        void AddToDBL( const TDesC& aUri, const TUint32 aMediaId, const TInt64& aLastModifiedTime );

        /*!
        @function CloseDB
        @discussion closes server's database.
        */
        void CloseDB();

        /*!
        @function ExeMainL
        @discussion creates server. 
        */
        static void ExeMainL(); 
    
    protected: // Functions from base classes

        /*!
        @function CustomSecurityCheckL
        @discussion check if service is allowed.
        */
        CPolicyServer::TCustomResult CustomSecurityCheckL( 
            const RMessage2 &aMsg, TInt &aAction, TSecurityInfo &aMissing );

        /*!
        @function CustomFailureActionL
        @discussion not used.
        */
        CPolicyServer::TCustomResult CustomFailureActionL(
            const RMessage2 &aMsg, TInt aAction, const TSecurityInfo &aMissing );   
    private:

        /*!
        @function CBlacklistServer
        @discussion standard C++ constructor
        */
        CBlacklistServer();

        /*!
        @function RBlacklistClient
        @discussion two phase constructor
        */
        void ConstructL();

        /*!
        @function NewSessionL
        @discussion creates new session
        @param aVersion, server version.
        @param aMessage, not used.
        @result return blacklist session.
        */
        CSession2* NewSessionL( const TVersion& aVersion, const RMessage2& aMessage ) const;
        
        /*!
        @function NewSessionL
        @discussion creates new session
        @param aVersion, server version.
        @result KErrNone or error if server RunL fails. 
        */
        TInt RunError( TInt aError );

    private:

          /*!
        @function CloseDatabase
        @discussion closes server database.
        */
        void CloseDatabase();

        /*!
        @function LoadDatabaseToMemoryL
        @discussion load database to server's list
        */
        void LoadDatabaseToMemoryL();
     
        /*!
        @function CreateBlacklistTableL
        @discussion Creates blacklist table.
        */
        void CreateBlacklistTableL();
     
        /*!
        @function AddToMemoryTableL
        @discussion adds object to server's list.
        @param aLastModifiedTime - time when object is modified.
        @param aUri - object uri.
        @param aMediaId - drive id where object is located (E,C ...).
        */
        void AddToMemoryTableL( const TInt64& aModified, const TDesC& aUri,
            const TUint32 aMediaId );
     
        /*!
        @function RemoveFromMemoryTable
        @discussion removes object from server's list 
        @param aUri - object uri.
        @param aMediaId - drive id where object is located (E,C ...).
        */
        void RemoveFromMemoryTable( const TDesC& aUri,
            const TUint32 aMediaId );
     
        /*!
        @function GetMemoryTableIndex
        @discussion returns list index. 
        @param aUri - object uri.
        @param aMediaId - drive id where object is located (E,C ...).
        @result returns list index.        
        */
        TInt GetMemoryTableIndex( const TDesC& aUri, TUint32 aMediaId );

        /*!
        @function TableExistsL
        @discussion 
        @result ETrue if table exists, EFalse otherwise. 
        */
        TBool TableExistsL();
        
        /*!
        @function SerializeToSharedMemoryL
        @discussion creates serialized buffer.
        */
        void SerializeToSharedMemoryL();
        
        /*!
        @function WriteAllDataToDBL
        @discussion write all data to server's database.
        */
        void WriteAllDataToDBL();
        
        /*!
        @function CommitBufferedItemsL
        @discussion commits buffered items to server's database.
        */
        void CommitBufferedItemsL();

    private: // private data
    
        /*! @var iSessionCount to indicate session count.*/
        TInt iSessionCount;
    	
        /*! @var iBlacklistServerChunk global memory chunk.*/
    	RChunk iBlacklistServerChunk;
        
        /*! @var iSqLiteConnection to handle database connections.*/
        CMdSSqLiteConnection* iSqLiteConnection;
        
        /*! @var iDatabaseOpen flag to indicate if database is open or not.*/
        TBool iDatabaseOpen;
        
        /*! @var iDBUpdateNeeded flag to indicate if databse need updating.*/
        TBool iDBUpdateNeeded;
        
        /*! @var iBlacklistMemoryTable contains objects which are serialized to client.*/
        RPointerArray<CBlacklistItem> iBlacklistMemoryTable;

        /*! @var iBufferedRemoveItems contains all buferred removed items.*/
        RPointerArray<RRowData> iBufferedRemoveItems;

        /*! @var iBufferedAddedItems contains all buferred added items.*/
        RPointerArray<RRowData> iBufferedAddedItems;

        /*! @var iHandle server handle.*/
        TInt iHandle;
    };

#endif // __BLACKLISTSERVER_H__
