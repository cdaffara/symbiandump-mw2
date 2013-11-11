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
* Description:  Monitor for user activity
*
*/


// INCLUDE FILES
#include    "CSatUserActivityMonitor.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatUserActivityMonitor::CSatUserActivityMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatUserActivityMonitor::CSatUserActivityMonitor(
    MSatEventMonitorUtils& aUtils ) :
    CSatEventMonitorHandler( aUtils )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatUserActivityMonitor::\
        CSatUserActivityMonitor calling" )

    iActivityManager = NULL;

    LOG( SIMPLE, "SATEVENTMONITORS: CSatUserActivityMonitor::\
        CSatUserActivityMonitor exiting" )
    }

// Destructor
CSatUserActivityMonitor::~CSatUserActivityMonitor()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatUserActivityMonitor::\
        ~CSatUserActivityMonitor calling" )

    Cancel();

    if ( iActivityManager )
        {
        delete iActivityManager;
        iActivityManager = NULL;
        }

    LOG( SIMPLE, "SATEVENTMONITORS: CSatUserActivityMonitor::\
        ~CSatUserActivityMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatUserActivityMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatUserActivityMonitor* CSatUserActivityMonitor::NewL(
    MSatEventMonitorUtils& aUtils )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatUserActivityMonitor::NewL calling" )

    CSatUserActivityMonitor* self =
        new( ELeave ) CSatUserActivityMonitor( aUtils );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATEVENTMONITORS: CSatUserActivityMonitor::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatUserActivityMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatUserActivityMonitor::ConstructL()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatUserActivityMonitor::\
        ConstructL calling" )

    // Create base class and give this monitors event as a parameter
    BaseConstructL( MSatEventMonitor::EUserActivity );
    iMonitoring = EFalse;

    LOG( SIMPLE, "SATEVENTMONITORS: CSatUserActivityMonitor::\
        ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatUserActivityMonitor::StartActivityManagerL
// Called when user has been inactive for a pre-defined time period
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatUserActivityMonitor::StartActivityManagerL()
    {
    LOG( SIMPLE,"SATEVENTMONITORS: \
        CSatUserActivityMonitor::StartActivityManag calling" )

    // Delete activity manager if it exists
    if ( iActivityManager )
        {
        LOG( SIMPLE,"SATEVENTMONITORS: \
        CSatUserActivityMonitor::StartActivityManag iActivityManager true" )
        iActivityManager->Cancel();
        delete iActivityManager;
        iActivityManager = NULL;
        }

    iMonitoring = ETrue;

    iActivityManager = CUserActivityManager::NewL( EPriorityNormal );
    iActivityManager->Start(
        TTimeIntervalSeconds( 1 ),
        TCallBack( DispatchUserInactive, this ),
        TCallBack( DispatchUserActive, this ) );

    // Changes checking immediatelly activity,
    // activity before exceeding one second.
    iActivityManager->SetInactivityTimeout( TTimeIntervalSeconds( 0 ) );

    LOG( SIMPLE, "SATEVENTMONITORS: \
        CSatUserActivityMonitor::StartActivityManagerL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatUserActivityMonitor::DispatchUserInactive
// Called when user has been inactive for a pre-defined time period
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSatUserActivityMonitor::DispatchUserInactive( TAny* /* aPtr */ )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: \
        CSatUserActivityMonitor::DispatchUserInactive calling/exiting" )
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSatUserActivityMonitor::DispatchUserActive
// Called when user does something
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSatUserActivityMonitor::DispatchUserActive( TAny* aPtr )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: \
        CSatUserActivityMonitor::DispatchUserActive calling" )

    TInt retVal( KErrNone );

    if ( aPtr )
        {
        LOG( SIMPLE, "SATEVENTMONITORS: \
        CSatUserActivityMonitor::DispatchUserActive aPtr true" )
        CSatUserActivityMonitor* ptrThis = 
            static_cast<CSatUserActivityMonitor*>( aPtr );
        if ( ptrThis->iMonitoring )
            {
            LOG( SIMPLE, "SATEVENTMONITORS: \
            CSatUserActivityMonitor::DispatchUserActive ptrThis->iMonitoring" )
            // Notify observer and set flag off, no more monitoring.
            ptrThis->EventOccured();
            ptrThis->iMonitoring = EFalse;
            }
        else
            {
            retVal = KErrCancel;
            }
        }
    LOG2( SIMPLE, "SATEVENTMONITORS: CSatUserActivityMonitor::\
    DispatchUserActive exiting, retVal: %d", retVal )
    return retVal;    
    }

// -----------------------------------------------------------------------------
// CSatUserActivityMonitor::DoCancelMonitor
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatUserActivityMonitor::DoCancelMonitor()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatUserActivityMonitor::DoCancelMonitor calling" )

    Cancel();

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatUserActivityMonitor::DoCancelMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatUserActivityMonitor::StartMonitor
// Starts monitoring the user activity. The real start of user actitivity
// monitoring starts in RunL, after call of Inactivity( 0 ), which causes run of
// RunL immediatly. The inacativity call is needed to reset the user activity
// timers in User class.
// -----------------------------------------------------------------------------
//
TInt CSatUserActivityMonitor::StartMonitor()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatUserActivityMonitor::StartMonitor calling" )

    // Starts activity manager.
    TRAPD( err, StartActivityManagerL() );

    LOG2( SIMPLE,
        "SATEVENTMONITORS: CSatUserActivityMonitor::StartMonitor exiting,\
        err: %d", err )
    return err;
    }

// -----------------------------------------------------------------------------
// CSatUserActivityMonitor::Cancel
// Cancels user activity monitor.
// If activity manager is active, cancels request
// and deletes activity manager.
// -----------------------------------------------------------------------------
//
void CSatUserActivityMonitor::Cancel()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatUserActivityMonitor::Cancel calling" )

    if ( iActivityManager )
        {
        LOG( DETAILED,
            "SATEVENTMONITORS: CSatUAMon::Cancel AM Cancel call" )
        iActivityManager->Cancel();

        delete iActivityManager;
        iActivityManager = NULL;
        }

    LOG( SIMPLE, "SATEVENTMONITORS: CSatUserActivityMonitor::Cancel exiting" )
    }

// End of file
