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
 *               Implements uplink thread object.
 *
 */

#include <IlbcEncoderIntfc.h>
#include <G711EncoderIntfc.h>
#include <G729EncoderIntfc.h>
#include <SpeechEncoderConfig.h>
#include "debugtracemacros.h"
#include "VoIPSharedData.h"
#include "VoIPServerThread.h"

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::CVoIPUplinkThread
// Standard Constructor
// -----------------------------------------------------------------------------
//
CVoIPUplinkThread::CVoIPUplinkThread(TSharedData& aData) :
    iShared(aData)
    {
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::~CVoIPUplinkThread
// Standard Constructor
// -----------------------------------------------------------------------------
//
CVoIPUplinkThread::~CVoIPUplinkThread()
    {
    TRACE_PRN_FN_ENT;

    Stop();
    iBitrates.Close();
    iShared.iCodecSettings.iArrBitrates = NULL;
    delete iSpeechEncoderConfig;
    delete iG711EncoderIntfc;
    delete iG729EncoderIntfc;
    delete iIlbcEncoderIntfc;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::NewL
// Symbian two-phase constructor
// -----------------------------------------------------------------------------
//
CVoIPUplinkThread* CVoIPUplinkThread::NewL(TSharedData& aData)
    {
    CVoIPUplinkThread* self = new (ELeave) CVoIPUplinkThread(aData);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::ConstructL
// Part two of Symbian two phase construction
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::ConstructL()
    {
    TRACE_PRN_FN_ENT;

    InitThreadL();
    iCodecID = iShared.iCodecSettings.iFourCC;
    InitMsgQueuesL(KUpLinkQueue, KUpLinkThreadComQueue);

    iShared.iMutex.Wait();
    TRAPD(err, InitDevSoundL(EMMFStateRecording, iShared.iPriority,
            iShared.iPreference));
    iShared.iMutex.Signal();

    if (err != KErrNone)
        {
        SendCmd(ECmdDnLinkError, err);
        }
    iMaxBufLen = DetermineMaxBufferLen();

    // Client must set these before querying!
    iG711CodecMode = CVoIPFormatIntfc::EG711ALaw;
    iILBCCodecMode = CVoIPFormatIntfc::EiLBC20mSecFrame;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::ThreadFunction
// Thread Startup function
// -----------------------------------------------------------------------------
//
TInt CVoIPUplinkThread::ThreadFunction(TAny* aData)
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

    CVoIPUplinkThread* thread = 0;
    TRAPD(err, thread = CVoIPUplinkThread::NewL(shared));
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

    TRACE_PRN_N(_L("CVoIPUplinkThread::ThreadFunction : Thread CLOSED"));
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::Start
//
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::Start()
    {
    TRACE_PRN_FN_ENT;

    TInt err = KErrNotReady;

    if (iStatus == EReady)
        {
        TRAP(err, iDevSound->RecordInitL());
        TRACE_PRN_IF_ERR(err);

        if (err != KErrNone)
            {
            SendCmd(ECmdUpLinkError, err);
            iStatus = EReady;
            }
#ifdef _DEBUG
        else
            {
            iSamplesRecCount = 0;
            }
#endif
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::Stop
//
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::Stop()
    {
    TRACE_PRN_FN_ENT;

    if (iStatus == EStreaming)
        {
        iDevSound->Stop();
        iStatus = EReady;
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::InitializeComplete
// A callback from the DevSound indicating completion of the initialization.
// It will send config data to the D/S and configure the encoder via CI.
// If everything goes well, the state of the thread is set EReady.
// The initialization completion message is sent back to the main thread.
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::InitializeComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;

    TInt err = aError;

    if (err == KErrNone)
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
            TInt gain = iDevSound->MaxGain();
            iShared.iMutex.Wait();
            iShared.iMaxGain = gain;
            iShared.iMutex.Signal();
            }

        // Init Custom Interface API to the Encoder
        TRAPD(err0, SetCodecCiL());
        if (err0 != KErrNone)
            {
            // DEBUG only
            // Can ignore error - the encoder is not fully configured but
            // can still run in the default mode.
            TRACE_PRN_IF_ERR(err0);
            }
        }

    // Notify the main thread
    SendCmd(ECmdUplinkInitComplete, err);

    TRACE_PRN_IF_ERR(err);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::BufferToBeEmptied
// From MDevSoundObserver
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::BufferToBeEmptied(CMMFBuffer* aBuffer)
    {
    //    TRACE_PRN_N1(_L("SAMPLES-RECORDED [%d]"), iSamplesRecCount++);

    iDevSoundBufPtr = static_cast<CMMFDataBuffer*> (aBuffer);
    TDes8& data = iDevSoundBufPtr->Data();

#ifndef __WINSCW__
    iBufLen = iDevSoundBufPtr->BufferSize();
#else //__WINSCW__
    // Buffer size is always 4kB in WINS (PCM). Since we are testing stub
    // adaptaions of all other supported codecs, and WINS playback is not
    // supported anyway, we can throw away generated frames and use only
    // up to the max length of other codecs just to keep the loopback alive.
    iBufLen = iMaxBufLen;
    data.SetLength(iBufLen);
#endif //__WINSCW__

    TRACE_PRN_N1(_L("VoIP->UPL: BTBE->LEN [%d]"), iBufLen);

    // Adjust/create RChunk if necessary
    TInt err = DoChunk(KChunkUPL, iBufLen, iMaxBufLen);

    if (err != KErrNone)
        {
        Stop();
        iMsgBuffer.iStatus = err;
        }
    else
        {
        // Pass buffer parameters to the client
        iMsgBuffer.iStatus = iChunk.Handle();
        iMsgBuffer.iInt = iBufLen;

        // Copy data over to RChunk
        TPtr8 dataPtr(iChunk.Base(), iBufLen, iMaxBufLen);
        dataPtr = data;
        iStatus = EStreaming;
        }

    // Notify client there is buffer ready to be emptied
    iMsgBuffer.iRequest = ECmdEmptyBuffer;
    iMsgQueue.Send(iMsgBuffer);
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::BufferEmptied
//
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::BufferEmptied()
    {
    //    TRACE_PRN_N(_L("VoIP->UPL: BE"));

    iDevSound->RecordData();
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::RecordError
// From MDevSoundObserver
// Recording error is send to the main thread.
// The state of recorder is rolled back to EReady.
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::RecordError(TInt aError)
    {
    TRACE_PRN_IF_ERR(aError);

#ifdef _DEBUG
    iSamplesRecCount = 0;
#endif
    iStatus = EReady;
    SendCmd(ECmdUpLinkError, aError);
    }

// ---------------------------------------------------------------------------
// CVoIPUplinkThread::SetCodecCiL
//
// ---------------------------------------------------------------------------
//
void CVoIPUplinkThread::SetCodecCiL()
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
        default:
            {
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
// CVoIPUplinkThread::SetGain
//
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::SetGain()
    {
    iShared.iMutex.Wait();
    TInt gain = iShared.iInt;
    iShared.iMutex.Signal();
    iDevSound->SetGain(gain);
    TRACE_PRN_N1(_L("VoIP->UPL: SetGain [%d]"), gain);
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::GetGain
//
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::GetGain()
    {
    TInt gain = iDevSound->Gain();
    iShared.iMutex.Wait();
    iShared.iInt = gain;
    iShared.iMutex.Signal();
    SendCmd(ECmdGetGainComplete);
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::SetIlbcCodecMode
//
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::SetIlbcCodecMode()
    {
    TInt err = KErrNotSupported;

    if (iStatus == EReady)
        {
        iShared.iMutex.Wait();
        iILBCCodecMode = iShared.iCodecSettings.iILBCCodecMode;
        iShared.iMutex.Signal();

        if (iIlbcEncoderIntfc && iCodecID == KMccFourCCIdILBC)
            {
            if (iILBCCodecMode == CVoIPFormatIntfc::EiLBC20mSecFrame)
                {
                err = iIlbcEncoderIntfc->SetEncoderMode(
                        CIlbcEncoderIntfc::E20msFrame);
                TRACE_PRN_0(
                        _L("VoIP->CVoIPUplinkThread[0x%x]::SetIlbcCodecMode [20ms Frame]"));
                }
            else
                {
                if (iILBCCodecMode == CVoIPFormatIntfc::EiLBC30mSecFrame)
                    {
                    err = iIlbcEncoderIntfc->SetEncoderMode(
                            CIlbcEncoderIntfc::E30msFrame);
                    TRACE_PRN_0(
                            _L("VoIP->CVoIPUplinkThread[0x%x]::SetIlbcCodecMode [30ms Frame]"));
                    }
                }
            }
        }

    if (err != KErrNone)
        {
        SendCmd(ECmdUpLinkError, err);
        }

    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::GetIlbcCodecMode
//
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::GetIlbcCodecMode()
    {
    // not available through CIs -> return local value
    iShared.iMutex.Wait();
    iShared.iCodecSettings.iILBCCodecMode = iILBCCodecMode;
    iShared.iMutex.Signal();
    SendCmd(ECmdGetIlbcCodecModeComplete);
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::SetG711CodecMode
//
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::SetG711CodecMode()
    {
    TInt err = KErrNotSupported;

    if (iStatus == EReady)
        {
        iShared.iMutex.Wait();
        iG711CodecMode = iShared.iCodecSettings.iG711CodecMode;
        iShared.iMutex.Signal();

        if (iG711EncoderIntfc && iCodecID == KMccFourCCIdG711)
            {
            if (iG711CodecMode == CVoIPFormatIntfc::EG711ALaw)
                {
                err = iG711EncoderIntfc->SetEncoderMode(
                        CG711EncoderIntfc::EEncALaw);
                TRACE_PRN_0(
                        _L("VoIP->CVoIPUplinkThread[0x%x]::SetG711CodecMode [ALaw]"));
                }
            else
                {
                if (iG711CodecMode == CVoIPFormatIntfc::EG711uLaw)
                    {
                    err = iG711EncoderIntfc->SetEncoderMode(
                            CG711EncoderIntfc::EEncULaw);
                    TRACE_PRN_0(
                            _L("VoIP->CVoIPUplinkThread[0x%x]::SetG711CodecMode [uLaw]"));
                    }
                }
            }
        }

    if (err != KErrNone)
        {
        SendCmd(ECmdUpLinkError, err);
        }

    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::GetG711CodecMode
//
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::GetG711CodecMode()
    {
    // not available through CIs -> return local value
    iShared.iMutex.Wait();
    iShared.iCodecSettings.iG711CodecMode = iG711CodecMode;
    iShared.iMutex.Signal();
    SendCmd(ECmdGetG711CodecModeComplete);
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::GetSupportedBitrates
//
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::GetSupportedBitrates()
    {
    TInt err = KErrNotSupported;

    if (iSpeechEncoderConfig)
        {
        iBitrates.Reset();
        err = iSpeechEncoderConfig->GetSupportedBitrates(iBitrates);
        iShared.iMutex.Wait();
        iShared.iCodecSettings.iArrBitrates = &iBitrates;
        iShared.iMutex.Signal();
        }

    SendCmd(ECmdGetSupportedBitratesComplete, err);
    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::SetBitrate
//
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::SetBitrate()
    {
    TInt err = KErrNotSupported;
    iShared.iMutex.Wait();
    TUint bitrate = iShared.iCodecSettings.iBitrate;
    iShared.iMutex.Signal();

    if (iSpeechEncoderConfig)
        {
        err = iSpeechEncoderConfig->SetBitrate(bitrate);
        TRACE_PRN_N1(_L("VoIP->UPL: SetBitrate [%d]"), bitrate);
        }

    if (err != KErrNone)
        {
        SendCmd(ECmdUpLinkError, err);
        }

    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::GetBitrate
//
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::GetBitrate()
    {
    TInt err = KErrNotSupported;
    TUint bitrate = 0;

    if (iSpeechEncoderConfig)
        {
        err = iSpeechEncoderConfig->GetBitrate(bitrate);
        }

    iShared.iMutex.Wait();
    iShared.iCodecSettings.iBitrate = bitrate;
    iShared.iMutex.Signal();
    SendCmd(ECmdGetBitrateComplete, err);
    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::SetVad
//
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::SetVad()
    {
    TInt err = KErrNotSupported;
    iShared.iMutex.Wait();
    TBool vad = iShared.iCodecSettings.iVad;
    iShared.iMutex.Signal();

    switch (iCodecID)
        {
        case KMccFourCCIdG711:
            {
            if (iG711EncoderIntfc)
                {
                err = iG711EncoderIntfc->SetVadMode(vad);
                }
            break;
            }
        case KMccFourCCIdG729:
            {
            if (iG729EncoderIntfc)
                {
                err = iG729EncoderIntfc->SetVadMode(vad);
                }
            break;
            }
        case KMccFourCCIdILBC:
            {
            if (iIlbcEncoderIntfc)
                {
                err = iIlbcEncoderIntfc->SetVadMode(vad);
                }
            break;
            }
        case KMccFourCCIdAMRNB:
            {
            if (iSpeechEncoderConfig)
                {
                err = iSpeechEncoderConfig->SetVadMode(vad);
                }
            break;
            }
        default:
            {
            break; //KErrNotSupported
            }
        }

    if (err != KErrNone)
        {
        SendCmd(ECmdUpLinkError, err);
        }

    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::GetVad
//
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::GetVad()
    {
    TInt err = KErrNotSupported;
    TBool vad = EFalse;

    switch (iCodecID)
        {
        case KMccFourCCIdG711:
            {
            if (iG711EncoderIntfc)
                {
                err = iG711EncoderIntfc->GetVadMode(vad);
                }
            break;
            }
        case KMccFourCCIdG729:
            {
            if (iG729EncoderIntfc)
                {
                err = iG729EncoderIntfc->GetVadMode(vad);
                }
            break;
            }
        case KMccFourCCIdILBC:
            {
            if (iIlbcEncoderIntfc)
                {
                err = iIlbcEncoderIntfc->GetVadMode(vad);
                }
            break;
            }
        case KMccFourCCIdAMRNB:
            {
            if (iSpeechEncoderConfig)
                {
                err = iSpeechEncoderConfig->GetVadMode(vad);
                }
            break;
            }
        default:
            {
            break; //KErrNotSupported
            }
        }

    iShared.iMutex.Wait();
    iShared.iCodecSettings.iVad = vad;
    iShared.iMutex.Signal();
    SendCmd(ECmdGetVadComplete, err);
    TRACE_PRN_IF_ERR(err);
    }

// -----------------------------------------------------------------------------
// CVoIPUplinkThread::SendCmd
// Completes active object's request and sets shared data command value to
// one of the user requested commands.
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::SendCmd(TUserCommand aCmd, TInt aError)
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
// CVoIPUplinkThread
// From MQueueHandlerObserverSrv
// -----------------------------------------------------------------------------
//
void CVoIPUplinkThread::Event(TInt aEventType, TInt /*aError*/)
    {
    switch (aEventType)
        {
        case ECmdStartUplink:
            {
            Start();
            break;
            }
        case ECmdStopUplink:
            {
            if (iStatus == EStreaming)
                {
                Stop();
                }
            break;
            }
        case ECmdBufferEmptied:
            {
            BufferEmptied();
            break;
            }
        case ECmdGetGain:
            {
            GetGain();
            break;
            }
        case ECmdSetGain:
            {
            SetGain();
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
        case ECmdGetSupportedBitrates:
            {
            GetSupportedBitrates();
            break;
            }
        case ECmdSetBitrate:
            {
            SetBitrate();
            break;
            }
        case ECmdGetBitrate:
            {
            GetBitrate();
            break;
            }
        case ECmdSetVad:
            {
            SetVad();
            break;
            }
        case ECmdGetVad:
            {
            GetVad();
            break;
            }
        case ECmdTerminateThread:
        default:
            {
            if (iStatus == EStreaming)
                {
                Stop();
                }
            // if unknown exception is raised, just exit this thread
            CActiveScheduler::Stop();
            break;
            }
        }
    }

// End of file
