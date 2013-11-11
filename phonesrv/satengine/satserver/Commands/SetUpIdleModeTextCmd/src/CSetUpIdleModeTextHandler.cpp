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
* Description:  Handles SetUpIdleModeText command
*
*/


#include    <e32property.h>
#include    <centralrepository.h>
#include    <networkhandlingdomainpskeys.h>
#include    "SATPrivateCRKeys.h"
#include    "MSatApi.h"
#include    "MSatUtils.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "MSatSystemState.h"
#include    "CSetUpIdleModeTextHandler.h"
#include    "SatLog.h"

// This was agreed with Idle.
const TInt KNoIcon = -1;

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSetUpIdleModeTextHandler::CSetUpIdleModeTextHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSetUpIdleModeTextHandler::CSetUpIdleModeTextHandler() :
    CSatCommandHandler(),
    iSetUpIdleModeTextData(),
    iSetUpIdleModeTextPckg( iSetUpIdleModeTextData ),
    iSetUpIdleModeTextRsp(),
    iSetUpIdleModeTextRspPckg( iSetUpIdleModeTextRsp ),
    iSimResetExecuting( EFalse ),
    iLastValidText( KNullDesC ),
    iLastValidIconId( KNoIcon ),
    iLastValidIconQualifier( RSat::EIconQualifierNotSet ),
    iHasHomezoneIndicator( 0 )
    {
    LOG( SIMPLE, "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler::\
        CSetUpIdleModeTextHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpIdleModeTextHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSetUpIdleModeTextHandler::ConstructL()
    {
    LOG( SIMPLE, "SETUPIDLEMODETEXT: \
        CSetUpIdleModeTextHandler::ConstructL calling" )

    // Register to listen ESimReset execution.
    iUtils->RegisterL( this, MSatUtils::ESimResetCalled );
    iUtils->RegisterL( this, MSatUtils::ESimResetCancelled );
    iUtils->RegisterL( this, MSatUtils::ESimInitDone );
    // Get the homezone indicator flag from the Central Repository.
    CRepository* repo = CRepository::NewLC( KCRUidSatServer );
    TInt errorCode = repo->Get( KSatHomeZoneIconVisible, iHasHomezoneIndicator );
    LOG2( NORMAL, "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler::ConstructL \
        get homezone indicator flag and return: %d", errorCode )
    CleanupStack::PopAndDestroy( repo );

    LOG( SIMPLE, "SETUPIDLEMODETEXT: \
        CSetUpIdleModeTextHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpIdleModeTextHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSetUpIdleModeTextHandler* CSetUpIdleModeTextHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE,
        "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler::NewL calling" )

    CSetUpIdleModeTextHandler* self = new( ELeave ) CSetUpIdleModeTextHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE,
        "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSetUpIdleModeTextHandler::~CSetUpIdleModeTextHandler
// Destructor.
// -----------------------------------------------------------------------------
//
CSetUpIdleModeTextHandler::~CSetUpIdleModeTextHandler()
    {
    LOG( SIMPLE, "SETUPIDLEMODETEXT: \
        CSetUpIdleModeTextHandler::~CSetUpIdleModeTextHandler calling" )

    Cancel();

    LOG( SIMPLE, "SETUPIDLEMODETEXT: \
        CSetUpIdleModeTextHandler::~CSetUpIdleModeTextHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpIdleModeTextHandler::Event
// -----------------------------------------------------------------------------
//
void CSetUpIdleModeTextHandler::Event( TInt aEvent )
    {
    LOG( SIMPLE,
        "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler::Event calling" )
    // Check the response
    switch ( aEvent )
        {
        case MSatUtils::ESimResetCalled:
            {
            LOG( NORMAL, "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler \
                 ESimResetCalled")
            iSimResetExecuting = ETrue;
            }
            //lint -fallthrough intended here

        case MSatUtils::ESimInitDone:
            {
            LOG( NORMAL, "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler \
                remove idle mode text")

            // Remove the idle mode text and icon.
            // Note no response now, responseOk remains EFalse.
            iUtils->SystemState().WriteSetIdleModeText(
                KNullDesC, KNoIcon, RSat::ENoIconId );
            // Remove the homezone indicator.
            if ( iHasHomezoneIndicator && iLastValidText.Length() )
                {
                TInt errorCode = RProperty::Set(
                        KPSUidNetworkInfo,
                        KNWHomeZoneStatus,
                        ENWNone );
                LOG2( NORMAL, "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler \
                    remove the homezone indicator and return %d", errorCode )
                }
            break;
            }

        case MSatUtils::ESimResetCancelled:
            {
            LOG( NORMAL, "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler \
                ESimResetCancelled")

            // Restore the idle mode text and icon.
            // Note no response now, responseOk remains EFalse.
            if ( iSimResetExecuting )
                {
                LOG( NORMAL, "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler \
                iSimResetExecuting true")
                iUtils->SystemState().WriteSetIdleModeText(
                    iLastValidText, iLastValidIconId, iLastValidIconQualifier );
                // Restore the homezone indicator as SIM reset was cancelled.
                if ( iHasHomezoneIndicator && iLastValidText.Length() )
                    {
                    TInt errorCode = RProperty::Set(
                            KPSUidNetworkInfo,
                            KNWHomeZoneStatus,
                            ENWHomeZone );
                    LOG2( NORMAL, "SETUPIDLEMODETEXT: \
                    CSetUpIdleModeTextHandler restore the homezone indicator \
                    and return %d", errorCode )
                    }
                iSimResetExecuting = EFalse;
                }
            break;
            }

        default:
            {
            CSatCommandHandler::Event( aEvent );
            break;
            }
        }
    LOG( SIMPLE,
        "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpIdleModeTextHandler::DoCancel
// Cancels the sat request.
// -----------------------------------------------------------------------------
//
void CSetUpIdleModeTextHandler::DoCancel()
    {
    LOG( SIMPLE,
        "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler::DoCancel calling" )

    iUtils->USatAPI().NotifySetUpIdleModeTextCancel();

    LOG( SIMPLE,
        "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpIdleModeTextHandler::IssueUSATRequest
// -----------------------------------------------------------------------------
//
void CSetUpIdleModeTextHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "SETUPIDLEMODETEXT: \
        CSetUpIdleModeTextHandler::IssueUSATRequest calling" )

    // Clear the IPC package.
    new (&iSetUpIdleModeTextData) RSat::TSetUpIdleModeTextV1();

    iUtils->USatAPI().NotifySetUpIdleModeText(
        aStatus, iSetUpIdleModeTextPckg );

    LOG( SIMPLE, "SETUPIDLEMODETEXT: \
        CSetUpIdleModeTextHandler::IssueUSATRequest exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpIdleModeTextHandler::CommandAllowed
// -----------------------------------------------------------------------------
//
TBool CSetUpIdleModeTextHandler::CommandAllowed()
    {
    LOG( SIMPLE, "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler::\
        CommandAllowed calling" )
    TBool commandAllowed( ETrue );

    if ( ( 0 == iSetUpIdleModeTextData.iText.Length() ) &&
         ( RSat::ENotSelfExplanatory ==
            iSetUpIdleModeTextData.iIconId.iQualifier ||
           RSat::ESelfExplanatory ==
            iSetUpIdleModeTextData.iIconId.iQualifier ) )
        {
        iSetUpIdleModeTextRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        iSetUpIdleModeTextRsp.iInfoType = RSat::KNoAdditionalInfo;
        iSetUpIdleModeTextRsp.iAdditionalInfo.Zero();
        iSetUpIdleModeTextRsp.SetPCmdNumber(
            iSetUpIdleModeTextData.PCmdNumber() );
        TerminalRsp( RSat::ESetUpIdleModeText, iSetUpIdleModeTextRspPckg );
        commandAllowed = EFalse;
        LOG( NORMAL,
            "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler \
            CommandAllowed icon received without alpha id" )
        }

    LOG2( SIMPLE, "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler::\
        CommandAllowed exiting,commandAllowed: %d", commandAllowed )
    return commandAllowed;
    }

// -----------------------------------------------------------------------------
// CSetUpIdleModeTextHandler::NeedUiSession
// -----------------------------------------------------------------------------
//
TBool CSetUpIdleModeTextHandler::NeedUiSession()
    {
    LOG( SIMPLE, "SETUPIDLEMODETEXT: \
        CSetUpIdleModeTextHandler::NeedUiSession calling - exiting" )
    return EFalse; // No need for UI Session.
    }

// -----------------------------------------------------------------------------
// CSetUpIdleModeTextHandler::HandleCommand
// -----------------------------------------------------------------------------
//
void CSetUpIdleModeTextHandler::HandleCommand()
    {
    LOG( SIMPLE,
        "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler::HandleCommand calling" )

    iUtils->NotifyEvent( MSatUtils::ESetUpIdleModeTextExecuting );

    // The idle mode text is not written to shared data if
    // Sim indicates the removal of idle mode text.
    TPtrC idleModeText( iSetUpIdleModeTextData.iText );
    TInt iconId( iSetUpIdleModeTextData.iIconId.iIdentifier );

    // Get the icon description.
    const RSat::TIconQualifier qualifier
        ( iSetUpIdleModeTextData.iIconId.iQualifier );

    if ( RSat::ERemoveExistingIdleModeText == iSetUpIdleModeTextData.iType )
        {
        LOG( SIMPLE,
        "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler::HandleCommand \
        ERemoveExistingIdleModeText" )
        idleModeText.Set( KNullDesC );
        iconId = KErrNotFound;
        }
    else
        {
        if ( ( RSat::ENoIconId == qualifier ) ||
             ( RSat::EIconQualifierNotSet == qualifier ) )
            {
            LOG( SIMPLE,
            "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler::HandleCommand \
            KErrNotFound" )
            // Icon information not provided in command.
            iconId = KErrNotFound;
            }
        }

    // Write text and icon id to P&S.
    const TInt err(
        iUtils->SystemState().WriteSetIdleModeText(
            idleModeText, iconId, qualifier ) );

    iSetUpIdleModeTextRsp.iInfoType = RSat::KNoAdditionalInfo;
    iSetUpIdleModeTextRsp.iAdditionalInfo.Zero();
    iSetUpIdleModeTextRsp.SetPCmdNumber(
        iSetUpIdleModeTextData.PCmdNumber() );

    // Send the terminal response now, if there was an error.
    if ( KErrNone != err )
        {
        LOG( NORMAL,
            "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler::HandleCommand \
            Error, Sending Terminal Response" )
        iSetUpIdleModeTextRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;

        // Send the response.
        TerminalRsp( RSat::ESetUpIdleModeText, iSetUpIdleModeTextRspPckg );
        }
    else
        {
        // Save data for restore.
        iSimResetExecuting = EFalse;
        iLastValidText = idleModeText;
        iLastValidIconId = iconId;
        iLastValidIconQualifier = qualifier;
        // Show or remove the homezone indicator.
        if ( iHasHomezoneIndicator )
            {
            if ( iLastValidText.Length() )
                {
                TInt errorCode = RProperty::Set(
                        KPSUidNetworkInfo,
                        KNWHomeZoneStatus,
                        ENWHomeZone );
                LOG2( NORMAL, "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler \
                    show the homezone indicator and return %d", errorCode )
                }
            else 
                {
                TInt errorCode = RProperty::Set(
                        KPSUidNetworkInfo,
                        KNWHomeZoneStatus,
                        ENWNone );
                LOG2( NORMAL, "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler \
                    remove the homezone indicator and return %d", errorCode )
                }
            }
        
        iSetUpIdleModeTextRsp.iGeneralResult = RSat::KSuccess;
        TerminalRsp( RSat::ESetUpIdleModeText, iSetUpIdleModeTextRspPckg );
        }

    LOG( SIMPLE,
        "SETUPIDLEMODETEXT: CSetUpIdleModeTextHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpIdleModeTextHandler::UiLaunchFailed
// -----------------------------------------------------------------------------
//
void CSetUpIdleModeTextHandler::UiLaunchFailed()
    {
    LOG( SIMPLE, "SETUPIDLEMODETEXT: \
        CSetUpIdleModeTextHandler::UiLaunchFailed calling - exiting" )
    // We don't need UI. This is derived function from CSatSCommandHandler,
    // so this function must exist, even if it is never called.
    }
