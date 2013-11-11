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


#include "PlayWindow.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CPlayWindow::CPlayWindow(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
}

CPlayWindow::~CPlayWindow()
{

	if (recorder) delete recorder;
	recorder = NULL;
}


void CPlayWindow::ConstructL(TFileName* fileName)
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

TInt CPlayWindow::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
{
	TInt error = KErrNone;
	aLogger->Log(_L("Creating scheduler"));

	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	aLogger->Log(_L("Creating CPlayWindow"));

	TFileName fileName;

	//Get file name
	TPtrC name;
//	TInt volume;
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


	//Get window start position
	TTimeIntervalMicroSeconds startPos = TInt64(0);
	item = aParser->GetItemLineL(KStartPos, ENoTag);
	if (item)
	{
		CleanupStack::PushL(item);
		startPos = ParseTime(item);
		CleanupStack::PopAndDestroy();
	}
	LogTime(_L("Window start:"),startPos.Int64(),aLogger);



	//Get window end position
	TTimeIntervalMicroSeconds endPos = TInt64(0);
	item = aParser->GetItemLineL(KEndPos, ENoTag);
	if (item)
	{
		CleanupStack::PushL(item);
		endPos = ParseTime(item);
		CleanupStack::PopAndDestroy();
	}
	LogTime(_L("Window end:"),endPos.Int64(),aLogger);

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


//	if (error != KErrNone)
//	{
//		CleanupStack::PopAndDestroy();
//		return error;
//	}


	// create CPlayWindow
	aLogger->Log(_L("Creating CPlayWindow"));

	CPlayWindow* selfObj = CPlayWindow::NewL(aConsole, aLogger, &fileName);

	selfObj->windowStart = startPos;
	selfObj->windowEnd = endPos;

	CleanupStack::PushL(selfObj);

	CActiveScheduler::Start();

	if (selfObj->windowStart != startPos)
	{
		aLogger->Log(_L("The window start position is not right after SetWindow: %d"),selfObj->windowStart);
		error = KErrDiffVolume;
	}


    TTimeIntervalMicroSeconds expectedPlayTime(Abs(endPos.Int64() - startPos.Int64()));

	LogTime(_L("expectedPlayTime:"),expectedPlayTime,aLogger);
	if (Abs(Abs(selfObj->endTime.Int64() - selfObj->startTime.Int64()) - expectedPlayTime.Int64()) > errorRange.Int64())
	{
		aLogger->Log(_L("Actual play time differs from expected play time"));
		error = KErrPlayWindow;
	}

	CleanupStack::PopAndDestroy(2); // schedule, selfObj

	return error;
}


CPlayWindow* CPlayWindow::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger, TFileName* fileName)
{
	CPlayWindow* self = new (ELeave) CPlayWindow(aConsole, aLogger);
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

void CPlayWindow::MoscoStateChangeEvent(CBase* /*aObject*/, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CPlayWindow::MoscoStateChangeEvent"));
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

		//	TInt expectedPlayTime;
//		TTimeIntervalMicroSeconds expectedPlayTime = Abs(windowEnd.Int64() - windowStart.Int64());
		LogTime(_L("Recorded clip duration:"),recorder->Duration());

		recorder->SetPlayWindow(windowStart,windowEnd);


		TTime time;
		time.UniversalTime();
		startTime = time.Int64();

		recorder->PlayL();
		return;
	}

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == CMdaAudioClipUtility::EPlaying)
	{
		//get position after play window, which should be start position
		windowStart = recorder->Position();

		TTime time;
		time.UniversalTime();
		endTime = time.Int64();

		CActiveScheduler::Stop();
	}
	   if (aErrorCode != KErrNone)
	    {
	    CActiveScheduler::Stop();   
	    }
	return;

}



void CPlayWindow::GetFileName(TPtrC path, TFileName* fileName)
{

	TParse p;
	p.Set(path,NULL,NULL);
	fileName->Append(p.DriveAndPath());
	fileName->Append(p.Name());

	fileName->Append(p.Ext());
}