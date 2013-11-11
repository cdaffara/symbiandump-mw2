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
* Description:  Class definition for Time Manager 
*
*/



#ifndef C_LBTSTRATEGYTIMER_H
#define C_LBTSTRATEGYTIMER_H


#include <e32base.h>

#include "lbtmovementdetector.h" //Movt Detection


/**
 * This is a helper class that is utilized by strategy engine to initiate 
 * requests for notification once a designated interval of time has elapsed.
 * This class also provides the interface to determine elapsed time 
 * (in microseconds). 
 */
NONSHARABLE_CLASS( CLbtStrategyTimer ) : public CBase,
										 public MLbtMovementObserver
    
    {
    
public:

    /**
     * Factory function that instantiates an object of CLbtStrategyTimeManager
     */
    static CLbtStrategyTimer* NewL();
    
    /**
     * Destructor
     */
    ~CLbtStrategyTimer();
    
public:

    /**
     * Initiates a request for notification after the specified time interval
     * has elapsed
     */
     void Set( TRequestStatus& aStatus, TTimeIntervalSeconds aSleepInterval,
                                       TBool aUtilizeMovementDetection );
     
     /** Movt Detection
     * Initiates a request for notification after the specified time interval
     * has elapsed
     */
     void ResetInterval( TRequestStatus& aStatus, TTimeIntervalSeconds aSleepInterval,
                                       TBool aUtilizeMovementDetection);
     
     /**
      * Cancels an outstanding request for notifications
      */
     void Cancel();
     
     /**
      * Determines the remaining sleep interval. This is calculated as 
      * estimated sleep time - (time when Set method was invoked - time when 
      * this method is invoked)
      */
     TBool RemainingSleepInterval( TTimeIntervalSeconds& aRemainingInterval );
      
     /**
      * Determines the remaining sleep interval. This is calculated as 
      * estimated sleep time - (time when Set method was invoked - time when 
      * this method is invoked)
      */
     TBool RemainingSleepInterval( TTimeIntervalSeconds aEstimatedTime, 
                               TTimeIntervalSeconds& aRemainingInterval );
                               
     /**
      * Determines if the sleep interval was interrupted during normal 
      * operations of strategy engine
      */
     TBool SleepIntervalInterrupted();
     
     /**
      *
      */
     void UtilizeMovementDetection( TBool aUsage = EFalse );
     
private:

    /**
     * Default constructor
     */
    CLbtStrategyTimer();
    
    /**
     * 2nd phase constuctor for instantiating member variables
     */
    void ConstructL();
    
     /** 
     * Handles Movement detection notification 
     */
    void HandleDetectorNotification(const TMovementObserverEvent aEvent);
    
private:

    /**
     * Timer instance used to generate notifications when a specified time
     * interval elapses
     */
    RTimer                      iTimer;
    
    /**
     * Sleep interval as specified by strategy engine
     */
    TTimeIntervalSeconds        iSleepTime;
    
    /**
     * Keeps track of the time when a request for timer notification is issued
     */
    TTime                       iStartTime;
    
    /**
     * Boolean flag to keep track of timer interrupts
     */
    TBool                       iInterrupt;
    
    //+Movt Detection
    /**
     * Will be initialized only if one of the following is true
     * 1) Movt detector is offline
     * 2) Movt detector has already notified the observer
     */
    TBool 				        iTimerInitialized;
    
    /*
     * Flag to indicate that movement has been detected
     */
    TBool 				        iMovtDetectorNotified;
    
    /*
     * Handle to the movement detector plugin
     */
    CLbtMovementDetector*       iMovtDetector;
    
    /*
     * Handle to the client TRequestStatus object
     */
    TRequestStatus*             iStatusPtr;
    
    /**
     *
     */
     TBool                      iUtilizeMovementDetection;
	//-Movt Detection    
    };

#endif  // C_LBTSTRATEGYTIMER_H
