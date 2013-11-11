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
* Description:  Handler for the SS Request Complete notification
*
*/


#include    "CSendSsHandler.h"
#include    "csatsendssrequestcompletehandler.h"
#include    "SatLog.h"

// ================= MEMBER FUNCTIONS =======================

// The class constructor.
CSatSendSsRequestCompleteHandler::CSatSendSsRequestCompleteHandler(
    TInt aPriority, RMmCustomAPI& aPhone,
    CSendSSHandler* aDispatcher )
    : CActive( aPriority ), iPhone( aPhone ),
    iDispatcher( aDispatcher )
    {
    LOG( SIMPLE,
        "CSatSendSsRequestCompleteHandler::CSatSendSsRequestCompleteHandler \
        calling" )

    // Add to active scheduler
    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "CSatSendSsRequestCompleteHandler::CSatSendSsRequestCompleteHandler \
        exiting" )
    }

// Two-phased constructor.
CSatSendSsRequestCompleteHandler* CSatSendSsRequestCompleteHandler::NewL(
    RMmCustomAPI& aPhone, CSendSSHandler* aDispatcher )
    {
    LOG( SIMPLE, "CSatSendSsRequestCompleteHandler::NewL calling" )

    // Perform construction
    CSatSendSsRequestCompleteHandler* self =
        new ( ELeave ) CSatSendSsRequestCompleteHandler(
            EPriorityLow, aPhone, aDispatcher );

    LOG( SIMPLE, "CSatSendSsRequestCompleteHandler::NewL exiting" )
    return self;
    }

// Destructor
CSatSendSsRequestCompleteHandler::~CSatSendSsRequestCompleteHandler()
    {
    LOG( SIMPLE,
        "CSatSendSsRequestCompleteHandler::~CSatSendSsRequestCompleteHandler \
        calling" )

    // Cancel any outstanding requests.
    Cancel();

    LOG( SIMPLE,
        "CSatSendSsRequestCompleteHandler::~CSatSendSsRequestCompleteHandler \
        exiting" )
    iDispatcher = NULL;
    }

// -----------------------------------------------------------------------------
// CSatSendSsRequestCompleteHandler::Start
// Starts the handler.
// -----------------------------------------------------------------------------
//
void CSatSendSsRequestCompleteHandler::Start()
    {
    LOG( SIMPLE, "CSatSendSsRequestCompleteHandler::Start calling" )

    if ( !IsActive() )
        {
        LOG( SIMPLE, "CSatSendSsRequestCompleteHandler::Start set active" )
        // Request the notification.
        iPhone.SsRequestCompleteNotification( iStatus, iSsStatus );
        // Set to active so that requests can be received.
        SetActive();
        }

    LOG( SIMPLE, "CSatSendSsRequestCompleteHandler::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsRequestCompleteHandler::RunL
// Handles the command.
// -----------------------------------------------------------------------------
//
void CSatSendSsRequestCompleteHandler::RunL()
    {
    LOG( SIMPLE, "CSatSendSsRequestCompleteHandler::RunL calling" )

    // Check the status of the asnychronous operation
    if ( iStatus == KErrNone )
        {
        LOG( NORMAL, " iStatus == KErrNone" )
        iDispatcher->DispatchSsRequestComplete( iSsStatus );
        // Renew the notification request.
        Start();
        }
    else
        {
        LOG2( NORMAL, "  iStatus == %i", iStatus.Int() )
        iDispatcher->DispatchSsRequestComplete( KErrGeneral );
        }

    LOG( SIMPLE, "CSatSendSsRequestCompleteHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsRequestCompleteHandler::DoCancel
// Cancels the pending request.
// -----------------------------------------------------------------------------
//
void CSatSendSsRequestCompleteHandler::DoCancel()
   {
   LOG( SIMPLE, "CSatSendSsRequestCompleteHandler::DoCancel calling" )

   // Cancel the outstanding request.
   iPhone.CancelAsyncRequest( ECustomNotifySsRequestCompleteIPC );

   LOG( SIMPLE, "CSatSendSsRequestCompleteHandler::DoCancel exiting" )
   }

//  End of File
