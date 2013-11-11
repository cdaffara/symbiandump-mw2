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

#include "prefetchlevelupdatetimer.h"
#include <mdaaudiosampleplayer.h>
#include <mdaaudiosampleplayer.h>
#include <videoplayer2.h>
#include <e32math.h>

extern "C"
{
	#include "xaadaptationmmf.h"
}

#define RET_ERR_IF_ERR(s) if (s!=KErrNone) return s;
#define RET_IF_ERR(s) if (s!=KErrNone) return;

CPrefetchLevelUpdateTimer::CPrefetchLevelUpdateTimer(	CMdaAudioPlayerUtility* aAudioPlayer,
        												CVideoPlayerUtility2* aVideoPlayer) :
				    										CActive(CActive::EPriorityStandard), 
															iAudioPlayer(aAudioPlayer),
															iVideoPlayer(aVideoPlayer),
															iNotifyIncrement(10),
															iLastIncrement(0)
{
	CActiveScheduler::Add(this);
}

CPrefetchLevelUpdateTimer::~CPrefetchLevelUpdateTimer()
{
	Cancel();
    iTimer.Close();
}

CPrefetchLevelUpdateTimer* CPrefetchLevelUpdateTimer::NewL(	CMdaAudioPlayerUtility* aAudioPlayer,
        														CVideoPlayerUtility2* aVideoPlayer)
{
	CPrefetchLevelUpdateTimer* self = new (ELeave) CPrefetchLevelUpdateTimer(aAudioPlayer,aVideoPlayer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

void CPrefetchLevelUpdateTimer::ConstructL()
{
	User::LeaveIfError(iTimer.CreateLocal());
}

void CPrefetchLevelUpdateTimer::SetContext(TAny* aCtx)
{
	iCtx = aCtx;
}

void CPrefetchLevelUpdateTimer::SetCallbackEventMask(XAuint32 aMask)
{
	iCallbackEventMask = aMask;
}

void CPrefetchLevelUpdateTimer::SetPrefetchLevelUpdatePeriod(XAmillisecond aPos)
{
    iUpdatePeriod = TTimeIntervalMicroSeconds32(aPos * 1000);
}

void CPrefetchLevelUpdateTimer::SetUpdateIncrement(XApermille inc)
{
	if(0<=inc && inc<=1000)
	{
		iNotifyIncrement = inc / 10;
	}
}

void CPrefetchLevelUpdateTimer::UseAudioPlayer()
{
	iPlayerToUse = static_cast<CBase*> (iAudioPlayer);
}
void CPrefetchLevelUpdateTimer::UseVideoPlayer()
{
	iPlayerToUse = static_cast<CBase*> (iVideoPlayer);
}

void CPrefetchLevelUpdateTimer::ResetPlayer()
{
    iPlayerToUse = NULL;
}

void CPrefetchLevelUpdateTimer::Start()
{
    if (IsActive())
    {
	    Cancel();
	}

	SendFillLevelChangeEvent(); //send a fill level change event (0%) right away 
	iLastIncrement = 0;

	
	iStatus = KRequestPending;
	iTimer.After(iStatus, iUpdatePeriod);
	SetActive();
}

void CPrefetchLevelUpdateTimer::Stop()
{
    if (IsActive())
    {
		SendFillLevelChangeEvent(); //send a fill level change event (100%)
	}

	Cancel();
	iLastIncrement = 0;
}

void CPrefetchLevelUpdateTimer::RunL()
{
    if ((iStatus == KErrNone) && iPlayerToUse)
	{
		TInt progress = 0;
		if(GetLoadingProgress(progress) == KErrNone)
		{
			if(progress==100)
			{
				Stop();
			}
			else if(progress >= (iLastIncrement + iNotifyIncrement))
			{
				SendFillLevelChangeEvent();
				iLastIncrement = progress - (progress % iNotifyIncrement);
			}
		}
	}

	iStatus = KRequestPending;
    iTimer.After(iStatus, iUpdatePeriod);
    SetActive();
}

void CPrefetchLevelUpdateTimer::DoCancel()
{
    iTimer.Cancel();
}

TInt CPrefetchLevelUpdateTimer::RunError(TInt aError)
{
    return aError;
}

TInt CPrefetchLevelUpdateTimer::GetLoadingProgress(TInt& progress)
{
    TInt err(KErrNone);
    if (iPlayerToUse && (iPlayerToUse == iAudioPlayer))
    {
		//TRAP(err, iAudioPlayer->GetAudioLoadingProgressL(progress));
        //always retrun 100 for audio
        progress = 100;
    }
    else if (iPlayerToUse && (iPlayerToUse == iVideoPlayer))
    {
        TRAP(err, iVideoPlayer->GetVideoLoadingProgressL(progress));
    }
    else
	{
		err = KErrNotFound;
    }

    return err;
}

void CPrefetchLevelUpdateTimer::SendFillLevelChangeEvent()
{
	if(iCallbackEventMask & XA_PREFETCHEVENT_FILLLEVELCHANGE)
	{		
		//use callback to send progress
		XAAdaptEvent xaevent =
		{
			XA_PREFETCHITFEVENTS, XA_PREFETCHEVENT_FILLLEVELCHANGE, 0, 0
		};
	
		XAAdaptationBase_SendAdaptEvents((XAAdaptationBaseCtx*) iCtx, &xaevent);
	}

}
	

