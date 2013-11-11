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
#include "msvutils.h"
#include "Testsetup.h"
#include <e32base.h>

#if !defined(_DEBUG)
#pragma warning( disable : 4702 ) //  unreachable code
#endif
#pragma warning( disable : 4238 ) //nonstandard extension used : class rvalue used as lvalue

RTest test(_L("MSV Client Testrig 0"));
CMsvEntrySelection* theRemovedEntries;

_LIT(KShortDescription, "Description");
_LIT(KShortDetails, "Details");

const TUid KUidMsvHiddenEntryStream = {0x10003C73};



// test classes //

class CMultipleSessionObserver : public CBase, public MMsvSessionObserver
	{
public:
	CMultipleSessionObserver();
	~CMultipleSessionObserver();
	void Start(TMsvSessionEvent aType, TInt aParameter);
	void Start(TMsvSessionEvent aType, TInt aParameter, TInt aParameter2);
	void Finish() {iActive=EFalse;};
	void Wait() {iWait=ETrue;};
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
public:
	TBool iWait;
	TBool iActive;
	TInt iEvents;
	TMsvSessionEvent iType;
	CMsvEntrySelection iSelection;
	TMsvId iParameter;
	TMsvId iParameter2;
	};

CMultipleSessionObserver::CMultipleSessionObserver() 
	{
	}

CMultipleSessionObserver::~CMultipleSessionObserver() 
	{
	}

void CMultipleSessionObserver::Start(TMsvSessionEvent aType, TInt aParameter) 
	{
	iActive=ETrue;
	iEvents=0;
	iType=aType; 
	iSelection.Reset();
	iParameter=aParameter;
	}

void CMultipleSessionObserver::Start(TMsvSessionEvent aType, TInt aParameter, TInt aParameter2) 
	{
	iActive=ETrue;
	iEvents=0;
	iType=aType; 
	iSelection.Reset();
	iParameter=aParameter;
	iParameter2=aParameter2;
	}

void CMultipleSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
	{
	if (iActive)
		{
		CMsvEntrySelection* selection=NULL;
		iEvents++;
		__ASSERT_ALWAYS(iType==aEvent,User::Panic(_L("T_CLIENT"),1));
		switch (iType)
			{
			case EMsvEntriesMoved:
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				__ASSERT_ALWAYS(UnmaskTMsvId(iParameter2)==UnmaskTMsvId(*(TMsvId*)aArg3), User::Panic(_L("T_CLIENT"),2));
#else
				__ASSERT_ALWAYS(iParameter2==*(TMsvId*)aArg3, User::Panic(_L("T_CLIENT"),2));
#endif
			case EMsvEntriesDeleted:
			case EMsvEntriesCreated:
				selection= (CMsvEntrySelection*)aArg1;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				__ASSERT_ALWAYS(UnmaskTMsvId(iParameter)==UnmaskTMsvId(*(TMsvId*)aArg2), User::Panic(_L("T_CLIENT"),2));
#else				
				__ASSERT_ALWAYS(iParameter==*(TMsvId*)aArg2, User::Panic(_L("T_CLIENT"),2));
#endif
				break;
			default:
				User::Panic(_L("T_CLIENT"),3);
				break;
			}
		if (selection)
			{
			for (TInt count=0; count<selection->Count(); count++)
				iSelection.AppendL(selection->At(count));
			}
		if (iWait)
			{
			CActiveScheduler::Stop();
			iWait=EFalse;
			}
		}
	}

// test classes //#

LOCAL_C void CreateEntry(TMsvEntry& aNewEntry, CMsvEntry& aClientEntry, TInt aError, CEntryObserver& aObserver)
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


	


LOCAL_C void TestMultipleNotifsL()
	{
	CMultipleSessionObserver* ob = new(ELeave) CMultipleSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CEntryObserver* entryOb1=new (ELeave) CEntryObserver; CleanupStack::PushL(entryOb1);
	cEntry->AddObserverL(*entryOb1);

	CTestActive* active = new (ELeave) CTestActive;
	CleanupStack::PushL(active);

	CMsvOperation* operation;
	CMsvEntrySelection* selection;

	// create folders to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;

	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;

	TMsvEntry folder3;
	folder3.iDescription.Set(KShortDescription);
	folder3.iDetails.Set(KShortDetails);
	folder3.iType = KUidMsvFolderEntry;
	folder3.iMtm = KUidMsvLocalServiceMtm;
	folder3.iServiceId = KMsvLocalServiceIndexEntryId;

	// create all th entries
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvFolderEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	// create folder 2 & 3
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	CreateEntry(folder2, *cEntry, KErrNone, *entryOb1);
	CreateEntry(folder3, *cEntry, KErrNone, *entryOb1);
	
	TMsvLocalOperationProgress prog;
	
	TInt max = 2*TMsvPackedChangeNotification::KMsvPackedChangeLimit+4;
	for (TInt count=1; count<=max; count++)
		{
		test.Printf(_L("."));
		
		// create another entry under folder 2
		cEntry->SetEntryL(folder2.Id());
		CreateEntry(entry, *cEntry, KErrNone, *entryOb1);

		// create folder1
		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
		CreateEntry(folder1, *cEntry, KErrNone, *entryOb1);

		// test
		cEntry->SetEntryL(folder3.Id());
		selection = cEntry->ChildrenL();
		test(selection->Count()==0);
		delete selection;

		// copy all entries to folder 3
		ob->Start(MMsvSessionObserver::EMsvEntriesCreated, folder3.Id());
		cEntry->SetEntryL(folder2.Id());
		selection = cEntry->ChildrenL();		
		test(selection->Count()==count);
		active->StartL();
		operation = cEntry->CopyL(*selection, folder3.Id(), active->iStatus);
		delete selection;
		CActiveScheduler::Start(); // operation complete
		test(operation->iStatus.Int()==KErrNone);
		test(operation->Mtm()==KUidMsvLocalServiceMtm);
		test(operation->Service()==KMsvLocalServiceIndexEntryId);
		prog = McliUtils::GetLocalProgressL(*operation);
		test(prog.iTotalNumberOfEntries==count);
		test(prog.iNumberCompleted==count);
		test(prog.iNumberFailed==0);
		test(prog.iError==KErrNone);
		delete operation; operation=NULL;
		// test
		cEntry->SetEntryL(folder3.Id());
		selection = cEntry->ChildrenL();
		test(selection->Count()==count);
		delete selection;
		if (ob->iEvents>1)
			test(ob->iSelection.Count()>TMsvPackedChangeNotification::KMsvPackedChangeLimit);
		else if (count>TMsvPackedChangeNotification::KMsvPackedChangeLimit)
			{
			ob->Wait();
			CActiveScheduler::Start();
			}
		// test
		selection = cEntry->ChildrenL();
		test(selection->Count()==count);
		delete selection;
		if (ob->iEvents>2)
			test(ob->iSelection.Count()>2*TMsvPackedChangeNotification::KMsvPackedChangeLimit);
		else if (count>2*TMsvPackedChangeNotification::KMsvPackedChangeLimit)
			{
			ob->Wait();
			CActiveScheduler::Start();
			}
		test(ob->iSelection.Count()==count);
		ob->Finish();

		// move all entries to folder 1
		selection = cEntry->ChildrenL();
		test(selection->Count()==count);
		ob->Start(MMsvSessionObserver::EMsvEntriesMoved, folder1.Id(), folder3.Id());
		active->StartL();
		operation = cEntry->MoveL(*selection, folder1.Id(), active->iStatus);
		delete selection;
		CActiveScheduler::Start(); // operation complete
		test(operation->iStatus.Int()==KErrNone);
		test(operation->Mtm()==KUidMsvLocalServiceMtm);
		test(operation->Service()==KMsvLocalServiceIndexEntryId);
		prog = McliUtils::GetLocalProgressL(*operation);
		test(prog.iTotalNumberOfEntries==count);
		test(prog.iNumberCompleted==count);
		test(prog.iNumberFailed==0);
		test(prog.iError==KErrNone);
		delete operation; operation=NULL;
		cEntry->SetEntryL(folder1.Id());
		selection = cEntry->ChildrenL();
		test(selection->Count()==count);
		delete selection;
		if (ob->iEvents>1)
			test(ob->iSelection.Count()>TMsvPackedChangeNotification::KMsvPackedChangeLimit);
		else if (count>TMsvPackedChangeNotification::KMsvPackedChangeLimit)
			{
			test(ob->iEvents==1);
			ob->Wait();
			CActiveScheduler::Start();
			test(ob->iEvents==2);
			}
		if (ob->iEvents>2)
			test(ob->iSelection.Count()>2*TMsvPackedChangeNotification::KMsvPackedChangeLimit);
		else if (count>2*TMsvPackedChangeNotification::KMsvPackedChangeLimit)
			{
			test(ob->iEvents==2);
			ob->Wait();
			CActiveScheduler::Start();
			test(ob->iEvents==3);
			}
		test(ob->iSelection.Count()==count);
		ob->Finish();
	

		// delete them
		ob->Start(MMsvSessionObserver::EMsvEntriesDeleted, KMsvGlobalInBoxIndexEntryId);
		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
		active->StartL();
		operation = cEntry->DeleteL(folder1.Id(), active->iStatus);
		CActiveScheduler::Start(); // operation complete
		test(operation->iStatus.Int()==KErrNone);
		test(operation->Mtm()==KUidMsvLocalServiceMtm);
		test(operation->Service()==KMsvLocalServiceIndexEntryId);
		prog = McliUtils::GetLocalProgressL(*operation);
		test(prog.iTotalNumberOfEntries==1);
		test(prog.iNumberFailed==0);
		test(prog.iError==KErrNone);
		delete operation; operation=NULL;
		if (count+1>TMsvPackedChangeNotification::KMsvPackedChangeLimit && ob->iEvents==1)
			{
			ob->Wait();
			CActiveScheduler::Start();
			}
		if (count+1>2*TMsvPackedChangeNotification::KMsvPackedChangeLimit && ob->iEvents==2)
			{
			ob->Wait();
			CActiveScheduler::Start();
			}
		test(ob->iSelection.Count()==count+1);
		ob->Finish();
		}

	test.Printf(_L("\n"));
	CleanupStack::PopAndDestroy(5);	
	}


LOCAL_C void TestSessionWithPriorityL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob,CMsvSession::EPriorityStandard);
	
	if (session->Priority() == session->EPriorityStandard)
		{
		CMsgsTestUtils::Next(_L("Dfault priority is EPriorityStandard "));	
		}
	delete session;
	session= NULL;
	
	session = CMsvSession::OpenAsyncL(ob,CMsvSession::EPriorityLow);
	if (session->Priority() == session->EPriorityLow)
		{
		CMsgsTestUtils::Next(_L("Dfault priority is EPriorityLow "));	
		}
	delete session;
	session= NULL;
	
	RFs rfs;
	User::LeaveIfError(rfs.Connect());
	CleanupClosePushL(rfs);
	session = CMsvSession::OpenAsyncL(ob,rfs,CMsvSession::EPriorityHigh);
	if (session->Priority() == session->EPriorityHigh)
		{
		CMsgsTestUtils::Next(_L("Dfault priority is EPriorityHigh "));	
		}
	delete session;
	session= NULL;
	CleanupStack::PopAndDestroy(&rfs);	
	
	}


LOCAL_C void TestRemovingAllFilesL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvLocalServiceIndexEntryId, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry1);

	CMsvEntrySelection* selection;
	CMsvOperation* opert;
	CTestActive* active = new (ELeave) CTestActive; CleanupStack::PushL(active);

	CDir* dir;
	TFileName filename;
	TInt count;


	// delete everything under the outbox
	cEntry1->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	selection = cEntry1->ChildrenL();
	if (selection->Count())
		{
		active->StartL();
		opert = cEntry1->DeleteL(*selection, active->iStatus);
		CActiveScheduler::Start(); // operation complete
		delete opert; 
		}
	delete selection;

	// delete everything under the inbox
	cEntry1->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	selection = cEntry1->ChildrenL();
	if (selection->Count())
		{
		active->StartL();
		opert = cEntry1->DeleteL(*selection, active->iStatus);
		CActiveScheduler::Start(); // operation complete
		delete opert; 
		}
	delete selection;
	
	// delete everything under the draft
	cEntry1->SetEntryL(KMsvDraftEntryId);
	selection = cEntry1->ChildrenL();
	if (selection->Count())
		{
		active->StartL();
		opert = cEntry1->DeleteL(*selection, active->iStatus);
		CActiveScheduler::Start(); // operation complete
		delete opert; 
		}
	delete selection;
	
	// delete everything under the sent
	cEntry1->SetEntryL(KMsvSentEntryId);
	selection = cEntry1->ChildrenL();
	if (selection->Count())
		{
		active->StartL();
		opert = cEntry1->DeleteL(*selection, active->iStatus);
		CActiveScheduler::Start(); // operation complete
		delete opert; 
		}
	delete selection;

	// delete everything under the local service
	cEntry1->SetEntryL(KMsvLocalServiceIndexEntryId);
	selection = cEntry1->ChildrenL();
	count=selection->Count();
	while (count--)
		{
		TMsvId id = selection->At(count);
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		id = UnmaskTMsvId(id);
#endif
		if (id==KMsvGlobalOutBoxIndexEntryId || 
			id==KMsvGlobalInBoxIndexEntryId || 
			id==KMsvDraftEntryId || 
			id==KMsvSentEntryId || 
			id==KMsvDeletedEntryFolderEntryId)
			selection->Delete(count);
		}
	if (selection->Count())
		{
		active->StartL();
		opert = cEntry1->DeleteL(*selection, active->iStatus);
		CActiveScheduler::Start(); // operation complete
		delete opert; 
		}
	delete selection;

	// test
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	cEntry1->SetEntryL(KMsvLocalServiceIndexEntryId);
	selection = cEntry1->ChildrenL();
	test(selection->Count()==5);
	delete selection;
	TestNumberOfFilesInService(KMsvLocalServiceIndexEntryId,0);

	// everything under the root
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	selection = cEntry1->ChildrenL();
	count=selection->Count();
	while (count--)
		{
		TMsvId id = selection->At(count);
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		if (UnmaskTMsvId(id)==KMsvLocalServiceIndexEntryId)
#else
		if (id==KMsvLocalServiceIndexEntryId)
#endif
				selection->Delete(count);
		}
	if (selection->Count())
		{
		active->StartL();
		opert = cEntry1->DeleteL(*selection, active->iStatus);
		CActiveScheduler::Start(); // operation complete
		delete opert; 
		}
	delete selection;

	// test
	cEntry1->SetEntryL(KMsvLocalServiceIndexEntryId);
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	selection = cEntry1->ChildrenL();
	test(selection->Count()==1);
	delete selection;

	RFs file;
	file.Connect();
	CleanupClosePushL(file);
	TPtrC driveName = TDriveUnit(MessageServer::CurrentDriveL(file)).Name();
	CleanupStack::PopAndDestroy(); 
	file.Close();
	filename = driveName;
	filename.Append(KMsvDefaultFolder2);
	REPORT_IF_ERROR(theUtils->FileSession().GetDir(filename, KEntryAttMaskSupported, ESortNone, dir));
	//Index File Will not be created in case of SQL
	test(dir->Count()==1);
	delete dir;

	CleanupStack::PopAndDestroy(3); 
	}


LOCAL_C void TestSessionWithSharedFsL()
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
		
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob, fs);
	delete session;	
	
	session = CMsvSession::OpenAsyncL(ob, fs);
	delete session;	

	session = CMsvSession::OpenAsObserverL(ob, fs);
	delete session;	

	// Check that the file server session has not been closed
	test(fs.Handle() != 0);	
	
	CleanupStack::PopAndDestroy(&fs);	
	test(fs.Handle() == 0);
	}

LOCAL_C void TestEntryStream(CMsvEntry& aEntry, TMsvId aId, TMsvId aService, TBool aAllDescendents)
	{
	// get the store
	aEntry.SetEntryL(aId);
	CMsvStore* store=NULL;
	if(aEntry.HasStoreL()) store = aEntry.ReadStoreL();
		test(store==NULL || store->IsPresentL(KUidMsvHiddenEntryStream)==EFalse);
		delete store;
	// recurse into the children
	if (aAllDescendents)
		{
		CMsvEntrySelection* selection = aEntry.ChildrenL();
		TInt count=selection->Count();
		while (count--)
			TestEntryStream(aEntry, selection->At(count), aService, EFalse);
		delete selection;
		}
	}


LOCAL_C void TestHiddenEntryStreamsL()
	{
	CDummyObserver ob;
	CMsvSession* session = CMsvSession::OpenSyncL(ob);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvRootIndexEntryId, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry1);
	TMsvSelectionOrdering sort(KMsvNoGrouping, EMsvSortByNone, ETrue);
	cEntry1->SetSortTypeL(sort);

	// all services
	CMsvEntrySelection* selection = cEntry1->ChildrenL();
	TInt count=selection->Count();
	while (count--)
		TestEntryStream(*cEntry1, selection->At(count), selection->At(count), EFalse);
	delete selection;
	
	// local entries
	TestEntryStream(*cEntry1, KMsvLocalServiceIndexEntryId, KMsvLocalServiceIndexEntryId, ETrue);

	CleanupStack::PopAndDestroy(2); // cEntry1,session
	}


LOCAL_C void TestWaitL()
	{
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(0x00B00000); // has to be longer than KMsvDelayTime
	CActiveScheduler::Start();
	delete timer;
	}




LOCAL_C void doTestsL()
	{
	theRemovedEntries = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(theRemovedEntries);


	CMsgsTestUtils::Start(_L("Large selection operations - will be slow"));
	TestMultipleNotifsL();
	CMsgsTestUtils::Complete();

	test.Printf(_L("** Waiting 10s for delayed updating **\n"));
	TestWaitL();

	CMsgsTestUtils::Next(_L("Hidden entry stream"));
	TestHiddenEntryStreamsL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Removing all entries"));
	test.Printf(_L("...and checking for orphan files\n"));
	TestRemovingAllFilesL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Testing Session With Shared RFs "));
	TestSessionWithSharedFsL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Testing Session With Priority Change "));
	TestSessionWithPriorityL();
	CMsgsTestUtils::Complete();

	CleanupStack::PopAndDestroy(); // theRemovedEntries
	}


#pragma warning( default : 4702 ) 
#pragma warning( default : 4238 ) 


