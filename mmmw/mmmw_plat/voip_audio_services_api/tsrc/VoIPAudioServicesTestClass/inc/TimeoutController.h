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
 * Description: Voip Audio Service - Stif test
 *
 */

#ifndef TIMEOUTCONTROLLER_H
#define TIMEOUTCONTROLLER_H

#include <e32svr.h>
#include <StifParser.h>
#include <StifLogger.h>
#include <Stiftestinterface.h>

/* This class is used to notify the observer
 *  about the timeout
 */
class MTimeoutObserver
    {
public:
    /**
     * Timeout
     */
    virtual void HandleTimeout(TInt error) = 0;
    };

_LIT(KFormatTimeStamp, "%:0%J%:1%T%:2%S%.%*C4%:3%+B");

/* This class is used for extra time controlling
 *  The STIF timeout isn't enough
 */
class CSimpleTimeout : public CActive
    {
public:

    /**
     * Two-phased constructor.
     */
    static CSimpleTimeout* NewL(MTimeoutObserver* aObserver,
            CStifLogger* aLogger);

    /**
     * Destructor.
     */
    ~CSimpleTimeout();

    /**
     * Starts timeout counting
     */
    void Start(TTimeIntervalMicroSeconds aTimeout);

    /**
     * Cancel timeout counting
     */
    void Stop();

    /**
     * RunL derived from CActive handles the completed requests.
     */
    void RunL();

    /**
     * DoCancel derived from CActive handles the cancel
     */
    void DoCancel();

    /**
     * RunError derived from CActive handles errors from active handler.
     */
    TInt RunError(TInt aError);

private:

    /**
     * By default Symbian OS constructor is private.
     */
    void ConstructL(MTimeoutObserver* aObserver, CStifLogger* aLogger);

    /**
     * C++ constructor.
     */
    CSimpleTimeout();

private:
    // Data
    MTimeoutObserver* iObserver;
    RTimer iTimer;

    // Storage for testcase timeout.
    TTime iTestCaseTimeout;
    CStifLogger* iLog;
    };

#endif //TIMEOUTCONTROLLER_H

