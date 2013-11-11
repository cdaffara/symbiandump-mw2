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

#include <s32mem.h>
#include <gstring.h>
#include <AudioPreference.h>
#include <tmseffectobsrvr.h>
#include <tmsglobalroutingobsrvr.h>
#include <tmsringtoneobsrvr.h>
#include <tmsdtmfobsrvr.h>
#include <tmsinbandtoneobsrvr.h>
#include "tmsutility.h"
#include "tmsclientserver.h"
#include "tmsproxy.h"

using namespace TMS;

// CONSTANTS
const guint KTMSServerConnectRetries = 2;
const guint KSessionMessageSlots = 10;
const guint KUTF8Multiply = 2;

// -----------------------------------------------------------------------------
// TMSProxy::StartServer
//
// Function that will launch TMS server executable in it its own process.
// Start the server process/thread, which lives in an EPOCEXE object.
// Returns: gint: TMS_RESULT_SUCCESS (0) if no error.
// -----------------------------------------------------------------------------
//
gint TMSProxy::StartServer()
    {
    const TUidType serverUid(KNullUid, KNullUid, KTMSServerUid3);

    // Only one instance of the server is allowed. Attempt of launching
    // second instance of the server will fail with KErrAlreadyExists.
    RProcess server;
    gint ret = server.Create(KTMSServerFile, KNullDesC, serverUid);

    if (ret == TMS_RESULT_SUCCESS)
        {
        TRequestStatus stat;
        server.Rendezvous(stat);

        if (stat != KRequestPending)
            {
            server.Kill(0); // abort startup
            }
        else
            {
            server.Resume(); // logon OK - start the server
            }

        User::WaitForRequest(stat); // wait for start or death

        // We can't use the 'exit reason' if the server panicked, as '0' is a
        // valid panic 'reason', which cannot be distinguished from
        // TMS_RESULT_SUCCESS.
        ret = (server.ExitType() == EExitPanic) ? KErrGeneral : stat.Int();
        server.Close();
        }

    return ret;
    }

// -----------------------------------------------------------------------------
// TMSProxy::TMSProxy
// -----------------------------------------------------------------------------
//
EXPORT_C TMSProxy::TMSProxy()
    {
    ResetObjectLists();
    iMsgQHandler = NULL;
    }

// -----------------------------------------------------------------------------
// TMSProxy::~TMSProxy
// -----------------------------------------------------------------------------
//
EXPORT_C TMSProxy::~TMSProxy()
    {
    ResetObjectLists();

    if (iMsgQHandler)
        {
        iMsgQHandler->Cancel();
        iMsgQHandler->RemoveObserver(*this);
        }
    delete iMsgQHandler;
    if (iMsgQueue.Handle() > 0)
        {
        iMsgQueue.Close();
        }
    if (Handle() > 0)
        {
        Close();
        }
    }

// -----------------------------------------------------------------------------
// TMSProxy::Connect
// Create a client-side session. Start the server if not started already.
// -----------------------------------------------------------------------------
//
EXPORT_C gint TMSProxy::Connect()
    {
    TRACE_PRN_FN_ENT;

    guint retry = KTMSServerConnectRetries;
    gint err(TMS_RESULT_GENERAL_ERROR);

    for (;;)
        {
        // Try to create a new session with the server
        err = CreateSession(KTMSServerName, Version(), KSessionMessageSlots);

        if ((err != KErrNotFound) && (err != KErrServerTerminated))
            {
            TRACE_PRN_N1(_L("[TMS session created; err==%d]"), err);
            break; // Connected to existing server - ok
            }

        if (--retry == 0)
            {
            break; // Failed.
            }

        // Server not running, try to start it.
        err = StartServer();
        TRACE_PRN_N1(_L("[TMS server started; err==%d]"), err);

        if ((err != TMS_RESULT_SUCCESS) && (err != KErrAlreadyExists))
            {
            break; // Server not launched - propagate error
            }
        }

    TRACE_PRN_IF_ERR(err);

    if (err == KErrNone)
        {
        err = CreateQueue(KTMSMsgQSlots);
        }

    TRACE_PRN_FN_EXT;
    return TMSRESULT(err);
    }

// -----------------------------------------------------------------------------
// TMSProxy::Version
// -----------------------------------------------------------------------------
//
EXPORT_C TVersion TMSProxy::Version() const
    {
    return (TVersion(KTMSServMajorVersionNumber, KTMSServMinorVersionNumber,
            KTMSServBuildVersionNumber));
    }

// -----------------------------------------------------------------------------
// TMSProxy::Close
// -----------------------------------------------------------------------------
//
EXPORT_C void TMSProxy::Close()
    {
    TRACE_PRN_FN_ENT;
    RSessionBase::Close();
    TRACE_PRN_FN_EXT;
    }

EXPORT_C gint TMSProxy::GetTMSCallSessionHandle()
    {
    gint handle = SendReceive(ETMSCallSessionHandle);
    return handle;
    }

EXPORT_C gint TMSProxy::GetSupportedDecoders(RArray<TUint32>& aDecoders,
        gint& aFrameSize)
    {
    TRACE_PRN_FN_ENT;
    gint err(TMS_RESULT_SUCCESS);

    TmsMsgBufPckg pckg1;
    TIpcArgs args1(&pckg1);
    err = SendReceive(ETMSGetSupportedDecodersCount, args1);
    gint count = 0;
    aFrameSize = 0;

    if (err == TMS_RESULT_SUCCESS)
        {
        count = pckg1().iInt;
        aFrameSize = pckg1().iUint; //for g711 10/20ms frame detection
        err = pckg1().iStatus;
        }

    if (count > 0 && err == TMS_RESULT_SUCCESS)
        {
        TRAP(err, PopulateArrayL(ETMSGetSupportedDecoders, aDecoders, count));
        }

    TRACE_PRN_FN_EXT;
    return TMSRESULT(err);
    }

EXPORT_C gint TMSProxy::GetSupportedEncoders(RArray<TUint32>& aEncoders,
        gint& aFrameSize)
    {
    TRACE_PRN_FN_ENT;
    gint err(TMS_RESULT_SUCCESS);

    TmsMsgBufPckg pckg1;
    TIpcArgs args1(&pckg1);
    err = SendReceive(ETMSGetSupportedEncodersCount, args1);
    gint count = 0;
    aFrameSize = 0;

    if (err == TMS_RESULT_SUCCESS)
        {
        count = pckg1().iInt;
        aFrameSize = pckg1().iUint; //for g711 10/20ms frame detection
        err = pckg1().iStatus;
        }

    if (count > 0 && err == TMS_RESULT_SUCCESS)
        {
        TRAP(err, PopulateArrayL(ETMSGetSupportedEncoders, aEncoders, count));
        }

    TRACE_PRN_FN_EXT;
    return TMSRESULT(err);
    }

EXPORT_C gint TMSProxy::SetOutput(TMSAudioOutput output)
    {
    TRACE_PRN_FN_ENT;
    gint status = TMS_RESULT_SUCCESS;
    if (output == TMS_AUDIO_OUTPUT_NONE)
        {
        status = TMS_RESULT_INVALID_ARGUMENT;
        }
    else
        {
        status = SendReceive(ETMSSetOutput, TIpcArgs(output));
        }
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::GetOutput(TMSAudioOutput& output)
    {
    TRACE_PRN_FN_ENT;
    TPckgBuf<gint> pckg;
    TIpcArgs args(&pckg);
    gint status = SendReceive(ETMSGetOutput, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        output = pckg();
        }
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::GetPreviousOutput(TMSAudioOutput& output)
    {
    TRACE_PRN_FN_ENT;
    TPckgBuf<guint> pckg;
    TIpcArgs args(&pckg);
    gint status = SendReceive(ETMSGetPreviousOutput, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        output = pckg();
        }
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::GetAvailableOutputs(OutputVector& outputs)
    {
    gint status(TMS_RESULT_DOES_NOT_EXIST);
    TRAP(status, GetAvailableOutputsL(outputs));
    return TMSRESULT(status);
    }

void TMSProxy::GetAvailableOutputsL(OutputVector& outputs)
    {
    HBufC8* buf = HBufC8::NewLC(10 * sizeof(TUint32));
    TPtr8 ptr = buf->Des();
    TPckgBuf<gint> countpkg;
    TIpcArgs args;
    args.Set(0, &countpkg);
    args.Set(1, &ptr);
    gint status = SendReceive(ETMSGetAvailableOutputs, args);
    outputs.clear();
    if (status != TMS_RESULT_SUCCESS)
        {
        User::Leave(status);
        }
    else
        {
        RDesReadStream stream(ptr);
        CleanupClosePushL(stream);

        for (gint i = 0; i < countpkg(); i++)
            {
            TMSAudioOutput output = stream.ReadUint32L();
            if (output != TMS_AUDIO_OUTPUT_NONE)
                {
                outputs.push_back(output);
                }
            //TRACE_PRN_N1(_L("TMS->TMSProxy: outputs: [%d]"), stream.ReadUint32L());
            }

        CleanupStack::PopAndDestroy(&stream);
        }
    CleanupStack::PopAndDestroy(buf);
    }

EXPORT_C gint TMSProxy::GetLevel(guint& level)
    {
    TRACE_PRN_FN_ENT;
    TPckgBuf<guint> pckg;
    TIpcArgs args(&pckg);
    gint status = SendReceive(ETMSGetGlobalVol, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        level = pckg();
        }
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::GetMaxLevel(guint& level)
    {
    TRACE_PRN_FN_ENT;
    TPckgBuf<guint> pckg;
    TIpcArgs args(&pckg);
    gint status = SendReceive(ETMSGetMaxGlobalVol, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        level = pckg();
        }
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::SetLevel(guint level)
    {
    TRACE_PRN_FN_ENT;
    gint status = SendReceive(ETMSSetGlobalVol, TIpcArgs(level));
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::GetGain(guint& level)
    {
    TRACE_PRN_FN_ENT;
    TPckgBuf<guint> pckg;
    TIpcArgs args(&pckg);
    gint status = SendReceive(ETMSGetGlobalGain, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        level = pckg();
        }
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::GetMaxGain(guint& level)
    {
    TRACE_PRN_FN_ENT;
    TPckgBuf<guint> pckg;
    TIpcArgs args(&pckg);
    gint status = SendReceive(ETMSGetMaxGlobalGain, args);
    if (status == TMS_RESULT_SUCCESS)
        {
        level = pckg();
        }
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::SetGain(guint level)
    {
    TRACE_PRN_FN_ENT;
    gint status = SendReceive(ETMSSetGlobalGain,
            TIpcArgs(level));
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::InitRT(const TMSRingToneType type, GString* str,
        GString* tts)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_INVALID_ARGUMENT);
    TIpcArgs args;
    HBufC* ttsBuf(NULL);

    if (tts)
        {
        // Convert buffer from UTF-8 to unicode (16-bit)
        // Note: UTF-8 strings can take up to 4 bytes per character
        guint unilen = tts->len / KUTF8Multiply;
        TRAP(status, ttsBuf = HBufC::NewL(unilen));
        if (status == KErrNone)
            {
            TPtr p = ttsBuf->Des();
            p.Copy((TUint16*) tts->str, unilen);
            args.Set(1, &p);
            }
        }

    switch (type)
        {
        case TMS_RINGTONE_DEFAULT:
            {
            status = SendReceive(ETMSRingToneInitDefault, args);
            break;
            }
        case TMS_RINGTONE_FILE:
            {
            if (str)
                {
                HBufC* buf(NULL);
                // Convert buffer from UTF-8 to unicode (16-bit)
                // Note: UTF-8 strings can take up to 4 bytes per character
                guint unilen = str->len / KUTF8Multiply;

                TRAP(status, buf = HBufC::NewL(unilen));
                if (buf && status == KErrNone)
                    {
                    TPtr p = buf->Des();
                    p.Copy((TUint16*) str->str, unilen);
                    TIpcArgs args;
                    args.Set(0, &p);
                    status = SendReceive(ETMSRingToneInitFile, args);
                    }
                delete buf;
                buf = NULL;
                }
            break;
            }
        case TMS_RINGTONE_BEEP_ONCE:
            {
            status = SendReceive(ETMSRingToneInitBeepOnce);
            break;
            }
        case TMS_RINGTONE_SILENT:
            {
            status = SendReceive(ETMSRingToneInitSilent);
            break;
            }
        case TMS_RINGTONE_UNSECURE_VOIP:
            {
            status = SendReceive(ETMSRingToneInitUnsecureVoIP);
            break;
            }
        case TMS_RINGTONE_SEQUENCE:
            {
            if (str)
                {
                HBufC8* buf(NULL);
                gint len = str->len;
                TRAP(status, buf = HBufC8::NewL(len));
                if (buf && status == KErrNone)
                    {
                    TPtr8 p = buf->Des();
                    p.Copy((TUint8*) str->str, len);
                    TIpcArgs args;
                    args.Set(0, &p);
                    status = SendReceive(ETMSRingToneInitSequence, args);
                    }
                delete buf;
                buf = NULL;
                }
            break;
            }
        default:
            break;
        }

    delete ttsBuf;
    ttsBuf = NULL;

    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::DeinitRT()
    {
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(ETMSRingToneDeinit);
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::PlayRT()
    {
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(ETMSRingTonePlay);
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::StopRT()
    {
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(ETMSRingToneStop);
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::PauseRT()
    {
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(ETMSRingTonePause);
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::MuteRT()
    {
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(ETMSRingToneMute);
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::InitDTMFPlayer(TMSStreamType streamtype)
    {
    gint status(TMS_RESULT_SUCCESS);
    TIpcArgs args;
    args.Set(0, streamtype);
    status = SendReceive(ETMSInitDTMF, args);
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::StartDTMF(TMSStreamType streamtype, GString* tone)
    {
    TRACE_PRN_FN_ENT;
    __ASSERT_ALWAYS(tone, PANIC(TMS_RESULT_NULL_ARGUMENT));

    gint status(TMS_RESULT_SUCCESS);
    HBufC* buf(NULL);
    TRAP(status, buf = HBufC::NewL(tone->len));
    if (status == KErrNone)
        {
        TPtr p1 = buf->Des();
        TPtr8 p2((TUint8*) tone->str, tone->len, tone->len);
        p1.Copy(p2);

        TRACE_PRN_N(p1);

        TIpcArgs args;
        args.Set(0, streamtype);
        args.Set(1, &p1);
        status = SendReceive(ETMSStartDTMF, args);
        }
    delete buf;
    buf = NULL;
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::StopDTMF(TMSStreamType streamtype)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(ETMSStopDTMF, TIpcArgs(streamtype));
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::ContinueDTMFStringSending(gboolean continuesending)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(ETMSContinueDTMF, TIpcArgs(continuesending));
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::StartInbandTone(TMSInbandToneType inbandtonetype)
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(ETMSStartInbandTone, TIpcArgs(inbandtonetype));
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::StopInbandTone()
    {
    TRACE_PRN_FN_ENT;
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(ETMSStopInbandTone);
    TRACE_PRN_FN_EXT;
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::StartGlobalEffectNotifier()
    {
    gint status(TMS_RESULT_SUCCESS);
    SendReceive(ETMSStartGlobalEffectNotifier); //CenRepHandler
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::CancelGlobalEffectNotifier()
    {
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(ETMSCancelGlobalEffectNotifier);
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::StartRoutingNotifier()
    {
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(ETMSStartRoutingNotifier); //TAR
    return TMSRESULT(status);
    }

EXPORT_C gint TMSProxy::CancelRoutingNotifier()
    {
    gint status(TMS_RESULT_SUCCESS);
    status = SendReceive(ETMSCancelRoutingNotifier);
    return TMSRESULT(status);
    }

// ---------------------------------------------------------------------------
// TMSProxy::SetMsgQueueNotifier
// ---------------------------------------------------------------------------
//
EXPORT_C gint TMSProxy::SetMsgQueueNotifier(TMSMsgQueueNotifierType type,
        gpointer obsrv, gpointer parent, gint clientid)
    {
    gint status(TMS_RESULT_SUCCESS);
    switch (type)
        {
        case EMsgQueueGlobalVolumeType:
        case EMsgQueueGlobalGainType:
            status = AddGlobalEffectObserver(
                    *(static_cast<TMSEffectObserver*>(obsrv)),
                    *(static_cast<TMSEffect*>(parent)), clientid);
			break;
        case EMsgQueueGlobalRoutingType:
            status = AddRoutingObserver(
                    *(static_cast<TMSGlobalRoutingObserver*>(obsrv)),
                    *(static_cast<TMSGlobalRouting*>(parent)), clientid);
            break;
        case EMsgQueueRingtoneType:
            status = AddRingToneObserver(
                    *(static_cast<TMSRingToneObserver*>(obsrv)),
                    *(static_cast<TMSRingTone*>(parent)), clientid);
            break;
        case EMsgQueueDTMFType:
            status = AddDTMFObserver(*(static_cast<TMSDTMFObserver*>(obsrv)),
                    *(static_cast<TMSDTMF*>(parent)), clientid);
            break;
        case EMsgQueueInbandToneType:
            status = AddInbandToneObserver(
                    *(static_cast<TMSInbandToneObserver*>(obsrv)),
                    *(static_cast<TMSInbandTone*>(parent)), clientid);
            break;
        default:
            status = TMS_RESULT_INVALID_ARGUMENT;
            break;
        }
    return TMSRESULT(status);
    }

// ---------------------------------------------------------------------------
// TMSProxy::RemoveMsgQueueNotifier
// ---------------------------------------------------------------------------
//
EXPORT_C gint TMSProxy::RemoveMsgQueueNotifier(TMSMsgQueueNotifierType type,
        gpointer obsrv)
    {
    gint status(TMS_RESULT_SUCCESS);

    switch (type)
        {
        case EMsgQueueGlobalVolumeType:
        case EMsgQueueGlobalGainType:
            status = RemoveGlobalEffectObserver(
                    *(static_cast<TMSEffectObserver*>(obsrv)));
            break;
        case EMsgQueueGlobalRoutingType:
            status = RemoveRoutingObserver(
                    *(static_cast<TMSGlobalRoutingObserver*>(obsrv)));
            break;
        case EMsgQueueRingtoneType:
            status = RemoveRingToneObserver(
                    *(static_cast<TMSRingToneObserver*>(obsrv)));
            break;
        case EMsgQueueDTMFType:
            status = RemoveDTMFObserver(
                    *(static_cast<TMSDTMFObserver*>(obsrv)));
            break;
        case EMsgQueueInbandToneType:
            status = RemoveInbandToneObserver(
                    *(static_cast<TMSInbandToneObserver*>(obsrv)));
            break;
        default:
            status = TMS_RESULT_INVALID_ARGUMENT;
            break;
        }
    return TMSRESULT(status);
    }

// ---------------------------------------------------------------------------
// TMSProxy::TermSrv
// ---------------------------------------------------------------------------
//
EXPORT_C void TMSProxy::TermSrv()
    {
    SendReceive(ETMSTermSrv);
    }

// ---------------------------------------------------------------------------
// TMSProxy::PopulateArrayL
// ---------------------------------------------------------------------------
//
void TMSProxy::PopulateArrayL(TMSClientServerRequest aRequest,
        RArray<TUint32>& aDecoders, gint aCount)
    {
    HBufC8* buf = HBufC8::NewLC(aCount * sizeof(TUint32));
    TPtr8 ptr = buf->Des();
    TIpcArgs args;
    args.Set(0, &ptr);
    gint err = SendReceive(aRequest, args);

    if (err == TMS_RESULT_SUCCESS)
        {
        RDesReadStream stream(ptr);
        CleanupClosePushL(stream);
        aDecoders.Reset();

        for (gint i = 0; i < aCount; i++)
            {
            aDecoders.Append(stream.ReadUint32L());
            }
        CleanupStack::PopAndDestroy(&stream);
        }
    CleanupStack::PopAndDestroy(buf);
    }

// ---------------------------------------------------------------------------
// TMSProxy::AddGlobalEffectObserver
// ---------------------------------------------------------------------------
//
gint TMSProxy::AddGlobalEffectObserver(TMSEffectObserver& obsrv,
        TMSEffect& parent, gint /*clientid*/)
    {
    gint status = iEffectsObsrvrList.Find(&obsrv);
    if (status == KErrNotFound)
        {
        status = iEffectsObsrvrList.Append(&obsrv);
        status = iEffectsParentList.Append(&parent);
        }
    else
        {
        status = TMS_RESULT_ALREADY_EXIST;
        }
    return TMSRESULT(status);
    }

// ---------------------------------------------------------------------------
// TMSProxy::RemoveGlobalEffectObserver
// ---------------------------------------------------------------------------
//
gint TMSProxy::RemoveGlobalEffectObserver(TMSEffectObserver& obsrv)
    {
    gint status(TMS_RESULT_SUCCESS);
    gint index = iEffectsObsrvrList.Find(&obsrv);
    if (index >= 0)
        {
        iEffectsObsrvrList.Remove(index);
        iEffectsParentList.Remove(index);
        }
    else
        {
        status = TMS_RESULT_DOES_NOT_EXIST;
        }
    return TMSRESULT(status);
    }

// ---------------------------------------------------------------------------
// TMSProxy::AddRoutingObserver
// ---------------------------------------------------------------------------
//
gint TMSProxy::AddRoutingObserver(TMSGlobalRoutingObserver& obsrv,
        TMSGlobalRouting& parent, gint /*clientid*/)
    {
    gint status = iRoutingObsrvrList.Find(&obsrv);
    if (status == KErrNotFound)
        {
        status = iRoutingObsrvrList.Append(&obsrv);
        status = iRoutingParentList.Append(&parent);
        }
    else
        {
        status = TMS_RESULT_ALREADY_EXIST;
        }
    return TMSRESULT(status);
    }

// ---------------------------------------------------------------------------
// TMSProxy::RemoveRoutingObserver
// ---------------------------------------------------------------------------
//
gint TMSProxy::RemoveRoutingObserver(TMSGlobalRoutingObserver& obsrv)
    {
    gint status(TMS_RESULT_SUCCESS);
    gint index = iRoutingObsrvrList.Find(&obsrv);
    if (index >= 0)
        {
        iRoutingObsrvrList.Remove(index);
        iRoutingParentList.Remove(index);
        }
    else
        {
        status = TMS_RESULT_DOES_NOT_EXIST;
        }
    return TMSRESULT(status);
    }

// ---------------------------------------------------------------------------
// TMSProxy::AddRingToneObserver
// ---------------------------------------------------------------------------
//
gint TMSProxy::AddRingToneObserver(TMSRingToneObserver& obsrv,
        TMSRingTone& parent, gint /*clientid*/)
    {
    gint status = iRingToneObsrvrList.Find(&obsrv);
    if (status == KErrNotFound)
        {
        status = iRingToneObsrvrList.Append(&obsrv);
        status = iRingToneParentList.Append(&parent);
        }
    else
        {
        status = TMS_RESULT_ALREADY_EXIST;
        }
    return TMSRESULT(status);
    }

// ---------------------------------------------------------------------------
// TMSProxy::RemoveRingToneObserver
// ---------------------------------------------------------------------------
//
gint TMSProxy::RemoveRingToneObserver(TMSRingToneObserver& obsrv)
    {
    gint status(TMS_RESULT_SUCCESS);
    gint index = iRingToneObsrvrList.Find(&obsrv);
    if (index >= 0)
        {
        iRingToneObsrvrList.Remove(index);
        iRingToneParentList.Remove(index);
        }
    else
        {
        status = TMS_RESULT_DOES_NOT_EXIST;
        }
    return TMSRESULT(status);
    }

// ---------------------------------------------------------------------------
// TMSProxy::AddDTMFObserver
// ---------------------------------------------------------------------------
//
gint TMSProxy::AddDTMFObserver(TMSDTMFObserver& obsrv, TMSDTMF& parent,
        gint /*clientid*/)
    {
    // Add to list if observer is not already added
    gint status = iDTMFObsrvrList.Find(&obsrv);
    if (status == KErrNotFound)
        {
        status = iDTMFObsrvrList.Append(&obsrv);
        status = iDTMFParentList.Append(&parent);
        }
    else
        {
        status = TMS_RESULT_ALREADY_EXIST;
        }
    return TMSRESULT(status);
    }

// ---------------------------------------------------------------------------
// TMSProxy::RemoveDTMFObserver
// ---------------------------------------------------------------------------
//
gint TMSProxy::RemoveDTMFObserver(TMSDTMFObserver& obsrv)
    {
    gint status(TMS_RESULT_SUCCESS);
    gint index = iDTMFObsrvrList.Find(&obsrv);
    if (index >= 0)
        {
        iDTMFObsrvrList.Remove(index);
        iDTMFParentList.Remove(index);
        }
    else
        {
        status = TMS_RESULT_DOES_NOT_EXIST;
        }
    return TMSRESULT(status);
    }

// ---------------------------------------------------------------------------
// TMSProxy::AddInbandToneObserver
// ---------------------------------------------------------------------------
//
gint TMSProxy::AddInbandToneObserver(TMSInbandToneObserver& obsrv,
        TMSInbandTone& parent, gint /*clientid*/)
    {
    // Add to list if observer is not already added
    gint status = iInbandToneObsrvrList.Find(&obsrv);
    if (status == KErrNotFound)
        {
        status = iInbandToneObsrvrList.Append(&obsrv);
        status = iInbandToneParentList.Append(&parent);
        }
    else
        {
        status = TMS_RESULT_ALREADY_EXIST;
        }
    return TMSRESULT(status);
    }

// ---------------------------------------------------------------------------
// TMSProxy::RemoveInbandToneObserver
// ---------------------------------------------------------------------------
//
gint TMSProxy::RemoveInbandToneObserver(TMSInbandToneObserver& obsrv)
    {
    gint status(TMS_RESULT_SUCCESS);
    gint index = iInbandToneObsrvrList.Find(&obsrv);
    if (index >= 0)
        {
        iInbandToneObsrvrList.Remove(index);
        iInbandToneParentList.Remove(index);
        }
    else
        {
        status = TMS_RESULT_DOES_NOT_EXIST;
        }
    return TMSRESULT(status);
    }

// ---------------------------------------------------------------------------
// TMSProxy::CreateQueue
// ---------------------------------------------------------------------------
//
gint TMSProxy::CreateQueue(const gint aNumSlots)
    {
    gint status(TMS_RESULT_INVALID_ARGUMENT);

    if (iMsgQueue.Handle() <= 0)
        {
        status = iMsgQueue.CreateGlobal(KNullDesC, aNumSlots);
        if (status == TMS_RESULT_SUCCESS)
            {
            TRAP(status, ReceiveMsgQHandlerEventsL());
            if (status == TMS_RESULT_SUCCESS)
                {
                TIpcArgs args;
                args.Set(0, iMsgQueue);
                status = SendReceive(ETMSSetMsgQueueHandle, args);
                }
            }
        }
    return TMSRESULT(status);
    }

// ---------------------------------------------------------------------------
// TMSProxy::ReceiveMsgQHandlerEventsL
// Starts message queue handler (A/O) to monitor communication and transfer
// buffer events between client and the server.
// ---------------------------------------------------------------------------
//
void TMSProxy::ReceiveMsgQHandlerEventsL()
    {
    if (iMsgQHandler)
        {
        iMsgQHandler->Cancel();
        }
    else
        {
        iMsgQHandler = TMSQueueHandler::NewL(&iMsgQueue, NULL);
        iMsgQHandler->AddObserver(*this, -1);
        }
    iMsgQHandler->Start();
    }

// ---------------------------------------------------------------------------
// TMSProxy::QueueEvent
// Call from QueueHandler as a result of TMS Server callback.
// ---------------------------------------------------------------------------
//
void TMSProxy::QueueEvent(gint aEventType, gint aError, void* event_data)
    {
    TMSSignalEvent event = {}; //all elements initialized to zeros
    event.type = aEventType;
    event.reason = TMSRESULT(aError);
    event.user_data = NULL; //use only to return data passed in by the user

    switch (aEventType)
        {
        case TMS_EVENT_EFFECT_VOL_CHANGED:
            {
            if (event_data)
                {
                event.event_data = static_cast<gpointer>(event_data);
                }
            for (gint i = 0; i < iEffectsObsrvrList.Count(); i++)
                {
                iEffectsObsrvrList[i]->EffectsEvent(*iEffectsParentList[i],
                        event);
                }
            break;
            }
        case TMS_EVENT_EFFECT_GAIN_CHANGED:
            {
            for (gint i = 0; i < iEffectsObsrvrList.Count(); i++)
                {
                iEffectsObsrvrList[i]->EffectsEvent(*iEffectsParentList[i],
                        event);
                }
            break;
            }
        case TMS_EVENT_ROUTING_AVAIL_OUTPUTS_CHANGED:
        case TMS_EVENT_ROUTING_OUTPUT_CHANGED:
        case TMS_EVENT_ROUTING_SET_OUTPUT_COMPLETE:
            {
            guint output(0);
            if (event_data != NULL)
                {
                output = *(static_cast<guint*>(event_data));
                }
            for (gint i = 0; i < iRoutingObsrvrList.Count(); i++)
                {
                iRoutingObsrvrList[i]->GlobalRoutingEvent(
                        *iRoutingParentList[i], event, output);
                }
            break;
            }
        case TMS_EVENT_RINGTONE_OPEN_COMPLETE:
        case TMS_EVENT_RINGTONE_PLAY_COMPLETE:
        case TMS_EVENT_RINGTONE_DEINIT_COMPLETE:
            {
            for (gint i = 0; i < iRingToneObsrvrList.Count(); i++)
                {
                iRingToneObsrvrList[i]->RingtoneEvent(*iRingToneParentList[i],
                        event);
                }
            break;
            }
        case TMS_EVENT_DTMF_TONE_STARTED:
        case TMS_EVENT_DTMF_TONE_STOPPED:
            {
            for (gint i = 0; i < iDTMFObsrvrList.Count(); i++)
                {
                iDTMFObsrvrList[i]->DTMFEvent(*iDTMFParentList[i], event);
                }
            break;
            }
        case TMS_EVENT_INBAND_TONE_STARTED:
        case TMS_EVENT_INBAND_TONE_STOPPED:
            {
            for (gint i = 0; i < iInbandToneObsrvrList.Count(); i++)
                {
                iInbandToneObsrvrList[i]->InbandToneEvent(
                        *iInbandToneParentList[i], event);
                }
            break;
            }
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// TMSProxy::ResetObjectLists
// ---------------------------------------------------------------------------
//
void TMSProxy::ResetObjectLists()
    {
    iEffectsObsrvrList.Reset();
    iRoutingObsrvrList.Reset();
    iRingToneObsrvrList.Reset();
    iDTMFObsrvrList.Reset();
    iInbandToneObsrvrList.Reset();

    iEffectsParentList.Reset();
    iRoutingParentList.Reset();
    iRingToneParentList.Reset();
    iDTMFParentList.Reset();
    iInbandToneParentList.Reset();
    }

// End of file
