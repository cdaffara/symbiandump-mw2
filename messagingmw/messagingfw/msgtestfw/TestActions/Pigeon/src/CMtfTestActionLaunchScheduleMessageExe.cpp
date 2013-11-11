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
// LaunchScheduleMessageExe
// [Action Parameters]
// None
// [Action Description]
// Launches ScheduleMessage.exe.
// [APIs Used]
// RProcess::Create
// RProcess::Resume
// RProcess::Close
// __ACTION_INFO_END__
// 
//

// System includes
#include <msvstd.h>
#include <e32std.h>
#include <imcmutil.h>
#include <e32cmn.h>

// User includes
#include "CMtfTestActionLaunchScheduleMessageExe.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


// Path of the SchSendExe.exe
_LIT(KMsvScheduleMessage, "schedulemessage.exe");

// UID of LaunchSchSendExe.exe
const TUid KMsvScheduleMessageUid = {0x10204284}; 

/**
  Function : NewL
  Description				:Creates a new CMtfTestActionLaunchScheduleMessageExe object
  @internalTechnology
  @param : aTestCase 		:Reference to the Test case
  @param : aActionParams 	:Test Action parameters 
  @return : CMtfTestAction* :a base class pointer to the newly created object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionLaunchScheduleMessageExe::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionLaunchScheduleMessageExe* self = new (ELeave) CMtfTestActionLaunchScheduleMessageExe(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}


/**
  Function : CMtfTestActionLaunchScheduleMessageExe
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @pre none
  @post none
*/
CMtfTestActionLaunchScheduleMessageExe::CMtfTestActionLaunchScheduleMessageExe(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


/**
  Function : ~CMtfTestActionLaunchScheduleMessageExe
  Description : Destructor
  @internalTechnology
*/
CMtfTestActionLaunchScheduleMessageExe::~CMtfTestActionLaunchScheduleMessageExe()
	{
	}


/**
  Function : ExecuteActionL
  Description		: Launches the ScheduleMessage.exe and verifies the result of process 
					  completion. If the process fails, then the test action is failed
  @internalTechnology
  @param			: none
  @return			: void
  @pre		: 
  @post	: none
*/
void CMtfTestActionLaunchScheduleMessageExe::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionLaunchScheduleMessageExe);
	if((TestCase().TestStepResult()) == EPass)
		{
		// Create process
		RProcess process;					
		TBuf<20> cmdString;
		User::LeaveIfError(process.Create(KMsvScheduleMessage,cmdString, TUidType(KNullUid,
													    KNullUid, KMsvScheduleMessageUid)));
		TestCase().INFO_PRINTF2(_L("RProcess::Create() is successful %S "), &KTestActionLaunchScheduleMessageExe);

		// Make the process eligible for execution
		TRequestStatus status = KRequestPending;
		process.Logon(status);
		process.Resume();

		// Wait for the process completion
		User::WaitForRequest(status);
				
		// Check the exit reason of the process.If Panic occurs, the return value is set
		// to KErrGeneral 
		TInt returnValue = (process.ExitType() == EExitPanic)? KErrGeneral: status.Int();
		process.Close();

		TestCase().INFO_PRINTF3(_L("Test Action %S completed with %d"), &KTestActionLaunchScheduleMessageExe, returnValue);
		
		if(returnValue  != KErrNone)
			{
			// Error or Panic has occured, fail the Test Action
			TestCase().SetTestStepResult(EFail);
			}
		TestCase().ActionCompletedL(*this);
		}	
	}
