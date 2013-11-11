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
// RemoveDefaultSmsService
// [Action Parameters]
// smsClientMtm		<input>:	CSmsClientMtm
// [Action Description]
// RemoveDefaultSmsService Test Action is intended to remove the default SMS
// service
// [APIs Used]	
// CSmsClientMtm::RemoveDefaultServiceL
// __ACTION_INFO_END__
// 
//


#include <smsclnt.h>

#include "CMtfTestActionRemoveDefaultSmsService.h"
#include "TMtfTestParameterType.h"

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionRemoveDefaultSmsService object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionRemoveDefaultSmsService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionRemoveDefaultSmsService* self = new (ELeave) CMtfTestActionRemoveDefaultSmsService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionRemoveDefaultSmsService
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionRemoveDefaultSmsService::CMtfTestActionRemoveDefaultSmsService(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionRemoveDefaultSmsService
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionRemoveDefaultSmsService::~CMtfTestActionRemoveDefaultSmsService()
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
void CMtfTestActionRemoveDefaultSmsService::ExecuteActionL()
{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionRemoveDefaultSmsService);
	CSmsClientMtm* smsClientMtm = reinterpret_cast<CSmsClientMtm*>
		( ObtainParameterReferenceL<CBaseMtm>(TestCase(), ActionParameters().Parameter(0) ) );
	
	TRAPD( error, smsClientMtm->RemoveDefaultServiceL() );
	
	if (error != KErrNone)
		{
		TestCase().ERR_PRINTF2(_L("Failed to remove default SMS service error(%d)"), error );
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionRemoveDefaultSmsService);
		}
	
	TestCase().ActionCompletedL(*this);		

}


