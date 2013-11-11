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
// This utility converts any email logfiles ( imlog*.txt ) to script
// files ( imsk*.scr ).
// It strips out the comments, date stamps and '<<' '>>' markers.
// 
//

#include "emailtestutils.h"
#include "scripttestutils.h"
#include <miutlog.h>
#include <miutmsg.h>
#include <txtrich.h>

// local variables etc //

_LIT(KImcmTest, "U_IMUT2 - Utility for stripping down logfiles for use as scripts");
RTest test(KImcmTest);

LOCAL_D CTrapCleanup* theCleanup;
LOCAL_D CImLog* log;
LOCAL_D CEmailTestUtils* testUtils;
LOCAL_D	CActiveScheduler* scheduler;
	

//

LOCAL_C void Init()
	{
	scheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(scheduler);
	CleanupStack::PushL(scheduler);
	
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));

	log = CImLog::NewL(_L("c:\\logs\\email\\u_imut2.log"), EAppend);
	CleanupStack::PushL(log);
	log->AppendComment(_L8("*** U_IMUT2 Utility ***"));
	TBuf8<80> buf;
#if defined(__WINS__)
	buf.Append(_L8("WINS "));
#else
	buf.Append(_L8("MARM "));
#endif
#if defined(_UNICODE)
	buf.Append(_L8("U"));
#endif
#if defined(_DEBUG)
	buf.Append(_L8("DEB"));
#else
	buf.Append(_L8("REL"));
#endif
	log->AppendComment(buf);
	}
	
LOCAL_C void Closedown()
	{
	log->AppendComment(_L8("**********    U_IMUT2 Complete    **********"));
	log->AppendComment(_L8(""));

	CleanupStack::PopAndDestroy(3);  //testUtils, log, scheduler
	}

//

enum TImStripLogType
	{
	EImMakeInFile,
	EImMakeOutFile,
	EImEndMakeFile
	};

LOCAL_C void CreateScriptFilesL()
	{
	RFs& fs = testUtils->FileSession();
	fs.SetSessionPath(_L("c:\\logs\\email\\"));

	CScriptTestUtils* scriptTestUtils = CScriptTestUtils::NewLC(*testUtils);

	CDir* logFiles;
	TInt portStringCounter;
	RFile logFile;
	RFile scriptFile;
	TBufC<KMaxFileName> logFileName;
	TBuf<KMaxFileName> scriptFileName;

	// First create the script file, then create the output test file.

	TImStripLogType lineType = EImMakeInFile;
	while ((lineType == EImMakeInFile)
			|| (lineType == EImMakeOutFile))
		{

		User::LeaveIfError(fs.GetDir(_L("imlog*.txt"), 0, ESortNone, logFiles));
		CleanupStack::PushL(logFiles);

		TInt logFileCounter = logFiles->Count();

		while (logFileCounter > 0)
			// Generate the script file from each log file
			{
			// Get the name of the log file.
			logFileCounter--;
			logFileName = ((*logFiles)[logFileCounter]).iName;

			// Get the name of the destination script file.
			scriptFileName = _L("imsk");
			portStringCounter = 5;

			while (logFileName[portStringCounter] != L'.')
				{
				scriptFileName.Append(logFileName[portStringCounter]);
				portStringCounter++;
				}

		if (EImMakeInFile == lineType)
			{
			scriptFileName.Append(_L(".scr"));
			}
		else if (EImMakeOutFile == lineType)
			{
			scriptFileName.Append(_L(".out"));
			}

		if (lineType == EImMakeInFile)
			scriptTestUtils->LogToInputFileL(logFileName, scriptFileName, EFalse);
		else
			scriptTestUtils->LogToOutputFileL(logFileName, scriptFileName, ETrue);

			// Close the files
			logFile.Close();
			scriptFile.Close();
			}

		CleanupStack::PopAndDestroy(); // logFiles

		// If we have just created the script file the create the output test file.
		if (EImMakeInFile == lineType)
			{
			lineType = EImMakeOutFile;
			}
		else
			{
			lineType = EImEndMakeFile;
			}
		}

	CleanupStack::PopAndDestroy(); // scriptTestUtils
	}

//
LOCAL_C void doMainL()
	{
	Init();

	CreateScriptFilesL();

	Closedown();
	}

GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;
	test.Start(_L("U_IMUT2 Log -> Script Utility"));
	theCleanup=CTrapCleanup::New();
	TRAPD(ret,doMainL());		
	test(ret==KErrNone);
	delete theCleanup;	
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	User::Heap().Check();
	return(KErrNone);
	}

