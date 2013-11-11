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

#include <rmmcustomapi.h>
#include <gsmerror.h>
#include "tmsdtmfprovider.h"
#include "tmsdtmfobserver.h"
#include "tmseteldtmfmonitor.h"
#include "tmsutility.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// TMSEtelDtmfMonitor::NewL.
// -----------------------------------------------------------------------------
//
TMSEtelDtmfMonitor* TMSEtelDtmfMonitor::NewL(TMSDTMFProvider& aObserver,
        RMmCustomAPI& aMmCustom)
    {
    TRACE_PRN_FN_ENT;
    TMSEtelDtmfMonitor* self = new (ELeave) TMSEtelDtmfMonitor(aObserver,
            aMmCustom);
    TRACE_PRN_FN_EXT;
    return self;
    }

// -----------------------------------------------------------------------------
// Constructs the monitor.
// -----------------------------------------------------------------------------
//
TMSEtelDtmfMonitor::TMSEtelDtmfMonitor(TMSDTMFProvider& aObserver,
        RMmCustomAPI& aMmCustom) :
    CActive(EPriorityStandard),
    iObserver(aObserver),
    iMmCustom(aMmCustom)
    {
    TRACE_PRN_FN_ENT;
    CActiveScheduler::Add(this);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// Destructs the object by canceling first ongoing monitoring.
// -----------------------------------------------------------------------------
//
TMSEtelDtmfMonitor::~TMSEtelDtmfMonitor()
    {
    TRACE_PRN_FN_ENT;
    Cancel();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// Starts the monitor.
// -----------------------------------------------------------------------------
//
void TMSEtelDtmfMonitor::StartMonitoring()
    {
    TRACE_PRN_FN_ENT;
    if (!IsActive())
        {
        iStatus = KRequestPending;
        iMmCustom.NotifyDtmfEvent(iStatus, iEventData);
        SetActive();
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// From CActive.
// Handles line status change notifying.
// -----------------------------------------------------------------------------
//
void TMSEtelDtmfMonitor::RunL()
    {
    TRACE_PRN_FN_ENT;
    gint err = iStatus.Int();
    TRACE_PRN_N1(_L("**TMS TMSEtelDtmfMonitor::RunL: status:%d"), err);

    if (err == KErrNone)
        {
        TMSDTMFObserver::TCCPDtmfEvent event;
        if (iEventData.iEvent == RMmCustomAPI::EDtmfStart
                && iEventData.iType == RMmCustomAPI::EDtmfManual)
            {
            event = TMSDTMFObserver::ECCPDtmfManualStart;
            }
        else if (iEventData.iEvent == RMmCustomAPI::EDtmfStart
                && iEventData.iType == RMmCustomAPI::EDtmfSequence)
            {
            event = TMSDTMFObserver::ECCPDtmfSequenceStart;
            }
        else if (iEventData.iEvent == RMmCustomAPI::EDtmfStop
                && iEventData.iType == RMmCustomAPI::EDtmfManual)
            {
            event = TMSDTMFObserver::ECCPDtmfManualStop;
            }
        else if (iEventData.iEvent == RMmCustomAPI::EDtmfStop
                && iEventData.iType == RMmCustomAPI::EDtmfSequence)
            {
            event = TMSDTMFObserver::ECCPDtmfSequenceStop;
            }
        else if (iEventData.iEvent == RMmCustomAPI::EDtmfAbort
                && iEventData.iType == RMmCustomAPI::EDtmfManual)
            {
            event = TMSDTMFObserver::ECCPDtmfManualAbort;
            }
        else if (iEventData.iEvent == RMmCustomAPI::EDtmfAbort
                && iEventData.iType == RMmCustomAPI::EDtmfSequence)
            {
            event = TMSDTMFObserver::ECCPDtmfSequenceAbort;
            }
        else
            {
            // Refresh and return (no observer notfiying).
            TRACE_PRN_N2(_L("**TMS TMSEtelDtmfMonitor::RunL: Unknown event \
                    = %¨d, type = %d"), iEventData.iEvent,iEventData.iType);
            StartMonitoring();
            return;
            }

        iObserver.NotifyDTMFEvent(event, err, iEventData.iTone);
        }
    else
        {
        TRACE_PRN_N1(_L("**TMS TMSEtelDtmfMonitor::RunL: Error \
                from DTMF: %d"),err);
        }

    // Continue if not in offline mode
    if (err != KErrGsmOfflineOpNotAllowed && err != KErrCancel
            && err != KErrNotSupported)
        {
        StartMonitoring();
        }
    TRACE_PRN_IF_ERR(err);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// From CActive
// Canceling functionality.
// -----------------------------------------------------------------------------
//
void TMSEtelDtmfMonitor::DoCancel()
    {
    TRACE_PRN_FN_ENT;
    iMmCustom.CancelAsyncRequest(ECustomNotifyDtmfEventIPC);
    TRACE_PRN_FN_EXT;
    }

