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
    

#include <e32test.h>
#include <e32svr.h>		// for RDebug::Print
#include <e32std.h>
#include <flogger.h>
#include "t_wappush.h"
#include "pushtests.h"
#include <push/pushmessage.h>
#include <ecom/ecom.h>

// service setup includes...
#include <msvstd.h>
#include <msvids.h>
#include "t_serviceutils.h"
#include <pushentry.h>

#include "cwappushfailmessagetest.h"

_LIT(KPushMtmDatFile, "z:\\system\\mtm\\WapPushMtmU.dat");
_LIT(KWapPushTestTitle,"WAP Push tests");
_LIT(KPushLogDir, "push");
_LIT(KPushLogFile, "WapPushLogging.txt");
_LIT(KSpace, " ");
_LIT(KStartTest,"Test Results");
_LIT(KFinishedTest,"Tests Completed");
_LIT(KTestDirectory,"C:\\Logs\\push\\");
_LIT(KTest,"> TEST ");
_LIT(KPassed," PASSED\n");
_LIT(KFailed," Failed\n RTEST: FAIL :");	// RTEST: FAIL : required for the DABS RTest result parser to work
_LIT (KOOMTestStep, "OOM Test step at %d\n" );

LOCAL_D RTest test( KWapPushTestTitle );

// Test harness requirements
//
// Uses an active object state machine to run tests.
// Creates a Connection Manager object.
// Creates a Dummy wap stack via CDummyWapStack::NewL()
// Feeds a push message to Dummy wap stack 
// Starts Connection Manager - just add active object to scheduler and start?
// Needs to know when completed - and callback?
// Verifies that CPushMessage received is the same as sent
// Closes/Destroys connection manager - delete?
//

// Note : 
// -To run the test harness on hardware, build the ROM image using 
//  TWappush.iby file in \Wap-Browser\WapPush\rom\.. directory.
// -Build the Wappush component and the TWappush test harness for DEBUG mode
// -Create a folder c:\logs\push\.. to get the WappushLogging.txt logfile.
// 

class CTestScheduler : public CActiveScheduler
	{
	virtual void Error(TInt anError) const;
	};

void CTestScheduler::Error(TInt anError) const
	{
	TBuf<80> buf;
	
	_LIT(KComment, "!! Error - %d\n-> Test Scheduler error handler called");
	buf.Format(KComment, anError);

	// Write to log file
	RFileLogger::Write(KPushLogDir, KPushLogFile, EFileLoggingModeAppend, buf);

	_LIT(KTestPanic,"CTestScheduler RunL leave");
	User::Panic(KTestPanic,anError);
	}




// cwappushtestengine

void CWapPushTestEngine::DoCancel()
	{
	// cancel the current test
	if (iCurrentTest)
		iCurrentTest->Cancel();
	}

void CWapPushTestEngine::RunL()
	{
	switch (iState)
		{
	case EIdle:
		{
		if(iTest!=EFinished)
			{
			test.Start(_L("Starting Unit Test"));
			}
		TRAPD(err,RunNextTestL())
		if(iTest!=EFinished)
			{
			test.End();
			test.Close();		
			}
		if (err!=KErrNone)
			iConsole->Printf(KTextFailedTest);
		}
		break;
	case ERunningTest:
		break;
	case EShuttingDown:
		// Clear Out the Plugin Cache
//		CPluginServerClient* psc = CPluginServerClient::NewL();
//		psc->PluginFactory().ClearCache();
//		delete psc;
		CActiveScheduler::Stop();
		break;
		}
	}

/**
 * Static factory function for a new test engine
 */
CWapPushTestEngine* CWapPushTestEngine::NewL()
	{
	CWapPushTestEngine* self = new(ELeave) CWapPushTestEngine();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 
	return self;
	}

/**
 * Constructor for the test engine
 * Creates a console and sets itself active
 */
void CWapPushTestEngine::ConstructL()
	{
	// Write to log file
	RFileLogger::Write(KPushLogDir, KPushLogFile, EFileLoggingModeAppend, KStartTest);
	RFileLogger::Write(KPushLogDir, KPushLogFile, EFileLoggingModeAppend, KSpace);
	
	iConsole = Console::NewL(KWapPushTestTitle,TSize(KConsFullScreen,KConsFullScreen));
	// set up service BEFORE active scheduler...
	InitialiseMsgServiceL();
	//
	CActiveScheduler::Add(this);
	SetActive();
	TRequestStatus* stat = &iStatus;
	User::RequestComplete(stat,KErrNone);
	}

/** 
 * Destructor for CWapPushTestEngine
 * Cancels any outstanding requests and deletes member variables
 */
CWapPushTestEngine::~CWapPushTestEngine()
	{
	Cancel();
	delete iCurrentTest;
	delete iConsole;
	}

/**
 * This should be called by tests to indicate that they have 
 * completed and whether they were sucessful or not 
 */
void CWapPushTestEngine::TestCompleted(TInt aResult)
	{
	if (aResult==KErrNone)
		{
		TBuf16<124> log;
		log = iCurrentTest->TestName();
		log.Append(KTest);
		log.Append(iCurrentTest->TestName());
		log.Append(KPassed);
		test.Printf(log);
		iState=EIdle;
		iTest=(TTest)(iTest+1);
		}
	else
		{
		TBuf16<124> log1;
		log1 = iCurrentTest->TestName();
		log1.Append(KTest);
		log1.Append(iCurrentTest->TestName());
		log1.Append(KFailed);
		test.Printf(log1);
		test.Printf(KTextFailedTest);
		iConsole->Printf(KTextFailedTest);
		iState = EShuttingDown;
		}
	// set active again
	TRequestStatus* stat = &iStatus;
	User::RequestComplete(stat,KErrNone);
	SetActive();
	}

/** 
 * Run the next test
 * The switch statement lists all tests to be run
 * 
 */
void CWapPushTestEngine::RunNextTestL()
	{
	// delete any old tests
	delete iCurrentTest;
	iCurrentTest=NULL;
	//

	switch (iTest)
		{
	case EFailMessageTest:
		test.Next( _L("Test - Failing Messages (were causing panics) started"));
		iCurrentTest = new (ELeave)CWapPushFailMessageTest();
		break;
	case ESIAMessage:
		test.Next( _L("Test - Service Intiation Application started"));
		iCurrentTest = new (ELeave) CWapPushSIAMessageTest();
		break;
	case EUnknownMessage:
		test.Next( _L("Test - Unknown Handler Test started"));
		iCurrentTest = new (ELeave) CWapPushUnkMessageTest();
		break;
	case ESLMessage:
		test.Next( _L("Test - Service Loading started"));
		iCurrentTest = new (ELeave) CWapPushSLMessageTest();
		break;
	case ESLCMessage:
		test.Next (_L("Test - Service Loading Content started") );
		iCurrentTest = new ( ELeave )CWapPushSLCMessageTest();
		break;
	case ESIMessage:
		test.Next( _L("Test - Service Indication started"));
		iCurrentTest = new (ELeave) CWapPushSIMessageTest();
		break;
	case ESICMessage:
		test.Next( _L("Test - Service Indication Content started"));
		iCurrentTest = new (ELeave) CWapPushSICMessageTest();
		break;
	case EMultipartMixed:
		test.Next( _L("Test - Multi-part Mixed Message Test started"));
		iCurrentTest = new (ELeave) CWapPushMMMessageTest();
		break;
	case EMultipartRelated:
		test.Next( _L("Test - Multi-part Related Message Test started"));
		iCurrentTest = new (ELeave) CWapPushMRMessageTest();
		break;
	case EMultipartAlternative:
		test.Next( _L("Test - Multi-part Alternative Message Test started"));
		iCurrentTest = new (ELeave) CWapPushMAMessageTest();
		break;
	case EReleaseThree:
		test.Next( _L("Test - Release Three started"));
		iCurrentTest = new (ELeave) CWapPushR3Test();
		break;
	case ECorruptMsgs:
		test.Next( _L("Test - Corrupt Message Test started"));
		iCurrentTest = new (ELeave) CWapPushCorruptMessageTest();
		break;
	case EClientMtmFind:
		test.Next( _L("Test - WapPushClientMtm Find Test started"));
		iCurrentTest = new (ELeave) CWapClientMtmFindTest();
		break;
	case EINC081489:
		test.Next( _L("Duplicate SI messages [CINC081489]"));
		iCurrentTest = new (ELeave) CINC081489();
		break;
	case EInvalidDTDMessage:
		test.Next ( _L("Invalid WAP DTD Message Test") );
		iCurrentTest = new ( ELeave) CInvalidWAPDTDMessages();
		break;
	case EDRMMessage:
		test.Next( _L("Test - DRM started"));
		iCurrentTest = new (ELeave) CWapPushDRMMessageTest();
		break;	
	case EReleaseTwo:
	default:
		// We've finished - set status to shut down and complete our own request
		SetPriority(EPriorityIdle);

		// Write to log file
		RFileLogger::Write(KPushLogDir, KPushLogFile, EFileLoggingModeAppend, KSpace);
		RFileLogger::Write(KPushLogDir, KPushLogFile, EFileLoggingModeAppend, KFinishedTest);

		iConsole->Printf(KTextFinishedTest);
		iState = EShuttingDown;
		TRequestStatus* stat = &iStatus;
		User::RequestComplete(stat,KErrNone);
		SetActive();
		return;
		}

	iCurrentTest->SetEngine(this);
	iState = ERunningTest;
	iCurrentTest->BeginTest();
	}

 /**
 * print a descriptor to the console - prefixes aString text with aName
 * @param aName the name of the test
 * @param aString the text to be displays
 */
void CWapPushTestEngine::Printf(const TDesC& aName, const TDesC& aString)
	{
	TBuf<12> format;
	format.Zero();
	format.AppendNum((TInt)iTest);
	_LIT(KTextCoreFormat,".%S: %S\n");
	format.Append(KTextCoreFormat);
	iConsole->Printf(format,&aName,&aString);
	}

/** 
 * get a character from the console 
 */
TKeyCode CWapPushTestEngine::Getch()
	{
	return iConsole->Getch();
	}

/**
 * return a reference to the console used by the test harness
 */
CConsoleBase& CWapPushTestEngine::Console() const 
	{
	return *iConsole;
	}

/** connect to the push message service

	if no message service set up then create one else
	use existing service and remove any existing messages 
*/
void CWapPushTestEngine::InitialiseMsgServiceL()
	{
	CMsvEntrySelection* idArray = new (ELeave) CMsvEntrySelection();
	CleanupStack::PushL(idArray);

	CWapPushTestMsgUtils* msgUtils = CWapPushTestMsgUtils::NewL(); 
	CleanupStack::PushL(msgUtils);
	TMsvId pushService = KMsvNullIndexEntryId; 
	TMsvId pushFolder = KMsvNullIndexEntryId;

	// uncomment this if you want to remove existing service entries
	//msgUtils->ClearExistingServiceEntriesL(KUidMtmWapPush);

	msgUtils->PushServiceIdL(pushService, idArray);
	if ((pushService == KMsvNullIndexEntryId) && (idArray->Count() ==0) ) // Found no service
		{
		pushService = msgUtils->CreateServiceL();
		}
	// install mtm - leaves if found to already exist 
	TRAP_IGNORE(msgUtils->InstallMtmGroupL(KPushMtmDatFile));


	idArray->Reset();
	msgUtils->PushFolderIdL(pushFolder, idArray);

	if ((pushFolder == KMsvNullIndexEntryId) && (idArray->Count() ==0) ) // Found no service
		pushFolder = msgUtils->CreatePushMsgFolderL();
		
	
	//Clean out all previous push entries from under the Push Service Entry
	msgUtils->RemoveServiceEntryChildrenL(pushService);

	// Clean the push msg folder of existing entries
	msgUtils->RemoveEntriesFromLocalServiceFolderL(pushFolder, KUidMtmWapPush);
	// Clean the inbox of all existing Wap Push entries....
	msgUtils->RemoveEntriesFromLocalServiceFolderL(KMsvGlobalInBoxIndexEntryId, KUidMtmWapPush);

	CleanupStack::PopAndDestroy(2);  //idarray, msgUtils
	}



/** Add the test to the active scheduler and set it active
 *  by completing its own request
 */
void CWapPushTest::BeginTest()
	{
	CActiveScheduler::Add(this);
	SetActive();
	TRequestStatus* stat = &iStatus;
	User::RequestComplete(stat,KErrNone);
	}


/** 
 * Print a descriptor to the console 
 * @param aString descriptor to print
 */
void CWapPushTest::Printf(const TDesC& aString)
	{
	iEngine->Printf(TestName(),aString);
	}

/** 
 * Check a test result and panic if false
 * @param aTestResult the result to check
 * @param aLine the line number - can be provided by MSVC macro __LINE__
 * #define TEST(A) Test(A,__LINE__)
 * TEST(err=KErrNone)
 */
void CWapPushTest::Test(TInt aTestResult,TInt aLine)
	{
	if (!aTestResult)
		{
		User::Panic(TestName(),aLine);
		}
	}

/**
* Printf
* 
* @param aDescription The text to print/log
*/
void CWapPushTest::WPLPrintf(const TDesC& aDescription)
	{
	const TInt KWatcherLogBuffer = 256;
	// Write to log file
	RFileLogger::Write(KPushLogDir, KPushLogFile, EFileLoggingModeAppend, aDescription);

	// Write to console
	_LIT(KPushLogFmt,"Push:%S\n");
	TPtrC	buf = aDescription.Left(Min(KWatcherLogBuffer, aDescription.Length()));
	iEngine->Console().Printf(KPushLogFmt,&buf);
	}

/**
 * Takes the data in a Push Message and prints it to console and the logs it to file.
 * Uses the accessor functions of the CPushMessage class and this classes Printf function 
 * to perform this.  Currently 4 headers and the message body are displayed and 
 * logged. The headers are:
 *		PushID, Content-Type, X-Application-ID, Expiry Date, 
 *
 *	@param	CPushMessage& aMessage
 *				in:  a reference to a Push Message.
 */
void CWapPushTest::WPLPrintfL(CPushMessage& aMessage)
	{
	const TInt KWatcherLogBuffer=256;
	TBuf<KWatcherLogBuffer> buf;
	
	// Content type
	_LIT(KLogContentFmt,"Content Type : \t%S");
	TPtrC contentPointer;
	aMessage.GetContentType(contentPointer);
	buf.Format(KLogContentFmt,&contentPointer);
	buf.Append('\n');
	WPLPrintf(buf);

	// Date Field
	_LIT(KLogDateTimeFmt," %-B%:0%J%:1%T%:2%S%+B   %D %N %Y  %4 %5 %3");
	_LIT(KLogDateFmt,"Date           :\t%S");
	TBool foundField;
	TTime timeValue;
	foundField = aMessage.GetHeaderField(EHttpDate, timeValue);
	if (foundField)
		{
		TRAPD(err, timeValue.FormatL(buf, KLogDateTimeFmt));
		if (err == KErrNone)
			{
			TBuf<KWatcherLogBuffer> dateBuf;
			dateBuf.Format(KLogDateFmt, &buf);
			WPLPrintf(dateBuf);
			}
		}

	//Expires Field
	aMessage.GetHeaderField(EHttpExpires, timeValue);
	if (foundField)
		{
		TRAPD(err, timeValue.FormatL(buf, KLogDateTimeFmt));
		if (err == KErrNone)
			{
			TBuf<KWatcherLogBuffer> dateBuf;
			dateBuf.Format(KLogDateFmt, &buf);
			WPLPrintf(dateBuf);
			}
		}

	// X-Wap-Application-ID
	TBool isInt = EFalse;
	TInt ID = 0;
	TPtrC8 generalPtr;
	_LIT(KLogAppIdIntFmt,"App ID         :\t%X");
	TRAPD(err, foundField = aMessage.GetAppIdL(generalPtr, ID, isInt));
	if ( err == KErrNone && foundField) // App ID is present
		{
		if (isInt)  // Field in integer format
			{
			buf.Format(KLogAppIdIntFmt,ID);
			}
		else  // should be descriptor format
			{
			buf.Copy(generalPtr);
			}
		WPLPrintf(buf);
		}

	// Content-Disposition
	TPtrC8 dispositionPtr;
	_LIT(KLogDisposition,"Content-Disposition:\t");
	foundField = aMessage.GetHeaderField(EHttpContentDisposition, dispositionPtr);
	if (foundField) // App ID is present
		{
		// Convert 8 bit to 16 bit
		HBufC* tempAddr = HBufC::NewLC(dispositionPtr.Length());
		tempAddr->Des().Copy(dispositionPtr);
		WPLPrintf(KLogDisposition);
		WPLPrintf(*tempAddr);
		CleanupStack::PopAndDestroy(tempAddr);
		}

	//Message Header Binary
	_LIT(KLogMsgHdr,"Header Binary:");
	WPLPrintf(KLogMsgHdr);
	aMessage.GetHeader(generalPtr);
	
	HBufC* tempHdr = HBufC::NewLC(generalPtr.Length());
	tempHdr->Des().Copy(generalPtr);
	WPLLogBinaryAsHex(*tempHdr);
	CleanupStack::PopAndDestroy(); //tempHdr

	//Message Body
	aMessage.GetMessageBody(generalPtr);

	// Dump Body As Text
	_LIT(KLogMsgBody,"Body Text:");
	WPLPrintf(KLogMsgBody);
	HBufC* tempBody = HBufC::NewLC(generalPtr.Length());
	tempBody->Des().Copy(generalPtr);
	WPLPrintf(*tempBody);

	// Dump Body As Hex
	_LIT(KBodyBinary,"\nBody Binary:");
	WPLPrintf(KBodyBinary);
	WPLLogBinaryAsHex(*tempBody);

	CleanupStack::PopAndDestroy(); //tempBody
	}
 

 /**
 * Prints out Buffer data in the format:
 *	%X %X %X %X %X %X %X\n etc 
 * For example
 *	AB CD 01 12 34 A2 
 *
 *	@param	aDescription
 *				in:  the descriptor to be dumped into the log
 */
void CWapPushTest::WPLLogBinaryAsHex(const TDesC& aDescription)
	{
	const TInt KWatcherLogBuffer=256;
	_LIT(KHexSpace,"%02X ");
	TBuf<KWatcherLogBuffer> hexBuf;
	TBuf<KWatcherLogBuffer> buf;

	TInt i = 0, bodyLen = aDescription.Length();

	for (; i < bodyLen; i++)
		{
		hexBuf.Format(KHexSpace,aDescription[i]);
		buf.Append(hexBuf);
		if ( i && ((i+1) % 8) == 0  )
			{
			WPLPrintf(buf);
			buf.Zero();
			}
		}

	if (buf.Length())
		WPLPrintf(buf);
	}

void CWapPushTest::WPLLogError(const TDesC& aDescription,TInt aError)
	{
	_LIT(KErrorLogFmt,"%S, Error\t%d");
	const TInt KWatcherLogBuffer=256;
	TBuf<KWatcherLogBuffer> buf;
	buf.Format(KErrorLogFmt,&aDescription,aError);
  	buf.Append('\n');
	WPLPrintf(buf);
	}

CWapPushOOMTestEngine* CWapPushOOMTestEngine::NewL ()
	{
	CWapPushOOMTestEngine* self = new (ELeave)CWapPushOOMTestEngine;
	CleanupStack::PushL ( self );
	self->ConstructL ();
	CleanupStack::Pop ();	
	return self;
	}

CWapPushOOMTestEngine::~CWapPushOOMTestEngine ()
	{
	// Do nothing. 	
	}
	
void CWapPushOOMTestEngine::TestCompleted ( TInt aResult )
	{
	SetError ( aResult );
	if ( aResult == KErrNone )
		{
		// The test passed. Increment the test number to next.
		iTest=(TTest)(iTest+1);	
		iState = EIdle;
		}		
	else
		{
		// Something went wrong shutdown the engine
		iState = EShuttingDown;
		}
		// set active again
		TRequestStatus* stat = &iStatus;
		User::RequestComplete(stat,KErrNone);
		SetActive();
	}
	
void CWapPushOOMTestEngine::DoCancel ()
	{
	CWapPushTestEngine::DoCancel ();
	}

CWapPushTestEngine::TTest CWapPushOOMTestEngine::NextTest () const
	{
	return iTest;		
	}

void CWapPushOOMTestEngine::SetNextTest ( TInt aTest )
	{
	iTest = (TTest)aTest;
	}

void CWapPushOOMTestEngine::RunL ()
	{
	switch (iState)
		{
	case CWapPushTestEngine::EIdle:
		{
	//Skip oom test for ESIMessage. it goes in loop
		if(iTest==ESIAMessage || iTest==ESICMessage || iTest==EDRMMessage)
			{
			iTest=(TTest)(iTest+1);
			}
		TRAPD( err,RunNextTestL() );
		if ( err != KErrNone )
			TestCompleted ( err );
		}
		break;
	case CWapPushTestEngine::ERunningTest:
		break;
	case CWapPushTestEngine::EShuttingDown:
		// We are stopping. Stop the active scheduler and exit.
		CActiveScheduler::Stop();
		break;		
		}
	}

TInt CWapPushOOMTestEngine::Error () const
	{
	return iError;		
	}

void CWapPushOOMTestEngine::SetError ( TInt aErr )
	{
	iError = aErr;	
	}

LOCAL_C void StartOomTestL ( TInt& aNextTest )
	{
		CWapPushOOMTestEngine* oomEngine = NULL;
		oomEngine = CWapPushOOMTestEngine::NewL();
		CleanupStack::PushL ( oomEngine );
		oomEngine->SetNextTest ( aNextTest );
		// All the action happens within the main event loop		
		CActiveScheduler::Start();		
		aNextTest = (TInt)oomEngine->NextTest ();
		User::LeaveIfError ( oomEngine->Error () );
		CleanupStack::PopAndDestroy ();	
	}
		
// main loop
//
GLDEF_C TInt E32Main()
    {
__UHEAP_MARK;
	
	// Install exception handler
	CTrapCleanup* theCleanup = CTrapCleanup::New(); 

	test.Printf(_L("@SYMTestCaseID IWS-WAPBROWSER-WAPPUSH-TPUSH-T_WAPPUSH-0001 "));	
	
	// Create test directory
	TRAPD(
		error,
		RFs fs;
		User::LeaveIfError(fs.Connect());
		CleanupClosePushL(fs);

		fs.MkDirAll(KTestDirectory);
		CleanupStack::PopAndDestroy(&fs);
		);
	if (error != KErrNone)
		User::Panic(_L("Failed to create test directory"), error);

	CTestScheduler* as = new CTestScheduler;
	if (!as)
		User::Panic(_L("Failed to create active scheduler"),KErrNoMemory);
	CActiveScheduler::Install(as); // Install active scheduler

	CWapPushTestEngine* engine=NULL;
	TRAPD(err,engine = CWapPushTestEngine::NewL());
	if (err != KErrNone)
		User::Panic(_L("Failed to create test engine object"),err);
	
	// All the action happens within the main event loop
	CActiveScheduler::Start();
	// Returned from start so shutdown

	delete engine;
	
	// Run OOM test cases.
	TInt nextTest = 1;
	TInt failAt = 0;
	err = KErrNoMemory;
	test.Start(_L("Starting OOM Test"));			
	while ( err != KErrNone )
		{
		failAt++;
		test.Printf ( KOOMTestStep, failAt );
		__UHEAP_SETFAIL( RHeap::EDeterministic, failAt );
		__UHEAP_MARK;
		TRAP ( err, StartOomTestL ( nextTest ) );
		REComSession::FinalClose();
		__UHEAP_MARKEND;
		__UHEAP_RESET;		
		}
	delete as;
	delete theCleanup;
	test.End();
	test.Close();		
__UHEAP_MARKEND;
	
	User::Heap().Check();
    return(KErrNone);
	}



