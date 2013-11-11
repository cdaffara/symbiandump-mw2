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
* Description:  AudioRecorder Test
*
*/

#include "RecordSampleRate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRecordSampleRate::CRecordSampleRate(CTestModuleIf *aConsole, CStifLogger *aLogger):CRecordingBase(aConsole,aLogger)
{
//	iFirstTime = true;
	iCurrSampleRate = 0;
}

CRecordSampleRate::~CRecordSampleRate()
{

}

CRecordingBase* CRecordSampleRate::CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	//aLogger->Log(_L("CRecordClip::CreateObjectL"));
	return (new (ELeave) CRecordSampleRate(aConsole, aLogger));
}

TInt CRecordSampleRate::RunTestL(CTestModuleIf *aConsole,
                              CStifLogger *aLogger,
                              CStifSectionParser *aParser,
                              TInt* clipCounter)
{
	TInt error = KErrNone;

	//Create scheduler
	aLogger->Log(_L("Creating scheduler"));
	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	//Create recorder
	CRecordingBase* recorderObj;
	if ((error = CRecordingBase::CreateRecorderL(aConsole, aLogger, aParser,
	                                             clipCounter, CRecordSampleRate::CreateObjectL,
	                                             recorderObj)) != KErrNone)
	{
		CleanupStack::PopAndDestroy(); //scheduler
		return error;
	}
	CleanupStack::PushL(recorderObj);

	error = CRecordingBase::SimpleRecordL(aConsole, aLogger, recorderObj, true, aParser);

	CleanupStack::PopAndDestroy(2); // recorder, scheduler
	return error;
}

TInt CRecordSampleRate::PreProcess(CStifSectionParser* /*aParser*/)
{
	return KErrNone;
}

TInt CRecordSampleRate::PostProcess(CStifSectionParser* /*aParser*/)
{
	TInt error = KErrNone;

	if ( iCurrSampleRate != expectedSampleRate)
	{
		logger->Log(_L("SampleRate differs after recording curr:%d exp:%d"), iCurrSampleRate, expectedSampleRate);
		error = KErrOutOfRange;
	}

	return error;
}


void CRecordSampleRate::MoscoStateChangeEvent(CBase* aObject,
                                           TInt aPreviousState,
                                           TInt aCurrentState,
                                           TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CRecordSampleRate::MoscoStateChangeEvent"));
#endif
	logger->Log(_L("CRecordSampleRate::MoscoStateChangeEvent called, error: %d	prev: %d curr : %d"),
	               aErrorCode,aPreviousState,aCurrentState);

	TInt err = KErrNone;
	RArray<TUint> supportedSampleRates;

	if(recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen)
	{
		TRAP(err, recorder->GetSupportedSampleRatesL(supportedSampleRates) );
		if (err != KErrNone)
		{
			logger->Log(_L("Error %d for GetSupportedSampleRateL"),err );
			return;
		}

		TInt j;
		for (j=0; j<supportedSampleRates.Count(); j++)
		{
			TUint supportedSR = supportedSampleRates[j];
			if (supportedSR == expectedSampleRate)
			{
				TRAPD(err, recorder->SetDestinationSampleRateL(supportedSR) );
				if (err)
				{
					logger->Log(_L("Error %d for SetDestinationSampleRateL %d"),err, supportedSR );
					return;
				}
				break;
			}
		}

		if (j>= supportedSampleRates.Count())
		{
			logger->Log(_L("expectedSampleRate %d is not supported by system, invalid test case."), expectedSampleRate );
			callbackErr = KErrInvalidId;
			return;
		}

		iCurrSampleRate = recorder->DestinationSampleRateL();
		logger->Log(_L("Get CurrSampleRate after set SampleRate: %d"),iCurrSampleRate);
	}

	CRecordingBase::MoscoStateChangeEvent(aObject, aPreviousState, aCurrentState, err);
}
