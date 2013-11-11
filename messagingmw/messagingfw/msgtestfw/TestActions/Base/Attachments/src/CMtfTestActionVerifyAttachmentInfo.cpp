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
// VerifyAttachmentInfo
// [Action Parameters]
// AttachmentInfo <input>: Reference to the attachment info.
// FileName		  <input>: File name to compare against file name in AttachmentInfo.
// FileSize		  <input>: Size of the file in KB.
// MimeType		  <input>: Mime type of attachment file.
// [Action Description]
// Compares the attachment information against a inputs provided.
// [APIs Used]
// TDesC8::Compare
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionVerifyAttachmentInfo.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <cmsvattachment.h>

const TInt KMimeTypeLength = 50;

CMtfTestAction* CMtfTestActionVerifyAttachmentInfo::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionVerifyAttachmentInfo* self = new(ELeave) CMtfTestActionVerifyAttachmentInfo(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionVerifyAttachmentInfo::CMtfTestActionVerifyAttachmentInfo(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionVerifyAttachmentInfo::~CMtfTestActionVerifyAttachmentInfo()
	{
	}


void CMtfTestActionVerifyAttachmentInfo::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionVerifyAttachmentInfo);
	CMsvAttachment* attachmentInfo = ObtainParameterReferenceL<CMsvAttachment>(TestCase(),ActionParameters().Parameter(0));
	HBufC*	paramFileName   = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(1));
	TInt	paramFileSize   = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));
	HBufC*	paramMimeType   = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(3));

	
	TestCase().INFO_PRINTF1(_L("VerifyAttachmentInfo....."));

	TestCase().INFO_PRINTF2(_L("Attachment Name - %S"), &attachmentInfo->AttachmentName());
	TestCase().INFO_PRINTF2(_L("Size - %d"), attachmentInfo->Size());
	
	TBuf<KMimeTypeLength> mimeType;
	mimeType.Copy(attachmentInfo->MimeType());
	TestCase().INFO_PRINTF2(_L("MimeType - %S"), &mimeType);
	

	TestCase().INFO_PRINTF1(_L("With.."));

	TestCase().INFO_PRINTF2(_L("Attachment Name - %S"), paramFileName);
	TestCase().INFO_PRINTF2(_L("Size - %d"), paramFileSize);
	TestCase().INFO_PRINTF2(_L("MimeType - %S"), paramMimeType);

	if ( 
		(paramFileName->Compare(attachmentInfo->AttachmentName()) == 0) &&
		(paramMimeType->Compare(mimeType) == 0) &&
		(paramFileSize == attachmentInfo->Size())
	   )
		{		
		}
	else
		{
		User::Leave(KErrGeneral);
		}

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionVerifyAttachmentInfo);
	TestCase().ActionCompletedL(*this);
	}
