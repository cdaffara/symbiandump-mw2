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
// BioTest.h
//

#ifndef __BIOTEST_H__
#define __BIOTEST_H__

#include <e32test.h>
#include "msvtestutilsbase.h"
#include "BIODB.H"
#include <bautils.h>
#include <bifchangeobserver.h>

// Constants
_LIT(KBifFolderV2, "\\resource\\messaging\\Bif\\");     


// The following must be defined by the test exe
GLREF_D RTest test;
LOCAL_C void doTestsL();

// Globals
TInt TestNum = 0;
class CBioTestUtils;
CBioTestUtils* theUtils = NULL;

//**********************************
// CBioTestUtils
//**********************************

class CBioTestUtils : public CTestUtils
	{
public:
	static CBioTestUtils* NewL(RTest& aTest);
	CBioTestUtils(RTest& aTest);
	void Panic(TInt aPanic);
	//
public:
	void Start(const TDesC& aDes);
	void Complete();
	void End(TInt aRet);
	//
	void ClearBifFoldersL();
	void ClearBifFolderL(TInt aDrive);
	TBool BifInstalledL(CBIODatabase& aDatabase, TUid aId);
	TBool FindFileL(TParse& aParse, const TDesC& aDir, const TDesC& aName);
	//
private:
	void ConstructL();
	TBool DoFindFileL(TParse& aParse, TInt aDrive, const TDesC& aDir, const TDesC& aName);
	//
private:
	TInt iTestNum;
	};


CBioTestUtils* CBioTestUtils::NewL(RTest& aTest)
	{
	CBioTestUtils* self = new(ELeave)CBioTestUtils(aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

CBioTestUtils::CBioTestUtils(RTest& aTest)
: CTestUtils(aTest)
	{
	}

void CBioTestUtils::ConstructL()
	{
	CTestUtils::ConstructL();

	// Is there a D-drive?
	TVolumeInfo info;
	Test()(FileSession().Volume(info, EDriveD) == KErrNone);
	}

void CBioTestUtils::Panic(TInt)
	{
	Test()(EFalse);
	}

void CBioTestUtils::Start(const TDesC& aDes)
	{
	if (iTestNum == 0)
		Test().Start(aDes);
	else
		Test().Next(aDes);

	TestStart(++iTestNum, aDes);
	}

void CBioTestUtils::Complete()
	{
	TestFinish(iTestNum, KErrNone);
	}

void CBioTestUtils::End(TInt aRet)
	{
	if (!aRet)
		TestHarnessCompleted();
	else
		TestHarnessFailed(aRet);
	
	Test()(aRet == KErrNone);

	if (iTestNum > 0)
		Test().End();

	Test().Close();
	}

void CBioTestUtils::ClearBifFoldersL()
	{
	ClearBifFolderL(EDriveC);
	ClearBifFolderL(EDriveD);
	}

void CBioTestUtils::ClearBifFolderL(TInt aDrive)
	{
	CFileMan* fileMan = CFileMan::NewL(FileSession());
	CleanupStack::PushL(fileMan);

	// Get drive name
	TDriveUnit unit(aDrive);
	TPtrC drive(unit.Name());

	// Generate path
	TParse parse;
	TInt error;

	User::LeaveIfError(parse.Set(KBifFolderV2(), &drive, NULL));

	// Delete the folder
	error = fileMan->RmDir(parse.DriveAndPath());
	if (error != KErrNotFound && error != KErrPathNotFound)
		User::LeaveIfError(error);

	CleanupStack::PopAndDestroy(); // fileMan
	}

TBool CBioTestUtils::BifInstalledL(CBIODatabase& aDatabase, TUid aId)
	{
	TInt index;
	TRAPD(error, aDatabase.GetBioIndexWithMsgIDL(aId, index));
	return !error;
	}

TBool CBioTestUtils::FindFileL(TParse& aParse, const TDesC& aDir, const TDesC& aName)
	{
	// Check rom
	if (DoFindFileL(aParse, EDriveZ, aDir, aName))
		return ETrue;

	// Check alternate drive
	if (DoFindFileL(aParse, EDriveD, aDir, aName))
		return ETrue;

	// Check internal disk
	if (DoFindFileL(aParse, EDriveC, aDir, aName))
		return ETrue;

	// File not found
	return EFalse;
	}

TBool CBioTestUtils::DoFindFileL(TParse& aParse, TInt aDrive, const TDesC& aDir, const TDesC& aName)
	{
	// Generate drive name
	TDriveUnit unit(aDrive);
	TPtrC drive(unit.Name());

	// Generate path and see if file exists
	User::LeaveIfError(aParse.Set(drive, &aDir, &aName));
	return BaflUtils::FileExists(FileSession(), aParse.FullName());
	}

//**********************************
// Global
//**********************************

LOCAL_C void SetupL()
	{
	CActiveScheduler::Install(new(ELeave)CActiveScheduler);

	// Create test class
	theUtils = CBioTestUtils::NewL(test);
	}

LOCAL_C void Close(TInt aRet)
	{
	if (theUtils)
		{
		theUtils->End(aRet);
		delete theUtils;
		}

	delete CActiveScheduler::Current();
	}

LOCAL_C void doMainL()
	{
	SetupL();	
	doTestsL();
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	__ASSERT_ALWAYS(cleanup, User::Invariant());
	TRAPD(ret,doMainL());		
	Close(ret);
	delete cleanup;	
	__UHEAP_MARKEND;
	return KErrNone;
	}

#endif
