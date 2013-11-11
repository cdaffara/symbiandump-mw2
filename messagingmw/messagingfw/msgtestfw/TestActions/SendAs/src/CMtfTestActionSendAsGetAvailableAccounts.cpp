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
// SendAsGetAvailableAccounts
// [Action Parameters]
// RSendAs			paramSendAs			<input>	:	Reference to the RSendAs object
// TUid				paramMessageType	<input>	:	Message type ID
// CSendAsAccounts	availableAccounts	<output>:	CSendAsAccounts object
// [Action Description]
// Gets the list of available accounts for the specified message type.
// [APIs Used]
// RSendAs::AvailableAccountsL (TUid aMessagType, CSendAsAccounts& Accounts)
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionSendAsGetAvailableAccounts.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  NewL()
  Constructs a CMtfTestActionSendAsGetAvailableAccounts object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsGetAvailableAccounts
  @pre    None
  @post   CMtfTestActionSendAsGetAvailableAccounts object is created
*/
CMtfTestAction* CMtfTestActionSendAsGetAvailableAccounts::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsGetAvailableAccounts* self = 
							new (ELeave) CMtfTestActionSendAsGetAvailableAccounts(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsGetAvailableAccounts constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsGetAvailableAccounts::CMtfTestActionSendAsGetAvailableAccounts(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsGetAvailableAccounts
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsGetAvailableAccounts::~CMtfTestActionSendAsGetAvailableAccounts()
	{
	}
	
/**
  ExecuteActionL
  Gets the list of available accounts for the specified messag type	
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsGetAvailableAccounts::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsGetAvailableAccounts);
		
		// Get test action input parameters
		RSendAs paramSendAs  = ObtainValueParameterL<RSendAs>(TestCase(),
									         ActionParameters().Parameter(0));

		TUid paramMessageType = ObtainValueParameterL<TUid>(TestCase(),
								 	         ActionParameters().Parameter(1));
		
		CSendAsAccounts* availableAccounts	= CSendAsAccounts::NewL();

		// Get available accounts info
		TRAPD(err, paramSendAs.AvailableAccountsL(paramMessageType,*availableAccounts));

		if( err == KErrNone )
			{
			StoreParameterL<CSendAsAccounts>(TestCase(),*availableAccounts,
												ActionParameters().Parameter(2));
			TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsGetAvailableAccounts);
			}
		else
			{
			TestCase().ERR_PRINTF2(_L("Getting AvailableAccounts failed with err %d "), err);
			TestCase().SetTestStepResult(EFail);
			}
		}

	TestCase().ActionCompletedL(*this);
	}

