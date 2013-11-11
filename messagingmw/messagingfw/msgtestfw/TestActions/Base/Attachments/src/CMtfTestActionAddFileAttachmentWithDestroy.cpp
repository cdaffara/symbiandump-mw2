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
// AddFileAttachmentWithDestroy
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// FilePath       <input>: File path of the attachment file.
// MimeType		  <input>: Mime-type of the attachment.
// [Action Description]
// Synchrounously creates a file attachment on the specified message entry located
// using a specified file. The store is then destroyed and the attachment should not exist.
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvStore::EditStoreL
// CMsvStore::AttachmentManagerL
// CMsvAttachment::NewL
// CMsvAttachment::SetMimeType
// MMsvAttachmentManager::AddAttachmentL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionAddFileAttachmentWithDestroy.h"
#include "CMtfAsyncWaiter.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <cmsvattachment.h>
#include <mmsvattachmentmanager.h>

CMtfTestAction* CMtfTestActionAddFileAttachmentWithDestroy::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionAddFileAttachmentWithDestroy* self = new(ELeave) CMtfTestActionAddFileAttachmentWithDestroy(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionAddFileAttachmentWithDestroy::CMtfTestActionAddFileAttachmentWithDestroy(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionAddFileAttachmentWithDestroy::~CMtfTestActionAddFileAttachmentWithDestroy()
	{
	}

void CMtfTestActionAddFileAttachmentWithDestroy::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionAddFileAttachmentWithDestroy);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	HBufC* paramFilePath   = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(2));
	HBufC8* paramMimeType = ObtainParameterReferenceL<HBufC8>(TestCase(),ActionParameters().Parameter(3));

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CMsvStore* store = entry->EditStoreL();
	CleanupStack::PushL(store);
	
	CMtfAsyncWaiter* waiter = CMtfAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);

	attachment->SetMimeTypeL(*paramMimeType);
	TParse fileNameParser;
	User::LeaveIfError(fileNameParser.Set(*paramFilePath, NULL, NULL));
	attachment->SetAttachmentNameL(fileNameParser.NameAndExt());

	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	
	manager.AddAttachmentL(*paramFilePath, attachment, waiter->iStatus);
	CleanupStack::Pop(attachment); // ownership passed to manager
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::PopAndDestroy(waiter);
	
	TMsvAttachmentId attachmentId = attachment->Id();
	
	// Destroy the store without Commit or Revert...
	CleanupStack::PopAndDestroy(2, entry); // store, entry
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionAddFileAttachmentWithDestroy);
	TestCase().ActionCompletedL(*this);
	}



