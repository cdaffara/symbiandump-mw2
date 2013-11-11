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
* Description:  The Sat Server session manages a connection to a client and
*                handles client requests.
*
*/

#ifndef CSATSSESSION_H
#define CSATSSESSION_H

//  INCLUDES
#include    <e32svr.h>

// FORWARD DECLARATIONS
class TSatEventMediator;
class CSatSServer;
class CSatSSubSession;
class MSatUiSession;
class CSatSServer;
class MSatApi;
class MThreadDeathNotifier;
class CSatIconHandler;
class MSatSIconAPI;
// CLASS DECLARATION

/**
*  CSatSSession represents the server-side session object.
*
*  @lib SatEngine.lib
*  @since Series 60 3.0
*/
class CSatSSession : public CSession2
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aServer SatServer class
        * @param aEventMediator Event mediator class
        * @param aUsatApi An USat API interface
        * @param aThreadDeathNotifier ThreadDeath notifier interface
        */
        static CSatSSession* NewL( CSatSServer* aServer,
            TSatEventMediator& aEventMediator,
            MSatApi& aUsatApi,
            MThreadDeathNotifier& aThreadDeathNotifier );

        /**
        * Destructor.
        */
        virtual ~CSatSSession();

    public: // Functions from base classes

        /**
        * From CSession2 Handles an incoming message.
        * @param aMessage Handle to the client message.
        */
        void ServiceL( const RMessage2 &aMessage );

    public: // New functions

        /**
        * Gives pointer of ui sub session interface. May be null.
        * @return Pointer of ui subsession interface.
        */
        MSatUiSession* UiSubSession();

        /**
        * Closes icon sub-session.
        * @param aMessage Handle to the client message.
        * @return TRUE if subsession exists. Otherwise return FALSE.
        */
        TBool CloseSubSession( const RMessage2& aMessage );

        /**
        * Gets the sub-session using handle.
        * @param aHandle Sub-session handle.
        * @return Sub-session. NULL if not found.
        */
        CSatSSubSession* GetSubSessionFromHandle( TInt aHandle ) const;

        /**
        * Panic the client.
        * @param aMessage The panic message
        * @param aPanic The panic code.
        */
        void PanicClient( const RMessage2& aMessage, TInt aPanic ) const;

        /**
        * Notifies Thread death monitor for thread is dying
        */
        void NotifyThreadDeathMonitor();

        /**
        * Gets pointer to SatServer
        * @param Pointer to SatServer
        */
        CSatSServer* SatServer();

        /**
        * Tells that UI Sub session is closed.
        */
        void UiSessionClosed();

        /**
        * Creates icon handler and returns it to icon sub session
        * return Pointer to created icon handler
        */
        MSatSIconAPI* CreateIconHandlerL();

    private: // New functions

        /**
        * Adds sub session to container. Sub session is deleted
        * if leave occurs.
        * @param aSubSession A Session that is needed to be added
        * @param aMessage An instance of RMessage2
        */
        void AddSubSessionL( CSatSSubSession* aSubSession,
            const RMessage2& aMessage );

    private: // Constructors

        /**
        * ConstrucL
        */
        void ConstructL();

        /**
        * C++ default constructor.
        * @param aServer SatServer class
        * @param aEventMediator Event mediator class
        * @param aUsatApi An USat API interface
        * @param aThreadDeathNotifier ThreadDeath notifier interface
        */
        CSatSSession( CSatSServer* aServer,
            TSatEventMediator& aEventMediator,
            MSatApi& aSatApi,
            MThreadDeathNotifier& aThreadDeathNotifier );

    private: // Data

        // A reference to the server.
        CSatSServer* iSatServer;

        // Event mediator reference
        TSatEventMediator& iEventMediator;

        // Sat api
        MSatApi& iSatApi;

        // Generates a unique handle for subsession.
        CObjectIx* iSubSessionIx;

        // Container for sub-sessions.
        CObjectCon* iSubSessionCon;

        // Interface for ui sub session
        MSatUiSession* iUiSubSession;

        // Thread death monitor notifier
        MThreadDeathNotifier& iThreadDeathNotifier;

        // Indicates is SatUiSubSession closed
        TBool iSatUiSubSessionClosed;

    };

#endif      // CSATSSESSION_H

// End of File
