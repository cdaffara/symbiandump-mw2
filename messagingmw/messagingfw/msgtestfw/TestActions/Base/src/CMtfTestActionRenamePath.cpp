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
// RenamePath
// [Action Parameters]
// HBufC FilePath   <input>: Full path of the file or the folder to be renamed.
// HBufC FilePath   <input>: New file path.
// [Action Description]
// Renames the specified file or folder.
// [APIs Used]
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionRenamePath.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


CMtfTestAction* CMtfTestActionRenamePath::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionRenamePath* self = new (ELeave) CMtfTestActionRenamePath(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionRenamePath::CMtfTestActionRenamePath(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionRenamePath::~CMtfTestActionRenamePath()
	{
	}

void CMtfTestActionRenamePath::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionRenamePath);
	HBufC* paramPath = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(0));
	HBufC* paramNewPath = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(1));

	TPtrC pathToRename = paramPath->Des();
	TPtrC newPath = paramNewPath->Des();

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	TParse parseRenamePath;
	TParse parseNewPath;

	parseRenamePath.Set(pathToRename, NULL, NULL);
	parseNewPath.Set(newPath, NULL, NULL);

	TInt error;
	
	error = fs.Rename(parseRenamePath.FullName(), parseNewPath.FullName());
	
	if (!(error==KErrNone))
		{
		User::Leave(error);
		}
	CleanupStack::PopAndDestroy(&fs);  // fs
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionRenamePath);
	TestCase().ActionCompletedL(*this);
	}
