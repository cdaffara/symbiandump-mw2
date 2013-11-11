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
 * Description:  Audio player implementation.
 *
 */

// INCLUDE FILES
#include <e32std.h>
#include <utf.h>    // For converting data for TTS
#include "tmsshared.h"
#include "tmsutility.h"
#include "tmsrtaudiohdlr.h"
#include "tmsrtcontainer.h"
#include "tmsrtaudiohdlrobsrv.h"

using namespace TMS;

// CONSTANTS
const TInt KMinVolumeLevel = 1;
const TInt KMaxVolumeLevel = 10;
const TInt KTMSRingingRepeatsTrailPause = 1000000;
const TInt KTMSAudioAscendingRampDuration = 3000000;
const TInt KTMSMdaAudioToneRepeatForever = -2;
const TInt KUTF8Multiply = 2;

_LIT(KFileListRngMimeType, "application/vnd.nokia.ringing-tone");

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::TMSRtAudioHdlr
// -----------------------------------------------------------------------------
//
TMSRtAudioHdlr::TMSRtAudioHdlr(TAudioDataFormat aDataFormat,
        TMSRtAudioHdlrObsrv& aObserver, TUint aPriority, TUint aPreference,
        TInt aType) :
    iFormat(aDataFormat),
    iObserver(aObserver),
    iPriority(aPriority),
    iPreference(aPreference),
    iRtType(aType)
    {
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::ConstructL
// -----------------------------------------------------------------------------
//
void TMSRtAudioHdlr::ConstructL(const TDesC& aFileName)
    {
    TRACE_PRN_FN_ENT;
    if (iFormat == EFormatTone)
        {
        iTonePlayer = CMdaAudioToneUtility::NewL(*this);
        iTonePlayer->PrepareToPlayFileSequence(aFileName);
        iPlayerStatus = EToneLoading;
        }
    else
        {
        iSamplePlayer = CMdaAudioPlayerUtility::NewFilePlayerL(aFileName, *this,
                iPriority, iPreference);
        iPlayerStatus = EToneLoading;
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::ConstructSeqL
// -----------------------------------------------------------------------------
//
void TMSRtAudioHdlr::ConstructSeqL(const TDesC8& aSequence)
    {
    TRACE_PRN_FN_ENT;
    iTonePlayer = CMdaAudioToneUtility::NewL(*this);
    iSequence = aSequence.AllocL();
    iTonePlayer->PrepareToPlayDesSequence(*iSequence);
    iPlayerStatus = EToneLoading;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::ConstructTtsL
// -----------------------------------------------------------------------------
//
void TMSRtAudioHdlr::ConstructTtsL(const TDesC& aTtsText, TUint aPriority,
        TUint aPreference)
    {
    TRACE_PRN_FN_ENT;
    delete iTtsPlayer;
    iTtsPlayer = NULL;
    iTtsPlayer = CMdaAudioPlayerUtility::NewL(*this, aPriority, aPreference);

    __ASSERT_DEBUG(iTtsPlayer, PANIC(TMS_RESULT_UNINITIALIZED_OBJECT));

    delete iTtsText;
    iTtsText = NULL;
    // UTF-8 strings can take up to 4 bytes per character
    iTtsText = HBufC8::NewL(aTtsText.Length() << KUTF8Multiply);

    __ASSERT_DEBUG(iTtsText, PANIC(TMS_RESULT_UNINITIALIZED_OBJECT));

    TPtr8 refText = iTtsText->Des();
    User::LeaveIfError(CnvUtfConverter::ConvertFromUnicodeToUtf8(refText,
            aTtsText));

    // UTF-8 chars can be up to 4 bytes long, but usually
    // take 1-2 bytes, 3 for asian chars.
    HBufC8* oldText = iTtsText;
    iTtsText = iTtsText->ReAlloc(iTtsText->Length());
    if (!iTtsText)
        {
        // ReAlloc failed, set back to original.
        iTtsText = oldText;
        }
    iTtsPlayer->OpenDesL(*iTtsText);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::NewL
// -----------------------------------------------------------------------------
//
TMSRtAudioHdlr* TMSRtAudioHdlr::NewL(const TMSRtContainer& aRingingTone,
        TUint aPriority, TUint aPreference, TMSRtAudioHdlrObsrv& aObserver,
        TInt aType, TBool aExtSecNeeded)
    {
    TRACE_PRN_FN_ENT;
    // Check file DRM property if extended security is needed.
    if (aExtSecNeeded)
        {
        if (!aRingingTone.IsFileInRom() && !aRingingTone.IsFileDrmProtected())
            {
            // DRM extended security check permission denied
            User::Leave(KErrPermissionDenied);
            }
        // DRM extended security check ok
        }

    // RNG file types have to be played with CMdaAudioToneUtility player.
    // Other audio formats use sample player
    TAudioDataFormat format = (aRingingTone.MimeType().CompareF(
            KFileListRngMimeType) == KErrNone) ? EFormatTone : EFormatSample;

    TMSRtAudioHdlr* self = new (ELeave) TMSRtAudioHdlr(format, aObserver,
            aPriority, aPreference, aType);
    CleanupStack::PushL(self);
    self->ConstructL(aRingingTone.FileName());
    CleanupStack::Pop(self);
    TRACE_PRN_FN_EXT;
    return self;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::NewL
// -----------------------------------------------------------------------------
//
TMSRtAudioHdlr* TMSRtAudioHdlr::NewL(const TDesC& aFileName, TUint aPriority,
        TUint aPreference, TMSRtAudioHdlrObsrv& aObserver, TInt aType,
        TBool aExtSecNeeded)
    {
    TRACE_PRN_FN_ENT;
    TMSRtContainer* tone = TMSRtContainer::NewL(aFileName);
    CleanupStack::PushL(tone);
    TMSRtAudioHdlr* player = TMSRtAudioHdlr::NewL(*tone, aPriority, aPreference,
            aObserver, aType, aExtSecNeeded);
    CleanupStack::PopAndDestroy(tone);
    TRACE_PRN_FN_EXT;
    return player;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::NewSeqL
// -----------------------------------------------------------------------------
//
TMSRtAudioHdlr* TMSRtAudioHdlr::NewSeqL(const TDesC8& aSequence,
        TUint aPriority, TUint aPreference, TMSRtAudioHdlrObsrv& aObserver,
        TInt aType)
    {
    TRACE_PRN_FN_ENT;
    TMSRtAudioHdlr* self = new (ELeave) TMSRtAudioHdlr(EFormatTone,
            aObserver, aPriority, aPreference, aType);
    CleanupStack::PushL(self);
    self->ConstructSeqL(aSequence);
    CleanupStack::Pop(self);
    TRACE_PRN_FN_EXT;
    return self;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::NewTtsL
// -----------------------------------------------------------------------------
//
TMSRtAudioHdlr* TMSRtAudioHdlr::NewTtsL(const TDesC& aText, TUint aPriority,
        TUint aPreference, TMSRtAudioHdlrObsrv& aObserver, TInt aType)
    {
    TRACE_PRN_FN_ENT;
    TMSRtAudioHdlr* self = new (ELeave) TMSRtAudioHdlr(EFormatTts, aObserver,
            aPriority, aPreference, aType);
    CleanupStack::PushL(self);
    self->ConstructTtsL(aText, aPriority, aPreference);
    CleanupStack::Pop(self);
    TRACE_PRN_FN_EXT;
    return self;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::~TMSRtAudioHdlr
// -----------------------------------------------------------------------------
//
TMSRtAudioHdlr::~TMSRtAudioHdlr()
    {
    TRACE_PRN_FN_ENT;
    /*if (i3DPlugin)
         {
         i3DPlugin->Stop();
         }
    delete i3DPlugin;
    delete iAudioOutput;*/
    delete iTonePlayer;
    iTonePlayer = NULL;
    delete iSamplePlayer;
    iSamplePlayer = NULL;
    delete iTtsPlayer;
    iTtsPlayer = NULL;
    delete iSequence;
    iSequence = NULL;
    delete iTtsText;
    iTtsText = NULL;
    REComSession::FinalClose();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::Play
// -----------------------------------------------------------------------------
//
void TMSRtAudioHdlr::Play(TRingingType aRingType, TInt aVolume,
        TInt aTTsToBePlayed)
    {
    TRACE_PRN_FN_ENT;
    iRingType = aRingType;
    iVolume = aVolume;
    iTTsToBePlayed = aTTsToBePlayed;

    if (iFormat == EFormatTone)
        {
        switch (iPlayerStatus)
            {
            case ETonePlaying:
                if (iTonePlayer)
                    {
                    iTonePlayer->CancelPlay();
                    }
                DoPlay();
                break;
            case EToneReady:
                iPlayerStatus = ETonePlaying;
                DoPlay();
                break;
            case EToneLoading:
                iToBePlaying = ETrue;
                break;
            default:
                __ASSERT_DEBUG(EFalse,
                        User::Panic(KTMSPanic, TMS_RESULT_DOES_NOT_EXIST));
                break;
            }
        }
    else if (iFormat == EFormatTts)
        {
        switch (iPlayerStatus)
            {
            case ETonePlaying:
                if (iTtsPlayer)
                    {
                    iTtsPlayer->Stop();
                    }
                DoPlay();
                break;
            case EToneReady:
                iPlayerStatus = ETonePlaying;
                DoPlay();
                break;
            case EToneLoading:
                iToBePlaying = ETrue;
                break;
            default:
                __ASSERT_DEBUG(EFalse,
                        User::Panic(KTMSPanic, TMS_RESULT_DOES_NOT_EXIST));
                break;
            }
        }
    else
        {
        switch (iPlayerStatus)
            {
            case ETonePlaying:
                StopPlaying();
                iPlayerStatus = ETonePlaying;
                DoPlay();
                break;
            case EToneReady:
                iPlayerStatus = ETonePlaying;
                DoPlay();
                break;
            case EToneLoading:
                iToBePlaying = ETrue;
                break;
            default:
                __ASSERT_DEBUG(EFalse,
                        User::Panic(KTMSPanic, TMS_RESULT_DOES_NOT_EXIST));
                break;
            }
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::ReStartPlaying
// -----------------------------------------------------------------------------
//
void TMSRtAudioHdlr::ReStartPlaying()
    {
    TRACE_PRN_FN_ENT;
    if (iFormat == EFormatTone && iTonePlayer)
        {
        iTonePlayer->Play();
        iPlayerStatus = ETonePlaying;
        }
    else if (iFormat == EFormatTts && iTtsPlayer)
        {
        iTtsPlayer->Play();
        iPlayerStatus = ETonePlaying;
        }
    else if (iFormat == EFormatSample && iSamplePlayer)
        {
        iSamplePlayer->Play();
        iPlayerStatus = ETonePlaying;
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::StopPlaying
// -----------------------------------------------------------------------------
//
void TMSRtAudioHdlr::StopPlaying()
    {
    TRACE_PRN_FN_ENT;
    iToBePlaying = EFalse;

    if (iFormat == EFormatTone)
        {
        if (EMdaAudioToneUtilityPrepared == iTonePlayer->State())
            {
            iTonePlayer->CancelPrepare();
            }
        else if (EMdaAudioToneUtilityPlaying == iTonePlayer->State())
            {
            iTonePlayer->CancelPlay();
            }
        }
    else if (iFormat == EFormatTts)
        {
        iTtsPlayer->Stop();
        }
    else
        {
/*        if (i3DPlugin)
            {
            i3DPlugin->Stop();
            delete i3DPlugin;
            i3DPlugin = NULL;
            }
        else
            {*/
            iSamplePlayer->Stop();
/*            }*/
        }
    iPlayerStatus = EToneReady;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::MapcInitComplete
// -----------------------------------------------------------------------------
//
void TMSRtAudioHdlr::MapcInitComplete(TInt aError,
        const TTimeIntervalMicroSeconds& /*aDuration*/)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_N2(_L("RT player type[%d], err[%d]"), iRtType, aError);

    __ASSERT_DEBUG((iFormat == EFormatSample) ||
            ((iFormat == EFormatTts) && (iPlayerStatus == EToneLoading)),
            PANIC(TMS_RESULT_INVALID_STATE));

    if (aError == TMS_RESULT_SUCCESS)
        {
        if (iToBePlaying)
            {
            iPlayerStatus = ETonePlaying;
            iToBePlaying = EFalse;
            DoPlay();
            }
        else
            {
            iPlayerStatus = EToneReady;
            }
        }
    iObserver.RtAudioHdlrEvent(ECmdRingToneOpenComplete, aError, iRtType);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::MapcPlayComplete
// -----------------------------------------------------------------------------
//
void TMSRtAudioHdlr::MapcPlayComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_N2(_L("RT player type[%d], err[%d]"), iRtType, aError);

    iPlayerStatus = EToneReady;

    // Delete 3D plugin before RT player. In case of continuous ringing type,
    // 3D plugin is destructed in StopPlaying().
/*    if (i3DPlugin)
        {
        i3DPlugin->Stop();
        delete i3DPlugin;
        i3DPlugin = NULL;
        }*/
    iObserver.RtAudioHdlrEvent(ECmdRingTonePlayComplete, aError, iRtType);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::MatoPrepareComplete
// -----------------------------------------------------------------------------
//
void TMSRtAudioHdlr::MatoPrepareComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_N2(_L("RT player type[%d], err[%d]"), iRtType, aError);

    __ASSERT_DEBUG(((iFormat == EFormatTone) &&
            (iPlayerStatus == EToneLoading)), PANIC(TMS_RESULT_INVALID_STATE));

    if (aError == TMS_RESULT_SUCCESS)
        {
        if (iToBePlaying)
            {
            iPlayerStatus = ETonePlaying;
            iToBePlaying = EFalse;
            DoPlay();
            }
        else
            {
            iPlayerStatus = EToneReady;
            }
        }
    iObserver.RtAudioHdlrEvent(ECmdRingToneOpenComplete, aError, iRtType);
    TRACE_PRN_FN_ENT;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::MatoPlayComplete
// -----------------------------------------------------------------------------
//
void TMSRtAudioHdlr::MatoPlayComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_N2(_L("RT player type[%d], err[%d]"), iRtType, aError);
    iObserver.RtAudioHdlrEvent(ECmdRingTonePlayComplete, aError, iRtType);
    TRACE_PRN_FN_EXT;
   }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::DoPlay
// -----------------------------------------------------------------------------
//
void TMSRtAudioHdlr::DoPlay()
    {
    TRACE_PRN_FN_ENT;

    __ASSERT_DEBUG(iPlayerStatus == ETonePlaying,
            PANIC(TMS_RESULT_INVALID_STATE));

    //TInt err(TMS_RESULT_SUCCESS);
    SetRingingTypeProperties();

    if (iFormat == EFormatTone)
        {
        iTonePlayer->Play();
        }
    else if (iFormat == EFormatTts)
        {
        iTtsPlayer->Play();
        }
    else
        {
#ifdef __WINSCW__
        iSamplePlayer->Play();
#else
/*
        //TODO: compare with phoneapp
        TInt err = KErrNone;
        TRAP(err, i3DPlugin = C3DRingingToneInterface::NewL(KNullUid));
        if (!err && i3DPlugin)
            {
            i3DPlugin->SetAttr(E3DRTIAttrAudioPlayerUtility, iSamplePlayer);
            TRAP(err, i3DPlugin->PlayL())
            }
        if (err || !i3DPlugin)
            {
            if (!iAudioOutput)
                {
                TRAP(err, iAudioOutput = CAudioOutput::NewL(*iSamplePlayer));
                }
            if (!err && iAudioOutput)
                {
                TRAP(err, iAudioOutput->SetAudioOutputL(CAudioOutput::EAll));
                }
            }*/
        iSamplePlayer->Play();
#endif //__WINSCW__
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::SetRingingTypeProperties
// -----------------------------------------------------------------------------
//
void TMSRtAudioHdlr::SetRingingTypeProperties()
    {
    TInt rampTime(0);
    if (iFormat == EFormatTone)
        {
        __ASSERT_DEBUG(iTonePlayer, PANIC(TMS_RESULT_UNINITIALIZED_OBJECT));
        switch (iRingType)
            {
            case ETypeRinging:
                {
                // If we have TTS activated and ringing type is "ringing", then
                // we need to play TTS sequence again when ringtone restarts.
                // That's why we need to set ringing type to ETypeRingingOnce,
                // because it is the only way of knowing when ringtone playback
                // is completed. Then we can restart it with new TTS iterations.
                if (iTTsToBePlayed)
                    {
                    // Play only once
                    iTonePlayer->SetRepeats(0, TTimeIntervalMicroSeconds(
                            KTMSRingingRepeatsTrailPause));
                    iTonePlayer->SetVolume(ConvertVolume(iVolume));
                    }
                else
                    {
                    iTonePlayer->SetRepeats(KTMSMdaAudioToneRepeatForever,
                            TTimeIntervalMicroSeconds(
                                    KTMSRingingRepeatsTrailPause));
                    iTonePlayer->SetVolume(ConvertVolume(iVolume));
                    }
                break;
                }
            case ETypeRingingOnce:
                {
                // Play only once
                iTonePlayer->SetRepeats(0, TTimeIntervalMicroSeconds(
                        KTMSRingingRepeatsTrailPause));
                iTonePlayer->SetVolume(ConvertVolume(iVolume));
                break;
                }
            case ETypeAscending:
                {
                iTonePlayer->SetRepeats(KTMSMdaAudioToneRepeatForever,
                        TTimeIntervalMicroSeconds(
                                KTMSRingingRepeatsTrailPause));

                // Special case for TTS + ascending profile. Volume and
                // ramptime need to be different then usual
                if (iTTsToBePlayed)
                    {
                    // RampTime is only 3 seconds and volume is 1.
                    rampTime = KTMSAudioAscendingRampDuration;
                    iTonePlayer->SetVolumeRamp(TTimeIntervalMicroSeconds(
                            rampTime));
                    iCurrentVolume = 1; //we start from the lowest volume
                    iTonePlayer->SetVolume(ConvertVolume(iCurrentVolume));
                    }
                else
                    {
                    // RampTime: time for one step * volume level
                    rampTime = KTMSAudioAscendingRampDuration * iVolume;
                    iTonePlayer->SetVolumeRamp(TTimeIntervalMicroSeconds(
                            rampTime));
                    iCurrentVolume = 1; //we start from the lowest volume
                    iTonePlayer->SetVolume(ConvertVolume(iVolume));
                    }
                break;
                }
            default:
                //PANIC(TMS_RESULT_DOES_NOT_EXIST);
                break;
            }
        //Set priority and preference
        iTonePlayer->SetPriority(iPriority, iPreference);
        }
    else if (iFormat == EFormatTts)
        {
        __ASSERT_DEBUG(iTtsPlayer, PANIC(TMS_RESULT_UNINITIALIZED_OBJECT));
        switch (iRingType)
            {
            case ETypeRinging:
            case ETypeAscending:
                {
                // Not allowed for TTS.
                break;
                }
            case ETypeRingingOnce:
                {
                iTtsPlayer->SetRepeats(0, TTimeIntervalMicroSeconds(
                        KTMSRingingRepeatsTrailPause));
                iTtsPlayer->SetVolume(ConvertVolume(iVolume));
                break;
                }
            default:
                {
                __ASSERT_DEBUG(EFalse, PANIC(TMS_RESULT_DOES_NOT_EXIST));
                break;
                }
            }
        }
    else
        {
        __ASSERT_DEBUG(iSamplePlayer, PANIC(TMS_RESULT_UNINITIALIZED_OBJECT));
        switch (iRingType)
            {
            case ETypeRinging:
                {
                // If we have TTS activated and ringing type is "ringing", then
                // we need to play TTS sequence again when ringtone restarts.
                // That's why we need to set ringingtype to ETypeRingingOnce,
                // because it is the only way of knowing when ringtone playback
                // is completed. Then we can restart it with new TTS iterations.
                if (iTTsToBePlayed)
                    {
                    // Play only once
                    iSamplePlayer->SetRepeats(0, TTimeIntervalMicroSeconds(
                            KTMSRingingRepeatsTrailPause));
                    iSamplePlayer->SetVolume(ConvertVolume(iVolume));
                    }
                else
                    {
                    iSamplePlayer->SetRepeats(
                            KTMSMdaAudioToneRepeatForever,
                            TTimeIntervalMicroSeconds(
                                    KTMSRingingRepeatsTrailPause));
                    iSamplePlayer->SetVolume(ConvertVolume(iVolume));
                    }
                break;
                }
            case ETypeRingingOnce:
                {
                iSamplePlayer->SetRepeats(0, TTimeIntervalMicroSeconds(
                        KTMSRingingRepeatsTrailPause));
                iSamplePlayer->SetVolume(ConvertVolume(iVolume));
                break;
                }
            case ETypeAscending:
                {
                iSamplePlayer->SetRepeats(KTMSMdaAudioToneRepeatForever,
                        TTimeIntervalMicroSeconds(
                                KTMSRingingRepeatsTrailPause));

                // Special case for TTS + ascending profile. Volume and
                // ramptime need to be different then usual
                if (iTTsToBePlayed)
                    {
                    // RampTime is only 3 seconds and volume is 1.
                    rampTime = KTMSAudioAscendingRampDuration;
                    iSamplePlayer->SetVolumeRamp(TTimeIntervalMicroSeconds(
                            rampTime));
                    iCurrentVolume = 1; //we start from the lowest volume
                    iSamplePlayer->SetVolume(ConvertVolume(iCurrentVolume));
                    }
                else
                    {
                    // RampTime: time for one step * volume level
                    rampTime = KTMSAudioAscendingRampDuration * iVolume;
                    iSamplePlayer->SetVolumeRamp(TTimeIntervalMicroSeconds(
                            rampTime));

                    iCurrentVolume = 1; //we start from the lowest volume
                    iSamplePlayer->SetVolume(ConvertVolume(iVolume));
                    }
                break;
                }
            default:
                {
                break;
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::SetNewVolumeAndRamptime
// -----------------------------------------------------------------------------
//
void TMSRtAudioHdlr::SetNewVolumeAndRamptime(TInt aVolume, TInt aRamptime)
    {
    // Check that volume is in a valid range.
    TInt volume = aVolume < 1 ? 1 : aVolume;

    if (iFormat == EFormatTone)
        {
        if (iTonePlayer)
            {
            iTonePlayer->SetVolumeRamp(TTimeIntervalMicroSeconds(aRamptime));
            iTonePlayer->SetVolume(ConvertVolume(volume));
            }
        }
    else if (iFormat == EFormatTts)
        {
        if (iTtsPlayer)
            {
            iTtsPlayer->SetVolumeRamp(TTimeIntervalMicroSeconds(aRamptime));
            iTtsPlayer->SetVolume(ConvertVolume(volume));
            }
        }
    else
        {
        if (iSamplePlayer)
            {
            iSamplePlayer->SetVolumeRamp(TTimeIntervalMicroSeconds(aRamptime));
            iSamplePlayer->SetVolume(ConvertVolume(volume));
            }
        }
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::ConvertVolume
// -----------------------------------------------------------------------------
//
TInt TMSRtAudioHdlr::ConvertVolume(TInt aVolume)
    {
    TInt result(0);

    if (iFormat == EFormatTone)
        {
        if (iTonePlayer)
            {
            result = iTonePlayer->MaxVolume() * aVolume / KMaxVolumeLevel;
            }
        }
    else if (iFormat == EFormatTts)
        {
        if (iTtsPlayer)
            {
            result = iTtsPlayer->MaxVolume() * aVolume / KMaxVolumeLevel;
            }
        }
    else
        {
        if (iSamplePlayer)
            {
            result = iSamplePlayer->MaxVolume() * aVolume / KMaxVolumeLevel;
            }
        }

    // If user selected minimum volume level set HW volume 1
    if (aVolume == KMinVolumeLevel && result == 0)
        {
        result = 1;
        }
    return result;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::SetTTsToBePlayed
// -----------------------------------------------------------------------------
//
void TMSRtAudioHdlr::SetTTsToBePlayed(const TBool aTTsToBePlayed)
    {
    iTTsToBePlayed = aTTsToBePlayed;
    }

// -----------------------------------------------------------------------------
// TMSRtAudioHdlr::MutePlaying
// -----------------------------------------------------------------------------
//
void TMSRtAudioHdlr::MutePlaying()
    {
    if (iPlayerStatus == ETonePlaying)
        {
        if (iFormat == EFormatTone)
            {
            if (iTonePlayer)
                {
                iTonePlayer->SetVolume(0);
                }
            }
        else if (iFormat == EFormatTts && iTtsPlayer)
            {
            if (iTtsPlayer)
                {
                iTtsPlayer->SetVolume(0);
                }
            }
        else // EFormatSample
            {
            if (iSamplePlayer)
                {
                iSamplePlayer->SetVolume(0);
                }
            }
        }
    else
        {
        // Mute during EToneLoading state.
        iToBePlaying = EFalse;
        }
    }

