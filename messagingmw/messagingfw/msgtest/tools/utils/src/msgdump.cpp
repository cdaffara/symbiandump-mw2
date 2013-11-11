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

#include "emailtestutils.h"
#include <miutlog.h>
#include <miutmsg.h>
#include <txtrich.h>


// local variables etc //

_LIT(KImcmTest, "U_IMUT - Utility for dumping the message tree");
RTest test(KImcmTest);

LOCAL_D CTrapCleanup* theCleanup;
//LOCAL_D CActiveScheduler* scheduler;
LOCAL_D CImLog* log;
LOCAL_D CEmailTestUtils* testUtils;

//

LOCAL_C void Init()
	{
	testUtils = CEmailTestUtils::NewLC(test);
	testUtils->CreateAllTestDirectories();
	testUtils->FileSession().SetSessionPath(_L("C:\\"));
	testUtils->GoClientSideL();

	log = CImLog::NewL(_L("c:\\logs\\email\\u_imut.log"), EAppend);
	CleanupStack::PushL(log);
	log->AppendComment(_L8("*** U_IMUT Utility ***"));
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
	log->AppendComment(_L8("**********    U_IMUT Complete    **********"));
	log->AppendComment(_L8(""));

	CleanupStack::PopAndDestroy(2);  //testUtils, log
	}

//

LOCAL_C void doMainL()
	{
	CActiveScheduler* scheduler;
	scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	Init();

	testUtils->FileSession().SetSessionPath(_L("c:\\"));

	testUtils->FindChildrenL(0x01000, ETrue);

	Closedown();

	CleanupStack::PopAndDestroy(); // scheduler
	}

GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;
	test.Start(_L("T_IMCM03 Test CImStoreMessagePart/CImRemoveMessagePart classes"));
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
