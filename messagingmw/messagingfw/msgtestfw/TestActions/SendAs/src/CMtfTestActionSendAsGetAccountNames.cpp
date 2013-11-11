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
// SendAsGetAccountNames
// [Action Parameters]
// CSendAsAccounts  paramAvailableAccounts <input>: Reference of CSendAsAccounts object
// TInt	   			acctNames			   <output>: Count of account names
// [Action Description]
// Get the account names from the available accounts and provides the count
// of names as output
// [APIs Used]
// CSendAsAccounts::AccountNames () const
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionSendAsGetAccountNames.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  NewL()
  Constructs a CMtfTestActionSendAsGetAccountNames object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsGetAccountNames
  @pre    None
  @post   CMtfTestActionSendAsGetAccountNames object is created
*/
CMtfTestAction* CMtfTestActionSendAsGetAccountNames::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsGetAccountNames* self = 
							new (ELeave) CMtfTestActionSendAsGetAccountNames(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsGetAccountNames constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsGetAccountNames::CMtfTestActionSendAsGetAccountNames(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsGetAccountNames
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsGetAccountNames::~CMtfTestActionSendAsGetAccountNames()
	{
	}
	
/**
  ExecuteActionL
	Get the array of account names from the available account list
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsGetAccountNames::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsGetAccountNames);
	
		// Get test action input parameters
		CSendAsAccounts* paramAvailableAccounts	= ObtainParameterReferenceL<CSendAsAccounts>(TestCase(),
																		ActionParameters().Parameter(0));
		
		// Get the count of account names
		TInt accountsCount = paramAvailableAccounts->AccountNames().MdcaCount();
		
		// Provide the array of account names as test action output
		StoreParameterL<TInt>(TestCase(), accountsCount, ActionParameters().Parameter(1));

		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsGetAccountNames);
		}
		TestCase().ActionCompletedL(*this);
	}

