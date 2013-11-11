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
// DeletePath
// [Action Parameters]
// HBufC FilePath   <input>: Full path of the file or the folder to be deleted.
// [Action Description]
// Deletes the specified file or folder.
// [APIs Used]
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionDeletePath.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


CMtfTestAction* CMtfTestActionDeletePath::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeletePath* self = new (ELeave) CMtfTestActionDeletePath(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeletePath::CMtfTestActionDeletePath(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionDeletePath::~CMtfTestActionDeletePath()
	{
	}

void CMtfTestActionDeletePath::ExecuteActionL()
	{
	TestCase().INFO_PRINTF1(_L("Test Action DeletePath start..."));
	HBufC* paramPath = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(0));

	TPtrC pathToDelete = paramPath->Des();

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	TParse parse;

	parse.Set(pathToDelete, NULL, NULL);
	TInt error;
	if(parse.NamePresent())
		{
		// specified a file to delete
		error = fs.Delete(parse.FullName());
		}
	else
		{
		// specified a directoy to delete
		error = fs.RmDir(parse.DriveAndPath()); 
		}
	if (!(error==KErrNotFound||error==KErrNone))
		{
		User::Leave(error);
		}
	CleanupStack::PopAndDestroy(&fs);  // fs
	TestCase().INFO_PRINTF1(_L("Test Action DeletePath completed."));
	TestCase().ActionCompletedL(*this);
	}


