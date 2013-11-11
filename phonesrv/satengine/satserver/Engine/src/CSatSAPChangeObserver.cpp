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
* Description:  Observes SIM Access Profile status changes.
*
*/


#include    <MSatShellController.h>
#include    <BTSapDomainPSKeys.h>
#include    "MSatUiSession.h"
#include    "MSatSUiClientHandler.h"
#include    "TSatSystemStateFactory.h"
#include    "MSatSystemStateChangeNotifier.h"
#include    "CSatSAPChangeObserver.h"
#include    "SatLog.h"
#include    "CSatCommandContainer.h"
#include    "CSatSUiClientHandler.h"


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatSAPChangeObserver::CSatSAPChangeObserver(
    CSatCommandContainer& aContainer ) :
    iContainer( aContainer )
    {
    LOG( SIMPLE, "SATENGINE: CSatSAPChangeObserver::CSatSAPChangeObserver\
         calling-exiting" )
    }

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatSAPChangeObserver* CSatSAPChangeObserver::NewL(
    CSatCommandContainer& aContainer )
    {
    LOG( SIMPLE, "SATENGINE: CSatSAPChangeObserver::NewL calling" )

    CSatSAPChangeObserver* self =
        new ( ELeave ) CSatSAPChangeObserver( aContainer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATENGINE: CSatSAPChangeObserver::NewL exiting" )
    return self;
    }


// Destructor
CSatSAPChangeObserver::~CSatSAPChangeObserver()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSAPChangeObserver::~CSatSAPChangeObserver calling" )

    if ( iStateNotifier )
        {
        iStateNotifier->CancelNotify();
        delete iStateNotifier;
        iStateNotifier = NULL;
        }

    LOG( SIMPLE,
        "SATENGINE: CSatSAPChangeObserver::~CSatSAPChangeObserver exiting" )
    }

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CSatSAPChangeObserver::ConstructL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSAPChangeObserver::ConstructL calling" )

    iStateNotifier =
        TSatSystemStateFactory::CreateSIMAccessProfileChangeNotifierL( *this );
    // Start observer immediately
    iStateNotifier->NotifyChangeL();

    LOG( SIMPLE, "SATENGINE: CSatSAPChangeObserver::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CSatSAPChangeObserver::StateChanged( const TInt aValue )
    {
    LOG2( SIMPLE,
        "SATENGINE: CSatSAPChangeObserver::StateChanged calling with value %d",
        aValue )

    TInt error( KErrNone );

    // If value is true, stop all command handlers and remove
    // SAT Icon from Application Shell.
    if ( EBTSapConnecting == aValue ||
         EBTSapConnected == aValue )
        {
        // SIM Access Profile activated.
        // Notify listeners about SAT UI icon removing from Application Shell.
        // This should be notified before command handlers are stopped.
        iContainer.NotifyEvent( MSatUtils::ERemoveSatUiCalled );

        // Then stop command handlers and remove SAT UI from Application Shell.
        iContainer.StopCommandHandlers();
        }
    else if ( EBTSapNotConnected == aValue )
        {
        LOG( SIMPLE, "SATENGINE: CSatSAPChangeObserver::StateChanged \
             EBTSapNotConnected" )
        // Start command handlers
        TRAP( error,
            iContainer.StartCommandHandlersL();
            ); // End of TRAP

        if ( KErrNone != error )
            {
            LOG2( SIMPLE, "SATENGINE: CSatSAPChangeObserver::StateChanged \
                failed to recover from SAP: %i", error )
            }

        // Notify listeners about SAT UI icon adding.
        iContainer.NotifyEvent( MSatUtils::EAddSatUiCalled );
        }
    else
        {
        LOG( SIMPLE, "SATENGINE:   Unexpected value" )
        }
    
    LOG2( SIMPLE, "SATENGINE: CSatSAPChangeObserver::StateChanged exiting, \
          error: %d",error )
    }
