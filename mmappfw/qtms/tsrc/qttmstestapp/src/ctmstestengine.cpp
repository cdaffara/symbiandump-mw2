/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Telephony Multimedia Service - TestApp
 *
 */

// INCLUDES
#include <e32base.h>
#include <eikenv.h>
#include <gstring.h>
#include <tmsclientsink.h>
#include <tmsclientsource.h>
#include <tmspcmformat.h>
#include <tmsamrformat.h>
#include <tmsilbcformat.h>
#include <tmsg711format.h>
#include <tmsg729format.h>
#include <tmsvolumeeffect.h>
#include <tmsgaineffect.h>
#include <tmsglobalvoleffect.h>
#include <tmsglobalgaineffect.h>
#include <tmsglobalrouting.h>
#include <tmsver.h>
#include "ctmstestengine.h"

#ifdef _DEBUG
#include "e32debug.h"
#define DEBPRN0(str)       RDebug::Print(str, this)
#define DEBPRN1(str, val1) RDebug::Print(str, this, val1)
#else
#define DEBPRN0(str)
#define DEBPRN1(str, val1)
#endif //_DEBUG

//#define __PROFILING_ENABLED__

#ifdef __PROFILING_ENABLED__
#include "perfutility.h"
#endif //__PROFILING_ENABLED__

//#define __TEST_CODE_COVERAGE__

// CONSTANTS
_LIT8(KRTBeepSequence, "\x00\x11\x0A\x0A\x08\x73\x0A\x40\x28\x0A\xF7\
\x05\xFC\x40\x64\x0A\x08\x40\x32\x0A\xF7\x06\x0B");
_LIT16(KTextToSpeak, "THE PHONE IS RINGING");
_LIT16(KTestFile1, "C:\\Data\\Sounds\\Digital\\NokiaTest.aac");

#ifdef __RECORD_WAV_TO_FILE__
_LIT(KFileName, "c:\\data\\tmsrec.amr");
const TUint KFileBufLen = 4096;
const TInt KAMRNBHeaderLen = 6;
const TUint8 KAMRNBHeader[KAMRNBHeaderLen] = {0x23,0x21,0x41,0x4d,0x52,0x0a};
#endif

// ----------------------------------------------------------------------------
// CTmsTestEngine::CTmsTestEngine()
// constructor
// ----------------------------------------------------------------------------
//
CTmsTestEngine::CTmsTestEngine()
    {
    // Disable GLib slice allocators; will take care of memory leaks
    // caused by g_string_free().
    g_setenv("G_SLICE", "always-malloc", 1);
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::~CTmsTestEngine
// destructor
// ----------------------------------------------------------------------------
//
CTmsTestEngine::~CTmsTestEngine()
    {
    EndCall();
    CloseRingTonePlayer();

    if (iFactory && iGlobalVol)
        {
        static_cast<TMSGlobalVolEffect*> (iGlobalVol)->RemoveObserver(*this);
        iFactory->DeleteEffect(iGlobalVol);
        }
    if (iFactory && iGlobalGain)
        {
        static_cast<TMSGlobalGainEffect*> (iGlobalGain)->RemoveObserver(*this);
        iFactory->DeleteEffect(iGlobalGain);
        }
    if (iFactory && iTmsGlobalRouting)
        {
        iTmsGlobalRouting->RemoveObserver(*this);
        iFactory->DeleteGlobalRouting(iTmsGlobalRouting);
        }
    if (iFactory && iInbandTonePlayer)
        {
        iInbandTonePlayer->RemoveObserver(*this);
        iFactory->DeleteInbandTonePlayer(iInbandTonePlayer);
        }

    delete iFactory;

#ifdef __PLAY_WAV_FROM_FILE__
    iFile.Close();
    iFs.Close();
    delete iBuf;
#endif
#ifdef __RECORD_WAV_TO_FILE__
    iFile.Close();
    iFs.Close();
    delete iWriteBuf;
#endif
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::NewL
// Symbian constructor.
// ----------------------------------------------------------------------------
//
CTmsTestEngine* CTmsTestEngine::NewL(QTextEdit* statusDisplay)
    {
    CTmsTestEngine* self(NULL);
    self=CTmsTestEngine::NewLC(statusDisplay);
    return self;
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::NewL
// Symbian constructor with self pointer pushed into the cleanup stack.
// ----------------------------------------------------------------------------
//
CTmsTestEngine* CTmsTestEngine::NewLC(QTextEdit* statusDisplay)
    {
    CTmsTestEngine* self = new (ELeave) CTmsTestEngine;
    //CleanupStack::PushL(self);
    self->ConstructL(statusDisplay);
    return self;
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::NewL
// Standard Symbian 2nd phase constructor
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::ConstructL(QTextEdit* statusDisplay)
    {
    DEBPRN0(_L("CTmsTestEngine[0x%x]::ConstructL :>"));
    iStatusDisplay = statusDisplay;

#ifdef __PLAY_WAV_FROM_FILE__
    iReadSize = KFileBufLen;
    TInt err = iFs.Connect();
    if (err == TMS_RESULT_SUCCESS)
        {
        err = iFile.Open(iFs, KTestFile1, EFileShareAny | EFileRead);
        }
    if (err == TMS_RESULT_SUCCESS)
        {
        iFile.Size(iFileLen);
        }
    iBuf = HBufC8::NewL(iReadSize);
#endif

#ifdef __RECORD_WAV_TO_FILE__
    TInt err = iFs.Connect();
    if (err == TMS_RESULT_SUCCESS)
        {
        err = iFile.Replace(iFs, KFileName, EFileWrite);
#ifndef __WINSCW__
        if (err == KErrNone)
            {
            // Prefix file with AMR-NB header
            TBuf8<KAMRNBHeaderLen> buf;
            buf.Append(KAMRNBHeader, 6);
            iFile.Write(buf, KAMRNBHeaderLen);
            }
#endif //__WINSCW__
        }
    iWriteLen = KFileBufLen;
    iWriteBuf = HBufC8::NewL(iWriteLen);
#endif //__RECORD_WAV_TO_FILE__

    CreateFactory();
    CreateGlobalRouting();
    CreateGlobalVol();
    CreateGlobalGain();
    CreateRingTonePlayer();

    DEBPRN0(_L("CTmsTestEngine[0x%x]::ConstructL :<"));
    }

gint CTmsTestEngine::CreateFactory()
    {
    gint status(TMS_RESULT_SUCCESS);
    TMSVer v(10,0,0);
    status = TMSFactory::CreateFactory(iFactory, v);

    if (status != TMS_RESULT_SUCCESS || !iFactory)
        {
        DisplayText("Tms create factory failed ", status);
        }
    else
        {
#ifdef __TEST_CODE_COVERAGE__
        TMSBuffer* tmsbuffer(NULL);
        TMSBufferType buftype(TMS_BUFFER_MEMORY);
        guint size(100);
        iFactory->CreateBuffer(TMS_BUFFER_MEMORY, size, tmsbuffer);
        if (tmsbuffer)
            {
            tmsbuffer->GetType(buftype);
            tmsbuffer->GetDataSize(size);
            tmsbuffer->SetDataSize(size);
            guint8* pbuf(NULL);
            tmsbuffer->GetDataPtr(pbuf);
            guint64 ts(0);
            tmsbuffer->GetTimeStamp(ts);
            tmsbuffer->SetTimeStamp(ts);
            iFactory->DeleteBuffer(tmsbuffer);
            }
#endif //__TEST_CODE_COVERAGE__

        DisplayText("Tms factory created");
        }
    return status;
    }

gint CTmsTestEngine::CreateCall(TMSCallType calltype)
    {
    gint status(TMS_RESULT_SUCCESS);
    if (iFactory && !iTmsCall)
        {
        status = iFactory->CreateCall(calltype, iTmsCall, 0);
        if (status == TMS_RESULT_SUCCESS)
            {
#ifdef __TEST_CODE_COVERAGE__
            guint ctxid;
            iTmsCall->GetCallContextId(ctxid);
#endif //__TEST_CODE_COVERAGE__
            DisplayText("Tms call created");
            }
        else
            {
            DisplayText("Tms call create failed ", status);
            }
        }
    return status;
    }

gint CTmsTestEngine::CreateUplink()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iTmsCall)
        {
        status = iTmsCall->CreateStream(TMS_STREAM_UPLINK, iTmsUplink);
        if (status == TMS_RESULT_SUCCESS)
            {
#ifdef __TEST_CODE_COVERAGE__
            guint ctxid;
            iTmsCall->GetCallContextId(ctxid);
#endif //__TEST_CODE_COVERAGE__
            DisplayText("TMS uplink created");
            }
        else
            {
            DisplayText("TMS uplink failed ", status);
            }
        }
    return status;
    }

gint CTmsTestEngine::CreateDownlink()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iTmsCall)
        {
        status = iTmsCall->CreateStream(TMS_STREAM_DOWNLINK, iTmsDnlink);
        if (status == TMS_RESULT_SUCCESS)
            {
            DisplayText("TMS downlink created");
            }
        else
            {
            DisplayText("TMS downlink failed ", status);
            }
        }
    return status;
    }

gint CTmsTestEngine::CreateClientSource()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory && !iTmsClientSource)
        {
        status = iFactory->CreateSource(TMS_SOURCE_CLIENT, iTmsClientSource);
        if (status == TMS_RESULT_SUCCESS)
            {
            DisplayText("TMS clientsource created");
            }
        else
            {
            DisplayText("TMS clientsource failed ", status);
            }
        }
    return status;
    }

gint CTmsTestEngine::CreateModemSource()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory && !iTmsModemSource)
        {
        status = iFactory->CreateSource(TMS_SOURCE_MODEM, iTmsModemSource);
        if (status == TMS_RESULT_SUCCESS)
            {
#ifdef __TEST_CODE_COVERAGE__
            TMSSourceType st;
            iTmsModemSource->GetType(st);
#endif //__TEST_CODE_COVERAGE__
            DisplayText("TMS modemsource created");
            }
        else
            {
            DisplayText("TMS modemsource failed ", status);
            }
        }
    return status;
    }

gint CTmsTestEngine::AddClientSourceToStream()
    {
    gint status(TMS_RESULT_SUCCESS);
    if (iTmsDnlink && iTmsClientSource)
        {
        static_cast<TMSClientSource*> (iTmsClientSource)->AddObserver(*this,
                NULL);
        status = iTmsDnlink->AddSource(iTmsClientSource);

#ifdef __TEST_CODE_COVERAGE__
        TMSSourceType st;
        static_cast<TMSClientSource*> (iTmsClientSource)->GetType(st);
        gboolean enqueue(false);
        static_cast<TMSClientSource*> (iTmsClientSource)->GetEnqueueMode(enqueue);
        static_cast<TMSClientSource*> (iTmsClientSource)->SetEnqueueMode(FALSE);
        static_cast<TMSClientSource*> (iTmsClientSource)->Flush();
#endif //__TEST_CODE_COVERAGE__
        }
    return status;
    }

gint CTmsTestEngine::AddModemSourceToStream()
    {
    gint status(TMS_RESULT_SUCCESS);
    if (iTmsDnlink && iTmsModemSource)
        {
        status = iTmsDnlink->AddSource(iTmsModemSource);
        }
    return status;
    }

gint CTmsTestEngine::CreateClientSink()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory && !iTmsClientSink)
        {
        status = iFactory->CreateSink(TMS_SINK_CLIENT, iTmsClientSink);
        if (status == TMS_RESULT_SUCCESS)
            {
            DisplayText("TMS client sink created");
            }
        else
            {
            DisplayText("TMS client sink failed ", status);
            }
        }
    return status;
    }

gint CTmsTestEngine::CreateModemSink()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory && !iTmsModemSink)
        {
        status = iFactory->CreateSink(TMS_SINK_MODEM, iTmsModemSink);
        if (status == TMS_RESULT_SUCCESS)
            {
#ifdef __TEST_CODE_COVERAGE__
            TMSSinkType st;
            iTmsModemSink->GetType(st);
#endif  //__TEST_CODE_COVERAGE__
            DisplayText("TMS modem sink created");
            }
        else
            {
            DisplayText("TMS modem sink failed ", status);
            }
        }
    return status;
    }

gint CTmsTestEngine::AddClientSinkToStream()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iTmsUplink && iTmsClientSink)
        {
        status = static_cast<TMSClientSink*> (iTmsClientSink)->AddObserver(
                *this, NULL);
        status |= iTmsUplink->AddSink(iTmsClientSink);
        }
    return status;
    }

gint CTmsTestEngine::AddModemSinkToStream()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iTmsUplink && iTmsModemSink)
        {
        status = iTmsUplink->AddSink(iTmsModemSink);
        }
    return status;
    }

gint CTmsTestEngine::CreateMicSource()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory && !iTmsMicSource)
        {
        status = iFactory->CreateSource(TMS_SOURCE_MIC, iTmsMicSource);
        if (status == TMS_RESULT_SUCCESS)
            {
            DisplayText("TMS mic source created");
            }
        else
            {
            DisplayText("TMS mic source failed ", status);
            }
        }
    return status;
    }

gint CTmsTestEngine::AddMicSourceToStream()
    {
    gint status(TMS_RESULT_SUCCESS);
    if (iTmsUplink && iTmsMicSource)
        {
        status = iTmsUplink->AddSource(iTmsMicSource);
        }
    return status;
    }

gint CTmsTestEngine::CreateSpeakerSink()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory && !iTmsSpeakerSink)
        {
        status = iFactory->CreateSink(TMS_SINK_SPEAKER, iTmsSpeakerSink);
        if (status == TMS_RESULT_SUCCESS)
            {
            DisplayText("TMS speaker sink created");
            }
        else
            {
            DisplayText("TMS speaker sink failed ", status);
            }
        }
    return status;
    }

gint CTmsTestEngine::AddSpeakerSinkToStream()
    {
    gint status(TMS_RESULT_SUCCESS);
    if (iTmsDnlink && iTmsSpeakerSink)
        {
        status = iTmsDnlink->AddSink(iTmsSpeakerSink);
        }
    return status;
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetCallType
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetCallType(TMSCallType calltype)
    {
    gint status(KErrNotFound);

#ifdef __PROFILING_ENABLED__
    TAG_CALLBACK_TIME_PROFILING_START;
#endif //__PROFILING_ENABLED__

    if (iFactory)
        {
        iCallType = calltype;

        if (!iTmsCall)
            {
            gboolean issupported(FALSE);
            iFactory->IsCallTypeSupported(iCallType, issupported);
            if (issupported)
                {
                status = iFactory->CreateCall(iCallType, iTmsCall);
                }
            else
                {
                status = KErrNotSupported;
                }
            }

        iCallType = iTmsCall->GetCallType();

        if (status == TMS_RESULT_SUCCESS)
            {
            DisplayText("Tms call created");
            }
        else
            {
            DisplayText("Tms call creation failed ", status);
            }
        }
    if (status == TMS_RESULT_SUCCESS)
        {
        status = CreateUplink();
        }
    if (status == TMS_RESULT_SUCCESS)
        {
        status = CreateDownlink();
        }
    if (iCallType == TMS_CALL_IP)
        {
        if (status == TMS_RESULT_SUCCESS)
            {
            status = CreateMicSource();
            status |= AddMicSourceToStream();
            status |= CreateClientSink();
            status |= AddClientSinkToStream();
            }

        if (status == TMS_RESULT_SUCCESS)
            {
            status |= CreateClientSource();
            status |= AddClientSourceToStream();
            status |= CreateSpeakerSink();
            status |= AddSpeakerSinkToStream();
            }
        }
    else if (iCallType == TMS_CALL_CS)
        {
        if (status == TMS_RESULT_SUCCESS)
            {
            status = CreateMicSource();
            status |= AddMicSourceToStream();
            status |= CreateModemSink();
            status |= AddModemSinkToStream();
            }
        if (status == TMS_RESULT_SUCCESS)
            {
            status = CreateModemSource();
            status |= AddModemSourceToStream();
            status |= CreateSpeakerSink();
            status |= AddSpeakerSinkToStream();
            }
        }

    if (status == TMS_RESULT_SUCCESS)
        {
        status = CreateVolumeEffect();
        status |= AddVolumeEffectToStream();
        status |= CreateGainEffect();
        status |= AddGainEffectToStream();
        }

    //NOTE: CS does not call SetDownlinkFormat/SetUplinkFormat
    //so it is OK to open downlink and uplink at this point.
    if (status == TMS_RESULT_SUCCESS && iCallType == TMS_CALL_CS)
        {
        // To avoid asyc calback racing situation, it may be safer to
        // start second stream after receiving TMS_STREAM_INITIALIZED event.
        // But for now, let's try opening both at the same time.
        OpenDownlink();
        OpenUplink();
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetSupportedDownlinkFormats
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetSupportedDownlinkFormats(TBool aDisplayList)
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory)
        {
        status = iFactory->GetSupportedFormats(TMS_STREAM_DOWNLINK, iDnlCodecs);

        if (status != TMS_RESULT_SUCCESS)
            {
            DisplayText("DNL Codecs retrieve error: ", status);
            }

        if (iDnlCodecs.size() > 0 && aDisplayList)
            {
            DisplayText("Supported DNL codecs");
            std::vector<TMSFormat*>::iterator itDnlCodecs = iDnlCodecs.begin();
            TMSFormatType fmttype;
            for (; itDnlCodecs < iDnlCodecs.end(); itDnlCodecs++)
                {
                (*itDnlCodecs)->GetType(fmttype);
                DisplayFormat(fmttype);
                }
            gint size = iDnlCodecs.size();
            for (gint i = 0; i < size; i++)
                {
                itDnlCodecs = iDnlCodecs.begin();
                iFactory->DeleteFormat(*itDnlCodecs);
                iDnlCodecs.erase(itDnlCodecs);
                }
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetSupportedUplinkFormats
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetSupportedUplinkFormats(TBool aDisplayList)
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory)
        {
        status = iFactory->GetSupportedFormats(TMS_STREAM_UPLINK, iUplCodecs);
        if (status != TMS_RESULT_SUCCESS)
            {
            DisplayText("UPL Codecs retrieve error: ", status);
            }

        if (iUplCodecs.size() > 0 && aDisplayList)
            {
            DisplayText("Supported UPL codecs");
            std::vector<TMSFormat*>::iterator codecs = iUplCodecs.begin();
            TMSFormatType fmttype;
            for (; codecs < iUplCodecs.end(); codecs++)
                {
                (*codecs)->GetType(fmttype);
                DisplayFormat(fmttype);
                }
            gint size = iUplCodecs.size();
            for (gint i = 0; i < size; i++)
                {
                codecs = iUplCodecs.begin();
                iFactory->DeleteFormat(*codecs);
                iUplCodecs.erase(codecs);
                }
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::DisplayFormat
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::DisplayFormat(TMSFormatType aFormat)
    {
    switch (aFormat)
        {
        case TMS_FORMAT_AMR:
            DisplayText("AMR-NB");
            break;
        case TMS_FORMAT_G711:
            DisplayText("G.711");
            break;
        case TMS_FORMAT_G729:
            DisplayText("G.729");
            break;
        case TMS_FORMAT_ILBC:
            DisplayText("iLBC");
            break;
        case TMS_FORMAT_PCM:
            DisplayText("PCM-16");
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetDownlinkFormat()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetDownlinkFormat(TMSFormatType aCodecFormat)
    {
    gint status(TMS_RESULT_SUCCESS);

    if (!iTmsCall)
        {
        SetCallType(TMS_CALL_IP);
        }
    if (iFactory && iDecFormatIntfc)
        {
        iFactory->DeleteFormat(iDecFormatIntfc);
        }

    iDnLinkCodec = aCodecFormat;

    if (iFactory && iTmsDnlink)
        {
        status = iFactory->CreateFormat(iDnLinkCodec, iDecFormatIntfc);
        status |= iTmsDnlink->SetFormat(iDecFormatIntfc);
        }

#ifdef __TEST_CODE_COVERAGE__
    TMSFormatType ft;
#endif //__TEST_CODE_COVERAGE__

    switch (iDnLinkCodec)
        {
        case TMS_FORMAT_G711:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iDecFormatIntfc->GetType(ft);
            if (ft != TMS_FORMAT_G711)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("G.711 DNL Codec Set");
            break;
            }
        case TMS_FORMAT_G729:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iDecFormatIntfc->GetType(ft);
            if (ft != TMS_FORMAT_G729)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("G.729 DNL Codec Set");
            break;
            }
        case TMS_FORMAT_ILBC:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iDecFormatIntfc->GetType(ft);
            if (ft != TMS_FORMAT_ILBC)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("iLBC DNL Codec Set");
            break;
            }
        case TMS_FORMAT_AMR:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iDecFormatIntfc->GetType(ft);
            if (ft != TMS_FORMAT_AMR)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("AMR-NB DNL Codec Set");
            break;
            }
        case TMS_FORMAT_PCM:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iDecFormatIntfc->GetType(ft);
            if (ft != TMS_FORMAT_PCM)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("PCM-16 DNL Codec Set");
            break;
            }
        default:
            {
            status = KErrNotSupported;
            }
        }

    if (status == TMS_RESULT_SUCCESS)
        {
        OpenDownlink(); //Initialize DNL
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::OpenDownlink()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::OpenDownlink()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iTmsDnlink)
        {
        iTmsDnlink->AddObserver(*this, NULL);
        status = iTmsDnlink->Init();
        if (status != TMS_RESULT_SUCCESS)
            {
            DisplayText("DNL init error ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::StartDownlink()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::StartDownlink()
    {
    if (iDnLinkStatus == EReady)
        {
#ifdef __TEST_CODE_COVERAGE__
        ConfigureDecoder();
        gint id = iTmsDnlink->GetStreamId();
#endif //__TEST_CODE_COVERAGE__

        iTmsDnlink->Start(2); //retry for ~2 sec
        iDnLinkStatus = EStreaming;
#ifdef __WINSCW__
        iBufIndex = 0;
#endif //__WINSCW__
        }
    else
        {
        DisplayText("DNL not ready");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::StopDownlink()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::StopDownlink()
    {
    if (iDnLinkStatus == EStreaming)
        {
#ifdef __TEST_CODE_COVERAGE__
        iTmsDnlink->Pause();
        iTmsDnlink->GetState();
#endif //__TEST_CODE_COVERAGE__

        iTmsDnlink->Stop();
        iDnLinkStatus = EReady;
        iOneTouchLoopback = EFalse;
        DisplayText("DNL stopped");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::CloseDownlink()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::CloseDownlink()
    {
    if (iTmsDnlink && iDnLinkStatus != ENotReady)
        {
        if (iTmsDnlinkEffect)
            {
            iTmsDnlink->RemoveEffect(iTmsDnlinkEffect);
            }
        if (iTmsClientSource)
            {
            iTmsDnlink->RemoveSource(iTmsClientSource);
            }
        if (iTmsModemSource)
            {
            iTmsDnlink->RemoveSource(iTmsModemSource);
            }
        if (iTmsSpeakerSink)
            {
            iTmsDnlink->RemoveSink(iTmsSpeakerSink);
            }
        iTmsDnlink->Deinit();
        iDnLinkStatus = ENotReady;
        }
    iPlayBufReady = EFalse;
    iOneTouchLoopback = EFalse;
    }

gint CTmsTestEngine::CreateVolumeEffect()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory && iTmsDnlink && !iTmsDnlinkEffect)
        {
        status = iFactory->CreateEffect(TMS_EFFECT_VOLUME, iTmsDnlinkEffect);
        if (status == TMS_RESULT_SUCCESS)
            {
            static_cast<TMSVolumeEffect*> (iTmsDnlinkEffect)->AddObserver(
                    *this, NULL);
            }
        }
    return status;
    }

gint CTmsTestEngine::AddVolumeEffectToStream()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iTmsDnlink && iTmsDnlinkEffect)
        {
        status = iTmsDnlink->AddEffect(iTmsDnlinkEffect);
        }
    return status;
    }

gint CTmsTestEngine::CreateGainEffect()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory && iTmsUplink && !iTmsUplinkEffect)
        {
        status = iFactory->CreateEffect(TMS_EFFECT_GAIN, iTmsUplinkEffect);
        if (status == TMS_RESULT_SUCCESS)
            {
            static_cast<TMSGainEffect*> (iTmsUplinkEffect)->AddObserver(*this,
                    NULL);
            }
        }
    return status;
    }

gint CTmsTestEngine::CreateGlobalRouting()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory)
        {
        status = iFactory->CreateGlobalRouting(iTmsGlobalRouting);
        if (status == TMS_RESULT_SUCCESS)
            {
            DisplayText("TMS routing created");
            iTmsGlobalRouting->AddObserver(*this, NULL);
            }
        else
            {
            DisplayText("Global routing failed: ", status);
            }
        }
    return status;
    }

gint CTmsTestEngine::AddGainEffectToStream()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iTmsUplink && iTmsUplinkEffect)
        {
        status = iTmsUplink->AddEffect(iTmsUplinkEffect);
        }
    return status;
    }

gint CTmsTestEngine::CreateRingTonePlayer()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory)
        {
        status = iFactory->CreateRingTonePlayer(iTmsRingTonePlayer);

        if (iTmsRingTonePlayer && status == TMS_RESULT_SUCCESS)
            {
            iTmsRingTonePlayer->AddObserver(*this, NULL);
            DisplayText("RingTone Player created");
            }
        else
            {
            DisplayText("RT create failed: ", status);
            }
        }
    return status;
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::ConfigureDecoder()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::ConfigureDecoder()
    {
    // NOTE: These calls can ONLY be made when codec is in a STOPPED state.

    switch (iDnLinkCodec)
        {
        case TMS_FORMAT_G711:
            {
            gboolean cng(TRUE);
            static_cast<TMSG711Format*> (iDecFormatIntfc)->SetCNG(cng);
            static_cast<TMSG711Format*> (iDecFormatIntfc)->GetCNG(cng);
            gboolean plc(FALSE);
            static_cast<TMSG711Format*> (iDecFormatIntfc)->SetPlc(plc);
            static_cast<TMSG711Format*> (iDecFormatIntfc)->GetPlc(plc);
            TMSG711CodecMode mode(TMS_G711_CODEC_MODE_ALAW);
            static_cast<TMSG711Format*> (iDecFormatIntfc)->SetMode(mode);
            static_cast<TMSG711Format*> (iDecFormatIntfc)->GetMode(mode);
            break;
            }
        case TMS_FORMAT_ILBC:
            {
            gboolean cng(TRUE);
            static_cast<TMSILBCFormat*> (iDecFormatIntfc)->SetCNG(cng);
            static_cast<TMSILBCFormat*> (iDecFormatIntfc)->GetCNG(cng);
            TMSILBCCodecMode mode(TMS_ILBC_CODEC_MODE_20MS_FRAME);
            static_cast<TMSILBCFormat*> (iDecFormatIntfc)->SetMode(mode);
            static_cast<TMSILBCFormat*> (iDecFormatIntfc)->GetMode(mode);
            break;
            }
        case TMS_FORMAT_G729:
        case TMS_FORMAT_AMR:
        case TMS_FORMAT_PCM:
        default:
            {
            break;
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetMaxVolume()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetMaxVolume()
    {
    if (iTmsDnlinkEffect)
        {
        static_cast<TMSVolumeEffect*> (iTmsDnlinkEffect)->GetMaxLevel(
                iMaxVolume);
        }
    DisplayText("Max Volume: ", iMaxVolume);
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetVolume
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetVolume()
    {
    if (iTmsDnlinkEffect)
        {
        static_cast<TMSVolumeEffect*> (iTmsDnlinkEffect)->GetLevel(iVolume);
        }
    DisplayText("Volume ", iVolume);
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::MuteSpeaker()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::MuteSpeaker()
    {
    SetVolume(0);
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetMaxVolume
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetMaxVolume()
    {
    SetVolume(iMaxVolume);
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetVolume
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetVolume(guint aVolume)
    {
    iVolume = aVolume;

    if (iTmsDnlinkEffect)
        {
        static_cast<TMSVolumeEffect*> (iTmsDnlinkEffect)->SetLevel(aVolume);
        }
    else
        {
        DisplayText("Create Dnlink VolumeEffect first");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::VolumeUp()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::VolumeUp()
    {
    if ((iDnLinkStatus == EReady || iDnLinkStatus == EStreaming)
            && iTmsDnlinkEffect)
        {
        if (iVolume < iMaxVolume)
            {
            static_cast<TMSVolumeEffect*> (iTmsDnlinkEffect)->SetLevel(
                    ++iVolume);
            }
        }
    else
        {
        DisplayText("Create DNL first");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::VolumeDn()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::VolumeDn()
    {
    if ((iDnLinkStatus == EReady || iDnLinkStatus == EStreaming)
            && iTmsDnlinkEffect)
        {
        if (iVolume > 0)
            {
            static_cast<TMSVolumeEffect*> (iTmsDnlinkEffect)->SetLevel(
                    --iVolume);
            }
        }
    else
        {
        DisplayText("Open DNL first");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::OpenUplink()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::OpenUplink()
    {
    gint status = TMS_RESULT_SUCCESS;

    if (iTmsUplink)
        {
        iTmsUplink->AddObserver(*this, NULL);
        status = iTmsUplink->Init(); //retry for ~3 sec
        if (status != TMS_RESULT_SUCCESS)
            {
            DisplayText("UPL init error: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::StartUplink()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::StartUplink()
    {
    if (iUpLinkStatus == EReady)
        {
#ifdef __TEST_CODE_COVERAGE__
        ConfigureEncoder();
        ToggleVad();
        GetVad();
        GetSupportedBitrates();
        SelectMaxBitrate();
        GetBitrate();
        gint id = iTmsUplink->GetStreamId();
#endif //__TEST_CODE_COVERAGE__

        iTmsUplink->Start(4); //retry for ~4 sec
        iUpLinkStatus = EStreaming;
        }
    else
        {
        DisplayText("UPL not ready");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::StopUplink()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::StopUplink()
    {
    if (iUpLinkStatus == EStreaming)
        {
        iTmsUplink->Stop();
        iUpLinkStatus = EReady;
        iOneTouchLoopback = EFalse;
        DisplayText("UPL stopped");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::CloseUplink()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::CloseUplink()
    {
    iRecBufReady = EFalse;
    iOneTouchLoopback = EFalse;

    if (iTmsUplink && iUpLinkStatus != ENotReady)
        {
        if (iTmsUplinkEffect)
            {
            iTmsUplink->RemoveEffect(iTmsUplinkEffect);
            }
        if (iTmsMicSource)
            {
            iTmsUplink->RemoveSource(iTmsMicSource);
            }
        if (iTmsClientSink)
            {
            iTmsUplink->RemoveSink(iTmsClientSink);
            }
        if (iTmsModemSink)
            {
            iTmsUplink->RemoveSink(iTmsModemSink);
            }
        iTmsUplink->Deinit();
        iUpLinkStatus = ENotReady;
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetUplinkFormat()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetUplinkFormat(TMSFormatType aCodecFormat)
    {
    gint status(TMS_RESULT_SUCCESS);

    if (!iTmsCall)
        {
        SetCallType(TMS_CALL_IP);
        }
    if (iFactory && iEncFormatIntfc)
        {
        iFactory->DeleteFormat(iEncFormatIntfc);
        }

    iUpLinkCodec = aCodecFormat;

    if (iFactory && iTmsUplink)
        {
        status = iFactory->CreateFormat(iUpLinkCodec, iEncFormatIntfc);
        status |= iTmsUplink->SetFormat(iEncFormatIntfc);
        }

#ifdef __TEST_CODE_COVERAGE__
    TMSFormatType ft;
#endif //__TEST_CODE_COVERAGE__

    switch (iUpLinkCodec)
        {
        case TMS_FORMAT_G711:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iEncFormatIntfc->GetType(ft);
            if (ft != TMS_FORMAT_G711)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("G.711 UPL Codec Set");
            break;
            }
        case TMS_FORMAT_G729:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iEncFormatIntfc->GetType(ft);
            if (ft != TMS_FORMAT_G729)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("G.729 UPL Codec Set");
            break;
            }
        case TMS_FORMAT_ILBC:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iEncFormatIntfc->GetType(ft);
            if (ft != TMS_FORMAT_ILBC)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("iLBC UPL Codec Set");
            break;
            }
        case TMS_FORMAT_AMR:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iEncFormatIntfc->GetType(ft);
            if (ft != TMS_FORMAT_AMR)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("AMR-NB UPL Codec Set");
            break;
            }
        case TMS_FORMAT_PCM:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iEncFormatIntfc->GetType(ft);
            if (ft != TMS_FORMAT_PCM)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("PCM-16 UPL Codec Set");
            break;
            }
        default:
            {
            status = KErrNotSupported;
            }
        }

    if (status == TMS_RESULT_SUCCESS)
        {
        OpenUplink(); //Initialize UPL
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::ConfigureEncoder()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::ConfigureEncoder()
    {
    // Any of these calls can ONLY be made when encoder is in a STOPPED state.

    switch (iUpLinkCodec)
        {
        case TMS_FORMAT_G711:
            {
            TMSG711CodecMode mode(TMS_G711_CODEC_MODE_ALAW);
            static_cast<TMSG711Format*> (iEncFormatIntfc)->SetMode(mode);
            static_cast<TMSG711Format*> (iEncFormatIntfc)->GetMode(mode);
            break;
            }
        case TMS_FORMAT_ILBC:
            {
            TMSILBCCodecMode mode(TMS_ILBC_CODEC_MODE_20MS_FRAME);
            static_cast<TMSILBCFormat*> (iEncFormatIntfc)->SetMode(mode);
            static_cast<TMSILBCFormat*> (iEncFormatIntfc)->GetMode(mode);
            break;
            }
        case TMS_FORMAT_G729:
        case TMS_FORMAT_AMR:
        case TMS_FORMAT_PCM:
        default:
            {
            break;
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetMaxGain()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetMaxGain()
    {
    if (iTmsUplinkEffect)
        {
        static_cast<TMSGainEffect*> (iTmsUplinkEffect)->GetMaxLevel(iMaxGain);
        DisplayText("MaxGain: ", iMaxGain);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetGain()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetGain()
    {
    guint gain;
    if (iTmsUplinkEffect)
        {
        static_cast<TMSGainEffect*> (iTmsUplinkEffect)->GetLevel(gain);
        DisplayText("Gain: ", gain);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetMaxGain()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetMaxGain()
    {
    static_cast<TMSGainEffect*> (iTmsUplinkEffect)->SetLevel(iMaxGain);
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::MuteMic()
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::MuteMic()
    {
    static_cast<TMSGainEffect*> (iTmsUplinkEffect)->SetLevel(0);
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetSupportedBitrates
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetSupportedBitrates()
    {
    TInt status = GetSupportedBitrates(iBitratesVector);

    if (status == TMS_RESULT_SUCCESS)
        {
        std::vector<guint>::iterator itBitrates = iBitratesVector.begin();
        for (; itBitrates < iBitratesVector.end(); itBitrates++)
            {
            DisplayText("BR ", *itBitrates);
            }
        }
    else
        {
        DisplayText("BR Error: ", status);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetSupportedBitrates
//
// ----------------------------------------------------------------------------
//
TInt CTmsTestEngine::GetSupportedBitrates(BitRateVector& aBrArr)
    {
    TInt status = KErrNotFound;

    switch (iUpLinkCodec)
        {
        case TMS_FORMAT_G711:
            status = static_cast<TMSG711Format*>
                    (iEncFormatIntfc)->GetSupportedBitRates(aBrArr);
            break;
        case TMS_FORMAT_G729:
            status = static_cast<TMSG729Format*>
                    (iEncFormatIntfc)->GetSupportedBitRates(aBrArr);
            break;
        case TMS_FORMAT_ILBC:
            status = static_cast<TMSILBCFormat*>
                    (iEncFormatIntfc)->GetSupportedBitRates(aBrArr);
            break;
        case TMS_FORMAT_AMR:
            status = static_cast<TMSAMRFormat*>
                    (iEncFormatIntfc)->GetSupportedBitRates(aBrArr);
            break;
        case TMS_FORMAT_PCM:
        default:
            status = KErrNotSupported;
            break;
        }
    return status;
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SelectMinBitrate
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SelectMinBitrate()
    {
    if (iBitratesVector.size() > 0)
        {
        std::vector<guint>::iterator bitrate = iBitratesVector.begin();
        iBitrate = *bitrate;
        SetBitrate(iBitrate);
        DisplayText("BR set: ", iBitrate);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SelectMaxBitrate
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SelectMaxBitrate()
    {
    if (iBitratesVector.size() > 0)
        {
        iBitrate = iBitratesVector.back();
        SetBitrate(iBitrate);
        DisplayText("BR set: ", iBitrate);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetBitrate
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetBitrate(TUint aBitrate)
    {
    switch (iUpLinkCodec)
        {
        case TMS_FORMAT_G711:
            static_cast<TMSG711Format*> (iEncFormatIntfc)->SetBitRate(aBitrate);
            DisplayText("Set BR: ", iBitrate);
            break;
        case TMS_FORMAT_G729:
            static_cast<TMSG729Format*> (iEncFormatIntfc)->SetBitRate(aBitrate);
            DisplayText("Set BR: ", iBitrate);
            break;
        case TMS_FORMAT_ILBC:
            static_cast<TMSILBCFormat*> (iEncFormatIntfc)->SetBitRate(aBitrate);
            DisplayText("Set BR: ", iBitrate);
            break;
        case TMS_FORMAT_AMR:
            static_cast<TMSAMRFormat*> (iEncFormatIntfc)->SetBitRate(aBitrate);
            DisplayText("Set BR: ", iBitrate);
            break;
        case TMS_FORMAT_PCM:
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetBitrate
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetBitrate()
    {
    if (iUpLinkCodec != TMS_FORMAT_PCM)
        {
        GetBitrate(iBitrate);
        DisplayText("Current BR: ", iBitrate);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetBitrate
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetBitrate(TUint& aBitrate)
    {
    switch (iUpLinkCodec)
        {
        case TMS_FORMAT_G711:
            static_cast<TMSG711Format*> (iEncFormatIntfc)->GetBitRate(aBitrate);
            break;
        case TMS_FORMAT_G729:
            static_cast<TMSG729Format*> (iEncFormatIntfc)->GetBitRate(aBitrate);
            break;
        case TMS_FORMAT_ILBC:
            static_cast<TMSILBCFormat*> (iEncFormatIntfc)->GetBitRate(aBitrate);
            break;
        case TMS_FORMAT_AMR:
            static_cast<TMSAMRFormat*> (iEncFormatIntfc)->GetBitRate(aBitrate);
            break;
        case TMS_FORMAT_PCM:
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::ToggleVad
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::ToggleVad()
    {
    iVad = (iVad) ? EFalse : ETrue;

    switch (iUpLinkCodec)
        {
        case TMS_FORMAT_G711:
            static_cast<TMSG711Format*> (iEncFormatIntfc)->SetVADMode(iVad);
            DisplayText("Set VAD: ", iVad);
            break;
        case TMS_FORMAT_G729:
            static_cast<TMSG729Format*> (iEncFormatIntfc)->SetVADMode(iVad);
            DisplayText("Set VAD: ", iVad);
            break;
        case TMS_FORMAT_ILBC:
            static_cast<TMSILBCFormat*> (iEncFormatIntfc)->SetVADMode(iVad);
            DisplayText("Set VAD: ", iVad);
            break;
        case TMS_FORMAT_AMR:
            //static_cast<TMSAMRFormat*> (iEncFormatIntfc)->SetVADMode(iVad);
            //DisplayText("Set VAD: ", iVad);
            break;
        case TMS_FORMAT_PCM:
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetVad
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetVad()
    {
    switch (iUpLinkCodec)
        {
        case TMS_FORMAT_G711:
            static_cast<TMSG711Format*> (iEncFormatIntfc)->GetVADMode(iVad);
            DisplayText("Current VAD: ", iVad);
            break;
        case TMS_FORMAT_G729:
            static_cast<TMSG729Format*> (iEncFormatIntfc)->GetVADMode(iVad);
            DisplayText("Current VAD: ", iVad);
            break;
        case TMS_FORMAT_ILBC:
            static_cast<TMSILBCFormat*> (iEncFormatIntfc)->GetVADMode(iVad);
            DisplayText("Current VAD: ", iVad);
            break;
        case TMS_FORMAT_AMR:
            //static_cast<TMSAMRFormat*> (iEncFormatIntfc)->GetVADMode(iVad);
            //DisplayText("Current VAD: ", iVad);
            break;
        case TMS_FORMAT_PCM:
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::ToggleCng
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::ToggleCng()
    {
    iCng = (iCng) ? EFalse : ETrue;

    switch (iDnLinkCodec)
        {
        case TMS_FORMAT_G711:
            static_cast<TMSG711Format*> (iDecFormatIntfc)->SetCNG(iCng);
            DisplayText("Set CNG ", iCng);
            break;
        case TMS_FORMAT_ILBC:
            static_cast<TMSILBCFormat*> (iDecFormatIntfc)->SetCNG(iCng);
            DisplayText("Set CNG ", iCng);
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetCng
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetCng()
    {
    switch (iDnLinkCodec)
        {
        case TMS_FORMAT_G711:
            static_cast<TMSG711Format*> (iDecFormatIntfc)->GetCNG(iCng);
            DisplayText("Current CNG ", iCng);
            break;
        case TMS_FORMAT_ILBC:
            static_cast<TMSILBCFormat*> (iDecFormatIntfc)->GetCNG(iCng);
            DisplayText("Current CNG ", iCng);
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::TogglePlc
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::TogglePlc()
    {
    iPlc = (iPlc) ? EFalse : ETrue;
    if (iDnLinkCodec == TMS_FORMAT_G711)
        {
        static_cast<TMSG711Format*> (iDecFormatIntfc)->SetPlc(ETrue);
        DisplayText("Set PLC ", iPlc);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetPlc
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetPlc()
    {
    if (iDnLinkCodec == TMS_FORMAT_G711)
        {
        static_cast<TMSG711Format*> (iDecFormatIntfc)->GetPlc(iPlc);
        DisplayText("Current PLC ", iPlc);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetDnLinkG711ALAW
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetDnLinkG711ALAW()
    {
    if (iDnLinkCodec == TMS_FORMAT_G711)
        {
        static_cast<TMSG711Format*> (iDecFormatIntfc)->SetMode(
                TMS_G711_CODEC_MODE_ALAW);
        DisplayText("DNL G.711 Mode Set: [aLaw]");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetDnLinkG711uLAW
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetDnLinkG711uLAW()
    {
    if (iDnLinkCodec == TMS_FORMAT_G711)
        {
        static_cast<TMSG711Format*> (iDecFormatIntfc)->SetMode(
                TMS_G711_CODEC_MODE_MULAW);
        DisplayText("DNL G.711 Mode Set: [uLaw]");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetDnLinkG711Mode
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetDnLinkG711Mode()
    {
    if (iDnLinkCodec == TMS_FORMAT_G711)
        {
        TMSG711CodecMode mode;
        TInt status = static_cast<TMSG711Format*> (iDecFormatIntfc)->GetMode(
                mode);

        if (status == TMS_RESULT_SUCCESS)
            {
            if (mode == TMS_G711_CODEC_MODE_MULAW)
                {
                DisplayText("DNL G711 Mode: [uLaw]");
                }
            else
                {
                DisplayText("DNL G711 Mode: [aLaw]");
                }
            }
        else
            {
            DisplayText("DNL G711 GetMode Error: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetDnLinkILBC20MS
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetDnLinkILBC20MS()
    {
    if (iDnLinkCodec == TMS_FORMAT_ILBC)
        {
        static_cast<TMSILBCFormat*> (iDecFormatIntfc)->SetMode(
                TMS_ILBC_CODEC_MODE_20MS_FRAME);
        DisplayText("DNL iLBC Mode Set: [20ms]");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetDnLinkILBC30MS
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetDnLinkILBC30MS()
    {
    if (iDnLinkCodec == TMS_FORMAT_ILBC)
        {
        static_cast<TMSILBCFormat*> (iDecFormatIntfc)->SetMode(
                TMS_ILBC_CODEC_MODE_30MS_FRAME);
        DisplayText("DNL iLBC Mode Set: [30ms]");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetDnLinkILBCMode
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetDnLinkILBCMode()
    {
    if (iDnLinkCodec == TMS_FORMAT_ILBC)
        {
        TMSILBCCodecMode mode;
        gint status = static_cast<TMSILBCFormat*> (iDecFormatIntfc)->GetMode(
                mode);

        if (status == TMS_RESULT_SUCCESS)
            {
            if (mode == TMS_ILBC_CODEC_MODE_30MS_FRAME)
                {
                DisplayText("DNL iLBC Mode: [30ms]");
                }
            else if (mode == TMS_ILBC_CODEC_MODE_20MS_FRAME)
                {
                DisplayText("DNL iLBC Mode: [20ms]");
                }
            }
        else
            {
            DisplayText("DNL iLBC GetMode Error: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetUpLinkG711ALAW
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetUpLinkG711ALAW()
    {
    if (iUpLinkCodec == TMS_FORMAT_G711)
        {
        static_cast<TMSG711Format*> (iEncFormatIntfc)->SetMode(
                TMS_G711_CODEC_MODE_ALAW);
        DisplayText("UPL G.711 Mode Set: [aLaw]");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetUpLinkG711uLAW
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetUpLinkG711uLAW()
    {

    if (iUpLinkCodec == TMS_FORMAT_G711)
        {
        static_cast<TMSG711Format*> (iEncFormatIntfc)->SetMode(
                TMS_G711_CODEC_MODE_MULAW);
        DisplayText("UPL G.711 Mode Set: [uLaw]");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetUpLinkG711Mode
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetUpLinkG711Mode()
    {
    gint status(TMS_RESULT_SUCCESS);
    if (iUpLinkCodec == TMS_FORMAT_G711)
        {
        TMSG711CodecMode mode;
        status = static_cast<TMSG711Format*> (iEncFormatIntfc)->GetMode(mode);

        if (status == TMS_RESULT_SUCCESS)
            {
            if (mode == TMS_G711_CODEC_MODE_MULAW)
                {
                DisplayText("UPL G.711 Mode: [uLaw]");
                }
            else if (mode == TMS_G711_CODEC_MODE_ALAW)
                {
                DisplayText("UPL G.711 Mode: [aLaw]");
                }
            }
        else
            {
            DisplayText("UPL G.711 GetMode Error: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetUpLinkILBC20MS
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetUpLinkILBC20MS()
    {
    if (iUpLinkCodec == TMS_FORMAT_ILBC)
        {
        static_cast<TMSILBCFormat*> (iEncFormatIntfc)->SetMode(
                TMS_ILBC_CODEC_MODE_20MS_FRAME);
        DisplayText("UPL iLBC Mode Set: [20ms]");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetUpLinkILBC30MS
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetUpLinkILBC30MS()
    {
    if (iUpLinkCodec == TMS_FORMAT_ILBC)
        {
        static_cast<TMSILBCFormat*> (iEncFormatIntfc)->SetMode(
                TMS_ILBC_CODEC_MODE_30MS_FRAME);
        DisplayText("UPL iLBC Mode Set: [30ms]");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetUpLinkILBCMode
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetUpLinkILBCMode()
    {
    gint status(TMS_RESULT_SUCCESS);
    if (iUpLinkCodec == TMS_FORMAT_ILBC)
        {
        TMSILBCCodecMode mode;
        status = static_cast<TMSILBCFormat*> (iEncFormatIntfc)->GetMode(mode);

        if (status == TMS_RESULT_SUCCESS)
            {
            if (mode == TMS_ILBC_CODEC_MODE_30MS_FRAME)
                {
                DisplayText("UPL iLBC Mode: [30ms]");
                }
            else if (mode == TMS_ILBC_CODEC_MODE_20MS_FRAME)
                {
                DisplayText("UPL iLBC Mode: [20ms]");
                }
            }
        else
            {
            DisplayText("UPL iLBC GetMode Error: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::CreateGlobalVol
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::CreateGlobalVol()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory && !iGlobalVol)
        {
        status = iFactory->CreateEffect(TMS_EFFECT_GLOBAL_VOL, iGlobalVol);

        if (status == TMS_RESULT_SUCCESS)
            {
            DisplayText("Global Vol Created");
            static_cast<TMSGlobalVolEffect*> (iGlobalVol)->AddObserver(*this,
                    NULL);
            }
        else
            {
            DisplayText("Global Vol failed: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::CreateGlobalGain
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::CreateGlobalGain()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory && !iGlobalGain)
        {
        status = iFactory->CreateEffect(TMS_EFFECT_GLOBAL_GAIN, iGlobalGain);

        if (status == TMS_RESULT_SUCCESS)
            {
            DisplayText("Global Gain Created");
            static_cast<TMSGlobalGainEffect*> (iGlobalGain)->AddObserver(*this,
                    NULL);
            }
        else
            {
            DisplayText("Global Gain failed: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetGlobalVol
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetGlobalVol()
    {
    if (iGlobalVol)
        {
        TUint level(0);
        static_cast<TMSGlobalVolEffect*> (iGlobalVol)->GetLevel(level);
        DisplayText("Global Vol: ", level);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetGlobalMaxVol
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetGlobalMaxVol()
    {
    if (iGlobalVol)
        {
        TUint level(0);
        static_cast<TMSGlobalVolEffect*> (iGlobalVol)->GetMaxLevel(level);
        DisplayText("Global Max Vol: ", level);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetGlobalVol
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetGlobalVol()
    {
    TUint level(0);
    if (iGlobalVol)
        {
        static_cast<TMSGlobalVolEffect*> (iGlobalVol)->GetMaxLevel(level);
        static_cast<TMSGlobalVolEffect*> (iGlobalVol)->SetLevel(level);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetGlobalGain
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetGlobalGain()
    {
    if (iGlobalGain)
        {
        TUint level(0);
        static_cast<TMSGlobalGainEffect*> (iGlobalGain)->GetLevel(level);
        DisplayText("Global Gain: ", level);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetGlobalMaxGain
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetGlobalMaxGain()
    {
    if (iGlobalGain)
        {
        TUint level(0);
        static_cast<TMSGlobalGainEffect*> (iGlobalGain)->GetMaxLevel(level);
        DisplayText("Global Max gain: ", level);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetGlobalGain
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetGlobalGain()
    {
    TUint level(0);
    if (iGlobalGain)
        {
        static_cast<TMSGlobalGainEffect*> (iGlobalGain)->GetMaxLevel(level);
        static_cast<TMSGlobalGainEffect*> (iGlobalGain)->SetLevel(level);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetOutputDevice
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetOutputDevice(TMSAudioOutput device)
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iTmsGlobalRouting)
        {
        switch (device)
            {
            case TMS_AUDIO_OUTPUT_NONE:
                status = iTmsGlobalRouting->SetOutput(TMS_AUDIO_OUTPUT_NONE);
                DisplayText("Routing none");
                break;
            case TMS_AUDIO_OUTPUT_PUBLIC:
                status = iTmsGlobalRouting->SetOutput(TMS_AUDIO_OUTPUT_PUBLIC);
                DisplayText("Routing public");
                break;
            case TMS_AUDIO_OUTPUT_PRIVATE:
                status = iTmsGlobalRouting->SetOutput(TMS_AUDIO_OUTPUT_PRIVATE);
                DisplayText("Routing private");
                break;
            case TMS_AUDIO_OUTPUT_HANDSET:
                status = iTmsGlobalRouting->SetOutput(TMS_AUDIO_OUTPUT_HANDSET);
                DisplayText("Routing to handset");
                break;
            case TMS_AUDIO_OUTPUT_LOUDSPEAKER:
                status = iTmsGlobalRouting->SetOutput(
                        TMS_AUDIO_OUTPUT_LOUDSPEAKER);
                DisplayText("Routing to Loudspeaker");
                break;
            case TMS_AUDIO_OUTPUT_WIRED_ACCESSORY:
                status = iTmsGlobalRouting->SetOutput(
                        TMS_AUDIO_OUTPUT_WIRED_ACCESSORY);
                DisplayText("Routing to Wired accessory");
                break;
            case TMS_AUDIO_OUTPUT_ACCESSORY:
                status = iTmsGlobalRouting->SetOutput(
                        TMS_AUDIO_OUTPUT_ACCESSORY);
                DisplayText("Routing to BT accessory");
                break;
            case TMS_AUDIO_OUTPUT_ETTY:
                status = iTmsGlobalRouting->SetOutput(TMS_AUDIO_OUTPUT_ETTY);
                DisplayText("Routing to TTY");
                break;
            default: // ENoPreference
                DisplayText("Default Device Routing");
                break;
            }
        }
    if (status != TMS_RESULT_SUCCESS)
        {
        DisplayText("Routing failed: ", status);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetHandset
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetHandset()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iTmsGlobalRouting)
        {
        status = iTmsGlobalRouting->SetOutput(TMS_AUDIO_OUTPUT_HANDSET);

        if (status != TMS_RESULT_SUCCESS)
            {
            DisplayText("SetHandSet failed: ", status);
            }
        else
            {
            DisplayText("SetHandSet");
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::SetLoudSpeaker
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::SetLoudSpeaker()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iTmsGlobalRouting)
        {
        status = iTmsGlobalRouting->SetOutput(TMS_AUDIO_OUTPUT_LOUDSPEAKER);

        if (status != TMS_RESULT_SUCCESS)
            {
            DisplayText("SetLoudSpeaker failed: ", status);
            }
        else
            {
            DisplayText("SetLoudSpeaker");
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetAudioDevice
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetAudioDevice()
    {
    TMSAudioOutput device;

    if (iTmsGlobalRouting)
        {
        iTmsGlobalRouting->GetOutput(device);
#ifdef __TEST_CODE_COVERAGE__
        TMSAudioOutput prevdevice;
        iTmsGlobalRouting->GetPreviousOutput(prevdevice);
#endif //__TEST_CODE_COVERAGE__

        switch (device)
            {
            case TMS_AUDIO_OUTPUT_NONE:
                DisplayText("Routing none");
                break;
            case TMS_AUDIO_OUTPUT_PUBLIC:
                DisplayText("Routing public");
                break;
            case TMS_AUDIO_OUTPUT_PRIVATE:
                DisplayText("Routing private");
                break;
            case TMS_AUDIO_OUTPUT_HANDSET:
                DisplayText("Routing to handset");
                break;
            case TMS_AUDIO_OUTPUT_LOUDSPEAKER:
                DisplayText("Routing to Loudspeaker");
                break;
            case TMS_AUDIO_OUTPUT_WIRED_ACCESSORY:
                DisplayText("Routing to Wired accessory");
                break;
            case TMS_AUDIO_OUTPUT_ACCESSORY:
                DisplayText("Routing to BT accessory");
                break;
            case TMS_AUDIO_OUTPUT_ETTY:
                DisplayText("Routing to TTY");
                break;
            default: // ENoPreference
                DisplayText("Default Device Routing");
                break;
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::GetAvailableOutput
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::GetAvailableOutput()
    {
    TInt status(TMS_RESULT_SUCCESS);

    if (iTmsGlobalRouting)
        {
        status = iTmsGlobalRouting->GetAvailableOutputs(iAvailableoutputs);

        if (status == TMS_RESULT_SUCCESS)
            {
            DisplayText("Available outputs: ", iAvailableoutputs.size());
            std::vector<guint>::iterator outputs = iAvailableoutputs.begin();
            for (; outputs < iAvailableoutputs.end(); outputs++)
                {
                DisplayDevice(*outputs);
                }
            }
        else
            {
            DisplayText("Available output error: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::DisplayDevice
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::DisplayDevice(TMSAudioOutput device)
    {
    switch (device)
        {
        case TMS_AUDIO_OUTPUT_NONE:
            DisplayText("none");
            break;
        case TMS_AUDIO_OUTPUT_PUBLIC:
            DisplayText("Public");
            break;
        case TMS_AUDIO_OUTPUT_PRIVATE:
            DisplayText("Private");
            break;
        case TMS_AUDIO_OUTPUT_HANDSET:
            DisplayText("Handset");
            break;
        case TMS_AUDIO_OUTPUT_LOUDSPEAKER:
            DisplayText("Loudspeaker");
            break;
        case TMS_AUDIO_OUTPUT_WIRED_ACCESSORY:
            DisplayText("Wired accessory");
            break;
        case TMS_AUDIO_OUTPUT_ACCESSORY:
            DisplayText("Accessory");
            break;
        case TMS_AUDIO_OUTPUT_ETTY:
            DisplayText("TTY");
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::InitDTMFTonePlayerDnlink
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::InitDTMFTonePlayerDnlink()
    {
    gint status(TMS_RESULT_SUCCESS);
    if (iFactory)
        {
        status = iFactory->CreateDTMF(TMS_STREAM_DOWNLINK, iDTMFTonePlayerDn);
        }
    if (iDTMFTonePlayerDn && status == TMS_RESULT_SUCCESS)
        {
        // Note: It is sufficient to register only 1 DTMF observer per client.
        // Since callbacks from UPL and DNL DTMF players are handled by the
        // same client, and callback mechanism doesn't distinguish between
        // UPL and DNL DTMF event, registering same client twice will result
        // in duplicated callbacks.
        if (iHasDTMFObserver == 0)
            {
            status = iDTMFTonePlayerDn->AddObserver(*this, NULL);
            iHasDTMFObserver++;
            }
        }
    if (status == TMS_RESULT_SUCCESS)
        {
        DisplayText("DTMF Downlink OK");
        }
    else
        {
        DisplayText("DTMF Downlink failed:", status);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::InitDTMFTonePlayerUplink
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::InitDTMFTonePlayerUplink()
    {
    gint status(TMS_RESULT_SUCCESS);
    if (iFactory)
        {
        status = iFactory->CreateDTMF(TMS_STREAM_UPLINK, iDTMFTonePlayerUp);
        }
    if (iDTMFTonePlayerUp && status == TMS_RESULT_SUCCESS)
        {
        // Note: It is sufficient to register only 1 DTMF observer per client.
        // Since callbacks from UPL and DNL DTMF players are handled by the
        // same client, and callback mechanism doesn't distinguish between
        // UPL and DNL DTMF event, registering same client twice will result
        // in duplicated callbacks.
        if (iHasDTMFObserver == 0)
            {
            status = iDTMFTonePlayerUp->AddObserver(*this, NULL);
            iHasDTMFObserver++;
            }
        }
    if (status == TMS_RESULT_SUCCESS)
        {
        DisplayText("DTMF Uplink OK");
        }
    else
        {
        DisplayText("DTMF Uplink failed: ", status);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::DTMFTonePlayDnlink
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::DTMFTonePlayDnlink()
    {
    GString* dtmfstring;
    gint status(TMS_RESULT_SUCCESS);
    if (iDTMFTonePlayerDn)
        {
        dtmfstring = g_string_new("4723");
        status = iDTMFTonePlayerDn->SetTone(dtmfstring);
        if (status == TMS_RESULT_SUCCESS)
            {
            status = iDTMFTonePlayerDn->Start();
            if (status != TMS_RESULT_SUCCESS)
                {
                DisplayText("DTMF downlink start failed: ", status);
                }
            }
        else
            {
            DisplayText("DTMF downlink settone failed: ", status);
            }
        g_string_free(dtmfstring, TRUE);
        }
    else
        {
        DisplayText("Downlink not ready");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::DTMFTonePlayUplink
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::DTMFTonePlayUplink()
    {
    GString* dtmfstring;
    gint status(TMS_RESULT_SUCCESS);
    // Note: uplink must be in the streaming state (disable for testing)
    if (iDTMFTonePlayerUp && iUpLinkStatus == EStreaming)
        {
        dtmfstring = g_string_new("4567890*#123");
        //dtmfstring = g_string_append_c(dtmfstring, '4');
        status = iDTMFTonePlayerUp->SetTone(dtmfstring);
        if (status == TMS_RESULT_SUCCESS)
            {
#ifdef __TEST_CODE_COVERAGE__
            // CS call only
            iDTMFTonePlayerUp->ContinueDTMFStringSending(TRUE);
#endif //__TEST_CODE_COVERAGE__

            status = iDTMFTonePlayerUp->Start();

            if (status != TMS_RESULT_SUCCESS)
                {
                DisplayText("DTMF uplink start failed: ", status);
                }
            }
        else
            {
            DisplayText("DTMF uplink settone failed: ", status);
            }
        g_string_free(dtmfstring, TRUE);
        }
    else
        {
        DisplayText("Uplink not ready");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::CloseDTMFPlayerDnlink
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::CloseDTMFPlayerDnlink()
    {
    if (iFactory && iDTMFTonePlayerDn)
        {
        iDTMFTonePlayerDn->Stop();
        iDTMFTonePlayerDn->RemoveObserver(*this);
        iHasDTMFObserver--;
        iFactory->DeleteDTMF(iDTMFTonePlayerDn);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::CloseDTMFPlayerUplink
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::CloseDTMFPlayerUplink()
    {
    if (iFactory && iDTMFTonePlayerUp)
        {
        iDTMFTonePlayerUp->Stop();
        iDTMFTonePlayerUp->RemoveObserver(*this);
        iHasDTMFObserver--;
        iFactory->DeleteDTMF(iDTMFTonePlayerUp);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::InitRingTonePlayerFromProfiles
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::InitRingTonePlayerFromProfiles()
    {
    if (iTmsRingTonePlayer)
        {
        gint status = iTmsRingTonePlayer->Init(TMS_RINGTONE_DEFAULT);
        DisplayText("RT Init Profile: ", status);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::InitRingTonePlayerFromFile
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::InitRingTonePlayerFromFile()
    {
    if (iTmsRingTonePlayer)
        {
        TBuf<sizeof(KTestFile1)> buf(KTestFile1);
        iRTStr = g_string_new_len((gchar*) buf.Ptr(), buf.Length() * 2);
        gint status = iTmsRingTonePlayer->Init(TMS_RINGTONE_FILE, iRTStr);
        DisplayText("RT Init File: ", status);
        g_string_free(iRTStr, TRUE);
        iRTStr = NULL;
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::InitRingToneVideoPlayer
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::InitRingToneVideoPlayer()
    {
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::InitRingToneSequencePlayer
// Creates sequence player to play custom sequence in descriptor format
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::InitRingToneSequencePlayer()
    {
    if (iTmsRingTonePlayer)
        {
        TBuf8<sizeof(KRTBeepSequence)> buf(KRTBeepSequence);
        iRTStr = g_string_new_len((gchar*) buf.Ptr(), buf.Length());
        gint status = iTmsRingTonePlayer->Init(TMS_RINGTONE_SEQUENCE, iRTStr);
        DisplayText("RT Init Sequence:", status);
        g_string_free(iRTStr, TRUE);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::InitRingToneBeepOnce
// Creates sequence player to play single beep
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::InitRingToneBeepOnce()
    {
    if (iTmsRingTonePlayer)
        {
        gint status = iTmsRingTonePlayer->Init(TMS_RINGTONE_BEEP_ONCE);
        DisplayText("RT Init BeepOnce: ", status);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::InitRingToneSilent
// Creates sequence player to play silent tone
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::InitRingToneSilent()
    {
    if (iTmsRingTonePlayer)
        {
        gint status = iTmsRingTonePlayer->Init(TMS_RINGTONE_SILENT);
        DisplayText("RT Init Silent: ", status);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::InitRingToneUnsecureVoIP
// Creates sequence player to play tone for unsecured VoIP call.
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::InitRingToneUnsecureVoIP()
    {
    if (iTmsRingTonePlayer)
        {
        gint status = iTmsRingTonePlayer->Init(TMS_RINGTONE_UNSECURE_VOIP);
        DisplayText("RT Init UnsecVoIP: ", status);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::InitRingToneWithTTS
// Creates sequence player to play default RT with Text-To-Speech
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::InitRingToneWithTTS()
    {
    if (iTmsRingTonePlayer)
        {
        TBuf<sizeof(KTextToSpeak)> buf(KTextToSpeak);
        iTTSStr = g_string_new_len((gchar*) buf.Ptr(), buf.Length() * 2);
        gint status = iTmsRingTonePlayer->Init(TMS_RINGTONE_DEFAULT, NULL,
                iTTSStr);
        DisplayText("RT Init TTS:", status);
        g_string_free(iTTSStr, TRUE);
        }
    iTTSStr = NULL;
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::PlayRingTone
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::PlayRingTone()
    {
    if (iTmsRingTonePlayer)
        {
        iTmsRingTonePlayer->Play();
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::StopRingTone
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::StopRingTone()
    {
    if (iTmsRingTonePlayer)
        {
        iTmsRingTonePlayer->Stop();
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::MuteRingTone
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::MuteRingTone()
    {
    if (iTmsRingTonePlayer)
        {
        iTmsRingTonePlayer->Mute();
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::PauseVideoRingTone
// Pause audio for video RT only
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::PauseVideoRingTone()
    {
    if (iTmsRingTonePlayer)
        {
        iTmsRingTonePlayer->Pause();
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::DeinitRingTonePlayer
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::DeinitRingTonePlayer()
    {
    if (iTmsRingTonePlayer)
        {
        iTmsRingTonePlayer->Deinit();
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::CloseRingTonePlayer
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::CloseRingTonePlayer()
    {
    if (iFactory && iTmsRingTonePlayer)
        {
        DeinitRingTonePlayer();
        iTmsRingTonePlayer->RemoveObserver(*this);
        iFactory->DeleteRingTonePlayer(iTmsRingTonePlayer);
        DisplayText("RT Player Closed");
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::CreateInbandTonePlayer
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::CreateInbandTonePlayer()
    {
    gint status(TMS_RESULT_SUCCESS);
    if (iFactory && !iInbandTonePlayer)
        {
        status = iFactory->CreateInbandTonePlayer(iInbandTonePlayer);
        if (iInbandTonePlayer && status == TMS_RESULT_SUCCESS)
            {
            iInbandTonePlayer->AddObserver(*this, NULL);
            DisplayText("Inband Tone Player created");
            }
        else
            {
            DisplayText("Inband Tone Player failed: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::StartInbandTone
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::StartInbandTone(TMSInbandToneType inbandtone)
    {
    gint status(TMS_RESULT_SUCCESS);
    CreateInbandTonePlayer();
    if (iFactory && iInbandTonePlayer)
        {
        status = iInbandTonePlayer->Start(inbandtone);
        if (status == TMS_RESULT_SUCCESS)
            {
            DisplayText("Inband Tone Player Start");
            }
        else
            {
            DisplayText("Inband tone failed: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::StopInbandTone
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::StopInbandTone()
    {
    gint status(TMS_RESULT_SUCCESS);
    if (iFactory && iInbandTonePlayer)
        {
        status = iInbandTonePlayer->Stop();
        if (status == TMS_RESULT_SUCCESS)
            {
            DisplayText("Inband Tone Player Stop");
            }
        else
            {
            DisplayText("Inband tone failed: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::OneTouchLoopback
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::OneTouchLoopback()
    {
    iOneTouchLoopback = ETrue;

    // Change between CS and IP call type for testing.
    //SetCallType(TMS_CALL_CS); //will activate streams
    SetCallType(TMS_CALL_IP);

    if (iTmsCall->GetCallType() == TMS_CALL_IP)
        {
#ifdef __WINSCW__
        SetDownlinkFormat(TMS_FORMAT_PCM);
        SetUplinkFormat(TMS_FORMAT_PCM);
#else  //__WINSCW__
        SetDownlinkFormat(TMS_FORMAT_AMR);
        SetUplinkFormat(TMS_FORMAT_AMR);
#endif //__WINSCW__
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::DoLoopback
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::DoLoopback()
    {
    if (iPlayBufReady && iRecBufReady)
        {
        guint8* srcptr(NULL);
        guint8* desptr(NULL);
        guint srcsize(0);
        guint dessize(0);

        iPlayBuf->GetDataPtr(desptr);
        iPlayBuf->GetDataSize(dessize);
        iRecBuf->GetDataPtr(srcptr);
        iRecBuf->GetDataSize(srcsize);

#ifdef __WINSCW__
        // This is the case when the size of the play buffer is larger than
        // the size of the recorded buffer. In WINS, DirectX views partially
        // filled buffers as an EOS and throws (-10). So, we will collect
        // multiple buffers here.
        Mem::Copy(desptr + iBufIndex, srcptr, srcsize);
        iBufIndex += srcsize;
        if (iBufIndex >= dessize)
            {
            iPlayBuf->SetDataSize(dessize);
            static_cast<TMSClientSource*> (iTmsClientSource)->BufferFilled(
                    *iPlayBuf);
            iPlayBufReady = EFalse; // buf filled, ready for next FillBuffer
            iBufIndex = 0;
            }
#else //__WINSCW__
        Mem::Copy(desptr, srcptr, srcsize);
        iPlayBuf->SetDataSize(srcsize);
        static_cast<TMSClientSource*> (iTmsClientSource)->BufferFilled(
                *iPlayBuf);
        iPlayBufReady = EFalse; // buf filled, ready for FillBuffer
#endif //__WINSCW__
        iRecBufReady = EFalse; // buf consumed, ready for next EmptyBuffer
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::EndCall
//
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::EndCall()
    {
    StopUplink();
    StopDownlink();
    CloseUplink();
    CloseDownlink();
    CloseDTMFPlayerUplink();
    CloseDTMFPlayerDnlink();

    if (iTmsUplink && iTmsCall)
        {
        iTmsCall->DeleteStream(iTmsUplink);
        }
    if (iTmsDnlink && iTmsCall)
        {
        iTmsCall->DeleteStream(iTmsDnlink);
        }
    if (iFactory && iTmsCall)
        {
        iFactory->DeleteCall(iTmsCall);
        }
    if (iFactory && iTmsClientSource)
        {
        if (iCallType == TMS_CALL_IP)
            {
            static_cast<TMSClientSource*> (iTmsClientSource)->RemoveObserver(
                    *this);
            }
        iFactory->DeleteSource(iTmsClientSource);
        }
    if (iFactory && iTmsMicSource)
        {
        iFactory->DeleteSource(iTmsMicSource);
        }
    if (iFactory && iTmsModemSource)
        {
        iFactory->DeleteSource(iTmsModemSource);
        }
    if (iFactory && iTmsClientSink)
        {
        if (iCallType == TMS_CALL_IP)
            {
            static_cast<TMSClientSink*> (iTmsClientSink)->RemoveObserver(*this);
            }
        iFactory->DeleteSink(iTmsClientSink);
        }
    if (iFactory && iTmsSpeakerSink)
        {
        iFactory->DeleteSink(iTmsSpeakerSink);
        }
    if (iFactory && iTmsModemSink)
        {
        iFactory->DeleteSink(iTmsModemSink);
        }
    if (iFactory && iTmsDnlinkEffect)
        {
        static_cast<TMSVolumeEffect*> (iTmsDnlinkEffect)->RemoveObserver(*this);
        iFactory->DeleteEffect(iTmsDnlinkEffect);
        }
    if (iFactory && iTmsUplinkEffect)
        {
        static_cast<TMSGainEffect*> (iTmsUplinkEffect)->RemoveObserver(*this);
        iFactory->DeleteEffect(iTmsUplinkEffect);
        }
    if (iFactory && iEncFormatIntfc)
        {
        iFactory->DeleteFormat(iEncFormatIntfc);
        }
    if (iFactory && iDecFormatIntfc)
        {
        iFactory->DeleteFormat(iDecFormatIntfc);
        }
    if (iFactory && iDTMFTonePlayerDn)
        {
        iFactory->DeleteDTMF(iDTMFTonePlayerDn);
        }
    if (iFactory && iDTMFTonePlayerUp)
        {
        iFactory->DeleteDTMF(iDTMFTonePlayerUp);
        }
    }

#ifdef __RECORD_WAV_TO_FILE__
void CTmsTestEngine::WriteToFile(const guint8* str, const guint len)
    {
    TPtrC8 ptr(str, len);
    TPtr8 p = iWriteBuf->Des();
    p.Copy(ptr);
    iFile.Write(p);
    }
#endif //__RECORD_WAV_TO_FILE__

// CALLBACKS

void CTmsTestEngine::TMSStreamEvent(const TMSStream& stream,
        TMSSignalEvent event)
    {
    switch (const_cast<TMSStream&> (stream).GetStreamType())
        {
        case TMS_STREAM_UPLINK:
            {
            switch (event.type)
                {
                case TMS_EVENT_STREAM_STATE_CHANGED:
                    {
                    switch (event.curr_state)
                        {
                        case TMS_STREAM_INITIALIZED:
                            {
#ifdef __PROFILING_ENABLED__
                            TAG_CALLBACK_TIME_PROFILING_END;
                            PRINT_CALLBACK_TIME_LATENCY;
#endif //__PROFILING_ENABLED__
                            iUpLinkStatus = EReady;
                            DisplayText("Uplink initialized ");
                            if (iOneTouchLoopback)
                                {
                                StartUplink();
                                }
                            break;
                            }
                        case TMS_STREAM_UNINITIALIZED:
                            iTmsUplink->RemoveObserver(*this);
                            DisplayText("Uplink uninitialized");
                            break;
                        case TMS_STREAM_PAUSED:
                            DisplayText("Uplink paused");
                            break;
                        case TMS_STREAM_STARTED:
                            DisplayText("Uplink started");
                            break;
                        default:
                            break;
                        }
                    break;
                    }
                case TMS_EVENT_STREAM_STATE_CHANGE_ERROR:
                    DisplayText("Uplink Error ", event.reason);
                    break;
                default:
                    break;
                }
            break;
            }
        case TMS_STREAM_DOWNLINK:
            {
            switch (event.type)
                {
                case TMS_EVENT_STREAM_STATE_CHANGED:
                    {
                    switch (event.curr_state)
                        {
                        case TMS_STREAM_INITIALIZED:
                            {
#ifdef __PROFILING_ENABLED__
                            TAG_CALLBACK_TIME_PROFILING_END;
                            PRINT_CALLBACK_TIME_LATENCY;
                            TAG_CALLBACK_TIME_PROFILING_START;
#endif //__PROFILING_ENABLED__
                            GetMaxVolume();
                            SetVolume(iMaxVolume / 2);
                            iDnLinkStatus = EReady;
                            DisplayText("Downlink initialized");
                            if (iOneTouchLoopback)
                                {
#ifndef __WINSCW__
                                // No audio mixing in WINS - do not start
                                StartDownlink();
#endif //__WINSCW__
                                }
                            break;
                            }
                        case TMS_STREAM_UNINITIALIZED:
                            iTmsDnlink->RemoveObserver(*this);
                            DisplayText("Downlink uninitialized");
                            break;
                        case TMS_STREAM_PAUSED:
                            DisplayText("Downlink paused");
                            break;
                        case TMS_STREAM_STARTED:
                            DisplayText("Downlink started");
                            break;
                        default:
                            break;
                        }
                    break;
                    }
                case TMS_EVENT_STREAM_STATE_CHANGE_ERROR:
                    DisplayText("Downlink Error ", event.reason);
                    break;
                default:
                    break;
                }
            break;
            }
        default:
            break;
        }
    }

//From TMSClientSourceObserver
void CTmsTestEngine::FillBuffer(TMSBuffer& buffer)
    {
    iPlayBufReady = ETrue;
    iPlayBuf = &buffer;

#ifdef __PLAY_WAV_FROM_FILE__
    guint8* gptr(NULL);
    iPlayBuf->GetDataPtr(gptr);
    guint gsize;
    iPlayBuf->GetDataSize(gsize);
    if (!iBuf)
        {
        iBuf = HBufC8::NewL(gsize);
        }
    TPtr8 p = iBuf->Des();

    if ((iReadPos + gsize) > iFileLen)
        {
        gsize = iFileLen - iReadPos;
        iEOF = ETrue;
        }

    iFile.Read(iReadPos, p, gsize);
    //  DEBPRN1(_L("CVoIPTestEngine[0x%x]::FillBuffer [%d]"), iReadPos);

    if (!iEOF)
        {
        iReadPos += gsize;
        }
    else
        {
        // start over from the beginning
        iReadPos = 0;
        iEOF = EFalse;
        }

    Mem::Copy(gptr, (TUint8*) iBuf->Ptr(), iBuf->Size());
    iPlayBuf->SetDataSize(iBuf->Size());
    static_cast<TMSClientSource*> (iTmsClientSource)->BufferFilled(*iPlayBuf);
    User::After(TTimeIntervalMicroSeconds32(100000)); //clears choppy audio

    iPlayBufReady = EFalse; // buf filled, ready for FillBuffer
    iRecBufReady = EFalse; // buf consumed, ready for EmptyBuffer

#else //__PLAY_WAV_FROM_FILE__
    if (iDnLinkStatus == EStreaming)
        {
        DoLoopback();
        }
#endif //__PLAY_WAV_FROM_FILE__
    }

//From TMSClientSourceObserver
void CTmsTestEngine::BufferProcessed(const TMSBuffer* /*buffer*/,
        gint /*reason*/)
    {
    }

// From TMSClientSinkObserver
void CTmsTestEngine::ProcessBuffer(const TMSBuffer* buffer)
    {
    iRecBufReady = ETrue;
    iRecBuf = const_cast<TMSBuffer*> (buffer);

    if (iUpLinkStatus == EStreaming)
        {
        // Process recorded buffer here.

#ifdef __RECORD_WAV_TO_FILE__
        guint8* p(NULL);
        guint len(0);
        iRecBuf->GetDataPtr(p);
        iRecBuf->GetDataSize(len);
        WriteToFile(p, len);
#endif //__RECORD_WAV_TO_FILE__

        DoLoopback();
        static_cast<TMSClientSink*> (iTmsClientSink)->BufferProcessed(iRecBuf);
        }
    }

// From TMSEffectObserver
void CTmsTestEngine::EffectsEvent(const TMSEffect& tmseffect,
        TMSSignalEvent event)
    {
    gint reason = event.reason;

    if (reason == TMS_RESULT_SUCCESS)
        {
        TMSEffectType effecttype;
        const_cast<TMSEffect&> (tmseffect).GetType(effecttype);
        switch (effecttype)
            {
            case TMS_EFFECT_GLOBAL_VOL:
                {
                switch (event.type)
                    {
                    case TMS_EVENT_EFFECT_VOL_CHANGED:
                        DisplayText("Global vol effect change");
                        TMSVolumeEventChangeData* vd;
                        vd = static_cast<TMSVolumeEventChangeData*>
                                (event.event_data);
                        DisplayText("Volume level: ", vd->level);
                        DisplayText("Output device: ", vd->output);
                        break;
                    default:
                        break;
                    }
                }
                break;
            case TMS_EFFECT_GLOBAL_GAIN:
                {
                switch (event.type)
                    {
                    case TMS_EVENT_EFFECT_GAIN_CHANGED:
                        DisplayText("Global gain effect change");
                        break;
                    default:
                        break;
                    }
                }
                break;
            case TMS_EFFECT_VOLUME:
                {
                switch (event.type)
                    {
                    case TMS_EVENT_EFFECT_VOL_CHANGED:
                        DisplayText("Stream vol effect change");
                        break;
                    default:
                        break;
                    }
                }
                break;
            case TMS_EFFECT_GAIN:
                {
                switch (event.type)
                    {
                    case TMS_EVENT_EFFECT_GAIN_CHANGED:
                        DisplayText("Stream gain effect change");
                        break;
                    default:
                        break;
                    }
                }
                break;
            default:
                break;
            }
        }
    else
        {
        DisplayText("Effect failed: ", reason);
        }
    }

// From TMSGlobalRoutingObserver
void CTmsTestEngine::GlobalRoutingEvent(const TMSGlobalRouting& /*routing*/,
        TMSSignalEvent event, TMSAudioOutput output)
    {
    gint reason = event.reason;

    if (reason == TMS_RESULT_SUCCESS)
        {
        switch (event.type)
            {
            case TMS_EVENT_ROUTING_AVAIL_OUTPUTS_CHANGED:
                DisplayText("Available outputs changed");
                break;
            case TMS_EVENT_ROUTING_OUTPUT_CHANGED:
                DisplayText("output changed");
                break;
            case TMS_EVENT_ROUTING_SET_OUTPUT_COMPLETE:
                DisplayText("set output complete");
                break;
            default:
                break;
            }
        DisplayDevice(output);
        }
    else
        {
        DisplayText("Routing failed", reason);
        }
    }

// From TMSRingToneObserver
void CTmsTestEngine::RingtoneEvent(const TMSRingTone& /*rt*/,
        TMSSignalEvent event)
    {
    gint reason = event.reason;

    if (reason == TMS_RESULT_SUCCESS)
        {
        switch (event.type)
            {
            case TMS_EVENT_RINGTONE_OPEN_COMPLETE:
                DisplayText("RT Open Complete");
                break;
            case TMS_EVENT_RINGTONE_PLAY_COMPLETE:
                DisplayText("RT Play Complete");
                break;
            case TMS_EVENT_RINGTONE_DEINIT_COMPLETE:
                DisplayText("RT Deinit Complete");
                break;
            default:
                break;
            }
        }
    else
        {
        DisplayText("Ringtone failed", reason);
        }
    }

// From TMSDTMFObserver
void CTmsTestEngine::DTMFEvent(const TMSDTMF& /*dtmf*/, TMSSignalEvent event)
    {
    gint reason = event.reason;

    if (reason == TMS_RESULT_SUCCESS)
        {
        switch (event.type)
            {
            case TMS_EVENT_DTMF_TONE_STARTED:
                DisplayText("DTMF Started");
                break;
            case TMS_EVENT_DTMF_TONE_STOPPED:
                DisplayText("DTMF Stopped");
                break;
            default:
                break;
            }
        }
    else
        {
        DisplayText("DTMF failed", reason);
        }
    }

// From TMSInbandToneObserver
void CTmsTestEngine::InbandToneEvent(const TMSInbandTone& /*inbandtone*/,
        TMSSignalEvent event)
    {
    gint reason = event.reason;

    if (reason != TMS_RESULT_SUCCESS)
        {
        switch (event.type)
            {
            case TMS_EVENT_INBAND_TONE_STARTED:
                DisplayText("Inband Tone Started");
                break;
            case TMS_EVENT_INBAND_TONE_STOPPED:
                DisplayText("Inband Tone Stopped");
                break;
            default:
                break;
            }
        }
    else
        {
        DisplayText("Inband tone failed", reason);
        }
    }

// ----------------------------------------------------------------------------
// CTmsTestEngine::DisplayText
// Print text with status code.
// ----------------------------------------------------------------------------
//
void CTmsTestEngine::DisplayText(const QString& str, const gint num)
    {
    if (num != 0)
        {
        iStatusDisplay->append(str + " " + QString::number(num));
        }
    else
        {
        iStatusDisplay->append(str);
        }
    }

// End of file
