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
* Description:  This is the handler for the SIM Application Toolkit
*                Send Message No logging.
*
*/



// INCLUDE FILES
#include    <e32base.h>
#include    "MSatApi.h"
#include    "CSatSSendMessageNoLoggingHandler.h"
#include    "MSatSmsObserver.h"
#include    "SatLog.h"

// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CSatSSendMessageNoLoggingHandler::CSatSSendMessageNoLoggingHandler
// The class constructor.
// -----------------------------------------------------------------------------
//
CSatSSendMessageNoLoggingHandler::CSatSSendMessageNoLoggingHandler(
    MSatApi& aSatApi,
    MSatSmsObserver& aObserver ) :
    CActive( CActive::EPriorityStandard ),
    iSmsData(),
    iSmsPckg( iSmsData ),
    iSatApi( aSatApi ),
    iObserver( aObserver )
    {
    LOG( SIMPLE, "SENDSM: CSatSSendMessageNoLoggingHandler::\
        CSatSSendMessageNoLoggingHandler calling" )

    CActiveScheduler::Add( this );

    LOG( SIMPLE, "SENDSM: CSatSSendMessageNoLoggingHandler::\
        CSatSSendMessageNoLoggingHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSendMessageNoLoggingHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatSSendMessageNoLoggingHandler* CSatSSendMessageNoLoggingHandler::NewL(
    MSatApi& aSatApi,
    MSatSmsObserver& aObserver )
    {
    LOG( SIMPLE, "SENDSM: CSatSSendMessageNoLoggingHandler::NewL calling" )

    CSatSSendMessageNoLoggingHandler* self =
        new ( ELeave ) CSatSSendMessageNoLoggingHandler( aSatApi, aObserver );

    LOG( SIMPLE, "SENDSM: CSatSSendMessageNoLoggingHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatSSendMessageNoLoggingHandler::~CSatSSendMessageNoLoggingHandler
// Destructor.
// -----------------------------------------------------------------------------
//
CSatSSendMessageNoLoggingHandler::~CSatSSendMessageNoLoggingHandler()
    {
    LOG( SIMPLE, "SENDSM: CSatSSendMessageNoLoggingHandler::\
        ~CSatSSendMessageNoLoggingHandler calling" )

    Cancel();

    LOG( SIMPLE, "SENDSM: CSatSSendMessageNoLoggingHandler::\
        ~CSatSSendMessageNoLoggingHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSendMessageNoLoggingHandler::Start
// Starts the handler.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSSendMessageNoLoggingHandler::Start(
    const RSat::TSendSmV1& aSendSmData )
    {
    LOG( SIMPLE, "SENDSM: CSatSSendMessageNoLoggingHandler::Start calling" )

    iSmsData.iServiceCenter = aSendSmData.iAddress;
    iSmsData.iBuf = aSendSmData.iSmsTpdu;

    if ( !IsActive() )
        {
        iSatApi.SendMessageNoLogging( iStatus, iSmsPckg, iMessageRef );
        SetActive();
        }
    else
        {
        LOG( SIMPLE, " Sms sender already active!" )
        }

    LOG( SIMPLE, "SENDSM: CSatSSendMessageNoLoggingHandler::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSendMessageNoLoggingHandler::RunL
// Handles the command.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSSendMessageNoLoggingHandler::RunL()
    {
    LOG( SIMPLE, "SENDSM: CSatSSendMessageNoLoggingHandler::RunL calling" )

    iObserver.SmsSent( iStatus.Int() );

    LOG( SIMPLE, "SENDSM: CSatSSendMessageNoLoggingHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSSendMessageNoLoggingHandler::DoCancel
// Cancels the pending request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSSendMessageNoLoggingHandler::DoCancel()
    {
    LOG( SIMPLE, "SENDSM: CSatSSendMessageNoLoggingHandler::DoCancel calling" )

    // Cancel the outstanding request.
    iSatApi.SendMessageNoLoggingCancel();

    LOG( SIMPLE, "SENDSM: CSatSSendMessageNoLoggingHandler::DoCancel exiting" )
    }

//  End of File
