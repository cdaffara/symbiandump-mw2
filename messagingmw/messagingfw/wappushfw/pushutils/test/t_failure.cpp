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
//

#include <e32std.h>
#include <e32test.h>
#include <push/pushmessage.h>
#include <push/cpushhandlerbase.h>
#include <push/pushlog.h>
#include "testlog.h"
#include "t_failure.h"

_LIT(KTestTitle,"T_Failure");
_LIT8(KHeader,"This is a dummy header");
_LIT8(KBody,"This is a dummy message body");
_LIT(KAppIdBufFormat, "0x%08x");

// Setting up parameters for CPushHandlerBase::NewL()
const TInt KAppId = 2;
const TInt KAppIdLength = 10;

// Global variables
RTest gTest(_L("t_Failure Test Harness"));

CPushMessageTester* CPushMessageTester::NewL()
	{
	CPushMessageTester* self = new (ELeave) CPushMessageTester();
	return self;
	}

CPushMessageTester::CPushMessageTester()
	{
	}

CPushMessageTester::~CPushMessageTester()
	{
	delete iHeader;
	delete iBody;
	delete iMessage;
	}

void CPushMessageTester::PluginFailureTestL()
	{
	TBuf<KAppIdLength> AppIdBuf;
	AppIdBuf.Format(KAppIdBufFormat, KAppId);
	CPushHandlerBase* pushHandler = NULL; 
	pushHandler = CPushHandlerBase::NewL(AppIdBuf, KUidPushHandlerBase);
	delete pushHandler;
	}

void CPushMessageTester::FailureTestL()
	{
	// constructing CPushMessage with the arguments as member variables
	iHeader	= KHeader().AllocL();
	iBody	= KBody().AllocL();

	iMessage = CPushMessage::NewL(iHeader, iBody);
	iHeader = NULL;
	iBody = NULL;
	}

CFailureTester* CFailureTester::NewL(CWapPushLog& aLog)
	{
	CFailureTester* self = new (ELeave) CFailureTester(aLog);
	return self;
	}

CFailureTester::CFailureTester(CWapPushLog& aLog)
:iLog(aLog)
	{
	}

CFailureTester::~CFailureTester()
	{
	delete iScheduler;
	}

void CFailureTester::LogFormatInt(const TDesC& aFormat, TInt aInt)
	{
	iFormatBuffer.Format(aFormat, aInt);
	iLog.WPLPrintf(iFormatBuffer);
	}

void CFailureTester::LogFormatInt(const TDesC& aFormat, TInt aInt1, TInt aInt2)
	{
	iFormatBuffer.Format(aFormat, aInt1, aInt2);
	iLog.WPLPrintf(iFormatBuffer);
	}

void CFailureTester::LogFormatStr(const TDesC& aFormat, const TDesC& aDes)
	{
	iFormatBuffer.Format(aFormat, &aDes);
	iLog.WPLPrintf(iFormatBuffer);
	}

void CFailureTester::LogStr(const TDesC& aDes)
	{
	iLog.WPLPrintf(aDes);
	}

void CFailureTester::HeapFailureTest()
	{
	__LOG_ALWAYS("Test Results");
	__LOG_ALWAYS(" ");

	__LOG_ALWAYS("***** HEAP FAILURE TEST FOR PUSH MESSAGE *****");

	TInt error;
	TBool finished = EFalse;
	TInt failCount = 0;
	CPushMessageTester* push = NULL;
	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		__UHEAP_MARK;
		TRAP(error, push = DoPushMessageTestL());
		if (error == KErrNone)
			{
			delete push;
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling DoPushMessageTestL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			// Check if error is out of memory
			gTest(error == KErrNoMemory);
			__UHEAP_RESET;
			}
		__UHEAP_MARKEND;
		}

	__LOG_ALWAYS("RTEST: SUCCESS ***** PUSH MESSAGE TEST SUCCESSFUL ******");
	__LOG_ALWAYS(" ");
	}

void CFailureTester::PluginHeapFailureTest()
	{
	__LOG_ALWAYS("***** HEAP FAILURE TEST FOR PLUGIN *****");

	TInt error;
	TBool finished = EFalse;
	TInt failCount = 0;
	CPushMessageTester* push = NULL;
	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		__UHEAP_MARK;
		TRAP(error, push = CreatePushHandlerBaseL());
		if (error == KErrNone)
			{
			delete push;
			__UHEAP_RESET;
			LogFormatInt(_L("Made %d attempts at calling CreatePushHandlerBaseL()"), failCount);
			finished = ETrue;
			}
		else  // Handle error
			{
			// Check if error is out of memory
			gTest(error == KErrNoMemory);
			__UHEAP_RESET;
			}
		REComSession::FinalClose();
		__UHEAP_MARKEND;
		}

	__LOG_ALWAYS("RTEST: SUCCESS ***** PLUGIN TEST SUCCESSFUL ******");
	__LOG_ALWAYS(" ");
	__LOG_ALWAYS("Tests Completed");
	}


CPushMessageTester* CFailureTester::DoPushMessageTestL()
	{
	// First case
	CPushMessageTester* tester = CPushMessageTester::NewL();	
	CleanupStack::PushL(tester);
	tester->FailureTestL();
	
	// Second case
	CPushMessage* message = DoPushMessageTest2L();
	delete message;

	CleanupStack::Pop(tester);
	return tester;	
	}


CPushMessageTester*  CFailureTester::CreatePushHandlerBaseL()
	{
	CPushMessageTester* tester = CPushMessageTester::NewL();	
	CleanupStack::PushL(tester);
	tester->PluginFailureTestL();
	CleanupStack::Pop(tester);
	return tester;	
	}


CPushMessage* CFailureTester::DoPushMessageTest2L()
	{
	// constructing CPushMessage with the arguments put on the cleanupstack

	HBufC8*	header	= KHeader().AllocLC();
	HBufC8* body	= KBody().AllocLC();

	CPushMessage* push = CPushMessage::NewL(header, body);	
	
	CleanupStack::Pop(2);  //Header, Body
	return push;	
	}

void CFailureTester::InitSchedulerL()
	{
	iScheduler = new (ELeave) CActiveScheduler();
	CActiveScheduler::Install( iScheduler );
	}


/**
 * setup test environment, create CFailureTester object and run tests.
 * 
 * This is an OOM test. 
 * It tests 3 cases: 
 * (i) constructing CPushMessage with the arguments as member variables.
 * (ii) constructing CPushMessage with the arguments put on the cleanupstack.
 * (iii) constructing CPushHandlerBase 
 * Running Instructions:
 * 1. Build this test harness:	cd \wappush\pushutils\group
 *								bldmake bldfiles
 *								abld test build wins udeb t_failure
 * 2. Create log directories in	c:\logs\push 
 * 3. Run TFailure.exe:			cd \epoc32\release\wins\udeb
 *								tfailure -dtextshell -- 
 */
LOCAL_C void doMainL()
	{
	gTest.Start(KTestTitle);
			
	gTest.Printf(_L("@SYMTestCaseID IWS-WAPBROWSER-PUSHUTILS-T_FAILURE-0001 "));	
	
	CWapPushLog* log = CWapPushLog::NewL(*gTest.Console());
	log->SetLogFileName(_L("TFailure.txt"));
	CleanupStack::PushL(log);

	CFailureTester* myTester = CFailureTester::NewL(*log);
	CleanupStack::PushL(myTester);
	myTester->HeapFailureTest();
	myTester->InitSchedulerL();
	myTester->PluginHeapFailureTest();

	
	CleanupStack::PopAndDestroy(2, log); //log, myTester
	gTest.End();
	gTest.Close();
	}


/** entry point for test executable */
GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;

	CTrapCleanup* theCleanup = CTrapCleanup::New();
	TRAPD(ret,doMainL());		
	gTest(ret==KErrNone);
	delete theCleanup;	

	__UHEAP_MARKEND;
	return(KErrNone);
	}
