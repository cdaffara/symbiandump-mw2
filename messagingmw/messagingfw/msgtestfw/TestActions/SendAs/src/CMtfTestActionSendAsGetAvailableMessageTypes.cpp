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
// SendAsGetAvailableMessageTypes
// [Action Parameters]
// CSendAsMessageTypes		paramMessageTypes <input>	:Reference to CSendAsMessageTypes
// object
// CDesC16Array				mtmArray		  <output>	:Array containing MTM names
// [Action Description]
// Gets the list of available message types from the list of filtered MTMs.
// The filtered MTM list is got using the SendAsGetFilteredMessageTypes Test Action.
// [APIs Used]
// CSendAsMessageTypes:: AvailableMessageTypes ( )
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/

// System include
#include <csendasmessagetypes.h>

// User include
#include "CMtfTestActionSendAsGetAvailableMessageTypes.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "sendas2.h"

/**
  NewL()
  Constructs a CMtfTestActionSendAsGetAvailableMessageTypes object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsGetAvailableMessageTypes
  @pre    None
  @post   CMtfTestActionSendAsGetAvailableMessageTypes object is created
*/
CMtfTestAction* CMtfTestActionSendAsGetAvailableMessageTypes::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsGetAvailableMessageTypes* self = 
							new (ELeave) CMtfTestActionSendAsGetAvailableMessageTypes(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsGetAvailableMessageTypes constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsGetAvailableMessageTypes::CMtfTestActionSendAsGetAvailableMessageTypes(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsGetAvailableMessageTypes
  Description : Destructor
  @internalTechnology
  @pre None
  @post None
*/
CMtfTestActionSendAsGetAvailableMessageTypes::~CMtfTestActionSendAsGetAvailableMessageTypes()
	{
	}


/**
  ExecuteActionL
	Store the reference of CdesC16Array returned by AvailableMessageTypes()
	as output parameter
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsGetAvailableMessageTypes::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsGetAvailableMessageTypes);
	if((TestCase().TestStepResult()) == EPass)
		{
		// Get the test action input parameter
		CSendAsMessageTypes* paramMessageTypes = ObtainParameterReferenceL
												<CSendAsMessageTypes>(TestCase(),
												ActionParameters().Parameter(0));
												
		// Get the array of message types that support SendAS	
		const CDesC16Array& mtmArray = reinterpret_cast<const CDesC16Array&>
										(paramMessageTypes->AvailableMessageTypes());

		CDesC16Array& tempMtmArray = const_cast<CDesC16Array&>(mtmArray);

		// Provide the array of message types as test action output
		StoreParameterL<CDesC16Array>(TestCase(),tempMtmArray,
												ActionParameters().Parameter(1));

		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsGetAvailableMessageTypes);
		}

	TestCase().ActionCompletedL(*this);
	}
