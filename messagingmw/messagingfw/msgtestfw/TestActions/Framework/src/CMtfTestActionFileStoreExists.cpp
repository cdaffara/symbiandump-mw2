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
// CreateAttachment
// [Action Parameters]
// EntryId       <input>: Value of the entry id for which we 
// [Action Description]
// Creates an attachment from the client's data cage to the message store.
// [APIs Used]
// RMsvServerSession::CreateAttachmentL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/
// 
#include "CMtfTestActionFileStoreExists.h"
#include "MCLIENT.H"
#include "CMtfTestActionParameters.h"
#include "CMtfTestCase.h"

CMtfTestAction* CMtfTestActionFileStoreExists::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionFileStoreExists* self = new (ELeave) CMtfTestActionFileStoreExists(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}

CMtfTestActionFileStoreExists::CMtfTestActionFileStoreExists(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

CMtfTestActionFileStoreExists::~CMtfTestActionFileStoreExists()
	{
	}

void CMtfTestActionFileStoreExists::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionFileStoreExists);
	// Obtain the input parameters
	TMsvId paramEntryId  = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));

	// Create file session at the client side
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	RMsvServerSession serverSession;
	serverSession.Connect(fs);
	CleanupClosePushL(serverSession);

	TBool fileStoreExists = serverSession.FileStoreExistsL(paramEntryId);
	
	if(fileStoreExists)
		{
		TestCase().INFO_PRINTF1(_L("File store exists"));		
		}	
	else
		{
		TestCase().INFO_PRINTF1(_L("File store does not exist"));
		}

	// Close the handles 
	CleanupStack::PopAndDestroy(2,&fs);

	StoreParameterL<TBool>(TestCase(),fileStoreExists,ActionParameters().Parameter(1));
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionFileStoreExists);
	TestCase().ActionCompletedL(*this);
	}

