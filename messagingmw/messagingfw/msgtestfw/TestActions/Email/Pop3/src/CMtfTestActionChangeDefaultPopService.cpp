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
// ChangeDefaultPopService
// [Action Parameters]
// serviceId	<input>:	TMsvId. Service ID to set as default.
// popClientMtm	<input>:	CPop3ClientMtm	
// [Action Description]
// ChangeDefaultPopService Test Action is intended to change the default POP3
// service to serviceId.
// [APIs Used]	
// CPop3ClientMtm::ChangeDefaultServiceL
// __ACTION_INFO_END__
// 
//

// system #includes
#include <popcmtm.h>

// user #includes
#include "CMtfTestActionChangeDefaultPopService.h"
#include "TMtfTestParameterType.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionGetPopAccounts object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionChangeDefaultPopService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionChangeDefaultPopService* self = new (ELeave) CMtfTestActionChangeDefaultPopService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionGetPopAccounts
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionChangeDefaultPopService::CMtfTestActionChangeDefaultPopService(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionGetPopAccounts
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionChangeDefaultPopService::~CMtfTestActionChangeDefaultPopService()
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
void CMtfTestActionChangeDefaultPopService::ExecuteActionL()
	{

	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionChangeDefaultPopService);
	TMsvId serviceId = ObtainValueParameterL<TMsvId>(TestCase(), ActionParameters().Parameter(0));
	CPop3ClientMtm* popClientMtm = ObtainParameterReferenceL<CPop3ClientMtm>(TestCase(), ActionParameters().Parameter(1));
	
	TRAPD( error, popClientMtm->ChangeDefaultServiceL(serviceId) );
		
	if (error != KErrNone)
		{
		TestCase().ERR_PRINTF2(_L("Failed to change POP3 default service error(%d)"), error);
		TestCase().SetTestStepResult(EFail);
		}
	else
		{			
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionChangeDefaultPopService);
		}


	TestCase().ActionCompletedL(*this);				
	}


