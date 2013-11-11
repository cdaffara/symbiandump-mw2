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
* Description:  Monitor for browser termination
*
*/


// INCLUDE FILES
#include    "csatbrowserwsmonitor.h"
#include    "CSatBrowserTerminationMonitor.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// CSatBrowserTerminationMonitor::CSatBrowserTerminationMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatBrowserTerminationMonitor::CSatBrowserTerminationMonitor(
    MSatEventMonitorUtils& aUtils ) :
    CSatEventMonitorHandler( aUtils )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserTerminationMonitor::\
        CSatBrowserTerminationMonitor calling-exiting" )
    }

// Destructor
CSatBrowserTerminationMonitor::~CSatBrowserTerminationMonitor()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserTerminationMonitor::\
        ~CSatBrowserTerminationMonitor calling" )

    if ( iWSMonitor )
        {
        iWSMonitor->DoCancelMonitor();
        delete iWSMonitor;
        iWSMonitor = NULL;
        }

    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserTerminationMonitor::\
        ~CSatBrowserTerminationMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserTerminationMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatBrowserTerminationMonitor* CSatBrowserTerminationMonitor::NewL(
    MSatEventMonitorUtils& aUtils )
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserTerminationMonitor::NewL calling" )

    CSatBrowserTerminationMonitor* self =
        new ( ELeave ) CSatBrowserTerminationMonitor( aUtils );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( /*self*/ );

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserTerminationMonitor::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatBrowserTerminationMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatBrowserTerminationMonitor::ConstructL()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserTerminationMonitor::ConstructL calling" )

    // Create base class and give this monitors event as a parameter.
    BaseConstructL( MSatEventMonitor::EBrowserTerminated );

    // Create Windows session monitor.
    iWSMonitor = CSatBrowserWSMonitor::NewL();

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserTerminationMonitor::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserTerminationMonitor::DoCancelMonitor
// -----------------------------------------------------------------------------
//
void CSatBrowserTerminationMonitor::DoCancelMonitor()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserTerminationMonitor::\
        DoCancelMonitor calling" )

    iWSMonitor->DoCancelMonitor();

    LOG( SIMPLE, "SATEVENTMONITORS: CSatBrowserTerminationMonitor::\
        DoCancelMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBrowserTerminationMonitor::StartMonitor
// -----------------------------------------------------------------------------
//
TInt CSatBrowserTerminationMonitor::StartMonitor()
    {
    LOG( SIMPLE,
    "SATEVENTMONITORS: CSatBrowserTerminationMonitor::StartMonitor calling" )

    if ( !iWSMonitor->IsActive() )
        {
        LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserTerminationMonitor::StartMonitor \
        StartMonitor" )
        iWSMonitor->StartMonitor( this );
        }

    LOG( SIMPLE,
    "SATEVENTMONITORS: CSatBrowserTerminationMonitor::StartMonitor exiting" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSatBrowserTerminationMonitor::NotifyBrowserTerminated
// Sends event download notification to SAT Engine.
// -----------------------------------------------------------------------------
//
void CSatBrowserTerminationMonitor::NotifyBrowserTerminated(
    const RSat::TBrowserTerminationCause aCause )
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBrowserTerminationMonitor::BrowserTerminated \
        calling" )

    // Send cause to SAT Engine
    EventOccured( aCause );

    // Restart the monitor
    StartMonitor();

    LOG( SIMPLE,
    "SATEVENTMONITORS: CSatBrowserTerminationMonitor::NotifyBrowserTerminated\
    exiting" )
    }

// End of file
