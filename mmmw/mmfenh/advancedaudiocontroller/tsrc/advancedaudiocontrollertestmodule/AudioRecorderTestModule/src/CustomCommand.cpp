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


#include "CustomCommand.h"
#include <mmfFormatImplementationUIDs.hrh>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CCustomCommand::CCustomCommand(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
	iCallBackErr = KErrNone;
}

CCustomCommand::~CCustomCommand()
{

	if (recorder) delete recorder;
	recorder = NULL;
}


void CCustomCommand::ConstructL(TFileName* fileName)
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

TInt CCustomCommand::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
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
	aLogger->Log(_L("Creating CCustomCommand"));

	CCustomCommand* selfObj = CCustomCommand::NewL(aConsole, aLogger, &fileName);


	CleanupStack::PushL(selfObj);

	CActiveScheduler::Start();
	
	if(selfObj->iCallBackErr != KErrNone)
	    {
        error = selfObj->iCallBackErr;
	    }
	
	CleanupStack::PopAndDestroy(2); // schedule, selfObj

	return error;
}


CCustomCommand* CCustomCommand::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger, TFileName* fileName)
{
	CCustomCommand* self = new (ELeave) CCustomCommand(aConsole, aLogger);
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

void CCustomCommand::MoscoStateChangeEvent(CBase* /*aObject*/, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CCustomCommand::MoscoStateChangeEvent"));
#endif
	

	logger->Log(_L("MoscoStateChangeEvent called, error: %d	prev: %d curr : %d"),aErrorCode,aPreviousState,aCurrentState);
	
	iCallBackErr = aErrorCode;
	
	TUint bitRate = recorder->SourceBitRateL();
	logger->Log(_L("SourceBitRateL %d "),bitRate);
	
	

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


		TRAPD(err, recorder->PlayControllerCustomCommandSync(dummyPckg, dummyFunc,
  								    	dummyBuff, dummyBuff, dataFrom));
	    if (err != KErrNone)
        {
            logger->Log(_L("PlayControllerCustomCommandSync with dataFrom - err=%d"), err);

        }
    	logger->Log(_L("PlayControllerCustomCommandSync with dataFrom successful"));

    	TRAP(err, recorder->PlayControllerCustomCommandSync(dummyPckg, dummyFunc,
        								dummyBuff, dummyBuff));
    	if (err != KErrNone)
        {
            logger->Log(_L("PlayControllerCustomCommandSync - err=%d"), err);

        }
    	logger->Log(_L("PlayControllerCustomCommandSync successful"));


    	recorder->RegisterForAudioLoadingNotification(*this);
		logger->Log(_L("RegisterForAudioLoadingNotification set"));
		recorder->RegisterAudioResourceNotification(*this,KUidInterfaceMMFController);
		logger->Log(_L("RegisterAudioResourceNotification set"));
        recorder->CancelRegisterAudioResourceNotification(KUidInterfaceMMFController);
		logger->Log(_L("CancelRegisterAudioResourceNotification set"));



		//PlayController
 	   	TRAPD(err1, recorder->RecordControllerCustomCommandSync(dummyPckg, dummyFunc,
  								    	dummyBuff, dummyBuff, dataFrom));
	    if (err1 != KErrNone)
        {
            logger->Log(_L("RecordControllerCustomCommandSync with dataFrom - err=%d"), err1);

        }
    	logger->Log(_L("RecordControllerCustomCommandSync with dataFrom successful"));

    	TRAP(err, recorder->RecordControllerCustomCommandSync(dummyPckg, dummyFunc,
        								dummyBuff, dummyBuff));
    	if (err != KErrNone)
        {
            logger->Log(_L("RecordControllerCustomCommandSync - err=%d"), err);

        }
    	logger->Log(_L("RecordControllerCustomCommandSync successful"));

    	logger->Log(_L("Using UseSharedHeap"));
    	recorder->UseSharedHeap();

		recorder->PlayL();


		return;

	}

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == CMdaAudioClipUtility::EPlaying)
	{
		// Create DRM custom command
    	MMMFDRMCustomCommand* drmCustomCommand;
    	drmCustomCommand = recorder->GetDRMCustomCommand();
    	if (drmCustomCommand == NULL)
        {
        	logger->Log(_L("MMMFDRMCustomCommand is NULL."));

        }
        logger->Log(_L("MMMFDRMCustomCommand is successful."));

		recorder->WillResumePlay();
		CActiveScheduler::Stop();
	}
	if(aErrorCode != KErrNone)
	        {
	            
	            CActiveScheduler::Stop();
	            
	        }

	return;

}

void CCustomCommand::MaloLoadingStarted()
{
		logger->Log(_L("MaloLoadingStarted "));
}

void CCustomCommand::MaloLoadingComplete()
{
	  	logger->Log(_L("MaloLoadingComplete "));
}


void CCustomCommand::MarncResourceAvailable(TUid aNotificationEventId, const TDesC8 &aNotificationData)
{
		logger->Log(_L("MarncResourceAvailable "));
}

void CCustomCommand::GetFileName(TPtrC path, TFileName* fileName)
{

	TParse p;
	p.Set(path,NULL,NULL);
	fileName->Append(p.DriveAndPath());
	fileName->Append(p.Name());

	fileName->Append(p.Ext());
}