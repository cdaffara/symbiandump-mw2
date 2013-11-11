// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Test harness written by Steve Pearce of EMCC (Manchester) 13/12/02
// A simple test harness to verify that CMsvOperationActiveSchedulerWait improves the handling of
// CActiveScheduler::Start() in comparison to CMsvOperationWait.
// It works by starting two active objects under the control of an overseer class. Each of these AOs
// complete themselves on starting, and re-enter the Active Scheduler in their RunL()s via one or 
// other of the wait classes (CMsvOperationWait (and calling CActiveScheduler::Start() explicitly)
// or the new class CMsvOperationActiveSchedulerWait). The wait class's iStatus member is passed
// into a call to RTimer::After(), so that it will complete after a predetermined period of time.
// Once the timer completes, the wait class exits the active scheduler in its RunL, causing the
// original AOs RunL to continue. This then notifies the overseer class of its completion; when both
// AOs have completed, the overseer completes the tests.
// The test should succeed with the COperationActiveSchedulerWaitTest classes used as AOs by the 
// overseer since they use the new COperationActiveSchedulerWait class for the non-blocking wait.
// If instances of COperationWaitTest are used instead, then the active scheduler will panic with
// a stray event.
// 
//


#include "t_operation_wait.h"

#include "Testsetup.h"
#include "msvutils.h"

RTest test(_L("MSGS operation wait testrig"));

/**************************************************************************************************

  CWaitTest implementation

**************************************************************************************************/

CWaitTest::CWaitTest(TInt aDelay) : 
CActive(CActive::EPriorityStandard),
iDelay(aDelay)
	{
	}

void CWaitTest::ConstructL()
	{
	User::LeaveIfError(iTimer.CreateLocal());
	}

CWaitTest::~CWaitTest()
	{
	Cancel();
	iTimer.Close();
	}

void CWaitTest::DoCancel()
	{
	iTimer.Cancel();
	}

void CWaitTest::RunL()
	{
	iTimer.After(WaiterRequestStatus(), iDelay);//Start the timer to complete the *waiter* when expired
	Wait();										//By whatever method, execution waits at this point until the timer expires
	iCompleted = ETrue;							//flag that *this* object's wait is over
	iOverseer->WaitComplete();					//Notify observer
	}

void CWaitTest::SetOperationWaitTestObserver(MOperationWaitTestObserver& aOperationWaitTestObserver)
	{
	iOverseer = &aOperationWaitTestObserver;
	}

void CWaitTest::Start()
	{
	CActiveScheduler::Add(this);
	SetActive();
	CompleteRequest(iStatus);					//cause our RunL to be invoked
	}

void CWaitTest::CompleteRequest(TRequestStatus& aRequestStatus, TInt aErr/*= KErrNone*/)	
	{
	TRequestStatus* status = &aRequestStatus;
	User::RequestComplete(status, aErr);
	}

TBool CWaitTest::Completed() const
	{
	return iCompleted;
	}

TInt CWaitTest::Delay() const
	{
	return iDelay;
	}

/**************************************************************************************************

  COperationWaitTest implementation

**************************************************************************************************/

COperationWaitTest::COperationWaitTest(TInt aDelay) :
CWaitTest(aDelay)
	{
	}

COperationWaitTest* COperationWaitTest::NewL(TInt aDelay)
	{
	COperationWaitTest* self = new (ELeave) COperationWaitTest(aDelay);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void COperationWaitTest::ConstructL()
	{
	CWaitTest::ConstructL();
	iOperationWait = CMsvOperationWait::NewLC();	//Safe, since no leave can occur before the pop
	CleanupStack::Pop(iOperationWait);
	}

COperationWaitTest::~COperationWaitTest()
	{
	//Cancel() called in the base class
	delete iOperationWait;
	}

void COperationWaitTest::Wait()
	{
	//The "classic" method--start the next level of the active scheduler which will only return 
	//on CActiveScheduler::Stop()
	CActiveScheduler::Start();								
	}

TRequestStatus& COperationWaitTest::WaiterRequestStatus()
	{
	return iOperationWait->iStatus;
	}

/**************************************************************************************************

  COperationActiveSchedulerWaitTest implementation

**************************************************************************************************/

COperationActiveSchedulerWaitTest::COperationActiveSchedulerWaitTest(TInt aDelay) :
CWaitTest(aDelay)
	{
	}

COperationActiveSchedulerWaitTest* COperationActiveSchedulerWaitTest::NewL(TInt aDelay)
	{
	COperationActiveSchedulerWaitTest* self = new (ELeave) COperationActiveSchedulerWaitTest(aDelay);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void COperationActiveSchedulerWaitTest::ConstructL()
	{
	CWaitTest::ConstructL();
	iOperationActiveSchedulerWait = CMsvOperationActiveSchedulerWait::NewLC();	//Safe, since no leave can occur before the pop
	CleanupStack::Pop(iOperationActiveSchedulerWait);
	}

COperationActiveSchedulerWaitTest::~COperationActiveSchedulerWaitTest()
	{
	//Cancel() called in the base class
	delete iOperationActiveSchedulerWait;
	}

void COperationActiveSchedulerWaitTest::Wait()
	{
	//The "new" method--start the next level of the active scheduler keeping track of which object owns which
	//level, so that the Stop can pass exection back to the correct place
	iOperationActiveSchedulerWait->Start();
	}

TRequestStatus& COperationActiveSchedulerWaitTest::WaiterRequestStatus()
	{
	return iOperationActiveSchedulerWait->iStatus;
	}

/**************************************************************************************************

  COperationWaitTestOverseer implementation

**************************************************************************************************/

COperationWaitTestOverseer::COperationWaitTestOverseer(CWaitTest* aWaitTestA, CWaitTest* aWaitTestB)
	{
	iWaitTestA = aWaitTestA;
	iWaitTestA->SetOperationWaitTestObserver(*this);

	iWaitTestB = aWaitTestB;
	iWaitTestB->SetOperationWaitTestObserver(*this);
	}

COperationWaitTestOverseer::~COperationWaitTestOverseer()
	{
	delete iWaitTestA;
	delete iWaitTestB;
	}

void COperationWaitTestOverseer::Start()
	{
	//Kick off the tests
	iWaitTestA->Start();
	iWaitTestB->Start();
	
	iActiveSchedulerWait.Start();	//Await completion of both...
	}

void COperationWaitTestOverseer::WaitComplete()
	{
	if (iWaitTestA->Completed() && iWaitTestB->Completed())
		iActiveSchedulerWait.AsyncStop();	//Only stop when both have completed
	}
/**************************************************************************************************

  The test itself

**************************************************************************************************/
TInt KShortDelay = 1000000;	//One second
TInt KLongDelay	 = 2000000;	//Two seconds

LOCAL_C void doTestsL()
	{
	test.Start(_L("Testing new implementation, with short delay first"));
	theUtils->WriteComment(_L("Starting first test..."));
	theUtils->WriteComment(_L("Instantiating 2 new-style waiters, with short delay first"));
	CWaitTest* test1A = COperationActiveSchedulerWaitTest::NewL(KShortDelay);
	CleanupStack::PushL(test1A);
	CWaitTest* test1B = COperationActiveSchedulerWaitTest::NewL(KLongDelay);
	CleanupStack::PushL(test1B);
	theUtils->WriteComment(_L("Instantiating overseer"));
	COperationWaitTestOverseer* overseer1 = new (ELeave) COperationWaitTestOverseer(test1A, test1B);
	CleanupStack::Pop(2);			//Now owned by overseer1
	theUtils->WriteComment(_L("Starting overseer, and hence the two waiters"));
	overseer1->Start();
	theUtils->WriteComment(_L("Test successful--overseer (and hence the two waiters) completed OK"));
	delete overseer1;

	test.Next(_L("Testing new implementation, with long delay first"));
	theUtils->WriteComment(_L("Starting second test..."));
	theUtils->WriteComment(_L("Instantiating 2 new-style waiters, with long delay first"));
	CWaitTest* test2A = COperationActiveSchedulerWaitTest::NewL(KLongDelay);
	CleanupStack::PushL(test2A);
	CWaitTest* test2B = COperationActiveSchedulerWaitTest::NewL(KShortDelay);
	CleanupStack::PushL(test2B);
	theUtils->WriteComment(_L("Instantiating overseer"));
	COperationWaitTestOverseer* overseer2 = new (ELeave) COperationWaitTestOverseer(test2A, test2B);
	CleanupStack::Pop(2);			//Now owned by overseer2
	theUtils->WriteComment(_L("Starting overseer, and hence the two waiters"));
	overseer2->Start();
	theUtils->WriteComment(_L("Test successful--overseer (and hence the two waiters) completed OK"));
	delete overseer2;

	//THIS WON'T WORK! It'll panic the AS.
	//(but you're welcome to try it to prove the point)
	/*
	test.Next(_L("Testing old implementation, with short delay first"));
	theUtils->WriteComment(_L("Starting third test..."));
	theUtils->WriteComment(_L("Instantiating 2 old-style waiters, with short delay first"));
	CWaitTest* test3A = COperationWaitTest::NewL(KShortDelay);
	CleanupStack::PushL(test3A);
	CWaitTest* test3B = COperationWaitTest::NewL(KLongDelay);
	CleanupStack::PushL(test3B);
	theUtils->WriteComment(_L("Instantiating overseer"));
	COperationWaitTestOverseer* overseer3 = new (ELeave) COperationWaitTestOverseer(test3A, test3B);
	CleanupStack::Pop(2);			//Now owned by overseer3
	theUtils->WriteComment(_L("Starting overseer, and hence the two waiters. The Active Scheduler will PANIC at this point!"));
	overseer3->Start();
	theUtils->WriteComment(_L("You won't see this message in the logs!"));
	delete overseer3;

	test.Next(_L("Testing old implementation, with long delay first"));
	theUtils->WriteComment(_L("Starting forth test..."));
	theUtils->WriteComment(_L("Instantiating 2 old-style waiters, with long delay first"));
	CWaitTest* test4A = COperationWaitTest::NewL(KLongDelay);
	CleanupStack::PushL(test4A);
	CWaitTest* test4B = COperationWaitTest::NewL(KShortDelay);
	CleanupStack::PushL(test4B);
	theUtils->WriteComment(_L("Instantiating overseer"));
	COperationWaitTestOverseer* overseer4 = new (ELeave) COperationWaitTestOverseer(test4A, test4B);
	CleanupStack::Pop(2);			//Now owned by overseer4
	theUtils->WriteComment(_L("Starting overseer, and hence the two waiters. The Active Scheduler will PANIC at this point!"));
	overseer4->Start();
	theUtils->WriteComment(_L("You won't see this message in the logs!"));
	delete overseer4;
	*/

	theUtils->WriteComment(_L("All tests succeeded!"));
	}
