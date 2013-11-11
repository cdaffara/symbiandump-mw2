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
 * Description: Telephony Multimedia Service
 *
 */

#ifndef TMSETELDTMFSTOPMONITOR_H
#define TMSETELDTMFSTOPMONITOR_H

#include <e32base.h>
#include <etelmm.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSDTMFProvider;

/**
 * TMSEtelDtmfStopMonitor class
 *
 * Monitors single ETel RMobilePhone for DTMF events.
 *
 * This monitor is applied only for listening incoming calls, i.e.
 * line capabilites are not monitored.
 *
 */
class TMSEtelDtmfStopMonitor : public CActive
    {
public:
    /**
     * Two-phased constructing for the monitor.
     *
     * @param aObserver the observer for getting notification
     * @param aPhone the line to monitor
     */
    static TMSEtelDtmfStopMonitor* NewL(TMSDTMFProvider& aObserver,
            RMobilePhone& aPhone);

    /**
     * C++ default destructor
     */
    virtual ~TMSEtelDtmfStopMonitor();

    /**
     * Start monitoring call events
     */
    void StartMonitoring();

protected: // From CActive
    /**
     * From CActive
     * RunL
     */
    void RunL();

    /**
     * From CActive
     * Cancels the monitor
     */
    void DoCancel();

private:
    /**
     * C++ default constructor
     * @param aObserver the observer for status change (incoming call)
     * @param aPhone the line associated with the call
     */
    TMSEtelDtmfStopMonitor(TMSDTMFProvider& aObserver, RMobilePhone& aPhone);

private:
    /**
     * Forwards events.
     */
    TMSDTMFProvider& iObserver;

    /**
     * Phone that is being observed for incoming call.
     */
    RMobilePhone& iPhone;

    /**
     * Event received.
     */
    RMobilePhone::TMobilePhoneDTMFEvent iEventData;

    };

} //namespace TMS

#endif // TMSETELDTMFSTOPMONITOR_H

