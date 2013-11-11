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

// INCLUDE FILES
#include "tmsdelaytimer.h"
#include "tmsutility.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// TMSCallDelayTimer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
TMSCallDelayTimer* TMSCallDelayTimer::NewL()
    {
    TRACE_PRN_FN_ENT;
    TMSCallDelayTimer* self = new (ELeave) TMSCallDelayTimer;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    TRACE_PRN_FN_EXT;
    return self;
    }

// -----------------------------------------------------------------------------
// TMSCallDelayTimer::TMSCallDelayTimer()
// C++ constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TMSCallDelayTimer::TMSCallDelayTimer() :
    CActive(CActive::EPriorityStandard)
    {
    TRACE_PRN_FN_ENT;
    CActiveScheduler::Add(this);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallDelayTimer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void TMSCallDelayTimer::ConstructL()
    {
    TRACE_PRN_FN_ENT;
    User::LeaveIfError(iShutDownTimer.CreateLocal());
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallDelayTimer::~TMSCallDelayTimer
// Destructor
// -----------------------------------------------------------------------------
//
TMSCallDelayTimer::~TMSCallDelayTimer()
    {
    TRACE_PRN_FN_ENT;
    Cancel();
    iShutDownTimer.Close();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallDelayTimer::SetDelay
// Request a timeout after aDelay
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TMSCallDelayTimer::SetDelay(TTimeIntervalMicroSeconds32 aDelay)
    {
    TRACE_PRN_FN_ENT;
    if (!IsActive())
        {
        iStatus = KRequestPending;
        SetActive();
        iShutDownTimer.After(iStatus, aDelay);
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallDelayTimer::RunL
// Called by Active object framework when timer times out.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TMSCallDelayTimer::RunL()
    {
    TRACE_PRN_FN_ENT;
    CActiveScheduler::Stop();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallDelayTimer::DoCancel
// Called by the Active object framework when user cancels active object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TMSCallDelayTimer::DoCancel()
    {
    TRACE_PRN_FN_ENT;
    iShutDownTimer.Cancel();
    TRACE_PRN_FN_EXT;
    }

//  End of File
