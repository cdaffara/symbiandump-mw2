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
// CompareAttachment
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// AttachmentId	  <input>: Attachment ID to compare.
// DataFilePath   <input>: File path of data to compare attachment to.
// [Action Description]
// Compares the data from a file attachment against a file.
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvStore::ReadStoreL
// CMsvStore::AttachmentManagerL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionCompareAttachment.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>

CMtfTestAction* CMtfTestActionCompareAttachment::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCompareAttachment* self = new(ELeave) CMtfTestActionCompareAttachment(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCompareAttachment::CMtfTestActionCompareAttachment(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCompareAttachment::~CMtfTestActionCompareAttachment()
	{
	}


void CMtfTestActionCompareAttachment::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCompareAttachment);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvAttachmentId attachId = ObtainValueParameterL<TMsvAttachmentId>(TestCase(),ActionParameters().Parameter(2));
	HBufC* dataFilePath   = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(3));

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CMsvStore* store = entry->ReadStoreL();
	CleanupStack::PushL(store);
	
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	
	RFile fileAttachment = manager.GetAttachmentFileL(attachId);
	CleanupClosePushL(fileAttachment);
	
	CompareFileL(fileAttachment, *dataFilePath);
	
	CleanupStack::PopAndDestroy(&fileAttachment);
	
	CleanupStack::PopAndDestroy(2, entry); // store, entry
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCompareAttachment);
	TestCase().ActionCompletedL(*this);
	}

void CMtfTestActionCompareAttachment::CompareFileL(RFile& aAttachment, const TDesC& aDataFilePath)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	RFile dataFile;
	User::LeaveIfError(dataFile.Open(fs, aDataFilePath, EFileRead|EFileShareReadersOnly));
	CleanupClosePushL(fs);
	
	TInt fileSize = 0;
	
	User::LeaveIfError(dataFile.Size(fileSize));
	HBufC8* dataBuffer = HBufC8::NewLC(fileSize);
	TPtr8 bufferPtr(dataBuffer->Des());
	
	User::LeaveIfError(aAttachment.Size(fileSize));
	HBufC8* attachmentData = HBufC8::NewLC(fileSize);
	TPtr8 attachmentPtr(attachmentData->Des());
	
	TInt compare = attachmentPtr.Compare(bufferPtr);
	if( compare != 0 )
		User::Leave(KErrCorrupt);
	
	CleanupStack::PopAndDestroy(4, &fs); // attachmentData, dataBuffer, dataFile, fs
	}
	
