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

RTest test(_L("SendAs2 Message Send."));
RSendAs gSendAs;
CSendAsMessageTypes* gMessageTypes;
TUid sendAsTestMtmUid;

_LIT(KSendAsServerPattern, "!SendAsServer*");

_LIT(KAddress1, "test1@symbian.com");
_LIT(KAddress3, "test3@symbian.com");
_LIT(KAddress4, "test4@symbian.com");
_LIT(KAlias1, "alias1");
_LIT(KAlias2, "alias2");

class CSentEntryObserver : public CBase, public MMsvEntryObserver
	{
public:
	CSentEntryObserver(TRequestStatus& aStatus) : iStatus(&aStatus) {aStatus = KRequestPending;}
	void HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
private:
	TRequestStatus* iStatus;
	};

void CSentEntryObserver::HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	if ((aEvent == EMsvNewChildren) || (aEvent == EMsvChildrenChanged))
		{
		if (aEvent != EMsvChildrenChanged)
			{
			User::RequestComplete(iStatus, KErrNone);
			}
		else
			{
			CMsvEntrySelection* selection = (CMsvEntrySelection*)aArg1;
			CDummyObserver* ob1 = new(ELeave) CDummyObserver;
			CleanupStack::PushL(ob1);

			CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
			CleanupStack::PushL(session);
	
			CMsvEntry* cEntry = CMsvEntry::NewL(*session, (*selection)[0], 
				TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
			CleanupStack::PushL(cEntry);
			if (cEntry->Entry().MtmData3() == 1234567890)
				{
				User::RequestComplete(iStatus, KErrNone);
				}
			CleanupStack::PopAndDestroy(3, ob1); // cEntry, session, ob1
			}
		}
	}

class CEditEntryObserver : public CBase, public MMsvEntryObserver
	{
public:
	CEditEntryObserver(TRequestStatus& aStatus) : iStatus(&aStatus) {aStatus = KRequestPending;}
	void HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	TBool iCompleted;
	private:
	TRequestStatus* iStatus;
	};

void CEditEntryObserver::HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	if (aEvent == EMsvChildrenChanged)
		{
		CMsvEntrySelection* selection = (CMsvEntrySelection*)aArg1;
		CDummyObserver* ob1 = new(ELeave) CDummyObserver;
		CleanupStack::PushL(ob1);

		CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
		CleanupStack::PushL(session);

		CMsvEntry* cEntry = CMsvEntry::NewL(*session, (*selection)[0], 
			TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
		CleanupStack::PushL(cEntry);
		if (!iCompleted && cEntry->Entry().MtmData3() == 234567890)
			{
			iCompleted = ETrue;
			User::RequestComplete(iStatus, KErrNone);
			}
		CleanupStack::PopAndDestroy(3, ob1); // cEntry, session, ob1
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

	TInt err = gSendAs.Connect();
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
	

void WaitForSendAsServerClose()
	{
	FOREVER
		{
		TFullName name;
		TFindProcess find(KSendAsServerPattern);
		if (find.Next(name) != KErrNone)
			break;
		User::After(100000);
		}
	}

void TestSendMessageL()
	{
	theUtils->Start(_L("Message Async Send"));
	CleanMailFolder(KMsvDraftEntryId);
	CleanMailFolder(KMsvSentEntryId);

	// Create a message.
	RSendAsMessage message;
	CleanupClosePushL(message);

	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);

	theUtils->WriteComment(_L("Send the created message."));
	message.SendMessage(testActive->iStatus);
	testActive->StartL();
	CActiveScheduler::Start();
	test(testActive->iStatus.Int() == KErrNone);

	theUtils->WriteComment(_L("Check message was sent."));
	CheckSentMessageL();

	CleanupStack::PopAndDestroy(2, &message); // testActive, message
	theUtils->Complete();
	}

void TestSendMessageAndCloseL()
	{
	theUtils->Start(_L("Message SendAndClose"));
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

	CSentEntryObserver* sentObserver = new(ELeave) CSentEntryObserver(testActive->iStatus);
	CleanupStack::PushL(sentObserver);
	cEntry->AddObserverL(*sentObserver);

	// Create a message.
	RSendAsMessage message;
	CleanupClosePushL(message);

	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);

	theUtils->WriteComment(_L("Send and close the created message."));
	message.SendMessageAndCloseL();
	CleanupStack::Pop(&message);

	testActive->StartL();
	CActiveScheduler::Start();
	test(testActive->iStatus.Int() == KErrNone);

	theUtils->WriteComment(_L("Check message was sent."));
	CheckSentMessageL();

	CleanupStack::PopAndDestroy(5, ob1); // sentObserver, testActive, cEntry, session, ob1

	theUtils->Complete();
	}
	
void TestSendMessageAndCheckMessageEntriesL()
	{
	theUtils->Start(_L("Message Send And Checking Entry"));
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

	CSentEntryObserver* sentObserver = new(ELeave) CSentEntryObserver(testActive->iStatus);
	CleanupStack::PushL(sentObserver);
	cEntry->AddObserverL(*sentObserver);

	// Create a message.
	RSendAsMessage message;
	CleanupClosePushL(message);

	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);

	theUtils->WriteComment(_L("Send and close the created message."));
	message.SendMessageAndCloseL();
	CleanupStack::Pop(&message);

	testActive->StartL();
	
	gSendAs.Close();
	
	CActiveScheduler::Start();
	test(testActive->iStatus.Int() == KErrNone);

	theUtils->WriteComment(_L("Check message entries."));
	test(cEntry->Entry().Visible());
    test(cEntry->Entry().InPreparation()==EFalse);


	CleanupStack::PopAndDestroy(5, ob1); // sentObserver, testActive, cEntry, session, ob1

	// Re-establish global RSendAs connection for other tests.
	test(gSendAs.Connect() == KErrNone);
	theUtils->Complete();
	}

void TestSendMessageAndCloseAndCloseSessionL()
	{
	theUtils->Start(_L("Message SendAndClose and close session"));
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

	CSentEntryObserver* sentObserver = new(ELeave) CSentEntryObserver(testActive->iStatus);
	CleanupStack::PushL(sentObserver);
	cEntry->AddObserverL(*sentObserver);

	// Create a message.
	RSendAsMessage message;
	CleanupClosePushL(message);

	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);

	theUtils->WriteComment(_L("Send and close the created message."));
	message.SendMessageAndCloseL();
	CleanupStack::Pop(&message);

	testActive->StartL();
	
	gSendAs.Close();
	
	CActiveScheduler::Start();
	test(testActive->iStatus.Int() == KErrNone);

	theUtils->WriteComment(_L("Check message was sent."));
	CheckSentMessageL();

	CleanupStack::PopAndDestroy(5, ob1); // sentObserver, testActive, cEntry, session, ob1

	// Re-establish global RSendAs connection for other tests.
	test(gSendAs.Connect() == KErrNone);
	theUtils->Complete();
	}

void TestSendMessageConfirmedL()
	{
	theUtils->Start(_L("Message Confirmed Async Send"));
	CleanMailFolder(KMsvDraftEntryId);
	CleanMailFolder(KMsvSentEntryId);

	// Create a message.
	RSendAsMessage message;	
	CleanupClosePushL(message);
	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);

	theUtils->WriteComment(_L("Send the created message."));
	message.SendMessageConfirmed(testActive->iStatus);
	testActive->StartL();
	CActiveScheduler::Start();
	
	test(testActive->iStatus.Int() == KErrNone);

	theUtils->WriteComment(_L("Check message was sent."));
	CheckSentMessageL();

	CleanupStack::PopAndDestroy(2, &message); // testActive, message
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

	CSentEntryObserver* sentObserver = new(ELeave) CSentEntryObserver(testActive->iStatus);
	CleanupStack::PushL(sentObserver);
	cEntry->AddObserverL(*sentObserver);

	// Create a message.
	RSendAsMessage message;	
	
	CleanupClosePushL(message);
	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);

	theUtils->WriteComment(_L("Send confirmed and close the created message."));
	message.SendMessageConfirmedAndCloseL();
	CleanupStack::Pop(&message);

	testActive->StartL();
	CActiveScheduler::Start();
	test(testActive->iStatus.Int() == KErrNone);

	theUtils->WriteComment(_L("Check message was sent."));
	CheckSentMessageL();

	CleanupStack::PopAndDestroy(5, ob1); // sentObserver, testActive, cEntry, session, ob1

	theUtils->Complete();
	}

void TestLaunchEditorAndCloseL()
	{
	theUtils->Start(_L("Message Launch Editor And Close"));
	CleanMailFolder(KMsvDraftEntryId);
	CleanMailFolder(KMsvSentEntryId);

	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvDraftEntryId, 
		TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(cEntry);
	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);
	CEditEntryObserver* editObserver = new(ELeave) CEditEntryObserver(testActive->iStatus);
	CleanupStack::PushL(editObserver);
	editObserver->iCompleted = EFalse;

	// Create a message.
	RSendAsMessage message;	
	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);
	CleanupClosePushL(message);

	cEntry->AddObserverL(*editObserver);
	
	theUtils->WriteComment(_L("Launch the editor for the message."));
	message.LaunchEditorAndCloseL();
	CleanupStack::Pop(&message);

	testActive->StartL();
	CActiveScheduler::Start();

	// close send-as session 
	User::After(1*1000*1000);

	// close send-as session (before editor has completed)
	gSendAs.Close();

	// wait for the sendas server to close (when edit utils signal completion)
	WaitForSendAsServerClose();
	
	theUtils->WriteComment(_L("Check the editor was launched."));
	test(testActive->iStatus.Int() == KErrNone);

	CMsvEntrySelection* selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);

	// Move the CMsvEntry to the new message entry
	cEntry->SetEntryL((*selection)[0]);
	// test data written by test edit tools.
	test(cEntry->Entry().MtmData3() == 234567890);

	CleanupStack::PopAndDestroy(6, ob1); // selection, editObserver, testActive, cEntry, session, ob1

	// Re-establish global RSendAs connection for other tests.
	test(gSendAs.Connect() == KErrNone);
	theUtils->Complete();
	}

void doTestsL()
	{
	InitL();			//Qualified: no leavescan error

	TestSendMessageL();	//Qualified: no leavescan error
	TestSendMessageAndCloseL();	//Qualified: no leavescan error
	TestSendMessageAndCheckMessageEntriesL();	//Qualified: no leavescan error
	TestSendMessageAndCloseAndCloseSessionL();	//Qualified: no leavescan error
	TestSendMessageConfirmedL();				//Qualified: no leavescan error
	TestSendMessageConfirmedAndCloseL();		//Qualified: no leavescan error
	TestLaunchEditorAndCloseL();	//Qualified: no leavescan error

	CleanupL();			//Qualified: no leavescan error
	}
