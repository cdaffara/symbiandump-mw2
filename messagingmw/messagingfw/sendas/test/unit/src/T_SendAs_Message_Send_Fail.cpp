// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "sendas2test.h"
#include <mmsvattachmentmanager.h>

RTest test(_L("SendAs2 Message Send Fail."));
RSendAs gSendAs;
CSendAsMessageTypes* gMessageTypes;
TUid sendAsTestMtmUid;

_LIT(KAddress1, "test1@symbian.com");
_LIT(KAddress3, "test3@symbian.com");
_LIT(KAddress4, "test4@symbian.com");
_LIT(KAlias1, "alias1");
_LIT(KAlias2, "alias2");

class CSentEntryObserver : public CBase, public MMsvEntryObserver
	{
public:
	CSentEntryObserver(TRequestStatus* aStatus) : iStatus(aStatus) {(*iStatus) = KRequestPending;}
	void HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
private:
	TRequestStatus* iStatus;
	};

void CSentEntryObserver::HandleEntryEventL(TMsvEntryEvent aEvent, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	if (aEvent == EMsvNewChildren)
		{
		User::RequestComplete(iStatus, KErrNone);
		}
	}

LOCAL_C void CleanMailFolder(TMsvId aFolderId)
	{
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, aFolderId, 
		TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(cEntry);
	
	CMsvEntrySelection* selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);

	TMsvLocalOperationProgress progress;
	if (selection->Count() > 0)
		{
		cEntry->DeleteL(*selection, progress);
		}

	CleanupStack::PopAndDestroy(4); // selection, cEntry, session, ob1
	}

void InitL()
	{
	gMessageTypes = CSendAsMessageTypes::NewL();
	CleanupStack::PushL(gMessageTypes);
	CleanMailFolder(KMsvDraftEntryId);

	TRAPD(err, gSendAs.Connect());
	test(err == KErrNone);
	
	gMessageTypes->Reset();
	gSendAs.FilteredMessageTypesL(*gMessageTypes);
	test((sendAsTestMtmUid = gMessageTypes->UidFromNameL(_L("SendAs Test Mtm Client"))) != KNullUid);
	}

void CleanupL()
	{
	gSendAs.Close();
	CleanupStack::PopAndDestroy(gMessageTypes); 
	}

void CreateSendMessageL(RSendAsMessage& aMessage)
	{
	aMessage.CreateL(gSendAs, sendAsTestMtmUid);
	aMessage.SetSubjectL(_L("async send test subject"));
	aMessage.AddRecipientL(KAddress1, KAlias1, RSendAsMessage::ESendAsRecipientTo);
	aMessage.AddRecipientL(KAddress3, RSendAsMessage::ESendAsRecipientCc);
	aMessage.AddRecipientL(KAddress4, KAlias2, RSendAsMessage::ESendAsRecipientCc);
	TUid bioType = {0xff};
	aMessage.SetBioTypeL(bioType);
	}
	
// Marks entry #0 in drafts folder as errored
void MarkMessageErroredL()
	{
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvDraftEntryId, 
		TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(cEntry);

	CMsvEntrySelection* selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);

	// ensure only one entry in drafts folder
	// the test sendas notifier will therefor reject sending it
	test(selection->Count() == 1);
	CMsvEntry* cEntry2 = session->GetEntryL(selection->At(0));
	CleanupStack::PushL(cEntry2);

	TMsvEntry entry = cEntry2->Entry();
	entry.iError = KErrPermissionDenied;
	cEntry2->ChangeL(entry);
	
	CleanupStack::PopAndDestroy(5, ob1); // cEntry2, selection, cEntry, session, ob1
	}


void CheckSentFolderEmptyL()
	{
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvSentEntryId, 
		TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(cEntry);
	
	CMsvEntrySelection* selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);

	test(selection->Count() == 0);

	CleanupStack::PopAndDestroy(4); // selection, cEntry, session, ob1
	}

void CheckSentMessageL()
	{
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvSentEntryId, 
		TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(cEntry);
	
	CMsvEntrySelection* selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);

	test(selection->Count() == 1);
	cEntry->SetEntryL((*selection)[0]);
	test(cEntry->Entry().iMtm == sendAsTestMtmUid);

	CleanupStack::PopAndDestroy(4); // selection, cEntry, session, ob1
	}

void TestSendMessageL()
	{
	theUtils->Start(_L("Message Async Send"));
	CleanMailFolder(KMsvDraftEntryId);
	CleanMailFolder(KMsvSentEntryId);

	// Create a message.
	RSendAsMessage message;	
	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);
	MarkMessageErroredL();

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);

	theUtils->WriteComment(_L("Send the created message."));
	message.SendMessage(testActive->iStatus);
	testActive->StartL();
	CActiveScheduler::Start();

	TSendAsProgress progress;
	message.ProgressL(progress);
	test(progress.iError == KErrPermissionDenied);

	theUtils->WriteComment(_L("Check message was not sent."));
	CheckSentFolderEmptyL();

	CleanupStack::PopAndDestroy(testActive);
	theUtils->Complete();
	}

void TestSendMessageAndCloseL()
	{
	theUtils->Start(_L("Message Send And Close"));
	CleanMailFolder(KMsvDraftEntryId);
	CleanMailFolder(KMsvSentEntryId);

	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvSentEntryId, 
		TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(cEntry);

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);

	CSentEntryObserver* sentObserver = new(ELeave) CSentEntryObserver(&(testActive->iStatus));
	CleanupStack::PushL(sentObserver);
	cEntry->AddObserverL(*sentObserver);

	// Create a message.
	RSendAsMessage message;	
	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);
	
	MarkMessageErroredL();
	
	theUtils->WriteComment(_L("Send and close the created message."));
	message.SendMessageAndCloseL();
	
	// Allow user to deny message send
	User::After(1000000);
	
	// Cannot check progress as handle to session has closed - check send folder is empty
	theUtils->WriteComment(_L("Check message was not sent."));
	CheckSentFolderEmptyL();

	CleanupStack::PopAndDestroy(5, ob1); // sentObserver, testActive, cEntry, session, ob1

	theUtils->Complete();
	}

void TestSendMessageConfirmedL()
	{
	theUtils->Start(_L("Message Confirmed Async Send"));
	CleanMailFolder(KMsvDraftEntryId);
	CleanMailFolder(KMsvSentEntryId);

	// Create a message.
	RSendAsMessage message;	
	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);

	theUtils->WriteComment(_L("Send the created message."));
	message.SendMessageConfirmed(testActive->iStatus);
	testActive->StartL();
	CActiveScheduler::Start();
	
	test(testActive->iStatus.Int() == KErrNone);

	theUtils->WriteComment(_L("Check message was sent despite not having LocalServices capability."));
	CheckSentMessageL();

	CleanupStack::PopAndDestroy(testActive);
	theUtils->Complete();
	}

void TestSendMessageConfirmedAndCloseL()
	{
	theUtils->Start(_L("Message Send Confirmed And Close"));
	CleanMailFolder(KMsvDraftEntryId);
	CleanMailFolder(KMsvSentEntryId);

	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvSentEntryId, 
		TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(cEntry);

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);

	CSentEntryObserver* sentObserver = new(ELeave) CSentEntryObserver(&(testActive->iStatus));
	CleanupStack::PushL(sentObserver);
	cEntry->AddObserverL(*sentObserver);

	// Create a message.
	RSendAsMessage message;	
	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);

	theUtils->WriteComment(_L("Send confirmed and close the created message."));
	message.SendMessageConfirmedAndCloseL();

	testActive->StartL();
	CActiveScheduler::Start();
	test(testActive->iStatus.Int() == KErrNone);

	theUtils->WriteComment(_L("Check message was sent despite not having LocalServices capability."));
	CheckSentMessageL();

	CleanupStack::PopAndDestroy(5, ob1); // sentObserver, testActive, cEntry, session, ob1

	theUtils->Complete();
	}

void doTestsL()
	{
	InitL();			//Qualified: no leavescan error

	TestSendMessageL();	//Qualified: no leavescan error
	TestSendMessageAndCloseL();				//Qualified: no leavescan error
	TestSendMessageConfirmedL();			//Qualified: no leavescan error
	TestSendMessageConfirmedAndCloseL();	//Qualified: no leavescan error

	CleanupL();			//Qualified: no leavescan error
	}
