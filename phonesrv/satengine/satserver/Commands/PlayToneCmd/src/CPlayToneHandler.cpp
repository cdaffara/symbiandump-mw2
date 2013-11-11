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
* Description:  Handles PlayTone command
*
*/


#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatUiSession.h"
#include    "MSatSystemState.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CPlayToneHandler.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPlayToneHandler* CPlayToneHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::NewL calling" )

    CPlayToneHandler* self = new( ELeave ) CPlayToneHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CPlayToneHandler::~CPlayToneHandler()
    {
    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::~CPlayToneHandler calling" )

    Cancel();

    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::~CPlayToneHandler exiting" )
    }

// -----------------------------------------------------------------------------
// From class MSatCommand.
// Response from the client.
// -----------------------------------------------------------------------------
//
void CPlayToneHandler::ClientResponse()
    {
    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::ClientResponse calling" )

    iPlayToneRsp.SetPCmdNumber( iPlayToneData.PCmdNumber() );
    if ( RSat::KPSessionTerminatedByUser == iPlayToneRsp.iGeneralResult )
        {
        LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::ClientResponse \
             KPSessionTerminatedByUser" )
        // Next SimSession end will close the ui session.
        iUtils->NotifyEvent( MSatUtils::ESessionTerminatedByUser );
        }
        
    // If command had icon data and was done succesfully, report that icon
    // was not shown
    // To be removed when icons are allowed in this command
    if ( ( RSat::KSuccess == iPlayToneRsp.iGeneralResult ) &&
        iIconCommand )
        {
        LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::ClientResponse \
             requested icon not displayed" )
        iPlayToneRsp.iGeneralResult =
                    RSat::KSuccessRequestedIconNotDisplayed;
        }

    CSatCommandHandler::TerminalRsp( RSat::EPlayTone, iPlayToneRspPckg );

    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::ClientResponse exiting" )
    }

// -----------------------------------------------------------------------------
// From class CActive.
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CPlayToneHandler::DoCancel()
    {
    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::DoCancel calling" )

    iUtils->USatAPI().NotifyPlayToneCancel();

    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Requests the command notification.
// -----------------------------------------------------------------------------
//
void CPlayToneHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iPlayToneData) RSat::TPlayToneV2();
    iPlayToneRsp.iGeneralResult = RSat::KPSessionTerminatedByUser; // default

    iUtils->USatAPI().NotifyPlayTone( aStatus, iPlayTonePckg );

    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Precheck before executing the command.
// -----------------------------------------------------------------------------
//
TBool CPlayToneHandler::CommandAllowed()
    {
    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::CommandAllowed calling" )
    TBool allowed( ETrue );

    // This is used to determine zero length duration.
    const TBool zeroDuration( iPlayToneData.iDuration.iNumOfUnits == 0 );
    // Determines is it standard supervisory tone.
    const TBool standardSupervisoryTone(
        ( iPlayToneData.iTone >= RSat::KDialTone ) &&
        ( iPlayToneData.iTone <= RSat::KRingingTone ) );
    // Determines is it terminal proprietary tone
    const TBool terminalProprietaryTone(
        ( iPlayToneData.iTone >= RSat::KGeneralBeep ) &&
        ( iPlayToneData.iTone <= RSat::KUserSelectedToneIncomingSms ) );
    const TBool durationValid(
        ( RSat::ENoDurationAvailable != iPlayToneData.iDuration.iTimeUnit ) &&
        ( RSat::ETimeUnitNotSet != iPlayToneData.iDuration.iTimeUnit ) );

    // if icon data is received without alpha id,
    // command data is not understood.
    //lint -e{961} Else block not needed, allowed is then true.
    if ( ( RSat::EAlphaIdProvided != iPlayToneData.iAlphaId.iStatus ) &&
            ( RSat::ENotSelfExplanatory == iPlayToneData.iIconId.iQualifier ||
              RSat::ESelfExplanatory == iPlayToneData.iIconId.iQualifier ) )
        {
        allowed = EFalse;
        iPlayToneRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        iPlayToneRsp.iInfoType = RSat::KNoAdditionalInfo;
        iPlayToneRsp.iAdditionalInfo.Zero();
        LOG( SIMPLE,
            "PLAYTONE: CPlayToneHandler::CommandAllowed \
            icon received without alpha id" )
        }
    // If the tone is not standard or terminal proprietary OR it is
    // standard without duration, command is not allowed.
    else if ( iPlayToneData.iTone != RSat::KToneNotSet &&
            ( !( standardSupervisoryTone ^ terminalProprietaryTone ) ||
               ( zeroDuration && durationValid ) ) )
        {
        allowed = EFalse;
        iPlayToneRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        iPlayToneRsp.iInfoType = RSat::KNoAdditionalInfo;
        iPlayToneRsp.iAdditionalInfo.Zero();
        LOG( SIMPLE,
            "PLAYTONE: CPlayToneHandler::CommandAllowed \
            Not valid tone or duration" )
        }
    // If alpha id is not to be shown and 
    // tones are not ON command is not allowed.
    else if ( !iUtils->SystemState().IsWarningAndGameTonesOn() &&
             ( 0 == iPlayToneData.iAlphaId.iAlphaId.Length() ) )
        {
        allowed = EFalse;
        iPlayToneRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        iPlayToneRsp.iInfoType = RSat::KNoAdditionalInfo;
        iPlayToneRsp.iAdditionalInfo.Zero();
        LOG( SIMPLE,
            "PLAYTONE: CPlayToneHandler::CommandAllowed \
            Tones are OFF" )
        }
    // Set icon command flag whether icon data was received and set qualifier
    // to no icon id
    // To be removed when icons are allowed in this command
    else if ( ( RSat::ESelfExplanatory ==
        iPlayToneData.iIconId.iQualifier ) ||
        ( RSat::ENotSelfExplanatory ==
        iPlayToneData.iIconId.iQualifier ) )
        {
        LOG( SIMPLE,
            "PLAYTONE: CPlayToneHandler::CommandAllowed ENoIconId" )
        iIconCommand = ETrue;
        iPlayToneData.iIconId.iQualifier = RSat::ENoIconId;
        }
    else
        {
        LOG( SIMPLE,
            "PLAYTONE: CPlayToneHandler::CommandAllowed others" )
        iIconCommand = EFalse;
        }
        
    // if not allowed, send terminal response immediatelly.
    if ( !allowed )
        {
        LOG( SIMPLE,
            "PLAYTONE: CPlayToneHandler::CommandAllowed allowed false" )
        iPlayToneRsp.SetPCmdNumber( iPlayToneData.PCmdNumber() );
        // Send terminal response.
        TerminalRsp( RSat::EPlayTone, iPlayToneRspPckg );
        }

    LOG2( SIMPLE, "PLAYTONE: CPlayToneHandler::CommandAllowed exiting,\
          allowed: %d", allowed )
    return allowed;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Answers for need of UI session.
// -----------------------------------------------------------------------------
//
TBool CPlayToneHandler::NeedUiSession()
    {
    LOG( NORMAL, "PLAYTONE: CPlayToneHandler::NeedUiSession calling" )

    // Notify Cover UI if it's supported
    if ( iUtils->CoverUiSupported() )
        {
        LOG( NORMAL, "PLAYTONE: CPlayToneHandler::NeedUiSession \
             CoverUiSupported" )
        TSatCommandData medEventData;
        medEventData.iPCmdNumber = RSat::EPlayTone;
        medEventData.iAlphaId = iPlayToneData.iAlphaId;
        medEventData.iDuration = iPlayToneData.iDuration;
        medEventData.iIconID = iPlayToneData.iIconId;
        TSatCommandPckg tPckg( medEventData );
        iUtils->RaiseSatEvent( tPckg );
        }

    LOG( NORMAL, "PLAYTONE: CPlayToneHandler::NeedUiSession exiting" )
    return ETrue;
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Called when USAT API notifies that command.
// -----------------------------------------------------------------------------
//
void CPlayToneHandler::HandleCommand()
    {
    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::HandleCommand calling" )

    iUtils->NotifyEvent( MSatUtils::EPlayToneExecuting );

    MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();
    uiSession->SendCommand(
        &iPlayTonePckg,
        &iPlayToneRspPckg,
        ESatSProactivePlayTone );

    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// From class CSatCommandHandler.
// Indicates the failure of launching ui client.
// -----------------------------------------------------------------------------
//
void CPlayToneHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::UiLaunchFailed calling" )

    iPlayToneRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
    iPlayToneRsp.iInfoType = RSat::KMeProblem;
    iPlayToneRsp.iAdditionalInfo.SetLength( 1 );
    iPlayToneRsp.iAdditionalInfo[0] = RSat::KNoSpecificMeProblem;
    iPlayToneRsp.SetPCmdNumber( iPlayToneData.PCmdNumber() );
    TerminalRsp( RSat::EPlayTone, iPlayToneRspPckg );

    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::UiLaunchFailed exiting" )
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CPlayToneHandler::CPlayToneHandler() :
    CSatCommandHandler(),
    iPlayToneData(),
    iPlayTonePckg( iPlayToneData ),
    iPlayToneRsp(),
    iPlayToneRspPckg( iPlayToneRsp ),
    // To be removed when icons are allowed in this command
    iIconCommand( EFalse )
    {
    LOG( SIMPLE,
        "PLAYTONE: CPlayToneHandler::CPlayToneHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPlayToneHandler::ConstructL()
    {
    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::ConstructL calling" )

    // Register service request handler for PlayTone command.
    iUtils->RegisterServiceRequestL(
        ESatSProactivePlayTone,
        ESatSProactivePlayToneResponse,
        this );

    LOG( SIMPLE, "PLAYTONE: CPlayToneHandler::ConstructL exiting" )
    }

