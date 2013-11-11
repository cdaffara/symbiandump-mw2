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
// LoadSmsSettings
// [Action Parameters]
// smsSettings     	output 		CSmsSettings
// [Action Description]
// LoadSmsSettings Test Action is intended to retrieve SMS service settings 
// from CenRep into an object of CSmsSettings class. 
// [APIs Used]	
// CSMSAccount::LoadSettingsL 
// __ACTION_INFO_END__
// 
//

#include <csmsaccount.h>

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionLoadSmsSettings.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionLoadSmsSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionLoadSmsSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionLoadSmsSettings* self = new (ELeave) CMtfTestActionLoadSmsSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionLoadSmsSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionLoadSmsSettings::CMtfTestActionLoadSmsSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionLoadSmsSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionLoadSmsSettings::~CMtfTestActionLoadSmsSettings()
	{
	}

/**
  Function : ExecuteActionL
  Description : Entry point for the this test action in the test framework
  @internalTechnology
  @param : none
  @return : void
  @pre none 
  @post none
*/
void CMtfTestActionLoadSmsSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionLoadSmsSettings);
	CSmsSettings* smsSettings = CSmsSettings::NewL();
	CleanupStack::PushL(smsSettings);
	
	CSmsAccount* smsAccount = CSmsAccount::NewL();
	CleanupStack::PushL(smsAccount);
	
	smsAccount->LoadSettingsL( *smsSettings );
	StoreParameterL<CSmsSettings>( TestCase(), *smsSettings, ActionParameters().Parameter(0) );
	
	CleanupStack::PopAndDestroy(smsAccount);
	CleanupStack::Pop(smsSettings); 

	TestCase().INFO_PRINTF2( _L("Test Action %S completed."), &KTestActionLoadSmsSettings );
	TestCase().ActionCompletedL( *this );	
	}


