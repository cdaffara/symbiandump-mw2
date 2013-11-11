/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:
 *    Utility class for CTimer object
 *
 */

#ifndef TMSTIMER_H
#define TMSTIMER_H

// INCLUDES
#include <e32base.h>
#include <glib.h>

namespace TMS {

/**
 *  TMSTimerObserver observer.
 */
class TMSTimerObserver
    {
public:

    /**
     * Called upon timer timeout.
     */
    virtual void TimerEvent() = 0;
    };

// CLASS DECLARATION
/**
 *  TMSTimer - Timer service.
 */
class TMSTimer : protected CTimer
    {
public:

    /**
     * Two-phased constructor.
     */
    IMPORT_C static TMSTimer* NewL(gint aPriority = CActive::EPriorityStandard);

    /**
     * Destructor.
     */
    IMPORT_C virtual ~TMSTimer();

    /**
     * Call callback method upon timer timeout event.
     * All former request will be canceled first
     */
    IMPORT_C void NotifyAfter(gint timeout, TCallBack aCallBack);

    /**
     * Call client observer upon timer timeout event.
     * All former request will be canceled first
     */
    IMPORT_C void NotifyAfter(gint timeout, TMSTimerObserver& observer);

    /**
     * Cancel the timer
     */
    IMPORT_C void CancelNotify();

    /**
     * Determines if the timer is running.
     */
    IMPORT_C gboolean IsRunning();

protected:

    /**
     * From CTimer::RunL()
     */
    virtual void RunL();

    /**
     * From CTimer::RunL()
     */
    virtual TInt RunError(TInt aError);

    /**
     * From CTimer::DoCancel()
     */
    virtual void DoCancel();

private:

    /**
     * C++ default constructor.
     */
    TMSTimer(gint aPriority);

    /**
     * Symbian OS constructor.
     */
    void ConstructL();

    // By default, prohibit copy constructor
    TMSTimer(const TMSTimer&);

    // Prohibit assigment operator
    TMSTimer& operator = (const TMSTimer&);

private:

    // Optional callback instead of observer
    TCallBack iCallBack;

    // Observer for notify service.
    TMSTimerObserver* iObserver;
    };

} //namespace TMS

#endif //TMSTIMER_H

