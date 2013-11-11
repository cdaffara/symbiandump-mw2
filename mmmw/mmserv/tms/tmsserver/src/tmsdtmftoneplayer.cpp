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
#include "tmsutility.h"
#include "tmsdtmftoneplayer.h"

// CONSTANTS
const gint KDTMFMaxBufSize = 64;
const gint KMaxVolumeLevel = 10;

_LIT(KPhoneTone_Asterisk, "*+w");

//from voip
const guint KDTMFToneLengthOn = 250000;
const guint KDTMFToneLengthOff = 50000;
const guint KDTMFToneLengthPause = 2500000;

using namespace TMS;

// -----------------------------------------------------------------------------
// TMSAudioDtmfTonePlayer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
TMSAudioDtmfTonePlayer* TMSAudioDtmfTonePlayer::NewL(
        TMSDTMFTonePlayerObserver& obsrvr, guint audioPreference,
        guint audioPriority)
    {
    TMSAudioDtmfTonePlayer* self = new (ELeave) TMSAudioDtmfTonePlayer(obsrvr,
            audioPreference, audioPriority);

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

//Destructor
TMSAudioDtmfTonePlayer::~TMSAudioDtmfTonePlayer()
    {
    TRACE_PRN_FN_ENT;
    Cancel();
    delete iDTMFPlayer;
    // Release memory and close handles to unused plug-ins held by the DevSound
    REComSession::FinalClose();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioDtmfTonePlayer::TMSAudioDtmfTonePlayer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TMSAudioDtmfTonePlayer::TMSAudioDtmfTonePlayer(
        TMSDTMFTonePlayerObserver& obsrvr, guint audioPreference,
        guint audioPriority) :
    iObserver(obsrvr),
    iPref(audioPreference),
    iPrior(audioPriority)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioDtmfTonePlayer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void TMSAudioDtmfTonePlayer::ConstructL()
    {
    TRACE_PRN_FN_ENT;

    TMMFPrioritySettings dtmfClickPlayerPrioritySettings;
    iDTMFPlayer = CMMFDevSound::NewL();
    iDTMFPlayer->InitializeL(*this, EMMFStateTonePlaying);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioDtmfTonePlayer::SetVolume
// Sets the volume level that is used when the dtmf tone is played.
// -----------------------------------------------------------------------------
//
void TMSAudioDtmfTonePlayer::SetVolume(gint volume)
    {
    TRACE_PRN_FN_ENT;
    gint vol = ConvertVolumeLevel(volume);

    if (vol != iVolume)
        {
        iVolume = vol;
        iDTMFPlayer->SetVolume(iVolume);
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioDtmfTonePlayer::PlayDtmfTone
// Searches the given DTMF tone from iToneArray and calls PlayCurrentTone
// member function.
// -----------------------------------------------------------------------------
//
void TMSAudioDtmfTonePlayer::PlayDtmfTone(TDes& tone)
    {
    TRACE_PRN_FN_ENT;
    Cancel();

    // DTMF signaling.
    if (iDTMFPlayer)
        {
        TBuf<KDTMFMaxBufSize> key;
        key.Append(tone);
        TRAP_IGNORE(iDTMFPlayer->PlayDTMFStringL(tone));
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioDtmfTonePlayer::Cancel
// Cancels the currently playing tone.
// -----------------------------------------------------------------------------
//
void TMSAudioDtmfTonePlayer::Cancel()
    {
    TRACE_PRN_FN_ENT;
    iDTMFPlayer->Stop();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioDtmfTonePlayer::Normalize
// Lowers case for chars and if its '*','+', or 'w' sets it to asterisk
// char ('*').
// -----------------------------------------------------------------------------
//
void TMSAudioDtmfTonePlayer::Normalize(TChar& tone)
    {
    TRACE_PRN_FN_ENT;
    tone.LowerCase();
    TPtrC ast(KPhoneTone_Asterisk);

    if (ast.Locate(tone) != KErrNotFound)
        {
        tone = ast[0];
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioDtmfTonePlayer::InitializeComplete
// -----------------------------------------------------------------------------
//
void TMSAudioDtmfTonePlayer::InitializeComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;

    if (aError == KErrNone)
        {
        TMMFPrioritySettings dtmfPlayerSettings;
        dtmfPlayerSettings.iPref = iPref;
        dtmfPlayerSettings.iPriority = iPrior;
        dtmfPlayerSettings.iState = EMMFStateTonePlaying;
        iDTMFPlayer->SetPrioritySettings(dtmfPlayerSettings);

        TTimeIntervalMicroSeconds32 length(KDTMFToneLengthOn);
        TTimeIntervalMicroSeconds32 pause(KDTMFToneLengthOff);
        TTimeIntervalMicroSeconds32 zero(KDTMFToneLengthPause);
        iDTMFPlayer->SetDTMFLengths(length, pause, zero);
        iDTMFPlayer->SetVolume(iDTMFPlayer->MaxVolume());
        }

    iObserver.DTMFInitCompleted(aError);
    TRACE_PRN_IF_ERR(aError);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioDtmfTonePlayer::ToneFinished
// -----------------------------------------------------------------------------
//
void TMSAudioDtmfTonePlayer::ToneFinished(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    iObserver.DTMFToneFinished(aError);
    TRACE_PRN_IF_ERR(aError);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioDtmfTonePlayer::ConvertVolumeLevel
// -----------------------------------------------------------------------------
//
gint TMSAudioDtmfTonePlayer::ConvertVolumeLevel(gint volume)
    {
    TRACE_PRN_FN_ENT;
    gint result = iDTMFPlayer->MaxVolume() * volume / KMaxVolumeLevel;
    TRACE_PRN_FN_EXT;
    return result;
    }

