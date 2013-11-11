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
// SendAsCloseMessage
// [Action Parameters]
// RSendAsMessage	sendAsMessage	<input> : Reference to the RSendAsMessage object
// [Action Description]
// Close the RSendAsMessage object.  If a message is created with the RSendAsMessage
// object and is not sent, closing the RSendAsMesssage object leaves the message in
// the local Drafts folder.
// [APIs Used]
// RSendAsMessage::Close()
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionSendAsCloseMessage.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  NewL()
  Constructs a CMtfTestActionSendAsCloseMessage object.
  Uses two phase construction.
  @internalTechnology
  @param  aTestCase				Test Case to which this Test Action belongs
  @param  aActionParameters		Test Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsCloseMessage
  @pre    None
  @post   None
*/
CMtfTestAction* CMtfTestActionSendAsCloseMessage::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsCloseMessage* self = 
							new (ELeave) CMtfTestActionSendAsCloseMessage(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsCloseMessage constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsCloseMessage::CMtfTestActionSendAsCloseMessage(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsCloseMessage
  Description : Destructor
  @internalTechnology
  @pre None
  @post None
*/
CMtfTestActionSendAsCloseMessage::~CMtfTestActionSendAsCloseMessage()
	{
	}
	
/**
  ExecuteActionL
  Close the RSendAsMessage
  @internalTechnology 
  @pre    None
  @post   None
  @leave  None
*/
void CMtfTestActionSendAsCloseMessage::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsCloseMessage);
			
		RSendAsMessage paramSendAsMessage = ObtainValueParameterL<RSendAsMessage>
											(TestCase(), ActionParameters().Parameter(0));
		paramSendAsMessage.Close();

		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsCloseMessage);
		}

	TestCase().ActionCompletedL(*this);
	}
