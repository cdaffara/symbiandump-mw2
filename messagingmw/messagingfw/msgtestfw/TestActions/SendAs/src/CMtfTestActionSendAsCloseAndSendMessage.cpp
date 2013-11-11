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
// SendAsCloseAndSendMessage
// [Action Parameters]
// RSendAsMessage	paramRSendAsMessage <input>	: Reference of the RSendAsMessage object
// TInt				closeAndSendMessageConfirmed	<input>	: 0 if the Close and Send Message 
// operation is not a confirmed operation, 
// 1 if the Close and Send Message operation
// has to be a confirmed operation. Default value is 0
// ErrorCode	  <output>: (Optional) Returned error code, if not requested then code will
// leave if not KErrNone
// [Action Description]
// Test Action is intended to request sending the message and closing the RSendAsMessage 
// object.  The Send-As server performs a confirmed or unconfirmed Close and Send Message
// operation based on the flag provided as second input parameter to the Test Action.
// Default action is to perform an unconfirmed Close and Send Message operation.
// [APIs Used]
// RSendAsMessage::CloseAndSendMessage ()
// RSendAsMessage::CloseAndSendMessageConfirmed ()
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/

// User include
#include "CMtfTestActionSendAsCloseAndSendMessage.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "sendas2.h"


/**
  NewL()
  Constructs a CMtfTestActionSendAsCloseAndSendMessage object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsCloseAndSendMessage
  @pre    None
  @post   CMtfTestActionSendAsCloseAndSendMessage object is created
*/
CMtfTestAction* CMtfTestActionSendAsCloseAndSendMessage::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsCloseAndSendMessage* self = 
							new (ELeave) CMtfTestActionSendAsCloseAndSendMessage(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsCloseAndSendMessage constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsCloseAndSendMessage::CMtfTestActionSendAsCloseAndSendMessage(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsCloseAndSendMessage
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsCloseAndSendMessage::~CMtfTestActionSendAsCloseAndSendMessage()
	{
	}

/**
  ExecuteActionL
  Obtains the parameters for the test action. 
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsCloseAndSendMessage::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsCloseAndSendMessage);
	RSendAsMessage	paramRSendAsMessage = ObtainValueParameterL<RSendAsMessage>(TestCase(),ActionParameters().Parameter(0));
	TInt paramCloseAndSendMessageConfirmed = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1), EFalse);
	
	TInt err = KErrNone;
	
	if (paramCloseAndSendMessageConfirmed)
		{
		TRAP(err,paramRSendAsMessage.SendMessageConfirmedAndCloseL());
		}
	else
		{
		TRAP(err,paramRSendAsMessage.SendMessageAndCloseL());
		}

	if( ActionParameters().Count() > 2 )
		{
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(2));
		}
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsCloseAndSendMessage);
	TestCase().ActionCompletedL(*this);
	}

