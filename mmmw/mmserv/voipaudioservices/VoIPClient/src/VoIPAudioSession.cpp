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
 * Description:  Implements RVoIPAudioSession object.
 *
 */

#include <s32mem.h>
#include "debugtracemacros.h"
#include "VoIPAudioClientServer.h"
#include "VoIPAudioSession.h"

// CONSTANTS
const TUint KServerConnectRetries = 2;
const TUint KSessionMessageSlots = 10;

// -----------------------------------------------------------------------------
// StartServer
// Static function to start the server process thread.
// Start the server process/thread which lives in an EPOCEXE object.
// Returns: TInt: KErrNone (0) if no error
// -----------------------------------------------------------------------------
//
static TInt StartServer()
    {
    const TUidType serverUid(KNullUid, KNullUid, KVoIPServerUid3);

    // Only one instance of the server is allowed. Attempt of launching
    // second instance of the server will fail with KErrAlreadyExists.
    RProcess server;
    TInt r = server.Create(KVoIPServerName, KNullDesC, serverUid);

    if (r != KErrNone)
        {
        return r;
        }

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

    // Panic reason cannot be '0' as it would conflict with KErrNone
    r = (server.ExitType() == EExitPanic) ? KErrGeneral : stat.Int();
    server.Close();
    return r;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::Connect
// Create a client-side session. Start the server if not started already.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::Connect()
    {
    TRACE_PRN_FN_ENT;

    TInt retry = KServerConnectRetries;
    TInt err = KErrGeneral;
    TInt numMessageSlots = KSessionMessageSlots;

    for (;;)
        {
        // Try to create a new session with the server
        err = CreateSession(KVoIPServerName, Version(), numMessageSlots);

        if ((err != KErrNotFound) && (err != KErrServerTerminated))
            {
            TRACE_PRN_N1(_L("[VAS session created; err==%d]"), err);
            break; // Connected to existing server - ok
            }

        if (--retry == 0)
            {
            break; // Failed.
            }

        // Server not running, try to start it.
        err = StartServer();
        TRACE_PRN_N1(_L("[VAS server started; err==%d]"), err);

        if ((err != KErrNone) && (err != KErrAlreadyExists))
            {
            break; // Server not launched - propagate error
            }
        }

    TRACE_PRN_FN_EXT;
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::Version
// -----------------------------------------------------------------------------
//
EXPORT_C TVersion RVoIPAudioSession::Version() const
    {
    return (TVersion(KVoIPServMajorVersionNumber,
            KVoIPServMinorVersionNumber, KVoIPServBuildVersionNumber));
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::Close
// -----------------------------------------------------------------------------
//
EXPORT_C void RVoIPAudioSession::Close()
    {
    TRACE_PRN_FN_ENT;

    SendClientRequest(EVoIPSessionClose, TIpcArgs(TIpcArgs::ENothing));
    RSessionBase::Close();

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::OpenDownlink
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::OpenDownlink(
        const TMMFPrioritySettings aPriority)
    {
    TMMFPrioritySettingsPckg pckg;
    pckg() = aPriority;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPOpenDownlink, args);
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::GetMaxVolume
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::GetMaxVolume()
    {
    TPckgBuf<TInt> pckg;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPGetMaxVolume, args);
    return (err == KErrNone) ? pckg() : err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::SetVolume
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::SetVolume(const TInt aVolume)
    {
    TInt err = SendClientRequest(EVoIPSetVolume, TIpcArgs(aVolume));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::GetVolume
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::GetVolume()
    {
    TInt vol = 0;
    TVoIPMsgBufPckg pckg;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPGetVolume, args);
    if (err == KErrNone)
        {
        vol = pckg().iInt;
        err = pckg().iStatus;
        }

    return (err == KErrNone) ? vol : err;
    }

// ---------------------------------------------------------------------------
// RVoIPAudioSession::GetSupportedDecoders
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::GetSupportedDecoders(
        const TMMFPrioritySettings aPriority, RArray<TUint32>& aDecoders,
        TInt& aFrameSize)
    {
    TRACE_PRN_FN_ENT;

    TMMFPrioritySettingsPckg pckg;
    pckg() = aPriority;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPSetDecoderPriority, args);

    TVoIPMsgBufPckg pckg1;
    TIpcArgs args1(&pckg1);
    err = SendClientRequest(EVoIPGetSupportedDecodersCount, args1);
    TInt count = 0;
    aFrameSize = 0;

    if (err == KErrNone)
        {
        count = pckg1().iInt;
        aFrameSize = pckg1().iUint; //for 10/20ms frame detection
        err = pckg1().iStatus;
        }

    if (count > 0 && err == KErrNone)
        {
        TRAP(err, PopulateArrayL(EVoIPGetSupportedDecoders, aDecoders, count));
        }

    TRACE_PRN_FN_EXT;
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::SetDecoder
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::SetDecoder(const TUint32 aDecoder)
    {
    TPckgBuf<TUint32> pckg;
    pckg() = aDecoder;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPSetDecoder, args);
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::SetAudioDevice
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::SetAudioDevice(
        const CVoIPAudioDownlinkStream::TVoIPOutputDevice aDevice)
    {
    TPckgBuf<TUint> pckg;
    pckg() = TUint(aDevice);
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPSetAudioDevice, args);
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::GetAudioDevice
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::GetAudioDevice(
        CVoIPAudioDownlinkStream::TVoIPOutputDevice& aDevice)
    {
    TVoIPMsgBufPckg pckg;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPGetAudioDevice, args);
    if (err == KErrNone)
        {
        aDevice = (CVoIPAudioDownlinkStream::TVoIPOutputDevice) pckg().iUint;
        err = pckg().iStatus;
        }

    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::StartDownlink
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::StartDownlink()
    {
    TInt err = SendClientRequest(EVoIPStartDownlink, TIpcArgs(
            TIpcArgs::ENothing));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::StopDownlink
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::StopDownlink()
    {
    TInt err = SendClientRequest(EVoIPStopDownlink, TIpcArgs(
            TIpcArgs::ENothing));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::CloseDownlink
// -----------------------------------------------------------------------------
//
EXPORT_C void RVoIPAudioSession::CloseDownlink()
    {
    SendClientRequest(EVoIPCloseDownlink, TIpcArgs(TIpcArgs::ENothing));
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::BufferFilled
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::BufferFilled(TPtr8 aBuffer,
        const TUint aBufferSequence)
    {
    TVoIPMsgBufPckg pckg;
    pckg().iInt = aBuffer.Size();
    pckg().iUint = aBufferSequence;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPBufferFilled, args);
    return err;
    }

// For uplink stream

// -----------------------------------------------------------------------------
// RVoIPAudioSession::OpenUplink
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::OpenUplink(
        const TMMFPrioritySettings aPriority)
    {
    TMMFPrioritySettingsPckg pckg;
    pckg() = aPriority;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPOpenUplink, args);
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::GetMaxGain
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::GetMaxGain()
    {
    TPckgBuf<TInt> pckg;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPGetMaxGain, args);
    return (err == KErrNone) ? pckg() : err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::SetGain
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::SetGain(const TInt aGain)
    {
    TInt err = SendClientRequest(EVoIPSetGain, TIpcArgs(aGain));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::GetGain
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::GetGain()
    {
    TInt gain = 0;
    TVoIPMsgBufPckg pckg;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPGetGain, args);
    if (err == KErrNone)
        {
        gain = pckg().iInt;
        err = pckg().iStatus;
        }

    return (err == KErrNone) ? gain : err;
    }

// ---------------------------------------------------------------------------
// RVoIPAudioSession::GetSupportedEncodersL
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::GetSupportedEncoders(
        const TMMFPrioritySettings aPriority, RArray<TUint32>& aEncoders,
        TInt& aFrameSize)
    {
    TRACE_PRN_FN_ENT;

    TMMFPrioritySettingsPckg pckg;
    pckg() = aPriority;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPSetEncoderPriority, args);

    TVoIPMsgBufPckg pckg1;
    TIpcArgs args1(&pckg1);
    err = SendClientRequest(EVoIPGetSupportedEncodersCount, args1);
    TInt count = 0;
    aFrameSize = 0;

    if (err == KErrNone)
        {
        count = pckg1().iInt;
        aFrameSize = pckg1().iUint; //for 10/20ms frame detection
        err = pckg1().iStatus;
        }

    if (count > 0 && err == KErrNone)
        {
        TRAP(err, PopulateArrayL(EVoIPGetSupportedEncoders, aEncoders, count));
        }

    TRACE_PRN_FN_EXT;
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::SetEncoder
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::SetEncoder(const TUint32 aEncoder)
    {
    TPckgBuf<TUint32> pckg;
    pckg() = aEncoder;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPSetEncoder, args);
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::StartUplink
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::StartUplink()
    {
    TInt err = SendClientRequest(EVoIPStartUplink, TIpcArgs(
            TIpcArgs::ENothing));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::StopUplink
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::StopUplink()
    {
    TInt err = SendClientRequest(EVoIPStopUplink, TIpcArgs(TIpcArgs::ENothing));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::CloseUplink
// -----------------------------------------------------------------------------
//
EXPORT_C void RVoIPAudioSession::CloseUplink()
    {
    SendClientRequest(EVoIPCloseUplink, TIpcArgs(TIpcArgs::ENothing));
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::BufferEmptied
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::BufferEmptied(TPtr8 aBuffer)
    {
    TVoIPMsgBufPckg pckg;
    pckg().iInt = aBuffer.Size();
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPBufferEmptied, args);
    return err;
    }

// For codec CIs

// -----------------------------------------------------------------------------
// RVoIPAudioSession::GetSupportedBitRatesL
// -----------------------------------------------------------------------------
//
EXPORT_C void RVoIPAudioSession::GetSupportedBitRatesL(RArray<TUint>& aArray)
    {
    TRACE_PRN_FN_ENT;

    TInt brCount = 0;
    TVoIPMsgBufPckg pckg;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPGetSupportedBitratesCount, args);
    if (err == KErrNone)
        {
        brCount = pckg().iInt;
        err = pckg().iStatus;
        }

    if (err != KErrNone)
        {
        User::Leave(err);
        }

    TRACE_PRN_N1(_L("BRCount [%d]"), brCount);

    if (brCount > 0)
        {
        HBufC8* buf = HBufC8::NewLC(brCount * sizeof(TUint));
        TPtr8 ptr = buf->Des();
        TIpcArgs args1;
        args1.Set(0, &ptr);
        err = SendClientRequest(EVoIPGetSupportedBitrates, args1);
        if (err == KErrNone)
            {
            RDesReadStream stream(ptr);
            CleanupClosePushL(stream); // stream on cleanup
            aArray.Reset();

            for (TInt i = 0; i < brCount; i++)
                {
                aArray.Append(stream.ReadUint32L());
                }

            CleanupStack::PopAndDestroy(&stream);
            }

        CleanupStack::PopAndDestroy(buf);
        }

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::SetBitRate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::SetBitRate(const TUint aBitrate)
    {
    TInt err = SendClientRequest(EVoIPSetBitrate, TIpcArgs(aBitrate));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::GetBitRate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::GetBitRate(TUint& aBitrate)
    {
    TVoIPMsgBufPckg pckg;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPGetBitrate, args);
    if (err == KErrNone)
        {
        aBitrate = pckg().iUint;
        err = pckg().iStatus;
        }

    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::SetVAD
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::SetVAD(const TBool aVad)
    {
    TInt err = SendClientRequest(EVoIPSetVad, TIpcArgs(aVad));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::GetVAD
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::GetVAD(TBool& aVad)
    {
    TVoIPMsgBufPckg pckg;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPGetVad, args);
    if (err == KErrNone)
        {
        aVad = pckg().iBool;
        err = pckg().iStatus;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::FrameModeRequiredForErrorConcealment
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::FrameModeRequiredForEC(TBool& aMode)
    {
    TVoIPMsgBufPckg pckg;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPFrameModeRqrdForEC, args);
    if (err == KErrNone)
        {
        aMode = pckg().iBool;
        err = pckg().iStatus;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::SetFrameMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::SetFrameMode(const TBool aMode)
    {
    TInt err = SendClientRequest(EVoIPSetFrameMode, TIpcArgs(aMode));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::GetFrameMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::GetFrameMode(TBool& aMode)
    {
    TVoIPMsgBufPckg pckg;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPGetFrameMode, args);
    if (err == KErrNone)
        {
        aMode = pckg().iBool;
        err = pckg().iStatus;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::ConcealErrorForNextBuffer
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::ConcealErrorForNextBuffer()
    {
    TInt err = SendClientRequest(EVoIPConcealErrForNextBuf, TIpcArgs(
            TIpcArgs::ENothing));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::SetMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::SetMode(
        const CVoIPFormatIntfc::TG711CodecMode aMode)
    {
    TPckgBuf<CVoIPFormatIntfc::TG711CodecMode> pckg;
    pckg() = aMode;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPSetG711CodecMode, args);
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::GetMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::GetMode(
        CVoIPFormatIntfc::TG711CodecMode& aMode)
    {
    TVoIPMsgBufPckg pckg;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPGetG711CodecMode, args);
    if (err == KErrNone)
        {
        aMode = (CVoIPFormatIntfc::TG711CodecMode) pckg().iUint;
        err = pckg().iStatus;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::SetMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::SetMode(
        const CVoIPFormatIntfc::TILBCCodecMode aMode)
    {
    TPckgBuf<CVoIPFormatIntfc::TILBCCodecMode> pckg;
    pckg() = aMode;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPSetIlbcCodecMode, args);
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::GetMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::GetMode(
        CVoIPFormatIntfc::TILBCCodecMode& aMode)
    {
    TVoIPMsgBufPckg pckg;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPGetIlbcCodecMode, args);
    if (err == KErrNone)
        {
        aMode = (CVoIPFormatIntfc::TILBCCodecMode) pckg().iUint;
        err = pckg().iStatus;
        }

    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::SetCNG
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::SetCNG(const TBool aCng)
    {
    TInt err = SendClientRequest(EVoIPSetCng, TIpcArgs(aCng));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::GetCNG
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::GetCNG(TBool& aCng)
    {
    TVoIPMsgBufPckg pckg;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPGetCng, args);
    if (err == KErrNone)
        {
        aCng = pckg().iBool;
        err = pckg().iStatus;
        }

    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::SetPLC
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::SetPLC(const TBool aPlc)
    {
    TInt err = SendClientRequest(EVoIPSetPlc, TIpcArgs(aPlc));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::GetPLC
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::GetPLC(TBool& aPlc)
    {
    TVoIPMsgBufPckg pckg;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPGetPlc, args);
    if (err == KErrNone)
        {
        aPlc = pckg().iBool;
        err = pckg().iStatus;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::BadLsfNextBuffer
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::BadLsfNextBuffer()
    {
    TInt err = SendClientRequest(EVoIPBadLsfNextBuffer, TIpcArgs(
            TIpcArgs::ENothing));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::OpenDTMFTonePlayer
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::OpenDTMFTonePlayer()
    {
    TInt err = SendClientRequest(EVoIPOpenDTMFTonePlayer, TIpcArgs(
            TIpcArgs::ENothing));
    return err;
    }

// ---------------------------------------------------------------------------
// RVoIPAudioSession::PlayDTMFTone
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::PlayDTMFTone(const TDesC& aTones)
    {
    TTonesMsgBufPckg pckg;
    pckg().iFileName = aTones;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPPlayDTMFTone, args);
    return err;
    }

// ---------------------------------------------------------------------------
// RVoIPAudioSession::StopDTMFTonePlay
// ---------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::StopDTMFTonePlay()
    {
    TInt err = SendClientRequest(EVoIPStopDTMFTone, TIpcArgs(
            TIpcArgs::ENothing));
    return err;
    }

// ---------------------------------------------------------------------------
// RVoIPAudioSession::CloseDTMFTonePlayer
// ---------------------------------------------------------------------------
//
EXPORT_C void RVoIPAudioSession::CloseDTMFTonePlayer()
    {
    SendClientRequest(EVoIPCloseDTMFTonePlayer, TIpcArgs(TIpcArgs::ENothing));
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::OpenRingTonePlayer
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::OpenRingTonePlayer()
    {
    TInt err = SendClientRequest(EVoIPOpenRingTonePlayerFromProfile,
            TIpcArgs(TIpcArgs::ENothing));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::OpenRingTonePlayer
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::OpenRingTonePlayer(const TDesC& aFileName)
    {
    TTonesMsgBufPckg pckg;
    pckg().iFileName = aFileName;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPOpenRingTonePlayerFromFile, args);
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::OpenRingTonePlayer
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::OpenRingTonePlayer(const RFile& aFileHandle)
    {
    TIpcArgs args;
    aFileHandle.TransferToServer(args, 0, 1); //0=Fs, 1==file handle
    TInt err = SendClientRequest(EVoIPOpenRingTonePlayerFromRFile, args);
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::OpenRingTonePlayer
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::OpenRingTonePlayer(const TDesC8& aDesData)
    {
    TPtrC8 ptr(aDesData);
    TIpcArgs args;
    args.Set(0, &ptr);
    TInt err = SendClientRequest(EVoIPOpenRingTonePlayerFromDes, args);
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::PlayRingTone
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::PlayRingTone()
    {
    TInt err = SendClientRequest(EVoIPPlayRingTone, TIpcArgs(
            TIpcArgs::ENothing));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::PauseRingTone
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::PauseRingTone()
    {
    TInt err = SendClientRequest(EVoIPPauseRingTone, TIpcArgs(
            TIpcArgs::ENothing));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::ResumeRingTone
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::ResumeRingTone()
    {
    TInt err = SendClientRequest(EVoIPResumeRingTone, TIpcArgs(
            TIpcArgs::ENothing));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::StopRingTone
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::StopRingTone()
    {
    TInt err = SendClientRequest(EVoIPStopRingTone, TIpcArgs(
            TIpcArgs::ENothing));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::CloseRingTonePlayer
// -----------------------------------------------------------------------------
//
EXPORT_C void RVoIPAudioSession::CloseRingTonePlayer()
    {
    SendClientRequest(EVoIPCloseRingTonePlayer, TIpcArgs(TIpcArgs::ENothing));
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::ConfigureJitterBuffer
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::ConfigureJitterBuffer(
        const TVoIPJBConfig& aJbConfig)
    {
    TPckgBuf<TVoIPJBConfig> pckg;
    pckg() = aJbConfig;
    TIpcArgs args(&pckg);
    TInt err = SendClientRequest(EVoIPConfigJitterBuffer, args);
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::ResetJitterBuffer
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::ResetJitterBuffer(const TBool aPlayTone)
    {
    TInt err = SendClientRequest(EVoIPResetJitterBuffer, TIpcArgs(aPlayTone));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::DelayDown
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::DelayDown()
    {
    TInt err = SendClientRequest(EVoIPJBDelayDown, TIpcArgs(
            TIpcArgs::ENothing));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::DelayUp
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RVoIPAudioSession::DelayUp()
    {
    TInt err = SendClientRequest(EVoIPJBDelayUp, TIpcArgs(TIpcArgs::ENothing));
    return err;
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::PopulateArrayL
// -----------------------------------------------------------------------------
//
void RVoIPAudioSession::PopulateArrayL(
        TVoIPAudioClientServerRequest aRequest, RArray<TUint32>& aDecoders,
        TInt aCount)
    {
    HBufC8* buf = HBufC8::NewLC(aCount * sizeof(TUint32));
    TPtr8 ptr = buf->Des();
    TIpcArgs args;
    args.Set(0, &ptr);
    TInt err = SendClientRequest(aRequest, args);

    if (err == KErrNone)
        {
        RDesReadStream stream(ptr);
        CleanupClosePushL(stream);
        aDecoders.Reset();

        for (TInt i = 0; i < aCount; i++)
            {
            aDecoders.Append(stream.ReadUint32L());
            }

        CleanupStack::PopAndDestroy(&stream);
        }

    CleanupStack::PopAndDestroy(buf);
    }

// -----------------------------------------------------------------------------
// RVoIPAudioSession::SendClientRequest
// -----------------------------------------------------------------------------
//
TInt RVoIPAudioSession::SendClientRequest(
        TVoIPAudioClientServerRequest aRequest, const TIpcArgs& aArgs) const
    {
    TInt status = KErrBadHandle;
    if (Handle())
        {
        status = SendReceive(aRequest, aArgs);
        }
    return status;
    }

// End of file
