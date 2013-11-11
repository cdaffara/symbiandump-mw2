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
// AddAttachmentAsLink
// [Action Parameters]
// Store			<input>: Reference to the store.
// FilePath			<input>: File path of the linked attachment file.
// MimeType			<input>: Mime-type of the attachment.
// AttachmentId		<output>: Value of the created attachment id.
// ErrorCode	  <output>: (Optional) Returned error code, if not requested then code will
// leave if not KErrNone
// [Action Description]
// Adds the specified file as a linked attachment to the specified message
// entry.
// [APIs Used]
// CMsvStore::AttachmentManagerL
// CMsvAttachment::NewL
// CMsvAttachment::SetMimeType
// MMsvAttachmentManager::AddLinkedAttachmentL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionAddAttachmentAsLink.h"
#include "CMtfAsyncWaiter.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <cmsvattachment.h>
#include <mmsvattachmentmanager.h>

CMtfTestAction* CMtfTestActionAddAttachmentAsLink::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionAddAttachmentAsLink* self = new(ELeave) CMtfTestActionAddAttachmentAsLink(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionAddAttachmentAsLink::CMtfTestActionAddAttachmentAsLink(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionAddAttachmentAsLink::~CMtfTestActionAddAttachmentAsLink()
	{
	}

void CMtfTestActionAddAttachmentAsLink::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionAddAttachmentAsLink);
	TRAPD(err, RunTestL());
	
	if( ActionParameters().Count() < 5 )
		{
		User::LeaveIfError(err);
		}
	else
		{
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(4));
		}
	
	TestCase().INFO_PRINTF1(_L("CMtfTestActionAddAttachmentAsLink completed....."));

	TestCase().ActionCompletedL(*this); 
	}

void CMtfTestActionAddAttachmentAsLink::RunTestL()
	{
	CMsvStore* paramStore = ObtainParameterReferenceL<CMsvStore>(TestCase(),ActionParameters().Parameter(0));
	HBufC* paramFilePath   = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(1));
	HBufC8* paramMimeType = ObtainParameterReferenceL<HBufC8>(TestCase(),ActionParameters().Parameter(2));
	
	
	CMtfAsyncWaiter* waiter = CMtfAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvLinkedFile);
	CleanupStack::PushL(attachment);
	
	attachment->SetMimeTypeL(*paramMimeType);
	
	TParse fileNameParser;
	User::LeaveIfError(fileNameParser.Set(*paramFilePath, NULL, NULL));
	attachment->SetAttachmentNameL(fileNameParser.NameAndExt());
	
	MMsvAttachmentManager& manager = paramStore->AttachmentManagerL();
	
	manager.AddLinkedAttachmentL(*paramFilePath, attachment, waiter->iStatus);
	CleanupStack::Pop(attachment); // ownership passed to manager

	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::PopAndDestroy(waiter);
	
	TMsvAttachmentId attachmentId = attachment->Id();
	
	paramStore->CommitL();
	
	StoreParameterL<TMsvAttachmentId>(TestCase(),attachmentId,ActionParameters().Parameter(3));	
	}
