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
// RemoveDefaultSmtpService
// [Action Parameters]
// smtpClientMtm		<input>:	CSmtpClientMtm
// [Action Description]
// RemoveDefaultSmtpService Test Action is intended to remove the default SMTP
// service
// [APIs Used]	
// CSmtpClientMtm::RemoveDefaultServiceL
// __ACTION_INFO_END__
// 
//

// system #includes
#include <smtcmtm.h>

// user #includes
#include "CMtfTestActionRemoveDefaultSmtpService.h"
#include "TMtfTestParameterType.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionRemoveDefaultSmtpService object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionRemoveDefaultSmtpService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionRemoveDefaultSmtpService* self = new (ELeave) CMtfTestActionRemoveDefaultSmtpService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionRemoveDefaultSmtpService
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionRemoveDefaultSmtpService::CMtfTestActionRemoveDefaultSmtpService(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionRemoveDefaultSmtpService
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionRemoveDefaultSmtpService::~CMtfTestActionRemoveDefaultSmtpService()
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
void CMtfTestActionRemoveDefaultSmtpService::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionRemoveDefaultSmtpService);
	CSmtpClientMtm* smtpClientMtm = reinterpret_cast<CSmtpClientMtm*>  
		(ObtainParameterReferenceL<CBaseMtm>(TestCase(), ActionParameters().Parameter(0)));
	TRAPD( error, smtpClientMtm->RemoveDefaultServiceL() );
	if (error != KErrNone)
		{
		// Failed
		TestCase().ERR_PRINTF2(_L("Failed to remove default SMTP service error(%d)"), error);
		TestCase().SetTestStepResult(EFail);
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionRemoveDefaultSmtpService);
	TestCase().ActionCompletedL(*this);	

	}


