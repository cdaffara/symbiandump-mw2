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


#include "PlayVolume.h"
#include "badesca.h"
#include "mmfcontrollerpluginresolver.h"
#include "RecPositionClip.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayVolume::CPlayVolume(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
}

CPlayVolume::~CPlayVolume()
{
	if (recorder) delete recorder;
	recorder = NULL;
}



CPlayVolume* CPlayVolume::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger, TFileName* fileName)
{
	CPlayVolume* self = new (ELeave) CPlayVolume(aConsole, aLogger);
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

void CPlayVolume::ConstructL(TFileName* fileName)
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


void CPlayVolume::MoscoStateChangeEvent(CBase* /*aObject*/, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{

#ifdef _DEBUG
    RDebug::Print (_L ("CPlayVolume::MoscoStateChangeEvent"));
#endif

	logger->Log(_L("MoscoStateChangeEvent called, error: %d	prev: %d curr : %d"),aErrorCode,aPreviousState,aCurrentState);

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == 0)
	{
        // Initialize
		recorder->SetAudioDeviceMode(CMdaAudioRecorderUtility::ELocal);
		logger->Log(_L("Mode set"));
		recorder->SetGain(recorder->MaxGain());
		logger->Log(_L("Gain set"));
		//recorder->SetVolume(recorder->MaxVolume());
		//logger->Log(_L("Volume set"));
		recorder->SetPosition(TTimeIntervalMicroSeconds(TInt64(0)));
		logger->Log(_L("Position set"));

		TInt maxVolume = recorder->MaxVolume();
		logger->Log(_L("MoscoStateChangeEvent get maxVolume: %d  "),maxVolume);
		recorder->SetVolume(expectedVol);
		logger->Log(_L("MoscoStateChangeEvent setVolume: %d"),expectedVol);
		recorder->PlayL();
		return;
	}

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == CMdaAudioClipUtility::EPlaying)
	{
		TInt err;

		err = recorder->GetVolume(volume);
		if (err != KErrNone)
		{
			logger->Log(_L("Error getting volume: %d"),err);
		}

		this->volume = volume;
		logger->Log(_L("MoscoStateChangeEvent getVolume:  %d "),volume);
		CActiveScheduler::Stop();
		return;
	}
	if (aErrorCode != KErrNone)
	    {
        CActiveScheduler::Stop();
		return;
        }
    
}



TInt CPlayVolume::RunTestL(CTestModuleIf* aConsole, CStifLogger *aLogger, CStifSectionParser* aParser, TInt* /*clipCounter*/)
{
	TInt error = KErrNone;
	aLogger->Log(_L("Creating scheduler"));

	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	aLogger->Log(_L("Creating CPlayVolume"));

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
	item = aParser->GetItemLineL(KMaxVolume);
	CleanupStack::PushL(item);
	if (item)
		if (item->GetInt(KMaxVolume,expectedVol) != KErrNone)
		{
			aLogger->Log(_L("Invalid expected volume."));
			error = KErrSyntax;
		}
	CleanupStack::PopAndDestroy(); // item
	aLogger->Log(_L("Expected max volume: %d"),expectedVol);

	//Get max volume error range
	TInt errRange = 0;
	item = aParser->GetItemLineL(KErrorRange);
	CleanupStack::PushL(item);
	if (item)
		if (item->GetInt(KErrorRange,errRange) != KErrNone)
		{
			aLogger->Log(_L("Invalid max volume error range."));
			error = KErrSyntax;
		}
	CleanupStack::PopAndDestroy(); // item
	aLogger->Log(_L("Expected max volume error range: %d"),errRange);


	// create CPlayVolume
	aLogger->Log(_L("Creating CPlayVolume"));

	if (error != KErrNone)
	{
		CleanupStack::PopAndDestroy();
		return error;
	}

	CPlayVolume* selfObj = CPlayVolume::NewL(aConsole, aLogger, &fileName);

	selfObj->expectedVol = expectedVol;

	CleanupStack::PushL(selfObj);

	CActiveScheduler::Start();


	if (Abs((selfObj->volume) - expectedVol) > errRange)
	{
		aLogger->Log(_L("Max volume differs from expected value by: %d"),Abs(selfObj->volume - expectedVol));
		error = KErrDiffVolume;
	}

	CleanupStack::PopAndDestroy(2); // schedule, selfObj

	return error;
}


void CPlayVolume::GetFileName(TPtrC path, TFileName* fileName)
{

	TParse p;
	p.Set(path,NULL,NULL);
	fileName->Append(p.DriveAndPath());
	fileName->Append(p.Name());

	fileName->Append(p.Ext());
}