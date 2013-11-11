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


#include "RecordBySize.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecordBySize::CRecordBySize(CTestModuleIf *aConsole, CStifLogger *aLogger):CRecordingBase(aConsole,aLogger)
{
	stopByMMF = false;
}

CRecordBySize::~CRecordBySize()
{

}

CRecordingBase* CRecordBySize::CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	//aLogger->Log(_L("CRecordClip::CreateObjectL"));
	return (new (ELeave) CRecordBySize(aConsole, aLogger));
}

TInt CRecordBySize::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter)
{
/*	TInt error = KErrNone;

	//Create scheduler
	aLogger->Log(_L("Creating scheduler"));
	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	CRecordingBase* recorderObj;
	if ((error = CRecordingBase::CreateRecorderL(aConsole, aLogger, aParser, clipCounter, CRecordBySize::CreateObjectL, recorderObj, true, false)) != KErrNone)
	{
		CleanupStack::PopAndDestroy(); //scheduler
		return error;
	}

	CleanupStack::PushL(recorderObj);
	error = CRecordingBase::SimpleRecordL(aConsole, aLogger, recorderObj, true, aParser, false);
	CleanupStack::PopAndDestroy(2); // recorder, scheduler

	return error;
*/
	return CRecordingBase::RunTestL(aConsole,aLogger,aParser,clipCounter,CRecordBySize::CreateObjectL);
}

TInt CRecordBySize::PreProcess(CStifSectionParser *aParser)
{
	TInt err(KErrNone);
	if (recorder)
	{
		//recorder->SetMaxWriteLength(clipSize);
	//	recorder->SetAudioDeviceMode(CMdaAudioRecorderUtility::ELocal);
//		recorder->SetGain(recorder->MaxGain());
//		recorder->SetPosition(TTimeIntervalMicroSeconds(TInt64(0)));
//		TRAPD(err,recorder->CropL());
//		if (err != KErrNone) logger->Log(_L("Error cropping file: %d"),err);

		//Get error range
		errorRange = 0;
		CStifItemParser* item = aParser->GetItemLineL(KErrorRange, ETag);
		CleanupStack::PushL(item);
		if (item)
			item->GetInt(KErrorRange,errorRange);
		CleanupStack::PopAndDestroy();
		logger->Log(_L("Error range: %d bytes"),errorRange);

		//Get recorded audio format
		TPtrC dataFormat(KPPNone);
		item = aParser->GetItemLineL(KDataFormat);
		if (item)
		{
			CleanupStack::PushL(item);
			err = item->GetString(KDataFormat,dataFormat);
			if (err != KErrNone)
			{
				CleanupStack::PopAndDestroy(); // item
				logger->Log(_L("dataFormat syntax error"));
				return err;
			}
			else
			{
				logger->Log(_L("dataFormat: %S"),&dataFormat);
				CleanupStack::PopAndDestroy(); // item
			}
		}

		if (dataFormat == KFormatMp4)
		{
			isMp4 = true;
		}

		//Get file size
	//	clipSize = 0;
	//	item = aParser->GetItemLineL(KFileSize, ETag);
	//	CleanupStack::PushL(item);
	//	if (item)
	//		item->GetInt(KFileSize,clipSize);
	//	CleanupStack::PopAndDestroy();
	//	logger->Log(_L("Expected size: %d bytes"),clipSize);

	//	recorder->SetMaxWriteLength(clipSize);
	}
	return KErrNone;
}

TInt CRecordBySize::PostProcess(CStifSectionParser* /*aParser*/)
{
	if (!stopByMMF)
	{
		logger->Log(_L("Recording wasn't stopped for reaching file size"));
		return KErrRecBySize;
	}

	TTimeIntervalMicroSeconds duration = Duration();
	//logger->Log(_L("Recorded clip duration: %d"),duration);
	LogTime(_L("Recorded clip duration:"),duration);

	TInt realSize = 0, err;
	if ((err=GetFileSize(recordingFileName, realSize)) != KErrNone)
		return err;

	logger->Log(_L("Recorded size: %d bytes"),realSize);
	logger->Log(_L("Expected size: %d bytes"),maxFileSize);
	TInt dif = Abs(maxFileSize - realSize);
	if (dif > errorRange)
	{
		logger->Log(_L("Recorded file size differs from expected size by: %d bytes"),dif);
		return KErrOutOfRange;
	}
	return KErrNone;
}

TInt CRecordBySize::GetFileSize(const TFileName &aFileName, TInt & size)
{
	TInt FileError, SoundSize=0;
	RFile file;
	RFs FsSession;

	logger->Log(_L("Connecting to File Server Session") );
	FileError = FsSession.Connect();
	if (FileError)
	{
		logger->Log(_L("File error (%d)") , FileError);
		return KErrFileSize;
	}

	FileError = file.Open(FsSession, aFileName, EFileStream);
	if (FileError)
	{
		logger->Log(_L("File error (%d)") , FileError);
		return KErrFileSize;
	}

	FileError = file.Size(SoundSize);
	if (FileError)
	{
		logger->Log(_L("Error getting size") );
		return KErrFileSize;
	}

	size = SoundSize;
	file.Close();
	FsSession.Close();
	return KErrNone;
}

void CRecordBySize::MoscoStateChangeEvent(CBase* aObject, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{
//	if (aErrorCode == KErrEof)
	if (aErrorCode == KErrEof || ( aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen &&  aPreviousState == CMdaAudioClipUtility::ERecording ))
	{
		logger->Log(_L("File limit reached, stopped recording"));
		stopByMMF = true;
		recorder->Stop();
		CActiveScheduler::Stop();
	}
	else
		CRecordingBase::MoscoStateChangeEvent(aObject, aPreviousState, aCurrentState, aErrorCode);
}

