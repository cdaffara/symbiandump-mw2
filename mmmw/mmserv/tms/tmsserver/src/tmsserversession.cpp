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

#include "tmsutility.h"
#include "tmsclientserver.h"
#include "tmsshared.h"
#include "tmsaudioinbandtoneplayer.h"
#include "tmsrtplayer.h"
#include "tmsserversession.h"

using namespace TMS;

const TUint KArrayExpandSize = 8;

// -----------------------------------------------------------------------------
// TMSServerSession::NewL
// Symbian constructor.
// -----------------------------------------------------------------------------
//
TMSServerSession* TMSServerSession::NewL(TMSServer &aServer)
    {
    TMSServerSession* self = new (ELeave) TMSServerSession(aServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// TMSServerSession::~TMSServerSession
// The destructor
// -----------------------------------------------------------------------------
//
TMSServerSession::~TMSServerSession()
    {
    TRACE_PRN_FN_ENT;

    iServer.DropSession(); // will start shutdown if no more sessions left
    delete iDevSound;
    delete iInbandTonePlayer;

    if (iMsgQueue.Handle() > 0)
        {
        iMsgQueue.Close();
        }

    if (iTMSCallSessionHandle.Handle() > 0)
        {
        iTMSCallSessionHandle.Close();
        }

    // Release memory and close handles to unused plug-ins held by the DevSound
    REComSession::FinalClose();

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServerSession::ConstructL
// 2-nd phase constructor.
// -----------------------------------------------------------------------------
//
void TMSServerSession::ConstructL()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServerSession::TMSServerSession
// -----------------------------------------------------------------------------
//
TMSServerSession::TMSServerSession(TMSServer &aServer) :
    iServer(aServer)
    {
    TRACE_PRN_FN_ENT;
    iServer.AddSession();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServerSession::ServiceL
// Service request handler.
// -----------------------------------------------------------------------------
//
void TMSServerSession::ServiceL(const RMessage2& aMessage)
    {
    TRAPD(err, DispatchMessageL(aMessage));

    if (err != TMS_RESULT_SUCCESS)
        {
        aMessage.Complete(err);
        }
    }

// -----------------------------------------------------------------------------
// TMSServerSession::DispatchMessageL
// Service request handler.
// -----------------------------------------------------------------------------
//
void TMSServerSession::DispatchMessageL(const RMessage2& aMessage)
    {
    switch (aMessage.Function())
        {
        case ETMSCallSessionHandle:
            GetTMSCallSessionHandleL(aMessage);
            break;
        case ETMSGetSupportedEncodersCount:
            GetCodecsCountL(aMessage, TMS_STREAM_UPLINK);
            break;
        case ETMSGetSupportedDecodersCount:
            GetCodecsCountL(aMessage, TMS_STREAM_DOWNLINK);
            break;
        case ETMSGetSupportedEncoders:
            GetSupportedCodecsL(aMessage, TMS_STREAM_UPLINK);
            break;
        case ETMSGetSupportedDecoders:
            GetSupportedCodecsL(aMessage, TMS_STREAM_DOWNLINK);
            break;
        case ETMSSetOutput:
            SetOutput(aMessage);
            break;
        case ETMSGetOutput:
            iServer.GetOutput(aMessage);
            break;
        case ETMSGetPreviousOutput:
            iServer.GetPreviousOutput(aMessage);
            break;
        case ETMSGetAvailableOutputs:
            iServer.GetAvailableOutputs(aMessage);
            break;
        case ETMSStartRoutingNotifier:
            iServer.StartRoutingNotifierL();
            aMessage.Complete(TMS_RESULT_SUCCESS);
            break;
        case ETMSCancelRoutingNotifier:
            iServer.CancelRoutingNotifier();
            aMessage.Complete(TMS_RESULT_SUCCESS);
            break;
        case ETMSStartGlobalEffectNotifier:
            iServer.StartCenRepHandlerL();
            aMessage.Complete(TMS_RESULT_SUCCESS);
            break;
        case ETMSCancelGlobalEffectNotifier:
            iServer.CancelCenRepHandler();
            aMessage.Complete(TMS_RESULT_SUCCESS);
            break;
        case ETMSSetMsgQueueHandle:
            SetMessageQueueHandleL(aMessage);
            break;
        case ETMSSetGlobalVol:
            SetVolLevel(aMessage);
            break;
        case ETMSGetGlobalVol:
            iServer.GetLevel(aMessage);
            break;
        case ETMSGetMaxGlobalVol:
            iServer.GetMaxLevel(aMessage);
            break;
        case ETMSSetGlobalGain:
            SetMicGain(aMessage);
            break;
        case ETMSGetGlobalGain:
            iServer.GetGain(aMessage);
            break;
        case ETMSGetMaxGlobalGain:
            iServer.GetMaxGain(aMessage);
            break;
        case ETMSRingToneInitDefault:
            iHasRtPlayer = ETrue;
            iServer.OpenRingTonePlayerFromProfileL(aMessage);
            break;
        case ETMSRingToneInitFile:
            iHasRtPlayer = ETrue;
            iServer.OpenRingTonePlayerFromFileL(aMessage);
            break;
        case ETMSRingToneInitSequence:
            iHasRtPlayer = ETrue;
            iServer.OpenRingToneSequencePlayerL(aMessage);
            break;
        case ETMSRingToneInitBeepOnce:
            iHasRtPlayer = ETrue;
            iServer.OpenRingToneBeepOnceL();
            aMessage.Complete(TMS_RESULT_SUCCESS);
            break;
        case ETMSRingToneInitSilent:
            iHasRtPlayer = ETrue;
            iServer.OpenRingToneSilentL();
            aMessage.Complete(TMS_RESULT_SUCCESS);
            break;
        case ETMSRingToneInitUnsecureVoIP:
            iHasRtPlayer = ETrue;
            iServer.OpenRingToneUnsecureVoipL();
            aMessage.Complete(TMS_RESULT_SUCCESS);
            break;
        case ETMSRingToneDeinit:
            iServer.DeinitRingTonePlayer();
            NotifyClient(ECmdRingToneDeinitComplete, KErrNone);
            iHasRtPlayer = EFalse;
            aMessage.Complete(TMS_RESULT_SUCCESS);
            break;
        case ETMSRingTonePlay:
            iHasRtPlayer = ETrue; //will play w/o prior initialization
            iServer.PlayRingToneL();
            aMessage.Complete(TMS_RESULT_SUCCESS);
            break;
        case ETMSRingTonePause:
            iServer.PauseVideoRingTone();
            aMessage.Complete(TMS_RESULT_SUCCESS);
            break;
        case ETMSRingToneStop:
            iServer.StopRingTone();
            aMessage.Complete(TMS_RESULT_SUCCESS);
            break;
        case ETMSRingToneMute:
            iServer.MuteRingTone();
            aMessage.Complete(TMS_RESULT_SUCCESS);
            break;
        case ETMSInitDTMF:
            iServer.InitDTMF(aMessage);
            break;
        case ETMSStartDTMF:
            iServer.StartDTMF(aMessage);
            break;
        case ETMSStopDTMF:
            iServer.StopDTMF(aMessage);
            break;
        case ETMSContinueDTMF:
            iServer.ContinueSendingDTMF(aMessage);
            break;
        case ETMSStartInbandTone:
            StartInbandTone(aMessage);
            break;
        case ETMSStopInbandTone:
            StopInbandTone(aMessage);
            break;
        case ETMSTermSrv:
            iServer.TermSrv(aMessage);
            break;
        default:
            User::Leave(KErrNotSupported);
            break;
        }
    }

// -----------------------------------------------------------------------------
// TMSServerSession::SetMessageQueueHandleL
//
// Open message queue for handling server notifications back to the client.
// -----------------------------------------------------------------------------
//
void TMSServerSession::SetMessageQueueHandleL(const RMessage2& aMessage)
    {
    gint status = TMS_RESULT_SUCCESS;
    if (iMsgQueue.Handle() <= 0)
        {
        status = iMsgQueue.Open(aMessage, 0);
        }
    if (status != TMS_RESULT_SUCCESS)
        {
        User::Leave(KErrArgument);
        }
    aMessage.Complete(TMS_RESULT_SUCCESS);
    }

// -----------------------------------------------------------------------------
// TMSServerSession::SetVolLevel
// -----------------------------------------------------------------------------
//
void TMSServerSession::SetVolLevel(const RMessage2& aMessage)
    {
    TInt level = aMessage.Int0();
    iServer.SetLevel(this, TRUE, level);
    aMessage.Complete(TMS_RESULT_SUCCESS);
    }

// -----------------------------------------------------------------------------
// TMSServerSession::SetMicGain
// -----------------------------------------------------------------------------
//
void TMSServerSession::SetMicGain(const RMessage2& aMessage)
    {
    TInt gain = aMessage.Int0();
    iServer.SetGain(this, gain);
    aMessage.Complete(TMS_RESULT_SUCCESS);
    }

// -----------------------------------------------------------------------------
// TMSServerSession::HandleGlobalEffectChange
// -----------------------------------------------------------------------------
//
void TMSServerSession::HandleGlobalEffectChange(TInt globalevent, TInt level,
        TBool output_change, TMSAudioOutput output)
    {
    TRACE_PRN_FN_ENT;
    iMsgBuffer.iInt = globalevent;
    iMsgBuffer.iUint = (guint)level;
    iMsgBuffer.iInt2 = (gint)output;
    iMsgBuffer.iBool = output_change;
    NotifyClient(ECmdGlobalEffectChange);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServerSession::HandleRoutingChange
// -----------------------------------------------------------------------------
//
void TMSServerSession::HandleRoutingChange(TRoutingMsgBufPckg routinginfo)
    {
    TRACE_PRN_FN_ENT;
    iMsgBuffer.iInt = routinginfo().iEvent;
    iMsgBuffer.iUint = routinginfo().iOutput;
    NotifyClient(ECmdGlobalRoutingChange);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServerSession::GetTMSCallSessionHandleL
// -----------------------------------------------------------------------------
//
void TMSServerSession::GetTMSCallSessionHandleL(const RMessage2& aMessage)
    {
    TRACE_PRN_FN_ENT;
    if (iTMSCallSessionHandle.Handle() == 0)
        {
        iServer.GetNewTMSCallSessionHandleL(iTMSCallSessionHandle);
        }
    aMessage.Complete(iTMSCallSessionHandle);
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServerSession::GetCodecsCountL
//
// Note: Currently, no data persists as a new session is created each time
//       before calling this method.
// -----------------------------------------------------------------------------
//
void TMSServerSession::GetCodecsCountL(const RMessage2& aMessage,
        TMSStreamType strmType)
    {
    TInt err = TMS_RESULT_SUCCESS;
    TInt codecsCount = 0;

    RArray<TFourCC>* codecs;
    iServer.GetSupportedCodecs(strmType, codecs);

    if (codecs)
        {
        codecsCount = codecs->Count();
        }

    if (codecs && codecsCount <= 0)
        {
        delete iDevSound;
        iDevSound = NULL;
        iDevSound = CMMFDevSound::NewL();

        if (iDevSound)
            {
            codecs->Reset();
            TMMFPrioritySettings priority;

            if (strmType == TMS_STREAM_UPLINK)
                {
                iDevSound->GetSupportedInputDataTypesL(*codecs, priority);
                }
            else if (strmType == TMS_STREAM_DOWNLINK)
                {
                iDevSound->GetSupportedOutputDataTypesL(*codecs, priority);
                }
            else
                {
                err = KErrNotSupported;
                }

            codecsCount = codecs->Count();
            }

        delete iDevSound;
        iDevSound = NULL;
        }

    TmsMsgBufPckg p;
    p().iInt = codecsCount;
#ifndef __WINSCW__
    p().iUint = 0; // G711 frame size undetermined
    //TODO: query G.711 codec to find its correct frame size (10 or 20 ms)
#else
    p().iUint = KTMSG711FrameLen20ms;
#endif //__WINSCW__
    p().iStatus = err;
    aMessage.WriteL(0, p);
    aMessage.Complete(TMS_RESULT_SUCCESS);
    }

// -----------------------------------------------------------------------------
// TMSServerSession::GetSupportedEncodersL
//
// -----------------------------------------------------------------------------
//
void TMSServerSession::GetSupportedCodecsL(const RMessage2& aMessage,
        TMSStreamType strmType)
    {
    TRACE_PRN_FN_ENT;

    CBufFlat* dataCopyBuffer = CBufFlat::NewL(KArrayExpandSize);
    CleanupStack::PushL(dataCopyBuffer);
    RBufWriteStream stream;
    stream.Open(*dataCopyBuffer);
    CleanupClosePushL(stream);

    RArray<TFourCC>* codecs;
    iServer.GetSupportedCodecs(strmType, codecs);
    TInt codecsCount = 0;
    codecsCount = codecs->Count();

    for (TInt i = 0; i < codecsCount; i++)
        {
        TFourCC fcc = (codecs->Array())[i];
        stream.WriteUint32L(fcc.FourCC());
        TRACE_PRN_N2(_L("TMS->Codec[%d]==[0x%x]"), i+1, fcc.FourCC());
        }

    aMessage.WriteL(0, dataCopyBuffer->Ptr(0));
    CleanupStack::PopAndDestroy(&stream);
    CleanupStack::PopAndDestroy(dataCopyBuffer);
    aMessage.Complete(TMS_RESULT_SUCCESS);

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSServerSession::SetOutput
// -----------------------------------------------------------------------------
//
void TMSServerSession::SetOutput(const RMessage2& aMessage)
    {
    TInt output = aMessage.Int0();
    iServer.SetOutput(this, output);
    aMessage.Complete(TMS_RESULT_SUCCESS);
    }

// -----------------------------------------------------------------------------
// TMSServerSession::StartInbandTone
//
// -----------------------------------------------------------------------------
//
void TMSServerSession::StartInbandTone(const RMessage2& aMessage)
    {
    if (!iInbandTonePlayer)
        {
        //TODO: Add inband tone observer
        iInbandTonePlayer = TMSAudioInbandTonePlayer::NewL();
        }

    if (iInbandTonePlayer)
        {
        TMSInbandToneType tonetype = (TMSInbandToneType) aMessage.Int0();
        iInbandTonePlayer->PlayInbandTone(tonetype);
        }

    //TODO: Move to inband tone observer callback
    NotifyClient(ECmdInbandToneStarted);
    aMessage.Complete(TMS_RESULT_SUCCESS);
    }

// -----------------------------------------------------------------------------
// TMSServerSession::StopInbandTone
//
// -----------------------------------------------------------------------------
//
void TMSServerSession::StopInbandTone(const RMessage2& aMessage)
    {
    if (iInbandTonePlayer)
        {
        iInbandTonePlayer->Cancel();
        }

    //TODO: Move to inband tone observer callback
    NotifyClient(ECmdInbandToneStopped);
    aMessage.Complete(TMS_RESULT_SUCCESS);
    }

// -----------------------------------------------------------------------------
// TMSServerSession::NotifyClient
// -----------------------------------------------------------------------------
//
void TMSServerSession::NotifyClient(const TInt aCommand, const TInt aStatus)
    {
    if (aCommand == ECmdRingToneOpenComplete ||
            aCommand == ECmdRingToneOpenComplete)
        {
        if (!iHasRtPlayer)
            {
            return;
            }
        }

    iMsgBuffer.iRequest = aCommand;
    iMsgBuffer.iStatus = aStatus;
    if (iMsgQueue.Handle() > 0)
        {
        iMsgQueue.Send(iMsgBuffer);
        }
    }

