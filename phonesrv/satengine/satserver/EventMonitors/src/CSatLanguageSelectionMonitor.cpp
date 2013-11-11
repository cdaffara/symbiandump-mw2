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
#include    "MSatEventMonitorUtils.h"
#include    "CSatLanguageSelectionMonitor.h"
#include    "SatLog.h"

// CONSTANTS
const TInt KSecond( 1000 );

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// CSatLanguageSelectionMonitor::CSatLanguageSelectionMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatLanguageSelectionMonitor::CSatLanguageSelectionMonitor(
    MSatEventMonitorUtils& aUtils ) :
    CSatEventMonitorHandler( aUtils )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatLanguageSelectionMonitor::\
        CSatLanguageSelectionMonitor calling-exiting" )
    }

// Destructor
CSatLanguageSelectionMonitor::~CSatLanguageSelectionMonitor()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatLanguageSelectionMonitor::\
        ~CSatLanguageSelectionMonitor calling" )

    delete iPSNotifier;

    LOG( SIMPLE, "SATEVENTMONITORS: CSatLanguageSelectionMonitor::\
        ~CSatLanguageSelectionMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatLanguageSelectionMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatLanguageSelectionMonitor* CSatLanguageSelectionMonitor::NewL(
    MSatEventMonitorUtils& aUtils )
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatLanguageSelectionMonitor::NewL calling" )

    CSatLanguageSelectionMonitor* self =
        new( ELeave ) CSatLanguageSelectionMonitor( aUtils );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatLanguageSelectionMonitor::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatLanguageSelectionMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSatLanguageSelectionMonitor::ConstructL()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatLanguageSelectionMonitor::ConstructL calling" )

    // Create base class and give this monitors event as a parameter
    BaseConstructL( MSatEventMonitor::ELanguageSelection );
    iPSNotifier =
        TSatSystemStateFactory::CreateLanguageSelectionChangeNotifierL( *this );

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatLanguageSelectionMonitor::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatLanguageSelectionMonitor::DoCancelMonitor
// Cancels monitor.
// -----------------------------------------------------------------------------
//
void CSatLanguageSelectionMonitor::DoCancelMonitor()
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CSatLanguageSelectionMonitor::\
        DoCancelMonitor calling" )

    iPSNotifier->CancelNotify();
    iEventUtils.WriteLanguageSelectionTime( KErrNone );

    LOG( SIMPLE, "SATEVENTMONITORS: CSatLanguageSelectionMonitor::\
        DoCancelMonitor exiting" )
    }

// -----------------------------------------------------------------------------
// CSatLanguageSelectionMonitor::StartMonitor
// Starts monitor.
// -----------------------------------------------------------------------------
//
TInt CSatLanguageSelectionMonitor::StartMonitor()
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatLanguageSelectionMonitor::StartMonitor calling" )

    // Cancel first, it might be activated already
    DoCancelMonitor();

    // Now start
    TRAPD( err, iPSNotifier->NotifyChangeL() );
    iEventUtils.WriteLanguageSelectionTime( KSecond );

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatLanguageSelectionMonitor::StartMonitor exiting" )
    return err;
    }

// -----------------------------------------------------------------------------
// CSatLanguageSelectionMonitor::StateChanged
// Called when SIM is removed.
// -----------------------------------------------------------------------------
//
void CSatLanguageSelectionMonitor::StateChanged( const TInt aValue )
    {
    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatLanguageSelectionMonitor::StateChanged calling" )

    // Inform SAT Engine that language is been changed.
    EventOccured( aValue );

    LOG( SIMPLE,
        "SATEVENTMONITORS: CSatLanguageSelectionMonitor::StateChanged exiting" )
    }

// End of file
