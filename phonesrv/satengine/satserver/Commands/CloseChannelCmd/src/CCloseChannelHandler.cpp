/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles CloseChannel command
*
*/


#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatBIPUtils.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CCloseChannelHandler.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCloseChannelHandler* CCloseChannelHandler::NewL( MSatUtils* aUtils )
    {
    LOG( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::NewL calling" )

    CCloseChannelHandler* self = new( ELeave ) CCloseChannelHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    // ConstructL not needed.
    CleanupStack::Pop( self );

    LOG( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CCloseChannelHandler::~CCloseChannelHandler()
    {
    LOG( NORMAL,
        "CLOSECHANNEL: CCloseChannelHandler::~CCloseChannelHandler calling" )

    Cancel();

    LOG( NORMAL,
        "CLOSECHANNEL: CCloseChannelHandler::~CCloseChannelHandler exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatCommand.
// Response from client
// -----------------------------------------------------------------------------
//
void CCloseChannelHandler::ClientResponse()
    {
    LOG( NORMAL,
        "CLOSECHANNEL: CCloseChannelHandler::ClientResponse calling" )

    // Send terminal response if closing channel doesn't have to be waited and
    // it was necessary to wait UI response due to icon response
    if ( !iWaitHandleCloseCommand && iWaitClientIconResponse )
        {
        // requested icon not shown
        if ( !iCloseChannelUiRsp.iRequestedIconDisplayed )
            {
            LOG( NORMAL, "CCloseChannelHandler::ClientResponse requested icon\
                 not shown" )
            // Icons are not yet supported
            iCloseChannelRspData.iGeneralResult =
                RSat::KSuccessRequestedIconNotDisplayed;
            }
        else
            {
            LOG( NORMAL, "CCloseChannelHandler::ClientResponse success" )
            iCloseChannelRspData.iGeneralResult = RSat::KSuccess;
            }

        iCloseChannelRspData.iInfoType = RSat::KNoAdditionalInfo;
        iCloseChannelRspData.iAdditionalInfo.Zero();
        TerminalRsp( RSat::ECloseChannel, iCloseChannelRspPckg );
        }

    iWaitClientIconResponse = EFalse;
    LOG( NORMAL,
        "CLOSECHANNEL: CCloseChannelHandler::ClientResponse calling" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CCloseChannelHandler::DoCancel()
    {
    LOG( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::DoCancel calling" )

    iUtils->USatAPI().NotifyCloseChannelCancel();

    LOG( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Requests the command notification.
// -----------------------------------------------------------------------------
//
void CCloseChannelHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( NORMAL,
        "CLOSECHANNEL: CCloseChannelHandler::IssueUSATRequest calling" )

    // Reset command data.
    new (&iCloseChannelData) RSat::TCloseChannelV2();

    iUtils->USatAPI().NotifyCloseChannel( aStatus, iCloseChannelPckg );

    LOG( NORMAL,
        "CLOSECHANNEL: CCloseChannelHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Precheck before executing the command.
// -----------------------------------------------------------------------------
//
TBool CCloseChannelHandler::CommandAllowed()
    {
    LOG( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::CommandAllowed calling" )
    TBool commandAllowed( ETrue );
    
    LOG2( NORMAL, 
    "CLOSECHANNEL: CCloseChannelHandler::CommandAllowed \
    iCloseChannelData.iAlphaId.iStatus: %d",
    iCloseChannelData.iAlphaId.iStatus )
    
    LOG2( NORMAL, 
    "CLOSECHANNEL: CCloseChannelHandler::CommandAllowed \
    iCloseChannelData.iIconId.iQualifier: %d",
    iCloseChannelData.iIconId.iQualifier )
    
    // If icon data without alpha id
    if ( ( RSat::EAlphaIdProvided != iCloseChannelData.iAlphaId.iStatus ) &&
        ( ( RSat::ESelfExplanatory ==
            iCloseChannelData.iIconId.iQualifier ) ||
        ( RSat::ENotSelfExplanatory ==
            iCloseChannelData.iIconId.iQualifier ) ) )
        {
        commandAllowed = EFalse;
        iCloseChannelRspData.iGeneralResult = RSat::KCmdDataNotUnderstood;
        iCloseChannelRspData.iInfoType = RSat::KNoAdditionalInfo;
        iCloseChannelRspData.iAdditionalInfo.Zero();
        iCloseChannelRspData.SetPCmdNumber( iCloseChannelData.PCmdNumber() );
        TerminalRsp( RSat::ECloseChannel, iCloseChannelRspPckg );
        LOG( SIMPLE,
            "CLOSECHANNEL: CCloseChannelHandler::CommandAllowed \
            data not understood" )
        }
    // Set icon command flag whether icon data was received and set qualifier
    // to no icon id
    // To be removed when icons are allowed in this command
    else if ( ( RSat::ESelfExplanatory ==
        iCloseChannelData.iIconId.iQualifier ) ||
        ( RSat::ENotSelfExplanatory ==
        iCloseChannelData.iIconId.iQualifier ) )
        {
        LOG( NORMAL, "CCloseChannelHandler::CommandAllowed set no icon id" )
        iIconCommand = ETrue;
        iCloseChannelData.iIconId.iQualifier = RSat::ENoIconId;
        }
    else
        {
        LOG( NORMAL, "CCloseChannelHandler::CommandAllowed others" )
        iIconCommand = EFalse;
        }
    
    LOG2( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::CommandAllowed \
          exiting,commandAllowed: %d", commandAllowed )
    return commandAllowed;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Need for ui session.
// -----------------------------------------------------------------------------
//
TBool CCloseChannelHandler::NeedUiSession()
    {
    LOG( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::NeedUiSession calling" )

    // UI is NOT needed in case where AlphaID is provided and it is a NULL
    const TBool retVal( !( RSat::EAlphaIdNull ==
                           iCloseChannelData.iAlphaId.iStatus ) );
    LOG( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::NeedUiSession calling" )

    // Send notification to Mediator client if Cover UI is supported
    if ( retVal && iUtils->CoverUiSupported() )
        {
        LOG( NORMAL, "CCloseChannelHandler::NeedUiSession CoverUiSupported" )
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::ECloseChannel;
        medEventData.iAlphaId = iCloseChannelData.iAlphaId;
        medEventData.iDuration.iNumOfUnits = KSatDefaultDuration;
        medEventData.iIconID = iCloseChannelData.iIconId;
        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        }

    LOG2( NORMAL, 
    "CLOSECHANNEL: CCloseChannelHandler::NeedUiSession exiting,retVal: %d",
    retVal )
    return retVal;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when USAT API notifies that command.
// -----------------------------------------------------------------------------
//
void CCloseChannelHandler::HandleCommand()
    {
    LOG( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::HandleCommand calling" )

    // Notify other's about this command execution.
    iUtils->NotifyEvent( MSatUtils::ECloseChannelExecuting );

    TInt uiError( KErrNone );

    // Reset flags
    iWaitHandleCloseCommand = EFalse;
    iWaitClientIconResponse = EFalse;

    // If information indication to the user is needed, show it now.
    if ( !( RSat::EAlphaIdNull == iCloseChannelData.iAlphaId.iStatus ) )
        {
        LOG( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::HandleCommand \
            Sending UI Notification" )
        iWaitHandleCloseCommand = ETrue;
        uiError = SendUiNotification();
        }
    LOG2( NORMAL, "CCloseChannelHandler::HandleCommand uiError: %i", 
          uiError )
    // Check UI error status
    if ( KErrNone == uiError )
        {
        // Proceed with the command
        const TInt channelId( iCloseChannelData.iDestination );
        LOG2( NORMAL, 
        "CLOSECHANNEL: CCloseChannelHandler::HandleCommand channelId: %d",
        channelId )
        if ( RSat::KChannel1 > channelId || RSat::KChannel7 < channelId )
            {
            // Create the response IPC package
            iCloseChannelRspData.SetPCmdNumber(
                                 iCloseChannelData.PCmdNumber() );
            iCloseChannelRspData.iGeneralResult = RSat::KCmdDataNotUnderstood;
            iCloseChannelRspData.iInfoType = RSat::KNoAdditionalInfo;
            iCloseChannelRspData.iAdditionalInfo.Zero();
            iWaitClientIconResponse = EFalse;
            }
        else
            {
            // Close the channel
            const TInt err( iUtils->BipUtils().CloseChannel( channelId ) );
            // Create the response IPC package
            iCloseChannelRspData.SetPCmdNumber(
                                 iCloseChannelData.PCmdNumber() );

            // Check were the channel close-up success.
            if ( MSatBIPUtils::ESatBIPSuccess != err )
                {
                LOG2( NORMAL,
                    "CLOSECHANNEL: CCloseChannel::HandleCommand Error %i", err )
                iCloseChannelRspData.iGeneralResult =
                    RSat::KBearerIndepProtocolError;
                iCloseChannelRspData.iInfoType = RSat::KMeProblem;
                iCloseChannelRspData.iAdditionalInfo.SetLength( 1 );
                iCloseChannelRspData.iAdditionalInfo[0] =
                    static_cast<TUint8> ( err );
                iWaitClientIconResponse = EFalse;
                }
            else
                {
                LOG( NORMAL,
                    "CLOSECHANNEL: CCloseChannel::HandleCommand success")
                LOG2( NORMAL,
                "CLOSECHANNEL: CCloseChannel::HandleCommand \
                iCloseChannelUiRsp.iRequestedIconDisplayed: %i", 
                iCloseChannelUiRsp.iRequestedIconDisplayed )
                
                LOG2( NORMAL,
                "CLOSECHANNEL: CCloseChannel::HandleCommand \
                iCloseChannelData.iIconId.iQualifier: %i", 
                iCloseChannelData.iIconId.iQualifier )
                // requested icon not shown
                if ( !iCloseChannelUiRsp.iRequestedIconDisplayed &&
                    ( ( RSat::ENotSelfExplanatory ==
                        iCloseChannelData.iIconId.iQualifier ) ||
                    ( RSat::ESelfExplanatory ==
                        iCloseChannelData.iIconId.iQualifier ) ) )
                    {
                    // Icons could not be shown
                    iCloseChannelRspData.iGeneralResult =
                        RSat::KSuccessRequestedIconNotDisplayed;
                    }
                else
                    {
                    iCloseChannelRspData.iGeneralResult = RSat::KSuccess;
                    }

                // No additional info for CloseChannel
                iCloseChannelRspData.iInfoType = RSat::KNoAdditionalInfo;
                iCloseChannelRspData.iAdditionalInfo.Zero();
                }
            }

        iWaitHandleCloseCommand = EFalse;
        
        // If command had icon data and was done succesfully, report that icon
        // was not shown
        // To be removed when icons are allowed in this command
        if ( ( RSat::KSuccess == iCloseChannelRspData.iGeneralResult ) &&
            iIconCommand )
            {
            LOG( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::\
                         HandleCommand requested icon not shown" )
            iCloseChannelRspData.iGeneralResult =
                RSat::KSuccessRequestedIconNotDisplayed;
            }
        
        // Send terminal response if UI response doesn't have to be waited
        if ( !iWaitClientIconResponse )
            {
            LOG( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::\
                         HandleCommand send terminal response" )
            TerminalRsp( RSat::ECloseChannel, iCloseChannelRspPckg );
            }
        }

    LOG( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Indicates the failure of launching ui client.
// -----------------------------------------------------------------------------
//
void CCloseChannelHandler::UiLaunchFailed()
    {
    LOG( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::UiLaunchFailed calling" )

    iCloseChannelRspData.iGeneralResult = RSat::KMeUnableToProcessCmd;
    iCloseChannelRspData.iInfoType = RSat::KMeProblem;
    iCloseChannelRspData.iAdditionalInfo.SetLength( 1 );
    iCloseChannelRspData.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
    iCloseChannelRspData.SetPCmdNumber( iCloseChannelData.PCmdNumber() );
    TerminalRsp( RSat::ECloseChannel, iCloseChannelRspPckg );

    LOG( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::UiLaunchFailed exiting" )
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized.
CCloseChannelHandler::CCloseChannelHandler() :
    CSatCommandHandler(),
    iCloseChannelData(),
    iCloseChannelPckg( iCloseChannelData ),
    iCloseChannelRspData(),
    iCloseChannelRspPckg( iCloseChannelRspData ),
    iCloseChannelUiData(),
    iCloseChannelUiPckg( iCloseChannelUiData ),
    iCloseChannelUiRsp(),
    iCloseChannelUiRspPckg( iCloseChannelUiRsp ),
    iWaitHandleCloseCommand( EFalse ),
    iWaitClientIconResponse( EFalse ),
    // To be removed when icons are allowed in this command
    iIconCommand( EFalse )
    {
    LOG( NORMAL, "CLOSECHANNEL: CCloseChannelHandler::CCloseChannelHandler \
        calling - exiting" )
    }

// -----------------------------------------------------------------------------
// Sends notification to UI.
// -----------------------------------------------------------------------------
//
TInt CCloseChannelHandler::SendUiNotification()
    {
    LOG( NORMAL,
        "CloseChannel: CCloseChannelHandler::SendUiNotification calling" )

    // Register to ui service request
    TRAPD( uiError, iUtils->RegisterServiceRequestL(
        ESatSProactiveNotification,
        ESatSProactiveNotificationResponse,
        this ) );
     LOG2( NORMAL, "CCloseChannelHandler::SendUiNotification uiError: %i",
           uiError )
    if ( KErrNone != uiError )
        {
        // Send error terminal response
        UiLaunchFailed();
        }
    else
        {
        // Create IPC data to UI
        iCloseChannelUiData.iCommand = ESatSCloseChannelNotify;
        iCloseChannelUiData.iText = iCloseChannelData.iAlphaId.iAlphaId;
        iCloseChannelUiData.iIconId.iIdentifier
             = iCloseChannelData.iIconId.iIdentifier;

        // Define icon qualifier
        if ( RSat::ENotSelfExplanatory ==
            iCloseChannelData.iIconId.iQualifier )
            {
            LOG( NORMAL, "CloseChannel: CCloseChannelHandler::\
                 SendUiNotification ENotSelfExplanatory" )
            iCloseChannelUiData.iIconId.iIconQualifier =
                ESatNotSelfExplanatory;
            // Terminal response may be only when UI has resposed
            iWaitClientIconResponse = ETrue;
            }
        else if ( RSat::ESelfExplanatory ==
            iCloseChannelData.iIconId.iQualifier )
            {
            LOG( NORMAL, "CloseChannel: CCloseChannelHandler::\
                 SendUiNotification ESelfExplanatory" )
            iCloseChannelUiData.iIconId.iIconQualifier = ESatSelfExplanatory;
            // Terminal response may be only when UI has resposed
            iWaitClientIconResponse = ETrue;
            }
        else // Icon qualifier not present
            {
            LOG( NORMAL, "CloseChannel: CCloseChannelHandler::\
                 SendUiNotification others" )
            iCloseChannelUiData.iIconId.iIconQualifier = ESatENoIconId;
            }
        LOG2( NORMAL, "CCloseChannelHandler::SendUiNotification \
              iCloseChannelData.iAlphaId.iStatus: %i",
              iCloseChannelData.iAlphaId.iStatus )
        // Define AlphaID status
        switch ( iCloseChannelData.iAlphaId.iStatus )
            {
            case RSat::EAlphaIdNotPresent:
                {
                iCloseChannelUiData.iAlphaIdStatus = ESatAlphaIdNotProvided;
                break;
                }

            case RSat::EAlphaIdProvided:
                {
                iCloseChannelUiData.iAlphaIdStatus = ESatAlphaIdNotNull;
                break;
                }

            default:
                {
                iCloseChannelUiData.iAlphaIdStatus = ESatAlphaIdNull;
                break;
                }
            }

        // Send notification to UI
        MSatUiSession* session = iUtils->SatUiHandler().UiSession();
        session->SendCommand( &iCloseChannelUiPckg,
                              &iCloseChannelUiRspPckg,
                              ESatSProactiveNotification );
        }

    LOG( NORMAL,
        "CloseChannel: CCloseChannelHandler::SendUiNotification exiting" )
    return uiError;
    }
