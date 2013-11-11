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

#include "RecPositionClip.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecPositionClip::CRecPositionClip(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
}

CRecPositionClip::~CRecPositionClip()
{

}

CRecPositionClip* CRecPositionClip::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	CRecPositionClip* self = new (ELeave) CRecPositionClip(aConsole, aLogger);
    CleanupStack::PushL(self);

    self->ConstructL();

	CleanupStack::Pop(self);
    return self;
}

TInt CRecPositionClip::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
{
	//Creating scheduler
	aLogger->Log(_L("Creating scheduler"));

	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	//Creating CRecPositionClip
	aLogger->Log(_L("Creating CRecPositionClip"));

	CRecPositionClip* recPos = CRecPositionClip::NewL(aConsole, aLogger);
	CleanupStack::PushL(recPos);

	//Creating recorder
	aLogger->Log(_L("Creating CMdaAudioRecorderUtility"));

	CMdaAudioRecorderUtility* recorder = CMdaAudioRecorderUtility::NewL(*recPos);
	CleanupStack::PushL(recorder);

	TInt error = KErrNone;
	TPtrC fileName;
	TTimeIntervalMicroSeconds duration = TInt64(0);
	TTimeIntervalMicroSeconds duration2 = TInt64(0);
	TTimeIntervalMicroSeconds errRange = TInt64(KDefaultErrorRange);

	//Get error range
	CStifItemParser* item = aParser->GetItemLineL(KErrorRange, ENoTag);
	CleanupStack::PushL(item);
	errRange = CRecPositionClip::ParseTime(item);
	CleanupStack::PopAndDestroy();
	//aLogger->Log(_L("Error range: %d"),errRange.Int64());
	LogTime(_L("Error range:"),errRange.Int64(),aLogger);

	//Get file name
	item = aParser->GetItemLineL(KFileName, ENoTag);

	if (!item)
	{
		CleanupStack::PopAndDestroy(3);
		return KErrSyntax;
	}

	CleanupStack::PushL(item);

	if (CRecPositionClip::ParseFileName(item, fileName, duration) != KErrNone)
	{
		aLogger->Log(_L("Invalid file name"));
		CleanupStack::PopAndDestroy(4);
		return KErrSyntax;
	}

	CleanupStack::PopAndDestroy();

	//Open file
	TRAPD(err,recorder->OpenFileL(fileName));

	if (err != KErrNone)
	{
		aLogger->Log(_L("Error %d opening file: %S"),err,&fileName);
		CleanupStack::PopAndDestroy(3);
		return KErrRecOpenFile;
	}

	aLogger->Log(_L("Recorder open file successful"));

	duration2 = recorder->Duration();
	aLogger->Log(_L("Opened file: %S"),&fileName);
//	aLogger->Log(_L("Expected duration: %d"),duration.Int64());
//	aLogger->Log(_L("Duration: %d"),duration2.Int64());
	LogTime(_L("Duration:"),duration2.Int64(),aLogger);

	TTimeIntervalMicroSeconds position = recorder->Position();
	TTimeIntervalMicroSeconds position2 = recorder->Position();
	//aLogger->Log(_L("Initial position: %d"),position);
	LogTime(_L("Initial position:"),position,aLogger);


	//Get and set desired positions
	bool loop = true;
	while (loop == true)
	{
		//Obtain relative position(s) from parser and execute test
		TInt relativePos = 0;
		CStifItemParser* item = aParser->GetNextItemLineL(KRelativePosition);
	//	CStifItemParser* item = aParser->GetItemLineL(KRelativePosition);
		CleanupStack::PushL(item);
		if (item)
			if (item->GetInt(KRelativePosition,relativePos) != KErrNone)
				continue;
		CleanupStack::PopAndDestroy(); // item

		if (!item) break;

		//Set position, using position2 which updated position from config file
		position2 = duration2.Int64()*relativePos/100.0;
		recorder->SetPosition(TTimeIntervalMicroSeconds(position2));
		//get position which is the position just seted
		position = recorder->Position();
		aLogger->Log(_L("Relative position: %d"),relativePos);
		//aLogger->Log(_L("Relative position in Microseconds: %d"),position2);
		//aLogger->Log(_L("Read position: %d"),position);

		LogTime(_L("Relative position in Microseconds:"),position2,aLogger);
		LogTime(_L("Read position:"),position,aLogger);

		if (Abs(position.Int64() - position2.Int64()) > errRange.Int64())
		{
			aLogger->Log(_L("Position obtained differs from expected position"));
			error = error || KErrOutOfRange;
		}
	}

	recorder->Close();

	CleanupStack::PopAndDestroy(); // recPos
	CleanupStack::PopAndDestroy(); // recorder
	CleanupStack::PopAndDestroy(); // scheduler

//	aLogger->Log(_L("CRecPositionClip finished"));

	return error;
}

void CRecPositionClip::ConstructL()
{

}

void CRecPositionClip::MoscoStateChangeEvent(CBase* /*aObject*/, TInt /*aPreviousState*/, TInt /*aCurrentState*/, TInt /*aErrorCode*/)
{
	//logger->Log(_L("MoscoStateChangeEvent called"));
}

