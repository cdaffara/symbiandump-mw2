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
 * Description: Telephony Multimedia Service
 *
 */

// INCLUDE FILES
#include <AudioPreference.h>
#include <MProEngTones.h>
#include <MProEngToneSettings.h>
#include <ProEngFactory.h>
#include "tmsshared.h"
#include "tmsutility.h"
#include "tmsrtplayerobsrv.h"
#include "tmsrtcontainer.h"
#include "tmsrtplayer.h"

using namespace TMS;

// CONSTANTS
const TInt KTMSMaxRingingWaiting = 3000000;
const TInt KTMSTtsAscendingStep = 3000000;
const TInt KRTDefaultVolume = 2;
//const TInt KTtsVolumeBoost = 9;
const TInt KTtsVolumeMin = 2;
const TInt KTtsVolumeAscendingRepeat = 4;
const TInt KPlayerVolumeAscendingRepeat = 3;
const TInt KTtsVolumeAscendingDecrease = 2;

// Delays between periods of Text-To-Speech playback
// {3000000, 4000000} means the following order of playing:
// 3 secs tone, then TTS, then 4 secs of tone, then TTS, then tone forever.
const TInt KTMSTtsDelays[] = {3000000, 4000000};
// Number of elements in KTMSTtsDelays array.
const TInt KTMSTtsDelaysCount = sizeof(KTMSTtsDelays) / sizeof (TInt);
const TInt Kkilo = 1024;

_LIT(KTMSTtsHeader, "(tts)");

#ifdef __WINSCW__
_LIT(KTMSDefaultSoundFile, "z:\\data\\sounds\\digital\\Nokia_tune.aac");
_LIT(KTMSNoSoundSequence,  "z:\\data\\sounds\\digital\\alarm.aac");
_LIT(KTMSBeepSequence,     "z:\\data\\sounds\\digital\\alarm.aac");
_LIT(KTMSUnsecureVoIPCall, "z:\\data\\sounds\\digital\\alarm.aac");
#else
// DO NOT change these sequences!
_LIT(KTMSDefaultSoundFile, "\\System\\Sounds\\Simple\\Nokia tune.wav");
_LIT8(KTMSBeepSequence, "\x00\x11\x0A\x0A\x08\x73\x0A\x40\x28\x0A\xF7\x05\
\xFC\x40\x64\x0A\x08\x40\x32\x0A\xF7\x06\x0B");
_LIT8(KTMSNoSoundSequence, "\x00\x11\x0A\x05\xFC\x0A\x08\x40\x32\x0A\xF7\
\x40\x64\x06\x0B");
_LIT8(KTMSUnsecureVoIPCall, "\x00\x11\x0B\x67\x04\x40\x04\x67\x04\x40\x04\
\x6C\x04\x40\x04\x6D\x04\x0B");
#endif // __WINSCW__

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::NewL
// -----------------------------------------------------------------------------
//
TMSRingTonePlayer* TMSRingTonePlayer::NewL(TMSRtPlayerObsrv& aObserver)
    {
    TMSRingTonePlayer* self = new (ELeave) TMSRingTonePlayer(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::TMSRingTonePlayer
// -----------------------------------------------------------------------------
//
TMSRingTonePlayer::TMSRingTonePlayer(TMSRtPlayerObsrv& aObserver) :
    iRtObserver(aObserver)
    {
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::ConstructL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::ConstructL()
    {
    TRACE_PRN_FN_ENT;

    for (TInt i = EPlayerFirst; i <= EPlayerLast; i++)
        {
        User::LeaveIfError(iAsyncDeletePlayers.Append(NULL));
        }

    // Construct timer.
    iTimer = TMSTimer::NewL();
    iTtsDelayTimer = TMSTimer::NewL();
    iAsyncReqAO = TMSAsyncReqAO::NewL(this);

    // TODO:
    // Checks if DRM extend security is needed
    // Check video player configuration.

    iEngine =  ProEngFactory::NewEngineL();

    InitRtParams(EPlayerBackup);
    //CreateDefaultRingToneL();
    CreateBkpRtPlayerL();
    //GetMaxToneFileSizeL(iToneFileSizeLimitKB);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::~TMSRingTonePlayer
// -----------------------------------------------------------------------------
//
TMSRingTonePlayer::~TMSRingTonePlayer()
    {
    TRACE_PRN_FN_ENT;

    iAsyncDeletePlayers.ResetAndDestroy();
    iAsyncDeletePlayers.Close();
    if (iProfile)
        {
        iProfile->Release();
        }
    if (iEngine)
        {
        iEngine->Release();
        }
    RemovePlayers();
    delete iBackupPlayer;
    iBackupPlayer = NULL;
    delete iAsyncReqAO;
    iAsyncReqAO = NULL;
    delete iTimer;
    iTimer = NULL;
    delete iTtsDelayTimer;
    iTtsDelayTimer = NULL;
    delete iRtContainer;
    iRtContainer = NULL;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::InitRtParams
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::InitRtParams(TRtPlayerType aPlayerType)
    {
    TRACE_PRN_FN_ENT;

    iRtParam.iType = aPlayerType;
    iRtParam.iPriority = KAudioPriorityPhoneCall;
    iRtParam.iPreference = KAudioPrefIncomingCall;
    iRtParam.iRingingType = EProfileRingingTypeRinging;
    iRtParam.iVolume = KRTDefaultVolume;
    iRtParam.iTextToSay.Set(KNullDesC());

    if (aPlayerType == EPlayerUnsecureVoIP)
        {
        iRtParam.iPriority = KAudioPrioritySpecialInformationTone;
        iRtParam.iPreference = KAudioPrefSpecialInformation;
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::CreateDefaultPlayerL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::CreateDefaultRingToneL()
    {
    TRACE_PRN_FN_ENT;

    InitRtParams(EPlayerDefault);
    LoadProfileRingToneL();

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::LoadActiveProfileSettingsL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::LoadActiveProfileSettingsL()
    {
    if (!iEngine)
        {
        iEngine =  ProEngFactory::NewEngineL();
        }
    if (iProfile)
        {
        iProfile->Release();
        iProfile = NULL;
        }
    __ASSERT_ALWAYS(iEngine, PANIC(TMS_RESULT_UNINITIALIZED_OBJECT));

    iProfile = iEngine->ActiveProfileL();
    if (iProfile)
        {
        MProEngToneSettings& settings = iProfile->ToneSettings();
        iRtParam.iVolume = settings.RingingVolume();
        iRtParam.iRingingType = settings.RingingType();
        iRtParam.iCallerText = settings.TextToSpeech();
        }
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::LoadProfileRingToneL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::LoadProfileRingToneL()
    {
    TRACE_PRN_FN_ENT;

    LoadActiveProfileSettingsL();
    __ASSERT_ALWAYS(iProfile, PANIC(TMS_RESULT_UNINITIALIZED_OBJECT));

    MProEngTones& tones = iProfile->ProfileTones();
    // Get alternate line RT: tones.RingingTone2()
    // Get default video RT: tones.VideoCallRingingTone()

    if (!iRtContainer)
        {
        iRtContainer = TMSRtContainer::NewL(tones.RingingTone1());
        }
    else
        {
        if (tones.RingingTone1().CompareF(iRtContainer->FileName()) != KErrNone)
            {
            // Profile changed - update RT
            iRtContainer->SetFileName(tones.RingingTone1());
            iRtContainer->RefreshMime();
            }
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::CreateBkpRtPlayerL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::CreateBkpRtPlayerL()
    {
    TRACE_PRN_FN_ENT;

    delete iBackupPlayer;
    iBackupPlayer = NULL;
    TFileName file(KTMSDefaultSoundFile);
    iBackupPlayer = TMSRtAudioHdlr::NewL(file, KAudioPriorityPhoneCall,
            KAudioPrefIncomingCall, *this, EPlayerBackup);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::CreateDefaultRtPlayerL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::CreateDefaultRtPlayerL()
    {
    TRACE_PRN_FN_ENT;

    // Fresh start for the default tone.
    CreateDefaultRingToneL();

    delete iDefaultPlayer;
    iDefaultPlayer = NULL;
    iDefaultPlayer = TMSRtAudioHdlr::NewL(*iRtContainer,
            KAudioPriorityPhoneCall, KAudioPrefIncomingCall, *this,
            EPlayerDefault, EFalse);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::CreateAudioPlayerL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::CreateAudioPlayerL()
    {
    TRACE_PRN_FN_ENT;

    delete iAudioPlayer;
    iAudioPlayer = NULL;
    iAudioPlayer = TMSRtAudioHdlr::NewL(*iRtContainer, iRtParam.iPriority,
            iRtParam.iPreference, *this, iRtParam.iType,
            iRtParam.iType == EPlayerDefault ? EFalse : iExtSecNeeded);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::CreateRingTonePlayerL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::CreateRingTonePlayerL(const TRtPlayerType aPlayer,
        const TDesC& aFile)
    {
    TRACE_PRN_FN_ENT;

    TInt len = aFile.Length();
    if (!len || aPlayer == EPlayerDefault)
        {
        CreateDefaultRtPlayerL();
        }
    else
        {
        if (!iRtContainer)
            {
            CreateDefaultRingToneL();
            }
        if (len && iRtContainer)
            {
            iRtContainer->SetFileName(aFile);
            iRtContainer->RefreshMime();
            iRtParam.iType = aPlayer;
            }
        CreateAudioPlayerL();
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::CreateSequencePlayerL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::CreateSequencePlayerL(const TRtPlayerType aPlayer)
    {
    TRACE_PRN_FN_ENT;

    switch (aPlayer)
        {
        case EPlayerBeepOnce:
            CreateBeepOncePlayerL();
            break;
        case EPlayerSilent:
            CreateSilentPlayerL();
            break;
        case EPlayerUnsecureVoIP:
            CreateUnsecureVoipPlayerL();
            break;
        default:
            break;
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::CreateBeepOncePlayerL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::CreateBeepOncePlayerL()
    {
    TRACE_PRN_FN_ENT;

    delete iBeepOncePlayer;
    iBeepOncePlayer = NULL;

#ifdef __WINSCW__
    TParse* fp = new (ELeave) TParse();
    fp->Set(KTMSBeepSequence, NULL, NULL);
    TFileName fileName(fp->FullName());
    delete fp;
    iBeepOncePlayer = TMSRtAudioHdlr::NewL(fileName, iRtParam.iPriority,
            iRtParam.iPreference, *this, EPlayerBeepOnce);
#else
    iBeepOncePlayer = TMSRtAudioHdlr::NewSeqL(KTMSBeepSequence(),
            iRtParam.iPriority, iRtParam.iPreference, *this, EPlayerBeepOnce);
#endif // __WINSCW__

    iRtParam.iType = EPlayerBeepOnce;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::CreateSilentPlayerL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::CreateSilentPlayerL()
    {
    TRACE_PRN_FN_ENT;

    iRtParam.iVolume = 0;
    delete iSilentPlayer;
    iSilentPlayer = NULL;

#ifdef __WINSCW__
    TParse* fp = new (ELeave) TParse();
    fp->Set(KTMSNoSoundSequence, NULL, NULL);
    TFileName fileName(fp->FullName());
    delete fp;
    iSilentPlayer = TMSRtAudioHdlr::NewL(fileName, iRtParam.iPriority,
            iRtParam.iPreference, *this, EPlayerSilent);
#else
    iSilentPlayer = TMSRtAudioHdlr::NewSeqL(KTMSNoSoundSequence(),
            iRtParam.iPriority, iRtParam.iPreference, *this, EPlayerSilent);
#endif // __WINSCW__

    iRtParam.iType = EPlayerSilent;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::CreateUnsecureVoipPlayerL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::CreateUnsecureVoipPlayerL()
    {
    TRACE_PRN_FN_ENT;

    delete iUnsecureVoIPTonePlayer;
    iUnsecureVoIPTonePlayer = NULL;

#ifdef __WINSCW__
    TParse* fp = new (ELeave) TParse();
    fp->Set(KTMSUnsecureVoIPCall, NULL, NULL);
    TFileName fileName(fp->FullName());
    delete fp;
    iUnsecureVoIPTonePlayer = TMSRtAudioHdlr::NewL(fileName,
            iRtParam.iPriority, iRtParam.iPreference, *this,
            EPlayerUnsecureVoIP);
#else
    iUnsecureVoIPTonePlayer = TMSRtAudioHdlr::NewSeqL(KTMSUnsecureVoIPCall(),
            iRtParam.iPriority, iRtParam.iPreference, *this,
            EPlayerUnsecureVoIP);
#endif // __WINSCW__

    iRtParam.iType = EPlayerUnsecureVoIP;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::CreateCustomSequencePlayerL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::CreateCustomSequencePlayerL(const TDesC8&
#ifdef __WINSCW__
        /*aSequence*/)
#else
        aSequence)
#endif
    {
    TRACE_PRN_FN_ENT;

    delete iSequencePlayer;
    iSequencePlayer = NULL;

#ifdef __WINSCW__
    TParse* fp = new (ELeave) TParse();
    fp->Set(KTMSBeepSequence, NULL, NULL);
    TFileName fileName(fp->FullName());
    delete fp;
    iSequencePlayer = TMSRtAudioHdlr::NewL(fileName, iRtParam.iPriority,
            iRtParam.iPreference, *this, EPlayerSequence);
#else
    iSequencePlayer = TMSRtAudioHdlr::NewSeqL(aSequence, iRtParam.iPriority,
            iRtParam.iPreference, *this, EPlayerSequence);
#endif // __WINSCW__

    iRtParam.iType = EPlayerSequence;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::CreateTTSPlayerL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::CreateTTSPlayerL(const TDesC& aTextToSay)
    {
    TRACE_PRN_FN_ENT;

    RemoveTTSPlayer();
    TPtrC ttsHeader(KTMSTtsHeader);
    HBufC* buf = HBufC::New(aTextToSay.Length() + ttsHeader.Length());
    if (buf)
        {
        TPtr ptr = buf->Des();
        ptr.Append(ttsHeader);
        ptr.Append(aTextToSay);
        TRACE_PRN_N1(_L("TMS->CreateTTSPlayerL: Text:[%S]"), &ptr);
        TRAPD(err, iTtsPlayer = TMSRtAudioHdlr::NewTtsL(*buf,
                KAudioPriorityTextToSpeechCall, KAudioPrefTextToSpeechCall,
                *this, EPlayerTts));
        if (err == TMS_RESULT_SUCCESS)
            {
            iTtsToneToBePlayed = ETrue;
            iRtParam.iTextToSay.Set(aTextToSay);
            }
        }
    delete buf;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::RemoveTTSPlayer
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::RemoveTTSPlayer()
    {
    TRACE_PRN_FN_ENT;

    delete iTtsPlayer;
    iTtsPlayer = NULL;
    iTtsToneToBePlayed = EFalse;
    iRtParam.iTextToSay.Set(KNullDesC());

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::PlayRingToneL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::PlayRingToneL()
    {
    TRACE_PRN_FN_ENT;

    // Check active Profile for any changes since Init()
    // TODO: To improve perfomrance, instead of polling Profile settings,
    //       consider subscribing to ProfilEngine P&S keys in CenRep.
    LoadActiveProfileSettingsL();

    switch (iRtParam.iType)
        {
        case EPlayerAudio:
            {
            PlayTtsTone();
            PlayAudioRingToneL();
            break;
            }
        case EPlayerBeepOnce:
            BeepOnceL();
            break;
        case EPlayerSilent:
            PlaySilentToneL();
            break;
        case EPlayerUnsecureVoIP:
            PlayUnsecureVoIPToneL();
            break;
        case EPlayerDefault:
            PlayTtsTone();
            PlayDefaultToneL();
            break;
        case EPlayerBackup:
            PlayBackupToneL();
            break;
        case EPlayerSequence:
            PlaySequenceToneL();
            break;
        default:
            break;
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::PlayAudioRingToneL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::PlayAudioRingToneL()
    {
    TRACE_PRN_FN_ENT;

    if (!iRtContainer || !CheckAndHandleToneSizeLimit(iRtContainer))
        {
        PlayDefaultToneL();
        }
    else if (iRtParam.iRingingType == EProfileRingingTypeSilent)
        {
        PlaySilentToneL();
        }
    else if (iRtParam.iRingingType == EProfileRingingTypeBeepOnce)
        {
        BeepOnceL();
        }
    else
        {
        if (!iAudioPlayer)
            {
            CreateAudioPlayerL();
            if (!iTimer->IsRunning())
                {
                // If InitComplete event doesn't occur within permittable time,
                // this will trigger fallback routine.
                iTimer->NotifyAfter(KTMSMaxRingingWaiting, *this);
                }
            }

        // Start playing.
        if (!iAudioPlayer)
            {
            iTimer->CancelNotify();
            PlayDefaultToneL();
            }
        else
            {
            iTonePlayingStatus = EAudioTonePlaying;
            iAudioPlayer->Play(ConvertRingingType(iRtParam.iRingingType),
                    iRtParam.iVolume, iTtsToneToBePlayed);
            }
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::BeepOnceL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::BeepOnceL()
    {
    TRACE_PRN_FN_ENT;

    if (!iBeepOncePlayer)
        {
        CreateBeepOncePlayerL();
        }
    if (iBeepOncePlayer)
        {
        iTonePlayingStatus = EBeepOnce;
        iBeepOncePlayer->Play(TMSRtAudioHdlr::ETypeRingingOnce,
                iRtParam.iVolume, EFalse);
        }
    else
        {
        PlayDefaultToneL();
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::PlaySilentToneL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::PlaySilentToneL()
    {
    TRACE_PRN_FN_ENT;

    if (!iSilentPlayer)
        {
        CreateSilentPlayerL();
        }
    if (iSilentPlayer)
        {
        iTonePlayingStatus = ESilentTonePlaying;
        iSilentPlayer->Play(TMSRtAudioHdlr::ETypeRinging, 0, EFalse);
        }
    else
        {
        PlayDefaultToneL();
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::PlayUnsecureVoIPToneL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::PlayUnsecureVoIPToneL()
    {
    TRACE_PRN_FN_ENT;

    if (!iUnsecureVoIPTonePlayer)
        {
        CreateUnsecureVoipPlayerL();
        }
    if (iUnsecureVoIPTonePlayer)
        {
        iTonePlayingStatus = EUnsecureVoIPTonePlaying;
        iUnsecureVoIPTonePlayer->Play(TMSRtAudioHdlr::ETypeRingingOnce,
                KRTDefaultVolume, EFalse);
        }
    else
        {
        PlayDefaultToneL();
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::PlaySequenceToneL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::PlaySequenceToneL()
    {
    TRACE_PRN_FN_ENT;

    if (iSequencePlayer)
        {
        iTonePlayingStatus = ESequenceTonePlaying;
        iSequencePlayer->Play(ConvertRingingType(iRtParam.iRingingType),
                iRtParam.iVolume, EFalse);
        }
    else
        {
        PlayDefaultToneL();
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::PlayTtsTone
// Schedule TTS to play, if text has been provided
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::PlayTtsTone()
    {
    TRACE_PRN_FN_ENT;

    TInt err = TMS_RESULT_SUCCESS;

    if (iRtParam.iTextToSay.Length() && !iTtsPlayer)
        {
        TRAP(err, CreateTTSPlayerL(iRtParam.iTextToSay));
        }
    if (err == TMS_RESULT_SUCCESS && iTtsPlayer && iTtsToneToBePlayed)
        {
        iTtsDelayIndex = 0;
        iTtsDelaysCount = KTMSTtsDelaysCount;

        // Start TTS timer just before RT
        if (!iTtsDelayTimer->IsRunning())
            {
            iTtsDelayTimer->NotifyAfter(KTMSTtsDelays[iTtsDelayIndex],
                    TCallBack(HandleTtsDelayTimeout, this));
            }
        iTtsVolume = iRtParam.iVolume;
        iTtsRingingType = iRtParam.iRingingType;
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::PlayBackupToneL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::PlayBackupToneL()
    {
    TRACE_PRN_FN_ENT;

    TInt err = TMS_RESULT_SUCCESS;

    if (!iBackupPlayer)
        {
        TRAP(err, CreateBkpRtPlayerL());
        }

    if (iBackupPlayer && err == TMS_RESULT_SUCCESS)
        {
        iTonePlayingStatus = EBackupTonePlaying;
        iTtsToneToBePlayed = EFalse;
        iTTsTimeOutCounter = 0;
        iTtsDelayTimer->CancelNotify();
        if (iTtsPlayer)
            {
            iTtsPlayer->StopPlaying();
            delete iTtsPlayer;
            iTtsPlayer = NULL;
            }
        iBackupPlayer->Play(ConvertRingingType(iRtParam.iRingingType),
                iRtParam.iVolume, EFalse);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::PlayDefaultToneL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::PlayDefaultToneL()
    {
    TRACE_PRN_FN_ENT;

    if (iRtParam.iRingingType == EProfileRingingTypeSilent ||
            iRtParam.iRingingType == EProfileRingingTypeBeepOnce)
        {
        return;
        }

    if (!iRtContainer)
        {
        // Default tone does not exist, play backup.
        PlayBackupToneL();
        return;
        }

    if (!iDefaultPlayer)
        {
        CreateDefaultRtPlayerL();
        if (!iTimer->IsRunning())
            {
            // If InitComplete event doesn't occur within permittable time,
            // this will trigger fallback routine.
            iTimer->NotifyAfter(KTMSMaxRingingWaiting, *this);
            }
        }

    // Start playing.
    if (!iDefaultPlayer)
        {
        // Default player not ready, play backup.
        iTimer->CancelNotify();
        PlayBackupToneL();
        }
    else
        {
        iTonePlayingStatus = EDefaultTonePlaying;
        iDefaultPlayer->Play(ConvertRingingType(iRtParam.iRingingType),
                iRtParam.iVolume, iTtsToneToBePlayed);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::StopPlaying
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::StopPlaying()
    {
    TRACE_PRN_FN_ENT;

    iTtsToneToBePlayed = EFalse;
    iTTsTimeOutCounter = 0;
    iTtsDelayTimer->CancelNotify();
    if (iTtsPlayer)
        {
        iTtsPlayer->StopPlaying();
        delete iTtsPlayer;
        iTtsPlayer = NULL;
        }
    iTimer->CancelNotify();

    switch (iTonePlayingStatus)
        {
        case EAudioTonePlaying:
            if (iAudioPlayer)
                {
                // Commented out to correct screen jamming after
                // REComSession::FinalClose(). Audio player will be stopped
                // in its destructor. Fixes EKKI-7WYG2G and JKAN-7XNGQU.
                // iAudioPlayer->StopPlaying();
                delete iAudioPlayer;
                iAudioPlayer = NULL;
                }
            break;
        case EDefaultTonePlaying:
            if (iDefaultPlayer)
                {
                iDefaultPlayer->StopPlaying();
                delete iDefaultPlayer;
                iDefaultPlayer = NULL;
                }
            break;
        case EBeepOnce:
            if (iBeepOncePlayer)
                {
                iBeepOncePlayer->StopPlaying();
                delete iBeepOncePlayer;
                iBeepOncePlayer = NULL;
                }
            break;
        case EBackupTonePlaying:
            if (iBackupPlayer)
                {
                iBackupPlayer->StopPlaying();
                }
            break;
        case ESilentTonePlaying:
            if (iSilentPlayer)
                {
                iSilentPlayer->StopPlaying();
                delete iSilentPlayer;
                iSilentPlayer = NULL;
                }
            break;
        case EUnsecureVoIPTonePlaying:
            if (iUnsecureVoIPTonePlayer)
                {
                iUnsecureVoIPTonePlayer->StopPlaying();
                delete iUnsecureVoIPTonePlayer;
                iUnsecureVoIPTonePlayer = NULL;
                }
            break;
        case ESequenceTonePlaying:
            if (iSequencePlayer)
                {
                iSequencePlayer->StopPlaying();
                delete iSequencePlayer;
                iSequencePlayer = NULL;
                }
            break;
        case EVideoTonePlaying: // video ringing tone
        case EPersonalVideoTonePlaying:
        case ESilentVideoTonePlaying:
            if (iBeepOncePlayer != NULL)
                {
                iBeepOncePlayer->StopPlaying();
                delete iBeepOncePlayer;
                iBeepOncePlayer = NULL;
                }
            if (iSilentPlayer != NULL)
                {
                iSilentPlayer->StopPlaying();
                delete iSilentPlayer;
                iSilentPlayer = NULL;
                }
            if (iRtContainer)
                {
                delete iRtContainer;
                iRtContainer = NULL;
                }
            break;
        case EIdle:
            // Do nothing here.
            return;
        default:
            break;
        }

    iTonePlayingStatus = EIdle;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::DeinitRingTonePlayer
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::DeinitRingTonePlayer()
    {
    StopPlaying();
    RemovePlayers();
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::RemovePlayers
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::RemovePlayers()
    {
    delete iAudioPlayer;
    iAudioPlayer = NULL;

    delete iDefaultPlayer;
    iDefaultPlayer = NULL;

    delete iBeepOncePlayer;
    iBeepOncePlayer = NULL;

    delete iSilentPlayer;
    iSilentPlayer = NULL;

    delete iUnsecureVoIPTonePlayer;
    iUnsecureVoIPTonePlayer = NULL;

    delete iSequencePlayer;
    iSequencePlayer = NULL;

    delete iTtsPlayer;
    iTtsPlayer = NULL;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::DeletePlayer
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::DeletePlayer()
    {
    switch (iTonePlayingStatus)
        {
        case EAudioTonePlaying:
            DeletePlayerAsync(EPlayerAudio);
            iAudioPlayer = NULL;
            break;
        case EDefaultTonePlaying:
            DeletePlayerAsync(EPlayerDefault);
            iDefaultPlayer = NULL;
            break;
        case ESilentTonePlaying:
            DeletePlayerAsync(EPlayerSilent);
            iSilentPlayer = NULL;
            break;
        case EUnsecureVoIPTonePlaying:
            DeletePlayerAsync(EPlayerUnsecureVoIP);
            iUnsecureVoIPTonePlayer = NULL;
            break;
        case EBeepOnce:
            DeletePlayerAsync(EPlayerBeepOnce);
            iBeepOncePlayer = NULL;
            break;
        case ESequenceTonePlaying:
            DeletePlayerAsync(EPlayerSequence);
            iSequencePlayer = NULL;
            break;
        case EBackupTonePlaying:
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::DeletePlayerAsync
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::DeletePlayerAsync(TRtPlayerType aPlayer)
    {
    TRACE_PRN_FN_ENT;

    __ASSERT_DEBUG(aPlayer >= EPlayerFirst &&
            aPlayer < iAsyncDeletePlayers.Count(),
            PANIC(TMS_RESULT_INVALID_ARGUMENT));

    // Player is deleted in HandleAsyncReq.

    if (iAsyncDeletePlayers[aPlayer])
        {
        delete iAsyncDeletePlayers[aPlayer];
        iAsyncDeletePlayers[aPlayer] = NULL;
        }

    TMSRtAudioHdlr** players[] = {
        &iAudioPlayer,
        &iBeepOncePlayer,
        &iSilentPlayer,
        &iUnsecureVoIPTonePlayer,
        &iDefaultPlayer,
        &iSequencePlayer,
        &iBackupPlayer,
        &iTtsPlayer
        };

    __ASSERT_DEBUG(iAsyncDeletePlayers.Count() > aPlayer,
            PANIC(TMS_RESULT_ILLEGAL_OPERATION));

    iAsyncDeletePlayers[aPlayer] = *(players[aPlayer]);
    *(players[aPlayer]) = NULL;
    iAsyncReq = ETrue;
    if (!iAsyncReqAO->IsActive())
        iAsyncReqAO->Start(); //Start async request

    TRACE_PRN_FN_ENT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::HandleError
// From TMSAsyncReqObsrv
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::HandleError(TInt /*aError*/)
    {
    // TODO: should we kill/restart a/o?
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::HandleAsyncReq
// From TMSAsyncReqObsrv
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::HandleAsyncReq()
    {
    TRACE_PRN_FN_ENT;

    if (iAsyncReq)
        {
        for (TInt i = EPlayerFirst; i <= EPlayerLast; i++)
            {
            delete iAsyncDeletePlayers[i];
            iAsyncDeletePlayers[i] = NULL;
            }
        }
    iAsyncReq = EFalse;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::HandleTtsDelayTimeout
// -----------------------------------------------------------------------------
//
TInt TMSRingTonePlayer::HandleTtsDelayTimeout(TAny* object)
    {
    static_cast<TMSRingTonePlayer*> (object)->DoHandleTtsDelayTimeout();
    return TMS_RESULT_SUCCESS;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::DoHandleTtsDelayTimeout
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::DoHandleTtsDelayTimeout()
    {
    TRACE_PRN_FN_ENT;

    TMSRtAudioHdlr* currPlayer = NULL;
    currPlayer = GetCurrentlyActiveAudioPlayerWithTTS();

    if (!currPlayer || !iTtsPlayer)
        {
        return;
        }

    TInt volume(0);

    // Ascending case. TTS player needs to be ascending.
    if (iTtsRingingType == EProfileRingingTypeAscending)
        {
        // Adjust volume for each ascending step
        if (!iTTsTimeOutCounter)
            {
            // Playing caller's name for the first time in ascending mode.
            volume = KTtsVolumeMin;
            iTtsPlayer->Play(ConvertRingingType(EProfileRingingTypeRingingOnce),
                    volume, iTtsToneToBePlayed);
            }
        else
            {
            // Checks the case that ringingtone is very quiet. Then do not play
            // TTS too loud
            volume = iTtsVolume < KTtsVolumeMin ?
                KTtsVolumeMin : KTtsVolumeAscendingRepeat;

            // We are saying callers name for the second time in ascending mode.
            // Say callers name by volume level KTtsVolumeAscendingRepeat and
            // decrease current players volume
            // to KPlayerVolumeAscendingRepeat. RampTime is zero
            currPlayer->SetNewVolumeAndRamptime(
                    KPlayerVolumeAscendingRepeat, 0);
            iTtsPlayer->Play(ConvertRingingType(EProfileRingingTypeRingingOnce),
                    volume, iTtsToneToBePlayed);
            }
        }
    else //Normal ringing case.
        {
        currPlayer->SetNewVolumeAndRamptime(iTtsVolume - 5, 0);
        iTtsPlayer->Play(ConvertRingingType(EProfileRingingTypeRingingOnce),
                iTtsVolume, iTtsToneToBePlayed);
        }

    iTTsTimeOutCounter++;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::MuteRingTone
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::MuteRingTone()
    {
    TMSRtAudioHdlr* activePlayer = ActiveAudioPlayer();
    if (activePlayer != NULL)
        {
        activePlayer->MutePlaying();
        }
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::RtAudioHdlrEvent
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::RtAudioHdlrEvent(TInt aEvent, TInt aError, TInt aPlayer)
    {
    if (aError == TMS_RESULT_SUCCESS)
        {
        switch (aEvent)
            {
            case ECmdRingToneOpenComplete:
                if (aPlayer != EPlayerBackup && aPlayer != EPlayerTts)
                    {
                    HandleRtAudioInitComplete(aEvent, aError, aPlayer);
                    }
                break;
            case ECmdRingTonePlayComplete:
                HandleRtAudioPlayComplete(aEvent, aError, aPlayer);
                break;
            default:
                //not possible
                break;
            }
        }
     else
         {
         HandleRtAudioError(aEvent, aError, aPlayer);
         }
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::HandleRtAudioError
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::HandleRtAudioError(TInt aEvent, TInt aError,
        TInt aPlayer)
    {
    TRACE_PRN_N1(_L("TMS->HandleRtAudioError [%d]"), aError);

    // Backup player events don't need to be propagated
    if (aPlayer == EPlayerBackup)
        {
        return;
        }
    if (aPlayer == EPlayerTts && aEvent == ECmdRingTonePlayComplete)
        {
        // Remove TTS and keep playing RT
        DeletePlayerAsync(EPlayerTts);
        return;
        }
    const TBool deleteAll = (aError == KErrServerTerminated);
    if (deleteAll)
        {
        // KErrServerTerminated is considered fatal error - delete all.
        for (TInt i = EPlayerFirst; i <= EPlayerLast; i++)
            {
            DeletePlayerAsync(static_cast<TRtPlayerType> (i));
            }
        // Recreate backup player
        TRAPD(ignore, CreateBkpRtPlayerL());
        if (ignore) ;
        }

    iTimer->CancelNotify();
    DoHandlePlayerError(!deleteAll, EFalse);
    iRtObserver.RtPlayerEvent(aEvent, aError);
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::HandleRtAudioInitComplete
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::HandleRtAudioInitComplete(TInt aEvent, TInt aError,
        TInt aPlayer)
    {
    if ((iTonePlayingStatus == EAudioTonePlaying && aPlayer == EPlayerAudio) ||
            (iTonePlayingStatus == EDefaultTonePlaying &&
            aPlayer == EPlayerDefault))
        {
        iTimer->CancelNotify();
        }
    else
        {
        iRtObserver.RtPlayerEvent(aEvent, aError);
        }
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::HandleRtAudioPlayComplete
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::HandleRtAudioPlayComplete(TInt aEvent, TInt aError,
        TInt aPlayer)
    {
    TRACE_PRN_N2(_L("TMS->HandleRtAudioPlayComplete: player=[%d], err=[%d]"),
            aPlayer, aError);

    if (aPlayer == EPlayerTts)
        {
        ResumeTTS();
        return;
        }
    // RT playback has completed: ring once and TTS is activated
    else if (iRtParam.iRingingType == EProfileRingingTypeRingingOnce &&
            iTtsToneToBePlayed)
        {
        // Ring once and TTS
        if (iTtsDelayIndex < iTtsDelaysCount)
            {
            ReStartRingTonePlayer();
            return;
            }
        }
    // RT has completed: ringing and TTS is activated.
    // We need to restart RT and new TTS iterations are required.
    else if (iRtParam.iRingingType == EProfileRingingTypeRinging &&
            iTtsToneToBePlayed)
        {
        // Ringing and TTS
        if (iTtsDelayIndex == iTtsDelaysCount)
            {
            // Ringingtype is "Ringing". New TTS iterations are required.
            // Need to restart TTS sequence TTS has completed its iterations,
            // set index to zero.
            iTtsDelayIndex = 0;
            if (!iTtsDelayTimer->IsRunning())
                {
                // Restart TTS sequence
                iTtsDelayTimer->NotifyAfter(KTMSTtsDelays[iTtsDelayIndex],
                        TCallBack(HandleTtsDelayTimeout, this));
                }
            }

        ReStartRingTonePlayer();
        return;
        }

    DeletePlayer();
    iTonePlayingStatus = EIdle;
    iRtObserver.RtPlayerEvent(aEvent, aError);
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::ResumeTTS
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::ResumeTTS()
    {
    // TTS playing complete, normal tone player should volume up.
    if ((iTtsToneToBePlayed) && (++iTtsDelayIndex < iTtsDelaysCount))
        {
        // Resuming after TTS first time
        // Caller name is said once. Increase tone player volume.
        SolveNewVolumeAndRamptime(ESaidOnce);

        if (!iTtsDelayTimer->IsRunning())
            {
            // There are more TTS iterations to be played.
            iTtsDelayTimer->NotifyAfter(KTMSTtsDelays[iTtsDelayIndex],
                    TCallBack(HandleTtsDelayTimeout, this));
            }
        }
    else
        {
        // Resuming after TTS second time.
        SolveNewVolumeAndRamptime(ESaidTwice);
        }
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::TimerEvent
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::TimerEvent()
    {
    DoHandlePlayerError(ETrue, ETrue);
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::DoHandlePlayerError
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::DoHandlePlayerError(TBool aDelete, TBool aSync)
    {
    switch (iTonePlayingStatus)
        {
        case EAudioTonePlaying:
            {
            if (aDelete)
                {
                if (aSync)
                    {
                    delete iAudioPlayer;
                    }
                else
                    {
                    DeletePlayerAsync(EPlayerAudio);
                    }
                iAudioPlayer = NULL;
                }

#ifdef __WINSCW__
            TRAP_IGNORE(PlayAudioRingToneL());
            iTonePlayingStatus = EDefaultTonePlaying;
#else
            TRAP_IGNORE(PlayDefaultToneL());
#endif //__WINSCW__
            break;
            }
        case EDefaultTonePlaying:
            {
            if (aDelete)
                {
                if (aSync)
                    {
                    delete iDefaultPlayer;
                    }
                else
                    {
                    DeletePlayerAsync(EPlayerDefault);
                    }
                iDefaultPlayer = NULL;
                }

            TRAPD(ignore, PlayBackupToneL());
            if (ignore) ;
            break;
            }
        case ESilentTonePlaying:
        case EBeepOnce:
        case EBackupTonePlaying:
        case EIdle:
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::ConvertRingingType
// -----------------------------------------------------------------------------
//
TMSRtAudioHdlr::TRingingType TMSRingTonePlayer::ConvertRingingType(
        TProfileRingingType aRingingType)
    {
    TMSRtAudioHdlr::TRingingType ringingType = TMSRtAudioHdlr::ETypeRinging;
    switch (aRingingType)
        {
        case EProfileRingingTypeRinging:
            ringingType = TMSRtAudioHdlr::ETypeRinging;
            break;
        case EProfileRingingTypeAscending:
            ringingType = TMSRtAudioHdlr::ETypeAscending;
            break;
        case EProfileRingingTypeRingingOnce:
            ringingType = TMSRtAudioHdlr::ETypeRingingOnce;
            break;
        default:
            break;
        }
    return ringingType;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::ExtendedSecurity
// -----------------------------------------------------------------------------
//
TBool TMSRingTonePlayer::ExtendedSecurity() const
    {
    return iExtSecNeeded;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::SolveNewVolumeAndRamptime
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::SolveNewVolumeAndRamptime(TTtsStatus aStatus)
    {
    TMSRtAudioHdlr* currPlayer = NULL;
    currPlayer = GetCurrentlyActiveAudioPlayerWithTTS();
    if (!currPlayer)
        {
        return;
        }

    TInt ramptime(0);

    // Handle TTS volume. TTS sequence is as follows:
    // 3 sec, tone + name + 4 sec, tone + name + the tone until the end.
    switch (aStatus)
        {
        case ESaidOnce:
            {
            if (iTtsRingingType == EProfileRingingTypeAscending)
                {
                // The ramptime could be (4)* KTMSTtsAscendingStep but now
                // we are setting this to 0 because when caller's name is played
                // for the first time: play ringing tone on level 3 for 4 secs.
                ramptime = 0;
                // Played once ascending
                currPlayer->SetNewVolumeAndRamptime(
                        KPlayerVolumeAscendingRepeat, ramptime);
                }
            else
                {
                // Normal ringtone case. Adjust volume back to profile level.
                // Played once normal
                currPlayer->SetNewVolumeAndRamptime(iTtsVolume, 0);
                }
            break;
            }
        case ESaidTwice:
            {
            if (iTtsRingingType == EProfileRingingTypeAscending)
                {
                TInt vol = iTtsVolume;
                if (vol > KPlayerVolumeAscendingRepeat)
                    {
                    vol = vol - KTtsVolumeAscendingDecrease;
                    }

                // TTS playing complete for the second time. Increase volume.
                ramptime = (vol) * KTMSTtsAscendingStep;
                // Played twice ascending
                currPlayer->SetNewVolumeAndRamptime(iTtsVolume, ramptime);
                }
            else
                {
                // Normal ringtone case. Adjust volume back to profile level.
                // Played twice normal
                currPlayer->SetNewVolumeAndRamptime(iTtsVolume, 0);
                }
            break;
            }
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::PlayerType
// -----------------------------------------------------------------------------
//
TRtPlayerType TMSRingTonePlayer::PlayerType() const
    {
    return iRtParam.iType;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::GetCurrentlyActiveAudioPlayerWithTTS
// -----------------------------------------------------------------------------
//
TMSRtAudioHdlr* TMSRingTonePlayer::GetCurrentlyActiveAudioPlayerWithTTS()
    {
    TMSRtAudioHdlr* player = NULL;

    if (iAudioPlayer)
        {
        player = iAudioPlayer;
        }
    else if (iDefaultPlayer)
        {
        player = iDefaultPlayer;
        }
    else if (iBackupPlayer)
        {
        player = iBackupPlayer;
        }
    return player;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::ReStartRingTonePlayer
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::ReStartRingTonePlayer()
    {
    // Find active player which plays along with TTS.
    TMSRtAudioHdlr* currPlayer = NULL;
    currPlayer = GetCurrentlyActiveAudioPlayerWithTTS();
    if (currPlayer)
        {
        currPlayer->ReStartPlaying();
        }
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::CheckAndHandleToneSizeLimit
// -----------------------------------------------------------------------------
//
TBool TMSRingTonePlayer::CheckAndHandleToneSizeLimit(
        TMSRtContainer* aRingingTone)
    {
    TBool bValidSize = ETrue;

    // If RT size is too large, will play default tone instead.
    if (iToneFileSizeLimitKB)
        {
        if (CheckToneFileSize(aRingingTone->FileName(), iToneFileSizeLimitKB)
                != TMS_RESULT_SUCCESS)
            {
            bValidSize = EFalse;
            }
        }
    return bValidSize;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::CheckToneFileSize
// -----------------------------------------------------------------------------
//
TInt TMSRingTonePlayer::CheckToneFileSize(const TDesC& aFile, TInt aSizeLimitKB)
    {
    // Get file size
    TInt size = 0;
    TInt error = TMS_RESULT_SUCCESS;

    RFs fs;
    error = fs.Connect();

    if (error == TMS_RESULT_SUCCESS)
        {
        TEntry entry;
        if (TMS_RESULT_SUCCESS == fs.Entry(aFile, entry))
            {
            size = entry.iSize;
            }
        }
    fs.Close();

    aSizeLimitKB *= Kkilo;
    if (aSizeLimitKB && size > aSizeLimitKB)
        {
        error = KErrTooBig;
        }

    TRACE_PRN_N1(_L("TMS->CheckToneFileSize: [%d]"), size);
    return error;
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::GetMaxToneFileSizeL
// -----------------------------------------------------------------------------
//
void TMSRingTonePlayer::GetMaxToneFileSizeL(TInt& /*aMaxSizeKB*/) const
    {/*
     const TInt error = CPhoneCenRepProxy::Instance()->GetInt(
     KCRUidProfileEngine, KProEngRingingToneMaxSize, aMaxSizeKB);

     if (error != TMS_RESULT_SUCCESS)
     {
     aMaxSizeKB = 0;
     }
     if (aMaxSizeKB < 0)
     {
     aMaxSizeKB = 0;
     }
     */
    //TRACE_PRN_N1("TMS->GetMaxToneFileSizeL: [%d]", aMaxSizeKB);
    }

// -----------------------------------------------------------------------------
// TMSRingTonePlayer::ActiveAudioPlayer
// -----------------------------------------------------------------------------
//
TMSRtAudioHdlr* TMSRingTonePlayer::ActiveAudioPlayer()
    {
    TMSRtAudioHdlr* audioHdlr = NULL;

    switch (iTonePlayingStatus)
        {
        case EAudioTonePlaying:
            {
            if (iAudioPlayer != NULL)
                {
                audioHdlr = iAudioPlayer;
                }
            break;
            }
        case EDefaultTonePlaying:
            {
            if (iDefaultPlayer != NULL)
                {
                audioHdlr = iDefaultPlayer;
                }
            break;
            }
        case EBeepOnce:
            {
            if (iBeepOncePlayer != NULL)
                {
                audioHdlr = iBeepOncePlayer;
                }
            break;
            }
        case ESequenceTonePlaying:
            {
            if (iSequencePlayer != NULL)
                {
                audioHdlr = iSequencePlayer;
                }
            break;
            }
         case EBackupTonePlaying:
             {
            if (iBackupPlayer != NULL)
                {
                audioHdlr = iBackupPlayer;
                }
            break;
             }
        case ESilentTonePlaying:
            {
            if (iSilentPlayer != NULL)
                {
                audioHdlr = iSilentPlayer;
                }
            break;
            }
        case EUnsecureVoIPTonePlaying:
            {
            if (iUnsecureVoIPTonePlayer != NULL)
                {
                audioHdlr = iUnsecureVoIPTonePlayer;
                }
            break;
            }
        default:
            break;
        }

    return audioHdlr;
    }

