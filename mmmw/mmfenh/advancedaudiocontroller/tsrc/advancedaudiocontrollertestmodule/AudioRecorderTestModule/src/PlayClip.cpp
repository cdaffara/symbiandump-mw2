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


#include "PlayClip.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CPlayClip::CPlayClip(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
}

CPlayClip::~CPlayClip()
{

	if (recorder) delete recorder;
	recorder = NULL;
}


void CPlayClip::ConstructL(TFileName* fileName)
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

TInt CPlayClip::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
{
	TInt error = KErrNone;
	aLogger->Log(_L("Creating scheduler"));

	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	aLogger->Log(_L("Creating CPlayClip"));

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


	//Get pause interval
	TTimeIntervalMicroSeconds pauseTime = TInt64(0);
	item = aParser->GetItemLineL(KPause, ENoTag);
	if (item)
	{
		CleanupStack::PushL(item);
		pauseTime = ParseTime(item);
		CleanupStack::PopAndDestroy();
	}
	LogTime(_L("Window start:"),pauseTime.Int64(),aLogger);



	//Get repeat times
	TInt repeats = 1;
	item = aParser->GetItemLineL(KRepeats);
	if (item)
	{
		CleanupStack::PushL(item);
		if (item->GetInt(KRepeats,repeats) != KErrNone)
		{
			aLogger->Log(_L("Invalid repeats value."));
			CleanupStack::PopAndDestroy(); // item
			error = KErrSyntax;
		}
		CleanupStack::PopAndDestroy(); // item
	}


	//Get error range
	TTimeIntervalMicroSeconds errorRange = TInt64(KDefaultErrorRange);
	item = aParser->GetItemLineL(KErrorRange, ENoTag);
	if (item)
	{
		CleanupStack::PushL(item);
		errorRange = ParseTime(item);
		CleanupStack::PopAndDestroy();
	}
	LogTime(_L("Error range:"),errorRange.Int64(),aLogger);


	if (error != KErrNone)
	{
		CleanupStack::PopAndDestroy();
		return error;
	}


	// create CPlayClip
	aLogger->Log(_L("Creating CPlayClip"));

	CPlayClip* selfObj = CPlayClip::NewL(aConsole, aLogger, &fileName);

	selfObj->pauseTime = pauseTime;
	selfObj->repeats = repeats;

	CleanupStack::PushL(selfObj);

	CActiveScheduler::Start();

    TTimeIntervalMicroSeconds expectedPlayTime(selfObj->expectedWholePlayTime);
	aLogger->Log(_L("expectedPlayTime: %d"),expectedPlayTime.Int64());
	aLogger->Log(_L("Actual Play time: %d"),Abs(selfObj->endTime.Int64() - selfObj->startTime.Int64()));

	if (Abs(Abs(selfObj->endTime.Int64() - selfObj->startTime.Int64()) - expectedPlayTime.Int64()) > errorRange.Int64())
	{
		aLogger->Log(_L("Actual play time differs from expected play time"));
		error = KErrPlayWindow;
	}

	CleanupStack::PopAndDestroy(2); // schedule, selfObj

	return error;
}


CPlayClip* CPlayClip::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger, TFileName* fileName)
{
	CPlayClip* self = new (ELeave) CPlayClip(aConsole, aLogger);
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

void CPlayClip::MoscoStateChangeEvent(CBase* /*aObject*/, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CPlayClip::MoscoStateChangeEvent"));
#endif

	logger->Log(_L("MoscoStateChangeEvent called, error: %d	prev: %d curr : %d"),aErrorCode,aPreviousState,aCurrentState);

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == 0)
	{
		// Initialize
	//	recorder->SetAudioDeviceMode(CMdaAudioRecorderUtility::ELocal);
	//	logger->Log(_L("Mode set"));
	//	recorder->SetGain(recorder->MaxGain());
	//	logger->Log(_L("Gain set"));
		recorder->SetVolume(recorder->MaxVolume());
		logger->Log(_L("Set max Volume"));
		recorder->SetPosition(TTimeIntervalMicroSeconds(TInt64(0)));
		logger->Log(_L("SetPosition 0"));

		TTimeIntervalMicroSeconds expectedPlayTime ((recorder->Duration().Int64() + pauseTime.Int64() ) * (repeats+1) );

		logger->Log(_L("clip duration, %d") , recorder->Duration().Int64());
		logger->Log(_L("expectedPlayTime, %d") , expectedPlayTime.Int64());

		expectedWholePlayTime = expectedPlayTime;

		recorder->SetRepeats(repeats,pauseTime);


		TTime time;
		time.UniversalTime();
		startTime = time.Int64();

		recorder->PlayL();
		return;
	}

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == CMdaAudioClipUtility::EPlaying)
	{
		TTime time;
		time.UniversalTime();
		endTime = time.Int64();

		CActiveScheduler::Stop();
	}

	if (recorder && aErrorCode != KErrNone)
	{
		logger->Log(_L("Return erro %d"), aErrorCode);
		CActiveScheduler::Stop();

	}

	return;

}



void CPlayClip::GetFileName(TPtrC path, TFileName* fileName)
{

	TParse p;
	p.Set(path,NULL,NULL);
	fileName->Append(p.DriveAndPath());
	fileName->Append(p.Name());

	fileName->Append(p.Ext());
}


