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
* Description:  Handles Refresh command
*
*/


#include    <msatrefreshobserver.h>
#include    "MSatSUiClientHandler.h"
#include    "CRefreshHandler.h"
#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatSystemState.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatRefreshSubSession.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CRefreshHandler::CRefreshHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CRefreshHandler::CRefreshHandler() :
    CSatCommandHandler(),
    iRefreshData(),
    iRefreshPckg( iRefreshData ),
    iRefreshRsp(),
    iRefreshRspPckg( iRefreshRsp ),
    iRefreshNotifyClientCount( 0 ),
    iAdditionalEFRead( EFalse ),
    iSimResetExecuting( EFalse )
    {
    LOG( SIMPLE,
        "REFRESH: CRefreshHandler::CRefreshHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRefreshHandler::ConstructL()
    {
    LOG( SIMPLE, "REFRESH: CRefreshHandler::ConstructL calling" )

    // Register events to be listen.
    iUtils->RegisterL( this, MSatUtils::ERefreshEFRead );
    iUtils->RegisterL( this, MSatUtils::ERefreshAdditionalEFRead );
    iUtils->RegisterL( this, MSatUtils::ESimResetCalled );

    LOG( SIMPLE, "REFRESH: CRefreshHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRefreshHandler* CRefreshHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "REFRESH: CRefreshHandler::NewL calling" )

    CRefreshHandler* self = new( ELeave ) CRefreshHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "REFRESH: CRefreshHandler::NewL exiting" )
    return self;
    }


// Destructor
CRefreshHandler::~CRefreshHandler()
    {
    LOG( SIMPLE, "REFRESH: CRefreshHandler::~CRefreshHandler calling" )

    Cancel();

    LOG( SIMPLE, "REFRESH: CRefreshHandler::~CRefreshHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshHandler::Event
// -----------------------------------------------------------------------------
//
void CRefreshHandler::Event( TInt aEvent )
    {
    LOG( SIMPLE, "REFRESH: CRefreshHandler::Event calling" )

    switch ( aEvent )
        {
        case MSatUtils::ERefreshAdditionalEFRead:
            {
            LOG( NORMAL, "REFRESH:   Event: Additional EF read" )

            // At least one client indicates additional EF read.
            iAdditionalEFRead = ETrue;
            }
            //lint -fallthrough intended here

        case MSatUtils::ERefreshEFRead:
            {
            LOG( NORMAL, "REFRESH:   Event: Client Completed" )

            if ( 0 < iRefreshNotifyClientCount )
                {
                iRefreshNotifyClientCount--;
                // Check whether all clients have replied.
                if ( 0 == iRefreshNotifyClientCount )
                    {
                    LOG( NORMAL, "REFRESH:   All clients are complete" )
                    // Construct and send terminal response..
                    CompleteRefresh();
                    }
                }
            break;
            }

        case MSatUtils::ESimResetCalled:
            {
            LOG( NORMAL, "REFRESH:   Event: Sim Reset called" )
            iSimResetExecuting = ETrue;
            break;
            }

        default:
            {
            LOG2( NORMAL, "REFRESH:   Event: %i", aEvent )
            CSatCommandHandler::Event( aEvent );
            }
        }

    LOG( SIMPLE, "REFRESH: CRefreshHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshHandler::DoCancel
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CRefreshHandler::DoCancel()
    {
    LOG( SIMPLE, "REFRESH: CRefreshHandler::DoCancel calling" )

    iUtils->USatAPI().NotifyRefreshCancel();

    LOG( SIMPLE, "REFRESH: CRefreshHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshHandler::IssueUSATRequest
// -----------------------------------------------------------------------------
//
void CRefreshHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "REFRESH: CRefreshHandler::IssueUSATRequest calling" )

    // Refresh has been cancelled, lets notify clients waiting for it.
    if ( KErrGeneral == aStatus.Int() )
        {
        LOG( SIMPLE, "REFRESH:   Refresh cancelled" )
        NotifyRefreshClientsCancel();

        if ( iSimResetExecuting )
            {
            LOG( SIMPLE, "CRefreshHandler::IssueUSATRequest \
            iSimResetExecuting" )
            iUtils->NotifyEvent( MSatUtils::EAddSatUiCalled );
            iUtils->NotifyEvent( MSatUtils::ESimResetCancelled );

            iSimResetExecuting = EFalse;
            }
        }

    // Clear the IPC package.
    new (&iRefreshData) RSat::TRefreshV2();

    iUtils->USatAPI().NotifyRefresh( aStatus, iRefreshPckg );

    LOG( SIMPLE, "REFRESH: CRefreshHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshHandler::CommandAllowed
// -----------------------------------------------------------------------------
//
TBool CRefreshHandler::CommandAllowed()
    {
    LOG( SIMPLE, "REFRESH: CRefreshHandler::CommandAllowed calling - exiting" )

    // Now we must always allow Refresh. We had our last chnage to deny it in
    // RefreshRequired command.
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CRefreshHandler::NeedUiSession
// -----------------------------------------------------------------------------
//
TBool CRefreshHandler::NeedUiSession()
    {
    LOG( SIMPLE, "REFRESH: CRefreshHandler::NeedUiSession calling - exiting" )
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CRefreshHandler::HandleCommand
// -----------------------------------------------------------------------------
//
void CRefreshHandler::HandleCommand()
    {
    LOG( SIMPLE, "REFRESH: CRefreshHandler::HandleCommand calling" )

    // Refresh has been done.
    iSimResetExecuting = EFalse;

    #ifdef ENABLE_SAT_LOGGING
    TInt fileCount( iRefreshData.iFileList.Length() );
    LOG2( NORMAL, "REFRESH:   Refresh file count: %d", fileCount )
    for ( TInt fileIndex = 0; fileIndex < fileCount; fileIndex++ )
        {
        LOG2( NORMAL, "REFRESH:     Refresh file : %x",
            iRefreshData.iFileList[ fileIndex ] )
        }
    #endif

    // Notify all registered clients about refresh.
    // Whether any clients exists, we must wait them all to complete EF read,
    // otherwise we can complete now.
    // Count must be local variable, due some client may reply before
    // all other has been notified.
    TInt clientCount( iUtils->RefreshSubSessions().Count() );
    iRefreshNotifyClientCount = clientCount; // Actually notified clients.
    iAdditionalEFRead = EFalse;
    if ( 0 != clientCount )
        {
        LOG2( NORMAL, "REFRESH:   Refresh client count: %d",
            iRefreshNotifyClientCount )

        // Notify refresh clients about refresh completion.
        for ( TInt i = 0; i < clientCount ; i++ )
            {
            iUtils->RefreshSubSessions()[i]->NotifyClient( iRefreshData );
            }
        }
    else
        {
        // Construct and send terminal response..
        CompleteRefresh();
        }

    LOG( SIMPLE, "REFRESH: CRefreshHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshHandler::UiLaunchFailed
// -----------------------------------------------------------------------------
//
void CRefreshHandler::UiLaunchFailed()
    {
    // This command will not launch ui.
    LOG( SIMPLE, "REFRESH: CRefreshHandler::UiLaunchFailed calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshHandler::CompleteRefresh
// Waits for all clients complete.
// -----------------------------------------------------------------------------
//
void CRefreshHandler::CompleteRefresh()
    {
    LOG( SIMPLE, "REFRESH: CRefreshHandler::CompleteRefresh calling" )

    // Remove the idle mode text and icon for initialization type refreshes.
    // Note that idle mode text was removed for ESimReset already in
    // RefreshRequired command.
    if ( ( RSat::EFileChangeNotification != iRefreshData.iType ) &&
         ( RSat::ESimReset != iRefreshData.iType ) )
        {
        LOG( SIMPLE, "REFRESH: CRefreshHandler::CompleteRefresh ESimInitDone" )
        iUtils->NotifyEvent( MSatUtils::ESimInitDone );
        }

    // Construct a proper refresh response data package.
    if ( iAdditionalEFRead )
        {
        LOG( SIMPLE, "REFRESH: CRefreshHandler::CompleteRefresh \
             iAdditionalEFRead" )
        iRefreshRsp.iGeneralResult = RSat::KRefreshAdditionEFRead;
        iAdditionalEFRead = EFalse;
        }
    else
        {
        iRefreshRsp.iGeneralResult = RSat::KSuccess;
        }

    iRefreshRsp.iInfoType = RSat::KNoAdditionalInfo;
    iRefreshRsp.iAdditionalInfo.Zero();
    iRefreshRsp.SetPCmdNumber( iRefreshData.PCmdNumber() );

    // Send the response
    TerminalRsp( RSat::ERefresh, iRefreshRspPckg );

    LOG( SIMPLE, "REFRESH: CRefreshHandler::CompleteRefresh exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshHandler::NotifyRefreshClientsCancel
// Notifies waiting clients about refrsh cancel.
// -----------------------------------------------------------------------------
//
void CRefreshHandler::NotifyRefreshClientsCancel()
    {
    LOG( SIMPLE,
        "REFRESH: CRefreshHandler::NotifyRefreshClientsCancel calling" )
    LOG2( SIMPLE,
        "REFRESH: CRefreshHandler::NotifyRefreshClientsCancel \
        iRefreshNotifyClientCount: %d", iRefreshNotifyClientCount )
    for ( TInt i = 0; i < iRefreshNotifyClientCount ; i++ )
        {
        iUtils->RefreshSubSessions()[i]->NotifyClientCancel();
        }
    iRefreshNotifyClientCount = 0;

    LOG( SIMPLE,
        "REFRESH: CRefreshHandler::NotifyRefreshClientsCancel exit" )
    }
