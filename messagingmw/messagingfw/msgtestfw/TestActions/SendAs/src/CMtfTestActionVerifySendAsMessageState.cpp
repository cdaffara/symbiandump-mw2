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
// GetMessageAtIndex
// [Action Parameters]
// RSendAsMessage	paramRSendAsMessage <input>	: Reference of the RSendAsMessage object
// TSendAsState		paramSendAsState	<input>	: One of the enumerations of type TSendAsState 
// [Action Description]
// Test Action is intended to get the progress information of message and the compare the 
// state of the message with the expected state.  The expected state of the message is 
// provided as an input the Test Action.
// [APIs Used]
// RSendAsMessage::Progress ()
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/

// User include
#include "CMtfTestActionVerifySendAsMessageState.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "sendas2.h"


/**
  NewL()
  Constructs a CMtfTestActionVerifySendAsMessageState object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionVerifySendAsMessageState
  @pre    None
  @post   CMtfTestActionVerifySendAsMessageState object is created
*/
CMtfTestAction* CMtfTestActionVerifySendAsMessageState::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionVerifySendAsMessageState* self = 
							new (ELeave) CMtfTestActionVerifySendAsMessageState(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionVerifySendAsMessageState constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionVerifySendAsMessageState::CMtfTestActionVerifySendAsMessageState(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionVerifySendAsMessageState
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionVerifySendAsMessageState::~CMtfTestActionVerifySendAsMessageState()
	{
	}

/**
  ExecuteActionL
  Obtains the parameters for the test action. Create a TSendAsState type object   
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionVerifySendAsMessageState::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionVerifySendAsMessageState);
	RSendAsMessage	paramRSendAsMessage = ObtainValueParameterL<RSendAsMessage>(TestCase(),
									ActionParameters().Parameter(0));

	CMsvSendOperation::TSendOperationState paramSendAsState = ObtainValueParameterL<CMsvSendOperation::TSendOperationState>(TestCase(),
															ActionParameters().Parameter(1));

	TSendAsProgress sendAsProgress;
	paramRSendAsMessage.ProgressL(sendAsProgress);
	if (sendAsProgress.iState != paramSendAsState)
		{
		User::Leave(KErrGeneral);
		}

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionVerifySendAsMessageState);
	TestCase().ActionCompletedL(*this);
	}

