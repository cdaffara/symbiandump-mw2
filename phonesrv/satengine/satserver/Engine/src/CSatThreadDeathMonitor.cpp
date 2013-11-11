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
* Description:  Monitors threads death and notifies an event, when thread dies
*
*/



// INCLUDE FILES
#include    "CSatThreadDeathMonitor.h"
#include    "MThreadDeathObserver.h"
#include    "SatLog.h"
#include    "MSatUtils.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatThreadDeathMonitor::CSatThreadDeathMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatThreadDeathMonitor::CSatThreadDeathMonitor() : CActive( EPriorityStandard )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatThreadDeathMonitor::CSatThreadDeathMonitor calling" )

    CActiveScheduler::Add( this );

    LOG( SIMPLE,
        "SATENGINE: CSatThreadDeathMonitor::CSatThreadDeathMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatThreadDeathMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatThreadDeathMonitor* CSatThreadDeathMonitor::NewL()
    {
    LOG( SIMPLE, "SATENGINE: CSatThreadDeathMonitor::NewL calling" )
    CSatThreadDeathMonitor* deathMonitor = new( ELeave ) CSatThreadDeathMonitor;
    LOG( SIMPLE, "SATENGINE: CSatThreadDeathMonitor::NewL exiting" )
    return deathMonitor;
    }


// Destructor
CSatThreadDeathMonitor::~CSatThreadDeathMonitor()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatThreadDeathMonitor::~CSatThreadDeathMonitor calling" )
    Cancel();
    iThread.Close();
    iObserver = NULL;
    LOG( SIMPLE, 
        "SATENGINE: CSatThreadDeathMonitor::~CSatThreadDeathMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatThreadDeathMonitor::NotifyThreadDeathL
// Prepares to notify observer of threads death
// -----------------------------------------------------------------------------
//
void CSatThreadDeathMonitor::NotifyThreadDeathL(
                                 const TThreadId& aThreadId,
                                 MThreadDeathObserver& aObserver )
    {

    LOG( SIMPLE,
        "SATENGINE: CSatThreadDeathMonitor::NotifyThreadDeathL calling" )
    Cancel();

    iThread.Close();

    User::LeaveIfError( iThread.Open( aThreadId ) );

    iObserver = &aObserver;
    StartObservingThreadDeath();
    LOG( SIMPLE,
        "SATENGINE: CSatThreadDeathMonitor::NotifyThreadDeathL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatThreadDeathMonitor::StartObservingThreadDeath
// Logons to thread and starts observing it's state
// -----------------------------------------------------------------------------
//
void CSatThreadDeathMonitor::StartObservingThreadDeath()
    {
    LOG( SIMPLE, "SATENGINE: CSatThreadDeathMonitor::\
            StartObservingThreadDeath calling" )
    iThread.Logon( iStatus );
    SetActive();
    LOG( SIMPLE, "SATENGINE: CSatThreadDeathMonitor::\
            StartObservingThreadDeath exiting" )
    }

// -----------------------------------------------------------------------------
// CSatThreadDeathMonitor::DoCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatThreadDeathMonitor::DoCancel()
    {
    LOG( SIMPLE, "SATENGINE: CSatThreadDeathMonitor::DoCancel calling" )
    iThread.LogonCancel( iStatus );
    LOG( SIMPLE, "SATENGINE: CSatThreadDeathMonitor::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatThreadDeathMonitor::RunL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatThreadDeathMonitor::RunL()
    {
    LOG( SIMPLE, "SATENGINE: CSatThreadDeathMonitor::RunL calling" )

    const TExitType exitType( iThread.ExitType() );

    if ( EExitPending == exitType ) // If thread is still alive, restart
        {
        LOG( SIMPLE, " Thread is still alive" )
        StartObservingThreadDeath();
        }
    else if ( EExitPanic == exitType )
        {
        LOG( SIMPLE, " Thread has panicked" )
        iThread.Close();
        iObserver->ThreadPanicedL();
        }
    else
        {
        LOG( SIMPLE, " Thread is dead" )
        iThread.Close();
        iObserver->ThreadDiedL();
        }
    LOG( SIMPLE, "SATENGINE: CSatThreadDeathMonitor::RunL exiting" )
    }

//  End of File
