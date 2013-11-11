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
#ifndef STSSETTINGSMANAGER_H_
#define STSSETTINGSMANAGER_H_

#include <e32std.h>
#include <MProEngActiveProfileObserver.h>
#include <MProEngProfileActivationObserver.h>
#include <systemtoneservice.h>

class CStsPlayerSettings;
class MProEngEngine;
class MProEngNotifyHandler;

class CStsSettingsManager : private MProEngActiveProfileObserver,
        MProEngProfileActivationObserver
    {
public:
    static CStsSettingsManager* Create();
    static void Delete(CStsSettingsManager* aStsSettingsManager);

public:
    // Data types
    class MPlayerSettings
        {
    public:
        virtual TPtrC GetFileName() = 0;
        virtual int GetNumberOfRepeats() = 0;
        virtual TTimeIntervalMicroSeconds& GetRepeatDelay() = 0;
        virtual int GetVolumePercentage() = 0;
        virtual TTimeIntervalMicroSeconds& GetVolumeRamp() = 0;
        };

    MPlayerSettings& GetPlayerSettings(CSystemToneService::TAlarmType aAlarm);
    MPlayerSettings& GetPlayerSettings(CSystemToneService::TToneType aTone);
    void GetAudioPriorityPref(CSystemToneService::TAlarmType aAlarm,
            TUint& aPriority, TUint& aPreference);
    void GetAudioPriorityPref(CSystemToneService::TToneType aTone,
            TUint& aPriority, TUint& aPreference);

protected:
    CStsSettingsManager();
    virtual ~CStsSettingsManager();
    bool Init();

private:
    bool LoadActiveProfileSettings();
    // inherited from MProEngActiveProfileObserver
    virtual void HandleActiveProfileModifiedL();
    //inherited from MProEngProfileActivationObserver
    virtual void HandleProfileActivatedL(TInt aProfileId);

    MProEngEngine* iEngine;
    MProEngNotifyHandler* iNotifyHandler;
    CStsPlayerSettings* iPlayerSettings;
    };

#endif //STSSETTINGSMANAGER_H_
