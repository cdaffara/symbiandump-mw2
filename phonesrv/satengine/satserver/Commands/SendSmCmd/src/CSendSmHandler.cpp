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
* Description:  Handles SendSm command
*
*/


#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatSystemState.h"
#include    "MSatUiSession.h"
#include    "SatSTypes.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CSatSSendMessageNoLoggingHandler.h"
#include    "CSendSmHandler.h"
#include    "SatLog.h"
#include    "gsmerror.h"
#include    <exterror.h>
#include    "TSatExtErrorUtils.h"

const TUint8 KPlus = 0x2b; // '+' character

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSendSmHandler* CSendSmHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "SENDSM: CSendSmHandler::NewL calling" )

    CSendSmHandler* self = new( ELeave ) CSendSmHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SENDSM: CSendSmHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CSendSmHandler::~CSendSmHandler()
    {
    LOG( SIMPLE, "SENDSM: CSendSmHandler::~CSendSmHandler calling" )

    delete iMsgSender;
    Cancel();

    LOG( SIMPLE, "SENDSM: CSendSmHandler::~CSendSmHandler exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatCommand.
// Query response.
// -----------------------------------------------------------------------------
//
void CSendSmHandler::ClientResponse()
    {
    LOG( SIMPLE, "SENDSM: CSendSmHandler::ClientResponse calling" )

    // Query response from client.
    if ( iQueryRsp.iAccepted && !iNotificationSent )
        {
        LOG( SIMPLE, "SENDSM: CSendSmHandler::ClientResponse Query response" )
        // Send notification to UI
        // Register notification observer
        TRAP_IGNORE( iUtils->RegisterServiceRequestL(
            ESatSProactiveNotification,
            ESatSProactiveNotificationResponse,
            this ) )

        // Get UISession.
        MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();

        // Send command to SatClient.
        uiSession->SendCommand( &iNotificationDataPckg,
            &iNotificationRspPckg, ESatSProactiveNotification );

        iNotificationSent = ETrue;
        }
    else if ( iNotificationRsp.iAccepted && iNotificationSent )
        {
        LOG( SIMPLE, "SENDSM: CSendSmHandler::ClientResponse get response" )
        // Then we can proceed with the msg sending
        if ( !iMsgSender->IsActive() )
            {
            LOG( SIMPLE, 
            "SENDSM: CSendSmHandler::ClientResponse start iMsgSender" )
            iMsgSender->Start( iSendSmData );
            }
        }
    else
        {
        if ( iQueryRsp.iSessionTerminatedByUser )
            {
            LOG( SIMPLE, 
            "SENDSM: CSendSmHandler::ClientResponse close ui session" )
            // Next SimSession end will close the ui session
            iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
            }

        iSendSmRsp.iGeneralResult = RSat::KPCmdNotAcceptedByUser;
        iSendSmRsp.iInfoType = RSat::KNoAdditionalInfo;
        iSendSmRsp.iAdditionalInfo.Zero();
        iSendSmRsp.SetPCmdNumber( iSendSmData.PCmdNumber() );

        TerminalRsp( RSat::ESendSm, iSendSmRspPckg );
        }

    LOG( SIMPLE, "SENDSM: CSendSmHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatEventObserver.
// Event notification.
// -----------------------------------------------------------------------------
//
void CSendSmHandler::Event( TInt aEvent )
    {
    LOG( SIMPLE, "SENDSM: CSendSmHandler::Event calling" )
    LOG2( SIMPLE, "SENDSM: CSendSmHandler::Event iWaitingUiLaunch=%d",  
    iWaitingUiLaunch ) 
    LOG2( SIMPLE, "SENDSM: CSendSmHandler::Event IsActive=%d", IsActive() ) 
    if ( MSatUtils::EMoSmControlExecuting == aEvent )
        {
        LOG( NORMAL, "SENDSM:   Event EMoSmControlExecuting" )
        iMoSmControlActive = ETrue;
        }
    else if ( MSatUtils::EMoSmControlDone == aEvent )
        {
        LOG( NORMAL, "SENDSM:   Event EMoSmControlDone" )
        iMoSmControlActive = EFalse;
        // Check if Sendsm is waiting.
        // Completing call control should not trigger this command handler 
        // if it is still waiting for UI to be launched. 
        if ( !IsActive() && !iWaitingUiLaunch ) 
            {
            LOG( SIMPLE, "SENDSM: CSendSmHandler::Event sendsm" )
            // Do the Sendsm.
            HandleCommand();
            }
        }

    CSatCommandHandler::Event( aEvent );

    LOG( SIMPLE, "SENDSM: CSendSmHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatEventObserver.
// Notification that sms was sent.
// -----------------------------------------------------------------------------
//
void CSendSmHandler::SmsSent( TInt aErrorCode )
    {
    LOG2( SIMPLE, "SENDSM: CSendSmHandler::SmsSent calling, &d", aErrorCode )

    // Ui session is listening for sms sent event.
    iUtils->NotifyUiEvent( ESatSSmEndEvent, ESatEventNone, aErrorCode );

    iSendSmRsp.iInfoType = RSat::KNoAdditionalInfo;
    iSendSmRsp.iAdditionalInfo.Zero();
    iSendSmRsp.SetPCmdNumber( iSendSmData.PCmdNumber() );

    if ( KErrSatMoSmControlModified == aErrorCode )
        {
        LOG( SIMPLE, "SENDSM:   KErrSatMoSmControlModified" )
        iSendSmRsp.iGeneralResult = RSat::KSuccess;
        }
    else if ( KErrSatMoSmControlBarred == aErrorCode )
        {
        LOG( SIMPLE, "SENDSM:   KErrSatMoSmControlBarred" )
        iSendSmRsp.iGeneralResult = RSat::KInteractionWithCCPermanentError;

        // Additional info field contains extra info.
        iSendSmRsp.iInfoType = RSat::KControlInteraction;

        // Lower byte contains the SMS error cause.
        iSendSmRsp.iAdditionalInfo.SetLength( 1 );
        iSendSmRsp.iAdditionalInfo[0] =
            static_cast<TUint16>( RSat::KActionNotAllowed );
        }
    else if ( TSatExtErrorUtils::IsExtendedError( aErrorCode ) )
        {
        LOG( SIMPLE, "SENDSM:   Error code < KErrGsmSmsBase" )

        // Map error value
        TUint8 addInfo( 0 );

        if ( TSatExtErrorUtils::IsNetworkError( aErrorCode ) )
            {
            LOG( SIMPLE, "SENDSM:   NetworkError" )
            // Map extended error
            addInfo = TSatExtErrorUtils::MapError( aErrorCode );

            // Error utils found the correct response
            iSendSmRsp.iGeneralResult = RSat::KNetworkUnableToProcessCmd;
            // Additional info field contains extra info.
            iSendSmRsp.iInfoType = RSat::KSatNetworkErrorInfo;
            }
        else
            {
            LOG( SIMPLE, "SENDSM:   MeError" )
            // No need to modify mapped value
            addInfo = TSatExtErrorUtils::MapError( aErrorCode, EFalse );

            // Return GSM Error specific value.
            iSendSmRsp.iGeneralResult = RSat::KSmsRpError;
            // Additional info field contains extra info.
            iSendSmRsp.iInfoType = RSat::KMeProblem;
            }

        // Add additional info into terminal response
        iSendSmRsp.iAdditionalInfo.SetLength( 1 );
        iSendSmRsp.iAdditionalInfo[0] = static_cast<TUint16>( addInfo );
        }
    else if ( ( KErrGsmSMSNoNetworkService == aErrorCode ) ||
              ( KErrGsmSSNotAvailable == aErrorCode ) )
        {
        LOG( SIMPLE, "SENDSM:     UnableToProcessCmd" )
        iSendSmRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        iSendSmRsp.iInfoType = RSat::KMeProblem;
        iSendSmRsp.iAdditionalInfo.SetLength( 1 );
        iSendSmRsp.iAdditionalInfo[0] = RSat::KNoService;
        }
    else if ( KErrGeneral == aErrorCode )
        {
        LOG( SIMPLE, "SENDSM:   KErrGeneral" )
        iSendSmRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        }
    else if ( KErrTimedOut == aErrorCode )
        {
        LOG( SIMPLE, "SENDSM:   KErrTimedOut" )
        iSendSmRsp.iGeneralResult = RSat::KNetworkUnableToProcessCmd;
        }
    else
        {
        if ( !iPartialComprehension )
            {
            if ( !iNotificationRsp.iRequestedIconDisplayed &&  // No icon displayed
                ( RSat::ESelfExplanatory ==             // Only icon
                    iSendSmData.iIconId.iQualifier ||
                  RSat::ENotSelfExplanatory ==          // Icon and text
                    iSendSmData.iIconId.iQualifier ) )
                {
                LOG( SIMPLE, "SENDSM:   KSuccessRequestedIconNotDisplayed" )
                iSendSmRsp.iGeneralResult =
                    RSat::KSuccessRequestedIconNotDisplayed;
                }
            else
                {
                LOG( SIMPLE, "SENDSM:   Succeed" )
                iSendSmRsp.iGeneralResult = RSat::KSuccess;
                }
            }
        else
            {
            LOG( SIMPLE, "SENDSM:   KPartialComprehension" )
            iPartialComprehension = EFalse;
            iSendSmRsp.iGeneralResult = RSat::KPartialComprehension;
            }
        }

    // If command had icon data and was done succesfully, report that icon
    // was not shown
    // To be removed when icons are allowed in this command
    if ( ( RSat::KSuccess == iSendSmRsp.iGeneralResult ) &&
        iIconCommand )
        {
        LOG( SIMPLE, 
        "SENDSM: CSendSmHandler::SmsSent KSuccessRequestedIconNotDisplayed" )
        iSendSmRsp.iGeneralResult =
                    RSat::KSuccessRequestedIconNotDisplayed;
        }

    TerminalRsp( RSat::ESendSm, iSendSmRspPckg );

    LOG( SIMPLE, "SENDSM: CSendSmHandler::SmsSent exiting" )
    }

// -----------------------------------------------------------------------------
// Sets the sca number and also sets the numbering plan and the type of number.
// -----------------------------------------------------------------------------
//
TBool CSendSmHandler::SetScaNumber( const RSat::TSatTelNumber& aScaNumber )
    {
    LOG2( SIMPLE,
        "SENDSM: CSendSmHandler::SetScaNumber calling, aScaNumber :%S",
        &aScaNumber )

    TBool scaAvailable ( EFalse );

    iSendSmData.iAddress.iTelNumber = aScaNumber;
    if ( aScaNumber.Length() > 0 )
        {
        LOG2( SIMPLE, "SENDSM:   aScaNumber.Length: %d", aScaNumber.Length() )
        const RSat::TAddress address = iSendSmData.iAddress;

        scaAvailable = ETrue;

        // Is the number in international format, ie first character is '+'.
        if ( KPlus == address.iTelNumber[0] )
            {
            const RSat::TTypeOfNumber ton = address.iTypeOfNumber;

            // Set the type of number, if it is not known.
            if ( ( RSat::ETypeOfNumberNotSet == ton ) ||
                 ( RSat::EUnknownNumber == ton ) )
                {
                LOG( SIMPLE, "SENDSM:   International Number" )
                iSendSmData.iAddress.iTypeOfNumber =
                    RSat::EInternationalNumber;
                }

            const RSat::TNumberingPlan npi = address.iNumberPlan;

            // Set the numbering plan if it is not known.
            if ( ( RSat::ENumberingPlanNotSet == npi ) ||
                 ( RSat::EUnknownNumberingPlan == npi ) )
                {
                LOG( SIMPLE, "SENDSM:   Isdn Number Plan" )
                iSendSmData.iAddress.iNumberPlan = RSat::EIsdnNumberPlan;
                }
            }
        }

    LOG2( SIMPLE,
        "SENDSM: CSendSmHandler::SetScaNumber exiting, scaAvailable:%i",
        scaAvailable )
    return scaAvailable;
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CSendSmHandler::DoCancel()
    {
    LOG( SIMPLE, "SENDSM: CSendSmHandler::DoCancel calling" )

    iUtils->USatAPI().NotifySendSmCancel();

    LOG( SIMPLE, "SENDSM: CSendSmHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Requests the command notification.
// -----------------------------------------------------------------------------
//
void CSendSmHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "SENDSM: CSendSmHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iSendSmData) RSat::TSendSmV1();
    iNeedUiSession = ETrue;
    iQueryRsp.iAccepted = EFalse; // default
    iNotificationRsp.iAccepted = EFalse;

    iUtils->USatAPI().NotifySendSm( aStatus, iSendSmPckg );

    LOG( SIMPLE, "SENDSM: CSendSmHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// SendSm command is not allowed in following situations:
//   - Phone is not registered to homenetwork and roaming.
// -----------------------------------------------------------------------------
//
TBool CSendSmHandler::CommandAllowed()
    {
    LOG( SIMPLE, "SENDSM: CSendSmHandler::CommandAllowed calling" )
    TBool commandAllowed( ETrue );

    RMobilePhone::TMobilePhoneRegistrationStatus registrationStatus(
        iUtils->SystemState().GetNetworkRegistrationStatus() );

    // Check the PC Suite backup / restore status. If it is ongoing, SendSm is
    // not allowed.
    const TBool backupOngoing(
        iUtils->SystemState().IsBackupProcessOngoing() );

    if ( ( RSat::EAlphaIdProvided != iSendSmData.iAlphaId.iStatus ) &&
        ( RSat::ESelfExplanatory == iSendSmData.iIconId.iQualifier ||
          RSat::ENotSelfExplanatory == iSendSmData.iIconId.iQualifier ) )
        {
        commandAllowed = EFalse;
        iSendSmRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        iSendSmRsp.iInfoType = RSat::KNoAdditionalInfo;
        iSendSmRsp.iAdditionalInfo.Zero();
        LOG( SIMPLE, "SENDSM:   Not allowed data not understood" )
        }
    else if ( ( RMobilePhone::ERegisteredOnHomeNetwork !=
        registrationStatus ) &&
        ( RMobilePhone::ERegisteredRoaming != registrationStatus ) ||
        backupOngoing )
        {
        commandAllowed = EFalse;
        iSendSmRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        iSendSmRsp.iInfoType = RSat::KMeProblem;
        iSendSmRsp.iAdditionalInfo.SetLength( 1 );
        iSendSmRsp.iAdditionalInfo[0] = RSat::KNoService;
        LOG( SIMPLE, "SENDSM:   Not allowed Me unable to process" )
        }
    else if ( !IsSCAAvailable() )
        {
        LOG( SIMPLE, "SENDSM:   Not allowed !IsSCAAvailable()" )
        commandAllowed = EFalse;
        iSendSmRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        iSendSmRsp.iInfoType = RSat::KMeProblem;
        iSendSmRsp.iAdditionalInfo.SetLength( 1 );
        iSendSmRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
        }
    // Set icon command flag whether icon data was received and set qualifier
    // to no icon id
    // To be removed when icons are allowed in this command
    else if ( ( RSat::ESelfExplanatory ==
        iSendSmData.iIconId.iQualifier ) ||
        ( RSat::ENotSelfExplanatory ==
        iSendSmData.iIconId.iQualifier ) )
        {
        LOG( SIMPLE, "SENDSM:   ENoIconId" )
        iIconCommand = ETrue;
        iSendSmData.iIconId.iQualifier = RSat::ENoIconId;
        }
    else
        {
        LOG( SIMPLE, "SENDSM:   others" )
        iIconCommand = EFalse;
        }

    //lint -e{961} else block not needed.
    if ( !commandAllowed )
        {
        iSendSmRsp.SetPCmdNumber( iSendSmData.PCmdNumber() );
        TerminalRsp( RSat::ESendSm, iSendSmRspPckg );
        }

    LOG2( SIMPLE, "SENDSM: CSendSmHandler::CommandAllowed exiting,\
          commandAllowed: %d", commandAllowed )
    return commandAllowed;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Need for ui session.
// -----------------------------------------------------------------------------
//
TBool CSendSmHandler::NeedUiSession()
    {
    LOG( NORMAL, "SENDSM: CSendSmHandler::NeedUiSession calling" )
    // Check do we need UI

    // Store the result for later use
    iNeedUiSession = !TransparentSmsSending();
    // Notify Cover UI if it's supported
    if ( iNeedUiSession && iUtils->CoverUiSupported() )
        {
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::ESendSm;
        medEventData.iAlphaId =  iSendSmData.iAlphaId;
        if ( iUtils->SystemState().IsConfirmSatOperationsOn() )
            {
            LOG( NORMAL, 
            "SENDSM: CSendSmHandler::NeedUiSession KSatLongDuration" )
            medEventData.iDuration.iNumOfUnits = KSatLongDuration;
            }
        else
            {
            LOG( NORMAL, 
            "SENDSM: CSendSmHandler::NeedUiSession KSatDefaultDuration" )
            medEventData.iDuration.iNumOfUnits = KSatDefaultDuration;
            }

        medEventData.iDuration.iTimeUnit = RSat::ESeconds;
        medEventData.iIconID = iSendSmData.iIconId;
        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        }

    LOG2( NORMAL, "SENDSM: CSendSmHandler::NeedUiSession exiting, \
          iNeedUiSession: %d", iNeedUiSession )
    return iNeedUiSession;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when USAT API notifies that command.
// -----------------------------------------------------------------------------
//
void CSendSmHandler::HandleCommand()
    {
    LOG( SIMPLE, "SENDSM: CSendSmHandler::HandleCommand calling" )

    // If mo sm control is active, command waits for the EMoSmControlDone event
    // and sends the command then.
    if ( !iMoSmControlActive )
        {
        LOG2( SIMPLE,  
        "SENDSM: CSendSmHandler::HandleCommand: !iMoSmControlActive, \
        iNeedUiSession=%d",
        iNeedUiSession ) 

        iUtils->NotifyEvent( MSatUtils::ESendSmExecuting );

        if ( !iNeedUiSession )
            {
            LOG( SIMPLE, 
            "SENDSM: CSendSmHandler::HandleCommand not iNeedUiSession" )
            // Dont show any messages in ui client, ETSI dictates that
            // if alpha is is provided and alpha id length is 0 then ui
            // should not show anyting.
            // This is OK also for icon support.
            // Icon is successfully processed. But not shown in this case.
            if ( !iMsgSender->IsActive() )
                {
                LOG( SIMPLE, 
                "SENDSM: CSendSmHandler::HandleCommand start iMsgSender" )
                iMsgSender->Start( iSendSmData );
                }
            }
        else
            {
            iQueryData.iCommand = ESatSSendSmQuery;
            iQueryData.iQueryText = iSendSmData.iAlphaId.iAlphaId;
            iQueryData.iIconId.iIdentifier = iSendSmData.iIconId.iIdentifier;

            iNotificationSent = EFalse;
            iNotificationData.iCommand = ESatSSendSmsNotify;
            iNotificationData.iText = iSendSmData.iAlphaId.iAlphaId;
            iNotificationData.iIconId.iIdentifier =
                iSendSmData.iIconId.iIdentifier;
            LOG2( NORMAL, "SENDSM: CSendSmHandler::HandleCommand \
                  iSendSmData.iIconId.iQualifier: %d", 
                  iSendSmData.iIconId.iQualifier )
            switch ( iSendSmData.iIconId.iQualifier )
                {
                case RSat::ESelfExplanatory:
                    {
                    // Icon qualifier is self explanatory (to display instead
                    // of the alpha id or text string).
                    iQueryData.iIconId.iIconQualifier = ESatSelfExplanatory;
                    iNotificationData.iIconId.iIconQualifier =
                        ESatSelfExplanatory;
                    break;
                    }

                case RSat::ENotSelfExplanatory:
                    {
                    // Icon qualifier is not self explanatory.
                    iQueryData.iIconId.iIconQualifier = ESatNotSelfExplanatory;
                    iNotificationData.iIconId.iIconQualifier =
                        ESatNotSelfExplanatory;
                    break;
                    }

                default:
                    {
                    // Icon qualifier not present
                    iQueryData.iIconId.iIconQualifier = ESatENoIconId;
                    iNotificationData.iIconId.iIconQualifier = ESatENoIconId;
                    break;
                    }

                }
            LOG2( NORMAL, "SENDSM: CSendSmHandler::HandleCommand \
                  iSendSmData.iAlphaId.iStatus: %d", 
                  iSendSmData.iAlphaId.iStatus )
            switch ( iSendSmData.iAlphaId.iStatus )
                {
                case RSat::EAlphaIdNotPresent:
                    {
                    iQueryData.iAlphaIdStatus = ESatAlphaIdNotProvided;
                    iNotificationData.iAlphaIdStatus = ESatAlphaIdNotProvided;
                    break;
                    }

                case RSat::EAlphaIdProvided:
                    {
                    iQueryData.iAlphaIdStatus = ESatAlphaIdNotNull;
                    iNotificationData.iAlphaIdStatus = ESatAlphaIdNotNull;
                    break;
                    }

                default:
                    {
                    iQueryData.iAlphaIdStatus = ESatAlphaIdNull;
                    iNotificationData.iAlphaIdStatus = ESatAlphaIdNull;
                    break;
                    }
                }

            // Check do we need to confirm operation from user
            if ( !iUtils->SystemState().IsConfirmSatOperationsOn() )
                {
                LOG( SIMPLE, 
                "SENDSM: CSendSmHandler::HandleCommand confirm operation" )
                // No need to confirm, show only notification
                // Register notification observer
                TRAP_IGNORE( iUtils->RegisterServiceRequestL(
                    ESatSProactiveNotification,
                    ESatSProactiveNotificationResponse,
                    this ) )

                LOG( SIMPLE, 
                    "SENDSM: CSendSmHandler::HandleCommand send ui notification" )
                iUtils->SatUiHandler().UiSession()->SendCommand(
                    &iNotificationDataPckg,
                    &iNotificationRspPckg,
                    ESatSProactiveNotification );

                iNotificationSent = ETrue;
                }
            else
                {
                LOG( SIMPLE, 
                "SENDSM: CSendSmHandler::HandleCommand user confirmation needed" ) 
                // Register service request handler for SendSm command,
                // If there is already service request for query, registering
                // updates command handler, so client responses comes to this
                // command handler
                TRAP_IGNORE( iUtils->RegisterServiceRequestL(
                    ESatSProactiveQuery,
                    ESatSProactiveQueryResponse,
                    this ) )

                iNotificationSent = EFalse;

                LOG( SIMPLE, 
                "SENDSM: CSendSmHandler::HandleCommand send user confirmation request" ) 

                // Ask the user permission to send sms. Reply will come
                // to ClientResponse method.
                iUtils->SatUiHandler().UiSession()->SendCommand(
                    &iQueryPckg,
                    &iQueryRspPckg,
                    ESatSProactiveQuery );
                }
            }
        }

    LOG( SIMPLE, "SENDSM: CSendSmHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when UI launch fails
// -----------------------------------------------------------------------------
//
void CSendSmHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "SENDSM: CSendSmHandler::UiLaunchFailed calling" )

    iSendSmRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
    iSendSmRsp.iInfoType = RSat::KMeProblem;
    iSendSmRsp.iAdditionalInfo.SetLength( 1 );
    iSendSmRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
    iSendSmRsp.SetPCmdNumber( iSendSmData.PCmdNumber() );

    TerminalRsp( RSat::ESendSm, iSendSmRspPckg );

    LOG( SIMPLE, "SENDSM: CSendSmHandler::UiLaunchFailed exiting" )
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized, harmless.
CSendSmHandler::CSendSmHandler() :
    CSatCommandHandler(),
    iSendSmData(),
    iSendSmPckg( iSendSmData ),
    iSendSmRsp(),
    iSendSmRspPckg( iSendSmRsp ),
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
    LOG( SIMPLE,
        "SENDSM: CSendSmHandler::CSendSmHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSendSmHandler::ConstructL()
    {
    LOG( SIMPLE, "SENDSM: CSendSmHandler::ConstructL calling" )

    iMsgSender = CSatSSendMessageNoLoggingHandler::NewL(
        iUtils->USatAPI(),
        *this );

    iUtils->RegisterL( this, MSatUtils::EMoSmControlExecuting );
    iUtils->RegisterL( this, MSatUtils::EMoSmControlDone );

    // Create request handler. This is same that LaunchBrowser uses, so this
    // is needed also in HandleCommand - function.
    iUtils->RegisterServiceRequestL(
        ESatSProactiveQuery,
        ESatSProactiveQueryResponse,
        this );

    LOG( SIMPLE, "SENDSM: CSendSmHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// SMS sending should be transparent if alpha identifier is provided but it's
// length is 0. Also user query setting is not on.
// -----------------------------------------------------------------------------
//
TBool CSendSmHandler::TransparentSmsSending() const
    {
    LOG( SIMPLE, "SENDSM: CSendSmHandler::TransparentSmsSending calling" )

    TBool result( EFalse );
    const RSat::TAlphaId alphaId( iSendSmData.iAlphaId );

    if ( ( alphaId.iStatus == RSat::EAlphaIdProvided &&
           alphaId.iAlphaId.Length() == 0 ) ||
           alphaId.iStatus == RSat::EAlphaIdNull )
        {
        LOG( SIMPLE, 
        "SENDSM: CSendSmHandler::TransparentSmsSending EAlphaIdProvided" )
        if ( !iUtils->SystemState().IsConfirmSatOperationsOn() )
            {
            LOG( SIMPLE, 
            "SENDSM: CSendSmHandler::TransparentSmsSending confirm" )
            result = ETrue;
            }

        }

    LOG2( SIMPLE, "SENDSM: CSendSmHandler::TransparentSmsSending exiting: %i",
        result )
    return result;
    }

// -----------------------------------------------------------------------------
// Chaeck SCA availability.
// -----------------------------------------------------------------------------
//
TBool CSendSmHandler::IsSCAAvailable()
    {
    LOG( SIMPLE, "SENDSM: CSendSmHandler::IsSCAAvailable calling" )

    TBool scaAvailable ( ETrue );

    // If Service Center Address in not specified in proactive command,
    // the SCA is taken from shared data. SMSUi sets the SCA in the
    // shared data. If the SCA is not found from shared data, then
    // sending is not allowed.
    if ( 0 == iSendSmData.iAddress.iTelNumber.Length() )
        {
        LOG( SIMPLE, "SENDSM:   TelNumber is empty" )

        // Address is not defined.
        if ( RSat::ETypeOfNumberNotSet == iSendSmData.iAddress.iTypeOfNumber )
            {
            iPartialComprehension = EFalse;
            }

        // Address is set intentionally empty.
        else
            {
            iPartialComprehension = ETrue;
            LOG( SIMPLE, "SENDSM:   The result is KPartialComprehension" )
            }

        scaAvailable = SetScaNumber( iUtils->SystemState().SCANumber() );
        }
    else
        {
        LOG( SIMPLE, "SENDSM:   TelNumber is not empty." )
        iPartialComprehension = EFalse;
        }

    LOG2( SIMPLE,
        "SENDSM: CSendSmHandler::IsSCAAvailable exiting: %i", scaAvailable )
    return scaAvailable;
    }
