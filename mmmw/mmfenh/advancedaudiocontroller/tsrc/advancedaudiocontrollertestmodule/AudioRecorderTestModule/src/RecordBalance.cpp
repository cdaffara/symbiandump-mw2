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

#include "RecordBalance.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRecordBalance::CRecordBalance(CTestModuleIf *aConsole, CStifLogger *aLogger):CRecordingBase(aConsole,aLogger)
{
	firstTime = true;
}

CRecordBalance::~CRecordBalance()
{

}

CRecordingBase* CRecordBalance::CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	//aLogger->Log(_L("CRecordClip::CreateObjectL"));
	return (new (ELeave) CRecordBalance(aConsole, aLogger));
}

TInt CRecordBalance::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter)
{
	TInt error = KErrNone;

	//Create scheduler
	aLogger->Log(_L("Creating scheduler"));
	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	//Create recorder
	CRecordingBase* recorderObj;
	if ((error = CRecordingBase::CreateRecorderL(aConsole, aLogger, aParser, clipCounter, CRecordBalance::CreateObjectL, recorderObj)) != KErrNone)
	{
		CleanupStack::PopAndDestroy(); //scheduler
		return error;
	}
	CleanupStack::PushL(recorderObj);

	aLogger->Log(_L("Balance center: %d"),KMMFBalanceCenter);
	aLogger->Log(_L("Balance max left: %d"),KMMFBalanceMaxLeft);
	aLogger->Log(_L("Balance max right: %d"),KMMFBalanceMaxRight);


	error = CRecordingBase::SimpleRecordL(aConsole, aLogger, recorderObj, true, aParser);

	CleanupStack::PopAndDestroy(2); // recorder, scheduler
	return error;
}

TInt CRecordBalance::PreProcess(CStifSectionParser* aParser)
{
	TInt err = KErrNone;
	if (recorder && firstTime)
	{
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

TInt CRecordBalance::PostProcess(CStifSectionParser* /*aParser*/)
{
	TInt error = KErrNone;

	if (( Abs(currBalance - expectedBalance) > 2 ) && (expectedBalance >= KMMFBalanceMaxLeft) && (expectedBalance <= KMMFBalanceMaxRight))
	{
		logger->Log(_L("Balance differs after recording"));
		error = KErrOutOfRange;
	}

	if (((expectedBalance < KMMFBalanceMaxLeft) && (currBalance != origBalance)) || ((expectedBalance > KMMFBalanceMaxRight) && (currBalance != origBalance)))
	{
		logger->Log(_L("expectedBalance: %d origBalance: %d currBalance: %d"),expectedBalance, origBalance, currBalance);
		error = KErrAPI;
	}

	return error;
//	return KErrNone;
}


void CRecordBalance::MoscoStateChangeEvent(CBase* aObject, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CRecordBalance::MoscoStateChangeEvent"));
#endif
	if(recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen)
//	if (aErrorCode == KErrOverflow || ( aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen &&  aPreviousState == CMdaAudioClipUtility::ERecording ))
	{
	//	logger->Log(_L("Balance center: %d"),KMMFBalanceCenter);
	//	logger->Log(_L("Balance max left: %d"),KMMFBalanceMaxLeft);
	//	logger->Log(_L("Balance max right: %d"),KMMFBalanceMaxRight);

		if (expectedBalance != -999)
		{
			recorder->GetRecordBalance(origBalance);
			logger->Log(_L("Get origBalance: %d"),origBalance);
			recorder->SetRecordBalance(expectedBalance);
			logger->Log(_L("Set expectedBalance: %d"),expectedBalance);
			recorder->GetRecordBalance(currBalance);
			logger->Log(_L("Get expectedBalance after set Balance: %d"),currBalance);
		}

	}

	CRecordingBase::MoscoStateChangeEvent(aObject, aPreviousState, aCurrentState, aErrorCode);
}
