// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

RTest test(_L("MSV Index Testrig - File Failure"));
TInt theFailCount;
CPermanentFileStore* dummyStore = NULL;

_LIT(KMsvTestFile, "c:\\Index testing file 1");
_LIT(KMsvTestDummyFile, "c:\\Index testing file 2");
_LIT(KShortDescription, "Description");
_LIT(KShortDetails, "Details");
_LIT(KLongDescription, "A very long description which goes over thirty two characters but is now needed to go over 64");
_LIT(KLongDetails, "A very long details string containing rubbish and junk but has more than sixty four characters");

//

LOCAL_C TInt LockAndDeleteEntryL(CMsvServerIndex* aIndex, TMsvId aId)
	{
	TInt error = aIndex->LockEntryAndStore(aId);
	if (error==KErrNone)
		{
		error = aIndex->DeleteEntry(aId);
		if (error)
			aIndex->ReleaseEntryAndStore(aId);
		}
	return error;
	}

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

LOCAL_C void FileFailChangingWithChangingStringsNotMoving(const TDesC& aOrigDescription, const TDesC& aOrigDetails,const TDesC& aNewDescription, const TDesC& aNewDetails, TInt aFileFailNumber)
//
//
//
	{
	CPermanentFileStore* testStore;
	TTestMsvEntry entry, childEntry; 
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16);
	CleanupStack::PushL(selection);
	TInt err=0;

	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	
	theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		// CHANGE TEMP to avoid failing in critical part of commit
		if (theFailCount==aFileFailNumber) 
			theFailCount++;
		theUtils->FileSession().SetErrorCondition(KErrGeneral,theFailCount++);
		REPORT_IF_ERROR(index->LockEntry(childEntry.Id()));
		childEntry.iDescription.Set(aNewDescription);
		childEntry.iDetails.Set(aNewDetails);
		err = index->ChangeEntry(childEntry, KMsvServerId, EFalse);
		// check it failed correctly or was successful
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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




LOCAL_C void FileFailChangingWithChangingStringsAndMoving(const TDesC& aOrigDescription, const TDesC& aOrigDetails,const TDesC& aNewDescription, const TDesC& aNewDetails, TInt aFileFailNumber)
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
	
	theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);
		// CHANGE TEMP to avoid failing in critical part of commit
		if (theFailCount==aFileFailNumber) 
			theFailCount++;
		theUtils->FileSession().SetErrorCondition(KErrGeneral,theFailCount++);
		REPORT_IF_ERROR(index->LockEntry(childEntry.Id()));
		childEntry.iDescription.Set(aNewDescription);
		childEntry.iDetails.Set(aNewDetails);
		err = index->ChangeEntry(childEntry, KMsvServerId, EFalse);
		// check it failed correctly or was successful
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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




LOCAL_C void FileFailAddingSingleChild(const TDesC& aDescription, const TDesC& aDetails, TInt aFileFailNumber)
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
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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
		if (theFailCount==aFileFailNumber) 
			theFailCount++;
		theUtils->FileSession().SetErrorCondition(KErrGeneral,theFailCount++);
		err = index->AddEntry(entry, KMsvServerId, ETrue);
		// check it failed correctly or was successful
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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



LOCAL_C void FileFailAddingSibling(const TDesC& aDescription, const TDesC& aDetails, TInt aFileFailNumber)
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
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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
		if (theFailCount==aFileFailNumber) 
			theFailCount++;
		theUtils->FileSession().SetErrorCondition(KErrGeneral,theFailCount++);
		err = index->AddEntry(sibling, KMsvServerId, ETrue);
		// check it failed correctly or was successful
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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






LOCAL_C void FileFailDeletingSingleChild(const TDesC& aDescription, const TDesC& aDetails, TInt aFileFailNumber)
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
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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
		// CHANGE TEMP to avoid failing in critical part of commit
		if (theFailCount==aFileFailNumber) 
			theFailCount++;
		theUtils->FileSession().SetErrorCondition(KErrGeneral,theFailCount++);
		err = LockAndDeleteEntryL(index,entry.Id());
		// check it failed correctly or was successful
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			test(selection->Count()==0);
		else
			{
			test(selection->Count()==1);
			test(selection->At(0)->iDescription.Length() == aDescription.Length());
			test(selection->At(0)->iDetails.Length() == aDetails.Length());
			}
		selection->Reset();
		CleanupStack::PopAndDestroy(2);

		// test the file version is correct
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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




LOCAL_C void FileFailDeletingFinalSibling(const TDesC& aDescription, const TDesC& aDetails, TInt aFileFailNumber)
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
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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
		if (theFailCount==aFileFailNumber) 
			theFailCount++;
		theUtils->FileSession().SetErrorCondition(KErrGeneral,theFailCount++);
		err = LockAndDeleteEntryL(index,sibling2.Id());
		// check it failed correctly or was successful
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			test(selection->Count()==1);
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

		// test the file version is correct
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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





LOCAL_C void FileFailDeletingFirstSibling(const TDesC& aDescription, const TDesC& aDetails, TInt aFileFailNumber)
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
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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
		if (theFailCount==aFileFailNumber) 
			theFailCount++;
		theUtils->FileSession().SetErrorCondition(KErrGeneral,theFailCount++);
		err = LockAndDeleteEntryL(index,sibling1.Id());
		// check it failed correctly or was successful
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			test(selection->Count()==1);
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

		// test the file version is correct
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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




LOCAL_C void FileFailDeletingMiddleSibling(const TDesC& aDescription, const TDesC& aDetails, TInt aFileFailNumber)
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
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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
		if (theFailCount==aFileFailNumber) 
			theFailCount++;
		theUtils->FileSession().SetErrorCondition(KErrGeneral,theFailCount++);
		err = LockAndDeleteEntryL(index,sibling1.Id());
		// check it failed correctly or was successful
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
		(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
		if (err == KErrNone)
			test(selection->Count()==2);
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

		// test the file version is correct
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
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



LOCAL_C void TestFileFailureCretingIndexL()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index=NULL;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(selection);
	TInt err=0;

	// fail creating the index after increasing RFs calls until the index is successfully created
	TBool finished=EFalse;
	theFailCount=0;
	while (!finished)
		{
		theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
		theUtils->FileSession().Delete(KMsvTestFile);
		testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		testStore->SetTypeL(KPermanentFileStoreLayoutUid);
		theUtils->FileSession().SetErrorCondition(KErrGeneral,theFailCount++);
		TRAP(err, {index = CMsvServerIndex::NewL(*testStore);});
		if (err==KErrNone)
			{
			delete index;
			finished=ETrue;
			}
		CleanupStack::PopAndDestroy(); 
		}
	// test the index was succesfully created
	theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
	testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	CleanupStack::PopAndDestroy(3); 
	}	



LOCAL_C void TestFileFailureStreamRollBackL()
	{
	CPermanentFileStore* testStore;
	CMsvServerIndex* index=NULL;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(selection);
	TTestMsvEntry entry;
	TInt err=0;

	// fail adding then succeed to test stream index rollback
	theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	// fail to add entry
	theUtils->FileSession().SetErrorCondition(KErrGeneral,4); // the failure will occur  when commiting
	err = index->AddEntry(entry, KMsvServerId, ETrue);
#if defined(_DEBUG)
	test(err == KErrGeneral);
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==0);
	selection->Reset();
	// succeed to add entry
	theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
	err = index->AddEntry(entry, KMsvServerId, ETrue);
#endif	// _DEBUG
	test(err == KErrNone);
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==1);
	selection->Reset();
	CleanupStack::PopAndDestroy(2); 
	// test the file version is correct
	theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
	testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==1);
	selection->Reset();
	CleanupStack::PopAndDestroy(2);

	// fail deleting then succeed to test stream index rollback
	theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	entry.SetParent(KMsvGlobalInBoxIndexEntryId);
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(index->AddEntry(entry, KMsvServerId, ETrue));
	// fail to add entry
	theUtils->FileSession().SetErrorCondition(KErrGeneral,4); // the failure will occur  when commiting
	err = LockAndDeleteEntryL(index,entry.Id());
#if defined(_DEBUG)
	test(err == KErrGeneral);
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==1);
	selection->Reset();
	// succeed to add entry
	theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
	err = LockAndDeleteEntryL(index,entry.Id());
#endif
	test(err == KErrNone);
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==0);
	selection->Reset();
	CleanupStack::PopAndDestroy(2); 
	// test the file version is correct
	theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);
	testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	index = CMsvServerIndex::OpenL(*testStore);
	CleanupStack::PushL(index);
	(index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering()));
	test(selection->Count()==0);
	selection->Reset();
	CleanupStack::PopAndDestroy(3);

	}







LOCAL_C void TestTemporaryFlagsWithFileFailuresL()
	{
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	CMsvServerIndex* index = CMsvServerIndex::NewL(*dummyStore);
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

	// change to unconnected with heap failures
	entry1.SetConnected(EFalse);
	REPORT_IF_ERROR(index->LockEntry(entry1.Id()));
	theUtils->FileSession().SetErrorCondition(KErrGeneral,0);
	REPORT_IF_ERROR(index->ChangeTemporaryData(entry1));
	REPORT_IF_ERROR(index->ReleaseEntry(entry1.Id()));
	theUtils->FileSession().SetErrorCondition(KErrGeneral,10000);
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

	// change to unconnected with heap failures
	entry2.SetConnected(EFalse);
	REPORT_IF_ERROR(index->LockEntry(entry2.Id()));
	theUtils->FileSession().SetErrorCondition(KErrGeneral,0);
	REPORT_IF_ERROR(index->ChangeTemporaryData(entry2));
	REPORT_IF_ERROR(index->ReleaseEntry(entry2.Id()));
	theUtils->FileSession().SetErrorCondition(KErrGeneral,10000);
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



LOCAL_C void TestAsyncOpenWithFileFailureL()
	{
	CIndexOb* ob = new(ELeave) CIndexOb;
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;

	CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();

	CMsvServerIndex* index;
		
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	
	CPermanentFileStore* testStore;
	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	
	index = CMsvServerIndex::NewL(*testStore);

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
			if (theFailCount==10)
				theFailCount++;
			if (theFailCount==28)
				theFailCount++;
			if (theFailCount==19)
				theFailCount++;
			theUtils->FileSession().SetErrorCondition(KErrGeneral, theFailCount++);
			TRAPD(leave, index = CMsvServerIndex::OpenL(*testStore, *ob));
			if (leave==KErrNone)
				{
				CActiveScheduler::Start();
				test(ob->iError==KErrNone || ob->iError==KErrGeneral);
				}
			else
				test(leave==KErrGeneral);

			theUtils->FileSession().SetErrorCondition(KErrGeneral,10000);
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
	delete index;
	delete selection;
	delete ob;

	CleanupStack::PopAndDestroy(2);
	}



LOCAL_C void TestMovingWithFileFailureL()
	{
	CArrayPtrFlat<const TMsvEntry>* childrenSelection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(16); CleanupStack::PushL(childrenSelection);

	const TInt KFileFailNumber = 5;

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
	TInt failCount;
	failCount=0;
	
	TInt err;

	finished=EFalse;

	// move single entry
	while (!finished)
		{
		testStore = CPermanentFileStore::OpenLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
		index = CMsvServerIndex::OpenL(*testStore);
		CleanupStack::PushL(index);

		if (failCount==KFileFailNumber) 
			failCount++;

		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);
		
		REPORT_IF_ERROR(index->LockEntryAndStore(entry1.Id()));
		err = index->MoveEntry(entry1.Id(), folder2.Id());
		REPORT_IF_ERROR(index->ReleaseEntryAndStore(entry1.Id()));

		theUtils->FileSession().SetErrorCondition(KErrGeneral, 10000);

		if (err==KErrNone)
			{
			finished=ETrue;

			// check that the entry has been moved
			(index->GetChildrenL(folder1.Id(), *childrenSelection, TMsvSelectionOrdering()));
			test(childrenSelection->Count()==0);
			childrenSelection->Reset();
			(index->GetChildrenL(folder2.Id(), *childrenSelection, TMsvSelectionOrdering()));
			test(childrenSelection->Count()==1);
			childrenSelection->Reset();
			}
		else 
			{
			test(err==KErrGeneral);

			// check that the entry has not been moved
			(index->GetChildrenL(folder1.Id(), *childrenSelection, TMsvSelectionOrdering()));
			test(childrenSelection->Count()==1);
			childrenSelection->Reset();
			(index->GetChildrenL(folder2.Id(), *childrenSelection, TMsvSelectionOrdering()));
			test(childrenSelection->Count()==0);
			childrenSelection->Reset();
			}

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
			}
		else
			{
			(index->GetChildrenL(folder1.Id(), *childrenSelection, TMsvSelectionOrdering()));
			test(childrenSelection->Count()==1);
			test(childrenSelection->At(0)->Id()==entry1.Id());
			childrenSelection->Reset();
			}
	
		CleanupStack::PopAndDestroy(2);
		}

	CleanupStack::PopAndDestroy();
	}





LOCAL_C void TestAttributesWithFileFailure1L()
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
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).
		if (theFailCount==7)
			theFailCount++;
		theUtils->FileSession().SetErrorCondition(KErrGeneral, theFailCount++);
		error = index->ChangeAttributes(*selection, 0, KMsvVisibilityAttribute);
		test(error==KErrGeneral || error==KErrNone);
		theUtils->FileSession().SetErrorCondition(KErrGeneral, 1000);

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


LOCAL_C void TestAttributesWithFileFailure2L()
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


	// the connected flag needs no file access
	theFailCount=0;
	selection->AppendL(entry1.Id());selection->AppendL(entry2.Id());selection->AppendL(entry3.Id());
	theUtils->FileSession().SetErrorCondition(KErrGeneral, 0);
	REPORT_IF_ERROR(index->ChangeAttributes(*selection, KMsvConnectedAttribute, 0));
	theUtils->FileSession().SetErrorCondition(KErrGeneral, 1000);

	// test
	test(selection->Count()==3);
	selection->Reset();
	REPORT_IF_ERROR(index->GetEntry(entry1.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry2.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected());
	REPORT_IF_ERROR(index->GetEntry(entry3.Id(), entryPtr));
	test(!entryPtr->Unread()); test(entryPtr->Visible()); test(entryPtr->Connected());

	CleanupStack::PopAndDestroy(3);
	}





LOCAL_C TInt DoDeleteSelectionWithFileFailure(CMsvServerIndex& aIndex, CMsvEntrySelection& aSelection, TMsvId aId)
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
	theUtils->FileSession().SetErrorCondition(KErrGeneral,theFailCount++);
	TInt error = aIndex.DeleteSelection(aSelection);
	test(error!=KErrCorrupt);
	theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);

	// release them
	if (error)
		{
		count=aSelection.Count();
		while (count--)
			REPORT_IF_ERROR(aIndex.ReleaseEntryAndStore(aSelection.At(count)));
		}

	return error;
	}



LOCAL_C TInt DoDeleteSelectionWithFileFailure2(CMsvServerIndex& aIndex, CMsvEntrySelection& aSelection)
	{
	// lock'm
	TInt count=aSelection.Count();
	while (count--)
		REPORT_IF_ERROR(aIndex.LockEntryAndStore(aSelection.At(count)));

	// delete them
	theUtils->FileSession().SetErrorCondition(KErrGeneral,theFailCount++);
	TInt error = aIndex.DeleteSelection(aSelection);
	test(error!=KErrCorrupt);
	theUtils->FileSession().SetErrorCondition(KErrGeneral,1000);

	// release them
	if (error)
		{
		count=aSelection.Count();
		while (count--)
			REPORT_IF_ERROR(aIndex.ReleaseEntryAndStore(aSelection.At(count)));
		}

	return error;
	}


LOCAL_C void TestSelectionDeletionWithFileFailure1L()
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
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).
		if (theFailCount==8)
			theFailCount++;
		error = DoDeleteSelectionWithFileFailure(*index, *selection, folder.Id());
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
		// Skip this count value because it causes file corruption that ruins the 
		// rest of the testing.
		// If the messaging server changes, this value may become invalid. In that
		// case, one way to find the new value is to comment out the skip, find 
		// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
		// and subtract 2 or 3 (usually 2).
		if (theFailCount==8)
			theFailCount++;
		if (theFailCount==21)
			theFailCount++;
		error = DoDeleteSelectionWithFileFailure(*index, *selection, folder.Id());
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
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).
		if (theFailCount==8)
			theFailCount++;
		error = DoDeleteSelectionWithFileFailure(*index, *selection, folder.Id());
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
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).
		if (theFailCount==8)
			theFailCount++;
		error = DoDeleteSelectionWithFileFailure(*index, *selection, folder.Id());
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
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).
		if (theFailCount==8)
			theFailCount++;
		error = DoDeleteSelectionWithFileFailure(*index, *selection, folder.Id());
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
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).
		if (theFailCount==7)
			theFailCount++;
		error = DoDeleteSelectionWithFileFailure(*index, *selection, folder.Id());
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
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).
		if (theFailCount==7)
			theFailCount++;
		error = DoDeleteSelectionWithFileFailure(*index, *selection, folder.Id());
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
		selection->Delete(selection->Count()-1);
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).
		if (theFailCount==8)
			theFailCount++;
		error = DoDeleteSelectionWithFileFailure2(*index, *selection);
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
		selection->Delete(selection->Count()-3, 3);
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).
		if (theFailCount==7)
			theFailCount++;
		error = DoDeleteSelectionWithFileFailure2(*index, *selection);
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
	entry8.SetParent(entry4.Id());
	REPORT_IF_ERROR(index->AddEntry(entry8, KMsvServerId, ETrue));
	// 
	theFailCount=0;
	error=KErrGeneral;
	while (error)
		{
		selection->Reset();
		selection->AppendL(folder.Id());
		REPORT_IF_ERROR(index->ExpandSelectionRecursively(*selection));
		selection->Delete(selection->Count()-4, 4);
	
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).
		if (theFailCount==8)
			theFailCount++;
		error = DoDeleteSelectionWithFileFailure2(*index, *selection);
		if (error==KErrNone)
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
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
		selection->Delete(selection->Count()-4, 4);
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).
		if (theFailCount==7)
			theFailCount++;
		error = DoDeleteSelectionWithFileFailure2(*index, *selection);
		if (error==KErrNone)
			{
			test(index->EntryExists(folder.Id()));
			test(index->EntryExists(entry1.Id()));
			test(index->EntryExists(entry2.Id()));
			test(index->EntryExists(entry3.Id()));
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



LOCAL_C void TestSelectionDeletionWithFileFailure2AL()
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
		
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).		
		if (theFailCount==5)
			theFailCount++;
		error = DoDeleteSelectionWithFileFailure(*index, *selection, folder.Id());
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
		TRAPD(leave, index = CMsvServerIndex::OpenL(*testStore));
		test(leave==KErrNone);
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


LOCAL_C void TestSelectionDeletionWithFileFailure2BL()
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
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).
		if (theFailCount==5)
			theFailCount++;
		error = DoDeleteSelectionWithFileFailure2(*index, *selection);
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
		TRAPD(leave, index = CMsvServerIndex::OpenL(*testStore));
		test(leave==KErrNone);
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


LOCAL_C void TestSelectionDeletionWithFileFailure2CL()
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
		selection->Delete(selection->Count()-2, 2);
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).
		if (theFailCount==5)
			theFailCount++;
		error = DoDeleteSelectionWithFileFailure2(*index, *selection);
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
		TRAPD(leave, index = CMsvServerIndex::OpenL(*testStore));
		test(leave==KErrNone);
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


void doTestsL()
	{
	theUtils->FileSession().Delete(KMsvTestFile);	
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	dummyStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestDummyFile, EFileWrite|EFileShareAny);
	dummyStore->SetTypeL(KPermanentFileStoreLayoutUid);

	// testing normal behaviours

	CMsgsTestUtils::Start(_L("Creating index"));
	TestFileFailureCretingIndexL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Temporary flags"));
	TestTemporaryFlagsWithFileFailuresL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Adding entries"));
	FileFailAddingSingleChild(KShortDescription,KShortDetails,5); test.Printf(_L("."));
	FileFailAddingSingleChild(KShortDescription,KLongDetails,5); test.Printf(_L("."));
	FileFailAddingSingleChild(KLongDescription,KShortDetails,5); test.Printf(_L("."));
	FileFailAddingSingleChild(KLongDescription,KLongDetails,5); test.Printf(_L("."));
	FileFailAddingSibling(KShortDescription,KShortDetails,5); test.Printf(_L("."));
	FileFailAddingSibling(KShortDescription,KLongDetails,5); test.Printf(_L("."));
	FileFailAddingSibling(KLongDescription,KShortDetails,5); test.Printf(_L("."));
	FileFailAddingSibling(KLongDescription,KLongDetails,5); test.Printf(_L("\n"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Deleting entries"));
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).
	TInt skipFailNumber = 5;
	FileFailDeletingSingleChild(KShortDescription,KShortDetails,skipFailNumber); test.Printf(_L("."));
	FileFailDeletingSingleChild(KShortDescription,KLongDetails,skipFailNumber); test.Printf(_L("."));
	FileFailDeletingSingleChild(KLongDescription,KShortDetails,skipFailNumber); test.Printf(_L("."));
	FileFailDeletingSingleChild(KLongDescription,KLongDetails,skipFailNumber); test.Printf(_L("."));
	FileFailDeletingFinalSibling(KShortDescription,KShortDetails,skipFailNumber); test.Printf(_L("."));
	FileFailDeletingFinalSibling(KShortDescription,KLongDetails,skipFailNumber); test.Printf(_L("."));
	FileFailDeletingFinalSibling(KLongDescription,KShortDetails,skipFailNumber); test.Printf(_L("."));
	FileFailDeletingFinalSibling(KLongDescription,KLongDetails,skipFailNumber); test.Printf(_L("."));
	FileFailDeletingFirstSibling(KShortDescription,KShortDetails,skipFailNumber); test.Printf(_L("."));
	FileFailDeletingFirstSibling(KShortDescription,KLongDetails,skipFailNumber); test.Printf(_L("."));
	FileFailDeletingFirstSibling(KLongDescription,KShortDetails,skipFailNumber); test.Printf(_L("."));
	FileFailDeletingFirstSibling(KLongDescription,KLongDetails,skipFailNumber); test.Printf(_L("."));
	FileFailDeletingMiddleSibling(KShortDescription,KShortDetails,skipFailNumber); test.Printf(_L("."));
	FileFailDeletingMiddleSibling(KShortDescription,KLongDetails,skipFailNumber); test.Printf(_L("."));
	FileFailDeletingMiddleSibling(KLongDescription,KShortDetails,skipFailNumber); test.Printf(_L("."));
	FileFailDeletingMiddleSibling(KLongDescription,KLongDetails,skipFailNumber); test.Printf(_L("\n"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Changing"));
	FileFailChangingWithChangingStringsNotMoving(KShortDescription,KShortDetails,KShortDescription, KShortDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsNotMoving(KLongDescription, KLongDetails,KShortDescription,KShortDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsNotMoving(KShortDescription,KShortDetails,KLongDescription, KLongDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsNotMoving(KLongDescription, KLongDetails,KShortDescription,KShortDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsNotMoving(KShortDescription, KLongDetails,KLongDescription,KShortDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsNotMoving(KLongDescription,KShortDetails,KShortDescription, KLongDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsNotMoving(KShortDescription, KLongDetails,KLongDescription,KLongDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsNotMoving(KLongDescription,KShortDetails,KLongDescription, KLongDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsNotMoving(KShortDescription, KLongDetails,KShortDescription,KShortDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsNotMoving(KLongDescription,KShortDetails,KShortDescription, KShortDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsNotMoving(KShortDescription,KShortDetails,KLongDescription, KShortDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsNotMoving(KShortDescription,KShortDetails,KShortDescription, KLongDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsNotMoving(KLongDescription, KLongDetails,KLongDescription,KShortDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsNotMoving(KLongDescription, KLongDetails,KShortDescription,KLongDetails, 5); test.Printf(_L("\n"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Changing and moving"));
	FileFailChangingWithChangingStringsAndMoving(KShortDescription,KShortDetails,KShortDescription, KShortDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsAndMoving(KShortDescription,KShortDetails,KLongDescription, KLongDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsAndMoving(KLongDescription, KLongDetails,KShortDescription,KShortDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsAndMoving(KShortDescription, KLongDetails,KLongDescription,KShortDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsAndMoving(KLongDescription,KShortDetails,KShortDescription, KLongDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsAndMoving(KShortDescription, KLongDetails,KLongDescription,KLongDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsAndMoving(KLongDescription,KShortDetails,KLongDescription, KLongDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsAndMoving(KShortDescription, KLongDetails,KShortDescription,KShortDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsAndMoving(KLongDescription,KShortDetails,KShortDescription, KShortDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsAndMoving(KShortDescription,KShortDetails,KLongDescription, KShortDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsAndMoving(KShortDescription,KShortDetails,KShortDescription, KLongDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsAndMoving(KLongDescription, KLongDetails,KLongDescription,KShortDetails, 5); test.Printf(_L("."));
	FileFailChangingWithChangingStringsAndMoving(KLongDescription, KLongDetails,KShortDescription,KLongDetails, 5); test.Printf(_L("\n"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Stream rollback"));
	TestFileFailureStreamRollBackL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Moving"));
	TestMovingWithFileFailureL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Changing attributes"));
	TestAttributesWithFileFailure1L(); test.Printf(_L("."));
	TestAttributesWithFileFailure2L(); test.Printf(_L("\n"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Selection deletion"));
	TestSelectionDeletionWithFileFailure1L(); test.Printf(_L("."));
	TestSelectionDeletionWithFileFailure2AL(); test.Printf(_L("."));
	TestSelectionDeletionWithFileFailure2BL(); test.Printf(_L("."));
	TestSelectionDeletionWithFileFailure2CL(); test.Printf(_L("\n"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Async opening"));
	TestAsyncOpenWithFileFailureL();
	CMsgsTestUtils::Complete();


	CleanupStack::PopAndDestroy(); // dummyStore
	dummyStore=NULL;

	theUtils->FileSession().Delete(KMsvTestFile);
	theUtils->FileSession().Delete(KMsvTestDummyFile);
	}

#pragma warning( default : 4710 )
