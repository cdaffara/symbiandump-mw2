/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Active object wrapper for SAT Server
*
*/


#include    "csatsactivewrapper.h"
#include    "SatLog.h"

const TInt KShortWait = 200; // 200 microseconds
const TInt KWaitTimes = 5;   // Number of retries

// ================= MEMBER FUNCTIONS ==========================================

// Class constructor.
CSatSActiveWrapper::CSatSActiveWrapper() :
    CActive( EPriorityStandard )
    {
    LOG( SIMPLE, "SATENGINE: CSatSActiveWrapper::CSatSActiveWrapper \
    calling-exiting" )
    CActiveScheduler::Add( this );
    }

// Destructor
CSatSActiveWrapper::~CSatSActiveWrapper()
    {
    LOG( SIMPLE, "SATENGINE: CSatSActiveWrapper::~CSatSActiveWrapper calling" )

    if ( iTimer )
        {
        iTimer->Cancel();
        delete iTimer;
        }

    LOG( SIMPLE, "SATENGINE: CSatSActiveWrapper::~CSatSActiveWrapper exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSActiveWrapper::RequestStatus
// -----------------------------------------------------------------------------
//
TRequestStatus& CSatSActiveWrapper::RequestStatus()
    {
    LOG( DETAILED, "SATENGINE: CSatSActiveWrapper::RequestStatus calling" )

    TInt retries( 0 );
    while ( IsActive() && ( retries++ < KWaitTimes ) )
        {
        // If this wrapper is already active, wait little bit
        After( KShortWait );
        }
    LOG2( DETAILED, "SATENGINE: CSatSActiveWrapper::RequestStatus retries: %d",
          retries )
    // Double check. If still active, cancel
    if ( IsActive() )
        {
        LOG( SIMPLE, "SATENGINE: CSatSActiveWrapper::RequestStatus active" )
        CancelWrapper();
        }

    LOG( DETAILED, "SATENGINE: CSatSActiveWrapper::RequestStatus exiting" )
    return iStatus;
    }
// -----------------------------------------------------------------------------
// CSatSActiveWrapper::SetActiveAndWait
// -----------------------------------------------------------------------------
//
TInt CSatSActiveWrapper::SetActiveAndWait()
    {
    LOG( DETAILED, "SATENGINE: CSatSActiveWrapper::SetActiveAndWait calling" )

    TInt returnValue( KErrInUse );
    if ( !IsActive() && !iWait.IsStarted() )
        {
        LOG( DETAILED,
        "SATENGINE: CSatSActiveWrapper::SetActiveAndWait setActiveAndWait" )
        SetActive();
        iWait.Start(); // Blocks until request is complete or cancelled
        returnValue = iStatus.Int();
        }

    LOG( DETAILED, "SATENGINE: CSatSActiveWrapper::SetActiveAndWait exiting" )
    return returnValue;
    }

// -----------------------------------------------------------------------------
// CSatSActiveWrapper::After
// -----------------------------------------------------------------------------
void CSatSActiveWrapper::After(
    const TTimeIntervalMicroSeconds32& aDelay,
    const TTimeIntervalMicroSeconds32& aInterval )
    {
    LOG( SIMPLE, "SATENGINE: CSatSActiveWrapper::After calling" )

    if ( !iAfterWait.IsStarted() )
        {
        LOG( SIMPLE, "SATENGINE: CSatSActiveWrapper::After start iAfterWait" )
        TRAP_IGNORE( ( iTimer = CPeriodic::NewL( EPriorityStandard ) ) )
        iTimer->Start( aDelay, aInterval, TCallBack( DelayCallBack, this ) );
        iAfterWait.Start();

        delete iTimer;
        iTimer = NULL;
        }

    LOG( SIMPLE, "SATENGINE: CSatSActiveWrapper::After exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSActiveWrapper::DelayCallBack
// Timer call back function
// -----------------------------------------------------------------------------
TInt CSatSActiveWrapper::DelayCallBack( TAny* aPtr )
    {
    LOG( SIMPLE, "SATENGINE: CSatSActiveWrapper::DelayCallBack calling" )
    CSatSActiveWrapper* ptrThis = static_cast<CSatSActiveWrapper*>( aPtr );
    if ( ptrThis && ( ptrThis->iAfterWait.IsStarted() ) )
        {
        LOG( SIMPLE, "SATENGINE: CSatSActiveWrapper::DelayCallBack stop \
             iAfterWait" )
        ptrThis->iAfterWait.AsyncStop();
        }
    LOG( SIMPLE, "SATENGINE: CSatSActiveWrapper::DelayCallBack exiting" )
    return ( EFalse );
    }

// -----------------------------------------------------------------------------
// CSatSActiveWrapper::AddSubSessionL
// -----------------------------------------------------------------------------
void CSatSActiveWrapper::CancelWrapper()
    {
    LOG( SIMPLE, "SATENGINE: CSatSActiveWrapper::CancelWrapper calling" )

    // If pending for request, cancel active
    if ( IsActive() )
        {
        LOG( SIMPLE, 
        "SATENGINE: CSatSActiveWrapper::CancelWrapper cancel iWait" )
        // Calls DoCancel which cancels iWait
        Cancel();
        }
    else if ( iAfterWait.IsStarted() )
        {
        LOG( SIMPLE, 
        "SATENGINE: CSatSActiveWrapper::CancelWrapper cancel timer" )
        // Cancels timer
        iAfterWait.AsyncStop();
        }
    else
        {
        LOG( SIMPLE, 
        "SATENGINE: CSatSActiveWrapper::CancelWrapper Wrapper not active" )
        }

    LOG( SIMPLE, "SATENGINE: CSatSActiveWrapper::CancelWrapper exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSActiveWrapper::Release
// -----------------------------------------------------------------------------
void CSatSActiveWrapper::Release()
    {
    LOG( SIMPLE, "SATENGINE: CSatSActiveWrapper::Release calling-exiting" )
    delete this;
    }

// -----------------------------------------------------------------------------
// CSatSActiveWrapper::RunL
// Synchronous request complete
// -----------------------------------------------------------------------------
//
void CSatSActiveWrapper::RunL()
    {
    LOG( DETAILED, "SATENGINE: CSatSActiveWrapper::RunL calling" )

    iWait.AsyncStop();

    LOG( DETAILED, "SATENGINE: CSatSActiveWrapper::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSActiveWrapper::DoCancel
// Synchronous request complete
// -----------------------------------------------------------------------------
//
void CSatSActiveWrapper::DoCancel()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::DoCancel calling" )

    iWait.AsyncStop();

    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::DoCancel exiting" )
    }
