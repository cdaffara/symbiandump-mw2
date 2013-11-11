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

#include "RecordTimeAvailable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecordTimeAvailable::CRecordTimeAvailable(CTestModuleIf *aConsole, CStifLogger *aLogger):CRecordingBase(aConsole,aLogger)
{
	errorRange = TInt64(KDefaultErrorRange);
	timeAvailable = 0;
}

CRecordTimeAvailable::~CRecordTimeAvailable()
{

}

CRecordingBase* CRecordTimeAvailable::CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	//aLogger->Log(_L("CRecordTimeAvailable::CreateObjectL"));
	return (new (ELeave) CRecordTimeAvailable(aConsole, aLogger));
}

TInt CRecordTimeAvailable::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter)
{
	return CRecordingBase::RunTestL(aConsole,aLogger,aParser,clipCounter,CRecordTimeAvailable::CreateObjectL);
}

TInt CRecordTimeAvailable::PreProcess(CStifSectionParser *aParser)
{
	if (recorder)
	{
//		recorder->SetAudioDeviceMode(CMdaAudioRecorderUtility::ELocal);
//		recorder->SetGain(recorder->MaxGain());
//		recorder->SetPosition(TTimeIntervalMicroSeconds(TInt64(0)));
		// Crop delete the whole file. This made the new content is not attached in the end.
		// It over write the previous file.
//		TRAPD(err,recorder->CropL());
//		if (err != KErrNone) logger->Log(_L("Error cropping file: %d"),err);

		//Get error range
		CStifItemParser* item = aParser->GetItemLineL(KErrorRange, ENoTag);
		CleanupStack::PushL(item);
		errorRange = CRecordingBase::ParseTime(item);
		CleanupStack::PopAndDestroy();
		LogTime(_L("Error range:"),errorRange);

//		timeAvailable = recorder->RecordTimeAvailable();
//		LogTime(_L("Record time available:"),timeAvailable);
	}
	return KErrNone;
}

TInt CRecordTimeAvailable::PostProcess(CStifSectionParser* /*aParser*/)
{
	TTimeIntervalMicroSeconds newTimeAvailable = recorder->RecordTimeAvailable();
	LogTime(_L("Clip duration:"),fileDuration);
	LogTime(_L("New record time available:"),newTimeAvailable);

	TInt64 val = newTimeAvailable.Int64();

	if ((val = Abs(timeAvailable.Int64() - newTimeAvailable.Int64() - fileDuration.Int64())) > errorRange.Int64())
	{
		LogTime(_L("Time available difference is out of specified range:"),val);
		return KErrOutOfRange;
	}

	return KErrNone;
}