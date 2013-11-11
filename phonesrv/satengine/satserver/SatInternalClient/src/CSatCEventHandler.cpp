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
* Description:  This is the handler for various events related to
*                those SIM Application Toolkit proactive commands that
*                simply indicate that an action occured at the server.
*
*/



// INCLUDE FILES
#include    <e32svr.h>
#include    <basched.h>
#include    "RSatUiSession.h"
#include    "MSatUiObserver.h"
#include    "CSatCEventHandler.h"
#include    "SatSOpcodes.h"
#include    "SatSTypes.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatCEventHandler::CSatCEventHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1769, 1403} Can not be initialized.
CSatCEventHandler::CSatCEventHandler(
    TInt aPriority,
    RSatUiSession* aSession ) :
    CActive( aPriority ),
    iSession( aSession ),
    iEventData(),
    iEventPckg( iEventData )
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCEventHandler::CSatCEventHandler calling" )

    // Add to active scheduler.
    CActiveScheduler::Add( this );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCEventHandler::CSatCEventHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCEventHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatCEventHandler* CSatCEventHandler::NewL(
    RSatUiSession* aSat )
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCEventHandler::NewL calling" )

    // Perform construction.
    CSatCEventHandler* self =
        new ( ELeave ) CSatCEventHandler( EPriorityLow, aSat );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCEventHandler::NewL exiting" )
    return self;
    }

// Destructor
CSatCEventHandler::~CSatCEventHandler()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCEventHandler::~CSatCEventHandler calling" )

    // Cancel any outstanding requests.
    Cancel();
    iSession = NULL;

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCEventHandler::~CSatCEventHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCEventHandler::Start
// Starts the handler.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCEventHandler::Start()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCEventHandler::Start calling" )

    // Request notifications about various server-side events.
    TIpcArgs arguments( &iEventPckg );
    iSession->CreateRequest( ESatSProactiveEvent, arguments, iStatus );

    // Set this handler to active so that it can receive requests.
    SetActive();

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCEventHandler::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCEventHandler::RunL
// Handles the command.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCEventHandler::RunL()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCEventHandler::RunL calling" )

    // Check the status of the asnychronous operation
    if ( KErrNone != iStatus.Int() )
        {
        LOG2(
            SIMPLE,
            "SATINTERNALCLIENT: CSatCEventHandler::RunL exiting, error: %d",
            iStatus.Int() )

        // Renew the request
        Start();

        return;
        }

    // Send event to UI
    iSession->SatUiObserver()->EventNotification( 
        iEventData.iEvent,
        iEventData.iStatus,
        iEventData.iError );
        
    if ( ESatSCloseSatUiAppEvent == iEventData.iEvent )
        {
        LOG( SIMPLE, "SATINTERNALCLIENT: CSatCEventHandler::RunL close ui" )
        // Close UI
        CBaActiveScheduler::Exit();
        }


    // Pass the Event response IPC package.
    TIpcArgs arguments( &iEventPckg );

    // Perform the IPC data transfer.
    iSession->CreateRequest( ESatSProactiveEventResponse, arguments );

    // Renew the service request.
    Start();

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCEventHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCEventHandler::DoCancel
// Cancels the pending request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCEventHandler::DoCancel()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCEventHandler::DoCancel calling" )

    // Complete the request with cancel code.
    TRequestStatus* requestStatus = &iStatus;
    User::RequestComplete( requestStatus, KErrCancel );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCEventHandler::DoCancel exiting" )
    }

//  End of File
