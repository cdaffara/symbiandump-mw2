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
// SendAsCheckMultipleRecipientFlag
// [Action Parameters]
// CMsvEntry	paramMsgEntry	   <input>: Reference to the message entry object
// [Action Description]
// SendAsCheckMultipleRecipientFlag Test Action is intended to verify that the Multiple Recipient Flag is set 
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionSendAsCheckMultipleRecipientFlag.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionUtilsMessage.h"

#include "btmsgtypeuid.h"
#include "irmsgtypeuid.h"
#include "MSVSTD.H" 

//Granularity for CArrayFixFlat arrays
const TInt KArrayGranularity = 16;

/**
  NewL()
  Constructs a CMtfTestActionSendAsCheckMultipleRecipientFlag object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsCheckMultipleRecipientFlag
  @pre    None
  @post   CMtfTestActionSendAsCheckMultipleRecipientFlag object is created
*/
CMtfTestAction* CMtfTestActionSendAsCheckMultipleRecipientFlag::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsCheckMultipleRecipientFlag* self = 
							new (ELeave) CMtfTestActionSendAsCheckMultipleRecipientFlag(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsCheckMultipleRecipientFlag constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsCheckMultipleRecipientFlag::CMtfTestActionSendAsCheckMultipleRecipientFlag(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsCheckMultipleRecipientFlag
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsCheckMultipleRecipientFlag::~CMtfTestActionSendAsCheckMultipleRecipientFlag()
	{
	}

/**
  ExecuteActionL
  Verifies whether the Multiple Recipient flag has been set
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsCheckMultipleRecipientFlag::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsCheckMultipleRecipientFlag);
			
		// Get Test Action input parameters
		CMsvEntry*	paramMsgEntry = ObtainParameterReferenceL<CMsvEntry>(TestCase(),
										ActionParameters().Parameter(0));
										
		
		if (paramMsgEntry == NULL)
			{
			TestCase().ERR_PRINTF1(_L("Invalid parameter: Message Entry value is NULL"));
			TestCase().SetTestStepResult(EFail);
			}
			
		if((TestCase().TestStepResult()) == EPass)	
			{
			TInt result ;			
			// Call function to verify the flag 
			result = CheckFlag(*paramMsgEntry);
			if (result == KErrNone)
					{
					TestCase().INFO_PRINTF2(_L("Multiple Flag was set successfully: %d"), result);
					}
				else if (result == KErrNotFound)
					{
					TestCase().ERR_PRINTF1(_L("Multiple Flag was not set"));
					TestCase().SetTestStepResult(EFail);			
					}
				else
					{
					TestCase().ERR_PRINTF2(_L("Verification of Recipient address failed with error: %d"), result);
					TestCase().SetTestStepResult(EFail);			
					}
			}
		}
			
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsCheckMultipleRecipientFlag);
	TestCase().ActionCompletedL(*this);
	}
	
	
/**
 Calls the MultipleRecipient() to check if it has been set to True 
*/
TInt CMtfTestActionSendAsCheckMultipleRecipientFlag::CheckFlag(CMsvEntry& aMsgEntry)
	{
	TInt returnValue = KErrNotFound;
	TBool check = aMsgEntry.Entry().MultipleRecipients();
	if (check)
		{
		return KErrNone;
		}
	return returnValue ;
	}
	
		
