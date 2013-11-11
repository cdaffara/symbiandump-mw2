/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This class implements duration timer for calls.
*
*/


// INCLUDE FILES
#include    "mccecallobserver.h"
#include    "mcceconferencecallobserver.h"
#include    "cccedurationtimer.h"
#include    "cccelogger.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCCEDurationTimer::CCCEDurationTimer
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------
//
CCCEDurationTimer::CCCEDurationTimer()
    {
    iObserver           = NULL;
    iConferenceCallObserver = NULL;
    iActive             = EFalse;
    iNumberOfBeats      = 0;
    iStartTime          = 0;
    iAirTimeDuration    = EFalse;
    }

// ---------------------------------------------------------
// CCCEDurationTimer::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------
//
void CCCEDurationTimer::ConstructL()
    {
    iHeartbeat = CHeartbeat::NewL( CActive::EPriorityStandard );
    }

// -----------------------------------------------------------------------------
// CCCEDurationTimer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCCEDurationTimer* CCCEDurationTimer::NewL()
    {
    CCCEDurationTimer* runner = new (ELeave) CCCEDurationTimer();
    CleanupStack::PushL( runner );
    runner->ConstructL();
    CleanupStack::Pop( runner );

    return runner;
    }

// ---------------------------------------------------------
// CCCEDurationTimer::~CCCEDurationTimer
// Destructor.
// ---------------------------------------------------------    
//
CCCEDurationTimer::~CCCEDurationTimer()
    {
    iObserver = NULL;
    iConferenceCallObserver = NULL;
    
    if ( iHeartbeat )
        {
        //stop calling Beat...
        iHeartbeat->Cancel();
        }
    delete iHeartbeat;
    }

// ---------------------------------------------------------
// CCCEDurationTimer::SetObserver
// ---------------------------------------------------------    
//
void CCCEDurationTimer::SetObserver( MCCECallObserver* aObserver )
    {
    iObserver = aObserver;
    }

// ---------------------------------------------------------
// CCCEDurationTimer::SetObserver
// ---------------------------------------------------------    
//
void CCCEDurationTimer::SetConferenceObserver( MCCEConferenceCallObserver* aObserver )
    {
    iConferenceCallObserver = aObserver;
    }

// -----------------------------------------------------------------------------
// CCCEDurationTimer::Start
//    Overload to resetting iNumberOfBeats 
//
// -----------------------------------------------------------------------------
//
void CCCEDurationTimer::Start()
    {
    Start(0);
   
    }
    
// -----------------------------------------------------------------------------
// CCCEDurationTimer::Start
//    if timer is not already active
//        set iStarTime to current time
//        set iNumberOfBeats to aBeatsOffset
//        start timer
//        set value ETrue to iActive
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCCEDurationTimer::Start(TInt aBeatsOffset)
    {
    //Check if active 
    if ( !iActive ) 
        {
        //Initialize iStartime
        iStartTime.HomeTime();
        //Clear number of beats
        iNumberOfBeats = aBeatsOffset;
        //Timer tick is on the second 
        iHeartbeat->Start( ETwelveOClock, this );
        //Set flag to indicate that timer is active
        iActive = ETrue;
        //Set flag to indicate that air time duration counter is not started
        iAirTimeDuration = EFalse;
        }
        
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//  
void CCCEDurationTimer::RestartAirTimeDuration(TInt aBeatsOffset)
    {
    //Check if active 
    if ( !iActive ) 
        {
        // Set number of beats
        iNumberOfBeats = aBeatsOffset;
        //Timer tick is on the second 
        iHeartbeat->Start( ETwelveOClock, this );
        //Set flag to indicate that timer is active
        iActive = ETrue;
        //Set flag to indicate that air time duration counter is started
        iAirTimeDuration = ETrue;
        }
    }
    
// -----------------------------------------------------------------------------
// CCCEDurationTimer::StartAirTimeDuration
//    if timer is not already active
//        set iStarTime to current time
//        start timer
//        set value ETrue to iActive
//        set value ETrue to iAirTimeDuration
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCCEDurationTimer::StartAirTimeDuration()
    {
    //Check if active 
    if ( !iActive ) 
        { 
        //Initialize iStartime
        iStartTime.HomeTime();
        // Clear number of beats
        iNumberOfBeats = 0;
        //Timer tick is on the second 
        iHeartbeat->Start( ETwelveOClock, this );
        //Set flag to indicate that timer is active
        iActive = ETrue;
        //Set flag to indicate that air time duration counter is started
        iAirTimeDuration = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CCCEDurationTimer::StartTime
// Returns current date and time object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TDateTime CCCEDurationTimer::StartTime()
    {
    return iStartTime.DateTime();
    }

// -----------------------------------------------------------------------------
// CCCEDurationTimer::Stop
// Stops timer
//
//    store iActive in ret boolean
//    if timer is active
//        reset iStartTime
//        stop timer.
//        set value EFalse to iActive
//    return ret
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCCEDurationTimer::Stop()
    {
    TBool ret = iActive;

    //Check if active 
    if ( iActive ) 
        { 
        //Reset iStartTime
        iStartTime = 0;
        //Cancel beat method calling 
        iHeartbeat->Cancel(); 
        //Set flag to indicate that timer is not active
        iActive = EFalse;
        }

    return ret;
    }

// -----------------------------------------------------------------------------
// CCCEDurationTimer::Beat
// Beat is called once every second. If iMmCall is not null
//    (timer is used for calculating the duration of a call), the method
//    CompleteNotifyCallDurationChange is called. The iNumberOfBeats is
//    increased to count the amount of seconds passed from the starting
//    of the timer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCCEDurationTimer::Beat()
    {
    iNumberOfBeats++;

    // If iOwner is NOT null, this timer is used to count call duration.
    if ( iObserver )
        {
        //complete notify duration change, 1 sec gone
        iObserver->CallDurationChanged( iNumberOfBeats );
        }

    // If iOwner is NOT null, this timer is used to count call duration.
    if ( iConferenceCallObserver )
        {
        //complete notify duration change, 1 sec gone
        iConferenceCallObserver->CallDurationChanged( iNumberOfBeats );
        }
    }

// -----------------------------------------------------------------------------
// CCCEDurationTimer::NumberOfBeats
// Returns number of beats from the previous Start of the timer.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

TInt CCCEDurationTimer::NumberOfBeats() const
    {
    return iNumberOfBeats;
    }
    
// -----------------------------------------------------------------------------
// CCCEDurationTimer::Reset
// Sets the number of beats to zero
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCCEDurationTimer::Reset() 
    {
    iNumberOfBeats = 0;
    }

// -----------------------------------------------------------------------------
// CCCEDurationTimer::Synchronize
// This methods is called when synchronization is lost (the 
// method Beat couldn't be called). It calculates the number
// of seconds passed after the start of the timer and assigns 
// it to iNumberOfBeats
//
//    if this is not air time duration counter
//        initialize local variables
//        get current home time
//        increase value of number of beats by one because at least one beat is 
//         missing
//        calculate difference of current home time and timer start time in 
//         seconds and save result in secondsFromStart
//        if secondsFromStart is positive
//            calculate difference of number of beats and secondsFromStart and 
//             save result in timeDifference
//            if timeDifference is less than 10
//                correct the deviation and set secondsFromStart in number of 
//                 beats
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCCEDurationTimer::Synchronize()
    {
    const TInt tenSeconds = 10;
    
    //we can't synchronize if this is air time duration counter
    if ( !iAirTimeDuration )
        {
        TInt timeDifference( 0 );
        //get the correct time
        TTime currentTime;
        currentTime.HomeTime();

        TTimeIntervalSeconds secondsFromStart( 0 );

        //At least one beat is now missing
        iNumberOfBeats++;

        //calculate the amount of seconds that should be
        currentTime.SecondsFrom( iStartTime, secondsFromStart );

        //calculate difference of iNumberOfBeats and secondsFromStart
        if ( 0 <= secondsFromStart.Int() )
            {
            if ( secondsFromStart.Int() > iNumberOfBeats )
                {
                timeDifference = secondsFromStart.Int() - iNumberOfBeats;
                }
            else if ( secondsFromStart.Int() < iNumberOfBeats )
                {
                timeDifference = iNumberOfBeats - secondsFromStart.Int();
                }

            //correct the deviation only if time difference is less than 10 
            //seconds
            if ( timeDifference < tenSeconds )
                {
                iNumberOfBeats = secondsFromStart.Int();
                }
            }
        }
    }

//  End of File 
