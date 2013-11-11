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
* Description:  Monitor for idle screen availability.
*
*/


// INCLUDE FILES
#include    "TSatSystemStateFactory.h"
#include    "MSatSystemStateChangeNotifier.h"
#include    "CSatIdleScreenMonitor.h"
#include    "SatLog.h"


// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// CSatIdleScreenMonitor::CSatIdleScreenMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatIdleScreenMonitor::CSatIdleScreenMonitor(
    MSatEventMonitorUtils& aUtils ) :
    CSatEventMonitorHandler( aUtils )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatIdleScreenMonitor::\
        CSatIdleScreenMonitor calling-exiting" )
    }

// Destructor
CSatIdleScreenMonitor::~CSatIdleScreenMonitor()
    {
    LOG( SIMPLE, "SATEVENTMONITORS:CSatIdleScreenMonitor::\
        ~CSatIdleScreenMonitor calling" )

    delete iPSNotifier;

    LOG( SIMPLE, "SATEVENTMONITORS:CSatIdleScreenMonitor::\
        ~CSatIdleScreenMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatIdleScreenMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatIdleScreenMonitor* CSatIdleScreenMonitor::NewL(
    MSatEventMonitorUtils& aUtils )
    {
    LOG( SIMPLE, "SATEVENTMONITORS:CSatIdleScreenMonitor::NewL calling" )

    CSatIdleScreenMonitor* self = new( ELeave ) CSatIdleScreenMonitor( aUtils );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATEVENTMONITORS:CSatIdleScreenMonitor::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatIdleScreenMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatIdleScreenMonitor::ConstructL()
    {
    LOG( SIMPLE, "SATEVENTMONITORS:CSatIdleScreenMonitor::\
        ConstructL calling" )

    // Create base class and give this monitors event as a parameter
    BaseConstructL( MSatEventMonitor::EIdleScreenAvailable );
    iPSNotifier =
        TSatSystemStateFactory::CreateIdleModeChangeNotifierL( *this );

    LOG( SIMPLE, "SATEVENTMONITORS:CSatIdleScreenMonitor::\
        ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatIdleScreenMonitor::DoCancelMonitor
// Cancels monitor.
// -----------------------------------------------------------------------------
//
void CSatIdleScreenMonitor::DoCancelMonitor()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS:CSatIdleScreenMonitor::DoCancelMonitor calling" )

    iPSNotifier->CancelNotify();

    LOG( SIMPLE,
        "SATEVENTMONITORS:CSatIdleScreenMonitor::DoCancelMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatIdleScreenMonitor::StartMonitor
// Starts monitor.
// -----------------------------------------------------------------------------
//
TInt CSatIdleScreenMonitor::StartMonitor()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS:CSatIdleScreenMonitor::StartMonitor calling" )

    // Cancel first, it might be activated already
    DoCancelMonitor();

    // Now start it again
    TRAPD( err, iPSNotifier->NotifyChangeL() );

    LOG2( SIMPLE, "SATEVENTMONITORS:CSatIdleScreenMonitor::\
        StartMonitor exiting with code: %d", err )
    return err;
    }

// -----------------------------------------------------------------------------
// CSatIdleScreenMonitor::StateChanged
// Called when idle screen is available.
// -----------------------------------------------------------------------------
//
void CSatIdleScreenMonitor::StateChanged()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS:CSatIdleScreenMonitor::StateChanged calling" )

    // Inform SAT Engine that idle screen is available.
    EventOccured();

    LOG( SIMPLE,
        "SATEVENTMONITORS:CSatIdleScreenMonitor::StateChanged exiting" )
    }

// End of file
