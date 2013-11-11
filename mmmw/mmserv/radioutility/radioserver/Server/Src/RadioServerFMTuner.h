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
* Description:  This class implements an interface to FM tuner control.
*
*/



#ifndef RADIOSERVERFMTUNER_H
#define RADIOSERVERFMTUNER_H

//  INCLUDES
#include <e32base.h>
#include <FMRadioTunerControl.h>
#include <RadioRdsControl.h>
#include "RadioServerData.h"

// DATA TYPES
enum TTunerEvent
	{
	ETunerEventTunerOn				=  1,
	ETunerEventTunerOff,
	ETunerEventSetFrequencyRange,
	ETunerEventSetFrequency,
	ETunerEventStationSeek,			// 5
	ETunerEventAudioMode,
	ETunerEventSetAudioMode,
	ETunerEventMaxSignalStrength,
	ETunerEventSignalStrength,
	ETunerEventSquelch,				// 10
	ETunerEventSetSquelch,
	ETunerEventAntennaStatusChange,
	ETunerEventTunerControlChange,
	ETunerEventStationSeekByPTY,
	ETunerEventStationSeekByTA,		// 15
	ETunerEventStationSeekByTP,
	};

struct TRadioFmTunerData
	{
	TSglQueLink iLink;
	TTunerEvent iEvent;
	TFMRadioTunerError iError;
	};

// CLASS DECLARATION

/**
*  This class provides the interface class to be implemented by object receiving events
*  from CRadioServerFMTuner.
*
*  @lib RadioServer.exe
*  @since 3.2
*/
class MRadioServerFMTunerObserver
	{
public:

	/**
	 * Completion message for TunerOn request.
     *
     * @since S60 3.0
	 * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
	 */
	virtual void TunerOnComplete( TRadioServerError aError ) = 0;

	/**
	 * Completion message for TunerOff request.
     *
	 * @since S60 3.0
	 * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
	 */
	virtual void TunerOffComplete( TRadioServerError aError ) = 0;

	/**
	 * Completion message for SetFrequencyRange request.
     *
	 * @since S60 3.0
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
     * Completion message for AudioMode request.
     *
     * @since S60 3.2
	 * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aStereo ETrue if signal is stereo.
     */
    virtual void AudioModeComplete( TRadioServerError aError, TBool aStereo ) = 0;

    /**
     * Completion message for SetAudioMode request.
     *
     * @since S60 3.2
	 * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     */
    virtual void SetAudioModeComplete( TRadioServerError aError ) = 0;

	/**
	 * Completion message for GetSignalStrength request.
     *
	 * @since S60 3.0
	 * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
	 * @param aStrength Valid only if aError is KErrNone. Contains the signal strength
	 *        of the currently tuned frequency.
	 */
	virtual void SignalStrengthComplete( TRadioServerError aError, TInt aStrength ) = 0;

	/**
	 * Completion message for GetMaxSignalStrength request.
     *
	 * @since S60 3.0
	 * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
	 * @param aMaxStrength Valid only if aError is KErrNone. Contains the maximum
	 *	     signal strength supported.
	 */
	virtual void MaxSignalStrengthComplete( TRadioServerError aError, TInt aMaxStrength ) = 0;

    /**
     * Completion message for Squelch request.
     *
     * @since S60 3.0
	 * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aEnabled ETrue if squelching is enabled, EFalse if disabled.
     */
    virtual void SquelchComplete( TRadioServerError aError, TBool aEnabled ) = 0;

    /**
     * Completion message for SetSquelch request.
     *
     * @since S60 3.0
	 * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     */
    virtual void SetSquelchComplete( TRadioServerError aError ) = 0;

	/**
	 * Notification mechanism provided to inform client that BufferToBeFilled request has completed.
     *
	 * @since S60 3.0
	 * @param aBuffer Buffer with data to be played.
	 */
	virtual void BufferFilled( TDes8& aBuffer ) = 0;

	/**
	 * Notification mechanism provided to inform that the antenna status has changed.
     *
	 * @since S60 3.0
	 * @param aAttached ETrue if antenna is attached.
	 */
	virtual void RadioEventAntennaStatusChange( TBool aAttached ) = 0;

	/**
     * Notification mechanism indicating tuner control change. When tuner receiver is
     * forced to be turned off due to hardware conflict such as a FM transmitter, this
     * event is sent with an error code.
     * NOTE: Once the tuner control is restored, this event is sent again with KErrNone
     * and the client can re-issue TunerOn.
     *
     * @since S60 3.2
	 * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     */
    virtual void RadioEventTunerControlChange( TRadioServerError aError ) = 0;

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
     * Notification mechanism indicating New Programme Identification(PI) is available.
     *
     * @since S60 3.2
     * @param aPi Programme identification
     */
    virtual void RdsDataPI( TInt aPi ) = 0;

    /**
     * Notification mechanism indicating New Programme Type (PTY) is available.
     *
     * @since S60 3.2
     * @param aPty Programme type
     */
    virtual void RdsDataPTY( TRdsProgrammeType aPty ) = 0;

    /**
     * Notification mechanism indicating New Programme Service name (PS) is available.
     *
     * @since S60 3.2
     * @param aPs Programme service
     */
    virtual void RdsDataPS( TRdsPSName& aPs ) = 0;

    /**
     * Notification mechanism indicating New Radio Text (RT) is available.
     *
     * @since S60 3.2
     * @param aRt Radio text
     * @param aRTplusTags RTplus tags
     */
    virtual void RdsDataRT( TRdsRadioText& aRt, RArray<TRdsRTplusTag>& aRTplusTags ) = 0;

    /**
     * Notification mechanism indicating Clock Time (CT) is available.
     *
     * @since S60 3.2
     * @param aCt Clock time
     */
    virtual void RdsDataCT( TDateTime& aCt ) = 0;

    /**
     * Notification mechanism indicating Traffice Announcement (TA) status changes.
     *
     * @since S60 3.2
     * @param aTaOn ETrue indicates that Traffic Announcement is on.
     */
    virtual void RdsDataTA( TBool aTaOn ) = 0;

    /**
     * Notification mechanism indicating that tuner started to search for an Alternate Frequency (AF).
     *
     * @since S60 3.2
     */
    virtual void RdsSearchBeginAF() = 0;

    /**
     * Notification mechanism indicating end of search for an Alternate Frequency.
     *
     * @since S60 3.2
	 * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aFrequency The frequency (Hz) of the radio station that was found.
     */
    virtual void RdsSearchEndAF( TRadioServerError aError, TInt aFrequency ) = 0;

    /**
     * Notification mechanism indicating RDS signal status change (i.e. signal is lost/restored).
     *
     * @since S60 3.2
     * @param aSignal ETrue indicates that RDS signal is available in the tuned frequency.
     */
    virtual void RdsEventSignalChange( TBool aSignal ) = 0;
	};

/**
*  This class is the FM tuner adaptation observer. This is pretty much a pass through.
*  The only purpose of this class is to guarantee asynchronous replies. In addition, it
*  also provides type conversion such as adaptation error code to RadioServer error code, etc.
*
*  @lib RadioServer.exe
*  @since S60 3.0
*/
class CRadioServerFMTuner : public CActive,
							public MFMRadioTunerControlObserver
#ifdef	RD_FM_RADIO_ENHANCEMENTS
							,public MRdsControlObserver
#endif
    {
public:  // Constructors and destructor

	/**
	 * Two-phased constructor.
	 */
	static CRadioServerFMTuner* NewL( MRadioServerFMTunerObserver& aObserver );

	/**
	 * Destructor.
	 */
	virtual ~CRadioServerFMTuner();

public: // Functions from base classes

	/**
	 * From MFMRadioTunerControlObserver
	 * See FMRadioTunerControl.h for detailed description.
	 * @since S60 3.0
	 */
	void TunerOnComplete( TFMRadioTunerError aError );
	void TunerOffComplete( TFMRadioTunerError aError );
	void SetFrequencyRangeComplete( TFMRadioTunerError aError );
	void SetFrequencyComplete( TFMRadioTunerError aError );
	void StationSeekComplete( TFMRadioTunerError aError, TInt aFrequency );
	void AudioModeComplete( TFMRadioTunerError aError, TFMRadioAudioMode aMode );
	void SetAudioModeComplete( TFMRadioTunerError aError );
	void MaxSignalStrengthComplete( TFMRadioTunerError aError, TInt aStrength );
	void SignalStrengthComplete( TFMRadioTunerError aError, TInt aStrength );
	void SquelchComplete( TFMRadioTunerError aError, TBool aEnabled );
	void SetSquelchComplete( TFMRadioTunerError aError );
	void BufferFilled( TDes8& aBuffer );
	void RadioEventAntennaStatusChange( TBool aAttached );

#ifdef	RD_FM_RADIO_ENHANCEMENTS
	void RadioEventTunerControlChange( TInt aError );

	/**
	 * From MRdsControlObserver
	 * See RadioRdsControl.h for detailed description.
	 * @since S60 3.2
	 */
	void StationSeekByPTYComplete( TInt aError, TInt aFrequency );
	void StationSeekByTAComplete( TInt aError, TInt aFrequency );
	void StationSeekByTPComplete( TInt aError, TInt aFrequency );
	void GetFreqByPTYComplete( TInt aError, RArray<TInt>& aFreqList );
	void GetFreqByTAComplete( TInt aError, RArray<TInt>& aFreqList );
	void GetPSByPTYComplete( TInt aError, RArray<TRdsPSName>& aPsList );
	void GetPSByTAComplete( TInt aError, RArray<TRdsPSName>& aPsList );
	void RdsDataPI( TInt aPi );
	void RdsDataPTY( TRdsProgrammeType aPty );
	void RdsDataPS( TRdsPSName& aPs );
	void RdsDataRT( TRdsRadioText& aRt, RArray<TRdsRTplusTag>& aRTplusTags );
	void RdsDataCT( TDateTime& aCt );
	void RdsDataTA( TBool aTaOn );
	void RdsSearchBeginAF();
	void RdsSearchEndAF( TInt aError, TInt aFrequency );
	void RdsStationChangeTA( TInt aFrequency );
	void RdsEventSignalChange( TBool aSignal );
#endif

private:  // Functions from base classes

	/**
	 * From CActive
	 * Cancel outstanding request.
	 */
	void DoCancel();

	/**
	 * From CActive
	 * Implementation of CActive::RunL. Called when server request has completed.
	 **/
	void RunL();

private:

	/**
	 * C++ default constructor.
	 */
	CRadioServerFMTuner( MRadioServerFMTunerObserver& aObserver );

	/**
	 * By default Symbian 2nd phase constructor is private.
	 */
	void ConstructL();

	/**
	 * Used internally to ensure asynchronous behaviour of a request completion message.
	 * @since S60 3.2
	 * @param aEvent Tuner event
	 * @param aError Tuner error
	 */
	void CompleteNotification( TTunerEvent aEvent, TFMRadioTunerError aError=KErrNone );

	/**
	 * Check if another request exists in the queue. If so, process the request.
	 * @since S60 3.2
	 */
	void ProcessNextItemInQueue();

	/**
	 * Map tuner error to a standard system error code.
	 */
	TInt MapTunerError( TFMRadioTunerError aError );

private:    // Data

	// FM Tuner Observer
	MRadioServerFMTunerObserver& iObserver;

	// Queue for asynchronous requests
	TSglQue<TRadioFmTunerData> iFmTunerQue;
	TRadioFmTunerData* iFmTunerData;

	// Temporary variables
	TInt iFrequency;
	TFMRadioAudioMode iMode;
	TInt iStrength;
	TBool iAttached;
	TBool iEnabled;
    };

#endif      // RADIOSERVERFMTUNER_H

// End of File
