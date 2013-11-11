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
// SendAsLaunchEditorAndClose
// [Action Parameters]
// RSendAsMessage		paramSendAsMessage	<input>		:	Reference of  the RSendAsMessage object
// [Action Description]
// SendAsLaunchEditorAndClose Test Action is intended launch the 
// message editor in "Fire and Forget" mode: the message created 
// using Send-As API is closed and is opened in the corresponding message editor. 
// [APIs Used]
// RSendAsMessage::LaunchEditorAndClose ()
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionSendAsLaunchEditorAndClose.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  NewL()
  Constructs a CMtfTestActionSendAsLaunchEditorAndClose object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsLaunchEditorAndClose
  @pre    None
  @post   CMtfTestActionSendAsLaunchEditorAndClose object is created
*/
CMtfTestAction* CMtfTestActionSendAsLaunchEditorAndClose::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsLaunchEditorAndClose* self = 
							new (ELeave) CMtfTestActionSendAsLaunchEditorAndClose(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsLaunchEditorAndClose constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsLaunchEditorAndClose::CMtfTestActionSendAsLaunchEditorAndClose(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsLaunchEditorAndClose
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsLaunchEditorAndClose::~CMtfTestActionSendAsLaunchEditorAndClose()
	{
	}

/**
  ExecuteActionL
  Obtains the input parameters
	1.	paramSendAsMessage 
  Call RSendAsMessage::LaunchEditorAndClose ()

  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsLaunchEditorAndClose::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsLaunchEditorAndClose);
	RSendAsMessage paramSendAsMessage = ObtainValueParameterL<RSendAsMessage>(TestCase(),
									ActionParameters().Parameter(0));

	paramSendAsMessage.LaunchEditorAndCloseL();

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsLaunchEditorAndClose);
	TestCase().ActionCompletedL(*this);
	}

