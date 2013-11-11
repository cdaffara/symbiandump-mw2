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


#include "RecordOpen.h"
#include <e32des8.h>

const TInt KOpenFile		= 1;  //File
const TInt KOpenFile2		= 2;  //File with controller
const TInt KOpenRFile		= 3;  //RFile
const TInt KOpenRFile2		= 4;  //RFile with controller
const TInt KOpenSFile		= 5;  //SourceFile
const TInt KOpenSFile2		= 6;  //SourceFile with controller
const TInt KOpenDescriptor	= 7;  //Desc
const TInt KOpenDescriptor2	= 8;  //Desc with const
const TInt KOpenDescriptor3	= 9;  //Desc with controller
const TInt KOpenURL			= 10; //URL
const TInt KOpenURL2		= 11; //URL with controller
const TInt KOpenLocation	= 12; //Open Location

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecordOpen::CRecordOpen(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
	callbackErr = KErrNone;
}

CRecordOpen::~CRecordOpen()
{

}

CRecordOpen* CRecordOpen::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	CRecordOpen* self = new (ELeave) CRecordOpen(aConsole, aLogger);
    CleanupStack::PushL(self);

    self->ConstructL();

	CleanupStack::Pop(self);
    return self;
}

TInt CRecordOpen::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
{
	//Creating scheduler
	aLogger->Log(_L("Creating scheduler"));

	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	//Creating CRecordOpen
	aLogger->Log(_L("Creating CRecordOpen"));

	CRecordOpen* selfObj = CRecordOpen::NewL(aConsole, aLogger);
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
	errRange = CRecordOpen::ParseTime(item);
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
		if (CRecordOpen::ParseTag(item, opType, fileName, duration) != KErrNone)
		{
			aLogger->Log(_L("Invalid tag"));
			CleanupStack::PopAndDestroy(4);
			return KErrSyntax;
		}

		switch (opType)
		{
		case KOpenFile: aLogger->Log(_L("Opening file: %S"),&fileName); break;
		case KOpenFile2: aLogger->Log(_L("Opening file with controller: %S"),&fileName); break;
		case KOpenRFile: aLogger->Log(_L("Opening Rfile: %S"),&fileName); break;
		case KOpenRFile2: aLogger->Log(_L("Opening Rfile with controller: %S"),&fileName); break;
		case KOpenSFile: aLogger->Log(_L("Opening source file: %S"),&fileName); break;
		case KOpenSFile2: aLogger->Log(_L("Opening source file with controller: %S"),&fileName); break;
		case KOpenDescriptor: aLogger->Log(_L("Opening descriptor: %S"),&fileName); break;
		case KOpenDescriptor2: aLogger->Log(_L("Opening const descriptor: %S"),&fileName); break;
		case KOpenDescriptor3: aLogger->Log(_L("Opening descriptor with controller: %S"),&fileName); break;
		case KOpenURL: aLogger->Log(_L("Opening URL: %S"),&fileName); break;
		case KOpenURL2: aLogger->Log(_L("Opening URL with controller: %S"),&fileName); break;
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
			TRAPD(err,desc = CRecordOpen::GetDescriptor8LC(fileName, aLogger));
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
				if (err2 == KErrNotSupported) 
				{
					return KErrNone;
				}
				else
				{
					return KErrRecOpenFile;
				}
		}
		aLogger->Log(_L("Open OK!!! "));
		CActiveScheduler::Start();

		aLogger->Log(_L("Opened: %S with error %d"),&fileName,selfObj->callbackErr);
		if (selfObj->callbackErr == KErrNotSupported || selfObj->callbackErr == KErrNotFound) 
		{
			error = KErrNone;
		}
		else
		{
			error = error || selfObj->callbackErr;
		}

		recorder->Close();
		if (desc != NULL) 
		{
			aLogger->Log(_L("Clean descriptor buffer "));
			CleanupStack::PopAndDestroy();
		}
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

//	aLogger->Log(_L("CRecordOpen finished"));

	return error;
}

void CRecordOpen::OpenTarget(CMdaAudioRecorderUtility* rec, TPtrC fileName, TInt type, HBufC8* desc, TInt iapid, TDesC8& mime, CStifLogger* aLogger)
{
	aLogger->Log(_L("openTarget with type %d"),type);

	//File
	if (type == KOpenFile)
	{
		rec->OpenFileL(fileName);
		return;
	}

	//File with controller
	if (type == KOpenFile2)
	{
		//TInt aUid = 1;
		//TUid aRecordControllerUid = TUid::Uid(aUid);
		TUid aRecordControllerUid = KNullUid;//KUidInterfaceMMFController;//TUid::Uid(KMmfUidPluginInterfaceFormatDecode);//KMmfUidPluginInterfaceController;//{0x10203827};
		//const TUid KUidBassBoostEffect = {0x10203827};

		TUid aPlaybackControllerUid = KNullUid;
		TUid aFormatUid = KNullUid;
		TFourCC aCodec = NULL;
		rec->OpenFileL(fileName, aRecordControllerUid, aPlaybackControllerUid, aFormatUid, aCodec);
		return;
	}
	//RFile
	if (type == KOpenRFile)
	{
		RFs aFs;
		RFile aFileHandler;
		User::LeaveIfError(aFs.Connect());

		User::LeaveIfError(aFileHandler.Open( aFs, fileName, EFileRead));
		rec->OpenFileL(aFileHandler);

		aFileHandler.Close();
		aFs.Close();

		return;
	}


	//RFile with controller
	if (type == KOpenRFile2)
	{
		TUid aRecordControllerUid = KNullUid;
		TUid aPlaybackControllerUid = KNullUid;
		TUid aFormatUid = KNullUid;
		TFourCC aCodec = NULL;

		RFs aFs;
		RFile aFileHandler;
		User::LeaveIfError(aFs.Connect());

		User::LeaveIfError(aFileHandler.Open( aFs, fileName, EFileRead));
		rec->OpenFileL(aFileHandler, aRecordControllerUid, aPlaybackControllerUid, aFormatUid, aCodec);

		aFileHandler.Close();
		aFs.Close();

		return;
	}

	//SourceFile
	if (type == KOpenSFile)
	{


	    RFs aFs;
		RFile aFileHandler;
		User::LeaveIfError(aFs.Connect());

		User::LeaveIfError(aFileHandler.Open( aFs, fileName, EFileRead));

		//TMMSource aSourceFile;
		TMMFileHandleSource filesource(aFileHandler, ContentAccess::KDefaultContentObject, ContentAccess::EPlay);
		rec->OpenFileL(filesource);
		aFileHandler.Close();  
		aFs.Close();
		return;
	}

	//SourceFile with controller
	if (type == KOpenSFile2)
	{
		TUid aRecordControllerUid = KNullUid;
		TUid aPlaybackControllerUid = KNullUid;
		TUid aFormatUid = KNullUid;
		TFourCC aCodec = NULL;

		RFs aFs;
		RFile aFileHandler;
		User::LeaveIfError(aFs.Connect());
		User::LeaveIfError(aFileHandler.Open( aFs, fileName, EFileRead));

		//TMMSource aSourceFile;
		TMMFileHandleSource filesource(aFileHandler, ContentAccess::KDefaultContentObject, ContentAccess::EPlay);



		rec->OpenFileL(filesource,aRecordControllerUid, aPlaybackControllerUid, aFormatUid, aCodec);
		aFileHandler.Close();
		aFs.Close();
		return;
	}

	//Desc
	if (type == KOpenDescriptor)
	{

       	TPtrC fileNamePtr;
    	RFile file;
    	RFs fileServer;
    	HBufC8* buffer = NULL;
      	User::LeaveIfError(fileServer.Connect());
      	TInt err = file.Open(fileServer, fileName, EFileShareReadersOnly);
        if (err != KErrNone)
        {

            file.Close();
            fileServer.Close();
            User::Leave(err);
        }

        TInt size;


        if ((err = file.Size(size)) == KErrNone)
        {
            TRAP(err, buffer = HBufC8::NewL(size));
            if (err != KErrNone)
            {

                file.Close();
                fileServer.Close();
                User::Leave(err);
            }

            TPtr8 bufferpointer = buffer->Des();
            if ((err = file.Read(bufferpointer)) != KErrNone)
            {

                file.Close();
                fileServer.Close();
                delete buffer;
                User::Leave(err);
            }
   			rec->OpenDesL (bufferpointer);
        }


                file.Close();  
                fileServer.Close();
//		rec->OpenDesL (*desc);
		return;
	}

	//Desc with const
	if (type == KOpenDescriptor2)
	{
		aLogger->Log(_L("KOpenDescriptor2 Desc with const "));

		TPtrC fileNamePtr;
    	RFile file;
    	RFs fileServer;
    	HBufC8* buffer = NULL;
      	User::LeaveIfError(fileServer.Connect());
      	TInt err = file.Open(fileServer, fileName, EFileShareReadersOnly);
        if (err != KErrNone)
        {

            file.Close();
            fileServer.Close();
            User::Leave(err);
        }

        TInt size;

        if ((err = file.Size(size)) == KErrNone)
        {
            TRAP(err, buffer = HBufC8::NewL(size));
            if (err != KErrNone)
            {

                file.Close();
                fileServer.Close();
                User::Leave(err);
            }

            TPtr8 bufferpointer = buffer->Des();
            if ((err = file.Read(bufferpointer)) != KErrNone)
            {

                file.Close();
                fileServer.Close();
                delete buffer;
                User::Leave(err);
            }

        }

		rec->OpenDesL (*buffer);
		            file.Close();  
                fileServer.Close();
		return;
	}

	//Desc with controller
	if (type == KOpenDescriptor3)
	{
		aLogger->Log(_L("KOpenDescriptor3 Desc with controller "));
		TUid aRecordControllerUid = KUidInterfaceMMFController;
		TUid aPlaybackControllerUid = KNullUid;
		TUid aFormatUid = KNullUid;
		TFourCC aCodec = NULL;

		TPtrC fileNamePtr;
    	RFile file;
    	RFs fileServer;
    	HBufC8* buffer = NULL;
      	User::LeaveIfError(fileServer.Connect());
      	TInt err = file.Open(fileServer, fileName, EFileShareReadersOnly);
        if (err != KErrNone)
        {

            file.Close();
            fileServer.Close();
            User::Leave(err);
        }

        TInt size;

        if ((err = file.Size(size)) == KErrNone)
        {
            TRAP(err, buffer = HBufC8::NewL(size));
            if (err != KErrNone)
            {

                file.Close();
                fileServer.Close();
                User::Leave(err);
            }

            TPtr8 bufferpointer = buffer->Des();
            if ((err = file.Read(bufferpointer)) != KErrNone)
            {

                file.Close();
                fileServer.Close();
                delete buffer;
                User::Leave(err);
            }
   			rec->OpenDesL(bufferpointer,aRecordControllerUid,aPlaybackControllerUid,aFormatUid, aCodec);
   					    file.Close();  
                fileServer.Close();
        }

		return;
	}

	//URL
	if (type == KOpenURL)
	{
		rec->OpenUrlL(fileName,iapid,mime);
		return;
	}

	//URL with controller
	if (type == KOpenURL2)
	{
		TUid aRecordControllerUid = KNullUid;
		TUid aPlaybackControllerUid = KNullUid;
		TUid aFormatUid = KNullUid;
		TFourCC aCodec = NULL;

		rec->OpenUrlL(fileName,iapid,aRecordControllerUid,aPlaybackControllerUid,aFormatUid, aCodec);
		return;
	}

	if (type == KOpenLocation)
	{

		TMdaClipFormat dummyFormat;
		TMdaPackage* codc = NULL;
		TMdaPackage* codc1 = NULL;

		TMdaFileClipLocation* fileSrc= new TMdaFileClipLocation( fileName );
		rec->OpenL( fileSrc, &dummyFormat, codc, codc1 );
		return;
	}

	User::Leave(KErrSyntax);
}

TInt CRecordOpen::ParseTag(CStifItemParser* item, TInt & type, TPtrC& name, TTimeIntervalMicroSeconds& duration)
{
	TInt sec=0, mil=0, mic=0;

	type = 0;
	if (item->GetString(KFileName,name) == KErrNone)
		type = KOpenFile;
	else if (item->GetString(KFileName2,name) == KErrNone)
		type = KOpenFile2;
	else if (item->GetString(KRFileName,name) == KErrNone)
		type = KOpenRFile;
	else if (item->GetString(KRFileName2,name) == KErrNone)
		type = KOpenRFile2;
	else if (item->GetString(KSFileName,name) == KErrNone)
		type = KOpenSFile;
	else if (item->GetString(KSFileName2,name) == KErrNone)
		type = KOpenSFile2;
	else if (item->GetString(KDescriptor,name) == KErrNone)
		type = KOpenDescriptor;
	else if (item->GetString(KDescriptor2,name) == KErrNone)
		type = KOpenDescriptor2;
	else if (item->GetString(KDescriptor3,name) == KErrNone)
		type = KOpenDescriptor3;
	else if (item->GetString(KURL,name) == KErrNone)
		type = KOpenURL;
	else if (item->GetString(KURL2,name) == KErrNone)
		type = KOpenURL2;
	else if (item->GetString(KLocation,name) == KErrNone)
		type = KOpenLocation;

	if (type == 0) return KErrSyntax;

	item->GetInt(name,sec);
	item->GetNextInt(mil);
	item->GetNextInt(mic);

	duration = TInt64(sec*1000000 + mil*1000 + mic);

	return KErrNone;
}

TDesC8* CRecordOpen::ConvertTDesC(TDesC16& string)
{
	HBufC8* buf = HBufC8::NewL(string.Length()+1);
	TPtr8 ptr(buf->Des());
	ptr.Copy(string);
	return buf;
}

void CRecordOpen::ConstructL()
{

}

void CRecordOpen::MoscoStateChangeEvent(CBase* /*aObject*/, TInt /*aPreviousState*/, TInt aCurrentState, TInt aErrorCode)
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

HBufC8 *CRecordOpen::GetDescriptor8LC(const TFileName &aFileName, CStifLogger *iLogger)
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


