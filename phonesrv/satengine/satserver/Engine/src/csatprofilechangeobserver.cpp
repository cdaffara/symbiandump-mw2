/*
* Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observes Profile state changes.
*
*/


// INCLUDE FILES
#include    <coreapplicationuissdkcrkeys.h>
#include    "TSatSystemStateFactory.h"
#include    "MSatSystemStateChangeNotifier.h"
#include    "MSatUtils.h"
#include    "csatprofilechangeobserver.h"
#include    "SatLog.h"
#include    "CSatSUiClientHandler.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatProfileChangeObserver::CSatProfileChangeObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatProfileChangeObserver::CSatProfileChangeObserver(
    MSatUtils& aUtils ) :
    iUtils( aUtils )
    {
    }

// -----------------------------------------------------------------------------
// CSatProfileChangeObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatProfileChangeObserver* CSatProfileChangeObserver::NewL(
    MSatUtils& aUtils )
    {
    LOG( SIMPLE, "SATENGINE: CSatProfileChangeObserver::NewL calling" )

    CSatProfileChangeObserver* self =
        new ( ELeave ) CSatProfileChangeObserver( aUtils );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATENGINE: CSatProfileChangeObserver::NewL exiting" )
    return self;
    }

// Destructor
CSatProfileChangeObserver::~CSatProfileChangeObserver()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatProfileChangeObserver::~CSatProfileChangeObserver \
        calling" )

    if ( iStateNotifier )
        {
        iStateNotifier->CancelNotify();
        delete iStateNotifier;
        iStateNotifier = NULL;
        }

    LOG( SIMPLE,
        "SATENGINE: CSatProfileChangeObserver::~CSatProfileChangeObserver \
        exiting" )
    }
    
// -----------------------------------------------------------------------------
// CSatProfileChangeObserver::ConstructL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CSatProfileChangeObserver::ConstructL()
    {
    LOG( SIMPLE, "SATENGINE: CSatProfileChangeObserver::ConstructL calling" )

    iStateNotifier = TSatSystemStateFactory::CreateProfileChangeNotifierL( *this );
    // Start observer immediately
    iStateNotifier->NotifyChangeL();
    
    // iPreviousProfile must set to -1 because values 0-49 
    // are reserved for different profiles.
    iPreviousProfile = -1;
    
    LOG( SIMPLE, "SATENGINE: CSatSAPChangeObserver::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatProfileChangeObserver::StateChanged
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CSatProfileChangeObserver::StateChanged( const TInt aValue )
    {
    LOG2( SIMPLE, "SATENGINE: CSatProfileChangeObserver::StateChanged calling \
        with value %d", aValue )

    // If profile is off-line send removing event
    if ( ECoreAppUIsNetworkConnectionNotAllowed == aValue )
        {
        LOG( SIMPLE, "SATENGINE: Off-line" )
        // Off-line state activated.
        // Notify listeners about SAT UI icon removing
        iUtils.NotifyEvent( MSatUtils::ERemoveSatUiCalled );       
        }

    // Send event if previous state was offline.
    // The event will not be sent in following case: Phone is started
    // in offline mode and profile is changed to online. Proactive 
    // SetupMenu command is received from SIM card not before phone is
    // in online mode. iPreviousProfile is -1 when StateChanged is called
    // first time.
    else if ( ( ECoreAppUIsNetworkConnectionNotAllowed != aValue ) &&  
        ( ECoreAppUIsNetworkConnectionNotAllowed  == iPreviousProfile ) )
        {
        LOG( SIMPLE, "SATENGINE: !Off-line" )
        // Notify listeners about SAT UI icon adding
        iUtils.NotifyEvent( MSatUtils::EAddSatUiCalled );
        }
    
    else
        {
        // meaningless else. Only for Lint
        }

    iPreviousProfile = aValue;

    LOG( SIMPLE, "SATENGINE: CSatProfileChangeObserver::StateChanged exiting" )
    }

// End of file
