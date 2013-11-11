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
// CreateAttachmentForWrite
// [Action Parameters]
// EntryId       <input>: entry id for which attachment needs to be created.
// FileName		 <input>: Name of the attachment that will be created.
// [Action Description]
// Creates a new empty file and allows clients to stream data to it. The empty file is
// passed back to the client using an opened file handle. 
// [APIs Used]
// RMsvServerssion::CreateAttachmentForWriteL()
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionCreateAttachmentForWrite.h"
#include "MCLIENT.H"
#include "CMtfTestActionParameters.h"
#include "CMtfTestCase.h"

CMtfTestAction* CMtfTestActionCreateAttachmentForWrite::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateAttachmentForWrite* self = new (ELeave) CMtfTestActionCreateAttachmentForWrite(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}

CMtfTestActionCreateAttachmentForWrite::CMtfTestActionCreateAttachmentForWrite(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

void CMtfTestActionCreateAttachmentForWrite::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateAttachmentForWrite);
	// Obtain the input parameters
	TMsvId paramEntryId  = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));
	HBufC* paramAttachmentFileName = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(1));

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	RMsvServerSession serverSession;
	serverSession.Connect(fs);
	CleanupClosePushL(serverSession);

	RFile file;
	TDes des = paramAttachmentFileName->Des();
	TBool isFileNameChanged = serverSession.CreateAttachmentForWriteL( paramEntryId, des, file );
	CleanupClosePushL(file);
	if(isFileNameChanged)
		{
		TestCase().INFO_PRINTF1(_L("File name changed as the total file path is more than maximum file length"));
		TestCase().INFO_PRINTF2(_L("Changed File Name %s"),paramAttachmentFileName->Des().Ptr());
		}
	else
		{
		TestCase().INFO_PRINTF1(_L("File name not changed "));
		}	
	
	// Write some test data into attachment file
	_LIT8(KTestAttachmentData,"This is some test to data be written in attachment file");
	TInt error = file.Write(KTestAttachmentData);
	if(error == KErrNone)
		{
		TestCase().INFO_PRINTF1(_L("Test Data Written to Attachment file successfully"));
		}
	else
		{
		TestCase().ERR_PRINTF2(_L("Error  writing to the attachment file , Error = %d"),error);
		TestCase().SetTestStepResult(EFail);
		}

	// Close the handles 
	CleanupStack::PopAndDestroy(3,&fs);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateAttachmentForWrite);
	TestCase().ActionCompletedL(*this);
	}

