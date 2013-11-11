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

#include "tmsaudiotoneutilityimpl.h"

using namespace TMS;

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
TMSAudioToneUtilityImpl::TMSAudioToneUtilityImpl()
    {
    iToneUtility = NULL;
    }

// ---------------------------------------------------------------------------
// 2nd phase construcotor
// ---------------------------------------------------------------------------
//
void TMSAudioToneUtilityImpl::ConstructL(MMdaAudioToneObserver& toneObserver)
    {
    iToneUtility = CMdaAudioToneUtility::NewL(toneObserver);
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
TMSAudioToneUtilityImpl* TMSAudioToneUtilityImpl::NewL(
        MMdaAudioToneObserver& toneObserver)
    {
    TMSAudioToneUtilityImpl* self = new (ELeave) TMSAudioToneUtilityImpl;
    CleanupStack::PushL(self);
    self->ConstructL(toneObserver);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
TMSAudioToneUtilityImpl::~TMSAudioToneUtilityImpl()
    {
    delete iToneUtility;
    }

// ---------------------------------------------------------------------------
// From class TMSAudioToneUtilityImpl
// ---------------------------------------------------------------------------
//
void TMSAudioToneUtilityImpl::PrepareToPlayDesSequence(const TDesC8& aSequence)
    {
    iToneUtility->PrepareToPlayDesSequence(aSequence);
    }

// ---------------------------------------------------------------------------
// From class TMSAudioToneUtilityImpl
// ---------------------------------------------------------------------------
//
TMdaAudioToneUtilityState TMSAudioToneUtilityImpl::State()
    {
    return iToneUtility->State();
    }

// ---------------------------------------------------------------------------
// From class TMSAudioToneUtilityImpl
// ---------------------------------------------------------------------------
//
void TMSAudioToneUtilityImpl::CancelPlay()
    {
    // Do state specific cancellations, this way we do not end up in a
    // endless play state if CancelPlay() is called during preparation.
    if (iToneUtility)
        {
        if (EMdaAudioToneUtilityPlaying == State())
            {
            iToneUtility->CancelPlay();
            }
        else if (EMdaAudioToneUtilityPrepared == State())
            {
            iToneUtility->CancelPrepare();
            }
        }
    }

// ---------------------------------------------------------------------------
// From class TMSAudioToneUtilityImpl
// ---------------------------------------------------------------------------
//
gint TMSAudioToneUtilityImpl::MaxVolume()
    {
    return iToneUtility->MaxVolume();
    }

// ---------------------------------------------------------------------------
// From class TMSAudioToneUtilityImpl
// ---------------------------------------------------------------------------
//
void TMSAudioToneUtilityImpl::SetVolume(gint volume)
    {
    iToneUtility->SetVolume(volume);
    }

// ---------------------------------------------------------------------------
// From class TMSAudioToneUtilityImpl
// ---------------------------------------------------------------------------
//
void TMSAudioToneUtilityImpl::SetRepeats(gint repeatNumberOfTimes,
        const TTimeIntervalMicroSeconds& trailingSilence)
    {
    iToneUtility->SetRepeats(repeatNumberOfTimes, trailingSilence);
    }

// ---------------------------------------------------------------------------
// From class TMSAudioToneUtilityImpl
// ---------------------------------------------------------------------------
//
void TMSAudioToneUtilityImpl::SetPriority(gint priority,
        TMdaPriorityPreference pref)
    {
    iToneUtility->SetPriority(priority, pref);
    }

// ---------------------------------------------------------------------------
// From class TMSAudioToneUtilityImpl
// ---------------------------------------------------------------------------
//
void TMSAudioToneUtilityImpl::SetVolumeRamp(
        const TTimeIntervalMicroSeconds& rampDuration)
    {
    iToneUtility->SetVolumeRamp(rampDuration);
    }

// ---------------------------------------------------------------------------
// From class TMSAudioToneUtilityImpl
// ---------------------------------------------------------------------------
//
void TMSAudioToneUtilityImpl::Play()
    {
    iToneUtility->Play();
    }

// ---------------------------------------------------------------------------
// From class TMSAudioToneUtilityImpl
// ---------------------------------------------------------------------------
//
void TMSAudioToneUtilityImpl::CancelPrepare()
    {
    iToneUtility->CancelPrepare();
    }

