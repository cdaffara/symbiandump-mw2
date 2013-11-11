// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

/**
 @file
*/
	
#include "CMtfTestCase.h"
#include "CMtfTestActionWait.h"
#include "CMtfTestActionSectionComplete.h"
#include "CMtfTestParameterStore.h"
#include "CMtfConfigurationType.h"
#include "CMtfTestServer.h"

_LIT(KMtfTestCasePanic,"Messaging Test Case Panic");

void CMtfTestCase::Panic(TMtfTestCasePanic aPanic)
{
	User::Panic(KMtfTestCasePanic,aPanic);
}

/** Constructor. */
CMtfTestCase::CMtfTestCase(const CMtfTestServer& aTestServer, const TBool& aScripted,
		const TBool& aSynchronous)
: 	CTestStep(), iCurrentState(EMtfTestCaseInitialState),
	iScriptedTestCase(aScripted), iSynchronous(aSynchronous), iTestServer(aTestServer) 
{
	SetTestStepResult(EPass);
}

/** Creates a new parameter store. */
void CMtfTestCase::ConstructL()
{
	iParameterStore = CMtfTestParameterStore::NewL();
}

/** Returns a reference to the parameter store. */
const CMtfTestParameterStore& CMtfTestCase::ParameterStore() const
{
	return *iParameterStore;
}
	

CMtfTestCase::~CMtfTestCase()
{
	delete iParameterStore;
	iQueuedActions.ResetAndDestroy();
	iCurrentActions.ResetAndDestroy();
	iCompletedActions.ResetAndDestroy();
	iTestCaseDefaultConfigurations.ResetAndDestroy();
	delete iScheduler;
}

/** Converts an enumerator value to its corresponding numerical value. */
TInt CMtfTestCase::ConvertEnumeratorL(const TDesC& aEnumerator) const
{
	return iTestServer.ConvertEnumeratorL(aEnumerator);
}

/** Obtains the correct configuration file given a type. */
const TDesC& CMtfTestCase::GetConfigurationFileL(const CMtfConfigurationType::TMtfConfigurationType& aType) const
{
	return GetConfigurationFileL(aType,0);
}

/** Tries to find a main script configuration first,
then the test case default and if none of them exist then gets the default configuration. */
const TDesC& CMtfTestCase::GetConfigurationFileL(const CMtfConfigurationType::TMtfConfigurationType& aType, TInt aIndex) const
{
	TInt error=KErrNone;
	const TDesC* retval=0;
	TRAP(error,retval=&iTestServer.GetMainScriptConfigurationFileL(aType,aIndex)); 

	if (error==KErrNone)
	{
		return *retval;
	}
	
	if (error != KErrNotFound)
	{
 		// nice to print some kind of error message here !!!
		User::Leave(error);
	}
	
	// find test case default if it exists
	
	TInt index=0;
	TRAP(error,index = CMtfConfigurationType::FindConfigurationL(aType,iTestCaseDefaultConfigurations));
	
	if (error==KErrNone)
	{
		// configuration found, return the required file
		// this may leave if item aIndex does not exist
		return iTestCaseDefaultConfigurations[index]->ConfigurationFileL(aIndex);
	}
	else if (error != KErrNotFound)
	{
 		// nice to print some kind of error message here !!!
		User::Leave(error);
	}
	
	// go to the server and ask for default
	// aIndex is ignored in this case
	return iTestServer.GetDefaultConfigurationFileL(aType);
	
	// ISSUE: at the end of the test case check whether all configurations have been used
}

#if defined(__WINS__) && !defined(_DEBUG)
#pragma warning( default : 4702 ) //  unreachable code
#endif

/** This function take ownership of aConfiguration immediately and therefore SHOULD NOT be 
called with aConfiguration already on the Cleanup stack. */
void CMtfTestCase::SetTestCaseDefaultConfigurationTypeL(CMtfConfigurationType* aConfiguration)
{
	CMtfConfigurationType::SetConfigurationTypeL(aConfiguration,iTestCaseDefaultConfigurations);
}

/** The function takes ownership of aParameter at the END. */
void CMtfTestCase::StoreParameterL(CMtfTestParameter* aParameter)
{
	iParameterStore->StoreParameterL(aParameter);
}

const CMtfTestParameter& CMtfTestCase::ObtainParameterL(const TDesC& aName) const
{
	return iParameterStore->ObtainParameterL(aName);
}

void CMtfTestCase::DeleteParameterL(const TDesC& aName)
{
	iParameterStore->DeleteParameterL(aName);
}

/**
 * @return - TVerdict code
 * Override of base class virtual
 */
TVerdict CMtfTestCase::doTestStepPreambleL()
{
	return TestStepResult();
}

/** Starts the execution of the test case by executing all test actions until the first wait action is 
encountered. This function then returns. Override of base class pure virtual.
@return - TVerdict code */
TVerdict CMtfTestCase::doTestStepL()
{
	iScheduler=new (ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);
		
	if (iCurrentState == EMtfTestCaseInitialState)
	{
		ChangeState();
	}
	
	if (iScriptedTestCase == EFalse)
	{	
		// non-scripted test case, run the first section function which builds up the queue
		SetupTestCaseL();
		
		// ISSUE: assert the state hasn't changed
			
		if (iQueuedActions.Count() > 0)
		{
			// the new test action will be owned by the test case
			CMtfTestActionSectionComplete::NewL(*this);	
			
			ExecuteActionsL();
		}
		else if (SetupTestCaseIsSynchronous())
		{
			// no queued actions and synchronous
			SectionCompletedL();
		}
	}	
	else
	{
		ExecuteActionsL();
	}
		
	if (iCurrentState != EMtfTestCaseTestCompleted)
	{
		// the test case contains some asynchronous test actions so we need to start
		// the scheduler
		iActiveSchedulerRunning = ETrue;
		iScheduler->Start();
	}

	return TestStepResult();
}

/**
 * @return - TVerdict code
 * Override of base class virtual
 */
TVerdict CMtfTestCase::doTestStepPostambleL()
{
	return TestStepResult();
}

/** Add a new action to the test case. Takes ownership of the test action at the END. */
void CMtfTestCase::QueueTestActionL(CMtfTestAction* aTestAction) 
{
	TBool synchronousTestCase = (iScriptedTestCase? iSynchronous: IsSynchronous());
	
	if (synchronousTestCase && !aTestAction->WaitAction() && !aTestAction->SectionCompleteAction())
	{
		// put a wait action after each test action to make it synchronous
		CMtfTestActionWait::NewL(*this);
		
		// at this point we can take ownership of aTestAction by inserting it before the wait
		User::LeaveIfError(iQueuedActions.Insert(aTestAction, iQueuedActions.Count()-1));
	}
	else
	{
		User::LeaveIfError(iQueuedActions.Append(aTestAction));
	}
}

TBool CMtfTestCase::IsSynchronous()
{
	return iSynchronous;
}
	
/** Handles an action completion by removing it from the list of current actions and putting it on the
list of completed actions. */
void CMtfTestCase::ActionCompletedL(CMtfTestAction& aAction)
{
	TInt index = iCurrentActions.Find(&aAction);
	User::LeaveIfError(index);
	User::LeaveIfError(iCompletedActions.Append(&aAction));
	iCurrentActions.Remove(index);	
	
	if (aAction.CurrentlyBeingWaitedFor())
	{
		aAction.SetCurrentlyBeingWaitedFor(EFalse);
		iWaitListCount--;
	}
	
	if (iWaitListCount == 0)
	{
		// no more actions that are currently being waited for
		ExecuteActionsL();
	}
}
	
/** Executing all queued actions until a wait action is found. */
void CMtfTestCase::ExecuteActionsL()
{
	for (;;)
	{
		if (iQueuedActions.Count()==0)
		{
			break;
		}
		
		CMtfTestAction& nextAction = *iQueuedActions[0];
		
		// remove the first action from the queue and put it on the list
		// of current actions
		
		iQueuedActions.Remove(0);	
		
		if (!nextAction.WaitAction() && !nextAction.SectionCompleteAction())
		{
			User::LeaveIfError(iCurrentActions.Append(&nextAction));
		}	
		else
		{
			// two framework actions are synchronous so they complete immediately
			User::LeaveIfError(iCompletedActions.Append(&nextAction));
		}
		
		// executing an action may: 
		//	- start off an asynchronous request in which case the action
		//		remains on the list of currently executing actions
		//  - start and complete a synchronous action in which case the action
		//		signals the test case that it has completed within ExecuteActionL(), 
		//		therefore the action must be already on iCurrentActions before
		//		ExecuteActionL() is called. ExecuteActionL() will be called recursively
		//		from ActionCompletedL().
		
		nextAction.ExecuteActionL();
		
		if (iWaitListCount>0)
		{
			// wait action was found, stop execution
			break;
		}
	}
}

/** Handles a section completion. The test case changes its state to the next state. For scripted test cases it checks whether 
the test case is finished and if it is not it simply returns. Since all test actions in a scripted test case are in the queue 
at the beginning the remaining test actions will be executed. For a non-scripted test case this function calls the appropriate 
function which queues more test actions for the next section. The function then attempts to start the execution of test 
actions in the next test case section. */
void CMtfTestCase::SectionCompletedL()
{
	ChangeState();

	if (iCurrentState == EMtfTestCaseTestCompleted)
	{
		__ASSERT_ALWAYS(iQueuedActions.Count()==0,Panic(EMtfUnexpectedQueuedAction));
		
		if (iActiveSchedulerRunning)
		{
			CActiveScheduler::Stop();
		}
		
		return;
	}
	
	if (!iScriptedTestCase)
	{		
		// Non-scripted test cases queue actions as they execute each section.
		// Therefore, actions are queued per section and when a section completes
		// there must be no more actions in the queue, unlike the scripted test cases
		// which queue all actions in the test case.
		
		__ASSERT_ALWAYS(iQueuedActions.Count()==0,Panic(EMtfUnexpectedQueuedAction));
		
		switch (iCurrentState)
		{
			case EMtfTestCaseExecutingTest:
				ExecuteTestCaseL();
		
				if (iQueuedActions.Count() > 0)
				{
					// owned by the test case
					CMtfTestActionSectionComplete::NewL(*this);	
					ExecuteActionsL();
				}
				else if (ExecuteTestCaseIsSynchronous())
				{
					SectionCompletedL();
				}	
				break;
			case EMtfTestCaseCheckingPostconditions:
				CheckPostConditionsL();
				if (iQueuedActions.Count() > 0)
				{
					// owned by the test case
					CMtfTestActionSectionComplete::NewL(*this);	
					ExecuteActionsL();
				} 
				else if (CheckPostconditionsIsSynchronous())
				{
					SectionCompletedL();
				}
				break;
			default:
				break;
		}    	
	}	
}

/** Sets the test case to wait for the specified action. */
void CMtfTestCase::WaitForActionL(const TDesC& aActionId)
{
	TInt countCurrent = iCurrentActions.Count();
	
	for (TInt count=0; count<countCurrent; count++)
	{
		if (iCurrentActions[count]->ActionIdL() == aActionId)
		{
			iCurrentActions[count]->SetCurrentlyBeingWaitedFor(ETrue);
			iWaitListCount++;
			break;
		}
	}
	
	// ISSUE: if not found maybe log this fact
}

/** Sets the test case to wait for all currently executing actions. */
void CMtfTestCase::WaitForAllActionsL()
{
	TInt numberOfCurrentActions = iCurrentActions.Count();
	
	for (TInt count=0; count < numberOfCurrentActions; count++)
	{
		iCurrentActions[count]->SetCurrentlyBeingWaitedFor(ETrue);
		iWaitListCount++;
	} 
}

void CMtfTestCase::ChangeState()
{
	switch (iCurrentState)
	{
		case EMtfTestCaseInitialState:
			iCurrentState = EMtfTestCaseExecutingSetup;
			break; 
		case EMtfTestCaseExecutingSetup:
			iCurrentState = EMtfTestCaseExecutingTest;
			break; 
		case EMtfTestCaseExecutingTest:
			iCurrentState = EMtfTestCaseCheckingPostconditions;
			break; 
		case EMtfTestCaseCheckingPostconditions:
			iCurrentState = EMtfTestCaseTestCompleted;
			break; 
		case EMtfTestCaseTestCompleted:
			User::Invariant();
			break;
		default:
			User::Invariant();
	}
}	

TBool CMtfTestCase::SetupTestCaseIsSynchronous() const
{
	return ETrue;
}

TBool CMtfTestCase::ExecuteTestCaseIsSynchronous() const
{
	return ETrue;
}

TBool CMtfTestCase::CheckPostconditionsIsSynchronous() const
{
	return ETrue;
} 
