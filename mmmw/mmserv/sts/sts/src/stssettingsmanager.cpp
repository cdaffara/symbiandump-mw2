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
 * This file provides the implementation of System Tone Service used
 * by the STS Server.
 */
//  Include Files  
#include "stssettingsmanager.h"
#include <AudioPreference.h>
#include <MProEngEngine.h>
#include <MProEngNotifyHandler.h>
#include <MProEngProfile.h>
#include <MProEngTones.h>
#include <MProEngToneSettings.h>
#include <ProEngFactory.h>
#include "stsplayersettings.h"
#include <NokiaAudioPreference.h>
_LIT(KDefaultFile,"z:\\data\\sounds\\digital\\clock.aac");

// Data types
/** The type of system tones that are supported by this API. */
enum TSettingsType
    {
    EDefaultAlarm = 0,
    EDefaultAlert,
    EEmailAlert,
    EIncomingCall,
    EIncomingCall2,
    EIncomingDataCall,
    EMessageAlert,
    EWarningBeep,
    EENDMARKER
    };

/*static*/CStsSettingsManager* CStsSettingsManager::Create()
    {
    CStsSettingsManager* self = new CStsSettingsManager();

    if (self != 0)
        {
        bool successful = self->Init();

        if (!successful)
            {
            delete self;
            self = 0;
            }
        }
    return self;
    }

/*static*/void CStsSettingsManager::Delete(
        CStsSettingsManager* aStsSettingsManager)
    {
    delete aStsSettingsManager;
    }

CStsSettingsManager::CStsSettingsManager() :
    iEngine(0), iNotifyHandler(0), iPlayerSettings(0)
    {
    }

bool CStsSettingsManager::Init()
    {
    bool successful = false;
    TRAPD( err, iEngine = ProEngFactory::NewEngineL() );
    if (err == KErrNone)
        {
        TRAP( err, iNotifyHandler = ProEngFactory::NewNotifyHandlerL() );
        }
    if (err == KErrNone)
        {
        TRAP( err, iNotifyHandler->RequestProfileActivationNotificationsL(*this) );
        }
    if (err == KErrNone)
        {
        TRAP( err, iNotifyHandler->RequestActiveProfileNotificationsL(*this) );
        }
    if (err == KErrNone)
        {
        iPlayerSettings = new CStsPlayerSettings[EENDMARKER];
        if (iPlayerSettings != 0)
            {
            successful = LoadActiveProfileSettings();
            }
        }
    return successful;
    }

CStsSettingsManager::~CStsSettingsManager()
    {
    if (iEngine != 0)
        {
        iEngine->Release();
        }
    if (iNotifyHandler != 0)
        {
        iNotifyHandler->CancelAll();
        delete iNotifyHandler;
        }
    delete[] iPlayerSettings;
    }

CStsSettingsManager::MPlayerSettings& CStsSettingsManager::GetPlayerSettings(
        CSystemToneService::TAlarmType aAlarm)
    {
    MPlayerSettings* result;
    switch (aAlarm)
        {
        case CSystemToneService::EIncomingCall:
            {
            result = &iPlayerSettings[EIncomingCall];
            break;
            }
        case CSystemToneService::EIncomingCallLine2:
            {
            result = &iPlayerSettings[EIncomingCall2];
            break;
            }
        case CSystemToneService::EIncomingDataCall:
            {
            result = &iPlayerSettings[EIncomingDataCall];
            break;
            }
        default:
            {
            result = &iPlayerSettings[EDefaultAlarm];
            break;
            }
        }
    return *result;
    }

CStsSettingsManager::MPlayerSettings& CStsSettingsManager::GetPlayerSettings(
        CSystemToneService::TToneType aTone)
    {
    MPlayerSettings* result;
    switch (aTone)
        {
        case CSystemToneService::EEmailAlert:
            {
            result = &iPlayerSettings[EEmailAlert];
            break;
            }
        case CSystemToneService::ESmsAlert:
        case CSystemToneService::EMmsAlert:
        case CSystemToneService::EChatAlert:
            {
            result = &iPlayerSettings[EMessageAlert];
            break;
            }
        case CSystemToneService::EWarningBeep:
            {
            result = &iPlayerSettings[EWarningBeep];
            break;
            }
        default:
            {
            result = &iPlayerSettings[EDefaultAlert];
            break;
            }
        }
    return *result;
    }

void CStsSettingsManager::GetAudioPriorityPref(
        CSystemToneService::TAlarmType aAlarm, TUint& aPriority,
        TUint& aPreference)
    {
    switch (aAlarm)
        {
        case CSystemToneService::EIncomingCall:
        case CSystemToneService::EIncomingCallLine2:
            {
            aPriority = KAudioPriorityRingingTone;
            aPreference = KAudioPrefIncomingCall;
            break;
            }
        case CSystemToneService::EIncomingDataCall:
            {
            aPriority = KAudioPriorityRealOnePlayer;
            aPreference = KAudioPrefIncomingDataCall;
            break;
            }
        default:
            {
            aPriority = KAudioPriorityAlarm;
            aPreference = KAudioPrefAlarmClock;
            break;
            }
        }
    }

void CStsSettingsManager::GetAudioPriorityPref(
        CSystemToneService::TToneType aTone, TUint& aPriority,
        TUint& aPreference)
    {
    switch (aTone)
        {
        case CSystemToneService::EEmailAlert:
        case CSystemToneService::ESmsAlert:
        case CSystemToneService::EMmsAlert:
        case CSystemToneService::EChatAlert:
            {
            aPriority = KAudioPriorityRecvMsg;
            aPreference = KAudioPrefNewSMS;
            break;
            }
        case CSystemToneService::EErrorBeep:
            {
            aPriority = KAudioPriorityHighLevel;
            aPreference = KAudioPrefError;
            break;
            }
        case CSystemToneService::EWarningBeep:
            {
            aPriority = KAudioPriorityWarningTone;
            aPreference = KAudioPrefWarning;
            break;
            }
        default:
            {
            aPriority = KAudioPriorityWarningTone;
            aPreference = KAudioPrefDefaultTone;
            break;
            }
        }
    }

bool CStsSettingsManager::LoadActiveProfileSettings()
    {
    bool successful = false;
    MProEngProfile* profile = 0;
    TRAPD( err, profile = iEngine->ActiveProfileL() );

    if (err == KErrNone)
        {
        MProEngTones& tones = profile->ProfileTones();
        MProEngToneSettings& settings = profile->ToneSettings();
        int volume = 50;
        if (profile->IsSilent())
            {
            volume = 0;
            }
        int warningVolume = 0;
        if (settings.WarningAndGameTones())
            {
            warningVolume = 50;
            }

        iPlayerSettings[EDefaultAlarm].SetFileName(KDefaultFile);
        iPlayerSettings[EDefaultAlert].SetFileName(KDefaultFile);
        iPlayerSettings[EEmailAlert].SetFileName(tones.EmailAlertTone());
        iPlayerSettings[EIncomingCall].SetFileName(tones.RingingTone1());
        iPlayerSettings[EIncomingCall2].SetFileName(tones.RingingTone2());
        iPlayerSettings[EIncomingDataCall].SetFileName(
                tones.VideoCallRingingTone());
        iPlayerSettings[EMessageAlert].SetFileName(tones.MessageAlertTone());
        iPlayerSettings[EWarningBeep].SetFileName(KDefaultFile);

        iPlayerSettings[EDefaultAlarm].SetNumberOfRepeats(5);
        iPlayerSettings[EDefaultAlert].SetNumberOfRepeats(0);
        iPlayerSettings[EEmailAlert].SetNumberOfRepeats(0);
        iPlayerSettings[EIncomingCall].SetNumberOfRepeats(100);
        iPlayerSettings[EIncomingCall2].SetNumberOfRepeats(100);
        iPlayerSettings[EIncomingDataCall].SetNumberOfRepeats(100);
        iPlayerSettings[EMessageAlert].SetNumberOfRepeats(0);
        iPlayerSettings[EWarningBeep].SetNumberOfRepeats(0);

        iPlayerSettings[EDefaultAlarm].SetRepeatDelay(1000);
        iPlayerSettings[EDefaultAlert].SetRepeatDelay(0);
        iPlayerSettings[EEmailAlert].SetRepeatDelay(0);
        iPlayerSettings[EIncomingCall].SetRepeatDelay(1000);
        iPlayerSettings[EIncomingCall2].SetRepeatDelay(1000);
        iPlayerSettings[EIncomingDataCall].SetRepeatDelay(1000);
        iPlayerSettings[EMessageAlert].SetRepeatDelay(0);
        iPlayerSettings[EWarningBeep].SetRepeatDelay(0);

        iPlayerSettings[EDefaultAlarm].SetVolumePercentage(volume);
        iPlayerSettings[EDefaultAlert].SetVolumePercentage(volume);
        iPlayerSettings[EEmailAlert].SetVolumePercentage(volume);
        iPlayerSettings[EIncomingCall].SetVolumePercentage(volume);
        iPlayerSettings[EIncomingCall2].SetVolumePercentage(volume);
        iPlayerSettings[EIncomingDataCall].SetVolumePercentage(volume);
        iPlayerSettings[EMessageAlert].SetVolumePercentage(volume);
        iPlayerSettings[EWarningBeep].SetVolumePercentage(warningVolume);

        iPlayerSettings[EDefaultAlarm].SetVolumeRamp(1000000);
        iPlayerSettings[EDefaultAlert].SetVolumeRamp(1000);
        iPlayerSettings[EEmailAlert].SetVolumeRamp(1000);
        iPlayerSettings[EIncomingCall].SetVolumeRamp(1000000);
        iPlayerSettings[EIncomingCall2].SetVolumeRamp(1000000);
        iPlayerSettings[EIncomingDataCall].SetVolumeRamp(1000000);
        iPlayerSettings[EMessageAlert].SetVolumeRamp(1000);
        iPlayerSettings[EWarningBeep].SetVolumeRamp(1000);

        profile->Release();
        successful = true;
        }
    return successful;
    }

void CStsSettingsManager::HandleActiveProfileModifiedL()
    {
    bool successful = LoadActiveProfileSettings();
    if (!successful)
        {
        //TODO: Log error
        }
    }

void CStsSettingsManager::HandleProfileActivatedL(TInt /*aProfileId*/)
    {
    bool successful = LoadActiveProfileSettings();
    if (!successful)
        {
        //TODO: Log error
        }
    }
