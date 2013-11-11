// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TEST_H__
#define __TEST_H__

#include <e32base.h>
#include "msvtestutilsbase.h"

// To be defined by test harness
GLREF_D RTest test; 
void doTestsL();

// Globals
TInt SchedulerError;
class CSendTestUtils;
CSendTestUtils* theUtils = NULL;
CTrapCleanup* theCleanup = NULL;

const TInt KNumberOfTestMtms=5; 
const TInt KNumberOfTestMtmsCanSend=4;

#define MTM_DATAFILE_1 _L("c:\\system\\mtm\\SENDMTM1.dat")
#define MTM_DATAFILE_2 _L("c:\\system\\mtm\\SENDMTM2.dat")
#define MTM_DATAFILE_3 _L("c:\\system\\mtm\\SENDMTM3.dat")
#define MTM_DATAFILE_4 _L("c:\\system\\mtm\\SENDMTM4.dat")
#define MTM_DATAFILE_5 _L("c:\\system\\mtm\\SENDMTM5.dat")
#define MTM_NAME_1 _L("test MTM 1")
#define MTM_NAME_2 _L("test MTM 2")
#define MTM_NAME_3 _L("test MTM 3 with longer name")
#define MTM_NAME_4 _L("test MTM 4")
#define MTM_NAME_5 _L("test MTM 5")
const TUid KUidTestMtm1={0x100014A4};
const TUid KUidTestMtm2={0x100014A5};
const TUid KUidTestMtm3={0x100014A6};
const TUid KUidTestMtm4={0x100014A7};
const TUid KUidTestMtm5={0x100014A8};


const TUid KUidRenderingUid={0x10001000};

TBuf<32> KMtm1Service1  = _L("a new service");
TBuf<64> KMtm1Service2  = _L("the last service (alphabetically)");
TBuf<32> KMtm1Service3  = _L("last created, but in the middle");
TInt theMtm1Service1Id,theMtm1Service2Id,theMtm1Service3Id;

TBuf<32> KMtm3Service1  = _L("zzzzzzzz");
TBuf<32> KMtm3Service2  = _L("aaaaaaaaahhhhhhhhh");
TInt theMtm3Service1Id,theMtm3Service2Id;

TBuf<32> KMtm4Service1  = _L("lalala");
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
	CSendTestUtils(RTest& aTest);
	void Panic(TInt aPanic);
	//
public:
	void Start(const TDesC& aDes);
	void Complete();
	void DeleteServicesL(CMsvEntry& aEntry, TUid aUid);
	TMsvId CreateServiceL(CMsvEntry& aEntry, const TMsvEntry& aService);
	void RemoveMailFolderL(TInt aDrive);
	//
private:
	TInt iTestNum;
	};

CSendTestUtils* CSendTestUtils::NewL(RTest& aTest)
	{
	CSendTestUtils* self = new(ELeave)CSendTestUtils(aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

CSendTestUtils::CSendTestUtils(RTest& aTest)
: CTestUtils(aTest)
	{
	}

void CSendTestUtils::Panic(TInt)
	{
	test(EFalse);
	}

void CSendTestUtils::Start(const TDesC& aDes)
	{
	if (iTestNum == 0)
		test.Start(aDes);
	else
		test.Next(aDes);

	TestStart(++iTestNum, aDes);
	}

void CSendTestUtils::Complete()
	{
	TestFinish(iTestNum, KErrNone);
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
		CleanupStack::PopAndDestroy(); // wait
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
	test(parse.Set(ptr, &KMsvDefaultIndexFile(), NULL) == KErrNone);

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
	theUtils = CSendTestUtils::NewL(test);
	}

void CloseScheduler(TInt aRet)
	{
	if (theUtils)
		{
		if (!aRet)
			theUtils->TestHarnessCompleted();
		else
			theUtils->TestHarnessFailed(aRet);
		}
	test(aRet == KErrNone);

	delete CActiveScheduler::Current();
	delete theUtils;
	}

void doMainL()
	{
	SetupSchedulerL();
	doTestsL();
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	theCleanup=CTrapCleanup::New();
	TRAPD(ret,doMainL());		
	CloseScheduler(ret);
	delete theCleanup;	
	test.Console()->SetPos(0, 13);
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return(KErrNone);
	}

#endif
