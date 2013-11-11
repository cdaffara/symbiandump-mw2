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

#ifndef TMSETELDTMFMONITOR_H
#define TMSETELDTMFMONITOR_H

#include <e32base.h>
#include <etelmm.h>
#include <rmmcustomapi.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSCSPCall;
class TMSDTMFProvider;

/**
 * TMSEtelDtmfMonitor class
 *
 * Monitors ETel for DTMF events.
 *
 * This monitor is applied only for listening incoming calls, i.e.
 * line capabilites are not monitored.
 *
 * @lib csplugin.dll
 *
 */
class TMSEtelDtmfMonitor : public CActive
    {
public:
    /**
     * Two-phased constructing for the monitor.
     *
     * @param aObserver the observer for getting notification
     * @param aMmCustom Custom API handle
     */
    static TMSEtelDtmfMonitor* NewL(TMSDTMFProvider& aObserver,
            RMmCustomAPI& aMmCustom);

    /**
     * C++ default destructor
     */
    virtual ~TMSEtelDtmfMonitor();

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
     * @param aMmCustom Custom API handle
     */
    TMSEtelDtmfMonitor(TMSDTMFProvider& aObserver, RMmCustomAPI& aMmCustom);

private:
    /**
     * Forwards events.
     */
    TMSDTMFProvider& iObserver;

    /**
     * Event data. Updated when event occurs.
     */
    RMmCustomAPI::TDtmfInfo iEventData;

    /**
     * Custom API reference.
     */
    RMmCustomAPI& iMmCustom;
    };

} //namespace TMS

#endif // TMSETELDTMFMONITOR_H

