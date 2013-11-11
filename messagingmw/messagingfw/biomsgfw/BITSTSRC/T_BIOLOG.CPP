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
// Test harness for CMsvBIOTestLogger, a class which is used to log 
// results of other test harnesses. 
// 
//



#pragma warning( disable : 4100 )

//  Includes...
#include <e32test.h>
#include <biologgr.h>


//  Global functions..
//  functions()
GLDEF_C void doMainL();
GLDEF_C TInt E32Main();
GLDEF_C void doTestEventsL(CMsvBIOTestLogger& aLogger);
GLDEF_C void doTestErrorsL(CMsvBIOTestLogger& aLogger);
GLDEF_C void doTestCommentingL(CMsvBIOTestLogger& aLogger);
GLDEF_C void doTestOutputL(CMsvBIOTestLogger& aLogger);


//  Resources..
GLDEF_C RTest test(_L("BIO Test Logger Tester"));
LOCAL_D CTrapCleanup*       myCleanup;
// LOCAL_D CTestScheduler*              theScheduler;
LOCAL_D RFs                 rFs;


//*****************************************************************************
//
//  Implementation; global stuff
//
//*****************************************************************************
GLDEF_C void doTestEventsL(CMsvBIOTestLogger& aLogger)
    {
	test.Printf(_L("Testing the events logging\n"));
	aLogger.LogEventL(EBIOParserCreate,_L("Made a parser"));
	aLogger.LogEventL(EBIOParserDelete,_L("Got rid of a parser"));
	aLogger.LogEventL(EBIOParserParseMessage,_L("Parsing my Smart Message"));
	aLogger.LogEventL(EBIOParserProcessMessage,_L("Processing message to create settings"));
	aLogger.LogEventL(EOpenBIOMessage,_L("Opening a new message"));
	aLogger.LogEventL(ECloseBIOMessage,_L("Closing this message"));
	aLogger.LogEventL(ECreateBIOAttachment,_L("Creating a new attachment file"));
	aLogger.LogEventL(ECloseBIOAttachment,_L("Closing the attachment file"));
	aLogger.LogEventL(ECreateBIODataStream,_L("Creating a data stream to store the parsed data"));
	aLogger.LogEventL(ECloseBIODataStream,_L("Closing the data stream"));
	test.Printf(_L("Completed test of event logging\n"));

	}


GLDEF_C void doTestErrorsL(CMsvBIOTestLogger& aLogger)
    {
	test.Printf(_L("Testing the error logging\n"));
	aLogger.LogErrorL(0,_L("No problemo"));
	aLogger.LogErrorL(1,_L("Positive value"));
	aLogger.LogErrorL(-1,_L("Negative value - error"));
	aLogger.LogErrorL(KErrNone,_L("KErrNone received"));
	aLogger.LogErrorL(KErrNotFound,_L("Could find it "));
	aLogger.LogErrorL(KErrPathNotFound,_L("Could find the path"));
	aLogger.LogErrorL(KErrNoMemory,_L("Out of memory"));
	
	test.Printf(_L("Completed test of error  logging\n"));

	}

GLDEF_C void doTestCommentingL(CMsvBIOTestLogger& aLogger)
    {
	test.Printf(_L("Testing the comment logging\n"));
	aLogger.LogCommentL(_L("A"));
	aLogger.LogCommentL(_L("Smallish comment"));
	aLogger.LogCommentL(_L("An exceedingly large, even humongous comment, which stretches the limits of the descriptors to their maximums, to ensure that the class is capable of holding the maximum likely size of a description. This should be more than sufficient for the task but you never can tell when therell be a problem so you're best testing every thing"));
	test.Printf(_L("Completed the testing of the comment logging\n"));
	}

GLDEF_C void doTestOutputL(CMsvBIOTestLogger& aLogger)
    {
	test.Printf(_L("Testing the output logging\n"));
	aLogger.LogOutputL(_L("Some output text"));
	test.Printf(_L("Completed testing output logging\n"));
	}

GLDEF_C TInt E32Main()
    {
	__UHEAP_MARK;
	myCleanup = CTrapCleanup::New();
	TRAPD(err,doMainL());           
	test(err==KErrNone);
	delete myCleanup;       
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return(KErrNone);
    }

GLDEF_C void doMainL()
    {
	// Create an active scheduler for the program session
    //theScheduler = new (ELeave) CTestScheduler();
    //CleanupStack::PushL(theScheduler);
    //CActiveScheduler::Install(theScheduler);
    test.Title();
    test.Printf(_L("\n"));
    test.Start(_L("Starting logger test harness"));
	rFs.Connect();
	TInt portAddr = 5000;
	test.Printf(_L("Creating a logger via port address 5000"));
	// Create a logger using a port address 
	CMsvBIOTestLogger* myLogger = CMsvBIOTestLogger::NewL(rFs, portAddr);
	CleanupStack::PushL(myLogger);
	doTestEventsL(*myLogger);
	doTestErrorsL(*myLogger);
	doTestCommentingL(*myLogger);
	doTestOutputL(*myLogger);
	CleanupStack::PopAndDestroy(); //myLogger
	test.Printf(_L("Destroying current logger"));
	// Create a logger by passing a filename.  Replace the existing file.
	test.Printf(_L("Creating a new logger using file name c:\\logs\\Bio\\iap.txt"));
	myLogger = CMsvBIOTestLogger::NewL(rFs, _L("C:\\logs\\Bio\\iap.txt"),EReplace);
	CleanupStack::PushL(myLogger);
	doTestEventsL(*myLogger);
	doTestErrorsL(*myLogger);
	doTestCommentingL(*myLogger);
	doTestOutputL(*myLogger);
	test.Printf(_L("Destroying current logger"));
	test.Printf(_L("Logging tests completed"));
	CleanupStack::PopAndDestroy(); //myLogger

	// Create a logger by passing a filename.  Append to the existing file.
	myLogger = CMsvBIOTestLogger::NewL(rFs, _L("C:\\logs\\Bio\\wap.txt"),EAppend);
	CleanupStack::PushL(myLogger);
	doTestEventsL(*myLogger);
	doTestErrorsL(*myLogger);
	doTestCommentingL(*myLogger);
	doTestOutputL(*myLogger);
	test.Printf(_L("Destroying current logger"));
	test.Printf(_L("Logging tests completed"));
	CleanupStack::PopAndDestroy(); //myLogger
    rFs.Close();  
    }

