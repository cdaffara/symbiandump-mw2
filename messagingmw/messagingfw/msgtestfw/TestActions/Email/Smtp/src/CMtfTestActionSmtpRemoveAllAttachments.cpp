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
// SmtpRemoveAllAttachments
// [Action Parameters]
// Session				<input>: Reference to the session.
// MsgId				<input>: Value of the message Id.
// AttachmentIdFlag		<input>: Flag to state if attachment is to be removed using attachment Id.
// [Action Description]
// Removes all the attachments of a specified message. If AttachmentIdFlag is set to ETrue
// attachments will be removed using attachment Id, else index value is used.
// [APIs Used]
// CMsvSession::GetEntryL
// CImEmailMessage::AttachmentManagerL
// MMsvAttachmentManager::RemoveAttachmentL
// MMsvAttachmentManager::AttachmentCountL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionSmtpRemoveAllAttachments.h"
#include "CMtfAsyncWaiter.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>
#include <cmsvattachment.h>
#include <miutmsg.h>

CMtfTestAction* CMtfTestActionSmtpRemoveAllAttachments::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSmtpRemoveAllAttachments* self = new(ELeave) CMtfTestActionSmtpRemoveAllAttachments(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSmtpRemoveAllAttachments::CMtfTestActionSmtpRemoveAllAttachments(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSmtpRemoveAllAttachments::~CMtfTestActionSmtpRemoveAllAttachments()
	{
	}

void CMtfTestActionSmtpRemoveAllAttachments::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSmtpRemoveAllAttachments);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TInt attachIdFlag = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2), 0);

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CImEmailMessage* emailMsg = CImEmailMessage::NewL(*entry);
	CleanupStack::PushL(emailMsg);
	
	MMsvAttachmentManager& manager = emailMsg->AttachmentManager();
	
	TInt attachmentCount = manager.AttachmentCount();
		
	for ( ; attachmentCount > 0 ; attachmentCount-- )
	{
	CMtfAsyncWaiter* waiter = CMtfAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	
	if( attachIdFlag == 1 )
		{
		CMsvAttachment* attachmentInfo = manager.GetAttachmentInfoL(0);
		CleanupStack::PushL(attachmentInfo);
		manager.RemoveAttachmentL(attachmentInfo->Id(), waiter->iStatus);
		CleanupStack::PopAndDestroy(attachmentInfo);
		}
	else
		{
		manager.RemoveAttachmentL(0, waiter->iStatus);
		}
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::PopAndDestroy(waiter);
	}
	
	CleanupStack::PopAndDestroy(2, entry); // emailMsg, entry

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSmtpRemoveAllAttachments);

	TestCase().ActionCompletedL(*this);
	}
