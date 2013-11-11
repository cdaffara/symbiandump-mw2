/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Network registration status monitor.
*
*/


#include "csatnetworkregstatusmonitor.h"
#include "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CSatNetworkRegStatusMonitor::CSatNetworkRegStatusMonitor(
    MSatMultiModeApi& aPhone ) :
    CActive( CActive::EPriorityStandard ),
    iRegistrationStatus( RMobilePhone::ERegistrationUnknown ),
    iWaitingRegistrationStatus( EFalse ),
    iPhone( aPhone )
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: \
        CSatNetworkRegStatusMonitor::CSatNetworkRegStatusMonitor calling" )

    // Add to active scheduler.
    CActiveScheduler::Add( this );

    LOG( SIMPLE, "SATSYSTEMSTATE: \
        CSatNetworkRegStatusMonitor::CSatNetworkRegStatusMonitor exiting" )
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CSatNetworkRegStatusMonitor* CSatNetworkRegStatusMonitor::NewL(
    MSatMultiModeApi& aPhone )
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: CSatNetworkRegStatusMonitor::NewL calling" )

    CSatNetworkRegStatusMonitor* self =
        new( ELeave ) CSatNetworkRegStatusMonitor( aPhone );

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatNetworkRegStatusMonitor::NewL exiting" )
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSatNetworkRegStatusMonitor::~CSatNetworkRegStatusMonitor()
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: \
        CSatNetworkRegStatusMonitor::~CSatNetworkRegStatusMonitor calling" )

    Cancel();

    LOG( SIMPLE, "SATSYSTEMSTATE: \
        CSatNetworkRegStatusMonitor::~CSatNetworkRegStatusMonitor exiting" )
    }

// ---------------------------------------------------------------------------
// Activate monitor.
// ---------------------------------------------------------------------------
//
void CSatNetworkRegStatusMonitor::Start()
    {
    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatNetworkRegStatusMonitor::Start calling" )

#ifndef SAT_USE_DUMMY_TSY
    if ( !IsActive() )
        {
        iRegistrationStatus = RMobilePhone::ERegistrationUnknown;

        iPhone.GetNetworkRegistrationStatus( iStatus, iRegistrationStatus );
        SetActive();
        iWaitingRegistrationStatus = ETrue;
        LOG( SIMPLE, "SATSYSTEMSTATE:   Started" )
        }
#else
    iRegistrationStatus = RMobilePhone::ERegisteredOnHomeNetwork;
    iWaitingRegistrationStatus = EFalse;
    LOG( SIMPLE, "SATSYSTEMSTATE:   Set for WINS Emulator" )
#endif

    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatNetworkRegStatusMonitor::Start exiting" )
    }

// ---------------------------------------------------------------------------
// Get current value of network registration status.
// ---------------------------------------------------------------------------
//
RMobilePhone::TMobilePhoneRegistrationStatus
    CSatNetworkRegStatusMonitor::CurrentValue()
    {
    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatNetworkRegStatusMonitor::CurrentValue calling" )

    // In case network registration notification failed on startup, we need to 
    // check it again, if not registered on home network and we are observing
    // changes in registration status ( iWaitingRegistrationStatus == EFalse )
    if ( !iWaitingRegistrationStatus &&
         ( RMobilePhone::ERegisteredOnHomeNetwork != iRegistrationStatus &&
           RMobilePhone::ERegisteredRoaming != iRegistrationStatus ) )
        {
        LOG( NORMAL, "Not registered -> Going to check value again" )
        Cancel(); // Cancel pending request...
        Start();  // ... and start again
        }

    // Wait, initial value has not yet been got.
    if ( iWaitingRegistrationStatus )
        {
        LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatNetworkRegStatusMonitor::CurrentValue \
        iWaitingRegistrationStatus true" )
        if ( !iWait.IsStarted() )
            {
            LOG( SIMPLE,
            "SATSYSTEMSTATE: CSatNetworkRegStatusMonitor::CurrentValue \
            start iWait" )
            iWait.Start();
            }
        }

    LOG2( SIMPLE,
        "SATSYSTEMSTATE: CSatNetworkRegStatusMonitor::CurrentValue exiting \
        (%d)", iRegistrationStatus )
    return iRegistrationStatus;
    }

// -----------------------------------------------------------------------------
// CRegistrationMonitor::DoCancel
// -----------------------------------------------------------------------------
//
void CSatNetworkRegStatusMonitor::DoCancel()
    {
    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatNetworkRegStatusMonitor::DoCancel calling" )

    iPhone.CancelAsyncRequest(
        EMobilePhoneNotifyNetworkRegistrationStatusChange );
    if ( iWait.IsStarted() )
        {
        LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatNetworkRegStatusMonitor::DoCancel stop iWait" )
        iWait.AsyncStop();
        }

    LOG( SIMPLE,
        "SATSYSTEMSTATE: CSatNetworkRegStatusMonitor::DoCancel exiting" )
    }

// ---------------------------------------------------------------------------
// From class CActive.
// Marks status value valid and activates change notifier.
// ---------------------------------------------------------------------------
//
void CSatNetworkRegStatusMonitor::RunL()
    {
    const TInt status( iStatus.Int() ); // Asynchronous status
    LOG2( SIMPLE,
        "SATSYSTEMSTATE: CSatNetworkRegStatusMonitor::RunL calling (%d)",
        status )

    // Restart request
    if ( KErrServerBusy == status )
        {
        iPhone.GetNetworkRegistrationStatus(
            iStatus,
            iRegistrationStatus );
        SetActive();
        }
    else if ( KErrCancel != status )
        {
        // (Re)start observing
        iPhone.NotifyNetworkRegistrationStatusChange(
            iStatus,
            iRegistrationStatus );
        SetActive();
        }
    else
        {
        LOG( SIMPLE, " Request cancelled" )
        }

    // Still waiting first value after start.
    if ( iWaitingRegistrationStatus )
        {
        LOG( SIMPLE, 
        "SATSYSTEMSTATE: CSatNetworkRegStatusMonitor::RunL \
        iWaitingRegistrationStatus true" )
        iWaitingRegistrationStatus = EFalse;
        if ( iWait.IsStarted() )
            {
            LOG( SIMPLE, 
            "SATSYSTEMSTATE: CSatNetworkRegStatusMonitor::RunL \
            stop iWait" )
            iWait.AsyncStop();
            }
        }

    LOG2( SIMPLE,
        "SATSYSTEMSTATE:   NetworkRegistrationStatus updated: %d",
        iRegistrationStatus )

    LOG( SIMPLE, "SATSYSTEMSTATE: CSatNetworkRegStatusMonitor::RunL exiting" )
    }

// End of file
