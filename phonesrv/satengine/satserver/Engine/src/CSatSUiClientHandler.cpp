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
* Description:  Handles some UI Client activity
*
*/



// INCLUDE FILES
#include    "CSatSUiClientHandler.h"
//lint -e766 Used inside TRAP macro, lint misfunction.
#include    "EnginePanic.h"
#include    "MSatSSessions.h"
#include    "MSatShellController.h"
#include    "CSatThreadDeathMonitor.h"
#include    "SatLog.h"
#include    "MSatUtils.h"
#include    "TSatEventMediator.h"

// CONSTANTS
_LIT( KShellControllerDll, "SatShellCntrl.dll" ); // ShellController dll

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatSUiClientHandler::CSatSUiClientHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatSUiClientHandler::CSatSUiClientHandler(
    MSatSSessions& aSessions,
    TSatEventMediator& aEventMediator ) :
    iSessions( aSessions ),
    iEventMediator( aEventMediator ),
    iUiLaunchedByUser( EFalse ),
    iSatUiClosing( EFalse ),
    iLaunchSatUiAfterClose( EFalse ),
    iSatUiPanicked( EFalse )
    {
    LOG( SIMPLE, "SATENGINE: \
        CSatSUiClientHandler::CSatSUiClientHandler calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiClientHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatSUiClientHandler::ConstructL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::ConstructL calling" )

    // Create thread death monitor
    iThreadDeathMonitor = CSatThreadDeathMonitor::NewL();

    iEventMediator.RegisterL( this, MSatUtils::ESatUiLaunched );
    iEventMediator.RegisterL( this, MSatUtils::EBringSatUiToForeGround );

    // Load SatShellController
    // Create the UI handler instance
    User::LeaveIfError( iSatShellCntrlLibrary.Load( KShellControllerDll ) );

    // creating the class...
    TLibraryFunction uiControllerFactory = iSatShellCntrlLibrary.Lookup( 1 );

    iSatShellCntrl =
        reinterpret_cast<MSatShellController*>( uiControllerFactory() );

    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiClientHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatSUiClientHandler* CSatSUiClientHandler::NewL(
    MSatSSessions& aSessions,
    TSatEventMediator& aEventMediator )
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::NewL calling" )

    CSatSUiClientHandler* self =
        new( ELeave ) CSatSUiClientHandler( aSessions, aEventMediator );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::NewL exiting" )
    return self;
    }


// Destructor
CSatSUiClientHandler::~CSatSUiClientHandler()
    {
    LOG( SIMPLE,
         "SATENGINE: CSatSUiClientHandler::~CSatSUiClientHandler calling" )

    iUtils = NULL;
    delete iSatShellCntrl;
    delete iThreadDeathMonitor;
    iSatShellCntrlLibrary.Close();

    LOG( SIMPLE,
         "SATENGINE: CSatSUiClientHandler::~CSatSUiClientHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiClientHandler::SetUtils
// Sets Sat Utils interface
// -----------------------------------------------------------------------------
//
void CSatSUiClientHandler::SetUtils( MSatUtils* aUtils )
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::SetUtils calling" )

    iUtils = aUtils;

    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::SetUtils exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiClientHandler::ThreadDiedL
// Notification from Thread death monitor
// -----------------------------------------------------------------------------
//
void CSatSUiClientHandler::ThreadDiedL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::ThreadDiedL calling" )

    // Sat UI Thread has died.
    iSatUiClosing = EFalse;
    iUiLaunchedByUser = EFalse;

    // Check is there LaunchUi request active.
    if ( iLaunchSatUiAfterClose )
        {
        LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::ThreadDiedL \
        iLaunchSatUiAfterClose true" )
        iLaunchSatUiAfterClose = EFalse;
        LaunchSatUiL();
        }

    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::ThreadDiedL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiClientHandler::ThreadPanicedL
// Notification from Thread death monitor
// -----------------------------------------------------------------------------
//
void CSatSUiClientHandler::ThreadPanicedL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::ThreadDiedL calling" )

    iSatUiPanicked = ETrue;
    ThreadDiedL();

    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::ThreadDiedL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiClientHandler::Event
// Event notified
// -----------------------------------------------------------------------------
//
void CSatSUiClientHandler::Event( TInt aEvent )
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::Event calling" )

    //lint -e{961} Other events do not need to be handled here
    if ( MSatUtils::ESatUiLaunched == aEvent )
        {
        iSatUiPanicked = EFalse;
        // Ui is not launced by user, if there are executing commands when
        // ui launch event has arrived.
        const TInt activeCommands( iUtils->NumberOfExecutingCommandHandlers() );
        LOG2( SIMPLE, "SATENGINE: CSatSUiClientHandler::Event \
              activeCommands: %i", activeCommands )
        if ( activeCommands > 0 )
            {
            iUiLaunchedByUser = EFalse;
            // Bring to foreground. This will complete the SAT UI cosntruction.
            ShellController().BringSatUiToForeground();
            }
        else
            {
            iUiLaunchedByUser = ETrue;
            }
        }
    else if ( MSatUtils::EBringSatUiToForeGround == aEvent )
        {
        LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::Event bring SAT UI \
             to foreground" )
        // If UI is not launched by user, bring SAT UI to foreground when 
        // needed.
        if ( !iUiLaunchedByUser )
            {
            LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::Event \
            iUiLaunchedByUser false" )
            ShellController().BringSatUiToForeground();
            }
        }

    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::Event exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiClientHandler::NotifyThreadDeath
// Notifies Thread death monitor
// -----------------------------------------------------------------------------
//
void CSatSUiClientHandler::NotifyThreadDeath()
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::NotifyThreadDeath calling" )

    iSatUiClosing = ETrue;
    iUiLaunchedByUser = EFalse;

    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::NotifyThreadDeath exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiClientHandler::StartObservingThread
// Notifies Thread death monitor
// -----------------------------------------------------------------------------
//
void CSatSUiClientHandler::StartObservingThread()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSUiClientHandler::StartObservingThread calling" )

    TInt err( KErrNone );
    TRAP( err, iThreadDeathMonitor->NotifyThreadDeathL( iThread, *this ) );
    if ( KErrNone != err )
        {
        LOG2( NORMAL, "SATENGINE: \
            CSatSUiClientHandler::NotifyThreadDeath error: %i", err )
        iThreadDeathMonitor->Cancel();
        }

    LOG( SIMPLE,
        "SATENGINE: CSatSUiClientHandler::StartObservingThread exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiClientHandler::SetObserveredThread
// Notifies Thread death monitor
// -----------------------------------------------------------------------------
//
void CSatSUiClientHandler::SetObserveredThread( TThreadId& aThreadId )
    {
    LOG( SIMPLE,
         "SATENGINE: CSatSUiClientHandler::SetObserveredThread calling" )

    iThread = aThreadId;

    LOG( SIMPLE,
         "SATENGINE: CSatSUiClientHandler::SetObserveredThread exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiClientHandler::UiSession
// Returns UI session
// -----------------------------------------------------------------------------
//
MSatUiSession* CSatSUiClientHandler::UiSession()
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::UiSession calling" )

    MSatUiSession* session = NULL;

    // If SAT UI is closing, return null, even if session is not null
    if ( !iSatUiClosing && !iSatUiPanicked )
        {
        LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::UiSession session \
        not null" )
        session = iSessions.UiSession();
        }

    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::UiSession exiting" )
    return session;
    }

// -----------------------------------------------------------------------------
// CSatSUiClientHandler::LaunchSatUiL
// Launches Sat UI
// -----------------------------------------------------------------------------
//
void CSatSUiClientHandler::LaunchSatUiL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::LaunchSatUiL calling" )

    // If ui client application is closing, we have to wait for the
    // closing and launch ui client after it.
    if ( iSatUiClosing )
        {
        LOG( NORMAL, "SATENGINE: CSatSUiClientHandler::LaunchSatUiL \
             UI IS CLOSING DOWN -> WAIT UI THREAD DEATH" )
        iLaunchSatUiAfterClose = ETrue;
        }
    else
        {
        LOG( NORMAL, "SATENGINE: CSatSUiClientHandler::LaunchSatUiL \
             LAUNCHING SAT UI" )
        ShellController().LaunchSatUiL();
        }

    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::LaunchSatUiL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSUiClientHandler::UiLaunchedByUser
// Is UI Launched by user
// -----------------------------------------------------------------------------
//
TBool CSatSUiClientHandler::UiLaunchedByUser()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSUiClientHandler::UiLaunchedByUser calling - exiting" )
    return iUiLaunchedByUser;
    }

// -----------------------------------------------------------------------------
// CSatSUiClientHandler::ShellController
// Returns SatShellController from UI side
// -----------------------------------------------------------------------------
//
MSatShellController& CSatSUiClientHandler::ShellController()
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::ShellController calling" )

    __ASSERT_ALWAYS( iSatShellCntrl, PanicSatEngine( ESatEngineNullPointer ) );

    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::ShellController exiting" )
    return *iSatShellCntrl;
    }

// -----------------------------------------------------------------------------
// CSatSUiClientHandler::ShellController
// Returns SatShellController from UI side
// -----------------------------------------------------------------------------
//
TBool CSatSUiClientHandler::IsUiClosing() const
    {
    LOG( SIMPLE, "SATENGINE: CSatSUiClientHandler::IsUiClosing call - exit" )
    return iSatUiClosing;
    }

//  End of File
