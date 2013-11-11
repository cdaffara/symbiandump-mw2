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
// Name of test harness: T_EMAIL
// Owner: EH
// Brief description of test harness:
// Sends mail via SMTP and retrieves mail via POP3.
// Detailed description of test harness:
// As above
// Input files required to run test harness:
// None
// Intermediate files produced while running test harness:
// None
// Output files produced by running test harness:
// <DRIVE>:\msglogs\T_EMAIL.<PLATFORM>.<VARIANT>.LOG.txt
// Description of how to build test harness:
// cd \msg\test\smoke\email
// bldmake bldfiles
// abld build
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
// WINS running instructions:
// 1. \epoc32\release\wins\<VARIANT>\T_EMAIL.exe can be used at the command prompt
// or executable can be run from Windows Explorer.
// All other platform running instructions:
// 1. Copy \epoc32\release\<PLATFORM>\<VARIANT>\T_EMAIL.exe onto the other platform
// 2. Copy \epoc32\release\<PLATFORM>\<VARIANT>\MSVTESTUTILS.DLL into 
// <DRIVE>:\system\libs on the other platform
// 3. Copy \epoc32\release\<PLATFORM>\<VARIANT>\EMAILTESTUTILS.DLL into 
// <DRIVE>:\system\libs on the other platform
// 4. Run T_EMAIL.exe on the other platform
// 
//


#include "emailsmoke.h"
#include "emailsmokeparse.h"
#include <bacline.h>


// For loading the serial comms device drivers 
#if defined (__WINS__)
	#define PDD_NAME		_L("ECDRV")
	#define LDD_NAME		_L("ECOMM")
#else
	#define PDD_NAME		_L("EUART1")
	#define LDD_NAME		_L("ECOMM")
#endif

_LIT(KTestParams,"-testparams");
_LIT(KNewLogFile,"newlog");
enum TCmdLineArgs
	{
	EArgProgramName,
	EArgTestParams,
	EArgScriptFile,
	EArgNewLogFile,
	EArgEnd
	};

// this is used if we can't find MsgLogs anywhere
_LIT(KMsvPathSep,"\\");
_LIT(KMsvTestFileInputBase,"MsgTest");

TInt ResolveFile(RFs& aFs, const TDesC& aComponent, const TDesC& aFileName, TParse& aParseOut)
	{
	TFileName* savedPath = new TFileName;
	TFileName* fileName = new TFileName;
	if ((savedPath == NULL) || (fileName == NULL))
		return KErrNoMemory;

	fileName->Append(KMsvPathSep);
	fileName->Append(KMsvTestFileInputBase);
	fileName->Append(KMsvPathSep);
	fileName->Append(aComponent);
	fileName->Append(KMsvPathSep);
	fileName->Append(aFileName);
	
	// file finder will look in the session drive first, then Y->A,Z
	// so set session drive to Y (save old and restore it afterwards)
	aFs.SessionPath(*savedPath);
	_LIT(KTopDrive,"Y:\\");
	aFs.SetSessionPath(KTopDrive);
    TFindFile file_finder(aFs);
    TInt err = file_finder.FindByDir(*fileName,KNullDesC);
	if(err==KErrNone)
		aParseOut.Set(file_finder.File(),NULL,NULL);
	aFs.SetSessionPath(*savedPath);
	delete savedPath;
	delete fileName;
	return(err);
	}

//
//
// Main Test Harness
//

LOCAL_D CTrapCleanup* theCleanup;

void doMainL()
	{
	RTest test(_L("Email smoke tests"));
	RFs fs;
	fs.Connect();

	// Load the serial comms device drivers.  If this is not done,
	// connecting via NT-RAS returns KErrNotFound (-1).
	TInt driverErr;
	driverErr=User::LoadPhysicalDevice(PDD_NAME);
	if (driverErr!=KErrNone && driverErr!=KErrAlreadyExists)
		User::Leave(driverErr);
	driverErr=User::LoadLogicalDevice(LDD_NAME);
	if (driverErr!=KErrNone && driverErr!=KErrAlreadyExists)
		User::Leave(driverErr);


	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(scheduler);
	CleanupStack::PushL(scheduler);

	// Handle command line arguments
	CCommandLineArguments* cmdLineArg=CCommandLineArguments::NewLC();
	TInt count = cmdLineArg->Count();
	TBool isCmdLine=FALSE;
	TBool keepLogFile=TRUE;
	if (count>2)	// Command line arguments?
		{
		if ((!(cmdLineArg->Arg(EArgTestParams).Compare(KTestParams))) && (count==EArgNewLogFile || count==EArgEnd))
			{
			isCmdLine=TRUE;
			if (count==EArgEnd)
				if (!(cmdLineArg->Arg(EArgNewLogFile).Compare(KNewLogFile)))
					keepLogFile=FALSE;
			}
		}	
	CTestScript* script = CTestScript::NewL(fs);
	CleanupStack::PushL(script);
	TParse scriptFileName;

	TInt err;
	TBuf<100> buf;
	if (isCmdLine)
		{
		TLex lex(cmdLineArg->Arg(EArgScriptFile));
		buf=lex.NextToken();
		test.Printf(_L("Script file: %S\n"),&buf);
		}
	else
		buf=_L("script.txt");

	CEmailTestUtils* testUtils = CEmailTestUtils::NewL(test,keepLogFile? ETuKeepLogFile:0);
	CleanupStack::PushL(testUtils);
	
	err = ResolveFile(fs, _L("emailsmoke"), buf, scriptFileName);
	if (err != KErrNone)
		{
		if (keepLogFile)
			testUtils->WriteComment(KTestHeader);
		testUtils->Printf(_L("Could not find script file:%S"),&buf);
		}
	script->LoadFileL(scriptFileName.FullName());

	CMainTestHarness* mainTest = CMainTestHarness::NewL(test);
	CleanupStack::PushL(mainTest);
	TTestDebugInfo debugInfo(*script, 0, 0);
	mainTest->SetDebugInfo(debugInfo);

	CMainSectionParser* mainSectionParser = CMainSectionParser::NewL(*mainTest, *script, keepLogFile);
	CleanupStack::PushL(mainSectionParser);

	if (err == KErrNone)
		{
		TRAP(err, mainSectionParser->ParseL());
		}

	if (err == KErrNone)
		{
		TInt pos=0;
		CTestActive* waiter = new (ELeave) CTestActive;
		CleanupStack::PushL(waiter);
		waiter->StartL();
		mainTest->StartL(waiter->iStatus);
		CActiveScheduler::Start();
		TInt status = waiter->iStatus.Int();
		CleanupStack::PopAndDestroy(waiter);
		testUtils->SeekLogFile(ESeekEnd,pos);
		testUtils->Printf(_L("SCRIPT FILE:%S"),&buf);
		if (status==KErrNone)
			testUtils->TestHarnessCompleted();
		else
			testUtils->TestHarnessFailed(status);
		}

	CleanupStack::PopAndDestroy(4,script);
	CleanupStack::PopAndDestroy();	// cmdLineArg
	CleanupStack::PopAndDestroy(scheduler);

	fs.Close();
	test.Close();
	}

GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;
	theCleanup=CTrapCleanup::New();

	TRAPD(ret,doMainL());		

	delete theCleanup;	
//	test.End();
//	test.Close();
	__UHEAP_MARKEND;
	User::Heap().Check();
	return(KErrNone);
	}

