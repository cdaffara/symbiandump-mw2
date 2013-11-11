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
//

#include "MsvTestUtils.h"
#include "EmailTestUtils.h"
#include "t_testutils.h"
#include <logwrap.h>
#include <logcli.h>

RTest test(_L("t_testutils Testrig"));
CTrapCleanup* theCleanup;

void CreateLogEventsL()
	{
	CTestActive* testActive = new (ELeave) CTestActive();
	CleanupStack::PushL(testActive);

	RFs fs;
	User::LeaveIfError(fs.Connect());

	CLogWrapper* wrap = CLogWrapper::NewL(fs);
	CleanupStack::PushL(wrap);

	CLogEvent* event = CLogEvent::NewL();
	CleanupStack::PushL(event);

	TBuf<256> textBuf;

	wrap->Log().GetString(textBuf, R_LOG_DIR_IN);
	event->SetDirection(textBuf);
	wrap->Log().GetString(textBuf, R_LOG_DEL_SENT);
	event->SetStatus(textBuf);
	event->SetEventType(KLogShortMessageEventTypeUid);
	event->SetSubject(_L("This is my subject"));

	testActive->StartL();
	wrap->Log().AddEvent(*event, testActive->iStatus);
	CActiveScheduler::Start();
	test(testActive->iStatus == KErrNone);

	wrap->Log().GetString(textBuf, R_LOG_DIR_OUT);
	event->SetDirection(textBuf);
	wrap->Log().GetString(textBuf, R_LOG_DEL_FAILED);
	event->SetStatus(textBuf);

	testActive->StartL();
	wrap->Log().AddEvent(*event, testActive->iStatus);
	CActiveScheduler::Start();
	test(testActive->iStatus == KErrNone);

	CleanupStack::PopAndDestroy(3);
	}

LOCAL_C void doMainL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install( scheduler );

	CEmailTestUtils* testUtils = CEmailTestUtils::NewLC(test);
	testUtils->WriteComment(_L("A comment from test utils"));

	CLogFilter* filter = CLogFilter::NewL();
	CleanupStack::PushL(filter);

	test.Next(_L("Display Current Log"));

	testUtils->DisplayLogL(*filter);

	test.Next(_L("Creating Log Events"));
	CreateLogEventsL();

	testUtils->DisplayLogL(*filter);
	CleanupStack::PopAndDestroy(filter);
	
	test.Next(_L("Searching for file"));

	TParse outfile;

	testUtils->Printf(_L("Looking for \"tests\" \"ADir\\AFile.txt\"\n"));
	TInt err=testUtils->ResolveFile(_L("TESTS"),_L("ADir\\AFile.txt"),outfile);
	testUtils->Printf(_L("Found "));
	testUtils->Printf(outfile.FullName());
	testUtils->Printf(_L("\n"));
	testUtils->Printf(_L("Looking for \"tests\" \"ADir\\AFile2.txt\"\n"));
	err=testUtils->ResolveFile(_L("TESTS"),_L("ADir\\AFile2.txt"),outfile);
	testUtils->Printf(_L("Found "));
	testUtils->Printf(outfile.FullName());
	testUtils->Printf(_L("\n"));

	testUtils->Printf(_L("Want to output \"ADir\\AFile2.log\"\n"));
	err=testUtils->ResolveLogFile(_L("ADir\\AFile2.txt"),outfile);
	testUtils->Printf(_L("Found "));
	testUtils->Printf(outfile.FullName());
	testUtils->Printf(_L("\n"));


	test.Printf(_L("Test Complete\n"));
	test.Console()->Getch();

	CleanupStack::PopAndDestroy(testUtils);
	CleanupStack::PopAndDestroy(scheduler);
	}



GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;
	test.Start(_L("Setup"));
	theCleanup=CTrapCleanup::New();
	TRAPD(ret,doMainL());		
	test(ret==KErrNone);
	delete theCleanup;	
	test.Console()->SetPos(0, 13);
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return(KErrNone);
	}



