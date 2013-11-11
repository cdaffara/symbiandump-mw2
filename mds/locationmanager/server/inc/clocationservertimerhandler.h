/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  helper class to handle location server tiimer related functionality
*
*/

#ifndef __CLOCATIONSERVERTIMERHANDLER_H__
#define __CLOCATIONSERVERTIMERHANDLER_H__


// INCLUDES
#include <e32std.h>
#include <e32base.h>


/*
*  Timer Observer class
*/

class MLocationServerTimerObserver
    {
public:
    typedef enum
        {
        EStopRecording = 0x1, // Stop recording
        EPositioningStopTimeout, // Stop positioning
        ELocGeneralPurpose  // generic purpose
        }TLocationServerTimerType;
    /**
        * Timer call back
        * @param aLocationServerTimerType timer type
        * @param aErrorCode error code
        */
     virtual void LocationServerTimerCallBackL
        (const TLocationServerTimerType aLocationServerTimerType, const TInt aErrorCode) = 0;
    };

/*
*  Helper class  for location server related to timer functionality.
*/
class CLocationServerTimerHandler : public CTimer
    {
    
public:
     /**
        * 1st phase constructor.
        * @param aLocationServerTimerObserver timer observer
        */
     static CLocationServerTimerHandler* NewL(MLocationServerTimerObserver& aLocationServerTimerObserver); 

     /**
        * Destructor
        */
     ~CLocationServerTimerHandler();

     /**
        * start a timer for predefined period
        * @param anInterval timer value
        * @param aLocationServerTimerType timer type
        */
     void StartTimer(const TTimeIntervalMicroSeconds32 anInterval,
        const MLocationServerTimerObserver::TLocationServerTimerType aLocationServerTimerType);

     
protected:
    /*
        * @see CActive::RunL()
        */
    void RunL();
private:
    /**
    * 2nd Phase constructor
    */
    void ConstructL();
		/**
	   * CLocationServerTimerHandler
	   * Constructor
	   * @param aLocationServerTimerObserver timer observer
	   * @return None.
	   */
     CLocationServerTimerHandler(MLocationServerTimerObserver& aLocationServerTimerObserver);
      


private:      
      MLocationServerTimerObserver& iLocationServerTimerObserver;
      MLocationServerTimerObserver::TLocationServerTimerType iLocationServerTimerType;
    };


#endif /*__CLOCATIONSERVERTIMERHANDLER_H__*/

// End of file

