// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "httptestutils.h"

#if !defined(__TESTSCRIPTS_H__)
#include "TestScripts.h"
#endif


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

_LIT(KWapIniFolder, 	"wapini");
_LIT(KWapIni, 			"wap.ini");
_LIT(KIniWapTestTitle,	"INI Based Wap Test");
_LIT(KWAPMessage,		"WAPMESSAGE");

_LIT(KSIContentType,    "\"SI\"");
_LIT(KSLContentType,    "\"SL\"");
_LIT(KContentType,      "Content-Type");

_LIT ( KSourcePath, "z:\\ineturilist\\testdata\\ineturilist_tpushscriptbased.xml" );
_LIT ( KDestinationPath, "c:\\private\\20009D70\\ineturilist.xml" );
_LIT ( KOldDB, "c:\\private\\10281e17\\[20009D70]URIList.dat");


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
	iTestUtils = CHTTPTestUtils::NewL(KIniWapTestTitle());
	iTestUtils->InitCommsL();
	
	// Write to log file
	RFileLogger::Write(KPushLogDir, KPushLogFile, EFileLoggingModeAppend, KStartTest);
	RFileLogger::Write(KPushLogDir, KPushLogFile, EFileLoggingModeAppend, KSpace);
	
	iConsole = Console::NewL(KWapPushTestTitle,TSize(KConsFullScreen,KConsFullScreen));
	// set up service BEFORE active scheduler...
	InitialiseMsgServiceL();
	//
	CreateTestArrayL();
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
	delete iTestUtils;
	iTestArray.Close();
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
		iTest=(TTest)(iTestArray[iIndex++]);
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


void CWapPushTestEngine::CreateTestArrayL()
	{
	TInt testType;
	CScriptFile* iniFile = NULL;
	iniFile = CScriptFile::NewL(*iTestUtils, KWapIniFolder(), KWapIni());
	CleanupStack::PushL(iniFile);
		
	// Create an array of sections from the script / ini file.
	CArrayPtrFlat<CScriptSection>& sections=iniFile->Sections();
	
	//create the  tests for iteration2 and  append them in the array
	TInt secCount = sections.Count();
	TInt count = secCount;
	CScriptSection* section = NULL;
		
	for ( TInt iter = 0 ; iter < count ; ++iter)
		{
		//create an array of tests to be sent to the engine
		section = sections[iter];
				
		// TestCase
		if ( section->SectionName().CompareF(KWAPMessage) == 0)
			{
			TInt itemFieldCount= iniFile->Section(iter).Items().Count();
			
			for (TInt fctr=0; fctr < itemFieldCount ; ++fctr)
				{
				//Get the field and value				
				TPtrC itemFieldPtr(iniFile->Section(iter).Item(fctr).Item());
				TPtrC itemValuePtr(iniFile->Section(iter).Item(fctr).Value());
					
				if(itemFieldPtr.Compare(KContentType()) == 0)
					{
					if(itemValuePtr.Compare(KSIContentType) == 0)				
						{
						testType = ESIMessage;
						User::LeaveIfError(iTestArray.Append(testType));
						}
					else if(itemValuePtr.Compare(KSLContentType) == 0)					
						{
						testType = ESLMessage;
						User::LeaveIfError(iTestArray.Append(testType));
						}
					else
						{
						testType = EFinished;
						User::LeaveIfError(iTestArray.Append(testType));	
						}	
					}
				}
			}
		}
	sections.ResetAndDestroy();
	CleanupStack::PopAndDestroy(iniFile);
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
	//TBuf<50> testName;

	if(iIndex < iTestArray.Count())
		{
		switch (iTestArray[iIndex])
			{
		case ESIMessage:
			iCurrentTest = new(ELeave)CWapPushSIMessageTest(iIndex, iTestUtils);
			break;
		case ESLMessage:
			iCurrentTest = new(ELeave)CWapPushSLMessageTest(iIndex, iTestUtils);
			break;
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
	else
		{
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


void DoOldDbCopyL()
	{
	RFs fs;
	User::LeaveIfError ( fs.Connect () );		
	CFileMan* fileMan = CFileMan::NewL ( fs );
	CleanupStack::PushL ( fileMan );
	fs.MkDirAll ( KDestinationPath );		
	fileMan->Attribs ( KDestinationPath, KEntryAttArchive, KEntryAttReadOnly, TTime(0), 0 );
	fileMan->Delete(KDestinationPath);
	User::LeaveIfError ( fileMan->Copy ( KSourcePath, KDestinationPath ) );						
	fileMan->Attribs ( KOldDB, KEntryAttArchive, KEntryAttReadOnly, TTime(0), 0 );
	fileMan->Delete(KOldDB, 0);
	CleanupStack::PopAndDestroy ( fileMan );
	fs.Close ();		
	}
	
	
// main loop
//
GLDEF_C TInt E32Main()
    {
__UHEAP_MARK;
	// Install exception handler
	CTrapCleanup* theCleanup = CTrapCleanup::New(); 

	test.Printf(_L("@SYMTestCaseID IWS-WAPBROWSER-WAPPUSH-TPUSHSCRIPTBASED-T_WAPPUSH-0001  "));	

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

	TRAP(error, DoOldDbCopyL());
	if(error != KErrNone)
		User::Panic(_L("Failed to copy xml file"), error);
	
	CWapPushTestEngine* engine=NULL;
	TRAPD(err,engine = CWapPushTestEngine::NewL());
	if (err != KErrNone)
		User::Panic(_L("Failed to create test engine object"),err);
	
	// All the action happens within the main event loop
	CActiveScheduler::Start();
	// Returned from start so shutdown

	delete engine;
	delete as;
	delete theCleanup;
	test.Close();
	User::Heap().Check();

	REComSession::FinalClose();
	__UHEAP_MARKEND;
	return(KErrNone);
	}



