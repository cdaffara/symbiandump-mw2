// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// common code shared between the test classes:
// CWapPushSIMessageTest
// CWapPushSLMessageTest
// CWapPushMMMessageTest
// 
//

#include "pushtests.h"
#include "dummywapstack.h"
#include "WPushUtils.h"

_LIT(KTextFinished,"All finished");



/** all derived classes use this RunL to move between the states listed.
	starts up setting up required test enbvironment, then creates a test
	message which is simualted as a push. This 'push' stage can be repeated
	for multiple messages and once all messages passed finishes up. */
void CWapPushBaseMessageTest::RunL()
	{
	switch (iState)
		{
	case EInitialisation:
		{
		// Create WAP Stack passing MDummyWapStackObserver derived instance
		// Create Connection Manager, once WAP stack setup
		iWapStack = CDummyWapStack::NewL(*this);
		iConnMan =  CConnectionManager::NewL(*this);
		NumberTestCasesToRun();
		iHasMsgBeenCompleted=EFalse;
		iHasTestBeenCompleted=EFalse;
		MoveToNextState();
		return;
		}
	case EWaitForPush:
		{
		// wait for callback to move to the next state
		if (iHasTestBeenCompleted)
			MoveToNextState();
		return;
		}
	case EFinished:
		{
		Printf(KTextFinished);
		SetPriority(EPriorityIdle);
		MoveToNextState();
		return;
		}
	default:
		// Finished
		ConfirmMessagesSavedL();
		iEngine->TestCompleted(iStatus.Int());
		return;
		}
	};

void CWapPushBaseMessageTest::RunError()
	{
	_LIT(KRunError, "CWapPushTestEngine::RunError");
	iEngine->Printf(TestName(), KRunError);
	iState = EFinished;
	}


/** destructor */
CWapPushBaseMessageTest::~CWapPushBaseMessageTest()
	{
	delete iConnMan;
	delete iWapStack;
	}	

/** cancel any active request and clean up before returning to main test
	harness
*/
void CWapPushBaseMessageTest::DoCancel()
	{
	delete iConnMan;
	delete iWapStack;
	iEngine->TestCompleted(KErrCancel);
	}

/** increment the value of iState and set active to create call to RunL
	with new state value 
*/
void CWapPushBaseMessageTest::MoveToNextState()
	{ 
	iState = (TState)(iState+1); 
	CompleteOwnRequest();
	}

/** set this object active */
void CWapPushBaseMessageTest::CompleteOwnRequest()
	{
	TRequestStatus* stat = &iStatus;
	User::RequestComplete(stat,KErrNone);	
	if (!IsActive())
		SetActive();
	}

/** sets base test name.
	@param void
	@return string - the test name 
*/
const TDesC& CWapPushBaseMessageTest::TestName()
	{
	_LIT(KTextBaseMessageTest,"Base Message Test");
	return KTextBaseMessageTest;
	}


/** 
	called by DummyWapStack and returns the push message which is
	created by the call to PrepareTestMessageL. The message is
	then passed through to aDummyCLConn which accepts as much of
	the message as possible and if this is not equal to the whole
	message then issues a rerquest for the rest of the message which
	retruns here.
	@param CDummyWSPCLConn
	@return void 

if all tests have completed - move to the next test state and complete own req
if all of msg received - move to next message

	Each derived class should have its own implementation of PrepareTestMessage
	which will be called from this method. This will allow a variable number of
	test cases to be used to test the push aspect of the dummy wap stack.
*/
void CWapPushBaseMessageTest::DWSOUnitWaitPushL(CDummyWSPCLConn& aDummyCLConn)	
	{
	if (iHasTestBeenCompleted)
		{	
		CompleteOwnRequest();
		return;
		}
	
	PrepareTestMessageL(iCurrentTestCase);

	aDummyCLConn.CompleteUnitWaitPushL(iBodyBuf, iHeadersBuf); // asynch
	
	iHasMsgBeenCompleted=aDummyCLConn.HasMsgBeenCompleted();
	
	if (iHasMsgBeenCompleted)
		{
		iHasMsgBeenCompleted=EFalse;
		iCurrentTestCase++;
		}

	iHasTestBeenCompleted = (iCurrentTestCase >= iNumberOfTestCases);
	}

/**
	display error message
	@param	string 
	@param	line number
*/
void CWapPushBaseMessageTest::DWSOError(const TDesC& aDes, TInt /*aLineNum*/)
	{
	// TO DO: format message + ?
	
	// simply display error message for now
	iEngine->Printf(TestName(),aDes);
	}

/**
	Wap Stack Server closing...
*/
void CWapPushBaseMessageTest::DWSOServerShutDown()
	{
	_LIT(KWPSLServerShutdown,"Wap stack server closing down");
	iEngine->Printf(TestName(), KWPSLServerShutdown);
	}

/**
 *	Cancel pushwait operation
 */
void CWapPushBaseMessageTest::DWSOCancelUnitWaitPush(CDummyWSPCLConn& /*aDummyCLConn*/)
	{
	Cancel(); // CActive closes open requests
	// ignore aDummyCLConn (not sure why being passed in)
	}	

/** placeholder method - does nothing */
void CWapPushBaseMessageTest::DWSOConnect(CDummyWSPCOConn&)
	{
	}

/** placeholder method - does nothing */
void CWapPushBaseMessageTest::DWSOGetEvent(CDummyWSPCOConn&)
	{
	}

/** placeholder method - does nothing */
void CWapPushBaseMessageTest::DWSOCancelGetEvent(CDummyWSPCOConn&)
	{
	}

/** placeholder method - does nothing */
void CWapPushBaseMessageTest::DWSOAckTransaction(CDummyWSPCOTrans&)
	{
	}

/** indicates number of test cases to run which should be implimented
	in a suitable switch statement in derieved class method:
	PrepareTestMessages()
*/
void CWapPushBaseMessageTest::NumberTestCasesToRun()
	{
	// default - override for derived classes 
	iNumberOfTestCases = 1;
	}




