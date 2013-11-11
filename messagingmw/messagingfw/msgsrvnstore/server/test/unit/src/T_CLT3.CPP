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

#include <msvipc.h>
#include "MSERVER.H"
#include <msvreg.h>
#include <msvuids.h>
#include <msvids.h>
#include "MSERVER.H"
#include "Testsetup.h"

#if !defined(_DEBUG)
#pragma warning( disable : 4702 ) //  unreachable code
#endif
#pragma warning( disable : 4238 ) //nonstandard extension used : class rvalue used as lvalue

RTest	test(_L("MSV Client Testrig 3"));

_LIT(KShortDescription ,"Description");
_LIT(KShortDetails, "Details");
_LIT(KLongDescription, "A very long description which goes over thirty two characters");
_LIT(KLongDetails, "A very long details string containing rubbish and junk");
_LIT(KDataComponentFilename, "c:\\system\\mtm\\testsmtm.dat");

const TUid KUidTestServerMtmType={0x10001304};
const TUid KUidTestServerMtmVersion1={0x10001305};
const TUid KUidTestServerMtmTech={0x10001304};
_LIT(KTestServerMtmFilename, "z:\\system\\libs\\testservermtm.dll");

//

class CSessionObserver : public CBase, public MMsvSessionObserver
	{
public:
	CSessionObserver();
	~CSessionObserver();
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
protected:
	void DoHandleSessionEvent(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
public:
	TMsvSessionEvent iEvent;
	};

CSessionObserver::CSessionObserver() 
	{
	}

CSessionObserver::~CSessionObserver() 
	{
	}

void CSessionObserver::DoHandleSessionEvent(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*)
	{
	iEvent = aEvent;
	}


void CSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
	{
	DoHandleSessionEvent(aEvent,aArg1,aArg2,aArg3);
	}

//

LOCAL_C void CreateMtmFilesL()
	{
	TInt error = theUtils->FileSession().MkDirAll(KDataComponentFilename);
	test(error==KErrNone || error == KErrAlreadyExists);

	CMtmDllInfoArray* mtmdllinfoarray=new(ELeave) CMtmDllInfoArray();
	CleanupStack::PushL(mtmdllinfoarray);

	CMtmDllInfo* mtmdllinfo1=mtmdllinfo1=CMtmDllInfo::NewL(_L("Test Server Mtm"),TUidType(KDynamicLibraryUid,KUidMtmServerComponent,TUid::Uid(KUidMtmDefaultSpecificVal)),KTestServerMtmFilename,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo1);

	TCapabilitySet capSet;
	capSet.SetEmpty();
	CleanupStack::Pop(mtmdllinfoarray); // next line takes ownership
	CMtmGroupData* mtmgroupdata=CMtmGroupData::NewL(KUidTestServerMtmType, KUidTestServerMtmTech, mtmdllinfoarray, capSet);
	CleanupStack::PushL(mtmgroupdata);

	CFileStore* filestore = CPermanentFileStore::ReplaceLC(theUtils->FileSession(),KDataComponentFilename,EFileShareExclusive|EFileStream|EFileWrite);
	TUidType uidtype(KPermanentFileStoreLayoutUid,KUidMsvDataComponent,KUidTestServerMtmVersion1);
	filestore->SetTypeL(uidtype);
	RStoreWriteStream out;
	TStreamId streamid=out.CreateLC(*filestore);							// Push to stack
	mtmgroupdata->ExternalizeL(out);
	out.CommitL();
	CleanupStack::PopAndDestroy(); // out
	filestore->SetRootL(streamid);
	filestore->CommitL();
	CleanupStack::PopAndDestroy(); // filestore
	
	CleanupStack::PopAndDestroy();  // mtmgroupdata
	}

void CheckNumberOfEntriesL(CMsvEntry &aEntry,TMsvId aId,TInt aRequiredNo)
	{
	if(aEntry.EntryId()==aId)
		{
		test(aEntry.Count()==aRequiredNo);
		if(aRequiredNo==0) test(aEntry.Entry().Owner()==EFalse);
		else test(aEntry.Entry().Owner()!=EFalse);
		CTestTimer *timer=CTestTimer::NewL();
		timer->After(100000);
		CActiveScheduler::Start();
		delete timer;
		test(aEntry.Count()==aRequiredNo);
		if(aRequiredNo==0) test(aEntry.Entry().Owner()==EFalse);
		else test(aEntry.Entry().Owner()!=EFalse);
		aEntry.SetEntryL(KMsvRootIndexEntryId);
		}
	aEntry.SetEntryL(aId);
	test(aEntry.Count()==aRequiredNo);
	if(aRequiredNo==0) test(aEntry.Entry().Owner()==EFalse);
	else test(aEntry.Entry().Owner()!=EFalse);
	}


LOCAL_C void TestCreateSyncL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	cEntry->CreateL(entry);

	CheckNumberOfEntriesL(*cEntry,folder.Id(),1);
	cEntry->SetEntryL(entry.Id());

	CleanupStack::PopAndDestroy(3); // ob, session, cEntry
	}



LOCAL_C void TestCreateSyncWithHeapFailL()
	{
	RMsvServerSession server;
	CleanupClosePushL(server);
	server.Connect(theUtils->FileSession());

	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	
	// Test heap failure in the client
	TInt error;
	TInt failCount = 0;
	TBool finished = EFalse;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		TRAP(error, cEntry->CreateL(entry));

		__UHEAP_RESET;

		if (error == KErrNone)
			{
			CheckNumberOfEntriesL(*cEntry,folder.Id(),1);
			cEntry->SetEntryL(entry.Id());
			finished = ETrue;
			}
		else
			{
			test(error == KErrNoMemory);
			CheckNumberOfEntriesL(*cEntry,folder.Id(),0);
			}
		}

	cEntry->SetEntryL(folder.Id());

	// Test heap failure in the server

	failCount = 0;
	finished = EFalse;

	while(!finished)
		{
		server.SetFailure(EHeapFailure, RHeap::EFailNext, failCount++);

		TRAP(error, cEntry->CreateL(entry));

		server.SetFailure(EHeapFailure, RHeap::ENone);

		if (error == KErrNone)
			{
			CheckNumberOfEntriesL(*cEntry,folder.Id(),2);
			cEntry->SetEntryL(entry.Id());
			finished = ETrue;
			}
		else
			{
			test(error == KErrNoMemory);
			CheckNumberOfEntriesL(*cEntry,folder.Id(),1);
			}
		}

	CleanupStack::PopAndDestroy(4, &server); // ob, session, cEntry
	server.Close();
	}


LOCAL_C void TestCreateSyncWithFileFailL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	
	TInt error;
	TInt failCount = 0;
	TBool finished = EFalse;

	while(!finished)
		{
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).


		if(failCount==7)
			failCount++;
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		TRAP(error, cEntry->CreateL(entry));

		theUtils->FileSession().SetErrorCondition(KErrGeneral, KMaxTInt);

		if (error == KErrNone)
			{
			CheckNumberOfEntriesL(*cEntry,folder.Id(),1);
			cEntry->SetEntryL(entry.Id());
			finished = ETrue;
			}
		else
			{
			test(error == KErrGeneral);
			CheckNumberOfEntriesL(*cEntry,folder.Id(),0);
			}
		}

	CleanupStack::PopAndDestroy(3); // ob, session, cEntry
	}


LOCAL_C void TestMoveSyncL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder1);

	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder2);

	cEntry->SetEntryL(folder1.Id());

	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry);

	CheckNumberOfEntriesL(*cEntry,folder2.Id(),0);
	CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);

	cEntry->MoveL(entry.Id(),folder2.Id());

	CheckNumberOfEntriesL(*cEntry,folder1.Id(),0);
	CheckNumberOfEntriesL(*cEntry,folder2.Id(),1);

	CleanupStack::PopAndDestroy(3,ob); // ob, session, cEntry
	}



LOCAL_C void TestMoveSyncWithHeapFailL()
	{
	RMsvServerSession server;
	CleanupClosePushL(server);
	server.Connect(theUtils->FileSession());

	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder1);

	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder2);

	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	cEntry->SetEntryL(folder1.Id());
	cEntry->CreateL(entry);


	// Test heap failure in the client
	TInt error;
	TInt failCount = 0;
	TBool finished = EFalse;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
	
		TRAP(error, cEntry->MoveL(entry.Id(),folder2.Id()));

		__UHEAP_RESET;

		if (error == KErrNone)
			{			
			CheckNumberOfEntriesL(*cEntry,folder1.Id(),0);
			CheckNumberOfEntriesL(*cEntry,folder2.Id(),1);
			finished = ETrue;
			}
		else
			{
			test(error == KErrNoMemory);
			CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
			}
		}

	
	// Test heap failure in the server

	failCount = 0;
	finished = EFalse;
	cEntry->SetEntryL(folder2.Id());
	while(!finished)
		{
		server.SetFailure(EHeapFailure, RHeap::EFailNext, failCount++);

		TRAP(error, cEntry->MoveL(entry.Id(),folder1.Id()));

		server.SetFailure(EHeapFailure, RHeap::ENone);

		if (error == KErrNone)
			{			
			CheckNumberOfEntriesL(*cEntry,folder2.Id(),0);
			CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
			finished = ETrue;
			}
		else
			{
			test(error == KErrNoMemory);
			CheckNumberOfEntriesL(*cEntry,folder2.Id(),1);
			}
		}

	CleanupStack::PopAndDestroy(4,&server); // ob, session, cEntry, server
	server.Close();
	}



LOCAL_C void TestCopySyncL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder1);

	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder2);

	cEntry->SetEntryL(folder1.Id());

	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry);
	
	CheckNumberOfEntriesL(*cEntry,folder2.Id(),0);
	CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
	cEntry->CopyL(entry.Id(),folder2.Id());

	CheckNumberOfEntriesL(*cEntry,folder2.Id(),1);
	CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);

	CleanupStack::PopAndDestroy(3,ob); // ob, session, cEntry
	}



LOCAL_C void TestCopySyncWithClientHeapFailL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder1);

	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder2);

	cEntry->SetEntryL(folder1.Id());

	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry);
	
	cEntry->SetEntryL(folder2.Id());
	test(cEntry->Count() == 0);
	cEntry->SetEntryL(folder1.Id());
	test(cEntry->Count() == 1);

	// Test heap failure in the client
	TInt error;
#ifdef _DEBUG
	TInt failCount = 0;
#endif
	TBool finished = EFalse;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		TRAP(error, cEntry->CopyL(entry.Id(),folder2.Id()));

		__UHEAP_RESET;

		if (error == KErrNone)
			{			
			CheckNumberOfEntriesL(*cEntry,folder2.Id(),1);
			CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
			finished = ETrue;
			}
		else
			{
			test(error == KErrNoMemory);
			CheckNumberOfEntriesL(*cEntry,folder2.Id(),0);
			CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
			}
		}
	CleanupStack::PopAndDestroy(3,ob); // ob, session, cEntry
	}

LOCAL_C void TestCopySyncWithServerHeapFailL()
	{
	RMsvServerSession server;
	CleanupClosePushL(server);
	server.Connect(theUtils->FileSession());

	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder1);

	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder2);

	cEntry->SetEntryL(folder1.Id());

	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry);
	
	cEntry->SetEntryL(folder2.Id());
	test(cEntry->Count() == 0);
	cEntry->SetEntryL(folder1.Id());
	test(cEntry->Count() == 1);

	// Test heap failure in the client
	TInt error;
	TInt failCount = 0;
	TBool finished = EFalse;

	while(!finished)
		{
		server.SetFailure(EHeapFailure, RHeap::EFailNext, failCount++);
	
		TRAP(error, cEntry->CopyL(entry.Id(),folder2.Id()));

		server.SetFailure(EHeapFailure, RHeap::ENone);

		if (error == KErrNone)
			{			
			CheckNumberOfEntriesL(*cEntry,folder2.Id(),1);
			CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
			finished = ETrue;
			}
		else
			{
			test(error == KErrNoMemory);
			CheckNumberOfEntriesL(*cEntry,folder2.Id(),0);
			CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
			}
		}

	CleanupStack::PopAndDestroy(4,&server); // ob, session, cEntry, server
	server.Close();
	}



LOCAL_C void TestCopySameSyncL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// add folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder1);
	cEntry->SetEntryL(folder1.Id());

	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry);
	
	CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
	cEntry->CopyL(entry.Id(),folder1.Id());
	CheckNumberOfEntriesL(*cEntry,folder1.Id(),2);

	CleanupStack::PopAndDestroy(3,ob); // ob, session, cEntry
	}



LOCAL_C void TestCopySameSyncWithClientHeapFailL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// add folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder1);

	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->SetEntryL(folder1.Id());
	cEntry->CreateL(entry);


	// Test heap failure in the client
	TInt error;
#ifdef _DEBUG
	TInt failCount = 0;
#endif
	TBool finished = EFalse;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
	
		TRAP(error, cEntry->CopyL(entry.Id(),folder1.Id()));

		__UHEAP_RESET;

		if (error == KErrNone)
			{			
			CheckNumberOfEntriesL(*cEntry,folder1.Id(),2);
			finished = ETrue;
			}
		else
			{
			test(error == KErrNoMemory);
			CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
			}
		}

	

	CleanupStack::PopAndDestroy(3,ob); // ob, session, cEntry
	}



LOCAL_C void TestCopySameSyncWithServerHeapFailL()
	{
	RMsvServerSession server;
	CleanupClosePushL(server);
	server.Connect(theUtils->FileSession());

	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// add folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder1);

	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->SetEntryL(folder1.Id());
	cEntry->CreateL(entry);


	// Test heap failure in the server

	TInt error;
	TInt failCount = 0;
	TBool finished = EFalse;

	while(!finished)
		{
		server.SetFailure(EHeapFailure, RHeap::EFailNext, failCount++);
	
		TRAP(error, cEntry->CopyL(entry.Id(),folder1.Id()));

		server.SetFailure(EHeapFailure, RHeap::ENone);

		if (error == KErrNone)
			{			
			CheckNumberOfEntriesL(*cEntry,folder1.Id(),2);
			finished = ETrue;
			}
		else
			{
			test(error == KErrNoMemory);
			CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
			}
		}

	

	CleanupStack::PopAndDestroy(4,&server); // ob, session, cEntry, server
	server.Close();
	}





LOCAL_C void TestChangeSyncL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	CEntryObserver* entryOb = new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);
	cEntry->AddObserverL(*entryOb);

	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	cEntry->CreateL(entry);
	cEntry->SetEntryL(entry.Id());

	// alter entry
	entry.iDescription.Set(KLongDescription);
	entry.iDetails.Set(KLongDetails);

	entryOb->Start();
	cEntry->ChangeL(entry);
	test(cEntry->Entry() == entry);

	CActiveScheduler::Start(); // Wait for change notification
	test(entryOb->iEvent == MMsvEntryObserver::EMsvEntryChanged);
	test(cEntry->Entry() == entry);

	cEntry->SetEntryL(folder.Id());
	cEntry->SetEntryL(entry.Id());

	test(cEntry->Entry() == entry);

	CleanupStack::PopAndDestroy(4); // ob, session, cEntry, entryOb
	}


LOCAL_C void TestChangeSyncWithHeapFailL()
	{
	RMsvServerSession server;
	CleanupClosePushL(server);
	server.Connect(theUtils->FileSession());

	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry oldEntry;
	oldEntry.iDescription.Set(_L(""));
	oldEntry.iDetails.Set(_L(""));
	oldEntry.iType = KUidMsvMessageEntry;
	oldEntry.iMtm = KUidMsvLocalServiceMtm;
	oldEntry.iServiceId = KMsvLocalServiceIndexEntryId;

	cEntry->CreateL(oldEntry);
	cEntry->SetEntryL(oldEntry.Id());
	oldEntry = cEntry->Entry();
	
	// alter entry
	TMsvEntry newEntry(oldEntry);
	newEntry.iDescription.Set(KShortDescription);
	newEntry.iDetails.Set(KShortDetails);

	// Test heap failure in the client

	TInt error;
	TInt failCount = 0;
	TBool finished = EFalse;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		TRAP(error, cEntry->ChangeL(newEntry));

		__UHEAP_RESET;

		cEntry->SetEntryL(folder.Id());
		cEntry->SetEntryL(oldEntry.Id());

		if (error == KErrNone)
			{
			test(cEntry->Entry() == newEntry);
			finished = ETrue;
			}
		else
			{
			test(error == KErrNoMemory);
			test(cEntry->Entry() == oldEntry);
			}
		}

	oldEntry.iDescription.Set(KShortDescription);
	oldEntry.iDetails.Set(KShortDetails);
	newEntry.iDescription.Set(KLongDescription);
	newEntry.iDetails.Set(KLongDetails);

	// Test heap failure in the server

	failCount = 0;
	finished = EFalse;

	while(!finished)
		{
		server.SetFailure(EHeapFailure, RHeap::EFailNext, failCount++);

		TRAP(error, cEntry->ChangeL(newEntry));

		server.SetFailure(EHeapFailure, RHeap::ENone);

		cEntry->SetEntryL(folder.Id());
		cEntry->SetEntryL(oldEntry.Id());

		if (error == KErrNone)
			{
			test(cEntry->Entry() == newEntry);
			finished = ETrue;
			}
		else
			{
			test(error == KErrNoMemory);
			test(cEntry->Entry() == oldEntry);
			}
		}

	CleanupStack::PopAndDestroy(4, &server); // ob, session, cEntry, server
	server.Close();
	}


LOCAL_C void TestChangeSyncWithFileFailL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry oldEntry;
	oldEntry.iDescription.Set(KShortDescription);
	oldEntry.iDetails.Set(KShortDetails);
	oldEntry.iType = KUidMsvMessageEntry;
	oldEntry.iMtm = KUidMsvLocalServiceMtm;
	oldEntry.iServiceId = KMsvLocalServiceIndexEntryId;

	cEntry->CreateL(oldEntry);
	cEntry->SetEntryL(oldEntry.Id());
	oldEntry = cEntry->Entry();

	// alter entry
	TMsvEntry newEntry(oldEntry);
	newEntry.iDescription.Set(KLongDescription);
	newEntry.iDetails.Set(KLongDetails);

	TInt error;
	TInt failCount = 0;
	TBool finished = EFalse;

	while(!finished)
		{
		// Skip file system problem
		if (failCount == 8)
			failCount++;

		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		TRAP(error, cEntry->ChangeL(newEntry));

		theUtils->FileSession().SetErrorCondition(KErrGeneral, KMaxTInt);

		cEntry->SetEntryL(folder.Id());
		cEntry->SetEntryL(oldEntry.Id());

		if (error == KErrNone)
			{
			test(cEntry->Entry() == newEntry);
			finished = ETrue;
			}
		else
			{
			test(error == KErrGeneral);
			test(cEntry->Entry() == oldEntry);
			}
		}

	CleanupStack::PopAndDestroy(3); // ob, session, cEntry
	}


LOCAL_C void TestDeleteSyncL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	cEntry->CreateL(entry);

	CheckNumberOfEntriesL(*cEntry,folder.Id(),1);

	cEntry->DeleteL(entry.Id());

	CheckNumberOfEntriesL(*cEntry,folder.Id(),0);
	CleanupStack::PopAndDestroy(3); // ob, session, cEntry
	}

LOCAL_C void TestDeleteSyncWithHeapFailL()
	{
	RMsvServerSession server;
	CleanupClosePushL(server);
	server.Connect(theUtils->FileSession());

	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	cEntry->CreateL(entry);

	test(cEntry->Entry().Owner());
	test(cEntry->Count() == 1);

	TInt error;
	TInt failCount = 0;
	TBool finished = EFalse;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		TRAP(error, cEntry->DeleteL(entry.Id()));

		__UHEAP_RESET;

		if (error == KErrNone)
			{
			CheckNumberOfEntriesL(*cEntry,folder.Id(),0);
			finished = ETrue;
			}
		else
			{
			test(error == KErrNoMemory);
			CheckNumberOfEntriesL(*cEntry,folder.Id(),1);
			cEntry->SetEntryL(entry.Id());
			cEntry->SetEntryL(folder.Id());
			}
		}

	cEntry->CreateL(entry);

	CheckNumberOfEntriesL(*cEntry,folder.Id(),1);

	failCount = 0;
	finished = EFalse;

	while(!finished)
		{
		server.SetFailure(EHeapFailure, RHeap::EFailNext, failCount++);

		TRAP(error, cEntry->DeleteL(entry.Id()));

		server.SetFailure(EHeapFailure, RHeap::ENone);

		if (error == KErrNone)
			{
			CheckNumberOfEntriesL(*cEntry,folder.Id(),0);
			finished = ETrue;
			}
		else
			{
			test(error == KErrNoMemory);
			CheckNumberOfEntriesL(*cEntry,folder.Id(),1);
			cEntry->SetEntryL(entry.Id());
			cEntry->SetEntryL(folder.Id());
			}
		}

	CleanupStack::PopAndDestroy(4, &server); // ob, session, cEntry, server
	}

LOCAL_C void TestDeleteSyncWithFileFailL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	cEntry->CreateL(entry);

	test(cEntry->Entry().Owner());
	test(cEntry->Count() == 1);

	TInt error;
	TInt failCount = 0;
	TBool finished = EFalse;

	while(!finished)
		{
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).


		if (failCount == 13)
			failCount++;

		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		TRAP(error, cEntry->DeleteL(entry.Id()));

		theUtils->FileSession().SetErrorCondition(KErrGeneral, KMaxTInt);

		if (error == KErrNone)
			{
			CheckNumberOfEntriesL(*cEntry,folder.Id(),0);
			finished = ETrue;
			}
		else
			{
			test(error == KErrGeneral);
			CheckNumberOfEntriesL(*cEntry,folder.Id(),1);
			cEntry->SetEntryL(entry.Id());
			cEntry->SetEntryL(folder.Id());
			}
		}

	CleanupStack::PopAndDestroy(3); // ob, session, cEntry
	}

LOCAL_C void TestTransferCommandSyncL()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	TInt ret = session->InstallMtmGroup(KDataComponentFilename);
	test(ret==KErrNone|| ret==KErrAlreadyExists);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// Service
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	TMsvEntry service;
	service.iType=KUidMsvServiceEntry;
	service.iMtm = KUidTestServerMtmType;
	cEntry->CreateL(service);

	// Folder
	cEntry->SetEntryL(KMsvLocalServiceIndexEntryId);
	TMsvEntry folder;
	folder.iType=KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);

	// Entry
	cEntry->SetEntryL(folder.Id());
	TMsvEntry entry;
	entry.iType=KUidMsvMessageEntry;
	entry.iMtm = KUidTestServerMtmType;
	entry.iServiceId = service.Id();
	cEntry->CreateL(entry);

	TBuf8<256> progress;
	TBuf8<32> param;

	selection->AppendL(entry.Id());

	// Delay of 4 seconds
	session->TransferCommandL(*selection, 4, param, progress);
	test(progress==_L8("timer completed"));

	// Set buffer
	param = _L8("test persistent buffer");
	session->TransferCommandL(*selection, 10, param, progress);
	test(progress==_L8("persistent"));
	
	// Restore buffer
	session->TransferCommandL(*selection, 11, param, progress);
	test(progress==_L8("test persistent buffer"));

	// Set progress
	param = _L8("this is the progress");
	session->TransferCommandL(*selection, 12, param, progress);
	test(progress==_L8("this is the progress"));

	// Forced leave
	TRAPD(error, session->TransferCommandL(*selection, 13, param, progress));
	test(error == -1300);

	CleanupStack::PopAndDestroy(4); // ob, session, cEntry, selection
	}

LOCAL_C void TestChangeAttributesL()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// Folder
	TMsvEntry folder;
	folder.iType=KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry1);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, entry1.Id(), TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);

	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KLongDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry2);

	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, entry2.Id(), TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);

	TMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry3);

	CMsvEntry* cEntry3 = CMsvEntry::NewL(*session, entry3.Id(), TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry3);

	// test
	test(!cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected());
	test(!cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(!cEntry2->Entry().Connected());
	test(!cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected());

	// work with all entries
	CMsvEntrySelection* selection=new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// change nothing
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());

	cEntry->ChangeAttributesL(*selection, 0, 0);

	// test
	test(!cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected());
	test(!cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(!cEntry2->Entry().Connected());
	test(!cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected());

	// change nothing that is not already set
	cEntry->ChangeAttributesL(*selection, KMsvVisibilityAttribute, 0);

	// test
	test(!cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected());
	test(!cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(!cEntry2->Entry().Connected());
	test(!cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected());

	// change nothing that is not already set
	cEntry->ChangeAttributesL(*selection, KMsvVisibilityAttribute, KMsvUnreadAttribute);

	// test
	test(!cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected());
	test(!cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(!cEntry2->Entry().Connected());
	test(!cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected());

	// set invisible
	cEntry->ChangeAttributesL(*selection, 0, KMsvVisibilityAttribute);

	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(!cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(!cEntry2->Entry().Unread()); test(!cEntry2->Entry().Visible()); test(!cEntry2->Entry().Connected());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(!cEntry3->Entry().Unread()); test(!cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected());

	// set connected
	cEntry->ChangeAttributesL(*selection, KMsvConnectedAttribute, 0);

	// test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(!cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(cEntry1->Entry().Connected());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(!cEntry2->Entry().Unread()); test(!cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(!cEntry3->Entry().Unread()); test(!cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected());

	// set visible clear connected
	cEntry->ChangeAttributesL(*selection, KMsvVisibilityAttribute, KMsvConnectedAttribute);

	// test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(!cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(!cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(!cEntry2->Entry().Connected());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(!cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected());

	// set connected & unread
	cEntry->ChangeAttributesL(*selection, KMsvUnreadAttribute|KMsvConnectedAttribute, 0);

	// test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(cEntry1->Entry().Connected());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected());

	// entry1
	selection->Reset();
	selection->AppendL(entry1.Id());
	cEntry->ChangeAttributesL(*selection, 0, KMsvVisibilityAttribute|KMsvUnreadAttribute|KMsvConnectedAttribute);

	// test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(!cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected());

	// entry2&3
	selection->Reset();
	selection->AppendL(entry2.Id()); selection->AppendL(entry3.Id());
	cEntry->ChangeAttributesL(*selection, 0, KMsvVisibilityAttribute|KMsvUnreadAttribute|KMsvConnectedAttribute);

	// test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(!cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(!cEntry2->Entry().Unread()); test(!cEntry2->Entry().Visible()); test(!cEntry2->Entry().Connected());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(!cEntry3->Entry().Unread()); test(!cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected());

	// entry2
	selection->Reset();
	selection->AppendL(entry2.Id());
	cEntry->ChangeAttributesL(*selection, KMsvVisibilityAttribute|KMsvUnreadAttribute|KMsvConnectedAttribute, 0);

	// test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(!cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(!cEntry3->Entry().Unread()); test(!cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected());

	// entry2&3
	selection->Reset();
	selection->AppendL(entry2.Id()); selection->AppendL(entry3.Id());
	cEntry->ChangeAttributesL(*selection, KMsvVisibilityAttribute|KMsvUnreadAttribute|KMsvConnectedAttribute, 0);

	// test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(!cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected());

	// entry1&2&3
	selection->Reset();
	selection->AppendL(entry1.Id()); selection->AppendL(entry2.Id()); selection->AppendL(entry3.Id());
	cEntry->ChangeAttributesL(*selection, KMsvVisibilityAttribute|KMsvUnreadAttribute|KMsvConnectedAttribute, 0);

	// test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(cEntry1->Entry().Connected());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected());

	// entry1
	selection->Reset();
	selection->AppendL(entry1.Id()); 
	cEntry->ChangeAttributesL(*selection, 0, KMsvVisibilityAttribute);

	// test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(cEntry1->Entry().Connected());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected());

	// entry1&2
	selection->Reset();
	selection->AppendL(entry1.Id()); selection->AppendL(entry2.Id());
	cEntry->ChangeAttributesL(*selection, 0, KMsvVisibilityAttribute|KMsvUnreadAttribute);

	// test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(!cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(cEntry1->Entry().Connected());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(!cEntry2->Entry().Unread()); test(!cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected());

	// entry1&2&3
	selection->Reset();
	selection->AppendL(entry1.Id()); selection->AppendL(entry2.Id()); selection->AppendL(entry3.Id());
	cEntry->ChangeAttributesL(*selection, 0, KMsvVisibilityAttribute|KMsvUnreadAttribute);

	// test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(!cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(cEntry1->Entry().Connected());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(!cEntry2->Entry().Unread()); test(!cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(!cEntry3->Entry().Unread()); test(!cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected());

	// test the long descriptors
	selection->Reset();
	selection->AppendL(entry1.Id());selection->AppendL(entry2.Id());selection->AppendL(entry3.Id());
	cEntry->ChangeAttributesL(*selection, KMsvVisibilityAttribute, KMsvUnreadAttribute|KMsvConnectedAttribute);
	
	// The visible folder flag is set for entry1, entry2 and entry3
	// by message server while they were created. This flag is not 
	// reflected in the local copy of entry objects. Hence setting 
	// the flag. Visible Folder Flag is introduced in PREQ 1189 (TB9.2).
	entry1.SetVisibleFolderFlag(EFalse);
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(cEntry1->Entry()==entry1);
	
	entry2.SetVisibleFolderFlag(EFalse);
	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(cEntry2->Entry()==entry2);
	
	entry3.SetVisibleFolderFlag(EFalse);
	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(cEntry3->Entry()==entry3);

	CleanupStack::PopAndDestroy(7); // selection, cEntry3, cEntry2, cEntry1, cEntry, session, ob
	}

LOCAL_C void TestChangeAttributesWithHeapFailL()
	{
	RMsvServerSession server;
	CleanupClosePushL(server);
	server.Connect(theUtils->FileSession());

	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// Folder
	TMsvEntry folder;
	folder.iType=KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry1);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, entry1.Id(), TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);

	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KLongDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry2);

	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, entry2.Id(), TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);

	TMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry3);

	CMsvEntry* cEntry3 = CMsvEntry::NewL(*session, entry3.Id(), TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry3);

	// work with all entries
	CMsvEntrySelection* selection=new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// change nothing
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());

	TInt error;
	TInt failCount = 0;
	TBool finished = EFalse;

	while(!finished)
		{
		server.SetFailure(EHeapFailure, RHeap::EFailNext, failCount++);

		// Negate the flags
		TRAP(error, cEntry->ChangeAttributesL(*selection, KMsvUnreadAttribute|KMsvConnectedAttribute, KMsvVisibilityAttribute));
		
		server.SetFailure(EHeapFailure, RHeap::ENone);

		if (error == KErrNone)
			{
			finished = ETrue;

			// test
			cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
			test(cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(cEntry1->Entry().Connected());
			cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
			test(cEntry2->Entry().Unread()); test(!cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected());
			cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
			test(cEntry3->Entry().Unread()); test(!cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected());
			}
		else
			{
			test(error == KErrNoMemory);

			cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
			test(!cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected());
			cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
			test(!cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(!cEntry2->Entry().Connected());
			cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
			test(!cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected());
			}
		}

	CleanupStack::PopAndDestroy(8,&server); // selection, cEntry3, cEntry2, cEntry1, cEntry, session, ob, server
	}

LOCAL_C void TestChangeAttributesWithFileFailL()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// Folder
	TMsvEntry folder;
	folder.iType=KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry1);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, entry1.Id(), TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);

	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KLongDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry2);

	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, entry2.Id(), TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);

	TMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry3);

	CMsvEntry* cEntry3 = CMsvEntry::NewL(*session, entry3.Id(), TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry3);

	// work with all entries
	CMsvEntrySelection* selection=new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// change nothing
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());

	TInt error;
	TInt failCount = 0;
	TBool finished = EFalse;

	while(!finished)
		{
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).


		if (failCount == 8)
			failCount+=1;

		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		// Negate the flags
		TRAP(error, cEntry->ChangeAttributesL(*selection, KMsvUnreadAttribute|KMsvConnectedAttribute, KMsvVisibilityAttribute));
		
		theUtils->FileSession().SetErrorCondition(KErrGeneral, KMaxTInt);

		if (error == KErrNone)
			{
			finished = ETrue;

			// test
			cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
			test(cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(cEntry1->Entry().Connected());
			cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
			test(cEntry2->Entry().Unread()); test(!cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected());
			cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
			test(cEntry3->Entry().Unread()); test(!cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected());
			}
		else
			{
			test(error == KErrGeneral);

			cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
			test(!cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected());
			cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
			test(!cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(!cEntry2->Entry().Connected());
			cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
			test(!cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected());
			}
		}

	CleanupStack::PopAndDestroy(7); // selection, cEntry3, cEntry2, cEntry1, cEntry, session, ob
	}

LOCAL_C void TestPcSyncL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// Create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);

	cEntry->SetEntryL(folder.Id());

	// Create an entry
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry1);

	// Create an entry
	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry2);

	// Create an entry
	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry3);

	// Create an entry
	TMsvEntry entry4;
	entry4.iDescription.Set(KShortDescription);
	entry4.iDetails.Set(KShortDetails);
	entry4.iType = KUidMsvMessageEntry;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry4);

	// Create an entry
	TMsvEntry entry5;
	entry5.iDescription.Set(KShortDescription);
	entry5.iDetails.Set(KShortDetails);
	entry5.iType = KUidMsvMessageEntry;
	entry5.iMtm = KUidMsvLocalServiceMtm;
	entry5.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry5);
	
#if (defined SYMBIAN_MESSAGESTORE_USING_SQLDB)
	// Increment sync count
	selection->AppendL(entry5.Id());
	session->IncPcSyncCountL(*selection);
	
	// Increment sync count
	selection->AppendL(entry4.Id());
	session->IncPcSyncCountL(*selection);

	// Increment sync count
	selection->AppendL(entry3.Id());
	session->IncPcSyncCountL(*selection);
	
	// Increment sync count
	selection->AppendL(entry2.Id());
	session->IncPcSyncCountL(*selection);
	
	// Increment sync count
	selection->AppendL(entry1.Id());
	session->IncPcSyncCountL(*selection);
#else
	// Increment sync count
	selection->AppendL(entry1.Id());
	session->IncPcSyncCountL(*selection);
	
	// Increment sync count
	selection->AppendL(entry2.Id());
	session->IncPcSyncCountL(*selection);
	
	// Increment sync count
	selection->AppendL(entry3.Id());
	session->IncPcSyncCountL(*selection);
	
	// Increment sync count
	selection->AppendL(entry4.Id());
	session->IncPcSyncCountL(*selection);
	
	// Increment sync count
	selection->AppendL(entry5.Id());
	session->IncPcSyncCountL(*selection);
	
#endif

	test(cEntry->Count() == 5);

	TMsvLocalOperationProgress progress;
	cEntry->DeleteL(*selection, progress);

	test(cEntry->Count() == 0);
	cEntry->SetEntryL(KMsvDeletedEntryFolderEntryId);
	test(cEntry->Count() == 5);

	for(TInt count = 5; count > 0; count--)
		{
		// Decrement the sync count
		session->DecPcSyncCountL(*selection);
		selection->Delete(selection->Count() - 1);

		// Wait for entries to be deleted
		CTestTimer* timer = CTestTimer::NewL();
		timer->After(1000000);
		CActiveScheduler::Start();
		delete timer;

		// Should now be four entries in delete folder
		cEntry->SetEntryL(folder.Id());
		cEntry->SetEntryL(KMsvDeletedEntryFolderEntryId);
		
		test(cEntry->Count() == selection->Count());
		}

	// Check error handling
	selection->AppendL(entry1.Id());
	TRAPD(error, session->IncPcSyncCountL(*selection));
	test(error == KErrNotFound);

	CleanupStack::PopAndDestroy(4); // ob, session, cEntry, selection
	}

LOCAL_C void TestPcSyncWithHeapFailL()
	{
	RMsvServerSession server;
	CleanupClosePushL(server);
	server.Connect(theUtils->FileSession());

	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// Create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);

	cEntry->SetEntryL(folder.Id());

	// Create an entry
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry1);
	selection->AppendL(entry1.Id());

	// Create an entry
	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry2);
	selection->AppendL(entry2.Id());

	// Create an entry
	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry3);
	selection->AppendL(entry3.Id());

	// Create an entry
	TMsvEntry entry4;
	entry4.iDescription.Set(KShortDescription);
	entry4.iDetails.Set(KShortDetails);
	entry4.iType = KUidMsvMessageEntry;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry4);
	selection->AppendL(entry4.Id());

	// Create an entry
	TMsvEntry entry5;
	entry5.iDescription.Set(KShortDescription);
	entry5.iDetails.Set(KShortDetails);
	entry5.iType = KUidMsvMessageEntry;
	entry5.iMtm = KUidMsvLocalServiceMtm;
	entry5.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry5);
	selection->AppendL(entry5.Id());

	test(cEntry->Count() == 5);
	session->IncPcSyncCountL(*selection);

	TMsvLocalOperationProgress progress;
	cEntry->DeleteL(*selection, progress);

	test(cEntry->Count() == 0);
	cEntry->SetEntryL(KMsvDeletedEntryFolderEntryId);
	test(cEntry->Count() == 5);

	TInt error;
	TInt failCount = 0;
	TBool finished = EFalse;

	while(!finished)
		{
		server.SetFailure(EHeapFailure, RHeap::EFailNext, failCount++);

		// Decrement the sync count
		TRAP(error, session->DecPcSyncCountL(*selection));

		server.SetFailure(EHeapFailure, RHeap::ENone);

		if (error == KErrNone)
			finished = ETrue;
		else
			{
			test(error == KErrNoMemory);

			cEntry->SetEntryL(entry1.Id());
			test(cEntry->Entry().PcSyncCount() == 1);

			cEntry->SetEntryL(entry2.Id());
			test(cEntry->Entry().PcSyncCount() == 1);
			
			cEntry->SetEntryL(entry3.Id());
			test(cEntry->Entry().PcSyncCount() == 1);
			
			cEntry->SetEntryL(entry4.Id());
			test(cEntry->Entry().PcSyncCount() == 1);
			
			cEntry->SetEntryL(entry4.Id());
			test(cEntry->Entry().PcSyncCount() == 1);
			}

		}

	CleanupStack::PopAndDestroy(5, &server); // ob, session, cEntry, selection, server
	server.Close();
	}

LOCAL_C void TestPcSyncWithFileFailL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// Create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);

	cEntry->SetEntryL(folder.Id());

	// Create an entry
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry1);
	selection->AppendL(entry1.Id());

	// Create an entry
	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KShortDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry2);
	selection->AppendL(entry2.Id());

	// Create an entry
	TMsvEntry entry3;
	entry3.iDescription.Set(KShortDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry3);
	selection->AppendL(entry3.Id());

	// Create an entry
	TMsvEntry entry4;
	entry4.iDescription.Set(KShortDescription);
	entry4.iDetails.Set(KShortDetails);
	entry4.iType = KUidMsvMessageEntry;
	entry4.iMtm = KUidMsvLocalServiceMtm;
	entry4.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry4);
	selection->AppendL(entry4.Id());

	// Create an entry
	TMsvEntry entry5;
	entry5.iDescription.Set(KShortDescription);
	entry5.iDetails.Set(KShortDetails);
	entry5.iType = KUidMsvMessageEntry;
	entry5.iMtm = KUidMsvLocalServiceMtm;
	entry5.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry5);
	selection->AppendL(entry5.Id());

	test(cEntry->Count() == 5);
	session->IncPcSyncCountL(*selection);

	TMsvLocalOperationProgress progress;
	cEntry->DeleteL(*selection, progress);

	test(cEntry->Count() == 0);

	TInt error;
	TInt failCount = 0;
	TBool finished = EFalse;

	while(!finished)
		{
// Skip this count value because it causes file corruption that ruins the 
// rest of the testing.
// If the messaging server changes, this value may become invalid. In that
// case, one way to find the new value is to comment out the skip, find 
// the value that gives the error -20 (KErrCorrupt) instead of -2 (KErrGeneral)
// and subtract 2 or 3 (usually 2).

		if (failCount == 8)
			failCount+=1;

		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		// Decrement the sync count
		TRAP(error, session->DecPcSyncCountL(*selection));

		theUtils->FileSession().SetErrorCondition(KErrGeneral, KMaxTInt);

		if (error == KErrNone)
			finished = ETrue;
		else
			{
			test(error == KErrGeneral);
			
			cEntry->SetEntryL(entry1.Id());
			test(cEntry->Entry().PcSyncCount() == 1);

			cEntry->SetEntryL(entry2.Id());
			test(cEntry->Entry().PcSyncCount() == 1);
			
			cEntry->SetEntryL(entry3.Id());
			test(cEntry->Entry().PcSyncCount() == 1);
			
			cEntry->SetEntryL(entry4.Id());
			test(cEntry->Entry().PcSyncCount() == 1);
			
			cEntry->SetEntryL(entry4.Id());
			test(cEntry->Entry().PcSyncCount() == 1);
			}
		}

	CleanupStack::PopAndDestroy(4); // ob, session, cEntry, selection
	}


LOCAL_C void TestChangePendingDeleteAttributesL()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// Folder
	TMsvEntry folder;
	folder.iType=KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder);
	
	cEntry->SetEntryL(folder.Id());

	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry1);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, entry1.Id(), TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);

	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KLongDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry2);

	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, entry2.Id(), TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);

	TMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KLongDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry3);

	CMsvEntry* cEntry3 = CMsvEntry::NewL(*session, entry3.Id(), TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry3);

	// test
	test(!cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected()); test(!cEntry1->Entry().PendingDelete());
	test(!cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(!cEntry2->Entry().Connected()); test(!cEntry2->Entry().PendingDelete());
	test(!cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected()); test(!cEntry3->Entry().PendingDelete());

	// work with all entries
	CMsvEntrySelection* selection=new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// change nothing
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());

	cEntry->ChangeAttributesL(*selection, 0, 0);

	// test
	test(!cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected()); test(!cEntry1->Entry().PendingDelete());
	test(!cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(!cEntry2->Entry().Connected()); test(!cEntry2->Entry().PendingDelete());
	test(!cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected()); test(!cEntry3->Entry().PendingDelete());

	// change nothing that is not already set
	cEntry->ChangeAttributesL(*selection, KMsvVisibilityAttribute, 0);

	// test
	test(!cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected()); test(!cEntry1->Entry().PendingDelete());
	test(!cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(!cEntry2->Entry().Connected()); test(!cEntry2->Entry().PendingDelete());
	test(!cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected()); test(!cEntry3->Entry().PendingDelete());

	// change nothing that is not already set
	cEntry->ChangeAttributesL(*selection, KMsvVisibilityAttribute, KMsvUnreadAttribute);

	// test
	test(!cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected()); test(!cEntry1->Entry().PendingDelete());
	test(!cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(!cEntry2->Entry().Connected()); test(!cEntry2->Entry().PendingDelete());
	test(!cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected()); test(!cEntry3->Entry().PendingDelete());

	// set invisible
	cEntry->ChangeAttributesL(*selection, 0, KMsvVisibilityAttribute);

	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(!cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected()); test(!cEntry1->Entry().PendingDelete());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(!cEntry2->Entry().Unread()); test(!cEntry2->Entry().Visible()); test(!cEntry2->Entry().Connected()); test(!cEntry2->Entry().PendingDelete());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(!cEntry3->Entry().Unread()); test(!cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected()); test(!cEntry3->Entry().PendingDelete());

	// set connected
	cEntry->ChangeAttributesL(*selection, KMsvConnectedAttribute, 0);

	// test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(!cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(cEntry1->Entry().Connected()); test(!cEntry1->Entry().PendingDelete());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(!cEntry2->Entry().Unread()); test(!cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected()); test(!cEntry2->Entry().PendingDelete());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(!cEntry3->Entry().Unread()); test(!cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected()); test(!cEntry3->Entry().PendingDelete());

	// set visible clear connected
	cEntry->ChangeAttributesL(*selection, KMsvVisibilityAttribute, KMsvConnectedAttribute);

	// test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(!cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(!cEntry1->Entry().Connected()); test(!cEntry1->Entry().PendingDelete());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(!cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(!cEntry2->Entry().Connected()); test(!cEntry2->Entry().PendingDelete());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(!cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(!cEntry3->Entry().Connected()); test(!cEntry3->Entry().PendingDelete());

	// set connected & unread
	cEntry->ChangeAttributesL(*selection, KMsvUnreadAttribute|KMsvConnectedAttribute, 0);

	// test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(cEntry1->Entry().Connected()); test(!cEntry1->Entry().PendingDelete());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected()); test(!cEntry2->Entry().PendingDelete());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected()); test(!cEntry3->Entry().PendingDelete());

	// set visible & pending delete
	cEntry->ChangeAttributesL(*selection, KMsvVisibilityAttribute | KMsvPendingDeleteAttribute, 0);

	//test (NOT visible because pending delete!)
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(cEntry1->Entry().Connected()); test(cEntry1->Entry().PendingDelete());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(cEntry2->Entry().Unread()); test(!cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected()); test(cEntry2->Entry().PendingDelete());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(cEntry3->Entry().Unread()); test(!cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected()); test(cEntry3->Entry().PendingDelete());

	// set invisible & pending delete
	cEntry->ChangeAttributesL(*selection, KMsvPendingDeleteAttribute, KMsvVisibilityAttribute);

	//test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(cEntry1->Entry().Connected()); test(cEntry1->Entry().PendingDelete());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(cEntry2->Entry().Unread()); test(!cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected()); test(cEntry2->Entry().PendingDelete());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(cEntry3->Entry().Unread()); test(!cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected()); test(cEntry3->Entry().PendingDelete());

	// clear pending delete
	cEntry->ChangeAttributesL(*selection, 0, KMsvPendingDeleteAttribute);

	//test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(cEntry1->Entry().Unread()); test(!cEntry1->Entry().Visible()); test(cEntry1->Entry().Connected()); test(!cEntry1->Entry().PendingDelete());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(cEntry2->Entry().Unread()); test(!cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected()); test(!cEntry2->Entry().PendingDelete());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(cEntry3->Entry().Unread()); test(!cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected()); test(!cEntry3->Entry().PendingDelete());

	// set visible
	cEntry->ChangeAttributesL(*selection, KMsvVisibilityAttribute, 0);

	//test
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(cEntry1->Entry().Unread()); test(cEntry1->Entry().Visible()); test(cEntry1->Entry().Connected()); test(!cEntry1->Entry().PendingDelete());

	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(cEntry2->Entry().Unread()); test(cEntry2->Entry().Visible()); test(cEntry2->Entry().Connected()); test(!cEntry2->Entry().PendingDelete());

	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(cEntry3->Entry().Unread()); test(cEntry3->Entry().Visible()); test(cEntry3->Entry().Connected()); test(!cEntry3->Entry().PendingDelete());

	// entry1
	selection->Reset();
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());

	//Make visible but pending delete
	cEntry->ChangeAttributesL(*selection, KMsvPendingDeleteAttribute | KMsvVisibilityAttribute, 0);

	//Test--not visible because pending delete
	cEntry1->SetEntryL(folder.Id()); cEntry1->SetEntryL(entry1.Id());
	test(!cEntry1->Entry().Visible()); test(cEntry1->Entry().PendingDelete());
	cEntry2->SetEntryL(folder.Id()); cEntry2->SetEntryL(entry2.Id());
	test(!cEntry2->Entry().Visible()); test(cEntry2->Entry().PendingDelete());
	cEntry3->SetEntryL(folder.Id()); cEntry3->SetEntryL(entry3.Id());
	test(!cEntry3->Entry().Visible()); test(cEntry3->Entry().PendingDelete());

	//Close down the message server...
	CleanupStack::PopAndDestroy(7,ob); // selection, cEntry3, cEntry2, cEntry1, cEntry, session, ob

	ob = 0;
	session = 0;
	cEntry = 0;
	selection = 0;

	//Now wait for the message server to close...
	CMsgsTestUtils::WaitForServerClose();

	ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	cEntry->SetEntryL(folder.Id());

	selection=new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// change nothing
	selection->AppendL(entry1.Id());
	selection->AppendL(entry2.Id());
	selection->AppendL(entry3.Id());

	//Test they ARE visible and NOT pending delete
	cEntry->SetEntryL(folder.Id()); cEntry->SetEntryL(entry1.Id());
	test(cEntry->Entry().Visible()); test(!cEntry->Entry().PendingDelete());

	cEntry->SetEntryL(folder.Id()); cEntry->SetEntryL(entry2.Id());
	test(cEntry->Entry().Visible()); test(!cEntry->Entry().PendingDelete());

	cEntry->SetEntryL(folder.Id()); cEntry->SetEntryL(entry3.Id());
	test(cEntry->Entry().Visible()); test(!cEntry->Entry().PendingDelete());

	CleanupStack::PopAndDestroy(4);	//selection, cEntry, session, ob
	}


void doTestsL()
	{
	CreateMtmFilesL();

	__UHEAP_MARK;
	CMsgsTestUtils::Start(_L("Syncronous entry creation"));
	TestCreateSyncL();
	TestCreateSyncWithHeapFailL();
	TestCreateSyncWithFileFailL();
	CMsgsTestUtils::Complete();
	__UHEAP_MARKEND;

	__UHEAP_MARK;
	CMsgsTestUtils::Next(_L("Syncronous entry move"));
	TestMoveSyncL();
	TestMoveSyncWithHeapFailL();
	CMsgsTestUtils::Complete();
	__UHEAP_MARKEND;

	__UHEAP_MARK;
	CMsgsTestUtils::Next(_L("Syncronous entry copy (to another folder)"));
	TestCopySyncL();
	TestCopySyncWithClientHeapFailL();
	TestCopySyncWithServerHeapFailL();
	CMsgsTestUtils::Complete();
	__UHEAP_MARKEND;

	CMsgsTestUtils::Next(_L("Syncronous entry copy (to same folder)"));
	__UHEAP_MARK;
	TestCopySameSyncL();
	__UHEAP_MARKEND;
	__UHEAP_MARK;
	TestCopySameSyncWithClientHeapFailL();
	__UHEAP_MARKEND;
	__UHEAP_MARK;
	TestCopySameSyncWithServerHeapFailL();
	__UHEAP_MARKEND;
	CMsgsTestUtils::Complete();
	
	__UHEAP_MARK;
	CMsgsTestUtils::Next(_L("Syncronous entry change"));
	TestChangeSyncL();
	TestChangeSyncWithHeapFailL();
	TestChangeSyncWithFileFailL();
	CMsgsTestUtils::Complete();
	__UHEAP_MARKEND;

	__UHEAP_MARK;
	CMsgsTestUtils::Next(_L("Syncronous entry deletion"));
	TestDeleteSyncL();
	TestDeleteSyncWithHeapFailL();
	TestDeleteSyncWithFileFailL();
	CMsgsTestUtils::Complete();
	__UHEAP_MARKEND;

	__UHEAP_MARK;
	CMsgsTestUtils::Next(_L("Syncronous Transfer command"));
	TestTransferCommandSyncL();
	CMsgsTestUtils::Complete();
	__UHEAP_MARKEND;

	__UHEAP_MARK;
	CMsgsTestUtils::Next(_L("Syncronous change attributes"));
	TestChangeAttributesL();
	TestChangeAttributesWithHeapFailL();
	TestChangeAttributesWithFileFailL();
	CMsgsTestUtils::Complete();
	__UHEAP_MARKEND;

	__UHEAP_MARK;
	CMsgsTestUtils::Next(_L("PC Syncronisation"));
	TestPcSyncL();
	TestPcSyncWithHeapFailL();
	TestPcSyncWithFileFailL();
	CMsgsTestUtils::Complete();
	__UHEAP_MARKEND;

	__UHEAP_MARK;
	CMsgsTestUtils::Next(_L("Syncronous change attributes pending delete"));
	TestChangePendingDeleteAttributesL();
	CMsgsTestUtils::Complete();
	__UHEAP_MARKEND;
	}

#pragma warning( default : 4702 ) 
#pragma warning( default : 4238 ) 

