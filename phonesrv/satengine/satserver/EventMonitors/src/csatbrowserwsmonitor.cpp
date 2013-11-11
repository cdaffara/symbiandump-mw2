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
* Description:  Monitor for browser termination
*
*/


// INCLUDE FILES
#include    <etelsat.h>
#include    <apgtask.h>
#include    <DocumentHandler.h>  //KWmlcHandler
#include    "CSatBrowserThreadMonitor.h"
#include    "csatbrowserwsmonitor.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatBrowserWSMonitor::CSatBrowserWSMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatBrowserWSMonitor::CSatBrowserWSMonitor() :
    CActive( CActive::EPriorityStandard )
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserWSMonitor::CSatBrowserWSMonitor calling" )

    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserWSMonitor::CSatBrowserWSMonitor exiting" )
    }

// Destructor
CSatBrowserWSMonitor::~CSatBrowserWSMonitor()
    {
    LOG( SIMPLE,
    "SATEVENTMONITORS: CSatBrowserWSMonitor::~CSatBrowserWSMonitor calling" )

    // Cancel any outstanding requests.
    Cancel();

    if ( iThreadMonitor )
        {
        iThreadMonitor->DoCancelMonitor();
        delete iThreadMonitor;
        iThreadMonitor = NULL;
        }

    iObserver = NULL;

    iWg.Close();
    iWsSession.Close();

    LOG( SIMPLE,
    "SATEVENTMONITORS: CSatBrowserWSMonitor::~CSatBrowserWSMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserWSMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatBrowserWSMonitor* CSatBrowserWSMonitor::NewL()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserWSMonitor::NewL calling" )

    CSatBrowserWSMonitor* self =
        new ( ELeave ) CSatBrowserWSMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( /*self*/ );

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserWSMonitor::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatBrowserWSMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatBrowserWSMonitor::ConstructL()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserWSMonitor::ConstructL calling" )
    // Window session is created when event is requested
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserWSMonitor::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserWSMonitor::DoCancelMonitor
// -----------------------------------------------------------------------------
//
void CSatBrowserWSMonitor::DoCancelMonitor()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserWSMonitor::\
        DoCancelMonitor calling" )

    Cancel();

    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserWSMonitor::\
        DoCancelMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserWSMonitor::StartMonitor
// -----------------------------------------------------------------------------
//
TInt CSatBrowserWSMonitor::StartMonitor(
    MSatBrowserWSObserver* aObserver )
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserWSMonitor::StartMonitor calling" )
    TInt err( KErrNone );
    
    if ( !iWGCreated )
        {
        // First time called, create needed services
        LOG( SIMPLE,"SATEVENTMONITORS: CSatBrowserWSMonitor::StartMonitor \
            connect to WsSession " )
        err = iWsSession.Connect();
        if ( KErrNone == err )
            {
            // Creates a window group
            TRAP( err, CreateWGL() );
            }
        }

    iObserver = aObserver;

    if ( !err )
        {
        // All fine, start monitoring
        Start();
        }
        
    LOG2( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserWSMonitor::StartMonitor exiting err=%d",\
            err )
    
    return err;
    }

// -----------------------------------------------------------------------------
// CSatBrowserWSMonitor::Start
// -----------------------------------------------------------------------------
//
void CSatBrowserWSMonitor::Start()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserWSMonitor::Start calling" )

    Cancel();

    // It migth be null already
    if ( iThreadMonitor )
        {
        LOG( SIMPLE, 
        "SATEVENTMONITORS: CSatBrowserWSMonitor::Start iThreadMonitor true" )
        delete iThreadMonitor;
        iThreadMonitor = NULL;
        }

    iWsSession.EventReady( &iStatus );
    SetActive();

    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserWSMonitor::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserWSMonitor::RunL
// Checks is the WML Browser launched and creates browser thread monitor to
// monitor browsers thread death.
// -----------------------------------------------------------------------------
//
void CSatBrowserWSMonitor::RunL()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserWSMonitor::RunL calling" )

    // Get the status of this object.
    const TInt errCode( iStatus.Int() );
    LOG2( SIMPLE,
    "SATEVENTMONITORS: CSatBrowserWSMonitor::RunL errCode: %d",
    errCode )
    if ( KErrNone == errCode )
        {
        // UID Code for WML Browser in S60.
        const TUid uidWmlBrowser( TUid::Uid( KWmlcHandler ) );
        TWsEvent event;
        iWsSession.GetEvent( event );
        TApaTaskList taskList( iWsSession );

        if ( taskList.FindApp( uidWmlBrowser ).Exists() )
            {
            LOG( SIMPLE,
            "SATEVENTMONITORS: CSatBrowserWSMonitor::RunL task exists" )
            const TApaTask task = taskList.FindApp( uidWmlBrowser );
            const TThreadId threadID = task.ThreadId();
            RThread thread;
            User::LeaveIfError ( thread.Open( threadID, EOwnerProcess ) );

            // Create Browser thread monitor.
            iThreadMonitor = CSatBrowserThreadMonitor::NewL( thread );

            // Call the active object to monitor for this threads death.
            iThreadMonitor->StartMonitor( this );
            }
        else
            {
            LOG( SIMPLE,
            "SATEVENTMONITORS: CSatBrowserWSMonitor::RunL start" )
            // Renew the request.
            Start();
            }
        }
    else if ( KErrCancel != errCode )
        {
        // Renew the request.
        Start();
        }
    else
        {
        LOG( SIMPLE,
            "SATEVENTMONITORS: CSatBrowserWSMonitor::RunL cancelled" )
        }

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserWSMonitor::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserWSMonitor::DoCancel
// -----------------------------------------------------------------------------
//
void CSatBrowserWSMonitor::DoCancel()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserWSMonitor::DoCancel calling" )

    if ( iThreadMonitor )
        {
        LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserWSMonitor::DoCancel iThreadMonitor true" )
        iThreadMonitor->DoCancelMonitor();
        }

    // Cancel the outstanding request.
    iWsSession.EventReadyCancel();

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserWSMonitor::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserWSMonitor::NotifyBrowserTerminated
// Sends event download notification to SAT Engine.
// -----------------------------------------------------------------------------
//
void CSatBrowserWSMonitor::NotifyBrowserTerminated(
    const RSat::TBrowserTerminationCause aCause )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserWSMonitor::\
        NotifyBrowserTerminated calling" )

    // Notify termination to observer.
    iObserver->NotifyBrowserTerminated( aCause );

    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserWSMonitor::\
        NotifyBrowserTerminated exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserWSMonitor::CreateWGL
// Creates a window group and hides it form the UI.
// -----------------------------------------------------------------------------
//
void CSatBrowserWSMonitor::CreateWGL()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserWSMonitor::CreateWGL calling" )

    // -1 means the window will never be visible.
    const TInt KWindowGroupPriority( -1 );

    RWindowGroup wg( iWsSession );
    // Make a handle from the address of window group obj.
    User::LeaveIfError( wg.Construct( reinterpret_cast<TUint32>( &wg ) ) );
    // Enable group changed events.
    User::LeaveIfError( wg.EnableGroupChangeEvents() );

    // Get the ordinal pos of this window group.
    const TInt ordPos( wg.OrdinalPosition() );
    // Set SAT so its never shown in the UI.
    wg.SetOrdinalPosition( ordPos , KWindowGroupPriority );

    // Needs to be a member variable. Must be closed in destructor.
    // Otherwise can't monitor what has been opened.
    iWg = wg;

    // Set SAT Server hidden now.
    iWsSession.Flush();
    iWGCreated = ETrue;
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserWSMonitor::CreateWGL exiting" )
    }

// End of file
