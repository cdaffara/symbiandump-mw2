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
// CheckMessageSendingState
// [Action Parameters]
// CMsvSession  Session							<input>: Reference to the session.
// TMsvId		Message							<input>: Message entry ID to check the sending state
// TInt		    ExpectedSendingState  		 	<input>: Expected sending state of the message
// TInt		    ExpectedPendingConditionMet   	<input>: Expected pending condition met state of the message
// TInt		    ExpectedScheduled   			<input>: Expected scheduled state of the message
// TInt		    ExpectedFailed   				<input>: Expected failed state of the message
// TInt			TimeOut							<input>: Time out value in seconds
// [Action Description]
// Wait for an expected entry state (Sent, Rescheduled, Failed, etc.).  
// If a state does not happen within the time out period, this action will fail.
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvEntry::Entry
// TMsvEntry::SendingState
// TMsvEntry::PendingConditionMet
// TMsvEntry::Failed
// TMsvEntry::Scheduled
// __ACTION_INFO_END__
// 
//


#include <msvapi.h>
#include <msvstd.h>

#include "CMtfTestActionCheckMessageSendingState.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

const TInt KOneSecond = 1000000;
CMtfTestAction* CMtfTestActionCheckMessageSendingState::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
 	CMtfTestActionCheckMessageSendingState* self = new (ELeave) CMtfTestActionCheckMessageSendingState(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

CMtfTestActionCheckMessageSendingState::CMtfTestActionCheckMessageSendingState(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}

CMtfTestActionCheckMessageSendingState::~CMtfTestActionCheckMessageSendingState()
	{
	Cancel();
	iExpireTimer.Close();
	//Note: CMsvSession has already been destroyed at this point by the framework,
	//so, we could not delete the iMsvEntry here.  
	}

void CMtfTestActionCheckMessageSendingState::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCheckMessageSendingState);
	if(ActionParameters().Count() != 7)
		{
		TestCase().ERR_PRINTF1( _L("CMtfTestActionCheckMessageSendingState param count error !") );
		User::Leave(KErrArgument);
		}
		
    CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId message= ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	iMsvEntry = CMsvEntry::NewL(*paramSession,message,TMsvSelectionOrdering());
		
	iExpectedSendingState = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));
	iExpectedPendingConditionMet = ObtainValueParameterL<TBool>(TestCase(),ActionParameters().Parameter(3));
	iExpectedScheduled = ObtainValueParameterL<TBool>(TestCase(),ActionParameters().Parameter(4));
	iExpectedFailed = ObtainValueParameterL<TBool>(TestCase(),ActionParameters().Parameter(5));
											
	TestCase().INFO_PRINTF5(_L("CMtfTestActionCheckMessageSendingState::expected states: %d, %d, %d, %d"), 
									iExpectedSendingState, 
									iExpectedScheduled, 
									iExpectedPendingConditionMet, 
									iExpectedFailed);
									
	TTimeIntervalMicroSeconds32 timeout = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(6)) * KOneSecond;
	
	if(IsMessageStateMet())
		{
		TestCase().ActionCompletedL(*this);
		}
	else
		{
		iMsvEntry->AddObserverL(*this);
		User::LeaveIfError(iExpireTimer.CreateLocal());
		iExpireTimer.After(iStatus, timeout);
		CActiveScheduler::Add(this);
		SetActive();	
		}	
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCheckMessageSendingState);
	}

void CMtfTestActionCheckMessageSendingState::DoCancel()
	{
	iExpireTimer.Cancel();
	delete iMsvEntry;
	iMsvEntry = NULL;
	}

 void CMtfTestActionCheckMessageSendingState::RunL()
	{
	TestCase().ERR_PRINTF1( _L("CMtfTestActionCheckMessageSendingState timed out !") );
	User::Leave(KErrTimedOut);
	}

/**
Called by CMsvEntry when a messaging event has occurred. It is used here 
to check the message sending state
*/
void CMtfTestActionCheckMessageSendingState::HandleEntryEventL(TMsvEntryEvent aEvent, TAny* /* aArg1*/, TAny* /* aArg2*/, TAny* /*aArg3*/)
	{
	if(aEvent == EMsvEntryChanged)
		{
		if(IsMessageStateMet())
			{
			Cancel();
			TestCase().ActionCompletedL(*this);
			}
		}
	}


TBool CMtfTestActionCheckMessageSendingState::IsMessageStateMet()
	{
	TMsvEntry entry = iMsvEntry->Entry();
	TInt sendingState = entry.SendingState();
	TInt scheduled = entry.Scheduled() ? 1:0;
	TInt failed = entry.Failed() ? 1: 0;
		
	TInt pendingConditionMet = entry.PendingConditions() ? 1:0;
	TestCase().Logger().WriteFormat(_L("CMtfTestActionCheckMessageSendingState::IsMessageStateMet state: %d, %d, %d, %d"), sendingState, scheduled, pendingConditionMet, failed);
	if(pendingConditionMet != iExpectedPendingConditionMet)
		{
		return EFalse;
		}
	if(sendingState == iExpectedSendingState &&
		scheduled == iExpectedScheduled && 
		failed == iExpectedFailed)
		{
		TestCase().Logger().Write(_L("CMtfTestActionCheckMessageSendingState::IsMessageStateMet Message state met"));
		return ETrue;
		}
	return EFalse;			
	}
