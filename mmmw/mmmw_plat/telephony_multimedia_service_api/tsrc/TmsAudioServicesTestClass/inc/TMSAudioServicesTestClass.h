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
 * Description: Telephony Multimedia Service - STIF TEST
 *
 */

#ifndef TMSAUDIOSERVICESTESTCLASS_H
#define TMSAUDIOSERVICESTESTCLASS_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

#include <e32svr.h>
#include <e32base.h>
#include <e32cons.h>
#include <tms.h>
#include <tmsfactory.h>
#include <tmscall.h>
#include <tmsstream.h>
#include <tmsformat.h>
#include <tmsclientsink.h>
#include <tmsclientsource.h>
#include <tmsclientsourceobsrvr.h>
#include <tmsclientsinkobsrvr.h>
#include <tmsmicsource.h>
#include <tmsmodemsource.h>
#include <tmsspeakersink.h>
#include <tmsmodemsink.h>
#include <tmsvolumeeffect.h>
#include <tmsglobalvoleffect.h>
#include <tmsgaineffect.h>
#include <tmsglobalgaineffect.h>
#include <tmseffectobsrvr.h>
#include <tmsstreamobsrvr.h>
#include <tmsbuffer.h>
#include <tmsglobalrouting.h>
#include <tmsglobalroutingobsrvr.h>
#include <tmsg711format.h>
#include <tmsg729format.h>
#include <tmsilbcformat.h>
#include <tmspcmformat.h>
#include <tmsamrformat.h>
#include <tmsdtmf.h>
#include <tmsdtmfobsrvr.h>
#include <tmsringtone.h>
#include <tmsringtoneobsrvr.h>
#include <tmsinbandtone.h>
#include <tmsinbandtoneobsrvr.h>

#include "TimeoutController.h"

using namespace TMS;

// CONSTANTS
const TInt KErrBadTestParameter = -1000; // Error on configuration file
const TInt KErrEventPending = -2000; // Timeout and not callback
const TInt KErrCallbackErrorCode = -2001; // Callback return error code
const TInt KErrUnexpectedValue = -2002; // Unexpected value at setting
const TInt KErrTimeoutController = -2007; // Unexpected notification

const TInt KMicroTimeout = 1000;
const TInt KShortTimeout = 2000;
const TInt KMediumTimeout = 4000;
const TInt KLongTimeout = 10000;

_LIT( KMsgBadTestParameters, "[Error] No valid test case parameters");

// General
_LIT( KTagG711, "G711");
_LIT( KTagG729, "G729");
_LIT( KTagILBC, "iLBC");
_LIT( KTagAMRNB, "AMR-NB");
_LIT( KTagPCM16, "PCM16");

_LIT( KTagUplink, "Uplink");
_LIT( KTagDnlink, "Dnlink");
_LIT( KTagFile, "File");
_LIT( KTagRFile, "RFile");
_LIT( KTagProfile, "Profile");
_LIT( KTagDesL, "DesL");
_LIT( KTagMin, "MIN");
_LIT( KTagMax, "MAX");
_LIT( KTagULaw, "uLaw");
_LIT( KTagALaw, "aLaw");
_LIT( KTag20ms, "20ms");
_LIT( KTag30ms, "30ms");
_LIT( KTagFile1, "File1");
_LIT( KTagFile2, "File2");

_LIT( KTagNone, "None");
_LIT( KTagHandset, "Handset");
_LIT( KTagLoudspeaker, "Loudspeaker");
_LIT( KTagWiredAccessory, "WiredAccessory");
_LIT( KTagAccessory, "Accessory");
_LIT( KTagTTY, "TTY");
_LIT( KTagPublic, "Public");
_LIT( KTagPrivate, "Private");

_LIT( KTagRTDefault, "RTDefault");
_LIT( KTagRTFile, "RTFile");
_LIT( KTagRTBeepOnce, "RTBeepOnce");
_LIT( KTagRTSilent, "RTSilent");
_LIT( KTagRTUnsecureVoIP, "RTUnsecureVoIP");
_LIT( KTagRTSequence, "RTSequence");
_LIT( KTagRTTts, "RTTts");

_LIT( KTagUserBusy, "UserBusy" );
_LIT( KTagRadioPathNotFound, "RadioPathNotFound" );
_LIT( KTagCongestion, "Congestion" );
_LIT( KTagSpecialInfo, "SpecialInfo" );
_LIT( KTagReorder, "Reorder" );
_LIT( KTagAlerting, "Alerting" );
_LIT( KTagWaiting, "Waiting" );
_LIT( KTagDataCall, "DataCall" );
_LIT( KTagNoSequence, "NoSequence" );
_LIT( KTagBeepSequence, "BeepSequence" );

// MACROS

#ifdef __WINSCW__
_LIT(KTmsAudioServicesTestClassLogPath, "\\logs\\testframework\\");
#else
// Note: The path must match setting in TmsAudioServicesTestClass.ini
_LIT(KTmsAudioServicesTestClassLogPath, "e:\\");    //Write to MMC
//_LIT(KTmsAudioServicesTestClassLogPath, "f:\\");  //Write to MMC
#endif

_LIT16(KTestFile1, "C:\\Data\\Sounds\\Digital\\NokiaTest.aac");
_LIT8(KRTBeepSequence, "\x00\x11\x0A\x0A\x08\x73\x0A\x40\x28\x0A\xF7\
\x05\xFC\x40\x64\x0A\x08\x40\x32\x0A\xF7\x06\x0B");
_LIT16(KTextToSpeak, "THE PHONE IS RINGING");

// Log file
_LIT(KTmsAudioServicesTestClassLogFile, "CTmsAudioServicesTestClass.txt");
const TUint KTonesBufSize = 6;
const TUint KMimeStringSize = 32;

// FORWARD DECLARATIONS
class CTmsAudioServicesTestClass;
class CTmsDataBuffer;
class TMSProxy;

// CLASS DECLARATION

/**
 *  CRadioUtilityTestClass test class for STIF Test Framework TestScripter.
 */
NONSHARABLE_CLASS(CTmsAudioServicesTestClass) :
    public CScriptBase,
    public MTimeoutObserver,
    public TMSStreamObserver,
    public TMSClientSourceObserver,
    public TMSClientSinkObserver,
    public TMSEffectObserver,
    public TMSGlobalRoutingObserver,
    public TMSRingToneObserver,
    public TMSDTMFObserver,
    public TMSInbandToneObserver
    {
    enum StreamState
        {
        UNINITIALIZED = 0,
        INITIALIZED,
        PAUSED,
        STARTED,
        };

public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CTmsAudioServicesTestClass* NewL(CTestModuleIf& aTestModuleIf);

    /**
     * Destructor.
     */
    virtual ~CTmsAudioServicesTestClass();

    enum TTmsExpectedEvent
        {
        EOpenDownlinkComplete,
        EOpenUplinkComplete,
        EDownlinkClosed,
        EUplinkClosed,
        EOpenCompleteDNL,
        EOpenCompleteUPL,
        EStreamPaused,
        EStreamStarted,
        EOpenComplete,
        EPlaybackComplete,
        EEmptyBuffer,
        EFillBuffer,
        EOutputChanged,
        ESetOutputComplete,
        ERTInitComplete,
        ERTPlayComplete,
        ERTDeinitComplete,
        EInbToneStarted,
        EInbToneStopped,
        EDTMFToneStarted,
        EDTMFToneStopped,
        EStreamVolChange,
        EStreamGainChange,
        EGlobalVolChange,
        EGlobalGainChange
        };

public:
    /**
     * From CScriptBase Runs a script line.
     * @param aItem Script line containing method name and parameters
     * @return Symbian OS error code
     */
    virtual TInt RunMethodL(CStifItemParser& aItem);

    // From MTimeoutObserver

    /**
     * @param none
     * Review if all the expected events have ocurred once the time is over
     */
    void HandleTimeout(TInt error);

    //From TMSStreamObserver
    void TMSStreamEvent(const TMSStream& stream, TMSSignalEvent event);

    //From TMSClientSourceObserver
    void FillBuffer(TMSBuffer& buffer);
    void BufferProcessed(const TMSBuffer* buffer, gint reason);

    //From TMSClientSinkObserver
    void ProcessBuffer(const TMSBuffer* buffer);

    //From TMSClientSinkObserver starts
    void EffectsEvent(const TMSEffect& tmseffect, TMSSignalEvent event);

    // from TMSGlobalRoutingObserver
    void GlobalRoutingEvent(const TMSGlobalRouting& /*routing*/,
            TMSSignalEvent event, TMSAudioOutput output);

    // from TMSRingToneObserver
    void RingtoneEvent(const TMSRingTone& rt, TMSSignalEvent event);

    // from TMSDTMFObserver
    void DTMFEvent(const TMSDTMF& dtmf, TMSSignalEvent event);

    // from TMSInbandToneObserver
    void InbandToneEvent(const TMSInbandTone& inbandtone, TMSSignalEvent event);

private:

    /**
     * C++ default constructor.
     */
    CTmsAudioServicesTestClass(CTestModuleIf& aTestModuleIf);

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

    /**
     * Frees all resources allocated from test methods.
     */
    void Delete();

    /**
     * Set an event as expected and set default timeout
     */
    void AddExpectedEvent(TTmsExpectedEvent event, TInt ms);

    /**
     * Unset an event as expected
     */
    TBool RemoveExpectedEvent(TTmsExpectedEvent event);

    /**
     * @param none
     * Removes all expected events
     */
    void RemoveAllExpectedEvents();

    /**
     * Verify that the event was expected, removes it from the list
     * Signal the TestScripter with the returned error code
     */
    void ProcessEvent(TTmsExpectedEvent aEvent, TInt aError);

    /**
     * Maps a event with a descriptor with its name
     */
    TPtrC EventName(TInt aKey);

    /**
     * Sets a timeout different since the default
     * @param aItem Script line containing parameters.
     * @return Symbian OS error code.
     */
    TInt SetTimeout(CStifItemParser& aItem);

    /**
     *
     * @param aItem Script line containing parameters.
     * @return Symbian OS error code.
     */
    TInt SetExpectedEvents(CStifItemParser& aItem);

    /**
     *
     * @param aItem Script line containing parameters.
     * @return Symbian OS error code.
     */
    TInt SetAllowedPanic(CStifItemParser& aItem);

    /**
     * Connect to the default client to the server
     * @param aItem Script line containing parameters.
     * @return Symbian OS error code.
     */

    TInt CreateTmsFactory(CStifItemParser& aItem);
    TInt CreateCall(CStifItemParser& aItem);
    TInt CreateFormat(CStifItemParser& aItem);
    TInt CreateGlobalRouting(CStifItemParser& aItem);

    TInt CreateBuffer(CStifItemParser& aItem);
    TInt CreateEffect(CStifItemParser& aItem);
    TInt CreateSource(CStifItemParser& aItem);
    TInt CreateSink(CStifItemParser& aItem);
    TInt DeleteSource(CStifItemParser& aItem);
    TInt DeleteSink(CStifItemParser& aItem);
    TInt GetSourceType(CStifItemParser& aItem);
    TInt GetSinkType(CStifItemParser& aItem);
    TInt GetEffectType(CStifItemParser& aItem);
    TInt CreateDownlinkStream(CStifItemParser& aItem);
    TInt GetDownlinkVersion(CStifItemParser& aItem);
    TInt CreateUplinkStream(CStifItemParser& aItem);
    TInt GetUplinkVersion(CStifItemParser& aItem);
    TInt GetSupportedFormats(CStifItemParser& aItem);

    TInt IsCallTypeSupported(CStifItemParser& aItem);
    TInt SetDownlinkFormat(CStifItemParser& aItem);
    TInt SetUplinkFormat(CStifItemParser& aItem);
    TInt ReSetDownlinkFormat(CStifItemParser& aItem);
    TInt ReSetUplinkFormat(CStifItemParser& aItem);
    TInt OpenDownlink(CStifItemParser& aItem);
    TInt OpenUplink(CStifItemParser& aItem);

    TInt Start(CStifItemParser& aItem);
    TInt Pause(CStifItemParser& aItem);
    TInt Stop(CStifItemParser& aItem);
    TInt Close(CStifItemParser& aItem);
    TInt Gain(CStifItemParser& aItem);
    TInt Volume(CStifItemParser& aItem);

    TInt CreateDTMFTonePlayer(CStifItemParser& aItem);
    TInt DTMFTonePlay(CStifItemParser& aItem);
    TInt StopDTMFTonePlayer(CStifItemParser& aItem);
    TInt ContinueDTMFStringSending(CStifItemParser& aItem);
    TInt CloseDTMFPlayer(CStifItemParser& aItem);

    TInt CreateRingTonePlayer(CStifItemParser& aItem);
    TInt CreateVideoRingTonePlayer(CStifItemParser& aItem);
    TInt InitRingTonePlayer(CStifItemParser& aItem);
    TInt PlayRingTone(CStifItemParser& aItem);
    TInt PlayRingToneNoEvent(CStifItemParser& aItem);
    TInt PauseRingTone(CStifItemParser& aItem); //IP video call only (10.2)
    TInt StopRingTone(CStifItemParser& aItem);
    TInt MuteRingTone(CStifItemParser& aItem);
    TInt CloseRingTonePlayer(CStifItemParser& aItem);

    TInt CreateInbandTonePlayer(CStifItemParser& aItem);
    TInt StartInbandTone(CStifItemParser& aItem);
    TInt StopInbandTone(CStifItemParser& aItem);
    TInt CloseInbandTonePlayer(CStifItemParser& aItem);

    //  TInt GetSupportedBitrates( CStifItemParser& aItem );

    TInt GetBitRateList(CStifItemParser& aItem);
    TInt SetBitrates(CStifItemParser& aItem);
    TInt GetBitrates(CStifItemParser& aItem);
    TInt GetType(CStifItemParser& aItem);
    TInt GetVAD(CStifItemParser& aItem);
    TInt ToggleVAD(CStifItemParser& aItem);

    TInt GetMode(CStifItemParser& aItem);
    TInt SetMode(CStifItemParser& aItem);
    TInt GetCNG(CStifItemParser& aItem);
    TInt ToggleCNG(CStifItemParser& aItem);
    TInt GetPLC(CStifItemParser& aItem);
    TInt TogglePLC(CStifItemParser& aItem);

    void DoLoopback();
    void DisplayFormat(TMSFormatType aFormat);
    void GetSupportedBitrates(RArray<TUint>& aBrArr);
    void DisplayDevice(TMSAudioOutput device);
    TInt GetBufferType(CStifItemParser& aItem);

    TInt SetLoopPlay(CStifItemParser& aItem);
    TInt ConfigEncAudDevice(CStifItemParser& aItem);

    TInt AddClientSrcToDnlStream(CStifItemParser& aItem);
    TInt AddMicSrcToUplStream(CStifItemParser& aItem);
    TInt AddModemSrcToDnlStream(CStifItemParser& aItem);
    TInt RemoveClientSrcFromDnlStream(CStifItemParser& aItem);
    TInt RemoveMicSrcFromUplStream(CStifItemParser& aItem);
    TInt RemoveModemSrcFromDnlStream(CStifItemParser& aItem);
    TInt AddSourceObserver(CStifItemParser& aItem);
    TInt RemoveSourceObserver(CStifItemParser& aItem);

    TInt AddClientSinkToUplStream(CStifItemParser& aItem);
    TInt AddModemSinkToUplStream(CStifItemParser& aItem);
    TInt AddSpkrSinkToDnlStream(CStifItemParser& aItem);
    TInt RemoveClientSinkFromUplStream(CStifItemParser& aItem);
    TInt RemoveModemSinkFromUplStream(CStifItemParser& aItem);
    TInt RemoveSpkrSinkFromDnlStream(CStifItemParser& aItem);
    TInt AddSinkObserver(CStifItemParser& aItem);
    TInt RemoveSinkObserver(CStifItemParser& aItem);

    gint CreateVolumeEffect();
    gint CreateVolumeGlobalEffect();
    TInt AddVolumeEffectToStream(CStifItemParser& aItem);
    TInt AddGlobalVolumeEffectToStream(CStifItemParser& aItem);
    TInt RemoveGlobalVolumeEffectToStream(CStifItemParser& aItem);
    gint CreateGainEffect();
    gint CreateGlobalGainEffect();
    TInt AddGainEffectToStream(CStifItemParser& aItem);
    TInt AddGlobalGainEffectToStream(CStifItemParser& aItem);
    TInt RemoveGlobalGainEffectToStream(CStifItemParser& aItem);
    TInt GetStreamType(CStifItemParser& aItem);
    TInt GetStreamState(CStifItemParser& aItem);

    TInt SetOutput(CStifItemParser& aItem);
    TInt GetOutput(CStifItemParser& aItem);
    TInt GetPreviousOutput(CStifItemParser& aItem);
    TInt GetAvailableOutputs(CStifItemParser& aItem);
    TInt AddGlobalRoutingObserver(CStifItemParser& aItem);
    TInt DeleteGlobalRoutingObserver(CStifItemParser& aItem);

    TInt TestRTPSession(CStifItemParser& aItem);
    TInt CreateStreamTest(CStifItemParser& aItem);
    TInt CreateCallTest(CStifItemParser& aItem);
    TInt CreateDTMFTest(CStifItemParser& aItem);
    TInt CreateInbandToneTest(CStifItemParser& aItem);
    TInt CreateRingToneTest(CStifItemParser& aItem);
    TInt CreateClientSinkTest(CStifItemParser& aItem);
    TInt CreateClientSourceTest(CStifItemParser& aItem);
    TInt CreateSinksTest(CStifItemParser& aItem);
    TInt CreateSourcesTest(CStifItemParser& aItem);
    TInt CreateGlobalVolEffectTest(CStifItemParser& aItem);
    TInt CreateGlobalGainEffectTest(CStifItemParser& aItem);
    TInt CreateVolumeEffectTest(CStifItemParser& aItem);
    TInt CreateGainEffectTest(CStifItemParser& aItem);
    TInt CreateGlobalRoutingTest(CStifItemParser& aItem);
    TInt CreateFormatsTest(CStifItemParser& aItem);
    TInt CreateStreamFailTest(CStifItemParser& aItem);
    TInt TerminateServer(CStifItemParser& aItem);

private:
    // Data

    // reference to TestModuleIf
    CTestModuleIf& iTestModuleIf;

    TMSFactory* iFactory;
    TMSStream* iTmsDnlink;
    TMSCall* iTmsCall;
    TMSStream* iTmsUplink;
    TMSDTMF* iDTMFTonePlayerDn;
    TMSDTMF* iDTMFTonePlayerUp;
    TMSRingTone* iRingTonePlayer;
    TMSInbandTone* iInbandTonePlayer;
    TMSAudioOutput iDevice;

    // Codec format APIs
    TMSBuffer* iTmsBuffer;
    TMSFormat* iTmsFormat;
    TMSFormat* iTmsFormatUp;
    TMSFormat* iTmsFormatDn;
    TMSEffect* iTmsEffect;
    TMSSource *iTmsClientSource;
    TMSSource *iTmsModemSource;
    TMSSource *iTmsMicSource;
    TMSSink *iTmsClientSink;
    TMSSink *iTmsModemSink;
    TMSSink *iTmsSpkrSink;
    TMSEffect* iTmsUplinkEffect;
    TMSEffect* iTmsDnlinkEffect;
    TMSEffect* iGlobalVol;
    TMSEffect* iGlobalGain;
    TMSGlobalRouting* iTmsGlobalRouting;

    // For internal app state control
    StreamState iDnLinkStatus;
    StreamState iUpLinkStatus;
    TMSFormatType iDnLinkCodec;
    TMSFormatType iUpLinkCodec;

    FormatVector iCodec;
    BitRateVector iBitratesVector;
    OutputVector iAvailableoutputs;

    // For storing codec configuration locally
    TBool iPlc;
    TBool iCng;
    TBool iVad;
    guint iMaxGain;
    guint iGain;
    guint iMaxVolume;
    guint iVolume;
    TUint currentBitrate;

    // For streaming buffer control
    TMSBuffer* iPlayBuf;
    TMSBuffer* iRecBuf;
    TBool iPlayBufReady;
    TBool iRecBufReady;
    TBool iLoopPlay;
    TInt iLoopCounter;

    // For ring tone playback
    GString* iRTStr;
    GString* iTTSStr;

    // Active object with a timer to timeout the test case
    CSimpleTimeout * iTimeoutController;

    // Indicates if the test case use a normal exit reason
    TBool iNormalExitReason;

    // List of expected events
    RArray<TTmsExpectedEvent> iExpectedEvents;

    // List of notification event (by the callback)
    RArray<TTmsExpectedEvent> iOcurredEvents;

    CActiveSchedulerWait* iActive;
    TMSProxy* iProxy;
    };

#endif //TMSAUDIOSERVICESTESTCLASS_H

// End of File
