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
// AddRecipient
// [Action Parameters]
// CBaseMtm	paramMtm		<input>	: Reference to CBaseMtm	object
// HBufC	paramRecipient	<input>	: Recipient Address
// [Action Description]
// AddRecipient Test Action is intended to set the body text 
// contents for a message that was created using Send-As API.  
// The body text content is read from a file and then stored in a 
// CRichText object. The CRichText object is then used to set the body text 
// for the message.
// [APIs Used]
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/

//system include
#include <msvapi.h>
#include <btmsgtypeuid.h>
#include <mtclbase.h>

// User include
#include "CMtfTestActionAddRecipient.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "TestFrameworkActionsUtils.h"
#include "CMtfTestActionUtilsMessage.h"

/**
  NewL()
  Constructs a CMtfTestActionAddRecipient object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsSetBodyText
  @pre    None
  @post   CMtfTestActionAddRecipient object is created
*/
CMtfTestAction* CMtfTestActionAddRecipient::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionAddRecipient* self = 
							new (ELeave) CMtfTestActionAddRecipient(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionAddRecipient constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionAddRecipient::CMtfTestActionAddRecipient(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionAddRecipient
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionAddRecipient::~CMtfTestActionAddRecipient()
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
void CMtfTestActionAddRecipient::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionAddRecipient);
			
		TMsvId paramMsgId	= ObtainValueParameterL<TMsvId>(TestCase(),
									ActionParameters().Parameter(0));
									
		CBaseMtm* paramMtm	= ObtainParameterReferenceL<CBaseMtm>(TestCase(),
									ActionParameters().Parameter(1));
	
		HBufC* paramRecipient = ObtainParameterReferenceL<HBufC>(TestCase(),
									ActionParameters().Parameter(2));
	
		TUid msgTypeMtm = paramMtm->Type();
		paramMtm->SwitchCurrentEntryL(paramMsgId);
	
		if(msgTypeMtm.operator==(KUidMsgTypeBt))
			{
			TPtrC16 ptr16 = paramRecipient->Des();
			TBuf16<3> btAddress;
			
			CMtfTestActionUtilsMessage::FormatBtRecipientAddress(*paramRecipient, btAddress);
			paramMtm->AddAddresseeL(btAddress);
			}
		else
			{
			paramMtm->AddAddresseeL(*paramRecipient);
			}
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionAddRecipient);
		}
	TestCase().ActionCompletedL(*this);
	}

