// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Author	Julia Blumin 
// Date		March 2000
// 
//

#include <e32test.h>
#include <e32hal.h>
#include <f32fsys.h>
#include <s32file.h>
#include <barsc.h>

#include <e32uid.h>

//#include <prsreg.h>
//#include <smdef.h>
#include "WapWatcher.h"		

#include <es_sock.h>
#include <e32test.h>
#include "gsmubuf.h"
#include "gsmumsg.h"
#include "smsuaddr.h"
#include "smsustrm.h"
#include "MsvTestUtilsBase.h"
#include "BioTestUtils.h"

#include "WapSocketWatcher.h"



#include "cmstd.h"

_LIT(KTelephoneNumber,"+447747065443");
//_LIT(KTelephoneNumber,"+447771656004");
//_LIT(KTelephoneNumber,"+44802258765");
_LIT(KServiceCenterAddress,"+44385016005");

RTest test(_L("Wap Watcher.dll Harness"));

RSocketServ gSS;
RSocket gSocket;

//----------------------------------------------------------------------------------------
TFileName theLogName;
RFile theLog;
RFs theFs;
RTest gTest(_L("WapWatcher Test Harness"));
CBioTestUtils *theUtils;
//----------------------------------------------------------------------------------------


const TUid KTestUid			= {0x10001270};
const TUid KTestUid1		= {0x10220108};
const TUid KTestUid2		= {0x10220109};


void TestScheduler::Error( TInt anError ) const
	{
	User::Leave( anError );
	}

//----------------------------------------------------------------------------------------
LOCAL_C void InitTestUtils()
//----------------------------------------------------------------------------------------
	{
	testUtils = CBioTestUtils::NewLC(gTest,ETuCleanMessageFolder);
	gTest.Start(_L("CBIODatabase"));
	gTest.Console()->ClearScreen();
	}

//----------------------------------------------------------------------------------------
LOCAL_C void CloseTestUtils()
//----------------------------------------------------------------------------------------
	{
	testUtils->TestHarnessCompleted();
	CleanupStack::PopAndDestroy(testUtils);
	gTest.End();
	gTest.Close();
	}



//----------------------------------------------------------------------------------------
LOCAL_C void SetupSchedulerL()
//----------------------------------------------------------------------------------------
	{
	CActiveScheduler::Install(new(ELeave)CActiveScheduler);
	User::LeaveIfError(theFs.Connect());

	TFileName logName;
	theLogName.Copy(RProcess().FileName());
	TInt start = theLogName.LocateReverse('\\');
	TInt end = theLogName.LocateReverse('.');
	theLogName = theLogName.Mid(start + 1, end - start - 1);

	// create the log filename
	theLogName.Insert(0, _L("C:\\"));
#if defined(__WINS__)
	theLogName.Append(_L(".WINS."));
#else
	theLogName.Append(_L(".MARM."));
#endif
#if defined(_UNICODE)
	theLogName.Append(_L("UNICODE."));
#else
	theLogName.Append(_L("ASCII."));
#endif
#if defined(_DEBUG)
	theLogName.Append(_L("DEB."));
#else
	theLogName.Append(_L("REL."));
#endif
	theLogName.Append(_L("LOG"));

	// create the logfile
	User::LeaveIfError(theLog.Replace(theFs, theLogName, EFileWrite|EFileShareExclusive));
	TBuf8<256> text;
	text.Copy(theLogName);
	theLog.Write(text);
	theLog.Write(_L8("\nTest results\n"));
	}

LOCAL_C void CloseScheduler()
	{
	theLog.Write(_L8("Tests completed\n"));
	test.Printf(_L("Results saved in %S"), &theLogName);

	theLog.Close();
	theFs.Close();

	delete CActiveScheduler::Current();
	}


LOCAL_C void OpenEsock()
	{
    RSocketServ gSS;
	test.Start(_L("Opening socket server"));
	TRAPD(ret,gSS.Connect());
    test(ret==KErrNone);


    test.Next(_L("Finding protocols"));

	TProtocolDesc protoInfo;
	TProtocolName protocolname=KSmsDatagram;
	ret=gSS.FindProtocol(protocolname,protoInfo);
	if(ret!=KErrNone)
		{
		test.Printf(_L("Dummy protocol not loaded"));
		test(EFalse);
		}

	test.Next(_L("Making sockets"));
	TUint numProtocols;
	test(gSS.NumProtocols(numProtocols)==KErrNone);
	test.Printf(_L("Loaded %d protocols\n"),numProtocols);
//	ret=gSS.GetProtocolInfo(1,protoInfo);
//	test(ret==KErrNone);

	test.Printf(_L("Creating Family: %d,Sock: %d,protocol: %d\n\r"),protoInfo.iAddrFamily,protoInfo.iSockType,protoInfo.iProtocol);
	ret=gSocket.Open(gSS,protoInfo.iAddrFamily,protoInfo.iSockType,protoInfo.iProtocol);
	if (ret!=KErrNone)
		test.Printf(_L("Failed %d\n\r"),ret);
	test(ret==KErrNone);

	test.Printf(_L("created %s socket OK\n"),protoInfo.iName.PtrZ());
	}

LOCAL_C void CloseEsock()
	{
	test.Next(_L("Closing socket"));
	gSocket.Close();

	test.Next(_L("Closing ESOCK"));
	gSS.Close();
	}



//----------------------------------------------------------------------------------------
LOCAL_C void SendMessage()
//----------------------------------------------------------------------------------------
	{
//
// Sending message
//

	TSmsAddr smsaddr;
	smsaddr.SetSmsAddrFamily(ESmsAddrSendOnly);
	TRAPD(ret, gSocket.Bind(smsaddr));
    test(ret==KErrNone);

	User::After(10000000);

	test.Next(_L("Sending message"));
	CSmsBuffer* buffer=CSmsBuffer::NewL();
	CSmsMessage* smsmessage=CSmsMessage::NewL(CSmsPDU::ESmsSubmit,buffer);
	CleanupStack::PushL(smsmessage);
	smsmessage->SetToFromAddressL(KTelephoneNumber);
	smsmessage->SmsPDU().SetServiceCenterAddressL(KServiceCenterAddress);

	buffer->InsertL(0,_L("Dis ees a message\n"));
	
	CSmsSubmit& submit=(CSmsSubmit&) smsmessage->SmsPDU();
	submit.SetStatusReportRequest(EFalse);
//	submit.SetStatusReportRequest(ETrue);
	submit.SetValidityPeriod(TTimeIntervalMinutes(30));

	RSmsSocketWriteStream writestream(gSocket);
	writestream << *smsmessage;
	writestream.CommitL();

	TPckgBuf<TUint> sbuf;
	TRequestStatus status;
	gSocket.Ioctl(KIoctlSendSmsMessage,status,&sbuf, KSolSmsProv);
	User::WaitForRequest(status);
	User::LeaveIfError(status.Int());


	delete smsmessage;
	}

//----------------------------------------------------------------------------------------
LOCAL_C void CreateDestroySocketWatcher(TInt aTestNumber)
//----------------------------------------------------------------------------------------
	{
	testUtils->TestStart(aTestNumber, _L("Create/Destroy Sock Watchers"));
	// reserve some space on the cleanup stack
/*	for (TInt ii=0;ii<1000;++ii)
		CleanupStack::PushL(&ii);
	CleanupStack::Pop(1000);


	CWapWatcher* WapWatcher1 = CWapWatcher::NewL(theFs, log);
	delete SocketWatcher1;

	__UHEAP_RESET;
	__UHEAP_MARK;
	CWapWatcher* WapWatcher2 = CWapWatcher::NewL(theFs, log);
	delete SocketWatcher2;
	__UHEAP_MARKEND;
*/
	testUtils->TestFinish(aTestNumber,0 );
	}

//----------------------------------------------------------------------------------------
LOCAL_C void StartWatchers(TInt aTestNumber)
//----------------------------------------------------------------------------------------
	{ 
	testUtils->TestStart(aTestNumber, _L("Starting Watchers"));

	
	CWatcher* watcher = NULL;
	
	SetupSchedulerL();
	CWatcherWait* wait = CWatcherWait::NewLC();


	// Create the watcher
	TRAPD(error, watcher = CWatcher::NewL(CActive::EPriorityStandard));
	if (error)
	{
		testUtils->TestFinish(aTestNumber,error );
		return;
	}

	CleanupStack::PushL(watcher);
	
	// Make sure watcher starts
	test.Start(_L("Waiting for SMS Messages"));

	wait->Start();
	watcher->Start(wait->iStatus);

	// Just Start and wait...
	CActiveScheduler::Start();
	
	testUtils->WriteComment(_L("Started Watchers"));
	
	//TestWatcherL();
	theLog.Write(_L8("Test 1 OK\n"));


	// Close the watcher
	watcher->Cancel();
	test(wait->iStatus.Int() == KErrCancel);
	test.End();

	CleanupStack::PopAndDestroy(2); // watcher, wait
	CloseScheduler();

	testUtils->TestFinish(aTestNumber,0 );
	}


//----------------------------------------------------------------------------------------
LOCAL_C void doMainL()
//----------------------------------------------------------------------------------------
	{
	TInt aTestNumber = 1;
	InitTestUtils();

	test.Start(_L("Create/Destroy Socket Watcher"));
	CreateDestroySocketWatcher(aTestNumber++);
	
	test.Next(_L("Start Watchers"));
	//SendMessage();
	StartWatchers(aTestNumber++);
	
	CloseTestUtils();
	}



//----------------------------------------------------------------------------------------
GLDEF_C TInt E32Main()
//----------------------------------------------------------------------------------------
	{	
__UHEAP_MARK;	
	CTrapCleanup* theCleanup = CTrapCleanup::New();
	TRAPD(ret,doMainL());		
	test(ret==KErrNone);
	delete theCleanup;	
	
__UHEAP_MARKEND;
	return(KErrNone);
	}
