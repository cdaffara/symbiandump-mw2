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
* Description:  Active object for each asynchronous request in RRadioSession.
*				 Notifies MRadioObserver upon request completion.
*
*/



// INCLUDE FILES
#include    "RadioRequest.h"
#include    "RadioDebug.h"
#include    "trace.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRadioRequest::CRadioRequest
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CRadioRequest::CRadioRequest(
	RRadioSession& aSession,
	MRadioObserver& aObserver,
	TRadioServerRequest aRequest )
	:	CActive(EPriorityStandard),
		iSession(aSession),
		iObserver(aObserver),
		iRequest(aRequest),
		iBool(EFalse),
		iBoolPckg(iBool),
		iInt(0),
		iIntPckg(iInt)
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CRadioRequest::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRadioRequest::ConstructL()
    {
    FUNC_LOG;
	CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CRadioRequest::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRadioRequest* CRadioRequest::NewLC(
	RRadioSession& aSession,
	MRadioObserver& aObserver,
	TRadioServerRequest aRequest )
    {
    FUNC_LOG;
    CRadioRequest* self = new( ELeave ) CRadioRequest(aSession, aObserver, aRequest);
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor
CRadioRequest::~CRadioRequest()
    {
    FUNC_LOG;
    INFO_1("My pointer: 0x%x", this );
	if ( IsActive() )
		{
		Cancel();
		}
    }

// -----------------------------------------------------------------------------
// CRadioRequest::CompleteRequest
// This method completes the request status and sets the object active
// to provide asynchronous behavior.
// -----------------------------------------------------------------------------
//
void CRadioRequest::CompleteRequest(
	TRadioServerRequest aRequest,
	TInt aError )
    {
    FUNC_LOG;
	iRequest = aRequest;

	TRequestStatus* stat = &iStatus;
    User::RequestComplete(stat, aError);
	SetActive();
    }

// -----------------------------------------------------------------------------
// CRadioRequest::DoCancel
// -----------------------------------------------------------------------------
//
void CRadioRequest::DoCancel()
    {
    FUNC_LOG;
	iSession.CancelRequest(iRequest);
    }

// -----------------------------------------------------------------------------
// CRadioRequest::RunL
// -----------------------------------------------------------------------------
//
void CRadioRequest::RunL()
    {
    FUNC_LOG;
	TInt err = iStatus.Int();
	if ( err == KErrCancel )
		{
		// Ignore the cancelled request
		INFO_1("cancelled iRequest [%d]", iRequest);
		return;
		}

	switch( iRequest )
		{
		// Place the most frequent ones on top
		case ERadioServStationSeek:
			{
			iInt = iIntPckg();
			INFO_1("ERadioServStationSeek [%d]", iInt);
			iObserver.StationSeekComplete(err, iInt);
			}
			break;
		case ERadioServSetFrequency:
			{
			INFO("ERadioServSetFrequency");
			iObserver.SetFrequencyComplete(err);
			}
			break;
		case ERadioServRequestTunerControl:
			{
			INFO("ERadioServRequestTunerControl");
			iObserver.RequestTunerControlComplete(err);
			}
			break;
		case ERadioServSetFrequencyRange:
			{
			INFO("ERadioServSetFrequencyRange");
			iObserver.SetFrequencyRangeComplete(err);
			}
			break;
		case ERadioServPlay:
			{
			INFO("INFOERadioServPlay");
			if ( err != KErrNone )
				{
				iObserver.RadioEventStateChange(EFalse, err);
				}
			}
			break;
		case ERadioServStationSeekByPTY:
			{
			iInt = iIntPckg();
			INFO_1("ERadioServStationSeekByPTY [%d]", iInt);
			iObserver.StationSeekByPTYComplete(err, iInt);
			}
			break;
		case ERadioServStationSeekByTA:
			{
			iInt = iIntPckg();
			INFO_1("ERadioServStationSeekByTA [%d]", iInt);
			iObserver.StationSeekByTAComplete(err, iInt);
			}
			break;
		case ERadioServStationSeekByTP:
			{
			iInt = iIntPckg();
			INFO_1("ERadioServStationSeekByTP [%d]", iInt);
			iObserver.StationSeekByTPComplete(err, iInt);
			}
			break;
		case ERadioServGetFreqByPTY:
			{
			RArray<TInt> freqList(1);
			iObserver.GetFreqByPTYComplete(err, freqList);
			}
			break;
		case ERadioServGetFreqByTA:
			{
			RArray<TInt> freqList(1);
			iObserver.GetFreqByTAComplete(err, freqList);
			}
			break;
		case ERadioServGetPSByPTY:
			{
			RArray<TRsRdsPSName> psList(1);
			iObserver.GetPSByPTYComplete(err, psList);
			}
			break;
		case ERadioServGetPSByTA:
			{
			RArray<TRsRdsPSName> psList(1);
			iObserver.GetPSByTAComplete(err, psList);
			}
			break;
		default:
			{
			INFO("ERROR case default !!!");
			User::Panic(_L("RadioServer"), KErrGeneral );
			break;
			}
		}
    }

//  End of File

