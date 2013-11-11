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
// GetDefaultSmsService
// [Action Parameters]
// smsClientMtm	<input>:	CSmsClientMtm
// serviceId	<output>:	TMsvId
// [Action Description]
// GetDefaultSmsService Test Action is intended to get the default SMS service ID.
// [APIs Used]	
// CSmsClientMtm::DefaultServiceL
// __ACTION_INFO_END__
// 
//


// system includes
#include <smsclnt.h>

// user includes
#include "CMtfTestActionGetDefaultSmsService.h"
#include "TMtfTestParameterType.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionGetDefaultSmsService object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionGetDefaultSmsService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetDefaultSmsService* self = new (ELeave) CMtfTestActionGetDefaultSmsService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionGetDefaultSmsService
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionGetDefaultSmsService::CMtfTestActionGetDefaultSmsService(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionGetDefaultSmsService
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionGetDefaultSmsService::~CMtfTestActionGetDefaultSmsService()
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
void CMtfTestActionGetDefaultSmsService::ExecuteActionL()
{

	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetDefaultSmsService);
	TMsvId serviceId = 0;
	CSmsClientMtm* smsClientMtm = reinterpret_cast<CSmsClientMtm*> 
		( ObtainParameterReferenceL<CBaseMtm>(TestCase(), ActionParameters().Parameter(0)) );

	TInt shouldFail = ObtainValueParameterL<TInt>( TestCase(),ActionParameters().Parameter(1), 0 );
		
		
	TRAPD( error, serviceId = smsClientMtm->DefaultServiceL() );
	
	if( shouldFail )
		{
		if ( error == KErrNone )
			{
			TestCase().SetTestStepResult(EFail);
			}
		}
	else
		{
		if( error != KErrNone || serviceId < 0 ) // failure check 
			{
			TestCase().ERR_PRINTF2( _L("Retrieved an invalid service id = %d !!"), serviceId );
			TestCase().SetTestStepResult(EFail);
			}
		}
		
	StoreParameterL<TMsvId>(TestCase(), serviceId, ActionParameters().Parameter(2));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetDefaultSmsService);
	TestCase().ActionCompletedL(*this);	
}


