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
// CountAttachments
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// ExpectedCount  <input>: (Optional) Additional check to check the expected number of attachments.
// Count		  <output>: The number of attachments.
// [Action Description]
// Counts the number attachments associated with the specified message entry.
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvStore::ReadStoreL
// CMsvStore::AttachmentManagerL
// MMsvAttachmentManager::AttachmentCount
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionCountAttachments.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>

CMtfTestAction* CMtfTestActionCountAttachments::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCountAttachments* self = new(ELeave) CMtfTestActionCountAttachments(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCountAttachments::CMtfTestActionCountAttachments(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCountAttachments::~CMtfTestActionCountAttachments()
	{
	}


void CMtfTestActionCountAttachments::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCountAttachments);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TInt expectedCount = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2), KErrNotFound);

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CMsvStore* store = entry->ReadStoreL();
	CleanupStack::PushL(store);

	MMsvAttachmentManager& manager = store->AttachmentManagerL();

	TInt attachmentCount = manager.AttachmentCount();

	CleanupStack::PopAndDestroy(2, entry); // store, entry

	if( expectedCount >= 0 )
		{
		// Expected count check is enabled
		if( expectedCount!=attachmentCount )
			{
			// Not the expected number of attachments
			User::Leave(KErrGeneral);
			}
		}

	StoreParameterL<TInt>(TestCase(),attachmentCount,ActionParameters().Parameter(3));
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCountAttachments);
	TestCase().ActionCompletedL(*this);
	}

