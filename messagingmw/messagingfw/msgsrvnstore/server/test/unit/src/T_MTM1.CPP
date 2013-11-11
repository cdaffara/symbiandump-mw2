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

#include <msvstd.h>
#include <msvids.h>

#include "msvindexadapter.h"
#include <msventry.h>
#include "MSERVER.H"
#include "msvutils.h"
#include "Testsetup.h"

RTest test(_L("Server Entry Heap/File Failure Testrig"));
CPermanentFileStore* dummyStore = NULL;
CMsvServer* theServer;

_LIT(KMsvTestIndexFile, "c:\\Server MTM testing\\Index");
_LIT(KMsvTestFolder, "c:\\Server MTM testing\\");
_LIT(KShortDescription, "Description");
_LIT(KLongDescription, "A very long description which goes over thirty two characters but is now needed to go over 64");
_LIT(KLongDetails, "A very long details string containing rubbish and junk but has more than sixty four characters");

const TUid KMsvTestUid1={0x10200000};
const TUid KMsvTestUid2={0x10200001};

#pragma warning (disable : 4238) //warning C4238: nonstandard extension used : class rvalue used as lvalue

//

class COperationCompleteActive : public CActive
	{
public:
	COperationCompleteActive();
	~COperationCompleteActive();
	void Start();
protected:
	void DoCancel();
	void RunL();
	};

COperationCompleteActive::COperationCompleteActive()
: CActive(0)
	{
	CActiveScheduler::Add(this);
	}

COperationCompleteActive::~COperationCompleteActive()
	{
	Cancel();
	}

void COperationCompleteActive::Start()
	{
	SetActive();
	}

void COperationCompleteActive::RunL() 
	{
	CActiveScheduler::Stop();
	}

void COperationCompleteActive::DoCancel()
	{
	TRequestStatus* s=&iStatus;
	User::RequestComplete(s, KErrCancel);
	}

//

LOCAL_C void GetStoreFileNameL(TMsvId aId, TMsvId aParent, TFileName& aFileName)
	{
	aFileName = KMsvTestFolder;
	
	TMsvId service=aParent;
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, service);
	while (sEntry->Entry().iType!=KUidMsvServiceEntry)
		{
		service = sEntry->Entry().Parent();
		REPORT_IF_ERROR(sEntry->SetEntry(service));
		}
	delete sEntry;

	MsvUtils::ConstructEntryName(service, aId, aFileName, MsvUtils::EStore);

	}


LOCAL_C void TestConstructorWithHeapFailL()
	{
	CMsvServerEntry* sEntry = NULL;

	TBool finished = EFalse;
#if defined(_DEBUG)		
	TInt failCount = 0;
#endif	
	TInt err = KErrNone;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		// create a CMsvServerEntry 
		TRAP(err, sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId));

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;
			test(sEntry != NULL);
			}
		else
			{
			// check we failed with no memory
			test(err == KErrNoMemory);
			}
		}

	// now create a folder with heap failure
	TMsvEntry folder;
	folder.iDescription.Set(KLongDescription);
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;

	finished = EFalse;
#if defined(_DEBUG)		
	failCount = 0;
#endif	

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		err = sEntry->CreateEntry(folder);

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			// check that folder creation worked
			REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
			test(sEntry->Entry()==folder);

			}
		else
			{
			// check we failed with no memory
			test(err == KErrNoMemory);
			}
		}

	delete sEntry; sEntry=NULL;

	}


LOCAL_C void TestConstructorWithFileFailL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);

	// create a folder with file failure
	TMsvEntry folder;
	folder.iDescription.Set(KLongDescription);
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;

	TBool finished = EFalse;
	TInt failCount = 0;
	TInt err = KErrNone;

	while(!finished)
		{
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).


		if(failCount==11)
			failCount++;
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		err = sEntry->CreateEntry(folder);
		test(err==KErrNone || err==KErrGeneral);
		theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);

		if (err == KErrNone)
			{
			finished = ETrue;

			// check that folder creation worked
			REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
			test(sEntry->Entry()==folder);

			}
		else
			test(ETrue);
		}

	delete sEntry; sEntry=NULL;

	}


LOCAL_C void TestSwitchingWithHeapFailL()
	{
	// create folder to use
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	
	TMsvEntry folder;
	folder.iDetails.Set(KLongDetails);
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));

	// check the context is correct
	test(sEntry->Entry().Id() == KMsvLocalServiceIndexEntryId);

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		// switch context to folder
		err = sEntry->SetEntry(folder.Id());
	
		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			// check context has changed
			test(sEntry->Entry()==folder);
			}
		else
			{
			// check we failed with no memory
			test(err == KErrNoMemory);

			// check context stays the same
			test(sEntry->Entry().Id() == KMsvLocalServiceIndexEntryId);
			}
		}


	// lose folder
	delete sEntry; sEntry=NULL;
	}


LOCAL_C void TestChangingWithHeapFailL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	
	// create folder to use
	TMsvEntry folder;
	folder.iDetails.Set(KLongDetails);
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));

	// switch to folder
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
	test(sEntry->Entry()==folder);
	
	folder.iDescription.Set(KShortDescription);
	folder.iDetails.Set(KLongDescription);

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		err = sEntry->ChangeEntry(folder);

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			// check the folder details are correct
			test(sEntry->Entry()==folder);
			}
		else
			{
			test(err = KErrNoMemory);
			}
		}
		
	delete sEntry; sEntry=NULL;
	}


LOCAL_C void TestChangingWithFileFailL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	
	// create folder to use
	TMsvEntry folder;
	folder.iDetails.Set(KLongDetails);
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));

	// switch to folder
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
	test(sEntry->Entry()==folder);
	
	folder.iDescription.Set(KShortDescription);
	folder.iDetails.Set(KLongDescription);

	TBool finished = EFalse;
	TInt failCount = 0;
	TInt err = KErrNone;

	while(!finished)
		{
		if(failCount==8)
			failCount++;
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		err = sEntry->ChangeEntry(folder);
		test(err==KErrGeneral || err==KErrNone);

		theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);

		if (err == KErrNone)
			{
			finished = ETrue;

			// check the folder details are correct
			test(sEntry->Entry()==folder);
			}
		else
			test(ETrue);
		}
		
	delete sEntry; sEntry=NULL;
	}



LOCAL_C void TestDeletingWithHeapFailL()
	{
	// create folder to use
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	TMsvEntry folder;
	folder.iDetails.Set(KLongDetails);
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));

	// create a child then delete it
	TMsvEntry entry;
	entry.iDescription.Set(KLongDescription);
	entry.iType = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		
		__UHEAP_FAILNEXT(failCount++);

		// delete the child
		err = sEntry->DeleteEntry(entry.Id());

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			test(sEntry->SetEntry(entry.Id()) == KErrNotFound);
			}
		else
			{
			REPORT_IF_ERROR(sEntry->SetEntry(KMsvNullIndexEntryId));

			TFileName fileName;
			GetStoreFileNameL(entry.Id(), folder.Id(), fileName);

			// check the entry still exists
			REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));
			REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
			}
		}

	CleanupStack::PopAndDestroy(); // sEntry
	}


LOCAL_C void TestDeletingWithFileFailL()
	{
	// create folder to use
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	TMsvEntry folder;
	folder.iDetails.Set(KLongDetails);
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));

	// create a child then delete it
	TMsvEntry entry;
	entry.iDescription.Set(KLongDescription);
	entry.iType = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));

	TBool finished = EFalse;
	TInt failCount = 0;
	TInt err = KErrNone;

	while(!finished)
		{
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).


		if (failCount==13)
			failCount++;

		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		// delete the child
		err = sEntry->DeleteEntry(entry.Id());
		test(err==KErrNone||err==KErrGeneral);

		theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);

		if (err == KErrNone)
			{
			finished = ETrue;

			test(sEntry->SetEntry(entry.Id()) == KErrNotFound);
			}
		else
			{
			REPORT_IF_ERROR(sEntry->SetEntry(KMsvNullIndexEntryId));

			TFileName fileName;
			GetStoreFileNameL(entry.Id(), folder.Id(), fileName);

			// check the entry still exists
			REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));
			REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
			}
		}

	CleanupStack::PopAndDestroy(); // sEntry
	}

	
LOCAL_C void TestDeleteAllWithHeapFailL()	
	{
	TMsvEntry folder;
	TMsvEntry entry1;
	TMsvEntry entry2;
	TMsvEntry entry3;
	TMsvEntry entry4;

	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	// create folder to use & create several children
	folder.iDetails.Set(KLongDetails);
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
	// 
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry1));

	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry2));
	entry3.iType = KUidMsvMessageEntry;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry3));
	entry4.iType = KUidMsvMessageEntry;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry4));

	// delete all 
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		err = sEntry->DeleteEntry(folder.Id());

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;
			test(sEntry->SetEntry(folder.Id()) == KErrNotFound);
			}
		else
			{
			test(err == KErrNoMemory);
			REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
			}

		REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
		}
	
	CleanupStack::PopAndDestroy(); // sEntry
	}
	

LOCAL_C void TestDeleteAllWithFileFailL()	
	{
	TMsvEntry folder;
	TMsvEntry entry1;
	TMsvEntry entry2;
	TMsvEntry entry3;
	TMsvEntry entry4;

	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	// create folder to use & create several children
	folder.iDetails.Set(KLongDetails);
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
	// 
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry1));

	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry2));
	entry3.iType = KUidMsvMessageEntry;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry3));
	entry4.iType = KUidMsvMessageEntry;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry4));

	// delete all 
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));

	TBool finished = EFalse;
	TInt failCount = 0;
	TInt err = KErrNone;

	while(!finished)
		{
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).


		if (failCount==33)
			failCount++;

		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		err = sEntry->DeleteEntry(folder.Id());
		test(err==KErrNone||err==KErrGeneral);

		theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);

		if (err == KErrNone)
			{
			finished = ETrue;
			test(sEntry->SetEntry(folder.Id()) == KErrNotFound);
			}
		else
			{
			// check that the deletion count is what we would expect
			REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
			}

		REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
		}
	
	CleanupStack::PopAndDestroy(); // sEntry
	}


LOCAL_C void TestDeletingWithStoresFilesWithHeapFailL()
	{
	RFile file;
	RMsvWriteStream writeStream;
	CMsvStore* store = NULL;
	TFileName filename;

	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMsvLocalServiceMtm;

	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	// deleting entry with closed store
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));
	sEntry->SetEntry(folder.Id());

	REPORT_IF_ERROR(sEntry->CreateEntry(entry));

	// create a store
	sEntry->SetEntry(entry.Id());
	store = sEntry->EditStoreL();
	CleanupStack::PushL(store);

	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	CleanupStack::PopAndDestroy(); // store

	// test file exists
	GetStoreFileNameL(entry.Id(), entry.Parent(), filename);
	test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrAlreadyExists);

	TBool finished = EFalse;
	TInt failCount = 0;
	TInt err = KErrNone;

	while(!finished)
		{

		sEntry->SetEntry(folder.Id());
		
		__UHEAP_FAILNEXT(failCount++);

		// delete the entry
		err = sEntry->DeleteEntry(entry.Id());

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			// test file has been removed
			sEntry->SetEntry(KMsvNullIndexEntryId);
			GetStoreFileNameL(entry.Id(), entry.Parent(), filename);
			test(file.Open(theUtils->FileSession(), filename, EFileShareAny|EFileWrite)==KErrNotFound);
			}
		else
			{
			test(err == KErrNoMemory);

			// test file exists
			sEntry->SetEntry(KMsvNullIndexEntryId);
			GetStoreFileNameL(entry.Id(), entry.Parent(), filename);

			sEntry->SetEntry(entry.Id());

			// PAH: No longer clearing complete flag
			if (failCount < 12 && failCount > 13)
				{
				// Check the file exists or is marked as incomplete
				test(file.Create(theUtils->FileSession(), filename, EFileShareAny|EFileWrite) == KErrAlreadyExists || !sEntry->Entry().Complete());
				file.Close();
				}
			}
		}

	CleanupStack::PopAndDestroy(); // sEntry
	}

LOCAL_C void TestChildrenSelectionWithHeapFailL()
	{
	TMsvEntry folder;
	TMsvEntry entry1, entry2, entry3, entry4;

	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
		
	// create folder to use & create several children
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));

	// create children
	entry1.iSize = 100;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry1));

	entry2.iSize = 10000;
	entry2.iType = KUidMsvFolderEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry2));

	entry3.iSize = 10;
	entry3.iType = KUidMsvMessageEntry;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry3));

	entry4.iSize = 1000;
	entry4.SetPriority(EMsvHighPriority);
	entry4.iType = KUidMsvMessageEntry;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry4));
	
	// sort order
	TMsvSelectionOrdering order;
	order.SetGroupByType(ETrue);
	order.SetGroupByPriority(ETrue);
	order.SetGroupByMtm(EFalse);
	order.SetSorting(EMsvSortBySize);

	// test the selections
	sEntry->SetSort(order);

	TBool finished = EFalse;
#if defined(_DEBUG)		
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		selection->Reset();

		__UHEAP_FAILNEXT(failCount++);

		err = sEntry->GetChildren(*selection);

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			// Check selection sort order is correct
			test(selection->At(0)==entry2.Id());
			test(selection->At(1)==entry4.Id());
			test(selection->At(2)==entry3.Id());
			test(selection->At(3)==entry1.Id());
			}
		else
			{
			test(err == KErrNoMemory);
			}
		}

	finished = EFalse;
#if defined(_DEBUG)		
	failCount=0;
#endif	

	while(!finished)
		{
		selection->Reset();

		__UHEAP_FAILNEXT(failCount++);

		err = sEntry->GetChildrenWithService(KMsvLocalServiceIndexEntryId, *selection);

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			// Check selection sort order is correct
			test(selection->At(0)==entry2.Id());
			test(selection->At(1)==entry4.Id());
			test(selection->At(2)==entry3.Id());
			test(selection->At(3)==entry1.Id());
			}
		else
			{
			test(err == KErrNoMemory);
			}
		}

	finished = EFalse;
#if defined(_DEBUG)		
	failCount=0;
#endif	

	while(!finished)
		{
		selection->Reset();

		__UHEAP_FAILNEXT(failCount++);

		err = sEntry->GetChildrenWithMtm(KUidMsvLocalServiceMtm, *selection);

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			// Check selection sort order is correct
			test(selection->At(0)==entry2.Id());
			test(selection->At(1)==entry4.Id());
			test(selection->At(2)==entry3.Id());
			test(selection->At(3)==entry1.Id());
			}
		else
			{
			test(err == KErrNoMemory);
			}
		}

	finished = EFalse;
#if defined(_DEBUG)		
	failCount=0;
#endif	

	while(!finished)
		{
		selection->Reset();

		__UHEAP_FAILNEXT(failCount++);

		err = sEntry->GetChildrenWithType(KUidMsvMessageEntry, *selection);

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			// Check selection sort order is correct
			test(selection->At(0)==entry4.Id());
			test(selection->At(1)==entry3.Id());
			test(selection->At(2)==entry1.Id());
			}
		else
			{
			test(err == KErrNoMemory);
			}
		}

	CleanupStack::PopAndDestroy(2); // selection, sEntry
	}


LOCAL_C void TestOwnerFlagWithHeapFailL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection; 
	CleanupStack::PushL(selection);

	TMsvEntry folder;
	folder.iDetails.Set(KLongDetails);
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));

	// test flag
	test(folder.Owner()==EFalse);

	// create child
	TMsvEntry entry;
	entry.iDescription.Set(KLongDescription);
	entry.iType = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMsvLocalServiceMtm;

	TBool finished = EFalse;
#if defined(_DEBUG)		
	TInt failCount = 0;
#endif	
	TInt err = KErrNone;

	// Test owner flag is not set if creation fails
	while(!finished)
		{	
		__UHEAP_FAILNEXT(failCount++);

		err = sEntry->CreateEntry(entry);

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			// test flag is set
			test(sEntry->Entry().Owner());
			REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
			REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
			test(sEntry->Entry().Owner());
			}
		else
			{
			// test flag is NOT set
			test(sEntry->Entry().Owner() == EFalse);
			REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
			REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
			test(sEntry->Entry().Owner() == EFalse);
			}
		}

	finished = EFalse;
#if defined(_DEBUG)		
	failCount = 0;
#endif	


	// Test owner flag not reset on deletion failure
	while(!finished)
		{

		__UHEAP_FAILNEXT(failCount++);

		// delete the child
		err = sEntry->DeleteEntry(entry.Id());

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			// test owner flag has been unset
			test(sEntry->Entry().Owner()==EFalse);
			REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
			REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
			test(sEntry->Entry().Owner()==EFalse);
			}
		else
			{
			// test owner flag still set
			test(sEntry->Entry().Owner());
			REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
			REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
			test(sEntry->Entry().Owner());
			}
		}

	CleanupStack::PopAndDestroy(2); // sEntry1, selection
	}


LOCAL_C void TestOwnerFlagWithFileFailL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection; 
	CleanupStack::PushL(selection);

	TMsvEntry folder;
	folder.iDetails.Set(KLongDetails);
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));

	// test flag
	test(folder.Owner()==EFalse);

	// create child
	TMsvEntry entry;
	entry.iDescription.Set(KLongDescription);
	entry.iType = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMsvLocalServiceMtm;

	TBool finished = EFalse;
	TInt failCount = 0;
	TInt err = KErrNone;

	// Test owner flag is not set if creation fails
	while(!finished)
		{
		if (failCount==8)
			failCount++;

		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		err = sEntry->CreateEntry(entry);
		test(err==KErrNone || err==KErrGeneral);

		theUtils->FileSession().SetErrorCondition(KErrGeneral, 10000);

		if (err == KErrNone)
			{
			finished = ETrue;

			// test flag is set
			test(sEntry->Entry().Owner());
			REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
			REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
			test(sEntry->Entry().Owner());
			}
		else
			{
			// test flag is NOT set
			test(sEntry->Entry().Owner() == EFalse);
			REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
			REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
			test(sEntry->Entry().Owner() == EFalse);
			}
		}

	finished = EFalse;
	failCount = 0;

	// Test owner flag not reset on deletion failure
	while(!finished)
		{

// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).


		if (failCount == 13)
			failCount+=1;

		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		// delete the child
		TRAP(err, err = sEntry->DeleteEntry(entry.Id()));
		test(err==KErrNone||err==KErrGeneral);

		theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);
		if (err == KErrNone)
			{
			finished = ETrue;

			// test owner flag has been unset
			test(sEntry->Entry().Owner()==EFalse);
			REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
			REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
			test(sEntry->Entry().Owner()==EFalse);
			}
		else
			{
			// test owner flag still set
			test(sEntry->Entry().Owner());
			REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
			REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
			test(sEntry->Entry().Owner());
			}
		}

	CleanupStack::PopAndDestroy(2); // sEntry1, selection
	}


LOCAL_C void TestStoreFlagWithHeapFailL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	// create entry to work with
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));

	CMsvStore* store = NULL;
	
	TBool finished = EFalse;
#if defined(_DEBUG)		
	TInt failCount = 0;
#endif	
	TInt err = KErrNone;

	// get the store flag
	TBool hasStore=sEntry->HasStoreL();

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		TRAP(err, store = sEntry->EditStoreL());

		__UHEAP_RESET;

		CleanupStack::PushL(store);

		// delete the store
		if (hasStore && store)
			store->DeleteL();
		else if(err==KErrNone)
			{
			RMsvWriteStream out;
			out.AssignLC(*store,KNullUid);
			out.WriteInt32L(1234);
			out.CommitL();
			CleanupStack::PopAndDestroy(&out);
			store->CommitL();
			}


		CleanupStack::PopAndDestroy(); // store 
		store = NULL;

		if (err == KErrNone)
			{
			finished = ETrue;

			// PAH
			CTestTimer* timer = CTestTimer::NewL();
			timer->After(0x00C00000); // has to be longer than KMsvDelayTime
			CActiveScheduler::Start();
			delete timer;
			}
		}

	REPORT_IF_ERROR(sEntry->SetEntry(KMsvRootIndexEntryId));

	// check behaviour of remote entry
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	REPORT_IF_ERROR(sEntry->CreateEntry(service));
	REPORT_IF_ERROR(sEntry->SetEntry(service.Id()));

	// create entry to work with
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KMsvTestUid1;
	entry1.iServiceId = service.Id();
	REPORT_IF_ERROR(sEntry->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));

	hasStore=sEntry->HasStoreL();

	finished = EFalse;
#if defined(_DEBUG)		
	failCount = 0;
#endif	

	while(!finished)
		{	
		__UHEAP_FAILNEXT(failCount++);

		TRAP(err, store = sEntry->EditStoreL());

		__UHEAP_RESET;

		CleanupStack::PushL(store);

		// delete the store
		if (hasStore && store)
			store->DeleteL();
		else if(err==KErrNone)
			{
			RMsvWriteStream out;
			out.AssignLC(*store,KNullUid);
			out.WriteInt32L(1234);
			out.CommitL();
			CleanupStack::PopAndDestroy(&out);
			store->CommitL();
			}

		CleanupStack::PopAndDestroy(); // store
		store = NULL;

		if (err == KErrNone)
			{
			finished = ETrue;

			// test the flag has been changed
			test((hasStore && !sEntry->HasStoreL()) || (!hasStore && sEntry->HasStoreL()));

			// test the flag has been changed on the server
			REPORT_IF_ERROR(sEntry->SetEntry(KMsvRootIndexEntryId));
			REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));

			// Test will fail because of defect: EDN817731
			// No point fixing this minor and unlikely problem
			//test((hasStore && !sEntry->Entry().Store()) || (!hasStore && sEntry->Entry().Store()));
			}
		else
			{
			// Test the flag has not changed
//PAH		test((hasStore && sEntry->HasStoreL()) || (!hasStore && !sEntry->HasStoreL()));
			}
		}

	CleanupStack::PopAndDestroy(); // sEntry
	}


LOCAL_C void TestStoreFlagWithFileFailL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	// create entry to work with
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));

	// get the store flag
	TBool hasStore=sEntry->HasStoreL();

	CMsvStore* store = NULL;
	
	TBool finished = EFalse;
	TInt failCount = 0;
	TInt err = KErrNone;

	while(!finished)
		{
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		TRAP(err, store = sEntry->EditStoreL());

		test(err==KErrNone || err==KErrGeneral);
		theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);

		CleanupStack::PushL(store);

		// delete the store
		if (hasStore && store)
			store->DeleteL();

		CleanupStack::PopAndDestroy(); // store 
		store = NULL;

		if (err == KErrNone)
			{
			finished = ETrue;
		
			// PAH
			CTestTimer* timer = CTestTimer::NewL();
			timer->After(0x00C00000); // has to be longer than KMsvDelayTime
			CActiveScheduler::Start();
			delete timer;
			}

		// Test the flag has not changed
		// local entries always have a store - Its recreated if deleted
		test((hasStore && sEntry->HasStoreL()) || (!hasStore && !sEntry->HasStoreL()));
		}

	REPORT_IF_ERROR(sEntry->SetEntry(KMsvRootIndexEntryId));

	// check behaviour of remote entry
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	REPORT_IF_ERROR(sEntry->CreateEntry(service));
	REPORT_IF_ERROR(sEntry->SetEntry(service.Id()));

	// create entry to work with
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KMsvTestUid1;
	entry1.iServiceId = service.Id();
	REPORT_IF_ERROR(sEntry->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));

	hasStore=sEntry->HasStoreL();
	
	finished = EFalse;
	failCount = 0;

	while(!finished)
		{
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		TRAP(err, store = sEntry->EditStoreL());
		test(err==KErrNone || err==KErrGeneral);
		
		theUtils->FileSession().SetErrorCondition(KErrGeneral, 10000);

		CleanupStack::PushL(store);

		// delete the store
		if (hasStore && store)
			store->DeleteL();
		else if(err==KErrNone)
			{
			RMsvWriteStream out;
			out.AssignLC(*store,KNullUid);
			out.WriteInt32L(1234);
			out.CommitL();
			CleanupStack::PopAndDestroy(&out);
			store->CommitL();
			}

		CleanupStack::PopAndDestroy(); // store
		store = NULL;

		if (err == KErrNone)
			{
			finished = ETrue;

			// test the flag has been changed
			test((hasStore && !sEntry->HasStoreL()) || (!hasStore && sEntry->HasStoreL()));

			// test the flag has been changed on the server
			REPORT_IF_ERROR(sEntry->SetEntry(KMsvRootIndexEntryId));
			REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));

			// Test will fail because of defect: EDN817731
			// No point fixing this minor and unlikely problem
			//test((hasStore && !sEntry->Entry().Store()) || (!hasStore && sEntry->Entry().Store()));
			}
		else
			{
			// Test the flag has not changed
// PAH		test((hasStore && sEntry->HasStoreL()) || (!hasStore && !sEntry->HasStoreL()));
			}
		}

	CleanupStack::PopAndDestroy(); // sEntry
	}


LOCAL_C void TestStoresWithHeapFailL()
	{
	RMsvReadStream readStream;
	RMsvWriteStream writeStream;
	TInt value;
	CMsvStore* store = NULL;

	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	// create folder to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));

	// create entry to work with
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		// create store for writing
		TRAP(err, store = sEntry->EditStoreL());

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			CleanupStack::PushL(store);

			writeStream.AssignL(*store, KMsvTestUid1);
			writeStream.WriteInt32L(4321);
			writeStream.CommitL();
			writeStream.Close();
			readStream.OpenL(*store, KMsvTestUid1);
			value = readStream.ReadInt32L();
			test(value==4321);
			readStream.Close();
			store->CommitL();
			CleanupStack::PopAndDestroy(); // store

			// open store for reading
			store = sEntry->ReadStoreL();
			CleanupStack::PushL(store);

			readStream.OpenL(*store, KMsvTestUid1);
			value = readStream.ReadInt32L();
			test(value==4321);
			readStream.Close();
			CleanupStack::PopAndDestroy(); // store
			}
		else
			test(err == KErrNoMemory);
		}

	CleanupStack::PopAndDestroy(); // sEntry
	}


LOCAL_C void TestReadStoreWithHeapFailL()
	{
	RMsvWriteStream writeStream;
	CMsvStore* store = NULL;
	
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);
	
	// create folder to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
	
	// create entry to work with
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	
	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;
	
	while(!finished)
		{
		// create a entry and make sure it has a store
		REPORT_IF_ERROR(sEntry->CreateEntry(entry));
		REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));		
		store=sEntry->EditStoreL();
		CleanupStack::PushL(store);
		writeStream.AssignLC(*store, KMsvTestUid1);
		writeStream.WriteInt32L(4321);
		writeStream.CommitL();
		CleanupStack::PopAndDestroy(&writeStream);
		store->CommitL();
		CleanupStack::PopAndDestroy(store);		

		
		__UHEAP_FAILNEXT(failCount++);
		
		// create store for reading
		TRAP(err, store = sEntry->ReadStoreL());
		
		__UHEAP_RESET;
		
		if (err != KErrNone)
			{
			test(err==KErrNoMemory);
			REPORT_IF_ERROR(sEntry->SetEntry(entry.Parent()));		
			TInt error=sEntry->DeleteEntry(entry.Id());
			test(error==KErrNone);
			}
		else
			{
			delete store;
			finished=ETrue;
			}
		}
	
	CleanupStack::PopAndDestroy(); // sEntry
	}




LOCAL_C void TestStoresWithFileFailL()
	{
	RMsvReadStream readStream;
	RMsvWriteStream writeStream;
	TInt value;
	CMsvStore* store = NULL;

	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	// create folder to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));

	// create entry to work with
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));

	TBool finished = EFalse;
	TInt failCount = 0;
	TInt err = KErrNone;

	while(!finished)
		{
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		// create store for writing
		TRAP(err, store = sEntry->EditStoreL());
		test(err==KErrNone || err==KErrGeneral);
		
		theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);

		if (err == KErrNone)
			{
			finished = ETrue;

			CleanupStack::PushL(store);

			writeStream.AssignL(*store, KMsvTestUid1);
			writeStream.WriteInt32L(4321);
			writeStream.CommitL();
			writeStream.Close();
			readStream.OpenL(*store, KMsvTestUid1);
			value = readStream.ReadInt32L();
			test(value==4321);
			readStream.Close();
			store->CommitL();
			CleanupStack::PopAndDestroy(); // store

			// open store for reading
			store = sEntry->ReadStoreL();
			CleanupStack::PushL(store);

			readStream.OpenL(*store, KMsvTestUid1);
			value = readStream.ReadInt32L();
			test(value==4321);
			readStream.Close();
			CleanupStack::PopAndDestroy(); // store
			}
		else
			test(ETrue);
		}

	CleanupStack::PopAndDestroy(); // sEntry
	}


LOCAL_C void TestLocalStoreFlagWithHeapFailL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	RMsvWriteStream writeStream;
	CMsvStore* store = NULL;

	// create entry to work with
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));

	test(!sEntry->HasStoreL());

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		// create store
		TRAP(err, store = sEntry->EditStoreL());

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			CleanupStack::PushL(store);
			writeStream.AssignL(*store, KMsvTestUid1);
			writeStream.WriteInt32L(4321);
			writeStream.CommitL();
			writeStream.Close();
			store->CommitL();
			CleanupStack::PopAndDestroy(); // store

			test(sEntry->HasStoreL());

			// test the server copy
			REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
			REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));
			test(sEntry->HasStoreL());
			}
		else
			test(err == KErrNoMemory);
		}

	CleanupStack::PopAndDestroy(); // sEntry
	}


LOCAL_C void TestLocalStoreFlagWithFileFailL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	RMsvWriteStream writeStream;
	CMsvStore* store = NULL;

	// create entry to work with
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));

	TBool finished = EFalse;
	TInt failCount = 0;
	TInt err = KErrNone;

	while(!finished)
		{
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		// create store
		TRAP(err, store = sEntry->EditStoreL());
		test(err==KErrNone || err==KErrGeneral);
		
		theUtils->FileSession().SetErrorCondition(KErrGeneral, 10000);

		if (err == KErrNone)
			{
			finished = ETrue;

			CleanupStack::PushL(store);
			writeStream.AssignL(*store, KMsvTestUid1);
			writeStream.WriteInt32L(4321);
			writeStream.CommitL();
			writeStream.Close();
			store->CommitL();
			CleanupStack::PopAndDestroy(); // store

			test(sEntry->HasStoreL());

			// test the server copy
			REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
			REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));
			test(sEntry->HasStoreL());
			}
		}

	// change store
	store = sEntry->EditStoreL();
	CleanupStack::PushL(store);

	writeStream.AssignL(*store, KMsvTestUid2);
	writeStream.WriteInt32L(4545);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	CleanupStack::PopAndDestroy(); // store

	test(sEntry->HasStoreL());

	// test the server copy
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));
	test(sEntry->HasStoreL());

	store = sEntry->EditStoreL();
	CleanupStack::PushL(store);

	finished = EFalse;
	failCount = 0;

	while(!finished)
		{
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		// delete the store
		TRAP(err, store->DeleteL());
		test(err==KErrNone || err==KErrGeneral);
		
		theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);
	
		if (err == KErrNone)
			{
			finished = ETrue;

			CleanupStack::PopAndDestroy(); // store

			// PAH
			CTestTimer* timer = CTestTimer::NewL();
			timer->After(0x00C00000); // has to be longer than KMsvDelayTime
			CActiveScheduler::Start();
			delete timer;
				// test the server copy
			REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
			REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));
			}
		}

	CleanupStack::PopAndDestroy(); // sEntry
	}


LOCAL_C void TestRemoteStoreFlagWithHeapFailL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvRootIndexEntryId);
	CleanupStack::PushL(sEntry);

	RMsvWriteStream writeStream;
	CMsvStore* store=NULL;

	// create folder to work under
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	REPORT_IF_ERROR(sEntry->CreateEntry(service));
	REPORT_IF_ERROR(sEntry->SetEntry(service.Id()));

	// create entry to work with
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KMsvTestUid1;
	entry.iServiceId = service.Id();
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));

	test(sEntry->HasStoreL()==EFalse);

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		// create store
		TRAP(err, store = sEntry->EditStoreL());

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			CleanupStack::PushL(store);

			writeStream.AssignL(*store, KMsvTestUid1);
			writeStream.WriteInt32L(4321);
			writeStream.CommitL();
			writeStream.Close();
			store->CommitL();

			CleanupStack::PopAndDestroy(); // store

			test(sEntry->HasStoreL());

			// test the server copy
			// Test will fail because of defect: EDN817731
			// No point fixing this minor and unlikely problem

			//REPORT_IF_ERROR(sEntry->SetEntry(service.Id()));
			//REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));
			//test(sEntry->Entry().Store());
			}
		else
			test(err == KErrNoMemory);
		}

	CleanupStack::PopAndDestroy(); // sEntry
	}


LOCAL_C void TestRemoteStoreFlagWithFileFailL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvRootIndexEntryId);
	CleanupStack::PushL(sEntry);

	RMsvWriteStream writeStream;
	CMsvStore* store = NULL;

	// create folder to work under
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	REPORT_IF_ERROR(sEntry->CreateEntry(service));
	REPORT_IF_ERROR(sEntry->SetEntry(service.Id()));

	// create entry to work with
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KMsvTestUid1;
	entry.iServiceId = service.Id();
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));

	test(sEntry->HasStoreL()==EFalse);

	TBool finished = EFalse;
	TInt failCount = 0;
	TInt err = KErrNone;

	while(!finished)
		{
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		// create store
		TRAP(err, store = sEntry->EditStoreL());
		test(err==KErrNone || err==KErrGeneral);
		
		theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);

		if (err == KErrNone)
			{
			finished = ETrue;

			CleanupStack::PushL(store);

			writeStream.AssignL(*store, KMsvTestUid1);
			writeStream.WriteInt32L(4321);
			writeStream.CommitL();
			writeStream.Close();
			store->CommitL();
			CleanupStack::PopAndDestroy(); // store

			test(sEntry->HasStoreL());

			// test the server copy
			// Test will fail because of defect: EDN817731
			// No point fixing this minor and unlikely problem

			//REPORT_IF_ERROR(sEntry->SetEntry(service.Id()));
			//REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));
			//test(sEntry->Entry().Store());
			}
		else
			test(ETrue);
		}

	CleanupStack::PopAndDestroy(); // sEntry
	}


LOCAL_C void TestMovingSyncWithHeapFailL()
	{
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry folder2;
	folder2.iType = KUidMsvFolderEntry;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMsvLocalServiceMtm;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	CMsvServerEntry* sEntry2 = CMsvServerEntry::NewL(*theServer, KMsvGlobalInBoxIndexEntryId);
	CleanupStack::PushL(sEntry2);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create the folders
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder2));
	sEntry1->SetEntry(folder1.Id());
	sEntry2->SetEntry(folder2.Id());

	// create the child
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry));
	
	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==0);

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		// move to folder2
		err = sEntry1->MoveEntryWithinService(entry.Id(), folder2.Id());

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			// test the entry has been moved
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==0);
			selection->Reset(); sEntry2->GetChildren(*selection);
			test(selection->Count()==1);
			}
		else
			{
			test(err == KErrNoMemory);

			// test the entry has not moved
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);
			selection->Reset(); sEntry2->GetChildren(*selection);
			test(selection->Count()==0);
			}
		}

	CleanupStack::PopAndDestroy(3); // selection, sEntry1, sEntry2
	}

LOCAL_C void TestMovingAsyncWithHeapFailL()
	{
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMsvLocalServiceMtm;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create the folders
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	sEntry1->SetEntry(folder.Id());
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry));
	
	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		// move to folder2
		TRAP(err, sEntry1->MoveEntryL(entry.Id(), service.Id(), active->iStatus));
		if (err == KErrNone)
			{
			active->Start();
			CActiveScheduler::Start();
			}
		__UHEAP_RESET;

		if (err == KErrNone && active->iStatus == KErrNone)
			{
			finished = ETrue;

			// test the entry has been moved
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==0);

			sEntry1->SetEntry(KMsvNullIndexEntryId);
			sEntry1->SetEntry(folder.Id());
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==0);

			sEntry1->SetEntry(service.Id());
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);
			}
		else
			{
			test(err == KErrNoMemory || active->iStatus == KErrNoMemory);

			// test the entry has not moved
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);

			sEntry1->SetEntry(KMsvNullIndexEntryId);
			sEntry1->SetEntry(folder.Id());
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);

			sEntry1->SetEntry(service.Id());
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==0);
			}

		sEntry1->SetEntry(folder.Id());
		}

	CleanupStack::PopAndDestroy(3); // selection, active, sEntry1
	}

LOCAL_C void TestCopyingAsyncWithHeapFailL()
	{
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMsvLocalServiceMtm;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create the folders
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	sEntry1->SetEntry(folder.Id());
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry));
	
	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		// move to folder2
		TRAP(err, sEntry1->CopyEntryL(entry.Id(), service.Id(), active->iStatus));
		if (err == KErrNone)
			{
			active->Start();
			CActiveScheduler::Start();
			}
		__UHEAP_RESET;

		if (err == KErrNone && active->iStatus == KErrNone)
			{
			finished = ETrue;

			// test the entry has been moved
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);

			sEntry1->SetEntry(KMsvNullIndexEntryId);
			sEntry1->SetEntry(folder.Id());
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);

			sEntry1->SetEntry(service.Id());
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);
			}
		else
			{
			test(err == KErrNoMemory || active->iStatus == KErrNoMemory);

			// test the entry has not moved
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);

			sEntry1->SetEntry(KMsvNullIndexEntryId);
			sEntry1->SetEntry(folder.Id());
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);

			sEntry1->SetEntry(service.Id());
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==0);
			}

		sEntry1->SetEntry(folder.Id());
		}

	CleanupStack::PopAndDestroy(3); // selection, active, sEntry1
	}
/* PAH: Fails in a very odd way
LOCAL_C void TestMovingAsyncWithFileFailL()
	{
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMsvLocalServiceMtm;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create the folders
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	sEntry1->SetEntry(folder.Id());
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry));
	
	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		// Skip over MsvUtils::HasDirectory and HasStore as these panic when file failure occurs
		if (failCount == 2)
			failCount+=2;

		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		// move to folder2
		TRAP(err, sEntry1->MoveEntryL(entry.Id(), service.Id(), active->iStatus));
		if (err == KErrNone)
			{
			active->Start();
			CActiveScheduler::Start();
			}
		theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);

		if (err == KErrNone && active->iStatus == KErrNone)
			{
			finished = ETrue;

			// test the entry has been moved
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==0);

			sEntry1->SetEntry(KMsvNullIndexEntryId);
			sEntry1->SetEntry(folder.Id());
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==0);

			sEntry1->SetEntry(service.Id());
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);
			}
		else
			{
			test(err == KErrGeneral || active->iStatus == KErrGeneral);

			// test the entry has not moved
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);

			sEntry1->SetEntry(KMsvNullIndexEntryId);
			sEntry1->SetEntry(folder.Id());
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);

			sEntry1->SetEntry(service.Id());
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==0);
			}

		sEntry1->SetEntry(folder.Id());
		}

	CleanupStack::PopAndDestroy(3); // selection, active, sEntry1
	}

LOCAL_C void TestCopyingAsyncWithFileFailL()
	{
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMsvLocalServiceMtm;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create the folders
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	sEntry1->SetEntry(folder.Id());
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry));
	
	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		// Skip over MsvUtils::HasDirectory and HasStore as these panic when file failure occurs
		if (failCount == 32)
			failCount+=16;

		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		// move to folder2
		TRAP(err, sEntry1->CopyEntryL(entry.Id(), service.Id(), active->iStatus));
		if (err == KErrNone)
			{
			active->Start();
			CActiveScheduler::Start();
			}
		theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);

		if (err == KErrNone && active->iStatus == KErrNone)
			{
			finished = ETrue;

			// test the entry has been moved
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);

			sEntry1->SetEntry(KMsvNullIndexEntryId);
			sEntry1->SetEntry(folder.Id());
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);

			// See comment below
			}
		else
			{
			test(err == KErrGeneral || active->iStatus == KErrGeneral);

			// test the entry has not moved
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);

			sEntry1->SetEntry(KMsvNullIndexEntryId);
			sEntry1->SetEntry(folder.Id());
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==1);

			// Note: It's possible for the entry to appear to have been copied to the service
			// This is because the copy was only half completed because of a file failure
			// The entry should partial entry should then be deleted by the server
			// However file failure is preventing this from happening.
			// This isn't a problem because the entry is still on the list of things to be deleted.
			}

		sEntry1->SetEntry(folder.Id());
		}

	CleanupStack::PopAndDestroy(3); // selection, active, sEntry1
	}
*/
LOCAL_C void TestMovingSelAsyncWithHeapFailL()
	{
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create the folders
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	sEntry1->SetEntry(folder.Id());
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		REPORT_IF_ERROR(sEntry1->SetEntry(folder.Id()));
		selection->Reset(); sEntry1->GetChildren(*selection);
		test(selection->Count()>0);

		__UHEAP_FAILNEXT(failCount++);

		// move to folder2
		TRAP(err, sEntry1->MoveEntriesL(*selection, service.Id(), active->iStatus));
		if (err == KErrNone)
			{
			active->Start();
			CActiveScheduler::Start();
			}
		__UHEAP_RESET;

		if (err == KErrNone && active->iStatus == KErrNone)
			{
			finished = ETrue;

			// test the entry has been moved
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==0);

			REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
			REPORT_IF_ERROR(sEntry1->SetEntry(folder.Id()));
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==0);

			REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==2);
			}
		else
			{
			test(err == KErrNoMemory || active->iStatus == KErrNoMemory);

			// test the entry has not moved
			selection->Reset(); sEntry1->GetChildren(*selection);

			// Some entries may have been copied
			test(selection->Count()>0);
			TInt count = selection->Count();

			REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
			REPORT_IF_ERROR(sEntry1->SetEntry(folder.Id()));
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==count);

			REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==2 - count);
			}
		}

	CleanupStack::PopAndDestroy(3); // selection, active, sEntry1
	}
/* PAH: Fails in a very odd way
LOCAL_C void TestCopyingSelAsyncWithHeapFailL()
	{
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry3;
	entry3.iType = KUidMsvMessageEntry;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iMtm = KUidMsvLocalServiceMtm;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create the folders
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	sEntry1->SetEntry(folder.Id());
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry3));

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		REPORT_IF_ERROR(sEntry1->SetEntry(folder.Id()));
		selection->Reset(); sEntry1->GetChildren(*selection);
		test(selection->Count()>0);

		// This skips file failure in CFileMan::Copy
		if (failCount == 0)
			failCount+=1000;

		__UHEAP_FAILNEXT(failCount++);

		// move to folder2
		TRAP(err, sEntry1->CopyEntriesL(*selection, service.Id(), active->iStatus));
		if (err == KErrNone)
			{
			active->Start();
			CActiveScheduler::Start();
			}
		__UHEAP_RESET;

		if (err == KErrNone && active->iStatus == KErrNone)
			{
			finished = ETrue;

			// test the entry has been moved
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==3);

			REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
			REPORT_IF_ERROR(sEntry1->SetEntry(folder.Id()));
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==3);
			}
		else
			{
			// Errors with KErrInUse when copying the store?
			test(err == KErrNoMemory || active->iStatus == KErrNoMemory);

			// test the entry has not moved
			selection->Reset(); sEntry1->GetChildren(*selection);

			// Some entries may have been copied
			test(selection->Count()>0);
			TInt count = selection->Count();

			REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
			REPORT_IF_ERROR(sEntry1->SetEntry(folder.Id()));
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count() == count);
			}
		}

	CleanupStack::PopAndDestroy(3); // selection, active, sEntry1
	}

LOCAL_C void TestMovingSelAsyncWithFileFailL()
	{
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry3;
	entry3.iType = KUidMsvMessageEntry;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iMtm = KUidMsvLocalServiceMtm;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// create the folders
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	sEntry1->SetEntry(folder.Id());
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry3));

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		if (failCount == 2 || failCount == 11 || failCount == 16)
			failCount+=2;

		REPORT_IF_ERROR(sEntry1->SetEntry(folder.Id()));
		selection->Reset(); sEntry1->GetChildren(*selection);
		test(selection->Count()>0);

		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		// move to folder2
		TRAP(err, sEntry1->MoveEntriesL(*selection, service.Id(), active->iStatus));
		if (err == KErrNone)
			{
			active->Start();
			CActiveScheduler::Start();
			}
		theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);

		if (err == KErrNone && active->iStatus == KErrNone)
			{
			finished = ETrue;

			// test the entry has been moved
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==0);

			REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
			REPORT_IF_ERROR(sEntry1->SetEntry(folder.Id()));
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==0);

			REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==3);
			}
		else
			{
//			test(err == KErrGeneral || active->iStatus == KErrGeneral);

			// test the entry has not moved
			selection->Reset(); sEntry1->GetChildren(*selection);

			// Some entries may have been copied
			test(selection->Count()>0);
			TInt count = selection->Count();

			REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
			REPORT_IF_ERROR(sEntry1->SetEntry(folder.Id()));
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==count);

			REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
			selection->Reset(); sEntry1->GetChildren(*selection);
			test(selection->Count()==3 - count);
			}
		}

	CleanupStack::PopAndDestroy(3); // selection, active, sEntry1
	}
*/
LOCAL_C void TestMovingSyncWithStoresWithHeapFailL()
	{

	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry folder2;
	folder2.iType = KUidMsvFolderEntry;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iMtm = KUidMsvLocalServiceMtm;

	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TFileName filename;

	// create the entries
	REPORT_IF_ERROR(sEntry->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry->CreateEntry(folder2));
	REPORT_IF_ERROR(sEntry->SetEntry(folder1.Id()));
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));

	// create a store
	REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));
	CMsvStore* store = sEntry->EditStoreL();
	store->CommitL();
	delete store; store = NULL;

	// test the store exists
	GetStoreFileNameL(entry.Id(), entry.Parent(), filename);
	CMsgsTestUtils::FileExists(filename);

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		REPORT_IF_ERROR(sEntry->SetEntry(folder1.Id()));

		__UHEAP_FAILNEXT(failCount++);

		// move the entry to a remote folder
		err = sEntry->MoveEntryWithinService(entry.Id(), folder2.Id());

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			// test the store has moved
//			FileMissing(filename);
//			GetStoreFileNameL(entry.Id(), folder2.Id(), filename);
			CMsgsTestUtils::FileExists(filename);
			}
		else
			{
			test(err == KErrNoMemory);

			// Check the file has not moved
			CMsgsTestUtils::FileExists(filename);
			}
		}

	CleanupStack::PopAndDestroy(2);
	}


LOCAL_C void TestPCSynchedEntryDeletionWithHeapFailL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TMsvEntry folder, entry;
	TMsvSelectionOrdering order(KMsvNoGrouping, EMsvSortByNone, ETrue);

	sEntry->SetSort(order);

	// create a folder to work under
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));
	
	// test
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvDeletedEntryFolderEntryId));
	REPORT_IF_ERROR(sEntry->GetChildren(*selection));
	test(selection->Count()==0);
	selection->Reset();

	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
	REPORT_IF_ERROR(sEntry->GetChildren(*selection));
	test(selection->Count()==0);
	selection->Reset();

	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
	entry.iDetails.Set(KLongDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.IncPcSyncCount();
	test(!entry.Deleted());
	test(entry.Visible());
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{

		REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));

		__UHEAP_FAILNEXT(failCount++);

		// delete entry with syncing
		err = sEntry->DeleteEntry(entry.Id());

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			// test
			REPORT_IF_ERROR(sEntry->GetChildren(*selection));
			test(selection->Count()==0);
			selection->Reset();

			REPORT_IF_ERROR(sEntry->SetEntry(KMsvDeletedEntryFolderEntryId));
			REPORT_IF_ERROR(sEntry->GetChildren(*selection));
			test(selection->Count()==1);
			}
		else
			{
			test(err == KErrNoMemory);

			// test
			REPORT_IF_ERROR(sEntry->GetChildren(*selection));
			test(selection->Count()==1);
			selection->Reset();

			REPORT_IF_ERROR(sEntry->SetEntry(KMsvDeletedEntryFolderEntryId));
			REPORT_IF_ERROR(sEntry->GetChildren(*selection));
			test(selection->Count()==0);
			}
		}

	CleanupStack::PopAndDestroy(2); // selection, sEntry
	}

const TInt KFolderCount = 2;
const TInt KEntriesPerFolder = 2;

LOCAL_C void TestDeleteSelWithHeapFailL()
	{
	TMsvEntry folder[KFolderCount];
	TMsvEntry entry[KFolderCount][KEntriesPerFolder];

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	// Create the folders
	for(TInt fc = 0; fc < KFolderCount; fc++)
		{
		REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));

		folder[fc].iDetails.Set(KLongDetails);
		folder[fc].iType = KUidMsvFolderEntry;
		folder[fc].iServiceId = KMsvLocalServiceIndexEntryId;
		folder[fc].iMtm = KUidMsvLocalServiceMtm;
		REPORT_IF_ERROR(sEntry->CreateEntry(folder[fc]));
		
		// Add folder to list of things to delete
		selection->AppendL(folder[fc].Id());
		REPORT_IF_ERROR(sEntry->SetEntry(folder[fc].Id()));

		// Create the entries
		for(TInt ec = 0; ec < KEntriesPerFolder; ec++)
			{
			entry[fc][ec].iType = KUidMsvMessageEntry;
			entry[fc][ec].iServiceId = KMsvLocalServiceIndexEntryId;
			entry[fc][ec].iMtm = KUidMsvLocalServiceMtm;
			REPORT_IF_ERROR(sEntry->CreateEntry(entry[fc][ec]));
			}
		}
	
	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	
	TInt error = KErrNone;

	while(!finished)
		{	
		REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));

		__UHEAP_FAILNEXT(failCount++);

		// Delete all the folders and their children
		error = sEntry->DeleteEntries(*selection);

		__UHEAP_RESET;

		if (error == KErrNone)
			{
			finished = ETrue;

			// Check there were no failures
			test(selection->Count() == 0);
			}
		else
			{
			test(error == KErrNoMemory);

			// Check failed entries still exist
			TInt count = 0;
			while(count < selection->Count())
				REPORT_IF_ERROR(sEntry->SetEntry(selection->At(count++)));
			}
		}

	CleanupStack::PopAndDestroy(2); // selection, sEntry
	}


LOCAL_C void TestDeleteSelWithFileFailL()
	{
	TMsvEntry folder[KFolderCount];
	TMsvEntry entry[KFolderCount][KEntriesPerFolder];

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	// Create the folders
	for(TInt fc = 0; fc < KFolderCount; fc++)
		{
		REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));

		folder[fc].iDetails.Set(KLongDetails);
		folder[fc].iType = KUidMsvFolderEntry;
		folder[fc].iServiceId = KMsvLocalServiceIndexEntryId;
		folder[fc].iMtm = KUidMsvLocalServiceMtm;
		REPORT_IF_ERROR(sEntry->CreateEntry(folder[fc]));
		
		// Add folder to list of things to delete
		selection->AppendL(folder[fc].Id());
		REPORT_IF_ERROR(sEntry->SetEntry(folder[fc].Id()));

		// Create the entries
		for(TInt ec = 0; ec < KEntriesPerFolder; ec++)
			{
			entry[fc][ec].iType = KUidMsvMessageEntry;
			entry[fc][ec].iServiceId = KMsvLocalServiceIndexEntryId;
			entry[fc][ec].iMtm = KUidMsvLocalServiceMtm;
			REPORT_IF_ERROR(sEntry->CreateEntry(entry[fc][ec]));
			}
		}
	
	TBool finished = EFalse;
	TInt failCount = 0;
	TInt error = KErrNone;

	while(!finished)
		{
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).



		if (failCount == 22)
			failCount+=2;
		

		REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));

		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		// Delete all the folders and their children
		error = sEntry->DeleteEntries(*selection);

		theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);

		if (error == KErrNone)
			{
			finished = ETrue;

			// Check there were no failures
			test(selection->Count() == 0);
			}
		else
			{
			test(error == KErrGeneral);

			// Check failed entries still exist
			TInt count = 0;
			while(count < selection->Count())
				REPORT_IF_ERROR(sEntry->SetEntry(selection->At(count++)));
			}
		}

	CleanupStack::PopAndDestroy(2); // selection, sEntry
	}

LOCAL_C void TestMoveSelSyncWithHeapFailL()
	{

	TMsvEntry folder[KFolderCount];
	TMsvEntry entry[KFolderCount][KEntriesPerFolder];

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	CMsvEntrySelection* children = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(children);

	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	TMsvEntry folder1;
	folder1.iDetails.Set(KLongDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder1));

	TMsvEntry folder2;
	folder2.iDetails.Set(KLongDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder2));

	// Create the folders
	for(TInt fc = 0; fc < KFolderCount; fc++)
		{
		REPORT_IF_ERROR(sEntry->SetEntry(folder1.Id()));

		folder[fc].iDetails.Set(KLongDetails);
		folder[fc].iType = KUidMsvFolderEntry;
		folder[fc].iServiceId = KMsvLocalServiceIndexEntryId;
		folder[fc].iMtm = KUidMsvLocalServiceMtm;
		REPORT_IF_ERROR(sEntry->CreateEntry(folder[fc]));
		
		// Add folder to list of things to delete
		selection->AppendL(folder[fc].Id());
		REPORT_IF_ERROR(sEntry->SetEntry(folder[fc].Id()));

		// Create the entries
		for(TInt ec = 0; ec < KEntriesPerFolder; ec++)
			{
			entry[fc][ec].iType = KUidMsvMessageEntry;
			entry[fc][ec].iServiceId = KMsvLocalServiceIndexEntryId;
			entry[fc][ec].iMtm = KUidMsvLocalServiceMtm;
			REPORT_IF_ERROR(sEntry->CreateEntry(entry[fc][ec]));
			}
		}

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	
	TInt error = KErrNone;

	while(!finished)
		{
		REPORT_IF_ERROR(sEntry->SetEntry(folder1.Id()));
	
		__UHEAP_FAILNEXT(failCount++);

		// Move all the folders and their children
		error = sEntry->MoveEntriesWithinService(*selection, folder2.Id());

		__UHEAP_RESET;

		if (error == KErrNone)
			{
			finished = ETrue;

			// Should be no failures
			test(selection->Count() == 0);

			// Check all entries have been moved
			REPORT_IF_ERROR(sEntry->SetEntry(folder2.Id()));
			sEntry->GetChildren(*children);
			test(children->Count() == KFolderCount);
			}
		else
			{
			test(error == KErrNoMemory);

			// Check correct number of entries moved
			REPORT_IF_ERROR(sEntry->SetEntry(folder2.Id()));
			sEntry->GetChildren(*children);
			test(children->Count() + selection->Count() == KFolderCount);
			}
		}

	CleanupStack::PopAndDestroy(3); // selection, children, sEntry
	}


LOCAL_C void TestMoveSelSyncWithFileFailL()
	{

	TMsvEntry folder[KFolderCount];
	TMsvEntry entry[KFolderCount][KEntriesPerFolder];

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	CMsvEntrySelection* children = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(children);

	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	TMsvEntry folder1;
	folder1.iDetails.Set(KLongDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder1));

	TMsvEntry folder2;
	folder2.iDetails.Set(KLongDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder2));

	// Create the folders
	for(TInt fc = 0; fc < KFolderCount; fc++)
		{
		REPORT_IF_ERROR(sEntry->SetEntry(folder1.Id()));

		folder[fc].iDetails.Set(KLongDetails);
		folder[fc].iType = KUidMsvFolderEntry;
		folder[fc].iServiceId = KMsvLocalServiceIndexEntryId;
		folder[fc].iMtm = KUidMsvLocalServiceMtm;
		REPORT_IF_ERROR(sEntry->CreateEntry(folder[fc]));
		
		// Add folder to list of things to delete
		selection->AppendL(folder[fc].Id());
		REPORT_IF_ERROR(sEntry->SetEntry(folder[fc].Id()));

		// Create the entries
		for(TInt ec = 0; ec < KEntriesPerFolder; ec++)
			{
			entry[fc][ec].iType = KUidMsvMessageEntry;
			entry[fc][ec].iServiceId = KMsvLocalServiceIndexEntryId;
			entry[fc][ec].iMtm = KUidMsvLocalServiceMtm;
			REPORT_IF_ERROR(sEntry->CreateEntry(entry[fc][ec]));
			}
		}

	TBool finished = EFalse;
	TInt failCount = 0;
	TInt error = KErrNone;

	while(!finished)
		{
		REPORT_IF_ERROR(sEntry->SetEntry(folder1.Id()));
		if(failCount==18)
			failCount++;
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		// Move all the folders and their children
		error = sEntry->MoveEntriesWithinService(*selection, folder2.Id());

		theUtils->FileSession().SetErrorCondition(KErrGeneral, 32767);

		if (error == KErrNone)
			{
			finished = ETrue;

			// Should be no failures
			test(selection->Count() == 0);

			// Check all entries have been moved
			REPORT_IF_ERROR(sEntry->SetEntry(folder2.Id()));
			sEntry->GetChildren(*children);
			test(children->Count() == KFolderCount);
			}
		else
			{
			test(error == KErrGeneral);

			// Check correct number of entries moved
			REPORT_IF_ERROR(sEntry->SetEntry(folder2.Id()));
			sEntry->GetChildren(*children);
			test(children->Count() + selection->Count() == KFolderCount);
			}
		}

	CleanupStack::PopAndDestroy(3); // selection, children, sEntry
	}

LOCAL_C void TestConnectedFlagWithHeapFileFailuresL()
	{
	CMsvServerEntry* sEntry1 = sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	
	// create entry to use
	TMsvEntry entry;
	entry.iDetails.Set(KLongDetails);
	entry.iType = KUidMsvFolderEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry));
	REPORT_IF_ERROR(sEntry1->SetEntry(entry.Id()));
	
	// change connected
	entry.SetConnected(ETrue);
	REPORT_IF_ERROR(sEntry1->ChangeEntry(entry));
	test(sEntry1->Entry()==entry);
	entry.SetConnected(EFalse);
	REPORT_IF_ERROR(sEntry1->ChangeEntry(entry));
	test(sEntry1->Entry()==entry);

	// change connected with heap failure
	entry.SetConnected(ETrue);
	REPORT_IF_ERROR(sEntry1->ChangeEntry(entry));
	test(sEntry1->Entry()==entry);
	entry.SetConnected(EFalse);
	__UHEAP_FAILNEXT(0);
	REPORT_IF_ERROR(sEntry1->ChangeEntry(entry));
	test(sEntry1->Entry()==entry);
	__UHEAP_RESET;

	// change connected with file failure
	entry.SetConnected(ETrue);
	REPORT_IF_ERROR(sEntry1->ChangeEntry(entry));
	test(sEntry1->Entry()==entry);
	entry.SetConnected(EFalse);
	theUtils->FileSession().SetErrorCondition(KErrGeneral,0);
	REPORT_IF_ERROR(sEntry1->ChangeEntry(entry));
	test(sEntry1->Entry()==entry);
	theUtils->FileSession().SetErrorCondition(KErrGeneral,10000);

	delete sEntry1;
	}

void doTestsL()
	{
	theUtils->CleanMessageFolderL(KMsvTestIndexFile);

	theServer = CMsgsTestUtils::CreateServerL();
	CleanupStack::PushL(theServer);

	test.Printf(_L("*** Testing for heap failure ***\r\n"));

	CMsgsTestUtils::Start(_L("Construction"));
	TestConstructorWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Switching context"));
	TestSwitchingWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Changing entries"));
	TestChangingWithHeapFailL();
	CMsgsTestUtils::Complete();


	CMsgsTestUtils::Next(_L("Deleting")); 
	TestDeletingWithHeapFailL();
	TestDeleteAllWithHeapFailL();
	TestDeletingWithStoresFilesWithHeapFailL(); 
	TestDeleteSelWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Child selection"));
	TestChildrenSelectionWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Owner flag"));
	TestOwnerFlagWithHeapFailL(); 
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Store flag"));
	TestStoreFlagWithHeapFailL(); 
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Moving"));
	TestMovingSyncWithHeapFailL();
	TestMovingSyncWithStoresWithHeapFailL();
	TestMoveSelSyncWithHeapFailL();
	TestMovingAsyncWithHeapFailL();
	TestMovingSelAsyncWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Copying"));
	TestCopyingAsyncWithHeapFailL();
// The following test is commented out intentionally.
//	TestCopyingSelAsyncWithHeapFailL(); 
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("PC Synching"));
	TestPCSynchedEntryDeletionWithHeapFailL(); 
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Stores"));
	TestStoresWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Local store"));
	TestLocalStoreFlagWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Remote store")); 
	TestRemoteStoreFlagWithHeapFailL();	
	CMsgsTestUtils::Complete();
	
	test.Printf(_L("*** Testing for file failure ***\r\n"));

	CMsgsTestUtils::Next(_L("Construction"));
	TestConstructorWithFileFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Changing entries"));
	TestChangingWithFileFailL();
	CMsgsTestUtils::Complete();


	CMsgsTestUtils::Next(_L("Deleting")); 
	TestDeletingWithFileFailL();
	TestDeleteAllWithFileFailL();
	TestDeleteSelWithFileFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Owner flag"));
	TestOwnerFlagWithFileFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Store flag"));
	TestStoreFlagWithFileFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Moving"));
	TestMoveSelSyncWithFileFailL();
//	TestMovingAsyncWithFileFailL();
//	TestMovingSelAsyncWithFileFailL();
	CMsgsTestUtils::Complete();

//	CMsgsTestUtils::Next(_L("Copying"));
//	TestCopyingAsyncWithFileFailL();
//	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Stores"));
	TestStoresWithFileFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Local store"));
	TestLocalStoreFlagWithFileFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Remote store")); 
	TestRemoteStoreFlagWithFileFailL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Connected flag"));
	TestConnectedFlagWithHeapFileFailuresL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("ReadStoreL Heap failure"));
	TestReadStoreWithHeapFailL();
	CMsgsTestUtils::Complete();
	
	CleanupStack::PopAndDestroy(); // theServer
	}

#pragma warning (default : 4238)
