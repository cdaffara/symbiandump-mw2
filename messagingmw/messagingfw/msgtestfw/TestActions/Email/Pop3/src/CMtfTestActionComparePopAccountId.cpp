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
// ComparePopAccountId
// [Action Parameters]
// accountId1			input		TPOP3AccountId
// accountId2			input		TPOP3AccountId
// [Action Description]
// ComparePopAccountId Test Action is intended to compare the account Ids.
// [APIs Used]	
// None
// __ACTION_INFO_END__
// 
//

#include <cemailaccounts.h>

#include "CMtfTestActionComparePopAccountId.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionComparePopAccountId object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionComparePopAccountId::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionComparePopAccountId* self = new (ELeave) CMtfTestActionComparePopAccountId(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionComparePopAccountId
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionComparePopAccountId::CMtfTestActionComparePopAccountId(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionComparePopAccountId
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionComparePopAccountId::~CMtfTestActionComparePopAccountId()
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
void CMtfTestActionComparePopAccountId::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionComparePopAccountId);
	TPopAccount accountId1 = ObtainValueParameterL<TPopAccount>(TestCase(), ActionParameters().Parameter(0));
	TPopAccount accountId2 = ObtainValueParameterL<TPopAccount>(TestCase(), ActionParameters().Parameter(1));
	
	if (accountId1.iPopAccountId != accountId2.iPopAccountId)
		{
		// failed
		TestCase().ERR_PRINTF1(_L("POP accounts do not match !"));
		TestCase().SetTestStepResult(EFail);
		}
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionComparePopAccountId);
	TestCase().ActionCompletedL(*this);
	}


