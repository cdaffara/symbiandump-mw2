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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// CreateAttachmentNoFileClose
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// DataFilePath	  <input>: File path that contains the data to populate new attachment with.
// AttachmentId   <output>: Value of the created attachment id.
// [Action Description]
// Creates a new file attachment on the specified message entry directly in message store.
// This test action attempts to Commit the store without the attachment file handle closed.
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvStore::EditStoreL
// CMsvStore::AttachmentManagerL
// CMsvAttachment::NewL
// CMsvAttachment::SetMimeType
// MMsvAttachmentManager::CreateAttachmentL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionCreateAttachmentNoFileClose.h"
#include "CMtfAsyncWaiter.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>
#include <cmsvattachment.h>

CMtfTestAction* CMtfTestActionCreateAttachmentNoFileClose::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateAttachmentNoFileClose* self = new(ELeave) CMtfTestActionCreateAttachmentNoFileClose(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateAttachmentNoFileClose::CMtfTestActionCreateAttachmentNoFileClose(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateAttachmentNoFileClose::~CMtfTestActionCreateAttachmentNoFileClose()
	{
	}


void CMtfTestActionCreateAttachmentNoFileClose::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateAttachmentNoFileClose);
	_LIT(KTxtFileName, "NewFile.txt");
	_LIT8(KTxtFileMimeType, "*/*");
	
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	HBufC* dataFilePath   = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(2));

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CMsvStore* store = entry->EditStoreL();
	CleanupStack::PushL(store);
	
	CMtfAsyncWaiter* waiter = CMtfAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);

	attachment->SetMimeTypeL(KTxtFileMimeType());
	
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	attachment->SetAttachmentNameL(KTxtFileName());
	
	RFile newFile;
	manager.CreateAttachmentL(KTxtFileName, newFile, attachment, waiter->iStatus);
	CleanupStack::Pop(attachment); // ownership passed to manager
	waiter->StartAndWait();
	CleanupClosePushL(newFile);
	User::LeaveIfError(waiter->Result());
	
	TMsvAttachmentId attachmentId = attachment->Id();

	PopulateFileL(newFile, *dataFilePath);
	
	// Don't close the returned file as we want to commit while file is open
	// Should be able to Commit while file is open.
	store->CommitL();
	
	CleanupStack::PopAndDestroy(&newFile);
	CleanupStack::PopAndDestroy(waiter);
	CleanupStack::PopAndDestroy(2, entry); // store, entry
	
	StoreParameterL<TMsvAttachmentId>(TestCase(),attachmentId,ActionParameters().Parameter(3));
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateAttachmentNoFileClose);
	TestCase().ActionCompletedL(*this);
	}

void CMtfTestActionCreateAttachmentNoFileClose::PopulateFileL(RFile& aFile, const TDesC& aDataFilePath)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	RFile dataFile;
	User::LeaveIfError(dataFile.Open(fs, aDataFilePath, EFileRead));
	CleanupClosePushL(fs);
	
	TInt fileSize = 0;
	User::LeaveIfError(dataFile.Size(fileSize));
	
	HBufC8* dataBuffer = HBufC8::NewLC(fileSize);
	TPtr8 bufferPtr(dataBuffer->Des());
	
	User::LeaveIfError(dataFile.Read(bufferPtr));
	
	User::LeaveIfError(aFile.Write(bufferPtr));
	
	CleanupStack::PopAndDestroy(3, &fs); // dataBuffer, dataFile, fs
	}
	
