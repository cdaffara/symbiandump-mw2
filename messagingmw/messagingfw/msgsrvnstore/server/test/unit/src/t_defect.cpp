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

#include <mtclreg.h>
#include <mtsr.h>
#include <msventry.h>
#include <mtuireg.h>
#include <mtudreg.h>
#include <mtclbase.h>
#include "Testsetup.h"

#include "CAsyncWaiter.h"

#include <cmsvattachment.h>
#include <mmsvattachmentmanager.h>

RTest test(_L("Defect fix test harness"));

LOCAL_C void TestNotifyBug1L()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	// Create the first CMsvEntry sitting on drafts
	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvDraftEntryId, TMsvSelectionOrdering(0, EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry1);

	// Add an observer to the first CMsvEntry
	CEntryObserver* eOb1 = new(ELeave)CEntryObserver;
	CleanupStack::PushL(eOb1);
	cEntry1->AddObserverL(*eOb1);

	// Create another CMsvEntry sitting on drafts
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvDraftEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);

	// Create an entry in drafts
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry2->CreateL(entry);

	// Wait for create notification to be received by the first CMsvEntry
	eOb1->Start();
	CActiveScheduler::Start();

	// Change the entry in drafts
	// This queues a change notification in the server
	// It is waiting to be handled by all the observers
	cEntry2->SetEntryL(entry.Id());
	cEntry2->ChangeL(entry);
	cEntry2->SetEntryL(KMsvDraftEntryId);

	// Start an asynchronous move using the second CMsvEntry
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->Start();
	CMsvOperation* op = cEntry2->MoveL(entry.Id(), KMsvGlobalOutBoxIndexEntryId, wait->iStatus);
	CleanupStack::PushL(op);

	// The first CMsvEntry is out of date because it hasn't been told that the entry has been moved from drafts yet
	// Switching the CMsvEntry in and out of drafts forces it to be up to date
	// But there is still a change notification waiting to be handled
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	cEntry1->SetEntryL(KMsvDraftEntryId);

	// The first CMsvEntry is now up to date, but in the next CActiveScheduler::Start() it receives a change notification
	// for a child that doesn't exist
	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(7); // op, wait, cEntry2, eOb1, cEntry1, session, ob
	}

LOCAL_C void TestNotifyBug2L()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	// Create the first CMsvEntry sitting on drafts
	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session, KMsvDraftEntryId, TMsvSelectionOrdering(0, EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry1);

	// Add an observer to the first CMsvEntry
	CEntryObserver* eOb1 = new(ELeave)CEntryObserver;
	CleanupStack::PushL(eOb1);
	cEntry1->AddObserverL(*eOb1);

	// Create another CMsvEntry sitting on drafts
	CMsvEntry* cEntry2 = CMsvEntry::NewL(*session, KMsvDraftEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry2);

	// Create an entry in drafts
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry2->CreateL(entry);

	// Wait for create notification to be received by the first CMsvEntry
	eOb1->Start();
	CActiveScheduler::Start();

	// Change the entry in drafts
	// This queues a change notification in the server
	// It is waiting to be handled by all the observers
	cEntry2->SetEntryL(entry.Id());
	cEntry2->ChangeL(entry);
	cEntry2->SetEntryL(KMsvDraftEntryId);

	// Start an asynchronous delete using the second CMsvEntry
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->Start();
	CMsvOperation* op = cEntry2->DeleteL(entry.Id(), wait->iStatus);
	CleanupStack::PushL(op);

	// The first CMsvEntry is out of date because it hasn't been told that the entry has been deleted from drafts yet
	// Switching the CMsvEntry in and out of drafts forces it to be up to date
	// But there is still a change notification waiting to be handled
	cEntry1->SetEntryL(KMsvRootIndexEntryId);
	cEntry1->SetEntryL(KMsvDraftEntryId);

	// The first CMsvEntry is now up to date, but in the next CActiveScheduler::Start() it receives a change notification
	// for a child that doesn't exist
	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(7); // op, wait, cEntry2, eOb1, cEntry1, session, ob
	}


// Define Uid's to use
_LIT(KDefaultRegistryFileStoreName, "\\private\\1000484b\\Mtm Registry v2");

_LIT(KMtmDataFile, "\\system\\mtm\\REGMTM1.dat");
_LIT(KMtmName, "Reg control test MTM 1");
const TUid KUidTestMtm = {0x10005097};
_LIT(KMtmDllName, "z:\\system\\libs\\regmtm1.dll");


LOCAL_C void TestClientMtmLoadPanicL()
	{
	// Make sure the server is closed
	CMsgsTestUtils::WaitForServerClose();
	User::After(500000);
	TChar driveChar= theUtils->FileSession().GetSystemDriveChar();
	TBuf<2> systemDrive;
	systemDrive.Append(driveChar);
	systemDrive.Append(KDriveDelimiter);
	TPath pathName(systemDrive);
	pathName.Append(KDefaultRegistryFileStoreName);
	
	theUtils->FileSession().Delete(pathName);

	CMtmDllInfoArray* mtmdllinfoarray=new(ELeave) CMtmDllInfoArray();
	CleanupStack::PushL(mtmdllinfoarray);

	// Create library info with silly imports
	CMtmDllInfo* mtmdllinfo=CMtmDllInfo::NewL(KMtmName, TUidType(KDynamicLibraryUid, KUidMtmServerComponent, TUid::Uid(KUidMtmDefaultSpecificVal)),KMtmDllName,123,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo);

	mtmdllinfo=CMtmDllInfo::NewL(KMtmName, TUidType(KDynamicLibraryUid, KUidMtmClientComponent, TUid::Uid(KUidMtmDefaultSpecificVal)),KMtmDllName,234,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo);

	TCapabilitySet capSet;
	capSet.SetEmpty();
	CleanupStack::Pop(mtmdllinfoarray); // next line takes ownership
	CMtmGroupData* mtmgroupdata=CMtmGroupData::NewL(KUidTestMtm, KUidTestMtm, mtmdllinfoarray, capSet);
	CleanupStack::PushL(mtmgroupdata);

	CFileStore* filestore = CPermanentFileStore::ReplaceLC(theUtils->FileSession(), KMtmDataFile, EFileShareExclusive|EFileStream|EFileWrite);
	TUidType uidtype(KPermanentFileStoreLayoutUid,KUidMsvDataComponent, KUidTestMtm);
	filestore->SetTypeL(uidtype);
	RStoreWriteStream out;
	TStreamId streamid=out.CreateLC(*filestore); // Push to stack
	mtmgroupdata->ExternalizeL(out);
	out.CommitL();
	CleanupStack::PopAndDestroy(); // out
	filestore->SetRootL(streamid);
	filestore->CommitL();
	CleanupStack::PopAndDestroy(2, mtmgroupdata); // filestore, mtmgroupdata
	
	// Now try to instatiate the DLL
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	TInt err = session->InstallMtmGroup(KMtmDataFile);
	test(err == KErrNone || err == KErrAlreadyExists);
	CClientMtmRegistry* reg = CClientMtmRegistry::NewL(*session);
	CleanupStack::PushL(reg);

	CBaseMtm* mtm = NULL;
	TRAPD(error, mtm = reg->NewMtmL(KUidTestMtm));
	test(mtm == NULL);
	test(error == KErrBadLibraryEntryPoint);

	// Defect panic'd in here
	CleanupStack::PopAndDestroy(3); // reg, session, ob
	}

LOCAL_C void TestServerMtmLoadPanicL()
	{
	// Make sure the server is closed
	CMsgsTestUtils::WaitForServerClose();
	User::After(500000);
	CMsvServer* server = CMsgsTestUtils::CreateServerL();
	CleanupStack::PushL(server);

	CServerMtmDllRegistry* reg = CServerMtmDllRegistry::NewL(theUtils->FileSession());
	CleanupStack::PushL(reg);

	CMtmRegistryControl* control = CMtmRegistryControl::NewL(theUtils->FileSession(), *reg);
	CleanupStack::PushL(control);

	CBaseServerMtm* mtm = NULL;
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*server, KMsvRootIndexEntryId);
	TRAPD(error, mtm = reg->NewServerMtmL(KUidTestMtm, sEntry));
	test(mtm == NULL);
	test(error==KErrBadLibraryEntryPoint|| error==KErrAlreadyExists);
	
	// Defect panic'd in here
	CleanupStack::PopAndDestroy(3); // control, reg, server
	}

LOCAL_C void TestUiMtmLoadPanicL()
	{
	// Make sure the server is closed
	CMsgsTestUtils::WaitForServerClose();
	User::After(500000);
	TChar driveChar= theUtils->FileSession().GetSystemDriveChar();
	TBuf<2> systemDrive;
	systemDrive.Append(driveChar);
	systemDrive.Append(KDriveDelimiter);
	TPath pathName(systemDrive);
	pathName.Append(KDefaultRegistryFileStoreName);
	theUtils->FileSession().Delete(pathName);

	CMtmDllInfoArray* mtmdllinfoarray=new(ELeave) CMtmDllInfoArray();
	CleanupStack::PushL(mtmdllinfoarray);

	// Create library info with silly imports
	CMtmDllInfo* mtmdllinfo=CMtmDllInfo::NewL(KMtmName, TUidType(KDynamicLibraryUid, KUidMtmServerComponent, TUid::Uid(KUidMtmDefaultSpecificVal)),KMtmDllName,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo);

	mtmdllinfo=CMtmDllInfo::NewL(KMtmName, TUidType(KDynamicLibraryUid, KUidMtmClientComponent, TUid::Uid(KUidMtmDefaultSpecificVal)),KMtmDllName,1,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo);

	mtmdllinfo=CMtmDllInfo::NewL(KMtmName, TUidType(KDynamicLibraryUid, KUidMtmUiComponent, TUid::Uid(KUidMtmDefaultSpecificVal)),KMtmDllName,345,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo);

	mtmdllinfo=CMtmDllInfo::NewL(KMtmName, TUidType(KDynamicLibraryUid, KUidMtmUiDataComponent, TUid::Uid(KUidMtmDefaultSpecificVal)),KMtmDllName,456,TVersion(2,0,0));
	mtmdllinfoarray->AddMtmDllInfoL(mtmdllinfo);

	TCapabilitySet capSet;
	capSet.SetEmpty();
	CleanupStack::Pop(mtmdllinfoarray); // next line takes ownership
	CMtmGroupData* mtmgroupdata=CMtmGroupData::NewL(KUidTestMtm, KUidTestMtm, mtmdllinfoarray, capSet);
	CleanupStack::PushL(mtmgroupdata);

	CFileStore* filestore = CPermanentFileStore::ReplaceLC(theUtils->FileSession(), KMtmDataFile, EFileShareExclusive|EFileStream|EFileWrite);
	TUidType uidtype(KPermanentFileStoreLayoutUid,KUidMsvDataComponent, KUidTestMtm);
	filestore->SetTypeL(uidtype);
	RStoreWriteStream out;
	TStreamId streamid=out.CreateLC(*filestore); // Push to stack
	mtmgroupdata->ExternalizeL(out);
	out.CommitL();
	CleanupStack::PopAndDestroy(); // out
	filestore->SetRootL(streamid);
	filestore->CommitL();
	CleanupStack::PopAndDestroy(2, mtmgroupdata); // filestore, mtmgroupdata

	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	TInt err = session->InstallMtmGroup(KMtmDataFile);
	test(err == KErrNone || err == KErrAlreadyExists);
	CClientMtmRegistry* clientReg = CClientMtmRegistry::NewL(*session);
	CleanupStack::PushL(clientReg);

	CBaseMtm* client = NULL;
	client = clientReg->NewMtmL(KUidTestMtm);
	CleanupStack::PushL(client);

	CMtmUiRegistry* uiReg = CMtmUiRegistry::NewL(*session);
	CleanupStack::PushL(uiReg);

	if (client !=NULL)
		{
		TRAPD(error, uiReg->NewMtmUiL(*client));
		test(error == KErrBadLibraryEntryPoint);
		}
	CleanupStack::PopAndDestroy(5); // uiReg, client, clientReg, session, ob
	}

LOCAL_C void TestUiDataLoadPanicL()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMtmUiDataRegistry* uiReg = CMtmUiDataRegistry::NewL(*session);
	CleanupStack::PushL(uiReg);

	CBaseMtmUi* mtm = NULL;
	TRAPD(error, uiReg->NewMtmUiDataLayerL(KUidTestMtm));
	test(mtm == NULL);
	test(error == KErrBadLibraryEntryPoint || error == KErrNotFound);

	CleanupStack::PopAndDestroy(3); // uiReg, session, ob
	}

LOCAL_C void TestSetSendingStateAttributeL()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvDraftEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TMsvEntry entry1;
	entry1.iType = KUidMsvMessageEntry;
	entry1.iMtm = KUidMsvLocalServiceMtm;
	entry1.iServiceId = KMsvLocalServiceIndexEntryId;
	entry1.SetSendingState(12);
	cEntry->CreateL(entry1);
	selection->AppendL(entry1.Id());

	TMsvEntry entry2;
	entry2.iType = KUidMsvMessageEntry;
	entry2.iMtm = KUidMsvLocalServiceMtm;
	entry2.iServiceId = KMsvLocalServiceIndexEntryId;
	entry2.SetSendingState(3);
	cEntry->CreateL(entry2);
	selection->AppendL(entry2.Id());
	
	session->ChangeAttributesL(*selection, 6<<KMsvSendingStateShift, 0);

	cEntry->SetEntryL(entry1.Id());
	test(cEntry->Entry().SendingState() == 6);

	cEntry->SetEntryL(entry2.Id());
	test(cEntry->Entry().SendingState() == 6);
	
	CleanupStack::PopAndDestroy(4); // selection, cEntry, session, ob
	}

LOCAL_C void TestAccessViolationL()
	{
	// Make sure the server is closed
	CMsgsTestUtils::WaitForServerClose();

	CMsvServer* server = CMsvServer::NewL();
	CleanupStack::PushL(server);

	// Wait a bit
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(15000000);
	CActiveScheduler::Start();
	delete timer;

	CleanupStack::PopAndDestroy(); // server
	}

LOCAL_C void TestLargeEntryL()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session1 = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session1);

	CMsvEntry* cEntry1 = CMsvEntry::NewL(*session1, KMsvDraftEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry1);

	// Create a folder
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry1->CreateL(folder);

	cEntry1->SetEntryL(folder.Id());

	TInt factor;
	for(factor = 1; factor <= 10; factor++)
		{
		// Create some large text buffers
		HBufC* text = HBufC::NewLC(factor * 1000);
		text->Des().SetLength(factor * 1000);

		// Create a large entry
		TMsvEntry entry;
		entry.iType = KUidMsvMessageEntry;
		entry.iMtm = KUidMsvLocalServiceMtm;
		entry.iServiceId = KMsvLocalServiceIndexEntryId;
		entry.iDetails.Set(*text);
		entry.iDescription.Set(*text);

		cEntry1->CreateL(entry);

		// Make sure data can be transferred across boundary
		cEntry1->SetEntryL(KMsvDraftEntryId);
		cEntry1->SetEntryL(folder.Id());

		// Check with another session
		CMsvSession* session2 = CMsvSession::OpenSyncL(*ob);
		CleanupStack::PushL(session2);

		CMsvEntry* cEntry2 = CMsvEntry::NewL(*session2, folder.Id(), TMsvSelectionOrdering());
		delete cEntry2;
		CleanupStack::PopAndDestroy(); // session2

		cEntry1->SetEntryL(entry.Id());

		// Check with another session
		session2 = CMsvSession::OpenSyncL(*ob);
		CleanupStack::PushL(session2);

		cEntry2 = CMsvEntry::NewL(*session2, entry.Id(), TMsvSelectionOrdering());
		delete cEntry2;

		CleanupStack::PopAndDestroy(2); // session2, text
		}

	CleanupStack::PopAndDestroy(3); // cEntry1, session1, ob
	}

LOCAL_C void TestReadOnlyDeletionL()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvDraftEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// Create an entry
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry);

	// Generate name of attachment
	cEntry->SetEntryL(entry.Id());
	TFileName fileName;

	fileName.Append(_L("Test"));
	CMsvStore* store = cEntry->EditStoreL();
	CleanupStack::PushL(store);

	CAsyncWaiter* waiter = CAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	fileName.Append(_L("Test"));
	attachment->SetAttachmentNameL(fileName);
	RFile file;
	store->AttachmentManagerL().CreateAttachmentL(fileName, file, attachment, waiter->iStatus);
	CleanupStack::Pop(attachment); // ownership passed
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::PopAndDestroy(waiter);
	
	CleanupClosePushL(file);
	User::LeaveIfError(file.Write(_L8("some text")));
	User::LeaveIfError(file.SetAtt(KEntryAttReadOnly, KEntryAttNormal));
	CleanupStack::PopAndDestroy(2, store); // file, store

	// Now try and delete the file
	cEntry->SetEntryL(entry.Parent());
	cEntry->DeleteL(entry.Id());

	CleanupStack::PopAndDestroy(3); // cEntry, session, ob
	}

LOCAL_C void TestCleanupStackL()
	{
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvDraftEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;

	// Create an entry
	cEntry->CreateL(entry);

	// Make sure heap failure occurs in CleanupEntryPushL
	// If a failure occurs the entry should still be on cleanup stack
	__UHEAP_FAILNEXT(0);
	TRAPD(error, session->CleanupEntryPushL(entry.Id()); session->CleanupEntryPop(); );
	__UHEAP_RESET;

	// Check the entry has been deleted
	if (error == KErrNoMemory)
		{
		TRAP(error, session->GetEntryL(entry.Id()));
		test(error == KErrNotFound);
		}

	// Try again
	cEntry->CreateL(entry);

	// Make sure heap failure occurs in CleanupEntryPushL
	// If a failure occurs the entry should still be on cleanup stack
	__UHEAP_FAILNEXT(0);
	TRAP(error, session->CleanupEntryPushL(entry.Id()); session->CleanupEntryPop(); );
	__UHEAP_RESET;

	// Check the entry has been deleted
	if (error == KErrNoMemory)
		{
		TRAP(error, session->GetEntryL(entry.Id()));
		test(error == KErrNotFound);
		}

	CleanupStack::PopAndDestroy(3); // cEntry, session, ob
	}




//
//
// Bespoke test harness to verify defect ALR-5ABMSZ:
//
//   "Strange behaviour in message server when
//    closing sessions causes crash in mce app"
//
// Incorporates test code posted on Notes defects DB by Peter Harper in response to
// defect EXT-4VGD8D (same defect -- ALR-5ABMSZ is this defect propagated to
// Hurricane).
//
// If the test is successful, the child thread that's created will die gracefully
// and the main thread will be allowed to complete. In this case, the last line in the
// log file (\epoc32\wins\c\MsgLogs\ALR_5ABMSZ.WINS.DEB.LOG) will be "Success."
//
// To reproduce the defect, comment out the following line in
// CMsvServerSession::~CMsvServerSession() (located in \msg\msgssrc\MSVSESS.CPP, line
// 137 at time of writing (build 683)):
//
//     // Set request queued flag to false so we're not notified to close the session
//     iRequestQueued = EFalse;
//
// Now rebuild MSGS.DLL and run this test again. This time when the child thread dies
// we get a PanicClient called from the messaging server.
//
//

void DoThreadDieL()
    {
    // Set up a new session then kill the thread.
    // This is run in a separate thread from the main thread, called by DoDyingThreadStart

    // Install AS
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(scheduler);
    CleanupStack::PushL(scheduler);                                             // PUSH

    // Create dummy observer to pass to session
    CDummyObserver* ob = new (ELeave) CDummyObserver;
    CleanupStack::PushL(ob);                                                    // PUSH

    // Create session
    CMsvSession* session = CMsvSession::OpenSyncL(*ob);
    CleanupStack::PushL(session);                                               // PUSH

    // Wait for 5 seconds to allow thread to be observed
    User::After(5000000);

    // Now kill this thread
    RThread thread; // Get handle on this thread
    thread.Kill(KErrGeneral);

    CleanupStack::PopAndDestroy(3, scheduler); // session, ob, scheduler        // POP(2)
    }

static TInt DoDyingThreadStart(TAny*)
    {
    // Entry function for the child thread which is created then killed.

    CTrapCleanup* cleanup = CTrapCleanup::New();
    test(cleanup != NULL);
    TRAPD(ret, DoThreadDieL());
    delete cleanup;
    return 0;
    }

LOCAL_C void TestLastRealClientDeathL()
    {
    // Creates a new thread and runs it then waits a short while to see
    // if anything untoward happens.

    // Local constants
    const TInt KMinTestHeapSize = 0x10000;
    const TInt KMaxTestHeapSize = 0x100000;
    const TInt KPauseAfterThreadDeath = 5000000;
    _LIT(KThreadName, "Test thread");

    CDummyObserver* ob = new (ELeave) CDummyObserver;
    CleanupStack::PushL(ob);                                                    // PUSH

    CMsvSession* session = CMsvSession::OpenAsObserverL(*ob);
    CleanupStack::PushL(session);                                               // PUSH

    // Create the new thread that will be killed off
    RThread thread;
    TRequestStatus status;
    test(thread.Create(KThreadName, DoDyingThreadStart, KDefaultStackSize, KMinTestHeapSize, KMaxTestHeapSize, NULL, EOwnerThread) == KErrNone);
    thread.Logon(status);

    // Let the thread run
    thread.Resume();
    User::WaitForRequest(status);
    thread.Close();

    // Wait for 5 seconds to see if anything nasty happens.
    User::After(KPauseAfterThreadDeath);

    CleanupStack::PopAndDestroy(2, ob); // session, ob                          // POP(2)
    }



LOCAL_C void doTestsL()
	{
	CMsgsTestUtils::CleanMessageFolderL();

	// EDNSHAY-4KMSQW
	CMsgsTestUtils::Start(_L("Access violation in server"));
	TestAccessViolationL();
	CMsgsTestUtils::Complete();
	
	// EDNDCRS-4MBDAJ
	CMsgsTestUtils::Next(_L("Large TMsvEntry"));
	TestLargeEntryL();
	CMsgsTestUtils::Complete();

	// EDNSHAY-4QDQ8G
	CMsgsTestUtils::Next(_L("ReadOnly file deletion"));
	TestReadOnlyDeletionL();
	CMsgsTestUtils::Complete();

    // ALR-5ABMSZ
	CMsgsTestUtils::Next(_L("Death of last real client thread"));
	TestLastRealClientDeathL();
	CMsgsTestUtils::Complete();

	// EDNPHAR-4PKJAW
	CMsgsTestUtils::Next(_L("First use of cleanup stack"));
	TestCleanupStackL();
	CMsgsTestUtils::Complete();

	// EDNMJOI-4FWCHV
	CMsgsTestUtils::Next(_L("Ignoring notifications"));
	TestNotifyBug1L();
	TestNotifyBug2L();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Creating Mtm with invalid import-2"));
	TestClientMtmLoadPanicL();
	TestServerMtmLoadPanicL();
	CMsgsTestUtils::Complete();

	// EDNPHAR-4GLEYV
	CMsgsTestUtils::Next(_L("Starting without an index"));
	CMsgsTestUtils::Complete();

	// EDNPHAR-4GLEWK & EDNKPIA-4H2M92
	CMsgsTestUtils::Next(_L("Creating session whilst index loading"));
	CMsgsTestUtils::Complete();
	CMsgsTestUtils::CleanMessageFolderL();
	
	// EDNSDOT-4H6PK6
	CMsgsTestUtils::Next(_L("Changing sending state"));
	TestSetSendingStateAttributeL();
	CMsgsTestUtils::Complete();

	// EDNPHAR-4H3FPZ
	CMsgsTestUtils::Next(_L("Creating Mtm with invalid import"));
	TestUiMtmLoadPanicL();
	TestUiDataLoadPanicL();
	CMsgsTestUtils::Complete();
}
