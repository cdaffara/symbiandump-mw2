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
 * Description: Handles new position timer implementation 
 *
 */

#include "positionupdatetimer.h"
#include <mdaaudiosampleplayer.h>
#include <videoplayer2.h>
#include <e32math.h>

extern "C"
    {
#include "xarecorditfadaptationmmf.h"
    }

#define RET_ERR_IF_ERR(s) if (s!=KErrNone) return s;
#define RET_IF_ERR(s) if (s!=KErrNone) return;

CPositionUpdateTimer::CPositionUpdateTimer(
        CMdaAudioPlayerUtility* aAudioPlayer,
        CVideoPlayerUtility2* aVideoPlayer) :
    CActive(CActive::EPriorityStandard), iAudioPlayer(aAudioPlayer),
            iVideoPlayer(aVideoPlayer)
    {
    CActiveScheduler::Add(this);
    }

CPositionUpdateTimer::~CPositionUpdateTimer()
    {
    Cancel();
    iTimer.Close();
    }

CPositionUpdateTimer* CPositionUpdateTimer::NewL(
        CMdaAudioPlayerUtility* aAudioPlayer,
        CVideoPlayerUtility2* aVideoPlayer)
    {
    CPositionUpdateTimer* self = new (ELeave) CPositionUpdateTimer(
            aAudioPlayer, aVideoPlayer);
    CleanupStack::PushL(self);
    self->CostructL();
    CleanupStack::Pop(self);
    return self;
    }

void CPositionUpdateTimer::CostructL()
    {
    User::LeaveIfError(iTimer.CreateLocal());
    }

void CPositionUpdateTimer::SetContext(TAny* aCtx)
    {
    iCtx = aCtx;
    }

void CPositionUpdateTimer::SetPositionUpdatePeriod(XAmillisecond aPos)
    {
    iPositionUpdatePeriod = aPos;
    iDelay = TTimeIntervalMicroSeconds32(aPos * 1000);
    }

void CPositionUpdateTimer::SetCallbackEventMask(XAuint32 aMask)
    {
    iCallbackEventMask = aMask;
    }

void CPositionUpdateTimer::RegisterCallback(xaPlayCallback aCallback)
    {
    iCallback = aCallback;
    }

void CPositionUpdateTimer::UseAudioPlayer()
    {
    iPlayerToUse = static_cast<CBase*> (iAudioPlayer);
    }
void CPositionUpdateTimer::UseVideoPlayer()
    {
    iPlayerToUse = static_cast<CBase*> (iVideoPlayer);
    }

void CPositionUpdateTimer::ResetPlayer()
    {
    iPlayerToUse = NULL;
    }

TInt CPositionUpdateTimer::Start()
    {
    TInt retVal(KErrNone);
    if (IsActive())
        {
        Cancel();
        }
    if ((iCallbackEventMask & XA_PLAYEVENT_HEADATNEWPOS) && iCallback
            && iPlayerToUse)
        {
        TTimeIntervalMicroSeconds curPlayPos;
        /* Convert milli to micro */
        TTimeIntervalMicroSeconds posUpdatePeriod(iPositionUpdatePeriod
                * 1000);
        TTimeIntervalMicroSeconds32 delay;
        /* Convert milli to micro */
        TReal res;
        TReal p;
        TReal q(posUpdatePeriod.Int64());

        iSyncToPlayHead = ETrue;

        retVal = GetCurrentPlayPosition(iSyncToPlayHeadStartPos);
        RET_ERR_IF_ERR(retVal);

        p = iSyncToPlayHeadStartPos.Int64();
        /* Adjust delay based on current play position
         * user may call this in the middle of playback */
        retVal = Math::Mod(res, p, q);
        RET_ERR_IF_ERR(retVal);

        /* Let the timer go off early and then re-adjust 
         * the remaining time based on then current position */
        delay = (posUpdatePeriod.Int64() - res) / 2;
#ifdef POSITIONUPDATETIMERLOG
        // Keep this block for debugging purposes
        RDebug::Print(_L("CPositionUpdateTimer::RunL:SyncPlayHead[%u]Delay Reset[%u]microSec"),
                iSyncToPlayHeadStartPos.Int64(),
                delay.Int());
#endif /* POSITIONUPDATETIMERLOG */
        if (delay >= TTimeIntervalMicroSeconds32(0))
            {
            iStatus = KRequestPending;
            iTimer.After(iStatus, delay);
            SetActive();
            }
        }
    return retVal;
    }

void CPositionUpdateTimer::Stop()
    {
    Cancel();
    }

void CPositionUpdateTimer::RunL()
    {
    TInt retVal(KErrNone);
    /* Make sure some of the attributes are not unset */
    if ((iStatus == KErrNone) && iCtx && (iCallbackEventMask
            & XA_PLAYEVENT_HEADATNEWPOS) && iCallback && iPlayerToUse)
        {
        TTimeIntervalMicroSeconds curPlayPos;
        if (iSyncToPlayHead)
            {
            retVal = GetCurrentPlayPosition(curPlayPos);
            RET_IF_ERR(retVal);

            /* If the play head hasn't moved, re-start all over again */
            if (curPlayPos == iSyncToPlayHeadStartPos)
                {
#ifdef POSITIONUPDATETIMERLOG
                RDebug::Print(_L("CPositionUpdateTimer::RunL:CurPlayPos[%u]SyncPlayHead[%u]microSec. Restart"),
                        iSyncToPlayHeadStartPos.Int64(),
                        curPlayPos.Int64());
#endif /* POSITIONUPDATETIMERLOG */
                Start();
                return;
                }
            /* Play head has moved. calculate remaining time and set the timer */
            /* Convert milli to micro */
            TTimeIntervalMicroSeconds posUpdatePeriod(iPositionUpdatePeriod
                    * 1000);
            TReal res;
            TReal p;
            TReal q(posUpdatePeriod.Int64());

            p = curPlayPos.Int64();
            iSyncToPlayHead = EFalse;

            retVal = Math::Mod(res, p, q);
            RET_IF_ERR(retVal);

            TTimeIntervalMicroSeconds32 delay = (posUpdatePeriod.Int64()
                    - res);
#ifdef POSITIONUPDATETIMERLOG
            RDebug::Print(_L("CPositionUpdateTimer::RunL:CurPlayPos[%u]SyncPlayHead[%u]Delay Reset[%u]microSec"),
                    iSyncToPlayHeadStartPos.Int64(),
                    curPlayPos.Int64(),
                    delay.Int());
#endif /* POSITIONUPDATETIMERLOG */
            if (delay >= TTimeIntervalMicroSeconds32(0))
                {
                iStatus = KRequestPending;
                iTimer.After(iStatus, delay);
                SetActive();
                }
            return;
            } /* of if (iSyncToPlayHead) */

#ifdef POSITIONUPDATETIMERLOG
        retVal = GetCurrentPlayPosition(curPlayPos);
        RDebug::Print(_L("CPositionUpdateTimer::RunL:CurPlayPos[%u]microSec. Posting XA_PLAYEVENT_HEADATNEWPOS."), curPlayPos.Int64());
#endif /* POSITIONUPDATETIMERLOG */
        XAAdaptEvent xaevent =
            {
            XA_PLAYITFEVENTS, XA_PLAYEVENT_HEADATNEWPOS, 0, 0
            };
        XAAdaptationBase_SendAdaptEvents((XAAdaptationBaseCtx*) iCtx,
                &xaevent);
        iStatus = KRequestPending;
        iTimer.After(iStatus, iDelay);
        SetActive();
        }
    }

void CPositionUpdateTimer::DoCancel()
    {
    iTimer.Cancel();
    }

TInt CPositionUpdateTimer::RunError(TInt aError)
    {
    return aError;
    }

TInt CPositionUpdateTimer::GetCurrentPlayPosition(
        TTimeIntervalMicroSeconds& aPos)
    {
    TTimeIntervalMicroSeconds pos;
    TInt err(KErrNone);
    if (iPlayerToUse && (iPlayerToUse == iAudioPlayer))
        {
        iAudioPlayer->GetPosition(aPos);
        }
    else if (iPlayerToUse && (iPlayerToUse == iVideoPlayer))
        {
        TRAP(err, aPos = iVideoPlayer->PositionL());
        }
    return err;
    }
