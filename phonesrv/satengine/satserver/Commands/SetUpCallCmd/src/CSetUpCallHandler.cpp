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
* Description:  Handles SetUpCall command
*
*/
#include    <e32base.h>
#include    <etelmm.h>
#include    <exterror.h>

#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatSystemState.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "csetupcallrequesthandler.h"
#include    "CSetUpCallHandler.h"
#include    "TSatExtErrorUtils.h"
#include    "SatLog.h"

const TUint8 KBCDAsterisk( 0x0A );
const TUint8 KBCDDash( 0x0B );
const TUint8 KBCDDTMF( 0x0C );
const TUint8 KBCDWild( 0x0D );
const TUint8 KBCDExpansion( 0x0E );

const TUint8 KAsteriskChar( 0x2A );
const TUint8 KDashChar( 0x23 );
const TUint8 KDTMFChar( 0x70 );
const TUint8 KWildChar( 0x77 );
const TUint8 KExpansionChar( 0x2E );

/** Maximum name length. */ 
const TInt KSatMaximumNameLength = 50;

/** Maximum phone number length same as  used by phone. */ 
const TInt KSatMaximumPhoneNumberLength = 100;

/** The subaddress length, see ITU-T I.330 and 3GPP TS 11.14. */ 
const TInt KSatSubAddressLength = 21;

/** The maximum bearer length. The bearer capabilities as 
defined in GSM 04.08. */ 
const TInt KSatBearerLength = 14;


_LIT( KFixedSimEmergencyNumber, "112" );

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSetUpCallHandler* CSetUpCallHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::NewL calling" )

    CSetUpCallHandler* self = new( ELeave ) CSetUpCallHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CSetUpCallHandler::~CSetUpCallHandler()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::~CSetUpCallHandler calling" )

    Cancel();
    
    delete iRequestHandler;
    iRequestHandler = NULL;

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::~CSetUpCallHandler exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatEventObserver.
// Event notification.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::Event( TInt aEvent )
    {
    LOG2( SIMPLE, "SETUPCALL: CSetUpCallHandler::Event calling,aEvent: %i",
          aEvent )

    if ( MSatUtils::ECallControlExecuting == aEvent )
        {
        LOG( SIMPLE,
            "SETUPCALL: CSetUpCallHandler::Event: ECallControlExecuting" )
        iCallControlActive = ETrue;
        }
    else if ( MSatUtils::ECallControlDone == aEvent )
        {
        LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::Event: ECallControlDone" )
        iCallControlActive = EFalse;

        // Check if SetUpCall command has arrived from SIM during CC execution
        // Emergency call is made immediate regardless of call control.
        if ( !IsActive() && iPCmdPending && !iEmergencyCall )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::Event: setupcall" )
            iPCmdPending = EFalse;
            // Execute the setupcall.
            DoHandleCommand();
            }
        }

    CSatCommandHandler::Event( aEvent );

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatCommand.
// Response from the client.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::ClientResponse()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::ClientResponse calling" )

    if ( iQueryRsp.iAccepted )
        {
        // User accepted the call, make the call
        if( iRequestHandler )
            {
            DoSetupCall( *iRequestHandler );
            }
        else
            {
            CompleteSetupCall(
                RSat::KMeUnableToProcessCmd,
                RSat::KNoSpecificMeProblem );
            }
        }
    else
        {
        LOG( NORMAL, 
        "SETUPCALL: CSetUpCallHandler::ClientResponse User Cancel" )

        if ( iQueryRsp.iSessionTerminatedByUser )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::ClientResponse TerminatedByUser" )
            // Notify sim session end command that next sim session end
            // should close the ui session.
            iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
            }

        // End key is pressed during confirmation or user denied call setup,
        // KPCmdNotAcceptedByUser is an expected response,
        CompleteSetupCall( RSat::KPCmdNotAcceptedByUser );
        }

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::DoCancel()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::DoCancel calling" )

    iUtils->USatAPI().NotifySetUpCallCancel();
    if( iRequestHandler )
        {
        iRequestHandler->Cancel();
        }

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Requests the command notification.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iSetUpCallData) RSat::TSetUpCallV1();
    iPCmdPending = EFalse;
    iQueryRsp.iAccepted = EFalse;

    iUtils->USatAPI().NotifySetUpCall( aStatus, iSetUpCallPckg );

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// SetUpCall command is not allowed in following situations:
//      - Phone is not registered to homenetwork and roaming.
//      - phone is ringing, alerting or call is on and SetUpCall command
//        is "make call only if not busy", "make call only if not busy wiht
//        redial" or "Call type not set".
// -----------------------------------------------------------------------------
//
TBool CSetUpCallHandler::CommandAllowed()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CommandAllowed calling" )
    TBool commandAllowed( ETrue );

    // Emergency call can newer be denied.
    // The usage of constant emergency number is according to ETSI TS 31.111.
    iEmergencyCall =
        ( 0 == iSetUpCallData.iAddress.iTelNumber.Compare(
            KFixedSimEmergencyNumber ) );

    if ( !iEmergencyCall )
        {
        RMobilePhone::TMobilePhoneRegistrationStatus networkStatus(
            iUtils->SystemState().GetNetworkRegistrationStatus() );

        if ( ( ( RSat::EAlphaIdProvided !=
                     iSetUpCallData.iAlphaIdConfirmationPhase.iStatus ) &&
               ( ( RSat::ESelfExplanatory ==
                       iSetUpCallData.iIconIdConfirmationPhase.iQualifier ) ||
                 ( RSat::ENotSelfExplanatory ==
                       iSetUpCallData.iIconIdConfirmationPhase.iQualifier ) ) )
                 ||
             ( ( RSat::EAlphaIdProvided !=
                     iSetUpCallData.iAlphaIdCallSetUpPhase.iStatus ) &&
               ( ( RSat::ESelfExplanatory ==
                       iSetUpCallData.iIconIdCallSetUpPhase.iQualifier ) ||
                 ( RSat::ENotSelfExplanatory ==
                       iSetUpCallData.iIconIdCallSetUpPhase.iQualifier ) ) ) )
            {
            // Icon data is received without alpha id.
            CompleteSetupCall( RSat::KCmdDataNotUnderstood );

            commandAllowed = EFalse;
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CommandAllowed Icon received \
            without alpha id" )
            }
        else if ( ( RMobilePhone::ERegisteredOnHomeNetwork != networkStatus ) &&
                  ( RMobilePhone::ERegisteredRoaming != networkStatus ) )
            {
            // Not registered to network.
            CompleteSetupCall( RSat::KMeUnableToProcessCmd, RSat::KNoService );

            commandAllowed = EFalse;
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CommandAllowed Registration not \
            valid" )
            }
        else
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CommandAllowed others" )
            // Call type
            const RSat::TSetUpCallType callType( iSetUpCallData.iType );

            // Command does not allow to make a call if busy
            const TBool dontMakeCallIfBusy(
                ( RSat::EOnlyIfNotBusy == callType ) ||
                ( RSat::EOnlyIfNotBusyWithRedial == callType ) ||
                ( RSat::ESetUpCallTypeNotSet == callType ) );

            if ( dontMakeCallIfBusy )
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CommandAllowed \
                dontMakeCallIfBusy true" )
                // Is the call ongoing
                const TBool callIsOngoing(
                    iUtils->SystemState().IsCallActive() );

                // Is the call incoming
                const TBool callIsIncoming(
                    iUtils->SystemState().IsCallIncoming() );

                if ( callIsOngoing || callIsIncoming )
                    {
                    // Set the terminal response info.
                    CompleteSetupCall(
                        RSat::KMeUnableToProcessCmd,
                        RSat::KMeBusyOnCall );

                    commandAllowed = EFalse;
                    LOG( SIMPLE, 
                    "SETUPCALL: CSetUpCallHandler::CommandAllowed Busy" )
                    }
                }
            }

        if ( commandAllowed )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CommandAllowed Allowed to call" )

            // Set icon command flag whether icon data was received and
            // set qualifier to no icon id
            // To be removed when icons are allowed in this command
            if ( ( RSat::ESelfExplanatory ==
                     iSetUpCallData.iIconIdConfirmationPhase.iQualifier ) ||
                 ( RSat::ENotSelfExplanatory ==
                     iSetUpCallData.iIconIdConfirmationPhase.iQualifier ) ||
                 ( RSat::ESelfExplanatory ==
                     iSetUpCallData.iIconIdCallSetUpPhase.iQualifier ) ||
                 ( RSat::ENotSelfExplanatory ==
                     iSetUpCallData.iIconIdCallSetUpPhase.iQualifier ) )
                {
                LOG( SIMPLE, "SETUPCALL:   ENoIconId" )
                iIconCommand = ETrue;
                iSetUpCallData.iIconIdConfirmationPhase.iQualifier =
                    RSat::ENoIconId;
                iSetUpCallData.iIconIdCallSetUpPhase.iQualifier =
                    RSat::ENoIconId;
                }
            else
                {
                iIconCommand = EFalse;
                }
            }
        }
    else
        {
        LOG( SIMPLE, 
        "SETUPCALL: CSetUpCallHandler::CommandAllowed Emergency call" )
        }

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CommandAllowed exiting" )
    
    return commandAllowed;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Need for ui session.
// -----------------------------------------------------------------------------
//
TBool CSetUpCallHandler::NeedUiSession()
    {
    LOG( NORMAL, "SETUPCALL: CSetUpCallHandler::NeedUiSession calling" )

    // Notify Cover UI if it's supported
    if ( iUtils->CoverUiSupported() )
        {
        LOG( NORMAL, 
        "SETUPCALL: CSetUpCallHandler::NeedUiSession CoverUiSupported" )
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::ESetUpCall;
        medEventData.iAlphaId = iSetUpCallData.iAlphaIdCallSetUpPhase;
        medEventData.iDuration.iTimeUnit = RSat::ESeconds;
        medEventData.iDuration.iNumOfUnits = KSatDefaultDuration;
        medEventData.iIconID = iSetUpCallData.iIconIdCallSetUpPhase;
        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        }

    LOG( NORMAL, "SETUPCALL: CSetUpCallHandler::NeedUiSession exiting" )
    return ETrue;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when USAT API notifies that command.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::HandleCommand()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::HandleCommand calling" )

    if ( iEmergencyCall )
        {
        LOG( SIMPLE, 
        "SETUPCALL: CSetUpCallHandler::HandleCommand iEmergencyCall true" )
        if ( iRequestHandler )
            {
            CreateEmergencyCall( *iRequestHandler );
            }
        else
            {
            iEmergencyCall = EFalse;
            // Set the terminal response info.
            CompleteSetupCall(
                RSat::KMeUnableToProcessCmd,
                RSat::KNoSpecificMeProblem );
            }
        }
    else
        {
        const RSat::TAlphaIdStatus alphaIdStatus(
            iSetUpCallData.iAlphaIdCallSetUpPhase.iStatus );

        // If the alpha id is null, then use SAT name as a alpha identifier.
        if ( ( RSat::EAlphaIdNull == alphaIdStatus )  ||
             ( RSat::EAlphaIdNotPresent == alphaIdStatus ) )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::HandleCommand set AlphaId" )
            iSetUpCallData.iAlphaIdCallSetUpPhase.iAlphaId =
                iUtils->SatAppName();
            iSetUpCallData.iAlphaIdCallSetUpPhase.iStatus =
                RSat::EAlphaIdProvided;
            }

        // Copy the data to package, which is sent to client.
        iQueryData.iCommand = ESatSSetUpCallQuery;
        iQueryData.iQueryText.Copy(
            iSetUpCallData.iAlphaIdConfirmationPhase.iAlphaId );
        iQueryData.iSimApplicationName.Copy( iUtils->SatAppName() );
        iQueryData.iAlphaIdStatus = ESatAlphaIdNotNull; // Always
        iQueryData.iIconId.iIdentifier =
            iSetUpCallData.iIconIdConfirmationPhase.iIdentifier;

        LOG2( SIMPLE, 
        "SETUPCALL: iSetUpCallData.iIconIdConfirmationPhase.iQualifier: %d",
        iSetUpCallData.iIconIdConfirmationPhase.iQualifier )
        switch ( iSetUpCallData.iIconIdConfirmationPhase.iQualifier )
            {
            case RSat::ESelfExplanatory:
                {
                // Icon qualifier is self explanatory (to display instead
                // of the alpha id or text string).
                iQueryData.iIconId.iIconQualifier = ESatSelfExplanatory;
                break;
                }

            case RSat::ENotSelfExplanatory:
                {
                // Icon qualifier is not self explanatory.
                iQueryData.iIconId.iIconQualifier = ESatNotSelfExplanatory;
                break;
                }

            default:
                {
                // Icon qualifier not present
                iQueryData.iIconId.iIconQualifier = ESatENoIconId;
                break;
                }
            }

        // If call control is active, set up call is made
        // after the call control note is showed in ui ie
        // ECallControlDone event is notified.
        if ( !iCallControlActive )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::HandleCommand iCallControlActive \
            false" )
            iUtils->NotifyEvent( MSatUtils::ESetUpCallExecuting );

            TRAPD( regErr, iUtils->RegisterServiceRequestL(
                ESatSProactiveQuery,
                ESatSProactiveQueryResponse,
                this ) );
            LOG2( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::HandleCommand regErr: %d", regErr )
            if ( KErrNone != regErr )
                {
                // Possible memory allocation error. Send error terminal
                // response
                UiLaunchFailed();
                }
            else
                {
                // Send query to UI
                iUtils->SatUiHandler().UiSession()->SendCommand(
                    &iQueryPckg,
                    &iQueryRspPckg,
                    ESatSProactiveQuery );
                }
            }
        else
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::HandleCommand iCallControlActive \
            true" )
            // Set pending flag on
            iPCmdPending = ETrue;
            }
        }

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Indication that UI lanching failed.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::UiLaunchFailed calling" )

    CompleteSetupCall( RSat::KMeUnableToProcessCmd, 
                       RSat::KNoSpecificMeProblem );

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::UiLaunchFailed exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpCallHandler::SetupCallRequestComplete
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::SetupCallRequestComplete( TInt aErrCode )
    {
    LOG( SIMPLE,
    "SETUPCALL: CSetUpCallHandler::SetupCallRequestComplete calling" )

    LOG2( NORMAL,
    "SETUPCALL: CSetUpCallHandler::SetupCallRequestComplete aErrCode %d",
    aErrCode )
    
    if( !iEmergencyCall )
        {
        CompleteSetupCallWithStatus( aErrCode );
        }
    else
        {
        iEmergencyCall = EFalse;

        if ( KErrNone == aErrCode )
            {
            // Set result
            CompleteSetupCall( RSat::KSuccess );
            }
        else
            {
            // The emergency call implementation 
            // before S60 SAT migration from AIW to EtelMM
            // According current information, no requirement for this.
            // We don't return extended network error.
            CompleteSetupCall(
                RSat::KNetworkUnableToProcessCmd,
                RSat::KNoSpecificMeProblem );

            LOG( SIMPLE,
            "SETUPCALL: CSetUpCallHandler::HandleEmergencyDialL Network unable \
            to process this" )
            }
        }
    LOG2( NORMAL,
        "SETUPCALL: CSetUpCallHandler::SetupCallRequestComplete exiting %d", aErrCode )
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized, harmless.
CSetUpCallHandler::CSetUpCallHandler() :
    CSatCommandHandler(),
    iSetUpCallData(),
    iSetUpCallPckg( iSetUpCallData ),
    iSetUpCallRsp(),
    iSetUpCallRspPckg( iSetUpCallRsp ),
    iQueryData(),
    iQueryPckg( iQueryData ),
    iQueryRsp(),
    iQueryRspPckg( iQueryRsp ),
    // To be removed when icons are allowed in this command
    iIconCommand( EFalse )
    {
    LOG( SIMPLE,
        "SETUPCALL: CSetUpCallHandler::CSetUpCallHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::ConstructL()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::ConstructL calling" )

    
    iRequestHandler = CSetupCallRequestHandler::NewL( iUtils->MultiModeApi(),
            this ); 
    // Create request handler. This is same that LaunchBrowser uses, so this
    // is needed also in HandleCommand - function.
    iUtils->RegisterServiceRequestL(
        ESatSProactiveQuery,
        ESatSProactiveQueryResponse,
        this );

    iUtils->RegisterL( this, MSatUtils::ECallControlExecuting );
    iUtils->RegisterL( this, MSatUtils::ECallControlDone );

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// Performs the request to dial
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::DoSetupCall( CSetupCallRequestHandler& aHandler )
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::DoSetupCallL calling" )
    
    if( CheckSetupCallParam() )
        {

        RSat::TSetUpCallType callType( iSetUpCallData.iType );
    
        TDes& telNumber( iSetUpCallData.iAddress.iTelNumber );
        CheckNumber( telNumber );

        RMobileCall::TMobileCallParamsV7 dialParams;
        RMobileCall::TMobileCallParamsV7Pckg package( dialParams );
    
        //Redail has been removed from MCL, no redail support.
        dialParams.iAutoRedial = EFalse;
        dialParams.iBearerMode = RMobileCall::EMulticallNewBearer;
        dialParams.iCallParamOrigin = RMobileCall::EOriginatorSIM;
        dialParams.iSubAddress = iSetUpCallData.iSubAddress;
        dialParams.iBearerCap1 = iSetUpCallData.iCapabilityConfigParams;
        
        dialParams.iBCRepeatIndicator = RMobileCall::EBCAlternateMode;
        
        dialParams.iIconId.iQualifier = RMobileCall::ENoIconId;
        
        
        dialParams.iAlphaId = iSetUpCallData.iAlphaIdCallSetUpPhase.iAlphaId;
        LOG2( NORMAL, 
            "SETUPCALL: CSetUpCallHandler::DoSetupCallL id:%S",
            &dialParams.iAlphaId )
        
        LOG2( NORMAL, 
            "SETUPCALL: CSetUpCallHandler::DoSetupCallL number:%S",
            &iSetUpCallData.iAddress.iTelNumber )
        
        TBool terminateOtherCall( EFalse );
        // check if we need to disconnect other calls
        if ( ( RSat::EDisconnectOtherCalls == callType ) ||
             ( RSat::EDisconnectOtherCallsWithRedial == callType ) )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::DoSetupCallL end other call" )
            terminateOtherCall = ETrue ;
            }
        
        aHandler.DialNumber( package, iSetUpCallData.iAddress.iTelNumber,
                terminateOtherCall, iUtils->CreateAsyncToSyncHelper() );
        }
    else
        {
        CompleteSetupCallWithStatus( KErrArgument );
        }
    
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::DoSetupCallL exiting" )
    }

// -----------------------------------------------------------------------------
// Return terminal response filled according to dial result.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::CompleteSetupCallWithStatus(
    const TInt32 aStatus )
    {
    LOG2( SIMPLE,
        "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus calling: \
        %i", aStatus )

    // Form Terminal Response
    if ( KErrNone != aStatus )
        {
        switch ( aStatus )
            {
            case KErrGsmCCCallRejected:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus Call \
                Control not allowed" )
                // If SetUpCall is not allowed by SIM in Call Control, then
                // give a correct response.
                CompleteSetupCall(
                    RSat::KInteractionWithCCPermanentError,
                    RSat::KActionNotAllowed );
                break;
                }

            case KErrGsmCCBearerCapabilityNotAuthorised:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
                Beyond ME Capabilities" )
                // If bearer capability is not authorized, return correct value
                CompleteSetupCall( RSat::KCmdBeyondMeCapabilities );
                break;
                }

            case KErrAbort:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus User \
                abort" )
                if ( iQueryRsp.iSessionTerminatedByUser )
                    {
                    LOG( SIMPLE, 
                    "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
                    TerminatedByUser" )
                    // Notify sim session end command that next sim session end
                    // should close the ui session.
                    iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
                    }

                // User has ended redial mechanism.
                CompleteSetupCall( RSat::KCallClearedBeforeConnectionOrReleased );
                break;
                }

            case KErrGeneral:
            case KErrArgument:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus Data \
                not understood" )
                CompleteSetupCall( RSat::KCmdDataNotUnderstood );
                break;
                }

            case KErrAccessDenied:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
                Network unable to process this" )
                CompleteSetupCall(
                    RSat::KNetworkUnableToProcessCmd,
                    RSat::KNoSpecificMeProblem );
                break;
                }
                
            case KErrSatControl:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
                Call Command performed, but modified by Call Control" )
                // Call control modified the type of request indicated in
                // the proactive command, and the action requested by 
                // call control was performed successfully.
                CompleteSetupCall( RSat::KModifiedByCallControl );
                break;
                }
            case KErrSatBusy:
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
                KInteractionWithCCTemporaryError" )
                // Call control some time can case this error.
                CompleteSetupCall( RSat::KInteractionWithCCTemporaryError );
                break;
                }
                
            default:
                {
                LOG( SIMPLE,
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
                Network unable to process with error info" )

                iSetUpCallRsp.iGeneralResult = RSat::KNetworkUnableToProcessCmd;
                iSetUpCallRsp.iInfoType = RSat::KSatNetworkErrorInfo;

                // Lower byte contains the error cause.
                iSetUpCallRsp.iAdditionalInfo.SetLength( 1 );

                // Get mapped additional info
                TUint8 addInfo( TSatExtErrorUtils::MapError( aStatus ) );
                iSetUpCallRsp.iAdditionalInfo[0] =
                    static_cast<TUint16>( addInfo );

                // Send terminal response
                TerminalRsp( RSat::ESetUpCall, iSetUpCallRspPckg );
                break;
                }
            }
        }
    else
        {
        // Convert terminal rsp if icon used
        RSat::TIconQualifier qualifier1(
            iSetUpCallData.iIconIdConfirmationPhase.iQualifier );
        RSat::TIconQualifier qualifier2(
            iSetUpCallData.iIconIdCallSetUpPhase.iQualifier );

        RSat::TPCmdResult result( RSat::KSuccess );

        // Icon support for call confirmtion phase not done.
        if ( !iQueryRsp.iRequestedIconDisplayed &&
           ( ( RSat::ESelfExplanatory == qualifier1 ) ||
             ( RSat::ENotSelfExplanatory == qualifier1 ) ) )
            {
            result = RSat::KSuccessRequestedIconNotDisplayed;
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
            Confirmation icon not shown" )
            }
        // Icon support for call setup phase not done.
        else if ( ( RSat::ESelfExplanatory == qualifier2 ) ||
                  ( RSat::ENotSelfExplanatory == qualifier2 ) )
            {
            // Until 2009-10 the phone and NTSY not support the icon.
            // to be updated after the updating of the phone and NTSY
            result = RSat::KSuccessRequestedIconNotDisplayed;
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus Icon \
            not shown" )
            }
        // If command had icon data and was done succesfully, report that icon
        // was not shown.
        // To be removed when icons are allowed in this command.
        else
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
            others" )
            if ( iIconCommand )
                {
                LOG( SIMPLE, 
                "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus \
                iIconCommand true" )
                result = RSat::KSuccessRequestedIconNotDisplayed;
                }
            }
        // Set result
        CompleteSetupCall( result );
        }

    LOG( SIMPLE,
        "SETUPCALL: CSetUpCallHandler::CompleteSetupCallWithStatus exiting" )
    }

// -----------------------------------------------------------------------------
// Return terminal response filled according to result.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::CompleteSetupCall(
    const RSat::TPCmdResult aGeneralResult )
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CompleteSetupCall calling" )

    // Fill Terminal Response.
    iSetUpCallRsp.iGeneralResult = aGeneralResult;
    iSetUpCallRsp.iInfoType = RSat::KNoAdditionalInfo;
    iSetUpCallRsp.iAdditionalInfo.Zero();
    iSetUpCallRsp.SetPCmdNumber( iSetUpCallData.PCmdNumber() );

    // Send terminal response.
    TerminalRsp( RSat::ESetUpCall, iSetUpCallRspPckg );

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CompleteSetupCall exiting" )
    }

// -----------------------------------------------------------------------------
// Return terminal response filled according to result.
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::CompleteSetupCall(
    const RSat::TPCmdResult aGeneralResult,
    const TInt16 aAdditionalInfo )
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CompleteSetupCall calling" )

    // Fill Terminal Response.
    iSetUpCallRsp.iGeneralResult = aGeneralResult;
    iSetUpCallRsp.iInfoType = RSat::KMeProblem;
    iSetUpCallRsp.iAdditionalInfo.SetLength( 1 );
    iSetUpCallRsp.iAdditionalInfo[0] = aAdditionalInfo;
    iSetUpCallRsp.SetPCmdNumber( iSetUpCallData.PCmdNumber() );

    // Send terminal response.
    TerminalRsp( RSat::ESetUpCall, iSetUpCallRspPckg );

    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CompleteSetupCall exiting" )
    }

// -----------------------------------------------------------------------------
// Checks the dialling number string for extended BCD
// values. Changes them to correct characters
// see ETSI 11.11 10.5.1
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::CheckNumber( TDes& aNumber ) const
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CheckNumber calling" )
    
    for ( TInt i = 0; i < aNumber.Length(); i++ )
        {
        // check values
        if ( KBCDAsterisk == aNumber[i] )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CheckNumber KAsteriskChar" )
            aNumber[i] = KAsteriskChar;
            }

        if ( KBCDDash == aNumber[i] )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CheckNumber KDashChar" )
            aNumber[i] = KDashChar;
            }

        if ( KBCDDTMF == aNumber[i] )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CheckNumber KDTMFChar" )
            aNumber[i] = KDTMFChar;
            }

        if ( KBCDWild == aNumber[i] )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CheckNumber KWildChar" )
            aNumber[i] = KWildChar;
            }

        if ( KBCDExpansion == aNumber[i] )
            {
            LOG( SIMPLE, 
            "SETUPCALL: CSetUpCallHandler::CheckNumber KExpansionChar" )
            aNumber[i] = KExpansionChar;
            }
        }
    
    LOG2( SIMPLE, 
    "SETUPCALL: CSetUpCallHandler::CheckNumber length of aNumber: %d",
     aNumber.Length() )
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CheckNumber exiting" )
    }

// -----------------------------------------------------------------------------
// Create emergency call
// -----------------------------------------------------------------------------
//
void CSetUpCallHandler::CreateEmergencyCall( 
        CSetupCallRequestHandler& aHandler )
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CreateEmergencyCall calling" )
   
    aHandler.DialEmergencyCall( iSetUpCallData.iAddress.iTelNumber );
    
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CreateEmergencyCall exiting" )    
    }

// -----------------------------------------------------------------------------
// check setup call param.
// -----------------------------------------------------------------------------
//
TBool CSetUpCallHandler::CheckSetupCallParam()
    {
    LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CheckSetupCallParam calling" )

    TBool valid( ETrue );
    if ( iSetUpCallData.iAddress.iTelNumber.Length()
          > KSatMaximumPhoneNumberLength )
        {
        LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CheckSetupCallParam num" )
        valid = EFalse;
        }    
    else if ( iSetUpCallData.iAlphaIdCallSetUpPhase.iAlphaId.Length()
               > KSatMaximumNameLength )
        {
        LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CheckSetupCallParam name" )
        valid = EFalse;
        }    
    else if ( iSetUpCallData.iSubAddress.Length() > KSatSubAddressLength )
        {
        LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CheckSetupCallParam sub" )
        valid = EFalse;
        }    
    else if ( iSetUpCallData.iCapabilityConfigParams.Length()
               > KSatBearerLength )
        {
        LOG( SIMPLE, "SETUPCALL: CSetUpCallHandler::CheckSetupCallParam bear" )
        valid = EFalse;
        }    
    LOG2( SIMPLE, 
    "SETUPCALL: CSetUpCallHandler::CheckSetupCallParam exiting %d", valid )
    
    return valid;        
    }
// End Of File
