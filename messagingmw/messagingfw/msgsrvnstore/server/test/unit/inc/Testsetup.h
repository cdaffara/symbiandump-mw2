// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifndef __TESTSETUP_H__
#define __TESTSETUP_H__

#include <e32base.h>
#include <s32file.h>
#include <f32file.h>
#include <e32test.h>
#include <e32uid.h>
#include "msvtestutilsbase.h"
#include <msvapi.h>
#include <msvstd.h>
#include "MSVSERV.H"
#include <msvruids.h>
#include "msvutils.h"
#include <sqldb.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

_LIT(KServerIndexResourceFileName, "MSGS.R*");
_LIT(KServerIndexResourceFilePath, "\\resource\\messaging\\");

#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
_LIT(KDbFileName, "\\messaging.db");
#else
_LIT(KDbFileName, "[1000484B]messaging.db");
#endif

_LIT(KMsvServerPattern, "!MsvServer*");

// The following must be defined by the test exe
GLREF_D RTest test;
LOCAL_C void doTestsL();

// Globals
CTrapCleanup* theCleanup = NULL;
class CTestScheduler;
CTestScheduler *theScheduler = NULL;
TInt SchedulerError;
TBool SchedulerActive = EFalse;
TBool PanicSchedulerOnError = ETrue;
TDriveUnit RemovableDrive;

TInt TestNum = 0;
class CMsgsTestUtils;
CMsgsTestUtils* theUtils = NULL;

// Constants
const TUid KUidMsvIndexFile = {0x10003C6B};
const TInt KMsvIndexStoreVersionNumber = 1;

// Useful macros
TInt theError;
#define REPORT_IF_ERROR(a) { theError = a; test(theError == KErrNone); }
#define __FILE_FAILNEXT(X) theUtils->FileSession().SetErrorCondition(KErrGeneral, X)
#define __FILE_RESET theUtils->FileSession().SetErrorCondition(KErrNone, 0)

//**********************************
// CDummyObserver
//**********************************

class CDummyObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};
	};

//**********************************
// CEntryObserver
//**********************************

class CEntryObserver : public CBase, public MMsvEntryObserver
	{
public:
	CEntryObserver();
	~CEntryObserver();
	void HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	void Start();
protected:
	void DoHandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
public:
	TMsvEntryEvent iEvent;
	TInt iActive;
	CMsvEntrySelection iSelection;
	};




CEntryObserver::CEntryObserver() 
:iEvent(EMsvEntryChanged), iActive(0)
	{
	}

CEntryObserver::~CEntryObserver() 
	{
	}

void CEntryObserver::DoHandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	iEvent = aEvent;
	iSelection.Reset();
	if (iEvent==EMsvNewChildren || iEvent==EMsvDeletedChildren || iEvent==EMsvChildrenChanged)
		{
		CMsvEntrySelection* selection = (CMsvEntrySelection*)aArg1;
		for (TInt count=0; count<selection->Count(); count++)
			iSelection.AppendL(selection->At(count));
		}
	}

void CEntryObserver::HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
	{
	if (iActive)
		{
		CActiveScheduler *scheduler= CActiveScheduler::Current();
		scheduler->Halt(KErrNone);
		iActive--;
		}
	DoHandleEntryEventL(aEvent, aArg1, aArg2, aArg3);
	}
	
void CEntryObserver::Start()
	{
	iActive++;
	}

//**********************************
// CTestScheduler
//**********************************

class CTestScheduler : public CActiveScheduler
	{
private:
	void Error(TInt aError) const;
	};

void CTestScheduler::Error(TInt aError) const
	{
	SchedulerError = aError;
	if (SchedulerActive)
		{
		Halt(KErrNone);
		SchedulerActive = EFalse;
		}
	else
		{
		if (PanicSchedulerOnError)
			CActiveScheduler::Error(aError);
		}
	}

//**********************************
// CMsgsTestUtils
//**********************************

class CMsgsTestUtils : public CTestUtils
	{
public:
	static CMsgsTestUtils* NewL(RTest& aTest);
	CMsgsTestUtils(RTest& aTest);
	void Panic(TInt aPanic);
	//
public:
	static CMsvServer* CreateServerL();
	static void CleanMessageFolderL(const TDesC& aDes = KNullDesC);
	static void CreateDataFilesL(const TDesC& aFilename, const TDesC& aMtmName, TUid aUid, const TDesC& aMtmFilename);
	static void InstallMtmL(const TDesC& aDataFile, const TDesC& aMtmName, TUid aUid, const TDesC& aMtmFilename);
	static TBool WaitForKeyL(TTimeIntervalMicroSeconds32 aDelay, TKeyCode* aKeyCode = NULL);
	static void WaitForServerClose();
	static TBool IsServerRunning();
	static void SetDriveL(TInt aDrive, TBool aStoreId = EFalse);
	static void RemoveMailFolderL(TInt aDrive);
	static void CorruptIndexFileL(TInt aMode, const TDesC& aDes = KNullDesC);
	static void RemoveIndexFile(TInt aDrive);
	static void SetDebugL(TBool aDebug);
	static void GetConfigL(TMsvConfig& aConfig);
	static void SetConfigL(TMsvConfig& aConfig);
	static void FileExists(const TDesC& aDes);
	static void FileMissing(const TDesC& aDes);
	static TInt IndexSizeL(TInt aDrive = EDriveC, const TDesC& aIndexFile = KMsvDefaultIndexFile2);
	//
	static void Start(const TDesC& aDes);
	static void Next(const TDesC& aDes);
	static void Complete();
	static void End();
	};

CMsgsTestUtils* CMsgsTestUtils::NewL(RTest& aTest)
	{
	CMsgsTestUtils* self = new(ELeave)CMsgsTestUtils(aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

CMsgsTestUtils::CMsgsTestUtils(RTest& aTest)
: CTestUtils(aTest)
	{
	}

void CMsgsTestUtils::Panic(TInt)
	{
	test(EFalse);
	}

void CMsgsTestUtils::Start(const TDesC& aDes)
	{
	test.Start(aDes);
	theUtils->TestStart(++TestNum, aDes);
	}

void CMsgsTestUtils::Next(const TDesC& aDes)
	{
	test.Next(aDes);
	theUtils->TestStart(++TestNum, aDes);
	}

void CMsgsTestUtils::Complete()
	{
	theUtils->TestFinish(TestNum, KErrNone);
	}

void CMsgsTestUtils::End()
	{
	test.End();
	}

TInt CMsgsTestUtils::IndexSizeL(TInt aDrive/* = EDriveC*/, const TDesC& aIndexFile/* = KMsvDefaultIndexFile*/)
	{
	TDriveUnit driveUnit(aDrive);
	TDriveName name = driveUnit.Name();

	TParse parse;
	User::LeaveIfError(parse.Set(aIndexFile, &name, NULL));

	TEntry file;
	User::LeaveIfError(theUtils->FileSession().Entry(parse.FullName(), file));
	return file.iSize;
	}

CMsvServer* CMsgsTestUtils::CreateServerL()
	{	
	CMsvServer* server = CMsvServer::NewL();
	CleanupStack::PushL(server);

	CTestTimer* timer = CTestTimer::NewL();
	timer->After(100000);
	CActiveScheduler::Start();
	delete timer;

	CleanupStack::Pop(); // server
	return server;
	}

void CMsgsTestUtils::FileExists(const TDesC& aDes)
	{
	TUint at;
	test(theUtils->FileSession().Att(aDes, at)==KErrNone);
	}

void CMsgsTestUtils::FileMissing(const TDesC& aDes)
	{
	TUint at;
	TInt error = theUtils->FileSession().Att(aDes, at);
	test(error==KErrNotFound || error==KErrPathNotFound);
	}

void CMsgsTestUtils::CleanMessageFolderL(const TDesC& aDes)
	{
	CFileMan* fileMan = CFileMan::NewL(theUtils->FileSession());
	CleanupStack::PushL(fileMan);

	//Kill the message server if its running
	_LIT(KMsvServerPattern, "!MsvServer*");
	TFindProcess findprocess(KMsvServerPattern);
	TFullName name;
	if(findprocess.Next(name)==KErrNone)
		{
		RProcess process;
    	User::LeaveIfError(process.Open(findprocess));
     	process.Kill(KErrCancel) ;
		process.Close() ;
		}
	
	// Wait for the server to close before trying to remove
	// the message folder
		
	TInt loopCount = 0;
	
	FOREVER
		{
		TFindServer find(KMsvServerPattern);
		if (find.Next(name) != KErrNone)
			break;
		User::After(100000);
		++loopCount;
		if(loopCount > 5)
			{
			User::Invariant();
			}
		}	
	TParse parse;
	TFileName fileName((aDes.Length() == 0) ? KMsvDefaultFolder2 : aDes);
	TBuf<4> drive;
	TChar driveChar=RFs::GetSystemDriveChar();
	drive.Append(driveChar);
	drive.Append(KDriveDelimiter);
	parse.Set(fileName, &drive, NULL);

	TInt error = fileMan->RmDir(parse.DriveAndPath());
	CleanupStack::PopAndDestroy(); // fileMan

	error = theUtils->FileSession().RmDir(parse.DriveAndPath());
	if (!(error == KErrNotFound || error == KErrNone || error == KErrPathNotFound))
		{
		TPtrC dir(parse.DriveAndPath());
		test.Printf(TRefByValue<const TDesC>(_L("Directory %S cannot be removed\n Exiting - press any key...\n")), &dir);
		User::Leave(KErrNotReady);
		}
	
	TPtrC drivePtr(NULL, 0);
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TDriveList drives;
	User::LeaveIfError(theUtils->FileSession().DriveList(drives));
	for(TInt index = EDriveA; index <= EDriveZ; ++index)
		{
		if(drives[index]) //if drive exists on the system
			{
			drivePtr.Set(TDriveUnit(index).Name());
			parse.Set(KDbFileName, &drivePtr, NULL);
			RSqlDatabase::Delete(parse.FullName());
			}
		}
#else	
	//delete DB
	TDriveUnit driveUnit(drive);
	TPath pathName(driveUnit.Name());
	pathName.Append(KDbFileName);
	RSqlDatabase::Delete(pathName);
#endif
	}

void CMsgsTestUtils::CreateDataFilesL(const TDesC& aFilename, const TDesC& aMtmName, TUid aUid, const TDesC& aMtmFilename)
	{
	CMtmDllInfoArray* mtmdllinfoarray=new(ELeave) CMtmDllInfoArray();
	CleanupStack::PushL(mtmdllinfoarray);

	CMtmDllInfo* mtmdllinfo1=CMtmDllInfo::NewL(aMtmName, TUidType(KDynamicLibraryUid, KUidMtmServerComponent, TUid::Uid(KUidMtmDefaultSpecificVal)),aMtmFilename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo1);

	CMtmDllInfo* mtmdllinfo2=CMtmDllInfo::NewL(aMtmName, TUidType(KDynamicLibraryUid, KUidMtmClientComponent, TUid::Uid(KUidMtmDefaultSpecificVal)),aMtmFilename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo2);

	// Note aUID is used here as the technology type UID, the MTM type UID,
	// the MTM dll 3rd UID *and* the data file 3rd UID
	TCapabilitySet capSet;
	capSet.SetEmpty();
	CleanupStack::Pop(mtmdllinfoarray); // next line takes ownership
	CMtmGroupData* mtmgroupdata=CMtmGroupData::NewL(aUid,aUid, mtmdllinfoarray, capSet);
	CleanupStack::PushL(mtmgroupdata);

	CFileStore* filestore = CPermanentFileStore::ReplaceLC(theUtils->FileSession(), aFilename, EFileShareExclusive|EFileStream|EFileWrite);
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

void CMsgsTestUtils::InstallMtmL(const TDesC& aDataFile, const TDesC& aMtmName, TUid aId, const TDesC& aMtmFilename)
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	CreateDataFilesL(aDataFile, aMtmName, aId, aMtmFilename); 
	session->InstallMtmGroup(aDataFile);

	CleanupStack::PopAndDestroy(2); // session, ob
	}

TBool CMsgsTestUtils::WaitForKeyL(TTimeIntervalMicroSeconds32 aDelay, TKeyCode* aKeyCode)
	{
	test(test.Console() != NULL);

	// Create timer
	CTestTimer* timer = CTestTimer::NewL();
	CleanupStack::PushL(timer);
	timer->After(aDelay);

	CTestActive* wait = new(ELeave)CTestActive;
	CleanupStack::PushL(wait);
	wait->StartL();

	// Wait for key press
	test.Console()->Read(wait->iStatus);
	CActiveScheduler::Start();

	// If timer still active a key was pressed
	TBool keyPressed = timer->IsActive();

	if (keyPressed)
		{
		// Get the key pressed
		if (aKeyCode)
			*aKeyCode = test.Console()->KeyCode();

		// Cancel timer
		timer->Cancel();
		}
	else
		{
		// Cancel wait for character
		test.Console()->ReadCancel();
		User::WaitForRequest(wait->iStatus);
		}

	CleanupStack::PopAndDestroy(2); // wait, timer
	return keyPressed;
	}

void CMsgsTestUtils::WaitForServerClose()
	{
	
	TFindProcess find(KMsvServerPattern);
	TFullName name;
	
	if (find.Next(name) == KErrNone)
		{
		TInt err;
		CDummyObserver* dummyObserver=NULL;
		TRAP(err,dummyObserver = new(ELeave)CDummyObserver);
		CleanupStack::PushL(dummyObserver);
		CMsvSession* session=NULL;
		TRAP(err,session = CMsvSession::OpenAsyncL(*dummyObserver));
		CleanupStack::PushL(session);
		session->CloseMessageServer();	
		CleanupStack::PopAndDestroy(2,dummyObserver); 
		
		TRequestStatus status;
		RProcess process;
    	User::LeaveIfError(process.Open(find));
     	process.Logon(status);
     	User::WaitForRequest(status);
     	User::After(200000);
		}
	}

// server is a thread on wins and a process on a real device
TBool CMsgsTestUtils::IsServerRunning()
	{
	TFullName name;
	TFindProcess find(KMsvServerPattern);
	if (find.Next(name) != KErrNone) return EFalse;
	else return ETrue;
	}

void CMsgsTestUtils::GetConfigL(TMsvConfig& aConfig)
	{
	// try to open system ini
	TUint unused;
	TChar driveChar= theUtils->FileSession().GetSystemDriveChar();
	TBuf<2> systemDrive;
	systemDrive.Append(driveChar);
	systemDrive.Append(KDriveDelimiter);
    TPath pathName(systemDrive);
	pathName.Append(KServerINIFile);
	
	if(theUtils->FileSession().Att(pathName,unused)==KErrNone)
		{
		CDictionaryFileStore *store = CDictionaryFileStore::OpenLC(theUtils->FileSession(),pathName,KNullUid);

		// get configuration from system ini
		if (store->IsPresentL(KUidMsvMessageDriveStream))
			{
			RDictionaryReadStream readStream;
			readStream.OpenLC(*store, KUidMsvMessageDriveStream);
			TInt version = readStream.ReadInt8L();
		
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

void CMsgsTestUtils::SetConfigL(TMsvConfig& aConfig)
	{
	// ignore the error code as next line will fail anyway unless it is KErrAlreadyExists or KErrNone
	TChar driveChar=theUtils->FileSession().GetSystemDriveChar();
	TBuf<2> systemDrive;
	systemDrive.Append(driveChar);
	systemDrive.Append(KDriveDelimiter);
    TPath pathName(systemDrive);
	pathName.Append(KServerINIFile);
	theUtils->FileSession().MkDirAll(pathName);
	CDictionaryFileStore *store = CDictionaryFileStore::OpenLC(theUtils->FileSession(),pathName,KNullUid);
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

void CMsgsTestUtils::SetDriveL(TInt aDrive, TBool aStoreId)
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

void CMsgsTestUtils::SetDebugL(TBool aDebug)
	{
	TMsvConfig config;
	GetConfigL(config);
	
	config.iDebug = aDebug;
	SetConfigL(config);
	}

void CMsgsTestUtils::RemoveMailFolderL(TInt aDrive)
	{
	CFileMan* fileMan = CFileMan::NewL(theUtils->FileSession());
	CleanupStack::PushL(fileMan);

	TDriveUnit drive(aDrive);
	TPtrC ptr(drive.Name());

	TParse parse;
	test(parse.Set(ptr, &KMsvDefaultIndexFile2(), NULL) == KErrNone);

	TInt error = fileMan->RmDir(parse.DriveAndPath());
	test(error == KErrNone || error == KErrPathNotFound);

	//delete DB
	TPath pathName(drive.Name());
	pathName.Append(KDbFileName);
	RSqlDatabase::Delete(pathName);

	CleanupStack::PopAndDestroy();
	}

void CMsgsTestUtils::RemoveIndexFile(TInt aDrive)
	{
	TDriveUnit drive(aDrive);
	TPtrC ptr(drive.Name());

	TParse parse;
	test(parse.Set(ptr, &KMsvDefaultIndexFile2(), NULL) == KErrNone);

	test(theUtils->FileSession().Delete(parse.FullName()) == KErrNone);
	//delete DB
	TPath pathName(drive.Name());
	pathName.Append(KDbFileName);
	RSqlDatabase::Delete(pathName);

	}

void CMsgsTestUtils::CorruptIndexFileL(TInt aMode, const TDesC& aDes)
	{
	switch (aMode)
		{
		case 0: // uicode/narrow conflict
			{
			theUtils->FileSession().Delete((aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes);
			CPermanentFileStore* indexStore = CPermanentFileStore::CreateL(theUtils->FileSession(), (aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes, EFileShareExclusive|EFileWrite);
			indexStore->SetTypeL(TUidType(KPermanentFileStoreLayoutUid, KNullUid)); // 2nd uid should be KUidMsvIndexFile
			indexStore->CommitL();
			delete indexStore;
			break;
			}
		case 1: // empty index file
			{
			theUtils->FileSession().Delete((aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes);
			CPermanentFileStore* indexStore = CPermanentFileStore::CreateL(theUtils->FileSession(), (aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes, EFileShareExclusive|EFileWrite);
			indexStore->SetTypeL(TUidType(KPermanentFileStoreLayoutUid, KUidMsvIndexFile)); // 2nd uid should be KUidMsvIndexFile
			indexStore->CommitL();
			delete indexStore;
			break;
			}
		case 2: // missing index stream
			{
			theUtils->FileSession().Delete((aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes);
			CPermanentFileStore* indexStore = CPermanentFileStore::CreateL(theUtils->FileSession(), (aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes, EFileShareExclusive|EFileWrite);
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
			theUtils->FileSession().Delete((aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes);
			CPermanentFileStore* indexStore = CPermanentFileStore::CreateL(theUtils->FileSession(), (aDes.Length() == 0) ? KMsvDefaultIndexFile2 : aDes, EFileShareExclusive|EFileWrite);
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
			test(EFalse);
		}
	}


void TestNumberOfFilesInService(TMsvId aService,TInt aNumberReq)
	{
	CDir *dir=NULL;
	TFileName filename=_L("C:");
	filename.Append(KMsvDefaultFolder2);
	MsvUtils::ConstructEntryName(aService, aService, filename, MsvUtils::EPath);
	REPORT_IF_ERROR(theUtils->FileSession().GetDir(filename, KEntryAttMaskSupported, ESortNone, dir));
	TInt counter=dir->Count();
	while(counter--)
		{
		CDir* subdir;
		TFileName subfilename(filename);
		subfilename.Append((*dir)[counter].iName);
		_LIT(KDirSep,"\\");
		subfilename.Append(KDirSep);
		REPORT_IF_ERROR(theUtils->FileSession().GetDir(subfilename, KEntryAttMaskSupported, ESortNone, subdir));
		aNumberReq -= subdir->Count();
		delete subdir;
		}
	test(aNumberReq==0);
	delete dir;
	}

TInt RunWhilePending(const TRequestStatus &aStatus)
	{
	TInt err=KErrNone;
	while(aStatus==KRequestPending)
		{
		CActiveScheduler::Current()->RunIfReady(err,0);
		CActiveScheduler::Current()->WaitForAnyRequest();
		}
	return err;
	}



//**********************************
// TTestMsvEntry
//**********************************

class TTestMsvEntry : public TMsvEntry
	{
public:
	void SetOwner(TBool aFlag) {TMsvEntry::SetOwner(aFlag);};
	void SetId(TMsvId aId) {TMsvEntry::SetId(aId);};
	void SetParent(TMsvId aId) {TMsvEntry::SetParent(aId);};
	void SetDeleted(TBool aFlag) {TMsvEntry::SetDeleted(aFlag);};
	TInt PcSyncCount() {return TMsvEntry::PcSyncCount();};
	TInt Reserved() {TInt* ptr = (TInt*)&iServiceId; return *--ptr;};
	void SetReserved(TInt aValue) {TInt* ptr = (TInt*)&iServiceId; *--ptr=aValue;};
	void SetData(TInt aData) {*(((TInt*)this)+2) = aData;};
	};

//**********************************
// Global
//**********************************

LOCAL_C void SetupL()
	{
	CActiveScheduler::Install(theScheduler = new(ELeave)CTestScheduler);

	// Create test class
	theUtils = CMsgsTestUtils::NewL(test);

	test.Printf(_L("Cleaning Message Folder\n"));
	CMsgsTestUtils::CleanMessageFolderL();
	CMsgsTestUtils::SetDriveL(EDriveC);
	CMsgsTestUtils::SetDebugL(ETrue);

	// Try and find the removable drive
	for (RemovableDrive = EDriveY; RemovableDrive > EDriveD; RemovableDrive = TInt(RemovableDrive) - 1)
		{
		TVolumeInfo info;
		if (theUtils->FileSession().Volume(info, TInt(RemovableDrive)) == KErrNone)
			break;
		}
	}

LOCAL_C void Close(TInt aRet)
	{
	if (theUtils)
		{
		if (!aRet)
			theUtils->TestHarnessCompleted();
		else
			theUtils->TestHarnessFailed(aRet);
		}
	test(aRet == KErrNone);
	delete theUtils;

	delete CActiveScheduler::Current();
	theScheduler = NULL;
	}

LOCAL_C void doMainL()
	{
	SetupL();		
	doTestsL();
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	theCleanup=CTrapCleanup::New();
	TRAPD(ret,doMainL());		
	Close(ret);
	delete theCleanup;	
	test.Console()->SetPos(0, 13);
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return(KErrNone);
	}

#endif //__TESTSETUP_H__
