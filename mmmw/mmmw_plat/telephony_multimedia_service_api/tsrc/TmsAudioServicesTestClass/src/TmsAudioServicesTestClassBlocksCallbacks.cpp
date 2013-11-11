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
 * Description: Telephony Multimedia Service - STIF TEST
 *
 */

// INCLUDE FILES
#include "TmsAudioServicesTestClass.h"
#include "debug.h"

// CONSTANTS
const TUint KLoopCount = 1000;

//From TMSStreamObserver
void CTmsAudioServicesTestClass::TMSStreamEvent(const TMSStream& stream,
        TMSSignalEvent event)
    {
    iLog->Log(_L("[tms cb]CTmsAudioServicesTestClass::TMSStreamEvent"));
    switch (const_cast<TMSStream&> (stream).GetStreamType())
        {
        case TMS_STREAM_UPLINK:
            {
            switch (event.type)
                {
                case TMS_EVENT_STREAM_STATE_CHANGED:
                    {
                    iLog->Log(_L("[tms cb]Uplink state change"));
                    switch (event.curr_state)
                        {
                        case TMS_STREAM_INITIALIZED:
                            iUpLinkStatus = INITIALIZED;
                            iLog->Log(_L("[tms cb]TMSStreamEvent EOpenUplinkComplete"));
                            ProcessEvent(EOpenUplinkComplete, KErrNone);
                            break;
                        case TMS_STREAM_UNINITIALIZED:
                            iUpLinkStatus = UNINITIALIZED;
                            iLog->Log(_L("[tms cb]TMSStreamEvent EUplinkClosed"));
                            ProcessEvent(EUplinkClosed, KErrNone);
                            break;
                        case TMS_STREAM_PAUSED:
                            iUpLinkStatus = PAUSED;
                            iLog->Log(_L("[tms cb]TMSStreamEvent EStreamPaused"));
                            ProcessEvent(EStreamPaused, KErrNone);
                            break;
                        case TMS_STREAM_STARTED:
                            iUpLinkStatus = STARTED;
                            iLog->Log(_L("[tms cb]TMSStreamEvent EStreamStarted"));
                            ProcessEvent(EStreamStarted, KErrNone);
                            break;
                        default:
                            break;
                        }
                    break;
                    }
                default:
                    break;
                }
            break;
            }
        case TMS_STREAM_DOWNLINK:
            {
            switch (event.type)
                {
                case TMS_EVENT_STREAM_STATE_CHANGED:
                    {
                    switch (event.curr_state)
                        {
                        case TMS_STREAM_INITIALIZED:
                            iDnLinkStatus = INITIALIZED;
                            iLog->Log(_L("[tms cb]TMSStreamEvent EOpenDownlinkComplete"));
                            ProcessEvent(EOpenDownlinkComplete, KErrNone);
                            break;
                        case TMS_STREAM_UNINITIALIZED:
                            iDnLinkStatus = UNINITIALIZED;
                            iLog->Log(_L("[tms cb]TMSStreamEvent EDownlinkClosed"));
                            ProcessEvent(EDownlinkClosed, KErrNone);
                            break;
                        case TMS_STREAM_PAUSED:
                            iDnLinkStatus = PAUSED;
                            iLog->Log(_L("[tms cb]TMSStreamEvent EStreamPaused"));
                            ProcessEvent(EStreamPaused, KErrNone);
                            break;
                        case TMS_STREAM_STARTED:
                            iDnLinkStatus = STARTED;
                            iLog->Log(_L("[tms cb]TMSStreamEvent EStreamStarted"));
                            ProcessEvent(EStreamStarted, KErrNone);
                            break;
                        default:
                            break;
                        }
                    break;
                    }
                default:
                    break;
                }
            break;
            }
        default:
            break;
        }
    }

//From TMSClientSourceObserver
void CTmsAudioServicesTestClass::FillBuffer(TMSBuffer& buffer)
    {
    //iLog->Log(_L("[tms cb]CTmsAudioServicesTestClass::FillBuffer"));

    // Note: Playback will continue until KLoopCount is reached or until timer
    //       timeout occurs set in the .cfg file by SetTimeout.

    if (iLoopCounter < KLoopCount)
        {
        iPlayBufReady = ETrue;
        iPlayBuf = &buffer;
        if (iDnLinkStatus == STARTED)
            {
            DoLoopback();
            }
        }
    }

void CTmsAudioServicesTestClass::BufferProcessed(const TMSBuffer* /*buffer*/,
        gint /*reason*/)
    {
    //iLog->Log(_L("[tms cb]CTmsAudioServicesTestClass::BufferProcessed"));
    }

//From TMSClientSinkObserver
void CTmsAudioServicesTestClass::ProcessBuffer(const TMSBuffer* buffer)
    {
    //iLog->Log(_L("[tms cb]CTmsAudioServicesTestClass::ProcessBuffer"));

    // Note: Recording will continue until KLoopCount is reached or until timer
    //       timeout occurs set in the .cfg file by SetTimeout.

    if (iLoopCounter < KLoopCount)
        {
        iRecBufReady = ETrue;
        iRecBuf = const_cast<TMSBuffer*> (buffer);
        if (iUpLinkStatus == STARTED)
            {
            DoLoopback();
            }
        }
    }

void CTmsAudioServicesTestClass::EffectsEvent(const TMSEffect& tmseffect,
        TMSSignalEvent /*event*/)
    {
    iLog->Log(_L("[tms cb]CTmsAudioServicesTestClass::EffectsEvent"));
    TMSEffectType effecttype;
    const_cast<TMSEffect&> (tmseffect).GetType(effecttype);
    switch (effecttype)
        {
        case TMS_EFFECT_VOLUME:
            ProcessEvent(EStreamVolChange, KErrNone);
            break;
        case TMS_EFFECT_GAIN:
            ProcessEvent(EStreamGainChange, KErrNone);
            break;
        case TMS_EFFECT_GLOBAL_VOL:
            ProcessEvent(EGlobalVolChange, KErrNone);
            break;
        case TMS_EFFECT_GLOBAL_GAIN:
            ProcessEvent(EGlobalGainChange, KErrNone);
            break;
        default:
            break;
        }
    }

// From TMSGlobalRoutingObserver
void CTmsAudioServicesTestClass::GlobalRoutingEvent(
        const TMSGlobalRouting& /*routing*/, TMSSignalEvent event,
        TMSAudioOutput /*output*/)
    {
    iLog->Log(_L("[tms cb]CTmsAudioServicesTestClass::GlobalRoutingEvent"));
    switch (event.type)
        {
        case TMS_EVENT_ROUTING_AVAIL_OUTPUTS_CHANGED:
            iLog->Log(_L("Available outputs changed"));
            break;
        case TMS_EVENT_ROUTING_OUTPUT_CHANGED:
            iLog->Log(_L("output changed"));
            ProcessEvent(EOutputChanged, KErrNone);
            break;
        case TMS_EVENT_ROUTING_SET_OUTPUT_COMPLETE:
            iLog->Log(_L("set output complete"));
            ProcessEvent(ESetOutputComplete, KErrNone);
            break;
        default:
            break;
        }
    }

// From TMSRingToneObserver
void CTmsAudioServicesTestClass::RingtoneEvent(const TMSRingTone& /*rt*/,
        TMSSignalEvent event)
    {
    iLog->Log(_L("[tms cb]CTmsAudioServicesTestClass::RingtoneEvent"));
    gint reason = event.reason;
    switch (event.type)
        {
        case TMS_EVENT_RINGTONE_OPEN_COMPLETE:
            iLog->Log(_L("[tms cb]RingtoneEvent ERTInitComplete"));
            ProcessEvent(ERTInitComplete, KErrNone);
            break;
        case TMS_EVENT_RINGTONE_PLAY_COMPLETE:
            iLog->Log(_L("[tms cb]RingtoneEvent ERTPlayComplete"));
            ProcessEvent(ERTPlayComplete, KErrNone);
            break;
        case TMS_EVENT_RINGTONE_DEINIT_COMPLETE:
            iLog->Log(_L("[tms cb]RingtoneEvent ERTDeinitComplete"));
            ProcessEvent(ERTDeinitComplete, KErrNone);
            break;
        default:
            break;
        }
    }

// From TMSDTMFObserver
void CTmsAudioServicesTestClass::DTMFEvent(const TMSDTMF& /*dtmf*/,
        TMSSignalEvent event)
    {
    iLog->Log(_L("[tms cb]CTmsAudioServicesTestClass::DTMFEvent"));
    gint reason = event.reason;
    switch (event.type)
        {
        case TMS_EVENT_DTMF_TONE_STARTED:
            //ProcessEvent(EDTMFToneStarted, KErrNone);
            iLog->Log(_L("DTMF Tone Started"));
            break;
        case TMS_EVENT_DTMF_TONE_STOPPED:
            ProcessEvent(EDTMFToneStopped, KErrNone);
            iLog->Log(_L("DTMF Tone Stopped"));
            break;
        default:
            break;
        }
    }

// From TMSInbandToneObserver
void CTmsAudioServicesTestClass::InbandToneEvent(
        const TMSInbandTone& /*inbandtone*/, TMSSignalEvent event)
    {
    iLog->Log(_L("[tms cb]CTmsAudioServicesTestClass::InbandToneEvent"));
    gint reason = event.reason;
    switch (event.type)
        {
        case TMS_EVENT_INBAND_TONE_STARTED:
            ProcessEvent(EInbToneStarted, KErrNone);
            iLog->Log(_L("Inband Tone Started"));
            break;
        case TMS_EVENT_INBAND_TONE_STOPPED:
            ProcessEvent(EInbToneStopped, KErrNone);
            iLog->Log(_L("Inband Tone Stopped"));
            break;
        default:
            break;
        }
    }

