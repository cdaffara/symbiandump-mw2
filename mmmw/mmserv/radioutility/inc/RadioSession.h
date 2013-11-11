/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class is the main interface to the RadioServer. It implements
*				 the client-side session. It also contains M-observer classes that
*				 the client must implement to receive completion notification for
*				 asynchronous requests and spontaneous event notifications.
*
*/



#ifndef RADIOSESSION_H
#define RADIOSESSION_H

//  INCLUDES
#include <mmf/common/mmfbase.h>
#include <mmf/common/mmfcontrollerframework.h>
#include <MCustomCommand.h>

#include "RadioServerData.h"

// FORWARD DECLARATIONS
class CRadioRequest;
class CRadioEventHandler;

// CLASS DECLARATION

/**
*  Defines functions that client must implement in order to receive
*  events from the radio server.
*
*  @lib RadioSession.lib
*  @since S60 3.0
*/
class MRadioObserver
    {
public: // New functions

//********** Tuner related

	/**
     * Completion message for RequestTunerControl request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     */
	virtual void RequestTunerControlComplete( TRadioServerError aError ) = 0;

	/**
     * Completion message for SetFrequencyRange request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     */
	virtual void SetFrequencyRangeComplete( TRadioServerError aError ) = 0;

	/**
     * Completion message for SetFrequency request.
     *
     * @since S60 3.0
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     */
	virtual void SetFrequencyComplete( TRadioServerError aError ) = 0;

	/**
     * Completion message for StationSeek request.
     *
     * @since S60 3.0
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aFrequency Valid only if aError is KErrNone. Contains the new frequency in Hz.
     */
	virtual void StationSeekComplete( TRadioServerError aError, TInt aFrequency ) = 0;

	/**
     * Event notification indicating FM transmitter status change. Radio receiver
     * is turned off when FM transmitter is active.
     *
     * @since S60 3.2
     * @param aActive ETrue if FM transmitter is active; EFalse otherwise.
     */
	virtual void RadioEventTransmitterStatusChange( TBool aActive ) = 0;

	/**
     * Event notification indicating antenna status change.
     *
     * @since S60 3.0
     * @param aAttached ETrue if antenna is attached.
     */
	virtual void RadioEventAntennaStatusChange( TBool aAttached ) = 0;

	/**
     * Event notification indicating offline mode change.
     *
     * @since S60 3.0
     * @param aOfflineMode ETrue if device is in offline mode.
     */
	virtual void RadioEventOfflineModeChange( TBool aOfflineMode ) = 0;

	/**
     * Event notification indicating frequency range change. This may be caused by
     * other applications.
     *
     * @since S60 3.2
     * @param aNewRange New frequency range.
     */
	virtual void RadioEventFrequencyRangeChanged( TRsFrequencyRange aNewRange ) = 0;

	/**
     * Event notification indicating frequency(Hz) change. This may be caused by
     * other applications or RDS if AF/TA is enabled.
     *
     * @since S60 3.2
     * @param aFrequency New frequency where tuner is currently tuned.
     */
	virtual void RadioEventFrequencyChange( TInt aFrequency ) = 0;

	/**
     * Event notification indicating forced mono status change.
     *
     * @since S60 3.2
     * @param aForcedMono ETrue if forced mode is enabled; EFalse otherwise.
     */
	virtual void RadioEventForcedMonoChanged( TBool aForcedMono ) = 0;

	/**
     * Event notification indicating squelch (muting the frequencies without broadcast) status change.
     *
     * @since S60 3.2
     * @param aSquelch ETrue if squelch is enabled; EFalse otherwise.
     */
	virtual void RadioEventSquelchChanged( TBool aSquelch ) = 0;

//********** Player related

	/**
     * Event notification indicating radio player state change. This may be caused by
     * other applications.
     *
     * @since S60 3.0
     * @param aRadioOn ETrue if radio is playing, otherwise radio is off.
     * @param aError Valid only if aRadioOn is EFalse. Contains the reason why radio is off.
     */
	virtual void RadioEventStateChange( TBool aRadioOn, TRadioServerError aError ) = 0;

	/**
     * Event notification indicating volume change.
     *
     * @since S60 3.2
     * @param aVolume New volume.
     */
	virtual void RadioEventVolumeChange( TInt aVolume ) = 0;

	/**
     * Event notification indicating mute setting change.
     *
     * @since S60 3.2
     * @param aMute ETrue indicates audio is muted.
     */
	virtual void RadioEventMuteChange( TBool aMute ) = 0;

	/**
     * Event notification indicating balance setting change.
     *
     * @since S60 3.2
     * @param aLeftPercentage Left speaker volume percentage. This value ranges from 0 to 100.
     * @param aRightPercentage Right speaker volume percentage. This value ranges from 0 to 100.
     */
	virtual void RadioEventBalanceChange( TInt aLeftPercentage, TInt aRightPercentage ) = 0;

//********** RDS related

	/**
     * Completion message for StationSeekByPTY request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aFrequency The frequency(Hz) of the radio station that was found.
     */
	virtual void StationSeekByPTYComplete( TRadioServerError aError, TInt aFrequency ) = 0;

	/**
     * Completion message for StationSeekByTA request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aFrequency The frequency(Hz) of the radio station that was found.
     */
	virtual void StationSeekByTAComplete( TRadioServerError aError, TInt aFrequency ) = 0;

	/**
     * Completion message for StationSeekByTP request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aFrequency The frequency(Hz) of the radio station that was found.
     */
	virtual void StationSeekByTPComplete( TRadioServerError aError, TInt aFrequency ) = 0;

	/**
     * Completion message for GetFreqByPTY request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aFreqList Array of frequencies (Hz), valid only if aError is KErrNone.
     */
	virtual void GetFreqByPTYComplete( TRadioServerError aError, RArray<TInt>& aFreqList ) = 0;

	/**
     * Completion message for GetFreqByTA request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aFreqList Array of frequencies (Hz), valid only if aError is KErrNone.
     */
	virtual void GetFreqByTAComplete( TRadioServerError aError, RArray<TInt>& aFreqList ) = 0;

	/**
     * Completion message for StatGetPSByPTY request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aPsList Array of programme service names, valid only if aError is KErrNone.
     */
	virtual void GetPSByPTYComplete( TRadioServerError aError, RArray<TRsRdsPSName>& aPsList ) = 0;

	/**
     * Completion message for GetPSByTA request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aPsList Array of programme service names, valid only if aError is KErrNone.
     */
	virtual void GetPSByTAComplete( TRadioServerError aError, RArray<TRsRdsPSName>& aPsList ) = 0;

	/**
     * Event notification indicating new Programme Identification(PI) is available.
     *
     * @since S60 3.2
     * @param aPi Programme identification
     */
	virtual void RadioEventRdsDataPI( TInt aPi ) = 0;

	/**
     * Event notification indicating new Programme Type(PTY) is available.
     *
     * @since S60 3.2
     * @param aPty Programme type
     */
	virtual void RadioEventRdsDataPTY( TRsRdsProgrammeType aPty ) = 0;

	/**
     * Event notification indicating new Programme Service(PS) is available.
     *
     * @since S60 3.2
     * @param aPs Programme service
     */
	virtual void RadioEventRdsDataPS( TRsRdsPSName& aPs ) = 0;

	/**
     * Event notification indicating new Radio Text(RT) is available.
     *
     * @since S60 3.2
     * @param aRt Radio text
     */
	virtual void RadioEventRdsDataRT( TRsRdsRadioText& aRt ) = 0;

	/**
     * Event notification indicating new Clock Time(CT) is available.
     *
     * @since S60 3.2
     * @param aCt Clock time
     */
	virtual void RadioEventRdsDataCT( TDateTime& aCt ) = 0;

	/**
     * Event notification indicating Traffice Announcement(TA) status change.
     *
     * @since S60 3.2
     * @param aTaOn ETrue indicates that Traffic Announcement is on.
     */
	virtual void RadioEventRdsDataTA( TBool aTaOn ) = 0;

	/**
     * Event notification indicating new Radio Text+(RT+) is available.
     *
     * @since S60 3.2
	 * @param aRtPlusClass Radio text plus class
	 * @param aRtPlusData Radio text plus data
	 */
	virtual void RadioEventRdsDataRTplus( TRsRdsRTplusClass aRtPlusClass, TRsRdsRadioText& aRtPlusData ) = 0;

	/**
     * Event notification indicating the beginning of Alternate Frequency(AF) search.
     *
     * @since S60 3.2
     */
	virtual void RadioEventRdsSearchBeginAF() = 0;

	/**
     * Event notification indicating the end of Alternate Frequency(AF) search.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aFrequency The frequency(Hz) of the radio station that was found.
     */
	virtual void RadioEventRdsSearchEndAF( TRadioServerError aError, TInt aFrequency ) = 0;

	/**
     * Event notification indicating station change to another frequency(Hz) that is
     * broadcasting Traffic Announcement(TA).
     *
     * @since S60 3.2
     * @param aFrequency The frequency(Hz) of the radio station that was found.
     */
	virtual void RadioEventRdsStationChangeTA( TInt aFrequency ) = 0;

	/**
     * Event notification indicating automatic switching (AF) setting change.
     *
     * @since S60 3.2
     * @param aAuto ETrue indicates that automatic switching is on.
     */
	virtual void RadioEventRdsAutomaticSwitchingChange( TBool aAuto ) = 0;

	/**
     * Event notification indicating automatic traffic announcement setting change.
     *
     * @since S60 3.2
     * @param aAuto ETrue indicates that automatic traffic announcement is on.
     */
	virtual void RadioEventRdsAutomaticTrafficAnnouncement( TBool aAuto ) = 0;

	/**
     * Event notification indicating RDS signal status change (i.e. signal is lost/restored).
     *
     * @since S60 3.2
     * @param aSignal ETrue indicates that RDS signal is available in the tuned frequency.
     */
	virtual void RadioEventRdsSignalChange( TBool aSignal ) = 0;
    };

/**
*  Main interface to the Radio Server.
*  Implements the client-side session.
*
*  @lib RadioSession.lib
*  @since S60 3.0
*/
class RRadioSession : public RSessionBase,
					  public MCustomCommand
    {
public: // New functions
    IMPORT_C static RRadioSession* NewL();
public:  // Constructors and destructor

	/**
     * C++ default constructor.
     */
	IMPORT_C RRadioSession();

public: // New functions

	/**
     * Connects a client to the radio server.
     * @since S60 3.0
     *
     * @param aObserver The observer object for receiving async completion callbacks.
     * @param aPrimaryClient Indicates whether the client is a primary client. Primary
     * clients are clients that can control the radio tuner such as FM Radio Application,
     * Visual Radio, or a Java Radio App.  Non-primary clients are observers of the tuner,
     * player, and RDS utilities and cannot exist without a primary client such as Active
     * Idle, Cover UI, or a smart accessory driver.
     * @return A standard system error code.
     */
	IMPORT_C TInt Connect( MRadioObserver& aObserver, TBool aPrimaryClient );

	/**
     * Gets the client side version number.
     *
     * @since S60 3.0
     * @return The client side version number.
     */
	IMPORT_C TVersion Version() const;

	/**
     * Closes connection to the radio server.
     *
     * @since S60 3.0
     */
	IMPORT_C void Close();

//********** TunerUtility control begins

	/**
     * Request for control of a tuner. If this method returns KErrNone, control of
     * the tuner has been granted. Control to the tuner must be granted before any
     * other request can be made.
     *
     * @since S60 3.2
     * @param Tuner type (e.g. FM, AM)
     * @return A standard system error code.
     * @see MRadioObserver::RequestTunerControlComplete
     */
	IMPORT_C void RequestTunerControl( TRsTuner aTuner );

	/**
     * Get the capabilities of the radio on the device.
     *
     * @since S60 3.2
     * @param aCaps The capabilities object to fill
     * @return A standard system error code.
     */
	IMPORT_C TInt GetTunerCapabilities( TRsTunerCapabilities& aCaps ) const;

	/**
     * EnableTunerInOfflineMode on the device.
     *
     * @since S60 3.2
     * @param aEnable ETrue to enable tuner functions in offline mode, EFalse to disable.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt EnableTunerInOfflineMode( TBool aEnable );

	/**
     * Asynchronous request to set the frequency range. If the frequency range is not set,
     * it will be defaulted to ERsTunerFM.
     *
     * @since S60 3.2
     * @param aRange Frequency range
     * @see MRadioObserver::SetFrequencyRangeComplete
     */
	IMPORT_C void SetFrequencyRange( TRsFrequencyRange aRange );

	/**
     * Cancels an outstanding SetFrequencyRange request. Note that SetFrequencyRange may
     * complete before cancel can occur and a callback may occur.
     *
     * @since S60 3.2
     * @return A standard system error code.
     */
	IMPORT_C void CancelSetFrequencyRange();

	/**
     * Get the current frequency range. It also returns the minimum and maximum frequencies(Hz)
     * for the returned range.
     *
     * @since S60 3.2
     * @param aRange On return contains the current frequency range.
     * @param aMinFreq On return contains the minimum frequency for the current frequency range.
     * @param aMaxFreq On return contains the maximum frequency for the current frequency range.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetFrequencyRange( TRsFrequencyRange& aRange, TInt& aMinFreq, TInt& aMaxFreq ) const;

	/**
     * Asynchronous request to tune the tuner to the specified frequency.
     *
     * @since S60 3.2
     * @param aFrequency The frequency (Hz) to tune to
     * @see MRadioObserver::SetFrequecyComplete
     */
	IMPORT_C void SetFrequency( TInt aFrequency );

	/**
     * Cancels an outstanding SetFrequency request. Note that SetFrequency may complete before
     * cancel can occur and a callback to MRadioObserver::SetFrequencyComplete may occur.
     *
     * @since S60 3.0
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C void CancelSetFrequency();

	/**
     * Get the current frequency.
     *
     * @since S60 3.2
     * @param aFrequency On return contains the current frequency(Hz).
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetFrequency( TInt& aFrequency ) const;

	/**
     * Asynchronous request to find a radio station, starting from current frequency and
     * seaching in the direction specified (i.e. up or down).
     *
     * @since S60 3.0
     * @param aSeekUp Search direction
     * @see MRadioObserver::StationSeekComplete
     */
	IMPORT_C void StationSeek( TBool aUpwards );

	/**
     * Cancels an outstanding StationSeek request. Note that StationSeek may complete before
     * cancel can occur and a callback to MRadioObserver::StationSeekComplete may occur.
     *
     * @since S60 3.0
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C void CancelStationSeek();

	/**
     * Gets the signal strength of the currently tuned signal.
     *
     * @since S60 3.2
     * @param aStrength On return contains the current signal strength.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetSignalStrength( TInt& aSignalStrength ) const;

	/**
     * Gets the maximum possible signal strength of a tuned signal.
     *
     * @since S60 3.2
     * @param aMaxStrength On return contains the maximum signal strength.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetMaxSignalStrength( TInt& aMaxSignalStrength ) const;

	/**
     * Get the stereo mode of the radio.
     *
     * @since S60 3.2
     * @param aStereo On return, will be ETrue if signal is stereo.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetStereoMode( TBool& aStereo ) const;

	/**
     * Indicates whether the reception should be forced into monophonic mode.
     *
     * @since S60 3.2
     * @param aMono If ETrue, all reception will be in mono mode even if a stereo signal is
     *		 available. If EFalse, a stereo signal will be received when possible.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt ForceMonoReception( TBool aForcedMono );

	/**
     * Checks whether force mono reception is on or not.
     * @since S60 3.2
     * @param aForceMono ETrue if force mono is on, EFalse otherwise.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetForceMonoReception( TBool& aForcedMono ) const;

	/**
     * Enable or disable quelch.
     *
     * @since S60 3.2
     * @param aEnabled ETrue to enable squelching, EFalse to disable it.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt SetSquelch( TBool aEnabled );

	/**
	 * Retrieves the current squelching (muting in frequencies without reception) setting
     *
     * @since S60 3.2
     * @param aSquelch ETrue if a squelching is currently enabled
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetSquelch( TBool& aSquelch ) const;

//********** PlayerUtility control begins

	/**
     * Retrieve the current state of the player.
	 * If the radio is already playing, client should simply retrieve current settings such
     * as volume, etc.
     *
     * @since S60 3.2
     * @return Radio player state.
     */
	IMPORT_C TInt PlayerState( TRsPlayerState& aState ) const;

	/**
     * Starts radio playback.
     *
     * @since S60 3.0
     */
	IMPORT_C void Play();

	/**
     * Stops playback, and release the output device for use by other clients.
     *
     * @since S60 3.0
     * @param aIfOnlyPrimaryClient ETrue to stop playback only if there are no other primary clients
     */
	IMPORT_C void Stop( TBool aIfOnlyPrimaryClient = EFalse );

	/**
     * Retrieves the maximum volume supported.
     *
     * @since S60 3.0
     * @param aVolume On return contains the maximum volume.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetMaxVolume( TInt& aMaxVolume ) const;

	/**
     * Sets the volume to the specified level.
     *
     * @since S60 3.0
     * @param aVolume The volume level to set
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt SetVolume( TInt aVolume );

	/**
     * Get the current volume.
     *
     * @since S60 3.2
     * @param aVolume On return contains the current volume.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetVolume( TInt& aVolume ) const;

	/**
     * Set a volume ramp.
     *
     * @since S60 3.2
     * @param aRampInterval The time interval over which the volume should be increased from
     *		 zero to the current volume setting.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt SetVolumeRamp( const TTimeIntervalMicroSeconds& aRampInterval );

	/**
     * Mutes or unmutes playback.
     *
     * @since S60 3.0
     * @param aMute ETrue to mute the audio, EFalse to unmute it.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt Mute( TBool aMute );

	/**
     * Find out if the audio is muted or not.
     *
     * @since S60 3.2
     * @param aVolume On return set to ETrue if audio is muted.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetMuteStatus( TBool& aMute ) const;

	/**
     * Set the speaker balance for playing.
     *
     * @since S60 3.2
     * @param aLeftPercentage Left speaker volume percentage. This value ranges from 0 to 100.
     * @param aRightPercentage Right speaker volume percentage. This value ranges from 0 to 100.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt SetBalance( TInt aLeftPercentage, TInt aRightPercentage );

	/**
     * Get the current speaker balance setting.
     *
     * @since S60 3.2
     * @param aLeftPercentage On return contains the left speaker volume percentage.
     * @param aRightPercentage On return contains the right speaker volume percentage.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetBalance( TInt& aLeftPercentage, TInt& aRightPercentage ) const;

//********** RDSUtility control begins

	/**
     * Get the capabilities of the RDS control on the device.
     *
     * @since S60 3.2
     * @param aCaps The capabilities object to fill
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetRdsCapabilities( TRsRdsCapabilities& aCaps ) const;

	/**
     * Get the status of the RDS reception.
     *
     * @since S60 3.2
     * @param aRdsSignal On return, will be ETrue if RDS signal can be recepted, EFalse otherwise.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetRdsSignalStatus( TBool& aRdsSignal ) const;

	/**
     * Subscribe for notification for the specified RDS data. Client should first check
     * the capabilities to see if a feature is supported.
     * Request for notification for non-supported features will simply be ignored.
     *
     * @since S60 3.2
     * @param aRdsData Bitfield indicating notification request.
     * @return A standard system/RadioServer error code.
     * @see MRadioObserver::RadioEventRdsDataPI
     * @see MRadioObserver::RadioEventRdsDataPTY
     * @see MRadioObserver::RadioEventRdsDataPS
     * @see MRadioObserver::RadioEventRdsDataRT
     * @see MRadioObserver::RadioEventRdsDataCT
     * @see MRadioObserver::RadioEventRdsDataTA
     */
	IMPORT_C TInt NotifyRdsDataChange( TRsRdsData aRdsData );

	/**
     * Cancel NotifyRdsDataChange request.
     *
     * @since S60 3.2
     */
	IMPORT_C void CancelNotifyRdsDataChange();

    /**
     * Subscribe for notification for the specified RadioText+ data. Client should first check
     * the capabilities to see if RT+ feature is supported.
     * Returns KErrNotSupported if RT+ is not supported.
     *
     * Note that if the client wishes to receive the entire radio text data chunk, client should
     * subscribe for ERsRdsRadioText using NotifyRdsDataChange instead.
     *
     * @since S60 3.2
     * @param aRtPlusClasses Array of RT+ class to be notified
     * @return A standard system/RadioServer error code.
     * @see MRadioObserver::RadioEventRdsDataRTplus
     */
    IMPORT_C TInt NotifyRadioTextPlusChange( RArray<TInt>& aRtPlusClasses );

	/**
     * Cancel NotifyRadioTextPlusChange request.
     *
     * @since S60 3.2
     */
	IMPORT_C void CancelNotifyRadioTextPlusChange();

	/**
     * Turn on/off automatic switching of frequency based on Alternate Frequency.
     * This will cause RDS device to search for alternate frequency when the signal strength
     * deteriorates. User should be ready to receive RadioEventRdsSearchBeginAF and
     * RadioEventRdsSearchEndAF. Automatic switching is off by default.
     *
     * @since S60 3.2
     * @param aAuto ETrue to turn automatic switching on, EFalse to turn it off.
     * @return A standard system/RadioServer error code.
     * @see MRadioObserver::RadioEventRdsSearchBeginAF
	 * @see MRadioObserver::RadioEventRdsSearchEndAF
     */
	IMPORT_C TInt SetAutomaticSwitching( TBool aAuto );

	/**
     * Find out whether automatic switching is on or off.
     *
     * @since S60 3.2
     * @param aAuto On return, ETrue indicates that automatic switching is enabled.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetAutomaticSwitching( TBool& aAuto );

	/**
     * Cancel ongoing search for an Alternate Frequency (AF) with stronger signal.
     *
     * Client can issue this request to interrupt the search indicated with
     * MRadioObserver::RadioEventRdsSearchBeginAF.
     *
     * @since S60 3.2
     */
	IMPORT_C void CancelAFSearch();

	/**
     * Turns on/off automatic switching of frequency based on Traffic Announcement.
     * This will cause RDS device to search for frequencies broadcasting traffic announcement.
     * Client will be notified of frequency change though the tuner event.
     * It's up to the client to return to the previous frequency when the traffic announcement
     * is finished.
     *
     * NOTE: This is only supported in dual tuner configuration since the secondary tuner
     * needs to perform continuous scanning for frequency broadcasting traffic announcement,
     * while the primary tuner is used for normal tuner activities.
     *
     * @since S60 3.2
     * @param aAuto ETrue indicates that automatic switching is on.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt SetAutomaticTrafficAnnouncement( TBool aAuto );

	/**
     * Find out whether automatic traffic announcement is enabled.
     *
     * @since S60 3.2
     * @param aAuto On return, ETrue indicates that automatic traffic announcement is on.
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetAutomaticTrafficAnnouncement( TBool& aAuto );

	/**
     * Asynchronous request to find a radio station with the specified Programme Type(PTY),
     * starting from the currently tuned frequency and searching in the direction specified
     * (i.e. up or down). User must be ready to receive callback method StationSeekByPTYComplete
     * The station found is returned in the callback.
     *
     * @since S60 3.2
     * @param aPty The type of programme to search for.
     * @param aSeekUp The direction to search in. Searches upwards if set to ETrue.
     * @see MRadioObserver::StationSeekByPTYComplete
     */
	IMPORT_C void StationSeekByPTY( TRsRdsProgrammeType aPty, TBool aSeekUp );

	/**
     * Asynchronous request to find a radio station with Traffic Announcement(TA),
     * starting from the currently tuned frequency and searching in the direction specified
     * (i.e. up or down). User must be ready to receive callback method StationSeekByTAComplete
     * The station found is returned in the callback.
     *
     * @since S60 3.2
     * @param aSeekUp The direction to search in. Searches upwards if set to ETrue.
     * @see MRadioObserver::StationSeekByTAComplete
     */
	IMPORT_C void StationSeekByTA( TBool aSeekUp );

	/**
     * Asynchronous request to find a radio station with Traffic Programme(TP),
     * starting from the currently tuned frequency and searching in the direction specified
     * (i.e. up or down). User must be ready to receive callback method StationSeekByTPComplete
     * The station found is returned in the callback.
     *
     * @since S60 3.2
     * @param aSeekUp The direction to search in. Searches upwards if set to ETrue.
     * @see MRadioObserver::StationSeekByTPComplete
     */
	IMPORT_C void StationSeekByTP( TBool aSeekUp );

	/**
     * Cancels an ongoing retune operation, as initiated by a call to StationSeekByPTY,
     * StationSeekByTA, or StationSeekByTP.
     * The usual callback will not occur if this has been called.
     *
     * @since S60 3.2
     */
	IMPORT_C void CancelRdsStationSeek();

	/**
     * Asynchronous request to find all frequencies sending the given Programme Type (PTY).
     * User must be ready to receive callback method GetFreqByPTYComplete.
     *
     * NOTE: This is only supported in dual tuner configuration since the secondary tuner
     * needs to perform continuous scanning for frequencies broadcasting given Programme Type
     * while the primary tuner is used for normal tuner activities.
     * Client should first check the tuner capabilities. Will return KErrNotSupported in
     * callback method if this feature is not supported.
     *
     * @since S60 3.2
     * @param aPty The type of programme to search for
     * @see MRadioObserver::GetFreqByPTYComplete
     */
	IMPORT_C void GetFreqByPTY( TRsRdsProgrammeType aPty );

	/**
     * Cancels an ongoing request to find all frequencies sending a given Programme Type (PTY).
     * The usual callback will not occur if this has been called.
     *
     * @since S60 3.2
     */
	IMPORT_C void CancelGetFreqByPTY();

	/**
     * Asynchronous request to find all frequencies sending Traffic Announcement (TA). User must
     * be ready to receive callback method GetFreqByTAComplete.
     *
     * NOTE: This is only supported in dual tuner configuration since the secondary tuner
     * needs to perform continuous scanning for frequencies broadcasting given Traffic Announcement
     * while the primary tuner is used for normal tuner activities.
     * Client should first check the tuner capabilities. Will return KErrNotSupported in
     * callback method if this feature is not supported.
     *
     * @since S60 3.2
     * @see MRadioObserver::GetFreqByTAComplete
     */
	IMPORT_C void GetFreqByTA();

	/**
     * Cancels an ongoing request to find all frequencies sending Traffic Announcement.
     * The usual callback will not occur if this has been called.
     *
     * @since S60 3.2
     */
	IMPORT_C void CancelGetFreqByTA();

	/**
     * Asynchronous request to find all Programme Service names (PS) sending the given Programme
     * Type (PTY). User must be ready to receive callback method GetPSByPTYComplete.
     *
     * NOTE: This is only supported in dual tuner configuration since the secondary tuner
     * needs to perform continuous scanning for frequencies broadcasting given Programme Type
     * while the primary tuner is used for normal tuner activities.
     * Client should first check the tuner capabilities. Will return KErrNotSupported in
     * callback method if this feature is not supported.
     *
     * @since S60 3.2
     * @param aPty The type of programme to search for
     * @see MRadioObserver::GetPSByPTYComplete
     */
	IMPORT_C void GetPSByPTY( TRsRdsProgrammeType aPty );

	/**
     * Cancels an ongoing request to find all Programme Service names (PS) sending a given
     * Programme Type (PTY). The usual callback will not occur if this has been called.
     *
     * @since S60 3.2
     */
	IMPORT_C void CancelGetPSByPTY();

	/**
     * Asynchronous request to find all Programme Service names (PS) sending Traffic Announcement (TA).
     * User must be ready to receive callback method GetPSByTAComplete.
     *
     * NOTE: This is only supported in dual tuner configuration since the secondary tuner
     * needs to perform continuous scanning for frequencies broadcasting given Traffic Announcement
     * while the primary tuner is used for normal tuner activities.
     * Client should first check the tuner capabilities. Will return KErrNotSupported in
     * callback method if this feature is not supported.
     *
     * @since S60 3.2
     * @see MRadioObserver::GetPSByTAComplete
     */
	IMPORT_C void GetPSByTA();

	/**
     * Cancels an ongoing request to find all Programme Service names (PS) sending Traffic Announcement.
     * The usual callback will not occur if this has been called.
     *
     * @since S60 3.2
     */
	IMPORT_C void CancelGetPSByTA();

	/**
     * Get the current Programme Identification code.
     * RDS data is received over the air and may not be available immediately following
     * tune operation. If data is not available, this function will return KErrNotFound.
     * If a value is returned, this is the last known value, which may not be up to date.
     * To be notified of the most recent value, client should use NotifyRdsDataChange.
     *
     * @since S60 3.2
     * @param aPi On return contains Programme Identification code
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetProgrammeIdentification( TInt& aPi );

	/**
     * Get the current Programme Type.
     * RDS data is received over the air and may not be available immediately following
     * tune operation. If data is not available, this function will return KErrNotFound.
     * If a value is returned, this is the last known value, which may not be up to date.
     * To be notified of the most recent value, client should use NotifyRdsDataChange.
     *
     * @since S60 3.2
     * @param aPty On return contains Programme Type
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetProgrammeType( TRsRdsProgrammeType& aPty );

	/**
     * Get the current Programme Service name.
     * RDS data is received over the air and may not be available immediately following
     * tune operation. If data is not available, this function will return KErrNotFound.
     * If a value is returned, this is the last known value, which may not be up to date.
     * To be notified of the most recent value, client should use NotifyRdsDataChange.
     *
     * Programme Service name is fixed to 8 characters.
     *
     * @since S60 3.2
     * @param aPs On return contains Programme Service name
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetProgrammeService( TRsRdsPSName& aPs );

	/**
     * Get the current Radio Text.
     * RDS data is received over the air and may not be available immediately following
     * tune operation. If data is not available, this function will return KErrNotFound.
     * If a value is returned, this is the last known value, which may not be up to date.
     * To be notified of the most recent value, client should use NotifyRdsDataChange.
     *
     * The maximum possible length for radio text field is 64 characters.
     *
     * @since S60 3.2
     * @param aRt On return contains Radio Text
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetRadioText( TRsRdsRadioText& aRt );

    /**
     * Get the current Radio Text+.
     * RDS data is received over the air and may not be available immediately following
     * tune operation. If data is not available, this function will return KErrNotFound.
     * If a value is returned, this is the last known value, which may not be up to date.
     * To be notified of the most recent value, client should use NotifyRdsDataChange.
     *
     * The maximum possible length for radio text+ field is 64 characters.
     *
     * @since S60 3.2
     * @param aRtPlusClass Radio text plus class
     * @param aRtPlusData On return contains Radio Text+ field
     * @return A standard system/RadioServer error code.
     */
    IMPORT_C TInt GetRadioTextPlus( TRsRdsRTplusClass aRtPlusClass, TRsRdsRadioText& aRtPlusData );

	/**
     * Get the current Clock Time and date.
     * RDS data is received over the air and may not be available immediately following
     * tune operation. If data is not available, this function will return KErrNotFound.
     * If a value is returned, this is the last known value, which may not be up to date.
     * To be notified of the most recent value, client should use NotifyRdsDataChange.
     *
     * @since S60 3.2
     * @param aCt On return contains current time and date
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetClockTime( TDateTime& aCt );

	/**
     * Get Traffic Announcement status at the current station.
     *
     * @since S60 3.2
     * @param aTaStatus On return, will be ETrue if current station has ongoing traffic announcement
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetTrafficAnnouncementStatus( TBool& aTaStatus );

	/**
     * Get Traffic Programme status at the current station.
     *
     * @since S60 3.2
     * @param aTpStatus On return, will be ETrue if current station supports traffic programme
     * @return A standard system/RadioServer error code.
     */
	IMPORT_C TInt GetTrafficProgrammeStatus( TBool& aTpStatus );

// from base class MCustomCommand

    /**
     * From MCustomCommand
     * Sends a synchronous custom command to the radio server.
     *
     * @since S60 3.0
     * @param aDestination The destination of the message, consisting of the uid of
     * 		 the interface of this message
     * @param aFunction The function number to indicate which function is to be called
     *		 on the interface defined in the aDestination parameter
     * @param aDataTo1 The first chunk of data to be copied to the controller
     *		 framework. The exact contents of the data are dependent on the
     *		 interface being called. Can be KNullDesC8.
     * @param aDataTo2 The second chunk of data to be copied to the controller
     *		 framework. The exact contents of the data are dependent on the
     *		 interface being called. Can be KNullDesC8.
     * @return A standard system error code.
     */
	IMPORT_C  TInt CustomCommandSync( const TMMFMessageDestinationPckg& aDestination,
					TInt aFunction, const TDesC8& aDataTo1, const TDesC8& aDataTo2 );

	/**
     * From MCustomCommand
     * Sends a synchronous custom command to the radio server.
     *
     * @since S60 3.0
     * @param aDestination The destination of the message, consisting of the uid of
     *		 the interface of this message
     * @param aFunction The function number to indicate which function is to be called
     *		 on the interface defined in the aDestination parameter
     * @param aDataTo1 The first chunk of data to be copied to the controller
     *		 framework. The exact contents of the data are dependent on the
     *		 interface being called. Can be KNullDesC8.
     * @param aDataTo2 The second chunk of data to be copied to the controller
     *		 framework. The exact contents of the data are dependent on the
     *		 interface being called. Can be KNullDesC8.
     * @param aDataFrom The area of memory to which the controller framework
     *		 will write any data to be passed back to the client. Can't be KNullDesC8.
     * @return A standard system error code.
     */
	IMPORT_C  TInt CustomCommandSync( const TMMFMessageDestinationPckg& aDestination,
					TInt aFunction, const TDesC8& aDataTo1, const TDesC8& aDataTo2, TDes8& aDataFrom );

	/**
     * From MCustomCommand
     * Sends an asynchronous custom command to the radio server.
     *
     * @since S60 3.0
     * @param aDestination The destination of the message, consisting of the uid of
     *		 the interface of this message
     * @param aFunction The function number to indicate which function is to be called
     *		 on the interface defined in the aDestination parameter
     * @param aDataTo1 The first chunk of data to be copied to the controller
     *		 framework. The exact contents of the data are dependent on the
     *		 interface being called. Can be KNullDesC8.
     * @param aDataTo2 The second chunk of data to be copied to the controller
     *		 framework. The exact contents of the data are dependent on the
     *		 interface being called. Can be KNullDesC8.
     * @param aStatus The TRequestStatus of an active object. This will contain the
     *		 result of the request on completion.  The exact range of result values is
     *		 dependent on the interface.
     */
	IMPORT_C  void CustomCommandAsync( const TMMFMessageDestinationPckg& aDestination,
					TInt aFunction, const TDesC8& aDataTo1, const TDesC8& aDataTo2, TRequestStatus& aStatus );

	/**
     * From MCustomCommand
     * Sends an asynchronous custom command to the radio server.
     *
     * @since S60 3.0
     * @param aDestination The destination of the message, consisting of the uid of
     *		 the interface of this message
     * @param aFunction The function number to indicate which function is to be called
     *		 on the interface defined in the aDestination parameter
     * @param aDataTo1 The first chunk of data to be copied to the controller
     *		 framework. The exact contents of the data are dependent on the
     *		 interface being called. Can be KNullDesC8.
     * @param aDataTo2 The second chunk of data to be copied to the controller
     *		 framework. The exact contents of the data are dependent on the
     *		 interface being called. Can be KNullDesC8.
     * @param aDataFrom The area of memory to which the controller framework
     *		 will write any data to be passed back to the client. Can't be KNullDesC8.
     * @param aStatus The TRequestStatus of an active object. This will contain the
     *		 result of the request on completion.  The exact range of result values is
     *		 dependent on the interface.
     */
	IMPORT_C  void CustomCommandAsync( const TMMFMessageDestinationPckg& aDestination, TInt aFunction,
					const TDesC8& aDataTo1, const TDesC8& aDataTo2, TDes8& aDataFrom, TRequestStatus& aStatus );

//********** Internal functions begin

	/**
     * Used internally to cancel outstanding asynchronous requests. This is triggered by
     * CRadioRequest.
     *
     * @since S60 3.0
     * @param aRequest The outstanding asynchronous request being cancelled
     */
	void CancelRequest( TInt aRequest );

private:

	/**
     * Creates request handlers for each asynchronous request.
     */
	void StartRequestHandlersL( MRadioObserver& aObserver );

	/**
     * Creates event handlers for tuner and playback events from radio tuner.
     */
	void StartEventHandlersL( MRadioObserver& aObserver );

	/**
     * Creates event handlers for RDS events from radio tuner.
     */
	void StartRdsEventHandlersL( TUint32 aRdsFunctions );

private:    // Data

	// Connection status
	TBool iConnected;
	// Requests that generates response to MRadioObserver
	RPointerArray<CRadioRequest> iRequests;
	// Event handlers that generates response to MRadioEventObserver
	RPointerArray<CRadioEventHandler> iEventHandlers;
	//Rds Event handlers
	RPointerArray<CRadioEventHandler> iRdsEventHandlers;

	// Destination information for standard radio interface messages
	TMMFMessageDestinationPckg iDestinationPckg;

	// Radio observer
	MRadioObserver* iObserver;
	// Client type
	TBool iPrimaryClient;
	// RDS notify flag
	TBool iRdsNotify;
    };

#endif      // RADIOSESSION_H

// End of File
