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
// CMtfTestActionLaunchSchSendExe.h
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// LaunchSchSendExe
// [Action Parameters]
// None
// [Action Description]
// Launches SchSendExe.exe passing SMTP Service Id as parameter.
// LaunchSchSendExe Test Action is intended to verify that the SchSendExe.exe cant be launched by
// any other process, other than Task Scheduler.
// To verify this conditon, the test action launches the SchSendExe.exe and compares the process 
// completion result with the expected result.
// If the ScheduleSend is secured, then the expected result is KErrPermissionDenied, otherwise 
// KErrNone. 
// If the ScheduleSend process completion result is not equal to the expected result, 
// the Test Action is failed.
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
#include "CMtfTestActionLaunchSchSendExe.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


// Path of the SchSendExe.exe
_LIT(KMsvSchSendExe, "Z:\\system\\PROGRAMS\\schsendexe.exe");

// UID of LaunchSchSendExe.exe
const TUid KMsvSchSendExeUid = {0x100056A1}; 

/**
  Function : NewL
  Description				:Creates a new CMtfTestActionLaunchSchSendExe object
  @internalTechnology
  @param : aTestCase 		:Reference to the Test case
  @param : aActionParams 	:Test Action parameters 
  @return : CMtfTestAction* :a base class pointer to the newly created object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionLaunchSchSendExe::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionLaunchSchSendExe* self = new (ELeave) CMtfTestActionLaunchSchSendExe(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}


/**
  Function : CMtfTestActionLaunchSchSendExe
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @pre none
  @post none
*/
CMtfTestActionLaunchSchSendExe::CMtfTestActionLaunchSchSendExe(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


/**
  Function : ~CMtfTestActionLaunchSchSendExe
  Description : Destructor
  @internalTechnology
*/
CMtfTestActionLaunchSchSendExe::~CMtfTestActionLaunchSchSendExe()
	{
	}


/**
  Function : ExecuteActionL
  Description		: Launches the schsendexe.exe and verifies the result of process 
					  completion with that of the expected result.
  @internalTechnology
  @param			: none
  @return			: void
  @pre	 
  @post	: none
*/
void CMtfTestActionLaunchSchSendExe::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionLaunchSchSendExe);
	// Expected result
	TInt expectedResult  = KErrPermissionDenied;
	
	TBuf<20> cmdString;
	
	// Return KErrNotSupported in case of Wins
	TInt returnValue = KErrNotSupported;
	
	TRequestStatus status = KRequestPending;

	RProcess process;					
	User::LeaveIfError(process.Create(KMsvSchSendExe, cmdString, TUidType(KNullUid, KNullUid, KMsvSchSendExeUid)));
	TestCase().INFO_PRINTF2(_L("RProcess::Create() is successful %S "), &KTestActionLaunchSchSendExe);

	// Make the process eligible for execution
	process.Logon(status);
	process.Resume();

	// Wait for the process completion
	User::WaitForRequest(status);
				
	// Check the exit reason of the process.If Panic occurs the return value is set to KErrGeneral as the 
	// focus of test action is to check if KErrPermissionDenied is returned.
	returnValue = (process.ExitType() == EExitPanic)? KErrGeneral: status.Int();

	process.Close();

	TestCase().INFO_PRINTF4(_L("Test Action %S completed with %d, while expected %d "), &KTestActionLaunchSchSendExe,returnValue, expectedResult );
	
	if(returnValue != expectedResult )
		{
		TestCase().SetTestStepResult(EFail);
		}
		
	TestCase().ActionCompletedL(*this);
	}
