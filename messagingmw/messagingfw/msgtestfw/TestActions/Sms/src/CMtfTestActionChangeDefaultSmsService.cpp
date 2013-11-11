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
// ChangeDefaultSmsService
// [Action Parameters]
// serviceId	<input>:	TMsvId. Service ID to set as default.
// smsClientMtm	<input>:	CSmsClientMtm	
// [Action Description]
// ChangeDefaultSmsService Test Action is intended to change the default SMS
// service to serviceId.
// [APIs Used]	
// CSmsClientMtm::ChangeDefaultServiceL
// __ACTION_INFO_END__
// 
//

// system #includes
#include <smsclnt.h>

//user #includes
#include "CMtfTestActionChangeDefaultSmsService.h"
#include "TMtfTestParameterType.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionChangeDefaultSmsService object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionChangeDefaultSmsService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionChangeDefaultSmsService* self = new (ELeave) CMtfTestActionChangeDefaultSmsService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionChangeDefaultSmsService
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionChangeDefaultSmsService::CMtfTestActionChangeDefaultSmsService(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionChangeDefaultSmsService
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionChangeDefaultSmsService::~CMtfTestActionChangeDefaultSmsService()
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
void CMtfTestActionChangeDefaultSmsService::ExecuteActionL()
	{
	
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionChangeDefaultSmsService);
	TMsvId serviceId = ObtainValueParameterL<TMsvId>(TestCase(), ActionParameters().Parameter(0));
	CSmsClientMtm* smsClientMtm = reinterpret_cast<CSmsClientMtm*> 
		( ObtainParameterReferenceL<CBaseMtm>(TestCase(), ActionParameters().Parameter(1)));
	
	TRAPD( error, smsClientMtm->ChangeDefaultServiceL(serviceId) );
	
	if (error != KErrNone)
		{
		// Failed
		TestCase().ERR_PRINTF2(_L("Failed to change SMS default service error(%d)"), error);
		TestCase().SetTestStepResult(EFail);
		}
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionChangeDefaultSmsService);
	TestCase().ActionCompletedL(*this);	
	}


