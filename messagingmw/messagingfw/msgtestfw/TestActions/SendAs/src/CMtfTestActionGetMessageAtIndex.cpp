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
// CMsvEntrySelection	paramMessageSelection <input>	: Reference to Mail selection
// TInt					index	<input>					: Value of Index
// TMsvId				paramMessageId	<output>		: Value of message Id
// [Action Description]
// GetMessagAtIndex Test Action takes a CMsvEntrySelection and an array index 
// value as inputs.  The Test Action provides the TMsvId present in the 
// CMsvEntrySelection at the specified index as output
// [APIs Used]
// CMsvEntrySelection::At()
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionGetMessageAtIndex.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"



/**
  NewL()
  Constructs a CMtfTestActionGetMessageAtIndex object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionGetMessageAtIndex
  @pre    None
  @post   CMtfTestActionGetMessageAtIndex object is created
*/
CMtfTestAction* CMtfTestActionGetMessageAtIndex::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetMessageAtIndex* self = 
							new (ELeave) CMtfTestActionGetMessageAtIndex(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionGetMessageAtIndex constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionGetMessageAtIndex::CMtfTestActionGetMessageAtIndex(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionGetMessageAtIndex
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionGetMessageAtIndex::~CMtfTestActionGetMessageAtIndex()
	{
	}

/**
  ExecuteActionL
  Obtains the parameters for the test action. Get the Message Id at given index. The Message 
  Id is stored as output parameters of this Test Action
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionGetMessageAtIndex::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetMessageAtIndex);
		
		// Get test action input parameters
		CMsvEntrySelection* paramSelection = ObtainParameterReferenceL<CMsvEntrySelection>(TestCase(),
																	ActionParameters().Parameter(0));
			
		TInt index = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1));
	
		// Validate the index value provided
		TMsvId paramMessageId;
		TInt length = paramSelection->Length();

		if(index > length)
			{
			TestCase().ERR_PRINTF3(_L("Index out of bounds, length of selection: %d, input provided = %d"),length, index);
			TestCase().SetTestStepResult(EFail);
			}
		else
			{
			// Valid index, get the message Id and store it as output parameter
			paramMessageId = paramSelection->At(index);
			StoreParameterL<TMsvId>(TestCase(),paramMessageId,ActionParameters().Parameter(2));
			}
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetMessageAtIndex);
		}
	TestCase().ActionCompletedL(*this);
	}

