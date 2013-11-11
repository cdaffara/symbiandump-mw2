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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// StartWatchers
// [Action Parameters]
// RProcess watcher <output-completion>:In case of EKA2 and EKA1 on hardware, output the process that started the watchers.
// If watchers are already running, this output parameter will not be returned
// or 
// RThread watcher  <output-completion>:In case of EKA1 on emulator, output the thread that started the watchers.
// If watchers are already running, this output parameter will not be returned
// TBool Started <output-completion>  : For all the cases, output ETrue if the watchers were created
// by the process or thread in this action, otherwise output EFalse
// [Action Description]
// If the watchers are not running, starts the watchers.  Watchers are started
// in their own process for EKA2 and EKA1 on the  hardware and in their own thread 
// for EKA1 on emulator.  The action waits for the watchers to start before completing.
// At the end of the test case, the watchers are stopped if they were created
// by the action.  If they were not created by the action, then the watchers 
// are not stopped.
// [APIs Used]
// None
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionStartWatchers.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "TestFrameworkActionsUtils.h"
#include "CMtfTestActionUtilsTimer.h"

const TInt KWaitForWatchersToStart = 2;
const TInt KWaitForWatchersToStartCounter = 5;




_LIT(KWatcherExe, "z:\\system\\libs\\watcher.exe");
			

CMtfTestAction* CMtfTestActionStartWatchers::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionStartWatchers* self = new (ELeave) CMtfTestActionStartWatchers(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionStartWatchers::CMtfTestActionStartWatchers(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	iCounter = KWaitForWatchersToStartCounter;	
	}


CMtfTestActionStartWatchers::~CMtfTestActionStartWatchers()
	{
	if(iWatcherStarted)
		{
		TestCase().Logger().Write(_L("Kill watchers"));
		iWatcherProcess.Terminate(0);
		iWatcherProcess.Close();
		}
	
	Cancel();
	delete iTimer;
	}

void CMtfTestActionStartWatchers::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionStartWatchers);
	CActiveScheduler::Add(this);

	if(TestFrameworkActionsUtils::CheckIfWatchersAlreadyRunningL())
		{	
		//Watchers already running, output EFalse to indicate that watchers are not created
		StoreParameterL<TInt>(TestCase(),iWatcherStarted,ActionParameters().Parameter(1));
		TestCase().ActionCompletedL(*this);
		}
	else
		{
		//Watchers not running, need to be started.
		//Watchers started in their own process
		User::LeaveIfError(iWatcherProcess.Create(KWatcherExe, KNullDesC));
		iWatcherProcess.Resume();
		//iWatcherProcess.Close();
		//Wait for the watchers to start
		iTimer = CMtfTestActionUtilsTimer::NewL();
		iTimer->After(KWaitForWatchersToStart,iStatus);
		SetActive();
		}	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionStartWatchers);
	}


void CMtfTestActionStartWatchers::DoCancel()
	{
	iTimer->Cancel();
	}


void CMtfTestActionStartWatchers::RunL()
	{
	User::LeaveIfError(iStatus.Int());

	if(TestFrameworkActionsUtils::CheckIfWatchersAlreadyRunningL())
		{
		iWatcherStarted = ETrue;

		StoreParameterL<RProcess>(TestCase(),iWatcherProcess,ActionParameters().Parameter(0));
		StoreParameterL<TInt>(TestCase(),iWatcherStarted,ActionParameters().Parameter(1));
		TestCase().ActionCompletedL(*this);
		}
	else
		{
		// Watcheres are not running, restart the timer or decide to leave
		if(iCounter--)
			{
			iTimer->After(KWaitForWatchersToStart,iStatus);
			SetActive();
			}
		else{
			User::Leave(KErrGeneral);	
			}
		}
	}		
	
