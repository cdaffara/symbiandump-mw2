/*
 * Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  VoIP Audio Services - Uplink stream API
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
#include "VoIPILBCEncoderIntfcImpl.h"
#include "VoIPG711EncoderIntfcImpl.h"
#include "VoIPSharedData.h"
#include "VoIPUplinkStreamImpl.h"

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::NewL
// ---------------------------------------------------------------------------
//
CVoIPAudioUplinkStreamImpl*
CVoIPAudioUplinkStreamImpl::NewL(const TMMFPrioritySettings aPriority)
    {
    CVoIPAudioUplinkStreamImpl* self =
            new (ELeave) CVoIPAudioUplinkStreamImpl();
    CleanupStack::PushL(self);
    self->ConstructL(aPriority);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::~CVoIPAudioUplinkStreamImpl
// ---------------------------------------------------------------------------
//
CVoIPAudioUplinkStreamImpl::~CVoIPAudioUplinkStreamImpl()
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
// CVoIPAudioUplinkStreamImpl::CVoIPAudioUplinkStreamImpl
// ---------------------------------------------------------------------------
//
CVoIPAudioUplinkStreamImpl::CVoIPAudioUplinkStreamImpl() :
    iBufferPtr(0, 0, 0)
    {
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPAudioUplinkStreamImpl::ConstructL(
        const TMMFPrioritySettings aPriority)
    {
    // Pass ownership to the parent class
    CVoIPAudioUplinkStream::ConstructL(this);
    iPriority = aPriority;
    iG711FrameSize = KVoIPG711FrameLen20ms;
    iFormat = EG711;
    TInt err = iVoIPAudioSession.Connect();
    User::LeaveIfError(err);
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::Open
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioUplinkStreamImpl::Open(MVoIPUplinkObserver& aObserver)
    {
    iObserver = &aObserver;

    TInt err = CreateQueue(KUpLinkQueue, KVoIPMsgQSlots, EMsgBufQueue);

    if (err == KErrNone)
        {
        err = CreateQueue(KUpLinkComQueue, KVoIPMsgComQSlots, EMsgComQueue);
        }

    if (err == KErrNone)
        {
        err = iVoIPAudioSession.OpenUplink(iPriority);
        }

    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::GetVersion
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioUplinkStreamImpl::GetVersion(TVersion& aVersion)
    {
    aVersion = TVersion(1, 0, 0);
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::GetMaxGain
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioUplinkStreamImpl::GetMaxGain(TInt& aGain)
    {
    aGain = iVoIPAudioSession.GetMaxGain();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::SetGain
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioUplinkStreamImpl::SetGain(TInt aGain)
    {
    TInt err = iVoIPAudioSession.SetGain(aGain);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::GetGain
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioUplinkStreamImpl::GetGain(TInt& aGain)
    {
    aGain = iVoIPAudioSession.GetGain();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::GetSupportedFormatsL
// ---------------------------------------------------------------------------
//
void CVoIPAudioUplinkStreamImpl::GetSupportedFormatsL(
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
        iVoIPAudioSession.GetSupportedEncoders(aPriority, codecIDs,
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
// CVoIPAudioUplinkStreamImpl::SetFormatL
// ---------------------------------------------------------------------------
//
void CVoIPAudioUplinkStreamImpl::SetFormatL(TVoIPCodecFormat aFormat,
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
            aIntfc = CVoIPG711EncoderIntfcImpl::NewL(this);
            iBufferLen = iG711FrameSize;
            break;
            }
        case EG729:
            {
            aIntfc = CVoIPBaseCodecIntfcImpl::NewL(this);
            iBufferLen = KVoIPG729FrameLen;
            break;
            }
        case EILBC:
            {
            aIntfc = CVoIPILBCEncoderIntfcImpl::NewL(this);
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
    iVoIPAudioSession.SetEncoder(codecFourCC);
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::Start
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioUplinkStreamImpl::Start()
    {
    iIsClosing = EFalse;
    TInt err = iVoIPAudioSession.StartUplink();
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::Stop
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioUplinkStreamImpl::Stop()
    {
    TInt err = iVoIPAudioSession.StopUplink();
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::Close
// ---------------------------------------------------------------------------
//
void CVoIPAudioUplinkStreamImpl::Close()
    {
    iIsClosing = ETrue;
    iVoIPAudioSession.CloseUplink();
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::BufferEmptied
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioUplinkStreamImpl::BufferEmptied(CVoIPDataBuffer* aBuffer)
    {
    aBuffer->GetPayloadPtr(iBufferPtr);
    TInt err = iVoIPAudioSession.BufferEmptied(iBufferPtr);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::GetSession
// ---------------------------------------------------------------------------
//
RVoIPAudioSession* CVoIPAudioUplinkStreamImpl::GetSession()
    {
    return &iVoIPAudioSession;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::CreateQueue
// ---------------------------------------------------------------------------
//
TInt CVoIPAudioUplinkStreamImpl::CreateQueue(const TDesC& aQueueName,
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
// CVoIPAudioUplinkStreamImpl::ReceiveMsgQHandlerEventsL
// Starts message queue handler (A/O) to monitor server side events
// ---------------------------------------------------------------------------
//
void CVoIPAudioUplinkStreamImpl::ReceiveMsgQHandlerEventsL()
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
// CVoIPAudioUplinkStreamImpl::ReceiveMsgQComHandlerEventsL
// Starts message queue handler (A/O) to monitor server side events
// ---------------------------------------------------------------------------
//
void CVoIPAudioUplinkStreamImpl::ReceiveMsgQComHandlerEventsL()
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
// CVoIPAudioUplinkStreamImpl::CodecFourCC
// ---------------------------------------------------------------------------
//
TUint32 CVoIPAudioUplinkStreamImpl::CodecFourCC(TVoIPCodecFormat aFormat)
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
// CVoIPAudioUplinkStreamImpl::ConvertFourCC
// -----------------------------------------------------------------------------
//
TVoIPCodecFormat CVoIPAudioUplinkStreamImpl::ConvertFourCC(TUint32 aCodec)
    {
    TVoIPCodecFormat format;

    switch (aCodec)
        {
        case KMccFourCCIdG711:
            {
            if (iG711FrameSize == KVoIPG711FrameLen10ms)
                {
                format = EG711_10MS;
                TRACE_PRN_N(_L("VoIP->UPL: G711 10ms frame rate detected"));
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
// CVoIPAudioUplinkStreamImpl::IsCodecSupportedL
// -----------------------------------------------------------------------------
//
TBool CVoIPAudioUplinkStreamImpl::IsCodecSupportedL(TVoIPCodecFormat aFormat)
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
// CVoIPAudioUplinkStreamImpl::FindFormat
// -----------------------------------------------------------------------------
//
TBool CVoIPAudioUplinkStreamImpl::FindFormat(TVoIPCodecFormat aFormat)
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
// CVoIPAudioUplinkStreamImpl::EmptyBuffer
// ---------------------------------------------------------------------------
//
void CVoIPAudioUplinkStreamImpl::EmptyBuffer(CVoIPDataBuffer* aBuffer)
    {
    // Recorded packets may still be flowing despite client's Close request.
    // It is safer just to throw them away here.
    if (!iIsClosing)
        {
        iObserver->EmptyBuffer(*this, aBuffer);
        }
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStreamImpl::Event
// ---------------------------------------------------------------------------
//
void CVoIPAudioUplinkStreamImpl::Event(TInt aEventType, TInt aError)
    {
    iObserver->Event(*this, aEventType, aError);
    }

// End of file
