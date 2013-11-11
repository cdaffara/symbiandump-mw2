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

#include "BioTest.h"

RTest test(_L("Bio Database Observer Tests"));

LOCAL_D void TestInstallL(TInt aDrive);

const TUid KTestBif = { 0x10009234 };

_LIT(KTestDir, "\\test\\bio\\bif\\");
_LIT(KBifDir, "\\resource\\messaging\\Bif\\");     
_LIT(KTestBif1Src, "testbif1.rsc");
_LIT(KTestMbm1Src, "testbif1.mbm");

_LIT(KTestBif2Src, "testbif2.rsc");
_LIT(KTestMbm2Src, "testbif2.mbm");

_LIT(KTestBifDst, "testbif.rsc");
_LIT(KTestMbmDst, "testbif.mbm");


class CBifObserver : public CBase , public MBifChangeObserver
	{
public:
	void HandleBifChangeL(TBifChangeEvent aEvent, TUid aBioUid);
	//
public:
	TBifChangeEvent iEvent;
	TUid iBioUid;
	TBool iFlag;
	};


void CBifObserver::HandleBifChangeL(TBifChangeEvent aEvent, TUid aBioUid)
	{
	iEvent = aEvent;
	iBioUid = aBioUid;
	iFlag = ETrue;

	CActiveScheduler::Stop();
	}


LOCAL_C void TestChangeL(TInt aDrive)
	{
	// Clear the BIF folders
	theUtils->ClearBifFoldersL();

	// Get the name of the drive
	TDriveUnit unit(aDrive);
	TPtrC drive(unit.Name());

	TParse src;
	TParse dst;

	// Copy the bif file
	theUtils->FindFileL(src, KTestDir, KTestBif1Src);
	User::LeaveIfError(dst.Set(drive, &KBifDir, &KTestBifDst));
	theUtils->FileSession().MkDirAll(dst.FullName());
	User::LeaveIfError(BaflUtils::CopyFile(theUtils->FileSession(), src.FullName(), dst.FullName()));

	// Copy the Mbm file
	theUtils->FindFileL(src, KTestDir, KTestMbm1Src);
	User::LeaveIfError(dst.Set(drive, &KBifDir, &KTestMbmDst));
	theUtils->FileSession().MkDirAll(dst.FullName());
	User::LeaveIfError(BaflUtils::CopyFile(theUtils->FileSession(), src.FullName(), dst.FullName()));

	// Create BIF database
	CBIODatabase* db = CBIODatabase::NewLC(theUtils->FileSession());

	// Create observer
	CBifObserver* obs = new(ELeave)CBifObserver;
	CleanupStack::PushL(obs);

	CBifChangeObserver* change = CBifChangeObserver::NewL(*obs, theUtils->FileSession());
	CleanupStack::PushL(change);
	change->Start();

	// Check the test BIO is installed
	test(theUtils->BifInstalledL(*db, KTestBif));
	test(!obs->iFlag);

	// Short delay to check that the observer isn't fired
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(10000000);
	CActiveScheduler::Start();
	delete timer;

	// Check the test BIO is still installed
	test(theUtils->BifInstalledL(*db, KTestBif));
	test(!obs->iFlag);

	// Copy the bif file
	theUtils->FindFileL(src, KTestDir, KTestBif2Src);
	User::LeaveIfError(dst.Set(drive, &KBifDir, &KTestBifDst));
	theUtils->FileSession().MkDirAll(dst.FullName());
	User::LeaveIfError(BaflUtils::CopyFile(theUtils->FileSession(), src.FullName(), dst.FullName()));

	// Copy the Mbm file
	theUtils->FindFileL(src, KTestDir, KTestMbm2Src);
	User::LeaveIfError(dst.Set(drive, &KBifDir, &KTestMbmDst));
	theUtils->FileSession().MkDirAll(dst.FullName());
	User::LeaveIfError(BaflUtils::CopyFile(theUtils->FileSession(), src.FullName(), dst.FullName()));

	// Wait for the notification to come through
	CActiveScheduler::Start();
	test(obs->iFlag);
	test(obs->iBioUid == KTestBif);
	test(obs->iEvent == MBifChangeObserver::EBifChanged);

	CleanupStack::PopAndDestroy(3); // change, obs, db
	}


LOCAL_C void TestImmediateCancelL()
	{
	CBifObserver* obs = new(ELeave)CBifObserver;
	CleanupStack::PushL(obs);
	
	CBifChangeObserver* iObserver=CBifChangeObserver::NewL(*obs, theUtils->FileSession());
	iObserver->Start();
	delete iObserver;
	CleanupStack::PopAndDestroy(obs);
	}


LOCAL_C void TestRemoveL(TInt aDrive)
	{
	// Clear the BIF folders
	theUtils->ClearBifFoldersL();

	// Get the name of the drive
	TDriveUnit unit(aDrive);
	TPtrC drive(unit.Name());

	TParse src;
	TParse dst;

	// Copy the bif file
	theUtils->FindFileL(src, KTestDir, KTestBif1Src);
	User::LeaveIfError(dst.Set(drive, &KBifDir, &KTestBifDst));
	theUtils->FileSession().MkDirAll(dst.FullName());
	
	User::LeaveIfError(BaflUtils::CopyFile(theUtils->FileSession(), src.FullName(), dst.FullName()));

	// Copy the Mbm file
	theUtils->FindFileL(src, KTestDir, KTestMbm1Src);
	User::LeaveIfError(dst.Set(drive, &KBifDir, &KTestMbmDst));
	theUtils->FileSession().MkDirAll(dst.FullName());
	User::LeaveIfError(BaflUtils::CopyFile(theUtils->FileSession(), src.FullName(), dst.FullName()));

	// Create BIF database
	CBIODatabase* db = CBIODatabase::NewLC(theUtils->FileSession());

	// Create observer
	CBifObserver* obs = new(ELeave)CBifObserver;
	CleanupStack::PushL(obs);

	CBifChangeObserver* change = CBifChangeObserver::NewL(*obs, theUtils->FileSession());
	CleanupStack::PushL(change);
	change->Start();

	// Check the test BIO is installed
	test(theUtils->BifInstalledL(*db, KTestBif));
	test(!obs->iFlag);

	// Short delay to check that the observer isn't fired
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(10000000);
	CActiveScheduler::Start();
	delete timer;

	// Check the test BIO is still installed
	test(theUtils->BifInstalledL(*db, KTestBif));
	test(!obs->iFlag);

	// Delete the bif file
	User::LeaveIfError(dst.Set(drive, &KBifDir, &KTestBifDst));
	User::LeaveIfError(theUtils->FileSession().Delete(dst.FullName()));

	// Delete the Mbm file
	User::LeaveIfError(dst.Set(drive, &KBifDir, &KTestMbmDst));
	User::LeaveIfError(theUtils->FileSession().Delete(dst.FullName()));

	// Wait for the notification to come through
	CActiveScheduler::Start();
	test(obs->iFlag);
	test(obs->iBioUid == KTestBif);
	test(obs->iEvent == MBifChangeObserver::EBifDeleted);

	CleanupStack::PopAndDestroy(); // change
	CleanupStack::PopAndDestroy(); // obs
	CleanupStack::PopAndDestroy(); // db
	}


LOCAL_C void doTestsL()
	{
	theUtils->Start(_L("Installing BIF"));
	TestInstallL(EDriveC);
	TestInstallL(EDriveD);
	theUtils->Complete();

	theUtils->Start(_L("Changing BIF"));
	TestChangeL(EDriveC);
	TestChangeL(EDriveD);
	theUtils->Complete();

	theUtils->Start(_L("Removing BIF"));
	TestRemoveL(EDriveC);
	TestRemoveL(EDriveD);
	theUtils->Complete();

	theUtils->Start(_L("Canceling BioObserver before active scheduler has been run"));
	TestImmediateCancelL();
	theUtils->Complete();	
	}


LOCAL_C void TestInstallL(TInt aDrive)
	{
	// Clear the BIF folders
	theUtils->ClearBifFoldersL();

	// Create BIF database
	CBIODatabase* db = CBIODatabase::NewLC(theUtils->FileSession());

	// Create observer
	CBifObserver* obs = new(ELeave)CBifObserver;
	CleanupStack::PushL(obs);

	CBifChangeObserver* change = CBifChangeObserver::NewL(*obs, theUtils->FileSession());
	CleanupStack::PushL(change);
	change->Start();

	// Check the test BIO isn't installed
	test(!theUtils->BifInstalledL(*db, KTestBif));
	test(!obs->iFlag);

	// Short delay to check that the observer isn't fired
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(10000000);
	CActiveScheduler::Start();
	delete timer;

	// Check the test BIO still isn't installed
	test(!theUtils->BifInstalledL(*db, KTestBif));
	test(!obs->iFlag);

	// Get the name of the drive
	TDriveUnit unit(aDrive);
	TPtrC drive(unit.Name());

	TParse src;
	TParse dst;

	// Copy the bif file
	theUtils->FindFileL(src, KTestDir, KTestBif1Src);
	User::LeaveIfError(dst.Set(drive, &KBifDir, &KTestBifDst));
	theUtils->FileSession().MkDirAll(dst.FullName());
	User::LeaveIfError(BaflUtils::CopyFile(theUtils->FileSession(), src.FullName(), dst.FullName()));

	// Copy the Mbm file
	theUtils->FindFileL(src, KTestDir, KTestMbm1Src);
	User::LeaveIfError(dst.Set(drive, &KBifDir, &KTestMbmDst));
	theUtils->FileSession().MkDirAll(dst.FullName());
	User::LeaveIfError(BaflUtils::CopyFile(theUtils->FileSession(), src.FullName(), dst.FullName()));

	// Wait for the notification to come through
	CActiveScheduler::Start();
	test(obs->iFlag);
	test(obs->iBioUid == KTestBif);
	test(obs->iEvent == MBifChangeObserver::EBifAdded);

	CleanupStack::PopAndDestroy(3); // change, obs, db
	}
