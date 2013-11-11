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


#include "RecordBitRate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRecordBitRate::CRecordBitRate(CTestModuleIf *aConsole, CStifLogger *aLogger):CRecordingBase(aConsole,aLogger)
{
	isBitRateTest = false;
}

CRecordBitRate::~CRecordBitRate()
{

}

CRecordingBase* CRecordBitRate::CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	//aLogger->Log(_L("CRecordClip::CreateObjectL"));
	return (new (ELeave) CRecordBitRate(aConsole, aLogger));
}

TInt CRecordBitRate::RunTestL(CTestModuleIf *aConsole,
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
	                                             clipCounter, CRecordBitRate::CreateObjectL,
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

TInt CRecordBitRate::PreProcess(CStifSectionParser* aParser)
{
//	if (recorder && iFirstTime)
//	{
//		iFirstTime = false;
//	}
	TInt error = KErrNone;
	isBitRateTest = true;
		//Get recorded audio format
	TPtrC dataFormat(KFormatAmr);
//	bool isWav,isMp4,isAmr;
	isAmr = true;
	CStifItemParser *item = aParser->GetItemLineL(KDataFormat);
	if (item)
	{
		CleanupStack::PushL(item);
		error = item->GetString(KDataFormat,dataFormat);
		if (error != KErrNone)
		{
			CleanupStack::PopAndDestroy(); // item
			logger->Log(_L("dataFormat syntax error"));
			return error;
		}
		else
		{
			logger->Log(_L("dataFormat: %S"),&dataFormat);
			if (dataFormat == KFormatWav)
			{
				isWav = true;
				isAmr = false;
			}
			else
			{
				if (dataFormat == KFormatMp4)
				{
					isMp4 = true;
					isAmr = false;
				}
			}

			CleanupStack::PopAndDestroy(); // item
		}
	}


	return KErrNone;
}

TInt CRecordBitRate::PostProcess(CStifSectionParser* /*aParser*/)
{
	TInt error = KErrNone;

	if ( iCurrBitRate != expectedBitRate)
	{
		logger->Log(_L("BitRate differs after recording curr:%d exp:%d"), iCurrBitRate, expectedBitRate);
		error = KErrOutOfRange;
	}

	return error;
//	return KErrNone;
}


void CRecordBitRate::MoscoStateChangeEvent(CBase* aObject,
                                           TInt aPreviousState,
                                           TInt aCurrentState,
                                           TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CRecordBitRate::MoscoStateChangeEvent"));
#endif
	logger->Log(_L("CRecordBitRate::MoscoStateChangeEvent called, error: %d	prev: %d curr : %d"),
	               aErrorCode,aPreviousState,aCurrentState);

	TInt err = KErrNone;
	RArray<TUint> supportedBitRates;
	if(recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen)
	{
		if (isAmr)
		{
			switch(expectedBitRate)
			{
				case 4750:
					TRAP(err, recorder->SetDestinationBitRateL(KAmrBitRate4750));
					break;
				case 5150:
					TRAP(err, recorder->SetDestinationBitRateL(KAmrBitRate5150));
					break;
				case 5900:
					TRAP(err, recorder->SetDestinationBitRateL(KAmrBitRate5900));
					break;
				case 6700:
					TRAP(err, recorder->SetDestinationBitRateL(KAmrBitRate6700));
					break;
				case 7400:
					TRAP(err, recorder->SetDestinationBitRateL(KAmrBitRate7400));
					break;
				case 7950:
					TRAP(err, recorder->SetDestinationBitRateL(KAmrBitRate7950));
					break;
				case 10200:
					TRAP(err, recorder->SetDestinationBitRateL(KAmrBitRate10200));
					break;
				case 12200:
					TRAP(err, recorder->SetDestinationBitRateL(KAmrBitRate12200));
					break;

				default:err = KErrInvalidId;
			}

			logger->Log(_L("Set expectedBitRate: %d"),expectedBitRate);
			TRAP(err, iCurrBitRate = recorder->DestinationBitRateL() );
			logger->Log(_L("Get expectedBitRate after set BitRate: %d"),iCurrBitRate);
		}
		else
		{
			logger->Log(_L("GetSupportedBitRatesL "));
			if (recorder)
			{
				TRAP(err, recorder->GetSupportedBitRatesL(supportedBitRates) );

				if (err != KErrNone)
				{
					logger->Log(_L("Error %d for GetSupportedBitRateL"),err );
					callbackErr = err;
					return;
				}
			}

			TInt j;
			for (j=0; j<supportedBitRates.Count(); j++)
			{

				expectedBitRate = supportedBitRates[j];
				logger->Log(_L("[%d]expectedBitRate  %d"), j, expectedBitRate);

				TRAP(err, recorder->SetDestinationBitRateL(expectedBitRate) );
				if (err)
				{
					logger->Log(_L("Error %d for SetDestinationBitRateL %d"),err, expectedBitRate );
					callbackErr = err;
					break;
				}

				TRAP(err, iCurrBitRate = recorder->DestinationBitRateL());
				logger->Log(_L("[%d]Get expectedBitRate after set BitRate: %d"), j,iCurrBitRate);

				if (expectedBitRate != iCurrBitRate)
				{
					callbackErr = KErrOutOfRange;
					break;
				}
			}
		}

	}

	if (isAmr && aErrorCode == KErrNone)
	{
		CRecordingBase::MoscoStateChangeEvent(aObject, aPreviousState, aCurrentState, aErrorCode);
	}
	else
	{
		if (aErrorCode != KErrNone)
		{
			callbackErr = aErrorCode;
		}
		CActiveScheduler::Stop();
	}

	return;

}
