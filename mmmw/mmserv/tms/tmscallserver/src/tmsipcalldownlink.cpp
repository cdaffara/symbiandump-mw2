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

#include <audiopreference.h>
#include <AudioOutput.h>
#include <mmcccodecinformation.h>
#include <gstappsrc.h>
#include "tmsutility.h"
#include "gsterrorconcealmentinterface.h"
#include "gstg711decoderinterface.h"
#include "gstilbcdecoderinterface.h"
#include "gstg729decoderinterface.h"
#include "ipcallstream.h"

using namespace TMS;

static TMSIPDownlink* iSelfDn;
#ifdef _DEBUG
static TInt iHeap;
#endif //_DEBUG
GstBuffer* gstDnlBuffer;
GstCaps* caps;
gint requestBufLen;

#ifdef __PLAY_WAV_FROM_FILE__
_LIT16(KTestFile1, "c:\\data\\sounds\\digital\\sample1.wav");
#endif

// -----------------------------------------------------------------------------
// TMSIPDownlink::cb_raw_playback_handoff
// -----------------------------------------------------------------------------
//
void TMSIPDownlink::cb_raw_playback_handoff(GstElement* /*appsrc*/,
        guint size)
    {
    TRACE_PRN_N(_L("TMS->DNL: cb_raw_playback_handoff"));

    requestBufLen = size;

#ifdef _DEBUG
    TInt block;
    RHeap &heap = User::Heap();
    TInt avail = heap.Available(block);
    RDebug::Print(_L("TMS->PRINT-HEAP-DN:Size[%d], Available[%d], delta[%d]"),
    heap.Size(), avail, (iHeap - avail));
    iHeap = avail;
#endif //_DEBUG
    iSelfDn->BufferToBeFilled();
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::bus_call
// -----------------------------------------------------------------------------
//
gboolean TMSIPDownlink::bus_call(GstBus* /*bus*/, GstMessage* msg,
        gpointer /*data*/)
    {
    switch (GST_MESSAGE_TYPE(msg))
        {
        case GST_MESSAGE_EOS:
            {
            gst_element_set_state(iSelfDn->iPipelinePlay, GST_STATE_NULL);
            gst_object_unref(GST_OBJECT(iSelfDn->iPipelinePlay));
            break;
            }
        case GST_MESSAGE_ERROR:
            {
            gchar* debug;
            GError* err;
            gst_message_parse_error(msg, &err, &debug);
            g_free(debug);
            g_print("Error: %s\n", err->message);
            g_error_free(err);
            break;
            }
        default:
            {
            break;
            }
        }

    return ETrue;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::gst_initialize_play
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::gst_initialize_play()
    {
    TRACE_PRN_FN_ENT;

    gint err = KErrNone;

    // create elements
    iPipelinePlay = gst_pipeline_new("pipelinePlay");
    iAppSrc = gst_element_factory_make("appsrc", "source");
    iSink = gst_element_factory_make("devsoundsink", "sink");

    if (!iAppSrc || !iSink)
        {
        err = KErrNotFound;
        TRACE_PRN_IF_ERR(err);
        return err;
        }

    iBusPlay = gst_pipeline_get_bus(GST_PIPELINE(iPipelinePlay));
    gst_bus_add_watch(iBusPlay, TMSIPDownlink::bus_call, NULL);
    gst_object_unref(iBusPlay);

    iMaxBufLen = ConfigureMedia(iCodecID);

    g_object_set(iAppSrc, "stream-type", 0, "blocksize", iMaxBufLen, NULL);
    gst_bin_add_many(GST_BIN(iPipelinePlay), iAppSrc, iSink, NULL);
    gst_element_link(iAppSrc, iSink);

    caps = gst_caps_new_simple(iMediaType,
                               "width", G_TYPE_INT, 16,
                               "depth", G_TYPE_INT, 16,
                               "signed", G_TYPE_BOOLEAN, TRUE,
                               "endianness", G_TYPE_INT, G_BYTE_ORDER,
                               "rate", G_TYPE_INT, 8000,
                               "channels", G_TYPE_INT, 1,
                               NULL);

    //NOTE: d/s is not ready at this time to return true maxvolume
    TInt maxvol = 10;
#ifdef __WINSCW__
    maxvol = 10000;
#endif //__WINSCW__
    g_object_set(G_OBJECT(iSink),
                 "volume", maxvol,
#ifndef __WINSCW__
                "priority", (gint)iPriority.iPriority,
                "preference", (gint)iPriority.iPref,
#endif
                NULL);

    gst_app_src_set_caps(GST_APP_SRC(iAppSrc), caps);
    gst_app_src_set_max_bytes(GST_APP_SRC(iAppSrc), iMaxBufLen);
    err = SetCodecCi();
    //gst_element_set_state(iPipelinePlay, GST_STATE_READY);

    TRACE_PRN_FN_EXT;
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::gst_play_raw
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::gst_play_raw()
    {
    TRACE_PRN_FN_ENT;

    // start playback
    gst_element_set_state(iPipelinePlay, GST_STATE_PLAYING);

    //configure the appsrc, we will push buffer to appsrc when it needs more data
    g_signal_connect(iAppSrc, "need-data",
                     G_CALLBACK (cb_raw_playback_handoff),
                     NULL);
    TRACE_PRN_FN_EXT;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::TMSIPDownlink
// Standard Constructor
// -----------------------------------------------------------------------------
//
TMSIPDownlink::TMSIPDownlink()
    {
    iSelfDn = this;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::~TMSIPDownlink
// Standard Constructor
// -----------------------------------------------------------------------------
//
TMSIPDownlink::~TMSIPDownlink()
    {
    TRACE_PRN_FN_ENT;

    Stop();
    gst_caps_unref(caps);
    gst_object_unref(GST_OBJECT(iPipelinePlay));
    gst_deinit();

    delete iErrConcealmentIntfc;
    delete iG711DecoderIntfc;
    delete iG729DecoderIntfc;
    delete iIlbcDecoderIntfc;

#ifdef __PLAY_WAV_FROM_FILE__
    iFs.Close();
    delete fbuf;
#endif

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::NewL
// Symbian two-phase constructor
// -----------------------------------------------------------------------------
//
TMSIPDownlink* TMSIPDownlink::NewL(const guint32 codecID,
        const TMMFPrioritySettings priority)
    {
    TMSIPDownlink* self = new (ELeave) TMSIPDownlink();
    CleanupStack::PushL(self);
    self->ConstructL(codecID, priority);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::ConstructL
// Part two of Symbian two phase construction
// -----------------------------------------------------------------------------
//
void TMSIPDownlink::ConstructL(const guint32 codecID,
        const TMMFPrioritySettings priority)
    {
    TRACE_PRN_FN_ENT;

    iCodecID = codecID;
    iPriority = priority;
    //    SetTime();
    gst_init(NULL, NULL);
    //    CalcLatency();
    //    SetTime();

    gint err = gst_initialize_play();
    if (err != TMS_RESULT_SUCCESS)
        {
        User::Leave(err);
        }

    // Client must set these before querying!
    iG711DecodeMode = TMS_G711_CODEC_MODE_ALAW;
    iILBCDecodeMode = TMS_ILBC_CODEC_MODE_20MS_FRAME;

#ifdef __PLAY_WAV_FROM_FILE__
    iReadSize = KVoIPPCM16FrameLen;
    err = iFs.Connect();
    if (err == KErrNone)
        {
        err = iFile.Open(iFs, KTestFile1, EFileShareAny | EFileRead);
        }
    if (err == KErrNone)
        {
        iFile.Size(fsize);
        }
    fbuf = HBufC8::NewL(fsize);
    TPtr8 p = fbuf->Des();
    iFile.Read(p, fsize);
    iFile.Close();
#endif
    //    CalcLatency();

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::Start
//
// -----------------------------------------------------------------------------
//
void TMSIPDownlink::Start()
    {
    TRACE_PRN_FN_ENT;

    gst_play_raw();
    iStatus = EStreaming;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::Stop
//
// -----------------------------------------------------------------------------
//
void TMSIPDownlink::Stop()
    {
    TRACE_PRN_FN_ENT;

    if (iStatus == EStreaming)
        {
        if (iPipelinePlay != NULL &&
            iPipelinePlay->current_state == GST_STATE_PLAYING)
            {
            gst_element_set_state(iPipelinePlay, GST_STATE_NULL);
            }

        iStatus = EReady;
        }

    if (gstDnlBuffer)
        {
        gst_buffer_unref(gstDnlBuffer);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::BufferToBeFilled
// -----------------------------------------------------------------------------
//
void TMSIPDownlink::BufferToBeFilled()
    {
    TRACE_PRN_N1(_L("TMS->DNL->BTBF: requestBufLen[%d]"), requestBufLen);

#ifndef __PLAY_WAV_FROM_FILE__
    // Create or adjust the chunk
    TInt err = DoChunk(requestBufLen, iMsgBuffer);

    if (err != TMS_RESULT_SUCCESS)
        {
        Stop();
        iMsgBuffer.iStatus = err;
        }
    else
        {
        // Notify client there is buffer ready to be filled
        iMsgBuffer.iStatus = iChunk.Handle();
        iMsgBuffer.iInt = requestBufLen;
        iStatus = EStreaming;
        // If chunk is opened, we will expect a call from the client to
        // get chunk handle. When we get a call to copy chunk handle,
        // check these variables and see if they match. This is not
        // completely secure, but will provide some level of security
        if (iMsgBuffer.iBool == TRUE)
            {
            iWriteDataXferHndlToClient = TRUE;
            iKey = iMsgBuffer.iUint32;
            }
        }

    iMsgBuffer.iRequest = ECmdFillBuffer;
    iMsgQueue.Send(iMsgBuffer);

#else //__PLAY_WAV_FROM_FILE__
    TInt err = KErrNone;
    BufferFilled();
#endif //__PLAY_WAV_FROM_FILE__
    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::BufferFilled
//
// -----------------------------------------------------------------------------
//
void TMSIPDownlink::BufferFilled(const guint buflen)
    {
#ifndef __PLAY_WAV_FROM_FILE__
    gstDnlBuffer = gst_buffer_new();
    gst_buffer_set_caps(gstDnlBuffer, caps);
    GST_BUFFER_SIZE(gstDnlBuffer) = buflen;

    //TODO: what if bufLen > requestBufLen?

    // IMPL#1:
    // gstDnlBuffer->data pointer is set to use chunk pointer.
    // Although unlikely in the pull mode, this may potentially result
    // in data loss if chunk gets overwritten before it is consumed.
    gst_buffer_set_data(gstDnlBuffer, iChunk.Base(), buflen);

    // IMPL#2
    // Alternative implementation with new buffer alloc and memcpy.
    // GStreamer will free this memory when buffer refcount becomes 0.
    //guint8* ptr = (guint8*)g_malloc(buflen);
    //GST_BUFFER_MALLOCDATA(gstDnlBuffer) = GST_BUFFER_DATA(gstDnlBuffer) = ptr;
    //memcpy(ptr, iChunk.Base(), buflen);

#else //__PLAY_WAV_FROM_FILE__
    User::After(TTimeIntervalMicroSeconds32(150000));

    TPtr8 p = fbuf->Des();
    TInt buflen = iReadSize;
    gstDnlBuffer = gst_buffer_new_and_alloc(buflen);
    gst_buffer_set_caps(gstDnlBuffer, caps);
    unsigned char* ptr = (unsigned char*)((fbuf->Ptr())+iReadBytes);
    gst_buffer_set_data(gstDnlBuffer, ptr, buflen);

    if ((iReadBytes + iReadSize> fsize))
        {
        // start over from the beginning
        iReadBytes = 0;
        }
    else
        {
        iReadBytes += iReadSize;
        }

#endif //__PLAY_WAV_FROM_FILE__
    gst_app_src_push_buffer(GST_APP_SRC(iAppSrc), gstDnlBuffer);
    TRACE_PRN_N1(_L("TMS->DNL->BF: LEN[%d]"), buflen);
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::SetCodecCi
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::SetCodecCi()
    {
    TRAPD(err, SetCodecCiL());
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::SetCodecCiL
//
// -----------------------------------------------------------------------------
//
void TMSIPDownlink::SetCodecCiL()
    {
    TRACE_PRN_FN_ENT;

    switch (iCodecID)
        {
        case KMccFourCCIdG711:
            {
            if (!iG711DecoderIntfc)
                {
                iG711DecoderIntfc = GST_G711_DECODER_GET_IFACE(iSink);
                }
            break;
            }
        case KMccFourCCIdG729:
            {
            if (!iG729DecoderIntfc)
                {
                iG729DecoderIntfc = GST_G729_DECODER_GET_IFACE(iSink);
                }
            break;
            }
        case KMccFourCCIdILBC:
            {
            if (!iIlbcDecoderIntfc)
                {
                iIlbcDecoderIntfc = GST_ILBC_DECODER_GET_IFACE(iSink);
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
            }
        }

    if (iCodecID == KMccFourCCIdG711 ||
        iCodecID == KMccFourCCIdG729 ||
        iCodecID == KMccFourCCIdILBC ||
        iCodecID == KMccFourCCIdAMRNB)
        {
        if (!iErrConcealmentIntfc)
            {
            iErrConcealmentIntfc = GST_ERROR_CONCEALMENT_GET_IFACE(iSink);
            }
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::SetVolume
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::SetVolume(const guint volume)
    {
    g_object_set(G_OBJECT(iSink), "volume", volume, NULL);
    TRACE_PRN_N1(_L("TMS->DNL: SetVolume [%d]"), volume);
    return TMS_RESULT_SUCCESS;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::GetVolume
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::GetVolume(guint& volume)
    {
    g_object_get(G_OBJECT(iSink), "volume", &volume, NULL);
    TRACE_PRN_N1(_L("TMS->DNL: GetVolume [%d]"), volume);
    return TMS_RESULT_SUCCESS;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::GetMaxVolume
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::GetMaxVolume(guint& volume)
    {
    g_object_get(G_OBJECT(iSink), "maxvolume", &volume, NULL);
    TRACE_PRN_N1(_L("TMS->DNL: MaxVolume [%d]"), volume);
    return TMS_RESULT_SUCCESS;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::GetDataXferChunkHndl
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::GetDataXferChunkHndl(const TUint32 key, RChunk& chunk)
    {
    gint status = TMS_RESULT_DOES_NOT_EXIST;

    if (iChunk.Handle())
        {
        if (iWriteDataXferHndlToClient && (iKey == key))
            {
            chunk = iChunk;
            iWriteDataXferHndlToClient = FALSE;
            iKey = 0;
            status = TMS_RESULT_SUCCESS;
            }
        else
            {
            status = TMS_RESULT_ILLEGAL_OPERATION;
            }
        }

    TRACE_PRN_IF_ERR(status);
    return status;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::SetIlbcCodecMode
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::SetIlbcCodecMode(gint mode)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    if (iStatus == EReady)
        {
        iILBCDecodeMode = mode;

        if (iIlbcDecoderIntfc && iCodecID == KMccFourCCIdILBC)
            {
            if (mode == TMS_ILBC_CODEC_MODE_20MS_FRAME)
                {
                err = iIlbcDecoderIntfc->SetDecoderMode(E20msFrame);
                TRACE_PRN_N(_L("TMS->DNL: iLBC Mode Set [20ms]"));
                }
            else if (mode == TMS_ILBC_CODEC_MODE_30MS_FRAME)
                {
                err = iIlbcDecoderIntfc->SetDecoderMode(E30msFrame);
                TRACE_PRN_N(_L("TMS->DNL: iLBC Mode Set [30ms]"));
                }
            }
        }

    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::GetIlbcCodecMode
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::GetIlbcCodecMode(gint& mode)
    {
    // not available through CIs -> return cashed value
    mode = iILBCDecodeMode;
    TRACE_PRN_N1(_L("TMS->DNL: GetIlbcCodecMode [%d]"), mode);
    return TMS_RESULT_SUCCESS;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::SetG711CodecMode
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::SetG711CodecMode(gint mode)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    if (iStatus == EReady)
        {
        iG711DecodeMode = mode;

        if (iG711DecoderIntfc && iCodecID == KMccFourCCIdG711)
            {
            if (mode == TMS_G711_CODEC_MODE_ALAW)
                {
                err = iG711DecoderIntfc->SetDecoderMode(EDecALaw);
                TRACE_PRN_N(_L("TMS->DNL: G711 Mode Set [ALaw]"));
                }
            else if (mode == TMS_G711_CODEC_MODE_MULAW)
                {
                err = iG711DecoderIntfc->SetDecoderMode(EDecULaw);
                TRACE_PRN_N(_L("TMS->DNL: G711 Mode Set [uLaw]"));
                }
            }
        }

    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::GetG711CodecMode
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::GetG711CodecMode(gint& mode)
    {
    // not available through CIs -> return cached value
    mode = iG711DecodeMode;
    TRACE_PRN_N1(_L("TMS->DNL: GetG711CodecMode [%d]"), mode);
    return TMS_RESULT_SUCCESS;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::FrameModeRqrdForEC
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::FrameModeRqrdForEC(gboolean& frmodereq)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    if (iStatus == EReady)
        {
        if (iErrConcealmentIntfc)
            {
            err = iErrConcealmentIntfc->FrameModeRqrdForEC(&frmodereq);
            TRACE_PRN_N1(_L("TMS->DNL: FrameModeRqrdForEC [%d]"), frmodereq);
            }
        }

    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::SetFrameMode
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::SetFrameMode(const gboolean frmode)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    if (iStatus == EReady)
        {
        iFrameMode = frmode;

        if (iErrConcealmentIntfc)
            {
            err = iErrConcealmentIntfc->SetFrameMode(frmode);
            TRACE_PRN_N1(_L("TMS->DNL: SetFrameMode [%d]"), frmode);
            }
        }

    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::GetFrameMode
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::GetFrameMode(gboolean& frmode)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    if (iErrConcealmentIntfc)
        {
        // not available through CIs -> return cached value
        frmode = iFrameMode;
        TRACE_PRN_N1(_L("TMS->DNL: GetFrameMode [%d]"), frmode);
        err = TMS_RESULT_SUCCESS;
        }

    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::ConcealErrorForNextBuffer
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::ConcealErrorForNextBuffer()
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    if (iErrConcealmentIntfc)
        {
        err = iErrConcealmentIntfc->ConcealErrorForNextBuffer();
        TRACE_PRN_N(_L("TMS->DNL: ConcealErrorForNextBuffer"));
        }

    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::SetCng
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::SetCng(const TMSFormatType fmttype, const gboolean cng)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    if (iStatus == EReady)
        {
        if (fmttype == TMS_FORMAT_G711 && iG711DecoderIntfc)
            {
            err = iG711DecoderIntfc->SetCng(cng);
            TRACE_PRN_N1(_L("TMS->DNL: SetCng [%d]"), cng);
            }
        else if (fmttype == TMS_FORMAT_ILBC && iIlbcDecoderIntfc)
            {
            err = iIlbcDecoderIntfc->SetCng(cng);
            TRACE_PRN_N1(_L("TMS->DNL: SetCng [%d]"), cng);
            }
        }

    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::GetCng
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::GetCng(const TMSFormatType fmttype, gboolean& cng)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    if (iStatus == EReady)
        {
        if (fmttype == TMS_FORMAT_G711 && iG711DecoderIntfc)
            {
            err = iG711DecoderIntfc->GetCng(&cng);
            TRACE_PRN_N1(_L("TMS->DNL: GetCng [%d]"), cng);
            }
        else if (fmttype == TMS_FORMAT_ILBC && iIlbcDecoderIntfc)
            {
            err = iIlbcDecoderIntfc->GetCng(&cng);
            TRACE_PRN_N1(_L("TMS->DNL: GetCng [%d]"), cng);
            }
        }

    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::SetPlc
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::SetPlc(const TMSFormatType fmttype, const gboolean plc)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    if (iStatus == EReady)
        {
        if (fmttype == TMS_FORMAT_G711 && iG711DecoderIntfc)
            {
            iPlc = plc;
            err = iG711DecoderIntfc->SetPlc(iPlc);
            TRACE_PRN_N1(_L("TMS->DNL: SetPlc [%d]"), plc);
            }
        }

    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::GetPlc
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::GetPlc(const TMSFormatType fmttype, gboolean& plc)
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    if (fmttype == TMS_FORMAT_G711 && iG711DecoderIntfc)
        {
        // not available through CIs -> return cached value
        plc = iPlc;
        err = TMS_RESULT_SUCCESS;
        TRACE_PRN_N1(_L("TMS->DNL: GetPlc [%d]"), plc);
        }

    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::BadLsfNextBuffer
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::BadLsfNextBuffer()
    {
    gint err = TMS_RESULT_DOES_NOT_EXIST;

    if (iStatus == EStreaming)
        {
        if (iCodecID == KMccFourCCIdG729 && iG729DecoderIntfc)
            {
            err = iG729DecoderIntfc->BadLsfNextBuffer();
            TRACE_PRN_N(_L("TMS->DNL: BadLsfNextBuffer"));
            }
        }

    TRACE_PRN_IF_ERR(err);
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::SetAudioDeviceL
//
// -----------------------------------------------------------------------------
void TMSIPDownlink::SetAudioDeviceL(TMSAudioOutput output)
    {
    gint outputDev;

    // ENoPreference=0, EAll=1, ENoOutput=2, EPrivate=3, EPublic=4
    switch (output)
        {
        case TMS_AUDIO_OUTPUT_PRIVATE:
        case TMS_AUDIO_OUTPUT_HANDSET:
            outputDev = (gint)CAudioOutput::EPrivate;
            break;
        case TMS_AUDIO_OUTPUT_PUBLIC:
        case TMS_AUDIO_OUTPUT_LOUDSPEAKER:
            outputDev = (gint)CAudioOutput::EPublic;
            break;
        default: // Use default device routing
            outputDev = (gint)CAudioOutput::ENoPreference;
            break;
        }

    g_object_set(G_OBJECT(iSink), "outputdevice", outputDev, NULL);
    TRACE_PRN_N1(_L("TMS->DNL: SetAudioDeviceL [%d]"), outputDev);
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::GetAudioDeviceL
//
// -----------------------------------------------------------------------------
//
void TMSIPDownlink::GetAudioDeviceL(TMSAudioOutput& output)
    {
    TRACE_PRN_FN_ENT;

    gint outputDev;
    g_object_get(G_OBJECT(iSink), "outputdevice", &outputDev, NULL);
    TRACE_PRN_N1(_L("TMS->DNL: GetAudioDevice [%d]"), outputDev);

    switch (outputDev)
        {
        case CAudioOutput::ENoPreference:
        case CAudioOutput::EAll:
        case CAudioOutput::ENoOutput:
        case CAudioOutput::EPrivate:
            output = TMS_AUDIO_OUTPUT_PRIVATE;
            break;
        case CAudioOutput::EPublic:
            output = TMS_AUDIO_OUTPUT_PUBLIC;
            break;
        default:
            output = TMS_AUDIO_OUTPUT_PRIVATE;
            break;
        }

    TRACE_PRN_FN_EXT;
    }

// End of file
