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
* Description:  Handles GetInputCall command
*
*/


#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatUiSession.h"
#include    "MSatSystemState.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CGetInputHandler.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CGetInputHandler* CGetInputHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "GETINPUT: CGetInputHandler::NewL calling" )

    CGetInputHandler* self = new( ELeave ) CGetInputHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "GETINPUT: CGetInputHandler::NewL exiting" )
    return self;
    }


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CGetInputHandler::~CGetInputHandler()
    {
    LOG( SIMPLE, "GETINPUT: CGetInputHandler::~CGetInputHandler calling" )

    Cancel();

    LOG( SIMPLE, "GETINPUT: CGetInputHandler::~CGetInputHandler exiting" )
    }

// ---------------------------------------------------------------------------
// From class MSatCommand.
// Response from the client.
// ---------------------------------------------------------------------------
//
void CGetInputHandler::ClientResponse()
    {
    LOG( SIMPLE, "GETINPUT: CGetInputHandler::ClientResponse calling" )

    iGetInputRsp.SetPCmdNumber( iGetInputData.PCmdNumber() );
    if ( RSat::KPSessionTerminatedByUser == iGetInputRsp.iGeneralResult )
        {
        LOG( SIMPLE, "GETINPUT: CGetInputHandler::ClientResponse close ui \
             session" )
        // Next SimSession end will close the ui session
        iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
        }
        
    // If command had icon data and was done succesfully, report that icon
    // was not shown
    // To be removed when icons are allowed in this command
    if ( ( RSat::KSuccess == iGetInputRsp.iGeneralResult ) &&
        iIconCommand )
        {
        LOG( SIMPLE, "GETINPUT: CGetInputHandler::ClientResponse requested \
             icon not displayed" )
        iGetInputRsp.iGeneralResult =
                    RSat::KSuccessRequestedIconNotDisplayed;
        }

    CSatCommandHandler::TerminalRsp( RSat::EGetInput, iGetInputRspPckg );

    LOG( SIMPLE, "GETINPUT: CGetInputHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CGetInputHandler::DoCancel()
    {
    LOG( SIMPLE, "GETINPUT: CGetInputHandler::DoCancel calling" )

    iUtils->USatAPI().NotifyGetInputCancel();

    LOG( SIMPLE, "GETINPUT: CGetInputHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Requests the command notification.
// -----------------------------------------------------------------------------
//
void CGetInputHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "GETINPUT: CGetInputHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iGetInputData) RSat::TGetInputV1();
    iGetInputRsp.iGeneralResult = RSat::KPSessionTerminatedByUser; // default

    iUtils->USatAPI().NotifyGetInput( aStatus, iGetInputPckg );

    LOG( SIMPLE, "GETINPUT: CGetInputHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Precheck before executing the command.
// -----------------------------------------------------------------------------
//
TBool CGetInputHandler::CommandAllowed()
    {
    LOG( SIMPLE, "GETINPUT: CGetInputHandler::CommandAllowed calling" )

    //Is This command Allowed.
    TBool response( ETrue );

     // If icon is provided without any text
    if ( ( 0 == iGetInputData.iText.Length() ) &&
        ( RSat::ENotSelfExplanatory == iGetInputData.iIconId.iQualifier ||
          RSat::ESelfExplanatory == iGetInputData.iIconId.iQualifier ) )
        {
        LOG( SIMPLE,
            "GETINPUT: CGetInputHandler::CommandAllowed icon without text" )
        iGetInputRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        iGetInputRsp.iInfoType = RSat::KNoAdditionalInfo;
        iGetInputRsp.iAdditionalInfo.Zero();
        response = EFalse;
        }
    // Check if phone is busy on calling and send response
    // to SIM if phone is busy.
    else if ( iUtils->SystemState().IsCallIncoming() )
        {
        LOG( SIMPLE,
            "GETINPUT: CGetInputHandler::CommandAllowed busy or calling" )
        // By default, this command does not have additional information
        // except in this case when phone is busy on calling
        iGetInputRsp.iInfoType = RSat::KMeProblem;
        iGetInputRsp.iAdditionalInfo.Zero();
        iGetInputRsp.iAdditionalInfo.Append( RSat::KMeBusyOnCall );
        iGetInputRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        // This command will not be allowed for now.
        response = EFalse;
        }
    // Set icon command flag whether icon data was received and set qualifier
    // to no icon id
    // To be removed when icons are allowed in this command
    else if ( ( RSat::ESelfExplanatory ==
        iGetInputData.iIconId.iQualifier ) ||
        ( RSat::ENotSelfExplanatory ==
        iGetInputData.iIconId.iQualifier ) )
        {
        LOG( SIMPLE,
            "GETINPUT: CGetInputHandler::CommandAllowed set no icon id" )
        iIconCommand = ETrue;
        iGetInputData.iIconId.iQualifier = RSat::ENoIconId;
        }
    else
        {
        iIconCommand = EFalse;
        }

    if ( !response )
        {
         LOG( SIMPLE,
            "GETINPUT: CGetInputHandler::CommandAllowed response false" )
        iGetInputRsp.SetPCmdNumber( iGetInputData.PCmdNumber() );
        TerminalRsp( RSat::EGetInput, iGetInputRspPckg );
        }

    LOG2( SIMPLE, "GETINPUT: CGetInputHandler::CommandAllowed exiting,\
          response: %d", response )
    return response;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Need for ui session.
// -----------------------------------------------------------------------------
//
TBool CGetInputHandler::NeedUiSession()
    {
    LOG( NORMAL, "GETINPUT: CGetInputHandler::NeedUiSession calling" )
    // Notify Cover UI if it's supported
    if ( iUtils->CoverUiSupported() )
        {
        LOG( NORMAL, "CGetInputHandler::NeedUiSession coverui supported" )
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::EGetInput;
        if ( iGetInputData.iText.Length() )
            {
            LOG( NORMAL, "CGetInputHandler::NeedUiSession EAlphaIdProvided" )
            medEventData.iAlphaId.iStatus = RSat::EAlphaIdProvided;
            medEventData.iAlphaId.iAlphaId.Copy( iGetInputData.iText );
            }
        else
            {
            medEventData.iAlphaId.iStatus = RSat::EAlphaIdNotPresent;
            }
        medEventData.iDuration.iNumOfUnits = KSatDefaultDuration;
        medEventData.iDuration.iTimeUnit = RSat::ESeconds;
        medEventData.iIconID = iGetInputData.iIconId;
        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        }
    LOG( NORMAL, "GETINPUT: CGetInputHandler::NeedUiSession exiting" )
    return ETrue;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when USAT API notifies that command.
// -----------------------------------------------------------------------------
//
void CGetInputHandler::HandleCommand()
    {
    LOG( SIMPLE, "GETINPUT: CGetInputHandler::HandleCommand calling" )

    // Send the response
    iUtils->NotifyEvent( MSatUtils::EGetInputExecuting );

    MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();
    uiSession->SendCommand(
        &iGetInputPckg,
        &iGetInputRspPckg,
        ESatSProactiveGetInput );

    LOG( SIMPLE, "GETINPUT: CGetInputHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Indicates the failure of launching ui client.
// -----------------------------------------------------------------------------
//
void CGetInputHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "GETINPUT: CGetInputHandler::UiLaunchFailed calling" )

    iGetInputRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
    iGetInputRsp.iInfoType = RSat::KMeProblem;
    iGetInputRsp.iAdditionalInfo.SetLength( 1 );
    iGetInputRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
    iGetInputRsp.SetPCmdNumber( iGetInputData.PCmdNumber() );
    TerminalRsp( RSat::EGetInput, iGetInputRspPckg );

    LOG( SIMPLE, "GETINPUT: CGetInputHandler::UiLaunchFailed exiting" )
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CGetInputHandler::CGetInputHandler() :
    CSatCommandHandler(),
    iGetInputData(),
    iGetInputPckg( iGetInputData ),
    iGetInputRsp(),
    iGetInputRspPckg( iGetInputRsp ),
    // To be removed when icons are allowed in this command
    iIconCommand( EFalse )
    {
    LOG( SIMPLE,
        "GETINPUT: CGetInputHandler::CGetInputHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CGetInputHandler::ConstructL()
    {
    LOG( SIMPLE, "GETINPUT: CGetInputHandler::ConstructL calling" )

    // Register service request handler for GetInput command
    iUtils->RegisterServiceRequestL(
        ESatSProactiveGetInput,
        ESatSProactiveGetInputResponse,
        this );

    LOG( SIMPLE, "GETINPUT: CGetInputHandler::ConstructL exiting" )
    }

