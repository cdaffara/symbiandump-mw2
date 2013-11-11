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
// RemoveEntryAttachmentById
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// AttachmentId	  <input>: Attachment Id of attachment to remove.
// ErrorCode	  <output>: (Optional) Returned error code, if not requested then code will
// leave if not KErrNone
// [Action Description]
// Removes the specified entry attachment by its attachment Id. The
// attachment is first checked to to ensure that it is an entry attachment and 
// then removes it. An additional check is done to ensure that the actual 
// entry still remains in the message store.
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvStore::ReadStoreL
// CMsvStore::AttachmentManagerL
// TODO
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionRemoveEntryAttachmentById.h"
#include "CMtfAsyncWaiter.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>
#include <cmsvattachment.h>

CMtfTestAction* CMtfTestActionRemoveEntryAttachmentById::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionRemoveEntryAttachmentById* self = new(ELeave) CMtfTestActionRemoveEntryAttachmentById(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionRemoveEntryAttachmentById::CMtfTestActionRemoveEntryAttachmentById(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionRemoveEntryAttachmentById::~CMtfTestActionRemoveEntryAttachmentById()
	{
	}

void CMtfTestActionRemoveEntryAttachmentById::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionRemoveEntryAttachmentById);
	TRAPD(err, RunTestL());
	
	if( ActionParameters().Count() < 4 )
		{
		User::LeaveIfError(err);
		}
	else
		{
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(3));
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionRemoveEntryAttachmentById);	
	TestCase().ActionCompletedL(*this);
	}
	
void CMtfTestActionRemoveEntryAttachmentById::RunTestL()
	{
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvAttachmentId attachId = ObtainValueParameterL<TMsvAttachmentId>(TestCase(),ActionParameters().Parameter(2));

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CMsvStore* store = entry->EditStoreL();
	CleanupStack::PushL(store);
	
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	
	CMsvAttachment* attachInfo = manager.GetAttachmentInfoL(attachId);
	CleanupStack::PushL(attachInfo);
	
	// First ensure that the attachment is a file attachment
	if( attachInfo->Type() != CMsvAttachment::EMsvMessageEntry )
		{
		User::Leave(KErrGeneral);
		}
		
	// Get the linked file
	TMsvId msgId = attachInfo->EntryAttachmentId();
	CleanupStack::PopAndDestroy(attachInfo);
		
	// Remove the attachment
	CMtfAsyncWaiter* waiter = CMtfAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	manager.RemoveAttachmentL(attachId, waiter->iStatus);
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::PopAndDestroy(waiter);
	
	store->CommitL();
	
	CleanupStack::PopAndDestroy(2, entry); // store, entry

	// Ensure that the message entry still exists
	CMsvEntry* checkEntry = paramSession->GetEntryL(msgId);
	delete checkEntry;
	checkEntry = NULL;	
	}


	
