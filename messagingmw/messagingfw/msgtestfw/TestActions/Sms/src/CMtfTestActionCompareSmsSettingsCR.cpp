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
// CompareSmsSettings
// [Action Parameters]
// smsSettingsOrg			input 			CSmsSettings
// smsSettingsRef			input			CSmsSettings
// [Action Description]
// CompareSmsSettings Test Action is intended to compare one SMS service settings values 
// with other SMS service settings values. 
// [APIs Used]
// CSmsSettings::CharacterSet
// CSmsSettings::CommDbAction
// CSmsSettings::Delivery
// CSmsSettings::DeliveryReport
// CSmsSettings::MessageConversion
// CSmsSettings::NumSCAddresses
// CSmsSettings::DefaultSC
// CSmsSettings::SCAddress
// CSmsSettings::RejectDuplicate
// CSmsSettings::ReplyPath
// CSmsSettings::ReplyQuoted
// CSmsSettings::SmsBearer
// CSmsSettings::SmsBearerAction
// CSmsSettings::SpecialMessageHandling
// CSmsSettings::StatusReportHandling
// CSmsSettings::ValidityPeriod
// CSmsSettings::ValidityPeriodFormat
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionCompareSmsSettingsCR.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionCompareSmsSettingsCR object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionCompareSmsSettingsCR::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCompareSmsSettingsCR* self = new (ELeave) CMtfTestActionCompareSmsSettingsCR(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	
/**
  Function : CMtfTestActionCompareSmsSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionCompareSmsSettingsCR::CMtfTestActionCompareSmsSettingsCR(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionCompareSmsSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionCompareSmsSettingsCR::~CMtfTestActionCompareSmsSettingsCR()
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
void CMtfTestActionCompareSmsSettingsCR::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCompareSmsSettingsCR);
	CSmsSettings* smsSettingsOrg = ObtainParameterReferenceL<CSmsSettings>(TestCase(),ActionParameters().Parameter(0) );
	CSmsSettings* smsSettingsRef = ObtainParameterReferenceL<CSmsSettings>(TestCase(),ActionParameters().Parameter(1) );
	
	if(	smsSettingsOrg->CanConcatenate() != smsSettingsRef->CanConcatenate() ||
		smsSettingsOrg->Class2Folder() != smsSettingsRef->Class2Folder() ||
		smsSettingsOrg->CharacterSet() != smsSettingsRef->CharacterSet() ||
		smsSettingsOrg->CommDbAction() != smsSettingsRef->CommDbAction() ||
		
		smsSettingsOrg->DefaultServiceCenter() != smsSettingsRef->DefaultServiceCenter() ||
	
		smsSettingsOrg->Delivery() != smsSettingsRef->Delivery() ||
		smsSettingsOrg->DeliveryReport() !=	smsSettingsRef->DeliveryReport() ||
		smsSettingsOrg->DescriptionLength() != smsSettingsRef->DescriptionLength() ||
		smsSettingsOrg->MessageConversion() != smsSettingsRef->MessageConversion() ||
		
		smsSettingsOrg->ServiceCenterCount() != smsSettingsRef->ServiceCenterCount() ||
	
		smsSettingsOrg->RejectDuplicate() != smsSettingsRef->RejectDuplicate() ||
		smsSettingsOrg->ReplyPath() != smsSettingsRef->ReplyPath() ||
		smsSettingsOrg->ReplyQuoted() != smsSettingsRef->ReplyQuoted() ||
		smsSettingsOrg->SmsBearer() != smsSettingsRef->SmsBearer() ||
		smsSettingsOrg->SmsBearerAction() != smsSettingsRef->SmsBearerAction() ||
		smsSettingsOrg->SpecialMessageHandling() !=	smsSettingsRef->SpecialMessageHandling() ||
		smsSettingsOrg->StatusReportHandling() != smsSettingsRef->StatusReportHandling() ||
		smsSettingsOrg->ValidityPeriod() !=	smsSettingsRef->ValidityPeriod() ||
		smsSettingsOrg->ValidityPeriodFormat() != smsSettingsRef->ValidityPeriodFormat()
		)
		{
		// failed
		TestCase().ERR_PRINTF1(_L("SMS Settings objects do not match !"));
		TestCase().SetTestStepResult(EFail);
 
		}
	else
		{
			// compare service centre addresses
		
	
		for (TInt i = 0; i<smsSettingsOrg->ServiceCenterCount(); i++)
			{
			if(smsSettingsOrg->GetServiceCenter(i).Name() != 		smsSettingsRef->GetServiceCenter(i).Name() ||
			   smsSettingsOrg->GetServiceCenter(i).Address() != 	smsSettingsRef->GetServiceCenter(i).Address() )
			   {
					TestCase().ERR_PRINTF1(_L("SMS Settings Service centres do not match !"));
					TestCase().SetTestStepResult(EFail);
			  	}
			}
			
		
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCompareSmsSettingsCR);	
	TestCase().ActionCompletedL(*this);
	}


