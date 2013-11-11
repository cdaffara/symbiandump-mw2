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
* Description:  Base class for event monitors
*
*/


// INCLUDE FILES
#include    "CSatEventMonitorHandler.h"
#include    "MSatEventMonitorUtils.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// CSatEventMonitorHandler::CSatEventMonitorHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatEventMonitorHandler::CSatEventMonitorHandler(
    MSatEventMonitorUtils& aUtils ) :
    iEventUtils( aUtils )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatEventMonitorHandler::\
        CSatEventMonitorHandler calling-exiting" )
    }

// Destructor
CSatEventMonitorHandler::~CSatEventMonitorHandler()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatEventMonitorHandler::\
        ~CSatEventMonitorHandler calling-exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorHandler::BaseConstructL
// Constructor of this class.
// -----------------------------------------------------------------------------
//
void CSatEventMonitorHandler::BaseConstructL( MSatEventMonitor::TEvent aEvent )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatEventMonitorHandler::\
        BaseConstructL calling" )

    iIsSimRemovalMonitor = EFalse;
    iSimRemovalMonitorActive = EFalse;

    // Resolve correct event
    switch ( aEvent )
        {
        case MSatEventMonitor::EUserActivity:
            {
            LOG( NORMAL, "SATEVENTMONITORS: CSatEventMonitorHandler::\
                BaseConstructL User activity" )
            iEvent = RSat::KUserActivity;
            break;
            }

        case MSatEventMonitor::EIdleScreenAvailable:
            {
            LOG( NORMAL, "SATEVENTMONITORS: CSatEventMonitorHandler::\
                BaseConstructL Idle Screen available" )
            iEvent = RSat::KIdleScreenAvailable;
            break;
            }

        case MSatEventMonitor::ELanguageSelection:
            {
            LOG( NORMAL, "SATEVENTMONITORS: CSatEventMonitorHandler::\
                BaseConstructL Language selection" )
            iEvent = RSat::KLanguageSelection;
            break;
            }

        case MSatEventMonitor::ESimRemoved:
            {
            LOG( NORMAL, "SATEVENTMONITORS: CSatEventMonitorHandler::\
                BaseConstructL SIM removal" )
            // There is no event for sim removal event.
            // The observer of events should cancel all monitors
            // if sim is removed.
            iIsSimRemovalMonitor = ETrue;
            break;
            }

        case MSatEventMonitor::EBrowserTerminated:
            {
            LOG( NORMAL, "SATEVENTMONITORS: CSatEventMonitorHandler::\
                BaseConstructL Browser termination" )
            iEvent = RSat::KBrowserTermination;
            break;
            }

        case MSatEventMonitor::EDataAvailable:
            {
            LOG( NORMAL, "SATEVENTMONITORS: CSatEventMonitorHandler::\
                BaseConstructL Data available" )
            iEvent = RSat::KDataAvailable;
            break;
            }

        case MSatEventMonitor::EChannelStatus:
            {
            LOG( NORMAL, "SATEVENTMONITORS: CSatEventMonitorHandler::\
                BaseConstructL Channel status" )
            iEvent = RSat::KChannelStatus;
            break;
            }

        default:
            {
            LOG( SIMPLE, "SATEVENTMONITORS: CSatEventMonitorHandler::\
                BaseConstructL No implementation" )
            }
        }

    LOG( SIMPLE, "SATEVENTMONITORS: CSatEventMonitorHandler::\
        BaseConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorHandler::CancelMonitor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSatEventMonitorHandler::CancelMonitor()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatEventMonitorHandler::\
        CancelMonitor calling" )

    DoCancelMonitor();

    LOG( SIMPLE, "SATEVENTMONITORS: CSatEventMonitorHandler::\
        CancelMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorHandler::UpdateMonitor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSatEventMonitorHandler::UpdateMonitor(
    TUint aEvents )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatEventMonitorHandler::\
        UpdateMonitor calling" )

    TInt errCode = KErrNone;

    LOG2( DETAILED, "SATEVENTMONITORS: CSatEventMonitorHandler::aEvents = %d",
        aEvents )
    LOG2( DETAILED, "SATEVENTMONITORS: CSatEventMonitorHandler::iEvent = %d",
        iEvent )

    // There is no specific event ID for SIM removal monitor,
    // so we have to check this different than others
    if ( iIsSimRemovalMonitor )
        {
        LOG( NORMAL, "SATEVENTMONITORS: CSatEventMonitorHandler::\
            IsSimRemovalMonitor" )

        // If events list is not empty, start
        // SIM removal monitor. Otherwise cancel it.
        if ( aEvents != 0 )
            {
            LOG( NORMAL, "SATEVENTMONITORS: CSatEventMonitorHandler::\
                IsSimRemovalMonitor - Events list is not empty" )
            // Start SIM removal monitor only if it is not already active
            if ( !iSimRemovalMonitorActive )
                {
                LOG( NORMAL, "SATEVENTMONITORS: CSatEventMonitorHandler::\
                    IsSimRemovalMonitor - Start SIM removal monitor" )
                errCode = StartMonitor();
                iSimRemovalMonitorActive = ETrue;
                }
            }
        else
            {
            LOG( NORMAL, "SATEVENTMONITORS: CSatEventMonitorHandler::\
                IsSimRemovalMonitor - Cancel monitor" )
            CancelMonitor();
            iSimRemovalMonitorActive = EFalse;
            }
        }

    // Check is derived monitor in list
    else if ( aEvents & iEvent )
        {
        LOG( NORMAL, "SATEVENTMONITORS: CSatEventMonitorHandler::\
            UpdateMonitor Starting monitor" )
        // Monitor found, start monitor
        errCode = StartMonitor();
        }
    else
        {
        LOG( NORMAL, "SATEVENTMONITORS: CSatEventMonitorHandler::\
            UpdateMonitor Cancelling monitor" )
        // Monitor not in list, cancel monitor
        CancelMonitor();
        }

    LOG2( SIMPLE, "SATEVENTMONITORS: CSatEventMonitorHandler::\
        UpdateMonitor exiting with value: %d", errCode )
    return errCode;
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorHandler::IsMonitoringEvent
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSatEventMonitorHandler::IsMonitoringEvent( const TInt aEvent )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatEventMonitorHandler::\
        IsMonitoringEvent calling-exiting" )
    return ( aEvent == iEvent );
    }

// -----------------------------------------------------------------------------
// CSatEventMonitorHandler::EventOccured
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatEventMonitorHandler::EventOccured( TInt aIntArg, TInt aSecArg )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatEventMonitorHandler::\
        EventOccured calling" )

    // Check is this SimRemoval monitor.
    if ( iIsSimRemovalMonitor )
        {
        LOG( SIMPLE, "SATEVENTMONITORS: CSatEventMonitorHandler::\
        EventOccured iIsSimRemovalMonitor true" )
        // Yes, Inform observer that SIM is removed
        iEventUtils.SimRemoved();
        }
    else
        {
        // No, Inform normal event occurrence
        iEventUtils.EventDownload( iEvent, aIntArg, aSecArg );
        }

    LOG( SIMPLE, "SATEVENTMONITORS: CSatEventMonitorHandler::\
        EventOccured exiting" )
    }

//  End of File
