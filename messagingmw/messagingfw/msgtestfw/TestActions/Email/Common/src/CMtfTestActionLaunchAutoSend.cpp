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
// LaunchAutoSend
// [Action Parameters]
// TMsvId smtpServiceId		   <input>: Value of the Smtp Service Id.
// [Action Description]
// Launches LaunchAutoSend.exe passing SMTP Service Id as parameter.
// The LaunchAutoSend.exe is developed for performing the Capability checking implemented
// in the AutoSend.exe.  LaunchAutoSend.exe in turn launches the AutoSend.exe and 
// returns the result of the AutoSend.exe execution to this Test Action.
// The LaunchAutoSend.exe is expected to be present under C:\System\Programs\ directory
// The Test Action checks the AutoSend exe's completion result and verifies it with 
// the expected value that is provided as an input to the Test Action
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
#include "CMtfTestActionLaunchAutoSend.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

class TDummyMsvSessionObserver: public MMsvSessionObserver
	{
public: 
	void HandleSessionEventL(TMsvSessionEvent,TAny*,TAny*,TAny*) {};
	};

// Path of the LaunchAutoSend.exe
_LIT(KMsvLaunchAutoSend, "LaunchAutoSend.exe");

// UID of LaunchAutoSend.exe
const TUid KMsvLaunchAutoSendExeUid = {0x10204283}; 

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase 		:Reference to the Test case
  @param : aActionParams 	:Test Action parameters 
  @return : CMtfTestAction* :a base class pointer to the newly created object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionLaunchAutoSend::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionLaunchAutoSend* self = new (ELeave) CMtfTestActionLaunchAutoSend(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}


/**
  Function : CMtfTestActionLaunchAutoSend
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @pre none
  @post none
*/
CMtfTestActionLaunchAutoSend::CMtfTestActionLaunchAutoSend(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


/**
  Function : ~CMtfTestActionLaunchAutoSend
  Description : Destructor
  @internalTechnology
*/
CMtfTestActionLaunchAutoSend::~CMtfTestActionLaunchAutoSend()
	{
	}


/**
  Function : ExecuteActionL
  Description : Starts the LaunchAutoSend.exe, and waits for the completion of the 
  process.  The process's exit reason is then compared with the expected value, fails
  the test if the process's exit reason does not match with the expected value.
  @internalTechnology
  @param  : none
  @return : void
  @pre  : LauchAutoSend.exe is available in c:\system\programs\ directory
  @post none
*/
void CMtfTestActionLaunchAutoSend::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionLaunchAutoSend);
	// Get Test Action input parameters
	
	// SMTP Service Id
	TMsvId paramSmtpServiceId = ObtainValueParameterL<TMsvId>(TestCase(), ActionParameters().Parameter(0));
	// Expected result
	TInt paramExpectedResult = ObtainValueParameterL<TInt>(TestCase(), ActionParameters().Parameter(1));
	//LaunchAutoSend Exe name if new one is created with different capabilites
	HBufC* paramLaunchAutoSendExeName = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(2), NULL);
	
	// Check to see if enforcement is on - only if not expecting KErrNone...
	if( paramExpectedResult != KErrNone )
		{
		if( PlatSec::ConfigSetting(PlatSec::EPlatSecEnforcement) )
			{
			// Enforment on - check that the SMTP required capabilities are actually
			// being enforced.
			TDummyMsvSessionObserver dummyObserver;
			
			CMsvSession* sess = CMsvSession::OpenSyncL(dummyObserver);
			CleanupStack::PushL(sess);
			
			// inf.iCaps should be the TCapabilitySet of the creator process.
			TCapabilitySet caps;
			sess->GetMtmRequiredCapabilitiesL(KUidMsgTypeSMTP, caps);
			CleanupStack::PopAndDestroy(sess);
			
			// Now this is a bit of a HACK, but can't think of a better way...
			// The current possibilities for SMPT capabilities are Network Services
			// and Local Services. Check each is in the SMTP capabilities and if so
			// see if it is enforced.
			if( caps.HasCapability(ECapabilityNetworkServices) &&
				!PlatSec::IsCapabilityEnforced(ECapabilityNetworkServices) )
				{
				// SMTP requires Network Services but this is not enforced - change
				// expected value to be KErrNone.
				TestCase().INFO_PRINTF4(_L("Test Action %S : Network Services not enforced - expected return value changed to %d from %d"), &KTestActionLaunchAutoSend, KErrNone, paramExpectedResult);
				paramExpectedResult = KErrNone;	
				}
			else if( caps.HasCapability(ECapabilityLocalServices) && 
					!PlatSec::IsCapabilityEnforced(ECapabilityLocalServices) )
				{
				// SMTP requires Local Services but this is not enforced - change
				// expected value to be KErrNone.
				TestCase().INFO_PRINTF4(_L("Test Action %S : Local Services not enforced - expected return value changed to %d from %d"), &KTestActionLaunchAutoSend, KErrNone, paramExpectedResult);
				paramExpectedResult = KErrNone;	
				}
			}
		else
			{
			// Enforcement off - change expected value to be KErrNone.
			TestCase().INFO_PRINTF4(_L("Test Action %S : PlatSec Enforcement OFF - expected return value changed to %d from %d"), &KTestActionLaunchAutoSend, KErrNone, paramExpectedResult);
			paramExpectedResult = KErrNone;
			}		
		}

	TBuf<20> cmdString;
	cmdString.Num(paramSmtpServiceId, EDecimal);
	
	// Return KErrNotSupported in case of Wins
	TInt returnValue = KErrNotSupported;
	
	TRequestStatus status = KRequestPending;

	RProcess process;
	if (paramLaunchAutoSendExeName)
	{
	User::LeaveIfError(process.Create(*paramLaunchAutoSendExeName, cmdString, TUidType(KNullUid, KNullUid, KMsvLaunchAutoSendExeUid)));	
	}
	else
	{
	User::LeaveIfError(process.Create(KMsvLaunchAutoSend, cmdString, TUidType(KNullUid, KNullUid, KMsvLaunchAutoSendExeUid)));	
	}
	TestCase().INFO_PRINTF2(_L("RProcess::Create() is successful %S "), &KTestActionLaunchAutoSend);

	// Make the process eligible for execution
	process.Logon(status);
	process.Resume();

	// Wait for the process completion
	User::WaitForRequest(status);
				
	// Check the exit reason of the process
	returnValue = (process.ExitType() == EExitPanic)? KErrGeneral: status.Int();

	process.Close();

	TestCase().INFO_PRINTF4(_L("Test Action %S completed with %d, while expected %d "), &KTestActionLaunchAutoSend,returnValue, paramExpectedResult);
	if(returnValue != paramExpectedResult)
		{
		TestCase().SetTestStepResult(EFail);
		}
		
	TestCase().ActionCompletedL(*this);
	}
