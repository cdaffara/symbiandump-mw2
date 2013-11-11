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

#include "RecordPriority.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecordPriority::CRecordPriority(CTestModuleIf *aConsole, CStifLogger *aLogger):CRecordingBase(aConsole,aLogger)
{
	finishPlace = -1;
	firstTime = true;
	isInterrupted = false;
	wasInterrupted = false;
}

CRecordPriority::~CRecordPriority()
{

}

CRecordingBase* CRecordPriority::CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	//aLogger->Log(_L("CRecordPriority::CreateObjectL"));
	return (new (ELeave) CRecordPriority(aConsole, aLogger));
}

TInt CRecordPriority::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter)
{
	TInt error = KErrNone;
	TInt stack = 0;
	TInt recorderCounter = 0;
	TInt finishPlace = 1;
	TInt totalFiles = 0;
	TBool pError = false;
	TInt clipNum = *clipCounter;
	*clipCounter = *clipCounter + 1;

	//Create scheduler
	aLogger->Log(_L("Creating scheduler"));
	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); stack++;// push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	//Read rec time
	TTimeIntervalMicroSeconds recTime = TInt64(0);

	//Obtain record time from parser
	CStifItemParser* item = aParser->GetItemLineL(KRecordTime, ENoTag);
	if (!item)
	{
		aLogger->Log(_L("No record time provided"));
		CleanupStack::PopAndDestroy();
		return KErrSyntax;
	}
	CleanupStack::PushL(item); stack++;
	recTime = CRecordingBase::ParseTime(item);
	CleanupStack::PopAndDestroy(); stack--;
	//aLogger->Log(_L("Record time: %d"),recTime.Int64());
	LogTime(_L("Record time:"),recTime.Int64(),aLogger);

	//Get first file
	item = aParser->GetItemLineL(KFileName, ETag);
	if (!item)
	{
		aLogger->Log(_L("No file provided"));
		CleanupStack::PopAndDestroy();
		return KErrSyntax;
	}

	//Create recorders
	bool loop = true;
	while (loop == true)
	{
		recorderCounter++;

		TFileName fileName;
		TPtrC path;
		TInt priority, err, finish;
		TTimeIntervalMicroSeconds toStart;

		//Get recorder data
		CleanupStack::PushL(item); stack++;
		if ((err = ParseTag(item,path,priority,finish,toStart)) != KErrNone)
		{
			aLogger->Log(_L("Error parsing tag"));
			CleanupStack::PopAndDestroy(2);
			return err;
		}
		CleanupStack::PopAndDestroy(); stack--;
		CRecordingBase::GetFileName(path,&fileName,clipNum);

		//Create recorder
		CRecordingBase* recObj = CRecordingBase::NewL(aConsole, aLogger, &fileName, CRecordPriority::CreateObjectL);
		if (!recObj)
		{
			CleanupStack::PopAndDestroy(2); // scheduler
			return KErrRecOpenFile;
		}
		CleanupStack::PushL(recObj); stack++;
		aLogger->Log(_L("Recorder for file %S, has priority %d"),&fileName,priority);
		recObj->timeToRecord = recTime;
		recObj->recordingFileName = fileName;
		recObj->recorder->SetPriority(priority,EMdaPriorityPreferenceQuality);
		((CRecordPriority*)recObj)->expectedFinishPlace = finish;
		((CRecordPriority*)recObj)->isInterrupted = (finish != 0);
		recObj->PreProcess(aParser);

		aLogger->Log(_L("Creating timer for stopping"));
		//Create timer to stop recording
		CPriorityParams* params2 = new CPriorityParams();
		params2->totalFiles = &totalFiles;
		params2->finishPlace = &finishPlace;
		params2->error = &pError;
		params2->iAction = KActionStopRecord;
		((CRecordPriority*)recObj)->priorityParams = params2;
		CMediaEvent* timer2 = CMediaEvent::NewL(*aConsole,*aLogger,recTime,TTimeIntervalMicroSeconds32(0),recObj,params2,CActive::EPriorityStandard,false);
		CleanupStack::PushL(timer2); stack++;

		aLogger->Log(_L("Creating timer for starting"));
		//Create timer to start recording
		CTimerParams* params = new CTimerParams();
		params->iAction = KActionRecord;
		params->timer = timer2;
		CMediaEvent* timer = CMediaEvent::NewL(*aConsole,*aLogger,toStart,TTimeIntervalMicroSeconds32(0),recObj,params);
		CleanupStack::PushL(timer); stack++;

		item = aParser->GetNextItemLineL(KFileName, ETag);
		if (!item) break;
	}

	totalFiles = recorderCounter;

	//Start scheduler
	aLogger->Log(_L("Starting Scheduler"));
	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(stack); // scheduler

	if (pError)
	{
		error = KPriorityError;
	}

	return error;
}

TInt CRecordPriority::PreProcess(CStifSectionParser* /*aParser*/)
{
/*	if (recorder && firstTime)
	{
		recorder->SetAudioDeviceMode(CMdaAudioRecorderUtility::ELocal);
		recorder->SetPosition(TTimeIntervalMicroSeconds(TInt64(0)));
		TRAPD(err,recorder->CropL());
		if (err != KErrNone) logger->Log(_L("Error cropping file: %d"),err);
		firstTime = false;
	} */
	return KErrNone;
}

TInt CRecordPriority::PostProcess(CStifSectionParser* /*aParser*/)
{
	return KErrNone;
}

TInt CRecordPriority::ParseTag(CStifItemParser* item, TPtrC& name, TInt& priority, TInt& finish ,TTimeIntervalMicroSeconds& timeToStart)
{
	TInt sec=0, mil=0, mic=0;

	if (item->GetString(KFileName,name) != KErrNone)
		return KErrSyntax;

	if (item->GetInt(name,priority) != KErrNone)
		return KErrSyntax;

	if (item->GetNextInt(finish) != KErrNone)
		return KErrSyntax;

	item->GetNextInt(sec);
	item->GetNextInt(mil);
	item->GetNextInt(mic);

	timeToStart = TInt64(sec*1000000 + mil*1000 + mic);

	return KErrNone;
}

TInt CRecordPriority::ExecuteL(CParameters* aParams)
{
	if (recorder)
	{
		if (aParams->iAction == KActionRecord)
		{
			((CTimerParams*)aParams)->timer->Start();
			logger->Log(_L("Start recording file %S"),&recordingFileName);
			StartRecordingL();
		}
		else
		if (aParams->iAction == KActionStopRecord && !wasInterrupted)
		{
			//Stop recorder
			recorder->Stop();
			logger->Log(_L("CRecordPriority recorder stopped"));

			finishPlace = (*((CPriorityParams*)aParams)->finishPlace)++;
			//logger->Log(_L("Finished recording: %S, place: %d, expected place: %d"),&recordingFileName, finishPlace, expectedFinishPlace);
			logger->Log(_L("Finished recording: %S"),&recordingFileName);

			//if (finishPlace != expectedFinishPlace) (*((CPriorityParams*)aParams)->error) = true;
		/*	if (isInterrupted)
			{
				logger->Log(_L("Error: %S was specified to be interrupted"),&recordingFileName);
				(*((CPriorityParams*)aParams)->error) = true;
			} */

			if (finishPlace == *((CPriorityParams*)aParams)->totalFiles)
			{
				logger->Log(_L("Stopping Scheduler"));
				CActiveScheduler::Stop();
			}
		}
	}
	return 0;
}

void CRecordPriority::MoscoStateChangeEvent(CBase* /*aObject*/, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{
	logger->Log(_L("MoscoStateChangeEvent called, error: %d (%S)"),aErrorCode,&recordingFileName);

	if (recorder && firstTime && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == 0)
	{
		recorder->SetAudioDeviceMode(CMdaAudioRecorderUtility::ELocal);
//		recorder->SetPosition(TTimeIntervalMicroSeconds(TInt64(0)));
//		TRAPD(err,recorder->CropL());
//		if (err != KErrNone)
		//	logger->Log(_L("Error cropping file: %d"),err);
	//	logger->Log(_L("Error cropping file:"));
		firstTime = false;
	}
	if (recorder && aErrorCode != KErrNone)
	{
		recorder->Stop();
		logger->Log(_L("CRecordPriority recorder stopped"));

		if (priorityParams)
		{
			wasInterrupted = true;
			finishPlace = (*priorityParams->finishPlace)++;
			logger->Log(_L("Stopped recording: %S"),&recordingFileName);

			if (!isInterrupted)
			{
				logger->Log(_L("Error: %S wasn't specified to be interrupted"),&recordingFileName);
				(*priorityParams->error) = true;
			}

			if (finishPlace == *priorityParams->totalFiles)
			{
				logger->Log(_L("Stopping Scheduler"));
				CActiveScheduler::Stop();
			}
		}
	}
}