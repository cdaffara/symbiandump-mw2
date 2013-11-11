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
// SmtpAddEntryAttachment
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// EntryId        <input>: Entry Id of message to add as an attachment.
// AttachmentId   <output>: Value of the created attachment id.
// ErrorCode	  <output>: (Optional) Returned error code, if not requested then code will
// leave if not KErrNone
// [Action Description]
// Adds the specified message entry as an attachment to the specified message
// entry.
// [APIs Used]
// CMsvSession::GetEntryL
// CImEmailMessage::AttachmentManagerL
// CMsvAttachment::NewL
// MMsvAttachmentManager::AddEntryAsAttachmentL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionSmtpAddEntryAttachment.h"
#include "CMtfAsyncWaiter.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <cmsvattachment.h>
#include <mmsvattachmentmanager.h>
#include <miutmsg.h>

CMtfTestAction* CMtfTestActionSmtpAddEntryAttachment::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSmtpAddEntryAttachment* self = new(ELeave) CMtfTestActionSmtpAddEntryAttachment(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSmtpAddEntryAttachment::CMtfTestActionSmtpAddEntryAttachment(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSmtpAddEntryAttachment::~CMtfTestActionSmtpAddEntryAttachment()
	{
	}

void CMtfTestActionSmtpAddEntryAttachment::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSmtpAddEntryAttachment);
	TRAPD(err, RunTestL());
	
	if( ActionParameters().Count() < 5 )
		{
		User::LeaveIfError(err);
		}
	else
		{
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(4));
		}
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSmtpAddEntryAttachment);
	TestCase().ActionCompletedL(*this); 
	}

void CMtfTestActionSmtpAddEntryAttachment::RunTestL()
	{
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvId attachmentMessageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(2));

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CImEmailMessage* emailMsg = CImEmailMessage::NewL(*entry);
	CleanupStack::PushL(emailMsg);
	
	CMtfAsyncWaiter* waiter = CMtfAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvMessageEntry);
	CleanupStack::PushL(attachment);
	
	TMsvEntry attachmentEntry;
	TMsvId attachmentServiceEntry;
	User::LeaveIfError(paramSession->GetEntry(attachmentMessageEntry, attachmentServiceEntry, attachmentEntry));
	attachment->SetSize(attachmentEntry.iSize);

	MMsvAttachmentManager& manager = emailMsg->AttachmentManager();
	
	manager.AddEntryAsAttachmentL(attachmentMessageEntry, attachment, waiter->iStatus);
	CleanupStack::Pop(attachment); // ownership passed to manager
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::PopAndDestroy(waiter);
	
	attachment = NULL;
	
	TInt attachmentCount = manager.AttachmentCount();
	attachment = manager.GetAttachmentInfoL(attachmentCount - 1);
	CleanupStack::PushL(attachment);
	
	TMsvAttachmentId attachmentId = attachment->Id();
	
	CleanupStack::PopAndDestroy(3, entry); // emailMsg, entry
	
	StoreParameterL<TMsvAttachmentId>(TestCase(),attachmentId,ActionParameters().Parameter(3));
	}

