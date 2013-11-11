/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  RadioUtilityTestClass test class for STIF Test Framework TestScripter.
*
*/



#ifndef RADIOUTILITYTESTCLASS_H
#define RADIOUTILITYTESTCLASS_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <RadioMonitor.h>
#include <RadioFmTunerUtility.h>
#include <RadioPlayerUtility.h>
#include <RadioRdsUtility.h>
#include <RadioUtility.h>
#include <RadioFmPresetUtility.h>

#include "TimeoutController.h"


// CONSTANTS
const TInt KErrBadTestParameter= -1000;         // Error on configuration file

const TInt KErrEventPending =-2000;             // Timeout and not callback 
const TInt KErrCallbackErrorCode = -2001;       // Callback return error code
const TInt KErrUnexpectedValue =-2002;          // Unexpected value at setting    
const TInt KExpectedFrequencyChange = -2003;		// Is not finding more than one station
const TInt KErrTimeoutController = -2007;		// Unexpected notification

const TInt KShortTimeout = 1000;
const TInt KMediumTimeout = 4000;
const TInt KLongTimeout = 100000;


_LIT( KMsgBadTestParameters, "[Error] No valid test case parameters");

_LIT( KTagSeek, "Seek");

// General
_LIT( KTagUp, "Up");
_LIT( KTagDown, "Down");
_LIT( KTagLeft, "Left");
_LIT( KTagRight, "Right");
_LIT( KTagCenter, "Center");
_LIT( KTagMaximum, "Max");
_LIT( KTagMinimum, "Min");
_LIT( KTagMaxLeft, "MaxLeft");
_LIT( KTagMaxRight, "MaxRight");
_LIT( KTagHigh, "High");
_LIT( KTagLow, "Low");
_LIT( KTagAll, "All");


// Utilities
_LIT( KTagTuner, "Tuner");
_LIT( KTagPlayer, "Player");
_LIT( KTagRds, "Rds");
_LIT( KTagPreset, "Preset");


// Check
_LIT( KTagCompareNotEqual, "<>");
_LIT( KTagCompareEqual, "=");


// Set & Get
_LIT( KTagPriority, "Priority");
_LIT( KTagForcedMono, "ForcedMonoReception");
_LIT( KTagSquelch, "Squelch");
_LIT( KTagMute, "Mute");
_LIT( KTagVolume, "Volume");
_LIT( KTagVolumeRamp, "VolumeRamp");
_LIT( KTagBalance, "Balance");
_LIT( KTagAmerica, "EFmRangeEuroAmerica");
_LIT( KTagJapan, "EFmRangeJapan");
_LIT( KTagStationSeek, "StationSeek");
_LIT( KTagPlayerState, "PlayerState");

_LIT( KTagFrequencyRange, "FrequencyRange");
_LIT( KTagFrequency, "Frequency");
_LIT( KTagSignalStrength, "SignalStrength");
_LIT( KTagAntenna, "Antenna");
_LIT( KTagFlightMode, "FlightMode");
_LIT( KTagStereo, "StereoMode");

_LIT( KTagAutomaticSwitching, "AutomaticSwitching");


// RDS Data
_LIT( KTagRdsProgrammeIdentification, "RdsProgrammeIdentification");
_LIT( KTagRdsProgrammeType, "RdsProgrammeType");
_LIT( KTagRdsProgrammeService, "RdsProgrammeService");
_LIT( KTagRdsRadioText, "RdsRadioText");
_LIT( KTagRdsClockTime, "RdsClockTime");
_LIT( KTagRdsTrafficAnnouncement, "RdsTrafficAnnouncement");

// RDS Station Seek
_LIT( KTagStationSeekByPTY, "StationSeekByPTY");
_LIT( KTagStationSeekByTA, "StationSeekByTA");
_LIT( KTagStationSeekByTP, "StationSeekByTP");

// RdsProgrammeType
_LIT( KTagRdsPtyNone, "RdsPtyNone");
_LIT( KTagRdsPtyNews, "RdsPtyNews");
_LIT( KTagRdsPtyCurrentAffairs, "RdsPtyCurrentAffairs");
_LIT( KTagRdsPtyInformation, "RdsPtyInformation");
_LIT( KTagRdsPtySport, "RdsPtySport");
_LIT( KTagRdsPtyEducation, "RdsPtyEducation");
_LIT( KTagRdsPtyDrama, "RdsPtyDrama");
_LIT( KTagRdsPtyCulture, "RdsPtyCulture");
_LIT( KTagRdsPtyScience, "RdsPtyScience");
_LIT( KTagRdsPtyVariedSpeech, "RdsPtyVariedSpeech");
_LIT( KTagRdsPtyPopMusic, "RdsPtyPopMusic");
_LIT( KTagRdsPtyRockMusic, "RdsPtyRockMusic");
_LIT( KTagRdsPtyEasyListening, "RdsPtyEasyListening");
_LIT( KTagRdsPtyLightClassical, "RdsPtyLightClassical");
_LIT( KTagRdsPtySeriousClassical, "RdsPtySeriousClassical");
_LIT( KTagRdsPtyOtherMusic, "RdsPtyOtherMusic");
_LIT( KTagRdsPtyWeather, "RdsPtyWeather");
_LIT( KTagRdsPtyFinance, "RdsPtyFinance");
_LIT( KTagRdsPtyChildrensProgrammes, "RdsPtyChildrensProgrammes");
_LIT( KTagRdsPtySocialAffairs, "RdsPtySocialAffairs");
_LIT( KTagRdsPtyReligion, "RdsPtyReligion");
_LIT( KTagRdsPtyPhoneIn, "RdsPtyPhoneIn");
_LIT( KTagRdsPtyTravel, "RdsPtyTravel");
_LIT( KTagRdsPtyLeisure, "RdsPtyLeisure");
_LIT( KTagRdsPtyJazzMusic, "RdsPtyJazzMusic");
_LIT( KTagRdsPtyCountryMusic, "RdsPtyCountryMusic");
_LIT( KTagRdsPtyNationalMusic, "RdsPtyNationalMusic");
_LIT( KTagRdsPtyOldiesMusic, "RdsPtyOldiesMusic");
_LIT( KTagRdsPtyFolkMusic, "RdsPtyFolkMusic");
_LIT( KTagRdsPtyDocumentary, "RdsPtyDocumentary");
_LIT( KTagRdsPtyAlarmTest, "RdsPtyAlarmTest");
_LIT( KTagRdsPtyAlarm, "RdsPtyAlarm");

_LIT( KTagRbdsPtyNone, "RbdsPtyNone");
_LIT( KTagRbdsPtyNews, "RbdsPtyNews");
_LIT( KTagRbdsPtyInformation, "RbdsPtyInformation");
_LIT( KTagRbdsPtySports, "RbdsPtySports");
_LIT( KTagRbdsPtyTalk, "RbdsPtyTalk");
_LIT( KTagRbdsPtyRock, "RbdsPtyRock");
_LIT( KTagRbdsPtyClassicRock, "RbdsPtyClassicRock");
_LIT( KTagRbdsPtyAdultHits, "RbdsPtyAdultHits");
_LIT( KTagRbdsPtySoftRock, "RbdsPtySoftRock");
_LIT( KTagRbdsPtyTop40, "RbdsPtyTop40");
_LIT( KTagRbdsPtyCountry, "RbdsPtyCountry");
_LIT( KTagRbdsPtyOldies, "RbdsPtyOldies");
_LIT( KTagRbdsPtySoft, "RbdsPtySoft");
_LIT( KTagRbdsPtyNostalgia, "RbdsPtyNostalgia");
_LIT( KTagRbdsPtyJazz, "RbdsPtyJazz");
_LIT( KTagRbdsPtyClassical, "RbdsPtyClassical");
_LIT( KTagRbdsPtyRhythmAndBlues, "RbdsPtyRhythmAndBlues");
_LIT( KTagRbdsPtySoftRhythmAndBlues, "RbdsPtySoftRhythmAndBlues");
_LIT( KTagRbdsPtyLanguage, "RbdsPtyLanguage");
_LIT( KTagRbdsPtyReligiousMusic, "RbdsPtyReligiousMusic");
_LIT( KTagRbdsPtyReligiousTalk, "RbdsPtyReligiousTalk");
_LIT( KTagRbdsPtyPersonality, "RbdsPtyPersonality");
_LIT( KTagRbdsPtyPublic, "RbdsPtyPublic");
_LIT( KTagRbdsPtyCollege, "RbdsPtyCollege");
_LIT( KTagRbdsPtyUnassigned1, "RbdsPtyUnassigned1");
_LIT( KTagRbdsPtyUnassigned2, "RbdsPtyUnassigned2");
_LIT( KTagRbdsPtyUnassigned3, "RbdsPtyUnassigned3");
_LIT( KTagRbdsPtyUnassigned4, "RbdsPtyUnassigned4");
_LIT( KTagRbdsPtyUnassigned5, "RbdsPtyUnassigned5");
_LIT( KTagRbdsPtyWeather, "RbdsPtyWeather");
_LIT( KTagRbdsPtyEmergencyTest, "RbdsPtyEmergencyTest");
_LIT( KTagRbdsPtyEmergency, "RbdsPtyEmergency");



// MACROS
//#define ?macro ?macro_def
// Logging path
#ifdef __WINSCW__
_LIT( KRadioUtilityTestClassLogPath, "\\testing\\log\\" ); 
#else
_LIT( KRadioUtilityTestClassLogPath, "e:\\testing\\log\\" );
#endif

// Log file
_LIT( KRadioUtilityTestClassLogFile, "RadioUtilityTestClass.txt" ); 


// FORWARD DECLARATIONS
class CRadioUtilityTestClass;


// CLASS DECLARATION

/**
*  CRadioUtilityTestClass test class for STIF Test Framework TestScripter.
*
*  @lib RadioUtilityTestClass.lib
*  @since Series60_3_2
*/
NONSHARABLE_CLASS(CRadioUtilityTestClass) : public CScriptBase, 
											public MTimeoutObserver, 
											public MRadioMonitorObserver,
											public MRadioFmTunerObserver,
											public MRadioPlayerObserver,
											public MRadioRdsObserver,
                                            public MRadioPresetObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CRadioUtilityTestClass* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CRadioUtilityTestClass();

		enum TRadioExpectedEvent
		{
			ERequestTunerControlComplete, 
			EStationSeekComplete, 
			ESetFrequencyRangeComplete, 
			ETuneComplete,
			EMrpoStateChange, 
			EFrequencyChanged,
			EMrftoOfflineModeStatusChange,
			EMrpoBalanceChange,
			EMrpoVolumeChange,
			EMrpoMuteChange,
			EMrftoSquelchChange,
			EMrftoForcedMonoChange,
			EMrroStationSeekByPTYComplete,
			EMrroStationSeekByTAComplete,
			EMrroStationSeekByTPComplete,
		};

    public: // New functions

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since 3.2
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

	    // From MTimeoutObserver
	    /**
	    * @since 3.2
	    * @param none
	    * Review if all the expected events have ocurred once the time is over
	    */
	    void HandleTimeout(TInt error);

	     // From MRadioMonitorObserver
	    /**
	     * Radio monitor event.
	     * @since S60 3.2
	     * @aEvent a radio event
	     */
	     void MrmEvent( TRadioMonitorEvent aEvent );

	     // From MRadioFmTunerObserver
	    /**
	     * Request for tuner control complete event. This event is asynchronous and is received after
	     * a call to CRadioFmTunerUtility::RequestTunerControl.
	     * @since S60 3.2
	     * @param aError A standard system error code or FM tuner error (TFmRadioTunerError).
	     */
	    void MrftoRequestTunerControlComplete( TInt aError );

	    /**
	     * Set frequency range complete event. This event is asynchronous and is received after
	     * a call to CRadioFmTunerUtility::SetFrequencyRange.
	     * @since S60 3.2
	     * @param aError A standard system error code or FM tuner error (TFmRadioTunerError).
	     */
	    void MrftoSetFrequencyRangeComplete( TInt aError );

	    /**
	     * Set frequency complete event. This event is asynchronous and is received after a call to
	     * CRadioFmTunerUtility::SetFrequency.
	     * @since S60 3.2
	     * @param aError A standard system error code or FM tuner error (TFmRadioTunerError).
	     */
	    void MrftoSetFrequencyComplete( TInt aError );

	    /**
	     * Station seek complete event. This event is asynchronous and is received after a call to
	     * CRadioFmTunerUtility::StationSeek.
	     * @since S60 3.2
	     * @param aError A standard system error code or FM tuner error (TFmRadioTunerError).
	     * @param aFrequency The frequency(Hz) of the radio station that was found.
	     */
	    void MrftoStationSeekComplete( TInt aError, TInt aFrequency );

	    /**
	     * Called when FM Transmitter status changes (if one is present in the device). Tuner receiver
	     * is forced to be turned off due to hardware conflicts when FM transmitter is activated.
	     * @since S60 3.2
	     * @param aActive ETrue if FM transmitter is active; EFalse otherwise.
	     */
	    void MrftoFmTransmitterStatusChange( TBool aActive );

	    /**
	     * Called when antenna status changes.
	     * @since S60 3.2
	     * @param aAttached ETrue if antenna is attached; EFalse otherwise.
	     */
	    void MrftoAntennaStatusChange( TBool aAttached );

	    /**
	     * Called when offline mode status changes.
	     * @since S60 3.2
	     ** @param aAttached ETrue if offline mode is enabled; EFalse otherwise.
	     */
	    void MrftoOfflineModeStatusChange( TBool aOfflineMode );

	    /**
	     * Called when the frequency range changes. This may be caused by other applications.
	     * @since S60 3.2
	     * @param aNewRange New frequency range.
	     */
		// temp solution
	    void MrftoFrequencyRangeChange( TFmRadioFrequencyRange aNewRange );
		// old solution
		//void MrftoFrequencyRangeChange(TFMRadioFrequencyRange aBand, TInt aMinFreq, TInt aMaxFreq);

	    /**
	     * Called when the tuned frequency changes. This may be caused by other
	     * applications or RDS if AF/TA is enabled.
	     * @since S60 3.2
	     * @param aNewFrequency The new tuned frequency(Hz).
	     */
	    void MrftoFrequencyChange( TInt aNewFrequency );

	    /**
	     * Called when the forced mono status change. This may be caused by other applications.
	     * @since S60 3.2
	     * @param aForcedMono ETrue if forced mono mode is enabled; EFalse otherwise.
	     */
	    void MrftoForcedMonoChange( TBool aForcedMono );

	    /**
	     * Called when the squelch (muting the frequencies without broadcast) status change.
	     * This may be caused by other applications.
	     * @since S60 3.2
	     * @param aSquelch ETrue if squelch is enabled; EFalse otherwise.
	     */
	    void MrftoSquelchChange( TBool aSquelch );

		// From MRadioPlayerObserver

		/* This event is asynchronous and is received after a call to
	 	 * CRadioPlayerUtility::Play and CRadioPlayerUtility::Stop. This event is also generated when
	 	 * the radio is disrupted due to an error
	     * @since S60 3.2
	     * @param aState Radio player state
	     * @param aError A standard system error code, only used when aState is ERadioPlayerIdle
	     */
		//temp solution
	    void MrpoStateChange( TPlayerState aState, TInt aError );
		//old solution
		 //void MrpoStateChange( TRsPlayerState aState, TInt aError );

	    /**
	     * Called when volume changes. This may be caused by other applications.
	     * @since S60 3.2
	     * @param aVolume Current volume.
	     */
	    void MrpoVolumeChange( TInt aVolume );

	    /**
	     * Called when mute setting changes. This may be caused by other applications.
	     * @since S60 3.2
	     * @param aMute ETrue indicates audio is muted.
	     */
	    void MrpoMuteChange( TBool aMute );

	    /**
	     * Called when balance setting changes. This may be caused by other applications.
	     * @since S60 3.2
	     *        Left speaker volume percentage. This can be any value from zero to 100.
	     *        Zero value means left speaker is muted.
	     * @param aRightPercentage
	     *        Right speaker volume percentage. This can be any value from zero to 100.
	     *        Zero value means right speaker is muted.
	     */
	    void MrpoBalanceChange( TInt aLeftPercentage, TInt aRightPercentage );

		// From MRadioRdsObserver
	    /**
	     * Station seek by Programme Type(PTY) complete event. This event is asynchronous and
	     * is received after a call to CRadioRdsUtility::StationSeekByPTY.
	     * @since S60 3.2
	     * @param aError A standard system error code or RDS control error (TRdsControlError).
	     * @param aFrequency The frequency(Hz) of the radio station that was found.
	     */
	    void MrroStationSeekByPTYComplete( TInt aError, TInt aFrequency );

	    /**
	     * Station seek by Traffic Announcement(TA) complete event. This event is asynchronous
	     * and is received after a call to CRadioRdsUtility::StationSeekByTA.
	     *
	     * @since S60 3.2
	     * @param aError A standard system error code or RDS control error (TRdsControlError).
	     * @param aFrequency The frequency(Hz) of the radio station that was found.
	     */
	    virtual void MrroStationSeekByTAComplete( TInt aError, TInt aFrequency );

	    /**
	     * Station seek by Traffic Programme(TP) complete event. This event is asynchronous and
	     * is received after a call to CRadioRdsUtility::StationSeekByTP.
	     *
	     * @since S60 3.2
	     * @param aError A standard system error code or RDS control error (TRdsControlError).
	     * @param aFrequency The frequency(Hz) of the radio station that was found.
	     */
	    virtual void MrroStationSeekByTPComplete( TInt aError, TInt aFrequency );

	    /**
	     * Get frequencies by Programme Type(PTY) complete event. This event is asynchronous
	     * and is received after a call to CRadioRdsUtility::GetFreqByPTY.
	     *
	     * Client takes ownership of the array and therefore is responsible for deallocating it.
	     *
	     * @since S60 3.2
	     * @param aError A standard system error code or RDS control error (TRdsControlError).
	     * @param aFreqList Array of frequencies (Hz), valid only if aError is KErrNone.
	     */
	    virtual void MrroGetFreqByPTYComplete( TInt aError, RArray<TInt>& aFreqList );

	    /**
	     * Get frequencies by Traffic Announcement(TA) complete event. This event is asynchronous
	     * and is received after a call to CRadioRdsUtility::GetFreqByTA.
	     * Client takes ownership of the array and therefore is responsible for deallocating it.
	     * @since S60 3.2
	     * @param aError A standard system error code or RDS control error (TRdsControlError).
	     * @param aFreqList Array of frequencies (Hz), valid only if aError is KErrNone.
	     */
	    void MrroGetFreqByTAComplete( TInt aError, RArray<TInt>& aFreqList );

	    /**
	     * Get Programme Service names (PS) by Programme Type(PTY) complete event. This event is
	     * asynchronous and is received after a call to CRadioRdsUtility::GetPSByPTY.
	     * Client takes ownership of the array and therefore is responsible for deallocating it.
	     * @since S60 3.2
	     * @param aError A standard system error code or RDS control error (TRdsControlError).
	     * @param aPsList Array of programme service names, valid only if aError is KErrNone.
	     */
	    void MrroGetPSByPTYComplete( TInt aError, RArray<TRdsPSName>& aPsList );

	    /**
	     * Get Programme Service names (PS) by Traffic Announcement(TA) complete event. This event is
	     * asynchronous and is received after a call to CRadioRdsUtility::GetPSByTA.
	     * Client takes ownership of the array and therefore is responsible for deallocating it.
	     * @since S60 3.2
	     * @param aError A standard system error code or RDS control error (TRdsControlError).
	     * @param aPsList Array of programme service names, valid only if aError is KErrNone.
	     */
	    void MrroGetPSByTAComplete( TInt aError, RArray<TRdsPSName>& aPsList );

	    /**
	     * New Programme Identification(PI) is available. This event is asynchronous and is
	     * received after a call to CRadioRdsUtility::NotifyRdsDataChange. Client will continue to
	     * receive this notification whenever new PI is available until clients unsubscribes.
	     * @since S60 3.2
	     * @param aPi Programme identification
	     */
	    void MrroRdsDataPI( TInt aPi );

	    /**
	     * New Programme Type(PTY) is available. This event is asynchronous and is received after
	     * a call to CRadioRdsUtility::NotifyRdsDataChange. Client will continue to receive this
	     * notification whenever new PTY is available until clients unsubscribes.
	     * @since S60 3.2
	     * @param aPty Programme type
	     */
	    void MrroRdsDataPTY( TRdsProgrammeType aPty );

	    /**
	     * New Programme Service(PS) is available. This event is asynchronous and is received after
	     * a call to CRadioRdsUtility::NotifyRdsDataChange. Client will continue to receive this
	     * notification whenever new PS is available until clients unsubscribes.
	     * @since S60 3.2
	     * @param aPs Programme service
	     */
	    void MrroRdsDataPS( TRdsPSName& aPs );

	    /**
	     * New Radio Text(RT) is available. This event is asynchronous and is received after
	     * a call to CRadioRdsUtility::NotifyRdsDataChange. Client will continue to receive this
	     * notification whenever new RT is available until clients unsubscribes.
	     * @since S60 3.2
	     * @param aRt Radio text
	     */
	    void MrroRdsDataRT( TRdsRadioText& aRt );

	    /**
	     * Clock Time(CT) is available. This event is asynchronous and is received after a call
	     * to CRadioRdsUtility::NotifyRdsDataChange. Client will continue to receive this notification
	     * whenever new CT is available until clients unsubscribes.
	     * @since S60 3.2
	     * @param aCt Clock time
	     */
	    void MrroRdsDataCT( TDateTime& aCt );

	    /**
	     * Called when Traffice Announcement(TA) status changes. This event is asynchronous
	     * and is received after a call to CRadioRdsUtility::NotifyRdsDataChange. Client will continue
	     * to receive this notification whenever TA status changes until clients unsubscribes.
	     * @since S60 3.2
	     * @param aTaOn ETrue indicates that Traffic Announcement is on.
	     */
	    void MrroRdsDataTA( TBool aTaOn );

	    /**
	     * New Radio Text+(RT+) is available. This event is asynchronous and is received after
	     * a call to CRadioRdsUtility::NotifyRadioTextPlusChange. Client will continue to
	     * receive this notification whenever new RT+ is available until clients unsubscribes.
	     * @since S60 3.2
	     * @param aRtPlusClass Radio text plus class
	     * @param aRtPlusData Radio text plus data
	     */
	    void MrroRdsDataRTplus( TRdsRTplusClass aRtPlusClass, TRdsRadioText& aRtPlusData );
		//void MrroRdsDataRTPlus( TRdsRTplusClass aRtPlusClass, TRdsRadioText& aRtPlusData );
		
	    /**
	     * Called when tuner starts to search for an Alternate Frequency(AF) with stronger signal.
	     * This event is asynchronous and is received after a call to
	     * CRadioRdsUtility::SetAutomaticSwitching. Client will continue to receive this notification
	     * whenever new search is started until clients turns automatic switching off.
	     * @since S60 3.2
	     */
	    void MrroRdsSearchBeginAF();

	    /**
	     * Called when search for an Alternate Frequency(AF) ends. This event is asynchronous and
	     * is received after a call to CRadioRdsUtility::SetAutomaticSwitching. Client will continue
	     * to receive this notification until clients turns automatic switching off.
	     * Most common error codes:
	     * KErrNone - Successful, new frequency found
	     * KErrNotFound - Ended, but frequency hasn't changed
	     * KErrCancel - Cancelled
	     *
	     * @since S60 3.2
	     * @param aError A standard system error code or RDS control error (TRdsControlError).
	     * @param aFrequency The frequency(Hz) of the radio station that was found.
	     */
	    void MrroRdsSearchEndAF( TInt aError, TInt aFrequency );

		/**
		 * Called when the tuner has re-tuned to another frequency that is broadcasting
		 * Traffic Announcement(TA). This notification sent only when the client has turned the
		 * automatic traffic announcement on.
		 * @since S60 3.2
	     * @param aFrequency The frequency(Hz) of the radio station that was found.
		 */
		void MrroRdsStationChangeTA( TInt aFrequency );

	    /**
	     * Called when automatic switching (AF) setting changes.
	     * @since S60 3.2
	     * @param aAuto ETrue indicates that automatic switching is on.
	     */
	    void MrroRdsEventAutomaticSwitchingChange( TBool aAuto );

	    /**
	     * Called when automatic traffic announcement setting changes.
	     * @since S60 3.2
	     * @param aAuto ETrue indicates that automatic traffic announcement is on.
	     */
	    void MrroRdsEventAutomaticTrafficAnnouncement( TBool aAuto );

	    /**
	     * Called when RDS signal status change (i.e. signal is lost/restored).
	     * @since S60 3.2
	     * @param aSignal ETrue indicates that RDS signal is available in the tuned frequency.
	     */
	    void MrroRdsEventSignalChange( TBool aSignal );

        // from MRadioPresetObserver
        /**
         * Called when a preset changes.
         * @since S60 3.2
         * @param aChange Change event type
         * @param aIndex Index to the preset that has changed. Zero means all presets.
         */
        virtual void MrpeoPresetChanged( TPresetChangeEvent aChange, TInt aIndex );
        
    protected:  // New functions

    protected:  // Functions from base classes

    private:

        /**
        * C++ default constructor.
        */
        CRadioUtilityTestClass( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // Prohibit assigment operator if not deriving from CBase.

        /**
        * Frees all resources allocated from test methods.
        * @since S60 3.2
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */

        /**
        * Example test method.
        * @since S60 3.2
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */

	    /**
	    * Set an event as expected and set default timeout
	    * @since S60 3.2
	    */
	    void AddExpectedEvent(TRadioExpectedEvent event, TInt ms);


	    /**
	    * Unset an event as expected
	    * @since S60 3.2
	    */
	    TBool RemoveExpectedEvent(TRadioExpectedEvent event);


	    /**
	    * @since S60 3.2
	    * @param none
	    * Removes all expected events
	    */
	    void RemoveAllExpectedEvents();

	    /**
	    * Verify that the event was expected, removes it from the list
	    * Signal the TestScripter with the returned error code
	    * @since S60 3.2
	    */
	    void ProcessEvent(TRadioExpectedEvent aEvent, TInt aError);

	    /**
	    * Maps a event with a descriptor with its name
	    * @since S60 3.2
	    */
	    TPtrC EventName( TInt aKey );

	    /*
	    * Test methods are listed below.
	    */

	    /**
	    * Sets a timeout different since the default
	    * @since S60 3.2
	    * @param aItem Script line containing parameters.
	    * @return Symbian OS error code.
	    */
	    TInt SetTimeout( CStifItemParser& aItem );

	    /**
	    *
	    * @since S60 3.2
	    * @param aItem Script line containing parameters.
	    * @return Symbian OS error code.
	    */
	    TInt SetExpectedEvents( CStifItemParser& aItem );

	    /**
	    *
	    * @since S60 3.2
	    * @param aItem Script line containing parameters.
	    * @return Symbian OS error code.
	    */
	    TInt SetAllowedPanic( CStifItemParser& aItem );
	    
	    /**
	    * Connect to the default client to the server
	    * @since S60 3.2
	    * @param aItem Script line containing parameters.
	    * @return Symbian OS error code.
	    */

		TInt StartMonitor( CStifItemParser& aItem );
		TInt StopMonitor( CStifItemParser& aItem );
        TInt IsRadioOn( CStifItemParser& aItem );
		TInt CreateFactory( CStifItemParser& aItem );
		TInt CreateUtility( CStifItemParser& aItem );
		TInt RequestTunerControl( CStifItemParser& aItem );
		TInt SetFrequencyRange( CStifItemParser& aItem );
		TInt SetFrequency( CStifItemParser& aItem );
		TInt Close( CStifItemParser& aItem );
		TInt StationSeek( CStifItemParser& aItem );
		TInt Cancel( CStifItemParser& aItem );
		TInt OfflineMode( CStifItemParser& aItem );

		TInt Play( CStifItemParser& aItem );
		TInt Stop( CStifItemParser& aItem );
		
		
		TInt VerifyCapabilities( CStifItemParser& aItem );
		TInt VerifyMaxValues( CStifItemParser& aItem );
		TInt Check( CStifItemParser& aItem );
		TInt CheckBalance( CStifItemParser& aItem );
		TInt SetBalance( CStifItemParser& aItem );
		TInt ChangeBalance( CStifItemParser& aItem );
		TInt Get(CStifItemParser& aItem );
		TInt Set( CStifItemParser& aItem );
		TInt ChangeVolume( CStifItemParser& aItem );
		
		TInt GetRdsCapabilities( CStifItemParser& aItem );
		TInt GetRdsSignalStatus( CStifItemParser& aItem );
		TInt NotifyRdsDataChange( CStifItemParser& aItem );
		TInt CancelNotifyRdsDataChange( CStifItemParser& aItem );
		TInt CancelAFSearch( CStifItemParser& aItem );
		TInt StationSeekByPTY( CStifItemParser& aItem );
		TInt StationSeekByTA( CStifItemParser& aItem );
		TInt StationSeekByTP( CStifItemParser& aItem );
		TInt GetProgrammeIdentification( CStifItemParser& aItem );
		TInt GetProgrammeType( CStifItemParser& aItem );
		TInt GetProgrammeService( CStifItemParser& aItem );
		TInt GetRadioText( CStifItemParser& aItem );
		TInt GetClockTime( CStifItemParser& aItem );
		TInt GetTrafficAnnouncementStatus( CStifItemParser& aItem );
		TInt GetTrafficProgrammeStatus( CStifItemParser& aItem );

		// FM Preset
        TInt SetPreset( CStifItemParser& aItem );
        TInt GetPreset( CStifItemParser& aItem );
        TInt GetMaxNumberOfPresets( CStifItemParser& aItem );
        TInt GetNumberOfPresets( CStifItemParser& aItem );
        TInt GetFirstPresetL( CStifItemParser& aItem );
        TInt GetNextPresetL( CStifItemParser& aItem );
        TInt DeletePreset( CStifItemParser& aItem );
        // New Preset info
        TInt SetUrlToPreset( CStifItemParser& aItem );
        TInt GetUrlFromPreset( CStifItemParser& aItem );
        TInt SetFavouriteInfoToPreset( CStifItemParser& aItem );
        TInt GetFavouriteInfoFromPreset( CStifItemParser& aItem );
        TInt SetPtyToPreset( CStifItemParser& aItem );
        TInt GetPtyFromPreset( CStifItemParser& aItem );
        TInt SetPIToPreset( CStifItemParser& aItem );
        TInt GetPIFromPreset( CStifItemParser& aItem );
        
	//	TInt AudioEnhancement( CStifItemParser& aItem );


    public:     // Data

    protected:  // Data

    private:    // Data
        
    // reference to TestModuleIf
    CTestModuleIf& iTestModuleIf;
    CRadioMonitor *iRadioMonitor;
    CRadioUtility *iRadioUtility;
    CRadioFmTunerUtility* iRadioFMTunerUtility;
    CRadioPlayerUtility* iRadioPlayerUtility;
    CRadioRdsUtility* iRadioRdsUtility;
    CRadioFmPresetUtility* iFmPresetUtility;
    
    // Active object with a timer to timeout the test case
    CSimpleTimeout * iTimeoutController;

    // Indicates if the test case use a normal exit reason
    TBool iNormalExitReason;

    // List of expected events
    RArray<TRadioExpectedEvent> iExpectedEvents;

    // List of notification event (by the callback)
    RArray<TRadioExpectedEvent> iOcurredEvents;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // RADIOUTILITYTESTCLASS_H

// End of File
