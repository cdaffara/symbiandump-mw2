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
// CMTFTESTACTIONREMOVEFILEATTACHMENTWITDESTROY.CPP
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// RemoveFileAttachmentWithDestroy
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// AttachmentId	  <input>: Attachment Id of attachment to remove.
// [Action Description]
// Removes the specified file attachment by its attachment Id. The attachment
// is first checked to to ensure that it is a file attachment and then
// removes it. The store is then destroyed so that the remove attachment is
// never commited.
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvStore::ReadStoreL
// CMsvStore::AttachmentManagerL
// TODO
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionRemoveFileAttachmentWithDestroy.h"
#include "CMtfAsyncWaiter.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>
#include <cmsvattachment.h>

CMtfTestAction* CMtfTestActionRemoveFileAttachmentWithDestroy::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionRemoveFileAttachmentWithDestroy* self = new(ELeave) CMtfTestActionRemoveFileAttachmentWithDestroy(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionRemoveFileAttachmentWithDestroy::CMtfTestActionRemoveFileAttachmentWithDestroy(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionRemoveFileAttachmentWithDestroy::~CMtfTestActionRemoveFileAttachmentWithDestroy()
	{
	}

void CMtfTestActionRemoveFileAttachmentWithDestroy::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionRemoveFileAttachmentWithDestroy);
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
	if( attachInfo->Type() != CMsvAttachment::EMsvFile )
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(attachInfo);
	
	CMtfAsyncWaiter* waiter = CMtfAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	manager.RemoveAttachmentL(attachId, waiter->iStatus);
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::PopAndDestroy(waiter);	
	
	// destroy the store without commit
	CleanupStack::PopAndDestroy(2, entry); // store, entry
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionRemoveFileAttachmentWithDestroy);
	TestCase().ActionCompletedL(*this); 
	}


	
