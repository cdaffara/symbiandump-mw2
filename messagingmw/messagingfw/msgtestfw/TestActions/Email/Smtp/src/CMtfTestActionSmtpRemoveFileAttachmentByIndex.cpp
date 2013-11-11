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
// SmtpRemoveFileAttachmentByIndex
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// Index		  <input>: Index value of attachment to remove.
// ErrorCode	  <output>: (Optional) Returned error code, if not requested then code will
// leave if not KErrNone
// [Action Description]
// Removes the specified file attachment by its index value. The attachment
// is first checked to to ensure that it is a file attachment and then
// removes it.
// [APIs Used]
// CMsvSession::GetEntryL
// CImEmailMessage::AttachmentManagerL
// MMsvAttachmentManager::GetAttachmentInfoL
// MMsvAttachmentManager::RemoveAttachmentL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionSmtpRemoveFileAttachmentByIndex.h"
#include "CMtfAsyncWaiter.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>
#include <cmsvattachment.h>
#include <miutmsg.h>

CMtfTestAction* CMtfTestActionSmtpRemoveFileAttachmentByIndex::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSmtpRemoveFileAttachmentByIndex* self = new(ELeave) CMtfTestActionSmtpRemoveFileAttachmentByIndex(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSmtpRemoveFileAttachmentByIndex::CMtfTestActionSmtpRemoveFileAttachmentByIndex(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSmtpRemoveFileAttachmentByIndex::~CMtfTestActionSmtpRemoveFileAttachmentByIndex()
	{
	}

void CMtfTestActionSmtpRemoveFileAttachmentByIndex::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSmtpRemoveFileAttachmentByIndex);
	TRAPD(err, RunTestL());
	
	if( ActionParameters().Count() < 4 )
		{
		User::LeaveIfError(err);
		}
	else
		{
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(3));
		}
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSmtpRemoveFileAttachmentByIndex);
	TestCase().ActionCompletedL(*this); 
	}
	
void CMtfTestActionSmtpRemoveFileAttachmentByIndex::RunTestL()
	{
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TInt attachIndex = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CImEmailMessage* emailMsg = CImEmailMessage::NewL(*entry);
	CleanupStack::PushL(emailMsg);
	
	MMsvAttachmentManager& manager = emailMsg->AttachmentManager();
	
	CMsvAttachment* attachmentInfo = manager.GetAttachmentInfoL(attachIndex);
	CleanupStack::PushL(attachmentInfo);
	
	// First ensure that the attachment is a file attachment
	if( attachmentInfo->Type() != CMsvAttachment::EMsvFile )
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(attachmentInfo);
	
	CMtfAsyncWaiter* waiter = CMtfAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	
	manager.RemoveAttachmentL(attachIndex, waiter->iStatus);
	
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::PopAndDestroy(waiter);

	CleanupStack::PopAndDestroy(2, entry); // emailMsg, entry
	}


	
