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
// SendAsGetMessageTypeUid
// [Action Parameters]
// CSendAsMessageTypes	paramSendAs			<input>	  :	Reference to 
// CSendAsMessageTypes object
// TInt					paramIndex			<input>	  :	Array index value
// TUid					paramMessageTypeUid	<output>  :	Message type UId
// [Action Description]
// Gets the Uid of the message type present at the specified index in the list of
// available message types present in the CSendAsMessageTypes object.
// [APIs Used]
// CSendAsMessageTypes::MessageTypeUid (TInt aIndex)
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
#include "CMtfTestActionSendAsGetMessageTypeUid.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


_LIT(KSpace," ");

/**
  NewL()
  Constructs a CMtfTestActionSendAsGetMessageTypeUid object.
  Uses two phase construction
  @internalTechnology
  @param  aTestCase				Test Case to which this Test Action belongs
  @param  aActionParameters		Test Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsGetMessageTypeUid
  @pre    None
  @post   CMtfTestActionSendAsGetMessageTypeUid object is created
*/
CMtfTestAction* CMtfTestActionSendAsGetMessageTypeUid::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsGetMessageTypeUid* self = 
					new (ELeave) CMtfTestActionSendAsGetMessageTypeUid(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsGetMessageTypeUid constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsGetMessageTypeUid::CMtfTestActionSendAsGetMessageTypeUid(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsGetMessageTypeUid
  Description : Destructor
  @internalTechnology
  @pre  None
  @post 
*/
CMtfTestActionSendAsGetMessageTypeUid::~CMtfTestActionSendAsGetMessageTypeUid()
	{
	}
	
/**
  ExecuteActionL
	Gets the list of filtered message types and the index value as inputs
	and then provides the UID of the message type present at the specified index
	as output of the Test Action
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsGetMessageTypeUid::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsGetMessageTypeUid);
		
		// Get the input parameters for the test action
		CSendAsMessageTypes* paramSendAsMessageTypes = ObtainParameterReferenceL<CSendAsMessageTypes>
														(TestCase(), ActionParameters().Parameter(0));
		TInt paramCount= ObtainValueParameterL<TInt>(TestCase(),
													ActionParameters().Parameter(1));
		HBufC* msgTypeString=NULL;
		
//Creating Name of the MessageTypes
		for(TInt Ctr=0; Ctr < paramCount; Ctr++)
			{
			HBufC* paramName	= ObtainParameterReferenceL<HBufC>(TestCase(),
											ActionParameters().Parameter(2+Ctr));
			TInt length = paramName->Length();
			
			if(msgTypeString)
				{
				length += msgTypeString->Length();
				HBufC* tempBuffer = HBufC::NewL(length+1);
				TPtr ptrToBuffer = tempBuffer->Des();
				ptrToBuffer.Append(msgTypeString->Des());
				ptrToBuffer.Append(KSpace);
				ptrToBuffer.Append(paramName->Des());
				CleanupStack::PopAndDestroy(msgTypeString);
				msgTypeString = ptrToBuffer.AllocLC();
				delete tempBuffer;
				}
			else
				{
				msgTypeString = HBufC::NewLC(length);
				msgTypeString->operator=(paramName->Des());
				}
			}

			TUid paramMessageTypeUid;
			
			paramMessageTypeUid = paramSendAsMessageTypes->UidFromNameL(msgTypeString->Des());
			CleanupStack::PopAndDestroy(msgTypeString);
			
			StoreParameterL<TUid>(TestCase(),paramMessageTypeUid,ActionParameters().Parameter(ActionParameters().Count()-1));

			TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsGetMessageTypeUid);
		}
		TestCase().ActionCompletedL(*this);
	}

