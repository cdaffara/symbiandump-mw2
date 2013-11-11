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

#include <etelmm.h>
#include <rmmcustomapi.h>
#include "tmsdtmfprovider.h"
#include "tmsdtmfobserver.h"
#include "tmsutility.h"
#include "tmseteldtmfmonitor.h"
#include "tmseteldtmfstopmonitor.h"

using namespace TMS;

TMSDTMFProvider* TMSDTMFProvider::NewL()
    {
    TRACE_PRN_FN_ENT;
    TMSDTMFProvider* self = new (ELeave) TMSDTMFProvider();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    TRACE_PRN_FN_EXT;
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor of the object.
// -----------------------------------------------------------------------------
//
TMSDTMFProvider::~TMSDTMFProvider()
    {
    TRACE_PRN_FN_ENT;
    Cancel();
    iObservers.Close();
    delete iMonitor;
    delete iStopMonitor;

    iMmCustom.Close();
    iPhone.Close();
    iServer.UnloadPhoneModule(iTsyname);
    iServer.Close();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// Constructs the requester.
// -----------------------------------------------------------------------------
//
TMSDTMFProvider::TMSDTMFProvider() :
    CActive(EPriorityStandard)
    {
    TRACE_PRN_FN_ENT;
    CActiveScheduler::Add(this);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// Constructs the requester in the second phase.
// -----------------------------------------------------------------------------
//
void TMSDTMFProvider::ConstructL()
    {
    TRACE_PRN_FN_ENT;

    User::LeaveIfError(iServer.Connect());

    TRACE_PRN_N(_L("**TMS ETel iServer.Connect"));

    User::LeaveIfError(iServer.GetTsyName(0, iTsyname));

    //Load in the phone device driver
    User::LeaveIfError(iServer.LoadPhoneModule(iTsyname));

    TRACE_PRN_N(_L("**TMS ETel iServer.LoadPhoneModule"));

    //Find the number of phones available from the tel server
    gint numberPhones;
    User::LeaveIfError(iServer.EnumeratePhones(numberPhones));

    TRACE_PRN_N(_L("**TMS ETel iServer.EnumeratePhones"));

    //Check there are available phones
    if (numberPhones < 1)
        {
        User::Leave(KErrNotFound);
        }

    //Get info about the first available phone
    RTelServer::TPhoneInfo info;
    User::LeaveIfError(iServer.GetPhoneInfo(0, info));

    TRACE_PRN_N(_L("**TMS ETel iServer.GetPhoneInfo"));

    CleanupClosePushL(iPhone);
    User::LeaveIfError(iPhone.Open(iServer, info.iName));

    TRACE_PRN_N(_L("**TMS ETel iPhone.Open"));

    RMobilePhone::TLineInfo lineInfo;
    User::LeaveIfError(iPhone.GetLineInfo(0, lineInfo));

    TRACE_PRN_N(_L("**TMS ETel iPhone.GetLineInfo"));

    gint errorCode = iMmCustom.Open(iPhone);

    if (!iPhone.SubSessionHandle() || !iMmCustom.SubSessionHandle())
        {
        User::Leave(KErrArgument);
        }
    iMonitor = TMSEtelDtmfMonitor::NewL(*this, iMmCustom);
    iMonitor->StartMonitoring();
    iStopMonitor = TMSEtelDtmfStopMonitor::NewL(*this, iPhone);
    // iStopMonitor->StartMonitoring();

    CleanupStack::Pop(1);//phone
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// Notifies observers about a DTMF event
// -----------------------------------------------------------------------------
//
void TMSDTMFProvider::NotifyDTMFEvent(
        const TMSDTMFObserver::TCCPDtmfEvent aEvent, const gint aError,
        const TChar aTone)
    {
    TRACE_PRN_FN_ENT;
    for (gint i = 0; i < iObservers.Count(); i++)
        {
        TMSDTMFObserver* obs = iObservers[i];
        if (obs)
            {
            iObservers[i]->HandleDTMFEvent(aEvent, aError, aTone);
            }
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// Cancel DTMF string sending
// -----------------------------------------------------------------------------
//
gint TMSDTMFProvider::CancelDtmfStringSending()
    {
    TRACE_PRN_FN_ENT;
    gint ret(KErrAlreadyExists);
    if (IsActive())
        {
        Cancel();
        ret = KErrNone;
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

// -----------------------------------------------------------------------------
// Starts DTMF string sending
// -----------------------------------------------------------------------------
//
gint TMSDTMFProvider::StartDtmfTone(const TChar aTone)
    {
    TRACE_PRN_FN_ENT;
    gint ret(KErrAlreadyExists);
    if (!IsActive())
        {
        ret = iPhone.StartDTMFTone(aTone);
        }
    TRACE_PRN_IF_ERR(ret);
    TRACE_PRN_FN_EXT;
    return ret;
    }

// -----------------------------------------------------------------------------
// Stop DTMF tone
// -----------------------------------------------------------------------------
//
gint TMSDTMFProvider::StopDtmfTone()
    {
    TRACE_PRN_FN_ENT;
    gint ret(KErrAlreadyExists);
    if (!IsActive())
        {
        ret = iPhone.StopDTMFTone();
        }
    TRACE_PRN_IF_ERR(ret);
    TRACE_PRN_FN_EXT;
    return ret;
    }

// -----------------------------------------------------------------------------
// Send DTMF string
// -----------------------------------------------------------------------------
//
gint TMSDTMFProvider::SendDtmfToneString(const TDesC& aString)
    {
    TRACE_PRN_FN_ENT;
    gint ret = KErrInUse;
    if (!IsActive())
        {
        iStatus = KRequestPending;
        iPhone.SendDTMFTones(iStatus, aString);
        SetActive();
        ret = KErrNone;
        }
    TRACE_PRN_IF_ERR(ret);
    TRACE_PRN_FN_EXT;
    return ret;
    }

// -----------------------------------------------------------------------------
// Continue DTMF string sending
// -----------------------------------------------------------------------------
//
gint TMSDTMFProvider::ContinueDtmfStringSending(const gboolean aContinue)
    {
    TRACE_PRN_FN_ENT;
    gint status = iPhone.ContinueDTMFStringSending(aContinue);
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// Adds observer.
// -----------------------------------------------------------------------------
//
void TMSDTMFProvider::AddObserver(const TMSDTMFObserver& aObserver)
    {
    TRACE_PRN_FN_ENT;
    if (iObservers.Find(&aObserver) == KErrNotFound)
        {
        iObservers.Append(&aObserver);
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// Removes given observer.
// -----------------------------------------------------------------------------
//
gint TMSDTMFProvider::RemoveObserver(const TMSDTMFObserver& aObserver)
    {
    TRACE_PRN_FN_ENT;
    gint ret = KErrNotFound;
    gint found = iObservers.Find(&aObserver);
    if (found != KErrNotFound)
        {
        iObservers.Remove(found);
        ret = KErrNone;
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

// -----------------------------------------------------------------------------
// From CActive.
// Handles request completion.
// -----------------------------------------------------------------------------
//
void TMSDTMFProvider::RunL()
    {
    TRACE_PRN_FN_ENT;
    if (iStatus == KErrNone)
        {
        TMSDTMFObserver::TCCPDtmfEvent event =
                TMSDTMFObserver::ECCPDtmfStringSendingCompleted;
        NotifyDTMFEvent(event, KErrNone, NULL);
        }
    else
        {
        gint error = iStatus.Int();
        if (error != KErrCancel)
            {
            TMSDTMFObserver::TCCPDtmfEvent event =
                    TMSDTMFObserver::ECCPDtmfStringSendingCompleted;
            NotifyDTMFEvent(event, error, NULL);
            TRACE_PRN_IF_ERR(error);
            }
        // Cancel is not notified
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// From CActive
// Canceling functionality.
// -----------------------------------------------------------------------------
//
void TMSDTMFProvider::DoCancel()
    {
    TRACE_PRN_FN_ENT;
    iPhone.CancelAsyncRequest(EMobilePhoneSendDTMFTones);
    TRACE_PRN_FN_EXT;
    }

