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
// GetPopAccountInArray
// [Action Parameters]
// indexNumber		<input>:		Index of account to get
// accountId		<output>:		TPOP3Account ID
// Note :  TPopAccounts is defined as typedef RArray<TPOP3AccountId> TPopAccounts 
// see TMtfTestParameterType.h for definition and parameter implementation
// [Action Description]
// GetPopAccountInArray Test Action is intended to get a TPOP3AccountId specified 
// by indexNumebr from an TPopAccounts array.
// [APIs Used]	
// __ACTION_INFO_END__
// 
//


#include <cemailaccounts.h>

#include "CMtfTestActionGetPopAccountInArray.h"
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
CMtfTestAction* CMtfTestActionGetPopAccountInArray::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetPopAccountInArray* self = new (ELeave) CMtfTestActionGetPopAccountInArray(aTestCase);
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
CMtfTestActionGetPopAccountInArray::CMtfTestActionGetPopAccountInArray(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
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
CMtfTestActionGetPopAccountInArray::~CMtfTestActionGetPopAccountInArray()
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
void CMtfTestActionGetPopAccountInArray::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetPopAccountInArray);
	TInt indexNumber = ObtainValueParameterL<TInt>(TestCase(), ActionParameters().Parameter(0));
	CEmailAccounts* accounts = CEmailAccounts::NewLC();	
	RArray<TPopAccount> accountArray;
	CleanupClosePushL(accountArray);
	accounts->GetPopAccountsL(accountArray);

	// make sure we're not out of range
	if (indexNumber >= 0 && indexNumber < accountArray.Count())
		{
		TPopAccount id = accountArray[indexNumber];		
		StoreParameterL<TPopAccount>( TestCase(), id, ActionParameters().Parameter(1));
		}
	else
		{
		TestCase().ERR_PRINTF1(_L("Index is out of range!"));
		TestCase().SetTestStepResult(EFail);
		}
	
	CleanupStack::PopAndDestroy( 2, accounts );	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetPopAccountInArray);
	
	TestCase().ActionCompletedL(*this);	
	}
