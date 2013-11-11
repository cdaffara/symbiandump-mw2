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

#ifndef CPOSITIONUPDATETIMER_H
#define CPOSITIONUPDATETIMER_H

#include <e32base.h>
#include "openmaxalwrapper.h"

class CMdaAudioPlayerUtility;
class CVideoPlayerUtility2;

NONSHARABLE_CLASS(CPositionUpdateTimer) : public CActive
    {
public:
    // Construct/destruct
    static CPositionUpdateTimer* NewL(CMdaAudioPlayerUtility* aAudioPlayer,
            CVideoPlayerUtility2* aVideoPlayer);
    virtual ~CPositionUpdateTimer();

public:
    void SetContext(TAny* aCtx);
    void SetPositionUpdatePeriod(XAmillisecond aPos);
    void SetCallbackEventMask(XAuint32 aMask);
    void RegisterCallback(xaPlayCallback aCallback);
    void UseAudioPlayer();
    void UseVideoPlayer();
    void ResetPlayer();
    TInt Start();
    void Stop();

protected:
    // From CActive
    void RunL();
    void DoCancel();
    TInt RunError(TInt aError);

private:
    CPositionUpdateTimer(CMdaAudioPlayerUtility* aAudioPlayer,
            CVideoPlayerUtility2* aVideoPlayer);
    void CostructL();
    TInt GetCurrentPlayPosition(TTimeIntervalMicroSeconds& aPos);

private:
    RTimer iTimer; // Has
    TAny* iCtx;
    CMdaAudioPlayerUtility* iAudioPlayer;
    CVideoPlayerUtility2* iVideoPlayer;
    CBase* iPlayerToUse;
    XAmillisecond iPositionUpdatePeriod;
    XAuint32 iCallbackEventMask;
    xaPlayCallback iCallback;
    TTimeIntervalMicroSeconds32 iDelay;
    TBool iSyncToPlayHead;
    TTimeIntervalMicroSeconds iSyncToPlayHeadStartPos;
    };

#endif /* CPOSITIONUPDATETIMER_H */
