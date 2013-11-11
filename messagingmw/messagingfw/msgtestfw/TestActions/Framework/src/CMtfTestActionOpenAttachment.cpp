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
// OpenAttachment
// [Action Parameters]
// EntryId			<input>	: Value of the attachment entry Id to be opened.
// FileName			<input> : Name of the attachment file name to be opened
// [Action Description]
// Allows clients to view/read the attachment
// [APIs Used]
// RMsvServerSession::OpenAttachment
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionOpenAttachment.h"
#include "MCLIENT.H"
#include "CMtfTestActionParameters.h"
#include "CMtfTestCase.h"

CMtfTestAction* CMtfTestActionOpenAttachment::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionOpenAttachment* self = new (ELeave) CMtfTestActionOpenAttachment(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}

CMtfTestActionOpenAttachment::CMtfTestActionOpenAttachment(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

CMtfTestActionOpenAttachment::~CMtfTestActionOpenAttachment()
	{
	}

void CMtfTestActionOpenAttachment::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionOpenAttachment);
	// Obtain the input parameters
	TMsvId paramEntryId  = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));
	HBufC* paramAttachmentFileName = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(1));

	// Create file session at the client side
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	RMsvServerSession serverSession;
	serverSession.Connect(fs);
	CleanupClosePushL(serverSession);
		
	RFile file;
	serverSession.OpenAttachmentL(paramEntryId,*paramAttachmentFileName,file);
	CleanupClosePushL(file);
	
	TBuf8<100> bufFileData(_L8("")); // To read test data
	TInt error = file.Read(bufFileData,bufFileData.MaxLength());
	if(error)
		{
		TestCase().ERR_PRINTF2(_L("Error Reading Client file , Error = %d"), error);
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TestCase().INFO_PRINTF1(_L("Test Data successfully read from Attachment file"));	
		}

	CleanupStack::PopAndDestroy(3,&fs);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionOpenAttachment);
	TestCase().ActionCompletedL(*this);
	}
