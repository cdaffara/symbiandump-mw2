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

#ifndef QTMSTESTENGINE_H
#define QTMSTESTENGINE_H

//#define __PLAY_WAV_FROM_FILE__

// INCLUDES
#include <QtCore>
#include <QTextEdit>
#include <QObject>
#include <qtmsfactory.h>
#include <qtmscall.h>
#include <qtmsstream.h>
#include <qtmsformat.h>
#include <qtmsbuffer.h>
#include <qtmsdtmf.h>
#include <qtmsringtone.h>
#include <qtmsinbandtone.h>
#include "mtmstestengine.h"

using namespace QTMS;

// CLASS DECLARATION
/**
 *  CTmsTestEngine application engine class.
 */
class QTmsTestEngine : public QObject,
                       public CBase,
                       public MTmsTestEngine
    {
    Q_OBJECT
public:
    enum TAppState
        {
        ENotReady = 0,
        EReady,
        EStreaming
        };

    /**
     * NewL()
     *
     * Create new CTmsTestEngine object
     * return a pointer to the created instance of CTmsTestEngine
     * @param aView a Pointer to CTmsTestView.
     */
    static QTmsTestEngine* NewL(QTextEdit* statusDisplay);

    /**
     * NewLC()
     *
     */
    static QTmsTestEngine* NewLC(QTextEdit* statusDisplay);

    /**
     * Destructor
     *
     */
    ~QTmsTestEngine();

    void GetSupportedDownlinkFormats(TBool aDisplayCodecs = EFalse);
    void GetSupportedUplinkFormats(TBool aDisplayCodecs = EFalse);
    void SetDownlinkFormat(QTMSFormatType aCodecFormat);
    void OpenDownlink();
    void StartDownlink();
    void StopDownlink();
    void CloseDownlink();
    void GetMaxVolume();
    void GetVolume();
    void SetVolume(guint aVolume);
    void MuteSpeaker();
    void SetMaxVolume();
    void VolumeUp();
    void VolumeDn();

    void SetUplinkFormat(QTMSFormatType aCodecFormat);
    void OpenUplink();
    void StartUplink();
    void StopUplink();
    void CloseUplink();
    void GetMaxGain();
    void GetGain();
    void SetMaxGain();
    void MuteMic();

    void GetSupportedBitrates();
    gint GetSupportedBitrates(BitRateVector& aBrArr);
    void SelectMinBitrate();
    void SelectMaxBitrate();
    void SetBitrate(TUint aBitrate);
    void GetBitrate();
    void GetBitrate(TUint& aBitrate);
    void ToggleVad();
    void GetVad();
    void ToggleCng();
    void GetCng();
    void TogglePlc();
    void GetPlc();
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

    void SetOutputDevice(QTMSAudioOutput device);
    void SetHandset();
    void SetLoudSpeaker();
    void GetAudioDevice();
    void GetAvailableOutput();
    void DoLoopback();
    void OneTouchLoopback();
    void EndCall();

    void InitDTMFTonePlayerDnlink();
    void InitDTMFTonePlayerUplink();
    void DTMFTonePlayDnlink();
    void DTMFTonePlayUplink();
    void CloseDTMFPlayerDnlink();
    void CloseDTMFPlayerUplink();

    gint CreateRingTonePlayer();
    void InitRingTonePlayerFromProfiles();
    void InitRingTonePlayerFromFile();
    void InitRingToneVideoPlayer();
    void InitRingToneSequencePlayer();
    void InitRingToneWithTTS();
    void InitRingToneBeepOnce();
    void InitRingToneSilent();
    void InitRingToneUnsecureVoIP();
    void PlayRingTone();
    void StopRingTone();
    void MuteRingTone();
    void PauseVideoRingTone();
    void CloseRingTonePlayer();
    void DeinitRingTonePlayer();

    void SetCallType(QTMSCallType calltype);

    void CreateInbandTonePlayer();
    void StartInbandTone(QTMSInbandToneType inbandtonetype);
    void StopInbandTone();

    gint CreateFactory();
    gint CreateCall(QTMSCallType calltype);
    gint CreateUplink();
    gint CreateDownlink();
    gint CreateClientSource();
    gint CreateModemSource();
    gint AddClientSourceToStream();
    gint AddModemSourceToStream();
    gint CreateClientSink();
    gint CreateModemSink();
    gint AddClientSinkToStream();
    gint AddModemSinkToStream();
    gint CreateMicSource();
    gint AddMicSourceToStream();
    gint CreateSpeakerSink();
    gint AddSpeakerSinkToStream();

    gint CreateVolumeEffect();
    gint AddVolumeEffectToStream();
    gint CreateGainEffect();
    gint CreateGlobalRouting();
    gint AddGainEffectToStream();

    void CreateGlobalVol();
    void CreateGlobalGain();
    void GetGlobalVol();
    void GetGlobalMaxVol();
    void SetGlobalVol();
    void GetGlobalGain();
    void GetGlobalMaxGain();
    void SetGlobalGain();

public slots:

    //Signal from QTMSClientSource
    void FillBuffer(QTMSBuffer& buffer);

    void BufferProcessed(const QTMSBuffer* buffer, gint reason);

    //Signal from QTMSClientSink
    void ProcessBuffer(const QTMSBuffer* buffer);

    //Signal from QTMSStream
    void TMSStreamEvent(const QTMSStream& stream, QTMSSignalEvent event);

    //Signal from QTMSEffect
    void EffectsEvent(const QTMSEffect& tmseffect, QTMSSignalEvent event);

    //Signal from QTMSGlobalRouting
    void GlobalRoutingEvent(const QTMSGlobalRouting& routing,
            QTMSSignalEvent event, QTMSAudioOutput output);

    //Signal from QTMSRingTone
    void RingtoneEvent(const QTMSRingTone& rt, QTMSSignalEvent event);

    //Signal from QTMSDTMF
    void DTMFEvent(const QTMSDTMF& dtmf, QTMSSignalEvent event);

    //Signal from QTMSInbandTone
    void InbandToneEvent(const QTMSInbandTone& inbandtone,
            QTMSSignalEvent event);

private:

    void ConstructL(QTextEdit* statusDisplay);
    QTmsTestEngine();

    void ConfigureDecoder();
    void ConfigureEncoder();

    void DisplayFormat(QTMSFormatType aFormat);
    void DisplayDevice(QTMSAudioOutput device);
    void DisplayText(const QString& str, const gint num = 0);

#ifdef __RECORD_WAV_TO_FILE__
    void WriteToFile(const guint8* str, const guint len);
#endif //__RECORD_WAV_TO_FILE__

private:

    // For text printing to the display
    QTextEdit* iStatusDisplay;

    // Dn/Uplink, DTMF, Ring tone APIs
    QTMSFactory*       iFactory;
    QTMSCall*          iTmsCall;
    QTMSCallType       iCallType;
    QTMSStream*        iTmsDnlink;
    QTMSStream*        iTmsUplink;
    QTMSSource*        iTmsClientSource;
    QTMSSource*        iTmsMicSource;
    QTMSSource*        iTmsModemSource;
    QTMSSink*          iTmsClientSink;
    QTMSSink*          iTmsSpeakerSink;
    QTMSSink*          iTmsModemSink;
    QTMSDTMF*          iDTMFTonePlayerDn;
    QTMSDTMF*          iDTMFTonePlayerUp;
    QTMSEffect*        iTmsUplinkEffect;
    QTMSEffect*        iTmsDnlinkEffect;
    QTMSEffect*        iGlobalVol;
    QTMSEffect*        iGlobalGain;
    QTMSGlobalRouting* iTmsGlobalRouting;
    QTMSRingTone*      iTmsRingTonePlayer;
    QTMSInbandTone*    iInbandTonePlayer;

    // Codec format APIs
    QTMSFormat* iDecFormatIntfc;
    QTMSFormat* iEncFormatIntfc;

    // For internal app state control
    TAppState iDnLinkStatus;
    TAppState iUpLinkStatus;
    QTMSFormatType iDnLinkCodec;
    QTMSFormatType iUpLinkCodec;
    TBool iOneTouchLoopback;

    // For storing codec configuration locally
    TBool iPlc;
    TBool iCng;
    TBool iVad;
    TBool iSpkr;
    guint iMaxGain;
    guint iMaxVolume;
    guint iVolume;
    TUint iBitrate;
    BitRateVector iBitratesVector;
    OutputVector iAvailableoutputs;

    // For streaming buffer control
    TBool iPlayBufReady;
    TBool iRecBufReady;
    TInt iPlayBufMaxLen;
    QTMSBuffer* iPlayBuf;
    QTMSBuffer* iRecBuf;
    TUint iSeqNum;
    TUint iSeqNumIncrement;
    gint iSizeCopy;
    guint8* iDesCopy;

    QTMS::FormatVector iUplCodecs;
    QTMS::FormatVector iDnlCodecs;
    GString* iRTStr;
    GString* iTTSStr;

#ifdef __WINSCW__
    TInt iBufIndex;
#endif //__WINSCW__

#ifdef __PLAY_WAV_FROM_FILE__
    RFile iFile;
    TInt iFileLen;
    TInt iReadSize;
    TInt iReadPos;
    TBool iEOF;
    RFs iFs;
    HBufC8* iBuf;
#endif

#ifdef __RECORD_WAV_TO_FILE__
    RFile iFile;
    TInt iWriteLen;
    TInt iReadPos;
    RFs iFs;
    HBufC8* iWriteBuf;
#endif //__RECORD_WAV_TO_FILE__
    };

#endif //QTMSTESTENGINE_H

// End of file
