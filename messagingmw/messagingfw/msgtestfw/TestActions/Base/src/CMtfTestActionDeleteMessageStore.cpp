// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// DeleteMessageStore
// [Action Parameters]
// StorePath <input>: Name of the messaging Store path to be deleted.
// [Action Description]
// Deletes specified message Store.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionDeleteMessageStore.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <s32file.h>


CMtfTestAction* CMtfTestActionDeleteMessageStore::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteMessageStore* self = new (ELeave) CMtfTestActionDeleteMessageStore(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeleteMessageStore::CMtfTestActionDeleteMessageStore(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionDeleteMessageStore::~CMtfTestActionDeleteMessageStore()
	{
	}


void CMtfTestActionDeleteMessageStore::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteMessageStore);
	HBufC* paramStorePath = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(0));

	TPtrC Store = paramStorePath->Des();
	RFs fs;
	fs.Connect();
	CDictionaryFileStore* store = CDictionaryFileStore::SystemLC(fs);
	const TUid KUidMsvMessageDriveStream = {0x1000163E};
	if (store->IsPresentL(KUidMsvMessageDriveStream))
		{
		store->RemoveL(KUidMsvMessageDriveStream); 
		store->CommitL();
		}
	CleanupStack::PopAndDestroy(store); 

	CFileMan* fileMan = CFileMan::NewL(fs); 
	CleanupStack::PushL(fileMan);
	TInt error;
	error = fileMan->RmDir(Store); 
	error = fs.RmDir(Store); 
	if (!(error==KErrNotFound||error==KErrNone))
		{
		User::Leave(KErrAccessDenied);
		}
	CleanupStack::PopAndDestroy(fileMan);
	fs.Close();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteMessageStore);
	TestCase().ActionCompletedL(*this);
	}


