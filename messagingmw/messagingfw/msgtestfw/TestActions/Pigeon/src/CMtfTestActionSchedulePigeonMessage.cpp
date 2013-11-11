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
// SchedulePigeonMessage
// [Action Parameters]
// CMsvSession	Session     <input>: Reference to the session
// TMsvId		ParentId	<input>: Parent message Id
// TInt			Command		<input>: Command to be passed to the Pigeon Server MTM	
// [Action Description]
// Issues command to Pigeon Server MTM to schedule/reschedule the messages.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

// User includes
#include "CMtfTestActionSchedulePigeonMessage.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

//User Include
#include "pigeonservermtm.h"

/**
  Function : NewL
  Description	: Constructs a new CMtfTestActionSchedulePigeonMessage object
  @internalTechnology
  @param		: aTestCase 		:Reference to the Test case
  @param		: aActionParams 	:Test Action parameters 
  @return		: CMtfTestAction* :a base class pointer to the newly created object
  @pre none
  @post: none
*/
CMtfTestAction* CMtfTestActionSchedulePigeonMessage::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSchedulePigeonMessage* self = new (ELeave) CMtfTestActionSchedulePigeonMessage(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Function : CMtfTestActionSchedulePigeonMessage
  Description	: Constructor
  @internalTechnology
  @param		: aTestCase - CMtfTestCase for the CMtfTestAction base class
  @pre none
  @post: none
*/
CMtfTestActionSchedulePigeonMessage::CMtfTestActionSchedulePigeonMessage(CMtfTestCase& aTestCase)
	:CMtfTestAction(aTestCase)
	{
	iOperation = NULL;
	}

/**
  Function : ~CMtfTestActionSchedulePigeonMessage
  Description : Destructor
  @internalTechnology
*/
CMtfTestActionSchedulePigeonMessage::~CMtfTestActionSchedulePigeonMessage()
	{
	delete iSelection;
	}

/**
  Function : DoCancelL
  Description : Operation to be performed on cancellation of the asynchronous request
  @internalTechnology
  @param  : none
  @return : void
  @pre 
  @post none
*/
void CMtfTestActionSchedulePigeonMessage::DoCancel()
	{
	iOperation->Cancel();
	}

/**
  Function : ExecuteActionL
  Description : Issues command to Pigeon Server MTM to schedule/reschedule the messages
  @internalTechnology
  @param  : none
  @return : void
  @pre  
  @post none
*/
void CMtfTestActionSchedulePigeonMessage::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSchedulePigeonMessage);
	// Obtain input parameters
	CMsvSession* paramSession	= ObtainParameterReferenceL<CMsvSession>(TestCase(),
														ActionParameters().Parameter(0));
	TMsvId paramParentId		= ObtainValueParameterL<TMsvId>(TestCase(),
														ActionParameters().Parameter(1));
	TInt paramCommand			= ObtainValueParameterL<TInt>(TestCase(),
														ActionParameters().Parameter(2));
	
	//Error Handling Just in case one of the above inputs fail.
	if(TestCase().TestStepResult() == EPass)
		{
		CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramParentId,TMsvSelectionOrdering());

		// Get the selection of Pigeon Messages
		iSelection = entry->ChildrenWithMtmL(KUidMsgTypePigeon);
		
		// Initiate asynchronous operation, passing the schedule/reschedule the message in the selection
		// Dummy aParameter value passed as Pigeon MTM does not make use of it.
		iOperation = paramSession->TransferCommandL(*iSelection,paramCommand, _L8("aaaa"), iStatus);
		
		CActiveScheduler::Add(this);
		SetActive();
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSchedulePigeonMessage);
	}

/**
  Function : RunL
  Description	: Handles the asynchronous request completion event. Informs the completion 
				  of the Test Action to the Test Case
  @internalTechnology
  @param		: none
  @return		: void
  @pre none
  @post: none
*/
void CMtfTestActionSchedulePigeonMessage::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);
	
	delete iOperation;
	iOperation = NULL;
	
	User::LeaveIfError(err);

	TestCase().ActionCompletedL(*this);
	}
