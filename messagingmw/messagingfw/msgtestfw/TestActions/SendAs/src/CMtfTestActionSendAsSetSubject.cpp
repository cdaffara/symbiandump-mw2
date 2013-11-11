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
// SendAsSetSubject
// [Action Parameters]
// RSendAsMessage		sendAsMessage<input> : Reference of  the RSendAsMessage object
// HBufC				Subject		 <input> : String containing the Subject to be set 
// for the message
// [Action Description]
// Sets subject field for a message created using SendAs API
// [APIs Used]
// RSendAsMessage::SetSubject (const TDesC& aSubject)
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionSendAsSetSubject.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  NewL()
  Constructs a CMtfTestActionSendAsSetSubject object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsSetSubject
  @pre    None
  @post   CMtfTestActionSendAsSetSubject object is created
*/
CMtfTestAction* CMtfTestActionSendAsSetSubject::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsSetSubject* self = 
							new (ELeave) CMtfTestActionSendAsSetSubject(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsSetSubject constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsSetSubject::CMtfTestActionSendAsSetSubject(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsSetSubject
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsSetSubject::~CMtfTestActionSendAsSetSubject()
	{
	}
	
/**
  ExecuteActionL
   Sets the subject for a message created using SendAs API
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsSetSubject::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsSetSubject);
	
		// Get Test action input parameters
		RSendAsMessage sendAsMessage = ObtainValueParameterL<RSendAsMessage>(TestCase(),
												ActionParameters().Parameter(0));

		HBufC* subject = ObtainParameterReferenceL<HBufC>(TestCase(),
												ActionParameters().Parameter(1));
		
		TInt paramExpectedResult = ObtainValueParameterL<TInt>(TestCase(),
												ActionParameters().Parameter(2));
	
		// Set the subject field
		TRAPD(err,sendAsMessage.SetSubjectL(subject->Des()));
		
		if(err != paramExpectedResult)
			{
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to set subject") , &KTestActionSendAsSetSubject);
			TestCase().SetTestStepResult(EFail);
			}
		else if((paramExpectedResult == KErrNone) && (paramExpectedResult == err))
			{
			TestCase().INFO_PRINTF2(_L("Subject filed was set successfully."), &KTestActionSendAsSetSubject);
			}
		else
			{
			TestCase().INFO_PRINTF2(_L("Set Subject filed: The error code matches the expected error value %d"), err);
			}
			
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsSetSubject);		
	TestCase().ActionCompletedL(*this);
	}

