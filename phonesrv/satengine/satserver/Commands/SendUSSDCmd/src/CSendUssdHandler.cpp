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
* Description:  Handles SendUssd command
*
*/


#include    <cphcltussdsatclient.h>
#include    <cphcltussd.h>
#include    <exterror.h>
#include    <centralrepository.h>
#include    <SATPrivateCRKeys.h>
#include    <hbdevicemessageboxsymbian.h>

#include    "MSatSystemState.h"
#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "msatmultimodeapi.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CSendUssdHandler.h"
#include    "TSatExtErrorUtils.h"
#include    "SatLog.h"

 /** 
  * USSD messages coded as a packed string within 160 octets, as defined for a 
  * ussd-String within GSM 04.80 and GSM 03.38. if the Dcs is 7 bit, a ussd 
  * string can have 182 charactor
  */
const TInt   KSatMaxUSSDString( 182 );

// USSD DCS coding.
const TUint8 KSatDcs7Bit( 0x40 );
const TUint8 KSatDcs8Bit( 0x44 );
const TUint8 KSatDcsUCS2( 0x48 );
const TUint8 KSatDcsUnknown( 0xFF );

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSendUssdHandler* CSendUssdHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::NewL calling" )

    CSendUssdHandler* self = new( ELeave ) CSendUssdHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CSendUssdHandler::~CSendUssdHandler()
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::~CSendUssdHandler calling" )

    Cancel();

    if ( iUssdClient )
        {
        delete iUssdClient;
        iUssdClient = NULL;
        }

    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::~CSendUssdHandler exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatCommand.
// Response from the client
// -----------------------------------------------------------------------------
//
void CSendUssdHandler::ClientResponse()
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::ClientResponse calling" )

    if ( iQueryRsp.iAccepted && !iNotificationSent )
        {
        LOG( NORMAL, 
        "SENDUSSD: CSendUssdHandler::ClientResponse Sending notification" )
        iNotificationSent = ETrue;

        // Register service request
        TRAP_IGNORE( iUtils->RegisterServiceRequestL(
            ESatSProactiveNotification,
            ESatSProactiveNotificationResponse,
            this ) )

        // Send notification
        iUtils->SatUiHandler().UiSession()->SendCommand(
            &iNotificationDataPckg,
            &iNotificationRspPckg,
            ESatSProactiveNotification );
        }

    // Notification sent
    else if ( iNotificationRsp.iAccepted && iNotificationSent )
        {
        LOG( NORMAL, 
        "SENDUSSD: CSendUssdHandler::ClientResponse Notification response" )
        }

    // User reject
    else
        {
        iUserAccepted = EFalse;
        iSendUssdRsp.iInfoType = RSat::KMeProblem;
        iSendUssdRsp.iAdditionalInfo.SetLength( 1 );
        iSendUssdRsp.iAdditionalInfo[0] = RSat::KScreenBusy;
        // Cannot return KPCmdNotAcceptedByUser (ETSI 11.14 v8.3.0 p65)
        iSendUssdRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;

        if ( iQueryRsp.iSessionTerminatedByUser )
            {
            LOG( NORMAL, 
            "SENDUSSD: CSendUssdHandler::ClientResponse \
            ESessionTerminatedByUser" )
            // Notify sim session end command that next sim session end
            // should close the ui session.
            iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
            }

        SendTerminalResponse();
        }

    // Release the wait
    if ( iWait.IsStarted() )
        {
        LOG( NORMAL, "SENDUSSD: CSendUssdHandler::ClientResponse stop iWait" )
        iWait.AsyncStop();
        }

    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Waits for indication of user rejection
// -----------------------------------------------------------------------------
//
void CSendUssdHandler::Event( TInt aEvent )
    {
    LOG2( SIMPLE,
        "SENDUSSD: CSendUssdHandler::Event calling, aEvent:%d", aEvent )

    switch ( aEvent )
        {
        case MSatUtils::ECancelledUsingEndKey:
            {
            // Notify sim session end command that next sim session end
            // should close the ui session.
            iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
            // This event is handled as above, but notification must be done.
            }
        //lint -fallthrough intended here
        case MSatUtils::ECommandCancelled:
            {
            // Cancel Ussd sending
            if ( iUssdClient )
                {
                LOG( SIMPLE, 
                "SENDUSSD: CSendUssdHandler::Event iUssdClient true" )
                iUssdClient->SendSatMessageCancel();
                iSendUssdRsp.iGeneralResult =
                    RSat::KUssdTransactionTerminatedByUser;
                iSendUssdRsp.iInfoType = RSat::KNoAdditionalInfo;
                iSendUssdRsp.iAdditionalInfo.Zero();
                SendTerminalResponse();
                }
            break;
            }

        default:
            {
            // Move event to base class
            CSatCommandHandler::Event( aEvent );
            }
        }

    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CSendUssdHandler::DoCancel()
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::DoCancel calling" )

    iUtils->USatAPI().NotifySendUssdCancel();

    if ( iUssdClient )
        {
        LOG( SIMPLE, 
        "SENDUSSD: CSendUssdHandler::DoCancel iUssdClient true" )
        delete iUssdClient;
        iUssdClient = NULL;
        }

    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Requests the command notification.
// -----------------------------------------------------------------------------
//
void CSendUssdHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iSendUssdData) RSat::TSendUssdV1();
    iNeedUiSession = ETrue;
    iQueryRsp.iAccepted = EFalse; // default
    iNotificationRsp.iAccepted = EFalse;
    iSendUssdRsp.iGeneralResult = RSat::KPSessionTerminatedByUser; // default

    iUtils->USatAPI().NotifySendUssd( aStatus, iSendUssdPckg );

    // Unregister from events
    iUtils->UnregisterEvent( this, MSatUtils::ECommandCancelled );
    iUtils->UnregisterEvent( this, MSatUtils::ECancelledUsingEndKey );

    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Precheck before executing the command.
// -----------------------------------------------------------------------------
//
TBool CSendUssdHandler::CommandAllowed()
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::CommandAllowed calling" )

    // Allow next terminal response to be sent
    iTerminalRespSent = EFalse;
    
    iSendUssdRsp.iUssdString.iUssdString.FillZ();
    iSendUssdRsp.iUssdString.iDcs = 0;

    RMobilePhone::TMobilePhoneRegistrationStatus registrationStatus(
        iUtils->SystemState().GetNetworkRegistrationStatus() );

    TBool commandAllowed( ETrue );

    // If icon data without alpha id
    if ( ( RSat::EAlphaIdProvided != iSendUssdData.iAlphaId.iStatus ) &&
        ( ( RSat::ESelfExplanatory == iSendUssdData.iIconId.iQualifier ) ||
        ( RSat::ENotSelfExplanatory == iSendUssdData.iIconId.iQualifier ) ) )
        {
        commandAllowed = EFalse;
        iSendUssdRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        iSendUssdRsp.iInfoType = RSat::KNoAdditionalInfo;
        iSendUssdRsp.iAdditionalInfo.Zero();
        LOG( SIMPLE,
        "SENDUSSD: CSendUssdHandler::CommandAllowed Icon without alphaid \
        not allowed" )
        }
    else if (
         ( RMobilePhone::ERegisteredOnHomeNetwork != registrationStatus ) &&
         ( RMobilePhone::ERegisteredRoaming != registrationStatus ) )
        {
        commandAllowed = EFalse;
        iSendUssdRsp.iInfoType = RSat::KMeProblem;
        iSendUssdRsp.iAdditionalInfo.SetLength( 1 );
        iSendUssdRsp.iAdditionalInfo[0] = RSat::KNoService;
        iSendUssdRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        LOG( SIMPLE,
        "SENDUSSD: CSendUssdHandler::CommandAllowed Not registered in legal \
        network" )
        }
    // OK
    else
        {
        // Check Ussd string validity
        TInt dataLength( iSendUssdData.iUssdString.iUssdString.Length() );
        LOG2( SIMPLE, 
        "SENDUSSD: CSendUssdHandler::CommandAllowed Length of string: %d", 
        dataLength )

        // Validate USSD string length.
        TBool dataValid(
            ( dataLength > 0 ) && ( dataLength <= KSatMaxUSSDString ) );

        if ( dataValid )
            {
            LOG( SIMPLE, "SENDUSSD:  dataValid true" )
            // Validate Data Coding Scheme.
            dataValid = iUtils->MultiModeApi().IsValidUssdDcs(
                iSendUssdData.iUssdString.iDcs );
            if ( !dataValid )
                {
                LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::CommandAllowed Dcs \
                not acceptable" )
                }
            }

        // Second check in case DcsValid() returns EFalse
        if ( !dataValid )
            {
            LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::CommandAllowed \
            dataValid false" )
            commandAllowed = EFalse;
            iSendUssdRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
            iSendUssdRsp.iInfoType = RSat::KNoAdditionalInfo;
            iSendUssdRsp.iAdditionalInfo.Zero();
            }
        }

    // Check is command allowd
    if ( !commandAllowed )
        {
        LOG( SIMPLE, "SENDUSSD: \
            CSendUssdHandler::CommandAllowed exiting - not allowed" )
        SendTerminalResponse();
        }
    // Set icon command flag whether icon data was received and set qualifier
    // to no icon id
    // To be removed when icons are allowed in this command
    else if ( ( RSat::ESelfExplanatory ==
        iSendUssdData.iIconId.iQualifier ) ||
        ( RSat::ENotSelfExplanatory ==
        iSendUssdData.iIconId.iQualifier ) )
        {
        LOG( SIMPLE, "SENDUSSD: \
            CSendUssdHandler::CommandAllowed ENoIconId" )
        iIconCommand = ETrue;
        iSendUssdData.iIconId.iQualifier = RSat::ENoIconId;
        }
    else
        {
        iIconCommand = EFalse;
        }

    LOG2( SIMPLE, "SENDUSSD: CSendUssdHandler::CommandAllowed exiting,\
    commandAllowed: %d", commandAllowed )
    return commandAllowed;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Answers for need of UI session.
// -----------------------------------------------------------------------------
//
TBool CSendUssdHandler::NeedUiSession()
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::NeedUiSession calling" )

    iNeedUiSession = !TransparentUssdSending();

    // Notify Cover UI if it's supported
    if ( iNeedUiSession && iUtils->CoverUiSupported() )
        {
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::ESendUssd;
        medEventData.iAlphaId =  iSendUssdData.iAlphaId;
        if ( iUtils->SystemState().IsConfirmSatOperationsOn() )
            {
            LOG( SIMPLE, 
            "SENDUSSD: CSendUssdHandler::NeedUiSession KSatLongDuration" )
            medEventData.iDuration.iNumOfUnits = KSatLongDuration;
            }
        else
            {
            LOG( SIMPLE, 
            "SENDUSSD: CSendUssdHandler::NeedUiSession KSatDefaultDuration" )
            medEventData.iDuration.iNumOfUnits = KSatDefaultDuration;
            }
        medEventData.iDuration.iTimeUnit = RSat::ESeconds;
        medEventData.iIconID = iSendUssdData.iIconId;
        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        }

    LOG2( SIMPLE, 
    "SENDUSSD: CSendUssdHandler::NeedUiSession exiting,iNeedUiSession:%d",
    iNeedUiSession )
    return iNeedUiSession;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when USAT API notifies that command.
// -----------------------------------------------------------------------------
//
void CSendUssdHandler::HandleCommand()
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::HandleCommand calling" )

    iUtils->NotifyEvent( MSatUtils::ESendUssdExecuting );

    // This is ETrue, until user rejects
    iUserAccepted = ETrue;

    if ( iNeedUiSession )
        {

        TRAP_IGNORE(
        // Register to listen user cancel events:
        // Cancel key event from dialog
        iUtils->RegisterL( this, MSatUtils::ECommandCancelled );
        // End key from dialog
        iUtils->RegisterL( this, MSatUtils::ECancelledUsingEndKey ) )

        // Build Qyery and Notify packages
        TSatAlphaIdStatus alphaIdStatus;
        if ( RSat::EAlphaIdNotPresent == iSendUssdData.iAlphaId.iStatus )
            {
            LOG( SIMPLE, 
            "SENDUSSD: CSendUssdHandler::HandleCommand EAlphaIdNotPresent" )
            alphaIdStatus = ESatAlphaIdNotProvided;
            }

        else if ( RSat::EAlphaIdProvided == iSendUssdData.iAlphaId.iStatus )
            {
            LOG( SIMPLE, 
            "SENDUSSD: CSendUssdHandler::HandleCommand EAlphaIdProvided" )
            alphaIdStatus = ESatAlphaIdNotNull;
            }

        else
            {
            LOG( SIMPLE, 
            "SENDUSSD: CSendUssdHandler::HandleCommand others" )
            alphaIdStatus = ESatAlphaIdNull;
            }

        // Has to be casted to TInt before casting to TSatIconQualifier, because
        // GCC warns about the direct cast.
        const struct TSatIconId iconId = { iSendUssdData.iIconId.iIdentifier,
            static_cast<TSatIconQualifier>(
                static_cast<TInt>( iSendUssdData.iIconId.iQualifier ) ) };

        iQueryData.iCommand = ESatSSendUssdQuery;
        iQueryData.iQueryText = iSendUssdData.iAlphaId.iAlphaId;
        iQueryData.iIconId = iconId;
        iQueryData.iAlphaIdStatus = alphaIdStatus;

        iNotificationSent = EFalse;
        iNotificationData.iCommand = ESatSSendUssdNotify;
        iNotificationData.iText = iSendUssdData.iAlphaId.iAlphaId;
        iNotificationData.iIconId = iconId;
        iNotificationData.iAlphaIdStatus = alphaIdStatus;

        MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();

        // Send either query or notification
        if ( iQueryOn )
            {
            LOG( NORMAL, 
            "SENDUSSD: CSendUssdHandler::HandleCommand Sending Query" )
            iNotificationSent = EFalse;

            // Register service request
            TRAP_IGNORE( iUtils->RegisterServiceRequestL(
                ESatSProactiveQuery,
                ESatSProactiveQueryResponse,
                this ) )

            // Send query
            uiSession->SendCommand(
                &iQueryPckg,
                &iQueryRspPckg,
                ESatSProactiveQuery );
            }
         else
            {
            LOG( NORMAL, 
            "SENDUSSD: CSendUssdHandler::HandleCommand Sending notification" )

            iNotificationSent = ETrue;

            // Register service request
            TRAP_IGNORE( iUtils->RegisterServiceRequestL(
                ESatSProactiveNotification,
                ESatSProactiveNotificationResponse,
                this ) )

            // Send notification
            uiSession->SendCommand(
                &iNotificationDataPckg,
                &iNotificationRspPckg,
                ESatSProactiveNotification );
            }

        if ( !iWait.IsStarted() )
            {
            LOG( SIMPLE, 
            "SENDUSSD: CSendUssdHandler::HandleCommand start iWait" )
            // Notification / query sent to UI, wait for response
            iWait.Start();
            }
        }

    if ( iUserAccepted )
        {
        LOG( SIMPLE, 
        "SENDUSSD: CSendUssdHandler::HandleCommand UserAccepted" )
        // No need to wait response, send Ussd string
        SendUssdString();
        }

    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Indicates the failure of launching ui client
// -----------------------------------------------------------------------------
//
void CSendUssdHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::UiLaunchFailed calling" )

    iSendUssdRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
    iSendUssdRsp.iInfoType = RSat::KMeProblem;
    iSendUssdRsp.iAdditionalInfo.SetLength( 1 );
    iSendUssdRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
    SendTerminalResponse();

    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::UiLaunchFailed exiting" )
    }
    
// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized.
CSendUssdHandler::CSendUssdHandler() :
    CSatCommandHandler(),
    iSendUssdData(),
    iSendUssdPckg( iSendUssdData ),
    iSendUssdRsp(),
    iSendUssdRspPckg( iSendUssdRsp ),
    iQueryData(),
    iQueryPckg( iQueryData ),
    iQueryRsp(),
    iQueryRspPckg( iQueryRsp ),
    iNotificationData(),
    iNotificationDataPckg( iNotificationData ),
    iNotificationRsp(),
    iNotificationRspPckg( iNotificationRsp ),
    // To be removed when icons are allowed in this command
    iIconCommand( EFalse ),
    iIsSatDisplayUssdResult( EFalse )
    {
    LOG( SIMPLE,
        "SENDUSSD: CSendUssdHandler::CSendUssdHandler calling" )
    TRAP_IGNORE( iIsSatDisplayUssdResult = SatDisplayUssdResultL(); )

    LOG( SIMPLE,
        "SENDUSSD: CSendUssdHandler::CSendUssdHandler exiting" )
    }


// -----------------------------------------------------------------------------
// Handles the Ussd string sending
// -----------------------------------------------------------------------------
//
void CSendUssdHandler::SendUssdString()
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::SendUssdString calling" )

    TBool sendCompletedFirst( EFalse ); // This is always false

    TBuf<RSat::KStringMaxSize> sendMessage;
    sendMessage.Copy( iSendUssdData.iUssdString.iUssdString );

    TBuf<RSat::KStringMaxSize> receiveMessage;

    TRAPD( error, SendUssdStringL(
        sendMessage,
        iSendUssdData.iUssdString.iDcs,
        receiveMessage,
        sendCompletedFirst,
        iSendUssdRsp.iUssdString.iDcs ) );

    iSendUssdRsp.iUssdString.iUssdString.Copy( receiveMessage );

    if ( ( RSat::EAlphaIdProvided != iSendUssdData.iAlphaId.iStatus )
        && iIsSatDisplayUssdResult )
        {
        // if no Alpha ID provided, show the text note.
        LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::SendUssdString Show Note" )
        TRAP_IGNORE(
        ShowUssdResponseNoteL( iSendUssdRsp.iUssdString.iUssdString ) );
        }

    HandleSendUssdResult( error );

    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::SendUssdString exiting" )
    }

// -----------------------------------------------------------------------------
// Handles the Ussd string sending.
// -----------------------------------------------------------------------------
//
void CSendUssdHandler::SendUssdStringL(
    const TDesC& aSendMessage,
    const TUint8 aSendDcs,
    TDes& aReceiveMessage,
    TBool& aSendCompletedFirst,
    TUint8& aReceivedDcs )
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::SendUssdStringL calling" )
    TInt error( KErrNone );

    if ( iUssdClient )
        {
        delete iUssdClient;
        iUssdClient = NULL;
        }

    // This needs to be EFalse, because otherwise KERN-EXEC:3 and SAT Server
    // crashes.
    iUssdClient = CPhCltUssdSatClient::NewL( EFalse );

    LOG2( NORMAL, 
    "SENDUSSD: CSendUssdHandler::SendUssdStringL aSendDcs:       %x", 
    aSendDcs )
    LOG2( NORMAL, 
    "SENDUSSD: CSendUssdHandler::SendUssdStringL Send length:    %d", 
    aSendMessage.Length() )

    #ifdef ENABLE_SAT_LOGGING

    const TInt count( aSendMessage.Length() );
    for ( TInt i = 0; i < count; i++ )
        {
        LOG2( NORMAL, 
        "SENDUSSD: CSendUssdHandler::SendUssdStringL :       0x%X", 
        (char)aSendMessage[i] )
        }

    #endif

    error = iUssdClient->SendSatMessage(
        aSendMessage,
        aSendDcs,
        aReceiveMessage,
        aSendCompletedFirst,
        aReceivedDcs );

    LOG2( NORMAL, 
    "SENDUSSD: CSendUssdHandler::SendUssdStringL error:          %d", error )
    LOG2( NORMAL, 
    "SENDUSSD: CSendUssdHandler::SendUssdStringL aReceivedDcs:   %x", 
    aReceivedDcs )
    LOG2( NORMAL, 
    "SENDUSSD: CSendUssdHandler::SendUssdStringL Received length:%d", 
    aReceiveMessage.Length() )
    LOG2( NORMAL, 
    "SENDUSSD: CSendUssdHandler::SendUssdStringL Completed first:%i", 
    aSendCompletedFirst )

    // Convert received Dcs
    ConvertReceivedDcs( aReceivedDcs );

    User::LeaveIfError( error );

    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::SendUssdStringL exiting" )
    }
    
// -----------------------------------------------------------------------------
// Show the ussd response note.
// -----------------------------------------------------------------------------
//
void CSendUssdHandler::ShowUssdResponseNoteL( const TDesC& aText )
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::ShowUssdResponseNoteL calling" )
    CHbDeviceMessageBoxSymbian::InformationL( aText );

    CHbDeviceMessageBoxSymbian* messageNote = CHbDeviceMessageBoxSymbian::NewL();
    CleanupStack::PushL( messageNote );
    messageNote->SetButton( CHbDeviceMessageBoxSymbian::ERejectButton, EFalse );
    CleanupStack::PopAndDestroy(); // messageNote

    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::ShowUssdResponseNoteL exiting" )
    }
    
// -----------------------------------------------------------------------------
// Handles the result of Ussd sending.
// -----------------------------------------------------------------------------
//
void CSendUssdHandler::HandleSendUssdResult( TInt aError )
    {
    LOG2( SIMPLE, "SENDUSSD: CSendUssdHandler::HandleSendUssdResult calling: \
        %i", aError )

    if ( iQueryRsp.iSessionTerminatedByUser ||
         iNotificationRsp.iSessionTerminatedByUser ||
         ( KErrCancel == aError ) ) // End key
        {
        LOG( SIMPLE, 
        "SENDUSSD: CSendUssdHandler::HandleSendUssdResult TerminatedByUser" )
        iSendUssdRsp.iGeneralResult = RSat::KUssdTransactionTerminatedByUser;
        iSendUssdRsp.iInfoType = RSat::KNoAdditionalInfo;
        iSendUssdRsp.iAdditionalInfo.Zero();
        }
    else if ( TSatExtErrorUtils::IsExtendedError( aError ) ) // extended error
        {
        TUint8 addInfo( 0 );
        if ( KErrGsmCCCallRejected == aError )   
           {
           LOG( SIMPLE, 
           "SENDUSSD: CSendUssdHandler::HandleSendUssdResult permanent error" )	
           // ussd request is rejected by SIM
           iSendUssdRsp.iGeneralResult = RSat::KInteractionWithCCPermanentError;
           iSendUssdRsp.iInfoType = RSat::KMeProblem;
           addInfo = RSat::KActionNotAllowed;
           }   
        // Check and map network failure
        else if ( TSatExtErrorUtils::IsNetworkError( aError ) )
            {
            LOG( SIMPLE, 
            "SENDUSSD: CSendUssdHandler::HandleSendUssdResult NetworkError" )
            // Network error, map and modify
            addInfo = TSatExtErrorUtils::MapError( aError );
            iSendUssdRsp.iGeneralResult = RSat::KNetworkUnableToProcessCmd;
            iSendUssdRsp.iInfoType = RSat::KSatNetworkErrorInfo;
            }
        else
            {
            LOG( SIMPLE, 
            "SENDUSSD: CSendUssdHandler::HandleSendUssdResult MeProblem" )
            // Not a network error, don't modify
            addInfo = TSatExtErrorUtils::MapError( aError, EFalse );
            iSendUssdRsp.iGeneralResult = RSat::KUssdReturnError;
            iSendUssdRsp.iInfoType = RSat::KMeProblem;
            }
        // Add additional info to response
        iSendUssdRsp.iAdditionalInfo.SetLength( 1 );
        iSendUssdRsp.iAdditionalInfo[0] = static_cast<TUint16>( addInfo );
        }
    else if ( KErrInUse == aError )
        {
        LOG( SIMPLE, 
        "SENDUSSD: CSendUssdHandler::HandleSendUssdResult KMeBusyOnUssd" )
        iSendUssdRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        iSendUssdRsp.iInfoType = RSat::KMeProblem;
        iSendUssdRsp.iAdditionalInfo.SetLength( 1 );
        iSendUssdRsp.iAdditionalInfo[0] = RSat::KMeBusyOnUssd;
        }
    else if ( KErrGeneral == aError )
        {
        LOG( SIMPLE, 
        "SENDUSSD: CSendUssdHandler::HandleSendUssdResult KMeBusyOnSs" )
        iSendUssdRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        iSendUssdRsp.iInfoType = RSat::KMeProblem;
        iSendUssdRsp.iAdditionalInfo.SetLength( 1 );
        iSendUssdRsp.iAdditionalInfo[0] = RSat::KMeBusyOnSs;
        }
    else if ( KErrNotFound == aError )
        {
        LOG( SIMPLE, 
        "SENDUSSD: CSendUssdHandler::HandleSendUssdResult \
        KNoSpecificMeProblem" )
        iSendUssdRsp.iGeneralResult = RSat::KUssdReturnError;
        iSendUssdRsp.iInfoType = RSat::KMeProblem;
        iSendUssdRsp.iAdditionalInfo.SetLength( 1 );
        iSendUssdRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
        }
    else if ( KErrSatBusy == aError )
        {
        LOG( SIMPLE, 
        "SENDUSSD: CSendUssdHandler::HandleSendUssdResult \
        KInteractionWithCCTemporaryError" )
        iSendUssdRsp.iGeneralResult = RSat::KInteractionWithCCTemporaryError;
        iSendUssdRsp.iInfoType = RSat::KNoAdditionalInfo;
        iSendUssdRsp.iAdditionalInfo.SetLength( 0 );
        iSendUssdRsp.iAdditionalInfo.Zero();
        }
    else if ( KErrSatControl == aError )
        {
        LOG( SIMPLE, 
        "SENDUSSD: CSendUssdHandler::HandleSendUssdResult \
        KModifiedByCallControl" )
        iSendUssdRsp.iGeneralResult = RSat::KModifiedByCallControl;
        iSendUssdRsp.iInfoType = RSat::KNoAdditionalInfo;
        iSendUssdRsp.iAdditionalInfo.SetLength( 0 );
        iSendUssdRsp.iAdditionalInfo.Zero();
		}
    else if ( KErrNone == aError )   //  Success case
        {
        LOG( SIMPLE, 
        "SENDUSSD: CSendUssdHandler::HandleSendUssdResult success" )
        // Convert terminal rsp if icon used
        iSendUssdRsp.iGeneralResult = RSat::KSuccess;

        // If command had icon data and was done succesfully,
        // report that icon was not shown.
        // To be removed and correct handling (i.e. ClientResponse to
        // notification is received) for general result
        // KSuccessRequestedIconNotDisplayed must be added when icons
        // are allowed in this command 
        if ( iIconCommand )
            {
            LOG( SIMPLE, 
            "SENDUSSD: CSendUssdHandler::HandleSendUssdResult \
            IconNotDisplayed" )
            iSendUssdRsp.iGeneralResult =
                        RSat::KSuccessRequestedIconNotDisplayed;
            }

        iSendUssdRsp.iInfoType = RSat::KNoAdditionalInfo;
        iSendUssdRsp.iAdditionalInfo.Zero();
        }
    else // unknown error
        {
        LOG( SIMPLE, 
        "SENDUSSD: CSendUssdHandler::HandleSendUssdResult unknown error" )
        iSendUssdRsp.iGeneralResult = RSat::KUssdReturnError;
        iSendUssdRsp.iInfoType = RSat::KNoAdditionalInfo;
        iSendUssdRsp.iAdditionalInfo.Zero();
        }

    SendTerminalResponse();
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::HandleSendUssdResult exiting" )
    }


// -----------------------------------------------------------------------------
// Converts reveived Dcs to correct format.
// -----------------------------------------------------------------------------
//
void CSendUssdHandler::ConvertReceivedDcs( TUint8& aReceivedDcs ) const
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::ConvertReceivedDcs calling" )

    switch ( aReceivedDcs )
        {
        case KPhCltDcs7Bit:
            {
            LOG( NORMAL, 
            "SENDUSSD: CSendUssdHandler::ConvertReceivedDcs Dcs7Bit" )
            aReceivedDcs = KSatDcs7Bit;
            }
            break;

        case KPhCltDcs8Bit:
            {
            LOG( NORMAL, 
            "SENDUSSD: CSendUssdHandler::ConvertReceivedDcs Dcs8Bit" )
            aReceivedDcs = KSatDcs8Bit;
            }
            break;

        case KPhCltDcsUcs2:
            {
            LOG( NORMAL, 
            "SENDUSSD: CSendUssdHandler::ConvertReceivedDcs DcsUCS2" )
            aReceivedDcs = KSatDcsUCS2;
            }
            break;

        case KPhCltDcsUnknown:
            {
            LOG( NORMAL, 
            "SENDUSSD: CSendUssdHandler::ConvertReceivedDcs DcsUnknown" )
            aReceivedDcs = KSatDcsUnknown;
            }
            break;

        case KSatDcs7Bit:
        case KSatDcs8Bit:
        case KSatDcsUCS2:
        case KSatDcsUnknown:
            {
            LOG( NORMAL, 
            "SENDUSSD: CSendUssdHandler::ConvertReceivedDcs Already valid" )
            }
            break;

        default:
            {
            LOG( NORMAL, 
            "SENDUSSD: CSendUssdHandler::ConvertReceivedDcs Unknown" )
            aReceivedDcs = KSatDcsUnknown;
            }
            break;
        }

    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::ConvertReceivedDcs exiting" )
    }
    
// -----------------------------------------------------------------------------
// Sends terminal response. Makes sure that terminal response
// is not send more that once / command.
// -----------------------------------------------------------------------------
//
void CSendUssdHandler::SendTerminalResponse()
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::SendTerminalResponse calling" )

    if ( !iTerminalRespSent )
        {
        LOG( SIMPLE, 
        "SENDUSSD: CSendUssdHandler::SendTerminalResponse iTerminalRespSent \
        false" )
        
        LOG3(SIMPLE, "SENDUSSD: CSendUssdHandler::SendTerminalResponse \
            iDcs=%d,iUssdString=%s", iSendUssdRsp.iUssdString.iDcs,
            &iSendUssdRsp.iUssdString.iUssdString);
        
        iTerminalRespSent = ETrue;
        iSendUssdRsp.SetPCmdNumber( iSendUssdData.PCmdNumber() );
        TerminalRsp( RSat::ESendUssd, iSendUssdRspPckg );
        }

    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::SendTerminalResponse exiting" )
    }


// -----------------------------------------------------------------------------
// USSD sending should be transparent if alpha identifier is provided but it's
// length is 0. Also user query setting is not on.
// -----------------------------------------------------------------------------
//
TBool CSendUssdHandler::TransparentUssdSending()
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::TransparentUssdSending calling" )
    TBool result( EFalse );
    const RSat::TAlphaId alphaId( iSendUssdData.iAlphaId );

    // Store to member variable for later use
    iQueryOn = iUtils->SystemState().IsConfirmSatOperationsOn();

    if ( ( alphaId.iStatus == RSat::EAlphaIdProvided &&
           alphaId.iAlphaId.Length() == 0 ) ||
           alphaId.iStatus == RSat::EAlphaIdNull )
        {
        LOG( SIMPLE, 
        "SENDUSSD: CSendUssdHandler::TransparentUssdSending AlphaId" )
        if ( !iQueryOn )
            {
            LOG( SIMPLE, 
            "SENDUSSD: CSendUssdHandler::TransparentUssdSending iQueryOn \
            false" )
            result = ETrue;
            }
        }

    LOG2( SIMPLE, 
    "SENDUSSD: CSendUssdHandler::TransparentUssdSending exiting: %i", result )
    return result;
    }

// -----------------------------------------------------------------------------
// Check whether the ussd sending result is displayed
// -----------------------------------------------------------------------------
//
TInt CSendUssdHandler::SatDisplayUssdResultL()
    {
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::SatDisplayUssdResultL calling" )
    TInt result( KErrNone );
    TInt ussdDisplayResult( KErrNone );

    CRepository* repository = NULL;
    repository = CRepository::NewL( KCRUidSatServer );
    
    result = repository->Get( KSatDisplayUssdResult, ussdDisplayResult );
    if ( result != KErrNone )
        {
        LOG2( NORMAL,
        "SENDUSSD: CSendUssdHandler::CSendUssdHandler "
        "get CRepository key failed. err=%d", result )
        }

    delete repository;
    repository = NULL;
    
    LOG( SIMPLE, "SENDUSSD: CSendUssdHandler::SatDisplayUssdResultL exiting" )
    return ussdDisplayResult;
    }

