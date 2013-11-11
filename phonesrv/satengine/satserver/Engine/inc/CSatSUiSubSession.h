/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles ui subsession
*
*/



#ifndef CSATSUISUBSESSION_H
#define CSATSUISUBSESSION_H

//  INCLUDES
#include    "CSatSSubSession.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "SatSTypes.h"
#include    "MSatCommand.h"
#include    "CSatClientServiceReq.h"
#include    "MSatEventObserver.h"
#include    "TSatEventMediator.h"

// FORWARD DECLARATIONS
class CSatSSession;
class MSatApi;


// CLASS DECLARATION

/**
*  Handles ui subsession.
*
*  @lib SatEngine.lib
*  @since Series 60 3.0
*/
class CSatSUiSubSession : public CSatSSubSession,
                          public MSatUiSession,
                          public MSatEventObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aSession Parent session
        * @param aSatApi SAT API Interface
        * @param aEventMediator Event handler
        */
        static CSatSUiSubSession* NewL( CSatSSession& aSession,
            MSatApi& aSatApi,
            TSatEventMediator& aEventMediator );

        /**
        * Destructor.
        */
        virtual ~CSatSUiSubSession();

    public: // Functions from base classes

        /**
        * From CSatSSubSession Handler for IPC messages.
        * @param aMessage IPC message from client.
        */
        void HandleCommandL( const RMessage2& aMessage );

        /**
        * From MSatUiSession Closes the ui sub session.
        */
        void CloseUiSession();

        /**
        * From MSatUiSession Clears the screen.
        */
        void ClearScreen();

        /**
        * From MSatUiSession. Notification that command data is available.
        * @param aCmdData Data to be sent to client.
        * @param aCmdRsp Client response
        * @param aRequest Request, where the data is sent.
        */
        void SendCommand(
            TDesC8* aCmdData,
            TDes8* aCmdRsp,
            TSatServerRequest aRequest );

        /**
        * From MSatUiSession.Ui Event.
        */
        void UiEventNotification( 
            TInt aEventId, 
            TInt aEventStatus, 
            TInt aEventError );

        /**
        * From MSatUiSession Queries the user.
        * @param aCommand Command handler interface
        */
        void QueryUser( MSatCommand* aCommand );

        /**
        * From MSatEventObserver.
        * @param aEvent An event that is notified
        */
        void Event( TInt aEvent );

    protected:  // New functions

        /**
        * Sends the command data to client if the request is available.
        * @param aRequestAvailable Indication if request is available. If
        * data is sent aRequestAvailable will be set to EFalse.
        * @param aData Data to be sent to client.
        * @param aMessage Clients request message.
        * @return Boolean value indicating was the command sent to client or not
        */
        TBool SendCommandDataToClient(
            TBool& aRequestAvailable,
            const TDesC8& aData,
            const RMessage2& aMessage ) const;

        /**
        * Sends ESatSSmEndEvent event to UI Client
        */
        void SendSmsSentEventToClient();
        
        /**
        * Handles UI Event notification
        */
        void HandleUiEventNotification();
        /**
        * Publish the P&S key KSatAppClosedUsingEndKey to indicate the occurred
        * End Key action
        */
        void PublishSatAppClosedUsingEndKey();

    private:

        /**
        * C++ default constructor.
        * @param aSession Parent session
        * @param aSatApi SAT API Interface
        * @param aEventMediator Event handler
        */
        CSatSUiSubSession( CSatSSession& aSession,
            MSatApi& aSatApi,
            TSatEventMediator& aEventMediator );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // Request / Response handlers array
        CArrayPtrFlat<CSatClientServiceReq>* iServiceRequest;

        // Indicates that event request is availabe
        TBool iEventRequestAvailable;

        // Indicates that some command has issued close ui session
        TBool iCloseUiRequested;

        // Indicates that DisplayText has requested screen to be cleared
        TBool iClearScreenRequested;

        // Indicates that sms has been sent by CSendSmCmd
        TBool iSmsSent;
        
        // Indicates is there event pending
        TBool iUiEventRequested;

        // Event message
        RMessage2 iEventRequest;

        // Event data
        TSatEventV1 iEventData;

        // Event data package
        TSatEventV1Pckg iEventDataPckg;

        // Sat api
        MSatApi& iSatApi;

        // Query request
        RMessage2 iQueryRequest;

        // Event mediator
        TSatEventMediator& iEventMediator;
    };

#endif      // CSATSUISUBSESSION_H

// End of File
