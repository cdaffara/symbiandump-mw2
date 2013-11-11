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
* Description:  Handles DisplayText command
*
*/


#include    <MSatShellController.h>
#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatSystemState.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CDisplayTextHandler.h"
#include    "CClearScreenHandler.h"
#include    "SatLog.h"

const TInt8 KSecondsInMinute = 60;
const TInt8 KSecond = 10;
const TUint8 KHalfSecond = 5;

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CDisplayTextHandler::CDisplayTextHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CDisplayTextHandler::CDisplayTextHandler() :
    CSatCommandHandler(),
    iDisplayTextData(),
    iDisplayTextPckg( iDisplayTextData ),
    iDisplayTextRsp(),
    iDisplayTextRspPckg( iDisplayTextRsp ),
    iDisplayTextSendData(),
    iDisplayTextSendPckg( iDisplayTextSendData )
    {
    LOG( SIMPLE, "DISPLAYTEXT: \
        CDisplayTextHandler::CDisplayTextHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CDisplayTextHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CDisplayTextHandler::ConstructL()
    {
    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::ConstructL calling" )

    // Register service request handler for DisplayText command
    iUtils->RegisterServiceRequestL(
        ESatSProactiveDisplayText,
        ESatSProactiveDisplayTextResponse,
        this );

    iClearScreenHandler = CClearScreenHandler::NewL( *iUtils );

    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CDisplayTextHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CDisplayTextHandler* CDisplayTextHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::NewL calling" )

    CDisplayTextHandler* self = new( ELeave ) CDisplayTextHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::NewL exiting" )
    return self;
    }

// Destructor
CDisplayTextHandler::~CDisplayTextHandler()
    {
    LOG( SIMPLE,
        "DISPLAYTEXT: CDisplayTextHandler::~CDisplayTextHandler calling" )

    Cancel();
    delete iClearScreenHandler;

    LOG( SIMPLE,
        "DISPLAYTEXT: CDisplayTextHandler::~CDisplayTextHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CDisplayTextHandler::ClientResponse
// -----------------------------------------------------------------------------
//
void CDisplayTextHandler::ClientResponse()
    {
    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::ClientResponse calling" )

    // SatShellController checks if SatUi was brought to foreground from
    // background and set SatUi background again.
    if ( RSat::EHighPriority == iDisplayTextData.iPriority )
        {
        LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::ClientResponse \
             set SatUi background" )
        iUtils->SatUiHandler().ShellController().SetSatUiToBackground();
        }

    TBool terminatedByUser( EFalse );
    if ( RSat::KPSessionTerminatedByUser == iDisplayTextRsp.iGeneralResult )
        {
        LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::ClientResponse \
             session terminated by user" )
        terminatedByUser = ETrue;

        iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
        }

    // Check immediate response. If true, no need to send TerminalResponse
    // This must be checked from our own member variable, because
    // ClearScreenHandler may have this value set to false already if the
    // timer has run out and screen was cleared.
    if ( iImmediateResponse )
        {
        // Check has sustained text cleared in ClearScreen handler
        if ( !iImmediatePending )
            {
            // No need to send terminal response since it is already sent.
            iImmediateResponse = EFalse;

            // Notify sustained text removal, if not yet notified.
            iUtils->NotifyEvent( MSatUtils::ESustainedTextRemoved );
            iClearScreenHandler->UpdateImmediateState( EFalse );

            if ( !iUtils->SatUiHandler().UiLaunchedByUser() ||
                 terminatedByUser )
                {
                LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::\
                     ClientResponse close ui session" )
                // Next SimSession end will close the ui session.
                iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
                }
            else
                {
                LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::\
                     ClientResponse others" )
                // If SATUI client application is running, show the setup menu.
                if ( iUtils->SatUiHandler().UiSession() )
                    {
                    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::\
                         ClientResponse SetUpMenu" )
                    iUtils->NotifyEvent( MSatUtils::ESetUpMenuNeeded );
                    }
                }
            }
        iImmediatePending = EFalse;
        }
    else
        {
        LOG( SIMPLE,
            "DISPLAYTEXT: CDisplayTextHandler::ClientResponse Success" )
        // Generate terminal response
        iDisplayTextRsp.SetPCmdNumber( iDisplayTextData.PCmdNumber() );
        TerminalRsp( RSat::EDisplayText, iDisplayTextRspPckg );
        }

    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// CDisplayTextHandler::DoCancel
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CDisplayTextHandler::DoCancel()
    {
    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::DoCancel calling" )

    iUtils->USatAPI().NotifyDisplayTextCancel();

    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CDisplayTextHandler::IssueUSATRequest
// -----------------------------------------------------------------------------
//
void CDisplayTextHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iDisplayTextData) RSat::TDisplayTextV2();
    iDisplayTextRsp.iGeneralResult = RSat::KPSessionTerminatedByUser; // default
    iDisplayTextSendData.iDuration = 0;
    iDisplayTextSendData.iClearScreen = RSat::EClearScreenTriggerNotSet;

    iUtils->USatAPI().NotifyDisplayText( aStatus, iDisplayTextPckg );

    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// CDisplayTextHandler::CommandAllowed
// -----------------------------------------------------------------------------
//
TBool CDisplayTextHandler::CommandAllowed()
    {
    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::CommandAllowed calling" )

    // By default, this is true.
    TBool commandAllowed( ETrue );

    const TBool callIncoming( iUtils->SystemState().IsCallIncoming() );
    const TBool phoneInIdle( IsPhoneInIdleState() );

    
    TBool screenSaver( EFalse );
    
    // By default, this is true.    
    TBool uiNotReady( ETrue );     
    
    // If screen saver is actived from idle.
    screenSaver = iUtils->SystemState().IsScreenSaverActivedFromIdle();
      
    if ( screenSaver )
        {
        uiNotReady = EFalse;
        }
    else
        {
        uiNotReady = ( !phoneInIdle  &&
                     ( !iUtils->SatUiHandler().UiSession() ) );        
        }

    const RSat::TDisplayPriority priority( iDisplayTextData.iPriority );

    // Icon without text
    if ( ( 0 == iDisplayTextData.iText.Length() ) &&
         ( RSat::ENotSelfExplanatory == iDisplayTextData.iIconId.iQualifier ||
           RSat::ESelfExplanatory == iDisplayTextData.iIconId.iQualifier ) )
        {
        iDisplayTextRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        iDisplayTextRsp.iInfoType = RSat::KNoAdditionalInfo;
        iDisplayTextRsp.iAdditionalInfo.Zero();
        commandAllowed = EFalse;
        }
    // Check is call incoming or if Normal priority check is call ongoing or is
    // phone in idle state and is there UiSession. AND Ui is not closing
    else if ( callIncoming ||
         ( ( priority == RSat::ENormalPriority ) &&
           ( uiNotReady ) ) )
        {
        // Check is UI closing
        if ( !iUtils->SatUiHandler().IsUiClosing() )
            {
            // Generate proper terminal response and send it.
            // By default, this command does not have additional information,
            // except in this case when screen is busy.
            iDisplayTextRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iDisplayTextRsp.iInfoType = RSat::KMeProblem;
            iDisplayTextRsp.iAdditionalInfo.SetLength( 1 );
            iDisplayTextRsp.iAdditionalInfo[0] = RSat::KScreenBusy;

            // Command is not allowed.
            LOG( SIMPLE,
                "DISPLAYTEXT: CDisplayTextHandler::CommandAllowed Not allowed" )
            commandAllowed = EFalse;
            }
        else // UI is closing, command is allowed. UI is launched again when it
             // has been fully closed
            {
            LOG( SIMPLE,
                "DISPLAYTEXT: CDisplayTextHandler::CommandAllowed Ui is closing\
                Command is allowed" )
            }
        }
    else
        {
        LOG( SIMPLE,
                "DISPLAYTEXT: CDisplayTextHandler::CommandAllowed Allowed" )
        }

    if ( !commandAllowed )
        {
        LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::CommandAllowed \
             commandAllowed false" )
        iDisplayTextRsp.SetPCmdNumber( iDisplayTextData.PCmdNumber() );
        TerminalRsp( RSat::EDisplayText, iDisplayTextRspPckg );
        }

    LOG2( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::CommandAllowed exiting,\
          commandAllowed: %d", commandAllowed )
    return commandAllowed;
    }

// -----------------------------------------------------------------------------
// CDisplayTextHandler::NeedUiSession
// -----------------------------------------------------------------------------
//
TBool CDisplayTextHandler::NeedUiSession()
    {
    // Priority and phone idle state is checked in CommandAllowed function, so
    // in this case it has passed and we need UI.
    LOG( NORMAL,
        "DISPLAYTEXT: CDisplayTextHandler::NeedUiSession calling - calling" )

    // Notify Mediator if Cover UI is supported
    if ( iUtils->CoverUiSupported() )
        {
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::EDisplayText;
        medEventData.iAlphaId.iStatus = RSat::EAlphaIdProvided;
        medEventData.iAlphaId.iAlphaId.Copy( iDisplayTextData.iText );
        if ( RSat::ETimeUnitNotSet == iDisplayTextData.iDuration.iTimeUnit &&
            RSat::EClearAfterDelay == iDisplayTextData.iClearScreenTrigger )
            {
            LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::NeedUiSession \
                 KSatDefaultDuration" )
            medEventData.iDuration.iTimeUnit = RSat::ESeconds;
            medEventData.iDuration.iNumOfUnits = KSatDefaultDuration;
            }
        else if (
            ( RSat::ETimeUnitNotSet == iDisplayTextData.iDuration.iTimeUnit ) &&
            ( RSat::EUserClear == iDisplayTextData.iClearScreenTrigger ) )
            {
            LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::NeedUiSession \
                 KSatLongDuration" )
            medEventData.iDuration.iTimeUnit = RSat::ESeconds;
            medEventData.iDuration.iNumOfUnits = KSatLongDuration;
            }
        else
            {
            LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::NeedUiSession \
                 others" )
            medEventData.iDuration = iDisplayTextData.iDuration;
            }

        medEventData.iIconID = iDisplayTextData.iIconId;
        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        }

    LOG( NORMAL,
        "DISPLAYTEXT: CDisplayTextHandler::NeedUiSession - exiting" )
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CDisplayTextHandler::HandleCommand
// -----------------------------------------------------------------------------
//
void CDisplayTextHandler::HandleCommand()
    {
    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::HandleCommand calling" )

    // If this is a High priority message, bring SatUi to foreground.
    if ( RSat::EHighPriority == iDisplayTextData.iPriority )
        {
        LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::HandleCommand bring \
             SatUi to foreground" )
        iUtils->SatUiHandler().ShellController().BringSatUiToForeground();
        }

    // Notify must be here, because it triggers ClearScreen and
    // this notify must be before UpdateImmediateState function call.
    iUtils->NotifyEvent( MSatUtils::EDisplayTextExecuting );

    // Send terminal response immediately if SIM requires it.
    if ( RSat::EImmediateRsp == iDisplayTextData.iImmediateRsp )
        {
        LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::HandleCommand \
             immediate response" )
        // There is sustained text already on display
        if ( iImmediateResponse )
            {
            LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::HandleCommand \
                 iImmediateResponse true" )
            iImmediatePending = ETrue;
            }

        // Generate terminal response
        iDisplayTextRsp.iGeneralResult = RSat::KSuccess;
        iDisplayTextRsp.iInfoType = RSat::KNoAdditionalInfo;
        iDisplayTextRsp.iAdditionalInfo.Zero();
        iDisplayTextRsp.SetPCmdNumber( iDisplayTextData.PCmdNumber() );

        // We cannot use CSatCommandHandler's TerminalRsp here because it
        // restarts the request and we are not yet ready for that
        iUtils->NotifyEvent( MSatUtils::EDelaySimSessionEnd );

        iUtils->USatAPI().TerminalRsp(
            RSat::EDisplayText, iDisplayTextRspPckg );

        iImmediateResponse = ETrue;
        iClearScreenHandler->UpdateImmediateState( ETrue );

        iUtils->NotifyEvent( MSatUtils::ESustainedTextInDisplay );

        // Inform UI that this is sustained text
        iDisplayTextSendData.iSustainedText = ETrue;
        }
    else
        {
        LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::HandleCommand \
             others" )
        // Reset information for other cases.
        // Inform UI that this is not sustained text.
        iDisplayTextSendData.iSustainedText = EFalse;
        }

    // Check if duration data is available.
    if ( ( RSat::ENoDurationAvailable != iDisplayTextData.iDuration.iTimeUnit )
        && ( RSat::ETimeUnitNotSet != iDisplayTextData.iDuration.iTimeUnit )
        && iDisplayTextData.iDuration.iNumOfUnits )
        {
        LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::HandleCommand \
             duration data available" )
        // The resolution of a timer is 1 second.
        TTimeIntervalSeconds duration( 0 );
        duration = DurationInSeconds( iDisplayTextData.iDuration );
        iDisplayTextSendData.iDuration = duration;
        }

    // Build the IPC Package.
    iDisplayTextSendData.iText = iDisplayTextData.iText;
    iDisplayTextSendData.iSimApplicationName = iUtils->SatAppName();
    iDisplayTextSendData.iPCmdNumber = iDisplayTextData.PCmdNumber();
    iDisplayTextSendData.iIconId = iDisplayTextData.iIconId;
    iDisplayTextSendData.iClearScreen = iDisplayTextData.iClearScreenTrigger;

    // Now we can send command to client.
    MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();
    uiSession->SendCommand(
        &iDisplayTextSendPckg,
        &iDisplayTextRspPckg,
        ESatSProactiveDisplayText );

    // Restart request, if immediate response.
    if ( RSat::EImmediateRsp == iDisplayTextData.iImmediateRsp )
        {
        LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::HandleCommand \
             immediate response" )
        // Renew the request
        Start();
        }

    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// CDisplayTextHandler::UiLaunchFailed
// -----------------------------------------------------------------------------
//
void CDisplayTextHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::UiLaunchFailed calling" )

    // Send terminal response
    iDisplayTextRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
    iDisplayTextRsp.iInfoType = RSat::KMeProblem;
    iDisplayTextRsp.iAdditionalInfo.SetLength( 1 );
    iDisplayTextRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
    iDisplayTextRsp.SetPCmdNumber( iDisplayTextData.PCmdNumber() );
    TerminalRsp( RSat::EDisplayText, iDisplayTextRspPckg );

    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::UiLaunchFailed exiting" )
    }

// -----------------------------------------------------------------------------
// CDisplayTextHandler::DurationInSeconds
// Return duration in seconds.
// -----------------------------------------------------------------------------
//
TTimeIntervalSeconds CDisplayTextHandler::DurationInSeconds(
    const RSat::TDuration& aDuration ) const
    {
    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::DurationInSeconds calling" )

    TTimeIntervalSeconds duration( 0 );
    LOG2( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::DurationInSeconds \
          aDuration.iTimeUnit: %d", aDuration.iTimeUnit )
    switch ( aDuration.iTimeUnit )
        {
        case RSat::EMinutes:
            {
            duration = aDuration.iNumOfUnits * KSecondsInMinute;
            break;
            }

        case RSat::ESeconds:
            {
            duration = aDuration.iNumOfUnits;
            break;
            }

        case RSat::ETenthsOfSeconds:
            {
            // If duration exists minimum values is 1 second
            if ( aDuration.iNumOfUnits < KHalfSecond )
                {
                LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::\
                     DurationInSeconds aDuration.iNumOfUnits < KHalfSecond" )
                duration = KSecond;
                }
            else
                {
                // Make Roundup
                duration = ( aDuration.iNumOfUnits + KHalfSecond ) / KSecond;
                }
            break;
            }

        default:
            {
            //duration is 0
            break;
            }
        }

    LOG( SIMPLE, "DISPLAYTEXT: CDisplayTextHandler::DurationInSeconds exiting" )
    return duration;
    }
