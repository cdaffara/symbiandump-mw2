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
 * Description:
 * This file defines the interface for creating, playing,
 * stopping, and deleting an MMF player for a tone type.
 */
#ifndef STSPLAYER_H_
#define STSPLAYER_H_

#include <systemtoneservice.h>
#include <mdaaudiosampleplayer.h>
#include "stssettingsmanager.h"

class MStsPlayerObserver
    {
public:
    virtual void PlayComplete(unsigned int aContext) = 0;
    };

class CStsPlayer : private MMdaAudioPlayerCallback
    {
public:
    static CStsPlayer* Create(MStsPlayerObserver& aObserver,
            CStsSettingsManager& aSettingsManager, unsigned int aContext,
            CSystemToneService::TAlarmType aAlarm);
    static CStsPlayer* Create(MStsPlayerObserver& aObserver,
            CStsSettingsManager& aSettingsManager, unsigned int aContext,
            CSystemToneService::TToneType aTone);
    virtual ~CStsPlayer();
    void Play();
    void Stop();

protected:
    CStsPlayer(MStsPlayerObserver& aObserver,
            CStsSettingsManager::MPlayerSettings& aPlayerSettings,
            unsigned int aContext, TUint aAudioPreference,
            TUint aAudioPriority);
    bool Init();

private:
    void MapcInitComplete(TInt aError,
            const TTimeIntervalMicroSeconds& aDuration);
    void MapcPlayComplete(TInt aError);

    MStsPlayerObserver& iObserver;
    CStsSettingsManager::MPlayerSettings& iPlayerSettings;
    unsigned int iContext;
    TUint iAudioPreference;
    TUint iAudioPriority;
    TInt iVolume;
    CMdaAudioPlayerUtility* iPlayer;
    };

#endif // STSPLAYER_H_
