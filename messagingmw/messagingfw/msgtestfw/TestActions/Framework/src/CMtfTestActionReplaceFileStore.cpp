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
// ReplaceFileStore
// [Action Parameters]
// EntryId			<input>	: Value of the store entry Id to be opened.
// [Action Description]
// Allows the message store classes to read the store file for a particular message entry.
// [APIs Used]
// RMsvServerSession::ReplaceFileStoreL()
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionReplaceFileStore.h"
#include "MCLIENT.H"
#include "CMtfTestActionParameters.h"
#include "CMtfTestCase.h"

CMtfTestAction* CMtfTestActionReplaceFileStore::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionReplaceFileStore* self = new (ELeave) CMtfTestActionReplaceFileStore(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}

CMtfTestActionReplaceFileStore::CMtfTestActionReplaceFileStore(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

CMtfTestActionReplaceFileStore::~CMtfTestActionReplaceFileStore()
	{
	}

void CMtfTestActionReplaceFileStore::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionReplaceFileStore);
	// Obtain the input parameters
	TMsvId paramEntryId  = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));
	// Create file session at the client side
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	RMsvServerSession serverSession;
	serverSession.Connect(fs);
	CleanupClosePushL(serverSession);

	serverSession.ReplaceFileStoreL(paramEntryId);
	
	CleanupStack::PopAndDestroy(2,&fs);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionReplaceFileStore);
	TestCase().ActionCompletedL(*this);
	}

