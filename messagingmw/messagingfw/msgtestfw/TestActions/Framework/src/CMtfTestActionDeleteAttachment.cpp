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
// DeleteAttachment
// [Action Parameters]
// EntryId			<input>: Value of the attachment entry Id to be deleted.
// FileName			<input>: Name of the attachment file name to be deleted.
// [Action Description]
// Deletes an attachment associated with the specified entry Id.
// [APIs Used]
// RMsvServerSession::DeleteAttachment()
// __ACTION_INFO_END__
// 
//

/**
 @file
*/
// 
#include "CMtfTestActionDeleteAttachment.h"
#include "MCLIENT.H"
#include "CMtfTestActionParameters.h"
#include "CMtfTestCase.h"


CMtfTestAction* CMtfTestActionDeleteAttachment::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteAttachment* self = new (ELeave) CMtfTestActionDeleteAttachment(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}

CMtfTestActionDeleteAttachment::CMtfTestActionDeleteAttachment(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

CMtfTestActionDeleteAttachment::~CMtfTestActionDeleteAttachment()
	{
	}

void CMtfTestActionDeleteAttachment::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteAttachment);
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

	TInt error = serverSession.DeleteAttachment(paramEntryId,*paramAttachmentFileName);
	
	if(error == KErrNone)
		{
		TestCase().INFO_PRINTF1(_L("Attachment file deleted successfully"));
		}
	else
		{
		TestCase().ERR_PRINTF2(_L("Error  deleting attachment file , Error = %d"),error);
		TestCase().SetTestStepResult(EFail);
		}

	//  close the handles 
	CleanupStack::PopAndDestroy(2,&fs);

	StoreParameterL<TInt>(TestCase(),error,ActionParameters().Parameter(2));

	TestCase().INFO_PRINTF1(_L("Test Action DeleteAttachment completed"));
	TestCase().ActionCompletedL(*this);
	}

