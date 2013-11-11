// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// GetAttachmentFileForWriteFromId
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// AttachmentId	  <input>: Attachment ID of attachment to retrieve.
// [Action Description]
// Gets the attachment file from the specified message id
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvStore::ReadStoreL
// CMsvStore::AttachmentManagerL
// MMsvAttachmentManager::GetAttachmentFileForWriteL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionGetAttachmentFileForWriteFromId.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>

CMtfTestAction* CMtfTestActionGetAttachmentFileForWriteFromId::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetAttachmentFileForWriteFromId* self = new(ELeave) CMtfTestActionGetAttachmentFileForWriteFromId(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionGetAttachmentFileForWriteFromId::CMtfTestActionGetAttachmentFileForWriteFromId(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionGetAttachmentFileForWriteFromId::~CMtfTestActionGetAttachmentFileForWriteFromId()
	{
	}

void CMtfTestActionGetAttachmentFileForWriteFromId::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetAttachmentFileForWriteFromId);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvAttachmentId attachId = ObtainValueParameterL<TMsvAttachmentId>(TestCase(),ActionParameters().Parameter(2));

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CMsvStore* store = entry->EditStoreL();
	CleanupStack::PushL(store);
	
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	
	RFile fileAttachment = manager.GetAttachmentFileForWriteL(attachId);
	CleanupClosePushL(fileAttachment);
	
	// read the contents of the file, and write them straight back
	TInt fileSize = 0;
	User::LeaveIfError(fileAttachment.Size(fileSize));
	
	HBufC8* dataBuffer = HBufC8::NewLC(fileSize);
	TPtr8 bufferPtr(dataBuffer->Des());
	
	User::LeaveIfError(fileAttachment.Read(bufferPtr));
	User::LeaveIfError(fileAttachment.Write(bufferPtr));
	
	CleanupStack::PopAndDestroy(4, entry); // dataBuffer, fileAttachment, store, entry

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetAttachmentFileForWriteFromId);
	TestCase().ActionCompletedL(*this);
	}
	

