// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CompareLastSegmentReport
// [Action Parameters]
// smsSettingsOrg			input 			CSmsSettings
// smsSettingsRef			input			CSmsSettings
// [Action Description]
// CompareLastSegmentReport Test Action is intended to compare the saved SMS service settings given in the SMS settings file  
// with the loaded SMS service settings values from the Central Repository for the last segment status report request . 
// [APIs Used]
// CSmsSettings::LastSegmentDeliveryReport
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionCompareLastSegmentReport.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParameters - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionCompareSmsSettingsCR object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionCompareLastSegmentReport::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCompareLastSegmentReport* self = new (ELeave) CMtfTestActionCompareLastSegmentReport(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	
/**
  Function : CMtfTestActionCompareLastSegmentReport
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionCompareLastSegmentReport::CMtfTestActionCompareLastSegmentReport(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionCompareLastSegmentReport
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionCompareLastSegmentReport::~CMtfTestActionCompareLastSegmentReport()
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
void CMtfTestActionCompareLastSegmentReport::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCompareLastSegmentReport);
	CSmsSettings* smsSettingsOrg = ObtainParameterReferenceL<CSmsSettings>(TestCase(),ActionParameters().Parameter(0) );
	CSmsSettings* smsSettingsRef = ObtainParameterReferenceL<CSmsSettings>(TestCase(),ActionParameters().Parameter(1) );

	if(	smsSettingsOrg->LastSegmentDeliveryReport() !=	smsSettingsRef->LastSegmentDeliveryReport())
		{
		// failed
		TestCase().ERR_PRINTF1(_L("SMS Settings for the last segment report request do not match !"));
		TestCase().SetTestStepResult(EFail);
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCompareLastSegmentReport);
	TestCase().ActionCompletedL(*this);
	}


