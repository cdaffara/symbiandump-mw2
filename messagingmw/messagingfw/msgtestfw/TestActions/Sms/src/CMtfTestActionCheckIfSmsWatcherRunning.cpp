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
// CheckIfSmsWatcherRunning
// [Action Parameters]
// TBool Started <output-completion>:   ETrue if the SMS watcher is running, EFalse otherwise.
// [Action Description]
// Checks if the SMS watcher is running.
// The action waits for the SMS watcher to start before completing
// [APIs Used]
// None
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionCheckIfSmsWatcherRunning.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "TestFrameworkActionsUtils.h"
#include "CMtfTestActionUtilsTimer.h"

#include <smsuaddr.h>

const TInt KWaitForSmsWatcherToStart = 2;
const TInt KWaitForSmsWatcherToStartCounter = 5;


CMtfTestAction* CMtfTestActionCheckIfSmsWatcherRunning::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCheckIfSmsWatcherRunning* self = new (ELeave) CMtfTestActionCheckIfSmsWatcherRunning(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCheckIfSmsWatcherRunning::CMtfTestActionCheckIfSmsWatcherRunning(CMtfTestCase& aTestCase)
	:CMtfTestAction(aTestCase)
	{
	iCounter = KWaitForSmsWatcherToStartCounter;
	}
	

CMtfTestActionCheckIfSmsWatcherRunning::~CMtfTestActionCheckIfSmsWatcherRunning()
	{
	Cancel();
	delete iTimer;
	}
	
	
void CMtfTestActionCheckIfSmsWatcherRunning::DoCancel()
	{
	iTimer->Cancel();
	}
	

void CMtfTestActionCheckIfSmsWatcherRunning::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCheckIfSmsWatcherRunning);
	CActiveScheduler::Add(this);
	TBool smsWatcherStarted = EFalse;
	
	if(TestFrameworkActionsUtils::CheckIfSmsWatcherAlreadyRunningL())
		{
		smsWatcherStarted = ETrue;
		StoreParameterL<TInt>(TestCase(),smsWatcherStarted,ActionParameters().Parameter(0));
		TestCase().ActionCompletedL(*this);
		}
	else
		{
		//Wait for the SMS watcher to start
		iTimer = CMtfTestActionUtilsTimer::NewL();
		iTimer->After(KWaitForSmsWatcherToStart,iStatus);
		SetActive();
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCheckIfSmsWatcherRunning);
	}
		
		
void CMtfTestActionCheckIfSmsWatcherRunning::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	
	TBool smsWatcherStarted = EFalse;
	
	if(TestFrameworkActionsUtils::CheckIfSmsWatcherAlreadyRunningL())
		{
		smsWatcherStarted = ETrue;
		StoreParameterL<TInt>(TestCase(),smsWatcherStarted,ActionParameters().Parameter(0));
		TestCase().ActionCompletedL(*this);
		}
	else
		{	
		// SMS Watcher is not running, restart the timer or decide to leave
		if(iCounter--)
			{
			iTimer->After(KWaitForSmsWatcherToStartCounter,iStatus);
			SetActive();
			}
		else
			{
			StoreParameterL<TInt>(TestCase(),smsWatcherStarted,ActionParameters().Parameter(0));
			TestCase().ActionCompletedL(*this);
			}
		}
	}		
