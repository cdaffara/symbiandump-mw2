// Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// t_msv_cache_indextableentry_step.cpp
//

#include "t_msv_cache_indextableentry_step.h"
#include <msvids.h>
#include <flogger.h>
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

		
_LIT(KSTDMSGINI, "C:\\private\\1000484b\\msgcache.ini");
_LIT(KMSGINI_2, "C:\\private\\1000484b\\msgcache2.ini");


CTestIndexTableEntry::CTestIndexTableEntry()
	{
	}

CTestIndexTableEntry::~CTestIndexTableEntry()
	{
	
	}

void CTestIndexTableEntry::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);
	
	//__UHEAP_MARK;
	
	TInt err =	iFileHandle.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);
	//err = iFileHandle.Replace(KMSGINI_2, KSTDMSGINI);
	CFileMan* file = CFileMan::NewL(iFileHandle);
	err = file->Copy(KMSGINI_2, KSTDMSGINI);
	MY_ASSERT_EQUALS(err, KErrNone);
	delete file;

	//Create entry free pool.
	TDblQue<CMsvCacheVisibleFolder> iEntryCache;
	iMsvEntryFreePool = CMsvEntryFreePool::CreateL(&iEntryCache);
	
	//create index table entry
	iIndexTableEntry = CMsvCacheIndexTableEntry::NewL();
	iEntryPointer = new(ELeave) RPointerArray<CMsvCacheEntry>;

	}

void CTestIndexTableEntry::TearDownL()
	{
	_LIT(KFunction, "TearDownL");
	INFO_PRINTF1(KFunction);
	
	//destroy the index table entry
	delete iIndexTableEntry;
	if(iEntryPointer)
		{
		iEntryPointer->Close();
		delete iEntryPointer;
		iEntryPointer = NULL;
		}
	
	//iFileHandle.Replace(KSTDMSGINI, KMSGINI_2);
	iFileHandle.Close();
	
	// Delete entry free pool.
#ifdef _DEBUG
	CMsvEntryFreePool::Destroy(iMsvEntryFreePool);
	iMsvEntryFreePool = NULL;
#else
	delete iMsvEntryFreePool;
	iMsvEntryFreePool = NULL;
#endif
	
	//__UHEAP_MARKEND;
	}

void CTestIndexTableEntry::TestConstructorL()
	{
	_LIT(KFunction, "TestConstructorL");
	INFO_PRINTF1(KFunction);
	
	//Test whether a Index table entry is created correctly.
	CMsvCacheIndexTableEntry* tempTableEntry = NULL;
	TRAPD(err, tempTableEntry = new(ELeave) CMsvCacheIndexTableEntry);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_TRUE(tempTableEntry->BlockPtr() == NULL);
	MY_ASSERT_TRUE(tempTableEntry->GetMaxMsvIdRange() == 0);
	MY_ASSERT_TRUE(tempTableEntry->GetMinMsvIdRange() == 0);
	MY_ASSERT_FALSE(tempTableEntry->IsGrandChildPresent());
	MY_ASSERT_FALSE(tempTableEntry->IsDirty());
	delete tempTableEntry;

	//Test the same using NewL
	TRAP(err, tempTableEntry = CMsvCacheIndexTableEntry::NewL());
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_TRUE(tempTableEntry->BlockPtr() == NULL);
	MY_ASSERT_TRUE(tempTableEntry->GetMaxMsvIdRange() == 0);
	MY_ASSERT_TRUE(tempTableEntry->GetMinMsvIdRange() == 0);
	MY_ASSERT_FALSE(tempTableEntry->IsGrandChildPresent());
	MY_ASSERT_FALSE(tempTableEntry->IsDirty());
	delete tempTableEntry;

	//Test creation with addition of one cache entry
	CMsvCacheEntry* entry = NULL;
	TRAP(err, entry = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entry->Entry().SetId((TMsvId)50);
	entry->Entry().SetParent((TMsvId)25);
	TRAP(err, tempTableEntry = CMsvCacheIndexTableEntry::NewL(entry));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_TRUE(tempTableEntry->BlockPtr() != NULL);
	MY_ASSERT_TRUE(tempTableEntry->GetMaxMsvIdRange() == 50);
	MY_ASSERT_TRUE(tempTableEntry->GetMinMsvIdRange() == 50);
	MY_ASSERT_FALSE(tempTableEntry->IsGrandChildPresent());
	MY_ASSERT_FALSE(tempTableEntry->IsDirty());
	MY_ASSERT_TRUE(tempTableEntry->GetEntry((TMsvId)50, entry));
	delete tempTableEntry;

	//Test the same using NewLC
	TRAP(err, entry = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entry->Entry().SetId((TMsvId)50);
	entry->Entry().SetParent((TMsvId)25);
	tempTableEntry = CMsvCacheIndexTableEntry::NewLC(entry);
	MY_ASSERT_TRUE(tempTableEntry->BlockPtr() != NULL);
	MY_ASSERT_TRUE(tempTableEntry->GetMaxMsvIdRange() == 50);
	MY_ASSERT_TRUE(tempTableEntry->GetMinMsvIdRange() == 50);
	MY_ASSERT_FALSE(tempTableEntry->IsGrandChildPresent());
	MY_ASSERT_FALSE(tempTableEntry->IsDirty());
	MY_ASSERT_TRUE(tempTableEntry->GetEntry((TMsvId)50, entry));
	CleanupStack::Pop(); //tempTableEntry
	delete tempTableEntry;

	//Test creation with addition of list of entries
	RPointerArray<CMsvCacheEntry> addList;
	for(TInt index = 0; index < 10; ++index)
		{
		TRAP(err, entry = CMsvEntryFreePool::Instance()->EntryL());
		MY_ASSERT_EQUALS(KErrNone, err);
		entry->Entry().SetId((TMsvId)50+index);
		entry->Entry().SetParent((TMsvId)25);
		TRAP(err, addList.AppendL(entry));
		MY_ASSERT_EQUALS(KErrNone, err);
		}
	TRAP(err, tempTableEntry = CMsvCacheIndexTableEntry::NewL(addList));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_TRUE(tempTableEntry->BlockPtr() != NULL);
	MY_ASSERT_TRUE(tempTableEntry->Size() == 10);
	delete tempTableEntry;

	//Test the same using NewLC
	addList.Reset();
	for(TInt index = 0; index < 10; ++index)
		{
		TRAP(err, entry = CMsvEntryFreePool::Instance()->EntryL());
		MY_ASSERT_EQUALS(KErrNone, err);
		entry->Entry().SetId((TMsvId)50+index);
		entry->Entry().SetParent((TMsvId)25);
		TRAP(err, addList.AppendL(entry));
		MY_ASSERT_EQUALS(KErrNone, err);
		}
	tempTableEntry = CMsvCacheIndexTableEntry::NewLC(addList);
	MY_ASSERT_TRUE(tempTableEntry->BlockPtr() != NULL);
	MY_ASSERT_TRUE(tempTableEntry->Size() == 10);
	CleanupStack::Pop(); //tempTableEntry
	delete tempTableEntry;
	addList.Close();
	}
	
void CTestIndexTableEntry::TestAddEntry()
	{

	_LIT(KFunction, "TestAddEntry");
	INFO_PRINTF1(KFunction);
	TInt err;
	TInt bulkCount = 100;
	
	TMsvId msvId = KFirstFreeEntryId;
	
	RArray<CMsvCacheEntry*> entries;
	RArray<TMsvEntry> tmsvEntries;
	
	// t1. add single entry to empty block
	MY_ASSERT_TRUE(iIndexTableEntry->BlockPtr() == NULL);
	TRAP(err, entryA = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entryA->Entry().SetId(++msvId);
	++msvId;
	TTime timeBefore = iIndexTableEntry->AccessTime();
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entryA->GetId()));
	User::After(1000);
	TTime timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_EQUALS(iIndexTableEntry->GetMinMsvIdRange(), entryA->GetId());
	MY_ASSERT_EQUALS(iIndexTableEntry->GetMaxMsvIdRange(), entryA->GetId());
	MY_ASSERT_TRUE(timeBefore != timeAfter);
	
	
	// t2. add entry to non-empty block
	TRAP(err, entryB = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entryB->Entry().SetId(++msvId);
	++msvId;
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	TRAP(err, iIndexTableEntry->AddEntryL(entryB));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entryB->GetId()));
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeBefore != timeAfter);
	MY_ASSERT_EQUALS(iIndexTableEntry->GetMaxMsvIdRange(), entryB->GetId());
	
	
	// t3. add duplicate entry
	//---3.1 older entry has details to be preserved
	TInt sizeBefore = iIndexTableEntry->Size();
	CMsvCacheEntry *dupB = NULL;
	TRAP(err, dupB = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	dupB->Entry().SetId(entryB->GetId());
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	TRAP(err, iIndexTableEntry->AddEntryL(dupB)); //replace older entry = EFalse (default)
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entryB->GetId()));
	RPointerArray<CMsvCacheEntry>* blockPtr = iIndexTableEntry->BlockPtr();
	for(TInt index = 0; index < sizeBefore; ++index) //entries have different address
		{
		if( (*blockPtr)[index]->GetId() == entryB->GetId() )
			{
			MY_ASSERT_TRUE( &(*blockPtr)[index] != &dupB );
			}
		}
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeBefore == timeAfter);
	TInt sizeAfter = iIndexTableEntry->Size();
	MY_ASSERT_EQUALS(sizeBefore, sizeAfter);
	//---3.2 newer entry has more recent details
	TRAP(err, entryD = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entryD->Entry().SetId(++msvId);
	++msvId;
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	TRAP(err, iIndexTableEntry->AddEntryL(entryD));
	MY_ASSERT_EQUALS(err, KErrNone);
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeBefore != timeAfter); 
	CMsvCacheEntry *dupD = NULL;
	TRAP(err, dupD = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	dupD->Entry().SetId(entryD->GetId());
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	TRAP(err, iIndexTableEntry->AddEntryL(dupD, ETrue)); //replace older entry = ETrue
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(dupD->GetId()));
	for(TInt index = 0; index < sizeBefore; ++index) //entries have different address
		{
		if( (*blockPtr)[index]->GetId() == dupD->GetId() )
			{
			MY_ASSERT_TRUE( (*blockPtr)[index] != entryD );
			}
		}
	MY_ASSERT_FALSE(iIndexTableEntry->EntryExists(entryD->GetId()));
	timeAfter = iIndexTableEntry->AccessTime();
	//MY_ASSERT_TRUE(timeBefore != timeAfter);
	
	
	// t4. add entry with TMsvId < iMinMsvId
	TMsvId minId = iIndexTableEntry->GetMinMsvIdRange();
	TRAP(err, entryC = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entryC->Entry().SetId(minId - 1);
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	TRAP(err, iIndexTableEntry->AddEntryL(entryC));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(iIndexTableEntry->GetMinMsvIdRange(), entryC->GetId());
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeBefore != timeAfter);
	
	
	// t5. add entry with TMsvId > iMaxMsvId
	TMsvId maxId = iIndexTableEntry->GetMaxMsvIdRange();
	TRAP(err, entryD = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entryD->Entry().SetId(maxId + 1);
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	TRAP(err, iIndexTableEntry->AddEntryL(entryD));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(iIndexTableEntry->GetMaxMsvIdRange(), entryD->GetId());
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeBefore != timeAfter);
		
	
	// t6. add large amount of entries
	TMsvId id = msvId + 10;
	for(TInt index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id);
		TRAP(err, entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		MY_ASSERT_EQUALS(KErrNone, err);
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		timeBefore = iIndexTableEntry->AccessTime();
		User::After(1000);
		TRAP(err, iIndexTableEntry->AddEntryL((entries[index])));
		ASSERT_EQUALS(err, KErrNone);
		timeAfter = iIndexTableEntry->AccessTime();
		ASSERT_TRUE(timeBefore != timeAfter);
		}
	for(TInt index = 0; index < bulkCount; ++index)
		{
		ASSERT_TRUE(iIndexTableEntry->EntryExists(tmsvEntries[index].Id()));
		}
	
	tmsvEntries.Close();
	entries.Close();
	}

void CTestIndexTableEntry::TestAddEntrySet()
	{
	_LIT(KFunction, "TestAddEntrySet");
	INFO_PRINTF1(KFunction);
	
	TInt err, index;
	TInt bulkCount = 5; //100;
	TTime timeBefore, timeAfter;
	
	TMsvId msvId = KFirstFreeEntryId;
	
	TMsvId id = msvId + 500;
	RPointerArray<CMsvCacheEntry> oEntries;
		
	// T1:	block is NULL. add only non-immediate children.
	MY_ASSERT_TRUE(iIndexTableEntry->BlockPtr() == NULL);
		
	RArray<CMsvCacheEntry*> entries;
	RArray<TMsvEntry> tmsvEntries;
	for(index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id);
		TRAP(err, entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		MY_ASSERT_EQUALS(KErrNone, err);
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		TRAP(err, iEntryPointer->AppendL(entries[index]));
		MY_ASSERT_EQUALS(KErrNone, err);
		}
	
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	TRAP(err, iIndexTableEntry->AddEntrySetL(*iEntryPointer, 0, iEntryPointer->Count()));
	MY_ASSERT_EQUALS(err, KErrNone);
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeBefore != timeAfter);
	for(index = 0; index < bulkCount; ++index)
		{
		MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entries[index]->GetId()));
		}
		
	//---add entries less than those in aEntries
	iIndexTableEntry->ReleaseBlock(ETrue);
	iEntryPointer->Reset();
	entries.Reset();
	tmsvEntries.Reset();
	for(index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id);
		TRAP(err, entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		MY_ASSERT_EQUALS(KErrNone, err);
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		TRAP(err, iEntryPointer->AppendL(entries[index]));
		MY_ASSERT_EQUALS(KErrNone, err);
		}
	
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	TInt aCount = 3;
	TRAP(err, iIndexTableEntry->AddEntrySetL(*iEntryPointer, 0, aCount));
	MY_ASSERT_EQUALS(err, KErrNone);
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeBefore != timeAfter);
	for(index = 0; index < aCount; ++index)
		{
		MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entries[index]->GetId()));
		}
	MY_ASSERT_EQUALS(iIndexTableEntry->Size(), aCount);
	for(index = aCount; index < bulkCount; ++index) //manually release remaining entries
		{
		CMsvEntryFreePool::Instance()->ReleaseEntryL(entries[index]);
		}
	
	//---aCount > aEntries.Count()
	iIndexTableEntry->ReleaseBlock(ETrue);
	iEntryPointer->Reset();
	entries.Reset();
	tmsvEntries.Reset();
	for(index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id);
		TRAP(err, entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		MY_ASSERT_EQUALS(KErrNone, err);
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		TRAP_IGNORE(iEntryPointer->AppendL(entries[index]));
		}
	
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	 aCount = bulkCount + 10;
	TRAP(err, iIndexTableEntry->AddEntrySetL(*iEntryPointer, 0, aCount));
	MY_ASSERT_EQUALS(err, KErrNone);
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeBefore != timeAfter);
	for(index = 0; index < bulkCount; ++index)
		{
		MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entries[index]->GetId()));
		}
	MY_ASSERT_EQUALS(iIndexTableEntry->Size(), bulkCount);
	
	//---[aInitIndex..aCount] < [0..bulkCount]
	iIndexTableEntry->ReleaseBlock(ETrue);
	iEntryPointer->Reset();
	entries.Reset();
	tmsvEntries.Reset();
	for(index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id);
		TRAP(err, entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		MY_ASSERT_EQUALS(KErrNone, err);
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		TRAP(err, iEntryPointer->AppendL(entries[index]));
		MY_ASSERT_EQUALS(KErrNone, err);
		}
	
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	TInt aInitIndex = 1;
	aCount = 3;
	//entries added are entries[1] to entries[3]. 0 and 4 are left out.
	TRAP(err, iIndexTableEntry->AddEntrySetL(*iEntryPointer, aInitIndex, aCount));
	MY_ASSERT_EQUALS(err, KErrNone);
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeBefore != timeAfter);
	for(index = aInitIndex; index < aCount+aInitIndex; ++index)
		{
		MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entries[index]->GetId()));
		}
	MY_ASSERT_EQUALS(iIndexTableEntry->Size(), aCount);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(entries[0]);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(entries[4]);
		
	//---out of bounds index
	iIndexTableEntry->ReleaseBlock(ETrue);
	iEntryPointer->Reset();
	entries.Reset();
	tmsvEntries.Reset();
	for(index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id);
		TRAP(err, entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		MY_ASSERT_EQUALS(KErrNone, err);
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		TRAP(err, iEntryPointer->AppendL(entries[index]));
		MY_ASSERT_EQUALS(KErrNone, err);
		}
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	aInitIndex = -1;
	aCount = 3;
	TRAP(err, iIndexTableEntry->AddEntrySetL(*iEntryPointer, aInitIndex, aCount));
	MY_ASSERT_EQUALS(err, KErrNone);
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeBefore == timeAfter);
	for(index = 0; index < bulkCount; ++index)
		{
		MY_ASSERT_FALSE(iIndexTableEntry->EntryExists(entries[index]->GetId()));
		}
	MY_ASSERT_EQUALS(iIndexTableEntry->Size(), 0);
	for(index = 0; index < bulkCount; ++index)
		{
		CMsvEntryFreePool::Instance()->ReleaseEntryL(entries[index]);
		}
		

	
	// T2:	block is NULL. add only immediate children.
	// start afresh.
	iIndexTableEntry->ReleaseBlock(ETrue);
	msvId = KFirstFreeEntryId;
	TRAP(err, parent = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	parent->Entry().SetId(++msvId);
	++msvId;
	TRAP(err, entryA = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entryA->Entry().SetId(++msvId);
	++msvId;
	TRAP(err, entryB = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entryB->Entry().SetId(++msvId);
	++msvId;
	TRAP(err, entryC = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entryC->Entry().SetId(++msvId);
	++msvId;
	iEntryPointer->Reset();
	entries.Reset();
	tmsvEntries.Reset();
	
	TRAP(err, iEntryPointer->AppendL(parent));
	MY_ASSERT_EQUALS(KErrNone, err);
	entryA->Entry().SetParent(parent->GetId());
	TRAP(err, iEntryPointer->AppendL(entryA));
	MY_ASSERT_EQUALS(KErrNone, err);
	entryB->Entry().SetParent(parent->GetId());
	TRAP(err, iEntryPointer->AppendL(entryB));
	MY_ASSERT_EQUALS(KErrNone, err);
	entryC->Entry().SetParent(parent->GetId());
	TRAP(err, iEntryPointer->AppendL(entryC));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	id = msvId;
	
	for(index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id);
		TRAP(err, entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		MY_ASSERT_EQUALS(KErrNone, err);
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		entries[index]->Entry().SetParent(parent->GetId());
		TRAP(err, iEntryPointer->AppendL(entries[index]));
		MY_ASSERT_EQUALS(KErrNone, err);
		}
	
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	TRAP(err, iIndexTableEntry->AddEntrySetL(*iEntryPointer, 0, iEntryPointer->Count()));
	MY_ASSERT_EQUALS(err, KErrNone);
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeBefore != timeAfter);
	oEntries.Reset();
	TRAP(err, iIndexTableEntry->GetChildrenL(parent->GetId(), oEntries));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(oEntries.Count(), bulkCount + 4); 
	for(index = 0; index < bulkCount; ++index)
		{
		MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(tmsvEntries[index].Id()));
		}
	MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entryA->GetId()));
	MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entryB->GetId()));
	MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entryC->GetId()));
	
	
	// T3:	block is not NULL. add only non-immediate children. there are immediate 
	//		children present in the old block.
	//start fresh
	iIndexTableEntry->ReleaseBlock(ETrue);
	iEntryPointer->Reset();
	entries.Reset();
	tmsvEntries.Reset();
	
	
	TRAP(err, parent = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	parent->Entry().SetId(++id);
	++id;
	TRAP(err, entryA = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entryA->Entry().SetId(++id);
	++id;
	TRAP(err, entryB = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entryB->Entry().SetId(++id);
	++id;
	
	
	TRAP(err, iIndexTableEntry->AddEntryL(parent));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iIndexTableEntry->AddEntryL(entryB));
	MY_ASSERT_EQUALS(err, KErrNone); //A and B are non-immediate children
	
	for(index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id);
		TRAP(err, entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		MY_ASSERT_EQUALS(KErrNone, err);
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		entries[index]->Entry().SetParent(parent->GetId());
		TRAP_IGNORE(iEntryPointer->AppendL(entries[index]));
		}
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	TRAP(err, iIndexTableEntry->AddEntrySetL(*iEntryPointer, 0, iEntryPointer->Count())); //add some children
	MY_ASSERT_EQUALS(err, KErrNone);
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeBefore != timeAfter);
	for(index = 0; index < bulkCount; ++index)
		{
		MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(tmsvEntries[index].Id()));
		}
	MY_ASSERT_EQUALS(iIndexTableEntry->Size(), 3 + bulkCount);
	
	entries.Reset();
	tmsvEntries.Reset();
	iEntryPointer->Reset();
	CMsvCacheEntry *dupA = NULL;
	TRAP(err, dupA = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	dupA->Entry().SetId(entryA->GetId());
	CMsvCacheEntry *dupB = NULL;
	TRAP(err, dupB = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	dupB->Entry().SetId(entryB->GetId());
	TMsvId entryAId = entryA->GetId();
	TMsvId entryBId = entryB->GetId();
	TRAP(err, iEntryPointer->AppendL(dupA));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, iEntryPointer->AppendL(dupB)); //A and B are duplicates in the new list
	MY_ASSERT_EQUALS(KErrNone, err);
	for(index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id);
		TRAP(err, entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		MY_ASSERT_EQUALS(KErrNone, err);
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		TRAP(err, iEntryPointer->AppendL(entries[index]));
		MY_ASSERT_EQUALS(KErrNone, err);
		}
	
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	TRAP(err, iIndexTableEntry->AddEntrySetL(*iEntryPointer, 0, iEntryPointer->Count())); //add some non-immediate children
	MY_ASSERT_EQUALS(err, KErrNone);
	iIndexTableEntry->SetGrandChildPresent();
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeBefore != timeAfter);
	//MY_ASSERT_TRUE(iIndexTableEntry->IsGrandChildPresent());
	MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entryAId));
	MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entryBId));
	//MY_ASSERT_EQUALS(iIndexTableEntry->GetMaxMsvIdRange(), entries[entries.Count() - 1]->GetId());
	for(index = 0; index < bulkCount; ++index)
		{
		MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(tmsvEntries[index].Id()));
		}
	oEntries.Reset();
	TRAP(err, iIndexTableEntry->GetChildrenL(parent->GetId(), oEntries));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(oEntries.Count(), bulkCount);
	MY_ASSERT_EQUALS(iIndexTableEntry->Size(), 3 + bulkCount + bulkCount);
	
	iIndexTableEntry->ReleaseBlock(ETrue);
	
	oEntries.Reset();
	
	entries.Reset();
	entries.Close();
	tmsvEntries.Reset();
	tmsvEntries.Close();
	iEntryPointer->Reset();
	}

void CTestIndexTableEntry::TestGetEntry()
	{
	_LIT(KFunction, "TestGetEntry");
	INFO_PRINTF1(KFunction);
	
	CMsvCacheEntry *oEntry = NULL;
	TBool ret;
	TInt err;
	TInt bulkCount = 100;
	TMsvId msvId = KFirstFreeEntryId;
	TMsvId id = msvId;
	TTime timeBefore, timeAfter;
	
	
	// t1. GetEntry from an empty block 
	ret = iIndexTableEntry->GetEntry(id, oEntry); //pass some arbitrary TMsvId for this
	MY_ASSERT_TRUE(oEntry == NULL);
	MY_ASSERT_FALSE(ret);
	
	
	// t2. get entry from non-empty block
	RArray<CMsvCacheEntry*> entries;
	RArray<TMsvEntry> tmsvEntries;
	for(TInt index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id);
		TRAP(err, entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		MY_ASSERT_EQUALS(KErrNone, err);
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		TRAP(err, iEntryPointer->AppendL(entries[index]));
		MY_ASSERT_EQUALS(KErrNone, err);
		}
	TRAP(err, iIndexTableEntry->AddEntrySetL(*iEntryPointer, 0, iEntryPointer->Count())); //add some entries
	MY_ASSERT_EQUALS(err, KErrNone);
	for(TInt index = 0; index < bulkCount; ++index)
		{
		ASSERT_TRUE(iIndexTableEntry->EntryExists(tmsvEntries[index].Id()));
		}
	TRAP(err, entryA = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entryA->Entry().SetId(++id);
	++id;
	TRAP(err, entryB = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entryB->Entry().SetId(++id);
	++id;
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iIndexTableEntry->AddEntryL(entryB));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entryA->GetId()));
	MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entryB->GetId()));
	
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	ret = iIndexTableEntry->GetEntry(entryA->GetId(), oEntry);
	MY_ASSERT_EQUALS(oEntry->GetId(), entryA->GetId());
	MY_ASSERT_TRUE(ret);
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeAfter != timeBefore);
	
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	ret = iIndexTableEntry->GetEntry(entryB->GetId(), oEntry);
	MY_ASSERT_EQUALS(oEntry->GetId(), entryB->GetId());
	MY_ASSERT_TRUE(ret);
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeAfter != timeBefore);
	
	for(TInt index = 0; index < bulkCount; ++index)
		{
		timeBefore = iIndexTableEntry->AccessTime();
		User::After(1000);
		ret = iIndexTableEntry->GetEntry(tmsvEntries[index].Id(), oEntry);
		ASSERT_TRUE(ret);
		ASSERT_EQUALS(oEntry->GetId(), tmsvEntries[index].Id());
		timeAfter = iIndexTableEntry->AccessTime();
		ASSERT_TRUE(timeAfter != timeBefore);
		}
	
	
	// t3. getentry - non-existing entry
	ret = iIndexTableEntry->GetEntry((TMsvId)50, oEntry); //50 has not been added
	MY_ASSERT_FALSE(ret);
	tmsvEntries.Reset();
	for(TInt index = 0 ; index < bulkCount; ++index)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(++id);
		}
	for(TInt index = 0; index < bulkCount; ++index)
		{
		timeBefore = iIndexTableEntry->AccessTime();
		ret = iIndexTableEntry->GetEntry(tmsvEntries[index].Id(), oEntry);
		ASSERT_FALSE(ret);
		timeAfter = iIndexTableEntry->AccessTime();
		//MY_ASSERT_FALSE(timeAfter == timeBefore);
		}
		
	entries.Reset();
	entries.Close();
	tmsvEntries.Reset();
	tmsvEntries.Close();
	
	}


void CTestIndexTableEntry::TestGetChildren()
	{
	_LIT(KFunction, "TestGetChildren");
	INFO_PRINTF1(KFunction);
	TInt err;
	TInt size;
	TInt bulkCount = 10;
	TMsvId msvId = KFirstFreeEntryId;
	TMsvId id = msvId + 50;
	TTime timeBefore, timeAfter;
	
	RArray<CMsvCacheEntry*> entries;
	RArray<TMsvEntry> tmsvEntries;
	
	RPointerArray<CMsvCacheEntry> oEntries; 
	CMsvCacheEntry* nonimmediateParent = NULL;
	TRAP(err, nonimmediateParent = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	nonimmediateParent->Entry().SetId(++id);
	TMsvId parentId;
	
	
	// t1. block is NULL, perform GetChildren
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	TRAP(err, iIndexTableEntry->GetChildrenL(id + 200, oEntries)); //non-existent parent
	MY_ASSERT_EQUALS(err, KErrNone);
	timeAfter = iIndexTableEntry->AccessTime();
	MY_ASSERT_TRUE(timeAfter != timeBefore);
	
	
	// t2. when grandchildren are present in the block.
	TRAP(err, parent = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	parent->Entry().SetId(++id);
	++id;
	TRAP(err, iIndexTableEntry->AddEntryL(parent));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, entryA = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entryA->Entry().SetId(++id);
	++id;
	entryA->Entry().SetParent(parent->GetId());
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_EQUALS(err, KErrNone);
			
	for(TInt index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id + 1);
		TRAP(err, entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		MY_ASSERT_EQUALS(KErrNone, err);
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		entries[index]->Entry().SetParent(parent->GetId());
		TRAP(err, iEntryPointer->AppendL(entries[index]));
		MY_ASSERT_EQUALS(KErrNone, err);
		}
	TRAP(err, iIndexTableEntry->AddEntrySetL(*iEntryPointer, 0, iEntryPointer->Count())); //immediate children
	MY_ASSERT_EQUALS(err, KErrNone);
	size = iIndexTableEntry->Size();
	
	TRAP(err, entryB = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	entryB->Entry().SetId(++id);
	++id;
	entryB->Entry().SetParent(nonimmediateParent->GetId());
	TRAP(err, iIndexTableEntry->AddEntryL(entryB)); //B is non-immediate child
	MY_ASSERT_EQUALS(err, KErrNone);
	iIndexTableEntry->SetGrandChildPresent();
	size = iIndexTableEntry->Size();
	
//	entries.Reset(); // add non-immediate children
//	tmsvEntries.Reset();
//	iEntryPointer->Reset();
//	for(TInt index = 0 ; index < bulkCount; ++index, ++id)
//		{
//		tmsvEntries.Append(TMsvEntry());
//		tmsvEntries[index].SetId(id);
//		entries.Append(CMsvEntryFreePool::Instance()->EntryL());
//		entries[index]->Entry().SetId(tmsvEntries[index].Id());
//		entries[index]->Entry().SetParent(parentEntry2->Id());
//		iEntryPointer->AppendL(entries[index]);
//		}
//	TRAP(err, iIndexTableEntry->AddEntrySetL(*iEntryPointer, 0)); //add some non-immediate children
//	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, iIndexTableEntry->GetChildrenL(parent->GetId(), oEntries));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(oEntries.Count(), 11); //entryA + 10 others
	
	oEntries.Reset();
	TRAP(err, iIndexTableEntry->GetChildrenL(nonimmediateParent->GetId(), oEntries));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(oEntries.Count(), 1); //entryB
	
	
	// t3. when grandchildren are not present in the block.
	entryB->LockEntry();
	TRAP(err, iIndexTableEntry->DeleteEntryL(entryB->GetId(), parentId));
	MY_ASSERT_EQUALS(err, KErrNone);
	oEntries.Reset();
	TRAP_IGNORE(iIndexTableEntry->GetChildrenL(parent->GetId(), oEntries));
	MY_ASSERT_EQUALS(oEntries.Count(), 11); //entryA + 10 others
	CMsvEntryFreePool::Instance()->ReleaseEntryL(nonimmediateParent);
	
	
	// t4. only grandchildren are present in the block
	iIndexTableEntry->ReleaseBlock(ETrue);
	entries.Reset(); 
	tmsvEntries.Reset();
	iEntryPointer->Reset();
	
	TRAP(err, nonimmediateParent = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	nonimmediateParent->Entry().SetId(++id);
	++id;
	
	TRAP(err, parent = CMsvEntryFreePool::Instance()->EntryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	parent->Entry().SetId(++id);
	++id;
	
	for(TInt index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id);
		TRAP_IGNORE(entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		entries[index]->Entry().SetParent(nonimmediateParent->GetId());
		TRAP_IGNORE(iEntryPointer->AppendL(entries[index]));
		}
	TRAP(err, iIndexTableEntry->AddEntrySetL(*iEntryPointer, 0, iEntryPointer->Count())); //add some non-immediate children
	MY_ASSERT_EQUALS(err, KErrNone);
	
	oEntries.Reset();
	TRAP(err, iIndexTableEntry->GetChildrenL(nonimmediateParent->GetId(), oEntries));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(oEntries.Count(), 10);
	
	oEntries.Reset();
	TRAP(err, iIndexTableEntry->GetChildrenL(parent->GetId(), oEntries));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(oEntries.Count(), 0);
	
	CMsvEntryFreePool::Instance()->ReleaseEntryL(nonimmediateParent);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(parent);
	oEntries.Close();
	tmsvEntries.Close();
	entries.Close();	
	}


void CTestIndexTableEntry::TestDeleteEntry()
	{
	_LIT(KFunction, "TestDeleteEntry");
	INFO_PRINTF1(KFunction);
	TInt err;
	
	TMsvId parentId;
	TMsvId msvId = KFirstFreeEntryId;
	TMsvId id = msvId + 50;
	TMsvId temp;
	TTime timeBefore, timeAfter;
	TInt bulkCount = 100;
	
	// t1. delete entry from NULL block.
	TRAP(err, iIndexTableEntry->DeleteEntryL(id, parentId));
	MY_ASSERT_EQUALS(err, KErrNotFound);
		
	
	// t2. remove unlocked existing entry
	TRAP_IGNORE(parent = CMsvEntryFreePool::Instance()->EntryL());
	parent->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	entryA->Entry().SetParent(parent->GetId());
	++id;
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP_IGNORE(entryB = CMsvEntryFreePool::Instance()->EntryL());
	entryB->Entry().SetId(++id);
	entryB->Entry().SetParent(parent->GetId());
	++id;
	TRAP(err, iIndexTableEntry->AddEntryL(entryB));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	TRAP(err, iIndexTableEntry->DeleteEntryL(entryA->GetId(), parentId));
	MY_ASSERT_EQUALS(err, KErrAccessDenied);
	MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entryA->GetId()));
	timeAfter = iIndexTableEntry->AccessTime();
 	MY_ASSERT_TRUE(timeAfter == timeBefore);
 		
	
	// t3. remove locked existing entry
	entryA->LockEntry();
	timeBefore = iIndexTableEntry->AccessTime();
	User::After(1000);
	temp = entryA->GetId();
 	TRAP(err, iIndexTableEntry->DeleteEntryL(entryA->GetId(), parentId));
 	MY_ASSERT_EQUALS(err, KErrNone);
 	MY_ASSERT_FALSE(iIndexTableEntry->EntryExists(temp)); 
 	MY_ASSERT_EQUALS(parentId, parent->GetId());
 	timeAfter = iIndexTableEntry->AccessTime();
 	MY_ASSERT_TRUE(timeAfter != timeBefore);
 	
 	
 	// t4. remove last entry in the block
 	entryB->LockEntry();
 	timeBefore = iIndexTableEntry->AccessTime();
 	User::After(1000);
 	temp = entryB->GetId();
 	TRAP(err, iIndexTableEntry->DeleteEntryL(entryB->GetId(), parentId));
 	MY_ASSERT_EQUALS(err, KErrNone);
 	MY_ASSERT_FALSE(iIndexTableEntry->EntryExists(temp)); 
 	MY_ASSERT_TRUE(iIndexTableEntry->BlockPtr() == NULL);
 	MY_ASSERT_EQUALS(parentId, parent->GetId());
 	timeAfter = iIndexTableEntry->AccessTime();
 	MY_ASSERT_TRUE(timeAfter != timeBefore);
 	
 	
 	// t5. remove non-existing entries
 	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	entryA->Entry().SetParent(parent->GetId());
	++id;
	TRAP_IGNORE(entryB = CMsvEntryFreePool::Instance()->EntryL());
	entryB->Entry().SetId(++id);
	entryB->Entry().SetParent(parent->GetId());
	++id;
 	TRAP(err, iIndexTableEntry->DeleteEntryL(entryA->GetId(), parentId));
 	MY_ASSERT_EQUALS(err, KErrNotFound);
 	
 	entryA->LockEntry();
 	TRAP(err, iIndexTableEntry->DeleteEntryL(entryA->GetId(), parentId));
 	MY_ASSERT_EQUALS(err, KErrNotFound);
 	
 	CMsvEntryFreePool::Instance()->ReleaseEntryL(parent);
 	CMsvEntryFreePool::Instance()->ReleaseEntryL(entryA);
 	CMsvEntryFreePool::Instance()->ReleaseEntryL(entryB);
 	
 	
 	// t6. bulk additions and deletions
 	TRAP_IGNORE(parent = CMsvEntryFreePool::Instance()->EntryL());
	parent->Entry().SetId(++id);
	++id;
 	
 	RArray<CMsvCacheEntry*> entries;
	RArray<TMsvEntry> tmsvEntries;
 	
 	for(TInt index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id);
		TRAP_IGNORE(entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		entries[index]->Entry().SetParent(parent->GetId());
		TRAP_IGNORE(iEntryPointer->AppendL(entries[index]));
		}
	TRAP(err, iIndexTableEntry->AddEntrySetL(*iEntryPointer, 0, iEntryPointer->Count()));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	for(TInt index = 0; index < bulkCount; ++index)
		{
		entries[index]->LockEntry();
		timeBefore = iIndexTableEntry->AccessTime();
		User::After(1000);
		temp = entries[index]->GetId();
		TRAP(err, iIndexTableEntry->DeleteEntryL(entries[index]->GetId(), parentId));
		ASSERT_EQUALS(err, KErrNone);
		ASSERT_FALSE(iIndexTableEntry->EntryExists(temp));
		ASSERT_EQUALS(parentId, parent->GetId());
		timeAfter = iIndexTableEntry->AccessTime();
 		ASSERT_TRUE(timeAfter != timeBefore);
		}
	MY_ASSERT_TRUE(iIndexTableEntry->BlockPtr() == NULL);
	
	CMsvEntryFreePool::Instance()->ReleaseEntryL(parent);
	tmsvEntries.Close();
	entries.Close();
	}


void CTestIndexTableEntry::TestSortBlock()
	{
	_LIT(KFunction, "TestSortBlock");
	INFO_PRINTF1(KFunction);
	
	TInt err;
	TMsvId id = KFirstFreeEntryId;
	RPointerArray<CMsvCacheEntry> oEntries;
	
	TRAP_IGNORE(parent = CMsvEntryFreePool::Instance()->EntryL());
	parent->Entry().SetId(++id);
	++id;
	
	/* Basic idea:
	 *  - append 3 entries with unsorted TMsvIds (descending order of Ids is used here)
	 *  - call SortBlock
	 *  - now when we pick any 2 successive (need not be adjacent) entries,
	 * 	  if the difference between their TMsvIds is -ve then block has been sorted
	 */
	TRAP_IGNORE(entryC = CMsvEntryFreePool::Instance()->EntryL());
	entryC->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryB = CMsvEntryFreePool::Instance()->EntryL());
	entryB->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	++id;
	
	entryC->Entry().SetParent(parent->GetId());
	TRAP(err, iIndexTableEntry->AddEntryL(entryC));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	entryB->Entry().SetParent(parent->GetId());
	TRAP(err, iIndexTableEntry->AddEntryL(entryB));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	entryA->Entry().SetParent(parent->GetId());
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	iIndexTableEntry->SortBlock();
	//atleast 3 entries have been added, so test with the first three entries.
	TRAP(err, iIndexTableEntry->GetChildrenL(parent->GetId(), oEntries));
	MY_ASSERT_EQUALS(err, KErrNone);
	err = oEntries[0]->GetId() - oEntries[2]->GetId(); //successive entries
	MY_ASSERT_TRUE(err < 0);
	err = oEntries[1]->GetId() - oEntries[2]->GetId(); //successive entries
	MY_ASSERT_TRUE(err < 0);
	err = oEntries[0]->GetId() - oEntries[1]->GetId(); //successive entries
	MY_ASSERT_TRUE(err < 0);
	
	CMsvEntryFreePool::Instance()->ReleaseEntryL(parent);
	oEntries.Close();
	}


void CTestIndexTableEntry::TestReleaseBlock()
	{
	_LIT(KFunction, "TestReleaseBlock");
	INFO_PRINTF1(KFunction);
	
	TInt err;
	TBool ret;
	TInt size;
	TMsvId msvId = KFirstFreeEntryId;
	TMsvId id = msvId + 50;
	
	// t1. release NULL block
	ret = iIndexTableEntry->ReleaseBlock();
	MY_ASSERT_TRUE(ret);
	ret = iIndexTableEntry->ReleaseBlock(ETrue);
	MY_ASSERT_TRUE(ret);
	
	
	// t2. release block with all entries locked
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryB = CMsvEntryFreePool::Instance()->EntryL());
	entryB->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryC = CMsvEntryFreePool::Instance()->EntryL());
	entryC->Entry().SetId(++id);
	++id;
	entryA->LockEntry();
	entryB->LockEntry();
	entryC->LockEntry();
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iIndexTableEntry->AddEntryL(entryB));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iIndexTableEntry->AddEntryL(entryC));
	MY_ASSERT_EQUALS(err, KErrNone);
	size = iIndexTableEntry->Size();
	ret = iIndexTableEntry->ReleaseBlock(EFalse);
	MY_ASSERT_FALSE(ret);
	MY_ASSERT_EQUALS(size, iIndexTableEntry->Size());
	
	// t3. release block with only 1 entry unswappable
	entryB->ReleaseEntry();
	entryC->ReleaseEntry();
	entryA->IncStoreReaderCount();
	size = iIndexTableEntry->Size();
	ret = iIndexTableEntry->ReleaseBlock(EFalse);
	MY_ASSERT_FALSE(ret);
	MY_ASSERT_EQUALS(size, iIndexTableEntry->Size());
	
	
	// t4. release block with no entries unswappable
	entryA->ReleaseEntry();
	entryA->DecStoreReaderCount();
	ret = iIndexTableEntry->ReleaseBlock(EFalse);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_TRUE(iIndexTableEntry->BlockPtr() == NULL);
	MY_ASSERT_TRUE(iIndexTableEntry->IsDirty());
	
	
	// t5. forcibly release a block even if entries are unswappable
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryB = CMsvEntryFreePool::Instance()->EntryL());
	entryB->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryC = CMsvEntryFreePool::Instance()->EntryL());
	entryC->Entry().SetId(++id);
	++id;
	entryA->LockEntry();
	entryB->LockEntry();
	entryC->LockEntry();
	entryA->IncStoreReaderCount();
	entryC->IncStoreReaderCount();
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iIndexTableEntry->AddEntryL(entryB));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iIndexTableEntry->AddEntryL(entryC));
	MY_ASSERT_EQUALS(err, KErrNone);
	ret = iIndexTableEntry->ReleaseBlock(ETrue);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_TRUE(iIndexTableEntry->BlockPtr() == NULL);
	
	
	// t6. forcibly release block when no entries are unswappable
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryB = CMsvEntryFreePool::Instance()->EntryL());
	entryB->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryC = CMsvEntryFreePool::Instance()->EntryL());
	entryC->Entry().SetId(++id);
	++id;
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iIndexTableEntry->AddEntryL(entryB));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iIndexTableEntry->AddEntryL(entryC));
	MY_ASSERT_EQUALS(err, KErrNone);
	ret = iIndexTableEntry->ReleaseBlock(ETrue);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_TRUE(iIndexTableEntry->BlockPtr() ==  NULL);
	
	}


void CTestIndexTableEntry::TestIsInRange()
	{
	_LIT(KFunction, "TestIsInRange");
	INFO_PRINTF1(KFunction);
	
	TMsvId id = KFirstFreeEntryId;
	
	//find max and min MsvIds.
	//test should fail for TMsvIds outside the boundaries. (the first 2 tests)
	//test should pass otherwise.
			
	iIndexTableEntry->SetMinMsvIdRange(id + 10);
	iIndexTableEntry->SetMaxMsvIdRange(id + 50);
			
	TBool ret;
	TMsvId maxId = iIndexTableEntry->GetMaxMsvIdRange();
	TMsvId minId = iIndexTableEntry->GetMinMsvIdRange();
	
	ret = iIndexTableEntry->IsInRange((TMsvId)(maxId + 1));
	MY_ASSERT_FALSE(ret);
	ret = iIndexTableEntry->IsInRange((TMsvId)(minId - 1));
	MY_ASSERT_FALSE(ret);
	ret = iIndexTableEntry->IsInRange((TMsvId)(maxId - 1));
	MY_ASSERT_TRUE(ret);	
	ret = iIndexTableEntry->IsInRange((TMsvId)(minId + 1));
	MY_ASSERT_TRUE(ret);
	}


void CTestIndexTableEntry::TestEntryExists()
	{
	_LIT(KFunction, "TestEntryExists");
	INFO_PRINTF1(KFunction);
	TInt err;
	TMsvId parentId;
	TMsvId id = KFirstFreeEntryId;
	TInt bulkCount = 100;
	
	TRAP_IGNORE(parent = CMsvEntryFreePool::Instance()->EntryL());
	parent->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	++id;
	
	
	// t1. no entries in the cache
	MY_ASSERT_FALSE(iIndexTableEntry->EntryExists(entryA->GetId()));
	MY_ASSERT_TRUE(iIndexTableEntry->BlockPtr() == NULL);
	
	
	// t2. delete added entry and check
	entryA->Entry().SetParent(parent->GetId());
	TRAP(err, iIndexTableEntry->AddEntryL(parent));
	MY_ASSERT_TRUE(err == KErrNone);
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_TRUE(err == KErrNone);
	MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entryA->GetId()));
		//cannot delete entry not locked in cache
	TRAP(err, iIndexTableEntry->DeleteEntryL(entryA->GetId(), parentId));
	MY_ASSERT_EQUALS(err, KErrAccessDenied);
	MY_ASSERT_TRUE(iIndexTableEntry->EntryExists(entryA->GetId()));
	entryA->LockEntry(); //can only delete entry locked in cache
	TMsvId entryAId = entryA->GetId();
	TRAP(err, iIndexTableEntry->DeleteEntryL(entryAId, parentId));
	MY_ASSERT_TRUE(err == KErrNone);
	MY_ASSERT_FALSE(iIndexTableEntry->EntryExists(entryAId));
	MY_ASSERT_EQUALS(parentId, parent->GetId());
	
	
	// t3. bulk operations
	RArray<CMsvCacheEntry*> entries;
	RArray<TMsvEntry> tmsvEntries;
	for(TInt index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id);
		TRAP_IGNORE(entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		TRAP_IGNORE(iEntryPointer->AppendL(entries[index]));
		}
	TRAP(err, iIndexTableEntry->AddEntrySetL(*iEntryPointer, 0, iEntryPointer->Count()));
	MY_ASSERT_EQUALS(err, KErrNone);
	for(TInt index = 0; index < bulkCount; ++index)
		{
		ASSERT_TRUE(iIndexTableEntry->EntryExists(entries[index]->GetId()));
		}
		
	tmsvEntries.Reset();
	++id;
	for(TInt index = 0; index < bulkCount; ++index, ++id) 
		{
		tmsvEntries.Append(TMsvEntry());  //non-existent entries
		tmsvEntries[index].SetId(id);
		ASSERT_FALSE(iIndexTableEntry->EntryExists(tmsvEntries[index].Id()));
		}
 	entries.Close();
 	tmsvEntries.Close();
 	}


void CTestIndexTableEntry::TestIsDirty()
	{
	_LIT(KFunction, "TestIsDirty");
	INFO_PRINTF1(KFunction);
	TBool err;
	TInt bulkCount = 100;
	TMsvId id = KFirstFreeEntryId;
	
		
	// t1. test using setting and resetting of the dirty flag
	iIndexTableEntry->SetDirty();
	err = iIndexTableEntry->IsDirty();
	MY_ASSERT_TRUE(err);
	
	iIndexTableEntry->ClearDirty();
	err = iIndexTableEntry->IsDirty();
	MY_ASSERT_FALSE(err);
	
	
	// t2. test using ClearFlags
	iIndexTableEntry->ClearFlags();
	err = iIndexTableEntry->IsDirty();
	MY_ASSERT_FALSE(err);
	
	
	// t3. releasing the block sets dirty flag
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	++id;
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_EQUALS(err, KErrNone);
	err = iIndexTableEntry->ReleaseBlock(ETrue);
	MY_ASSERT_TRUE(err);
	MY_ASSERT_TRUE(iIndexTableEntry->IsDirty());
	}


void CTestIndexTableEntry::TestSetDirty()
	{
	_LIT(KFunction, "TestSetDirty");
	INFO_PRINTF1(KFunction);
	TInt err;
	TInt bulkCount = 100;
	TMsvId id = KFirstFreeEntryId;
	
	// t1. test using setting and resetting of dirty flag
	iIndexTableEntry->SetDirty();
	TBool ret = iIndexTableEntry->IsDirty();
	MY_ASSERT_TRUE(ret);
	
	iIndexTableEntry->ClearFlags();
	ret = iIndexTableEntry->IsDirty();
	MY_ASSERT_FALSE(ret);
	
	iIndexTableEntry->ClearDirty();
	ret = iIndexTableEntry->IsDirty();
	MY_ASSERT_FALSE(ret);
	
	iIndexTableEntry->SetDirty();
	ret = iIndexTableEntry->IsDirty();
	MY_ASSERT_TRUE(ret);
	iIndexTableEntry->ClearDirty();
	
	
	// t2. Releasing a block to free pool causes setting of dirty bit 
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	++id;
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_EQUALS(err, KErrNone);
	err = iIndexTableEntry->ReleaseBlock(ETrue);
	MY_ASSERT_TRUE(err);
	MY_ASSERT_TRUE(iIndexTableEntry->IsDirty());
	
	}

void CTestIndexTableEntry::TestClearDirty()
	{
	_LIT(KFunction, "TestClearDirty");
	INFO_PRINTF1(KFunction);
	TMsvId id = KFirstFreeEntryId;
	
	// t1. test using setting and resetting of dirty flag
	TInt err;
	iIndexTableEntry->ClearDirty();
	err = iIndexTableEntry->IsDirty();
	MY_ASSERT_FALSE(err);
	
	iIndexTableEntry->SetDirty();
	err = iIndexTableEntry->IsDirty();
	MY_ASSERT_TRUE(err);
	
	iIndexTableEntry->ClearDirty();
	err = iIndexTableEntry->IsDirty();
	MY_ASSERT_FALSE(err);
	
	}

void CTestIndexTableEntry::TestGetMaxMsvIdRange()
	{
	_LIT(KFunction, "TestGetMaxMsvIdRange");
	INFO_PRINTF1(KFunction);
	TInt err;
	TMsvId id = KFirstFreeEntryId;
	
	// t1. test by setting manually
	iIndexTableEntry->SetMaxMsvIdRange(id);
	MY_ASSERT_EQUALS(iIndexTableEntry->GetMaxMsvIdRange(), id);
	
	
	// t2. AddEntryL to a NULL block
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	++id;
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_TRUE(err == KErrNone);
	MY_ASSERT_EQUALS(iIndexTableEntry->GetMaxMsvIdRange(), entryA->GetId());
	
	
	// t3. AddEntryL to last block (assuming this block is the last one)
	TRAP_IGNORE(entryB = CMsvEntryFreePool::Instance()->EntryL());
	entryB->Entry().SetId(++id);
	++id;
	TRAP(err, iIndexTableEntry->AddEntryL(entryB));
	MY_ASSERT_TRUE(err == KErrNone);
	MY_ASSERT_EQUALS(iIndexTableEntry->GetMaxMsvIdRange(), entryB->GetId());
	
	}


void CTestIndexTableEntry::TestGetMinMsvIdRange()
	{
	_LIT(KFunction, "TestGetMinMsvIdRange");
	INFO_PRINTF1(KFunction);
	TInt err;
	TMsvId id = KFirstFreeEntryId;
	
	// t1. test by setting manually
	iIndexTableEntry->SetMinMsvIdRange(id + 50);
	MY_ASSERT_EQUALS(iIndexTableEntry->GetMinMsvIdRange(), id + 50);
	
	
	// t2. AddEntryL to a NULL block
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(id + 20);
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_TRUE(err == KErrNone);
	MY_ASSERT_EQUALS(iIndexTableEntry->GetMinMsvIdRange(), entryA->GetId());
	
	
	// t3. AddEntryL to folder with no indextable created
	TRAP_IGNORE(entryB = CMsvEntryFreePool::Instance()->EntryL());
	entryB->Entry().SetId(id + 10);
	TRAP(err, iIndexTableEntry->AddEntryL(entryB));
	MY_ASSERT_TRUE(err == KErrNone);
	MY_ASSERT_EQUALS(iIndexTableEntry->GetMinMsvIdRange(), entryB->GetId());
	
	}

void CTestIndexTableEntry::TestSetMaxMsvIdRange()
	{
	_LIT(KFunction, "TestSetMaxMsvIdRange");
	INFO_PRINTF1(KFunction);
	TInt err;
	TInt id = KFirstFreeEntryId + 10;
	
	// t1. testing by setting iMaxMsvId and getting it.
	TMsvId aId= id - 5;
	iIndexTableEntry->SetMaxMsvIdRange(aId);
	MY_ASSERT_EQUALS(iIndexTableEntry->GetMaxMsvIdRange(), aId);
		
	
	// t2. AddEntryL on a NULL block results in setting of iMaxMsvId
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	++id;
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(iIndexTableEntry->GetMaxMsvIdRange(), entryA->GetId());
	
	}


void CTestIndexTableEntry::TestSetMinMsvIdRange()
	{
	_LIT(KFunction, "TestSetMinMsvIdRange");
	INFO_PRINTF1(KFunction);
	TInt err;
	TInt id = KFirstFreeEntryId;
	
	// t1. testing by setting iMinMsvId and getting it.
	TMsvId aId= id + 10;
	iIndexTableEntry->SetMinMsvIdRange(aId);
	MY_ASSERT_EQUALS(iIndexTableEntry->GetMinMsvIdRange(), aId);
	
		
	// t2. AddEntryL on a NULL block results in setting of iMaxMsvId
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(id + 8);
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(iIndexTableEntry->GetMinMsvIdRange(), entryA->GetId());
	
	}


void CTestIndexTableEntry::TestSize()
	{
	_LIT(KFunction, "TestSize");
	INFO_PRINTF1(KFunction);
	TInt err;
	TInt bulkCount = 50;
	TMsvId id = KFirstFreeEntryId;
	
	// t1. perform Size() on NULL block
	TInt ret = iIndexTableEntry->Size();
	MY_ASSERT_TRUE(ret == 0); 
	
	
	// t2. add a single entry
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	++id;
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_EQUALS(err, KErrNone);
	ret = iIndexTableEntry->Size();
	MY_ASSERT_TRUE(ret == 1);
			
	
	// t3. add large number of entries and check
	RArray<CMsvCacheEntry*> entries;
	RArray<TMsvEntry> tmsvEntries;
	for(TInt index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id);
		TRAP_IGNORE(entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		TRAP_IGNORE(iEntryPointer->AppendL(entries[index]));
		}
	TRAP(err, iIndexTableEntry->AddEntrySetL(*iEntryPointer, 0, iEntryPointer->Count()));
	MY_ASSERT_EQUALS(err, KErrNone);
	ret = iIndexTableEntry->Size();
	MY_ASSERT_EQUALS(ret, bulkCount + 1);
	entries.Close();
	tmsvEntries.Close();
	}

void CTestIndexTableEntry::TestUpdateChildMsvIds()
	{
	_LIT(KFunction, "TestUpdateChildMsvIds");
	INFO_PRINTF1(KFunction);
	TInt err;
	TInt bulkCount = 100;
	TMsvId id = KFirstFreeEntryId;
	RArray<TMsvId> *childArray;
	
		
	// t1. Updating iChildIdArray with many child entries.
	TBool flag = ETrue;
	TInt index = 0;
	TRAP_IGNORE(parent = CMsvEntryFreePool::Instance()->EntryL());
	parent->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryB = CMsvEntryFreePool::Instance()->EntryL());
	entryB->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryC = CMsvEntryFreePool::Instance()->EntryL());
	entryC->Entry().SetId(++id);
	++id;
	entryA->Entry().SetParent(parent->GetId());
	entryB->Entry().SetParent(parent->GetId());
	entryC->Entry().SetParent(parent->GetId());
	TRAP_IGNORE(iEntryPointer->AppendL(entryA));
	TRAP_IGNORE(iEntryPointer->AppendL(entryB));
	
	// when iBlockPtr is NULL. ------------------
	MY_ASSERT_TRUE(iIndexTableEntry->Size() == 0);
	
	// when iBlockPtr is not NULL. --------------
	// ---update on parent on whom GetChildren has not been performed
	TRAP(err, iIndexTableEntry->AddEntryL(parent));
	MY_ASSERT_TRUE(err == KErrNone);
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_TRUE(err == KErrNone);
	TRAP(err, iIndexTableEntry->AddEntryL(entryB));
	MY_ASSERT_TRUE(err == KErrNone);
	TRAP(err, iIndexTableEntry->UpdateChildMsvIdsL(*iEntryPointer));
	MY_ASSERT_TRUE(err == KErrNone);
	childArray = parent->ChildIdArray();
	MY_ASSERT_EQUALS(childArray->Count(), 2); // entryA and entryB
	// ---update on parent on whom GetChildren has been performed => array is reset and assigned
	TRAP_IGNORE(iEntryPointer->AppendL(entryC));
	TRAP(err, iIndexTableEntry->AddEntryL(entryC));
	MY_ASSERT_TRUE(err == KErrNone);
	TRAP(err, iIndexTableEntry->UpdateChildMsvIdsL(*iEntryPointer));
	MY_ASSERT_TRUE(err == KErrNone);
	childArray = parent->ChildIdArray();
	MY_ASSERT_EQUALS(childArray->Count(), 3); // entryA, entryB and now entryC		
	
	// t2. Updating iChildIdArray of a parent with a single child entry.
	// when iBlockPtr is NULL. ------------------
	iIndexTableEntry->ReleaseBlock(ETrue);
	MY_ASSERT_TRUE(iIndexTableEntry->Size() == NULL);
	
	// when iBlockPtr is not NULL. --------------
	// ---iChildIdArray != NULL, and addition is being performed
	TRAP_IGNORE(parent = CMsvEntryFreePool::Instance()->EntryL());
	parent->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryB = CMsvEntryFreePool::Instance()->EntryL());
	entryB->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(entryC = CMsvEntryFreePool::Instance()->EntryL());
	entryC->Entry().SetId(++id);
	++id;
	entryA->Entry().SetParent(parent->GetId());
	entryB->Entry().SetParent(parent->GetId());
	entryC->Entry().SetParent(parent->GetId());
	TRAP(err, iIndexTableEntry->AddEntryL(parent));
	MY_ASSERT_TRUE(err == KErrNone);
	TRAP(err, iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_TRUE(err == KErrNone);
	TRAP(err, iIndexTableEntry->AddEntryL(entryB));
	MY_ASSERT_TRUE(err == KErrNone);
	TRAP(err, iIndexTableEntry->AddEntryL(entryC));
	MY_ASSERT_TRUE(err == KErrNone);
	
	RArray<TMsvId> *childArray1 = NULL;
	TRAP_IGNORE(childArray1 = new(ELeave) RArray<TMsvId>);
	TRAP_IGNORE(childArray1->AppendL(entryA->GetId()));
	TRAP_IGNORE(childArray1->AppendL(entryB->GetId()));
	TRAP_IGNORE(childArray1->AppendL(entryC->GetId()));
	parent->SetChildIdArray(childArray1);
	TRAP_IGNORE(entryD = CMsvEntryFreePool::Instance()->EntryL()); // new child
	entryD->Entry().SetId(++id);
	++id;
	entryD->Entry().SetParent(parent->GetId());
	TRAP(err, iIndexTableEntry->AddEntryL(entryD));
	MY_ASSERT_TRUE(err == KErrNone);
	TRAP(err, iIndexTableEntry->UpdateChildMsvIdsL(parent->GetId(), entryD->GetId())); //addition
	MY_ASSERT_TRUE(err == KErrNone);	
	childArray1 = parent->ChildIdArray();
	MY_ASSERT_EQUALS(childArray1->Count(), 4); // A, B, C and now D
	
	// ---deletion of child entry from iChildIdArray
	TRAP(err, iIndexTableEntry->UpdateChildMsvIdsL(parent->GetId(), entryD->GetId(), EFalse));
	MY_ASSERT_TRUE(err == KErrNone);
	childArray1 = parent->ChildIdArray();
	MY_ASSERT_EQUALS(childArray1->Count(), 3); // A, B and C. D has now been deleted
	
	// ---delete all children from child array
	TRAP(err, iIndexTableEntry->UpdateChildMsvIdsL(parent->GetId(), entryA->GetId(), EFalse));
	MY_ASSERT_TRUE(err == KErrNone);
	TRAP(err, iIndexTableEntry->UpdateChildMsvIdsL(parent->GetId(), entryB->GetId(), EFalse));
	MY_ASSERT_TRUE(err == KErrNone);
	TRAP(err, iIndexTableEntry->UpdateChildMsvIdsL(parent->GetId(), entryC->GetId(), EFalse));
	MY_ASSERT_TRUE(err == KErrNone);
	childArray1 = parent->ChildIdArray();
	MY_ASSERT_EQUALS(childArray1->Count(), 0); // all children have been deleted	
	}

void CTestIndexTableEntry::TestSetGrandChildPresent()
	{
	_LIT(KFunction, "TestSetGrandChildPresent");
	INFO_PRINTF1(KFunction);
	TMsvId id = KFirstFreeEntryId;
	
	// t1. test by setting and clearing of the flag(s)
	iIndexTableEntry->SetGrandChildPresent();
	MY_ASSERT_TRUE(iIndexTableEntry->IsGrandChildPresent());
	iIndexTableEntry->ClearFlags();
	MY_ASSERT_FALSE(iIndexTableEntry->IsGrandChildPresent());	
	iIndexTableEntry->SetGrandChildPresent();
	MY_ASSERT_TRUE(iIndexTableEntry->IsGrandChildPresent());
	iIndexTableEntry->ClearGrandChildPresent();
	MY_ASSERT_FALSE(iIndexTableEntry->IsGrandChildPresent());	
	
	}

void CTestIndexTableEntry::TestClearGrandChildPresent()
	{
	_LIT(KFunction, "TestClearGrandChildPresent");
	INFO_PRINTF1(KFunction);
	TMsvId id = KFirstFreeEntryId;
	
	// t1. test by setting and clearing of the flag(s)
	iIndexTableEntry->SetGrandChildPresent();
	MY_ASSERT_TRUE(iIndexTableEntry->IsGrandChildPresent());
	iIndexTableEntry->ClearGrandChildPresent();
	MY_ASSERT_FALSE(iIndexTableEntry->IsGrandChildPresent());		
	iIndexTableEntry->SetGrandChildPresent();
	MY_ASSERT_TRUE(iIndexTableEntry->IsGrandChildPresent());
	iIndexTableEntry->ClearFlags();
	MY_ASSERT_FALSE(iIndexTableEntry->IsGrandChildPresent());	
	
	}

void CTestIndexTableEntry::TestIsGrandChildPresent()
	{
	_LIT(KFunction, "TestIsGrandChildPresent");
	INFO_PRINTF1(KFunction);
	TMsvId id = KFirstFreeEntryId;
	
	// t1. test by setting and clearing of the flag(s)
	iIndexTableEntry->SetGrandChildPresent();
	MY_ASSERT_TRUE(iIndexTableEntry->IsGrandChildPresent());
	iIndexTableEntry->ClearGrandChildPresent();
	MY_ASSERT_FALSE(iIndexTableEntry->IsGrandChildPresent());		
	iIndexTableEntry->SetGrandChildPresent();
	MY_ASSERT_TRUE(iIndexTableEntry->IsGrandChildPresent());
	iIndexTableEntry->ClearFlags();
	MY_ASSERT_FALSE(iIndexTableEntry->IsGrandChildPresent());	
	
	}

void CTestIndexTableEntry::TestBlockPtr()
	{
	_LIT(KFunction, "TestBlockPtr");
	INFO_PRINTF1(KFunction);
	TInt err;
	TMsvId id = KFirstFreeEntryId, parentId;
	
	
	// t1. call the function when block is NULL. it should return NULL
	MY_ASSERT_TRUE(iIndexTableEntry->BlockPtr() == NULL);
	
	
	// t2. add something to the block, and attempt to get the iBlockPtr.
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_TRUE(iIndexTableEntry->iBlockPtr == iIndexTableEntry->BlockPtr());
	
	
	// t3. release block makes iBlockPtr NULL
	err = iIndexTableEntry->ReleaseBlock(ETrue);
	MY_ASSERT_TRUE(err);
	MY_ASSERT_TRUE(NULL == iIndexTableEntry->BlockPtr());
	
	
	// t4. deleting last entry in the block
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(iIndexTableEntry->AddEntryL(entryA));
	entryA->LockEntry();
	TRAP_IGNORE(iIndexTableEntry->DeleteEntryL(entryA->GetId(), parentId));
	MY_ASSERT_TRUE(NULL == iIndexTableEntry->BlockPtr());
	
	}

void CTestIndexTableEntry::TestAccessTime()
	{ 
	_LIT(KFunction, "AccessTime");
	INFO_PRINTF1(KFunction);
	TInt err;
	TMsvId id = KFirstFreeEntryId, parentId;
	TInt bulkCount = 50;
	
	// t1. access time is changed on AddEntryL
	TTime tempTime = iIndexTableEntry->AccessTime();
	TRAP_IGNORE(entryA = CMsvEntryFreePool::Instance()->EntryL());
	entryA->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(iIndexTableEntry->AddEntryL(entryA));
	MY_ASSERT_TRUE( tempTime != iIndexTableEntry->AccessTime());
	
	
	// t2. access time is changed on AddEntrySetL
	tempTime = iIndexTableEntry->AccessTime();
	User::After(1000);
	TRAP_IGNORE(parent = CMsvEntryFreePool::Instance()->EntryL());
	parent->Entry().SetId(++id);
	++id;
	TRAP_IGNORE(iIndexTableEntry->AddEntryL(parent));
	RArray<CMsvCacheEntry*> entries;
	RArray<TMsvEntry> tmsvEntries;
	for(TInt index = 0 ; index < bulkCount; ++index, ++id)
		{
		tmsvEntries.Append(TMsvEntry());
		tmsvEntries[index].SetId(id);
		TRAP_IGNORE(entries.Append(CMsvEntryFreePool::Instance()->EntryL()));
		entries[index]->Entry().SetId(tmsvEntries[index].Id());
		entries[index]->Entry().SetParent(parent->GetId());
		TRAP_IGNORE(iEntryPointer->AppendL(entries[index]));
		}
	TRAP_IGNORE(iIndexTableEntry->AddEntrySetL(*iEntryPointer, 0, iEntryPointer->Count()));
	MY_ASSERT_TRUE( tempTime != iIndexTableEntry->AccessTime()); 
	entries.Close();
	tmsvEntries.Close();
	//*/
	
	// t3. access time is changed on GetEntryL hit
	tempTime = iIndexTableEntry->AccessTime();
	User::After(1000);
	CMsvCacheEntry *dummy;
	iIndexTableEntry->GetEntry(entryA->GetId(), dummy);
	MY_ASSERT_TRUE( tempTime != iIndexTableEntry->AccessTime());
	
	
	// t4. when entry is deleted
	tempTime = iIndexTableEntry->AccessTime();
	User::After(1000);
	entryA->LockEntry();
	TMsvId entryAId = entryA->GetId();
	TRAP(err, iIndexTableEntry->DeleteEntryL(entryA->GetId(), parentId));
	MY_ASSERT_TRUE(err == KErrNone);
	MY_ASSERT_FALSE(iIndexTableEntry->EntryExists(entryAId));
	MY_ASSERT_TRUE( tempTime != iIndexTableEntry->AccessTime());
	
	
	// t5. when GetChildren is performed
	iIndexTableEntry->ClearGrandChildPresent();
	iEntryPointer->Reset();
	tempTime = iIndexTableEntry->AccessTime();
	User::After(1000);
	TRAP_IGNORE(iIndexTableEntry->GetChildrenL(parent->GetId(), *iEntryPointer));
	MY_ASSERT_TRUE( tempTime != iIndexTableEntry->AccessTime());
	//*/
	}



#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
/*
void Print(CMsvCacheIndexTableEntry *aIndexTable)
	{
	RFileLogger logger;
	if (logger.Connect() == KErrNone)
		{
		logger.CreateLog(_L("msgs"), _L("Cache.txt"), EFileLoggingModeOverwrite);
		logger.Write(_L(""));
	
		aIndexTable->Print(logger);
		logger.CloseLog();
		}
	logger.Close();		
	}
*/



#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)


CTestSuite* CTestIndexTableEntry::CreateSuiteL(const TDesC& aName)
// static
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestConstructorL);
	ADD_ASYNC_TEST_STEP(TestAddEntry);
	ADD_ASYNC_TEST_STEP(TestAddEntrySet);
	ADD_ASYNC_TEST_STEP(TestGetEntry);
	ADD_ASYNC_TEST_STEP(TestGetChildren);
	ADD_ASYNC_TEST_STEP(TestDeleteEntry);
	ADD_ASYNC_TEST_STEP(TestSortBlock);
	ADD_ASYNC_TEST_STEP(TestReleaseBlock);
	ADD_ASYNC_TEST_STEP(TestIsInRange);
	ADD_ASYNC_TEST_STEP(TestEntryExists);
	ADD_ASYNC_TEST_STEP(TestIsDirty);
	ADD_ASYNC_TEST_STEP(TestSetDirty);
	ADD_ASYNC_TEST_STEP(TestClearDirty);
	ADD_ASYNC_TEST_STEP(TestSetMaxMsvIdRange);
	ADD_ASYNC_TEST_STEP(TestSetMinMsvIdRange);
	ADD_ASYNC_TEST_STEP(TestGetMaxMsvIdRange);
	ADD_ASYNC_TEST_STEP(TestGetMinMsvIdRange);
	ADD_ASYNC_TEST_STEP(TestUpdateChildMsvIds);
	ADD_ASYNC_TEST_STEP(TestSize);
	ADD_ASYNC_TEST_STEP(TestSetGrandChildPresent);
	ADD_ASYNC_TEST_STEP(TestClearGrandChildPresent);
	ADD_ASYNC_TEST_STEP(TestIsGrandChildPresent);
	ADD_ASYNC_TEST_STEP(TestBlockPtr);
	ADD_ASYNC_TEST_STEP(TestAccessTime);

	END_SUITE;
	}
