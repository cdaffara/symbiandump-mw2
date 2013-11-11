/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is the handler for various notifications related to
*                those SIM Application Toolkit proactive commands that
*                uses notifications to inform user about their actions.
*
*/



// INCLUDE FILES
#include    <e32base.h>
#include    <e32svr.h>
#include    "RSatUiSession.h"
#include    "MSatUiObserver.h"
#include    "SatSOpcodes.h"
#include    "CSatCNotifyHandler.h"
#include    "SatLog.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatCNotifyHandler::CSatCNotifyHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized, harmless.
CSatCNotifyHandler::CSatCNotifyHandler(
    TInt aPriority,
    RSatUiSession* aSession ) :
    CActive( aPriority ),
    iSession( aSession ),
    iNotifyData(),
    iNotifyPckg( iNotifyData ),
    iNotifyRsp(),
    iNotifyRspPckg( iNotifyRsp )
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCNotifyHandler::CSatCNotifyHandler calling" )

    // Add to active scheduler.
    CActiveScheduler::Add( this );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCNotifyHandler::CSatCNotifyHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCNotifyHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatCNotifyHandler* CSatCNotifyHandler::NewL(
    RSatUiSession* aSat )
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCNotifyHandler::NewL calling" )

    // Perform construction.
    CSatCNotifyHandler* self =
        new ( ELeave ) CSatCNotifyHandler( EPriorityLow, aSat );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCNotifyHandler::NewL exiting" )
    return self;
    }

// Destructor
CSatCNotifyHandler::~CSatCNotifyHandler()
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCNotifyHandler::~CSatCNotifyHandler calling" )

    // Cancel any outstanding requests.
    Cancel();
    iSession = NULL;

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCNotifyHandler::~CSatCNotifyHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCNotifyHandler::Start
// Starts the handler.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCNotifyHandler::Start()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCNotifyHandler::Start calling" )

    // Empty the IPC data
    TSatNotificationV1 temp;
    iNotifyData = temp;
    TSatNotificationRspV1 temp2;
    temp2.iAccepted = EFalse;
    temp2.iCommand = ESatSSendDataNotify;
    iNotifyRsp = temp2;

    // Request notification.
    TIpcArgs arguments( &iNotifyPckg );

    // Pass the notification IPC package.
    iSession->CreateRequest( ESatSProactiveNotification, arguments, iStatus );

    // Set this handler to active so that it can receive requests.
    SetActive();

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCNotifyHandler::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCNotifyHandler::RunL
// Handles the command.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCNotifyHandler::RunL()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCNotifyHandler::RunL calling" )

    // Check the status of the asnychronous operation
    if ( KErrNone != iStatus.Int() )
        {
        LOG2(
            SIMPLE,
            "SATINTERNALCLIENT: CSatCNotifyHandler::RunL error: %d",
            iStatus.Int() )

        // Renew the request
        Start();
        }

    else
        {
        // This will contain the result of the user query.
        TBool userQuery( ETrue );

        // This will contain EFalse if requested icon is not displayed.
        // And if icon is displayed, it contains ETrue.
        TBool requestedIconDisplayed( EFalse );

        // This will contain the result that has the user pressed end key.
        TBool terminatedByUser( EFalse );
        
        // Has to be casted to TInt before casting to TSatIconQualifier, because
        // GCC warns about the direct cast.
        const struct TSatIconId iconId =
            {
            iNotifyData.iIconId.iIdentifier,
                static_cast<TSatIconQualifier>(
                    static_cast<TInt>(
                        iNotifyData.iIconId.iIconQualifier ) )
            };

        // Send notification to UI
        TSatUiResponse response = iSession->SatUiObserver()->Notification(
            iNotifyData.iCommand,
            iNotifyData.iAlphaIdStatus,
            iNotifyData.iText,
            iconId,
            requestedIconDisplayed,
            iNotifyData.iControlResult );
        
        // Check response
        if ( ESatSuccess != response )
            {
            LOG( SIMPLE, 
            "SATINTERNALCLIENT: CSatCNotifyHandler::RunL ESatSuccess != response" )
            terminatedByUser = ETrue;
            }
        
        // Indicate SAT Server that notification is complete
        iNotifyRsp.iCommand = iNotifyData.iCommand;
        iNotifyRsp.iRequestedIconDisplayed = requestedIconDisplayed;
        iNotifyRsp.iSessionTerminatedByUser = terminatedByUser;
        iNotifyRsp.iAccepted = userQuery;

        // Pass the notification response IPC package.
        TIpcArgs arguments( &iNotifyRspPckg );

        // Perform the IPC data transfer.
        iSession->CreateRequest( ESatSProactiveNotificationResponse, arguments );

        Start();
        }

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCNotifyHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCNotifyHandler::DoCancel
// Cancels the pending request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCNotifyHandler::DoCancel()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCNotifyHandler::DoCancel calling" )

    // Complete the request with cancel code.
    TRequestStatus* requestStatus = &iStatus;
    User::RequestComplete( requestStatus, KErrCancel );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCNotifyHandler::DoCancel exiting" )
    }

//  End of File
