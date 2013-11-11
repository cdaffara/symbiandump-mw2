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

#include <msvids.h>

#include "msvindexadapter.h"
#include <msventry.h>
#include "MSERVER.H"
#include "msvutils.h"
#include "Testsetup.h"

RTest test(_L("Server Entry Testrig"));
CPermanentFileStore* dummyStore = NULL;
CMsvServer* theServer;

_LIT(KMsvTestIndexFile, "\\Server MTM testing\\Index");
/**
pre 1189 the message server used to create it's data files under the path passed
to it. From 1189 implementation onwards \\private\\1000484b\\Mail2\\ is taken as
the root for files.
*/
_LIT(KMsvTestFolder, "\\private\\1000484b\\Mail2\\");
_LIT(KShortDescription, "Description");
_LIT(KShortDetails, "Details");
_LIT(KLongDescription, "A very long description which goes over thirty two characters but is now needed to go over 64");
_LIT(KLongDetails, "A very long details string containing rubbish and junk but has more than sixty four characters");

const TUid KMsvTestUid1 = {0x10200000};
const TUid KMsvTestUid2 = {0x10200001};

const TInt KFolderCount = 5;
const TInt KEntriesPerFolder = 3;

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
	iStatus = KRequestPending;
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

LOCAL_C void GetServicePath(TMsvId aService, TFileName& aFileName)
	{
	aFileName = KMsvTestFolder;
	MsvUtils::ConstructEntryName(aService, aService, aFileName, MsvUtils::EPath);
	}


LOCAL_C void TestConstructorL()
	{
	CMsvServerEntry* sEntry1=NULL;
	CMsvServerEntry* sEntry2=NULL;

	TInt leave;
	
	// create folder to work under
	sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	TMsvEntry folder1;
	folder1.iDescription.Set(KLongDescription);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	delete sEntry1; sEntry1=NULL;

	//
	// create and destroy a CMsvServerEntry
	sEntry1 = CMsvServerEntry::NewL(*theServer, folder1.Id());
	test(sEntry1->Entry()==folder1);
	delete sEntry1; sEntry1=NULL;

	//
	// try to create with locked contents
	sEntry1 = CMsvServerEntry::NewL(*theServer, folder1.Id());
	test(sEntry1->Entry()==folder1);
	TRAP(leave, sEntry2 = CMsvServerEntry::NewL(*theServer, folder1.Id()));
	test(leave==KErrLocked);	
	test(sEntry1->Entry()==folder1);
	delete sEntry1; sEntry1=NULL;
	delete sEntry2; sEntry2=NULL;
	
	//
	// try to create non-existant entry
	TRAP(leave, sEntry1 = CMsvServerEntry::NewL(*theServer, -2345));
	test(leave==KErrNotFound);

	}


LOCAL_C void TestSwitchingL()
	{
	CMsvServerEntry* sEntry1=NULL;
	CMsvServerEntry* sEntry2=NULL;

	// create folder to use
	sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	TMsvEntry folder1;
	folder1.iDetails.Set(KLongDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	delete sEntry1; sEntry1=NULL;

	// get folder
	sEntry1 = CMsvServerEntry::NewL(*theServer, folder1.Id());
	test(sEntry1->Entry()==folder1);

	// switch to non-existant folder
	test(sEntry1->SetEntry(-2134)==KErrNotFound);
	test(sEntry1->Entry()==folder1);
	
	// switch to itself
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	test(sEntry1->Entry()==folder1);

	//try to switch to lock entry
	sEntry2 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	test(sEntry2->SetEntry(folder1.Id())==KErrLocked);
	test(sEntry1->Entry()==folder1);
	delete sEntry2; sEntry2=NULL;


	// loose folder
	delete sEntry1; sEntry1=NULL;
	}	


LOCAL_C void TestChangingL()
	{
	CMsvServerEntry* sEntry1 = sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	
	// create folder to use
	TMsvEntry folder1;
	folder1.iDetails.Set(KLongDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));

	// switch to folder
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	test(sEntry1->Entry()==folder1);
	
	// change entry
	folder1.SetVisible(!folder1.Visible());
	REPORT_IF_ERROR(sEntry1->ChangeEntry(folder1));
	test(sEntry1->Entry()==folder1);
	folder1.iSize = 23435+folder1.iSize;
	REPORT_IF_ERROR(sEntry1->ChangeEntry(folder1));
	test(sEntry1->Entry()==folder1);

	// change strings
	folder1.iDetails.Set(TPtrC());
	REPORT_IF_ERROR(sEntry1->ChangeEntry(folder1));
	test(sEntry1->Entry()==folder1);
	folder1.iDescription.Set(KLongDescription);
	REPORT_IF_ERROR(sEntry1->ChangeEntry(folder1));
	test(sEntry1->Entry()==folder1);
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KLongDescription);
	REPORT_IF_ERROR(sEntry1->ChangeEntry(folder1));
	test(sEntry1->Entry()==folder1);
	folder1.iDescription.Set(KLongDescription);
	folder1.iDetails.Set(TPtrC());
	REPORT_IF_ERROR(sEntry1->ChangeEntry(folder1));
	test(sEntry1->Entry()==folder1);

	// change read only flag
	folder1.iSize = 23435+folder1.iSize;
	folder1.SetReadOnly(ETrue);
	REPORT_IF_ERROR(sEntry1->ChangeEntry(folder1));
	test(sEntry1->Entry()==folder1);
	//
	folder1.iSize = 23435+folder1.iSize;
	REPORT_IF_ERROR(sEntry1->ChangeEntry(folder1));
	test(sEntry1->Entry()==folder1);
	//
	folder1.iSize = 23435+folder1.iSize;
	folder1.SetReadOnly(EFalse);
	REPORT_IF_ERROR(sEntry1->ChangeEntry(folder1));
	test(sEntry1->Entry()==folder1);
	
	// change to standard entry
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvGlobalInBoxIndexEntryId));
	TMsvEntry inbox = sEntry1->Entry();
	inbox.iSize = 23435+inbox.iSize;
	test(sEntry1->ChangeEntry(inbox)==KErrAccessDenied);
	test(!(sEntry1->Entry()==inbox));
	inbox.SetStandardFolder(EFalse);
	test(sEntry1->ChangeEntry(inbox)==KErrAccessDenied);
	test(!(sEntry1->Entry()==inbox));


	delete sEntry1; sEntry1=NULL;
	}



LOCAL_C void TestChangingServerFlagsL()
	{
	CMsvServerEntry* sEntry1 = sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	
	// create folder to use
	TTestMsvEntry folder1;
	folder1.iDetails.Set(KLongDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));

	// switch to folder
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	test(sEntry1->Entry()==folder1);

	// change entry trying to set the deleted flag
	folder1.SetDeleted(ETrue);
	folder1.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(sEntry1->ChangeEntry(folder1));
	test(sEntry1->Entry().iDetails==KLongDetails);
	test(!sEntry1->Entry().Deleted());
	test(folder1.Deleted());

	// create remote folder to use
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid2;
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
	TMsvEntry folderR;
	folderR.iType = KUidMsvFolderEntry;
	folderR.iServiceId = service.Id();
	folderR.iMtm = KMsvTestUid2;
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderR));
	REPORT_IF_ERROR(sEntry1->SetEntry(folderR.Id()));

	// set the store flag
	CMsvStore* store = sEntry1->EditStoreL();
	RMsvWriteStream out;
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	test(sEntry1->HasStoreL());
	delete store;
		
	// change entry
	folderR.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(sEntry1->ChangeEntry(folderR));
	test(sEntry1->Entry().iDetails==KLongDetails);
	test(sEntry1->HasStoreL());
	
	// create child
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	test(sEntry1->Entry().Owner());
	test(!folderR.Owner());

	// change entry
	folderR.iDetails.Set(KShortDetails);
	REPORT_IF_ERROR(sEntry1->ChangeEntry(folderR));
	test(sEntry1->Entry().iDetails==KShortDetails);
	test(sEntry1->Entry().Owner());
	test(!folderR.Owner());


	
	delete sEntry1;
	}


LOCAL_C void TestChangingRootL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvRootIndexEntryId);
	
	// change the store flag
	TBool hasStore=sEntry->HasStoreL();
	if (hasStore)
		{
		// delete the store
		CMsvStore* store = sEntry->EditStoreL();
		store->DeleteL();
		delete store;
		}
	else
		{
		// create the store
		CMsvStore* store = sEntry->EditStoreL();
		RMsvWriteStream out;
		out.AssignLC(*store,KNullUid);
		out.WriteInt32L(1234);
		out.CommitL();
		CleanupStack::PopAndDestroy(&out);
		store->CommitL();
		delete store;
		}

	// test the flag has been changed
	test((hasStore && !sEntry->HasStoreL()) || (!hasStore && sEntry->HasStoreL()));
	// test the flag has been changed on the server
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvRootIndexEntryId));
	test((hasStore && !sEntry->HasStoreL()) || (!hasStore && sEntry->HasStoreL()));

	delete sEntry;
	}

LOCAL_C void TestChangingWithStoreOpenL()
	{
	CMsvServerEntry* sEntry1 = sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CMsvStore* store;

	// create entry to use
	TMsvEntry entry1;
	entry1.iDetails.Set(KLongDetails);
	entry1.iType = KUidMsvFolderEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	// switch to entry
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	test(sEntry1->Entry()==entry1);
	// open the store
	store = sEntry1->EditStoreL();
	// change entry
	entry1.SetVisible(EFalse);
	REPORT_IF_ERROR(sEntry1->ChangeEntry(entry1));
	// close
	delete store;


	// create entry to use
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	TMsvEntry entry2;
	entry2.iDetails.Set(KLongDetails);
	entry2.iType = KUidMsvFolderEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	// switch to entry
	REPORT_IF_ERROR(sEntry1->SetEntry(entry2.Id()));
	test(sEntry1->Entry()==entry2);
	// open the store
	store = sEntry1->EditStoreL();
	// change entry
	entry1.SetVisible(EFalse);
	REPORT_IF_ERROR(sEntry1->ChangeEntry(entry2));
	// change strings
	entry2.iDetails.Set(TPtrC());
	REPORT_IF_ERROR(sEntry1->ChangeEntry(entry2));
	// change entry
	entry1.SetComplete(!entry1.Complete());
	REPORT_IF_ERROR(sEntry1->ChangeEntry(entry2));
	// close
	delete store;
	

	// create service to use
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	TMsvEntry service;
	service.iDetails.Set(KLongDetails);
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	// switch to entry
	REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
	test(sEntry1->Entry()==service);
	// open the store
	store = sEntry1->EditStoreL();
	// change strings
	service.iDetails.Set(TPtrC());
	REPORT_IF_ERROR(sEntry1->ChangeEntry(service));
	// close
	delete store;

	// create remote entry to use
	REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
	TMsvEntry entryR;
	entryR.iDetails.Set(KLongDetails);
	entryR.iType = KUidMsvFolderEntry;
	entryR.iMtm = KMsvTestUid1;
	entryR.iServiceId = service.Id();
	REPORT_IF_ERROR(sEntry1->CreateEntry(entryR));
	// switch to entry
	REPORT_IF_ERROR(sEntry1->SetEntry(entryR.Id()));
	test(sEntry1->Entry()==entryR);
	// open the store
	store = sEntry1->EditStoreL();
	// change strings
	entryR.iDetails.Set(TPtrC());
	REPORT_IF_ERROR(sEntry1->ChangeEntry(entryR));
	// close
	delete store;
	
	delete sEntry1; sEntry1=NULL;
	}


LOCAL_C void TestCreatingDeletingL()
	{
	CMsvServerEntry* sEntry1=NULL;
	CMsvServerEntry* sEntry2=NULL;

	TMsvId id1,id2,id3;
	TMsvId id;

	// create folder to use
	sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	TMsvEntry folder1;
	folder1.iDetails.Set(KLongDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));

	//
	// create a child then delete it
	TMsvEntry entry1;
	entry1.iDescription.Set(KLongDescription);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	// get child
	sEntry2 = CMsvServerEntry::NewL(*theServer, entry1.Id());
	test(sEntry2->Entry()==entry1);
	// delete the child while locked
	test(sEntry1->DeleteEntry(entry1.Id())==KErrLocked);
	// delete the child
	delete sEntry2; sEntry2=NULL;
	REPORT_IF_ERROR(sEntry1->DeleteEntry(entry1.Id()));

	//
	// create a child then delete it
	entry1.iDetails.Set(KLongDetails);
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	// get child
	sEntry2 = CMsvServerEntry::NewL(*theServer, entry1.Id());
	test(sEntry2->Entry()==entry1);
	// delete the child while locked
	test(sEntry1->DeleteEntry(entry1.Id())==KErrLocked);
	// delete the child
	delete sEntry2; sEntry2=NULL;
	REPORT_IF_ERROR(sEntry1->DeleteEntry(entry1.Id()));

	//
	// create a child then delete it
	entry1.iDetails.Set(KLongDetails);
	entry1.iDescription.Set(KLongDescription);
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	// get child
	sEntry2 = CMsvServerEntry::NewL(*theServer, entry1.Id());
	test(sEntry2->Entry()==entry1);
	// delete the child while locked
	test(sEntry1->DeleteEntry(entry1.Id())==KErrLocked);
	// delete the child
	delete sEntry2; sEntry2=NULL;
	REPORT_IF_ERROR(sEntry1->DeleteEntry(entry1.Id()));

	//
	// create three children and delete
	entry1.iDetails.Set(KLongDetails);
	entry1.iDescription.Set(KLongDescription);
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	id1 = entry1.Id();
	entry1.iDetails.Set(KShortDetails);
	entry1.iDescription.Set(KLongDescription);
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	id2 = entry1.Id();
	entry1.iDetails.Set(KLongDetails);
	entry1.iDescription.Set(TPtrC());
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	id3 = entry1.Id();
	//
	// child 2
	id = id2;
	sEntry2 = CMsvServerEntry::NewL(*theServer, id);
	test(sEntry2->Entry().Id()==id);
	test(sEntry2->Entry().iDetails==KShortDetails);
	test(sEntry2->Entry().iDescription==KLongDescription);
	// delete the child while locked
	test(sEntry1->DeleteEntry(id)==KErrLocked);
	// delete the child
	delete sEntry2; sEntry2=NULL;
	REPORT_IF_ERROR(sEntry1->DeleteEntry(id));
	//
	// child 3
	id = id3;
	sEntry2 = CMsvServerEntry::NewL(*theServer, id);
	test(sEntry2->Entry().Id()==id);
	test(sEntry2->Entry().iDetails==KLongDetails);
	test(sEntry2->Entry().iDescription==TPtrC());
	// delete the child while locked
	test(sEntry1->DeleteEntry(id)==KErrLocked);
	// delete the child
	delete sEntry2; sEntry2=NULL;
	REPORT_IF_ERROR(sEntry1->DeleteEntry(id));
	//
	// child 1
	id = id1;
	sEntry2 = CMsvServerEntry::NewL(*theServer, id);
	test(sEntry2->Entry().Id()==id);
	test(sEntry2->Entry().iDetails==KLongDetails);
	test(sEntry2->Entry().iDescription==KLongDescription);
	// delete the child while locked
	test(sEntry1->DeleteEntry(id)==KErrLocked);
	// delete the child
	delete sEntry2; sEntry2=NULL;
	REPORT_IF_ERROR(sEntry1->DeleteEntry(id));
	
	//
	// create three children and delete
	entry1.iDetails.Set(KLongDetails);
	entry1.iDescription.Set(KLongDescription);
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	id1 = entry1.Id();
	entry1.iDetails.Set(KShortDetails);
	entry1.iDescription.Set(KLongDescription);
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	id2 = entry1.Id();
	entry1.iDetails.Set(KLongDetails);
	entry1.iDescription.Set(TPtrC());
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	id3 = entry1.Id();
	//
	// child 3
	id = id3;
	sEntry2 = CMsvServerEntry::NewL(*theServer, id);
	test(sEntry2->Entry().Id()==id);
	test(sEntry2->Entry().iDetails==KLongDetails);
	test(sEntry2->Entry().iDescription==TPtrC());
	// delete the child while locked
	test(sEntry1->DeleteEntry(id)==KErrLocked);
	// delete the child
	delete sEntry2; sEntry2=NULL;
	REPORT_IF_ERROR(sEntry1->DeleteEntry(id));
	//
	// child 1
	id = id1;
	sEntry2 = CMsvServerEntry::NewL(*theServer, id);
	test(sEntry2->Entry().Id()==id);
	test(sEntry2->Entry().iDetails==KLongDetails);
	test(sEntry2->Entry().iDescription==KLongDescription);
	// delete the child while locked
	test(sEntry1->DeleteEntry(id)==KErrLocked);
	// delete the child
	delete sEntry2; sEntry2=NULL;
	REPORT_IF_ERROR(sEntry1->DeleteEntry(id));
	//
	// child 2
	id = id2;
	sEntry2 = CMsvServerEntry::NewL(*theServer, id);
	test(sEntry2->Entry().Id()==id);
	test(sEntry2->Entry().iDetails==KShortDetails);
	test(sEntry2->Entry().iDescription==KLongDescription);
	// delete the child while locked
	test(sEntry1->DeleteEntry(id)==KErrLocked);
	// delete the child
	delete sEntry2; sEntry2=NULL;
	REPORT_IF_ERROR(sEntry1->DeleteEntry(id));

	// try to delete a standard folder
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	test(sEntry1->DeleteEntry(KMsvGlobalInBoxIndexEntryId)==KErrAccessDenied);

	delete sEntry1; sEntry1=NULL;
	}


	
LOCAL_C void TestDeleteAllL()	
	{
	CMsvServerEntry* sEntry1=NULL;
	CMsvServerEntry* sEntry2=NULL;

	TMsvEntry folder1;
	TMsvEntry entry1;
	TMsvEntry entry2;
	TMsvEntry entry3;
	TMsvEntry entry4;

	sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);

	// create folder to use & create several children
	folder1.iDetails.Set(KLongDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	// 
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	entry3.iType = KUidMsvMessageEntry;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry3));
	entry4.iType = KUidMsvMessageEntry;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry4));

	//
	// delete all 
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->DeleteEntry(folder1.Id()));
	
	// create folder to use & create several children
	folder1.iDetails.Set(KLongDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	// 
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	entry3.iType = KUidMsvMessageEntry;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry3));
	REPORT_IF_ERROR(sEntry1->SetEntry(entry3.Id()));
	entry4.iType = KUidMsvMessageEntry;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry4));

	// lock one entry 
	sEntry2 = CMsvServerEntry::NewL(*theServer, entry2.Id());
	// delete all
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	test(sEntry1->DeleteEntry(folder1.Id())==KErrLocked);
	// unlock entry
	delete sEntry2; sEntry2=NULL;

	// lock one entry 
	sEntry2 = CMsvServerEntry::NewL(*theServer, folder1.Id());
	// delete all
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	test(sEntry1->DeleteEntry(folder1.Id())==KErrLocked);
	// unlock entry
	delete sEntry2; sEntry2=NULL;

	// lock one entry 
	sEntry2 = CMsvServerEntry::NewL(*theServer, entry4.Id());
	// delete all
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	test(sEntry1->DeleteEntry(folder1.Id())==KErrLocked);
	// unlock entry
	delete sEntry2; sEntry2=NULL;

	// delete all
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->DeleteEntry(folder1.Id()));
	
	// create folder to use & create several children (one is read only)
	folder1.iDetails.Set(KLongDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	// 
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	entry3.iType = KUidMsvMessageEntry;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.SetReadOnly(ETrue);
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry3));
	entry4.iType = KUidMsvMessageEntry;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry4));

	// delete all
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->DeleteEntry(folder1.Id()));
	
	delete sEntry1;
	}
	

LOCAL_C void TestChidlrenSelectionL()
	{
	CMsvServerEntry* sEntry1=NULL;

	TMsvEntry folder1;
	TMsvEntry entry1;
	TMsvEntry entry2;
	TMsvEntry entry3;
	TMsvEntry entry4;

	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection;

	//
	// sort order
	sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	TMsvSelectionOrdering order;
	test(sEntry1->Sort()==order);
	order.SetGroupByPriority(ETrue);
	order.SetSorting(EMsvSortByDescription);
	sEntry1->SetSort(order);
	test(sEntry1->Sort()==order);
	delete sEntry1; sEntry1=NULL;
	

	// create folder to use & create several children
	sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	// 
	entry1.iSize = 100;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	entry2.iSize = 10000;
	entry2.iType = KUidMsvFolderEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	entry3.iSize = 10;
	entry3.iType = KUidMsvMessageEntry;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry3));
	entry4.iSize = 1000;
	entry4.SetPriority(EMsvHighPriority);
	entry4.iType = KUidMsvMessageEntry;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry4));
	
	//
	// test the selections
	order.SetGroupByType(EFalse);
	order.SetGroupByPriority(EFalse);
	order.SetGroupByMtm(EFalse);
	order.SetSorting(EMsvSortByNone);
	selection->Reset();
	sEntry1->SetSort(order);
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry2.Id());
	test(selection->At(2)==entry3.Id());
	test(selection->At(3)==entry4.Id());
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithService(KMsvLocalServiceIndexEntryId, *selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry2.Id());
	test(selection->At(2)==entry3.Id());
	test(selection->At(3)==entry4.Id());
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithService(0x123, *selection));
	test(selection->Count()==0);
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithMtm(KUidMsvLocalServiceMtm, *selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry2.Id());
	test(selection->At(2)==entry3.Id());
	test(selection->At(3)==entry4.Id());
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithMtm(KMsvTestUid1, *selection));
	test(selection->Count()==0);
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithType(KUidMsvMessageEntry, *selection));
	test(selection->Count()==3);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry3.Id());
	test(selection->At(2)==entry4.Id());
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithType(KUidMsvFolderEntry, *selection));
	test(selection->Count()==1);
	test(selection->At(0)==entry2.Id());
	//
	order.SetGroupByType(EFalse);
	order.SetGroupByPriority(EFalse);
	order.SetGroupByMtm(EFalse);
	order.SetSorting(EMsvSortBySize);
	selection->Reset();
	sEntry1->SetSort(order);
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry3.Id());
	test(selection->At(1)==entry1.Id());
	test(selection->At(2)==entry4.Id());
	test(selection->At(3)==entry2.Id());
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithService(KMsvLocalServiceIndexEntryId, *selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry3.Id());
	test(selection->At(1)==entry1.Id());
	test(selection->At(2)==entry4.Id());
	test(selection->At(3)==entry2.Id());
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithService(0x123, *selection));
	test(selection->Count()==0);
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithMtm(KUidMsvLocalServiceMtm, *selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry3.Id());
	test(selection->At(1)==entry1.Id());
	test(selection->At(2)==entry4.Id());
	test(selection->At(3)==entry2.Id());
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithMtm(KMsvTestUid1, *selection));
	test(selection->Count()==0);
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithType(KUidMsvMessageEntry, *selection));
	test(selection->Count()==3);
	test(selection->At(0)==entry3.Id());
	test(selection->At(1)==entry1.Id());
	test(selection->At(2)==entry4.Id());
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithType(KUidMsvFolderEntry, *selection));
	test(selection->Count()==1);
	test(selection->At(0)==entry2.Id());
	//
	order.SetGroupByType(ETrue);
	order.SetGroupByPriority(EFalse);
	order.SetGroupByMtm(EFalse);
	order.SetSorting(EMsvSortBySize);
	selection->Reset();
	sEntry1->SetSort(order);
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry2.Id());
	test(selection->At(1)==entry3.Id());
	test(selection->At(2)==entry1.Id());
	test(selection->At(3)==entry4.Id());
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithService(KMsvLocalServiceIndexEntryId, *selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry2.Id());
	test(selection->At(1)==entry3.Id());
	test(selection->At(2)==entry1.Id());
	test(selection->At(3)==entry4.Id());
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithService(0x123, *selection));
	test(selection->Count()==0);
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithMtm(KUidMsvLocalServiceMtm, *selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry2.Id());
	test(selection->At(1)==entry3.Id());
	test(selection->At(2)==entry1.Id());
	test(selection->At(3)==entry4.Id());
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithMtm(KMsvTestUid1, *selection));
	test(selection->Count()==0);
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithType(KUidMsvMessageEntry, *selection));
	test(selection->Count()==3);
	test(selection->At(0)==entry3.Id());
	test(selection->At(1)==entry1.Id());
	test(selection->At(2)==entry4.Id());
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithType(KUidMsvFolderEntry, *selection));
	test(selection->Count()==1);
	test(selection->At(0)==entry2.Id());
	//
	order.SetGroupByType(ETrue);
	order.SetGroupByPriority(ETrue);
	order.SetGroupByMtm(EFalse);
	order.SetSorting(EMsvSortBySize);
	selection->Reset();
	sEntry1->SetSort(order);
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->At(0)==entry2.Id());
	test(selection->At(1)==entry4.Id());
	test(selection->At(2)==entry3.Id());
	test(selection->At(3)==entry1.Id());

	REPORT_IF_ERROR(sEntry1->GetChildrenWithService(KMsvLocalServiceIndexEntryId, *selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry2.Id());
	test(selection->At(1)==entry4.Id());
	test(selection->At(2)==entry3.Id());
	test(selection->At(3)==entry1.Id());
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithService(0x123, *selection));
	test(selection->Count()==0);
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithMtm(KUidMsvLocalServiceMtm, *selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry2.Id());
	test(selection->At(1)==entry4.Id());
	test(selection->At(2)==entry3.Id());
	test(selection->At(3)==entry1.Id());
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithMtm(KMsvTestUid1, *selection));
	test(selection->Count()==0);
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithType(KUidMsvMessageEntry, *selection));
	test(selection->Count()==3);
	test(selection->At(0)==entry4.Id());
	test(selection->At(1)==entry3.Id());
	test(selection->At(2)==entry1.Id());
	//
	REPORT_IF_ERROR(sEntry1->GetChildrenWithType(KUidMsvFolderEntry, *selection));
	test(selection->Count()==1);
	test(selection->At(0)==entry2.Id());
	//
	delete selection;
	delete sEntry1;
	}

LOCAL_C void TestInvisibleSelectionL()
	{
	CMsvServerEntry* sEntry1=NULL;

	TMsvEntry folder1;
	TMsvEntry entry1;
	TMsvEntry entry2;
	TMsvEntry entry3;
	TMsvEntry entry4;

	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection;


	// create folder to use & create several children
	sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	// 
	entry1.iSize = 100;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.SetVisible(EFalse);
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	entry2.iSize = 10000;
	entry2.iType = KUidMsvFolderEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.SetVisible(ETrue);
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	entry3.iSize = 10;
	entry3.iType = KUidMsvMessageEntry;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.SetVisible(ETrue);
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry3));
	entry4.iSize = 1000;
	entry4.SetPriority(EMsvHighPriority);
	entry4.iType = KUidMsvMessageEntry;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	entry4.SetVisible(EFalse);
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry4));


	//
	// test
	TMsvSelectionOrdering order1(KMsvNoGrouping, EMsvSortByNone);
	sEntry1->SetSort(order1);
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==2);
	test(selection->At(0)==entry2.Id());
	test(selection->At(1)==entry3.Id());
	// invisible
	TMsvSelectionOrdering order2(KMsvNoGrouping, EMsvSortByNone, ETrue);
	sEntry1->SetSort(order2);
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry2.Id());
	test(selection->At(2)==entry3.Id());
	test(selection->At(3)==entry4.Id());
	// grouping 
	TMsvSelectionOrdering order3(KMsvGroupByType|KMsvGroupByPriority, EMsvSortByNone, EFalse);
	sEntry1->SetSort(order3);
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==2);
	test(selection->At(0)==entry2.Id());
	test(selection->At(1)==entry3.Id());
	// sorting 
	TMsvSelectionOrdering order4(KMsvNoGrouping, EMsvSortBySize, EFalse);
	sEntry1->SetSort(order4);
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==2);
	test(selection->At(0)==entry3.Id());
	test(selection->At(1)==entry2.Id());
	// grouping - invisible
	TMsvSelectionOrdering order5(KMsvGroupByType|KMsvGroupByPriority, EMsvSortByNone, ETrue);
	sEntry1->SetSort(order5);
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry2.Id());
	test(selection->At(1)==entry4.Id());
	test(selection->At(2)==entry1.Id());
	test(selection->At(3)==entry3.Id());
	// sorting 
	TMsvSelectionOrdering order6(KMsvNoGrouping, EMsvSortBySize, ETrue);
	sEntry1->SetSort(order6);
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry3.Id());
	test(selection->At(1)==entry1.Id());
	test(selection->At(2)==entry4.Id());
	test(selection->At(3)==entry2.Id());

	
	delete selection;
	delete sEntry1;
	}


LOCAL_C void TestMtmSelectionL()
	{
	CMsvServerEntry* sEntry1=NULL;

	TMsvEntry folder1;
	TMsvEntry entry1;
	TMsvEntry entry2;
	TMsvEntry entry3;
	TMsvEntry entry4;

	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection;

	TUid uid1={0x100};
	TUid uid2={0x101};
	TUid uid3={0x102};

	// create folder to use & create several children
	sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	// 
	entry1.iSize = 100;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = uid1;
	entry2.iSize = 10000;
	entry2.iType = KUidMsvFolderEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = uid2;
	entry3.iSize = 10;
	entry3.iType = KUidMsvMessageEntry;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	entry3.iMtm = uid2;
	entry4.iSize = 1000;
	entry4.SetPriority(EMsvHighPriority);
	entry4.iType = KUidMsvMessageEntry;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	entry4.iMtm = uid3;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry4));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry3));


	//
	// test
	TMsvSelectionOrdering order(KMsvGroupByMtm, EMsvSortByNone);
	sEntry1->SetSort(order);
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry2.Id());
	test(selection->At(2)==entry3.Id());
	test(selection->At(3)==entry4.Id());
	//
	sEntry1->SetMtm(uid1);
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry2.Id());
	test(selection->At(2)==entry3.Id());
	test(selection->At(3)==entry4.Id());
	//
	sEntry1->SetMtm(uid2);
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry2.Id());
	test(selection->At(1)==entry3.Id());
	test(selection->At(2)==entry1.Id());
	test(selection->At(3)==entry4.Id());
	//
	sEntry1->SetMtm(uid3);
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==4);
	test(selection->At(0)==entry4.Id());
	test(selection->At(1)==entry1.Id());
	test(selection->At(2)==entry2.Id());
	test(selection->At(3)==entry3.Id());
	
	delete selection;
	delete sEntry1;
	}



LOCAL_C void TestStoresL()
	{
	RMsvReadStream readStream;
	RMsvWriteStream writeStream;
	TInt value,leave;
	CMsvStore* store;

	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	// create folder to work under
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry->SetEntry(folder1.Id()));

	// create entry to work with
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));

	// create store for writing
	store = sEntry->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	readStream.OpenL(*store, KMsvTestUid1);
	value = readStream.ReadInt32L();
	test(value==4321);
	readStream.Close();
	store->CommitL();
	delete store; store=NULL;

	// open store for reading
	store = sEntry->ReadStoreL();
	readStream.OpenL(*store, KMsvTestUid1);
	value = readStream.ReadInt32L();
	test(value==4321);
	readStream.Close();
	delete store; store=NULL;

	// try to write to a read store
	store = sEntry->ReadStoreL();
	TRAP(leave, writeStream.AssignL(*store, KMsvTestUid2));
	test(leave==KErrAccessDenied);
	delete store; store=NULL;


	// create read only entry to work with
	TMsvEntry entry2;
	entry2.SetReadOnly(ETrue);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry2));
	REPORT_IF_ERROR(sEntry->SetEntry(entry2.Id()));

	// try to create store
	TRAP(leave, store = sEntry->EditStoreL());
	test(leave==KErrAccessDenied);

	// change from read only
	entry2.SetReadOnly(EFalse);
	REPORT_IF_ERROR(sEntry->ChangeEntry(entry2));

	// create store
	store = sEntry->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(31437);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;

	// change back to read only
	entry2.SetReadOnly(ETrue);
	REPORT_IF_ERROR(sEntry->ChangeEntry(entry2));

	// try to edit store
	TRAP(leave, store = sEntry->EditStoreL());
	test(leave==KErrAccessDenied);

	// try to read store
	store = sEntry->ReadStoreL();
	readStream.OpenL(*store, KMsvTestUid1);
	value = readStream.ReadInt32L();
	test(value==31437);
	readStream.Close();
	delete store; store=NULL;
	
	// change back to write
	entry2.SetReadOnly(EFalse);
	REPORT_IF_ERROR(sEntry->ChangeEntry(entry2));

	CleanupStack::PopAndDestroy(); 
	}


LOCAL_C void TestLocalStoreFlagL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	RMsvWriteStream writeStream;
	CMsvStore* store;

	// create entry to work with
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));

	test(!sEntry->HasStoreL());

	// create store
	store = sEntry->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;
	//
	test(sEntry->HasStoreL());
	// test the server copy
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));
	test(sEntry->HasStoreL());

	// change store
	store = sEntry->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid2);
	writeStream.WriteInt32L(4545);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;
	//
	test(sEntry->HasStoreL());

	// test the server copy
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));
	test(sEntry->HasStoreL());

	// delete the store
	store = sEntry->EditStoreL();
	store->DeleteL();
	delete store; store=NULL;

	// Note: We check that the store exists in a later test

	CleanupStack::PopAndDestroy(); // sEntry
	}


LOCAL_C void TestRemoteStoreFlagL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvRootIndexEntryId);
	CleanupStack::PushL(sEntry);

	RMsvWriteStream writeStream;
	CMsvStore* store;

	// create folder to work under
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	REPORT_IF_ERROR(sEntry->CreateEntry(service));
	REPORT_IF_ERROR(sEntry->SetEntry(service.Id()));

	// create entry to work with
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KMsvTestUid1;
	entry1.iServiceId = service.Id();
	REPORT_IF_ERROR(sEntry->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));

	test(sEntry->HasStoreL()==EFalse);

	// create store
	store = sEntry->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;
	//
	test(sEntry->HasStoreL());
	// test the server copy
	REPORT_IF_ERROR(sEntry->SetEntry(service.Id()));
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));
	test(sEntry->HasStoreL());

	// change store
	store = sEntry->EditStoreL();
	writeStream.AssignL(*store, KMsvTestUid2);
	writeStream.WriteInt32L(4545);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	delete store; store=NULL;
	//
	test(sEntry->HasStoreL());
	// test the server copy
	REPORT_IF_ERROR(sEntry->SetEntry(service.Id()));
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));
	test(sEntry->HasStoreL());


	// delete the store
	store = sEntry->EditStoreL();
	store->DeleteL();
	delete store; store=NULL;
	//
	test(sEntry->HasStoreL()==EFalse);
	// test the server copy
	REPORT_IF_ERROR(sEntry->SetEntry(service.Id()));
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));
	test(sEntry->HasStoreL()==EFalse);

	CleanupStack::PopAndDestroy(); // sEntry
	}


LOCAL_C void TestOwnerFlagL()
	{
	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	TMsvId id1,id2;

	TMsvEntry folder1;
	folder1.iDetails.Set(KLongDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));

	// test
	test(folder1.Owner()==EFalse);

	// create child
	TMsvEntry entry1;
	entry1.iDescription.Set(KLongDescription);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));

	// test
	test(sEntry1->Entry().Owner());
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	test(sEntry1->Entry().Owner());

	// delete child
	REPORT_IF_ERROR(sEntry1->DeleteEntry(entry1.Id()));
	
	// test
	test(sEntry1->Entry().Owner()==EFalse);
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	test(sEntry1->Entry().Owner()==EFalse);

	// create child
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	id1 = entry1.Id();

	// test
	test(sEntry1->Entry().Owner());
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	test(sEntry1->Entry().Owner());

	// create another child
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	id2 = entry1.Id();
	
	// test
	test(sEntry1->Entry().Owner());
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	test(sEntry1->Entry().Owner());

	// delete child
	REPORT_IF_ERROR(sEntry1->DeleteEntry(id1));
	
	// test
	test(sEntry1->Entry().Owner());
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	test(sEntry1->Entry().Owner());

	// delete child
	REPORT_IF_ERROR(sEntry1->DeleteEntry(id2));
	
	// test
	test(sEntry1->Entry().Owner()==EFalse);
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	test(sEntry1->Entry().Owner()==EFalse);

	// create two child
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	id1 = entry1.Id();
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	id2 = entry1.Id();
	
	// test
	test(sEntry1->Entry().Owner());
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	test(sEntry1->Entry().Owner());

	// delete child recursively
	REPORT_IF_ERROR(sEntry1->DeleteEntry(id1));

	// test
	test(sEntry1->Entry().Owner());
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	test(sEntry1->Entry().Owner());

	// delete child recursively
	REPORT_IF_ERROR(sEntry1->DeleteEntry(id2));

	// test
	test(sEntry1->Entry().Owner()==EFalse);
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	test(sEntry1->Entry().Owner()==EFalse);

	CleanupStack::PopAndDestroy(); // sEntry1
	}


LOCAL_C void TestNullContextL()
	{
	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	// create a folder
	TMsvEntry folder1;
	folder1.iDetails.Set(_L("some details"));
	folder1.iDescription.Set(KLongDescription);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));


	// set to null
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	test(sEntry1->Entry().Id()==KMsvNullIndexEntryId);
	test(sEntry1->Entry().iDescription==TPtrC());
	test(sEntry1->Entry().iDetails==TPtrC());

	// set to null
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	test(sEntry1->Entry().Id()==KMsvNullIndexEntryId);
	test(sEntry1->Entry().iDescription==TPtrC());
	test(sEntry1->Entry().iDetails==TPtrC());

	// set to not found
	test(sEntry1->SetEntry(-1234)==KErrNotFound);
	test(sEntry1->Entry().Id()==KMsvNullIndexEntryId);
	test(sEntry1->Entry().iDescription==TPtrC());
	test(sEntry1->Entry().iDetails==TPtrC());

	// set to folder
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	test(sEntry1->Entry().Id()==folder1.Id());
	test(sEntry1->Entry().iDescription==KLongDescription);
	test(sEntry1->Entry().iDetails==_L("some details"));

	// set to null
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	test(sEntry1->Entry().Id()==KMsvNullIndexEntryId);
	test(sEntry1->Entry().iDescription==TPtrC());
	test(sEntry1->Entry().iDetails==TPtrC());

	// set to folder
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	test(sEntry1->Entry().Id()==folder1.Id());
	test(sEntry1->Entry().iDescription==KLongDescription);
	test(sEntry1->Entry().iDetails==_L("some details"));

#if  !defined(_DEBUG)
	// set to null
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	test(sEntry1->Entry().Id()==KMsvNullIndexEntryId);
	test(sEntry1->Entry().iDescription==TPtrC());
	test(sEntry1->Entry().iDetails==TPtrC());

	TInt leave;
	TFileName name;
	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection;
	test(sEntry1->ChangeEntry(TMsvEntry())==KErrNotSupported);
	TMsvEntry entry;
	test(sEntry1->CreateEntry(entry)==KErrNotSupported);
	test(sEntry1->DeleteEntry(0)==KErrNotFound);
	test(sEntry1->DeleteEntry(folder1.Id())==KErrNotFound);
	TRAP(leave, sEntry1->EditStoreL());
	test(leave==KErrNotSupported);
	TRAP(leave, sEntry1->ReadStoreL());
	test(leave==KErrNotSupported);
	test(sEntry1->GetChildren(*selection)==KErrNotFound);
	delete selection;

#endif

	CleanupStack::PopAndDestroy(); // sEntry1
	}


LOCAL_C void TestMovingL()
	{
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry folder2;
	folder2.iType = KUidMsvFolderEntry;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CMsvServerEntry* sEntry2 = CMsvServerEntry::NewL(*theServer, KMsvGlobalInBoxIndexEntryId);
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;

	// create the folders
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder2));
	sEntry1->SetEntry(folder1.Id());
	sEntry2->SetEntry(folder2.Id());

	// move non-existant child
	test(sEntry1->MoveEntryWithinService(-132, -34334)==KErrNotFound);

	// create the child
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	
	// move to non-existant  target
	test(sEntry1->MoveEntryWithinService(entry1.Id(), -34334)==KErrNotFound);

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==0);

	// move to folder2
	REPORT_IF_ERROR(sEntry1->MoveEntryWithinService(entry1.Id(), folder2.Id()));

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==0);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==1);

	// lock the entry
	sEntry1->SetEntry(entry1.Id());

	// try move to folder1
	test(sEntry2->MoveEntryWithinService(entry1.Id(), folder1.Id())==KErrLocked);

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==0);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==1);

	// create a service
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==0);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==1);
/*
	// move entry to a service
	REPORT_IF_ERROR(sEntry2->MoveEntryWithinService(entry1.Id(), service.Id()));
	
	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==0);

	// move entry from a service
	REPORT_IF_ERROR(sEntry1->MoveEntryWithinService(entry1.Id(), folder2.Id()));

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==0);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==1);
*/
	// create a remote folder
	TMsvEntry folder3;
	folder3.iType = KUidMsvFolderEntry;
	folder3.iServiceId = service.Id();
	folder3.iMtm = KMsvTestUid1;
	REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder3));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder3.Id()));

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==0);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==1);
/*
	// move entry to a remote folder
	REPORT_IF_ERROR(sEntry2->MoveEntryWithinService(entry1.Id(), folder3.Id()));
*/

	// create entry in remote folder
	TMsvEntry entry3;
	entry3.iType = KUidMsvMessageEntry;
	entry3.iServiceId = service.Id();
	entry3.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry3));

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
//	selection->Reset(); sEntry2->GetChildren(*selection);
//	test(selection->Count()==0);

	REPORT_IF_ERROR(sEntry2->SetEntry(service.Id()));

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==1);

	// move within remote service
	REPORT_IF_ERROR(sEntry1->MoveEntryWithinService(entry3.Id(), service.Id()));

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==0);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==2);

	// move with remote service
	REPORT_IF_ERROR(sEntry2->MoveEntryWithinService(entry3.Id(), folder3.Id()));

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==1);

	// try to move into a descendent
	sEntry1->SetEntry(KMsvNullIndexEntryId);
	test(sEntry2->MoveEntryWithinService(folder3.Id(), entry3.Id())==KErrArgument);

	// try to move into same entry
	sEntry1->SetEntry(KMsvNullIndexEntryId);
	REPORT_IF_ERROR(sEntry2->MoveEntryWithinService(folder3.Id(), sEntry2->Entry().Id()));

	// try to move into itself
#if defined(_NDEBUG)
	sEntry1->SetEntry(KMsvNullIndexEntryId);
	test(sEntry2->MoveEntryWithinService(folder3.Id(), folder3.Id())==KErrArgument);
#endif

	delete selection;
	delete sEntry1;	
	delete sEntry2;	
	}


LOCAL_C void TestMovingBetweenServicesL()
	{
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;

	TRequestStatus st;
	TInt leave;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CMsvServerEntry* sEntry2 = CMsvServerEntry::NewL(*theServer, KMsvGlobalInBoxIndexEntryId);
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;

	// create the entries
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry2->SetEntry(service.Id()));

	// move non-existant child
	TRAP(leave, sEntry1->MoveEntryL(-132, -34334, st));
	test(leave==KErrNotFound);

	// move to non-existant  target
	TRAP(leave, sEntry1->MoveEntryL(entry1.Id(), -34334, st));
	test(leave==KErrNotFound);

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==0);

	// move to service
	sEntry1->MoveEntryL(entry1.Id(), service.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==0);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==1);

	// lock the entry
	sEntry1->SetEntry(entry1.Id());

	// try move to folder1
	TRAP(leave,sEntry2->MoveEntryL(entry1.Id(), folder1.Id(),st));
	test(leave==KErrLocked);

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==0);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==1);

	// release the entry
	sEntry1->SetEntry(folder1.Id());

	// moved again
	sEntry2->MoveEntryL(entry1.Id(), folder1.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);
	
	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==0);
	
	// cancelled during a move
	sEntry1->MoveEntryL(entry1.Id(), service.Id(), active->iStatus);
	active->Start();
	sEntry1->Cancel();
	CActiveScheduler::Start();
	test(active->iStatus==KErrCancel);
	
	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==0);

	// deleted after a move before completion
	sEntry1->MoveEntryL(entry1.Id(), service.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	sEntry1->Cancel();
	test(active->iStatus==KErrNone);
	
	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==0);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==1);

	
	delete selection;
	delete sEntry1;	
	delete sEntry2;	
	delete active;
	}

LOCAL_C void TestMovingSelectionBetweenServicesL()
	{
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;

	TRequestStatus st;
	TInt leave;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	CMsvServerEntry* sEntry2 = CMsvServerEntry::NewL(*theServer, KMsvGlobalInBoxIndexEntryId);
	CleanupStack::PushL(sEntry2);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	// create the entries
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	REPORT_IF_ERROR(sEntry2->SetEntry(service.Id()));

	selection->AppendL(-123);
	selection->AppendL(-456);

	// move non-existant children
	TRAP(leave, sEntry1->MoveEntriesL(*selection, -34334, st));
	test(leave==KErrNotFound);

	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);

	// move to non-existant  target
	TRAP(leave, sEntry1->MoveEntriesL(*selection, -34334, st));
	test(leave==KErrNotFound);

	// test
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==0);
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);

	// move to service
	sEntry1->MoveEntriesL(*selection, service.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==0);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==2);

	// lock the entry
	sEntry1->SetEntry(entry1.Id());

	// try move to folder1
	TRAP(leave,sEntry2->MoveEntriesL(*selection, folder1.Id(), active->iStatus));
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrLocked);

	sEntry1->SetEntry(folder1.Id());

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==1);

	// release the entry
	sEntry1->SetEntry(folder1.Id());

	// moved again
	sEntry2->MoveEntriesL(*selection, folder1.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);
	
	// test
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==0);
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	
	// cancelled during a move
	sEntry1->MoveEntriesL(*selection, service.Id(), active->iStatus);
	active->Start();
	sEntry1->Cancel();
	CActiveScheduler::Start();
	test(active->iStatus==KErrCancel);
	
	// test
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==0);
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);

	// deleted after a move before completion
	sEntry1->MoveEntriesL(*selection, service.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	sEntry1->Cancel();
	test(active->iStatus==KErrNone);
	
	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==0);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==2);

	CleanupStack::PopAndDestroy(4); // selection, sEntry1, sEntry2, active;
	}

LOCAL_C void TestCopyingBetweenServicesL()
	{
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry service;

	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;

	TRequestStatus st;
	TInt leave;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	CMsvServerEntry* sEntry2 = CMsvServerEntry::NewL(*theServer, KMsvGlobalInBoxIndexEntryId);
	CleanupStack::PushL(sEntry2);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	// create the entries
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry2->SetEntry(service.Id()));

	// copy non-existant child
	TRAP(leave, sEntry1->CopyEntryL(-132, -34334, st));
	test(leave==KErrNotFound);

	// copy to non-existant  target
	TRAP(leave, sEntry1->CopyEntryL(entry1.Id(), -34334, st));
	test(leave==KErrNotFound);

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==0);

	// copy to service
	sEntry1->CopyEntryL(entry1.Id(), service.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==1);

	TMsvId id = selection->At(0);

	// lock the entry
	sEntry1->SetEntry(id);

	// try copy to folder1
	TRAP(leave,sEntry2->CopyEntryL(id, folder1.Id(),st));
	test(leave==KErrAccessDenied);

	// release the entry
	sEntry1->SetEntry(folder1.Id());

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==1);

	// copy again
	sEntry2->CopyEntryL(id, folder1.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);
	
	// test
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==1);
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	
	id = selection->At(1);

	// cancelled during a copy
	sEntry1->CopyEntryL(id, service.Id(), active->iStatus);
	active->Start();
	sEntry1->Cancel();
	CActiveScheduler::Start();
	test(active->iStatus==KErrCancel);

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==1);
	// deleted after a copy before completion
	sEntry1->CopyEntryL(id, service.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	sEntry1->Cancel();
	test(active->iStatus==KErrNone);
	
	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==2);

	CleanupStack::PopAndDestroy(4); // selection, sEntry1, sEntry2, active;
	}

LOCAL_C void TestCopyingSelectionBetweenServicesL()
	{
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;

	TRequestStatus st;
	TInt leave;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	CMsvServerEntry* sEntry2 = CMsvServerEntry::NewL(*theServer, KMsvGlobalInBoxIndexEntryId);
	CleanupStack::PushL(sEntry2);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	// create the entries
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	REPORT_IF_ERROR(sEntry2->SetEntry(service.Id()));

	selection->AppendL(-123);
	selection->AppendL(-456);

	// copy non-existant child
	TRAP(leave, sEntry1->CopyEntriesL(*selection, -34334, st));
	test(leave==KErrNotFound);

	selection->Reset();
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());

	// copy to non-existant  target
	TRAP(leave, sEntry1->CopyEntriesL(*selection, -34334, st));
	test(leave==KErrNotFound);

	// test
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==0);
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);

	// copy to service
	sEntry1->CopyEntriesL(*selection, service.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==2);

	// lock the entry
	sEntry1->SetEntry(selection->At(0));

	// try copy to folder1
	TRAP(leave,sEntry2->CopyEntriesL(*selection, folder1.Id(), active->iStatus));
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrAccessDenied);

	// release the entry
	sEntry1->SetEntry(folder1.Id());

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==3);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==2);

	// copy again
	sEntry2->CopyEntriesL(*selection, folder1.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);
	
	// test
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==2);
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==5);
	
	// cancelled during a copy
	sEntry1->CopyEntriesL(*selection, service.Id(), active->iStatus);
	active->Start();
	sEntry1->Cancel();
	CActiveScheduler::Start();
	test(active->iStatus==KErrCancel);

	// test
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==2);
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==5);

	// deleted after a copy before completion
	sEntry1->CopyEntriesL(*selection, service.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	sEntry1->Cancel();
	test(active->iStatus==KErrNone);
	
	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==5);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count()==7);

	CleanupStack::PopAndDestroy(4); // selection, sEntry1, sEntry2, active;
	}

LOCAL_C void TestMovingWithStoresL()
	{
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry folder2;
	folder2.iType = KUidMsvFolderEntry;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	TMsvEntry folder3;
	folder3.iType = KUidMsvFolderEntry;
	folder3.iMtm = KMsvTestUid1;
	TMsvEntry service2;
	service2.iType = KUidMsvServiceEntry;
	service2.iMtm = KMsvTestUid1;
	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CMsvStore* store=NULL;

	TFileName filename1;

	// create the entries
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder2));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service2));
	REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
	folder3.iServiceId = service.Id();
	entry2.iServiceId = service.Id();
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder3));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder3.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));

	// create a store
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	store = sEntry1->EditStoreL();
	RMsvWriteStream out;
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();

	delete store;

	// test the store exists
	GetStoreFileNameL(entry1.Id(), entry1.Parent(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	// move the entry between local folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	REPORT_IF_ERROR(sEntry1->MoveEntryWithinService(entry1.Id(), folder2.Id()));

	// test the store has not been moved
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(entry1.Id(), folder2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	// create a store
	REPORT_IF_ERROR(sEntry1->SetEntry(entry2.Id()));
	store = sEntry1->EditStoreL();
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();

	delete store;

	// test the store exists
	GetStoreFileNameL(entry2.Id(), entry2.Parent(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	// move the entry between local folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folder3.Id()));
	REPORT_IF_ERROR(sEntry1->MoveEntryWithinService(entry2.Id(), service.Id()));

	// test the store has not been moved
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(entry2.Id(), service.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);
	
	
	delete selection;
	delete sEntry1;	
	}

LOCAL_C void TestMovingBetweenServicesWithStoresL()
	{
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry folder2;
	folder2.iType = KUidMsvFolderEntry;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;
	TMsvEntry folder3;
	folder3.iType = KUidMsvFolderEntry;
	folder3.iMtm = KMsvTestUid1;
	TMsvEntry service2;
	service2.iType = KUidMsvServiceEntry;
	service2.iMtm = KMsvTestUid1;
	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CMsvStore* store=NULL;
	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;

	TFileName filename1;

	// create the entries
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder2));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service2));
	REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
	folder3.iServiceId = service.Id();
	entry2.iServiceId = service.Id();
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder3));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder3.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));

	// create a store
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	store = sEntry1->EditStoreL();
	RMsvWriteStream out;
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	// test the store exists
	GetStoreFileNameL(entry1.Id(), entry1.Parent(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	// move the entry between local folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	sEntry1->MoveEntryL(entry1.Id(), folder2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store has not been moved
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(entry1.Id(), folder2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	// move the entry to a remote folder
	REPORT_IF_ERROR(sEntry1->SetEntry(folder2.Id()));
	sEntry1->MoveEntryL(entry1.Id(), folder3.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store has moved
	CMsgsTestUtils::FileMissing(filename1);
	GetStoreFileNameL(entry1.Id(), folder3.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	// move the entry between remote folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folder3.Id()));
	sEntry1->MoveEntryL(entry1.Id(), service.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store is still there
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(entry1.Id(), service.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	// move the entry between different services (shouldn't really be done)
	REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
	sEntry1->MoveEntryL(entry1.Id(), service2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store is still there
	CMsgsTestUtils::FileMissing(filename1);
	GetStoreFileNameL(entry1.Id(), service2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	// move the entry to a local folder
	REPORT_IF_ERROR(sEntry1->SetEntry(service2.Id()));
	sEntry1->MoveEntryL(entry1.Id(), folder2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store has moved
	CMsgsTestUtils::FileMissing(filename1);
	GetStoreFileNameL(entry1.Id(), folder2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	delete selection;
	delete sEntry1;	
	delete active;
	}

LOCAL_C void TestMovingSelectionBetweenServicesWithStoresL()
	{
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry folder2;
	folder2.iType = KUidMsvFolderEntry;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;

	TMsvEntry folder3;
	folder3.iType = KUidMsvFolderEntry;
	folder3.iMtm = KMsvTestUid1;

	TMsvEntry service2;
	service2.iType = KUidMsvServiceEntry;
	service2.iMtm = KMsvTestUid1;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	CMsvStore* store=NULL;
	TFileName filename1, filename2;

	// create the entries
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder2));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service2));
	REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
	folder3.iServiceId = service.Id();
	entry2.iServiceId = service.Id();
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder3));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder3.Id()));

	// create stores
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	store = sEntry1->EditStoreL();
	RMsvWriteStream out;
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	REPORT_IF_ERROR(sEntry1->SetEntry(entry2.Id()));
	store = sEntry1->EditStoreL();
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	// test the stores exist
	GetStoreFileNameL(entry1.Id(), entry1.Parent(), filename1);
	GetStoreFileNameL(entry2.Id(), entry2.Parent(), filename2);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);

	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());

	// move entries between local folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	sEntry1->MoveEntriesL(*selection, folder2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the stores have not been moved
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(entry1.Id(), folder2.Id(), filename1);
	GetStoreFileNameL(entry2.Id(), folder2.Id(), filename2);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);

	// move the entries to a remote folder
	REPORT_IF_ERROR(sEntry1->SetEntry(folder2.Id()));
	sEntry1->MoveEntriesL(*selection, folder3.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store has moved
	CMsgsTestUtils::FileMissing(filename1);
	CMsgsTestUtils::FileMissing(filename2);
	GetStoreFileNameL(entry1.Id(), folder3.Id(), filename1);
	GetStoreFileNameL(entry2.Id(), folder3.Id(), filename2);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);

	// move the entries between remote folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folder3.Id()));
	sEntry1->MoveEntriesL(*selection, service.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store is still there
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(entry1.Id(), service.Id(), filename1);
	GetStoreFileNameL(entry2.Id(), service.Id(), filename2);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);

	// move the entries between different services (shouldn't really be done)
	REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
	sEntry1->MoveEntriesL(*selection, service2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store is still there
	CMsgsTestUtils::FileMissing(filename1);
	CMsgsTestUtils::FileMissing(filename2);
	GetStoreFileNameL(entry1.Id(), service2.Id(), filename1);
	GetStoreFileNameL(entry2.Id(), service2.Id(), filename2);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);

	// move the entry to a local folder
	REPORT_IF_ERROR(sEntry1->SetEntry(service2.Id()));
	sEntry1->MoveEntriesL(*selection, folder2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store has moved
	CMsgsTestUtils::FileMissing(filename1);
	CMsgsTestUtils::FileMissing(filename2);
	GetStoreFileNameL(entry1.Id(), folder2.Id(), filename1);
	GetStoreFileNameL(entry2.Id(), folder2.Id(), filename2);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);

	CleanupStack::PopAndDestroy(3); // selection, sEntry1, active;
	}

LOCAL_C void TestCopyingBetweenServicesWithStoresL()
	{
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry folder2;
	folder2.iType = KUidMsvFolderEntry;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;

	TMsvEntry folder3;
	folder3.iType = KUidMsvFolderEntry;
	folder3.iMtm = KMsvTestUid1;

	TMsvEntry service2;
	service2.iType = KUidMsvServiceEntry;
	service2.iMtm = KMsvTestUid1;

	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	CMsvStore* store=NULL;
	TFileName filename1;

	// create the entries
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder2));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service2));
	REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
	folder3.iServiceId = service.Id();
	entry2.iServiceId = service.Id();
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder3));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder3.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));

	// create a store
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	store = sEntry1->EditStoreL();
	RMsvWriteStream out;
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	// test the store exists
	GetStoreFileNameL(entry1.Id(), entry1.Parent(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	// copy the entry between local folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	sEntry1->CopyEntryL(entry1.Id(), folder2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	REPORT_IF_ERROR(sEntry1->SetEntry(folder2.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));

	TMsvId id = selection->At(0);

	// test the store has been copied
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(id, folder2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	// copy the entry to a remote folder
	REPORT_IF_ERROR(sEntry1->SetEntry(folder2.Id()));
	sEntry1->CopyEntryL(id, folder3.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	REPORT_IF_ERROR(sEntry1->SetEntry(folder3.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	REPORT_IF_ERROR(sEntry1->SetEntry(folder2.Id()));

	id = selection->At(1);

	// test the store has been copied
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(id, folder3.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	// copy the entry between remote folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folder3.Id()));
	sEntry1->CopyEntryL(id, service.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	REPORT_IF_ERROR(sEntry1->SetEntry(folder3.Id()));

	id = selection->At(1);

	// test the store is still there
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(id, service.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	// copy the entry between different services (shouldn't really be done)
	REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));
	sEntry1->CopyEntryL(id, service2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	REPORT_IF_ERROR(sEntry1->SetEntry(service2.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	REPORT_IF_ERROR(sEntry1->SetEntry(service.Id()));

	id = selection->At(0);

	// test the store is there
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(id, service2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	// copy the entry to a local folder
	REPORT_IF_ERROR(sEntry1->SetEntry(service2.Id()));
	sEntry1->CopyEntryL(id, folder2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	REPORT_IF_ERROR(sEntry1->SetEntry(folder2.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	REPORT_IF_ERROR(sEntry1->SetEntry(service2.Id()));

	id = selection->At(1);

	// test the store has been copied
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(id, folder2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	CleanupStack::PopAndDestroy(3); // selection, sEntry1, active
	}

LOCAL_C void TestCopyingSelectionBetweenServicesWithStoresL()
	{
	TMsvEntry folderL1;
	folderL1.iType = KUidMsvFolderEntry;
	folderL1.iServiceId = KMsvLocalServiceIndexEntryId;
	folderL1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry folderL2;
	folderL2.iType = KUidMsvFolderEntry;
	folderL2.iServiceId = KMsvLocalServiceIndexEntryId;
	folderL2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry folderR1;
	folderR1.iType = KUidMsvFolderEntry;
	folderR1.iServiceId = KMsvLocalServiceIndexEntryId;
	folderR1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry folderR2;
	folderR2.iType = KUidMsvFolderEntry;
	folderR2.iServiceId = KMsvLocalServiceIndexEntryId;
	folderR2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry folderL3;
	folderL3.iType = KUidMsvFolderEntry;
	folderL3.iServiceId = KMsvLocalServiceIndexEntryId;
	folderL3.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service1;
	service1.iType = KUidMsvServiceEntry;
	service1.iMtm = KMsvTestUid1;

	TMsvEntry service2;
	service2.iType = KUidMsvServiceEntry;
	service2.iMtm = KMsvTestUid1;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	CMsvStore* store=NULL;
	TFileName filename1, filename2;

	// create the entries
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderL1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderL2));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderL3));
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service2));
	REPORT_IF_ERROR(sEntry1->SetEntry(service1.Id()));
	folderR1.iServiceId = service1.Id();
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderR1));
	folderR2.iServiceId = service1.Id();
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderR2));

	// create stores
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	store = sEntry1->EditStoreL();
	RMsvWriteStream out;
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	REPORT_IF_ERROR(sEntry1->SetEntry(entry2.Id()));
	store = sEntry1->EditStoreL();
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	// test the stores exist
	GetStoreFileNameL(entry1.Id(), entry1.Parent(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	GetStoreFileNameL(entry2.Id(), entry2.Parent(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());

	// copy the entries between local folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL1.Id()));
	sEntry1->CopyEntriesL(*selection, folderL2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	REPORT_IF_ERROR(sEntry1->SetEntry(folderL2.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL1.Id()));

	// test the store has been copied
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(selection->At(0), folderL2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(selection->At(1), folderL2.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	// copy the entries to a remote folder
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL2.Id()));
	sEntry1->CopyEntriesL(*selection, folderR1.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	REPORT_IF_ERROR(sEntry1->SetEntry(folderR1.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL2.Id()));

	// test the stores have been copied
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(selection->At(0), folderR1.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(selection->At(1), folderR1.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	// copy the entries between remote folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folderR1.Id()));
	sEntry1->CopyEntriesL(*selection, folderR2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	REPORT_IF_ERROR(sEntry1->SetEntry(folderR2.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	REPORT_IF_ERROR(sEntry1->SetEntry(folderR1.Id()));

	// test the stores are still there
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(selection->At(0), folderR2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(selection->At(1), folderR2.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	// copy the entry between different services (shouldn't really be done)
	REPORT_IF_ERROR(sEntry1->SetEntry(folderR2.Id()));
	sEntry1->CopyEntriesL(*selection, service2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	REPORT_IF_ERROR(sEntry1->SetEntry(service2.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	REPORT_IF_ERROR(sEntry1->SetEntry(service1.Id()));

	// test the store is there
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(selection->At(0), service2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(selection->At(1), service2.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	// copy the entries to a local folder
	REPORT_IF_ERROR(sEntry1->SetEntry(service2.Id()));
	sEntry1->CopyEntriesL(*selection, folderL3.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	REPORT_IF_ERROR(sEntry1->SetEntry(folderL3.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	REPORT_IF_ERROR(sEntry1->SetEntry(service2.Id()));

	// test the store has been copied
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(selection->At(0), folderL3.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(selection->At(1), folderL3.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	CleanupStack::PopAndDestroy(3); // selection, sEntry1, active
	}

LOCAL_C void TestMovingComplexL()
	{
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry folder2;
	folder2.iType = KUidMsvFolderEntry;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry attach1;
	attach1.iType = KUidMsvMessageEntry;
	attach1.iServiceId = KMsvLocalServiceIndexEntryId;
	attach1.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry attach2;
	attach2.iType = KUidMsvMessageEntry;
	attach2.iServiceId = KMsvLocalServiceIndexEntryId;
	attach2.iMtm = KUidMsvLocalServiceMtm;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;

	// create the entries
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder2));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(attach1));
	REPORT_IF_ERROR(sEntry1->SetEntry(entry2.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(attach2));

	// test
	REPORT_IF_ERROR(sEntry1->SetEntry(folder2.Id()));
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==0);

	// move the entry between local folders
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->MoveEntryWithinService(folder1.Id(), folder2.Id()));

	// test
	REPORT_IF_ERROR(sEntry1->SetEntry(folder2.Id()));
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==1);
	test(selection->At(0)==folder1.Id());
	selection->Reset();
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==2);
	test(selection->At(0)==entry1.Id());
	test(selection->At(1)==entry2.Id());
	selection->Reset();
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==1);
	test(selection->At(0)==attach1.Id());
	selection->Reset();
	REPORT_IF_ERROR(sEntry1->SetEntry(entry2.Id()));
	REPORT_IF_ERROR(sEntry1->GetChildren(*selection));
	test(selection->Count()==1);
	test(selection->At(0)==attach2.Id());
	

	delete sEntry1;
	delete selection;
	}






LOCAL_C void TestMovingWithChildrenAndStoresL()
	{
	TMsvEntry folderL;
	folderL.iType = KUidMsvFolderEntry;
	folderL.iServiceId = KMsvLocalServiceIndexEntryId;
	folderL.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry folderL2;
	folderL2.iType = KUidMsvFolderEntry;
	folderL2.iServiceId = KMsvLocalServiceIndexEntryId;
	folderL2.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry service1;
	service1.iType = KUidMsvServiceEntry;
	service1.iMtm = KMsvTestUid1;
	TMsvEntry folderR;
	folderR.iType = KUidMsvFolderEntry;
	folderR.iMtm = KMsvTestUid1;
	TMsvEntry service2;
	service2.iType = KUidMsvServiceEntry;
	service2.iMtm = KMsvTestUid1;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CMsvStore* store=NULL;

	TFileName filename1,filename2;

	// create the entries
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderL));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderL2));
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service2));
	REPORT_IF_ERROR(sEntry1->SetEntry(service1.Id()));
	folderR.iServiceId = service1.Id();
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderR));

	// create the stores
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	store = sEntry1->EditStoreL();
	RMsvWriteStream out;
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;
	REPORT_IF_ERROR(sEntry1->SetEntry(entry2.Id()));
	store = sEntry1->EditStoreL();
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();

	delete store;

	// test the store exists
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	GetStoreFileNameL(entry1.Id(), folderL.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(entry2.Id(), entry1.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	// move the entry between local folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL.Id()));
	REPORT_IF_ERROR(sEntry1->MoveEntryWithinService(entry1.Id(), KMsvGlobalInBoxIndexEntryId));

	// test the store has not been moved
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(entry1.Id(), KMsvGlobalInBoxIndexEntryId, filename1);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(entry2.Id(), entry1.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	delete selection;
	delete sEntry1;	
	}


LOCAL_C void TestMovingBetweenServicesWithChildrenAndStoresL()
	{
	TMsvEntry folderL;
	folderL.iType = KUidMsvFolderEntry;
	folderL.iServiceId = KMsvLocalServiceIndexEntryId;
	folderL.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry folderL2;
	folderL2.iType = KUidMsvFolderEntry;
	folderL2.iServiceId = KMsvLocalServiceIndexEntryId;
	folderL2.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	TMsvEntry service1;
	service1.iType = KUidMsvServiceEntry;
	service1.iMtm = KMsvTestUid1;
	TMsvEntry folderR;
	folderR.iType = KUidMsvFolderEntry;
	folderR.iMtm = KMsvTestUid1;
	TMsvEntry service2;
	service2.iType = KUidMsvServiceEntry;
	service2.iMtm = KMsvTestUid1;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CMsvStore* store=NULL;
	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;

	TFileName filename1,filename2;

	// create the entries
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderL));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderL2));
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service2));
	REPORT_IF_ERROR(sEntry1->SetEntry(service1.Id()));
	folderR.iServiceId = service1.Id();
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderR));

	// create the stores
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	store = sEntry1->EditStoreL();
	RMsvWriteStream out;
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;
	REPORT_IF_ERROR(sEntry1->SetEntry(entry2.Id()));
	store = sEntry1->EditStoreL();
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	// test the store exists
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	GetStoreFileNameL(entry1.Id(), folderL.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(entry2.Id(), entry1.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	// move the entry between local folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL.Id()));
	sEntry1->MoveEntryL(entry1.Id(), KMsvGlobalInBoxIndexEntryId, active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store has not been moved
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(entry1.Id(), KMsvGlobalInBoxIndexEntryId, filename1);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(entry2.Id(), entry1.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	// move the entry to a remote folder
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvGlobalInBoxIndexEntryId));
	sEntry1->MoveEntryL(entry1.Id(), folderR.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store has moved
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileMissing(filename1);
	GetStoreFileNameL(entry1.Id(), folderR.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileMissing(filename2);
	GetStoreFileNameL(entry2.Id(), entry1.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	// move the entry between remote folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folderR.Id()));
	sEntry1->MoveEntryL(entry1.Id(), service1.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store is still there
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(entry1.Id(), service1.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(entry2.Id(), entry1.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	// move the entry between different services (shouldn't really be done)
	REPORT_IF_ERROR(sEntry1->SetEntry(service1.Id()));
	sEntry1->MoveEntryL(entry1.Id(), service2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store has moved
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileMissing(filename1);
	GetStoreFileNameL(entry1.Id(), service2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileMissing(filename2);
	GetStoreFileNameL(entry2.Id(), entry1.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	// move the entry to a local folder
	REPORT_IF_ERROR(sEntry1->SetEntry(service2.Id()));
	sEntry1->MoveEntryL(entry1.Id(), folderL2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store has moved
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileMissing(filename1);
	GetStoreFileNameL(entry1.Id(), folderL2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileMissing(filename2);
	GetStoreFileNameL(entry2.Id(), entry1.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);


	delete selection;
	delete sEntry1;	
	delete active;
	}

LOCAL_C void TestMovingSelectionBetweenServicesWithChildrenAndStoresL()
	{
	TMsvEntry folderL;
	folderL.iType = KUidMsvFolderEntry;
	folderL.iServiceId = KMsvLocalServiceIndexEntryId;
	folderL.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry folderL2;
	folderL2.iType = KUidMsvFolderEntry;
	folderL2.iServiceId = KMsvLocalServiceIndexEntryId;
	folderL2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry child1;
	child1.iType = KUidMsvMessageEntry;
	child1.iServiceId = KMsvLocalServiceIndexEntryId;
	child1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry child2;
	child2.iType = KUidMsvMessageEntry;
	child2.iServiceId = KMsvLocalServiceIndexEntryId;
	child2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service1;
	service1.iType = KUidMsvServiceEntry;
	service1.iMtm = KMsvTestUid1;

	TMsvEntry folderR;
	folderR.iType = KUidMsvFolderEntry;
	folderR.iMtm = KMsvTestUid1;

	TMsvEntry service2;
	service2.iType = KUidMsvServiceEntry;
	service2.iMtm = KMsvTestUid1;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);
	
	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	CMsvStore* store=NULL;
	TFileName filename1, filename2;
	TFileName childFilename1, childFilename2;

	// create the entries
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderL));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderL2));
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(child1));
	REPORT_IF_ERROR(sEntry1->SetEntry(entry2.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(child2));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service2));
	REPORT_IF_ERROR(sEntry1->SetEntry(service1.Id()));
	folderR.iServiceId = service1.Id();
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderR));

	// create the stores
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	store = sEntry1->EditStoreL();
	RMsvWriteStream out;
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	REPORT_IF_ERROR(sEntry1->SetEntry(child1.Id()));
	store = sEntry1->EditStoreL();
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	REPORT_IF_ERROR(sEntry1->SetEntry(entry2.Id()));
	store = sEntry1->EditStoreL();
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	// create the stores
	REPORT_IF_ERROR(sEntry1->SetEntry(child2.Id()));
	store = sEntry1->EditStoreL();
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());

	// test the stores exist
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));

	GetStoreFileNameL(entry1.Id(), folderL.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(child1.Id(), entry1.Id(), childFilename1);
	CMsgsTestUtils::FileExists(childFilename1);

	GetStoreFileNameL(entry2.Id(), folderL.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(child2.Id(), entry2.Id(), childFilename2);
	CMsgsTestUtils::FileExists(childFilename2);

	// move the entries between local folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL.Id()));
	sEntry1->MoveEntriesL(*selection, KMsvGlobalInBoxIndexEntryId, active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the stores have not been moved
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));

	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(entry1.Id(), KMsvGlobalInBoxIndexEntryId, filename1);
	GetStoreFileNameL(entry2.Id(), KMsvGlobalInBoxIndexEntryId, filename2);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);

	CMsgsTestUtils::FileExists(childFilename1);
	CMsgsTestUtils::FileExists(childFilename2);
	GetStoreFileNameL(child1.Id(), entry1.Id(), childFilename1);
	GetStoreFileNameL(child2.Id(), entry2.Id(), childFilename2);
	CMsgsTestUtils::FileExists(childFilename1);
	CMsgsTestUtils::FileExists(childFilename2);

	// move the entry to a remote folder
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvGlobalInBoxIndexEntryId));
	sEntry1->MoveEntriesL(*selection, folderR.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the stores have moved
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));

	CMsgsTestUtils::FileMissing(filename1);
	CMsgsTestUtils::FileMissing(filename2);
	GetStoreFileNameL(entry1.Id(), folderR.Id(), filename1);
	GetStoreFileNameL(entry2.Id(), folderR.Id(), filename2);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);

	CMsgsTestUtils::FileMissing(childFilename1);
	CMsgsTestUtils::FileMissing(childFilename2);
	GetStoreFileNameL(child1.Id(), entry1.Id(), childFilename1);
	GetStoreFileNameL(child2.Id(), entry2.Id(), childFilename2);
	CMsgsTestUtils::FileExists(childFilename1);
	CMsgsTestUtils::FileExists(childFilename2);

	// move the entries between remote folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folderR.Id()));
	sEntry1->MoveEntriesL(*selection, service1.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store is still there
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(entry1.Id(), service1.Id(), filename1);
	GetStoreFileNameL(entry2.Id(), service1.Id(), filename2);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);

	CMsgsTestUtils::FileExists(childFilename1);
	CMsgsTestUtils::FileExists(childFilename2);
	GetStoreFileNameL(child1.Id(), entry1.Id(), childFilename1);
	GetStoreFileNameL(child2.Id(), entry2.Id(), childFilename2);
	CMsgsTestUtils::FileExists(childFilename1);
	CMsgsTestUtils::FileExists(childFilename2);

	// move the entries between different services (shouldn't really be done)
	REPORT_IF_ERROR(sEntry1->SetEntry(service1.Id()));
	sEntry1->MoveEntriesL(*selection, service2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the stores have moved
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileMissing(filename1);
	CMsgsTestUtils::FileMissing(filename2);
	GetStoreFileNameL(entry1.Id(), service2.Id(), filename1);
	GetStoreFileNameL(entry2.Id(), service2.Id(), filename2);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);

	CMsgsTestUtils::FileMissing(childFilename1);
	CMsgsTestUtils::FileMissing(childFilename2);
	GetStoreFileNameL(child1.Id(), entry1.Id(), childFilename1);
	GetStoreFileNameL(child2.Id(), entry1.Id(), childFilename2);
	CMsgsTestUtils::FileExists(childFilename1);
	CMsgsTestUtils::FileExists(childFilename2);

	// move the entries to a local folder
	REPORT_IF_ERROR(sEntry1->SetEntry(service2.Id()));
	sEntry1->MoveEntriesL(*selection, folderL2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test the store has moved
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));

	CMsgsTestUtils::FileMissing(filename1);
	CMsgsTestUtils::FileMissing(filename2);
	GetStoreFileNameL(entry1.Id(), folderL2.Id(), filename1);
	GetStoreFileNameL(entry2.Id(), folderL2.Id(), filename2);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);

	CMsgsTestUtils::FileMissing(childFilename1);
	CMsgsTestUtils::FileMissing(childFilename2);
	GetStoreFileNameL(child1.Id(), entry1.Id(), childFilename1);
	GetStoreFileNameL(child2.Id(), entry2.Id(), childFilename2);
	CMsgsTestUtils::FileExists(childFilename1);
	CMsgsTestUtils::FileExists(childFilename2);

	CleanupStack::PopAndDestroy(3); // selection, sEntry1, active;
	}

LOCAL_C void TestCopyingBetweenServicesWithChildrenAndStoresL()
	{
	TMsvEntry folderL;
	folderL.iType = KUidMsvFolderEntry;
	folderL.iServiceId = KMsvLocalServiceIndexEntryId;
	folderL.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry folderL2;
	folderL2.iType = KUidMsvFolderEntry;
	folderL2.iServiceId = KMsvLocalServiceIndexEntryId;
	folderL2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service1;
	service1.iType = KUidMsvServiceEntry;
	service1.iMtm = KMsvTestUid1;

	TMsvEntry folderR;
	folderR.iType = KUidMsvFolderEntry;
	folderR.iMtm = KMsvTestUid1;

	TMsvEntry service2;
	service2.iType = KUidMsvServiceEntry;
	service2.iMtm = KMsvTestUid1;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	CMsvStore* store=NULL;
	TFileName filename1,filename2;

	// create the entries
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderL));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderL2));
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service2));
	REPORT_IF_ERROR(sEntry1->SetEntry(service1.Id()));
	folderR.iServiceId = service1.Id();
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderR));

	// create the stores
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	store = sEntry1->EditStoreL();
	RMsvWriteStream out;
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	REPORT_IF_ERROR(sEntry1->SetEntry(entry2.Id()));
	store = sEntry1->EditStoreL();
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	// test the store exists
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	GetStoreFileNameL(entry1.Id(), folderL.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(entry2.Id(), entry1.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	// copy the entry between local folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL.Id()));
	sEntry1->CopyEntryL(entry1.Id(), folderL2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	REPORT_IF_ERROR(sEntry1->SetEntry(folderL2.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));

	TMsvId id = selection->At(0);

	// test the store has been copied
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(id, folderL2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	REPORT_IF_ERROR(sEntry1->SetEntry(id));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));

	TMsvId childId = selection->At(0);

	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(childId, id, filename2);
	CMsgsTestUtils::FileExists(filename2);

	// copy the entry to a remote folder
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL2.Id()));
	sEntry1->CopyEntryL(id, folderR.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	REPORT_IF_ERROR(sEntry1->SetEntry(folderR.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));

	id = selection->At(0);

	// test the store has been copied
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(id, folderR.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	REPORT_IF_ERROR(sEntry1->SetEntry(id));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));

	childId = selection->At(0);

	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(childId, id, filename2);
	CMsgsTestUtils::FileExists(filename2);

	// copy the entry between remote folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folderR.Id()));
	sEntry1->CopyEntryL(id, service1.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	REPORT_IF_ERROR(sEntry1->SetEntry(service1.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));

	id = selection->At(1);

	// test the store is still there
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(id, service1.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	REPORT_IF_ERROR(sEntry1->SetEntry(id));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));

	childId = selection->At(0);

	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(childId, id, filename2);
	CMsgsTestUtils::FileExists(filename2);

	// Copy the entry between different services (shouldn't really be done)
	REPORT_IF_ERROR(sEntry1->SetEntry(service1.Id()));
	sEntry1->CopyEntryL(id, service2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	REPORT_IF_ERROR(sEntry1->SetEntry(service2.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));

	id = selection->At(0);

	// test the store has moved
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(id, service2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	REPORT_IF_ERROR(sEntry1->SetEntry(id));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));

	childId = selection->At(0);

	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(childId, id, filename2);
	CMsgsTestUtils::FileExists(filename2);

	// copy the entry to a local folder
	REPORT_IF_ERROR(sEntry1->SetEntry(service2.Id()));
	sEntry1->CopyEntryL(id, folderL2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	REPORT_IF_ERROR(sEntry1->SetEntry(folderL2.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));

	id = selection->At(1);

	// test the store has moved
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(id, folderL2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);

	REPORT_IF_ERROR(sEntry1->SetEntry(id));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));

	childId = selection->At(0);

	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(childId, id, filename2);
	CMsgsTestUtils::FileExists(filename2);

	CleanupStack::PopAndDestroy(3); // selection, sEntry1, active;
	}

LOCAL_C void TestCopyingSelectionBetweenServicesWithChildrenAndStoresL()
	{
	TMsvEntry folderL1;
	folderL1.iType = KUidMsvFolderEntry;
	folderL1.iServiceId = KMsvLocalServiceIndexEntryId;
	folderL1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry folderL2;
	folderL2.iType = KUidMsvFolderEntry;
	folderL2.iServiceId = KMsvLocalServiceIndexEntryId;
	folderL2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry folderR1;
	folderR1.iType = KUidMsvFolderEntry;
	folderR1.iServiceId = KMsvLocalServiceIndexEntryId;
	folderR1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry folderR2;
	folderR2.iType = KUidMsvFolderEntry;
	folderR2.iServiceId = KMsvLocalServiceIndexEntryId;
	folderR2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry folderL3;
	folderL3.iType = KUidMsvFolderEntry;
	folderL3.iServiceId = KMsvLocalServiceIndexEntryId;
	folderL3.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry child1;
	child1.iType = KUidMsvMessageEntry;
	child1.iServiceId = KMsvLocalServiceIndexEntryId;
	child1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry child2;
	child2.iType = KUidMsvMessageEntry;
	child2.iServiceId = KMsvLocalServiceIndexEntryId;
	child2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service1;
	service1.iType = KUidMsvServiceEntry;
	service1.iMtm = KMsvTestUid1;

	TMsvEntry service2;
	service2.iType = KUidMsvServiceEntry;
	service2.iMtm = KMsvTestUid1;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	CMsvStore* store=NULL;
	TFileName filename1, filename2;
	TFileName childFilename1, childFilename2;

	// create the entries
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderL1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderL2));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderL3));
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(child1));
	REPORT_IF_ERROR(sEntry1->SetEntry(entry2.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(child2));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service2));
	REPORT_IF_ERROR(sEntry1->SetEntry(service1.Id()));
	folderR1.iServiceId = service1.Id();
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderR1));
	folderR2.iServiceId = service1.Id();
	REPORT_IF_ERROR(sEntry1->CreateEntry(folderR2));

	// create the stores
	REPORT_IF_ERROR(sEntry1->SetEntry(entry1.Id()));
	store = sEntry1->EditStoreL();
	RMsvWriteStream out;
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	REPORT_IF_ERROR(sEntry1->SetEntry(child1.Id()));
	store = sEntry1->EditStoreL();
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	REPORT_IF_ERROR(sEntry1->SetEntry(entry2.Id()));
	store = sEntry1->EditStoreL();
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	REPORT_IF_ERROR(sEntry1->SetEntry(child2.Id()));
	store = sEntry1->EditStoreL();
	out.AssignLC(*store,KNullUid);
	out.WriteInt32L(1234);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	store->CommitL();
	delete store;

	// test the stores exist
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));

	GetStoreFileNameL(entry1.Id(), folderL1.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(entry2.Id(), folderL1.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	GetStoreFileNameL(child1.Id(), entry1.Id(), childFilename1);
	CMsgsTestUtils::FileExists(childFilename1);

	GetStoreFileNameL(child2.Id(), entry2.Id(), childFilename2);
	CMsgsTestUtils::FileExists(childFilename2);

	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());

	// copy the entries between local folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL1.Id()));
	sEntry1->CopyEntriesL(*selection, folderL2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// Test the stores have been copied
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL2.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);

	TMsvId id1 = selection->At(0);
	TMsvId id2 = selection->At(1);

	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(id1, folderL2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(id2, folderL2.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	REPORT_IF_ERROR(sEntry1->SetEntry(id1));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);

	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(childFilename1);
	GetStoreFileNameL(selection->At(0), id1, childFilename1);
	CMsgsTestUtils::FileExists(childFilename1);

	REPORT_IF_ERROR(sEntry1->SetEntry(id2));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);

	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(childFilename2);
	GetStoreFileNameL(selection->At(0), id2, childFilename2);
	CMsgsTestUtils::FileExists(childFilename2);

	// copy the entries to a remote folder
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL2.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	sEntry1->CopyEntriesL(*selection, folderR1.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// Test the stores have been copied
	REPORT_IF_ERROR(sEntry1->SetEntry(folderR1.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);

	id1 = selection->At(0);
	id2 = selection->At(1);

	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(id1, folderR1.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(id2, folderR1.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	REPORT_IF_ERROR(sEntry1->SetEntry(id1));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);

	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(childFilename1);
	GetStoreFileNameL(selection->At(0), id1, childFilename1);
	CMsgsTestUtils::FileExists(childFilename1);

	REPORT_IF_ERROR(sEntry1->SetEntry(id2));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);

	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(childFilename2);
	GetStoreFileNameL(selection->At(0), id2, childFilename2);
	CMsgsTestUtils::FileExists(childFilename2);

	// copy the entries between remote folders
	REPORT_IF_ERROR(sEntry1->SetEntry(folderR1.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	sEntry1->CopyEntriesL(*selection, folderR2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// Test the stores have been copied
	REPORT_IF_ERROR(sEntry1->SetEntry(folderR2.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);

	id1 = selection->At(0);
	id2 = selection->At(1);

	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(id1, folderR2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(id2, folderR2.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	REPORT_IF_ERROR(sEntry1->SetEntry(id1));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);

	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(childFilename1);
	GetStoreFileNameL(selection->At(0), id1, childFilename1);
	CMsgsTestUtils::FileExists(childFilename1);

	REPORT_IF_ERROR(sEntry1->SetEntry(id2));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);

	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(childFilename2);
	GetStoreFileNameL(selection->At(0), id2, childFilename2);
	CMsgsTestUtils::FileExists(childFilename2);

	// Copy the entries between different services (shouldn't really be done)
	REPORT_IF_ERROR(sEntry1->SetEntry(folderR2.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	sEntry1->CopyEntriesL(*selection, service2.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// Test the stores have been copied
	REPORT_IF_ERROR(sEntry1->SetEntry(service2.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);

	id1 = selection->At(0);
	id2 = selection->At(1);

	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(id1, service2.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(id2, service2.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	REPORT_IF_ERROR(sEntry1->SetEntry(id1));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);

	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(childFilename1);
	GetStoreFileNameL(selection->At(0), id1, childFilename1);
	CMsgsTestUtils::FileExists(childFilename1);

	REPORT_IF_ERROR(sEntry1->SetEntry(id2));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);

	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(childFilename2);
	GetStoreFileNameL(selection->At(0), id2, childFilename2);
	CMsgsTestUtils::FileExists(childFilename2);

	// copy the entries to a local folder
	REPORT_IF_ERROR(sEntry1->SetEntry(service2.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	sEntry1->CopyEntriesL(*selection, folderL3.Id(), active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// Test the stores have been copied
	REPORT_IF_ERROR(sEntry1->SetEntry(folderL3.Id()));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);

	id1 = selection->At(0);
	id2 = selection->At(1);

	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(filename1);
	GetStoreFileNameL(id1, folderL3.Id(), filename1);
	CMsgsTestUtils::FileExists(filename1);
	CMsgsTestUtils::FileExists(filename2);
	GetStoreFileNameL(id2, folderL3.Id(), filename2);
	CMsgsTestUtils::FileExists(filename2);

	REPORT_IF_ERROR(sEntry1->SetEntry(id1));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);

	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(childFilename1);
	GetStoreFileNameL(selection->At(0), id1, childFilename1);
	CMsgsTestUtils::FileExists(childFilename1);

	REPORT_IF_ERROR(sEntry1->SetEntry(id2));
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);

	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvNullIndexEntryId));
	CMsgsTestUtils::FileExists(childFilename2);
	GetStoreFileNameL(selection->At(0), id2, childFilename2);
	CMsgsTestUtils::FileExists(childFilename2);

	CleanupStack::PopAndDestroy(3); // selection, sEntry1, active;
	}












LOCAL_C void TestRemovingAllFilesL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvGlobalInBoxIndexEntryId);
	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection;
	TInt count;
	CDir* dir;
	TFileName filename;

	TMsvSelectionOrdering sort(KMsvNoGrouping, EMsvSortByNone, ETrue);
	sEntry->SetSort(sort);

	// everything under the inbox
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvGlobalInBoxIndexEntryId));	
	REPORT_IF_ERROR(sEntry->GetChildren(*selection));
	for (count=0; count<selection->Count(); count++)
		REPORT_IF_ERROR(sEntry->DeleteEntry(selection->At(count)));
	selection->Reset();

	// everything under the outbox
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvGlobalOutBoxIndexEntryId));	
	REPORT_IF_ERROR(sEntry->GetChildren(*selection));
	for (count=0; count<selection->Count(); count++)
		REPORT_IF_ERROR(sEntry->DeleteEntry(selection->At(count)));
	selection->Reset();

	// everything under the local service
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));	
	REPORT_IF_ERROR(sEntry->GetChildren(*selection));
	for (count=0; count<selection->Count(); count++)
		{
		TMsvId id = selection->At(count);
		if (id!=KMsvGlobalOutBoxIndexEntryId && id!=KMsvGlobalInBoxIndexEntryId && id!=KMsvDraftEntryIdValue && id!=KMsvSentEntryIdValue && id!=KMsvDeletedEntryFolderEntryId)
			REPORT_IF_ERROR(sEntry->DeleteEntry(id));
		}
	selection->Reset();

	// test
	GetServicePath(KMsvLocalServiceIndexEntryId, filename);
	REPORT_IF_ERROR(theUtils->FileSession().GetDir(filename, KEntryAttMaskSupported, ESortNone, dir));
	TInt counter=dir->Count();
	while(counter--)
		{
		CDir* subdir;
		TFileName subfilename(filename);
		subfilename.Append((*dir)[counter].iName);
		_LIT(KDirSep,"\\");
		subfilename.Append(KDirSep);
		REPORT_IF_ERROR(theUtils->FileSession().GetDir(subfilename, KEntryAttMaskSupported, ESortNone, subdir));		
		test(subdir->Count()==0);
		delete subdir;
		}
	delete dir;

	// everything under the root
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvRootIndexEntryId));	
	REPORT_IF_ERROR(sEntry->GetChildren(*selection));
	for (count=0; count<selection->Count(); count++)
		{
		TMsvId id = selection->At(count);
		if (id!=KMsvLocalServiceIndexEntryId)
			{
			TInt error = sEntry->DeleteEntry(id);
			test(error==KErrNone);
			}
		}
	selection->Reset();
	// test
	REPORT_IF_ERROR(theUtils->FileSession().GetDir(KMsvTestFolder, KEntryAttMaskSupported, ESortNone, dir));
	test(dir->Count()==2);
	delete dir;
	delete selection;
	delete sEntry;
	}


LOCAL_C void TestWaitL()
	{
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(0x00C00000); // has to be longer than KMsvDelayTime
	CActiveScheduler::Start();
	delete timer;
	}



LOCAL_C void TestOwningServiceL()
	{
	TMsvEntry entryL;
	entryL.iType = KUidMsvMessageEntry;
	entryL.iMtm = KMsvTestUid1;
	TMsvEntry entryR;
	entryR.iType = KUidMsvMessageEntry;
	entryR.iMtm = KMsvTestUid1;
	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;

	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);

	// create service
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry->CreateEntry(service));

	entryR.iServiceId = service.Id();
	entryL.iServiceId = service.Id();

	// create the entries
	REPORT_IF_ERROR(sEntry->SetEntry(service.Id()));
	REPORT_IF_ERROR(sEntry->CreateEntry(entryR));
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvGlobalInBoxIndexEntryId));
	REPORT_IF_ERROR(sEntry->CreateEntry(entryL));

	// test the owning services

	// root
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvRootIndexEntryId));
	test(sEntry->OwningService()==KMsvRootIndexEntryId);

	// local
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
	test(sEntry->OwningService()==KMsvLocalServiceIndexEntryId);
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvGlobalInBoxIndexEntryId));
	test(sEntry->OwningService()==KMsvLocalServiceIndexEntryId);

	// service
	REPORT_IF_ERROR(sEntry->SetEntry(service.Id()));
	test(sEntry->OwningService()==service.Id());

	// entries
	REPORT_IF_ERROR(sEntry->SetEntry(entryL.Id()));
	test(sEntry->OwningService()==KMsvLocalServiceIndexEntryId);
	REPORT_IF_ERROR(sEntry->SetEntry(entryR.Id()));
	test(sEntry->OwningService()==service.Id());

	delete sEntry;
	}


LOCAL_C void TestChangingAttributesL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);

	// create a folder to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));

	// create three entries
	TMsvEntry entry1, entry2, entry3;
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


	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection;
	CMsvEntrySelection* children = new(ELeave) CMsvEntrySelection;

	// empty selection
	REPORT_IF_ERROR(sEntry->ChangeAttributes(*selection, 0, 0));

	// unknown entry
	selection->AppendL(-1234);
	test(sEntry->ChangeAttributes(*selection, 0, 0)==KErrNotFound);
	selection->AppendL(entry1.Id());
	test(sEntry->ChangeAttributes(*selection, 0, 0)==KErrNotFound);
	selection->Reset();
	selection->AppendL(entry1.Id());
	selection->AppendL(-1234);
	test(sEntry->ChangeAttributes(*selection, 0, 0)==KErrNotFound);

	// work with all three entries
	selection->Reset();
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());
	
	// no attributes changed
	REPORT_IF_ERROR(sEntry->ChangeAttributes(*selection, 0, 0));

	// mark all as invisible
	REPORT_IF_ERROR(sEntry->ChangeAttributes(*selection, 0, KMsvVisibilityAttribute));
	REPORT_IF_ERROR(sEntry->GetChildren(*children));
	test(children->Count()==0);

	// mark all as visible
	REPORT_IF_ERROR(sEntry->ChangeAttributes(*selection, KMsvVisibilityAttribute, 0));
	REPORT_IF_ERROR(sEntry->GetChildren(*children));
	test(children->Count()==3);

	// mark all as unread
	REPORT_IF_ERROR(sEntry->ChangeAttributes(*selection, KMsvUnreadAttribute, 0));

	// test
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));
	test(sEntry->Entry().Unread());
	REPORT_IF_ERROR(sEntry->SetEntry(entry2.Id()));
	test(sEntry->Entry().Unread());
	REPORT_IF_ERROR(sEntry->SetEntry(entry3.Id()));
	test(sEntry->Entry().Unread());
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
	
	// mark all as read
	REPORT_IF_ERROR(sEntry->ChangeAttributes(*selection, 0, KMsvUnreadAttribute));

	// test
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));
	test(!sEntry->Entry().Unread());
	REPORT_IF_ERROR(sEntry->SetEntry(entry2.Id()));
	test(!sEntry->Entry().Unread());
	REPORT_IF_ERROR(sEntry->SetEntry(entry3.Id()));
	test(!sEntry->Entry().Unread());
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));

	// mark all as invisible and unread
	REPORT_IF_ERROR(sEntry->ChangeAttributes(*selection, KMsvUnreadAttribute, KMsvVisibilityAttribute));
	REPORT_IF_ERROR(sEntry->GetChildren(*children));
	test(children->Count()==0);
	
	// test
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));
	test(sEntry->Entry().Unread());
	REPORT_IF_ERROR(sEntry->SetEntry(entry2.Id()));
	test(sEntry->Entry().Unread());
	REPORT_IF_ERROR(sEntry->SetEntry(entry3.Id()));
	test(sEntry->Entry().Unread());
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));


	// mark all as connected
	REPORT_IF_ERROR(sEntry->ChangeAttributes(*selection, KMsvConnectedAttribute, 0));

	// test
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));
	test(sEntry->Entry().Connected());
	REPORT_IF_ERROR(sEntry->SetEntry(entry2.Id()));
	test(sEntry->Entry().Connected());
	REPORT_IF_ERROR(sEntry->SetEntry(entry3.Id()));
	test(sEntry->Entry().Connected());
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
	
	// mark all as read
	REPORT_IF_ERROR(sEntry->ChangeAttributes(*selection, 0, KMsvConnectedAttribute));

	// test
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));
	test(!sEntry->Entry().Connected());
	REPORT_IF_ERROR(sEntry->SetEntry(entry2.Id()));
	test(!sEntry->Entry().Connected());
	REPORT_IF_ERROR(sEntry->SetEntry(entry3.Id()));
	test(!sEntry->Entry().Connected());
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
	
	// fail to change because it is locked
	CMsvServerEntry* sEntry2 = CMsvServerEntry::NewL(*theServer, entry2.Id());

	// mark all as read
	test(sEntry->ChangeAttributes(*selection, 0, KMsvUnreadAttribute)==KErrLocked);
	
	// clear the lock
	REPORT_IF_ERROR(sEntry2->SetEntry(KMsvLocalServiceIndexEntryId));
	
	// test
	REPORT_IF_ERROR(sEntry->SetEntry(entry1.Id()));
	test(sEntry->Entry().Unread());
	REPORT_IF_ERROR(sEntry->SetEntry(entry2.Id()));
	test(sEntry->Entry().Unread());
	REPORT_IF_ERROR(sEntry->SetEntry(entry3.Id()));
	test(sEntry->Entry().Unread());
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));

	delete sEntry2;
	delete sEntry;
	delete selection;
	delete children;
	}

LOCAL_C void TestScheduledSendingL()
	{
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry);

	// create a folder to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	folder.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(folder));
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));

	// create entry
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMsvLocalServiceMtm;
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));

	test(!sEntry->Entry().OffPeak());
	test(!sEntry->Entry().Scheduled());
	test(sEntry->Entry().SendingState() == 0);

	entry.SetOffPeak(ETrue);
	sEntry->ChangeEntry(entry);
	test(sEntry->Entry().OffPeak());
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
	REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));
	test(sEntry->Entry().OffPeak());

	entry.SetOffPeak(EFalse);
	sEntry->ChangeEntry(entry);
	test(!sEntry->Entry().OffPeak());
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
	REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));
	test(!sEntry->Entry().OffPeak());

	entry.SetScheduled(ETrue);
	sEntry->ChangeEntry(entry);
	test(sEntry->Entry().Scheduled());
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
	REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));
	test(sEntry->Entry().Scheduled());

	entry.SetScheduled(EFalse);
	sEntry->ChangeEntry(entry);
	test(!sEntry->Entry().Scheduled());
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
	REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));
	test(!sEntry->Entry().Scheduled());

	TUint state;
	for(state = 1; state < 16; state++)
		{
		entry.SetSendingState(state);
		sEntry->ChangeEntry(entry);
		test(sEntry->Entry().SendingState() == state);
		REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
		REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));
		test(sEntry->Entry().SendingState() == state);
		}

	entry.SetSendingState(0);
	sEntry->ChangeEntry(entry);
	test(sEntry->Entry().SendingState() == 0);
	REPORT_IF_ERROR(sEntry->SetEntry(folder.Id()));
	REPORT_IF_ERROR(sEntry->SetEntry(entry.Id()));
	test(sEntry->Entry().SendingState() == 0);

	CleanupStack::PopAndDestroy(); // sEntry
	}

LOCAL_C void TestDeleteSelL()
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

	REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
	
	// Delete all the folders and their children
	REPORT_IF_ERROR(sEntry->DeleteEntries(*selection));

	// Check we get what we expect
	test(selection->Count() == 0);

	CleanupStack::PopAndDestroy(2); // selection, sEntry
	}

LOCAL_C void TestMoveSelL()
	{

	TMsvEntry folder[KFolderCount];
	TMsvEntry entry[KFolderCount][KEntriesPerFolder];

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

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

	REPORT_IF_ERROR(sEntry->SetEntry(folder1.Id()));
	
	// Move all the folders and their children
	REPORT_IF_ERROR(sEntry->MoveEntriesWithinService(*selection, folder2.Id()));

	// Check we get what we expect
	test(selection->Count() == 0);

	REPORT_IF_ERROR(sEntry->SetEntry(folder2.Id()));
	sEntry->GetChildren(*selection);
	test(selection->Count() == KFolderCount);

	CleanupStack::PopAndDestroy(2); // selection, sEntry
	}

LOCAL_C void TestCopyingSelectionWithCompletedEntriesL()
//
// Test that the CMsvServerEntry::CopyEntriesL() function returns the correct
// completed entry selection. Compares returned selection and children from target folder.
//
	{
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	CMsvServerEntry* sEntry2 = CMsvServerEntry::NewL(*theServer, KMsvGlobalInBoxIndexEntryId);
	CleanupStack::PushL(sEntry2);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	CMsvEntrySelection* returnedSelection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(returnedSelection);

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	// create the entries
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry2));
	REPORT_IF_ERROR(sEntry2->SetEntry(service.Id()));

	// copy to service
	selection->Reset();
	sEntry1->GetChildren(*selection);
	sEntry1->CopyEntriesL(*selection, service.Id(), *returnedSelection, active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==2);
	selection->Reset(); sEntry2->GetChildren(*selection);
	TInt count = selection->Count();
	TBool found = ETrue;
	test(count == 2);
	test(returnedSelection->Count() == 2);

	// Test that the two selections contain the same IDs
	while (count--)
		{
		TInt returnedCount = returnedSelection->Count();
		TBool thisFound = EFalse;
		while (returnedCount--)
			{
			if (returnedSelection->At(returnedCount) == selection->At(count))
				thisFound = ETrue;
			}
		if (!thisFound)
			found = EFalse;
		}
	test(found);

	CleanupStack::PopAndDestroy(5); // returnedSelection, selection, sEntry1, sEntry2, active;
	}

LOCAL_C void TestCopyingWithCompletedEntryL()
//
// Test that the CMsvServerEntry::CopyEntryL() function returns the correct
// completed entry ID. Compares returned entry and child from target folder.
//
	{
	TMsvEntry folder1;
	folder1.iType = KUidMsvFolderEntry;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	folder1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.iMtm = KUidMsvLocalServiceMtm;

	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KMsvTestUid1;

	CMsvServerEntry* sEntry1 = CMsvServerEntry::NewL(*theServer, KMsvLocalServiceIndexEntryId);
	CleanupStack::PushL(sEntry1);

	CMsvServerEntry* sEntry2 = CMsvServerEntry::NewL(*theServer, KMsvGlobalInBoxIndexEntryId);
	CleanupStack::PushL(sEntry2);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TMsvId returnedEntryId;

	COperationCompleteActive* active = new(ELeave)COperationCompleteActive;
	CleanupStack::PushL(active);

	// create the entries
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvLocalServiceIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(folder1));
	REPORT_IF_ERROR(sEntry1->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry1->CreateEntry(service));
	REPORT_IF_ERROR(sEntry1->SetEntry(folder1.Id()));
	REPORT_IF_ERROR(sEntry1->CreateEntry(entry1));
	REPORT_IF_ERROR(sEntry2->SetEntry(service.Id()));

	// copy to service
	selection->Reset();
	sEntry1->GetChildren(*selection);
	sEntry1->CopyEntryL(selection->At(0), service.Id(), returnedEntryId, active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	test(active->iStatus==KErrNone);

	// test
	selection->Reset(); sEntry1->GetChildren(*selection);
	test(selection->Count()==1);
	selection->Reset(); sEntry2->GetChildren(*selection);
	test(selection->Count() == 1);
	test(selection->At(0) == returnedEntryId);

	CleanupStack::PopAndDestroy(4); // selection, sEntry1, sEntry2, active;
	}


void doTestsL()
	{
	theUtils->CleanMessageFolderL(KMsvTestIndexFile);

	theServer = CMsgsTestUtils::CreateServerL();
	CleanupStack::PushL(theServer);

	CMsgsTestUtils::Start(_L("C'tors"));
	TestConstructorL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Context"));
	TestSwitchingL();
	TestChangingL();
	TestChangingRootL();
	TestChangingWithStoreOpenL();
	TestChangingServerFlagsL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Children"));
	TestCreatingDeletingL();
	TestDeleteAllL();
	TestDeleteSelL();
	TestMoveSelL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Selections"));
	TestChidlrenSelectionL();
	TestInvisibleSelectionL();
	TestMtmSelectionL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Flags"));
	TestOwnerFlagL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Null context"));
	TestNullContextL();
	CMsgsTestUtils::Complete();

	// Wait 10s for a compact to complete
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(10000000);
	CActiveScheduler::Start();
	delete timer;

	CMsgsTestUtils::Next(_L("Moving"));
	TestMovingL();
	TestMovingComplexL();
	TestMovingWithStoresL();
	TestMovingWithChildrenAndStoresL();
	CMsgsTestUtils::Complete();

	// Wait 10s for a compact to complete
	timer = CTestTimer::NewL();
	timer->After(10000000);
	CActiveScheduler::Start();
	delete timer;

	CMsgsTestUtils::Next(_L("Moving entry between services"));
	TestMovingBetweenServicesL();
	TestMovingBetweenServicesWithStoresL();
	TestMovingBetweenServicesWithChildrenAndStoresL();
	CMsgsTestUtils::Complete();

	// Wait 10s for a compact to complete
	timer = CTestTimer::NewL();
	timer->After(10000000);
	CActiveScheduler::Start();
	delete timer;

	CMsgsTestUtils::Next(_L("Moving selection between services"));
	TestMovingSelectionBetweenServicesL();
	TestMovingSelectionBetweenServicesWithStoresL();
	TestMovingSelectionBetweenServicesWithChildrenAndStoresL();
	CMsgsTestUtils::Complete();

	// Wait 10s for a compact to complete
	timer = CTestTimer::NewL();
	timer->After(10000000);
	CActiveScheduler::Start();
	delete timer;

	CMsgsTestUtils::Next(_L("Copying entry between services"));
	TestCopyingBetweenServicesL();
	TestCopyingWithCompletedEntryL();
	TestCopyingBetweenServicesWithStoresL();
	TestCopyingBetweenServicesWithChildrenAndStoresL();
	CMsgsTestUtils::Complete();

	// Wait 10s for a compact to complete
	timer = CTestTimer::NewL();
	timer->After(10000000);
	CActiveScheduler::Start();
	delete timer;

	CMsgsTestUtils::Next(_L("Copying selection between services"));
	TestCopyingSelectionBetweenServicesL();
	TestCopyingSelectionWithCompletedEntriesL();
	TestCopyingSelectionBetweenServicesWithStoresL();
	TestCopyingSelectionBetweenServicesWithChildrenAndStoresL();
	CMsgsTestUtils::Complete();

	// Wait 10s for a compact to complete
	timer = CTestTimer::NewL();
	timer->After(10000000);
	CActiveScheduler::Start();
	delete timer;


	CMsgsTestUtils::Next(_L("Stores"));
	TestStoresL();	
	TestLocalStoreFlagL();	
	TestRemoteStoreFlagL();	
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Owning Service"));
	TestOwningServiceL();	
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Changing attributes"));
	TestChangingAttributesL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Scheduled Sending"));
	TestScheduledSendingL();
	CMsgsTestUtils::Complete();

	test.Printf(_L("** Waiting 10s for delayed updating of files **\n"));
	TestWaitL();

	CMsgsTestUtils::Next(_L("Removing all entries"));
	test.Printf(_L("...checking for orphan files\n"));
	TestRemovingAllFilesL();
	CMsgsTestUtils::Complete();

	CleanupStack::PopAndDestroy(); // theServer
	}

#pragma warning (default : 4238)
