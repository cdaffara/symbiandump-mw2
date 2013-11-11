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
 * Description:  Central Repository listening
 *
 */

// INCLUDE FILES
#include "tmscenreplistener.h"
#include "tmscenrepobserver.h"
#include "tmsutility.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// TMSCenRepListener::TMSCenRepListener
// C++ default constructor
// -----------------------------------------------------------------------------
//
TMSCenRepListener* TMSCenRepListener::NewL(TUid aUid, TUint32 aMonitorSetting,
        TMSCenRepObserver* aObserver)
    {
    TRACE_PRN_FN_ENT;
    TMSCenRepListener* self = new (ELeave) TMSCenRepListener(aUid,
            aMonitorSetting, aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    TRACE_PRN_FN_EXT;
    return self;
    }

// Destructor
TMSCenRepListener::~TMSCenRepListener()
    {
    TRACE_PRN_FN_ENT;
    Cancel();
    delete iRepository;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCenRepListener::Get
// -----------------------------------------------------------------------------
//
gint TMSCenRepListener::Get(gint& aValue)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    if (iRepository)
        {
        status = iRepository->Get(iMonitorSetting, aValue);
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCenRepListener::Set
// -----------------------------------------------------------------------------
//
gint TMSCenRepListener::Set(gint aValue)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    if (iRepository)
        {
        status = iRepository->Set(iMonitorSetting, aValue);
        }
    TRACE_PRN_FN_EXT;
    return status;

    }
// -----------------------------------------------------------------------------
// TMSCenRepListener::DoCancel
// -----------------------------------------------------------------------------
//
void TMSCenRepListener::DoCancel()
    {
    TRACE_PRN_FN_ENT;
    if (iRepository)
        {
        iRepository->NotifyCancel(iMonitorSetting);
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCenRepListener::RunError
// -----------------------------------------------------------------------------
//
gint TMSCenRepListener::RunError(
#ifdef _DEBUG
        TInt aError // Log the leavecode from RunL
#else
        TInt /*aError*/
#endif
        )
    {
    TRACE_PRN_FN_ENT;
#ifdef _DEBUG
    TRACE_PRN_IF_ERR(aError);
#endif
    TRACE_PRN_FN_EXT;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// TMSCenRepListener::RunL
// -----------------------------------------------------------------------------
//
void TMSCenRepListener::RunL()
    {
    TRACE_PRN_FN_ENT;
    // Don't resubmit the request on error
    // Central repositry completes notifications with id of setting
    // so check only that value of iStatus is negative
    if (iStatus.Int() == KErrNone)
        {
        SubmitNotifyRequestL();
        }

    // The loudspeaker volume has changed in repository.
    // Retrieve the current volume from repository.
    gint volume;
    Get(volume);
    if (iObserver)
        {
        iObserver->HandleNotifyCenRepL(iUid, iMonitorSetting, volume);
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCenRepListener::TMSCenRepListener
// -----------------------------------------------------------------------------
//
TMSCenRepListener::TMSCenRepListener(TUid aUid, TUint32 aMonitorSetting,
        TMSCenRepObserver* aObserver) :
    CActive(EPriorityStandard),
    iUid(aUid),
    iMonitorSetting(aMonitorSetting),
    iObserver(aObserver)
    {
    TRACE_PRN_FN_ENT;
    CActiveScheduler::Add(this);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCenRepListener::ConstructL
// -----------------------------------------------------------------------------
//
void TMSCenRepListener::ConstructL()
    {
    TRACE_PRN_FN_ENT;
    // Create repository instance
    iRepository = CRepository::NewL(iUid);
    // Start monitoring
    SubmitNotifyRequestL();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCenRepListener::SubmitNotifyRequestL
// -----------------------------------------------------------------------------
//
void TMSCenRepListener::SubmitNotifyRequestL()
    {
    TRACE_PRN_FN_ENT;
    if (iRepository)
        {
        iStatus = KRequestPending;
        iRepository->NotifyRequest(iMonitorSetting, iStatus);
        SetActive();
        }
    TRACE_PRN_FN_EXT;
    }

// End of file
