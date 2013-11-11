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
* Description:  Monitors browser's thread death.
*
*/


// INCLUDE FILES
#include    <etelsat.h>
#include    <e32std.h>
#include    "msatbrowserthreadobserver.h"
#include    "CSatBrowserThreadMonitor.h"
#include    "SatLog.h"


// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CSatBrowserThreadMonitor::CSatBrowserThreadMonitor
// The class constructor
// -----------------------------------------------------------------------------
//
CSatBrowserThreadMonitor::CSatBrowserThreadMonitor(
    const RThread& aThread ) :
    CActive( EPriorityStandard ),
    iThread( aThread )
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserThreadMonitor::CSatBrowserThreadMonitor \
        calling" )

    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserThreadMonitor::CSatBrowserThreadMonitor \
        exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSBrowserTerminationMonitor::NewL
// Two-phased constructor
// -----------------------------------------------------------------------------
//
CSatBrowserThreadMonitor* CSatBrowserThreadMonitor::NewL(
    const RThread& aThread )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserThreadMonitor::NewL calling" )

    CSatBrowserThreadMonitor* self =
        new( ELeave )CSatBrowserThreadMonitor( aThread );

    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserThreadMonitor::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatBrowserThreadMonitor::~CSatBrowserThreadMonitor
// Destructor
// -----------------------------------------------------------------------------
//
CSatBrowserThreadMonitor::~CSatBrowserThreadMonitor()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserThreadMonitor::\
        ~CSatBrowserThreadMonitor calling" )

    // Cancel any outstanding requests.
    Cancel();
    iThread.Close();

    iObserver = NULL;

    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserThreadMonitor::\
        ~CSatBrowserThreadMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserThreadMonitor::StartMonitor
// Starts the handler.
// -----------------------------------------------------------------------------
//
void CSatBrowserThreadMonitor::StartMonitor(
    MSatBrowserThreadObserver* aObserver )
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserThreadMonitor::StartMonitor calling" )

    iObserver = aObserver;

    Start();

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserThreadMonitor::StartMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserThreadMonitor::Start
// Starts the handler.
// -----------------------------------------------------------------------------
//
void CSatBrowserThreadMonitor::Start()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserThreadMonitor::Start calling" )

    // Just in case.
    Cancel();

    // Monitor changes only in Browser Thread.
    iThread.Logon( iStatus );
    SetActive();

    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserThreadMonitor::Start exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserThreadMonitor::DoCancelMonitor
// -----------------------------------------------------------------------------
//
void CSatBrowserThreadMonitor::DoCancelMonitor()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserThreadMonitor::\
        DoCancelMonitor calling" )

    Cancel();

    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserThreadMonitor::\
        DoCancelMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserThreadMonitor::RunL
// Handles the command.
// -----------------------------------------------------------------------------
//
void CSatBrowserThreadMonitor::RunL()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserThreadMonitor::RunL calling" )

    const TExitType exitType( iThread.ExitType() );

    // Check whether the thread is alive.
    if ( EExitPending == exitType )
        {
        LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserThreadMonitor::RunL() | \
           iThread.ExitType = EExitPending , renew the request" )
        // Renew the request.
        Start();
        }
    // Thread has exited
    else
        {
        LOG2( SIMPLE, "SATEVENTMONITORS: CSatBrowserThreadMonitor::RunL() | \
            Thread has exited with exit code %i", exitType )

        // Why did the browser exit.
        RSat::TBrowserTerminationCause cause( ExitCause() );

        // Call the event monitor with the cause of the thread termination.
        iObserver->NotifyBrowserTerminated( cause );
        }
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserThreadMonitor::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserThreadMonitor::DoCancel
// Cancels the pending request.
// -----------------------------------------------------------------------------
//
void CSatBrowserThreadMonitor::DoCancel()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserThreadMonitor::DoCancel calling" )

    // No longer monitor this thread.
    iThread.LogonCancel( iStatus );

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserThreadMonitor::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserThreadMonitor::ExitCause
// Why did the application close.
// -----------------------------------------------------------------------------
//
RSat::TBrowserTerminationCause CSatBrowserThreadMonitor::ExitCause() const
    {
    LOG( SIMPLE,
    "SATEVENTMONITORS: CSatBrowserThreadMonitor::ExitCause calling" )

    TExitType exitType = iThread.ExitType();
    RSat::TBrowserTerminationCause terminationCause;
    LOG2( SIMPLE,
    "SATEVENTMONITORS: CSatBrowserThreadMonitor::ExitCause exitType: %d",
    exitType )
    switch ( exitType )
        {
        // The thread has ended as a result of a kill.
        // i.e. Kill() has been called on the RThread.
        case EExitKill:
            {
            terminationCause = RSat::EUserTermination;
            break;
            }

        // The thread has been panicked.
        case EExitPanic:
            {
            terminationCause = RSat::EErrorTermination;
            break;
            }

        // The thread ended for unknown reasons.
        default:
            {
            terminationCause = RSat::EUnknownCause;
            break;
            }
        }

    LOG2( SIMPLE, "SATEVENTMONITORS: CSatBrowserThreadMonitor::\
        ExitCause exiting with value: %d", terminationCause )
    return terminationCause;
    }

//  End of File
