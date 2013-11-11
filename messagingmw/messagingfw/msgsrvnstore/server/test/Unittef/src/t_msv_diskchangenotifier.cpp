// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// MACRO Declaration
// 
//

/**
 @file 
 @internalTechnology
*/

#include "t_msv_diskchangenotifier.h"


#define DEFAULT_RETURN KErrNone
#define MY_ASSERT_TRUE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_TRUE(a);
#define MY_ASSERT_FALSE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_FALSE(a);
#define MY_ASSERT_EQUALS(a,b)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_EQUALS(a,b);


void CTestDiskChangeObserver::DiskRemoved(const TDriveUnit& aDrive, TBool aTestRemoval)
	{
	iType = EMsvTestDiskRemoveNotification;
	iDrive = aDrive;
	aTestRemoval = EFalse;
	CActiveScheduler::Stop();
	}

void CTestDiskChangeObserver::DiskInserted(const TDriveUnit& aDrive, TBool aTestInsert)
	{
	iType = EMsvTestDiskInsertNotification;
	iDrive = aDrive;
	aTestInsert = EFalse;
	CActiveScheduler::Stop();
	}

void CTestDiskChangeObserver::DiskChanged(const TDriveUnit& aDrive)
	{
	iType = EMsvTestDiskChangeNotification;
	iDrive = aDrive;
	CActiveScheduler::Stop();
	}
	
	
void CTestDiskChangeNotifier::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);
	
	__UHEAP_MARK;
	
	iDiskChangeObserver = new(ELeave) CTestDiskChangeObserver();
	iDiskChangeNotifierD = CMsvDiskChangeNotifier::NewL((TDriveUnit)EDriveD, *iDiskChangeObserver);
	iDiskChangeNotifierE = CMsvDiskChangeNotifier::NewL((TDriveUnit)EDriveE, *iDiskChangeObserver);
	iDiskChangeNotifierF = CMsvDiskChangeNotifier::NewL((TDriveUnit)EDriveF, *iDiskChangeObserver);
	
	iFs.Connect();
	iFileSystemD.Create(20);
	iFileSystemE.Create(20);
	iFileSystemF.Create(20);
	MY_ASSERT_EQUALS(KErrNone, iFs.FileSystemName(iFileSystemD, (TInt) EDriveD));
	MY_ASSERT_EQUALS(KErrNone, iFs.FileSystemName(iFileSystemE, (TInt) EDriveE));
	MY_ASSERT_EQUALS(KErrNone, iFs.FileSystemName(iFileSystemF, (TInt) EDriveF));
	
	iDiskChangeNotifierD->Start();
	iDiskChangeNotifierE->Start();
	iDiskChangeNotifierF->Start();
	}


void CTestDiskChangeNotifier::TearDownL()
	{
	_LIT(KFunction, "TearDownL");
	INFO_PRINTF1(KFunction);	
	
	delete iDiskChangeNotifierD;
	delete iDiskChangeNotifierE;
	delete iDiskChangeNotifierF;
	delete iDiskChangeObserver;
	iFileSystemD.Close();
	iFileSystemE.Close();
	iFileSystemF.Close();
	iFs.Close();
	__UHEAP_MARKEND;
	}



CTestDiskChangeNotifier::CTestDiskChangeNotifier()
:iDiskChangeObserver(NULL),iDiskChangeNotifierD(NULL),
iDiskChangeNotifierE(NULL),iDiskChangeNotifierF(NULL)
	{
	
	}

CTestDiskChangeNotifier::~CTestDiskChangeNotifier()
	{
	
	}

void CTestDiskChangeNotifier::TestNotification()
	{
	_LIT(KFunction, "TestNotification");
	INFO_PRINTF1(KFunction);
	
	MY_ASSERT_EQUALS(KErrNone, iFs.DismountFileSystem(iFileSystemD, EDriveD));
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iDiskChangeObserver->iType, EMsvTestDiskRemoveNotification);
	MY_ASSERT_EQUALS((TInt)iDiskChangeObserver->iDrive, (TInt)EDriveD);
	
	MY_ASSERT_EQUALS(KErrNone, iFs.DismountFileSystem(iFileSystemE, EDriveE));
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iDiskChangeObserver->iType, EMsvTestDiskRemoveNotification);
	MY_ASSERT_EQUALS((TInt)iDiskChangeObserver->iDrive, (TInt)EDriveE);
	
	MY_ASSERT_EQUALS(KErrNone, iFs.DismountFileSystem(iFileSystemF, EDriveF));
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iDiskChangeObserver->iType, EMsvTestDiskRemoveNotification);
	MY_ASSERT_EQUALS((TInt)iDiskChangeObserver->iDrive, (TInt)EDriveF);
	
	MY_ASSERT_EQUALS(KErrNone, iFs.MountFileSystem(iFileSystemD, EDriveD));
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iDiskChangeObserver->iType, EMsvTestDiskInsertNotification);
	MY_ASSERT_EQUALS((TInt)iDiskChangeObserver->iDrive, (TInt)EDriveD);
	
	MY_ASSERT_EQUALS(KErrNone, iFs.MountFileSystem(iFileSystemE, EDriveE));
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iDiskChangeObserver->iType, EMsvTestDiskInsertNotification);
	MY_ASSERT_EQUALS((TInt)iDiskChangeObserver->iDrive, (TInt)EDriveE);
	
	MY_ASSERT_EQUALS(KErrNone, iFs.MountFileSystem(iFileSystemF, EDriveF));
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iDiskChangeObserver->iType, EMsvTestDiskInsertNotification);
	MY_ASSERT_EQUALS((TInt)iDiskChangeObserver->iDrive, (TInt)EDriveF);	
	}
	

CTestSuite* CTestDiskChangeNotifier::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestNotification);
	END_SUITE;
	}

