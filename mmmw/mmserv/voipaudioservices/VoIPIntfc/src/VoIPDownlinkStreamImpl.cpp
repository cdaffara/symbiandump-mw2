/*
 * Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  VOIP Audio Services  Downlink stream APIs.
 *
 */

#include <e32base.h>
#include <mmffourcc.h>
#include <voipdatabuffer.h>
#include "debugtracemacros.h"
#include "VoIPAudioSession.h"
#include "VoIPAudioClientServer.h"
#include "VoIPFormatIntfcImpl.h"
#include "VoIPBaseCodecIntfcImpl.h"
#include "VoIPILBCDecoderIntfcImpl.h"
#include "VoIPG711DecoderIntfcImpl.h"
#include "VoIPG729DecoderIntfcImpl.h"
#include "VoIPJitterBufferIntfcImpl.h"
#include "VoIPSharedData.h"
#include "VoIPDownlinkStreamImpl.h"

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::NewL
// ---------------------------------------------------------------------------
//
CVoIPAudioDownlinkStreamImpl*
CVoIPAudioDownlinkStreamImpl::NewL(const TMMFPrioritySettings aPriority)
    {
    CVoIPAudioDownlinkStreamImpl* self =
            new (ELeave) CVoIPAudioDownlinkStreamImpl();
    CleanupStack::PushL(self);
    self->ConstructL(aPriority);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::~CVoIPAudioDownlinkStreamImpl
// ---------------------------------------------------------------------------
//
CVoIPAudioDownlinkStreamImpl::~CVoIPAudioDownlinkStreamImpl()
    {
    Close();
    iCodecFormats.Close();

    if (iMsgQHandler)
        {
        iMsgQHandler->Cancel();
        delete iMsgQHandler;
        }

    if (iMsgQComHandler)
        {
        iMsgQComHandler->Cancel();
        delete iMsgQComHandler;
        }

    if (iMsgQueue.Handle() > 0)
        {
        iMsgQueue.Close();
        }

    if (iMsgComQueue.Handle() > 0)
        {
        iMsgComQueue.Close();
        }

    if (iVoIPAudioSession.Handle() > 0)
        {
        iVoIPAudioSession.Close();
        }
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::CVoIPAudioDownlinkStreamImpl
// ---------------------------------------------------------------------------
//
CVoIPAudioDownlinkStreamImpl::CVoIPAudioDownlinkStreamImpl() :
    iBufferPtr(0, 0, 0)
    {
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPAudioDownlinkStreamImpl::ConstructL(
        const TMMFPrioritySettings aPriority)
    {
    // Pass ownership to the parent class
    CVoIPAudioDownlinkStream::ConstructL(this);
    iPriority = aPriority;
    iG711FrameSize = KVoIPG711FrameLen20ms;
    iFormat = EG711;
    TInt err = iVoIPAudioSession.Connect();
    User::LeaveIfError(err);
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::Open
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioDownlinkStreamImpl::Open(MVoIPDownlinkObserver& aObserver)
    {
    iObserver = &aObserver;

    TInt err = CreateQueue(KDnLinkQueue, KVoIPMsgQSlots, EMsgBufQueue);

    if (err == KErrNone)
        {
        err = CreateQueue(KDnLinkComQueue, KVoIPMsgComQSlots, EMsgComQueue);
        }

    if (err == KErrNone)
        {
        err = iVoIPAudioSession.OpenDownlink(iPriority);
        }

    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::GetVersion
// Returns version of this component.
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioDownlinkStreamImpl::GetVersion(TVersion& aVersion)
    {
    aVersion = TVersion(1, 0, 0);
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::GetMaxVolume
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioDownlinkStreamImpl::GetMaxVolume(TInt& aVolume)
    {
    aVolume = iVoIPAudioSession.GetMaxVolume();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::SetVolume
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioDownlinkStreamImpl::SetVolume(TInt aVolume)
    {
    TInt err = iVoIPAudioSession.SetVolume(aVolume);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::GetVolume
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioDownlinkStreamImpl::GetVolume(TInt& aVolume)
    {
    aVolume = iVoIPAudioSession.GetVolume();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::GetSupportedFormatsL
// ---------------------------------------------------------------------------
//
void CVoIPAudioDownlinkStreamImpl::GetSupportedFormatsL(
        const TMMFPrioritySettings aPriority,
        RArray<TVoIPCodecFormat>& aFormats)
    {
    TInt count = iCodecFormats.Count();
    aFormats.Reset();
    if (count > 0)
        {
        for (TInt i = 0; i < count; i++)
            {
            aFormats.Append(iCodecFormats[i]);
            }
        }
    else
        {
        RArray<TUint32> codecIDs;
        CleanupClosePushL(codecIDs);
        codecIDs.Reset();
        iVoIPAudioSession.GetSupportedDecoders(aPriority, codecIDs,
                iG711FrameSize);

#ifdef _DEBUG
        RDebug::Print(_L("G711 Frame Size=%d"), iG711FrameSize);
#endif

        TUint32 codec = 0;
        TInt count = codecIDs.Count();
        TVoIPCodecFormat format;
        iCodecFormats.Reset();

        for (TInt i = 0; i < count; i++)
            {
            codec = codecIDs[i];

#ifdef _DEBUG
            TInt8 a = codec;
            TInt8 b = codec >> 8;
            TInt8 c = codec >> 16;
            TInt8 d = codec >> 24;
            RDebug::Print(_L("%c%c%c%c"), a, b, c, d);
#endif
            format = ConvertFourCC(codec);
            if (format != ENULL)
                {
                iCodecFormats.Append(format);
                aFormats.Append(format);
                }
            }
        CleanupStack::PopAndDestroy(&codecIDs);
        }
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::SetFormatL
// ---------------------------------------------------------------------------
//
void CVoIPAudioDownlinkStreamImpl::SetFormatL(TVoIPCodecFormat aFormat,
        CVoIPFormatIntfc*& aIntfc)
    {
    // Check if requested codec is on the list of supported codecs
    if (!IsCodecSupportedL(aFormat))
        {
        iFormat = ENULL;
        }

    // TODO: reuse when switching codecs on the fly and same codec is selected
    delete aIntfc;
    aIntfc = NULL;

    switch (iFormat)
        {
        case EG711:
        case EG711_10MS:
            {
            aIntfc = CVoIPG711DecoderIntfcImpl::NewL(this);
            iBufferLen = iG711FrameSize;
            break;
            }
        case EG729:
            {
            aIntfc = CVoIPG729DecoderIntfcImpl::NewL(this);
            iBufferLen = KVoIPG729FrameLen;
            break;
            }
        case EILBC:
            {
            aIntfc = CVoIPILBCDecoderIntfcImpl::NewL(this);
            iBufferLen = KVoIPILBCFrameLen;
            break;
            }
        case EAMR_NB:
            {
            aIntfc = CVoIPBaseCodecIntfcImpl::NewL(this);
            iBufferLen = KVoIPAMRNBFrameLen;
            break;
            }
        case EPCM16:
            {
            aIntfc = CVoIPBaseCodecIntfcImpl::NewL(this);
            iBufferLen = KVoIPPCM16FrameLen;
            break;
            }
        default:
            {
            iFormat = ENULL;
            iBufferLen = 0;
            User::Leave(KErrNotSupported);
            }
        }

    TUint32 codecFourCC = CodecFourCC(iFormat);
    iVoIPAudioSession.SetDecoder(codecFourCC);
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::SetAudioDevice
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioDownlinkStreamImpl::SetAudioDevice(
        const TVoIPOutputDevice aDevice)
    {
    TInt err = iVoIPAudioSession.SetAudioDevice(aDevice);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::GetAudioDevice
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioDownlinkStreamImpl::GetAudioDevice(TVoIPOutputDevice& aDevice)
    {
    TInt err = iVoIPAudioSession.GetAudioDevice(aDevice);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::Start
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioDownlinkStreamImpl::Start()
    {
    TInt err = iVoIPAudioSession.StartDownlink();
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::Stop
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioDownlinkStreamImpl::Stop()
    {
    TInt err = iVoIPAudioSession.StopDownlink();
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::Close
// ---------------------------------------------------------------------------
//
void CVoIPAudioDownlinkStreamImpl::Close()
    {
    iVoIPAudioSession.CloseDownlink();
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::BufferFilled
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioDownlinkStreamImpl::BufferFilled(CVoIPDataBuffer* aBuffer)
    {
    TUint bufSeq = 0;
    aBuffer->GetPayloadPtr(iBufferPtr);
    CVoIPDataBuffer::TVoIPBufferType type;
    aBuffer->GetBufferType(type);

    if (type == CVoIPDataBuffer::EJitterBuffer)
        {
        static_cast<CVoIPJBDataBuffer*> (aBuffer)->GetBufferSequence(bufSeq);
        }

    TInt err = iVoIPAudioSession.BufferFilled(iBufferPtr, bufSeq);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::CreateJitterBufferIntfcL
// ---------------------------------------------------------------------------
//
void CVoIPAudioDownlinkStreamImpl::CreateJitterBufferIntfcL(
        CVoIPJitterBufferIntfc*& aJBIntfc)
    {
    aJBIntfc = CVoIPJitterBufferIntfcImpl::NewL(&iVoIPAudioSession);
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::GetSession
// ---------------------------------------------------------------------------
//
RVoIPAudioSession* CVoIPAudioDownlinkStreamImpl::GetSession()
    {
    return &iVoIPAudioSession;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioStream::CreateQueue
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioDownlinkStreamImpl::CreateQueue(const TDesC& aQueueName,
        const TInt aNumSlots, const TQueueType aQueueType)
    {
    TInt err = KErrNone;

    if (aQueueType == EMsgBufQueue)
        {
        if (iMsgQueue.Handle() <= 0)
            {
            err = iMsgQueue.CreateGlobal(aQueueName, aNumSlots);
            if (err != KErrNone)
                {
                return err;
                }
            }

        TRAP(err, ReceiveMsgQHandlerEventsL());
        }
    else if (aQueueType == EMsgComQueue)
        {
        if (iMsgComQueue.Handle() <= 0)
            {
            err = iMsgComQueue.CreateGlobal(aQueueName, aNumSlots);
            if (err != KErrNone)
                {
                return err;
                }
            }

        TRAP(err, ReceiveMsgQComHandlerEventsL());
        }

    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioStream::ReceiveMsgQHandlerEventsL
// Starts message queue handler (A/O) to monitor server side events
// ---------------------------------------------------------------------------
//
void CVoIPAudioDownlinkStreamImpl::ReceiveMsgQHandlerEventsL()
    {
    if (iMsgQHandler)
        {
        iMsgQHandler->Cancel();
        }
    else
        {
        iMsgQHandler = CQueueHandler::NewL(this, &iMsgQueue, iBufferLen);
        }

    iMsgQHandler->Start();
    }

// ---------------------------------------------------------------------------
// CVoIPAudioStream::ReceiveMsgQComHandlerEventsL
// Starts message queue handler (A/O) to monitor server side events
// ---------------------------------------------------------------------------
//
void CVoIPAudioDownlinkStreamImpl::ReceiveMsgQComHandlerEventsL()
    {
    if (iMsgQComHandler)
        {
        iMsgQComHandler->Cancel();
        }
    else
        {
        iMsgQComHandler = CQueueHandler::NewL(this, &iMsgComQueue);
        }

    iMsgQComHandler->Start();
    }

// ---------------------------------------------------------------------------
// CVoIPAudioStream::CodecFourCC
// ---------------------------------------------------------------------------
//
TUint32 CVoIPAudioDownlinkStreamImpl::CodecFourCC(TVoIPCodecFormat aFormat)
    {
    TUint32 codecFourCC;

    switch (aFormat)
        {
        case EG711_10MS:
        case EG711:
            {
            codecFourCC = KMccFourCCIdG711;
            break;
            }
        case EG729:
            {
            codecFourCC = KMccFourCCIdG729;
            break;
            }
        case EILBC:
            {
            codecFourCC = KMccFourCCIdILBC;
            break;
            }
        case EAMR_NB:
            {
            codecFourCC = KMccFourCCIdAMRNB;
            break;
            }
        case EPCM16:
            {
            codecFourCC = KMMFFourCCCodePCM16;
            break;
            }
        default:
            {
            codecFourCC = KFourCCNULL;
            }
        }

    return codecFourCC;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::ConvertFourCC
// -----------------------------------------------------------------------------
//
TVoIPCodecFormat CVoIPAudioDownlinkStreamImpl::ConvertFourCC(TUint32 aCodec)
    {
    TVoIPCodecFormat format;

    switch (aCodec)
        {
        case KMccFourCCIdG711:
            {
            if (iG711FrameSize == KVoIPG711FrameLen10ms)
                {
                format = EG711_10MS;
                TRACE_PRN_N(_L("VoIP->DNL: G711 10ms frame rate detected"));
                }
            else
                {
                format = EG711;
                }
            break;
            }
        case KMccFourCCIdG729:
            format = EG729;
            break;
        case KMccFourCCIdILBC:
            format = EILBC;
            break;
        case KMccFourCCIdAMRNB:
            format = EAMR_NB;
            break;
        case KMMFFourCCCodePCM16:
            format = EPCM16;
            break;
        default:
            format = ENULL;
            break;
        }

    return format;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::IsCodecSupportedL
// -----------------------------------------------------------------------------
//
TBool CVoIPAudioDownlinkStreamImpl::IsCodecSupportedL(
        TVoIPCodecFormat aFormat)
    {
    TBool status = EFalse;

    if (iCodecFormats.Count() > 0)
        {
        status = FindFormat(aFormat);
        }
    else
        {
        // Client hasn't called GetSupportedFormatsL
        RArray<TVoIPCodecFormat> codecs;
        CleanupClosePushL(codecs);
        codecs.Reset();
        GetSupportedFormatsL(iPriority, codecs); //sets iCodecFormats
        status = FindFormat(aFormat);
        CleanupStack::PopAndDestroy(&codecs);
        }

    return status;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::FindFormat
// -----------------------------------------------------------------------------
//
TBool CVoIPAudioDownlinkStreamImpl::FindFormat(TVoIPCodecFormat aFormat)
    {
    TBool found = EFalse;

    if (iCodecFormats.Count() > 0)
        {
        if (iCodecFormats.Find(aFormat) == KErrNotFound)
            {
            // For backward compatibility with VAS 1.0
            if (aFormat == EG711)
                {
                if (iCodecFormats.Find(EG711_10MS) != KErrNotFound)
                    {
                    iFormat = EG711_10MS;
                    found = ETrue;
                    }
                }
            else if (aFormat == EG711_10MS)
                {
                if (iCodecFormats.Find(EG711) != KErrNotFound)
                    {
                    iFormat = EG711;
                    found = ETrue;
                    }
                }
            }
        else
            {
            iFormat = aFormat;
            found = ETrue;
            }
        }

    return found;
    }

// ======== CALLBACK FUNCTIONS ========

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::FillBuffer
// From MQueueHandlerObserver
// ---------------------------------------------------------------------------
//
void CVoIPAudioDownlinkStreamImpl::FillBuffer(CVoIPDataBuffer* aBuffer)
    {
    iObserver->FillBuffer(*this, aBuffer);
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStreamImpl::Event
// From MQueueHandlerObserver
// ---------------------------------------------------------------------------
//
void CVoIPAudioDownlinkStreamImpl::Event(TInt aEventType, TInt aError)
    {
    iObserver->Event(*this, aEventType, aError);
    }

// End of file
