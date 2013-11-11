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
// SetSmsServiceSettings
// [Action Parameters]
// CMsvSession& Session    <input>: Reference to the session.
// TMsvId       ServiceId  <input>: Value of the SMS service id.
// TInt         Index      <input>: (optional) Selects what defaults file to use
// [Action Description]
// Modifies the settings of the specified SMS service to match the ones specified on a file.
// The Messaging Test server will decide on what file to be used to read the settings from.
// New settings are created if the SMS service doesn't have any settings saved.
// [APIs Used]
// CMsvEntry::EditStoreL
// CSmsSettings::StoreL
// CMsvStore::CommitL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include <msvapi.h>
#include <smutset.h>
#include  <csmsaccount.h>


#include "CMtfTestActionSetSmsServiceSettings.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include "CMtfTestActionUtilsSmsScripts.h"

CMtfTestAction* CMtfTestActionSetSmsServiceSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetSmsServiceSettings* self = new (ELeave) CMtfTestActionSetSmsServiceSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSetSmsServiceSettings::CMtfTestActionSetSmsServiceSettings(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSetSmsServiceSettings::~CMtfTestActionSetSmsServiceSettings()
	{
	}

void CMtfTestActionSetSmsServiceSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetSmsServiceSettings);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramSmsServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	
	
	TInt paramDefaultIndex = 0;
	if(ActionParameters().Count() == 3)
		{
		paramDefaultIndex = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));
		}
	
	// Get the settings file name from the test case
	const TPtrC settingsFile = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfSmsSettings, paramDefaultIndex);
	
	// Create SMS settings from the file
	CSmsSettings* smsSettings = CSmsSettings::NewL();
	CleanupStack::PushL(smsSettings);

	CSmsAccount* account = CSmsAccount::NewLC();
	account->InitialiseDefaultSettingsL(*smsSettings);

	// now override the settings with the config file contents
	CMtfTestActionUtilsSmsScripts::ReadSmsSettingsFromConfigurationFileL(TestCase(), settingsFile, *smsSettings);
	
	account->SaveSettingsL(*smsSettings);
	CleanupStack::PopAndDestroy(2, smsSettings); // account smsSettings	
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetSmsServiceSettings);
	TestCase().ActionCompletedL(*this);
	}



