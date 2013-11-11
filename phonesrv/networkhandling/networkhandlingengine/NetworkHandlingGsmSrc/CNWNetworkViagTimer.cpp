/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CNWNetworkCurrentNetworkMonitor
*                class member functions.
*
*/



// INCLUDE FILES
#include    "CNWNetworkViagTimer.h"
#include    "MNWNetworkTimer.h"
#include    "NWLogger.h"


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWNetworkViagTimer::CNWNetworkViagTimer
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkViagTimer::CNWNetworkViagTimer(
    TInt aPriority ) 
    : CTimer( aPriority)
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagTimer::CNWNetworkViagTimer() Begin" );
    
    CActiveScheduler::Add( this );
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagTimer::CNWNetworkViagTimer() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagTimer::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWNetworkViagTimer::ConstructL()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagTimer::ConstructL() Begin" );
    
    CTimer::ConstructL();
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagTimer::ConstructL() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagTimer::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CNWNetworkViagTimer* CNWNetworkViagTimer::NewL( TInt aPriority )
    {
    NWLOGSTRING2( KNWOBJECT,
        "NW: CNWNetworkViagTimer::NewL() Begin,\
        aPriority = %d ", aPriority );
    
    CNWNetworkViagTimer* self = new (ELeave) CNWNetworkViagTimer( aPriority );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagTimer::NewL() End" );
    
    return self;
    }

    
// Destructor
CNWNetworkViagTimer::~CNWNetworkViagTimer()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagTimer::~CNWNetworkViagTimer() Begin" );
    
    CTimer::Cancel();
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagTimer::~CNWNetworkViagTimer() End" );
    }//lint !e1740 iTimerObserver is never allocated.


// ----------------------------------------------------------------------------
// CNWNetworkViagTimer::RunL
// Receives the completion of the pending async. command.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkViagTimer::RunL()
    {
    NWLOGSTRING2( KNWOBJECT,
        "NW: CNWNetworkViagTimer::RunL() Begin,\
        iStatus.Int() = %d", iStatus.Int() );
    
    if ( iStatus != KErrNone )
        {
        //error code is ignored, as CNWriodic. 
        NWLOGSTRING( KNWERROR,
            "NW: CNWNetworkViagTimer::~RunL(),\
            iStaus error found" );
        return;
        }
   
    iTimerObserver->HandleTimeOut();
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagTimer::RunL() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagTimer::After
// Uses the mixin class to notify the timer. All former requests will be
// canceled first.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkViagTimer::After( 
    const TTimeIntervalMicroSeconds32& anInterval,
    MNWNetworkTimer* aObserver )
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagTimer::After() Begin" );
    
    if ( IsActive() )
        {
        NWLOGSTRING( KNWOBJECT,
            "NW: CNWNetworkViagTimer::After(), Timer \
            already exists!" );
        Cancel();
        }
    iTimerObserver = aObserver;
    CTimer::After( anInterval );
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagTimer::After() End" );
    }

//  End of File  
