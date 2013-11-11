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
#include <biouids.h>
#include <msvreg.h>
#include <msvids.h>
#include "MSVSERV.H"
#include <msventry.h>
#include "msvutils.h"
#include <regpsdll.h>
#include <biodb.h>

#include <wapp.h>
#include "biotestutils.h"
#include <ecom/ecom.h>

LOCAL_D RTest test(_L("Final Close Test"));
CBioTestUtils* theUtils;

#define CBASESCRIPTPARSER CBaseScriptParser2

class CParseTestActive : public CTestActive
	{
public:
	IMPORT_C CParseTestActive(CBASESCRIPTPARSER& aParser, const TDesC& aSms) :
	  iParser(aParser), iMessage(aSms) {}
	IMPORT_C void StartL()
		{
		iParser.ParseL(iStatus, iMessage);
		CTestActive::StartL();
		}
private:
	CBASESCRIPTPARSER& iParser;
	const TDesC& iMessage;
	};

class CProcessTestActive : public CTestActive
	{
public:
	IMPORT_C CProcessTestActive(CBASESCRIPTPARSER& aParser) :
	  iParser(aParser){}

	IMPORT_C void StartL()
		{
		iParser.ProcessL(iStatus);
		CTestActive::StartL();
		}
private:
	CBASESCRIPTPARSER& iParser;
	};

class RFinalCloseTest
	{
public:
	//Test case 1
	static TInt Create_Destroy_FinalCloseL(TAny*);
	static TInt Create_Destroy_TestL(RTest& aTest);
	
	static void ThreadPanicTest(const TDesC& aName,TThreadFunction aFunction);
	};
	
//The test code for creating and destroying an implementation
//To be used in the two test cases above
TInt RFinalCloseTest::Create_Destroy_TestL(RTest& aTest)
	{
	const  TPtrC KWappTestTxtFileDir = _L("c:\\test\\bio\\wappfinalclose\\");
	CBioTestUtils* tempUtils = CBioTestUtils::NewL(aTest);
	
	tempUtils->GoServerSideL();	

	CMsvEntrySelection* msvSelection = tempUtils->GenerateMessagesL(KWappTestTxtFileDir, EFalse);
	CleanupStack::PushL(msvSelection);
	TMsvId msgId = (*msvSelection)[0];

	tempUtils->GoClientSideL();	

	CBASESCRIPTPARSER* parser = tempUtils->CreateParserL(msgId);
	test(parser != NULL);
	
	HBufC* message = HBufC::NewL(2048);
	CleanupStack::PushL(message);
	message->Des().Copy(tempUtils->MessageBodyL((*msvSelection)[0]));
	
	CParseTestActive* parseTest = new(ELeave) CParseTestActive(*parser, message->Des());
	CleanupStack::PushL(parseTest);

	parseTest->StartL();
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(parseTest);

	CProcessTestActive* processTest = new(ELeave) CProcessTestActive(*parser);
	CleanupStack::PushL(processTest);
	
	processTest->StartL();
	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(3, msvSelection);

	delete parser;
	parser = NULL;

	delete tempUtils;
	tempUtils = NULL;

	return KErrNone;
	}

TInt RFinalCloseTest::Create_Destroy_FinalCloseL(TAny* /*aArg*/)
	{
	__UHEAP_MARK;
	
	CTrapCleanup* threadcleanup = CTrapCleanup::New();
	CActiveScheduler* scheduler = new(ELeave)CActiveScheduler;
	CActiveScheduler::Install(scheduler);
	
	RTest tempTest(_L("Final Close Test"));

	TRAPD(err,Create_Destroy_TestL(tempTest));
	
	test(err==KErrNone);

	delete scheduler;
	delete threadcleanup;

	tempTest.Close();
	
	__UHEAP_MARKEND;

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
	
	TInt err=thread.Create(aName,aFunction,KDefaultStackSize*8,KMinHeapSize,0x100000,NULL);
	test(err==KErrNone);
	thread.Logon(threadStatus);
	theUtils->WriteComment(_L("Starting thread to create and destroy CMsvBIOWapAccessParser..."));
	thread.Resume();
	
	User::WaitForRequest(threadStatus);
	//Now check why the thread Exit
	theUtils->WriteComment(_L("Checking thread completed correctly..."));

	
	if ((thread.ExitType() != EExitKill) || (thread.ExitReason() != 0))
		theUtils->TestHarnessFailed(KErrAbort);

	test((thread.ExitType()== EExitKill) && (thread.ExitReason() == 0));
	theUtils->WriteComment(_L("Successful."));

	thread.Close();
	User::SetJustInTime(jit);	
	}


LOCAL_C void RunTestL()
	{
	__UHEAP_MARK;
	
	theUtils = CBioTestUtils::NewL(test);
	CleanupStack::PushL(theUtils);

	theUtils->TestStart(1);

	RFinalCloseTest::ThreadPanicTest(_L("Create Destroy Panic Testing"),RFinalCloseTest::Create_Destroy_FinalCloseL);
	
	theUtils->TestHarnessCompleted();
	CleanupStack::PopAndDestroy(theUtils);

	__UHEAP_MARKEND;
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
		
	test.Title();
	test.Start(_L("Final Close tests."));
	
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
