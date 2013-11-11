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
#include <cmsvmimeheaders.h>

RTest test(_L("SendAs2 Message Attachments."));
RSendAs gSendAs;
CSendAsMessageTypes* gMessageTypes;
TUid sendAsTestMtmUid;
const TUint KNoCharsetSpecifed = 0;
const TUint KTestCharset = 4;

_LIT8(KAttachmentContent, "content!");
_LIT8(KMimeType, "text/html");
_LIT(KExistingAttachmentName1, "c:\\t_sendas2_send_a1.txt");
_LIT(KExistingAttachmentName2, "c:\\t_sendas2_send_a2.txt");
_LIT(KCreatedAttachmentName1, "t_sendas2_send_b1.txt");
_LIT(KCreatedAttachmentName2, "t_sendas2_send_b2.txt");

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
	
void CreateAttachmentFileL(RFs& aFs, const TDesC& aFileName)
	{
	// Delete the file if it exists.
	TInt err = aFs.Delete(aFileName);
	if (err != KErrNotFound)
		{
		User::LeaveIfError(err);
		}

	// Create a new attachment file
	RFile file;
	User::LeaveIfError(file.Create(aFs, aFileName, EFileWrite));
	User::LeaveIfError(file.Write(KAttachmentContent));
	User::LeaveIfError(file.Write(KAttachmentContent));
	file.Close();	
	}

void CheckNewAttachmentL(const TDesC& aFileName, TInt aFileSize, const TDesC8& aMimeType, TUint aCharset, TBool aLinkedAttachment)
	{
	// Check that file exists in the store and it matches our control file.
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
	
	CMsvStore* store = cEntry->ReadStoreL();
	CleanupStack::PushL(store);
	
	MMsvAttachmentManager& attachmentMgr = store->AttachmentManagerL();
	TInt count = attachmentMgr.AttachmentCount();
	test(count > 0);
	
	TBool found = EFalse;
	
	while( count-- > 0 && !found )
		{
		CMsvAttachment* attachmentInfo = attachmentMgr.GetAttachmentInfoL(count);
		CleanupStack::PushL(attachmentInfo);

		// Check the attachment info is what we expect.
		TParse parser1;
		TParse parser2;
		parser1.Set(attachmentInfo->FilePath(), NULL, NULL);
		parser2.Set(aFileName, NULL, NULL);

		found = ETrue;
		if (aLinkedAttachment)
			{
			found = found && (attachmentInfo->Type() == CMsvAttachment::EMsvLinkedFile);  // Check it's a linked file.
			found = found && (parser1.DriveAndPath() == parser2.DriveAndPath());
			}
		else
			{
			found = found && (attachmentInfo->Type() == CMsvAttachment::EMsvFile);  // Check it's a file.
			found = found && (parser1.DriveAndPath() != parser2.DriveAndPath());
			}
			
		found = found && (attachmentInfo->MimeType().Match(aMimeType) == 0);  // Check mime type matches.
		found = found && (parser1.NameAndExt() == parser2.NameAndExt()); // This necessarily correct?
		found = found && (attachmentInfo->Size() == aFileSize); // Check size matches.
		
		// check charset matches
		CMsvMimeHeaders* mimeHeaders = CMsvMimeHeaders::NewLC();
		mimeHeaders->RestoreL(*attachmentInfo);
		found = found && (mimeHeaders->MimeCharset() == aCharset);
		
		CleanupStack::PopAndDestroy(2, attachmentInfo); // mimeHeaders, attachmentInfo
		}
	test(found);

	CleanupStack::PopAndDestroy(5, ob1); // store, selection, cEntry, sesion, ob1
	}

void AddNewAttachmentL(RSendAsMessage& aMessage, RFs& aFs, const TDesC& aFileName, 
					   const TDesC8& aMimeType, TBool aUseRFile, TRequestStatus& aStatus)
	{
	if (aUseRFile)
		{
		RFile attachment;
		User::LeaveIfError(attachment.Open(aFs, aFileName, EFileRead));
		if (aMimeType.Length() > 0)
			{
			aMessage.AddAttachment(attachment, aMimeType, aStatus);
			}
		else
			{
			aMessage.AddAttachment(attachment, aStatus);
			}
		attachment.Close();
		}
	else
		{
		if (aMimeType.Length() > 0)
			{
			aMessage.AddAttachment(aFileName, aMimeType, aStatus);
			}
		else
			{
			aMessage.AddAttachment(aFileName, aStatus);
			}
		}
	}

void AddNewAttachmentWithCharsetL(RSendAsMessage& aMessage, RFs& aFs, const TDesC& aFileName, 
					   const TDesC8& aMimeType, TUint aCharset, TBool aUseRFile, TRequestStatus& aStatus)
	{
	if (aUseRFile)
		{
		RFile attachment;
		User::LeaveIfError(attachment.Open(aFs, aFileName, EFileRead));
		aMessage.AddAttachment(attachment, aMimeType, aCharset, aStatus);
		attachment.Close();
		}
	else
		{
		aMessage.AddAttachment(aFileName, aMimeType, aCharset, aStatus);
		}
	}

void TestMessageAddAttachmentL(TBool aUseName, const TDesC8& aMimeType)
	{
	CleanMailFolder(KMsvDraftEntryId);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	User::LeaveIfError(fs.ShareProtected());
	// Clean up any existing attachment files and create two new ones.
	theUtils->WriteComment(_L("Create two attachment files."));
	CreateAttachmentFileL(fs, KExistingAttachmentName1);
	CreateAttachmentFileL(fs, KExistingAttachmentName2);

	// Create a message.
	RSendAsMessage message;
	theUtils->WriteComment(_L("Create a message."));
	message.CreateL(gSendAs, sendAsTestMtmUid);

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);

	theUtils->WriteComment(_L("Add first created attachment file."));
	AddNewAttachmentL(message, fs, KExistingAttachmentName1, aMimeType, aUseName, testActive->iStatus);
	testActive->StartL();
	CActiveScheduler::Start();
	test(testActive->iStatus.Int() == KErrNone);

	theUtils->WriteComment(_L("Add second created attachment file."));
	AddNewAttachmentL(message, fs, KExistingAttachmentName2, aMimeType, aUseName, testActive->iStatus);
	testActive->StartL();
	CActiveScheduler::Start();
	test(testActive->iStatus.Int() == KErrNone);

	CleanupStack::PopAndDestroy(testActive);
	theUtils->WriteComment(_L("Save and close the message."));
	message.SaveMessageAndCloseL();

	CheckNewAttachmentL(KExistingAttachmentName1, 16, aMimeType, KNoCharsetSpecifed, EFalse);
	CheckNewAttachmentL(KExistingAttachmentName2, 16, aMimeType, KNoCharsetSpecifed, EFalse);
	fs.Close();
	
	theUtils->Complete();
	}

void TestMessageAddAttachmentL(TBool aUseName, const TDesC8& aMimeType, TUint aCharset)
	{
	CleanMailFolder(KMsvDraftEntryId);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	User::LeaveIfError(fs.ShareProtected());
	// Clean up any existing attachment files and create two new ones.
	theUtils->WriteComment(_L("Create two attachment files."));
	CreateAttachmentFileL(fs, KExistingAttachmentName1);
	CreateAttachmentFileL(fs, KExistingAttachmentName2);

	// Create a message.
	RSendAsMessage message;
	theUtils->WriteComment(_L("Create a message."));
	message.CreateL(gSendAs, sendAsTestMtmUid);

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);

	theUtils->WriteComment(_L("Add first created attachment file."));
	AddNewAttachmentWithCharsetL(message, fs, KExistingAttachmentName1, aMimeType, aCharset, aUseName, testActive->iStatus);
	testActive->StartL();
	CActiveScheduler::Start();
	test(testActive->iStatus.Int() == KErrNone);

	theUtils->WriteComment(_L("Add second created attachment file."));
	AddNewAttachmentWithCharsetL(message, fs, KExistingAttachmentName2, aMimeType, aCharset, aUseName, testActive->iStatus);
	testActive->StartL();
	CActiveScheduler::Start();
	test(testActive->iStatus.Int() == KErrNone);

	CleanupStack::PopAndDestroy(testActive);
	theUtils->WriteComment(_L("Save and close the message."));
	message.SaveMessageAndCloseL();

	CheckNewAttachmentL(KExistingAttachmentName1, 16, aMimeType, aCharset, EFalse);
	CheckNewAttachmentL(KExistingAttachmentName2, 16, aMimeType, aCharset, EFalse);
	fs.Close();
	
	theUtils->Complete();
	}

void TestMessageAddLinkedAttachmentL(const TDesC8& aMimeType)
	{
	CleanMailFolder(KMsvDraftEntryId);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	User::LeaveIfError(fs.ShareProtected());
	// Clean up any existing attachment files and create two new ones.
	theUtils->WriteComment(_L("Create two attachment files."));
	CreateAttachmentFileL(fs, KExistingAttachmentName1);
	CreateAttachmentFileL(fs, KExistingAttachmentName2);

	// Create a message.
	RSendAsMessage message;
	theUtils->WriteComment(_L("Create a message."));
	message.CreateL(gSendAs, sendAsTestMtmUid);

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);

	theUtils->WriteComment(_L("Add first created attachment file as a link."));
	if (aMimeType.Length() > 0)
		{
		message.AddLinkedAttachment(KExistingAttachmentName1, aMimeType, testActive->iStatus);
		}
	else
		{
		message.AddLinkedAttachment(KExistingAttachmentName1, testActive->iStatus);
		}
	testActive->StartL();
	CActiveScheduler::Start();
	
	theUtils->WriteComment(_L("Add second created attachment file as a link."));
	if (aMimeType.Length() > 0)
		{
		message.AddLinkedAttachment(KExistingAttachmentName2, aMimeType, testActive->iStatus);
		}
	else
		{
		message.AddLinkedAttachment(KExistingAttachmentName2, testActive->iStatus);
		}
	testActive->StartL();
	CActiveScheduler::Start();

	test(testActive->iStatus.Int() == KErrNone);
	CleanupStack::PopAndDestroy(testActive);
	theUtils->WriteComment(_L("Save and close the message."));
	message.SaveMessageAndCloseL();

	CheckNewAttachmentL(KExistingAttachmentName1, 16, aMimeType, KNoCharsetSpecifed, ETrue); // ETrue to indicate linked attachment
	CheckNewAttachmentL(KExistingAttachmentName2, 16, aMimeType, KNoCharsetSpecifed, ETrue); // ETrue to indicate linked attachment
	fs.Close();
	
	theUtils->Complete();
	}

void TestMessageCreateAttachmentL(const TDesC8& aMimeType)
	{
	RFile createdAttachment;

	CleanMailFolder(KMsvDraftEntryId);

	// Create a message.
	RSendAsMessage message;
	theUtils->WriteComment(_L("Create a message."));
	message.CreateL(gSendAs, sendAsTestMtmUid);
	theUtils->WriteComment(_L("Create first attachment file."));
	if (aMimeType.Length() > 0)
		{
		message.CreateAttachmentL(KCreatedAttachmentName1, createdAttachment, aMimeType);
		}
	else
		{
		message.CreateAttachmentL(KCreatedAttachmentName1, createdAttachment);
		}

	// RFile should now be open on the new attachment.
	test(createdAttachment.Write(KAttachmentContent) == KErrNone);
	createdAttachment.Close();

	theUtils->WriteComment(_L("Create second attachment file."));
	if (aMimeType.Length() > 0)
		{
		message.CreateAttachmentL(KCreatedAttachmentName2, createdAttachment, aMimeType);
		}
	else
		{
		message.CreateAttachmentL(KCreatedAttachmentName2, createdAttachment);
		}

	// RFile should now be open on the new attachment.
	test(createdAttachment.Write(KAttachmentContent) == KErrNone);
	createdAttachment.Close();

	message.SaveMessageAndCloseL();

	CheckNewAttachmentL(KCreatedAttachmentName1, 0, aMimeType, KNoCharsetSpecifed, EFalse);
	CheckNewAttachmentL(KCreatedAttachmentName2, 0, aMimeType, KNoCharsetSpecifed, EFalse);
	
	theUtils->Complete();
	}

void TestMessageCreateAttachmentL(const TDesC8& aMimeType, TUint aCharset)
	{
	RFile createdAttachment;

	CleanMailFolder(KMsvDraftEntryId);

	// Create a message.
	RSendAsMessage message;
	theUtils->WriteComment(_L("Create a message."));
	message.CreateL(gSendAs, sendAsTestMtmUid);
	theUtils->WriteComment(_L("Create first attachment file."));
	message.CreateAttachmentL(KCreatedAttachmentName1, createdAttachment, aMimeType, aCharset);

	// RFile should now be open on the new attachment.
	test(createdAttachment.Write(KAttachmentContent) == KErrNone);
	createdAttachment.Close();

	theUtils->WriteComment(_L("Create second attachment file."));
	message.CreateAttachmentL(KCreatedAttachmentName2, createdAttachment, aMimeType, aCharset);

	// RFile should now be open on the new attachment.
	test(createdAttachment.Write(KAttachmentContent) == KErrNone);
	createdAttachment.Close();

	message.SaveMessageAndCloseL();

	CheckNewAttachmentL(KCreatedAttachmentName1, 0, aMimeType, aCharset, EFalse);
	CheckNewAttachmentL(KCreatedAttachmentName2, 0, aMimeType, aCharset, EFalse);
	
	theUtils->Complete();
	}

void TestMessageAddLinkedAttachmentL(const TDesC8& aMimeType, TUint aCharset)
	{
	CleanMailFolder(KMsvDraftEntryId);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	User::LeaveIfError(fs.ShareProtected());
	// Clean up any existing attachment files and create two new ones.
	theUtils->WriteComment(_L("Create two attachment files."));
	CreateAttachmentFileL(fs, KExistingAttachmentName1);
	CreateAttachmentFileL(fs, KExistingAttachmentName2);

	// Create a message.
	RSendAsMessage message;
	theUtils->WriteComment(_L("Create a message."));
	message.CreateL(gSendAs, sendAsTestMtmUid);

	CTestActive* testActive = new(ELeave) CTestActive;
	CleanupStack::PushL(testActive);

	theUtils->WriteComment(_L("Add first created attachment file as a link."));
	message.AddLinkedAttachment(KExistingAttachmentName1, aMimeType, aCharset, testActive->iStatus);

	testActive->StartL();
	CActiveScheduler::Start();
	
	theUtils->WriteComment(_L("Add second created attachment file as a link."));
	message.AddLinkedAttachment(KExistingAttachmentName2, aMimeType, aCharset, testActive->iStatus);

	testActive->StartL();
	CActiveScheduler::Start();

	test(testActive->iStatus.Int() == KErrNone);
	CleanupStack::PopAndDestroy(testActive);
	theUtils->WriteComment(_L("Save and close the message."));
	message.SaveMessageAndCloseL();

	CheckNewAttachmentL(KExistingAttachmentName1, 16, aMimeType, aCharset, ETrue); // ETrue to indicate linked attachment
	CheckNewAttachmentL(KExistingAttachmentName2, 16, aMimeType, aCharset, ETrue); // ETrue to indicate linked attachment
	fs.Close();
	
	theUtils->Complete();
	}

void doTestsL()
	{
	InitL();	// Qualified: no leavescan error

	theUtils->Start(_L("Message Add Attachments using RFile"));
	TestMessageAddAttachmentL(EFalse, KNullDesC8);	// Qualified: no leavescan error
	
	theUtils->Start(_L("Message Add Attachments using filename"));
	TestMessageAddAttachmentL(ETrue, KNullDesC8);	// Qualified: no leavescan error

	theUtils->Start(_L("Message Add Attachments using RFile with mime type"));
	TestMessageAddAttachmentL(EFalse, KMimeType);	// Qualified: no leavescan error

	theUtils->Start(_L("Message Add Attachments using filename with mime type"));
	TestMessageAddAttachmentL(ETrue, KMimeType);	// Qualified: no leavescan error

	theUtils->Start(_L("Message Add Linked Attachments"));
	TestMessageAddLinkedAttachmentL(KNullDesC8);	// Qualified: no leavescan error

	theUtils->Start(_L("Message Add Linked Attachments with mime type"));
	TestMessageAddLinkedAttachmentL(KMimeType);		// Qualified: no leavescan error

	theUtils->Start(_L("Message Create Attachments"));
	TestMessageCreateAttachmentL(KNullDesC8);		// Qualified: no leavescan error
	
	theUtils->Start(_L("Message Create Attachments with mime type"));
	TestMessageCreateAttachmentL(KMimeType);		// Qualified: no leavescan error
	
	// Charset testing
	theUtils->Start(_L("Message Add Attachments using RFile with mime type/charset"));
	TestMessageAddAttachmentL(EFalse, KMimeType, KTestCharset);	// Qualified: no leavescan error
	
	theUtils->Start(_L("Message Add Attachments using filename with mime type/charset"));
	TestMessageAddAttachmentL(ETrue, KMimeType, KTestCharset);	// Qualified: no leavescan error

	theUtils->Start(_L("Message Add Linked Attachments with mime type/charset"));
	TestMessageAddLinkedAttachmentL(KMimeType, KTestCharset);	// Qualified: no leavescan error
	
	theUtils->Start(_L("Message Create Attachments with mime type/charset"));
	TestMessageCreateAttachmentL(KMimeType, KTestCharset);		// Qualified: no leavescan error
	
	CleanupL();	// Qualified: no leavescan error
	}
