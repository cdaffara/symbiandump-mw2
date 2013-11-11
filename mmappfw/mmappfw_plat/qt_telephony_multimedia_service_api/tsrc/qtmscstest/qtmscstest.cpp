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
 * Description: QTMS CS test
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

using namespace QTMS;
_LIT8(KRTBeepSequence, "\x00\x11\x0A\x0A\x08\x73\x0A\x40\x28\x0A\xF7\
\x05\xFC\x40\x64\x0A\x08\x40\x32\x0A\xF7\x06\x0B");
_LIT16(KTestFile1, "c:\\data\\sounds\\digital\\test_8khz.wav");

class QTmsCsTest: public QObject
{
Q_OBJECT
private slots:
    void initTestCase();
    void CSCallSetup();

    void CreateUplink();
    void CreateDnlink();
    void CreateMicSource();
    void AddMicSourceToStream();
    void CreateModemSink();
    void AddModemSinkToStream();
    void CreateModemSource();
    void AddModemSourceToStream();
    void CreateSpeakerSink();
    void AddSpeakerSinkToStream();

    void CreateStreamVol();
    void CreateStreamGain();

    void InitUplink();
    void InitDnlink();

    void StartUplink();
    void StopUplink();
    void StartDnlink();

    void StreamVol();
    void StreamGain();
    // void StopDnlink();

    void CreateGlobalVol();
    void CreateGlobalGain();
    void CreateRouting_data();
    void CreateRouting();

    void CreateInband_data();
    void CreateInband();
    void CreateDnlinkDtmf();
    void CreateUplinkDtmf();

    void CreateRingTone();

    void InitRingTonePlayerFromProfiles();
    void PlayRingTone();
    void PauseRingTone();
    void MuteRingTone();
    void StopRingTone();
    void CloseRingTonePlayer();

    void CloseUplink();
    void CloseDownlink();
    void cleanupTestCase();
public slots:
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
    //From TMSInbandToneObserver
    void InbandToneEvent(const QTMSInbandTone& inbandtone, QTMSSignalEvent event);

private:
    void InitRingToneSequencePlayer();
    void InitRingTonePlayerFromFile();
    void StopDnlink();
    void GetAvailableOutput();
    void DisplayDevice(QTMSAudioOutput device);

private:
    QTMSFactory *m_Factory;
    QTMSCall *m_Call;
    QTMSDTMF *m_Dnlnkdtmf;
    QTMSDTMF *m_Uplnkdtmf;
    QTMSStream *m_Dnlink;
    QTMSStream *m_Uplink;
    QTMSSource *m_MicSource;
    QTMSSource *m_ModemSource;
    QTMSSink *m_SpeakerSink;
    QTMSSink *m_ModemSink;
    QTMSEffect *m_Vol;
    QTMSEffect *m_Gain;
    QTMSEffect *m_GlobalVol;
    QTMSEffect *m_GlobalGain;
    QTMSGlobalRouting *m_Routing;
    QTMSInbandTone *m_InbandTonePlayer;
    QTMSRingTone *m_RingTonePlayer;
    GString* m_RTStr;
    GString* m_TTSStr;
    QSignalSpy *m_UplinkSpy;
    QSignalSpy *m_DnlinkSpy;
    QSignalSpy *m_InbandSpy;
    QSignalSpy *m_RoutingSpy;
    OutputVector m_Availableoutputs;

};

void QTmsCsTest::initTestCase()
{
    qDebug("Start QTms CS tests");

    g_setenv("G_SLICE", "always-malloc", 1);
    m_Factory = NULL;
    m_Call = NULL;
    m_Dnlink = NULL;
    m_Uplink = NULL;
    m_MicSource = NULL;
    m_ModemSource = NULL;
    m_SpeakerSink = NULL;
    m_ModemSink = NULL;
    m_Vol = NULL;
    m_Gain = NULL;
    m_GlobalVol = NULL;
    m_GlobalGain = NULL;
    m_Routing = NULL;
    m_Dnlnkdtmf = NULL;
    m_Uplnkdtmf = NULL;
    m_InbandTonePlayer = NULL;
    m_RingTonePlayer = NULL;
    m_UplinkSpy = NULL;
    m_DnlinkSpy = NULL;

    QTMSVer *ver(NULL);
    gint status;
    status = QTMSFactory::CreateFactory(m_Factory, *ver);

    QVERIFY(status == QTMS_RESULT_SUCCESS);

    qRegisterMetaType<QTMSSignalEvent> ("QTMSSignalEvent");
    qRegisterMetaType<QTMSAudioOutput> ("QTMSAudioOutput");

}

void QTmsCsTest::cleanupTestCase()
{
    qDebug("End QTms tests");

    StopDnlink();

    m_Factory->DeleteSource(m_MicSource);
    m_Factory->DeleteSource(m_ModemSource);
    m_Factory->DeleteSink(m_ModemSink);
    m_Factory->DeleteSink(m_SpeakerSink);

    m_Factory->DeleteDTMF(m_Dnlnkdtmf);
    m_Factory->DeleteInbandTonePlayer(m_InbandTonePlayer);
    m_Factory->DeleteRingTonePlayer(m_RingTonePlayer);

    m_Factory->DeleteEffect(m_Vol);
    m_Factory->DeleteEffect(m_Gain);
    m_Factory->DeleteEffect(m_GlobalVol);
    m_Factory->DeleteEffect(m_GlobalGain);

    m_Factory->DeleteGlobalRouting(m_Routing);

    m_Call->DeleteStream(m_Uplink);
    m_Call->DeleteStream(m_Dnlink);

    m_Factory->DeleteCall(m_Call);

    delete m_Factory;

}

void QTmsCsTest::CSCallSetup()
{
    gint status(QTMS_RESULT_SUCCESS);
    if (m_Factory && !m_Call) {
        gboolean issupported(FALSE);
        m_Factory->IsCallTypeSupported(QTMS_CALL_CS, issupported);
        QVERIFY(issupported == TRUE);
        status = m_Factory->CreateCall(QTMS_CALL_CS, m_Call, 0);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTMSCallType calltype = m_Call->GetCallType();
        QVERIFY(calltype == QTMS_CALL_CS);
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsCsTest::CreateUplink()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Call) {
        status = m_Call->CreateStream(QTMS_STREAM_UPLINK, m_Uplink);

        QVERIFY(status == QTMS_RESULT_SUCCESS);

        m_UplinkSpy = new QSignalSpy(m_Uplink,
            SIGNAL(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)));

        QVERIFY( m_UplinkSpy->isValid() );

        //QCOMPARE(m_UplinkSpy->count(), 0 );


        connect(m_Uplink, SIGNAL(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)), this,
            SLOT(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)));

    }
}

void QTmsCsTest::CreateDnlink()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Call) {
        status = m_Call->CreateStream(QTMS_STREAM_DOWNLINK, m_Dnlink);

        QVERIFY(status == QTMS_RESULT_SUCCESS);

        m_DnlinkSpy = new QSignalSpy(m_Dnlink,
            SIGNAL(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)));

        QVERIFY( m_DnlinkSpy->isValid() );

        //QCOMPARE(m_DnlinkSpy->count(), 0 );

        connect(m_Dnlink, SIGNAL(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)), this,
            SLOT(TMSStreamEvent(const QTMSStream&, QTMSSignalEvent)));
    }
}

void QTmsCsTest::CreateModemSink()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Factory && !m_ModemSink) {
        status = m_Factory->CreateSink(QTMS_SINK_MODEM, m_ModemSink);
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsCsTest::CreateModemSource()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Factory && !m_ModemSource) {
        status = m_Factory->CreateSource(QTMS_SOURCE_MODEM, m_ModemSource);
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsCsTest::AddModemSinkToStream()
{
    gint status(QTMS_RESULT_SUCCESS);
    if (m_Uplink && m_ModemSink) {
        status = m_Uplink->AddSink(m_ModemSink);
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsCsTest::AddModemSourceToStream()
{
    gint status(QTMS_RESULT_SUCCESS);
    if (m_Dnlink && m_ModemSource) {
        status = m_Dnlink->AddSource(m_ModemSource);
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsCsTest::CreateMicSource()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Factory && !m_MicSource) {
        status = m_Factory->CreateSource(TMS_SOURCE_MIC, m_MicSource);
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsCsTest::AddMicSourceToStream()
{
    gint status(QTMS_RESULT_SUCCESS);
    if (m_Uplink && m_MicSource) {
        status = m_Uplink->AddSource(m_MicSource);
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsCsTest::CreateSpeakerSink()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Factory && !m_SpeakerSink) {
        status = m_Factory->CreateSink(QTMS_SINK_SPEAKER, m_SpeakerSink);
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsCsTest::AddSpeakerSinkToStream()
{
    gint status(QTMS_RESULT_SUCCESS);
    if (m_Dnlink && m_SpeakerSink) {
        status = m_Dnlink->AddSink(m_SpeakerSink);
        qDebug("QTMS speaker sink added to stream");
    }
    QVERIFY(status == QTMS_RESULT_SUCCESS);
}

void QTmsCsTest::InitUplink()
{
    QVERIFY(m_Uplink->Init() == QTMS_RESULT_SUCCESS);
    QTest::qWait(1000);
    //QCOMPARE(m_UplinkSpy->count(), 1 );
}

void QTmsCsTest::InitDnlink()
{
    QVERIFY(m_Dnlink->Init() == QTMS_RESULT_SUCCESS);
    QTest::qWait(1000);
    //QCOMPARE(m_DnlinkSpy->count(), 1 );
}

void QTmsCsTest::StartUplink()
{
    QVERIFY(m_Uplink->Start() == QTMS_RESULT_SUCCESS);
    QTest::qWait(1000);
    //QCOMPARE(m_UplinkSpy->count(), 2 );
}

void QTmsCsTest::StartDnlink()
{
    QVERIFY(m_Dnlink->Start() == QTMS_RESULT_SUCCESS);
    QTest::qWait(1000);
    //QCOMPARE(m_DnlinkSpy->count(), 2 );
}
void QTmsCsTest::StopUplink()
{
    QVERIFY(m_Uplink->Stop() == QTMS_RESULT_SUCCESS);
    QTest::qWait(1000);
    //QCOMPARE(m_UplinkSpy->count(), 3 );
}

void QTmsCsTest::StopDnlink()
{
    QVERIFY(m_Dnlink->Stop() == QTMS_RESULT_SUCCESS);
    QTest::qWait(1000);
    //QCOMPARE(m_DnlinkSpy->count(), 3 );
}

void QTmsCsTest::CloseUplink()
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
        if (m_ModemSink) {
            QVERIFY(m_Uplink->RemoveSink(m_ModemSink)== QTMS_RESULT_SUCCESS);
        }
    }
}

void QTmsCsTest::CloseDownlink()
{
    m_Dnlink->Deinit();
    QTest::qWait(1000);
    if (m_Dnlink) {
        if (m_Vol) {
            QVERIFY(m_Dnlink->RemoveEffect(m_Vol)== QTMS_RESULT_SUCCESS);
        }
        if (m_ModemSource) {
            QVERIFY(m_Dnlink->RemoveSource(m_ModemSource)== QTMS_RESULT_SUCCESS);
        }
        if (m_SpeakerSink) {
            QVERIFY(m_Dnlink->RemoveSink(m_SpeakerSink) == QTMS_RESULT_SUCCESS);
        }
    }

}

void QTmsCsTest::CreateStreamVol()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Factory && !m_Vol) {
        status = m_Factory->CreateEffect(QTMS_EFFECT_VOLUME, m_Vol);

        QVERIFY(status == QTMS_RESULT_SUCCESS);

        connect(static_cast<QTMSVolumeEffect*> (m_Vol),
            SIGNAL(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)), this,
            SLOT(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)));

        if (m_Dnlink) {
            status = m_Dnlink->AddEffect(m_Vol);
        }
        QVERIFY(status == QTMS_RESULT_SUCCESS);

    }
}

void QTmsCsTest::StreamVol()
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

void QTmsCsTest::CreateStreamGain()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Factory && !m_Gain) {
        status = m_Factory->CreateEffect(QTMS_EFFECT_GAIN, m_Gain);

        QVERIFY(status == QTMS_RESULT_SUCCESS);

        connect(static_cast<QTMSGainEffect*> (m_Gain),
            SIGNAL(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)), this,
            SLOT(EffectsEvent(const QTMSEffect&,QTMSSignalEvent)));

        if (m_Uplink) {
            status = m_Uplink->AddEffect(m_Gain);
        }
        QVERIFY(status == QTMS_RESULT_SUCCESS);
    }
}

void QTmsCsTest::StreamGain()
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

void QTmsCsTest::CreateGlobalVol()
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

        status = static_cast<QTMSGlobalVolEffect*> (m_GlobalVol)->SetLevel(maxvol);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);

        guint vol(0);
        status = static_cast<QTMSGlobalVolEffect*> (m_GlobalVol)->GetLevel(vol);
        QVERIFY(status == QTMS_RESULT_SUCCESS);

        QCOMPARE(vol,maxvol);

    }
}

void QTmsCsTest::CreateGlobalGain()
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

        static_cast<QTMSGlobalGainEffect*> (m_GlobalGain)->SetLevel(maxgain);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);

        guint gain(0);
        static_cast<QTMSGlobalGainEffect*> (m_GlobalGain)->GetLevel(gain);
        QVERIFY(status == QTMS_RESULT_SUCCESS);

        QCOMPARE(gain,maxgain);

    }
}

void QTmsCsTest::CreateRouting_data()
{
    gint status;
    if (m_Factory && !m_Routing) {
        status = m_Factory->CreateGlobalRouting(m_Routing);
        QVERIFY(status == QTMS_RESULT_SUCCESS);

        m_RoutingSpy = new QSignalSpy(m_Routing,
            SIGNAL(GlobalRoutingEvent(const QTMSGlobalRouting&,
                    QTMSSignalEvent,
                    QTMSAudioOutput)));

        QVERIFY( m_RoutingSpy->isValid() );

        // QCOMPARE(m_RoutingSpy->count(), 0 );
        connect(m_Routing, SIGNAL(GlobalRoutingEvent(const QTMSGlobalRouting&,
                QTMSSignalEvent,QTMSAudioOutput)), this,
            SLOT(GlobalRoutingEvent(const QTMSGlobalRouting&,
                    QTMSSignalEvent,QTMSAudioOutput)));
    }

    //Populate data and test based on available output
    GetAvailableOutput();
}

void QTmsCsTest::CreateRouting()
{
    gint status(QTMS_RESULT_SUCCESS);
    QFETCH(QTMSAudioOutput, routing);

    if (m_Routing) {
        status = m_Routing->SetOutput(routing);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
#ifndef __WINSCW__
        QTest::qWait(1000);
        //  QCOMPARE(m_RoutingSpy->count(), 1 );
#endif
        QTMSAudioOutput current;
        status = m_Routing->GetOutput(current);

        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QCOMPARE(routing,current);

        QTMSAudioOutput previous;
        status = m_Routing->GetPreviousOutput(previous);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        qDebug("Previous routing:");
        DisplayDevice(previous);
    }
}

void QTmsCsTest::GetAvailableOutput()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Routing) {
        status = m_Routing->GetAvailableOutputs(m_Availableoutputs);

        QVERIFY(status == QTMS_RESULT_SUCCESS);

        qDebug("Available outputs: %d", m_Availableoutputs.size());
        std::vector<guint>::iterator iteroutputs = m_Availableoutputs.begin();
        for (; iteroutputs < m_Availableoutputs.end(); iteroutputs++) {
            DisplayDevice(*iteroutputs);
        }
    }
}

void QTmsCsTest::DisplayDevice(QTMSAudioOutput device)
{
    QTest::addColumn<QTMSAudioOutput>("routing");

    switch (device) {
    case QTMS_AUDIO_OUTPUT_NONE:
    {
        qDebug("none");
        QTest::newRow("None") << QTMS_AUDIO_OUTPUT_NONE;
    }
        break;
    case QTMS_AUDIO_OUTPUT_PUBLIC:
    {
        qDebug("Public");
        QTest::newRow("Public") << QTMS_AUDIO_OUTPUT_PUBLIC;
    }
        break;
    case QTMS_AUDIO_OUTPUT_PRIVATE:
    {
        qDebug("Private");
        QTest::newRow("Private") << QTMS_AUDIO_OUTPUT_PRIVATE;
    }
        break;
    case QTMS_AUDIO_OUTPUT_HANDSET:
    {
        qDebug("Handset");
        QTest::newRow("Handset") << QTMS_AUDIO_OUTPUT_HANDSET;
    }
        break;
    case QTMS_AUDIO_OUTPUT_LOUDSPEAKER:
    {
        qDebug("Loudspeaker");
        QTest::newRow("Loud speaker") << QTMS_AUDIO_OUTPUT_LOUDSPEAKER;
    }
        break;
    case QTMS_AUDIO_OUTPUT_WIRED_ACCESSORY:
    {
        qDebug("Wired accessory");
        QTest::newRow("Wired accessory") << QTMS_AUDIO_OUTPUT_WIRED_ACCESSORY;
    }
        break;
    case QTMS_AUDIO_OUTPUT_ACCESSORY:
    {
        qDebug("Accessory");
        QTest::newRow("Accessory") << QTMS_AUDIO_OUTPUT_ACCESSORY;
    }
        break;
    case QTMS_AUDIO_OUTPUT_ETTY:
    {
        qDebug("TTY");
        QTest::newRow("ETTY") << QTMS_AUDIO_OUTPUT_ETTY;
    }
        break;
    default:
        break;
    }
}

void QTmsCsTest::CreateDnlinkDtmf()
{
    gint status(QTMS_RESULT_SUCCESS);
    GString* dtmfstring(NULL);

    status = m_Factory->CreateDTMF(QTMS_STREAM_DOWNLINK, m_Dnlnkdtmf);

    QVERIFY(status == QTMS_RESULT_SUCCESS);
    if (m_Dnlnkdtmf) {
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

void QTmsCsTest::CreateUplinkDtmf()
{
    gint status(QTMS_RESULT_SUCCESS);
    GString* dtmfstring(NULL);

    status = m_Factory->CreateDTMF(QTMS_STREAM_UPLINK, m_Uplnkdtmf);

    QVERIFY(status == QTMS_RESULT_SUCCESS);
    if (m_Uplnkdtmf) {
        QSignalSpy uplinkdtmfspy(m_Uplnkdtmf, SIGNAL(DTMFEvent(const QTMSDTMF&,QTMSSignalEvent)));

        QVERIFY( uplinkdtmfspy.isValid() );

        //QCOMPARE(dtmfspy.count(), 0 );

        connect(m_Uplnkdtmf, SIGNAL(DTMFEvent(const QTMSDTMF&,QTMSSignalEvent)), this,
            SLOT(DTMFEvent(const QTMSDTMF&,QTMSSignalEvent)));

        dtmfstring = g_string_new("4723");
        status = m_Uplnkdtmf->SetTone(dtmfstring);

        m_Uplnkdtmf->Start();
        QTest::qWait(1000);
        g_string_free(dtmfstring, TRUE);
        //QCOMPARE(dtmfspy.count(), 1 );

        m_Uplnkdtmf->ContinueDTMFStringSending(FALSE);

        status = m_Uplnkdtmf->Stop();
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);
        //QCOMPARE(dtmfspy.count(), 2 );

    }
}

void QTmsCsTest::CreateInband_data()
{

    gint status(QTMS_RESULT_SUCCESS);
    if (m_Factory && !m_InbandTonePlayer) {
        status = m_Factory->CreateInbandTonePlayer(m_InbandTonePlayer);

        QVERIFY(status == QTMS_RESULT_SUCCESS);
        m_InbandSpy = new QSignalSpy(m_InbandTonePlayer,
            SIGNAL(InbandToneEvent(const QTMSInbandTone&,QTMSSignalEvent)));

        QVERIFY( m_InbandSpy->isValid() );

        //QCOMPARE(m_InbandSpy->count(), 0 );
        connect(m_InbandTonePlayer,
            SIGNAL(InbandToneEvent(const QTMSInbandTone&, QTMSSignalEvent)), this,
            SLOT(InbandToneEvent(const QTMSInbandTone&, QTMSSignalEvent)));
    }

    QTest::addColumn<QTMSInbandToneType>("inbandTone");

    QTest::newRow("Busy") << QTMS_INBAND_USER_BUSY;
    QTest::newRow("Path not avail") << QTMS_INBAND_RADIO_PATH_NOT_AVAIL;
    QTest::newRow("Congestion") << QTMS_INBAND_CONGESTION;
    QTest::newRow("Special info") << QTMS_INBAND_SPECIAL_INFO;
    QTest::newRow("Reorder") << QTMS_INBAND_REORDER;
    QTest::newRow("Remote alerting") << QTMS_INBAND_REMOTE_ALEARTING;
    QTest::newRow("call waiting") << QTMS_INBAND_CALL_WAITING;
    QTest::newRow("data call") << QTMS_INBAND_DATA_CALL;
    QTest::newRow("no sequence") << QTMS_INBAND_NO_SEQUENCE;
    QTest::newRow("beep sequence") << QTMS_INBAND_BEEP_SEQUENCE;

}

void QTmsCsTest::CreateInband()
{
    gint status(QTMS_RESULT_SUCCESS);
    QFETCH(QTMSInbandToneType, inbandTone);

    if (m_InbandTonePlayer) {
        status = m_InbandTonePlayer->Start(inbandTone);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);
        // QCOMPARE(m_InbandSpy->count(), 1 );

        status = m_InbandTonePlayer->Stop();
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);
        // QCOMPARE(m_InbandSpy->count(), 2 );
    }
}

void QTmsCsTest::CreateRingTone()
{
    gint status(QTMS_RESULT_SUCCESS);

    if (m_Factory) {
        status = m_Factory->CreateRingTonePlayer(m_RingTonePlayer);
        QVERIFY(status == QTMS_RESULT_SUCCESS);

        connect(m_RingTonePlayer, SIGNAL(RingtoneEvent(const QTMSRingTone&,QTMSSignalEvent)), this,
            SLOT(RingtoneEvent(const QTMSRingTone&,QTMSSignalEvent)));
    }

}

void QTmsCsTest::InitRingTonePlayerFromProfiles()
{
    if (m_RingTonePlayer) {
        gint status = m_RingTonePlayer->Init(QTMS_RINGTONE_DEFAULT);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);
    }

}

void QTmsCsTest::InitRingTonePlayerFromFile()
{
    if (m_RingTonePlayer) {
        TBuf<sizeof(KTestFile1)> buf(KTestFile1);
        m_RTStr = g_string_new_len((gchar*) buf.Ptr(), buf.Length() * 2);
        gint status = m_RingTonePlayer->Init(QTMS_RINGTONE_FILE, m_RTStr);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);
        g_string_free(m_RTStr, TRUE);
        m_RTStr = NULL;
    }

}

void QTmsCsTest::InitRingToneSequencePlayer()
{
    if (m_RingTonePlayer) {
        TBuf8<sizeof(KRTBeepSequence)> buf(KRTBeepSequence);
        m_RTStr = g_string_new_len((gchar*) buf.Ptr(), buf.Length());
        gint status = m_RingTonePlayer->Init(QTMS_RINGTONE_SEQUENCE, m_RTStr);
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);
        g_string_free(m_RTStr, TRUE);
    }
}

void QTmsCsTest::PlayRingTone()
{
    if (m_RingTonePlayer) {
        gint status = m_RingTonePlayer->Play();
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);
    }
}

void QTmsCsTest::PauseRingTone()
{
    if (m_RingTonePlayer) {
        gint status = m_RingTonePlayer->Pause(); //pause audio for video RT only
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);
    }
}

void QTmsCsTest::StopRingTone()
{
    if (m_RingTonePlayer) {
        gint status = m_RingTonePlayer->Stop();
        QVERIFY(status == QTMS_RESULT_SUCCESS);
        QTest::qWait(1000);
    }
}

void QTmsCsTest::MuteRingTone()
{
    if (m_RingTonePlayer) {
        m_RingTonePlayer->Mute();
    }
}

void QTmsCsTest::CloseRingTonePlayer()
{
    if (m_Factory && m_RingTonePlayer) {
        m_RingTonePlayer->Deinit();
        m_Factory->DeleteRingTonePlayer(m_RingTonePlayer);
    }
}

void QTmsCsTest::EffectsEvent(const QTMSEffect& tmseffect, QTMSSignalEvent event)
{
    QTMSEffectType effecttype;
    const_cast<QTMSEffect&> (tmseffect).GetType(effecttype);

    QVERIFY(event.reason == QTMS_RESULT_SUCCESS);

    switch (effecttype) {
    case TMS_EFFECT_GLOBAL_VOL:
    {
        switch (event.type) {
        case TMS_EVENT_EFFECT_VOL_CHANGED:
            qDebug("Global vol effect change");
            break;
        default:
            break;
        }
    }
        break;
    case TMS_EFFECT_GLOBAL_GAIN:
    {
        switch (event.type) {
        case TMS_EVENT_EFFECT_GAIN_CHANGED:
            qDebug("Global gain effect change");
            break;
        default:
            break;
        }
    }
        break;
    case TMS_EFFECT_VOLUME:
        qDebug("Stream vol effect change");
        break;
    case TMS_EFFECT_GAIN:
        qDebug("Stream gain effect change");
        break;
    default:
        break;
    }
}

void QTmsCsTest::GlobalRoutingEvent(const QTMSGlobalRouting& /*routing*/, QTMSSignalEvent event,
    QTMSAudioOutput /*output*/)
{
    QVERIFY(event.reason == QTMS_RESULT_SUCCESS);
    switch (event.type) {
    case TMS_EVENT_ROUTING_AVAIL_OUTPUTS_CHANGED:
        qDebug("Available outputs changed");
        break;
    case TMS_EVENT_ROUTING_OUTPUT_CHANGED:
        qDebug("output changed");
        break;
    case TMS_EVENT_ROUTING_SET_OUTPUT_COMPLETE:
        qDebug("set output complete");
        break;
    default:
        break;
    }

}

void QTmsCsTest::TMSStreamEvent(const QTMSStream& stream, QTMSSignalEvent event)
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
            {
                qDebug("Downlink initialized ");
                break;
            }
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

void QTmsCsTest::RingtoneEvent(const QTMSRingTone& /*rt*/, QTMSSignalEvent event)
{
    QVERIFY(event.reason == QTMS_RESULT_SUCCESS);
    switch (event.type) {
    case TMS_EVENT_RINGTONE_OPEN_COMPLETE:
        qDebug("RT Open Complete");
        break;
    case TMS_EVENT_RINGTONE_PLAY_COMPLETE:
        qDebug("RT Play Complete");
        break;
    default:
        break;
    }
}

void QTmsCsTest::DTMFEvent(const QTMSDTMF& /*dtmf*/, QTMSSignalEvent event)
{
    if (event.reason != QTMS_RESULT_SUCCESS) {
        QEXPECT_FAIL("","Need to be in call, expect uplink dtmf to fail", Continue);
    }
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

void QTmsCsTest::InbandToneEvent(const QTMSInbandTone& /*inbandtone*/, QTMSSignalEvent event)
{
    QVERIFY(event.reason == QTMS_RESULT_SUCCESS);
    switch (event.type) {
    case QTMS_EVENT_INBAND_TONE_STARTED:
        qDebug("Inband Tone Started");
        break;
    case QTMS_EVENT_INBAND_TONE_STOPPED:
        qDebug("Inband Tone Stopped");
        break;
    default:
        break;
    }
}

QTEST_MAIN_S60(QTmsCsTest)

#include "qtmscstest.moc"

