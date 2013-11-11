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
* Description:  Implementation header of the Radio RDS Utility API
*
*
*/


#ifndef C_RADIORDSUTILITYBODY_H
#define C_RADIORDSUTILITYBODY_H

#include <RadioRdsUtility.h>
#include "RadioUtilityBody.h"

class RRadioSession;
class CRadioUtility;

/**
 *  This class encapsulates the implementation body of CRadioRdsUtility interface.
 *
 *  @lib Radio_Utility.lib
 *  @since S60 3.2
 */
class CRadioRdsUtility::CBody : public CBase
    {

public:

    static CBody*  NewL( RRadioSession& aRadioSession, MRadioRdsObserver& aObserver );

    ~CBody();

    /**
     * Close the RDS utility and free all memory allocated.
     *
     * @since S60 3.2
     */
    void Close();

    /**
     * Get the capabilities of the RDS control on the device.
     *
     * @since S60 3.2
     * @param aCaps The capabilities object to fill
     * @return A standard system error code or RDS control error (TRdsControlError).
     */
    TInt GetCapabilities( TRdsCapabilities& aCaps ) const;

    /**
     * Get the status of the RDS reception.
     *
     * @since S60 3.2
     * @param aRdsSignal On return, will be ETrue if RDS signal can be recepted, EFalse otherwise.
     * @return A standard system error code or RDS control error (TRdsControlError).
     */
    TInt GetRdsSignalStatus( TBool& aRdsSignal ) const;

    /**
     * Subscribe for notification for the specified RDS data. Client should first check
     * the capabilities to see if a feature is supported.
     * Request for notification for non-supported features will simply be ignored.
     *
     * For example, if the client wishes to be notified when new Programme Type and
     * Traffic Announcement is available:
     *      iRdsData = ERdsProgrammeType | ERdsTrafficAnnouncement;
     *      iRdsUtility->NotifyRdsDataChange(iRdsData);
     *
     * Subscribe for notification for the specified RadioText+ data. Client should first check
     * the capabilities to see if RT+ feature is supported.
     * Returns KErrNotSupported if RT+ is not supported.
     *
     * For example, if the client wishes to be notified only when new Title and Genre are available:
     *      iRtPlusClassArray.Append(ERTplusItemTitle);
     *      iRtPlusClassArray.Append(ERTplusItemTitle);
     *      iRdsUtility->NotifyRadioTextPlusChangeL(iRtPlusClassArray);
     *
     * Note that if the client wishes to receive the entire radio text data chunk, client should
     * subscribe for ERdsRadioText using NotifyRdsDataChange instead.
     *
     * @since S60 3.2
     * @param aRtPlusClasses Array of RT+ class to be notified
     *
     * @since S60 3.2
     * @param aRdsData Bitfield indicating notification request.
     * @see MRadioRdsObserver::MrroRdsDataPI
     * @see MRadioRdsObserver::MrroRdsDataPTY
     * @see MRadioRdsObserver::MrroRdsDataPS
     * @see MRadioRdsObserver::MrroRdsDataRT
     * @see MRadioRdsObserver::MrroRdsDataRTPlus
     * @see MRadioRdsObserver::MrroRdsDataCT
     * @see MRadioRdsObserver::MrroRdsDataTA
     */
     TInt NotifyRdsDataChange( TRdsData aRdsData );

    /**
     * Cancel NotifyRdsDataChange request.
     *
     * @since S60 3.2
     */
    void CancelNotifyRdsDataChange();

    /**
     * Turn on/off automatic switching of frequency based on Alternate Frequency.
     * This will cause RDS device to search for alternate frequency when the signal strength
     * deteriorates. User should be ready to receive MrroRdsSearchBeginAF and MrroRdsSearchEndAF.
     * Automatic switching is off by default.
     *
     * @since S60 3.2
     * @param aAuto ETrue to turn automatic switching on, EFalse to turn it off.
     * @return A standard system error code.
     * @see MRadioRdsObserver::MrroRdsSearchBeginAF
     * @see MRadioRdsObserver::MrroRdsSearchEndAF
     */
    TInt SetAutomaticSwitching( TBool aAuto );

    /**
     * Find out whether automatic switching is on or off.
     *
     * @since S60 3.2
     * @param aAuto On return, ETrue indicates that automatic switching is enabled.
     * @return A standard system error code.
     */
    TInt GetAutomaticSwitching( TBool& aAuto );

    /**
     * Cancel ongoing search for an Alternate Frequency (AF) with stronger signal.
     *
     * Client can issue this request to interrupt the search indicated with
     * MRadioRdsObserver::MrroRdsSearchBeginAF.
     *
     * @since S60 3.2
     */
    void CancelAFSearch();

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
     * @return A standard system error code.
     */
    TInt SetAutomaticTrafficAnnouncement( TBool aAuto );

    /**
     * Find out whether automatic traffic announcement is enabled.
     *
     * @since S60 3.2
     * @param aAuto On return, ETrue indicates that automatic traffic announcement is on.
     * @return A standard system error code.
     */
    IMPORT_C TInt GetAutomaticTrafficAnnouncement( TBool& aAuto );

    /**
     * Asynchronous request to find a radio station with the specified Programme Type(PTY),
     * starting from the currently tuned frequency and searching in the direction specified
     * (i.e. up or down). User must be ready to receive callback method MrroStationSeekByPTYComplete
     * The station found is returned in the callback.
     *
     * @since S60 3.2
     * @param aPty The type of programme to search for.
     * @param aSeekUp The direction to search in. Searches upwards if set to ETrue.
     * @see MRadioRdsObserver::MrroStationSeekByPTYComplete
     */
    IMPORT_C void StationSeekByPTY( TRdsProgrammeType aPty, TBool aSeekUp );

    /**
     * Asynchronous request to find a radio station with Traffic Announcement(TA),
     * starting from the currently tuned frequency and searching in the direction specified
     * (i.e. up or down). User must be ready to receive callback method MrroStationSeekByTAComplete
     * The station found is returned in the callback.
     *
     * @since S60 3.2
     * @param aSeekUp The direction to search in. Searches upwards if set to ETrue.
     * @see MRadioRdsObserver::MrroStationSeekByTAComplete
     */
     void StationSeekByTA( TBool aSeekUp );

    /**
     * Asynchronous request to find a radio station with Traffic Programme(TP),
     * starting from the currently tuned frequency and searching in the direction specified
     * (i.e. up or down). User must be ready to receive callback method MrroStationSeekByTPComplete
     * The station found is returned in the callback.
     *
     * @since S60 3.2
     * @param aSeekUp The direction to search in. Searches upwards if set to ETrue.
     * @see MRadioRdsObserver::MrroStationSeekByTPComplete
     */
    void StationSeekByTP( TBool aSeekUp );

    /**
     * Cancels an ongoing retune operation, as initiated by a call to StationSeekByPTY,
     * StationSeekByTA, or StationSeekByTP.
     * The usual callback will not occur if this has been called.
     *
     * @since S60 3.2
     */
     void CancelRdsStationSeek();

    /**
     * Asynchronous request to find all frequencies sending the given Programme Type (PTY).
     * User must be ready to receive callback method MrroGetFreqByPTYComplete.
     *
     * NOTE: This is only supported in dual tuner configuration since the secondary tuner
     * needs to perform continuous scanning for frequencies broadcasting given Programme Type
     * while the primary tuner is used for normal tuner activities.
     * Client should first check the tuner capabilities. Will return KErrNotSupported in
     * callback method if this feature is not supported.
     *
     * @since S60 3.2
     * @param aPty The type of programme to search for
     * @see MRadioRdsObserver::MrroGetFreqByPTYComplete
     */
    void GetFreqByPTY( TRdsProgrammeType aPty );

    /**
     * Cancels an ongoing request to find all frequencies sending a given Programme Type (PTY).
     * The usual callback will not occur if this has been called.
     *
     * @since S60 3.2
     */
     void CancelGetFreqByPTY();

    /**
     * Asynchronous request to find all frequencies sending Traffic Announcement (TA). User must
     * be ready to receive callback method MrroGetFreqByTAComplete.
     *
     * NOTE: This is only supported in dual tuner configuration since the secondary tuner
     * needs to perform continuous scanning for frequencies broadcasting given Traffic Announcement
     * while the primary tuner is used for normal tuner activities.
     * Client should first check the tuner capabilities. Will return KErrNotSupported in
     * callback method if this feature is not supported.
     *
     * @since S60 3.2
     * @see MRadioRdsObserver::MrroGetFreqByTAComplete
     */
     void GetFreqByTA();

    /**
     * Cancels an ongoing request to find all frequencies sending Traffic Announcement.
     * The usual callback will not occur if this has been called.
     *
     * @since S60 3.2
     */
    void CancelGetFreqByTA();

    /**
     * Asynchronous request to find all Programme Service names (PS) sending the given Programme
     * Type (PTY). User must be ready to receive callback method MrroGetPSByPTYComplete.
     *
     * NOTE: This is only supported in dual tuner configuration since the secondary tuner
     * needs to perform continuous scanning for frequencies broadcasting given Programme Type
     * while the primary tuner is used for normal tuner activities.
     * Client should first check the tuner capabilities. Will return KErrNotSupported in
     * callback method if this feature is not supported.
     *
     * @since S60 3.2
     * @param aPty The type of programme to search for
     * @see MRadioRdsObserver::MrroGetPSByPTYComplete
     */
     void GetPSByPTY( TRdsProgrammeType aPty );

    /**
     * Cancels an ongoing request to find all Programme Service names (PS) sending a given
     * Programme Type (PTY). The usual callback will not occur if this has been called.
     *
     * @since S60 3.2
     */
     void CancelGetPSByPTY();

    /**
     * Asynchronous request to find all Programme Service names (PS) sending Traffic Announcement (TA).
     * User must be ready to receive callback method MrroGetPSByTAComplete.
     *
     * NOTE: This is only supported in dual tuner configuration since the secondary tuner
     * needs to perform continuous scanning for frequencies broadcasting given Traffic Announcement
     * while the primary tuner is used for normal tuner activities.
     * Client should first check the tuner capabilities. Will return KErrNotSupported in
     * callback method if this feature is not supported.
     *
     * @since S60 3.2
     * @see MRadioRdsObserver::MrroGetPSByTAComplete
     */
    void GetPSByTA();

    /**
     * Cancels an ongoing request to find all Programme Service names (PS) sending Traffic Announcement.
     * The usual callback will not occur if this has been called.
     *
     * @since S60 3.2
     */
     void CancelGetPSByTA();

    /**
     * Get the current Programme Identification code.
     * RDS data is received over the air and may not be available immediately following
     * tune operation. If data is not available, this function will return KErrNotFound.
     * If a value is returned, this is the last known value, which may not be up to date.
     * To be notified of the most recent value, client should use NotifyRdsDataChange.
     *
     * @since S60 3.2
     * @param aPi On return contains Programme Identification code
     * @return A standard system error code or RDS control error (TRdsControlError).
     */
     TInt GetProgrammeIdentification( TInt& aPi );

    /**
     * Get the current Programme Type.
     * RDS data is received over the air and may not be available immediately following
     * tune operation. If data is not available, this function will return KErrNotFound.
     * If a value is returned, this is the last known value, which may not be up to date.
     * To be notified of the most recent value, client should use NotifyRdsDataChange.
     *
     * @since S60 3.2
     * @param aPty On return contains Programme Type
     * @return A standard system error code or RDS control error (TRdsControlError).
     */
     TInt GetProgrammeType( TRdsProgrammeType& aPty );

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
     * @return A standard system error code or RDS control error (TRdsControlError).
     */
     TInt GetProgrammeService( TRdsPSName& aPs );

    /**
     * Get the current Radio Text.
     * RDS data is received over the air and may not be available immediately following
     * tune operation. If data is not available, this function will return KErrNotFound.
     * If a value is returned, this is the last known value, which may not be up to date.
     * To be notified of the most recent value, client should use NotifyRdsDataChange.
     *
     * The maximum possible length for radio text is 64 characters.
     *
     * @since S60 3.2
     * @param aRt On return contains Radio Text
     * @return A standard system error code or RDS control error (TRdsControlError).
     */
     TInt GetRadioText( TRdsRadioText& aRt );

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
     * @return A standard system error code or RDS control error (TRdsControlError).
     */
     TInt GetRadioTextPlus(TRdsRTplusClass aRtPlusClass, TRdsRadioText& aRtPlusData);

    /**
     * Get the current Clock Time and date.
     * RDS data is received over the air and may not be available immediately following
     * tune operation. If data is not available, this function will return KErrNotFound.
     * If a value is returned, this is the last known value, which may not be up to date.
     * To be notified of the most recent value, client should use NotifyRdsDataChange.
     *
     * @since S60 3.2
     * @param aCt On return contains current time and date
     * @return A standard system error code or RDS control error (TRdsControlError).
     */
     TInt GetClockTime( TDateTime& aCt );

    /**
     * Get Traffic Announcement status at the current station.
     *
     * @since S60 3.2
     * @param aTaStatus On return, will be ETrue if current station has ongoing traffic announcement
     * @return A standard system error code or RDS control error (TRdsControlError).
     */
     TInt GetTrafficAnnouncementStatus( TBool& aTaStatus );

    /**
     * Get Traffic Programme status at the current station.
     *
     * @since S60 3.2
     * @param aTpStatus On return, will be ETrue if current station supports traffic programme
     * @return A standard system error code or RDS control error (TRdsControlError).
     */
     TInt GetTrafficProgrammeStatus( TBool& aTpStatus );

private:

    void ConstructL();
    CBody( RRadioSession& aRadioSession, MRadioRdsObserver& aObserver );

private: // data

    // The radio server session
    RRadioSession& iRadioSession;
    // RDS Utility Observer
    MRadioRdsObserver& iRadioRdsUtilityClient;
    // RDS notification reqs
    TRsRdsData iRdsDataReq;
    };

#endif    // C_RADIORDSUTILITY_H

// End of File
