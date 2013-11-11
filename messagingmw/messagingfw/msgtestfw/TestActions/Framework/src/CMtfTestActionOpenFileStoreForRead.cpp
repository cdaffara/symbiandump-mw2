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
// OpenFileStoreForRead
// [Action Parameters]
// EntryId			<input>	: Value of the store entry Id to be opened.
// [Action Description]
// Allows the message store classes to read the store file for a particular message entry.
// [APIs Used]
// RMsvServerSession::OpenFileStoreForReadL()
// __ACTION_INFO_END__
// 
//

/**
 @file
*/
// 
#include "CMtfTestActionOpenFileStoreForRead.h"
#include "MCLIENT.H"
#include "CMtfTestActionParameters.h"
#include "CMtfTestCase.h"

CMtfTestAction* CMtfTestActionOpenFileStoreForRead::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionOpenFileStoreForRead* self = new (ELeave) CMtfTestActionOpenFileStoreForRead(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}

CMtfTestActionOpenFileStoreForRead::CMtfTestActionOpenFileStoreForRead(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

CMtfTestActionOpenFileStoreForRead::~CMtfTestActionOpenFileStoreForRead()
	{
	}

void CMtfTestActionOpenFileStoreForRead::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionOpenFileStoreForRead);
	// Obtain the input parameters
	TMsvId paramEntryId  = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));
	TBool paramFileStoreExists  = ObtainValueParameterL<TBool>(TestCase(),ActionParameters().Parameter(1));
	// Create file session at the client side
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	RMsvServerSession serverSession;
	serverSession.Connect(fs);
	CleanupClosePushL(serverSession);
	
	RFile file;
	TInt error  = serverSession.OpenFileStoreForRead(paramEntryId,file);
	if(error)
		{
		TestCase().ERR_PRINTF2(_L("Error opening the store file , Error = %d"), error);
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TestCase().INFO_PRINTF1(_L("File opened successfully"));	
		}
	CleanupClosePushL(file);
	
	TBuf8<100> bufFileData(_L8("")); // To read test data
	error = file.Read(bufFileData,bufFileData.MaxLength());
	if(error)
		{
		TestCase().ERR_PRINTF2(_L("Error Reading store file , Error = %d"), error);
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TestCase().INFO_PRINTF1(_L("Test Data successfully read from store file"));	
		}

	CleanupStack::PopAndDestroy(3,&fs);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionOpenFileStoreForRead);
	TestCase().ActionCompletedL(*this);
	}

