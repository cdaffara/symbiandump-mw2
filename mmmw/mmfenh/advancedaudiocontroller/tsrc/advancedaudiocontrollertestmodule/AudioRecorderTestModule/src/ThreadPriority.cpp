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


#include "ThreadPriority.h"
#include <mmfFormatImplementationUIDs.hrh>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CThreadPriority::CThreadPriority(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;

}

CThreadPriority::~CThreadPriority()
{

	if (recorder) delete recorder;
	recorder = NULL;
}


void CThreadPriority::ConstructL(TFileName* fileName)
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

TInt CThreadPriority::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
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






	if (error != KErrNone)
	{
		CleanupStack::PopAndDestroy();  //scheduler
		return error;
	}


	// create CThreadPriority
	aLogger->Log(_L("Creating CThreadPriority"));

	CThreadPriority* selfObj = CThreadPriority::NewL(aConsole, aLogger, &fileName);


	CleanupStack::PushL(selfObj);

	CActiveScheduler::Start();
	
	if(selfObj->iCallbackErr != KErrNone)
	    error = selfObj->iCallbackErr;
	
	CleanupStack::PopAndDestroy(2); // schedule, selfObj

	return error;
}


CThreadPriority* CThreadPriority::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger, TFileName* fileName)
{
	CThreadPriority* self = new (ELeave) CThreadPriority(aConsole, aLogger);
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

void CThreadPriority::MoscoStateChangeEvent(CBase* /*aObject*/, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CThreadPriority::MoscoStateChangeEvent"));
#endif
	

	logger->Log(_L("MoscoStateChangeEvent called, error: %d	prev: %d curr : %d"),aErrorCode,aPreviousState,aCurrentState);
logger->Log(_L("Before Checking Duaration."));
iCallbackErr = aErrorCode;
    
		TTimeIntervalMicroSeconds aDuration, aRecordTime;
  	    TMMFDurationInfo clipDuration = recorder->Duration(aDuration);
		logger->Log(_L("Duaration is %d"), aDuration.Int64());


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

		recorder->PlayL();


		return;


	}

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == CMdaAudioClipUtility::EPlaying)
	{
	    logger->Log(_L("Checking Duaration."));
		TTimeIntervalMicroSeconds aDuration, aRecordTime;
  	    TMMFDurationInfo clipDuration = recorder->Duration(aDuration);
		logger->Log(_L("Duaration is %d"), aDuration.Int64());

		const CMMFControllerImplementationInformation& info = recorder->AudioPlayControllerImplementationInformationL();

		const CMMFControllerImplementationInformation& info_1 = recorder->AudioRecorderControllerImplementationInformationL();



		//aRecordTime= recorder->RecordTimeAvailable();
 	    //logger->Log(_L("RecordTimeAvailable %d"), aRecordTime);


		CActiveScheduler::Stop();
		return;
	}
	 if (aErrorCode != KErrNone)
        {
        
        CActiveScheduler::Stop();
        return ;
        }




}



void CThreadPriority::GetFileName(TPtrC path, TFileName* fileName)
{

	TParse p;
	p.Set(path,NULL,NULL);
	fileName->Append(p.DriveAndPath());
	fileName->Append(p.Name());

	fileName->Append(p.Ext());
}