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
// VerifySubject
// [Action Parameters]
// CMsvEntry		msgEntry		<input>	:		Reference to the message entry object
// HBufC			subject			<input>	:		String containing the Subject
// [Action Description]
// Compares the subject field of the message with the string provided as input to the Test Action.
// If the subject field of the message does not match with the specified string, the Test Action fails.
// [APIs Used]
// CImHeader::RestoreL ()
// CImHeader::Subject ()
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionVerifySubject.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionUtilsMessage.h"
#include "CMtfTestActionParameters.h"

#include <smut.h> //KUidMsgTypeSms
#include <irmsgtypeuid.h>
#include <btmsgtypeuid.h>

/**
  NewL()
  Constructs a CMtfTestActionVerifySubject object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionVerifySubject
  @pre    None
  @post   CMtfTestActionVerifySubject object is created
*/
CMtfTestAction* CMtfTestActionVerifySubject::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionVerifySubject* self = 
							new (ELeave) CMtfTestActionVerifySubject(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionVerifySubject constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionVerifySubject::CMtfTestActionVerifySubject(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionVerifySubject
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionVerifySubject::~CMtfTestActionVerifySubject()
	{
	}



/**
  ExecuteActionL
  Verifies  the subject field contents set for a message by comparing it with the expected value.
  The subject field is varified for SMTP, IR and BT messages, where as for SMS message, returns
  KErrNotSupported.

  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionVerifySubject::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionVerifySubject);
		
		// Get test action input parameters
		CMsvEntry*	paramMsgEntry = ObtainParameterReferenceL<CMsvEntry>(TestCase(),
													ActionParameters().Parameter(0));

		HBufC* paramSubject		  = ObtainParameterReferenceL<HBufC>(TestCase(),
													ActionParameters().Parameter(1));
													
		if((paramMsgEntry == NULL) || (paramSubject == NULL))
			{
			TestCase().ERR_PRINTF1(_L("Test Action input not valid"));
			TestCase().SetTestStepResult(EFail);
			}

		if((TestCase().TestStepResult()) == EPass)
			{
			// Call function to verify the subject field
			TInt err = VerifySubjectL(*paramMsgEntry, paramSubject);
					
			if (err == KErrNone)
				{
				TestCase().INFO_PRINTF1(_L("Comparison of Subject field was successful."));
				}
			else
				{
				TestCase().ERR_PRINTF2(_L("Test VerifySubject failed with error %d"), err);
				TestCase().SetTestStepResult(EFail);
				}
				
			}
		}
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionVerifySubject);
		TestCase().ActionCompletedL(*this);
	}



/**
	Compares the Subject filed of the message with the expected value
*/
TInt CMtfTestActionVerifySubject::VerifySubjectL(CMsvEntry& aMsvEntry,HBufC* aSubject)
	{
	TInt returnValue = KErrNone;

	// Get Message type id 
	TUid msgMtmType = aMsvEntry.Entry().iMtm;


	if(	(msgMtmType.operator == (KUidMsgTypeSMTP)) || 
		(msgMtmType.operator == (KUidMsgTypePOP3)) || 
		(msgMtmType.operator == (KUidMsgTypeIMAP4) ))
	  	{
		// Email message
		returnValue  = CMtfTestActionUtilsMessage::VerifyEmailSubjectL (aMsvEntry, aSubject);
		}
	else if( (msgMtmType.operator == (KUidMsgTypeBt)) || 
			 (msgMtmType.operator == (KUidMsgTypeIrUID))
		   )
		{
		// OBEX message
		returnValue  = CMtfTestActionUtilsMessage::VerifyObexSubjectL (aMsvEntry, aSubject);	
		}
	else if(msgMtmType.operator == (KUidMsgTypeSMS))
		{
		// SMS message, subject field is not supported
		returnValue = KErrNotSupported;
		}
	else
		{
		// Message type supported, can be extended later 
		returnValue = KErrNotSupported;
		}

	return returnValue;
	}
