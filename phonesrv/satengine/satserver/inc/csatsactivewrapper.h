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


#ifndef CSATSACTIVEWRAPPER_H
#define CSATSACTIVEWRAPPER_H

#include <e32base.h>
#include "msatasynctosync.h"

/**
 *  Active object wrapper for SAT Server
 *
 *  Provides asynchronous function calls to be synchronized functions. 
 *  Provides also timer functionality.
 *
 *  @lib satengine.lib
 *  @since S60 v3.1
 */
class CSatSActiveWrapper : public CActive, public MSatAsyncToSync
    {

public:

    /**
    * Default C++ constructor
    */
    CSatSActiveWrapper();

    /**
    * Destructor
    */
    virtual ~CSatSActiveWrapper();


    /**
     * Gives reference to this active object wrapper's request status. Use this 
     * when calling asynchronous function and you want to use this wrapper.
     *
     * @return  Request status to be sent to asynchronous function call
     */
    TRequestStatus& RequestStatus();

    /**
     * Sets this wrapper active and starts waiting for request to complete.
     * Function returns after request status given to asynchronous funtion with 
     * RequestStatus is completed or wrapper is cancelled.
     *
     * @return System-wide error code indicating the completion of request
     */
    TInt SetActiveAndWait();

    /**
     * Starts timer. Function returns as the given time has elapsed or 
     * timer is cancelled.
     *
     * @param aDelay Delay from the function call to complete function in microseconds
     * @param aInterval Interval to complete wait after aDelay, zero by default
     */
    void After( const TTimeIntervalMicroSeconds32& aDelay,
                const TTimeIntervalMicroSeconds32& aInterval = 0 );

     /**
     * Cancels request status or timer depending which is active
     */
    void CancelWrapper();

     /**
     * Release object
     */
    void Release();

    /**
     * From CActive.
     * Called when request status given SetActiveAndWait completes
     */
    void RunL();

    /**
     * From CActive.
     * Called when request is cancelled
     */
    void DoCancel();

private:

    /**
     * Called after given delay in After function. 
     * Causes the After function to complete
     *
     * @param aPtr Pointer to itself since this is static function
     * @return whether to call function again after an interval
     */
    static TInt DelayCallBack( TAny* aPtr );

private: // data

    /**
     * Blocker for After
     */
    CActiveSchedulerWait iAfterWait;

    /**
     * Blocker for SetActiveAndWait
     */
    CActiveSchedulerWait iWait;

    /**
     * Timer used in After
     */
    CPeriodic* iTimer;
    
    };

#endif // CSATSACTIVEWRAPPER_H
