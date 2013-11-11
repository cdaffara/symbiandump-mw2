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
* Description:  Handles SetUpEventList command
*
*/


#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatUiSession.h"
#include    "MSatEventMonitorContainer.h"
#include    "CSetUpEventListHandler.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSetUpEventListHandler::CSetUpEventListHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSetUpEventListHandler::CSetUpEventListHandler() :
    CSatCommandHandler(),
    iSetUpEventListData(),
    iSetUpEventListPckg( iSetUpEventListData ),
    iSetUpEventListRsp(),
    iSetUpEventListRspPckg( iSetUpEventListRsp )
    {
    LOG( SIMPLE, "SETUPEVENTLIST: \
        CSetUpEventListHandler::CSetUpEventListHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpEventListHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSetUpEventListHandler::ConstructL()
    {
    LOG( SIMPLE, "SETUPEVENTLIST: CSetUpEventListHandler::ConstructL calling" )

    // Register to listen ESimReset execution.
    iUtils->RegisterL( this, MSatUtils::ESimResetCalled );

    LOG( SIMPLE, "SETUPEVENTLIST: CSetUpEventListHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpEventListHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSetUpEventListHandler* CSetUpEventListHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "SETUPEVENTLIST: CSetUpEventListHandler::NewL calling" )

    CSetUpEventListHandler* self = new( ELeave ) CSetUpEventListHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SETUPEVENTLIST: CSetUpEventListHandler::NewL exiting" )
    return self;
    }
    
// -----------------------------------------------------------------------------
// CSetUpEventListHandler::~CSetUpEventListHandler
// Destructor.
// -----------------------------------------------------------------------------
//
CSetUpEventListHandler::~CSetUpEventListHandler()
    {
    LOG( SIMPLE, "SETUPEVENTLIST: \
        CSetUpEventListHandler::~CSetUpEventListHandler calling" )

    Cancel();

    LOG( SIMPLE, "SETUPEVENTLIST: \
        CSetUpEventListHandler::~CSetUpEventListHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpEventListHandler::Event
// -----------------------------------------------------------------------------
//
void CSetUpEventListHandler::Event( TInt aEvent )
    {
    LOG2( SIMPLE, 
    "SETUPEVENTLIST: CSetUpEventListHandler::Event calling,aEvent: %d",
    aEvent )

    // Check the response
    switch ( aEvent )
        {
        case MSatUtils::ESimResetCalled:
            {
            LOG( NORMAL,
                "SETUPEVENTLIST: CSetUpEventListHandler ESimResetCalled" )

            // Cancel all event monitors.
            iUtils->EventMonitors()->CancelAllMonitors();
            break;
            }

        default:
            {
            CSatCommandHandler::Event( aEvent );
            break;
            }
        }

    LOG( SIMPLE, "SETUPEVENTLIST: CSetUpEventListHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpEventListHandler::DoCancel
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CSetUpEventListHandler::DoCancel()
    {
    LOG( SIMPLE, "SETUPEVENTLIST: CSetUpEventListHandler::DoCancel calling" )

    iUtils->USatAPI().NotifySetUpEventListCancel();

    LOG( SIMPLE, "SETUPEVENTLIST: CSetUpEventListHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpEventListHandler::IssueUSATRequest
// -----------------------------------------------------------------------------
//
void CSetUpEventListHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE,
        "SETUPEVENTLIST: CSetUpEventListHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iSetUpEventListData) RSat::TSetUpEventListV1();

    iUtils->USatAPI().NotifySetUpEventList( aStatus, iSetUpEventListPckg );

    LOG( SIMPLE,
        "SETUPEVENTLIST: CSetUpEventListHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpEventListHandler::CommandAllowed
// -----------------------------------------------------------------------------
//
TBool CSetUpEventListHandler::CommandAllowed()
    {
    LOG( SIMPLE, "SETUPEVENTLIST: \
        CSetUpEventListHandler::CommandAllowed calling - exiting" )
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CSetUpEventListHandler::NeedUiSession
// -----------------------------------------------------------------------------
//
TBool CSetUpEventListHandler::NeedUiSession()
    {
    LOG( SIMPLE, "SETUPEVENTLIST: \
        CSetUpEventListHandler::NeedUiSession calling - exiting" )
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSetUpEventListHandler::HandleCommand
// -----------------------------------------------------------------------------
//
void CSetUpEventListHandler::HandleCommand()
    {
    LOG( SIMPLE,
        "SETUPEVENTLIST: CSetUpEventListHandler::HandleCommand calling" )

    iUtils->NotifyEvent( MSatUtils::ESetUpEventListExecuting );

    // Response
    RSat::TSetUpEventListRspV1 setUpEventListRsp;
    RSat::TSetUpEventListRspV1Pckg setUpEventListRspPckg( setUpEventListRsp );

    // Update / cancel event monitors
    TInt errorCode( KErrNone );

    // Check the event list request type
    if ( RSat::EUpdateEventList == iSetUpEventListData.iType )
        {
        LOG( NORMAL,
        "SETUPEVENTLIST: CSetUpEventListHandler::HandleCommand UpdEvList" )
        // Update event monitor list
        errorCode = iUtils->EventMonitors()->
            SetUpEventMonitors( iSetUpEventListData.iEvents );

        if ( KErrNone != errorCode )
            {
            LOG2( NORMAL,
            "SETUPEVENTLIST: CSetUpEventListHandler::HandleCommand errCode %d",
            errorCode )
            // If any error, cancel all monitors
            iUtils->EventMonitors()->CancelAllMonitors();
            }
        }
    else if ( RSat::ERemoveExistingEventList == iSetUpEventListData.iType )
        {
        LOG( NORMAL,
        "SETUPEVENTLIST: CSetUpEventListHandler::HandleCommand RemExistEvList" )
        // Cancel all monitors
        errorCode = iUtils->EventMonitors()->CancelAllMonitors();
        }
    else
        {
        errorCode = KErrNotSupported;
        }

    // Resolve general result
    if ( KErrNone == errorCode )
        {
        LOG( NORMAL,
        "SETUPEVENTLIST: CSetUpEventListHandler::HandleCommand KSuccess" )
        setUpEventListRsp.iGeneralResult = RSat::KSuccess;
        }
    else
        {
        setUpEventListRsp.iGeneralResult = RSat::KCmdBeyondMeCapabilities;
        }

    // Build the rest of the response
    setUpEventListRsp.iInfoType = RSat::KNoAdditionalInfo;
    setUpEventListRsp.iAdditionalInfo.Zero();
    setUpEventListRsp.SetPCmdNumber( iSetUpEventListData.PCmdNumber() );

    // Send terminal response
    TerminalRsp( RSat::ESetUpEventList, setUpEventListRspPckg );

    LOG( SIMPLE,
        "SETUPEVENTLIST: CSetUpEventListHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpEventListHandler::UiLaunchFailed
// -----------------------------------------------------------------------------
//
void CSetUpEventListHandler::UiLaunchFailed()
    {
    LOG( SIMPLE,
        "SETUPEVENTLIST: CSetUpEventListHandler::UiLaunchFailed calling" )
    // This command does not launch ui client.
    LOG( SIMPLE,
        "SETUPEVENTLIST: CSetUpEventListHandler::UiLaunchFailed exiting" )
    }
