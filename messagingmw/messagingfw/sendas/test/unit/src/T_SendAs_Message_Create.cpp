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

#include <txtrich.h>
#include "sendas2test.h"
#include "csendastestmessage.h"
#include "csendastestaddress.h"
#include <barsc.h>
#include <barsread.h>
#include <t_sendas2_message_create.rsg>

RTest test(_L("SendAs2 Message Create."));
RSendAs gSendAs;
CSendAsMessageTypes* gMessageTypes;
CSendAsAccounts* gAccounts;
TUid sendAsTestMtmUid;

_LIT(KRichTextBodyText, "boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring boring ");
_LIT(KAddress1, "test1@symbian.com");
_LIT(KAddress2, "test2@symbian.com");
_LIT(KAddress3, "test3@symbian.com");
_LIT(KAddress4, "test4@symbian.com");
_LIT(KAlias1, "alias1");
_LIT(KAlias2, "alias2");
_LIT(KResourceFile, "z:\\resource\\messaging\\t_sendas2_message_create.rsc");

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
	gAccounts = CSendAsAccounts::NewL();
	CleanupStack::PushL(gAccounts);
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
	CleanupStack::PopAndDestroy(2, gMessageTypes); // gAccounts, gMessageTypes
	}

// Tests the client AvailableAccountsL routine
void TestAvailableAccountsL()
	{
	theUtils->Start(_L("Available Accounts"));

	// Test that MTM1 has 3 accounts
	gAccounts->Reset();
	gSendAs.AvailableAccountsL(KUidTestMtm1, *gAccounts);
	theUtils->WriteComment(_L("Check that MTM1 has 3 accounts available."));
	test(gAccounts->Count() == 3);

	// Add a new account under MTM5. The name of the account
	// is read in from a resource file and contains some Chinese
	// characters in unicode format. This tests a fix for a crash
	// seen when using SendAs with unicode account names.

	CDummyObserver* ob = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* entry = session->GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(entry);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	RResourceFile resourceFile;
	resourceFile.OpenL(fs, KResourceFile);
	CleanupClosePushL(resourceFile);

	HBufC8* dataBuffer = resourceFile.AllocReadLC(R_CHINESE_UNICODE_SERVICE_NAME);

	TResourceReader resourceReader;
	resourceReader.SetBuffer(dataBuffer);

	HBufC* serviceName = resourceReader.ReadHBufCL();
	CleanupStack::PopAndDestroy(3, &fs);   // fs, resourceFile, dataBuffer
	CleanupStack::PushL(serviceName);

	TMsvEntry service;
	service.iType = KUidMsvServiceEntry;
	service.iMtm = KUidTestMtm5;
	service.iDetails.Set(serviceName->Des());
	theUtils->CreateServiceL(*entry, service);
	CleanupStack::PopAndDestroy(serviceName);

	gAccounts->Reset();
	gSendAs.AvailableAccountsL(KUidTestMtm5, *gAccounts);
	theUtils->WriteComment(_L("Check that MTM5 has 1 account available."));
	test(gAccounts->Count() == 1);

	CleanupStack::PopAndDestroy(3, ob);   // ob, session, entry

	theUtils->Complete();
	}

void TestMessageCreationL()
	{
	theUtils->Start(_L("Message Creation"));

	// Ensure no messages in darfts folder.	
	CleanMailFolder(KMsvDraftEntryId);
	
	RSendAsMessage message;

	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvDraftEntryId, 
		TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(cEntry);

	CMsvEntrySelection* selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);
	test(selection->Count() == 0); // Check no message
	CleanupStack::PopAndDestroy(selection);
	selection = NULL;

	// Creation with mtm type.
	theUtils->WriteComment(_L("Creating message using mtm type."));
	message.CreateL(gSendAs, sendAsTestMtmUid);

	cEntry->SetEntryL(KMsvSentEntryId);
	cEntry->SetEntryL(KMsvDraftEntryId);
	selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);

	// Check a message has been created.
	theUtils->WriteComment(_L("Check message entry has been created."));
	test(selection->Count() == 1);

	cEntry->SetEntryL((*selection)[0]);
	CleanupStack::PopAndDestroy(selection);
	selection = NULL;

	theUtils->WriteComment(_L("Check created message entry has the correct settings."));
	test(cEntry->Entry().iMtm == sendAsTestMtmUid);
	test(cEntry->Entry().Visible() == EFalse);
	test(cEntry->Entry().InPreparation());
	// Need to check default service was used.

	theUtils->WriteComment(_L("Check closing RMessage doesn't save the message."));
	message.Close();

	cEntry->SetEntryL(KMsvDraftEntryId);
	selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);
	test(selection->Count() == 0); // Check no message exists
	CleanupStack::PopAndDestroy(selection);
	selection = NULL;

	// Creation with account
	theUtils->WriteComment(_L("Creating message using account."));
	gAccounts->Reset();
	gSendAs.AvailableAccountsL(sendAsTestMtmUid, *gAccounts);
	message.CreateL(gSendAs, gAccounts->Account(0));

	cEntry->SetEntryL(KMsvSentEntryId);
	cEntry->SetEntryL(KMsvDraftEntryId);
	selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);
	
	// Check a message has been created.
	theUtils->WriteComment(_L("Check message entry has been created."));
	test(selection->Count() == 1);

	cEntry->SetEntryL((*selection)[0]);
	CleanupStack::PopAndDestroy(selection);
	selection = NULL;

	theUtils->WriteComment(_L("Check created message entry has the correct settings."));
	test(cEntry->Entry().iMtm == sendAsTestMtmUid);
	test(cEntry->Entry().Visible() == EFalse);
	test(cEntry->Entry().InPreparation());

	theUtils->WriteComment(_L("Check closing RMessage doesn't save the message."));
	message.Close();

	cEntry->SetEntryL(KMsvDraftEntryId);
	selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);
	test(selection->Count() == 0); // Check no message exists
	CleanupStack::PopAndDestroy(4, ob1); //selection, cEntry, session, ob1

	theUtils->Complete();
	}

void TestMessageSubjectL()
	{
	_LIT(KSubject, "test subject");
	theUtils->Start(_L("Message Subject"));

	CleanMailFolder(KMsvDraftEntryId);

	RSendAsMessage message;
	
	theUtils->WriteComment(_L("Create the message."));
	message.CreateL(gSendAs, sendAsTestMtmUid);
	theUtils->WriteComment(_L("Set the message subject."));
	message.SetSubjectL(KSubject);
	theUtils->WriteComment(_L("Save and close the message."));
	message.SaveMessageAndCloseL();

	// Check newly created message.
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvDraftEntryId, 
		TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(cEntry);
	CMsvEntrySelection* selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);

	theUtils->WriteComment(_L("Check the message exists."));
	test(selection->Count() == 1);
	cEntry->SetEntryL((*selection)[0]);
	CSendAsTestMessage* testMessage = CSendAsTestMessage::NewL();
	CleanupStack::PushL(testMessage);
	CMsvStore* store = cEntry->ReadStoreL();
	CleanupStack::PushL(store);
	testMessage->RestoreL(*store);

	theUtils->WriteComment(_L("Check the new entry's subject field matches the message's subject."));
	test(testMessage->Subject().Match(KSubject) == 0);

	CleanupStack::PopAndDestroy(6, ob1); // store, testMessage, selection, cEntry, session, ob1
	theUtils->Complete();
	}

void TestMessageBioTypeL()
	{
	theUtils->Start(_L("Message Biotype"));

	TUid testBioType = {0x10005534};
	CleanMailFolder(KMsvDraftEntryId);

	RSendAsMessage message;
	
	theUtils->WriteComment(_L("Create the message."));
	message.CreateL(gSendAs, sendAsTestMtmUid);
	theUtils->WriteComment(_L("Set the message biotype."));
	message.SetBioTypeL(testBioType);
	theUtils->WriteComment(_L("Save and close the message."));
	message.SaveMessageAndCloseL();

	// Check newly created message.
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvDraftEntryId, 
		TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(cEntry);
	CMsvEntrySelection* selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);

	theUtils->WriteComment(_L("Check the message exists."));
	test(selection->Count() == 1);
	cEntry->SetEntryL((*selection)[0]);

	theUtils->WriteComment(_L("Check the new entry's biotype matches the message's biotype."));	
	test(cEntry->Entry().iBioType == testBioType.iUid);

	CleanupStack::PopAndDestroy(4, ob1); // selection, cEntry, session, ob1
	theUtils->Complete();
	}

void TestMessageRecipientsL()
	{
	theUtils->Start(_L("Message Recipients"));

	CleanMailFolder(KMsvDraftEntryId);

	RSendAsMessage message;
	theUtils->WriteComment(_L("Create a message."));	
	message.CreateL(gSendAs, sendAsTestMtmUid);
	theUtils->WriteComment(_L("Add 3 'To' recipients."));
	message.AddRecipientL(KAddress1, KAlias1, RSendAsMessage::ESendAsRecipientTo);
	message.AddRecipientL(KAddress2, KAlias2, RSendAsMessage::ESendAsRecipientTo);
	message.AddRecipientL(KAddress3, RSendAsMessage::ESendAsRecipientTo);
	
	theUtils->WriteComment(_L("Add 2 'Cc' recipients."));
	message.AddRecipientL(KAddress3, RSendAsMessage::ESendAsRecipientCc);
	message.AddRecipientL(KAddress4, RSendAsMessage::ESendAsRecipientCc);

	theUtils->WriteComment(_L("Add 1 'Bcc' recipient."));
	message.AddRecipientL(KAddress1, RSendAsMessage::ESendAsRecipientBcc);
	theUtils->WriteComment(_L("Save and close the message"));
	message.SaveMessageAndCloseL();

	// Check newly created message.
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvDraftEntryId, 
		TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry);
	CMsvEntrySelection* selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);

	theUtils->WriteComment(_L("Check the message exists."));
	test(selection->Count() == 1);
	cEntry->SetEntryL((*selection)[0]);
	
	CSendAsTestMessage* savedMessage = CSendAsTestMessage::NewL();
	CleanupStack::PushL(savedMessage);

	CMsvStore* store = cEntry->ReadStoreL();
	CleanupStack::PushL(store);
	theUtils->WriteComment(_L("Restore the message from the new message entry's store."));
	savedMessage->RestoreL(*store);

	RPointerArray<CSendAsTestAddress>& recipients = savedMessage->Addresses();

	theUtils->WriteComment(_L("Check there are the correct number of recipients."));
	TInt count = recipients.Count();
	test(count == 6);

	theUtils->WriteComment(_L("Check the recipients entries match what was set."));
	for (TInt i = 0; i < count; ++i)
		{
		CSendAsTestAddress* address = recipients[i];
		switch(address->AddressType())
			{
			case CSendAsTestMessage::ESendAsTestAddressTypeTo:
				{
				if (address->Address().Match(KAddress1) == 0)
					{
					test(address->Alias().Match(KAlias1) == 0);
					}
				else if (address->Address().Match(KAddress2) == 0)
					{
					test(address->Alias().Match(KAlias2) == 0);
					}
				else
					{
					test(address->Address().Match(KAddress3) == 0);
					}

				break;
				}
			case CSendAsTestMessage::ESendAsTestAddressTypeCc:
				{
				test ( (address->Address().Match(KAddress3) == 0) ||
					(address->Address().Match(KAddress4) == 0));
				break;
				}
			case CSendAsTestMessage::ESendAsTestAddressTypeBcc:
				{
				test(address->Address().Match(KAddress1) == 0);
				break;
				}
			}
		}

	CleanupStack::PopAndDestroy(6, ob1); // store, savedMessage, selection, cEntry, session, ob1
	theUtils->Complete();
	}

void SetRichTextBodyL()
	{
	theUtils->Start(_L("Message Set Rich Text Body"));

	CleanMailFolder(KMsvDraftEntryId);

	RSendAsMessage message;
	theUtils->WriteComment(_L("Create a message."));	
	message.CreateL(gSendAs, sendAsTestMtmUid);

	theUtils->WriteComment(_L("Add CRichText"));
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL(); 
	CleanupStack::PushL(charFormatLayer);

	CRichText* bodyText = CRichText::NewL(paraFormatLayer, charFormatLayer, CEditableText::EFlatStorage, 256);
	CleanupStack::PushL(bodyText);
	bodyText->InsertL(0, KRichTextBodyText);
	message.SetBodyTextL(*bodyText);
	CleanupStack::PopAndDestroy(bodyText);
	bodyText = NULL;

	message.SaveMessageAndCloseL();

	// Check new message.
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvDraftEntryId, 
		TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry);
	CMsvEntrySelection* selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);

	test(selection->Count() == 1);
	cEntry->SetEntryL((*selection)[0]);

	CMsvStore* store = cEntry->ReadStoreL();
	CleanupStack::PushL(store);

	test(store->HasBodyTextL());
	bodyText = CRichText::NewL(paraFormatLayer, charFormatLayer, CEditableText::EFlatStorage, 256);
	CleanupStack::PushL(bodyText);
	store->RestoreBodyTextL(*bodyText);
	HBufC* buffer = HBufC::NewLC(bodyText->DocumentLength());
	TPtr ptr(buffer->Des());
	bodyText->Extract(ptr);

	theUtils->WriteComment(_L("Check new message entry's store for body text and match it to what was set."));
	test(buffer->Match(KRichTextBodyText) == 0);

	CleanupStack::PopAndDestroy(7, ob1); // buffer, bodyText, store, selection, cEntry, session, ob1
	CleanupStack::PopAndDestroy(2, paraFormatLayer); // paraFormatLayer, charFormatLayer

	theUtils->Complete();
	}

void SetPlainTextBodyL()
	{
	theUtils->Start(_L("Message Set Plain Text Body"));

	CleanMailFolder(KMsvDraftEntryId);

	RSendAsMessage message;
	theUtils->WriteComment(_L("Create a message."));	
	message.CreateL(gSendAs, sendAsTestMtmUid);

	theUtils->WriteComment(_L("Add text"));
	message.SetBodyTextL(KRichTextBodyText);
	message.SaveMessageAndCloseL();

	// Check new message.
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);
	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvDraftEntryId, 
		TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone, ETrue));
	CleanupStack::PushL(cEntry);
	CMsvEntrySelection* selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);

	test(selection->Count() == 1);
	cEntry->SetEntryL((*selection)[0]);

	CMsvStore* store = cEntry->ReadStoreL();
	CleanupStack::PushL(store);

	test(store->HasBodyTextL());

	theUtils->WriteComment(_L("Add CRichText"));
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL(); 
	CleanupStack::PushL(charFormatLayer);
	CRichText* bodyText = CRichText::NewL(paraFormatLayer, charFormatLayer, CEditableText::EFlatStorage, 256);
	CleanupStack::PushL(bodyText);
	store->RestoreBodyTextL(*bodyText);
	HBufC* buffer = HBufC::NewLC(bodyText->DocumentLength());
	TPtr ptr(buffer->Des());
	bodyText->Extract(ptr);

	theUtils->WriteComment(_L("Check new message entry's store for body text and match it to what was set."));
	test (buffer->Match(KRichTextBodyText) == 0);
	CleanupStack::PopAndDestroy(9, ob1); // buffer, bodyText, charFormatLayer, paraFormatLayer, store, selection, cEntry, session, ob1

	theUtils->Complete();
	}

void TestSavedMessageEntries()
    {
    theUtils->Start(_L("Message Check Entries"));
 
    CleanMailFolder(KMsvDraftEntryId);
 
    RSendAsMessage message;
    theUtils->WriteComment(_L("Create a message.")); 
    message.CreateL(gSendAs, sendAsTestMtmUid);
 
    theUtils->WriteComment(_L("Add text"));
    message.SetBodyTextL(KRichTextBodyText);   
    theUtils->WriteComment(_L("Save and close the message."));   
    message.SaveMessageAndCloseL();
 
    //check new messages
    CDummyObserver* ob1 = new(ELeave) CDummyObserver;
    CleanupStack::PushL(ob1);
    CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
    CleanupStack::PushL(session);
    CMsvEntry* cEntry = CMsvEntry::NewL(*session, KMsvDraftEntryId, 
    TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
    CleanupStack::PushL(cEntry);
    CMsvEntrySelection* selection = cEntry->ChildrenL();
    CleanupStack::PushL(selection);
 
    theUtils->WriteComment(_L("Check the message exists."));
    test(selection->Count() == 1);
    cEntry->SetEntryL((*selection)[0]);
    CSendAsTestMessage* testMessage = CSendAsTestMessage::NewL();
    CleanupStack::PushL(testMessage);
    CMsvStore* store = cEntry->ReadStoreL();
    CleanupStack::PushL(store);
    testMessage->RestoreL(*store);
 
    theUtils->WriteComment(_L("Check the message entry flags."));
    test(cEntry->Entry().Visible());
    test(cEntry->Entry().InPreparation()==EFalse);
      
    CleanupStack::PopAndDestroy(6, ob1); // store, testMessage,selection, cEntry, session, ob1
     
    theUtils->Complete();
    }

void doTestsL()
	{
	InitL();					// Qualified: no leavescan error

	TestAvailableAccountsL();	// Qualified: no leavescan error

	// Test message creation
	TestMessageCreationL();		// Qualified: no leavescan error

	// Test message initialisation
	TestMessageSubjectL();		// Qualified: no leavescan error
	TestMessageBioTypeL();		// Qualified: no leavescan error
	TestMessageRecipientsL();	// Qualified: no leavescan error
	SetRichTextBodyL();			// Qualified: no leavescan error
	SetPlainTextBodyL();		// Qualified: no leavescan error
	TestSavedMessageEntries();

	CleanupL();					// Qualified: no leavescan error
	}
