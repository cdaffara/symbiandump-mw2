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
// t_msv_entryfreepool_step.cpp
//

#include "t_msv_entryfreepool_step.h"
#include "msvcachevisiblefolder.h"
#include <msvids.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

#define MY_ASSERT_TRUE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_TRUE(a);
#define MY_ASSERT_FALSE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_FALSE(a);
#define MY_ASSERT_EQUALS(a,b)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_EQUALS(a,b);
#define MY_ASSERT_NOT_NULL(a)	\
		INFO_PRINTF1(KFunction);	\
		ASSERT_NOT_NULL(a);


//Ini file which contains the cache setting
_LIT(KSTDMSGINI, "C:\\private\\1000484b\\msgcache.ini");
_LIT(KMSGINI_1, "C:\\private\\1000484b\\msgcache1.ini");
_LIT(KMSGINI_2, "C:\\private\\1000484b\\msgcache2.ini");
_LIT(KMSGINI_3, "C:\\private\\1000484b\\msgcache3.ini");
_LIT(KMSGINI_4, "C:\\private\\1000484b\\msgcache4.ini");
_LIT(KMSGINI_5, "C:\\private\\1000484b\\msgcache5.ini");



//Ini keywords used for cache setting
_LIT(KIniMsvMaximumCacheSize, "MsvMaximumCacheSize");


CTestEntryFreePool::CTestEntryFreePool()
	{
	}

CTestEntryFreePool::~CTestEntryFreePool()
	{
	}

void CTestEntryFreePool::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);
	
	TInt err =	iFileHandle.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);
//	err = iFileHandle.Replace(KMSGINI_4, KSTDMSGINI);
	CFileMan *file = NULL;
	TRAP_IGNORE(file = CFileMan::NewL(iFileHandle));
	err = file->Copy(KMSGINI_4, KSTDMSGINI);
	MY_ASSERT_EQUALS(err, KErrNone);
	delete file;
	
	TDblQue<CMsvCacheVisibleFolder> entryCache;
	TRAP(err, iFreePool = CMsvEntryFreePool::CreateL(&entryCache));
	MY_ASSERT_EQUALS(err, KErrNone);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TMsvPreferredDrive driveEntry;
	driveEntry.driveNum = EDriveC;
	driveEntry.driveId = 1;
	driveEntry.status = EMsvMessageStoreAvailableStatus;
	TRAP_IGNORE(iDriveList = CMsvPreferredDriveList::CreateL());
	iDriveList->Insert(driveEntry, 0);
	iDriveList->SetCurrentDriveIndex(0);
#endif	

	}




void CTestEntryFreePool::TearDownL()
	{
	_LIT(KFunction, "TearDownL");
	INFO_PRINTF1(KFunction);
	
#ifdef _DEBUG
	CMsvEntryFreePool::Destroy(iFreePool);
	iFreePool = NULL;
#else
	delete iFreePool;
	iFreePool = NULL;
#endif
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	delete iDriveList;
#endif

	// Restore a sane config file at the end of every test step.
	CFileMan* file = CFileMan::NewL(iFileHandle);
	TInt err = file->Copy(KMSGINI_4, KSTDMSGINI);
	MY_ASSERT_EQUALS(err, KErrNone);
	delete file;
		
	iFileHandle.Close();
	}




void CTestEntryFreePool::TestCreate()
	{
	_LIT(KFunction, "TestCreate");
	INFO_PRINTF1(KFunction);
	
	TInt err;
	TDblQue<CMsvCacheVisibleFolder> entryCache;

	// 1. Verify data of the .ini file.
	MY_ASSERT_EQUALS(1024, iFreePool->GetMaximumCacheSize());
	MY_ASSERT_EQUALS(70, iFreePool->GetCacheThreshold());
	MY_ASSERT_EQUALS(3297, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(3297, iFreePool->GetNumberOfEntriesPresentInFreePool());
	
	// 2. Perform CreateL() when .ini file is missing.
	// 2.1. Revert the setupL()
#ifdef _DEBUG
	CMsvEntryFreePool::Destroy(iFreePool);
	iFreePool = NULL;
#else
	delete iFreePool;
	iFreePool = NULL;
#endif
//	err = iFileHandle.Replace(KSTDMSGINI, KMSGINI_4);
	CFileMan *file = NULL;
	TRAP_IGNORE(file = CFileMan::NewL(iFileHandle));
	err = file->Copy(KSTDMSGINI, KMSGINI_4);
	/*MY_ASSERT_EQUALS(err, KErrNone);
	// 2.2. Now create new freepool instance.
	TRAP(err, iFreePool = CMsvEntryFreePool::CreateL(&entryCache));
	// Make sure that .ini file is also missing from Z: drive,
	// as, if the file is not found in C: drive the function
	// will automatically check in Z: drive as well.
	MY_ASSERT_EQUALS(err, KErrNotFound);
	MY_ASSERT_EQUALS(iFreePool, (CMsvEntryFreePool *)NULL);*/
	
	// 2.3. Some parameter missing from .ini file.
	// Param KIniMsvInitialCacheCreation is missing in KMSGINI_2.
	err = file->Copy(KMSGINI_2, KSTDMSGINI);
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iFreePool = CMsvEntryFreePool::CreateL(&entryCache));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(1000, iFreePool->GetMaximumCacheSize());
	//MY_ASSERT_EQUALS(1610, iFreePool->GetGranularity());
	MY_ASSERT_EQUALS(70, iFreePool->GetCacheThreshold());
	MY_ASSERT_EQUALS(3220, iFreePool->GetCacheNumberOfEntriesCreated());
	
	// 2. Perform CreateL() when .ini file is empty.
	// 2.4. Revert the setupL()
#ifdef _DEBUG
	CMsvEntryFreePool::Destroy(iFreePool);
	iFreePool = NULL;
#else
	delete iFreePool;
	iFreePool = NULL;
#endif
	err = file->Copy(KMSGINI_3, KSTDMSGINI);
	MY_ASSERT_EQUALS(err, KErrNone);
	// 2.2. Now create new freepool instance.
	TRAP(err, iFreePool = CMsvEntryFreePool::CreateL(&entryCache));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(1024, iFreePool->GetMaximumCacheSize());
	//MY_ASSERT_EQUALS(1648, iFreePool->GetGranularity());
	MY_ASSERT_EQUALS(70, iFreePool->GetCacheThreshold());
	MY_ASSERT_EQUALS(3297, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(3297, iFreePool->GetNumberOfEntriesPresentInFreePool());

	err = file->Copy(KSTDMSGINI, KMSGINI_3);
	MY_ASSERT_EQUALS(err, KErrNone);
	delete file;
	
	}





void CTestEntryFreePool::TestInstance()
	{
	_LIT(KFunction, "TestInstance");
	INFO_PRINTF1(KFunction);

	// Freepool instance is already created.
	CMsvEntryFreePool* freePool = CMsvEntryFreePool::Instance();
	MY_ASSERT_EQUALS(freePool, iFreePool);
	
	}



/**
 * TestEntry()
 * 
 * The function checks following APIs.
 *  - Entry()
 *  - ReleaseEntry()
 *  - ReleaseEntrySet()
 */
void CTestEntryFreePool::TestEntry()
	{
	_LIT(KFunction, "TestEntry");
	INFO_PRINTF1(KFunction);
	RPointerArray<CMsvCacheEntry> msvEntries;

	// 1. Fetching five entries from freepool.
	MY_ASSERT_EQUALS(3297, iFreePool->GetCacheNumberOfEntriesCreated());
	for(TInt index=0; index<5; index++)
		{
		CMsvCacheEntry *entry = NULL;
		TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
		MY_ASSERT_TRUE(entry!=NULL);
		TRAP_IGNORE(msvEntries.AppendL(entry));
		}
	// Freepool is NOT empty now.
	MY_ASSERT_EQUALS(3292, CMsvEntryFreePool::Instance()->GetNumberOfEntriesPresentInFreePool());
	MY_ASSERT_EQUALS(3297, iFreePool->GetCacheNumberOfEntriesCreated());
	
	// 2. Releasing first two entries to freepool.
	for(TInt index=0; index<2; index++)
		{
		CMsvCacheEntry* entry = msvEntries[0];
		msvEntries.Remove(0);
		CMsvEntryFreePool::Instance()->ReleaseEntry(entry);
		}
	MY_ASSERT_EQUALS(3294, CMsvEntryFreePool::Instance()->GetNumberOfEntriesPresentInFreePool());
	MY_ASSERT_EQUALS(3297, iFreePool->GetCacheNumberOfEntriesCreated());
	
	// 3. Releasing entries in bulk.
	TRAP_IGNORE(CMsvEntryFreePool::Instance()->ReleaseEntrySet(msvEntries));
	MY_ASSERT_EQUALS(3297, CMsvEntryFreePool::Instance()->GetNumberOfEntriesPresentInFreePool());
	MY_ASSERT_EQUALS(3297, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(0, msvEntries.Count());	
	
	// Fetching all entries from freepool.
	TInt count = CMsvEntryFreePool::Instance()->GetNumberOfEntriesPresentInFreePool();
	for(TInt index=0; index< count; index++)
		{
		CMsvCacheEntry* entry = NULL;
		TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
		ASSERT_TRUE(entry!=NULL);
		TRAP_IGNORE(msvEntries.AppendL(entry));
		}
	// Freepool is empty now.
	MY_ASSERT_EQUALS(0, CMsvEntryFreePool::Instance()->GetNumberOfEntriesPresentInFreePool());
	MY_ASSERT_EQUALS(3297, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(3297, msvEntries.Count());

	// 4. Fetching two entries from freepool when freepool is empty.
	CMsvCacheEntry* entry = NULL;
	TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_TRUE(entry!=NULL);
	TRAP_IGNORE(msvEntries.AppendL(entry));
	MY_ASSERT_EQUALS(NULL, CMsvEntryFreePool::Instance()->GetNumberOfEntriesPresentInFreePool());
	MY_ASSERT_EQUALS(3298, iFreePool->GetCacheNumberOfEntriesCreated());
	
	entry = NULL;
	TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_TRUE(entry!=NULL);
	TRAP_IGNORE(msvEntries.AppendL(entry));
	MY_ASSERT_EQUALS(NULL, CMsvEntryFreePool::Instance()->GetNumberOfEntriesPresentInFreePool());
	MY_ASSERT_EQUALS(3299, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(3299, msvEntries.Count());
	
	// 5. Releasing entries in bulk.
	TRAP_IGNORE(CMsvEntryFreePool::Instance()->ReleaseEntrySet(msvEntries));
	MY_ASSERT_EQUALS(3299, CMsvEntryFreePool::Instance()->GetNumberOfEntriesPresentInFreePool());
	MY_ASSERT_EQUALS(3299, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(0, msvEntries.Count());
	
	// 6. Releasing entries in bulk.
	// Passed list is empty.
	TRAP_IGNORE(CMsvEntryFreePool::Instance()->ReleaseEntrySet(msvEntries));
	MY_ASSERT_EQUALS(3299, CMsvEntryFreePool::Instance()->GetNumberOfEntriesPresentInFreePool());
	MY_ASSERT_EQUALS(3299, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(0, msvEntries.Count());
	
	
	// 7. Releasing entries had error. Freepool will have less number of entries created.
	entry = NULL;
	TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_TRUE(entry!=NULL);
	TRAP_IGNORE(msvEntries.AppendL(entry));
	CMsvEntryFreePool::Instance()->iReleaseError = ETrue; // for simulating error while releasing entries
	TRAP_IGNORE(CMsvEntryFreePool::Instance()->ReleaseEntrySet(msvEntries));
	MY_ASSERT_EQUALS(3298, CMsvEntryFreePool::Instance()->GetNumberOfEntriesPresentInFreePool());
	MY_ASSERT_EQUALS(3298, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(0, msvEntries.Count());
		
	// 8. Releasing entries had error. Freepool will have less number of entries created.
	entry = NULL;
	TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_TRUE(entry!=NULL);
	TRAP_IGNORE(msvEntries.AppendL(entry));
	entry = NULL;
	TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_TRUE(entry!=NULL);
	TRAP_IGNORE(msvEntries.AppendL(entry));
	entry = NULL;
	TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_TRUE(entry!=NULL);
	TRAP_IGNORE(msvEntries.AppendL(entry));
	
	CMsvEntryFreePool::Instance()->iReleaseError = ETrue; // for simulating error while releasing entries
	CMsvEntryFreePool::Instance()->iReleaseErrorOffset = 1; // for simulating error while releasing entries
	TRAP_IGNORE(CMsvEntryFreePool::Instance()->ReleaseEntrySet(msvEntries));
	MY_ASSERT_EQUALS(3296, CMsvEntryFreePool::Instance()->GetNumberOfEntriesPresentInFreePool());
	MY_ASSERT_EQUALS(3296, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(0, msvEntries.Count());
		
	msvEntries.Reset();
	TInt err = iFileHandle.Replace(KSTDMSGINI, KMSGINI_4);
	MY_ASSERT_EQUALS(err, KErrNone);
	}
	
void CTestEntryFreePool::TestIsAllocationRequired()
	{
	_LIT(KFunction, "TestIsAllocationRequired");
	INFO_PRINTF1(KFunction);
	
	TDblQue<CMsvCacheVisibleFolder> entryCache;
	RPointerArray<CMsvCacheEntry> entryPointer;
	CMsvCacheVisibleFolder *f1 = NULL;
	TInt err;
	
	
	// 1. Test with initially created freepool with no entries removed from it
	TDblQue<CMsvCacheVisibleFolder> temp;
	TRAP_IGNORE(f1 = CMsvCacheVisibleFolder::NewL((TMsvId)(KFirstFreeEntryId + 05)));
	temp.AddFirst(*f1);
#ifdef _DEBUG
	CMsvEntryFreePool::Destroy(iFreePool);
	iFreePool = NULL;
#else
	delete iFreePool;
	iFreePool = NULL;
#endif
	TRAP(err, iFreePool = CMsvEntryFreePool::CreateL(&temp));
	TRAP_IGNORE(MY_ASSERT_FALSE(iFreePool->IsAllocationRequiredL())); //not reached threshold
	temp.Reset();

		
	// 2. Create another freepool. Remove 70% of entries from it.
	err = iFileHandle.Replace(KSTDMSGINI, KMSGINI_4);
	MY_ASSERT_EQUALS(err, KErrNone);
	err = iFileHandle.Replace(KMSGINI_5, KSTDMSGINI); //INI file with cache size = 10KB
	MY_ASSERT_EQUALS(err, KErrNone);
#ifdef _DEBUG
	CMsvEntryFreePool::Destroy(iFreePool);//destroy the old free pool
	iFreePool = NULL;
#else
	delete iFreePool;
	iFreePool = NULL;
#endif
	TRAP_IGNORE(f1 = CMsvCacheVisibleFolder::NewL((TMsvId)KFirstFreeEntryId + 05));
	entryCache.AddFirst(*f1);
	CMsvEntryFreePool *fp1 = NULL;
	TRAP(err, fp1 = CMsvEntryFreePool::CreateL(&entryCache)); //create another freepool
	ASSERT_EQUALS(KErrNone, err);
	for(TInt index = 0; index < 56; ++index)
		{
		CMsvCacheEntry *entry = NULL;
		TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
		entry->Entry().SetId((TMsvId)7+index + KFirstFreeEntryId);
		entry->Entry().SetParent((TMsvId)KFirstFreeEntryId + 05);
		entryPointer.Append(entry);
		}
	TRAP_IGNORE(f1->AddEntryListL(entryPointer, ETrue));
	TRAP_IGNORE(MY_ASSERT_TRUE(fp1->IsAllocationRequiredL())); //has reached threshold
		
	f1->ReleaseAllBlocks(err);
	err = iFileHandle.Replace(KSTDMSGINI, KMSGINI_5);
	MY_ASSERT_EQUALS(err, KErrNone);
	entryCache.Reset();
	entryPointer.Reset();
	
	
	// 3. Maximum cache size has been reached and attempt to take an entry is made.
	//	  Entries will be swapped at the last entry addition. Freepool status
	//	  will be checked against the second threshold of 80%.
	err = iFileHandle.Replace(KMSGINI_5, KSTDMSGINI); //INI file with cache size = 10KB
	MY_ASSERT_EQUALS(err, KErrNone);
#ifdef _DEBUG
	CMsvEntryFreePool::Destroy(fp1);//destroy the old freepool
	fp1 = NULL;
#else
	delete fp1;
	fp1 = NULL;
#endif
	TRAP_IGNORE(f1 = CMsvCacheVisibleFolder::NewL((TMsvId)KFirstFreeEntryId + 05));
	entryCache.AddFirst(*f1);
	TRAP(err, fp1 = CMsvEntryFreePool::CreateL(&entryCache)); //create another freepool
	ASSERT_EQUALS(KErrNone, err);
	CMsvCacheEntry *entry = NULL;
	for(TInt index = 0; index < 80; ++index)
		{
		TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
		entry->Entry().SetId((TMsvId)7+ index + KFirstFreeEntryId);
		entry->Entry().SetParent((TMsvId)KFirstFreeEntryId + 05);
		entryPointer.Append(entry);
		}
	TRAP_IGNORE(f1->AddEntryListL(entryPointer, ETrue));
	TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
	entry->Entry().SetId((TMsvId)7+81+KFirstFreeEntryId);
	entry->Entry().SetParent((TMsvId)KFirstFreeEntryId + 05);
	entryPointer.Append(entry);
	TRAP_IGNORE(f1->AddEntryL(entry));	
	TRAP_IGNORE(MY_ASSERT_FALSE(fp1->IsAllocationRequiredL()));
	
	f1->ReleaseAllBlocks(err);
#ifdef _DEBUG
	CMsvEntryFreePool::Destroy(fp1);
	fp1 = NULL;
#else
	delete iFreePool;
	fp1 = NULL;
#endif
	err = iFileHandle.Replace(KSTDMSGINI, KMSGINI_5);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	entryPointer.Close();
	}




void CTestEntryFreePool::TestAllocateMemory()
	{
	_LIT(KFunction, "TestAllocateMemoryL");
	INFO_PRINTF1(KFunction);
	
	TInt err;
	TDblQue<CMsvCacheVisibleFolder> entryCache(CMsvCacheVisibleFolder::iOffset);
	RPointerArray<CMsvCacheEntry> entryPointer;
	CMsvCacheEntry *entry = NULL;
	
	// 1. test the amount the cache expands by.
	err = iFileHandle.Replace(KSTDMSGINI, KMSGINI_4);
	MY_ASSERT_EQUALS(err, KErrNone);
	err = iFileHandle.Replace(KMSGINI_5, KSTDMSGINI); //INI file with cache size = 10KB
	MY_ASSERT_EQUALS(err, KErrNone);
#ifdef _DEBUG
	CMsvEntryFreePool::Destroy(iFreePool);
	iFreePool = NULL;
#else
	delete iFreePool;
	iFreePool = NULL;
#endif
	CMsvCacheVisibleFolder *tempFolder = NULL;
	TRAP_IGNORE(tempFolder = CMsvCacheVisibleFolder::NewL((TMsvId)KFirstFreeEntryId + 50));
	entryCache.AddFirst(*tempFolder);
	TRAP(err, iFreePool = CMsvEntryFreePool::CreateL(&entryCache)); //create another freepool
	ASSERT_EQUALS(KErrNone, err);
	for(TInt index = 0; index < 8; ++index)
		{
		TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
		entry->Entry().SetId((TMsvId)51+KFirstFreeEntryId + index);
		entry->Entry().SetParent((TMsvId)KFirstFreeEntryId + 50);
		entryPointer.Append(entry);
		}
	TRAP_IGNORE(tempFolder->AddEntryListL(entryPointer, ETrue));
	err = iFreePool->GetMaximumCacheSize();
	MY_ASSERT_EQUALS(10, iFreePool->GetMaximumCacheSize());
	//MY_ASSERT_EQUALS(16, iFreePool->GetGranularity());
	MY_ASSERT_EQUALS(70, iFreePool->GetCacheThreshold());
	MY_ASSERT_EQUALS(8, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(0, iFreePool->GetNumberOfEntriesPresentInFreePool());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFreePool->IsAllocationRequiredL()));
	TRAP(err, iFreePool->AllocateMemoryL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(21, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(13, iFreePool->GetNumberOfEntriesPresentInFreePool());
	tempFolder->ReleaseAllBlocks(err);
	delete tempFolder;
	
	// 2. test when there's <threshold memory left to allocate.
	CMsvCacheVisibleFolder *folder = NULL;
	TRAP_IGNORE(folder = CMsvCacheVisibleFolder::NewL((TMsvId)KFirstFreeEntryId + 60));
	entryCache.Reset();
	entryCache.AddFirst(*folder);
#ifdef _DEBUG
	CMsvEntryFreePool::Destroy(iFreePool);
	iFreePool = NULL;
#else
	delete iFreePool;
	iFreePool = NULL;
#endif
	TRAP(err, iFreePool = CMsvEntryFreePool::CreateL(&entryCache)); //create another freepool
	ASSERT_EQUALS(KErrNone, err); //10%
	entryPointer.Reset();
	for(TInt index = 0; index < 56; ++index)
		{
		CMsvCacheEntry *temp = NULL;
		TRAP_IGNORE(temp = CMsvEntryFreePool::Instance()->EntryL());
		temp->Entry().SetId((TMsvId)(61+KFirstFreeEntryId + index));
		temp->Entry().SetParent((TMsvId)KFirstFreeEntryId + 60);
		entryPointer.Append(temp);
		}
	TRAP_IGNORE(folder->AddEntryListL(entryPointer, ETrue));
	MY_ASSERT_EQUALS(56, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(0, iFreePool->GetNumberOfEntriesPresentInFreePool());
	//although only 70% of the max entries have been allocated above, total cache size is 86%.
	//only 11 more entries can be allocated whereas the increment value is 16 entries.
	TBool status = EFalse;
	TRAP_IGNORE(status = iFreePool->IsAllocationRequiredL());
	TRAP(err, iFreePool->AllocateMemoryL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(67, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(11, iFreePool->GetNumberOfEntriesPresentInFreePool());
	folder->ReleaseAllBlocks(err);
	delete folder;
	
	// 3. not all memory is occupied. but on allocation, threshold is still exceeded.
	//	  swapping should now be performed.
#ifdef _DEBUG
	CMsvEntryFreePool::Destroy(iFreePool);
	iFreePool = NULL;
#else
	delete iFreePool;
	iFreePool = NULL;
#endif
	entryCache.Reset();
	CMsvCacheVisibleFolder *f2 = NULL;
	TRAP_IGNORE(f2 = CMsvCacheVisibleFolder::NewL((TMsvId)KFirstFreeEntryId+6));
	entryCache.AddFirst(*f2);
	CMsvCacheVisibleFolder *f1 = NULL;
	TRAP_IGNORE( f1 = CMsvCacheVisibleFolder::NewL((TMsvId)KFirstFreeEntryId+5));
	entryCache.AddFirst(*f1);
	TRAP(err, iFreePool = CMsvEntryFreePool::CreateL(&entryCache));
	MY_ASSERT_EQUALS(err, KErrNone);
	entryPointer.Reset();
	for(TInt index = 0; index < 44; ++index) //4 entries into f2
		{
		TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
		entry->Entry().SetId((TMsvId)(7+KFirstFreeEntryId + index));
		entry->Entry().SetParent((TMsvId)KFirstFreeEntryId + 6);
		entryPointer.Append(entry);
		}
	TRAP_IGNORE(f2->AddEntryListL(entryPointer, ETrue));
	entryPointer.Reset();
	for(TInt index = 44; index < 60; ++index) //16 entries into f1
		{
		TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
		entry->Entry().SetId((TMsvId)(7+KFirstFreeEntryId + index));
		entry->Entry().SetParent((TMsvId)KFirstFreeEntryId + 5);
		entryPointer.Append(entry);
		}
	TRAP_IGNORE(f1->AddEntryListL(entryPointer, ETrue));
	
	MY_ASSERT_EQUALS(60, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(0, iFreePool->GetNumberOfEntriesPresentInFreePool());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFreePool->IsAllocationRequiredL()));
	TRAP(err, iFreePool->AllocateMemoryL());
	//5 entries will be allocated and 100% is reached. but the cache size still
	//exceeds the threshold of the total cache size, so we go in for the next increment.
	//swapping is performed and 44 entries of folder f2 are released.
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(65, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(49, iFreePool->GetNumberOfEntriesPresentInFreePool());
	//44 entries were released from f2(last in the Dblque)
	
	f1->ReleaseAllBlocks(err);
	f2->ReleaseAllBlocks(err);
	delete f1;
	delete f2;
	
	// 4. 100% of the memory is occupied and then we attempt to allocate more memory.
#ifdef _DEBUG
	CMsvEntryFreePool::Destroy(iFreePool);
	iFreePool = NULL;
#else
	delete iFreePool;
	iFreePool = NULL;
#endif
	entryCache.Reset();
	CMsvCacheVisibleFolder *f3 = NULL;
	TRAP_IGNORE(f3 = CMsvCacheVisibleFolder::NewL((TMsvId)KFirstFreeEntryId + 4));
	entryCache.AddFirst(*f3);
	TRAP_IGNORE(f2 = CMsvCacheVisibleFolder::NewL((TMsvId)KFirstFreeEntryId + 6));
	entryCache.AddFirst(*f2);
	TRAP_IGNORE(f1 = CMsvCacheVisibleFolder::NewL((TMsvId)KFirstFreeEntryId + 5));
	entryCache.AddLast(*f1);
	
	TRAP(err, iFreePool = CMsvEntryFreePool::CreateL(&entryCache));
	MY_ASSERT_EQUALS(err, KErrNone);
	entryPointer.Reset();
	for(TInt index = 0; index < 20; ++index) //20 entries into f2
		{
		TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
		entry->Entry().SetId((TMsvId)(7+KFirstFreeEntryId + index));
		entry->Entry().SetParent((TMsvId)KFirstFreeEntryId + 6);
		entryPointer.Append(entry);
		}
	TRAP_IGNORE(f2->AddEntryListL(entryPointer, EFalse));
	entryPointer.Reset();
	for(TInt index = 20; index < 40; ++index) //20 entries into f3
		{
		TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
		entry->Entry().SetId((TMsvId)(7+KFirstFreeEntryId + index));
		entry->Entry().SetParent((TMsvId)KFirstFreeEntryId + 4);
		entryPointer.Append(entry);
		}
	TRAP_IGNORE(f3->AddEntryListL(entryPointer, EFalse));
	entryPointer.Reset();
	for(TInt index = 40; index < 56; ++index) //16 entries into f1
		{
		TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
		entry->Entry().SetId((TMsvId)(7+KFirstFreeEntryId + index));
		entry->Entry().SetParent((TMsvId)KFirstFreeEntryId + 5);
		entry->LockEntry();
		entryPointer.Append(entry);
		} //make f1 non-swappable
	TRAP_IGNORE(f1->AddEntryListL(entryPointer, EFalse));
	
	MY_ASSERT_EQUALS(56, iFreePool->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(0, iFreePool->GetNumberOfEntriesPresentInFreePool());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFreePool->IsAllocationRequiredL()));
	//56 created. allocation gives 10 more entries.
	TRAP(err, iFreePool->AllocateMemoryL());
	//max size is reached now, but allocation is required to maintain threshold.
	TRAP_IGNORE(MY_ASSERT_TRUE(iFreePool->IsAllocationRequiredL()));
	//allocation swaps entries, from f2 in this case.
	TRAP(err, iFreePool->AllocateMemoryL());
	TDblQueIter<CMsvCacheVisibleFolder> dqIter(entryCache);
    dqIter.SetToFirst();
    while ((f1 = dqIter++) != NULL)
        {
        f1->iDlink.Deque();
        delete f1;
        };

	err = iFileHandle.Replace(KSTDMSGINI, KMSGINI_5);
	MY_ASSERT_EQUALS(err, KErrNone);
	entryPointer.Close();
	}
	


void CTestEntryFreePool::TestMissingIniFile()
	{
	_LIT(KFunction, "TestMissingIniFile");
	INFO_PRINTF1(KFunction);
	
	// 1. Delete the cache file from C: that is used to	create the freepool.
	_LIT(KTestDummyName, "C:\\private\\1000484b\\dummy.ini");
	TInt err = iFileHandle.Rename(KSTDMSGINI, KTestDummyName);
	MY_ASSERT_EQUALS(err, KErrNone);
	delete iFreePool;
	iFreePool = NULL;
	
	// Create freepool.
	TDblQue<CMsvCacheVisibleFolder> entryCache;
	TRAP(err, iFreePool = CMsvEntryFreePool::CreateL(&entryCache));
	MY_ASSERT_EQUALS(err, KErrNone); //freepool is created taking the INI file from Z:
	
	// Restore the cache file in C:.
	err = iFileHandle.Rename(KTestDummyName, KSTDMSGINI);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	}
	
	
	
CTestSuite* CTestEntryFreePool::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
#ifdef WINSCW
/*	ADD_ASYNC_TEST_STEP(TestCreate);
	ADD_ASYNC_TEST_STEP(TestInstance);*/
#endif
	ADD_ASYNC_TEST_STEP(TestEntry);
#ifdef WINSCW
/*	ADD_ASYNC_TEST_STEP(TestIsAllocationRequired);
	ADD_ASYNC_TEST_STEP(TestAllocateMemory);*/
#endif
	ADD_ASYNC_TEST_STEP(TestMissingIniFile);
	END_SUITE;
	}
