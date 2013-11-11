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
// GetAttachmentIndexFromId
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// AttachmentId   <input>: Value of the attachment id.
// Index		  <output>: The index of the attachment.
// [Action Description]
// Get the index position of the attachment specified by its id for the
// specified message entry.
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvStore::ReadStoreL
// CMsvStore::AttachmentManagerL
// TODO
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionGetAttachmentIndexFromId.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>
#include <cmsvattachment.h>

CMtfTestAction* CMtfTestActionGetAttachmentIndexFromId::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetAttachmentIndexFromId* self = new(ELeave) CMtfTestActionGetAttachmentIndexFromId(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionGetAttachmentIndexFromId::CMtfTestActionGetAttachmentIndexFromId(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionGetAttachmentIndexFromId::~CMtfTestActionGetAttachmentIndexFromId()
	{
	}


void CMtfTestActionGetAttachmentIndexFromId::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetAttachmentIndexFromId);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvAttachmentId attachmentId = ObtainValueParameterL<TMsvAttachmentId>(TestCase(),ActionParameters().Parameter(2));

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CMsvStore* store = entry->ReadStoreL();
	CleanupStack::PushL(store);

	MMsvAttachmentManager& manager = store->AttachmentManagerL();

	TInt attachmentCount = manager.AttachmentCount();
	TBool found = EFalse;
	
	TInt attachmentIndex;
	
	for(attachmentIndex=0; attachmentIndex<attachmentCount; ++attachmentIndex)
		{
		CMsvAttachment* attachment = manager.GetAttachmentInfoL(attachmentIndex);
		
		if( attachment->Id() == attachmentId )
			{
			// found the attachment
			found = ETrue;
			delete attachment;
			break;
			}
			
		delete attachment;
		}

	CleanupStack::PopAndDestroy(2, entry); // store, entry	

	if( !found )
		{
		// can't find the attachment
		User::Leave(KErrNotFound);
		}

	StoreParameterL<TInt>(TestCase(),attachmentIndex,ActionParameters().Parameter(3));
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetAttachmentIndexFromId);
	TestCase().ActionCompletedL(*this);
	}

