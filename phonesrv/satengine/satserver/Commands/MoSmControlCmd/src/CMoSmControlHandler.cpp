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
* Description:  Handles MO ShortMessage Control command
*
*/



// INCLUDE FILES
#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatUiSession.h"
#include    "MSatSUiClientHandler.h"
#include    "CMoSmControlHandler.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMoSmControlHandler::CMoSmControlHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
//lint -e{1403, 1769} Can not be initialized, harmless.
CMoSmControlHandler::CMoSmControlHandler() :
    CSatCommandHandler(),
    iMoSmControlData(),
    iMoSmControlPckg( iMoSmControlData ),
    iMoSmControlSendData(),
    iMoSmControlSendPckg( iMoSmControlSendData ),
    iMoSmControlUiRespData(),
    iMoSmControlUiRespPckg( iMoSmControlUiRespData )
    {
    LOG( SIMPLE, "MOSMCONTROL: \
        CMoSmControlHandler::CMoSmControlHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CMoSmControlHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMoSmControlHandler::ConstructL()
    {
    LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::ConstructL calling" )

    // Register notification observer
    iUtils->RegisterServiceRequestL(
        ESatSProactiveNotification,
        ESatSProactiveNotificationResponse,
        this );

    LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CMoSmControlHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMoSmControlHandler* CMoSmControlHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::NewL calling" )

    CMoSmControlHandler* self = new( ELeave ) CMoSmControlHandler();

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::NewL exiting" )
    return self;
    }


// Destructor
CMoSmControlHandler::~CMoSmControlHandler()
    {
    LOG( SIMPLE,
        "MOSMCONTROL: CMoSmControlHandler::~CMoSmControlHandler calling" )

    Cancel();

    LOG( SIMPLE,
        "MOSMCONTROL: CMoSmControlHandler::~CMoSmControlHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CMoSmControlHandler::ClientResponse
// Handles responses from client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMoSmControlHandler::ClientResponse()
    {
    LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::ClientResponse calling" )

    // Start to receive more commands. We have to start again here because
    // we don't send terminal response, wich does this automatically.
    Start();

    // Inform the system that we are finished executing. If there is proactive
    // SendSm command pending, it is notified and it continues from here.
    iUtils->NotifyEvent( MSatUtils::EMoSmControlDone );

    // If we launched UI, close it.
    if ( !iUtils->SatUiHandler().UiLaunchedByUser() )
        {
        LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::ClientResponse \
             ESimSessionEndCallBack" )
        iUtils->NotifyEvent( MSatUtils::ESimSessionEndCallBack );
        }

    else
        {
        LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::ClientResponse \
             EDelaySimSessionEnd" )
        // Inform the session that we are done here.
        iUtils->NotifyEvent( MSatUtils::EDelaySimSessionEnd );
        }

    // We do not send TerminalResponse to SIM.
    LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// CMoSmControlHandler::DoCancel
// Cancels the SAT request
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMoSmControlHandler::DoCancel()
    {
    LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::DoCancel calling" )

    iUtils->USatAPI().NotifyMoSmControlCancel();

    LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CMoSmControlHandler::IssueUSATRequest
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMoSmControlHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iMoSmControlData) RSat::TMoSmControlV1();
    iNeedUiSession = EFalse;

    iUtils->USatAPI().NotifyMoSmControl( aStatus, iMoSmControlPckg );

    LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// CMoSmControlHandler::CommandAllowed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CMoSmControlHandler::CommandAllowed()
    {
    LOG( SIMPLE, 
        "MOSMCONTROL: CMoSmControlHandler::CommandAllowed calling - exiting" )
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMoSmControlHandler::NeedUiSession
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CMoSmControlHandler::NeedUiSession()
    {
    LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::NeedUiSession calling" )
    // UI is not needed, if control is AllowedWithMod and AlphaId is NULL or
    // control is Allowed and AlphaId is not provided

    const RSat::TAlphaIdStatus alphaId( iMoSmControlData.iAlphaId.iStatus );
    const RSat::TControlResult control( iMoSmControlData.iResult );
    
    const TBool alphaIdNull( RSat::EAlphaIdNull == alphaId ||
                             RSat::EAlphaIdNotSet == alphaId );

    const TBool controlAllowed( 
                RSat::EAllowedNoModification == control ||
                RSat::EControlResultNotSet == control );

    if ( RSat::EAllowedWithModifications == control && 
         alphaIdNull )
        {
        LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::NeedUiSession \
        RSat::EAllowedWithModifications == control" )
        iNeedUiSession = EFalse;
        }
    else if ( controlAllowed && ( RSat::EAlphaIdProvided != alphaId ) )
        {
        LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::NeedUiSession \
        RSat::EAlphaIdProvided != alphaId" )
        iNeedUiSession = EFalse;
        }
    else
        {
        iNeedUiSession = ETrue;
        }

    LOG2( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::NeedUiSession exiting,\
          iNeedUiSession:%d", iNeedUiSession )
    return iNeedUiSession;
    }

// -----------------------------------------------------------------------------
// CMoSmControlHandler::HandleCommand
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMoSmControlHandler::HandleCommand()
    {
    LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::HandleCommand calling" )

    // Notify other commands that we are executing.
    iUtils->NotifyEvent( MSatUtils::EMoSmControlExecuting );
    LOG2( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::HandleCommand \
          iNeedUiSession:%d", iNeedUiSession )
    if ( iNeedUiSession )
        {
        // Clear send data.
        TSatNotificationV1 temp;
        iMoSmControlSendData = temp;
        
        // Enums to UI related parameters
        TSatAlphaIdStatus satAlphaIdStatus;
        TSatControlResult satControlResult;
        LOG2( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::HandleCommand \
              iMoSmControlData.iAlphaId.iStatus:%d", 
              iMoSmControlData.iAlphaId.iStatus )
        // Map RSat value
        switch ( iMoSmControlData.iAlphaId.iStatus )
            {
            case RSat::EAlphaIdProvided :
                {
                satAlphaIdStatus = ESatAlphaIdNotNull;
                break;
                }

            case RSat::EAlphaIdNotPresent :
                {
                satAlphaIdStatus = ESatAlphaIdNotProvided;
                break;
                }

            // Use this also as a default value
            case RSat::EAlphaIdNull :
            case RSat::EAlphaIdNotSet :
            default :
                {
                satAlphaIdStatus = ESatAlphaIdNull;
                break;
                }
            }
        LOG2( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::HandleCommand \
              iMoSmControlData.iResult:%d", iMoSmControlData.iResult )
        // Map RSat value
        switch ( iMoSmControlData.iResult )
            {
            case RSat::ENotAllowed :
                {
                satControlResult = ESatNotAllowed;
                break;
                }

            case RSat::EAllowedWithModifications :
                {
                satControlResult = ESatAllowedWithModifications;
                break;
                }

            // Use this also as a default value
            case RSat::EAllowedNoModification :
            case RSat::EControlResultNotSet :
            default :
                {
                satControlResult = ESatAllowedNoModification;
                break;
                }
            }

        // Build the IPC package.
        iMoSmControlSendData.iCommand = ESatSMoSmControlNotify;
        iMoSmControlSendData.iText = iMoSmControlData.iAlphaId.iAlphaId;
        iMoSmControlSendData.iAlphaIdStatus = satAlphaIdStatus;
        iMoSmControlSendData.iControlResult = satControlResult;

        // Register notification observer
        TRAPD( regErr, iUtils->RegisterServiceRequestL(
            ESatSProactiveNotification,
            ESatSProactiveNotificationResponse,
            this )
            ); // TRAPD
        LOG2( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::HandleCommand \
              regErr:%d", regErr )
        // No need to check error value, since we cannot send terminal response
        // for this command
        if ( KErrNone == regErr )
            {
            // Get UISession.
            MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();

            // Send command to SatClient.
            uiSession->SendCommand( &iMoSmControlSendPckg,
                &iMoSmControlUiRespPckg, ESatSProactiveNotification );
            }
        }

    else
        {
        // Restart request since we don't get client response
        Start();

        // Inform the system that we are finished executing. If there is 
        // proactive
        // SendSm command pending, it is notified and it continues from here.
        iUtils->NotifyEvent( MSatUtils::EMoSmControlDone );

        // Inform the session that we are done here.
        iUtils->NotifyEvent( MSatUtils::EDelaySimSessionEnd );
        }

    LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// CMoSmControlHandler::UiLaunchFailed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMoSmControlHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::UiLaunchFailed calling" )

    // We cannot send terminal response to UICC since it doesn't
    // expect terminal response from MoSmControl command...

    // Inform the session that we are done here.
    iUtils->NotifyEvent( MSatUtils::EDelaySimSessionEnd );

    // Start to receive more commands.
    Start();

    LOG( SIMPLE, "MOSMCONTROL: CMoSmControlHandler::UiLaunchFailed exiting" )
    }

//  End of File
