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

#ifndef CMARKERPOSITIONTIMER_H
#define CMARKERPOSITIONTIMER_H

#include <e32base.h>
#include "openmaxalwrapper.h"

class CMdaAudioPlayerUtility;
class CVideoPlayerUtility2;

NONSHARABLE_CLASS(CMarkerPositionTimer) : public CActive
    {
public:
    // Construct/destruct
    static CMarkerPositionTimer* NewL(CMdaAudioPlayerUtility* aAudioPlayer,
            CVideoPlayerUtility2* aVideoPlayer);
    virtual ~CMarkerPositionTimer();

public:
    void SetContext(TAny* aCtx);
    void SetMarkerPosition(XAmillisecond aMarkerPos);
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
    CMarkerPositionTimer(CMdaAudioPlayerUtility* aAudioPlayer,
            CVideoPlayerUtility2* aVideoPlayer);
    void ConstructL();
    TInt GetCurrentPlayPosition(TTimeIntervalMicroSeconds& aPos);

private:
    RTimer iTimer; // Has
    TAny* iCtx;
    CMdaAudioPlayerUtility* iAudioPlayer;
    CVideoPlayerUtility2* iVideoPlayer;
    CBase* iPlayerToUse;
    XAmillisecond iMarkerPosition;
    XAuint32 iCallbackEventMask;
    xaPlayCallback iCallback;
    TBool iSyncToPlayHead;
    TTimeIntervalMicroSeconds iSyncToPlayHeadStartPos;
    };

#endif /* CMARKERPOSITIONTIMER_H */
