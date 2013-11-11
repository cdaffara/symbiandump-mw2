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

#include "t_msv_cacheentry_step.h"
#include "msvcacheentry.h"

CTestCacheEntry::CTestCacheEntry()
	{
	}

CTestCacheEntry::~CTestCacheEntry()
	{
	
	}

void CTestCacheEntry::SetupL()
	{
	_LIT(KSetupL, "SetupL");
	INFO_PRINTF1(KSetupL);
	
	__UHEAP_MARK;
	}

void CTestCacheEntry::TearDownL()
	{
	_LIT(KTearDownL, "TearDownL");
	INFO_PRINTF1(KTearDownL);
	
	__UHEAP_MARKEND;
	}

void CTestCacheEntry::TestEntryL()
	{
	_LIT(KTestEntryL, "TestEntryL");
	INFO_PRINTF1(KTestEntryL);

	//TMsvEntry::SetId is not public so using other api's to test
	TBool isComplete = EFalse;
	TBool isNew = ETrue;
	TBool isVisible = ETrue;

	TMsvEntry msvEntry;

	//Set the message complete flag to isComplete which is false
	msvEntry.SetComplete(isComplete);

	//Set the message new flag to isNew which is true
	msvEntry.SetNew(isNew);

	//Set the message visible flag to isVisible which is true
	msvEntry.SetVisible(isVisible);

	CMsvCacheEntry* cacheEntry = CMsvCacheEntry::NewL();
	cacheEntry->SetEntryL(msvEntry);

	TMsvEntry getMsvEntry;
	getMsvEntry = cacheEntry->Entry();

	//Should return false as isComplete is false
	ASSERT_FALSE(getMsvEntry.Complete());

	//Should return true as isNew is true
	ASSERT_TRUE(getMsvEntry.New());

	//Should return true as isvisible is true
	ASSERT_TRUE(getMsvEntry.Visible());

	delete cacheEntry;
	}

//
void CTestCacheEntry::TestLockL()
	{
	TestEntryLockL();
	TestStoreLockL();
	TestStoreAndEntryLockL();
	}

void CTestCacheEntry::TestEntryLockL()
	{
	_LIT(KTestEntryLockL, "TestEntryLockL");
	INFO_PRINTF1(KTestEntryLockL);

	CMsvCacheEntry* cacheEntry = CMsvCacheEntry::NewL();
	//By default entry should not be locked
	ASSERT_FALSE(cacheEntry->IsEntryLocked());

	//Lock the entry
	ASSERT_EQUALS(cacheEntry->LockEntry(), KErrNone);
	ASSERT_TRUE(cacheEntry->IsEntryLocked());

	//Try locking entry again
	ASSERT_EQUALS(cacheEntry->LockEntry(), KErrLocked);
	ASSERT_TRUE(cacheEntry->IsEntryLocked());

	//Release lock on entry
	cacheEntry->ReleaseEntry();
	ASSERT_FALSE(cacheEntry->IsEntryLocked());

	delete cacheEntry;
	}

void CTestCacheEntry::TestStoreLockL()
	{
	_LIT(KTestStoreLockL, "TestStoreLockL");
	INFO_PRINTF1(KTestStoreLockL);

	CMsvCacheEntry* cacheEntry = CMsvCacheEntry::NewL();
	//By default store should not be locked
	ASSERT_FALSE(cacheEntry->IsStoreLocked());

	//Lock the store
	ASSERT_EQUALS(cacheEntry->LockStore(), KErrNone);
	ASSERT_TRUE(cacheEntry->IsStoreLocked());

	//Try locking entry again
	ASSERT_EQUALS(cacheEntry->LockStore(), KErrLocked);
	ASSERT_TRUE(cacheEntry->IsStoreLocked());

	//Release lock on store
	cacheEntry->ReleaseStore();
	ASSERT_FALSE(cacheEntry->IsStoreLocked());

	delete cacheEntry;
	}

void CTestCacheEntry::TestStoreAndEntryLockL()
	{
	_LIT(KTestStoreAndEntryLockL, "TestStoreAndEntryLockL");
	INFO_PRINTF1(KTestStoreAndEntryLockL);

	CMsvCacheEntry* cacheEntry = CMsvCacheEntry::NewL();

	//Test with entry lock
	//Lock the entry
	cacheEntry->LockEntry();
	ASSERT_TRUE(cacheEntry->IsEntryOrStoreLocked());

	//Should return false as store is not locked
	ASSERT_FALSE(cacheEntry->IsEntryAndStoreLocked());

	//Release lock on entry
	cacheEntry->ReleaseEntryAndStore();

	//Lock should not exist
	ASSERT_FALSE(cacheEntry->IsEntryOrStoreLocked());
	ASSERT_FALSE(cacheEntry->IsEntryLocked());
	
	//Test with store lock
	//Lock the store
	cacheEntry->LockStore();
	ASSERT_TRUE(cacheEntry->IsEntryOrStoreLocked());

	//Should return false as store is not locked
	ASSERT_FALSE(cacheEntry->IsEntryAndStoreLocked());

	//Release lock on store
	cacheEntry->ReleaseEntryAndStore();

	//Lock should not exist
	ASSERT_FALSE(cacheEntry->IsEntryOrStoreLocked());
	ASSERT_FALSE(cacheEntry->IsStoreLocked());

	//Test with entry and store lock
	//Lock the store and entry individually
	cacheEntry->LockEntry();
	cacheEntry->LockStore();
	ASSERT_TRUE(cacheEntry->IsEntryOrStoreLocked());

	//Should return true as both entry and store are locked
	ASSERT_TRUE(cacheEntry->IsEntryAndStoreLocked());

	//Release lock on entry and store
	cacheEntry->ReleaseEntryAndStore();

	//Lock should not exist
	ASSERT_FALSE(cacheEntry->IsEntryOrStoreLocked());
	ASSERT_FALSE(cacheEntry->IsStoreLocked());

	//Lock the store and entry
	cacheEntry->LockEntryAndStore();
	ASSERT_TRUE(cacheEntry->IsEntryOrStoreLocked());

	//Should return true as both entry and store are locked
	ASSERT_TRUE(cacheEntry->IsEntryAndStoreLocked());

	//Release lock on entry and store
	cacheEntry->ReleaseEntryAndStore();

	delete cacheEntry;
	}

//
void CTestCacheEntry::TestReaderCountL()
	{
	_LIT(KTestReaderCountL, "TestReaderCountL");
	INFO_PRINTF1(KTestReaderCountL);

	CMsvCacheEntry* cacheEntry = CMsvCacheEntry::NewL();

	//Should not be locked by default
	ASSERT_FALSE(cacheEntry->IsStoreReadingLocked());

	//Increment reader count by 10
	for(int incRCount = 0; incRCount < 10; incRCount++)
		{
		//Increment the lock count
		cacheEntry->IncStoreReaderCount();
		}

	//Should return true as it is locked
	ASSERT_TRUE(cacheEntry->IsStoreReadingLocked());

	//Test the reader lock after every decrement
	for(int decRCount = 0; decRCount < 10; decRCount++)
		{
		//Decrement the lock count
		cacheEntry->DecStoreReaderCount();

		if( decRCount < 9)
			{
			//Should return true even if one reader has locked
			ASSERT_TRUE(cacheEntry->IsStoreReadingLocked());
			}
		else
			{
			//Should return false as reading lock count will be zero
			ASSERT_FALSE(cacheEntry->IsStoreReadingLocked());
			}
		}

	delete cacheEntry;
	}


void CTestCacheEntry::TestDateL()
	{
	TestCreatedDateL();
	TestLastChangeDateL();
	}

void CTestCacheEntry::TestCreatedDateL()
	{
	_LIT(KTestCreatedDateL, "TestCreatedDateL");
	INFO_PRINTF1(KTestCreatedDateL);

	CMsvCacheEntry* cacheEntry = CMsvCacheEntry::NewL();

	TMsvTime setCreatedMsvTime;
	TTime setCreatedTime;

	//Set the time to universal time
	setCreatedTime.UniversalTime();

	//Set time to current time
	setCreatedMsvTime.SetTime(setCreatedTime);

	//Set the creation date to universal time
	cacheEntry->SetCreatedDate(setCreatedMsvTime);

	//SetCreatedDate (actually it is TMsvTime::SetTime) ignores the seconds and milli seconds. 
	//Set the setCreatedMsvTime to the time similarly, so that it can be used for checking 
	//the getCreatedMsvTime.
	//TTime(TDateTime(1980,EJanuary,0,0,0,0,0)) taken from TMsvTime::MinTime()
	TUint16 days = (TMsvDays)setCreatedTime.DaysFrom(TTime(TDateTime(1980,EJanuary,0,0,0,0,0))).Int();
	TUint16 minutes = (TMsvMinutes)(setCreatedTime.DateTime().Hour() * 60 + setCreatedTime.DateTime().Minute());

	setCreatedTime = TTime(TDateTime(1980,EJanuary,0,0,0,0,0));
	setCreatedTime += TTimeIntervalDays(days);
	setCreatedTime += TTimeIntervalMinutes(minutes);
	//

	//Get the created TMsvTime
	TMsvTime getCreatedMsvTime = cacheEntry->CreatedDate();

	TTime getCreatedTime;
	//Get the TTime from TMsvTime
	getCreatedMsvTime.GetTime(getCreatedTime);

	//Time got should be same with what has been set.
	ASSERT_TRUE(setCreatedTime == getCreatedTime);

	delete cacheEntry;
	}

void CTestCacheEntry::TestLastChangeDateL()
	{
	_LIT(KTestLastChangeDateL, "TestLastChangeDateL");
	INFO_PRINTF1(KTestLastChangeDateL);

	CMsvCacheEntry* cacheEntry = CMsvCacheEntry::NewL();

	TMsvTime setLastChangeMsvTime;
	TTime setLastChangeTime;

	//Set the time to universal time
	setLastChangeTime.UniversalTime();

	//Set time to current time
	setLastChangeMsvTime.SetTime(setLastChangeTime);

	//Set the creation date to universal time
	cacheEntry->SetLastChangeDate(setLastChangeMsvTime);

	//SetLastChangeDate (actually it is TMsvTime::SetTime) ignores the seconds and milli seconds. 
	//Set the setLastChangeMsvTime to the time similarly, so that it can be used for checking 
	//the getLastChangeMsvTime.
	//TTime(TDateTime(1980,EJanuary,0,0,0,0,0)) taken from TMsvTime::MinTime()
	TUint16 days = (TMsvDays)setLastChangeTime.DaysFrom(TTime(TDateTime(1980,EJanuary,0,0,0,0,0))).Int();
	TUint16 minutes = (TMsvMinutes)(setLastChangeTime.DateTime().Hour() * 60 + setLastChangeTime.DateTime().Minute());

	setLastChangeTime = TTime(TDateTime(1980,EJanuary,0,0,0,0,0));
	setLastChangeTime += TTimeIntervalDays(days);
	setLastChangeTime += TTimeIntervalMinutes(minutes);
	//

	//Get the created TMsvTime
	TMsvTime getLastChangeMsvTime = cacheEntry->LastChangeDate();

	TTime getLastChangeTime;

	//Get the TTime from TMsvTime
	getLastChangeMsvTime.GetTime(getLastChangeTime);

	//Time got should be same with what has been set.
	ASSERT_TRUE(setLastChangeTime == getLastChangeTime);

	delete cacheEntry;
	}
//

void CTestCacheEntry::TestCopyEntryL()
	{
	_LIT(KTestCopyEntryL, "TestCopyEntryL");
	INFO_PRINTF1(KTestCopyEntryL);

	//Initialise first TMsvEntry
	TMsvEntry entry1;
	entry1.SetPriority(EMsvHighPriority);
	entry1.SetOperation(ETrue);
	entry1.iServiceId = 3;
	entry1.iRelatedId = 2;
	TUid uid1;
	TUid uid2;
	uid1.iUid = 1;
	uid2.iUid = 2;
	entry1.iType = uid1;
	entry1.iMtm	 = uid2;
	TBuf<32> des1;
	TBuf<32> det1;
	des1.Copy(_L("description1"));
	det1.Copy(_L("details1"));
	entry1.iDescription.Set(des1);
	entry1.iDetails.Set(det1);
	entry1.iDate.HomeTime();
	entry1.iSize = 1234;
	entry1.SetMtmData1(1111);
	entry1.SetMtmData2(2222);
	entry1.SetMtmData3(3333);

	//Initialise second TMsvEntry for testing
	TMsvEntry entry2;
	entry2.SetPriority(EMsvLowPriority);
	entry2.SetOperation(EFalse);
	entry2.iServiceId = 4;
	entry2.iRelatedId = 1;
	TUid uid3;
	TUid uid4;
	uid3.iUid = 3;
	uid4.iUid = 4;
	entry2.iType = uid3;
	entry2.iMtm	 = uid4;
	TBuf<32> des2;
	TBuf<32> det2;
	des2.Copy(_L("description2"));
	det2.Copy(_L("details2"));
	entry2.iDescription.Set(des2);
	entry2.iDetails.Set(det2);
	entry2.iDate.HomeTime();
	entry2.iSize = 1237;
	entry2.SetMtmData1(1112);
	entry2.SetMtmData2(2223);
	entry2.SetMtmData3(3334);

	CMsvCacheEntry* cacheEntry = CMsvCacheEntry::NewL();

	//
	//Test with both description and detail data different with the existing one
	cacheEntry->SetEntryL(entry1);

	TBool changedPrivateInfo;
	cacheEntry->CopyEntryL(entry2, changedPrivateInfo);

	//changedPrivateInfo should be true as entry1 is not same as entry2.
	ASSERT_TRUE(changedPrivateInfo);

	TMsvEntry entry = cacheEntry->Entry();
	//Test the entry got from cache entry with the entry set
	ASSERT_EQUALS(entry, entry2);

	//Should reset the cache entry before reusing
	cacheEntry->ResetEntry();

	//
	//Test with detail data different with the existing one
	cacheEntry->SetEntryL(entry1);

	entry2.iDetails.Set(det1);
	cacheEntry->CopyEntryL(entry2, changedPrivateInfo);

	//changedPrivateInfo should be true as entry1 is not same as entry2.
	ASSERT_TRUE(changedPrivateInfo);

	entry = cacheEntry->Entry();
	//Test the entry got from cache entry with the entry set
	ASSERT_EQUALS(entry, entry2);

	//
	//Test with description data different with the existing one
	cacheEntry->ResetEntry();
	cacheEntry->SetEntryL(entry1);

	entry2.iDetails.Set(det2);
	entry2.iDescription.Set(des1);
	cacheEntry->CopyEntryL(entry2, changedPrivateInfo);

	//changedPrivateInfo should be true as entry1 is not same as entry2.
	ASSERT_TRUE(changedPrivateInfo);

	entry = cacheEntry->Entry();
	//Test the entry got from cache entry with the entry set
	ASSERT_EQUALS(entry, entry2);

	//
	//Test with same description and detail data
	cacheEntry->ResetEntry();
	cacheEntry->SetEntryL(entry1);
	cacheEntry->CopyEntryL(entry1, changedPrivateInfo);

	//changedPrivateInfo should be false as there is no change in description
	//or detail data
	ASSERT_FALSE(changedPrivateInfo);

	entry = cacheEntry->Entry();
	//Test the entry got from cache entry with the entry set
	ASSERT_EQUALS(entry, entry1);

	delete cacheEntry;
	}

void CTestCacheEntry::TestRollBackCopyEntryL()
	{
	_LIT(KTestRollBackCopyEntry, "TestRollBackCopyEntry");
	INFO_PRINTF1(KTestRollBackCopyEntry);

	//Initialise first TMsvEntry
	//Only description and detail data are required for testing
	TMsvEntry entry1;
	TBuf<32> des1;
	TBuf<32> det1;
	des1.Copy(_L("description1"));
	det1.Copy(_L("details1"));
	entry1.iDescription.Set(des1);
	entry1.iDetails.Set(det1);

	//Initialise second TMsvEntry
	TMsvEntry entry2;
	TBuf<32> des2;
	TBuf<32> det2;
	des2.Copy(_L("description2"));
	det2.Copy(_L("details2"));
	entry2.iDescription.Set(des2);
	entry2.iDetails.Set(det2);

	CMsvCacheEntry* cacheEntry = CMsvCacheEntry::NewL();
	//Set cache entry with entry1
	cacheEntry->SetEntryL(entry1);

	TBool changedPrivateInfo;
	//Set entry2 in to cache entry
	cacheEntry->CopyEntryL(entry2, changedPrivateInfo);

	TMsvEntry entry = cacheEntry->Entry();
	ASSERT_EQUALS(entry2, entry);

	//Rollback cache entry to entry1
	cacheEntry->RollBackCopyEntry(entry1);
	entry = cacheEntry->Entry();

	//Test the entry got from cache entry with entry1
	ASSERT_EQUALS(entry, entry1);

	delete cacheEntry;
	}

void CTestCacheEntry::TestOwnerIdL()
	{
	_LIT(KTestOwnerIdL, "TestOwnerIdL");
	INFO_PRINTF1(KTestOwnerIdL);

	CMsvCacheEntry* cacheEntry = CMsvCacheEntry::NewL();

	//Creating secure id for testing
 	_LIT_SECURE_ID(TestSecureId,0x1234567);

 	//Set the entry owner id to test secure id
 	cacheEntry->SetEntryOwnerId(TestSecureId);

 	//Test the entry owner id to be same as test secure id
// 	ASSERT_EQUALS(cacheEntry->EntryOwnerId(), (TSecureId)TestSecureId);

	delete cacheEntry;
	}

void CTestCacheEntry::TestChildArrayL()
	{
	_LIT(KTestChildArrayL, "TestChildArrayL");
	INFO_PRINTF1(KTestChildArrayL);

	CMsvCacheEntry* cacheEntry = CMsvCacheEntry::NewL();
	//Push cache entry on to cleanuup stack
	CleanupStack::PushL(cacheEntry);

	RArray<TMsvId>* childArray;
	//Get the child id's
	childArray = cacheEntry->ChildIdArray();

	//Child array pointer should be null as its not being created yet
	ASSERT_FALSE(NULL!=cacheEntry->ChildIdArray())

	//Create an RArray
 	childArray = new (ELeave) RArray<TMsvId>();

 	//Initialise the array with test TMsvId's
 	childArray->Append(TMsvId(2));
 	childArray->Append(TMsvId(57));
 	childArray->Append(TMsvId(178));

 	//Set the child array with the initialised one
 	/*for(TInt index = 0; index < childArray->Count(); ++index)
 		{
 		cacheEntry->ChildIdArray()->AppendL( (*childArray)[index] );
 		}*/
 	cacheEntry->SetChildIdArray(childArray);
 	

 	RArray<TMsvId>* updatedChildArray;
 	//Get the child array from entry
	updatedChildArray = cacheEntry->ChildIdArray();

	//Child array pointer should not be null as its already created and set
	ASSERT_NOT_SAME(updatedChildArray, (RArray<TMsvId>*)NULL);

	for(int count = 0; count < childArray->Count(); ++count)
 		{
 		//Check the id got from the entry to be same as been set
 		ASSERT_EQUALS((*childArray)[count], (*updatedChildArray)[count]);
 		}

	//Pop entry from cleanup stack and delete
	CleanupStack::PopAndDestroy(cacheEntry);
	}
	


void CTestCacheEntry::TestSetEntryL()
	{
	_LIT(KFormat, "TestSetEntryL");
	INFO_PRINTF1(KFormat);

	CMsvCacheEntry* entry1 = CMsvCacheEntry::NewL();
	TMsvEntry tEntry;
	TRAPD(err, entry1->SetEntryL(tEntry));
	ASSERT_EQUALS(err, KErrNone);
	
	_LIT16(KDetail, "Detail Text");
	_LIT16(KDescription, "Description Text");
	
	HBufC16* detail = KDetail().AllocL();
	tEntry.iDetails.Set(detail->Des());
	TRAP(err, entry1->SetEntryL(tEntry));
	ASSERT_EQUALS(err, KErrNone);
	
	HBufC16* description = KDescription().AllocL();
	tEntry.iDescription.Set(description->Des());
	TRAP(err, entry1->SetEntryL(tEntry));
	ASSERT_EQUALS(err, KErrNone);
	
	_LIT16(KDetail1, "Detail");
	_LIT16(KDescription1, "Description");
	
	HBufC16* detail1 = KDetail1().AllocL();
	tEntry.iDetails.Set(detail1->Des());
	TRAP(err, entry1->SetEntryL(tEntry));
	ASSERT_EQUALS(err, KErrNone);
	delete detail;
	
	HBufC16* description1 = KDescription1().AllocL();
	tEntry.iDescription.Set(description1->Des());
	TRAP(err, entry1->SetEntryL(tEntry));
	ASSERT_EQUALS(err, KErrNone);
	delete description;
	delete entry1;
	delete detail1;
	delete description1;
	}
	
	


CTestSuite* CTestCacheEntry::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestEntryL);
	ADD_ASYNC_TEST_STEP(TestLockL);
	ADD_ASYNC_TEST_STEP(TestEntryLockL);
	ADD_ASYNC_TEST_STEP(TestStoreLockL);
	ADD_ASYNC_TEST_STEP(TestStoreAndEntryLockL);
	ADD_ASYNC_TEST_STEP(TestReaderCountL);
	ADD_ASYNC_TEST_STEP(TestDateL);
	ADD_ASYNC_TEST_STEP(TestCreatedDateL);
	ADD_ASYNC_TEST_STEP(TestLastChangeDateL);
	ADD_ASYNC_TEST_STEP(TestCopyEntryL);
	ADD_ASYNC_TEST_STEP(TestOwnerIdL);
	ADD_ASYNC_TEST_STEP(TestRollBackCopyEntryL);
	ADD_ASYNC_TEST_STEP(TestChildArrayL);
	ADD_ASYNC_TEST_STEP(TestSetEntryL);
	END_SUITE;
	}
