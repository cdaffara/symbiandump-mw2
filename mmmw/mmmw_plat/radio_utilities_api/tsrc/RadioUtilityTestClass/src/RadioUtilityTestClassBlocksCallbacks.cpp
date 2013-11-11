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
* Description:  RadioUtilityTestClass callback  class for STIF Test Framework TestScripter.
*
*/



// INCLUDE FILES
#include "RadioUtilityTestClass.h"
#include "debug.h"


// ===================================================================
// CRadioUtilityTestClass::MrmEvent
// ===================================================================
void CRadioUtilityTestClass::MrmEvent( TRadioMonitorEvent aEvent )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrmEvent")));
    iLog->Log(_L("CRadioUtilityTestClass::MrmEvent"));
  	if (aEvent == ERadioStateOff)
  	    iLog->Log(_L("MrmEvent: Radio state is off"));
    else if(aEvent == ERadioStateOn)
    	iLog->Log(_L("MrmEvent: Radio state is on"));
}


// ===================================================================
// CRadioUtilityTestClass::MrftoRequestTunerControlComplete
/*
 * If this function returns KErrNone, control of the tuner has been granted.
 * If this function returns KFMRadioErrAntennaNotConnected, KFMRadioErrFmTransmitterActive,
 * or KFMRadioErrOfflineMode, client will receive the following events respectively:
 * MrfteoAntennaStatusChanged, MrfteoFmTransmitterStatusChanged, MrfteoOfflineModeStatusChanged.
 * At this time, client can chose to re-issue RequestTunerControl or proceed to other tuner
 * control requests. Note that functions that require control of the tuner (such as Tune) will
 * return one of the above error codes.
 * In case of multiple clients accessing the tuner, or when RequestTunerControl is re-issued,
 * this function returns KErrAlreadyExists.
*/
// ===================================================================
void CRadioUtilityTestClass::MrftoRequestTunerControlComplete( TInt aError )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrftoRequestTunerControlComplete")));
    // Send the error to the testscripter instead just return KErrCallbackErrorCode
    iLog->Log(_L("MrftoRequestTunerControlComplete, return code =%d!!!"),aError);
   // sue replace following line to comment out lines
    ProcessEvent(ERequestTunerControlComplete, aError);
  //  if (aError != KErrNone)
  //  	Signal(aError);
}


// ===================================================================
// CRadioUtilityTestClass::MrftoSetFrequencyRangeComplete
/**
 * Set frequency range complete event. This event is asynchronous and is received after
 * a call to CRadioFmTunerUtility::SetFrequencyRange.
 * @since S60 3.2
 * @param aError A standard system error code or FM tuner error (TFmRadioTunerError).
 */
// ===================================================================
void CRadioUtilityTestClass::MrftoSetFrequencyRangeComplete( TInt aError )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrftoSetFrequencyRangeComplete")));
    
    iLog->Log(_L("MrftoSetFrequencyRangeComplete, return code =%d!!!"),aError);
    ProcessEvent(ESetFrequencyRangeComplete, aError);
}


// ===================================================================
// CRadioUtilityTestClass::MrftoSetFrequencyComplete
/**
 * Set frequency complete event. This event is asynchronous and is received after a call to
 * CRadioFmTunerUtility::SetFrequency.
 * @since S60 3.2
 * @param aError A standard system error code or FM tuner error (TFmRadioTunerError).
 */
// ===================================================================
void CRadioUtilityTestClass::MrftoSetFrequencyComplete( TInt aError )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrftoSetFrequencyComplete")));
    
    iLog->Log(_L("MrftoSetFrequencyComplete, return code =%d!!!"),aError);
    ProcessEvent(ETuneComplete, aError);
}


// ===================================================================
// CRadioUtilityTestClass::MrftoStationSeekComplete
/**
 * Station seek complete event. This event is asynchronous and is received after a call to
 * CRadioFmTunerUtility::StationSeek.
 * @since S60 3.2
 * @param aError A standard system error code or FM tuner error (TFmRadioTunerError).
 * @param aFrequency The frequency(Hz) of the radio station that was found.
 */
// KErrNotfound if not station was found
// ===================================================================
void CRadioUtilityTestClass::MrftoStationSeekComplete( TInt aError, TInt aFrequency )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrftoStationSeekComplete")));
    iLog->Log(_L("MrftoStationSeekComplete, return code =%d!!!"),aError);
    iLog->Log(_L("MrftoStationSeekComplete, return frequency =%d!!!"),aFrequency);
    
    ProcessEvent(EStationSeekComplete, aError);
}


// ===================================================================
// CRadioUtilityTestClass::MrftoFmTransmitterStatusChange
/**
 * Called when FM Transmitter status changes (if one is present in the device). Tuner receiver
 * is forced to be turned off due to hardware conflicts when FM transmitter is activated.
 * @since S60 3.2
 * @param aActive ETrue if FM transmitter is active; EFalse otherwise.
 */
// ===================================================================
void CRadioUtilityTestClass::MrftoFmTransmitterStatusChange( TBool aActive )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrftoFmTransmitterStatusChange")));
    iLog->Log(_L("MrftoFmTransmitterStatusChange, return FM transmitter=%d!!!"),aActive);
}


// ===================================================================
// CRadioUtilityTestClass::MrftoAntennaStatusChange
/**
 * Called when antenna status changes.
 * @since S60 3.2
 * @param aAttached ETrue if antenna is attached; EFalse otherwise.
 */
// ===================================================================
void CRadioUtilityTestClass::MrftoAntennaStatusChange( TBool aAttached )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrftoAntennaStatusChange")));
    iLog->Log(_L("MrftoAntennaStatusChange, return Attached attached=%d!!!"),aAttached);
}


// ===================================================================
// CRadioUtilityTestClass::MrftoOfflineModeStatusChange
/**
 * Called when offline mode status changes.
 * @since S60 3.2
 ** @param aAttached ETrue if offline mode is enabled; EFalse otherwise.
 */
// ===================================================================
void CRadioUtilityTestClass::MrftoOfflineModeStatusChange( TBool aOfflineMode )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrftoOfflineModeStatusChange")));
    iLog->Log(_L("MrftoOfflineModeStatusChange, return OfflineMode enabled=%d!!!"),aOfflineMode);

	ProcessEvent(EMrftoOfflineModeStatusChange, KErrNone);
}


// ===================================================================
// CRadioUtilityTestClass::MrftoFrequencyRangeChange
/**
 * Called when the frequency range changes. This may be caused by other applications.
 * @since S60 3.2
 * @param aNewRange New frequency range.
 */
// ===================================================================
// temp solution
void CRadioUtilityTestClass::MrftoFrequencyRangeChange( TFmRadioFrequencyRange aNewRange )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrftoFrequencyRangeChange")));
    iLog->Log(_L("MrftoFrequencyRangeChange, aBand =%s"),aNewRange);
		
}

	

// ===================================================================
// CRadioUtilityTestClass::MrftoFrequencyChange
/**
 * Called when the tuned frequency changes. This may be caused by other
 * applications or RDS if AF/TA is enabled.
 * @since S60 3.2
 * @param aNewFrequency The new tuned frequency(Hz).
 */
// ===================================================================
void CRadioUtilityTestClass::MrftoFrequencyChange( TInt aNewFrequency )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrftoFrequencyChange")));
    iLog->Log(_L("MrftoFrequencyChange, return NewFrequency=%d!!!"),aNewFrequency);
	ProcessEvent(EFrequencyChanged, KErrNone);
}


// ===================================================================
// CRadioUtilityTestClass::MrftoForcedMonoChange  
/**
 * Called when the forced mono status change. This may be caused by other applications.
 * @since S60 3.2
 * @param aForcedMono ETrue if forced mono mode is enabled; EFalse otherwise.
 */
// ===================================================================
void CRadioUtilityTestClass::MrftoForcedMonoChange( TBool aForcedMono )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrftoForcedMonoChange")));
    iLog->Log(_L("MrftoForcedMonoChange, return forced mono mode enabled=%d!!!"),aForcedMono);
	ProcessEvent(EMrftoForcedMonoChange, KErrNone);
}


// ===================================================================
// CRadioUtilityTestClass::MrftoSquelchChange
/**
 * Called when the squelch (muting the frequencies without broadcast) status change.
 * This may be caused by other applications.
 * @since S60 3.2
 * @param aSquelch ETrue if squelch is enabled; EFalse otherwise.
 */
// ===================================================================
void CRadioUtilityTestClass::MrftoSquelchChange( TBool aSquelch )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrftoSquelchChange")));
    iLog->Log(_L("MrftoSquelchChange, return squelch enabled=%d!!!"),aSquelch);
	ProcessEvent(EMrftoSquelchChange, KErrNone);
}


// ===================================================================
// CRadioUtilityTestClass::MrpoStateChange
/**
 * Radio state changed. This event is asynchronous and is received after a call to
 * CRadioPlayerUtility::Play and CRadioPlayerUtility::Stop. This event is also generated when
 * the radio is disrupted due to an error.
 *
 * If the radio is disrupted due to an error, appropriate error code will be sent.
 * The following are the most common error codes: KErrDied when radio is pre-empted by another
 * application with higher priority, KErrHardwareNotAvailable when tuner is forced off
 * (e.g. when FM transmitter is turned on).

If radio plays successfully, the user will receive callback method with ERadioPlayerPlaying and KErrNone. If a problem is encountered, user will receive callback method with ERadioPlayerIdle and an error code.

 * @since S60 3.2
 * @param aState Radio player state
 * @param aError A standard system error code, only used when aState is ERadioPlayerIdle
 */
// ===================================================================
//temp solution
void CRadioUtilityTestClass::MrpoStateChange( TPlayerState aState, TInt aError )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrpoStateChange")));
    iLog->Log(_L("MrpoStateChange, return TPlayerState=%d!!!"),aState);
    ProcessEvent(EMrpoStateChange, aError);
    
}


// ===================================================================
// CRadioUtilityTestClass::MrpoVolumeChange
/**
 * Called when volume changes. This may be caused by other applications.
 *
 * @since S60 3.2
 * @param aVolume Current volume.
 */
// ===================================================================
void CRadioUtilityTestClass::MrpoVolumeChange( TInt aVolume )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrpoVolumeChange")));
    iLog->Log(_L("MrpoVolumeChange, return Current volume=%d!!!"),aVolume);
	ProcessEvent(EMrpoVolumeChange, KErrNone);
}


// ===================================================================
// CRadioUtilityTestClass::MrpoMuteChange
// ===================================================================
void CRadioUtilityTestClass::MrpoMuteChange( TBool aMute )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrpoMuteChange")));
    iLog->Log(_L("MrpoMuteChange, return Mute=%d!!!"),aMute);
	ProcessEvent(EMrpoMuteChange, KErrNone);
}


// ===================================================================
// CRadioUtilityTestClass::MrpoBalanceChange
/**
 * Called when balance setting changes. This may be caused by other applications.
 *
 * @since S60 3.2
 *        Left speaker volume percentage. This can be any value from zero to 100.
 *        Zero value means left speaker is muted.
 * @param aRightPercentage
 *        Right speaker volume percentage. This can be any value from zero to 100.
 *        Zero value means right speaker is muted.
 */
// ===================================================================
void CRadioUtilityTestClass::MrpoBalanceChange( TInt aLeftPercentage, TInt aRightPercentage )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrpoBalanceChange")));
	iLog->Log(_L("MrpoBalanceChange, return Left=%d, Right=%d!!!"),aLeftPercentage,aRightPercentage);
	ProcessEvent(EMrpoBalanceChange, KErrNone);
}


// RDS Call back

// ===================================================================
// CRadioUtilityTestClass::MrroStationSeekByPTYComplete
/**
 * Station seek by Programme Type(PTY) complete event. This event is asynchronous and
 * is received after a call to CRadioRdsUtility::StationSeekByPTY.
 *
 * @since S60 3.2
 * @param aError A standard system error code or RDS control error (TRdsControlError).
 * @param aFrequency The frequency(Hz) of the radio station that was found.
 */
// ===================================================================
void CRadioUtilityTestClass::MrroStationSeekByPTYComplete( TInt aError, TInt aFrequency )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroStationSeekByPTYComplete")));
    iLog->Log(_L("MrroStationSeekByPTYComplete, return code =%d!!!"),aError);
    iLog->Log(_L("MrroStationSeekByPTYComplete, return frequency =%d!!!"),aFrequency);
    
    ProcessEvent(EMrroStationSeekByPTYComplete, aError);
}


// ===================================================================
// CRadioUtilityTestClass::MrroStationSeekByTAComplete
/**
 * Station seek by Traffic Announcement(TA) complete event. This event is asynchronous
 * and is received after a call to CRadioRdsUtility::StationSeekByTA.
 *
 * @since S60 3.2
 * @param aError A standard system error code or RDS control error (TRdsControlError).
 * @param aFrequency The frequency(Hz) of the radio station that was found.
 */
// ===================================================================
void CRadioUtilityTestClass::MrroStationSeekByTAComplete( TInt aError, TInt aFrequency )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroStationSeekByTAComplete")));
    iLog->Log(_L("MrroStationSeekByTAComplete, return code =%d!!!"),aError);
    iLog->Log(_L("MrroStationSeekByTAComplete, return frequency =%d!!!"),aFrequency);
    
    ProcessEvent(EMrroStationSeekByTAComplete, aError);
}


// ===================================================================
// CRadioUtilityTestClass::MrroStationSeekByTPComplete
/**
 * Station seek by Traffic Programme(TP) complete event. This event is asynchronous and
 * is received after a call to CRadioRdsUtility::StationSeekByTP.
 *
 * @since S60 3.2
 * @param aError A standard system error code or RDS control error (TRdsControlError).
 * @param aFrequency The frequency(Hz) of the radio station that was found.
 */
// ===================================================================
void CRadioUtilityTestClass::MrroStationSeekByTPComplete( TInt aError, TInt aFrequency )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroStationSeekByTPComplete")));
    iLog->Log(_L("MrroStationSeekByTPComplete, return code =%d!!!"),aError);
    iLog->Log(_L("MrroStationSeekByTPComplete, return frequency =%d!!!"),aFrequency);
    
    ProcessEvent(EMrroStationSeekByTPComplete, aError);
}


// ===================================================================
// CRadioUtilityTestClass::MrroGetFreqByPTYComplete
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
// ===================================================================
void CRadioUtilityTestClass::MrroGetFreqByPTYComplete( TInt /*aError*/, RArray<TInt>& /*aFreqList */)
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroGetFreqByPTYComplete")));
}


// ===================================================================
// CRadioUtilityTestClass::MrroGetFreqByTAComplete
/**
 * Get frequencies by Traffic Announcement(TA) complete event. This event is asynchronous
 * and is received after a call to CRadioRdsUtility::GetFreqByTA.
 *
 * Client takes ownership of the array and therefore is responsible for deallocating it.
 *
 * @since S60 3.2
 * @param aError A standard system error code or RDS control error (TRdsControlError).
 * @param aFreqList Array of frequencies (Hz), valid only if aError is KErrNone.
 */
// ===================================================================
void CRadioUtilityTestClass::MrroGetFreqByTAComplete( TInt /*aError*/, RArray<TInt>& /*aFreqList*/ )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroGetFreqByTAComplete")));
}


// ===================================================================
// CRadioUtilityTestClass::MrroGetPSByPTYComplete
/**
 * Get Programme Service names (PS) by Programme Type(PTY) complete event. This event is
 * asynchronous and is received after a call to CRadioRdsUtility::GetPSByPTY.
 *
 * Client takes ownership of the array and therefore is responsible for deallocating it.
 *
 * @since S60 3.2
 * @param aError A standard system error code or RDS control error (TRdsControlError).
 * @param aPsList Array of programme service names, valid only if aError is KErrNone.
 */
// ===================================================================
void CRadioUtilityTestClass::MrroGetPSByPTYComplete( TInt /*aError*/, RArray<TRdsPSName>& /*aPsList*/ )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroGetPSByPTYComplete")));
}


// ===================================================================
// CRadioUtilityTestClass::MrroGetPSByTAComplete
/**
 * Get Programme Service names (PS) by Traffic Announcement(TA) complete event. This event is
 * asynchronous and is received after a call to CRadioRdsUtility::GetPSByTA.
 *
 * Client takes ownership of the array and therefore is responsible for deallocating it.
 *
 * @since S60 3.2
 * @param aError A standard system error code or RDS control error (TRdsControlError).
 * @param aPsList Array of programme service names, valid only if aError is KErrNone.
 */
// ===================================================================
void CRadioUtilityTestClass::MrroGetPSByTAComplete( TInt /*aError*/, RArray<TRdsPSName>& /*aPsList*/ )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroGetPSByTAComplete")))
}


// ===================================================================
// CRadioUtilityTestClass::MrroRdsDataPI
/**
 * New Programme Identification(PI) is available. This event is asynchronous and is
 * received after a call to CRadioRdsUtility::NotifyRdsDataChange. Client will continue to
 * receive this notification whenever new PI is available until clients unsubscribes.
 *
 * @since S60 3.2
 * @param aPi Programme identification
 */
// ===================================================================
void CRadioUtilityTestClass::MrroRdsDataPI( TInt /*aPi*/ )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroRdsDataPI")))
}


// ===================================================================
// CRadioUtilityTestClass::MrroRdsDataPTY
/**
 * New Programme Type(PTY) is available. This event is asynchronous and is received after
 * a call to CRadioRdsUtility::NotifyRdsDataChange. Client will continue to receive this
 * notification whenever new PTY is available until clients unsubscribes.
 *
 * @since S60 3.2
 * @param aPty Programme type
 */
// ===================================================================
void CRadioUtilityTestClass::MrroRdsDataPTY( TRdsProgrammeType /*aPty*/  )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroRdsDataPTY")));
}


// ===================================================================
// CRadioUtilityTestClass::MrroRdsDataPS
/**
 * New Programme Service(PS) is available. This event is asynchronous and is received after
 * a call to CRadioRdsUtility::NotifyRdsDataChange. Client will continue to receive this
 * notification whenever new PS is available until clients unsubscribes.
 *
 * @since S60 3.2
 * @param aPs Programme service
 */
// ===================================================================
void CRadioUtilityTestClass::MrroRdsDataPS( TRdsPSName& /*aPs*/  )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroRdsDataPS")));
}


// ===================================================================
// CRadioUtilityTestClass::MrroRdsDataRT
/**
 * New Radio Text(RT) is available. This event is asynchronous and is received after
 * a call to CRadioRdsUtility::NotifyRdsDataChange. Client will continue to receive this
 * notification whenever new RT is available until clients unsubscribes.
 *
 * @since S60 3.2
 * @param aRt Radio text
 */
// ===================================================================
void CRadioUtilityTestClass::MrroRdsDataRT( TRdsRadioText& /*aRt*/  )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroRdsDataRT")));
}


// ===================================================================
// CRadioUtilityTestClass::MrroRdsDataCT
/**
 * Clock Time(CT) is available. This event is asynchronous and is received after a call
 * to CRadioRdsUtility::NotifyRdsDataChange. Client will continue to receive this notification
 * whenever new CT is available until clients unsubscribes.
 *
 * @since S60 3.2
 * @param aCt Clock time
 */
// ===================================================================
void CRadioUtilityTestClass::MrroRdsDataCT( TDateTime& /*aCt */ )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroRdsDataCT")));
}


// ===================================================================
// CRadioUtilityTestClass::MrroRdsDataTA
/**
 * Called when Traffice Announcement(TA) status changes. This event is asynchronous
 * and is received after a call to CRadioRdsUtility::NotifyRdsDataChange. Client will continue
 * to receive this notification whenever TA status changes until clients unsubscribes.
 *
 * @since S60 3.2
 * @param aTaOn ETrue indicates that Traffic Announcement is on.
 */
// ===================================================================
void CRadioUtilityTestClass::MrroRdsDataTA( TBool /*aTaOn */ )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroRdsDataTA")));
}


// ===================================================================
// CRadioUtilityTestClass::MrroRdsDataRTplus
/**
 * New Radio Text+(RT+) is available. This event is asynchronous and is received after
 * a call to CRadioRdsUtility::NotifyRadioTextPlusChange. Client will continue to
 * receive this notification whenever new RT+ is available until clients unsubscribes.
 *
 * @since S60 3.2
 * @param aRtPlusClass Radio text plus class
 * @param aRtPlusData Radio text plus data
 */
// ===================================================================
void CRadioUtilityTestClass::MrroRdsDataRTplus( TRdsRTplusClass /*aRtPlusClass*/ , TRdsRadioText& /*aRtPlusData*/ )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroRdsDataRTplus")));
}


// ===================================================================
// CRadioUtilityTestClass::MrroRdsSearchBeginAF
/**
 * Called when tuner starts to search for an Alternate Frequency(AF) with stronger signal.
 * This event is asynchronous and is received after a call to
 * CRadioRdsUtility::SetAutomaticSwitching. Client will continue to receive this notification
 * whenever new search is started until clients turns automatic switching off.
 *
 * @since S60 3.2
 */
// ===================================================================
void CRadioUtilityTestClass::MrroRdsSearchBeginAF()
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroRdsSearchBeginAF")));
}


// ===================================================================
// CRadioUtilityTestClass::MrroRdsSearchEndAF
/**
 * Called when search for an Alternate Frequency(AF) ends. This event is asynchronous and
 * is received after a call to CRadioRdsUtility::SetAutomaticSwitching. Client will continue
 * to receive this notification until clients turns automatic switching off.
 *
 * Most common error codes:
 * KErrNone - Successful, new frequency found
 * KErrNotFound - Ended, but frequency hasn't changed
 * KErrCancel - Cancelled
 *
 * @since S60 3.2
 * @param aError A standard system error code or RDS control error (TRdsControlError).
 * @param aFrequency The frequency(Hz) of the radio station that was found.
 */
// ===================================================================
void CRadioUtilityTestClass::MrroRdsSearchEndAF( TInt /*aError*/ , TInt /*aFrequency*/  )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroRdsSearchEndAF")));
}


// ===================================================================
// CRadioUtilityTestClass::MrroRdsStationChangeTA
/**
 * Called when the tuner has re-tuned to another frequency that is broadcasting
 * Traffic Announcement(TA). This notification sent only when the client has turned the
 * automatic traffic announcement on.
 *
 * @since S60 3.2
 * @param aFrequency The frequency(Hz) of the radio station that was found.
 */
// ===================================================================
void CRadioUtilityTestClass::MrroRdsStationChangeTA( TInt /*aFrequency */ )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroRdsStationChangeTA")));
}


// ===================================================================
// CRadioUtilityTestClass::MrroRdsEventAutomaticSwitchingChange
/**
 * Called when automatic switching (AF) setting changes.
 *
 * @since S60 3.2
 * @param aAuto ETrue indicates that automatic switching is on.
 */
// ===================================================================
void CRadioUtilityTestClass::MrroRdsEventAutomaticSwitchingChange( TBool /*aAuto*/  )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroRdsEventAutomaticSwitchingChange")));
}


// ===================================================================
// CRadioUtilityTestClass::MrroRdsEventAutomaticTrafficAnnouncement
/**
 * Called when automatic traffic announcement setting changes.
 *
 * @since S60 3.2
 * @param aAuto ETrue indicates that automatic traffic announcement is on.
 */
// ===================================================================
void CRadioUtilityTestClass::MrroRdsEventAutomaticTrafficAnnouncement( TBool /*aAuto*/  )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroRdsEventAutomaticTrafficAnnouncement")));
}


// ===================================================================
// CRadioUtilityTestClass::MrroRdsEventSignalChange
/**
 * Called when RDS signal status change (i.e. signal is lost/restored).
 *
 * @since S60 3.2
 * @param aSignal ETrue indicates that RDS signal is available in the tuned frequency.
 */
// ===================================================================
void CRadioUtilityTestClass::MrroRdsEventSignalChange( TBool /*aSignal */ )
{
	FTRACE(FPrint(_L("CRadioUtilityTestClass::MrroRdsEventSignalChange")));
}

// FM Preset Callback

// ===================================================================
// CRadioUtilityTestClass::MrpeoPresetChanged
/**
 * Called when a preset changes.
 *
 * @since S60 3.2
 * @param aChange Change event type
 * @param aIndex Index to the preset that has changed. Zero means all presets.
 */
// ===================================================================
void CRadioUtilityTestClass::MrpeoPresetChanged( TPresetChangeEvent aChange, TInt aIndex )
{
    FTRACE(FPrint(_L("CRadioUtilityTestClass::MrpeoPresetChanged")));
    iLog->Log(_L("MrpeoPresetChanged: Change Event = %d, Index = %d."), aChange, aIndex);
}
	
