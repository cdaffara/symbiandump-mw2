// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "t_schsendexe.h"
#include <msvids.h>
#include <msvuids.h>

CTrapCleanup* theCleanup;

void doMainL(RTest& aTest)
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install( scheduler );

	CSchSendExeTest* schSendTest = CSchSendExeTest::NewL(aTest);
	CleanupStack::PushL(schSendTest);

	schSendTest->StartL();

	CleanupStack::PopAndDestroy(2); //schSendTest, scheduler

	aTest.Printf(_L("\nWaiting 5 seconds before closing...\n"));
	User::After(5000000);
	}

GLDEF_C TInt E32Main()
	{
	RTest test(_L("Scheduled Sending Test Harness"));
	__UHEAP_MARK;
	test.Title();
	test.Start(_L("Scheduled Sending Test Harness"));
	theCleanup = CTrapCleanup::New();
	test(theCleanup !=NULL);
	TRAPD(error,doMainL(test));
	test(error==KErrNone);
	delete theCleanup;
	test.Printf(_L("Completed with return code %d"),error);
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return KErrNone;
	}

CSchSendExeTest* CSchSendExeTest::NewL(RTest& aTest)
	{
	CSchSendExeTest* self = new (ELeave) CSchSendExeTest(aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); //self
	return self;
	}

CSchSendExeTest::CSchSendExeTest(RTest& aTest)
: CSchSendTestUtils(aTest)
	{
	}

void CSchSendExeTest::ShowMenuL()
	{
	RunAutoL();
	}

CSchSendExeTest::~CSchSendExeTest()
	{
	}

void CSchSendExeTest::RunAutoL()
	{
	TInt nextTest = 0;
	TestStart(++nextTest, _L("Operation Fail"));
	TestOperationFailureL();
	TestFinish(nextTest, KErrNone);

// DEF051316
	}

void CSchSendExeTest::TestOperationFailureL()
	{
	Printf(_L("TestOperationFailure\n"));
	TTime now;
	now.HomeTime();
	TMsvEntry entry;
	CreateMessageLC(entry, KMsvGlobalOutBoxIndexEntryId, now);

	iSelection->Reset();
	iSelection->AppendL(entry.Id());

	const TInt KTestOpFailError = -111;
	TPckgBuf<TInt> pkg(KTestOpFailError);
	iSchTestActive->TransferCommandSyncL(EScheduleOpFail, *iSelection, pkg, ETrue);

	CSchSendTestWaitForState* wait = CSchSendTestWaitForState::NewL(*this);
	CleanupStack::PushL(wait);

	wait->Reset();
	wait->AppendL(KMsvSendStateResend);
	wait->StartL(*iSelection, 1000000 * 60 * 4); //4 mins

	SetEntryL(entry.Id());
	entry = Entry();
	iRTest(entry.iError == KTestOpFailError);

	Printf(_L("Wait completed with %d\n"), wait->iStatus.Int());
	iRTest(wait->iStatus.Int() == KErrNone);

	CleanupStack::PopAndDestroy(wait);
	CleanupStack::PopAndDestroy(); //message
	}

void CSchSendExeTest::TestSessionFailureL()
	{
	Printf(_L("TestSessionFailure\n"));
	TTime now;
	now.HomeTime();
	TMsvEntry entry;
	CreateMessageLC(entry, KMsvGlobalOutBoxIndexEntryId, now);

	iSelection->Reset();
	iSelection->AppendL(entry.Id());

	// create a semaphore that causes CMsvSession construction to fail
	RSemaphore moving;
	User::LeaveIfError(moving.CreateGlobal(KMsvMovingSemaphore,0));
	CleanupClosePushL(moving);

	iSchTestActive->TransferCommandSyncL(EScheduleAllL, *iSelection, ETrue);

	CSchSendTestWaitForState* wait = CSchSendTestWaitForState::NewL(*this);
	CleanupStack::PushL(wait);

	Printf(_L("Waiting %d secs to check message scheduled\n"), 5);
	wait->AppendL(KMsvSendStateScheduled);
	wait->StartL(*iSelection, 1000000 * 5); //5 secs

	Printf(_L("Wait completed with %d\n"), wait->iStatus.Int());
	iRTest(wait->iStatus.Int() == KErrNone);

	Printf(_L("Closing Move Semaphore\n"));
	Printf(_L("Waiting %d secs for message to send\n"), 60 * 2);
	moving.Close();
	wait->Reset();
	wait->AppendL(KMsvSendStateSent);
	wait->StartL(*iSelection, 1000000 * 60 * 2); //2 mins

	Printf(_L("Wait completed with %d\n"), wait->iStatus.Int());
	iRTest(wait->iStatus.Int() == KErrNone);

	CleanupStack::PopAndDestroy(wait);
	CleanupStack::Pop(); //moving
	CleanupStack::PopAndDestroy(); //message
	}


