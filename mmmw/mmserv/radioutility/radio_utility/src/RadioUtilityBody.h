/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation header of the Radio Utility API
*
*/



#ifndef C_RADIOUTILITYBODY_H
#define C_RADIOUTILITYBODY_H

#include <RadioUtility.h>
#include "RadioServerData.h"
#include "RadioSession.h"

class CRadioFmTunerUtility;
class MRadioFmTunerObserver;
class CRadioPlayerUtility;
class MRadioPlayerObserver;
class CRadioRdsUtility;
class MRadioRdsObserver;

/**
 *  This class encapsulates the implementation body of the CRadioUtility interface.
 *
 *  @lib radio_utility.lib
 *  @since S60 3.2
 */
class CRadioUtility::CBody : public CBase,
                             public MRadioObserver
    {

public:

    static CBody* NewL( TBool aPrimaryClient );

    ~CBody();

    /**
     * Returns a reference to FM Tuner Utility class for controlling the tuner settings.
     *
     * @since S60 3.2
     * @param aObserver The observer object for receiving async completion callbacks
     */
    CRadioFmTunerUtility& RadioFmTunerUtilityL( MRadioFmTunerObserver& aObserver );

    /**
     * Returns a reference to Radio Player Utility class for controlling the radio playback.
     *
     * @since S60 3.2
     * @param aObserver The observer object for receiving async completion callbacks
     */
    CRadioPlayerUtility& RadioPlayerUtilityL( MRadioPlayerObserver& aObserver );

    /**
     * Returns a reference to Radio RDS Utility class for accessing the Radio Data System (RDS).
     *
     * @since S60 3.2
     * @param aObserver The observer object for receiving async completion callbacks
     */
    CRadioRdsUtility& RadioRdsUtilityL( MRadioRdsObserver& aObserver );

private:

// from base class MRadioObserver

    // Tuner Events

    /**
     * From MRadioObserver
     * Completion message for RequestTunerControl request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     */
    void RequestTunerControlComplete( TRadioServerError aError );

    /**
     * From MRadioObserver
     * Completion message for SetFrequencyRange request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     */
    void SetFrequencyRangeComplete( TRadioServerError aError );

    /**
     * From MRadioObserver
     * Completion message for SetFrequency request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     */
    void SetFrequencyComplete( TRadioServerError aError );

    /**
     * From MRadioObserver
     * Completion message for StationSeek request.
     *
     * @since Series 60 3.0
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aFrequency Valid only if aError is KErrNone. Contains the new frequency in Hz.
     */
    void StationSeekComplete( TRadioServerError aError, TInt aFrequency );

    /**
     * From MRadioObserver
     * Antenna status has changed.
     *
     * @since Series 60 3.0
     * @param aAttached ETrue if antenna is attached.
     */
    void RadioEventAntennaStatusChange( TBool aAttached );

    /**
     * From MRadioObserver
     * Event notification indicating FM transmitter status change. Radio receiver
     * is turned off when FM transmitter is active.
     *
     * @since S60 3.2
     * @param aActive ETrue if FM transmitter is active; EFalse otherwise.
     */
    void RadioEventTransmitterStatusChange( TBool aActive );

    /**
     * From MRadioObserver
     * Event notification indicating offline mode change.
     *
     * @since S60 3.0
     * @param aOfflineMode ETrue if device is in offline mode.
     */
    void RadioEventOfflineModeChange( TBool aOfflineMode );

    /**
     * From MRadioObserver
     * Event notification indicating frequency range change. This may be caused by
     * other applications.
     *
     * @since S60 3.2
     * @param aNewRange New frequency range.
     */
    void RadioEventFrequencyRangeChanged( TRsFrequencyRange aNewRange );

    /**
     * From MRadioObserver
     * Event notification indicating frequency(Hz) change. This may be caused by
     * other applications or RDS if AF/TA is enabled.
     *
     * @since S60 3.2
     * @param aFrequency New frequency where tuner is currently tuned.
     */
    void RadioEventFrequencyChange( TInt aFrequency );

     /**
     * From MRadioObserver
     * Event notification indicating forced mono status change.
     *
     * @since S60 3.2
     * @param aForcedMono ETrue if forced mode is enabled; EFalse otherwise.
     */
    void RadioEventForcedMonoChanged( TBool aForcedMono );

    /**
     * From MRadioObserver
     * Event notification indicating squelch (muting the frequencies without broadcast)
     * status change.
     *
     * @since S60 3.2
     * @param aSquelch ETrue if squelch is enabled; EFalse otherwise.
     */
    void RadioEventSquelchChanged( TBool aSquelch );

    // Player events

    /**
     * From MRadioObserver
     * Event notification indicating radio player state change. This may be caused by
     * other applications.
     *
     * @since S60 3.0
     * @param aRadioOn ETrue if radio is playing, otherwise radio is off.
     * @param aError Valid only if aRadioOn is EFalse. Contains the reason why radio is off.
     */
    void RadioEventStateChange( TBool aRadioOn, TInt aError );

    /**
     * From MRadioObserver
     * Event notification indicating volume change.
     *
     * @since S60 3.2
     * @param aVolume New volume.
     */
    void RadioEventVolumeChange( TInt aVolume );


    /**
     * From MRadioObserver
     * Event notification indicating mute setting change.
     *
     * @since S60 3.2
     * @param aMute ETrue indicates audio is muted.
     */
    void RadioEventMuteChange( TBool aMute );


    /**
     * From MRadioObserver
     * Event notification indicating balance setting change.
     *
     * @since S60 3.2
     * @param aLeftPercentage Left speaker volume percentage. This value ranges from 0 to 100.
     * @param aRightPercentage Right speaker volume percentage. This value ranges from 0 to 100.
     */
    void RadioEventBalanceChange( TInt aLeftPercentage, TInt aRightPercentage );

    // RDS Events

    /**
     * From MRadioObserver
     * Completion message for StationSeekByPTY request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aFrequency The frequency(Hz) of the radio station that was found.
     */
    void StationSeekByPTYComplete( TInt aError, TInt aFrequency );

    /**
     * From MRadioObserver
     * Completion message for StationSeekByTA request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aFrequency The frequency(Hz) of the radio station that was found.
     */
    void StationSeekByTAComplete( TInt aError, TInt aFrequency );

    /**
     * From MRadioObserver
     * Completion message for StationSeekByTP request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aFrequency The frequency(Hz) of the radio station that was found.
     */
    void StationSeekByTPComplete( TInt aError, TInt aFrequency );

    /**
     * From MRadioObserver
     * Completion message for GetFreqByPTY request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aFreqList Array of frequencies (Hz), valid only if aError is KErrNone.
     */
    void GetFreqByPTYComplete( TInt aError, RArray<TInt>& aFreqList );

    /**
     * From MRadioObserver
     * Completion message for GetFreqByTA request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aFreqList Array of frequencies (Hz), valid only if aError is KErrNone.
     */
    void GetFreqByTAComplete( TInt aError, RArray<TInt>& aFreqList );

    /**
     * From MRadioObserver
     * Completion message for StatGetPSByPTY request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aPsList Array of programme service names, valid only if aError is KErrNone.
     */
    void GetPSByPTYComplete( TInt aError, RArray<TRsRdsPSName>& aPsList );

    /**
     * From MRadioObserver
     * Completion message for GetPSByTA request.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aPsList Array of programme service names, valid only if aError is KErrNone.
     */
    void GetPSByTAComplete( TInt aError, RArray<TRsRdsPSName>& aPsList );

    /**
     * From MRadioObserver
     * Event notification indicating new Programme Identification(PI) is available.
     *
     * @since S60 3.2
     * @param aPi Programme identification
     */
    void RadioEventRdsDataPI( TInt aPi );

    /**
     * From MRadioObserver
     * Event notification indicating new Programme Type(PTY) is available.
     *
     * @since S60 3.2
     * @param aPty Programme type
     */
    void RadioEventRdsDataPTY( TRsRdsProgrammeType aPty );

    /**
     * From MRadioObserver
     * Event notification indicating new Programme Service(PS) is available.
     *
     * @since S60 3.2
     * @param aPs Programme service
     */
    void RadioEventRdsDataPS( TRsRdsPSName& aPs );

    /**
     * From MRadioObserver
     * Event notification indicating new Radio Text(RT) is available.
     *
     * @since S60 3.2
     * @param aRt Radio text
     */
    void RadioEventRdsDataRT( TRsRdsRadioText& aRt );

    /**
     * From MRadioObserver
     * Event notification indicating new Clock Time(CT) is available.
     *
     * @since S60 3.2
     * @param aCt Clock time
     */
    void RadioEventRdsDataCT( TDateTime& aCt );

    /**
     * From MRadioObserver
     * Event notification indicating Traffice Announcement(TA) status change.
     *
     * @since S60 3.2
     * @param aTaOn ETrue indicates that Traffic Announcement is on.
     */
    void RadioEventRdsDataTA( TBool aTaOn );

    /**
     * From MRadioObserver
     * Event notification indicating new Radio Text+(RT+) is available.
     *
     * @since S60 3.2
     * @param aRtPlusClass Radio text plus class
     * @param aRtPlusData Radio text plus data
     */
    void RadioEventRdsDataRTplus( TRsRdsRTplusClass aRtPlusClass, TRsRdsRadioText& aRtPlusData );

    /**
     * From MRadioObserver
     * Event notification indicating the beginning of Alternate Frequency(AF) search.
     *
     * @since S60 3.2
     */
    void RadioEventRdsSearchBeginAF();

    /**
     * From MRadioObserver
     * Event notification indicating the end of Alternate Frequency(AF) search.
     *
     * @since S60 3.2
     * @param aError KErrNone if successful, otherwise one of the system/RadioServer errors.
     * @param aFrequency The frequency(Hz) of the radio station that was found.
     */
    void RadioEventRdsSearchEndAF( TRadioServerError aError, TInt aFrequency );

    /**
     * From MRadioObserver
     * Event notification indicating station change to another frequency(Hz) that is
     * broadcasting Traffic Announcement(TA).
     *
     * @since S60 3.2
     * @param aFrequency The frequency(Hz) of the radio station that was found.
     */
    void RadioEventRdsStationChangeTA( TInt aFrequency );

    /**
     * From MRadioObserver
     * Event notification indicating automatic switching (AF) setting change.
     *
     * @since S60 3.2
     * @param aAuto ETrue indicates that automatic switching is on.
     */
    void RadioEventRdsAutomaticSwitchingChange( TBool aAuto );

    /**
     * From MRadioObserver
     * Event notification indicating automatic traffic announcement setting change.
     *
     * @since S60 3.2
     * @param aAuto ETrue indicates that automatic traffic announcement is on.
     */
    void RadioEventRdsAutomaticTrafficAnnouncement( TBool aAuto );

    /**
     * From MRadioObserver
     * Event notification indicating RDS signal status change (i.e. signal is lost/restored).
     *
     * @since S60 3.2
     * @param aSignal ETrue indicates that RDS signal is available in the tuned frequency.
     */
    void RadioEventRdsSignalChange( TBool aSignal );

private:

    void ConstructL( TBool aPrimaryClient );

    CBody();

private:

    // The radio server session
    RRadioSession*          iRadioSession;   // own

    // FM Tuner Utility
    CRadioFmTunerUtility*   iFmTunerUtility; // own
    // Radio Player Utility
    CRadioPlayerUtility*    iPlayerUtility;  // own
    // Radio RDS Utility
    CRadioRdsUtility*       iRdsUtility;     // own

    MRadioFmTunerObserver*  iTunerObserver;     // not own
    MRadioPlayerObserver*   iPlayerObserver;    // not own
    MRadioRdsObserver*      iRdsObserver;       // not own

};

#endif  // C_RADIOUTILITYBODY_H

// End of File


