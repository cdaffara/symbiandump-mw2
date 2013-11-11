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
#include "msvindexadapter.h"
#include "Testsetup.h"

RTest test(_L("MSV Resource File Testrig"));

_LIT(KMsvTestFile, "c:\\Server resource testing");


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


LOCAL_C void TestLocalL()
	{
// test the local folders are created correctly on start up	
	CPermanentFileStore* testStore;
	CMsvServerIndex* index;
	CArrayPtrFlat<const TMsvEntry>* selection=new(ELeave)CArrayPtrFlat<const TMsvEntry>(32);
	CleanupStack::PushL(selection);

	theUtils->FileSession().Delete(KMsvTestFile);
	testStore = CPermanentFileStore::CreateLC(theUtils->FileSession(), KMsvTestFile, EFileWrite|EFileShareAny);
	testStore->SetTypeL(KPermanentFileStoreLayoutUid);
	index = CMsvServerIndex::NewL(*testStore);
	CleanupStack::PushL(index);
	CreateDefaultIndex(*index);
	
	index->GetChildrenL(KMsvRootIndexEntryId, *selection, TMsvSelectionOrdering());
	test(selection->Count()==1);
	selection->Reset();

	index->GetChildrenL(KMsvLocalServiceIndexEntryId, *selection, TMsvSelectionOrdering());
	test(selection->Count()>=2);
	selection->Reset();

	index->GetChildrenL(KMsvGlobalInBoxIndexEntryId, *selection, TMsvSelectionOrdering());
	test(selection->Count()==0);
	selection->Reset();

	index->GetChildrenL(KMsvGlobalOutBoxIndexEntryId, *selection, TMsvSelectionOrdering());
	test(selection->Count()==0);
	selection->Reset();
	
	TMsvEntry* entry=NULL;
	REPORT_IF_ERROR(index->GetEntry(KMsvLocalServiceIndexEntryId, entry));
	test(entry->Parent()==KMsvRootIndexEntryId);
	test(entry->iDescription==_L(""));
	test(entry->iDetails==_L("Local"));
	test(entry->Priority()==EMsvLowPriority);
	test(entry->Complete());
	test(entry->Visible());
	test(entry->ReadOnly());
	test(!entry->Failed() && !entry->Operation());
	test(entry->iType.iUid==KUidMsvServiceEntryValue);

	entry=NULL;
	REPORT_IF_ERROR(index->GetEntry(KMsvGlobalInBoxIndexEntryIdValue, entry));
	test(entry->Parent()==KMsvLocalServiceIndexEntryId);
	test(entry->iDescription==_L(""));
	test(entry->iDetails==_L("Inbox"));
	test(entry->Priority()==EMsvLowPriority);
	test(entry->Complete() && entry->Visible() && entry->ReadOnly());
	test(!entry->Failed() && !entry->Operation());
	test(entry->iType.iUid==KUidMsvFolderEntryValue);
	test(entry->StandardFolder());

	entry=NULL;
	REPORT_IF_ERROR(index->GetEntry(KMsvGlobalOutBoxIndexEntryIdValue, entry));
	test(entry->Parent()==KMsvLocalServiceIndexEntryId);
	test(entry->iDescription==_L(""));
	test(entry->iDetails==_L("Outbox"));
	test(entry->Priority()==EMsvLowPriority);
	test(entry->Complete() && entry->Visible() && entry->ReadOnly());
	test(!entry->Failed() && !entry->Operation());
	test(entry->iType.iUid==KUidMsvFolderEntryValue);
	test(entry->StandardFolder());

	entry=NULL;
	REPORT_IF_ERROR(index->GetEntry(KMsvDeletedEntryFolderEntryId, entry));
	test(entry->Parent()==KMsvLocalServiceIndexEntryId);
	test(entry->Complete() && !entry->Visible() && entry->ReadOnly());
	test(!entry->Failed() && !entry->Operation());
	test(entry->iType.iUid==KUidMsvFolderEntryValue);
	test(entry->StandardFolder());

	CleanupStack::PopAndDestroy(3);
	}

LOCAL_C void doTestsL()
	{
	theUtils->FileSession().MkDirAll(KMsvTestFile);
	theUtils->FileSession().Delete(KMsvTestFile);

	CMsgsTestUtils::Start(_L("Create local folders from resource"));
	TestLocalL();
	CMsgsTestUtils::Complete();
	}
