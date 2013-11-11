// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CountAvailableAccounts
// [Action Parameters]
// CSendAsAccounts sendAsAccounts <input> : Reference to the CSendAsAccounts object
// TInt		expectedCount  <input>: expected number of accounts names.
// TInt 	index 		   <input>: Index of the account name to be verified
// HbufC	accountName	   <input>: Expected account name
// [Action Description]
// Count Number of Available SendAs Account names. Verifies the account 
// name string present at the specified index
// [APIs Used]
// CSendAsAccounts::AccountNames()
// __ACTION_INFO_END__
// 
//

/**
 @file
*/
//system include
#include <sendas2.h>

//user include 
#include "CMtfTestActionCountAvailableAccounts.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  NewL()
  Constructs a CMtfTestActionCountAvailableAccounts object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionCountAvailableAccounts
  @pre    None
  @post   CMtfTestActionCountAvailableAccounts object is created
*/
CMtfTestAction* CMtfTestActionCountAvailableAccounts::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCountAvailableAccounts* self = new (ELeave) CMtfTestActionCountAvailableAccounts(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	
/**
  CMtfTestActionCountAvailableAccounts constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionCountAvailableAccounts::CMtfTestActionCountAvailableAccounts(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionCountAvailableAccounts
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionCountAvailableAccounts::~CMtfTestActionCountAvailableAccounts()
	{
	}

/**
  ExecuteActionL
	Verifies the count of the account names. Test case fails if the number of 
	accounts does not match with the expected value.
	Compares the account name present at the specified index with the expected value.
	The test case fails if the account name does not match with the expected value.
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionCountAvailableAccounts::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCountAvailableAccounts);
		
		// Get test action input parameters
		CSendAsAccounts* paramAvailableAccounts	= ObtainParameterReferenceL<CSendAsAccounts>(TestCase(),
																		ActionParameters().Parameter(0));
				
		TInt paramExpectedCount = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1));


		TInt paramIndex = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));


		HBufC* paramAccountName = ObtainParameterReferenceL<HBufC>(TestCase(),
																ActionParameters().Parameter(3));
														
														
														
		TInt accountsCount = paramAvailableAccounts->AccountNames().MdcaCount();
		
		if(accountsCount == paramExpectedCount)
			{
			TestCase().INFO_PRINTF2(_L("Number of accounts matched the expcted count value %d"), accountsCount);
			}
		else
			{
			TestCase().ERR_PRINTF3(_L("Number of accounts did not match. Number of available accounts = %d, expected value = %d"), accountsCount, paramExpectedCount);
			TestCase().SetTestStepResult(EFail);
			}
			
		if((TestCase().TestStepResult()) == EPass)
			{
			TBufC<256> accountname = paramAvailableAccounts->AccountNames().MdcaPoint(paramIndex);
			
			if((accountname.Compare(paramAccountName->Des())) == 0)
				{
				TestCase().INFO_PRINTF1(_L("Comparison of account name was successful"));
				}
			else
				{
				TestCase().ERR_PRINTF1(_L("Comparison of account name falied"));
				TestCase().SetTestStepResult(EFail);
				}
			}
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCountAvailableAccounts);	
		}
		
	TestCase().ActionCompletedL(*this);
	}

