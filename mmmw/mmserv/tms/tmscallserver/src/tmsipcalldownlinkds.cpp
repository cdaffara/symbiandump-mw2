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
#include <AudioOutput.h>
#include <mmcccodecinformation.h>
#include <IlbcDecoderIntfc.h>
#include <G711DecoderIntfc.h>
#include <G729DecoderIntfc.h>
#include <ErrorConcealmentIntfc.h>
#include "tmsutility.h"
#include "tmsipcallstream.h"

using namespace TMS;

// -----------------------------------------------------------------------------
// TMSIPDownlink::TMSIPDownlink
// Standard Constructor
// -----------------------------------------------------------------------------
//
TMSIPDownlink::TMSIPDownlink(TMSIPDevSoundObserver& observer) :
    TMSIPCallStreamBase(observer)
    {
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

    delete iAudioOutput;
    delete iErrConcealmentIntfc;
    delete iG711DecoderIntfc;
    delete iG729DecoderIntfc;
    delete iIlbcDecoderIntfc;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::NewL
// Symbian two-phase constructor
// -----------------------------------------------------------------------------
//
TMSIPDownlink* TMSIPDownlink::NewL(TMSIPDevSoundObserver& observer,
        const guint32 codecID, const TMMFPrioritySettings priority,
        const gint retrytime)
    {
    TMSIPDownlink* self = new (ELeave) TMSIPDownlink(observer);
    CleanupStack::PushL(self);
    self->ConstructL(codecID, priority, retrytime);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::ConstructL
// Part two of Symbian two phase construction
// -----------------------------------------------------------------------------
//
void TMSIPDownlink::ConstructL(const guint32 codecID,
        const TMMFPrioritySettings priority, const gint /*retrytime*/)
    {
    TRACE_PRN_FN_ENT;
    iCodecID = codecID;
    iPriority = priority;

    // Client must set these before querying!
    iG711DecodeMode = TMS_G711_CODEC_MODE_ALAW;
    iILBCDecodeMode = TMS_ILBC_CODEC_MODE_20MS_FRAME;
    TRAPD(err, InitDevSoundL(EMMFStatePlaying, priority));
    if (err != TMS_RESULT_SUCCESS)
        {
        User::Leave(err);
        }

    iMaxBufLen = ConfigureMedia(iCodecID);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::Start
//
// -----------------------------------------------------------------------------
//
void TMSIPDownlink::Start(const gint /*retrytime*/)
    {
    TRACE_PRN_FN_ENT;
    gint err = TMS_RESULT_ILLEGAL_OPERATION;

    if (iStatus == EReady && iDevSound)
        {
        TRAP(err, iDevSound->PlayInitL());
        TRACE_PRN_IF_ERR(err);
        iObserver.DownlinkStarted(err);
        }
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

    if (iStatus == EStreaming && iDevSound)
        {
        iDevSound->Stop();
        iStatus = EReady;
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::BufferToBeFilled
// A reference to the buffer delivered from the DevSound is stored locally
// for later use. It will be filled with the data passed from the client
// when it calls BufferFilled.
//
// -----------------------------------------------------------------------------
//
void TMSIPDownlink::BufferToBeFilled(CMMFBuffer* aBuffer)
    {
    // Store pointer to the received buffer
    iDevSoundBufPtr = static_cast<CMMFDataBuffer*> (aBuffer);
    iBufLen = iDevSoundBufPtr->RequestSize();
    TRACE_PRN_N1(_L("TMS->DNL->BTBF: LEN[%d]"), iBufLen);

#ifndef __WINSCW__
    //TODO: Is this still true?
    // The first AMR buffer returns 1 for no data frame.
    /*if (iCodecID == KMccFourCCIdAMRNB)
     {
     iBufLen = iMaxBufLen;
     }*/
#endif //__WINSCW__

    // Create or adjust the chunk
    gint err = DoChunk(iBufLen, iMsgBuffer);

    if (err != TMS_RESULT_SUCCESS)
        {
        Stop();
        iMsgBuffer.iStatus = err;
        }
    else
        {
        // Notify client there is buffer ready to be filled
        iMsgBuffer.iStatus = err;
        iMsgBuffer.iInt = iBufLen;
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
    err = iMsgQueue.Send(iMsgBuffer);
    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::BufferFilled
//
// -----------------------------------------------------------------------------
//
void TMSIPDownlink::BufferFilled(const guint buflen)
    {
    TRACE_PRN_N1(_L("TMS->DNL->BF: LEN[%d]"), buflen);

    // Copy data over from chunk
    TPtr8 dataPtr(iChunk.Base(), buflen, iMaxBufLen);
    //RDebug::RawPrint(dataPtr);

    if (iStatus == EStreaming && iDevSound && iDevSoundBufPtr)
        {
        // Fill D/S buffer and send it for playback
        iDevSoundBufPtr->Data() = dataPtr;
        TRAP_IGNORE(iDevSoundBufPtr->SetRequestSizeL(buflen));
        //try playing anyway
        iDevSound->PlayData();
        }
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
                iG711DecoderIntfc = CG711DecoderIntfc::NewL(*iDevSound);
                }
            break;
            }
        case KMccFourCCIdG729:
            {
            if (!iG729DecoderIntfc)
                {
                iG729DecoderIntfc = CG729DecoderIntfc::NewL(*iDevSound);
                }
            break;
            }
        case KMccFourCCIdILBC:
            {
            if (!iIlbcDecoderIntfc)
                {
                iIlbcDecoderIntfc = CIlbcDecoderIntfc::NewL(*iDevSound);
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

    if (!iErrConcealmentIntfc && iCodecID != KMMFFourCCCodePCM16)
        {
        iErrConcealmentIntfc = CErrorConcealmentIntfc::NewL(*iDevSound);
        }

    if (!iAudioOutput)
        {
        iAudioOutput = CAudioOutput::NewL(*iDevSound);
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
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iDevSound)
        {
        iDevSound->SetVolume(volume);
        ret = TMS_RESULT_SUCCESS;
        }
    TRACE_PRN_N1(_L("TMS->DNL: SetVolume [%d]"), volume);
    return ret;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::GetVolume
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::GetVolume(guint& volume)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iDevSound)
        {
        volume = iDevSound->Volume();
        ret = TMS_RESULT_SUCCESS;
        }
    TRACE_PRN_N1(_L("TMS->DNL: GetVolume [%d]"), volume);
    return ret;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::GetMaxVolume
//
// -----------------------------------------------------------------------------
//
gint TMSIPDownlink::GetMaxVolume(guint& volume)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iDevSound)
        {
        volume = iDevSound->MaxVolume();
        ret = TMS_RESULT_SUCCESS;
        }
    TRACE_PRN_N1(_L("TMS->DNL: MaxVolume [%d]"), volume);
    return ret;
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
                err = iIlbcDecoderIntfc->SetDecoderMode(
                        CIlbcDecoderIntfc::E20msFrame);
                TRACE_PRN_N(_L("TMS->DNL: iLBC Mode Set [20ms]"));
                }
            else if (mode == TMS_ILBC_CODEC_MODE_30MS_FRAME)
                {
                err = iIlbcDecoderIntfc->SetDecoderMode(
                        CIlbcDecoderIntfc::E30msFrame);
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
                err = iG711DecoderIntfc->SetDecoderMode(
                        CG711DecoderIntfc::EDecALaw);
                TRACE_PRN_N(_L("TMS->DNL: G711 Mode Set [ALaw]"));
                }
            else if (mode == TMS_G711_CODEC_MODE_MULAW)
                {
                err = iG711DecoderIntfc->SetDecoderMode(
                        CG711DecoderIntfc::EDecULaw);
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
            err = iG711DecoderIntfc->GetCng(cng);
            TRACE_PRN_N1(_L("TMS->DNL: GetCng [%d]"), cng);
            }
        else if (fmttype == TMS_FORMAT_ILBC && iIlbcDecoderIntfc)
            {
            err = iIlbcDecoderIntfc->GetCng(cng);
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
// TMSIPDownlink::SetAudioDeviceL
//
// -----------------------------------------------------------------------------
void TMSIPDownlink::SetAudioDeviceL(TMSAudioOutput output)
    {
    TRACE_PRN_FN_ENT;

    if (iAudioOutput)
        {
        // ENoPreference=0, EAll=1, ENoOutput=2, EPrivate=3, EPublic=4
        CAudioOutput::TAudioOutputPreference outputDev;

        switch (output)
            {
            case TMS_AUDIO_OUTPUT_PRIVATE:
            case TMS_AUDIO_OUTPUT_HANDSET:
                outputDev = CAudioOutput::EPrivate;
                break;
            case TMS_AUDIO_OUTPUT_PUBLIC:
            case TMS_AUDIO_OUTPUT_LOUDSPEAKER:
                outputDev = CAudioOutput::EPublic;
                break;
            default: // Use default device routing
                outputDev = CAudioOutput::ENoPreference;
                break;
            }

        iAudioOutput->SetAudioOutputL(outputDev);
        TRACE_PRN_N1(_L("TMS->DNL: SetAudioDeviceL [%d]"), outputDev);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::GetAudioDeviceL
//
// -----------------------------------------------------------------------------
//
void TMSIPDownlink::GetAudioDeviceL(TMSAudioOutput& output)
    {
    TRACE_PRN_FN_ENT;

    if (iAudioOutput)
        {
        CAudioOutput::TAudioOutputPreference outputDev =
                iAudioOutput->AudioOutput();
        TRACE_PRN_N1(_L("TMS->DNL: GetAudioDeviceL [%d]"), outputDev);

        switch (outputDev)
            {
            case CAudioOutput::EPublic:
                output = TMS_AUDIO_OUTPUT_PUBLIC;
                break;
            case CAudioOutput::ENoPreference:
            case CAudioOutput::EAll:
            case CAudioOutput::ENoOutput:
            case CAudioOutput::EPrivate:
            default:
                output = TMS_AUDIO_OUTPUT_PRIVATE;
                break;
            }
        }

    TRACE_PRN_FN_EXT;
    }

// CALLBACKS

// -----------------------------------------------------------------------------
// TMSIPDownlink::InitializeComplete
// A callback from the DevSound indicating completion of the initialization.
// It will send config data to the D/S and configure the encoder via CI.
// If everything goes well, the state of the thread is set EReady.
// The initialization completion message is sent back to the main thread.
// -----------------------------------------------------------------------------
//
void TMSIPDownlink::InitializeComplete(TInt aError)
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
            // We are ready to stream even in case of later CI setting failure
            iStatus = EReady;
            iMaxVolume = iDevSound->MaxVolume();
            }

        // Init Custom Interface API to the Decoder. Any return error can
        // be ignored as codec can still run in the default mode even if not
        // fully configured.
        SetCodecCi();
        }

    iObserver.DownlinkInitCompleted(status);
    TRACE_PRN_IF_ERR(status);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSIPDownlink::PlayError
// From MDevSoundObserver
// Record error is send to client over comm channel.
// The state of recorder is rolled back to EReady.
// -----------------------------------------------------------------------------
//
void TMSIPDownlink::PlayError(TInt aError)
    {
    iStatus = EReady;

#ifndef __WINSCW__
    iObserver.DownlinkStarted(aError);
#else  //__WINSCW__
    if (aError == KErrAccessDenied || aError == KErrInUse)
        {
        iObserver.DownlinkStarted(TMS_RESULT_SUCCESS);
        }
#endif //__WINSCW__

    TRACE_PRN_IF_ERR(aError);
    }

// End of file
