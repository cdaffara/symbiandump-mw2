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
// t_finalclose.cpp
// 
//

#include <e32std.h>
#include <e32test.h>
#include <e32panic.h>
#include <e32uid.h>
#include <f32file.h>
#include <bautils.h>
#include <wapmessage.h>
#include <wapmsgerr.h>
#include "CLWatcher.h"
#include "testlog.h" // CWapPushLog class

#include <ecom/ecom.h>

LOCAL_D RTest test(_L("Final Close Test"));
CWapPushLog* gPushLog;

class CTestLog : public MWapPushLog
	{
	// from MWapPushLog
	virtual void WPLPrintf(const TDesC& /*aDescription*/) {}
	virtual void WPLPrintfL(CPushMessage& /*aMessage*/) {}
	virtual void WPLLogBinaryAsHex(const TDesC& /*aDescription*/) {}
	virtual void WPLLogError(const TDesC& /*aDescription*/,TInt /*aError*/) {}
	};

class CTestObserver : public MConnManObserver
	{
	// from MConnManObserver
	void CMOpenConnectionL(TPushConnPoint& /*aConnPoint*/) {}
	void CMWatcherComplete(CCOWatcherBase& /*aCOWatcher*/, TInt /*aError*/) {}
	};

class RFinalCloseTest
	{
public:
	//Test case 1
	static TInt Create_Destroy_FinalCloseL(TAny*);
	static TInt Create_Destroy_TestL();
	
	static void ThreadPanicTest(const TDesC& aName,TThreadFunction aFunction);
	};

TInt RFinalCloseTest::Create_Destroy_FinalCloseL(TAny*)
	{
	__UHEAP_MARK;
	CTrapCleanup* threadcleanup = CTrapCleanup::New();
	CActiveScheduler* scheduler = new(ELeave)CActiveScheduler;
	CActiveScheduler::Install(scheduler);

	TRAPD(err,Create_Destroy_TestL());
	
	test(err==KErrNone);

	delete scheduler;
	delete threadcleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}
	
//The test code for creating and destroying an implementation
//To be used in the two test cases above
TInt RFinalCloseTest::Create_Destroy_TestL()
	{
	CTestLog* log = new(ELeave) CTestLog;
	CTestObserver* observer = new(ELeave) CTestObserver;

	CCLUnsecureWatcher* watcher = CCLUnsecureWatcher::NewL(*log, *observer);

	delete watcher;
	watcher = NULL;
	delete observer;
	observer = NULL;
	delete log;
	log = NULL;
	return KErrNone;
	}

//The test code is used for capturing the PANIC that occurs as a result of not
//calling REComSession::FinalClose() when using ECOM plugins.
void RFinalCloseTest::ThreadPanicTest(const TDesC& aName,TThreadFunction aFunction)
	{
	test.Next(aName);
	TRequestStatus threadStatus;
	RThread thread;
	TBool jit;
	jit=User::JustInTime();
	User::SetJustInTime(EFalse);
	
	TInt err=thread.Create(aName,aFunction,KDefaultStackSize*8,KMinHeapSize,0x100000,0);
	test(err==KErrNone);
	gPushLog->WPLPrintf(_L("Starting thread to create and destroy CLWatcherBase..."));
	thread.Logon(threadStatus);
	thread.Resume();
	
	User::WaitForRequest(threadStatus);
	gPushLog->WPLPrintf(_L("Checking thread completed correctly..."));
	
	if ((thread.ExitType() != EExitKill) || (thread.ExitReason() != 0))
		gPushLog->WPLPrintf(_L("Failed!"));
	//Now check why the thread Exit
	test(thread.ExitType() == EExitKill);
	test(thread.ExitReason() == 0);

	gPushLog->WPLPrintf(_L("Successful."));

	thread.Close();
	User::SetJustInTime(jit);
	}

LOCAL_C void RunTestL()
	{
	__UHEAP_MARK;
	gPushLog = CWapPushLog::NewL(*test.Console());
	CleanupStack::PushL(gPushLog);

	gPushLog->WPLPrintf(_L("Starting Create Destroy Final Close Panic Test..."));

	RFinalCloseTest::ThreadPanicTest(_L("Create Destroy Panic Testing"),RFinalCloseTest::Create_Destroy_FinalCloseL);
	
	CleanupStack::PopAndDestroy(gPushLog);
	__UHEAP_MARKEND;
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;

	test.Title();
	test.Start(_L("@SYMTestCaseID IWS-WAPBROWSER-WAPPUSH-PUSHWATCHER-T_PUSHWATCHERFINALCLOSE-0001 Final Close tests."));
	
	CTrapCleanup* cleanup = CTrapCleanup::New();
	CActiveScheduler* scheduler = new(ELeave)CActiveScheduler;
	CActiveScheduler::Install(scheduler);

	TRAPD(err,RunTestL());
	test(err==KErrNone);
			
	delete scheduler;
	delete cleanup;
	
	test.End();
	test.Close();
	
	__UHEAP_MARKEND;
	return(0);
	}
