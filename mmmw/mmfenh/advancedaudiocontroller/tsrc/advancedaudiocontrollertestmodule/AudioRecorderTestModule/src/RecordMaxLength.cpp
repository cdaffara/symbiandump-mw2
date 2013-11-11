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

#include "RecordMaxLength.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecordMaxLength::CRecordMaxLength(CTestModuleIf *aConsole, CStifLogger *aLogger):CRecordingBase(aConsole,aLogger)
{
	errorRange = 1000;
	timeAvailable = 0;
}

CRecordMaxLength::~CRecordMaxLength()
{

}

/*CRecordMaxLength* CRecordMaxLength::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	CRecordMaxLength* self = new (ELeave) CRecordMaxLength(aConsole, aLogger);
    CleanupStack::PushL(self);

    self->ConstructL();

	CleanupStack::Pop(self);
    return self;
}*/

TInt CRecordMaxLength::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter)
{
	TInt error = KErrNone;

	aLogger->Log(_L("Run CRecordingBase::RunTestL"));
	error = CRecordingBase::RunTestL(aConsole,aLogger,aParser,clipCounter,CRecordMaxLength::CreateObjectL);

	return error;
}



CRecordingBase* CRecordMaxLength::CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	//aLogger->Log(_L("CRecordTimeAvailable::CreateObjectL"));
	return (new (ELeave) CRecordMaxLength(aConsole, aLogger));
}


TInt CRecordMaxLength::PreProcess(CStifSectionParser *aParser)
{
	if (recorder)
	{
		recorder->SetAudioDeviceMode(CMdaAudioRecorderUtility::ELocal);
		recorder->SetGain(recorder->MaxGain());
		recorder->SetPosition(TTimeIntervalMicroSeconds(TInt64(0)));
		// Crop delete the whole file. This made the new content is not attached in the end.
		// It over write the previous file.
		logger->Log(_L("Do the Crop to delete the file"));
		TRAPD(err,recorder->CropL());
		if (err != KErrNone) logger->Log(_L("Error cropping file: %d"),err);

		//Get error range
		CStifItemParser* item = aParser->GetItemLineL(KErrorRange);
		CleanupStack::PushL(item);
	//	errorRange = CRecordingBase::ParseTime(item);
		item->GetInt(KErrorRange,errorRange);
		CleanupStack::PopAndDestroy();
		logger->Log(_L("ErrRange defined in config. is: %d"),errorRange);

	//	timeAvailable = recorder->RecordTimeAvailable();
	//	LogTime(_L("Record time available:"),timeAvailable);
	}
	return KErrNone;
}

TInt CRecordMaxLength::PostProcess(CStifSectionParser* /*aParser*/)
{

	TInt error = KErrNone;

    TInt size;


	size = recordingFileName.Size();
	logger->Log(_L("The recorded file size is: %d"),size);
	if ( Abs( maxFileSize - size) > errorRange )
	{
		logger->Log(_L("File size  left differs from expected file size"));
		error = error || KErrOutOfRange;
		return error;
	}



	return error;
}