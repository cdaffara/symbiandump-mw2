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
 * Description: Local timer to update position for recording 
 *
 */

#include "sfmrlocaltimer.h"

extern "C"
    {
#include "xarecorditfadaptationmmf.h"
    }

#include "cmmfbackendengine.h"

LocalTimer::LocalTimer(CMMFBackendEngine* parent, void* adaptContext) :
    CActive(CActive::EPriorityStandard), iTime(0)
    {
    CActiveScheduler::Add(this);
    iParent = parent;
    iAdaptContext = adaptContext;
    }

LocalTimer::~LocalTimer()
    {
    Cancel();
    iTimer.Close();
    }

TInt LocalTimer::PostInit()
    {
    return iTimer.CreateLocal();
    }

void LocalTimer::Start(TUint64 aDelay)
    {
    if (!IsActive())
        {
        iDelay = aDelay;
        /* Convert milli to micro */
        TTimeIntervalMicroSeconds32 delay(aDelay * 1000);
        iStatus = KRequestPending;
        iTimer.After(iStatus, delay);
        SetActive();
        }
    }

void LocalTimer::Stop()
    {
    Cancel();
    }

void LocalTimer::ResetTime()
    {
    iTime = 0;
    }

TUint64 LocalTimer::GetTime()
    {
    return iTime;
    }

void LocalTimer::RunL()
    {
    if (iStatus == KErrNone)
        {
        iTime += iDelay;
        XAuint64 position;
        iParent->GetRecordPosition(&position);

        XARecordItfAdaptMMF_PositionUpdate(iAdaptContext, position);
        Start(iDelay);
        }
    }

void LocalTimer::DoCancel()
    {
    iTimer.Cancel();
    }
