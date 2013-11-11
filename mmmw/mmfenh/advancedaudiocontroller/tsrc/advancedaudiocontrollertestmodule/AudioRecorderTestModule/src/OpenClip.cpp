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


#include "OpenClip.h"

const TInt KOpenFile		= 1;
const TInt KOpenDescriptor	= 2;
const TInt KOpenURL			= 3;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COpenClip::COpenClip(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
	callbackErr = KErrNone;
}

COpenClip::~COpenClip()
{

}

COpenClip* COpenClip::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	COpenClip* self = new (ELeave) COpenClip(aConsole, aLogger);
    CleanupStack::PushL(self);

    self->ConstructL();

	CleanupStack::Pop(self);
    return self;
}

TInt COpenClip::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
{
	//Creating scheduler
	aLogger->Log(_L("Creating scheduler"));

	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	//Creating COpenClip
	aLogger->Log(_L("Creating COpenClip"));

	COpenClip* selfObj = COpenClip::NewL(aConsole, aLogger);
	CleanupStack::PushL(selfObj);

	//Creating recorder
	aLogger->Log(_L("Creating CMdaAudioRecorderUtility"));

	CMdaAudioRecorderUtility* recorder = CMdaAudioRecorderUtility::NewL(*selfObj);
	CleanupStack::PushL(recorder);

	TInt error = KErrNone;
	TPtrC fileName;
	TTimeIntervalMicroSeconds duration = TInt64(0);
	TTimeIntervalMicroSeconds errRange = TInt64(KDefaultErrorRange);

	TBuf<5> nextItem;
	TInt itemCounter = 0;
	nextItem.Format(_L("%d"),++itemCounter);
	TInt opType;

	//Get error range
	CStifItemParser* item = aParser->GetItemLineL(KErrorRange, ENoTag);

	CleanupStack::PushL(item);
	errRange = COpenClip::ParseTime(item);
	CleanupStack::PopAndDestroy();

	LogTime(_L("Error range:"),errRange.Int64(),aLogger);

	item = aParser->GetItemLineL(nextItem, ENoTag);

	if (!item)
	{
		aLogger->Log(_L("No file provided"));
		CleanupStack::PopAndDestroy(3);
		return KErrSyntax;
	}

	//Cicle to get files and open them
	bool loop = true;
	while (loop == true)
	{
		CleanupStack::PushL(item);

		//Get file name
		if (COpenClip::ParseTag(item, opType, fileName, duration) != KErrNone)
		{
			aLogger->Log(_L("Invalid tag"));
			CleanupStack::PopAndDestroy(4);
			return KErrSyntax;
		}

		switch (opType)
		{
		case KOpenFile: aLogger->Log(_L("Opening file: %S"),&fileName); break;
		case KOpenDescriptor: aLogger->Log(_L("Opening descriptor: %S"),&fileName); break;
		case KOpenURL: aLogger->Log(_L("Opening URL: %S"),&fileName); break;
		}

		if (duration.Int64() == 0)
		{
			aLogger->Log(_L("Invalid file duration"));
			CleanupStack::PopAndDestroy(4);
			return KErrFileDuration;
		}

		//Create descriptor
		HBufC8* desc = NULL;
		if (opType == KOpenDescriptor)
		{
			TRAPD(err,desc = COpenClip::GetDescriptor8LC(fileName, aLogger));
			if (err != KErrNone || desc == NULL)
			{
				aLogger->Log(_L("Error %d creating descriptor for: %S"),err,&fileName);
				CleanupStack::PopAndDestroy(4);
				return KErrRecOpenFile;
			}
			CleanupStack::PushL(desc);
		}

		//Parse URL params
		TInt iapId = KUseDefaultIap;
		TDesC8 nullMimeType = KNullDesC8;
		TDesC8* mimeType = &nullMimeType;
		if (opType == KOpenURL)
		{
			TPtrC mime;
			if (aParser->GetNextLine(KMimeType,mime,ENoTag) == KErrNone)
			{
				HBufC8* buf = HBufC8::NewL(mime.Length()+1);
				CleanupStack::PushL(buf);
				mimeType = buf;
				TPtr8 ptr(buf->Des());
				ptr.Append(mime);
				ptr.Append('\0');
				aLogger->Log(_L8("Provided Mime type: %S"),mimeType);
			}
			else
				aLogger->Log(_L8("Default Mime type: %S"),mimeType);

			CStifItemParser* item2;
			if ((item2 = aParser->GetNextItemLineL(KIAPID, ETag)) != NULL)
			{
				item2->GetInt(KIAPID,iapId);
				aLogger->Log(_L("Provided Internet Access Point ID: %d"),iapId);
			}
			else
				aLogger->Log(_L("Default Internet Access Point ID: %d"),iapId);
		}



		//Call open function
		TRAPD(err2,OpenTarget(recorder,fileName,opType,desc,iapId,*mimeType,aLogger));
		if (err2 != KErrNone)
		{
			aLogger->Log(_L("Error %d opening: %S"),err2,&fileName);
			if (desc == NULL || *mimeType != KNullDesC8)
				CleanupStack::PopAndDestroy(4);
			else
				CleanupStack::PopAndDestroy(5);
			return KErrRecOpenFile;
		}
		aLogger->Log(_L("Open OK!!! "));
		CActiveScheduler::Start();

		aLogger->Log(_L("Opened: %S with error %d"),&fileName,selfObj->callbackErr);
		error = error || selfObj->callbackErr;

		recorder->Close();
		if (desc != NULL) CleanupStack::PopAndDestroy();
		if (*mimeType != KNullDesC8) CleanupStack::PopAndDestroy();
		CleanupStack::PopAndDestroy(); // item

		//Obtain next file name
		nextItem.Format(_L("%d"),++itemCounter);
		if ((item = aParser->GetItemLineL(nextItem, ENoTag)) == NULL)
			break;
	}

	CleanupStack::PopAndDestroy(); // selfObj
	CleanupStack::PopAndDestroy(); // recorder
	CleanupStack::PopAndDestroy(); // scheduler

//	aLogger->Log(_L("COpenClip finished"));

	return error;
}

void COpenClip::OpenTarget(CMdaAudioRecorderUtility* rec, TPtrC fileName, TInt type, HBufC8* desc, TInt iapid, TDesC8& mime, CStifLogger* /*iLogger*/)
{
	if (type == KOpenFile)
	{
		rec->OpenFileL(fileName);
		return;
	}

	if (type == KOpenDescriptor)
	{
		rec->OpenDesL(*desc);
		return;
	}

	if (type == KOpenURL)
	{
		rec->OpenUrlL(fileName,iapid,mime);
		return;
	}

	User::Leave(KErrSyntax);
}

TInt COpenClip::ParseTag(CStifItemParser* item, TInt & type, TPtrC& name, TTimeIntervalMicroSeconds& duration)
{
	TInt sec=0, mil=0, mic=0;

	type = 0;
	if (item->GetString(KFileName,name) == KErrNone)
		type = KOpenFile;
	else
	{
		if (item->GetString(KDescriptor,name) == KErrNone)
			type = KOpenDescriptor;
		else
		{
			if (item->GetString(KURL,name) == KErrNone)
				type = KOpenURL;
		}
	}

	if (type == 0) return KErrSyntax;

	item->GetInt(name,sec);
	item->GetNextInt(mil);
	item->GetNextInt(mic);

	duration = TInt64(sec*1000000 + mil*1000 + mic);

	return KErrNone;
}

TDesC8* COpenClip::ConvertTDesC(TDesC16& string)
{
	HBufC8* buf = HBufC8::NewL(string.Length()+1);
	TPtr8 ptr(buf->Des());
	ptr.Copy(string);
	return buf;
}

void COpenClip::ConstructL()
{

}

void COpenClip::MoscoStateChangeEvent(CBase* /*aObject*/, TInt /*aPreviousState*/, TInt aCurrentState, TInt aErrorCode)
{
	logger->Log(_L("MoscoStateChangeEvent called"));

	if(aErrorCode == KErrNone && aCurrentState ==  CMdaAudioClipUtility::EOpen)
	{
		CActiveScheduler::Stop();
	}
	else
	{
		callbackErr = aErrorCode;
		CActiveScheduler::Stop();
	}
}

HBufC8 *COpenClip::GetDescriptor8LC(const TFileName &aFileName, CStifLogger *iLogger)
{
	TInt FileError, SoundSize=0;
	RFile file;
	RFs FsSession;
	HBufC8 *SoundFile;

	iLogger->Log(_L("Connecting to File Server Session") );
	FileError = FsSession.Connect();
	if (FileError)
		{
		iLogger->Log(_L("File error (%d)") , FileError);
		User::LeaveIfError(FileError);
		}

	iLogger->Log(_L("Opening file to get size") );
	FileError = file.Open(FsSession, aFileName, EFileStream);
	if (FileError)
		{
		iLogger->Log(_L("File error (%d)") , FileError);
		User::LeaveIfError(FileError);
		}
	FileError = file.Size(SoundSize);
	iLogger->Log(_L("File Size (%d)") , SoundSize);
	if (FileError)
		{
		iLogger->Log(_L("Error getting size") );
		User::LeaveIfError(FileError);
		}

	SoundFile = HBufC8::NewL(SoundSize);
	TPtr8 helper = SoundFile->Des();
	FileError = file.Read( helper );

	iLogger->Log(_L("Descriptor length [%d]") , SoundFile->Size() );
	file.Close();
	FsSession.Close();
	return SoundFile;
}


