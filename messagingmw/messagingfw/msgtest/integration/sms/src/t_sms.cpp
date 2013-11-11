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
// GT Messaging Test Harness T_SMS
// Test Harness: T_SMS
// Component: SMS
// Owner: AA
// Brief Description: GT Messaging SMS Smoke Test
// Detailed Description: Tests
// - SMS Client MTM
// - SMS utility classes (CSmsHeader, CSmsNumber, CSmsSettings, TSmsUtilities)
// - Reading and writing Service Centres on the SIM
// - Reading and writing messages to the SIM
// - Sending and receiving messages
// Input Files REQUIRED:
// <DRIVE>:\msgtest\sms\defaults.script
// <DRIVE>:\msgtest\sms\smoke_recv.script
// <DRIVE>:\msgtest\sms\smoke_enum.script
// Intermediate Files Produced: None
// Output files produced: <DRIVE>:\msglogs\T_Sms.<PLATFORM>.<VARIANT>.log
// Building Instructions:
// cd \msg\test\smoke\
// bldmake bldfiles
// abld test build
// Running Instructions:
// All Platforms:
// 1. Build T_DB test harness from COMMDB component:
// cd \commdb\group
// bldmake bldfiles
// abld test build t_db
// 2. Run T_DB and select the your modem (probably GSM Mobile Phone)
// 3. Build the test utilities:
// cd \msg\testutils\group\
// bldmake bldfiles
// abld test build
// 4. Edit \epoc32\wins\c\msgtest\sms\defaults.script. Replace the telephone number ("Recipients") with your mobile's number. You may also need to change the service centre ("SC") number (the default is for Vodafone UK).
// WINS Only:
// 1. Run T_DB.exe (if it has not already been run) and select your modem (GSM mobile phone).
// 2. \epoc32\release\wins\<VARIANT>\T_Sms.exe can be used at the command prompt or executable can be run from Windows Explorer.
// Platform other than WINS:
// 1. Copy \epoc32\release\<PLATFORM>\<VARIANT>\T_Sms.exe onto the other platform (or onto the CF card)
// 2. Copy \epoc32\release\<PLATFORM>\<VARIANT>\T_DB.exe onto the other platform (or onto the CF card)
// 3. Copy \epoc32\release\<PLATFORM>\<VARIANT>\MsvTestUtils.DLL into <DRIVE>:\system\libs on the other platform (or onto the CF card)
// 4. Copy \epoc32\release\<PLATFORM>\<VARIANT>\SmsTestUtils.DLL into <DRIVE>:\system\libs on the other platform (or onto the CF card)
// 5. Copy \epoc32\wins\c\msgtest\sms\smoke_recv.script, smoke_enum.script and defaults.script into <DRIVE>:\msgtest\sms on the other platform (or onto the CF card)
// 6. Run T_DB.exe on the other platform (if it has not already been run) and select your modem (GSM mobile phone).
// 7. Run T_Sms.exe on the other platform.
// 
//

#include <e32test.h>
#include <e32uid.h>
#include <e32std.h>

#include <barsc.h>
#include <smsuaddr.h>

// Test code header files
#include "T_SmsEnum.h"
#include "T_smcm.h"
#include "T_SmsSc.h"
#include "T_SmsSendRecv.h"

RTest test(_L("T_Sms Smokin' Testrig"));
CTrapCleanup* theCleanup;

_LIT(KSmsEnumTest, "smoke_enum.script");
_LIT(KSmsRecvTest, "smoke_recv.script");
_LIT(KBetweenTests, "############################################################\n");

const TInt KWaitForWatchersToStart = 15000000; //10 secs

LOCAL_C void PrintTestStart(CSmsTestBase& aTestBase)
	{
	TPtrC name(aTestBase.TestName());
	aTestBase.SmsTest().Printf(KBetweenTests);
	aTestBase.SmsTest().Printf(_L("Starting %S Test\n"), &name);
	aTestBase.SmsTest().Printf(KBetweenTests);
	}

LOCAL_C void PrintTestFail(CSmsTestBase& aTestBase)
	{
	TPtrC name(aTestBase.TestName());
	aTestBase.SmsTest().Printf(KBetweenTests);
	aTestBase.SmsTest().Printf(_L("%S Test Failed with %d\n"), &name, aTestBase.Error());
	aTestBase.SmsTest().Printf(KBetweenTests);
	}

LOCAL_C void StartAndVerifyTestCompleteL(CSmsTestBase& aTestBase)
	{
	PrintTestStart(aTestBase);
	aTestBase.StartL();
	if (!aTestBase.Complete())
		{
		PrintTestFail(aTestBase);
		User::Leave(aTestBase.Error());
		}
	}

LOCAL_C void doMainL()
	{
	// Open the socket server

	RSocketServ serv;
	RSocket socket;

	User::LeaveIfError(serv.Connect());
	CleanupClosePushL(serv);
	
	TProtocolDesc protoinfo;
	TProtocolName protocolname(KSmsDatagram);
	User::LeaveIfError(serv.FindProtocol(protocolname,protoinfo));
	User::LeaveIfError(socket.Open(serv,protoinfo.iAddrFamily,protoinfo.iSockType,protoinfo.iProtocol));

	TSmsAddr smsaddr;
	smsaddr.SetSmsAddrFamily(ESmsAddrSendOnly);
	User::LeaveIfError(socket.Bind(smsaddr));

	CleanupClosePushL(socket);

	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install( scheduler );

	CSmsTestUtils* smsTest = CSmsTestUtils::NewLC(test);
	smsTest->iSmokeTest = ETrue;

	TInt currentTest = 0;

	smsTest->NotifySaPhoneOnL();
	smsTest->WaitForInitializeL();

	//Start the watchers
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	smsTest->Printf(_L("Waiting %d secs for watchers to start\n"), KWaitForWatchersToStart / 1000000); 
	CTestUtilsWatcherStarter* watchers = CTestUtilsWatcherStarter::NewL(wait->iStatus, KWaitForWatchersToStart);
	wait->Start();
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(wait);
	CleanupStack::PushL(watchers);

	CSmsTestBase* testBase = CSmsScTest::NewLC(*smsTest, KNullDesC, currentTest);
	StartAndVerifyTestCompleteL(*testBase);
	CleanupStack::PopAndDestroy(testBase);
	
	testBase = CSmsEnumTest::NewLC(*smsTest, KSmsEnumTest, currentTest);
	StartAndVerifyTestCompleteL(*testBase);
	CleanupStack::PopAndDestroy(testBase);

	testBase = CSmsSendRecvTest::NewLC(*smsTest, KSmsRecvTest, currentTest);
	StartAndVerifyTestCompleteL(*testBase);
	CleanupStack::PopAndDestroy(testBase);
	
	testBase = CSmcmTest::NewLC(*smsTest, KNullDesC, currentTest);
	StartAndVerifyTestCompleteL(*testBase);
	CleanupStack::PopAndDestroy(testBase);
	
	testBase = CSmutTest::NewLC(*smsTest, KNullDesC, currentTest);
	StartAndVerifyTestCompleteL(*testBase);
	CleanupStack::PopAndDestroy(testBase);

	CleanupStack::PopAndDestroy(3); //SmsTest, scheduler, watchers

	CleanupStack::PopAndDestroy(&socket);
	CleanupStack::PopAndDestroy(&serv);
	}

GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;
	test.Start(_L("Setup"));
	theCleanup = CTrapCleanup::New();
	TRAPD(ret,doMainL());		
//	test(ret==KErrNone);
	delete theCleanup;	
	test.Console()->SetPos(0, 13);
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return(KErrNone);
	}
