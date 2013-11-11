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
* Description:  Handles SendData command
*
*/


#include    <e32std.h>
#include    <etelmmerr.h>
#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatSystemState.h"
#include    "MSatBIPUtils.h"
#include    "MSatBIPDataChannel.h"
#include    "MSatApnHandler.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CSendDataHandler.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSendDataHandler* CSendDataHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "SendData: CSendDataHandler::NewL calling" )

    CSendDataHandler* self = new( ELeave ) CSendDataHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SendData: CSendDataHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CSendDataHandler::~CSendDataHandler()
    {
    LOG( SIMPLE, "SendData: CSendDataHandler::~CSendDataHandler calling" )

    Cancel();

    iDataChannel = NULL;

    LOG( SIMPLE, "SendData: CSendDataHandler::~CSendDataHandler exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatCommand.
// Response from the client.
// -----------------------------------------------------------------------------
//
void CSendDataHandler::ClientResponse()
    {
    LOG( SIMPLE, "SendData: CSendDataHandler::ClientResponse calling-exiting" )
    // Don't send terminal response, already sent in HandleCommand
    }

// -----------------------------------------------------------------------------
// From class MSatSendDataObserver.
// Notification of data has been sent.
// -----------------------------------------------------------------------------
//
void CSendDataHandler::DataSentNotification(
    const TInt aError, const TInt aLength )
    {
    LOG( SIMPLE, "SendData: CSendDataHandler::DataSentNotification calling" )

    if ( MSatBIPUtils::ESatBIPSuccess != aError )
        {
        LOG( SIMPLE, "SendData: CSendDataHandler::DataSentNotification Error" )
        iSendDataRspData.iGeneralResult = RSat::KBearerIndepProtocolError;
        }
    else
        {
        iSendDataRspData.iGeneralResult = RSat::KSuccess;
        iFreeBuffSize = aLength;
        }

    SendTerminalResponse( aError );

    LOG( SIMPLE, "SendData: CSendDataHandler::DataSentNotification exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatSendDataObserver.
// Notfication from the connection.
// -----------------------------------------------------------------------------
//
void CSendDataHandler::ConnectionNotification( TInt aError )
    {
    LOG2( SIMPLE, "SENDDATA: CSendDataHandler::ConnectionNotification\
         calling with code %i", aError )

    if ( KErrNone == aError )
        {
        // Connection established, send data
        SendData();
        }
    else
        {

        // Get connection info and remove APN from CommDB, if created during
        // OpenChannel command
        TUint32 apnId( iDataChannel->ConnInfo().iCreatedApnId );
        LOG2( SIMPLE, "SENDDATA: CSendDataHandler::ConnectionNotification \
        apnId: %i", apnId )
        if ( apnId > 0 )
            {
            TRAP_IGNORE( iUtils->BipUtils().ApnHandler().DeleteApnL( apnId ) )
            }

        // Connection failed
        iSendDataRspData.iGeneralResult = RSat::KBearerIndepProtocolError;
        SendTerminalResponse( MSatBIPUtils::ESatBIPChannelClosed );
        }

    LOG( SIMPLE, "SENDDATA: CSendDataHandler::ConnectionNotification exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Waits for indication of user wanting to close the BIP session.
// -----------------------------------------------------------------------------
//
void CSendDataHandler::Event( TInt aEvent )
    {
    LOG2( SIMPLE, "CSendDataHandler::Event calling, aEvent:%d", aEvent )

    switch ( aEvent )
        {
        case MSatUtils::ECommandCancelled:
        case MSatUtils::ECancelledUsingEndKey:
            {
            // Check Channel activation status
            if ( iUtils->BipUtils().IsContextActive() )
                {
                LOG( SIMPLE, "CSendDataHandler::Event IsContextActive" )
                // Next command is rejected
                iUserCancelled = ETrue;
                SendTerminalResponse(); // If command is executing, send.
                }
            break;
            }
        case MSatUtils::EOpenChannelExecuting:
            {
            // New channel, reset user cancel flag.
            iUserCancelled = EFalse;
            break;
            }
        default:
            {
            // Move event to base class
            CSatCommandHandler::Event( aEvent );
            }
        }

    LOG( SIMPLE, "CSendDataHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CSendDataHandler::DoCancel()
    {
    LOG( SIMPLE, "SendData: CSendDataHandler::DoCancel calling" )

    iUtils->USatAPI().NotifySendDataCancel();

    LOG( SIMPLE, "SendData: CSendDataHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Requests the command notification.
// -----------------------------------------------------------------------------
//
void CSendDataHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "SendData: CSendDataHandler::IssueUSATRequest calling" )

    // Empty previously recevied data.
    new (&iSendDataData) RSat::TSendDataV2();

    iUtils->USatAPI().NotifySendData( aStatus, iSendDataPckg );
    iUtils->NotifyEvent( MSatUtils::EBipCommandDoneExecuting );

    LOG( SIMPLE, "SendData: CSendDataHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Precheck before executing the command.
// -----------------------------------------------------------------------------
//
TBool CSendDataHandler::CommandAllowed()
    {
    LOG( SIMPLE, "SendData: CSendDataHandler::CommandAllowed calling" )

    TBool commandAllowed( ETrue );

    if ( iUserCancelled )
        {
        LOG( SIMPLE, "SendData: CSendDataHandler::CommandAllowed \
        UserCancelled" )
        SendTerminalResponse();

        commandAllowed = EFalse;
        }
    else
        {
        // Proceed with the command
        const TInt channelId( iSendDataData.iDestination );

        if ( ( RSat::KChannel1 > channelId || RSat::KChannel7 < channelId ) ||
            // If icon data without alpha id
            ( ( RSat::EAlphaIdProvided !=
                iSendDataData.iAlphaId.iStatus ) &&
            ( ( RSat::ESelfExplanatory ==
                iSendDataData.iIconId.iQualifier ) ||
            ( RSat::ENotSelfExplanatory ==
                iSendDataData.iIconId.iQualifier ) ) ) )
            {
            LOG( NORMAL, "SendData: CSendDataHandler::CommandAllowed Unknown \
                channel identifier or icon without alpha id - Not allowed" )
            // Create the response IPC package
            iSendDataRspData.iGeneralResult = RSat::KCmdDataNotUnderstood;
            iSendDataRspData.iInfoType = RSat::KNoAdditionalInfo;
            iSendDataRspData.iAdditionalInfo.Zero();
            commandAllowed = EFalse;
            }
        else
            {
            RMobilePhone::TMobilePhoneRegistrationStatus registrationStatus(
                iUtils->SystemState().GetNetworkRegistrationStatus() );

            if ( ( RMobilePhone::ERegisteredOnHomeNetwork !=
                    registrationStatus ) &&
                 ( RMobilePhone::ERegisteredRoaming != registrationStatus ) )
                {

                iSendDataRspData.iInfoType = RSat::KMeProblem;
                iSendDataRspData.iAdditionalInfo.SetLength( 1 );
                iSendDataRspData.iAdditionalInfo[0] = RSat::KNoService;
                iSendDataRspData.iGeneralResult = RSat::KMeUnableToProcessCmd;
                commandAllowed = EFalse;
                LOG( NORMAL,
                    "SendData: CSendDataHandler::CommandAllowed Not allowed" )
                }
            // Set icon command flag whether icon data was received and set qualifier
            // to no icon id
            // To be removed when icons are allowed in this command
            else if ( ( RSat::ESelfExplanatory ==
                iSendDataData.iIconId.iQualifier ) ||
                ( RSat::ENotSelfExplanatory ==
                iSendDataData.iIconId.iQualifier ) )
                {
                LOG( NORMAL,
                    "SendData: CSendDataHandler::CommandAllowed ENoIconId" )
                iIconCommand = ETrue;
                iSendDataData.iIconId.iQualifier = RSat::ENoIconId;
                }
            else
                {
                LOG( NORMAL,
                    "SendData: CSendDataHandler::CommandAllowed others" )
                iIconCommand = EFalse;
                }
            }

        if ( !commandAllowed )
            {
            LOG( NORMAL,
            "SendData: CSendDataHandler::CommandAllowed command not allowed" )
            iSendDataRspData.SetPCmdNumber( iSendDataData.PCmdNumber() );
            TerminalRsp( RSat::ESendData, iSendDataRspPckg );
            }
        }

    LOG2( SIMPLE, 
    "SendData: CSendDataHandler::CommandAllowed exiting, commandAllowed: %d", 
    commandAllowed )
    return commandAllowed;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Need for ui session.
// -----------------------------------------------------------------------------
//
TBool CSendDataHandler::NeedUiSession()
    {
    LOG( SIMPLE, "SendData: CSendDataHandler::NeedUiSession calling" )

    // UI is NOT needed in case where AlphaID is provided and it is a NULL
    // If information indication to the user is needed and data is not
    // stored in Tx Buffer, show it now.
    iNeedUiSession = ( !( RSat::EAlphaIdNull ==
                          iSendDataData.iAlphaId.iStatus ) &&
                       !( RSat::EAlphaIdNotPresent ==
                          iSendDataData.iAlphaId.iStatus ) &&
                       !( RSat::EStoreDataInTxBuffer ==
                          iSendDataData.iMode ) );
    // Send notification to Mediator client if Cover UI is supported
    if ( iNeedUiSession && iUtils->CoverUiSupported() )
        {
        LOG( SIMPLE, 
            "SendData: CSendDataHandler::NeedUiSession CoverUiSupported" )
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::ESendData;
        medEventData.iAlphaId = iSendDataData.iAlphaId;
        medEventData.iDuration.iNumOfUnits = KSatDefaultDuration;
        medEventData.iIconID = iSendDataData.iIconId;
        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        }

    LOG2( SIMPLE, 
    "SendData: CSendDataHandler::NeedUiSession exiting,iNeedUiSession: %d",
    iNeedUiSession )
    return iNeedUiSession;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when USAT API notifies that command.
// -----------------------------------------------------------------------------
//
void CSendDataHandler::HandleCommand()
    {
    LOG( SIMPLE, "SendData: CSendDataHandler::HandleCommand calling" )

    // Notify other's about this command executiuon
    iUtils->NotifyEvent( MSatUtils::ESendDataExecuting );
    iUtils->NotifyEvent( MSatUtils::EBipCommandExecuting );
    TInt uiError( KErrNone );

    if ( iNeedUiSession )
        {
        LOG( NORMAL, "SendData: CSendDataHandler::HandleCommand \
            Sending UI Notification" )

        uiError = SendUiNotification();
        }
    LOG2( NORMAL, "SendData: CSendDataHandler::HandleCommand \
          uiError: %i", uiError )
    if ( KErrNone == uiError )
        {
        iFreeBuffSize = 0;
        TInt err( MSatBIPUtils::ESatBIPSuccess );

        // Get Data Channel
        iDataChannel = iUtils->BipUtils().DataChannel(
            iSendDataData.iDestination, err );

        // Check the SendData mode. If Store, no need for active channel
        if ( ( iDataChannel ) &&
             ( RSat::EStoreDataInTxBuffer == iSendDataData.iMode ) )
            {
            LOG( NORMAL, "SendData: CSendDataHandler::HandleCommand \
                 send data" )
            SendData();
            }
        //Check if ch was found and if not active, try to activate
        else if ( ( iDataChannel ) &&
                  ( MSatBIPUtils::ESatBIPChannelClosed == err ) )
            {
            LOG( NORMAL, "SendData: CSendDataHandler::HandleCommand \
                Inactive channel found, try to activate" )
            TRAP( err, iDataChannel->ActivateConnectionL( this ) );

            if ( err != KErrNone )
                {
                LOG2( NORMAL, "SendData: CSendDataHandler::HandleCommand \
                 Channel activation Failed Error %i", err )

                // Get connection info and remove APN from CommDB, if created
                // during OpenChannel command
                TUint32 apnId( iDataChannel->ConnInfo().iCreatedApnId );
                
                LOG2( NORMAL, "SendData: CSendDataHandler::HandleCommand \
                apnId: %i", apnId )
                if ( apnId > 0 )
                    {
                    TRAP_IGNORE(
                        iUtils->BipUtils().ApnHandler().DeleteApnL( apnId ) )
                    }
                    
                if ( ( KErrMMEtelActivationBlockedByCallControlNoText == err ) ||
                     ( KErrMMEtelActivationBlockedByCallControlWithText == err ) )
                    {
                    err = RSat::KActionNotAllowed;
                    iSendDataRspData.iGeneralResult = 
                        RSat::KInteractionWithCCPermanentError;
                    LOG( DETAILED, "  CC on GPRS not allowed" )
                    }
                    
                else
                    {
                    err = MSatBIPUtils::ESatBIPChannelClosed;
                    iSendDataRspData.iGeneralResult =
                        RSat::KBearerIndepProtocolError;
                    }
                    
                SendTerminalResponse( err );
                }
            }
        // SendImmediate, Only if Active channel exists
        else if ( MSatBIPUtils::ESatBIPSuccess == err )
            {
            LOG( NORMAL, "SendData: CSendDataHandler::HandleCommand \
                 SendImmediate" )
            SendData();
            }
        // Active channel doesn't exist
        else
            {
            LOG2( NORMAL, "SendData: CSendData::HandleCommand \
                    Error %i", err )
            iSendDataRspData.iGeneralResult = RSat::KBearerIndepProtocolError;
            SendTerminalResponse( err );
            }
        }

    LOG( SIMPLE, "SendData: CSendDataHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Indicates the failure of launching ui client.
// -----------------------------------------------------------------------------
//
void CSendDataHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "SendData: CSendDataHandler::UiLaunchFailed calling" )

    iSendDataRspData.iGeneralResult = RSat::KMeUnableToProcessCmd;
    iSendDataRspData.iInfoType = RSat::KMeProblem;
    iSendDataRspData.iAdditionalInfo.SetLength( 1 );
    iSendDataRspData.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
    iSendDataRspData.SetPCmdNumber( iSendDataData.PCmdNumber() );
    TerminalRsp( RSat::ESendData, iSendDataRspPckg );

    LOG( SIMPLE, "SendData: CSendDataHandler::UiLaunchFailed exiting" )
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized.
CSendDataHandler::CSendDataHandler() :
    CSatCommandHandler(),
    iSendDataData(),
    iSendDataPckg( iSendDataData ),
    iSendDataRspData(),
    iSendDataRspPckg( iSendDataRspData ),
    iSendDataUiData(),
    iSendDataUiPckg( iSendDataUiData ),
    iSendDataUiRsp(),
    iSendDataUiRspPckg( iSendDataUiRsp ),
    // To be removed when icons are allowed in this command
    iIconCommand( EFalse )
    {
    LOG( SIMPLE,
        "SendData: CSendDataHandler::CSendDataHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSendDataHandler::ConstructL()
    {
    LOG( SIMPLE, "SendData: CSendDataHandler::ConstructL calling" )

     // Register to ui service request
     iUtils->RegisterServiceRequestL( ESatSProactiveNotification,
        ESatSProactiveNotificationResponse, this );

    // Register to ui service request
    // Cancel key event from dialog
    iUtils->RegisterL( this, MSatUtils::ECommandCancelled );

    // End key from dialog
    iUtils->RegisterL( this, MSatUtils::ECancelledUsingEndKey );

    // Open channel execution
    iUtils->RegisterL( this, MSatUtils::EOpenChannelExecuting );

    LOG( SIMPLE, "SendData: CSendDataHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// Sends data
// -----------------------------------------------------------------------------
//
void CSendDataHandler::SendData()
    {
    LOG( SIMPLE, "SendData: CSendDataHandler::SendData calling" )

    // Indicates the send mode.
    TBool sendImmediately( RSat::ESendDataImmediately == iSendDataData.iMode );
    TInt err( MSatBIPUtils::ESatBIPSuccess );

    TRAPD( error, err = iDataChannel->SendDataL(
        iSendDataData.iChannelData,
        sendImmediately,
        iFreeBuffSize,
        this ) );
    LOG2( NORMAL, "SendData: CSendDataHandler::SendData error: %i", error )

    // Check were the send data success.
    if ( ( error != KErrNone ) || ( err != MSatBIPUtils::ESatBIPSuccess ) )
        {
        LOG2( NORMAL, "SendData: CSendDataHandler::SendData Error %i", err )

        if ( MSatBIPUtils::ESatBIPServiceError == err )
            {
            // Connection suspended
            iSendDataRspData.iGeneralResult = RSat::KMeUnableToProcessCmd;
            err = RSat::KNoSpecificMeProblem;
            }
        else
            {
            iSendDataRspData.iGeneralResult = RSat::KBearerIndepProtocolError;
            }

        SendTerminalResponse( err );
        }
    else if ( !sendImmediately )
        {
        // If data is stored, send terminal response, otherwise wait for
        // notification of data sent.
        LOG( NORMAL, "SendData: CSendDataHandler::SendData Succeed" )
        iSendDataRspData.iGeneralResult = RSat::KSuccess;
        SendTerminalResponse();
        }
    else
        {
        LOG( NORMAL,
            "SendData: CSendDataHandler::SendData Waiting send to complete" )
        }

    LOG( SIMPLE, "SendData: CSendDataHandler::SendData exiting" )
    }

// -----------------------------------------------------------------------------
// Completes Terminal response and sends it
// -----------------------------------------------------------------------------
//
void CSendDataHandler::SendTerminalResponse( const TInt aError )
    {
    LOG( SIMPLE, "SendData: CSendDataHandler::SendTerminalResponse calling" )

    if ( iUserCancelled )
        {
        LOG( NORMAL, "SENDDATA: CSendDataHandler::SendTerminalResponse \
            User cancelled the action" )
        iSendDataRspData.iGeneralResult = RSat::KPSessionTerminatedByUser;
        iSendDataRspData.iInfoType = RSat::KNoAdditionalInfo;
        iSendDataRspData.iAdditionalInfo.Zero();
        }
    else if ( RSat::KSuccess == iSendDataRspData.iGeneralResult )
        {
        LOG( NORMAL, "SENDDATA: CSendDataHandler::SendTerminalResponse \
            KSuccess" )
        iSendDataRspData.iInfoType = RSat::KNoAdditionalInfo;
        iSendDataRspData.iAdditionalInfo.Zero();
        if ( iFreeBuffSize > KSatBIPMoreThanMaxSize )
            {
            LOG( NORMAL, "SENDDATA: CSendDataHandler::SendTerminalResponse \
            iFreeBuffSize > KSatBIPMoreThanMaxSize" )
            iSendDataRspData.iChannelDataLength = KSatBIPMoreThanMaxSize;
            }
        else
            {
            iSendDataRspData.iChannelDataLength =
                static_cast<TUint8> ( iFreeBuffSize );
            }
        // If command had icon data and was done succesfully,
        // report that icon was not shown.
        // To be removed and correct handling (i.e. ClientResponse to
        // notification is received) for general result
        // KSuccessRequestedIconNotDisplayed must be added when icons
        // are allowed in this command 
        if ( iIconCommand )
            {
            LOG( NORMAL, "SENDDATA: CSendDataHandler::SendTerminalResponse \
            iIconCommand true" )
            iSendDataRspData.iGeneralResult =
                RSat::KSuccessRequestedIconNotDisplayed;
            }
        }
    else
        {
        iSendDataRspData.iInfoType = RSat::KMeProblem;
        iSendDataRspData.iAdditionalInfo.SetLength( 1 );
        // Error code from MSatBIPUtils is the same as defined in etelsat.h,
        // so we can put that error code straigth to additional info
        iSendDataRspData.iAdditionalInfo[0] = static_cast<TUint8> ( aError );
        iSendDataRspData.iChannelDataLength = 0;
        }

    if ( IsCommandExecuting() )
        {
        LOG( NORMAL, "SENDDATA: CSendDataHandler::SendTerminalResponse \
            IsCommandExecuting true" )
        iSendDataRspData.SetPCmdNumber( iSendDataData.PCmdNumber() );

        // Send terminal response
        iUtils->NotifyEvent( MSatUtils::EDelaySimSessionEnd );
        iUtils->USatAPI().TerminalRsp( RSat::ESendData, iSendDataRspPckg );
        Start();
        }

    LOG( SIMPLE, "SendData: CSendDataHandler::::SendTerminalResponse exiting" )
    }
    
// -----------------------------------------------------------------------------
// Sends notification to UI
// -----------------------------------------------------------------------------
//
TInt CSendDataHandler::SendUiNotification()
    {
    LOG( NORMAL, "SendData: CSendDataHandler::SendUiNotification calling" )

    // Register to ui service request
    TRAPD( uiError, iUtils->RegisterServiceRequestL(
        ESatSProactiveNotification,
        ESatSProactiveNotificationResponse, this ) );
    LOG2( NORMAL, "CSendDataHandler::SendUiNotification uiError %i", uiError )
    // Check UI error
    if ( KErrNone == uiError )
        {
        // Create IPC data to UI
        iSendDataUiData.iCommand = ESatSSendDataNotify;

        iSendDataUiData.iText = iSendDataData.iAlphaId.iAlphaId;
        iSendDataUiData.iIconId.iIdentifier =
            iSendDataData.iIconId.iIdentifier;
        LOG2( NORMAL, "CSendDataHandler::SendUiNotification \
              iSendDataData.iIconId.iQualifier: %i",
              iSendDataData.iIconId.iQualifier )
        // Define icon qualifier
        if ( RSat::ENotSelfExplanatory == iSendDataData.iIconId.iQualifier )
            {
            iSendDataUiData.iIconId.iIconQualifier = ESatNotSelfExplanatory;
            }
        else if ( RSat::ESelfExplanatory == iSendDataData.iIconId.iQualifier )
            {
            iSendDataUiData.iIconId.iIconQualifier = ESatSelfExplanatory;
            }
        else // Icon qualifier not present
            {
            iSendDataUiData.iIconId.iIconQualifier = ESatENoIconId;
            }
        LOG2( NORMAL, "CSendDataHandler::SendUiNotification \
              iSendDataData.iAlphaId.iStatus: %i",
              iSendDataData.iAlphaId.iStatus )
        // Define AlphaID status
        switch ( iSendDataData.iAlphaId.iStatus )
            {
            case RSat::EAlphaIdNotPresent:
                {
                iSendDataUiData.iAlphaIdStatus = ESatAlphaIdNotProvided;
                break;
                }

            case RSat::EAlphaIdProvided:
                {
                iSendDataUiData.iAlphaIdStatus = ESatAlphaIdNotNull;
                break;
                }

            default:
                {
                iSendDataUiData.iAlphaIdStatus = ESatAlphaIdNull;
                break;
                }
            }

        // Send notification to UI
        MSatUiSession* session = iUtils->SatUiHandler().UiSession();
        session->SendCommand( &iSendDataUiPckg,
                              &iSendDataUiRspPckg,
                              ESatSProactiveNotification );
        }
    else
        {
        // Send error terminal response
        iSendDataRspData.iGeneralResult = RSat::KMeUnableToProcessCmd;
        SendTerminalResponse( RSat::KNoSpecificMeProblem );
        }

    LOG( NORMAL, "SendData: CSendDataHandler::SendUiNotification exiting" )
    return uiError;
    }

