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
// RemoveLinkedAttachmentById
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// AttachmentId	  <input>: Attachment Id of attachment to remove.
// ErrorCode	  <output>: (Optional) Returned error code, if not requested then code will
// leave if not KErrNone
// [Action Description]
// Removes the specified linked file attachment by its attachment Id. The
// attachment is first checked to to ensure that it is a linked attachment and 
// then removes it. An additional check is done to access the file to ensure
// that the linked file has not been deleted.
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvStore::ReadStoreL
// CMsvStore::AttachmentManagerL
// TODO
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionRemoveLinkedAttachmentById.h"
#include "CMtfAsyncWaiter.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>
#include <cmsvattachment.h>

CMtfTestAction* CMtfTestActionRemoveLinkedAttachmentById::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionRemoveLinkedAttachmentById* self = new(ELeave) CMtfTestActionRemoveLinkedAttachmentById(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionRemoveLinkedAttachmentById::CMtfTestActionRemoveLinkedAttachmentById(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionRemoveLinkedAttachmentById::~CMtfTestActionRemoveLinkedAttachmentById()
	{
	}

void CMtfTestActionRemoveLinkedAttachmentById::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionRemoveLinkedAttachmentById);
	TRAPD(err, RunTestL());
	
	if( ActionParameters().Count() < 4 )
		{
		User::LeaveIfError(err);
		}
	else
		{
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(3));
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionRemoveLinkedAttachmentById);	
	TestCase().ActionCompletedL(*this);
	}
	
void CMtfTestActionRemoveLinkedAttachmentById::RunTestL()
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
	if( attachInfo->Type() != CMsvAttachment::EMsvLinkedFile )
		{
		User::Leave(KErrGeneral);
		}
		
	// Get the linked file
	RFile file = manager.GetAttachmentFileL(attachId);
	CleanupClosePushL(file);
	
	// Remove the attachment
	CMtfAsyncWaiter* waiter = CMtfAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	manager.RemoveAttachmentL(attachId, waiter->iStatus);
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::PopAndDestroy(waiter);
	
	store->CommitL();
	
	// Ensure that the file still there
	TInt fileSize;
	User::LeaveIfError(file.Size(fileSize));
	if( !(fileSize > 0) )
		{
		// no contents in the file
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(4, entry); // attachInfo, file, store, entry
	}


	
