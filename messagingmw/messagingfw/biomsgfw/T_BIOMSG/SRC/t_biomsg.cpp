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
//

/**
 * @file
 * Copyright (c) 2003 Symbian Ltd.  All rights reserved. 
 * 
 * Name of test harness: T_BIO_S
 * 
 * Owner: GKL
 * 
 */

#include <e32base.h>
#include <e32test.h>
#include <f32file.h>
#include <bacline.h>

#include "biotestutils.h"
#include "testframeutils.h"
#include "harness.h"
#include "parsers.h"
#include "msvtestutilsbase.h"

#ifdef SYMBIAN_BOOKMARK_DATABASE
#include <bookmarkdatabase.h>
#include <bookmark.h>
#endif // SYMBIAN_BOOKMARK_DATABASE

// Name of test harness.
_LIT(KBioTestName, "BIO Msg Tests");
_LIT(KBioTestExeName, "T_BIOMSG");

// Name of the main script file.
_LIT(KScriptFileName, "c:\\msgtest\\biomsg\\scripts\\script.txt");

// Where the test harness places the log file.
_LIT(KMsvMsgLogsSrcPath, "c:\\MsgLogs\\");


RFs g_rfs; 

void MoveLogsL(const TDesC& aTestScript)
	{
	//	Move the test log directory and file.
	//
	//	For example: test script "c:\msgtest\biomsg\Wapp\wapp0001.txt",
	//	under a WINS UDEB build, test harness name T_BIOMSG.
	//
	//	dir src = c:\MsgLogs\T_BIOMSG
	//	dir dst = c:\MsgLogs\T_BIOMSG_wapp001
	//
	//	file src = c:\MsgLogs\T_BIOMSG.WINS.DEB.LOG
	//	file dst = c:\MsgLogs\T_BIOMSG_wapp001.WINS.DEB.LOG

	CFileMan*	fileMan = CFileMan::NewL(g_rfs);
	CleanupStack::PushL(fileMan);

	//
	// Determine dir name for src.
	TFileName	msgLogsSrc;

	msgLogsSrc.Append(KMsvMsgLogsSrcPath);	// Add "c:\MsgLogs\".
	msgLogsSrc.Append(KBioTestExeName);		// Add "T_BIOMSG".

	//
	// Determine dir name for dst.
	TFileName	msgLogsDst;
	TInt		fileErr;
	TInt		loc;
	TInt		len;

	msgLogsDst = msgLogsSrc;	// Add "c:\MsgLogs\T_BIOMSG".
	msgLogsDst.Append('_');		// Add "_".

	// Add test name.
	len = aTestScript.Length();
	loc = aTestScript.LocateReverse('\\');
	msgLogsDst.Append(aTestScript.Right(len - loc - 1));

	// Remove the ".ext".
	msgLogsDst = msgLogsDst.Left(msgLogsDst.Locate('.'));

	//
	// Move the dir.
	fileErr = fileMan->Move(msgLogsSrc, msgLogsDst);

	// Move will not delete existing directories, so we have to do it ourself.
	// The documentation says it will delete directories !!!!
	if (fileErr == KErrAlreadyExists)
		{
		msgLogsDst.Append('\\');								// To ensure we delete the correct directory.
		fileErr = fileMan->RmDir(msgLogsDst);
		msgLogsDst = msgLogsDst.Left(msgLogsDst.Length() - 1);	// Remove '\' again.
		if (fileErr == KErrNone)
			fileErr = fileMan->Move(msgLogsSrc, msgLogsDst);
		}

	//
	// Now move the file. Append ".WINS.DEB.LOG" to src and dst.
	CBioTestUtils::DoAppendVariantName(msgLogsSrc);
	CBioTestUtils::DoAppendVariantName(msgLogsDst);

	fileErr = fileMan->Move(msgLogsSrc, msgLogsDst);

	CleanupStack::PopAndDestroy(fileMan);
	}


TInt RunTestL(RTest& aTest, const TDesC& aTestScript)
	{
	TInt err = KErrNone;

#ifdef SYMBIAN_BOOKMARK_DATABASE
	// Reset the bookmark database by deleting all bookmarks in root...
	RBkDatabase bookmarkDb;
	bookmarkDb.OpenL();
	CleanupClosePushL(bookmarkDb);
	RBkFolder rootFolder = bookmarkDb.OpenRootL();
	CleanupClosePushL(rootFolder);
	RBkNode bookmark;
	const TInt bookmarkCount = rootFolder.Count();
	for( TInt ii=0; ii<bookmarkCount; ++ii )
		{
		bookmark = rootFolder.OpenItemL(0);
		CleanupClosePushL(bookmark);
		if( bookmark.Type() == Bookmark::ETypeBookmark )
			{
			Bookmark::TItemId bookmarkId = bookmark.Id();
			CleanupStack::PopAndDestroy(&bookmark);
			TRAP_IGNORE(bookmarkDb.DeleteItemL(bookmarkId)); // ignore errors
			}
		else
			{
			CleanupStack::PopAndDestroy(&bookmark);
			}
		}
	bookmarkDb.CommitL();
	CleanupStack::PopAndDestroy(2, &bookmarkDb);	// rootFolder, bookmarkDb
#endif // SYMBIAN_BOOKMARK_DATABASE

	// Create the test script object, which will use the test script
	CTestScript* script = CTestScript::NewL(g_rfs);
	CleanupStack::PushL(script);

	script->LoadFileL(aTestScript);

	// Create the main test harness, to which all test states will
	// be appended.
	CMainTestHarness* mainTest = CMainTestHarness::NewL(aTest);
	CleanupStack::PushL(mainTest);
	
	TTestDebugInfo debugInfo(*script, 0, 0);
	
	mainTest->SetDebugInfo(debugInfo);

	// Create a new section parser.
	CBioMainSectionParser* mainSectionParser = CBioMainSectionParser::NewL(*mainTest, *script);
	CleanupStack::PushL(mainSectionParser);

	// Attempt to parse the script
	TRAP(err, mainSectionParser->ParseL());
	if (err == KErrNone)
		{
		// If we've parsed the script OK, attempt to run the test.
		// The "waiter" object is provided just in case we don't
		// want to start and stop the active scheduler.
		CTestActive* waiter = new (ELeave) CTestActive;
		CleanupStack::PushL(waiter);

		waiter->StartL();
		mainTest->StartL(waiter->iStatus);
		CActiveScheduler::Start();
		err = waiter->iStatus.Int();
		
		CleanupStack::PopAndDestroy(waiter);
		}

	CleanupStack::PopAndDestroy(mainSectionParser);
	CleanupStack::PopAndDestroy(mainTest);
	CleanupStack::PopAndDestroy(script);

	return err;
	}


#ifndef CENREP_SOLID_AS_A_ROCK
void doCleanEnvironmentL(RTest& aTest)
	{

	// central repository cleanup
	CFileMan* fman = CFileMan::NewL(g_rfs);
	CleanupStack::PushL(fman);
	fman->Delete(_L("c:\\private\\10202be9\\*.*"));
	CleanupStack::PopAndDestroy(fman);
				
	// clean message store
	CBioTestUtils* utils = CBioTestUtils::NewL(aTest);
	CleanupStack::PushL(utils);
	TRAPD(ignor, utils->CleanMessageFolderL());
	CleanupStack::PopAndDestroy(utils);
	}
#endif


TInt doMainL()
	{
	TInt err = KErrNone;
	
	// Create the test object which we'll use for output etc.
	RTest test(KBioTestName);

	g_rfs.Connect();

	// Start a new active scheduler.
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(scheduler);
	CleanupStack::PushL(scheduler);

	// Name of the actual script being executed.
	HBufC*	scriptFileName = HBufC::NewLC(KMaxFileName);
	TPtr	strPtr = scriptFileName->Des();

	_LIT(KInputScriptParameterId, "-i");
	_LIT(KScriptNotFoundErr, "%S not found!\n\n");

	// Handle command line arguments
	CCommandLineArguments*	args = CCommandLineArguments::NewLC();
	TInt					argCount = args->Count();
	TInt					currArg = 0;

	// Search for "-i" parameter.
	while ((currArg < argCount) && args->Arg(currArg).Compare(KInputScriptParameterId))
		{
		currArg++;
		}
	
	// Input script filename argument NOT present
	//
	// Run multiple test scripts, held in KScriptFileName.
	if ((currArg == argCount) || (currArg == (argCount - 1)))
		{
		CleanupStack::PopAndDestroy(args); // Done with arguments.
		
		// Load the main script file, used to determine which actual test scripts are run.
		CTestScript*	mainScript = CTestScript::NewL(g_rfs);
		CleanupStack::PushL(mainScript);

		if (mainScript->LoadFileL(KScriptFileName))
			{
			CTestScript::TTestScriptString		line;
			TBool								lineGot = false;
			TInt								linePos = 0;


			// For each valid line in the main script file, run the test.
			lineGot = mainScript->GetLineL(line, linePos);
			while (lineGot)
				{
#ifndef CENREP_SOLID_AS_A_ROCK
				// MLM added to handle KErrCorrupts from CenRep !!!
				doCleanEnvironmentL(test);
#endif
				
				if (line.Length() > 0)
					{
					TInt	fileErr;
					TBool	dummy = TRUE;


					// Determine the test script file name and path.
					strPtr.Copy(line);
					strPtr.Insert(0, KScriptFileName().Left(KScriptFileName().LocateReverse('\\') + 1)); // inserts path

					// Ensure file exsits.
					fileErr = g_rfs.IsFileOpen(scriptFileName->Des(), dummy);
					if (fileErr == KErrNone)
						{
						// Start the test.
						err = RunTestL(test, scriptFileName->Des());

						// Move the output log.
						MoveLogsL(scriptFileName->Des());
						}
					}

				else
					{
					test.Printf(KScriptNotFoundErr, scriptFileName);
					}

				linePos += line.Length() + 2;	// 2 for cr/lf pair.
				lineGot = mainScript->GetLineL(line, linePos);
				}
			}

		else
			{
			test.Printf(KScriptNotFoundErr, &KScriptFileName);
			User::After(5000000);
			}
	
		CleanupStack::PopAndDestroy(mainScript);
		}
	
	// Run single test script.
	else
		{
#ifndef CENREP_SOLID_AS_A_ROCK
		// MLM added to handle KErrCorrupts from CenRep !!!
		doCleanEnvironmentL(test);
#endif
		
		TInt	fileErr;
		TBool	dummy = TRUE;

		// Determine the test script file name and path.
		strPtr.Copy(args->Arg(currArg + 1));
		strPtr.Insert(0, KScriptFileName().Left(KScriptFileName().LocateReverse('\\') + 1)); // inserts path
		
		CleanupStack::PopAndDestroy(args);	// Done with arguments.
		
		// Ensure file exsits.
		fileErr = g_rfs.IsFileOpen(scriptFileName->Des(), dummy);
		if (fileErr == KErrNone)
			{
			// Start the test.
			err = RunTestL(test, scriptFileName->Des());

			// Move the output log.
			MoveLogsL(scriptFileName->Des());
			}
		else
			{
			test.Printf(KScriptNotFoundErr, scriptFileName);
			User::After(5000000);
			}
		}
	CleanupStack::PopAndDestroy(scriptFileName);
	CleanupStack::PopAndDestroy(scheduler);

	g_rfs.Close();
	test.Close();

	return err;
	}


LOCAL_D CTrapCleanup* theCleanup;

GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;
	theCleanup=CTrapCleanup::New();

	TInt status = KErrNone;

	TRAPD(ret, status = doMainL());		

	delete theCleanup;	
	__UHEAP_MARKEND;
	User::Heap().Check();
	return status;
	}
