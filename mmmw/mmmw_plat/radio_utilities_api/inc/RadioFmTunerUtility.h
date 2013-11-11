/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class provides an interface for controlling the FM tuner
*                controls such as tuning or seeking frequencies. In addition an
*                observer class is provided for receiving completion notification
*                for asynchronous requests made to FM tuner control and for receiving
*                events notifying changes in the tuner such as frequency change
*                (caused by other clients or RDS, if supported), loss of tuner
*                control,etc.
*
*
*/



#ifndef C_RADIOFMTUNERUTILITY_H
#define C_RADIOFMTUNERUTILITY_H

#include <RadioUtility.h>

class RRadioSession;

/**
 *  FM tuner error definitions.
 *  In addition to the standard system error code, the following error codes may
 *  be returned.
 */
typedef TInt TFmRadioTunerError;
const TFmRadioTunerError KFmRadioErrHardwareFaulty              = -12050;
const TFmRadioTunerError KFmRadioErrTuning                      = -12051;
const TFmRadioTunerError KFmRadioErrFrequencyOutOfBandRange     = -12052;
const TFmRadioTunerError KFmRadioErrFrequencyNotValid           = -12053;
const TFmRadioTunerError KFmRadioErrNotPermittedInCurrentState 	= -12054;
const TFmRadioTunerError KFmRadioErrDuplicateRequest            = -12055;
const TFmRadioTunerError KFmRadioErrAntennaNotConnected	        = -12056;
const TFmRadioTunerError KFmRadioErrFmTransmitterActive	        = -12057;
const TFmRadioTunerError KFmRadioErrOfflineMode                 = -12058;


/**  FM radio frequency range */
enum TFmRadioFrequencyRange
    {
    EFmRangeEuroAmerica		= 0x01,		// European and American FM frequency range
    EFmRangeJapan			= 0x02		// Japanese FM frequency range
    };


/**
 *  The FM Tuner Capabilities class defines the capabilities of the FM tuner on the
 *  device, as retrieved using the function GetCapabilities.
 *
 *  @since S60 3.2
 */
class TFmTunerCapabilities
    {
public:
    //  Tuner Features bit flags - may be extended in future
    enum TFmTunerFeatures
        {
        // This bit is set if the tuner can be used even when the device is in
		// offline mode. Some tuner may not allow this due to hw conflict.
        ETunerAvailableInOfflineMode	  = 0x01,
        // This bit is set if the tuner has RDS support.
        ETunerRdsSupport                  = 0x02,
        // This bit is set when the device supports dual tuner, which allows
		// more extensive RDS feature support.
        ETunerDualTunerSupport            = 0x04
        };

public:
    // Bitfield (as defined by TFmRadioFrequencyRange) with the bits of the supported frequency range
    TUint32 iTunerBands;
    // Bitfield (as defined by TFmTunerFunctions) with the bits of the supported functions set
    TUint32 iTunerFunctions;
    // Reserved for future
    TUint32 iAdditionalFunctions1;
    // Reserved for future
    TUint32 iAdditionalFunctions2;
    };


/**
 *  This class provides the interface class to be implemented by object receiving request
 *  complete notification from FM tuner control. These functions are called once the asynchronous
 *  request completes successfully or with an error.
 *
 *  In addition, this class provides the interface for receiving events from FM tuner control.
 *
 *  @since S60 3.2
 */
class MRadioFmTunerObserver
    {
public:

    /**
     * Request for tuner control complete event. This event is asynchronous and is received after
     * a call to CRadioFmTunerUtility::RequestTunerControl.
     *
     * If this function returns KErrNone, control of the tuner has been granted.
     *
     * If this function returns KFMRadioErrAntennaNotConnected, KFMRadioErrFmTransmitterActive,
     * or KFMRadioErrOfflineMode, client will receive the following events respectively:
     * MrfteoAntennaStatusChanged, MrfteoFmTransmitterStatusChanged, MrfteoOfflineModeStatusChanged.
     * At this time, client must re-issue RequestTunerControl. Note that functions that require
     * control of the tuner (such as Tune) will return one of the above error codes.
     *
     * In case of multiple clients accessing the tuner, or when RequestTunerControl is re-issued,
     * this function returns KErrAlreadyExists.
     *
     * @since S60 3.2
     * @param aError A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    virtual void MrftoRequestTunerControlComplete( TInt aError ) = 0;

    /**
     * Set frequency range complete event. This event is asynchronous and is received after
     * a call to CRadioFmTunerUtility::SetFrequencyRange.
     *
     * @since S60 3.2
     * @param aError A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    virtual void MrftoSetFrequencyRangeComplete( TInt aError ) = 0;

    /**
     * Set frequency complete event. This event is asynchronous and is received after a call to
	 * CRadioFmTunerUtility::SetFrequency.
     *
     * @since S60 3.2
     * @param aError A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    virtual void MrftoSetFrequencyComplete( TInt aError ) = 0;

    /**
     * Station seek complete event. This event is asynchronous and is received after a call to
	 * CRadioFmTunerUtility::StationSeek.
     *
     * @since S60 3.2
     * @param aError A standard system error code or FM tuner error (TFmRadioTunerError).
     * @param aFrequency The frequency(Hz) of the radio station that was found.
     */
    virtual void MrftoStationSeekComplete( TInt aError, TInt aFrequency ) = 0;

    /**
     * Called when FM Transmitter status changes (if one is present in the device). Tuner receiver
     * is forced to be turned off due to hardware conflicts when FM transmitter is activated.
     *
     * @since S60 3.2
     * @param aActive ETrue if FM transmitter is active; EFalse otherwise.
     */
    virtual void MrftoFmTransmitterStatusChange( TBool aActive ) = 0;

    /**
     * Called when antenna status changes.
     *
     * @since S60 3.2
     * @param aAttached ETrue if antenna is attached; EFalse otherwise.
     */
    virtual void MrftoAntennaStatusChange( TBool aAttached ) = 0;

    /**
     * Called when offline mode status changes.
     * @since S60 3.2
     *
     ** @param aAttached ETrue if offline mode is enabled; EFalse otherwise.
     */
    virtual void MrftoOfflineModeStatusChange( TBool aOfflineMode ) = 0;

    /**
     * Called when the frequency range changes. This may be caused by other applications.
     *
     * @since S60 3.2
     * @param aNewRange New frequency range.
     */
    virtual void MrftoFrequencyRangeChange( TFmRadioFrequencyRange aNewRange ) = 0;

    /**
     * Called when the tuned frequency changes. This may be caused by other
     * applications or RDS if AF/TA is enabled.
     *
     * @since S60 3.2
     * @param aNewFrequency The new tuned frequency(Hz).
     */
    virtual void MrftoFrequencyChange( TInt aNewFrequency ) = 0;

    /**
     * Called when the forced mono status change. This may be caused by other applications.
     *
     * @since S60 3.2
     * @param aForcedMono ETrue if forced mono mode is enabled; EFalse otherwise.
     */
    virtual void MrftoForcedMonoChange( TBool aForcedMono ) = 0;

    /**
     * Called when the squelch (muting the frequencies without broadcast) status change.
     * This may be caused by other applications.
     *
     * @since S60 3.2
     * @param aSquelch ETrue if squelch is enabled; EFalse otherwise.
     */
    virtual void MrftoSquelchChange( TBool aSquelch ) = 0;
	};

/**
 *  This class provides an interface to allow clients to control the FM tuner hardware
 *  present on a device.
 *
 *  @lib RadioUtility.lib
 *  @since S60 3.2
 */
class CRadioFmTunerUtility : public CBase
    {
    friend class CRadioUtility::CBody;

public:

    /**
     * Asynchronous request to get control of the FM tuner. This method call must be made before
     * any other method calls in this API.
     * User must be ready to receive callback method MrftoRequestTunerControlComplete. Control
     * to the tuner must be granted before any functions that require control of the tuner
     * (such as SetFrequency) can be made.
     * The only API call that doesn't require tuner control to be granted is EnableTunerInOfflineMode().
     *
     * @since S60 3.2
     * @see MRadioFmTunerObserver::MrftoRequestTunerControlComplete
     */
    IMPORT_C void RequestTunerControl();

    /**
     * Close the tuner and free all memory allocated. Any ongoing playing activity will be stopped,
     * control of the tuner will be released, and the hardware will be powered down if no other
     * clients need it.
     *
     * @since S60 3.2
     */
	IMPORT_C void Close();

    /**
     * Get the capabilities of the FM tuner on the device.
     *
     * @since S60 3.2
     * @param aCaps The capabilities object to fill
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    IMPORT_C TInt GetCapabilities( TFmTunerCapabilities& aCaps );

    /**
     * Enable tuner functions when the device is in offline mode. Client
     * should first check the capabilities to see if this feature is supported.
     *
     * When requested prior to requesting tuner control, this function will succeed with
     * KErrNone, however, tuner will only be enabled in offline mode if supported by the
     * tuner device.
     *
     * Tuner functions are disabled in offline mode by default.
     *
     * @since S60 3.2
     * @param aEnable ETrue to enable tuner functions in offline mode, EFalse to disable.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    IMPORT_C TInt EnableTunerInOfflineMode( TBool aEnable );

    /**
     * Asynchronous request to set the frequency range. User must be ready to receive
     * callback method MrftoSetFrequencyRangeComplete. If not set, it will be defaulted
     * to EFmRangeEuroAmerica.
     *
     * @since S60 3.2
     * @param aRange Frequency range
     */
    IMPORT_C void SetFrequencyRange( TFmRadioFrequencyRange aRange );

    /**
     * Cancels an ongoing set frequency range operation, as initiated by a call to
     * SetFrequencyRange. The usual callback will not occur if this has been called.
     *
     * Has not effect if no set frequency range operation is ongoing.
     *
     * @since S60 3.2
     */
    IMPORT_C void CancelSetFrequencyRange();

    /**
     * Get the current frequency range. It also returns the minimum and maximum frequencies (Hz)
     * for the returned range. Client should always use the minimum and maximum frequencies returned
     * by the tuner device, since it may vary slightly depending on the hardware.
     *
     * @since S60 3.2
     * @param aRange On return contains the current frequency range.
     * @param aMinFreq On return contains the minimum frequency for the current frequency range.
     * @param aMaxFreq On return contains the maximum frequency for the current frequency range.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    IMPORT_C TInt GetFrequencyRange( TFmRadioFrequencyRange& aRange, TInt& aMinFreq, TInt& aMaxFreq ) const;

    /**
     * Asynchronous request to tune the tuner to the speficied frequency (Hz). User must be
     * ready to receive callback method MrftoTuneComplete.
     *
     * @since S60 3.2
     * @param aFrequency The frequency to tune to
     * @see MRadioFmTunerObserver::MrftoTuneComplete
     */
    IMPORT_C void SetFrequency( TInt aFrequency );

    /**
     * Cancels an ongoing tune operation, as initiated by a call to Tune.
     * The usual callback will not occur if this has been called.
     *
     * Has not effect if no tune operation is ongoing.
     *
     * @since S60 3.2
     */
    IMPORT_C void CancelSetFrequency();

    /**
     * Get the currently tuned frequency.
     *
     * @since S60 3.2
     * @param aFrequency On return contains the current frequency (Hz).
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    IMPORT_C TInt GetFrequency( TInt& aFrequency ) const;

    /**
     * Asynchronous request to find a radio station, starting from the currently tuned frequency and
     * searching in the direction specified (i.e. up or down). User must be ready to receive callback
     * method MrftoSeekComplete. The station found is returned in the callback.
     *
     * If no station is found after a complete loop, seek will stop and return KErrNotfound
     * in the callback method and frequency will not change.
     *
     * @since S60 3.2
     * @param aUpwards The direction to search in. Searches upwards if set to ETrue.
     * @see MRadioFmTunerObserver::MrftoStationSeekComplete
     */
    IMPORT_C void StationSeek( TBool aUpwards );

    /**
     * Cancels an ongoing station seek operation, as initiated by a call to StationSeek.
     * The usual callback will not occur if this has been called.
     *
	 * Has not effect if no station seek operation is ongoing.
     *
     * @since S60 3.2
     */
    IMPORT_C void CancelStationSeek();

    /**
     * Get the signal strenth of the currently tuned frequency.
     *
     * @since S60 3.2
     * @param aSignalStrength On return contains the signal strength.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    IMPORT_C TInt GetSignalStrength( TInt& aSignalStrength ) const;

    /**
     * Get the maximum possible signal strength of a tuned frequency.
     *
     * @since S60 3.2
     * @param aMaxSignalStrength On return contains the maximum signal strength.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    IMPORT_C TInt GetMaxSignalStrength( TInt& aMaxSignalStrength ) const;

    /**
     * Get the current frequency's stereo mode (stereo or mono).
     *
     * @since S60 3.2
     * @param aStereo On return, will be ETrue if a stereo signal is currently being received.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    IMPORT_C TInt GetStereoMode( TBool& aStereo ) const;

    /**
     * Indicates whether the reception should be forced into monophonic mode.
     *
     * @since S60 3.2
     * @param aMono If ETrue, all reception will be in mono mode even if a stereo signal
     *              is available. If EFalse, a stereo signal will be received when possible.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    IMPORT_C TInt ForceMonoReception( TBool aForcedMono );

    /**
     * Find out whether reception is forced into monophonic mode or not.
     *
     * @since S60 3.2
     * @param aForcedMono On return, will be ETrue if a forced mono is currently enabled.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    IMPORT_C TInt GetForcedMonoReception( TBool& aForcedMono ) const;

    /**
     * Set the current squleching (muting in frequencies without reception) setting.
     * Squelching is enabled by default.
     *
     * @since S60 3.2
     * @param aEnabled ETrue to enable squelching, EFalse to disable it.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    IMPORT_C TInt SetSquelch( TBool aSquelch );

    /**
     * Get the current squelching (muting in frequencies without reception) setting.
     *
     * @since S60 3.2
     * @param aSquelch On return, will be ETrue if a squelching is currently enabled.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    IMPORT_C TInt GetSquelch( TBool& aSquelch ) const;

private:

    /**
     * Factory function to create a new instance of the FM tuner.
     *
     * @since S60 3.2
     * @param aRadioSession Reference to radio session
     * @param aObserver The observer object for receiving async completion callbacks
     * @return A new FM tuner utility object.
     */
    static CRadioFmTunerUtility* NewL( RRadioSession& aRadioSession, MRadioFmTunerObserver& aObserver );

    virtual ~CRadioFmTunerUtility();

    CRadioFmTunerUtility();

    void ConstructL();

private: // data

    class CBody;

    /**
     * FM tuner utility implementation body
     * Own.
     */
    CBody* iBody;
    };

#endif // C_RADIOFMTUNERUTILITY_H
