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
 * Description: voip audio service - Stif test
 *
 */

#ifndef VOIPAUDIOSERVICESTESTCLASS_H
#define VOIPAUDIOSERVICESTESTCLASS_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

#include <e32svr.h>
#include <e32base.h>
#include <e32cons.h>
#include <VoIPUtilityFactory.h>
#include <VoIPDownlinkStream.h>
#include <VoIPUplinkStream.h>
#include <VoIPFormatIntfc.h>
#include <VoIPG711DecoderIntfc.h>
#include <VoIPG711EncoderIntfc.h>
#include <VoIPG729DecoderIntfc.h>
#include <VoIPILBCDecoderIntfc.h>
#include <VoIPILBCEncoderIntfc.h>
#include <VoIPDtmfTonePlayer.h>
#include <VoIPRingTonePlayer.h>
#include <VoIPRingToneFactory.h>
#include <VoIPJitterBufferIntfc.h>
#include "TimeoutController.h"

// CONSTANTS
const TInt KErrBadTestParameter = -1000; // Error on configuration file

const TInt KErrEventPending = -2000; // Timeout and not callback
const TInt KErrCallbackErrorCode = -2001; // Callback return error code
const TInt KErrUnexpectedValue = -2002; // Unexpected value at setting
const TInt KErrTimeoutController = -2007; // Unexpected notification

const TInt KShortTimeout = 2000;
const TInt KMediumTimeout = 4000;
const TInt KLongTimeout = 10000;

const TUint KDefaultSampleRateInkHz = 8; // for sequence number calculation
const TUint KMaxSeqNumber = 0xFFFF; //65535

_LIT( KMsgBadTestParameters, "[Error] No valid test case parameters");
_LIT( KTagG711, "G711");
_LIT( KTagG711_10MS, "G711_10MS");
_LIT( KTagG729, "G729");
_LIT( KTagILBC, "iLBC");
_LIT( KTagAMRNB, "AMR-NB");
_LIT( KTagPCM16, "PCM16");
_LIT( KTagNULL, "NULL");
_LIT( KTagHandset, "Handset");
_LIT( KTagLoudSpeaker, "LoudSpeaker");
_LIT( KTagUplink, "Uplink");
_LIT( KTagDnlink, "Dnlink");
_LIT( KTagStdBuf, "StdBuf");
_LIT( KTagJbBuf, "JbBuf");
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

#ifdef __WINSCW__
_LIT(KVoIPAudioServicesTestClassLogPath, "\\logs\\testframework\\");
#else
// Note: The path must match setting in VoIPAudioServicesTestClass.ini
_LIT(KVoIPAudioServicesTestClassLogPath, "e:\\"); //write to MMC
//_LIT(KVoIPAudioServicesTestClassLogPath, "f:\\"); //write to MMC
#endif

_LIT16(KTestFile1, "C:\\Data\\Sounds\\Digital\\NokiaTest.aac");
_LIT( KVoIPAudioServicesTestClassLogFile, "CVoIPAudioServicesTestClass.txt" );
_LIT8(KMimetypeWAV, "audio/wav");

const TUint KTonesBufSize = 6;
const TUint KMimeStringSize = 32;

// FORWARD DECLARATIONS
class CVoIPAudioServicesTestClass;
class CVoIPDataBuffer;
class CVoIPJBDataBuffer;

// CLASS DECLARATION

/**
 *  CRadioUtilityTestClass test class for STIF Test Framework TestScripter.
 */
NONSHARABLE_CLASS(CVoIPAudioServicesTestClass) : public CScriptBase,
        public MTimeoutObserver,
        public MVoIPDownlinkObserver,
        public MVoIPUplinkObserver,
        public MVoIPFormatObserver,
        public MRingToneObserver,
        public MDTMFToneObserver,
        public MVoIPJitterBufferObserver
    {
    enum TAppState
        {
        ENotReady = 0,
        EReady,
        EStreaming
        };

public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CVoIPAudioServicesTestClass* NewL(CTestModuleIf& aTestModuleIf);

    /**
     * Destructor.
     */
    virtual ~CVoIPAudioServicesTestClass();

    enum TVoIPExpectedEvent
        {
        EOpenDownlinkComplete,
        EOpenUplinkComplete,
        EDownlinkClosed,
        EUplinkClosed,
        EOpenCompleteDNL,
        EOpenCompleteUPL,
        EOpenComplete,
        EPlaybackComplete,
        EEmptyBuffer,
        EFillBuffer,
        EDownlinkError,
        EUplinkError
        };

public:
    /**
     * From CScriptBase Runs a script line.
     * @param aItem Script line containing method name and parameters
     * @return Symbian OS error code
     */
    virtual TInt RunMethodL(CStifItemParser& aItem);

    // From MVoIPDownlinkObserver
    virtual void FillBuffer(const CVoIPAudioDownlinkStream& aSrc,
            CVoIPDataBuffer* aBuffer);
    virtual void Event(const CVoIPAudioDownlinkStream& aSrc, TInt aEventType,
            TInt aError);

    // From MVoIPUplinkObserver
    virtual void EmptyBuffer(const CVoIPAudioUplinkStream& aSrc,
            CVoIPDataBuffer* aBuffer);
    virtual void Event(const CVoIPAudioUplinkStream& aSrc, TInt aEventType,
            TInt aError);

    // From MVoIPFormatObserver
    virtual void Event(const CVoIPFormatIntfc& aSrc, TInt aEventType);

    // From MRingToneObserver

    /**
     * @param CRingTonePlayer& - Ring tone player reference
     * @param TInt - Event type
     * @param TInt - Status
     * @return void
     */
    virtual void Event(const CRingTonePlayer& aSrc, TInt aEventType,
            TInt aError);

    // From MDTMFToneObserver

    /**
     * @param CDTMFTonePlayer& - DTMF tone player reference
     * @param TInt - Event type
     * @param TInt - Status
     * @return void
     */
    virtual void Event(const CDTMFTonePlayer& aPlayer, TInt aEventType,
            TInt aError);

    // From MTimeoutObserver

    /**
     * @param none
     * Review if all the expected events have ocurred once the time is over
     */
    void HandleTimeout(TInt error);

    // From MVoIPJitterBufferObserver
    virtual void Event(const CVoIPJitterBufferIntfc& aSrc, TInt aEventType);

protected:
    // Functions from base classes

private:

    /**
     * C++ default constructor.
     */
    CVoIPAudioServicesTestClass(CTestModuleIf& aTestModuleIf);

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
    void AddExpectedEvent(TVoIPExpectedEvent event, TInt ms);

    /**
     * Unset an event as expected
     */
    TBool RemoveExpectedEvent(TVoIPExpectedEvent event);

    /**
     * @param none
     * Removes all expected events
     */
    void RemoveAllExpectedEvents();

    /**
     * Verify that the event was expected, removes it from the list
     * Signal the TestScripter with the returned error code
     */
    void ProcessEvent(TVoIPExpectedEvent aEvent, TInt aError);

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

    TInt CreateVoIPUtilityFactory(CStifItemParser& aItem);
    TInt CreateRingToneFactory(CStifItemParser& aItem);
    TInt CreateDownlinkStream(CStifItemParser& aItem);
    TInt CreateDownlinkStreamJB(CStifItemParser& aItem);
    TInt GetDownlinkVersion(CStifItemParser& aItem);
    TInt CreateUplinkStream(CStifItemParser& aItem);
    TInt GetUplinkVersion(CStifItemParser& aItem);
    TInt CreateDTMFTonePlayer(CStifItemParser& aItem);
    TInt CreateRingTonePlayer(CStifItemParser& aItem);
    TInt GetSupportedDownlinkFormats(CStifItemParser& aItem);
    TInt GetSupportedUplinkFormats(CStifItemParser& aItem);
    TInt SetDownlinkFormat(CStifItemParser& aItem);
    TInt SetUplinkFormat(CStifItemParser& aItem);
    TInt OpenDownlink(CStifItemParser& aItem);
    TInt OpenUplink(CStifItemParser& aItem);

    TInt Start(CStifItemParser& aItem);
    TInt Stop(CStifItemParser& aItem);
    TInt Close(CStifItemParser& aItem);
    TInt Gain(CStifItemParser& aItem);
    TInt Volume(CStifItemParser& aItem);
    TInt SetDevice(CStifItemParser& aItem);

    TInt InitDTMFTonePlayer(CStifItemParser& aItem);
    TInt DTMFTonePlay(CStifItemParser& aItem);
    TInt CloseDTMFPlayer(CStifItemParser& aItem);
    TInt StopDTMFTonePlayer(CStifItemParser& aItem);

    TInt InitRingTonePlayer(CStifItemParser& aItem);
    TInt CloseRingTonePlayer(CStifItemParser& aItem);
    TInt PlayRingTone(CStifItemParser& aItem);
    TInt PauseRingTone(CStifItemParser& aItem);
    TInt ResumeRingTone(CStifItemParser& aItem);
    TInt StopRingTone(CStifItemParser& aItem);
    TInt GetBufferType(CStifItemParser& aItem);

    TInt GetSupportedBitrates(CStifItemParser& aItem);
    TInt SetBitrate(CStifItemParser& aItem);
    TInt GetBitrate(CStifItemParser& aItem);
    TInt GetVAD(CStifItemParser& aItem);
    TInt ToggleVAD(CStifItemParser& aItem);
    TInt GetFrameMode(CStifItemParser& aItem);
    TInt ToggleFrameMode(CStifItemParser& aItem);
    TInt GetFrameModeRqrdForEC(CStifItemParser& aItem);
    TInt ConcealErrForNextBuf(CStifItemParser& aItem);
    TInt GetMode(CStifItemParser& aItem);
    TInt SetMode(CStifItemParser& aItem);
    TInt GetCNG(CStifItemParser& aItem);
    TInt ToggleCNG(CStifItemParser& aItem);
    TInt GetPLC(CStifItemParser& aItem);
    TInt TogglePLC(CStifItemParser& aItem);
    TInt BadLsfNextBuffer(CStifItemParser& aItem);
    TInt BadLsfNextBuffer();

    void DoLoopback();
    void DisplayFormat(TVoIPCodecFormat aFormat);
    TInt GetSupportedBitrates(RArray<TUint>& aBrArr);
    TInt SetBitrate(TUint aBitrate);
    TInt GetBitrate(TUint& aBitrate);

    TInt ConfigEncAudDevice(CStifItemParser& aItem);
    TInt SetLoopPlay(CStifItemParser& aItem);
    TInt TestNullObjects(CStifItemParser& aItem);

    TInt ConfigureJB(CStifItemParser& aItem);
    TInt ResetJB(CStifItemParser& aItem);
    TInt DelayDown(CStifItemParser& aItem);
    TInt DelayUp(CStifItemParser& aItem);
    void DoJBLoopback();

private:
    // Data

    // reference to TestModuleIf
    CTestModuleIf& iTestModuleIf;

    // Dn/Uplink, DTMF, Ring tone APIs
    CVoIPUtilityFactory* iFactory;
    CVoIPAudioDownlinkStream* iVoIPDnlink;
    CVoIPAudioUplinkStream* iVoIPUplink;
    CDTMFTonePlayer* iDTMFTonePlayerDn;
    CDTMFTonePlayer* iDTMFTonePlayerUp;
    CRingToneFactory* iRingToneFactory;
    CRingTonePlayer* iRingTonePlayer;

    // Codec format APIs
    CVoIPFormatIntfc* iDecFormatIntfc;
    CVoIPFormatIntfc* iEncFormatIntfc;

    CVoIPG711EncoderIntfc* iG711EncFormatIntfc;
    CVoIPG711DecoderIntfc* iG711DecFormatIntfc;
    CVoIPFormatIntfc* iG729EncFormatIntfc;
    CVoIPG729DecoderIntfc* iG729DecFormatIntfc;
    CVoIPILBCEncoderIntfc* iILBCEncFormatIntfc;
    CVoIPILBCDecoderIntfc* iILBCDecFormatIntfc;
    CVoIPFormatIntfc* iAMRNBEncFormatIntfc;
    CVoIPFormatIntfc* iAMRNBDecFormatIntfc;
    CVoIPFormatIntfc* iPCM16EncFormatIntfc;
    CVoIPFormatIntfc* iPCM16DecFormatIntfc;

    CVoIPJitterBufferIntfc* iJBIntfc;
    CVoIPJBDataBuffer* iPlayJBuf;
    TUint iSeqNum;
    TUint iSeqNumIncrement;

    // For internal app state control
    TAppState iDnLinkStatus;
    TAppState iUpLinkStatus;
    TVoIPCodecFormat iDnLinkCodec;
    TVoIPCodecFormat iUpLinkCodec;

    // For storing codec configuration locally
    TBool iPlc;
    TBool iCng;
    TBool iVad;
    TBool iFrameMode;
    TBool iFrameModeReqEC;
    TBool iSpkr;
    TInt iMaxGain;
    TInt iGain;
    TInt iMaxVolume;
    TInt iVolume;
    TUint iBitrate;
    RArray<TUint> iBitratesArray;

    // For streaming buffer control
    CVoIPDataBuffer* iPlayBuf;
    CVoIPDataBuffer* iRecBuf;
    TPtr8 iPlayBufPtr;
    TPtr8 iRecBufPtr;

    TBool iPlayBufReady;
    TBool iRecBufReady;

    // For ring tone playback
    RFile iRTFile;
    RFs iFs;

    // Active object with a timer to timeout the test case
    CSimpleTimeout * iTimeoutController;

    // Indicates if the test case use a normal exit reason
    TBool iNormalExitReason;

    // List of expected events
    RArray<TVoIPExpectedEvent> iExpectedEvents;

    // List of notification event (by the callback)
    RArray<TVoIPExpectedEvent> iOcurredEvents;

    CActiveSchedulerWait* iActive;
    TInt iRecCounter;
    TInt iPlayCounter;
    TBool iLoopPlay;
    };

#endif //VOIPAUDIOSERVICESTESTCLASS_H
