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
//

/**
 @file 
 @internalTechnology
*/
 
#include "t_msv_preferreddrivelist_step.h"

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


CTestMsvPreferredDriveList::CTestMsvPreferredDriveList()
	{
	}



CTestMsvPreferredDriveList::~CTestMsvPreferredDriveList()
	{	
	}



void CTestMsvPreferredDriveList::SetupL()
	{
	_LIT(KFunction, "*_*SetupL");
	INFO_PRINTF1(KFunction);
	__UHEAP_MARK;
	
	}



void CTestMsvPreferredDriveList::TearDownL()
	{
	_LIT(KFunction, "*_*TearDownL");
	INFO_PRINTF1(KFunction);
	
	if(iDriveList)
		{
		delete iDriveList;
		}
	__UHEAP_MARKEND;
	}



void CTestMsvPreferredDriveList::TestDriveOperationsL()
	{
	_LIT(KFunction, "TestDriveOperationsL");
	INFO_PRINTF1(KFunction);
	
	TMsvPreferredDrive drive;
	TUint driveIndex = 0;
	TUint driveId = 0;
		
	// 1. Insert drives into the drive list.
	// 1.0 Creation and testing creation.
	iDriveList = CMsvPreferredDriveList::CreateL();
	MY_ASSERT_TRUE(NULL != iDriveList);
	driveIndex = 0;
	MY_ASSERT_EQUALS(iDriveList->CurrentDriveIndex(), driveIndex);
	MY_ASSERT_EQUALS(iDriveList->Count(), 0);
	// 1.1 Insert.
	// 1.1.1 Insert first drive.
	drive.driveNum = RFs::GetSystemDrive();
	drive.driveId = 1;
	drive.status = EMsvMessageStoreAvailableStatus;
	iDriveList->Insert(drive, driveIndex);
	MY_ASSERT_EQUALS(iDriveList->Count(), 1);
	iDriveList->SetCurrentDriveIndex(driveIndex);
	// 1.1.2 Insert second drive after first one. Current drive index unchanged.
	drive.driveNum = EDriveD;
	drive.driveId = 2;
	drive.status = EMsvMessageStoreUnavailableStatus;
	iDriveList->Insert(drive, ++driveIndex);
	MY_ASSERT_EQUALS(iDriveList->Count(), 2);
	MY_ASSERT_EQUALS(iDriveList->CurrentDriveIndex(), (TUint)0);
	// 1.1.3 Insert third drive before first one. Current drive index changed.
	drive.driveNum = EDriveE;
	drive.driveId = 3;
	drive.status = EMsvMessageStoreNotSupportedStatus;
	iDriveList->Insert(drive, 0);
	MY_ASSERT_EQUALS(iDriveList->Count(), 3);
	MY_ASSERT_EQUALS(iDriveList->CurrentDriveIndex(), (TUint)1);
	// 1.1.4 Insert drive in last position. Current drive index unchanged.
	drive.driveNum = EDriveF;
	drive.driveId = 4;
	drive.status = EMsvMessageStoreNotSupportedStatus;
	iDriveList->Insert(drive, iDriveList->Count());
	MY_ASSERT_EQUALS(iDriveList->Count(), 4);
	MY_ASSERT_EQUALS(iDriveList->CurrentDriveIndex(), (TUint)1);
	
	
	// 2. Drive information retrieval.
	// 2.1 Drive info on valid drive index.
	driveIndex = 1;
	TRAPD(err, iDriveList->DriveInfoL(driveIndex, drive));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)1, drive.driveId);
	MY_ASSERT_EQUALS(RFs::GetSystemDrive(), drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	// 2.2 Drive info on invalid drive index.
	driveIndex = 10;
	TRAP(err, iDriveList->DriveInfoL(driveIndex, drive));
	MY_ASSERT_EQUALS(KErrNotFound, err);
	
	
	// 3. Drive id updation.
	// 3.1 Update drive id of a valid drive index.
	driveIndex = 0;
	driveId = 5;
	TRAP(err, iDriveList->UpdateDriveIdL(driveIndex, driveId));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, iDriveList->DriveInfoL(driveIndex, drive));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)5, drive.driveId);
	MY_ASSERT_EQUALS(EDriveE, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreNotSupportedStatus, drive.status);
	// 3.2 Update drive if of an invalid drive index.
	driveIndex = 10;
	driveId = 6;
	TRAP(err, iDriveList->UpdateDriveIdL(driveIndex, driveId));
	MY_ASSERT_EQUALS(KErrNotFound, err);
	
	
	// 4. Drive status updation.
	// 4.1 Update drive status of a valid drive index.
	driveIndex = 0;
	TRAP(err, iDriveList->UpdateDriveStatusL(driveIndex, EMsvMessageStoreUnavailableStatus));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, iDriveList->DriveInfoL(driveIndex, drive));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)5, drive.driveId);
	MY_ASSERT_EQUALS(EDriveE, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreUnavailableStatus, drive.status);
	// 4.2 Update drive if of an invalid drive index.
	driveIndex = 10;
	TRAP(err, iDriveList->UpdateDriveStatusL(driveIndex, EMsvMessageStoreUnavailableStatus));
	MY_ASSERT_EQUALS(KErrNotFound, err);
	
	
	// 5. Retrieving drive number from drive id.
	// 5.1 Get drive number of drive id 0. Should be current drive's number.
	driveId = 0;
	TDriveNumber retval;
	MY_ASSERT_EQUALS(KErrNone, iDriveList->GetDriveNumber(driveId, retval));
	MY_ASSERT_EQUALS(RFs::GetSystemDrive(), retval);
	// 5.2 Get drive number of other drives.
	driveId = 1;
	MY_ASSERT_EQUALS(KErrNone, iDriveList->GetDriveNumber(driveId, retval));
	MY_ASSERT_EQUALS(RFs::GetSystemDrive(), retval);
	driveId = 2;
	MY_ASSERT_EQUALS(KErrNone, iDriveList->GetDriveNumber(driveId, retval));
	MY_ASSERT_EQUALS(EDriveD, retval);
	// 5.3 Get drive number of invalid drive id.
	driveId = 10;
	MY_ASSERT_EQUALS(KErrNotFound, iDriveList->GetDriveNumber(driveId, retval));
		
	
	// 6. Other drive information.
	// 6.1 Get current drive index.
	MY_ASSERT_EQUALS((TUint)1, iDriveList->CurrentDriveIndex());
	// 6.2 Set current drive index.
	driveIndex = 2;
	iDriveList->SetCurrentDriveIndex(driveIndex);
	MY_ASSERT_EQUALS((TUint)2, iDriveList->CurrentDriveIndex());
	// 6.3 Set current drive index to an invalid index.
	driveIndex = 10;
	iDriveList->SetCurrentDriveIndex(driveIndex);
	MY_ASSERT_EQUALS((TUint)2, iDriveList->CurrentDriveIndex()); //unchanged.
	// 6.4 Get current drive id.
	// 6.4.0 Get.
//	MY_ASSERT_EQUALS((TUint)2, iDriveList->CurrentDriveId());
	// 6.4.1 Set and get.
	driveIndex = 1;
	iDriveList->SetCurrentDriveIndex(driveIndex);
//	MY_ASSERT_EQUALS((TUint)1, iDriveList->CurrentDriveId());
	
	
	// 7. Indexing.
	// 7.1
	driveIndex = 2;
	MY_ASSERT_EQUALS((*iDriveList)[driveIndex].driveNum, EDriveD);
	// 7.2
	driveIndex = 2;
	TRAP(err, iDriveList->DriveInfoL(driveIndex, drive));
	MY_ASSERT_EQUALS(EMsvMessageStoreUnavailableStatus, drive.status);
	(*iDriveList)[driveIndex].status = EMsvMessageStoreAvailableStatus;
	TRAP(err, iDriveList->DriveInfoL(driveIndex, drive));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)2, drive.driveId);
	MY_ASSERT_EQUALS(EDriveD, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status); //changed.
	
	}
	


CTestSuite* CTestMsvPreferredDriveList::CreateSuiteL(const TDesC& aName)
// static
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestDriveOperationsL);
	END_SUITE;
	}

