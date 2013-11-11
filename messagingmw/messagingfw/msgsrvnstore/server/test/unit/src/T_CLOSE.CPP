// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "MSERVER.H"
#include <msvids.h>
#include "Testsetup.h"

RTest test(_L("MSV Server Starting/Closing Testrig"));

const TInt KMsvTestInterval = 0x80000;  // 1/4 of 


// test classes //

class CSpecialObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/) {iEvent=aEvent; CActiveScheduler::Stop();};
public:
	TMsvSessionEvent iEvent;
	};

// test classes //

class CCloseTimer : public CTimer
	{
public:
	CCloseTimer();
	void ConstructL();
	void RunL(void);	
	};

LOCAL_D CCloseTimer* theTimer;

CCloseTimer::CCloseTimer()
: CTimer(EPriorityLow)
	{}

void CCloseTimer::RunL()
	{
	CActiveScheduler::Stop();
	}

void CCloseTimer::ConstructL()
	{
	CTimer::ConstructL();
	CActiveScheduler::Add(this);
	}

//

LOCAL_C TInt FindServer()
	{
	TFileName name;
	TFindServer findS(KMsvServerName);
	return findS.Next(name);
	}


LOCAL_C void TestTimerWorks()
	{
	TInt count=0;
	TInt error=KErrNone;
	while (error==KErrNone)
		{
		test.Printf(_L("."));
		theTimer->After(KMsvTestInterval);
		CActiveScheduler::Start();
		error = FindServer();
		test(++count<=4);
		}
	test(error==KErrNotFound);
	test.Printf(_L("\n"));
	}


LOCAL_C void TestServerLives()
	{
	for (TInt count=0; count<6; count++)
		{
		test.Printf(_L("."));
		theTimer->After(KMsvTestInterval);
		CActiveScheduler::Start();
		test(FindServer()==KErrNone);
		}
	test.Printf(_L("\n"));
	}

LOCAL_C void TestAutoClosingL()
	{
	CMsvSession* session1;
	CMsvSession* session2;

	// create and delete session
	CDummyObserver* obD = new(ELeave) CDummyObserver;
	session1 = CMsvSession::OpenSyncL(*obD);	
	delete session1;
	TestTimerWorks();
	
	// create two sessions and delete one
	session1 = CMsvSession::OpenSyncL(*obD);	
	session2 = CMsvSession::OpenSyncL(*obD);	
	delete session1;
	TestServerLives();
	delete session2;
	TestTimerWorks();

	// create two sessions and delete one
	session1 = CMsvSession::OpenSyncL(*obD);	
	session2 = CMsvSession::OpenSyncL(*obD);	
	delete session2;
	TestServerLives();
	delete session1;
	TestTimerWorks();

	// create delete create
	session1 = CMsvSession::OpenSyncL(*obD);	
	delete session1;
	session1 = CMsvSession::OpenSyncL(*obD);	
	TestServerLives();
	delete session1;
	TestTimerWorks();
	delete obD;
	}



LOCAL_C void TestForcedClosingL()
	{
	CMsvSession* session1;
	CMsvSession* session2;

	CSpecialObserver* obS1 = new(ELeave) CSpecialObserver;
	CSpecialObserver* obS2 = new(ELeave) CSpecialObserver;
	CDummyObserver* obS3 = new(ELeave) CDummyObserver;

	// single session
	session1 = CMsvSession::OpenSyncL(*obS1);
	session1->CloseMessageServer();
	CActiveScheduler::Start();
	test(obS1->iEvent==MMsvSessionObserver::EMsvCloseSession);
	User::After(100000);
	test(FindServer()==KErrNone || FindServer()==KErrNotFound);
	delete session1;
	User::After(200000);
	test(FindServer()==KErrNone || FindServer()==KErrNotFound);

	// two sessions - both active before
	session1 = CMsvSession::OpenSyncL(*obS1);
	session2 = CMsvSession::OpenSyncL(*obS2);
	session1->CloseMessageServer();
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	test(obS1->iEvent==MMsvSessionObserver::EMsvCloseSession || \
				obS1->iEvent==MMsvSessionObserver::EMsvEntriesCreated || \
				obS1->iEvent==MMsvSessionObserver::EMsvServerTerminated);
	test(obS2->iEvent==MMsvSessionObserver::EMsvCloseSession|| \
				obS1->iEvent==MMsvSessionObserver::EMsvServerTerminated);
	User::After(100000);
	test(FindServer()==KErrNone || FindServer()==KErrNotFound);
	delete session1;
	User::After(100000);
	test(FindServer()==KErrNone || FindServer()==KErrNotFound);
	delete session2;
	User::After(200000);
	test(FindServer()==KErrNone || FindServer()==KErrNotFound);
	
	// two sessions - both active before
	session1 = CMsvSession::OpenSyncL(*obS1);
	session2 = CMsvSession::OpenSyncL(*obS2);
	session1->CloseMessageServer();
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	test(obS1->iEvent==MMsvSessionObserver::EMsvCloseSession || \
				obS1->iEvent==MMsvSessionObserver::EMsvServerTerminated);
	test(obS2->iEvent==MMsvSessionObserver::EMsvCloseSession || \
				obS1->iEvent==MMsvSessionObserver::EMsvServerTerminated);
	User::After(100000);
	test(FindServer()==KErrNone || FindServer()==KErrNotFound);
	delete session2;
	User::After(100000);
	test(FindServer()==KErrNone || FindServer()==KErrNotFound);
	delete session1;
	User::After(200000);
	test(FindServer()==KErrNone || FindServer()==KErrNotFound);

	// two sessions - one started after
	session1 = CMsvSession::OpenSyncL(*obS1);
	session1->CloseMessageServer();
	CActiveScheduler::Start();
	test(obS1->iEvent==MMsvSessionObserver::EMsvCloseSession);
	User::After(100000);
	test(FindServer()==KErrNone || FindServer()==KErrNotFound);
	TRAPD(error, CMsvSession::OpenSyncL(*obS3));
	test(error == KErrServerBusy || error == KErrNone);
	delete session1;
	User::After(300000);
	test(FindServer()==KErrNone || FindServer()==KErrNotFound);

	delete obS1;
	delete obS2;
	delete obS3;
	}


LOCAL_C void TestTerminationL()
	{
	CMsvSession* session;
	CSpecialObserver* ob = new(ELeave) CSpecialObserver;

	// single session
	session = CMsvSession::OpenSyncL(*ob);
	session->CloseMessageServer();
	// terminate the server

	TFullName name;
	TFindProcess find(_L("!MsvServer*"));
	REPORT_IF_ERROR(find.Next(name));

	RProcess process;
	process.Open(find);
	process.Terminate(KErrGeneral);
	process.Close();

	delete session;
	delete ob;
	}


LOCAL_C void TestObserverClosingL()
	{
	CMsvSession* session1;
	CMsvSession* session2;
	CMsvSession* session3;

	// create observers
	CSpecialObserver* obs1 = new(ELeave) CSpecialObserver;
	CleanupStack::PushL(obs1);
	CSpecialObserver* obs2 = new(ELeave) CSpecialObserver;
	CleanupStack::PushL(obs2);
	CSpecialObserver* obs3 = new(ELeave) CSpecialObserver;
	CleanupStack::PushL(obs3);

	// Delete normal session
	session2 = CMsvSession::OpenAsObserverL(*obs2);	
	CleanupStack::PushL(session2);

	session3 = CMsvSession::OpenAsObserverL(*obs3);	
	CleanupStack::PushL(session3);

	session1 = CMsvSession::OpenSyncL(*obs1);	
	delete session1;

	CActiveScheduler::Start();
	CActiveScheduler::Start();

	theTimer->After(KMsvTestInterval);
	CActiveScheduler::Start();

	// Check that observer sessions are told to close
	test(obs1->iEvent != MMsvSessionObserver::EMsvCloseSession);
	test(obs2->iEvent == MMsvSessionObserver::EMsvCloseSession);
	test(obs3->iEvent == MMsvSessionObserver::EMsvCloseSession);

	CleanupStack::PopAndDestroy(2); // session2, session3
	User::After(200000);
	obs1->iEvent = obs2->iEvent = obs3->iEvent = MMsvSessionObserver::EMsvEntriesCreated;

	// Delete observer session
	session3 = CMsvSession::OpenAsObserverL(*obs3);	
	CleanupStack::PushL(session3);

	session1 = CMsvSession::OpenSyncL(*obs1);	
	CleanupStack::PushL(session1);

	session2 = CMsvSession::OpenAsObserverL(*obs2);	
	delete session2;

	theTimer->After(KMsvTestInterval);
	CActiveScheduler::Start();

	// Check that none of the sessions are told to close
	test(obs1->iEvent != MMsvSessionObserver::EMsvCloseSession);
	test(obs2->iEvent != MMsvSessionObserver::EMsvCloseSession);
	test(obs3->iEvent != MMsvSessionObserver::EMsvCloseSession);

	// Now delete the remaining normal session
	CleanupStack::PopAndDestroy(); // session1

	CActiveScheduler::Start();

	theTimer->After(KMsvTestInterval);
	CActiveScheduler::Start();

	// Check that observer session was told to close
	test(obs3->iEvent == MMsvSessionObserver::EMsvCloseSession);

	CleanupStack::PopAndDestroy(); // session3
	User::After(200000);
	obs1->iEvent = obs2->iEvent = obs3->iEvent = MMsvSessionObserver::EMsvEntriesCreated;

	// Delete observer session

	session1 = CMsvSession::OpenAsObserverL(*obs1);	
	CleanupStack::PushL(session1);

	session2 = CMsvSession::OpenAsObserverL(*obs2);	
	CleanupStack::PushL(session2);

	session3 = CMsvSession::OpenAsObserverL(*obs3);	
	delete session3;

	CActiveScheduler::Start();
	CActiveScheduler::Start();

	theTimer->After(KMsvTestInterval);
	CActiveScheduler::Start();

	// Check that the remaining observer sessions are told to close
	test(obs1->iEvent == MMsvSessionObserver::EMsvCloseSession);
	test(obs2->iEvent == MMsvSessionObserver::EMsvCloseSession);
	test(obs3->iEvent != MMsvSessionObserver::EMsvCloseSession);

	obs1->iEvent = obs2->iEvent = MMsvSessionObserver::EMsvEntriesCreated;
	CleanupStack::PopAndDestroy(); // session2

	theTimer->After(KMsvTestInterval);
	CActiveScheduler::Start();

	// The sessions observer session should not be notified again
	test(obs1->iEvent != MMsvSessionObserver::EMsvCloseSession);
	test(obs2->iEvent != MMsvSessionObserver::EMsvCloseSession);

	CleanupStack::PopAndDestroy(); // session1
	User::After(200000);

	CleanupStack::PopAndDestroy(3); // obs1, obs2, obs3
	}



LOCAL_C void doTestsL()
	{
	theTimer = new(ELeave)CCloseTimer;
	CleanupStack::PushL(theTimer);
	theTimer->ConstructL();

	CMsgsTestUtils::Start(_L("Forced closing"));
	TestForcedClosingL();
	CMsgsTestUtils::Complete();

	test.Printf(_L("Waiting 2 1/2 minutes before running Test Termination test's...\n"));
	User::After(150000000);
	
	CMsgsTestUtils::Next(_L("Server termination"));
	TestTerminationL();
	CMsgsTestUtils::Complete();

	CleanupStack::PopAndDestroy(); // theTimer
	}
