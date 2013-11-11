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
// AddLinkedAttachment
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// FilePath       <input>: File path of the attachment file to add as linked.
// MimeType		  <input>: Mime-type of the attachment.
// AttachmentId   <output>: Value of the created attachment id.
// ErrorCode	  <output>: (Optional) Returned error code, if not requested then code will
// leave if not KErrNone
// [Action Description]
// Adds the specified file as a linked attachment to the specified message
// entry.
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvStore::EditStoreL
// CMsvStore::AttachmentManagerL
// CMsvAttachment::NewL
// CMsvAttachment::SetMimeType
// MMsvAttachmentManager::AddLinkedAttachmentL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionAddLinkedAttachment.h"
#include "CMtfAsyncWaiter.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <cmsvattachment.h>
#include <mmsvattachmentmanager.h>

CMtfTestAction* CMtfTestActionAddLinkedAttachment::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionAddLinkedAttachment* self = new(ELeave) CMtfTestActionAddLinkedAttachment(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionAddLinkedAttachment::CMtfTestActionAddLinkedAttachment(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionAddLinkedAttachment::~CMtfTestActionAddLinkedAttachment()
	{
	}

void CMtfTestActionAddLinkedAttachment::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionAddLinkedAttachment);
	TRAPD(err, RunTestL());
	
	if( ActionParameters().Count() < 6 )
		{
		User::LeaveIfError(err);
		}
	else
		{
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(5));
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionAddLinkedAttachment);
	TestCase().ActionCompletedL(*this); 
	}
	
void CMtfTestActionAddLinkedAttachment::RunTestL()
	{
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
	
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvLinkedFile);
	CleanupStack::PushL(attachment);

	attachment->SetMimeTypeL(*paramMimeType);
	TParse fileNameParser;
	User::LeaveIfError(fileNameParser.Set(*paramFilePath, NULL, NULL));
	attachment->SetAttachmentNameL(fileNameParser.NameAndExt());
	TEntry fileEntry;
	User::LeaveIfError(paramSession->FileSession().Entry( fileNameParser.FullName(), fileEntry) );
	attachment->SetSize(fileEntry.iSize);

	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	
	manager.AddLinkedAttachmentL(*paramFilePath, attachment, waiter->iStatus);
	CleanupStack::Pop(attachment); // ownership passed to manager
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::PopAndDestroy(waiter);

	TMsvAttachmentId attachmentId = attachment->Id();
	
	store->CommitL();
	
	CleanupStack::PopAndDestroy(2, entry); // store, entry
	
	StoreParameterL<TMsvAttachmentId>(TestCase(),attachmentId,ActionParameters().Parameter(4));
	}



