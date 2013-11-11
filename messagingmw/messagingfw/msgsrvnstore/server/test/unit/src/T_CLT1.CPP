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

#if !defined(_DEBUG)
#pragma warning( disable : 4702 ) //  unreachable code
#endif
#pragma warning( disable : 4238 ) //nonstandard extension used : class rvalue used as lvalue

RTest	test(_L("MSV Client Testrig 1"));
LOCAL_D CMsvEntrySelection* theRemovedEntries;


// test classes //

class CSessionObserver : public CBase, public MMsvSessionObserver
	{
public:
	CSessionObserver();
	~CSessionObserver();
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	void Start() {iActive=ETrue;};
protected:
	void DoHandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
public:
	TMsvSessionEvent iType;
	CMsvEntrySelection iSelection;
	TMsvId iParameter;
	TMsvId iParameter2;
	TBool iActive;
	};

CSessionObserver::CSessionObserver() 
	{
	}

CSessionObserver::~CSessionObserver() 
	{
	}

void CSessionObserver::DoHandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
	{
	CMsvEntrySelection* selection=NULL;
	iSelection.Reset();
	iParameter=0;
	iType = aEvent;
	switch (iType)
		{
		case EMsvEntriesCreated:
			selection= (CMsvEntrySelection*)aArg1;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			iParameter = UnmaskTMsvId(*(TMsvId*)aArg2);
#else
			iParameter = *(TMsvId*)aArg2;
#endif
			break;
		case EMsvEntriesChanged:
			selection= (CMsvEntrySelection*)aArg1;
			break;
		case EMsvEntriesDeleted:
			selection= (CMsvEntrySelection*)aArg1;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			iParameter = UnmaskTMsvId(*(TMsvId*)aArg2);
#else
			iParameter = *(TMsvId*)aArg2;
#endif
			break;
		case EMsvEntriesMoved:
			selection = (CMsvEntrySelection*)aArg1;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			iParameter = UnmaskTMsvId(*(TMsvId*)aArg2);
			iParameter2 = UnmaskTMsvId(*(TMsvId*)aArg3);
#else
			iParameter = *(TMsvId*)aArg2;
			iParameter2 = *(TMsvId*)aArg3;
#endif
			break;
		default:
			break;
		}
	if (selection)
		{
		for (TInt count=0; count<selection->Count(); count++)
			iSelection.AppendL(selection->At(count));
		}
	}


void CSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
	{
	DoHandleSessionEventL(aEvent,aArg1,aArg2,aArg3);
	if (iActive)
		CActiveScheduler::Stop();
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


	RFs file;
	file.Connect();
	CleanupClosePushL(file);
	TPtrC driveName = TDriveUnit(MessageServer::CurrentDriveL(file)).Name();
    CleanupStack::PopAndDestroy();
	file.Close();
   	filename = driveName;
	filename.Append(KMsvDefaultFolder2);
	MsvUtils::ConstructEntryName(KMsvLocalServiceIndexEntryId, KMsvLocalServiceIndexEntryId, filename, MsvUtils::EPath);
	REPORT_IF_ERROR(theUtils->FileSession().GetDir(filename, KEntryAttMaskSupported, ESortNone, dir));
	
		test(dir->Count()==0);
	delete dir;

	// everything under the root
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	selection = cEntry1->ChildrenL();
	count=selection->Count();
	while (count--)
		{
		TMsvId id = selection->At(count);
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		id = UnmaskTMsvId(id);
#endif
		if (id==KMsvLocalServiceIndexEntryId)
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

#if (!defined SYMBIAN_MESSAGESTORE_USING_SQLDB)
	// check corrupt index flag was set when index file was corrupted
	TBool corrupt=session->GetAndClearIndexCorruptFlagL();
	test(corrupt!=EFalse);
#endif

	// test
	cEntry1->SetEntryL(KMsvLocalServiceIndexEntryId);
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	selection = cEntry1->ChildrenL();
	test(selection->Count()==1);
	delete selection;

	filename = driveName;
	filename.Append(KMsvDefaultFolder2);
	REPORT_IF_ERROR(theUtils->FileSession().GetDir(filename, KEntryAttMaskSupported, ESortNone, dir));
		test(dir->Count()==1); //no Index file under Mail2 folder after PREQ1189.
	delete dir;


	CleanupStack::PopAndDestroy(3); 
	}




LOCAL_C void TestWaitL()
	{
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(0x00B00000); // has to be longer than KMsvDelayTime
	CActiveScheduler::Start();
	delete timer;
	}


LOCAL_C void CorruptIndexFileL(TInt aMode)
	{
	switch (aMode)
		{
		case 0: // uicode/narrow conflict
			{
			theUtils->FileSession().Delete(KMsvDefaultIndexFile2);
			CPermanentFileStore* indexStore = NULL;
			TRAPD (bogus, indexStore = CPermanentFileStore::CreateL(theUtils->FileSession(), KMsvDefaultIndexFile2, EFileShareExclusive|EFileWrite));
			indexStore->SetTypeL(TUidType(KPermanentFileStoreLayoutUid, KNullUid)); // 2nd uid should be KUidMsvIndexFile
			indexStore->CommitL();
			delete indexStore;
			break;
			}
		default:
			test(EFalse);
		}
	}


LOCAL_C void CloseServerL()
	{
	CDummyObserver ob;
	CMsvSession* session	= CMsvSession::OpenSyncL(ob);
	session->CloseMessageServer();
	delete session;
	User::After(1000000);
	}


LOCAL_C void TestRebuildingAsyncL()
	{
	CSessionObserver* ob = new(ELeave) CSessionObserver;
	CMsvSession* session;

	// close the server
	CloseServerL();

	// create the index
	CMsgsTestUtils::CleanMessageFolderL();
	session = CMsvSession::OpenAsyncL(*ob);
	ob->Start();
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	delete session;

	// close the server
	CloseServerL();

	// corrupt it
	CorruptIndexFileL(0);

	// start the server again
	session = CMsvSession::OpenAsyncL(*ob);
	ob->Start();
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	delete session;

	delete ob;
	}


LOCAL_C void TestRebuildingSyncL()
	{
	CSessionObserver* ob = new(ELeave) CSessionObserver;
	CMsvSession* session;

	// close the server
	CloseServerL();

	// create the index
	CMsgsTestUtils::CleanMessageFolderL();
	session = CMsvSession::OpenSyncL(*ob);
	delete session;

	// close the server
	CloseServerL();

	// corrupt it
	CorruptIndexFileL(0);

	// start the server again
	session = CMsvSession::OpenSyncL(*ob);
	delete session;

	delete ob;
	}


LOCAL_C void doTestsL()
	{
	theRemovedEntries = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(theRemovedEntries);


	CMsgsTestUtils::Start(_L("Index rebuilding"));
#if (!defined SYMBIAN_MESSAGESTORE_USING_SQLDB)
	TestRebuildingAsyncL();
	TestRebuildingSyncL();
#endif
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Removing all entries"));
	test.Printf(_L("...and checking for orphan files\n"));
	TestWaitL();
	TestRemovingAllFilesL();
	CMsgsTestUtils::Complete();

	
	CleanupStack::PopAndDestroy(); // theRemovedEntries
	}

#pragma warning( default : 4702 ) 
#pragma warning( default : 4238 ) 

