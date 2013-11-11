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
* Description:  Implementation header of the Radio FM Tuner Utility API
*
*
*/



#ifndef C_RADIOFMTUENRUTILITYBODY_H
#define C_RADIOFMTUENRUTILITYBODY_H

#include <RadioFmTunerUtility.h>

class RRadioSession;

/**
 *  This class encapsulates the implementation body of CRadioFmTunerUtility interface.
 *
 *  @lib Radio_Utility.lib
 *  @since S60 3.2
 */
class CRadioFmTunerUtility::CBody : public CBase
    {

public:

    static CBody* NewL( RRadioSession& aRadioSession, MRadioFmTunerObserver& aObserver );

    ~CBody();

     /**
     * Asynchronous request to get control of the FM tuner. User must be ready to receive
     * callback method MrftoRequestTunerControlComplete. Control to the tuner must be granted
     * before any functions that require control of the tuner (such as Tune) can be made.
     *
     * @since S60 3.2
     * @see MRadioFmTunerObserver::MrftoRequestTunerControlComplete
     */
    void RequestTunerControl();

    /**
     * Close the tuner and free all memory allocated. Any ongoing playing activity will be stopped,
     * control of the tuner will be released, and the hardware will be powered down if no other
     * clients need it.
     *
     * @since S60 3.2
     */
    void Close();

    /**
     * Get the capabilities of the FM tuner on the device.
     *
     * @since S60 3.2
     * @param aCaps The capabilities object to fill
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    TInt GetCapabilities( TFmTunerCapabilities& aCaps ) const;

    /**
     * Enable tuner functions when the device is in offline mode. Client
     * should first check the capabilities to see if this feature is supported.
     * Tuner functions are disabled in offline mode by default.
     *
     * @since S60 3.2
     * @param aEnable ETrue to enable tuner functions in offline mode, EFalse to disable.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    TInt EnableTunerInOfflineMode( TBool aEnable );

    /**
     * Asynchronous request to set the frequency range. User must be ready to receive
     * callback method MrftoSetFrequencyRangeComplete. If not set, it will be defaulted
     * to EFmRangeEuroAmerica.
     *
     * @since S60 3.2
     * @param aRange Frequency range
     */
    void SetFrequencyRange( TFmRadioFrequencyRange aRange );

    /**
     * Cancels an ongoing set frequency range operation, as initiated by a call to
     * SetFrequencyRange. The usual callback will not occur if this has been called.
     *
     * Has not effect if no set frequency range operation is ongoing.
     *
     * @since S60 3.2
     */
    void CancelSetFrequencyRange();

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
    TInt GetFrequencyRange( TFmRadioFrequencyRange& aRange, TInt& aMinFreq, TInt& aMaxFreq ) const;

    /**
     * Asynchronous request to tune the tuner to the speficied frequency (Hz). User must be
     * ready to receive callback method MrftoTuneComplete.
     *
     * @since S60 3.2
     * @param aFrequency The frequency to tune to
     * @see MRadioFmTunerObserver::MrftoTuneComplete
     */
    void SetFrequency( TInt aFrequency );

    /**
     * Cancels an ongoing tune operation, as initiated by a call to Tune.
     * The usual callback will not occur if this has been called.
     *
     * Has not effect if no tune operation is ongoing.
     *
     * @since S60 3.2
     */
    void CancelSetFrequency();

    /**
     * Get the currently tuned frequency.
     *
     * @since S60 3.2
     * @param aFrequency On return contains the current frequency (Hz).
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    TInt GetFrequency( TInt& aFrequency ) const;

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
    void StationSeek( TBool aUpwards );

    /**
     * Cancels an ongoing station seek operation, as initiated by a call to StationSeek.
     * The usual callback will not occur if this has been called.
     *
     * Has not effect if no station seek operation is ongoing.
     *
     * @since S60 3.2
     */
    void CancelStationSeek();

    /**
     * Get the signal strenth of the currently tuned frequency.
     *
     * @since S60 3.2
     * @param aSignalStrength On return contains the signal strength.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    TInt GetSignalStrength( TInt& aSignalStrength ) const;

    /**
     * Get the maximum possible signal strength of a tuned frequency.
     *
     * @since S60 3.2
     * @param aMaxSignalStrength On return contains the maximum signal strength.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    TInt GetMaxSignalStrength( TInt& aMaxSignalStrength ) const;

    /**
     * Get the current frequency's stereo mode (stereo or mono).
     *
     * @since S60 3.2
     * @param aStereo On return, will be ETrue if a stereo signal is currently being received.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    TInt GetStereoMode( TBool& aStereo ) const;

    /**
     * Indicates whether the reception should be forced into monophonic mode.
     *
     * @since S60 3.2
     * @param aMono If ETrue, all reception will be in mono mode even if a stereo signal
     *              is available. If EFalse, a stereo signal will be received when possible.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    TInt ForceMonoReception( TBool aForcedMono );

    /**
     * Find out whether reception is forced into monophonic mode or not.
     *
     * @since S60 3.2
     * @param aForcedMono On return, will be ETrue if a forced mono is currently enabled.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    TInt GetForcedMonoReception( TBool& aForcedMono ) const;

    /**
     * Set the current squleching (muting in frequencies without reception) setting.
     * Squelching is enabled by default.
     *
     * @since S60 3.2
     * @param aEnabled ETrue to enable squelching, EFalse to disable it.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    TInt SetSquelch( TBool aSquelch );

    /**
     * Get the current squelching (muting in frequencies without reception) setting.
     *
     * @since S60 3.2
     * @param aSquelch On return, will be ETrue if a squelching is currently enabled.
     * @return A standard system error code or FM tuner error (TFmRadioTunerError).
     */
    TInt GetSquelch( TBool& aSquelch ) const;

private:

    void ConstructL();
    CBody( RRadioSession& aRadioSession, MRadioFmTunerObserver& aObserver );

private: // data

    // The radio server session
    RRadioSession& iRadioSession;
    // Tuner utility observer
    MRadioFmTunerObserver& iRadioFmTunerUtilityClient;
    };

#endif  // C_RADIOFMTUNERUTILITY_H

// End of File
