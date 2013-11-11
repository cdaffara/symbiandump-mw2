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
* Description:  Handler for the SS Additional Info notification
*
*/


#include    "csatsendssadditionalinfohandler.h"
#include    "CSendSsHandler.h"
#include    "SatLog.h"

// ================= MEMBER FUNCTIONS ==========================================

// The class constructor.
CSatSendSsAdditionalInfoHandler::CSatSendSsAdditionalInfoHandler(
    TInt aPriority, RMmCustomAPI& aPhone,
    CSendSSHandler* aDispatcher )
    : CActive( aPriority ), iPhone( aPhone ),
    iDispatcher( aDispatcher )
    {
    LOG( SIMPLE,
        "CSatSendSsAdditionalInfoHandler::CSatSendSsAdditionalInfoHandler \
        calling" )

    // Add to active scheduler
    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "CSatSendSsAdditionalInfoHandler::CSatSendSsAdditionalInfoHandler \
        exiting" )
    }

// Destructor
CSatSendSsAdditionalInfoHandler::~CSatSendSsAdditionalInfoHandler()
    {
    LOG( SIMPLE,
        "CSatSendSsAdditionalInfoHandler::~CSatSendSsAdditionalInfoHandler \
        calling" )

    // Cancel any outstanding requests.
    Cancel();

    iDispatcher = NULL;

    LOG( SIMPLE,
        "CSatSendSsAdditionalInfoHandler::~CSatSendSsAdditionalInfoHandler \
        exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsAdditionalInfoHandler::NewL
// -----------------------------------------------------------------------------
//
CSatSendSsAdditionalInfoHandler* CSatSendSsAdditionalInfoHandler::NewL(
    RMmCustomAPI& aPhone, CSendSSHandler* aDispatcher )
    {
    LOG( SIMPLE, "CSatSendSsAdditionalInfoHandler::NewL calling" )

    // Perform construction
    CSatSendSsAdditionalInfoHandler* self =
        new ( ELeave ) CSatSendSsAdditionalInfoHandler(
            EPriorityStandard, aPhone, aDispatcher );

    LOG( SIMPLE, "CSatSendSsAdditionalInfoHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatSendSsAdditionalInfoHandler::Start
// Starts the handler.
// -----------------------------------------------------------------------------
//
void CSatSendSsAdditionalInfoHandler::Start()
    {
    LOG( SIMPLE, "CSatSendSsAdditionalInfoHandler::Start calling" )

    // Clear the IPC package
    RMmCustomAPI::TSsAdditionalInfo temp;
    iSsAdditionalInfo = temp;

    if ( !IsActive() )
        {
        LOG( SIMPLE, "CSatSendSsAdditionalInfoHandler::Start set active" )
        // Request the notification.
        iPhone.SsAdditionalInfoNotification( iStatus, iSsAdditionalInfo );
        // Set to active so that requests can be received.
        SetActive();
        }

    LOG( SIMPLE, "CSatSendSsAdditionalInfoHandler::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsAdditionalInfoHandler::RunL
// Handles the command.
// -----------------------------------------------------------------------------
//
void CSatSendSsAdditionalInfoHandler::RunL()
    {
    LOG( SIMPLE, "CSatSendSsAdditionalInfoHandler::RunL calling" )

    TBuf16<RSat::KAdditionalInfoMaxSize> additionalInfo;
    LOG2( SIMPLE, "CSatSendSsAdditionalInfoHandler::RunL iStatus: %d",
          iStatus.Int() )
    // Check the status of the asnychronous operation
    if ( iStatus == KErrNone )
        {
        additionalInfo.Append( iSsAdditionalInfo.iOperationCode );
        TInt i = 0;
        while ( i < iSsAdditionalInfo.iAdditionalInfo.Length()
            && additionalInfo.Length() < RSat::KAdditionalInfoMaxSize )
            {
            additionalInfo.Append( iSsAdditionalInfo.iAdditionalInfo[i++] );
            }
        LOG2( SIMPLE, "CSatSendSsAdditionalInfoHandler::RunL i: %d", i )
        }

    iDispatcher->DispatchSsAdditionalInfo( additionalInfo );

    if ( KErrNone == iStatus.Int() )
        {
        // Renew the notification request.
        Start();
        }

    LOG( SIMPLE, "CSatSendSsAdditionalInfoHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSendSsAdditionalInfoHandler::DoCancel
// Cancels the pending request.
// -----------------------------------------------------------------------------
//
void CSatSendSsAdditionalInfoHandler::DoCancel()
   {
   LOG( SIMPLE, "CSatSendSsAdditionalInfoHandler::DoCancel calling" )

   // Cancel the outstanding request.
   iPhone.CancelAsyncRequest( ECustomSsAdditionalInfoNotificationIPC );

   LOG( SIMPLE, "CSatSendSsAdditionalInfoHandler::DoCancel exiting" )
   }

//  End of File
