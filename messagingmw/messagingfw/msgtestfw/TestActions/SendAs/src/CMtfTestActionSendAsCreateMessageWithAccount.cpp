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
// SendAsCreateMessageWithAccount
// [Action Parameters]
// RSendAs			sendAs				:	Reference to the RSendAs object
// RSendAsMessage	sendAsMessage		:	Reference to the RSendAsMessage object with which message is to be created
// TSendAsAccount	account				:	Account 
// [Action Description]
// SendAsCreateMessageWithAccount Test Action is intended to create a message with 
// the specified account; the type of the message depends on the type of the account specified.
// [APIs Used]
// RSendAsMessage::Create (RSendAs& aSendAs, TSendAsAccount aAccount)
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionSendAsCreateMessageWithAccount.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  NewL()
  Constructs a CMtfTestActionSendAsCreateMessageWithAccount object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionGetMessageAtIndex
  @pre    None
  @post   CMtfTestActionSendAsCreateMessageWithAccount object is created
*/
CMtfTestAction* CMtfTestActionSendAsCreateMessageWithAccount::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsCreateMessageWithAccount* self = 
							new (ELeave) CMtfTestActionSendAsCreateMessageWithAccount(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsCreateMessageWithAccount constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsCreateMessageWithAccount::CMtfTestActionSendAsCreateMessageWithAccount(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsCreateMessageWithAccount
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsCreateMessageWithAccount::~CMtfTestActionSendAsCreateMessageWithAccount()
	{
	}
	
/**
  ExecuteActionL
  Obtains the parameters for the test action. Get the Message Id at given index. The Message 
  Id is stored as output parameters of this Test Action
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsCreateMessageWithAccount::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsCreateMessageWithAccount);
	RSendAs paramSendAs	 = ObtainValueParameterL<RSendAs>(TestCase(),
									ActionParameters().Parameter(0));
	TSendAsAccount paramAccount = ObtainValueParameterL<TSendAsAccount>(TestCase(),
									ActionParameters().Parameter(1));
	RSendAsMessage sendAsMessage;

	sendAsMessage.CreateL(paramSendAs, paramAccount);
	
	StoreParameterL<RSendAsMessage>(TestCase(),sendAsMessage,ActionParameters().Parameter(2));
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsCreateMessageWithAccount);
	TestCase().ActionCompletedL(*this);
	}

