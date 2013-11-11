// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// msgservertestutils.cpp
//

#include "msgservertestutils.h"

#include <e32test.h>
#include <f32fsys.h>
#include <s32file.h>
#include <e32uid.h>
#include <txtrich.h>
#include <txtfmlyr.h>
#include <msvids.h>
#include <msvuids.h>
#include <msvruids.h>
#include <e32std.h>
#include <mtclbase.h>
#include <mtclreg.h>
#include <msvapi.h>
#include <mtmuids.h>
#include <smuthdr.h>
#include <miutset.h>
#include <sqldb.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

_LIT(KMsvServerPattern, "!MsvServer*");
_LIT(KMsgServerTestUtilsPanic,"MSGS-TESTS");
#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
_LIT(KDbFileName, "\\messaging.db");
#else
_LIT(KDbFileName, "[1000484B]messaging.db");
#endif


class CDummyObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};
	};


EXPORT_C CMsgServerTestUtils* CMsgServerTestUtils::NewL(RTest& aRTest)
	{
	CMsgServerTestUtils* self = NewLC(aRTest);
	CleanupStack::Pop();
	return self;
	}

EXPORT_C CMsgServerTestUtils* CMsgServerTestUtils::NewLC(RTest& aRTest)
	{
	CMsgServerTestUtils* self = new (ELeave) CMsgServerTestUtils(aRTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}



CMsgServerTestUtils::CMsgServerTestUtils(RTest& aRTest)
: CTestUtils(aRTest)
	{
	iTestNum=0;
	}

CMsgServerTestUtils::~CMsgServerTestUtils()
	{
	delete CActiveScheduler::Current();
	}


void CMsgServerTestUtils::ConstructL()
	{
	CTestUtils::ConstructL();
	}


void CMsgServerTestUtils::Panic(TInt aPanic)
	{
	User::Panic(KMsgServerTestUtilsPanic, aPanic);
	}



EXPORT_C void CMsgServerTestUtils::SetDebugL(TBool aDebug)
	{
	TMsvConfig config;
	GetConfigL(config);
	config.iDebug = aDebug;
	SetConfigL(config);
	}

EXPORT_C void CMsgServerTestUtils::GetConfigL(TMsvConfig& aConfig)
	{
	// try to open system ini
	TUint unused;        
	TChar iDriveChar=FileSession().GetSystemDriveChar();
 	TBuf<2> systemDrive;
 	systemDrive.Append(iDriveChar);
 	systemDrive.Append(KDriveDelimiter);
 	TPath pathName(systemDrive);
	pathName.Append(KServerINIFile);  
 	if(FileSession().Att(pathName,unused)==KErrNone)
		{
		CDictionaryFileStore *store = CDictionaryFileStore::OpenLC(FileSession(),pathName,KNullUid);
		// get configuration from system ini
		if (store->IsPresentL(KUidMsvMessageDriveStream))
			{
			RDictionaryReadStream readStream;
			readStream.OpenLC(*store, KUidMsvMessageDriveStream);
			TInt version = readStream.ReadInt8L();
			iRTest(version <= KMsvMessageDriveStreamVersionNumber);

			TBuf<2> drive;
			readStream >> drive;
			aConfig.iDrive = drive;

			aConfig.iUniqueID = 0;
			aConfig.iDebug = EFalse;

			// The drive Id didn't exist in earlier versions
			if (version > 1)
				aConfig.iUniqueID = readStream.ReadUint32L();

			// This debug frig wasn't in earlier versions
			if (version > 2)
				aConfig.iDebug = readStream.ReadInt8L();

			CleanupStack::PopAndDestroy(); // readStream
			}

		CleanupStack::PopAndDestroy(); // store
                }
	}

EXPORT_C void CMsgServerTestUtils::SetConfigL(TMsvConfig& aConfig)
	{
	TChar iDriveChar=FileSession().GetSystemDriveChar();
 	TBuf<2> systemDrive;
 	systemDrive.Append(iDriveChar);
 	systemDrive.Append(KDriveDelimiter);
    TPath pathName(systemDrive);
 	pathName.Append(KServerINIFile);  
 	CDictionaryFileStore *store = CDictionaryFileStore::OpenLC(FileSession(),pathName,KNullUid);
	RDictionaryWriteStream stream;
	stream.AssignLC(*store, KUidMsvMessageDriveStream);

	stream.WriteUint8L(KMsvMessageDriveStreamVersionNumber); // version number
	stream << aConfig.iDrive.Name();
	stream.WriteUint32L(aConfig.iUniqueID);
	stream.WriteInt8L(aConfig.iDebug);

	stream.CommitL();
	store->CommitL();
	CleanupStack::PopAndDestroy(2); // stream, store
	}



EXPORT_C void CMsgServerTestUtils::SetupL()
	{
	CActiveScheduler::Install(new(ELeave)CTestScheduler);
	iRTest.Printf(_L("Cleaning Message Folder\n"));
	CleanMessageFolderL();
	SetDriveL(EDriveC);
	SetDebugL(ETrue);

	// Try and find the removable drive
	for (iRemovableDrive = EDriveY; iRemovableDrive > EDriveD; iRemovableDrive = TInt(iRemovableDrive) - 1)
		{
		TVolumeInfo info;
		if (FileSession().Volume(info, TInt(iRemovableDrive)) == KErrNone)
			break;
		}
	}


EXPORT_C void CMsgServerTestUtils::SetDriveL(TInt aDrive, TBool aStoreId)
	{
	TMsvConfig config;
	GetConfigL(config);

	config.iDrive = aDrive;

	if (aStoreId)
		config.iUniqueID = (TUint)-1;
	else
		config.iUniqueID = 0;

	SetConfigL(config);
	}


EXPORT_C void CMsgServerTestUtils::Close(TInt aRet)
	{
	if (!aRet)
		TestHarnessCompleted();
	else
		TestHarnessFailed(aRet);
	}


EXPORT_C void CMsgServerTestUtils::CleanMessageFolderL(const TDesC& aDes)
	{
	CFileMan* fileMan = CFileMan::NewL(FileSession());
	CleanupStack::PushL(fileMan);

	TParse parse;
	TFileName fileName((aDes.Length() == 0) ? KMsvDefaultFolder2 : aDes);
	TMsvConfig config;

	TChar driveChar=FileSession().GetSystemDriveChar();
 	TBuf<4> drive;
 	drive.Append(driveChar);
 	drive.Append(KDriveDelimiter);
	config.iDrive=TDriveUnit(drive);
	GetConfigL(config);
	TDriveName name=config.iDrive.Name();
	parse.Set(fileName, &name, NULL);

	TInt error = fileMan->RmDir(parse.DriveAndPath());
	CleanupStack::PopAndDestroy(); // fileMan

	error = FileSession().RmDir(parse.DriveAndPath());
	if (!(error == KErrNotFound || error == KErrNone || error==KErrPathNotFound))
		{
		TPtrC dir(parse.DriveAndPath());
		iRTest.Printf(TRefByValue<const TDesC>(_L("Directory %S cannot be removed\n Exiting - press any key...\n")), &dir);
		User::Leave(KErrNotReady);
		}

	//delete DB
	TDriveUnit driveUnit(drive);
 	TPath pathName(driveUnit.Name());
 	pathName.Append(KDbFileName);
 	RSqlDatabase::Delete(pathName);
	}


EXPORT_C CMsvServer* CMsgServerTestUtils::CreateServerL(const TDesC& aIndexFile)
	{	
	TDesC dummy = aIndexFile; //dummy to avoid test build error when aIndexFile is not used inside
	CMsvServer* server = CMsvServer::NewL();
	CleanupStack::PushL(server);

	CTestTimer* timer = CTestTimer::NewL();
	timer->After(100000);
	CActiveScheduler::Start();
	delete timer;

	CleanupStack::Pop(); // server
	return server;
	}


EXPORT_C void CMsgServerTestUtils::FileExists(const TDesC& aDes)
	{
	TUint at;
	iRTest(FileSession().Att(aDes, at)==KErrNone);
	}

EXPORT_C void CMsgServerTestUtils::FileMissing(const TDesC& aDes)
	{
	TUint at;
	TInt error = FileSession().Att(aDes, at);
	iRTest(error==KErrNotFound || error==KErrPathNotFound);
	}


EXPORT_C void CMsgServerTestUtils::CreateDataFilesL(const TDesC& aFilename, const TDesC& aMtmName, TUid aUid, const TDesC& aServerMtmFileName, const TDesC& aClientMtmFileName)
	{
	CMtmDllInfoArray* mtmdllinfoarray=new(ELeave) CMtmDllInfoArray();
	CleanupStack::PushL(mtmdllinfoarray);

	CMtmDllInfo* mtmdllinfo1=CMtmDllInfo::NewL(aMtmName, TUidType(KDynamicLibraryUid, KUidMtmServerComponent, TUid::Uid(KUidMtmDefaultSpecificVal)),aServerMtmFileName,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo1);

	CMtmDllInfo* mtmdllinfo2=CMtmDllInfo::NewL(aMtmName, TUidType(KDynamicLibraryUid, KUidMtmClientComponent, TUid::Uid(KUidMtmDefaultSpecificVal)),aClientMtmFileName,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo2);

	// Create an empty capability set for creating a new group data object
	TCapabilitySet capSet;
	capSet.SetEmpty();
	CleanupStack::Pop(mtmdllinfoarray); // next line takes ownership
	CMtmGroupData* mtmgroupdata=CMtmGroupData::NewL(aUid,aUid, mtmdllinfoarray, capSet);
	CleanupStack::PushL(mtmgroupdata);

	CFileStore* filestore = CPermanentFileStore::ReplaceLC(FileSession(), aFilename, EFileShareExclusive|EFileStream|EFileWrite);
	TUidType uidtype(KPermanentFileStoreLayoutUid,KUidMsvDataComponent, aUid);
	filestore->SetTypeL(uidtype);
	RStoreWriteStream out;
	TStreamId streamid=out.CreateLC(*filestore);							// Push to stack
	mtmgroupdata->ExternalizeL(out);
	out.CommitL();
	CleanupStack::PopAndDestroy(); // out
	filestore->SetRootL(streamid);
	filestore->CommitL();
	CleanupStack::PopAndDestroy(); // filestore
	
	CleanupStack::PopAndDestroy();  // mtmgroupdata
	}

EXPORT_C void CMsgServerTestUtils::InstallMtmL(const TDesC& aDataFile, const TDesC& aMtmName, TUid aId, const TDesC& aServerMtmFileName, const TDesC& aClientMtmFileName)
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	CreateDataFilesL(aDataFile, aMtmName, aId, aServerMtmFileName, aClientMtmFileName); 
	session->InstallMtmGroup(aDataFile);

	CleanupStack::PopAndDestroy(2); // session, ob
	}

EXPORT_C TBool CMsgServerTestUtils::WaitForKeyL(TTimeIntervalMicroSeconds32 aDelay, TKeyCode* aKeyCode)
	{
	iRTest(iRTest.Console() != NULL);

	// Create timer
	CTestTimer* timer = CTestTimer::NewL();
	CleanupStack::PushL(timer);
	timer->After(aDelay);

	CTestActive* wait = new(ELeave)CTestActive;
	CleanupStack::PushL(wait);
	wait->StartL();

	// Wait for key press
	iRTest.Console()->Read(wait->iStatus);
	CActiveScheduler::Start();

	// If timer still active a key was pressed
	TBool keyPressed = timer->IsActive();

	if (keyPressed)
		{
		// Get the key pressed
		if (aKeyCode)
			*aKeyCode = iRTest.Console()->KeyCode();

		// Cancel timer
		timer->Cancel();
		}
	else
		{
		// Cancel wait for character
		iRTest.Console()->ReadCancel();
		User::WaitForRequest(wait->iStatus);
		}

	CleanupStack::PopAndDestroy(2); // wait, timer
	return keyPressed;
	}


EXPORT_C void CMsgServerTestUtils::WaitForServerClose()
	{
	//Kill the message server if its running
	TFindProcess findprocess(KMsvServerPattern);
	TFullName name;
	if(findprocess.Next(name)==KErrNone)
		{
		RProcess process;
    	User::LeaveIfError(process.Open(findprocess));
     	process.Kill(KErrCancel) ;
		process.Close() ;
		}
		
	FOREVER
		{
		TFullName name;
		TFindProcess find(KMsvServerPattern);
		if (find.Next(name) != KErrNone)
			break;
		User::After(100000);
		}
	}


EXPORT_C void CMsgServerTestUtils::RemoveMailFolderL(TInt aDrive)
	{
	CFileMan* fileMan = CFileMan::NewL(FileSession());
	CleanupStack::PushL(fileMan);

	TDriveUnit drive(aDrive);
	TPtrC ptr(drive.Name());

	TParse parse;
#ifdef __MESSAGING_API_V2
	iRTest(parse.Set(ptr, &KMsvDefaultIndexFile2(), NULL) == KErrNone);
#else
	iRTest(parse.Set(ptr, &KMsvDefaultIndexFile(), NULL) == KErrNone);
#endif

	TInt error = fileMan->RmDir(parse.DriveAndPath());
	iRTest(error == KErrNone || error == KErrPathNotFound);

	CleanupStack::PopAndDestroy();
	}

EXPORT_C void CMsgServerTestUtils::RemoveIndexFile(TInt aDrive)
	{
	TDriveUnit drive(aDrive);
	TPtrC ptr(drive.Name());

	TParse parse;
#ifdef __MESSAGING_API_V2
	iRTest(parse.Set(ptr, &KMsvDefaultIndexFile2(), NULL) == KErrNone);
#else
	iRTest(parse.Set(ptr, &KMsvDefaultIndexFile(), NULL) == KErrNone);
#endif

	iRTest(FileSession().Delete(parse.FullName()) == KErrNone);
	}

EXPORT_C void CMsgServerTestUtils::CorruptIndexFileL(TInt aMode, const TDesC& aDes)
	{
	switch (aMode)
		{
		case 0: // uicode/narrow conflict
			{
#ifdef __MESSAGING_API_V2
			FileSession().Delete((aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes);
			CPermanentFileStore* indexStore = CPermanentFileStore::CreateL(FileSession(), (aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes, EFileShareExclusive|EFileWrite);
#else
			FileSession().Delete((aDes.Length() == 0) ? KMsvDefaultIndexFile : aDes);
			CPermanentFileStore* indexStore = CPermanentFileStore::CreateL(FileSession(), (aDes.Length() == 0) ? KMsvDefaultIndexFile : aDes, EFileShareExclusive|EFileWrite);
#endif
			indexStore->SetTypeL(TUidType(KPermanentFileStoreLayoutUid, KNullUid)); // 2nd uid should be KUidMsvIndexFile
			indexStore->CommitL();
			delete indexStore;
			break;
			}
		case 1: // empty index file
			{
#ifdef __MESSAGING_API_V2
			FileSession().Delete((aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes);
			CPermanentFileStore* indexStore = CPermanentFileStore::CreateL(FileSession(), (aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes, EFileShareExclusive|EFileWrite);
#else
			FileSession().Delete((aDes.Length() == 0) ? KMsvDefaultIndexFile : aDes);
			CPermanentFileStore* indexStore = CPermanentFileStore::CreateL(FileSession(), (aDes.Length() == 0) ? KMsvDefaultIndexFile : aDes, EFileShareExclusive|EFileWrite);
#endif
			indexStore->SetTypeL(TUidType(KPermanentFileStoreLayoutUid, KUidMsvIndexFile)); // 2nd uid should be KUidMsvIndexFile
			indexStore->CommitL();
			delete indexStore;
			break;
			}
		case 2: // missing index stream
			{
#ifdef __MESSAGING_API_V2
			FileSession().Delete((aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes);
			CPermanentFileStore* indexStore = CPermanentFileStore::CreateL(FileSession(), (aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes, EFileShareExclusive|EFileWrite);
#else
			FileSession().Delete((aDes.Length() == 0) ? KMsvDefaultIndexFile : aDes);
			CPermanentFileStore* indexStore = CPermanentFileStore::CreateL(FileSession(), (aDes.Length() == 0) ? KMsvDefaultIndexFile : aDes, EFileShareExclusive|EFileWrite);
#endif
			indexStore->SetTypeL(TUidType(KPermanentFileStoreLayoutUid, KUidMsvIndexFile)); // 2nd uid should be KUidMsvIndexFile
			//
			RStoreWriteStream writeStream;
			TStreamId rootIndexId = writeStream.CreateLC(*indexStore);
			writeStream.WriteInt32L(TInt(rootIndexId.Value()+100));
			writeStream.WriteInt32L(TInt(rootIndexId.Value()+101));
			writeStream.CommitL();
			CleanupStack::PopAndDestroy(); // writeStream
			indexStore->SetRootL(rootIndexId);
			//	
			indexStore->CommitL();
			delete indexStore;
			break;
			}
		case 3: // missing entry stream
			{
#ifdef __MESSAGING_API_V2
			FileSession().Delete((aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes);
			CPermanentFileStore* indexStore = CPermanentFileStore::CreateL(FileSession(), (aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes, EFileShareExclusive|EFileWrite);
#else
			FileSession().Delete((aDes.Length() == 0) ? KMsvDefaultIndexFile : aDes);
			CPermanentFileStore* indexStore = CPermanentFileStore::CreateL(FileSession(), (aDes.Length() == 0) ? KMsvDefaultIndexFile : aDes, EFileShareExclusive|EFileWrite);
#endif
			indexStore->SetTypeL(TUidType(KPermanentFileStoreLayoutUid, KUidMsvIndexFile)); // 2nd uid should be KUidMsvIndexFile
			//
			RStoreWriteStream writeStream;
			// store the blank stream index in file
			TStreamId indexStreamId = writeStream.CreateLC(*indexStore);
			writeStream.WriteInt32L(KMsvIndexStoreVersionNumber);
			writeStream.WriteInt32L(1000000);
			writeStream.WriteInt32L(1);
			writeStream.WriteInt32L(122334);
			writeStream.CommitL();
			CleanupStack::PopAndDestroy(); // writeStream
			// store the commit stream value
			TStreamId commitStreamId = writeStream.CreateLC(*indexStore);
			writeStream.WriteInt32L(12243);
			writeStream.CommitL();
			CleanupStack::PopAndDestroy(); // writeStream
			//
			TStreamId rootIndexId = writeStream.CreateLC(*indexStore);
			writeStream << indexStreamId;
			writeStream << commitStreamId;
			writeStream.CommitL();
			CleanupStack::PopAndDestroy(); // writeStream
			indexStore->SetRootL(rootIndexId);
			//	
			indexStore->CommitL();
			delete indexStore;
			break;
			}
		default:
			iRTest(EFalse);
		}
	}

EXPORT_C TInt CMsgServerTestUtils::IndexSizeL(TInt aDrive/* = EDriveC*/, const TDesC& aIndexFile/* = KMsvDefaultIndexFile*/)
	{
	TDriveUnit driveUnit(aDrive);
	TDriveName name = driveUnit.Name();

	TParse parse;
	User::LeaveIfError(parse.Set(aIndexFile, &name, NULL));

	TEntry file;
	User::LeaveIfError(FileSession().Entry(parse.FullName(), file));
	return file.iSize;
	}


EXPORT_C void CMsgServerTestUtils::Start(const TDesC& aDes)
	{
	iRTest.Start(aDes);
	TestStart(++iTestNum, aDes);
	}

EXPORT_C void CMsgServerTestUtils::Next(const TDesC& aDes)
	{
	iRTest.Next(aDes);
	TestStart(++iTestNum, aDes);
	}

EXPORT_C void CMsgServerTestUtils::Complete()
	{
	TestFinish(iTestNum, KErrNone);
	}

EXPORT_C void CMsgServerTestUtils::End()
	{
	iRTest.End();
	}


EXPORT_C void CMsgServerTestUtils::ChooseDriveL(TDriveUnit &aResult)
	{
	CConsoleBase &cons=*iRTest.Console();
	CArrayFixFlat<TDriveUnit> *drives=new (ELeave) CArrayFixFlat<TDriveUnit>(5);
	CleanupStack::PushL(drives);
	TDriveUnit drive;
	for (drive = EDriveC; drive < EDriveY; drive = TInt(drive) + 1)
		{
		TVolumeInfo info;
		if (FileSession().Volume(info, TInt(drive)) == KErrNone)
			{
			drives->AppendL(drive);
			cons.Printf(_L("%d. "),drives->Count());
			cons.Printf(drive.Name());
			cons.Printf(_L("\n"));
			}
		}
	if(drives->Count()==0) User::Leave(KErrNotFound);
	cons.Printf(_L("\nPlease choose a drive\n"));
	TKeyCode ch=cons.Getch();
	while(ch<'1' && ch > ('1'+drives->Count())) ch=cons.Getch();
	aResult=drives->At(ch-'1');
	CleanupStack::PopAndDestroy(drives);
	}

EXPORT_C TInt CMsgServerTestUtils::ReadInteger(const TDesC& aDes,TInt &aResult)
	{
	CConsoleBase &cons=*iRTest.Console();
	_LIT(KPrompt,"%S > ");
	cons.Printf(KPrompt,&aDes);
	TBuf<80> number;
	TKeyCode ch;
	while((ch=cons.Getch())!=EKeyEnter)
		{
		cons.Printf(_L("%c"),ch);
		number.Append(ch);
		}
	cons.Printf(_L("\n"),ch);
	TLex lex(number);
	return(lex.Val(aResult));
	}


EXPORT_C CTestScheduler::CTestScheduler() : iSchedulerActive(EFalse), iPanicSchedulerOnError(ETrue)
	{

	}


EXPORT_C void CTestScheduler::Error(TInt aError) const
	{
	CTestScheduler *nonConst=CONST_CAST(CTestScheduler *,this);
	nonConst->iSchedulerError = aError;
	if (iSchedulerActive)
		{
		CActiveScheduler::Halt(KErrNone);
		nonConst->iSchedulerActive = EFalse;
		}
	else
		{
		if (iPanicSchedulerOnError)
			CActiveScheduler::Error(aError);
		}
	}



