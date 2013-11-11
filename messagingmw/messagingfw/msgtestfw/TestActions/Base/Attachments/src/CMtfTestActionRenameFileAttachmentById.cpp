// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// RenameFileAttachmentById
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// AttachmentId	  <input>: Attachment Id of attachment to remove.
// ErrorCode	  <output>: (Optional) Returned error code, if not requested then code will
// leave if not KErrNone
// [Action Description]
// Removes the specified file attachment by its attachment Id. The attachment
// is first checked to to ensure that it is a file attachment and then
// removes it.
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvStore::ReadStoreL
// CMsvStore::AttachmentManagerL
// TODO
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionRenameFileAttachmentById.h"
#include "CMtfAsyncWaiter.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>
#include <cmsvattachment.h>

CMtfTestAction* CMtfTestActionRenameFileAttachmentById::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionRenameFileAttachmentById* self = new(ELeave) CMtfTestActionRenameFileAttachmentById(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionRenameFileAttachmentById::CMtfTestActionRenameFileAttachmentById(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionRenameFileAttachmentById::~CMtfTestActionRenameFileAttachmentById()
	{
	}

void CMtfTestActionRenameFileAttachmentById::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionRenameFileAttachmentById);
	TRAPD(err, RunTestL());
	
	if( ActionParameters().Count() < 4 )
		{
		User::LeaveIfError(err);
		}
	else
		{
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(3));
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionRenameFileAttachmentById);	
	TestCase().ActionCompletedL(*this); 
	}

void CMtfTestActionRenameFileAttachmentById::RunTestL()
	{
	_LIT(KTxtFileName, "C:\\Private\\1000484b\\Mail2\\00001001_S\\0\\00100002_F\\sample1.jpg");
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvAttachmentId originalAttachId = ObtainValueParameterL<TMsvAttachmentId>(TestCase(),ActionParameters().Parameter(2));
	TMsvAttachmentId renameAttachId = ObtainValueParameterL<TMsvAttachmentId>(TestCase(),ActionParameters().Parameter(3));

	//Obtains the entry details.
	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	//Read the entry details from the store.
	CMsvStore* store = entry->EditStoreL();
	CleanupStack::PushL(store);
	
	//Refer to the attachment info from the store.
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	
	//Obtains the attachment info 
	CMsvAttachment* attachInfo = manager.GetAttachmentInfoL(renameAttachId);
	CleanupStack::PushL(attachInfo);
	
	// First ensure that the attachment is a file attachment
	if( attachInfo->Type() != CMsvAttachment::EMsvFile )
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(attachInfo);
	
	CMtfAsyncWaiter* waiter = CMtfAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	manager.RenameAttachmentL(originalAttachId,KTxtFileName,waiter->iStatus);
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	TMsvAttachmentId newattachmentId = attachInfo->Id();
	TestCase().TEST(newattachmentId == renameAttachId);
	CleanupStack::PopAndDestroy(waiter);
	
	store->CommitL();
	CleanupStack::PopAndDestroy(2, entry); // store, entry
	}


	
