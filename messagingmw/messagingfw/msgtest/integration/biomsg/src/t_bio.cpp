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
// Name of test harness: T_BIO
// Component: SMOKE
// Owner: KP
// Brief description of test harness:
// Sends, receives and parses Internet Configuration messages.  An internet
// connection is made using the received settings.
// Detailed description of test harness:
// As above
// Input files required to run test harness:
// \MSG\TEST\SMOKE\BIO\SCRIPT\BIOTEST.SCRIPT	<DRIVE>:\msgtest\BIO\biotest.script
// Intermediate files produced while running test harness:
// None
// Output files produced by running test harness:
// <DRIVE>:\msglogs\T_BIO.<PLATFORM>.<VARIANT>.LOG.txt
// Description of how to build test harness:
// cd \msg\test\smoke\
// bldmake bldfiles
// abld test build
// Description of how to run test harness:
// The following instructions are the same for all platforms:
// 1. Build T_DB test harness from COMMDB component:
// cd \commdb\group
// bldmake bldfiles
// abld test build t_db
// 2. Build the test utilities:
// cd \msg\testutils\group\
// bldmake bldfiles
// abld build
// 3. Build the BIO test utilities:
// cd \biomsg\bits\
// bldmake bldfiles
// abld build
// WINS running instructions:
// 1. \epoc32\release\wins\<VARIANT>\T_BIO.exe can be used at the command prompt
// or executable can be run from Windows Explorer.
// All other platform running instructions:
// 1. Copy \epoc32\release\<PLATFORM>\<VARIANT>\T_BIO.exe onto the other platform
// 2. Copy \epoc32\release\<PLATFORM>\<VARIANT>\MSVTESTUTILS.DLL into 
// <DRIVE>:\system\libs on the other platform
// 3. Copy \epoc32\release\<PLATFORM>\<VARIANT>\EMAILTESTUTILS.DLL into 
// <DRIVE>:\system\libs on the other platform
// 4. Copy \epoc32\release\<PLATFORM>\<VARIANT>\SMSTESTUTILS.DLL into 
// <DRIVE>:\system\libs on the other platform
// 5. Copy \epoc32\release\<PLATFORM>\<VARIANT>\BITS.DLL into 
// <DRIVE>:\system\libs on the other platform
// 4. Run T_BIO.exe on the other platform
// 
//

#include <e32uid.h>
#include <e32test.h>
#include <watcher.h>
#include <txtrich.h>
#include <smuthdr.h>
#include <commdb.h>

#include "t_bio.h"
#include "sendrecv.h"
#include "parsertest.h"

const TTimeIntervalMicroSeconds32 KWaitForWatchersToStart = 40000000;
const TTimeIntervalMicroSeconds32 KWaitForWatchersToStartDisplayInterval = 5000000;
const TTimeIntervalMicroSeconds32 KWaitForWatchersToDie = 5000000;

RTest test(_L("T_Bio Biomsg SmokeTestrig"));
CTrapCleanup* theCleanup;


LOCAL_C void doMainL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install( scheduler );

	CBioSmokeTest* bioSmokeTest = CBioSmokeTest::NewL(test);
	CleanupStack::PushL(bioSmokeTest);

	bioSmokeTest->StartL();

	CleanupStack::PopAndDestroy(2); //SmsTest, scheduler
	}

GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;
	test.Start(_L("Setup"));
	theCleanup = CTrapCleanup::New();
	TRAPD(ret,doMainL());		
	test(ret==KErrNone);
	delete theCleanup;	
	test.Console()->SetPos(0, 13);
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return(KErrNone);
	}


// CEntryObserver
//
CEntryObserver::CEntryObserver(	CSmsTestUtils& aTestUtils, CMsvEntrySelection& aReceived, const TInt& aCurrentMessageNum)
: CBase(), iTestUtils(aTestUtils), iReceived(aReceived), iCurrentMessageNum(aCurrentMessageNum)
	{
	}


CEntryObserver::~CEntryObserver() 
	{
	delete iNew;
	}

void CEntryObserver::HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	if (aEvent == EMsvNewChildren && iCurrentMessageNum > -1)
		{
		delete iNew;
		iNew = NULL;
		iNew = ((CMsvEntrySelection*)aArg1)->CopyL();

		TInt count = iNew->Count();

		CSmsHeader* header = CSmsHeader::NewL(CSmsPDU::ESmsDeliver, *iTestUtils.iRichText);
		CleanupStack::PushL(header);

		TInt msgRecv = 0;

		while (count--)
			{
			TMsvId id = iNew->At(count);
			iTestUtils.SetEntryL(id);

			CMsvStore* store = iTestUtils.ReadStoreL();
			CleanupStack::PushL(store);
	
			header->RestoreL(*store);

			if (header->Type() == CSmsPDU::ESmsDeliver)
				{
				iReceived.AppendL(id);
				msgRecv++;
				}

			CleanupStack::PopAndDestroy(); //store
			}

		CleanupStack::PopAndDestroy(); //header

		if (msgRecv)	
			iTestUtils.Printf(_L("%d Messages Received in inbox\n"), msgRecv);
		
		}
	}

//
// CBioSmokeTest
//
CBioSmokeTest::~CBioSmokeTest()
	{
	CancelWatcher();
	// delete everything
	delete iTimer;
	
	delete iSendReceive;
	delete iParserTest;

	delete iEntryObserver;
	delete iReceived;
	delete iEntryForObserver;

	delete iSmsTestUtils;

	delete iWatcher;

	iSocket.Close();
	iSocketServ.Close();
	}

CBioSmokeTest::CBioSmokeTest()
	{
	}

CBioSmokeTest* CBioSmokeTest::NewL(RTest& aTest)
	{
	CBioSmokeTest* self = new (ELeave) CBioSmokeTest();
	CleanupStack::PushL(self);

	self->ConstructL(aTest);

	CleanupStack::Pop();
	return self;
	}

void CBioSmokeTest::StartL()
	{
	// fire up the watcher
	StartWatcherL();
	// send receive 

	iBioSmokeState = EBioSmokeSendRecv;
	
	// create the send receive 
	iSendReceive = CSendReceive::NewL(*iSmsTestUtils);
	iSendReceive->StartL(iReceived);

	delete iSendReceive;
	iSendReceive = NULL;

	// test parser
	iParserTest = CParserTest::NewL(*iSmsTestUtils);
	iParserTest->StartL(iReceived);
	}

void CBioSmokeTest::ConstructL(RTest& aTest)
	{
	iSmsTestUtils = CSmsTestUtils::NewL(aTest);
	iSmsTestUtils->SetLogToFile();

	iSmsTestUtils->NotifySaPhoneOnL();

	User::LeaveIfError(iSocketServ.Connect());
	TProtocolDesc protoinfo;
	TProtocolName protocolname(KSmsDatagram);
	User::LeaveIfError(iSocketServ.FindProtocol(protocolname,protoinfo));
	User::LeaveIfError(iSocket.Open(iSocketServ,protoinfo.iAddrFamily,protoinfo.iSockType,protoinfo.iProtocol));
	
	TSmsAddr smsaddr;
	smsaddr.SetSmsAddrFamily(ESmsAddrSendOnly);
	User::LeaveIfError(iSocket.Bind(smsaddr));

	iSmsTestUtils->WaitForInitializeL();
	}

void CBioSmokeTest::StartWatcherL()
	{
	test.Next(_L("Start the Watcher"));

	delete iReceived;
	iReceived = new (ELeave) CMsvEntrySelection();

	delete iEntryObserver;
	iEntryObserver = new (ELeave) CEntryObserver(*iSmsTestUtils, *iReceived, iCurrentMessageNum);

	delete iEntryForObserver;
	iEntryForObserver = Session().GetEntryL(KMsvGlobalInBoxIndexEntryId);
	iEntryForObserver->AddObserverL(*iEntryObserver);

	// Create the watcher
	delete iWatcher;
	iWatcher = NULL;
	iWatcher = CTestUtilsWatcherStarter::NewL(CActive::EPriorityStandard);

	TTimeIntervalMicroSeconds32 wait = KWaitForWatchersToStart;
	CTestTimer* timer = CTestTimer::NewL();

	while (wait.Int() > 0)
		{
		iSmsTestUtils->Printf(_L("Waiting %d secs for watchers to start...\n"), wait.Int() / 1000000);
		timer->After(KWaitForWatchersToStartDisplayInterval);
		wait = wait.Int() - KWaitForWatchersToStartDisplayInterval.Int();
		CActiveScheduler::Start();
		}

	delete timer;
	}

void CBioSmokeTest::CancelWatcher()
	{
	if (iWatcher)
		{
		iWatcher->Cancel();
		CActiveScheduler::Start();
		delete iWatcher;
		iWatcher = NULL;

		iSmsTestUtils->Printf(_L("Waiting %d seconds for the watchers to die\n"), KWaitForWatchersToDie.Int() / 1000000);

		CTestTimer* timer = NULL;
		TRAPD(ret,timer=CTestTimer::NewL());
		if (ret==KErrNone)
			{
			timer->After(KWaitForWatchersToDie);
			CActiveScheduler::Start();
			delete timer;
			}		
		iSmsTestUtils->TestHarnessCompleted();
		}
	}
