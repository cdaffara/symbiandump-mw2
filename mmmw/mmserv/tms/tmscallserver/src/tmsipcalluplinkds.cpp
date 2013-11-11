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
#include <mmcccodecinformation.h>
#include <IlbcEncoderIntfc.h>
#include <G711EncoderIntfc.h>
#include <G729EncoderIntfc.h>
#include <SpeechEncoderConfig.h>
#include "tmsutility.h"
#include "tmsipcallstream.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// TMSIPUplink::TMSIPUplink
// Standard Constructor
// -----------------------------------------------------------------------------
//
TMSIPUplink::TMSIPUplink(TMSIPDevSoundObserver& observer) :
    TMSIPCallStreamBase(observer)
    {
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::~TMSIPUplink
// Standard Constructor
// -----------------------------------------------------------------------------
//
TMSIPUplink::~TMSIPUplink()
    {
    TRACE_PRN_FN_ENT;

    Stop();

    delete iSpeechEncoderConfig;
    delete iG711EncoderIntfc;
    delete iG729EncoderIntfc;
    delete iIlbcEncoderIntfc;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::NewL
// Symbian two-phase constructor
// -----------------------------------------------------------------------------
//
TMSIPUplink* TMSIPUplink::NewL(TMSIPDevSoundObserver& observer,
        const guint32 codecID, const TMMFPrioritySettings priority,
        const gint retrytime)
    {
    TMSIPUplink* self = new (ELeave) TMSIPUplink(observer);
    CleanupStack::PushL(self);
    self->ConstructL(codecID, priority, retrytime);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::ConstructL
// Part two of Symbian two phase construction
// -----------------------------------------------------------------------------
//
void TMSIPUplink::ConstructL(const guint32 codecID,
        const TMMFPrioritySettings priority, const gint /*retrytime*/)
    {
    TRACE_PRN_FN_ENT;
    iCodecID = codecID;
    iPriority = priority;

    // Client must set these before querying!
    iG711EncodeMode = TMS_G711_CODEC_MODE_ALAW;
    iILBCEncodeMode = TMS_ILBC_CODEC_MODE_20MS_FRAME;
    TRAPD(err, InitDevSoundL(EMMFStateRecording, priority));
    if (err != TMS_RESULT_SUCCESS)
        {
        User::Leave(err);
        }

    iMaxBufLen = ConfigureMedia(iCodecID);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::Start
//
// -----------------------------------------------------------------------------
//
void TMSIPUplink::Start(const gint /*retrytime*/)
    {
    TRACE_PRN_FN_ENT;

    gint err = TMS_RESULT_ILLEGAL_OPERATION;
    iWriteDataXferHndlToClient = FALSE;

    if (iStatus == EReady && iDevSound)
        {
        TRAP(err, iDevSound->RecordInitL());
        TRACE_PRN_IF_ERR(err);
        iObserver.UplinkStarted(err);
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::Stop
//
// -----------------------------------------------------------------------------
//
void TMSIPUplink::Stop()
    {
    TRACE_PRN_FN_ENT;

    if (iStatus == EStreaming && iDevSound)
        {
        iDevSound->Stop();
        iStatus = EReady;
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::BufferToBeEmptied
// From MDevSoundObserver
// -----------------------------------------------------------------------------
//
void TMSIPUplink::BufferToBeEmptied(CMMFBuffer* aBuffer)
    {
    iDevSoundBufPtr = static_cast<CMMFDataBuffer*> (aBuffer);
    TDes8& data = iDevSoundBufPtr->Data();
    iBufLen = iDevSoundBufPtr->BufferSize();
    TRACE_PRN_N1(_L("TMS->UPL: BTBE->LEN [%d]"), iBufLen);

    // Adjust/create RChunk if necessary
    gint err = DoChunk(iBufLen, iMsgBuffer);

    if (err != TMS_RESULT_SUCCESS)
        {
        Stop();
        iMsgBuffer.iStatus = err;
        }
    else
        {
        // Pass buffer parameters to the client
        iMsgBuffer.iStatus = err;
        iMsgBuffer.iInt = iBufLen;

        // Copy data over to RChunk
        TPtr8 dataPtr(iChunk.Base(), iBufLen, iBufLen);
        dataPtr = data;
        iStatus = EStreaming;

        // If chunk is opened, we will expect a call from the client to
        // get chunk handle. When we get a call to copy chunk handle,
        // check these variables and see if they match. This is not
        // completely secure, but will provide some level of security.
        if (iMsgBuffer.iBool == TRUE)
            {
            iWriteDataXferHndlToClient = TRUE;
            iKey = iMsgBuffer.iUint32;
            }
        }

    // Notify client there is buffer ready to be emptied
    iMsgBuffer.iRequest = ECmdEmptyBuffer;
    err = iMsgQueue.Send(iMsgBuffer);
    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::BufferEmptied
//
// -----------------------------------------------------------------------------
//
void TMSIPUplink::BufferEmptied()
    {
    //TRACE_PRN_N(_L("TMS->UPL->BE"));
    if (iDevSound)
        {
        iDevSound->RecordData();
        }
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::SetCodecCi
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::SetCodecCi()
    {
    TRAPD(err, SetCodecCiL());
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::SetCodecCiL
//
// -----------------------------------------------------------------------------
//
void TMSIPUplink::SetCodecCiL()
    {
    TRACE_PRN_FN_ENT;

    switch (iCodecID)
        {
        case KMccFourCCIdG711:
            {
            if (!iG711EncoderIntfc)
                {
                iG711EncoderIntfc = CG711EncoderIntfc::NewL(*iDevSound);
                }
            break;
            }
        case KMccFourCCIdG729:
            {
            if (!iG729EncoderIntfc)
                {
                iG729EncoderIntfc = CG729EncoderIntfc::NewL(*iDevSound);
                }
            break;
            }
        case KMccFourCCIdILBC:
            {
            if (!iIlbcEncoderIntfc)
                {
                iIlbcEncoderIntfc = CIlbcEncoderIntfc::NewL(*iDevSound);
                }
            break;
            }
        case KMccFourCCIdAMRNB:
        case KMMFFourCCCodePCM16:
            {
            break;
            }
        default:
            {
            User::Leave(TMS_RESULT_INVALID_ARGUMENT);
            break;
            }
        }

    if (!iSpeechEncoderConfig && iCodecID != KMMFFourCCCodePCM16)
        {
        iSpeechEncoderConfig = CSpeechEncoderConfig::NewL(*iDevSound);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::SetGain
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::SetGain(const guint gain)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iDevSound)
        {
        iDevSound->SetGain(gain);
        status = TMS_RESULT_SUCCESS;
        }
    TRACE_PRN_N1(_L("TMS->UPL: SetGain [%d]"), gain);
    return status;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::GetGain
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::GetGain(guint& gain)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iDevSound)
        {
        gain = iDevSound->Gain();
        status = TMS_RESULT_SUCCESS;
        }
    TRACE_PRN_N1(_L("TMS->UPL: GetGain [%d]"), gain);
    return status;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::GetMaxGain
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::GetMaxGain(guint& gain)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iDevSound)
        {
        gain = iDevSound->MaxGain();
        status = TMS_RESULT_SUCCESS;
        }
    TRACE_PRN_N1(_L("TMS->UPL: MaxGain [%d]"), gain);
    return status;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::GetDataXferChunkHndl
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::GetDataXferChunkHndl(const TUint32 key, RChunk& chunk)
    {
    gint status = TMS_RESULT_SUCCESS;

    if (iChunk.Handle())
        {
        if (iWriteDataXferHndlToClient && (iKey == key))
            {
            chunk = iChunk;
            iWriteDataXferHndlToClient = FALSE;
            iKey = 0;
            }
        else
            {
            status = TMS_RESULT_ILLEGAL_OPERATION;
            }
        }
    return status;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::SetIlbcCodecMode
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::SetIlbcCodecMode(const gint mode)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    if (iStatus == EReady)
        {
        iILBCEncodeMode = mode;

        if (iIlbcEncoderIntfc && iCodecID == KMccFourCCIdILBC)
            {
            if (mode == TMS_ILBC_CODEC_MODE_20MS_FRAME)
                {
                err = iIlbcEncoderIntfc->SetEncoderMode(
                        CIlbcEncoderIntfc::E20msFrame);
                TRACE_PRN_N(_L("TMS->UPL: SetIlbcCodecMode [20ms Frame]"));
                }
            else if (mode == TMS_ILBC_CODEC_MODE_30MS_FRAME)
                {
                err = iIlbcEncoderIntfc->SetEncoderMode(
                        CIlbcEncoderIntfc::E30msFrame);
                TRACE_PRN_N(_L("TMS->UPL: SetIlbcCodecMode [30ms Frame]"));
                }
            }
        }
    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::GetIlbcCodecMode
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::GetIlbcCodecMode(gint& mode)
    {
    // not available through CIs -> return cached value
    mode = iILBCEncodeMode;
    TRACE_PRN_N1(_L("TMS->UPL: GetIlbcCodecMode [%d]"), mode);
    return TMS_RESULT_SUCCESS;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::SetG711CodecMode
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::SetG711CodecMode(const gint mode)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    if (iStatus == EReady)
        {
        iG711EncodeMode = mode;

        if (iG711EncoderIntfc && iCodecID == KMccFourCCIdG711)
            {
            if (mode == TMS_G711_CODEC_MODE_ALAW)
                {
                err = iG711EncoderIntfc->SetEncoderMode(
                        CG711EncoderIntfc::EEncALaw);
                TRACE_PRN_N(_L("TMS->UPL: SetG711CodecMode [ALaw]"));
                }
            else if (mode == TMS_G711_CODEC_MODE_MULAW)
                {
                err = iG711EncoderIntfc->SetEncoderMode(
                        CG711EncoderIntfc::EEncULaw);
                TRACE_PRN_N(_L("TMS->UPL: SetG711CodecMode [uLaw]"));
                }
            }
        }
    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::GetG711CodecMode
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::GetG711CodecMode(gint& mode)
    {
    // not available through CIs -> return cached value
    mode = iG711EncodeMode;
    TRACE_PRN_N1(_L("TMS->UPL: GetG711CodecMode [%d]"), mode);
    return TMS_RESULT_SUCCESS;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::GetSupportedBitrates
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::GetSupportedBitrates(RArray<guint>& bitrates)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;
    bitrates.Reset();

    if (iSpeechEncoderConfig)
        {
        err = iSpeechEncoderConfig->GetSupportedBitrates(bitrates);
        }
    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::SetBitrate
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::SetBitrate(guint bitrate)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    if (iSpeechEncoderConfig)
        {
        err = iSpeechEncoderConfig->SetBitrate(bitrate);
        TRACE_PRN_N1(_L("TMS->UPL: SetBitrate [%d]"), bitrate);
        }
    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::GetBitrate
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::GetBitrate(guint& bitrate)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    if (iSpeechEncoderConfig)
        {
        err = iSpeechEncoderConfig->GetBitrate(bitrate);
        TRACE_PRN_N1(_L("TMS->UPL: GetBitrate [%d]"), bitrate);
        }
    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::SetVad
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::SetVad(const TMSFormatType fmttype, const gboolean vad)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    switch (fmttype)
        {
        case TMS_FORMAT_G711:
            {
            if (iG711EncoderIntfc)
                {
                err = iG711EncoderIntfc->SetVadMode(vad);
                TRACE_PRN_N1(_L("TMS->UPL: SetVad [%d]"), vad);
                }
            break;
            }
        case TMS_FORMAT_G729:
            {
            if (iG729EncoderIntfc)
                {
                err = iG729EncoderIntfc->SetVadMode(vad);
                TRACE_PRN_N1(_L("TMS->UPL: SetVad [%d]"), vad);
                }
            break;
            }
        case TMS_FORMAT_ILBC:
            {
            if (iIlbcEncoderIntfc)
                {
                err = iIlbcEncoderIntfc->SetVadMode(vad);
                TRACE_PRN_N1(_L("TMS->UPL: SetVad [%d]"), vad);
                }
            break;
            }
        case TMS_FORMAT_AMR:
            {
            if (iSpeechEncoderConfig)
                {
                err = iSpeechEncoderConfig->SetVadMode(vad);
                TRACE_PRN_N1(_L("TMS->UPL: SetVad [%d]"), vad);
                }
            break;
            }
        default:
            {
            break; //TMS_RESULT_DOES_NOT_EXIST
            }
        }

    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::GetVad
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::GetVad(const TMSFormatType fmttype, gboolean& vad)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    switch (fmttype)
        {
        case TMS_FORMAT_G711:
            {
            if (iG711EncoderIntfc)
                {
                err = iG711EncoderIntfc->GetVadMode(vad);
                TRACE_PRN_N1(_L("TMS->UPL: GetVad [%d]"), vad);
                }
            break;
            }
        case TMS_FORMAT_G729:
            {
            if (iG729EncoderIntfc)
                {
                err = iG729EncoderIntfc->GetVadMode(vad);
                TRACE_PRN_N1(_L("TMS->UPL: GetVad [%d]"), vad);
                }
            break;
            }
        case TMS_FORMAT_ILBC:
            {
            if (iIlbcEncoderIntfc)
                {
                err = iIlbcEncoderIntfc->GetVadMode(vad);
                TRACE_PRN_N1(_L("TMS->UPL: GetVad [%d]"), vad);
                }
            break;
            }
        case TMS_FORMAT_AMR:
            {
            if (iSpeechEncoderConfig)
                {
                err = iSpeechEncoderConfig->GetVadMode(vad);
                TRACE_PRN_N1(_L("TMS->UPL: GetVad [%d]"), vad);
                }
            break;
            }
        default:
            {
            break; //TMS_RESULT_DOES_NOT_EXIST
            }
        }

    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::InitializeComplete
// A callback from the DevSound indicating completion of the initialization.
// It will send config data to the D/S and configure the encoder via CI.
// If everything goes well, the state of the thread is set EReady.
// The initialization completion message is sent back to the main thread.
// -----------------------------------------------------------------------------
//
void TMSIPUplink::InitializeComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;

    gint status = aError;

    if (status == TMS_RESULT_SUCCESS && iDevSound)
        {
        TMMFCapabilities conf;
        conf = iDevSound->Config();
        conf.iRate = EMMFSampleRate8000Hz;
        conf.iChannels = EMMFMono;
        TRAP(status, iDevSound->SetConfigL(conf));
        if (status == TMS_RESULT_SUCCESS)
            {
            // We are ready to stream even in case of CI setting failure
            iStatus = EReady;
            iMaxGain = iDevSound->MaxGain();
            }

        // Init Custom Interface API to the Encoder. Any return error can
        // be ignored as codec can still run in the default mode even if not
        // fully configured.
        SetCodecCi();
        }

    iObserver.UplinkInitCompleted(status);
    TRACE_PRN_IF_ERR(status);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::RecordError
// From MDevSoundObserver
// Recording error is send to the main thread.
// The state of recorder is rolled back to EReady.
// -----------------------------------------------------------------------------
//
void TMSIPUplink::RecordError(TInt aError)
    {
    iStatus = EReady;

#ifndef __WINSCW__
    iObserver.UplinkStarted(aError);
#else  //__WINSCW__
    if (aError == KErrAccessDenied || aError == KErrInUse)
        {
        iObserver.UplinkStarted(TMS_RESULT_SUCCESS);
        }
#endif //__WINSCW__

    TRACE_PRN_IF_ERR(aError);
    }

// End of file

