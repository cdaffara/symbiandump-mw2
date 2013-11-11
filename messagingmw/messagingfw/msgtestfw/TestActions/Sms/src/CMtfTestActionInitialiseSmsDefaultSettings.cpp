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
// InitialiseSmsDefaultSettings
// [Action Parameters]
// smsSettings     		output	 CSmsSettings
// [Action Description]
// Creates an SMS service entry in central repository, sets all settings to 
// default values and returns a CSmsSettings object populated with the settings.
// [APIs Used]
// CSMSAccount::InitialiseDefaultSettingsL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include <csmsaccount.h>

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionInitialiseSmsDefaultSettings.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionInitialiseSmsDefaultSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionInitialiseSmsDefaultSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
{
	CMtfTestActionInitialiseSmsDefaultSettings* self = new (ELeave) CMtfTestActionInitialiseSmsDefaultSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
}

/**
  Function : CMtfTestActionInitialiseSmsDefaultSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionInitialiseSmsDefaultSettings::CMtfTestActionInitialiseSmsDefaultSettings(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
{
	
}

/**
  Function : ~CMtfTestActionInitialiseSmsDefaultSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionInitialiseSmsDefaultSettings::~CMtfTestActionInitialiseSmsDefaultSettings()
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
void CMtfTestActionInitialiseSmsDefaultSettings::ExecuteActionL()
{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionInitialiseSmsDefaultSettings);
	CSmsSettings* smsSettings = CSmsSettings::NewL();
	CleanupStack::PushL(smsSettings);
	
	CSmsAccount* smsAccount = CSmsAccount::NewL();
	CleanupStack::PushL(smsAccount);
	
	smsAccount->InitialiseDefaultSettingsL(*smsSettings);
	StoreParameterL<CSmsSettings>(TestCase(), *smsSettings, ActionParameters().Parameter(0));
	
	CleanupStack::PopAndDestroy(smsAccount);
	CleanupStack::Pop(smsSettings); 

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionInitialiseSmsDefaultSettings);
	TestCase().ActionCompletedL(*this);
}

