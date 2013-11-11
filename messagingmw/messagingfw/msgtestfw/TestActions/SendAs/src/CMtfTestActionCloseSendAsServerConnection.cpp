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
// CloseSendAsServerConnection
// [Action Parameters]
// RSendAs	sendAs <input>	: RSendAs object
// [Action Description]
// Close the connection that is established with the SendAs server
// [APIs Used]
// RSendAs::Close()
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionCloseSendAsServerConnection.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  NewL()
  Constructs a CMtfTestActionCloseSendAsServerConnection object.
  Uses two phase construction
  @internalTechnology
  @param  aTestCase				Test Case to which this Test Action belongs
  @param  aActionParameters		Test Action parameters, must not be NULL
  @return Created object of type CMtfTestActionGetMessageAtIndex
  @pre    None
  @post   None
*/
CMtfTestAction* CMtfTestActionCloseSendAsServerConnection::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCloseSendAsServerConnection* self = 
				new (ELeave) CMtfTestActionCloseSendAsServerConnection(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionCloseSendAsServerConnection constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionCloseSendAsServerConnection::CMtfTestActionCloseSendAsServerConnection
											(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionCloseSendAsServerConnection
  Description : Destructor
  @internalTechnology
  @pre None
  @post: None
*/
CMtfTestActionCloseSendAsServerConnection::~CMtfTestActionCloseSendAsServerConnection()
	{
	}
	
/**
  ExecuteActionL
  Close the connection with the SendAs server.
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionCloseSendAsServerConnection::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCloseSendAsServerConnection);
		
		// Get test action input parameters
		RSendAs paramSendAs = ObtainValueParameterL<RSendAs>(TestCase(),
									ActionParameters().Parameter(0));
		// Close the handle
		paramSendAs.Close();

		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCloseSendAsServerConnection);
		}
	TestCase().ActionCompletedL(*this);
	}

