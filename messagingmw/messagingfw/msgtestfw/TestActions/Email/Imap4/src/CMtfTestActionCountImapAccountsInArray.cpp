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
// CountImapAccountsInArray
// [Action Parameters]
// accountArray		<input>:		TImapAccounts
// numberToMatch	<input>:		Number to match count of array objects
// Note :  TImapAccounts is defined as typedef RArray<TIMAP4AccountId> TImapAccounts 
// see TMtfTestParameterType.h for definition and parameter implementation
// [Action Description]
// CountImapAccountsInArray Test Action is intended to count the number of objects in
// an TImapAccounts array and compare them against the provided number. Test action fails 
// if count and number do not match.
// [APIs Used]	
// TImapAccounts::Count
// __ACTION_INFO_END__
// 
//

#include <cemailaccounts.h>

#include "CMtfTestActionCountImapAccountsInArray.h"
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
CMtfTestAction* CMtfTestActionCountImapAccountsInArray::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCountImapAccountsInArray* self = new (ELeave) CMtfTestActionCountImapAccountsInArray(aTestCase);
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
CMtfTestActionCountImapAccountsInArray::CMtfTestActionCountImapAccountsInArray(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
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
CMtfTestActionCountImapAccountsInArray::~CMtfTestActionCountImapAccountsInArray()
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
void CMtfTestActionCountImapAccountsInArray::ExecuteActionL()
{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCountImapAccountsInArray);
	TInt numberToMatch = ObtainValueParameterL<TInt>(TestCase(), ActionParameters().Parameter(0));
	
	CEmailAccounts *account = CEmailAccounts::NewLC();	
	RArray<TImapAccount> arrayImapAccounts;
	account->GetImapAccountsL(arrayImapAccounts);
	TInt count = arrayImapAccounts.Count();
	
	CleanupStack::PopAndDestroy(account);
	arrayImapAccounts.Reset();
	
	if (count != numberToMatch)
	{
		// failed
		TestCase().ERR_PRINTF3(_L("Number of IMAP accounts do not match ! expected = %d actual = %d"), 
								  numberToMatch, count );
		TestCase().SetTestStepResult(EFail);
	}
	TestCase().INFO_PRINTF2(_L("Number of IMAP accounts matched value = %d !"), numberToMatch );	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCountImapAccountsInArray);
	TestCase().ActionCompletedL(*this);
	
}
