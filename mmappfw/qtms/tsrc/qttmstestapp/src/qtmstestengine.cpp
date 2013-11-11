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
#include <gstring.h>
#include <qtmsclientsink.h>
#include <qtmsclientsource.h>
#include <qtmspcmformat.h>
#include <qtmsamrformat.h>
#include <qtmsilbcformat.h>
#include <qtmsg711format.h>
#include <qtmsg729format.h>
#include <qtmsvolumeeffect.h>
#include <qtmsgaineffect.h>
#include <qtmsglobalvoleffect.h>
#include <qtmsglobalgaineffect.h>
#include <qtmsglobalrouting.h>
#include <qtmsver.h>
#include "qtmstestengine.h"

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
// QTmsTestEngine::QTmsTestEngine()
// constructor
// ----------------------------------------------------------------------------
//
QTmsTestEngine::QTmsTestEngine()
    {
    // Disable GLib slice allocators; will take care of memory leaks
    // caused by g_string_free().
    g_setenv("G_SLICE", "always-malloc", 1);
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::~QTmsTestEngine
// destructor
// ----------------------------------------------------------------------------
//
QTmsTestEngine::~QTmsTestEngine()
    {
    EndCall();
    CloseRingTonePlayer();

    if (iFactory && iGlobalVol)
        {
        iFactory->DeleteEffect(iGlobalVol);
        }
    if (iFactory && iGlobalGain)
        {
        iFactory->DeleteEffect(iGlobalGain);
        }
    if (iFactory && iTmsGlobalRouting)
        {
        iFactory->DeleteGlobalRouting(iTmsGlobalRouting);
        }
    if (iFactory && iInbandTonePlayer)
        {
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
// QTmsTestEngine::NewL
// Symbian constructor.
// ----------------------------------------------------------------------------
//
QTmsTestEngine* QTmsTestEngine::NewL(QTextEdit* statusDisplay)
    {
    QTmsTestEngine* self(NULL);
    self = QTmsTestEngine::NewLC(statusDisplay);
    return self;
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::NewL
// Symbian constructor with self pointer pushed into the cleanup stack.
// ----------------------------------------------------------------------------
//
QTmsTestEngine* QTmsTestEngine::NewLC(QTextEdit* statusDisplay)
    {
    QTmsTestEngine* self = new (ELeave) QTmsTestEngine;
    //CleanupStack::PushL(self);
    self->ConstructL(statusDisplay);
    return self;
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::NewL
// Standard Symbian 2nd phase constructor
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::ConstructL(QTextEdit* statusDisplay)
    {
    DEBPRN0(_L("QTmsTestEngine[0x%x]::ConstructL :>"));
    iStatusDisplay = statusDisplay;

#ifdef __PLAY_WAV_FROM_FILE__
    iReadSize = KFileBufLen;
    TInt err = iFs.Connect();
    if (err == QTMS_RESULT_SUCCESS)
        {
        err = iFile.Open(iFs, KTestFile1, EFileShareAny | EFileRead);
        }
    if (err == QTMS_RESULT_SUCCESS)
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

    DEBPRN0(_L("QTmsTestEngine[0x%x]::ConstructL :<"));
    }

gint QTmsTestEngine::CreateFactory()
    {
    gint status(QTMS_RESULT_SUCCESS);
    QTMSVer v(10,0,0);
    status = QTMSFactory::CreateFactory(iFactory, v);

    if (status != QTMS_RESULT_SUCCESS || !iFactory)
        {
        DisplayText("QTms create factory failed: ", status);
        }
    else
        {
#ifdef __TEST_CODE_COVERAGE__
        QTMSBuffer* qtmsbuffer(NULL);
        QTMSBufferType buftype(QTMS_BUFFER_MEMORY);
        guint size(100);
        iFactory->CreateBuffer(QTMS_BUFFER_MEMORY, size, qtmsbuffer);
        if (qtmsbuffer)
            {
            qtmsbuffer->GetType(buftype);
            qtmsbuffer->GetDataSize(size);
            qtmsbuffer->SetDataSize(size);
            guint8* pbuf(NULL);
            qtmsbuffer->GetDataPtr(pbuf);
            guint64 ts(0);
            qtmsbuffer->GetTimeStamp(ts);
            qtmsbuffer->SetTimeStamp(ts);
            iFactory->DeleteBuffer(qtmsbuffer);
            }
#endif //__TEST_CODE_COVERAGE__

        DisplayText("Tms factory created");
        }
    return status;
    }

gint QTmsTestEngine::CreateCall(QTMSCallType calltype)
    {
    gint status(QTMS_RESULT_SUCCESS);
    if (iFactory && !iTmsCall)
        {
        status = iFactory->CreateCall(calltype, iTmsCall, 0);
        if (status == QTMS_RESULT_SUCCESS)
            {
#ifdef __TEST_CODE_COVERAGE__
            guint ctxid;
            iTmsCall->GetCallContextId(ctxid);
#endif //__TEST_CODE_COVERAGE__
            DisplayText("Tms call created");
            }
        else
            {
            DisplayText("Tms call create failed: ", status);
            }
        }
    return status;
    }

gint QTmsTestEngine::CreateUplink()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iTmsCall)
        {
        status = iTmsCall->CreateStream(QTMS_STREAM_UPLINK, iTmsUplink);
        if (status == QTMS_RESULT_SUCCESS)
            {
#ifdef __TEST_CODE_COVERAGE__
            guint ctxid;
            iTmsCall->GetCallContextId(ctxid);
#endif //__TEST_CODE_COVERAGE__
            DisplayText("Connected to uplink signal");
            connect(iTmsUplink,
                    SIGNAL(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)),
                    this,
                    SLOT(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)));
            }
        else
            {
            DisplayText("TMS uplink failed: ", status);
            }
        }
    return status;
    }

gint QTmsTestEngine::CreateDownlink()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iTmsCall)
        {
        status = iTmsCall->CreateStream(QTMS_STREAM_DOWNLINK, iTmsDnlink);
        if (status == QTMS_RESULT_SUCCESS)
            {
            DisplayText("Connected to downlink signal");
            connect(iTmsDnlink,
                    SIGNAL(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)),
                    this,
                    SLOT(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)));
            }
        else
            {
            DisplayText("TMS downlink failed: ", status);
            }
        }
    return status;
    }

gint QTmsTestEngine::CreateClientSource()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iFactory && !iTmsClientSource)
        {
        status = iFactory->CreateSource(QTMS_SOURCE_CLIENT, iTmsClientSource);
        if (status == QTMS_RESULT_SUCCESS)
            {
            DisplayText("TMS clientsource created");
            }
        else
            {
            DisplayText("TMS clientsource failed: ", status);
            }
        }
    return status;
    }

gint QTmsTestEngine::CreateModemSource()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iFactory && !iTmsModemSource)
        {
        status = iFactory->CreateSource(QTMS_SOURCE_MODEM, iTmsModemSource);
        if (status == QTMS_RESULT_SUCCESS)
            {
#ifdef __TEST_CODE_COVERAGE__
            QTMSSourceType st;
            iTmsModemSource->GetType(st);
#endif //__TEST_CODE_COVERAGE__
            DisplayText("TMS modemsource created");
            }
        else
            {
            DisplayText("TMS modemsource failed: ", status);
            }
        }
    return status;
    }

gint QTmsTestEngine::AddClientSourceToStream()
    {
    gint status(QTMS_RESULT_SUCCESS);
    if (iTmsDnlink && iTmsClientSource)
        {
        connect(static_cast<QTMSClientSource*> (iTmsClientSource),
                SIGNAL(FillBuffer(QTMSBuffer&)), this,
                SLOT(FillBuffer(QTMSBuffer&)));
        status = iTmsDnlink->AddSource(iTmsClientSource);

#ifdef __TEST_CODE_COVERAGE__
        QTMSSourceType st;
        static_cast<QTMSClientSource*>(iTmsClientSource)->GetType(st);
        gboolean enqueue(false);
        static_cast<QTMSClientSource*>(iTmsClientSource)->GetEnqueueMode(enqueue);
        static_cast<QTMSClientSource*>(iTmsClientSource)->SetEnqueueMode(FALSE);
        static_cast<QTMSClientSource*>(iTmsClientSource)->Flush();
#endif //__TEST_CODE_COVERAGE__
        }
    return status;
    }

gint QTmsTestEngine::AddModemSourceToStream()
    {
    gint status(QTMS_RESULT_SUCCESS);
    if (iTmsDnlink && iTmsModemSource)
        {
        status = iTmsDnlink->AddSource(iTmsModemSource);
        }
    return status;
    }

gint QTmsTestEngine::CreateClientSink()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iFactory && !iTmsClientSink)
        {
        status = iFactory->CreateSink(QTMS_SINK_CLIENT, iTmsClientSink);
        if (status == QTMS_RESULT_SUCCESS)
            {
            DisplayText("TMS client sink created");
            }
        else
            {
            DisplayText("TMS client sink failed: ", status);
            }
        }
    return status;
    }

gint QTmsTestEngine::CreateModemSink()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iFactory && !iTmsModemSink)
        {
        status = iFactory->CreateSink(QTMS_SINK_MODEM, iTmsModemSink);
        if (status == QTMS_RESULT_SUCCESS)
            {
#ifdef __TEST_CODE_COVERAGE__
            QTMSSinkType st;
            iTmsModemSink->GetType(st);
#endif  //__TEST_CODE_COVERAGE__
            DisplayText("TMS modem sink created");
            }
        else
            {
            DisplayText("TMS modem sink failed: ", status);
            }
        }
    return status;
    }

gint QTmsTestEngine::AddClientSinkToStream()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iTmsUplink && iTmsClientSink)
        {
        connect(static_cast<QTMSClientSink*> (iTmsClientSink),
                SIGNAL(ProcessBuffer(const QTMSBuffer*)), this,
                SLOT(ProcessBuffer(const QTMSBuffer*)));
        status = iTmsUplink->AddSink(iTmsClientSink);
        }
    return status;
    }

gint QTmsTestEngine::AddModemSinkToStream()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iTmsUplink && iTmsModemSink)
        {
        status = iTmsUplink->AddSink(iTmsModemSink);
        }
    return status;
    }

gint QTmsTestEngine::CreateMicSource()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iFactory && !iTmsMicSource)
        {
        status = iFactory->CreateSource(QTMS_SOURCE_MIC, iTmsMicSource);
        if (status == QTMS_RESULT_SUCCESS)
            {
            DisplayText("TMS mic source created");
            }
        else
            {
            DisplayText("TMS mic source failed: ", status);
            }
        }
    return status;
    }

gint QTmsTestEngine::AddMicSourceToStream()
    {
    gint status(QTMS_RESULT_SUCCESS);
    if (iTmsUplink && iTmsMicSource)
        {
        status = iTmsUplink->AddSource(iTmsMicSource);
        }
    return status;
    }

gint QTmsTestEngine::CreateSpeakerSink()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iFactory && !iTmsSpeakerSink)
        {
        status = iFactory->CreateSink(QTMS_SINK_SPEAKER, iTmsSpeakerSink);
        if (status == QTMS_RESULT_SUCCESS)
            {
            DisplayText("TMS speaker sink created");
            }
        else
            {
            DisplayText("TMS speaker sink failed: ", status);
            }
        }
    return status;
    }

gint QTmsTestEngine::AddSpeakerSinkToStream()
    {
    gint status(QTMS_RESULT_SUCCESS);
    if (iTmsDnlink && iTmsSpeakerSink)
        {
        status = iTmsDnlink->AddSink(iTmsSpeakerSink);
        }
    return status;
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SetCallType
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetCallType(QTMSCallType calltype)
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

        if (status == QTMS_RESULT_SUCCESS)
            {
            DisplayText("Tms call created");
            }
        else
            {
            DisplayText("Tms call creation failed: ", status);
            }
        }
    if (status == QTMS_RESULT_SUCCESS)
        {
        status = CreateUplink();
        }
    if (status == QTMS_RESULT_SUCCESS)
        {
        status = CreateDownlink();
        }
    if (iCallType == QTMS_CALL_IP)
        {
        if (status == QTMS_RESULT_SUCCESS)
            {
            status = CreateMicSource();
            status |= AddMicSourceToStream();
            status |= CreateClientSink();
            status |= AddClientSinkToStream();
            }

        if (status == QTMS_RESULT_SUCCESS)
            {
            status |= CreateClientSource();
            status |= AddClientSourceToStream();
            status |= CreateSpeakerSink();
            status |= AddSpeakerSinkToStream();
            }
        }
    else if (iCallType == QTMS_CALL_CS)
        {
        if (status == QTMS_RESULT_SUCCESS)
            {
            status = CreateMicSource();
            status |= AddMicSourceToStream();
            status |= CreateModemSink();
            status |= AddModemSinkToStream();
            }
        if (status == QTMS_RESULT_SUCCESS)
            {
            status = CreateModemSource();
            status |= AddModemSourceToStream();
            status |= CreateSpeakerSink();
            status |= AddSpeakerSinkToStream();
            }
        }

    if (status == QTMS_RESULT_SUCCESS)
        {
        status = CreateVolumeEffect();
        status |= AddVolumeEffectToStream();
        status |= CreateGainEffect();
        status |= AddGainEffectToStream();
        }

    //NOTE: CS does not call SetDownlinkFormat/SetUplinkFormat
    //so it is OK to open downlink and uplink at this point.
    if (status == QTMS_RESULT_SUCCESS && iCallType == QTMS_CALL_CS)
        {
        // To avoid asyc calback racing situation, it may be safer to
        // start second stream after receiving TMS_STREAM_INITIALIZED event.
        // But for now, let's try opening both at the same time.
        OpenDownlink();
        OpenUplink();
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetSupportedDownlinkFormats
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetSupportedDownlinkFormats(TBool aDisplayList)
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iFactory)
        {
        status = iFactory->GetSupportedFormats(QTMS_STREAM_DOWNLINK,
                iDnlCodecs);

        if (status != QTMS_RESULT_SUCCESS)
            {
            DisplayText("DNL Codecs retrieve error: ", status);
            }

        if (iDnlCodecs.size() > 0 && aDisplayList)
            {
            DisplayText("Supported DNL codecs");
            std::vector<QTMSFormat*>::iterator itDnlCodecs =
                    iDnlCodecs.begin();
            QTMSFormatType fmttype;
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
// QTmsTestEngine::GetSupportedUplinkFormats
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetSupportedUplinkFormats(TBool aDisplayList)
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iFactory)
        {
        status = iFactory->GetSupportedFormats(QTMS_STREAM_UPLINK, iUplCodecs);
        if (status != QTMS_RESULT_SUCCESS)
            {
            DisplayText("UPL Codecs retrieve error: ", status);
            }

        if (iUplCodecs.size() > 0 && aDisplayList)
            {
            DisplayText("Supported UPL codecs");
            std::vector<QTMSFormat*>::iterator codecs = iUplCodecs.begin();
            QTMSFormatType fmttype;
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
// QTmsTestEngine::DisplayFormat
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::DisplayFormat(QTMSFormatType aFormat)
    {
    switch (aFormat)
        {
        case QTMS_FORMAT_AMR:
            DisplayText("AMR-NB");
            break;
        case QTMS_FORMAT_G711:
            DisplayText("G.711");
            break;
        case QTMS_FORMAT_G729:
            DisplayText("G.729");
            break;
        case QTMS_FORMAT_ILBC:
            DisplayText("iLBC");
            break;
        case QTMS_FORMAT_PCM:
            DisplayText("PCM-16");
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SetDownlinkFormat()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetDownlinkFormat(QTMSFormatType aCodecFormat)
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (!iTmsCall)
        {
        SetCallType(QTMS_CALL_IP);
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
        case QTMS_FORMAT_G711:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iDecFormatIntfc->GetType(ft);
            if (ft != QTMS_FORMAT_G711)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("G.711 DNL Codec Set");
            break;
            }
        case QTMS_FORMAT_G729:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iDecFormatIntfc->GetType(ft);
            if (ft != QTMS_FORMAT_G729)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("G.729 DNL Codec Set");
            break;
            }
        case QTMS_FORMAT_ILBC:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iDecFormatIntfc->GetType(ft);
            if (ft != QTMS_FORMAT_ILBC)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("iLBC DNL Codec Set");
            break;
            }
        case QTMS_FORMAT_AMR:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iDecFormatIntfc->GetType(ft);
            if (ft != QTMS_FORMAT_AMR)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("AMR-NB DNL Codec Set");
            break;
            }
        case QTMS_FORMAT_PCM:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iDecFormatIntfc->GetType(ft);
            if (ft != QTMS_FORMAT_PCM)
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

    if (status == QTMS_RESULT_SUCCESS)
        {
        OpenDownlink(); //Initialize DNL
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::OpenDownlink()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::OpenDownlink()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iTmsDnlink)
        {
        status = iTmsDnlink->Init();
        if (status != QTMS_RESULT_SUCCESS)
            {
            DisplayText("DNL init error: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::StartDownlink()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::StartDownlink()
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
// QTmsTestEngine::StopDownlink()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::StopDownlink()
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
// QTmsTestEngine::CloseDownlink()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::CloseDownlink()
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

gint QTmsTestEngine::CreateVolumeEffect()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iFactory && iTmsDnlink && !iTmsDnlinkEffect)
        {
        status = iFactory->CreateEffect(QTMS_EFFECT_VOLUME, iTmsDnlinkEffect);
        }
    return status;
    }

gint QTmsTestEngine::AddVolumeEffectToStream()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iTmsDnlink && iTmsDnlinkEffect)
        {
        status = iTmsDnlink->AddEffect(iTmsDnlinkEffect);
        }
    return status;
    }

gint QTmsTestEngine::CreateGainEffect()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iFactory && iTmsUplink && !iTmsUplinkEffect)
        {
        status = iFactory->CreateEffect(QTMS_EFFECT_GAIN, iTmsUplinkEffect);
        }
    return status;
    }

gint QTmsTestEngine::CreateGlobalRouting()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iFactory)
        {
        status = iFactory->CreateGlobalRouting(iTmsGlobalRouting);
        if (status == QTMS_RESULT_SUCCESS)
            {
            DisplayText("TMS routing created");
            connect(iTmsGlobalRouting,
                    SIGNAL(GlobalRoutingEvent(const QTMSGlobalRouting&,QTMSSignalEvent,QTMSAudioOutput)),
                    this,
                    SLOT(GlobalRoutingEvent(const QTMSGlobalRouting&,QTMSSignalEvent,QTMSAudioOutput)));
            //   DisplayText("connected to routing signal");
            }
        else
            {
            DisplayText("Global routing failed: ", status);
            }
        }
    return status;
    }

gint QTmsTestEngine::AddGainEffectToStream()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iTmsUplink && iTmsUplinkEffect)
        {
        status = iTmsUplink->AddEffect(iTmsUplinkEffect);
        }
    return status;
    }

gint QTmsTestEngine::CreateRingTonePlayer()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iFactory)
        {
        status = iFactory->CreateRingTonePlayer(iTmsRingTonePlayer);

        if (iTmsRingTonePlayer && status == QTMS_RESULT_SUCCESS)
            {
            connect(iTmsRingTonePlayer,
                    SIGNAL(RingtoneEvent(const QTMSRingTone&,QTMSSignalEvent)),
                    this,
                    SLOT(RingtoneEvent(const QTMSRingTone&,QTMSSignalEvent)));
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
// QTmsTestEngine::ConfigureDecoder()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::ConfigureDecoder()
    {
    // NOTE: These calls can ONLY be made when codec is in a STOPPED state.

    switch (iDnLinkCodec)
        {
        case QTMS_FORMAT_G711:
            {
            gboolean cng(TRUE);
            static_cast<QTMSG711Format*> (iDecFormatIntfc)->SetCNG(cng);
            static_cast<QTMSG711Format*> (iDecFormatIntfc)->GetCNG(cng);
            gboolean plc(FALSE);
            static_cast<QTMSG711Format*> (iDecFormatIntfc)->SetPlc(plc);
            static_cast<QTMSG711Format*> (iDecFormatIntfc)->GetPlc(plc);
            QTMSG711CodecMode mode(QTMS_G711_CODEC_MODE_ALAW);
            static_cast<QTMSG711Format*> (iDecFormatIntfc)->SetMode(mode);
            static_cast<QTMSG711Format*> (iDecFormatIntfc)->GetMode(mode);
            break;
            }
        case QTMS_FORMAT_ILBC:
            {
            gboolean cng(TRUE);
            static_cast<QTMSILBCFormat*> (iDecFormatIntfc)->SetCNG(cng);
            static_cast<QTMSILBCFormat*> (iDecFormatIntfc)->GetCNG(cng);
            QTMSILBCCodecMode mode(QTMS_ILBC_CODEC_MODE_20MS_FRAME);
            static_cast<QTMSILBCFormat*> (iDecFormatIntfc)->SetMode(mode);
            static_cast<QTMSILBCFormat*> (iDecFormatIntfc)->GetMode(mode);
            break;
            }
        case QTMS_FORMAT_G729:
        case QTMS_FORMAT_AMR:
        case QTMS_FORMAT_PCM:
        default:
            {
            break;
            }
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetMaxVolume()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetMaxVolume()
    {
    if (iTmsDnlinkEffect)
        {
        static_cast<QTMSVolumeEffect*> (iTmsDnlinkEffect)->GetMaxLevel(
                iMaxVolume);
        }
    DisplayText("Max Volume: ", iMaxVolume);
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetVolume
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetVolume()
    {
    if (iTmsDnlinkEffect)
        {
        static_cast<QTMSVolumeEffect*> (iTmsDnlinkEffect)->GetLevel(iVolume);
        }
    DisplayText("Volume ", iVolume);
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::MuteSpeaker()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::MuteSpeaker()
    {
    SetVolume(0);
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SetMaxVolume
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetMaxVolume()
    {
    SetVolume(iMaxVolume);
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SetVolume
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetVolume(guint aVolume)
    {
    iVolume = aVolume;

    if (iTmsDnlinkEffect)
        {
        static_cast<QTMSVolumeEffect*> (iTmsDnlinkEffect)->SetLevel(aVolume);
        }
    else
        {
        DisplayText("Create Dnlink VolumeEffect first");
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::VolumeUp()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::VolumeUp()
    {
    if ((iDnLinkStatus == EReady || iDnLinkStatus == EStreaming)
            && iTmsDnlinkEffect)
        {
        if (iVolume < iMaxVolume)
            {
            static_cast<QTMSVolumeEffect*> (iTmsDnlinkEffect)->SetLevel(
                    ++iVolume);
            }
        }
    else
        {
        DisplayText("Create DNL first");
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::VolumeDn()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::VolumeDn()
    {
    if ((iDnLinkStatus == EReady || iDnLinkStatus == EStreaming)
            && iTmsDnlinkEffect)
        {
        if (iVolume > 0)
            {
            static_cast<QTMSVolumeEffect*> (iTmsDnlinkEffect)->SetLevel(
                    --iVolume);
            }
        }
    else
        {
        DisplayText("Open DNL first");
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::OpenUplink()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::OpenUplink()
    {
    gint status = QTMS_RESULT_SUCCESS;

    if (iTmsUplink)
        {
        status = iTmsUplink->Init(3); //retry for ~3 sec
        if (status != QTMS_RESULT_SUCCESS)
            {
            DisplayText("UPL init error: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::StartUplink()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::StartUplink()
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
// QTmsTestEngine::StopUplink()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::StopUplink()
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
// QTmsTestEngine::CloseUplink()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::CloseUplink()
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
// QTmsTestEngine::SetUplinkFormat()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetUplinkFormat(QTMSFormatType aCodecFormat)
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (!iTmsCall)
        {
        SetCallType(QTMS_CALL_IP);
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
        case QTMS_FORMAT_G711:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iEncFormatIntfc->GetType(ft);
            if (ft != QTMS_FORMAT_G711)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("G.711 UPL Codec Set");
            break;
            }
        case QTMS_FORMAT_G729:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iEncFormatIntfc->GetType(ft);
            if (ft != QTMS_FORMAT_G729)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("G.729 UPL Codec Set");
            break;
            }
        case QTMS_FORMAT_ILBC:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iEncFormatIntfc->GetType(ft);
            if (ft != QTMS_FORMAT_ILBC)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("iLBC UPL Codec Set");
            break;
            }
        case QTMS_FORMAT_AMR:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iEncFormatIntfc->GetType(ft);
            if (ft != QTMS_FORMAT_AMR)
                {
                status = KErrArgument;
                }
#endif //__TEST_CODE_COVERAGE__
            DisplayText("AMR-NB UPL Codec Set");
            break;
            }
        case QTMS_FORMAT_PCM:
            {
#ifdef __TEST_CODE_COVERAGE__
            status = iEncFormatIntfc->GetType(ft);
            if (ft != QTMS_FORMAT_PCM)
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

    if (status == QTMS_RESULT_SUCCESS)
        {
        OpenUplink(); //Initialize UPL
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::ConfigureEncoder()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::ConfigureEncoder()
    {
    // Any of these calls can ONLY be made when encoder is in a STOPPED state.

    switch (iUpLinkCodec)
        {
        case QTMS_FORMAT_G711:
            {
            QTMSG711CodecMode mode(QTMS_G711_CODEC_MODE_ALAW);
            static_cast<QTMSG711Format*> (iEncFormatIntfc)->SetMode(mode);
            static_cast<QTMSG711Format*> (iEncFormatIntfc)->GetMode(mode);
            break;
            }
        case QTMS_FORMAT_ILBC:
            {
            QTMSILBCCodecMode mode(QTMS_ILBC_CODEC_MODE_20MS_FRAME);
            static_cast<QTMSILBCFormat*> (iEncFormatIntfc)->SetMode(mode);
            static_cast<QTMSILBCFormat*> (iEncFormatIntfc)->GetMode(mode);
            break;
            }
        case QTMS_FORMAT_G729:
        case QTMS_FORMAT_AMR:
        case QTMS_FORMAT_PCM:
        default:
            {
            break;
            }
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetMaxGain()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetMaxGain()
    {
    if (iTmsUplinkEffect)
        {
        static_cast<QTMSGainEffect*> (iTmsUplinkEffect)->GetMaxLevel(iMaxGain);
        DisplayText("MaxGain: ", iMaxGain);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetGain()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetGain()
    {
    guint gain;
    if (iTmsUplinkEffect)
        {
        static_cast<QTMSGainEffect*> (iTmsUplinkEffect)->GetLevel(gain);
        DisplayText("Gain: ", gain);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SetMaxGain()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetMaxGain()
    {
    static_cast<QTMSGainEffect*> (iTmsUplinkEffect)->SetLevel(iMaxGain);
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::MuteMic()
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::MuteMic()
    {
    static_cast<QTMSGainEffect*> (iTmsUplinkEffect)->SetLevel(0);
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetSupportedBitrates
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetSupportedBitrates()
    {
    TInt status = GetSupportedBitrates(iBitratesVector);

    if (status == QTMS_RESULT_SUCCESS)
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
// QTmsTestEngine::GetSupportedBitrates
//
// ----------------------------------------------------------------------------
//
gint QTmsTestEngine::GetSupportedBitrates(BitRateVector& aBrArr)
    {
    gint status = KErrNotFound;

    switch (iUpLinkCodec)
        {
        case QTMS_FORMAT_G711:
            status = static_cast<QTMSG711Format*>
                    (iEncFormatIntfc)->GetSupportedBitRates(aBrArr);
            break;
        case QTMS_FORMAT_G729:
            status = static_cast<QTMSG729Format*>
                    (iEncFormatIntfc)->GetSupportedBitRates(aBrArr);
            break;
        case QTMS_FORMAT_ILBC:
            status = static_cast<QTMSILBCFormat*>
                    (iEncFormatIntfc)->GetSupportedBitRates(aBrArr);
            break;
        case QTMS_FORMAT_AMR:
            status = static_cast<QTMSAMRFormat*>
                    (iEncFormatIntfc)->GetSupportedBitRates(aBrArr);
            break;
        case QTMS_FORMAT_PCM:
        default:
            status = KErrNotSupported;
            break;
        }
    return status;
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SelectMinBitrate
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SelectMinBitrate()
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
// QTmsTestEngine::SelectMaxBitrate
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SelectMaxBitrate()
    {
    if (iBitratesVector.size() > 0)
        {
        iBitrate = iBitratesVector.back();
        SetBitrate(iBitrate);
        DisplayText("BR set: ", iBitrate);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SetBitrate
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetBitrate(TUint aBitrate)
    {
    switch (iUpLinkCodec)
        {
        case QTMS_FORMAT_G711:
            static_cast<QTMSG711Format*> (iEncFormatIntfc)->SetBitRate(
                    aBitrate);
            DisplayText("Set BR: ", iBitrate);
            break;
        case QTMS_FORMAT_G729:
            static_cast<QTMSG729Format*> (iEncFormatIntfc)->SetBitRate(
                    aBitrate);
            DisplayText("Set BR: ", iBitrate);
            break;
        case QTMS_FORMAT_ILBC:
            static_cast<QTMSILBCFormat*> (iEncFormatIntfc)->SetBitRate(
                    aBitrate);
            DisplayText("Set BR: ", iBitrate);
            break;
        case QTMS_FORMAT_AMR:
            static_cast<QTMSAMRFormat*> (iEncFormatIntfc)->SetBitRate(
                    aBitrate);
            DisplayText("Set BR: ", iBitrate);
            break;
        case QTMS_FORMAT_PCM:
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetBitrate
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetBitrate()
    {
    if (iUpLinkCodec != QTMS_FORMAT_PCM)
        {
        GetBitrate(iBitrate);
        DisplayText("Current BR: ", iBitrate);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetBitrate
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetBitrate(TUint& aBitrate)
    {
    switch (iUpLinkCodec)
        {
        case QTMS_FORMAT_G711:
            static_cast<QTMSG711Format*> (iEncFormatIntfc)->GetBitRate(
                    aBitrate);
            break;
        case QTMS_FORMAT_G729:
            static_cast<QTMSG729Format*> (iEncFormatIntfc)->GetBitRate(
                    aBitrate);
            break;
        case QTMS_FORMAT_ILBC:
            static_cast<QTMSILBCFormat*> (iEncFormatIntfc)->GetBitRate(
                    aBitrate);
            break;
        case QTMS_FORMAT_AMR:
            static_cast<QTMSAMRFormat*> (iEncFormatIntfc)->GetBitRate(
                    aBitrate);
            break;
        case QTMS_FORMAT_PCM:
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::ToggleVad
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::ToggleVad()
    {
    iVad = (iVad) ? EFalse : ETrue;

    switch (iUpLinkCodec)
        {
        case QTMS_FORMAT_G711:
            static_cast<QTMSG711Format*> (iEncFormatIntfc)->SetVADMode(iVad);
            DisplayText("Set VAD: ", iVad);
            break;
        case QTMS_FORMAT_G729:
            static_cast<QTMSG729Format*> (iEncFormatIntfc)->SetVADMode(iVad);
            DisplayText("Set VAD: ", iVad);
            break;
        case QTMS_FORMAT_ILBC:
            static_cast<QTMSILBCFormat*> (iEncFormatIntfc)->SetVADMode(iVad);
            DisplayText("Set VAD: ", iVad);
            break;
        case QTMS_FORMAT_AMR:
            //static_cast<QTMSAMRFormat*> (iEncFormatIntfc)->SetVADMode(iVad);
            //DisplayText("Set VAD: ", iVad);
            break;
        case QTMS_FORMAT_PCM:
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetVad
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetVad()
    {
    switch (iUpLinkCodec)
        {
        case QTMS_FORMAT_G711:
            static_cast<QTMSG711Format*> (iEncFormatIntfc)->GetVADMode(iVad);
            DisplayText("Current VAD: ", iVad);
            break;
        case QTMS_FORMAT_G729:
            static_cast<QTMSG729Format*> (iEncFormatIntfc)->GetVADMode(iVad);
            DisplayText("Current VAD: ", iVad);
            break;
        case QTMS_FORMAT_ILBC:
            static_cast<QTMSILBCFormat*> (iEncFormatIntfc)->GetVADMode(iVad);
            DisplayText("Current VAD: ", iVad);
            break;
        case QTMS_FORMAT_AMR:
            //static_cast<QTMSAMRFormat*> (iEncFormatIntfc)->GetVADMode(iVad);
            //DisplayText("Current VAD: ", iVad);
            break;
        case QTMS_FORMAT_PCM:
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::ToggleCng
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::ToggleCng()
    {
    iCng = (iCng) ? EFalse : ETrue;

    switch (iDnLinkCodec)
        {
        case QTMS_FORMAT_G711:
            static_cast<QTMSG711Format*> (iDecFormatIntfc)->SetCNG(iCng);
            DisplayText("Set CNG ", iCng);
            break;
        case QTMS_FORMAT_ILBC:
            static_cast<QTMSILBCFormat*> (iDecFormatIntfc)->SetCNG(iCng);
            DisplayText("Set CNG ", iCng);
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetCng
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetCng()
    {
    switch (iDnLinkCodec)
        {
        case QTMS_FORMAT_G711:
            static_cast<QTMSG711Format*> (iDecFormatIntfc)->GetCNG(iCng);
            DisplayText("Current CNG: ", iCng);
            break;
        case QTMS_FORMAT_ILBC:
            static_cast<QTMSILBCFormat*> (iDecFormatIntfc)->GetCNG(iCng);
            DisplayText("Current CNG: ", iCng);
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::TogglePlc
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::TogglePlc()
    {
    iPlc = (iPlc) ? EFalse : ETrue;
    if (iDnLinkCodec == QTMS_FORMAT_G711)
        {
        static_cast<QTMSG711Format*> (iDecFormatIntfc)->SetPlc(iPlc);
        DisplayText("Set PLC: ", iPlc);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetPlc
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetPlc()
    {
    if (iDnLinkCodec == QTMS_FORMAT_G711)
        {
        static_cast<QTMSG711Format*> (iDecFormatIntfc)->GetPlc(iPlc);
        DisplayText("Current PLC: ", iPlc);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SetDnLinkG711ALAW
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetDnLinkG711ALAW()
    {
    if (iDnLinkCodec == QTMS_FORMAT_G711)
        {
        static_cast<QTMSG711Format*> (iDecFormatIntfc)->SetMode(
                QTMS_G711_CODEC_MODE_ALAW);
        DisplayText("DNL G.711 Mode Set: [aLaw]");
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SetDnLinkG711uLAW
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetDnLinkG711uLAW()
    {
    if (iDnLinkCodec == QTMS_FORMAT_G711)
        {
        static_cast<QTMSG711Format*> (iDecFormatIntfc)->SetMode(
                QTMS_G711_CODEC_MODE_MULAW);
        DisplayText("DNL G.711 Mode Set: [uLaw]");
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetDnLinkG711Mode
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetDnLinkG711Mode()
    {
    if (iDnLinkCodec == QTMS_FORMAT_G711)
        {
        QTMSG711CodecMode mode;
        TInt status = static_cast<QTMSG711Format*> (iDecFormatIntfc)->GetMode(
                mode);

        if (status == QTMS_RESULT_SUCCESS)
            {
            if (mode == QTMS_G711_CODEC_MODE_MULAW)
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
// QTmsTestEngine::SetDnLinkILBC20MS
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetDnLinkILBC20MS()
    {
    if (iDnLinkCodec == QTMS_FORMAT_ILBC)
        {
        static_cast<QTMSILBCFormat*> (iDecFormatIntfc)->SetMode(
                QTMS_ILBC_CODEC_MODE_20MS_FRAME);
        DisplayText("DNL iLBC Mode Set: [20ms]");
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SetDnLinkILBC30MS
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetDnLinkILBC30MS()
    {
    if (iDnLinkCodec == QTMS_FORMAT_ILBC)
        {
        static_cast<QTMSILBCFormat*> (iDecFormatIntfc)->SetMode(
                QTMS_ILBC_CODEC_MODE_30MS_FRAME);
        DisplayText("DNL iLBC Mode Set: [30ms]");
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetDnLinkILBCMode
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetDnLinkILBCMode()
    {
    if (iDnLinkCodec == QTMS_FORMAT_ILBC)
        {
        QTMSILBCCodecMode mode;
        gint status = static_cast<QTMSILBCFormat*> (iDecFormatIntfc)->GetMode(
                mode);

        if (status == QTMS_RESULT_SUCCESS)
            {
            if (mode == QTMS_ILBC_CODEC_MODE_30MS_FRAME)
                {
                DisplayText("DNL iLBC Mode: [30ms]");
                }
            else if (mode == QTMS_ILBC_CODEC_MODE_20MS_FRAME)
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
// QTmsTestEngine::SetUpLinkG711ALAW
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetUpLinkG711ALAW()
    {
    if (iUpLinkCodec == QTMS_FORMAT_G711)
        {
        static_cast<QTMSG711Format*> (iEncFormatIntfc)->SetMode(
                QTMS_G711_CODEC_MODE_ALAW);
        DisplayText("UPL G.711 Mode Set: [aLaw]");
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SetUpLinkG711uLAW
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetUpLinkG711uLAW()
    {

    if (iUpLinkCodec == QTMS_FORMAT_G711)
        {
        static_cast<QTMSG711Format*> (iEncFormatIntfc)->SetMode(
                QTMS_G711_CODEC_MODE_MULAW);
        DisplayText("UPL G.711 Mode Set: [uLaw]");
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetUpLinkG711Mode
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetUpLinkG711Mode()
    {
    gint status(QTMS_RESULT_SUCCESS);
    if (iUpLinkCodec == QTMS_FORMAT_G711)
        {
        QTMSG711CodecMode mode;
        status = static_cast<QTMSG711Format*> (iEncFormatIntfc)->GetMode(mode);

        if (status == QTMS_RESULT_SUCCESS)
            {
            if (mode == QTMS_G711_CODEC_MODE_MULAW)
                {
                DisplayText("UPL G.711 Mode: [uLaw]");
                }
            else if (mode == QTMS_G711_CODEC_MODE_ALAW)
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
// QTmsTestEngine::SetUpLinkILBC20MS
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetUpLinkILBC20MS()
    {
    if (iUpLinkCodec == QTMS_FORMAT_ILBC)
        {
        static_cast<QTMSILBCFormat*> (iEncFormatIntfc)->SetMode(
                QTMS_ILBC_CODEC_MODE_20MS_FRAME);
        DisplayText("UPL iLBC Mode Set: [20ms]");
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SetUpLinkILBC30MS
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetUpLinkILBC30MS()
    {
    if (iUpLinkCodec == QTMS_FORMAT_ILBC)
        {
        static_cast<QTMSILBCFormat*> (iEncFormatIntfc)->SetMode(
                QTMS_ILBC_CODEC_MODE_30MS_FRAME);
        DisplayText("UPL iLBC Mode Set: [30ms]");
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetUpLinkILBCMode
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetUpLinkILBCMode()
    {
    gint status(QTMS_RESULT_SUCCESS);
    if (iUpLinkCodec == QTMS_FORMAT_ILBC)
        {
        QTMSILBCCodecMode mode;
        status = static_cast<QTMSILBCFormat*> (iEncFormatIntfc)->GetMode(mode);

        if (status == QTMS_RESULT_SUCCESS)
            {
            if (mode == QTMS_ILBC_CODEC_MODE_30MS_FRAME)
                {
                DisplayText("UPL iLBC Mode: [30ms]");
                }
            else if (mode == QTMS_ILBC_CODEC_MODE_20MS_FRAME)
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
// QTmsTestEngine::CreateGlobalVol
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::CreateGlobalVol()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iFactory && !iGlobalVol)
        {
        status = iFactory->CreateEffect(QTMS_EFFECT_GLOBAL_VOL, iGlobalVol);

        if (status == QTMS_RESULT_SUCCESS)
            {
            DisplayText("Global Vol Created");
            connect(static_cast<QTMSGlobalVolEffect*> (iGlobalVol),
                    SIGNAL(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)),
                    this,
                    SLOT(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)));
                //  DisplayText("connected to global vol signal");
            }
        else
            {
            DisplayText("Global Vol failed: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::CreateGlobalGain
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::CreateGlobalGain()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iFactory && !iGlobalGain)
        {
        status = iFactory->CreateEffect(QTMS_EFFECT_GLOBAL_GAIN, iGlobalGain);

        if (status == QTMS_RESULT_SUCCESS)
            {
            DisplayText("Global Gain Created");
            connect(static_cast<QTMSGlobalGainEffect*> (iGlobalGain),
                    SIGNAL(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)),
                    this,
                    SLOT(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)));
                //  DisplayText("connected to global gain signal");
            }
        else
            {
            DisplayText("Global Gain failed: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetGlobalVol
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetGlobalVol()
    {
    if (iGlobalVol)
        {
        TUint level(0);
        static_cast<QTMSGlobalVolEffect*> (iGlobalVol)->GetLevel(level);
        DisplayText("Global Vol: ", level);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetGlobalMaxVol
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetGlobalMaxVol()
    {
    if (iGlobalVol)
        {
        TUint level(0);
        static_cast<QTMSGlobalVolEffect*> (iGlobalVol)->GetMaxLevel(level);
        DisplayText("Global Max Vol: ", level);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SetGlobalVol
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetGlobalVol()
    {
    TUint level(0);
    if (iGlobalVol)
        {
        static_cast<QTMSGlobalVolEffect*> (iGlobalVol)->GetMaxLevel(level);
        static_cast<QTMSGlobalVolEffect*> (iGlobalVol)->SetLevel(level);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetGlobalGain
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetGlobalGain()
    {
    if (iGlobalGain)
        {
        TUint level(0);
        static_cast<QTMSGlobalGainEffect*> (iGlobalGain)->GetLevel(level);
        DisplayText("Global Gain: ", level);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetGlobalMaxGain
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetGlobalMaxGain()
    {
    if (iGlobalGain)
        {
        TUint level(0);
        static_cast<QTMSGlobalGainEffect*> (iGlobalGain)->GetMaxLevel(level);
        DisplayText("Global Max gain: ", level);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SetGlobalGain
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetGlobalGain()
    {
    TUint level(0);
    if (iGlobalGain)
        {
        static_cast<QTMSGlobalGainEffect*> (iGlobalGain)->GetMaxLevel(level);
        static_cast<QTMSGlobalGainEffect*> (iGlobalGain)->SetLevel(level);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SetOutputDevice
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetOutputDevice(QTMSAudioOutput device)
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iTmsGlobalRouting)
        {
        switch (device)
            {
            case QTMS_AUDIO_OUTPUT_NONE:
                status = iTmsGlobalRouting->SetOutput(QTMS_AUDIO_OUTPUT_NONE);
                DisplayText("Routing none");
                break;
            case QTMS_AUDIO_OUTPUT_PUBLIC:
                status = iTmsGlobalRouting->SetOutput(QTMS_AUDIO_OUTPUT_PUBLIC);
                DisplayText("Routing public");
                break;
            case QTMS_AUDIO_OUTPUT_PRIVATE:
                status = iTmsGlobalRouting->SetOutput(
                        QTMS_AUDIO_OUTPUT_PRIVATE);
                DisplayText("Routing private");
                break;
            case QTMS_AUDIO_OUTPUT_HANDSET:
                status = iTmsGlobalRouting->SetOutput(
                        QTMS_AUDIO_OUTPUT_HANDSET);
                DisplayText("Routing to handset");
                break;
            case QTMS_AUDIO_OUTPUT_LOUDSPEAKER:
                status = iTmsGlobalRouting->SetOutput(
                        QTMS_AUDIO_OUTPUT_LOUDSPEAKER);
                DisplayText("Routing to Loudspeaker");
                break;
            case QTMS_AUDIO_OUTPUT_WIRED_ACCESSORY:
                status = iTmsGlobalRouting->SetOutput(
                        QTMS_AUDIO_OUTPUT_WIRED_ACCESSORY);
                DisplayText("Routing to Wired accessory");
                break;
            case QTMS_AUDIO_OUTPUT_ACCESSORY:
                status = iTmsGlobalRouting->SetOutput(
                        QTMS_AUDIO_OUTPUT_ACCESSORY);
                DisplayText("Routing to BT accessory");
                break;
            case QTMS_AUDIO_OUTPUT_ETTY:
                status = iTmsGlobalRouting->SetOutput(QTMS_AUDIO_OUTPUT_ETTY);
                DisplayText("Routing to TTY");
                break;
            default: // ENoPreference
                DisplayText("Default Device Routing");
                break;
            }
        }
    if (status != QTMS_RESULT_SUCCESS)
        {
        DisplayText("Routing failed: ", status);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::SetHandset
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetHandset()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iTmsGlobalRouting)
        {
        status = iTmsGlobalRouting->SetOutput(QTMS_AUDIO_OUTPUT_HANDSET);

        if (status != QTMS_RESULT_SUCCESS)
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
// QTmsTestEngine::SetLoudSpeaker
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::SetLoudSpeaker()
    {
    gint status(QTMS_RESULT_SUCCESS);

    if (iTmsGlobalRouting)
        {
        status = iTmsGlobalRouting->SetOutput(QTMS_AUDIO_OUTPUT_LOUDSPEAKER);

        if (status != QTMS_RESULT_SUCCESS)
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
// QTmsTestEngine::GetAudioDevice
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetAudioDevice()
    {
    QTMSAudioOutput device;

    if (iTmsGlobalRouting)
        {
        iTmsGlobalRouting->GetOutput(device);
#ifdef __TEST_CODE_COVERAGE__
        TMSAudioOutput prevdevice;
        iTmsGlobalRouting->GetPreviousOutput(prevdevice);
#endif //__TEST_CODE_COVERAGE__

        switch (device)
            {
            case QTMS_AUDIO_OUTPUT_NONE:
                DisplayText("Routing none");
                break;
            case QTMS_AUDIO_OUTPUT_PUBLIC:
                DisplayText("Routing public");
                break;
            case QTMS_AUDIO_OUTPUT_PRIVATE:
                DisplayText("Routing private");
                break;
            case QTMS_AUDIO_OUTPUT_HANDSET:
                DisplayText("Routing to handset");
                break;
            case QTMS_AUDIO_OUTPUT_LOUDSPEAKER:
                DisplayText("Routing to Loudspeaker");
                break;
            case QTMS_AUDIO_OUTPUT_WIRED_ACCESSORY:
                DisplayText("Routing to Wired accessory");
                break;
            case QTMS_AUDIO_OUTPUT_ACCESSORY:
                DisplayText("Routing to BT accessory");
                break;
            case QTMS_AUDIO_OUTPUT_ETTY:
                DisplayText("Routing to TTY");
                break;
            default: // ENoPreference
                DisplayText("Default Device Routing");
                break;
            }
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::GetAvailableOutput
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::GetAvailableOutput()
    {
    TInt status(QTMS_RESULT_SUCCESS);

    if (iTmsGlobalRouting)
        {
        status = iTmsGlobalRouting->GetAvailableOutputs(iAvailableoutputs);

        if (status == QTMS_RESULT_SUCCESS)
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
// QTmsTestEngine::DisplayDevice
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::DisplayDevice(QTMSAudioOutput device)
    {
    switch (device)
        {
        case QTMS_AUDIO_OUTPUT_NONE:
            DisplayText("none");
            break;
        case QTMS_AUDIO_OUTPUT_PUBLIC:
            DisplayText("Public");
            break;
        case QTMS_AUDIO_OUTPUT_PRIVATE:
            DisplayText("Private");
            break;
        case QTMS_AUDIO_OUTPUT_HANDSET:
            DisplayText("Handset");
            break;
        case QTMS_AUDIO_OUTPUT_LOUDSPEAKER:
            DisplayText("Loudspeaker");
            break;
        case QTMS_AUDIO_OUTPUT_WIRED_ACCESSORY:
            DisplayText("Wired accessory");
            break;
        case QTMS_AUDIO_OUTPUT_ACCESSORY:
            DisplayText("Accessory");
            break;
        case QTMS_AUDIO_OUTPUT_ETTY:
            DisplayText("TTY");
            break;
        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::InitDTMFTonePlayerDnlink
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::InitDTMFTonePlayerDnlink()
    {
    gint status(QTMS_RESULT_SUCCESS);
    if (iFactory)
        {
        status = iFactory->CreateDTMF(QTMS_STREAM_DOWNLINK, iDTMFTonePlayerDn);
        }
    if (iDTMFTonePlayerDn && status == QTMS_RESULT_SUCCESS)
        {
        connect(iDTMFTonePlayerDn,
                SIGNAL(DTMFEvent(QTMSDTMF*,QTMSSignalEvent)), this,
                SLOT(DTMFEvent(QTMSDTMF*,QTMSSignalEvent)));
        }
    if (status == QTMS_RESULT_SUCCESS)
        {
        DisplayText("DTMF Downlink OK");
        }
    else
        {
        DisplayText("DTMF Downlink failed: ", status);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::InitDTMFTonePlayerUplink
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::InitDTMFTonePlayerUplink()
    {
    gint status(QTMS_RESULT_SUCCESS);
    if (iFactory)
        {
        status = iFactory->CreateDTMF(QTMS_STREAM_UPLINK, iDTMFTonePlayerUp);
        }
    if (iDTMFTonePlayerUp && status == QTMS_RESULT_SUCCESS)
        {
        connect(iDTMFTonePlayerUp,
                SIGNAL(DTMFEvent(QTMSDTMF*,QTMSSignalEvent)), this,
                SLOT(DTMFEvent(QTMSDTMF*,QTMSSignalEvent)));
        }
    if (status == QTMS_RESULT_SUCCESS)
        {
        DisplayText("DTMF Uplink OK");
        }
    else
        {
        DisplayText("DTMF Uplink failed: ", status);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::DTMFTonePlayDnlink
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::DTMFTonePlayDnlink()
    {
    GString* dtmfstring;
    gint status(QTMS_RESULT_SUCCESS);
    if (iDTMFTonePlayerDn)
        {
        dtmfstring = g_string_new("4723");
        status = iDTMFTonePlayerDn->SetTone(dtmfstring);
        if (status == QTMS_RESULT_SUCCESS)
            {
            status = iDTMFTonePlayerDn->Start();
            if (status != QTMS_RESULT_SUCCESS)
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
// QTmsTestEngine::DTMFTonePlayUplink
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::DTMFTonePlayUplink()
    {
    GString* dtmfstring;
    gint status(QTMS_RESULT_SUCCESS);
    // Note: uplink must be in the streaming state (disable for testing)
    if (iDTMFTonePlayerUp && iUpLinkStatus == EStreaming)
        {
        dtmfstring = g_string_new("4567890*#123");
        //dtmfstring = g_string_append_c(dtmfstring, '4');
        status = iDTMFTonePlayerUp->SetTone(dtmfstring);
        if (status == QTMS_RESULT_SUCCESS)
            {
#ifdef __TEST_CODE_COVERAGE__
            // CS call only
            iDTMFTonePlayerUp->ContinueDTMFStringSending(TRUE);
#endif //__TEST_CODE_COVERAGE__

            status = iDTMFTonePlayerUp->Start();

            if (status != QTMS_RESULT_SUCCESS)
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
// QTmsTestEngine::CloseDTMFPlayerDnlink
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::CloseDTMFPlayerDnlink()
    {
    if (iFactory && iDTMFTonePlayerDn)
        {
        iDTMFTonePlayerDn->Stop();
        iFactory->DeleteDTMF(iDTMFTonePlayerDn);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::CloseDTMFPlayerUplink
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::CloseDTMFPlayerUplink()
    {
    if (iFactory && iDTMFTonePlayerUp)
        {
        iDTMFTonePlayerUp->Stop();
        iFactory->DeleteDTMF(iDTMFTonePlayerUp);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::InitRingTonePlayerFromProfiles
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::InitRingTonePlayerFromProfiles()
    {
    if (iTmsRingTonePlayer)
        {
        gint status = iTmsRingTonePlayer->Init(QTMS_RINGTONE_DEFAULT);
        DisplayText("RT Init Profile: ", status);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::InitRingTonePlayerFromFile
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::InitRingTonePlayerFromFile()
    {
    if (iTmsRingTonePlayer)
        {
        TBuf<sizeof(KTestFile1)> buf(KTestFile1);
        iRTStr = g_string_new_len((gchar*) buf.Ptr(), buf.Length() * 2);
        gint status = iTmsRingTonePlayer->Init(QTMS_RINGTONE_FILE, iRTStr);
        DisplayText("RT Init File: ", status);
        g_string_free(iRTStr, TRUE);
        iRTStr = NULL;
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::InitRingToneVideoPlayer
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::InitRingToneVideoPlayer()
    {
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::InitRingToneSequencePlayer
// Creates sequence player to play custom sequence in descriptor format
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::InitRingToneSequencePlayer()
    {
    if (iTmsRingTonePlayer)
        {
        TBuf8<sizeof(KRTBeepSequence)> buf(KRTBeepSequence);
        iRTStr = g_string_new_len((gchar*) buf.Ptr(), buf.Length());
        gint status = iTmsRingTonePlayer->Init(QTMS_RINGTONE_SEQUENCE, iRTStr);
        DisplayText("RT Init Sequence: ", status);
        g_string_free(iRTStr, TRUE);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::InitRingToneBeepOnce
// Creates sequence player to play single beep
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::InitRingToneBeepOnce()
    {
    if (iTmsRingTonePlayer)
        {
        gint status = iTmsRingTonePlayer->Init(QTMS_RINGTONE_BEEP_ONCE);
        DisplayText("RT Init BeepOnce: ", status);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::InitRingToneSilent
// Creates sequence player to play silent tone
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::InitRingToneSilent()
    {
    if (iTmsRingTonePlayer)
        {
        gint status = iTmsRingTonePlayer->Init(QTMS_RINGTONE_SILENT);
        DisplayText("RT Init Silent: ", status);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::InitRingToneUnsecureVoIP
// Creates sequence player to play tone for unsecured VoIP call.
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::InitRingToneUnsecureVoIP()
    {
    if (iTmsRingTonePlayer)
        {
	    gint status = iTmsRingTonePlayer->Init(QTMS_RINGTONE_UNSECURE_VOIP);
        DisplayText("RT Init UnsecVoIP: ", status);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::InitRingToneWithTTS
// Creates sequence player to play default RT with Text-To-Speech
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::InitRingToneWithTTS()
    {
    if (iTmsRingTonePlayer)
        {
        TBuf<sizeof(KTextToSpeak)> buf(KTextToSpeak);
        iTTSStr = g_string_new_len((gchar*) buf.Ptr(), buf.Length() * 2);
        gint status = iTmsRingTonePlayer->Init(QTMS_RINGTONE_DEFAULT, NULL,
                iTTSStr);
        DisplayText("RT Init TTS: ", status);
        g_string_free(iTTSStr, TRUE);
        }
    iTTSStr = NULL;
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::PlayRingTone
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::PlayRingTone()
    {
    if (iTmsRingTonePlayer)
        {
        iTmsRingTonePlayer->Play();
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::StopRingTone
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::StopRingTone()
    {
    if (iTmsRingTonePlayer)
        {
        iTmsRingTonePlayer->Stop();
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::MuteRingTone
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::MuteRingTone()
    {
    if (iTmsRingTonePlayer)
        {
        iTmsRingTonePlayer->Mute();
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::PauseVideoRingTone
// Pause audio for video RT only
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::PauseVideoRingTone()
    {
    if (iTmsRingTonePlayer)
        {
        iTmsRingTonePlayer->Pause();
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::DeinitRingTonePlayer
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::DeinitRingTonePlayer()
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
void QTmsTestEngine::CloseRingTonePlayer()
    {
    if (iFactory && iTmsRingTonePlayer)
        {
        DeinitRingTonePlayer();
        iFactory->DeleteRingTonePlayer(iTmsRingTonePlayer);
        DisplayText("RT Player Closed");
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::CreateInbandTonePlayer
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::CreateInbandTonePlayer()
    {
    gint status(QTMS_RESULT_SUCCESS);
    if (iFactory && !iInbandTonePlayer)
        {
        status = iFactory->CreateInbandTonePlayer(iInbandTonePlayer);
        if (iInbandTonePlayer && status == QTMS_RESULT_SUCCESS)
            {
            connect(iInbandTonePlayer,
                    SIGNAL(InbandToneEvent(QTMSInbandTone*, QTMSSignalEvent)),
                    this,
                    SLOT(InbandToneEvent(QTMSInbandTone* , QTMSSignalEvent)));
            DisplayText("Inband Tone Player created");
            }
        else
            {
            DisplayText("Inband Tone Player failed: ", status);
            }
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::StartInbandTone
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::StartInbandTone(QTMSInbandToneType inbandtone)
    {
    gint status(QTMS_RESULT_SUCCESS);
    CreateInbandTonePlayer();
    if (iFactory && iInbandTonePlayer)
        {
        status = iInbandTonePlayer->Start(inbandtone);
        if (status == QTMS_RESULT_SUCCESS)
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
// QTmsTestEngine::StopInbandTone
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::StopInbandTone()
    {
    gint status(QTMS_RESULT_SUCCESS);
    if (iFactory && iInbandTonePlayer)
        {
        status = iInbandTonePlayer->Stop();
        if (status == QTMS_RESULT_SUCCESS)
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
// QTmsTestEngine::OneTouchLoopback
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::OneTouchLoopback()
    {
    iOneTouchLoopback = ETrue;

    // Change between CS and IP call type for testing.
    //SetCallType(QTMS_CALL_CS); //will activate streams
    SetCallType(QTMS_CALL_IP);

    if (iTmsCall->GetCallType() == QTMS_CALL_IP)
        {
#ifdef __WINSCW__
        SetDownlinkFormat(QTMS_FORMAT_PCM);
        SetUplinkFormat(QTMS_FORMAT_PCM);
#else  //__WINSCW__
        SetDownlinkFormat(QTMS_FORMAT_AMR);
        SetUplinkFormat(QTMS_FORMAT_AMR);
#endif //__WINSCW__
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::DoLoopback
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::DoLoopback()
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
            static_cast<QTMSClientSource*> (iTmsClientSource)->BufferFilled(
                    *iPlayBuf);
            iPlayBufReady = EFalse; // buf filled, ready for next FillBuffer
            iBufIndex = 0;
            }
#else //__WINSCW__
        Mem::Copy(desptr, srcptr, srcsize);
        iPlayBuf->SetDataSize(srcsize);
        static_cast<QTMSClientSource*> (iTmsClientSource)->BufferFilled(
                *iPlayBuf);
        iPlayBufReady = EFalse; // buf filled, ready for FillBuffer
#endif //__WINSCW__
        iRecBufReady = EFalse; // buf consumed, ready for next EmptyBuffer
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine::EndCall
//
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::EndCall()
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
        iFactory->DeleteEffect(iTmsDnlinkEffect);
        }
    if (iFactory && iTmsUplinkEffect)
        {
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

void QTmsTestEngine::TMSStreamEvent(const QTMSStream& stream,
        QTMSSignalEvent event)
    {
    switch (const_cast<QTMSStream&> (stream).GetStreamType())
        {
        case QTMS_STREAM_UPLINK:
            {
            switch (event.type)
                {
                case QTMS_EVENT_STREAM_STATE_CHANGED:
                    {
                    switch (event.curr_state)
                        {
                        case QTMS_STREAM_INITIALIZED:
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
                        case QTMS_STREAM_UNINITIALIZED:
                            DisplayText("Uplink uninitialized ");
                            break;
                        case QTMS_STREAM_PAUSED:
                            DisplayText("Uplink paused ");
                            break;
                        case QTMS_STREAM_STARTED:
                            DisplayText("Uplink started ");
                            break;
                        default:
                            break;
                        }
                    break;
                    }
                case QTMS_EVENT_STREAM_STATE_CHANGE_ERROR:
                    DisplayText("Uplink Error ", event.reason);
                    break;
                default:
                    break;
                }
            break;
            }
        case QTMS_STREAM_DOWNLINK:
            {
            switch (event.type)
                {
                case QTMS_EVENT_STREAM_STATE_CHANGED:
                    {
                    switch (event.curr_state)
                        {
                        case QTMS_STREAM_INITIALIZED:
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
                        case QTMS_STREAM_UNINITIALIZED:
                            DisplayText("Downlink uninitialized");
                            break;
                        case QTMS_STREAM_PAUSED:
                            DisplayText("Downlink paused");
                            break;
                        case QTMS_STREAM_STARTED:
                            DisplayText("Downlink started");
                            break;
                        default:
                            break;
                        }
                    break;
                    }
                case QTMS_EVENT_STREAM_STATE_CHANGE_ERROR:
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
void QTmsTestEngine::FillBuffer(QTMSBuffer& buffer)
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
    static_cast<QTMSClientSource*> (iTmsClientSource)->BufferFilled(*iPlayBuf);
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
void QTmsTestEngine::BufferProcessed(const QTMSBuffer* /*buffer*/,
        gint /*reason*/)
    {
    }

// From TMSClientSinkObserver
void QTmsTestEngine::ProcessBuffer(const QTMSBuffer* buffer)
    {
    iRecBufReady = ETrue;
    iRecBuf = const_cast<QTMSBuffer*> (buffer);

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
        static_cast<QTMSClientSink*> (iTmsClientSink)->BufferProcessed(
                iRecBuf);
        }
    }

// From TMSEffectObserver
void QTmsTestEngine::EffectsEvent(const QTMSEffect& tmseffect,
        QTMSSignalEvent event)
    {
    gint reason = event.reason;

    if (reason == QTMS_RESULT_SUCCESS)
        {
        QTMSEffectType effecttype;
        const_cast<QTMSEffect&> (tmseffect).GetType(effecttype);
        switch (effecttype)
            {
            case QTMS_EFFECT_GLOBAL_VOL:
                {
                switch (event.type)
                    {
                    case QTMS_EVENT_EFFECT_VOL_CHANGED:
                        DisplayText("Global vol effect change");
                        QTMSVolumeEventChangeData* vd;
                        vd = static_cast<QTMSVolumeEventChangeData*>
                                (event.event_data);
                        DisplayText("Volume level: ", vd->level);
                        DisplayText("Output device: ", vd->output);
                        break;
                    default:
                        break;
                    }
                }
                break;
            case QTMS_EFFECT_GLOBAL_GAIN:
                {
                switch (event.type)
                    {
                    case QTMS_EVENT_EFFECT_GAIN_CHANGED:
                        DisplayText("Global gain effect change");
                        break;
                    default:
                        break;
                    }
                }
                break;
            case QTMS_EFFECT_VOLUME:
                {
                switch (event.type)
                    {
	                case QTMS_EVENT_EFFECT_VOL_CHANGED:
                        DisplayText("Stream vol effect change");
                        break;
                    default:
                        break;
                    }
                }
                break;
	        case QTMS_EFFECT_GAIN:
                {
                switch (event.type)
                    {
	                case QTMS_EVENT_EFFECT_GAIN_CHANGED:
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
void QTmsTestEngine::GlobalRoutingEvent(const QTMSGlobalRouting& /*routing*/,
        QTMSSignalEvent event, QTMSAudioOutput output)
    {
    gint reason = event.reason;

    if (reason == QTMS_RESULT_SUCCESS)
        {
        switch (event.type)
            {
            case QTMS_EVENT_ROUTING_AVAIL_OUTPUTS_CHANGED:
                DisplayText("Available outputs changed");
                break;
            case QTMS_EVENT_ROUTING_OUTPUT_CHANGED:
                DisplayText("output changed");
                break;
            case QTMS_EVENT_ROUTING_SET_OUTPUT_COMPLETE:
                DisplayText("set output complete");
                break;
            default:
                break;
            }
        DisplayDevice(output);
        }
    else
        {
        DisplayText("Routing failed: ", reason);
        }
    }

// From TMSRingToneObserver
void QTmsTestEngine::RingtoneEvent(const QTMSRingTone& /*rt*/,
        QTMSSignalEvent event)
    {
    gint reason = event.reason;

    if (reason == QTMS_RESULT_SUCCESS)
        {
        switch (event.type)
            {
            case QTMS_EVENT_RINGTONE_OPEN_COMPLETE:
                DisplayText("RT Open Complete");
                break;
            case QTMS_EVENT_RINGTONE_PLAY_COMPLETE:
                DisplayText("RT Play Complete");
                break;
            case QTMS_EVENT_RINGTONE_DEINIT_COMPLETE:
                DisplayText("RT Deinit Complete");
                break;
            default:
                break;
            }
        }
    else
        {
        DisplayText("Ringtone failed: ", reason);
        }
    }

// From TMSDTMFObserver
void QTmsTestEngine::DTMFEvent(const QTMSDTMF& /*dtmf*/, QTMSSignalEvent event)
    {
    gint reason = event.reason;

    if (reason == QTMS_RESULT_SUCCESS)
        {
        switch (event.type)
            {
            case QTMS_EVENT_DTMF_TONE_STARTED:
                DisplayText("DTMF Started");
                break;
            case QTMS_EVENT_DTMF_TONE_STOPPED:
                DisplayText("DTMF Stopped");
                break;
            default:
                break;
            }
        }
    else
        {
        DisplayText("DTMF failed: ", reason);
        }
    }

// From TMSInbandToneObserver
void QTmsTestEngine::InbandToneEvent(const QTMSInbandTone& /*inbandtone*/,
        QTMSSignalEvent event)
    {
    gint reason = event.reason;

    if (reason == QTMS_RESULT_SUCCESS)
        {
        switch (event.type)
            {
            case QTMS_EVENT_INBAND_TONE_STARTED:
                DisplayText("Inband Tone Started");
                break;
            case QTMS_EVENT_INBAND_TONE_STOPPED:
                DisplayText("Inband Tone Stopped");
                break;
            default:
                break;
            }
        }
    else
        {
        DisplayText("Inband tone failed: ", reason);
        }
    }

// ----------------------------------------------------------------------------
// QTmsTestEngine:://DisplayText
// Print text with status code.
// ----------------------------------------------------------------------------
//
void QTmsTestEngine::DisplayText(const QString& str, const gint num)
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
