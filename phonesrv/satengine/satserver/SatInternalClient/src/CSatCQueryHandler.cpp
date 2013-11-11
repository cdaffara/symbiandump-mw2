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
* Description:  This is the handler for various query requests related to
*                those SIM Application Toolkit proactive commands that require
*                user permissions to complete their actions.
*
*/



// INCLUDE FILES
#include    <e32base.h>
#include    <e32svr.h>
#include    "RSatUiSession.h"
#include    "MSatUiObserver.h"
#include    "SatSOpcodes.h"
#include    "CSatCQueryHandler.h"
#include    "SatLog.h"

// CONSTANTS

// This will contain the Window Group Id of SAT UI
const TInt KSatUiWgId = -1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatCQueryHandler::CSatCQueryHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized, harmless.
CSatCQueryHandler::CSatCQueryHandler(
    TInt aPriority,
    RSatUiSession* aSession ) :
    CActive( aPriority ),
    iSession( aSession ),
    iQueryData(),
    iQueryPckg( iQueryData ),
    iQueryRsp(),
    iQueryRspPckg( iQueryRsp )
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCQueryHandler::CSatCQueryHandler calling" )

    // Add to active scheduler.
    CActiveScheduler::Add( this );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCQueryHandler::CSatCQueryHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCQueryHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatCQueryHandler* CSatCQueryHandler::NewL(
    RSatUiSession* aSat )
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCQueryHandler::NewL calling" )

    // Perform construction.
    CSatCQueryHandler* self =
        new ( ELeave ) CSatCQueryHandler( EPriorityLow, aSat );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCQueryHandler::NewL exiting" )
    return self;
    }

// Destructor
CSatCQueryHandler::~CSatCQueryHandler()
    {
    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCQueryHandler::~CSatCQueryHandler calling" )

    // Cancel any outstanding requests.
    Cancel();
    iSession = NULL;

    LOG( SIMPLE,
        "SATINTERNALCLIENT: CSatCQueryHandler::~CSatCQueryHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCQueryHandler::Start
// Starts the handler.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCQueryHandler::Start()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCQueryHandler::Start calling" )

    // Empty the IPC data
    TSatQueryV1 temp;
    iQueryData = temp;
    TSatQueryRspV1 temp2;
    temp2.iAccepted = EFalse;
    temp2.iCommand = ESatSSendSmQuery;
    iQueryRsp = temp2;

    // Request queries notification.
    TIpcArgs arguments( &iQueryPckg );

    // Pass the Query IPC package.
    iSession->CreateRequest( ESatSProactiveQuery, arguments, iStatus );

    // Set this handler to active so that it can receive requests.
    SetActive();

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCQueryHandler::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCQueryHandler::RunL
// Handles the command.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCQueryHandler::RunL()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCQueryHandler::RunL calling" )

    // Check the status of the asnychronous operation
    if ( KErrNone != iStatus.Int() )
        {
        LOG2(
            SIMPLE,
            "SATINTERNALCLIENT: CSatCQueryHandler::RunL error: %d",
            iStatus.Int() )

        // Renew the request
        Start();
        }

    else
        {
        // This will contain the result of the user query.
        TBool userQuery( EFalse );

        // This will contain EFalse if requested icon is not displayed.
        // And if icon is displayed, it contains ETrue.
        TBool requestedIconDisplayed( EFalse );

        // This will contain the result that has the user pressed end key.
        TBool terminatedByUser( EFalse );

        // Use general configm function
        iSession->SatUiObserver()->ConfirmCommand(
            iQueryData.iCommand,
            iQueryData.iAlphaIdStatus,
            iQueryData.iQueryText,
            iQueryData.iSimApplicationName,
            userQuery,
            iQueryData.iIconId,
            requestedIconDisplayed,
            terminatedByUser );

        // Return the result of the query to the server.
        iQueryRsp.iAccepted = userQuery;
        iQueryRsp.iCommand = iQueryData.iCommand;
        iQueryRsp.iWgId = KSatUiWgId;
        iQueryRsp.iRequestedIconDisplayed = requestedIconDisplayed;
        iQueryRsp.iSessionTerminatedByUser = terminatedByUser;

        // Pass the Query response IPC package.
        TIpcArgs arguments( &iQueryRspPckg );

        // Perform the IPC data transfer.
        iSession->CreateRequest( ESatSProactiveQueryResponse, arguments );

        Start();
        }

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCQueryHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatCQueryHandler::DoCancel
// Cancels the pending request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatCQueryHandler::DoCancel()
    {
    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCQueryHandler::DoCancel calling" )

    // Complete the request with cancel code.
    TRequestStatus* requestStatus = &iStatus;
    User::RequestComplete( requestStatus, KErrCancel );

    LOG( SIMPLE, "SATINTERNALCLIENT: CSatCQueryHandler::DoCancel exiting" )
    }

//  End of File
