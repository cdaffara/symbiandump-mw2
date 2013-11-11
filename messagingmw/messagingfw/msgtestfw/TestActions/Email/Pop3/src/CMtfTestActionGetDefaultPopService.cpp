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
// GetDefaultPopService
// [Action Parameters]
// popClientMtm	<input>:	CPop3ClientMtm
// serviceId	<output>:	TMsvId
// [Action Description]
// GetDefaultPopService Test Action is intended to get the default POP3 service ID.
// [APIs Used]	
// CPOP3ClientMtm::DefaultServiceL
// __ACTION_INFO_END__
// 
//

// system #includes
#include <popcmtm.h>

// user #includes
#include "CMtfTestActionGetDefaultPopService.h"
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
CMtfTestAction* CMtfTestActionGetDefaultPopService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetDefaultPopService* self = new (ELeave) CMtfTestActionGetDefaultPopService(aTestCase);
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
CMtfTestActionGetDefaultPopService::CMtfTestActionGetDefaultPopService(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
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
CMtfTestActionGetDefaultPopService::~CMtfTestActionGetDefaultPopService()
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
void CMtfTestActionGetDefaultPopService::ExecuteActionL()
	{

	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetDefaultPopService);
	CPop3ClientMtm* popClientMtm = 
		ObtainParameterReferenceL<CPop3ClientMtm>(TestCase(), ActionParameters().Parameter(0));
	TInt shouldFail = ObtainValueParameterL<TInt>( TestCase(),ActionParameters().Parameter(1), 0 );
	TMsvId serviceId = 0;
	
	TRAPD( error, serviceId = popClientMtm->DefaultServiceL() );

	if( shouldFail )
		{
		if ( error == KErrNone ) // pass check
			{
			TestCase().SetTestStepResult(EFail);
			}
		}
	else 
		{		
		if( error != KErrNone || serviceId < 0 ) // failure check 
			{
			TestCase().SetTestStepResult(EFail);
			}
		}

	StoreParameterL<TMsvId>(TestCase(), serviceId, ActionParameters().Parameter(2));
	TestCase().INFO_PRINTF3(_L("GetDefaultPopService Completed service id = %d should Fail = %d"), serviceId, shouldFail );

	TestCase().ActionCompletedL(*this);						
	}
	

