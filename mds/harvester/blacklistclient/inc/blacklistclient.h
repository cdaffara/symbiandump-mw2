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
#ifndef _RBLACKLISTCLIENT_H_
#define _RBLACKLISTCLIENT_H_

// SYSTEM INCLUDE
#include <e32base.h>
#include <e32std.h>
#include <f32file.h>

// FORWARD DECLERATIONS
class CBlacklistItem;

/*!
  @class RBlacklistClient
  @discussion An instance of the RBlacklistClient object 
  */
NONSHARABLE_CLASS( RBlacklistClient ) : public RSessionBase
    {
    public:
        /*!
        @function RBlacklistClient
        @discussion standard C++ constructor
        */
        IMPORT_C RBlacklistClient();
        
        /*!
        @function ~RBlacklistClient
        @discussion Destroy the object and release all memory objects
        */
        IMPORT_C ~RBlacklistClient();
            
    public: // Exported public functions

        /*!
        @function Connect
        @discussion Create connection to server
        @result KErrNone if connected succesfully
        */
        IMPORT_C TInt Connect();
        
        /*!
        @function IsBlacklistedL
        @discussion Destroy the object and release all memory objects.
        @result ETrue if object is blaklisted, EFalse otherwise.
        @param aUri - object uri.
        @param aMediaId - drive id where object is located (E,C ...).
        @param aLastModifiedTime - time when object is modified.
        */
        IMPORT_C TBool IsBlacklistedL( const TDesC& aUri, TUint32 aMediaId, TTime aLastModifiedTime );
        
        /*!
        @function AddL
        @discussion Prepare adding object to blacklist. 
        @param aUri - object uri.
        @param aMediaId - drive id where object is located (E,C ...).
        @param aLastModifiedTime - time when object is modified.
        */
        IMPORT_C void AddL( const TDesC& aUri, TUint32 aMediaId, TTime aLastModifiedTime ) const;
        
        /*!
        @function RemoveL
        @discussion Remove object from blacklist.
        */
        IMPORT_C void RemoveL( const TDesC& aUri, TUint32 aMediaId ) const;
        
        /*!
        @function LoadBlacklistL
        @discussion Load blacklist from server.
        */
        IMPORT_C void LoadBlacklistL();
        
        /*!
        @function CloseDBL
        @discussion Closes server database. 
        */
        IMPORT_C void CloseDBL();
        
        IMPORT_C void Close();

    private: // private functions
    
        /*!
        @function Version
        @discussion Returns version
        @result TVersion - Returns version of the server.
        */
        TVersion Version() const;
        
        /*!
        @function DoLoadBlacklistL
        @discussion sends request load blacklist to server.  
        @param aHandle memory handle, used to create memory chunk.
        */
        void DoLoadBlacklistL( TInt& aHandle ) const;
        
        /*!
        @function AddToMemoryTableL
        @discussion Add object to client list (not actually a memory table).
        @param aLastModifiedTime - time when object is modified.
        @param aUri - object uri.
        @param aMediaId - drive id where object is located (E,C ...).
        */
        void AddToMemoryTableL( const TInt64& aModified, const TDesC& aUri,
            const TUint32 aMediaId );
        
        /*!
        @function RemoveFromMemoryTableL
        @discussion Remove object from client list (not actually a memory table).
        @param aUri - object uri.
        @param aMediaId - drive id where object is located (E,C ...).
        */
        void RemoveFromMemoryTableL( const TDesC& aUri, const TUint32 aMediaId );
        
        /*!
        @function GetListIndex NOTE: WRONG NAMING NOT BASED S60 CODING CONVENTIONS!!!
        @discussion Returns list index.
        */
        TInt GetListIndex( const TDesC& aUri, TUint32 aMediaId );
          
        /*!
        @function RemoveFromDBL
        @discussion removes object from server's database.
        @param aUri - object uri.
        @param aMediaId - drive id where object is located (E,C ...).
        */
        void RemoveFromDBL( const TDesC& aUri, TUint32 aMediaId ) const;  

        /*!
        @function StartServer
        @discussion starts server.
        @result KErrNone if succesfully created.
        */
        TInt StartServer();
  
    private: // private data
       
        /** 
         * Handle to File server session.
         */
        RFs iFs;
        
        /*! @var iSessionOk flag to indicate if session is ok or not.*/
        TBool iSessionOk;
       
       	/*! @var iBlacklistChunk used to create global memory area.*/
       	RChunk iBlacklistChunk;
       
        /*! @var iBlacklistMemoryTable used to keep blacklisted items.*/
        RPointerArray<CBlacklistItem> iBlacklistMemoryTable;

    };

#endif      // _RBLACKLISTCLIENT_H_   
            

