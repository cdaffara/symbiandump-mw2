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

#include <gsmerror.h>
#include "tmsutility.h"
#include "tmsdtmfobserver.h"
#include "tmsdtmfprovider.h"
#include "tmseteldtmfstopmonitor.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// TMSEtelDtmfStopMonitor::NewL.
// -----------------------------------------------------------------------------
//
TMSEtelDtmfStopMonitor* TMSEtelDtmfStopMonitor::NewL(TMSDTMFProvider& aObserver,
        RMobilePhone& aPhone)
    {
    TRACE_PRN_FN_EXT;
    TMSEtelDtmfStopMonitor* self = new (ELeave) TMSEtelDtmfStopMonitor(
            aObserver, aPhone);
    TRACE_PRN_FN_EXT;
    return self;
    }

// -----------------------------------------------------------------------------
// Constructs the monitor.
// -----------------------------------------------------------------------------
//
TMSEtelDtmfStopMonitor::TMSEtelDtmfStopMonitor(TMSDTMFProvider& aObserver,
        RMobilePhone& aPhone) :
    CActive(EPriorityStandard),
    iObserver(aObserver),
    iPhone(aPhone)
    {
    TRACE_PRN_FN_ENT;
    CActiveScheduler::Add(this);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// Destructs the object by canceling first ongoing monitoring.
// -----------------------------------------------------------------------------
//
TMSEtelDtmfStopMonitor::~TMSEtelDtmfStopMonitor()
    {
    TRACE_PRN_FN_ENT;
    Cancel();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// Starts the monitor.
// -----------------------------------------------------------------------------
//
void TMSEtelDtmfStopMonitor::StartMonitoring()
    {
    TRACE_PRN_FN_ENT;
    if (!IsActive())
        {
        iStatus = KRequestPending;
        iPhone.NotifyStopInDTMFString(iStatus);
        SetActive();
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// From CActive.
// Handles line status change notifying.
// -----------------------------------------------------------------------------
//
void TMSEtelDtmfStopMonitor::RunL()
    {
    TRACE_PRN_FN_ENT;
    gint err = iStatus.Int();
    TRACE_PRN_N1(_L("**TMS TMSEtelDtmfStopMonitor::RunL: status: %d"), err);

    if (err != KErrCancel && err != KErrServerTerminated)
        {
        TChar tone = NULL;
        TMSDTMFObserver::TCCPDtmfEvent event =
                TMSDTMFObserver::ECCPDtmfStopInDtmfString;
        iObserver.NotifyDTMFEvent(event, err, tone);
        }
    else
        {
        TRACE_PRN_N1(_L("**TMS TMSEtelDtmfStopMonitor::RunL: Error \
                       from RMobilePhone: %d"),iStatus.Int());
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
void TMSEtelDtmfStopMonitor::DoCancel()
    {
    TRACE_PRN_FN_ENT;
    iPhone.CancelAsyncRequest(EMobilePhoneNotifyStopInDTMFString);
    TRACE_PRN_FN_EXT;
    }

