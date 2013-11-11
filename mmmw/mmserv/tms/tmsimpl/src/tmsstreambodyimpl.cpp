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

#include <stdio.h>
#include <tms.h>
#include <tmsstreamobsrvr.h>
#include <tmsbuffer.h>
#include <tmseffect.h>
#include <tmsformat.h>
#include "tmsutility.h"
#include "tmscallproxy.h"
#include "tmsclientserver.h"
#include "tmsshared.h"
#include "tmsstreambodyimpl.h"
#include "tmsmembuffer.h"
#include "tmsclientsourceimpl.h"
#include "tmsclientsinkimpl.h"
#include "tmsmodemsourceimpl.h"
#include "tmsmodemsinkimpl.h"
#include "tmsmicsourceimpl.h"
#include "tmsspeakersinkimpl.h"
#include "tmsvolumeeffectimpl.h"
#include "tmsgaineffectimpl.h"
#include "tmsg711formatimpl.h"
#include "tmsg729formatimpl.h"
#include "tmsilbcformatimpl.h"
#include "tmspcmformatimpl.h"
#include "tmsamrformatimpl.h"

using namespace TMS;

TMSStreamBodyImpl::TMSStreamBodyImpl() :
    iProxy(NULL),
    iParent(NULL),
    iSource(NULL),
    iSink(NULL),
    iMsgQHandler(NULL),
    iObserver(NULL),
    iFormat(NULL),
    iStreamState(TMS_STREAM_UNINITIALIZED)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

TMSStreamBodyImpl::~TMSStreamBodyImpl()
    {
    TRACE_PRN_FN_ENT;
    if (iMsgQHandler)
        {
        iMsgQHandler->Cancel();
        }
    iMsgQHandler->RemoveObserver(*this);
    delete iMsgQHandler;
    if (iMsgQueue.Handle() > 0)
        {
        iMsgQueue.Close();
        }

    iEffectsList.Reset();

    if (iContext.CallProxy && (iContext.StreamId != -1))
        {
        iContext.CallProxy->DeleteStream(iContext.CallType,
                iContext.StreamType, iContext.StreamId);
        }
    TRACE_PRN_FN_EXT;
    }

gint TMSStreamBodyImpl::Create(TMSCallType callType, TMSStreamType stype,
        TMSCallProxy* proxy, TMSStream& parent, TMSStreamBody*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSStreamBodyImpl* self = new TMSStreamBodyImpl();
    if (self)
        {
        ret = self->PostConstruct(callType, stype, proxy, parent);
        if (ret != TMS_RESULT_SUCCESS)
            {
            delete self;
            self = NULL;
            }
        }
    bodyimpl = self;
    return ret;
    }

gint TMSStreamBodyImpl::PostConstruct(TMSCallType callType,
        TMSStreamType stype, TMSCallProxy* proxy, TMSStream& parent)
    {
    gint status(TMS_RESULT_SUCCESS);
    iContext.CallProxy = proxy;
    iParent = &parent;
    iContext.CallType = callType;
    iContext.StreamType = stype;

    status = CreateQueue(KTMSMsgQSlots);

    if (status == TMS_RESULT_SUCCESS)
        {
        if (iContext.CallProxy)
            {
            status = (iContext.CallProxy)->CreateStream(iContext.CallType,
                    iContext.StreamType, iContext.StreamId);
            }
        else
            {
            status = TMS_RESULT_UNINITIALIZED_OBJECT;
            }
        }
    return status;
    }

gint TMSStreamBodyImpl::AddObserver(TMSStreamObserver& obsrvr,
        gpointer user_data)
    {
    gint ret(TMS_RESULT_SUCCESS);
    // TODO allow support for multiple observers
    if (!iObserver)
        {
        iObserver = &obsrvr;
        iUserData = user_data;
        }
    else
        {
        ret = TMS_RESULT_ALREADY_EXIST;
        }
    return ret;
    }

gint TMSStreamBodyImpl::RemoveObserver(TMSStreamObserver& obsrvr)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (&obsrvr == iObserver)
        {
        iObserver = NULL;
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSStreamBodyImpl::AddSource(TMSSource* source)
    {
    gint ret(TMS_RESULT_SUCCESS);

    if (iStreamState == TMS_STREAM_UNINITIALIZED)
        {
        if (!iSource)
            {
            ret = ValidateSource(source);
            }
        else
            {
            ret = TMS_RESULT_ALREADY_EXIST;
            }
        }
    else
        {
        ret = TMS_RESULT_INVALID_STATE;
        }
    return ret;
    }

gint TMSStreamBodyImpl::RemoveSource(TMSSource* source)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iStreamState == TMS_STREAM_UNINITIALIZED)
        {
        if (source == iSource)
            {
            iSource = NULL;
            }
        else
            {
            ret = TMS_RESULT_DOES_NOT_EXIST;
            }
        }
    else
        {
        ret = TMS_RESULT_INVALID_STATE;
        }
    return ret;
    }

gint TMSStreamBodyImpl::AddSink(TMSSink* sink)
    {
    gint ret(TMS_RESULT_SUCCESS);

    if (iStreamState == TMS_STREAM_UNINITIALIZED)
        {
        if (!iSink)
            {
            ret = ValidateSink(sink);
            }
        else
            {
            ret = TMS_RESULT_ALREADY_EXIST;
            }
        }
    else
        {
        ret = TMS_RESULT_INVALID_STATE;
        }
    return ret;
    }

gint TMSStreamBodyImpl::RemoveSink(TMSSink* sink)
    {
    gint ret(TMS_RESULT_SUCCESS);

    if (iStreamState == TMS_STREAM_UNINITIALIZED)
        {
        if (sink == iSink)
            {
            iSink = NULL;
            }
        else
            {
            ret = TMS_RESULT_DOES_NOT_EXIST;
            }
        }
    else
        {
        ret = TMS_RESULT_INVALID_STATE;
        }
    return ret;
    }

gint TMSStreamBodyImpl::SetFormat(TMSFormat* format)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iStreamState == TMS_STREAM_UNINITIALIZED)
        {
        ret = ValidateFormat(format);
        }
    else
        {
        ret = TMS_RESULT_INVALID_STATE;
        }
    return ret;
    }

gint TMSStreamBodyImpl::ResetFormat(TMSFormat* format)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iStreamState == TMS_STREAM_UNINITIALIZED)
        {
        if (format == iFormat)
            {
            iFormat = NULL;
            }
        else
            {
            ret = TMS_RESULT_DOES_NOT_EXIST;
            }
        }
    else
        {
        ret = TMS_RESULT_INVALID_STATE;
        }
    return ret;
    }

gint TMSStreamBodyImpl::AddEffect(TMSEffect* effect)
    {
    gint ret(TMS_RESULT_SUCCESS);

    if (iStreamState == TMS_STREAM_UNINITIALIZED)
        {
        // Add to list if effect is not already added
        ret = iEffectsList.Find(effect);
        if (ret == KErrNotFound)
            {
            ret = iEffectsList.Append(effect);
            }
        else
            {
            ret = TMS_RESULT_ALREADY_EXIST;
            }
        }
    else
        {
        ret = TMS_RESULT_INVALID_STATE;
        }
    return ret;
    }

gint TMSStreamBodyImpl::RemoveEffect(TMSEffect* effect)
    {
    TInt status(TMS_RESULT_SUCCESS);

    if (iStreamState == TMS_STREAM_UNINITIALIZED)
        {
        TInt index = iEffectsList.Find(effect);
        // If found status has index to the effect in the array
        // else it would contain KErrNotFound
        if (index >= 0)
            {
            iEffectsList.Remove(index);
            }
        else
            {
            status = TMS_RESULT_DOES_NOT_EXIST;
            }
        }
    else
        {
        status = TMS_RESULT_INVALID_STATE;
        }
    return status;
    }

TMSStreamState TMSStreamBodyImpl::GetState()
    {
    return iStreamState;
    }

TMSStreamType TMSStreamBodyImpl::GetStreamType()
    {
    return iContext.StreamType;
    }

gint TMSStreamBodyImpl::GetStreamId()
    {
    return iContext.StreamId;
    }

gint TMSStreamBodyImpl::Init(gint retrytime)
    {
    gint ret(TMS_RESULT_SUCCESS);

    ret = ValidateStream();

    RET_REASON_IF_ERR(ret);

    TMSFormatType fmttype(TMS_FORMAT_PCM);
    if (!iFormat && iContext.CallType == TMS_CALL_IP)
        {
        return TMS_RESULT_FORMAT_TYPE_UNSPECIFIED;
        }
    else if (iContext.CallType == TMS_CALL_IP)
        {
        iFormat->GetType(fmttype);
        }

    if (iContext.CallProxy)
        {
        ret = (iContext.CallProxy)->InitStream(iContext.CallType,
                iContext.StreamType, iContext.StreamId, fmttype, &iMsgQueue,
                retrytime);
        }
    else
        {
        ret = TMS_RESULT_UNINITIALIZED_OBJECT;
        }

    RET_REASON_IF_ERR(ret);

    if (iEffectsList.Count())
        {
        for (TInt i = 0; i < iEffectsList.Count(); i++)
            {
            TMSEffectType effecttype;
            iEffectsList[i]->GetType(effecttype);
            switch (effecttype)
                {
                case TMS_EFFECT_GAIN:
                    static_cast<TMSGainEffectImpl*> (iEffectsList[i])->SetProxy(
                            iContext.CallProxy, iMsgQHandler);
                    break;
                case TMS_EFFECT_VOLUME:
                    static_cast<TMSVolumeEffectImpl*>
                        (iEffectsList[i])->SetProxy(iContext.CallProxy,
                        iMsgQHandler);
                    break;
                default:
                    ret = TMS_RESULT_INVALID_ARGUMENT;
                    break;
                }
            }
        }

    if (iFormat)
        {
        TMSFormatType fmttype;
        iFormat->GetType(fmttype);
        switch (fmttype)
            {
            case TMS_FORMAT_PCM:
                static_cast<TMSPCMFormatImpl*> (iFormat)->SetProxy(&iContext,
                        iMsgQHandler);
                break;
            case TMS_FORMAT_AMR:
                static_cast<TMSAMRFormatImpl*> (iFormat)->SetProxy(&iContext,
                        iMsgQHandler);
                break;
            case TMS_FORMAT_G711:
                static_cast<TMSG711FormatImpl*> (iFormat)->SetProxy(&iContext,
                        iMsgQHandler);
                break;
            case TMS_FORMAT_G729:
                static_cast<TMSG729FormatImpl*> (iFormat)->SetProxy(&iContext,
                        iMsgQHandler);
                break;
            case TMS_FORMAT_ILBC:
                static_cast<TMSILBCFormatImpl*> (iFormat)->SetProxy(&iContext,
                        iMsgQHandler);
                break;
            default:
                ret = TMS_RESULT_INVALID_ARGUMENT;
                break;
            }
        }

    return ret;
    }

gint TMSStreamBodyImpl::Pause()
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iContext.CallProxy)
        {
        ret = (iContext.CallProxy)->PauseStream(iContext.CallType,
                iContext.StreamType, iContext.StreamId);
        }
    else
        {
        ret = TMS_RESULT_UNINITIALIZED_OBJECT;
        }
    return ret;
    }

gint TMSStreamBodyImpl::Start(gint retrytime)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iContext.CallProxy)
        {
        ret = (iContext.CallProxy)->StartStream(iContext.CallType,
                iContext.StreamType, iContext.StreamId, retrytime);
        }
    else
        {
        ret = TMS_RESULT_UNINITIALIZED_OBJECT;
        }
    return ret;
    }

gint TMSStreamBodyImpl::Stop()
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iContext.CallProxy)
        {
        ret = (iContext.CallProxy)->StopStream(iContext.CallType,
                iContext.StreamType, iContext.StreamId);
        }
    else
        {
        ret = TMS_RESULT_UNINITIALIZED_OBJECT;
        }
    return ret;
    }

void TMSStreamBodyImpl::Deinit()
    {
    if (iContext.CallProxy)
        {
        (iContext.CallProxy)->DeinitStream(iContext.CallType,
                iContext.StreamType, iContext.StreamId);
        }
    }

gint TMSStreamBodyImpl::ValidateSource(TMSSource* source)
    {
    gint ret(TMS_RESULT_NULL_ARGUMENT);
    TMSSourceType sourcetype;

    if (source)
        {
        source->GetType(sourcetype);

        switch (sourcetype)
            {
            case TMS_SOURCE_CLIENT:
                {
                if (iContext.CallType == TMS_CALL_IP &&
                        iContext.StreamType == TMS_STREAM_DOWNLINK)
                    {
                    iSource = source;
                    static_cast<TMSClientSourceImpl*> (iSource)->SetProxy(
                             &iContext, iMsgQHandler);
                    ret = TMS_RESULT_SUCCESS;
                    }
                }
                break;
            case TMS_SOURCE_MIC:
                {
                if (iContext.StreamType == TMS_STREAM_UPLINK)
                    {
                    iSource = source;
                    static_cast<TMSMicSourceImpl*> (iSource)->SetProxy(
                             iContext.CallProxy, iMsgQHandler);
                    ret = TMS_RESULT_SUCCESS;
                    }
                }
                break;
            case TMS_SOURCE_MODEM:
                {
                if (iContext.CallType == TMS_CALL_CS &&
                        iContext.StreamType == TMS_STREAM_DOWNLINK)
                    {
                    iSource = source;
                    static_cast<TMSModemSourceImpl*> (iSource)->SetProxy(
                              iContext.CallProxy, iMsgQHandler);
                    ret = TMS_RESULT_SUCCESS;
                    }
                break;
                }
            default:
                break;
            }
        }

    return ret;
    }

gint TMSStreamBodyImpl::ValidateSink(TMSSink* sink)
    {
    gint ret(TMS_RESULT_NULL_ARGUMENT);
    TMSSinkType sinktype;

    if (sink)
        {
        sink->GetType(sinktype);

        switch (sinktype)
            {
            case TMS_SINK_CLIENT:
                {
                if (iContext.CallType == TMS_CALL_IP &&
                        iContext.StreamType == TMS_STREAM_UPLINK)
                    {
                    iSink = sink;
                    static_cast<TMSClientSinkImpl*> (iSink)->SetProxy(&iContext,
                            iMsgQHandler);
                    ret = TMS_RESULT_SUCCESS;
                    }
                }
                break;
            case TMS_SINK_SPEAKER:
                {
                if (iContext.StreamType == TMS_STREAM_DOWNLINK)
                    {
                    iSink = sink;
                    static_cast<TMSSpeakerSinkImpl*> (iSink)->SetProxy(iContext.CallProxy,
                             iMsgQHandler);
                    ret = TMS_RESULT_SUCCESS;
                    }
                }
                break;
            case TMS_SINK_MODEM:
                {
                if (iContext.CallType == TMS_CALL_CS &&
                        iContext.StreamType == TMS_STREAM_UPLINK)
                    {
                    iSink = sink;
                    static_cast<TMSModemSinkImpl*> (iSink)->SetProxy(iContext.CallProxy,
                             iMsgQHandler);
                    ret = TMS_RESULT_SUCCESS;
                    }
                }
                break;
            default:
                break;
            }
        }

    return ret;
    }

gint TMSStreamBodyImpl::ValidateFormat(TMSFormat* format)
    {
    gint ret(TMS_RESULT_NULL_ARGUMENT);

    if (format)
        {
        if (iContext.CallType == TMS_CALL_IP)
            {
            iFormat = format;
            ret = TMS_RESULT_SUCCESS;
            }
        else
            {
            ret = TMS_RESULT_INVALID_STATE;
            }
        }
    return ret;
    }

gint TMSStreamBodyImpl::ValidateStream()
    {
    gint ret(TMS_RESULT_INVALID_STATE);

    if (iContext.StreamType == TMS_STREAM_UPLINK ||
            iContext.StreamType == TMS_STREAM_DOWNLINK)
        {
        if (iContext.CallType == TMS_CALL_IP)
            {
            if (ValidateSource(iSource) == TMS_RESULT_SUCCESS &&
                    ValidateSink(iSink) == TMS_RESULT_SUCCESS &&
                    ValidateFormat(iFormat) == TMS_RESULT_SUCCESS)
                {
                ret = TMS_RESULT_SUCCESS;
                }
            }
        else if (iContext.CallType == TMS_CALL_CS)
            {
            if (ValidateSource(iSource) == TMS_RESULT_SUCCESS &&
                    ValidateSink(iSink) == TMS_RESULT_SUCCESS)
                {
                ret = TMS_RESULT_SUCCESS;
                }
            }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TMSStreamBodyImpl::CreateQueue
// ---------------------------------------------------------------------------
//
gint TMSStreamBodyImpl::CreateQueue(const gint aNumSlots)
    {
    gint err = TMS_RESULT_INVALID_ARGUMENT;

    if (iMsgQueue.Handle() <= 0)
        {
        err = iMsgQueue.CreateGlobal(KNullDesC, aNumSlots);
        if (err == KErrNone)
            {
            TRAP(err, ReceiveMsgQHandlerEventsL());
            }
        }

    return err;
    }

// ---------------------------------------------------------------------------
// TMSStreamBodyImpl::ReceiveMsgQHandlerEventsL
// Starts message queue handler (A/O) to monitor communication and transfer
// buffer events between client and the server.
// ---------------------------------------------------------------------------
//
void TMSStreamBodyImpl::ReceiveMsgQHandlerEventsL()
    {
    if (iMsgQHandler)
        {
        iMsgQHandler->Cancel();
        }
    else
        {
        iMsgQHandler = TMSQueueHandler::NewL(&iMsgQueue, &iContext);
        iMsgQHandler->AddObserver(*this, iContext.StreamType);
        }

    iMsgQHandler->Start();
    }

// ======== CALLBACK FUNCTIONS ========

void TMSStreamBodyImpl::QueueEvent(TInt aEventType, TInt aError,
        void* user_data)
    {
    TMSSignalEvent event = {}; //all elements initialized to zeros
    event.reason = TMSRESULT(aError);
    event.user_data = user_data;
    iStreamState = aEventType;

    switch (aEventType)
        {
        case TMS_STREAM_INITIALIZED:
        case TMS_STREAM_UNINITIALIZED:
        case TMS_STREAM_PAUSED:
        case TMS_STREAM_STARTED:
            {
            event.curr_state = aEventType;
            event.prev_state = iPrevState;
            if (iObserver)
                {
                if (aError != TMS_RESULT_SUCCESS)
                    {
                    event.type = TMS_EVENT_STREAM_STATE_CHANGE_ERROR;
                    }
                else
                    {
                    event.type = TMS_EVENT_STREAM_STATE_CHANGED;
                    }
                iObserver->TMSStreamEvent(*iParent, event);
                }
            break;
            }
        default:
            break;
        }
    iPrevState = aEventType;
    }

