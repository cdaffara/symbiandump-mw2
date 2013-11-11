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
// SetDefaultSmsSettings
// [Action Parameters]
// HBufC& FilePath(s) <input>: Name of the file paths to be set as the SMS settings script for this test case.
// There can be more than one parameter, each identifying one SMS settings file
// [Action Description]
// Sets the default SMS settings files for the test case.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionSetDefaultSmsSettings.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <s32file.h>


CMtfTestAction* CMtfTestActionSetDefaultSmsSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetDefaultSmsSettings* self = new (ELeave) CMtfTestActionSetDefaultSmsSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSetDefaultSmsSettings::CMtfTestActionSetDefaultSmsSettings(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSetDefaultSmsSettings::~CMtfTestActionSetDefaultSmsSettings()
	{
	}

void CMtfTestActionSetDefaultSmsSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetDefaultSmsSettings);
	HBufC* paramFilePath = NULL;
	
	// Create a ConfigurationType object of type SMS settings
	CMtfConfigurationType* smsSettingsConfiguration = CMtfConfigurationType::NewL(CMtfConfigurationType::EMtfSmsSettings);
	CleanupStack::PushL(smsSettingsConfiguration);
	
	// loop through the parameters and add the files as configuration files to the object
	TInt numFiles = ActionParameters().Count();
	
	if(numFiles <= 0) // panic if we don't have any parameters
		TestCase().Panic(CMtfTestCase::EMtfMissingParameters);

	for (TInt i=0; i<numFiles; i++)
		{
		paramFilePath = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(i));

		smsSettingsConfiguration->AddConfigurationFilenameL(paramFilePath->Des());
		} 

	// Pop it, since the test case will take ownership of it.
	CleanupStack::Pop(smsSettingsConfiguration); // smsSettingsConfiguration
	
	TestCase().SetTestCaseDefaultConfigurationTypeL(smsSettingsConfiguration);
	 
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetDefaultSmsSettings);
	TestCase().ActionCompletedL(*this);
	}



