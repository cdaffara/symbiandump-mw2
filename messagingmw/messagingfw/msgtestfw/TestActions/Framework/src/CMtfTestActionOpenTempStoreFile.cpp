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
// OpenTempStoreFile
// [Action Parameters]
// EntryId			<input>	: Value of the store entry Id to be opened.
// [Action Description]
// Opens a temporary store file for write which will replace the actual store associated with the entry id upon
// calling ReplaceFileStore.
// [APIs Used]
// CMsvClientStoreManager::OpenTempStoreFileL()
// RMsvServerSession::StoreManager()
// __ACTION_INFO_END__
// 
//

/**
 @file
*/
// 
#include "CMtfTestActionOpenTempStoreFile.h"
#include "MCLIENT.H"
#include "CMtfTestActionParameters.h"
#include "CMtfTestCase.h"

CMtfTestAction* CMtfTestActionOpenTempStoreFile::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionOpenTempStoreFile* self = new (ELeave) CMtfTestActionOpenTempStoreFile(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}

CMtfTestActionOpenTempStoreFile::CMtfTestActionOpenTempStoreFile(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

CMtfTestActionOpenTempStoreFile::~CMtfTestActionOpenTempStoreFile()
	{
	}

void CMtfTestActionOpenTempStoreFile::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionOpenTempStoreFile);
	// Obtain the input parameters
	TMsvId paramEntryId  = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	RMsvServerSession serverSession;
	serverSession.Connect(fs);
	CleanupClosePushL(serverSession);

	RFile file;	
	serverSession.OpenTempStoreFileL(paramEntryId,file);
	CleanupClosePushL(file);
	
	_LIT8(KOpenTempStoreFileTestData,"This is a test data for temporary store file"); 
	TInt error = file.Write(0,KOpenTempStoreFileTestData);
	if(error)
		{
		TestCase().ERR_PRINTF2(_L("Error writing to temporary store file , Error = %d"), error);
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TestCase().INFO_PRINTF1(_L("Test data successfully written to temporary store file"));	
		}

	// Close the handles 
	CleanupStack::PopAndDestroy(3,&fs);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionOpenTempStoreFile);
	TestCase().ActionCompletedL(*this);
	}

