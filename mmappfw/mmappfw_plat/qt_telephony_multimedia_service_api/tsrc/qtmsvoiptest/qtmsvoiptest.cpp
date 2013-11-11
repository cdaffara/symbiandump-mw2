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
 * Description: QTMS Voip test
 *
 */
#include <QObject>
#include <QtTest/QtTest>
#include "qtestmains60.h"

#include <qtms.h>
#include <qtmsfactory.h>
#include <qtmscall.h>
#include <qtmsstream.h>
#include <tmsstreamobsrvr.h>
#include <qtmsformat.h>

#include <qtmspcmformat.h>
#include <qtmsamrformat.h>
#include <qtmsilbcformat.h>
#include <qtmsg711format.h>
#include <qtmsg729format.h>

#include <qtmsclientsink.h>
#include <qtmsclientsource.h>
#include <qtmsvolumeeffect.h>
#include <qtmsgaineffect.h>
#include <qtmsglobalvoleffect.h>
#include <qtmsglobalgaineffect.h>
#include <qtmsglobalrouting.h>

#include <qtmsbuffer.h>
#include <qtmsdtmf.h>
#include <qtmsringtone.h>
#include <qtmsinbandtone.h>
#include <tmsbuffer.h>

// CONSTANTS
#define __TEST_CODE_COVERAGE__

using namespace QTMS;


class QTmsVoipTest: public QObject
{
Q_OBJECT
private slots:
    void initTestCase();
    void init();
    void cleanup();
    void SupportedFormats();
    void CreateRouting_data();
    void CreateRouting();
    void CreateGlobalVol();
    void CreateGlobalGain();
    void CreateFormat_data();
    void CreateFormat();
    void CreateStreamVol();
    void CreateStreamGain();
    void CreateDnlinkDtmf();
    void CreateBuffer();

    void cleanupTestCase();
public slots:
    //From TMSClientSourceObserver
    void FillBuffer(QTMSBuffer& buffer);
    void BufferProcessed(QTMSBuffer* buffer, gint reason);
    //From TMSClientSinkObserver
    void ProcessBuffer(const QTMSBuffer* buffer);
    //From TMSStreamObserver
    void TMSStreamEvent(const QTMSStream& stream, QTMSSignalEvent event);
    //From TMSClientSinkObserver
    void EffectsEvent(const QTMSEffect& tmseffect, QTMSSignalEvent event);
    //From TMSGlobalRoutingObserver
    void GlobalRoutingEvent(const QTMSGlobalRouting& routing, QTMSSignalEvent event,
        QTMSAudioOutput output);
    //From TMSRingToneObserver
    void RingtoneEvent(const QTMSRingTone& rt, QTMSSignalEvent event);
    //From TMSDTMFObserver
    void DTMFEvent(const QTMSDTMF& dtmf, QTMSSignalEvent event);

private:

    void VoipCallSetup();

    void SupportedUplinkFormats();
    void SupportedDnlinkFormats();

    void CreateUplink();
    void CreateDnlink();

    void CreateMicSource();
    void AddMicSourceToStream();

    void CreateClientSink();
    void AddClientSinkToStream();

    void CreateClientSource();
    void AddClientSourceToStream();

    void CreateSpeakerSink();
    void AddSpeakerSinkToStream();

    void InitUplink();
    void InitDnlink();

    void DeinitUplink();
    void DeinitDnlink();

    void StartUplink();
    void StopUplink();
    void StartDnlink();

    void ResetUplnkFormat();
    void ResetDnlnkFormat();

    void StreamVol();
    void StreamGain();

    void CloseUplink();
    void CloseDownlink();

    void DisplayFormat(QTMSFormatType format);
    void StopDnlink();
    void CreateRingTone();

    void ToggleVad();
    void GetVad();
    void ToggleCng();
    void GetCng();
    void TogglePlc();
    void GetPlc();

    void SelectMinBitrate();
    void SelectMaxBitrate();
    void SetBitrate(guint bitrate);
    void GetSupportedBitrates();
    gint GetSupportedBitrates(BitRateVector& aBrArr);
    void GetBitrate();

    void SetDnLinkG711ALAW();
    void SetDnLinkG711uLAW();
    void SetDnLinkILBC20MS();
    void SetDnLinkILBC30MS();
    void SetUpLinkG711ALAW();
    void SetUpLinkG711uLAW();
    void SetUpLinkILBC20MS();
    void SetUpLinkILBC30MS();
    void GetDnLinkG711Mode();
    void GetDnLinkILBCMode();
    void GetUpLinkG711Mode();
    void GetUpLinkILBCMode();

    void DisplayDevice(QTMSAudioOutput device);

private:
    QTMSFactory *m_Factory;
    QTMSCall *m_Call;
    QTMSDTMF *m_Dnlnkdtmf;
    QTMSStream *m_Dnlink;
    QTMSStream *m_Uplink;

    QTMSSource *m_MicSource;
    QTMSSource *m_ClientSource;
    QTMSSink *m_ClientSink;
    QTMSSink *m_SpeakerSink;

    QTMSFormat *m_G711EncFormatIntfc;
    QTMSFormat *m_G711DecFormatIntfc;
    QTMSFormat *m_G729EncFormatIntfc;
    QTMSFormat *m_G729DecFormatIntfc;
    QTMSFormat *m_ILBCEncFormatIntfc;
    QTMSFormat *m_ILBCDecFormatIntfc;
    QTMSFormat *m_AMRNBEncFormatIntfc;
    QTMSFormat *m_AMRNBDecFormatIntfc;
    QTMSFormat *m_PCM16EncFormatIntfc;
    QTMSFormat *m_PCM16DecFormatIntfc;

    QTMS::FormatVector m_UplCodecs;
    QTMS::FormatVector m_DnlCodecs;

    BitRateVector m_BitratesVector;
    guint iBitrate;

    QTMSEffect *m_Vol;
    QTMSEffect *m_Gain;
    QTMSEffect *m_GlobalVol;
    QTMSEffect *m_GlobalGain;
    QTMSGlobalRouting *m_Routing;

    QTMSFormatType m_DnLinkCodec;
    QTMSFormatType m_UpLinkCodec;

    QSignalSpy *m_UplinkSpy;
    QSignalSpy *m_DnlinkSpy;
    QSignalSpy *m_RoutingSpy;

    gboolean m_Vad;
    gboolean m_Cng;
    gboolean m_Plc;

};

void QTmsVoipTest::initTestCase()
{
    qDebug("Start QTms Voip tests");

    g_setenv("G_SLICE", "always-malloc", 1);

    qRegisterMetaType<QTMSSignalEvent> ("QTMSSignalEvent");
    qRegisterMetaType<QTMSAudioOutput> ("QTMSAudioOutput");
}

void QTmsVoipTest::cleanupTestCase()
{
    qDebug("End QTms tests");
}

void QTmsVoipTest::init()
{
    qDebug("****NEW TEST****");
    m_Factory = NULL;
    m_Call = NULL;
    m_Dnlink = NULL;
    m_Uplink = NULL;
    m_MicSource = NULL;
    m_ClientSource = NULL;
    m_SpeakerSink = NULL;
    m_ClientSink = NULL;
    m_Vol = NULL;
    m_Gain = NULL;
    m_GlobalVol = NULL;
    m_GlobalGain = NULL;
    m_Routing = NULL;
    m_Dnlnkdtmf = NULL;

    m_G711EncFormatIntfc = NULL;
    m_G711DecFormatIntfc = NULL;
    m_G729EncFormatIntfc = NULL;
    m_G729DecFormatIntfc = NULL;
    m_ILBCEncFormatIntfc = NULL;
    m_ILBCDecFormatIntfc = NULL;
    m_AMRNBEncFormatIntfc = NULL;
    m_AMRNBDecFormatIntfc = NULL;
    m_PCM16EncFormatIntfc = NULL;
    m_PCM16DecFormatIntfc = NULL;

    m_UplinkSpy = NULL;
    m_DnlinkSpy = NULL;

    QTMSVer *ver(NULL);
    gint status;
    status = QTMSFactory::CreateFactory(m_Factory, *ver);

    QVERIFY(status == QTMS_RESULT_SUCCESS);

    VoipCallSetup();

    CreateUplink();
    CreateDnlink();

    CreateMicSource();
    AddMicSourceToStream();

    CreateClientSink();
    AddClientSinkToStream();

    CreateClientSource();
    AddClientSourceToStream();

    CreateSpeakerSink();
    AddSpeakerSinkToStream();

}
void QTmsVoipTest::cleanup()
{
    qDebug("****END TEST******");

    ResetUplnkFormat();
    ResetDnlnkFormat();

    CloseUplink();
    CloseDownlink();

    if (m_MicSource) {
        m_Factory->DeleteSource(m_MicSource);
    }
    if (m_ClientSource) {
        m_Factory->DeleteSource(m_ClientSource);
    }
    if (m_ClientSink) {
        m_Factory->DeleteSink(m_ClientSink);
    }
    if (m_SpeakerSink) {
        m_Factory->DeleteSink(m_SpeakerSink);
    }

    if (m_Dnlnkdtmf) {
        m_Factory->DeleteDTMF(m_Dnlnkdtmf);
    }

    if (m_Vol) {
        m_Factory->DeleteEffect(m_Vol);
    }
    if (m_Gain) {
        m_Factory->DeleteEffect(m_Gain);
    }
    if (m_GlobalVol) {
        m_Factory->DeleteEffect(m_GlobalVol);
    }
    if (m_GlobalGain) {
        m_Factory->DeleteEffect(m_GlobalGain);
    }

    if (m_Routing) {
        m_Factory->DeleteGlobalRouting(m_Routing);
    }

    if (m_PCM16EncFormatIntfc) {
        m_Factory->DeleteFormat(m_PCM16EncFormatIntfc);
    }
    if (m_ILBCEncFormatIntfc) {
        m_Factory->DeleteFormat(m_ILBCEncFormatIntfc);
    }
    if (m_AMRNBEncFormatIntfc) {
        m_Factory->DeleteFormat(m_AMRNBEncFormatIntfc);
    }
    if (m_G711EncFormatIntfc) {
        m_Factory->DeleteFormat(m_G711EncFormatIntfc);
    }
    if (m_G729EncFormatIntfc) {
        m_Factory->DeleteFormat(m_G729EncFormatIntfc);
    }
    if (m_PCM16DecFormatIntfc) {
        m_Factory->DeleteFormat(m_PCM16DecFormatIntfc);
    }
    if (m_ILBCDecFormatIntfc) {
        m_Factory->DeleteFormat(m_ILBCDecFormatIntfc);
    }
    if (m_AMRNBDecFormatIntfc) {
        m_Factory->DeleteFormat(m_AMRNBDecFormatIntfc);
    }
    if (m_G711DecFormatIntfc) {

        m_Factory->DeleteFormat(m_G711DecFormatIntfc);
    }
    if (m_G729DecFormatIntfc) {
        m_Factory->DeleteFormat(m_G729DecFormatIntfc);
    }

    if (m_Uplink) {
        m_Call->DeleteStream(m_Uplink);
    }
    if (m_Dnlink) {
        m_Call->DeleteStream(m_Dnlink);
    }

    if (m_Call) {
        m_Factory->DeleteCall(m_Call);
    }

    if (m_Factory) {
        delete m_Factory;
    }

}

void QTmsVoipTest::VoipCallSetup()
{
    gint status(QTMS_RESULT_SUCCESS);
    if (m_Factory && !m_Call) {
        gboolean issupported(FALSE);
        m_Factory->IsCallTypeSupported(QTMS_CALL_IP, issupported);
        QVERIFY(issupported == TRUE);
        status = m_Factory->CreateCall(QTMS_CALL_IP, m_Call, 0);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        guint ctxid;
        m_Call->GetCallContextId(ctxid);
    }

}

void QTmsVoipTest::SupportedFormats()
{
    SupportedUplinkFormats();
    SupportedDnlinkFormats();
}

void QTmsVoipTest::SupportedUplinkFormats()
{
    gint status(QTMS_RESULT_SUCCESS);
    status = m_Factory->GetSupportedFormats(QTMS_STREAM_UPLINK, m_UplCodecs);
    QVERIFY(status == QTMS_RESULT_SUCCESS);

    qDebug("Supported UPL codecs");
    std::vector<QTMSFormat*>::iterator codecs = m_UplCodecs.begin();
    QTMSFormatType fmttype;
    for (; codecs < m_UplCodecs.end(); codecs++) {
        (*codecs)->GetType(fmttype);
        DisplayFormat(fmttype);
    }
    gint size = m_UplCodecs.size();
    for (gint i = 0; i < size; i++) {
        codecs = m_UplCodecs.begin();
        m_Factory->DeleteFormat(*codecs);
        m_UplCodecs.erase(codecs);
    }

}

void QTmsVoipTest::SupportedDnlinkFormats()
{
    gint status(QTMS_RESULT_SUCCESS);
    status = m_Factory->GetSupportedFormats(QTMS_STREAM_DOWNLINK, m_DnlCodecs);
    QVERIFY(status == QTMS_RESULT_SUCCESS);

    qDebug("Supported DNL codecs");
    std::vector<QTMSFormat*>::iterator codecs = m_DnlCodecs.begin();
    QTMSFormatType fmttype;
    for (; codecs < m_DnlCodecs.end(); codecs++) {
        (*codecs)->GetType(fmttype);
        DisplayFormat(fmttype);
    }
    gint size = m_DnlCodecs.size();
    for (gint i = 0; i < size; i++) {
        codecs = m_DnlCodecs.begin();
        m_Factory->DeleteFormat(*codecs);
        m_DnlCodecs.erase(codecs);
    }
}

void QTmsVoipTest::DisplayFormat(QTMSFormatType format)
{
    switch (format) {
    case QTMS_FORMAT_AMR:
        qDebug("AMR-NB");
        break;
    case QTMS_FORMAT_G711:
        qDebug("G.711");
        break;
    case QTMS_FORMAT_G729:
        qDebug("G.729");
        break;
    case QTMS_FORMAT_ILBC:
        qDebug("iLBC");
        break;
    case QTMS_FORMAT_PCM:
        qDebug("PCM-16");
        break;
    default:
        break;
    }
}

void QTmsVoipTest::CreateUplink()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Call) {
        status = m_Call->CreateStream(QTMS_STREAM_UPLINK, m_Uplink);

        m_UplinkSpy = new QSignalSpy(m_Uplink,
            SIGNAL(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)));

        QVERIFY( m_UplinkSpy->isValid() );

        // QCOMPARE(m_UplinkSpy->count(), 0 );

        QVERIFY(status == QTMS_RESULT_SUCCESS);
        connect(m_Uplink, SIGNAL(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)), this,
            SLOT(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)));
    }
}

void QTmsVoipTest::CreateDnlink()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Call) {
        status = m_Call->CreateStream(QTMS_STREAM_DOWNLINK, m_Dnlink);

        QVERIFY(status == QTMS_RESULT_SUCCESS);

        m_DnlinkSpy = new QSignalSpy(m_Dnlink,
            SIGNAL(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)));

        QVERIFY( m_DnlinkSpy->isValid() );

        // QCOMPARE(m_DnlinkSpy->count(), 0 );

        connect(m_Dnlink, SIGNAL(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)), this,
            SLOT(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)));
    }
}

void QTmsVoipTest::CreateClientSource()
{
    gint status(QTMS_RESULT_SUCCESS);
    if (m_Factory && !m_ClientSource) {
        status = m_Factory->CreateSource(QTMS_SOURCE_CLIENT, m_ClientSource);
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsVoipTest::AddClientSourceToStream()
{
    gint status(QTMS_RESULT_SUCCESS);
    if (m_Dnlink && m_ClientSource) {
        connect(static_cast<QTMSClientSource*> (m_ClientSource),
            SIGNAL(FillBuffer(QTMSBuffer&)), this, SLOT(FillBuffer(QTMSBuffer&)));
        status = m_Dnlink->AddSource(m_ClientSource);

#ifdef __TEST_CODE_COVERAGE__
        QTMSSourceType st;
        static_cast<QTMSClientSource*> (m_ClientSource)->GetType(st);
        gboolean enqueue(FALSE);
        static_cast<QTMSClientSource*> (m_ClientSource)->GetEnqueueMode(enqueue);
        static_cast<QTMSClientSource*> (m_ClientSource)->SetEnqueueMode(FALSE);
        static_cast<QTMSClientSource*> (m_ClientSource)->Flush();
#endif //__TEST_CODE_COVERAGE__
    }

    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsVoipTest::CreateClientSink()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Factory && !m_ClientSink) {
        status = m_Factory->CreateSink(QTMS_SINK_CLIENT, m_ClientSink);
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsVoipTest::AddClientSinkToStream()
{
    gint status(QTMS_RESULT_SUCCESS);
    if (m_Uplink && m_ClientSink) {
        connect(static_cast<QTMSClientSink*> (m_ClientSink),
            SIGNAL(ProcessBuffer(const QTMSBuffer*)), this,
            SLOT(ProcessBuffer(const QTMSBuffer*)));
        status = m_Uplink->AddSink(m_ClientSink);
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsVoipTest::CreateMicSource()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Factory && !m_MicSource) {
        status = m_Factory->CreateSource(TMS_SOURCE_MIC, m_MicSource);
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsVoipTest::AddMicSourceToStream()
{
    gint status(QTMS_RESULT_SUCCESS);
    if (m_Uplink && m_MicSource) {
        status = m_Uplink->AddSource(m_MicSource);
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsVoipTest::CreateSpeakerSink()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Factory && !m_SpeakerSink) {
        status = m_Factory->CreateSink(QTMS_SINK_SPEAKER, m_SpeakerSink);
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsVoipTest::AddSpeakerSinkToStream()
{
    gint status(QTMS_RESULT_SUCCESS);
    if (m_Dnlink && m_SpeakerSink) {
        status = m_Dnlink->AddSink(m_SpeakerSink);
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsVoipTest::InitUplink()
{
    QVERIFY(m_Uplink->Init() == QTMS_RESULT_SUCCESS);
    QTest::qWait(1000);
    //QCOMPARE(m_UplinkSpy->count(), 1 );

    GetSupportedBitrates();
    SelectMinBitrate();
    SelectMaxBitrate();
    GetBitrate();

    ToggleVad();
    GetVad();

    SetUpLinkG711ALAW();
    SetUpLinkG711uLAW();
    GetUpLinkG711Mode();

    SetUpLinkILBC20MS();
    SetUpLinkILBC30MS();
    GetUpLinkILBCMode();

}

void QTmsVoipTest::InitDnlink()
{
    QVERIFY(m_Dnlink->Init() == QTMS_RESULT_SUCCESS);
    QTest::qWait(1000);
    //QCOMPARE(m_DnlinkSpy->count(), 1 );
    ToggleCng();
    GetCng();
    TogglePlc();
    GetPlc();

    SetDnLinkG711ALAW();
    SetDnLinkG711uLAW();
    GetDnLinkG711Mode();

    SetDnLinkILBC20MS();
    SetDnLinkILBC30MS();
    GetDnLinkILBCMode();

}
void QTmsVoipTest::DeinitUplink()
{
    m_Uplink->Deinit();
    QTest::qWait(1000);
}

void QTmsVoipTest::DeinitDnlink()
{
    m_Dnlink->Deinit();
    QTest::qWait(1000);
}

void QTmsVoipTest::StartUplink()
{
    QVERIFY(m_Uplink->Start() == QTMS_RESULT_SUCCESS);
    QTest::qWait(1000);
    //QCOMPARE(m_UplinkSpy->count(), 2 );
}

void QTmsVoipTest::StartDnlink()
{
    QVERIFY(m_Dnlink->Start() == QTMS_RESULT_SUCCESS);
    QTest::qWait(1000);
    //QCOMPARE(m_DnlinkSpy->count(), 2 );
}
void QTmsVoipTest::StopUplink()
{
#ifdef __TEST_CODE_COVERAGE__
    QVERIFY(m_Uplink->Pause()== QTMS_RESULT_SUCCESS);
    QTest::qWait(1000);
#endif //__TEST_CODE_COVERAGE__
    QVERIFY(m_Uplink->Stop() == QTMS_RESULT_SUCCESS);
    QTest::qWait(1000);
    //QCOMPARE(m_UplinkSpy->count(), 3 );
}

void QTmsVoipTest::StopDnlink()
{
#ifdef __TEST_CODE_COVERAGE__
    QVERIFY(m_Dnlink->Pause() == QTMS_RESULT_SUCCESS);
    QTest::qWait(1000);
#endif //__TEST_CODE_COVERAGE__
    QVERIFY(m_Dnlink->Stop() == QTMS_RESULT_SUCCESS);
    QTest::qWait(1000);
    //QCOMPARE(m_DnlinkSpy->count(), 3 );
}

void QTmsVoipTest::CloseUplink()
{
    m_Uplink->Deinit();
    QTest::qWait(1000);
    if (m_Uplink) {
        if (m_Gain) {
            QVERIFY(m_Uplink->RemoveEffect(m_Gain)== QTMS_RESULT_SUCCESS);
        }
        if (m_MicSource) {
            QVERIFY(m_Uplink->RemoveSource(m_MicSource)== QTMS_RESULT_SUCCESS);
        }
        if (m_ClientSink) {
            QVERIFY(m_Uplink->RemoveSink(m_ClientSink)== QTMS_RESULT_SUCCESS);
        }
    }
}

void QTmsVoipTest::CloseDownlink()
{
    m_Dnlink->Deinit();
    QTest::qWait(1000);
    if (m_Dnlink) {
        if (m_Vol) {
            QVERIFY(m_Dnlink->RemoveEffect(m_Vol)== QTMS_RESULT_SUCCESS);
        }
        if (m_ClientSource) {
            QVERIFY(m_Dnlink->RemoveSource(m_ClientSource)== QTMS_RESULT_SUCCESS);
        }
        if (m_SpeakerSink) {
            QVERIFY(m_Dnlink->RemoveSink(m_SpeakerSink) == QTMS_RESULT_SUCCESS);
        }
    }

}

void QTmsVoipTest::ResetUplnkFormat()
{
    if (m_Uplink) {
        switch (m_UpLinkCodec) {
        case QTMS_FORMAT_PCM:
            m_Uplink->ResetFormat(m_PCM16EncFormatIntfc);
            break;
        case QTMS_FORMAT_AMR:
            m_Uplink->ResetFormat(m_AMRNBEncFormatIntfc);
            break;
        case QTMS_FORMAT_G711:
            m_Uplink->ResetFormat(m_G711EncFormatIntfc);
            break;
        case QTMS_FORMAT_G729:
            m_Uplink->ResetFormat(m_G729EncFormatIntfc);
            break;
        case QTMS_FORMAT_ILBC:
            m_Uplink->ResetFormat(m_ILBCEncFormatIntfc);
            break;
        default:
            break;
        }
    }

}

void QTmsVoipTest::ResetDnlnkFormat()
{
    if (m_Dnlink) {
        switch (m_DnLinkCodec) {
        case QTMS_FORMAT_PCM:
            m_Dnlink->ResetFormat(m_PCM16DecFormatIntfc);
            break;
        case QTMS_FORMAT_AMR:
            m_Dnlink->ResetFormat(m_AMRNBDecFormatIntfc);
            break;
        case QTMS_FORMAT_G711:
            m_Dnlink->ResetFormat(m_G711DecFormatIntfc);
            break;
        case QTMS_FORMAT_G729:
            m_Dnlink->ResetFormat(m_G729DecFormatIntfc);
            break;
        case QTMS_FORMAT_ILBC:
            m_Dnlink->ResetFormat(m_ILBCDecFormatIntfc);
            break;
        default:
            break;
        }
    }
}

void QTmsVoipTest::CreateFormat_data()
{
    QTest::addColumn<QTMSFormatType>("format");

#ifndef __WINSCW__
    QTest::newRow("G711")<<QTMS_FORMAT_G711;
    QTest::newRow("G729")<<QTMS_FORMAT_G729;
    QTest::newRow("iLBC")<<QTMS_FORMAT_ILBC;
    QTest::newRow("AMR")<<QTMS_FORMAT_AMR;
#endif
    QTest::newRow("PCM") << QTMS_FORMAT_PCM;
}

void QTmsVoipTest::CreateFormat()
{
    gint decstatus(QTMS_RESULT_SUCCESS);
    gint encstatus(QTMS_RESULT_SUCCESS);
    gint setencstatus(QTMS_RESULT_SUCCESS);
    gint setdecstatus(QTMS_RESULT_SUCCESS);
    QFETCH(QTMSFormatType, format);

    m_DnLinkCodec = format;
    m_UpLinkCodec = format;

    switch (format) {
    case QTMS_FORMAT_G711:
    {
        decstatus = m_Factory->CreateFormat(QTMS_FORMAT_G711, m_G711DecFormatIntfc);
        encstatus = m_Factory->CreateFormat(QTMS_FORMAT_G711, m_G711EncFormatIntfc);
        setdecstatus = m_Dnlink->SetFormat(m_G711DecFormatIntfc);
        setencstatus = m_Uplink->SetFormat(m_G711EncFormatIntfc);
        qDebug("G.711 Set");

    }
        break;
    case QTMS_FORMAT_G729:
    {
        decstatus = m_Factory->CreateFormat(QTMS_FORMAT_G729, m_G729DecFormatIntfc);
        encstatus = m_Factory->CreateFormat(QTMS_FORMAT_G729, m_G729EncFormatIntfc);
        setdecstatus = m_Dnlink->SetFormat(m_G729DecFormatIntfc);
        setdecstatus = m_Uplink->SetFormat(m_G729EncFormatIntfc);
        qDebug("G.729 Set");
    }
        break;
    case QTMS_FORMAT_ILBC:
    {
        decstatus = m_Factory->CreateFormat(QTMS_FORMAT_ILBC, m_ILBCDecFormatIntfc);
        encstatus = m_Factory->CreateFormat(QTMS_FORMAT_ILBC, m_ILBCEncFormatIntfc);
        setdecstatus = m_Dnlink->SetFormat(m_ILBCDecFormatIntfc);
        setencstatus = m_Uplink->SetFormat(m_ILBCEncFormatIntfc);
        qDebug("iLBC Set");
    }
        break;
    case QTMS_FORMAT_AMR:
    {
        decstatus = m_Factory->CreateFormat(QTMS_FORMAT_AMR, m_AMRNBDecFormatIntfc);
        encstatus = m_Factory->CreateFormat(QTMS_FORMAT_AMR, m_AMRNBEncFormatIntfc);
        setdecstatus = m_Dnlink->SetFormat(m_AMRNBDecFormatIntfc);
        setencstatus = m_Uplink->SetFormat(m_AMRNBEncFormatIntfc);
        qDebug("AMR-NB Set");
    }
        break;
    case QTMS_FORMAT_PCM:
    {
        decstatus = m_Factory->CreateFormat(QTMS_FORMAT_PCM, m_PCM16DecFormatIntfc);
        encstatus = m_Factory->CreateFormat(QTMS_FORMAT_PCM, m_PCM16EncFormatIntfc);
        setdecstatus = m_Dnlink->SetFormat(m_PCM16DecFormatIntfc);
        setencstatus = m_Uplink->SetFormat(m_PCM16EncFormatIntfc);
        qDebug("PCM-16 Set");
    }
        break;
    default:
        break;
    }

    QVERIFY(decstatus == QTMS_RESULT_SUCCESS);
    QVERIFY(encstatus == QTMS_RESULT_SUCCESS);
    QVERIFY(setdecstatus == QTMS_RESULT_SUCCESS);
    QVERIFY(setencstatus == QTMS_RESULT_SUCCESS);

    InitUplink();
    InitDnlink();
}

void QTmsVoipTest::ToggleVad()
{
    //m_Vad = (m_Vad) ? EFalse : ETrue;

    m_Vad = ETrue;
    switch (m_UpLinkCodec) {
    case QTMS_FORMAT_G711:
        static_cast<QTMSG711Format*> (m_G711EncFormatIntfc)->SetVADMode(m_Vad);
        qDebug("G711 VAD set %d", m_Vad);
        break;
    case QTMS_FORMAT_G729:
        static_cast<QTMSG729Format*> (m_G729EncFormatIntfc)->SetVADMode(m_Vad);
        qDebug("G729 VAD set %d", m_Vad);
        break;
    case QTMS_FORMAT_ILBC:
        static_cast<QTMSILBCFormat*> (m_ILBCEncFormatIntfc)->SetVADMode(m_Vad);
        qDebug("iLBC VAD set %d", m_Vad);
        break;
    case QTMS_FORMAT_AMR:
        //static_cast<TMSAMRFormat*>(iAMRNBEncFormatIntfc)->SetVADMode(m_Vad);
        break;
    case QTMS_FORMAT_PCM:
        // Not supported; just for testing failing condition
        //static_cast<QTMSPCMFormat*>(iPCM16EncFormatIntfc)->SetVADMode(m_Vad);
        break;
    default:
        break;
    }
}

void QTmsVoipTest::GetVad()
{
    gint status(QTMS_RESULT_SUCCESS);
    gboolean vad = EFalse;

    switch (m_UpLinkCodec) {
    case QTMS_FORMAT_G711:
        status = static_cast<QTMSG711Format*> (m_G711EncFormatIntfc)->GetVADMode(vad);
        qDebug("get G711 VAD: %d", vad);
        break;
    case QTMS_FORMAT_G729:
        status = static_cast<QTMSG729Format*> (m_G729EncFormatIntfc)->GetVADMode(vad);
        qDebug("get G729 VAD: %d", vad);
        break;
    case QTMS_FORMAT_ILBC:
        status = static_cast<QTMSILBCFormat*> (m_ILBCEncFormatIntfc)->GetVADMode(vad);
        qDebug("get iLBC VAD: %d", vad);
        break;
    default:
        return;
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
    // QVERIFY(vad == m_Vad);


}

void QTmsVoipTest::ToggleCng()
{
    //    m_Cng = (m_Cng) ? EFalse : ETrue;

    m_Cng = ETrue;
    switch (m_DnLinkCodec) {
    case QTMS_FORMAT_G711:
        //case EG711_10MS:
        static_cast<QTMSG711Format*> (m_G711DecFormatIntfc)->SetCNG(m_Cng);
        qDebug("G711 CNG set %d", m_Cng);
        break;
    case QTMS_FORMAT_ILBC:
        static_cast<QTMSILBCFormat*> (m_ILBCDecFormatIntfc)->SetCNG(m_Cng);
        qDebug("iLBC CNG set %d", m_Cng);
        break;
    default:
        break;
    }
}

void QTmsVoipTest::GetCng()
{
    gint status;
    gboolean cng = EFalse;
    switch (m_DnLinkCodec) {
    case QTMS_FORMAT_G711:
        status = static_cast<QTMSG711Format*> (m_G711DecFormatIntfc)->GetCNG(cng);
        qDebug("G711 CNG: %d", cng);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        break;
    case QTMS_FORMAT_ILBC:
        status = static_cast<QTMSILBCFormat*> (m_ILBCDecFormatIntfc)->GetCNG(cng);
        qDebug("iLBC CNG: %d", cng);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        break;
    default:
        break;
    }

    // QVERIFY(cng == m_Cng);

}

void QTmsVoipTest::TogglePlc()
{
    //    m_Plc = (m_Plc) ? EFalse : ETrue;
    m_Plc = ETrue;
    if (m_DnLinkCodec == QTMS_FORMAT_G711) {
        static_cast<QTMSG711Format*> (m_G711DecFormatIntfc)->SetPlc(m_Plc);
        qDebug("G711 PLC set %d", m_Plc);
    }
}

void QTmsVoipTest::GetPlc()
{
    gboolean plc = EFalse;
    if (m_G711DecFormatIntfc && m_DnLinkCodec == QTMS_FORMAT_G711) {
        gint status = static_cast<QTMSG711Format*> (m_G711DecFormatIntfc)->GetPlc(plc);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        qDebug("G711 PLC: %d", plc);
        // QVERIFY(plc == m_Plc);
    }
}

void QTmsVoipTest::SelectMinBitrate()
{
    if (m_BitratesVector.size() > 0) {
        std::vector<guint>::iterator bitrate = m_BitratesVector.begin();
        iBitrate = *bitrate;
        SetBitrate(iBitrate);
        qDebug("BR set %d", iBitrate);
    }
}

void QTmsVoipTest::SelectMaxBitrate()
{
    if (m_BitratesVector.size() > 0) {
        iBitrate = m_BitratesVector.back();
        SetBitrate(iBitrate);
        qDebug("BR set %d", iBitrate);
    }
}

void QTmsVoipTest::SetBitrate(guint bitrate)
{
    switch (m_UpLinkCodec) {
    case QTMS_FORMAT_G711:
        static_cast<QTMSG711Format*> (m_G711EncFormatIntfc)->SetBitRate(bitrate);
        qDebug("Set BR %d", bitrate);
        break;
    case QTMS_FORMAT_G729:
        static_cast<QTMSG729Format*> (m_G729EncFormatIntfc)->SetBitRate(bitrate);
        qDebug("Set BR %d", bitrate);
        break;
    case QTMS_FORMAT_ILBC:
        static_cast<QTMSILBCFormat*> (m_ILBCEncFormatIntfc)->SetBitRate(bitrate);
        qDebug("Set BR %d", bitrate);
        break;
    case QTMS_FORMAT_AMR:
        static_cast<QTMSAMRFormat*> (m_AMRNBEncFormatIntfc)->SetBitRate(bitrate);
        qDebug("Set BR %d", bitrate);
        break;
    case QTMS_FORMAT_PCM:
    default:
        break;
    }
}

void QTmsVoipTest::GetSupportedBitrates()
{
    gint numOfItems(0);
    m_BitratesVector.clear();
    gint status = GetSupportedBitrates(m_BitratesVector);

    numOfItems = m_BitratesVector.size();
    qDebug("BR Count %d", numOfItems);
    std::vector<guint>::iterator itBitrates = m_BitratesVector.begin();
    for (; itBitrates < m_BitratesVector.end(); itBitrates++) {
        qDebug("BR %d", *itBitrates);
    }
}

gint QTmsVoipTest::GetSupportedBitrates(BitRateVector& aBrArr)
{
    gint status(QTMS_RESULT_SUCCESS);

    switch (m_UpLinkCodec) {
    case QTMS_FORMAT_G711:
        status = m_G711EncFormatIntfc->GetSupportedBitRates(aBrArr);
        break;
    case QTMS_FORMAT_G729:
        status = m_G729EncFormatIntfc->GetSupportedBitRates(aBrArr);
        break;
    case QTMS_FORMAT_ILBC:
        status = m_ILBCEncFormatIntfc->GetSupportedBitRates(aBrArr);
        break;
    case QTMS_FORMAT_AMR:
        status = m_AMRNBEncFormatIntfc->GetSupportedBitRates(aBrArr);
        break;
    case QTMS_FORMAT_PCM:
        // not supported, but test for correct error handling
        //status = m_PCM16EncFormatIntfc->GetSupportedBitRates(aBrArr);
        break;
    default:
        status = KErrNotSupported;
        break;
    }

    return status;
}

void QTmsVoipTest::GetBitrate()
{
    guint bitrate;
    switch (m_UpLinkCodec) {
    case QTMS_FORMAT_G711:
        static_cast<QTMSG711Format*> (m_G711EncFormatIntfc)->GetBitRate(bitrate);
        qDebug("G711 bitrate: %d", bitrate);
        break;
    case QTMS_FORMAT_G729:
        static_cast<QTMSG729Format*> (m_G729EncFormatIntfc)->GetBitRate(bitrate);
        qDebug("G729 bitrate: %d", bitrate);
        break;
    case QTMS_FORMAT_ILBC:
        static_cast<QTMSILBCFormat*> (m_ILBCEncFormatIntfc)->GetBitRate(bitrate);
        qDebug("iLBC bitrate: %d", bitrate);
        break;
    case QTMS_FORMAT_AMR:
        static_cast<QTMSAMRFormat*> (m_AMRNBEncFormatIntfc)->GetBitRate(bitrate);
        qDebug("AMR bitrate: %d", bitrate);
        break;
    case TMS_FORMAT_PCM:
    default:
        break;
    }
}

void QTmsVoipTest::SetDnLinkG711ALAW()
{
    if (m_G711DecFormatIntfc && m_DnLinkCodec == QTMS_FORMAT_G711) {
        gint status = static_cast<QTMSG711Format*> (m_G711DecFormatIntfc)->SetMode(
            QTMS_G711_CODEC_MODE_ALAW);
        qDebug("SetDnLinkG711ALAW status %d", status);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        qDebug("DNL G.711 Mode Set: [aLaw]");
    }
}

void QTmsVoipTest::SetDnLinkG711uLAW()
{
    if (m_G711DecFormatIntfc && m_DnLinkCodec == QTMS_FORMAT_G711) {
        gint status = static_cast<QTMSG711Format*> (m_G711DecFormatIntfc)->SetMode(
            QTMS_G711_CODEC_MODE_MULAW);
        qDebug("SetDnLinkG711uLAW status %d", status);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        qDebug("DNL G.711 Mode Set: [uLaw]");
    }
}

void QTmsVoipTest::GetDnLinkG711Mode()
{
    QTMSG711CodecMode mode;
    if (m_G711DecFormatIntfc && m_DnLinkCodec == QTMS_FORMAT_G711) {
        gint status = static_cast<QTMSG711Format*> (m_G711DecFormatIntfc)->GetMode(mode);
        qDebug("GetDnLinkG711Mode status %d", status);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        if (mode == TMS_G711_CODEC_MODE_MULAW) {
            qDebug("DNL G711 Mode: [uLaw]");
        }
        else {
            qDebug("DNL G711 Mode: [aLaw]");
        }
    }
}

void QTmsVoipTest::SetDnLinkILBC20MS()
{
    if (m_ILBCDecFormatIntfc && m_DnLinkCodec == QTMS_FORMAT_ILBC) {
        gint status = static_cast<QTMSILBCFormat*> (m_ILBCDecFormatIntfc)->SetMode(
            TMS_ILBC_CODEC_MODE_20MS_FRAME);
        qDebug("SetDnLinkILBC20MS status %d", status);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        qDebug("DNL iLBC Mode Set: [20ms]");
    }
}

void QTmsVoipTest::SetDnLinkILBC30MS()
{
    if (m_ILBCDecFormatIntfc && m_DnLinkCodec == QTMS_FORMAT_ILBC) {
        gint status = static_cast<QTMSILBCFormat*> (m_ILBCDecFormatIntfc)->SetMode(
            TMS_ILBC_CODEC_MODE_30MS_FRAME);
        qDebug("SetDnLinkILBC30MS status %d", status);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        qDebug("DNL iLBC Mode Set: [30ms]");
    }
}

void QTmsVoipTest::GetDnLinkILBCMode()
{
    QTMSILBCCodecMode mode;
    if (m_ILBCDecFormatIntfc && m_DnLinkCodec == QTMS_FORMAT_ILBC) {
        gint status = static_cast<QTMSILBCFormat*> (m_ILBCDecFormatIntfc)->GetMode(mode);
        qDebug("GetDnLinkILBCMode status %d", status);

        QVERIFY(status == QTMS_RESULT_SUCCESS);
        if (mode == QTMS_ILBC_CODEC_MODE_30MS_FRAME) {
            qDebug("DNL iLBC Mode: [30ms]");
        }
        else if (mode == QTMS_ILBC_CODEC_MODE_20MS_FRAME) {
            qDebug("DNL iLBC Mode: [20ms]");
        }
    }
}

void QTmsVoipTest::SetUpLinkG711ALAW()
{
    if (m_G711EncFormatIntfc && m_UpLinkCodec == QTMS_FORMAT_G711) {
        gint status = static_cast<QTMSG711Format*> (m_G711EncFormatIntfc)->SetMode(
            QTMS_G711_CODEC_MODE_ALAW);

        qDebug("SetUpLinkG711ALAW status %d", status);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        qDebug("UPL G.711 Mode Set: [aLaw]");
    }
}

void QTmsVoipTest::SetUpLinkG711uLAW()
{
    if (m_G711EncFormatIntfc && m_UpLinkCodec == QTMS_FORMAT_G711) {
        gint status = static_cast<QTMSG711Format*> (m_G711EncFormatIntfc)->SetMode(
            QTMS_G711_CODEC_MODE_MULAW);
        qDebug("SetUpLinkG711uLAW status %d", status);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        qDebug("UPL G.711 Mode Set: [uLaw]");
    }
}

void QTmsVoipTest::GetUpLinkG711Mode()
{
    QTMSG711CodecMode mode;
    if (m_G711EncFormatIntfc && m_UpLinkCodec == QTMS_FORMAT_G711) {
        gint status = static_cast<QTMSG711Format*> (m_G711EncFormatIntfc)->GetMode(mode);
        qDebug("GetUpLinkG711Mode status %d", status);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        if (mode == QTMS_G711_CODEC_MODE_MULAW) {
            qDebug("UPL G.711 Mode: [uLaw]");
        }
        else if (mode == QTMS_G711_CODEC_MODE_ALAW) {
            qDebug("UPL G.711 Mode: [aLaw]");
        }
    }

}

void QTmsVoipTest::SetUpLinkILBC20MS()
{
    if (m_ILBCEncFormatIntfc && m_UpLinkCodec == QTMS_FORMAT_ILBC) {
        gint status = static_cast<QTMSILBCFormat*> (m_ILBCEncFormatIntfc)->SetMode(
            QTMS_ILBC_CODEC_MODE_20MS_FRAME);
        qDebug("SetUpLinkILBC20MS status %d", status);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        qDebug("UPL iLBC Mode Set: [20ms]");
    }
}

void QTmsVoipTest::SetUpLinkILBC30MS()
{
    if (m_ILBCEncFormatIntfc && m_UpLinkCodec == QTMS_FORMAT_ILBC) {
        gint status = static_cast<QTMSILBCFormat*> (m_ILBCEncFormatIntfc)->SetMode(
            QTMS_ILBC_CODEC_MODE_30MS_FRAME);
        qDebug("SetUpLinkILBC30MS status %d", status);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        qDebug("UPL iLBC Mode Set: [30ms]");
    }
}

void QTmsVoipTest::GetUpLinkILBCMode()
{
    QTMSILBCCodecMode mode;
    if (m_ILBCEncFormatIntfc && m_UpLinkCodec == QTMS_FORMAT_ILBC) {
        gint status = static_cast<QTMSILBCFormat*> (m_ILBCEncFormatIntfc)->GetMode(mode);
        qDebug("GetUpLinkILBCMode status %d", status);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        if (mode == QTMS_ILBC_CODEC_MODE_30MS_FRAME) {
            qDebug("UPL iLBC Mode: [30ms]");
        }
        else if (mode == QTMS_ILBC_CODEC_MODE_20MS_FRAME) {
            qDebug("UPL iLBC Mode: [20ms]");
        }
    }

}

void QTmsVoipTest::CreateStreamVol()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Factory && m_Dnlink && !m_Vol) {
        m_Factory->CreateFormat(QTMS_FORMAT_PCM, m_PCM16DecFormatIntfc);
        m_Dnlink->SetFormat(m_PCM16DecFormatIntfc);

        status = m_Factory->CreateEffect(QTMS_EFFECT_VOLUME, m_Vol);

        QVERIFY(status == QTMS_RESULT_SUCCESS);

        connect(static_cast<QTMSVolumeEffect*> (m_Vol),
            SIGNAL(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)), this,
            SLOT(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)));

        if (m_Dnlink) {
            status = m_Dnlink->AddEffect(m_Vol);
        }
        QVERIFY(status == QTMS_RESULT_SUCCESS);

        InitDnlink();

        StreamVol();

    }
}

void QTmsVoipTest::StreamVol()
{
    gint status(QTMS_RESULT_SUCCESS);
    guint maxvol(0);
    if (m_Vol) {
        status = static_cast<QTMSVolumeEffect*> (m_Vol)->GetMaxLevel(maxvol);
        QVERIFY(status == QTMS_RESULT_SUCCESS);

        status = static_cast<QTMSVolumeEffect*> (m_Vol)->SetLevel(maxvol);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);

        guint vol(0);
        status = static_cast<QTMSVolumeEffect*> (m_Vol)->GetLevel(vol);
        QVERIFY(status == QTMS_RESULT_SUCCESS);

        QCOMPARE(vol,maxvol);
    }
}

void QTmsVoipTest::CreateStreamGain()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Factory && m_Uplink && !m_Gain) {

        m_Factory->CreateFormat(QTMS_FORMAT_PCM, m_PCM16EncFormatIntfc);
        m_Uplink->SetFormat(m_PCM16EncFormatIntfc);
        status = m_Factory->CreateEffect(QTMS_EFFECT_GAIN, m_Gain);

        QVERIFY(status == QTMS_RESULT_SUCCESS);

        connect(static_cast<QTMSGainEffect*> (m_Gain),
            SIGNAL(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)), this,
            SLOT(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)));

        status = m_Uplink->AddEffect(m_Gain);
        QVERIFY(status == QTMS_RESULT_SUCCESS);

        InitUplink();

        StreamGain();
    }
}

void QTmsVoipTest::StreamGain()
{
    gint status(QTMS_RESULT_SUCCESS);
    if (m_Gain) {
        guint maxgain(0);
        static_cast<QTMSGainEffect*> (m_Gain)->GetMaxLevel(maxgain);
        QVERIFY(status == QTMS_RESULT_SUCCESS);

        static_cast<QTMSGainEffect*> (m_Gain)->SetLevel(maxgain);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);

        guint gain(0);
        static_cast<QTMSGainEffect*> (m_Gain)->GetLevel(gain);
        QVERIFY(status == QTMS_RESULT_SUCCESS);

        QCOMPARE(gain,maxgain);
    }

}

void QTmsVoipTest::CreateGlobalVol()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Factory && !m_GlobalVol) {
        status = m_Factory->CreateEffect(QTMS_EFFECT_GLOBAL_VOL, m_GlobalVol);

        QVERIFY(status == QTMS_RESULT_SUCCESS);

        connect(static_cast<QTMSGlobalVolEffect*> (m_GlobalVol),
            SIGNAL(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)), this,
            SLOT(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)));

        guint maxvol(0);
        status = static_cast<QTMSGlobalVolEffect*> (m_GlobalVol)->GetMaxLevel(maxvol);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        qDebug("global max vol %d", maxvol);

        status = static_cast<QTMSGlobalVolEffect*> (m_GlobalVol)->SetLevel(maxvol);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);

        guint vol(0);
        status = static_cast<QTMSGlobalVolEffect*> (m_GlobalVol)->GetLevel(vol);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        qDebug("global vol %d", vol);

        QCOMPARE(vol,maxvol);

    }
}

void QTmsVoipTest::CreateGlobalGain()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Factory && !m_GlobalGain) {
        status = m_Factory->CreateEffect(QTMS_EFFECT_GLOBAL_GAIN, m_GlobalGain);

        QVERIFY(status == QTMS_RESULT_SUCCESS);

        connect(static_cast<QTMSGlobalGainEffect*> (m_GlobalGain),
            SIGNAL(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)), this,
            SLOT(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)));

        guint maxgain(0);
        static_cast<QTMSGlobalGainEffect*> (m_GlobalGain)->GetMaxLevel(maxgain);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        qDebug("global max gain %d", maxgain);

        static_cast<QTMSGlobalGainEffect*> (m_GlobalGain)->SetLevel(maxgain);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);

        guint gain(0);
        static_cast<QTMSGlobalGainEffect*> (m_GlobalGain)->GetLevel(gain);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        qDebug("global gain %d", gain);

        QCOMPARE(gain,maxgain);

    }
}

void QTmsVoipTest::CreateRouting_data()
{

    QTest::addColumn<QTMSAudioOutput>("routing");

    QTest::newRow("Public") << QTMS_AUDIO_OUTPUT_PUBLIC;
    QTest::newRow("Private") << QTMS_AUDIO_OUTPUT_PRIVATE;

}

void QTmsVoipTest::CreateRouting()
{
    gint status(QTMS_RESULT_SUCCESS);
    QFETCH(QTMSAudioOutput, routing);

    if (m_Factory && !m_Routing) {
        m_UpLinkCodec = QTMS_FORMAT_PCM;
        m_Factory->CreateFormat(QTMS_FORMAT_PCM, m_PCM16DecFormatIntfc);
        m_Dnlink->SetFormat(m_PCM16DecFormatIntfc);

        InitDnlink();
        StartDnlink();

        status = m_Factory->CreateGlobalRouting(m_Routing);
        QVERIFY(status == QTMS_RESULT_SUCCESS);

        m_RoutingSpy = new QSignalSpy(m_Routing,
            SIGNAL(GlobalRoutingEvent(const QTMSGlobalRouting&,
                    QTMSSignalEvent,
                    QTMSAudioOutput)));

        QVERIFY( m_RoutingSpy->isValid() );

        //QCOMPARE(m_RoutingSpy->count(), 0 );
        connect(m_Routing,
            SIGNAL(GlobalRoutingEvent(const QTMSGlobalRouting&,QTMSSignalEvent,QTMSAudioOutput)),
            this,
            SLOT(GlobalRoutingEvent(const QTMSGlobalRouting&,QTMSSignalEvent,QTMSAudioOutput)));
    }

    if (m_Routing) {
        status = m_Routing->SetOutput(routing);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
#ifndef __WINSCW__
        QTest::qWait(1000);
        //QCOMPARE(m_RoutingSpy->count(), 1 );
#endif
        QTMSAudioOutput current;
        status = m_Routing->GetOutput(current);
        DisplayDevice(current);

        QVERIFY(status == QTMS_RESULT_SUCCESS);
#ifndef __WINSCW__
        QCOMPARE(routing,current);
#endif
        StopDnlink();
    }
}

void QTmsVoipTest::DisplayDevice(QTMSAudioOutput device)
{
    qDebug("Current Output:");

    switch (device) {
    case QTMS_AUDIO_OUTPUT_NONE:
    {
        qDebug("none");
    }
        break;
    case QTMS_AUDIO_OUTPUT_PUBLIC:
    {
        qDebug("Public");
    }
        break;
    case QTMS_AUDIO_OUTPUT_PRIVATE:
    {
        qDebug("Private");
    }
        break;
    default:
        break;
    }
}

void QTmsVoipTest::CreateDnlinkDtmf()
{
    gint status(QTMS_RESULT_SUCCESS);
    GString* dtmfstring(NULL);

    if (m_Factory && m_Dnlink && !m_Dnlnkdtmf) {
        m_UpLinkCodec = QTMS_FORMAT_PCM;
        m_Factory->CreateFormat(QTMS_FORMAT_PCM, m_PCM16DecFormatIntfc);
        m_Dnlink->SetFormat(m_PCM16DecFormatIntfc);

        InitDnlink();

        status = m_Factory->CreateDTMF(QTMS_STREAM_DOWNLINK, m_Dnlnkdtmf);

        QVERIFY(status == QTMS_RESULT_SUCCESS);

        QSignalSpy dtmfspy(m_Dnlnkdtmf, SIGNAL(DTMFEvent(const QTMSDTMF&,QTMSSignalEvent)));

        QVERIFY( dtmfspy.isValid() );

        //QCOMPARE(dtmfspy.count(), 0 );

        connect(m_Dnlnkdtmf, SIGNAL(DTMFEvent(const QTMSDTMF&,QTMSSignalEvent)), this,
            SLOT(DTMFEvent(const QTMSDTMF&,QTMSSignalEvent)));

        dtmfstring = g_string_new("4723");
        status = m_Dnlnkdtmf->SetTone(dtmfstring);

        m_Dnlnkdtmf->Start();
        QTest::qWait(1000);
        g_string_free(dtmfstring, TRUE);
        //QCOMPARE(dtmfspy.count(), 1 );

        status = m_Dnlnkdtmf->Stop();
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);
        //QCOMPARE(dtmfspy.count(), 2 );
    }
}

void QTmsVoipTest::CreateRingTone()
{

}

void QTmsVoipTest::CreateBuffer()
{
    gint status(QTMS_RESULT_SUCCESS);
    if (m_Factory) {
        QTMSBuffer* qtmsbuffer(NULL);
        QTMSBufferType buftype(QTMS_BUFFER_MEMORY);
        guint size(100);
        status = m_Factory->CreateBuffer(QTMS_BUFFER_MEMORY, size, qtmsbuffer);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        if (qtmsbuffer) {
            qtmsbuffer->GetType(buftype);
            qtmsbuffer->GetDataSize(size);
            qtmsbuffer->SetDataSize(size);
            guint8* pbuf(NULL);
            qtmsbuffer->GetDataPtr(pbuf);
            guint64 ts(0);
            qtmsbuffer->GetTimeStamp(ts);
            qtmsbuffer->SetTimeStamp(ts);
            m_Factory->DeleteBuffer(qtmsbuffer);
        }
    }
}

void QTmsVoipTest::FillBuffer(QTMSBuffer& buffer)
{
    //TODO: add loopback test and file playback
#ifdef  __TEST_CODE_COVERAGE__
    if (m_ClientSource) {
        static_cast<QTMSClientSource*> (m_ClientSource)->BufferFilled(buffer);
    }
#endif

}

void QTmsVoipTest::BufferProcessed(QTMSBuffer* /*buffer*/, gint /*reason*/)
{
}

void QTmsVoipTest::ProcessBuffer(const QTMSBuffer* buffer)
{
    //TODO: add loopback test
#ifdef  __TEST_CODE_COVERAGE__
    QTMSBuffer* recBuf(NULL);
    recBuf = const_cast<QTMS::QTMSBuffer*> (buffer);
    if (m_ClientSink) {
        static_cast<QTMSClientSink*> (m_ClientSink)->BufferProcessed(recBuf);
    }
#endif
}

void QTmsVoipTest::EffectsEvent(const QTMSEffect& tmseffect, QTMSSignalEvent event)
{
    QTMSEffectType effecttype;
    const_cast<QTMSEffect&> (tmseffect).GetType(effecttype);

    QVERIFY(event.reason == QTMS_RESULT_SUCCESS);

    switch (effecttype) {
    case QTMS_EFFECT_GLOBAL_VOL:
    {
        switch (event.type) {
        case QTMS_EVENT_EFFECT_VOL_CHANGED:
            qDebug("Global vol effect change");
            break;
        default:
            break;
        }
    }
        break;
    case QTMS_EFFECT_GLOBAL_GAIN:
    {
        switch (event.type) {
        case QTMS_EVENT_EFFECT_GAIN_CHANGED:
            qDebug("Global gain effect change");
            break;
        default:
            break;
        }
    }
        break;
    case QTMS_EFFECT_VOLUME:
    {
        switch (event.type) {
        case QTMS_EVENT_EFFECT_GAIN_CHANGED:
            qDebug("Stream vol effect change");
            break;
        default:
            break;
        }
    }
        break;
    case QTMS_EFFECT_GAIN:
    {
        switch (event.type) {
        case QTMS_EVENT_EFFECT_GAIN_CHANGED:
            qDebug("Stream gain effect change");
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

void QTmsVoipTest::GlobalRoutingEvent(const QTMSGlobalRouting& /*routing*/, QTMSSignalEvent event,
    QTMSAudioOutput /*output*/)
{
    QVERIFY(event.reason == QTMS_RESULT_SUCCESS);
    switch (event.type) {
    case QTMS_EVENT_ROUTING_AVAIL_OUTPUTS_CHANGED:
        qDebug("Available outputs changed");
        break;
    case QTMS_EVENT_ROUTING_OUTPUT_CHANGED:
        qDebug("output changed");
        break;
    case QTMS_EVENT_ROUTING_SET_OUTPUT_COMPLETE:
        qDebug("set output complete");
        break;
    default:
        break;
    }

}

void QTmsVoipTest::TMSStreamEvent(const QTMSStream& stream, QTMSSignalEvent event)
{

    QVERIFY(event.reason == QTMS_RESULT_SUCCESS);

    gint streamId;
    streamId = const_cast<QTMSStream&> (stream).GetStreamId();
    qDebug("StreamID %d", streamId);
    gint state;
    state = const_cast<QTMSStream&> (stream).GetState();
    QVERIFY(state == event.curr_state);

    switch (const_cast<QTMSStream&> (stream).GetStreamType()) {
    case QTMS_STREAM_UPLINK:
    {
        switch (event.type) {
        case QTMS_EVENT_STREAM_STATE_CHANGED:
        {
            switch (event.curr_state) {
            case QTMS_STREAM_INITIALIZED:
            {
                qDebug("Uplink initialized ");
                break;
            }
            case QTMS_STREAM_UNINITIALIZED:
                qDebug("Uplink uninitialized ");
                break;
            case QTMS_STREAM_PAUSED:
                qDebug("Uplink paused ");
                break;
            case QTMS_STREAM_STARTED:
                qDebug("Uplink started ");
                break;
            default:
                break;
            }
            break;
        }
        default:
            break;
        }
        break;
    }
    case QTMS_STREAM_DOWNLINK:
    {
        switch (event.type) {
        case QTMS_EVENT_STREAM_STATE_CHANGED:
        {
            switch (event.curr_state) {
            case QTMS_STREAM_INITIALIZED:
                qDebug("Downlink initialized ");
                break;
            case QTMS_STREAM_UNINITIALIZED:
                qDebug("Downlink uninitialized ");
                break;
            case QTMS_STREAM_PAUSED:
                qDebug("Downlink paused ");
                break;
            case QTMS_STREAM_STARTED:
                qDebug("Downlink started ");
                break;
            default:
                break;
            }
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void QTmsVoipTest::RingtoneEvent(const QTMSRingTone& /*rt*/, QTMSSignalEvent event)
{
    QVERIFY(event.reason == QTMS_RESULT_SUCCESS);
    switch (event.type) {
    case QTMS_EVENT_RINGTONE_OPEN_COMPLETE:
        qDebug("RT Open Complete");
        break;
    case QTMS_EVENT_RINGTONE_PLAY_COMPLETE:
        qDebug("RT Play Complete");
        break;
    default:
        break;
    }
}

void QTmsVoipTest::DTMFEvent(const QTMSDTMF& /*dtmf*/, QTMSSignalEvent event)
{

    QVERIFY(event.reason == QTMS_RESULT_SUCCESS);
    switch (event.type) {
    case QTMS_EVENT_DTMF_TONE_STARTED:
        qDebug("DTMF Started");
        break;
    case QTMS_EVENT_DTMF_TONE_STOPPED:
        qDebug("DTMF Stopped");
        break;
    default:
        break;
    }
}

QTEST_MAIN_S60(QTmsVoipTest)

#include "qtmsvoiptest.moc"

