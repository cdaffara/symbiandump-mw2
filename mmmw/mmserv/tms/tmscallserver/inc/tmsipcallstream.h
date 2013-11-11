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

#ifndef __IPCALLSTREAM_H
#define __IPCALLSTREAM_H

//#define __PLAY_WAV_FROM_FILE__

#include <e32msgqueue.h>
#include <mmfbase.h>
#include <tms.h>
#include "tmsgstreamer.h"
#include "tmsshared.h"
#include "tmsclientserver.h"

#ifdef __USE_GSTREAMER__
#include <gst.h>
#include "gstg711encoderinterface.h"
#include "gstspeechencoderconfig.h"
#include "gsterrorconcealmentinterface.h"
#include "gstg711decoderinterface.h"
#include "gstilbcdecoderinterface.h"
#include "gstilbcencoderinterface.h"
#include "gstg729encoderinterface.h"
#include "gstg729decoderinterface.h"
#else //__USE_GSTREAMER__
#include <sounddevice.h>
#include <voipdownlinkstream.h>
#include <voipuplinkstream.h>

// FORWARD DECLARATIONS
class CAudioOutput;
class CErrorConcealmentIntfc;
class CSpeechEncoderConfig;
class CG711DecoderIntfc;
class CG729DecoderIntfc;
class CIlbcDecoderIntfc;
class CG711EncoderIntfc;
class CG729EncoderIntfc;
class CIlbcEncoderIntfc;
#endif //__USE_GSTREAMER__

namespace TMS {

/**
 *  TMSIPDevSoundObserver
  * An observer interface providing TMSIPCallStreamBase event notifications.
 */
class TMSIPDevSoundObserver
    {
public:
    virtual void DownlinkInitCompleted(gint status) = 0;
    virtual void UplinkInitCompleted(gint status) = 0;
    virtual void UplinkStarted(gint status) = 0;
    virtual void DownlinkStarted(gint status) = 0;
    };

// -----------------------------------------------------------------------------
//  Class Name:  TMSIPCallStreamBase
//
//  Base abstract class for creating VoIP uplink and downlink streams.
// -----------------------------------------------------------------------------
class TMSIPCallStreamBase : public CBase
#ifndef __USE_GSTREAMER__
                          , public MDevSoundObserver
#endif //__USE_GSTREAMER__
    {
public:
    enum TStreamState
        {
        ENotReady, // not initialized
        EReady, // initialized and ready for streaming
        EStreaming
        // active streaming
        };

public:
    virtual ~TMSIPCallStreamBase();

    virtual void Start(const gint retrytime) = 0;
    virtual void Stop() = 0;
    virtual gint SetCodecCi() = 0;

#ifndef __USE_GSTREAMER__
    void InitDevSoundL(const TMMFState aDevSoundState,
            const TMMFPrioritySettings priority);
#endif //__USE_GSTREAMER__
    void SetMsgQueue(const RMsgQueue<TmsMsgBuf> aMsgQueue);
    gint DoChunk(const gint aDataLen, TmsMsgBuf& aMsgBuffer);
    gint ConfigureMedia(const guint32 aCodecID);

protected:
    TMSIPCallStreamBase(TMSIPDevSoundObserver& observer);

#ifndef __USE_GSTREAMER__
    // From MDevSoundObserver
    virtual void InitializeComplete(TInt aError) = 0;
    virtual void BufferToBeEmptied(CMMFBuffer* /*aBuffer*/) {}
    virtual void RecordError(TInt /*aError*/) {}
    virtual void BufferToBeFilled(CMMFBuffer* /*aBuffer*/) {}
    virtual void PlayError(TInt /*aError*/) {}
    virtual void ToneFinished(TInt /*aError*/) {}
    virtual void ConvertError(TInt /*aError*/) {}
    virtual void DeviceMessage(TUid /*aMessageType*/, const TDesC8& /*aMsg*/) {}
    virtual void SendEventToClient(const TMMFEvent& /*aEvent*/) {}
#endif //__USE_GSTREAMER__

protected:
    TMSIPDevSoundObserver& iObserver;
    CActiveScheduler* iActiveScheduler;
    TStreamState iStatus;

#ifndef __USE_GSTREAMER__
    CMMFDevSound* iDevSound;
    CMMFDataBuffer* iDevSoundBufPtr;
#endif //__USE_GSTREAMER__

    RMsgQueue<TmsMsgBuf> iMsgQueue; // ITC back to client
    TmsMsgBuf iMsgBuffer;
    RChunk iChunk;
    gint iMaxBufLen;
    guint32 iCodecID;
    TMMFPrioritySettings iPriority;
    const gchar* iMediaType;

    gint iG711DecodeMode;
    gint iILBCDecodeMode;
    gint iG711EncodeMode;
    gint iILBCEncodeMode;
    gint iBufLen;
    TBool iWriteDataXferHndlToClient;
    TUint32 iKey;
    };

// -----------------------------------------------------------------------------
//  Class Name:  TMSIPDownlink
//
//  Class handling VoIP downlink stream object.
// -----------------------------------------------------------------------------
class TMSIPDownlink : public TMSIPCallStreamBase
    {
public:
    virtual ~TMSIPDownlink();

#ifdef __USE_GSTREAMER__
    static void cb_raw_playback_handoff(GstElement* appsrc, guint size);
    static gboolean bus_call(GstBus* bus, GstMessage* msg, gpointer data);
#endif //__USE_GSTREAMER__

    static TMSIPDownlink* NewL(TMSIPDevSoundObserver& observer,
            const guint32 codecID, const TMMFPrioritySettings priority,
            const gint retrytime);

    gint SetCodecCi();
    void Start(const gint retrytime);
    void Stop();
    void BufferFilled(const guint buflen);
    gint SetVolume(const guint volume);
    gint GetVolume(guint& volume);
    gint GetMaxVolume(guint& volume);
    gint GetDataXferChunkHndl(const TUint32 key, RChunk& chunk);

    // For CIs
    gint SetIlbcCodecMode(const gint mode);
    gint GetIlbcCodecMode(gint& mode);
    gint SetG711CodecMode(const gint mode);
    gint GetG711CodecMode(gint& mode);
    gint SetCng(const TMSFormatType fmttype, const gboolean cng);
    gint GetCng(const TMSFormatType fmttype, gboolean& cng);
    gint SetPlc(const TMSFormatType fmttype, const gboolean plc);
    gint GetPlc(const TMSFormatType fmttype, gboolean& plc);

    void SetAudioDeviceL(TMSAudioOutput output);
    void GetAudioDeviceL(TMSAudioOutput& output);

protected:
    TMSIPDownlink(TMSIPDevSoundObserver& observer);
    void ConstructL(const guint32 codecID,
            const TMMFPrioritySettings priority, const gint retrytime);

private:
    void SetCodecCiL();

#ifdef __USE_GSTREAMER__
    void BufferToBeFilled();
    gint gst_play_raw();
    gint gst_initialize_play();
#else //__USE_GSTREAMER__
    // From MDevSoundObserver
    virtual void BufferToBeFilled(CMMFBuffer* aBuffer);
    virtual void InitializeComplete(TInt aError);
    virtual void PlayError(TInt aError);
#endif //__USE_GSTREAMER__

private:

    gboolean iFrameMode;
    gboolean iPlc;
    gint iMaxVolume;

#ifndef __USE_GSTREAMER__
    CAudioOutput* iAudioOutput;
    CErrorConcealmentIntfc* iErrConcealmentIntfc;
    CG711DecoderIntfc* iG711DecoderIntfc;
    CG729DecoderIntfc* iG729DecoderIntfc;
    CIlbcDecoderIntfc* iIlbcDecoderIntfc;
#else //__USE_GSTREAMER__
    GstErrorConcealmentIntfc* iErrConcealmentIntfc;
    GstG711DecoderIntfc* iG711DecoderIntfc;
    GstG729DecoderIntfc* iG729DecoderIntfc;
    GstIlbcDecoderIntfc* iIlbcDecoderIntfc;
    GstElement* iPipelinePlay;
    GstElement* iAppSrc;
    GstElement* iSink;
    GstBus* iBusPlay;
#endif //__USE_GSTREAMER__

#ifdef __PLAY_WAV_FROM_FILE__
    RFile iFile;
    gint fsize;
    gint iReadSize;
    gint iReadBytes;
    RFs iFs;
    HBufC8* fbuf;
#endif
    };

// -----------------------------------------------------------------------------
//  Class Name:  TMSIPUplink
//
//  Class handling VoIP uplink stream.
// -----------------------------------------------------------------------------
class TMSIPUplink : public TMSIPCallStreamBase
    {
public:
    virtual ~TMSIPUplink();

#ifdef __USE_GSTREAMER__
    static void cb_record_raw_handoff(GstElement *sink);
    static gboolean bus_call(GstBus* bus, GstMessage* msg, gpointer data);
#endif //__USE_GSTREAMER__

    static TMSIPUplink* NewL(TMSIPDevSoundObserver& observer,
            const guint32 codecID, const TMMFPrioritySettings priority,
            const gint retrytime);

    gint SetCodecCi();
    void Start(const gint retrytime);
    void Stop();
    void BufferEmptied();
    gint SetGain(const guint gain);
    gint GetGain(guint& gain);
    gint GetMaxGain(guint& gain);
    gint GetDataXferChunkHndl(const TUint32 key, RChunk& chunk);

    // For CIs
    gint SetIlbcCodecMode(const gint mode);
    gint GetIlbcCodecMode(gint& mode);
    gint SetG711CodecMode(const gint mode);
    gint GetG711CodecMode(gint& mode);
    gint GetSupportedBitrates(RArray<TUint>& bitrates);
    gint SetBitrate(guint bitrate);
    gint GetBitrate(guint& bitrate);
    gint SetVad(const TMSFormatType fmttype, const gboolean vad);
    gint GetVad(const TMSFormatType fmttype, gboolean& vad);

protected:
    TMSIPUplink(TMSIPDevSoundObserver& observer);
    void ConstructL(const guint32 codecID,
            const TMMFPrioritySettings priority, const gint retrytime);

private:
    void SetCodecCiL();

#ifdef __USE_GSTREAMER__
    void BufferToBeEmptied();
    gint gst_record_raw();
    gint gst_initialize_record();
#else //__USE_GSTREAMER__
    // From MDevSoundObserver
    virtual void BufferToBeEmptied(CMMFBuffer* aBuffer);
    virtual void InitializeComplete(TInt aError);
    virtual void RecordError(TInt /*aError*/);
#endif //__USE_GSTREAMER__

private:
    gint iMaxGain;

#ifndef __USE_GSTREAMER__
    CSpeechEncoderConfig* iSpeechEncoderConfig;
    CG711EncoderIntfc* iG711EncoderIntfc;
    CG729EncoderIntfc* iG729EncoderIntfc;
    CIlbcEncoderIntfc* iIlbcEncoderIntfc;
#else //__USE_GSTREAMER__
    GstSpeechEncoderConfigIntfc* iSpeechEncoderConfig;
    GstG711EncoderIntfc* iG711EncoderIntfc;
    GstG729EncoderIntfc* iG729EncoderIntfc;
    GstIlbcEncoderIntfc* iIlbcEncoderIntfc;
    GstElement* iPipelineRec;
    GstElement* iSource;
    GstElement* iAppSink;
    GstBus* iBusRec;
#endif //__USE_GSTREAMER__
    };

} //namespace TMS

#endif  //__IPCALLSTREAM_H

// End of file
