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
// RemoveAllAttachments
// [Action Parameters]
// Session				<input>: Reference to the session.
// MsgId				<input>: Value of the message Id.
// AttachmentIdFlag		<input>: Flag to state if attachment is to be removed using attachment Id.
// [Action Description]
// Removes all the attachments of a specified message. If AttachmentIdFlag is set to ETrue
// attachments will be removed using attachment Id, else index value is used.
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvStore::ReadStoreL
// CMsvEntry::SetEntryL
// CMsvStore::AttachmentManagerL
// MMsvAttachmentManager::RemoveAttachmentL
// MMsvAttachmentManager::AttachmentCountL
// TODO
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionRemoveAllAttachments.h"
#include "CMtfAsyncWaiter.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <mmsvattachmentmanager.h>
#include <cmsvattachment.h>

CMtfTestAction* CMtfTestActionRemoveAllAttachments::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionRemoveAllAttachments* self = new(ELeave) CMtfTestActionRemoveAllAttachments(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionRemoveAllAttachments::CMtfTestActionRemoveAllAttachments(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionRemoveAllAttachments::~CMtfTestActionRemoveAllAttachments()
	{
	}

void CMtfTestActionRemoveAllAttachments::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionRemoveAllAttachments);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TInt attachIdFlag = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2), 0);

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CMsvStore* store = entry->EditStoreL();
	CleanupStack::PushL(store);
	
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	
	TInt attachmentCount = manager.AttachmentCount();
		
	for ( ; attachmentCount > 0 ; attachmentCount-- )
	{
	CMtfAsyncWaiter* waiter = CMtfAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
		
	if( attachIdFlag == 1 )
		{
		CMsvAttachment* attachInfo = manager.GetAttachmentInfoL(0);
		CleanupStack::PushL(attachInfo);
				
		manager.RemoveAttachmentL(attachInfo->Id(), waiter->iStatus);
		CleanupStack::PopAndDestroy(attachInfo);
		}
	else
		{
		manager.RemoveAttachmentL(0, waiter->iStatus);
		}
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::PopAndDestroy(waiter);
	}
	
	store->CommitL();
	
	CleanupStack::PopAndDestroy(2, entry); // store, entry

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionRemoveAllAttachments);
	TestCase().ActionCompletedL(*this);
	}

