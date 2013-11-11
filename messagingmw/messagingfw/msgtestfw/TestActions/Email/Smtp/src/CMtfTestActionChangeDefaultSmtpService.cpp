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
// ChangeDefaultSmtpService
// [Action Parameters]
// serviceId		<input>:	TMsvId. Service ID to set as default.
// smtpClientMtm	<input>:	CSmtpClientMtm	
// [Action Description]
// ChangeDefaultSmtpService Test Action is intended to change the default SMTP
// service to serviceId.
// [APIs Used]	
// CSmtpClientMtm::ChangeDefaultServiceL
// __ACTION_INFO_END__
// 
//

// system #includes
#include <smtcmtm.h>

// user #includes
#include "CMtfTestActionChangeDefaultSmtpService.h"
#include "TMtfTestParameterType.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionChangeDefaultSmtpService object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionChangeDefaultSmtpService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionChangeDefaultSmtpService* self = new (ELeave) CMtfTestActionChangeDefaultSmtpService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionChangeDefaultSmtpService
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionChangeDefaultSmtpService::CMtfTestActionChangeDefaultSmtpService(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionChangeDefaultSmtpService
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionChangeDefaultSmtpService::~CMtfTestActionChangeDefaultSmtpService()
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
void CMtfTestActionChangeDefaultSmtpService::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionChangeDefaultSmtpService);
	TMsvId serviceId = ObtainValueParameterL<TMsvId>(TestCase(), ActionParameters().Parameter(0));
	// For some reason CMtfTestActionCreateSmtpMtm stores the mtm as a CBaseMtm* 
	// unlike the other create mtm test action so a cast to CSmtpClientMtm* is required 
	// this should be changed at some point and all tests using the 
	// CMtfTestActionCreateSmtpMtm test action will need to be updated

	CSmtpClientMtm* smtpClientMtm = reinterpret_cast<CSmtpClientMtm*>
		( ObtainParameterReferenceL<CBaseMtm>(TestCase(), ActionParameters().Parameter(1)) );
	
	TRAPD( error, smtpClientMtm->ChangeDefaultServiceL( serviceId ) );
	
	if (error != KErrNone)
		{
		TestCase().ERR_PRINTF2(_L("Failed to change SMTP default service error(%d)" ), error);
		TestCase().SetTestStepResult(EFail);
		}	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionChangeDefaultSmtpService);
	TestCase().ActionCompletedL(*this);	
	}


