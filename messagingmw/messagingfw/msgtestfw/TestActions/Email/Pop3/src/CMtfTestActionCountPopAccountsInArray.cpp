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
// CountPopAccountsInArray
// [Action Parameters]
// accountArray		<input>:		TPopAccounts
// numberToMatch	<input>:		Number to match count of array objects
// Note :  TPopAccounts is defined as typedef RArray<TPOP3AccountId> TPopAccounts 
// see TMtfTestParameterType.h for definition and parameter implementation
// [Action Description]
// CountPopAccountsInArray Test Action is intended to count the number of objects in
// an TPopAccounts array and compare them against the provided number. Test action fails 
// if count and number do not match.
// [APIs Used]	
// TPopAccounts::Count
// __ACTION_INFO_END__
// 
//


// system #includes
#include <cemailaccounts.h>


// user #includes
#include "CMtfTestActionCountPopAccountsInArray.h"
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
CMtfTestAction* CMtfTestActionCountPopAccountsInArray::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCountPopAccountsInArray* self = new (ELeave) CMtfTestActionCountPopAccountsInArray(aTestCase);
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
CMtfTestActionCountPopAccountsInArray::CMtfTestActionCountPopAccountsInArray(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
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
CMtfTestActionCountPopAccountsInArray::~CMtfTestActionCountPopAccountsInArray()
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
void CMtfTestActionCountPopAccountsInArray::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCountPopAccountsInArray);
	TInt numberToMatch = ObtainValueParameterL<TInt>(TestCase(), ActionParameters().Parameter(0));
		
	CEmailAccounts* accounts = CEmailAccounts::NewLC();	
	RArray<TPopAccount> arrayPopAccounts;
	accounts->GetPopAccountsL(arrayPopAccounts);
	TInt count = arrayPopAccounts.Count();
	CleanupStack::PopAndDestroy(accounts);
	arrayPopAccounts.Reset();
	
	if ( count != numberToMatch )
		{
		// failed
		TestCase().ERR_PRINTF3(_L("Number of POP accounts do not match ! expected = %d actual = %d"), 
								  numberToMatch, count );
		TestCase().SetTestStepResult(EFail);
		}
	
	TestCase().INFO_PRINTF2(_L("Number of POP accounts matched value = %d !"), numberToMatch );
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCountPopAccountsInArray);
	TestCase().ActionCompletedL(*this);	
	}
