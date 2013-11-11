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
// CreateAttachment
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// DataFilePath	  <input>: File path that contains the data to populate new attachment with.
// AttachmentId   <output>: Value of the created attachment id.
// ErrorCode	  <output>: (Optional) Returned error code, if not requested then code will
// leave if not KErrNone
// [Action Description]
// Creates a new file attachment on the specified message entry directly in message store.
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


#include "CMtfTestActionCreateAttachment.h"
#include "CMtfAsyncWaiter.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>
#include <cmsvattachment.h>

CMtfTestAction* CMtfTestActionCreateAttachment::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateAttachment* self = new(ELeave) CMtfTestActionCreateAttachment(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateAttachment::CMtfTestActionCreateAttachment(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateAttachment::~CMtfTestActionCreateAttachment()
	{
	}

void CMtfTestActionCreateAttachment::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateAttachment);
	TRAPD(err, RunTestL());
	
	if( ActionParameters().Count() < 5 )
		{
		User::LeaveIfError(err);
		}
	else
		{
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(4));
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateAttachment);	
	TestCase().ActionCompletedL(*this); 
	}
	
void CMtfTestActionCreateAttachment::RunTestL()
	{
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
	attachment->SetAttachmentNameL(KTxtFileName());
	
	
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	RFile newFile;
	manager.CreateAttachmentL(KTxtFileName, newFile, attachment, waiter->iStatus);
	CleanupStack::Pop(attachment); // ownership passed to manager
	waiter->StartAndWait();
	CleanupClosePushL(newFile);
	User::LeaveIfError(waiter->Result());
	
	TMsvAttachmentId attachmentId = attachment->Id();
	
	TInt fileSize = PopulateFileL(newFile, *dataFilePath);
	attachment->SetSize(fileSize);
	
	CleanupStack::PopAndDestroy(&newFile);
	CleanupStack::PopAndDestroy(waiter);
	
	store->CommitL();
	
	CleanupStack::PopAndDestroy(2, entry); // store, entry
	
	StoreParameterL<TMsvAttachmentId>(TestCase(),attachmentId,ActionParameters().Parameter(3));
	}

TInt CMtfTestActionCreateAttachment::PopulateFileL(RFile& aFile, const TDesC& aDataFilePath)
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
	return fileSize;
	}
	
