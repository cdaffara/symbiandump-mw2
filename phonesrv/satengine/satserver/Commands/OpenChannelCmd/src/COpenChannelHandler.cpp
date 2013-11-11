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
* Description:  Handles OpenChannel command
*
*/


#include    <etelpckt.h>
#include    <etelmmerr.h>

#include    "COpenChannelHandler.h"
#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatUiSession.h"
#include    "MSatSystemState.h"
#include    "MSatApnHandler.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "SatLog.h"
#include    "extendedconnpref.h"

const TUint8 KChannelStatusLength( 2 );

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COpenChannelHandler* COpenChannelHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::NewL calling" )

    COpenChannelHandler* self = new( ELeave ) COpenChannelHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
COpenChannelHandler::~COpenChannelHandler()
    {
    LOG( SIMPLE,
        "OPENCHANNEL: COpenChannelHandler::~COpenChannelHandler calling" )

    Cancel();
    iDataChannel = NULL;

    delete iConnectionInfo.iOverrideSet;

    LOG( SIMPLE,
        "OPENCHANNEL: COpenChannelHandler::~CCloseChannelHandler exiting" )
    }

// ---------------------------------------------------------------------------
// From class MSatCommand.
// Response from the client.
// ---------------------------------------------------------------------------
//
void COpenChannelHandler::ClientResponse()
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::ClientResponse calling" )

    // Query response from client.
    if ( !iQueryRsp.iAccepted ) // Not accepted by user
        {
        // Close data channel
        CloseChannel();

        if ( iQueryRsp.iSessionTerminatedByUser ) // Terminated by user
            {
            LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::ClientResponse \
                Session terminated by user" )
            // Notify sim session end command that next sim session end
            // should close the ui session.
            iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );

            // Terminal response, SimSessionTerminatedByUser
            iOpenChannelRsp.iGeneralResult = RSat::KPSessionTerminatedByUser;
            }
        else
            {
            LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::ClientResponse \
                User not accept command" )

            // Send terminal response -> User not accepted
            iOpenChannelRsp.iGeneralResult = RSat::KPCmdNotAcceptedByUser;
            }

        iOpenChannelRsp.iInfoType = RSat::KNoAdditionalInfo;
        iOpenChannelRsp.iAdditionalInfo.Zero();
        iOpenChannelRsp.SetPCmdNumber( iOpenChannelData.PCmdNumber() );
        TerminalRsp( iPCmd, iOpenChannelRspPckg );
        }
    else // Accepted by user
        {
        LOG( NORMAL,
            "OPENCHANNEL: COpenChannelHandler::ClientResponse User accepted" )
        // User accepted
        const TInt linkEst( iOpenChannelData.iLinkEst );

        // SetUp channel if activation is on demand
        if ( ( RSat::EOnDemand == linkEst ) && ( KErrNone != SetUpChannel() ) )
            {
            // Close channel and send error if setup failed
            CloseChannel();
            LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::ClientResponse \
                SetUp failed" )
            iOpenChannelRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iOpenChannelRsp.iInfoType = RSat::KNoAdditionalInfo;
            iOpenChannelRsp.iAdditionalInfo.Zero();
            iOpenChannelRsp.SetPCmdNumber( iOpenChannelData.PCmdNumber() );
            TerminalRsp( iPCmd, iOpenChannelRspPckg );
            }
        else
            {
            // Send terminal response
            SendSuccessTerminalRsp();
            }

        }

    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatConnectionObserver.
// Notification from the connection.
// -----------------------------------------------------------------------------
//
void COpenChannelHandler::ConnectionNotification( TInt aError )
    {
    LOG2( SIMPLE, "OPENCHANNEL: COpenChannelHandler::ConnectionNotification\
         calling with code %i", aError )

    // Check connection status
    if ( KErrNone == aError )
        {
        // If information indication to the user is needed, show it now.
        if ( iNeedUiSession )
            {
            LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::HandleCommand \
                Sending UI Notification" )

            // Send confirmation to UI and wait for client response
            SendConfirmToUi();
            }
        else
            {
            // Send successfull terminal response
            SendSuccessTerminalRsp();
            }
        }
    else
        {
        // Activation failed, Close channel and send error
        LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::ConnectionNotification \
            Channel activation failed" )

        // Close channel
        CloseChannel();

        // PDP Activation failed
        iOpenChannelRsp.iGeneralResult = RSat::KBearerIndepProtocolError;
        iOpenChannelRsp.iInfoType = RSat::KMeProblem;
        iOpenChannelRsp.iAdditionalInfo.SetLength( 1 );
        iOpenChannelRsp.iAdditionalInfo[0] = RSat::KNoSpecificBIPError;
        iOpenChannelRsp.SetPCmdNumber( iOpenChannelData.PCmdNumber() );
        TerminalRsp( iPCmd, iOpenChannelRspPckg );
        }

    LOG( SIMPLE,
        "OPENCHANNEL: COpenChannelHandler::ConnectionNotification exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Event notification.
// -----------------------------------------------------------------------------
//
void COpenChannelHandler::Event( TInt aEvent )
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::Event calling" )

    // Check event
    if ( MSatUtils::ESatUiLaunched == aEvent )
        {
        LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::Event ESatUiLaunched" )
        iUtils->UnregisterEvent( this, MSatUtils::ESatUiLaunched );

        // Send confirm to UI
        SendConfirmToUi();
        }

    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void COpenChannelHandler::DoCancel()
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::DoCancel calling" )

    iUtils->USatAPI().NotifyOpenChannelCancel();

    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Requests the command notification.
// -----------------------------------------------------------------------------
//
void COpenChannelHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::IssueUSATRequest calling" )

    // Reset values
    iDataChannel = NULL;
    iSetUpWithModifications = EFalse;
    iNeedUiSession = EFalse;

    // Reset open channel data.
    new (&iOpenChannelData) RSat::TOpenGprsChannelV4();
    new (&iQueryData) TSatQueryV1();

    iUtils->USatAPI().NotifyOpenChannel( aStatus, iOpenChannelPckg );
    iUtils->NotifyEvent( MSatUtils::EBipCommandDoneExecuting );

    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// OpenChannel command is not allowed in following situations:
//      - Phone is not registered to homenetwork and roaming.
//      - phone is ringing, alerting or call is on and OpenChannel command
//        is "make call only if not busy", "make call only if not busy wiht
//        redial" or "Call type not set".
// -----------------------------------------------------------------------------
//
TBool COpenChannelHandler::CommandAllowed()
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::CommandAllowed calling" )

    // Define the PCmd here since this is the first place after notification
    iPCmd = RSat::EOpenChannelGprs; // Other possible values
                                    // RSat::EOpenChannelCs
                                    // RSat::EOpenChannelLocal

    // Check parameters here, because every terminal response must contain info
    // about Bearer Set and BufferSize.
    TBool result( CheckParameters() );

    // Indicates channel activation type
    const TBool activateImmediately(
        RSat::EImmediate == iOpenChannelData.iLinkEst );

    //lint -e{961} Else block not needed.
    if ( ( RSat::EAlphaIdProvided != iOpenChannelData.iAlphaId.iStatus ) &&
         ( RSat::ESelfExplanatory == iOpenChannelData.iIconId.iQualifier ||
           RSat::ENotSelfExplanatory == iOpenChannelData.iIconId.iQualifier ) )
        {
        iOpenChannelRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        iOpenChannelRsp.iInfoType = RSat::KNoAdditionalInfo;
        iOpenChannelRsp.iAdditionalInfo.Zero();
        result = EFalse;
        LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::CommandAllowed \
            icon received without alpha id" )
        }
    // If parameters are OK, check rest.
    else if ( result && activateImmediately )
        {
        // Set result to False. If checks goes OK, this is set to ETrue.
        result = EFalse;

        RMobilePhone::TMobilePhoneRegistrationStatus registrationStatus(
            iUtils->SystemState().GetNetworkRegistrationStatus() );

        // Indicates is there service on network
        const TBool noService(
            ( RMobilePhone::ERegisteredOnHomeNetwork != registrationStatus ) &&
            ( RMobilePhone::ERegisteredRoaming != registrationStatus ) );

        if ( RMobilePhone::ERegistrationUnknown == registrationStatus )
            {
            LOG( NORMAL,
                "OPENCHANNEL: COpenChannelHandler::CommandAllowed \
                NetworkRegStatus not available" )
            iOpenChannelRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iOpenChannelRsp.iInfoType = RSat::KMeProblem;
            iOpenChannelRsp.iAdditionalInfo.SetLength( 1 );
            iOpenChannelRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
            }
        // Check registration status
        else if ( noService )
            {
            // No service
            LOG( NORMAL,
                "OPENCHANNEL: COpenChannelHandler::CommandAllowed No service" )

            iOpenChannelRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iOpenChannelRsp.iInfoType = RSat::KMeProblem;
            iOpenChannelRsp.iAdditionalInfo.SetLength( 1 );
            iOpenChannelRsp.iAdditionalInfo[0] = RSat::KNoService;
            }
        else
            {
            // Command allowed
            LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::CommandAllowed \
                Command is allowed Immediate activation" )
            result = ETrue;
            }
        }

    if ( !result ) // Send Terminal response, if result is not OK
        {
        LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::CommandAllowed \
        result not OK" )
        iOpenChannelRsp.SetPCmdNumber( iOpenChannelData.PCmdNumber() );
        TerminalRsp( iPCmd, iOpenChannelRspPckg );
        }
    else
        {
        // Register to handle UI commands.
        TRAPD( err, iUtils->RegisterServiceRequestL( ESatSProactiveQuery,
            ESatSProactiveQueryResponse, this ) );

        LOG2( NORMAL, "OPENCHANNEL: COpenChannelHandler::CommandAllowed \
        err: %d", err )
        // Above function leaves only when there is not enough memory to
        // allocate new object. So this case is very rare.
        if ( KErrNone != err )
            {
            LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::CommandAllowed \
                 Error while registering service request" )

            // Send general terminal response (no BIP specific)
            iOpenChannelRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iOpenChannelRsp.iInfoType = RSat::KMeProblem;
            iOpenChannelRsp.iAdditionalInfo.SetLength( 1 );
            iOpenChannelRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
            iOpenChannelRsp.SetPCmdNumber( iOpenChannelData.PCmdNumber() );
            TerminalRsp( iPCmd, iOpenChannelRspPckg );

            // Cannot complete, there are some major memory problems
            result = EFalse;
            }
        // Set icon command flag whether icon data was received and 
        // set qualifier to no icon id
        // To be removed when icons are allowed in this command
        else if ( ( RSat::ESelfExplanatory ==
            iOpenChannelData.iIconId.iQualifier ) ||
            ( RSat::ENotSelfExplanatory ==
            iOpenChannelData.iIconId.iQualifier ) )
            {
            LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::CommandAllowed \
                 ENoIconId" )
            iIconCommand = ETrue;
            iOpenChannelData.iIconId.iQualifier = RSat::ENoIconId;
            }
        else
            {
            LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::CommandAllowed \
                 others" )
            iIconCommand = EFalse;
            }
        }

    LOG2( SIMPLE, "OPENCHANNEL: COpenChannelHandler::CommandAllowed exiting,\
    result: %d", result )
    return result;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Need for ui session.
// -----------------------------------------------------------------------------
//
TBool COpenChannelHandler::NeedUiSession()
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::NeedUiSession calling" )

    // UI is NOT needed in case where AlphaID is provided and it is a NULL
    iNeedUiSession =
        !( RSat::EAlphaIdNull == iOpenChannelData.iAlphaId.iStatus );

    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::NeedUiSession exiting" )
    // We do not need UI session at this point, but we will launch it by
    // ourselves when we do.
    return EFalse;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when USAT API notifies that command.
// -----------------------------------------------------------------------------
//
void COpenChannelHandler::HandleCommand()
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::HandleCommand calling" )

    // Notify others
    iUtils->NotifyEvent( MSatUtils::EOpenChannelExecuting );
    iUtils->NotifyEvent( MSatUtils::EBipCommandExecuting );

    TInt error( MSatBIPUtils::ESatBIPNoChannelAvail ); // By default, negative

    // Create data channel
    TRAPD( leaveErr,
        iDataChannel = iUtils->BipUtils().CreateChannelL(
            iBearerType, error ) );

    // Check the channel creation
    if ( ( MSatBIPUtils::ESatBIPSuccess != error ) || 
         ( KErrNone != leaveErr ) )
        {
        LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::HandleCommand\
            Channel creation failed" )

        // Channel creation failed -> Send terminal response
        iOpenChannelRsp.iGeneralResult = RSat::KBearerIndepProtocolError;
        iOpenChannelRsp.iInfoType = RSat::KMeProblem;
        iOpenChannelRsp.iAdditionalInfo.SetLength( 1 );
        iOpenChannelRsp.iAdditionalInfo[0] = RSat::KNoChannelAvailable;
        iOpenChannelRsp.SetPCmdNumber( iOpenChannelData.PCmdNumber() );
        TerminalRsp( iPCmd, iOpenChannelRspPckg );

        LOG( SIMPLE, "OPENCHANNEL: COpenChannel::HandleCommand exiting" )
        return;
        }

    LOG( NORMAL,
        "OPENCHANNEL: COpenChannelHandler::HandleCommand Channel creation OK" )

    // Open channel or request confirmation from user
    const TInt linkEst( iOpenChannelData.iLinkEst );

    // If not confirmation to user or immediate activation, open channel
    if ( ( !iNeedUiSession || RSat::EImmediate == linkEst )
           && KErrNone != SetUpChannel() )
        {
        // Setup failed, close channel and send error
        LOG( NORMAL,
            "OPENCHANNEL: COpenChannelHandler::HandleCommand SetUp failed" )

        // Close channel, there was an error
        CloseChannel();

        // Channel setup failed
        iOpenChannelRsp.iGeneralResult = RSat::KBearerIndepProtocolError;
        iOpenChannelRsp.iInfoType = RSat::KMeProblem;
        iOpenChannelRsp.iAdditionalInfo.SetLength( 1 );
        iOpenChannelRsp.iAdditionalInfo[0] = RSat::KNoSpecificBIPError;
        iOpenChannelRsp.SetPCmdNumber( iOpenChannelData.PCmdNumber() );
        TerminalRsp( iPCmd, iOpenChannelRspPckg );

        LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::HandleCommand \
            exiting" )
        return;
        }

    // If immediate activation, activate.
    if ( RSat::EImmediate == linkEst )
        {
        LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::HandleCommand \
             immediate activation" )
        // Check activation
        TInt activateError = ActivateChannel();
        if ( KErrNone != activateError )
            {
            // Activation failed, Close channel and send error
            LOG2( NORMAL, "OPENCHANNEL: COpenChannelHandler::HandleCommand \
                Channel activation failed error = %i", activateError )
            // Close channel
            CloseChannel();

            // Check is there a call ongoing
            const TBool callIsOngoing( iUtils->SystemState().IsCallActive() ||
                iUtils->SystemState().IsCallIncoming() );

            iOpenChannelRsp.iGeneralResult = RSat::KBearerIndepProtocolError;

            // Map the error to correct response
            if ( KErrGprsUserAuthenticationFailure == activateError )
                {
                LOG( DETAILED, 
                "OPENCHANNEL: COpenChannelHandler::HandleCommand Security \
                error" )
                activateError = RSat::KSecurityError;
                }
            else if ( KErrGprsServicesNotAllowed == activateError || callIsOngoing )
                {
                LOG( DETAILED, 
                "OPENCHANNEL: COpenChannelHandler::HandleCommand Busy on \
                call or CC on GPRS" )
                activateError = RSat::KMeBusyOnCall;
                iOpenChannelRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
                }
            else if ( ( KErrMMEtelActivationBlockedByCallControlNoText ==
                            activateError ) ||
                      ( KErrMMEtelActivationBlockedByCallControlWithText ==
                            activateError ) )
                {
                iOpenChannelRsp.iGeneralResult = RSat::KInteractionWithCCPermanentError;
                activateError = RSat::KActionNotAllowed;

                LOG( DETAILED, 
                "OPENCHANNEL: COpenChannelHandler::HandleCommand CC on GPRS \
                not allowed" )
                }
            else
                {
                LOG( DETAILED, 
                "OPENCHANNEL: COpenChannelHandler::HandleCommand No specific \
                BIP error" )
                activateError = RSat::KNoSpecificBIPError;
                }

            // PDP Activation failed
            iOpenChannelRsp.iInfoType = RSat::KMeProblem;
            iOpenChannelRsp.iAdditionalInfo.SetLength( 1 );
            iOpenChannelRsp.iAdditionalInfo[0] =
                static_cast<TUint16>( activateError );
            iOpenChannelRsp.SetPCmdNumber( iOpenChannelData.PCmdNumber() );
            TerminalRsp( iPCmd, iOpenChannelRspPckg );
            }
        }
    else // OnDemand. Send terminal response or confirm to UI
        {
        // If information indication to the user is needed, show it now.
        if ( iNeedUiSession )
            {
            LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::HandleCommand \
                Sending UI Notification" )
            // Send confirmation to UI and wait for client response
            SendConfirmToUi();
            }
        else
            {
            // Send successfull terminal response
            SendSuccessTerminalRsp();
            }
        }

    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Indicates the failure of launching ui client.
// -----------------------------------------------------------------------------
//
void COpenChannelHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::UiLaunchFailed calling" )

    iOpenChannelRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
    iOpenChannelRsp.iInfoType = RSat::KMeProblem;
    iOpenChannelRsp.iAdditionalInfo.SetLength( 1 );
    iOpenChannelRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
    iOpenChannelRsp.SetPCmdNumber( iOpenChannelData.PCmdNumber() );
    TerminalRsp( iPCmd, iOpenChannelRspPckg );

    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::UiLaunchFailed exiting" )
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized.
COpenChannelHandler::COpenChannelHandler() :
    CSatCommandHandler(),
    iOpenChannelData(),
    iOpenChannelPckg( iOpenChannelData ),
    iOpenChannelRsp(),
    iOpenChannelRspPckg( iOpenChannelRsp ),
    iQueryData(),
    iQueryPckg( iQueryData ),
    iQueryRsp(),
    iQueryRspPckg( iQueryRsp ),
    // To be removed when icons are allowed in this command
    iIconCommand( EFalse )
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::COpenChannelHandler \
        calling - exiting" )
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COpenChannelHandler::ConstructL()
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::ConstructL calling" )

    iUtils->RegisterServiceRequestL( ESatSProactiveQuery,
        ESatSProactiveQueryResponse, this );

    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// Sets new connection settings for channel to open.
// -----------------------------------------------------------------------------
//
void COpenChannelHandler::SetConnectionSettingsL(
    MSatBIPUtils::TSatBearerType aBearerType )
    {
    LOG2( SIMPLE,
        "OPENCHANNEL: COpenChannelHandler::SetConnectionSettingsL calling,\
        aBearerType: %d", aBearerType )

    RPacketContext::TProtocolType pdpType = RPacketContext::EPdpTypeIPv4;
    TUint32 defaultId( 0 );
    switch ( aBearerType )
        {
        case MSatBIPUtils::ESatGPRS:
            {
            // Resolve override settings
            if ( iOpenChannelData.iAccessName.Length() > 0 )
                {
                LOG( NORMAL, 
                "OPENCHANNEL: COpenChannelHandler::SetConnectionSettingsL \
                Getting APN" )

                TUint32 iapId( 0 );
                TUint32 nwId( 0 );
                TBool apnCreated( EFalse );

                pdpType = RPacketContext::EPdpTypeIPv6;

                if ( RSat::EIPv4Address ==
                    iOpenChannelData.iDestinationAddress.iType )
                    {
                    LOG( NORMAL, 
                    "OPENCHANNEL: COpenChannelHandler::SetConnectionSettingsL \
                    IPV4 Address" )
                    pdpType = RPacketContext::EPdpTypeIPv4;
                    }
                iUtils->BipUtils().ApnHandler().GetApnInfoL
                    (
                    iOpenChannelData.iAccessName,
                    iOpenChannelData.iUserLogin,
                    iOpenChannelData.iUserPassword,
                    iapId,
                    nwId,
                    apnCreated,
                    pdpType, // IPv6 or IPv4
                    iOpenChannelData.iLocalAddress.iAddress
                    );

                if ( apnCreated )
                    {
                    LOG( NORMAL, 
                    "OPENCHANNEL: COpenChannelHandler::SetConnectionSettingsL \
                    APN created by SAT" )
                    // Store APN Id for later use
                    iConnectionInfo.iCreatedApnId = nwId;
                    }

                SetOverrideSettingsL( iapId );
                }
            else
                {
                LOG( NORMAL, "OPENCHANNEL: SetConnectionSettingsL \
                No APN, using defaults" )
                // No APN, use default settings
                TRAP_IGNORE( defaultId = 
                    iUtils->BipUtils().ApnHandler().FindDefaultApL( pdpType ) );
                SetOverrideSettingsL( defaultId );
                }
            break;
            }

        default:
            {
            LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::\
                SetConnectionSettingsL exiting default" )
            // Leave if not supported
            User::Leave( KErrNotSupported );
            }
        }

#ifdef SAT_USE_DUMMY_TSY
    // No APN, use default settings
    TRAPD( err, 
    defaultId = iUtils->BipUtils().ApnHandler().FindDefaultApL( pdpType ) );
    LOG2( NORMAL, "OPENCHANNEL: COpenChannelHandler::SetConnectionSettingsL \
        No APN, using defaults err = %i", err )
    SetOverrideSettingsL( defaultId );
#endif // SAT_USE_DUMMY_TSY

    LOG( SIMPLE,
        "OPENCHANNEL: COpenChannelHandler::SetConnectionSettingsL exiting" )
    }

// -----------------------------------------------------------------------------
// Sets override settings into connection information.
// -----------------------------------------------------------------------------
//
void COpenChannelHandler::SetOverrideSettingsL( const TUint32 aIapId )
    {
    
    LOG2( SIMPLE,
    "OPENCHANNEL: COpenChannelHandler::SetOverrideSettingsL aIapId: %i", 
    aIapId )

    delete iConnectionInfo.iOverrideSet;
    iConnectionInfo.iOverrideSet = NULL;

    // Override connection preferences
    TExtendedConnPref* overrideSettings = new( ELeave ) TExtendedConnPref;
    overrideSettings->SetIapId( aIapId );
    overrideSettings->SetNoteBehaviour( 
            TExtendedConnPref::ENoteBehaviourConnSilent );
    iConnectionInfo.iOverrideSet = overrideSettings;

    LOG( SIMPLE,
        "OPENCHANNEL: COpenChannelHandler::SetOverrideSettingsL exiting" )
    }

// -----------------------------------------------------------------------------
// Sets up data channel. Does not activate context.
// -----------------------------------------------------------------------------
//
TInt COpenChannelHandler::SetUpChannel()
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::SetUpChannel calling" )

    // get connection settings from CommDB and
    // set connection settings to data channel
    TRAPD( status,
           SetConnectionSettingsL( iBearerType );
           iDataChannel->SetupConnectionL( iConnectionInfo ); );

    LOG2( SIMPLE,
        "OPENCHANNEL: COpenChannelHandler::SetUpChannel exiting with status %i",
        status )
    return status;
    }

// -----------------------------------------------------------------------------
// Activates context.
// -----------------------------------------------------------------------------
//
TInt COpenChannelHandler::ActivateChannel()
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::ActivateChannel calling" )

    // Activate the connection
    TRAPD( err, iDataChannel->ActivateConnectionL( this ) );

    LOG2( SIMPLE, "OPENCHANNEL: COpenChannelHandler::ActivateChannel exiting\
        with error %i:", err )
    return err;
    }

// -----------------------------------------------------------------------------
// Creates and sends the terminal response on successful cases.
// -----------------------------------------------------------------------------
//
void COpenChannelHandler::SendSuccessTerminalRsp()
    {
    LOG( SIMPLE,
        "OPENCHANNEL: COpenChannelHandler::SendSuccessTerminalRsp calling" )

    iDataChannel->GetNegotiatedQoSParams( iOpenChannelRsp.iBearer.iParams );

    // Get the channel status
    RSat::TChannelStatus channelStatus;
    TRAPD( err, channelStatus = iUtils->BipUtils().GenerateChannelStatusL(
            iDataChannel->ChannelId(), 0 ) );
    LOG2( SIMPLE, "OPENCHANNEL: COpenChannelHandler::SendSuccessTerminalRsp \
          error: %i", err )
    
    if ( KErrNone == err )
        {
        iOpenChannelRsp.iInfoType = RSat::KChannelStatusInfo;
        iOpenChannelRsp.iAdditionalInfo.SetLength( KChannelStatusLength );

        // Channel ID + PDP context status
        iOpenChannelRsp.iAdditionalInfo[0] = channelStatus[0];

        // Channel status
        iOpenChannelRsp.iAdditionalInfo[1] = channelStatus[1];
        iOpenChannelRsp.SetPCmdNumber( iOpenChannelData.PCmdNumber() );

        // Define the result of this command handling
        if ( iSetUpWithModifications )
            {
            LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::HandleCommand\
                Channel setup modified" )
            iOpenChannelRsp.iGeneralResult = RSat::KPerformedWithModifications;
            }
        else if ( !iQueryRsp.iRequestedIconDisplayed &&
            ( ( RSat::ESelfExplanatory ==
            iOpenChannelData.iIconId.iQualifier ) ||
            ( RSat::ENotSelfExplanatory ==
            iOpenChannelData.iIconId.iQualifier ) ) )
            {
            LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::HandleCommand\
                Open channel successful, no icon displayed" )
            iOpenChannelRsp.iGeneralResult =
                RSat::KSuccessRequestedIconNotDisplayed;
            }
        else
            {
            LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::HandleCommand\
                Open channel successful" )
            iOpenChannelRsp.iGeneralResult = RSat::KSuccess;
            }
        }
    else
        {
        LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::HandleCommand\
            Channel not found!" )
        iOpenChannelRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        iOpenChannelRsp.iInfoType = RSat::KMeProblem;
        iOpenChannelRsp.iAdditionalInfo.SetLength( 1 );
        iOpenChannelRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
        }

    // If command had icon data and was done succesfully, report that icon
    // was not shown
    // To be removed when icons are allowed in this command
    if ( ( RSat::KSuccess == iOpenChannelRsp.iGeneralResult ) &&
        iIconCommand )
        {
        LOG( SIMPLE, "COpenChannelHandler::SendSuccessTerminalRsp requested\
             icon not displayed" )
        iOpenChannelRsp.iGeneralResult =
            RSat::KSuccessRequestedIconNotDisplayed;
        }

    // Sending terminal response
    TerminalRsp( iPCmd, iOpenChannelRspPckg );

    LOG( SIMPLE,
        "OPENCHANNEL: COpenChannelHandler::SendSuccessTerminalRsp exiting" )
    }

// -----------------------------------------------------------------------------
// Sends confirmation query to UI.
// -----------------------------------------------------------------------------
//
void COpenChannelHandler::SendConfirmToUi()
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::SendConfirmToUi calling" )

    // Command Duration for Mediator Client
    TUint8 commandDuration( KSatDurationNotSet );

    // Launch UI of not launched. This function is called again when UI is
    // launched.
    if ( !LaunchUiSession() )
        {
        LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::SendConfirmToUi \
             not launch UI" )
        // Create IPC data to UI
        iQueryData.iCommand = ESatOpenChannelQuery;
        iQueryData.iQueryText = iOpenChannelData.iAlphaId.iAlphaId;
        iQueryData.iIconId.iIdentifier = iOpenChannelData.iIconId.iIdentifier;
        LOG2( SIMPLE, "COpenChannelHandler::SendConfirmToUi \
             iOpenChannelData.iIconId.iQualifier:%d",
             iOpenChannelData.iIconId.iQualifier )
        // Check Icon
        switch ( iOpenChannelData.iIconId.iQualifier )
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
        LOG2( SIMPLE, "COpenChannelHandler::SendConfirmToUi \
             iOpenChannelData.iAlphaId.iStatus:%d",
             iOpenChannelData.iAlphaId.iStatus )
        // Define AlphaID status
        switch ( iOpenChannelData.iAlphaId.iStatus )
            {
            case RSat::EAlphaIdNotPresent:
                {
                iQueryData.iAlphaIdStatus = ESatAlphaIdNotProvided;
                commandDuration = KSatLongDuration;
                break;
                }

            case RSat::EAlphaIdProvided:
                {
                iQueryData.iAlphaIdStatus = ESatAlphaIdNotNull;
                commandDuration = KSatLongDuration;
                break;
                }

            default:
                {
                iQueryData.iAlphaIdStatus = ESatAlphaIdNull;
                commandDuration = KSatDurationNotSet;
                break;
                }
            }

        // Send notification to Mediator client if Cover UI is supported
        if ( iUtils->CoverUiSupported() )
            {
            LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::SendConfirmToUi \
                 CoverUiSupported" )
            TSatCommandData medEventData;
            medEventData.iPCmdNumber = iPCmd;
            medEventData.iAlphaId = iOpenChannelData.iAlphaId;
            medEventData.iDuration.iTimeUnit = RSat::ESeconds;
            medEventData.iDuration.iNumOfUnits = commandDuration;
            medEventData.iIconID = iOpenChannelData.iIconId;
            TSatCommandPckg tPckg( medEventData );
            iUtils->RaiseSatEvent( tPckg );
            }
        // Send notification to UI
        MSatUiSession* session = iUtils->SatUiHandler().UiSession();
        session->SendCommand(
            &iQueryPckg, &iQueryRspPckg, ESatSProactiveQuery );
        }

    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::SendConfirmToUi exiting" )
    }

// -----------------------------------------------------------------------------
// Closes the channel in error cases.
// -----------------------------------------------------------------------------
//
void COpenChannelHandler::CloseChannel()
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::CloseChannel calling" )

    // If data channel is not null, close it
    if ( iDataChannel )
        {
        LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::CloseChannel have\
             channel" )
        iUtils->BipUtils().CloseChannel( iDataChannel->ChannelId() );
        }

    if ( iConnectionInfo.iCreatedApnId > 0 )
        {
        LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::CloseChannel delete\
             APN" )
        // APN created -> Remove it
        TRAP_IGNORE( iUtils->BipUtils().ApnHandler().DeleteApnL(
            iConnectionInfo.iCreatedApnId ) )
        }

    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::CloseChannel exiting" )
    }

// -----------------------------------------------------------------------------
// Checks parameters from SIM and makes modifications, if needed.
// -----------------------------------------------------------------------------
//
TBool COpenChannelHandler::CheckParameters()
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::CheckParameters calling" )
    // Return variable. Indicates is the context activation possible with
    // given / modified parameters.
    TBool paramsOk( EFalse );

    // Check buffer size
    if ( KSatBIPMaxBufferSize < iOpenChannelData.iBufferSize ||
         0 == iOpenChannelData.iBufferSize )
        {
        LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::CheckParameters \
            Changing buffersize" )
        // Too big requested buffer size -> set to max
        iConnectionInfo.iBufferSize = KSatBIPMaxBufferSize;
        // put modified buffer size to terminal response
        iOpenChannelRsp.iBufferSize = KSatBIPMaxBufferSize;
        // Modified parameters
        iSetUpWithModifications = ETrue;
        }
    else
        {
        iConnectionInfo.iBufferSize = iOpenChannelData.iBufferSize;
        iOpenChannelRsp.iBufferSize = iOpenChannelData.iBufferSize;
        }

    // Set bearer for the terminal response. This information is mandatory to
    // each terminal response.
    iOpenChannelRsp.iBearer = iOpenChannelData.iBearer;

    // Indicates is there destination address defined
    const TBool destinationAddressNotDefined
        (
        RSat::EIPv4Address != iOpenChannelData.iDestinationAddress.iType &&
        RSat::EIPv6Address != iOpenChannelData.iDestinationAddress.iType
        );

    // Indicates is there a transport protocol defined
    const TBool transportProtoNotDefined
        (
        RSat::ETcp != iOpenChannelData.iSimMeInterface.iTransportProto &&
        RSat::EUdp != iOpenChannelData.iSimMeInterface.iTransportProto
        );

    // Indicates is there a transport port number defined
    const TBool transportPortNumberNotDefined
        (
        0 == iOpenChannelData.iSimMeInterface.iPrtNumber
        );

    // Indicates is selected bearer supported
    const TBool bearerNotSupported
        (
        RSat::EPCmdTypeNotSet != iOpenChannelData.iPCmdType &&
        RSat::EGprsBearer != iOpenChannelData.iPCmdType &&
        RSat::EAnyBearer != iOpenChannelData.iPCmdType
        );

    // Check parameters
    paramsOk = !( destinationAddressNotDefined ||
                  transportProtoNotDefined ||
                  bearerNotSupported ||
                  transportPortNumberNotDefined );

    // Check is connection possible
    if ( paramsOk )
        {
        LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::CheckParameters \
        Bearer type is GPRS" )
        // Bearer type is GPRS
        iBearerType = MSatBIPUtils::ESatGPRS;
        // Set bearer type for the terminal response in case it was not defined
        iOpenChannelRsp.iBearer.iType = RSat::EGPRSBearer;
        // Destination address
        iConnectionInfo.iDestination = iOpenChannelData.iDestinationAddress;
        // Local address
        iConnectionInfo.iSource = iOpenChannelData.iLocalAddress;
        // Bearer parameters
        iConnectionInfo.iBearerParams = iOpenChannelData.iBearer.iParams;
        // Set protocol info to connection data
        iConnectionInfo.iProtocol = iOpenChannelData.iSimMeInterface;
        }
    else if ( transportProtoNotDefined ||
              bearerNotSupported ||
              transportPortNumberNotDefined )
        {
        // Fail
        LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::CheckParameters \
        Unable to connect" )
        // If parameters are not OK, send terminal response
        LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::CommandAllowed \
            Not valid parameters" )
        iOpenChannelRsp.iGeneralResult = RSat::KCmdBeyondMeCapabilities;
        iOpenChannelRsp.iInfoType = RSat::KNoAdditionalInfo;
        iOpenChannelRsp.iAdditionalInfo.Zero();
        }
    else if ( destinationAddressNotDefined )
        {
        // Fail
        LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::CheckParameters \
        Destination not set" )
        iOpenChannelRsp.iGeneralResult = RSat::KErrorRequiredValuesMissing;
        iOpenChannelRsp.iInfoType = RSat::KNoAdditionalInfo;
        iOpenChannelRsp.iAdditionalInfo.Zero();
        }
    else
        {
        // Fail
        LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::CheckParameters \
        Unable to connect" )
        // If parameters are not OK, send terminal response
        LOG( NORMAL, "OPENCHANNEL: COpenChannelHandler::CheckParameters \
            Not valid parameters" )
        iOpenChannelRsp.iGeneralResult = RSat::KCmdBeyondMeCapabilities;
        iOpenChannelRsp.iInfoType = RSat::KNoAdditionalInfo;
        iOpenChannelRsp.iAdditionalInfo.Zero();
        }

    LOG2( SIMPLE, "OPENCHANNEL: COpenChannelHandler::CheckParameters exiting,\
          paramsOk: %d", paramsOk )
    return paramsOk;
    }

// -----------------------------------------------------------------------------
// Launches UI if it not active.
// -----------------------------------------------------------------------------
//
TBool COpenChannelHandler::LaunchUiSession()
    {
    LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::LaunchUiSession calling" )
    TBool retVal( EFalse );

    // If ui session is not availabe, SATUI has to be
    // launched.
    if ( NULL == iUtils->SatUiHandler().UiSession() )
        {
        TRAPD( err,
               iUtils->RegisterL( this, MSatUtils::ESatUiLaunched );
               // Try to launch the ui client.
               iUtils->SatUiHandler().LaunchSatUiL();
             ); // End of TRAPD

        if ( KErrNone != err )
            {
            LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::LaunchUiSession \
                 Ui launch failed" )
            // Ui launch failed, unregister event
            iUtils->UnregisterEvent( this, MSatUtils::ESatUiLaunched );
            // Notify SIM that ui launch failed.
            UiLaunchFailed();
            }
        else
            {
            LOG( SIMPLE, "OPENCHANNEL: COpenChannelHandler::LaunchUiSession \
                 Ui launch successfully" )
            // Return true indicating that we had to launch UI
            retVal = ETrue;
            }
        }

    LOG2( SIMPLE, "OPENCHANNEL: COpenChannelHandler::LaunchUiSession exiting,\
          retVal: %d", retVal )
    return retVal;
    }
