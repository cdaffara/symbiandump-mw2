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
* Description:  This module contains the implementation of CNWNetworkMcnTimer
*                class member functions.
*
*/



// INCLUDE FILES
#include    "CNWNetworkMcnTimer.h"
#include    "MNWNetworkTimer.h"
#include    "NWLogger.h"


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWNetworkMcnTimer::CNWNetworkMcnTimer
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkMcnTimer::CNWNetworkMcnTimer( 
    TInt aPriority ) 
    : CTimer( aPriority)
    {
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkMcnTimer::CNWNetworkMcnTimer() Begin " );

    CActiveScheduler::Add( this );
    
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkMcnTimer::CNWNetworkMcnTimer() End " );
    }
    
// ----------------------------------------------------------------------------
// CNWNetworkMcnTimer::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWNetworkMcnTimer::ConstructL()
    {
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkMcnTimer::ConstructL() Begin " );
    
    CTimer::ConstructL();
    
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkMcnTimer::ConstructL() End " );
    }

// ----------------------------------------------------------------------------
// CNWNetworkMcnTimer::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CNWNetworkMcnTimer* CNWNetworkMcnTimer::NewL( TInt aPriority )
    {
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkMcnTimer::NewL() Begin " );
    
    CNWNetworkMcnTimer* self = new (ELeave) CNWNetworkMcnTimer( aPriority );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkMcnTimer::NewL() End " );
    return self;
    }

    
// Destructor
CNWNetworkMcnTimer::~CNWNetworkMcnTimer()
    {
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkMcnTimer::~CNWNetworkMcnTimer() Begin " );
    
    CTimer::Cancel();
    
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkMcnTimer::~CNWNetworkMcnTimer() End " );
    }//lint !e1740 iTimerObserver is never allocated.


// ----------------------------------------------------------------------------
// CNWNetworkMcnTimer::RunL
// Receives the completion of the pending async. command.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkMcnTimer::RunL()
    {
    NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkMcnTimer::RunL() Begin,\
             iStatus.Int() = %d ", iStatus.Int() );
    
    if ( iStatus != KErrNone )
        {
        //error code is ignored, as CNWriodic. 
        return;
        }
   
    if( !iTimerObserver )
        {
        NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkMcnTimer::RunL(), timer not exists. " );
        iCallBack.CallBack();
        }
    else
        {
        iTimerObserver->HandleTimeOut();
        }
    
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkMcnTimer::RunL() End " );
    }

// ----------------------------------------------------------------------------
// CNWNetworkMcnTimer::After
// The Callback function is called after the interval. All former 
// requests will be canceled first.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkMcnTimer::After( 
    const TTimeIntervalMicroSeconds32& anInterval, 
    const TCallBack& aCallBack )
    {
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkMcnTimer::After() Begin " );
    
    if ( IsActive() )
        {
        Cancel();
        }
    iTimerObserver = NULL;
    iCallBack = aCallBack;
    CTimer::After( anInterval );
    
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkMcnTimer::After() End " );
    }

// ----------------------------------------------------------------------------
// CNWNetworkMcnTimer::After
// Uses the mixin class to notify the timer. All former requests will be
// canceled first.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkMcnTimer::After( 
    const TTimeIntervalMicroSeconds32& anInterval, 
    MNWNetworkTimer* aObserver )
    {
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkMcnTimer::After() Begin " );
    
    if ( IsActive() )
        {
        Cancel();
        }
    iTimerObserver = aObserver;
    CTimer::After( anInterval );
    
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkMcnTimer::After() End " );
    }

//  End of File  
