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

#include <barsc.h>
#include <barsread.h>
#include <bautils.h>
#include <msgs.rsg>
#include <msvids.h>
#include <msvuids.h>
#include "msvindexadapter.h"
#include "Testsetup.h"

#pragma warning( disable : 4710 )

RTest test(_L("MSV Index Testrig - Heap failure"));
CPermanentFileStore* DummyStore = NULL;
TInt theFailCount;


_LIT(KMsvTestFile, "c:\\Index testing file 1");
_LIT(KMsvTestDummyFile, "c:\\Index testing file 2");

_LIT(KShortDescription, "Description");
_LIT(KShortDetails, "Details");

_LIT(KLongDescription, "A very long description which goes over thirty two characters but is now needed to go over 64");
_LIT(KLongDetails, "A very long details string containing rubbish and junk but has more than sixty four characters");

//

LOCAL_C void CreateDefaultIndex(CMsvServerIndex& aIndex)
	{
	// find the resource file
	TFileName fileName = KServerIndexResourceFileName();
	TParse parse;
	TFindFile findfile(theUtils->FileSession());
	CDir* dirs=NULL;
	User::LeaveIfError(findfile.FindWildByDir(fileName, KServerIndexResourceFilePath, dirs));
	fileName=(*dirs)[0].iName;
	delete dirs;
	parse.Set(fileName, &findfile.File(), NULL);
	fileName=parse.FullName();
	BaflUtils::NearestLanguageFile(theUtils->FileSession(), fileName);
	
	// load the initial index structure from resource file
	RResourceFile rf;
	rf.OpenL(theUtils->FileSession(),fileName);
	CleanupClosePushL(rf);
	TResourceReader reader;
	HBufC8* buf=rf.AllocReadLC(R_SERVER_INDEX_STARTUP);
	reader.SetBuffer(buf);

	const TInt numberOfEntries=reader.ReadInt16();
	for (TInt ii=0; ii<numberOfEntries; ii++)
		{
		TTestMsvEntry newEntry;
		// values from resource file
		newEntry.SetId(reader.ReadInt32());
		newEntry.SetParent(reader.ReadInt32());
		newEntry.iServiceId = reader.ReadInt32();
		newEntry.iType.iUid = reader.ReadInt32();
		newEntry.iMtm.iUid = reader.ReadInt32();
		newEntry.SetData(reader.ReadInt32());
		newEntry.iDescription.Set(reader.ReadTPtrC());
		newEntry.iDetails.Set(reader.ReadTPtrC());
		// plus other ones
		newEntry.iDate.HomeTime();
		newEntry.iSize=0;
		// populate it
		User::LeaveIfError(aIndex.AddEntry(newEntry, KMsvServerId, EFalse));
		}

	CleanupStack::PopAndDestroy(2); // reader(buf), rf

	}





LOCAL_C void HeapFailChangingWithChangingStringsNotMoving(const TDesC& aOrigDescription, const TDesC& aOrigDetails, const TDesC& aNewDescription, const TDesC& aNewDetails)
//
//
//
	{
	CPermanentFileStore* testStore;
	TTestMsvEntry entry, childEntry; 
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(selection);
	TInt err=0;

	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	
	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));
	childEntry = entry;
	childEntry.SetParent(entry.Id());
	childEntry.iServiceId = 1000;
	childEntry.iDescription.Set(aOrigDescription);
	childEntry.iDetails.Set(aOrigDetails);
	REPORT_IF_ERROR(index->AddEntry(childEntry, KMsvServerId, ETrue));
	CleanupStack::PopAndDestroy(2); 
	TBool finished=EFalse;
	theFailCount=0;
	childEntry.iServiceId = 101;
	while (!finished)
		{
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		REPORT_IF_ERROR(index->LockEntry(childEntry.Id()));
		childEntry.iDescription.Set(aNewDescription);
		childEntry.iDetails.Set(aNewDetails);
		__UHEAP_FAILNEXT(theFailCount++);
		err = index->ChangeEntry(childEntry, KMsvServerId, EFalse);
		__UHEAP_RESET;
		// check it failed correctly or was successful
		(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering()));
		test(selection->Count()==1);
		if (err == KErrNone)
			{
			test(selection->At(0)->iServiceId == 101);
			test(selection->At(0)->iDescription.Length() == aNewDescription.Length());
			test(selection->At(0)->iDetails.Length() == aNewDetails.Length());
			}
		else
			{
			test(selection->At(0)->iServiceId == 1000);
			test(selection->At(0)->iDescription.Length() == aOrigDescription.Length());
			test(selection->At(0)->iDetails.Length() == aOrigDetails.Length());
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		(index->GetChildrenL(entry.Id(), *selection, TMsvSelectionOrdering()));
		test(selection->Count()==1);
		if (err == KErrNone)
			{
			test(selection->At(0)->iServiceId == 101);
			test(selection->At(0)->iDescription.Length() == aNewDescription.Length());
			test(selection->At(0)->iDetails.Length() == aNewDetails.Length());
			finished = ETrue;
			}
		else
			{
			test(selection->At(0)->iServiceId == 1000);
			test(selection->At(0)->iDescription.Length() == aOrigDescription.Length());
			test(selection->At(0)->iDetails.Length() == aOrigDetails.Length());
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);
		}
	CleanupStack::PopAndDestroy();
	}


LOCAL_C void HeapFailChangingWithChangingStringsAndMoving(const TDesC& aOrigDescription, const TDesC& aOrigDetails, const TDesC& aNewDescription, const TDesC& aNewDetails)
//
//
//
	{
	CPermanentFileStore* testStore;
	TTestMsvEntry entry1, entry2, childEntry; 
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(selection);
	TInt err=0;

	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry2.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	
	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	childEntry.SetParent(entry1.Id());
	childEntry.iServiceId = 1000;
	childEntry.iDescription.Set(aOrigDescription);
	childEntry.iDetails.Set(aOrigDetails);
	REPORT_IF_ERROR(index->AddEntry(childEntry, KMsvServerId, ETrue));
	CleanupStack::PopAndDestroy(2); 
	TBool finished=EFalse;
	theFailCount=0;
	childEntry.iServiceId = 101;
	childEntry.SetParent(entry2.Id());
	while (!finished)
		{
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		REPORT_IF_ERROR(index->LockEntry(childEntry.Id()));
		childEntry.iDescription.Set(aNewDescription);
		childEntry.iDetails.Set(aNewDetails);
		__UHEAP_FAILNEXT(theFailCount++);
		err = index->ChangeEntry(childEntry, KMsvServerId, EFalse);
		__UHEAP_RESET;
		// check it failed correctly or was successful
		(index->GetChildrenL(entry1.Id(), *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			{
			test(selection->Count()==0);
			selection->Reset();
			(index->GetChildrenL(entry2.Id(), *selection, TMsvSelectionOrdering()));
			test(selection->At(0)->iServiceId == 101);
			test(selection->At(0)->iDescription.Length() == aNewDescription.Length());
			test(selection->At(0)->iDetails.Length() == aNewDetails.Length());
			}
		else
			{
			test(selection->Count()==1);
			test(selection->At(0)->iServiceId == 1000);
			test(selection->At(0)->iDescription.Length() == aOrigDescription.Length());
			test(selection->At(0)->iDetails.Length() == aOrigDetails.Length());
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		(index->GetChildrenL(entry1.Id(), *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			{
			test(selection->Count()==0);
			selection->Reset();
			(index->GetChildrenL(entry2.Id(), *selection, TMsvSelectionOrdering()));
			test(selection->At(0)->iServiceId == 101);
			test(selection->At(0)->iDescription.Length() == aNewDescription.Length());
			test(selection->At(0)->iDetails.Length() == aNewDetails.Length());
			finished = ETrue;
			}
		else
			{
			test(selection->Count()==1);
			test(selection->At(0)->iServiceId == 1000);
			test(selection->At(0)->iDescription.Length() == aOrigDescription.Length());
			test(selection->At(0)->iDetails.Length() == aOrigDetails.Length());
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);
		}
	CleanupStack::PopAndDestroy();
	}





LOCAL_C void HeapFailAddingSingleChild(const TDesC& aDescription, const TDesC& aDetails)
//
//
//
	{
	CPermanentFileStore* testStore;
	TTestMsvEntry entry; 
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(selection);
	TInt err=0;

	TBool finished=EFalse;
	theFailCount=0;
	while (!finished)
		{
		theUtils->FileSession().Delete(KMsvTestFile);
		testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		testStore->SetTypeL(KPermanentFileStoreLayoutUid);
		index = CMsvServerIndex::NewL(*testStore);
		CleanupStack::PushL(index);
		CreateDefaultIndex(*index);
		entry.SetParent(KMsvGlobalInBoxIndexEntryId);
		entry.iDescription.Set(aDescription);
		entry.iDetails.Set(aDetails);
		// CHANGE TEMP to avoid failing in critical part of commit
		__UHEAP_FAILNEXT(theFailCount++);
		err = index->AddEntry(entry, KMsvServerId, ETrue);
		__UHEAP_RESET;
		// check it failed correctly or was successful
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			{
			test(selection->Count()==1);
			test(selection->At(0)->iDescription.Length() == aDescription.Length());
			test(selection->At(0)->iDetails.Length() == aDetails.Length());
			}
		else
			test(selection->Count()==0);
		selection->Reset();
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			{
			test(selection->Count()==1);
			test(selection->At(0)->iDescription.Length() == aDescription.Length());
			test(selection->At(0)->iDetails.Length() == aDetails.Length());
			finished = ETrue;
			}
		else
			test(selection->Count()==0);
		selection->Reset();
		CleanupStack::PopAndDestroy(2);
		}
	CleanupStack::PopAndDestroy();
	}




LOCAL_C void HeapFailAddingSibling(const TDesC& aDescription, const TDesC& aDetails)
//
//
//
	{
	CPermanentFileStore* testStore;
	TTestMsvEntry entry, sibling; 
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(selection);
	TInt err=0;

	TBool finished=EFalse;
	theFailCount=0;
	while (!finished)
		{
		theUtils->FileSession().Delete(KMsvTestFile);
		testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		testStore->SetTypeL(KPermanentFileStoreLayoutUid);
		index = CMsvServerIndex::NewL(*testStore);
		CleanupStack::PushL(index);
		CreateDefaultIndex(*index);
		entry.SetParent(KMsvGlobalInBoxIndexEntryId);
		entry.iDescription.Set(aDescription);
		entry.iDetails.Set(aDetails);
		sibling.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling.iDescription.Set(aDescription);
		sibling.iDetails.Set(aDetails);
		REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));
		// CHANGE TEMP to avoid failing in critical part of commit
		__UHEAP_FAILNEXT(theFailCount++);
		err = index->AddEntry(sibling, KMsvServerId, ETrue);
		__UHEAP_RESET;
		// check it failed correctly or was successful
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			{
			test(selection->Count()==2);
			if (selection->At(0)->Id()==sibling.Id())
				{
				test(selection->At(0)->iDescription.Length() == aDescription.Length());
				test(selection->At(0)->iDetails.Length() == aDetails.Length());
				}
			else
				{
				test(selection->At(1)->iDescription.Length() == aDescription.Length());
				test(selection->At(1)->iDetails.Length() == aDetails.Length());
				}
			}
		else
			test(selection->Count()==1);
		selection->Reset();
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			{
			test(selection->Count()==2);
			if (selection->At(0)->Id()==sibling.Id())
				{
				test(selection->At(0)->iDescription.Length() == aDescription.Length());
				test(selection->At(0)->iDetails.Length() == aDetails.Length());
				}
			else
				{
				test(selection->At(1)->iDescription.Length() == aDescription.Length());
				test(selection->At(1)->iDetails.Length() == aDetails.Length());
				}
			finished = ETrue;
			}
		else
			test(selection->Count()==1);
		selection->Reset();
		CleanupStack::PopAndDestroy(2);
		}
	CleanupStack::PopAndDestroy();
	}




LOCAL_C void HeapFailDeletingSingleChild(const TDesC& aDescription, const TDesC& aDetails)
//
//
//
	{
	CPermanentFileStore* testStore;
	TTestMsvEntry entry; 
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(selection);
	TInt err=0;
	
	

	TBool finished=EFalse;
	theFailCount=0;
	while (!finished)
		{
		theUtils->FileSession().Delete(KMsvTestFile);
		testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		testStore->SetTypeL(KPermanentFileStoreLayoutUid);
		index = CMsvServerIndex::NewL(*testStore);
		CleanupStack::PushL(index);
		CreateDefaultIndex(*index);
		entry.SetParent(KMsvGlobalInBoxIndexEntryId);
		entry.iDescription.Set(aDescription);
		entry.iDetails.Set(aDetails);
		REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));
		index->LockEntryAndStore(entry.Id());
		// CHANGE TEMP to avoid failing in critical part of commit
		__UHEAP_FAILNEXT(theFailCount++);
		err = index->DeleteEntry(entry.Id());
		__UHEAP_RESET;
		// check it failed correctly or was successful
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			test(selection->Count()==0);
		else
			{
			index->ReleaseEntryAndStore(entry.Id());
			test(selection->Count()==1);
			test(selection->At(0)->iDescription.Length() == aDescription.Length());
			test(selection->At(0)->iDetails.Length() == aDetails.Length());
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			{
			test(selection->Count()==0);
			finished = ETrue;
			}
		else
			{
			test(selection->Count()==1);
			test(selection->At(0)->iDescription.Length() == aDescription.Length());
			test(selection->At(0)->iDetails.Length() == aDetails.Length());
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);
		}
	CleanupStack::PopAndDestroy();
	}





LOCAL_C void HeapFailDeletingFinalSibling(const TDesC& aDescription, const TDesC& aDetails)
//
//
//
	{
	CPermanentFileStore* testStore;
	TTestMsvEntry sibling1,sibling2; 
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(selection);
	TInt err=0;

	TBool finished=EFalse;
	theFailCount=0;
	while (!finished)
		{
		theUtils->FileSession().Delete(KMsvTestFile);
		testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		testStore->SetTypeL(KPermanentFileStoreLayoutUid);
		index = CMsvServerIndex::NewL(*testStore);
		CleanupStack::PushL(index);
		CreateDefaultIndex(*index);
		sibling1.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling1.iDescription.Set(aDescription);
		sibling1.iDetails.Set(aDetails);
		sibling2.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling2.iDescription.Set(aDescription);
		sibling2.iDetails.Set(aDetails);
		REPORT_IF_ERROR(index->AddEntry(sibling1, KMsvServerId, ETrue));
		REPORT_IF_ERROR(index->AddEntry(sibling2, KMsvServerId, ETrue));
		// CHANGE TEMP to avoid failing in critical part of commit
		index->LockEntryAndStore(sibling2.Id());
		__UHEAP_FAILNEXT(theFailCount++);
		err = index->DeleteEntry(sibling2.Id());
		__UHEAP_RESET;
		// check it failed correctly or was successful
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			test(selection->Count()==1);
		else
			{
			index->ReleaseEntryAndStore(sibling2.Id());
			test(selection->Count()==2);
			if (selection->At(0)->Id()==sibling2.Id())
				{
				test(selection->At(0)->iDescription.Length() == aDescription.Length());
				test(selection->At(0)->iDetails.Length() == aDetails.Length());
				}
			else
				{
				test(selection->At(1)->iDescription.Length() == aDescription.Length());
				test(selection->At(1)->iDetails.Length() == aDetails.Length());
				}
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			{
			test(selection->Count()==1);
			finished = ETrue;
			}
		else
			{
			test(selection->Count()==2);
			if (selection->At(0)->Id()==sibling2.Id())
				{
				test(selection->At(0)->iDescription.Length() == aDescription.Length());
				test(selection->At(0)->iDetails.Length() == aDetails.Length());
				}
			else
				{
				test(selection->At(1)->iDescription.Length() == aDescription.Length());
				test(selection->At(1)->iDetails.Length() == aDetails.Length());
				}
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);
		}
	CleanupStack::PopAndDestroy();
	}






LOCAL_C void HeapFailDeletingFirstSibling(const TDesC& aDescription, const TDesC& aDetails)
//
//
//
	{
	CPermanentFileStore* testStore;
	TTestMsvEntry sibling1,sibling2; 
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(selection);
	TInt err=0;

	

	TBool finished=EFalse;
	theFailCount=0;
	while (!finished)
		{
		theUtils->FileSession().Delete(KMsvTestFile);
		testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		testStore->SetTypeL(KPermanentFileStoreLayoutUid);
		index = CMsvServerIndex::NewL(*testStore);
		CleanupStack::PushL(index);
		CreateDefaultIndex(*index);
		sibling1.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling1.iDescription.Set(aDescription);
		sibling1.iDetails.Set(aDetails);
		sibling2.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling2.iDescription.Set(aDescription);
		sibling2.iDetails.Set(aDetails);
		REPORT_IF_ERROR(index->AddEntry(sibling1, KMsvServerId, ETrue));
		REPORT_IF_ERROR(index->AddEntry(sibling2, KMsvServerId, ETrue));
		// CHANGE TEMP to avoid failing in critical part of commit
		index->LockEntryAndStore(sibling1.Id());
		__UHEAP_FAILNEXT(theFailCount++);
		err = index->DeleteEntry(sibling1.Id());
		__UHEAP_RESET;
		// check it failed correctly or was successful
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			test(selection->Count()==1);
		else
			{
			index->ReleaseEntryAndStore(sibling1.Id());
			test(selection->Count()==2);
			if (selection->At(0)->Id()==sibling1.Id())
				{
				test(selection->At(0)->iDescription.Length() == aDescription.Length());
				test(selection->At(0)->iDetails.Length() == aDetails.Length());
				}
			else
				{
				test(selection->At(1)->iDescription.Length() == aDescription.Length());
				test(selection->At(1)->iDetails.Length() == aDetails.Length());
				}
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			{
			test(selection->Count()==1);
			finished = ETrue;
			}
		else
			{
			test(selection->Count()==2);
			if (selection->At(0)->Id()==sibling1.Id())
				{
				test(selection->At(0)->iDescription.Length() == aDescription.Length());
				test(selection->At(0)->iDetails.Length() == aDetails.Length());
				}
			else
				{
				test(selection->At(1)->iDescription.Length() == aDescription.Length());
				test(selection->At(1)->iDetails.Length() == aDetails.Length());
				}
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);
		}
	CleanupStack::PopAndDestroy();
	}




LOCAL_C void HeapFailDeletingMiddleSibling(const TDesC& aDescription, const TDesC& aDetails)
//
//
//
	{
	CPermanentFileStore* testStore;
	TTestMsvEntry sibling1,sibling2, sibling3; 
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(selection);
	TInt err=0;

	

	TBool finished=EFalse;
	theFailCount=0;
	while (!finished)
		{
		theUtils->FileSession().Delete(KMsvTestFile);
		testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		testStore->SetTypeL(KPermanentFileStoreLayoutUid);
		index = CMsvServerIndex::NewL(*testStore);
		CleanupStack::PushL(index);
		CreateDefaultIndex(*index);
		sibling1.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling1.iDescription.Set(aDescription);
		sibling1.iDetails.Set(aDetails);
		sibling2.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling2.iDescription.Set(aDescription);
		sibling2.iDetails.Set(aDetails);
		sibling3.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling3.iDescription.Set(aDescription);
		sibling3.iDetails.Set(aDetails);
		REPORT_IF_ERROR(index->AddEntry(sibling1, KMsvServerId, ETrue));
		REPORT_IF_ERROR(index->AddEntry(sibling2, KMsvServerId, ETrue));
		REPORT_IF_ERROR(index->AddEntry(sibling3, KMsvServerId, ETrue));
		// CHANGE TEMP to avoid failing in critical part of commit
		index->LockEntryAndStore(sibling2.Id());
		__UHEAP_FAILNEXT(theFailCount++);
		err = index->DeleteEntry(sibling2.Id());
		__UHEAP_RESET;
		// check it failed correctly or was successful
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			test(selection->Count()==2);
		else
			{
			index->ReleaseEntryAndStore(sibling2.Id());
			test(selection->Count()==3);
			if (selection->At(0)->Id()==sibling1.Id())
				{
				test(selection->At(0)->iDescription.Length() == aDescription.Length());
				test(selection->At(0)->iDetails.Length() == aDetails.Length());
				}
			else if (selection->At(1)->Id()==sibling1.Id())
				{
				test(selection->At(1)->iDescription.Length() == aDescription.Length());
				test(selection->At(1)->iDetails.Length() == aDetails.Length());
				}
			else 
				{
				test(selection->At(2)->iDescription.Length() == aDescription.Length());
				test(selection->At(2)->iDetails.Length() == aDetails.Length());
				}
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			{
			test(selection->Count()==2);
			finished = ETrue;
			}
		else
			{
			test(selection->Count()==3);
			if (selection->At(0)->Id()==sibling1.Id())
				{
				test(selection->At(0)->iDescription.Length() == aDescription.Length());
				test(selection->At(0)->iDetails.Length() == aDetails.Length());
				}
			else if (selection->At(1)->Id()==sibling1.Id())
				{
				test(selection->At(1)->iDescription.Length() == aDescription.Length());
				test(selection->At(1)->iDetails.Length() == aDetails.Length());
				}
			else 
				{
				test(selection->At(2)->iDescription.Length() == aDescription.Length());
				test(selection->At(2)->iDetails.Length() == aDetails.Length());
				}
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);
		}
	CleanupStack::PopAndDestroy();
	}



LOCAL_C void HeapFailDeletingLastTwoSiblings(const TDesC& aDescription, const TDesC& aDetails)
//
//
//
	{
	CPermanentFileStore* testStore;
	TTestMsvEntry sibling1,sibling2, sibling3; 
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(selection);
	TInt err=0;
	TInt result=0;
	

	TBool finished=EFalse;
	theFailCount=0;
	while (!finished)
		{
		theUtils->FileSession().Delete(KMsvTestFile);
		testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		testStore->SetTypeL(KPermanentFileStoreLayoutUid);
		index = CMsvServerIndex::NewL(*testStore);
		CleanupStack::PushL(index);
		CreateDefaultIndex(*index);
		sibling1.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling1.iDescription.Set(aDescription);
		sibling1.iDetails.Set(aDetails);
		sibling2.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling2.iDescription.Set(aDescription);
		sibling2.iDetails.Set(aDetails);
		sibling3.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling3.iDescription.Set(aDescription);
		sibling3.iDetails.Set(aDetails);
		REPORT_IF_ERROR(index->AddEntry(sibling1, KMsvServerId, ETrue));
		REPORT_IF_ERROR(index->AddEntry(sibling2, KMsvServerId, ETrue));
		REPORT_IF_ERROR(index->AddEntry(sibling3, KMsvServerId, ETrue));
		// CHANGE TEMP to avoid failing in critical part of commit
		index->LockEntryAndStore(sibling2.Id());
		index->LockEntryAndStore(sibling3.Id());
		__UHEAP_FAILNEXT(theFailCount++);
		err = index->DeleteEntry(sibling2.Id());
		if (err==KErrNone)	
			err = index->DeleteEntry(sibling3.Id());
		__UHEAP_RESET;
		// check it failed correctly or was successful
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			test(selection->Count()==1);
		else
			{
			test(selection->Count()==2||selection->Count()==3);
			result=selection->Count();
			index->ReleaseEntryAndStore(sibling2.Id());
			index->ReleaseEntryAndStore(sibling3.Id());
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			{
			test(selection->Count()==1);
			finished = ETrue;
			}
		else
			{
			test(selection->Count()==result);
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);
		}
	CleanupStack::PopAndDestroy();
	}




LOCAL_C void HeapFailDeletingLastTwoSiblingsReversed(const TDesC& aDescription, const TDesC& aDetails)
//
//
//
	{
	CPermanentFileStore* testStore;
	TTestMsvEntry sibling1,sibling2, sibling3; 
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(selection);
	TInt err=0;
	TInt result=0;

	

	TBool finished=EFalse;
	theFailCount=0;
	while (!finished)
		{
		theUtils->FileSession().Delete(KMsvTestFile);
		testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		testStore->SetTypeL(KPermanentFileStoreLayoutUid);
		index = CMsvServerIndex::NewL(*testStore);
		CleanupStack::PushL(index);
		CreateDefaultIndex(*index);
		sibling1.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling1.iDescription.Set(aDescription);
		sibling1.iDetails.Set(aDetails);
		sibling2.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling2.iDescription.Set(aDescription);
		sibling2.iDetails.Set(aDetails);
		sibling3.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling3.iDescription.Set(aDescription);
		sibling3.iDetails.Set(aDetails);
		REPORT_IF_ERROR(index->AddEntry(sibling1, KMsvServerId, ETrue));
		REPORT_IF_ERROR(index->AddEntry(sibling2, KMsvServerId, ETrue));
		REPORT_IF_ERROR(index->AddEntry(sibling3, KMsvServerId, ETrue));
		// CHANGE TEMP to avoid failing in critical part of commit
		index->LockEntryAndStore(sibling2.Id());
		index->LockEntryAndStore(sibling3.Id());
		__UHEAP_FAILNEXT(theFailCount++);
		err = index->DeleteEntry(sibling3.Id());
		if (err==KErrNone)	
			err = index->DeleteEntry(sibling2.Id());
		__UHEAP_RESET;
		// check it failed correctly or was successful
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			test(selection->Count()==1);
		else
			{
			index->ReleaseEntryAndStore(sibling2.Id());
			index->ReleaseEntryAndStore(sibling3.Id());
			test(selection->Count()==3||selection->Count()==2);
			result = selection->Count();
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			{
			test(selection->Count()==1);
			finished = ETrue;
			}
		else
			{
			test(selection->Count()==result);
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);
		}
	CleanupStack::PopAndDestroy();
	}


LOCAL_C void HeapFailDeletingFirstTwoSiblings(const TDesC& aDescription, const TDesC& aDetails)
//
//
//
	{
	CPermanentFileStore* testStore;
	TTestMsvEntry sibling1,sibling2, sibling3; 
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(selection);
	TInt err=0;
	TInt result=0;

	

	TBool finished=EFalse;
	theFailCount=0;
	while (!finished)
		{
		theUtils->FileSession().Delete(KMsvTestFile);
		testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		testStore->SetTypeL(KPermanentFileStoreLayoutUid);
		index = CMsvServerIndex::NewL(*testStore);
		CleanupStack::PushL(index);
		CreateDefaultIndex(*index);
		sibling1.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling1.iDescription.Set(aDescription);
		sibling1.iDetails.Set(aDetails);
		sibling2.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling2.iDescription.Set(aDescription);
		sibling2.iDetails.Set(aDetails);
		sibling3.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling3.iDescription.Set(aDescription);
		sibling3.iDetails.Set(aDetails);
		REPORT_IF_ERROR(index->AddEntry(sibling1, KMsvServerId, ETrue));
		REPORT_IF_ERROR(index->AddEntry(sibling2, KMsvServerId, ETrue));
		REPORT_IF_ERROR(index->AddEntry(sibling3, KMsvServerId, ETrue));
		// CHANGE TEMP to avoid failing in critical part of commit
		index->LockEntryAndStore(sibling1.Id());
		index->LockEntryAndStore(sibling2.Id());
		__UHEAP_FAILNEXT(theFailCount++);
		err = index->DeleteEntry(sibling1.Id());
		if (err==KErrNone)	
			err = index->DeleteEntry(sibling2.Id());
		__UHEAP_RESET;
		// check it failed correctly or was successful
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			test(selection->Count()==1);
		else
			{
			index->ReleaseEntryAndStore(sibling1.Id());
			index->ReleaseEntryAndStore(sibling2.Id());
			test(selection->Count()==3||selection->Count()==2);
			result=selection->Count();
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			{
			test(selection->Count()==1);
			finished = ETrue;
			}
		else
			{
			test(selection->Count()==result);
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);
		}
	CleanupStack::PopAndDestroy();
	}



LOCAL_C void HeapFailDeletingFirstTwoSiblingsReversed(const TDesC& aDescription, const TDesC& aDetails)
//
//
//
	{
	CPermanentFileStore* testStore;
	TTestMsvEntry sibling1,sibling2, sibling3; 
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(selection);
	TInt err=0;
	TInt result=0;

	

	TBool finished=EFalse;
	theFailCount=0;
	while (!finished)
		{
		theUtils->FileSession().Delete(KMsvTestFile);
		testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		testStore->SetTypeL(KPermanentFileStoreLayoutUid);
		index = CMsvServerIndex::NewL(*testStore);
		CleanupStack::PushL(index);
		CreateDefaultIndex(*index);
		sibling1.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling1.iDescription.Set(aDescription);
		sibling1.iDetails.Set(aDetails);
		sibling2.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling2.iDescription.Set(aDescription);
		sibling2.iDetails.Set(aDetails);
		sibling3.SetParent(KMsvGlobalInBoxIndexEntryId);
		sibling3.iDescription.Set(aDescription);
		sibling3.iDetails.Set(aDetails);
		REPORT_IF_ERROR(index->AddEntry(sibling1, KMsvServerId, ETrue));
		REPORT_IF_ERROR(index->AddEntry(sibling2, KMsvServerId, ETrue));
		REPORT_IF_ERROR(index->AddEntry(sibling3, KMsvServerId, ETrue));
		// CHANGE TEMP to avoid failing in critical part of commit
		index->LockEntryAndStore(sibling2.Id());
		index->LockEntryAndStore(sibling1.Id());
		__UHEAP_FAILNEXT(theFailCount++);
		err = index->DeleteEntry(sibling2.Id());
		if (err==KErrNone)	
			err = index->DeleteEntry(sibling1.Id());
		__UHEAP_RESET;
		// check it failed correctly or was successful
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			test(selection->Count()==1);
		else
			{
			index->ReleaseEntryAndStore(sibling1.Id());
			index->ReleaseEntryAndStore(sibling2.Id());
			test(selection->Count()==3||selection->Count()==2);
			result=selection->Count();
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			{
			test(selection->Count()==1);
			finished = ETrue;
			}
		else
			{
			test(selection->Count()==result);
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);
		}
	CleanupStack::PopAndDestroy();
	}



LOCAL_C void TestCreatingWithHeapFailuresL()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index=NULL;
	TInt err;

	// fail creating the index after increasing RFs calls until the index is successfully created
	TBool finished=EFalse;
	theFailCount=0;
	while (!finished)
		{
		theUtils->FileSession().Delete(KMsvTestFile);
		testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		testStore->SetTypeL(KPermanentFileStoreLayoutUid);
		__UHEAP_FAILNEXT(theFailCount++);
		TRAP(err, {index = CMsvServerIndex::NewL(*testStore);});
		if (err==KErrNone)
			{
			delete index;
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(err==KErrNoMemory);
			}
		CleanupStack::PopAndDestroy(); 
		}

	// test the index was succesfully created
	testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	CleanupStack::PopAndDestroy(2); 
	}






LOCAL_C void TestTemporaryFlagsWithHeapFailuresL()
	{
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	CMsvServerIndex* index = CMsvServerIndex::NewL(*DummyStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	TMsvEntry* entryPtr=NULL;

	// create connected entry 
	TTestMsvEntry entry1;
	entry1.SetConnected(ETrue);
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(),entryPtr));
	test(entryPtr->Connected());

	// change to unconnected with s
	entry1.SetConnected(EFalse);
	REPORT_IF_ERROR(index->LockEntry(entry1.Id()));
	__UHEAP_FAILNEXT(0);
	REPORT_IF_ERROR(index->ChangeTemporaryData(entry1));
	REPORT_IF_ERROR(index->ReleaseEntry(entry1.Id()));
	__UHEAP_RESET;
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(),entryPtr));
	test(!entryPtr->Connected());

	// create connected entry with long string
	TTestMsvEntry entry2;
	entry2.SetConnected(ETrue);
	entry2.iDescription.Set(KLongDescription);
	entry2.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(),entryPtr));
	test(entryPtr->Connected());

	// change to unconnected with s
	entry2.SetConnected(EFalse);
	REPORT_IF_ERROR(index->LockEntry(entry2.Id()));
	__UHEAP_FAILNEXT(0);
	REPORT_IF_ERROR(index->ChangeTemporaryData(entry2));
	REPORT_IF_ERROR(index->ReleaseEntry(entry2.Id()));
	__UHEAP_RESET;
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(),entryPtr));
	test(!entryPtr->Connected());

	CleanupStack::PopAndDestroy(); // index
	}



class CIndexOb : public CBase, public MMsvIndexObserver
	{
public:
	CIndexOb() {};
	void IndexLoadingCompleted(TInt aError) {iError=aError; CActiveScheduler::Stop();};
	void IndexRebuildingCompleted(TInt aError) {iError=aError; CActiveScheduler::Stop();};
public:
	TInt iError;
	};




LOCAL_C void TestAsyncOpenWithHeapFailureL()
	{
	CIndexOb* ob = new(ELeave) CIndexOb;
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;

	CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();

	theUtils->FileSession().Delete(KMsvTestDummyFile);
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;

	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	
	

	TTestMsvEntry entry;
	entry.SetParent(KMsvRootIndexEntryId);

	for (TInt i=1; i<=50; i++)
		{
		REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));
		delete index;
		index=NULL;
		
		theFailCount=0;
		TBool failed=ETrue;
		while (failed)
			{
			__UHEAP_FAILNEXT(theFailCount++);
			TRAPD(leave, index = CMsvServerIndex::OpenL(*DummyStore, *ob));
			if (leave==KErrNone)
				CActiveScheduler::Start();
			__UHEAP_RESET;
			if (ob->iError==KErrNone && leave==KErrNone)
				{
				failed=EFalse;
				REPORT_IF_ERROR(index->GetChildrenId(KMsvRootIndexEntryId, *filter, *selection));
				test(selection->Count()==i);
				selection->Reset();
				}
			else
				{
				delete index;
				index=NULL;
				}
			}
		}
	delete selection;
	delete ob;

	CleanupStack::PopAndDestroy(3); // index, testStore, filter
	}

LOCAL_C void TestMovingWithHeapFailureL()
	{
	CArrayPtrFlat<const TMsvEntry>* childrenSelection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(childrenSelection);

	CPermanentFileStore* testStore;
	CMsvServerIndex* index;

	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);

	
	// create entry to work under
	TTestMsvEntry entry;
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));

	// create two folders
	TTestMsvEntry folder1;
	folder1.SetParent(entry.Id());
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(folder1, KMsvServerId, ETrue));

	TTestMsvEntry folder2;
	folder2.SetParent(entry.Id());
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(folder2, KMsvServerId, ETrue));

	// create entry in folder1
	TTestMsvEntry entry1;
	entry1.SetParent(folder1.Id());
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));

	// Delete store and index
	CleanupStack::PopAndDestroy(2);

	TBool finished;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	
	TInt err;

	finished=EFalse;


	// move single entry
	while (!finished)
		{
	
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);

		__UHEAP_FAILNEXT(failCount++);

		REPORT_IF_ERROR(index->LockEntryAndStore(entry1.Id()));
		err = index->MoveEntry(entry1.Id(), folder2.Id());
		REPORT_IF_ERROR(index->ReleaseEntryAndStore(entry1.Id()));

		__UHEAP_RESET;

		if (err==KErrNone)
			{
			finished=ETrue;

			// check that the entry has been moved
			(index->GetChildrenL(folder1.Id(), *childrenSelection, TMsvSelectionOrdering()));
			test(childrenSelection->Count() == 0);
			childrenSelection->Reset();
			(index->GetChildrenL(folder2.Id(), *childrenSelection, TMsvSelectionOrdering()));
			test(childrenSelection->Count() == 1);
			childrenSelection->Reset();
			}
		else 
			{
			test(err==KErrNoMemory);

			// check that the entry has not been moved
			(index->GetChildrenL(folder1.Id(), *childrenSelection, TMsvSelectionOrdering()));
			test(childrenSelection->Count() == 1);
			childrenSelection->Reset();
			(index->GetChildrenL(folder2.Id(), *childrenSelection, TMsvSelectionOrdering()));
			test(childrenSelection->Count() == 0);
			childrenSelection->Reset();
			}

		// Delete store and index
		CleanupStack::PopAndDestroy(2);

		// Test file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
	
		// Test for the entry in the Inbox
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *childrenSelection, TMsvSelectionOrdering()));
		test(childrenSelection->Count()==1);
		test(childrenSelection->At(0)->Id()==entry.Id());
		childrenSelection->Reset();

		// Test for folders
		(index->GetChildrenL(entry.Id(), *childrenSelection, TMsvSelectionOrdering()));
		test(childrenSelection->Count()==2);
		test(childrenSelection->At(0)->Id()==folder1.Id());
		test(childrenSelection->At(1)->Id()==folder2.Id());
		childrenSelection->Reset();

		// Test for entry in folder
		if (err==KErrNone)
			{
			(index->GetChildrenL(folder2.Id(), *childrenSelection, TMsvSelectionOrdering()));
			test(childrenSelection->Count()==1);
			test(childrenSelection->At(0)->Id()==entry1.Id());
			childrenSelection->Reset();

			(index->GetChildrenL(folder1.Id(), *childrenSelection, TMsvSelectionOrdering()));
			test(childrenSelection->Count()==0);
			childrenSelection->Reset();
			}
		else
			{
			(index->GetChildrenL(folder1.Id(), *childrenSelection, TMsvSelectionOrdering()));
			test(childrenSelection->Count()==1);
			test(childrenSelection->At(0)->Id()==entry1.Id());
			childrenSelection->Reset();

			(index->GetChildrenL(folder2.Id(), *childrenSelection, TMsvSelectionOrdering()));
			test(childrenSelection->Count()==0);
			childrenSelection->Reset();
			}
	
		CleanupStack::PopAndDestroy(2);
		}
	CleanupStack::PopAndDestroy();
	}


LOCAL_C void TestAttributesWithHeapFailureL()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;

	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);

	// create entry to work with
	TTestMsvEntry entry1;
	entry1.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	TTestMsvEntry entry2;
	entry2.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	TTestMsvEntry entry3;
	entry3.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));

	TMsvEntry* entryPtr;

	// test
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected());

	// work with all entries
	CMsvEntrySelection* selection=new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);
	REPORT_IF_ERROR(index->LockEntry(entry1.Id()));
	REPORT_IF_ERROR(index->LockEntry(entry2.Id()));
	REPORT_IF_ERROR(index->LockEntry(entry3.Id()));


	theFailCount=0;
	TInt error=KErrGeneral;
	while (error)
		{
		selection->AppendL(entry1.Id());selection->AppendL(entry2.Id());selection->AppendL(entry3.Id());
		__UHEAP_FAILNEXT(theFailCount++);
		error = index->ChangeAttributes(*selection, 0, KMsvVisibilityAttribute);
		__UHEAP_RESET;

		if (error==KErrNone)
			{
			test(selection->Count()==3);
			selection->Reset();
			// test
			REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
			test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(!entryPtr->Connected());
			REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
			test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(!entryPtr->Connected());
			REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
			test(!entryPtr->Unread()); test(!entryPtr->Visible()); test(!entryPtr->Connected());
			}
		else
			{
			selection->Reset();
			REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
			test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected());
			REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
			test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected());
			REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
			test(!entryPtr->Unread()); test(entryPtr->Visible()); test(!entryPtr->Connected());
			}
		}

	CleanupStack::PopAndDestroy(3);
	}



LOCAL_C TInt DoDeleteSelectionWithHeapFailure(CMsvServerIndex& aIndex, CMsvEntrySelection& aSelection, TMsvId aId)
	{
	// expand the selection
	aSelection.Reset();
	aSelection.AppendL(aId);
	REPORT_IF_ERROR(aIndex.ExpandSelectionRecursively(aSelection));

	// lock'm
	TInt count=aSelection.Count();
	while (count--)
		REPORT_IF_ERROR(aIndex.LockEntryAndStore(aSelection.At(count)));

	// delete them
	__UHEAP_FAILNEXT(theFailCount++);
	TInt error = aIndex.DeleteSelection(aSelection);
	__UHEAP_RESET;

	// release them
	if (error)
		{
		count=aSelection.Count();
		while (count--)
			REPORT_IF_ERROR(aIndex.ReleaseEntryAndStore(aSelection.At(count)));
		}

	return error;
	}



LOCAL_C TInt DoDeleteSelectionWithHeapFailure2(CMsvServerIndex& aIndex, CMsvEntrySelection& aSelection)
	{
	// lock'm
	TInt count=aSelection.Count();
	while (count--)
		REPORT_IF_ERROR(aIndex.LockEntryAndStore(aSelection.At(count)));

	// delete them
	__UHEAP_FAILNEXT(theFailCount++);
	TInt error = aIndex.DeleteSelection(aSelection);
	__UHEAP_RESET;

	// release them
	if (error)
		{
		count=aSelection.Count();
		while (count--)
			REPORT_IF_ERROR(aIndex.ReleaseEntryAndStore(aSelection.At(count)));
		}

	return error;
	}




LOCAL_C void TestSelectionDeletionWithHeapFailure1L()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;

	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TTestMsvEntry folder;
	folder.iDescription.Set(KShortDescription);
	folder.iDetails.Set(KShortDetails);
	TTestMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	TTestMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KLongDetails);
	TTestMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	TTestMsvEntry entry4;
	entry4.iDescription.Set(KLongDescription);
	entry4.iDetails.Set(KLongDetails);
	TTestMsvEntry entry5;
	entry5.iDescription.Set(KLongDescription);
	entry5.iDetails.Set(KLongDetails);
	TTestMsvEntry entry6;
	entry6.iDescription.Set(KShortDescription);
	entry6.iDetails.Set(KLongDetails);
	TTestMsvEntry entry7;
	entry7.iDescription.Set(KShortDescription);
	entry7.iDetails.Set(KShortDescription);
	TTestMsvEntry entry8;
	entry8.iDescription.Set(KShortDescription);
	entry8.iDetails.Set(KShortDescription);

	//
	// delete entry with one child
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	// 
	theFailCount=0;
	TInt error=KErrGeneral;
	while (error)
		{
		error = DoDeleteSelectionWithHeapFailure(*index, *selection, folder.Id());
		if (error==KErrNone)
			{
			test(!index->EntryExists(folder.Id()));
			test(!index->EntryExists(entry1.Id()));
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			}
		}


	//
	// vertical 
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(entry1.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	// 
	theFailCount=0;
	error=KErrGeneral;
	while (error)
		{
		error = DoDeleteSelectionWithHeapFailure(*index, *selection, folder.Id());
		if (error==KErrNone)
			{
			test(!index->EntryExists(folder.Id()));
			test(!index->EntryExists(entry1.Id()));
			test(!index->EntryExists(entry2.Id()));
			test(!index->EntryExists(entry3.Id()));
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
			}
		theFailCount++;
		}


	//
	// horizontal
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	// 
	theFailCount=0;
	error=KErrGeneral;
	while (error)
		{
		error = DoDeleteSelectionWithHeapFailure(*index, *selection, folder.Id());
		if (error==KErrNone)
			{
			test(!index->EntryExists(folder.Id()));
			test(!index->EntryExists(entry1.Id()));
			test(!index->EntryExists(entry2.Id()));
			test(!index->EntryExists(entry3.Id()));
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
			}
		}
	

	//
	// complex
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(entry4.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(entry4.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(entry6.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	// 
	theFailCount=0;
	error=KErrGeneral;
	while (error)
		{
		error = DoDeleteSelectionWithHeapFailure(*index, *selection, folder.Id());
		if (error==KErrNone)
			{
			test(!index->EntryExists(folder.Id()));
			test(!index->EntryExists(entry1.Id()));
			test(!index->EntryExists(entry2.Id()));
			test(!index->EntryExists(entry3.Id()));
			test(!index->EntryExists(entry4.Id()));
			test(!index->EntryExists(entry5.Id()));
			test(!index->EntryExists(entry6.Id()));
			test(!index->EntryExists(entry7.Id()));
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
			test(index->EntryExists(entry4.Id()));
			test(index->EntryExists(entry5.Id()));
			test(index->EntryExists(entry6.Id()));
			test(index->EntryExists(entry7.Id()));
			}
		}
	
	
	//
	// complex
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(entry1.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(entry3.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(entry5.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(entry6.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));

	// 
	theFailCount=0;
	error=KErrGeneral;
	while (error)
		{
		error = DoDeleteSelectionWithHeapFailure(*index, *selection, folder.Id());
		if (error==KErrNone)
			{
			test(!index->EntryExists(folder.Id()));
			test(!index->EntryExists(entry1.Id()));
			test(!index->EntryExists(entry2.Id()));
			test(!index->EntryExists(entry3.Id()));
			test(!index->EntryExists(entry4.Id()));
			test(!index->EntryExists(entry5.Id()));
			test(!index->EntryExists(entry6.Id()));
			test(!index->EntryExists(entry7.Id()));
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
			test(index->EntryExists(entry4.Id()));
			test(index->EntryExists(entry5.Id()));
			test(index->EntryExists(entry6.Id()));
			test(index->EntryExists(entry7.Id()));
			}
		}

	//
	// complex
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(entry1.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(entry1.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(entry1.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(entry1.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(entry6.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	// 
	theFailCount=0;
	error=KErrGeneral;
	while (error)
		{
		error = DoDeleteSelectionWithHeapFailure(*index, *selection, folder.Id());
		if (error==KErrNone)
			{
			test(!index->EntryExists(folder.Id()));
			test(!index->EntryExists(entry1.Id()));
			test(!index->EntryExists(entry2.Id()));
			test(!index->EntryExists(entry3.Id()));
			test(!index->EntryExists(entry4.Id()));
			test(!index->EntryExists(entry5.Id()));
			test(!index->EntryExists(entry6.Id()));
			test(!index->EntryExists(entry7.Id()));
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry3.Id()));
			test(index->EntryExists(entry4.Id()));
			test(index->EntryExists(entry5.Id()));
			test(index->EntryExists(entry6.Id()));
			test(index->EntryExists(entry7.Id()));
			}
		}

	//
	// complex
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(entry4.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(entry5.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(entry6.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	entry8.SetParent(entry6.Id());
	REPORT_IF_ERROR(index->AddEntry(entry8, KMsvServerId, ETrue));
	// 
	theFailCount=0;
	error=KErrGeneral;
	while (error)
		{
		error = DoDeleteSelectionWithHeapFailure(*index, *selection, folder.Id());
		if (error==KErrNone)
			{
			test(!index->EntryExists(folder.Id()));
			test(!index->EntryExists(entry2.Id()));
			test(!index->EntryExists(entry1.Id()));
			test(!index->EntryExists(entry3.Id()));
			test(!index->EntryExists(entry4.Id()));
			test(!index->EntryExists(entry5.Id()));
			test(!index->EntryExists(entry6.Id()));
			test(!index->EntryExists(entry7.Id()));
			test(!index->EntryExists(entry8.Id()));
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
			test(index->EntryExists(entry4.Id()));
			test(index->EntryExists(entry5.Id()));
			test(index->EntryExists(entry6.Id()));
			test(index->EntryExists(entry8.Id()));
			}
		}

	
	//
	// complex
	folder.SetParent(KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(entry4.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(entry5.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(entry6.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	entry8.SetParent(entry6.Id());
	REPORT_IF_ERROR(index->AddEntry(entry8, KMsvServerId, ETrue));
	// 
	theFailCount=0;
	error=KErrGeneral;
	while (error)
		{
		selection->Reset();
		selection->AppendL(folder.Id());
		REPORT_IF_ERROR(index->ExpandSelectionRecursively(*selection));
		selection->Delete(selection->Count()-1);
		error = DoDeleteSelectionWithHeapFailure2(*index, *selection);
		if (error==KErrNone)
			{
			test(index->EntryExists(folder.Id()));
			test(!index->EntryExists(entry1.Id()));
			test(!index->EntryExists(entry2.Id()));
			test(!index->EntryExists(entry3.Id()));
			test(!index->EntryExists(entry4.Id()));
			test(!index->EntryExists(entry5.Id()));
			test(!index->EntryExists(entry6.Id()));
			test(!index->EntryExists(entry7.Id()));
			test(!index->EntryExists(entry8.Id()));
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
			test(index->EntryExists(entry4.Id()));
			test(index->EntryExists(entry5.Id()));
			test(index->EntryExists(entry6.Id()));
			test(index->EntryExists(entry8.Id()));
			}
		}


	//
	// complex
	entry1.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
	entry2.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
	entry3.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	entry8.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry8, KMsvServerId, ETrue));
	// 
	theFailCount=0;
	error=KErrGeneral;
	while (error)
		{
		selection->Reset();
		selection->AppendL(folder.Id());
		REPORT_IF_ERROR(index->ExpandSelectionRecursively(*selection));
		selection->Delete(selection->Count()-3,3);
		error = DoDeleteSelectionWithHeapFailure2(*index, *selection);
		if (error==KErrNone)
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(!index->EntryExists(entry3.Id()));
			test(!index->EntryExists(entry4.Id()));
			test(!index->EntryExists(entry5.Id()));
			test(!index->EntryExists(entry6.Id()));
			test(!index->EntryExists(entry7.Id()));
			test(!index->EntryExists(entry8.Id()));
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
			test(index->EntryExists(entry4.Id()));
			test(index->EntryExists(entry5.Id()));
			test(index->EntryExists(entry6.Id()));
			test(index->EntryExists(entry8.Id()));
			}
		}


	//
	// complex
	entry3.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	entry8.SetParent(entry7.Id());
	REPORT_IF_ERROR(index->AddEntry(entry8, KMsvServerId, ETrue));
	// 
	theFailCount=0;
	error=KErrGeneral;
	while (error)
		{
		selection->Reset();
		selection->AppendL(folder.Id());
		REPORT_IF_ERROR(index->ExpandSelectionRecursively(*selection));
		selection->Delete(selection->Count()-3,3);
		error = DoDeleteSelectionWithHeapFailure2(*index, *selection);
		if (error==KErrNone)
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(!index->EntryExists(entry3.Id()));
			test(!index->EntryExists(entry4.Id()));
			test(!index->EntryExists(entry5.Id()));
			test(!index->EntryExists(entry6.Id()));
			test(!index->EntryExists(entry7.Id()));
			test(!index->EntryExists(entry8.Id()));
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
			test(index->EntryExists(entry4.Id()));
			test(index->EntryExists(entry5.Id()));
			test(index->EntryExists(entry6.Id()));
			test(index->EntryExists(entry8.Id()));
			}
		}

	//
	// complex
	entry3.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	entry8.SetParent(entry3.Id());
	REPORT_IF_ERROR(index->AddEntry(entry8, KMsvServerId, ETrue));
	// 
	theFailCount=0;
	error=KErrGeneral;
	while (error)
		{
		selection->Reset();
		selection->AppendL(folder.Id());
		REPORT_IF_ERROR(index->ExpandSelectionRecursively(*selection));
		selection->Delete(selection->Count()-3,3);
		error = DoDeleteSelectionWithHeapFailure2(*index, *selection);
		if (error==KErrNone)
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(!index->EntryExists(entry3.Id()));
			test(!index->EntryExists(entry4.Id()));
			test(!index->EntryExists(entry5.Id()));
			test(!index->EntryExists(entry6.Id()));
			test(!index->EntryExists(entry7.Id()));
			test(!index->EntryExists(entry8.Id()));
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
			test(index->EntryExists(entry4.Id()));
			test(index->EntryExists(entry5.Id()));
			test(index->EntryExists(entry6.Id()));
			test(index->EntryExists(entry8.Id()));
			}
		}

	//
	// complex
	entry3.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
	entry4.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
	entry5.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
	entry6.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
	entry7.SetParent(folder.Id());
	REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
	entry8.SetParent(entry2.Id());
	REPORT_IF_ERROR(index->AddEntry(entry8, KMsvServerId, ETrue));
	// 
	theFailCount=0;
	error=KErrGeneral;
	while (error)
		{
		selection->Reset();
		selection->AppendL(folder.Id());
		REPORT_IF_ERROR(index->ExpandSelectionRecursively(*selection));
		selection->Delete(selection->Count()-3,3);
		error = DoDeleteSelectionWithHeapFailure2(*index, *selection);
		if (error==KErrNone)
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(!index->EntryExists(entry3.Id()));
			test(!index->EntryExists(entry4.Id()));
			test(!index->EntryExists(entry5.Id()));
			test(!index->EntryExists(entry6.Id()));
			test(!index->EntryExists(entry7.Id()));
			test(!index->EntryExists(entry8.Id()));
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
			test(index->EntryExists(entry4.Id()));
			test(index->EntryExists(entry5.Id()));
			test(index->EntryExists(entry6.Id()));
			test(index->EntryExists(entry8.Id()));
			}
		}

	CleanupStack::PopAndDestroy(3);
	}


LOCAL_C void TestSelectionDeletionWithHeapFailure2AL()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;
	TInt error;
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	
	TTestMsvEntry folder;
	folder.iDescription.Set(KShortDescription);
	folder.iDetails.Set(KShortDetails);
	TTestMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	TTestMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KLongDetails);
	TTestMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	TTestMsvEntry entry4;
	entry4.iDescription.Set(KLongDescription);
	entry4.iDetails.Set(KLongDetails);
	TTestMsvEntry entry5;
	entry5.iDescription.Set(KLongDescription);
	entry5.iDetails.Set(KLongDetails);
	TTestMsvEntry entry6;
	entry6.iDescription.Set(KShortDescription);
	entry6.iDetails.Set(KLongDetails);
	TTestMsvEntry entry7;
	entry7.iDescription.Set(KShortDescription);
	entry7.iDetails.Set(KShortDescription);
	TTestMsvEntry entry8;
	entry8.iDescription.Set(KShortDescription);
	entry8.iDetails.Set(KShortDescription);



	TBool finished=EFalse;
	theFailCount=0;
	while (!finished)
		{
		theUtils->FileSession().Delete(KMsvTestFile);
		testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		testStore->SetTypeL(KPermanentFileStoreLayoutUid);
		index = CMsvServerIndex::NewL(*testStore);
		CleanupStack::PushL(index);
		CreateDefaultIndex(*index);

		folder.SetParent(KMsvGlobalInBoxIndexEntryId);
		REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
		entry1.SetParent(folder.Id());
		REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
		entry2.SetParent(folder.Id());
		REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
		entry3.SetParent(entry1.Id());
		REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
		entry4.SetParent(entry3.Id());
		REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
		entry5.SetParent(entry2.Id());
		REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
		entry6.SetParent(entry5.Id());
		REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
		entry7.SetParent(entry6.Id());
		REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
		//
		error = DoDeleteSelectionWithHeapFailure(*index, *selection, folder.Id());
		//
		if (error==KErrNone)
			{
			test(!index->EntryExists(folder.Id()));
			test(!index->EntryExists(entry1.Id()));
			test(!index->EntryExists(entry3.Id()));
			test(!index->EntryExists(entry4.Id()));
			test(!index->EntryExists(entry5.Id()));
			test(!index->EntryExists(entry6.Id()));
			test(!index->EntryExists(entry7.Id()));
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry3.Id()));
			test(index->EntryExists(entry4.Id()));
			test(index->EntryExists(entry5.Id()));
			test(index->EntryExists(entry6.Id()));
			test(index->EntryExists(entry7.Id()));
			}
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		if (error==KErrNone)
			{
			test(!index->EntryExists(folder.Id()));
			test(!index->EntryExists(entry1.Id()));
			test(!index->EntryExists(entry3.Id()));
			test(!index->EntryExists(entry4.Id()));
			test(!index->EntryExists(entry5.Id()));
			test(!index->EntryExists(entry6.Id()));
			test(!index->EntryExists(entry7.Id()));
			finished=ETrue;
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry3.Id()));
			test(index->EntryExists(entry4.Id()));
			test(index->EntryExists(entry5.Id()));
			test(index->EntryExists(entry6.Id()));
			test(index->EntryExists(entry7.Id()));
			}
		CleanupStack::PopAndDestroy(2);
		}

	CleanupStack::PopAndDestroy();
	}


LOCAL_C void TestSelectionDeletionWithHeapFailure2BL()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;
	TInt error;
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	
	TTestMsvEntry folder;
	folder.iDescription.Set(KShortDescription);
	folder.iDetails.Set(KShortDetails);
	TTestMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	TTestMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KLongDetails);
	TTestMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	TTestMsvEntry entry4;
	entry4.iDescription.Set(KLongDescription);
	entry4.iDetails.Set(KLongDetails);
	TTestMsvEntry entry5;
	entry5.iDescription.Set(KLongDescription);
	entry5.iDetails.Set(KLongDetails);
	TTestMsvEntry entry6;
	entry6.iDescription.Set(KShortDescription);
	entry6.iDetails.Set(KLongDetails);
	TTestMsvEntry entry7;
	entry7.iDescription.Set(KShortDescription);
	entry7.iDetails.Set(KShortDescription);
	TTestMsvEntry entry8;
	entry8.iDescription.Set(KShortDescription);
	entry8.iDetails.Set(KShortDescription);


	TBool finished=EFalse;
	theFailCount=0;
	while (!finished)
		{
		theUtils->FileSession().Delete(KMsvTestFile);
		testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		testStore->SetTypeL(KPermanentFileStoreLayoutUid);
		index = CMsvServerIndex::NewL(*testStore);
		CleanupStack::PushL(index);
		CreateDefaultIndex(*index);

		folder.SetParent(KMsvGlobalInBoxIndexEntryId);
		REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
		entry1.SetParent(folder.Id());
		REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
		entry2.SetParent(folder.Id());
		REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
		entry3.SetParent(folder.Id());
		REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
		entry4.SetParent(entry3.Id());
		REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
		entry5.SetParent(entry2.Id());
		REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
		entry6.SetParent(entry5.Id());
		REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
		entry7.SetParent(entry6.Id());
		REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
		//
		selection->Reset();
		selection->AppendL(folder.Id());
		REPORT_IF_ERROR(index->ExpandSelectionRecursively(*selection));
		selection->Delete(selection->Count()-1);
		error = DoDeleteSelectionWithHeapFailure2(*index, *selection);
		//
		if (error==KErrNone)
			{
			test(index->EntryExists(folder.Id()));
			test(!index->EntryExists(entry1.Id()));
			test(!index->EntryExists(entry2.Id()));
			test(!index->EntryExists(entry3.Id()));
			test(!index->EntryExists(entry4.Id()));
			test(!index->EntryExists(entry5.Id()));
			test(!index->EntryExists(entry6.Id()));
			test(!index->EntryExists(entry7.Id()));
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
			test(index->EntryExists(entry4.Id()));
			test(index->EntryExists(entry5.Id()));
			test(index->EntryExists(entry6.Id()));
			test(index->EntryExists(entry7.Id()));
			}
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		if (error==KErrNone)
			{
			test(index->EntryExists(folder.Id()));
			test(!index->EntryExists(entry1.Id()));
			test(!index->EntryExists(entry2.Id()));
			test(!index->EntryExists(entry3.Id()));
			test(!index->EntryExists(entry4.Id()));
			test(!index->EntryExists(entry5.Id()));
			test(!index->EntryExists(entry6.Id()));
			test(!index->EntryExists(entry7.Id()));
			finished=ETrue;
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
			test(index->EntryExists(entry4.Id()));
			test(index->EntryExists(entry5.Id()));
			test(index->EntryExists(entry6.Id()));
			test(index->EntryExists(entry7.Id()));
			}
		CleanupStack::PopAndDestroy(2);
		}

	CleanupStack::PopAndDestroy();
	}


LOCAL_C void TestSelectionDeletionWithHeapFailure2CL()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;
	TInt error;
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	
	TTestMsvEntry folder;
	folder.iDescription.Set(KShortDescription);
	folder.iDetails.Set(KShortDetails);
	TTestMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	TTestMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KLongDetails);
	TTestMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	TTestMsvEntry entry4;
	entry4.iDescription.Set(KLongDescription);
	entry4.iDetails.Set(KLongDetails);
	TTestMsvEntry entry5;
	entry5.iDescription.Set(KLongDescription);
	entry5.iDetails.Set(KLongDetails);
	TTestMsvEntry entry6;
	entry6.iDescription.Set(KShortDescription);
	entry6.iDetails.Set(KLongDetails);
	TTestMsvEntry entry7;
	entry7.iDescription.Set(KShortDescription);
	entry7.iDetails.Set(KShortDescription);
	TTestMsvEntry entry8;
	entry8.iDescription.Set(KShortDescription);
	entry8.iDetails.Set(KShortDescription);


	TBool finished=EFalse;
	theFailCount=0;
	while (!finished)
		{
		theUtils->FileSession().Delete(KMsvTestFile);
		testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		testStore->SetTypeL(KPermanentFileStoreLayoutUid);
		index = CMsvServerIndex::NewL(*testStore);
		CleanupStack::PushL(index);
		CreateDefaultIndex(*index);

		folder.SetParent(KMsvGlobalInBoxIndexEntryId);
		REPORT_IF_ERROR(index->AddEntry(folder, KMsvServerId, ETrue));
		entry1.SetParent(folder.Id());
		REPORT_IF_ERROR(index->AddEntry(entry1, KMsvServerId, ETrue));
		entry2.SetParent(folder.Id());
		REPORT_IF_ERROR(index->AddEntry(entry2, KMsvServerId, ETrue));
		entry3.SetParent(folder.Id());
		REPORT_IF_ERROR(index->AddEntry(entry3, KMsvServerId, ETrue));
		entry4.SetParent(entry3.Id());
		REPORT_IF_ERROR(index->AddEntry(entry4, KMsvServerId, ETrue));
		entry5.SetParent(entry2.Id());
		REPORT_IF_ERROR(index->AddEntry(entry5, KMsvServerId, ETrue));
		entry6.SetParent(entry5.Id());
		REPORT_IF_ERROR(index->AddEntry(entry6, KMsvServerId, ETrue));
		entry7.SetParent(entry6.Id());
		REPORT_IF_ERROR(index->AddEntry(entry7, KMsvServerId, ETrue));
		//
		selection->Reset();
		selection->AppendL(folder.Id());
		REPORT_IF_ERROR(index->ExpandSelectionRecursively(*selection));
		selection->Delete(selection->Count()-2,2);
		error = DoDeleteSelectionWithHeapFailure2(*index, *selection);
		//
		if (error==KErrNone)
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(!index->EntryExists(entry2.Id()));
			test(!index->EntryExists(entry3.Id()));
			test(!index->EntryExists(entry4.Id()));
			test(!index->EntryExists(entry5.Id()));
			test(!index->EntryExists(entry6.Id()));
			test(!index->EntryExists(entry7.Id()));
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
			test(index->EntryExists(entry4.Id()));
			test(index->EntryExists(entry5.Id()));
			test(index->EntryExists(entry6.Id()));
			test(index->EntryExists(entry7.Id()));
			}
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		if (error==KErrNone)
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(!index->EntryExists(entry2.Id()));
			test(!index->EntryExists(entry3.Id()));
			test(!index->EntryExists(entry4.Id()));
			test(!index->EntryExists(entry5.Id()));
			test(!index->EntryExists(entry6.Id()));
			test(!index->EntryExists(entry7.Id()));
			finished=ETrue;
			}
		else
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
			test(index->EntryExists(entry4.Id()));
			test(index->EntryExists(entry5.Id()));
			test(index->EntryExists(entry6.Id()));
			test(index->EntryExists(entry7.Id()));
			}
		CleanupStack::PopAndDestroy(2);
		}

	CleanupStack::PopAndDestroy();
	}

LOCAL_C void doTestsL()
	{
	theUtils->FileSession().Delete(KMsvTestFile);
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	
	DummyStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestDummyFile, EFileWrite|EFileShareAny);
	DummyStore->SetTypeL(KPermanentFileStoreLayoutUid);

	// testing normal behaviours

	CMsgsTestUtils::Start(_L("Creating index"));
	TestCreatingWithHeapFailuresL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Temporary flags"));
	TestTemporaryFlagsWithHeapFailuresL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Adding entries"));
	HeapFailAddingSingleChild(KShortDescription,KShortDetails); test.Printf(_L("."));
	HeapFailAddingSingleChild(KShortDescription,KLongDetails); test.Printf(_L("."));
	HeapFailAddingSingleChild(KLongDescription,KShortDetails); test.Printf(_L("."));
	HeapFailAddingSingleChild(KLongDescription,KLongDetails); test.Printf(_L("."));
	HeapFailAddingSibling(KShortDescription,KShortDetails); test.Printf(_L("."));
	HeapFailAddingSibling(KShortDescription,KLongDetails); test.Printf(_L("."));
	HeapFailAddingSibling(KLongDescription,KShortDetails); test.Printf(_L("."));
	HeapFailAddingSibling(KLongDescription,KLongDetails); test.Printf(_L("\n"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Deleting entries"));
	HeapFailDeletingSingleChild(KShortDescription,KShortDetails); test.Printf(_L("."));
	HeapFailDeletingSingleChild(KShortDescription,KLongDetails); test.Printf(_L("."));
	HeapFailDeletingSingleChild(KLongDescription,KShortDetails); test.Printf(_L("."));
	HeapFailDeletingSingleChild(KLongDescription,KLongDetails); test.Printf(_L("."));
	HeapFailDeletingFinalSibling(KShortDescription,KShortDetails); test.Printf(_L("."));
	HeapFailDeletingFinalSibling(KShortDescription,KLongDetails); test.Printf(_L("."));
	HeapFailDeletingFinalSibling(KLongDescription,KShortDetails); test.Printf(_L("."));
	HeapFailDeletingFinalSibling(KLongDescription,KLongDetails); test.Printf(_L("."));
	HeapFailDeletingFirstSibling(KShortDescription,KShortDetails); test.Printf(_L("."));
	HeapFailDeletingFirstSibling(KShortDescription,KLongDetails); test.Printf(_L("."));
	HeapFailDeletingFirstSibling(KLongDescription,KShortDetails); test.Printf(_L("."));
	HeapFailDeletingFirstSibling(KLongDescription,KLongDetails); test.Printf(_L("."));
	HeapFailDeletingMiddleSibling(KShortDescription,KShortDetails); test.Printf(_L("."));
	HeapFailDeletingMiddleSibling(KShortDescription,KLongDetails); test.Printf(_L("."));
	HeapFailDeletingMiddleSibling(KLongDescription,KShortDetails); test.Printf(_L("."));
	HeapFailDeletingMiddleSibling(KLongDescription,KLongDetails); test.Printf(_L("."));
	HeapFailDeletingLastTwoSiblings(KShortDescription,KShortDetails); test.Printf(_L("."));
	HeapFailDeletingLastTwoSiblingsReversed(KShortDescription,KShortDetails); test.Printf(_L("."));
	HeapFailDeletingFirstTwoSiblings(KShortDescription,KShortDetails); test.Printf(_L("."));
	HeapFailDeletingFirstTwoSiblingsReversed(KShortDescription,KShortDetails); test.Printf(_L("\n"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Changing"));
	HeapFailChangingWithChangingStringsNotMoving(KShortDescription,KShortDetails,KShortDescription, KShortDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsNotMoving(KLongDescription, KLongDetails,KShortDescription,KShortDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsNotMoving(KShortDescription,KShortDetails,KLongDescription, KLongDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsNotMoving(KLongDescription, KLongDetails,KShortDescription,KShortDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsNotMoving(KShortDescription, KLongDetails,KLongDescription,KShortDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsNotMoving(KLongDescription,KShortDetails,KShortDescription, KLongDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsNotMoving(KShortDescription, KLongDetails,KLongDescription,KLongDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsNotMoving(KLongDescription,KShortDetails,KLongDescription, KLongDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsNotMoving(KShortDescription, KLongDetails,KShortDescription,KShortDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsNotMoving(KLongDescription,KShortDetails,KShortDescription, KShortDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsNotMoving(KShortDescription,KShortDetails,KLongDescription, KShortDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsNotMoving(KShortDescription,KShortDetails,KShortDescription, KLongDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsNotMoving(KLongDescription, KLongDetails,KLongDescription,KShortDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsNotMoving(KLongDescription, KLongDetails,KShortDescription,KLongDetails); test.Printf(_L("\n"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Changing & moving"));
	HeapFailChangingWithChangingStringsAndMoving(KShortDescription,KShortDetails,KShortDescription, KShortDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsAndMoving(KShortDescription,KShortDetails,KLongDescription, KLongDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsAndMoving(KLongDescription, KLongDetails,KShortDescription,KShortDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsAndMoving(KShortDescription, KLongDetails,KLongDescription,KShortDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsAndMoving(KLongDescription,KShortDetails,KShortDescription, KLongDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsAndMoving(KShortDescription, KLongDetails,KLongDescription, KLongDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsAndMoving(KLongDescription,KShortDetails,KLongDescription, KLongDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsAndMoving(KShortDescription, KLongDetails,KShortDescription,KShortDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsAndMoving(KLongDescription,KShortDetails,KShortDescription, KShortDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsAndMoving(KShortDescription,KShortDetails,KLongDescription, KShortDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsAndMoving(KShortDescription,KShortDetails,KShortDescription, KLongDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsAndMoving(KLongDescription, KLongDetails,KLongDescription,KShortDetails); test.Printf(_L("."));
	HeapFailChangingWithChangingStringsAndMoving(KLongDescription, KLongDetails,KShortDescription,KLongDetails); test.Printf(_L("\n"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Async opening"));
	TestAsyncOpenWithHeapFailureL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Moving"));
	TestMovingWithHeapFailureL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Changing attributes"));
	TestAttributesWithHeapFailureL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Selection deletion"));
	TestSelectionDeletionWithHeapFailure1L(); test.Printf(_L("."));
	TestSelectionDeletionWithHeapFailure2AL(); test.Printf(_L("."));
	TestSelectionDeletionWithHeapFailure2BL(); test.Printf(_L("."));
	TestSelectionDeletionWithHeapFailure2CL(); test.Printf(_L("\n"));
	CMsgsTestUtils::Complete();


	theUtils->FileSession().Delete(KMsvTestFile);
	theUtils->FileSession().Delete(KMsvTestDummyFile);

	CleanupStack::PopAndDestroy(); // DummyStore
	}

#pragma warning( default : 4710 )
