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


#include "PlayBalance.h"

#include "MdaAudioSampleEditor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CPlayBalance::CPlayBalance(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
}

CPlayBalance::~CPlayBalance()
{

	if (recorder) delete recorder;
	recorder = NULL;
}


void CPlayBalance::ConstructL(TFileName* fileName)
{
	logger->Log(_L("Creating file: %S"),fileName);
	recorder = CMdaAudioRecorderUtility::NewL(*this);
	if (recorder)
	{
		TRAPD(err,recorder->OpenFileL(*fileName));
		if (err != KErrNone)
		{
			logger->Log(_L("Error %d opening file: %S"),err,fileName);
			User::Leave(KErrRecOpenFile);
		}
	}
}

TInt CPlayBalance::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
{
	TInt error = KErrNone;
	aLogger->Log(_L("Creating scheduler"));

	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	TFileName fileName;

	//Get file name
	TPtrC name;
	CStifItemParser* item = aParser->GetItemLineL(KFileName, ENoTag);
	if (!item)
	{
		CleanupStack::PopAndDestroy();
		return KErrSyntax;
	}

	CleanupStack::PushL(item);
	if (item->GetString(KEmptyString,name) != KErrNone)
	{
		aLogger->Log(_L("Invalid file name"));
		CleanupStack::PopAndDestroy(2);
		return KErrSyntax;
	}
	CleanupStack::PopAndDestroy();

	GetFileName(name,&fileName);


	//Obtain balances(s) from parser
	TInt expectedBalance1 = 0;
	item = aParser->GetNextItemLineL(KBalance);

	if (item)
	{
		CleanupStack::PushL(item);
		if (item->GetInt(KBalance,expectedBalance1) != KErrNone)
		{
			aLogger->Log(_L("expectedBalance syntax error"));
		}
		CleanupStack::PopAndDestroy(); // item
	}
	else
	{
		error = KErrSetPlayBalance;
	}


	if (error != KErrNone)
	{
		CleanupStack::PopAndDestroy();  //scheduler
		return error;
	}


	// create CPlayBalance
	aLogger->Log(_L("Creating CPlayBalance"));

	CPlayBalance* selfObj = CPlayBalance::NewL(aConsole, aLogger, &fileName);

	selfObj->expectedBalance = expectedBalance1;

	CleanupStack::PushL(selfObj);

	CActiveScheduler::Start();

//	if (selfObj->windowStart != startPos)
//	{
//		aLogger->Log(_L("The window start position is not right after SetWindow: %d"),selfObj->windowStart);
	//	error = KErrDiffVolume;
//	}


  //  TTimeIntervalMicroSeconds expectedPlayTime(Abs(endPos.Int64() - startPos.Int64()));

//	LogTime(_L("expectedPlayTime:"),expectedPlayTime,aLogger);
	if (Abs(selfObj->expectedBalance - expectedBalance1)> 2)
	{
		aLogger->Log(_L("GetPlaybackBalance differs from SetPlaybackBalance"));
		error = KErrSetPlayBalance;
	}

	CleanupStack::PopAndDestroy(2); // schedule, selfObj

	return error;
}


CPlayBalance* CPlayBalance::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger, TFileName* fileName)
{
	CPlayBalance* self = new (ELeave) CPlayBalance(aConsole, aLogger);
    CleanupStack::PushL(self);

 //   self->ConstructL(fileName);
	TRAPD(err, self->ConstructL(fileName));
	if (err != KErrNone)
	{
		CleanupStack::PopAndDestroy();
		return NULL;
	}

	CleanupStack::Pop(self);
    return self;
}

void CPlayBalance::MoscoStateChangeEvent(CBase* /*aObject*/, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CPlayBalance::MoscoStateChangeEvent"));
#endif

	logger->Log(_L("MoscoStateChangeEvent called, error: %d	prev: %d curr : %d"),aErrorCode,aPreviousState,aCurrentState);

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == 0)
	{
		// Initialize
		recorder->SetAudioDeviceMode(CMdaAudioRecorderUtility::ELocal);
		logger->Log(_L("Mode set"));
		recorder->SetGain(recorder->MaxGain());
		logger->Log(_L("Gain set"));
		recorder->SetVolume(recorder->MaxVolume());
		logger->Log(_L("Volume set"));
		recorder->SetPosition(TTimeIntervalMicroSeconds(TInt64(0)));
		logger->Log(_L("Position set"));

		recorder->SetThreadPriorityPlayback(EPriorityNormal);
		logger->Log(_L("ThreadPriorityPlayback set"));
		recorder->SetThreadPriorityRecord(EPriorityNormal);
		logger->Log(_L("ThreadPriorityPlayRecord set"));

		recorder->ClearPlayWindow();
		logger->Log(_L("ClearPlayWindow set"));

		recorder->CancelRegisterAudioResourceNotification(KUidInterfaceMMFController);
		logger->Log(_L("CancelRegisterAudioResourceNotification set"));



		//	TInt expectedPlayTime;
//		TTimeIntervalMicroSeconds expectedPlayTime = Abs(windowEnd.Int64() - windowStart.Int64());
	//	LogTime(_L("Recorded clip duration:"),recorder->Duration());
		logger->Log(_L("SetPlaybackBalance before playing: %d"),expectedBalance);
		recorder->SetPlaybackBalance(expectedBalance);


	//	TTime time;
	//	time.UniversalTime();
	//	startTime = time.Int64();

		recorder->PlayL();


		return;
	}

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == CMdaAudioClipUtility::EPlaying)
	{
		//get position after play window, which should be start position
	//	windowStart = recorder->Position();

	//	TTime time;
	//	time.UniversalTime();
	//	endTime = time.Int64();
		recorder->GetPlaybackBalance(expectedBalance);
		logger->Log(_L("GetPlaybackBalance. Get"));

		CActiveScheduler::Stop();
	}
	if (aErrorCode != KErrNone)
	    {
	    expectedBalance = 0;
	    CActiveScheduler::Stop();   
	    }

	return;

}



void CPlayBalance::GetFileName(TPtrC path, TFileName* fileName)
{

	TParse p;
	p.Set(path,NULL,NULL);
	fileName->Append(p.DriveAndPath());
	fileName->Append(p.Name());

	fileName->Append(p.Ext());
}