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
* Description:  Monitor for SIM removal
*
*/


// INCLUDE FILES
#include    "TSatSystemStateFactory.h"
#include    "MSatSystemStateChangeNotifier.h"
#include    "CSatSimRemovalMonitor.h"
#include    "SatLog.h"


// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// CSatSimRemovalMonitor::CSatSimRemovalMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatSimRemovalMonitor::CSatSimRemovalMonitor(
    MSatEventMonitorUtils& aUtils ) :
    CSatEventMonitorHandler( aUtils )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatSimRemovalMonitor::\
        CSatSimRemovalMonitor calling-exiting" )
    }

// Destructor
CSatSimRemovalMonitor::~CSatSimRemovalMonitor()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatSimRemovalMonitor::\
        ~CSatSimRemovalMonitor calling" )

    delete iPSNotifier;

    LOG( SIMPLE, "SATEVENTMONITORS: CSatSimRemovalMonitor::\
        ~CSatSimRemovalMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSimRemovalMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatSimRemovalMonitor* CSatSimRemovalMonitor::NewL(
    MSatEventMonitorUtils& aUtils )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatSimRemovalMonitor::NewL calling" )

    CSatSimRemovalMonitor* self = new( ELeave ) CSatSimRemovalMonitor( aUtils );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATEVENTMONITORS: CSatSimRemovalMonitor::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatSimRemovalMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatSimRemovalMonitor::ConstructL()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatSimRemovalMonitor::ConstructL calling" )

    // Create base class and give this monitors event as a parameter
    BaseConstructL( MSatEventMonitor::ESimRemoved );

    iPSNotifier =
        TSatSystemStateFactory::CreateSimStatusChangeNotifierL( *this );

    LOG( SIMPLE, "SATEVENTMONITORS: CSatSimRemovalMonitor::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSimRemovalMonitor::DoCancelMonitor
// Cancels monitor.
// -----------------------------------------------------------------------------
//
void CSatSimRemovalMonitor::DoCancelMonitor()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatSimRemovalMonitor::DoCancelMonitor calling" )

    iPSNotifier->CancelNotify();

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatSimRemovalMonitor::DoCancelMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSimRemovalMonitor::StartMonitor
// Starts monitor.
// -----------------------------------------------------------------------------
//
TInt CSatSimRemovalMonitor::StartMonitor()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatSimRemovalMonitor::StartMonitor calling" )

    // Cancel first. It might be activated already
    DoCancelMonitor();

    // Now start it again.
    TRAPD( err, iPSNotifier->NotifyChangeL() );

    LOG2( SIMPLE, "SATEVENTMONITORS: CSatSimRemovalMonitor::StartMonitor \
        exiting with value: %d", err )
    return err;
    }

// -----------------------------------------------------------------------------
// CSatSimRemovalMonitor::StateChanged
// Called when SIM is removed.
// -----------------------------------------------------------------------------
//
void CSatSimRemovalMonitor::StateChanged()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatSimRemovalMonitor::StateChanged calling" )

    // Inform SAT Engine that SIM status state has changed.
    EventOccured();

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatSimRemovalMonitor::StateChanged exiting" )
    }

// End of file
