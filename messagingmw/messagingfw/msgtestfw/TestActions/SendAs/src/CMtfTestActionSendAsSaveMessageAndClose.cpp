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
// SendAsSaveMessageAndClose
// [Action Parameters]
// RSendAsMessage	sendAsMessage	<input>	:	RSendAsMessage object
// [Action Description]
// Saves the message in to Drafts folder and closes the message handle
// [APIs Used]
// RSendAsMessage::SendAsSaveMessageAndClose()
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionSendAsSaveMessageAndClose.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  NewL()
  Constructs a SendAsSendAsSaveMessageAndClose object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type SendAsSaveMessageAndClose
  @pre    None
  @post   SendAsSaveMessageAndClose object is created
*/
CMtfTestAction* CMtfTestActionSendAsSaveMessageAndClose::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsSaveMessageAndClose* self = 
							new (ELeave) CMtfTestActionSendAsSaveMessageAndClose(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  SendAsSaveMessageAndClose constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsSaveMessageAndClose::CMtfTestActionSendAsSaveMessageAndClose(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsSaveMessageAndClose
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsSaveMessageAndClose::~CMtfTestActionSendAsSaveMessageAndClose()
	{
	}

/**
  ExecuteActionL
    Saves the message in the Drafts folder and closes the message
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsSaveMessageAndClose::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsSaveMessageAndClose);
	
		// Get Test action input parameters
		RSendAsMessage paramSendAsMessage = ObtainValueParameterL<RSendAsMessage>(TestCase(),
														ActionParameters().Parameter(0));

		// Save and close the message
		TRAPD(err, paramSendAsMessage.SaveMessageAndCloseL());
		
		if (err == KErrNone)
			{
			TestCase().INFO_PRINTF1(_L("Save and Close of message was successful"));	
			}
		else
			{
			TestCase().ERR_PRINTF2(_L("Save and Close of message failed with error %d"), err);
			TestCase().SetTestStepResult(EFail);
			}

		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsSaveMessageAndClose);
		}
	TestCase().ActionCompletedL(*this);
	}

