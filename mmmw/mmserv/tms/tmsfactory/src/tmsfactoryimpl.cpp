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

#include "tmscallimpl.h"
#include "tmsformatimpl.h"
#include "tmseffectimpl.h"
#include "tmsbufferimpl.h"
#include "tmssourceimpl.h"
#include "tmssinkimpl.h"
#include "tmsglobalroutingimpl.h"
#include "tmsringtoneimpl.h"
#include "tmsdtmfimpl.h"
#include "tmsinbandtoneimpl.h"
#include "tmsutility.h"
#include "tmsproxy.h"
#include "tmsfactoryimpl.h"

using namespace TMS;

TMSFactoryImpl::TMSFactoryImpl(TMSVer& /*ver*/)
    {
    iFormats.Reset();
    }

TMSFactoryImpl::~TMSFactoryImpl()
    {
    iFormats.Reset();
    iFormats.Close();
    }

gint TMSFactoryImpl::CreateCall(TMSCallType ctype, TMSCall*& tmscall,
        guint ctxid)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSCallImpl::Create(ctype, tmscall, ctxid);
    return ret;
    }

gint TMSFactoryImpl::DeleteCall(TMSCall*& tmscall)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSCallImpl::Delete(tmscall);
    return ret;
    }

gint TMSFactoryImpl::IsCallTypeSupported(TMSCallType ctype, gboolean& flag)
    {
    gint ret(TMS_RESULT_SUCCESS);

    switch (ctype)
        {
        case TMS_CALL_CS:
        case TMS_CALL_IP:
            flag = ETrue;
            break;
        default:
            flag = EFalse;
            break;
        }
    return ret;
    }

gint TMSFactoryImpl::GetSupportedFormats(const TMSStreamType strmtype,
        FormatVector& fmtlist)
    {
    gint ret(TMS_RESULT_SUCCESS);
    gint fmtCount = iFormats.Count();
    if (fmtCount <= 0)
        {
        iFormats.Reset();
        ret = QueryDevice(strmtype);
        fmtCount = iFormats.Count();
        }
    if (ret == TMS_RESULT_SUCCESS)
        {
        TMSFormatType fmtype = NULL;
        TMSFormat* format(NULL);
        for (gint i = 0; i < fmtCount; i++)
            {
            fmtype = TOTMSFORMAT(iFormats[i]);
            if (fmtype)
                {
                format = NULL;
                ret = CreateFormat(fmtype, format);
                if (ret == TMS_RESULT_SUCCESS && format)
                    {
                    fmtlist.push_back(format);
                    }
                }
            }
        }
    return ret;
    }

gint TMSFactoryImpl::CreateFormat(TMSFormatType fmttype, TMSFormat*& tmsfmt)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSFormatImpl::Create(fmttype, tmsfmt);
    return ret;
    }

gint TMSFactoryImpl::DeleteFormat(TMSFormat*& tmsfmt)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSFormatImpl::Delete(tmsfmt);
    return ret;
    }

gint TMSFactoryImpl::CreateEffect(TMSEffectType tmseffecttype,
        TMSEffect*& tmseffect)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSEffectImpl::Create(tmseffecttype, tmseffect);
    return ret;
    }

gint TMSFactoryImpl::DeleteEffect(TMSEffect*& tmseffect)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSEffectImpl::Delete(tmseffect);
    return ret;
    }

gint TMSFactoryImpl::CreateBuffer(TMSBufferType buffertype, guint size,
        TMSBuffer*& tmsbuffer)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSBufferImpl::Create(buffertype, size, tmsbuffer);
    return ret;
    }

gint TMSFactoryImpl::DeleteBuffer(TMSBuffer*& tmsbuffer)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSBufferImpl::Delete(tmsbuffer);
    return ret;
    }

gint TMSFactoryImpl::CreateSource(TMSSourceType srctype, TMSSource*& tmssrc)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSSourceImpl::Create(srctype, tmssrc);
    return ret;
    }

gint TMSFactoryImpl::DeleteSource(TMSSource*& tmssrc)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSSourceImpl::Delete(tmssrc);
    return ret;
    }

gint TMSFactoryImpl::CreateSink(TMSSinkType sinktype, TMSSink*& tmssink)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSSinkImpl::Create(sinktype, tmssink);
    return ret;
    }

gint TMSFactoryImpl::DeleteSink(TMSSink*& tmssink)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSSinkImpl::Delete(tmssink);
    return ret;
    }

gint TMSFactoryImpl::CreateGlobalRouting(TMSGlobalRouting*& globrouting)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSGlobalRoutingImpl::Create(globrouting);
    return ret;
    }

gint TMSFactoryImpl::DeleteGlobalRouting(TMSGlobalRouting*& globrouting)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSGlobalRoutingImpl::Delete(globrouting);
    return ret;
    }

gint TMSFactoryImpl::CreateDTMF(TMSStreamType streamtype, TMSDTMF*& dtmf)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSDTMFImpl::Create(streamtype, dtmf);
    return ret;
    }

gint TMSFactoryImpl::DeleteDTMF(TMSDTMF*& dtmf)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSDTMFImpl::Delete(dtmf);
    return ret;
    }

gint TMSFactoryImpl::CreateRingTonePlayer(TMSRingTone*& rt)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSRingToneImpl::Create(rt);
    return ret;
    }

gint TMSFactoryImpl::CreateRingTonePlayer(TMSRingTone*& rt, RWindow& window,
        gint scrid)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSRingToneImpl::Create(rt, window, scrid);
    return ret;
    }

gint TMSFactoryImpl::DeleteRingTonePlayer(TMSRingTone*& rt)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSRingToneImpl::Delete(rt);
    return ret;
    }

gint TMSFactoryImpl::CreateInbandTonePlayer(TMSInbandTone*& inbandtone)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSInbandToneImpl::Create(inbandtone);
    return ret;
    }

gint TMSFactoryImpl::DeleteInbandTonePlayer(TMSInbandTone*& inbandtone)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSInbandToneImpl::Delete(inbandtone);
    return ret;
    }

gint TMSFactoryImpl::QueryDevice(const TMSStreamType strmtype)
    {
    gint ret(TMS_RESULT_SUCCESS);
    TMSProxy* session = new TMSProxy();
    if (session)
        {
        if (session->Connect() == TMS_RESULT_SUCCESS)
            {
            if (strmtype == TMS_STREAM_UPLINK)
                {
                ret = session->GetSupportedEncoders(iFormats, iFrameSize);
                }
            else if (strmtype == TMS_STREAM_DOWNLINK)
                {
                ret = session->GetSupportedDecoders(iFormats, iFrameSize);
                }
            else
                {
                ret = TMS_RESULT_STREAM_TYPE_NOT_SUPPORTED;
                }
            }
        delete session;
        session = NULL;
        }
    else
        {
        ret = TMS_RESULT_INSUFFICIENT_MEMORY;
        }
    return ret;
    }

// End of file
