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
// GetImapAccountInArray
// [Action Parameters]
// accountArray		<input>:		TImapAccounts
// indexNumber		<input>:		Index of account to get
// accountId		<output>:		TIMAP4Account ID
// Note :  TImapAccounts is defined as typedef RArray<TIMAP4AccountId> TImapAccounts 
// see TMtfTestParameterType.h for definition and parameter implementation
// [Action Description]
// GetImapAccountInArray Test Action is intended to get a TIMAP4AccountId specified 
// by indexNumebr from an TImapAccounts array.
// [APIs Used]	
// __ACTION_INFO_END__
// 
//


#include <cemailaccounts.h>


#include "CMtfTestActionGetImapAccountInArray.h"
#include "TMtfTestParameterType.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionGetImapAccounts object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionGetImapAccountInArray::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetImapAccountInArray* self = new (ELeave) CMtfTestActionGetImapAccountInArray(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionGetImapAccounts
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionGetImapAccountInArray::CMtfTestActionGetImapAccountInArray(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionGetImapAccounts
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionGetImapAccountInArray::~CMtfTestActionGetImapAccountInArray()
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
void CMtfTestActionGetImapAccountInArray::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetImapAccountInArray);
	TInt indexNumber = ObtainValueParameterL<TInt>(TestCase(), ActionParameters().Parameter(0));
	CEmailAccounts* account = CEmailAccounts::NewLC();	
	RArray<TImapAccount> accountArray;
	CleanupClosePushL(accountArray);
	account->GetImapAccountsL(accountArray);
	
	// make sure we're not out of range
	if (indexNumber >= 0 && indexNumber < accountArray.Count())
		{
		TImapAccount id = accountArray[indexNumber];	
		StoreParameterL<TImapAccount>(TestCase(), id, ActionParameters().Parameter(1));
		}
	else
		{
		TestCase().ERR_PRINTF1(_L("Index is out of range!"));
		TestCase().SetTestStepResult(EFail);
		}

	CleanupStack::PopAndDestroy( 2, account );	
	

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetImapAccountInArray);
	TestCase().ActionCompletedL(*this);	
	}
