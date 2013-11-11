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

#include <msvreg.h>
#include <mtsr.h>
#include <msventry.h>
#include <mtclreg.h>
#include "Testsetup.h"

RTest test(_L("Bulk Server Entry Testrig"));

LOCAL_D CMsvServer*	theServer;

TUint serviceEntriesCreated = 0;


_LIT(KMsvTestIndexFile, "\\Server MTM testing\\Index");

#pragma warning (disable : 4238) //warning C4238: nonstandard extension used : class rvalue used as lvalue
	
	
LOCAL_C void DoCheckEntryDescription(CMsvEntrySelection &aSelection, TBool aChangeEntries, CMsvServerEntry &aRootEntry)
	{
	TUint count = aSelection.Count();
	TUint i = 0;
	for (i=0; i < count; i++)
		{
		aRootEntry.SetEntry(aSelection.At(i));
		TMsvEntry theEntry = aRootEntry.Entry();
		if (!aChangeEntries)
			{
			test (theEntry.iDescription == _L("Created Entry"));
			}
		else
			{
			test (theEntry.iDescription == _L("Changed Entry"));
			}	
		}
	}


#define KNumberofBulkEntriesToUse 18
#define KInterruptAfterThisManyEntries 6

LOCAL_C void DoCheckInterruptedEntryDescription(CMsvEntrySelection &aSelection, CMsvServerEntry &aRootEntry)
	{
	TUint count = aSelection.Count();
	TUint i = 0;
	for (i=0; i < count; i++)
		{
		aRootEntry.SetEntry(aSelection.At(i));
		TMsvEntry theEntry = aRootEntry.Entry();
		if (i != KInterruptAfterThisManyEntries)
			{
			test (theEntry.iDescription == _L("Created Entry"));
			}
		else
			{
			test (theEntry.iDescription == _L("Interrupted Entry"));
			}	
		}
	}

	
	
	
const TUid KMsvTestUid1={0x10200000};

LOCAL_C void TestBulkEntryCreation(TUint aEntryCount, TBool aChangeEntries)
	{
	CMsvServerEntry* rootEntry = CMsvServerEntry::NewL(*theServer, KMsvRootIndexEntryId);
	CleanupStack::PushL(rootEntry);
	//Create a service entry for 
	TMsvEntry svcEntry;
	svcEntry.iType = KUidMsvServiceEntry;
	svcEntry.iMtm = KMsvTestUid1;
	svcEntry.SetVisible(ETrue);
	svcEntry.SetReadOnly(EFalse);
	svcEntry.SetUnread(ETrue);
	svcEntry.SetSendingState(KMsvSendStateNotApplicable);
	rootEntry->CreateEntry(svcEntry);
	serviceEntriesCreated++;  	// to keep track of service entries
								// during deletion

	// Set to the service entry
	rootEntry->SetEntry(svcEntry.Id());
	

	//Create a series of entries using the bulk API
	TUint i = 0;
	for (i=0; i < aEntryCount; i++)
		{
		TMsvEntry newEntry;
		newEntry.iType = KUidMsvMessageEntry;
		newEntry.iMtm = KMsvTestUid1;
		newEntry.iServiceId = 1;
		newEntry.SetVisible(ETrue);
		newEntry.SetReadOnly(EFalse);
		newEntry.SetUnread(ETrue);
		newEntry.iDescription.Set(_L("Created Entry"));
		newEntry.SetSendingState(KMsvSendStateNotApplicable);
		rootEntry->CreateEntryBulk(newEntry);
		if (aChangeEntries)
			{
			newEntry.iDescription.Set(_L("Changed Entry"));
			rootEntry->SetEntry(newEntry.Id());
			rootEntry->ChangeEntryBulk(newEntry);
			rootEntry->SetEntry(svcEntry.Id());
			}
		}
	rootEntry->CompleteBulk();	
	
	//Check that the proper number of entries have been created
	CMsvEntrySelection* theSelection = new (ELeave) CMsvEntrySelection;
	rootEntry->GetChildren(*theSelection);
	TUint theFinalCount = theSelection->Count();
	test (theFinalCount == aEntryCount);
	DoCheckEntryDescription(*theSelection, aChangeEntries, *rootEntry);
	delete theSelection;
	CleanupStack::PopAndDestroy(rootEntry);
	}
	
	
LOCAL_C void TestInterruptedBulkEntryCreation()
	{
	CMsvServerEntry* rootEntry = CMsvServerEntry::NewL(*theServer, KMsvRootIndexEntryId);
	CleanupStack::PushL(rootEntry);
	//Create a service entry for 
	TMsvEntry svcEntry;
	svcEntry.iType = KUidMsvServiceEntry;
	svcEntry.iMtm = KMsvTestUid1;
	svcEntry.SetVisible(ETrue);
	svcEntry.SetReadOnly(EFalse);
	svcEntry.SetUnread(ETrue);
	svcEntry.SetSendingState(KMsvSendStateNotApplicable);
	rootEntry->CreateEntry(svcEntry);
	serviceEntriesCreated++;  	// to keep track of service entries
								// during deletion


	// Set to the service entry
	rootEntry->SetEntry(svcEntry.Id());
	

	//Create a series of KNumberofBulkEntriesToUse entries using the bulk API,
	// interrupting for a "normal" creation after KInterruptAfterThisManyEntries entries
	TUint i = 0;
	
	// Create the first KInterruptAfterThisManyEntries bulk entries
	for (i=0; i < KInterruptAfterThisManyEntries; i++)
		{
		TMsvEntry newEntry;
		newEntry.iType = KUidMsvMessageEntry;
		newEntry.iMtm = KMsvTestUid1;
		newEntry.iServiceId = 1;
		newEntry.SetVisible(ETrue);
		newEntry.SetReadOnly(EFalse);
		newEntry.SetUnread(ETrue);
		newEntry.iDescription.Set(_L("Created Entry"));
		newEntry.SetSendingState(KMsvSendStateNotApplicable);
		rootEntry->CreateEntryBulk(newEntry);
		}
		
	// Interrupt the bulk operation with a standard entry creation
	TMsvEntry normEntry;
	normEntry.iType = KUidMsvMessageEntry;
	normEntry.iMtm = KMsvTestUid1;
	normEntry.iServiceId = 1;
	normEntry.SetVisible(ETrue);
	normEntry.SetReadOnly(EFalse);
	normEntry.SetUnread(ETrue);
	normEntry.iDescription.Set(_L("Interrupted Entry"));
	normEntry.SetSendingState(KMsvSendStateNotApplicable);
	rootEntry->CreateEntry(normEntry);
		
	// 	Create the final 12 entries
	TUint remainingCount = KNumberofBulkEntriesToUse - KInterruptAfterThisManyEntries;
	for (i=0; i < remainingCount; i++)
		{
		TMsvEntry newEntry;
		newEntry.iType = KUidMsvMessageEntry;
		newEntry.iMtm = KMsvTestUid1;
		newEntry.iServiceId = 1;
		newEntry.SetVisible(ETrue);
		newEntry.SetReadOnly(EFalse);
		newEntry.SetUnread(ETrue);
		newEntry.iDescription.Set(_L("Created Entry"));
		newEntry.SetSendingState(KMsvSendStateNotApplicable);
		rootEntry->CreateEntryBulk(newEntry);
		}
		
	rootEntry->CompleteBulk();	
	
	//Check that the proper number of entries have been created
	CMsvEntrySelection* theSelection = new (ELeave) CMsvEntrySelection;
	rootEntry->GetChildren(*theSelection);
	TUint theFinalCount = theSelection->Count();
	test (theFinalCount == (KNumberofBulkEntriesToUse + 1));
	DoCheckInterruptedEntryDescription(*theSelection, *rootEntry);
	delete theSelection;
	CleanupStack::PopAndDestroy(rootEntry);
	}
		
	
LOCAL_C void TestRemovingAllFilesL()
	{
	CMsvServerEntry* rootEntry = CMsvServerEntry::NewL(*theServer, KMsvRootIndexEntryId);
	CleanupStack::PushL(rootEntry);

	// find all the service entries we created
	CMsvEntrySelection* childSel = new(ELeave)CMsvEntrySelection;
	CMsvEntrySelection* grandchildSel = new(ELeave)CMsvEntrySelection;

	rootEntry->GetChildren(*childSel);
	TUint childCount1 = childSel->Count();
	TUint childCount2 = childCount1;
	test(childCount1 == serviceEntriesCreated + 1);

	while (childCount1--)
		{
		// set to the the child, check for and delete
		// grandchildren if service entry
		TMsvEntry* child;
		rootEntry->GetEntryFromId(childSel->At(childCount1), child); // ignore return code
		if (child->iType == KUidMsvServiceEntry)
			{
			grandchildSel->Reset();
			rootEntry->SetEntry(childSel->At(childCount1));
			rootEntry->GetChildren(*grandchildSel);
			test (grandchildSel->Count() > 0);
			rootEntry->DeleteEntries(*grandchildSel);
			}
		}
		
	// go back to the root and delete the services
	rootEntry->SetEntry(KMsvRootIndexEntryId);
	childSel->Reset();
	rootEntry->GetChildren(*childSel);
	// all root children should still be here
	test(childCount2 == static_cast<TUint>(childSel->Count()));
	
	rootEntry->DeleteEntries(*childSel);
	
	// now test to see that it's empty
	childSel->Reset();
	rootEntry->GetChildren(*childSel);
	test(childSel->Count() == 1);
	
	CleanupStack::PopAndDestroy(rootEntry);
	delete childSel;
	delete grandchildSel;
	
	}


LOCAL_C void doTestsL()
    {
	theUtils->FileSession().MkDirAll(KMsvTestIndexFile);
	theUtils->FileSession().Delete(KMsvTestIndexFile);

	theServer = CMsgsTestUtils::CreateServerL();
	CleanupStack::PushL(theServer);
	
	// Test Bulk Entry creation for various numbers including "edge cases"
	
	// Normal Bulk Creation - 100 entries
	CMsgsTestUtils::Start(_L("Bulk Entry Creation - 100 items"));
	TestBulkEntryCreation(100, EFalse);
	CMsgsTestUtils::Complete();
	
	//Edge Case 1: (Number of Entries = Interval - 1)
	CMsgsTestUtils::Next(_L("Bulk Entry Creation (Edge Case 1)- CommitInterval -1 items"));
	TestBulkEntryCreation(KBulkCommitInterval - 1, EFalse);
	CMsgsTestUtils::Complete();
	
	//Edge Case 2: (Number of Entries = Interval)
	CMsgsTestUtils::Next(_L("Bulk Entry Creation (Edge Case 2)- CommitInterval items"));
	TestBulkEntryCreation(KBulkCommitInterval, EFalse);
	CMsgsTestUtils::Complete();

	//Edge Case 3: (Number of Entries = Interval + 1)
	CMsgsTestUtils::Next(_L("Bulk Entry Creation (Edge Case 3)- CommitInterval + 1 items"));
	TestBulkEntryCreation(KBulkCommitInterval + 1, EFalse);
	CMsgsTestUtils::Complete();

	//Edge Case 4: (Number of Entries = 2*Interval)
	CMsgsTestUtils::Next(_L("Bulk Entry Creation (Edge Case 4)- 2*CommitInterval items"));
	TestBulkEntryCreation(2*KBulkCommitInterval, EFalse);
	CMsgsTestUtils::Complete();
	
	//Edge Case 5: (Number of Entries = 2*Interval + 1)
	CMsgsTestUtils::Next(_L("Bulk Entry Creation (Edge Case 5)- 2*CommitInterval + 1 items"));
	TestBulkEntryCreation(2*KBulkCommitInterval + 1, EFalse);
	CMsgsTestUtils::Complete();
	
	//Edge Case 6: (Number of Entries = 1)
	CMsgsTestUtils::Next(_L("Bulk Entry Creation (Edge Case 6)- 1 item"));
	TestBulkEntryCreation(1, EFalse);
	CMsgsTestUtils::Complete();
	
	// Test Bulk Entry Creation & Modification
	CMsgsTestUtils::Next(_L("Bulk Entry Creation & Modification- 100 items"));
	TestBulkEntryCreation(100, ETrue);
	CMsgsTestUtils::Complete();
	
	// Test interrupted bulk operation
	CMsgsTestUtils::Next(_L("Bulk Entry Creation with Interruption"));
	TestInterruptedBulkEntryCreation();
	CMsgsTestUtils::Complete();
	
	// Clean up after the tests
	CMsgsTestUtils::Next(_L("Removing all entries"));
	test.Printf(_L("...and checking for orphan files\n"));
	TestRemovingAllFilesL();
	CMsgsTestUtils::Complete();



	CleanupStack::PopAndDestroy(); // theServer
    }

#pragma warning (default : 4238)
