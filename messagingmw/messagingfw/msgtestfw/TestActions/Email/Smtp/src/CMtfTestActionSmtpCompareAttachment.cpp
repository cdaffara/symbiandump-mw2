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
// SmtpCompareAttachment
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// AttachmentId	  <input>: Attachment ID to compare.
// DataFilePath   <input>: File path of data to compare attachment to.
// [Action Description]
// Compares the data from a file attachment against a file.
// [APIs Used]
// CMsvSession::GetEntryL
// CImEmailMessage::AttachmentManagerL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionSmtpCompareAttachment.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>
#include <miutmsg.h>

CMtfTestAction* CMtfTestActionSmtpCompareAttachment::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSmtpCompareAttachment* self = new(ELeave) CMtfTestActionSmtpCompareAttachment(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSmtpCompareAttachment::CMtfTestActionSmtpCompareAttachment(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSmtpCompareAttachment::~CMtfTestActionSmtpCompareAttachment()
	{
	}


void CMtfTestActionSmtpCompareAttachment::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSmtpCompareAttachment);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvAttachmentId attachId = ObtainValueParameterL<TMsvAttachmentId>(TestCase(),ActionParameters().Parameter(2));
	HBufC* dataFilePath   = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(3));

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CImEmailMessage* emailMsg = CImEmailMessage::NewL(*entry);
	CleanupStack::PushL(emailMsg);
	
	MMsvAttachmentManager& manager = emailMsg->AttachmentManager();
	
	RFile fileAttachment = manager.GetAttachmentFileL(attachId);
	CleanupClosePushL(fileAttachment);
	
	CompareFileL(fileAttachment, *dataFilePath);
	
	CleanupStack::PopAndDestroy(&fileAttachment);
	
	CleanupStack::PopAndDestroy(2, entry); // emailMsg, entry

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSmtpCompareAttachment);
	TestCase().ActionCompletedL(*this);
	}

void CMtfTestActionSmtpCompareAttachment::CompareFileL(RFile& aAttachment, const TDesC& aDataFilePath)
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
	
