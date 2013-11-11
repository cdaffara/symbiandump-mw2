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
// SendAsCreateMessageofType
// [Action Parameters]
// RSendAs		sendAs			:	RSendAs object
// TUid			messageType	 	:	Message type ID
// [Action Description]
// Creates a message of the specified message type
// [APIs Used]
// RSendAsMessage::Create(RSendAs& aSendAs, TUid aMessageType) 
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionSendAsCreateMessageofType.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  NewL()
  Constructs a CMtfTestActionSendAsCreateMessageofType object.
  Uses two phase construction.
  @internalTechnology
  @param  aTestCase				Test Case to which this Test Action belongs
  @param  aActionParameters		Test Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsCreateMessageofType
  @pre    None
  @post   CMtfTestActionSendAsCreateMessageofType object is created
*/
CMtfTestAction* CMtfTestActionSendAsCreateMessageofType::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsCreateMessageofType* self = 
							new (ELeave) CMtfTestActionSendAsCreateMessageofType(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsCreateMessageofType constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsCreateMessageofType::CMtfTestActionSendAsCreateMessageofType(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsCreateMessageofType
  Description : Destructor
  @internalTechnology
  @pre 
  @post 
*/
CMtfTestActionSendAsCreateMessageofType::~CMtfTestActionSendAsCreateMessageofType()
	{
	}

/**
  ExecuteActionL
   Creates a message of specified message type in the Drafts folder
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsCreateMessageofType::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsCreateMessageofType);
		
		// Get test action input parameters				
		RSendAs		   paramSendAs		  = ObtainValueParameterL<RSendAs>(TestCase(),
												ActionParameters().Parameter(0));
	
		TUid           paramMessageType	  = ObtainValueParameterL<TUid>(TestCase(),
												ActionParameters().Parameter(1));
	
		RSendAsMessage sendAsMessage;
		
		// Create a message of type paramMessageType in Drafts folder
		TRAPD(err, sendAsMessage.CreateL(paramSendAs, paramMessageType));

		// If message creation falied, fail the Test Case
		if (err != KErrNone)
			{
			TestCase().ERR_PRINTF3(_L("Creation of message of type %d failed with err %d "), paramMessageType, err);
			TestCase().SetTestStepResult(EFail);
			}
		else
			{
			// Store the message handle as output of the test action
			StoreParameterL<RSendAsMessage>(TestCase(),sendAsMessage,
											ActionParameters().Parameter(2));
			TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsCreateMessageofType);
			}
		}
	TestCase().ActionCompletedL(*this);	
	}


