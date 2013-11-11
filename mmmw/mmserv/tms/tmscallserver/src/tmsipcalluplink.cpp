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
#include <mmcccodecinformation.h>
#include <gstappsink.h>
#include "tmsutility.h"
#include "ipcallstream.h"

using namespace TMS;

static TMSIPUplink* iSelfUp;
GstBuffer* gstUplBuffer;
#ifdef _DEBUG
//static TInt iHeapUp;
#endif //_DEBUG

// -----------------------------------------------------------------------------
// TMSIPUplink::cb_record_raw_handoff
// -----------------------------------------------------------------------------
//
void TMSIPUplink::cb_record_raw_handoff(GstElement *sink)
    {
    TRACE_PRN_N(_L("TMS->UPL: cb_record_raw_handoff Enter"));

#ifdef _DEBUG
    /*    TInt block;
     RHeap &heap = User::Heap();
     TInt avail = heap.Available(block);
     RDebug::Print(_L("VoIP->PRINT-HEAP-UP:Available:[%d], delta:[%d]"),
     avail, iHeapUp-avail);
     iHeapUp = avail;*/
#endif

    gstUplBuffer = gst_app_sink_pull_buffer(GST_APP_SINK(sink));
    iSelfUp->BufferToBeEmptied();
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::bus_call
// -----------------------------------------------------------------------------
//
gboolean TMSIPUplink::bus_call(GstBus* /*bus*/, GstMessage* msg,
        gpointer /*data*/)
    {
    switch (GST_MESSAGE_TYPE(msg))
        {
        case GST_MESSAGE_EOS:
            {
            gst_element_set_state(iSelfUp->iPipelineRec, GST_STATE_NULL);
            gst_object_unref(GST_OBJECT(iSelfUp->iPipelineRec));
            break;
            }
        case GST_MESSAGE_ERROR:
            {
            gchar *debug;
            GError *err;
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
// TMSIPUplink::gst_initialize_record
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::gst_initialize_record()
    {
    TRACE_PRN_FN_ENT;

    gint err = KErrNone;

    // create a new bin to hold the elements
    iPipelineRec = gst_pipeline_new("pipelineRec");
    iSource = gst_element_factory_make("devsoundsrc", "record_audio");
    iAppSink = gst_element_factory_make("appsink", "appsink");

    if (!iSource || !iAppSink)
        {
        err = KErrNotFound;
        TRACE_PRN_IF_ERR(err);
        return err;
        }

    iMaxBufLen = ConfigureMedia(iCodecID);

    GstCaps* caps = gst_caps_new_simple(iMediaType,
                                        "width", G_TYPE_INT, 16,
                                        "depth", G_TYPE_INT, 16,
                                        "signed", G_TYPE_BOOLEAN, TRUE,
                                        "endianness", G_TYPE_INT, G_BYTE_ORDER,
                                        "rate", G_TYPE_INT, 8000,
                                        "channels", G_TYPE_INT, 1,
                                        NULL);

    iBusRec = gst_pipeline_get_bus(GST_PIPELINE(iPipelineRec));
    gst_bus_add_watch(iBusRec, (TMSIPUplink::bus_call), NULL);
    gst_object_unref(iBusRec);

    // add objects to the main iPipelineRec
    gst_bin_add_many(GST_BIN(iPipelineRec), iSource, iAppSink, NULL);

    // link the elements
    gst_element_link_filtered(iSource, iAppSink, caps);

    g_object_set(G_OBJECT(iAppSink),
                 "max-buffers", 1,
                 "emit-signals", TRUE,
                 "sync", FALSE,
                 NULL);

    g_signal_connect(iAppSink, "new-buffer",
                     G_CALLBACK(cb_record_raw_handoff),
                     NULL);

    gst_app_sink_set_caps(GST_APP_SINK(iAppSink), caps);
    gst_caps_unref(caps);

    //NOTE: d/s is not ready at this time to return true maxgain
    TInt maxgain = 1;
#ifdef __WINSCW__
    maxgain = 64;
#endif //__WINSCW__
    g_object_set(G_OBJECT(iSource),
                 "blocksize", iMaxBufLen,
                 "gain", maxgain,
#ifndef __WINSCW__
                 "priority", (gint)iPriority.iPriority,
                 "preference", (gint)iPriority.iPref,
#endif
                 NULL);

    err = SetCodecCi();
    //gst_element_set_state(iPipelineRec, GST_STATE_READY);

    TRACE_PRN_FN_EXT;
    return err;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::gst_record_raw
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::gst_record_raw()
    {
    // start recording
    gst_element_set_state(iPipelineRec, GST_STATE_PLAYING);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::TMSIPUplink
// Standard Constructor
// -----------------------------------------------------------------------------
//
TMSIPUplink::TMSIPUplink()
    {
    iSelfUp = this;
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
    gst_object_unref(GST_OBJECT(iPipelineRec));
    gst_deinit();

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
TMSIPUplink* TMSIPUplink::NewL(const guint32 codecID,
        const TMMFPrioritySettings priority)
    {
    TMSIPUplink* self = new (ELeave) TMSIPUplink();
    CleanupStack::PushL(self);
    self->ConstructL(codecID, priority);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::ConstructL
// Part two of Symbian two phase construction
// -----------------------------------------------------------------------------
//
void TMSIPUplink::ConstructL(const guint32 codecID,
        const TMMFPrioritySettings priority)
    {
    TRACE_PRN_FN_ENT;

    iCodecID = codecID;
    iPriority = priority;
    //    SetTime();
    gst_init(NULL, NULL);
    //    CalcLatency();
    //    SetTime();
    gint err = gst_initialize_record();
    if (err != TMS_RESULT_SUCCESS)
        {
        User::Leave(err);
        }

    // Client must set these before querying!
    iG711EncodeMode = TMS_G711_CODEC_MODE_ALAW;
    iILBCEncodeMode = TMS_ILBC_CODEC_MODE_20MS_FRAME;
    //    CalcLatency();

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::Start
//
// -----------------------------------------------------------------------------
//
void TMSIPUplink::Start()
    {
    TRACE_PRN_FN_ENT;

    gst_record_raw();
    iStatus = EStreaming;

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

    if (iStatus == EStreaming)
        {
        if (iPipelineRec != NULL &&
            iPipelineRec->current_state == GST_STATE_PLAYING)
            {
            GstStateChangeReturn ret;
            ret = gst_element_set_state(iPipelineRec, GST_STATE_NULL);
            if (ret == GST_STATE_CHANGE_FAILURE)
                {
                ;// retry???
                //GST_STATE_CHANGE_SUCCESS
                }
            }
        iStatus = EReady;
        }

    if (gstUplBuffer)
        {
        gst_buffer_unref(gstUplBuffer);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::BufferToBeEmptied
// From MDevSoundObserver
// -----------------------------------------------------------------------------
//
void TMSIPUplink::BufferToBeEmptied()
    {
    TInt buflen = GST_BUFFER_SIZE(gstUplBuffer);
    TRACE_PRN_N1(_L("TMS->UPL: BTBE->LEN [%d]"), buflen);

    // Adjust/create RChunk if necessary
    TInt err = DoChunk(buflen, iMsgBuffer);

    if (err != TMS_RESULT_SUCCESS)
        {
        Stop();
        iMsgBuffer.iStatus = err;
        }
    else
        {
        // Pass buffer parameters to the client
        iMsgBuffer.iStatus = err;
        iMsgBuffer.iInt = buflen;

        // Copy data over to RChunk
        TPtr8 dataPtr(iChunk.Base(), buflen, iMaxBufLen);
        dataPtr.Copy(GST_BUFFER_DATA(gstUplBuffer), buflen);
        gst_buffer_unref(gstUplBuffer);

        //RDebug::RawPrint(dataPtr);
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
                iG711EncoderIntfc = GST_G711_ENCODER_GET_IFACE(iSource);
                }
            break;
            }
        case KMccFourCCIdG729:
            {
            if (!iG729EncoderIntfc)
                {
                iG729EncoderIntfc = GST_G729_ENCODER_GET_IFACE(iSource);
                }
            break;
            }
        case KMccFourCCIdILBC:
            {
            if (!iIlbcEncoderIntfc)
                {
                iIlbcEncoderIntfc = GST_ILBC_ENCODER_GET_IFACE(iSource);
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

    if (!iSpeechEncoderConfig && iCodecID != KMMFFourCCCodePCM16)
        {
        iSpeechEncoderConfig = GST_SPEECH_ENCODER_CONFIG_GET_IFACE(iSource);
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
    g_object_set(G_OBJECT(iSource), "gain", gain, NULL);
    TRACE_PRN_N1(_L("TMS->UPL: SetGain [%d]"), gain);
    return TMS_RESULT_SUCCESS;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::GetGain
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::GetGain(guint& gain)
    {
    g_object_get(G_OBJECT(iSource), "gain", &gain, NULL);
    TRACE_PRN_N1(_L("TMS->UPL: GetGain [%d]"), gain);
    return TMS_RESULT_SUCCESS;
    }

// -----------------------------------------------------------------------------
// TMSIPUplink::GetMaxGain
//
// -----------------------------------------------------------------------------
//
gint TMSIPUplink::GetMaxGain(guint& gain)
    {
    g_object_get(G_OBJECT(iSource), "maxgain", &gain, NULL);
    TRACE_PRN_N1(_L("TMS->UPL: MaxGain [%d]"), gain);
    return TMS_RESULT_SUCCESS;
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
                err = iIlbcEncoderIntfc->SetEncoderMode(EIlbc20msFrame);
                TRACE_PRN_N(_L("TMS->UPL: SetIlbcCodecMode [20ms Frame]"));
                }
            else if (mode == TMS_ILBC_CODEC_MODE_30MS_FRAME)
                {
                err = iIlbcEncoderIntfc->SetEncoderMode(EIlbc30msFrame);
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
                err = iG711EncoderIntfc->SetEncoderMode(EEncALaw);
                TRACE_PRN_N(_L("TMS->UPL: SetG711CodecMode [ALaw]"));
                }
            else if (mode == TMS_G711_CODEC_MODE_MULAW)
                {
                err = iG711EncoderIntfc->SetEncoderMode(EEncULaw);
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
        gint i = 0;
        GList* gbitrates = NULL;
        //        GList* walk;
        err = iSpeechEncoderConfig->GetSupportedBitrates(&gbitrates);

        //TODO: try returning GList and do not convert to RArray
        for (; gbitrates; gbitrates = g_list_next (gbitrates))
            {
            //            guint br = GPOINTER_TO_INT(gbitrates->data);
            guint br = GPOINTER_TO_UINT(gbitrates->data);
            bitrates.Append(br);
            TRACE_PRN_N2(_L("TMS->UPL: GetSupportedBitrates %d:[%d]"), i+1, br);
            i++;
            }
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
        err = iSpeechEncoderConfig->GetBitrate(&bitrate);
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
                err = iG711EncoderIntfc->GetVadMode(&vad);
                TRACE_PRN_N1(_L("TMS->UPL: GetVad [%d]"), vad);
                }
            break;
            }
        case TMS_FORMAT_G729:
            {
            if (iG729EncoderIntfc)
                {
                err = iG729EncoderIntfc->GetVadMode(&vad);
                TRACE_PRN_N1(_L("TMS->UPL: GetVad [%d]"), vad);
                }
            break;
            }
        case TMS_FORMAT_ILBC:
            {
            if (iIlbcEncoderIntfc)
                {
                err = iIlbcEncoderIntfc->GetVadMode(&vad);
                TRACE_PRN_N1(_L("TMS->UPL: GetVad [%d]"), vad);
                }
            break;
            }
        case TMS_FORMAT_AMR:
            {
            if (iSpeechEncoderConfig)
                {
                err = iSpeechEncoderConfig->GetVadMode(&vad);
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

// End of file
