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
* Description:  This is the main implementation of radio server.
*
*/



#ifndef RADIOSERVER_H
#define RADIOSERVER_H

//  INCLUDES
#include <e32base.h>
#include <FMRadioTunerControl.h>	// Adaptation
#include <RadioRdsControl.h>		// Adaptation
#include <mmf/server/sounddevice.h>
#include <cenrepnotifyhandler.h>

#ifdef RD_TSP_CLIENT_MAPPER
#include <tspclientmapper.h>
#endif // RD_TSP_CLIENT_MAPPER
#include "RadioServerData.h"
#include "RadioClientServer.h"
#include "RadioServerFMTuner.h"

// DATA TYPES
struct TRadioMessageRequestData
	{
	TSglQueLink iLink;
	TRadioServerRequest iType;
	RMessage2 iMessage;
	};

// FORWARD DECLARATIONS
class CFMRadioTunerControl;
class CRadioServerSettings;
class CRadioServerShutdown;
class CMMFBuffer;
class CRepository;

// CLASS DECLARATION

/**
*  This is the main implemetation of radio server.
*
*  @lib RadioServer.exe
*  @since S60 3.0
*/
class CRadioServer: public CServer2,
					public MRadioServerFMTunerObserver,
					public MDevSoundObserver,
					public MCenRepNotifyHandlerCallback
    {
public:  // Constructors and destructor

	/**
	 * Two-phased constructor.
	 */
	static CRadioServer* NewL();

	/**
	 * Destructor.
	 */
	virtual ~CRadioServer();

public: // New functions

	/**
	 * Keep track of session count (and cancel shutdown timer)
	 * @since S60 3.0
	 */
	void AddSession();

	/**
	 * Keep track of session count (and start shutdown timer if session count is zero)
	 * @since S60 3.0
	 */
	void DropSession();

	/**
	 * Called by CRadioServerSession to handle messages for Radio Server.
	 * @since S60 3.0
	 * @param aMessage Message containing client request and data
	 */
	void ServiceRequestL( const RMessage2& aMessage );

private:  // Functions from base classes

	/**
	 * From CServer2
	 * Creates a server-side client session object.
	 */
	CSession2* NewSessionL(const TVersion& aVersion,const RMessage2& aMessage) const;

	/**
	 * From MRadioServerFMTunerObserver
	 * See RadioServerFMTuner.h (or the actual adaptation APIs in FMRadioTunerControl.h
	 * and RadioRdsControl) for detailed description.
	 * @since S60 3.2
	 */
	void TunerOnComplete( TRadioServerError aError );
	void TunerOffComplete( TRadioServerError aError );
	void SetFrequencyRangeComplete( TRadioServerError aError );
	void SetFrequencyComplete( TRadioServerError aError );
	void StationSeekComplete( TRadioServerError aError, TInt aFrequency );
	void AudioModeComplete( TRadioServerError aError, TBool aStereo );
	void SetAudioModeComplete( TRadioServerError aError );
	void SignalStrengthComplete( TRadioServerError aError, TInt aStrength );
	void MaxSignalStrengthComplete( TRadioServerError aError, TInt aMaxStrength );
    void SquelchComplete( TRadioServerError aError, TBool aEnabled );
    void SetSquelchComplete( TRadioServerError aError );
	void BufferFilled( TDes8& aBuffer );
	void RadioEventAntennaStatusChange( TBool aAttached );
    void RadioEventTunerControlChange( TRadioServerError aError );
    // RDS
    void StationSeekByPTYComplete( TRadioServerError aError, TInt aFrequency );
    void StationSeekByTAComplete( TRadioServerError aError, TInt aFrequency );
    void StationSeekByTPComplete( TRadioServerError aError, TInt aFrequency );
	void RdsDataPI( TInt aPi );
	void RdsDataPTY( TRdsProgrammeType aPty );
	void RdsDataPS( TRdsPSName& aPs );
	void RdsDataRT( TRdsRadioText& aRt, RArray<TRdsRTplusTag>& aRTplusTags );
	void RdsDataCT( TDateTime& aCt );
	void RdsDataTA( TBool aTaOn );
	void RdsSearchBeginAF();
	void RdsSearchEndAF( TRadioServerError aError, TInt aFrequency );
	void RdsEventSignalChange( TBool aSignal );

	/**
	 * From MDevSoundObserver
	 * See SoundDevice.h for detailed description.
	 * @since S60 3.0
	 */
	void InitializeComplete( TInt aError );
	void ToneFinished( TInt aError );
	void BufferToBeFilled( CMMFBuffer* aBuffer );
	void PlayError( TInt aError );
	void BufferToBeEmptied( CMMFBuffer* aBuffer );
	void RecordError( TInt aError );
	void ConvertError(  TInt aError );
	void DeviceMessage( TUid aMessageType, const TDesC8& aMsg );
	void SendEventToClient( const TMMFEvent& aEvent );

	/**
	 * From MCenRepNotifyHandlerCallback
	 * See CenRepNotifyHandler.h for detailed description.
	 * @since S60 3.0
	 */
	void HandleNotifyInt( TUint32 aId, TInt aNewValue );

private:

	/**
	 * C++ default constructor.
	 */
	CRadioServer();

	/**
	 * By default Symbian 2nd phase constructor is private.
	 */
	void ConstructL();

	/**
	 * All asynchronous requests from the client are Q'ed and processed in order. This
	 * function is called to process the next request in the Q.
	 * @since S60 3.0
	 * @param aRequest Request from the client
	 */
	void ProcessAsyncRequest( TRadioMessageRequestData* aRequest );

	/**
	 * Request for control of a tuner.
	 * @since S60 3.2
	 * @param Tuner type (e.g. FM, AM)
	 */
	void ProcessRequestTunerControl( TRsTuner aTuner );

	/**
	 * Request to set the frequency range.
	 * @since S60 3.2
	 * @param aRange Frequency range
	 */
	void ProcessSetFrequencyRange( TRsFrequencyRange aRange );

	/**
	 * Tunes the tuner to the specified frequency.
	 * @since S60 3.0
	 * @param aFrequency The frequency (Hz) to tune to
	 */
	void ProcessSetFrequency( TInt aFrequency );

	/**
	 * Finds a radio station, starting from current frequency and seaching in the
	 * direction specified (i.e. up or down).
	 * @since S60 3.0
	 * @param aUpwards Search direction
	 */
	void ProcessStationSeek( TBool aUpwards );

	/**
	 * Starts playback of the tuner output.
	 * @since S60 3.0
	 */
	void ProcessPlay();

	/**
	 * Stops playback, and release the output device for use by other clients.
	 * @since S60 3.2
     * @param aIfOnlyPrimaryClient ETrue to stop playback only if there are no other primary clients
	 */
	void ProcessStop( TBool aIfOnlyPrimaryClient );

	/**
     * Asynchronous request to find a radio station with the specified Programme Type(PTY).
     * @since S60 3.2
     * @param aPty The type of programme to search for.
     * @param aSeekUp The direction to search in. Searches upwards if set to ETrue.
     */
	void ProcessStationSeekByPTY( TRsRdsProgrammeType aPty, TBool aSeekUp );

	/**
     * Asynchronous request to find a radio station with Traffic Announcement(TA).
     * @since S60 3.2
     * @param aSeekUp The direction to search in. Searches upwards if set to ETrue.
     */
	void ProcessStationSeekByTA( TBool aSeekUp );

	/**
     * Asynchronous request to find a radio station with Traffic Programme(TP).
     * @since S60 3.2
     * @param aSeekUp The direction to search in. Searches upwards if set to ETrue.
     */
	void ProcessStationSeekByTP( TBool aSeekUp );

	/**
	 * Indication that a session from primary client is being closed.
	 * @since S60 3.2
	 */
    TInt ProcessRemovePrimaryClientL( const RMessage2& aMessage );

	/**
	 * Get the capabilities of the radio on the device.
	 * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system/RadioServer error code.
	 */
	TInt ProcessGetTunerCapabilities( const RMessage2& aMessage );

	/**
	 * Request to enabled tuner in offline mode (if supported by the adaptation).
	 * @since S60 3.2
	 * @param aEnable ETrue to enable tuner functions in offline mode, EFalse to disable.
	 * @return A standard system error/RadioServer code.
	 */
	TInt ProcessEnableTunerInOfflineMode( TBool aEnable );

	/**
	 * Get the current frequency range. It also returns the minimum and maximum frequencies(Hz)
	 * for the returned range.
	 * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
	 */
	TInt ProcessGetFrequencyRange( const RMessage2& aMessage );

	/**
	 * Get the current frequency.
	 * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
	 */
	TInt ProcessGetFrequency( const RMessage2& aMessage );

	/**
	 * Checks whether force mono reception is on or not.
	 * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
	 */
	TInt ProcessGetForceMonoReception( const RMessage2& aMessage );

	/**
	 * Set the squelching flag.
	 * @since S60 3.2
	 * @param aEnabled ETrue to enable squelching, EFalse to disable it.
	 * @return A standard system error/RadioServer code.
	 */
	TInt ProcessSetSquelch( TBool aEnabled );

	/**
	 * Get the squelch status.
	 * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
	 */
	TInt ProcessGetSquelch( const RMessage2& aMessage );

	/**
	 * Some synchronous requests from the client are Q'ed and processed in order. This
	 * function is called to process the next request in the Q.
	 * @since S60 3.0
	 * @param aRequest Request from the client
	 */
	void ProcessSyncRequest( TRadioMessageRequestData* aRequest );

	/**
	 * Retrieves the signal strength of the currently tuned signal.
	 * @since S60 3.2
	 */
	void ProcessGetSignalStrength();

	/**
	 * Retrieves the maximum possible signal strength of a tuned signal.
	 * @since S60 3.2
	 */
	void ProcessGetMaxSignalStrength();

	/**
	 * Finds out if the current signal is being received in stereo or not.
	 * @since S60 3.0
	 */
	void ProcessGetStereoMode();

	/**
	 * Indicates whether the reception should be forced into monophonic mode.
	 * @since S60 3.2
	 * @param aForcedMono If ETrue, all reception will be in mono mode even if a stereo signal is
	 *		 available. If EFalse, a stereo signal will be received when possible.
	 */
	void ProcessForceMonoReception( TBool aForcedMono );

	/**
	 * Get the player state.
	 * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
	 */
	TInt ProcessGetPlayerState( const RMessage2& aMessage );

	/**
	 * Retrieves the maximum volume supported.
	 * @since S60 3.0
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
	 */
	TInt ProcessGetMaxVolume( const RMessage2& aMessage );

	/**
	 * Sets the volume to the specified level.
	 * @since S60 3.0
	 * @param aVolume The volume level to set
	 * @return A standard system error/RadioServer code.
	 */
	TInt ProcessSetVolume( TInt aVolume );

	/**
	 * Retrieves the current volume.
	 * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
	 */
	TInt ProcessGetVolume( const RMessage2& aMessage );

	/**
	 * Sets a volume ramp.
	 * @since S60 3.0
	 * @param aMessage Message containing client request and data
     * @return A standard system error/RadioServer code.
	 */
	TInt ProcessSetVolumeRamp( const RMessage2& aMessage );

	/**
	 * Mutes or unmutes playback.
	 * @since S60 3.0
	 * @param aMute ETrue to mute the audio, EFalse to unmute it.
	 * @return A standard system error/RadioServer code.
	 */
	TInt ProcessSetMute( TBool aMute );

 	/**
	 * Get mute status.
	 * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
	 */
	TInt ProcessGetMuteStatus( const RMessage2& aMessage );

	/**
	 * Sets the stereo balance between left and right channels.
	 * @since S60 3.2
     * @param aLeft Left speaker volume percentage. This value ranges from 0 to 100.
     * @param aRight Right speaker volume percentage. This value ranges from 0 to 100.

	 * @return A standard system error/RadioServer code.
	 */
	TInt ProcessSetBalance( TInt aLeft, TInt aRight );

	/**
	 * Gets the stereo balance between left and right channels.
	 * @since S60 3.0
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
	 */
	TInt ProcessGetBalance( const RMessage2& aMessage );

	/**
     * Gets the capabilities of the RDS control on the device.
     * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
     */
	TInt ProcessGetRdsCapabilities( const RMessage2& aMessage );

	/**
     * Gets the status of the RDS reception.
     * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
     */
	TInt ProcessGetRdsSignalStatus( const RMessage2& aMessage );

	/**
     * Subscribe for notification for the specified RDS data.
     * @since S60 3.2
	 * @return A standard system error/RadioServer code.
     */
	TInt ProcessNotifyRdsDataChange( const RMessage2& aMessage );

	/**
	 * Cancel NotifyRdsDataChange request.
     * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
     */
	TInt ProcessCancelNotifyRdsDataChange( const RMessage2& aMessage );

	/**
     * Turn on/off automatic switching of frequency based on Alternate Frequency.
     * @since S60 3.2
     * @param aAuto ETrue to turn automatic switching on, EFalse to turn it off.
	 * @return A standard system error/RadioServer code.
     */
	TInt ProcessSetAutomaticSwitching( TBool aAuto );

	/**
     * Find out whether automatic switching is on or off.
     * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
     */
	TInt ProcessGetAutomaticSwitching( const RMessage2& aMessage );

	/**
     * Cancel ongoing search for an Alternate Frequency (AF) with stronger signal.
     * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
     */
	TInt ProcessCancelAFSearch();

	/**
     * Get the current Programme Identification code.
     * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
     */
	TInt ProcessGetProgrammeIdentification( const RMessage2& aMessage );

	/**
     * Get the current Programme Type.
     * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
     */
	TInt ProcessGetProgrammeType( const RMessage2& aMessage );

	/**
     * Get the current Programme Service name.
     * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
     */
	TInt ProcessGetProgrammeService( const RMessage2& aMessage );

	/**
     * Get the current Radio Text.
     * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
     */
	TInt ProcessGetRadioText( const RMessage2& aMessage );

	/**
     * Get the current Radio Text+.
     * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
     */
	TInt ProcessGetRadioTextPlus( const RMessage2& aMessage );

	/**
     * Get the current Clock Time and date.
     * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
     */
	TInt ProcessGetClockTime( const RMessage2& aMessage );

	/**
     * Get Traffic Announcement status at the current station.
     * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
     */
	TInt ProcessGetTrafficAnnouncementStatus( const RMessage2& aMessage );

	/**
     * Get Traffic Programme status at the current station.
     * @since S60 3.2
	 * @param aMessage Message containing client request and data
	 * @return A standard system error/RadioServer code.
     */
	TInt ProcessGetTrafficProgrammeStatus( const RMessage2& aMessage );

	/**
	 * Completes the outstanding asynchronous message with aError and processes the next
	 * message in the queue, if any.
	 * @since S60 3.2
	 * @param aError A standard system/RadioServer error code
	 */
	void CompleteAsyncRequest( TInt aError );

	/**
	 * Completes the outstanding synchronous message with aError and processes the next
	 * message in the queue, if any.
	 * @since S60 3.2
	 * @param aError A standard system/RadioServer error code
	 */
	void CompleteSyncRequest( TInt aError );

	/**
	 * Request has been cancelled. Delete the request from the queue, if found.
	 * @since S60 3.0
	 * @param aQue Request queue
	 * @param aMessageType Request to be deleted
	 */
	void RemoveFromQueue( TSglQue<TRadioMessageRequestData>& aQue, TRadioServerRequest aMessageType );

	/**
	 * Check if another request exists in the queue. If so, process the request.
	 * @since S60 3.0
	 * @param aQue Request queue
	 */
	void ProcessNextItemInQueue( TSglQue<TRadioMessageRequestData>& aQue );

    /**
     * Clears all outstanding requests in RadioServer owned Qs.
     * @since S60 3.2
     */
	void ClearQueue();

    /**
     * Check to see if radio is allowed in offline mode.
     * @since S60 3.2
     */
	TBool AllowRadioInOfflineMode();

    /**
     * Gets the current frequency range.
     *
     * @since S60 3.2
     * @param aRange On return contains the current frequency range.
     * @param aMinFreq On return contains the minimum frequency for the current frequency range.
     * @param aMaxFreq On return contains the maximum frequency for the current frequency range.
     * @return A standard system/RadioServer error code
     */
    TInt GetFrequencyRange( TRsFrequencyRange& aRange, TInt& aMinFreq, TInt& aMaxFreq ) const;

    /**
     * Converts RadioServer range to Tuner control range.
     * @since S60 3.2
     */
	TFMRadioFrequencyRange TunerControlFreqRange( TRsFrequencyRange aRange );

    /**
     * Configures DevSound and initiates playback.
     * @since S60 3.2
     */
	void DoPlayL();

    /**
     * AK - temporary until adaptation fix is available (CPHU-73YTQW)
     */
	void RecreateFmTunerControl();

    /**
     * Add client ID to array.
     *
     * @since S60 3.2
     * @param aClientId client id.
     */
    void AddRdsNotifyClientId( TUint aClientId );

    /**
     * Remove client ID from array.
     *
     * @since S60 3.2
     * @param aClientId client id.
     */
    void RemoveRdsNotifyClientId( TUint aClientId );

    /**
     * Sets the TSP target client type based on the state of the player.
     *
     * @param aPlayerTargetState The target state of the player.
     * @since S60 3.2
     */
    void SetTspTargetClient( TRsPlayerState aPlayerTargetState );

    /**
     * Register client's PID
     *
     * @param aMessage Message containing client request and data
     * @since S60 3.2
     */
    void RegisterClientPidL( const RMessage2& aMessage );
private:    // Data

	enum TServerState
		{
		EStateStarted,
		EStateTunerOn,
		EStateTunerOff,
		EStatePlaying,
		EStateShutdown
		};

	enum TTunerOnTrigger
		{
		ETriggerTunerControl,
		ETriggerSetFrequency,
		ETriggerSetFrequencyRange,
		ETriggerPlay,
		ETriggerAntenna,
		ETriggerTransmitter,
		ETriggerOffline
		};

	enum TServerChannel
		{
		EChannelMono,
		EChannelStereo,
		EChannelBoth
		};

	// Server state
	TServerState iState;
	// Total number of active sessions
	TInt iSessionCount;
	// Number of active sessions from primary clients
	TInt iSessionCountPrimary;
	// Request which triggered TunerOn
	TTunerOnTrigger iTunerOnTrigger;

	// Delayed shutdown timer
	CRadioServerShutdown* iShutdownTimer;
	// Radio settings
	CRadioServerSettings* iSettings;
	// Tuner control observer handler
	CRadioServerFMTuner* iTunerControlObserver;

	// Tuner control adaptation
	CFMRadioTunerControl* iTunerControl;
	// Tuner control capabilities
	TFMRadioCapabilities iTunerCaps;
	// RDS control adaptation
	MRdsControl* iRdsControl;
	// RDS control capabilities
	TRdsCapabilities iRdsCaps;

	// DevSound
	CMMFDevSound* iDevSound;
	// Max volume supported by DevSound
	TInt iMaxVolume;
	// Central repository
	CRepository* iRepository;
	// Central repository notify handler
	CCenRepNotifyHandler* iCenRepNotifyHandler;

	// Queue for asynchronous requests
	TSglQue<TRadioMessageRequestData> iAsyncRequestQue;
	// Outstanding asynchronous request
	TRadioMessageRequestData* iAsyncRequest;

	// Queue for synchronous requests
	TSglQue<TRadioMessageRequestData> iSyncRequestQue;
	// Outstanding synchronous request
	TRadioMessageRequestData* iSyncRequest;

	// Max signal strength supported by the tuner
	TInt iMaxSigStrength;
	// Enable tuner functions when the device is in offline mode
	TBool iEnableTunerInOffline;
	// Temporary holder for frequency range
	TRsFrequencyRange iFreqRange;
	// Internal squelching indicator while seeking
	TBool iSquelch;
	// DevSound configuration
	TMMFCapabilities iDevSoundConfig;
	// Number of channels supported, i.e. Stereo/Mono

	TServerChannel iChannels;
	// Wait for Play - we've been pre-empted
	TBool iPreEmpted;
	// RDS notify request counter
	TInt iRdsNotifyCount;
	// Array to keep track of clients requesting to be notified of RDS Data
	RArray<TUint> iRdsNotifyClientIdArray;
	// Wait logic implementation towards adaptation
	CActiveSchedulerWait* iSchedulerWait;

#ifdef RD_TSP_CLIENT_MAPPER
    // Process Id of the client
    TProcessId iProcessId;
    CTspClientMapper* iMapper;
    CTspClientMapper::TTspClientMappingType iTspState;
    RMessage2 iMessageTsp;
    RArray<TProcessId> iClientPids;
#endif // RD_TSP_CLIENT_MAPPER
private:    // Friend classes
	friend class CRadioServerSession;
    };

#endif      // RADIOSERVER_H

// End of File
