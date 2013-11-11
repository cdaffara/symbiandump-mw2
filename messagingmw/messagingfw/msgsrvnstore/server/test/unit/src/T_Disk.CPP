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

#include "Testsetup.h"
#include <cemailaccounts.h>
#include <imapset.h>
#include <iapprefs.h>
#include <centralrepository.h>


RTest test(_L("Media Changing Test Harness"));
TBool RunManual = EFalse;
const TUid KUidMsvImapMtm = { 0x1000102a };

//

class CSessionObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*);
public:
	CMsvSession* iSession;
	TMsvSessionEvent iType;
	TInt iArg1;
	TInt iArg2;
	};

void CSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny*)
	{
	if (aEvent == EMsvServerReady ||
		aEvent == EMsvCorruptedIndexRebuilt ||
		aEvent == EMsvMediaChanged ||
		aEvent == EMsvMediaUnavailable ||
		aEvent == EMsvMediaAvailable ||
		aEvent == EMsvMediaIncorrect ||
		aEvent == EMsvCorruptedIndexRebuilding)
		{
		// See if the index is available
		if (aEvent != EMsvMediaUnavailable && aEvent != EMsvMediaIncorrect &&  aEvent != EMsvCorruptedIndexRebuilding)
			{
			TMsvId service;
			TMsvEntry entry;

			// Check we can retrieve the inbox details from the index
			test(iSession->GetEntry(KMsvGlobalInBoxIndexEntryId, service, entry) == KErrNone);
			}

		iType = aEvent;
		if (aArg1)
			iArg1 = *(TInt*)aArg1;
		if (aArg2)
			iArg2 = *(TInt*)aArg2;
		CActiveScheduler::Stop();
		}
	}

//

LOCAL_C void TestEventNotificationOnChangeDriveL()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CleanupStack::PushL(session);
	
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	TPckgBuf<TMsvIndexLoadProgress> progressPack;
	TMsvIndexLoadProgress progress;

	// Change to drive with no message store
	active->StartL();
	CMsvOperation* operation = NULL;
	operation = session->ChangeDriveL(TInt(RemovableDrive), active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation);

	// Check its possible to identify the progress
	TBuf8<KMsvProgressBufferLength> buf;
	buf.Copy(operation->ProgressL());

	TMsvServerOperationProgress serverProgress;
	TPckg<TMsvServerOperationProgress> package(serverProgress);
	package.Copy(buf.Ptr(), sizeof(TMsvServerOperationProgress));

	test(serverProgress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);
	test(package().iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	// Check progress
    progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	test(progress.iError == KErrNone);
	test(progress.iState == TMsvIndexLoadProgress::EIndexComplete || progress.iState == TMsvIndexLoadProgress::EIndexNotLoaded);
	test(progress.iIndex.iTotal > 0);
	test(progress.iIndex.iCompleted == progressPack().iIndex.iTotal);
	test(progress.iIndex.iRemaining == 0);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	// Check notifications
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==EDriveC);
	test(ob->iArg2==TInt(RemovableDrive));

	// Complete operation
	CActiveScheduler::Start();


	operation = session->CopyStoreL(EDriveC, active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation);


	// Check progress
    progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	test(progress.iError == KErrNone);
	test(progress.iState == TMsvIndexLoadProgress::EIndexNotLoaded);
	test(progress.iIndex.iTotal >= 0);
	test(progress.iIndex.iCompleted >= 0);
	test(progress.iIndex.iRemaining == progress.iIndex.iTotal - progress.iIndex.iCompleted);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvCopyOperation);

	// Remember the total so we can check it later
	TInt total = progress.iIndex.iTotal;

	// Check notifications
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
	// Make sure that iArg1 points to external drive...
	test(ob->iArg1==TInt(RemovableDrive));
	CleanupStack::PopAndDestroy(4);// operation,active,cEntry,session
	// Now switch back to C: which has a message store
	session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CleanupStack::PushL(session);
	
	cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	// Change to drive with message store
	active->StartL();
	// Change back to drive containing message store....
	operation = session->ChangeDriveL(EDriveC, active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaAvailable);

	// Make sure we are pointing to external drive at this moment...
	test(ob->iArg1==TInt(RemovableDrive));
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	
	test(ob->iArg1==TInt(RemovableDrive));
	
	CActiveScheduler::Start();
	progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	test(progress.iError == KErrNone);
	test(progress.iState == TMsvIndexLoadProgress::EIndexComplete || progress.iState == TMsvIndexLoadProgress::EIndexNotLoaded);
	test(progress.iIndex.iTotal > 0);
	test(progress.iIndex.iCompleted == progressPack().iIndex.iTotal);
	test(progress.iIndex.iRemaining == 0);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==TInt(RemovableDrive));
	
	CleanupStack::PopAndDestroy(5); // operation,active,cEntry,session
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);
	CleanupStack::PopAndDestroy(ob);
	}
LOCAL_C void TestMediaRemovedOnStartL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsgsTestUtils::RemoveMailFolderL(EDriveC);

	// Try creating the session with store on E: 
	// C: has no mail folder

	CMsgsTestUtils::SetDriveL(EDriveY);

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
	test(ob->iArg1==EDriveY);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==EDriveY);
	test(ob->iArg2==EDriveC);
	delete session;	
	
	// Check drive changed
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);

	// Try creating the session with store on E: 
	// C: does have a mail folder this time

	CMsgsTestUtils::SetDriveL(EDriveY);
	
	session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
	test(ob->iArg1==EDriveY);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==EDriveY);
	test(ob->iArg2==EDriveC);
	delete session;	
	
	// Check drive changed
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);

	TInt mode;
	for(mode = 0; mode <= 3; mode++)
		{
		// Try creating the session with store on E: 
		// C: has a corrupt a mail folder
		CMsgsTestUtils::CorruptIndexFileL(mode);
		CMsgsTestUtils::SetDriveL(EDriveY);
		
		session = CMsvSession::OpenAsyncL(*ob);
		ob->iSession = session; // Set session in observer
		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvServerReady);
		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
		test(ob->iArg1==EDriveY);
		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
		test(ob->iArg1==EDriveY);
		test(ob->iArg2==EDriveC);
		delete session;	

		// Check drive changed
		CMsgsTestUtils::WaitForServerClose();
		test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);
		}

	CleanupStack::PopAndDestroy(); // ob
	}

LOCAL_C void TestMediaChangedOnStartL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsgsTestUtils::RemoveMailFolderL(EDriveC);

	// Try creating the session with store on TInt(RemovableDrive)
	// C: has no mail folder

	CMsgsTestUtils::SetDriveL(TInt(RemovableDrive), ETrue);

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaIncorrect);
	test(ob->iArg1==TInt(RemovableDrive));
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==TInt(RemovableDrive));
	test(ob->iArg2==EDriveC);
	delete session;	

	// Check drive changed
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);

	// Try creating the session with store on TInt(RemovableDrive) 
	// C: does have a mail folder this time

	CMsgsTestUtils::SetDriveL(TInt(RemovableDrive), ETrue);

	session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaIncorrect);
	test(ob->iArg1==TInt(RemovableDrive));
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==TInt(RemovableDrive));
	test(ob->iArg2==EDriveC);
	delete session;	

	// Check drive changed
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);

	TInt mode;
	for(mode = 0; mode <= 3; mode++)
		{
		// Try creating the session with store on TInt(RemovableDrive)
		// C: has a corrupt a mail folder
		CMsgsTestUtils::CorruptIndexFileL(mode);
		CMsgsTestUtils::SetDriveL(TInt(RemovableDrive), ETrue);

		session = CMsvSession::OpenAsyncL(*ob);
		ob->iSession = session; // Set session in observer
		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvServerReady);
		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvMediaIncorrect);
		test(ob->iArg1==TInt(RemovableDrive));
		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
		test(ob->iArg1==TInt(RemovableDrive));
		test(ob->iArg2==EDriveC);
		delete session;	

		// Check drive changed
		CMsgsTestUtils::WaitForServerClose();
		test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);
		}

	CleanupStack::PopAndDestroy(); // ob
	}

LOCAL_C void RemoveMedia()
	{
	if (RunManual)
		test.Printf(_L("Remove disk in drive now.\n"));
	else
		{
		RMsvServerSession server;
		server.Connect(theUtils->FileSession());

		// This tells the server to pretend that its media is missing next time it gets a disk notification
		server.SetFailure(EDiskFailure, ETrue);


		// Close session
		server.Close();
		}
	}

LOCAL_C void InsertMedia(TBool aWrongId = EFalse)
	{
	if (RunManual)
		{
		if (aWrongId)
			test.Printf(_L("Insert the wrong disk in drive now.\n"));
		else
			test.Printf(_L("Insert the correct disk in drive now.\n"));
		}
	else
		{
		RMsvServerSession server;
		server.Connect(theUtils->FileSession());

		// This tells the server to pretend that its media is missing next time it gets a disk notification
		server.SetFailure(EDiskFailure, EFalse, aWrongId);

		User::After(500000);

		// Close session
		server.Close();
		}
	}

LOCAL_C void TestMediaRemovedWhileRunningL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));
	CMsgsTestUtils::SetDriveL(TInt(RemovableDrive));

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	// Remove the message server media
	RemoveMedia();

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
	test(ob->iArg1==TInt(RemovableDrive));

	// Create a new session - it is told that the media is currently unavailable
	CMsvSession* session2 = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
	test(ob->iArg1==TInt(RemovableDrive));
	delete session2;

	// Reinsert the message server media
	InsertMedia();

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaAvailable);
	test(ob->iArg1==TInt(RemovableDrive));
	delete session;	

	// Check drive still on TInt(RemovableDrive)
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == TInt(RemovableDrive));

	CleanupStack::PopAndDestroy(); // ob
	}

LOCAL_C void TestMediaChangedWhileRunningL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));
	CMsgsTestUtils::SetDriveL(TInt(RemovableDrive));

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	// Remove the message server media
	RemoveMedia();

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
	test(ob->iArg1==TInt(RemovableDrive));

	// Create a new session - it is told that the media is currently unavailable
	CMsvSession* session2 = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
	test(ob->iArg1==TInt(RemovableDrive));
	delete session2;

	// Reinsert the wrong media
	InsertMedia(ETrue);

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaIncorrect);
	test(ob->iArg1==TInt(RemovableDrive));

	// Create a new session - it is told that the media is incorrect
	session2 = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaIncorrect);
	test(ob->iArg1==TInt(RemovableDrive));
	delete session2;

	// Remove the message server media again
	RemoveMedia();

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
	test(ob->iArg1==TInt(RemovableDrive));

	// Reinsert the correct media
	InsertMedia();

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaAvailable);
	test(ob->iArg1==TInt(RemovableDrive));

	delete session;	

	// Check drive still on TInt(RemovableDrive)
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == TInt(RemovableDrive));

	CleanupStack::PopAndDestroy(); // ob
	}

LOCAL_C void TestReadOperationsL(CMsvSession& aSession)
	{
	// Test CMsvSession functions succeed

	CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();
	
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	aSession.GetChildIdsL(KMsvLocalServiceIndexEntryId, *filter, *selection);
	test(selection->Find(KMsvGlobalInBoxIndexEntryId) >= 0);
	test(selection->Find(KMsvGlobalOutBoxIndexEntryId) >= 0);
	test(selection->Find(KMsvDraftEntryId) >= 0);
	test(selection->Find(KMsvSentEntryId) >= 0);

	TInt count = selection->Count();
	while(count--)
		{
		TMsvEntry entry;
		TMsvId service;
		test(aSession.GetEntry(selection->At(count), service, entry) == KErrNone);
		test(service == KMsvLocalServiceIndexEntryId);

		CMsvEntry* cEntry = aSession.GetEntryL(selection->At(count));
		delete cEntry;
		}

	CleanupStack::PopAndDestroy(); // selection

	// Test CMsvEntry functions succeed

	CMsvEntry* cEntry = CMsvEntry::NewL(aSession, KMsvLocalServiceIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);

	count = selection->Count();
	while(count--)
		{
		TMsvEntry entry;
		entry = cEntry->ChildDataL(selection->At(count));

		CMsvEntry* newEntry = cEntry->ChildEntryL(selection->At(count));
		delete newEntry;

		CMsvEntrySelection* children;

		children = cEntry->ChildrenWithMtmL(KUidMsvLocalServiceMtm);
		test(children->Count() > 0);
		delete children;
		children = cEntry->ChildrenWithTypeL(KUidMsvFolderEntry);
		test(children->Count() > 0);
		delete children;
		children = cEntry->ChildrenWithServiceL(KMsvLocalServiceIndexEntryId);
		test(children->Count() > 0);
		delete children;
		}

	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);

	CleanupStack::PopAndDestroy(3); // selection, cEntry, filter
	}

LOCAL_C void TestWriteOperationsL(CMsvSession& aSession, TMsvId aId, TInt aError)
	{
	// Test CMsvSession

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);
	
	TInt error;
	selection->AppendL(aId);

	TRAP(error, aSession.ChangeAttributesL(*selection, KMsvUnreadAttribute, KMsvInPreparationAttribute));
	test(error == aError);

	TRAP(error, aSession.IncPcSyncCountL(*selection));
	test(error == aError);

	TRAP(error, aSession.DecPcSyncCountL(*selection));
	test(error == aError);

	// Test CMsvEntry

	CMsvEntry* cEntry = CMsvEntry::NewL(aSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	TRAP(error, cEntry->ChangeAttributesL(*selection, KMsvUnreadAttribute, KMsvInPreparationAttribute));
	test(error == aError);

	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	active->StartL();
	CMsvOperation* operation = NULL;
	TRAP(error, operation = cEntry->CopyL(aId, KMsvGlobalInBoxIndexEntryId, active->iStatus));

	CActiveScheduler::Start();
	TMsvLocalOperationProgress progress = McliUtils::GetLocalProgressL(*operation);
	test(progress.iError == aError);
	delete operation;

	active->StartL();
	TRAP(error, operation = cEntry->MoveL(aId, KMsvGlobalOutBoxIndexEntryId, active->iStatus));

	CActiveScheduler::Start();
	progress = McliUtils::GetLocalProgressL(*operation);
	test(progress.iError == aError);
	delete operation;

	CleanupStack::PopAndDestroy(); // active
	
	cEntry->SetEntryL(aId);

	TMsvEntry entry = cEntry->Entry();
	TRAP(error, cEntry->ChangeL(entry));
	test(error == aError);

	CMsvStore* store = NULL;
	TRAP(error, store = cEntry->EditStoreL());
	if(error==KErrNone) // if we got a store put something in it so that ReadStore will not leave
		{
		RMsvWriteStream out;
		out.AssignLC(*store,KNullUid);
		out.WriteInt32L(1234);
		out.CommitL();
		CleanupStack::PopAndDestroy(&out);
		store->CommitL();		
		}
	test(error == aError);
	delete store;
	store = NULL;

	TRAP(error, store = cEntry->ReadStoreL());
	test(error == aError);
	delete store;

	cEntry->SetEntryL(cEntry->Entry().Parent());

	TRAP(error, cEntry->DeleteL(aId));
	test(error == aError);

	cEntry->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	
	TMsvEntry newEntry;
	newEntry.iType = KUidMsvMessageEntry;
	newEntry.iMtm = KUidMsvLocalServiceMtm;
	newEntry.iServiceId = KMsvLocalServiceIndexEntryId;

	TRAP(error, cEntry->CreateL(newEntry));
	test(error == aError);

	CleanupStack::PopAndDestroy(2); // cEntry, selection
	}

LOCAL_C void TestReadWriteOperationsL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));
	CMsgsTestUtils::SetDriveL(TInt(RemovableDrive));

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CleanupStack::PushL(session);
	
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	// Create an entry for testing function failure
	cEntry->CreateL(entry);
	CleanupStack::PopAndDestroy(); // cEntry

	// Remove the message server media
	RemoveMedia();

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
	test(ob->iArg1==TInt(RemovableDrive));

	TestReadOperationsL(*session);
	TestWriteOperationsL(*session, entry.Id(), KMsvMediaUnavailable);

	// Reinsert the wrong media
	InsertMedia(ETrue);

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaIncorrect);
	test(ob->iArg1==TInt(RemovableDrive));

	TestReadOperationsL(*session);
	TestWriteOperationsL(*session, entry.Id(), KMsvMediaIncorrect);

	// Remove the message server media again
	RemoveMedia();

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
	test(ob->iArg1==TInt(RemovableDrive));

	// Reinsert the correct media
	InsertMedia();

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaAvailable);
	test(ob->iArg1==TInt(RemovableDrive));

	TestReadOperationsL(*session);
	TestWriteOperationsL(*session, entry.Id(), KErrNone);

	CleanupStack::PopAndDestroy(); // session

	// Check drive still on TInt(RemovableDrive)
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == TInt(RemovableDrive));

	CleanupStack::PopAndDestroy(); // ob
	}

LOCAL_C void TestCreatingEntriesL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	// Test for missing disk
	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));
	CMsgsTestUtils::RemoveMailFolderL(EDriveC);
	CMsgsTestUtils::SetDriveL(TInt(RemovableDrive));

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CleanupStack::PushL(session);
	
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// Remove the message server media
	RemoveMedia();

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
	test(ob->iArg1==TInt(RemovableDrive));

	// Try and create an entry - This will switch the drive to TInt(RemovableDrive)
	cEntry->CreateL(entry);

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==TInt(RemovableDrive));
	test(ob->iArg2==EDriveC);

	// Make sure the entry was created
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry->SetEntryL(entry.Id());

	CleanupStack::PopAndDestroy(2); // cEntry, session

	// Check the message store is where we expect it to be
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);

	if (RunManual)
		{
		test.Printf(_L("Insert the correct Media then press any key.\n"));
		test.Getch();
		}

	CMsgsTestUtils::SetDriveL(TInt(RemovableDrive));

	// Now try again with a store on C:
	session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CleanupStack::PushL(session);
	
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// Remove the message server media
	RemoveMedia();

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
	test(ob->iArg1==TInt(RemovableDrive));

	// Try and create an entry - This will switch the drive to C:
	cEntry->CreateL(entry);

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==TInt(RemovableDrive));
	test(ob->iArg2==EDriveC);

	// Make sure the entry was created
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry->SetEntryL(entry.Id());

	CleanupStack::PopAndDestroy(2); // cEntry, session

	// Check the message store is where we expect it to be
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);

	// Now test with corrupted store on C:
	TInt mode;
	for(mode = 0; mode <= 3; mode++)
		{
		if (RunManual)
			{
			test.Printf(_L("Insert the correct Media then press any key.\n"));
			test.Getch();
			}

		// Try creating the session with store on TInt(RemovableDrive) 
		// C: has a corrupt a mail folder
		CMsgsTestUtils::CorruptIndexFileL(mode);
		CMsgsTestUtils::SetDriveL(TInt(RemovableDrive));

		session = CMsvSession::OpenAsyncL(*ob);
		ob->iSession = session; // Set session in observer
		CleanupStack::PushL(session);
		
		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvServerReady);

		cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
		CleanupStack::PushL(cEntry);

		// Remove the message server media
		RemoveMedia();

		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
		test(ob->iArg1==TInt(RemovableDrive));

		// Try and create an entry - This will switch the drive to C:
		cEntry->CreateL(entry);

		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvServerReady);
		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
		test(ob->iArg1==TInt(RemovableDrive));
		test(ob->iArg2==EDriveC);

		// Make sure the entry was created
		cEntry->SetEntryL(KMsvRootIndexEntryId);
		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
		cEntry->SetEntryL(entry.Id());

		CleanupStack::PopAndDestroy(2); // cEntry, session

		// Check the message store is where we expect it to be
		CMsgsTestUtils::WaitForServerClose();
		test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);
		}

	if (RunManual)
		{
		test.Printf(_L("Insert the correct Media then press any key.\n"));
		test.Getch();
		}

	// Test for incorrect disk
	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));
	CMsgsTestUtils::RemoveMailFolderL(EDriveC);
	CMsgsTestUtils::SetDriveL(TInt(RemovableDrive));

	session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CleanupStack::PushL(session);
	
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// Try and create an entry
	cEntry->CreateL(entry);

	// Remove the message server media
	RemoveMedia();

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
	test(ob->iArg1==TInt(RemovableDrive));

	// Insert the incorrect media
	InsertMedia(ETrue);

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaIncorrect);
	test(ob->iArg1==TInt(RemovableDrive));

	// Try and create an entry - This will switch the drive to C:
	cEntry->CreateL(entry);

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==TInt(RemovableDrive));
	test(ob->iArg2==EDriveC);

	// Make sure the entry was created
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry->SetEntryL(entry.Id());

	CleanupStack::PopAndDestroy(2); // cEntry, session

	// Check the message store is where we expect it to be
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);

	// Now test with corrupted store on C:
	for(mode = 0; mode <= 3; mode++)
		{
		if (RunManual)
			{
			test.Printf(_L("Insert the correct Media then press any key.\n"));
			test.Getch();
			}

		// Try creating the session with store on TInt(RemovableDrive)
		// C: has a corrupt a mail folder
		CMsgsTestUtils::CorruptIndexFileL(mode);
		CMsgsTestUtils::SetDriveL(TInt(RemovableDrive));

		session = CMsvSession::OpenAsyncL(*ob);
		ob->iSession = session; // Set session in observer
		CleanupStack::PushL(session);
		
		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvServerReady);

		cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
		CleanupStack::PushL(cEntry);

		// Remove the message server media
		RemoveMedia();

		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
		test(ob->iArg1==TInt(RemovableDrive));

		// Insert the wrong media
		InsertMedia(ETrue);

		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvMediaIncorrect);
		test(ob->iArg1==TInt(RemovableDrive));

		// Try and create an entry - This will switch the drive to C:
		cEntry->CreateL(entry);

		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvServerReady);
		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
		test(ob->iArg1==TInt(RemovableDrive));
		test(ob->iArg2==EDriveC);

		// Make sure the entry was created
		cEntry->SetEntryL(KMsvRootIndexEntryId);
		cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
		cEntry->SetEntryL(entry.Id());

		CleanupStack::PopAndDestroy(2); // cEntry, session

		// Check the message store is where we expect it to be
		CMsgsTestUtils::WaitForServerClose();
		test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);
		}

	if (RunManual)
		{
		test.Printf(_L("Insert the correct Media then press any key.\n"));
		test.Getch();
		}

	CleanupStack::PopAndDestroy(); // ob
	}

LOCAL_C void TestCreatingEntriesAndEditStoreL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	// Test for missing disk
	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));
	CMsgsTestUtils::RemoveMailFolderL(EDriveC);
	CMsgsTestUtils::SetDriveL(TInt(RemovableDrive));

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CleanupStack::PushL(session);
	
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// Remove the message server media
	RemoveMedia();

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaUnavailable);
	test(ob->iArg1==TInt(RemovableDrive));

	// Try and create an entry - This will switch the drive to TInt(RemovableDrive)
	cEntry->CreateL(entry);
	cEntry->SetEntryL(entry.Id());

	// create a store file
	CMsvStore *store=cEntry->EditStoreL();
	CleanupStack::PushL(store);
	store->CommitL();
	CleanupStack::PopAndDestroy(store);
	
	// check the store file was created on C: (session has updated iMessageFolder)
	_LIT(KMessageStore,"c:\\private\\1000484b\\Mail\\");
	TFileName storeName(KMessageStore);
	MsvUtils::ConstructEntryName(entry.iServiceId,entry.Id(),storeName,MsvUtils::EStore);
	CMsgsTestUtils::FileExists(storeName);


	// check we get the notifcations
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==TInt(RemovableDrive));
	test(ob->iArg2==EDriveC);

	// Make sure the entry was created
	cEntry->SetEntryL(KMsvRootIndexEntryId);
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	cEntry->SetEntryL(entry.Id());

	CleanupStack::PopAndDestroy(cEntry);
	CleanupStack::PopAndDestroy(session);
	
	// Check the message store is where we expect it to be
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);
	CleanupStack::PopAndDestroy(ob);
	}


LOCAL_C void TestChangeDriveOperationL()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));

	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CleanupStack::PushL(session);
	
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	TPckgBuf<TMsvIndexLoadProgress> progressPack;
	TMsvIndexLoadProgress progress;

	// Change to drive with no message store
	active->StartL();
	CMsvOperation* operation = NULL;
	operation = session->ChangeDriveL(TInt(RemovableDrive), active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation);

	// Check its possible to identify the progress
	TBuf8<KMsvProgressBufferLength> buf;
	buf.Copy(operation->ProgressL());

	TMsvServerOperationProgress serverProgress;
	TPckg<TMsvServerOperationProgress> package(serverProgress);
	package.Copy(buf.Ptr(), sizeof(TMsvServerOperationProgress));

	test(serverProgress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);
	test(package().iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	// Check progress
    progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	test(progress.iError == KErrNone);
//	test(progress.iState == TMsvIndexLoadProgress::EIndexNotLoaded);
	test(progress.iIndex.iTotal > 0);
	test(progress.iIndex.iCompleted == progressPack().iIndex.iTotal);
	test(progress.iIndex.iRemaining == 0);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	// Check notifications
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==EDriveC);
	test(ob->iArg2==TInt(RemovableDrive));

	// Complete operation
	CActiveScheduler::Start();

	// Check progress
    progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	test(progress.iError == KErrNone);
	test(progress.iState == TMsvIndexLoadProgress::EIndexComplete);
	test(progress.iIndex.iTotal > 0);
	test(progress.iIndex.iCompleted == progressPack().iIndex.iTotal);
	test(progress.iIndex.iRemaining == 0);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	CleanupStack::PopAndDestroy(4); // operation, active, cEntry, session

	// Check the message store is where we expect it to be
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == TInt(RemovableDrive));

	// Now switch back to C: which has a message store
	session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CleanupStack::PushL(session);
	
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	// Change to drive with message store
	active->StartL();
	operation = session->ChangeDriveL(EDriveC, active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation);

	// Check progress
    progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	test(progress.iError == KErrNone);
//	test(progress.iState == TMsvIndexLoadProgress::EIndexLoading);
	test(progress.iIndex.iTotal >= 0);
	test(progress.iIndex.iCompleted >= 0);
	test(progress.iIndex.iRemaining == progress.iIndex.iTotal - progress.iIndex.iCompleted);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	// Remember the total so we can check it later
	TInt total = progress.iIndex.iTotal;

	// Check notifications
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==TInt(RemovableDrive));
	test(ob->iArg2==EDriveC);

	// Complete operation
	CActiveScheduler::Start();

	// Check progress
    progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	test(progress.iError == KErrNone);
	test(progress.iState == TMsvIndexLoadProgress::EIndexComplete);
	test(progress.iIndex.iTotal == total);
	test(progress.iIndex.iCompleted == total);
	test(progress.iIndex.iRemaining == 0);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	CleanupStack::PopAndDestroy(4); // operation, active, cEntry, session

	// Check the message store is where we expect it to be
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);

	// Now lets try change to a drive with a corrupt index
	TInt mode;
	for(mode = 0; mode <= 3; mode++)
		{
		CMsgsTestUtils::CorruptIndexFileL(mode);
		CMsgsTestUtils::SetDriveL(TInt(RemovableDrive));

		session = CMsvSession::OpenAsyncL(*ob);
		ob->iSession = session; // Set session in observer
		CleanupStack::PushL(session);
	
		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvServerReady);

		cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
		CleanupStack::PushL(cEntry);

		active = new(ELeave)CTestActive;
		CleanupStack::PushL(active);

		// Change to drive with message store
		active->StartL();
		operation = session->ChangeDriveL(EDriveC, active->iStatus);
		test(operation->Mtm() == KUidMsvServerMtm);
		CleanupStack::PushL(operation);

		// Check progress
		progressPack.Copy(operation->ProgressL());
		progress = progressPack();
		test(progress.iError == KErrNone);
//		test(progress.iState == TMsvIndexLoadProgress::EIndexNotLoaded || progress.iState == TMsvIndexLoadProgress::EIndexRebuilding);
		test(progress.iIndex.iTotal >= 0);
		test(progress.iIndex.iCompleted >= 0);
		test(progress.iIndex.iRemaining == progress.iIndex.iTotal - progress.iIndex.iCompleted);
		test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

		// Check notifications
		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvServerReady);
		CActiveScheduler::Start();
		test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
		test(ob->iArg1==TInt(RemovableDrive));
		test(ob->iArg2==EDriveC);

		// Complete operation
		CActiveScheduler::Start();

		// Check progress
		progressPack.Copy(operation->ProgressL());
		progress = progressPack();
		test(progress.iError == KErrNone);
		test(progress.iState == TMsvIndexLoadProgress::EIndexComplete);
		test(progress.iIndex.iTotal > 0 );
		test(progress.iIndex.iCompleted == progress.iIndex.iTotal);
		test(progress.iIndex.iRemaining == 0);
		test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

		CleanupStack::PopAndDestroy(4); // operation, active, cEntry, session

		// Check the message store is where we expect it to be
		CMsgsTestUtils::WaitForServerClose();
		test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);
		}

	CleanupStack::PopAndDestroy(); // ob
	}

LOCAL_C void TestCreatingAccountsOnExternalDriveL()
	{
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);


	// Set drive to C:, clean message folders and reset repositories
	CMsgsTestUtils::SetDriveL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));
	CRepository* repository = CRepository::NewL(KUidMsvImapMtm);
	repository->Reset();
	delete repository;


	// create 4 imap accounts

	CEmailAccounts* account = CEmailAccounts::NewLC();	
	
	// check there are no accounts
	RArray<TImapAccount> imapAccounts;
	account->GetImapAccountsL(imapAccounts);	
	test(imapAccounts.Count() == 0);

	CImImap4Settings* imapSetting = new (ELeave) CImImap4Settings;
	CleanupStack::PushL(imapSetting);
	CImIAPPreferences* iap = CImIAPPreferences::NewLC();
	account->PopulateDefaultImapSettingsL(*imapSetting, *iap);
	TImapAccount id1 = account->CreateImapAccountL(_L("ImapAccount1"), *imapSetting, *iap, EFalse);
	TImapAccount id2 = account->CreateImapAccountL(_L("ImapAccount2"), *imapSetting, *iap, EFalse);
	TImapAccount id3 = account->CreateImapAccountL(_L("ImapAccount3"), *imapSetting, *iap, EFalse);
	TImapAccount id4 = account->CreateImapAccountL(_L("ImapAccount4"), *imapSetting, *iap, EFalse);

	// check accounts
	imapAccounts.Reset();
	account->GetImapAccountsL(imapAccounts);	
	test(imapAccounts.Count() == 4);

	CleanupStack::PopAndDestroy(3, account);


	// Change to an external drive (with no message store)

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CleanupStack::PushL(session);
	
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	TPckgBuf<TMsvIndexLoadProgress> progressPack;
	TMsvIndexLoadProgress progress;

	active->StartL();
	CMsvOperation* operation = NULL;
	operation = session->ChangeDriveL(TInt(RemovableDrive), active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation);

	// Check its possible to identify the progress
	TBuf8<KMsvProgressBufferLength> buf;
	buf.Copy(operation->ProgressL());

	TMsvServerOperationProgress serverProgress;
	TPckg<TMsvServerOperationProgress> package(serverProgress);
	package.Copy(buf.Ptr(), sizeof(TMsvServerOperationProgress));

	test(serverProgress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);
	test(package().iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	// Check progress
    progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	test(progress.iError == KErrNone);
	test(progress.iIndex.iTotal > 0);
	test(progress.iIndex.iCompleted == progressPack().iIndex.iTotal);
	test(progress.iIndex.iRemaining == 0);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	// Check notifications
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==EDriveC);
	test(ob->iArg2==TInt(RemovableDrive));

	// Complete operation
	CActiveScheduler::Start();

	// Check progress
    progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	test(progress.iError == KErrNone);
	test(progress.iState == TMsvIndexLoadProgress::EIndexComplete);
	test(progress.iIndex.iTotal > 0);
	test(progress.iIndex.iCompleted == progressPack().iIndex.iTotal);
	test(progress.iIndex.iRemaining == 0);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	CleanupStack::PopAndDestroy(4); // operation, active, cEntry, session

	// Check the message store is where we expect it to be
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == TInt(RemovableDrive));


	// create 2 imap accounts on external drive

	account = CEmailAccounts::NewLC();	
	
	// check there are no accounts
	imapAccounts.Reset();
	account->GetImapAccountsL(imapAccounts);	
	test(imapAccounts.Count() == 0);

	imapSetting = new (ELeave) CImImap4Settings;
	CleanupStack::PushL(imapSetting);
	iap = CImIAPPreferences::NewLC();
	account->PopulateDefaultImapSettingsL(*imapSetting, *iap);
	TImapAccount id5 = account->CreateImapAccountL(_L("ImapAccount5"), *imapSetting, *iap, EFalse);
	TImapAccount id6 = account->CreateImapAccountL(_L("ImapAccount6"), *imapSetting, *iap, EFalse);

	// check accounts
	imapAccounts.Reset();
	account->GetImapAccountsL(imapAccounts);	
	test(imapAccounts.Count() == 2);

	CleanupStack::PopAndDestroy(3, account);


	// Now switch back to C: which has a message store with 4 imap accounts

	session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CleanupStack::PushL(session);
	
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	// Change to drive with message store
	active->StartL();
	operation = session->ChangeDriveL(EDriveC, active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation);

	// Check progress
    progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	test(progress.iError == KErrNone);
	test(progress.iIndex.iTotal >= 0);
	test(progress.iIndex.iCompleted >= 0);
	test(progress.iIndex.iRemaining == progress.iIndex.iTotal - progress.iIndex.iCompleted);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	// Remember the total so we can check it later
	TInt total = progress.iIndex.iTotal;

	// Check notifications
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==TInt(RemovableDrive));
	test(ob->iArg2==EDriveC);

	// Complete operation
	CActiveScheduler::Start();

	// Check progress
    progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	test(progress.iError == KErrNone);
	test(progress.iState == TMsvIndexLoadProgress::EIndexComplete);
	test(progress.iIndex.iTotal == total);
	test(progress.iIndex.iCompleted == total);
	test(progress.iIndex.iRemaining == 0);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	CleanupStack::PopAndDestroy(4); // operation, active, cEntry, session

	// Check the message store is where we expect it to be
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);


	// Check that this message store has 4 imap accounts

	account = CEmailAccounts::NewLC();	
	imapAccounts.Reset();

	account->GetImapAccountsL(imapAccounts);	
	test(imapAccounts.Count() == 4);
	test(imapAccounts[0].iImapAccountName.Compare(_L("ImapAccount1")) == 0);
	test(imapAccounts[1].iImapAccountName.Compare(_L("ImapAccount2")) == 0);
	test(imapAccounts[2].iImapAccountName.Compare(_L("ImapAccount3")) == 0);
	test(imapAccounts[3].iImapAccountName.Compare(_L("ImapAccount4")) == 0);

	// Test Get<x>AccountL APIs
	TImapAccount tempId;
	account->GetImapAccountL(imapAccounts[1].iImapService, tempId);
	test(tempId.iImapAccountId == imapAccounts[1].iImapAccountId);
	CleanupStack::PopAndDestroy(account);


	// Move back to the external drive
	session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CleanupStack::PushL(session);
	
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	active->StartL();
	operation = session->ChangeDriveL(TInt(RemovableDrive), active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation);

	// Check its possible to identify the progress
	buf.Copy(operation->ProgressL());
	package.Copy(buf.Ptr(), sizeof(TMsvServerOperationProgress));

	test(serverProgress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);
	test(package().iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	// Check progress
    progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	test(progress.iError == KErrNone);
	test(progress.iIndex.iTotal > 0);
	test(progress.iIndex.iCompleted == progressPack().iIndex.iTotal);
	test(progress.iIndex.iRemaining == 0);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	// Check notifications
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==EDriveC);
	test(ob->iArg2==TInt(RemovableDrive));

	// Complete operation
	CActiveScheduler::Start();

	// Check progress
    progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	test(progress.iError == KErrNone);
	test(progress.iState == TMsvIndexLoadProgress::EIndexComplete);
	test(progress.iIndex.iTotal > 0);
	test(progress.iIndex.iCompleted == progressPack().iIndex.iTotal);
	test(progress.iIndex.iRemaining == 0);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	CleanupStack::PopAndDestroy(4); // operation, active, cEntry, session

	// Check the message store is where we expect it to be
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == TInt(RemovableDrive));

	account = CEmailAccounts::NewLC();	
	imapAccounts.Reset();

	account->GetImapAccountsL(imapAccounts);	
	test(imapAccounts.Count() == 2);
	test(imapAccounts[0].iImapAccountName.Compare(_L("ImapAccount5")) == 0);
	test(imapAccounts[1].iImapAccountName.Compare(_L("ImapAccount6")) == 0);

	// Test Get<x>AccountL APIs
	account->GetImapAccountL(imapAccounts[1].iImapService, tempId);
	test(tempId.iImapAccountId == imapAccounts[1].iImapAccountId);
	CleanupStack::PopAndDestroy(account);

	CleanupStack::PopAndDestroy(ob); // ob
	}

LOCAL_C void TestChangeDriveOperationWithHeapFail1L()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));

	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);
	
	TPckgBuf<TMsvIndexLoadProgress> progressPack;
	TMsvIndexLoadProgress progress;

	TInt error;
#if defined(_DEBUG)
	TInt failCount;
	failCount=0;
#endif	

	TBool finished = EFalse;
	TInt drive = EDriveC;

	CMsvOperation* operation = NULL;

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	while(!finished)
		{	
		__UHEAP_FAILNEXT(failCount++);

		// Change to drive with no message store
		active->StartL();
		TRAP(error, operation = session->ChangeDriveL(TInt(RemovableDrive), active->iStatus));

		if (error == KErrNone)
			{
			CleanupStack::PushL(operation);
			test(operation->Mtm() == KUidMsvServerMtm);

			// Check progress
			progressPack.Copy(operation->ProgressL());
			progress = progressPack();
			test(progress.iError == KErrNone);
			test(progress.iState == TMsvIndexLoadProgress::EIndexNotLoaded);
			test(progress.iIndex.iTotal > 0);
			test(progress.iIndex.iCompleted == progressPack().iIndex.iTotal);
			test(progress.iIndex.iRemaining == 0);
			test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

			// Complete operation
			CActiveScheduler::Start();

			if (active->iStatus == KErrNone)
				{
				finished = ETrue;
				drive = TInt(RemovableDrive);

				// Check progress
				progressPack.Copy(operation->ProgressL());
				progress = progressPack();
				test(progress.iError == KErrNone);
				test(progress.iState == TMsvIndexLoadProgress::EIndexComplete);
				test(progress.iIndex.iTotal > 0);
				test(progress.iIndex.iCompleted == progressPack().iIndex.iTotal);
				test(progress.iIndex.iRemaining == 0);
				test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);
				}
			else
				test(active->iStatus == KErrNoMemory);

			CleanupStack::PopAndDestroy(); // operation
			}
		else
			{
			active->Cancel();
			test(error == KErrNoMemory);
			}

		__UHEAP_RESET;
		}

	CleanupStack::PopAndDestroy(); // session

	// Check the message store is where we expect it to be
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == drive);

#if defined(_DEBUG)
	failCount = 0;
#endif	

	finished = EFalse;

	session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	// Now switch back to C: which has a message store
	while(!finished)
		{	
		__UHEAP_FAILNEXT(failCount++);

		// Change to drive with message store
		active->StartL();
		TRAP(error, operation = session->ChangeDriveL(EDriveC, active->iStatus));

		if (error == KErrNone)
			{
			CleanupStack::PushL(operation);
			test(operation->Mtm() == KUidMsvServerMtm);

			// Check progress
			progressPack.Copy(operation->ProgressL());
			progress = progressPack();
			test(progress.iError == KErrNone);
			test(progress.iState == TMsvIndexLoadProgress::EIndexLoading);
			test(progress.iIndex.iTotal >= 0);
			test(progress.iIndex.iCompleted >= 0);
			test(progress.iIndex.iRemaining == progress.iIndex.iTotal - progress.iIndex.iCompleted);
			test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

			// Remember the total so we can check it later
			TInt total = progress.iIndex.iTotal;

			// Complete operation
			CActiveScheduler::Start();

			if (active->iStatus == KErrNone)
				{
				finished = ETrue;
				drive = EDriveC;

				// Check progress
				progressPack.Copy(operation->ProgressL());
				progress = progressPack();
				test(progress.iError == KErrNone);
				test(progress.iState == TMsvIndexLoadProgress::EIndexComplete);
				test(progress.iIndex.iTotal == total);
				test(progress.iIndex.iCompleted == total);
				test(progress.iIndex.iRemaining == 0);
				test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);
				}
			else
				test(active->iStatus == KErrNoMemory);

			CleanupStack::PopAndDestroy(); // operation
			}
		else
			{
			active->Cancel();
			test(error == KErrNoMemory);
			}

		__UHEAP_RESET;
		}

	CleanupStack::PopAndDestroy(); // session

	// Check the message store is where we expect it to be
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == drive);

	// Now lets try change to a drive with a corrupt index
	TInt mode;
	for(mode = 0; mode <= 3; mode++)
		{
		CMsgsTestUtils::CorruptIndexFileL(mode);
		CMsgsTestUtils::SetDriveL(TInt(RemovableDrive));
#if defined(_DEBUG)		
		failCount = 0;
#endif		
		finished = EFalse;
		drive = TInt(RemovableDrive);

		session = CMsvSession::OpenSyncL(*ob);
		CleanupStack::PushL(session);

		while(!finished)
			{	
			__UHEAP_FAILNEXT(failCount++);

			// Change to drive with message store
			active->StartL();
			TRAP(error, operation = session->ChangeDriveL(EDriveC, active->iStatus));

			if (error == KErrNone)
				{
				CleanupStack::PushL(operation);
				test(operation->Mtm() == KUidMsvServerMtm);

				// Check progress
				progressPack.Copy(operation->ProgressL());
				progress = progressPack();
				test(progress.iError == KErrNone);
				test(progress.iState == TMsvIndexLoadProgress::EIndexNotLoaded || progress.iState == TMsvIndexLoadProgress::EIndexRebuilding);
				test(progress.iIndex.iTotal >= 0);
				test(progress.iIndex.iCompleted >= 0);
				test(progress.iIndex.iRemaining == progress.iIndex.iTotal - progress.iIndex.iCompleted);
				test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

				// Complete operation
				CActiveScheduler::Start();

				if (active->iStatus == KErrNone)
					{
					finished = ETrue;
					drive = EDriveC;

					// Check progress
					progressPack.Copy(operation->ProgressL());
					progress = progressPack();
					test(progress.iError == KErrNone);
					test(progress.iState == TMsvIndexLoadProgress::EIndexComplete);
					test(progress.iIndex.iTotal > 0 );
					test(progress.iIndex.iCompleted == progress.iIndex.iTotal);
					test(progress.iIndex.iRemaining == 0);
					test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);
					}
				else
					test(active->iStatus == KErrNoMemory);

				CleanupStack::PopAndDestroy(); // operation
				}
			else
				{
				active->Cancel();
				test(error == KErrNoMemory);
				}

			__UHEAP_RESET;
			}

		CleanupStack::PopAndDestroy(); // session

		// Check the message store is where we expect it to be
		CMsgsTestUtils::WaitForServerClose();
		test(MessageServer::CurrentDriveL(theUtils->FileSession()) == drive);
		}

	CleanupStack::PopAndDestroy(2); // active, ob
	}

LOCAL_C void TestCancelChangeDriveL()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));

	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);
	
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	// Change drive
	active->StartL();
	CMsvOperation* operation = session->ChangeDriveL(TInt(RemovableDrive), active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation);

	operation->Cancel();

	// Complete operation
	CActiveScheduler::Start();

	test(active->iStatus == KErrCancel);

	// Check progress
	TPckgBuf<TMsvIndexLoadProgress> progressPack;
	progressPack.Copy(operation->ProgressL());
	TMsvIndexLoadProgress progress = progressPack();
	test(progress.iError == KErrNone);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	CleanupStack::PopAndDestroy(2); // session, operation

	// Check the message store is where we expect it to be
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);

	session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	// Change drive
	active->StartL();
	operation = session->ChangeDriveL(TInt(RemovableDrive), active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation);

	// Complete operation
	CActiveScheduler::Start();

	operation->Cancel();
	test(active->iStatus == KErrNone);

	// Check progress
	progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	test(progress.iError == KErrNone);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	CleanupStack::PopAndDestroy(2); // session, operation

	// Check the message store is where we expect it to be
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == TInt(RemovableDrive));

	CleanupStack::PopAndDestroy(2); // active, ob
	}

LOCAL_C void TestMultipleRequestsL()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));

	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CTestActive* active1 = new(ELeave)CTestActive;
	CleanupStack::PushL(active1);

	CTestActive* active2 = new(ELeave)CTestActive;
	CleanupStack::PushL(active2);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	// Change drive
	active1->StartL();
	CMsvOperation* operation1 = session->ChangeDriveL(TInt(RemovableDrive), active1->iStatus);
	test(operation1->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation1);

	// Try and change drive again
	active2->StartL();
	CMsvOperation* operation2 = session->ChangeDriveL(TInt(RemovableDrive), active2->iStatus);
	test(operation2->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation2);

	// Complete operations
	CActiveScheduler::Start();
	CActiveScheduler::Start();

	test(active1->iStatus == KErrNone);
	test(active2->iStatus == KErrServerBusy);

	CleanupStack::PopAndDestroy(3); // session, operation1, operation2

	// Check the message store is where we expect it to be
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == TInt(RemovableDrive));

	// Now test if we can add an entry to the Inbox when there is an outstanding change drive operation
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	RemoveMedia();

	// Change drive
	active1->StartL();
	operation1 = session->ChangeDriveL(EDriveC, active1->iStatus);
	test(operation1->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation1);

	// Create an entry in the Inbox
	TRAPD(error, cEntry->CreateL(entry));
	test(error == KErrNone || error == KMsvMediaUnavailable);

	CActiveScheduler::Start();
	test(active1->iStatus == KErrNone);

	CleanupStack::PopAndDestroy(3); // session, cEntry, operation1

	// Check the message store is where we expect it to be
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);

	CMsgsTestUtils::SetDriveL(TInt(RemovableDrive));

	session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	RemoveMedia();
	InsertMedia(ETrue);

	// Change drive
	active1->StartL();
	operation1 = session->ChangeDriveL(EDriveC, active1->iStatus);
	test(operation1->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation1);

	// Create an entry in the Inbox
	TRAP(error, cEntry->CreateL(entry));
	test(error == KErrNone || error == KMsvMediaIncorrect);

	CActiveScheduler::Start();
	test(active1->iStatus == KErrNone);

	CleanupStack::PopAndDestroy(3); // session, cEntry, operation1

	// Check the message store is where we expect it to be
	CMsgsTestUtils::WaitForServerClose();
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);

	CleanupStack::PopAndDestroy(3); // active1, active2, ob
	}

LOCAL_C void TestSyncChangeDriveL()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));

	// Start the server
	CMsvServer* server = CMsvServer::NewL();
	CleanupStack::PushL(server);

	// Wait a bit
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(1000000);
	CActiveScheduler::Start();
	delete timer;

	// Change to drive with missing store
	test(server->ChangeDrive(TInt(RemovableDrive), NULL) == KErrNone);
	CleanupStack::PopAndDestroy(); // server

	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == TInt(RemovableDrive));

	// Start the server
	server = CMsvServer::NewL();
	CleanupStack::PushL(server);

	// Wait a bit
	timer = CTestTimer::NewL();
	timer->After(1000000);
	CActiveScheduler::Start();
	delete timer;

	// Change to drive with store
	test(server->ChangeDrive(EDriveC, NULL) == KErrNone);
	CleanupStack::PopAndDestroy(); // server

	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);

	// Try changing to drive with corrupted message store
	// Now lets try change to a drive with a corrupt index
	TInt mode;
	for(mode = 0; mode <= 3; mode++)
		{
		CMsgsTestUtils::CorruptIndexFileL(mode);
		CMsgsTestUtils::SetDriveL(TInt(RemovableDrive));

		// Start the server
		server = CMsvServer::NewL();
		CleanupStack::PushL(server);

		// Wait a bit
		timer = CTestTimer::NewL();
		timer->After(1000000);
		CActiveScheduler::Start();
		delete timer;

		// Change to drive with store
		test(server->ChangeDrive(EDriveC, NULL) == KErrNone);
		CleanupStack::PopAndDestroy(); // server

		test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);
		}
	}

LOCAL_C void TestSyncChangeDriveWithHeapFailL()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));

	// Start the server
	CMsvServer* server = CMsvServer::NewL();
	CleanupStack::PushL(server);

	// Wait a bit
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(1000000);
	CActiveScheduler::Start();
	delete timer;

#if defined(_DEBUG)
	TInt failCount;
	failCount=0;
#endif	

	TInt error;
	TBool finished = EFalse;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		// Change to drive with missing store
		error = server->ChangeDrive(TInt(RemovableDrive), NULL);

		__UHEAP_RESET;

		if (error == KErrNone)
			{
			finished = ETrue;
			CleanupStack::PopAndDestroy(); // server
			test(MessageServer::CurrentDriveL(theUtils->FileSession()) == TInt(RemovableDrive));
			}
		else
			test(error == KErrNoMemory);
		}

	// Start the server
	server = CMsvServer::NewL();
	CleanupStack::PushL(server);

	// Wait a bit
	timer = CTestTimer::NewL();
	timer->After(1000000);
	CActiveScheduler::Start();
	delete timer;

#if defined(_DEBUG)
	failCount = 0;
#endif	
	finished = EFalse;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		// Change to drive with store
		error = server->ChangeDrive(EDriveC, NULL);

		__UHEAP_RESET;

		if (error == KErrNone)
			{
			finished = ETrue;
			CleanupStack::PopAndDestroy(); // server
			test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);
			}
		else
			test(error == KErrNoMemory);
		}

	// Try changing to drive with corrupted message store
	// Now lets try change to a drive with a corrupt index
	TInt mode;
	for(mode = 0; mode <= 3; mode++)
		{
		CMsgsTestUtils::CorruptIndexFileL(mode);
		CMsgsTestUtils::SetDriveL(TInt(RemovableDrive));

		// Start the server
		server = CMsvServer::NewL();
		CleanupStack::PushL(server);

		// Wait a bit
		timer = CTestTimer::NewL();
		timer->After(1000000);
		CActiveScheduler::Start();
		delete timer;

#if defined(_DEBUG)
		failCount = 0;
#endif		
		finished = EFalse;

		while(!finished)
			{
			__UHEAP_FAILNEXT(failCount++);

			// Change to drive with store
			error = server->ChangeDrive(EDriveC, NULL);

			__UHEAP_RESET;

			if (error == KErrNone)
				{
				finished = ETrue;
				CleanupStack::PopAndDestroy(); // server
				test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC);
				}
			else
				test(error == KErrNoMemory);
			}
		}
	}

LOCAL_C void TestDriveContainsStoreL()
	{
	CMsgsTestUtils::RemoveMailFolderL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));

	test(!MessageServer::DriveContainsStore(theUtils->FileSession(), EDriveC));
	test(!MessageServer::DriveContainsStore(theUtils->FileSession(), TInt(RemovableDrive)));
	test(!MessageServer::DriveContainsStore(theUtils->FileSession(), EDriveY));

	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	// Create an index on C:
	CMsgsTestUtils::SetDriveL(EDriveC);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	delete session;
	
	CMsgsTestUtils::WaitForServerClose();

	test(MessageServer::DriveContainsStore(theUtils->FileSession(), EDriveC));
	test(!MessageServer::DriveContainsStore(theUtils->FileSession(), TInt(RemovableDrive)));
	test(!MessageServer::DriveContainsStore(theUtils->FileSession(), EDriveY));

	// Create an index on TInt(RemovableDrive)
	CMsgsTestUtils::SetDriveL(TInt(RemovableDrive));
	session = CMsvSession::OpenSyncL(*ob);
	delete session;
	
	CMsgsTestUtils::WaitForServerClose();

	test(MessageServer::DriveContainsStore(theUtils->FileSession(), EDriveC));
	test(MessageServer::DriveContainsStore(theUtils->FileSession(), TInt(RemovableDrive)));
	test(!MessageServer::DriveContainsStore(theUtils->FileSession(), EDriveY));

	CleanupStack::PopAndDestroy(); // ob
	}

LOCAL_C void TestOutstandingOperationsL()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session1 = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session1);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session1, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvSession* session2 = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session2);

	test(session1->OutstandingOperationsL() == 0);
	test(session2->OutstandingOperationsL() == 0);

	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	cEntry->CreateL(entry);

	test(session1->OutstandingOperationsL() == 0);
	test(session2->OutstandingOperationsL() == 0);

	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	active->StartL();
	CMsvOperation* operation = NULL;
	operation = cEntry->CopyL(entry.Id(), KMsvGlobalInBoxIndexEntryId, active->iStatus);
	CleanupStack::PushL(operation);

	test(session1->OutstandingOperationsL() == 1);
	test(session2->OutstandingOperationsL() == 1);

	// Complete operation
	CActiveScheduler::Start();

	test(session1->OutstandingOperationsL() == 0);
	test(session2->OutstandingOperationsL() == 0);

	CleanupStack::PopAndDestroy(2); // operation, active
	CleanupStack::PopAndDestroy(4); // session2, cEntry, session1, ob

	CMsgsTestUtils::WaitForServerClose();
	}

LOCAL_C void TestCurrentDriveWithHeapFailL()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);

#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	
	TBool finished = EFalse;
	TInt drive = 0;
	TInt error;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		TRAP(error, drive = MessageServer::CurrentDriveL(theUtils->FileSession()));

		__UHEAP_RESET;
		
		if (error == KErrNone)
			{
			finished = ETrue;
			test(drive == EDriveC);
			}
		else
			test(error == KErrNoMemory);
		}
	}

LOCAL_C void TestCurrentDriveWithFileFailL()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);

	TInt failCount;
	failCount=0;
	
	TBool finished = EFalse;
	TInt drive = 0;
	TInt error;

	while(!finished)
		{
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		TRAP(error, drive = MessageServer::CurrentDriveL(theUtils->FileSession()));

		theUtils->FileSession().SetErrorCondition(KErrNone, 0);
		
		if (error == KErrNone)
			{
			finished = ETrue;
			test(drive == EDriveC);
			}
		else
			test(error == KErrGeneral);
		}
	}

LOCAL_C void TestDriveContainsStoreWithHeapFailL()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);

	// Make sure the drive contains a store
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	delete session;
	CleanupStack::PopAndDestroy(); // ob

#if defined(_DEBUG)	
	TInt failCount;
	failCount=0;
#endif	
	TBool result = EFalse;

	while(!result)
		{
		__UHEAP_FAILNEXT(failCount++);

		result = MessageServer::DriveContainsStore(theUtils->FileSession(), EDriveC);

		__UHEAP_RESET;
		}

	CMsgsTestUtils::WaitForServerClose();
	}

LOCAL_C void TestDriveContainsStoreWithFileFailL()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);

	// Make sure the drive contains a store
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	delete session;
	CleanupStack::PopAndDestroy(); // ob

	TInt failCount;
	failCount=0;

	TBool result = EFalse;

	while(!result)
		{
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);

		result = MessageServer::DriveContainsStore(theUtils->FileSession(), EDriveC);

		theUtils->FileSession().SetErrorCondition(KErrNone, 0);
		}

	CMsgsTestUtils::WaitForServerClose();
	}

LOCAL_C void TestLockDriveOperationL()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));

	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; // Set session in observer
	CleanupStack::PushL(session);
	
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	TPckgBuf<TMsvIndexLoadProgress> progressPack;
	TMsvIndexLoadProgress progress;

	// Change to drive with no message store
	active->StartL();
	CMsvOperation* operation = NULL;
	operation = session->ChangeDriveL(TInt(RemovableDrive), active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation);

	// Check its possible to identify the progress
	TBuf8<KMsvProgressBufferLength> buf;
	buf.Copy(operation->ProgressL());

	TMsvServerOperationProgress serverProgress;
	TPckg<TMsvServerOperationProgress> package(serverProgress);
	package.Copy(buf.Ptr(), sizeof(TMsvServerOperationProgress));

	test(serverProgress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);
	test(package().iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	// Check progress
    progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	test(progress.iError == KErrNone);
//	test(progress.iState == TMsvIndexLoadProgress::EIndexNotLoaded);
	test(progress.iIndex.iTotal > 0);
	test(progress.iIndex.iCompleted == progressPack().iIndex.iTotal);
	test(progress.iIndex.iRemaining == 0);
	test(progress.iOperationType == TMsvServerOperationProgress::EMsvChangeDriveOperation);

	// Check notifications
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvMediaChanged);
	test(ob->iArg1==EDriveC);
	test(ob->iArg2==TInt(RemovableDrive));

	// Complete operation
	CActiveScheduler::Start();


	operation = session->CopyStoreL(TInt(RemovableDrive), active->iStatus);
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == TInt(RemovableDrive));
	test(operation->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation);

	// Check progress
    progressPack.Copy(operation->ProgressL());
	progress = progressPack();
	
	
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	
	//Check current drive 
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == TInt(RemovableDrive));
	
	// Create an entry for testing function failure
	cEntry->CreateL(entry);
	
	//Check if defaultDrive has been chosen since store is locked 
	test(MessageServer::CurrentDriveL(theUtils->FileSession()) == EDriveC); 
	CleanupStack::PopAndDestroy(); // cEntry


	CleanupStack::PopAndDestroy(4); // operation, active, cEntry, session
	CleanupStack::PopAndDestroy(); // ob
	}

LOCAL_C void TestOutstandingOperationsWithHeapFileL()
	{
	RMsvServerSession server;
	server.Connect(theUtils->FileSession());

	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	cEntry->CreateL(entry);

	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	active->StartL();
	CMsvOperation* operation = NULL;
	operation = cEntry->CopyL(entry.Id(), KMsvGlobalInBoxIndexEntryId, active->iStatus);
	CleanupStack::PushL(operation);

	TInt failCount;
	failCount=0;

	TBool finished = EFalse;
	TInt result = 0;
	TInt error;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		TRAP(error, result = session->OutstandingOperationsL());

		__UHEAP_RESET;

		if (error == KErrNone)
			{
			finished = ETrue;
			test(result = 1);
			}
		else
			test(error == KErrNoMemory);
		}

	failCount = 0;

	finished = EFalse;
	result = 0;

	while(!finished)
		{
		server.SetFailure(EHeapFailure, RHeap::EFailNext, failCount++);

		TRAP(error, result = session->OutstandingOperationsL());

		server.SetFailure(EHeapFailure, RHeap::ENone);

		if (error == KErrNone)
			{
			finished = ETrue;
			test(result = 1);
			}
		else
			test(error == KErrNoMemory);
		}

	// Complete operation
	CActiveScheduler::Start();

	failCount = 0;
	finished = EFalse;
	result = 1;

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		TRAP(error, result = session->OutstandingOperationsL());

		__UHEAP_RESET;

		if (error == KErrNone)
			{
			finished = ETrue;
			test(result == 0);
			}
		else
			test(error == KErrNoMemory);
		}

	failCount = 0;
	finished = EFalse;
	result = 1;

	while(!finished)
		{
		server.SetFailure(EHeapFailure, RHeap::EFailNext, failCount++);

		TRAP(error, result = session->OutstandingOperationsL());

		server.SetFailure(EHeapFailure, RHeap::ENone);

		if (error == KErrNone)
			{
			finished = ETrue;
			test(result == 0);
			}
		else
			test(error == KErrNoMemory);
		}

	CleanupStack::PopAndDestroy(5); // operation, active, cEntry, session, ob
	server.Close();

	CMsgsTestUtils::WaitForServerClose();
	}

LOCAL_C void TestClientEntryL()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(TInt(RemovableDrive));

	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// Create an entry
	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry1);
	cEntry->DeleteL(entry1.Id());
	cEntry->CreateL(entry1);

	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);

	// Change drive
	active->StartL();
	CMsvOperation* operation = NULL;
	operation = session->ChangeDriveL(TInt(RemovableDrive), active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);

	// Complete operation
	CActiveScheduler::Start();
	delete operation;

	// Wait a bit for the notifications
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(3000000);
	CActiveScheduler::Start();
	delete timer;

	// Check the child doesn't exist
	TInt count = cEntry->Count();
	while(count--)
		test((*cEntry)[count].Id() != entry1.Id());
	TRAPD(error, cEntry->SetEntryL(entry1.Id()));
	test(error == KErrNotFound);

	// Create an entry
	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry2);

	// Now switch drive back
	active->StartL();
	operation = session->ChangeDriveL(EDriveC, active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);

	// Complete operation
	CActiveScheduler::Start();
	delete operation;

	// Wait a bit for the notifications
	timer = CTestTimer::NewL();
	timer->After(3000000);
	CActiveScheduler::Start();
	delete timer;

	// Check the child that shouldn't exist
	count = cEntry->Count();
	while(count--)
		test((*cEntry)[count].Id() != entry2.Id());
	TRAP(error, cEntry->SetEntryL(entry2.Id()));
	test(error == KErrNotFound);

	// Check the child that should exist
	count = cEntry->Count();
	while(count--)
		{
		if ((*cEntry)[count].Id() == entry1.Id())
			break;
		}
	test(count >= 0);
	cEntry->SetEntryL(entry1.Id());
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);

	// Switch back to removable disk
	active->StartL();
	operation = session->ChangeDriveL(TInt(RemovableDrive), active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);

	// Complete operation
	CActiveScheduler::Start();
	delete operation;

	// Wait a bit for the notifications
	timer = CTestTimer::NewL();
	timer->After(3000000);
	CActiveScheduler::Start();
	delete timer;

	// Check the child that shouldn't exist
	count = cEntry->Count();
	while(count--)
		test((*cEntry)[count].Id() != entry1.Id());
	TRAP(error, cEntry->SetEntryL(entry1.Id()));
	test(error == KErrNotFound);

	// Check the child that should exist
	count = cEntry->Count();
	while(count--)
		{
		if ((*cEntry)[count].Id() == entry2.Id())
			break;
		}
	test(count >= 0);
	cEntry->SetEntryL(entry2.Id());
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);

	// Now switch drive back
	active->StartL();
	operation = session->ChangeDriveL(EDriveC, active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);

	// Complete operation
	CActiveScheduler::Start();
	delete operation;

	// Wait a bit for the notifications
	timer = CTestTimer::NewL();
	timer->After(3000000);
	CActiveScheduler::Start();
	delete timer;

	// Check the child that shouldn't exist
	count = cEntry->Count();
	while(count--)
		test((*cEntry)[count].Id() != entry2.Id());
	TRAP(error, cEntry->SetEntryL(entry2.Id()));
	test(error == KErrNotFound);

	// Check the child that should exist
	count = cEntry->Count();
	while(count--)
		{
		if ((*cEntry)[count].Id() == entry1.Id())
			break;
		}
	test(count >= 0);
	cEntry->SetEntryL(entry1.Id());
	cEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);

	CleanupStack::PopAndDestroy(4); // active, cEntry, session, ob
	CMsgsTestUtils::WaitForServerClose();
	}

LOCAL_C void doTestsL()
	{
	PanicSchedulerOnError = EFalse;

#if !defined(__WINS__)
	test.Printf(_L("Manual or Automatic?\n"));
	test.Printf(_L("Press a key in the next 10 seconds to run these tests manually\n"));
	RunManual = CMsgsTestUtils::WaitForKeyL(10000000);

	// Are we running manually or automatic
	if (RunManual)
		{
		test.Printf(_L("Running tests manually. Make sure drive contains a removable disk. Please follow instructions. Press any key to continue.\n"));
		test.Getch();
		}
	else
		test.Printf(_L("Running tests automatically.\n"));
#endif

	CMsgsTestUtils::Start(_L("Media Changed after Startup And Get Proper Notification"));
	TestEventNotificationOnChangeDriveL();

/*	CMsgsTestUtils::Next(_L("Media not available on startup"));
	TestMediaRemovedOnStartL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Media changed on startup"));
	TestMediaChangedOnStartL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Media removed whilst running"));
	TestMediaRemovedWhileRunningL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Media changed whilst running"));
	TestMediaChangedWhileRunningL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Lock drive operation"));
	TestLockDriveOperationL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Read and Write functions"));
	TestReadWriteOperationsL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Creating entries in Inbox"));
	TestCreatingEntriesL();
	TestCreatingEntriesAndEditStoreL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Change drive operation"));
	TestChangeDriveOperationL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Cancelling drive change"));
	TestCancelChangeDriveL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Multiple change requests"));
	TestMultipleRequestsL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Synchronous drive change"));
	TestSyncChangeDriveL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Miscellaneous functionality"));
	TestDriveContainsStoreL();
	TestOutstandingOperationsL();
	TestCurrentDriveWithHeapFailL();
	TestCurrentDriveWithFileFailL();
	TestDriveContainsStoreWithHeapFailL();
	TestDriveContainsStoreWithFileFailL();
	TestOutstandingOperationsWithHeapFileL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Drive change and client entry context"));
	TestClientEntryL();
	CMsgsTestUtils::Complete();

	// Don't bother with manual running of tests for heap failure testing
	RunManual = EFalse;

	CMsgsTestUtils::Next(_L("Change drive with heap failure in Client"));
	TestChangeDriveOperationWithHeapFail1L();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Synchronous change drive with heap failure"));
	TestSyncChangeDriveWithHeapFailL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Creating Accounts On External Drive"));
	TestCreatingAccountsOnExternalDriveL();*/
	CMsgsTestUtils::Complete();
	}
