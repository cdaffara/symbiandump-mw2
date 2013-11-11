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
// T_MSGS_CACHE_VISIBLEFOLDER_STEP.CPP
// 
//

#include "t_msv_cache_visiblefolder_step.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
#include "msvindexadapter.h"
#include <flogger.h>
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


//Database used for testing
#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
_LIT(KTestDbFileName, "c:\\messaging.db");
#else
_LIT(KTestDbFileName, "c:[1000484B]testDB.db");
#endif
_LIT(KSTDMSGINI, "C:\\private\\1000484b\\msgcache.ini");
_LIT(KMSGINI_2, "C:\\private\\1000484b\\msgcache2.ini");


CTestVisibleFolder::CTestVisibleFolder()
	{
	}

CTestVisibleFolder::~CTestVisibleFolder()
	{

	}

void CTestVisibleFolder::SetupL()
	{
	_LIT(KFunction, "CreateFolderList");
	INFO_PRINTF1(KFunction);
	
	//__UHEAP_MARK;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TMsvPreferredDrive driveEntry;
	driveEntry.driveNum = EDriveC;
	driveEntry.driveId = 2;		// This is in sync with InitDb()
	driveEntry.status = EMsvMessageStoreAvailableStatus;
	
	TRAP_IGNORE(iDriveList = CMsvPreferredDriveList::CreateL());
	iDriveList->Insert(driveEntry, 0);
	iDriveList->SetCurrentDriveIndex(0);
#endif 
	
	TMsvId aId = 5;
	CMsvCacheVisibleFolder *folderA = NULL;
	TRAP_IGNORE(folderA= CMsvCacheVisibleFolder::NewL(aId));
	iFolderPointer.Append (folderA);

	aId = 30;
	CMsvCacheVisibleFolder *folderB = NULL;	
	TRAP_IGNORE(folderB= CMsvCacheVisibleFolder::NewL(aId));
	iFolderPointer.Append (folderB);
	
	//===============Init============================//
	TInt err =	iFileHandle.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);
	//err = iFileHandle.Replace(KMSGINI_2, KSTDMSGINI);
	CFileMan* file  = NULL;
	TRAP_IGNORE(file = CFileMan::NewL(iFileHandle));
	err = file->Copy(KMSGINI_2, KSTDMSGINI);
	MY_ASSERT_EQUALS(err, KErrNone);
	delete file;
	
	TDblQue<CMsvCacheVisibleFolder> iEntryCache;
	TRAP_IGNORE(iMsvEntryFreePool = CMsvEntryFreePool::CreateL(&iEntryCache));
	}

void CTestVisibleFolder::AddTestEntries()
	{
	TMsvId msvId = 05;
	
	//===============Init============================//
	aEntryA = Entry();
	aEntryB = Entry();
	aEntryC = Entry();
	
	aEntryA->Entry().SetParent((TMsvId)30);
	aEntryA->Entry().SetId (msvId);//TMsvId = 05
	iEntryPointer.Append (aEntryA);
	
	++msvId;//TMsvId = 06 is missing in the block for now
	
	aEntryB->Entry().SetParent((TMsvId)30);
	aEntryB->Entry().SetId (++msvId);//TMsvId = 07
	iEntryPointer.Append (aEntryB);
		
	++msvId;//TMsvId = 08 is missing in the block for now
	
	aEntryC->Entry().SetParent((TMsvId)30);
	aEntryC->Entry().SetId (++msvId);//TMsvId = 09
	iEntryPointer.Append (aEntryC);

	TRAP_IGNORE(iFolderPointer[1]->AddEntryListL(iEntryPointer, ETrue));
	//===============Func============================//
	}



void CTestVisibleFolder::TearDownL()
	{
	_LIT(KDeleteFolderList, "DeleteFolderList");
	INFO_PRINTF1(KDeleteFolderList);
	
	iEntryPointer.Reset();
	iFolderPointer.ResetAndDestroy();
	
	//iFileHandle.Replace(KSTDMSGINI, KMSGINI_2);
	iFileHandle.Close();

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	delete iDriveList;
#endif
	
#ifdef _DEBUG
	CMsvEntryFreePool::Destroy(iMsvEntryFreePool);
	iMsvEntryFreePool = NULL;
#else
	delete iMsvEntryFreePool;
	iMsvEntryFreePool = NULL;
#endif
	//__UHEAP_MARKEND;
	}

void CTestVisibleFolder::CreateEntry(TMsvId aId, TMsvId aParentId, TUid aUid, TMsvId& aVisibleFolder, TBool aIsVisible /* (Default = EFalse)*/)
	{
	TMsvEntry sampleTMsvEntry;
	
	sampleTMsvEntry.SetId(aId);
	sampleTMsvEntry.SetParent(aParentId);
	sampleTMsvEntry.iDate.UniversalTime();
	sampleTMsvEntry.iType = aUid;
	sampleTMsvEntry.SetVisible(aIsVisible);
	
	// Extra Information (Needed only in TestGetEntry()
	sampleTMsvEntry.SetUnread(ETrue);
	sampleTMsvEntry.iServiceId = aId+10;		// Some Random Id.
	sampleTMsvEntry.iRelatedId = aId+20;		// Some Random Id.
	sampleTMsvEntry.iMtm = aUid;		
	sampleTMsvEntry.iSize = 32;
	sampleTMsvEntry.iError = NULL;		
	sampleTMsvEntry.iBioType = 64;		
	sampleTMsvEntry.iMtmData1 = 64;		
	sampleTMsvEntry.iMtmData2 = 64;		
	sampleTMsvEntry.iMtmData3 = 64;	
	
	_LIT(KName, "Hello World");
	HBufC* data = NULL;
	TRAP_IGNORE(data = HBufC::NewL(15)); 
	*data = KName;
	sampleTMsvEntry.iDescription.Set(data->Des());
	
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleTMsvEntry, aVisibleFolder));	
	
	delete data;
	data = NULL;	
	}


/**
 TestAddEntry()
@param None.
@return None.
 
Tests the AddEntryL() functionality of CMsCacheVisibleFolder Class.
Tested Steps
01. Init
02. Add Entry When IndexTable is not created
03. Add entry when index table is created and Entry in range
04. Add entry when index table is created and Entry not in range
05. Adding an already Existing Entry in the cache
06. Adding a Child entry when index table is not created
07. Adding a Child entry whose parent is available and child entry is in range
08. Adding a Child entry whose parent is available and child entry is not in range
09. Adding a Child entry whose parent is not available
10. Adding a Great Grand Child entry whose parent is available
11. Adding a Great Grand Child entry whose parent is not available
*/
void CTestVisibleFolder::TestAddEntry()
	{
	_LIT(KFunction, "TestAddEntry");
	INFO_PRINTF1(KFunction);
	
	// 01. Init
	CMsvCacheEntry *entryE;
	TInt releasedEntries;
	
	// 02. Add Entry When IndexTable is not created
	CMsvCacheEntry* entryF = Entry();
	
	entryF->Entry().SetParent((TMsvId)30);
	entryF->Entry().SetId ((TMsvId)06);//TMsvId = 06
	entryF->LockEntry();
	
	TRAP_IGNORE(iFolderPointer[1]->AddEntryL(entryF));
	MY_ASSERT_TRUE(iFolderPointer[1]->GetEntry ((TMsvId)06, entryE));
	TRAP_IGNORE(iFolderPointer[1]->DeleteEntryL((TMsvId)06));

	ASSERT_SAME(entryF, entryE);
	entryE = NULL;
	entryF = NULL;
	
	// 03. Add entry when index table is created and Entry in range
	AddTestEntries();
	entryF = Entry();
	
	entryF->Entry().SetParent((TMsvId)30);
	entryF->Entry().SetId ((TMsvId)8);//TMsvId = 08
	
	TRAP_IGNORE(iFolderPointer[1]->AddEntryL(entryF));
	MY_ASSERT_TRUE(iFolderPointer[1]->GetEntry ((TMsvId)8, entryE));
	
	ASSERT_SAME(entryF, entryE);
	entryE =NULL;
	entryF =NULL;
	
	// 04. Add entry when index table is created and Entry not in range
	entryF = Entry();
	
	entryF->Entry().SetParent((TMsvId)30);
	entryF->Entry().SetId ((TMsvId)15);
	
	TRAP_IGNORE(iFolderPointer[1]->AddEntryL(entryF));
	MY_ASSERT_TRUE(iFolderPointer[1]->GetEntry ((TMsvId)15, entryE));
	ASSERT_SAME(entryF, entryE);
	entryE = NULL;
	entryF = NULL;

	// 05. Adding an already Existing Entry in the cache
	entryF = Entry();
	
	entryF->Entry().SetParent((TMsvId)30);
	entryF->Entry().SetId ((TMsvId)15);
	
	TRAP_IGNORE(iFolderPointer[1]->AddEntryL(entryF));
	MY_ASSERT_TRUE(iFolderPointer[1]->GetEntry ((TMsvId)15, entryE));
	ASSERT_SAME(entryF, entryE);
	entryE = NULL;
	entryF = NULL;
	
	//ReCreate
	TRAP_IGNORE(TearDownL());
	TRAP_IGNORE(SetupL());
	INFO_PRINTF1(KFunction);
	
	// 06. Adding a Child entry when index table is not created
	entryF = Entry();
	
	entryF->Entry().SetParent((TMsvId)5);
	entryF->Entry().SetId ((TMsvId)15);
		
	TRAP_IGNORE(iFolderPointer[1]->AddEntryL(entryF));
	MY_ASSERT_TRUE(iFolderPointer[1]->GetEntry ((TMsvId)15, entryE));
	entryE =NULL;
	MY_ASSERT_TRUE(iFolderPointer[1]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(1, releasedEntries);
	
	// 07. Adding a Child entry whose parent is available and child entry is in range
	AddTestEntries();
	entryF = Entry();
	
	entryF->Entry().SetParent((TMsvId)5);
	entryF->Entry().SetId ((TMsvId)6);
	
	TRAP_IGNORE(iFolderPointer[1]->AddEntryL(entryF));
	MY_ASSERT_TRUE(iFolderPointer[1]->GetEntry ((TMsvId)6, entryE));
	
	ASSERT_SAME(entryF, entryE);
	entryE = NULL;
	entryF = NULL;
	
	// 08. Adding a Child entry whose parent is available and child entry is not in range
	entryF = Entry();
	
	entryF->Entry().SetParent((TMsvId)5);
	entryF->Entry().SetId ((TMsvId)19);
	
	TRAP_IGNORE(iFolderPointer[1]->AddEntryL(entryF));
	MY_ASSERT_TRUE(iFolderPointer[1]->GetEntry ((TMsvId)19, entryE));
	
	ASSERT_SAME(entryF, entryE);
	entryE = NULL;
	entryF = NULL;
	
	// 09. Adding a Child entry whose parent is not available
	entryF = Entry();
	
	entryF->Entry().SetParent((TMsvId)50);
	entryF->Entry().SetId ((TMsvId)51);
	
	TRAP_IGNORE(iFolderPointer[1]->AddEntryL(entryF));
	MY_ASSERT_TRUE(iFolderPointer[1]->GetEntry ((TMsvId)51, entryE));
	
	entryE = NULL;
		
	}

/**
 TestAddEntryList()
@param None.
@return None.
 
Tests the AddEntryListL() functionality of CMsCacheVisibleFolder Class.
Tested Steps
01. Init
-- Index Table is not created
02. Add Zero Entries into the cache
03. Add one Entry into the cache
04. Add 64(Maximum size of a block) entries into the cache
05. Add 65 entries into the cache
06. Add 128 entries into the cache
07. Add 350 entries into the cache

-- Index Table already exists

08. Add Already existing entries ino the cache
09. Add new set of entries to the existing entries
10. Add existing and non-existing entries into the cache

-- Index Table is not created(Rogue Entries)

12. Add complete childrens
13. Add Grand Childrens
14. Add Great Grand Childrens

*/
void CTestVisibleFolder::TestAddEntryList()
	{
	_LIT(KFunction, "TestAddEntryList");
	INFO_PRINTF1(KFunction);
	
	//01. Init
	CMsvCacheEntry* entryF;
	TInt releasedEntries;
	TInt p = 0, j = 0, k = 0;
	RPointerArray<CMsvCacheEntry> addEntryPointer;
	RPointerArray<CMsvCacheEntry> getEntryPointer;
	RPointerArray<CMsvCacheIndexTableEntry>* indexTable;

	//02. Add Zero Entries into the cache
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer));

	
	/**************** 
	  * TEST CASE WITH (aIsCompleteChildOfFolder = ETrue)
	  * IndexTable is not created.
	  * 
	  * This will mainly check the functionality
	  * of SplitAndAppend()
	  */
	
	//03. Add one Entry into the cache
	entryF = Entry();
	
	entryF->Entry().SetParent((TMsvId)5);
	entryF->Entry().SetId ((TMsvId)10);
	entryF->LockEntry();
	addEntryPointer.Append(entryF);
	
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
    indexTable = iFolderPointer[0]->GetIndexTableEntry();
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(1, getEntryPointer.Count());
	MY_ASSERT_EQUALS(1, addEntryPointer.Count());
	MY_ASSERT_EQUALS((TMsvId)10, getEntryPointer[0]->Entry().Id());
	MY_ASSERT_EQUALS((TMsvId)10, addEntryPointer[0]->Entry().Id());
	MY_ASSERT_EQUALS(1, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)10, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(1, (*indexTable)[0]->BlockPtr()->Count());
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	entryF = NULL;
	delete iFolderPointer[0];
	
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));
	//04. Add 64(Maximum size of a block) entries into the cache
	for (TInt index = 10; index < 74; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(64, getEntryPointer.Count());
	MY_ASSERT_EQUALS(64, addEntryPointer.Count());
	for (TInt index = 0; index < 64; ++index)
		{
		ASSERT_EQUALS((TMsvId)(index+10), getEntryPointer[index]->Entry().Id());
		ASSERT_EQUALS((TMsvId)(index+10), addEntryPointer[index]->Entry().Id());
		}
	MY_ASSERT_EQUALS(1, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)73, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(64, (*indexTable)[0]->BlockPtr()->Count());
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(64, releasedEntries);
	entryF=NULL;
	delete iFolderPointer[0];
	
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));

	//05. Add 80(maximum size of block + 16) entries into the cache
	for (TInt index = 30; index < 110; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(80, getEntryPointer.Count());
	MY_ASSERT_EQUALS(80, addEntryPointer.Count());
	for (TInt index = 0; index < 80; ++index)
		{
		ASSERT_EQUALS((TMsvId)(index+30), getEntryPointer[index]->Entry().Id());
		ASSERT_EQUALS((TMsvId)(index+30), addEntryPointer[index]->Entry().Id());
		}
	MY_ASSERT_EQUALS(1, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)109, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(80, (*indexTable)[0]->BlockPtr()->Count());
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(80, releasedEntries);
	entryF=NULL;
	delete iFolderPointer[0];
	
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));
	
	//5.1 Add 16 (BLOCK_THRESHOLD) entries to cache
	for (TInt index = 100; index < 116; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(16, getEntryPointer.Count());
	MY_ASSERT_EQUALS(16, addEntryPointer.Count());
	MY_ASSERT_EQUALS(1, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)115, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(16, (*indexTable)[0]->BlockPtr()->Count());
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(16, releasedEntries);
		
	entryF=NULL;
	delete iFolderPointer[0];
	
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));
	
	//06. Add 100(64 + 16 + 10 ) entries into the cache
	for (TInt index = 100; index < 300; index+=2)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(100, getEntryPointer.Count());
	MY_ASSERT_EQUALS(100, addEntryPointer.Count());
	MY_ASSERT_EQUALS(2, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)198, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)199, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)298, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(50, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(50, (*indexTable)[1]->BlockPtr()->Count());
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(100, releasedEntries);
	entryF=NULL;
	delete iFolderPointer[0];
	
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));

	
	//07. Add 143(64+64+15 blocks) entries into the cache
	for (TInt index = 10; index < 153; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(143, getEntryPointer.Count());
	MY_ASSERT_EQUALS(143, addEntryPointer.Count());
	MY_ASSERT_EQUALS(2, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)80, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)81, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)152, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(71, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(72, (*indexTable)[1]->BlockPtr()->Count());
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(143, releasedEntries);
	entryF=NULL;
	delete iFolderPointer[0];
	
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));

	//08. Add 145(64+64+64+64+17 blocks) entries into the cache
	for (TInt index = 0; index < 273; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	MY_ASSERT_EQUALS(273, addEntryPointer.Count());
	MY_ASSERT_EQUALS(5, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)53, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)54, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)107, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)108, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)161, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)162, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)215, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[3]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)216, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)272, (*indexTable)[4]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[4]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(54, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(54, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(54, (*indexTable)[2]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(54, (*indexTable)[3]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(57, (*indexTable)[4]->BlockPtr()->Count());
	addEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(273, releasedEntries);
	entryF=NULL;
	delete iFolderPointer[0];
	
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));
	
	/* IsComplete() is set. 
	 * None of the blocks are dirty.
	 * Adding few grand children/immediate children(replica)
	 * in first and second blocks.
	 */
	// 1. Create initial state.
	for (TInt index = 0; index < 192; index+=3)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 0; index < 192; index+=3)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index+200);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 0; index < 192; index+=3)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index+400);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(192, getEntryPointer.Count());
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	MY_ASSERT_EQUALS(3, indexTable->Count());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[2]->IsDirty());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)189, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)190, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)389, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)390, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)589, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(64, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[2]->BlockPtr()->Count());
	
	// 2. Create array to be added.
	for (TInt index = 1; index < 50; index+=3)
		{
		j++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index-1);
		addEntryPointer.Append(entryF);
		if(index%2)
			{
			p++;
			entryF = Entry();
			entryF->Entry().SetParent((TMsvId)5);
			entryF->Entry().SetId ((TMsvId)index);
			addEntryPointer.Append(entryF);
			}		
		}
	p = addEntryPointer.Count();
	for (TInt index = 401; index < 500; index+=3)
		{
		j++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index-1);
		addEntryPointer.Append(entryF);		
		if(index%2)
			{
			k++;
			entryF = Entry();
			entryF->Entry().SetParent((TMsvId)5);
			entryF->Entry().SetId ((TMsvId)index);
			addEntryPointer.Append(entryF);
			}		
		}
	p = addEntryPointer.Count();	
	MY_ASSERT_EQUALS(76, addEntryPointer.Count());
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	MY_ASSERT_EQUALS(76, addEntryPointer.Count());
	addEntryPointer.Reset();

	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(3, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)189, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)190, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)389, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)390, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)589, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(73, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(81, (*indexTable)[2]->BlockPtr()->Count());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(218, getEntryPointer.Count());
	getEntryPointer.Reset();
	
	p=0; j=0; k=0;
	for (TInt index = 1; index < 50; index+=3)
		{
		if(index%2)
			continue;
		p++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);		
		}
	for (TInt index = 401; index < 500; index+=3)
		{
		if(index%2)
			continue;
		j++;
		entryF->Entry().SetParent((TMsvId)9);
		entryF = Entry();
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 700; index < 740; index++)
		{
		k++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	// addEntryPointer has some immediate children
	// (not already present and already added), and
	// some grand children as well.
	MY_ASSERT_EQUALS(64, addEntryPointer.Count());
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	MY_ASSERT_EQUALS(4, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)189, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)190, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)389, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)390, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)589, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)590, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)739, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[3]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(81, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(97, (*indexTable)[2]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(40, (*indexTable)[3]->BlockPtr()->Count());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	p = getEntryPointer.Count();
	MY_ASSERT_EQUALS(218, getEntryPointer.Count());

	addEntryPointer.Reset();
	getEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(282, releasedEntries);
	entryF=NULL;
	delete iFolderPointer[0];
	
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));
	
	
	/**
	 * IsComplete() flag is set.
	 * Last block is dirty. Block Ptr is NOT NULL.
	 */
	 // Creating complete index table.
	for (TInt index = 0; index < 192; index+=3)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 0; index < 192; index+=3)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index+200);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 0; index < 192; index+=3)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index+400);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(192, getEntryPointer.Count());
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	(*indexTable)[2]->ReleaseBlock(ETrue);
	
	// 2. Create array to be added.
	for (TInt index = 1; index < 50; index+=3)
		{
		j++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index-1);
		addEntryPointer.Append(entryF);
		if(index%2)
			{
			p++;
			entryF = Entry();
			entryF->Entry().SetParent((TMsvId)5);
			entryF->Entry().SetId ((TMsvId)index);
			addEntryPointer.Append(entryF);
			}		
		}
	p = addEntryPointer.Count();
	for (TInt index = 401; index < 500; index+=3)
		{
		j++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index-1);
		addEntryPointer.Append(entryF);		
		if(index%2)
			{
			k++;
			entryF = Entry();
			entryF->Entry().SetParent((TMsvId)5);
			entryF->Entry().SetId ((TMsvId)index);
			addEntryPointer.Append(entryF);
			}		
		}
	p = addEntryPointer.Count();	
	MY_ASSERT_EQUALS(76, addEntryPointer.Count());
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	MY_ASSERT_EQUALS(76, addEntryPointer.Count());
	addEntryPointer.Reset();

	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(3, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)189, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)190, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)389, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)390, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)589, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(73, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(50, (*indexTable)[2]->BlockPtr()->Count());
	(*indexTable)[2]->ClearDirty();	// Done temporary to avoid DB operation using GetChildren()
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	(*indexTable)[2]->SetDirty();   // Reverting back.
	MY_ASSERT_EQUALS(187, getEntryPointer.Count());
	getEntryPointer.Reset();
	
	p=0; j=0; k=0;
	for (TInt index = 1; index < 50; index+=3)
		{
		if(index%2)
			continue;
		p++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);		
		}
	for (TInt index = 401; index < 500; index+=3)
		{
		if(index%2)
			continue;
		j++;
		entryF->Entry().SetParent((TMsvId)9);
		entryF = Entry();
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 700; index < 740; index++)
		{
		k++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	// addEntryPointer has some immediate children
	// (not already present and already added), and
	// some grand children as well.
	MY_ASSERT_EQUALS(64, addEntryPointer.Count());
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	MY_ASSERT_EQUALS(4, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)189, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)190, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)389, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)390, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)589, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)590, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)739, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[3]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(81, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(66, (*indexTable)[2]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(40, (*indexTable)[3]->BlockPtr()->Count());
	(*indexTable)[2]->ClearDirty();	// Done temporary to avoid DB operation using GetChildren()
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	(*indexTable)[2]->SetDirty();   // Reverting back.
	p = getEntryPointer.Count();
	MY_ASSERT_EQUALS(187, getEntryPointer.Count());

	addEntryPointer.Reset();
	getEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(251, releasedEntries);
	entryF=NULL;
	delete iFolderPointer[0];
	
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));


	/**
	 * IsComplete() flag is set.
	 * All blocks are dirty. Block Ptr is also NULL.
	 */
	 // Creating complete index table.
	for (TInt index = 0; index < 192; index+=3)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 0; index < 192; index+=3)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index+200);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 0; index < 192; index+=3)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index+400);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(192, getEntryPointer.Count());
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	iFolderPointer[0]->ReleaseAllBlocks(releasedEntries);
	MY_ASSERT_EQUALS(192, releasedEntries);

	p=0; j=0; k=0;
	for (TInt index = 1; index < 50; index+=3)
		{
		if(index%2)
			continue;
		p++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);		
		}
	for (TInt index = 401; index < 500; index+=3)
		{
		if(index%2)
			continue;
		j++;
		entryF->Entry().SetParent((TMsvId)9);
		entryF = Entry();
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 700; index < 710; index++)
		{
		k++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	// addEntryPointer has some immediate children
	// (not already present and already added), and
	// some grand children as well.
	MY_ASSERT_EQUALS(34, addEntryPointer.Count());
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	MY_ASSERT_EQUALS(3, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)189, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)190, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)389, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)390, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)709, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(8, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry> *)0, (*indexTable)[1]->BlockPtr());
	MY_ASSERT_EQUALS(26, (*indexTable)[2]->BlockPtr()->Count());
	addEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(34, releasedEntries);
	entryF=NULL;
	delete iFolderPointer[0];
	
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));

	/**
	 * IsComplete() is False. The folder has some grandchildren.
	 * Add all immediate children of the visible folder.
	 */
	 // Add some grandchildren to empty folder.
	 for (TInt index = 0; index < 192; index+=3)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 0; index < 192; index+=3)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index+200);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 0; index < 192; index+=3)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index+400);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer));
	MY_ASSERT_EQUALS(192, addEntryPointer.Count());
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	addEntryPointer.Reset();
	MY_ASSERT_FALSE(iFolderPointer[0]->IsComplete());
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(3, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)189, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)190, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)389, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)390, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)589, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(64, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[2]->BlockPtr()->Count());
	
	// Add all children of the visible folder. 
	// IsComplete() is still EFalse.
	for (TInt index = 1; index < 192; index+=3)
		{
		p++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);		
		}
	for (TInt index = 401; index < 592; index+=3)
		{
		j++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 701; index < 892; index+=3)
		{
		k++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	// addEntryPointer has some immediate children
	// (not already present and already added), and
	// some grand children as well.	
	MY_ASSERT_EQUALS(192, addEntryPointer.Count());
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	MY_ASSERT_EQUALS(192, addEntryPointer.Count());
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	MY_ASSERT_EQUALS(3, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)190, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)191, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)590, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)591, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)890, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_TRUE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[2]->IsDirty());
	MY_ASSERT_EQUALS(128, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(192, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[2]->BlockPtr()->Count());
	addEntryPointer.Reset();
	getEntryPointer.Reset();
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(192, getEntryPointer.Count());
	getEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(384, releasedEntries);
	entryF=NULL;
	delete iFolderPointer[0];
	
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));


    /**
     * IsComplete() is False. The folder has some immediate and some grandchildren.
	 * Add all immediate children of the visible folder.
	 */
	 // Add some grandchildren to empty folder.
	 for (TInt index = 0; index < 192; index+=3)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 0; index < 192; index+=3)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index+200);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 0; index < 192; index+=3)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index+400);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer));
	MY_ASSERT_EQUALS(192, addEntryPointer.Count());
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	addEntryPointer.Reset();
	MY_ASSERT_FALSE(iFolderPointer[0]->IsComplete());
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(3, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)189, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)190, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)389, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)390, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)589, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(64, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[2]->BlockPtr()->Count());
	
	// Adding few immediate children of folder.
	for (TInt index = 1; index < 192; index+=9)
		{
		p++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);		
		}
	for (TInt index = 701; index < 892; index+=12)
		{
		k++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer));
	MY_ASSERT_EQUALS(38, addEntryPointer.Count());
	addEntryPointer.Reset();
	MY_ASSERT_FALSE(iFolderPointer[0]->IsComplete());

	// Add all children of the visible folder. 
	// IsComplete() is still EFalse.
	for (TInt index = 1; index < 192; index+=3)
		{
		p++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);		
		}
	for (TInt index = 401; index < 592; index+=3)
		{
		j++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 701; index < 892; index+=3)
		{
		k++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	// addEntryPointer has some immediate children
	// (not already present and already added), and
	// some grand children as well.	
	MY_ASSERT_EQUALS(192, addEntryPointer.Count());
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	MY_ASSERT_EQUALS(192, addEntryPointer.Count());
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	MY_ASSERT_EQUALS(3, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)190, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)191, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)590, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)591, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)890, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_TRUE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[2]->IsDirty());
	MY_ASSERT_EQUALS(128, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(192, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[2]->BlockPtr()->Count());
	addEntryPointer.Reset();
	getEntryPointer.Reset();
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(192, getEntryPointer.Count());
	getEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(384, releasedEntries);
	entryF=NULL;
	delete iFolderPointer[0];
	
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));

	/**
	 * Is Complete is False. Initial number of entries < 64.
	 * Add list of rogue entries.
	 */
    p=0; k=0;
  	for (TInt index = 1; index < 192; index+=9)
		{
		p++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);		
		}
	for (TInt index = 701; index < 892; index+=12)
		{
		k++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer));
	MY_ASSERT_EQUALS(38, addEntryPointer.Count());
	addEntryPointer.Reset();
	MY_ASSERT_FALSE(iFolderPointer[0]->IsComplete());
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(1, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)881, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(38, (*indexTable)[0]->BlockPtr()->Count());
	
	p=0; k=0; j=0;
	for (TInt index = 1; index < 192; index+=3)
		{
		p++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);		
		}
	for (TInt index = 401; index < 592; index+=3)
		{
		j++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 845; index < 992; index+=3)
		{
		k++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	MY_ASSERT_EQUALS(177, addEntryPointer.Count());
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer));
	MY_ASSERT_EQUALS(177, addEntryPointer.Count());
	addEntryPointer.Reset();
	MY_ASSERT_FALSE(iFolderPointer[0]->IsComplete());
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(1, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)989, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(189, (*indexTable)[0]->BlockPtr()->Count());
	getEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(189, releasedEntries);
	entryF=NULL;
	delete iFolderPointer[0];
	
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));

	/**
	 * Is Complete is False. Initial number of entries > 64.
	 * Add list of rogue entries.
	 */
    p=0; k=0; j =0;
  	for (TInt index = 1; index < 192; index+=3)
		{
		p++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);		
		}
	for (TInt index = 401; index < 592; index+=3)
		{
		j++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 701; index < 892; index+=3)
		{
		k++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer));
	MY_ASSERT_EQUALS(192, addEntryPointer.Count());
	addEntryPointer.Reset();
	MY_ASSERT_FALSE(iFolderPointer[0]->IsComplete());
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(3, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)190, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)191, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)590, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)591, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)890, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_TRUE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_TRUE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(64, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[2]->BlockPtr()->Count());
		
	p=0; k=0; j=0;
	for (TInt index = 1; index < 192; index+=12)
		{
		p++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);		
		}
	for (TInt index = 845; index < 992; index+=6)
		{
		k++;
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	MY_ASSERT_EQUALS(41, addEntryPointer.Count());
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer));
	MY_ASSERT_EQUALS(41, addEntryPointer.Count());
	addEntryPointer.Reset();
	MY_ASSERT_FALSE(iFolderPointer[0]->IsComplete());
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(3, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)190, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)191, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)590, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)591, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)989, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_TRUE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_TRUE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_EQUALS(64, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(81, (*indexTable)[2]->BlockPtr()->Count());
	getEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(209, releasedEntries);
	entryF=NULL;
	delete iFolderPointer[0];
	
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));



	/*
	TearDownL();
	INFO_PRINTF1(KTestAddEntryList);
	SetupL();

	//09. Add Already existing entries into the cache
	for (TInt index = 10; index < 154; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	
	iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue);
	addEntryPointer.Reset();
	getEntryPointer.Reset();
	entryF=NULL;
	
	for (TInt index = 10; index < 154; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}

	iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue);
	ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer));
	tempId = 10;
	for(TInt index = 0; index < getEntryPointer.Count(); ++index)
		{
		ASSERT_EQUALS(tempId, getEntryPointer[index]->Entry().Id());
		++tempId;
		}
	ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(144,releasedEntries);
	getEntryPointer.Reset();
	entryF=NULL;
		
	//10. Add new set of entries to the existing entries
	for (TInt index = 10; index < 154; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	
	iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue);
	addEntryPointer.Reset();
	getEntryPointer.Reset();
	entryF=NULL;
	
	for (TInt index = 154; index < 164; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}

	iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue);
	ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer));
	tempId = 10;
	for(TInt index = 0; index < getEntryPointer.Count(); ++index)
		{
		ASSERT_EQUALS(tempId, getEntryPointer[index]->Entry().Id());
		++tempId;
		}
	ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(154,releasedEntries);
	getEntryPointer.Reset();
	entryF=NULL;
	
	//11. Add existing and non-existing entries into the cache
	for (TInt index = 10; index < 164; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	
	iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue);
	ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer));
	tempId = 10;
	for(TInt index = 0; index < getEntryPointer.Count(); ++index)
		{
		ASSERT_EQUALS(tempId, getEntryPointer[index]->Entry().Id());
		++tempId;
		}
	ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(154,releasedEntries);
	getEntryPointer.Reset();
	entryF=NULL;
	
	TearDownL();
	INFO_PRINTF1(KTestAddEntryList);
	SetupL();
	
	//12. Add Grand childrens
	for (TInt index = 10; index < 164; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		entryF->Entry().SetVisible(EFalse);
		addEntryPointer.Append(entryF);
		
		}

	iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue);
	addEntryPointer.Reset();
	
	for (TInt index = 165; index < 170; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)160);
		entryF->Entry().SetId ((TMsvId)index);
		entryF->Entry().SetVisible(EFalse);
		addEntryPointer.Append(entryF);
		}

	iFolderPointer[0]->UpdateChildMsvIdsL(addEntryPointer);
	iFolderPointer[0]->AddEntryListL(addEntryPointer,EFalse);
	
	getEntryPointer.Reset();
	ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)160, NULL, getEntryPointer));
	tempId = 169;
	for(TInt index = 0; index < getEntryPointer.Count(); ++index)
		{
		ASSERT_EQUALS(tempId, getEntryPointer[index]->Entry().Id());
		--tempId;
		}
	ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(159,releasedEntries);
	getEntryPointer.Reset();
	entryF=NULL;
	
	//14. Add Great Grand Childrens
	for (TInt index = 10; index < 164; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		entryF->Entry().SetVisible(EFalse);
		addEntryPointer.Append(entryF);
		
		}

	iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue);
	addEntryPointer.Reset();
	
	for (TInt index = 165; index < 170; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)160);
		entryF->Entry().SetId ((TMsvId)index);
		entryF->Entry().SetVisible(EFalse);
		addEntryPointer.Append(entryF);
		}

	iFolderPointer[0]->UpdateChildMsvIdsL(addEntryPointer);
	iFolderPointer[0]->AddEntryListL(addEntryPointer,EFalse);
	addEntryPointer.Reset();
	
	for (TInt index = 171; index < 173; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)168);
		entryF->Entry().SetId ((TMsvId)index);
		entryF->Entry().SetVisible(EFalse);
		addEntryPointer.Append(entryF);
		}

	iFolderPointer[0]->UpdateChildMsvIdsL(addEntryPointer);
	iFolderPointer[0]->AddEntryListL(addEntryPointer,EFalse);
	
	
	getEntryPointer.Reset();
	ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)168, NULL, getEntryPointer));
	tempId = 172;
	TMsvId temp2;
	for(TInt index = 0; index < getEntryPointer.Count(); ++index)
		{
		 temp2 =  getEntryPointer[index]->Entry().Id();
		ASSERT_EQUALS(tempId, temp2);
		--tempId;
		}
	ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(161,releasedEntries);
	getEntryPointer.Reset();
	entryF=NULL;
*/	
	}

/**
 TestGetEntry()
@param None.
@return None.
 
Tests the GetEntry() functionality of CMsCacheVisibleFolder Class.
Tested Steps
01. Init
02. Get Entry When IndexTable is not created
03. Get Entry Which is not available
04. Get an Entry Which is available and immediate child of the visible Folder
05. Get Entry Which is available and a Grand Child of the visible Folder
06. Get Entry Which is available and a Great Grand Child of the visible Folder
07. Get Entry Which is deleted
*/
void CTestVisibleFolder::TestGetEntry()
	{
	_LIT(KFunction, "TestGetEntry");
	INFO_PRINTF1(KFunction);
		
	// 01. Init
	CMsvCacheEntry* entryE;
	
	// 02. Get Entry When IndexTable is not created
	ASSERT_FALSE(iFolderPointer[1]->GetEntry ((TMsvId)06, entryE));
	
	// 03. Get Entry Which is not available
	AddTestEntries();
	ASSERT_FALSE(iFolderPointer[1]->GetEntry ((TMsvId)06, entryE));

	// 04. Get an Entry Which is available and immediate child of the visible Folder
	ASSERT_TRUE(iFolderPointer[1]->GetEntry ((TMsvId)07, entryE));
	
	ASSERT_SAME(aEntryB, entryE);
	entryE = NULL;
	
	// 05. Get Entry Which is available and a Grand Child of the visible Folder
	CMsvCacheEntry* entryF = Entry();
	
	entryF->Entry().SetParent((TMsvId)5);
	entryF->Entry().SetId ((TMsvId)6);

	TRAP_IGNORE(iFolderPointer[1]->AddEntryL(entryF));
	ASSERT_TRUE(iFolderPointer[1]->GetEntry ((TMsvId)6, entryE));
	
	ASSERT_SAME(entryF, entryE);
	entryE = NULL;
	entryF = NULL;
	
	// 06. Get Entry Which is available and a Great Grand Child of the visible Folder
	entryF = Entry();
	
	entryF->Entry().SetParent((TMsvId)6);
	entryF->Entry().SetId ((TMsvId)12);
	
	TRAP_IGNORE(iFolderPointer[1]->AddEntryL(entryF));
	ASSERT_TRUE(iFolderPointer[1]->GetEntry ((TMsvId)12, entryE));
	
	ASSERT_SAME(entryF, entryE);
	entryE = NULL;
	entryF = NULL;
	
	// 06. Get Entry Which is available and a Great Grand Child of the visible Folder
	entryF = Entry();
	entryF->Entry().SetParent((TMsvId)12);
	entryF->Entry().SetId ((TMsvId)13);
	entryF->LockEntry();
	
	TRAP_IGNORE(iFolderPointer[1]->AddEntryL(entryF));
	ASSERT_TRUE(iFolderPointer[1]->GetEntry ((TMsvId)13, entryE));
	
	ASSERT_SAME(entryF, entryE);
	entryE = NULL;
	
	// 07. Get Entry Which is deleted
	TRAP_IGNORE(iFolderPointer[1]->DeleteEntryL((TMsvId)13));
	ASSERT_FALSE(iFolderPointer[1]->GetEntry ((TMsvId)13, entryE));
	
	entryE = NULL;
	}



#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

// Added as part of PREQ 557.
void CTestVisibleFolder::InitDb()
	{
	_LIT(KFunction, "InitDb");
	INFO_PRINTF1(KFunction);

	_LIT(KDbFileName, "\\messaging.db");	
	_LIT(KBaseDBFileName, "C:\\messaging_base.db");

	//Delete database
	TParse parse;
	TPtrC drive(TDriveUnit(2).Name());
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dBFile = parse.FullName();
	RSqlDatabase::Delete(dBFile);
	TInt err = RSqlDatabase::Copy(KBaseDBFileName, dBFile);
	MY_ASSERT_EQUALS(KErrNone, err);
	
	//Create and open the database
	TMsvId maxId;
	TRAP(err, iMsvDbAdaptor = CMsvDBAdapter::NewL());
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP_IGNORE(iMsvDbAdaptor->AttachDBL(TDriveUnit(2), 2, maxId));
	
	err = iFileHandle.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);
	}



// Added as part of PREQ 557.
void CTestVisibleFolder::DeleteDb()
	{
	_LIT(KFunction, "DeleteDb().");
	INFO_PRINTF1(KFunction);
	
	TRAP_IGNORE(iMsvDbAdaptor->DetachDBL(2));
	delete iMsvDbAdaptor;	
	}
	
	
/**
 TestGetChildren()
@param None.
@return None.
 
Added as part of PREQ 557.
Tests the GetChildrenL() functionality of CMsCacheVisibleFolder Class.
Tested Steps
1. Init

---GetChildren on a visible folder
2. Complete flag = 0. Indextable = NULL, no children in DB
3. Folder not complete, some random entries present in Indextable
4. Folder not complete, >1 blocks, only random entries present in Indextable
5. Folder not complete, >1 blocks, random + immediate child entries in indextable
6. Folder complete, all blocks NULL.
7. Folder complete, 1 block NULL
8. Folder complete, 1 block dirty and has rogue entries
9. Folder complete, children + grandchildren present
10. Folder complete, 1st and last blocks dirty

---GetChildren on a non-visible folder
11. GetChildren when parent of the folder doesn't exist
12. GetChildren when parent is visible folder but not iVisibleFolderId
13. GetChildren on non-visible folder on whom GetChildren hasn't been performed
14. GetChildren on non-visible folder who has no children
15. GetChildren on folder with child array filled and all children present in cache

*/
void CTestVisibleFolder::TestGetChildren()
	{
	_LIT(KFunction, "TestGetChildren");
	INFO_PRINTF1(KFunction);

	// 01. Init
	CMsvCacheEntry* entryF;
	RPointerArray<CMsvCacheEntry> addEntryPointer;
	RPointerArray<CMsvCacheEntry> getEntryPointer;
	TInt releasedEntries;
	TMsvId tempId = MaskTMsvId(2, KFirstFreeEntryId+35);
	TMsvId id = 100; //KFirstFreeEntryId;
	TMsvId visibleFolder;
	TInt err;
	TInt bulkCount = 50;
	TBool ret;
	RPointerArray<CMsvCacheIndexTableEntry>* indexTable;
	
	//create a visible folder
	InitDb(); //will not replace the INI files in this
	CMsvCacheVisibleFolder *TestFolder = NULL;
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	TMsvEntry folderEntry;
	folderEntry.SetId(tempId);
	folderEntry.SetVisible(ETrue);
	folderEntry.SetParent(KMsvRootIndexEntryId);
	visibleFolder = KMsvRootIndexEntryId;
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder));
	
	
	// test cases 2-10 are with the folder as the visible folder.
	// remaining are with the folder as non-visible folder.
	
	// 2. Complete flag = 0. Indextable = NULL, no children in DB
	TestFolder->SetComplete(EFalse);
	TRAP(err, ret = TestFolder->GetChildrenL(tempId, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	//MY_ASSERT_FALSE(TestFolder->IsComplete());
	MY_ASSERT_EQUALS(TestFolder->GetIndexTableEntry()->Count(), 0);
	DeleteDb();
	delete TestFolder;
	
	
	// 3. Folder not complete, some random entries present in Indextable
	TMsvEntry sampleEntry;
	TMsvId minId, maxId;
	id = MaskTMsvId(2, KFirstFreeEntryId+100);
	TMsvId sampleId = id;
	InitDb();
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	folderEntry.SetId(tempId);
	folderEntry.SetVisible(ETrue);
	folderEntry.SetParent(KMsvRootIndexEntryId);
	visibleFolder = KMsvRootIndexEntryId; //or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	CMsvCacheVisibleFolder *parent = NULL;
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL(MaskTMsvId(2, 9)));
	TMsvEntry rogueParent;
	rogueParent.SetId(MaskTMsvId(2, KFirstFreeEntryId+9));
	rogueParent.SetVisible(ETrue);
	rogueParent.SetParent(KMsvRootIndexEntryId);
	visibleFolder = KMsvRootIndexEntryId; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	for(TInt index = 0; index < bulkCount; ++index) //create the entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+9));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	maxId = sampleId;
	for (TInt index = 0; index < bulkCount - 10; ++index) //(bulkCount - 10) entries in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+9));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //random entries
	MY_ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL(tempId, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 0);
	MY_ASSERT_EQUALS(1, indexTable->Count());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS(maxId-10, (*indexTable)[0]->GetMaxMsvIdRange());
	
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
	
	
	// 4. Folder not complete, >1 blocks, only random entries present in Indextable
	InitDb();
	id = MaskTMsvId(2, KFirstFreeEntryId+100);
	sampleId = id;
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	TestFolder->SetComplete(EFalse);
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL((TMsvId)(KFirstFreeEntryId+9)));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	for(TInt index = 0; index < 3*bulkCount; ++index) //create entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+9));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	maxId = sampleId;
	addEntryPointer.Reset();
	for (TInt index = 0; index < 2*bulkCount - 5; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+9));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //random entries
	MY_ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL(tempId, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
//	MY_ASSERT_FALSE(TestFolder->IsComplete());
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(2, indexTable->Count());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_TRUE((*indexTable)[1]->IsGrandChildPresent());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+147), maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+148), minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+195), maxId);
	
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
		
	
	// 5. Folder not complete, >1 blocks, random + immediate child entries in indextable
	InitDb();
	id = MaskTMsvId(2, KFirstFreeEntryId+100);
	sampleId = id;
	tempId = MaskTMsvId(2, KFirstFreeEntryId+35);
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	TestFolder->SetComplete(EFalse);
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL(MaskTMsvId(2, KFirstFreeEntryId+9)));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	for(TInt index = 0; index < bulkCount; ++index) //create rogue entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+9));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	addEntryPointer.Reset();
	for (TInt index = 0; index < bulkCount; ++index) //rogue entries in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+9));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //rogue entries
	ASSERT_EQUALS(err, KErrNone);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 2*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+35));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 2*bulkCount - 10; ++index) //10 less children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+35));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL(MaskTMsvId(2, KFirstFreeEntryId+35), iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 2*bulkCount); //2*bulkCount children
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 2);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+200), maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+201), minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+250), maxId);
		
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
		
	
	// 6. Folder complete, all blocks NULL.
	//add children to DB and get them to cache. then release all blocks to make them NULL
	InitDb();
	id = MaskTMsvId(2, KFirstFreeEntryId+100);
	sampleId = id;
	tempId = MaskTMsvId(2, KFirstFreeEntryId+35);
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL(MaskTMsvId(2, KFirstFreeEntryId+9)));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+35));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount - 10; ++index) //10 less children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(2, 35));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL(MaskTMsvId(2, KFirstFreeEntryId+35), iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 3*bulkCount); //bulkCount children
	TestFolder->ReleaseAllBlocks(releasedEntries);
	MY_ASSERT_EQUALS(releasedEntries, 3*bulkCount);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_TRUE(TestFolder->IsAllBlocksDirty());
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL(MaskTMsvId(2, KFirstFreeEntryId+35), iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 3 * bulkCount); //150 children
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 3); //3 blocks still
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+150), maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+151), minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+200), maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+201), minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+250), maxId);
			
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
		
	
	// 7. Folder complete, 1 block NULL
	//add children in DB and cache. release last block to make it NULL.
	InitDb();
	id = MaskTMsvId(2, KFirstFreeEntryId+100);
	sampleId = id;
	tempId = MaskTMsvId(2, KFirstFreeEntryId+35);
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL(MaskTMsvId(2, KFirstFreeEntryId+9)));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+35));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+35));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	TestFolder->SetComplete(ETrue);
	indexTable = TestFolder->GetIndexTableEntry();
	(*indexTable)[2]->ReleaseBlock();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	MY_ASSERT_TRUE((*indexTable)[0]->BlockPtr() != NULL);
	MY_ASSERT_TRUE((*indexTable)[1]->BlockPtr() != NULL);
	MY_ASSERT_TRUE((*indexTable)[2]->BlockPtr() == NULL);
	MY_ASSERT_TRUE((*indexTable)[2]->IsDirty());
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL(MaskTMsvId(2, KFirstFreeEntryId+35), iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 3*bulkCount); //150 entries in 3 blocks
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+150), maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+151), minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+200), maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+201), minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+250), maxId);
			
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
	
	
	// 8. Folder complete, 1 block dirty and has rogue entries
	InitDb();
	id = MaskTMsvId(2, KFirstFreeEntryId+100);
	sampleId = id;
	tempId = MaskTMsvId(2, KFirstFreeEntryId+35);
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL(MaskTMsvId(2, KFirstFreeEntryId+9)));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+35));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+35));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	TestFolder->SetComplete(ETrue);
	(*indexTable)[2]->ReleaseBlock();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	MY_ASSERT_TRUE((*indexTable)[0]->BlockPtr() != NULL);
	MY_ASSERT_TRUE((*indexTable)[1]->BlockPtr() != NULL);
	MY_ASSERT_TRUE((*indexTable)[2]->BlockPtr() == NULL);
	MY_ASSERT_TRUE((*indexTable)[2]->IsDirty());
	addEntryPointer.Reset();
	for(TInt index = 0; index < 10; ++index) //create rogue entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+9));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	addEntryPointer.Reset();
	for (TInt index = 0; index < 10; ++index) //rogue entries in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+9));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //rogue entries
	ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL(MaskTMsvId(2, KFirstFreeEntryId+35), iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 3*bulkCount); //150 children
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+150), maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+151), minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+200), maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+201), minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+260), maxId); //rogue entries have changed iMaxMsvId
			
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
	
	
	// 9. Folder complete, children + grandchildren present
	//Add children and grandchildren. Perform GetChildren on the folder.
	InitDb();
	id = MaskTMsvId(2, KFirstFreeEntryId+100);
	sampleId = id;
	tempId = MaskTMsvId(2, KFirstFreeEntryId+35);
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL(MaskTMsvId(2, KFirstFreeEntryId+9)));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+35));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+35));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	TestFolder->SetComplete(ETrue);
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL(MaskTMsvId(2, KFirstFreeEntryId+35), iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 3*bulkCount);
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	addEntryPointer.Reset();
	for(TInt index = 0; index < 5; ++index) //create grandchild entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+250));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 5; ++index) //all grandchildren in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+250));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //grandchild entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	MY_ASSERT_TRUE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL(MaskTMsvId(2, KFirstFreeEntryId+35), iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 3*bulkCount); //150 children
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+150), maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+151), minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+200), maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+201), minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+255), maxId); //grandchild entries change iMaxMsvId in the last block
	
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
	
	
	// 10. Folder complete, 1st and last blocks dirty
	InitDb();
	id = MaskTMsvId(2, KFirstFreeEntryId+100);
	sampleId = id;
	tempId = MaskTMsvId(2, KFirstFreeEntryId+35);
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL(MaskTMsvId(2, KFirstFreeEntryId+9)));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+35));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+35));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	TestFolder->SetComplete(ETrue);
	(*indexTable)[0]->ReleaseBlock(); //release first and last blocks
	(*indexTable)[2]->ReleaseBlock();
	ASSERT_TRUE((*indexTable)[0]->IsDirty());
	ASSERT_TRUE((*indexTable)[2]->IsDirty());
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL(MaskTMsvId(2, KFirstFreeEntryId+35), iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 3*bulkCount); //150 children
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2,  KFirstFreeEntryId+150), maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2,  KFirstFreeEntryId+151), minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2,  KFirstFreeEntryId+200), maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2,  KFirstFreeEntryId+201), minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2,  KFirstFreeEntryId+250), maxId);
	
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
	
	
	
	// test cases 11-   are with the folder as a non-visible folder.
	//Create a non-visible folder with a visible folder parent.
	RArray<TMsvId> childArr;
	CMsvCacheEntry *pCacheEntry = NULL;
	InitDb();
	id = MaskTMsvId(2, KFirstFreeEntryId+100);
	sampleId = id;
	
	CMsvCacheVisibleFolder *visFolder = NULL;
	TRAP_IGNORE(visFolder = CMsvCacheVisibleFolder::NewL(MaskTMsvId(2,  20)));
	TMsvEntry visEntry;
	visEntry.SetId(MaskTMsvId(2, KFirstFreeEntryId+20));
	visEntry.SetVisible(ETrue); //visible
	visEntry.SetParent(KMsvRootIndexEntryId);
	TMsvId immvisId = KErrNotFound;
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(visEntry, immvisId)); //visible folder, parent of below non-visible folder
	
	tempId = MaskTMsvId(2, KFirstFreeEntryId+35);
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	TMsvEntry nonvisEntry;
	nonvisEntry.SetId(MaskTMsvId(2, KFirstFreeEntryId+35));
	nonvisEntry.SetVisible(EFalse); //non-visible
	nonvisEntry.SetParent(visEntry.Id());
	TMsvId nonvisId = KErrNotFound;
	immvisId = visEntry.Id();
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(nonvisEntry, immvisId)); //non-visible folder
	ASSERT_TRUE(immvisId == visEntry.Id());
	
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL(MaskTMsvId(2, KFirstFreeEntryId+9)));
	TMsvEntry parentEntry;
	parentEntry.SetId(MaskTMsvId(2, KFirstFreeEntryId+9));
	parentEntry.SetVisible(EFalse);
	parentEntry.SetParent(KMsvRootIndexEntryId);
	visibleFolder = KErrNotFound; // or Root
	visibleFolder = KMsvRootIndexEntryId;
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(parentEntry, visibleFolder));
	ASSERT_TRUE(visibleFolder == KMsvRootIndexEntryId);
	
	
	// 11. GetChildren when parent of the folder doesn't exist
	nonvisEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+40)); //40 doesn't exist, set it for this test
	getEntryPointer.Reset();
	TRAP(err, ret = visFolder->GetChildrenL(tempId, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	
	// 12. GetChildren when parent is visible folder but not iVisibleFolderId
	nonvisEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+9)); //TMsvId of parent
	entryF = Entry();
	entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+9));
	entryF->Entry().SetId(MaskTMsvId(2, KFirstFreeEntryId+35));
	TRAP(err, TestFolder->AddEntryL(entryF, ETrue));
	ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	ret = EFalse;
	TRAP(err, ret = visFolder->GetChildrenL(tempId, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	MY_ASSERT_FALSE(ret);
	
	DeleteDb();
	delete TestFolder;
	delete parent;
	delete visFolder;
	entryF = NULL;
	

	// 13. GetChildren on non-visible folder on whom GetChildren hasn't been performed
	InitDb();
	id = MaskTMsvId(2, KFirstFreeEntryId+100);
	sampleId = id;
	TRAP_IGNORE(visFolder = CMsvCacheVisibleFolder::NewL(MaskTMsvId(2, KFirstFreeEntryId+20)));
	visEntry.SetId(MaskTMsvId(2, KFirstFreeEntryId+20));
	visEntry.SetVisible(ETrue); //visible
	visEntry.SetParent(KMsvRootIndexEntryId);
	immvisId = KErrNotFound;
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(visEntry, immvisId)); //visible folder, parent of below non-visible folder
	tempId = MaskTMsvId(2, KFirstFreeEntryId+35);
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	nonvisEntry.SetId(MaskTMsvId(2, KFirstFreeEntryId+35));
	nonvisEntry.SetVisible(EFalse); //non-visible
	nonvisEntry.SetVisibleFolderFlag(EFalse); //non-visible
	nonvisEntry.SetParent(visEntry.Id());
	nonvisId = KErrNotFound;
	immvisId = KMsvRootIndexEntryId;
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(nonvisEntry, immvisId)); //non-visible folder
	//ASSERT_TRUE(immvisId == visEntry.Id());
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL(MaskTMsvId(2, KFirstFreeEntryId+9)));
	parentEntry.SetId(MaskTMsvId(2, KFirstFreeEntryId+9));
	parentEntry.SetVisible(EFalse);
	parentEntry.SetVisibleFolderFlag(EFalse);
	parentEntry.SetParent(KMsvRootIndexEntryId);
	visibleFolder = KMsvRootIndexEntryId; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(parentEntry, visibleFolder));
	ASSERT_TRUE(visibleFolder == KMsvRootIndexEntryId);
	addEntryPointer.Reset();
	tempId = MaskTMsvId(2, KFirstFreeEntryId+20);
	for(TInt index = 0; index < bulkCount; ++index) //create child entries of parent in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+20));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+20));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, visFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries of visible parent folder
	ASSERT_EQUALS(err, KErrNone);
	
	nonvisEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+9)); //add the non-visible folder with non-visible parent to cache
	parentEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+20));
	entryF = Entry();
	entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+20));
	entryF->Entry().SetId (MaskTMsvId(2, KFirstFreeEntryId+9));
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetVisibleFolderFlag(EFalse);
	TRAP(err, visFolder->AddEntryL(entryF, ETrue));
	ASSERT_EQUALS(err, KErrNone);
	entryF = Entry();
	entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+9));
	entryF->Entry().SetId (MaskTMsvId(2, KFirstFreeEntryId+35)); 
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetVisibleFolderFlag(EFalse);
	TRAP(err, visFolder->AddEntryL(entryF, ETrue));
	ASSERT_EQUALS(err, KErrNone);
	pCacheEntry = entryF;
	
	addEntryPointer.Reset();
	//tempId = MaskTMsvId(2, 35);
	tempId = MaskTMsvId(2, KFirstFreeEntryId+20);
	for(TInt index = 0; index < 5; ++index) //create child entries of non-visible folder in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+35));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	TRAP(err, visFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries of non-visible folder
	ASSERT_EQUALS(err, KErrNone); //(bulkCount + 2 + 5) entries added till now
	indexTable = visFolder->GetIndexTableEntry();
	getEntryPointer.Reset();
	TRAP(err, ret = visFolder->GetChildrenL(MaskTMsvId(2, KFirstFreeEntryId+35), iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 5); //5 children
	MY_ASSERT_EQUALS(indexTable->Count(), 1);
	childArr = *(pCacheEntry->ChildIdArray());
	MY_ASSERT_EQUALS(childArr.Count(), 5);
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+155), maxId); //non-visible folder's children have maximum TMsvId

	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete visFolder;
	delete parent;
	

	// 14. GetChildren on non-visible folder who has no children
	InitDb();
	id = MaskTMsvId(2, KFirstFreeEntryId+100);
	sampleId = id;
	TRAP_IGNORE(visFolder = CMsvCacheVisibleFolder::NewL(MaskTMsvId(2, KFirstFreeEntryId+20)));
	visEntry.SetId(MaskTMsvId(2, KFirstFreeEntryId+20));
	visEntry.SetVisible(ETrue); //visible
	visEntry.SetParent(KMsvRootIndexEntryId);
	immvisId = KMsvRootIndexEntryId;
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(visEntry, immvisId)); //visible folder, parent of below non-visible folder
	tempId = MaskTMsvId(2, KFirstFreeEntryId+35);
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	nonvisEntry.SetId(MaskTMsvId(2, KFirstFreeEntryId+35));
	nonvisEntry.SetVisible(EFalse); //non-visible
	nonvisEntry.SetVisibleFolderFlag(EFalse); //non-visible
	nonvisEntry.SetParent(visEntry.Id());
	nonvisId = KErrNotFound;
	immvisId = MaskTMsvId(2, KFirstFreeEntryId+20);
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(nonvisEntry, immvisId)); //non-visible folder
//	ASSERT_TRUE(immvisId == visEntry.Id());
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL(MaskTMsvId(2, KFirstFreeEntryId+9)));
	parentEntry.SetId(MaskTMsvId(2, KFirstFreeEntryId+9));
	parentEntry.SetVisible(EFalse);
	parentEntry.SetVisibleFolderFlag(EFalse);
	parentEntry.SetParent(KMsvRootIndexEntryId);
	visibleFolder = KMsvRootIndexEntryId; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(parentEntry, visibleFolder));
	ASSERT_TRUE(visibleFolder == KMsvRootIndexEntryId);
	
	addEntryPointer.Reset();
	tempId = MaskTMsvId(2, KFirstFreeEntryId+20);
	for(TInt index = 0; index < bulkCount; ++index) //create child entries of parent in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+20));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+20));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, visFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries of visible parent folder
	ASSERT_EQUALS(err, KErrNone);
	parentEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+20));
	entryF = Entry();
	entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+20));
	entryF->Entry().SetId (MaskTMsvId(2, KFirstFreeEntryId+9));
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetVisibleFolderFlag(EFalse);
	TRAP(err, visFolder->AddEntryL(entryF, ETrue));
	ASSERT_EQUALS(err, KErrNone);
	nonvisEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+9)); //add the non-visible folder with non-visible parent to cache
	entryF = Entry();
	entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+9));
	entryF->Entry().SetId (MaskTMsvId(2, KFirstFreeEntryId+35));
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetVisibleFolderFlag(EFalse);
	TRAP(err, visFolder->AddEntryL(entryF, ETrue));
	ASSERT_EQUALS(err, KErrNone);
	pCacheEntry = entryF;

	getEntryPointer.Reset();
	TRAP(err, ret = visFolder->GetChildrenL(MaskTMsvId(2, KFirstFreeEntryId+35), iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 0); //no children
	childArr = *(pCacheEntry->ChildIdArray());
	MY_ASSERT_TRUE(childArr.Count() == 0);
	indexTable = visFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 1);
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId+150), maxId);

	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete visFolder;
	delete parent;
	

	// 15. GetChildren on folder with child array filled and all children present in cache
	InitDb();
	id = MaskTMsvId(2, KFirstFreeEntryId+100);
	sampleId = id;
	TRAP_IGNORE(visFolder = CMsvCacheVisibleFolder::NewL(MaskTMsvId(2, KFirstFreeEntryId+20)));
	visEntry.SetId(MaskTMsvId(2, KFirstFreeEntryId+20));
	visEntry.SetVisible(ETrue); //visible
	visEntry.SetParent(KMsvRootIndexEntryId);
	immvisId = KMsvRootIndexEntryId;
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(visEntry, immvisId)); //visible folder, parent of below non-visible folder
	tempId = MaskTMsvId(2, KFirstFreeEntryId+35);
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	nonvisEntry.SetId(MaskTMsvId(2, KFirstFreeEntryId+35));
	nonvisEntry.SetVisible(EFalse); //non-visible
	nonvisEntry.SetVisibleFolderFlag(EFalse); //non-visible
	nonvisEntry.SetParent(visEntry.Id());
	nonvisId = KErrNotFound;
	immvisId = MaskTMsvId(2, KFirstFreeEntryId+20);
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(nonvisEntry, immvisId)); //non-visible folder
	ASSERT_TRUE(immvisId == visEntry.Id());
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL(MaskTMsvId(2, KFirstFreeEntryId+9)));
	parentEntry.SetId(MaskTMsvId(2, KFirstFreeEntryId+9));
	parentEntry.SetVisible(EFalse);
	parentEntry.SetVisibleFolderFlag(EFalse);
	parentEntry.SetParent(KMsvRootIndexEntryId);
	visibleFolder = KMsvRootIndexEntryId; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(parentEntry, visibleFolder));
	ASSERT_TRUE(visibleFolder == KMsvRootIndexEntryId);
	
	addEntryPointer.Reset();
	tempId = MaskTMsvId(2, KFirstFreeEntryId+20);
	for(TInt index = 0; index < bulkCount; ++index) //create child entries of parent in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+20));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+20));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, visFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries of visible parent folder
	ASSERT_EQUALS(err, KErrNone);
	
	parentEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+20));
	entryF = Entry();
	entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+20));
	entryF->Entry().SetId (MaskTMsvId(2, KFirstFreeEntryId+9));
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetVisibleFolderFlag(EFalse);
	TRAP(err, visFolder->AddEntryL(entryF, ETrue));
	ASSERT_EQUALS(err, KErrNone);
	nonvisEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+9)); //add the non-visible folder with non-visible parent to cache
	entryF = Entry();
	entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+9));
	entryF->Entry().SetId (MaskTMsvId(2, KFirstFreeEntryId+35)); 
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetVisibleFolderFlag(EFalse);
	TRAP(err, visFolder->AddEntryL(entryF, ETrue));
	ASSERT_EQUALS(err, KErrNone);
	pCacheEntry = entryF;
	
	addEntryPointer.Reset();
	tempId = MaskTMsvId(2, KFirstFreeEntryId+20);
	for(TInt index = 0; index < 5; ++index) //create child entries of non-visible folder in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(2, KFirstFreeEntryId+35));
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	RArray<TMsvId> *temp = NULL;
	TRAP_IGNORE(temp = new(ELeave)RArray<TMsvId>);
	for (TInt index = 0; index < 5; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(2, KFirstFreeEntryId+35));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		temp->Append(id); //append children to child array of non-visible folder
		}
	pCacheEntry->SetChildIdArray(temp);
	TRAP(err, visFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries of visible parent folder
	ASSERT_EQUALS(err, KErrNone);

	getEntryPointer.Reset();
	TRAP(err, ret = visFolder->GetChildrenL(MaskTMsvId(2, KFirstFreeEntryId+35), iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 5); //5 children
	childArr = *(pCacheEntry->ChildIdArray());
	MY_ASSERT_TRUE(childArr.Count() == 5);
	indexTable = visFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 1);
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(2,  KFirstFreeEntryId+155), maxId);
	
	
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete visFolder;
	delete parent;
	addEntryPointer.Close();
	getEntryPointer.Close();
	}



#else		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

void CTestVisibleFolder::InitDb()
	{
	_LIT(KFunction, "InitDb");
	INFO_PRINTF1(KFunction);
	TFileName dbName(KTestDbFileName);

	//Delete database
	RSqlDatabase::Delete(dbName);

	//Create and open the database
	TRAP_IGNORE(iMsvDbAdaptor = CMsvDBAdapter::NewL(dbName));
	
	TInt err =	iFileHandle.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);
	}
	


void CTestVisibleFolder::DeleteDb()
	{
	_LIT(KFunction, "DeleteDb");
	INFO_PRINTF1(KFunction);
	
	if(iMsvDbAdaptor)
		{
		delete iMsvDbAdaptor;
		}
	iMsvDbAdaptor = NULL;
	
	//Delete database
	TFileName dbName(KTestDbFileName);
	RSqlDatabase::Delete(dbName);
	}




/**
 TestGetChildren()
@param None.
@return None.
 
Tests the GetChildrenL() functionality of CMsCacheVisibleFolder Class.
Tested Steps
1. Init

---GetChildren on a visible folder
2. Complete flag = 0. Indextable = NULL, no children in DB
3. Folder not complete, some random entries present in Indextable
4. Folder not complete, >1 blocks, only random entries present in Indextable
5. Folder not complete, >1 blocks, random + immediate child entries in indextable
6. Folder complete, all blocks NULL.
7. Folder complete, 1 block NULL
8. Folder complete, 1 block dirty and has rogue entries
9. Folder complete, children + grandchildren present
10. Folder complete, 1st and last blocks dirty

---GetChildren on a non-visible folder
11. GetChildren when parent of the folder doesn't exist
12. GetChildren when parent is visible folder but not iVisibleFolderId
13. GetChildren on non-visible folder on whom GetChildren hasn't been performed
14. GetChildren on non-visible folder who has no children
15. GetChildren on folder with child array filled and all children present in cache

*/
void CTestVisibleFolder::TestGetChildren()
	{
	_LIT(KFunction, "TestGetChildren");
	INFO_PRINTF1(KFunction);

	// 01. Init
	CMsvCacheEntry* entryF;
	RPointerArray<CMsvCacheEntry> addEntryPointer;
	RPointerArray<CMsvCacheEntry> getEntryPointer;
	TInt releasedEntries;
	TMsvId tempId = (TMsvId)35;
	TMsvId id = 100; //KFirstFreeEntryId;
	TMsvId visibleFolder;
	TInt err;
	TInt bulkCount = 50;
	TBool ret;
	RPointerArray<CMsvCacheIndexTableEntry>* indexTable;
	
	//create a visible folder
	InitDb(); //will not replace the INI files in this
	CMsvCacheVisibleFolder *TestFolder = NULL;
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	TMsvEntry folderEntry;
	folderEntry.SetId(tempId);
	folderEntry.SetVisible(ETrue);
	folderEntry.SetParent(KMsvRootIndexEntryId);
	visibleFolder = KErrNotFound; //or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder));
	
	
	// test cases 2-10 are with the folder as the visible folder.
	// remaining are with the folder as non-visible folder.
	
	// 2. Complete flag = 0. Indextable = NULL, no children in DB
	TestFolder->SetComplete(EFalse);
	TRAP(err, ret = TestFolder->GetChildrenL((TMsvId)35, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	//MY_ASSERT_FALSE(TestFolder->IsComplete());
	MY_ASSERT_EQUALS(TestFolder->GetIndexTableEntry()->Count(), 0);
	DeleteDb();
	delete TestFolder;
	
	
	// 3. Folder not complete, some random entries present in Indextable
	TMsvEntry sampleEntry;
	TMsvId minId, maxId;
	id = 100;
	TMsvId sampleId = id;
	InitDb();
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	folderEntry.SetId(tempId);
	folderEntry.SetVisible(ETrue);
	folderEntry.SetParent(KMsvRootIndexEntryId);
	visibleFolder = KErrNotFound; //or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	CMsvCacheVisibleFolder *parent = NULL;
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL((TMsvId)9));
	TMsvEntry rogueParent;
	rogueParent.SetId((TMsvId)9);
	rogueParent.SetVisible(ETrue);
	rogueParent.SetParent(KMsvRootIndexEntryId);
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	for(TInt index = 0; index < bulkCount; ++index) //create the entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)9);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	maxId = sampleId;
	for (TInt index = 0; index < bulkCount - 10; ++index) //(bulkCount - 10) entries in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //random entries
	MY_ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL((TMsvId)35, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 0);
	MY_ASSERT_EQUALS(1, indexTable->Count());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS(maxId - 10, (*indexTable)[0]->GetMaxMsvIdRange());
	
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
	
	
	// 4. Folder not complete, >1 blocks, only random entries present in Indextable
	InitDb();
	id = 100;
	sampleId = id;
	tempId = (TMsvId)35;
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	TestFolder->SetComplete(EFalse);
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL((TMsvId)9));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	for(TInt index = 0; index < 3*bulkCount; ++index) //create entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)9);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	maxId = sampleId;
	addEntryPointer.Reset();
	for (TInt index = 0; index < 2*bulkCount - 5; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //random entries
	MY_ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL((TMsvId)35, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 0); //no children
//	MY_ASSERT_FALSE(TestFolder->IsComplete());
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(2, indexTable->Count());
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_TRUE((*indexTable)[1]->IsGrandChildPresent());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)147, maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)148, minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)195, maxId);
	
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
		
	
	// 5. Folder not complete, >1 blocks, random + immediate child entries in indextable
	InitDb();
	id = 100;
	sampleId = id;
	tempId = (TMsvId)35;
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	TestFolder->SetComplete(EFalse);
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL((TMsvId)9));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	for(TInt index = 0; index < bulkCount; ++index) //create rogue entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)9);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	addEntryPointer.Reset();
	for (TInt index = 0; index < bulkCount; ++index) //rogue entries in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //rogue entries
	ASSERT_EQUALS(err, KErrNone);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 2*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)35);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 2*bulkCount - 10; ++index) //10 less children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)35);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL((TMsvId)35, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 2*bulkCount); //2*bulkCount children
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 2);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)200, maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)201, minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)250, maxId);
		
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
		
	
	// 6. Folder complete, all blocks NULL.
	//add children to DB and get them to cache. then release all blocks to make them NULL
	InitDb();
	id = 100;
	sampleId = id;
	tempId = (TMsvId)35;
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL((TMsvId)9));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)35);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount - 10; ++index) //10 less children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)35);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL((TMsvId)35, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 3*bulkCount); //bulkCount children
	TestFolder->ReleaseAllBlocks(releasedEntries);
	MY_ASSERT_EQUALS(releasedEntries, 3*bulkCount);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_TRUE(TestFolder->IsAllBlocksDirty());
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL((TMsvId)35, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 3 * bulkCount); //150 children
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 3); //3 blocks still
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)150, maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)151, minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)200, maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)201, minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)250, maxId);
			
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
		
	
	// 7. Folder complete, 1 block NULL
	//add children in DB and cache. release last block to make it NULL.
	InitDb();
	id = 100;
	sampleId = id;
	tempId = (TMsvId)35;
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL((TMsvId)9));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)35);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)35);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	TestFolder->SetComplete(ETrue);
	indexTable = TestFolder->GetIndexTableEntry();
	(*indexTable)[2]->ReleaseBlock();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	MY_ASSERT_TRUE((*indexTable)[0]->BlockPtr() != NULL);
	MY_ASSERT_TRUE((*indexTable)[1]->BlockPtr() != NULL);
	MY_ASSERT_TRUE((*indexTable)[2]->BlockPtr() == NULL);
	MY_ASSERT_TRUE((*indexTable)[2]->IsDirty());
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL((TMsvId)35, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 3*bulkCount); //150 entries in 3 blocks
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)150, maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)151, minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)200, maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)201, minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)250, maxId);
			
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
	
	
	// 8. Folder complete, 1 block dirty and has rogue entries
	InitDb();
	id = 100;
	sampleId = id;
	tempId = (TMsvId)35;
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL((TMsvId)9));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)35);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)35);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	TestFolder->SetComplete(ETrue);
	(*indexTable)[2]->ReleaseBlock();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	MY_ASSERT_TRUE((*indexTable)[0]->BlockPtr() != NULL);
	MY_ASSERT_TRUE((*indexTable)[1]->BlockPtr() != NULL);
	MY_ASSERT_TRUE((*indexTable)[2]->BlockPtr() == NULL);
	MY_ASSERT_TRUE((*indexTable)[2]->IsDirty());
	addEntryPointer.Reset();
	for(TInt index = 0; index < 10; ++index) //create rogue entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)9);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	addEntryPointer.Reset();
	for (TInt index = 0; index < 10; ++index) //rogue entries in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)9);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //rogue entries
	ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL((TMsvId)35, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 3*bulkCount); //150 children
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)150, maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)151, minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)200, maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)201, minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)260, maxId); //rogue entries have changed iMaxMsvId
			
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
	
	
	// 9. Folder complete, children + grandchildren present
	//Add children and grandchildren. Perform GetChildren on the folder.
	InitDb();
	id = 100;
	sampleId = id;
	tempId = (TMsvId)35;
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL((TMsvId)9));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)35);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)35);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	TestFolder->SetComplete(ETrue);
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL((TMsvId)35, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 3*bulkCount);
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	addEntryPointer.Reset();
	for(TInt index = 0; index < 5; ++index) //create grandchild entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)250);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 5; ++index) //all grandchildren in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)250);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //grandchild entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	MY_ASSERT_TRUE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL((TMsvId)35, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 3*bulkCount); //150 children
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)150, maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)151, minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)200, maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)201, minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)255, maxId); //grandchild entries change iMaxMsvId in the last block
	
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
	
	
	// 10. Folder complete, 1st and last blocks dirty
	InitDb();
	id = 100;
	sampleId = id;
	tempId = (TMsvId)35;
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(folderEntry, visibleFolder)); //create folder entry in DB
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL((TMsvId)9));
	visibleFolder = KErrNotFound; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(rogueParent, visibleFolder)); //create parent for rogue entries in DB
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)35);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)35);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	TestFolder->SetComplete(ETrue);
	(*indexTable)[0]->ReleaseBlock(); //release first and last blocks
	(*indexTable)[2]->ReleaseBlock();
	ASSERT_TRUE((*indexTable)[0]->IsDirty());
	ASSERT_TRUE((*indexTable)[2]->IsDirty());
	getEntryPointer.Reset();
	TRAP(err, ret = TestFolder->GetChildrenL((TMsvId)35, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 3*bulkCount); //150 children
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)150, maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)151, minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)200, maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)201, minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)250, maxId);
	
	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete parent;
	
	
	
	// test cases 11-   are with the folder as a non-visible folder.
	//Create a non-visible folder with a visible folder parent.
	RArray<TMsvId> childArr;
	CMsvCacheEntry *pCacheEntry = NULL;
	InitDb();
	id = 100;
	sampleId = id;
	
	CMsvCacheVisibleFolder *visFolder = NULL;
	TRAP_IGNORE(visFolder = CMsvCacheVisibleFolder::NewL((TMsvId)20));
	TMsvEntry visEntry;
	visEntry.SetId((TMsvId)20);
	visEntry.SetVisible(ETrue); //visible
	visEntry.SetParent(KMsvRootIndexEntryId);
	TMsvId immvisId = KErrNotFound;
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(visEntry, immvisId)); //visible folder, parent of below non-visible folder
	
	tempId = (TMsvId)35;
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	TMsvEntry nonvisEntry;
	nonvisEntry.SetId((TMsvId)35);
	nonvisEntry.SetVisible(EFalse); //non-visible
	nonvisEntry.SetParent(visEntry.Id());
	TMsvId nonvisId = KErrNotFound;
	immvisId = visEntry.Id();
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(nonvisEntry, immvisId)); //non-visible folder
	ASSERT_TRUE(immvisId == visEntry.Id());
	
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL((TMsvId)9));
	TMsvEntry parentEntry;
	parentEntry.SetId((TMsvId)9);
	parentEntry.SetVisible(EFalse);
	parentEntry.SetParent(KMsvRootIndexEntryId);
	visibleFolder = KErrNotFound; // or Root
	visibleFolder = KMsvRootIndexEntryId;
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(parentEntry, visibleFolder));
	ASSERT_TRUE(visibleFolder == KMsvRootIndexEntryId);
	
	
	// 11. GetChildren when parent of the folder doesn't exist
	nonvisEntry.SetParent((TMsvId)40); //40 doesn't exist, set it for this test
	getEntryPointer.Reset();
	TRAP(err, ret = visFolder->GetChildrenL(tempId, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	
	// 12. GetChildren when parent is visible folder but not iVisibleFolderId
	nonvisEntry.SetParent((TMsvId)9); //TMsvId of parent
	entryF = Entry();
	entryF->Entry().SetParent((TMsvId)9);
	entryF->Entry().SetId((TMsvId)35);
	TRAP(err, TestFolder->AddEntryL(entryF, ETrue));
	ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	ret = EFalse;
	TRAP(err, ret = visFolder->GetChildrenL(tempId, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	MY_ASSERT_FALSE(ret);
	
	DeleteDb();
	delete TestFolder;
	delete parent;
	delete visFolder;
	entryF = NULL;
	

	// 13. GetChildren on non-visible folder on whom GetChildren hasn't been performed
	InitDb();
	id = 100;
	sampleId = id;
	TRAP_IGNORE(visFolder = CMsvCacheVisibleFolder::NewL((TMsvId)20));
	visEntry.SetId((TMsvId)20);
	visEntry.SetVisible(ETrue); //visible
	visEntry.SetParent(KMsvRootIndexEntryId);
	immvisId = KErrNotFound;
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(visEntry, immvisId)); //visible folder, parent of below non-visible folder
	tempId = (TMsvId)35;
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	nonvisEntry.SetId((TMsvId)35);
	nonvisEntry.SetVisible(EFalse); //non-visible
	nonvisEntry.SetVisibleFolderFlag(EFalse); //non-visible
	nonvisEntry.SetParent(visEntry.Id());
	nonvisId = KErrNotFound;
	immvisId = KMsvRootIndexEntryId;
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(nonvisEntry, immvisId)); //non-visible folder
	//ASSERT_TRUE(immvisId == visEntry.Id());
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL((TMsvId)9));
	parentEntry.SetId((TMsvId)9);
	parentEntry.SetVisible(EFalse);
	parentEntry.SetVisibleFolderFlag(EFalse);
	parentEntry.SetParent(KMsvRootIndexEntryId);
	visibleFolder = KMsvRootIndexEntryId; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(parentEntry, visibleFolder));
	ASSERT_TRUE(visibleFolder == KMsvRootIndexEntryId);
	addEntryPointer.Reset();
	tempId = (TMsvId)20;
	for(TInt index = 0; index < bulkCount; ++index) //create child entries of parent in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)20);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)20);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, visFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries of visible parent folder
	ASSERT_EQUALS(err, KErrNone);
	
	nonvisEntry.SetParent((TMsvId)9); //add the non-visible folder with non-visible parent to cache
	parentEntry.SetParent((TMsvId)20);
	entryF = Entry();
	entryF->Entry().SetParent((TMsvId)20);
	entryF->Entry().SetId ((TMsvId)9);
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetVisibleFolderFlag(EFalse);
	TRAP(err, visFolder->AddEntryL(entryF, ETrue));
	ASSERT_EQUALS(err, KErrNone);
	entryF = Entry();
	entryF->Entry().SetParent((TMsvId)9);
	entryF->Entry().SetId ((TMsvId)35); 
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetVisibleFolderFlag(EFalse);
	TRAP(err, visFolder->AddEntryL(entryF, ETrue));
	ASSERT_EQUALS(err, KErrNone);
	pCacheEntry = entryF;
	
	addEntryPointer.Reset();
	//tempId = (TMsvId)35;
	tempId = (TMsvId)20;
	for(TInt index = 0; index < 5; ++index) //create child entries of non-visible folder in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)35);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	TRAP(err, visFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries of non-visible folder
	ASSERT_EQUALS(err, KErrNone); //(bulkCount + 2 + 5) entries added till now
	indexTable = visFolder->GetIndexTableEntry();
	getEntryPointer.Reset();
	TRAP(err, ret = visFolder->GetChildrenL((TMsvId)35, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 5); //5 children
	MY_ASSERT_EQUALS(indexTable->Count(), 1);
	childArr = *(pCacheEntry->ChildIdArray());
	MY_ASSERT_EQUALS(childArr.Count(), 5);
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)155, maxId); //non-visible folder's children have maximum TMsvId

	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete visFolder;
	delete parent;
	

	// 14. GetChildren on non-visible folder who has no children
	InitDb();
	id = 100;
	sampleId = id;
	TRAP_IGNORE(visFolder = CMsvCacheVisibleFolder::NewL((TMsvId)20));
	visEntry.SetId((TMsvId)20);
	visEntry.SetVisible(ETrue); //visible
	visEntry.SetParent(KMsvRootIndexEntryId);
	immvisId = KMsvRootIndexEntryId;
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(visEntry, immvisId)); //visible folder, parent of below non-visible folder
	tempId = (TMsvId)35;
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	nonvisEntry.SetId((TMsvId)35);
	nonvisEntry.SetVisible(EFalse); //non-visible
	nonvisEntry.SetVisibleFolderFlag(EFalse); //non-visible
	nonvisEntry.SetParent(visEntry.Id());
	nonvisId = KErrNotFound;
	immvisId = (TMsvId)20;
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(nonvisEntry, immvisId)); //non-visible folder
//	ASSERT_TRUE(immvisId == visEntry.Id());
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL((TMsvId)9));
	parentEntry.SetId((TMsvId)9);
	parentEntry.SetVisible(EFalse);
	parentEntry.SetVisibleFolderFlag(EFalse);
	parentEntry.SetParent(KMsvRootIndexEntryId);
	visibleFolder = KMsvRootIndexEntryId; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(parentEntry, visibleFolder));
	ASSERT_TRUE(visibleFolder == KMsvRootIndexEntryId);
	
	addEntryPointer.Reset();
	tempId = (TMsvId)20;
	for(TInt index = 0; index < bulkCount; ++index) //create child entries of parent in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)20);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)20);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, visFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries of visible parent folder
	ASSERT_EQUALS(err, KErrNone);
	parentEntry.SetParent((TMsvId)20);
	entryF = Entry();
	entryF->Entry().SetParent((TMsvId)20);
	entryF->Entry().SetId ((TMsvId)9);
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetVisibleFolderFlag(EFalse);
	TRAP(err, visFolder->AddEntryL(entryF, ETrue));
	ASSERT_EQUALS(err, KErrNone);
	nonvisEntry.SetParent((TMsvId)9); //add the non-visible folder with non-visible parent to cache
	entryF = Entry();
	entryF->Entry().SetParent((TMsvId)9);
	entryF->Entry().SetId ((TMsvId)35);
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetVisibleFolderFlag(EFalse);
	TRAP(err, visFolder->AddEntryL(entryF, ETrue));
	ASSERT_EQUALS(err, KErrNone);
	pCacheEntry = entryF;

	getEntryPointer.Reset();
	TRAP(err, ret = visFolder->GetChildrenL((TMsvId)35, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 0); //no children
	childArr = *(pCacheEntry->ChildIdArray());
	MY_ASSERT_TRUE(childArr.Count() == 0);
	indexTable = visFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 1);
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)150, maxId);

	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete visFolder;
	delete parent;
	

	// 15. GetChildren on folder with child array filled and all children present in cache
	InitDb();
	id = 100;
	sampleId = id;
	TRAP_IGNORE(visFolder = CMsvCacheVisibleFolder::NewL((TMsvId)20));
	visEntry.SetId((TMsvId)20);
	visEntry.SetVisible(ETrue); //visible
	visEntry.SetParent(KMsvRootIndexEntryId);
	immvisId = KMsvRootIndexEntryId;
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(visEntry, immvisId)); //visible folder, parent of below non-visible folder
	tempId = (TMsvId)35;
	TRAP_IGNORE(TestFolder = CMsvCacheVisibleFolder::NewL(tempId));
	nonvisEntry.SetId((TMsvId)35);
	nonvisEntry.SetVisible(EFalse); //non-visible
	nonvisEntry.SetVisibleFolderFlag(EFalse); //non-visible
	nonvisEntry.SetParent(visEntry.Id());
	nonvisId = KErrNotFound;
	immvisId = (TMsvId)20;
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(nonvisEntry, immvisId)); //non-visible folder
	ASSERT_TRUE(immvisId == visEntry.Id());
	TRAP_IGNORE(parent = CMsvCacheVisibleFolder::NewL((TMsvId)9));
	parentEntry.SetId((TMsvId)9);
	parentEntry.SetVisible(EFalse);
	parentEntry.SetVisibleFolderFlag(EFalse);
	parentEntry.SetParent(KMsvRootIndexEntryId);
	visibleFolder = KMsvRootIndexEntryId; // or Root
	TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(parentEntry, visibleFolder));
	ASSERT_TRUE(visibleFolder == KMsvRootIndexEntryId);
	
	addEntryPointer.Reset();
	tempId = (TMsvId)20;
	for(TInt index = 0; index < bulkCount; ++index) //create child entries of parent in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)20);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)20);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, visFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries of visible parent folder
	ASSERT_EQUALS(err, KErrNone);
	
	parentEntry.SetParent((TMsvId)20);
	entryF = Entry();
	entryF->Entry().SetParent((TMsvId)20);
	entryF->Entry().SetId ((TMsvId)9);
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetVisibleFolderFlag(EFalse);
	TRAP(err, visFolder->AddEntryL(entryF, ETrue));
	ASSERT_EQUALS(err, KErrNone);
	nonvisEntry.SetParent((TMsvId)9); //add the non-visible folder with non-visible parent to cache
	entryF = Entry();
	entryF->Entry().SetParent((TMsvId)9);
	entryF->Entry().SetId ((TMsvId)35); 
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetVisibleFolderFlag(EFalse);
	TRAP(err, visFolder->AddEntryL(entryF, ETrue));
	ASSERT_EQUALS(err, KErrNone);
	pCacheEntry = entryF;
	
	addEntryPointer.Reset();
	tempId = (TMsvId)20;
	for(TInt index = 0; index < 5; ++index) //create child entries of non-visible folder in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)35);
		TRAP_IGNORE(iMsvDbAdaptor->CreateEntryL(sampleEntry, tempId));
		}
	RArray<TMsvId> *temp = NULL;
	TRAP_IGNORE(temp = new(ELeave)RArray<TMsvId>);
	for (TInt index = 0; index < 5; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)35);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		temp->Append(id); //append children to child array of non-visible folder
		}
	pCacheEntry->SetChildIdArray(temp);
	TRAP(err, visFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries of visible parent folder
	ASSERT_EQUALS(err, KErrNone);

	getEntryPointer.Reset();
	TRAP(err, ret = visFolder->GetChildrenL((TMsvId)35, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	MY_ASSERT_EQUALS(getEntryPointer.Count(), 5); //5 children
	childArr = *(pCacheEntry->ChildIdArray());
	MY_ASSERT_TRUE(childArr.Count() == 5);
	indexTable = visFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 1);
	MY_ASSERT_TRUE((*indexTable)[0]->IsGrandChildPresent());
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)155, maxId);


	entryF = NULL;
	DeleteDb();
	delete TestFolder;
	delete visFolder;
	delete parent;
	}

#endif 		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)


/**
 TestDeleteEntry()
@param None.
@return None.
 
Tests the DeleteEntry() functionality of CMsCacheVisibleFolder Class.
Tested Steps
01. Init
02. Delete an Entry When IndexTable is not created
03. Delete an Entry which does not Exist
04. Delete an Entry which is a child of the visible folder
05. Delete an Entry which is a Grand child of the visible folder
06. Delete an Entry which is a Great Grand child of the visible folder
07. Delete an Entry which is the last Entry in the List
*/
void CTestVisibleFolder::TestDeleteEntry()
	{
	_LIT(KFunction, "TestDeleteEntry");
	INFO_PRINTF1(KFunction);

	// 01. Init
	CMsvCacheEntry* entryF;
	RPointerArray<CMsvCacheEntry> addEntryPointer;
	RPointerArray<CMsvCacheEntry> getEntryPointer;
	TMsvId tempId;
	
	// 02. Delete an Entry When IndexTable is not created
	TRAPD(err,iFolderPointer[1]->DeleteEntryL((TMsvId)06));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	// 03. Delete an Entry which does not Exist
	AddTestEntries();
	MY_ASSERT_FALSE(iFolderPointer[1]->EntryExists((TMsvId)06));
	TRAP(err,iFolderPointer[1]->DeleteEntryL((TMsvId)06));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	// 03. Delete an Entry which Exists but not locked
	MY_ASSERT_TRUE(iFolderPointer[1]->EntryExists((TMsvId)05));
	TRAP(err,iFolderPointer[1]->DeleteEntryL((TMsvId)05));
	MY_ASSERT_TRUE(iFolderPointer[1]->EntryExists((TMsvId)05));
	MY_ASSERT_EQUALS(err, KErrAccessDenied);
	
	// 04. Delete an Entry which Exists and locked
	aEntryA->LockEntry();
	MY_ASSERT_TRUE(iFolderPointer[1]->EntryExists((TMsvId)05));
	TRAP_IGNORE(iFolderPointer[1]->DeleteEntryL((TMsvId)05));
	MY_ASSERT_FALSE(iFolderPointer[1]->EntryExists((TMsvId)05));
	
	// 05. Delete an Entry which is a Grand child of the visible folder
	entryF = Entry();
	entryF->Entry().SetParent((TMsvId)30);
	entryF->Entry().SetId ((TMsvId)05);
	
	TRAP_IGNORE(iFolderPointer[1]->AddEntryL(entryF));
	
	aEntryB->Entry().SetVisible(EFalse);
	aEntryB->Entry().SetVisibleFolderFlag(EFalse);
	
	for (TInt index = 10; index < 12; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)07);
		entryF->Entry().SetId ((TMsvId)index);
		entryF->LockEntry();
		entryF->Entry().SetVisible(EFalse);
		entryF->Entry().SetVisibleFolderFlag(EFalse);
		addEntryPointer.Append(entryF);
		}
	
	iFolderPointer[1]->SetComplete(ETrue);
	
	TRAP_IGNORE(iFolderPointer[1]->UpdateChildMsvIdsL(addEntryPointer));
	TRAP_IGNORE(iFolderPointer[1]->AddEntryListL(addEntryPointer,EFalse));
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[1]->GetChildrenL((TMsvId)07, NULL, getEntryPointer)));
	tempId = 11;
	for(TInt index = 0; index < getEntryPointer.Count(); ++index)
		{
		ASSERT_EQUALS(tempId, getEntryPointer[index]->Entry().Id());
		--tempId;
		}
	
	TRAP_IGNORE(iFolderPointer[1]->DeleteEntryL((TMsvId)11));
	MY_ASSERT_FALSE(iFolderPointer[1]->EntryExists((TMsvId)11));

	getEntryPointer.Reset();
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[1]->GetChildrenL((TMsvId)07, NULL, getEntryPointer)));
	tempId = 10;
	for(TInt index = 0; index < getEntryPointer.Count(); ++index)
		{
		ASSERT_EQUALS(tempId, getEntryPointer[index]->Entry().Id());
		--tempId;
		}
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	
	// 06. Delete an Entry which is a Great Grand child of the visible folder
	for (TInt index = 10; index < 12; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)07);
		entryF->Entry().SetId ((TMsvId)index);
		entryF->LockEntry();
		entryF->Entry().SetVisible(EFalse);
		entryF->Entry().SetVisibleFolderFlag(EFalse);
		addEntryPointer.Append(entryF);
		}
	
	TRAP_IGNORE(iFolderPointer[1]->UpdateChildMsvIdsL(addEntryPointer));
	TRAP_IGNORE(iFolderPointer[1]->AddEntryListL(addEntryPointer,EFalse));
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[1]->GetChildrenL((TMsvId)07, NULL, getEntryPointer)));
	tempId = 11;
	for(TInt index = 0; index < getEntryPointer.Count(); ++index)
		{
		ASSERT_EQUALS(tempId, getEntryPointer[index]->Entry().Id());
		--tempId;
		}
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	
	for (TInt index = 12; index < 14; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)10);
		entryF->Entry().SetId ((TMsvId)index);
		entryF->LockEntry();
		entryF->Entry().SetVisible(EFalse);
		entryF->Entry().SetVisibleFolderFlag(EFalse);
		addEntryPointer.Append(entryF);
		}
	
	TRAP_IGNORE(iFolderPointer[1]->UpdateChildMsvIdsL(addEntryPointer));
	TRAP_IGNORE(iFolderPointer[1]->AddEntryListL(addEntryPointer,EFalse));
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[1]->GetChildrenL((TMsvId)10, NULL, getEntryPointer)));
	tempId = 13;
	for(TInt index = 0; index < getEntryPointer.Count(); ++index)
		{
		ASSERT_EQUALS(tempId, getEntryPointer[index]->Entry().Id());
		--tempId;
		}
	
	TRAP_IGNORE(iFolderPointer[1]->DeleteEntryL((TMsvId)12));
	MY_ASSERT_FALSE(iFolderPointer[1]->EntryExists((TMsvId)12));

	getEntryPointer.Reset();
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[1]->GetChildrenL((TMsvId)10, NULL, getEntryPointer)));
	tempId = 13;
	for(TInt index = 0; index < getEntryPointer.Count(); ++index)
		{
		ASSERT_EQUALS(tempId, getEntryPointer[index]->Entry().Id());
		--tempId;
		}
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	
	// 07. Delete an Entry which is the last Entry in the List
	RPointerArray<CMsvCacheIndexTableEntry>* indexTable;
	iFolderPointer[1]->SetComplete(EFalse);
	MY_ASSERT_FALSE(iFolderPointer[1]->IsComplete());
	indexTable =iFolderPointer[1]->GetIndexTableEntry();
	MY_ASSERT_TRUE((*indexTable)[0]->ReleaseBlock (ETrue));
	
	entryF = Entry();
	entryF->Entry().SetParent((TMsvId)30);
	entryF->Entry().SetId ((TMsvId)05);
	entryF->LockEntry();
	TRAP_IGNORE(iFolderPointer[1]->AddEntryL(entryF));
	
	TRAP_IGNORE(iFolderPointer[1]->DeleteEntryL((TMsvId)05));
	ASSERT_FALSE(iFolderPointer[1]->EntryExists((TMsvId)05));
	indexTable =iFolderPointer[1]->GetIndexTableEntry();
	ASSERT_EQUALS(0,indexTable->Count());
	
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	
	//08. Delete all entries in the first block and observe range changes in second block
	//Complete Flag is False
	for (TInt index = 10; index < 138; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		entryF->LockEntry();
		addEntryPointer.Append(entryF);
		}
	
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	tempId = 10;
	for(TInt index = 0; index < getEntryPointer.Count(); ++index)
		{
		ASSERT_EQUALS(tempId, getEntryPointer[index]->Entry().Id());
		++tempId;
		}

	indexTable =iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)73, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)74, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)137, (*indexTable)[1]->GetMaxMsvIdRange());
	
	iFolderPointer[0]->SetComplete(EFalse);
	
	for (TInt index = 10; index < 74; ++index)
		{
		TRAP_IGNORE(iFolderPointer[0]->DeleteEntryL((TMsvId)index));
		}

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)137, (*indexTable)[0]->GetMaxMsvIdRange());
	getEntryPointer.Reset();
	entryF=NULL;
	
	getEntryPointer.Close();
	addEntryPointer.Close();
	}


/**
 TestDeleteEntry()
@param None.
@return None.
 
Tests the TestGetEntry() functionality of CMsCacheVisibleFolder Class.
Tested Steps
01. Init
02. Delete a set of Entries when IndexTable is not created
03. Delete a set of Entries which contains no non-existant entries
04. Delete a set of Entries which contains one non-existant entry
05. Delete a set of Entries which contains all non-existant entries
06. Delete a set of Entries which contains one non-Locked entry.
07. Delete Successful
*/
void CTestVisibleFolder::TestDeleteEntryList()
	{
	_LIT(KFunction, "TestDeleteEntryList");
	INFO_PRINTF1(KFunction);
	
	//01. Init
	CMsvEntrySelection* aEntrySelection = NULL;
	TRAP_IGNORE(aEntrySelection = new (ELeave)CMsvEntrySelection);
	RPointerArray<CMsvCacheIndexTableEntry>* indexTable;
	RPointerArray<CMsvCacheEntry> addEntryPointer;
	CMsvCacheEntry* entryF;
	
	// 02. Delete a set of Entries when IndexTable is not created
	TRAP_IGNORE(aEntrySelection->AppendL((TMsvId)07));
	TRAPD(err,iFolderPointer[0]->DeleteEntryListL(*aEntrySelection));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	// 03.Delete a set of Entries which contains no non-existant entries
	aEntrySelection->Reset();
	
	for (TInt index = 10; index < 20; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		entryF->LockEntry();
		addEntryPointer.Append(entryF);	
		TRAP_IGNORE(aEntrySelection->AppendL((TMsvId)index));
		}
	
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));


	TRAP_IGNORE(iFolderPointer[0]->DeleteEntryListL(*aEntrySelection));
	for (TInt index = 10; index < 20; ++index)
		{
		MY_ASSERT_FALSE(iFolderPointer[0]->EntryExists((TMsvId)index));
		}
	
	delete iFolderPointer[0];
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));
	
	// 04. Delete a set of Entries which contains one non-existant entry
	aEntrySelection->Reset();
	addEntryPointer.Reset();
	
	for (TInt index = 10; index < 20; ++index)
		{
		if(index != 15)
			{
			entryF = Entry();
			entryF->Entry().SetParent((TMsvId)5);
			entryF->Entry().SetId ((TMsvId)index);
			entryF->LockEntry();
			addEntryPointer.Append(entryF);	
			}
		TRAP_IGNORE(aEntrySelection->AppendL((TMsvId)index));
		}
	
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));

	
	CMsvCacheEntry *entry = NULL;
	TRAP(err,iFolderPointer[0]->DeleteEntryListL(*aEntrySelection));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	(*indexTable)[0]->ReleaseBlock(ETrue);
	
	delete iFolderPointer[0];
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));
	
	// 05. Delete a set of Entries which contains all non-existant entries
	aEntrySelection->Reset();
	addEntryPointer.Reset();
	
	for (TInt index = 10; index < 20; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		entryF->LockEntry();
		addEntryPointer.Append(entryF);	
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	
	aEntrySelection->Reset();
	TRAP_IGNORE(aEntrySelection->AppendL((TMsvId)15));
	TRAP_IGNORE(aEntrySelection->AppendL((TMsvId)17));
	TRAP_IGNORE(aEntrySelection->AppendL((TMsvId)20));
		
	TRAP(err,iFolderPointer[0]->DeleteEntryListL(*aEntrySelection));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	(*indexTable)[0]->ReleaseBlock(ETrue);
	
	delete iFolderPointer[0];
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));
	
	// 06. Delete a set of Entries which contains one non-Locked entry.
	aEntrySelection->Reset();
	addEntryPointer.Reset();
	
	for (TInt index = 10; index < 20; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		if(index != 15)
			{
			entryF->LockEntry();	
			}
		addEntryPointer.Append(entryF);	
		TRAP_IGNORE(aEntrySelection->AppendL((TMsvId)index));
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	

	TRAP(err,iFolderPointer[0]->DeleteEntryListL(*aEntrySelection));
	MY_ASSERT_EQUALS(err, KErrAccessDenied);
	
	for (TInt index = 10; index < 20; ++index)
		{
		if(index < 15)
			{
			MY_ASSERT_FALSE(iFolderPointer[0]->EntryExists((TMsvId)index));	
			}
		else
			{
			MY_ASSERT_TRUE(iFolderPointer[0]->EntryExists((TMsvId)index));
			}
		}
		
		
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	(*indexTable)[0]->ReleaseBlock(ETrue);
	
	delete iFolderPointer[0];
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));
	
	delete aEntrySelection;
	addEntryPointer.Close();
	}
	

/**
 TestReleaseAllBlocks()
@param None.
@return None.
 
Tests the ReleaseAllBlocks() functionality of CMsCacheVisibleFolder Class.
Tested Steps
01. Init
02. Release Entries when IndexTable is not created  and IsComplete Flag is Set.
03. Release Entries which contains no Locked entries and IsComplete Flag is Set.
04. Release Entries which contains one Locked entry and IsComplete Flag is Set.
05. Release Entries which contains no Locked entries and IsComplete Flag is Clear.
06. Release Entries which contains one Locked entry and IsComplete Flag is Clear.
*/
void CTestVisibleFolder::TestReleaseAllBlocks()
	{
	_LIT(KFunction, "TestReleaseAllBlocks");
	INFO_PRINTF1(KFunction);
	
	// 01. Init
	CMsvCacheEntry* entryF;
	RPointerArray<CMsvCacheEntry> addEntryPointer;
	RPointerArray<CMsvCacheEntry> getEntryPointer;
	TInt releasedEntries;
	TMsvId tempId;
	
	// 02. Release Entries when IndexTable is not created  and IsComplete Flag is Set.
	MY_ASSERT_TRUE(iFolderPointer[1]->ReleaseAllBlocks (releasedEntries));
	
	// 03. Release Entries which contains no Locked entries and IsComplete Flag is Set.
	for (TInt index = 10; index < 155; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	tempId = 10;
	for(TInt index = 0; index < getEntryPointer.Count(); ++index)
		{
		ASSERT_EQUALS(tempId, getEntryPointer[index]->Entry().Id());
		++tempId;
		}
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(145,releasedEntries);
	ASSERT_FALSE(iFolderPointer[1]->EntryExists((TMsvId)40));//Random Entry in First Block
	ASSERT_FALSE(iFolderPointer[1]->EntryExists((TMsvId)75));//Random Entry in second Block
	ASSERT_FALSE(iFolderPointer[1]->EntryExists((TMsvId)150));//Random Entry in Thrid Block

	getEntryPointer.Reset();
	addEntryPointer.Reset();
	entryF=NULL;
	
	// 04. Release Entries which contains no Locked entries and IsComplete Flag is Clear.
	for (TInt index = 10; index < 155; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));

	for(TInt index = 0; index < getEntryPointer.Count(); ++index)
		{
		ASSERT_TRUE(iFolderPointer[0]->EntryExists(getEntryPointer[index]->Entry().Id()));
		}
	
	iFolderPointer[0]->SetComplete(EFalse);
	
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(145,releasedEntries);
	ASSERT_FALSE(iFolderPointer[0]->EntryExists((TMsvId)40));//Random Entry in First Block
	ASSERT_FALSE(iFolderPointer[0]->EntryExists((TMsvId)75));//Random Entry in First Block
	ASSERT_FALSE(iFolderPointer[0]->EntryExists((TMsvId)150));//Random Entry in Thrid Block

	getEntryPointer.Reset();
	addEntryPointer.Reset();
	entryF=NULL;
	
	// 05. Release Entries which contains one Locked entry and IsComplete Flag is Set.
	for (TInt index = 10; index < 155; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		if(index == 75)
			{
			entryF->LockEntry();
			}
		addEntryPointer.Append(entryF);
		}
	
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	tempId = 10;
	for(TInt index = 0; index < getEntryPointer.Count(); ++index)
		{
		ASSERT_EQUALS(tempId, getEntryPointer[index]->Entry().Id());
		++tempId;
		}
	ASSERT_FALSE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(97,releasedEntries);
	ASSERT_FALSE(iFolderPointer[0]->EntryExists((TMsvId)40));//Random Entry in First Block
	MY_ASSERT_TRUE(iFolderPointer[0]->EntryExists((TMsvId)75));//Entry in second Block which is not locked
	ASSERT_FALSE(iFolderPointer[0]->EntryExists((TMsvId)150));//Random Entry in Thrid Block

	getEntryPointer.Reset();
	addEntryPointer.Reset();
	entryF=NULL;
	
	// 06. Release Entries which contains one Locked entry and IsComplete Flag is Clear.
	for (TInt index = 10; index < 155; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		if(index == 75)
			{
			entryF->LockEntry();
			}
		addEntryPointer.Append(entryF);
		}
	
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	tempId = 10;
	for(TInt index = 0; index < getEntryPointer.Count(); ++index)
		{
		ASSERT_EQUALS(tempId, getEntryPointer[index]->Entry().Id());
		++tempId;
		}
	
	iFolderPointer[0]->SetComplete(EFalse);
	
	ASSERT_FALSE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(97,releasedEntries);
	ASSERT_FALSE(iFolderPointer[0]->EntryExists((TMsvId)40));//Random Entry in First Block
	MY_ASSERT_TRUE(iFolderPointer[0]->EntryExists((TMsvId)75));//Entry in second Block which is not locked
	ASSERT_FALSE(iFolderPointer[0]->EntryExists((TMsvId)150));//Random Entry in Thrid Block

	getEntryPointer.Reset();
	entryF=NULL;
	
	addEntryPointer.Close();
	getEntryPointer.Close();
	}

/**
 TestReleaseBlocks()
@param None.
@return None.
 
Tests the ReleaseBlocks() functionality of CMsCacheVisibleFolder Class.
Tested Steps
01. Init
02. Release Entries when IndexTable is not created  and IsComplete Flag is Set.
COMPLETE FLAG is SET
03. Release Blocks based on access time; the least used block is released first and No Entries are locked.
04. Release Blocks based on access time; the least used block is released first and Few Entries are locked.
05. Release Blocks based on access time; the least used block is released first and All Entries are locked.
COMPLETE FLAG is CLEAR
06. Release Blocks based on access time; the least used block is released first and No Entries are locked.
07. Release Blocks based on access time; the least used block is released first and Few Entries are locked.
*/
void CTestVisibleFolder::TestReleaseBlocks()
	{
	_LIT(KFunction, "TestReleaseBlocks");
	INFO_PRINTF1(KFunction);
	
	// 01. Init
	CMsvCacheEntry* entryF;
	RPointerArray<CMsvCacheEntry> addEntryPointer;
	RPointerArray<CMsvCacheEntry> getEntryPointer;
	TBool isFolderEmpty;
	TInt numberOfEntriesToRelease;
	TInt releasedEntries;
	TTime accessTime;
	RPointerArray<CMsvCacheIndexTableEntry>* indexTable;
	TInt err;
	TInt ret;

	
	// 02. Release Entries when IndexTable is not created  and IsComplete Flag is Set.
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease = 10, isFolderEmpty);
	MY_ASSERT_TRUE(isFolderEmpty);
	MY_ASSERT_EQUALS(0, releasedEntries);
	
	/**** COMPLETE FLAG IS SET ****/
	//03. Release Blocks based on access time; the least used block is released first and No Entries are locked.
	for (TInt index = 10; index < 300; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	
	TRAP(err, iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	MY_ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = iFolderPointer[0]->GetChildrenL((TMsvId)5, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(getEntryPointer.Count(), (5 * 58)); //290 entries in 5 blocks
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5); //5 blocks still
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[4]->SetAccessTime(accessTime);
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[2]->SetAccessTime(accessTime);
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[3]->SetAccessTime(accessTime);
	
	/*
	 Original	 Sorted
	 Block		 Block
	 Ranges		 Ranges
	|---------| |---------|
0	|  0 - 67 | | 68 -125 |  1
	|---------| |---------|
1	| 68 -125 | |  0 - 67 |  0
	|---------| |---------|
2	| 126-183 | | 242-299 |  4
	|---------| |---------|
3	| 184-241 | | 126-183 |  2
	|---------| |---------|
4	| 242-299 | | 184-241 |  3
	|---------| |---------|*/

	//IndexTables are not deleted as the Complete Flag is set
	//This Test Releases block 1 i.e. range 68 - 125 (numberOfEntriesToRelease is set to 40 < 58[BLOCKSIZE])
	numberOfEntriesToRelease = 40;
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(58, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5);

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	//Block 1 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[1]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[1]->BlockPtr());
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
	//This Test Releases block 0 i.e. range 0 - 67 (numberOfEntriesToRelease is set to 40 < 58[BLOCKSIZE]
	//since Block 1 is already released
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(58, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5);

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	//Block 0 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[0]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[0]->BlockPtr());
	
	//Block 1 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[1]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[1]->BlockPtr());
	
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
	//This Test Releases block 4 i.e. range 242 - 299 (numberOfEntriesToRelease is set to 40 < 58[BLOCKSIZE]
	//since Block 0,1 are already released
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(58, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5);

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	//Block 0 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[0]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[0]->BlockPtr());
	
	//Block 1 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[1]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[1]->BlockPtr());
	
	//Block 4 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[4]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[4]->BlockPtr());
	
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
	//This Test Releases block 2 i.e. range 126-183 (numberOfEntriesToRelease is set to 40 < 58[BLOCKSIZE]
	//since Block 0,1,4 are already released
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(58, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5);

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	//Block 0 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[0]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[0]->BlockPtr());
	
	//Block 1 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[1]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[1]->BlockPtr());
	
	//Block 4 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[4]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[4]->BlockPtr());
	
	//Block 2 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[2]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[2]->BlockPtr());
	
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
	//This Test Releases block 3 i.e. range 184-241 (numberOfEntriesToRelease is set to 40 < 58[BLOCKSIZE]
	//since Block 0,1,4,2 are already released
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(58, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5);

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	//Block 0 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[0]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[0]->BlockPtr());
	
	//Block 1 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[1]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[1]->BlockPtr());
	
	//Block 4 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[4]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[4]->BlockPtr());
	
	//Block 2 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[2]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[2]->BlockPtr());
	
	//Block 3 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[3]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[3]->BlockPtr());
	
	MY_ASSERT_TRUE(iFolderPointer[0]->IsAllBlocksDirty());
	
	//since All Blocks are already released Release block shouldnt release any entries
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(0, releasedEntries);
	
	addEntryPointer.Reset();
	getEntryPointer.Reset();
	
	//04. Release Blocks based on access time; the least used block is released first and Few Entries are locked.
	for (TInt index = 10; index < 300; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		if (index == 65 || index == 129 || index == 250)//Locking Entries in Blocks 0,2,4
			{
			entryF->LockEntry();
			}
		addEntryPointer.Append(entryF);
		}
	
	TRAP(err, iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	MY_ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = iFolderPointer[0]->GetChildrenL((TMsvId)5, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(getEntryPointer.Count(), (5 * 58)); //290 entries in 5 blocks
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5); //5 blocks still
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[0]->SetAccessTime(accessTime);
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[4]->SetAccessTime(accessTime);
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[1]->SetAccessTime(accessTime);
	
	/*
	 Original	 Sorted
	 Block		 Block
	 Ranges		 Ranges
	|---------| |---------|
0	|  0 - 67 | | 184-241 |  3
	|---------| |---------|
1	| 68 -125 | | 126-183 |  2 LOCKED
	|---------| |---------|
2	| 126-183 | |  0 - 67 |  0 LOCKED
	|---------| |---------|
3	| 184-241 | | 242-299 |  4 LOCKED
	|---------| |---------|
4	| 242-299 | |  68-125 |  1
	|---------| |---------|*/

	//IndexTables are not deleted as the Complete Flag is set
	//This Test Releases block 3 i.e. range 184 - 241 (numberOfEntriesToRelease is set to 40 < 58[BLOCKSIZE])
	numberOfEntriesToRelease = 40;
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(58, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5);

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	//Block 1 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[3]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[3]->BlockPtr());
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
	//This Test Releases block 1 i.e. range 68 - 125 (numberOfEntriesToRelease is set to 40 < 58[BLOCKSIZE]
	//since Block 3 is already released
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(58, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5);

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	//Block 3 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[3]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[3]->BlockPtr());
	
	//Block 1 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[1]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[1]->BlockPtr());
	
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
	//This Test will not relase any blocks since the block since the entries are locked
	//Blocks 0,1 are already released
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(0, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5);

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	//Block 3 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[3]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[3]->BlockPtr());
	
	//Block 1 should be NULL and DIRTY
	MY_ASSERT_TRUE((*indexTable)[1]->IsDirty());
	MY_ASSERT_EQUALS((RPointerArray<CMsvCacheEntry>*)NULL, (*indexTable)[1]->BlockPtr());
	
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
	addEntryPointer.Reset();
	getEntryPointer.Reset();
	
	//05. Release Blocks based on access time; the least used block is released first and All Entries are locked.
	for (TInt index = 10; index < 300; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		if (index == 76 || index == 190 || index == 65 || index == 129 || index == 250)//Locking Entries in All Blocks
			{
			entryF->LockEntry();
			}
		addEntryPointer.Append(entryF);
		}
	
	TRAP(err, iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	MY_ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = iFolderPointer[0]->GetChildrenL((TMsvId)5, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(getEntryPointer.Count(), (5 * 58)); //290 entries in 5 blocks
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5); //5 blocks still
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[0]->SetAccessTime(accessTime);
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[4]->SetAccessTime(accessTime);
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[1]->SetAccessTime(accessTime);
	
	/*
	 Original	 Sorted
	 Block		 Block
	 Ranges		 Ranges
	|---------| |---------|
0	|  0 - 67 | | 184-241 |  3 LOCKED
	|---------| |---------|
1	| 68 -125 | | 126-183 |  2 LOCKED
	|---------| |---------|
2	| 126-183 | |  0 - 67 |  0 LOCKED
	|---------| |---------|
3	| 184-241 | | 242-299 |  4 LOCKED
	|---------| |---------|
4	| 242-299 | |  68-125 |  1 LOCKED
	|---------| |---------|*/

	//This Test will not relase any blocks since the block since the entries are locked
	//Blocks 0,1 are already released
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(0, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5);

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	//No BLocks DIRTY
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[2]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[3]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[4]->IsDirty());
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
	addEntryPointer.Reset();
	getEntryPointer.Reset();
	
	/**** COMPLETE FLAG IS CLEARED ****/
	//06. Release Blocks based on access time; the least used block is released first and No Entries are locked.
	for (TInt index = 10; index < 300; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	
	//Unlock the locked entries
	iFolderPointer[0]->GetEntry((TMsvId) 76, entryF);
	entryF->ReleaseEntry();
	iFolderPointer[0]->GetEntry((TMsvId) 190, entryF);
	entryF->ReleaseEntry();
	iFolderPointer[0]->GetEntry((TMsvId) 65, entryF);
	entryF->ReleaseEntry();
	iFolderPointer[0]->GetEntry((TMsvId) 129, entryF);
	entryF->ReleaseEntry();
	iFolderPointer[0]->GetEntry((TMsvId) 250, entryF);
	entryF->ReleaseEntry();
	
	TRAP(err, iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	MY_ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = iFolderPointer[0]->GetChildrenL((TMsvId)5, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(getEntryPointer.Count(), (5 * 58)); //290 entries in 5 blocks
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5); //5 blocks still
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[4]->SetAccessTime(accessTime);
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[2]->SetAccessTime(accessTime);
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[3]->SetAccessTime(accessTime);
	
	/*
	 Original	 Sorted
	 Block		 Block
	 Ranges		 Ranges
	|---------| |---------|
0	|  0 - 67 | | 68 -125 |  1
	|---------| |---------|
1	| 68 -125 | |  0 - 67 |  0
	|---------| |---------|
2	| 126-183 | | 242-299 |  4
	|---------| |---------|
3	| 184-241 | | 126-183 |  2
	|---------| |---------|
4	| 242-299 | | 184-241 |  3
	|---------| |---------|*/

	iFolderPointer[0]->SetComplete(EFalse);
	
	//IndexTables are deleted as the Complete Flag is Cleared
	//This Test Releases block 1 i.e. range 68 - 125 and updates block 2 MinMsvId to 68
	//(numberOfEntriesToRelease is set to 40 < 58[BLOCKSIZE])
	numberOfEntriesToRelease = 40;
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(58, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 4); //Now 4 Blocks

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[3]->GetMaxMsvIdRange());
	
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
/*
	 Original	 Sorted
	 Block		 Block
	 Ranges		 Ranges
	|---------| |---------|
0	|  0 - 67 | |  0 - 67 |  0
	|---------| |---------|
1	|  68-183 | | 242-299 |  3
	|---------| |---------|
2	| 184-241 | |  68-183 |  1
	|---------| |---------|
3	| 242-299 | | 184-241 |  2
	|---------| |---------|*/
	
	//IndexTables are deleted as the Complete Flag is Cleared
	//This Test Releases block 0 i.e. range 0 - 67 and updates block 1 MinMsvId to 0
	//(numberOfEntriesToRelease is set to 40 < 58[BLOCKSIZE])
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(58, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 3);//Now 3

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[2]->GetMaxMsvIdRange());
	
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
/*
	 Original	 Sorted
	 Block		 Block
	 Ranges		 Ranges
	|---------| |---------|
0	|  0 -183 | | 242-299 |  2
	|---------| |---------|
1	| 184-241 | |  0 -183 |  0
	|---------| |---------|
2	| 242-299 | | 184-241 |  1
	|---------| |---------|*/
	
	//IndexTables are deleted as the Complete Flag is Cleared
	//This Test Releases block 2 i.e. range 242 - 299.
	//(numberOfEntriesToRelease is set to 40 < 58[BLOCKSIZE])
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(58, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 2);//Now 2

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[1]->GetMaxMsvIdRange());
	
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
	/*
	 Original	 Sorted
	 Block		 Block
	 Ranges		 Ranges
	|---------| |---------|
0	|  0 -183 | |  0 -183 |  0
	|---------| |---------|
1	| 184-241 | | 184-241 |  1
	|---------| |---------|*/
	
	//IndexTables are deleted as the Complete Flag is Cleared
	//This Test Releases block 0 i.e. range 242 - 299 and updates block 1 MinMsvId to 0
	//(numberOfEntriesToRelease is set to 40 < 58[BLOCKSIZE])
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(58, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 1);//Now 1

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[0]->GetMaxMsvIdRange());
	
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
	/*
	 Original	 Sorted
	 Block		 Block
	 Ranges		 Ranges
	|---------| |---------|
0	|  0 -241 | |  0 -241 |  0
	|---------| |---------|*/
	
	//Complete IndexTable is deleted
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_TRUE(isFolderEmpty);
	ASSERT_EQUALS(58, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 0);//Now 0

	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
	//Complete IndexTable is deleted
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_TRUE(isFolderEmpty);
	ASSERT_EQUALS(0, releasedEntries);

	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
	addEntryPointer.Reset();
	getEntryPointer.Reset();

	//06. Release Blocks based on access time; the least used block is released first and Few Entries are locked.
	for (TInt index = 10; index < 300; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		if (index == 65 || index == 129 || index == 250)//Locking Entries in Blocks 0,2,4
			{
			entryF->LockEntry();
			}
		addEntryPointer.Append(entryF);
		}
	
	TRAP(err, iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	MY_ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = iFolderPointer[0]->GetChildrenL((TMsvId)5, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(getEntryPointer.Count(), (5 * 58)); //290 entries in 5 blocks
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5); //5 blocks still
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[0]->SetAccessTime(accessTime);
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[4]->SetAccessTime(accessTime);
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[1]->SetAccessTime(accessTime);
	
	/*
	 Original	 Sorted
	 Block		 Block
	 Ranges		 Ranges
	|---------| |---------|
0	|  0 - 67 | | 184-241 |  3
	|---------| |---------|
1	| 68 -125 | | 126-183 |  2 LOCKED
	|---------| |---------|
2	| 126-183 | |  0 - 67 |  0 LOCKED
	|---------| |---------|
3	| 184-241 | | 242-299 |  4 LOCKED
	|---------| |---------|
4	| 242-299 | |  68-125 |  1
	|---------| |---------|*/

	iFolderPointer[0]->SetComplete(EFalse);
	
	//This Test Releases block 3 i.e. range 184 - 241 and updates block 4 MinMsvId to 184
	//(numberOfEntriesToRelease is set to 40 < 58[BLOCKSIZE])
	numberOfEntriesToRelease = 40;
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(58, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 4); //Now 4

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[3]->GetMaxMsvIdRange());
	
	//This Test Releases block 1 i.e. range 184 - 241 and updates block 2 MinMsvId to 68
	//(numberOfEntriesToRelease is set to 40 < 58[BLOCKSIZE])
	numberOfEntriesToRelease = 40;
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(58, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 3); //Now 4

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[2]->GetMaxMsvIdRange());
	
	//This Test wont Releases any blocks  since some entries are locked in it
	//(numberOfEntriesToRelease is set to 40 < 58[BLOCKSIZE])
	numberOfEntriesToRelease = 40;
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(0, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 3); //Now 4

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());

	addEntryPointer.Reset();
	getEntryPointer.Reset();
	
	//07. Release Blocks based on access time; release more than one block at once
	for (TInt index = 10; index < 300; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		if (index == 129 || index == 250)//Locking Entries in Blocks 2,4
			{
			entryF->LockEntry();
			}
		addEntryPointer.Append(entryF);
		}

	iFolderPointer[0]->GetEntry((TMsvId) 65, entryF);
	entryF->ReleaseEntry();
	
	TRAP(err, iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	MY_ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = iFolderPointer[0]->GetChildrenL((TMsvId)5, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(getEntryPointer.Count(), (5 * 58)); //290 entries in 5 blocks
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5); //5 blocks still
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[0]->SetAccessTime(accessTime);
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[4]->SetAccessTime(accessTime);
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[1]->SetAccessTime(accessTime);
	
	/*
	 Original	 Sorted
	 Block		 Block
	 Ranges		 Ranges
	|---------| |---------|
0	|  0 - 67 | | 184-241 |  3
	|---------| |---------|
1	| 68 -125 | | 126-183 |  2 LOCKED
	|---------| |---------|
2	| 126-183 | |  0 - 67 |  0
	|---------| |---------|
3	| 184-241 | | 242-299 |  4 LOCKED
	|---------| |---------|
4	| 242-299 | |  68-125 |  1
	|---------| |---------|*/

	iFolderPointer[0]->SetComplete(EFalse);
	
	numberOfEntriesToRelease = 170;
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_FALSE(isFolderEmpty);
	ASSERT_EQUALS(174, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 2); //Now 2

	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[1]->GetMaxMsvIdRange());

	//08. Release Blocks based on access time; release more than one block at once
	//clear previous test residues
	iFolderPointer[0]->GetEntry((TMsvId) 129, entryF);
	entryF->ReleaseEntry();
	iFolderPointer[0]->GetEntry((TMsvId) 250, entryF);
	entryF->ReleaseEntry();
	numberOfEntriesToRelease = 116;
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_TRUE(isFolderEmpty);
	ASSERT_EQUALS(116, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 0);
	addEntryPointer.Reset();
	
	for (TInt index = 10; index < 300; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}

	TRAP(err, iFolderPointer[0]->AddEntryListL(addEntryPointer, ETrue));
	MY_ASSERT_EQUALS(err, KErrNone);
	getEntryPointer.Reset();
	TRAP(err, ret = iFolderPointer[0]->GetChildrenL((TMsvId)5, iMsvDbAdaptor, getEntryPointer));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(ret);
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(getEntryPointer.Count(), (5 * 58)); //290 entries in 5 blocks
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 5); //5 blocks still
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)67, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)68, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)125, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)126, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)183, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)184, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)241, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)242, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)299, (*indexTable)[4]->GetMaxMsvIdRange());
	
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[0]->SetAccessTime(accessTime);
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[4]->SetAccessTime(accessTime);
	User::After(1000);
	accessTime.UniversalTime();
	(*indexTable)[1]->SetAccessTime(accessTime);
	
	/*
	 Original	 Sorted
	 Block		 Block
	 Ranges		 Ranges
	|---------| |---------|
0	|  0 - 67 | | 184-241 |  3
	|---------| |---------|
1	| 68 -125 | | 126-183 |  2
	|---------| |---------|
2	| 126-183 | |  0 - 67 |  0
	|---------| |---------|
3	| 184-241 | | 242-299 |  4
	|---------| |---------|
4	| 242-299 | |  68-125 |  1
	|---------| |---------|*/

	iFolderPointer[0]->SetComplete(EFalse);
	
	numberOfEntriesToRelease = 290;
	releasedEntries = iFolderPointer[0]->ReleaseBlocks( numberOfEntriesToRelease, isFolderEmpty);
	ASSERT_TRUE(isFolderEmpty);
	ASSERT_EQUALS(290, releasedEntries);
	MY_ASSERT_EQUALS(iFolderPointer[0]->GetIndexTableEntry()->Count(), 0);
	
	addEntryPointer.Close();
	getEntryPointer.Close();
	}

/**
 TestIsComplete()
@param None.
@return None.
 
Tests the IsComplete() functionality of CMsCacheVisibleFolder Class.
Tested Steps
01. Init
02. Release Entries when IndexTable is not created  and IsComplete Flag is Set.
03. Release Entries when iscomplete flag is not set and a single block
04. Release Entries when iscomplete flag is not set and more than one block
05. Release a Blocks.
06. Release two Blocks.
07. Release all(3) Blocks.
08. Release entries when one entry is locked
09. Release entires when one entry in each block is locked
*/
void CTestVisibleFolder::TestIsComplete()
	{
	_LIT(KFunction, "TestIsComplete");
	INFO_PRINTF1(KFunction);
	
	// 01. Init
	CMsvCacheEntry* entryF;
	CMsvCacheEntry* entryE;
	TInt releasedEntries;

	// 02. Test the default flag status
	MY_ASSERT_FALSE(iFolderPointer[1]->IsComplete());
	
	// 03. Add a single Entry and Test the status of the Flag
	entryF = Entry();
	
	entryF->Entry().SetParent((TMsvId)30);
	entryF->Entry().SetId ((TMsvId)06);//TMsvId = 06
	
	TRAP_IGNORE(iFolderPointer[1]->AddEntryL(entryF));
	MY_ASSERT_TRUE(iFolderPointer[1]->GetEntry ((TMsvId)06, entryE));
	MY_ASSERT_FALSE(iFolderPointer[1]->IsComplete());
	
	// 04. Test the flag status after AddEntryL
	AddTestEntries();
	MY_ASSERT_TRUE(iFolderPointer[1]->IsComplete());
	
	//5. Test the Flag after blocks been released to free pool
	ASSERT_TRUE(iFolderPointer[1]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(3,releasedEntries);
	
	//6. Set functions
	iFolderPointer[1]->SetComplete (ETrue);
	MY_ASSERT_TRUE(iFolderPointer[1]->IsComplete());

	iFolderPointer[1]->SetComplete (EFalse);
	MY_ASSERT_FALSE(iFolderPointer[1]->IsComplete());
	}

/**
 TestIsAllBlocksDirty()
@param None.
@return None.
 
Tests the IsAllBlocksDirty() functionality of CMsCacheVisibleFolder Class.
Tested Steps
1. Init
2. Check the default condition of the flag
3. Add some Entries and check the status of the flag
4. Release a single block then check the status of the flag
5. Release all blocks and then check the status of the flag
*/
void CTestVisibleFolder::TestIsAllBlocksDirty()
	{
	_LIT(KFunction, "TestIsAllBlocksDirty");
	INFO_PRINTF1(KFunction);
	
	//1. Init
	CMsvCacheEntry* entryF;
	RPointerArray<CMsvCacheEntry> addEntryPointer;
	TInt releasedEntries;
	TInt entriesToBeReleased;
	TBool folderEmtpy;
	
	// 2. Check the default condition of the flag
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
	//3. Add some Entries and check the status of the flag
	for (TInt index = 10; index < 155; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
	//4. Release a single block then check the status of the flag
	entriesToBeReleased = 48;
	
	releasedEntries = iFolderPointer[0]->ReleaseBlocks(entriesToBeReleased, folderEmtpy);
	ASSERT_FALSE(folderEmtpy);
	ASSERT_EQUALS(48,releasedEntries);
	MY_ASSERT_FALSE(iFolderPointer[0]->IsAllBlocksDirty());
	
	//5. Release all blocks and then check the status of the flag

	entriesToBeReleased = 97;
	
	releasedEntries = iFolderPointer[0]->ReleaseBlocks(entriesToBeReleased, folderEmtpy);
	ASSERT_FALSE(folderEmtpy);
	ASSERT_EQUALS(97,releasedEntries);
	MY_ASSERT_TRUE(iFolderPointer[0]->IsAllBlocksDirty());
	
	addEntryPointer.Close();
	}


/**
 TestSetComplete()
@param None.
@return None.
 
Tests the SetComplete() functionality of CMsCacheVisibleFolder Class.
Tested Steps
Similar to TestIsComplete();
*/
void CTestVisibleFolder::TestSetComplete()
	{
	_LIT(KTestSetComplete, "TestSetComplete");
	INFO_PRINTF1(KTestSetComplete);
	
	TestIsComplete();
	}

/**
 TestEntryExists()
@param None.
@return None.
 
Tests the EntryExists() functionality of CMsCacheVisibleFolder Class.
Tested Steps
01. Check for an Entry Which Does not Exist
02. Check for an Entry Which Exists
03. Check the Id of the Visible folder
*/
void CTestVisibleFolder::TestEntryExists()
	{
	_LIT(KTestSetComplete, "TestSetComplete");
	INFO_PRINTF1(KTestSetComplete);
	
	// 01. Check for an Entry Which Does not Exist
	ASSERT_FALSE(iFolderPointer[1]->EntryExists((TMsvId)06));

	// 02. Check for an Entry Which Exists
	AddTestEntries();
	ASSERT_TRUE(iFolderPointer[1]->EntryExists((TMsvId)05));
	
	//03. Check the Id of the Visible folder
	ASSERT_TRUE(iFolderPointer[1]->EntryExists((TMsvId)30));
	}

void CTestVisibleFolder::TestSplitBlock()
	{
	_LIT(KFunction, "TestSplitBlock");
	INFO_PRINTF1(KFunction);
	
	// 01. Init
	TInt error;
	CMsvCacheEntry* entryF;
	RPointerArray<CMsvCacheEntry> addEntryPointer;
	RPointerArray<CMsvCacheEntry> getEntryPointer;
	RPointerArray<CMsvCacheIndexTableEntry>* indexTable;
	TInt releasedEntries;
	
	// 02. SplitBlockL on a folder with no indextable entries
	error = KErrNone;
	TRAP(error,iFolderPointer[0]->SplitBlockL());
	MY_ASSERT_EQUALS(error, KErrNone);
	
	//03. SplitBlockL on a single Block which is < 120 Entries
	for (TInt index = 10; index < 74; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(64, getEntryPointer.Count());
	MY_ASSERT_EQUALS(1, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)73, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(64, (*indexTable)[0]->BlockPtr()->Count());

	error = KErrNone;
	TRAP(error,iFolderPointer[0]->SplitBlockL());
	MY_ASSERT_EQUALS(error, KErrNone);
	
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(1, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)73, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(64, (*indexTable)[0]->BlockPtr()->Count());
	
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(64, releasedEntries);

	entryF=NULL;
	delete iFolderPointer[0];
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));
	
	// 04. SplitBlockL on a single Block which is > 120 Entries
	for (TInt index = 10; index < 74; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	
	for (TInt index = 74; index < 138; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		TRAP_IGNORE(iFolderPointer[0]->AddEntryL(entryF));
		}
	
	
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(128, getEntryPointer.Count());
	MY_ASSERT_EQUALS(1, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)137, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(128, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());

	error = KErrNone;
	TRAP(error,iFolderPointer[0]->SplitBlockL());
	MY_ASSERT_EQUALS(error, KErrNone);
	
	getEntryPointer.Reset();
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(2, indexTable->Count());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(128, getEntryPointer.Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)73, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)74, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)137, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(64, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(128, releasedEntries);

	entryF=NULL;
	delete iFolderPointer[0];
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));
	
	// 05. SplitBlockL on 2 blocks with 1 block < 120 and 2 Block > 120
	for (TInt index = 10; index < 205; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		if (index == 74 )
			{
			index = 139;
			}
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	
	for (TInt index = 74; index < 139; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		TRAP_IGNORE(iFolderPointer[0]->AddEntryL(entryF));
		}
	
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(194, getEntryPointer.Count());
	MY_ASSERT_EQUALS(2, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)74, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(65, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(129, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());

	error = KErrNone;
	TRAP(error,iFolderPointer[0]->SplitBlockL());
	MY_ASSERT_EQUALS(error, KErrNone);
	
	getEntryPointer.Reset();
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(3, indexTable->Count());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(194, getEntryPointer.Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)74, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)75, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)139, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)140, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)204, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(65, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(65, (*indexTable)[2]->BlockPtr()->Count());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());

	
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	ASSERT_EQUALS(194, releasedEntries);

	entryF=NULL;
	delete iFolderPointer[0];
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));

	// 06. SplitBlockL on 2 blocks with 1 block > 120 and 2 Block < 120
	for (TInt index = 10; index < 239; ++index)
		{
		if (index == 73 )
			{
			index = 173;
			}
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	
	for (TInt index = 73; index < 174; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		TRAP_IGNORE(iFolderPointer[0]->AddEntryL(entryF));
		}

	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(229, getEntryPointer.Count());
	MY_ASSERT_EQUALS(2, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)173, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)174, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)238, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(164, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(65, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());

	
	error = KErrNone;
	TRAP(error,iFolderPointer[0]->SplitBlockL());
	MY_ASSERT_EQUALS(error, KErrNone);
	
	getEntryPointer.Reset();
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(3, indexTable->Count());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(229, getEntryPointer.Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)91, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)92, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)173, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)174, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)238, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(82, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(82, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(65, (*indexTable)[2]->BlockPtr()->Count());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[2]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[2]->IsGrandChildPresent());
	
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(229, releasedEntries);

	entryF=NULL;
	delete iFolderPointer[0];
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));

	// 07. SplitBlockL on 2 blocks with 1 block > 120 and 2 Block >120
	for (TInt index = 10; index < 239; ++index)
		{
		if (index == 73 )
			{
			index = 173;
			}
		if (index == 238 )
			{
			index = 300;
			}
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	
	for (TInt index = 73; index < 174; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		TRAP_IGNORE(iFolderPointer[0]->AddEntryL(entryF));
		}
	
	for (TInt index = 238; index < 300; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		TRAP_IGNORE(iFolderPointer[0]->AddEntryL(entryF));
		}

	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(291, getEntryPointer.Count());
	MY_ASSERT_EQUALS(2, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)173, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)174, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)300, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(164, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(127, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());

	error = KErrNone;
	TRAP(error,iFolderPointer[0]->SplitBlockL());
	MY_ASSERT_EQUALS(error, KErrNone);
	
	getEntryPointer.Reset();
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(4, indexTable->Count());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(291, getEntryPointer.Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)91, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)92, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)173, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)174, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)236, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)237, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)300, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(82, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(82, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(63, (*indexTable)[2]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[3]->BlockPtr()->Count());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[2]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[3]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[3]->IsGrandChildPresent());
	
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(291, releasedEntries);

	entryF=NULL;
	delete iFolderPointer[0];
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));
	
	// 08. SplitBlockL on 2 blocks with block 1 > 120 and block 2 < 120 and block 3 > 120
	for (TInt index = 10; index < 304; ++index)
		{
		if (index == 73 )
			{
			index = 173;
			}
		if (index == 303 )
			{
			index = 400;
			}
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	
	for (TInt index = 73; index < 174; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		TRAP_IGNORE(iFolderPointer[0]->AddEntryL(entryF));
		}
	
	for (TInt index = 238; index < 400; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		TRAP_IGNORE(iFolderPointer[0]->AddEntryL(entryF));
		}

	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(391, getEntryPointer.Count());
	MY_ASSERT_EQUALS(3, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)173, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)174, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)237, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)238, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)400, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(164, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(163, (*indexTable)[2]->BlockPtr()->Count());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[2]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[2]->IsGrandChildPresent());

	error = KErrNone;
	TRAP(error,iFolderPointer[0]->SplitBlockL());
	MY_ASSERT_EQUALS(error, KErrNone);
	
	getEntryPointer.Reset();
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(5, indexTable->Count());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(391, getEntryPointer.Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)91, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)92, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)173, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)174, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)237, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)238, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)318, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)319, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)400, (*indexTable)[4]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(82, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(82, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[2]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(81, (*indexTable)[3]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(82, (*indexTable)[4]->BlockPtr()->Count());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[2]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[3]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[4]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[3]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[4]->IsGrandChildPresent());
	
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(391, releasedEntries);

	entryF=NULL;
	delete iFolderPointer[0];
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));
	
	// 08. SplitBlockL on 2 blocks with block 1 > 120, dirty and block 2 < 120 and block 3 > 120, dirty
	for (TInt index = 10; index < 304; ++index)
		{
		if (index == 73 )
			{
			index = 173;
			}
		if (index == 303 )
			{
			index = 400;
			}
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	
	for (TInt index = 73; index < 174; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		TRAP_IGNORE(iFolderPointer[0]->AddEntryL(entryF));
		}
	
	for (TInt index = 238; index < 400; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		TRAP_IGNORE(iFolderPointer[0]->AddEntryL(entryF));
		}

	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(391, getEntryPointer.Count());
	MY_ASSERT_EQUALS(3, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)173, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)174, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)237, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)238, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)400, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(164, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(163, (*indexTable)[2]->BlockPtr()->Count());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());
	(*indexTable)[0]->SetDirty();
	MY_ASSERT_TRUE((*indexTable)[0]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[2]->IsDirty());
	(*indexTable)[2]->SetDirty();
	MY_ASSERT_TRUE((*indexTable)[2]->IsDirty());

	error = KErrNone;
	TRAP(error,iFolderPointer[0]->SplitBlockL());
	MY_ASSERT_EQUALS(error, KErrNone);
	
	getEntryPointer.Reset();
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(5, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)91, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)92, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)173, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)174, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)237, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)238, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)318, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)319, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)400, (*indexTable)[4]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(82, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(82, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[2]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(81, (*indexTable)[3]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(82, (*indexTable)[4]->BlockPtr()->Count());
	MY_ASSERT_TRUE((*indexTable)[0]->IsDirty());
	MY_ASSERT_TRUE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[2]->IsDirty());
	MY_ASSERT_TRUE((*indexTable)[3]->IsDirty());
	MY_ASSERT_TRUE((*indexTable)[4]->IsDirty());
	
	(*indexTable)[0]->ClearDirty();
	(*indexTable)[1]->ClearDirty();
	(*indexTable)[3]->ClearDirty();
	(*indexTable)[4]->ClearDirty();
	
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(391, getEntryPointer.Count());
	
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(391, releasedEntries);

	entryF=NULL;
	delete iFolderPointer[0];
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));
	
	// 09. SplitBlockL on 2 blocks with block 1 > 120, dirty and block 2 < 120 and block 3 > 120, dirty
	for (TInt index = 10; index < 304; ++index)
		{
		if (index == 73 )
			{
			index = 173;
			}
		if (index == 303 )
			{
			index = 400;
			}
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		addEntryPointer.Append(entryF);
		}
	TRAP_IGNORE(iFolderPointer[0]->AddEntryListL(addEntryPointer,ETrue));
	
	for (TInt index = 73; index < 174; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		TRAP_IGNORE(iFolderPointer[0]->AddEntryL(entryF));
		}
	
	for (TInt index = 238; index < 400; ++index)
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)5);
		entryF->Entry().SetId ((TMsvId)index);
		TRAP_IGNORE(iFolderPointer[0]->AddEntryL(entryF));
		}

	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_TRUE(iFolderPointer[0]->IsComplete());
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(391, getEntryPointer.Count());
	MY_ASSERT_EQUALS(3, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)173, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)174, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)237, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)238, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)400, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(164, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(163, (*indexTable)[2]->BlockPtr()->Count());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[2]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[2]->IsGrandChildPresent());
	(*indexTable)[1]->SetGrandChildPresent();
	(*indexTable)[2]->SetGrandChildPresent();
	MY_ASSERT_TRUE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_TRUE((*indexTable)[2]->IsGrandChildPresent());

	error = KErrNone;
	TRAP(error,iFolderPointer[0]->SplitBlockL());
	MY_ASSERT_EQUALS(error, KErrNone);
	
	getEntryPointer.Reset();
	indexTable = iFolderPointer[0]->GetIndexTableEntry();
	MY_ASSERT_EQUALS(5, indexTable->Count());
	MY_ASSERT_EQUALS((TMsvId)0, (*indexTable)[0]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)91, (*indexTable)[0]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)92, (*indexTable)[1]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)173, (*indexTable)[1]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)174, (*indexTable)[2]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)237, (*indexTable)[2]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)238, (*indexTable)[3]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)318, (*indexTable)[3]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)319, (*indexTable)[4]->GetMinMsvIdRange());
	MY_ASSERT_EQUALS((TMsvId)400, (*indexTable)[4]->GetMaxMsvIdRange());
	MY_ASSERT_EQUALS(82, (*indexTable)[0]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(82, (*indexTable)[1]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(64, (*indexTable)[2]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(81, (*indexTable)[3]->BlockPtr()->Count());
	MY_ASSERT_EQUALS(82, (*indexTable)[4]->BlockPtr()->Count());
	MY_ASSERT_FALSE((*indexTable)[0]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[1]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[2]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[3]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[4]->IsDirty());
	MY_ASSERT_FALSE((*indexTable)[0]->IsGrandChildPresent());
	MY_ASSERT_FALSE((*indexTable)[1]->IsGrandChildPresent());
	MY_ASSERT_TRUE((*indexTable)[2]->IsGrandChildPresent());
	MY_ASSERT_TRUE((*indexTable)[3]->IsGrandChildPresent());
	MY_ASSERT_TRUE((*indexTable)[4]->IsGrandChildPresent());
	
	
	TRAP_IGNORE(MY_ASSERT_TRUE(iFolderPointer[0]->GetChildrenL((TMsvId)5, NULL, getEntryPointer)));
	MY_ASSERT_EQUALS(391, getEntryPointer.Count());
	
	getEntryPointer.Reset();
	addEntryPointer.Reset();
	MY_ASSERT_TRUE(iFolderPointer[0]->ReleaseAllBlocks(releasedEntries));
	MY_ASSERT_EQUALS(391, releasedEntries);

	entryF=NULL;
	delete iFolderPointer[0];
	TRAP_IGNORE(iFolderPointer[0] = CMsvCacheVisibleFolder::NewL((TMsvId)5));
	
	addEntryPointer.Close();
	getEntryPointer.Close();
	}

void CTestVisibleFolder::TestCreation()
	{
	_LIT(KFunction, "TestCreation");
	INFO_PRINTF1(KFunction);
	CMsvCacheVisibleFolder* folder = NULL;
	TRAP_IGNORE(folder = CMsvCacheVisibleFolder::NewL((TMsvId)10));
	RPointerArray<CMsvCacheEntry> aEntries;
	CMsvCacheEntry* entry = Entry();
	entry->Entry().SetId((TMsvId)50);
	entry->Entry().SetParent((TMsvId)10);
	aEntries.Append(entry);
	TRAP_IGNORE(folder->ConstructL(aEntries));
	MY_ASSERT_TRUE(folder->IsComplete());
	MY_ASSERT_TRUE(folder->GetEntry((TMsvId)50, entry));
	delete folder;
	
	aEntries.Reset();
	for(TInt index = 0; index < 10; ++index)
		{
		entry = Entry();
		entry->Entry().SetId((TMsvId)50+index);
		entry->Entry().SetParent((TMsvId)10);
		aEntries.Append(entry);
		}
	TRAP_IGNORE(folder = CMsvCacheVisibleFolder::NewL((TMsvId)10, aEntries));
	MY_ASSERT_TRUE(folder->IsComplete());
	for(TInt index = 0; index < 10; ++index)
		{
		ASSERT_TRUE(folder->GetEntry((TMsvId)50+index, entry));
		}
	delete folder;
	aEntries.Close();
	}


CMsvCacheEntry* CTestVisibleFolder::Entry()
	{
	CMsvCacheEntry *entry = NULL;
	TRAP_IGNORE(entry = CMsvEntryFreePool::Instance()->EntryL());
	return entry;
	}



void CTestVisibleFolder::TestUpdateChildMsvIds()
	{
	_LIT(KFunction, "TestUpdateChildMsvIds");
	INFO_PRINTF1(KFunction);	
		
	// Block count is zero.
	TRAPD(err, iFolderPointer[0]->UpdateChildMsvIdsL(KFirstFreeEntryId, KFirstFreeEntryId+10));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CMsvCacheIndexTableEntry* tableEntry1 = NULL;	
	CMsvCacheIndexTableEntry* tableEntry2 = NULL;
	
	CMsvCacheEntry *entry = Entry();
	entry->Entry().SetId((TMsvId)KFirstFreeEntryId+65);
	entry->Entry().SetParent((TMsvId)10);
	TRAP(err, tableEntry1 = CMsvCacheIndexTableEntry::NewL(entry));
	
	entry = Entry();
	entry->Entry().SetId((TMsvId)KFirstFreeEntryId+75);
	entry->Entry().SetParent((TMsvId)10);
	TRAP(err, tableEntry2 = CMsvCacheIndexTableEntry::NewL(entry));
	
	iFolderPointer[0]->iIndexTable.Insert(tableEntry1, 0);
	iFolderPointer[0]->iIndexTable.Insert(tableEntry2, 1);
	
	TRAP(err, iFolderPointer[0]->UpdateChildMsvIdsL(KFirstFreeEntryId+75, KFirstFreeEntryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	}


#ifdef _DEBUG
void Print(CMsvCacheVisibleFolder *aFolder)
	{
	RFileLogger logger;
	if (logger.Connect() == KErrNone)
		{
		logger.CreateLog(_L("msgs"), _L("Cache.txt"), EFileLoggingModeOverwrite);
		logger.Write(_L(""));
	
		aFolder->Print(logger);
		logger.CloseLog();
		}
	logger.Close();		
	}
#endif



CTestSuite* CTestVisibleFolder::CreateSuiteL(const TDesC& aName)
// static
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestAddEntry);
	ADD_ASYNC_TEST_STEP(TestAddEntryList);
	ADD_ASYNC_TEST_STEP(TestGetEntry);
	ADD_ASYNC_TEST_STEP(TestGetChildren);
	ADD_ASYNC_TEST_STEP(TestDeleteEntry);
	ADD_ASYNC_TEST_STEP(TestDeleteEntryList);
	ADD_ASYNC_TEST_STEP(TestReleaseAllBlocks);
	ADD_ASYNC_TEST_STEP(TestReleaseBlocks);
	ADD_ASYNC_TEST_STEP(TestIsComplete);
	ADD_ASYNC_TEST_STEP(TestIsAllBlocksDirty);
	ADD_ASYNC_TEST_STEP(TestSetComplete);
	ADD_ASYNC_TEST_STEP(TestEntryExists);
	ADD_ASYNC_TEST_STEP(TestSplitBlock);
	ADD_ASYNC_TEST_STEP(TestCreation);
	ADD_ASYNC_TEST_STEP(TestUpdateChildMsvIds);
	END_SUITE;
	}
