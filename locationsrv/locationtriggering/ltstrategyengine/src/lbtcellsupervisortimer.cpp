/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class definition of triggering timer
*
*/


// INCLUDE FILES
#include "lbtcellsupervisortimer.h"
#include "lbtlogger.h"


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CLbtCellSupervisorTimer::NewL 
// -----------------------------------------------------------------------------
//
CLbtCellSupervisorTimer* CLbtCellSupervisorTimer::NewL( MLbtCellTimerObserver& aObserver )
    {
    CLbtCellSupervisorTimer* self = new (ELeave) CLbtCellSupervisorTimer( aObserver );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CLbtCellSupervisorTimer::CLbtCellSupervisorTimer 
// -----------------------------------------------------------------------------
//
CLbtCellSupervisorTimer::CLbtCellSupervisorTimer( MLbtCellTimerObserver& aObserver ) :
    CTimer(EPriorityStandard),iObserver( aObserver )
    {
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CLbtCellSupervisorTimer::~CLbtCellSupervisorTimer 
// -----------------------------------------------------------------------------
//
CLbtCellSupervisorTimer::~CLbtCellSupervisorTimer()
    {
    if (IsActive())
        {
        Cancel();
        }
    }

// -----------------------------------------------------------------------------
// CLbtCellSupervisorTimer::ConstructL 
// -----------------------------------------------------------------------------
//
void CLbtCellSupervisorTimer::ConstructL()
    {
    CTimer::ConstructL();
    }
        

// -----------------------------------------------------------------------------
// CLbtCellSupervisorTimer::RunL 
// -----------------------------------------------------------------------------
//
void CLbtCellSupervisorTimer::RunL()
    {
    FUNC_ENTER("CLbtCellSupervisorTimer::RunL");
    if ( iStatus.Int() == KErrNone )
    	{
    	iObserver.TimeOut();
    	}
    }


// -----------------------------------------------------------------------------
// CLbtCellSupervisorTimer::SetTimer
// -----------------------------------------------------------------------------
//
void CLbtCellSupervisorTimer::SetTimer(TInt aSeconds)
	{
	FUNC_ENTER("CLbtCellSupervisorTimer::SetTimer");
	iSleepTime = aSeconds; 
	iStartTime.UniversalTime();
	
	if(!IsActive())
		{
		After(1000000 * iSleepTime );
		}	
	}
// -----------------------------------------------------------------------------
// CLbtCellSupervisorTimer::RemainingTime
// -----------------------------------------------------------------------------
//
TInt CLbtCellSupervisorTimer::RemainingTime()
    {
    FUNC_ENTER("CLbtCellSupervisorTimer::RemainingTime");
    TTime currentTime;
	TTimeIntervalSeconds interval;
	currentTime.UniversalTime();
	currentTime.SecondsFrom( iStartTime, interval );
    return( iSleepTime - interval.Int());
    }

// -----------------------------------------------------------------------------
// CLbtCellSupervisorTimer::InterruptSleep
// -----------------------------------------------------------------------------
//
void CLbtCellSupervisorTimer::InterruptSleep( TInt aNewSleepTime )
    {
    FUNC_ENTER("CLbtCellSupervisorTimer::InterruptSleep");
    iSleepTime = aNewSleepTime;
    if (IsActive())
        {
        Cancel();
        }
    SetTimer( aNewSleepTime );
    }
// end of file
