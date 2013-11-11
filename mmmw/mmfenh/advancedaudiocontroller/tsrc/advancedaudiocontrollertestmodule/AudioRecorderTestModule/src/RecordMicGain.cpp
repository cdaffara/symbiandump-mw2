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

#include "RecordMicGain.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecordMicGain::CRecordMicGain(CTestModuleIf *aConsole, CStifLogger *aLogger):CRecordingBase(aConsole,aLogger)
{
	firstTime = true;
	theMaxGain = 0;
	expectedGain = 0;
	currGain = 0;
}

CRecordMicGain::~CRecordMicGain()
{

}

CRecordingBase* CRecordMicGain::CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	//aLogger->Log(_L("CRecordClip::CreateObjectL"));
	return (new (ELeave) CRecordMicGain(aConsole, aLogger));
}

TInt CRecordMicGain::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter)
{
	TInt error = KErrNone;

	//Create scheduler
	aLogger->Log(_L("Creating scheduler"));
	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	//Create recorder
	CRecordingBase* recorderObj;
	if ((error = CRecordingBase::CreateRecorderL(aConsole, aLogger, aParser, clipCounter, CRecordMicGain::CreateObjectL, recorderObj)) != KErrNone)
	{
		CleanupStack::PopAndDestroy(); //scheduler
		return error;
	}
	CleanupStack::PushL(recorderObj);

//	TInt maxGain = recorderObj->recorder->MaxGain();
//	aLogger->Log(_L("Maximum gain: %d"),maxGain);

//	while (true && (error==KErrNone))
//	{
		//Obtain relative gain(s) from parser and execute test
//		TInt relativeGain = 0;
//		CStifItemParser* item = aParser->GetNextItemLineL(KRelativeGain);
//		CleanupStack::PushL(item);
//		if (item)
//			if (item->GetInt(KRelativeGain,relativeGain) != KErrNone)
//				continue;
//		CleanupStack::PopAndDestroy(); // item

//		if (!item) break;

		//Calculate expected gain
//		TInt expectedGain = maxGain*relativeGain/100;
	//	aLogger->Log(_L("Relative gain: %d, Value: %d"),relativeGain,expectedGain);

	//	recorderObj->recorder->SetGain(expectedGain);

		error = CRecordingBase::SimpleRecordL(aConsole, aLogger, recorderObj, true, aParser);

/*		if (error == KErrNone)
		{
			TInt gain;

			if ((error = recorderObj->recorder->GetGain(gain)) == KErrNone)
			{
				aLogger->Log(_L("Gain after recording: %d"),gain);

				if ((gain != expectedGain) && (expectedGain >=0) && (expectedGain <= maxGain))
				{
					aLogger->Log(_L("Gain differs after recording"));
					error = KErrOutOfRange;
				}
				if (((expectedGain < 0) && (gain != 0)) || ((expectedGain > maxGain) && (gain != maxGain)))
				{
					aLogger->Log(_L("Gain 0 or %d (maxGain)"),maxGain);
					error = KErrAPI;
				}
			}
			else
			{
				aLogger->Log(_L("Error obtaining gain: %d"),error);
				error = KErrGetGain;
			}
		}*/
	//}

	CleanupStack::PopAndDestroy(2); // recorder, scheduler
	return error;
}

TInt CRecordMicGain::PreProcess(CStifSectionParser* aParser)
{
	TInt err = KErrNone;
	if (recorder && firstTime)
	{
	//	recorder->SetAudioDeviceMode(CMdaAudioRecorderUtility::ELocal);
	//	recorder->SetPosition(TTimeIntervalMicroSeconds(TInt64(0)));
	//	TRAPD(err,recorder->CropL());
	//	if (err != KErrNone) logger->Log(_L("Error cropping file: %d"),err);
		firstTime = false;

		//Get recorded audio format
		TPtrC dataFormat(KPPNone);
		CStifItemParser* item = aParser->GetItemLineL(KDataFormat);
		if (item)
		{
			CleanupStack::PushL(item);
			err = item->GetString(KDataFormat,dataFormat);
			if (err != KErrNone)
			{
				CleanupStack::PopAndDestroy(); // item
				logger->Log(_L("dataFormat syntax error"));
				return err;
			}
			else
			{
				logger->Log(_L("dataFormat: %S"),&dataFormat);
				CleanupStack::PopAndDestroy(); // item
			}
		}

		if (dataFormat == KFormatMp4)
		{
			isMp4 = true;
		}
	}
	return KErrNone;
}

TInt CRecordMicGain::PostProcess(CStifSectionParser* /*aParser*/)
{
	TInt error = KErrNone;

	if ((currGain != expectedGain) && (expectedGain >=0) && (expectedGain <= theMaxGain))
	{
		logger->Log(_L("Gain differs after recording"));
		error = KErrOutOfRange;
	}

	if (((expectedGain < 0) && (currGain != 0)) || ((expectedGain > theMaxGain) && (currGain != theMaxGain)))
	{
		logger->Log(_L("Gain 0 or %d (maxGain)"),theMaxGain);
		error = KErrAPI;
	}

	return error;
}

void CRecordMicGain::MoscoStateChangeEvent(CBase* aObject, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CRecordMicGain::MoscoStateChangeEvent"));
#endif
	if(recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen)
//	if (aErrorCode == KErrOverflow || ( aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen &&  aPreviousState == CMdaAudioClipUtility::ERecording ))
	{
		theMaxGain = recorder->MaxGain();
		logger->Log(_L("the max Gain is : %d"),theMaxGain);
		if (relativeGain != 0)
		{
		//	TInt maxGain = recorder->MaxGain();
			expectedGain = theMaxGain*relativeGain/100;
			recorder->SetGain(expectedGain);
			logger->Log(_L("Set expectedGain: %d"),expectedGain);
			recorder->GetGain(currGain);
			logger->Log(_L("Get Gain after set Gain: %d"),currGain);
		}

	}

	CRecordingBase::MoscoStateChangeEvent(aObject, aPreviousState, aCurrentState, aErrorCode);
}
