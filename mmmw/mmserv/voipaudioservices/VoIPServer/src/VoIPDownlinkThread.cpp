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
 * Description:  VoIP Audio Services
 *               Implements downlink thread object.
 *
 */

#include <AudioOutput.h>
#include <IlbcDecoderIntfc.h>
#include <G711DecoderIntfc.h>
#include <G729DecoderIntfc.h>
#include <ErrorConcealmentIntfc.h>
#include <voipdownlinkstream.h>
#include "debugtracemacros.h"
#include "VoIPSharedData.h"
#include "VoIPServerThread.h"

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::CVoIPDownlinkThread
// Standard Constructor
// -----------------------------------------------------------------------------
//
CVoIPDownlinkThread::CVoIPDownlinkThread(TSharedData& aData) :
    iShared(aData)
    {
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::~CVoIPDownlinkThread
// Standard Constructor
// -----------------------------------------------------------------------------
//
CVoIPDownlinkThread::~CVoIPDownlinkThread()
    {
    TRACE_PRN_FN_ENT;

    Stop();

    delete iJitterBuffer;
    delete iAddJBuffer;
    delete iGetJBuffer;
    delete iAudioOutput;
    delete iErrConcealmentIntfc;
    delete iG711DecoderIntfc;
    delete iG729DecoderIntfc;
    delete iIlbcDecoderIntfc;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::NewL
// Symbian two-phase constructor
// -----------------------------------------------------------------------------
//
CVoIPDownlinkThread* CVoIPDownlinkThread::NewL(TSharedData& aData)
    {
    CVoIPDownlinkThread* self = new (ELeave) CVoIPDownlinkThread(aData);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::ConstructL
// Part two of Symbian two phase construction
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::ConstructL()
    {
    TRACE_PRN_FN_ENT;

    InitThreadL();
    iCodecID = iShared.iCodecSettings.iFourCC;
    InitMsgQueuesL(KDnLinkQueue, KDnLinkThreadComQueue);

    iShared.iMutex.Wait();
    TRAPD(err, InitDevSoundL(EMMFStatePlaying, iShared.iPriority,
            iShared.iPreference));
    iShared.iMutex.Signal();

    if (err != KErrNone)
        {
        SendCmd(ECmdDnLinkError, err);
        }
    iMaxBufLen = DetermineMaxBufferLen(iShared.iCodecSettings.iG711FrameRate);

    // Client must set these before querying!
    iG711CodecMode = CVoIPFormatIntfc::EG711ALaw;
    iILBCCodecMode = CVoIPFormatIntfc::EiLBC20mSecFrame;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::ThreadFunction
// Thread Startup function
// -----------------------------------------------------------------------------
//
TInt CVoIPDownlinkThread::ThreadFunction(TAny* aData)
    {
    // get a pointer to the shared data object
    TSharedData& shared = *((TSharedData*) aData);

    // we can set the sync flag here
    shared.iMutex.Wait();

    // create a cleanup stack
    CTrapCleanup* cleanupStack = CTrapCleanup::New();

    if (!cleanupStack)
        {
        shared.iMutex.Signal();
        return KErrNoMemory;
        }

    CVoIPDownlinkThread* thread = 0;
    TRAPD(err, thread = CVoIPDownlinkThread::NewL(shared));
    if (err != KErrNone)
        {
        shared.iMutex.Signal();
        return err;
        }

    shared.iMutex.Signal();

    // if we're still here, active scheduler has been constructed
    // start wait loop which runs until it's time to end the thread
    CActiveScheduler::Start();

    // Termination cleanup
    delete thread;
    delete cleanupStack;

    TRACE_PRN_N(_L("VoIP Downlink Thread CLOSED"));
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::Start
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::Start()
    {
    TRACE_PRN_FN_ENT;

    TInt err = KErrNotReady;

    if (iStatus == EReady)
        {
        TRAP(err, iDevSound->PlayInitL());
        TRACE_PRN_IF_ERR(err);

#ifdef _DEBUG
        iSamplesPlayedCount = 0;
#endif
        if (err != KErrNone)
            {
            SendCmd(ECmdDnLinkError, err);
            iStatus = EReady;
            }
        else
            {
            if (iJitterBuffer && iCodecID != KMMFFourCCCodePCM16)
                {
                iJitterBuffer->Play();
                }
            }
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::Stop
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::Stop()
    {
    TRACE_PRN_FN_ENT;

    if (iStatus == EStreaming)
        {
        if (iJitterBuffer && iCodecID != KMMFFourCCCodePCM16)
            {
            iJitterBuffer->Stop();
            }

        iDevSound->Stop();
        iStatus = EReady;
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::InitializeComplete
// A callback from the DevSound indicating completion of the initialization.
// It will send config data to the D/S and configure the encoder via CI.
// If everything goes well, the state of the thread is set EReady.
// The initialization completion message is sent back to the main thread.
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::InitializeComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;

    TInt err = aError;

    if (iDevSound && err == KErrNone)
        {
        TMMFCapabilities conf;
        conf = iDevSound->Config();
        conf.iRate = EMMFSampleRate8000Hz;
        conf.iChannels = EMMFMono;
        TRAP(err, iDevSound->SetConfigL(conf));
        if (err == KErrNone)
            {
            // We are ready to stream even in case of later CI setting failure
            iStatus = EReady;
            TInt vol = iDevSound->MaxVolume();
            iShared.iMutex.Wait();
            iShared.iMaxVolume = vol;
            iShared.iMutex.Signal();
            }

        // Init Custom Interface API to the decoder
        TRAPD(err0, SetCodecCiL());
        if (err0 != KErrNone)
            {
            // DEBUG only
            // Can ignore error - the decoder is not fully configured but
            // can still run in the default mode.
            TRACE_PRN_IF_ERR(err0);
            }
        }

    // Notify the main thread
    SendCmd(ECmdDownlinkInitComplete, err);

    TRACE_PRN_IF_ERR(err);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::BufferToBeFilled
// A reference to the buffer delivered from the DevSound is stored locally
// for later use. It will be filled with the data passed from the client
// when it calls BufferFilled.
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::BufferToBeFilled(CMMFBuffer* aBuffer)
    {
    //TRACE_PRN_FN_ENT;

    // Store pointer to the received buffer
    iDevSoundBufPtr = static_cast<CMMFDataBuffer*> (aBuffer);
    iBufLen = iDevSoundBufPtr->RequestSize();
    TRACE_PRN_N1(_L("VoIP->DNL->BTBF:LEN[%d]"), iBufLen);

#ifndef __WINSCW__
    // The first AMR buffer returns 1 for no data frame.
    if (iCodecID == KMccFourCCIdAMRNB)
        {
        iBufLen = iMaxBufLen;
        }
#else  //__WINSCW__
    // Don't care about full 4k data buffer in WINS
    iBufLen = iMaxBufLen;
#endif //__WINSCW__

    // Create or adjust the chunk
    TInt err = DoChunk(KChunkDNL, iBufLen, iMaxBufLen);

    if (iJitterBuffer && iCodecID != KMMFFourCCCodePCM16)
        {
        // Note: Do not send ECmdFillBuffer to the client except if error
        if (err != KErrNone)
            {
            Stop();
            iMsgBuffer.iStatus = err;
            iMsgBuffer.iRequest = ECmdFillBuffer;
            iMsgQueue.Send(iMsgBuffer);
            }
        else
            {
            // If JB below threshold, it will generate and return SID frames.
            TRAP(err, iGetJBuffer->SetRequestSizeL(iBufLen)); // for CNG

            // Use current buf to play (if data available)
            err = iJitterBuffer->FillBuffer(iGetJBuffer);
            iStatus = EStreaming;
            }
        }
    else // non-JB case
        {
        if (err != KErrNone)
            {
            Stop();
            iMsgBuffer.iStatus = err;
            }
        else
            {
            // Notify client there is buffer ready to be filled
            iMsgBuffer.iStatus = iChunk.Handle();
            iMsgBuffer.iInt = iBufLen;
            iStatus = EStreaming;
            }

        iMsgBuffer.iRequest = ECmdFillBuffer;
        iMsgQueue.Send(iMsgBuffer);
        }

    TRACE_PRN_IF_ERR(err);
    //TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::BufferFilled
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::BufferFilled()
    {
    // TODO: Check D/S status to protect from call to PlayData prior to
    // PlayInitL, the case when D/S is stopped either by the user or due to
    // an error.

    TUint seqNum;
    TBool badFrame = EFalse;

    iShared.iMutex.Wait();
    iBufLen = iShared.iBufferSize; //length of data in the buffer
    seqNum = iShared.iSequenceNum;
    iShared.iMutex.Signal();
    TRACE_PRN_N1(_L("VoIP->DNL->BF: LEN[%d]"), iBufLen);

    if (iBufLen > iMaxBufLen)
        {
        iBufLen = iMaxBufLen;
        badFrame = ETrue;
        TRACE_PRN_N(_L("VoIP->DNL: BAD FRAME"));
        }

    // Copy data over from chunk
    TPtr8 dataPtr(iChunk.Base(), iBufLen, iMaxBufLen);

    if (iJitterBuffer && iCodecID != KMMFFourCCCodePCM16)
        {
        if (badFrame)
            {
            dataPtr.FillZ(); // JB will throw it away
            }

        // Send data to JB. When ready to play JB will call back via EventJB().
        iAddJBuffer->Data() = dataPtr;
        iAddJBuffer->SetFrameNumber(seqNum);
        iJitterBuffer->EmptyBuffer(iAddJBuffer);
        }
    else
        {
        if (iStatus == EStreaming)
            {
            if (badFrame)
                {
                ConcealErrorForNextBuffer();
                }

            // Fill D/S buffer and send it for playback
            iDevSoundBufPtr->Data() = dataPtr;
            iDevSound->PlayData();

            //TRACE_PRN_N1(_L("SAMPLES-PLAYED [%d]"), iSamplesPlayedCount++);
            }
        }
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::PlayError
// From MDevSoundObserver
// Record error is send to client over comm channel.
// The state of recorder is rolled back to EReady.
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::PlayError(TInt aError)
    {
    TRACE_PRN_IF_ERR(aError);

#ifdef _DEBUG
    iSamplesPlayedCount = 0;
#endif
    iStatus = EReady;
    SendCmd(ECmdDnLinkError, aError);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::SetCodecCiL
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::SetCodecCiL()
    {
    TRACE_PRN_FN_ENT;

    if (iCodecID == KMccFourCCIdG711 ||
        iCodecID == KMccFourCCIdG729 ||
        iCodecID == KMccFourCCIdILBC ||
        iCodecID == KMccFourCCIdAMRNB)
        {
        if (!iErrConcealmentIntfc)
            {
            iErrConcealmentIntfc = CErrorConcealmentIntfc::NewL(*iDevSound);
            }
        }

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
        default:
            {
            break;
            }
        }

    if (!iAudioOutput)
        {
        iAudioOutput = CAudioOutput::NewL(*iDevSound);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::SetVolume
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::SetVolume()
    {
    iShared.iMutex.Wait();
    TInt vol = iShared.iInt;
    iShared.iMutex.Signal();
    iDevSound->SetVolume(vol);
    TRACE_PRN_N1(_L("VoIP->DNL: SetVolume [%d]"), vol);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::GetVolume
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::GetVolume()
    {
    TInt vol = iDevSound->Volume();
    iShared.iMutex.Wait();
    iShared.iInt = vol;
    iShared.iMutex.Signal();
    SendCmd(ECmdGetVolumeComplete);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::SetAudioDeviceL
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::SetAudioDeviceL()
    {
    TRACE_PRN_FN_ENT;

    iShared.iMutex.Wait();
    TUint device = iShared.iAudioDevice;
    iShared.iMutex.Signal();

    if (iAudioOutput)
        {
        // ENoPreference=0, EAll=1, ENoOutput=2, EPrivate=3, EPublic=4
        CAudioOutput::TAudioOutputPreference outputDev;

        if (CVoIPAudioDownlinkStream::TVoIPOutputDevice(device) ==
                CVoIPAudioDownlinkStream::EHandset)
            {
            outputDev = CAudioOutput::EPrivate;
            }
        else if (CVoIPAudioDownlinkStream::TVoIPOutputDevice(device) ==
                CVoIPAudioDownlinkStream::ELoudSpeaker)
            {
            outputDev = CAudioOutput::EPublic;
            }
        else // Use default device routing
            {
            outputDev = CAudioOutput::ENoPreference;
            }

        iAudioOutput->SetAudioOutputL(outputDev);
        TRACE_PRN_N1(_L("Output device set=[%d]"), outputDev);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::GetAudioDeviceL
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::GetAudioDeviceL()
    {
    TRACE_PRN_FN_ENT;

    if (iAudioOutput)
        {
        CVoIPAudioDownlinkStream::TVoIPOutputDevice device;
        CAudioOutput::TAudioOutputPreference outputDev =
                iAudioOutput->AudioOutput();
        TRACE_PRN_N1(_L("VoIP->DNL GetAudioDeviceL [%d]"), outputDev);

        switch (outputDev)
            {
            case (CAudioOutput::ENoPreference):
            case (CAudioOutput::EAll):
            case (CAudioOutput::ENoOutput):
            case (CAudioOutput::EPrivate):
            default:
                device = CVoIPAudioDownlinkStream::EHandset;
                break;
            case (CAudioOutput::EPublic):
                device = CVoIPAudioDownlinkStream::ELoudSpeaker;
                break;
            }

        iShared.iMutex.Wait();
        iShared.iAudioDevice = TUint(device);
        iShared.iMutex.Signal();
        SendCmd(ECmdGetAudioDeviceComplete);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::SetIlbcCodecMode
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::SetIlbcCodecMode()
    {
    TInt err = KErrNotSupported;

    if (iStatus == EReady)
        {
        iShared.iMutex.Wait();
        iILBCCodecMode = iShared.iCodecSettings.iILBCCodecMode;
        iShared.iMutex.Signal();

        if (iIlbcDecoderIntfc && iCodecID == KMccFourCCIdILBC)
            {
            if (iILBCCodecMode == CVoIPFormatIntfc::EiLBC20mSecFrame)
                {
                err = iIlbcDecoderIntfc->SetDecoderMode(
                        CIlbcDecoderIntfc::E20msFrame);
                TRACE_PRN_N(_L("VoIP->DNL iLBC Mode Set: [20ms]"));
                }
            else if (iILBCCodecMode == CVoIPFormatIntfc::EiLBC30mSecFrame)
                {
                err = iIlbcDecoderIntfc->SetDecoderMode(
                        CIlbcDecoderIntfc::E30msFrame);
                TRACE_PRN_N(_L("VoIP->DNL iLBC Mode Set: [30ms]"));
                }
            }
        }

    if (err != KErrNone)
        {
        SendCmd(ECmdDnLinkError, err);
        }

    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::GetIlbcCodecMode
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::GetIlbcCodecMode()
    {
    // not available through CIs -> return local value
    iShared.iMutex.Wait();
    iShared.iCodecSettings.iILBCCodecMode = iILBCCodecMode;
    iShared.iMutex.Signal();
    SendCmd(ECmdGetIlbcCodecModeComplete);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::SetG711CodecMode
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::SetG711CodecMode()
    {
    TInt err = KErrNotSupported;

    if (iStatus == EReady)
        {
        iShared.iMutex.Wait();
        iG711CodecMode = iShared.iCodecSettings.iG711CodecMode;
        iShared.iMutex.Signal();

        if (iG711DecoderIntfc && iCodecID == KMccFourCCIdG711)
            {
            if (iG711CodecMode == CVoIPFormatIntfc::EG711ALaw)
                {
                err = iG711DecoderIntfc->SetDecoderMode(
                        CG711DecoderIntfc::EDecALaw);
                TRACE_PRN_N(_L("VoIP->DNL G711 Mode Set: [ALaw]"));
                }
            else if (iG711CodecMode == CVoIPFormatIntfc::EG711uLaw)
                {
                err = iG711DecoderIntfc->SetDecoderMode(
                        CG711DecoderIntfc::EDecULaw);
                TRACE_PRN_N(_L("VoIP->DNL G711 Mode Set: [uLaw]"));
                }
            }
        }

    if (err != KErrNone)
        {
        SendCmd(ECmdDnLinkError, err);
        }

    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::GetG711CodecMode
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::GetG711CodecMode()
    {
    // not available through CIs -> return local value
    iShared.iMutex.Wait();
    iShared.iCodecSettings.iG711CodecMode = iG711CodecMode;
    iShared.iMutex.Signal();
    SendCmd(ECmdGetG711CodecModeComplete);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::FrameModeRqrdForEC
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::FrameModeRqrdForEC()
    {
    TInt err = KErrNotSupported;

    if (iStatus == EReady)
        {
        TBool modeReq = EFalse;

        if (iErrConcealmentIntfc)
            {
            err = iErrConcealmentIntfc->FrameModeRqrdForEC(modeReq);
            }

        iShared.iMutex.Wait();
        iShared.iCodecSettings.iFrameModeReqForEC = modeReq;
        iShared.iMutex.Signal();
        }

    SendCmd(ECmdGetFrameModeReqForECComplete, err);
    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::SetFrameMode
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::SetFrameMode()
    {
    TInt err = KErrNotSupported;

    if (iStatus == EReady)
        {
        iShared.iMutex.Wait();
        iFrameMode = iShared.iCodecSettings.iFrameMode;
        iShared.iMutex.Signal();

        if (iErrConcealmentIntfc)
            {
            err = iErrConcealmentIntfc->SetFrameMode(iFrameMode);
            }
        }

    if (err != KErrNone)
        {
        SendCmd(ECmdDnLinkError, err);
        }

    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::GetFrameMode
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::GetFrameMode()
    {
    TInt err = KErrNotSupported;

    if (iErrConcealmentIntfc)
        {
        // not available through CIs -> return local value
        iShared.iMutex.Wait();
        iShared.iCodecSettings.iFrameMode = iFrameMode;
        iShared.iMutex.Signal();
        err = KErrNone;
        }

    SendCmd(ECmdGetFrameModeComplete, err);
    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::ConcealErrorForNextBuffer
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::ConcealErrorForNextBuffer()
    {
    TInt err = KErrNotSupported;

    if (iErrConcealmentIntfc)
        {
        err = iErrConcealmentIntfc->ConcealErrorForNextBuffer();
        }

    if (err != KErrNone)
        {
        SendCmd(ECmdDnLinkError, err);
        }

    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::SetCng
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::SetCng()
    {
    TInt err = KErrNotSupported;

    if (iStatus == EReady)
        {
        iShared.iMutex.Wait();
        TBool cng = iShared.iCodecSettings.iCng;
        iShared.iMutex.Signal();

        if (iCodecID == KMccFourCCIdG711 && iG711DecoderIntfc)
            {
            err = iG711DecoderIntfc->SetCng(cng);
            }
        else
            {
            if (iCodecID == KMccFourCCIdILBC && iIlbcDecoderIntfc)
                {
                err = iIlbcDecoderIntfc->SetCng(cng);
                }
            }
        }

    if (err != KErrNone)
        {
        SendCmd(ECmdDnLinkError, err);
        }

    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::GetCng
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::GetCng()
    {
    TInt err = KErrNotSupported;

    if (iStatus == EReady)
        {
        TBool cng = EFalse;

        if (iCodecID == KMccFourCCIdG711 && iG711DecoderIntfc)
            {
            err = iG711DecoderIntfc->GetCng(cng);
            }
        else
            {
            if (iCodecID == KMccFourCCIdILBC && iIlbcDecoderIntfc)
                {
                err = iIlbcDecoderIntfc->GetCng(cng);
                }
            }

        iShared.iMutex.Wait();
        iShared.iCodecSettings.iCng = cng;
        iShared.iMutex.Signal();
        }

    SendCmd(ECmdGetCngComplete, err);
    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::SetPlc
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::SetPlc()
    {
    TInt err = KErrNotSupported;

    if (iStatus == EReady)
        {
        if (iCodecID == KMccFourCCIdG711 && iG711DecoderIntfc)
            {
            iShared.iMutex.Wait();
            iPlc = iShared.iCodecSettings.iPlc;
            iShared.iMutex.Signal();
            err = iG711DecoderIntfc->SetPlc(iPlc);
            }
        }

    if (err != KErrNone)
        {
        SendCmd(ECmdDnLinkError, err);
        }

    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::GetPlc
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::GetPlc()
    {
    TInt err = KErrNotSupported;

    if (iCodecID == KMccFourCCIdG711 && iG711DecoderIntfc)
        {
        // not available through CIs -> return local value
        iShared.iMutex.Wait();
        iShared.iCodecSettings.iPlc = iPlc;
        iShared.iMutex.Signal();
        err = KErrNone;
        }

    SendCmd(ECmdGetPlcComplete, err);
    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::BadLsfNextBuffer
//
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::BadLsfNextBuffer()
    {
    TInt err = KErrNotSupported;

    if (iStatus == EStreaming)
        {
        if (iCodecID == KMccFourCCIdG729 && iG729DecoderIntfc)
            {
            err = iG729DecoderIntfc->BadLsfNextBuffer();
            }
        }

    if (err != KErrNone)
        {
        SendCmd(ECmdDnLinkError, err);
        }

    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::ConfigureJitterBufferL
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::ConfigureJitterBufferL()
    {
    TInt err = KErrNotSupported;

    if (iCodecID != KMMFFourCCCodePCM16)
        {
        if (iJitterBuffer)
            {
            delete iJitterBuffer;
            iJitterBuffer = NULL;
            }
        iJitterBuffer = CVoIPJitterBuffer::NewL(this);

        if (iAddJBuffer)
            {
            delete iAddJBuffer;
            iAddJBuffer = NULL;
            }
        iAddJBuffer = CMMFDataBuffer::NewL(iMaxBufLen);

        if (iGetJBuffer)
            {
            delete iGetJBuffer;
            iGetJBuffer = NULL;
            }
        iGetJBuffer = CMMFDataBuffer::NewL(iMaxBufLen);

        if (iJitterBuffer && iAddJBuffer && iGetJBuffer)
            {
            TRACE_PRN_N(_L("VoIP::ConfigureJitterBufferL [OK]"));

            TVoIPJBConfig conf;
            iShared.iMutex.Wait();
            conf.iSampleInterval = iShared.iJBConfig.iSampleInterval;
            conf.iJitterLatency = iShared.iJBConfig.iJitterLatency;
            conf.iJBBufferLength = iShared.iJBConfig.iJBBufferLength;
            conf.iJBThreshold = iShared.iJBConfig.iJBThreshold;
            conf.iJBInactivityTimeOut
                    = iShared.iJBConfig.iJBInactivityTimeOut;
            // TODO: add tone playback params
            //conf.iJBPlayToneTimeout = ?;
            //conf.iJBPlayToneDuration = ?;
            //conf.iJBPlayToneFrequency = ?;
            iShared.iMutex.Signal();

            iJitterBuffer->SetupL(iCodecID, conf);
            iBufLen = iMaxBufLen;
            err = DoChunk(KChunkDNL, iBufLen, iMaxBufLen);
            }
        else
            {
            err = KErrGeneral;
            }
        }

    if (err != KErrNone)
        {
        Stop();
        iMsgBuffer.iStatus = err;
        }
    else
        {
        // Notify client there is buffer ready to be filled
        iMsgBuffer.iStatus = iChunk.Handle();
        iMsgBuffer.iInt = iBufLen;
        }

    // This is the only time we will send ECmdFillBuffer in JB mode to
    // indicate to the client we are ready to receive packets of iBufLen size.
    iMsgBuffer.iRequest = ECmdFillBuffer;
    iMsgQueue.Send(iMsgBuffer);

    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::ResetJitterBuffer
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::ResetJitterBuffer()
    {
    if (iJitterBuffer && iCodecID != KMMFFourCCCodePCM16)
        {
        iShared.iMutex.Wait();
        TBool playTone = iShared.iBool;
        iShared.iMutex.Signal();
        iJitterBuffer->ResetBuffer(playTone);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::JBDelayDown
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::JBDelayDown()
    {
    if (iJitterBuffer && iCodecID != KMMFFourCCCodePCM16)
        {
        iJitterBuffer->DelayDown();
        }
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::JBDelayUp
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::JBDelayUp()
    {
    if (iJitterBuffer && iCodecID != KMMFFourCCCodePCM16)
        {
        iJitterBuffer->DelayUp();
        }
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::SendCmd
// Completes active object's request and sets shared data command value to
// one of the user requested commands.
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::SendCmd(TUserCommand aCmd, TInt aError)
    {
    iShared.iMutex.Wait();

    iShared.iCmd = aCmd;
    TRequestStatus* status = iShared.iMnThreadStatus;

    if (status)
        {
        if (status->Int() == KRequestPending)
            {
            RThread t;
            TInt err = t.Open(iShared.iMainThreadID);
            if (err == KErrNone)
                {
                t.RequestComplete(status, aError);
                }
            }
        }

    iShared.iMutex.Signal();
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::Event
// From MQueueHandlerObserverSrv
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::Event(TInt aEventType, TInt /*aError*/)
    {
    switch (aEventType)
        {
        case ECmdStartDownlink:
            {
            Start();
            break;
            }
        case ECmdStopDownlink:
            {
            if (iStatus == EStreaming)
                {
                Stop();
                }
            break;
            }
        case ECmdBufferFilled:
            {
            BufferFilled();
            break;
            }
        case ECmdGetVolume:
            {
            GetVolume();
            break;
            }
        case ECmdSetVolume:
            {
            SetVolume();
            break;
            }
        case ECmdSetAudioDevice:
            {
            TRAPD(err, SetAudioDeviceL());
            if (err != KErrNone)
                {
                SendCmd(ECmdDnLinkError, err);
                }
            break;
            }
        case ECmdGetAudioDevice:
            {
            TRAPD(err, GetAudioDeviceL());
            if (err != KErrNone)
                {
                SendCmd(ECmdDnLinkError, err);
                }
            break;
            }
        case ECmdSetIlbcCodecMode:
            {
            SetIlbcCodecMode();
            break;
            }
        case ECmdGetIlbcCodecMode:
            {
            GetIlbcCodecMode();
            break;
            }
        case ECmdSetG711CodecMode:
            {
            SetG711CodecMode();
            break;
            }
        case ECmdGetG711CodecMode:
            {
            GetG711CodecMode();
            break;
            }
        case ECmdSetFrameMode:
            {
            SetFrameMode();
            break;
            }
        case ECmdGetFrameMode:
            {
            GetFrameMode();
            break;
            }
        case ECmdFrameModeRqrdForEC:
            {
            FrameModeRqrdForEC();
            break;
            }
        case ECmdConcealErrForNextBuf:
            {
            ConcealErrorForNextBuffer();
            break;
            }
        case ECmdSetCng:
            {
            SetCng();
            break;
            }
        case ECmdGetCng:
            {
            GetCng();
            break;
            }
        case ECmdSetPlc:
            {
            SetPlc();
            break;
            }
        case ECmdGetPlc:
            {
            GetPlc();
            break;
            }
        case ECmdBadLsfNextBuffer:
            {
            BadLsfNextBuffer();
            break;
            }
        case ECmdVoIPConfigJB:
            {
            TRAPD(err, ConfigureJitterBufferL());
            if (err != KErrNone)
                {
                SendCmd(ECmdDnLinkError, err);
                }
            break;
            }
        case ECmdVoIPResetJB:
            {
            ResetJitterBuffer();
            break;
            }
        case ECmdVoIPJBDelayDown:
            {
            JBDelayDown();
            break;
            }
        case ECmdVoIPJBDelayUp:
            {
            JBDelayUp();
            break;
            }
        case ECmdTerminateThread:
        default:
            {
            if (iStatus == EStreaming)
                {
                Stop();
                }
            // if unknown exception is raised exit thread
            CActiveScheduler::Stop();
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CVoIPDownlinkThread::Event
// From MJitterBufferObserver
// -----------------------------------------------------------------------------
//
void CVoIPDownlinkThread::EventJB(TInt aEventType, TInt aError)
    {
    switch (aEventType)
        {
        case MJitterBufferObserver::EBufferReadyToPlay:
            {
            if (iStatus == EStreaming)
                {
                iDevSoundBufPtr->Data() = iGetJBuffer->Data();
                iDevSound->PlayData();
                TRACE_PRN_N1(_L("SAMPLES-PLAYED [%d]"), iSamplesPlayedCount++);
                }
            break;
            }
        case MJitterBufferObserver::EBufferConsumed:
            {
            // Notify client (?)
            break;
            }
        case MJitterBufferObserver::EConcealErrorForNextBuffer:
            {
            ConcealErrorForNextBuffer();
            break;
            }
        case MJitterBufferObserver::EGeneralError:
//      case MJitterBufferObserver::EBufferUnderflow:
//      case MJitterBufferObserver::EBufferOverflow:
        default:
            {
            SendCmd(ECmdDnLinkJBError, aError);
            break;
            }
        }
    }

// End of file
