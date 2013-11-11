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
// IsUsingHardcodedDefaultSettings
// [Action Parameters]
// Session        <input>: Reference to the session.
// DefaultUsed    <input>: 1 if settings should be default settings, 0 otherwise.
// ErrorCode	  <output>: (Optional) Returned error code, if not requested then code will
// leave if not KErrNone
// [Action Description]
// Compares the default messaging settings to the current settings (i.e. those stored in msgs.ini).
// [APIs Used]
// MessageServer::CurrentDriveL()
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionIsUsingHardcodedDefaultSettings.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"


CMtfTestAction* CMtfTestActionIsUsingHardcodedDefaultSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionIsUsingHardcodedDefaultSettings* self = new(ELeave) CMtfTestActionIsUsingHardcodedDefaultSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

CMtfTestActionIsUsingHardcodedDefaultSettings::CMtfTestActionIsUsingHardcodedDefaultSettings(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionIsUsingHardcodedDefaultSettings::~CMtfTestActionIsUsingHardcodedDefaultSettings()
	{
	}

void CMtfTestActionIsUsingHardcodedDefaultSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionIsUsingHardcodedDefaultSettings);

	CMsvSession* paramSession	= ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));

	TInt defaultUsed			= ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1));

	TDriveUnit currentDrive = MessageServer::CurrentDriveL(paramSession->FileSession());
	TInt intCurrentDrive = currentDrive;
	TDriveUnit defaultDrive = 2;	// The C: drive
	TInt intDefaultDrive = defaultDrive;

	TestCase().INFO_PRINTF1(_L("Current drive:"));
	TestCase().INFO_PRINTF1(currentDrive.Name());

	TestCase().INFO_PRINTF1(_L("Default drive:"));
	TestCase().INFO_PRINTF1(defaultDrive.Name());

	if (intCurrentDrive == intDefaultDrive)
		{
		TestCase().INFO_PRINTF1(_L("Current drive is the default drive."));
		if (!defaultUsed)
			{
			TestCase().SetTestStepResult(EFail);
			}
		}
	else
		{
		TestCase().INFO_PRINTF1(_L("Current drive is NOT the default drive."));
		if (defaultUsed)
			{
			TestCase().SetTestStepResult(EFail);
			}
		}
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionIsUsingHardcodedDefaultSettings);

	TestCase().ActionCompletedL(*this);
	}
