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

RTest test(_L("SendAs2 Message Progress."));
RSendAs gSendAs;
CSendAsMessageTypes* gMessageTypes;
TUid sendAsTestMtmUid;

// -----------------------------------------------------------------------------

_LIT(KAddress1, "test1@symbian.com");
_LIT(KAddress3, "test3@symbian.com");
_LIT(KAddress4, "test4@symbian.com");
_LIT(KAlias1, "alias1");
_LIT(KAlias2, "alias2");

// -----------------------------------------------------------------------------

class CTestProgressActive : public CActive
	{
public:
	CTestProgressActive(RSendAsMessage& aMessage);
	~CTestProgressActive();
	void StartL();
protected:
	void DoCancel();
	void RunL();
public:
	TSendAsProgress iProgress;
private:
	RSendAsMessage& iMessage;
	};

// -----------------------------------------------------------------------------

CTestProgressActive::CTestProgressActive(RSendAsMessage& aMessage)
: CActive(0), iMessage(aMessage)
	{
	CActiveScheduler::Add(this);
	}

// -----------------------------------------------------------------------------

CTestProgressActive::~CTestProgressActive()
	{
	Cancel();
	}

// -----------------------------------------------------------------------------

void CTestProgressActive::DoCancel()
	{
	TRequestStatus* s=&iStatus;
	User::RequestComplete(s, KErrNone);
	// Cancel the message
	iMessage.Cancel();
	}

// -----------------------------------------------------------------------------

void CTestProgressActive::StartL()
	{
	SetActive();
	}

// -----------------------------------------------------------------------------

void CTestProgressActive::RunL() 
	{
	iMessage.ProgressL(iProgress); // Get the message's progress
	}

// -----------------------------------------------------------------------------

class CSentEntryObserver : public CBase, public MMsvEntryObserver
	{
public:
	CSentEntryObserver(TRequestStatus& aStatus) : iStatus(&aStatus) {aStatus = KRequestPending;}
	void HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
private:
	TRequestStatus* iStatus;
	};

// -----------------------------------------------------------------------------

void CSentEntryObserver::HandleEntryEventL(TMsvEntryEvent aEvent, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	if (aEvent == EMsvNewChildren)
		{
		User::RequestComplete(iStatus, KErrNone);
		}
	}

// -----------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------

void CleanupL()
	{
	gSendAs.Close();
	CleanupStack::PopAndDestroy(gMessageTypes); 
	}

// -----------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------

void TestMessagePreCreateL()
	{
	theUtils->Start(_L("Message Pre-Create Progress"));
	CleanMailFolder(KMsvDraftEntryId);

	// Create a message.
	RSendAsMessage message;
	CleanupClosePushL(message);

	TSendAsProgress progress;
	theUtils->WriteComment(_L("Get progress on uncreated message."));
	TRAPD(err, message.ProgressL(progress));

	theUtils->WriteComment(_L("Check call aborted."));
	test(err == KErrAbort);

	CleanupStack::PopAndDestroy(&message);

	theUtils->Complete();
	}

// -----------------------------------------------------------------------------

void TestMessageCreatedL()
	{
	theUtils->Start(_L("Created Message Progress"));
	CleanMailFolder(KMsvDraftEntryId);

	// Create a message.
	RSendAsMessage message;
	CleanupClosePushL(message);

	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);

	TSendAsProgress progress;
	theUtils->WriteComment(_L("Get progress on message."));
	message.ProgressL(progress);

	theUtils->WriteComment(_L("Check progress."));
	test(progress.iState == CMsvSendOperation::ESendStateInPreparation);
	test(progress.iError == KErrNone);
	test(progress.iProgressMax == 0);
	test(progress.iProgress == 0);

	CleanupStack::PopAndDestroy(&message);

	theUtils->Complete();
	}

// -----------------------------------------------------------------------------

void TestMessageDeletedL()
	{
	theUtils->Start(_L("Deleted Message Progress"));
	CleanMailFolder(KMsvDraftEntryId);

	// Create a message.
	RSendAsMessage message;

	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);

	theUtils->WriteComment(_L("Delete the created message."));
	message.Close();

	TSendAsProgress progress;
	theUtils->WriteComment(_L("Get progress on message."));
	TRAPD(err, message.ProgressL(progress));

	theUtils->WriteComment(_L("Check call aborted."));
	test(err == KErrAbort);

	theUtils->Complete();
	}

// -----------------------------------------------------------------------------

void TestMessageAddingAttachmentL()
	{
	RFs fs;
	RFile attachment;
	_LIT(KAttachmentName, "c:\\t_sendas2_attachment.txt");
	_LIT8(KContent, "This is a test file. Delete me.");

	theUtils->Start(_L("Message Adding Attachment Progress"));
	CleanMailFolder(KMsvDraftEntryId);
	
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	User::LeaveIfError(fs.ShareProtected());

	// Clean up any existing attachment file.
	TInt err = fs.Delete(KAttachmentName);
	if (err != KErrNotFound)
		{
		User::LeaveIfError(err);
		}

	// Create a new attachment file
	theUtils->WriteComment(_L("Create an attachment file."));
	User::LeaveIfError(attachment.Create(fs, KAttachmentName, EFileWrite));
	User::LeaveIfError(attachment.Write(KContent));

	// Create a message.
	RSendAsMessage message;
	CleanupClosePushL(message);

	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);

	theUtils->WriteComment(_L("Add the created attachment file."));
	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);
	message.AddAttachment(attachment, testActive->iStatus);
	attachment.Close();
	testActive->StartL();
	CActiveScheduler::Start();

	TSendAsProgress progress;
	theUtils->WriteComment(_L("Get progress on message."));
	message.ProgressL(progress);

	theUtils->WriteComment(_L("Check progress."));
	test(progress.iState == CMsvSendOperation::ESendStateInPreparation);
	test(progress.iError == KErrNone);
	test(progress.iProgressMax == 0);
	test(progress.iProgress == 0);	
	
	// Clean up existing attachment file.
	err = fs.Delete(KAttachmentName);
	if (err != KErrNotFound)
		{
		User::LeaveIfError(err);
		}

	CleanupStack::PopAndDestroy(3, &fs); // testActive, message, fs

	theUtils->Complete();
	}

// -----------------------------------------------------------------------------

void TestMessageStartSendingL()
	{
	theUtils->Start(_L("Message Start Sending Progress"));
	CleanMailFolder(KMsvDraftEntryId);
	CleanMailFolder(KMsvSentEntryId);

	// Create a message.
	RSendAsMessage message;
	CleanupClosePushL(message);

	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);

	theUtils->WriteComment(_L("Begin sending the created message."));
	message.SendMessage(testActive->iStatus);

	TSendAsProgress progress;
	theUtils->WriteComment(_L("Get progress on message."));
	message.ProgressL(progress);

	theUtils->WriteComment(_L("Check progress."));
	test(progress.iState == CMsvSendOperation::ESendStateWaitingToSend);

	testActive->StartL();
	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(2, &message); // testActive, message
	theUtils->Complete();
	}

// -----------------------------------------------------------------------------

void TestMessageDuringSendingL()
	{
	theUtils->Start(_L("Message During Sending Progress"));
	CleanMailFolder(KMsvDraftEntryId);
	CleanMailFolder(KMsvSentEntryId);

	// Create a message.
	RSendAsMessage message;
	CleanupClosePushL(message);

	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);
	CTestProgressActive* testProgress = new(ELeave) CTestProgressActive(message);
	CleanupStack::PushL(testProgress);
	CSentEntryObserver* sentObserver = new(ELeave) CSentEntryObserver(testProgress->iStatus);
	CleanupStack::PushL(sentObserver);

	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvGlobalOutBoxIndexEntryId, 
		TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(cEntry);
	cEntry->AddObserverL(*sentObserver);

	theUtils->WriteComment(_L("Begin sending the created message."));
	message.SendMessage(testActive->iStatus);

	theUtils->WriteComment(_L("Get progress on message."));
	testActive->StartL();
	testProgress->StartL();
	CActiveScheduler::Start();

	theUtils->WriteComment(_L("Check progress."));
	test(testProgress->iProgress.iState == CMsvSendOperation::ESendStateWaitingToSend);
	test(testProgress->iProgress.iProgressMax == 0);
	test(testProgress->iProgress.iError == 0);

	CleanupStack::PopAndDestroy(7, &message); // cEntry, session, ob1, sendObserver, testProgress, testActive, message
	theUtils->Complete();
	}

// -----------------------------------------------------------------------------

void TestMessageSentL()
	{
	theUtils->Start(_L("Message Sent Progress"));
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

	TSendAsProgress progress;
	theUtils->WriteComment(_L("Get progress on message."));
	message.ProgressL(progress);

	theUtils->WriteComment(_L("Check progress."));
	test(progress.iState == CMsvSendOperation::ESendStateDone);

	CleanupStack::PopAndDestroy(2, &message); // testActive, message
	theUtils->Complete();
	}

// -----------------------------------------------------------------------------

void doTestsL()
	{
	InitL();					//Qualified: no leavescan error

	TestMessagePreCreateL();	//Qualified: no leavescan error
	TestMessageCreatedL();		//Qualified: no leavescan error
	TestMessageDeletedL();		//Qualified: no leavescan error
	TestMessageAddingAttachmentL();	//Qualified: no leavescan error
	TestMessageStartSendingL();		//Qualified: no leavescan error
	TestMessageDuringSendingL();	//Qualified: no leavescan error
	TestMessageSentL();			//Qualified: no leavescan error

	CleanupL();					//Qualified: no leavescan error
	}

// -----------------------------------------------------------------------------

