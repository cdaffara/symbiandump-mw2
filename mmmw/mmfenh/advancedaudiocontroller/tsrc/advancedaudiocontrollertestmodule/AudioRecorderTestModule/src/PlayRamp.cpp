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


#include "PlayRamp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CPlayRamp::CPlayRamp(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
}

CPlayRamp::~CPlayRamp()
{

	if (recorder) delete recorder;
	recorder = NULL;
}


void CPlayRamp::ConstructL(TFileName* fileName)
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

TInt CPlayRamp::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
{
	TInt error = KErrNone;
	aLogger->Log(_L("Creating scheduler"));

	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	aLogger->Log(_L("Creating CPlayRamp"));

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

	//Get expected volume
	TInt expectedVol = 0;
	item = aParser->GetItemLineL(KVolume);
	CleanupStack::PushL(item);
	if (item)
		if (item->GetInt(KVolume,expectedVol) != KErrNone)
		{
			aLogger->Log(_L("Invalid expected volume."));
			error = KErrSyntax;
		}
	CleanupStack::PopAndDestroy(); // item
	aLogger->Log(_L("Expected max volume: %d"),expectedVol);


	//Get ramp time
	TTimeIntervalMicroSeconds rampTime = TInt64(0);
	item = aParser->GetItemLineL(KRamp, ENoTag);
	if (item)
	{
		CleanupStack::PushL(item);
		rampTime = ParseTime(item);
		CleanupStack::PopAndDestroy();
	}
	LogTime(_L("Ramp time:"),rampTime.Int64(),aLogger);

	//Get max volume error range
//	TInt errRange = 0;
//	item = aParser->GetItemLineL(KErrorRange);
//	CleanupStack::PushL(item);
//	if (item)
//		if (item->GetInt(KErrorRange,errRange) != KErrNone)
//		{
	//		aLogger->Log(_L("Invalid max volume error range."));
	//		error = KErrSyntax;
//		}
//	CleanupStack::PopAndDestroy(); // item
//	aLogger->Log(_L("Expected max volume error range: %d"),errRange);


	if (error != KErrNone)
	{
		CleanupStack::PopAndDestroy();
		return error;
	}


	// create CPlayRamp
	aLogger->Log(_L("Creating CPlayRamp"));

	CPlayRamp* selfObj = CPlayRamp::NewL(aConsole, aLogger, &fileName);

	selfObj->expectedVol = expectedVol;
	selfObj->rampTime = rampTime;

	CleanupStack::PushL(selfObj);

	CActiveScheduler::Start();


	if (Abs((selfObj->expectedVol) - expectedVol) != 0)
	{
		aLogger->Log(_L("Max volume differs from expected value by: %d"),Abs(selfObj->expectedVol - expectedVol));
		error = KErrDiffVolume;
	}

	CleanupStack::PopAndDestroy(2); // schedule, selfObj

	return error;
}


CPlayRamp* CPlayRamp::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger, TFileName* fileName)
{
	CPlayRamp* self = new (ELeave) CPlayRamp(aConsole, aLogger);
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

void CPlayRamp::MoscoStateChangeEvent(CBase* /*aObject*/, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CPlayRamp::MoscoStateChangeEvent"));
#endif

	logger->Log(_L("MoscoStateChangeEvent called, error: %d	prev: %d curr : %d"),aErrorCode,aPreviousState,aCurrentState);

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == 0)
	{
		TInt currVolume;
		recorder->GetVolume(currVolume);
		logger->Log(_L("MoscoStateChangeEvent get currVolume: %d  "),currVolume);
		recorder->SetVolume(expectedVol);
		logger->Log(_L("MoscoStateChangeEvent setVolume: %d"),expectedVol);
		recorder->SetVolumeRamp(rampTime);
		logger->Log(_L("MoscoStateChangeEvent rampTime: %d"),I64INT(rampTime.Int64())/1000000);
		recorder->PlayL();
		return;
	}

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == CMdaAudioClipUtility::EPlaying)
	{
		//TInt err;
		recorder->GetVolume(expectedVol);
		logger->Log(_L("MoscoStateChangeEvent GetVolume after play: %d  "),expectedVol);
		CActiveScheduler::Stop();
	}
	
	if (aErrorCode != KErrNone)
	    {
	    expectedVol = 0;
	    CActiveScheduler::Stop();
	    
	    }
	return;

}



void CPlayRamp::GetFileName(TPtrC path, TFileName* fileName)
{

	TParse p;
	p.Set(path,NULL,NULL);
	fileName->Append(p.DriveAndPath());
	fileName->Append(p.Name());

	fileName->Append(p.Ext());
}
