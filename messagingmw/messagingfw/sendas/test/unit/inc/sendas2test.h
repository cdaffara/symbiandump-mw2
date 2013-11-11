// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// sendas2test.cpp
//
#ifndef __SENDAS2TEST_H__
#define __SENDAS2TEST_H__

#include <sendas2.h>
#include <csendasaccounts.h>
#include <csendasmessagetypes.h>
#include <e32base.h>
#include "msvtestutilsbase.h"
#include <mtclreg.h>
#include <msvids.h>
#include <msvuids.h>
#include <msvapi.h>
#include <e32uid.h>
#include <msvruids.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

// to be defined by test harness
GLREF_D RTest test; 
GLREF_D void doTestsL();

// forward declarations
class CSendTestUtils;

// globals
TInt SchedulerError;
CSendTestUtils* theUtils = NULL;
CTrapCleanup* theCleanup = NULL;

const TInt KNumberOfTestMtms		=5; 
const TInt KNumberOfTestMtmsCanSend	=4;

_LIT(KMtmDataDir, "c:\\system\\mtm\\");
_LIT(KMtmDataFile1, "c:\\system\\mtm\\SENDAS2MTM1.dat");
_LIT(KMtmDataFile2, "c:\\system\\mtm\\SENDAS2MTM2.dat");
_LIT(KMtmDataFile3, "c:\\system\\mtm\\SENDAS2MTM3.dat");
_LIT(KMtmDataFile4, "c:\\system\\mtm\\SENDAS2MTM4.dat");
_LIT(KMtmDataFile5, "c:\\system\\mtm\\SENDAS2MTM5.dat");
_LIT(KMtmDataFile6, "c:\\system\\mtm\\SENDAS2MTM6.dat");

_LIT(KMtmName1, "test MTM 1");
_LIT(KMtmName2, "test MTM 2");
_LIT(KMtmName3, "test MTM 3 with longer name");
_LIT(KMtmName4, "test MTM 4");
_LIT(KMtmName5, "test MTM 5");
_LIT(KMtmName6, "SMS Test CLIENT");

const TUid KUidTestMtm1={0x100014A4};
const TUid KUidTestMtm2={0x100014A5};
const TUid KUidTestMtm3={0x100014A6};
const TUid KUidTestMtm4={0x100014A7};
const TUid KUidTestMtm5={0x100014A8};
const TUid KUidTestMtm6={0x1000102C};

_LIT(KTestMtmFileName1, "sendas2mtm1.dll");
_LIT(KTestMtmFileName2, "sendas2mtm2.dll");
_LIT(KTestMtmFileName3, "sendas2mtm3.dll");
_LIT(KTestMtmFileName4, "sendas2mtm4.dll");
_LIT(KTestMtmFileName5, "sendas2mtm5.dll");
_LIT(KTestMtmFileName6, "smcm.dll");

_LIT(KMtmRegistryFile, "c:\\private\\1000484b\\MTM Registry v2");

const TUid KUidRenderingUid={0x10001000};

TBuf<32> KMtm1Service1  = _L("a new service");
TBuf<64> KMtm1Service2  = _L("the last service (alphabetically)");
TBuf<32> KMtm1Service3  = _L("last created, but in the middle");
TInt theMtm1Service1Id,theMtm1Service2Id,theMtm1Service3Id;

TBuf<32> KMtm3Service1  = _L("zzzzzzzz");
TBuf<32> KMtm3Service2  = _L("aaaaaaaaahhhhhhhhh");
TInt theMtm3Service1Id,theMtm3Service2Id;

TBuf<32> KMtm4Service1  = _L("lalala");
TBuf<32> KMtm6Service1  = _L("SMS Client");
TInt theMtm4Service1Id;

// Attachment names
_LIT(KErrAttachmentName1, "attachment1.txt");
_LIT(KErrAttachmentName2, "attachment2.txt");
_LIT(KAttachmentName, "Attach.txt");


TInt theErr;
#define REPORT_IF_ERROR(a) {theErr=a; if (theErr) test(0);}

//**********************************
// CTestScheduler
//**********************************

class CTestScheduler : public CActiveScheduler
	{
public:
	void Error (TInt aError) const;
	};

void CTestScheduler::Error( TInt anError ) const
	{
	SchedulerError = anError;
	}

//**********************************
// CDummyObserver
//**********************************

class CDummyObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};
	};

//**********************************
// CSendTestUtils
//**********************************

class CSendTestUtils : public CTestUtils
	{
public:
	static CSendTestUtils* NewL(RTest& aTest);
protected:
	CSendTestUtils(RTest& aTest);
public:
	void Panic(TInt aPanic);
	void Start(const TDesC& aDes);
	void Complete();
	TUid FindMessageTypeL(RSendAs& aSendAs, const TDesC& aName);
	void SetupMtmsAndServicesL();
	void CreateDataFilesL(const TDesC& aFilename, const TDesC& aMtmName, TUid aUid, const TDesC& aServerMtmFileName, const TDesC& aClientMtmFileName);
	void DeleteServicesL(CMsvEntry& aEntry, TUid aUid);
	TMsvId CreateServiceL(CMsvEntry& aEntry, const TMsvEntry& aService);
	void RemoveMailFolderL(TInt aDrive);
private:
	TInt iTestNum;
	};

CSendTestUtils* CSendTestUtils::NewL(RTest& aTest)
	{
	CSendTestUtils* self = new(ELeave)CSendTestUtils(aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CSendTestUtils::CSendTestUtils(RTest& aTest) :
	CTestUtils(aTest)
	{
	}

void CSendTestUtils::Panic(TInt)
	{
	test(EFalse);
	}

void CSendTestUtils::Start(const TDesC& aDes)
	{
	if (iTestNum == 0)
		{
		test.Start(aDes);
		}
	else
		{
		test.Next(aDes);
		}

	TestStart(++iTestNum, aDes);
	}

void CSendTestUtils::Complete()
	{
	TestFinish(iTestNum, KErrNone);
	}

TUid CSendTestUtils::FindMessageTypeL(RSendAs& aSendAs, const TDesC& aName)
	{
	// get list of available mtms
	CSendAsMessageTypes* messageTypes = CSendAsMessageTypes::NewL();
	CleanupStack::PushL(messageTypes);
	aSendAs.FilteredMessageTypesL(*messageTypes);

	// get count of mtms
	TUid mtmUid(KNullUid);
	TInt count = messageTypes->Count();
	while (count--)
		{
		if (messageTypes->AvailableMessageTypes().MdcaPoint(count) == aName)
			{
			mtmUid = messageTypes->MessageTypeUid(count);
			break;
			}
		}
	test(mtmUid != KNullUid);
	return mtmUid;
	}

void CSendTestUtils::SetupMtmsAndServicesL()
	{
	FileSession().Delete(KMtmDataFile1);
	FileSession().Delete(KMtmDataFile2);
	FileSession().Delete(KMtmDataFile3);
	FileSession().Delete(KMtmDataFile4);
	FileSession().Delete(KMtmDataFile5);
	FileSession().Delete(KMtmRegistryFile);
	FileSession().Delete(KMtmDataFile6);
	
	CDummyObserver* ob = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	// MTMs
	CClientMtmRegistry* reg = CClientMtmRegistry::NewL(*session);
	TInt initialMtmCount = reg->NumRegisteredMtmDlls();
	delete reg; 
	reg=NULL;

	// Create an mtm directory if not already exists.
	TBool isOpen(EFalse);
	if (FileSession().IsFileOpen(KMtmDataDir, isOpen) != KErrNone)
		{
		TInt err = FileSession().MkDirAll(KMtmDataDir);
		if (err != KErrAlreadyExists)
			{
			User::Leave(err);
			}
		}

	TInt installedTestMtms = 0;
	CreateDataFilesL(KMtmDataFile1, KMtmName1, KUidTestMtm1, KTestMtmFileName1, KTestMtmFileName1); 
	TInt err = session->InstallMtmGroup(KMtmDataFile1);
	if( err != KErrAlreadyExists )
		{
		++installedTestMtms;
		}
	CreateDataFilesL(KMtmDataFile2, KMtmName2, KUidTestMtm2, KTestMtmFileName2, KTestMtmFileName2); 
	err = session->InstallMtmGroup(KMtmDataFile2);
	if( err != KErrAlreadyExists )
		{
		++installedTestMtms;
		}
	CreateDataFilesL(KMtmDataFile3, KMtmName3, KUidTestMtm3, KTestMtmFileName3, KTestMtmFileName3); 
	err = session->InstallMtmGroup(KMtmDataFile3);
	if( err != KErrAlreadyExists )
		{
		++installedTestMtms;
		}
	CreateDataFilesL(KMtmDataFile4, KMtmName4, KUidTestMtm4, KTestMtmFileName4, KTestMtmFileName4); 
	err = session->InstallMtmGroup(KMtmDataFile4);
	if( err != KErrAlreadyExists )
		{
		++installedTestMtms;
		}
	CreateDataFilesL(KMtmDataFile5, KMtmName5, KUidTestMtm5, KTestMtmFileName5, KTestMtmFileName5); 
	err = session->InstallMtmGroup(KMtmDataFile5);
	if( err != KErrAlreadyExists )
		{
		++installedTestMtms;
		}
	CreateDataFilesL(KMtmDataFile6, KMtmName6, KUidTestMtm6, KTestMtmFileName6, KTestMtmFileName6); 
	err = session->InstallMtmGroup(KMtmDataFile6);
	if( err != KErrAlreadyExists )
		{
		++installedTestMtms;
		}

	//
	reg = CClientMtmRegistry::NewL(*session);
	test(reg->NumRegisteredMtmDlls()==initialMtmCount+installedTestMtms);
	delete reg; reg=NULL;
	test.Printf(TRefByValue<const TDesC>(_L("created %d test mtms\n")),installedTestMtms);
	test.Printf(TRefByValue<const TDesC>(_L("-> %d test mtms already installed\n")),KNumberOfTestMtms-installedTestMtms);
	
	//
	// services
	CMsvEntry* entry = session->GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(entry);
	DeleteServicesL(*entry, KUidTestMtm1);
	DeleteServicesL(*entry, KUidTestMtm3);
	DeleteServicesL(*entry, KUidTestMtm4);
	DeleteServicesL(*entry, KUidTestMtm5);
	DeleteServicesL(*entry, KUidTestMtm6);
	//
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KUidTestMtm1;
	service.iDetails.Set(KMtm1Service1);
	theMtm1Service1Id = CreateServiceL(*entry, service);
	//
	service.iDetails.Set(KMtm1Service2);
	theMtm1Service2Id = CreateServiceL(*entry, service);
	//
	service.iDetails.Set(KMtm1Service3);
	theMtm1Service3Id = CreateServiceL(*entry, service);
	//
	service.iMtm = KUidTestMtm3;
	service.iDetails.Set(KMtm3Service1);
	theMtm3Service1Id = CreateServiceL(*entry, service);
	//
	service.iDetails.Set(KMtm3Service2);
	theMtm3Service2Id = CreateServiceL(*entry, service);
	//
	service.iMtm = KUidTestMtm4;
	service.iDetails.Set(KMtm4Service1);
	theMtm4Service1Id = CreateServiceL(*entry, service);

	service.iMtm = KUidTestMtm6;
	service.iDetails.Set(KMtm6Service1);
	theMtm4Service1Id = CreateServiceL(*entry, service);

	CleanupStack::PopAndDestroy(3); // ob, session, entry
	}


void CSendTestUtils::CreateDataFilesL(const TDesC& aFilename, const TDesC& aMtmName, TUid aUid, const TDesC& aServerMtmFileName, const TDesC& aClientMtmFileName)
	{
	CMtmDllInfoArray* mtmdllinfoarray=new(ELeave) CMtmDllInfoArray();
	CleanupStack::PushL(mtmdllinfoarray);

	CMtmDllInfo* mtmdllinfo1=CMtmDllInfo::NewL(aMtmName, TUidType(KDynamicLibraryUid, KUidMtmServerComponent, TUid::Uid(KUidMtmDefaultSpecificVal)),aServerMtmFileName,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo1);

	CMtmDllInfo* mtmdllinfo2=CMtmDllInfo::NewL(aMtmName, TUidType(KDynamicLibraryUid, KUidMtmClientComponent, TUid::Uid(KUidMtmDefaultSpecificVal)),aClientMtmFileName,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo2);

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


void CSendTestUtils::DeleteServicesL(CMsvEntry& aEntry, TUid aUid)
	{
	CMsvEntrySelection* selection = aEntry.ChildrenWithMtmL(aUid);
	if (selection->Count())
		{
		CMsvOperationWait* wait = CMsvOperationWait::NewLC();
		wait->Start();
		CMsvOperation* op = aEntry.DeleteL(*selection, wait->iStatus);
		CActiveScheduler::Start();
		test(wait->iStatus.Int()==KErrNone||wait->iStatus.Int()==KErrNotFound);
		delete op;
		CleanupStack::PopAndDestroy(wait);
		}
	delete selection;
	}


TMsvId CSendTestUtils::CreateServiceL(CMsvEntry& aEntry, const TMsvEntry& aService)
	{
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->Start();
	CMsvOperation* op = aEntry.CreateL(aService, wait->iStatus);
	CActiveScheduler::Start();
	REPORT_IF_ERROR(wait->iStatus.Int());
	TMsvLocalOperationProgress localProg(McliUtils::GetLocalProgressL(*op));
	TMsvId id = localProg.iId;
	delete op;
	CleanupStack::PopAndDestroy(); // wait
	return id;
	}

void CSendTestUtils::RemoveMailFolderL(TInt aDrive)
	{
	CFileMan* fileMan = CFileMan::NewL(theUtils->FileSession());
	CleanupStack::PushL(fileMan);

	TDriveUnit drive(aDrive);
	TPtrC ptr(drive.Name());

	TParse parse;
	test(parse.Set(ptr, &KMsvDefaultFolder(), NULL) == KErrNone);

	TInt error = fileMan->RmDir(parse.DriveAndPath());
	test(error == KErrNone || error == KErrPathNotFound);

	CleanupStack::PopAndDestroy();
	}

//**********************************
// Global
//**********************************

void SetupSchedulerL()
	{
	CActiveScheduler::Install(new(ELeave)CTestScheduler);
	}

void CloseScheduler(TInt aRet)
	{
	if (theUtils)
		{
		if (!aRet)
			{
			theUtils->TestHarnessCompleted();
			}
		else
			{
			theUtils->TestHarnessFailed(aRet);
			}
		}
	test(aRet == KErrNone);

	delete CActiveScheduler::Current();
	}

void doMainL()
	{
	SetupSchedulerL();

	// sendas2 test utils
	theUtils = CSendTestUtils::NewL(test);

	// TODO: Use test server to delete message folder and remove CENREP entries
	//theUtils->RemoveMailFolderL(EDriveC);

	// setup mtms
  	theUtils->SetupMtmsAndServicesL();
  	
  	// do the tests
	doTestsL();
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	theCleanup=CTrapCleanup::New();
	TRAPD(ret,doMainL());		
	test(ret == KErrNone);
	CloseScheduler(ret);
	delete theUtils;
	delete theCleanup;	
	test.Console()->SetPos(0, 13);
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return(KErrNone);
	}

#endif
