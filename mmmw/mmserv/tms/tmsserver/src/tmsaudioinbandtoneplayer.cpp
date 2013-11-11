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

//  INCLUDE FILES

#include <tms.h>
#include <AudioPreference.h>
#include <data_caging_path_literals.hrh>
#include <defaultbeep.rsg>
#include "tmsutility.h"
#include "tmsaudioinbandtoneplayer.h"
#include "tmsaudiotoneutilityimpl.h"

using namespace TMS;

// CONSTANTS
const gint KPhoneInbandVolumeMax = 10;
const gint KPhoneInbandVolumeMin = 0;

// Resourcefile destination.
_LIT(KSystemDefaultbeepRscDrive, "Z:");
_LIT(KSystemDefaultbeepRscFile, "DEFAULTBEEP.rSC");

// -----------------------------------------------------------------------------
// TMSAudioInbandTonePlayer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
TMSAudioInbandTonePlayer* TMSAudioInbandTonePlayer::NewL()
    {
    TMSAudioInbandTonePlayer* self = new (ELeave) TMSAudioInbandTonePlayer();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

//Destructor
TMSAudioInbandTonePlayer::~TMSAudioInbandTonePlayer()
    {
    TRACE_PRN_FN_ENT;
    iResourceFile.Close();
    iFsSession.Close();
    iTones.ResetAndDestroy();
    iTones.Close();
    if (iPlayer)
        {
        iPlayer->CancelPlay();
        }
    delete iPlayer;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioInbandTonePlayer::TMSAudioInbandTonePlayer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TMSAudioInbandTonePlayer::TMSAudioInbandTonePlayer() :
    iPlayer(NULL)
    {
    }

// -----------------------------------------------------------------------------
// TMSAudioInbandTonePlayer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void TMSAudioInbandTonePlayer::ConstructL()
    {
    TRACE_PRN_FN_ENT;
    iPlayer = TMSAudioToneUtilityImpl::NewL(*this);
    iTones.Reset();
    CreateToneSequencesL();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioInbandTonePlayer::CreateSequencesL
// Create inband tone sequences from resources.
// -----------------------------------------------------------------------------
//
void TMSAudioInbandTonePlayer::CreateToneSequencesL()
    {
    TRACE_PRN_FN_ENT;

    iFileName = KSystemDefaultbeepRscDrive;
    iFileName.Append(KDC_RESOURCE_FILES_DIR);
    iFileName.Append(KSystemDefaultbeepRscFile);
    iFsSession.Connect();
    iResourceFile.OpenL(iFsSession, iFileName);

    // DO NOT change order of sequence allocation!
    AllocSeqFromResourceL(R_NET_BUSY);                 //iTones[0]
    AllocSeqFromResourceL(R_NET_RADIO_NOT_AVAILABLE);  //iTones[1]
    AllocSeqFromResourceL(R_NET_CONGESTION);           //iTones[2]
    AllocSeqFromResourceL(R_NET_SPECIAL_INFORMATION);  //iTones[3]
    AllocSeqFromResourceL(R_NET_REORDER);              //iTones[4]
    AllocSeqFromResourceL(R_NET_RING_GOING);           //iTones[5]
    AllocSeqFromResourceL(R_NET_CALL_WAITING);         //iTones[6]
    AllocSeqFromResourceL(R_DATA_CALL_TONE);           //iTones[7]
    AllocSeqFromResourceL(R_NO_SOUND_SEQUENCE);        //iTones[8]
    AllocSeqFromResourceL(R_BEEP_SEQUENCE);            //iTones[9]

    iResourceFile.Close();
    iFsSession.Close();

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioInbandTonePlayer::AllocSeqFromResourceL
//
// -----------------------------------------------------------------------------
//
void TMSAudioInbandTonePlayer::AllocSeqFromResourceL(const gint resource)
    {
    HBufC8* res = iResourceFile.AllocReadLC(resource);
    iReader.SetBuffer(res);
    gint length = iReader.ReadInt16();
    HBufC8* buf = HBufC8::NewL(length);
    TPtr8 ptr(buf->Des());
    for (gint i = 0; i < length; i++)
        {
        ptr.Append(iReader.ReadUint16());
        }
    User::LeaveIfError(iTones.Append(buf));
    CleanupStack::PopAndDestroy(res);
    }

// -----------------------------------------------------------------------------
// TMSAudioInbandTonePlayer::PlayInbandTone
// Searches the given Inband tone from iToneArray and calls PlayCurrentTone.
// -----------------------------------------------------------------------------
//
void TMSAudioInbandTonePlayer::PlayInbandTone(TMSInbandToneType tone)
    {
    TRACE_PRN_FN_ENT;

    // First stop any ongoing playback
    Cancel();
    UpdateTonePlayerVolume();
    iToneName = tone;

    if (iPlayer && (tone >= 0 && tone < iTones.Count()))
        {
        iPlayer->PrepareToPlayDesSequence(iTones[tone]->Des());
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioInbandTonePlayer::Cancel
// Cancels the currently playing tone.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TMSAudioInbandTonePlayer::Cancel()
    {
    TRACE_PRN_FN_ENT;

    if (iPlayer)
        {
        if (iPlayer->State() == EMdaAudioToneUtilityNotReady)
            {
            // Prepare is called, but toneplayer's state is not yet prepare,
            // then cancel to prepare is needed.
            iPlayer->CancelPrepare();
            }
        else
            {
            iPlayer->CancelPlay();
            }
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioInbandTonePlayer::PlayCurrentTone
// Cancels the currently playing tone.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TMSAudioInbandTonePlayer::PlayCurrentTone()
    {
    TRACE_PRN_FN_ENT;
    if (iPlayer)
        {
        if (iPlayer->State() == EMdaAudioToneUtilityPrepared)
            {
            UpdateTonePlayerVolume();

            switch (iToneName)
                {
                case TMS_INBAND_USER_BUSY:
                    SetToneAttributes(KAudioPrefBusy);
                    break;
                case TMS_INBAND_RADIO_PATH_NOT_AVAIL:
                    SetToneAttributes(KAudioPrefRadioNotAvailable);
                    break;
                case TMS_INBAND_CONGESTION:
                    SetToneAttributes(KAudioPrefCongestion);
                    break;
                case TMS_INBAND_SPECIAL_INFO:
                    SetToneAttributes(KAudioPrefSpecialInformation);
                    break;
                case TMS_INBAND_REORDER:
                    SetToneAttributes(KAudioPrefReorder);
                    break;
                case TMS_INBAND_REMOTE_ALEARTING:
                    SetToneAttributes(KAudioPrefRingGoing);
                    break;
                case TMS_INBAND_CALL_WAITING:
                    SetToneAttributes(KAudioPrefCallWaiting,
                            KAudioPriorityNetMsg, 0);
                    break;
                case TMS_INBAND_DATA_CALL:
                    SetToneAttributes(KAudioPrefIncomingDataCall,
                            KAudioPriorityPhoneCall);

            //      if ( iRingingType == EProfileRingingTypeAscending )
            //           {
            //           iPlayer->SetVolumeRamp(
            //               TTimeIntervalMicroSeconds( KPERingingToneRampDuration ) );
            //           }
            //       else
            //          {
            //          EProfileRingingTypeRinging, EProfileRingingTypeRingingOnce
                    iPlayer->SetVolumeRamp(TTimeIntervalMicroSeconds(
                            KPhoneInbandToneZero));
            //           }
            //      if ( iRingingType == EProfileRingingTypeRingingOnce )
            //          {
            //        iPlayer->SetRepeats(0, TTimeIntervalMicroSeconds(
            //                KPhoneInbandToneZero));
                    //          }
                    break;
                case TMS_INBAND_NO_SEQUENCE:
                case TMS_INBAND_BEEP_SEQUENCE:
                    SetToneAttributes(KAudioPrefIncomingCall,
                            KAudioPriorityPhoneCall);

                    //iPlayer->SetRepeats(0, TTimeIntervalMicroSeconds(
                    //        KPhoneInbandToneZero));
                    iPlayer->SetVolumeRamp(TTimeIntervalMicroSeconds(
                            KPhoneInbandToneZero));
                    break;
                default:
                    break;
                }
#ifndef __WINSCW__
            //Play the tone
            iPlayer->Play();
#endif
            }
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioInbandTonePlayer::SetToneAttributes
//
// -----------------------------------------------------------------------------
//
void TMSAudioInbandTonePlayer::SetToneAttributes(const guint pref,
        const guint priority, const gint repeatTimes, const gint trailSilence)
    {
    iPlayer->SetRepeats(repeatTimes, TTimeIntervalMicroSeconds(trailSilence));
    iPlayer->SetPriority(priority, static_cast<TMdaPriorityPreference> (pref));
    }

// -----------------------------------------------------------------------------
// TMSAudioInbandTonePlayer::SetVolume
// -----------------------------------------------------------------------------
//
void TMSAudioInbandTonePlayer::SetVolume(gint volume)
    {
    TRACE_PRN_FN_ENT;
    gint safeVolume = Max(Min(volume, KPhoneInbandVolumeMax),
            KPhoneInbandVolumeMin);

    if (iPlayer)
        {
        gint mediaServerVolume = CalculateMediaServerVolume(safeVolume);
        if (EMdaAudioToneUtilityNotReady != iPlayer->State())
            {
            iPlayer->SetVolume(mediaServerVolume);
            }
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioInbandTonePlayer::MatoPrepareComplete
// Updates flag values and plays current inbandtone.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TMSAudioInbandTonePlayer::MatoPrepareComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    if (aError == KErrNone)
        {
        PlayCurrentTone();
        }
    TRACE_PRN_IF_ERR(aError);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioInbandTonePlayer::MatoPlayComplete
// -
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TMSAudioInbandTonePlayer::MatoPlayComplete(
#ifdef _DEBUG
        TInt aError)
#else
        TInt /*aError*/)
#endif
    {
    TRACE_PRN_FN_ENT;
    // TODO: process error?
#ifdef _DEBUG
    TRACE_PRN_IF_ERR(aError);
#endif
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSAudioInbandTonePlayer::CalculateMediaServerVolume
// -----------------------------------------------------------------------------
//
gint TMSAudioInbandTonePlayer::CalculateMediaServerVolume(gint volume) const
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    // Our volume level scale is 0...10, media server's can be anything.
    // This scales the volume to correct level.
    return (iPlayer->MaxVolume() * volume) /
            (KPhoneInbandVolumeMax - KPhoneInbandVolumeMin);
    }

// -----------------------------------------------------------------------------
// TMSAudioInbandTonePlayer::UpdateTonePlayerVolume
// -----------------------------------------------------------------------------
//
void TMSAudioInbandTonePlayer::UpdateTonePlayerVolume()
    {
    TRACE_PRN_FN_ENT;
    gint volume( /*KPhoneInbandVolumeMin*/KPhoneInbandVolumeMax);
    SetVolume(volume);
    TRACE_PRN_FN_EXT;
    }

