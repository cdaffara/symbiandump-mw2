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
// CheckIfMessagePresent
// [Action Parameters]
// CMsvEntrySelection MessageSelection     <input>: Reference to the 
// CMsvEntrySelection object.
// TMsvId MessageId						   <input>: Value of the the message id.
// [Action Description]
// Checks a specified folder for a specified message id.
// [APIs Used]
// CMsvEntrySelection::Find
// __ACTION_INFO_END__
// 
//

#include <msvstd.h>

#include "CMtfTestActionCheckIfMessagePresent.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionCompareImapAccountId object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionCheckIfMessagePresent::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCheckIfMessagePresent* self = new (ELeave) CMtfTestActionCheckIfMessagePresent(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}


/**
  Function : CMtfTestActionCompareImapAccountId
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionCheckIfMessagePresent::CMtfTestActionCheckIfMessagePresent(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


/**
  Function : ~CMtfTestActionCompareImapAccountId
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionCheckIfMessagePresent::~CMtfTestActionCheckIfMessagePresent()
	{
	}


/**
  Function : ExecuteActionL
  Description : Entry point for the this test action in the test framework
  @internalTechnology
  @param : none
  @return : void
  @pre none
  @post none
*/
void CMtfTestActionCheckIfMessagePresent::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCheckIfMessagePresent);
	CMsvEntrySelection* paramMsgSelection = ObtainParameterReferenceL<CMsvEntrySelection>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramMessageId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));

	if(!paramMsgSelection)
		{
		User::Leave(KErrArgument);
		}

	TInt messageIndex = paramMsgSelection->Find(paramMessageId);
	User::LeaveIfError(messageIndex);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCheckIfMessagePresent);
	TestCase().ActionCompletedL(*this);
	}

