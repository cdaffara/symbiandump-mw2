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
// SendAsGetAccount
// [Action Parameters]
// CSendAsAccounts		paramAvailableAccounts	<input>	:	Reference of CSendAsAccounts object
// TInt					paramIndex				<input>	:	Index value
// TSendAsAccount		paramAccount			<output>:	Account object present at specified index
// [Action Description]
// Gets the account from the CSendAsAccounts object which is present at the specified index
// [APIs Used]
// CSendAsAccounts::Account (TInt aIndex) const
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionSendAsGetAccount.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  NewL()
  Constructs a CMtfTestActionSendAsGetAccount object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsGetAccount
  @pre    None
  @post   CMtfTestActionSendAsGetAccount object is created
*/
CMtfTestAction* CMtfTestActionSendAsGetAccount::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsGetAccount* self = 
							new (ELeave) CMtfTestActionSendAsGetAccount(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsGetAccount constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsGetAccount::CMtfTestActionSendAsGetAccount(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsGetAccount
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsGetAccount::~CMtfTestActionSendAsGetAccount()
	{
	}
	
/**
  ExecuteActionL
	Obtain the input parameters
	1.	paramAvailableAccounts
	2.	paramIndex
	Create a TSendAsAccount object
	Call availableAccoutns::Account(TInt aIndex) passing the index value as input 
	parameter, store the returned value in the TSendAsAccount object
	Store the TSendAsAccount object as the output parameter of the Test Action by 
	calling the StoreParameterL() function

  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsGetAccount::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsGetAccount);
		
		// Get test action input parameters	
		CSendAsAccounts* paramAvailableAccounts	= ObtainParameterReferenceL<CSendAsAccounts>(TestCase(),
																		ActionParameters().Parameter(0));
		TInt paramIndex 						= ObtainValueParameterL<TInt>(TestCase(),
																		ActionParameters().Parameter(1));

		TSendAsAccount paramAccount = paramAvailableAccounts->Account(paramIndex);

		StoreParameterL<TSendAsAccount>(TestCase(),paramAccount,ActionParameters().Parameter(2));
		
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsGetAccount);
		}
	TestCase().ActionCompletedL(*this);
	}

