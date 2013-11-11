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
* Description:  Implementation of Timer class 
*
*/



#include "lbtstrategytimer.h"
#include "lbtlogger.h"


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CLbtStrategyTimer::NewL
// 
// -----------------------------------------------------------------------------
//
CLbtStrategyTimer* CLbtStrategyTimer::NewL()
    {
    CLbtStrategyTimer* timer = new ( ELeave ) CLbtStrategyTimer;
    CleanupStack::PushL( timer );
    timer->ConstructL();
    CleanupStack::Pop();
    return timer;
    }

    
// -----------------------------------------------------------------------------
// CLbtStrategyTimer::~CLbtStrategyTimer
// 
// -----------------------------------------------------------------------------
//
CLbtStrategyTimer::~CLbtStrategyTimer()
    {
    FUNC_ENTER("CLbtStrategyTimer::~CLbtStrategyTimer");
    iTimer.Close();
    if( iUtilizeMovementDetection )
    	{
    	if( iMovtDetector )
			{
			iMovtDetector->StopNotification();
			}
    	}    	
    delete iMovtDetector;
    }

    
// -----------------------------------------------------------------------------
// CLbtStrategyTimer::Set
// 
// -----------------------------------------------------------------------------
//    
void CLbtStrategyTimer::Set( TRequestStatus& aStatus, 
                             TTimeIntervalSeconds aSleepInterval,
                             TBool aUtilizeMovementDetection )
    {
    FUNC_ENTER("CLbtStrategyTimer::Set");
    LOG1("Sleeping for interval : %d", aSleepInterval.Int() );
    // Initialize sleeptime with respect to current time. 
    iSleepTime = aSleepInterval;
    
    iUtilizeMovementDetection = aUtilizeMovementDetection;
    
    // Update start time of sleep
    iStartTime.UniversalTime();
    iInterrupt = EFalse;
    
    aStatus = KRequestPending;    
    
    iMovtDetectorNotified = EFalse;
    //Initiate Movt Detector
    if( iMovtDetector && iUtilizeMovementDetection )
        {
        TRAPD( err, iMovtDetector->RequestNotificationL( this , iSleepTime.Int() ) );
        if( KErrNone == err )
            {
            iStatusPtr = &aStatus;
            return;
            }
        else
            {
            delete iMovtDetector;
            iMovtDetector = NULL;
            }
        }
   	iTimerInitialized = ETrue;
    iTimer.AtUTC( aStatus, ( iStartTime + iSleepTime ) );
    }

    
// -----------------------------------------------------------------------------
// CLbtStrategyTimer::ResetInterval
// 
// -----------------------------------------------------------------------------
//  
void CLbtStrategyTimer::ResetInterval( TRequestStatus& aStatus,
                                       TTimeIntervalSeconds aSleepInterval,
                                       TBool aUtilizeMovementDetection )
	{
	FUNC_ENTER("CLbtStrategyTimer::ResetInterval");
	LOG1("ResetInterval;=%d", aSleepInterval.Int());
	if( !iMovtDetectorNotified )
		{
		Set( aStatus, aSleepInterval,aUtilizeMovementDetection );
		return;
		}
	iUtilizeMovementDetection = aUtilizeMovementDetection;
		
	TTime sleepTime;
    sleepTime.UniversalTime();
    sleepTime += aSleepInterval;
    
    iTimer.AtUTC( aStatus, sleepTime );
	iSleepTime = aSleepInterval;
	iStartTime.UniversalTime();
    iInterrupt = EFalse;    
	}

    
// -----------------------------------------------------------------------------
// CLbtStrategyTimer::Cancel
// 
// -----------------------------------------------------------------------------
//
void CLbtStrategyTimer::Cancel()
    {
    FUNC_ENTER("CLbtStrategyTimer::Cancel");
    if( iTimerInitialized )
        {
        iTimer.Cancel();
        }
    if( iMovtDetector && iUtilizeMovementDetection )
        {
        iMovtDetector->StopNotification();
        }
    if(iStatusPtr != NULL)
    	{
    	User::RequestComplete(iStatusPtr, KErrCancel);
    	}
    //  iMovtDetectorNotified = EFalse;
    iInterrupt = ETrue;
    }
    
  
// -----------------------------------------------------------------------------
// CLbtStrategyTimer::RemainingSleepInterval
// 
// -----------------------------------------------------------------------------
//    
TBool CLbtStrategyTimer::RemainingSleepInterval( 
                                    TTimeIntervalSeconds& aRemainingInterval )
    {
    return RemainingSleepInterval( iSleepTime, aRemainingInterval );
    }


// -----------------------------------------------------------------------------
// CLbtStrategyTimer::RemainingSleepInterval
// 
// -----------------------------------------------------------------------------
//    
TBool CLbtStrategyTimer::RemainingSleepInterval( 
                                TTimeIntervalSeconds aEstimatedTime, 
                                TTimeIntervalSeconds& aRemainingInterval )
    {
    TTime currentTime;
	TTimeIntervalSeconds interval;
	currentTime.UniversalTime();
	currentTime.SecondsFrom( iStartTime, interval );
    
    aRemainingInterval = aEstimatedTime.Int() - interval.Int(); 
             
	if( aRemainingInterval.Int() <= 0 )
        {
        return EFalse;
        }
	else
        {
        return ETrue;
        }
    }
 
 
// -----------------------------------------------------------------------------
// CLbtStrategyTimer::SleepIntervalInterrupted
// 
// -----------------------------------------------------------------------------
//    
TBool CLbtStrategyTimer::SleepIntervalInterrupted()
    {
    TBool interrupted = iInterrupt;
    iInterrupt = EFalse;
    return interrupted;
    }


// -----------------------------------------------------------------------------
// CLbtStrategyTimer::ElapsedTime
// 
// -----------------------------------------------------------------------------
//    
CLbtStrategyTimer::CLbtStrategyTimer() : iTimerInitialized( EFalse ), 
										 iMovtDetectorNotified( EFalse ),
										 iMovtDetector( NULL )
    {
    }


// -----------------------------------------------------------------------------
// CLbtStrategyTimer::ElapsedTime
// 
// -----------------------------------------------------------------------------
//    
void CLbtStrategyTimer::ConstructL()
    {
    iInterrupt = EFalse;
    User::LeaveIfError( iTimer.CreateLocal() );
    iMovtDetector = CLbtMovementDetector::NewL();
    iUtilizeMovementDetection = ETrue;
    iStatusPtr = NULL;
    }


// -----------------------------------------------------------------------------
// CLbtStrategyEngine::HandleDetectorNotification
// Handles Movt detection notification
// -----------------------------------------------------------------------------
//
void CLbtStrategyTimer::HandleDetectorNotification( const TMovementObserverEvent /*aEvent*/ )
	{
	FUNC_ENTER("CLbtStrategyTimer::HandleDetectorNotification");
	LOG("Movement detected");
	 // aEvent need not be checked as the handling will remain the same  
	iMovtDetector->StopNotification();
	iMovtDetectorNotified = ETrue;
	
	TTime currentTime;
  	currentTime.UniversalTime();
    
  	if( currentTime > ( iStartTime + iSleepTime ) )
   		{
   		//Notification  after the completion of sleepinterval => Notify the engine immediately
   		User::RequestComplete( iStatusPtr, KErrNone ); 
   		iStatusPtr = NULL;
   		}
  	else
   		{
   		//Notification before the completion of sleepinterval => Notify the engine after the completion 
 		//of sleep interval
		iTimerInitialized = ETrue;
		iTimer.AtUTC( *iStatusPtr, ( iStartTime + iSleepTime ) );
		iStatusPtr = NULL;
   		}
   	}
