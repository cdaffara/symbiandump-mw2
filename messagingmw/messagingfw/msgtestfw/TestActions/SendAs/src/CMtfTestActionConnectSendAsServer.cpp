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
// ConnectSendAsServer
// [Action Parameters]
// messageSlots  TInt		<input>	: Number of message slots
// rSendAs		 RSendAs	<output>: Created RSendAs object
// [Action Description]
// Establishes a connection with SendAs server
// [APIs Used]
// RSendAs::Connect
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

//user include 
#include "CMtfTestActionConnectSendAsServer.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  NewL()
  Constructs a CMtfTestActionConnectSendAsServer object.
  Uses two phase construction
  @internalTechnology
  @param	aTestCase			Test Case to which this Test Action belongs
  @param	aActionParameters	Test Action parameters, must not be NULL
  @return	Created object of type CMtfTestActionConnectSendAsServer 
  @pre		None
  @post		CMtfTestActionConnectSendAsServer object is created
*/
CMtfTestAction* CMtfTestActionConnectSendAsServer::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionConnectSendAsServer* self = new (ELeave) CMtfTestActionConnectSendAsServer(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	
/**
  CMtfTestActionConnectSendAsServer constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionConnectSendAsServer::CMtfTestActionConnectSendAsServer(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Destructor
*/
CMtfTestActionConnectSendAsServer::~CMtfTestActionConnectSendAsServer()
	{
	}


/**
  ExecuteActionL
  Creates an RSendAs object and establishes a connection with SendAs server
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionConnectSendAsServer::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S Start..."), &KTestActionConnectSendAsServer);
		
		// Get the number of message slots to be supported
		TInt messageSlots = ObtainValueParameterL<TInt> (TestCase(),
									ActionParameters().Parameter(0));
		
		// Create an RSendAs object and establish connection with SendAs server
		RSendAs rSendAs;
		TInt err = rSendAs.Connect(messageSlots);

		// Fail the Test Case if the connection is not established
		if (err != KErrNone)
			{
			TestCase().ERR_PRINTF2(_L("Connection with the SendAs server is failed with error %d "), err);
			TestCase().SetTestStepResult(EFail);
			}
		else
			{
			// Connection successful, Provide the RSendAs object as an output of the Test Action
			StoreParameterL<RSendAs>(TestCase(),rSendAs,ActionParameters().Parameter(1));
			}
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionConnectSendAsServer);
	TestCase().ActionCompletedL(*this);
	}
