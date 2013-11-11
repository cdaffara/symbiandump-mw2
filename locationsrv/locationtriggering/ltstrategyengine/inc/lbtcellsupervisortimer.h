/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This header file describes the triggering timer
*
*/

#ifndef C_LBTCELLSUPERVISORTIMER_H
#define C_LBTCELLSUPERVISORTIMER_H


// INCLUDES
#include <e32base.h>


/**
 * Observer interface class for CLbtTimer.
 * 
 * @see CLbtTimer
 */
class MLbtCellTimerObserver
	{
	public:
    
        /**
         * Call back method for timer. This will be called when timer has
         * timed out. Deriving class must implement this.
         */
		virtual void TimeOut() = 0;
	};

/**
 * Timer class used to time out to get the current cell information
 */
class CLbtCellSupervisorTimer : public CTimer
    {
    public:
    
        /**
         * Instantiation method for this class. Requires that reference to
         * timer observer class is passed as parameter. The observer shall be
         * notified upon time out.
         * 
         * @param aObs reference to observer.
         * @return new instance of this class.
         */
        static CLbtCellSupervisorTimer* NewL(MLbtCellTimerObserver& aObserver);
        
        /**
         * Default Destructor
         */
        ~CLbtCellSupervisorTimer();

        /**
         * Sets timer and activates this object.
         * 
         * @param aSeconds time in second in which to time out.
         */ 
		void SetTimer(TInt aSeconds);
		
		/**
		 * Gets the Sleep time 
		 */
		TInt RemainingTime();
		
		// Interrupts sleep and sets timer to new value
		void InterruptSleep( TInt aNewSleepTime);
		
    protected:
    
        // methods from CActive
        virtual void RunL();
 		
    private:
        // functions
    
        /**
         * Default constructor
         * 
         * @param aObs timer observer
         */
        CLbtCellSupervisorTimer(MLbtCellTimerObserver& aObserver);

        /**
         * Default second level constructor.
         */
        void ConstructL();
        
    private:
        // data
        MLbtCellTimerObserver&	iObserver;  ///< reference to observer class.
        
        
        // Keeps track of the time when a request for timer notification is issued
        
        TTime                   iStartTime;
        
        TInt                    iSleepTime;
    };

#endif //C_LBTCELLSUPERVISORTIMER_H
