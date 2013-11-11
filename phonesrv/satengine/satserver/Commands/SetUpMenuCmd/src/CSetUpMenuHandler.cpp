/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles the SetUpMenu command from SIM.
*
*/


#include    <MSatShellController.h>
#include    "MSatApi.h"
#include    "MSatUiSession.h"
#include    "SatSOpcodes.h"
#include    "MSatSUiClientHandler.h"
#include    "CSetUpMenuHandler.h"
#include    "SATPrivateCRKeys.h"
#include    "SatLog.h"

const TInt KMaxSatAppName = 30;

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::CSetUpMenuHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSetUpMenuHandler::CSetUpMenuHandler() :
    CSatCommandHandler(),
    iSetUpMenuData(),
    iSetUpMenuPckg( iSetUpMenuData ),
    iArrivedSetUpMenuData(),
    iArrivedSetUpMenuPckg( iArrivedSetUpMenuData ),
    iSatAppNameUpdated( EFalse ),
    iSatServiceAvailable( EFalse ),
    // To be removed when icons are allowed in this command
    iIconCommand( EFalse )
    {
    LOG( SIMPLE,
        "SETUPMENU: CSetUpMenuHandler::CSetUpMenuHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSetUpMenuHandler::ConstructL()
    {
    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::ConstructL calling" )

    // Register Service request handler for this command handler.
    iUtils->RegisterServiceRequestL(
        ESatSProactiveSetUpMenu,
        ESatSNone,
        this );

    // Remove the sat ui client icon from the shell, otherwise
    // sat icon remains in shell once set in shell.
    iUtils->SatUiHandler().ShellController().RemoveSatUiL();

    // When sim session arrives, setupmenu may need to be displayed.
    iUtils->RegisterL( this, MSatUtils::ESimSessionEndExecuting );
    iUtils->RegisterL( this, MSatUtils::ESatUiLaunched );
    iUtils->RegisterL( this, MSatUtils::ESetUpMenuNeeded );
    iUtils->RegisterL( this, MSatUtils::EApplNameUpdated );
    iUtils->RegisterL( this, MSatUtils::ESimResetCalled );
    iUtils->RegisterL( this, MSatUtils::ERemoveSatUiCalled );
    iUtils->RegisterL( this, MSatUtils::EAddSatUiCalled );
    iUtils->RegisterL( this, MSatUtils::ECallControlDone );

    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSetUpMenuHandler* CSetUpMenuHandler::NewL( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::NewL calling" )

    CSetUpMenuHandler* self = new( ELeave ) CSetUpMenuHandler;

    CleanupStack::PushL( self );
    self->BaseConstructL( aUtils ); // Need to be called before ConstructL
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::NewL exiting" )
    return self;
    }


// Destructor
CSetUpMenuHandler::~CSetUpMenuHandler()
    {
    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::~CSetUpMenuHandler calling" )

    Cancel();

    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::~CSetUpMenuHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::ClientResponse
// -----------------------------------------------------------------------------
//
void CSetUpMenuHandler::ClientResponse()
    {
    // This is needed to prevent Panic when UI is closed right after start
    }

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::Event
// Event notification
// -----------------------------------------------------------------------------
//
void CSetUpMenuHandler::Event( TInt aEvent )
    {
    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::Event calling" )

    switch ( aEvent )
        {
        // Sim session has arrived from SIM or ui launched
        case MSatUtils::ESatUiLaunched:
            {
            LOG( NORMAL, "SETUPMENU:   Event: ESatUiLaunched" )
            // Send SetUpMenu to ui client if ui client is running,
            // ui client is launced by user and the set up menu data
            // has arrived from SIM.
            if ( iSetUpMenuArrived )
                {
                LOG( NORMAL, "SETUPMENU:   Event: iSetUpMenuArrived true" )
                SendSetUpMenuToClient();
                }
            else
                {
                SendEmptySetupMenuToClient();
                }

            break;
            }

        case MSatUtils::ESetUpMenuNeeded:
            {
            LOG( NORMAL, "SETUPMENU:   Event: ESetUpMenuNeeded" )
            // Some command has finished executing and needs SetUpMenu.
            SendSetUpMenuToClient();
            break;
            }

        case MSatUtils::EApplNameUpdated:
            {
            LOG( NORMAL, "SETUPMENU:   Event: EApplNameUpdated" )
            // Update SetUpMenu to ui client if ui client is running,
            // ui client is launced by user and the set up menu data
            // has arrived from SIM.
            TRAPD( err, UpdateSetUpMenuToClientL() );
            if ( KErrNone != err )
                {
                LOG2( SIMPLE, "SETUPMENU:   Error: %i", err )
                }            
            break;
            }

        case MSatUtils::ESimSessionEndExecuting:
            {
            LOG( NORMAL, "SETUPMENU:   Event: ESimSessionEndExecuting" )
            // If the sim session end is only command active currently
            // then send the setupmenu to client.
            if ( 1 == iUtils->NumberOfExecutingCommandHandlers() )
                {
                LOG( NORMAL, "SETUPMENU:   Event: setupmenu" )
                SendSetUpMenuToClient();
                }
            break;
            }

        case MSatUtils::ESimResetCalled:
            {
            LOG( NORMAL, "SETUPMENU:   Event: ESimResetCalled" )
            // Close SatUi and remove Sat icon from application shell
            MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();
            if ( uiSession )
                {
                LOG( NORMAL, "SETUPMENU:   Event: CloseUiSession" )
                uiSession->CloseUiSession();
                }
            TRAP_IGNORE( RemoveIconFromShellL() )
            break;
            }

        case MSatUtils::EAddSatUiCalled:
            {
            LOG( NORMAL, "SETUPMENU:   Event: EAddSatUiCalled" )
            iSatServiceAvailable = ETrue;
            if ( iSetUpMenuArrived )
                {
                LOG( NORMAL, "SETUPMENU:   Event: iSetUpMenuArrived true" )
                TRAP_IGNORE( AddIconToShellL(
                    iArrivedSetUpMenuData.iAlphaId.iAlphaId,
                    iArrivedSetUpMenuData.iIconId ) )
                }
            break;
            }

        case MSatUtils::ERemoveSatUiCalled:
            {
            iSatServiceAvailable = EFalse;
            LOG( NORMAL, "SETUPMENU:   Event: ERemoveSatUiCalled" )
            // Close SAT UI and remove SAT icon from Application Shell.
            MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();
            if ( uiSession )
                {
                LOG( NORMAL, "SETUPMENU:   Event: CloseUiSession" )
                uiSession->CloseUiSession();
                }
            TRAP_IGNORE( RemoveIconFromShellL() )
            break;
            }

        case MSatUtils::ECallControlDone:
            {
            // If all commands end up, send the setupmenu to client.
            if ( 0 == iUtils->NumberOfExecutingCommandHandlers() )
                {
                LOG( NORMAL, "SETUPMENU:   Event: setupmenu" )
                SendSetUpMenuToClient();
                }
            break;
            }

        default:
            {
            LOG2( NORMAL, "SETUPMENU:   Unexpected event: %i", aEvent )
            CSatCommandHandler::Event( aEvent );
            }
        }

    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::DoCancel
// Cancels the SetUpMenu notification from SIM.
// -----------------------------------------------------------------------------
//
void CSetUpMenuHandler::DoCancel()
    {
    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::DoCancel calling" )

    iUtils->USatAPI().NotifySetUpMenuCancel();

    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::AddIconToShellL
// -----------------------------------------------------------------------------
//
void CSetUpMenuHandler::AddIconToShellL(
    const TDesC& aAlphaId, // Alpha id of SetUpMenu command
    const RSat::TIconId& aIconId ) // Icon id of SetUpMenu command
    {
    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::AddIconToShellL calling" )
    TPtrC name( aAlphaId );

    if ( 0 != aAlphaId.Length() )
        {
        LOG( SIMPLE, 
        "SETUPMENU: CSetUpMenuHandler::AddIconToShellL aAlphaId not null" )
        // Set new sat app name.
        iUtils->SetSatAppNameL( aAlphaId );
        }
    else
        {
        LOG( SIMPLE, 
        "SETUPMENU: CSetUpMenuHandler::AddIconToShellL aAlphaId null" )
        iUtils->RestoreSatAppNameL();
        name.Set( iUtils->SatAppName() );
        }

    // Menu application needs only a short name.
    if ( iUtils->SatAppName().Length() > KMaxSatAppName )
        {
        LOG( SIMPLE, 
        "SETUPMENU: CSetUpMenuHandler::AddIconToShellL AppName too long" )
        name.Set( iUtils->SatAppName().Left( KMaxSatAppName ) );
        }
    else
        {
        name.Set( iUtils->SatAppName() );
        }

    // Check if there is icon available.
    MSatShellController& controller = iUtils->SatUiHandler().ShellController();
    if ( ( RSat::ENoIconId == aIconId.iQualifier ) ||
         ( RSat::EIconQualifierNotSet == aIconId.iQualifier ) )
        {
        LOG( SIMPLE, 
        "SETUPMENU: CSetUpMenuHandler::AddIconToShellL no icon" )
        controller.AddSatUiL( name );
        }
    else
        {
        controller.AddSatUiL( name, aIconId.iIdentifier );
        }

    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::AddIconToShellL exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::RemoveIconFromShellL
// -----------------------------------------------------------------------------
//
void CSetUpMenuHandler::RemoveIconFromShellL()
    {
    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::RemoveIconFromShellL calling" )

    iUtils->SatUiHandler().ShellController().RemoveSatUiL();

    // Restore the original name of SAT application.
    iUtils->RestoreSatAppNameL();

    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::RemoveIconFromShellL exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::SendSetUpMenuToClient
// -----------------------------------------------------------------------------
//
void CSetUpMenuHandler::SendSetUpMenuToClient()
    {
    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::SendSetUpMenuToClient calling" )

    MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();
    if ( uiSession &&
         iUtils->SatUiHandler().UiLaunchedByUser() &&
         iSetUpMenuArrived )
        {
        LOG( SIMPLE, 
        "SETUPMENU: CSetUpMenuHandler::SendSetUpMenuToClient setupmenu" )

        // Does not update SIM application name when it is restored and service
        // is not available.
        if ( iSatServiceAvailable )
            {
            LOG( SIMPLE, 
            "SETUPMENU: CSetUpMenuHandler::SendSetUpMenuToClient \
            iSatServiceAvailable true" )
            // When ME is launched with special SIM and SetUpMenu made with
            // different name. Then also SetUpMenu data update is needed
            // when Menu name was already OK.
            iArrivedSetUpMenuData.iAlphaId.iAlphaId.Zero();
            iArrivedSetUpMenuData.iAlphaId.iAlphaId.Copy(
                iUtils->SatAppName() );
            }

        // Client does not respond to setupmenu cmd, therefore NULL.
        uiSession->SendCommand(
            &iArrivedSetUpMenuPckg,
            NULL,
            ESatSProactiveSetUpMenu );
        }

    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::SendSetUpMenuToClient exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::SendEmptySetupMenuToClient
// -----------------------------------------------------------------------------
//
void CSetUpMenuHandler::SendEmptySetupMenuToClient()
    {
    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::SendEmptySetupMenuToClient\
     calling" )

    MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();
    if ( uiSession &&
         iUtils->SatUiHandler().UiLaunchedByUser() )
        {
        LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::SendEmptySetupMenuToClient\
        send setupmenu" )
        // Client does not respond to setupmenu cmd, therefore NULL.
        uiSession->SendCommand(
            &iSetUpMenuPckg,
            NULL,
            ESatSProactiveSetUpMenu );
        }

    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::SendEmptySetupMenuToClient\
     exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::UpdateSetUpMenuToClientL
// -----------------------------------------------------------------------------
//
void CSetUpMenuHandler::UpdateSetUpMenuToClientL()
    {
    LOG( SIMPLE,
        "SETUPMENU: CSetUpMenuHandler::UpdateSetUpMenuToClientL calling" )

    iSatAppNameUpdated = ETrue;
    if ( iSetUpMenuArrived )
        {
        // SetUpMenu application name update is needed.
        iArrivedSetUpMenuData.iAlphaId.iAlphaId.Zero();
        iArrivedSetUpMenuData.iAlphaId.iAlphaId.Append( iUtils->SatAppName() );
        LOG( NORMAL, "SETUPMENU:   Name updated" )

        // Restore icon after update.
        AddIconToShellL(
            iArrivedSetUpMenuData.iAlphaId.iAlphaId,
            iArrivedSetUpMenuData.iIconId );

        // Update display, when ui is launched.
        MSatUiSession* uiSession = iUtils->SatUiHandler().UiSession();
        if ( uiSession && iUtils->SatUiHandler().UiLaunchedByUser() )
            {
            LOG( SIMPLE,
            "SETUPMENU: CSetUpMenuHandler::UpdateSetUpMenuToClientL \
            UiLaunchedByUser" )
            // Update it only when it is not empty.
            if ( 0 != iArrivedSetUpMenuData.NumberOfItems() )
                {
                LOG( SIMPLE,
                "SETUPMENU: CSetUpMenuHandler::UpdateSetUpMenuToClientL \
                item not empty" )
                // Send update to ui.
                // Client does not respond to setupmenu cmd, therefore NULL.
                uiSession->SendCommand(
                    &iArrivedSetUpMenuPckg,
                    NULL,
                    ESatSProactiveSetUpMenu );
                }
            }
        }

    LOG( SIMPLE,
        "SETUPMENU: CSetUpMenuHandler::UpdateSetUpMenuToClientL exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::IssueUSATRequest
// Makes the request of SetUpMenu from SIM
// -----------------------------------------------------------------------------
//
void CSetUpMenuHandler::IssueUSATRequest( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::IssueUSATRequest calling" )

    // Without reset, old menu items stay in the list.
    iSetUpMenuData.Reset();

#ifdef __WINS__
    // Empty the IPC data
    RSat::TSetUpMenuV2 temp;
    iSetUpMenuData = temp;
#endif

    iUtils->USatAPI().NotifySetUpMenu( aStatus, iSetUpMenuPckg );

    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::IssueUSATRequestx exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::CommandAllowed
// SetUpMenu is allowed always.
// -----------------------------------------------------------------------------
//
TBool CSetUpMenuHandler::CommandAllowed()
    {
    LOG( SIMPLE,
        "SETUPMENU: CSetUpMenuHandler::CommandAllowed calling" )

    TBool commandAllowed( ETrue );
    TBool nullText( EFalse );
    TInt err ( KErrNone );
    RSat::TItem item;
    TUint numberOfItems( iSetUpMenuData.NumberOfItems() );
    TBool titleIcon(
        ( RSat::ENotSelfExplanatory == iSetUpMenuData.iIconId.iQualifier ) ||
        ( RSat::ESelfExplanatory == iSetUpMenuData.iIconId.iQualifier ) );
    TBool itemIcons(
        ( RSat::ENotSelfExplanatory == iSetUpMenuData.iIconListQualifier ) ||
        ( RSat::ESelfExplanatory == iSetUpMenuData.iIconListQualifier ) );
    RSat::TSetUpMenuRspV1 setUpMenuRsp;
    RSat::TSetUpMenuRspV1Pckg setUpMenuRspPckg( setUpMenuRsp );

    // Check if some item does not have text
    for ( TUint count = 1; ( count <= numberOfItems ) && !nullText &&
        ( KErrNone == err ); count++ )
        {
        err = iSetUpMenuData.GetItem( count, item );

        if ( ( KErrNone == err ) && ( 0 == item.iItemString.Length() ) )
            {
            LOG( SIMPLE,
            "SETUPMENU: CSetUpMenuHandler::CommandAllowed item null" )
            nullText = ETrue;
            }
        }
    LOG2( SIMPLE,
    "SETUPMENU: CSetUpMenuHandler::CommandAllowed numberOfItems:%d",
    numberOfItems )
    // If icon is provided without any text
    if ( ( ( RSat::EAlphaIdProvided != iSetUpMenuData.iAlphaId.iStatus ) &&
             titleIcon ) || ( nullText && itemIcons ) )
        {
        setUpMenuRsp.iGeneralResult = RSat::KCmdDataNotUnderstood;
        setUpMenuRsp.iInfoType = RSat::KNoAdditionalInfo;
        setUpMenuRsp.iAdditionalInfo.Zero();
        setUpMenuRsp.SetPCmdNumber( setUpMenuRsp.PCmdNumber() );
        TerminalRsp( RSat::ESetUpMenu, setUpMenuRspPckg );
        commandAllowed = EFalse;
        LOG( NORMAL, "SETUPMENU: CSetUpMenuHandler::\
            CommandAllowed icon received without alpha id" )
        }
    // Set icon command flag whether icon data was received and set qualifier
    // to no icon id
    // To be removed when icons are allowed in this command
    else if ( !titleIcon && !itemIcons )
        {
        LOG( SIMPLE,
        "SETUPMENU: CSetUpMenuHandler::CommandAllowed no icon" )
        iIconCommand = EFalse;
        }
    else
        {
        iIconCommand = ETrue;

        if ( titleIcon )
            {
            LOG( SIMPLE,
            "SETUPMENU: CSetUpMenuHandler::CommandAllowed titleIcon true" )
            iSetUpMenuData.iIconId.iQualifier = RSat::ENoIconId;
            }

        if ( itemIcons )
            {
            LOG( SIMPLE,
            "SETUPMENU: CSetUpMenuHandler::CommandAllowed itemIcons true" )
            iSetUpMenuData.iIconListQualifier = RSat::ENoIconId;
            }
        }

    LOG2( SIMPLE,
    "SETUPMENU: CSetUpMenuHandler::CommandAllowed exiting,commandAllowed: %d",
    commandAllowed )
    return commandAllowed;
    }

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::NeedUiSession
// SetUpMenu command itself does not need ui session.
// -----------------------------------------------------------------------------
//
TBool CSetUpMenuHandler::NeedUiSession()
    {
    LOG( SIMPLE,
        "SETUPMENU: CSetUpMenuHandler::NeedUiSession calling - exiting" )
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::HandleCommand
// Handles the SetUpMenu proactive command.
// -----------------------------------------------------------------------------
//
void CSetUpMenuHandler::HandleCommand()
    {
    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::HandleCommand calling" )

    // This could be moved to base class.
    iUtils->NotifyEvent( MSatUtils::ESetUpMenuExecuting );

    iArrivedSetUpMenuData = iSetUpMenuData;

    TInt err( KErrNone );

    if ( iSetUpMenuData.IsRemoveMenu() ||
         ( 0 == iSetUpMenuData.NumberOfItems() ) )
        {
        LOG( SIMPLE, 
        "SETUPMENU: CSetUpMenuHandler::HandleCommand remove icon" )
        TRAP( err, RemoveIconFromShellL() );
        }
    else
        {
        // If name has been changed prior launch of SetUpMenu, update name.
        if ( iSatAppNameUpdated )
            {
            LOG( SIMPLE, 
            "SETUPMENU: CSetUpMenuHandler::HandleCommand iSatAppNameUpdated" )
            iArrivedSetUpMenuData.iAlphaId.iAlphaId.Zero();
            }

        // Set SAT name to alpha id if alpha ID is zero.
        if ( 0 == iArrivedSetUpMenuData.iAlphaId.iAlphaId.Length() )
            {
            TRAP( err, iUtils->RestoreSatAppNameL() );
            iArrivedSetUpMenuData.iAlphaId.iAlphaId.Append(
                iUtils->SatAppName() );
            LOG( NORMAL, 
            "SETUPMENU: CSetUpMenuHandler::HandleCommand Name updated" )
            }        
        //Rentain the title of "SIM card application" for special card
        else
            {
            // Set new sat app name.
            TRAP( err, iUtils->SetSatAppNameL( 
                               iArrivedSetUpMenuData.iAlphaId.iAlphaId ) );            
            iArrivedSetUpMenuData.iAlphaId.iAlphaId.Zero();
            iArrivedSetUpMenuData.iAlphaId.iAlphaId.Copy(
                iUtils->SatAppName() );            
            }
            
        MSatUiSession* session = iUtils->SatUiHandler().UiSession();
        if ( session )
            {
            LOG( SIMPLE, 
            "SETUPMENU: CSetUpMenuHandler::HandleCommand session true" )
            // Send the setupmenu to ui client.
            session->SendCommand(
                &iArrivedSetUpMenuPckg,
                NULL,
                ESatSProactiveSetUpMenu );
            }

        if ( KErrNone == err )
            {
            LOG( SIMPLE, 
            "SETUPMENU: CSetUpMenuHandler::HandleCommand AddIconToShellL" )
            TRAP( err, AddIconToShellL(
                iArrivedSetUpMenuData.iAlphaId.iAlphaId,
                iArrivedSetUpMenuData.iIconId ) );
            }

        }

    RSat::TSetUpMenuRspV1 setUpMenuRsp;
    RSat::TSetUpMenuRspV1Pckg setUpMenuRspPckg( setUpMenuRsp );
    setUpMenuRsp.iInfoType = RSat::KNoAdditionalInfo;
    setUpMenuRsp.iAdditionalInfo.Zero();
    setUpMenuRsp.SetPCmdNumber( iArrivedSetUpMenuData.PCmdNumber() );

    if ( KErrNone == err )
        {
        LOG( SIMPLE, 
        "SETUPMENU: CSetUpMenuHandler::HandleCommand no error" )
        iSetUpMenuArrived = ETrue;
        setUpMenuRsp.iGeneralResult = RSat::KSuccess;
        // If command had icon data and was done succesfully, report that icon
        // was not shown
        // To be removed when icons are allowed in this command
        // Result of icon showing must be received
        if ( iIconCommand )
            {
            LOG( SIMPLE, 
            "SETUPMENU: CSetUpMenuHandler::HandleCommand iIconCommand true" )
            setUpMenuRsp.iGeneralResult =
                        RSat::KSuccessRequestedIconNotDisplayed;
            }
        }
    else
        {
        LOG( SIMPLE, 
        "SETUPMENU: CSetUpMenuHandler::HandleCommand error" )
        setUpMenuRsp.iGeneralResult = RSat::KMeUnableToProcessCmd;
        }

    CSatCommandHandler::TerminalRsp( RSat::ESetUpMenu, setUpMenuRspPckg );

    LOG( SIMPLE, "SETUPMENU: CSetUpMenuHandler::HandleCommand exiting" )
    }

// -----------------------------------------------------------------------------
// CSetUpMenuHandler::UiLaunchFailed
// -----------------------------------------------------------------------------
//
void CSetUpMenuHandler::UiLaunchFailed()
    {
    LOG( SIMPLE,
        "SETUPMENU: CSetUpMenuHandler::UiLaunchFailed calling - exiting" )
    // SetUpMenu command does not launch UI.
    }
