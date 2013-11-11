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
* Description:  Handles ReceiveData command
*
*/


#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatSystemState.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CReceiveDataHandler.h"
#include    "SatLog.h"
#include    "MSatBIPDataChannel.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CReceiveDataHandler* CReceiveDataHandler::NewL( MSatUtils* aUtils )
    {
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::NewL calling" )
    CReceiveDataHandler* self = new( ELeave ) CReceiveDataHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CReceiveDataHandler::~CReceiveDataHandler()
    {
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::\
        CReceiveDataHandler calling" )
    Cancel();
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::\
        ~CReceiveDataHandler exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatCommand.
// Response from the client.
// -----------------------------------------------------------------------------
//
void CReceiveDataHandler::ClientResponse()
    {
    LOG( NORMAL, "CReceiveDataHandler::ClientResponse calling-exiting" )
    }
    
// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Waits for indication of user wanting to close the BIP session.
// -----------------------------------------------------------------------------
//
void CReceiveDataHandler::Event( TInt aEvent )
    {
    LOG2( NORMAL, "ReceiveData: CReceiveDataHandler::Event calling, \
        aEvent:%d", aEvent )

    switch ( aEvent )
        {
        case MSatUtils::ECommandCancelled:
        case MSatUtils::ECancelledUsingEndKey:
            {
            // Check Channel activation status
            if ( iUtils->BipUtils().IsContextActive() )
                {
                LOG( NORMAL, "ReceiveData: CReceiveDataHandler::Event \
                     IsContextActive" )
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
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::Event exiting" )
    }


// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CReceiveDataHandler::DoCancel()
    {
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::\
        DoCancel calling" )
    iUtils->USatAPI().NotifyReceiveDataCancel();
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::\
        DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Requests the command notification.
// -----------------------------------------------------------------------------
//
void CReceiveDataHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::\
        IssueUSATRequest calling" )
    // Empty data
    new (&iReceiveDataData) RSat::TReceiveDataV2();

    iUtils->USatAPI().NotifyReceiveData( aStatus, iReceiveDataPckg );
    iUtils->NotifyEvent( MSatUtils::EBipCommandDoneExecuting );
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::\
        IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Precheck before executing the command.
// -----------------------------------------------------------------------------
//
TBool CReceiveDataHandler::CommandAllowed()
    {
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::CommandAllowed \
        calling" )
    TBool commandAllowed( ETrue );

    // Check user cancel
    if ( iUserCancelled )
        {
        LOG( NORMAL, "ReceiveData: CReceiveDataHandler::CommandAllowed \
             iUserCancelled true" )
        SendTerminalResponse();
        commandAllowed = EFalse;
        }
    else
        {

        // Proceed with the command
        const TInt channelId( iReceiveDataData.iDestination );

        if ( ( RSat::KChannel1 > channelId ) ||
            ( RSat::KChannel7 < channelId ) ||
            // If icon data without alpha id 
            ( ( RSat::EAlphaIdProvided !=
                iReceiveDataData.iAlphaId.iStatus ) &&
            ( ( RSat::ESelfExplanatory ==
                iReceiveDataData.iIconId.iQualifier ) ||
            ( RSat::ENotSelfExplanatory ==
                iReceiveDataData.iIconId.iQualifier ) ) ) )
            {
            LOG( NORMAL, "ReceiveData: CReceiveDataHandler::CommandAllowed \
                Unknown channel identifier or icon without alpha id - \
                Not allowed" )
            // Create the response IPC package
            iReceiveDataRspData.SetPCmdNumber(
                                 iReceiveDataData.PCmdNumber() );
            iReceiveDataRspData.iGeneralResult = RSat::KCmdDataNotUnderstood;
            iReceiveDataRspData.iInfoType = RSat::KNoAdditionalInfo;
            iReceiveDataRspData.iAdditionalInfo.Zero();
            TerminalRsp( RSat::EReceiveData, iReceiveDataRspPckg );
            commandAllowed = EFalse;
            }
        // Set icon command flag whether icon data was received and set qualifier
        // to no icon id
        // To be removed when icons are allowed in this command
        else if ( ( RSat::ESelfExplanatory ==
            iReceiveDataData.iIconId.iQualifier ) ||
            ( RSat::ENotSelfExplanatory ==
            iReceiveDataData.iIconId.iQualifier ) )
            {
            LOG( NORMAL, "ReceiveData: CReceiveDataHandler::CommandAllowed \
                 ENoIconId" )
            iIconCommand = ETrue;
            iReceiveDataData.iIconId.iQualifier = RSat::ENoIconId;
            }
        else
            {
            LOG( NORMAL, "ReceiveData: CReceiveDataHandler::CommandAllowed \
                 others" )
            iIconCommand = EFalse;
            }
            
        // ReceiveData doesn't need to check Service, ongoing calls etc.
        // because data is waiting in SAT Engine, so ReceiveData doesn't 
        // do any network operations. 
        }
    LOG2( NORMAL, "ReceiveData: CReceiveDataHandler::CommandAllowed exiting,\
          commandAllowed: %d", commandAllowed )
    return commandAllowed;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Need for ui session.
// -----------------------------------------------------------------------------
//
TBool CReceiveDataHandler::NeedUiSession()
    {
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::NeedUiSession calling" )
    // UI is NOT needed in case where AlphaID is provided and it is a NULL
    iNeedUiSession = ( !( RSat::EAlphaIdNull ==
                          iReceiveDataData.iAlphaId.iStatus ) &&
                       !( RSat::EAlphaIdNotPresent ==
                          iReceiveDataData.iAlphaId.iStatus ) );
    // Send notification to Mediator client if Cover UI is supported
    if ( iNeedUiSession && iUtils->CoverUiSupported() )
        {
        LOG( NORMAL, "ReceiveData: CReceiveDataHandler::NeedUiSession \
             CoverUiSupported" )
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::EReceiveData;
        medEventData.iAlphaId = iReceiveDataData.iAlphaId;
        medEventData.iDuration.iNumOfUnits = KSatDefaultDuration;
        medEventData.iIconID = iReceiveDataData.iIconId;
        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        }
    LOG2( NORMAL, "ReceiveData: CReceiveDataHandler::NeedUiSession exiting,\
          iNeedUiSession: %d", iNeedUiSession )
    return iNeedUiSession;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when USAT API notifies that command.
// -----------------------------------------------------------------------------
//
void CReceiveDataHandler::HandleCommand()
    {
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::HandleCommand calling" )

    // Notify other's about this command executiuon
    iUtils->NotifyEvent( MSatUtils::EReceiveDataExecuting );
    iUtils->NotifyEvent( MSatUtils::EBipCommandExecuting );
    TInt uiError( KErrNone );

    if ( iNeedUiSession )
        {
        LOG( NORMAL, "ReceiveData: CReceiveDataHandler::HandleCommand \
            Sending UI Notification" )
        uiError = SendUiNotification();
        }
    LOG2( NORMAL, 
    "ReceiveData: CReceiveDataHandler::HandleCommand uiError: %i", uiError )
    if ( KErrNone == uiError )
        {
        iChDataLength = 0;

        // Proceed with the command
        TInt err( MSatBIPUtils::ESatBIPSuccess );
        TInt error( KErrNone );

        TInt requestedLength( iReceiveDataData.iChannelDataLength );

        // Make sure that reguested data length is not
        // more than KSatBIPMaxResponseSize
        if ( requestedLength > KSatBIPMaxResponseSize )
            {
            LOG( NORMAL, "ReceiveData: CReceiveDataHandler::HandleCommand \
            requestedLength > KSatBIPMaxResponseSize" )
            requestedLength = KSatBIPMaxResponseSize;
            }

        LOG2( NORMAL, "ReceiveData: CReceiveDataHandler::HandleCommand \
        Requested data length: %i", requestedLength )

        // Get Data Channel
        MSatBIPDataChannel* datachannel =
            iUtils->BipUtils().DataChannel(
                iReceiveDataData.iDestination, err );
                
        LOG2( NORMAL, "ReceiveData: CReceiveDataHandler::HandleCommand \
        err: %i", err )
        //Check if ch was found
        if ( MSatBIPUtils::ESatBIPSuccess == err 
            || MSatBIPUtils::ESatBIPChannelClosed == err )
            {
            LOG( NORMAL, "ReceiveData: CReceiveDataHandler::HandleCommand \
            MSatBIPUtils::ESatBIPSuccess == err" )
            iChData.Zero();
            TRAP( error, err = datachannel->ReceiveDataL(
                                    iChData,
                                    requestedLength,
                                    iChDataLength ) );
            }
        LOG2( NORMAL, "ReceiveData: CReceiveDataHandler::HandleCommand \
        error: %i", error )
        // No point to continue
        if ( error != KErrNone )
            {
            iReceiveDataRspData.iGeneralResult =
                RSat::KBearerIndepProtocolError;
            }
        // Not all requested data yet available
        else if ( MSatBIPUtils::ESatBIPReqBufSizeNotAvail == err ||
                  MSatBIPUtils::ESatBIPChannelClosed == err )
            {
            LOG( NORMAL, "ReceiveData: CReceiveDataHandler::HandleCommand \
                Not all requested data available" )
            iReceiveDataRspData.iGeneralResult = RSat::KMissingInformation;
            }
        // Some other error
        else if ( MSatBIPUtils::ESatBIPSuccess != err )
            {
            LOG2( NORMAL, "ReceiveData: CReceiveDataHandler::HandleCommand \
                Error %i", err )
            iReceiveDataRspData.iGeneralResult =
                RSat::KBearerIndepProtocolError;
            }
        // Everything fine
        else
            {
            LOG( NORMAL, 
            "ReceiveData: CReceiveDataHandler::HandleCommand success")
            iReceiveDataRspData.iGeneralResult = RSat::KSuccess;
            }

        // Send terminal response
        SendTerminalResponse( err );
        }

    LOG2( NORMAL, 
    "ReceiveData: CReceiveDataHandler::HandleCommand %i bytes Remain in RX \
    buffer.", iChDataLength )
    LOG2( NORMAL, 
    "ReceiveData: CReceiveDataHandler::HandleCommand Returning %i bytes", 
    iChData.Length() )

    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Indicates the failure of launching ui client.
// -----------------------------------------------------------------------------
//
void CReceiveDataHandler::UiLaunchFailed()
    {
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::UiLaunchFailed calling" )
    iReceiveDataRspData.iGeneralResult = RSat::KMeUnableToProcessCmd;
    iReceiveDataRspData.iInfoType = RSat::KMeProblem;
    iReceiveDataRspData.iAdditionalInfo.SetLength( 1 );
    iReceiveDataRspData.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
    iReceiveDataRspData.SetPCmdNumber( iReceiveDataData.PCmdNumber() );
    TerminalRsp( RSat::EReceiveData, iReceiveDataRspPckg );
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::UiLaunchFailed exiting" )
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized.
CReceiveDataHandler::CReceiveDataHandler() :
    CSatCommandHandler(),
    iReceiveDataData(),
    iReceiveDataPckg( iReceiveDataData ),
    iReceiveDataRspData(),
    iReceiveDataRspPckg( iReceiveDataRspData ),
    iReceiveDataUiData(),
    iReceiveDataUiPckg( iReceiveDataUiData ),
    iReceiveDataUiRsp(),
    iReceiveDataUiRspPckg( iReceiveDataUiRsp ),
    // To be removed when icons are allowed in this command
    iIconCommand( EFalse )
    {
    LOG( NORMAL,
        "ReceiveData: CReceiveDataHandler::\
            CReceiveDataHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CReceiveDataHandler::ConstructL()
    {
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::ConstructL \
        calling" )

    iUtils->RegisterServiceRequestL( ESatSProactiveNotification,
        ESatSProactiveNotificationResponse, this );

    // Register to ui service request
    iUtils->RegisterL( this, MSatUtils::ECommandCancelled );
    iUtils->RegisterL( this, MSatUtils::ECancelledUsingEndKey );
    iUtils->RegisterL( this, MSatUtils::EOpenChannelExecuting );

    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::ConstructL \
        exiting" )
    }

// -----------------------------------------------------------------------------
// Sends notification to UI.
// -----------------------------------------------------------------------------
//
TInt CReceiveDataHandler::SendUiNotification()
    {
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::SendUiNotification \
        calling" )
    // Register to ui service request
    TRAPD( uiError, iUtils->RegisterServiceRequestL(
        ESatSProactiveNotification,
        ESatSProactiveNotificationResponse, this ) );
    LOG2( NORMAL, "ReceiveData: CReceiveDataHandler::SendUiNotification \
          uiError: %d", uiError )
    // Check UI error
    if ( KErrNone == uiError )
        {
        // Create IPC data to UI
        iReceiveDataUiData.iCommand = ESatSReceiveDataNotify;
        
        iReceiveDataUiData.iText = iReceiveDataData.iAlphaId.iAlphaId;
        iReceiveDataUiData.iIconId.iIdentifier
             = iReceiveDataData.iIconId.iIdentifier;

        // Define icon qualifier
        if ( RSat::ENotSelfExplanatory ==
            iReceiveDataData.iIconId.iQualifier )
            {
            LOG( NORMAL, "ReceiveData: CReceiveDataHandler::\
                 SendUiNotification ENotSelfExplanatory" )
            iReceiveDataUiData.iIconId.iIconQualifier =
                ESatNotSelfExplanatory;
            }
        else if ( RSat::ESelfExplanatory ==
            iReceiveDataData.iIconId.iQualifier )
            {
            LOG( NORMAL, "ReceiveData: CReceiveDataHandler::\
                 SendUiNotification ESelfExplanatory" )
            iReceiveDataUiData.iIconId.iIconQualifier = ESatSelfExplanatory; 
            }
        else // Icon qualifier not present
            {
            LOG( NORMAL, "ReceiveData: CReceiveDataHandler::\
                 SendUiNotification ESatENoIconId" )
            iReceiveDataUiData.iIconId.iIconQualifier = ESatENoIconId;
            }
        LOG2( NORMAL, "ReceiveData: CReceiveDataHandler::SendUiNotification \
              iReceiveDataData.iAlphaId.iStatus: %d", 
              iReceiveDataData.iAlphaId.iStatus )    
        // Define AlphaID status
        switch ( iReceiveDataData.iAlphaId.iStatus )
            {
            case RSat::EAlphaIdNotPresent:
                {
                iReceiveDataUiData.iAlphaIdStatus = ESatAlphaIdNotProvided;
                break;
                }

            case RSat::EAlphaIdProvided:
                {
                iReceiveDataUiData.iAlphaIdStatus = ESatAlphaIdNotNull;
                break;
                }

            default:
                {
                iReceiveDataUiData.iAlphaIdStatus = ESatAlphaIdNull;
                break;
                }
            }

        // Send notification to UI
        MSatUiSession* session = iUtils->SatUiHandler().UiSession();
        session->SendCommand( &iReceiveDataUiPckg,
                              &iReceiveDataUiRspPckg,
                              ESatSProactiveNotification );
        }
    else
        {
        // Send error terminal response
        iReceiveDataRspData.iGeneralResult = RSat::KMeUnableToProcessCmd;
        SendTerminalResponse( RSat::KNoSpecificMeProblem );
        }
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::SendUiNotification \
        exiting" )
    return uiError;
    }

// -----------------------------------------------------------------------------
// Completes Terminal response and sends it.
// -----------------------------------------------------------------------------
//
void CReceiveDataHandler::SendTerminalResponse( const TInt aError )
    {
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::SendTerminalResponse \
        calling" )

    if ( iUserCancelled )
        {
        LOG( NORMAL, "ReceiveData: CReceiveDataHandler::SendTerminalResponse \
            User cancelled the action" )
        iReceiveDataRspData.iGeneralResult = RSat::KPSessionTerminatedByUser;
        iReceiveDataRspData.iInfoType = RSat::KNoAdditionalInfo;
        iReceiveDataRspData.iAdditionalInfo.Zero();
        }
    else if ( RSat::KMissingInformation == iReceiveDataRspData.iGeneralResult ||
              RSat::KSuccess == iReceiveDataRspData.iGeneralResult )
        {
        LOG( NORMAL, "ReceiveData: CReceiveDataHandler::SendTerminalResponse\
            KMissingInformation or KSuccess" )
        iReceiveDataRspData.iInfoType = RSat::KChannelData;
        iReceiveDataRspData.iAdditionalInfo.Copy( iChData );
        // More than maximum bytes remaining
        if ( iChDataLength > KSatBIPMoreThanMaxSize )
            {
            LOG( NORMAL, "ReceiveData: CReceiveDataHandler::\
            SendTerminalResponse iChDataLength > KSatBIPMoreThanMaxSize" )
            iReceiveDataRspData.iChannelDataLength = KSatBIPMoreThanMaxSize;
            }
        else
            {
            iReceiveDataRspData.iChannelDataLength =
                static_cast<TUint8>( iChDataLength );
            }

        // If command had icon data and was done succesfully,
        // report that icon was not shown.
        // To be removed and correct handling (i.e. ClientResponse to
        // notification is received) for general result
        // KSuccessRequestedIconNotDisplayed must be added when icons
        // are allowed in this command 
        if ( ( RSat::KSuccess == iReceiveDataRspData.iGeneralResult ) &&
            iIconCommand )
            {
            LOG( NORMAL, "ReceiveData: CReceiveDataHandler::\
            SendTerminalResponse KSuccessRequestedIconNotDisplayed" )
            iReceiveDataRspData.iGeneralResult =
                RSat::KSuccessRequestedIconNotDisplayed;
            }
        }
    else
        {
        LOG( NORMAL, "ReceiveData: CReceiveDataHandler::SendTerminalResponse\
             others" )
        iReceiveDataRspData.iInfoType = RSat::KMeProblem;
        iReceiveDataRspData.iAdditionalInfo.SetLength( 1 );
        // Error code from MSatBIPUtils is the same as defined in etelsat.h,
        // so we can put that error code straigth to additional info
        iReceiveDataRspData.iAdditionalInfo[0] = static_cast<TUint8> ( aError );
        iReceiveDataRspData.iChannelDataLength = 0;
        }

    if ( IsCommandExecuting() )
        {
        LOG( NORMAL, "ReceiveData: CReceiveDataHandler::SendTerminalResponse\
             IsCommandExecuting" )
        // Create the response IPC package
        iReceiveDataRspData.SetPCmdNumber( iReceiveDataData.PCmdNumber() );

        // Fixed order to send terminal response.
        iUtils->NotifyEvent( MSatUtils::EDelaySimSessionEnd );
        iUtils->USatAPI().TerminalRsp( RSat::EReceiveData, iReceiveDataRspPckg );
        Start();
        }
    LOG( NORMAL, "ReceiveData: CReceiveDataHandler::SendTerminalResponse \
        exiting" )
    }

