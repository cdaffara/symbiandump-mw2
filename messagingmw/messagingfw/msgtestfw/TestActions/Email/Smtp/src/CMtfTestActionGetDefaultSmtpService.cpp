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
// GetDefaultSmtpService
// [Action Parameters]
// smtpClientMtm	<input>:	CSmtpClientMtm
// serviceId		<output>:	TMsvId
// [Action Description]
// GetDefaultSmtpService Test Action is intended to get the default SMTP service ID.
// [APIs Used]	
// CSmtpClientMtm::DefaultServiceL
// __ACTION_INFO_END__
// 
//

// system #includes
#include <smtcmtm.h>

// user #includes
#include "CMtfTestActionGetDefaultSmtpService.h"
#include "TMtfTestParameterType.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionGetDefaultSmtpService object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionGetDefaultSmtpService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetDefaultSmtpService* self = new (ELeave) CMtfTestActionGetDefaultSmtpService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionGetDefaultSmtpService
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionGetDefaultSmtpService::CMtfTestActionGetDefaultSmtpService(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionGetDefaultSmtpService
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionGetDefaultSmtpService::~CMtfTestActionGetDefaultSmtpService()
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
void CMtfTestActionGetDefaultSmtpService::ExecuteActionL()
	{
	
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetDefaultSmtpService);
	CSmtpClientMtm* smtpClientMtm = reinterpret_cast<CSmtpClientMtm*> 
		( ObtainParameterReferenceL<CBaseMtm>(TestCase(), ActionParameters().Parameter(0)) );
	TInt shouldFail = ObtainValueParameterL<TInt>( TestCase(),ActionParameters().Parameter(1), 0 );
	TMsvId serviceId = 0;
	TRAPD(error, serviceId = smtpClientMtm->DefaultServiceL() );

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
	TestCase().INFO_PRINTF2( _L("GetDefaultSmtpService completed serviceId = %d"), serviceId );
	

	TestCase().ActionCompletedL(*this);	
	}

