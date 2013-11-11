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

#include <s32mem.h>
#include <tmsbuffer.h>
#include "tmsutility.h"
#include "tmsproxy.h"
#include "tmscallclisrv.h"
#include "tmscallproxy.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// TMSCallProxy::TMSCallProxy
//
// -----------------------------------------------------------------------------
//
EXPORT_C TMSCallProxy::TMSCallProxy() :
    iTMSProxy(NULL)
    {
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::~TMSCallProxy
//
// -----------------------------------------------------------------------------
//
EXPORT_C TMSCallProxy::~TMSCallProxy()
    {
    TRACE_PRN_FN_ENT;
    this->Close();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::Connect
// Create a client-side session. Start the server if not started already.
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::Connect()
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    TRAP(status, iTMSProxy = new (ELeave) TMSProxy());
    if (status == TMS_RESULT_SUCCESS)
        {
        status = iTMSProxy->Connect();
        if (status == TMS_RESULT_SUCCESS)
            {
            status = SetReturnedHandle(iTMSProxy->GetTMSCallSessionHandle());
            }
        else
            {
            delete iTMSProxy;
            iTMSProxy = NULL;
            }
        }
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::CreateCall
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::CreateCall(gint calltype)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    status = RSessionBase::SendReceive(TMS_CREATE_CALL, TIpcArgs(calltype));
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::CreateStream
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::CreateStream(TMSCallType callType,
        TMSStreamType strmType, gint& strmId)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    TMSCliSrvStreamCreateDataStructBufPckg inPckg;
    inPckg().CallType = callType;
    inPckg().StreamType = strmType;
    TPckgBuf<gint> outPckg;
    status = RSessionBase::SendReceive(TMS_CREATE_STREAM, TIpcArgs(&inPckg,
            &outPckg));
    if (status == TMS_RESULT_SUCCESS)
        {
        strmId = outPckg();
        }

    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::InitStream
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::InitStream(const TMSCallType callType,
        const TMSStreamType strmType, gint strmId,
        const TMSFormatType frmtType, RHandleBase* msgQueue,
        const gint retrytime)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    TMSCliSrvStreamInitDataStructBufPckg inPckg;
    inPckg().CallType = callType;
    inPckg().StreamType = strmType;
    inPckg().StreamId = strmId;
    inPckg().FormatType = frmtType;
    inPckg().RetryTime = retrytime;
    status = RSessionBase::SendReceive(TMS_INIT_STREAM, TIpcArgs(&inPckg,
            *msgQueue));
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::StartStream
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::StartStream(const TMSCallType callType,
        const TMSStreamType strmType, gint strmId, const gint retrytime)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    TMSCliSrvStreamOpDataStructBufPckg inPckg;
    inPckg().CallType = callType;
    inPckg().StreamType = strmType;
    inPckg().StreamId = strmId;
    inPckg().RetryTime = retrytime;
    status = RSessionBase::SendReceive(TMS_START_STREAM, TIpcArgs(&inPckg));
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::PauseStream
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::PauseStream(const TMSCallType callType,
        const TMSStreamType strmType, gint strmId)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    TMSCliSrvStreamOpDataStructBufPckg inPckg;
    inPckg().CallType = callType;
    inPckg().StreamType = strmType;
    inPckg().StreamId = strmId;
    status = RSessionBase::SendReceive(TMS_PAUSE_STREAM, TIpcArgs(&inPckg));
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::StopStream
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::StopStream(const TMSCallType callType,
        const TMSStreamType strmType, gint strmId)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    TMSCliSrvStreamOpDataStructBufPckg inPckg;
    inPckg().CallType = callType;
    inPckg().StreamType = strmType;
    inPckg().StreamId = strmId;
    status = RSessionBase::SendReceive(TMS_STOP_STREAM, TIpcArgs(&inPckg));
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::DeinitStream
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::DeinitStream(const TMSCallType callType,
        const TMSStreamType strmType, gint strmId)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    TMSCliSrvStreamOpDataStructBufPckg inPckg;
    inPckg().CallType = callType;
    inPckg().StreamType = strmType;
    inPckg().StreamId = strmId;
    status = RSessionBase::SendReceive(TMS_DEINIT_STREAM, TIpcArgs(&inPckg));
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::DeleteStream
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::DeleteStream(const TMSCallType callType,
        const TMSStreamType strmType, gint& strmId)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    TMSCliSrvStreamOpDataStructBufPckg inPckg;
    inPckg().CallType = callType;
    inPckg().StreamType = strmType;
    inPckg().StreamId = strmId;
    status = RSessionBase::SendReceive(TMS_DELETE_STREAM, TIpcArgs(&inPckg));
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::DeleteCall
//
// -----------------------------------------------------------------------------
//
EXPORT_C void TMSCallProxy::DeleteCall()
    {
    TRACE_PRN_FN_ENT;
    RSessionBase::SendReceive(TMS_DELETE_CALL);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::Close
//
// -----------------------------------------------------------------------------
//
EXPORT_C void TMSCallProxy::Close()
    {
    TRACE_PRN_FN_ENT;
    if (Handle() > 0)
        {
        RSessionBase::Close();
        }

    if (iTMSProxy)
        {
        iTMSProxy->Close();
        delete iTMSProxy;
        iTMSProxy = NULL;
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::BufferEmptied
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::BufferEmptied(TMSCallType callType,
        TMSStreamType strmType, gint strmId, TMSBuffer& /*buffer*/)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    // For now we can use TMSCliSrvStreamOpDataStructBufPckg, there is
    // nothing that needs to be sent from the client side for buffer
    // emptied call
    TMSCliSrvDataXferDataStructBufPckg inPckg;
    inPckg().CallType = callType;
    inPckg().StreamType = strmType;
    inPckg().StreamId = strmId;
    status = RSessionBase::SendReceive(TMS_DATA_XFER_BUFFER_EMPTIED,
            TIpcArgs(&inPckg));

    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::BufferFilled
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::BufferFilled(TMSCallType callType,
        TMSStreamType strmType, gint strmId, TMSBuffer& buffer)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    // For now we can use TMSCliSrvStreamOpDataStructBufPckg, there is
    // nothing that needs to be sent from the client side for buffer
    // emptied call
    TMSCliSrvDataXferDataStructBufPckg inPckg;
    inPckg().CallType = callType;
    inPckg().StreamType = strmType;
    inPckg().StreamId = strmId;
    buffer.GetDataSize(inPckg().DataSize);
    status = RSessionBase::SendReceive(TMS_DATA_XFER_BUFFER_FILLED,
            TIpcArgs(&inPckg));
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::GetMaxVolume
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::GetMaxVolume(guint& aVol)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    TPckgBuf<guint> pckg;
    TIpcArgs args(&pckg);
    status = SendReceive(TMS_EFFECT_VOLUME_GETMAX, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        aVol = pckg();
        }
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::SetVolume
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::SetVolume(const guint aVol)
    {
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(TMS_EFFECT_VOLUME_SET, TIpcArgs(aVol));
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::GetVolume
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::GetVolume(guint& aVol)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    TPckgBuf<guint> pckg;
    TIpcArgs args(&pckg);
    status = SendReceive(TMS_EFFECT_VOLUME_GET, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        aVol = pckg();
        }
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::GetMaxGain
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::GetMaxGain(guint& aGain)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    TPckgBuf<guint> pckg;
    TIpcArgs args(&pckg);
    status = SendReceive(TMS_EFFECT_GAIN_GETMAX, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        aGain = pckg();
        }
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::SetGain
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::SetGain(const guint aGain)
    {
    return SendReceive(TMS_EFFECT_GAIN_SET, TIpcArgs(aGain));
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::GetGain
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::GetGain(guint& aGain)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    TPckgBuf<guint> pckg;
    TIpcArgs args(&pckg);
    status = SendReceive(TMS_EFFECT_GAIN_GET, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        aGain = pckg();
        }
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::GetSupportedBitRates
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::GetSupportedBitRates(BitRateVector& aVector)
    {
    TRAPD(status, GetSupportedBitRatesL(aVector));
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::GetSupportedBitRatesL
//
// -----------------------------------------------------------------------------
//
void TMSCallProxy::GetSupportedBitRatesL(BitRateVector& aVector)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    guint brCount = 0;
    TPckgBuf<guint> pckg;
    TIpcArgs args(&pckg);
    status = SendReceive(TMS_FORMAT_GETSUPPORTEDBITRATESCOUNT, args);

    if (status == TMS_RESULT_SUCCESS)
        {
        brCount = pckg();
        }
    else
        {
        User::Leave(status);
        }

    TRACE_PRN_N1(_L("TMSCallProxy->BRCount [%d]"), brCount);

    if (brCount > 0 && status == TMS_RESULT_SUCCESS)
        {
        HBufC8* buf = HBufC8::NewLC(brCount * sizeof(TUint));
        TPtr8 ptr = buf->Des();
        TIpcArgs args1;
        args1.Set(0, &ptr);
        status = SendReceive(TMS_FORMAT_GETSUPPORTEDBITRATES, args1);
        if (status == TMS_RESULT_SUCCESS)
            {
            RDesReadStream stream(ptr);
            CleanupClosePushL(stream); // stream on cleanup
            //aArray.Reset();

            for (guint i = 0; i < brCount; i++)
                {
                //aArray.Append(stream.ReadUint32L());
                aVector.push_back(stream.ReadUint32L());
                }

            CleanupStack::PopAndDestroy(&stream);
            }
        else
            {
            CleanupStack::PopAndDestroy(buf);
            User::Leave(status);
            }

        CleanupStack::PopAndDestroy(buf);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::SetBitRate
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::SetBitRate(const guint aBitrate)
    {
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(TMS_FORMAT_SETBITRATE, TIpcArgs(aBitrate));
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::GetBitRate
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::GetBitRate(guint& aBitrate)
    {
    gint status(TMS_RESULT_SUCCESS);
    TPckgBuf<guint> pckg;
    TIpcArgs args(&pckg);
    status = SendReceive(TMS_FORMAT_GETBITRATE, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        aBitrate = pckg();
        }
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::GetFormatCodecMode
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::GetFormatCodecMode(const TMSFormatType aFmtType,
        const TMSStreamType aStrmType, gint& aMode)
    {
    gint status(TMS_RESULT_SUCCESS);
    TPckgBuf<gint> pckg;
    TIpcArgs args(aFmtType, aStrmType, &pckg);
    status = SendReceive(TMS_FORMAT_GETCODECMODE, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        aMode = pckg();
        }
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::SetFormatCodecMode(const TMSFormatType aFmtType,
        const TMSStreamType aStrmType, gint aMode)
    {
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(TMS_FORMAT_SETCODECMODE, TIpcArgs(aFmtType,
            aStrmType, aMode));
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::GetCNG
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::GetCNG(const TMSFormatType aFmtType,
        gboolean& aCng)
    {
    gint status(TMS_RESULT_SUCCESS);
    TPckgBuf<gboolean> pckg;
    TIpcArgs args(aFmtType, &pckg);
    status = SendReceive(TMS_FORMAT_GETCNG, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        aCng = pckg();
        }
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::SetCNG
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::SetCNG(const TMSFormatType aFmtType,
        const gboolean aCng)
    {
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(TMS_FORMAT_SETCNG, TIpcArgs(aFmtType, aCng));
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::GetPlc
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::GetPlc(const TMSFormatType aFmtType,
        gboolean& aPlc)
    {
    gint status(TMS_RESULT_SUCCESS);
    TPckgBuf<gboolean> pckg;
    TIpcArgs args(aFmtType, &pckg);
    status = SendReceive(TMS_FORMAT_GETPLC, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        aPlc = pckg();
        }
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::SetPlc
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::SetPlc(const TMSFormatType aFmtType,
        const gboolean aPlc)
    {
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(TMS_FORMAT_SETPLC, TIpcArgs(aFmtType, aPlc));
    return TMSRESULT(status);
}

// -----------------------------------------------------------------------------
// TMSCallProxy::GetVADMode
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::GetVADMode(const TMSFormatType fmttype,
        gboolean& vad)
    {
    gint status(TMS_RESULT_SUCCESS);
    TPckgBuf<gboolean> pckg;
    TIpcArgs args(fmttype, &pckg);
    status = SendReceive(TMS_FORMAT_GETVAD, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        vad = pckg();
        }
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::SetVADMode
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::SetVADMode(const TMSFormatType fmttype,
        const gboolean vad)
    {
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(TMS_FORMAT_SETVAD, TIpcArgs(fmttype, vad));
    return TMSRESULT(status);
    }

// -----------------------------------------------------------------------------
// TMSCallProxy::GetDataXferChunkHandle
//
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSCallProxy::GetDataXferChunkHandle(TMSCallType callType,
        TMSStreamType strmType, gint strmId, guint32 key)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    TMSCliSrvDataXferChunkHndlDataStructBufPckg inPckg;
    inPckg().CallType = callType;
    inPckg().StreamType = strmType;
    inPckg().StreamId = strmId;
    inPckg().Key = key;
    // We don't want to convert Symbain error to TMS error in this case.
    status = RSessionBase::SendReceive(TMS_DATA_XFER_BUFFER_GET_HNDL,
            TIpcArgs(&inPckg));
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

// End of file
