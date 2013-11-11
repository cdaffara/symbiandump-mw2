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
#include "tmsutility.h"
#include "tmsclientserver.h"
#include "tmsshared.h"
#include "tmscallipadpt.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::NewL
// Symbian constructor.
// -----------------------------------------------------------------------------
//
TMSCallIPAdpt* TMSCallIPAdpt::NewL()
    {
    TMSCallIPAdpt* self = new (ELeave) TMSCallIPAdpt();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::ConstructL
// 2-nd phase constructor.
// -----------------------------------------------------------------------------
//
void TMSCallIPAdpt::ConstructL()
    {
    TRACE_PRN_FN_ENT;
    iIPDownlink = NULL;
    iIPUplink = NULL;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::TMSCallIPAdpt
//
// -----------------------------------------------------------------------------
//
TMSCallIPAdpt::TMSCallIPAdpt()
    {
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::~TMSCallIPAdpt
//
// -----------------------------------------------------------------------------
//
TMSCallIPAdpt::~TMSCallIPAdpt()
    {
    TRACE_PRN_FN_ENT;

    iCodecs.Reset();
    iCodecs.Close();
    iArrBitrates.Reset();
    delete iIPUplink;
    delete iIPDownlink;

    if (iMsgQueueUp.Handle() > 0)
        {
        iMsgQueueUp.Close();
        }
    if (iMsgQueueDn.Handle() > 0)
        {
        iMsgQueueDn.Close();
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::PostConstruct
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::PostConstruct()
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    iNextStreamId = 1;
    iUplState = EIdle;
    iDnlState = EIdle;
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::CreateStream
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::CreateStream(const TMSCallType /*callType*/,
        const TMSStreamType strmType, gint& outStrmId)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    switch (strmType)
        {
        case TMS_STREAM_UPLINK:
            {
            status = TMS_RESULT_ALREADY_EXIST;
            if (iUplState == EIdle)
                {
                iUplinkStreamId = iNextStreamId;
                outStrmId = iUplinkStreamId;
                iNextStreamId++;
                status = TMS_RESULT_SUCCESS;
                }
            break;
            }
        case TMS_STREAM_DOWNLINK:
            {
            status = TMS_RESULT_ALREADY_EXIST;
            if (iDnlState == EIdle)
                {
                iDnlinkStreamId = iNextStreamId;
                outStrmId = iDnlinkStreamId;
                iNextStreamId++;
                status = TMS_RESULT_SUCCESS;
                }
            break;
            }
        default:
            {
            status = TMS_RESULT_STREAM_TYPE_NOT_SUPPORTED;
            break;
            }
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::InitStream
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::InitStream(const TMSCallType /*callType*/,
        const TMSStreamType strmType, const gint strmId,
        const TMSFormatType frmtType, const gint retrytime,
        const RMessage2& message)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    guint32 fourCC = TOFOURCC(frmtType);
    if (fourCC == NULL)
        {
        return TMS_RESULT_INVALID_ARGUMENT;
        }

    switch (strmType)
        {
        case TMS_STREAM_UPLINK:
            {
            status = TMS_RESULT_DOES_NOT_EXIST;
            if (strmId == iUplinkStreamId)
                {
                SetFormat(iUplinkStreamId, fourCC);
                status = OpenUplink(message, retrytime);
                }
            break;
            }
        case TMS_STREAM_DOWNLINK:
            {
            status = TMS_RESULT_DOES_NOT_EXIST;
            if (strmId == iDnlinkStreamId)
                {
                SetFormat(iDnlinkStreamId, fourCC);
                status = OpenDownlink(message, retrytime);
                }
            break;
            }
        default:
            {
            status = TMS_RESULT_STREAM_TYPE_NOT_SUPPORTED;
            break;
            }
        }

    TRACE_PRN_IF_ERR(status);
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::StartStream
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::StartStream(const TMSCallType /*callType*/,
        const TMSStreamType strmType, const gint strmId, const gint retrytime)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    switch (strmType)
        {
        case TMS_STREAM_UPLINK:
            {
            if (iIPUplink && strmId == iUplinkStreamId &&
                    iUplState == EInitialized)
                {
                iIPUplink->Start(retrytime);
                status = TMS_RESULT_SUCCESS;
                }
            break;
            }
        case TMS_STREAM_DOWNLINK:
            {
            if (iIPDownlink && strmId == iDnlinkStreamId &&
                    iDnlState == EInitialized)
                {
                iIPDownlink->Start(retrytime);
                status = TMS_RESULT_SUCCESS;
                }
            break;
            }
        default:
            {
            status = TMS_RESULT_STREAM_TYPE_NOT_SUPPORTED;
            break;
            }
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::PauseStream
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::PauseStream(const TMSCallType /*callType*/,
        const TMSStreamType strmType, const gint strmId)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    switch (strmType)
        {
        case TMS_STREAM_UPLINK:
            {
            if (strmId == iUplinkStreamId && iIPUplink)
                {
                iIPUplink->Stop();
                iUplState = EInitialized;
                status = TMS_RESULT_SUCCESS;
                NotifyClient(iUplinkStreamId, ECmdUplinkPaused, status);
                }
            break;
            }
        case TMS_STREAM_DOWNLINK:
            {
            if (strmId == iDnlinkStreamId && iIPDownlink)
                {
                iIPDownlink->Stop();
                iDnlState = EInitialized;
                status = TMS_RESULT_SUCCESS;
                NotifyClient(iDnlinkStreamId, ECmdDownlinkPaused, status);
                }
            break;
            }
        default:
            {
            status = TMS_RESULT_STREAM_TYPE_NOT_SUPPORTED;
            break;
            }
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::StopStream
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::StopStream(const TMSCallType /*callType*/,
        const TMSStreamType strmType, const gint strmId)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);

    switch (strmType)
        {
        case TMS_STREAM_UPLINK:
            {
            if (iIPUplink && strmId == iUplinkStreamId &&
                    iUplState == EActivated)
                {
                iIPUplink->Stop();
                iUplState = EInitialized;
                status = TMS_RESULT_SUCCESS;
                NotifyClient(iUplinkStreamId, ECmdUplinkInitComplete, status);
                }
            break;
            }
        case TMS_STREAM_DOWNLINK:
            {
            if (iIPDownlink && strmId == iDnlinkStreamId &&
                    iDnlState == EActivated)
                {
                iIPDownlink->Stop();
                iDnlState = EInitialized;
                status = TMS_RESULT_SUCCESS;
                NotifyClient(iDnlinkStreamId, ECmdDownlinkInitComplete, status);
                }
            break;
            }
        default:
            {
            status = TMS_RESULT_STREAM_TYPE_NOT_SUPPORTED;
            break;
            }
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::DeinitStream
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::DeinitStream(const TMSCallType /*callType*/,
        const TMSStreamType strmType, const gint strmId)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);

    switch (strmType)
        {
        case TMS_STREAM_UPLINK:
            {
            if (iIPUplink && strmId == iUplinkStreamId && iUplState != EIdle)
                {
                iIPUplink->Stop();
                iUplState = EIdle;
                status = TMS_RESULT_SUCCESS;
                NotifyClient(iUplinkStreamId, ECmdUplinkDeInitComplete, status);
                }
            break;
            }
        case TMS_STREAM_DOWNLINK:
            {
            if (iIPDownlink && strmId == iDnlinkStreamId && iDnlState != EIdle)
                {
                iIPDownlink->Stop();
                iDnlState = EIdle;
                status = TMS_RESULT_SUCCESS;
                NotifyClient(iDnlinkStreamId, ECmdDownlinkDeInitComplete,
                        status);
                }
            break;
            }
        default:
            {
            status = TMS_RESULT_STREAM_TYPE_NOT_SUPPORTED;
            break;
            }
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::DeleteStream
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::DeleteStream(const TMSCallType /*callType*/,
        const TMSStreamType strmType, const gint strmId)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    switch (strmType)
        {
        case TMS_STREAM_UPLINK:
            {
            if (strmId == iUplinkStreamId)
                {
                iUplinkStreamId = -1;
                iUplState = EIdle;
                }
            break;
            }
        case TMS_STREAM_DOWNLINK:
            {
            if (strmId == iDnlinkStreamId)
                {
                iDnlinkStreamId = -1;
                iDnlState = EIdle;
                }
            break;
            }
        default:
            {
            status = TMS_RESULT_STREAM_TYPE_NOT_SUPPORTED;
            break;
            }
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::DataXferBufferEmptied
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::DataXferBufferEmptied(const TMSCallType /*callType*/,
        const TMSStreamType strmType, const gint strmId)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    switch (strmType)
        {
        case TMS_STREAM_UPLINK:
            {
            if (strmId == iUplinkStreamId)
                {
                TRAP(status, BufferEmptiedL());
                }
            break;
            }
        case TMS_STREAM_DOWNLINK:
            status = TMS_RESULT_ILLEGAL_OPERATION;
            break;
        default:
            status = TMS_RESULT_STREAM_TYPE_NOT_SUPPORTED;
            break;
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::DataXferBufferFilled
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::DataXferBufferFilled(const TMSCallType /*callType*/,
        const TMSStreamType strmType, const gint strmId, const guint datasize)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    switch (strmType)
        {
        case TMS_STREAM_DOWNLINK:
            if (strmId == iDnlinkStreamId)
                {
                TRAP(status, BufferFilledL(datasize));
                }
            break;
        case TMS_STREAM_UPLINK:
            status = TMS_RESULT_ILLEGAL_OPERATION;
            break;
        default:
            status = TMS_RESULT_STREAM_TYPE_NOT_SUPPORTED;
            break;
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetDataXferBufferHndl
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetDataXferBufferHndl(const TMSCallType /*callType*/,
        const TMSStreamType strmType, const gint strmId, const guint32 key,
        RChunk& chunk)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    switch (strmType)
        {
        case TMS_STREAM_DOWNLINK:
            {
            if (strmId == iDnlinkStreamId)
                {
                TRAP(status, GetDataXferChunkHndl(strmType, key, chunk));
                }
            break;
            }
        case TMS_STREAM_UPLINK:
            {
            if (strmId == iUplinkStreamId)
                {
                TRAP(status, GetDataXferChunkHndl(strmType, key, chunk));
                }
            break;
            }
        default:
            status = TMS_RESULT_STREAM_TYPE_NOT_SUPPORTED;
            break;
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetMaxVolume
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetMaxVolume(guint& volume)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPDownlink && iDnlState != EIdle)
        {
        status = iIPDownlink->GetMaxVolume(volume);
        iMaxVolume = volume;
        TRACE_PRN_N1(_L("TMS->TMSCallIPAdpt: GetMaxVolume [%d]"), iMaxVolume);
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::SetVolume
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::SetVolume(const guint volume)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPDownlink && iDnlState != EIdle)
        {
        status = iIPDownlink->SetVolume(volume);
        NotifyClient(iDnlinkStreamId, ECmdSetVolume, status);
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetVolume
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetVolume(guint& volume)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPDownlink && iDnlState != EIdle)
        {
        status = iIPDownlink->GetVolume(volume);
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetMaxGain
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetMaxGain(guint& gain)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPUplink && iUplState != EIdle)
        {
        status = iIPUplink->GetMaxGain(gain);
        iMaxGain = gain;
        TRACE_PRN_N1(_L("TMS->TMSCallIPAdpt::GetMaxGain [%d]"), iMaxGain);
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::SetGain
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::SetGain(const guint gain)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPUplink && iUplState != EIdle)
        {
        status = iIPUplink->SetGain(gain);
        NotifyClient(iUplinkStreamId, ECmdSetGain, status);
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetGain
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetGain(guint& gain)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPUplink && iUplState != EIdle)
        {
        status = iIPUplink->GetGain(gain);
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetCodecMode
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetCodecMode(const TMSFormatType fmttype,
        const TMSStreamType strmtype, gint& mode)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    switch (fmttype)
        {
        case TMS_FORMAT_ILBC:
            status = GetIlbcCodecMode(mode, strmtype);
            break;
        case TMS_FORMAT_G711:
            status = GetG711CodecMode(mode, strmtype);
            break;
        default:
            status = TMS_RESULT_FORMAT_TYPE_NOT_SUPPORTED;
            break;
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::SetCodecMode
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::SetCodecMode(const TMSFormatType fmttype,
        const TMSStreamType strmtype, const gint mode)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    switch (fmttype)
        {
        case TMS_FORMAT_ILBC:
            status = SetIlbcCodecMode(mode, strmtype);
            break;
        case TMS_FORMAT_G711:
            status = SetG711CodecMode(mode, strmtype);
            break;
        default:
            status = TMS_RESULT_FORMAT_TYPE_NOT_SUPPORTED;
            break;
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetSupportedBitRatesCount
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetSupportedBitRatesCount(guint& count)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPUplink && iUplState != EIdle)
        {
        status = iIPUplink->GetSupportedBitrates(iArrBitrates);
        count = iArrBitrates.Count();
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetSupportedBitRates
//
// Bitrates are already returned from the codec as a result of call to
// GetSupportedBitratesCount(). Just pack them into a descriptor and return
// back to the client.
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetSupportedBitRates(CBufFlat*& brbuffer)
    {
    TRAPD(status, GetSupportedBitRatesL(brbuffer));
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetSupportedBitRatesL
//
// GetSupportedBitRates implementation which can leave.
// -----------------------------------------------------------------------------
//
void TMSCallIPAdpt::GetSupportedBitRatesL(CBufFlat*& brbuffer)
    {
    TRACE_PRN_FN_ENT;
    RBufWriteStream stream;
    stream.Open(*brbuffer);
    CleanupClosePushL(stream);
    guint numOfItems = iArrBitrates.Count();

    for (guint i = 0; i < numOfItems; i++)
        {
        stream.WriteUint32L(iArrBitrates[i]);
        //TRACE_PRN_N1(_L("TMS->TMSCallIPAdpt: BR: [%d]"), iArrBitrates[i]);
        }

    CleanupStack::PopAndDestroy(&stream);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetBitRate
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetBitRate(guint& bitrate)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPUplink && iUplState != EIdle)
        {
        status = iIPUplink->GetBitrate(bitrate);
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::SetBitRate
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::SetBitRate(const guint bitrate)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPUplink && iUplState != EIdle)
        {
        status = iIPUplink->SetBitrate(bitrate);
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetVAD
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetVAD(const TMSFormatType fmttype, gboolean& vad)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPUplink && iUplState != EIdle)
        {
        status = iIPUplink->GetVad(fmttype, vad);
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::SetVAD
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::SetVAD(const TMSFormatType fmttype, const gboolean vad)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPUplink && iUplState != EIdle)
        {
        status = iIPUplink->SetVad(fmttype, vad);
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetCNG
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetCNG(const TMSFormatType fmttype, gboolean& cng)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPDownlink && iDnlState == EInitialized)
        {
        status = iIPDownlink->GetCng(fmttype, cng);
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::SetCNG
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::SetCNG(const TMSFormatType fmttype, const gboolean cng)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPDownlink && iDnlState == EInitialized)
        {
        status = iIPDownlink->SetCng(fmttype, cng);
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetPlc
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetPlc(const TMSFormatType fmttype, gboolean& plc)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPDownlink && iDnlState != EIdle)
        {
        status = iIPDownlink->GetPlc(fmttype, plc);
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::SetPlc
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::SetPlc(const TMSFormatType fmttype, const gboolean plc)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPDownlink && iDnlState == EInitialized)
        {
        status = iIPDownlink->SetPlc(fmttype, plc);
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::OpenDownlink
// Method for player initialization.
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::OpenDownlink(const RMessage2& message, const gint retrytime)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);

    // Clients must have MultimediaDD capability to use this priority/pref.
    // TODO: Also, TMS will monitor for emergency call and if detected it
    //       will deny access to audio resources.

    iPriority.iPref = KAudioPrefVoipAudioDownlink;
    iPriority.iPriority = KAudioPriorityVoipAudioDownlink;

    delete iIPDownlink;
    iIPDownlink = NULL;
    TRAP(status, iIPDownlink = TMSIPDownlink::NewL(*this, iDnFourCC, iPriority,
            retrytime));

    if (iIPDownlink && status == TMS_RESULT_SUCCESS)
        {
        // Open message queue for handling server notifications to the client
        if (iMsgQueueDn.Handle() <= 0)
            {
            // Second argument in TMSCallProxy::InitStream
            status = iMsgQueueDn.Open(message, 1);
            }

        if (status == TMS_RESULT_SUCCESS)
            {
            // For transfer data buffer processing
            iIPDownlink->SetMsgQueue(iMsgQueueDn);
            }
        }
    TRACE_PRN_IF_ERR(status);
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::OpenUplink
// Method for recorder initialization.
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::OpenUplink(const RMessage2& message, const gint retrytime)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);

    // Clients must have MultimediaDD capability to use this priority/pref.
    // KAudioPrefVoipAudioUplinkNonSignal must be used to allow DTMF mixing.
    iPriority.iPref = KAudioPrefVoipAudioUplinkNonSignal;
    iPriority.iPriority = KAudioPriorityVoipAudioUplink;

    delete iIPUplink;
    iIPUplink = NULL;
    TRAP(status, iIPUplink = TMSIPUplink::NewL(*this, iUpFourCC, iPriority,
            retrytime));

    if (iIPUplink && status == TMS_RESULT_SUCCESS)
        {
        // Open message queue for handling server notifications to the client
        if (iMsgQueueUp.Handle() <= 0)
            {
            // Second argument in TMSCallProxy::InitStream
            status = iMsgQueueUp.Open(message, 1);
            }

        if (status == TMS_RESULT_SUCCESS)
            {
            // For transfer data buffer processing
            iIPUplink->SetMsgQueue(iMsgQueueUp);
            }
        }
    TRACE_PRN_IF_ERR(status);
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::SetFormat
//
// -----------------------------------------------------------------------------
//
void TMSCallIPAdpt::SetFormat(const gint strmId, const guint32 aFormat)
    {
    if (strmId == iUplinkStreamId)
        {
        iUpFourCC = aFormat;
        }
    else if (strmId == iDnlinkStreamId)
        {
        iDnFourCC = aFormat;
        }
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::BufferFilled
//
// -----------------------------------------------------------------------------
//
void TMSCallIPAdpt::BufferFilledL(guint dataSize)
    {
    if (iIPDownlink)
        {
        iIPDownlink->BufferFilled(dataSize);
        }
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::BufferEmptied
//
// -----------------------------------------------------------------------------
//
void TMSCallIPAdpt::BufferEmptiedL()
    {
    if (iIPUplink)
        {
        iIPUplink->BufferEmptied();
        }
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetDataXferChunkHndl
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetDataXferChunkHndl(const TMSStreamType strmType,
        const guint32 key, RChunk& chunk)
    {
    TRACE_PRN_FN_ENT;

    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    switch (strmType)
        {
        case TMS_STREAM_DOWNLINK:
            {
            if (iIPDownlink)
                {
                status = iIPDownlink->GetDataXferChunkHndl(key, chunk);
                }
            break;
            }
        case TMS_STREAM_UPLINK:
            {
            if (iIPUplink)
                {
                status = iIPUplink->GetDataXferChunkHndl(key, chunk);
                }
            break;
            }
        default:
            status = TMS_RESULT_STREAM_TYPE_NOT_SUPPORTED;
            break;
        }

    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::SetIlbcCodecMode
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::SetIlbcCodecMode(const gint mode,
        const TMSStreamType strmtype)
    {
    gint status(TMS_RESULT_SUCCESS);

    if (strmtype == TMS_STREAM_DOWNLINK && iIPDownlink &&
            iDnlState == EInitialized)
        {
        status = iIPDownlink->SetIlbcCodecMode(mode);
        }
    else if (strmtype == TMS_STREAM_UPLINK && iIPUplink &&
            iUplState == EInitialized)
        {
        status = iIPUplink->SetIlbcCodecMode(mode);
        }
    else
        {
        status = TMS_RESULT_INVALID_STATE;
        }
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetIlbcCodecMode
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetIlbcCodecMode(gint& mode, const TMSStreamType strmtype)
    {
    gint status(TMS_RESULT_SUCCESS);

    if (strmtype == TMS_STREAM_DOWNLINK && iIPDownlink &&
            iDnlState != EIdle)
        {
        status = iIPDownlink->GetIlbcCodecMode(mode);
        }
    else if (strmtype == TMS_STREAM_UPLINK && iIPUplink &&
            iUplState != EIdle)
        {
        status = iIPUplink->GetIlbcCodecMode(mode);
        }
    else
        {
        status = TMS_RESULT_INVALID_STATE;
        }
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::SetG711CodecMode
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::SetG711CodecMode(const gint mode,
        const TMSStreamType strmtype)
    {
    gint status(TMS_RESULT_SUCCESS);

    if (strmtype == TMS_STREAM_DOWNLINK && iIPDownlink &&
            iDnlState == EInitialized)
        {
        status = iIPDownlink->SetG711CodecMode(mode);
        }
    else if (strmtype == TMS_STREAM_UPLINK && iIPUplink &&
            iUplState == EInitialized)
        {
        status = iIPUplink->SetG711CodecMode(mode);
        }
    else
        {
        status = TMS_RESULT_INVALID_STATE;
        }
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetG711CodecMode
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetG711CodecMode(gint& mode, const TMSStreamType strmtype)
    {
    gint status(TMS_RESULT_SUCCESS);

    if (strmtype == TMS_STREAM_DOWNLINK && iIPDownlink &&
            iDnlState != EIdle)
        {
        status = iIPDownlink->GetG711CodecMode(mode);
        }
    else if (strmtype == TMS_STREAM_UPLINK && iIPUplink &&
            iUplState != EIdle)
        {
        status = iIPUplink->GetG711CodecMode(mode);
        }
    else
        {
        status = TMS_RESULT_INVALID_STATE;
        }
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::SetOutput
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::SetOutput(TMSAudioOutput output)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPDownlink && iDnlState != EIdle)
        {
        TRAP(status, iIPDownlink->SetAudioDeviceL(output));
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetOutput
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetOutput(TMSAudioOutput& output)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_STATE);
    if (iIPDownlink && iDnlState != EIdle)
        {
        TRAP(status, iIPDownlink->GetAudioDeviceL(output));
        }
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetPreviousOutput
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetPreviousOutput(TMSAudioOutput& /*output*/)
    {
    return TMS_RESULT_FEATURE_NOT_SUPPORTED;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::GetAvailableOutputsL
//
// -----------------------------------------------------------------------------
//
gint TMSCallIPAdpt::GetAvailableOutputsL(gint& count, CBufFlat*& outputsbuf)
    {
    RBufWriteStream stream;
    stream.Open(*outputsbuf);
    CleanupClosePushL(stream);
    stream.WriteUint32L(TMS_AUDIO_OUTPUT_PUBLIC);
    stream.WriteUint32L(TMS_AUDIO_OUTPUT_PRIVATE);
    CleanupStack::PopAndDestroy(&stream);
    count = 2;
    return TMS_RESULT_SUCCESS;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::DownlinkInitCompleted
// From TMSIPDevSoundObserver
// -----------------------------------------------------------------------------
//
void TMSCallIPAdpt::DownlinkInitCompleted(gint status)
    {
    TRACE_PRN_FN_ENT;
    if (status == TMS_RESULT_SUCCESS)
        {
        iDnlState = EInitialized;
        }
    NotifyClient(iDnlinkStreamId, ECmdDownlinkInitComplete, status);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::UplinkInitCompleted
// From TMSIPDevSoundObserver
// -----------------------------------------------------------------------------
//
void TMSCallIPAdpt::UplinkInitCompleted(gint status)
    {
    TRACE_PRN_FN_ENT;
    if (status == TMS_RESULT_SUCCESS)
        {
        iUplState = EInitialized;
        }
    NotifyClient(iUplinkStreamId, ECmdUplinkInitComplete, status);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::UplinkStarted
// From TMSIPDevSoundObserver
// -----------------------------------------------------------------------------
//
void TMSCallIPAdpt::UplinkStarted(gint status)
    {
    TRACE_PRN_FN_ENT;
    if (status == TMS_RESULT_SUCCESS)
        {
        iUplState = EActivated;
        }
    NotifyClient(iUplinkStreamId, ECmdUplinkStarted, status);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::DownlinkStarted
// From TMSIPDevSoundObserver
// -----------------------------------------------------------------------------
//
void TMSCallIPAdpt::DownlinkStarted(gint status)
    {
    TRACE_PRN_FN_ENT;
    if (status == TMS_RESULT_SUCCESS)
        {
        iDnlState = EActivated;
        }
    NotifyClient(iDnlinkStreamId, ECmdDownlinkStarted, status);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallIPAdpt::NotifyClient
// -----------------------------------------------------------------------------
//
void TMSCallIPAdpt::NotifyClient(const gint strmId, const gint command,
        const gint status, const gint64 /*int64*/)
    {
    iMsgBuffer.iRequest = command;
    iMsgBuffer.iStatus = status;

    if (strmId == iUplinkStreamId)
        {
        iMsgQueueUp.Send(iMsgBuffer);
        }
    else if (strmId == iDnlinkStreamId)
        {
        iMsgQueueDn.Send(iMsgBuffer);
        }
    }

