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


#include "CropFromBeginning.h"
#include <mmfFormatImplementationUIDs.hrh>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CCropFromBeginning::CCropFromBeginning(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
    callbackErr = KErrNone;
}

CCropFromBeginning::~CCropFromBeginning()
{

	if (recorder) delete recorder;
	recorder = NULL;
}


void CCropFromBeginning::ConstructL(TFileName* fileName)
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

TInt CCropFromBeginning::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
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


	// create CCropFromBeginning
	aLogger->Log(_L("Creating CCropFromBeginning"));

	CCropFromBeginning* selfObj = CCropFromBeginning::NewL(aConsole, aLogger, &fileName);


	CleanupStack::PushL(selfObj);

	CActiveScheduler::Start();

	if (selfObj->callbackErr != KErrNone)
	{
		error = selfObj->callbackErr;
	}

	CleanupStack::PopAndDestroy(2); // schedule, selfObj

	return error;
}


CCropFromBeginning* CCropFromBeginning::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger, TFileName* fileName)
{
	CCropFromBeginning* self = new (ELeave) CCropFromBeginning(aConsole, aLogger);
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

void CCropFromBeginning::MoscoStateChangeEvent(CBase* /*aObject*/, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CCropFromBeginning::MoscoStateChangeEvent"));
#endif

	callbackErr = aErrorCode;

	logger->Log(_L("MoscoStateChangeEvent called, error: %d	prev: %d curr : %d"),aErrorCode,aPreviousState,aCurrentState);

    TInt aState = recorder->State();
    logger->Log(_L("Current State %d"), aState);

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



		TRAPD(err, recorder->CropFromBeginningL());
		if (err != KErrNone)
			{
				logger->Log(_L("Error %d for CropFromBeginningL."),err);
				callbackErr = KErrCropFromBeginning;

			}

		logger->Log(_L("CropFromBeginningL set"));

		recorder->PlayL();


		return;


	}

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == CMdaAudioClipUtility::EPlaying)
	{

		CActiveScheduler::Stop();
	}
	if (aErrorCode != KErrNone)
	        {

	        CActiveScheduler::Stop();
	        
	        }


	return;

}



void CCropFromBeginning::GetFileName(TPtrC path, TFileName* fileName)
{

	TParse p;
	p.Set(path,NULL,NULL);
	fileName->Append(p.DriveAndPath());
	fileName->Append(p.Name());

	fileName->Append(p.Ext());
}