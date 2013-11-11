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
* Description:  Monitor for Channel status events
*
*/


// INCLUDE FILES
#include    "CSatBIPChannelStatusMonitor.h"
#include    "MSatEventMonitorUtils.h"
#include    "MSatBIPEventNotifier.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// CSatBIPChannelStatusMonitor::CSatBIPChannelStatusMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatBIPChannelStatusMonitor::CSatBIPChannelStatusMonitor(
    MSatEventMonitorUtils& aUtils ) :
    CSatEventMonitorHandler( aUtils )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBIPChannelStatusMonitor::\
        CSatBIPChannelStatusMonitor calling-exiting" )
    }

// Destructor
CSatBIPChannelStatusMonitor::~CSatBIPChannelStatusMonitor()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBIPChannelStatusMonitor::\
        ~CSatBIPChannelStatusMonitor calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPChannelStatusMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatBIPChannelStatusMonitor* CSatBIPChannelStatusMonitor::NewL(
    MSatEventMonitorUtils& aUtils )
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPChannelStatusMonitor::NewL calling" )

    CSatBIPChannelStatusMonitor* self =
        new ( ELeave ) CSatBIPChannelStatusMonitor( aUtils );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPChannelStatusMonitor::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatBIPChannelStatusMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatBIPChannelStatusMonitor::ConstructL()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPChannelStatusMonitor::ConstructL calling" )

    // Create base class and give this monitors event as a parameter
    BaseConstructL( MSatEventMonitor::EChannelStatus );
    iIsActive = EFalse;

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPChannelStatusMonitor::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPChannelStatusMonitor::DoCancelMonitor
// Cancels monitor.
// -----------------------------------------------------------------------------
//
void CSatBIPChannelStatusMonitor::DoCancelMonitor()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBIPChannelStatusMonitor::\
        DoCancelMonitor calling" )

    if ( iIsActive )
        {
        LOG( SIMPLE, "SATEVENTMONITORS: CSatBIPChannelStatusMonitor::\
            DoCancelMonitor Canceling monitor" )
        // Call MSatBIPEventNotifier.CancelChannelStatus
        iEventUtils.BipNotifier().CancelChannelStatus();
        }

    LOG( SIMPLE, "SATEVENTMONITORS: CSatBIPChannelStatusMonitor::\
        DoCancelMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPChannelStatusMonitor::StartMonitor
// Starts monitor.
// -----------------------------------------------------------------------------
//
TInt CSatBIPChannelStatusMonitor::StartMonitor()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPChannelStatusMonitor::StartMonitor calling" )

    if ( !iIsActive )
        {
        LOG( NORMAL, "SATEVENTMONITORS: CSatBIPChannelStatusMonitor::\
            StartMonitor Activating monitor" )
        // Call MSatBIPEventNotifier.NotifyChannelStatus
        iEventUtils.BipNotifier().NotifyChannelStatus( this );
        }

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPChannelStatusMonitor::StartMonitor exiting" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSatBIPChannelStatusMonitor::ChannelStatus
// Called when data is received
// -----------------------------------------------------------------------------
//
void CSatBIPChannelStatusMonitor::ChannelStatus(
    const TInt aChannelId, const TInt aStatus )
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPChannelStatusMonitor::ChannelStatus calling" )

    // Send BIP Event.
    EventOccured( aChannelId, aStatus );

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPChannelStatusMonitor::ChannelStatus exiting" )
    }

// End of file
