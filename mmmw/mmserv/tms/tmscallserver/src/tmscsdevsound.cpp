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

#include <AudioPreference.h>
#include <tms.h>
#include <S60FourCC.h>
#include "tmsutility.h"
#include "tmscsdevsound.h"
#include "tmscsdevsoundobserver.h"

using namespace TMS;

// CONSTANTS
const gint KTimeoutInitial = 250000; // 250 ms initial timeout
const gint KTimeoutMultiplier = 2;   // Double the timeout for each retry
const gint KPeriodicTimeoutMax = 2000000; // 2 sec max periodic timeout
const gint KMicroSecMultiply = 1000000;   // 1 sec

// -----------------------------------------------------------------------------
// TMSCSDevSound
// -----------------------------------------------------------------------------
//
TMSCSDevSound::TMSCSDevSound(TMSCSDevSoundObserver& observer) :
    iObserver(observer)
    {
    iTimer = NULL;
    iPeriodic = KTimeoutInitial;
    iElapsedTime = 0;
    iInitRetryTime = 0;
    iStartRetryTime = 0;
    }

// -----------------------------------------------------------------------------
// ConstructL
// -----------------------------------------------------------------------------
//
void TMSCSDevSound::ConstructL(const TMSStreamType strmtype,
        const gint retrytime)
    {
    TRACE_PRN_FN_ENT;
    iInitRetryTime = retrytime;
    iStreamType = strmtype;

    if (strmtype == TMS_STREAM_UPLINK)
        {
        iMode = EMMFStateRecording;
        iPriority = KAudioPriorityCSCallUplink;
        iPreference = KAudioPrefCSCallUplink;
        }
    else if (strmtype == TMS_STREAM_DOWNLINK)
        {
        iMode = EMMFStatePlaying;
        iPriority = KAudioPriorityCSCallDownlink;
        iPreference = KAudioPrefCSCallDownlink;
        }

    iTimer = TMSTimer::NewL();
    InitializeL();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// InitializeL
// -----------------------------------------------------------------------------
//
void TMSCSDevSound::InitializeL()
    {
    TRACE_PRN_FN_ENT;
    TMMFPrioritySettings audioPriority;
    TFourCC modemFourCC;
    modemFourCC.Set(KS60FourCCCodeModem);

    delete iDevSound;
    iDevSound = NULL;
    iDevSound = CMMFDevSound::NewL();
    if (iDevSound)
        {
#ifndef __WINSCW__
        iDevSound->InitializeL(*this, modemFourCC, iMode);
#else
        //For testing TMS in WINSCW
        iDevSound->InitializeL(*this, KMMFFourCCCodePCM16, iMode);
#endif
        audioPriority.iPriority = iPriority;
        audioPriority.iPref = iPreference;
        audioPriority.iState = iMode;
        iDevSound->SetPrioritySettings(audioPriority);
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// From class MDevSoundObserver
// Not implemented
// -----------------------------------------------------------------------------
//
TMSCSDevSound::~TMSCSDevSound()
    {
    TRACE_PRN_FN_ENT;
    CancelTimer();
    delete iTimer;
    delete iDevSound;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// Tries to activate the audio stream if not already active or activating
// -----------------------------------------------------------------------------
//
void TMSCSDevSound::Activate(const gint retrytime)
    {
    iStartRetryTime = retrytime;

    if (!iActive && !iActivationOngoing)
        {
        iActivationOngoing = ETrue;
        TRAP_IGNORE(DoActivateL());
        }
    }

// -----------------------------------------------------------------------------
// Deactivates the audio device.
// -----------------------------------------------------------------------------
//
void TMSCSDevSound::Deactivate(gboolean reset)
    {
    TRACE_PRN_FN_ENT;
    if (reset)
        {
        iPeriodic = KTimeoutInitial;
        }
    CancelTimer();
    if (iDevSound && (iActive || iActivationOngoing))
        {
        iDevSound->Stop();
        iActive = EFalse;
        iActivationOngoing = EFalse;
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// DevSound
// -----------------------------------------------------------------------------
//
CMMFDevSound& TMSCSDevSound::DevSound()
    {
    return *iDevSound;
    }

// -----------------------------------------------------------------------------
// From class MDevSoundObserver
// -----------------------------------------------------------------------------
//
void TMSCSDevSound::InitializeComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    if (aError != TMS_RESULT_SUCCESS && iInitRetryTime != 0)
        {
        StartTimer();
        }
    else
        {
        iPeriodic = KTimeoutInitial;
        CancelTimer();
        NotifyEvent(aError);
        }
    TRACE_PRN_IF_ERR(aError);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCSDevSound::NotifyEvent
// -----------------------------------------------------------------------------
//
void TMSCSDevSound::NotifyEvent(gint error)
    {
    if (iStreamType == TMS_STREAM_DOWNLINK)
        {
        iObserver.DownlinkInitCompleted(error);
        }
    else if (iStreamType == TMS_STREAM_UPLINK)
        {
        iObserver.UplinkInitCompleted(error);
        }
    }

// -----------------------------------------------------------------------------
// TMSCSDevSound::CancelTimer
// Resets timer
// -----------------------------------------------------------------------------
//
void TMSCSDevSound::CancelTimer()
    {
    iInitRetryTime = 0;
    iStartRetryTime = 0;
    iElapsedTime = 0;

    if (iTimer)
        {
        if (iTimer->IsRunning())
            {
            iTimer->CancelNotify();
            }
        }
    }

// -----------------------------------------------------------------------------
// TMSCSDevSound::StartTimer
// Activates timer
// -----------------------------------------------------------------------------
//
void TMSCSDevSound::StartTimer()
    {
    if (iTimer && (iInitRetryTime != 0 || iStartRetryTime != 0))
        {
        iTimer->NotifyAfter(iPeriodic, *this);
        }
    }

// -----------------------------------------------------------------------------
// From TMSTimerObserver
// Notification upon TMSTimer timeout.
// -----------------------------------------------------------------------------
//
void TMSCSDevSound::TimerEvent()
    {
    if (iPeriodic < KPeriodicTimeoutMax)
        {
        iPeriodic *= KTimeoutMultiplier;
        }
    iElapsedTime += iPeriodic;

    if (!iActivationOngoing) //Initializing
        {
        if (iElapsedTime >= (iInitRetryTime * KMicroSecMultiply))
            {
            iInitRetryTime = 0; //timer will not start again
            }
        TRAPD(status, InitializeL());
        if (status != TMS_RESULT_SUCCESS)
            {
            NotifyEvent(status);
            }
        }
    else //Activating
        {
        if (iElapsedTime >= (iStartRetryTime * KMicroSecMultiply))
            {
            iStartRetryTime = 0; //timer will not start again
            }
        Deactivate(FALSE);
        Activate(iStartRetryTime);
        }
    }

//  End of File
