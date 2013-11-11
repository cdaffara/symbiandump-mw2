/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles RefreshRequired command
*
*/



// INCLUDE FILES
#include    <msatrefreshobserver.h>
#include    "MSatSSimSubscriberId.h"
#include    "MSatSystemState.h"
#include    "MSatEventMonitorContainer.h"
#include    "MSatSUiClientHandler.h"
#include    "CRefreshRequiredHandler.h"
#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatBIPUtils.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatRefreshSubSession.h"
#include    "SatLog.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::CRefreshRequiredHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized, harmless.
CRefreshRequiredHandler::CRefreshRequiredHandler() :
    CSatCommandHandler(),
    iRefreshRequiredData(),
    iRefreshRequiredPckg( iRefreshRequiredData ),
    iRefreshRequiredRsp(),
    iRefreshRequiredRspPckg( iRefreshRequiredRsp ),
    iQueryData(),
    iQueryPckg( iQueryData ),
    iQueryRsp(),
    iQueryRspPckg( iQueryRsp ),
    iRefreshQueryClientCount( 0 ),
    iAllowRefresh( ETrue )
    {
    LOG( SIMPLE, "REFRESHREQ: \
        CRefreshRequiredHandler::CRefreshRequiredHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRefreshRequiredHandler::ConstructL()
    {
    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::ConstructL calling" )

    // Register events to be listen.
    iUtils->RegisterL( this, MSatUtils::ERefreshAllowed );
    iUtils->RegisterL( this, MSatUtils::ERefreshNotAllowed );

    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRefreshRequiredHandler* CRefreshRequiredHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::NewL calling" )

    CRefreshRequiredHandler* self = new( ELeave ) CRefreshRequiredHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::NewL exiting" )
    return self;
    }


// Destructor
CRefreshRequiredHandler::~CRefreshRequiredHandler()
    {
    LOG( SIMPLE, "REFRESHREQ: \
        CRefreshRequiredHandler::~CRefreshRequiredHandler calling" )

    Cancel();

    LOG( SIMPLE, "REFRESHREQ: \
        CRefreshRequiredHandler::~CRefreshRequiredHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::Event
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRefreshRequiredHandler::Event( TInt aEvent )
    {
    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::Event calling" )

    switch ( aEvent )
        {
        case MSatUtils::ERefreshAllowed:
            {
            LOG( NORMAL, "REFRESHREQ:   Event: ERefreshAllowed" )
            AllowRefresh( ETrue );
            break;
            }

        case MSatUtils::ERefreshNotAllowed:
            {
            LOG( NORMAL, "REFRESHREQ:   Event: ERefreshNotAllowed" )
            AllowRefresh( EFalse );
            break;
            }

        default:
            {
            LOG2( NORMAL, "REFRESHREQ:   Event: %i", aEvent )
            CSatCommandHandler::Event( aEvent );
            break;
            }
        }

    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::ClientResponse
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRefreshRequiredHandler::ClientResponse()
    {
    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::ClientResponse calling" )
    TBool commandAllowed( ETrue );

    // Check ui client response for accepting refresh.
    if ( iQueryRsp.iAccepted )
        {
        LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::ClientResponse \
             accept refresh" )
        // Check phone state again, whether refresh is still allowed,
        if ( BusyNotAllowed() )
            {
            LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::ClientResponse \
            BusyNotAllowed" )
            // Refresh not allowed due phone is busy.
            iRefreshRequiredRsp.iAdditionalInfo.SetLength( 1 );
            iRefreshRequiredRsp.iAdditionalInfo[0] = RSat::KMeBusyOnCall;
            commandAllowed = EFalse;
            }
        }
    // Client did not accept refresh.
    else
        {
        // Refresh is prevented by the user
        iRefreshRequiredRsp.iAdditionalInfo.SetLength( 1 );
        iRefreshRequiredRsp.iAdditionalInfo[0] = RSat::KScreenBusy;
        commandAllowed = EFalse;

        if ( iQueryRsp.iSessionTerminatedByUser )
            {
            LOG( SIMPLE, "REFRESHREQ:   close the ui session" )
            // Next SimSession end will close the ui session
            iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
            }
        LOG( SIMPLE, "REFRESHREQ:   User did not accept refresh" )
        }

    if ( commandAllowed )
        {
        LOG( SIMPLE, "REFRESHREQ:   commandAllowed" )
        // Refresh allowed.
        iRefreshRequiredRsp.iInfoType = RSat::KNoAdditionalInfo;
        iRefreshRequiredRsp.iGeneralResult = RSat::KSuccess;

        if ( RSat::ESimReset == iRefreshRequiredData.iType )
            {
            LOG( SIMPLE, "REFRESHREQ:   ESimReset" )
            iUtils->NotifyEvent( MSatUtils::ESimResetCalled );
            }
        }
    else
        {
        // Notify clients already allowed Refresh about cancel.
        NotifyRefreshClientsCancel();

        // Refresh not allowed.
        iRefreshRequiredRsp.iInfoType = RSat::KMeProblem;
        iRefreshRequiredRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        }

    // Send the response
    iRefreshRequiredRsp.SetPCmdNumber( iRefreshRequiredData.PCmdNumber() );
    RefreshAllowed( iRefreshRequiredRspPckg );

    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::DoCancel
// Cancels the sat request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRefreshRequiredHandler::DoCancel()
    {
    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::DoCancel calling" )

    iUtils->USatAPI().NotifyRefreshRequiredCancel();

    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::IssueUSATRequest
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRefreshRequiredHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE,
        "REFRESHREQ: CRefreshRequiredHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iRefreshRequiredData) RSat::TRefreshV2();
    iQueryRsp.iAccepted = EFalse; // default

    iUtils->USatAPI().NotifyRefreshRequired( aStatus, iRefreshRequiredPckg );

    LOG( SIMPLE,
        "REFRESHREQ: CRefreshRequiredHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::CommandAllowed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CRefreshRequiredHandler::CommandAllowed()
    {
    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::CommandAllowed calling" )
    TBool commandAllowed( ETrue );

    #ifdef ENABLE_SAT_LOGGING

    // Log Refresh type.
    switch ( iRefreshRequiredData.iType )
        {
        case RSat::ERefreshTypeNotSet:
            {
            LOG( NORMAL,
                "REFRESHREQ:   Refresh type: ERefreshTypeNotSet" )
            break;
            }

        case RSat::ESimInitFullFileChangeNotification:
            {
            LOG( NORMAL,
                "REFRESHREQ:   Refresh type: ESimInitFullFileChangeNotificat." )
            break;
            }

        case RSat::EFileChangeNotification:
            {
            LOG( NORMAL,
                "REFRESHREQ:   Refresh type: EFileChangeNotification" )
            break;
            }

        case RSat::ESimInitFileChangeNotification:
            {
            LOG( NORMAL,
                "REFRESHREQ:   Refresh type: ESimInitFileChangeNotification" )
            break;
            }

        case RSat::ESimInit:
            {
            LOG( NORMAL,
                "REFRESHREQ:   Refresh type: ESimInit" )
            break;
            }

        case RSat::ESimReset:
            {
            LOG( NORMAL,
                "REFRESHREQ:   Refresh type: ESimReset" )
            break;
            }

        case RSat::EUsimApplicationReset:
            {
            LOG( NORMAL,
                "REFRESHREQ:   Refresh type: EUsimApplicationReset" )
            break;
            }

        case RSat::E3GSessionReset:
            {
            LOG( NORMAL,
                "REFRESHREQ:   Refresh type: E3GSessionReset" )
            break;
            }

        default:
            {
            LOG2( NORMAL,
                "REFRESHREQ:   Refresh type: unknown %d",
                iRefreshRequiredData.iType )
            break;
            }
        }

    // Log the file list.
    TInt fileCount ( iRefreshRequiredData.iFileList.Length() );
    LOG2( NORMAL, "REFRESHREQ:   Refresh file count: %d", fileCount )
    for ( TInt fileIndex = 0; fileIndex < fileCount; fileIndex++ )
        {
        LOG2( NORMAL, "REFRESHREQ:     Refresh file: %x",
            iRefreshRequiredData.iFileList[ fileIndex ] )
        }

    #endif

    // Check EFimsi file refresh type.
    if ( !EFIMSIRefreshAllowed() )
        {
        LOG( NORMAL, "REFRESHREQ:   EFIMSIRefreshAllowed false" )
        // Refresh not allowed
        iRefreshRequiredRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        iRefreshRequiredRsp.iInfoType = RSat::KNoAdditionalInfo;
        commandAllowed = EFalse;
        }
    // Check phone state.
    else if ( BusyNotAllowed() )
        {
        LOG( NORMAL, "REFRESHREQ:   BusyNotAllowed true" )
        // Refresh not allowed
        iRefreshRequiredRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        iRefreshRequiredRsp.iInfoType = RSat::KMeProblem;
        iRefreshRequiredRsp.iAdditionalInfo.SetLength( 1 );
        iRefreshRequiredRsp.iAdditionalInfo[0] = RSat::KMeBusyOnCall;
        commandAllowed = EFalse;
        }

    // When refresh is not allowed, send terminal response to sim.
    if ( !commandAllowed )
        {
        LOG( NORMAL, "REFRESHREQ:   Refresh not allowed" )
        iRefreshRequiredRsp.SetPCmdNumber( iRefreshRequiredData.PCmdNumber() );

        // Send the response
        RefreshAllowed( iRefreshRequiredRspPckg );
        }

    LOG2( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::CommandAllowed \
          exiting, commandAllowed: %d", commandAllowed )
    return commandAllowed;
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::NeedUiSession
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CRefreshRequiredHandler::NeedUiSession()
    {
    LOG( NORMAL, "REFRESHREQ: CRefreshRequiredHandler::NeedUiSession calling" )
    TBool needUiSession( EFalse );

    if ( RSat::ESimReset == iRefreshRequiredData.iType
            && iUtils->SystemState().IsConfirmSatOperationsOn() )
        {
        LOG( NORMAL, "REFRESHREQ:   Need ui session" )
        needUiSession = ETrue;
        }

    // Notify Cover UI if it's supported
    if ( needUiSession && iUtils->CoverUiSupported() )
        {
        LOG( NORMAL, "REFRESHREQ:   CoverUiSupported" )
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::ERefresh;

        //Doesn't have alpha id
        medEventData.iAlphaId.iStatus = RSat::EAlphaIdNotSet;
        medEventData.iDuration.iNumOfUnits = KSatSustainedDuration;
        medEventData.iDuration.iTimeUnit = RSat::ESeconds;
        medEventData.iIconID.iQualifier = RSat::ENoIconId;

        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        }

    LOG2( NORMAL, "REFRESHREQ: CRefreshRequiredHandler::NeedUiSession exiting\
          needUiSession: %d", needUiSession )
    return needUiSession;
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::HandleCommand
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRefreshRequiredHandler::HandleCommand()
    {
    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::HandleCommand calling" )

    // Ask permission from clients.
    if ( QueryRefreshClients() )
        {
        LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::HandleCommand \
             refresh" )
        DoRefresh();
        }

    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::UiLaunchFailed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRefreshRequiredHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::UiLaunchFailed calling" )

    iRefreshRequiredRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
    iRefreshRequiredRsp.iInfoType = RSat::KNoAdditionalInfo;
    iRefreshRequiredRsp.SetPCmdNumber( iRefreshRequiredData.PCmdNumber() );

    // Send the response
    RefreshAllowed( iRefreshRequiredRspPckg );

    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::UiLaunchFailed exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::EFIMSIRefreshAllowed
// If an EFimsi file change occurs under certain conditions a refresh is not
// allowed.
// -----------------------------------------------------------------------------
//
TBool CRefreshRequiredHandler::EFIMSIRefreshAllowed() const
    {
    LOG( SIMPLE,
        "REFRESHREQ: CRefreshRequiredHandler::EFIMSIRefreshAllowed calling" )
    TBool refreshAllowed( ETrue );

    // If changed file is EFimsi and the refresh type is any of the ones
    // specifed below, no refresh will take place as the behaviour of the UE
    // is unpredicatable.
    if ( RSat::EFileChangeNotification == iRefreshRequiredData.iType )
        {
        LOG( SIMPLE,
        "REFRESHREQ: CRefreshRequiredHandler::EFIMSIRefreshAllowed \
        EFileChangeNotification" )
        const TChar efImsi( RSat::KImsiEf );
        const TInt imsi( iRefreshRequiredData.iFileList.Locate( efImsi ) );

        // Does the EFimsi file want to be updated.
        if ( KErrNotFound != imsi )
            {
            LOG( SIMPLE, "REFRESHREQ:   EFImsi file refresh not allowed" )
            refreshAllowed = EFalse;
            }
        }

    LOG2( SIMPLE,
        "REFRESHREQ: CRefreshRequiredHandler::EFIMSIRefreshAllowed exiting,\
        refreshAllowed: %d", refreshAllowed )
    return refreshAllowed;
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::BusyNotAllowed
// Check whether Refresh is allowed.
// -----------------------------------------------------------------------------
//
TBool CRefreshRequiredHandler::BusyNotAllowed()
    {
    LOG( SIMPLE, 
        "REFRESHREQ: CRefreshRequiredHandler::BusyNotAllowed calling" )
    TBool notAllowed( EFalse );

    // Busy checked only for reset due only it may prevent call.
    if ( RSat::EFileChangeNotification != iRefreshRequiredData.iType )
        {
        LOG( SIMPLE,
        "REFRESHREQ: CRefreshRequiredHandler::EFIMSIRefreshAllowed \
        not EFileChangeNotification" )
        // Refresh is not allowed when call is active or incoming.
        if ( iUtils->SystemState().IsCallActive() ||
             iUtils->SystemState().IsCallIncoming() )
            {
            notAllowed = ETrue;
            LOG( NORMAL, "REFRESHREQ:   Refresh not allowed: phone is busy" )
            }
        }

    LOG2( SIMPLE, 
        "REFRESHREQ: CRefreshRequiredHandler::BusyNotAllowed  exiting,\
        notAllowed: %d", notAllowed )
    return notAllowed;
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::QueryRefreshClients
// Notifies the observers that refresh query should start.
// -----------------------------------------------------------------------------
//
TBool CRefreshRequiredHandler::QueryRefreshClients()
    {
    LOG( SIMPLE,
        "REFRESHREQ: CRefreshRequiredHandler::QueryRefreshClients calling" )
    TBool allowNow( ETrue );
    TInt clientCount( iUtils->RefreshSubSessions().Count() );

    // By default refresh is allowed after all clients have queried.
    iAllowRefresh = ETrue;

    // When there are clients, we must ask them first and refresh will be
    // started later, after all clients have allowed it.
    // When there are clients, we can just start refresh.
    if ( 0 != clientCount )
        {
        LOG2( NORMAL, "REFRESHREQ:   Client count: %d", clientCount )
        allowNow = EFalse;
        iRefreshQueryClientCount = clientCount;
        for ( TInt i = 0; i < clientCount; i++ )
            {
            iUtils->RefreshSubSessions()[i]->QueryClient(
                iRefreshRequiredData );
            }
        }

    LOG2( SIMPLE, 
        "REFRESHREQ: CRefreshRequiredHandler::QueryRefreshClients exiting,\
        allowNow: %d", allowNow )
    return allowNow;
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::AllowRefresh
// Waits for all the responses of refresh query and
// starts or declines the refresh.
// -----------------------------------------------------------------------------
//
void CRefreshRequiredHandler::AllowRefresh( TBool aAllow )
    {
    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::AllowRefresh calling" )
    LOG2( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::AllowRefresh \
          iAllowRefresh: %d", iAllowRefresh )
    LOG2( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::AllowRefresh \
          iRefreshQueryClientCount: %d", iRefreshQueryClientCount )
    // if iAllowRefresh already false no need to set new value.
    if ( iAllowRefresh )
        {
        iAllowRefresh = aAllow;
        }

    // Check whether all clients have replied.
    iRefreshQueryClientCount--;
    if ( iRefreshQueryClientCount <= 0 )
        {
        if ( iAllowRefresh )
            {
            DoRefresh();
            }
        else
            {
            LOG( NORMAL, "REFRESHREQ:   At least one client rejected refresh" )
            // Notify clients already allowed Refresh about cancel.
            NotifyRefreshClientsCancel();

            // Construct a proper refresh response data package
            iRefreshRequiredRsp.iInfoType = RSat::KNoAdditionalInfo;
            iRefreshRequiredRsp.SetPCmdNumber(
                iRefreshRequiredData.PCmdNumber() );
            iRefreshRequiredRsp.iGeneralResult = RSat::KPCmdNotAcceptedByUser;

            // Send the response
            RefreshAllowed( iRefreshRequiredRspPckg );
            }
        }

    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::AllowRefresh exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::DoRefresh
// Starts the refresh
// -----------------------------------------------------------------------------
//
void CRefreshRequiredHandler::DoRefresh()
    {
    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::DoRefresh calling" )

    if ( RSat::ESimReset == iRefreshRequiredData.iType )
        {
        LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::DoRefresh \
        ESimReset" )
        // Register service request handler for RefreshRequired command,
        // If there is already service request for query, registering
        // updates command handler, so client responses comes to this
        // command handler.
        TRAPD( err, iUtils->RegisterServiceRequestL(
            ESatSProactiveQuery,
            ESatSProactiveQueryResponse,
            this ) );

        if ( KErrNone != err )
            {
            iRefreshRequiredRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iRefreshRequiredRsp.iInfoType = RSat::KMeProblem;
            iRefreshRequiredRsp.iAdditionalInfo.SetLength( 1 );
            iRefreshRequiredRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
            iRefreshRequiredRsp.SetPCmdNumber(
                iRefreshRequiredData.PCmdNumber() );

            // Notify clients already allowed Refresh about cancel.
            NotifyRefreshClientsCancel();

            // Send the response
            RefreshAllowed( iRefreshRequiredRspPckg );
            LOG2( SIMPLE, 
                "REFRESHREQ: CRefreshRequiredHandler::DoRefresh error", err )
            }
        else
            {
            iUtils->NotifyEvent( MSatUtils::ERefreshExecuting );

            // Ask the user permission to refresh. Reply will come
            // to ClientResponse method.
            if ( iUtils->SystemState().IsConfirmSatOperationsOn() )
                {
                LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::DoRefresh \
                     IsConfirmSatOperationsOn true" )
                iQueryData.iCommand = ESatSRefreshQuery;
                iUtils->SatUiHandler().UiSession()->SendCommand(
                    &iQueryPckg,
                    &iQueryRspPckg,
                    ESatSProactiveQuery );
                }
            else
                {
                LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::DoRefresh \
                     IsConfirmSatOperationsOn false" )
                iQueryRsp.iAccepted = ETrue;
                ClientResponse();
                }
            }
        }
    else
        {
        LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::DoRefresh refresh" )
        // Construct a proper refresh response data package.
        iRefreshRequiredRsp.iInfoType = RSat::KNoAdditionalInfo;
        iRefreshRequiredRsp.SetPCmdNumber( iRefreshRequiredData.PCmdNumber() );
        iRefreshRequiredRsp.iGeneralResult = RSat::KSuccess;

        // Send the response
        RefreshAllowed( iRefreshRequiredRspPckg );
        }

    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::DoRefresh exiting" )
    }

// -----------------------------------------------------------------------------
// CRefreshHandler::NotifyRefreshClientsCancel
// Notifies waiting clients about refrsh cancel.
// -----------------------------------------------------------------------------
//
void CRefreshRequiredHandler::NotifyRefreshClientsCancel()
    {
    LOG( SIMPLE, 
        "REFRESHREQ: CRefreshHandler::NotifyRefreshClientsCancel calling" )
    LOG2( SIMPLE,
        "REFRESHREQ: CRefreshHandler::NotifyRefreshClientsCancel \
        RefreshSubSessions().Count: %d",iUtils->RefreshSubSessions().Count() )
    for ( TInt i = 0; i < iUtils->RefreshSubSessions().Count() ; i++ )
        {
        iUtils->RefreshSubSessions()[i]->NotifyClientCancel();
        }

    LOG( SIMPLE, "REFRESHREQ: CRefreshHandler::NotifyRefreshClientsCancel exit" )
    }

// -----------------------------------------------------------------------------
// CRefreshRequiredHandler::RefreshAllowed
// Sends the RefreshRequired specific response to the SIM and renews the sat
// request. Notem, this replaces TerminalRsp() here.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CRefreshRequiredHandler::RefreshAllowed( const TDesC8& aRsp )
    {
    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::RefreshAllowed calling" )

    iUtils->USatAPI().RefreshAllowed( aRsp );
    iUtils->NotifyEvent( MSatUtils::EDelaySimSessionEnd );
    Start();

    LOG( SIMPLE, "REFRESHREQ: CRefreshRequiredHandler::RefreshAllowed exiting" )
    }

//  End of File
