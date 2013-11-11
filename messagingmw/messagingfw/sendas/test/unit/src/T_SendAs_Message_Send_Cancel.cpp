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

RTest test(_L("SendAs2 Message Send Cancel."));
RSendAs gSendAs;
CSendAsMessageTypes* gMessageTypes;
TUid sendAsTestMtmUid;

_LIT8(KAttachmentContent, "content!");
_LIT8(KMimeType, "text/html");
_LIT(KExistingAttachmentName, "c:\\t_sendas2_send_a.txt");
_LIT(KAddress1, "test1@symbian.com");
_LIT(KAddress3, "test3@symbian.com");
_LIT(KAddress4, "test4@symbian.com");
_LIT(KAlias1, "alias1");
_LIT(KAlias2, "alias2");

typedef enum 
	{
	EByName,
	EByNameWithMimeType,
	EByFile,
	EByFileWithMimeType,
	ELinked,
	ELinkedWithMimeType
	} TAttachmentTestType;

// -----------------------------------------------------------------------------

class CTestCancelMessageActive : public CActive
	{
public:
	CTestCancelMessageActive(RSendAsMessage& aMessage);
	~CTestCancelMessageActive();
	void StartL();
protected:
	void DoCancel();
	void RunL();
private:
	RSendAsMessage& iMessage;
	};

// -----------------------------------------------------------------------------

CTestCancelMessageActive::CTestCancelMessageActive(RSendAsMessage& aMessage)
: CActive(0), iMessage(aMessage)
	{
	iStatus = KRequestPending;
	CActiveScheduler::Add(this);
	}

CTestCancelMessageActive::~CTestCancelMessageActive()
	{
	Cancel();
	}

void CTestCancelMessageActive::DoCancel()
	{
	TRequestStatus* s=&iStatus;
	User::RequestComplete(s, KErrNone);
	// Cancel the message
	iMessage.Cancel();
	}

void CTestCancelMessageActive::StartL()
	{
	SetActive();
	// Just complete
	TRequestStatus* s=&iStatus;
	User::RequestComplete(s, KErrNone);
	}

void CTestCancelMessageActive::RunL() 
	{
	// Cancel the message
	iMessage.Cancel();
	}
// -----------------------------------------------------------------------------

class CTestCloseSessionActive : public CActive
	{
public:
	IMPORT_C CTestCloseSessionActive(RSendAs& aSendAs);
	IMPORT_C ~CTestCloseSessionActive();
	IMPORT_C void StartL();
protected:
	IMPORT_C void DoCancel();
	IMPORT_C void RunL();
private:
	RSendAs& iSendAs;
	};

// -----------------------------------------------------------------------------

CTestCloseSessionActive::CTestCloseSessionActive(RSendAs& aSendAs)
: CActive(0), iSendAs(aSendAs)
	{
	iStatus = KRequestPending;
	CActiveScheduler::Add(this);
	}

CTestCloseSessionActive::~CTestCloseSessionActive()
	{
	Cancel();
	}

void CTestCloseSessionActive::DoCancel()
	{
	TRequestStatus* s=&iStatus;
	User::RequestComplete(s, KErrNone);
	// Cancel the message
	iSendAs.Close();
	}

void CTestCloseSessionActive::StartL()
	{
	SetActive();
	// Just complete
	TRequestStatus* s=&iStatus;
	User::RequestComplete(s, KErrNone);
	}

void CTestCloseSessionActive::RunL() 
	{
	// Cancel the message
	iSendAs.Close();
	CActiveScheduler::Stop();
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
	CleanupStack::PopAndDestroy(gMessageTypes); // gMessageTypes
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

void CheckNoAttachmentsL()
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

	test(selection->Count() == 1);
	cEntry->SetEntryL((*selection)[0]);

	if (cEntry->HasStoreL())
		{
		CMsvStore* store = cEntry->ReadStoreL();
		CleanupStack::PushL(store);
	
		MMsvAttachmentManager& attachmentMgr = store->AttachmentManagerL();
		test(attachmentMgr.AttachmentCount() == 0);
		CleanupStack::PopAndDestroy(store);
		}

	CleanupStack::PopAndDestroy(4, ob1); // selection, cEntry, session, ob1
	}

// -----------------------------------------------------------------------------

void TestSendMessageCancelL()
	{
	theUtils->Start(_L("Cancel Message Async Send"));
	CleanMailFolder(KMsvDraftEntryId);
	CleanMailFolder(KMsvSentEntryId);

	// Create a message.
	RSendAsMessage message;	
	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);

	theUtils->WriteComment(_L("Send the created message."));
	message.SendMessage(testActive->iStatus);
	testActive->StartL();

	// Create the object to cancel the message send.
	CTestCancelMessageActive* cancelMessage = new(ELeave) CTestCancelMessageActive(message);
	CleanupStack::PushL(cancelMessage);
	cancelMessage->StartL();

	CActiveScheduler::Start();

	TSendAsProgress progress;
	message.ProgressL(progress);
	theUtils->WriteComment(_L("Check message was cancelled."));
	test(progress.iError == KErrCancel);

	CleanupStack::PopAndDestroy(2, testActive); // cancelMessage, testActive
	theUtils->Complete();
	}

// -----------------------------------------------------------------------------

void TestSendMessageConfirmedCancelL()
	{
	theUtils->Start(_L("Cancel Message Confirmed Async Send"));
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

	// Create the object to cancel the message send.
	CTestCancelMessageActive* cancelMessage = new(ELeave) CTestCancelMessageActive(message);
	CleanupStack::PushL(cancelMessage);
	cancelMessage->StartL();

	CActiveScheduler::Start();
	
	TSendAsProgress progress;
	message.ProgressL(progress);
	theUtils->WriteComment(_L("Check message was cancelled."));
	test(progress.iError == KErrCancel);

	CleanupStack::PopAndDestroy(2, testActive); // cancelMessage, testActive
	theUtils->Complete();
	}

// -----------------------------------------------------------------------------

void TestAddAttachmentCancelL(TAttachmentTestType aTest)
	{
	RFs fs;
	RFile existingAttachment;

	CleanMailFolder(KMsvDraftEntryId);
	
	CleanupClosePushL(fs);
	CleanupClosePushL(existingAttachment);
	
	User::LeaveIfError(fs.Connect());
	User::LeaveIfError(fs.ShareProtected());
	// Clean up any existing attachment file.
	TInt err = fs.Delete(KExistingAttachmentName);
	if (err != KErrNotFound)
		{
		User::LeaveIfError(err);
		}

	// Create a new attachment file
	User::LeaveIfError(existingAttachment.Create(fs, KExistingAttachmentName, EFileWrite));
	User::LeaveIfError(existingAttachment.Write(KAttachmentContent));

	// Create a message.
	RSendAsMessage message;
	
	CleanupClosePushL(message);
	message.CreateL(gSendAs, sendAsTestMtmUid);

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);

	switch (aTest)
		{
		case EByName:
			{
			theUtils->Start(_L("Cancel Message Add Attachment By Name"));
			theUtils->WriteComment(_L("Add the created attachment file."));
			message.AddAttachment(KExistingAttachmentName, testActive->iStatus);
			break;
			}
		case EByNameWithMimeType:
			{
			theUtils->Start(_L("Cancel Message Add Attachment By Name With Mime Type"));
			theUtils->WriteComment(_L("Add the created attachment file."));
			message.AddAttachment(KExistingAttachmentName, KMimeType, testActive->iStatus);
			break;
			}
		case EByFile:
			{
			theUtils->Start(_L("Cancel Message Add Attachment By File"));
			theUtils->WriteComment(_L("Add the created attachment file."));
			message.AddAttachment(existingAttachment, testActive->iStatus);
			break;
			}

		case EByFileWithMimeType:
			{
			theUtils->Start(_L("Cancel Message Add Attachment By File With Mime Type"));
			theUtils->WriteComment(_L("Add the created attachment file."));
			message.AddAttachment(existingAttachment, KMimeType, testActive->iStatus);
			break;
			}
		case ELinked:
			{
			theUtils->Start(_L("Cancel Message Add Linked Attachment"));
			theUtils->WriteComment(_L("Add the created attachment file."));
			message.AddLinkedAttachment(KExistingAttachmentName, testActive->iStatus);
			break;
			}
		case ELinkedWithMimeType:
			{
			theUtils->Start(_L("Cancel Message Add Linked Attachment With Mime Type"));
			theUtils->WriteComment(_L("Add the created attachment file."));
			message.AddLinkedAttachment(KExistingAttachmentName, KMimeType, testActive->iStatus);
			break;
			}
		}
	testActive->StartL();
	
	CTestCancelMessageActive* cancelActive = new(ELeave) CTestCancelMessageActive(message);
	CleanupStack::PushL(cancelActive);
	cancelActive->StartL();

	CActiveScheduler::Start();

	theUtils->WriteComment(_L("Check message has no attachments."));
	CheckNoAttachmentsL();

	CleanupStack::PopAndDestroy(5, &fs); // cancelActive, testActive, message, existingAttachment fs
	theUtils->Complete();
	}

// -----------------------------------------------------------------------------

void TestSendMessageAndSessionCloseL()
	{
	theUtils->Start(_L("Message Async Send And Session Close"));
	CleanMailFolder(KMsvDraftEntryId);
	CleanMailFolder(KMsvSentEntryId);

	// Create a message.
	RSendAsMessage message;	
	theUtils->WriteComment(_L("Create a message."));
	CreateSendMessageL(message);

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);

	theUtils->WriteComment(_L("Send the created message."));
	message.SendMessage(testActive->iStatus);
	testActive->StartL();

	// Create the object to close send-as session.
	CTestCloseSessionActive* closeSession = new(ELeave) CTestCloseSessionActive(gSendAs);
	CleanupStack::PushL(closeSession);
	closeSession->StartL();

	CActiveScheduler::Start();
	
	CleanupStack::PopAndDestroy(2, testActive); // closeSession, testActive
	// Reestablish global RSendAs connection for other tests.
	test(gSendAs.Connect() == KErrNone);
	theUtils->Complete();
	}

// -----------------------------------------------------------------------------

void TestAddAttachmentAndSessionCloseL(TAttachmentTestType aTest)
	{
	RFs fs;
	RFile existingAttachment;

	CleanMailFolder(KMsvDraftEntryId);
	
	CleanupClosePushL(fs);
	CleanupClosePushL(existingAttachment);
	
	User::LeaveIfError(fs.Connect());
	User::LeaveIfError(fs.ShareProtected());
	// Clean up any existing attachment file.
	TInt err = fs.Delete(KExistingAttachmentName);
	if (err != KErrNotFound)
		{
		User::LeaveIfError(err);
		}

	// Create a new attachment file
	User::LeaveIfError(existingAttachment.Create(fs, KExistingAttachmentName, EFileWrite));
	User::LeaveIfError(existingAttachment.Write(KAttachmentContent));

	// Create a message.
	RSendAsMessage message;
	message.CreateL(gSendAs, sendAsTestMtmUid);
	CleanupClosePushL(message);

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);

	switch (aTest)
		{
		case EByName:
			{
			theUtils->Start(_L("Close Session During Add Attachment By Name"));
			theUtils->WriteComment(_L("Add the created attachment file."));
			message.AddAttachment(KExistingAttachmentName, testActive->iStatus);
			break;
			}
		case EByNameWithMimeType:
			{
			theUtils->Start(_L("Close Session During Add Attachment By Name With Mime Type"));
			theUtils->WriteComment(_L("Add the created attachment file."));
			message.AddAttachment(KExistingAttachmentName, KMimeType, testActive->iStatus);
			break;
			}
		case EByFile:
			{
			theUtils->Start(_L("Close Session During Add Attachment By File"));
			theUtils->WriteComment(_L("Add the created attachment file."));
			message.AddAttachment(existingAttachment, testActive->iStatus);
			break;
			}

		case EByFileWithMimeType:
			{
			theUtils->Start(_L("Close Session During Add Attachment By File With Mime Type"));
			theUtils->WriteComment(_L("Add the created attachment file."));
			message.AddAttachment(existingAttachment, KMimeType, testActive->iStatus);
			break;
			}
		case ELinked:
			{
			theUtils->Start(_L("Close Session During Add Linked Attachment"));
			theUtils->WriteComment(_L("Add the created attachment file."));
			message.AddLinkedAttachment(KExistingAttachmentName, testActive->iStatus);
			break;
			}
		case ELinkedWithMimeType:
			{
			theUtils->Start(_L("Close Session During Add Linked Attachment With Mime Type"));
			theUtils->WriteComment(_L("Add the created attachment file."));
			message.AddLinkedAttachment(KExistingAttachmentName, KMimeType, testActive->iStatus);
			break;
			}
		}
	testActive->StartL();
	
	// Create the object to close send-as session.
	CTestCloseSessionActive* closeSession = new(ELeave) CTestCloseSessionActive(gSendAs);
	CleanupStack::PushL(closeSession);
	closeSession->StartL();
	
	// Need to pop-off the RSendAsMessage - as session is being closed, the pop 
	// and destroy will be calling Close on an invalid session handle.
	CleanupStack::Pop(3, &message);
	
	// Push the testActive and closeSession back onto the cleanup stack.
	CleanupStack::PushL(testActive);
	CleanupStack::PushL(closeSession);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(4, &fs); // cancelActive, testActive, existingAttachment, fs

	// Re-establish global RSendAs connection for other tests.
	test(gSendAs.Connect() == KErrNone);
	theUtils->Complete();
	}

// -----------------------------------------------------------------------------

void doTestsL()
	{
	InitL();	//Qualified: no leavescan error

	TestSendMessageCancelL();						//Qualified: no leavescan error
	TestSendMessageConfirmedCancelL();				//Qualified: no leavescan error
	TestAddAttachmentCancelL(EByName);				//Qualified: no leavescan error
	TestAddAttachmentCancelL(EByNameWithMimeType);	//Qualified: no leavescan error
	TestAddAttachmentCancelL(EByFile);				//Qualified: no leavescan error
	TestAddAttachmentCancelL(EByFileWithMimeType);	//Qualified: no leavescan error
	TestAddAttachmentCancelL(ELinked);				//Qualified: no leavescan error
	TestAddAttachmentCancelL(ELinkedWithMimeType);	//Qualified: no leavescan error
	TestSendMessageAndSessionCloseL();				//Qualified: no leavescan error
	TestAddAttachmentAndSessionCloseL(EByName);		//Qualified: no leavescan error
	TestAddAttachmentAndSessionCloseL(EByNameWithMimeType);	//Qualified: no leavescan error
	TestAddAttachmentAndSessionCloseL(EByFile);				//Qualified: no leavescan error
	TestAddAttachmentAndSessionCloseL(EByFileWithMimeType);	//Qualified: no leavescan error
	TestAddAttachmentAndSessionCloseL(ELinked);				//Qualified: no leavescan error	
	TestAddAttachmentAndSessionCloseL(ELinkedWithMimeType);	//Qualified: no leavescan error

	CleanupL();	//Qualified: no leavescan error
	}
