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
* Description:  This class implements an interface to FM tuner.
*
*/



// INCLUDE FILES
#include    "RadioServerFMTuner.h"
#include    "RadioDebug.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::CRadioServerFMTuner
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CRadioServerFMTuner::CRadioServerFMTuner(
	MRadioServerFMTunerObserver& aObserver )
	:	CActive(EPriorityStandard),
		iObserver(aObserver),
		iFmTunerQue(_FOFF( TRadioFmTunerData, iLink )),
		iFmTunerData(NULL)
    {
    }

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::ConstructL()
    {
	RADIO_RDEBUG(_L("[RADIO-SVR] CRadioServerFMTuner::ConstructL()"));
	CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRadioServerFMTuner* CRadioServerFMTuner::NewL(
	MRadioServerFMTunerObserver& aObserver )
    {
    CRadioServerFMTuner* self = new( ELeave ) CRadioServerFMTuner(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CRadioServerFMTuner::~CRadioServerFMTuner()
    {
	RADIO_RDEBUG(_L("[RADIO-SVR] CRadioServerFMTuner::~CRadioServerFMTuner()"));
	if ( IsActive() )
		{
		Cancel();
		}

	while ( !iFmTunerQue.IsEmpty() )
		{
		iFmTunerData = iFmTunerQue.First();
		iFmTunerQue.Remove(*iFmTunerData);
		delete iFmTunerData;
		}
    }

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::TunerOnComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::TunerOnComplete(
	TFMRadioTunerError aError )
    {
	CompleteNotification(ETunerEventTunerOn, aError);
    }

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::TunerOffComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::TunerOffComplete(
	TFMRadioTunerError aError )
    {
	CompleteNotification(ETunerEventTunerOff, aError);
    }

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::SetFrequencyRangeComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::SetFrequencyRangeComplete(
	TFMRadioTunerError aError )
    {
	CompleteNotification(ETunerEventSetFrequencyRange, aError);
    }

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::SetFrequencyComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::SetFrequencyComplete(
	TFMRadioTunerError aError )
    {
	CompleteNotification(ETunerEventSetFrequency, aError);
    }

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::StationSeekComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::StationSeekComplete(
	TFMRadioTunerError aError,
	TInt aFrequency )
    {
	iFrequency = aFrequency;
	CompleteNotification(ETunerEventStationSeek, aError);
    }

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::AudioModeComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::AudioModeComplete(
	TFMRadioTunerError aError,
	TFMRadioAudioMode aMode )
    {
	iMode = aMode;
	CompleteNotification(ETunerEventAudioMode, aError);
    }

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::SetAudioModeComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::SetAudioModeComplete(
	TFMRadioTunerError aError )
    {
	CompleteNotification(ETunerEventSetAudioMode, aError);
    }

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::MaxSignalStrengthComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::MaxSignalStrengthComplete(
	TFMRadioTunerError aError,
	TInt aStrength )
    {
	iStrength = aStrength;
	CompleteNotification(ETunerEventMaxSignalStrength, aError);
    }

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::SignalStrengthComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::SignalStrengthComplete(
	TFMRadioTunerError aError,
	TInt aStrength )
    {
	iStrength = aStrength;
	CompleteNotification(ETunerEventSignalStrength, aError);
    }

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::SquelchComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::SquelchComplete(
	TFMRadioTunerError aError,
	TBool aEnabled )
	{
	iEnabled = aEnabled;
	CompleteNotification(ETunerEventSquelch, aError);
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::SetSquelchComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::SetSquelchComplete(
	TFMRadioTunerError aError )
	{
	CompleteNotification(ETunerEventSetSquelch, aError);
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::BufferFilled
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::BufferFilled(
	TDes8& /*aBuffer*/ )
	{
	// Not supported
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::RadioEventAntennaStatusChange
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::RadioEventAntennaStatusChange(
	TBool aAttached )
	{
	iAttached = aAttached;
	CompleteNotification(ETunerEventAntennaStatusChange);
	}

#ifdef RD_FM_RADIO_ENHANCEMENTS

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::RadioEventTunerControlChange
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::RadioEventTunerControlChange(
	TInt aError )
	{
	CompleteNotification(ETunerEventTunerControlChange, aError);
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::StationSeekByPTYComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::StationSeekByPTYComplete(
	TInt aError,
	TInt aFrequency )
	{
	iFrequency = aFrequency;
	CompleteNotification(ETunerEventStationSeekByPTY, aError);
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::StationSeekByTAComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::StationSeekByTAComplete(
	TInt aError,
	TInt aFrequency )
	{
	iFrequency = aFrequency;
	CompleteNotification(ETunerEventStationSeekByTA, aError);
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::StationSeekByTPComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::StationSeekByTPComplete(
	TInt aError,
	TInt aFrequency )
	{
	iFrequency = aFrequency;
	CompleteNotification(ETunerEventStationSeekByTP, aError);
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::GetFreqByPTYComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::GetFreqByPTYComplete(
	TInt /*aError*/,
	RArray<TInt>& /*aFreqList*/ )
	{
	// Not supported
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::GetFreqByTAComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::GetFreqByTAComplete(
	TInt /*aError*/,
	RArray<TInt>& /*aFreqList*/ )
	{
	// Not supported
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::GetPSByPTYComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::GetPSByPTYComplete(
	TInt /*aError*/,
	RArray<TRdsPSName>& /*aPsList*/ )
	{
	// Not supported
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::GetPSByTAComplete
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::GetPSByTAComplete(
	TInt /*aError*/,
	RArray<TRdsPSName>& /*aPsList*/ )
	{
	// Not supported
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::RdsDataPI
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::RdsDataPI(
	TInt aPi )
	{
	iObserver.RdsDataPI(aPi);
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::RdsDataPTY
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::RdsDataPTY(
	TRdsProgrammeType aPty )
	{
	iObserver.RdsDataPTY(aPty);
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::RdsDataPS
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::RdsDataPS(
	TRdsPSName& aPs )
	{
	iObserver.RdsDataPS(aPs);
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::RdsDataRT
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::RdsDataRT(
	TRdsRadioText& aRt,
	RArray<TRdsRTplusTag>& aRTplusTags )
	{
	iObserver.RdsDataRT(aRt, aRTplusTags);
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::RdsDataCT
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::RdsDataCT(
	TDateTime& aCt )
	{
	iObserver.RdsDataCT(aCt);
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::RdsDataTA
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::RdsDataTA(
	TBool aTaOn )
	{
	iObserver.RdsDataTA(aTaOn);
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::RdsSearchBeginAF
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::RdsSearchBeginAF()
	{
	iObserver.RdsSearchBeginAF();
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::RdsSearchEndAF
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::RdsSearchEndAF(
	TInt aError,
	TInt aFrequency )
	{
	iObserver.RdsSearchEndAF(aError, aFrequency);
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::RdsStationChangeTA
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::RdsStationChangeTA(
	TInt /*aFrequency*/ )
	{
	// Not supported
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::RdsEventSignalChange
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::RdsEventSignalChange(
	TBool aSignal )
	{
	iObserver.RdsEventSignalChange(aSignal);
	}

#endif //RD_FM_RADIO_ENHANCEMENTS


// -----------------------------------------------------------------------------
// CRadioServerFMTuner::DoCancel
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::DoCancel()
    {
    }

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::RunL
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::RunL()
    {
	RADIO_RDEBUG_INT2(_L("[RADIO-SVR] CRadioServerFMTuner::RunL(): Err[%d] Event[%d]"), iFmTunerData->iError, iFmTunerData->iEvent);
	TInt error = MapTunerError(iFmTunerData->iError);
	switch ( iFmTunerData->iEvent )
		{
		case ETunerEventTunerOn:
			iObserver.TunerOnComplete(error);
			break;
		case ETunerEventTunerOff:
			iObserver.TunerOffComplete(error);
			break;
		case ETunerEventSetFrequencyRange:
			iObserver.SetFrequencyRangeComplete(error);
			break;
		case ETunerEventSetFrequency:
			iObserver.SetFrequencyComplete(error);
			break;
		case ETunerEventStationSeek:
			iObserver.StationSeekComplete(error, iFrequency);
			break;
		case ETunerEventAudioMode:
			if ( iMode == EFMRadioAudioStereo )
				{
				iObserver.AudioModeComplete(error, ETrue);
				}
			else
				{
				iObserver.AudioModeComplete(error, EFalse);
				}
			break;
		case ETunerEventSetAudioMode:
			iObserver.SetAudioModeComplete(error);
			break;
		case ETunerEventMaxSignalStrength:
			iObserver.MaxSignalStrengthComplete(error, iStrength);
			break;
		case ETunerEventSignalStrength:
			iObserver.SignalStrengthComplete(error, iStrength);
			break;
		case ETunerEventSquelch:
			iObserver.SquelchComplete(error, iEnabled);
			break;
		case ETunerEventSetSquelch:
			iObserver.SetSquelchComplete(error);
			break;
		case ETunerEventAntennaStatusChange:
			iObserver.RadioEventAntennaStatusChange(iAttached);
			break;
		case ETunerEventTunerControlChange:
			iObserver.RadioEventTunerControlChange(error);
			break;
		case ETunerEventStationSeekByPTY:
			iObserver.StationSeekByPTYComplete(error, iFrequency);
			break;
		case ETunerEventStationSeekByTA:
			iObserver.StationSeekByTAComplete(error, iFrequency);
			break;
		case ETunerEventStationSeekByTP:
			iObserver.StationSeekByTPComplete(error, iFrequency);
			break;
		default:
			RADIO_RDEBUG(_L("[RADIO-SVR] CRadioServerFMTuner::RunL(): ERROR case default !!!"));
			break;
		}
	ProcessNextItemInQueue();
	}

// -----------------------------------------------------------------------------
// CRadioServerFMTuner::CompleteNotification
// This method completes the request status and sets the object active
// to provide asynchronous behavior.
// -----------------------------------------------------------------------------
//
void CRadioServerFMTuner::CompleteNotification(
	TTunerEvent aEvent,
	TFMRadioTunerError aError )
    {
	TRadioFmTunerData* data = NULL;
	TRAPD(err, data = new (ELeave) TRadioFmTunerData);
	if ( err != KErrNone )
		{
		RADIO_RDEBUG_INT(_L("[RADIO-SVR] CRadioServerFMTuner::CompleteNotification(): Err[%d]"), err);
		return;
		}
	data->iEvent = aEvent;
	data->iError = aError;
	if ( iFmTunerData )
		{
		// Outstanding request exits; add it to the queue
		iFmTunerQue.AddLast(*data);
		}
	else
		{
		// Set the current outstanding request
		iFmTunerData = data;
		TRequestStatus* stat = &iStatus;
		User::RequestComplete(stat, KErrNone);
		SetActive();
		}
    }

// ----------------------------------------------------
// CRadioServer::ProcessNextItemInQueue
// ----------------------------------------------------
//
void CRadioServerFMTuner::ProcessNextItemInQueue()
	{
	delete iFmTunerData;
	iFmTunerData = NULL;
	if ( !iFmTunerQue.IsEmpty() )
		{
		// there is at least one element in the linked list
		TRadioFmTunerData* data;
		data = iFmTunerQue.First();
		iFmTunerQue.Remove(*data);
		iFmTunerData = data;
		TRequestStatus* stat = &iStatus;
		User::RequestComplete(stat, KErrNone);
		SetActive();
		}
	}

// ----------------------------------------------------
// CRadioServerFMTuner::MapTunerError
// ----------------------------------------------------
//
TInt CRadioServerFMTuner::MapTunerError(
	TFMRadioTunerError aError )
	{
	TInt err;
	switch ( aError )
		{
		case KFMRadioErrHardwareFaulty:
		case KRdsErrHardwareFaulty:
			err = KRadioServErrHardwareFaulty;
			break;
		case KFMRadioErrTuning:
			err = KRadioServErrTuning;
			break;
		case KFMRadioErrFrequencyOutOfBandRange:
			err = KRadioServErrFrequencyOutOfBandRange;
			break;
		case KFMRadioErrFrequencyNotValid:
			err = KRadioServErrFrequencyNotValid;
			break;
		case KFMRadioErrNotPermittedInCurrentState:
		case KRdsErrNotPermittedInCurrentState:
			err = KRadioServErrNotPermittedInCurrentState;
			break;
		case KFMRadioErrDuplicateRequest:
		case KRdsErrDuplicateRequest:
			err = KRadioServErrDuplicateRequest;
			break;
		case KFMRadioErrAntennaNotConnected:
			err = KRadioServErrAntennaNotConnected;
			break;
		case KFMRadioErrFmTransmitterActive:
			err = KRadioServErrFmTransmitterActive;
			break;
		default:
			err = aError;
			break;
		}
	RADIO_RDEBUG_INT2(_L("[RADIO-SVR] CRadioServerFMTuner::MapTunerError() [%d-%d]"), aError, err);
	return err;
	}

//  End of File
