/*
 * Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: voip audio service -
 *
 */

// INCLUDE FILES
#include "VoIPAudioServicesTestClass.h"
#include "debug.h"

const TUint KLoopCount = 200;

// ----------------------------------------------------------------------------
// CVoIPAudioServicesTestClass::FillBuffer
//
// Callback from MVoIPDownlinkObserver
// ----------------------------------------------------------------------------
//
void CVoIPAudioServicesTestClass::FillBuffer(
        const CVoIPAudioDownlinkStream& /*aSrc*/, CVoIPDataBuffer* aBuffer)
    {
    iLog->Log(_L("CVoIPAudioServicesTestClass::FillBuffer"));

    if (!iJBIntfc)
        {
        if (iPlayCounter++ < KLoopCount)
            {
            aBuffer->GetPayloadPtr(iPlayBufPtr);
            iPlayBufReady = ETrue; // always ready in JB mode

            if (iDnLinkStatus == EStreaming)
                {
                if (iDnLinkCodec == EG729 && ((iPlayCounter == KLoopCount / 2)
                        || (iPlayCounter == KLoopCount / 5)))
                    {
                    BadLsfNextBuffer();
                    }
                DoLoopback();
                }
            }
        }
    else //JB case
        {
        // In JB mode we will only receive FillBuffer callback once after
        // ConfigureJB. We will simply reuse same buffer to send data to VAS.
        // The loopback play is controlled by the recorder side.
        aBuffer->GetPayloadPtr(iPlayBufPtr);
        iPlayBufReady = ETrue; // always ready in JB mode
        }
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServicesTestClass::Event
//
// Callback from MVoIPDownlinkObserver
// ----------------------------------------------------------------------------
//
void CVoIPAudioServicesTestClass::Event(
        const CVoIPAudioDownlinkStream& /*aSrc*/, TInt aEventType,
        TInt aError)
    {
    switch (aEventType)
        {
        case MVoIPDownlinkObserver::KOpenComplete:
            {
            if (aError == KErrNone)
                {
                // Now we can configure decoder and audio device
                iVoIPDnlink->GetMaxVolume(iMaxVolume);
                iVoIPDnlink->SetVolume(iMaxVolume / 2);
                //  ConfigureDecoder();
                iDnLinkStatus = EReady;
                iLog->Log(_L("MVoIPDownlinkObserver::KOpenComplete: aError = %d"),
                        aError);
                ProcessEvent(EOpenDownlinkComplete, aError);
                }
            iLog->Log(_L("DNL Initialized: aError = %d"), aError);
            break;
            }
        case MVoIPDownlinkObserver::KDownlinkClosed:
            {
            iDnLinkStatus = ENotReady;
            iLog->Log(_L("MVoIPDownlinkObserver::KDownlinkClosed: aError = %d"),
                    aError);
            ProcessEvent(EDownlinkClosed, aError);
            break;
            }
        case MVoIPDownlinkObserver::KDownlinkError:
            {
            iDnLinkStatus = EReady;
            iLog->Log(_L("DNL Error: aError = %d"), aError);
            ProcessEvent(EDownlinkError, aError);
            break;
            }
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServicesTestClass::EmptyBuffer
//
// Callback from MVoIPUplinkObserver
// ----------------------------------------------------------------------------
//
void CVoIPAudioServicesTestClass::EmptyBuffer(
        const CVoIPAudioUplinkStream& /*aSrc*/, CVoIPDataBuffer* aBuffer)
    {
    iLog->Log(_L("CVoIPAudioServicesTestClass::EmptyBuffer"));

    if (iRecCounter++ < KLoopCount)
        {
        aBuffer->GetPayloadPtr(iRecBufPtr);
        iRecBufReady = ETrue;

        if (iUpLinkStatus == EStreaming)
            {
            if (iJBIntfc)
                {
                DoJBLoopback();
                }
            else
                {
                DoLoopback();
                }
            }
        }
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServicesTestClass::Event
//
// Callback from MVoIPUplinkObserver
// ----------------------------------------------------------------------------
//
void CVoIPAudioServicesTestClass::Event(
        const CVoIPAudioUplinkStream& /*aSrc*/, TInt aEventType, TInt aError)
    {
    switch (aEventType)
        {
        case MVoIPUplinkObserver::KOpenComplete:
            {
            if (aError == KErrNone)
                {
                // Now we can configure encoder and audio device
                iVoIPUplink->GetMaxGain(iMaxGain);
                iVoIPUplink->SetGain(iMaxGain);
                iUpLinkStatus = EReady;
                iLog->Log(_L("MVoIPUplinkObserver::KOpenComplete: aError = %d"),
                        aError);
                ProcessEvent(EOpenUplinkComplete, aError);
                }
            iLog->Log(_L("UPL Initialized: aError = %d"), aError);
            break;
            }
        case MVoIPUplinkObserver::KUplinkClosed:
            {
            iUpLinkStatus = ENotReady;
            iLog->Log(_L("MVoIPUplinkObserver::KUplinkClosed: aError = %d"),
                    aError);
            ProcessEvent(EUplinkClosed, aError);
            break;
            }
        case MVoIPUplinkObserver::KUplinkError:
            {
            iUpLinkStatus = EReady;
            iLog->Log(_L("UPL Error: aError = %d"), aError);
            ProcessEvent(EUplinkError, aError);
            break;
            }
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServicesTestClass::Event
//
// Callback from MVoIPFormatObserver
// ----------------------------------------------------------------------------
//
void CVoIPAudioServicesTestClass::Event(const CVoIPFormatIntfc& /*aSrc*/,
        TInt aEventType)
    {
    switch (aEventType)
        {
        case MVoIPFormatObserver::KSilenceBegin:
        case MVoIPFormatObserver::KSilenceUpdate:
        case MVoIPFormatObserver::KSilenceEnd:
        case MVoIPFormatObserver::KObserverEventMax:
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServicesTestClass::Event
//
// Callback from MDTMFToneObserver
// ----------------------------------------------------------------------------
//
void CVoIPAudioServicesTestClass::Event(const CDTMFTonePlayer& /*aSrc*/,
        TInt aEventType, TInt aError)
    {
    switch (aEventType)
        {
        case MDTMFToneObserver::KOpenCompleteDNL:
            iLog->Log(
                    _L("MDTMFToneObserver:KOpenCompleteDNL:DTMF DNL-PLR Ready: aError = %d"),
                    aError);
            ProcessEvent(EOpenCompleteDNL, aError);
            break;
        case MDTMFToneObserver::KOpenCompleteUPL:
            iLog->Log(
                    _L("MDTMFToneObserver:KOpenCompleteUPL:DTMF UPL-PLR Ready: aError = %d"),
                    aError);
            ProcessEvent(EOpenCompleteUPL, aError);
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServicesTestClass::Event
//
// Callback from MRingToneObserver
// ----------------------------------------------------------------------------
//
void CVoIPAudioServicesTestClass::Event(const CRingTonePlayer& /*aSrc*/,
        TInt aEventType, TInt aError)
    {
    switch (aEventType)
        {
        case MRingToneObserver::KOpenComplete:
            ProcessEvent(EOpenComplete, aError);
            iLog->Log(_L("RT PLR Ready: error = %d"), aError);
            break;
        case MRingToneObserver::KPlaybackComplete:
            ProcessEvent(EPlaybackComplete, aError);
            iLog->Log(_L("RT Play Complete: error = %d"), aError);
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServicesTestClass::Event
//
// Callback from MVoIPJitterBufferObserver
// ----------------------------------------------------------------------------
//
void CVoIPAudioServicesTestClass::Event(
        const CVoIPJitterBufferIntfc& /*aSrc*/, TInt aEventType)
    {
    switch (aEventType)
        {
        // Reserved for future use
        case MVoIPJitterBufferObserver::KJBReserved1:
        case MVoIPJitterBufferObserver::KJBReserved2:
        default:
            break;
        }
    }
