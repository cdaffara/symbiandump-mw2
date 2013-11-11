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
// Example CTestStep derived implementation
// Unit test case identified for CMsvIndexAdapter
// >> NewL()
// 1.1. CMsvIndexAdapter creates succesfully.
// 1.2. Messaging resource file does not exists.
// 1.3. Messaging resource file is corrupted.
// 1.4. Messaging resource file is not in correct format.
// 1.5. Messaging resource file does not have folder entry info.
// 1.6. DB creation fails.
// 1.7. Cache creation leaves.
// 1.8. Freepool creation fails.
// 1.9. NewL() leaves.
// >> OpenL()
// 2.1. CMsvIndexAdapter instance is created succesfully.
// 2.2. Opening DB fails. (DB file is corrupted.)
// 2.3. Creation of initial cache fails.
// 2.4. Creation of freepool fails.
// 2.5. Openl() leaves.
// >> AddEntry()
// 3.1. Adding a new entry.
// 3.2. Adding an entry with few wrong values in the fields.
// 3.3. Adding a folder entry.
// 3.4. Adding a message entry.
// 3.5. Adding an attachment entry, when parent is in cache and CMsvCacheEntry::GetChildren() on parent returns NULL.
// 3.6. Adding an attachment entry, when parent is in cache and CMsvCacheEntry::GetChildren() on parent does not return NULL.
// 3.7. Adding an attachment entry, when parent is not in cache.
// 3.8. Adding an entry when parent does not exists.
// 3.9. Adding a service entry.
// 3.10. Adding an folder entry with AutoAssigned OFF.
// 3.11. Adding an message entry with AutoAssigned OFF.
// 3.12. Adding first entry with AutoAssigned OFF.
// >> AddEntryNoCommit()
// 4.1. Similar set of testcases as for AddEntry().
// >> DeleteEntry()
// 5.1. Delete a normal entry.
// 5.2. Delete a non-committed entry.
// 5.3. Delete a message entry.
// 5.4. Delete a visible folder entry, which has child entries and which is loaded in cache.
// 5.5. Delete a non-existent entry.
// >> DeleteSelection()
// 6.1. List has only one entry.
// 6.2. List has couple of entries, all under same folder.
// 6.3. List is empty.
// 6.4. List has entries, dispersed across the various folders and sub-folders.
// 6.5. Couple of entries are repeated in the list.
// 6.6. List has entries not committed to the DB.
// 6.7. ExpandSelectionRecursively() on list is invoked. 
// MACRO Declaration
// 
//

/**
 @file 
 @internalTechnology
*/

#include "t_msv_indexadapter_step.h"

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
#include "msvpreferreddrivelist.h"
#endif
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

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
#define MY_ASSERT_NOT_NULL(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_NOT_NULL(a);


_LIT(KDbFileName, "\\messaging.db");
_LIT(KBaseDBFileName, "C:\\messaging_base.db");
_LIT(KDBOpenFileName,"C:\\messaging.db");
_LIT(KSTDMSGINI, "C:\\private\\1000484b\\msgcache.ini");
_LIT(KMSGINI_2, "C:\\private\\1000484b\\msgcache2.ini");
_LIT(KMsgIndexFile, "\\private\\1000484b\\Mail2\\Index");

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
_LIT(KPreferredDriveListFilePath, "C:\\private\\1000484b\\msgprioritydrivelist.ini");
_LIT(KMsgTestPriorityDriveListFile2, "C:\\private\\1000484b\\msgprioritydrivelist2.ini");
#endif

TFileName dbFileName;
TMsvEntry sampleTMsvEntry;
TInt bulkCount = 50;


CTestIndexAdapter::CTestIndexAdapter()
	{
	}

CTestIndexAdapter::~CTestIndexAdapter()
	{	
	}



void CTestIndexAdapter::CreateFileNames()
	{	
	TChar driveLetter = 'C';
	dbFileName.Zero();
	dbFileName.Append(driveLetter);
	dbFileName.Append(KDbFileName);
	}




void CTestIndexAdapter::DeleteEntry(TMsvId aId)
	{
	_LIT(KFunction, "DeleteEntry");
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->DeleteEntry(aId));
	
	
	}




void CTestIndexAdapter::DeleteSelection(const CMsvEntrySelection& aIdList)
	{
	_LIT(KFunction, "DeleteSelection");
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->DeleteSelection(aIdList));
	}


	
	
void CTestIndexAdapter::AddEntry(TBool aCommit, TBool aAutoAssigned, TMsvId aId /*(Default =0) */, TMsvId aParent)
/* Default value for aParent = KMsvRootIndexEntryIdValue (Before 557 Implementation)
                               KMsvLocalServiceIndexEntryId  (After 557 implementation)
*/
	{
	_LIT(KFunction, "AddEntry");
	TSecureId aOwnerId = 0x999;
	
	// Adding entry with false AutoAssigned
	sampleTMsvEntry.iDate.UniversalTime();
	sampleTMsvEntry.SetParent(aParent);
	sampleTMsvEntry.iType = KUidMsvFolderEntry;
	
	if( !aAutoAssigned )
		sampleTMsvEntry.SetId(aId);
	
	if(aCommit)
		{
		MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry, aOwnerId, aAutoAssigned));
		}	
	else
		{
		MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntryNoCommit(sampleTMsvEntry, aOwnerId, aAutoAssigned));
		}
	}


// Changes for 557.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

void CTestIndexAdapter::TestAddEntryNoCommit()
	{
	_LIT(KFunction, "TestAddEntryNoCommit");
	
	TMsvId newId = MaskTMsvId(2, KFirstFreeEntryId);
	// 2. Adding bulk entry using transaction.
	// Single entry insertion fails. Entry insertion should rollback.
	// 2.a. Adding 10 entries using AddEntryNoCommit()
	for(TInt index=20; index<30; index++)
		{
		AddEntry(EFalse, EFalse, newId+index, KMsvLocalServiceIndexEntryId);
		}
	
	// Add one more entry if DB pointer is NULL.
	CMsvDBAdapter* dbPtr = iIndexAdapter->iDbAdapter;
	iIndexAdapter->iDbAdapter = NULL;
	TSecureId aOwnerId = 0x999;
	sampleTMsvEntry.SetId(newId+30);
	TInt err = iIndexAdapter->AddEntryNoCommit(sampleTMsvEntry, aOwnerId, EFalse);
	iIndexAdapter->iDbAdapter = dbPtr;
		
	// 2.b. Check the status of NonComittedAddedEntry list.
	RPointerArray<CMsvIndexAdapter::CNonCommittedAddedEntries> *entryList = NULL;
	iIndexAdapter->GetNonComittedAddedEntry(entryList);
	MY_ASSERT_EQUALS(10, entryList->Count());
	CMsvCacheEntry *entry = NULL;
	TBool res = EFalse;
	for(TInt index=20; index<30; index++)
		{
		CMsvIndexAdapter::CNonCommittedAddedEntries* eMeta = (*entryList)[index-20];
		ASSERT_EQUALS(newId+index, (*entryList)[index-20]->entry->GetId());
		ASSERT_EQUALS(KMsvLocalServiceIndexEntryId, (*entryList)[index-20]->iVisibleFolder);
		TRAP_IGNORE(res = iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+index));
		MY_ASSERT_TRUE(res);
		MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(newId+index, entry));
		}

	// 2.c. Rolling back additions explicitly.
	iIndexAdapter->RollbackAdditions();
	
	// 2.d. Check the status.
	entryList = NULL;
	iIndexAdapter->GetNonComittedAddedEntry(entryList);
	MY_ASSERT_EQUALS(0, entryList->Count());
	entry = NULL;
	res = ETrue;
	for(TInt index=20; index<30; index++)
		{
		TRAP_IGNORE(res =iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+index));
		MY_ASSERT_FALSE(res)
		ASSERT_FALSE(iIndexAdapter->FindEntryInCache(newId+index, entry));
		}
	}




void CTestIndexAdapter::TestChangeEntryNoCommit()
	{
	_LIT(KFunction, "TestChangeEntryNoCommit");
	INFO_PRINTF1(KFunction);
	TSecureId aOwnerId = 0x999;
	CMsvCacheVisibleFolder *TestFolder = NULL;
	TFileName openFilename;
	TMsvEntry* entry2;
//	TMsvId visibleFolderId;
//	CMsvCacheEntry* testentry;
	
	// 1. Entry created with Commit to DB.
	sampleTMsvEntry = TMsvEntry();
	TMsvId newId = MaskTMsvId(1, KFirstFreeEntryId);
	AddEntry(ETrue, EFalse, newId);
	iIndexAdapter->LockEntry(newId);		
	// 1.1. With aForceUpdate True
	TInt32 mtm1 = 35;
	TInt32 mtm2 = 45;
	sampleTMsvEntry.SetMtmData1(mtm1);
	sampleTMsvEntry.SetMtmData2(mtm2);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntryNoCommit(sampleTMsvEntry, 0x999, ETrue));
	
	TMsvEntry* entry;
	iIndexAdapter->GetEntry(newId, entry);
	MY_ASSERT_EQUALS(entry->iMtmData1, sampleTMsvEntry.iMtmData1);
	MY_ASSERT_EQUALS(entry->iMtmData2, sampleTMsvEntry.iMtmData2);
	
	
	
	// 1.2. With aForceUpdate False
	mtm1 = 105;
	mtm2 = 205;
	sampleTMsvEntry.SetMtmData1(mtm1);
	sampleTMsvEntry.SetMtmData2(mtm2);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntryNoCommit(sampleTMsvEntry, 0x999, EFalse));
	
	TMsvEntry* entry1;
	iIndexAdapter->GetEntry(newId, entry1);
	MY_ASSERT_EQUALS(entry1->iMtmData1, sampleTMsvEntry.iMtmData1);
	MY_ASSERT_EQUALS(entry1->iMtmData2, sampleTMsvEntry.iMtmData2);
	
	
	// 2. Entry created with NO Commit to DB.
	AddEntry(EFalse, EFalse, newId+10);
	iIndexAdapter->LockEntry(newId+10);	
	// 2.1. With aForceUpdate True
	mtm1 = 35;
	mtm2 = 45;
	sampleTMsvEntry.SetMtmData1(mtm1);
	sampleTMsvEntry.SetMtmData2(mtm2);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntryNoCommit(sampleTMsvEntry, 0x999, ETrue));
	
	iIndexAdapter->GetEntry(newId +10, entry2);
	MY_ASSERT_EQUALS(entry2->iMtmData1, sampleTMsvEntry.iMtmData1);
	MY_ASSERT_EQUALS(entry2->iMtmData2, sampleTMsvEntry.iMtmData2);
	
	// 2.2. With aForceUpdate False
	mtm1 = 105;
	mtm2 = 205;
	sampleTMsvEntry.SetMtmData1(mtm1);
	sampleTMsvEntry.SetMtmData2(mtm2);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntryNoCommit(sampleTMsvEntry, 0x999, EFalse));
	
	TMsvEntry* entry3;
	iIndexAdapter->GetEntry(newId +10, entry3);
	MY_ASSERT_EQUALS(entry3->iMtmData1, sampleTMsvEntry.iMtmData1);
	MY_ASSERT_EQUALS(entry3->iMtmData2, sampleTMsvEntry.iMtmData2);
	iIndexAdapter->CommitNonCommitedEntries();
	
	
	// SOME TEST CASES ARE REMOVED BECAUSE THEY ARE NOT VALID
	// AFTER 557 IMPLEMENTATION. WE CANNOT CLOSE/DETACH A DB
	// WHEN THE TRANSACTION IS STILL OPEN.
	

	/* TEST Code added after changes in DOChangeEntry for rollback */
	// 6. Modifying 10 entries using no-commit.
	// 6.1. Adding 10 entry.
	for(TInt index=1100; index<1110; index++)
		{
		AddEntry(ETrue, EFalse, newId+index);
		}
	// 6.2. Modifying 10 entries using transaction.
	for(TInt index=1100; index<1110; index++)
		{
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(newId+index));
		TMsvEntry tEntry;
		tEntry.iDate.UniversalTime();
		tEntry.SetId(newId+index);
		tEntry.SetParent(KMsvLocalServiceIndexEntryId);
		tEntry.iType = KUidMsvMessageEntry;
		tEntry.SetMtmData1(index);
		MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
		}
	// 6.3. Verifying the state of entries before commit.
	RArray<CMsvIndexAdapter::TNonCommittedChangedEntries>& entryList = iIndexAdapter->GetNonComittedChangedEntry();
	MY_ASSERT_EQUALS(10, entryList.Count());
	CMsvDBAdapter* dbPtr = iIndexAdapter->GetDbAdapter();
	CMsvCacheEntry *cEntry = NULL;
	TMsvId visibleParentId;
	for(TInt index=1100; index<1110; index++)
		{
		TRAP_IGNORE(dbPtr->GetEntryL(newId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS((long)index, cEntry->Entry().MtmData1());
		MY_ASSERT_EQUALS(KUidMsvMessageEntry, cEntry->Entry().iType);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		
		iIndexAdapter->FindEntryInCache(newId+index, cEntry);
		MY_ASSERT_EQUALS((long)index, cEntry->Entry().MtmData1());
		MY_ASSERT_EQUALS(KUidMsvMessageEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(entryList[index-1100].iEntry, cEntry);
		}
	iIndexAdapter->CommitNonCommitedEntries();
	// 6.4. Verify the state of entries after commit.
	MY_ASSERT_EQUALS(NULL, entryList.Count());
	for(TInt index=1100; index<1110; index++)
		{
		TRAP_IGNORE(dbPtr->GetEntryL(newId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS((long)index, cEntry->Entry().MtmData1());
		MY_ASSERT_EQUALS(KUidMsvMessageEntry, cEntry->Entry().iType);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		
		iIndexAdapter->FindEntryInCache(newId+index, cEntry);
		MY_ASSERT_EQUALS((long)index, cEntry->Entry().MtmData1());
		MY_ASSERT_EQUALS(KUidMsvMessageEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->ReleaseEntry(newId+index));
		}
	
	// 7. Same as test case 6 but with rollback.
	// 7.1. Adding 10 entry.
	CMsvEntryFreePool *pool = CMsvEntryFreePool::Instance();
	for(TInt index=1200; index<1210; index++)
		{
		sampleTMsvEntry.SetMtmData2(index);
		AddEntry(ETrue, EFalse, newId+index);
		}
	// 7.2. Modifying 10 entries using transaction.
	for(TInt index=1200; index<1210; index++)
		{
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(newId+index));
		TMsvEntry tEntry;
		tEntry.iDate.UniversalTime();
		tEntry.SetId(newId+index);
		tEntry.SetParent(KMsvLocalServiceIndexEntryId);
		tEntry.iType = KUidMsvAttachmentEntry;
		tEntry.SetMtmData2(index+200);
		MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
		}
	// 7.3. Verifying the state of entries before rollback.
	MY_ASSERT_EQUALS(10, entryList.Count());
	dbPtr = iIndexAdapter->GetDbAdapter();
	for(TInt index=1200; index<1210; index++)
		{
		TRAP_IGNORE(dbPtr->GetEntryL(newId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS((long)(index+200), cEntry->Entry().MtmData2());
		MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		
		iIndexAdapter->FindEntryInCache(newId+index, cEntry);
		MY_ASSERT_EQUALS((long)(index+200), cEntry->Entry().MtmData2());
		MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(entryList[index-1200].iEntry, cEntry);
		}
	// 7.4. Try changing a non-existent entry, which should result in rollback.
	TMsvEntry tEntry;
	tEntry.SetId(newId+1500);
	tEntry.SetParent(KMsvRootIndexEntryIdValue);
	tEntry.iType = KUidMsvAttachmentEntry;
	MY_ASSERT_EQUALS(KErrNotFound, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
	// 7.5. Verify the state of entry after rollback.
	MY_ASSERT_EQUALS(NULL, entryList.Count());
	for(TInt index=1200; index<1210; index++)
		{
		TRAP_IGNORE(dbPtr->GetEntryL(newId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS((long)index, cEntry->Entry().MtmData2());
		MY_ASSERT_EQUALS(KUidMsvFolderEntry, cEntry->Entry().iType);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		
		iIndexAdapter->FindEntryInCache(newId+index, cEntry);
		MY_ASSERT_EQUALS((long)index, cEntry->Entry().MtmData2());
		MY_ASSERT_EQUALS(KUidMsvFolderEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->ReleaseEntry(newId+index));
		}

	// 8. Modifying parent Id of the entries, such that their
	// visibleFolderId is also changed.
	// 8.1. Add a new visibleFolder under local service.
	sampleTMsvEntry.SetVisible(ETrue);
	AddEntry(ETrue, EFalse, newId+1500);
	// 8.2. Modifying 10 entries using transaction.
	for(TInt index=1200; index<1210; index++)
		{
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(newId+index));
		TMsvEntry tEntry;
		tEntry.iDate.UniversalTime();
		tEntry.SetId(newId+index);
		tEntry.SetParent(newId+1500);
		tEntry.iType = KUidMsvAttachmentEntry;
		MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
		}
	// 8.3. Verifying the state of entries before commit.
	MY_ASSERT_EQUALS(10, entryList.Count());
	dbPtr = iIndexAdapter->GetDbAdapter();
	TestFolder = NULL;
	TDblQueIter<CMsvCacheVisibleFolder> fq(iIndexAdapter->GetVisibleFolderQueue());
	fq.SetToFirst();
	while((TestFolder = fq++) != NULL)
		{
		if(KMsvLocalServiceIndexEntryId == TestFolder->GetFolderId())
			{
			break;
			}
		}
    CMsvCacheVisibleFolder *NewTestFolder = NULL;
    fq.SetToFirst();
    while((NewTestFolder = fq++) != NULL)
        {
        if(newId+1500 == NewTestFolder->GetFolderId())
            {
            break;
            }
        }
    MY_ASSERT_TRUE(NewTestFolder!=NULL);
	MY_ASSERT_TRUE(TestFolder!=NULL);
	for(TInt index=1200; index<1210; index++)
		{
		// Check entry in DB.
		TRAP_IGNORE(dbPtr->GetEntryL(newId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS((long)(newId+1500), cEntry->Entry().Parent());
		MY_ASSERT_EQUALS((long)(newId+1500), visibleParentId);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		// Check entry in cache.
		MY_ASSERT_TRUE(TestFolder->GetEntry(newId+index, cEntry));
		MY_ASSERT_EQUALS(KUidMsvFolderEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS((long)KMsvLocalServiceIndexEntryId, cEntry->Entry().Parent());
		MY_ASSERT_TRUE(NewTestFolder->GetEntry(newId+index, cEntry));
        MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
        MY_ASSERT_EQUALS((long)(newId+1500), cEntry->Entry().Parent());
        // Check non committed entry list.
		MY_ASSERT_EQUALS(entryList[index-1200].iEntry, cEntry);
		MY_ASSERT_EQUALS(entryList[index-1200].iNewVisibleFolderNode->GetFolderId(), newId+1500);
		MY_ASSERT_EQUALS(entryList[index-1200].iOldVisibleFolderNode, TestFolder);
		}
	iIndexAdapter->CommitNonCommitedEntries();
	// 8.4. Verify the state of entry after commit.
	MY_ASSERT_EQUALS(NULL, entryList.Count());
	for(TInt index=1200; index<1210; index++)
		{
		// Check entry in DB.
		TRAP_IGNORE(dbPtr->GetEntryL(newId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(newId+1500, cEntry->Entry().Parent());
		MY_ASSERT_EQUALS(newId+1500, visibleParentId);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		// Check entry in cache.
		MY_ASSERT_FALSE(TestFolder->GetEntry(newId+index, cEntry));
		MY_ASSERT_TRUE(NewTestFolder->GetEntry(newId+index, cEntry));
		MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(newId+1500, cEntry->Entry().Parent());
		cEntry->ReleaseEntry();
		}
	
	// 9. Same as 8 but with rollback.
	// Entries now exists under newId+1500.
	// They will be modified such that they are under root.
	
	// 9.1. Modifying 10 entries using transaction.
	for(TInt index=1200; index<1210; index++)
		{
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(newId+index));
		TMsvEntry tEntry;
		tEntry.iDate.UniversalTime();
		tEntry.SetId(newId+index);
		tEntry.SetParent(KMsvLocalServiceIndexEntryId);
		tEntry.iType = KUidMsvFolderEntry;
		MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
		}
	// 9.2. Verifying the state of entries before rollback.
	MY_ASSERT_EQUALS(10, entryList.Count());
	TestFolder = NULL;
	fq.SetToFirst();
	while((TestFolder = fq++) != NULL)
		{
		if(newId+1500 == TestFolder->GetFolderId())
			{
			break;
			}
		}
    fq.SetToFirst();
    while((NewTestFolder = fq++) != NULL)
        {
        if(KMsvLocalServiceIndexEntryId == NewTestFolder->GetFolderId())
            {
            break;
            }
        }
    MY_ASSERT_TRUE(NewTestFolder!=NULL);
	MY_ASSERT_TRUE(TestFolder!=NULL);
	for(TInt index=1200; index<1210; index++)
		{
		// Check entry in DB.
		TRAP_IGNORE(dbPtr->GetEntryL(newId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS(KUidMsvFolderEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS((long)(KMsvLocalServiceIndexEntryId), cEntry->Entry().Parent());
		MY_ASSERT_EQUALS((long)KMsvLocalServiceIndexEntryId, visibleParentId);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		// Check entry in cache.
		MY_ASSERT_TRUE(TestFolder->GetEntry(newId+index, cEntry));
		MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS((long)(newId+1500), cEntry->Entry().Parent());
        MY_ASSERT_TRUE(NewTestFolder->GetEntry(newId+index, cEntry));
        MY_ASSERT_EQUALS(KUidMsvFolderEntry, cEntry->Entry().iType);
        MY_ASSERT_EQUALS((long)(KMsvLocalServiceIndexEntryId), cEntry->Entry().Parent());
		// Check non committed entry list.
		MY_ASSERT_EQUALS(entryList[index-1200].iEntry, cEntry);
		MY_ASSERT_EQUALS(entryList[index-1200].iNewVisibleFolderNode->GetFolderId(), (long)KMsvLocalServiceIndexEntryId);
		MY_ASSERT_EQUALS(entryList[index-1200].iOldVisibleFolderNode, TestFolder);
		}
	// 9.3. Try changing a non-existent entry, which should result in rollback.
	tEntry.SetId(newId+2000);
	tEntry.SetParent(KMsvRootIndexEntryIdValue);
	tEntry.iType = KUidMsvAttachmentEntry;
	MY_ASSERT_EQUALS(KErrNotFound, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
	// 9.4. Verify the state of entry after rollback.
	MY_ASSERT_EQUALS(NULL, entryList.Count());
	for(TInt index=1200; index<1210; index++)
		{
		// Check entry in DB.
		TRAP_IGNORE(dbPtr->GetEntryL(newId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(newId+1500, cEntry->Entry().Parent());
		MY_ASSERT_EQUALS(newId+1500, visibleParentId);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		// Check entry in cache.
		MY_ASSERT_TRUE(TestFolder->GetEntry(newId+index, cEntry));
		MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(newId+1500, cEntry->Entry().Parent());
		cEntry->ReleaseEntry();
		}
	
	// 10. Change parent of the entry. 
	// First five entries are children of newId+2000 (visibleFolder).
	// Next five entries are children of newId+2100 (non-visibleFolder).
	// Both newId+2000 and newId+2100 are children of local Service.
	// newId+2200 (Non-visibleFolder) is also children of localService.
	// Move first five entries from newId+2000 to newId+2200.
	// (parent and visibleFolderId both changed).
	// Move next five entries from newId+2100 to newId+2200.
	// (Only parent changed).
	
	// Children of root.
	sampleTMsvEntry.SetVisibleFolderFlag(ETrue);
	sampleTMsvEntry.SetVisible(ETrue);
	AddEntry(ETrue, EFalse, newId+2000);	// VisibleFolder
	sampleTMsvEntry.SetVisibleFolderFlag(EFalse);
	sampleTMsvEntry.SetVisible(EFalse);		
	AddEntry(ETrue, EFalse, newId+2100);	// Non-VisibleFolder
	AddEntry(ETrue, EFalse, newId+2200);	// Non-VisibleFolder
	for(TInt index=2001; index<2011; index++)
		{
		if(index<2006)
			AddEntry(ETrue, EFalse, newId+index, newId+2000);
		else
			AddEntry(ETrue, EFalse, newId+index, newId+2100);
		}
	CMsvEntrySelection *entrySelection = new CMsvEntrySelection;
	CMsvEntryFilter *dummyFilter = NULL;
	TRAP_IGNORE(dummyFilter = CMsvEntryFilter::NewL());
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->GetChildrenId(newId+2000, *dummyFilter, *entrySelection));
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->GetChildrenId(newId+2100, *dummyFilter, *entrySelection));
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->GetChildrenId(newId+2200, *dummyFilter, *entrySelection));
	delete entrySelection;
	delete dummyFilter;
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(newId+2100, cEntry));
	RArray<TMsvId> *childArr = cEntry->ChildIdArray();
	MY_ASSERT_TRUE(childArr!=NULL);
	MY_ASSERT_EQUALS(5, childArr->Count());
	for(TInt index=2006; index<2011; index++)
		{
		MY_ASSERT_EQUALS((*childArr)[index-2006], (long)(newId+index));
		}
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(newId+2200, cEntry));
	childArr = cEntry->ChildIdArray();
	MY_ASSERT_TRUE(childArr!=NULL);
	MY_ASSERT_EQUALS(0, childArr->Count());
	
	for(TInt index=2001; index<2011; index++)
		{
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(newId+index));
		TMsvEntry tEntry;
		tEntry.iDate.UniversalTime();
		tEntry.SetId(newId+index);
		tEntry.SetParent(newId+2200);
		tEntry.iType = KUidMsvFolderEntry;
		MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
		}
	// 10.2. Verifying the state of entries before rollback.
	MY_ASSERT_EQUALS(10, entryList.Count());
	TestFolder = NULL;
	NewTestFolder = NULL;
	fq.SetToFirst();
	while((TestFolder = fq++) != NULL)
		{
		if(newId+2000 == TestFolder->GetFolderId())
			{
			break;
			}
		}
	fq.SetToFirst();
	while((NewTestFolder = fq++) != NULL)
		{
		if(KMsvLocalServiceIndexEntryId == NewTestFolder->GetFolderId())
			{
			break;
			}
		}
	MY_ASSERT_TRUE(TestFolder!=NULL);
	MY_ASSERT_TRUE(NewTestFolder!=NULL);
	for(TInt index=2001; index<2011; index++)
		{
		// Check entry in DB.
		TRAP_IGNORE(dbPtr->GetEntryL(newId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS((long)(newId+2200), cEntry->Entry().Parent());
		MY_ASSERT_EQUALS((long)KMsvLocalServiceIndexEntryId, visibleParentId);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		// Check entry in cache.
		if(index < 2006)
			{
			MY_ASSERT_TRUE(TestFolder->GetEntry(newId+index, cEntry));
			MY_ASSERT_EQUALS((long)(newId+2000), cEntry->Entry().Parent());
			MY_ASSERT_EQUALS(entryList[index-2001].iOldVisibleFolderNode, TestFolder);
			MY_ASSERT_TRUE(NewTestFolder->GetEntry(newId+index, cEntry));
            MY_ASSERT_EQUALS((long)(newId+2200), cEntry->Entry().Parent());
            MY_ASSERT_EQUALS(entryList[index-2001].iNewVisibleFolderNode, NewTestFolder);
            }
		else
			{
			MY_ASSERT_TRUE(NewTestFolder->GetEntry(newId+index, cEntry));
			MY_ASSERT_EQUALS((long)(newId+2200), cEntry->Entry().Parent());
			MY_ASSERT_EQUALS(entryList[index-2001].iOldVisibleFolderNode, NewTestFolder);
			MY_ASSERT_EQUALS(entryList[index-2001].iNewVisibleFolderNode, (CMsvCacheVisibleFolder *)NULL);
			}
		// Check non committed entry list.
		MY_ASSERT_EQUALS(entryList[index-2001].iEntry, cEntry);
		}
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(newId+2100, cEntry));
	childArr = cEntry->ChildIdArray();
	MY_ASSERT_TRUE(childArr!=NULL);
	MY_ASSERT_EQUALS(5, childArr->Count());
	for(TInt index=2006; index<2011; index++)
		{
		MY_ASSERT_EQUALS((*childArr)[index-2006], (long)(newId+index));
		}
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(newId+2200, cEntry));
	childArr = cEntry->ChildIdArray();
	MY_ASSERT_TRUE(childArr!=NULL);
	MY_ASSERT_EQUALS(0, childArr->Count());
	
	iIndexAdapter->CommitNonCommitedEntries();
	// 10.4. Verify the state of entry after commit.
	MY_ASSERT_EQUALS(NULL, entryList.Count());
	for(TInt index=2001; index<2011; index++)
		{
		// Check entry in DB.
		TRAP_IGNORE(dbPtr->GetEntryL(newId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS(newId+2200, cEntry->Entry().Parent());
		MY_ASSERT_EQUALS((long)KMsvLocalServiceIndexEntryId, visibleParentId);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		// Check entry in cache.
		MY_ASSERT_FALSE(TestFolder->GetEntry(newId+index, cEntry));
		MY_ASSERT_TRUE(NewTestFolder->GetEntry(newId+index, cEntry));
		MY_ASSERT_EQUALS(newId+2200, cEntry->Entry().Parent());
		cEntry->ReleaseEntry();
		}
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(newId+2100, cEntry));
	childArr = cEntry->ChildIdArray();
	MY_ASSERT_TRUE(childArr!=NULL);
	MY_ASSERT_EQUALS(0, childArr->Count());
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(newId+2200, cEntry));
	childArr = cEntry->ChildIdArray();
	MY_ASSERT_TRUE(childArr!=NULL);
	MY_ASSERT_EQUALS(10, childArr->Count());
	for(TInt index=2001; index<2011; index++)
		{
		MY_ASSERT_TRUE(KErrNotFound != childArr->Find(newId+index));
		}
	
	//11. Scenario: Local service has 2 children, newId+5000 and newId+5100.
	//				newId+5000 is visible and the other is not visible.
	//				newId+5100 has 10 children and the other has none.
	//				newId+5100 is moved to under newId+5000.
	//				Visible folder of children of newId+5100 is changed.
	sampleTMsvEntry = TMsvEntry();
	sampleTMsvEntry.SetVisible(ETrue);
	sampleTMsvEntry.SetVisibleFolderFlag(ETrue);
	AddEntry(ETrue, EFalse, newId+5000, KMsvRootIndexEntryIdValue);
	sampleTMsvEntry.SetVisible(EFalse);
	sampleTMsvEntry.SetVisibleFolderFlag(EFalse);
	AddEntry(ETrue, EFalse, newId+5100, KMsvRootIndexEntryIdValue);
	for(TInt index = 0; index < 10; ++index)
		{
		AddEntry(ETrue, EFalse, newId+5200+index, newId+5100);
		}
	//11.1 Shift newId+5100 to under newId+5000.
	MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(newId+5100));
	tEntry.iDate.UniversalTime();
	tEntry.SetId(newId+5100);
	tEntry.SetParent(newId+5000);
	tEntry.iType = KUidMsvFolderEntry;
	tEntry.SetVisible(EFalse);
	tEntry.SetVisibleFolderFlag(EFalse);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
	//11.3 Commit the changes.
	iIndexAdapter->CommitNonCommitedEntries();
	//11.4 Check if the children of newId+5100 have visible parent
	//	   appropriately set.
	dbPtr = iIndexAdapter->GetDbAdapter();
	for(TInt index = 0; index < 10; ++index)
		{
		TRAP_IGNORE(dbPtr->GetEntryL(newId+index+5200, cEntry, visibleParentId));
		ASSERT_EQUALS(visibleParentId, newId+5000);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		}
	TRAP_IGNORE(dbPtr->GetEntryL(newId+5100, cEntry, visibleParentId));
	MY_ASSERT_EQUALS(visibleParentId, newId+5000);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
	
	//12. Scenario: Local Service has 2 children, newId+6000 and newId+6100.
	//				newId+6000 is visible and the other is not visible.
	//				newId+6100 has 10 children and the other has none.
	//				One of these grandchildren is visible and has a child.
	//				newId+6100 is moved to under newId+6000.
	//				Visible folder of children of newId+6100 is changed.
	//				Visible folder of grandchild should not be changed.
	sampleTMsvEntry = TMsvEntry();
	sampleTMsvEntry.SetVisible(ETrue);
	sampleTMsvEntry.SetVisibleFolderFlag(ETrue);
	sampleTMsvEntry.iType = KUidMsvFolderEntry;
	AddEntry(ETrue, EFalse, newId+6000, KMsvRootIndexEntryIdValue);
	sampleTMsvEntry.SetVisible(EFalse);
	sampleTMsvEntry.SetVisibleFolderFlag(EFalse);
	sampleTMsvEntry.iType = KUidMsvMessageEntry;
	AddEntry(ETrue, EFalse, newId+6100, KMsvRootIndexEntryIdValue);
	sampleTMsvEntry.SetVisible(ETrue);
	sampleTMsvEntry.SetVisibleFolderFlag(ETrue);
	sampleTMsvEntry.iType = KUidMsvFolderEntry;
	AddEntry(ETrue, EFalse, newId+6200, newId+6100); //child,
	sampleTMsvEntry.SetVisible(EFalse);
	sampleTMsvEntry.SetVisibleFolderFlag(EFalse);
	sampleTMsvEntry.iType = KUidMsvMessageEntry;
	AddEntry(ETrue, EFalse, newId+6300, newId+6200); //grandchild of 6100
	
	//12.1 Shift newId+6100 to under newId+6000.
	MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(newId+6100));
	TMsvEntry myEntry;
	myEntry.iDate.UniversalTime();
	myEntry.SetId(newId+6100);
	myEntry.SetParent(newId+6000);
	myEntry.iType = KUidMsvFolderEntry;
	myEntry.SetVisibleFolderFlag(EFalse);
	myEntry.SetVisible(EFalse);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(myEntry, 0x999, ETrue));
	//12.3 Commit the changes.
	iIndexAdapter->CommitNonCommitedEntries();
	//12.4 Check if the sub-tree of newId+6100 has visible parent
	//	   appropriately set.
	dbPtr = iIndexAdapter->GetDbAdapter();
	TRAP_IGNORE(dbPtr->GetEntryL(newId+6100, cEntry, visibleParentId));
	MY_ASSERT_EQUALS(visibleParentId, newId+6000);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
	
	TRAP_IGNORE(dbPtr->GetEntryL(newId+6200, cEntry, visibleParentId));
	MY_ASSERT_EQUALS(visibleParentId, newId+6000);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
	
	//TRAP_IGNORE(dbPtr->GetEntryL(newId+6300, cEntry, visibleParentId));
	//MY_ASSERT_EQUALS(visibleParentId, newId+6200);
	//CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
	
	//13. Moving a visible folder with children to under a non-visible folder which has
	//	  a visible parent.
	sampleTMsvEntry = TMsvEntry();
	sampleTMsvEntry.SetVisible(EFalse);
	sampleTMsvEntry.SetVisibleFolderFlag(EFalse);
	AddEntry(ETrue, EFalse, newId+7000, KMsvRootIndexEntryId);
	
	sampleTMsvEntry.SetVisible(ETrue);
	sampleTMsvEntry.SetVisibleFolderFlag(ETrue);
	AddEntry(ETrue, EFalse, newId+7100, KMsvRootIndexEntryId); //entry to be moved
	AddEntry(ETrue, EFalse, newId+7200, newId+7100);
	AddEntry(ETrue, EFalse, newId+7300, newId+7200);
	
	//13.1 Shift the visible folder to under the non-visible folder.
	MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(newId+7100));
	myEntry = TMsvEntry();
	myEntry.iDate.UniversalTime();
	myEntry.SetId(newId+7100);
	myEntry.SetParent(newId+7000);
	myEntry.iType = KUidMsvFolderEntry;
	myEntry.SetVisibleFolderFlag(ETrue);
	myEntry.SetVisible(ETrue);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(myEntry, 0x999, ETrue));
	//13.3 Commit the changes.
	iIndexAdapter->CommitNonCommitedEntries();
	//13.4 Check if the sub-tree of newId+7100 has visible parent
	//	   appropriately set.
	TRAP_IGNORE(dbPtr->GetEntryL(newId+7100, cEntry, visibleParentId));
	MY_ASSERT_EQUALS(visibleParentId, KMsvRootIndexEntryId);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
	
	TRAP_IGNORE(dbPtr->GetEntryL(newId+7200, cEntry, visibleParentId));
	MY_ASSERT_EQUALS(visibleParentId, newId+7100);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
	
	TRAP_IGNORE(dbPtr->GetEntryL(newId+7300, cEntry, visibleParentId));
	MY_ASSERT_EQUALS(visibleParentId, newId+7200);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
	}
	
	


// Added to test PREQ 557 methods
void CTestIndexAdapter::TestGetNextAvailableDriveId()
	{
	_LIT(KFunction, ": Inside TestGetNextAvailableDriveId.");
	INFO_PRINTF1(KFunction);

	// Precondition:
	// 1. Drives D, E & F should be mentioned in epoc.ini.
	// 2. msgprioritydrivelist.ini should have values C, D, E, F, G, H, I.
	// 3. Raw database should exist in D & F.
	// 4. The driveId assigned will be 1, 2, 999, 3, 999, 999, 999.
	
	TUint driveId;
	driveId = iIndexAdapter->GetNextAvailableDriveId();	
	MY_ASSERT_EQUALS((TUint)4, driveId);
	
	driveId = iIndexAdapter->GetNextAvailableDriveId();	
	MY_ASSERT_EQUALS((TUint)5, driveId);
	
	driveId = iIndexAdapter->GetNextAvailableDriveId();	
	MY_ASSERT_EQUALS((TUint)6, driveId);
	
	driveId = iIndexAdapter->GetNextAvailableDriveId();	
	MY_ASSERT_EQUALS((TUint)7, driveId);
	
	driveId = iIndexAdapter->GetNextAvailableDriveId();	
	MY_ASSERT_EQUALS((TUint)4, driveId);
	
	// Introducing drive id 5 & 6 in the preferred drive list.
	TRAPD(err, CMsvPreferredDriveList::GetDriveList()->UpdateDriveIdL(2, 5));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CMsvPreferredDriveList::GetDriveList()->UpdateDriveIdL(3, 6));	// Overwriting driveId 3.
	MY_ASSERT_EQUALS(KErrNone, err);
	
	driveId = iIndexAdapter->GetNextAvailableDriveId();	
	MY_ASSERT_EQUALS((TUint)7, driveId);
	
	// 3 should be available now.
	driveId = iIndexAdapter->GetNextAvailableDriveId();	
	MY_ASSERT_EQUALS((TUint)3, driveId);	
	}




void CTestIndexAdapter::ValidateIndexAdapterCreation()
	{
	_LIT(KFunction, ": Inside ValidateIndexAdapterCreation.");
	INFO_PRINTF1(KFunction);

	// Precondition:
	// 1. Drives D, E & F should be mentioned in epoc.ini.
	// 2. msgprioritydrivelist.ini should have values C, D, E, F, G, H, I.
	// 3. Raw database should exist in D & F.
	
	// Check DB Adapter creation.	
	MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()!=NULL);
	
	// Check MaxMsvId list.	
	MY_ASSERT_EQUALS(8, iIndexAdapter->iMaxMsvIdList.Count());
	MY_ASSERT_EQUALS(0l, iIndexAdapter->iMaxMsvIdList[0]);
	MY_ASSERT_EQUALS(MaskTMsvId(1, KFirstFreeEntryId), iIndexAdapter->iMaxMsvIdList[1]);
	MY_ASSERT_EQUALS(MaskTMsvId(2, KFirstFreeEntryId), iIndexAdapter->iMaxMsvIdList[2]);
	MY_ASSERT_EQUALS(MaskTMsvId(3, KFirstFreeEntryId), iIndexAdapter->iMaxMsvIdList[3]);
	MY_ASSERT_EQUALS(0l, iIndexAdapter->iMaxMsvIdList[4]);
	
	// Drive Id.
	MY_ASSERT_EQUALS((TUint)4, iIndexAdapter->iFirstFreeDriveId);
	
	// Root Entry.
	CMsvCacheEntry *entry = NULL;
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvRootIndexEntryId, entry));
	MY_ASSERT_TRUE(entry->Entry().Owner());
	
	// Check cache.
	CMsvEntrySelection *children = NULL;
	TRAPD(err, children = new(ELeave)CMsvEntrySelection);
	MY_ASSERT_EQUALS(err, KErrNone);
	CMsvEntryFilter *dummyFilter = NULL;
	TRAP_IGNORE(dummyFilter = CMsvEntryFilter::NewL());
	dummyFilter->SetOrder(TMsvSelectionOrdering(0, EMsvSortByNone, ETrue));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvLocalServiceIndexEntryId, entry));
	MY_ASSERT_TRUE(entry->Entry().Parent() == KMsvRootIndexEntryId);
		
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryId, entry));
	MY_ASSERT_TRUE(entry->Entry().Parent() == KMsvLocalServiceIndexEntryId);
		
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvGlobalOutBoxIndexEntryId, entry));
	MY_ASSERT_TRUE(entry->Entry().Parent() == KMsvLocalServiceIndexEntryId);
		
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvDraftEntryId, entry));
	MY_ASSERT_TRUE(entry->Entry().Parent() == KMsvLocalServiceIndexEntryId);
		
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvSentEntryId, entry));
	MY_ASSERT_TRUE(entry->Entry().Parent() == KMsvLocalServiceIndexEntryId);
		
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvDeletedEntryFolderEntryId, entry));
	MY_ASSERT_TRUE(entry->Entry().Parent() == KMsvLocalServiceIndexEntryId);
		
	iIndexAdapter->GetChildrenId(KMsvLocalServiceIndexEntryId, *dummyFilter, *children);
	MY_ASSERT_EQUALS(5, children->Count());
	children->Reset();
	
	iIndexAdapter->GetChildrenId(KMsvRootIndexEntryId, *dummyFilter, *children);
	MY_ASSERT_EQUALS(1, children->Count());
	MY_ASSERT_EQUALS(KMsvLocalServiceIndexEntryId, children->At(0));
	
	delete children;
	delete dummyFilter;
	
	TMsvIndexProgress progress = iIndexAdapter->Progress();
	MY_ASSERT_TRUE(progress.iTotal > 0);
	MY_ASSERT_TRUE(progress.iCompleted == progress.iTotal);
	MY_ASSERT_TRUE(progress.iRemaining == 0);
	iIndexAdapter->DoCancel();
	}




void CTestIndexAdapter::AddEntryToCacheOnly(TMsvId aId, TMsvId aParentId, TMsvId aVisibleFolderId, TBool aIsVisible /*DEFAULT=EFalse*/)
	{
	_LIT(KFunction, "AddEntryToCacheOnly.");
	TSecureId aOwnerId = 0x999;
	
	// Adding entry with false AutoAssigned
	sampleTMsvEntry.SetId(aId);
	sampleTMsvEntry.SetParent(aParentId);
	sampleTMsvEntry.iDate.UniversalTime();
	sampleTMsvEntry.iType = KUidMsvFolderEntry;
	sampleTMsvEntry.SetVisibleFolderFlag(aIsVisible);
	
	CMsvCacheEntry* newCacheEntry = Entry();	
	TRAP_IGNORE(newCacheEntry->SetEntryL(sampleTMsvEntry));
	newCacheEntry->SetEntryOwnerId(aOwnerId);
	
	TRAPD(err, iIndexAdapter->AddEntryToCacheL(aVisibleFolderId, newCacheEntry));
	MY_ASSERT_EQUALS(KErrNone, err);
	}



void CTestIndexAdapter::TestGetChildrenIdAll()
	{
	_LIT(KFunction, ": Inside TestGetChildrenIdAll.");
	INFO_PRINTF1(KFunction);

	CMsvEntryFilter *dummyFilter = NULL;
	TRAP_IGNORE(dummyFilter = CMsvEntryFilter::NewL());
	CMsvEntrySelection *children = NULL;
	TRAP_IGNORE(children = new(ELeave)CMsvEntrySelection);
	dummyFilter->SetOrder(TMsvSelectionOrdering(0, EMsvSortByNone, ETrue));
	
	// Inserting some non-standard children under Inbox of drive 1 and 2.
	for(TUint drive=1; drive<3; drive++)
		{
		AddEntryToCacheOnly(iIndexAdapter->NextId(drive), MaskTMsvId(drive, KMsvGlobalInBoxIndexEntryId), MaskTMsvId(drive, KMsvGlobalInBoxIndexEntryId), ETrue);
		for(TInt index=0; index<5; index++)
			{
			AddEntryToCacheOnly(iIndexAdapter->NextId(drive), MaskTMsvId(drive, KFirstFreeEntryId), MaskTMsvId(drive, KFirstFreeEntryId));			
			}
		}
	iIndexAdapter->GetChildrenIdAll(KMsvRootIndexEntryId, *dummyFilter, *children);
	MY_ASSERT_EQUALS(3, children->Count());
	
	children->Reset();
	iIndexAdapter->GetChildrenIdAll(MaskTMsvId(1, KMsvLocalServiceIndexEntryId), *dummyFilter, *children);
	MY_ASSERT_EQUALS(15, children->Count());	

	children->Reset();
	iIndexAdapter->GetChildrenIdAll(MaskTMsvId(1, KFirstFreeEntryId), *dummyFilter, *children);
//	MY_ASSERT_EQUALS(5, children->Count());
	
	delete dummyFilter;
	delete children;	
	}
	


	
void CTestIndexAdapter::TestChangeDrive()
	{
	_LIT(KFunction, ": Inside TestChangeDrive.");
	INFO_PRINTF1(KFunction);
	
	// 1. Change the current drive to a drive,
	// already in use (DB attached). The old
	// drive is not removed from the drive list.
	
	// Create non-standard visible folder in the current drive (C:).
	sampleTMsvEntry.SetVisible(ETrue);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvRootIndexEntryIdValue);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvRootIndexEntryIdValue);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvLocalServiceIndexEntryId);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvGlobalInBoxIndexEntryId);
	// Creating message entry under standard and non-standard visible folder.
	sampleTMsvEntry.SetVisible(EFalse);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), MaskTMsvId(1, KFirstFreeEntryId));
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), MaskTMsvId(1, KFirstFreeEntryId + 1));
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), MaskTMsvId(1, KFirstFreeEntryId + 2));
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), MaskTMsvId(1, KFirstFreeEntryId + 3));
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvGlobalInBoxIndexEntryId);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvGlobalInBoxIndexEntryId);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvGlobalInBoxIndexEntryId);	
	CMsvEntrySelection *entrySelection = new CMsvEntrySelection;
	CMsvEntryFilter *dummyFilter = NULL;
	TRAP_IGNORE(dummyFilter = CMsvEntryFilter::NewL());
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->GetChildrenId(KMsvGlobalInBoxIndexEntryId, *dummyFilter, *entrySelection));
	MY_ASSERT_EQUALS(entrySelection->Count(), 1);
	delete entrySelection;
	delete dummyFilter;
	
	// 1 (A)
	// Old Current Drive C: (Not removed completely)
	// New Current Drive D:	(DB already attached, does not already have non-std children).
	TRAPD(err, iIndexAdapter->ChangeDriveL(1, ETrue));
	MY_ASSERT_EQUALS(err, KErrNone);
	CMsvPreferredDriveList::GetDriveList()->SetCurrentDriveIndex(1);
	CMsvCacheEntry *entry = NULL;
	// Non-standard folder of C: should be removed.
	for(TInt i=0; i<8; i++)
		{
		MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+i), entry));
		}
	// Message entry and std folder of C: should be present in cache.
	TInt driveId = 4; // New Drive Id of C:
	entrySelection = new CMsvEntrySelection;
	TRAP_IGNORE(dummyFilter = CMsvEntryFilter::NewL());
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->GetChildrenId(MaskTMsvId(driveId, KMsvGlobalInBoxIndexEntryId), *dummyFilter, *entrySelection));
	delete entrySelection;
	delete dummyFilter;
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId+8), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId+9), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId+10), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvLocalServiceIndexEntryId, entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryId, entry));
	
	// Create non-standard visible folder in the current drive (D:).
	sampleTMsvEntry.SetVisible(ETrue);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvRootIndexEntryIdValue);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvRootIndexEntryIdValue);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvLocalServiceIndexEntryId);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvGlobalInBoxIndexEntryId);
	// Creating message entry under standard and non-standard visible folder.
	sampleTMsvEntry.SetVisible(EFalse);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), MaskTMsvId(1, KFirstFreeEntryId));
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), MaskTMsvId(1, KFirstFreeEntryId + 1));
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), MaskTMsvId(1, KFirstFreeEntryId + 2));
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), MaskTMsvId(1, KFirstFreeEntryId + 3));
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvGlobalInBoxIndexEntryId);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvGlobalInBoxIndexEntryId);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvGlobalInBoxIndexEntryId);
	// 1 (B)
	// Old Current Drive D: (Not removed completely)
	// New Current Drive C:	(DB already attached, already have non-std children)
	TRAP(err, iIndexAdapter->ChangeDriveL(0, ETrue));
	MY_ASSERT_EQUALS(err, KErrNone);
	CMsvPreferredDriveList::GetDriveList()->SetCurrentDriveIndex(0);
	// Non-std folder of D: should be removed.	
	driveId = 5; // New Drive Id of D:
	for(TInt i=0; i<8; i++)
		{
		MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId+i), entry));
		}		
	// Get inbox of D:
	entrySelection = new CMsvEntrySelection;
	TRAP_IGNORE(dummyFilter = CMsvEntryFilter::NewL());
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->GetChildrenId(MaskTMsvId(driveId, KMsvGlobalInBoxIndexEntryId), *dummyFilter, *entrySelection));
	delete entrySelection;
	delete dummyFilter;
	// Get inbox of C:
	entrySelection = new CMsvEntrySelection;
	TRAP_IGNORE(dummyFilter = CMsvEntryFilter::NewL());
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->GetChildrenId(MaskTMsvId(1, KMsvGlobalInBoxIndexEntryId), *dummyFilter, *entrySelection));
	delete entrySelection;
	delete dummyFilter;
	// Message entry and std folder of D: should be present in cache.
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId+8), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId+9), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId+10), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvLocalServiceIndexEntryId, entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryId, entry));
	// Std folder of C: (And message entry under Local/Root) should be present in cache.
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+1), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+2), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+3), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+8), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+9), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+10), entry));
	// Non-stad folder and message entry under Inbox and other std folder may not be present.
	MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+4), entry));
	MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+5), entry));
	MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+6), entry));
	MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+7), entry));
	// 2 (A).
	// Old Current Drive C: (Removed completely)
	// New Current Drive D:	(DB already attached)
	TRAP(err, iIndexAdapter->ChangeDriveL(1, EFalse));
	MY_ASSERT_EQUALS(err, KErrNone);	
	CMsvPreferredDriveList::GetDriveList()->SetCurrentDriveIndex(1);
	// All entries of C: is removed.
	
	// Fetch children of D:\inbox
	entrySelection = new CMsvEntrySelection;
	TRAP_IGNORE(dummyFilter = CMsvEntryFilter::NewL());
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->GetChildrenId(MaskTMsvId(1, KMsvGlobalInBoxIndexEntryId), *dummyFilter, *entrySelection));
	delete entrySelection;
	delete dummyFilter;
	// Std folder of D: (And message entry under Local/Root) should be present in cache.
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+1), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+2), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+3), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+8), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+9), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+10), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvLocalServiceIndexEntryId, entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryId, entry));
	// Non-stand folder and message entry under Inbox and other std folder may not be present.
	MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+4), entry));
	MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+5), entry));
	MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+6), entry));
	MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+7), entry));
	// 2 (B).
	// Old Current Drive D: (Removed completely)
	// New Current Drive C:	(DB NOT already attached)
	// New Drive id (4) assigned to C:
	// Update Drive status of C: to EMsvMessageStoreAvailableStatus
	TRAP_IGNORE(CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(0, EMsvMessageStoreAvailableStatus));
	TRAP(err, iIndexAdapter->ChangeDriveL(0, EFalse));
	MY_ASSERT_EQUALS(err, KErrNone);
	CMsvPreferredDriveList::GetDriveList()->SetCurrentDriveIndex(0);
	// All entries of D: is removed. D does not have a new drive Id.

	// Std entries of C: and all children of local/root is present in cache.
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+1), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+2), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvLocalServiceIndexEntryId, entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryId, entry));
	// Non-stad folder and message entry under Inbox and other std folder may not be present.
	for(TInt i=3; i<11; i++)
		{
		MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+i), entry));
		}	

	// 3. Change the current drive to a drive,
	// not already in use (DB NOT attached). The old
	// drive is not removed from the drive list.
	// Old Drive C: (Not removed completely)
	// New drive D: (Not already attached)
	// New id 5 is assigned to D:
	TRAP_IGNORE(CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(1, EMsvMessageStoreAvailableStatus));
	TRAP(err, iIndexAdapter->ChangeDriveL(1, ETrue));
	MY_ASSERT_EQUALS(err, KErrNone);
	CMsvPreferredDriveList::GetDriveList()->SetCurrentDriveIndex(1);

	// All entries except std entries of C: is present in cache.
	driveId = 6;	// new drive Id of C:
	for(TInt i=0; i<11; i++)
		{
		MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId+i), entry));
		}
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvLocalServiceIndexEntryId, entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryId, entry));
	// Std entries of D: and all children of local/root is present in cache.
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+1), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+2), entry));
	// Non-stad folder and message entry under Inbox and other std folder may not be present.
	for(TInt i=3; i<11; i++)
		{
		MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+i), entry));
		}	
	}




void CTestIndexAdapter::TestReloadCache()
	{
	_LIT(KFunction, ": Inside TestReloadCacheL.");
	INFO_PRINTF1(KFunction);
	
	// Create non-standard visible folder in the current drive (C:).
	sampleTMsvEntry.SetVisible(ETrue);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvRootIndexEntryIdValue);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvRootIndexEntryIdValue);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvLocalServiceIndexEntryId);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvGlobalInBoxIndexEntryId);
	// Creating message entry under standard and non-standard visible folder.
	sampleTMsvEntry.SetVisible(EFalse);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), MaskTMsvId(1, KFirstFreeEntryId));
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), MaskTMsvId(1, KFirstFreeEntryId + 1));
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), MaskTMsvId(1, KFirstFreeEntryId + 2));
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), MaskTMsvId(1, KFirstFreeEntryId + 3));
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvGlobalInBoxIndexEntryId);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvGlobalInBoxIndexEntryId);
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvGlobalInBoxIndexEntryId);
	
	// Create some entries only in DB.
	// Create (KFirstFreeEntryId+11 & 12) msg entry under Inbox.
	TMsvEntry sampleEntry;
	TMsvId visibleFolder = KMsvGlobalInBoxIndexEntryId;
	sampleEntry.SetId(iIndexAdapter->NextId(1));
	sampleEntry.iType = KUidMsvMessageEntry;
	sampleEntry.SetParent(visibleFolder);
	sampleEntry.SetVisible(EFalse);
	CMsvDBAdapter *dbAdapter = iIndexAdapter->GetDbAdapter();
	TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, visibleFolder));
	sampleEntry.SetId(iIndexAdapter->NextId(1));
	TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, visibleFolder));
	// Create (KFirstFreeEntryId+13) visibleFolder entry under Inbox.
	sampleEntry.SetVisible(ETrue);
	sampleEntry.SetId(iIndexAdapter->NextId(1));
	TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, visibleFolder));
	// Create (KFirstFreeEntryId+14) visibleFolder entry under Local.
	visibleFolder = KMsvLocalServiceIndexEntryId;
	sampleEntry.SetId(iIndexAdapter->NextId(1));
	sampleEntry.SetParent(visibleFolder);
	TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, visibleFolder));
	
	TUint currDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	
	TRAPD(err, iIndexAdapter->RemoveDriveL(KCurrentDriveId, currDriveIndex, EFalse));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, CMsvPreferredDriveList::GetDriveList()->UpdateDriveIdL(currDriveIndex, KCurrentDriveId));
	MY_ASSERT_EQUALS(err, KErrNone);

	TRAP_IGNORE(iIndexAdapter->ReloadCacheL());
	CMsvCacheEntry* entry = NULL;
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvLocalServiceIndexEntryId, entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryId, entry));
	for(TInt i=3; i<14; i++)
		{
		MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+i), entry));
		}	
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+1), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+2), entry));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+14), entry));
	}




void CTestIndexAdapter::TestSetLocalServiceComplete()
	{
	_LIT(KFunction, "TestSetLocalServiceComplete");
	INFO_PRINTF1(KFunction);
	
	CMsvCacheVisibleFolder* visFolder = CMsvCacheVisibleFolder::NewL(KFirstFreeEntryId);
	iIndexAdapter->iFolderListHeader.AddFirst(*visFolder);

	iIndexAdapter->SetLocalServiceComplete();
	}
	
	
	
void CTestIndexAdapter::TestToImproveCoverage()
	{
	_LIT(KFunction, "TestToImproveCoverage");
	INFO_PRINTF1(KFunction);
	
	TBool locked = ETrue;
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvRootIndexEntryIdValue);
	TInt err = iIndexAdapter->IsEntryLocked(KFirstFreeEntryId+1000, locked);
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	err = iIndexAdapter->IsEntryLocked(KFirstFreeEntryId, locked);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	err = iIndexAdapter->LockStore(KFirstFreeEntryId+1000);
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	err = iIndexAdapter->LockStore(KFirstFreeEntryId);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	err = iIndexAdapter->ReleaseStore(KFirstFreeEntryId+1000);
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	err = iIndexAdapter->ReleaseStore(KFirstFreeEntryId);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	err = iIndexAdapter->LockEntryAndStore(KFirstFreeEntryId+1000);
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	err = iIndexAdapter->LockEntryAndStore(KFirstFreeEntryId);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	err = iIndexAdapter->ReleaseEntryAndStore(KFirstFreeEntryId+1000);
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	err = iIndexAdapter->ReleaseEntryAndStore(KFirstFreeEntryId);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	err = iIndexAdapter->IsEntryOrStoreLocked(KFirstFreeEntryId+1000, locked);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	err = iIndexAdapter->IsEntryOrStoreLocked(KFirstFreeEntryId, locked);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	err = iIndexAdapter->IncStoreReaderCount(KFirstFreeEntryId+1000);
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	err = iIndexAdapter->IncStoreReaderCount(KFirstFreeEntryId);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	err = iIndexAdapter->DecStoreReaderCount(KFirstFreeEntryId+1000);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	err = iIndexAdapter->DecStoreReaderCount(KFirstFreeEntryId);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CMsvCacheEntry* entry = Entry();
	iIndexAdapter->UpdateDates(*entry, ETrue);
	iIndexAdapter->UpdateDates(*entry, EFalse);
	iIndexAdapter->iNonCommittedChangedEntryList.Reset();
	iIndexAdapter->RollbackChanges();
	
	CMsvDBAdapter* dbPtr = iIndexAdapter->iDbAdapter;
	iIndexAdapter->iDbAdapter = NULL;
	iIndexAdapter->BeginTransaction();
	iIndexAdapter->CommitTransaction();
	iIndexAdapter->RollbackAdditions();
	iIndexAdapter->RollbackChanges();
	
	TMsvId siblingId;
	TRAP(err, iIndexAdapter->GetNextSiblingL(KFirstFreeEntryId, KFirstFreeEntryId+10, siblingId));
	TRAP(err, iIndexAdapter->GetNextSiblingL(KFirstFreeEntryId, KFirstFreeEntryId+10, siblingId));
	
	TRAP(err, iIndexAdapter->GetFirstChildIdL(KFirstFreeEntryId, siblingId));
	TRAP(err, iIndexAdapter->GetFirstChildIdL(KFirstFreeEntryId, siblingId));	
	iIndexAdapter->iDbAdapter = dbPtr;
	}
	
		



void CTestIndexAdapter::TestDoForceDeleteEntry()
	{
	_LIT(KFunction, "TestDoForceDeleteEntry");
	INFO_PRINTF1(KFunction);
	
	CMsvDBAdapter* dbPtr = iIndexAdapter->iDbAdapter;
	iIndexAdapter->iDbAdapter = NULL;
	TRAPD(err, iIndexAdapter->DoForceDeleteEntryL(KFirstFreeEntryId));
	iIndexAdapter->iDbAdapter = dbPtr;
	
	TUint driveId = 1;
	AddEntry(ETrue, EFalse, iIndexAdapter->NextId(1), KMsvGlobalInBoxIndexEntryIdValue);
	CMsvCacheVisibleFolder* folderNode = NULL;
	TDblQueIter<CMsvCacheVisibleFolder> dqIter(iIndexAdapter->iFolderListHeader);
	dqIter.SetToFirst();
    TMsvId id = KMsvGlobalInBoxIndexEntryIdValue;
    while ((folderNode = dqIter++)!=NULL)
        {
        if(folderNode->GetFolderId() == id)
        	{
        	break;
        	}
        }
    MY_ASSERT_FALSE(folderNode==NULL);
	folderNode->iDlink.Deque();
    iIndexAdapter->iFolderListHeader.AddLast(*folderNode);
	TRAP(err, iIndexAdapter->DoForceDeleteEntryL((driveId<<28)|KFirstFreeEntryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TSecureId aOwnerId = 0x999;
	sampleTMsvEntry.SetParent(KMsvGlobalInBoxIndexEntryIdValue);
	sampleTMsvEntry.iType = KUidMsvFolderEntry;
	sampleTMsvEntry.SetVisible(ETrue);
	sampleTMsvEntry.SetVisibleFolderFlag(ETrue);
	TRAP(err, iIndexAdapter->AddEntry(sampleTMsvEntry, aOwnerId, ETrue));
	sampleTMsvEntry.SetParent((driveId<<28)|KFirstFreeEntryId+1);
	sampleTMsvEntry.SetVisible(EFalse);
	sampleTMsvEntry.SetVisibleFolderFlag(EFalse);
	TRAP(err, iIndexAdapter->AddEntry(sampleTMsvEntry, aOwnerId, ETrue));
	sampleTMsvEntry.SetParent((driveId<<28)|KFirstFreeEntryId+2);
	TRAP(err, iIndexAdapter->AddEntry(sampleTMsvEntry, aOwnerId, ETrue));
   	TRAP(err, iIndexAdapter->DoForceDeleteEntryL((driveId<<28)|KFirstFreeEntryId+3));
	MY_ASSERT_EQUALS(err, KErrNone);

	sampleTMsvEntry.SetParent((driveId<<28)|KFirstFreeEntryId+2);
	TRAP(err, iIndexAdapter->AddEntry(sampleTMsvEntry, aOwnerId, ETrue));

	dqIter.SetToFirst();
    id = (driveId<<28)|(KFirstFreeEntryId+1);
    while ((folderNode = dqIter++)!=NULL)
        {
        if(folderNode->GetFolderId() == id)
        	{
        	break;
        	}
        }
    MY_ASSERT_FALSE(folderNode==NULL);
    TRAP(err, folderNode->DeleteEntryL((driveId<<28)|(KFirstFreeEntryId+2), ETrue));
    TRAP(err, folderNode->DeleteEntryL((driveId<<28)|(KFirstFreeEntryId+4), ETrue));
   	TRAP(err, iIndexAdapter->DoForceDeleteEntryL((driveId<<28)|KFirstFreeEntryId+4));
	MY_ASSERT_EQUALS(err, KErrNone);
	}
	
	
	
	
	
void CTestIndexAdapter::TestDoRemoveDeletedEntries()
	{
	_LIT(KFunction, "TestDoRemoveDeletedEntries");
	INFO_PRINTF1(KFunction);
	
	TRAPD(err, iIndexAdapter->DoRemoveDeletedEntriesL());
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TSecureId aOwnerId = 0x999;
	sampleTMsvEntry.SetParent(KMsvDeletedEntryFolderEntryId);
	sampleTMsvEntry.iType = KUidMsvFolderEntry;
	sampleTMsvEntry.SetVisibleFolderFlag(ETrue);
	sampleTMsvEntry.SetDeleted(ETrue);
	sampleTMsvEntry.IncPcSyncCount();	
	for(TInt i=0; i<5; i++)
		{
		sampleTMsvEntry.DecPcSyncCount();
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->AddEntry(sampleTMsvEntry, aOwnerId, ETrue));
		sampleTMsvEntry.IncPcSyncCount();
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->AddEntry(sampleTMsvEntry, aOwnerId, ETrue));
		}
	TRAP(err, iIndexAdapter->DoRemoveDeletedEntriesL());
	MY_ASSERT_EQUALS(err, KErrNone);	
	}
	
	


/**
 * TestGetChildren()
 *
 *
 * ---Test cases 1-6 on a visible folder.
 * 1. Visible folder present in folderlist, and is complete.
 * 2. Visible folder present in folderlist, is complete and has few blocks dirty.
 * 3. Visible folder present in folderlist, is complete and all blocks are dirty.
 * 4. Visible folder present in folderlist and is not complete. Has few grandchildren
      and children in cache.
 * 5. Visible folder not present in folderlist but it is in cache.
 * 6. Visible folder not present in cache at all.
 * ---Test cases 7-13 on non-visible folder.
 * 7. Folder's parent does not exist.
 * 8. Folder's parent is not in cache.
 * 9. Parent is in cache, GetChildren has not been performed.
 * 10. Parent is in cache, GetChildren has been performed, but folder has no children.
 * 11. Parent is in cache, GetChildren has been performed and all children are in cache.
 * 12. Parent is in cache, GetChildren has been performed but few children not in cache.
 * 13. Parent is in cache, GetChildren has been performed and no children are in cache.
 * ---Test cases 14-19 are based on filtering criteria.
 *
 */
void CTestIndexAdapter::TestGetChildren()
	{
	_LIT(KFunction, "TestGetChildren");
	INFO_PRINTF1(KFunction);
	
	TInt err, ret;
	CMsvDBAdapter *dbAdapter = NULL;
	CMsvCacheEntry *entryF = NULL;
	CMsvCacheVisibleFolder *TestFolder = NULL;
	CMsvCacheVisibleFolder *parent = NULL;
	TMsvEntry visEntry, sampleEntry;
	TMsvId visibleFolder;
	TMsvId id = 100, tempId, sampleId, maxId, minId;
	RPointerArray<CMsvCacheIndexTableEntry>* indexTable = NULL;
	RPointerArray<CMsvCacheEntry> addEntryPointer;
	CArrayPtrSeg<const TMsvEntry>* aSelection = NULL;
	TRAP_IGNORE(aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	TMsvSelectionOrdering dummyOrder;
	TMsvSelectionOrdering order;
	RPointerArray<CMsvCacheEntry>* blockPtr;
		
	
	// ---------------------- Tests 1-6 on a visible folder
	// 1. Visible folder present in folderlist, and is complete.
	id = MaskTMsvId(1, 26100);
	sampleId = id;
	tempId = MaskTMsvId(1, 26035);
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	TDblQueIter<CMsvCacheVisibleFolder> fq(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq.SetToFirst();
	while((TestFolder = fq++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(tempId);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}		
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}		
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, 26150), maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, 26151), minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, 26200), maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, 26201), minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, 26250), maxId);
	
	entryF = NULL;
	delete aSelection;
			
	// 2. Visible folder present in folderlist, is complete and has few blocks dirty.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = MaskTMsvId(1, 25500);
	sampleId = id;
	tempId = MaskTMsvId(1, 25000);
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	TDblQueIter<CMsvCacheVisibleFolder> fq1(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq1.SetToFirst();
	while((TestFolder = fq1++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	TestFolder->ReleaseAllBlocks(ret);
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(tempId);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	(*indexTable)[1]->ReleaseBlock(); //release one block to make it dirty
	MY_ASSERT_TRUE((*indexTable)[1]->IsDirty());
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, 25550), maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, 25551), minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, 25600), maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, 25601), minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, 25650), maxId);
	
	entryF = NULL;
	delete aSelection;
	
	// 3. Visible folder present in folderlist, is complete and all blocks are dirty.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = MaskTMsvId(1, KFirstFreeEntryId+800);
	sampleId = id;
	tempId = MaskTMsvId(1, KFirstFreeEntryId+1035);
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	TDblQueIter<CMsvCacheVisibleFolder> fq2(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq2.SetToFirst();
	while((TestFolder = fq2++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(tempId);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	TestFolder->ReleaseAllBlocks(ret); //release all blocks to make them dirty
	ASSERT_EQUALS(ret, 150);
	MY_ASSERT_TRUE(TestFolder->IsAllBlocksDirty());
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, KFirstFreeEntryId+850), maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, KFirstFreeEntryId+851), minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, KFirstFreeEntryId+900), maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, KFirstFreeEntryId+901), minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, KFirstFreeEntryId+950), maxId);
	
	entryF = NULL;
	delete aSelection;
	
	// 4. Visible folder present in folderlist and is not complete. Has few grandchildren
    //    and children in cache.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	id = MaskTMsvId(1, KFirstFreeEntryId+100);
	sampleId = id;
	tempId = MaskTMsvId(1, KFirstFreeEntryId+35);
	
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	
	TDblQueIter<CMsvCacheVisibleFolder> fq3(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq3.SetToFirst();
	while((TestFolder = fq3++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	TestFolder->ReleaseAllBlocks(ret);
	
	sampleTMsvEntry.iType = KUidMsvFolderEntry;
	sampleTMsvEntry.SetVisible(EFalse);
	AddEntry(ETrue, EFalse, MaskTMsvId(1, KFirstFreeEntryId+40)); //a non-visible folder, parent of grandchildren
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(MaskTMsvId(1, KFirstFreeEntryId+35));
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 2*bulkCount; ++index) //not all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(1, KFirstFreeEntryId+35));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 0; index < bulkCount; ++index)
		{
		++id;
		}

	TestFolder->SetComplete(EFalse);
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //child entries

	ASSERT_EQUALS(err, KErrNone);
	indexTable = TestFolder->GetIndexTableEntry();
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	addEntryPointer.Reset();
	tempId = MaskTMsvId(1, KFirstFreeEntryId+40);

	for(TInt index = 0; index < 10; ++index) //create grandchild entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(MaskTMsvId(1, KFirstFreeEntryId+40));
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 10; ++index) //all grandchildren in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(1, KFirstFreeEntryId+40));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}

	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //grandchild entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_FALSE(TestFolder->IsComplete());
	indexTable = TestFolder->GetIndexTableEntry();
	tempId = MaskTMsvId(1, KFirstFreeEntryId+35);

	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, KFirstFreeEntryId+150), maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, KFirstFreeEntryId+151), minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, KFirstFreeEntryId+200), maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, KFirstFreeEntryId+201), minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, KFirstFreeEntryId+260), maxId); //grandchild and missing child entries in last block
	
	// 5. Visible folder not present in folderlist but it is in cache.
	delete aSelection;
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = MaskTMsvId(1, KFirstFreeEntryId+2500);
	sampleId = id;
	tempId = MaskTMsvId(1, KFirstFreeEntryId+2030); //parent
	sampleTMsvEntry.SetVisible(ETrue);
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	sampleTMsvEntry.SetVisible(ETrue);
	AddEntry(ETrue, EFalse, MaskTMsvId(1, KFirstFreeEntryId+2035), tempId); //test folder
	addEntryPointer.Reset();
	//-since visible folder is not in folderlist it is listed under its parent
	TDblQueIter<CMsvCacheVisibleFolder> fq4(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq4.SetToFirst();
	while((parent = fq4++) != NULL) //find parent
		{
		if(tempId == parent->GetFolderId())
			{
			break;
			}
		}
	tempId = MaskTMsvId(1, KFirstFreeEntryId+2030);
	for(TInt index = 0; index < bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(tempId);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().iType = KUidMsvMessageEntry;
		entryF->Entry().SetId (++id);
		TRAP_IGNORE(addEntryPointer.AppendL(entryF));
		}
	TRAP(err, parent->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	visibleFolder = MaskTMsvId(1, KFirstFreeEntryId+2035);
	for(TInt index = 0; index < 10; ++index) //10 child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(MaskTMsvId(1, KFirstFreeEntryId+2035));
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, visibleFolder));
		}
	
	TRAP(err, iIndexAdapter->GetChildrenL(MaskTMsvId(1, KFirstFreeEntryId+2035), *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	TDblQueIter<CMsvCacheVisibleFolder> fq5(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq5.SetToFirst();
	visibleFolder = MaskTMsvId(1, KFirstFreeEntryId+2035);
	while((TestFolder = fq5++) != NULL) //find the folder
		{
		if(visibleFolder == TestFolder->GetFolderId())
			{
			break;
			}
		}
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 1);
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, KFirstFreeEntryId+2560), maxId);
	
	entryF = NULL;
	delete aSelection;
		
	
	// 6. Visible folder is not present in cache.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = MaskTMsvId(1, KFirstFreeEntryId+3500);
	sampleId = id;
	tempId = MaskTMsvId(1, KFirstFreeEntryId+3030);
	dbAdapter = iIndexAdapter->GetDbAdapter();
	// Add 3030 to cache & DB.
	AddEntry(ETrue, EFalse, tempId); //parent visible folder

	// Add 3035 (visible child of 3030) to Db only.
	sampleEntry.SetVisible(ETrue); //the folder to be tested
	sampleEntry.SetParent(MaskTMsvId(1, KFirstFreeEntryId+3030));
	sampleEntry.SetId(MaskTMsvId(1, KFirstFreeEntryId+3035));
	sampleEntry.iType = KUidMsvFolderEntry;
	TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, MaskTMsvId(1, KFirstFreeEntryId+3030))); //create test folder in D)B
	
	// Create children of 3035 in DB.
	addEntryPointer.Reset();
	visibleFolder = MaskTMsvId(1, KFirstFreeEntryId+3035);
	for(TInt index = 0; index < 10; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(1, KFirstFreeEntryId+3035));
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, visibleFolder));
		}

	// Get Children of 3035.	
	TRAP(err, iIndexAdapter->GetChildrenL(MaskTMsvId(1, KFirstFreeEntryId+3035), *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	TDblQueIter<CMsvCacheVisibleFolder> fq7(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq7.SetToFirst();
	while((TestFolder = fq7++) != NULL) //find test folder
		{
		if(visibleFolder == TestFolder->GetFolderId())
			{
			break;
			}
		}
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 1);
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS(MaskTMsvId(1, KFirstFreeEntryId+3510), maxId);
	
	entryF = NULL;
	delete aSelection;

	// ----------------- Tests 7-13 on a non-visible folder
	
	// 8. Folder's parent is not in cache.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 4500;
	sampleId = id;
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+4020);
	dbAdapter = iIndexAdapter->GetDbAdapter();
	AddEntry(ETrue, EFalse, tempId); //parent visible folder
	entryF = Entry();
	entryF->Entry().SetParent(KMsvRootIndexEntryId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(ETrue);
	entryF->Entry().SetId(tempId);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create parent entry under root
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+4030);
	AddEntry(ETrue, EFalse, tempId); //parent visible folder
	entryF = Entry();
	entryF->Entry().SetParent(MaskTMsvId(1, KFirstFreeEntryId+4020));
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetId(tempId);

	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(MaskTMsvId(1, 20), entryF));//create parent entry under root
	TDblQueIter<CMsvCacheVisibleFolder> fq8(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq8.SetToFirst();
	tempId = MaskTMsvId(1, KFirstFreeEntryId+4020);
	while((parent = fq8++) != NULL) //find parent of parent
		{
		if(tempId == parent->GetFolderId())
			{
			break;
			}
		}

	parent->ReleaseAllBlocks(ret); //release all blocks, now parent is not in cache
	sampleTMsvEntry.SetVisible(EFalse);
	AddEntry(ETrue, EFalse, MaskTMsvId(1, KFirstFreeEntryId+4035), tempId); //non-visible test folder
	entryF = Entry();
	entryF->Entry().SetParent(MaskTMsvId(1, KFirstFreeEntryId+4030));
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetId(MaskTMsvId(1, KFirstFreeEntryId+4035));
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(MaskTMsvId(1, KFirstFreeEntryId+4020), entryF));//create test folder
	
	addEntryPointer.Reset();
	visibleFolder = MaskTMsvId(1, KFirstFreeEntryId+4035);
	for(TInt index = 0; index < 10; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent(MaskTMsvId(1, KFirstFreeEntryId+4035));
		sampleEntry.iType = KUidMsvMessageEntry;
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, visibleFolder));
		}
	for (TInt index = 0; index < 10; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(1, KFirstFreeEntryId+4035));
		entryF->Entry().iType = KUidMsvMessageEntry;
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	
	TRAP(err, parent->AddEntryListL(addEntryPointer, EFalse)); //child entries of test folder
	ASSERT_EQUALS(err, KErrNone);	
	ASSERT_FALSE(parent->IsComplete());
	
	TRAP(err, iIndexAdapter->GetChildrenL(MaskTMsvId(1, KFirstFreeEntryId+4035), *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 10);
	
	entryF = NULL;
	delete aSelection;
	
	// 9. Parent is in cache, GetChildren has not been performed.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = KFirstFreeEntryId+5500;
	sampleId = id;

	tempId = MaskTMsvId(1, KFirstFreeEntryId+5030);
	dbAdapter = iIndexAdapter->GetDbAdapter();
	AddEntry(ETrue, EFalse, tempId); //parent visible folder
	entryF = Entry();
	entryF->Entry().SetParent(KMsvRootIndexEntryId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(ETrue);
	entryF->Entry().SetId(tempId);
	
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create parent entry under root
	TDblQueIter<CMsvCacheVisibleFolder> fq9(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq9.SetToFirst();
	while((parent = fq9++) != NULL) //find parent
		{
		if(tempId == parent->GetFolderId())
			{
			break;
			}
		}
	parent->ReleaseAllBlocks(ret);
	sampleTMsvEntry.SetVisible(EFalse);
	AddEntry(ETrue, EFalse, MaskTMsvId(1, KFirstFreeEntryId+5035), tempId); //test folder

	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetId(MaskTMsvId(1, KFirstFreeEntryId+5035));
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create test folder under parent
	addEntryPointer.Reset();
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+5035);
	for(TInt index = 0; index < bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(MaskTMsvId(1, KFirstFreeEntryId+5035));
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+5035);
	for (TInt index = 0; index < bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(1, KFirstFreeEntryId+35));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	
	TRAP(err, parent->AddEntryListL(addEntryPointer, EFalse)); //child entries of test folder
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_FALSE(parent->IsComplete());
	
	TRAP(err, iIndexAdapter->GetChildrenL(MaskTMsvId(1, KFirstFreeEntryId+5035), *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 50);
	
	entryF = NULL;
	delete aSelection;

	
	// 10. Parent is in cache, GetChildren has been performed, but folder has no children.
/*	aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	id = MaskTMsvId(1, 6500);
	sampleId = id;
	
	tempId = MaskTMsvId(1, 6030);
	AddEntry(ETrue, EFalse, tempId); //parent visible folder

	sampleTMsvEntry.SetVisible(EFalse);
	AddEntry(ETrue, EFalse, MaskTMsvId(1, 6035), tempId); //test folder
	TDblQueIter<CMsvCacheVisibleFolder> fq10(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq10.SetToFirst();
	while((parent = fq10++) != NULL) //find parent
		{
		if(tempId == parent->GetFolderId())
			{
			break;
			}
		}
	addEntryPointer.Reset();
	TRAP(err, iIndexAdapter->GetChildrenL(MaskTMsvId(1, 6035), *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 0);
	
	entryF = NULL;
	delete aSelection;
		
*/	
	// 11. Parent is in cache, GetChildren has been performed and all children are in cache.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = KFirstFreeEntryId+7500;
	sampleId = id;
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+7030);
	dbAdapter = iIndexAdapter->GetDbAdapter();
	AddEntry(ETrue, EFalse, tempId); //parent visible folder
	entryF = Entry();
	entryF->Entry().SetParent(KMsvRootIndexEntryId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(ETrue);
	entryF->Entry().SetId(tempId);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create parent entry under root
	
	TDblQueIter<CMsvCacheVisibleFolder> fq11(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq11.SetToFirst();
	while((parent = fq11++) != NULL) //find parent
		{
		if(tempId == parent->GetFolderId())
			{
			break;
			}
		}
	parent->ReleaseAllBlocks(err);
	sampleTMsvEntry.SetVisible(EFalse);
	AddEntry(ETrue, EFalse, MaskTMsvId(1, KFirstFreeEntryId+7035), tempId); //test folder
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetId(MaskTMsvId(1, KFirstFreeEntryId+7035));
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create test folder under parent
	addEntryPointer.Reset();
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+7035);
	for(TInt index = 0; index < bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(MaskTMsvId(1, KFirstFreeEntryId+7035));
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+7035);
	for(TInt index = 0; index < bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(1, KFirstFreeEntryId+35));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	
	TRAP(err, parent->AddEntryListL(addEntryPointer, EFalse)); //child entries of test folder
	ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, iIndexAdapter->GetChildrenL(MaskTMsvId(1, KFirstFreeEntryId+7035), *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 50);
	
	entryF = NULL;
	delete aSelection;
		
	
	// 12. Parent is in cache, GetChildren has been performed but few children not in cache.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = KFirstFreeEntryId+8500;
	sampleId = id;
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+8030);
	dbAdapter = iIndexAdapter->GetDbAdapter();
	AddEntry(ETrue, EFalse, tempId); //parent visible folder
	entryF = Entry();
	entryF->Entry().SetParent(KMsvRootIndexEntryId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(ETrue);
	entryF->Entry().SetId(tempId);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create parent entry under root
	
	TDblQueIter<CMsvCacheVisibleFolder> fq12(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq12.SetToFirst();
	while((parent = fq12++) != NULL) //find parent
		{
		if(tempId == parent->GetFolderId())
			{
			break;
			}
		}
	parent->ReleaseAllBlocks(err);
	sampleTMsvEntry.SetVisible(EFalse);
	AddEntry(ETrue, EFalse, MaskTMsvId(1, KFirstFreeEntryId+8035), tempId); //test folder
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetId(MaskTMsvId(1, KFirstFreeEntryId+8035));
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create test folder under parent
	addEntryPointer.Reset();
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+8030);
	for(TInt index = 0; index < 2*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(tempId);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for(TInt index = 0; index < 2*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, parent->AddEntryListL(addEntryPointer, ETrue)); //child entries of test folder
	ASSERT_EQUALS(err, KErrNone);
	addEntryPointer.Reset();
	for(TInt index = 0; index < bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(MaskTMsvId(1, KFirstFreeEntryId+8035));
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	tempId = MaskTMsvId(1, KFirstFreeEntryId+8035);
	for(TInt index = 0; index < bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(MaskTMsvId(1, KFirstFreeEntryId+35));
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, parent->AddEntryListL(addEntryPointer, ETrue)); //child entries of test folder
	ASSERT_EQUALS(err, KErrNone);
	indexTable = parent->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	(*indexTable)[2]->ReleaseBlock(); //2nd and last (3rd) block have children
	
	TRAP(err, iIndexAdapter->GetChildrenL(MaskTMsvId(1, KFirstFreeEntryId+8035), *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, bulkCount);
	
	entryF = NULL;
	delete aSelection;
	
	// 13. Parent is in cache, GetChildren has been performed but no children not in cache.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = KFirstFreeEntryId+9500;
	sampleId = id;
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+9030);
	dbAdapter = iIndexAdapter->GetDbAdapter();
	AddEntry(ETrue, EFalse, tempId); //parent visible folder
	entryF = Entry();
	entryF->Entry().SetParent(KMsvRootIndexEntryId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(ETrue);
	entryF->Entry().SetId(tempId);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create parent entry under root
	
	TDblQueIter<CMsvCacheVisibleFolder> fq13(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq13.SetToFirst();
	while((parent = fq13++) != NULL) //find parent
		{
		if(tempId == parent->GetFolderId())
			{
			break;
			}
		}
	parent->ReleaseAllBlocks(err);
	sampleTMsvEntry.SetVisible(EFalse);
	
	AddEntry(ETrue, EFalse, MaskTMsvId(1, KFirstFreeEntryId+9035), tempId); //test folder
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetId(MaskTMsvId(1, KFirstFreeEntryId+9035));
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create test folder under parent
	addEntryPointer.Reset();
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+9030);
	for(TInt index = 0; index < 2*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(tempId);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for(TInt index = 0; index < 2*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, parent->AddEntryListL(addEntryPointer, ETrue)); //child entries of test folder
	ASSERT_EQUALS(err, KErrNone);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 10; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(MaskTMsvId(1, KFirstFreeEntryId+9035));
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	tempId = MaskTMsvId(1, KFirstFreeEntryId+9035);
	for(TInt index = 0; index < 10; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, parent->AddEntryListL(addEntryPointer, ETrue)); //child entries of test folder
	ASSERT_EQUALS(err, KErrNone);
	indexTable = parent->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 2);
	(*indexTable)[1]->ReleaseBlock();
	
	TRAP(err, iIndexAdapter->GetChildrenL(MaskTMsvId(1, KFirstFreeEntryId+9035), *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 10);
	
	entryF = NULL;
	delete aSelection;
	
	
	// ---------------------- Tests 14-19 are with filtering and sorting criteria
	
	// 14. Order in reverse by Id, filter by mtm
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = KFirstFreeEntryId+10500;
	sampleId = id;
	tempId = MaskTMsvId(1, KFirstFreeEntryId+10035);
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	
	TDblQueIter<CMsvCacheVisibleFolder> fq14(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq14.SetToFirst();
	while((TestFolder = fq14++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.iMtm = KUidMsvMessageEntry;
		sampleEntry.SetParent(MaskTMsvId(1, KFirstFreeEntryId+10035));
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+10035);
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().iMtm = KUidMsvMessageEntry;
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	order.SetSorting(EMsvSortByIdReverse); //sort by id in reverse order
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, order, KUidMsvMessageEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 150);
	
	entryF = NULL;
	delete aSelection;
	
	
	// 15. Order in reverse by Id, filter by mtm
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = KFirstFreeEntryId+11500;
	sampleId = id;
	tempId = MaskTMsvId(1, KFirstFreeEntryId+11035);
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	
	TDblQueIter<CMsvCacheVisibleFolder> fq15(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq15.SetToFirst();
	while((TestFolder = fq15++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			sampleEntry.iMtm = KUidMsvFolderEntry;
			}
		sampleEntry.SetParent(MaskTMsvId(1, KFirstFreeEntryId+11035));
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+11035);
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		entryF->Entry().iType = KUidMsvMessageEntry;
		entryF->Entry().iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			entryF->Entry().iMtm = KUidMsvFolderEntry;
			}
		addEntryPointer.Append(entryF);
		} //last 25 entries are folder entries
	
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	order.SetSorting(EMsvSortByIdReverse); //sort by id in reverse order
	
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, order, KUidMsvFolderEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 150);
	
	indexTable = TestFolder->GetIndexTableEntry();
	blockPtr = (*indexTable)[2]->BlockPtr();
	for(TInt index = 25; index < blockPtr->Count(); ++index)
		{
		ASSERT_TRUE((*blockPtr)[index]->Entry().iMtm == KUidMsvFolderEntry);
		}
	for(TInt index = 0; index < 25; ++index) //First 25 entries are folder entries in the selection
		{
		const TMsvEntry* entry = aSelection->At(index);
		ASSERT_TRUE(entry->iMtm == KUidMsvFolderEntry);
		ASSERT_TRUE(entry->Id() > (aSelection->At(index + 1))->Id()); //Ids are descending order
		}
	
	entryF = NULL;
	delete aSelection;
	
	// 16. Order by date in reverse order, filter by mtm
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = KFirstFreeEntryId+12500;
	sampleId = id;
	tempId = MaskTMsvId(1, KFirstFreeEntryId+12035);
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	
	TDblQueIter<CMsvCacheVisibleFolder> fq16(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq16.SetToFirst();
	while((TestFolder = fq16++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.iMtm = KUidMsvMessageEntry;
		sampleEntry.SetParent(MaskTMsvId(1, 12035));
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+12035);
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		entryF->Entry().iType = KUidMsvMessageEntry;
		entryF->Entry().iMtm = KUidMsvMessageEntry;
		addEntryPointer.Append(entryF);
		} 
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	order.SetSorting(EMsvSortByDateReverse); //sort by id in reverse order
	
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, order, KUidMsvMessageEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 150);
	for(TInt index = 0; index < aSelection->Count() - 1; ++index)
		{
		const TMsvEntry* entry = aSelection->At(index);
		ASSERT_TRUE(entry->iDate >= (aSelection->At(index + 1))->iDate);
		}
	
	entryF = NULL;
	delete aSelection;
	
	
	// 17. Sort and filter only by mtm
	TRAP(err ,aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = KFirstFreeEntryId+13500;
	sampleId = id;
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+13035);
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	
	TDblQueIter<CMsvCacheVisibleFolder> fq17(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq17.SetToFirst();
	while((TestFolder = fq17++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			sampleEntry.iMtm = KUidMsvFolderEntry;
			}
		sampleEntry.SetParent(MaskTMsvId(1, KFirstFreeEntryId+13035));
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+13035);
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		entryF->Entry().iType = KUidMsvMessageEntry;
		entryF->Entry().iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			entryF->Entry().iMtm = KUidMsvFolderEntry;
			}
		addEntryPointer.Append(entryF);
		} 
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	order.SetSorting(EMsvSortByNone); 
	
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, order, KUidMsvFolderEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 150);
	for(TInt index = 0; index < aSelection->Count() - 1; ++index)
		{
		const TMsvEntry* entry = aSelection->At(index);
		if(index < 125)
			{
			ASSERT_TRUE(entry->iMtm == KUidMsvMessageEntry);
			}
		else
			{
			ASSERT_TRUE(entry->iMtm == KUidMsvFolderEntry);
			}
		
		}
	
	entryF = NULL;
	delete aSelection;
	
	
	// 18. Sort/filter by mtm 
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = KFirstFreeEntryId+14500;
	sampleId = id;
	
	tempId = MaskTMsvId(1, KFirstFreeEntryId+14035);
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	
	TDblQueIter<CMsvCacheVisibleFolder> fq18(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq18.SetToFirst();
	while((TestFolder = fq18++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			sampleEntry.iMtm = KUidMsvFolderEntry;
			}
		if(index >= 130)
			{
			sampleEntry.iMtm = KUidMsvServiceEntry;
			}
		sampleEntry.SetParent(MaskTMsvId(1, KFirstFreeEntryId+14035));
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	tempId = MaskTMsvId(1, KFirstFreeEntryId+14035);
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		entryF->Entry().iType = KUidMsvMessageEntry;
		entryF->Entry().iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			entryF->Entry().iMtm = KUidMsvFolderEntry;
			}
		if(index >= 130)
			{
			entryF->Entry().iMtm = KUidMsvServiceEntry;
			}
		addEntryPointer.Append(entryF);
		} 
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	order.SetSorting(EMsvSortByNone); //no sorting
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, order, KUidMsvServiceEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 150);
	for(TInt index = 0; index < aSelection->Count() - 1; ++index)
		{
		const TMsvEntry* entry = aSelection->At(index);
		if(index < 125)
			{
			ASSERT_TRUE(entry->iMtm == KUidMsvMessageEntry);
			}
		else if( index >=125 && index <130)
			{
			ASSERT_TRUE(entry->iMtm == KUidMsvFolderEntry);
			}
		else
			{
			ASSERT_TRUE(entry->iMtm == KUidMsvServiceEntry);
			}
		
		}
	
	entryF = NULL;
	delete aSelection;
	
	
	// 19. Revers order of Ids, sort by mtm
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = KFirstFreeEntryId+15500;
	sampleId = id;

	tempId = MaskTMsvId(1, KFirstFreeEntryId+15035);
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	
	TDblQueIter<CMsvCacheVisibleFolder> fq19(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq19.SetToFirst();
	while((TestFolder = fq19++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			sampleEntry.iMtm = KUidMsvFolderEntry;
			}
		if(index >= 130)
			{
			sampleEntry.iMtm = KUidMsvServiceEntry;
			}
		sampleEntry.SetParent(MaskTMsvId(1, KFirstFreeEntryId+15035));
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}

	tempId = MaskTMsvId(1, KFirstFreeEntryId+15035);
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		entryF->Entry().iType = KUidMsvMessageEntry;
		entryF->Entry().iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			entryF->Entry().iMtm = KUidMsvFolderEntry;
			}
		if(index >= 130)
			{
			entryF->Entry().iMtm = KUidMsvServiceEntry;
			}
		addEntryPointer.Append(entryF);
		} 
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	addEntryPointer.Close();
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	order.SetSorting(EMsvSortByIdReverse); // reverse order of Ids
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, order, KUidMsvServiceEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 150);
	for(TInt index = 0; index < aSelection->Count() - 1; ++index)
		{
		const TMsvEntry* entry = aSelection->At(index);
		if(index < 20)
			{
			ASSERT_TRUE(entry->iMtm ==KUidMsvServiceEntry );
			}
		else if( index >=20 && index <25)
			{
			ASSERT_TRUE(entry->iMtm == KUidMsvFolderEntry);
			}
		else
			{
			ASSERT_TRUE(entry->iMtm == KUidMsvMessageEntry);
			}
		
		}
	
	entryF = NULL;
	delete aSelection;
	//resetting
	sampleTMsvEntry.SetVisible(ETrue);
	sampleTMsvEntry.iType = KUidMsvFolderEntry;
	sampleTMsvEntry.iDate.UniversalTime();
	
	/***** TEST CASE ADDED FOR FOLLOWING SPECIAL USE CASE ****/
	// When all entries fetched by GetChildren() cannot be 
	// accomodated in cache.

	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	// Destroy the server object.
	delete iServer;
	iServer = 0;
	// Delete the database.
	TFileName dbName(KDbFileName);
	RSqlDatabase::Delete(dbName);
	
	// Use a low cache configuration file for cache configuration.
	RFs iFileHandle;
	err = iFileHandle.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT(KSTDMSGINI, "C:\\private\\1000484b\\msgcache.ini");
	_LIT(KMSGINI_1, "C:\\private\\1000484b\\msgcache1.ini");
	CFileMan* file = NULL;
	TRAP_IGNORE(file = CFileMan::NewL(iFileHandle));
	err = file->Copy(KMSGINI_1, KSTDMSGINI);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// Create the server object with the new cache configuration.
	TRAP(err, iServer = CMsvServer::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);
	iIndexAdapter = iServer->Context().IndexAdapter();
	MY_ASSERT_EQUALS(80, CMsvEntryFreePool::Instance()->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(805, CMsvEntryFreePool::Instance()->GetMaxEntries());
	MY_ASSERT_EQUALS(0, CMsvEntryFreePool::Instance()->GetUsedEntriesCount());
	
	TMsvEntry entry;
	TMsvId aVisibleFolder;
	TMsvId newId = MaskTMsvId(1, KFirstFreeEntryId);
	for(TInt index=0; index<1500; index++)
		{
		entry.SetId(newId+index);
		entry.SetParent(MaskTMsvId(1, KMsvGlobalInBoxIndexEntryId));
		entry.iType = KUidMsvMessageEntry;
		//TRAP(err, iIndexAdapter->GetDbAdapter()->CreateEntryL(entry, aVisibleFolder));
		aVisibleFolder = KMsvGlobalInBoxIndexEntryId;
		TRAP(err, iIndexAdapter->GetDbAdapter()->CreateEntryL(entry, aVisibleFolder));
		//above change made after tweaking for CreateEntry use case
		ASSERT_EQUALS(KErrNone, err);
		}
	
	TRAP(err, iIndexAdapter->GetChildrenL(MaskTMsvId(1, KMsvGlobalInBoxIndexEntryId), *aSelection, dummyOrder, KUidMsvNullEntry));	
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(aSelection->Count(), 1500);
	for(TInt index=0; index<1500; index++)
		{
		ASSERT_EQUALS(MaskTMsvId(1, KFirstFreeEntryId+index), aSelection->At(index)->Id());		
		}
	
	// for PREQ557 root is made a cache entry**
	MY_ASSERT_EQUALS(1507, CMsvEntryFreePool::Instance()->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(702, CMsvEntryFreePool::Instance()->GetUsedEntriesCount());
	MY_ASSERT_EQUALS(805, CMsvEntryFreePool::Instance()->GetMaxEntries());
	
	CMsvEntryFreePool::Instance()->FlushExcessMemory();
	MY_ASSERT_EQUALS(805, CMsvEntryFreePool::Instance()->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(805, CMsvEntryFreePool::Instance()->GetMaxEntries());
	MY_ASSERT_EQUALS(0, CMsvEntryFreePool::Instance()->GetUsedEntriesCount());
	
	CMsvCacheEntry* cEntry = NULL;
	MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(newId, cEntry));
	MY_ASSERT_TRUE(NULL==cEntry);
	MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(newId+701, cEntry));
	MY_ASSERT_TRUE(NULL==cEntry);	
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(newId+702, cEntry));
	MY_ASSERT_FALSE(NULL==cEntry);
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(newId+703, cEntry));
	MY_ASSERT_FALSE(NULL==cEntry);
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(newId+704, cEntry));
	MY_ASSERT_FALSE(NULL==cEntry);
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(newId+1499, cEntry));
	MY_ASSERT_FALSE(NULL==cEntry);
	
	delete aSelection;
	
	// Reverting file renaming.
	err = file->Copy(KMSGINI_2, KSTDMSGINI);
	MY_ASSERT_EQUALS(err, KErrNone);	
	delete file;
	iFileHandle.Close();
	}



void CTestIndexAdapter::TestGetChildrenId()
	{
	_LIT(KFunction, "TestGetChildrenId");
	
	sampleTMsvEntry.SetVisible(ETrue);
	sampleTMsvEntry.SetVisible(ETrue);
	for(int i=0; i<10; i++)
		AddEntry(ETrue, EFalse, MaskTMsvId(1, KFirstFreeEntryId+i), KMsvGlobalInBoxIndexEntryId);
		
	CMsvEntrySelection *entrySelection = new CMsvEntrySelection;
	CMsvEntryFilter *dummyFilter = NULL;
	TRAP_IGNORE(dummyFilter = CMsvEntryFilter::NewL());
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->GetChildrenId(KMsvGlobalInBoxIndexEntryId, *dummyFilter, *entrySelection));
	
	for(TInt i= 0; i<10; ++i)
		{
		MY_ASSERT_EQUALS(MaskTMsvId(1, KFirstFreeEntryId + i), entrySelection->At(i));
		}
	delete dummyFilter;
	delete entrySelection;
	}



void CTestIndexAdapter::TestAddEntry()
	{
	_LIT(KFunction, ": Inside TestAddEntry");
	INFO_PRINTF1(KFunction);
	
	//Init
	TSecureId aOwnerId = 0x999;
	TFileName openFilename;

	// 1. Adding entry with AutoAssigned value False
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+1000);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+1001);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+1002);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+1003);

	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvRootIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvLocalServiceIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvGlobalInBoxIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvGlobalOutBoxIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvDraftEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvSentEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvDeletedEntryFolderEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+1000));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+1001));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+1002));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+1003));
	MY_ASSERT_FALSE(iIndexAdapter->EntryExists(KFirstFreeEntryId+2));	
	
	
	// 2. Test AddEntry where parent is non-visible folder
	TMsvEntry sampleTMsvEntry1;
	sampleTMsvEntry1.iDate.UniversalTime();
	sampleTMsvEntry1.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry1.SetVisible(EFalse);
	sampleTMsvEntry1.iType = KUidMsvFolderEntry;
	sampleTMsvEntry1.SetId(KFirstFreeEntryId+1050);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry1, aOwnerId, EFalse));

	AddEntry(ETrue, EFalse, KFirstFreeEntryId+2000, KFirstFreeEntryId+1050);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+2001, KFirstFreeEntryId+1050);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+2002, KFirstFreeEntryId+1050);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+2003, KFirstFreeEntryId+1050);

	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvRootIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvLocalServiceIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvGlobalInBoxIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvGlobalOutBoxIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvDraftEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvSentEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvDeletedEntryFolderEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+1050));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+2000));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+2001));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+2002));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+2003));
	MY_ASSERT_FALSE(iIndexAdapter->EntryExists(KFirstFreeEntryId+2004));
		
	// 3. Adding entry with AutoAssigned value True
	AddEntry(ETrue, ETrue);
	AddEntry(ETrue, ETrue);
	AddEntry(ETrue, ETrue);
	AddEntry(EFalse, ETrue);
	AddEntry(EFalse, ETrue);
		
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvRootIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvLocalServiceIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvGlobalInBoxIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvGlobalOutBoxIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvDraftEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvSentEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvDeletedEntryFolderEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(MaskTMsvId(KCurrentDriveId, KFirstFreeEntryId)));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(MaskTMsvId(KCurrentDriveId, KFirstFreeEntryId+1)));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(MaskTMsvId(KCurrentDriveId, KFirstFreeEntryId+2)));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(MaskTMsvId(KCurrentDriveId, KFirstFreeEntryId+3)));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(MaskTMsvId(KCurrentDriveId, KFirstFreeEntryId+4)));
	MY_ASSERT_FALSE(iIndexAdapter->EntryExists(MaskTMsvId(KCurrentDriveId, KFirstFreeEntryId+5)));
	iIndexAdapter->CommitNonCommitedEntries();
	
	//4. Add Entry using Transaction
/*	a. Open Transaction
 * 	b. Add entry with no commit
 * 	c. Check if the entry is present
 * 	d. close the transaction
 * 	e. close the database
 * 	f. open the database and check if the entry exists
*/
	//a. Open Transaction
	//b. Add entry with no commit

	TMsvPreferredDrive driveEntry;
	TRAPD(err, CMsvPreferredDriveList::GetDriveList()->DriveInfoL(1, driveEntry));	
	MY_ASSERT_EQUALS(KErrNone, err);

	TMsvEntry sampleTMsvEntry3;
	TMsvId entryId = MaskTMsvId(driveEntry.driveId, (KFirstFreeEntryId+77));
	TBool res = ETrue;
	TRAP_IGNORE(res = iIndexAdapter->GetDbAdapter()->EntryExistsL(entryId));
	MY_ASSERT_FALSE(res);
	MY_ASSERT_FALSE(iIndexAdapter->EntryExists(entryId));
	
	sampleTMsvEntry3.iDate.UniversalTime();
	sampleTMsvEntry3.SetParent(MaskTMsvId(driveEntry.driveId, KMsvLocalServiceIndexEntryId));
	sampleTMsvEntry3.SetVisible(EFalse);
	sampleTMsvEntry3.iType = KUidMsvFolderEntry;
	sampleTMsvEntry3.SetId(entryId);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntryNoCommit(sampleTMsvEntry3, aOwnerId, EFalse));

	//c. Check if the entry is present
	res = EFalse;
	TRAP_IGNORE(res = iIndexAdapter->GetDbAdapter()->EntryExistsL(entryId));
	MY_ASSERT_TRUE(res);
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(entryId));
	
	//d. close the transaction
	TRAP(err,iIndexAdapter->CommitNonCommitedEntries());
	
	//e. close the database
	//delete iIndexAdapter->GetDbAdapter();
	TRAP(err, iIndexAdapter->GetDbAdapter()->DetachDBL(driveEntry.driveId));
	MY_ASSERT_EQUALS(KErrNone, err)
		
	//f. open the database and check if the entry exists
	TMsvId maxId;
	TRAP(err, iIndexAdapter->GetDbAdapter()->AttachDBL(driveEntry.driveNum, driveEntry.driveId, maxId));
	MY_ASSERT_EQUALS(KErrNone, err);
	res = EFalse;
	TRAP_IGNORE(res = iIndexAdapter->GetDbAdapter()->EntryExistsL(entryId));
	MY_ASSERT_TRUE(res);
		
//5. Add Entry using Transaction
/*	a. Open Transaction
	b. Add entry with no commit
	c. Check if the entry is present
	d. close the database
	e. open the database and check if the entry exists
	f. check if the non-committed entry is present

	//a. Open Transaction
	//b. Add entry with no commit
	
	TMsvEntry sampleTMsvEntry9;
	sampleTMsvEntry9.iDate.UniversalTime();
	sampleTMsvEntry9.SetParent(MaskTMsvId(driveEntry.driveId, KMsvLocalServiceIndexEntryId));
	sampleTMsvEntry9.SetVisible(EFalse);
	sampleTMsvEntry9.iType = KUidMsvFolderEntry;
	sampleTMsvEntry9.SetId(++entryId);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntryNoCommit(sampleTMsvEntry9, aOwnerId, EFalse));

	//c. Check if the entry is present
	MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(entryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(entryId));
		
	//d. close the database
	TRAP(err, iIndexAdapter->GetDbAdapter()->DetachDBL(driveEntry.driveId));
	MY_ASSERT_EQUALS(KErrNone, err)
		
	//e. open the database and check if the entry exists
	TRAP(err, iIndexAdapter->GetDbAdapter()->AttachDBL(driveEntry.driveNum, driveEntry.driveId, maxId));
	MY_ASSERT_EQUALS(KErrNone, err)	

	//f. check if the non-committed entry is present
	MY_ASSERT_FALSE(iIndexAdapter->GetDbAdapter()->EntryExistsL(entryId));	
*/	}



#else    // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

// PRE-557 Implementation.


void CTestIndexAdapter::TestAddEntryNoCommit()
	{
	_LIT(KFunction, "TestAddEntryNoCommit");
	
	// 2. Adding bulk entry using transaction.
	// Single entry insertion fails. Entry insertion should rollback.
	// 2.a. Adding 10 entries using AddEntryNoCommit()
	for(TInt index=20; index<30; index++)
		{
		AddEntry(EFalse, EFalse, KFirstFreeEntryId+index);
		}
	
	// 2.b. Check the status of NonComittedAddedEntry list.
	RPointerArray<CMsvIndexAdapter::CNonCommittedAddedEntries> *entryList = NULL;
	iIndexAdapter->GetNonComittedAddedEntry(entryList);
	MY_ASSERT_EQUALS(10, entryList->Count());
	CMsvCacheEntry *entry = NULL;
	TInt res = EFalse;
	for(TInt index=20; index<30; index++)
		{
		ASSERT_EQUALS(KFirstFreeEntryId+index, (*entryList)[index-20]->entry->GetId());
		ASSERT_EQUALS((long)KMsvRootIndexEntryIdValue, (*entryList)[index-20]->iVisibleFolder);
		TRAP_IGNORE(res = iIndexAdapter->GetDbAdapter()->EntryExistsL(KFirstFreeEntryId+index));
		MY_ASSERT_TRUE(res);
		MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+index, entry));
		}

	// 2.c. Rolling back additions explicitly.
	iIndexAdapter->RollbackAdditions();
	
	// 2.d. Check the status.
	entryList = NULL;
	iIndexAdapter->GetNonComittedAddedEntry(entryList);
	MY_ASSERT_EQUALS(0, entryList->Count());
	entry = NULL;
	res = ETrue;
	for(TInt index=20; index<30; index++)
		{
		TRAP_IGNORE(res = iIndexAdapter->GetDbAdapter()->EntryExistsL(KFirstFreeEntryId+index));
		ASSERT_FALSE(res);
		ASSERT_FALSE(iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+index, entry));
		}
	}




void CTestIndexAdapter::TestChangeEntryNoCommit()
	{
	_LIT(KFunction, "TestChangeEntryNoCommit");
	INFO_PRINTF1(KFunction);
	TSecureId aOwnerId = 0x999;
	CMsvCacheVisibleFolder *TestFolder = NULL;
	TFileName openFilename;
	TMsvEntry* entry2;
	TMsvId visibleFolderId;
	CMsvCacheEntry* testentry;
	
	// 1. Entry created with Commit to DB.
	sampleTMsvEntry = TMsvEntry();
	AddEntry(ETrue, EFalse, KFirstFreeEntryId);
	iIndexAdapter->LockEntry(KFirstFreeEntryId);		
	// 1.1. With aForceUpdate True
	TInt32 mtm1 = 35;
	TInt32 mtm2 = 45;
	sampleTMsvEntry.SetMtmData1(mtm1);
	sampleTMsvEntry.SetMtmData2(mtm2);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntryNoCommit(sampleTMsvEntry, 0x999, ETrue));
	
	TMsvEntry* entry;
	iIndexAdapter->GetEntry(KFirstFreeEntryId, entry);
	MY_ASSERT_EQUALS(entry->iMtmData1, sampleTMsvEntry.iMtmData1);
	MY_ASSERT_EQUALS(entry->iMtmData2, sampleTMsvEntry.iMtmData2);
	
	
	
	// 1.2. With aForceUpdate False
	mtm1 = 105;
	mtm2 = 205;
	sampleTMsvEntry.SetMtmData1(mtm1);
	sampleTMsvEntry.SetMtmData2(mtm2);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntryNoCommit(sampleTMsvEntry, 0x999, EFalse));
	
	TMsvEntry* entry1;
	iIndexAdapter->GetEntry(KFirstFreeEntryId, entry1);
	MY_ASSERT_EQUALS(entry1->iMtmData1, sampleTMsvEntry.iMtmData1);
	MY_ASSERT_EQUALS(entry1->iMtmData2, sampleTMsvEntry.iMtmData2);
	
	
	// 2. Entry created with NO Commit to DB.
	AddEntry(EFalse, EFalse, KFirstFreeEntryId+10);
	iIndexAdapter->LockEntry(KFirstFreeEntryId+10);	
	// 2.1. With aForceUpdate True
	mtm1 = 35;
	mtm2 = 45;
	sampleTMsvEntry.SetMtmData1(mtm1);
	sampleTMsvEntry.SetMtmData2(mtm2);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntryNoCommit(sampleTMsvEntry, 0x999, ETrue));
	
	iIndexAdapter->GetEntry(KFirstFreeEntryId +10, entry2);
	MY_ASSERT_EQUALS(entry2->iMtmData1, sampleTMsvEntry.iMtmData1);
	MY_ASSERT_EQUALS(entry2->iMtmData2, sampleTMsvEntry.iMtmData2);
	
	// 2.2. With aForceUpdate False
	mtm1 = 105;
	mtm2 = 205;
	sampleTMsvEntry.SetMtmData1(mtm1);
	sampleTMsvEntry.SetMtmData2(mtm2);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntryNoCommit(sampleTMsvEntry, 0x999, EFalse));
	
	TMsvEntry* entry3;
	iIndexAdapter->GetEntry(KFirstFreeEntryId +10, entry3);
	MY_ASSERT_EQUALS(entry3->iMtmData1, sampleTMsvEntry.iMtmData1);
	MY_ASSERT_EQUALS(entry3->iMtmData2, sampleTMsvEntry.iMtmData2);
	iIndexAdapter->CommitNonCommitedEntries();
	
	//3. change Entry 
	//a. Open Transaction
	//b. Add entry
	TMsvEntry sampleTMsvEntry1;
	sampleTMsvEntry1.iDate.UniversalTime();
	sampleTMsvEntry1.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry1.SetVisible(EFalse);
	sampleTMsvEntry1.iType = KUidMsvFolderEntry;
	sampleTMsvEntry1.SetId(KFirstFreeEntryId+100);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry1, aOwnerId, EFalse));

	//c. Check if the entry is present
	TInt res = EFalse;
	TRAP_IGNORE(res = iIndexAdapter->GetDbAdapter()->EntryExistsL(KFirstFreeEntryId+100));
	MY_ASSERT_TRUE(res);
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+100));
	
	iIndexAdapter->LockEntry(KFirstFreeEntryId+100);
	iIndexAdapter->GetEntry(KFirstFreeEntryId+100, entry2);
	
	//e. Change some property
	entry2->SetVisible(ETrue);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntryNoCommit(*entry2, 0x999, EFalse));

	delete iIndexAdapter->GetDbAdapter();
	openFilename.Zero();
	openFilename.Append(KDBOpenFileName);
	TRAP_IGNORE(iIndexAdapter->OpenclosedL(openFilename));
		
	//Changes will be undone in DB but will not be udone in cache
	TRAP_IGNORE(iIndexAdapter->GetDbAdapter()->GetEntryL(KFirstFreeEntryId +100, testentry, visibleFolderId));
	iIndexAdapter->GetEntry(KFirstFreeEntryId+100, entry2);
	MY_ASSERT_FALSE(testentry->Entry().Visible());
	MY_ASSERT_TRUE(entry2->Visible());
	MY_ASSERT_EQUALS(entry2->Id(), KFirstFreeEntryId+100);

	RArray<CMsvIndexAdapter::TNonCommittedChangedEntries>& entryList = iIndexAdapter->GetNonComittedChangedEntry();
	MY_ASSERT_EQUALS(1, entryList.Count());
	CMsvEntryFreePool::Instance()->ReleaseEntryL(entryList[0].iBkpEntry);
	entryList.Reset();
	
	CMsvEntryFreePool::Instance()->ReleaseEntryL(testentry);
	
	//4. change Entry 
	//a. Open Transaction
	//b. Add entry
	TMsvEntry sampleTMsvEntry2;
	sampleTMsvEntry2.iDate.UniversalTime();
	sampleTMsvEntry2.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry2.SetVisible(EFalse);
	sampleTMsvEntry2.iType = KUidMsvFolderEntry;
	sampleTMsvEntry2.SetId(KFirstFreeEntryId+200);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry2, aOwnerId, EFalse));
	
	//Create a visible folder entry and add some childrens to it to 
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+300, KMsvRootIndexEntryId);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+310, KFirstFreeEntryId+300);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+320, KFirstFreeEntryId+300);

	//c. Check if the entry is present
	TDblQueIter<CMsvCacheVisibleFolder> fq3(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq3.SetToFirst();
	while((TestFolder = fq3++) != NULL)
		{
		if(KFirstFreeEntryId+300 == TestFolder->GetFolderId())
			{
			break;
			}
		}

	MY_ASSERT_TRUE(TestFolder->EntryExists(KFirstFreeEntryId+310));
	MY_ASSERT_TRUE(TestFolder->EntryExists(KFirstFreeEntryId+320));
	MY_ASSERT_FALSE(TestFolder->EntryExists(sampleTMsvEntry2.Id()));
	res = EFalse;
	TRAP_IGNORE(res = iIndexAdapter->GetDbAdapter()->EntryExistsL(KFirstFreeEntryId+200));
	MY_ASSERT_TRUE(res);
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+200));
	
	iIndexAdapter->LockEntry(KFirstFreeEntryId+200);
	sampleTMsvEntry2.SetParent(KFirstFreeEntryId+300);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntryNoCommit(sampleTMsvEntry2, 0x999, EFalse));
	
	fq3.SetToFirst();
	while((TestFolder = fq3++) != NULL)
		{
		if(KFirstFreeEntryId+300 == TestFolder->GetFolderId())
			{
			break;
			}
		}

	MY_ASSERT_TRUE(TestFolder->EntryExists(KFirstFreeEntryId+310));
	MY_ASSERT_TRUE(TestFolder->EntryExists(KFirstFreeEntryId+320));
	MY_ASSERT_TRUE(TestFolder->EntryExists(sampleTMsvEntry2.Id()));
	res = EFalse;
	TRAP_IGNORE(res=iIndexAdapter->GetDbAdapter()->EntryExistsL(KFirstFreeEntryId+200));
	MY_ASSERT_TRUE(res);
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+200));
	
	//f. close the database
	delete iIndexAdapter->GetDbAdapter();
	openFilename.Zero();
	openFilename.Append(KDBOpenFileName);
	TRAP_IGNORE(iIndexAdapter->OpenclosedL(openFilename));
	
	MY_ASSERT_EQUALS(1, entryList.Count());
	CMsvEntryFreePool::Instance()->ReleaseEntryL(entryList[0].iBkpEntry);
	entryList.Reset();
	
	iIndexAdapter->GetEntry(KFirstFreeEntryId+200, entry2);
	
	fq3.SetToFirst();
	while((TestFolder = fq3++) != NULL)
		{
		if(KMsvRootIndexEntryId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	
	MY_ASSERT_TRUE(TestFolder->EntryExists(sampleTMsvEntry2.Id()));
	MY_ASSERT_EQUALS(entry2->Visible(), (TInt)EFalse);
	MY_ASSERT_EQUALS(entry2->Id(), KFirstFreeEntryId+200);

	//5. change Entry 
	//a. Open Transaction
	//b. Add entry
	TMsvEntry sampleTMsvEntry4;
	sampleTMsvEntry4.iDate.UniversalTime();
	sampleTMsvEntry4.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry4.SetVisible(EFalse);
	sampleTMsvEntry4.iType = KUidMsvFolderEntry;
	sampleTMsvEntry4.SetId(KFirstFreeEntryId+700);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry4, aOwnerId, EFalse));

	//c. Check if the entry is present
	res = EFalse;
	TRAP_IGNORE(res = iIndexAdapter->GetDbAdapter()->EntryExistsL(KFirstFreeEntryId+700));
	MY_ASSERT_TRUE(res);
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+700));
	
	iIndexAdapter->LockEntry(KFirstFreeEntryId+700);
	//iIndexAdapter->GetEntry(KFirstFreeEntryId+700, entry2);
	
	//e. Change some property
	//entry2->SetVisible(ETrue);
	sampleTMsvEntry4.SetVisible(ETrue);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntryNoCommit(sampleTMsvEntry4, 0x999, EFalse));
	
	//f. close the database
	TInt err= KErrNone;
	TRAP(err,iIndexAdapter->CommitNonCommitedEntries());
	MY_ASSERT_EQUALS(err, KErrNone);

	delete iIndexAdapter->GetDbAdapter();
	openFilename.Zero();
	openFilename.Append(KDBOpenFileName);
	TRAP_IGNORE(iIndexAdapter->OpenclosedL(openFilename);)	

	//Changes will be done in DB and cache
	testentry = NULL;
	TRAP_IGNORE(iIndexAdapter->GetDbAdapter()->GetEntryL(KFirstFreeEntryId +700, testentry, visibleFolderId));
	iIndexAdapter->GetEntry(KFirstFreeEntryId+700, entry2);
	MY_ASSERT_TRUE(testentry->Entry().Visible());
	MY_ASSERT_TRUE(entry2->Visible());
	MY_ASSERT_EQUALS(entry2->Id(), KFirstFreeEntryId+700);

	CMsvEntryFreePool::Instance()->ReleaseEntryL(testentry);
	
	/* TEST Code added after changes in DOChangeEntry for rollback */
	// 6. Modifying 10 entries using no-commit.
	// 6.1. Adding 10 entry.
	for(TInt index=1100; index<1110; index++)
		{
		AddEntry(ETrue, EFalse, KFirstFreeEntryId+index);
		}
	// 6.2. Modifying 10 entries using transaction.
	for(TInt index=1100; index<1110; index++)
		{
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(KFirstFreeEntryId+index));
		TMsvEntry tEntry;
		tEntry.iDate.UniversalTime();
		tEntry.SetId(KFirstFreeEntryId+index);
		tEntry.SetParent(KMsvRootIndexEntryIdValue);
		tEntry.iType = KUidMsvMessageEntry;
		tEntry.SetMtmData1(index);
		MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
		}
	// 6.3. Verifying the state of entries before commit.
	MY_ASSERT_EQUALS(10, entryList.Count());
	CMsvDBAdapter* dbPtr = iIndexAdapter->GetDbAdapter();
	CMsvCacheEntry *cEntry = NULL;
	TMsvId visibleParentId;
	for(TInt index=1100; index<1110; index++)
		{
		TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS((long)index, cEntry->Entry().MtmData1());
		MY_ASSERT_EQUALS(KUidMsvMessageEntry, cEntry->Entry().iType);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		
		iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+index, cEntry);
		MY_ASSERT_EQUALS((long)index, cEntry->Entry().MtmData1());
		MY_ASSERT_EQUALS(KUidMsvMessageEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(entryList[index-1100].iEntry, cEntry);
		}
	iIndexAdapter->CommitNonCommitedEntries();
	// 6.4. Verify the state of entries after commit.
	MY_ASSERT_EQUALS(NULL, entryList.Count());
	for(TInt index=1100; index<1110; index++)
		{
		TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS((long)index, cEntry->Entry().MtmData1());
		MY_ASSERT_EQUALS(KUidMsvMessageEntry, cEntry->Entry().iType);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		
		iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+index, cEntry);
		MY_ASSERT_EQUALS((long)index, cEntry->Entry().MtmData1());
		MY_ASSERT_EQUALS(KUidMsvMessageEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->ReleaseEntry(KFirstFreeEntryId+index));
		}
	
	// 7. Same as test case 6 but with rollback.
	// 7.1. Adding 10 entry.
	CMsvEntryFreePool *pool = CMsvEntryFreePool::Instance();
	for(TInt index=1200; index<1210; index++)
		{
		sampleTMsvEntry.SetMtmData2(index);
		AddEntry(ETrue, EFalse, KFirstFreeEntryId+index);
		}
	// 7.2. Modifying 10 entries using transaction.
	for(TInt index=1200; index<1210; index++)
		{
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(KFirstFreeEntryId+index));
		TMsvEntry tEntry;
		tEntry.iDate.UniversalTime();
		tEntry.SetId(KFirstFreeEntryId+index);
		tEntry.SetParent(KMsvRootIndexEntryIdValue);
		tEntry.iType = KUidMsvAttachmentEntry;
		tEntry.SetMtmData2(index+200);
		MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
		}
	// 7.3. Verifying the state of entries before rollback.
	MY_ASSERT_EQUALS(10, entryList.Count());
	dbPtr = iIndexAdapter->GetDbAdapter();
	for(TInt index=1200; index<1210; index++)
		{
		TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS((long)(index+200), cEntry->Entry().MtmData2());
		MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		
		iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+index, cEntry);
		MY_ASSERT_EQUALS((long)(index+200), cEntry->Entry().MtmData2());
		MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(entryList[index-1200].iEntry, cEntry);
		}
	// 7.4. Try changing a non-existent entry, which should result in rollback.
	TMsvEntry tEntry;
	tEntry.SetId(KFirstFreeEntryId+1500);
	tEntry.SetParent(KMsvRootIndexEntryIdValue);
	tEntry.iType = KUidMsvAttachmentEntry;
	MY_ASSERT_EQUALS(KErrNotFound, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
	// 7.5. Verify the state of entry after rollback.
	MY_ASSERT_EQUALS(NULL, entryList.Count());
	for(TInt index=1200; index<1210; index++)
		{
		TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS((long)index, cEntry->Entry().MtmData2());
		MY_ASSERT_EQUALS(KUidMsvFolderEntry, cEntry->Entry().iType);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		
		iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+index, cEntry);
		MY_ASSERT_EQUALS((long)index, cEntry->Entry().MtmData2());
		MY_ASSERT_EQUALS(KUidMsvFolderEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->ReleaseEntry(KFirstFreeEntryId+index));
		}

	// 8. Modifying parent Id of the entries, such that their
	// visibleFolderId is also changed.
	// 8.1. Add a new visibleFolder under root.
	sampleTMsvEntry.SetVisible(ETrue);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+1500);
	// 8.2. Modifying 10 entries using transaction.
	for(TInt index=1200; index<1210; index++)
		{
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(KFirstFreeEntryId+index));
		TMsvEntry tEntry;
		tEntry.iDate.UniversalTime();
		tEntry.SetId(KFirstFreeEntryId+index);
		tEntry.SetParent(KFirstFreeEntryId+1500);
		tEntry.iType = KUidMsvAttachmentEntry;
		MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
		}
	// 8.3. Verifying the state of entries before commit.
	MY_ASSERT_EQUALS(10, entryList.Count());
	dbPtr = iIndexAdapter->GetDbAdapter();
	TestFolder = NULL;
	TDblQueIter<CMsvCacheVisibleFolder> fq(iIndexAdapter->GetVisibleFolderQueue());
	fq.SetToFirst();
	while((TestFolder = fq++) != NULL)
		{
		if(KMsvRootIndexEntryIdValue == TestFolder->GetFolderId())
			{
			break;
			}
		}
	MY_ASSERT_TRUE(TestFolder!=NULL);
    CMsvCacheVisibleFolder *NewTestFolder = NULL;
    fq.SetToFirst();
    while((NewTestFolder = fq++) != NULL)
        {
        if(KFirstFreeEntryId+1500 == NewTestFolder->GetFolderId())
            {
            break;
            }
        }
    MY_ASSERT_TRUE(NewTestFolder!=NULL);

	for(TInt index=1200; index<1210; index++)
		{
		// Check entry in DB.
		TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS((long)(KFirstFreeEntryId+1500), cEntry->Entry().Parent());
		MY_ASSERT_EQUALS((long)(KFirstFreeEntryId+1500), visibleParentId);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		// Check entry in cache.
		MY_ASSERT_TRUE(TestFolder->GetEntry(KFirstFreeEntryId+index, cEntry));
		MY_ASSERT_FALSE(KUidMsvAttachmentEntry==cEntry->Entry().iType);
		// If the entry's visible folder is changed, a new entry is created
		// under new visible folder and modified. The original entry under
		// old visible folder is unchanged.
		MY_ASSERT_EQUALS((long)(KMsvRootIndexEntryIdValue), cEntry->Entry().Parent());
		// Check entry under new visible folder
		MY_ASSERT_TRUE(NewTestFolder->GetEntry(KFirstFreeEntryId+index, cEntry));
        MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
        MY_ASSERT_EQUALS((long)(KFirstFreeEntryId+1500), cEntry->Entry().Parent());
		// Check non committed entry list.
		MY_ASSERT_EQUALS(entryList[index-1200].iEntry, cEntry);
		MY_ASSERT_EQUALS(entryList[index-1200].iNewVisibleFolderNode->GetFolderId(), KFirstFreeEntryId+1500);
		MY_ASSERT_EQUALS(entryList[index-1200].iOldVisibleFolderNode, TestFolder);
		}
	iIndexAdapter->CommitNonCommitedEntries();
	// 8.4. Verify the state of entry after commit.
	MY_ASSERT_EQUALS(NULL, entryList.Count());
	for(TInt index=1200; index<1210; index++)
		{
		// Check entry in DB.
		TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(KFirstFreeEntryId+1500, cEntry->Entry().Parent());
		MY_ASSERT_EQUALS(KFirstFreeEntryId+1500, visibleParentId);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		// Check entry in cache.
		MY_ASSERT_FALSE(TestFolder->GetEntry(KFirstFreeEntryId+index, cEntry));
		MY_ASSERT_TRUE(NewTestFolder->GetEntry(KFirstFreeEntryId+index, cEntry));
		MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(KFirstFreeEntryId+1500, cEntry->Entry().Parent());
		cEntry->ReleaseEntry();
		}
	
	// 9. Same as 8 but with rollback.
	// Entries now exists under KFirstFreeEntryId+1500.
	// They will be modified such that they are under root.
	
	// 9.1. Modifying 10 entries using transaction.
	for(TInt index=1200; index<1210; index++)
		{
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(KFirstFreeEntryId+index));
		TMsvEntry tEntry;
		tEntry.iDate.UniversalTime();
		tEntry.SetId(KFirstFreeEntryId+index);
		// Move entry under inbox, moving under root is not supported by the code.
		tEntry.SetParent(KMsvGlobalInBoxIndexEntryId);
		tEntry.iType = KUidMsvFolderEntry;
		MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
		}
	// 9.2. Verifying the state of entries before rollback.
	MY_ASSERT_EQUALS(10, entryList.Count());
	TestFolder = NULL;
	fq.SetToFirst();
	while((TestFolder = fq++) != NULL)
		{
		if(KFirstFreeEntryId+1500 == TestFolder->GetFolderId())
			{
			break;
			}
		}
	MY_ASSERT_TRUE(TestFolder!=NULL);
    fq.SetToFirst();
    while((NewTestFolder = fq++) != NULL)
        {
        if(KMsvGlobalInBoxIndexEntryId == NewTestFolder->GetFolderId())
            {
            break;
            }
        }
    MY_ASSERT_TRUE(NewTestFolder!=NULL);
	for(TInt index=1200; index<1210; index++)
		{
		// Check entry in DB.
		TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS(KUidMsvFolderEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS((long)(KMsvGlobalInBoxIndexEntryId), cEntry->Entry().Parent());
		MY_ASSERT_EQUALS((long)KMsvGlobalInBoxIndexEntryId, visibleParentId);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		// Check entry in cache.
		MY_ASSERT_TRUE(TestFolder->GetEntry(KFirstFreeEntryId+index, cEntry));
		MY_ASSERT_FALSE(KUidMsvFolderEntry==cEntry->Entry().iType);
        // If the entry's visible folder is changed, a new entry is created
        // under new visible folder and modified. The original entry under
        // old visible folder is unchanged.
		MY_ASSERT_EQUALS((long)(KFirstFreeEntryId+1500), cEntry->Entry().Parent());
        // Check entry under new visible folder
        MY_ASSERT_TRUE(NewTestFolder->GetEntry(KFirstFreeEntryId+index, cEntry));
        MY_ASSERT_EQUALS(KUidMsvFolderEntry, cEntry->Entry().iType);
        MY_ASSERT_EQUALS((long)(KMsvGlobalInBoxIndexEntryId), cEntry->Entry().Parent());
        
		// Check non committed entry list.
		MY_ASSERT_EQUALS(entryList[index-1200].iEntry, cEntry);
        MY_ASSERT_EQUALS(entryList[index-1200].iNewVisibleFolderNode->GetFolderId(), (long)KMsvGlobalInBoxIndexEntryId);
		MY_ASSERT_EQUALS(entryList[index-1200].iOldVisibleFolderNode, TestFolder);
		}
	// 9.3. Try changing a non-existent entry, which should result in rollback.
	tEntry.SetId(KFirstFreeEntryId+2000);
	tEntry.SetParent(KMsvRootIndexEntryIdValue);
	tEntry.iType = KUidMsvAttachmentEntry;
	MY_ASSERT_EQUALS(KErrNotFound, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
	// 9.4. Verify the state of entry after rollback.
	MY_ASSERT_EQUALS(NULL, entryList.Count());
	for(TInt index=1200; index<1210; index++)
		{
		// Check entry in DB.
		TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(KFirstFreeEntryId+1500, cEntry->Entry().Parent());
		MY_ASSERT_EQUALS(KFirstFreeEntryId+1500, visibleParentId);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		// Check entry in cache.
		MY_ASSERT_TRUE(TestFolder->GetEntry(KFirstFreeEntryId+index, cEntry));
		MY_ASSERT_EQUALS(KUidMsvAttachmentEntry, cEntry->Entry().iType);
		MY_ASSERT_EQUALS(KFirstFreeEntryId+1500, cEntry->Entry().Parent());
		cEntry->ReleaseEntry();
		}
	
	// 10. Change parent of the entry. 
	// First five entries are children of KFirstFreeEntryId+2000 (visibleFolder).
	// Next five entries are children of KFirstFreeEntryId+2100 (non-visibleFolder).
	// Both KFirstFreeEntryId+2000 and KFirstFreeEntryId+2100 are children of root.
	// KFirstFreeEntryId+2200 (Non-visibleFolder) is also children of root.
	// Move first five entries from KFirstFreeEntryId+2000 to KFirstFreeEntryId+2200.
	// (parent and visibleFolderId both changed).
	// Move next five entries from KFirstFreeEntryId+2100 to KFirstFreeEntryId+2200.
	// (Only parent changed).
	
	// Children of root.
	sampleTMsvEntry.SetVisibleFolderFlag(ETrue);
	sampleTMsvEntry.SetVisible(ETrue);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+2000);	// VisibleFolder
	sampleTMsvEntry.SetVisibleFolderFlag(EFalse);
	sampleTMsvEntry.SetVisible(EFalse);		
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+2100);	// Non-VisibleFolder
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+2200);	// Non-VisibleFolder
	for(TInt index=2001; index<2011; index++)
		{
		if(index<2006)
			AddEntry(ETrue, EFalse, KFirstFreeEntryId+index, KFirstFreeEntryId+2000);
		else
			AddEntry(ETrue, EFalse, KFirstFreeEntryId+index, KFirstFreeEntryId+2100);
		}
	CMsvEntrySelection *entrySelection = new CMsvEntrySelection;
	CMsvEntryFilter *dummyFilter = NULL;
	TRAP_IGNORE(dummyFilter = CMsvEntryFilter::NewL());
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->GetChildrenId(KFirstFreeEntryId+2000, *dummyFilter, *entrySelection));
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->GetChildrenId(KFirstFreeEntryId+2100, *dummyFilter, *entrySelection));
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->GetChildrenId(KFirstFreeEntryId+2200, *dummyFilter, *entrySelection));
	delete entrySelection;
	delete dummyFilter;
	
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+2100, cEntry));
	RArray<TMsvId> *childArr = cEntry->ChildIdArray();
	MY_ASSERT_TRUE(childArr!=NULL);
	MY_ASSERT_EQUALS(5, childArr->Count());
	for(TInt index=2006; index<2011; index++)
		{
		MY_ASSERT_EQUALS((*childArr)[index-2006], (long)(KFirstFreeEntryId+index));
		}
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+2200, cEntry));
	childArr = cEntry->ChildIdArray();
	MY_ASSERT_TRUE(childArr!=NULL);
	MY_ASSERT_EQUALS(0, childArr->Count());
	
	for(TInt index=2001; index<2011; index++)
		{
		MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(KFirstFreeEntryId+index));
		TMsvEntry tEntry;
		tEntry.iDate.UniversalTime();
		tEntry.SetId(KFirstFreeEntryId+index);
		tEntry.SetParent(KFirstFreeEntryId+2200);
		tEntry.iType = KUidMsvFolderEntry;
		MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
		}
	// 10.2. Verifying the state of entries before rollback.
	MY_ASSERT_EQUALS(10, entryList.Count());
	TestFolder = NULL;
	NewTestFolder = NULL;
	fq.SetToFirst();
	while((TestFolder = fq++) != NULL)
		{
		if(KFirstFreeEntryId+2000 == TestFolder->GetFolderId())
			{
			break;
			}
		}
	fq.SetToFirst();
	while((NewTestFolder = fq++) != NULL)
		{
		if(KMsvRootIndexEntryIdValue == NewTestFolder->GetFolderId())
			{
			break;
			}
		}
	MY_ASSERT_TRUE(TestFolder!=NULL);
	MY_ASSERT_TRUE(NewTestFolder!=NULL);
	for(TInt index=2001; index<2011; index++)
		{
		// Check entry in DB.
		TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS((long)(KFirstFreeEntryId+2200), cEntry->Entry().Parent());
		MY_ASSERT_EQUALS((long)KMsvRootIndexEntryIdValue, visibleParentId);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		// Check entry in cache.
		if(index < 2006)
			{
			MY_ASSERT_TRUE(TestFolder->GetEntry(KFirstFreeEntryId+index, cEntry));
			MY_ASSERT_EQUALS((long)(KFirstFreeEntryId+2000), cEntry->Entry().Parent());
			
			MY_ASSERT_EQUALS(entryList[index-2001].iOldVisibleFolderNode, TestFolder);
			MY_ASSERT_EQUALS(entryList[index-2001].iNewVisibleFolderNode, NewTestFolder);
			// Check entries under new visible folder.
			MY_ASSERT_TRUE(NewTestFolder->GetEntry(KFirstFreeEntryId+index, cEntry));
            MY_ASSERT_EQUALS((long)(KFirstFreeEntryId+2200), cEntry->Entry().Parent());            
			}
		else
			{
			MY_ASSERT_TRUE(NewTestFolder->GetEntry(KFirstFreeEntryId+index, cEntry));
			MY_ASSERT_EQUALS((long)(KFirstFreeEntryId+2200), cEntry->Entry().Parent());
			MY_ASSERT_EQUALS(entryList[index-2001].iOldVisibleFolderNode, NewTestFolder);
			MY_ASSERT_EQUALS(entryList[index-2001].iNewVisibleFolderNode, (CMsvCacheVisibleFolder *) NULL);
			}
		// Check non committed entry list.
		MY_ASSERT_EQUALS(entryList[index-2001].iEntry, cEntry);
		}
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+2100, cEntry));
	childArr = cEntry->ChildIdArray();
	MY_ASSERT_TRUE(childArr!=NULL);
	MY_ASSERT_EQUALS(5, childArr->Count());
	for(TInt index=2006; index<2011; index++)
		{
		MY_ASSERT_EQUALS((*childArr)[index-2006], (long)(KFirstFreeEntryId+index));
		}
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+2200, cEntry));
	childArr = cEntry->ChildIdArray();
	MY_ASSERT_TRUE(childArr!=NULL);
	MY_ASSERT_EQUALS(0, childArr->Count());
	
	iIndexAdapter->CommitNonCommitedEntries();
	// 10.4. Verify the state of entry after commit.
	MY_ASSERT_EQUALS(NULL, entryList.Count());
	for(TInt index=2001; index<2011; index++)
		{
		// Check entry in DB.
		TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+index, cEntry, visibleParentId));
		MY_ASSERT_EQUALS(KFirstFreeEntryId+2200, cEntry->Entry().Parent());
		MY_ASSERT_EQUALS((long)KMsvRootIndexEntryIdValue, visibleParentId);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		// Check entry in cache.
		MY_ASSERT_FALSE(TestFolder->GetEntry(KFirstFreeEntryId+index, cEntry));
		MY_ASSERT_TRUE(NewTestFolder->GetEntry(KFirstFreeEntryId+index, cEntry));
		MY_ASSERT_EQUALS(KFirstFreeEntryId+2200, cEntry->Entry().Parent());
		cEntry->ReleaseEntry();
		}
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+2100, cEntry));
	childArr = cEntry->ChildIdArray();
	MY_ASSERT_TRUE(childArr!=NULL);
	MY_ASSERT_EQUALS(0, childArr->Count());
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+2200, cEntry));
	childArr = cEntry->ChildIdArray();
	MY_ASSERT_TRUE(childArr!=NULL);
	MY_ASSERT_EQUALS(10, childArr->Count());
	for(TInt index=2001; index<2011; index++)
		{
		MY_ASSERT_TRUE(KErrNotFound != childArr->Find(KFirstFreeEntryId+index));
		}
	
	//11. Scenario: Root has 2 children, KFirstFreeEntryId+5000 and KFirstFreeEntryId+5100.
	//				KFirstFreeEntryId+5000 is visible and the other is not visible.
	//				KFirstFreeEntryId+5100 has 10 children and the other has none.
	//				KFirstFreeEntryId+5100 is moved to under KFirstFreeEntryId+5000.
	//				Visible folder of children of KFirstFreeEntryId+5100 is changed.
	sampleTMsvEntry = TMsvEntry();
	sampleTMsvEntry.SetVisible(ETrue);
	sampleTMsvEntry.SetVisibleFolderFlag(ETrue);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+5000, KMsvRootIndexEntryIdValue);
	sampleTMsvEntry.SetVisible(EFalse);
	sampleTMsvEntry.SetVisibleFolderFlag(EFalse);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+5100, KMsvRootIndexEntryIdValue);
	for(TInt index = 0; index < 10; ++index)
		{
		AddEntry(ETrue, EFalse, KFirstFreeEntryId+5200+index, KFirstFreeEntryId+5100);
		}
	//11.1 Shift KFirstFreeEntryId+5100 to under KFirstFreeEntryId+5000.
	MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(KFirstFreeEntryId+5100));
	tEntry.iDate.UniversalTime();
	tEntry.SetId(KFirstFreeEntryId+5100);
	tEntry.SetParent(KFirstFreeEntryId+5000);
	tEntry.iType = KUidMsvFolderEntry;
	tEntry.SetVisible(EFalse);
	tEntry.SetVisibleFolderFlag(EFalse);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(tEntry, 0x999, ETrue));
	//11.3 Commit the changes.
	iIndexAdapter->CommitNonCommitedEntries();
	//11.4 Check if the children of KFirstFreeEntryId+5100 have visible parent
	//	   appropriately set.
	dbPtr = iIndexAdapter->GetDbAdapter();
	for(TInt index = 0; index < 10; ++index)
		{
		TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+index+5200, cEntry, visibleParentId));
		ASSERT_EQUALS(visibleParentId, KFirstFreeEntryId+5000);
		CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
		}
	TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+5100, cEntry, visibleParentId));
	MY_ASSERT_EQUALS(visibleParentId, KFirstFreeEntryId+5000);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
	
	//12. Scenario: Root has 2 children, KFirstFreeEntryId+6000 and KFirstFreeEntryId+6100.
	//				KFirstFreeEntryId+6000 is visible and the other is not visible.
	//				KFirstFreeEntryId+6100 has 10 children and the other has none.
	//				One of these grandchildren is visible and has a child.
	//				KFirstFreeEntryId+6100 is moved to under KFirstFreeEntryId+6000.
	//				Visible folder of children of KFirstFreeEntryId+6100 is changed.
	//				Visible folder of grandchild should not be changed.
	sampleTMsvEntry = TMsvEntry();
	sampleTMsvEntry.SetVisible(ETrue);
	sampleTMsvEntry.SetVisibleFolderFlag(ETrue);
	sampleTMsvEntry.iType = KUidMsvFolderEntry;
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+6000, KMsvRootIndexEntryIdValue);
	sampleTMsvEntry.SetVisible(EFalse);
	sampleTMsvEntry.SetVisibleFolderFlag(EFalse);
	sampleTMsvEntry.iType = KUidMsvMessageEntry;
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+6100, KMsvRootIndexEntryIdValue);
	sampleTMsvEntry.SetVisible(ETrue);
	sampleTMsvEntry.SetVisibleFolderFlag(ETrue);
	sampleTMsvEntry.iType = KUidMsvFolderEntry;
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+6200, KFirstFreeEntryId+6100); //child,
	sampleTMsvEntry.SetVisible(EFalse);
	sampleTMsvEntry.SetVisibleFolderFlag(EFalse);
	sampleTMsvEntry.iType = KUidMsvMessageEntry;
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+6300, KFirstFreeEntryId+6200); //grandchild of 6100
	
	//12.1 Shift KFirstFreeEntryId+6100 to under KFirstFreeEntryId+6000.
	MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(KFirstFreeEntryId+6100));
	TMsvEntry myEntry;
	myEntry.iDate.UniversalTime();
	myEntry.SetId(KFirstFreeEntryId+6100);
	myEntry.SetParent(KFirstFreeEntryId+6000);
	myEntry.iType = KUidMsvFolderEntry;
	myEntry.SetVisibleFolderFlag(EFalse);
	myEntry.SetVisible(EFalse);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(myEntry, 0x999, ETrue));
	//12.3 Commit the changes.
	iIndexAdapter->CommitNonCommitedEntries();
	//12.4 Check if the sub-tree of KFirstFreeEntryId+6100 has visible parent
	//	   appropriately set.
	dbPtr = iIndexAdapter->GetDbAdapter();
	TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+6100, cEntry, visibleParentId));
	MY_ASSERT_EQUALS(visibleParentId, KFirstFreeEntryId+6000);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
	
	TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+6200, cEntry, visibleParentId));
	MY_ASSERT_EQUALS(visibleParentId, KFirstFreeEntryId+6000);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
	
	//13. Moving a visible folder with children to under a non-visible folder which has
	//	  a visible parent.
	sampleTMsvEntry = TMsvEntry();
	sampleTMsvEntry.SetVisible(EFalse);
	sampleTMsvEntry.SetVisibleFolderFlag(EFalse);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+7000, KMsvRootIndexEntryId);
	
	sampleTMsvEntry.SetVisible(ETrue);
	sampleTMsvEntry.SetVisibleFolderFlag(ETrue);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+7100, KMsvRootIndexEntryId); //entry to be moved
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+7200, KFirstFreeEntryId+7100);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+7300, KFirstFreeEntryId+7200);
	
	//13.1 Shift the visible folder to under the non-visible folder.
	MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->LockEntry(KFirstFreeEntryId+7100));
	myEntry = TMsvEntry();
	myEntry.iDate.UniversalTime();
	myEntry.SetId(KFirstFreeEntryId+7100);
	myEntry.SetParent(KFirstFreeEntryId+7000);
	myEntry.iType = KUidMsvFolderEntry;
	myEntry.SetVisibleFolderFlag(ETrue);
	myEntry.SetVisible(ETrue);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->ChangeEntryNoCommit(myEntry, 0x999, ETrue));
	//13.3 Commit the changes.
	iIndexAdapter->CommitNonCommitedEntries();
	//13.4 Check if the sub-tree of KFirstFreeEntryId+7100 has visible parent
	//	   appropriately set.
	TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+7100, cEntry, visibleParentId));
	MY_ASSERT_EQUALS(visibleParentId, KMsvRootIndexEntryId);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
	
	TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+7200, cEntry, visibleParentId));
	MY_ASSERT_EQUALS(visibleParentId, KFirstFreeEntryId+7100);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);
	
	TRAP_IGNORE(dbPtr->GetEntryL(KFirstFreeEntryId+7300, cEntry, visibleParentId));
	MY_ASSERT_EQUALS(visibleParentId, KFirstFreeEntryId+7200);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cEntry);	
	}
	


/**
 * TestGetChildren()
 *
 *
 * ---Test cases 1-6 on a visible folder.
 * 1. Visible folder present in folderlist, and is complete.
 * 2. Visible folder present in folderlist, is complete and has few blocks dirty.
 * 3. Visible folder present in folderlist, is complete and all blocks are dirty.
 * 4. Visible folder present in folderlist and is not complete. Has few grandchildren
      and children in cache.
 * 5. Visible folder not present in folderlist but it is in cache.
 * 6. Visible folder not present in cache at all.
 * ---Test cases 7-13 on non-visible folder.
 * 7. Folder's parent does not exist.
 * 8. Folder's parent is not in cache.
 * 9. Parent is in cache, GetChildren has not been performed.
 * 10. Parent is in cache, GetChildren has been performed, but folder has no children.
 * 11. Parent is in cache, GetChildren has been performed and all children are in cache.
 * 12. Parent is in cache, GetChildren has been performed but few children not in cache.
 * 13. Parent is in cache, GetChildren has been performed and no children are in cache.
 * ---Test cases 14-19 are based on filtering criteria.
 *
 */
void CTestIndexAdapter::TestGetChildren()
	{
	_LIT(KFunction, "TestGetChildren");
	INFO_PRINTF1(KFunction);
	
	TInt err, ret;
	CMsvDBAdapter *dbAdapter = NULL;
	CMsvCacheEntry *entryF = NULL;
	CMsvCacheVisibleFolder *TestFolder = NULL;
	CMsvCacheVisibleFolder *parent = NULL;
	TMsvEntry visEntry, sampleEntry;
	TMsvId visibleFolder;
	TMsvId id = 100, tempId, sampleId, maxId, minId;
	RPointerArray<CMsvCacheIndexTableEntry>* indexTable = NULL;
	RPointerArray<CMsvCacheEntry> addEntryPointer;
	CArrayPtrSeg<const TMsvEntry>* aSelection = NULL;
	TRAP_IGNORE(aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	TMsvSelectionOrdering dummyOrder;
	TMsvSelectionOrdering order;
	RPointerArray<CMsvCacheEntry>* blockPtr;
		
	
	// ---------------------- Tests 1-6 on a visible folder
		
	// 1. Visible folder present in folderlist, and is complete.
	id = 26100;
	sampleId = id;
	tempId = (TMsvId)26035;
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	TDblQueIter<CMsvCacheVisibleFolder> fq(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq.SetToFirst();
	while((TestFolder = fq++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(tempId);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)26150, maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)26151, minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)26200, maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)26201, minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)26250, maxId);
	
	entryF = NULL;
	delete aSelection;
			
		
	// 2. Visible folder present in folderlist, is complete and has few blocks dirty.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 25500;
	sampleId = id;
	tempId = (TMsvId)25000;
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	TDblQueIter<CMsvCacheVisibleFolder> fq1(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq1.SetToFirst();
	while((TestFolder = fq1++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	TestFolder->ReleaseAllBlocks(ret);
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(tempId);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	(*indexTable)[1]->ReleaseBlock(); //release one block to make it dirty
	MY_ASSERT_TRUE((*indexTable)[1]->IsDirty());
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)25550, maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)25551, minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)25600, maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)25601, minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)25650, maxId);
	
	entryF = NULL;
	delete aSelection;
	
	
	// 3. Visible folder present in folderlist, is complete and all blocks are dirty.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 800;
	sampleId = id;
	tempId = (TMsvId)1035;
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	TDblQueIter<CMsvCacheVisibleFolder> fq2(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq2.SetToFirst();
	while((TestFolder = fq2++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(tempId);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	TestFolder->ReleaseAllBlocks(ret); //release all blocks to make them dirty
	ASSERT_EQUALS(ret, 150);
	MY_ASSERT_TRUE(TestFolder->IsAllBlocksDirty());
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)850, maxId);
	minId = (*indexTable)[1]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)851, minId);
	maxId = (*indexTable)[1]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)900, maxId);
	minId = (*indexTable)[2]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)901, minId);
	maxId = (*indexTable)[2]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)950, maxId);
	
	entryF = NULL;
	delete aSelection;
	
	
	// 4. Visible folder present in folderlist and is not complete. Has few grandchildren
    //    and children in cache.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	id = 100;
	sampleId = id;
	tempId = (TMsvId)35;
	
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	
	TDblQueIter<CMsvCacheVisibleFolder> fq3(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq3.SetToFirst();
	while((TestFolder = fq3++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	TestFolder->ReleaseAllBlocks(ret);
	
	sampleTMsvEntry.iType = KUidMsvFolderEntry;
	sampleTMsvEntry.SetVisible(EFalse);
	AddEntry(ETrue, EFalse, (TMsvId)40); //a non-visible folder, parent of grandchildren
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent((TMsvId)35);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 2*bulkCount; ++index) //not all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)35);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 0; index < bulkCount; ++index)
		{
		++id;
		}

	TestFolder->SetComplete(EFalse);
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //child entries

	ASSERT_EQUALS(err, KErrNone);
	indexTable = TestFolder->GetIndexTableEntry();
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	addEntryPointer.Reset();
	tempId = (TMsvId)40;

	for(TInt index = 0; index < 10; ++index) //create grandchild entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent((TMsvId)40);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 10; ++index) //all grandchildren in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)40);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}

	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //grandchild entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_FALSE(TestFolder->IsComplete());
	indexTable = TestFolder->GetIndexTableEntry();
	tempId = (TMsvId)35;

	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
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
	MY_ASSERT_EQUALS((TMsvId)260, maxId); //grandchild and missing child entries in last block
	
	
	// 5. Visible folder not present in folderlist but it is in cache.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 2500;
	sampleId = id;
	tempId = (TMsvId)2030; //parent
	sampleTMsvEntry.SetVisible(ETrue);
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	sampleTMsvEntry.SetVisible(ETrue);
	AddEntry(ETrue, EFalse, (TMsvId)2035, tempId); //test folder
	addEntryPointer.Reset();
	//-since visible folder is not in folderlist it is listed under its parent
	TDblQueIter<CMsvCacheVisibleFolder> fq4(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq4.SetToFirst();
	while((parent = fq4++) != NULL) //find parent
		{
		if(tempId == parent->GetFolderId())
			{
			break;
			}
		}
	tempId = (TMsvId)2030;
	for(TInt index = 0; index < bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(tempId);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().iType = KUidMsvMessageEntry;
		entryF->Entry().SetId (++id);
		TRAP_IGNORE(addEntryPointer.AppendL(entryF));
		}
	TRAP(err, parent->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	visibleFolder = (TMsvId)2035;
	for(TInt index = 0; index < 10; ++index) //10 child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent((TMsvId)2035);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, visibleFolder));
		}
	
	TRAP(err, iIndexAdapter->GetChildrenL((TMsvId)2035, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	TDblQueIter<CMsvCacheVisibleFolder> fq5(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq5.SetToFirst();
	visibleFolder = (TMsvId)2035;
	while((TestFolder = fq5++) != NULL) //find the folder
		{
		if(visibleFolder == TestFolder->GetFolderId())
			{
			break;
			}
		}
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 1);
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)2560, maxId);
	
	entryF = NULL;
	delete aSelection;
		
	
	// 6. Visible folder is not present in cache.	
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 3500;
	sampleId = id;
	tempId = (TMsvId)3030;
	dbAdapter = iIndexAdapter->GetDbAdapter();
	// Add 3030 to cache & DB.
	AddEntry(ETrue, EFalse, tempId); //parent visible folder
	
	// Add 3035 (visible child of 3030) to Db only.
	sampleEntry.SetVisible(ETrue); //the folder to be tested
	sampleEntry.SetParent((TMsvId)3030);
	sampleEntry.SetId((TMsvId)3035);
	sampleEntry.iType = KUidMsvFolderEntry;
	TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, (TMsvId)3030)); //create test folder in DB
	
	// Create children of 3035 in DB.
	addEntryPointer.Reset();
	visibleFolder = (TMsvId)3035;
	for(TInt index = 0; index < 10; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)3035);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, visibleFolder));
		}
	for (TInt index = 0; index < 10; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)3035);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	// Get Children of 3035.
	TRAP(err, iIndexAdapter->GetChildrenL((TMsvId)3035, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	TDblQueIter<CMsvCacheVisibleFolder> fq7(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq7.SetToFirst();
	while((TestFolder = fq7++) != NULL) //find test folder
		{
		if(visibleFolder == TestFolder->GetFolderId())
			{
			break;
			}
		}
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 1);
	minId = (*indexTable)[0]->GetMinMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)0, minId);
	maxId = (*indexTable)[0]->GetMaxMsvIdRange();
	MY_ASSERT_EQUALS((TMsvId)3510, maxId);
	
	entryF = NULL;
	delete aSelection;

	// ----------------- Tests 7-13 on a non-visible folder
	
	// 8. Folder's parent is not in cache.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 4500;
	sampleId = id;
	
	tempId = (TMsvId)4020;
	dbAdapter = iIndexAdapter->GetDbAdapter();
	AddEntry(ETrue, EFalse, tempId); //parent visible folder
	entryF = Entry();
	entryF->Entry().SetParent(KMsvRootIndexEntryId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(ETrue);
	entryF->Entry().SetId(tempId);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create parent entry under root
	
	tempId = (TMsvId)4030;
	AddEntry(ETrue, EFalse, tempId); //parent visible folder
	entryF = Entry();
	entryF->Entry().SetParent((TMsvId)4020);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetId(tempId);

	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL((TMsvId)20, entryF));//create parent entry under root
	TDblQueIter<CMsvCacheVisibleFolder> fq8(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq8.SetToFirst();
	tempId = (TMsvId)4020;
	while((parent = fq8++) != NULL) //find parent of parent
		{
		if(tempId == parent->GetFolderId())
			{
			break;
			}
		}

	parent->ReleaseAllBlocks(ret); //release all blocks, now parent is not in cache
	sampleTMsvEntry.SetVisible(EFalse);
	AddEntry(ETrue, EFalse, (TMsvId)4035, tempId); //non-visible test folder
	entryF = Entry();
	entryF->Entry().SetParent((TMsvId)4030);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetId((TMsvId)4035);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL((TMsvId)4020, entryF));//create test folder
	
	addEntryPointer.Reset();
	visibleFolder = (TMsvId)4035;
	for(TInt index = 0; index < 10; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.SetParent((TMsvId)4035);
		sampleEntry.iType = KUidMsvMessageEntry;
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, visibleFolder));
		}
	for (TInt index = 0; index < 10; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)4035);
		entryF->Entry().iType = KUidMsvMessageEntry;
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	
	TRAP(err, parent->AddEntryListL(addEntryPointer, EFalse)); //child entries of test folder
	ASSERT_EQUALS(err, KErrNone);	
	ASSERT_FALSE(parent->IsComplete());
	
	TRAP(err, iIndexAdapter->GetChildrenL((TMsvId)4035, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 10);
	
	entryF = NULL;
	delete aSelection;
	
	// 9. Parent is in cache, GetChildren has not been performed.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 5500;
	sampleId = id;

	tempId = (TMsvId)5030;
	dbAdapter = iIndexAdapter->GetDbAdapter();
	AddEntry(ETrue, EFalse, tempId); //parent visible folder
	entryF = Entry();
	entryF->Entry().SetParent(KMsvRootIndexEntryId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(ETrue);
	entryF->Entry().SetId(tempId);
	
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create parent entry under root
	TDblQueIter<CMsvCacheVisibleFolder> fq9(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq9.SetToFirst();
	while((parent = fq9++) != NULL) //find parent
		{
		if(tempId == parent->GetFolderId())
			{
			break;
			}
		}
	parent->ReleaseAllBlocks(ret);
	sampleTMsvEntry.SetVisible(EFalse);
	AddEntry(ETrue, EFalse, (TMsvId)5035, tempId); //test folder

	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetId((TMsvId)5035);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create test folder under parent
	addEntryPointer.Reset();
	
	tempId = (TMsvId)5035;
	for(TInt index = 0; index < bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent((TMsvId)5035);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	
	tempId = (TMsvId)5035;
	for (TInt index = 0; index < bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)35);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	
	TRAP(err, parent->AddEntryListL(addEntryPointer, EFalse)); //child entries of test folder
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_FALSE(parent->IsComplete());
	
	TRAP(err, iIndexAdapter->GetChildrenL((TMsvId)5035, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 50);
	
	entryF = NULL;
	delete aSelection;

	
	// 10. Parent is in cache, GetChildren has been performed, but folder has no children.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 6500;
	sampleId = id;
	
	tempId = (TMsvId)6030;
	dbAdapter = iIndexAdapter->GetDbAdapter();
	AddEntry(ETrue, EFalse, tempId); //parent visible folder
	entryF = Entry();
	entryF->Entry().SetParent(KMsvRootIndexEntryId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(ETrue);
	entryF->Entry().SetId(tempId);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create parent entry under root
	
	TDblQueIter<CMsvCacheVisibleFolder> fq10(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq10.SetToFirst();
	while((parent = fq10++) != NULL) //find parent
		{
		if(tempId == parent->GetFolderId())
			{
			break;
			}
		}
	parent->ReleaseAllBlocks(err);
	sampleTMsvEntry.SetVisible(EFalse);
	
	AddEntry(ETrue, EFalse, (TMsvId)6035, tempId); //test folder
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetId((TMsvId)6035);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create test folder under parent
	addEntryPointer.Reset();
	
	tempId = (TMsvId)6030;
	for(TInt index = 0; index < bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(tempId);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, parent->AddEntryListL(addEntryPointer, ETrue)); //child entries of test folder
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(parent->IsComplete());
	
	TRAP(err, iIndexAdapter->GetChildrenL((TMsvId)6035, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 0);
	
	entryF = NULL;
	delete aSelection;
		
	
	// 11. Parent is in cache, GetChildren has been performed and all children are in cache.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 7500;
	sampleId = id;
	
	tempId = (TMsvId)7030;
	dbAdapter = iIndexAdapter->GetDbAdapter();
	AddEntry(ETrue, EFalse, tempId); //parent visible folder
	entryF = Entry();
	entryF->Entry().SetParent(KMsvRootIndexEntryId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(ETrue);
	entryF->Entry().SetId(tempId);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create parent entry under root
	
	TDblQueIter<CMsvCacheVisibleFolder> fq11(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq11.SetToFirst();
	while((parent = fq11++) != NULL) //find parent
		{
		if(tempId == parent->GetFolderId())
			{
			break;
			}
		}
	parent->ReleaseAllBlocks(err);
	sampleTMsvEntry.SetVisible(EFalse);
	AddEntry(ETrue, EFalse, (TMsvId)7035, tempId); //test folder
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetId((TMsvId)7035);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create test folder under parent
	addEntryPointer.Reset();
	
	tempId = (TMsvId)7035;
	for(TInt index = 0; index < bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent((TMsvId)7035);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	
	tempId = (TMsvId)7035;
	for(TInt index = 0; index < bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)35);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	
	TRAP(err, parent->AddEntryListL(addEntryPointer, EFalse)); //child entries of test folder
	ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, iIndexAdapter->GetChildrenL((TMsvId)7035, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 50);
	
	entryF = NULL;
	delete aSelection;
		
	
	// 12. Parent is in cache, GetChildren has been performed but few children not in cache.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 8500;
	sampleId = id;
	
	tempId = (TMsvId)8030;
	dbAdapter = iIndexAdapter->GetDbAdapter();
	AddEntry(ETrue, EFalse, tempId); //parent visible folder
	entryF = Entry();
	entryF->Entry().SetParent(KMsvRootIndexEntryId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(ETrue);
	entryF->Entry().SetId(tempId);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create parent entry under root
	
	TDblQueIter<CMsvCacheVisibleFolder> fq12(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq12.SetToFirst();
	while((parent = fq12++) != NULL) //find parent
		{
		if(tempId == parent->GetFolderId())
			{
			break;
			}
		}
	parent->ReleaseAllBlocks(err);
	sampleTMsvEntry.SetVisible(EFalse);
	AddEntry(ETrue, EFalse, (TMsvId)8035, tempId); //test folder
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetId((TMsvId)8035);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create test folder under parent
	addEntryPointer.Reset();
	
	tempId = (TMsvId)8030;
	for(TInt index = 0; index < 2*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(tempId);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for(TInt index = 0; index < 2*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, parent->AddEntryListL(addEntryPointer, ETrue)); //child entries of test folder
	ASSERT_EQUALS(err, KErrNone);
	addEntryPointer.Reset();
	for(TInt index = 0; index < bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent((TMsvId)8035);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	tempId = (TMsvId)8035;
	for(TInt index = 0; index < bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent((TMsvId)35);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, parent->AddEntryListL(addEntryPointer, ETrue)); //child entries of test folder
	ASSERT_EQUALS(err, KErrNone);
	indexTable = parent->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	(*indexTable)[2]->ReleaseBlock(); //2nd and last (3rd) block have children
	
	TRAP(err, iIndexAdapter->GetChildrenL((TMsvId)8035, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, bulkCount);
	
	entryF = NULL;
	delete aSelection;
	
	// 13. Parent is in cache, GetChildren has been performed but no children not in cache.
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 9500;
	sampleId = id;
	
	tempId = (TMsvId)9030;
	dbAdapter = iIndexAdapter->GetDbAdapter();
	AddEntry(ETrue, EFalse, tempId); //parent visible folder
	entryF = Entry();
	entryF->Entry().SetParent(KMsvRootIndexEntryId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(ETrue);
	entryF->Entry().SetId(tempId);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create parent entry under root
	
	TDblQueIter<CMsvCacheVisibleFolder> fq13(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq13.SetToFirst();
	while((parent = fq13++) != NULL) //find parent
		{
		if(tempId == parent->GetFolderId())
			{
			break;
			}
		}
	parent->ReleaseAllBlocks(err);
	sampleTMsvEntry.SetVisible(EFalse);
	
	AddEntry(ETrue, EFalse, (TMsvId)9035, tempId); //test folder
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(EFalse);
	entryF->Entry().SetId((TMsvId)9035);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));//create test folder under parent
	addEntryPointer.Reset();
	
	tempId = (TMsvId)9030;
	for(TInt index = 0; index < 2*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(tempId);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for(TInt index = 0; index < 2*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, parent->AddEntryListL(addEntryPointer, ETrue)); //child entries of test folder
	ASSERT_EQUALS(err, KErrNone);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 10; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent((TMsvId)9035);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	tempId = (TMsvId)9035;
	for(TInt index = 0; index < 10; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, parent->AddEntryListL(addEntryPointer, ETrue)); //child entries of test folder
	ASSERT_EQUALS(err, KErrNone);
	indexTable = parent->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 2);
	(*indexTable)[1]->ReleaseBlock();
	
	TRAP(err, iIndexAdapter->GetChildrenL((TMsvId)9035, *aSelection, dummyOrder, KUidMsvNullEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 10);
	
	entryF = NULL;
	delete aSelection;
	
	
	// ---------------------- Tests 14-19 are with filtering and sorting criteria
	
	// 14. Order in reverse by Id, filter by mtm
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 10500;
	sampleId = id;
	tempId = (TMsvId)10035;
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	
	TDblQueIter<CMsvCacheVisibleFolder> fq14(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq14.SetToFirst();
	while((TestFolder = fq14++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.iMtm = KUidMsvMessageEntry;
		sampleEntry.SetParent((TMsvId)10035);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	
	tempId = (TMsvId)10035;
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().iMtm = KUidMsvMessageEntry;
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	order.SetSorting(EMsvSortByIdReverse); //sort by id in reverse order
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, order, KUidMsvMessageEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 150);
	
	entryF = NULL;
	delete aSelection;
	
	
	// 15. Order in reverse by Id, filter by mtm
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 11500;
	sampleId = id;
	tempId = (TMsvId)11035;
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	
	TDblQueIter<CMsvCacheVisibleFolder> fq15(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq15.SetToFirst();
	while((TestFolder = fq15++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			sampleEntry.iMtm = KUidMsvFolderEntry;
			}
		sampleEntry.SetParent((TMsvId)11035);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	
	tempId = (TMsvId)11035;
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		entryF->Entry().iType = KUidMsvMessageEntry;
		entryF->Entry().iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			entryF->Entry().iMtm = KUidMsvFolderEntry;
			}
		addEntryPointer.Append(entryF);
		} //last 25 entries are folder entries
	
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	order.SetSorting(EMsvSortByIdReverse); //sort by id in reverse order
	
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, order, KUidMsvFolderEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 150);
	
	indexTable = TestFolder->GetIndexTableEntry();
	blockPtr = (*indexTable)[2]->BlockPtr();
	for(TInt index = 25; index < blockPtr->Count(); ++index)
		{
		ASSERT_TRUE((*blockPtr)[index]->Entry().iMtm == KUidMsvFolderEntry);
		}
	for(TInt index = 0; index < 25; ++index) //First 25 entries are folder entries in the selection
		{
		const TMsvEntry* entry = aSelection->At(index);
		ASSERT_TRUE(entry->iMtm == KUidMsvFolderEntry);
		ASSERT_TRUE(entry->Id() > (aSelection->At(index + 1))->Id()); //Ids are descending order
		}
	
	entryF = NULL;
	delete aSelection;
	
	// 16. Order by date in reverse order, filter by mtm
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 12500;
	sampleId = id;
	tempId = (TMsvId)12035;
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	
	TDblQueIter<CMsvCacheVisibleFolder> fq16(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq16.SetToFirst();
	while((TestFolder = fq16++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.iMtm = KUidMsvMessageEntry;
		sampleEntry.SetParent((TMsvId)12035);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	
	tempId = (TMsvId)12035;
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		entryF->Entry().iType = KUidMsvMessageEntry;
		entryF->Entry().iMtm = KUidMsvMessageEntry;
		addEntryPointer.Append(entryF);
		} 
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	order.SetSorting(EMsvSortByDateReverse); //sort by id in reverse order
	
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, order, KUidMsvMessageEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 150);
	for(TInt index = 0; index < aSelection->Count() - 1; ++index)
		{
		const TMsvEntry* entry = aSelection->At(index);
		ASSERT_TRUE(entry->iDate >= (aSelection->At(index + 1))->iDate);
		}
	
	entryF = NULL;
	delete aSelection;
	
	
	// 17. Sort and filter only by mtm
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 13500;
	sampleId = id;
	
	tempId = (TMsvId)13035;
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	
	TDblQueIter<CMsvCacheVisibleFolder> fq17(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq17.SetToFirst();
	while((TestFolder = fq17++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			sampleEntry.iMtm = KUidMsvFolderEntry;
			}
		sampleEntry.SetParent((TMsvId)13035);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	
	tempId = (TMsvId)13035;
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		entryF->Entry().iType = KUidMsvMessageEntry;
		entryF->Entry().iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			entryF->Entry().iMtm = KUidMsvFolderEntry;
			}
		addEntryPointer.Append(entryF);
		} 
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	order.SetSorting(EMsvSortByNone); 
	
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, order, KUidMsvFolderEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 150);
	for(TInt index = 0; index < aSelection->Count() - 1; ++index)
		{
		const TMsvEntry* entry = aSelection->At(index);
		if(index < 125)
			{
			ASSERT_TRUE(entry->iMtm == KUidMsvMessageEntry);
			}
		else
			{
			ASSERT_TRUE(entry->iMtm == KUidMsvFolderEntry);
			}
		
		}
	
	entryF = NULL;
	delete aSelection;
	
	
	// 18. Sort/filter by mtm 
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);
	id = 14500;
	sampleId = id;
	
	tempId = (TMsvId)14035;
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	
	TDblQueIter<CMsvCacheVisibleFolder> fq18(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq18.SetToFirst();
	while((TestFolder = fq18++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			sampleEntry.iMtm = KUidMsvFolderEntry;
			}
		if(index >= 130)
			{
			sampleEntry.iMtm = KUidMsvServiceEntry;
			}
		sampleEntry.SetParent((TMsvId)14035);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	tempId = (TMsvId)14035;
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		entryF->Entry().iType = KUidMsvMessageEntry;
		entryF->Entry().iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			entryF->Entry().iMtm = KUidMsvFolderEntry;
			}
		if(index >= 130)
			{
			entryF->Entry().iMtm = KUidMsvServiceEntry;
			}
		addEntryPointer.Append(entryF);
		} 
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	order.SetSorting(EMsvSortByNone); //no sorting
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, order, KUidMsvServiceEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 150);
	for(TInt index = 0; index < aSelection->Count() - 1; ++index)
		{
		const TMsvEntry* entry = aSelection->At(index);
		if(index < 125)
			{
			ASSERT_TRUE(entry->iMtm == KUidMsvMessageEntry);
			}
		else if( index >=125 && index <130)
			{
			ASSERT_TRUE(entry->iMtm == KUidMsvFolderEntry);
			}
		else
			{
			ASSERT_TRUE(entry->iMtm == KUidMsvServiceEntry);
			}
		
		}
	
	entryF = NULL;
	delete aSelection;
	
	
	// 19. Revers order of Ids, sort by mtm
	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);	
	id = 15500;
	sampleId = id;

	tempId = (TMsvId)15035;
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	
	TDblQueIter<CMsvCacheVisibleFolder> fq19(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq19.SetToFirst();
	while((TestFolder = fq19++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	
	iIndexAdapter->GetVisibleFolderId(tempId, visibleFolder);
	ASSERT_EQUALS(tempId, visibleFolder);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			sampleEntry.iMtm = KUidMsvFolderEntry;
			}
		if(index >= 130)
			{
			sampleEntry.iMtm = KUidMsvServiceEntry;
			}
		sampleEntry.SetParent((TMsvId)15035);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}

	tempId = (TMsvId)15035;
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		entryF->Entry().iType = KUidMsvMessageEntry;
		entryF->Entry().iMtm = KUidMsvMessageEntry;
		if(index >= 125)
			{
			entryF->Entry().iMtm = KUidMsvFolderEntry;
			}
		if(index >= 130)
			{
			entryF->Entry().iMtm = KUidMsvServiceEntry;
			}
		addEntryPointer.Append(entryF);
		} 
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	MY_ASSERT_FALSE(TestFolder->IsAllBlocksDirty());
	indexTable = TestFolder->GetIndexTableEntry();
	order.SetSorting(EMsvSortByIdReverse); // reverse order of Ids
	TRAP(err, iIndexAdapter->GetChildrenL(tempId, *aSelection, order, KUidMsvServiceEntry));
	ASSERT_EQUALS(err, KErrNone);
	ret = aSelection->Count();
	MY_ASSERT_EQUALS(ret, 150);
	for(TInt index = 0; index < aSelection->Count() - 1; ++index)
		{
		const TMsvEntry* entry = aSelection->At(index);
		if(index < 20)
			{
			ASSERT_TRUE(entry->iMtm ==KUidMsvServiceEntry );
			}
		else if( index >=20 && index <25)
			{
			ASSERT_TRUE(entry->iMtm == KUidMsvFolderEntry);
			}
		else
			{
			ASSERT_TRUE(entry->iMtm == KUidMsvMessageEntry);
			}
		
		}
	
	entryF = NULL;
	delete aSelection;
	//resetting
	sampleTMsvEntry.SetVisible(ETrue);
	sampleTMsvEntry.iType = KUidMsvFolderEntry;
	sampleTMsvEntry.iDate.UniversalTime();
	
	/***** TEST CASE ADDED FOR FOLLOWING SPECIAL USE CASE ****/
	// When all entries fetched by GetChildren() cannot be 
	// accomodated in cache.

	TRAP(err, aSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16));
	MY_ASSERT_EQUALS(err, KErrNone);

	// Destroy the server object.
	delete iServer;
	iServer = 0;
	// Delete the database.
	TFileName dbName(KDbFileName);
	RSqlDatabase::Delete(dbName);
	
	// Use a low cache configuration file for cache configuration.
	RFs iFileHandle;
	err = iFileHandle.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);
	
	_LIT(KSTDMSGINI, "C:\\private\\1000484b\\msgcache.ini");
	_LIT(KMSGINI_1, "C:\\private\\1000484b\\msgcache1.ini");
	CFileMan* file = NULL;
	TRAP_IGNORE(file = CFileMan::NewL(iFileHandle));
	err = file->Copy(KMSGINI_1, KSTDMSGINI);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// Create the server object with the new cache configuration.
	TRAP(err, iServer = CMsvServer::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);
	iIndexAdapter = iServer->Context().IndexAdapter();
	MY_ASSERT_EQUALS(80, CMsvEntryFreePool::Instance()->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(805, CMsvEntryFreePool::Instance()->GetMaxEntries());
	MY_ASSERT_EQUALS(0, CMsvEntryFreePool::Instance()->GetUsedEntriesCount());
	
	TMsvEntry entry;
	TMsvId aVisibleFolder;
	for(TInt index=0; index<1500; index++)
		{
		entry.SetId(KFirstFreeEntryId+index);
		entry.SetParent(KMsvGlobalInBoxIndexEntryId);
		entry.iType = KUidMsvMessageEntry;
		//TRAP(err, iIndexAdapter->GetDbAdapter()->CreateEntryL(entry, aVisibleFolder));
		aVisibleFolder = KMsvGlobalInBoxIndexEntryId;
		TRAP(err, iIndexAdapter->GetDbAdapter()->CreateEntryL(entry, aVisibleFolder));
		//above change made after tweaking for CreateEntry use case
		ASSERT_EQUALS(KErrNone, err);
		}
	
	TRAP(err, iIndexAdapter->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *aSelection, dummyOrder, KUidMsvNullEntry));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(aSelection->Count(), 1500);
	for(TInt index=0; index<1500; index++)
		{
		ASSERT_EQUALS(KFirstFreeEntryId+index, aSelection->At(index)->Id());		
		}
	MY_ASSERT_EQUALS(1507, CMsvEntryFreePool::Instance()->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(805, CMsvEntryFreePool::Instance()->GetMaxEntries());
	MY_ASSERT_EQUALS(702, CMsvEntryFreePool::Instance()->GetUsedEntriesCount());
	
	CMsvEntryFreePool::Instance()->FlushExcessMemory();
	MY_ASSERT_EQUALS(805, CMsvEntryFreePool::Instance()->GetCacheNumberOfEntriesCreated());
	MY_ASSERT_EQUALS(805, CMsvEntryFreePool::Instance()->GetMaxEntries());
	MY_ASSERT_EQUALS(0, CMsvEntryFreePool::Instance()->GetUsedEntriesCount());
	
	CMsvCacheEntry* cEntry = NULL;
	MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(KFirstFreeEntryId, cEntry));
	MY_ASSERT_TRUE(NULL==cEntry);
	MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+694, cEntry));
	MY_ASSERT_TRUE(NULL==cEntry);
	MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+695, cEntry));
	MY_ASSERT_TRUE(NULL==cEntry);
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+702, cEntry));
	MY_ASSERT_FALSE(NULL==cEntry);
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(KFirstFreeEntryId+1499, cEntry));
	MY_ASSERT_FALSE(NULL==cEntry);
	
	delete aSelection;
	
	// Reverting file renaming.
	err = file->Copy(KSTDMSGINI, KMSGINI_1);
	MY_ASSERT_EQUALS(err, KErrNone);
	delete file;
	iFileHandle.Close();
	}


void CTestIndexAdapter::TestGetChildrenId()
	{
	_LIT(KFunction, "TestGetChildrenId");
	
	sampleTMsvEntry.SetVisible(ETrue);
	sampleTMsvEntry.SetVisible(ETrue);
	for(int i=0; i<10; i++)
		AddEntry(ETrue, EFalse, KFirstFreeEntryId+i);
		
	CMsvEntrySelection *entrySelection = new CMsvEntrySelection;
	CMsvEntryFilter *dummyFilter = NULL;
	TRAP_IGNORE(dummyFilter = CMsvEntryFilter::NewL());
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->GetChildrenId(KMsvRootIndexEntryIdValue, *dummyFilter, *entrySelection));
	
	TInt count = 0;
	for(TInt i= 1; i<=10; ++i)
		{
		MY_ASSERT_EQUALS(KFirstFreeEntryId + count, entrySelection->At(i));
		count++;
		}
	delete entrySelection;
	delete dummyFilter;
	}
	
	
void CTestIndexAdapter::TestAdapterCreation()
	{
	_LIT(KFunction, ": Inside TestAdapterCreation");
	INFO_PRINTF1(KFunction);
	
	TMsvEntry *entry;
	// Check if root entry is created in cache.
	// This function should be checked only in trace mode.
	MY_ASSERT_EQUALS(KErrNone, iIndexAdapter->GetEntry(KMsvRootIndexEntryId, entry));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, entry->iId);
	
	TMsvIndexProgress progress = iIndexAdapter->Progress();
	MY_ASSERT_TRUE(progress.iTotal > 0);
	MY_ASSERT_TRUE(progress.iCompleted == progress.iTotal);
	MY_ASSERT_TRUE(progress.iRemaining == 0);
	iIndexAdapter->DoCancel();
	
	}	



void CTestIndexAdapter::TestAddEntry()
	{
	_LIT(KFunction, ": Inside TestAddEntry");
	INFO_PRINTF1(KFunction);
	
	//Init
	TSecureId aOwnerId = 0x999;
	TFileName openFilename;

	// 1. Adding entry with AutoAssigned value False
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+1000);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+1001);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+1002);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+1003);

	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvRootIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvLocalServiceIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvGlobalInBoxIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvGlobalOutBoxIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvDraftEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvSentEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvDeletedEntryFolderEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+1000));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+1001));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+1002));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+1003));
	MY_ASSERT_FALSE(iIndexAdapter->EntryExists(KFirstFreeEntryId+2));	
	
	
	// 2. Test AddEntry where parent is non-visible folder
	TMsvEntry sampleTMsvEntry1;
	sampleTMsvEntry1.iDate.UniversalTime();
	sampleTMsvEntry1.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry1.SetVisible(EFalse);
	sampleTMsvEntry1.iType = KUidMsvFolderEntry;
	sampleTMsvEntry1.SetId(KFirstFreeEntryId+1050);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry1, aOwnerId, EFalse));

	AddEntry(ETrue, EFalse, KFirstFreeEntryId+2000, KFirstFreeEntryId+1050);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+2001, KFirstFreeEntryId+1050);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+2002, KFirstFreeEntryId+1050);
	AddEntry(ETrue, EFalse, KFirstFreeEntryId+2003, KFirstFreeEntryId+1050);

	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvRootIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvLocalServiceIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvGlobalInBoxIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvGlobalOutBoxIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvDraftEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvSentEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvDeletedEntryFolderEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+1050));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+2000));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+2001));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+2002));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+2003));
	MY_ASSERT_FALSE(iIndexAdapter->EntryExists(KFirstFreeEntryId+2004));
		
	// 3. Adding entry with AutoAssigned value True
	AddEntry(ETrue, ETrue);
	AddEntry(ETrue, ETrue);
	AddEntry(ETrue, ETrue);
	AddEntry(EFalse, ETrue);
	AddEntry(EFalse, ETrue);
	
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvRootIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvLocalServiceIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvGlobalInBoxIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvGlobalOutBoxIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvDraftEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvSentEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvDeletedEntryFolderEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+1));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+2));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+3));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+4));
	MY_ASSERT_FALSE(iIndexAdapter->EntryExists(KFirstFreeEntryId+5));
	iIndexAdapter->CommitNonCommitedEntries();
	
	//4. Add Entry using Transaction
/*	a. Open Transaction
 * 	b. Add entry with no commit
 * 	c. Check if the entry is present
 * 	d. close the transaction
 * 	e. close the database
 * 	f. open the database and check if the entry exists
*/
	//a. Open Transaction
	//b. Add entry with no commit
	TRAP_IGNORE(MY_ASSERT_FALSE(iIndexAdapter->GetDbAdapter()->EntryExistsL(KFirstFreeEntryId+77)));
	MY_ASSERT_FALSE(iIndexAdapter->EntryExists(KFirstFreeEntryId+77));
	
	TMsvEntry sampleTMsvEntry3;
	sampleTMsvEntry3.iDate.UniversalTime();
	sampleTMsvEntry3.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry3.SetVisible(EFalse);
	sampleTMsvEntry3.iType = KUidMsvFolderEntry;
	sampleTMsvEntry3.SetId(KFirstFreeEntryId+77);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntryNoCommit(sampleTMsvEntry3, aOwnerId, EFalse));

	//c. Check if the entry is present
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(KFirstFreeEntryId+77)));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+77));
	
	//d. close the transaction
	TRAPD(err,iIndexAdapter->CommitNonCommitedEntries());
	
	//e. close the database
	delete iIndexAdapter->GetDbAdapter();
		
	//f. open the database and check if the entry exists
	openFilename.Zero();
	openFilename.Append(KDBOpenFileName);
	TRAPD(error,iIndexAdapter->OpenclosedL(openFilename));
	MY_ASSERT_EQUALS(KErrNone, error);
	
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(KFirstFreeEntryId)));
		
//5. Add Entry using Transaction
/*	a. Open Transaction
	b. Add entry with no commit
	c. Check if the entry is present
	d. close the database
	e. open the database and check if the entry exists
	f. check if the non-committed entry is present
*/
		//a. Open Transaction
		//b. Add entry with no commit
		TMsvEntry sampleTMsvEntry9;
		sampleTMsvEntry9.iDate.UniversalTime();
		sampleTMsvEntry9.SetParent(KMsvRootIndexEntryId);
		sampleTMsvEntry9.SetVisible(EFalse);
		sampleTMsvEntry9.iType = KUidMsvFolderEntry;
		sampleTMsvEntry9.SetId(KFirstFreeEntryId+78);
		MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntryNoCommit(sampleTMsvEntry9, aOwnerId, EFalse));

		//c. Check if the entry is present
		TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(KFirstFreeEntryId+78)));
		MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+78));
		
		//d. close the database
		delete iIndexAdapter->GetDbAdapter();
			
		//e. open the database and check if the entry exists
		openFilename.Zero();
		openFilename.Append(KDBOpenFileName);
		TRAP(error,iIndexAdapter->OpenclosedL(openFilename));
		MY_ASSERT_EQUALS(KErrNone, error);
		
		//f. check if the non-committed entry is present
		TRAP_IGNORE(MY_ASSERT_FALSE(iIndexAdapter->GetDbAdapter()->EntryExistsL(KFirstFreeEntryId+78)));
	
	}



#endif 		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)




void CTestIndexAdapter::TestMoveEntry()
	{
	_LIT(KFunction, "TestMoveEntry");
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TMsvId newId = MaskTMsvId(1, KFirstFreeEntryId);
#else
	TMsvId newId = KFirstFreeEntryId;
#endif

	AddEntry(ETrue, EFalse, newId);
	iIndexAdapter->LockEntryAndStore(newId);

	AddEntry(ETrue, EFalse, newId+1);
	
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->MoveEntry(newId, newId+1));
	
	TMsvEntry* entry1;
	TInt error = iIndexAdapter->GetEntry(newId, entry1);
	
	MY_ASSERT_EQUALS(KErrNone, error);
	TMsvId parent = entry1->Parent();
	MY_ASSERT_EQUALS(parent, newId+1);
	
	// move entry to a non-existent parent.
	AddEntry(ETrue, EFalse, newId+2);
	iIndexAdapter->LockEntryAndStore(newId+2);
	TInt err = iIndexAdapter->MoveEntry(newId+2, newId+3);
	MY_ASSERT_EQUALS(err, KErrNotFound);
	}


void CTestIndexAdapter::TestEntryExists()
	{
	_LIT(KFunction, "TestEntryExists");
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TMsvId id = MaskTMsvId(1, KFirstFreeEntryId);
#else
	TMsvId id = KFirstFreeEntryId;
#endif
	
	AddEntry(ETrue, EFalse, id);
	iIndexAdapter->LockEntryAndStore(id);
	
	AddEntry(ETrue, EFalse, id+2);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->MoveEntry(id, id+2));

	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(id));
	}


void CTestIndexAdapter::TestIsDescendent()
	{
	_LIT(KFunction, "TestIsDescendent");
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TMsvId newId = MaskTMsvId(2, KFirstFreeEntryId);
	TMsvId localServiceId = KMsvLocalServiceIndexEntryId;
	AddEntry(ETrue, EFalse, newId, localServiceId);
	AddEntry(ETrue, EFalse, newId+1, localServiceId);
#else
	TMsvId newId = KFirstFreeEntryId;
	AddEntry(ETrue, EFalse, newId);
	AddEntry(ETrue, EFalse, newId+1);
#endif

	AddEntry(ETrue, EFalse, newId+2, newId);
	AddEntry(ETrue, EFalse, newId+3, newId);
	AddEntry(ETrue, EFalse, newId+4, newId+3);
	AddEntry(ETrue, EFalse, newId+5, newId+3);
	AddEntry(ETrue, EFalse, newId+6, newId+1);
	AddEntry(ETrue, EFalse, newId+7, newId+6);
	AddEntry(ETrue, EFalse, newId+8, newId+6);
	AddEntry(ETrue, EFalse, newId+9, newId+7);
	
	TBool isDescendent = ETrue;
	// Should panic: PANIC - MSGS Code = 64
	//MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->IsADescendent(newId, KMsvRootIndexEntryIdValue, isDescendent));
	//MY_ASSERT_FALSE(isDescendent);

	// Should panic: PANIC - MSGS Code = 64
	//MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->IsADescendent(KMsvRootIndexEntryIdValue, newId, isDescendent));
	//MY_ASSERT_TRUE(isDescendent);

	// Should panic: PANIC - MSGS Code = 63
	//MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->IsADescendent(newId, newId, isDescendent));
	//MY_ASSERT_FALSE(isDescendent);

	isDescendent = EFalse;
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->IsADescendent(newId, newId+3, isDescendent));
	MY_ASSERT_TRUE(isDescendent);
	isDescendent = EFalse;
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->IsADescendent(newId, newId+5, isDescendent));
	MY_ASSERT_TRUE(isDescendent);
	isDescendent = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->IsADescendent(newId, newId+7, isDescendent));
	MY_ASSERT_FALSE(isDescendent);
	isDescendent = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->IsADescendent(newId, newId+9, isDescendent));
	MY_ASSERT_FALSE(isDescendent);
	isDescendent = EFalse;
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->IsADescendent(newId+6, newId+9, isDescendent));
	MY_ASSERT_TRUE(isDescendent);
	isDescendent = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->IsADescendent(newId+8, newId+9, isDescendent));
	MY_ASSERT_FALSE(isDescendent);
	isDescendent = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->IsADescendent(newId+7, newId+8, isDescendent));
	MY_ASSERT_FALSE(isDescendent);
	isDescendent = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->IsADescendent(newId+5, newId+9, isDescendent));
	MY_ASSERT_FALSE(isDescendent);
	isDescendent = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->IsADescendent(newId+2, newId+7, isDescendent));
	MY_ASSERT_FALSE(isDescendent);
	MY_ASSERT_EQUALS(KErrNotFound, iIndexAdapter->IsADescendent(newId+2, newId+11, isDescendent));

	iIndexAdapter->LockEntryAndStore(newId);
	// Also test with move entry
	MY_ASSERT_EQUALS(KErrArgument, iIndexAdapter->MoveEntry(newId, newId+2));
	}



void CTestIndexAdapter::TestGetVisibleFolderId()
	{
	_LIT(KFunction, ": Inside TestGetVisibleFolderId");
	INFO_PRINTF1(KFunction);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TMsvId newId = MaskTMsvId(1, KFirstFreeEntryId);
#else
	TMsvId newId = KFirstFreeEntryId;
#endif

	TMsvId visibleFolderId;
	TSecureId aOwnerId = 0x999;
		
	//01. Get for Root Entry
	MY_ASSERT_TRUE(iIndexAdapter->GetVisibleFolderId(KErrNotFound, visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	
	//02. Get for visible folder entry (child of root)
	AddEntry(ETrue, EFalse, newId, KMsvRootIndexEntryId);
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId));
	MY_ASSERT_TRUE(iIndexAdapter->GetVisibleFolderId(KMsvRootIndexEntryId, visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	
	//03. Get for non-visible folder entry (child of root)
	TMsvEntry sampleTMsvEntry1;
	sampleTMsvEntry1.iDate.UniversalTime();
	sampleTMsvEntry1.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry1.SetVisible(EFalse);
	sampleTMsvEntry1.iType = KUidMsvFolderEntry;
	sampleTMsvEntry1.SetId(newId+10);
	
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry1, aOwnerId, EFalse));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+10));
	MY_ASSERT_TRUE(iIndexAdapter->GetVisibleFolderId(sampleTMsvEntry1.Parent(), visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	
	//04. Get for MessageEntry entry (child of root)
	TMsvEntry sampleTMsvEntry2;
	sampleTMsvEntry2.iDate.UniversalTime();
	sampleTMsvEntry2.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry2.iType = KUidMsvMessageEntry;
	sampleTMsvEntry2.SetId(newId+20);
	
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry2, aOwnerId, EFalse));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+20));
	MY_ASSERT_TRUE(iIndexAdapter->GetVisibleFolderId(sampleTMsvEntry1.Parent(), visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	
	
	//05. Get for MessageEntry entry child of a visible folder which is child of a root entry
	// Addvisible Folder Entry
	AddEntry(ETrue, EFalse, newId+30);
	// Addvisible Child Message Entry
	TMsvEntry sampleTMsvEntry3;
	sampleTMsvEntry3.iDate.UniversalTime();
	sampleTMsvEntry3.SetParent(newId+30);
	sampleTMsvEntry3.SetVisible(EFalse);
	sampleTMsvEntry3.iType = KUidMsvMessageEntry;
	sampleTMsvEntry3.SetId(newId+40);
	
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry3, aOwnerId, EFalse));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+30));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+40));
	MY_ASSERT_TRUE(iIndexAdapter->GetVisibleFolderId(sampleTMsvEntry3.Parent(), visibleFolderId));
	MY_ASSERT_EQUALS(newId+30, visibleFolderId);
	
	//06. Get for MessageEntry entry child of a non-visible folder which is child of a root entry
	// Create parent non-visible folder entry
	TMsvEntry sampleTMsvEntry4;
	sampleTMsvEntry4.iDate.UniversalTime();
	sampleTMsvEntry4.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry4.SetVisible(EFalse);
	sampleTMsvEntry4.SetVisible(EFalse);
	sampleTMsvEntry4.iType = KUidMsvFolderEntry;
	sampleTMsvEntry4.SetId(newId+50);
	// Create Child Message entry
	TMsvEntry sampleTMsvEntry5;
	sampleTMsvEntry5.iDate.UniversalTime();
	sampleTMsvEntry5.SetParent(newId+50);
	sampleTMsvEntry5.SetVisible(EFalse);
	sampleTMsvEntry5.iType = KUidMsvMessageEntry;
	sampleTMsvEntry5.SetId(newId+60);
	
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry4, aOwnerId, EFalse));
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry5, aOwnerId, EFalse));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+50));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+60));
	MY_ASSERT_TRUE(iIndexAdapter->GetVisibleFolderId(sampleTMsvEntry5.Parent(), visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);

	//07. Get for a Visible Folder entry which is a child of another visible folder entry
	// Add Visible folder under Root
	AddEntry(ETrue, EFalse, newId+70);
	
	//add a another visible folder under newId+70
	TMsvEntry sampleTMsvEntry6;
	sampleTMsvEntry6.iDate.UniversalTime();
	sampleTMsvEntry6.SetParent(newId+70);
	sampleTMsvEntry6.SetVisible(ETrue);
	sampleTMsvEntry6.iType = KUidMsvMessageEntry;
	sampleTMsvEntry6.SetId(newId+80);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry6, aOwnerId, EFalse));
	
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+70));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+80));
	MY_ASSERT_TRUE(iIndexAdapter->GetVisibleFolderId(sampleTMsvEntry6.Parent(), visibleFolderId));
	MY_ASSERT_EQUALS(newId+70, visibleFolderId);
	
	//08. Get for a non-Visible Folder entry which is a child of another non-visible folder entry
	// Add non-Visible folder under Root
	TMsvEntry sampleTMsvEntry7;
	sampleTMsvEntry7.iDate.UniversalTime();
	sampleTMsvEntry7.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry7.SetVisible(EFalse);
	sampleTMsvEntry7.SetVisible(EFalse);
	sampleTMsvEntry7.iType = KUidMsvMessageEntry;
	sampleTMsvEntry7.SetId(newId+90);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry7, aOwnerId, EFalse));
	
	//add a another non-visible folder under newId+90
	TMsvEntry sampleTMsvEntry8;
	sampleTMsvEntry8.iDate.UniversalTime();
	sampleTMsvEntry8.SetParent(newId+90);
	sampleTMsvEntry8.SetVisible(EFalse);
	sampleTMsvEntry8.SetVisible(EFalse);
	sampleTMsvEntry8.iType = KUidMsvMessageEntry;
	sampleTMsvEntry8.SetId(newId+100);
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry8, aOwnerId, EFalse));
	
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+90));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+100));
	MY_ASSERT_TRUE(iIndexAdapter->GetVisibleFolderId(sampleTMsvEntry8.Parent(), visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
		
	//09. Get for non-visible folder entry (child of root) + entry is not in cache
	TMsvEntry sampleTMsvEntry9;
	sampleTMsvEntry9.iDate.UniversalTime();
	sampleTMsvEntry9.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry9.SetVisible(ETrue);
	sampleTMsvEntry9.iType = KUidMsvFolderEntry;
	sampleTMsvEntry9.SetId(newId+110);
	
	TRAP_IGNORE(iIndexAdapter->GetDbAdapter()->CreateEntryL(sampleTMsvEntry9, visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(sampleTMsvEntry9.Id())));
	MY_ASSERT_TRUE(iIndexAdapter->GetVisibleFolderId(sampleTMsvEntry9.Parent(), visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	
	//10. Get for MessageEntry entry (child of root) + entry not in cache
	TMsvEntry sampleTMsvEntry10;
	sampleTMsvEntry10.iDate.UniversalTime();
	sampleTMsvEntry10.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry10.iType = KUidMsvMessageEntry;
	sampleTMsvEntry10.SetId(newId+120);
	
	TRAP_IGNORE(iIndexAdapter->GetDbAdapter()->CreateEntryL(sampleTMsvEntry10, visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(sampleTMsvEntry10.Id())));
	MY_ASSERT_TRUE(iIndexAdapter->GetVisibleFolderId(sampleTMsvEntry10.Parent(), visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	
	//11. Get for MessageEntry entry child of a visible folder which is child of a root entry + entries not in cache
	// Add visible Folder Entry
	TMsvEntry sampleTMsvEntry11;
	sampleTMsvEntry11.iDate.UniversalTime();
	sampleTMsvEntry11.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry11.SetVisible(ETrue);
	sampleTMsvEntry11.iType = KUidMsvFolderEntry;
	sampleTMsvEntry11.SetId(newId+130);
	TRAP_IGNORE(iIndexAdapter->GetDbAdapter()->CreateEntryL(sampleTMsvEntry11, visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(sampleTMsvEntry11.Id())));
	
	// Add visible Child Message Entry
	TMsvEntry sampleTMsvEntry12;
	sampleTMsvEntry12.iDate.UniversalTime();
	sampleTMsvEntry12.SetParent(newId+130);
	sampleTMsvEntry12.SetVisible(EFalse);
	sampleTMsvEntry12.SetVisible(EFalse);
	sampleTMsvEntry12.iType = KUidMsvMessageEntry;
	sampleTMsvEntry12.SetId(newId+140);
	visibleFolderId = newId+130;
	TRAP_IGNORE(iIndexAdapter->GetDbAdapter()->CreateEntryL(sampleTMsvEntry12, visibleFolderId));
	MY_ASSERT_EQUALS(newId+130, visibleFolderId);
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(sampleTMsvEntry12.Id())));
	
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+130));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+140));
	MY_ASSERT_TRUE(iIndexAdapter->GetVisibleFolderId(sampleTMsvEntry12.Parent(), visibleFolderId));
	MY_ASSERT_EQUALS(newId+130, visibleFolderId);
	
	//12. Get for MessageEntry entry child of a non-visible folder which is child of a root entry
	// + entries not in cache
	// Create parent non-visible folder entry
	TMsvEntry sampleTMsvEntry13;
	sampleTMsvEntry13.iDate.UniversalTime();
	sampleTMsvEntry13.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry13.SetVisible(EFalse);
	sampleTMsvEntry13.SetVisibleFolderFlag(EFalse);
	sampleTMsvEntry13.iType = KUidMsvFolderEntry;
	sampleTMsvEntry13.SetId(newId+150);
	visibleFolderId = KMsvRootIndexEntryId;
	TRAP_IGNORE(iIndexAdapter->GetDbAdapter()->CreateEntryL(sampleTMsvEntry13, visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(sampleTMsvEntry13.Id())));
	
	// Add visible Child Message Entry
	TMsvEntry sampleTMsvEntry14;
	sampleTMsvEntry14.iDate.UniversalTime();
	sampleTMsvEntry14.SetParent(newId+150);
	sampleTMsvEntry14.SetVisible(ETrue);
	sampleTMsvEntry14.SetVisibleFolderFlag(EFalse);
	sampleTMsvEntry14.iType = KUidMsvMessageEntry;
	sampleTMsvEntry14.SetId(newId+160);
	visibleFolderId = KMsvRootIndexEntryId;
	TRAP_IGNORE(iIndexAdapter->GetDbAdapter()->CreateEntryL(sampleTMsvEntry14, visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(sampleTMsvEntry14.Id())));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+150));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+160));
	MY_ASSERT_TRUE(iIndexAdapter->GetVisibleFolderId(sampleTMsvEntry14.Parent(), visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);

	//13. Get for a Visible Folder entry which is a child of another visible folder entry
	// + entries not in cache
	// Add Visible folder under Root
	TMsvEntry sampleTMsvEntry15;
	sampleTMsvEntry15.iDate.UniversalTime();
	sampleTMsvEntry15.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry15.SetVisible(ETrue);
	sampleTMsvEntry15.SetVisibleFolderFlag(ETrue);
	sampleTMsvEntry15.iType = KUidMsvFolderEntry;
	sampleTMsvEntry15.SetId(newId+170);
	visibleFolderId = KMsvRootIndexEntryId;
	TRAP_IGNORE(iIndexAdapter->GetDbAdapter()->CreateEntryL(sampleTMsvEntry15, visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(sampleTMsvEntry15.Id())));
	
	// Add visible Child Message Entry
	TMsvEntry sampleTMsvEntry16;
	sampleTMsvEntry16.iDate.UniversalTime();
	sampleTMsvEntry16.SetParent(newId+170);
	sampleTMsvEntry16.SetVisible(ETrue);
	sampleTMsvEntry16.SetVisibleFolderFlag(ETrue);
	sampleTMsvEntry16.iType = KUidMsvFolderEntry;
	sampleTMsvEntry16.SetId(newId+180);
	visibleFolderId = newId+170;
	TRAP_IGNORE(iIndexAdapter->GetDbAdapter()->CreateEntryL(sampleTMsvEntry16, visibleFolderId));
	MY_ASSERT_EQUALS(newId+170, visibleFolderId);
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(sampleTMsvEntry16.Id())));
	
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+170));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+180));
	MY_ASSERT_TRUE(iIndexAdapter->GetVisibleFolderId(sampleTMsvEntry16.Parent(), visibleFolderId));
	MY_ASSERT_EQUALS(newId+170, visibleFolderId);
	
	//08. Get for a non-Visible Folder entry which is a child of another non-visible folder entry
	// Add non-Visible folder under Root
	TMsvEntry sampleTMsvEntry17;
	sampleTMsvEntry17.iDate.UniversalTime();
	sampleTMsvEntry17.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry17.SetVisible(EFalse);
	sampleTMsvEntry17.SetVisibleFolderFlag(EFalse);
	sampleTMsvEntry17.iType = KUidMsvMessageEntry;
	sampleTMsvEntry17.SetId(newId+190);
	visibleFolderId = KMsvRootIndexEntryId;
	TRAP_IGNORE(iIndexAdapter->GetDbAdapter()->CreateEntryL(sampleTMsvEntry17, visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(sampleTMsvEntry17.Id())));
	
	//add a another non-visible folder under newId+190
	TMsvEntry sampleTMsvEntry18;
	sampleTMsvEntry18.iDate.UniversalTime();
	sampleTMsvEntry18.SetParent(newId+190);
	sampleTMsvEntry18.SetVisible(EFalse);
	sampleTMsvEntry18.SetVisibleFolderFlag(EFalse);
	sampleTMsvEntry18.iType = KUidMsvMessageEntry;
	sampleTMsvEntry18.SetId(newId+200);
	visibleFolderId = KMsvRootIndexEntryId;
	TRAP_IGNORE(iIndexAdapter->GetDbAdapter()->CreateEntryL(sampleTMsvEntry18, visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(sampleTMsvEntry18.Id())));
	
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+190));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+200));
	MY_ASSERT_TRUE(iIndexAdapter->GetVisibleFolderId(sampleTMsvEntry18.Parent(), visibleFolderId));
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	}

void CTestIndexAdapter::TestEntryTreeInfo()
	{
	_LIT(KFunction, ": Inside TestEntryTreeInfo");
	INFO_PRINTF1(KFunction);
	
	TSecureId aOwnerId = 0x999;
	CMsvIndexAdapter::TMsvServerEntryInfo fetchedEntryInfo;
	
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvRootIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvLocalServiceIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvGlobalInBoxIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvGlobalOutBoxIndexEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvDraftEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvSentEntryId));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KMsvDeletedEntryFolderEntryId));
	
	//2. Fetch Info for Root Entry
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->EntryTreeInfo(KMsvRootIndexEntryId, fetchedEntryInfo));
	
	MY_ASSERT_EQUALS(fetchedEntryInfo.iType, KUidMsvRootEntry);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iMtm, KUidMsvLocalServiceMtm);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iService, KMsvLocalServiceIndexEntryId);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iServiceMtm, KUidMsvLocalServiceMtm);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iTopFolder, KMsvRootIndexEntryId);
	
	//3. Fetch a Non-Existing Entry
	MY_ASSERT_FALSE(iIndexAdapter->EntryExists(KFirstFreeEntryId));
	MY_ASSERT_EQUALS(KErrNotFound,iIndexAdapter->EntryTreeInfo(KFirstFreeEntryId, fetchedEntryInfo));

	//4. Get for a service entry which is a child of Root
	TMsvEntry sampleTMsvEntry1;
	sampleTMsvEntry1.iDate.UniversalTime();
	sampleTMsvEntry1.SetParent(KMsvRootIndexEntryId);
	sampleTMsvEntry1.SetVisible(ETrue);
	sampleTMsvEntry1.iType = KUidMsvServiceEntry;
	sampleTMsvEntry1.SetId(KFirstFreeEntryId);
	sampleTMsvEntry1.iMtm = KUidMsvLocalServiceMtm;
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry1, aOwnerId, EFalse));
	
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId));
	iIndexAdapter->EntryTreeInfo(KFirstFreeEntryId, fetchedEntryInfo);
	
	MY_ASSERT_EQUALS(fetchedEntryInfo.iType, KUidMsvServiceEntry);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iMtm, KUidMsvLocalServiceMtm);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iService, sampleTMsvEntry1.iServiceId);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iServiceMtm, KUidMsvLocalServiceMtm);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iParentOwnerId ,(TSecureId)0X1000484b);
	
	//4. Get for a Visible Folder child of the service entry
	TMsvEntry sampleTMsvEntry2;
	sampleTMsvEntry2.iDate.UniversalTime();
	sampleTMsvEntry2.SetParent(KFirstFreeEntryId);
	sampleTMsvEntry2.SetVisible(EFalse);
	sampleTMsvEntry2.iType = KUidMsvServiceEntry;
	sampleTMsvEntry2.SetId(KFirstFreeEntryId+10);
	sampleTMsvEntry2.iMtm = KUidMsvLocalServiceMtm;
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry2, aOwnerId, EFalse));
	
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+10));
	iIndexAdapter->EntryTreeInfo(KFirstFreeEntryId+10, fetchedEntryInfo);
	
	MY_ASSERT_EQUALS(fetchedEntryInfo.iType, KUidMsvServiceEntry);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iMtm, KUidMsvLocalServiceMtm);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iService, sampleTMsvEntry2.iServiceId);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iServiceMtm, KUidMsvLocalServiceMtm);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iParentOwnerId ,(TSecureId)0X1000484b);
	
	//4. Get for a Message which is child of service entry
	TMsvEntry sampleTMsvEntry3;
	sampleTMsvEntry3.iDate.UniversalTime();
	sampleTMsvEntry3.SetParent(KFirstFreeEntryId);
	sampleTMsvEntry3.SetVisible(EFalse);
	sampleTMsvEntry3.iType = KUidMsvMessageEntry;
	sampleTMsvEntry3.SetId(KFirstFreeEntryId+20);
	sampleTMsvEntry3.iMtm = KUidMsvLocalServiceMtm;
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->AddEntry(sampleTMsvEntry3, aOwnerId, EFalse));
	
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(KFirstFreeEntryId+10));
	iIndexAdapter->EntryTreeInfo(KFirstFreeEntryId+20, fetchedEntryInfo);
	
	MY_ASSERT_EQUALS(fetchedEntryInfo.iType, KUidMsvMessageEntry);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iMtm, KUidMsvLocalServiceMtm);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iService, sampleTMsvEntry1.iId);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iServiceMtm, KUidMsvLocalServiceMtm);
	MY_ASSERT_EQUALS(fetchedEntryInfo.iParentOwnerId ,(TSecureId)0X1000484b);
	}


	

void CTestIndexAdapter::SetupL()
	{
	_LIT(KFunction, ": Inside SetupL");
	INFO_PRINTF1(KFunction);
	
//	__UHEAP_MARK;

	//Delete database
	RFs fs;
	TParse parse;	
	User::LeaveIfError(fs.Connect());
	CFileMan* file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
		
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	
	TPtrC drive(TDriveUnit(2).Name());
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dBFile = parse.FullName();
	RSqlDatabase::Delete(dBFile);
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->Delete(parse.FullName());
	file->RmDir(parse.DriveAndPath());
	
	drive.Set(TDriveUnit(3).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dBFile = parse.FullName();
	RSqlDatabase::Delete(dBFile);
	TInt err = RSqlDatabase::Copy(KBaseDBFileName, dBFile);
	MY_ASSERT_EQUALS(KErrNone, err);
	
	drive.Set(TDriveUnit(4).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dBFile = parse.FullName();
	RSqlDatabase::Delete(dBFile);
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->Delete(parse.FullName());
	file->RmDir(parse.DriveAndPath());
	
	drive.Set(TDriveUnit(5).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dBFile = parse.FullName();
	RSqlDatabase::Delete(dBFile);
	err = RSqlDatabase::Copy(KBaseDBFileName, dBFile);
	MY_ASSERT_EQUALS(KErrNone, err);
	
	drive.Set(TDriveUnit(6).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dBFile = parse.FullName();
	RSqlDatabase::Delete(dBFile);
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->Delete(parse.FullName());
	file->RmDir(parse.DriveAndPath());
	
	drive.Set(TDriveUnit(7).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dBFile = parse.FullName();
	RSqlDatabase::Delete(dBFile);
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->Delete(parse.FullName());
	file->RmDir(parse.DriveAndPath());
	
	drive.Set(TDriveUnit(8).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dBFile = parse.FullName();
	RSqlDatabase::Delete(dBFile);
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->Delete(parse.FullName());
	file->RmDir(parse.DriveAndPath());
#else
	TFileName dbName(KDbFileName);
	RSqlDatabase::Delete(dbName);
#endif
	
	iServer=0;
	iIndexAdapter=0;
	
	TInt err1 = iFileHandle.Connect();
	MY_ASSERT_EQUALS(err1, KErrNone);
	err1 = file->Copy(KMSGINI_2, KSTDMSGINI);
	MY_ASSERT_EQUALS(err1, KErrNone);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
	drive.Set(TDriveUnit(RFs::GetSystemDrive()).Name());
	parse.Set(KPreferredDriveListFilePath, &drive, NULL);
	TFileName fileName = parse.FullName();
	err = file->Copy(KMsgTestPriorityDriveListFile2, fileName);
	MY_ASSERT_EQUALS(err, KErrNone);

	CMsvServer::ResetRepositoryL();
#endif

	CleanupStack::PopAndDestroy(file);
	fs.Close();
	TRAP(err1, iServer = CMsvServer::NewL());
	MY_ASSERT_EQUALS(err1, KErrNone);	
	iIndexAdapter = iServer->Context().IndexAdapter();
	}



void CTestIndexAdapter::TearDownL()
	{
	_LIT(KFunction, ": Inside TearDownL");
	INFO_PRINTF1(KFunction);
	
	iIndexAdapter = 0;
	delete iServer;
	iServer = 0;
	
	iFileHandle.Close();
	
#if (!defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TFileName dbName(KDbFileName);
	RSqlDatabase::Delete(dbName);
#endif
	
//	__UHEAP_MARKEND;
	}
	
	
void CTestIndexAdapter::TestEntryLocking()
	{
	_LIT(KFunction, "TestEntryLocking");
	
	AddEntry(ETrue, EFalse, KFirstFreeEntryId);
	TBool isLocked = ETrue;
	
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryOrStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->LockEntry(KFirstFreeEntryId));
	isLocked = EFalse;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_TRUE(isLocked);
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	isLocked = EFalse;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryOrStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_TRUE(isLocked);
	
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ReleaseEntry(KFirstFreeEntryId));
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryOrStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	}
	
	

void CTestIndexAdapter::TestStoreLocking()
	{
	_LIT(KFunction, "TestStoreLocking");
	
	AddEntry(ETrue, EFalse, KFirstFreeEntryId);
	TBool isLocked = ETrue;
		
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryOrStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);

	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->LockStore(KFirstFreeEntryId));
	isLocked = EFalse;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_TRUE(isLocked);
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	isLocked = EFalse;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryOrStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_TRUE(isLocked);

	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ReleaseStore(KFirstFreeEntryId));
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryOrStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	}



void CTestIndexAdapter::TestEntryAndStoreLocking()
	{
	_LIT(KFunction, "TestEntryAndStoreLocking");
		
	AddEntry(ETrue, EFalse, KFirstFreeEntryId);
	TBool isLocked = ETrue;
	
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryOrStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);

	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->LockEntryAndStore(KFirstFreeEntryId));
	isLocked = EFalse;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_TRUE(isLocked);
	isLocked = EFalse;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_TRUE(isLocked);
	isLocked = EFalse;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryOrStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_TRUE(isLocked);

	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ReleaseEntryAndStore(KFirstFreeEntryId));
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->IsEntryLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	isLocked = ETrue;
	MY_ASSERT_EQUALS(DEFAULT_RETURN, iIndexAdapter->IsEntryOrStoreLocked(KFirstFreeEntryId, isLocked));
	MY_ASSERT_FALSE(isLocked);
	}


/**
 TestDeleteEntry()

5.1. Delete a normal entry.
5.2. Delete a non-committed entry.
5.3. Delete a message entry.
5.4. Delete a visible folder entry, which has child entries and which is loaded in cache.
5.5. Delete a non-existent entry.
*/

void CTestIndexAdapter::TestDeleteEntry()
	{
	_LIT(KFunction, "TestDeleteEntry");
	
	TMsvId visibleFolder;

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TMsvId newId = MaskTMsvId(1, KFirstFreeEntryId);
#else
	TMsvId newId = KFirstFreeEntryId;
#endif
	
	// 01. Delete an entry present in cache and DB and entry is not locked.
	AddEntry(ETrue, EFalse, newId);
	iIndexAdapter->LockEntry(newId);
	DeleteEntry(newId);
	TMsvEntry* entry;
	TInt error = iIndexAdapter->GetEntry(newId, entry);
	MY_ASSERT_EQUALS(error, KErrNotFound);

	// 02. Delete a non-existant entry.
	TRAP_IGNORE(MY_ASSERT_FALSE(iIndexAdapter->GetDbAdapter()->EntryExistsL(newId)));
	MY_ASSERT_FALSE(iIndexAdapter->EntryExists(newId));
	error = iIndexAdapter->DeleteEntry(newId);
	MY_ASSERT_EQUALS(error, KErrNotFound);
	
	// 03. Delete a non-commited entry.
	AddEntry(EFalse, EFalse, newId + 10);
	iIndexAdapter->LockEntry(newId + 10);
	DeleteEntry(newId+10);
	error = iIndexAdapter->GetEntry(newId+10, entry);
	MY_ASSERT_EQUALS(error, KErrNotFound);
	
	//Delete an entry present in the cache and DB and  entry is not locked
	AddEntry(ETrue, EFalse, newId + 20);
	error =  iIndexAdapter->DeleteEntry(newId + 20);
	MY_ASSERT_EQUALS(error, KErrAccessDenied);
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+20)));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+20));
	
	//Delete an entry present in  DB only
	
	sampleTMsvEntry.iDate.UniversalTime();
	sampleTMsvEntry.SetParent(KMsvRootIndexEntryIdValue);
	sampleTMsvEntry.iType = KUidMsvFolderEntry;
	sampleTMsvEntry.SetId(newId+30);
	
	//iIndexAdapter->GetDbAdapter()->CreateEntryL(sampleTMsvEntry, visibleFolder);
	visibleFolder = KMsvRootIndexEntryIdValue;
	TRAP_IGNORE(iIndexAdapter->GetDbAdapter()->CreateEntryL(sampleTMsvEntry, visibleFolder));
	//above change made after tweaking for CreateEntry use case
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+30)));
	CMsvCacheEntry *cEntry = NULL;
	MY_ASSERT_FALSE(iIndexAdapter->FindEntryInCache(newId+30, cEntry));
	error =  iIndexAdapter->DeleteEntry(newId + 30);
	MY_ASSERT_EQUALS(error, KErrAccessDenied);
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+30)));
	}

void CTestIndexAdapter::TestDeleteSelection()
	{
	_LIT(KFunction, "TestDeleteSelection");
	
	CMsvEntrySelection *entryList = NULL;
	TRAP_IGNORE(entryList = new(ELeave) CMsvEntrySelection());
	
	TMsvId id;

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TMsvId newId = MaskTMsvId(1, KFirstFreeEntryId);
	id = newId;
#else
	TMsvId newId = id = KFirstFreeEntryId;	
#endif

	TMsvId tempId;

	// 01. Delete Empty selection
	DeleteSelection(*entryList);
	
	// 02. Delete When all entries are present in cache and locked
	for(int i=0; i<10; ++i)
		{
		id = id + i;
		AddEntry(ETrue, EFalse, id);
		iIndexAdapter->LockEntry(id);
		TRAPD(err, entryList->AppendL(id));
		MY_ASSERT_EQUALS(err,KErrNone);
		}
	DeleteSelection(*entryList);
	delete entryList;
	CMsvCacheEntry *cEntry = NULL;
	for(int i=0; i<10; ++i)
		{
		TRAP_IGNORE(ASSERT_FALSE(iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+i)));
		ASSERT_FALSE(iIndexAdapter->FindEntryInCache(newId+i, cEntry));
		}
	
	//03. Delete when selection contains the same entry more than once
	TRAPD(err, entryList = new(ELeave) CMsvEntrySelection());
	MY_ASSERT_EQUALS(err, KErrNone);
	AddEntry(ETrue, EFalse, ++id);
	iIndexAdapter->LockEntry(id);
	tempId = id;
	
	for(int i=0; i<5; i++)
		{
		TRAPD(err, entryList->AppendL(id));
		++id;
		MY_ASSERT_EQUALS(err,KErrNone);
		}
	MY_ASSERT_EQUALS(KErrNotFound, iIndexAdapter->DeleteSelection(*entryList));
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(tempId)));
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(tempId, cEntry));
	MY_ASSERT_EQUALS(cEntry->GetId(), tempId);
	delete entryList;
	
	// 04. Delete When few entries are not locked
	TRAP(err, entryList = new(ELeave) CMsvEntrySelection());
	MY_ASSERT_EQUALS(err, KErrNone);
	AddEntry(ETrue, EFalse, ++id);
	iIndexAdapter->LockEntry(id);
	TRAP_IGNORE(entryList->AppendL(id));
	AddEntry(ETrue, EFalse, ++id);
	//iIndexAdapter->LockEntry(id); //don't lock this entry
	TRAP_IGNORE(entryList->AppendL(id));
	AddEntry(ETrue, EFalse, ++id);
	iIndexAdapter->LockEntry(id);
	TRAP_IGNORE(entryList->AppendL(id));
	
	MY_ASSERT_EQUALS(KErrAccessDenied, iIndexAdapter->DeleteSelection(*entryList));
	TRAP_IGNORE(ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(entryList->At(0))));
	ASSERT_TRUE(iIndexAdapter->FindEntryInCache(entryList->At(0), cEntry));
	MY_ASSERT_EQUALS(cEntry->GetId(), entryList->At(0));
	TRAP_IGNORE(ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(entryList->At(1))));
	ASSERT_TRUE(iIndexAdapter->FindEntryInCache(entryList->At(1), cEntry));
	MY_ASSERT_EQUALS(cEntry->GetId(), entryList->At(1));
	TRAP_IGNORE(ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(entryList->At(2))));
	ASSERT_TRUE(iIndexAdapter->FindEntryInCache(entryList->At(2), cEntry));
	MY_ASSERT_EQUALS(cEntry->GetId(), entryList->At(2));
	delete entryList;
	
	//05. Delete when few entries does not exist
	TRAP(err, entryList = new(ELeave) CMsvEntrySelection());
	MY_ASSERT_EQUALS(err, KErrNone);
	tempId = id;
	for(int i=0; i<10; i++)
		{
		++id;
		if(i < 4) //only 4 added
			{
			AddEntry(ETrue, EFalse, id);
			iIndexAdapter->LockEntry(id);
			}
		TRAPD(err, entryList->AppendL(id));
		MY_ASSERT_EQUALS(err,KErrNone);
		}
	MY_ASSERT_EQUALS(KErrNotFound, iIndexAdapter->DeleteSelection(*entryList));
	
	for(TInt i = 0 ; i < 10; ++i)
		{
		++tempId;
		if(i<4)
			{
			TRAP_IGNORE(ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(tempId)));
			ASSERT_TRUE(iIndexAdapter->FindEntryInCache(tempId, cEntry));
			ASSERT_EQUALS(cEntry->GetId(), tempId);
			}
		else
			{
			TRAP_IGNORE(ASSERT_FALSE(iIndexAdapter->GetDbAdapter()->EntryExistsL(tempId)));
			ASSERT_FALSE(iIndexAdapter->FindEntryInCache(tempId, cEntry));
			}
		}
	
	delete entryList;
	// 06. Delete Entry having childrens and grand childrens using ExpandSelection and then DeleteSelection
	TRAP(err, entryList = new(ELeave) CMsvEntrySelection());
	MY_ASSERT_EQUALS(err, KErrNone);
	sampleTMsvEntry.SetVisible(ETrue);
	AddEntry(ETrue, EFalse, newId+90);
	AddEntry(ETrue, EFalse, newId+100, newId+90);
	AddEntry(ETrue, EFalse, newId+200, newId+90);
	AddEntry(ETrue, EFalse, newId+210, newId+200);
	AddEntry(ETrue, EFalse, newId+220, newId+200);

	TRAP(err, entryList->AppendL(newId+90));
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ExpandSelectionRecursively(*entryList));
	
	MY_ASSERT_EQUALS(entryList->Count(), 5);
	TInt index = entryList->Count() - 1;
	
	for(TInt i = 0 ; i  < entryList->Count(); ++i)
		{
		TMsvId temp = entryList->At(index);
		}
	
	MY_ASSERT_EQUALS(entryList->At(index), newId+90);
	MY_ASSERT_EQUALS(entryList->At(--index), newId+200);
	MY_ASSERT_EQUALS(entryList->At(--index), newId+210);
	MY_ASSERT_EQUALS(entryList->At(--index), newId+220);
	MY_ASSERT_EQUALS(entryList->At(--index), newId+100);
	
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+90)));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+90));
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+200)));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+200));
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+210)));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+210));
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+220)));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+220));
	TRAP_IGNORE(MY_ASSERT_TRUE(iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+100)));
	MY_ASSERT_TRUE(iIndexAdapter->EntryExists(newId+100));

	iIndexAdapter->LockEntry(newId+90);
	iIndexAdapter->LockEntry(newId+100);
	iIndexAdapter->LockEntry(newId+200);
	iIndexAdapter->LockEntry(newId+210);
	iIndexAdapter->LockEntry(newId+220);

	DeleteSelection(*entryList);

	delete entryList;
	
	TRAP_IGNORE(ASSERT_FALSE(iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+90)));
	TRAP_IGNORE(ASSERT_FALSE(iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+200)));
	TRAP_IGNORE(ASSERT_FALSE(iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+210)));
	TRAP_IGNORE(ASSERT_FALSE(iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+220)));
	TRAP_IGNORE(ASSERT_FALSE(iIndexAdapter->GetDbAdapter()->EntryExistsL(newId+100)));
	
	//07. Delete selection where few entries have not been loaded to DB.
	CMsvDBAdapter *dbAdapter = NULL;
	dbAdapter = iIndexAdapter->GetDbAdapter();
	
	TRAP(err, entryList = new(ELeave) CMsvEntrySelection());
	MY_ASSERT_EQUALS(err, KErrNone);
	AddEntry(ETrue, EFalse, newId+90);
	TRAP_IGNORE(entryList->AppendL(newId+90));
	AddEntry(ETrue, EFalse, newId+100, newId+90);
	TRAP_IGNORE(entryList->AppendL(newId+100));
	AddEntry(ETrue, EFalse, newId+200, newId+90);
	TRAP_IGNORE(entryList->AppendL(newId+200));
	AddEntry(ETrue, EFalse, newId+210, newId+200);
	TRAP_IGNORE(entryList->AppendL(newId+210));
	AddEntry(ETrue, EFalse, newId+220, newId+200);
	TRAP_IGNORE(entryList->AppendL(newId+220));
	
	CMsvCacheEntry *entryF = NULL;
	entryF = Entry(); //one entry not in DB
	entryF->Entry().SetParent(newId+100);
	entryF->Entry().SetId (newId+110);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(newId+100, entryF));
	TRAP_IGNORE(entryList->AppendL(entryF->GetId()));
	entryF = Entry(); //another entry not in DB
	entryF->Entry().SetParent(newId+100);
	entryF->Entry().SetId (newId+120);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(newId+100, entryF));
	TRAP_IGNORE(entryList->AppendL(entryF->GetId()));
	
	TRAP_IGNORE(ASSERT_TRUE(dbAdapter->EntryExistsL(newId+90)));
	TRAP_IGNORE(ASSERT_TRUE(dbAdapter->EntryExistsL(newId+100)));
	TRAP_IGNORE(ASSERT_TRUE(dbAdapter->EntryExistsL(newId+200)));
	TRAP_IGNORE(ASSERT_TRUE(dbAdapter->EntryExistsL(newId+210)));
	TRAP_IGNORE(ASSERT_TRUE(dbAdapter->EntryExistsL(newId+220)));
	TRAP_IGNORE(ASSERT_FALSE(dbAdapter->EntryExistsL(newId+110)));
	TRAP_IGNORE(ASSERT_FALSE(dbAdapter->EntryExistsL(newId+120)));
	
	ASSERT_TRUE(iIndexAdapter->EntryExists(newId+90));
	ASSERT_TRUE(iIndexAdapter->EntryExists(newId+100));
	ASSERT_TRUE(iIndexAdapter->EntryExists(newId+200));
	ASSERT_TRUE(iIndexAdapter->EntryExists(newId+210));
	ASSERT_TRUE(iIndexAdapter->EntryExists(newId+220));
	ASSERT_TRUE(iIndexAdapter->EntryExists(newId+110));
	ASSERT_TRUE(iIndexAdapter->EntryExists(newId+120));
	
	iIndexAdapter->LockEntry(newId+90);
	iIndexAdapter->LockEntry(newId+100);
	iIndexAdapter->LockEntry(newId+200);
	iIndexAdapter->LockEntry(newId+210);
	iIndexAdapter->LockEntry(newId+220);
	iIndexAdapter->LockEntry(newId+110);
	iIndexAdapter->LockEntry(newId+120);
	
	MY_ASSERT_EQUALS(KErrNotFound, iIndexAdapter->DeleteSelection(*entryList));
	delete entryList;
	}


void CTestIndexAdapter::TestExpandSelectionRecursively()
	{
	_LIT(KFunction, "TestExpandSelectionRecursively");
	INFO_PRINTF1(KFunction);
	
	CMsvEntrySelection *entryList = NULL;
	TInt bulkCount = 15;
	TMsvId id;
	TInt err;
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TMsvId rootId = MaskTMsvId(1, KFirstFreeEntryId+100);
#else
	TMsvId rootId = 100;
#endif

	// 1. Root entry has no entries, other entries present in cache.
	id = rootId;
	TRAP_IGNORE(entryList = new(ELeave) CMsvEntrySelection());
	for(int i=0; i<10; ++i)
		{
		AddEntry(ETrue, EFalse, id);
		++id;
		}
	
	TRAP(err, entryList->AppendL(rootId));
	ASSERT_EQUALS(err, KErrNone);
		
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ExpandSelectionRecursively(*entryList));
	for(TInt i=0; i<entryList->Count(); ++i)
		{
		MY_ASSERT_EQUALS(entryList->At(i), rootId + i); //KFirstFreeEntryId+i);
		}
	
	entryList->Reset();
	delete entryList;
	
	
	// 2. Root has 1 child, other entries present in cache.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	rootId = MaskTMsvId(1, KFirstFreeEntryId+200);
#else
	rootId = 200;
#endif

	id = rootId;
	TRAP(err, entryList = new(ELeave) CMsvEntrySelection());
	MY_ASSERT_EQUALS(err, KErrNone);
	AddEntry(ETrue, EFalse, id); //root
	AddEntry(ETrue, EFalse, ++id, rootId); //1 child
	
	for(TInt i = 0; i < bulkCount; ++i) // other entries
		{
		AddEntry(ETrue, EFalse, ++id);
		}
	TRAP(err, entryList->AppendL(rootId));
	ASSERT_EQUALS(err, KErrNone);
	id = rootId + 1;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ExpandSelectionRecursively(*entryList));
	MY_ASSERT_EQUALS(entryList->At(1), rootId);
	MY_ASSERT_EQUALS(entryList->At(0), rootId + 1);
	
	entryList->Reset();
	delete entryList;
	
	
	// 3. Root has many children in cache.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	rootId = MaskTMsvId(1, KFirstFreeEntryId+300);
#else
	rootId = 300;
#endif

	id = rootId;
	TRAP(err, entryList = new(ELeave) CMsvEntrySelection());
	MY_ASSERT_EQUALS(err, KErrNone);
	AddEntry(ETrue, EFalse, id); //root
	for(TInt i = 0; i < bulkCount; ++i) // other entries
		{
		AddEntry(ETrue, EFalse, ++id, rootId); //children of root
		}
	TRAP(err, entryList->AppendL(rootId));
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ExpandSelectionRecursively(*entryList));
	
	MY_ASSERT_EQUALS(entryList->Count(), 16);
	
	for(TInt i = 0; i < entryList->Count() - 1; ++i)
		{
		ASSERT_EQUALS(entryList->At(i), rootId + i + 1);
		}
	ASSERT_EQUALS(entryList->At(entryList->Count() - 1), rootId);
	
	entryList->Reset();
	delete entryList;
	
	
	// 4. Root has many children. Remove one child from the selection. Expand again.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	rootId = MaskTMsvId(1, KFirstFreeEntryId+400);
#else
	rootId = 400;
#endif

	TRAP(err, entryList = new(ELeave) CMsvEntrySelection());
	MY_ASSERT_EQUALS(err, KErrNone);
	id = rootId;
	AddEntry(ETrue, EFalse, rootId); //400
	iIndexAdapter->LockEntry(rootId);
	AddEntry(ETrue, EFalse, ++id, rootId); //401
	AddEntry(ETrue, EFalse, ++id, rootId); //402
	AddEntry(ETrue, EFalse, ++id, (TMsvId)rootId + 1); //403
	AddEntry(ETrue, EFalse, ++id, (TMsvId)rootId + 2); //404
	AddEntry(ETrue, EFalse, ++id, (TMsvId)rootId + 3); //405
	AddEntry(ETrue, EFalse, ++id, (TMsvId)rootId + 3); //406
	AddEntry(ETrue, EFalse, ++id, (TMsvId)rootId + 4); //407
	AddEntry(ETrue, EFalse, ++id, (TMsvId)rootId + 4); //408
	AddEntry(ETrue, EFalse, ++id, (TMsvId)rootId + 5); //409
	AddEntry(ETrue, EFalse, ++id, (TMsvId)rootId + 5); //410
	AddEntry(ETrue, EFalse, ++id, (TMsvId)rootId + 7); //411
	AddEntry(ETrue, EFalse, ++id, (TMsvId)rootId + 7); //412
	AddEntry(ETrue, EFalse, ++id, (TMsvId)rootId + 8); //413
	AddEntry(ETrue, EFalse, ++id, (TMsvId)rootId + 8); //414
	AddEntry(ETrue, EFalse, ++id, (TMsvId)rootId + 9); //415
	AddEntry(ETrue, EFalse, ++id, (TMsvId)rootId + 9); //416
	
	TRAP(err, entryList->AppendL(rootId));
	ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ExpandSelectionRecursively(*entryList));
	
	MY_ASSERT_EQUALS(entryList->Count(), 17);
	TInt index = entryList->Count() - 1;
	ASSERT_EQUALS(entryList->At(index), rootId);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 2);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 4);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 7);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 11);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 12);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 8);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 13);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 14);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 1);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 3);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 5);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 9);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 15);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 16);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 10);
	ASSERT_EQUALS(entryList->At(--index), (TMsvId)rootId + 06);
	delete entryList;
	}
	


void CTestIndexAdapter::TestChangeEntry()
	{
	_LIT(KFunction, "TestChangeEntry");
	
	// 1. Entry created with Commit to DB.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TMsvId newId = MaskTMsvId(1, KFirstFreeEntryId);
#else
	TMsvId newId = KFirstFreeEntryId;
#endif
	
	AddEntry(ETrue, EFalse, newId);
	iIndexAdapter->LockEntry(newId);
	
	// 1.1. With aForceUpdate True
	TInt32 mtm1 = 35;
	TInt32 mtm2 = 45;
	
	sampleTMsvEntry.SetMtmData1(mtm1);
	sampleTMsvEntry.SetMtmData2(mtm2);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntry(sampleTMsvEntry, 0x999, ETrue));

	TMsvEntry* entry;
	iIndexAdapter->GetEntry(newId, entry);
	MY_ASSERT_EQUALS(entry->iMtmData1, sampleTMsvEntry.iMtmData1);
	MY_ASSERT_EQUALS(entry->iMtmData2, sampleTMsvEntry.iMtmData2);

	// 1.2. With aForceUpdate False
	mtm1 = 105;
	mtm2 = 205;
	sampleTMsvEntry.SetMtmData1(mtm1);
	sampleTMsvEntry.SetMtmData2(mtm2);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntry(sampleTMsvEntry, 0x999, EFalse));
	
	TMsvEntry* entry1;
	iIndexAdapter->GetEntry(newId, entry1);
	MY_ASSERT_EQUALS(entry1->iMtmData1, sampleTMsvEntry.iMtmData1);
	MY_ASSERT_EQUALS(entry1->iMtmData2, sampleTMsvEntry.iMtmData2);

	
	// 2. Entry created with NO Commit to DB.
	// Is this testcase valid. Check in the code.
	AddEntry(EFalse, EFalse, newId+10);
	iIndexAdapter->LockEntry(newId + 10);
	// 2.1. With aForceUpdate True
	mtm1 = 35;
	mtm2 = 45;
	sampleTMsvEntry.SetMtmData1(mtm1);
	sampleTMsvEntry.SetMtmData2(mtm2);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntry(sampleTMsvEntry, 0x999, ETrue));
	
	TMsvEntry* entry2;
	iIndexAdapter->GetEntry(newId +10, entry2);
	MY_ASSERT_EQUALS(entry2->iMtmData1, sampleTMsvEntry.iMtmData1);
	MY_ASSERT_EQUALS(entry2->iMtmData2, sampleTMsvEntry.iMtmData2);
	
	// 2.2. With aForceUpdate False
	mtm1 = 105;
	mtm2 = 205;
	sampleTMsvEntry.SetMtmData1(mtm1);
	sampleTMsvEntry.SetMtmData2(mtm2);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntry(sampleTMsvEntry, 0x999, EFalse));
	
	TMsvEntry* entry3;
	iIndexAdapter->GetEntry(newId +10, entry3);
	MY_ASSERT_EQUALS(entry3->iMtmData1, sampleTMsvEntry.iMtmData1);
	MY_ASSERT_EQUALS(entry3->iMtmData2, sampleTMsvEntry.iMtmData2);	
	}




void CTestIndexAdapter::TestChangeEntryInternal()
	{
	_LIT(KFunction, "TestChangeEntryInternal");
	INFO_PRINTF1(KFunction);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TMsvId newId = MaskTMsvId(1, KFirstFreeEntryId);
#else
	TMsvId newId = KFirstFreeEntryId;
#endif

	// 1. Entry created with Commit to DB.
	AddEntry(ETrue, EFalse, newId);
	iIndexAdapter->LockEntry(newId);
	
	// 1.1. Change the entry
	TInt32 mtm1 = 35;
	TInt32 mtm2 = 45;
	
	TMsvEntry sample = sampleTMsvEntry;
	sample.SetMtmData1(mtm1);
	sample.SetMtmData2(mtm2);
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->ChangeEntryInternal(sample, 0x999));

	TMsvEntry* entry;
	iIndexAdapter->GetEntry(newId, entry);
	MY_ASSERT_EQUALS(entry->iMtmData1, sample.iMtmData1);
	MY_ASSERT_EQUALS(entry->iMtmData2, sample.iMtmData2);
	}



void CTestIndexAdapter::TestGetEntry()
	{
	_LIT(KFunction, "TestGetEntry");

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TMsvId id = MaskTMsvId(1, KFirstFreeEntryId);
#else
	TMsvId id = KFirstFreeEntryId;
#endif

	AddEntry(ETrue, EFalse, id);
	TMsvEntry* aEntry;
	MY_ASSERT_EQUALS(DEFAULT_RETURN,iIndexAdapter->GetEntry(id, aEntry));
	MY_ASSERT_EQUALS(id, aEntry->Id());
	}




void CTestIndexAdapter::TestAddEntryToCache()
	{
	_LIT(KFunction, "TestAddEntryToCache");
	INFO_PRINTF1(KFunction);
	
	TInt err, ret;
	CMsvDBAdapter *dbAdapter = NULL;
	CMsvCacheEntry *entryF = NULL;
	CMsvCacheVisibleFolder *TestFolder = NULL;
	CMsvCacheVisibleFolder *parent = NULL;
	TMsvId id = 100, tempId, sampleId;
	RPointerArray<CMsvCacheIndexTableEntry>* indexTable = NULL;
	RPointerArray<CMsvCacheEntry> addEntryPointer;
	
	

	// 1. Add entry whose visible folder is present in cache but is not complete.
	TMsvEntry sampleEntry;
	id = KFirstFreeEntryId+100;
	sampleId = id;

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TMsvId tId = MaskTMsvId(1, KFirstFreeEntryId+35);
#else
	TMsvId tId = (TMsvId)(KFirstFreeEntryId+35);
#endif

	tempId = tId;
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	TDblQueIter<CMsvCacheVisibleFolder> fq3(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq3.SetToFirst();
	while((TestFolder = fq3++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	TestFolder->ReleaseAllBlocks(ret);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{
		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(tId);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 2*bulkCount; ++index) //not all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tId);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	for (TInt index = 0; index < bulkCount; ++index)
		{
		++id;
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, EFalse)); //child entries
	ASSERT_EQUALS(err, KErrNone);
	ASSERT_FALSE(TestFolder->IsComplete());
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 2);
	tempId = tId; //parent of the entry
	entryF = Entry(); //new entry
	entryF->Entry().SetParent(tempId);
	entryF->Entry().SetId (++id);
	MY_ASSERT_FALSE((*indexTable)[1]->EntryExists(entryF->GetId()));
	TRAP(err, iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE((*indexTable)[1]->EntryExists(entryF->GetId()));

	entryF = NULL;
	
	// 2. Add entry whose visible folder is in cache and is complete.
	TMsvEntry sampleEntry1;
	id = KFirstFreeEntryId+2000;
	sampleId = id;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	tId = MaskTMsvId(1, KFirstFreeEntryId+40);
#else
	tId = (TMsvId) 40;
#endif
	
	tempId = tId;
	AddEntry(ETrue, EFalse, tempId); //create folder entry under root
	dbAdapter = iIndexAdapter->GetDbAdapter();
	entryF = Entry();
	entryF->Entry().SetParent(tempId);
	entryF->Entry().iType = KUidMsvMessageEntry;
	entryF->Entry().SetId (id +1);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	TDblQueIter<CMsvCacheVisibleFolder> fq2(iIndexAdapter->GetVisibleFolderQueue()); //folder list
	fq2.SetToFirst();
	while((TestFolder = fq2++) != NULL)
		{
		if(tempId == TestFolder->GetFolderId())
			{
			break;
			}
		}
	TestFolder->ReleaseAllBlocks(ret);
	addEntryPointer.Reset();
	for(TInt index = 0; index < 3*bulkCount; ++index) //create child entries in DB
		{

		sampleEntry.SetId(++sampleId);
		sampleEntry.iType = KUidMsvMessageEntry;
		sampleEntry.SetParent(tempId);
		TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry, tempId));
		}
	for (TInt index = 0; index < 3*bulkCount; ++index) //all children in cache
		{
		entryF = Entry();
		entryF->Entry().SetParent(tempId);
		entryF->Entry().SetId (++id);
		addEntryPointer.Append(entryF);
		}
	TRAP(err, TestFolder->AddEntryListL(addEntryPointer, ETrue)); //child entries
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(TestFolder->IsComplete());
	indexTable = TestFolder->GetIndexTableEntry();
	MY_ASSERT_EQUALS(indexTable->Count(), 3);
	tempId = tId; //parent of the entry
	entryF = Entry(); //new entry
	entryF->Entry().SetParent(tempId);
	entryF->Entry().SetId (++id);
	MY_ASSERT_FALSE((*indexTable)[2]->EntryExists(entryF->GetId()));
	TRAP(err, iIndexAdapter->AddEntryToCacheL(tempId, entryF));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE((*indexTable)[2]->EntryExists(entryF->GetId()));
		
	entryF = NULL;
	addEntryPointer.Close();

	// 3. Add entry whose visible folder is not in cache.
	TMsvEntry sampleEntry2;	
	sampleEntry2.SetVisible(ETrue); //parent folder of the entry
	sampleEntry2.SetParent(KMsvRootIndexEntryId);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	sampleEntry2.SetId(MaskTMsvId(1, KFirstFreeEntryId+1077));	
#else
	sampleEntry2.SetId((TMsvId)1077);	
#endif
	sampleEntry2.iType = KUidMsvFolderEntry;

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry2, MaskTMsvId(1, KMsvRootIndexEntryId))); //create it in DB
#else
	TRAP_IGNORE(dbAdapter->CreateEntryL(sampleEntry2, KMsvRootIndexEntryId));
#endif
	
	// Entry 1077 is not in cache, but only in DB.
	// Now create a entry 1045, child of 1077 in cache.
	entryF = Entry();
	entryF->Entry().iType = KUidMsvFolderEntry;
	entryF->Entry().SetVisible(ETrue);	
	CMsvCacheEntry* aEntry = NULL;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	entryF->Entry().SetParent(MaskTMsvId(1, KFirstFreeEntryId+1077));	
	entryF->Entry().SetId(MaskTMsvId(1, KFirstFreeEntryId+1045));
	TRAP(err, iIndexAdapter->AddEntryToCacheL(MaskTMsvId(1, KFirstFreeEntryId+1077), entryF));//create folder entry under root
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(MaskTMsvId(1, KFirstFreeEntryId+1045), aEntry));
#else
	entryF->Entry().SetParent((TMsvId)1077);	
	entryF->Entry().SetId((TMsvId)1045);
	TRAP_IGNORE(iIndexAdapter->AddEntryToCacheL(1077, entryF));//create folder entry under root
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_TRUE(iIndexAdapter->FindEntryInCache(1045, aEntry));
#endif	

	entryF = NULL;
	}



CMsvCacheEntry* CTestIndexAdapter::Entry()
	{
	CMsvCacheEntry *entry = NULL;
	TRAP_IGNORE( entry = CMsvEntryFreePool::Instance()->EntryL());
	return entry;
	}
	


CTestSuite* CTestIndexAdapter::CreateSuiteL(const TDesC& aName)
// static
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestAddEntry);
	ADD_ASYNC_TEST_STEP(TestEntryLocking);
	ADD_ASYNC_TEST_STEP(TestStoreLocking);
	ADD_ASYNC_TEST_STEP(TestEntryAndStoreLocking);
	ADD_ASYNC_TEST_STEP(TestIsDescendent);
	ADD_ASYNC_TEST_STEP(TestAddEntryNoCommit);
	ADD_ASYNC_TEST_STEP(TestDeleteEntry);
	ADD_ASYNC_TEST_STEP(TestDeleteSelection);
	ADD_ASYNC_TEST_STEP(TestExpandSelectionRecursively);
	ADD_ASYNC_TEST_STEP(TestChangeEntry);
	ADD_ASYNC_TEST_STEP(TestChangeEntryNoCommit);
	ADD_ASYNC_TEST_STEP(TestGetEntry);
	ADD_ASYNC_TEST_STEP(TestGetChildren);
	ADD_ASYNC_TEST_STEP(TestGetChildrenId);
	ADD_ASYNC_TEST_STEP(TestAddEntryToCache);
	ADD_ASYNC_TEST_STEP(TestMoveEntry);
	ADD_ASYNC_TEST_STEP(TestEntryExists);
	ADD_ASYNC_TEST_STEP(TestGetVisibleFolderId);
	ADD_ASYNC_TEST_STEP(TestEntryTreeInfo);
	ADD_ASYNC_TEST_STEP(TestChangeEntryInternal);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	ADD_ASYNC_TEST_STEP(TestGetNextAvailableDriveId);
	ADD_ASYNC_TEST_STEP(ValidateIndexAdapterCreation);
	ADD_ASYNC_TEST_STEP(TestGetChildrenIdAll);
	ADD_ASYNC_TEST_STEP(TestChangeDrive);
	ADD_ASYNC_TEST_STEP(TestReloadCache);
	ADD_ASYNC_TEST_STEP(TestSetLocalServiceComplete);
	ADD_ASYNC_TEST_STEP(TestToImproveCoverage);
	ADD_ASYNC_TEST_STEP(TestDoRemoveDeletedEntries);
	ADD_ASYNC_TEST_STEP(TestDoForceDeleteEntry);
#endif 
	END_SUITE;
	}

