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
// SendAsGetFilteredMessageTypes
// [Action Parameters]
// RSendAs				paramSendAs			 <input> : Reference to the RSendAs object
// CSendAsMessageTypes	filteredMessageTypes <output>: Reference to the 
// CSendAsMessageTypes object
// [Action Description]
// Gets the list of Client MTMs that support SendAs feature
// [APIs Used]
// RSendAs::FilteredMessageTypes
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

// System include
#include <csendasmessagetypes.h>

// User include
#include "CMtfTestActionSendAsGetFilteredMessageTypes.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  NewL()
  Constructs a CMtfTestActionSendAsGetFilteredMessageTypes object.
  Uses two phase construction.
  @internalTechnology
  @param  aTestCase				Test Case to which this Test Action belongs
  @param  aActionParameters		Test Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsGetFilteredMessageTypes
  @pre    None
  @post   CMtfTestActionSendAsGetFilteredMessageTypes object is created
*/
CMtfTestAction* CMtfTestActionSendAsGetFilteredMessageTypes::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsGetFilteredMessageTypes* self = 
				new (ELeave) CMtfTestActionSendAsGetFilteredMessageTypes(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsGetFilteredMessageTypes constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsGetFilteredMessageTypes::CMtfTestActionSendAsGetFilteredMessageTypes(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsGetFilteredMessageTypes
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsGetFilteredMessageTypes::~CMtfTestActionSendAsGetFilteredMessageTypes()
	{
	}

/**
  ExecuteActionL
  Creates a CSendAsMessageTypes object and gets the Filtered message types that support SendAs
  @internalTechnology 
  @pre    A connection has been established with SendAs server
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsGetFilteredMessageTypes::ExecuteActionL()
	{
	if( (TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start ..."), &KTestActionSendAsGetFilteredMessageTypes);
					
		// Get test action input parameters			
		RSendAs paramSendAs = ObtainValueParameterL<RSendAs>(TestCase(),
										ActionParameters().Parameter(0));

		CSendAsMessageTypes* filteredMessageTypes = CSendAsMessageTypes::NewL();
		CleanupStack::PushL(filteredMessageTypes);

		// Get the list of message types that support SendAs
		TRAPD(err, paramSendAs.FilteredMessageTypesL(*filteredMessageTypes));
		
		// Failure condition: fail the Test Case
		if (err != KErrNone)
			{
			TestCase().ERR_PRINTF2(_L("Getting filtered message types failed with error %d "), err);
			TestCase().SetTestStepResult(EFail);
			}
		else
			{
			// Success: provide the CSendAsMessageTypes object as output of the Test Action
			StoreParameterL<CSendAsMessageTypes>(TestCase(), *filteredMessageTypes,
													ActionParameters().Parameter(1));
			
			TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsGetFilteredMessageTypes);
			}
		// Pop filteredMessageTypes, do not destroy, it is stored as an output parameter
		CleanupStack::Pop(filteredMessageTypes);
		}
	TestCase().ActionCompletedL(*this);
	}
