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
// SetDefaultConfiguration
// [Action Parameters]
// CMtfConfigurationType::TMtfConfigurationType 	ConfigType <input>: Configuration type to set.
// HBufC&	FilePath(s) <input>: Name of the file paths to be set as the input file(s)  
// for the specified configuration type for this test case.
// There can be more than one parameter, each identifying one input file
// [Action Description]
// Sets the default input file(s) for the specified configuration type for the test case.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionSetDefaultConfiguration.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <s32file.h>


CMtfTestAction* CMtfTestActionSetDefaultConfiguration::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetDefaultConfiguration* self = new (ELeave) CMtfTestActionSetDefaultConfiguration(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSetDefaultConfiguration::CMtfTestActionSetDefaultConfiguration(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSetDefaultConfiguration::~CMtfTestActionSetDefaultConfiguration()
	{
	}

void CMtfTestActionSetDefaultConfiguration::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetDefaultConfiguration);
	HBufC* paramFilePath = NULL;

	/*
	TSmsDataCodingScheme::TSmsAlphabet characterSet = ObtainValueParameterL<TSmsDataCodingScheme::TSmsAlphabet>(aTestCase, stringPtr);
	*/
	CMtfConfigurationType::TMtfConfigurationType configType = ObtainValueParameterL<CMtfConfigurationType::TMtfConfigurationType>(TestCase(), ActionParameters().Parameter(0));
	
	// Create a ConfigurationType object of the specified type
	CMtfConfigurationType* configTypeRecord = CMtfConfigurationType::NewL(configType);
	CleanupStack::PushL(configTypeRecord);
	
	// loop through the parameters and add the files as configuration files to the object
	TInt numFiles = ActionParameters().Count() - 1;
	
	if(numFiles <= 0) // panic if we don't have any parameters
		TestCase().Panic(CMtfTestCase::EMtfMissingParameters);

	for (TInt i=1; i<=numFiles; i++)
		{
		paramFilePath = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(i));
		configTypeRecord->AddConfigurationFilenameL(paramFilePath->Des());
		} 

	// Pop it, since the test case will take ownership of it.
	CleanupStack::Pop(configTypeRecord); // configTypeRecord
	
	TestCase().SetTestCaseDefaultConfigurationTypeL(configTypeRecord);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetDefaultConfiguration );
	TestCase().ActionCompletedL(*this);
	}



