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
// DeleteFileStore
// [Action Parameters]
// EntryId       <input>: Value of the store entry id to be deleted
// [Action Description]
// Deletes the store file and the temporary store file if one exists.
// [APIs Used]
// RMsvServerSession::DeleteFileStoreL()
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionDeleteFileStore.h"
#include "MCLIENT.H"
#include "CMtfTestActionParameters.h"
#include "CMtfTestCase.h"

CMtfTestAction* CMtfTestActionDeleteFileStore::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteFileStore* self = new (ELeave) CMtfTestActionDeleteFileStore(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}

CMtfTestActionDeleteFileStore::CMtfTestActionDeleteFileStore(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

CMtfTestActionDeleteFileStore::~CMtfTestActionDeleteFileStore()
	{
	}

void CMtfTestActionDeleteFileStore::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteFileStore);
	// Obtain the input parameters
	TMsvId paramEntryId  = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	RMsvServerSession serverSession;
	serverSession.Connect(fs);
	CleanupClosePushL(serverSession);

	serverSession.DeleteFileStoreL(paramEntryId);

	CleanupStack::PopAndDestroy(2,&fs);
	
	TestCase().ActionCompletedL(*this);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteFileStore);
	}

