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
// CheckIfMessageScheduleDateExpired
// [Action Parameters]
// CMsvSession  Session        	<input>: Reference to the session.
// TMsvId 		MessageId     	<input>: Value of the Message id.
// [Action Description]
// Checks if the schedule time of the message has expired.  If not, the Test Action
// fails.
// [APIs Used]	
// TMsvEntry::iDate
// TMsvEntry::Id
// CMsvEntry::EntryL
// CMsvSession::GetEntryL
// __ACTION_INFO_END__
// 
//

// System Includes
#include <msvapi.h>

// User Includes
#include "CMtfTestActionCheckIfMessageScheduleDateExpired.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  Function : NewL
  Description 	: Constructs a new CMtfTestActionCheckIfMessageScheduleDateExpired object
  @internalTechnology
  @param 		: aTestCase 		:Reference to the Test case
  @param 		: aActionParams 	:Test Action parameters 
  @return 		: CMtfTestAction* 	:a base class pointer to the newly created object
  @pre none
  @post: none
*/
CMtfTestAction* CMtfTestActionCheckIfMessageScheduleDateExpired::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCheckIfMessageScheduleDateExpired* self = new (ELeave) CMtfTestActionCheckIfMessageScheduleDateExpired(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  Function 		: CMtfTestActionCheckIfMessageScheduleDateExpired
  Description 	: Constructor
  @internalTechnology
  @param 		: aTestCase - CMtfTestCase for the CMtfTestAction base class
  @pre none
  @post: none
*/
CMtfTestActionCheckIfMessageScheduleDateExpired::CMtfTestActionCheckIfMessageScheduleDateExpired(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


/**
  Function : ExecuteActionL
  Description : Checks if the schedule time of the message has expired.  If not,
  fails the Test Action.
  @internalTechnology
  @param  : none
  @return : void
  @pre  : none
  @post none
*/
void CMtfTestActionCheckIfMessageScheduleDateExpired::ExecuteActionL()
	{
	if((TestCase().TestStepResult())== EPass)
		{
		TestCase().Logger().Write(_L("CMtfTestActionCheckIfMessageScheduleDateExpired::ExecuteActionL IN"));
				
		// Obtain Input parameters
		CMsvSession* paramSession	= ObtainParameterReferenceL<CMsvSession>(TestCase(),
															ActionParameters().Parameter(0));
		
		TMsvId paramMsgId 		= ObtainValueParameterL<TMsvId>(TestCase(),
															ActionParameters().Parameter(1));
			
		// Create a message entry
		CMsvEntry* entry = paramSession->GetEntryL(paramMsgId);
		CleanupStack::PushL(entry);
		TMsvEntry msgEntry = entry->Entry();

		// Get the message scheduled time
		TTime date ;
		date = msgEntry.iDate;
		
		// Get the current time
		TTime currentTime;
		currentTime.HomeTime();	

		// Compre the message schedule time with the current time
		if(date >= currentTime)
			{
			// Message schedule time not expired
			TestCase().ERR_PRINTF1(_L("Message schedule time has not expired."));
			TestCase().SetTestStepResult(EFail);	
			}
		else
			{
			TestCase().INFO_PRINTF1(_L("Message schedule time has expired."));
			}
	
		CleanupStack::PopAndDestroy(entry);	

		TestCase().Logger().Write(_L("CMtfTestActionCheckIfMessageScheduleDateExpired::ExecuteActionL OUT"));
		}

	TestCase().ActionCompletedL(*this);
	}
