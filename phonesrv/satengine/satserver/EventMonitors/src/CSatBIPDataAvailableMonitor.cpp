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
* Description:  Monitor for Data available
*
*/


// INCLUDE FILES
#include    "CSatBIPDataAvailableMonitor.h"
#include    "MSatEventMonitorUtils.h"
#include    "MSatBIPEventNotifier.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// CSatBIPDataAvailableMonitor::CSatBIPDataAvailableMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatBIPDataAvailableMonitor::CSatBIPDataAvailableMonitor(
    MSatEventMonitorUtils& aUtils ) :
    CSatEventMonitorHandler( aUtils )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBIPDataAvailableMonitor::\
        CSatBIPDataAvailableMonitor calling-exiting" )
    }

// Destructor
CSatBIPDataAvailableMonitor::~CSatBIPDataAvailableMonitor()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBIPDataAvailableMonitor::\
        ~CSatBIPDataAvailableMonitor calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPDataAvailableMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatBIPDataAvailableMonitor* CSatBIPDataAvailableMonitor::NewL(
    MSatEventMonitorUtils& aUtils )
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPDataAvailableMonitor::NewL calling" )

    CSatBIPDataAvailableMonitor* self =
        new( ELeave ) CSatBIPDataAvailableMonitor( aUtils );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPDataAvailableMonitor::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatBIPDataAvailableMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatBIPDataAvailableMonitor::ConstructL()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPDataAvailableMonitor::ConstructL calling" )

    // Create base class and give this monitors event as a parameter
    BaseConstructL( MSatEventMonitor::EDataAvailable );
    iIsActive = EFalse;

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPDataAvailableMonitor::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPDataAvailableMonitor::DoCancelMonitor
// Cancels monitor.
// -----------------------------------------------------------------------------
//
void CSatBIPDataAvailableMonitor::DoCancelMonitor()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatBIPDataAvailableMonitor::\
        DoCancelMonitor calling" )

    if ( iIsActive )
        {
        LOG( NORMAL, "SATEVENTMONITORS: CSatBIPDataAvailableMonitor::\
            DoCancelMonitor Canceling monitor" )
        // Call MSatBIPEventNotifier.CancelDataAvailable
        iEventUtils.BipNotifier().CancelDataAvailable();
        }

    LOG( SIMPLE, "SATEVENTMONITORS: CSatBIPDataAvailableMonitor::\
        DoCancelMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatBIPDataAvailableMonitor::StartMonitor
// Starts monitor.
// -----------------------------------------------------------------------------
//
TInt CSatBIPDataAvailableMonitor::StartMonitor()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPDataAvailableMonitor::StartMonitor calling" )

    if ( !iIsActive )
        {
        LOG( NORMAL, "SATEVENTMONITORS: CSatBIPDataAvailableMonitor::\
            StartMonitor Activating monitor" )
        // Call MSatBIPEventNotifier.NotifyDataAvailable
        iEventUtils.BipNotifier().NotifyDataAvailable( this );
        }

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPDataAvailableMonitor::StartMonitor exiting" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSatBIPDataAvailableMonitor::DataAvailable
// Called when data is received
// -----------------------------------------------------------------------------
//
void CSatBIPDataAvailableMonitor::DataAvailable( const TInt aChannelId,
    const TInt aLength )
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPDataAvailableMonitor::DataAvailable calling" )

    // Inform SAT Engine that data is available.
    EventOccured( aChannelId, aLength );

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatBIPDataAvailableMonitor::DataAvailable exiting" )
    }

// End of file
