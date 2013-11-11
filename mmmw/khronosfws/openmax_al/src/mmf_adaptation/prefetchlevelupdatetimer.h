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

#ifndef PREFETCHLEVELUPDATETIMER_H
#define PREFETCHLEVELUPDATETIMER_H

#include <e32base.h>
#include "openmaxalwrapper.h"

class CMdaAudioPlayerUtility;
class CVideoPlayerUtility2;

NONSHARABLE_CLASS(CPrefetchLevelUpdateTimer) : public CActive
//class CPrefetchLevelUpdateTimer : public CActive
    {
public:
    // Construct/destruct
    static CPrefetchLevelUpdateTimer* NewL(	CMdaAudioPlayerUtility* aAudioPlayer,
            								CVideoPlayerUtility2* aVideoPlayer);
    virtual ~CPrefetchLevelUpdateTimer();

public:
    void SetContext(TAny* aCtx);
    void SetPrefetchLevelUpdatePeriod(XAmillisecond aPos);
	void SetUpdateIncrement(XApermille inc);
    void SetCallbackEventMask(XAuint32 aMask);
    void RegisterCallback(xaPrefetchCallback aCallback);
    void Start();
    void Stop();

	TInt GetLoadingProgress(TInt&);

    void UseAudioPlayer();
    void UseVideoPlayer();
    void ResetPlayer();
protected:
    // From CActive
    void RunL();
    void DoCancel();
    TInt RunError(TInt aError);

private:
    CPrefetchLevelUpdateTimer(	CMdaAudioPlayerUtility* aAudioPlayer,
            					CVideoPlayerUtility2* aVideoPlayer);
    void ConstructL();

	void SendFillLevelChangeEvent();

private:
    RTimer iTimer; // Has
    TAny* iCtx;
    XAuint32 iCallbackEventMask;
    xaPrefetchCallback iCallback;
    TTimeIntervalMicroSeconds32 iUpdatePeriod;

    CBase* iPlayerToUse;
    CMdaAudioPlayerUtility* iAudioPlayer;
    CVideoPlayerUtility2* iVideoPlayer;

	TInt iNotifyIncrement;
	TInt iLastIncrement;
};

#endif /* PREFETCHLEVELUPDATETIMER_H */
