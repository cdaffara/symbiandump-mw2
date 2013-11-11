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
 * Description: Handles marker timer implementation 
 *
 */

#include "markerpositiontimer.h"
#include <mdaaudiosampleplayer.h>
#include <videoplayer2.h>
#include <e32math.h>

extern "C"
    {
#include "xarecorditfadaptationmmf.h"
    }

#define RET_ERR_IF_ERR(s) if (s!=KErrNone) return s;
#define RET_IF_ERR(s) if (s!=KErrNone) return;

CMarkerPositionTimer::CMarkerPositionTimer(
        CMdaAudioPlayerUtility* aAudioPlayer,
        CVideoPlayerUtility2* aVideoPlayer) :
    CActive(CActive::EPriorityStandard), iAudioPlayer(aAudioPlayer),
            iVideoPlayer(aVideoPlayer)
    {
    CActiveScheduler::Add(this);
    }

CMarkerPositionTimer::~CMarkerPositionTimer()
    {
    Cancel();
    iTimer.Close();
    }

CMarkerPositionTimer* CMarkerPositionTimer::NewL(
        CMdaAudioPlayerUtility* aAudioPlayer,
        CVideoPlayerUtility2* aVideoPlayer)
    {
    CMarkerPositionTimer* self = new (ELeave) CMarkerPositionTimer(
            aAudioPlayer, aVideoPlayer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

void CMarkerPositionTimer::ConstructL()
    {
    User::LeaveIfError(iTimer.CreateLocal());
    }

void CMarkerPositionTimer::SetContext(TAny* aCtx)
    {
    iCtx = aCtx;
    }

void CMarkerPositionTimer::SetMarkerPosition(XAmillisecond aMarkerPos)
    {
    iMarkerPosition = aMarkerPos;
    }

void CMarkerPositionTimer::SetCallbackEventMask(XAuint32 aMask)
    {
    iCallbackEventMask = aMask;
    }

void CMarkerPositionTimer::RegisterCallback(xaPlayCallback aCallback)
    {
    iCallback = aCallback;
    }

void CMarkerPositionTimer::UseAudioPlayer()
    {
    iPlayerToUse = static_cast<CBase*> (iAudioPlayer);
    }
void CMarkerPositionTimer::UseVideoPlayer()
    {
    iPlayerToUse = static_cast<CBase*> (iVideoPlayer);
    }

void CMarkerPositionTimer::ResetPlayer()
    {
    iPlayerToUse = NULL;
    }

TInt CMarkerPositionTimer::Start()
    {
    TInt retVal(KErrNone);
    if (IsActive())
        {
        Cancel();
        }
    if ((iMarkerPosition != XA_TIME_UNKNOWN) && (iCallbackEventMask
            & XA_PLAYEVENT_HEADATMARKER) && iCallback && iPlayerToUse)
        {
        TTimeIntervalMicroSeconds32 delay;
        /* Convert milli to micro */
        TTimeIntervalMicroSeconds markerPos(iMarkerPosition * 1000);
        TReal res;
        TReal p;
        TReal q(markerPos.Int64());

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
        delay = (markerPos.Int64() - res) / 2;
#ifdef MARKERPOSITIONTIMERLOG
        // Keep this block for debugging purposes
        RDebug::Print(_L("CMarkerPositionTimer::RunL:SyncPlayHead[%u]Delay Reset[%u]microSec"),
                iSyncToPlayHeadStartPos.Int64(),
                delay.Int());
#endif /* MARKERPOSITIONTIMERLOG */
        if (delay >= TTimeIntervalMicroSeconds32(0))
            {
            iStatus = KRequestPending;
            iTimer.After(iStatus, delay);
            SetActive();
            }
        }
    return retVal;
    }

void CMarkerPositionTimer::Stop()
    {
    Cancel();
    }

void CMarkerPositionTimer::RunL()
    {
    TInt retVal(KErrNone);
    /* Make sure some of the attributes are not unset */
    if ((iStatus == KErrNone) && iCtx && (iMarkerPosition != XA_TIME_UNKNOWN)
            && (iCallbackEventMask & XA_PLAYEVENT_HEADATMARKER) && iCallback
            && iPlayerToUse)
        {
        TTimeIntervalMicroSeconds curPlayPos;
        if (iSyncToPlayHead)
            {
            retVal = GetCurrentPlayPosition(curPlayPos);
            RET_IF_ERR(retVal);

            /* If the play head hasn't moved, re-start all over again */
            if (curPlayPos == iSyncToPlayHeadStartPos)
                {
#ifdef MARKERPOSITIONTIMERLOG
                RDebug::Print(_L("CMarkerPositionTimer::RunL:CurPlayPos[%u]SyncPlayHead[%u]microSec. Restart"),
                        iSyncToPlayHeadStartPos.Int64(),
                        curPlayPos.Int64());
#endif /* MARKERPOSITIONTIMERLOG */
                Start();
                return;
                }
            /* Play head has moved. calculate remaining time and set the timer */
            /* Convert milli to micro */
            TTimeIntervalMicroSeconds markerPos(iMarkerPosition * 1000);
            TReal res;
            TReal p;
            TReal q(markerPos.Int64());

            p = curPlayPos.Int64();
            iSyncToPlayHead = EFalse;

            retVal = Math::Mod(res, p, q);
            RET_IF_ERR(retVal);

            TTimeIntervalMicroSeconds32 delay = (markerPos.Int64() - res);
#ifdef MARKERPOSITIONTIMERLOG
            RDebug::Print(_L("CMarkerPositionTimer::RunL:CurPlayPos[%u]SyncPlayHead[%u]Delay Reset[%u]microSec"),
                    iSyncToPlayHeadStartPos.Int64(),
                    curPlayPos.Int64(),
                    delay.Int());
#endif /* MARKERPOSITIONTIMERLOG */
            if (delay >= TTimeIntervalMicroSeconds32(0))
                {
                iStatus = KRequestPending;
                iTimer.After(iStatus, delay);
                SetActive();
                }
            return;
            } /* of if (iSyncToPlayHead) */

#ifdef MARKERPOSITIONTIMERLOG
        retVal = GetCurrentPlayPosition(curPlayPos);
        RDebug::Print(_L("CMarkerPositionTimer::RunL:CurPlayPos[%u]microSec. Posting XA_PLAYEVENT_HEADATMARKER."), curPlayPos.Int64());
#endif /* MARKERPOSITIONTIMERLOG */
        XAAdaptEvent xaevent =
            {
            XA_PLAYITFEVENTS, XA_PLAYEVENT_HEADATMARKER, 0, 0
            };
        XAAdaptationBase_SendAdaptEvents((XAAdaptationBaseCtx*) iCtx,
                &xaevent);
        }
    }

void CMarkerPositionTimer::DoCancel()
    {
    iTimer.Cancel();
    }

TInt CMarkerPositionTimer::RunError(TInt /*aError*/)
    {
    return KErrNone;
    }

TInt CMarkerPositionTimer::GetCurrentPlayPosition(
        TTimeIntervalMicroSeconds& aPos)
    {
    TTimeIntervalMicroSeconds pos;
    TInt err(KErrNone);
    if (iPlayerToUse == iAudioPlayer)
        {
        iAudioPlayer->GetPosition(aPos);
        }
    else if (iPlayerToUse == iVideoPlayer)
        {
        TRAP(err, aPos = iVideoPlayer->PositionL());
        }
    return err;
    }
