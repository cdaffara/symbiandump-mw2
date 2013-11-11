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
 * Description: Telephony Multimedia Service
 *
 */

// INCLUDE FILES
#include "tmstimer.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
//
TMSTimer::TMSTimer(gint aPriority) :
    CTimer(aPriority)
    {
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// TMSTimer::ConstructL
// -----------------------------------------------------------------------------
//
void TMSTimer::ConstructL()
    {
    CTimer::ConstructL();
    }

// -----------------------------------------------------------------------------
// TMSTimer::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C TMSTimer* TMSTimer::NewL(gint aPriority)
    {
    TMSTimer* self = new (ELeave) TMSTimer(aPriority);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// TMSTimer::~TMSTimer
// -----------------------------------------------------------------------------
//
EXPORT_C TMSTimer::~TMSTimer()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// TMSTimer::NotifyAfter
// -----------------------------------------------------------------------------
//
EXPORT_C void TMSTimer::NotifyAfter(gint timeout, TCallBack callback)
    {
    if (IsActive())
        {
        Cancel();
        }
    iObserver = NULL;
    iCallBack = callback;
    CTimer::After(TTimeIntervalMicroSeconds32(timeout));
    }

// -----------------------------------------------------------------------------
// TMSTimer::NotifyAfter
// -----------------------------------------------------------------------------
//
EXPORT_C void TMSTimer::NotifyAfter(gint timeout, TMSTimerObserver& observer)
    {
    if (IsActive())
        {
        Cancel();
        }
    iObserver = &observer;
    CTimer::After(TTimeIntervalMicroSeconds32(timeout));
    }

// -----------------------------------------------------------------------------
//  TMSTimer::CancelNotify
// -----------------------------------------------------------------------------
//
EXPORT_C void TMSTimer::CancelNotify()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// TMSTimer::IsRunning
// -----------------------------------------------------------------------------
//
EXPORT_C gboolean TMSTimer::IsRunning()
    {
    return IsActive();
    }

// -----------------------------------------------------------------------------
// TMSTimer::RunL()
// -----------------------------------------------------------------------------
//
void TMSTimer::RunL()
    {
    // Currently RunL doesn't leave.
    if (iObserver)
        {
        iObserver->TimerEvent();
        }
    else
        {
        iCallBack.CallBack();
        }
    }

// -----------------------------------------------------------------------------
// TMSTimer::RunError()
// -----------------------------------------------------------------------------
//
TInt TMSTimer::RunError(TInt aError)
    {
    return aError;
    }

// -----------------------------------------------------------------------------
//  TMSTimer::DoCancel
// -----------------------------------------------------------------------------
//
void TMSTimer::DoCancel()
    {
    iObserver = NULL;
    CTimer::DoCancel();
    }

//  End of File
