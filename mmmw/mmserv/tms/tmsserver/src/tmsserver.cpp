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

#include <e32svr.h>
#include <e32uid.h>
#include <e32capability.h>
#include <AudioPreference.h>
#include "tmsutility.h"
#include "tmsclientserver.h"
#include "tmstelephonycenrep.h"
#include "tmsservershutdown.h"
#include "tmsserversession.h"
#include "tmscallserverstartparam.h"
#include "tmscallserver.h"
#include "tmscallclisrv.h"
#include "tmsglobaleffectssettings.h"
#include "tmstareventhandler.h"
#include "tmscenrepaudiohandler.h"
#include "tmsdtmftoneplayer.h"
#include "tmsdtmfprovider.h"
#include "tmspubsublistener.h"
#include "tmsrtparam.h"
#include "tmsserver.h"

using namespace TMS;

// CONSTANTS
const TInt KTMSShutDownDelayTime = 1000000; // 1 sec delay time
const TInt KOutputsArraySize = 10;

// -----------------------------------------------------------------------------
// TMSServer::NewL
//
// -----------------------------------------------------------------------------
//
TMSServer* TMSServer::NewLC()
    {
    TMSServer* self = new (ELeave) TMSServer;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// TMSServer::TMSServer
//
// -----------------------------------------------------------------------------
//
TMSServer::TMSServer() :
    CServer2(CActive::EPriorityHigh, ESharableSessions),
    iSession(0)
    {
    }

// -----------------------------------------------------------------------------
// TMSServer::~TMSServer
//
// -----------------------------------------------------------------------------
//
TMSServer::~TMSServer()
    {
    TRACE_PRN_FN_ENT;

    delete iShutdownTimer;
    iTMSCallServList.ResetAndDestroy();
    iTMSCallServList.Close();
    iDnlCodecs.Reset();
    iDnlCodecs.Close();
    iUplCodecs.Reset();
    iUplCodecs.Close();
    delete iEffectSettings;
    delete iTarHandler;
    delete iAudioCenRepHandler;
    DeinitRingTonePlayer();
    delete iTMSRtPlayer;
    delete iDTMFUplinkPlayer;
    delete iDTMFUplinkPlayerEtel;
    delete iDTMFDnlinkPlayer;
    delete iSyncVol;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::NewSessionL
//
// -----------------------------------------------------------------------------
//
CSession2* TMSServer::NewSessionL(const TVersion& aVersion,
        const RMessage2& aMessage) const
    {
    TRACE_PRN_FN_ENT;

    if (!aMessage.HasCapability(ECapabilityMultimediaDD))
        {
        User::Leave(KErrPermissionDenied);
        }

    const TVersion version(KTMSServMajorVersionNumber,
            KTMSServMinorVersionNumber, KTMSServBuildVersionNumber);

    if (!User::QueryVersionSupported(version, aVersion))
        {
        User::Leave(KErrNotSupported);
        }

    TMSServerSession* session = TMSServerSession::NewL(*((TMSServer*) this));

    TRACE_PRN_FN_EXT;
    return session;
    }

// -----------------------------------------------------------------------------
// TMSServer::ConstructL
//
// -----------------------------------------------------------------------------
//
void TMSServer::ConstructL()
    {
    TRACE_PRN_FN_ENT;

    iShutdownTimer = TMSServerShutDown::NewL();
    StartL(KTMSServerName);
    RThread().SetPriority(EPriorityRealTime);
    iEffectSettings = TMSGlobalEffectsSettings::NewL();
    iTarHandler = NULL;
    iAudioCenRepHandler = NULL;
    iCurrentRouting = TMS_AUDIO_OUTPUT_PRIVATE;
    iDTMFDnlinkPlayer = NULL;
    iDTMFUplinkPlayer = NULL;
    iDTMFUplinkPlayerEtel = NULL;
    iActiveCallType = -1;
    iSyncVol = TMSPubSubListener::NewL(KTMSPropertyCategory, ESyncVolume, this);

    //TODO: EUnit fails to initialize ProfileEngine in RT in eshell mode
    TRAP_IGNORE(InitRingTonePlayerL());

    // We need it running for global volume change updates
    StartRoutingNotifierL();

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::AddSession
//
// -----------------------------------------------------------------------------
//
void TMSServer::AddSession()
    {
    iSession++;
    // If shutdown timer is active, cancel it here.
    iShutdownTimer->Cancel();

    TRACE_PRN_N1(_L("TMS->SRV: AddSession->Active Sessions: [%d]"), iSession);
    }

// -----------------------------------------------------------------------------
// TMSServer::DropSession
//
// -----------------------------------------------------------------------------
//
void TMSServer::DropSession()
    {
    iSession--;

    // If session count is zero, start server shutdown
    if (iSession <= 0 && iShutdownTimer)
        {
        iShutdownTimer->SetDelay(TTimeIntervalMicroSeconds32(
                KTMSShutDownDelayTime));
        }

    TRACE_PRN_N1(_L("TMS->DNL: DropSession->Active Sessions: [%d]"), iSession);
    }

// -----------------------------------------------------------------------------
// TMSServer::SessionCount
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::SessionCount() const
    {
    return iSession;
    }

// -----------------------------------------------------------------------------
// TMSServer::GetNewTMSCallSessionHandleL
//
// -----------------------------------------------------------------------------
//
void TMSServer::GetNewTMSCallSessionHandleL(RHandleBase& aHandle)
    {
    TRACE_PRN_FN_ENT;

    TInt i = 0;
    while (i < iTMSCallServList.Count())
        {
        TMSStartAndMonitorTMSCallThread* callThread = iTMSCallServList[i];
        if (!callThread->IsActive())
            {
            iTMSCallServList.Remove(i);
            delete callThread;
            }
        else
            {
            i++;
            }
        }
    iTMSCallServList.Compress();

    TMSCallProxyLocal tmsCallSessionHandle;
    User::LeaveIfError(StartTMSCallServer(tmsCallSessionHandle));
    aHandle = tmsCallSessionHandle;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::StartTMSCallServer
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::StartTMSCallServer(TMSCallProxyLocal& aHandle)
    {
    TRACE_PRN_FN_ENT;

    TInt status = TMS_RESULT_SUCCESS;
    TMSStartAndMonitorTMSCallThread* callServerThread = NULL;
    TRAP(status, callServerThread = TMSStartAndMonitorTMSCallThread::NewL(
            const_cast<TMSServer*>(this)));
    if (status != TMS_RESULT_SUCCESS)
        {
        delete callServerThread;
        }
    else
        {
        status = iTMSCallServList.Append(callServerThread);
        if (callServerThread && status == TMS_RESULT_SUCCESS)
            {
            status = callServerThread->StartTMSCallServer(aHandle);
            TInt count = 0;
            count = iTMSCallServList.Count();

            iTMSCallServList[count - 1]->iTMSCallProxyLocal = aHandle;
            FindActiveCallType();

            if (iActiveCallType == TMS_CALL_CS)
                {
                //cache global vol and global gain to call server thread.
                TInt volume;
                switch (iCurrentRouting)
                    {
                    case TMS_AUDIO_OUTPUT_LOUDSPEAKER:
                        {
                        iEffectSettings->GetLoudSpkrVolume(volume);
                        TRACE_PRN_N1(_L("loudspk vol %d"),volume);
                        }
                        break;
                    default:
                        {
                        iEffectSettings->GetEarPieceVolume(volume);
                        TRACE_PRN_N1(_L("ear vol %d"),volume);
                        }
                        break;
                    }
                aHandle.SendToCallServer(TMS_EFFECT_GLOBAL_VOL_SET, volume);
                aHandle.SendToCallServer(TMS_EFFECT_GLOBAL_GAIN_SET,
                        iEffectSettings->Gain());
                }
            }
        else
            {
            delete callServerThread;
            }
        }

    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::StartRoutingNotifierL
//
// -----------------------------------------------------------------------------
//
void TMSServer::StartRoutingNotifierL()
    {
    TRACE_PRN_FN_ENT;

    if (!iTarHandler)
        {
        iTarHandlerCount = 0;
        iTarHandler = TMSTarEventHandler::NewL((const_cast<TMSServer*> (this)));
        }
    iTarHandlerCount++;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::CancelRoutingNotifier
//
// -----------------------------------------------------------------------------
//
void TMSServer::CancelRoutingNotifier()
    {
    if (--iTarHandlerCount < 1)
        {
        delete iTarHandler;
        iTarHandler = NULL;
        }
    }

// -----------------------------------------------------------------------------
// TMSServer::StartCenRepHandlerL
//
// -----------------------------------------------------------------------------
//
void TMSServer::StartCenRepHandlerL()
    {
    TRACE_PRN_FN_ENT;

#ifdef _USE_TELEPHONY_CENREP_
    if (!iAudioCenRepHandler)
        {
        iAudioCenRepHandlerCount = 0;
        iAudioCenRepHandler = TMSCenRepAudioHandler::NewL(
                (const_cast<TMSServer*> (this)));
        }
    iAudioCenRepHandlerCount++;
#endif

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::CancelCenRepHandler
//
// -----------------------------------------------------------------------------
//
void TMSServer::CancelCenRepHandler()
    {
    if (--iAudioCenRepHandlerCount <= 0)
        {
        delete iAudioCenRepHandler;
        iAudioCenRepHandler = NULL;
        }
    }

// -----------------------------------------------------------------------------
// TMSServer::SetOutput
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::SetOutput(CSession2* /*sid*/, TInt output)
    {
    TRACE_PRN_FN_ENT;

    TInt status = SendMessageToCallServ(TMS_ROUTING_OUTPUT_SET, output);
    if (status == TMS_RESULT_SUCCESS)
        {
        iCurrentRouting = output;
        }

    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::GetOutput
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::GetOutput(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    TUint output;
    TInt i = 0;
    TInt status(TMS_RESULT_SUCCESS);
    while (i < iTMSCallServList.Count())
        {
        TMSStartAndMonitorTMSCallThread* callThread = iTMSCallServList[i];

        if (callThread)
            {
            status = callThread->iTMSCallProxyLocal.ReceiveFromCallServer(
                    TMS_ROUTING_OUTPUT_GET, output);
            if (status != TMS_RESULT_SUCCESS)
                {
                break;
                }
            }
        i++;
        }
    TPckgBuf<TInt> p(output);
    aMessage.Write(0, p);
    aMessage.Complete(TMS_RESULT_SUCCESS);

    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::GetPreviousOutput
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::GetPreviousOutput(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    TUint output;
    TInt i = 0;
    TInt status(TMS_RESULT_SUCCESS);
    while (i < iTMSCallServList.Count())
        {
        TMSStartAndMonitorTMSCallThread* callThread = iTMSCallServList[i];

        if (callThread)
            {
            status = callThread->iTMSCallProxyLocal.ReceiveFromCallServer(
                    TMS_ROUTING_PREVIOUSOUTPUT_GET, output);
            if (status != TMS_RESULT_SUCCESS)
                {
                break;
                }
            }
        i++;
        }
    TPckgBuf<TInt> p(output);
    aMessage.Write(0, p);
    aMessage.Complete(TMS_RESULT_SUCCESS);

    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::GetAvailableOutputs
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::GetAvailableOutputs(const RMessage2& aMessage)
    {
    TRAPD(status, GetAvailableOutputsL(aMessage));
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::GetAvailableOutputsL
//
// -----------------------------------------------------------------------------
//
void TMSServer::GetAvailableOutputsL(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    TInt status = TMS_RESULT_SUCCESS;
    OutputVector outputs;
    HBufC8* buf = HBufC8::NewLC(KOutputsArraySize * sizeof(TUint32));
    TPtr8 ptr = buf->Des();
    TPckgBuf<TInt> countpkg;
    TIpcArgs args;
    args.Set(0, &countpkg);
    args.Set(1, &ptr);

    TInt i = 0;

    while (i < iTMSCallServList.Count())
        {
        TMSStartAndMonitorTMSCallThread* callThread = iTMSCallServList[i];

        if (callThread)
            {
            status = callThread->iTMSCallProxyLocal.ReceiveFromCallServer(
                    TMS_ROUTING_AVAILABLE_OUTPUT_GET, args);
            if (status != TMS_RESULT_SUCCESS)
                {
                break;
                }
            }
        i++;
        }

    aMessage.WriteL(0, countpkg);
    aMessage.WriteL(1, buf->Des());
    aMessage.Complete(status);

    CleanupStack::PopAndDestroy(buf);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::GetMaxLevel
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::GetMaxLevel(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    TInt status(TMS_RESULT_SUCCESS);
    TPckgBuf<TUint> pckg;

    pckg() = iEffectSettings->MaxVolume();
    if (status == TMS_RESULT_SUCCESS)
        {
        status = aMessage.Write(0, pckg);
        aMessage.Complete(status);
        }

    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::GetLevel
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::GetLevel(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    TInt status(TMS_RESULT_SUCCESS);
    TPckgBuf<TUint> pckg;
    TInt volume;

    switch (iCurrentRouting)
        {
        case TMS_AUDIO_OUTPUT_PUBLIC:
        case TMS_AUDIO_OUTPUT_LOUDSPEAKER:
            iEffectSettings->GetLoudSpkrVolume(volume);
            TRACE_PRN_N1(_L("TMSServer::GetLevel loudspkr vol %d"),volume);
            break;
        default:
            iEffectSettings->GetEarPieceVolume(volume);
            TRACE_PRN_N1(_L("TMSServer::GetLevel ear vol %d"),volume);
            break;
        }

    pckg() = volume;
    if (status == TMS_RESULT_SUCCESS)
        {
        aMessage.Write(0, pckg);
        aMessage.Complete(status);
        }

    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::SetLevel
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::SetLevel(CSession2* /*sid*/, TBool tmsclient, TInt level)
    {
    TRACE_PRN_FN_ENT;

    TInt status = SendMessageToCallServ(TMS_EFFECT_GLOBAL_VOL_SET, level);
    if (status == TMS_RESULT_SUCCESS)
        {
        switch (iCurrentRouting)
            {
            case TMS_AUDIO_OUTPUT_PUBLIC:
            case TMS_AUDIO_OUTPUT_LOUDSPEAKER:
                {
                if (tmsclient && iAudioCenRepHandler)
                    {
                    iAudioCenRepHandler->SetLoudSpeakerVol(level);
                    }
                iEffectSettings->SetLoudSpkrVolume(level);
                }
                break;
            case TMS_AUDIO_OUTPUT_NONE:
            case TMS_AUDIO_OUTPUT_PRIVATE:
            case TMS_AUDIO_OUTPUT_HANDSET:
            case TMS_AUDIO_OUTPUT_WIRED_ACCESSORY:
            case TMS_AUDIO_OUTPUT_ACCESSORY:
            case TMS_AUDIO_OUTPUT_ETTY:
                {
                if (tmsclient && iAudioCenRepHandler)
                    {
                    iAudioCenRepHandler->SetEarPieceVol(level);
                    }
                iEffectSettings->SetEarPieceVolume(level);
                }
                break;
            default:
                break;
            }

        iSessionIter.SetToFirst();
        TMSServerSession* ss = static_cast<TMSServerSession*> (iSessionIter++);
        while (ss != NULL)
            {
            ss->HandleGlobalEffectChange(TMS_EVENT_EFFECT_VOL_CHANGED, level,
                    EFalse, iCurrentRouting);
            ss = static_cast<TMSServerSession*> (iSessionIter++);
            }
        }

    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::GetMaxGain
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::GetMaxGain(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    TInt status(TMS_RESULT_SUCCESS);
    TPckgBuf<TUint> pckg;
    pckg() = iEffectSettings->MaxGain();
    if (status == TMS_RESULT_SUCCESS)
        {
        status = aMessage.Write(0, pckg);
        aMessage.Complete(status);
        }

    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::GetGain
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::GetGain(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    TInt status(TMS_RESULT_SUCCESS);
    TPckgBuf<TUint> pckg;
    pckg() = iEffectSettings->Gain();
    if (status == TMS_RESULT_SUCCESS)
        {
        status = aMessage.Write(0, pckg);
        aMessage.Complete(status);
        }

    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::SetGain
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::SetGain(CSession2* /*sid*/, TInt level)
    {
    TRACE_PRN_FN_ENT;

    TInt status = SendMessageToCallServ(TMS_EFFECT_GLOBAL_GAIN_SET, level);
    if (status == TMS_RESULT_SUCCESS)
        {
        iAudioCenRepHandler->SetMuteState(level);
        iEffectSettings->SetGain(level);
        iSessionIter.SetToFirst();
        TMSServerSession* ss = static_cast<TMSServerSession*> (iSessionIter++);
        while (ss != NULL)
            {
            ss->HandleGlobalEffectChange(TMS_EVENT_EFFECT_GAIN_CHANGED, level);
            ss = static_cast<TMSServerSession*> (iSessionIter++);
            }
        }

    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::GetSupportedCodecs
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::GetSupportedCodecs(const TMSStreamType strmType,
        RArray<TFourCC>*& aCodecs)
    {
    TRACE_PRN_FN_ENT;

    if (strmType == TMS_STREAM_UPLINK)
        {
        aCodecs = &iUplCodecs;
        }
    else if (strmType == TMS_STREAM_DOWNLINK)
        {
        aCodecs = &iDnlCodecs;
        }

    TRACE_PRN_FN_EXT;
    return TMS_RESULT_SUCCESS;
    }

// -----------------------------------------------------------------------------
// TMSServer::SendMessageToCallServ
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::SendMessageToCallServ(TInt func, TInt value)
    {
    TRACE_PRN_FN_ENT;

    TInt status(TMS_RESULT_SUCCESS);
    TInt i = 0;
    while (i < iTMSCallServList.Count())
        {
        TMSStartAndMonitorTMSCallThread* callThread = iTMSCallServList[i];

        if (callThread)
            {
            if (!callThread->IsActive())
                {
                iTMSCallServList.Remove(i);
                delete callThread;
                }
            else
                {
                status = callThread->iTMSCallProxyLocal.SendToCallServer(
                        func, value);
                if (status != TMS_RESULT_SUCCESS)
                    {
                    break;
                    }
                }
            }
        i++;
        }

    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::NotifyTarClients
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::NotifyTarClients(TRoutingMsgBufPckg routingpckg)
    {
    TRACE_PRN_FN_ENT;

    TInt vol;
    TInt status(TMS_RESULT_SUCCESS);
    FindActiveCallType();

    if (iActiveCallType == TMS_CALL_CS)
        {
        iCurrentRouting = routingpckg().iOutput;
        if (iCurrentRouting == TMS_AUDIO_OUTPUT_PUBLIC ||
                iCurrentRouting == TMS_AUDIO_OUTPUT_LOUDSPEAKER)
            {
            iEffectSettings->GetLoudSpkrVolume(vol);
            TRACE_PRN_N1(_L("TMSServer::NotifyTarClients loudspkr vol %d"),vol);
            }
        else
            {
            iEffectSettings->GetEarPieceVolume(vol);
            TRACE_PRN_N1(_L("TMSServer::NotifyTarClients ear vol %d"),vol);
            }

        status = SendMessageToCallServ(TMS_EFFECT_GLOBAL_VOL_SET, vol);
        }

    iSessionIter.SetToFirst();
    TMSServerSession* ss = static_cast<TMSServerSession*> (iSessionIter++);
    while (ss != NULL)
        {
        // Send only if there is a subscriber to TMS routing notifications.
        if (iTarHandlerCount > 1)
            {
            ss->HandleRoutingChange(routingpckg);
            }
        if (iActiveCallType == TMS_CALL_CS)
            {
            ss->HandleGlobalEffectChange(TMS_EVENT_EFFECT_VOL_CHANGED, vol,
                    ETrue, iCurrentRouting);
            }
        ss = static_cast<TMSServerSession*> (iSessionIter++);
        }

    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::InitDTMF
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::InitDTMF(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;
    TInt status(TMS_RESULT_SUCCESS);
    TMSStreamType strmtype;
    strmtype = (TMSStreamType) aMessage.Int0();

    if (strmtype == TMS_STREAM_UPLINK)
        {
        if (!iDTMFUplinkPlayerEtel) //CS call
            {
            // Uses ETel for uplink
            TRAP(status, iDTMFUplinkPlayerEtel = TMSDTMFProvider::NewL());
            if (iDTMFUplinkPlayerEtel && status == TMS_RESULT_SUCCESS)
                {
                iDTMFUplinkPlayerEtel->AddObserver(*this);
                }
            }
        if (!iDTMFUplinkPlayer) //IP call
            {
            TRAP(status, iDTMFUplinkPlayer = TMSAudioDtmfTonePlayer::NewL(*this,
                    KAudioDTMFString, KAudioPriorityDTMFString));
            }
        }
    else if (strmtype == TMS_STREAM_DOWNLINK)
        {
        if (!iDTMFDnlinkPlayer) //CS or IP call
            {
            TRAP(status, iDTMFDnlinkPlayer = TMSAudioDtmfTonePlayer::NewL(*this,
                    KAudioDTMFString, KAudioPriorityDTMFString));
            }
        }

    aMessage.Complete(status);
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::StartDTMF
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::StartDTMF(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    TInt status(TMS_RESULT_SUCCESS);
    TInt len(0);
    TMSStreamType strmtype;
    strmtype = (TMSStreamType) aMessage.Int0();
    len = aMessage.GetDesLength(1);
    if (len > 0)
        {
        HBufC* tone(NULL);
        TRAP(status, tone = HBufC::NewL(len));
        if (status == TMS_RESULT_SUCCESS)
            {
            TPtr ptr = tone->Des();
            status = aMessage.Read(1, ptr);
            TRACE_PRN_N(ptr);

            if (strmtype == TMS_STREAM_UPLINK)
                {
#ifdef __WINSCW__
                // Just to hear DTMF tones in Wins
                iActiveCallType = TMS_CALL_IP;
#else
                FindActiveCallType();
#endif
                if (iActiveCallType == TMS_CALL_IP && iDTMFUplinkPlayer)
                    {
                    iDTMFUplinkPlayer->PlayDtmfTone(ptr);
                    status = TMS_RESULT_SUCCESS;
                    }
                else if (iActiveCallType == TMS_CALL_CS &&
                        iDTMFUplinkPlayerEtel)
                    {
                    status = iDTMFUplinkPlayerEtel->SendDtmfToneString(ptr);
                    }
                else
                    {
                    status = TMS_RESULT_INVALID_STATE;
                    }
                NotifyDtmfClients(ECmdDTMFToneUplPlayStarted, status);
                }
            else if (strmtype == TMS_STREAM_DOWNLINK)
                {
                status = TMS_RESULT_INVALID_STATE;
                if (iDTMFDnlinkPlayer)
                    {
                    iDTMFDnlinkPlayer->PlayDtmfTone(ptr);
                    status = TMS_RESULT_SUCCESS;
                    }
                NotifyDtmfClients(ECmdDTMFToneDnlPlayStarted, status);
                }
            else
                {
                status = TMS_RESULT_STREAM_TYPE_NOT_SUPPORTED;
                }
            }
        delete tone;
        tone = NULL;
        }

    aMessage.Complete(status);
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::StopDTMF
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::StopDTMF(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    TInt status(TMS_RESULT_INVALID_STATE);
    TMSStreamType streamtype;
    streamtype = (TMSStreamType) aMessage.Int0();

    if (streamtype == TMS_STREAM_UPLINK)
        {
        if (iActiveCallType == TMS_CALL_IP && iDTMFUplinkPlayer)
            {
            iDTMFUplinkPlayer->Cancel();
            status = TMS_RESULT_SUCCESS;
            }
        else if (iActiveCallType == TMS_CALL_CS && iDTMFUplinkPlayerEtel)
            {
            status = iDTMFUplinkPlayerEtel->StopDtmfTone();
            status = TMSUtility::EtelToTMSResult(status);
            }
        }
    else if (streamtype == TMS_STREAM_DOWNLINK)
        {
        if (iDTMFDnlinkPlayer)
            {
            iDTMFDnlinkPlayer->Cancel();
            status = TMS_RESULT_SUCCESS;
            }
        }
    iActiveCallType = -1;
    NotifyDtmfClients(ECmdDTMFTonePlayFinished, status);
    aMessage.Complete(status);
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::ContinueSendingDTMF
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::ContinueSendingDTMF(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;
    TInt status(TMS_RESULT_INVALID_STATE);
    if (iActiveCallType == TMS_CALL_CS && iDTMFUplinkPlayerEtel)
        {
        TBool continuesnd = (TBool) aMessage.Int0();
        status = iDTMFUplinkPlayerEtel->ContinueDtmfStringSending(continuesnd);
        status = TMSUtility::EtelToTMSResult(status);
        }
    aMessage.Complete(status);
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::NotifyDtmfClients
//
// -----------------------------------------------------------------------------
//
TInt TMSServer::NotifyDtmfClients(gint aEventType, gint aError)
    {
    TRACE_PRN_FN_ENT;
    iSessionIter.SetToFirst();
    TMSServerSession* ss = static_cast<TMSServerSession*> (iSessionIter++);
    while (ss != NULL)
        {
        ss->NotifyClient(aEventType, aError);
        ss = static_cast<TMSServerSession*> (iSessionIter++);
        }
    TRACE_PRN_FN_EXT;
    return TMS_RESULT_SUCCESS;
    }

//From DTMFTonePlayerObserver
// -----------------------------------------------------------------------------
// TMSServer::DTMFInitCompleted
//
// -----------------------------------------------------------------------------
//
void TMSServer::DTMFInitCompleted(gint status)
    {
    TRACE_PRN_FN_ENT;
    if (status != TMS_RESULT_SUCCESS)
        {
        NotifyDtmfClients(ECmdDTMFTonePlayFinished, status);
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::DTMFToneFinished
//
// -----------------------------------------------------------------------------
//
void TMSServer::DTMFToneFinished(gint status)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(status);

    // KErrUnderflow indicates end of DTMF playback.
    if (status == KErrUnderflow /*|| status == KErrInUse*/)
        {
        status = TMS_RESULT_SUCCESS;
        }
    NotifyDtmfClients(ECmdDTMFTonePlayFinished, status);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::FindActiveCallType
//
// -----------------------------------------------------------------------------
//
gint TMSServer::FindActiveCallType()
    {
    TInt status(TMS_RESULT_INVALID_STATE);
    iActiveCallType = -1;
    TInt i = 0;
    while (i < iTMSCallServList.Count())
        {
        TMSStartAndMonitorTMSCallThread* callThread = iTMSCallServList[i];
        if (callThread)
            {
            TmsCallMsgBufPckg pckg;
            TIpcArgs args(&pckg);
            status = callThread->iTMSCallProxyLocal.ReceiveFromCallServer(
                    TMS_GET_ACTIVE_CALL_PARAMS, args);
            if (pckg().iBool || status != TMS_RESULT_SUCCESS)
                {
                iActiveCallType = static_cast<TMSCallType> (pckg().iInt);
                break;
                }
            }
        i++;
        }
    return status;
    }

// -----------------------------------------------------------------------------
// TMSServer::HandleDTMFEvent
//
// -----------------------------------------------------------------------------
//
void TMSServer::HandleDTMFEvent(const TMSDTMFObserver::TCCPDtmfEvent event,
        const gint status, const TChar /*tone*/)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_IF_ERR(status);
    gint cmd = ECmdDTMFTonePlayFinished;

    switch (event)
        {
        case ECCPDtmfUnknown:               //Unknown
            break;
        case ECCPDtmfManualStart:           //DTMF sending started manually
        case ECCPDtmfSequenceStart:         //Automatic DTMF sending initialized
            cmd = ECmdDTMFToneUplPlayStarted;
            break;
        case ECCPDtmfManualStop:            //DTMF sending stopped manually
        case ECCPDtmfManualAbort:           //DTMF sending aborted manually
        case ECCPDtmfSequenceStop:          //Automatic DTMF sending stopped
        case ECCPDtmfSequenceAbort:         //Automatic DTMF sending aborted
        case ECCPDtmfStopInDtmfString:      //There was stop mark in DTMF string
        case ECCPDtmfStringSendingCompleted://DTMF sending success
        default:
            break;
        }

    NotifyDtmfClients(cmd, status);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::InitRingTonePlayerL
//
// -----------------------------------------------------------------------------
//
void TMSServer::InitRingTonePlayerL()
    {
    TRACE_PRN_FN_ENT;

    DeinitRingTonePlayer();

    if (!iTMSRtPlayer)
        {
        iTMSRtPlayer = TMSRingTonePlayer::NewL(*this);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::DeinitRingTonePlayer
//
// -----------------------------------------------------------------------------
//
void TMSServer::DeinitRingTonePlayer()
    {
    TRACE_PRN_FN_ENT;

    if (iTMSRtPlayer)
        {
        iTMSRtPlayer->DeinitRingTonePlayer();
        }

    delete iRtFile;
    iRtFile = NULL;
    delete iRtSequence;
    iRtSequence = NULL;
    delete iTtsText;
    iTtsText = NULL;

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::OpenRingTonePlayerFromFileL
//
// -----------------------------------------------------------------------------
//
void TMSServer::OpenRingTonePlayerFromFileL(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    gint status(TMS_RESULT_INVALID_ARGUMENT);
    gint len = aMessage.GetDesLength(0);
    if (len > 0)
        {
        InitRingTonePlayerL();

        if (iTMSRtPlayer)
            {
            iRtFile = HBufC::NewL(len);
            TPtr ptr = iRtFile->Des();
            aMessage.ReadL(0, ptr);
            iTMSRtPlayer->CreateRingTonePlayerL(EPlayerAudio, *iRtFile);
            len = aMessage.GetDesLength(1);
            if (len > 0)
                {
                delete iTtsText;
                iTtsText = NULL;
                iTtsText = HBufC::NewL(len);
                TPtr ptr1 = iTtsText->Des();
                aMessage.ReadL(1, ptr1);
                iTMSRtPlayer->CreateTTSPlayerL(ptr1);
                }
            status = TMS_RESULT_SUCCESS;
            }
        }
    aMessage.Complete(status);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::OpenRingTonePlayerFromProfileL
//
// -----------------------------------------------------------------------------
//
void TMSServer::OpenRingTonePlayerFromProfileL(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    gint status(TMS_RESULT_INVALID_ARGUMENT);
    InitRingTonePlayerL();

    if (iTMSRtPlayer)
        {
        iTMSRtPlayer->CreateRingTonePlayerL(EPlayerDefault);
        gint len = aMessage.GetDesLength(1);
        if (len > 0)
            {
            delete iTtsText;
            iTtsText = NULL;
            iTtsText = HBufC::NewL(len);
            TPtr ptr = iTtsText->Des();
            aMessage.ReadL(1, ptr);
            iTMSRtPlayer->CreateTTSPlayerL(ptr);
            }
        status = TMS_RESULT_SUCCESS;
        }
    aMessage.Complete(status);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::OpenRingToneSequencePlayerL
//
// -----------------------------------------------------------------------------
//
void TMSServer::OpenRingToneSequencePlayerL(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    gint status(TMS_RESULT_INVALID_ARGUMENT);
    gint len = aMessage.GetDesLength(0);
    if (len > 0)
        {
        InitRingTonePlayerL();
        iRtSequence = HBufC8::NewL(len);
        TPtr8 ptr = iRtSequence->Des();
        aMessage.ReadL(0, ptr);

        if (iTMSRtPlayer)
            {
            iTMSRtPlayer->CreateCustomSequencePlayerL(ptr);
            status = TMS_RESULT_SUCCESS;
            }
        }
    aMessage.Complete(status);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::OpenRingToneBeepOnceL
//
// -----------------------------------------------------------------------------
//
void TMSServer::OpenRingToneBeepOnceL()
    {
    TRACE_PRN_FN_ENT;

    InitRingTonePlayerL();

    if (iTMSRtPlayer)
        {
        iTMSRtPlayer->CreateSequencePlayerL(EPlayerBeepOnce);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::DeinitRingTone
//
// -----------------------------------------------------------------------------
//
void TMSServer::OpenRingToneSilentL()
    {
    TRACE_PRN_FN_ENT;

    InitRingTonePlayerL();

    if (iTMSRtPlayer)
        {
        iTMSRtPlayer->CreateSequencePlayerL(EPlayerSilent);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::OpenRingToneUnsecureVoipL
//
// -----------------------------------------------------------------------------
//
void TMSServer::OpenRingToneUnsecureVoipL()
    {
    TRACE_PRN_FN_ENT;

    InitRingTonePlayerL();

    if (iTMSRtPlayer)
        {
        iTMSRtPlayer->CreateSequencePlayerL(EPlayerUnsecureVoIP);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::PlayRingToneL
//
// -----------------------------------------------------------------------------
//
void TMSServer::PlayRingToneL()
    {
    TRACE_PRN_FN_ENT;

    if (iTMSRtPlayer)
        {
        iTMSRtPlayer->PlayRingToneL();
        }
    else
        {
        User::Leave(TMS_RESULT_UNINITIALIZED_OBJECT);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::PauseVideoRingTone
//
// -----------------------------------------------------------------------------
//
void TMSServer::PauseVideoRingTone()
    {
    StopRingTone();
    }

// -----------------------------------------------------------------------------
// TMSServer::StopRingTone
//
// -----------------------------------------------------------------------------
//
void TMSServer::StopRingTone()
    {
    TRACE_PRN_FN_ENT;

    if (iTMSRtPlayer)
        {
        iTMSRtPlayer->StopPlaying();
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::MuteRingTone
//
// -----------------------------------------------------------------------------
//
void TMSServer::MuteRingTone()
    {
    TRACE_PRN_FN_ENT;

    if (iTMSRtPlayer)
        {
        iTMSRtPlayer->MuteRingTone();
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::RtPlayerEvent
//
// -----------------------------------------------------------------------------
//
void TMSServer::RtPlayerEvent(gint aEventType, gint aError)
    {
    TRACE_PRN_FN_ENT;

    iSessionIter.SetToFirst();
    TMSServerSession* ss = static_cast<TMSServerSession*> (iSessionIter++);

    while (ss != NULL)
        {
        ss->NotifyClient(aEventType, aError);
        ss = static_cast<TMSServerSession*> (iSessionIter++);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::HandleNotifyPSL
//
// -----------------------------------------------------------------------------
//
void TMSServer::HandleNotifyPSL(const TUid /*aUid*/, const TInt& /*aKey*/,
        const TRequestStatus& /*aStatus*/)
    {
    TRACE_PRN_FN_ENT;
    if (iEffectSettings)
        {
        iEffectSettings->ResetDefaultVolume();
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServer::RunServerL
//
// -----------------------------------------------------------------------------
//
void TMSServer::RunServerL()
    {
    TRACE_PRN_N(_L("TMS->RunServerL"));

    // Naming the server thread after the server helps to debug panics
    User::LeaveIfError(User::RenameThread(KTMSServerName));

    // Create and install the active scheduler we need
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);

    // Create the server (leave it on the cleanup stack)
    TMSServer* server = TMSServer::NewLC();

    // Initialisation complete, now signal the client
    RProcess::Rendezvous(TMS_RESULT_SUCCESS);

    // Start the scheduler and wait for client requests
    CActiveScheduler::Start();

    // Ready to exit.
    // Cleanup scheduler and delete the server
    CleanupStack::PopAndDestroy(server);
    CleanupStack::PopAndDestroy(scheduler);

    TRACE_PRN_N(_L("TMS->RunServerL - TMS server closed"));
    }

// -----------------------------------------------------------------------------
// TMSServer::TerminateServer
//
// -----------------------------------------------------------------------------
//
void TMSServer::TermSrv(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;

    // First, make sure request comes from a legitimate source

    // Level 1 - check process capability
    //TBool allow = aMessage.HasCapability(ECapabilityTCB);
    TBool allow = aMessage.HasCapability(ECapabilityPowerMgmt);

    if (allow)
        {
        // Level 2 - check Vendor ID
        _LIT_VENDOR_ID(AllowId, VID_DEFAULT);
        allow = (aMessage.VendorId() == AllowId) ? ETrue : EFalse;
        }
  /*if (allow)
        {
        // Level 3 - check SecureID of the process
        TUid uid = ALLOWED_UID;
        allow = ((static_cast<TUid> (aMessage.SecureId()) == uid)) ? ETrue : EFalse;
        }*/

    aMessage.Complete(TMS_RESULT_SUCCESS);

    if (allow)
        {
        TRACE_PRN_N(_L("TMS->Server going down!"));
        SendMessageToCallServ(TMS_TERM_CALL_SRV, 0);

        TMSServerSession *session;
        iSessionIter.SetToFirst();

        for (;;)
            {
            session = static_cast<TMSServerSession*> (iSessionIter++);
            if (!session)
                {
                break;
                }
            delete session;
            }
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSStartAndMonitorTMSCallThread::TMSStartAndMonitorTMSCallThread
// Perhaps we need to move this to a .cpp?
// -----------------------------------------------------------------------------
//
TMSStartAndMonitorTMSCallThread::TMSStartAndMonitorTMSCallThread(
        TMSServer* aServer) :
    CActive(EPriorityStandard),
    iTMSServer(aServer)
    {
    TRACE_PRN_FN_ENT;
    CActiveScheduler::Add(this);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSStartAndMonitorTMSCallThread::~TMSStartAndMonitorTMSCallThread
//
// -----------------------------------------------------------------------------
//
TMSStartAndMonitorTMSCallThread::~TMSStartAndMonitorTMSCallThread()
    {
    TRACE_PRN_FN_ENT;
    Cancel();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSStartAndMonitorTMSCallThread::NewL
//
// -----------------------------------------------------------------------------
//
TMSStartAndMonitorTMSCallThread* TMSStartAndMonitorTMSCallThread::NewL(
        TMSServer* aServer)
    {
    TRACE_PRN_FN_ENT;

    TMSStartAndMonitorTMSCallThread* self =
            new (ELeave) TMSStartAndMonitorTMSCallThread(aServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    TRACE_PRN_FN_EXT;
    return self;
    }

// -----------------------------------------------------------------------------
// TMSStartAndMonitorTMSCallThread::ConstructL
//
// -----------------------------------------------------------------------------
//
void TMSStartAndMonitorTMSCallThread::ConstructL()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSStartAndMonitorTMSCallThread::StartTMSCallServer
//
// -----------------------------------------------------------------------------
//
TInt TMSStartAndMonitorTMSCallThread::StartTMSCallServer(
        TMSCallProxyLocal& aHndl)
    {
    TRACE_PRN_FN_ENT;

    TInt status(TMS_RESULT_SUCCESS);
    TMSCallServerStartParam start(iTMSServer, iCallSrvrHndl);
    const TUidType serverUid(KNullUid, KNullUid, KUidTMSCallServerUid3);
    TThreadFunction serverFunc = TMSCallServer::StartThread;

    status = iServerThread.Create(_L(""), serverFunc, KTMSCallServerStackSize,
            KTMSCallServerInitHeapSize, KTMSCallServerMaxHeapSize, &start,
            EOwnerProcess);

    if (status != TMS_RESULT_SUCCESS)
        {
        return status;
        }

    //NOTE: set priority to EPriorityRealTime or speech codec audio will choke.
    iServerThread.SetPriority(EPriorityRealTime);

    // Synchronise with the server
    TRequestStatus reqStatus;
    iServerThread.Rendezvous(reqStatus);

    if (reqStatus != KRequestPending)
        {
        iServerThread.Kill(0);
        }
    else
        {
        // Start the test harness
        iServerThread.Resume();
        // Server will call the reciprocal static synchronise call
        }

    User::WaitForRequest(reqStatus); // wait for start or death
    if (reqStatus.Int() != TMS_RESULT_SUCCESS)
        {
        iServerThread.Close();
        iCallSrvrHndl.Close();
        return reqStatus.Int();
        }
    status = aHndl.Open(iCallSrvrHndl);

    if (status != TMS_RESULT_SUCCESS)
        {
        iServerThread.Close();
        iCallSrvrHndl.Close();
        return status;
        }
    aHndl.ShareProtected();
    iStatus = KRequestPending;
    iServerThread.Logon(iStatus);
    SetActive();

    TRACE_PRN_FN_EXT;
    return TMS_RESULT_SUCCESS;
    }

// -----------------------------------------------------------------------------
// TMSStartAndMonitorTMSCallThread::RunL
// From CActive
// -----------------------------------------------------------------------------
//
void TMSStartAndMonitorTMSCallThread::RunL()
    {
    TRACE_PRN_FN_ENT;
    iServerThread.Close();
    //NOTE: This is causing a panic when closing down TMS server.
    //iCallSrvrHndl.Close();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSStartAndMonitorTMSCallThread::DoCancel
// From CActive
// -----------------------------------------------------------------------------
//
void TMSStartAndMonitorTMSCallThread::DoCancel()
    {
    TRACE_PRN_FN_ENT;
    if (iServerThread.Handle())
        {
        iServerThread.LogonCancel(iStatus);
        User::WaitForRequest(iStatus);
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallProxyLocal::Open
// Perhaps we need to move this to a .cpp?
// -----------------------------------------------------------------------------
TInt TMSCallProxyLocal::Open(RServer2& aTMSCallServerHandle)
    {
    TRACE_PRN_FN_ENT;

    TInt status(KErrNotSupported);
    status = CreateSession(aTMSCallServerHandle, TVersion(
            KTMSCallServerMajorVersionNumber, KTMSCallServerMinorVersionNumber,
            KTMSCallServerBuildVersionNumber), -1, EIpcSession_GlobalSharable);
    TRACE_PRN_FN_EXT;
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallProxyLocal::SendToCallServer
//
// -----------------------------------------------------------------------------
//
TInt TMSCallProxyLocal::SendToCallServer(TInt aFunc, TUint value)
    {
    TInt status(TMS_RESULT_SUCCESS);
    status = SendReceive(aFunc, TIpcArgs(value));
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallProxyLocal::ReceiveFromCallServer
//
// -----------------------------------------------------------------------------
//
TInt TMSCallProxyLocal::ReceiveFromCallServer(TInt aFunc, TUint& value)
    {
    TInt status(TMS_RESULT_SUCCESS);
    TPckgBuf<TUint> pckg;
    TIpcArgs args(&pckg);
    status = SendReceive(aFunc, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        value = pckg();
        }
    return status;
    }

// -----------------------------------------------------------------------------
// TMSCallProxyLocal::ReceiveFromCallServer
//
// -----------------------------------------------------------------------------
//
TInt TMSCallProxyLocal::ReceiveFromCallServer(TInt aFunc, TIpcArgs args)
    {
    TInt status(TMS_RESULT_SUCCESS);
    status = SendReceive(aFunc, args);
    return status;
    }

// -----------------------------------------------------------------------------
// E32Main
// Entry point for the server
// -----------------------------------------------------------------------------
//
TInt E32Main()
    {
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    TInt r = KErrNoMemory;
    if (cleanup)
        {
        TRAP(r, TMSServer::RunServerL());
        delete cleanup;
        }
    __UHEAP_MARKEND;
    return r;
    }

