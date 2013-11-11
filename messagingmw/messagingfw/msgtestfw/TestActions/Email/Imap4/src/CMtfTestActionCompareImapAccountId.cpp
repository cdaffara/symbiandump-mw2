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
// CompareImapAccountId
// [Action Parameters]
// accountId1			input		TIMAP4Account
// accountId2			input		TIMAP4Account
// [Action Description]
// CompareImapAccountId Test Action is intended to compare the IMAP account Ids.
// [APIs Used]	
// none
// __ACTION_INFO_END__
// 
//



//#include <centralrepository.h>
#include <cemailaccounts.h>

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionCompareImapAccountId.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionCompareImapAccountId object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionCompareImapAccountId::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCompareImapAccountId* self = new (ELeave) CMtfTestActionCompareImapAccountId(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : ~CMtfTestActionCompareImapAccountId
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionCompareImapAccountId::~CMtfTestActionCompareImapAccountId()
	{
	}
	
/**
  Function : CMtfTestActionCompareImapAccountId
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionCompareImapAccountId::CMtfTestActionCompareImapAccountId(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
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
void CMtfTestActionCompareImapAccountId::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCompareImapAccountId);
	TImapAccount accountId1 = ObtainValueParameterL<TImapAccount>( TestCase(),ActionParameters().Parameter(0) );				
	TImapAccount accountId2 = ObtainValueParameterL<TImapAccount>( TestCase(),ActionParameters().Parameter(1) );

	if (accountId1.iImapAccountId != accountId2.iImapAccountId)
		{
		// failed
		TestCase().ERR_PRINTF1(_L("IMAP accounts do not match !"));
		TestCase().SetTestStepResult(EFail);
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCompareImapAccountId);	
	TestCase().ActionCompletedL(*this);

	}

