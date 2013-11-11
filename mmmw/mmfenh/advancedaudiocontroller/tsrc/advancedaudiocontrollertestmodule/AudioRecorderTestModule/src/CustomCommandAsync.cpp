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


#include "CustomCommandAsync.h"
#include <mmfFormatImplementationUIDs.hrh>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CCustomCommandAsync::CCustomCommandAsync(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
	iCallBackErr = KErrNone;

}

CCustomCommandAsync::~CCustomCommandAsync()
{

	if (recorder) delete recorder;
	recorder = NULL;
}


void CCustomCommandAsync::ConstructL(TFileName* fileName)
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

TInt CCustomCommandAsync::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
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
	aLogger->Log(_L("Creating CCustomCommandAsync"));

	CCustomCommandAsync* selfObj = CCustomCommandAsync::NewL(aConsole, aLogger, &fileName);


	CleanupStack::PushL(selfObj);

	CActiveScheduler::Start();

	if(selfObj->iCallBackErr != KErrNone)
	        {
	        error = selfObj->iCallBackErr;
	        }
	CleanupStack::PopAndDestroy(2); // schedule, selfObj

	return error;
}


CCustomCommandAsync* CCustomCommandAsync::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger, TFileName* fileName)
{
	CCustomCommandAsync* self = new (ELeave) CCustomCommandAsync(aConsole, aLogger);
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

void CCustomCommandAsync::MoscoStateChangeEvent(CBase* /*aObject*/, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CCustomCommandAsync::MoscoStateChangeEvent"));
#endif
	

	logger->Log(_L("MoscoStateChangeEvent called, error: %d	prev: %d curr : %d"),aErrorCode,aPreviousState,aCurrentState);

	iCallBackErr = aErrorCode;
	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == 0)
	{
		TBuf8<25> dataFrom;

 	   	TRequestStatus status;
		TMMFMessageDestinationPckg dummyPckg;
		TInt dummyFunc = 100;
		TBuf8<8> dummyBuff;

	    // Initialize
		recorder->SetAudioDeviceMode(CMdaAudioRecorderUtility::ELocal);
		logger->Log(_L("Mode set"));
		recorder->SetGain(recorder->MaxGain());
		logger->Log(_L("Gain set"));
		recorder->SetVolume(recorder->MaxVolume());
		logger->Log(_L("Volume set"));
		recorder->SetPosition(TTimeIntervalMicroSeconds(TInt64(0)));
		logger->Log(_L("Position set"));



		//RecordController
    	recorder->RecordControllerCustomCommandAsync(dummyPckg, dummyFunc,
    							    	dummyBuff, dummyBuff, dataFrom, status);
    	User::WaitForRequest(status);
		logger->Log(_L("RecordControllerCustomCommandAsync with dataFrom"));

    	recorder->RecordControllerCustomCommandAsync(dummyPckg, dummyFunc,
        								dummyBuff, dummyBuff, status);
    	User::WaitForRequest(status);
		logger->Log(_L("RecordControllerCustomCommandAsync"));

		//PlayController
	    recorder->PlayControllerCustomCommandAsync(dummyPckg, dummyFunc,
    							    	dummyBuff, dummyBuff, dataFrom, status);
	    User::WaitForRequest(status);
		logger->Log(_L("PlayControllerCustomCommandAsync with dataFrom"));

    	recorder->PlayControllerCustomCommandAsync(dummyPckg, dummyFunc,
        								dummyBuff, dummyBuff, status);
    	User::WaitForRequest(status);
		logger->Log(_L("PlayControllerCustomCommandAsync"));


    	logger->Log(_L("Using UseSharedHeap"));
    	recorder->UseSharedHeap();

		recorder->PlayL();


		return;

	}

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == CMdaAudioClipUtility::EPlaying)
	{

		recorder->WillResumePlay();
		CActiveScheduler::Stop();
	}

    if (aErrorCode != KErrNone)
    {
    
    CActiveScheduler::Stop();
    
    }
	return;

}

void CCustomCommandAsync::MaloLoadingStarted()
{
		logger->Log(_L("MaloLoadingStarted "));
}

void CCustomCommandAsync::MaloLoadingComplete()
{
	  	logger->Log(_L("MaloLoadingComplete "));
}


void CCustomCommandAsync::MarncResourceAvailable(TUid aNotificationEventId, const TDesC8 &aNotificationData)
{
		logger->Log(_L("MarncResourceAvailable "));
}

void CCustomCommandAsync::GetFileName(TPtrC path, TFileName* fileName)
{

	TParse p;
	p.Set(path,NULL,NULL);
	fileName->Append(p.DriveAndPath());
	fileName->Append(p.Name());

	fileName->Append(p.Ext());
}