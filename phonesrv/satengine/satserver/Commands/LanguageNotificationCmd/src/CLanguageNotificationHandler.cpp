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
* Description:  Handles CLanguageNotification command
*
*/


// INCLUDE FILES
#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatUiSession.h"
#include    "MSatSystemState.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CLanguageNotificationHandler.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLanguageNotificationHandler::CLanguageNotificationHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLanguageNotificationHandler::CLanguageNotificationHandler() :
    CSatCommandHandler(),
    iLanguageNotify(),
    iLanguageNotifyPckg( iLanguageNotify ),
    iLanguageNotifyRsp(),
    iLanguageNotifyRspPckg( iLanguageNotifyRsp )
    {
    LOG( SIMPLE, "LangNotify: CLanguageNotificationHandler:: \
        CLanguageNotificationHandler calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CLanguageNotificationHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLanguageNotificationHandler* CLanguageNotificationHandler::NewL(
MSatUtils* aUtils )
    {
    LOG( SIMPLE, "LangNotify: CLanguageNotificationHandler::NewL calling" )
    CLanguageNotificationHandler* self = new( ELeave ) CLanguageNotificationHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    // ConstructL not needed.
    CleanupStack::Pop( self );
    LOG( SIMPLE, "LangNotify: CLanguageNotificationHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CLanguageNotificationHandler::~CLanguageNotificationHandler
// Destructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CLanguageNotificationHandler::~CLanguageNotificationHandler()
    {
    LOG( SIMPLE,
        "LangNotify: CLanguageNotificationHandler:: \
        ~CLanguageNotificationHandler() calling" )

    Cancel();

    LOG( SIMPLE,
        "LangNotify: CLanguageNotificationHandler:: \
        ~CLanguageNotificationHandler() exiting" )
    }

// -----------------------------------------------------------------------------
// CLanguageNotificationHandler::DoCancel
// Cancels the sat request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLanguageNotificationHandler::DoCancel()
    {
    LOG( SIMPLE,
        "LangNotify: CLanguageNotificationHandler::DoCancel calling" )

    iUtils->USatAPI().NotifyLanguageNotificationCancel();

    LOG( SIMPLE,
        "LangNotify: CLanguageNotificationHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CLanguageNotificationHandler::IssueUSATRequest
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLanguageNotificationHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE,
        "LangNotify: CLanguageNotificationHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iLanguageNotify) RSat::TLanguageNotificationV2();

    iUtils->USatAPI().NotifyLanguageNotification(
        aStatus,
        iLanguageNotifyPckg );

    LOG( SIMPLE,
        "LangNotify: CLanguageNotificationHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// CLanguageNotificationHandler::CommandAllowed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CLanguageNotificationHandler::CommandAllowed()
    {
    LOG( SIMPLE, "LangNotify: \
        CLanguageNotificationHandler::CommandAllowed calling-exiting" )
    // By default, this is true
    // There is never any reason to disallow this
    // command, even if an error occurs.
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CLanguageNotificationHandler::NeedUiSession
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CLanguageNotificationHandler::NeedUiSession()
    {
    LOG( SIMPLE, "LangNotify: CLanguageNotificationHandler::NeedUiSession \
        calling-exiting" )
    // The UI is not needed by this command.
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CLanguageNotificationHandler::HandleCommand
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLanguageNotificationHandler::HandleCommand()
    {
    LOG( SIMPLE,
        "LangNotify: CLanguageNotificationHandler::HandleCommand calling" )

    // Just OK the request and send a response.
    iLanguageNotifyRsp.iInfoType = RSat::KNoAdditionalInfo;
    iLanguageNotifyRsp.iAdditionalInfo.Zero();
    iLanguageNotifyRsp.iGeneralResult = RSat::KSuccess;
    iLanguageNotifyRsp.SetPCmdNumber( iLanguageNotify.PCmdNumber() );

    TerminalRsp( RSat::ELanguageNotification, iLanguageNotifyRspPckg );

    LOG( SIMPLE,
        "LangNotify: CLanguageNotificationHandler::HandleCommand exiting" )
    }

//  End of File
