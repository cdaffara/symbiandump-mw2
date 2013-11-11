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
* Description:  Handles GetInkey command
*
*/


#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CGetInkeyHandler.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CGetInkeyHandler* CGetInkeyHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::NewL calling" )

    CGetInkeyHandler* self = new( ELeave ) CGetInkeyHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::NewL exiting" )
    return self;
    }
    
// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CGetInkeyHandler::~CGetInkeyHandler()
    {
    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::~CGetInkeyHandler calling" )

    Cancel();

    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::~CGetInkeyHandler exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatCommand.
// Response from the client.
// -----------------------------------------------------------------------------
//
void CGetInkeyHandler::ClientResponse()
    {
    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::ClientResponse calling" )

    if ( RSat::KPSessionTerminatedByUser == iGetInkeyRsp.iGeneralResult )
        {
        LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::ClientResponse close ui \
             session" )
        // Next SimSession end will close the ui session
        iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
        }
        
    // If command had icon data and was done succesfully, report that icon
    // was not shown
    // To be removed when icons are allowed in this command
    if ( ( RSat::KSuccess == iGetInkeyRsp.iGeneralResult ) &&
        iIconCommand )
        {
        LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::ClientResponse requested \
             icon not displayed" )
        iGetInkeyRsp.iGeneralResult =
                    RSat::KSuccessRequestedIconNotDisplayed;
        }

    iGetInkeyRsp.SetPCmdNumber( iGetInkeyData.PCmdNumber() );
    CSatCommandHandler::TerminalRsp( RSat::EGetInkey, iGetInkeyRspPckg );

    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CGetInkeyHandler::DoCancel()
    {
    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::DoCancel calling" )

    iUtils->USatAPI().NotifyGetInkeyCancel();

    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Requests the command notification.
// -----------------------------------------------------------------------------
//
void CGetInkeyHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iGetInkeyData) RSat::TGetInkeyV2();
    iGetInkeyRsp.iGeneralResult = RSat::KPSessionTerminatedByUser; // default

    iUtils->USatAPI().NotifyGetInkey( aStatus, iGetInkeyPckg );

    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Precheck before executing the command.
// -----------------------------------------------------------------------------
//
TBool CGetInkeyHandler::CommandAllowed()
    {
    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::CommandAllowed calling" )
    TBool allowed( ETrue );
    
    // If provided duration is missing time units
    if ( ( RSat::ENoDurationAvailable != iGetInkeyData.iDuration.iTimeUnit &&
        RSat::ETimeUnitNotSet != iGetInkeyData.iDuration.iTimeUnit ) &&
        0 == iGetInkeyData.iDuration.iNumOfUnits )
        {
        LOG( SIMPLE,
            "GETINKEY: CGetInkeyHandler::CommandAllowed invalid duration" )
        allowed = EFalse;
        }

    // If icon is provided without any text
    if ( ( 0 == iGetInkeyData.iText.Length() ) &&
         ( RSat::ENotSelfExplanatory == iGetInkeyData.iIconId.iQualifier ||
           RSat::ESelfExplanatory == iGetInkeyData.iIconId.iQualifier ) )
        {
        LOG( SIMPLE,
            "GETINKEY: CGetInkeyHandler::CommandAllowed icon w/o alpha id" )
        allowed = EFalse;
        }
    // Set icon command flag whether icon data was received and set qualifier
    // to no icon id
    // To be removed when icons are allowed in this command
    else if ( ( RSat::ESelfExplanatory ==
        iGetInkeyData.iIconId.iQualifier ) ||
        ( RSat::ENotSelfExplanatory ==
        iGetInkeyData.iIconId.iQualifier ) )
        {
        LOG( SIMPLE,
            "GETINKEY: CGetInkeyHandler::CommandAllowed set no icon id" )
        iIconCommand = ETrue;
        iGetInkeyData.iIconId.iQualifier = RSat::ENoIconId;
        }
    else
        {
        iIconCommand = EFalse;
        }

    if ( !allowed )
        {
        LOG( SIMPLE,
            "GETINKEY: CGetInkeyHandler::CommandAllowed allowed false" )
        iGetInkeyRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        iGetInkeyRsp.iInfoType = RSat::KNoAdditionalInfo;
        iGetInkeyRsp.iAdditionalInfo.Zero();        
        iGetInkeyRsp.SetPCmdNumber( iGetInkeyData.PCmdNumber() );
        // send terminal response
        TerminalRsp( RSat::EGetInkey, iGetInkeyRspPckg );   
        }

    LOG2( SIMPLE, "GETINKEY: CGetInkeyHandler::CommandAllowed exiting,\
          allowed: %d", allowed )
    return allowed;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Need for ui session.
// -----------------------------------------------------------------------------
//
TBool CGetInkeyHandler::NeedUiSession()
    {
    LOG( NORMAL, "GETINKEY: CGetInkeyHandler::NeedUiSession calling" )

    // Notify Cover UI if it's supported
    if ( iUtils->CoverUiSupported() )
        {
        LOG( NORMAL, "CGetInkeyHandler::NeedUiSession coverui supported" )
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::EGetInkey;
        if ( iGetInkeyData.iText.Length() )
            {
            LOG( NORMAL, "CGetInkeyHandler::NeedUiSession EAlphaIdProvided" )
            medEventData.iAlphaId.iStatus = RSat::EAlphaIdProvided;
            medEventData.iAlphaId.iAlphaId.Copy( iGetInkeyData.iText );
            }
        else
            {
            medEventData.iAlphaId.iStatus = RSat::EAlphaIdNotPresent;
            }
        // Duration not set, use KSatLongDuration    
        if ( RSat::ETimeUnitNotSet == iGetInkeyData.iDuration.iTimeUnit )
            {
            LOG( NORMAL, "CGetInkeyHandler::NeedUiSession Duration not set" )
            medEventData.iDuration.iTimeUnit = RSat::ESeconds;
            medEventData.iDuration.iNumOfUnits = KSatLongDuration;
            }
        else
            {
            medEventData.iDuration = iGetInkeyData.iDuration;
            }
            
        medEventData.iIconID = iGetInkeyData.iIconId;
        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        }

    LOG( NORMAL, "GETINKEY: CGetInkeyHandler::NeedUiSession exiting" )
    return ETrue;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when USAT API notifies that command.
// -----------------------------------------------------------------------------
//
void CGetInkeyHandler::HandleCommand()
    {
    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::HandleCommand calling" )

    iUtils->NotifyEvent( MSatUtils::EGetInkeyExecuting );

    MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();
    uiSession->SendCommand(
        &iGetInkeyPckg,
        &iGetInkeyRspPckg,
        ESatSProactiveGetInkey );

    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Indicates the failure of launching ui client.
// -----------------------------------------------------------------------------
//
void CGetInkeyHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::UiLaunchFailed calling" )

    iGetInkeyRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
    iGetInkeyRsp.iInfoType = RSat::KMeProblem;
    iGetInkeyRsp.iAdditionalInfo.SetLength( 1 );
    iGetInkeyRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
    iGetInkeyRsp.SetPCmdNumber( iGetInkeyData.PCmdNumber() );
    TerminalRsp( RSat::EGetInkey, iGetInkeyRspPckg );

    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::UiLaunchFailed exiting" )
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CGetInkeyHandler::CGetInkeyHandler() :
    CSatCommandHandler(),
    iGetInkeyData(),
    iGetInkeyPckg( iGetInkeyData ),
    iGetInkeyRsp(),
    iGetInkeyRspPckg( iGetInkeyRsp ),
    // To be removed when icons are allowed in this command
    iIconCommand( EFalse )
    {
    LOG( SIMPLE,
        "GETINKEY: CGetInkeyHandler::CGetInkeyHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CGetInkeyHandler::ConstructL()
    {
    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::ConstructL calling" )

    // Register service request handler for GetInkey command
    iUtils->RegisterServiceRequestL(
        ESatSProactiveGetInkey,
        ESatSProactiveGetInkeyResponse,
        this );

    LOG( SIMPLE, "GETINKEY: CGetInkeyHandler::ConstructL exiting" )
    }

