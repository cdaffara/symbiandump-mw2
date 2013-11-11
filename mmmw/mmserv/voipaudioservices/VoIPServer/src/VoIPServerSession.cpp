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
 * Description: VoIP audio server session class implementation.
 *              Each session is created either for an uplink, downlink or
 *              ring tone player. Separate thread is created for uplink and
 *              downlink stream. Ring tone player runs in the main thread.
 *
 */

#include <AudioPreference.h>
#include <MProEngEngine.h>
#include <MProEngProfile.h>
#include <MProEngTones.h>
#include <ProEngFactory.h>
#include <voipaudiocommon.h>
#include "debugtracemacros.h"
#include "VoIPAudioSession.h"
#include "VoIPServerThread.h"
#include "VoIPSharedData.h"
#include "VoIPServerSession.h"

// These are the default tone length settings for the US
const TUint KDTMFToneLengthOn = 250000;
const TUint KDTMFToneLengthOff = 50000;
const TUint KDTMFToneLengthPause = 2500000;
const TUint KArrayExpandSize = 8;

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::NewL
// Symbian constructor.
// -----------------------------------------------------------------------------
//
CVoIPAudioServerSession* CVoIPAudioServerSession::NewL(
        CVoIPAudioServer &aServer)
    {
    CVoIPAudioServerSession* self = new (ELeave) CVoIPAudioServerSession(
            aServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::~CVoIPAudioServerSession
// The destructor
// -----------------------------------------------------------------------------
//
CVoIPAudioServerSession::~CVoIPAudioServerSession()
    {
    TRACE_PRN_FN_ENT;

    if (iAO)
        {
        iAO->Cancel();
        delete iAO;
        }

    delete iDevSound;
    delete iToneBuf;
    delete iRingTonePlayer;
    delete iDTMFTonePlayer;
    iCodecsUp.Reset();
    iCodecsUp.Close();
    iCodecsDn.Reset();
    iCodecsDn.Close();

    if (iThread.Handle() > 0)
        {
        iThread.Close(); // close thread handle
        }

    if (iMsgComQueueDtmf.Handle() > 0)
        {
        iMsgComQueueDtmf.Close();
        }

    if (iMsgComQueue.Handle() > 0)
        {
        iMsgComQueue.Close();
        }

    if (iITCMsgComQueue.Handle() > 0)
        {
        iITCMsgComQueue.Close();
        }

    iShared.iMutex.Close();
    iVoIPServer.DropSession(); // will start shutdown if no more sessions left

    // Release memory and close handles to unused plug-ins held by the DevSound
    REComSession::FinalClose();

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::ConstructL
// 2-nd phase constructor.
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::ConstructL()
    {
    TRACE_PRN_FN_ENT;

    // Create semaphore for thread signaling when accessing shared data
    iShared.iMutex.CreateLocal();

    // Setup A/O for ITC to receive callbacks from the uplink/downlink threads
    // Note: Must replace with msg queue if multiple callbacks can be
    //       received at the same time.
    TThreadId tID = RThread().Id();
    iShared.iMutex.Wait();
    iShared.iMainThreadID = tID;
    iAO = new (ELeave) CVoIPServerAO(this, tID);
    iShared.iMnThreadStatus = &(this->iAO->iStatus);
    iShared.iCodecSettings.iG711FrameRate = TVoIPCodecSettings::E20MS;
    iShared.iCodecSettings.iArrBitrates = NULL;
    iShared.iMutex.Signal();
    iAO->Request();
    iSessionType = ENone;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::CVoIPAudioServerSession
// -----------------------------------------------------------------------------
//
CVoIPAudioServerSession::CVoIPAudioServerSession(CVoIPAudioServer &aServer) :
    iVoIPServer(aServer)
    {
    TRACE_PRN_FN_ENT;
    iVoIPServer.AddSession(); //add new session
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::ServiceL
// Service request handler.
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::ServiceL(const RMessage2& aMessage)
    {
    TRAPD(err, DispatchMessageL(aMessage));
    if (!iMsgQueued)
        {
        aMessage.Complete(err);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::DispatchMessageL
// Service request handler.
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::DispatchMessageL(const RMessage2& aMessage)
    {
    switch (aMessage.Function())
        {
        case EVoIPSetEncoderPriority:
        case EVoIPSetDecoderPriority:
            SetPriorityL(aMessage);
            break;
        case EVoIPGetSupportedEncodersCount:
            if (iSessionType == ENone)
                {
                iSessionType = EQueryEnc;
                }
            GetCodecsCountL(aMessage);
            break;
        case EVoIPGetSupportedDecodersCount:
            if (iSessionType == ENone)
                {
                iSessionType = EQueryDec;
                }
            GetCodecsCountL(aMessage);
            break;
        case EVoIPGetSupportedEncoders:
            if (iSessionType == ENone)
                {
                iSessionType = EQueryEnc;
                }
            GetSupportedCodecsL(aMessage);
            break;
        case EVoIPGetSupportedDecoders:
            GetSupportedCodecsL(aMessage);
            break;
        case EVoIPSetDecoder:
            if (iSessionType == ENone)
                {
                iSessionType = EQueryDec;
                }
            SetFormatL(aMessage);
            break;
        case EVoIPSetEncoder:
            if (iSessionType == ENone)
                {
                iSessionType = EQueryEnc;
                }
            SetFormatL(aMessage);
            break;
        case EVoIPOpenDownlink:
            OpenDownlinkL(aMessage);
            break;
        case EVoIPOpenUplink:
            OpenUplinkL(aMessage);
            break;
        case EVoIPStartDownlink:
            StartDownlink();
            break;
        case EVoIPStartUplink:
            StartUplink();
            break;
        case EVoIPStopDownlink:
            StopDownlink();
            break;
        case EVoIPStopUplink:
            StopUplink();
            break;
        case EVoIPCloseDownlink:
            CloseDownlink();
            break;
        case EVoIPCloseUplink:
            CloseUplink();
            break;
        case EVoIPGetMaxVolume:
            GetMaxVolumeL(aMessage);
            break;
        case EVoIPGetVolume:
            GetVolumeL(aMessage);
            break;
        case EVoIPSetVolume:
            SetVolume(aMessage);
            break;
        case EVoIPGetMaxGain:
            GetMaxGainL(aMessage);
            break;
        case EVoIPGetGain:
            GetGainL(aMessage);
            break;
        case EVoIPSetGain:
            SetGain(aMessage);
            break;
        case EVoIPSetAudioDevice:
            SetAudioDeviceL(aMessage);
            break;
        case EVoIPGetAudioDevice:
            GetAudioDeviceL(aMessage);
            break;
        case EVoIPBufferFilled:
            BufferFilledL(aMessage);
            break;
        case EVoIPBufferEmptied:
            BufferEmptiedL(aMessage);
            break;
        case EVoIPSetIlbcCodecMode:
            SetIlbcCodecModeL(aMessage);
            break;
        case EVoIPGetIlbcCodecMode:
            GetIlbcCodecModeL(aMessage);
            break;
        case EVoIPSetG711CodecMode:
            SetG711CodecModeL(aMessage);
            break;
        case EVoIPGetG711CodecMode:
            GetG711CodecModeL(aMessage);
            break;
        case EVoIPGetSupportedBitratesCount:
            GetSupportedBitratesCountL(aMessage);
            break;
        case EVoIPGetSupportedBitrates:
            GetSupportedBitratesL(aMessage);
            break;
        case EVoIPSetBitrate:
            SetBitrate(aMessage);
            break;
        case EVoIPGetBitrate:
            GetBitrateL(aMessage);
            break;
        case EVoIPFrameModeRqrdForEC:
            FrameModeRqrdForEcL(aMessage);
            break;
        case EVoIPSetFrameMode:
            SetFrameMode(aMessage);
            break;
        case EVoIPGetFrameMode:
            GetFrameModeL(aMessage);
            break;
        case EVoIPConcealErrForNextBuf:
            ConcealErrorForNextBuffer();
            break;
        case EVoIPSetVad:
            SetVad(aMessage);
            break;
        case EVoIPGetVad:
            GetVadL(aMessage);
            break;
        case EVoIPSetCng:
            SetCng(aMessage);
            break;
        case EVoIPGetCng:
            GetCngL(aMessage);
            break;
        case EVoIPSetPlc:
            SetPlc(aMessage);
            break;
        case EVoIPGetPlc:
            GetPlcL(aMessage);
            break;
        case EVoIPBadLsfNextBuffer:
            BadLsfNextBuffer();
            break;
        case EVoIPOpenDTMFTonePlayer:
            OpenDTMFTonePlayerL();
            break;
        case EVoIPPlayDTMFTone:
            PlayDTMFToneL(aMessage);
            break;
        case EVoIPStopDTMFTone:
            StopDTMFTonePlay();
            break;
        case EVoIPCloseDTMFTonePlayer:
            CloseDTMFTonePlayer();
            break;
        case EVoIPOpenRingTonePlayerFromFile:
            OpenRingTonePlayerFromFileL(aMessage);
            break;
        case EVoIPOpenRingTonePlayerFromRFile:
            OpenRingTonePlayerFromRFileL(aMessage);
            break;
        case EVoIPOpenRingTonePlayerFromProfile:
            OpenRingTonePlayerFromProfileL();
            break;
        case EVoIPOpenRingTonePlayerFromDes:
            OpenRingTonePlayerFromDesL(aMessage);
            break;
        case EVoIPPlayRingTone:
            PlayRingTone();
            break;
        case EVoIPPauseRingTone:
            PauseRingTone();
            break;
        case EVoIPResumeRingTone:
            ResumeRingTone();
            break;
        case EVoIPStopRingTone:
            StopRingTone();
            break;
        case EVoIPCloseRingTonePlayer:
            CloseRingTonePlayer();
            break;
        case EVoIPConfigJitterBuffer:
            ConfigureJitterBufferL(aMessage);
            break;
        case EVoIPResetJitterBuffer:
            ResetJitterBuffer(aMessage);
            break;
        case EVoIPJBDelayDown:
            JBDelayDown();
            break;
        case EVoIPJBDelayUp:
            JBDelayUp();
            break;
        case EVoIPSessionClose:
            break;
        default:
            User::Leave(KErrNotSupported);
            break;
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::SetDecoderPriority
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::SetPriorityL(const RMessage2& aMessage)
    {
    TMMFPrioritySettingsPckg pckg;
    aMessage.ReadL(0, pckg);
    iPriority = pckg();
    iShared.iMutex.Wait();
    iShared.iPreference = iPriority.iPref;
    iShared.iPriority = iPriority.iPriority;
    iShared.iMutex.Signal();
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetCodecsCountL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetCodecsCountL(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    TVoIPMsgBufPckg p;
    p().iStatus = KErrNone;
    p().iInt = 0;

    // Is codecs info already available?
    if (iCodecsCountUp > 0 && (iSessionType == EQueryEnc ||
            iSessionType == EUplink))
        {
        p().iInt = iCodecsCountUp;
        iShared.iMutex.Wait();
        p().iUint = iShared.iCodecSettings.iG711FrameRate; //TODO:
        iShared.iMutex.Signal();
        }
    else if (iCodecsCountDn > 0 && (iSessionType == EQueryDec ||
            iSessionType == EDnlink))
        {
        p().iInt = iCodecsCountDn;
        iShared.iMutex.Wait();
        p().iUint = iShared.iCodecSettings.iG711FrameRate; //TODO:
        iShared.iMutex.Signal();
        }
    else
        {
        delete iDevSound;
        iDevSound = NULL;
        iDevSound = CMMFDevSound::NewL();

        if (iDevSound)
            {
            iCodecsUp.Reset();
            iCodecsCountUp = 0;
            iDevSound->GetSupportedInputDataTypesL(iCodecsUp, iPriority);
            iCodecsCountUp = iCodecsUp.Count();

            iCodecsDn.Reset();
            iCodecsCountDn = 0;
            iDevSound->GetSupportedOutputDataTypesL(iCodecsDn, iPriority);
            iCodecsCountDn = iCodecsDn.Count();
            }
        }

    delete iDevSound;
    iDevSound = NULL;

    if (iSessionType == EQueryEnc || iSessionType == EUplink)
        {
        p().iInt = iCodecsCountUp;
        }
    else if (iSessionType == EQueryDec || iSessionType == EDnlink)
        {
        p().iInt = iCodecsCountDn;
        }

    p().iUint = KVoIPG711FrameLen20ms; //good for 10ms and 20ms
    aMessage.WriteL(0, p);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetSupportedEncodersL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetSupportedCodecsL(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    CBufFlat* dataCopyBuffer = CBufFlat::NewL(KArrayExpandSize);
    CleanupStack::PushL(dataCopyBuffer);
    RBufWriteStream stream;
    stream.Open(*dataCopyBuffer);
    CleanupClosePushL(stream);

    if (iCodecsCountUp > 0 && (iSessionType == EQueryEnc ||
            iSessionType == EUplink))
        {
        for (TInt i = 0; i < iCodecsCountUp; i++)
            {
            stream.WriteUint32L(iCodecsUp[i].FourCC());
            TRACE_PRN_N2(_L("VoIP->CodecUP[%d]==[0x%x]"), i+1, iCodecsUp[i].FourCC());
            }
        }
    else if (iCodecsCountDn > 0 && (iSessionType == EQueryDec ||
            iSessionType == EDnlink))
        {
        for (TInt i = 0; i < iCodecsCountDn; i++)
            {
            stream.WriteUint32L(iCodecsDn[i].FourCC());
            TRACE_PRN_N2(_L("VoIP->CodecDN[%d]==[0x%x]"), i+1, iCodecsDn[i].FourCC());
            }
        }

    aMessage.WriteL(0, dataCopyBuffer->Ptr(0));

    CleanupStack::PopAndDestroy(&stream);
    CleanupStack::PopAndDestroy(dataCopyBuffer);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::OpenDownlinkL
// Method for player initialization.
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::OpenDownlinkL(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    TMMFPrioritySettingsPckg pckg;
    aMessage.ReadL(0, pckg);
    iShared.iMutex.Wait();
    iShared.iPreference = pckg().iPref;
    iShared.iPriority = pckg().iPriority;
    iShared.iMutex.Signal();

    TInt err = KErrPermissionDenied;

    if (iVoIPServer.HasDnLinkSession())
        {
        User::Leave(err);
        }

    // Create thread only once
    if (iThread.Handle() <= 0)
        {
        err = iThread.Create(_L("VoIPDownlinkThread"),
                CVoIPDownlinkThread::ThreadFunction, KDefaultStackSize,
                KMinHeapSize, KMinHeapSize << 12, //1MB
                &iShared);
        if (err == KErrNone)
            {
            iThread.SetPriority(EPriorityRealTime);
            iThread.Resume();

            // Open message queue for ITC back to the client
            if (iMsgComQueue.Handle() <= 0)
                {
                err = iMsgComQueue.OpenGlobal(KDnLinkComQueue);
                }

            // Create message queue for ITC with CVoIPDownlinkThread
            if (iITCMsgComQueue.Handle() <= 0)
                {
                err = iITCMsgComQueue.CreateGlobal(KDnLinkThreadComQueue,
                        KVoIPMsgComQSlots);
                }

            iVoIPServer.SetDnLinkSession(this, ETrue);
            iSessionType = EDnlink;
            }
        }

    TRACE_PRN_IF_ERR(err);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::OpenUplinkL
// Method for recorder initialization.
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::OpenUplinkL(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    TMMFPrioritySettingsPckg pckg;
    aMessage.ReadL(0, pckg);
    iShared.iMutex.Wait();
    iShared.iPreference = pckg().iPref;
    iShared.iPriority = pckg().iPriority;
    iShared.iMutex.Signal();

    TInt err = KErrPermissionDenied;

    if (iVoIPServer.HasUpLinkSession())
        {
        User::Leave(err);
        }

    // Create thread only once
    if (iThread.Handle() <= 0)
        {
        err = iThread.Create(_L("VoIPUplinkThread"),
                CVoIPUplinkThread::ThreadFunction, KDefaultStackSize,
                KMinHeapSize, KMinHeapSize << 12, //1MB
                &iShared);
        if (err == KErrNone)
            {
            iThread.SetPriority(EPriorityRealTime);
            iThread.Resume();

            // Create message queues
            if (iMsgComQueue.Handle() <= 0)
                {
                err = iMsgComQueue.OpenGlobal(KUpLinkComQueue);
                }

            // Create message queue for ITC with CVoIPDownlinkThread
            if (iITCMsgComQueue.Handle() <= 0)
                {
                err = iITCMsgComQueue.CreateGlobal(KUpLinkThreadComQueue,
                        KVoIPMsgComQSlots);
                }

            iVoIPServer.SetUpLinkSession(this, ETrue);
            iSessionType = EUplink;
            }
        }

    TRACE_PRN_IF_ERR(err);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::SetFormat
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::SetFormatL(const RMessage2& aMessage)
    {
    TPckgBuf<TUint32> pckg;
    aMessage.ReadL(0, pckg);
    iShared.iMutex.Wait();
    iShared.iCodecSettings.iFourCC = pckg();
    iShared.iMutex.Signal();
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::StartDownlink
// Call the thread to initialize downlink stream
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::StartDownlink()
    {
    SendCmdToThread(ECmdStartDownlink);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::StartUplink
// Call the thread to initialize uplink stream
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::StartUplink()
    {
    SendCmdToThread(ECmdStartUplink);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::StopDownlink
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::StopDownlink()
    {
    SendCmdToThread(ECmdStopDownlink);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::StopUplink
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::StopUplink()
    {
    SendCmdToThread(ECmdStopUplink);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::CloseDownlink
// Request downlink thread termination
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::CloseDownlink()
    {
    if (iThread.Handle() > 0)
        {
        SendCmdToThread(ECmdTerminateThread);
        iThread.Close(); // close thread handle
        iVoIPServer.SetDnLinkSession(this, EFalse);
        NotifyClient(ECmdDownlinkThreadClosed);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::CloseUplink
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::CloseUplink()
    {
    if (iThread.Handle() > 0)
        {
        SendCmdToThread(ECmdTerminateThread);
        iThread.Close(); // close thread handle
        iVoIPServer.SetUpLinkSession(this, EFalse);
        NotifyClient(ECmdUplinkThreadClosed);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetMaxVolume
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetMaxVolumeL(const RMessage2& aMessage)
    {
    if (iThread.Handle() > 0 && iMaxVolume > 0)
        {
        TPckgBuf<TInt> p(iMaxVolume);
        aMessage.WriteL(0, p);
        }
    else
        {
        User::Leave(KErrBadHandle);
        }

    TRACE_PRN_N1(_L("VoIP->SRV-SESSION: GetMaxVolumeL->MaxVolume [%d]"),
            iMaxVolume);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetVolumeL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetVolumeL(const RMessage2& aMessage)
    {
    if (iThread.Handle() > 0)
        {
        iMessage = aMessage;
        iMsgQueued = ETrue;
        SendCmdToThread(ECmdGetVolume);
        }
    else
        {
        User::Leave(KErrBadHandle);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::SetVolume
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::SetVolume(const RMessage2& aMessage)
    {
    iShared.iMutex.Wait();
    iShared.iInt = aMessage.Int0();
    iShared.iMutex.Signal();
    SendCmdToThread(ECmdSetVolume);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetMaxGain
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetMaxGainL(const RMessage2& aMessage)
    {
    if (iThread.Handle() > 0 && iMaxGain > 0)
        {
        TPckgBuf<TInt> p(iMaxGain);
        aMessage.WriteL(0, p);
        }
    else
        {
        User::Leave(KErrBadHandle);
        }
    TRACE_PRN_N1(_L("VoIP->SRV-SESSION: GetMaxGainL->MaxGain [%d]"), iMaxGain);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetGainL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetGainL(const RMessage2& aMessage)
    {
    if (iThread.Handle() > 0)
        {
        iMessage = aMessage;
        iMsgQueued = ETrue;
        SendCmdToThread(ECmdGetGain);
        }
    else
        {
        User::Leave(KErrBadHandle);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::SetGain
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::SetGain(const RMessage2& aMessage)
    {
    TInt gain = aMessage.Int0();
    if (gain > iMaxGain)
        {
        gain = iMaxGain;
        }

    iShared.iMutex.Wait();
    iShared.iInt = gain;
    iShared.iMutex.Signal();
    SendCmdToThread(ECmdSetGain);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::SetAudioDeviceL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::SetAudioDeviceL(const RMessage2& aMessage)
    {
    TPckgBuf<TUint> pckg;
    aMessage.ReadL(0, pckg);
    iShared.iMutex.Wait();
    iShared.iAudioDevice = pckg();
    iShared.iMutex.Signal();
    SendCmdToThread(ECmdSetAudioDevice);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetAudioDeviceL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetAudioDeviceL(const RMessage2& aMessage)
    {
    if (iThread.Handle() > 0)
        {
        iMessage = aMessage;
        iMsgQueued = ETrue;
        SendCmdToThread(ECmdGetAudioDevice);
        }
    else
        {
        User::Leave(KErrBadHandle);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::BufferFilled
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::BufferFilledL(const RMessage2& aMessage)
    {
    TVoIPMsgBufPckg pckg;
    aMessage.ReadL(0, pckg);
    iShared.iMutex.Wait();
    iShared.iBufferSize = pckg().iInt;
    iShared.iSequenceNum = pckg().iUint;
    iShared.iMutex.Signal();
    SendCmdToThread(ECmdBufferFilled);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::BufferEmptied
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::BufferEmptiedL(const RMessage2& /*aMessage*/)
    {
    SendCmdToThread(ECmdBufferEmptied);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::SetIlbcCodecMode
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::SetIlbcCodecModeL(const RMessage2& aMessage)
    {
    TPckgBuf<CVoIPFormatIntfc::TILBCCodecMode> pckg;
    aMessage.ReadL(0, pckg);
    iShared.iMutex.Wait();
    iShared.iCodecSettings.iILBCCodecMode = pckg();
    iShared.iMutex.Signal();
    SendCmdToThread(ECmdSetIlbcCodecMode);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetIlbcCodecModeL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetIlbcCodecModeL(const RMessage2& aMessage)
    {
    if (iThread.Handle() > 0)
        {
        iMessage = aMessage;
        iMsgQueued = ETrue;
        SendCmdToThread(ECmdGetIlbcCodecMode);
        }
    else
        {
        User::Leave(KErrBadHandle);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::SetG711CodecMode
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::SetG711CodecModeL(const RMessage2& aMessage)
    {
    TPckgBuf<CVoIPFormatIntfc::TG711CodecMode> pckg;
    aMessage.ReadL(0, pckg);
    iShared.iMutex.Wait();
    iShared.iCodecSettings.iG711CodecMode = pckg();
    iShared.iMutex.Signal();
    SendCmdToThread(ECmdSetG711CodecMode);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetG711CodecModeL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetG711CodecModeL(const RMessage2& aMessage)
    {
    if (iThread.Handle() > 0)
        {
        iMessage = aMessage;
        iMsgQueued = ETrue;
        SendCmdToThread(ECmdGetG711CodecMode);
        }
    else
        {
        User::Leave(KErrBadHandle);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetSupportedBitratesCountL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetSupportedBitratesCountL(
        const RMessage2& aMessage)
    {
    if (iThread.Handle() > 0)
        {
        iMessage = aMessage;
        iMsgQueued = ETrue;
        SendCmdToThread(ECmdGetSupportedBitrates);
        }
    else
        {
        User::Leave(KErrBadHandle);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetSupportedBitratesL
// Bitrates are already returned from the codec as a result of call to
// GetSupportedBitratesCount(). Just pack them into a descriptor and return
// bact to the client.
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetSupportedBitratesL(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    CBufFlat* dataCopyBuffer = CBufFlat::NewL(KArrayExpandSize);
    CleanupStack::PushL(dataCopyBuffer);
    RBufWriteStream stream;
    stream.Open(*dataCopyBuffer);
    CleanupClosePushL(stream);

    RArray<TUint> bitrates;
    iShared.iMutex.Wait();
    bitrates = *iShared.iCodecSettings.iArrBitrates;
    iShared.iMutex.Signal();
    TInt numOfItems = bitrates.Count();

    for (TInt i = 0; i < numOfItems; i++)
        {
        stream.WriteUint32L(bitrates[i]);
        TRACE_PRN_N1(_L("VoIP->SRV-SESSION: BR: [%d]"), bitrates[i]);
        }

    aMessage.WriteL(0, dataCopyBuffer->Ptr(0));

    CleanupStack::PopAndDestroy(&stream);
    CleanupStack::PopAndDestroy(dataCopyBuffer);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::SetBitrate
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::SetBitrate(const RMessage2& aMessage)
    {
    TUint bitrate = aMessage.Int0();
    iShared.iMutex.Wait();
    iShared.iCodecSettings.iBitrate = bitrate;
    iShared.iMutex.Signal();
    SendCmdToThread(ECmdSetBitrate);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetBitrateL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetBitrateL(const RMessage2& aMessage)
    {
    if (iThread.Handle() > 0)
        {
        iMessage = aMessage;
        iMsgQueued = ETrue;
        SendCmdToThread(ECmdGetBitrate);
        }
    else
        {
        User::Leave(KErrBadHandle);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::FrameModeRequiredForEcL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::FrameModeRqrdForEcL(const RMessage2& aMessage)
    {
    if (iThread.Handle() > 0)
        {
        iMessage = aMessage;
        iMsgQueued = ETrue;
        SendCmdToThread(ECmdFrameModeRqrdForEC);
        }
    else
        {
        User::Leave(KErrBadHandle);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::SetFrameMode
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::SetFrameMode(const RMessage2& aMessage)
    {
    iShared.iMutex.Wait();
    iShared.iCodecSettings.iFrameMode = TBool(aMessage.Int0());
    iShared.iMutex.Signal();
    SendCmdToThread(ECmdSetFrameMode);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetFrameModeL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetFrameModeL(const RMessage2& aMessage)
    {
    if (iThread.Handle() > 0)
        {
        iMessage = aMessage;
        iMsgQueued = ETrue;
        SendCmdToThread(ECmdGetFrameMode);
        }
    else
        {
        User::Leave(KErrBadHandle);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::ConcealErrorForNextBuffer
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::ConcealErrorForNextBuffer()
    {
    SendCmdToThread(ECmdConcealErrForNextBuf);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::SetVad
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::SetVad(const RMessage2& aMessage)
    {
    iShared.iMutex.Wait();
    iShared.iCodecSettings.iVad = TBool(aMessage.Int0());
    iShared.iMutex.Signal();
    SendCmdToThread(ECmdSetVad);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetVadL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetVadL(const RMessage2& aMessage)
    {
    if (iThread.Handle() > 0)
        {
        iMessage = aMessage;
        iMsgQueued = ETrue;
        SendCmdToThread(ECmdGetVad);
        }
    else
        {
        User::Leave(KErrBadHandle);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::SetCng
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::SetCng(const RMessage2& aMessage)
    {
    iShared.iMutex.Wait();
    iShared.iCodecSettings.iCng = TBool(aMessage.Int0());
    iShared.iMutex.Signal();
    SendCmdToThread(ECmdSetCng);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetCngL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetCngL(const RMessage2& aMessage)
    {
    if (iThread.Handle() > 0)
        {
        iMessage = aMessage;
        iMsgQueued = ETrue;
        SendCmdToThread(ECmdGetCng);
        }
    else
        {
        User::Leave(KErrBadHandle);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::SetPlc
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::SetPlc(const RMessage2& aMessage)
    {
    iShared.iMutex.Wait();
    iShared.iCodecSettings.iPlc = TBool(aMessage.Int0());
    iShared.iMutex.Signal();
    SendCmdToThread(ECmdSetPlc);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetPlcL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetPlcL(const RMessage2& aMessage)
    {
    if (iThread.Handle() > 0)
        {
        iMessage = aMessage;
        iMsgQueued = ETrue;
        SendCmdToThread(ECmdGetPlc);
        }
    else
        {
        User::Leave(KErrBadHandle);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::BadLsfNextBuffer
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::BadLsfNextBuffer()
    {
    SendCmdToThread(ECmdBadLsfNextBuffer);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::ConfigureJitterBufferL
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::ConfigureJitterBufferL(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    TPckgBuf<TVoIPJBConfig> pckg;
    aMessage.ReadL(0, pckg);
    iShared.iMutex.Wait();
    iShared.iJBConfig.iJBBufferLength = pckg().iJBBufferLength;
    iShared.iJBConfig.iJBThreshold = pckg().iJBThreshold;
    iShared.iJBConfig.iJitterLatency = pckg().iJitterLatency;
    iShared.iJBConfig.iSampleInterval = pckg().iSampleInterval;
    iShared.iJBConfig.iJBInactivityTimeOut = pckg().iJBInactivityTimeOut;
    iShared.iMutex.Signal();
    SendCmdToThread(ECmdVoIPConfigJB);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::ResetJitterBuffer
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::ResetJitterBuffer(const RMessage2& aMessage)
    {
    iShared.iMutex.Wait();
    iShared.iBool = TBool(aMessage.Int0());
    iShared.iMutex.Signal();
    SendCmdToThread(ECmdVoIPResetJB);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::JBDelayDown
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::JBDelayDown()
    {
    SendCmdToThread(ECmdVoIPJBDelayDown);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::JBDelayUp
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::JBDelayUp()
    {
    SendCmdToThread(ECmdVoIPJBDelayUp);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::OpenDTMFTonePlayerL
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::OpenDTMFTonePlayerL()
    {
    TRACE_PRN_FN_ENT;

    if (iSessionType == EUplink || iSessionType == EDnlink)
        {
        CloseDTMFTonePlayer();
        iDTMFTonePlayer = CMMFDevSound::NewL();

        if (iDTMFTonePlayer)
            {
            iDTMFTonePlayerInitRequest = ETrue;
            TRAPD(err, iDTMFTonePlayer->InitializeL(*this,
                    EMMFStateTonePlaying));
            if (err != KErrNone)
                {
                delete iDTMFTonePlayer;
                iDTMFTonePlayer = NULL;
                iDTMFTonePlayerInitRequest = EFalse;
                return;
                }

            // Open message queue for ITC back to the client
            if (iMsgComQueueDtmf.Handle() <= 0)
                {
                switch (iSessionType)
                    {
                    case EDnlink:
                        {
                        while (iMsgComQueueDtmf.OpenGlobal(
                                KDTMFToneComQueueDNL))
                            {
                            // to make sure queue is created
                            User::After(KQueueCreateWaitTime);
                            }
                        break;
                        }
                    case EUplink:
                        {
                        while (iMsgComQueueDtmf.OpenGlobal(
                                KDTMFToneComQueueUPL))
                            {
                            // to make sure queue is created
                            User::After(KQueueCreateWaitTime);
                            }
                        break;
                        }
                    default:
                        break;
                    }
                }
            }
        }

    TRACE_PRN_FN_EXT;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioServerSession::PlayDTMFToneL
// ---------------------------------------------------------------------------
//
void CVoIPAudioServerSession::PlayDTMFToneL(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    // NOTE: no need to queue message, we can complete the message and let
    //       DTMF tones play automatically upon MatoPrepareComplete.

    if (iDTMFTonePlayer)
        {
        TTonesMsgBufPckg pckg;
        aMessage.ReadL(0, pckg);
        TBuf<KTonesBufferSize> tones;
        tones = pckg().iFileName;
        iDTMFTonePlayer->SetDTMFLengths(iToneLenOn, iToneLenOff,
                iToneLenPause);
        iDTMFTonePlayer->PlayDTMFStringL(tones);
        }

    TRACE_PRN_FN_EXT;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioServerSession::StopDTMFTonePlay
// ---------------------------------------------------------------------------
//
void CVoIPAudioServerSession::StopDTMFTonePlay()
    {
    if (iDTMFTonePlayer)
        {
        iDTMFTonePlayer->Stop();
        }
    }

// ---------------------------------------------------------------------------
// CVoIPAudioServerSession::CloseDTMFTonePlayer
// ---------------------------------------------------------------------------
//
void CVoIPAudioServerSession::CloseDTMFTonePlayer()
    {
    delete iDTMFTonePlayer;
    iDTMFTonePlayer = NULL;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::OpenRingTonePlayerFromFileL
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::OpenRingTonePlayerFromFileL(
        const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    InitRingTonePlayerL();
    TTonesMsgBufPckg pckg;
    aMessage.ReadL(0, pckg);
    iRingTonePlayer->OpenFileL(pckg().iFileName);
    iSessionType = ERingTone;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::OpenRingTonePlayerFromRFileL
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::OpenRingTonePlayerFromRFileL(
        const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    InitRingTonePlayerL();
    RFile file;
    // Note: iFs and file handle are already open by the client who
    //       owns these handles.
    TInt err = file.AdoptFromClient(aMessage, 0, 1); // 0==iFs, 1==fileHandle
    CleanupClosePushL(file);

    if (err == KErrNone)
        {
        iRingTonePlayer->OpenFileL(file);
        iSessionType = ERingTone;
        }

    file.Close(); //will RFs close?
    CleanupStack::Pop(&file);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::OpenRingTonePlayerFromProfileL
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::OpenRingTonePlayerFromProfileL()
    {
    TRACE_PRN_FN_ENT;

    InitRingTonePlayerL();

    MProEngEngine* engine = ProEngFactory::NewEngineL();
    CleanupReleasePushL(*engine);
    MProEngProfile* profile = engine->ActiveProfileLC();
    MProEngTones& tones = profile->ProfileTones();

    // NOTE: Here is how to use volume settings from Profiles
    // const TProfileToneSettings& ts = tones.ToneSettings();
    // iVolume = ts.iRingingVolume;

    TBuf<KMaxFileName> file;
    file = tones.RingingTone1();
    // NOTE: alternate line settings can also be used from RingingTone2
    iRingTonePlayer->OpenFileL(file);
    iSessionType = ERingTone;

    CleanupStack::PopAndDestroy(2); //profile, engine

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::OpenRingTonePlayerFromDesL
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::OpenRingTonePlayerFromDesL(
        const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    InitRingTonePlayerL();
    TInt size = aMessage.GetDesLength(0);

    if (size > 0)
        {
        delete iToneBuf;
        iToneBuf = NULL;
        iToneBuf = HBufC8::NewL(size);
        TPtr8 ptr = iToneBuf->Des();
        aMessage.ReadL(0, ptr);
        iRingTonePlayer->OpenDesL(*iToneBuf);
        iSessionType = ERingTone;
        }
    else
        {
        User::Leave(KErrArgument); //-6
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::PlayRingTone
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::PlayRingTone()
    {
    if (iRingTonePlayer)
        {
        iRingTonePlayer->Play(); //TODO: function returns status
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::PauseRingTone
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::PauseRingTone()
    {
    if (iRingTonePlayer)
        {
        iRingTonePlayer->Pause(); //TODO: function returns status
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::ResumeRingTone
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::ResumeRingTone()
    {
    if (iRingTonePlayer)
        {
        iRingTonePlayer->Play();
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::StopRingTone
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::StopRingTone()
    {
    if (iRingTonePlayer)
        {
        iRingTonePlayer->Stop();
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::CloseRingTonePlayer
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::CloseRingTonePlayer()
    {
    if (iRingTonePlayer)
        {
        iRingTonePlayer->Close();
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::InitRingTonePlayerL
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::InitRingTonePlayerL()
    {
    if (!iRingTonePlayer)
        {
        TMdaPriorityPreference pref = TMdaPriorityPreference(
                KAudioPrefIncomingCall);
        TInt prio = KAudioPriorityRingingTone;
        iRingTonePlayer = CMdaAudioPlayerUtility::NewL(*this, prio, pref);
        }

    // Open message queue for ITC back to the client
    if (iRingTonePlayer && iMsgComQueue.Handle() <= 0)
        {
        while (iMsgComQueue.OpenGlobal(KRingToneComQueue))
            {
            // just to make sure queue is created
            User::After(KQueueCreateWaitTime);
            }
        }
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServerSession::DownlinkInitComplete
//
// ----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::DownlinkInitComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(aError);

    // state == DNL initialized
    //                    iStatus = EReady;
    iShared.iMutex.Wait();
    iMaxVolume = iShared.iMaxVolume;
    iShared.iMutex.Signal();
    NotifyClient(ECmdDownlinkInitComplete, aError);

    TRACE_PRN_FN_EXT;
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServerSession::UplinkInitComplete
//
// ----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::UplinkInitComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(aError);

    // state == UPL initialized
    //                    iStatus = EReady;
    iShared.iMutex.Wait();
    iMaxGain = iShared.iMaxGain;
    iShared.iMutex.Signal();
    NotifyClient(ECmdUplinkInitComplete, aError);

    TRACE_PRN_FN_EXT;
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetVolumeComplete
//
// ----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetVolumeComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(aError);

    iShared.iMutex.Wait();
    TInt vol = iShared.iInt;
    iShared.iMutex.Signal();
    TVoIPMsgBufPckg p;
    p().iInt = vol;
    p().iStatus = aError;
    TRAPD(err, iMessage.WriteL(0, p));
    iMessage.Complete(err);
    iMsgQueued = EFalse;

    TRACE_PRN_FN_EXT;
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetGainComplete
//
// ----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetGainComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(aError);

    iShared.iMutex.Wait();
    TInt gain = iShared.iInt;
    iShared.iMutex.Signal();
    TVoIPMsgBufPckg p;
    p().iInt = gain;
    p().iStatus = aError;
    TRAPD(err, iMessage.WriteL(0, p));
    iMessage.Complete(err);
    iMsgQueued = EFalse;

    TRACE_PRN_FN_EXT;
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetAudioDeviceComplete
//
// ----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetAudioDeviceComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(aError);

    TVoIPMsgBufPckg p;
    iShared.iMutex.Wait();
    p().iUint = iShared.iAudioDevice;
    p().iStatus = aError;
    iShared.iMutex.Signal();
    TRAPD(err, iMessage.WriteL(0, p));
    iMessage.Complete(err);
    iMsgQueued = EFalse;

    TRACE_PRN_FN_EXT;
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetSupportedBitratesComplete
// Notifies bitrate count first.
// ----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetSupportedBitratesComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(aError);

    TVoIPMsgBufPckg p;
    p().iInt = 0;
    p().iStatus = aError;

    iShared.iMutex.Wait();
    if (iShared.iCodecSettings.iArrBitrates)
        {
        p().iInt = iShared.iCodecSettings.iArrBitrates->Count();
        }
    iShared.iMutex.Signal();
    TRAPD(err, iMessage.WriteL(0, p));
    iMessage.Complete(err);
    iMsgQueued = EFalse;

    TRACE_PRN_FN_EXT;
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetBitrateComplete
//
// ----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetBitrateComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(aError);

    TVoIPMsgBufPckg p;
    iShared.iMutex.Wait();
    p().iUint = iShared.iCodecSettings.iBitrate;
    p().iStatus = aError;
    iShared.iMutex.Signal();
    TRAPD(err, iMessage.WriteL(0, p));
    iMessage.Complete(err);
    iMsgQueued = EFalse;

    TRACE_PRN_FN_EXT;
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetG711CodecModeComplete
//
// ----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetG711CodecModeComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(aError);

    TVoIPMsgBufPckg p;
    iShared.iMutex.Wait();
    p().iUint = TUint(iShared.iCodecSettings.iG711CodecMode);
    iShared.iMutex.Signal();
    p().iStatus = aError;
    TRAPD(err, iMessage.WriteL(0, p));
    iMessage.Complete(err);
    iMsgQueued = EFalse;

    TRACE_PRN_FN_EXT;
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetIlbcCodecModeComplete
//
// ----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetIlbcCodecModeComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(aError);

    TVoIPMsgBufPckg p;
    iShared.iMutex.Wait();
    p().iUint = TUint(iShared.iCodecSettings.iILBCCodecMode);
    iShared.iMutex.Signal();
    p().iStatus = aError;
    TRAPD(err, iMessage.WriteL(0, p));
    iMessage.Complete(err);
    iMsgQueued = EFalse;

    TRACE_PRN_FN_EXT;
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetFrameModeComplete
//
// ----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetFrameModeComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(aError);

    TVoIPMsgBufPckg p;
    iShared.iMutex.Wait();
    p().iBool = iShared.iCodecSettings.iFrameMode;
    iShared.iMutex.Signal();
    p().iStatus = aError;
    TRAPD(err, iMessage.WriteL(0, p));
    iMessage.Complete(err);
    iMsgQueued = EFalse;

    TRACE_PRN_FN_EXT;
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetFrameModeReqForECComplete
//
// ----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetFrameModeReqForECComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(aError);

    TVoIPMsgBufPckg p;
    iShared.iMutex.Wait();
    p().iBool = iShared.iCodecSettings.iFrameModeReqForEC;
    iShared.iMutex.Signal();
    p().iStatus = aError;
    TRAPD(err, iMessage.WriteL(0, p));
    iMessage.Complete(err);
    iMsgQueued = EFalse;

    TRACE_PRN_FN_EXT;
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetVadComplete
//
// ----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetVadComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(aError);

    TVoIPMsgBufPckg p;
    iShared.iMutex.Wait();
    p().iBool = iShared.iCodecSettings.iVad;
    iShared.iMutex.Signal();
    p().iStatus = aError;
    TRAPD(err, iMessage.WriteL(0, p));
    iMessage.Complete(err);
    iMsgQueued = EFalse;

    TRACE_PRN_FN_EXT;
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetCngComplete
//
// ----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetCngComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(aError);

    TVoIPMsgBufPckg p;
    iShared.iMutex.Wait();
    p().iBool = iShared.iCodecSettings.iCng;
    iShared.iMutex.Signal();
    p().iStatus = aError;
    TRAPD(err, iMessage.WriteL(0, p));
    iMessage.Complete(err);
    iMsgQueued = EFalse;

    TRACE_PRN_FN_EXT;
    }

// ----------------------------------------------------------------------------
// CVoIPAudioServerSession::GetPlcComplete
//
// ----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::GetPlcComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(aError);

    TVoIPMsgBufPckg p;
    iShared.iMutex.Wait();
    p().iBool = iShared.iCodecSettings.iPlc;
    iShared.iMutex.Signal();
    p().iStatus = aError;
    TRAPD(err, iMessage.WriteL(0, p));
    iMessage.Complete(err);
    iMsgQueued = EFalse;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::DetermineG711FrameRateL
// -----------------------------------------------------------------------------
/*
void CVoIPAudioServerSession::DetermineG711FrameRateL()
    {
    if (iDevSound)
        {
        iDevSoundInitRequest = ETrue;
        TMMFState state = EMMFStatePlaying;

        if (iSessionType == EUplink)
            {
            state = EMMFStateRecording;
            }
        TRAPD(err, iDevSound->InitializeL(*this, TFourCC(KMccFourCCIdG711),
                state));

        if (err != KErrNone)
            {
            delete iDevSound;
            iDevSound = NULL;
            iDevSoundInitRequest = EFalse;
            }
        }
    }*/

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::ReleaseQueuedMessage
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::ReleaseQueuedMessage(TInt aError, TUint aFrmSize)
    {
    delete iDevSound;
    iDevSound = NULL;
    iDevSoundInitRequest = EFalse;
    TVoIPMsgBufPckg p;

    if (iSessionType == EQueryEnc || iSessionType == EUplink)
        {
        p().iInt = iCodecsCountUp;
        }
    else if (iSessionType == EQueryDec || iSessionType == EDnlink)
        {
        p().iInt = iCodecsCountDn;
        }

    p().iUint = aFrmSize;
    p().iStatus = aError;
    TRAPD(err, iMessage.WriteL(0, p));
    iMessage.Complete(err);
    iMsgQueued = EFalse;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::SendCmdToThread
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::SendCmdToThread(const TInt aCommand,
        const TInt aStatus)
    {
    if (iThread.Handle() > 0)
        {
        iMsgBuffer.iRequest = aCommand;
        iMsgBuffer.iStatus = aStatus;
        iITCMsgComQueue.Send(iMsgBuffer);
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::NotifyClient
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::NotifyClient(const TInt aCommand,
        const TInt aStatus, const TInt64 /*aInt64*/)
    {
    iMsgBuffer.iRequest = aCommand;
    iMsgBuffer.iStatus = aStatus;
    iMsgComQueue.Send(iMsgBuffer);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::NotifyDtmfClient
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::NotifyDtmfClient(const TInt aCommand,
        const TInt aStatus)
    {
    iMsgBuffer.iRequest = aCommand;
    iMsgBuffer.iStatus = aStatus;
    iMsgComQueueDtmf.Send(iMsgBuffer);
    }

// CALBACKS

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::InitializeComplete
//
// From MDevSoundObserver
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::InitializeComplete(TInt aError)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(aError);

    if (aError == KErrNone)
        {
        TMMFPrioritySettings pSet;

        if (iDTMFTonePlayer && iDTMFTonePlayerInitRequest)
            {
            // initialized DTMF player
            pSet.iPref = KAudioDTMFString;
            pSet.iPriority = KAudioPriorityDTMFString;
            iDTMFTonePlayer->SetPrioritySettings(pSet);
            iDTMFTonePlayer->SetVolume(iMaxVolume);
            iToneLenOn = KDTMFToneLengthOn;
            iToneLenOff = KDTMFToneLengthOff;
            iToneLenPause = KDTMFToneLengthPause;
            iDTMFTonePlayerInitRequest = EFalse;

            if (iSessionType == EUplink)
                {
                NotifyDtmfClient(ECmdDTMFOpenUplinkComplete, aError);
                }
            else
                {
                NotifyDtmfClient(ECmdDTMFOpenDnlinkComplete, aError);
                }
            }
        else if (iDevSound && iDevSoundInitRequest)
            {
            // initialized standard player to check G711 frame rate
            iShared.iMutex.Wait();
            pSet.iPref = iShared.iPreference;
            pSet.iPriority = iShared.iPriority;
            iShared.iMutex.Signal();
            iDevSound->SetPrioritySettings(pSet);
            iDevSoundInitRequest = EFalse;

            TInt err = KErrNone;

            if (iSessionType == EUplink)
                {
                TRAP(err, iDevSound->RecordInitL());
                }
            else
                {
                TRAP(err, iDevSound->PlayInitL());
                }

            if (err != KErrNone)
                {
                ReleaseQueuedMessage(err);
                }
            }
        }
    else
        {
        if (iDevSound && iDevSoundInitRequest)
            {
            ReleaseQueuedMessage(aError);
            }
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::ToneFinished
//
// From MDevSoundObserver
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::ToneFinished(TInt aError)
    {
    TRACE_PRN_IF_ERR(aError);
    NotifyDtmfClient(ECmdDTMFTonePlayFinished, aError);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::BufferToBeEmptied
//
// From MDevSoundObserver
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::BufferToBeEmptied(CMMFBuffer* aBuffer)
    {
    CMMFDataBuffer* bufPtr = static_cast<CMMFDataBuffer*> (aBuffer);
    TInt len = bufPtr->BufferSize();
    iDevSound->Stop();

    if (len == KVoIPG711FrameLen10ms)
        {
        iShared.iMutex.Wait();
        iShared.iCodecSettings.iG711FrameRate = TVoIPCodecSettings::E10MS;
        iShared.iMutex.Signal();
        }

    ReleaseQueuedMessage(KErrNone, len);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::BufferToBeFilled
//
// From MDevSoundObserver
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::BufferToBeFilled(CMMFBuffer* aBuffer)
    {
    CMMFDataBuffer* bufPtr = static_cast<CMMFDataBuffer*> (aBuffer);
    TInt len = bufPtr->RequestSize();
    iDevSound->Stop();

    if (len == KVoIPG711FrameLen10ms)
        {
        iShared.iMutex.Wait();
        iShared.iCodecSettings.iG711FrameRate = TVoIPCodecSettings::E10MS;
        iShared.iMutex.Signal();
        }

    ReleaseQueuedMessage(KErrNone, len);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::RecordError
//
// From MDevSoundObserver
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::RecordError(TInt aError)
    {
    ReleaseQueuedMessage(aError);
    TRACE_PRN_IF_ERR(aError);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::PlayError
//
// From MDevSoundObserver
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::PlayError(TInt aError)
    {
    ReleaseQueuedMessage(aError);
    TRACE_PRN_IF_ERR(aError);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::MapcInitComplete
//
// From MMdaAudioPlayerCallback
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::MapcInitComplete(TInt aError,
        const TTimeIntervalMicroSeconds& aDuration)
    {
    TRACE_PRN_IF_ERR(aError);

    if (aError == KErrNone)
        {
        TInt vol = iRingTonePlayer->MaxVolume();
        iRingTonePlayer->SetVolume(vol / 2);
        }

    NotifyClient(ECmdRingToneOpenComplete, aError, aDuration.Int64());
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::MapcInitComplete
//
// From MMdaAudioPlayerCallback
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::MapcPlayComplete(TInt aError)
    {
    TRACE_PRN_IF_ERR(aError);
    NotifyClient(ECmdRingTonePlayComplete, aError);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::DoHandleError
//
// From MThreadComObserver
//
// The recorder thread passes commands to this thread via an active object.
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::DoHandleError(TInt /*aError*/)
    {/*
    DEBPRN1(_L("VoIP->CVoIPAudioServerSession[0x%x]::DoHandleError [%d] :>"), aError);
    iShared.iMutex.Wait();
    TUserCommand cmd = iShared.iCmd;
    iShared.iMutex.Signal();

    if (iMsgQueued)
        {
        iMessage.Complete(aError);
        iMsgQueued = EFalse;
        }
    else
        {
        NotifyClient(cmd, aError);
        }*/
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServerSession::DoHandleCmd
//
// From MThreadComObserver
//
// Threads pass complete async request events to this thread via an AO.
// -----------------------------------------------------------------------------
//
void CVoIPAudioServerSession::DoHandleCmd(TExcType aExc, TInt aError)
    {
    TRACE_PRN_IF_ERR(aError);

    iShared.iMutex.Wait();
    TUserCommand cmd = iShared.iCmd;
    iShared.iMutex.Signal();

    if (aExc == EExcUserInterrupt)
        {
        switch (cmd)
            {
            case ECmdDownlinkInitComplete:
                {
                DownlinkInitComplete(aError);
                break;
                }
            case ECmdUplinkInitComplete:
                {
                UplinkInitComplete(aError);
                break;
                }
            case ECmdGetVolumeComplete:
                {
                GetVolumeComplete(aError);
                break;
                }
            case ECmdGetGainComplete:
                {
                GetGainComplete(aError);
                break;
                }
            case ECmdGetAudioDeviceComplete:
                {
                GetAudioDeviceComplete(aError);
                break;
                }
            case ECmdGetSupportedBitratesComplete:
                {
                GetSupportedBitratesComplete(aError);
                break;
                }
            case ECmdGetBitrateComplete:
                {
                GetBitrateComplete(aError);
                break;
                }
            case ECmdGetG711CodecModeComplete:
                {
                GetG711CodecModeComplete(aError);
                break;
                }
            case ECmdGetIlbcCodecModeComplete:
                {
                GetIlbcCodecModeComplete(aError);
                break;
                }
            case ECmdGetFrameModeComplete:
                {
                GetFrameModeComplete(aError);
                break;
                }
            case ECmdGetFrameModeReqForECComplete:
                {
                GetFrameModeReqForECComplete(aError);
                break;
                }
            case ECmdGetVadComplete:
                {
                GetVadComplete(aError);
                break;
                }
            case ECmdGetCngComplete:
                {
                GetCngComplete(aError);
                break;
                }
            case ECmdGetPlcComplete:
                {
                GetPlcComplete(aError);
                break;
                }
            case ECmdDnLinkError:
                {
                NotifyClient(ECmdDnLinkError, aError);
                break;
                }
            case ECmdUpLinkError:
                {
                NotifyClient(ECmdUpLinkError, aError);
                break;
                }
            default:
                break;
            }
        }
    }

// End of file
