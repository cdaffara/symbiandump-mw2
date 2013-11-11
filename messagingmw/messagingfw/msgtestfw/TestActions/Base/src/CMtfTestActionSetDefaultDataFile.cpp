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
// SetDefaultDataFile
// [Action Parameters]
// FilePath <input>: Name of the Default File path to be set 
// [Action Description]
// Sets the default configuration file of type EMtfDataFile 
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionSetDefaultDataFile.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfConfigurationType.h"


CMtfTestAction* CMtfTestActionSetDefaultDataFile::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetDefaultDataFile* self = new (ELeave) CMtfTestActionSetDefaultDataFile(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	
//  does nothing
CMtfTestActionSetDefaultDataFile::CMtfTestActionSetDefaultDataFile(CMtfTestCase& aTestCase)
	:CMtfSynchronousTestAction(aTestCase)
	{
	}

// does nothing
CMtfTestActionSetDefaultDataFile::~CMtfTestActionSetDefaultDataFile()
	{
	}

void CMtfTestActionSetDefaultDataFile::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetDefaultDataFile);
	HBufC* paramFilePath = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(0));
	CMtfConfigurationType* configType=CMtfConfigurationType::NewL(CMtfConfigurationType::EMtfDataFile);
    CleanupStack::PushL(configType);
	configType->AddConfigurationFilenameL(*paramFilePath);
    CleanupStack::PopAndDestroy(configType);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetDefaultDataFile);
	TestCase().ActionCompletedL(*this);
	}
