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
// CompareSmsSettings
// [Action Parameters]
// CMsvSession&	Session    <input>: Reference to the session.
// TMsvId 		ServiceId  <input>: Value of the SMS service id.
// TInt 		Index      <input>: (optional) Selects what defaults file to use, if not specified 0 is assumed
// [Action Description]
// Compares the settings of the specified SMS service to those stored on a file. The Messaging Test 
// server will decide on what file to be used to read the settings from.
// [APIs Used]
// CSmsSettings::
// CSmsSettings::CanConcatenate()
// CSmsSettings::CharacterSet()
// CSmsSettings::CommDbAction()
// CSmsSettings::Delivery()
// CSmsSettings::DeliveryReport(
// CSmsSettings::MessageConversion()
// CSmsSettings::NumSCAddresses()
// CSmsSettings::RejectDuplicate()
// CSmsSettings::ReplyPath()
// CSmsSettings::ReplyQuoted()
// CSmsSettings::SmsBearer()
// CSmsSettings::SmsBearerAction()
// CSmsSettings::SpecialMessageHandling()
// CSmsSettings::StatusReportHandling()
// CSmsSettings::ValidityPeriod()
// CSmsSettings::ValidityPeriodFormat()
// CSmsSettings::SCAddress()
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionCompareSmsSettings.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include "CMtfTestActionUtilsSmsScripts.h"

#include "MSVAPI.H"


	#include <csmsaccount.h>
	

CMtfTestAction* CMtfTestActionCompareSmsSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCompareSmsSettings* self = new (ELeave) CMtfTestActionCompareSmsSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCompareSmsSettings::CMtfTestActionCompareSmsSettings(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCompareSmsSettings::~CMtfTestActionCompareSmsSettings()
	{
	}

void CMtfTestActionCompareSmsSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCompareSmsSettings);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramSmsServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	
	
	TInt paramDefaultIndex = 0;
	if(ActionParameters().Count() == 3)
		{
		paramDefaultIndex = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));
		}
	
	// Get the settings file name from the test case
	const TPtrC settingsFile = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfSmsSettings, paramDefaultIndex);
	
	// Create reference SMS settings from the file
	CSmsSettings* smsSettingsReference = CSmsSettings::NewL();
	CleanupStack::PushL(smsSettingsReference);
	CMtfTestActionUtilsSmsScripts::ReadSmsSettingsFromConfigurationFileL(TestCase(), settingsFile, *smsSettingsReference);
	
	CSmsSettings* smsSettings = CSmsSettings::NewL();
	CleanupStack::PushL(smsSettings);
	
	// Restore the settings from the specified service into second CSmsSettings object
	
	CSmsAccount* smsAccount = CSmsAccount::NewLC();
	smsAccount->LoadSettingsL(*smsSettings);
	
	
	// Compare them, might want to split "if" below for ease of debugging..
	
	if(	smsSettings->CanConcatenate() != 			smsSettingsReference->CanConcatenate() ||
		smsSettings->Class2Folder() !=				smsSettingsReference->Class2Folder() ||
		smsSettings->CharacterSet() != 				smsSettingsReference->CharacterSet() ||
		smsSettings->CommDbAction() != 				smsSettingsReference->CommDbAction() ||
	
		smsSettings->DefaultServiceCenter() !=		smsSettingsReference->DefaultServiceCenter() ||
	
		smsSettings->Delivery() !=					smsSettingsReference->Delivery() ||
		smsSettings->DeliveryReport() !=			smsSettingsReference->DeliveryReport() ||
		smsSettings->DescriptionLength() != 		smsSettingsReference->DescriptionLength() ||
		smsSettings->MessageConversion() != 		smsSettingsReference->MessageConversion() ||
		
		smsSettings->ServiceCenterCount() != 		smsSettingsReference->ServiceCenterCount() ||
	
		smsSettings->RejectDuplicate() != 			smsSettingsReference->RejectDuplicate() ||
		smsSettings->ReplyPath() != 				smsSettingsReference->ReplyPath() ||
		smsSettings->ReplyQuoted() !=				smsSettingsReference->ReplyQuoted() ||
		smsSettings->SmsBearer() != 				smsSettingsReference->SmsBearer() ||
		smsSettings->SmsBearerAction() != 			smsSettingsReference->SmsBearerAction() ||
		smsSettings->SpecialMessageHandling() !=	smsSettingsReference->SpecialMessageHandling() ||
		smsSettings->StatusReportHandling() != 		smsSettingsReference->StatusReportHandling() ||
		smsSettings->ValidityPeriod() !=			smsSettingsReference->ValidityPeriod() ||
		smsSettings->ValidityPeriodFormat() != 		smsSettingsReference->ValidityPeriodFormat()
		)
		{
		// settings differ
		User::Leave(KErrGeneral);
		}
		
	// compare service centre addresses
	
	
	for (TInt i = 0; i<smsSettings->ServiceCenterCount(); i++)
		{
		if(smsSettings->GetServiceCenter(i).Name() != 		smsSettingsReference->GetServiceCenter(i).Name() ||
		   smsSettings->GetServiceCenter(i).Address() != 	smsSettingsReference->GetServiceCenter(i).Address() )
		  	{
			// settings differ
			User::Leave(KErrGeneral);
		  	}
		}
		  
		  	
		

	CleanupStack::PopAndDestroy(3,smsSettingsReference); // smsSettings, smsAccount

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCompareSmsSettings);
	TestCase().ActionCompletedL(*this);
	}


