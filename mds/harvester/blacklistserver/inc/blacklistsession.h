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
#ifndef __SESSION_H__
#define __SESSION_H__

// SYSTEM INCLUDE
#include <e32base.h>

// FORWARD DECLERATIONS
class CBlacklistServer;

/*!
  @class CBlacklistSession
  @discussion An instance of the CBlacklistSession object 
  */
class CBlacklistSession : public CSession2
    {
    public:

        /*!
        @function CBlacklistSession
        @discussion standard C++ constructor.
        */
        CBlacklistSession();
        
        /*!
        @function CreateL
        @discussion Adds session to server.
        */
        void CreateL();

    private: // private functions
 
         /*!
        @function LoadBlacklistDataL
        @discussion forward request to server to load blacklist.
        @param aMessage, contains service request.
        */
        void LoadBlacklistDataL( const RMessage2& aMessage ); 
        
         /*!
        @function AddL
        @discussion forward request to server to add item to blacklist.
        @param aMessage, contains added item's data.
        */
        void AddL( const RMessage2& aMessage );
        
         /*!
        @function RemoveL
        @discussion forward request to server to remove item from blacklist.
        @param aMessage, contains removed item's data.
        */
        void RemoveL( const RMessage2& aMessage );  
        
         /*!
        @function RemoveFromDBL
        @discussion forward request to server to remove item from database.
        @param aMessage, contains removed item's data.
        */
        void RemoveFromDBL( const RMessage2& aMessage );
        
         /*!
        @function CloseDB
        @discussion forward request to server to close database.
        @param aMessage, empty, not used.
        */
        void CloseDB( const RMessage2& aMessage );  
        
         /*!
        @function ~CBlacklistSession
        @discussion standard C++ destructor. Send request to server to remove session.
        */
        ~CBlacklistSession();

         /*!
        @function Server
        @discussion factory function.
        @result returns reference to server.
        */
        CBlacklistServer& Server();
        
         /*!
        @function ServiceL
        @discussion handles service requests.
        @param aMessage, contains service request.
        */
        void ServiceL( const RMessage2& aMessage );
    };

#endif
