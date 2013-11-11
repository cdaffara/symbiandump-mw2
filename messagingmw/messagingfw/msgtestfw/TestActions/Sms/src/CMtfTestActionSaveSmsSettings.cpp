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
// SaveSmsSettings
// [Action Parameters]
// configFileIndex	<input>:	Index of the config file
// smsSettings		<output>:	Settings object
// [Action Description]
// SaveSmsSettings Test Action is intended to modify and save the existing Sms Service Settings
// to CenRep using the configuration settings file.  The Test Action creates the CSmsSettings 
// object, parses the settings file and then sets the member variables of the object to the 
// corresponding value stored in the settings file.  The CSmsSettings object is then used to 
// save the settings into the Central Repository.
// [APIs Used]	
// CSMSAccount::SaveSettingsL
// CSmsSettings::SetCharacterSet
// CSmsSettings::SetCommDbAction
// CSmsSettings::SetDelivery
// CSmsSettings::SetDeliveryReport
// CSmsSettings::SetMessageConversion
// CSmsSettings::SetDefaultSC
// CSmsSettings::SetRejectDuplicate
// CSmsSettings::SetReplyPath
// CSmsSettings::SetReplyQuoted
// CSmsSettings::SetSmsBearer
// CSmsSettings::SetSmsBearerAction
// CSmsSettings::SetSpecialMessageHandling
// CSmsSettings::SetStatusReportHandling
// CSmsSettings::SetValidityPeriod
// CSmsSettings::SetValidityPeriodFormat
// CSmsSettings::AddSCAddressL
// CSmsSettings::SetClass2Folder
// CSmsSettings::SetDescriptionLength
// CSmsSettings::SetCanConcatenate
// __ACTION_INFO_END__
// 
//


//#include <smutset.h>

#include <csmsaccount.h>

#include "CMtfTestActionSaveSmsSettings.h"
#include "CMtfTestActionUtilsSmsScripts.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSaveSmsSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSaveSmsSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSaveSmsSettings* self = new (ELeave) CMtfTestActionSaveSmsSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}



/**
  Function : CMtfTestActionSaveSmsSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSaveSmsSettings::CMtfTestActionSaveSmsSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSaveSmsSettings
  Description : Destructor
  @internalTechnology
  @param :
  @
  @pre 
  @post 
*/
CMtfTestActionSaveSmsSettings::~CMtfTestActionSaveSmsSettings()
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
void CMtfTestActionSaveSmsSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSaveSmsSettings);
	CSmsSettings* smsSettings = CSmsSettings::NewL();
	CleanupStack::PushL(smsSettings);
	
	CSmsAccount* smsAccount = CSmsAccount::NewL();
	CleanupStack::PushL(smsAccount);
	
	TInt configFileIndex = ObtainValueParameterL<TInt>(TestCase(), ActionParameters().Parameter(0));
	
	// Get the settings file name from the test case
	const TPtrC settingsFile = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfSmsSettings, configFileIndex);
	
	// Read SMS settings from config file
	CMtfTestActionUtilsSmsScripts::ReadSmsSettingsFromConfigurationFileL(TestCase(), settingsFile, *smsSettings);
		
	// Save settings to central repository
	smsAccount->SaveSettingsL(*smsSettings);
	
	StoreParameterL<CSmsSettings>(TestCase(), *smsSettings, ActionParameters().Parameter(1));
	
	CleanupStack::PopAndDestroy(smsAccount);
	CleanupStack::Pop(smsSettings);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSaveSmsSettings);
	TestCase().ActionCompletedL(*this);
	}


