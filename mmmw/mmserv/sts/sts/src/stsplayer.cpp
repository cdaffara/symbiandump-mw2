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
 * This file provides the implementation for creating and deleting a
 * an MMF based player for playing and stopping a tone playback.
 */

//  Include Files  
#include "stsplayer.h"
#include <AudioPreference.h>
#include <systemtoneservice.h>
#include "stsplayersettings.h"
#include "stssettingsmanager.h"
#include "stsdebug.h"

/*static*/CStsPlayer* CStsPlayer::Create(MStsPlayerObserver& aObserver,
        CStsSettingsManager& aSettingsManager, unsigned int aContext,
        CSystemToneService::TAlarmType aAlarm)
    {
    CStsPlayer* self = 0;
    CStsSettingsManager::MPlayerSettings& playerSettings =
            aSettingsManager.GetPlayerSettings(aAlarm);
    TUint audioPreference;
    TUint audioPriority;
    aSettingsManager.GetAudioPriorityPref(aAlarm, audioPriority,
            audioPreference);
    self = new CStsPlayer(aObserver, playerSettings, aContext,
            audioPreference, audioPriority);
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

/*static*/CStsPlayer* CStsPlayer::Create(MStsPlayerObserver& aObserver,
        CStsSettingsManager& aSettingsManager, unsigned int aContext,
        CSystemToneService::TToneType aTone)
    {
    CStsPlayer* self = 0;
    CStsSettingsManager::MPlayerSettings& playerSettings =
            aSettingsManager.GetPlayerSettings(aTone);
    TUint audioPreference;
    TUint audioPriority;
    aSettingsManager.GetAudioPriorityPref(aTone, audioPriority,
            audioPreference);
    self = new CStsPlayer(aObserver, playerSettings, aContext,
            audioPreference, audioPriority);
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

CStsPlayer::CStsPlayer(MStsPlayerObserver& aObserver,
        CStsSettingsManager::MPlayerSettings& aPlayerSettings,
        unsigned int aContext, TUint aAudioPreference, TUint aAudioPriority) :
    iObserver(aObserver), iPlayerSettings(aPlayerSettings),
            iContext(aContext), iAudioPreference(aAudioPreference),
            iAudioPriority(aAudioPriority), iPlayer(0)
    {
    }

bool CStsPlayer::Init()
    {
    TRAPD(result, iPlayer = CMdaAudioPlayerUtility::NewL(*this));
    TRACE_LOG((_L("CStsPlayer::Init() : CMdaAudioPlayerUtility::NewL result[%d]"), result));
    return result == KErrNone;
    }

CStsPlayer::~CStsPlayer()
    {
    TRACE_FUNCTION_ENTRY;
    delete iPlayer;
    TRACE_FUNCTION_EXIT;
    }

void CStsPlayer::Play()
    {
    // Play the tone
    TRAPD( err, iPlayer->OpenFileL(iPlayerSettings.GetFileName()) );
    TRACE_LOG((_L("CStsPlayer::Play() : CMdaAudioPlayerUtility::OpenFileL result[%d]"), err));
    // If there is an error, indicate that the playback is complete. 
    if (err)
        {
        //TODO: Add trace here
        iObserver.PlayComplete(iContext);
        }
    }

void CStsPlayer::Stop()
    {
    TRACE_FUNCTION_ENTRY;
    iPlayer->Stop();
    TRACE_FUNCTION_EXIT;
    }

void CStsPlayer::MapcInitComplete(TInt aError,
        const TTimeIntervalMicroSeconds& /*aDuration*/)
    {
    if (aError == KErrNone)
        {
        TInt64 volumex100 = iPlayer->MaxVolume()
                * iPlayerSettings.GetVolumePercentage();
        TInt volume = volumex100 / 100;
        // Set PriorityPref
        iPlayer->SetPriority(iAudioPriority, iAudioPreference);
        iPlayer->SetRepeats(iPlayerSettings.GetNumberOfRepeats(),
                iPlayerSettings.GetRepeatDelay());
        iPlayer->SetVolume(volume);
        iPlayer->SetVolumeRamp(iPlayerSettings.GetVolumeRamp());
        TRACE_LOG((_L("CStsPlayer::Play() Settings-Priority[%d]Preference[0x%x]Repeats[%d]Volume[%d]"), 
                iAudioPriority,
                iAudioPreference,
                iPlayerSettings.GetNumberOfRepeats(),
                volume));
        TRACE_LOG((_L("CStsPlayer::Play() Settings-RepeatDelay[%d]VolumeRamp[0x%x]"), 
                iPlayerSettings.GetRepeatDelay().Int64(),
                iPlayerSettings.GetVolumeRamp().Int64()));
        iPlayer->Play();
        }
    else
        {
        //TODO: add trace
        // Since there is an error, indicate that the playback is complete
        //RDebug::Print(_L("InitCompleteReturned"), aError);
        TRACE_LOG((_L("CStsPlayer::MapcInitComplete() Error[%d]"), aError));
        iObserver.PlayComplete(iContext);
        }
    }

void CStsPlayer::MapcPlayComplete(TInt aError)
    {
    TRACE_LOG((_L("CStsPlayer::MapcPlayComplete() Error[%d]"), aError));
    if (aError != KErrNone)
        {
        //TODO: add trace
        }
    iObserver.PlayComplete(iContext);
    }
