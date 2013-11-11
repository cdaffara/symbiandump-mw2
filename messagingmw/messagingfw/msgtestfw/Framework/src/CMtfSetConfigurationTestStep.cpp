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
//

/**
 @file
*/

#include "CMtfSetConfigurationTestStep.h"
#include "CMtfConfigurationType.h"
#include "CMtfTestCase.h"
#include "CMtfTestServer.h"
#include "CMtfScriptedTestCase.h"

_LIT(KMtfConfigurationParameters,"configurationParams");

CMtfSetConfigurationTestStep* CMtfSetConfigurationTestStep::NewL(CMtfTestServer& aTestServer)
{
	CMtfSetConfigurationTestStep* self=new(ELeave) CMtfSetConfigurationTestStep(aTestServer);
	return self;
}
 
CMtfSetConfigurationTestStep::CMtfSetConfigurationTestStep(CMtfTestServer& aTestServer)
:iTestServer(aTestServer)
{
}
		
CMtfSetConfigurationTestStep::~CMtfSetConfigurationTestStep() 
{
}

TVerdict CMtfSetConfigurationTestStep::doTestStepPreambleL()  
{
	return TestStepResult();
}
	
/** Obtain the test step parameters. The first parameter is the configuration type name,
the second parameter is the name of the file. There must be at least two parameters. */
TVerdict CMtfSetConfigurationTestStep::doTestStepL()
{
	TPtrC parameter;
	
	if (!GetStringFromConfig(ConfigSection(),KMtfConfigurationParameters,parameter))
	{
		User::Leave(KErrGeneral);
	}
	
	TLex parser(parameter);
	parser.SkipSpace();
		
	if (parser.Eos())
	{
		// type parameter is missing
		User::Leave(KErrGeneral);
	}
		
	_LIT(KCMtfConfigurationType,"CMtfConfigurationType::TMtfConfigurationType::");
	TPtrC nextToken = parser.NextToken();
	TInt length = KCMtfConfigurationType().Length() + nextToken.Length();
	HBufC* enumerator = HBufC::NewLC(length);
	enumerator->Des().Append(KCMtfConfigurationType);
	enumerator->Des().Append(nextToken);
		
	// dummy test case used in calling ObtainValueParameter
	// this only works because we are obtaining a constant value
	CMtfScriptedTestCase* dummyCase = CMtfScriptedTestCase::NewL(iTestServer,EFalse);
	CleanupStack::PushL(dummyCase);
	CMtfConfigurationType::TMtfConfigurationType type = ObtainValueParameterL<CMtfConfigurationType::TMtfConfigurationType>(*dummyCase,*enumerator);
	CleanupStack::PopAndDestroy(dummyCase);
	CleanupStack::PopAndDestroy(enumerator);
	
	// now obtain the second (or more) parameter	
	CMtfConfigurationType* configurationType = CMtfConfigurationType::NewL(type);
	CleanupStack::PushL(configurationType);
	parser.SkipSpace();
		
	while(!parser.Eos())
	{
		configurationType->AddConfigurationFilenameL(parser.NextToken());
		parser.SkipSpace();
	}		
		
	if (configurationType->Count() > 0)
	{
		CleanupStack::Pop(configurationType);
		
		// this function takes ownership immediately
		iTestServer.SetMainScriptConfigurationTypeL(configurationType);
	}
	else
	{
		User::Leave(KErrGeneral);
	}
		
	return TestStepResult();
}
		
TVerdict CMtfSetConfigurationTestStep::doTestStepPostambleL()
{
	return TestStepResult();
}
