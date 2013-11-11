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
// RemoveDefaultPopService
// [Action Parameters]
// popClientMtm		<input>:	CPop3ClientMtm
// [Action Description]
// RemoveDefaultPopService Test Action is intended to remove the default POP3
// service
// [APIs Used]	
// CPop3ClientMtm::RemoveDefaultServiceL
// __ACTION_INFO_END__
// 
//

// system #includes
#include <popcmtm.h>

// user #includes
#include "CMtfTestActionRemoveDefaultPopService.h"
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
CMtfTestAction* CMtfTestActionRemoveDefaultPopService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionRemoveDefaultPopService* self = new (ELeave) CMtfTestActionRemoveDefaultPopService(aTestCase);
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
CMtfTestActionRemoveDefaultPopService::CMtfTestActionRemoveDefaultPopService(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
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
CMtfTestActionRemoveDefaultPopService::~CMtfTestActionRemoveDefaultPopService()
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
void CMtfTestActionRemoveDefaultPopService::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionRemoveDefaultPopService);
	CPop3ClientMtm* popClientMtm = 
		ObtainParameterReferenceL<CPop3ClientMtm>(TestCase(), ActionParameters().Parameter(0));	
	
	TRAPD( error, popClientMtm->RemoveDefaultServiceL() );
		
	if (error != KErrNone)
		{
		// Failed
		TestCase().ERR_PRINTF2(_L("Failed to remove default POP3 service error(%d)"), error);
		TestCase().SetTestStepResult(EFail);
		}


	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionRemoveDefaultPopService);
	TestCase().ActionCompletedL(*this);	
	}


