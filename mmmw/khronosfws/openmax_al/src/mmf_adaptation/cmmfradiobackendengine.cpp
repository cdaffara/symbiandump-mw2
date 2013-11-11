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
* Description:
*
*/

#include "cmmfradiobackendengine.h"

extern "C" {
#include "xaradioitfadaptation.h"
#include "xaadaptationmmf.h"
#include "xaadptbasectx.h"
}

CMMFRadioBackendEngine *CMMFRadioBackendEngine::s_instance = 0;

CMMFRadioBackendEngine* CMMFRadioBackendEngine::Instance()
{
    if (!s_instance)
    {
        s_instance = new CMMFRadioBackendEngine();
        if (s_instance)
        {
            s_instance->ConstructL();
        }
    }
    return s_instance;
}

void CMMFRadioBackendEngine::DeleteInstance()
{
    if (iFmTunerUtility)
    {
        iFmTunerUtility->Close();
        iFmTunerUtility = NULL;
    }
    if (iRadioPlayerUtility)
    {
        iRadioPlayerUtility->Close();
        iRadioPlayerUtility = NULL;
    }

    if (iRadioUtility)
    {
        delete iRadioUtility;
        iRadioUtility = NULL;
    }
}

CMMFRadioBackendEngine::~CMMFRadioBackendEngine()
{

}

CMMFRadioBackendEngine::CMMFRadioBackendEngine()
{

}

void CMMFRadioBackendEngine::ConstructL()
{
    iRadioUtility = CRadioUtility::NewL( ETrue );
    if (iRadioUtility)
    {
        iFmTunerUtility = &iRadioUtility->RadioFmTunerUtilityL( *this );
    }
    if (iFmTunerUtility)
    {
        iFmTunerUtility->EnableTunerInOfflineMode( ETrue );
        iFmTunerUtility->RequestTunerControl();  
    }
    if (iRadioUtility)
    {
        iRadioPlayerUtility = &iRadioUtility->RadioPlayerUtilityL( *this );
    }

    SetForceMonoFlag();
    iDefaultFrequency = RADIO_DEFAULT_FREQ;
    iDefaultFreqRange = (TFmRadioFrequencyRange)RADIO_DEFAULT_FREQ_RANGE;
    iDefaultMinFreq = RADIO_DEFAULT_MIN_FREQ;
    iDefaultMaxFreq = RADIO_DEFAULT_MAX_FREQ;

    iAdaptContext = (XAAdaptationBaseCtx*)CMMFRadioBackendEngine::Instance();

}
void CMMFRadioBackendEngine::StationSeek(TBool aUpwards)
{
    if (iFmTunerUtility)
    {
        iFmTunerUtility->StationSeek(aUpwards);
    }
}

void CMMFRadioBackendEngine::SetFrequency(TInt aFreq)
{
//  DEBUG_CPP_API_A1("CMMFRadioBackendEngine::SetFrequency: %d", aFreq);

    if (iFmTunerUtility)
    {
        iFmTunerUtility->SetFrequency(aFreq);
    }
}

XAresult CMMFRadioBackendEngine::GetFrequency(TInt& aFreq)
{
//    DEBUG_CPP_API("CMMFRadioBackendEngine::GetFrequency");
    TInt ret = KErrNotFound;
    if (iFmTunerUtility)
    {
        ret = iFmTunerUtility->GetFrequency(aFreq);
        if (ret != KErrNone)
        {
            aFreq = iDefaultFrequency;
            ret = KErrNone;
        }
    }
//  DEBUG_CPP_API_A1("CMMFRadioBackendEngine::GetFrequency RET: %d", ret);
    return TranslateError(ret);
}

XAresult CMMFRadioBackendEngine::GetSignalStrength(TInt& aSignalStrength)
{
    TInt ret = KErrNotFound;
    if (iFmTunerUtility)
    {
        ret = iFmTunerUtility->GetSignalStrength(aSignalStrength);
    }
//    DEBUG_CPP_API_A1("CMMFRadioBackendEngine::GetSignalStrength RET: %d", ret);
    return TranslateError(ret);
}

void CMMFRadioBackendEngine::CancelSetFrequency()
{
    if (iFmTunerUtility)
    {
        iFmTunerUtility->CancelSetFrequency();
    }
}

void CMMFRadioBackendEngine::CancelStationSeek()
{
    if (iFmTunerUtility)
    {
        iFmTunerUtility->CancelStationSeek();
    }
}

void CMMFRadioBackendEngine::SetFreqRange(TFmRadioFrequencyRange aRange)
{
    if (iFmTunerUtility)
    {
        iFmTunerUtility->SetFrequencyRange(aRange);
    }
}

XAresult CMMFRadioBackendEngine::GetFreqRange(TFmRadioFrequencyRange& aRange)
{
    TInt ret = KErrNotFound;
    TInt minFreq;
    TInt maxFreq;

    if (iFmTunerUtility)
    {
        ret = iFmTunerUtility->GetFrequencyRange((TFmRadioFrequencyRange&)aRange, (TInt&)minFreq, (TInt&)maxFreq);
        if (ret != KErrNone)
        {
            minFreq = iDefaultMinFreq;
            maxFreq = iDefaultMaxFreq;
            aRange = iDefaultFreqRange;
            ret = KErrNone;
        }
    }
//    DEBUG_CPP_API_A1("CMMFRadioBackendEngine::GetFreqRange RET: %d", ret);
    return TranslateError(ret);
}

XAresult CMMFRadioBackendEngine::GetFreqRangeProperties(TFmRadioFrequencyRange& aRange, TInt& aMinFreq, TInt& aMaxFreq)
{
    TInt ret = KErrNotFound;
    if (iFmTunerUtility)
    {
        ret = iFmTunerUtility->GetFrequencyRange((TFmRadioFrequencyRange&)aRange, (TInt&)aMinFreq, (TInt&)aMaxFreq);
        if (ret != KErrNone)
        {
            aRange = iDefaultFreqRange;
            aMinFreq = iDefaultMinFreq;
            aMaxFreq = iDefaultMaxFreq;
            ret = KErrNone;
        }
    }
 //   DEBUG_CPP_API_A1("CMMFRadioBackendEngine::GetFreqRangeProperties RET: %d", ret);
    return TranslateError(ret);
}

XAresult CMMFRadioBackendEngine::SetVolume(TInt aVol)
{
    TInt ret = KErrNotFound; 
    if (iRadioPlayerUtility)
    {
        ret = iRadioPlayerUtility->SetVolume(aVol);
    }
    return TranslateError(ret);
}

XAresult CMMFRadioBackendEngine::SetMute(TBool aMute)
{
    TInt ret = KErrNotFound;
    if (iRadioPlayerUtility)
    {
        ret = iRadioPlayerUtility->Mute(aMute);
    }
    return TranslateError(ret);
}

XAresult CMMFRadioBackendEngine::GetVolume(TInt& aVol)
{
    TInt ret = KErrNotFound;
    if (iRadioPlayerUtility)
    {
        ret = iRadioPlayerUtility->GetVolume(aVol);
    }
    return TranslateError(ret);
}

XAresult CMMFRadioBackendEngine::GetForcedMonoReception(TUint& aForcedMono)
{
    TInt ret = KErrNotFound;
    TBool forceMono = XA_BOOLEAN_FALSE;

    if (iFmTunerUtility)
    {
        ret = iFmTunerUtility->GetForcedMonoReception(forceMono);
        if (ret == KErrNotReady)
        {
            aForcedMono = XA_STEREOMODE_AUTO; // Radio Utility Default value
            return TranslateError(KErrNone);
        }
    }

    if (forceMono)
    {
        aForcedMono = XA_STEREOMODE_MONO;
    }
    else
    {
        if (iForceStereo)
            aForcedMono = XA_STEREOMODE_STEREO;
        else
            aForcedMono = XA_STEREOMODE_AUTO;
    }
    return TranslateError(ret);
}

void CMMFRadioBackendEngine::PlayRadio()
{
    if (iRadioPlayerUtility)
    {
        iRadioPlayerUtility->Play();
    }
}

void CMMFRadioBackendEngine::StopRadio()
{
    if (iRadioPlayerUtility)
    {
        iRadioPlayerUtility->Stop();
    }
}

XAresult CMMFRadioBackendEngine::ForceMonoReception(TUint aForcedMono)
{
    TInt ret = KErrNotFound;
    TUint currentMode;

    ret = GetForcedMonoReception(currentMode);
    if (ret != XA_RESULT_SUCCESS)
        return TranslateError(ret);

    if (iFmTunerUtility)
    {
        if (aForcedMono == XA_STEREOMODE_MONO)
        {
            iForceStereo = EFalse;
            ret = iFmTunerUtility->ForceMonoReception(ETrue);
        }
        else if (aForcedMono == XA_STEREOMODE_STEREO)
        {
            iForceStereo = ETrue;
            if (currentMode == XA_STEREOMODE_AUTO) // Transition from Auto to Stereo doesn't require Utility call:
            {
                MrftoForcedMonoChange(ETrue);
            }
            else
            {
                ret = iFmTunerUtility->ForceMonoReception(EFalse);
            }
        }
        else // (aForcedMono == XA_STEREOMODE_AUTO)
        {
            iForceStereo = EFalse;
            if (currentMode == XA_STEREOMODE_STEREO) // Transition from Stereo to Auto doesn't require Utility call:
            {
                MrftoForcedMonoChange(EFalse);
            }
            else
            {
                ret = iFmTunerUtility->ForceMonoReception(EFalse);
            }
        }
    }
//    DEBUG_CPP_API_A1("CMMFRadioBackendEngine::ForceMonoReception RET: %d", ret);
    return TranslateError(ret);
}

XAresult CMMFRadioBackendEngine::SetForceMonoFlag() 
{
    TInt ret = KErrNotFound;
    TBool forceMono = XA_BOOLEAN_FALSE;

    if (iFmTunerUtility)
    {
        ret = iFmTunerUtility->GetForcedMonoReception(forceMono);
        if (ret == KErrNotReady)
        {
            // For Radio Utility Default value = XA_STEREOMODE_STEREO
            iForceStereo = ETrue;
            return TranslateError(KErrNone);
        }
    }
    if (!forceMono)
    {
        iForceStereo = EFalse;
    }
    else
    {
        iForceStereo = ETrue;
    }
  return TranslateError(ret);
}

XAresult CMMFRadioBackendEngine::GetSquelch(TBool& aSquelch)
{
    TInt ret = KErrNotFound;
    if (iFmTunerUtility)
    {
        ret = iFmTunerUtility->GetSquelch(aSquelch);
    }
    return TranslateError(ret);
}

XAresult CMMFRadioBackendEngine::SetSquelch(TBool aSquelch)
{
    TInt ret = KErrNotFound;
    if (iFmTunerUtility)
    {
        ret = iFmTunerUtility->SetSquelch(aSquelch);
    }
    return TranslateError(ret);
}

void CMMFRadioBackendEngine::SetAdaptContext(void * adaptcontext)
{
    iAdaptContext = (XAAdaptationBaseCtx*)adaptcontext;
}

XAresult CMMFRadioBackendEngine::TranslateError(TInt error)
    {
    XAresult status(XA_RESULT_SUCCESS);
    switch(error)
        {
        case KErrNone:
            status = XA_RESULT_SUCCESS;
            break;
        case KErrNotFound:
            status = XA_RESULT_MEMORY_FAILURE;
            break;
        // to do: investigate and add other possible errors:
            
       /* case XA_RESULT_PRECONDITIONS_VIOLATED:
 
            break;
        case XA_RESULT_PARAMETER_INVALID:

            break;

        case XA_RESULT_RESOURCE_ERROR:

            break;
        case XA_RESULT_RESOURCE_LOST:
 
            break;
        case XA_RESULT_IO_ERROR:

            break;
        case XA_RESULT_BUFFER_INSUFFICIENT:

            break;
        case XA_RESULT_CONTENT_CORRUPTED:

            break;
        case XA_RESULT_CONTENT_UNSUPPORTED:
 
            break;
        case XA_RESULT_CONTENT_NOT_FOUND:

            break;
        case XA_RESULT_PERMISSION_DENIED:

            break;
        case XA_RESULT_FEATURE_UNSUPPORTED:

            break;
        case XA_RESULT_INTERNAL_ERROR:

            break;

        case XA_RESULT_OPERATION_ABORTED:

            break;
        case XA_RESULT_CONTROL_LOST:

            break;
            */
        default:
            status = XA_RESULT_UNKNOWN_ERROR;
            break;
        } // end switch
    return status;
    }
// -----------------------------------------------------------------------------
// CMMFRadioBackendEngine::MrpeoPresetChanged
// Observer for Presets
// -----------------------------------------------------------------------------
void CMMFRadioBackendEngine::MrpeoPresetChanged(
    TPresetChangeEvent /*aChange*/,
    TInt /*aIndex*/ )
{

}

// ----------------------------------------------------
// CMMFRadioBackendEngine::MrftoSquelchChange
//
// ----------------------------------------------------
//
void CMMFRadioBackendEngine::MrftoSquelchChange(
    TBool aSquelch)
{
//    DEBUG_CPP_API_A1("CMMFRadioBackendEngine::MrftoSquelchChange: aSquelch =  %d", aSquelch);
// Currently, Openmaxal doesn't have event defined to handle this callback
}

// ----------------------------------------------------
// CMMFRadioBackendEngine::MrftoForcedMonoChanged
// Called when a client enables/disabled forced mono reception
// ----------------------------------------------------
//
void CMMFRadioBackendEngine::MrftoForcedMonoChange(
    TBool aForcedMono )
{
//    DEBUG_CPP_API_A1("CMMFRadioBackendEngine::MrftoForcedMonoChange: aForcedMono =  %d", aForcedMono);
    XARadioItfAdapt_StereoStatusChange((XAAdaptationBaseCtx*)iAdaptContext, aForcedMono);
}

 // ----------------------------------------------------
// CMMFRadioBackendEngine::MrftoFrequencyChange
// Called when the tuned frequency changes
// ----------------------------------------------------
//
void CMMFRadioBackendEngine::MrftoFrequencyChange(
    TInt aNewFrequency)
{
//    DEBUG_CPP_API_A1("CMMFRadioBackendEngine::MrftoFrequencyChange: aNewFrequency =  %d", aNewFrequency);
    XARadioItfAdapt_FrequencyChange((XAAdaptationBaseCtx*)iAdaptContext, aNewFrequency);
}

// ----------------------------------------------------
// CMMFRadioBackendEngine::MrftoFrequencyRangeChange
//
// ----------------------------------------------------
//
void CMMFRadioBackendEngine::MrftoFrequencyRangeChange(
    TFmRadioFrequencyRange aNewRange )
{
//    DEBUG_CPP_API_A1("CMMFRadioBackendEngine::MrftoFrequencyRangeChange: aNewRange =  %d", aNewRange);
    XARadioItfAdapt_FrequencyRangeChange((XAAdaptationBaseCtx*)iAdaptContext, aNewRange);
}

// ----------------------------------------------------
// CMMFRadioBackendEngine::MrftoOfflineModeStatusChange
// Called when offline mode status changes
// ----------------------------------------------------
//
void CMMFRadioBackendEngine::MrftoOfflineModeStatusChange(
    TBool /*aOfflineMode*/ )
{

}

// ----------------------------------------------------
// CMMFRadioBackendEngine::MrftoAntennaStatusChange
// Called when antenna status changes.
// ----------------------------------------------------
//
void CMMFRadioBackendEngine::MrftoAntennaStatusChange(
    TBool /*aAttached*/ )
{

}

// ----------------------------------------------------
// CMMFRadioBackendEngine::MrftoFmTransmitterStatusChange
// The FM tuner is turned off when FM transmitter is on.
// ----------------------------------------------------
//
void CMMFRadioBackendEngine::MrftoFmTransmitterStatusChange(
    TBool /*aActive*/ )
{

}

// ----------------------------------------------------
// CMMFRadioBackendEngine::MrftoStationSeekComplete
//
// ----------------------------------------------------
//
void CMMFRadioBackendEngine::MrftoStationSeekComplete(
    TInt aError,
    TInt aFrequency )
{
    XARadioItfAdapt_SeekComplete((XAAdaptationBaseCtx*)iAdaptContext, aError, aFrequency);
}
 
// ----------------------------------------------------
// CMMFRadioBackendEngine::MrftoSetFrequencyComplete
// ----------------------------------------------------
//
void CMMFRadioBackendEngine::MrftoSetFrequencyComplete(
    TInt aError )
{
//    DEBUG_CPP_API_A1("CMMFRadioBackendEngine::MrftoSetFrequencyComplete: aError =  %d", aError);
    TInt freq = 0;
    if (aError == KErrNone)
    {
        iFmTunerUtility->GetFrequency(freq);
    }
//    DEBUG_CPP_API_A1("CMMFRadioBackendEngine::MrftoSetFrequencyComplete: new freq is: %d", freq);
    XARadioItfAdapt_SetFrequencyComplete((XAAdaptationBaseCtx*)iAdaptContext, aError, freq);
}

// ----------------------------------------------------
// CMMFRadioBackendEngine::MrftoSetFrequencyRangeComplete
//
// ----------------------------------------------------
//
void CMMFRadioBackendEngine::MrftoSetFrequencyRangeComplete(
    TInt aError )
{
//    DEBUG_CPP_API_A1("CMMFRadioBackendEngine::MrftoSetFrequencyRangeComplete: aError =  %d", aError);
    XARadioItfAdapt_SetFrequencyRangeComplete((XAAdaptationBaseCtx*)iAdaptContext, aError);
}

// ----------------------------------------------------
// CMMFRadioBackendEngine::MrftoRequestTunerControlComplete
// ----------------------------------------------------
//
void CMMFRadioBackendEngine::MrftoRequestTunerControlComplete(
    TInt aError)
{

}

// ----------------------------------------------------
// CMMFRadioBackendEngine::MrpoBalanceChange
//
// ----------------------------------------------------
//
void CMMFRadioBackendEngine::MrpoBalanceChange(
    TInt /*aLeftPercentage*/,
    TInt /*aRightPercentage*/ )
{

}

// ----------------------------------------------------
// CMMFRadioBackendEngine::MrpoMuteChange
//
// ----------------------------------------------------
//
void CMMFRadioBackendEngine::MrpoMuteChange(
    TBool aMute )
{
    XARadioItfAdapt_MuteChange((XAAdaptationBaseCtx*)iAdaptContext, aMute);
}

// ----------------------------------------------------
// CMMFRadioBackendEngine::MrpoVolumeChange
//
// ----------------------------------------------------
//
void CMMFRadioBackendEngine::MrpoVolumeChange(
    TInt aVolume )
{
    XARadioItfAdapt_VolumeChange((XAAdaptationBaseCtx*)iAdaptContext, aVolume);
}

// ----------------------------------------------------
// CMMFRadioBackendEngine::MrpoStateChange This is Tuner State
// Called when the state of the tuner changes
// ----------------------------------------------------
//
void CMMFRadioBackendEngine::MrpoStateChange(
    TPlayerState aState,
    TInt aError )
{
//    if ( aError )
//    {
//        DEBUG_CPP_API_A1("CMMFRadioBackendEngine::MrpoStateChange: aError =  %d", aError);
//    }

//    DEBUG_CPP_API_A1("CMMFRadioBackendEngine::MrpoStateChange: new state =  %d", aState);

    if ( aState == ERadioPlayerPlaying )
    {
        XARadioItfAdapt_StateChange((XAAdaptationBaseCtx*)iAdaptContext, ETrue);
    }  
    else  // ERadioPlayerIdle
    {
        XARadioItfAdapt_StateChange((XAAdaptationBaseCtx*)iAdaptContext, EFalse);
    }

}

extern "C" {

    void* cmmfradiobackendengine_init()
    {
    return CMMFRadioBackendEngine::Instance();
    }

    void cmmfradiobackendengine_delete(void* pContext)
    { 
        ((CMMFRadioBackendEngine*)(pContext))->DeleteInstance();
        delete ((CMMFRadioBackendEngine*)pContext);
    }

    void  set_frequency(void* pContext, void* pAdaptcontext, XAuint32 freq)
    {
        ((CMMFRadioBackendEngine*)(pContext))->SetAdaptContext(pAdaptcontext);
        ((CMMFRadioBackendEngine*)(pContext))->SetFrequency((TInt)freq);
    }

    XAresult  get_frequency(void* pContext, XAuint32* pFreq)
    {
        return ((CMMFRadioBackendEngine*)(pContext))->GetFrequency((TInt&)*pFreq);
    }

    XAresult  get_signal_strength(void* pContext, XAuint32* pSignalStrength)
    {
        return ((CMMFRadioBackendEngine*)(pContext))->GetSignalStrength((TInt&)*pSignalStrength);
    }

    void station_seek(void* pContext, XAboolean upwards)
    {
        ((CMMFRadioBackendEngine*)(pContext))->StationSeek(upwards);
    }

    void cancel_set_frequency(void* pContext, void* pAdaptcontext)
    {
        ((CMMFRadioBackendEngine*)(pContext))->SetAdaptContext(pAdaptcontext);
        ((CMMFRadioBackendEngine*)(pContext))->CancelSetFrequency();
    }

    void cancel_station_seek(void* pContext)
    {
        ((CMMFRadioBackendEngine*)(pContext))->CancelStationSeek();
    }

    void set_freq_range(void* pContext, void* pAdaptcontext, XAuint8 range)
    {
        ((CMMFRadioBackendEngine*)(pContext))->SetAdaptContext(pAdaptcontext);
        ((CMMFRadioBackendEngine*)(pContext))->SetFreqRange((TFmRadioFrequencyRange)range);
    }

    XAresult  get_freq_range(void* pContext, XAuint8* pRange)
    {
        return ((CMMFRadioBackendEngine*)(pContext))->GetFreqRange((TFmRadioFrequencyRange&)*pRange);
    }

    XAresult  get_freq_range_properties(void* pContext, XAuint8 aRange, XAuint32* pMinFreq, XAuint32* pMaxFreq)
    {
        return ((CMMFRadioBackendEngine*)(pContext))->GetFreqRangeProperties((TFmRadioFrequencyRange&) aRange, (TInt&) *pMinFreq, (TInt&) *pMaxFreq);
    }

    XAresult  set_volume(void* pContext, void* pAdaptcontext, XAuint32 vol)
    {
        ((CMMFRadioBackendEngine*)(pContext))->SetAdaptContext(pAdaptcontext);
        return ((CMMFRadioBackendEngine*)(pContext))->SetVolume((TInt)vol);
    }

    XAresult  get_volume(void* pContext, XAuint32* pVol)
    {
        return ((CMMFRadioBackendEngine*)(pContext))->GetVolume((TInt&)*pVol);
    }

    XAresult  set_mute(void* pContext, void* pAdaptcontext, XAboolean mute)
    {
        ((CMMFRadioBackendEngine*)(pContext))->SetAdaptContext(pAdaptcontext);
        return ((CMMFRadioBackendEngine*)(pContext))->SetMute(mute);
    }

    XAresult  get_stereo_mode(void* pContext, XAuint32* pMode)
    {
        return ((CMMFRadioBackendEngine*)(pContext))->GetForcedMonoReception((TUint&)*pMode);
    }

    XAresult  set_stereo_mode(void* pContext, void* pAdaptcontext, XAuint32 mode)
    {
        ((CMMFRadioBackendEngine*)(pContext))->SetAdaptContext(pAdaptcontext);
        return ((CMMFRadioBackendEngine*)(pContext))->ForceMonoReception(mode);
    }

    void play_radio(void* pContext, void* pAdaptcontext)
    {
        ((CMMFRadioBackendEngine*)(pContext))->SetAdaptContext(pAdaptcontext);
        ((CMMFRadioBackendEngine*)(pContext))->PlayRadio();
    }

    void stop_radio(void* pContext, void* pAdaptcontext)
    {
        ((CMMFRadioBackendEngine*)(pContext))->SetAdaptContext(pAdaptcontext);
        ((CMMFRadioBackendEngine*)(pContext))->StopRadio();
    }
    void get_squelch(void* pContext, XAboolean* pSquelch)
    {
        ((CMMFRadioBackendEngine*)(pContext))->GetSquelch((TBool&)*pSquelch);
    }
    void set_squelch(void* pContext, void* pAdaptcontext, XAboolean squelch)
    {
        ((CMMFRadioBackendEngine*)(pContext))->SetAdaptContext(pAdaptcontext);
        ((CMMFRadioBackendEngine*)(pContext))->SetSquelch((TBool)squelch);
    }
}
