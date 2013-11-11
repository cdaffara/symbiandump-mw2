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
// SmtpCountAttachments
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// ExpectedCount  <input>: (Optional) Additional check to check the expected number of attachments.
// Count		  <output>: The number of attachments.
// [Action Description]
// Counts the number attachments associated with the specified message entry.
// [APIs Used]
// CMsvSession::GetEntryL
// CImEmailMessage::AttachmentManagerL
// MMsvAttachmentManager::AttachmentCount
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionSmtpCountAttachments.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>
#include <miutmsg.h>

CMtfTestAction* CMtfTestActionSmtpCountAttachments::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSmtpCountAttachments* self = new(ELeave) CMtfTestActionSmtpCountAttachments(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSmtpCountAttachments::CMtfTestActionSmtpCountAttachments(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSmtpCountAttachments::~CMtfTestActionSmtpCountAttachments()
	{
	}


void CMtfTestActionSmtpCountAttachments::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSmtpCountAttachments);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TInt expectedCount = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2), KErrNotFound);

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CImEmailMessage* emailMsg = CImEmailMessage::NewL(*entry);
	CleanupStack::PushL(emailMsg);

	MMsvAttachmentManager& manager = emailMsg->AttachmentManager();

	TInt attachmentCount = manager.AttachmentCount();

	CleanupStack::PopAndDestroy(2, entry); // emailMsg, entry

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
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSmtpCountAttachments);
	TestCase().ActionCompletedL(*this);
	}
