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


#include "RecordFormats.h"
#include "badesca.h"
#include "mmfcontrollerpluginresolver.h"
#include "RecPositionClip.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecordFormats::CRecordFormats(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
	iCallBackErr = KErrNone;
}

CRecordFormats::~CRecordFormats()
{
	if (recorder) delete recorder;
	recorder = NULL;
}



CRecordFormats* CRecordFormats::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger, TFileName* fileName)
{
	CRecordFormats* self = new (ELeave) CRecordFormats(aConsole, aLogger);
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

void CRecordFormats::ConstructL(TFileName* fileName)
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


void CRecordFormats::MoscoStateChangeEvent(CBase* /*aObject*/, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{

#ifdef _DEBUG
    RDebug::Print (_L ("CRecordFormats::MoscoStateChangeEvent"));
#endif

	logger->Log(_L("MoscoStateChangeEvent called, error: %d	prev: %d curr : %d"),aErrorCode,aPreviousState,aCurrentState);
	iCallBackErr = aErrorCode;
	
	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen)
	{
		TRAPD(err3, recorder->AudioRecorderControllerImplementationInformationL());
		if (err3 != KErrNone)
		{
			logger->Log(_L("Error %d openning controller"),err3);

		}
		TTimeIntervalMicroSeconds aRecordTime;
		aRecordTime= recorder->RecordTimeAvailable();
 	    logger->Log(_L("RecordTimeAvailable %d"), aRecordTime);

		logger->Log(_L("In if loop Error %d openning controller"),err3);
	}
	logger->Log(_L("Before Stop ...."));

	CActiveScheduler::Stop();
	return;
}



TInt CRecordFormats::RunTestL(CTestModuleIf* aConsole, CStifLogger *aLogger, CStifSectionParser* aParser, TInt* /*clipCounter*/)
{
    TInt error = KErrNone;
	aLogger->Log(_L("Creating scheduler"));

	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	aLogger->Log(_L("Creating CRecordFormats"));

	TFileName fileName;
	TPtrC name;


	//Obtain file name from parser
	if (aParser->GetLine(KFileName, name, ENoTag) != KErrNone)
	{
		aLogger->Log(_L("No file name provided"));
		return KErrSyntax;
	}

	GetFileName(name,&fileName);


	aLogger->Log(_L("Creating CRecordFormats"));

	CRecordFormats* selfObj = CRecordFormats::NewL(aConsole, aLogger, &fileName);
	CleanupStack::PushL(selfObj);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(2);
	
	if(selfObj->iCallBackErr != KErrNone)
	    error = selfObj->iCallBackErr;
	
	return error;
}


void CRecordFormats::GetFileName(TPtrC path, TFileName* fileName)
{

	TParse p;
	p.Set(path,NULL,NULL);
	fileName->Append(p.DriveAndPath());
	fileName->Append(p.Name());

	fileName->Append(p.Ext());
}