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

RTest test(_L("MSV Client Heap/File Failure Testrig"));
CMsvEntrySelection* theRemovedEntries;

_LIT(KShortDescription, "Description");
_LIT(KShortDetails, "Details");
_LIT(KLongDescription, "A very long description which goes over thirty two characters");
_LIT(KLongDetails, "A very long details string containing rubbish and junk");

const TUid KMsvTestUid1 = {0x102001D6};

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
	// Defect: EDN733196
	if (aEvent == MMsvSessionObserver::EMsvGeneralError /*&& testScheduler->Level() > 0*/)
		{
		CActiveScheduler *scheduler= CActiveScheduler::Current();
		scheduler->Halt(KErrNone);
		}

	iEvent = aEvent;
	}


void CSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
	{
	DoHandleSessionEvent(aEvent,aArg1,aArg2,aArg3);
	}

//

LOCAL_C void CreateEntryL(TMsvEntry& aNewEntry, CMsvEntry& aClientEntry, TInt aError, CEntryObserver& aObserver)
	{
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);
	active->StartL();
	aObserver.Start();
	CMsvOperation*  opert = aClientEntry.CreateL(aNewEntry, active->iStatus);
	CActiveScheduler::Start(); // operation complete
	CActiveScheduler::Start(); // cEntry changed
	test(opert->iStatus.Int()==KErrNone);
	test(opert->Mtm()==KUidMsvLocalServiceMtm);
	test(opert->Service()==KMsvLocalServiceIndexEntryId);
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);	
	package.Set(opert->ProgressL());
	test(package().iType==TMsvLocalOperationProgress::ELocalNew);
	test(package().iTotalNumberOfEntries==1);
	test(package().iError==aError);
	if (aError)
		{
		test(package().iNumberCompleted==0);
		test(package().iNumberFailed==1);
		}
	else
		{
		test(package().iNumberCompleted==1);
		test(package().iNumberFailed==0);
		TTestMsvEntry* ptr = (TTestMsvEntry*)&aNewEntry; 
		ptr->SetId(package().iId);
		ptr->SetParent(aClientEntry.EntryId());
		if (aNewEntry.iType==KUidMsvServiceEntry)
			aNewEntry.iServiceId = aNewEntry.Id();
		}
	test(package().iNumberRemaining==0);
	delete opert; opert=NULL;
	CleanupStack::PopAndDestroy(); // active
	}

//
// Heap/File failure testing functions
//

LOCAL_C void CreateEntryL(TMsvEntry& aNewEntry, CMsvEntry& aClientEntry)
	{
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->Start();
	CMsvOperation*  opert = aClientEntry.CreateL(aNewEntry, wait->iStatus);
	CActiveScheduler::Start();
	test(opert->iStatus.Int()==KErrNone);
	
	TPckgBuf<TMsvLocalOperationProgress> package;	
	package.Copy(opert->ProgressL());
	*(TMsvId*)&aNewEntry = package().iId;

	delete opert; opert=NULL;
	CleanupStack::PopAndDestroy(); // wait
	}


LOCAL_C void TestEntryAccessWithHeapFailL()
	{
	CDummyObserver ob;

	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
		
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	CreateEntryL(entry, *cEntry);
	CleanupStack::PopAndDestroy(); // cEntry
	cEntry = NULL;

	TBool finished = EFalse;
#if defined(_DEBUG)
	TInt failCount;
	failCount=0;
#endif

	TInt err = KErrNone;

	while(!finished)
		{
			__UHEAP_FAILNEXT(failCount++);

			TRAP(err, cEntry = session->GetEntryL(entry.Id()));
			
			__UHEAP_RESET;

			if (err == KErrNone)
				{
				finished = ETrue;

				test(cEntry->EntryId() == entry.Id());
				delete cEntry; cEntry = NULL;
				}
			else
				{
				test(err == KErrNoMemory);
				test(cEntry == NULL);
				}
		}

	CleanupStack::PopAndDestroy(); // session
	}


LOCAL_C void TestEntryCreationWithHeapFailL()
	{
	CDummyObserver ob;

	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = NULL; 

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		TRAP(err, cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering()));

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			test(cEntry->EntryId() == KMsvGlobalInBoxIndexEntryId);
			delete cEntry; cEntry = NULL;
			}
		else
			{
			test(err == KErrNoMemory);
			test(cEntry == NULL);
			}
		}

	CleanupStack::PopAndDestroy(); // session
	}


LOCAL_C void TestReadStoreWithHeapFailL()
	{
	RMsvReadStream readStream;
	RMsvWriteStream writeStream;

	CDummyObserver ob;

	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	CreateEntryL(entry, *cEntry);
	cEntry->SetEntryL(entry.Id());

	// create store for writing
	CMsvStore* store = cEntry->EditStoreL();
	CleanupStack::PushL(store);

	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	CleanupStack::PopAndDestroy(); // store
	store = NULL;

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		TRAP(err, store = cEntry->ReadStoreL());

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			CleanupStack::PushL(store);

			readStream.OpenL(*store, KMsvTestUid1);
			TInt value = readStream.ReadInt32L();
			test(value==4321);
			readStream.Close();
			
			CleanupStack::PopAndDestroy(); // store

			}
		else
			{
			test(err == KErrNoMemory);
			test(store == NULL);
			}
		}

	CleanupStack::PopAndDestroy(2); // session, cEntry
	}


LOCAL_C void TestReadStoreWithFileFailL()
	{
	RMsvReadStream readStream;
	RMsvWriteStream writeStream;

	CDummyObserver ob;

	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	CreateEntryL(entry, *cEntry);
	cEntry->SetEntryL(entry.Id());

	// create store for writing
	CMsvStore* store = cEntry->EditStoreL();
	CleanupStack::PushL(store);

	writeStream.AssignL(*store, KMsvTestUid1);
	writeStream.WriteInt32L(4321);
	writeStream.CommitL();
	writeStream.Close();
	store->CommitL();
	CleanupStack::PopAndDestroy(); // store
	store = NULL;

	TBool finished = EFalse;
	TInt failCount = 0;
	TInt err = KErrNone;

	while(!finished)
		{
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		TRAP(err, store = cEntry->ReadStoreL());

		theUtils->FileSession().SetErrorCondition(KErrGeneral, 10000);

		if (err == KErrNone)
			{
			finished = ETrue;

			CleanupStack::PushL(store);

			readStream.OpenL(*store, KMsvTestUid1);
			TInt value = readStream.ReadInt32L();
			test(value==4321);
			readStream.Close();
			
			CleanupStack::PopAndDestroy(); // store

			}
		else
			{
			test(err == KErrGeneral);
			test(store == NULL);
			}
		}

	CleanupStack::PopAndDestroy(2); // session, cEntry
	}


LOCAL_C void TestWriteStoreWithHeapFailL()
	{
	RMsvReadStream readStream;
	RMsvWriteStream writeStream;

	CDummyObserver ob;

	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	CreateEntryL(entry, *cEntry);
	cEntry->SetEntryL(entry.Id());

	// create store for writing
	CMsvStore* store = NULL;

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		TRAP(err, store = cEntry->EditStoreL());

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

			store = cEntry->ReadStoreL();
			CleanupStack::PushL(store);

			readStream.OpenL(*store, KMsvTestUid1);
			TInt value = readStream.ReadInt32L();
			test(value==4321);
			readStream.Close();
			
			CleanupStack::PopAndDestroy(); // store
			}
		else
			{
			test(err == KErrNoMemory);
			test(store == NULL);
			}
		}

	CleanupStack::PopAndDestroy(2); // session, cEntry
	}


LOCAL_C void TestWriteStoreWithFileFailL()
	{
	RMsvReadStream readStream;
	RMsvWriteStream writeStream;

	CDummyObserver ob;

	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	CreateEntryL(entry, *cEntry);
	cEntry->SetEntryL(entry.Id());

	// create store for writing
	CMsvStore* store = NULL;

	TBool finished = EFalse;
	TInt failCount = 0;
	TInt err = KErrNone;

	while(!finished)
		{
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		TRAP(err, store = cEntry->EditStoreL());

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

			store = cEntry->ReadStoreL();
			CleanupStack::PushL(store);

			readStream.OpenL(*store, KMsvTestUid1);
			TInt value = readStream.ReadInt32L();
			test(value==4321);
			readStream.Close();
			
			CleanupStack::PopAndDestroy(); // store
			}
		else
			{
			test(err == KErrGeneral);
			test(store == NULL);
			}
		}

	CleanupStack::PopAndDestroy(2); // session, cEntry
	}


LOCAL_C void TestChildSelectWithHeapFailL()
	{
	CDummyObserver ob;

	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(folder, *cEntry);
	cEntry->SetEntryL(folder.Id());

	// add entries
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(entry1, *cEntry);

	TMsvEntry entry2;
	entry2.iDescription.Set(KLongDescription);
	entry2.iDetails.Set(KLongDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(entry2, *cEntry);

	TMsvEntry entry3;
	entry3.iDescription.Set(KLongDescription);
	entry3.iDetails.Set(KShortDetails);
	entry3.iType = KUidMsvMessageEntry;
	entry3.iMtm = KUidMsvLocalServiceMtm;
	entry3.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(entry3, *cEntry);

	CMsvEntrySelection* selection = NULL;
	cEntry->SetEntryL(folder.Id());

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		TRAP(err, selection = cEntry->ChildrenL());

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			test(selection->Count()==3);
			delete selection;
			}
		else
			{
			test(err == KErrNoMemory);
			test(selection == NULL);
			}
		}

	CleanupStack::PopAndDestroy(2); // cEntry, session
	}


LOCAL_C void TestChildEntryWithHeapFailL()
	{
	CDummyObserver ob;

	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(folder, *cEntry);
	cEntry->SetEntryL(folder.Id());

	// add entries
	TMsvEntry entry1;
	entry1.iDescription.Set(KShortDescription);
	entry1.iDetails.Set(KShortDetails);
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(entry1, *cEntry);

	cEntry->SetEntryL(entry1.Id());	

	TMsvEntry entry2;
	entry2.iDescription.Set(KShortDescription);
	entry2.iDetails.Set(KLongDetails);
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(entry2, *cEntry);

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	CMsvEntry* child = NULL;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		TRAP(err, child = cEntry->ChildEntryL(entry2.Id()));

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			finished = ETrue;

			test(child->EntryId() == entry2.Id());
			delete child;
			}
		else
			{
			test(err == KErrNoMemory);
			test(child == NULL);
			}
		}

	CleanupStack::PopAndDestroy(2); // cEntry, session
	}


LOCAL_C void TestAsyncChangeWithHeapFailL()
	{
	CSessionObserver ob;

	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);
	cEntry->AddObserverL(*entryOb);
	
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert = NULL;
	
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);	

	// add an entry - short description
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(entry, *cEntry, KErrNone, *entryOb);
	cEntry->SetEntryL(entry.Id());

	// alter entry
	entry.iDescription.Set(KLongDescription);
	entry.iDetails.Set(KLongDetails);
	
	// change the sever copy

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		SchedulerError = KErrNone;
		SchedulerActive = ETrue;

		__UHEAP_FAILNEXT(failCount++);

		TRAP(err, opert = cEntry->ChangeL(entry, active->iStatus));
		
		if (err == KErrNone)
			{
			entryOb->iEvent = (MMsvEntryObserver::TMsvEntryEvent)-1;
			ob.iEvent = (MMsvSessionObserver::TMsvSessionEvent)-1;

			active->StartL();
			entryOb->Start();

			CActiveScheduler::Start(); // operation complete

			CActiveScheduler::Start(); // observer reported
			}

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			// always correct
			test(opert->iStatus.Int()==KErrNone);
			test(opert->Mtm()==KUidMsvLocalServiceMtm);
			test(opert->Service()==KMsvLocalServiceIndexEntryId);
			package.Set(opert->ProgressL());
			test(package().iType==TMsvLocalOperationProgress::ELocalChanged);
			test(package().iTotalNumberOfEntries==1);
			test(package().iNumberCompleted==1);
			test(package().iNumberFailed==0);
			test(package().iNumberRemaining==0);

			if (SchedulerError == KErrNone && entryOb->iEvent==MMsvEntryObserver::EMsvEntryChanged)
				{
				finished=ETrue;
				test(cEntry->Entry()==entry);
				SchedulerActive = EFalse;
				}
			else
				{
				test(SchedulerError == KErrNoMemory ||
					entryOb->iEvent==MMsvEntryObserver::EMsvContextInvalid ||
					ob.iEvent == MMsvSessionObserver::EMsvGeneralError); // Defect: EDN733196

				cEntry->SetEntryL(entry.Id());
				}

			delete opert; opert=NULL;
			}
		else
			test(err == KErrNoMemory);

		}

	CleanupStack::PopAndDestroy(4); // entryOb, active, cEntry, session
	}


LOCAL_C void TestAsyncCreationWithHeapFailL()
	{
	CSessionObserver ob;

	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);
	cEntry->AddObserverL(*entryOb);
	
	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert = NULL;
	
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);	

	// add an entry - short description
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	TInt childCount = cEntry->Count();

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		SchedulerError = KErrNone;
		SchedulerActive = ETrue;

		__UHEAP_FAILNEXT(failCount++);

		TRAP(err, opert = cEntry->CreateL(entry, active->iStatus));

		if (err == KErrNone)
			{
			entryOb->iEvent = (MMsvEntryObserver::TMsvEntryEvent)-1;
			ob.iEvent = (MMsvSessionObserver::TMsvSessionEvent)-1;

			active->StartL();
			entryOb->Start();
			CActiveScheduler::Start(); // operation complete
			CActiveScheduler::Start(); // observer reported
			}

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			// always correct
			test(opert->iStatus.Int()==KErrNone);
			test(opert->Mtm()==KUidMsvLocalServiceMtm);
			test(opert->Service()==KMsvLocalServiceIndexEntryId);
			package.Set(opert->ProgressL());
			test(package().iType==TMsvLocalOperationProgress::ELocalNew);
			test(package().iTotalNumberOfEntries==1);

			cEntry->SetEntryL(KMsvRootIndexEntryId);
			cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);

			childCount++;

			if (SchedulerError == KErrNone && entryOb->iEvent==MMsvEntryObserver::EMsvNewChildren)
				{
				finished = ETrue;				
				test(cEntry->Count() == childCount);
				SchedulerActive = EFalse;
				}
			else
				{
				test(	SchedulerError == KErrNoMemory ||
						entryOb->iEvent == MMsvEntryObserver::EMsvContextInvalid ||
						entryOb->iEvent == MMsvEntryObserver::EMsvChildrenMissing ||
						ob.iEvent == MMsvSessionObserver::EMsvGeneralError); // Defect: EDN733196				
				}

			delete opert; opert=NULL;
			}
		else
			test(err == KErrNoMemory);

		}

	CleanupStack::PopAndDestroy(4); // session, cEntry, entryOb, active
	}


LOCAL_C void TestAsyncDeleteWithHeapFailL()
	{
	CSessionObserver ob;
	
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CEntryObserver* entryOb=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb);;
	cEntry->AddObserverL(*entryOb);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert = NULL;

	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);	

	// Get count of children for later
	TInt children = cEntry->Count();

	// add entries
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(entry, *cEntry, KErrNone, *entryOb);

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		SchedulerError = KErrNone;
		SchedulerActive = ETrue;

		__UHEAP_FAILNEXT(failCount++);

		// delete the entries
		TRAP(err, opert = cEntry->DeleteL(entry.Id(), active->iStatus));

		if (err == KErrNone)
			{

			entryOb->iEvent = (MMsvEntryObserver::TMsvEntryEvent)-1;
			ob.iEvent = (MMsvSessionObserver::TMsvSessionEvent)-1;

			active->StartL();
			entryOb->Start();
			CActiveScheduler::Start(); // Operation complete
			CActiveScheduler::Start(); // Observer reported
			}

		__UHEAP_RESET;

		if (err == KErrNone)
			{

			test(opert->iStatus.Int()==KErrNone);
			test(opert->Mtm()==KUidMsvLocalServiceMtm);
			test(opert->Service()==KMsvLocalServiceIndexEntryId);
			package.Set(opert->ProgressL());
			test(package().iType==TMsvLocalOperationProgress::ELocalDelete);
			test(package().iTotalNumberOfEntries==1);
			test(package().iNumberCompleted==1);
			test(package().iNumberFailed==0);
			test(package().iNumberRemaining==0);
		
			if (SchedulerError == KErrNone && entryOb->iEvent == MMsvEntryObserver::EMsvDeletedChildren)
				{
				finished = ETrue;
				test(cEntry->Count() == children);
				SchedulerActive = EFalse;
				}
			else
				{
				test(SchedulerError == KErrNoMemory || 
					ob.iEvent == MMsvSessionObserver::EMsvGeneralError);
				}
		
			cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);

			delete opert; opert=NULL;
			}
		else
		if (err == KErrNotFound)
			{
			// The deletion was performed
			// Note: no testing of allocation in CMsvEntry::DeletedChildren
			// This should not cause any problems
			finished = ETrue;
			}
		else
			{
			test(err == KErrNoMemory);
			}
		}

	CleanupStack::PopAndDestroy(4); // session, cEntry, entryOb, active
	}


LOCAL_C void TestAsyncCopyWithHeapFailL()
	{
	CSessionObserver ob;

	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvEntry* cSrc = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cSrc);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);
	cEntry->AddObserverL(*entryOb);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert = NULL;

	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(folder1, *cEntry, KErrNone, *entryOb);

	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(folder2, *cEntry, KErrNone, *entryOb);

	cEntry->SetEntryL(folder1.Id());

	// add entries
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(entry, *cEntry, KErrNone, *entryOb);

	cSrc->SetEntryL(folder1.Id());

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	TInt expectedNumber = 0;
	while(!finished)
		{
		SchedulerError = KErrNone;
		SchedulerActive = ETrue;

		cEntry->SetEntryL(folder2.Id());

		__UHEAP_FAILNEXT(failCount++);

		TRAP(err, opert = cSrc->CopyL(entry.Id(), folder2.Id(), active->iStatus));

		if (err == KErrNone)
			{
			expectedNumber++;

			entryOb->iEvent = (MMsvEntryObserver::TMsvEntryEvent)-1;
			ob.iEvent = (MMsvSessionObserver::TMsvSessionEvent)-1;

			active->StartL();
			entryOb->Start();
			CActiveScheduler::Start();
			CActiveScheduler::Start();
			}

		__UHEAP_RESET;

		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);

		if (err == KErrNone)
			{
			test(opert->iStatus.Int()==KErrNone);
			package.Set(opert->ProgressL());
			test(package().iType==TMsvLocalOperationProgress::ELocalCopy);
			test(package().iTotalNumberOfEntries==1);
			test(package().iNumberCompleted==1);
			test(package().iNumberFailed==0);
			test(package().iNumberRemaining==0);

			if (SchedulerError == KErrNone && entryOb->iEvent==MMsvEntryObserver::EMsvNewChildren)
				{
				finished = ETrue;
			
				cEntry->SetEntryL(folder1.Id()); 	//this is not a leavescan error
				test(cEntry->Count() == 1);	
				cEntry->SetEntryL(folder2.Id());	//this is not a leavescan error
				test(cEntry->Count() == expectedNumber);

				SchedulerActive = EFalse;
				}
			else
				{
				test(SchedulerError == KErrNoMemory ||
					ob.iEvent == MMsvSessionObserver::EMsvGeneralError ||
					entryOb->iEvent==MMsvEntryObserver::EMsvChildrenMissing);
	
				cEntry->SetEntryL(folder1.Id());
				test(cEntry->Count() == 1);
				cEntry->SetEntryL(folder2.Id());
				test(cEntry->Count() == expectedNumber);
				}

			delete opert; opert = NULL;
			}
		else
			{
			test(err == KErrNoMemory);
			
			cEntry->SetEntryL(folder2.Id());
			test(cEntry->Count() == 0);
			}
		}

	CleanupStack::PopAndDestroy(5); // session, cEntry, cSrc, entryOb, active
	}


LOCAL_C void TestAsyncCopyToChildWithHeapFailL()
	{
	CSessionObserver ob;

	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvEntry* cSrc = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cSrc);

	CEntryObserver* entryOb=new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);
	cEntry->AddObserverL(*entryOb);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert = NULL;

	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(folder1, *cEntry, KErrNone, *entryOb);

	cEntry->SetEntryL(folder1.Id());

	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(folder2, *cEntry, KErrNone, *entryOb);
	
	// add entries
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(entry, *cEntry, KErrNone, *entryOb);

	cEntry->SetEntryL(folder1.Id());
	test(cEntry->Entry().Owner());
	cEntry->SetEntryL(folder2.Id());
	test(cEntry->Entry().Owner() == EFalse);

	cSrc->SetEntryL(folder1.Id());

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	TInt expectedNumber = 0;
	while(!finished)
		{
		SchedulerError = KErrNone;
		SchedulerActive = ETrue;

		cEntry->SetEntryL(folder2.Id());

		__UHEAP_FAILNEXT(failCount++);

		TRAP(err, opert = cSrc->CopyL(entry.Id(), folder2.Id(), active->iStatus));

		if (err == KErrNone)
			{
			expectedNumber++;

			entryOb->iEvent = (MMsvEntryObserver::TMsvEntryEvent)-1;
			ob.iEvent = (MMsvSessionObserver::TMsvSessionEvent)-1;

			active->StartL();
			entryOb->Start();
			CActiveScheduler::Start();
			CActiveScheduler::Start();
			}

		__UHEAP_RESET;

		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);

		if (err == KErrNone)
			{
			test(opert->iStatus.Int()==KErrNone);
			package.Set(opert->ProgressL());
			test(package().iType==TMsvLocalOperationProgress::ELocalCopy);
			test(package().iTotalNumberOfEntries==1);
			test(package().iNumberCompleted==1);
			test(package().iNumberFailed==0);
			test(package().iNumberRemaining==0);

			if (SchedulerError == KErrNone && entryOb->iEvent==MMsvEntryObserver::EMsvNewChildren)
				{
				finished = ETrue;
			
				cEntry->SetEntryL(folder1.Id());	//this is not a leavescan error
				test(cEntry->Count() == 2);
				cEntry->SetEntryL(folder2.Id());	//this is not a leavescan error
				test(cEntry->Count() == expectedNumber);
				test(cEntry->Entry().Owner());

				SchedulerActive = EFalse;
				}
			else
				{
				test(SchedulerError == KErrNoMemory ||
					ob.iEvent == MMsvSessionObserver::EMsvGeneralError);
	
				cEntry->SetEntryL(folder1.Id());
				test(cEntry->Count() == 2);
				cEntry->SetEntryL(folder2.Id());
				test(cEntry->Count() == expectedNumber);
				}

			delete opert; opert = NULL;
			}
		else
			{
			test(err == KErrNoMemory);
			
			cEntry->SetEntryL(folder2.Id());
			test(cEntry->Count() == 0);
			test(cEntry->Entry().Owner() == EFalse);
			}
		}

	CleanupStack::PopAndDestroy(5); // session, cEntry, cSrc, entryOb, active
	}


LOCAL_C void TestAsyncMoveWithHeapFailL()
	{
	CSessionObserver ob;

	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);
	
	CEntryObserver* entryOb = new (ELeave) CEntryObserver; 
	CleanupStack::PushL(entryOb);
	cEntry->AddObserverL(*entryOb);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* opert = NULL;
	
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(folder1, *cEntry, KErrNone, *entryOb);

	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(folder2, *cEntry, KErrNone, *entryOb);

	cEntry->SetEntryL(folder1.Id());

	// add entries
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	CreateEntryL(entry, *cEntry, KErrNone, *entryOb);

	TBool finished = EFalse;
#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	

	TInt err = KErrNone;

	while(!finished)
		{
		SchedulerError = KErrNone;
		SchedulerActive = ETrue;

		cEntry->SetEntryL(folder1.Id());

		__UHEAP_FAILNEXT(failCount++);

		TRAP(err, opert = cEntry->MoveL(entry.Id(), folder2.Id(), active->iStatus));

		if (err == KErrNone)
			{
			entryOb->iEvent = (MMsvEntryObserver::TMsvEntryEvent)-1;
			ob.iEvent = (MMsvSessionObserver::TMsvSessionEvent)-1;

			active->StartL();
			entryOb->Start();
			CActiveScheduler::Start();
			CActiveScheduler::Start();
			}

		__UHEAP_RESET;

		if (err == KErrNone)
			{
			test(opert->iStatus.Int()==KErrNone);
			package.Set(opert->ProgressL());
			test(package().iType==TMsvLocalOperationProgress::ELocalMove);
			test(package().iTotalNumberOfEntries==1);
			test(package().iNumberCompleted==1);
			test(package().iNumberFailed==0);
			test(package().iNumberRemaining==0);
	
			if (SchedulerError == KErrNone && entryOb->iEvent==MMsvEntryObserver::EMsvDeletedChildren)
				{
				finished = ETrue;

				cEntry->SetEntryL(folder2.Id());	//this is not a leavescan error
				test(cEntry->Count() == 1);
				cEntry->SetEntryL(folder1.Id());	//this is not a leavescan error
				test(cEntry->Count() == 0);

				SchedulerActive = EFalse;
				}
			else
				{
				test(SchedulerError == KErrNoMemory ||
					ob.iEvent == MMsvSessionObserver::EMsvGeneralError ||
					entryOb->iEvent == MMsvEntryObserver::EMsvChildrenInvalid);

				delete opert; opert = NULL;

				cEntry->SetEntryL(folder2.Id());
				cEntry->SetEntryL(folder1.Id());

				if (cEntry->Count() == 0)
					{
					cEntry->SetEntryL(folder2.Id());
					entryOb->Start();
					active->StartL();
					opert = cEntry->MoveL(entry.Id(), folder1.Id(), active->iStatus);
					CActiveScheduler::Start(); // operation complete
					CActiveScheduler::Start(); // cEntry updated
					}
				}

			delete opert; opert = NULL;
			}
		else
			{
			test(err == KErrNoMemory);
			
			test(cEntry->Count() == 1);
			cEntry->SetEntryL(folder2.Id());
			test(cEntry->Count() == 0);
			}
		}
	CleanupStack::PopAndDestroy(4); // session, cEntry, entryOb, active
	}


LOCAL_C void doTestsL()
	{
	PanicSchedulerOnError = EFalse;
	theRemovedEntries = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(theRemovedEntries);

	CMsgsTestUtils::Start(_L("Entry access with heap failure"));
	TestEntryAccessWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Entry creation with heap failure"));
	TestEntryCreationWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Reading message store with heap failure"));
	TestReadStoreWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Reading message store with file failure"));
	TestReadStoreWithFileFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Editing message store with heap failure"));
	TestWriteStoreWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Editing message store with file failure"));
	TestWriteStoreWithFileFailL();	
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Child selection with heap failure"));
	TestChildSelectWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Child entry with heap failure"));
	TestChildEntryWithHeapFailL();
	CMsgsTestUtils::Complete();

	// Start of asyncronous testing

	CMsgsTestUtils::Next(_L("Asyncronous entry change with heap failure"));
	TestAsyncChangeWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Asyncronous entry creation with heap failure"));
	TestAsyncCreationWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Asyncronous entry deletion with heap failure"));
	TestAsyncDeleteWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Asyncronous copy with heap failure"));
	TestAsyncCopyWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Asyncronous copy to child with heap failure"));
	TestAsyncCopyToChildWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Asyncronous move with heap failure"));
	TestAsyncMoveWithHeapFailL();
	CMsgsTestUtils::Complete();

	// End of asyncronous testing

	CleanupStack::PopAndDestroy(); // theRemovedEntries	
	}

#pragma warning( default : 4702 ) 
#pragma warning( default : 4238 ) 

