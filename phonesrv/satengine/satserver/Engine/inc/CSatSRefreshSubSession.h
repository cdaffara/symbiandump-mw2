/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles the refresh sub-session commands.
*
*/


#ifndef CSATSREFRESHSUBSESSION_H
#define CSATSREFRESHSUBSESSION_H

#include "CSatSSubSession.h"
#include "MSatRefreshSubSession.h"
#include "TSatEventMediator.h"

class CSatSSession;
class MSatCommand;

/**
*  Handles the refresh commands.
*
*  @lib SatServer
*  @since 2.6
*/
class CSatSRefreshSubSession : public CSatSSubSession,
                               public MSatRefreshSubSession
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aSession Session
        * @param aEventMediator Event handler
        */
        static CSatSRefreshSubSession* NewL(
            CSatSSession& aSession,
            TSatEventMediator& aEventMediator );

        /**
        * Destructor.
        */
        virtual ~CSatSRefreshSubSession();

    public: // Functions from base classes

        /**
        * From CSatSSubSession
        * @param aMessage Message from the client.
        */
        void HandleCommandL( const RMessage2& aMessage );

        /**
        * From MSatRefreshSubSession.
        * @param aRefresh Refresh data.
        */
        void QueryClient( const RSat::TRefreshV2& aRefresh );

        /**
        * From MSatRefreshSubSession.
        * @param aRefresh Refresh data.
        */
        void NotifyClient( const RSat::TRefreshV2& aRefresh );

        /**
        * From MSatRefreshSubSession.
        */
        void NotifyClientCancel();

    private: // New functions

        /**
        * Handles the NotifyRefresh command from SatClient.
        * @param aMessage Message from the client.
        */
        void NotifyRefreshAllowed( const RMessage2& aMessage );

        /**
        * Cancels the allow refresh.
        * @param aMessage Message from the client.
        */
        void CancelNotifyAllowRefresh( const RMessage2& aMessage );

        /**
        * Handles the RefreshAllowed command from SatClient.
        * @param aMessage Message from the client.
        */
        void RefreshAllowed( const RMessage2& aMessage );

        /**
        * Handles the refresh notification.
        * @param aMessage Message from the client.
        */
        void NotifyRefresh( const RMessage2& aMessage );

        /**
        * Cancels the notify refresh.
        * @param aMessage Message from the client.
        */
        void CancelNotifyRefresh( const RMessage2& aMessage );

        /**
        * Sends the client's completion to SatServer.
        * @param aMessage Message from the client.
        */
        void CompleteRefresh( const RMessage2& aMessage );

        /**
        * Checks whether current client is interested about this refresh.
        * @param aRefresh Refresh data.
        */
        TBool VerifyObservedFiles( const RSat::TRefreshV2& aRefresh ) const;

        /**
        * Timer call back for clients refresh query response timeout.
        * @param aRefreshSubSession Reference to this class.
        */
        static TInt TimerQueryCallback( TAny* aRefreshSubSession );

        /**
        * Timeout handler for the client's response of refresh query.
        */
        void QueryTimeout();

        /**
        * Timer call back for clients refresh notify response timeout.
        * @param aRefreshSubSession Reference to this class.
        */
        static TInt TimerNotifyCallback( TAny* aRefreshSubSession );

        /**
        * Timeout handler for the client's response of refresh notify.
        */
        void NotifyTimeout();

    private:

        /**
        * C++ default constructor.
        * @param aSession
        * @param aEventMediator Event handler
        */
        CSatSRefreshSubSession(
            CSatSSession& aSession,
            TSatEventMediator& aEventMediator );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // Message for allow refresh.
        RMessage2 iRefreshAllowMessage;

        // Indicates if allow refresh has not been complited.
        TBool iRefreshAllowPending;

        // Messge for refresh.
        RMessage2 iNotifyRefreshMessage;

        // Indicates if refresh has not been completed.
        TBool iNotifyRefreshPending;

        // Event mediator.
        TSatEventMediator& iEventMediator;

        // Timer for securing user query.
        CPeriodic* iQueryTimer;

        // Timer for securing refresh complete.
        CPeriodic* iNotifyTimer;

        // A client has been queried to allow refresh and we are waiting
        // for response. Enables discarding of timed out responses.
        TBool iClientQueryPending;

        // A client has been notified about refresh and we are waiting
        // for response. Enables discarding of timed out responses.
        TBool iClientNotifyPending;

        // A client has allowed refresh and is waiting for Refesh notification.
        // Flag is used when notificating Refresh cancel for clients waiting.
        TBool iClientWaitingRefresh;

    };

#endif      // CSATSREFRESHSUBSESSION_H
