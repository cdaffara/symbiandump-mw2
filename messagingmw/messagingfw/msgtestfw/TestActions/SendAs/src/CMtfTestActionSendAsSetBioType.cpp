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
// SendAsSetBioType
// [Action Parameters]
// RSendAsMessage		sendAsMessage<input>	: Reference of  the RSendAsMessage object
// TUid					bioType		 <input>	: Value of Bio type UID
// [Action Description]
// GSendAsSetBioType Test Action is intended to set BIO 
// type for the message that is created using Send-As API.
// [APIs Used]
// RSendAsMessage::SetBioType (TUid aBioTypeUid)
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// System include
//#include <smuthdr.h>
#include <bif.h>
#include <bitsids.h>

// User include
#include "CMtfTestActionSendAsSetBioType.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  NewL()
  Constructs a CMtfTestActionSendAsSetBioType object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsSetBioType
  @pre    None
  @post   CMtfTestActionSendAsSetBioType object is created
*/
CMtfTestAction* CMtfTestActionSendAsSetBioType::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsSetBioType* self = 
							new (ELeave) CMtfTestActionSendAsSetBioType(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsSetBioType constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsSetBioType::CMtfTestActionSendAsSetBioType(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsSetBioType
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsSetBioType::~CMtfTestActionSendAsSetBioType()
	{
	}
	
/**
  ExecuteActionL
	Obtains the input parameters
	1.	sendAsMessage
	2.	bioType
	Call RSendAsMessage:: SetBioType(TUid aBioTypeUid) by passing bioType as parameter

  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsSetBioType::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsSetBioType);
	
		// Get test action input parameters
		RSendAsMessage sendAsMessage = ObtainValueParameterL<RSendAsMessage>(TestCase(),
														ActionParameters().Parameter(0));
	
		TInt bioTypeUid = ObtainValueParameterL<TInt>(TestCase(),
														ActionParameters().Parameter(1));
														
		TInt expectedErrorCode = ObtainValueParameterL<TInt>(TestCase(),
														ActionParameters().Parameter(2), KErrNone);
			
		TRAPD(err, sendAsMessage.SetBioTypeL(TUid::Uid(bioTypeUid)));
		
		if ((expectedErrorCode == KErrNone) && (expectedErrorCode == err))
			{
			TestCase().INFO_PRINTF1(_L("The Bio type was set successfully"));
			}
		else if(expectedErrorCode == err)
			{
			TestCase().INFO_PRINTF3(_L("The Bio type failed with the expected error code. Expected error code %d, Error code %d"), expectedErrorCode, err);
			}
		else
			{
			TestCase().ERR_PRINTF2(_L("Setting of Bio Type failed with error %d"), err);
			TestCase().SetTestStepResult(EFail);
			}
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsSetBioType);	
		}
	
	TestCase().ActionCompletedL(*this);
	}

