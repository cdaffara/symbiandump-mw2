/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles SendSS command
*
*/


#include    <cphonegsmparserbase.h>
#include    <cphonegsmparserresult.h>
#include    <phonegsmparser.h>
#include    <cphonegsmoptioncontainerbase.h>

#include    "MSatSystemState.h"
#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CSendSsHandler.h"
#include    "SatLog.h"
#include    "csatsendssrequestcompletehandler.h"
#include    "csatsendssadditionalinfohandler.h"
#include    "csatsendsshandler.h"

// CustomAPI returns KCustomApiSsGsmActive=0x01
const TInt KSsGsmActiveSuccess = 0x01;
// CustomAPI returns KCustomApiSsGsmRegistered=0x02
const TInt KSsGsmRegistered = 0x02;
// CustomAPI returns KCustomApiSsGsmProvisioned=0x04
const TInt KSsGsmProvisioned = 0x04;
// CustomAPI returns KCustomApiSsGsmQuiescent=0x08
const TInt KSsGsmQuiescent = 0x08;
// CustomAPI returns SsServiceFailedResp=0xFFFF
const TInt KSsServiceFailed = 0xFFFF;
// Error code for NetworkFailure = 0xFFFE
const TInt KSsNetworkError = 0xFFFE;
//interval wait for ussd send
const TInt KCallbackInterval = 1000000;
// Interval wait for additional info
const TInt KSSRequestCallbackInterval = 5000000;
// Ss OperationCode value.
const TInt KSsOperationShowFDNLIst = 0x05;
const TInt KSsOperationPasswordRegistration = 0x06;
// Ss error
const TInt KSsSimAtkCcRejected = 0x13;
const TInt KTwo = 2;

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSendSSHandler* CSendSSHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::NewL calling" )

    CSendSSHandler* self = new( ELeave ) CSendSSHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SENDSS: CSendSSHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CSendSSHandler::~CSendSSHandler()
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::~CSendSSHandler calling" )

    Cancel();
    // Destroy all objects.
    delete iAdditionalInfoHandler;
    delete iRequestCompleteHandler;
    delete iPhoneGsmHandlerBase;
    delete iParser;
    delete iResult;
    delete iPhoneGsmOptionContainerBase;
    if ( iTimer )
        {
        iTimer->Cancel();
        delete iTimer;
        iTimer = NULL;
        }

    LOG( SIMPLE, "SENDSS: CSendSSHandler::~CSendSSHandler exiting" )
    }
    
// -----------------------------------------------------------------------------
// Processes the SS Request Complete.
// -----------------------------------------------------------------------------
//
void CSendSSHandler::DispatchSsRequestComplete( TInt aErrCode )
    {
    LOG2( SIMPLE, "SENDSS: CSendSSHandler::DispatchSsRequestComplete \
    called aErrCode: %i", aErrCode )

    if ( ( KSsGsmActiveSuccess == aErrCode ) ||
         ( KSsGsmRegistered == aErrCode ) ||
         ( KSsGsmProvisioned == aErrCode ) ||
         ( KSsGsmQuiescent == aErrCode ) )
        {
        iSsResult = KErrNone;
        }
    else
        {
        iSsResult = aErrCode;
        }

    // Store result for later use
    iSsResult = aErrCode;
    iRequestCompleteArrived = ETrue;

    //is additional info received
    if ( iAdditionalInfoArrived )
        {
        LOG( SIMPLE, 
        "SENDSS: CSendSSHandler::DispatchSsRequestComplete \
        iAdditionalInfoArrived true" )
        HandleSendSsResult();
        }
    else    //if not 5 second timer to get additional info
        {
        if ( !iTimer )
            {
            LOG( SIMPLE, 
            "CSendSSHandler::DispatchSsRequestComplete iTimer false" )
            TRAPD( err, iTimer = CPeriodic::NewL( CActive::EPriorityStandard ); );
            
            LOG2( NORMAL, "SENDSS: CSendSSHandler::DispatchSsRequestComplete \
            CPeriodic::NewL err: %d ", err )              
            if ( KErrNone == err )
                {
                iTimer->Start( KSSRequestCallbackInterval,
                    KCallbackInterval, TCallBack( SSRequestCallback, this ) );
                }
            }
        }

    LOG( SIMPLE, "CSendSSHandler::DispatchSsRequestComplete exit" )
    }

// -----------------------------------------------------------------------------
// Processes the SS aditional info.
// -----------------------------------------------------------------------------
//
void CSendSSHandler::DispatchSsAdditionalInfo(
    const TDesC& aAdditionalInfo )
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::DispatchSsAdditionalInfo called" )

    iAdditionalInfoArrived = ETrue;
    iAdditionalInfo.Copy( aAdditionalInfo );
    LOG2( SIMPLE, "SENDSS: CSendSSHandler::DispatchSsAdditionalInfo length of\
    aAdditionalInfo: %i", aAdditionalInfo.Length())

    if ( aAdditionalInfo.Length() > 0 )
        {
        // This is special case due tsy will not send request complete.
        if ( ( ( KSsOperationPasswordRegistration == aAdditionalInfo[0] ) ||
               ( KSsOperationShowFDNLIst == aAdditionalInfo[0] ) ) &&
             ( !iRequestCompleteArrived ) )
            {
            LOG( SIMPLE, 
            "SENDSS: CSendSSHandler::DispatchSsAdditionalInfo special case" )
            iSsResult = KErrNone;
            iRequestCompleteArrived = ETrue;
            }
        }

    //is requestcompletenotification reveived
    if ( iRequestCompleteArrived )
        {
        LOG( SIMPLE, 
        "SENDSS: CSendSSHandler::DispatchSsAdditionalInfo \
        iRequestCompleteArrived true" )
        HandleSendSsResult();
        }
    else    //if not 5 second timer to get requestcompletenote
        {
        if ( !iTimer )
            {
            LOG( SIMPLE, 
            "SENDSS: CSendSSHandler::DispatchSsAdditionalInfo iTimer false" )
            TRAPD( err, iTimer = CPeriodic::NewL( CActive::EPriorityStandard ); );
            
            LOG2( NORMAL, "SENDSS: CSendSSHandler::DispatchSsAdditionalInfo \
            CPeriodic::NewL err: %d ", err )            
            if ( KErrNone == err )
                {
                iTimer->Start( KSSRequestCallbackInterval,
                    KCallbackInterval, TCallBack( SSRequestCallback, this ) );
                }
            }
        }

    LOG( SIMPLE, "SENDSS: CSendSSHandler::DispatchSsAdditionalInfo exit" )
    }

// -----------------------------------------------------------------------------
// From class MSatCommand.
// Response from the client.
// -----------------------------------------------------------------------------
//
void CSendSSHandler::ClientResponse()
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::ClientResponse calling" )

    if ( iQueryRsp.iAccepted && !iNotificationSent )
        {
        LOG( NORMAL, 
        "SENDSS: CSendSSHandler::ClientResponse Sending notification" )
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
    else if ( iNotificationRsp.iAccepted && iNotificationSent )
        {
        LOG( NORMAL, 
        "SENDSS: CSendSSHandler::ClientResponse Notification response" )
        }
    else // User reject
        {
        iUserAccepted = EFalse;
        // Cannot return KPCmdNotAcceptedByUser (ETSI 11.14 v8.3.0 p65)
        iSendSsRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        iSendSsRsp.iInfoType = RSat::KMeProblem;
        iSendSsRsp.iAdditionalInfo.SetLength( 1 );
        iSendSsRsp.iAdditionalInfo[0] = RSat::KScreenBusy;

        if ( iQueryRsp.iSessionTerminatedByUser )
            {
            LOG( NORMAL, "SENDSS: CSendSSHandler::ClientResponse \
            ESessionTerminatedByUser" )
            // Notify sim session end command that next sim session end
            // should close the ui session.
            iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
            }

        SendTerminalResponse();
        }

    // Release Wait
    if ( iSendWait.IsStarted() )
        {
        LOG( NORMAL, "SENDSS: CSendSSHandler::ClientResponse stop iSendWait" )
        iSendWait.AsyncStop();
        }

    LOG( SIMPLE, "SENDSS: CSendSSHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Waits for indication of user rejection
// -----------------------------------------------------------------------------
//
void CSendSSHandler::Event( TInt aEvent )
    {
    LOG2( SIMPLE, "SENDSS: CSendSSHandler::Event calling, aEvent:%d", aEvent )

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
            // Check is there a confirmation on screen
            if ( !iQueryOn || iNotificationSent )
                {
                LOG( SIMPLE, 
                "SENDSS: CSendSSHandler::Event iNotificationSent true" )
                // Cancel notification requests
                iRequestCompleteHandler->Cancel();
                iAdditionalInfoHandler->Cancel();

                iSendSsRsp.iGeneralResult = RSat::KUssdTransactionTerminatedByUser;
                iSendSsRsp.iInfoType = RSat::KNoAdditionalInfo;
                iSendSsRsp.iAdditionalInfo.Zero();
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

    LOG( SIMPLE, "SENDSS: CSendSSHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CSendSSHandler::DoCancel()
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::DoCancel calling" )

    iUtils->USatAPI().NotifySendSsCancel();

    LOG( SIMPLE, "SENDSS: CSendSSHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Requests the command notification.
// -----------------------------------------------------------------------------
//
void CSendSSHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iSendSsData) RSat::TSendSsV1();
    iQueryOn = EFalse;
    iQueryRsp.iAccepted = EFalse; // default
    iNotificationRsp.iAccepted = EFalse;
    iSendSsRsp.iGeneralResult = RSat::KPSessionTerminatedByUser; // default
    iSendSsRsp.iInfoType = RSat::KNoAdditionalInfo;
    iSendSsRsp.iAdditionalInfo.Zero();

    iUtils->USatAPI().NotifySendSs( aStatus, iSendSsPckg );

    // Unregister from events
    iUtils->UnregisterEvent( this, MSatUtils::ECommandCancelled );
    iUtils->UnregisterEvent( this, MSatUtils::ECancelledUsingEndKey );

    LOG( SIMPLE, "SENDSS: CSendSSHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Precheck before executing the command.
// -----------------------------------------------------------------------------
//
TBool CSendSSHandler::CommandAllowed()
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::CommandAllowed calling" )

    // Allow this command to send terminal response
    iTerminalRespSent = EFalse;
    RMobilePhone::TMobilePhoneRegistrationStatus registrationStatus(
        iUtils->SystemState().GetNetworkRegistrationStatus() );

    TBool commandAllowed( ETrue );

    // icon without alpha id
    if ( ( RSat::EAlphaIdProvided != iSendSsData.iAlphaId.iStatus ) &&
         ( RSat::ESelfExplanatory == iSendSsData.iIconId.iQualifier ||
           RSat::ENotSelfExplanatory == iSendSsData.iIconId.iQualifier ) )
        {
        iSendSsRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        iSendSsRsp.iInfoType = RSat::KNoAdditionalInfo;
        iSendSsRsp.iAdditionalInfo.Zero();
        commandAllowed = EFalse;
        LOG( SIMPLE,
            "SENDSS: CSendSSHandler::CommandAllowed icon without alpha id" )
        }
    else if ( ( RMobilePhone::ERegisteredOnHomeNetwork != registrationStatus ) &&
        ( RMobilePhone::ERegisteredRoaming != registrationStatus ) )
        {
        iSendSsRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        iSendSsRsp.iInfoType = RSat::KMeProblem;
        iSendSsRsp.iAdditionalInfo.SetLength( 1 );
        iSendSsRsp.iAdditionalInfo[0] = RSat::KNoService;
        commandAllowed = EFalse;
        LOG( SIMPLE,
            "SENDSS: CSendSSHandler::CommandAllowed no service" )
        }
    // Set icon command flag whether icon data was received and set qualifier
    // to no icon id
    // To be removed when icons are allowed in this command
    else if ( ( RSat::ESelfExplanatory ==
        iSendSsData.iIconId.iQualifier ) ||
        ( RSat::ENotSelfExplanatory ==
        iSendSsData.iIconId.iQualifier ) )
        {
        LOG( SIMPLE,
            "SENDSS: CSendSSHandler::CommandAllowed ENoIconId" )
        iIconCommand = ETrue;
        iSendSsData.iIconId.iQualifier = RSat::ENoIconId;
        }
    else
        {
        LOG( SIMPLE,
            "SENDSS: CSendSSHandler::CommandAllowed others" )
        iIconCommand = EFalse;
        }

    if ( !commandAllowed )
        {
        SendTerminalResponse();
        LOG( SIMPLE, "SENDSS: CSendSSHandler::CommandAllowed not allowed" )
        }

    LOG( SIMPLE, "SENDSS: CSendSSHandler::CommandAllowed exiting" )
    return commandAllowed;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Answers for need of UI session.
// -----------------------------------------------------------------------------
//
TBool CSendSSHandler::NeedUiSession()
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::NeedUiSession calling" )

    iNeedUiSession = !TransparentSsSending();

    // Notify Cover UI if it's supported
    if ( iNeedUiSession && iUtils->CoverUiSupported() )
        {
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::ESendSs;
        medEventData.iAlphaId = iSendSsData.iAlphaId;
        if ( iUtils->SystemState().IsConfirmSatOperationsOn() )
            {
            LOG( SIMPLE, 
            "SENDSS: CSendSSHandler::NeedUiSession KSatLongDuration" )
            medEventData.iDuration.iNumOfUnits = KSatLongDuration;
            }
        else
            {
            LOG( SIMPLE, 
            "SENDSS: CSendSSHandler::NeedUiSession KSatDefaultDuration" )
            medEventData.iDuration.iNumOfUnits = KSatDefaultDuration;
            }
        medEventData.iDuration.iTimeUnit = RSat::ESeconds;
        medEventData.iIconID = iSendSsData.iIconId;
        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        }

    LOG2( SIMPLE, "SENDSS: CSendSSHandler::NeedUiSession exiting,\
    iNeedUiSession: %d", iNeedUiSession )
    return iNeedUiSession;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when USAT API notifies that command.
// -----------------------------------------------------------------------------
//
void CSendSSHandler::HandleCommand()
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::HandleCommand calling" )

    iUtils->NotifyEvent( MSatUtils::ESendSsExecuting );

    // This is true, by default
    iUserAccepted = ETrue;

    if ( iNeedUiSession )
        {
        LOG( SIMPLE, 
        "SENDSS: CSendSSHandler::HandleCommand iNeedUiSession true" )
        TRAP_IGNORE(
        // Register to listen user cancel events:
        // Cancel key event from dialog
        iUtils->RegisterL( this, MSatUtils::ECommandCancelled );
        // End key from dialog
        iUtils->RegisterL( this, MSatUtils::ECancelledUsingEndKey ) )

        // Build Qyery and Notify packages
        // Has to be casted to TInt before casting to TSatIconQualifier, because
        // GCC warns about the direct cast.
        const struct TSatIconId iconId = { iSendSsData.iIconId.iIdentifier,
            static_cast<TSatIconQualifier>(
                static_cast<TInt>( iSendSsData.iIconId.iQualifier ) ) };

        if ( RSat::EAlphaIdNotPresent == iSendSsData.iAlphaId.iStatus )
            {
            LOG( SIMPLE, 
            "SENDSS: CSendSSHandler::HandleCommand EAlphaIdNotPresent" )
            iQueryData.iAlphaIdStatus = ESatAlphaIdNotProvided;
            iNotificationData.iAlphaIdStatus = ESatAlphaIdNotProvided;
            }
        else if ( RSat::EAlphaIdProvided == iSendSsData.iAlphaId.iStatus )
            {
            LOG( SIMPLE, 
            "SENDSS: CSendSSHandler::HandleCommand EAlphaIdProvided" )
            iQueryData.iAlphaIdStatus = ESatAlphaIdNotNull;
            iNotificationData.iAlphaIdStatus = ESatAlphaIdNotNull;
            }
        else
            {
            LOG( SIMPLE, 
            "SENDSS: CSendSSHandler::HandleCommand ESatAlphaIdNull" )
            iQueryData.iAlphaIdStatus = ESatAlphaIdNull;
            iNotificationData.iAlphaIdStatus = ESatAlphaIdNull;
            }

        iQueryData.iCommand = ESatSSendSsQuery;
        iQueryData.iQueryText = iSendSsData.iAlphaId.iAlphaId;
        iQueryData.iIconId = iconId;

        iNotificationSent = EFalse;
        iNotificationData.iCommand = ESatSSendSsNotify;
        iNotificationData.iText = iSendSsData.iAlphaId.iAlphaId;
        iNotificationData.iIconId = iconId;

        MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();

        // Send either query or notification
        if ( iQueryOn )
            {
            LOG( NORMAL, 
            "SENDSS: CSendSSHandler::HandleCommand Sending Query" )
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
            "SENDSS: CSendSSHandler::HandleCommand Sending notification" )
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

        if ( !iSendWait.IsStarted() )
            {
            LOG( NORMAL, 
            "SENDSS: CSendSSHandler::HandleCommand start iSendWait" )
            // Start waiting response from the user
            iSendWait.Start();
            }
        }

    if ( iUserAccepted )
        {
        LOG( NORMAL, 
        "SENDSS: CSendSSHandler::HandleCommand iUserAccepted true" )
        // Ready to send Ss string
        TRAPD( err, SendSsStringL() )
        if ( KErrNone != err )
            {
            LOG2( NORMAL, " Ss sending failed: %i", err )
            iSendSsRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iSendSsRsp.iInfoType = RSat::KMeProblem;
            iSendSsRsp.iAdditionalInfo.SetLength( 1 );
            iSendSsRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
            SendTerminalResponse();
            }
        }

    LOG( SIMPLE, "SENDSS: CSendSSHandler::HandleCommand exiting" )
    }
    
// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Indicates the failure of launching ui client
// -----------------------------------------------------------------------------
//
void CSendSSHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::UiLaunchFailed calling" )

    iSendSsRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
    iSendSsRsp.iInfoType = RSat::KMeProblem;
    iSendSsRsp.iAdditionalInfo.SetLength( 1 );
    iSendSsRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
    SendTerminalResponse();

    LOG( SIMPLE, "SENDSS: CSendSSHandler::UiLaunchFailed exiting" )
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized.
CSendSSHandler::CSendSSHandler() :
    CSatCommandHandler(),
    iSendSsData(),
    iSendSsPckg( iSendSsData ),
    iSendSsRsp(),
    iSendSsRspPckg( iSendSsRsp ),
    iQueryData(),
    iQueryPckg( iQueryData ),
    iQueryRsp(),
    iQueryRspPckg( iQueryRsp ),
    iNotificationData(),
    iNotificationDataPckg( iNotificationData ),
    iNotificationRsp(),
    iNotificationRspPckg( iNotificationRsp ),
    // To be removed when icons are allowed in this command
    iIconCommand( EFalse )
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::CSendSSHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSendSSHandler::ConstructL()
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::ConstructL calling" )

    // Create additional info and request complete handlers
    iAdditionalInfoHandler = CSatSendSsAdditionalInfoHandler::NewL(
        *iUtils->CustomApi(), this );
    iRequestCompleteHandler = CSatSendSsRequestCompleteHandler::NewL(
        *iUtils->CustomApi(), this );
    iPhoneGsmHandlerBase = new ( ELeave ) CSatSendSsHandler();

    LOG( SIMPLE, "SENDSS: CSendSSHandler::ConstructL exiting" )
    }
    
// -----------------------------------------------------------------------------
// Handles the Ss string sending.
// -----------------------------------------------------------------------------
//
void CSendSSHandler::SendSsStringL()
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::SendSsString calling" )

    // Set default values
    iSsResult = KErrArgument;
    iRequestCompleteArrived = EFalse;
    iAdditionalInfoArrived = EFalse;

    //Parsing and interpreting
    TBuf<RSat::KStringMaxSize> sendMessage;
    sendMessage.Copy( iSendSsData.iSsString.iSsString );

    // CPhoneGsmOptionContainerBase cannot be deleted before SendSsString 
    // complete, so the object is designed as a member in order that it can be 
    // deleted asychronously.
    iParser = PhoneGsmParser::CreateParserL();
    iResult = PhoneGsmParser::CreateResultL();
    iPhoneGsmOptionContainerBase = PhoneGsmParser::CreateOptionContainerL();

    //Update options
    iPhoneGsmOptionContainerBase->SetOptionStatus( KPhoneOptionSend, ETrue );

    //Parsing and interpreting

    //Parse string
    if ( iParser->ParseL( sendMessage, *iResult, *iPhoneGsmOptionContainerBase ) )
        {
        if ( PhoneGsmParser::DetermineContentType( *iResult ) ==
            PhoneGsmParser::EContentSupplementaryService )
            {

            if ( !iRequestCompleteHandler->IsActive() )
                {
                LOG( SIMPLE, 
                "SENDSS: CSendSSHandler::SendSsString start \
                iRequestCompleteHandler" )
                iRequestCompleteHandler->Start();
                }

            if ( !iAdditionalInfoHandler->IsActive() )
                {
                LOG( SIMPLE, 
                "SENDSS: CSendSSHandler::SendSsString start \
                iAdditionalInfoHandler" )
                iAdditionalInfoHandler->Start();
                }

            // Make a Default Additional Info here, in case additional
            // info field won't be received
            // the field is a dummy
            iAdditionalInfo.SetLength( 1 );
            iAdditionalInfo[0] = 0x00;

            LOG( SIMPLE, 
            "SENDSS: CSendSSHandler::SendSsString Processing Ss" )

            // CPhoneGsmHandlerBase lives in all lifetime of 
            // CSendSSHandler. So, it designed as a member.
            iPhoneGsmHandlerBase->ProcessL( *iResult );
            }
        else
            {
            LOG( SIMPLE, 
            "SENDSS: CSendSSHandler::SendSsString KCmdDataNotUnderstood" )
            iSendSsRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
            iSendSsRsp.iInfoType = RSat::KNoAdditionalInfo;
            iSendSsRsp.iAdditionalInfo.Zero();

            // Send terminal response
            SendTerminalResponse();
            }
        }

    LOG( SIMPLE, "SENDSS: CSendSSHandler::SendSsString exiting" )
    }

// -----------------------------------------------------------------------------
// Handles the result of Ss sending.
// -----------------------------------------------------------------------------
//
void CSendSSHandler::HandleSendSsResult()
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::HandleSendSsResult calling" )
    LOG2( SIMPLE, 
    "SENDSS: CSendSSHandler::HandleSendSsResult iSsResult: %i", iSsResult )
    // Remove progress bar from the screen
    if ( KSsServiceFailed == iSsResult || KSsNetworkError == iSsResult )
        {
        LOG( SIMPLE, 
        "SENDSS: CSendSSHandler::HandleSendSsResult Clearing Progress bar.." )
        iUtils->NotifyUiEvent( ESatSsEndEvent, ESatEventFailure, iSsResult );
        // Send Error notification, but only if Alpha ID was provided
	    if ( ESatAlphaIdNotNull == iNotificationData.iAlphaIdStatus )
	        {
	        LOG( SIMPLE, 
	        "SENDSS: CSendSSHandler::HandleSendSsResult Sending Ss note: \
	        Not Done" )
	        iUtils->NotifyUiEvent( ESatSsErrorEvent, ESatEventFailure, 
	        	iSsResult );
	        }
        }
    else if ( iNotificationSent )
        {
        // Remove the UI dialog
        LOG( SIMPLE, 
        "SENDSS: CSendSSHandler::HandleSendSsResult iNotificationSent true" )
        iUtils->NotifyUiEvent( ESatSsEndEvent, ESatEventCompleteOk, iSsResult );
        }

    switch ( iSsResult )
        {
        case KErrGeneral: // Command not processed
            {
            iSendSsRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
            iSendSsRsp.iInfoType = RSat::KMeProblem;
            iSendSsRsp.iAdditionalInfo.SetLength( 1 );
            iSendSsRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
            break;
            }

        case KErrArgument:
            {
            iSendSsRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
            iSendSsRsp.iInfoType = RSat::KNoAdditionalInfo;
            iSendSsRsp.iAdditionalInfo.Zero();
            break;
            }

        case KSsServiceFailed:
            {
            //This is a special case, TSY sends error value in additional info
            //when SS is rejected by Call control 
            if ( KTwo <= iAdditionalInfo.Length() && 
                KSsSimAtkCcRejected == iAdditionalInfo[1] )
                {
                LOG( SIMPLE, 
                "SENDSS: CSendSSHandler::HandleSendSsResult \
                KInteractionWithCCPermanentError" )
                iSendSsRsp.iGeneralResult = 
                    RSat::KInteractionWithCCPermanentError;
                iSendSsRsp.iAdditionalInfo.SetLength( 1 );
                iSendSsRsp.iAdditionalInfo[0] = RSat::KActionNotAllowed;	
                }
            else
                {
                LOG( SIMPLE, 
                "SENDSS: CSendSSHandler::HandleSendSsResult KSsReturnError" )
                iSendSsRsp.iGeneralResult = RSat::KSsReturnError;
                iSendSsRsp.iAdditionalInfo.Copy( iAdditionalInfo );	
                }
            iSendSsRsp.iInfoType = RSat::KMeProblem;
            break;
            }

        case KSsNetworkError:
            {
            iSendSsRsp.iGeneralResult = RSat::KNetworkUnableToProcessCmd;
            iSendSsRsp.iInfoType = RSat::KSatNetworkErrorInfo;
            iSendSsRsp.iAdditionalInfo.SetLength( 1 );
            if ( iAdditionalInfo.Length() > 1 )
                {
                LOG( SIMPLE, 
                "SENDSS: CSendSSHandler::HandleSendSsResult \
                iAdditionalInfo.Length() > 1" )
                iSendSsRsp.iAdditionalInfo[0] = \
                    ( iAdditionalInfo[1] == RSat::KNoSpecificMeProblem ) \
                    ? iAdditionalInfo[1] : ( iAdditionalInfo[1] | 0x80 );
                }
            else
                {
                iSendSsRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
                }
            break;
            }

        default:
            {
            // Convert terminal rsp if icon used
            iSendSsRsp.iGeneralResult = RSat::KSuccess;

            // If command had icon data and was done succesfully,
            // report that icon was not shown.
            // To be removed and correct handling (i.e. ClientResponse to
            // notification is received) for general result
            // KSuccessRequestedIconNotDisplayed must be added when icons are
            // allowed in this command 
            if ( iIconCommand )
                {
                LOG( SIMPLE, 
                "SENDSS: CSendSSHandler::HandleSendSsResult iIconCommand \
                true" )
                iSendSsRsp.iGeneralResult =
                            RSat::KSuccessRequestedIconNotDisplayed;
                }
            
            if ( iAdditionalInfo.Length() )
                {
                LOG( SIMPLE, 
                "SENDSS: CSendSSHandler::HandleSendSsResult \
                iAdditionalInfo.Length >0" )
                iSendSsRsp.iInfoType = RSat::KSendSsInfo;
                iSendSsRsp.iAdditionalInfo.Copy( iAdditionalInfo );
                }
            else
                {
                iSendSsRsp.iInfoType = RSat::KNoAdditionalInfo;
                iSendSsRsp.iAdditionalInfo.Zero();
                }

            break;
            }
        }

    delete iResult;
    iResult = NULL;

    delete iParser;
    iParser = NULL;

    delete iPhoneGsmOptionContainerBase;
    iPhoneGsmOptionContainerBase = NULL;

    iRequestCompleteHandler->Cancel();
    iAdditionalInfoHandler->Cancel();

    // Send terminal response
    SendTerminalResponse();

    LOG( SIMPLE, "SENDSS: CSendSSHandler::HandleSendSsResult exiting" )
    }
    
// -----------------------------------------------------------------------------
// SS sending should be transparent if alpha identifier is provided but it's
// length is 0. Also user query setting is not on.
// -----------------------------------------------------------------------------
//
TBool CSendSSHandler::TransparentSsSending()
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::TransparentSsSending calling" )

    TBool result( EFalse );
    const RSat::TAlphaId alphaId( iSendSsData.iAlphaId );

    // Store to member variable for later use
    iQueryOn = iUtils->SystemState().IsConfirmSatOperationsOn();

    if ( ( alphaId.iStatus == RSat::EAlphaIdProvided &&
           alphaId.iAlphaId.Length() == 0 ) ||
           alphaId.iStatus == RSat::EAlphaIdNull )
        {
        LOG( SIMPLE, 
        "SENDSS: CSendSSHandler::TransparentSsSending EAlphaIdNull" )
        if ( !iQueryOn )
            {
            LOG( SIMPLE, 
            "SENDSS: CSendSSHandler::TransparentSsSending iQueryOn false" )
            result = ETrue;
            }
        }

    LOG2( SIMPLE, "SENDSS: CSendSSHandler::TransparentSsSending exiting: %i",
        result )
    return result;
    }

// -----------------------------------------------------------------------------
// Sends terminal response, if not yet sent
// -----------------------------------------------------------------------------
//
void CSendSSHandler::SendTerminalResponse()
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::SendTerminalResponse calling" )

    if ( !iTerminalRespSent )
        {
        LOG( SIMPLE, 
        "SENDSS: CSendSSHandler::SendTerminalResponse iTerminalRespSent false" )
        iTerminalRespSent = ETrue;
        iSendSsRsp.SetPCmdNumber( iSendSsData.PCmdNumber() );
        TerminalRsp( RSat::ESendSs, iSendSsRspPckg );
        }

    // Delete timer in case it is still active
    if ( iTimer )
        {
        LOG( SIMPLE, 
        "SENDSS: CSendSSHandler::SendTerminalResponse iTimer true" )
        iTimer->Cancel();
        delete iTimer;
        iTimer = NULL;
        }

    LOG( SIMPLE, "SENDSS: CSendSSHandler::SendTerminalResponse exiting" )
    }

// -----------------------------------------------------------------------------
// Callback for SendSS additionalinfo waiting timer.
// -----------------------------------------------------------------------------
//
TInt CSendSSHandler::SSRequestCallback( TAny* aPtr )
    {
    LOG( SIMPLE, "SENDSS: CSendSSHandler::SSRequestCallback calling" )

    CSendSSHandler* handler =
        static_cast<CSendSSHandler*>( aPtr );

    if ( handler )
        {
        LOG( SIMPLE, 
        "SENDSS: CSendSSHandler::SSRequestCallback handler true" )
        handler->HandleSendSsResult();

        if ( handler->iTimer )
            {
            LOG( SIMPLE, 
            "SENDSS: CSendSSHandler::SSRequestCallback cancel iTimer" )
            handler->iTimer->Cancel();
            delete handler->iTimer;
            handler->iTimer = NULL;
            }
        }

    LOG( SIMPLE, "SENDSS: CSendSSHandler::SSRequestCallback exiting" )
    return ( KErrNone );
    }

