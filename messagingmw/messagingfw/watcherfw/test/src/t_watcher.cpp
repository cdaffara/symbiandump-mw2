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
// t_watcher.cpp
//

#include <watcher.h>
#include <e32test.h>
#include <bautils.h>
#include "msvtestutils.h"
#include "cwatcher.h"

_LIT(KTestFile, "c:\\TESTWATCHER");
const TInt KWaitDelay = 1000000;
const TInt KWaitCount = 30;

RTest test(_L("Watcher Test Harness"));
class CWatcherTestUtils;
CWatcherTestUtils* theUtils = NULL;

//**********************************
// CWatcherTestUtils
//**********************************

class CWatcherTestUtils : public CTestUtils
	{
public:
	static CWatcherTestUtils* NewL(RTest& aTest);
	CWatcherTestUtils(RTest& aTest);
	void Panic(TInt aPanic);
	//
public:
	void Start(const TDesC& aDes);
	void Complete();
	//
private:
	TInt iTestNum;
	};

CWatcherTestUtils* CWatcherTestUtils::NewL(RTest& aTest)
	{
	CWatcherTestUtils* self = new(ELeave)CWatcherTestUtils(aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

CWatcherTestUtils::CWatcherTestUtils(RTest& aTest)
: CTestUtils(aTest)
	{
	}

void CWatcherTestUtils::Panic(TInt)
	{
	test(EFalse);
	}

void CWatcherTestUtils::Start(const TDesC& aDes)
	{
	if (iTestNum == 0)
		test.Start(aDes);
	else
		test.Next(aDes);

	TestStart(++iTestNum, aDes);
	}

void CWatcherTestUtils::Complete()
	{
	TestFinish(iTestNum, KErrNone);
	}

LOCAL_C TBool WaitForTestFile()
	{
	TInt count = KWaitCount;
	while(count--)
		{
		User::After(KWaitDelay);
		if (BaflUtils::FileExists(theUtils->FileSession(), KTestFile))
			return ETrue;
		}
	return EFalse;
	}

void DoTestsL()
	{
	// Create waiter
	CWatcherWait* wait = CWatcherWait::NewLC();

	// Create the watcher
	CWatcher* watcher = CWatcher::NewL(CActive::EPriorityStandard);
	CleanupStack::PushL(watcher);

	theUtils->Start(_L("Watcher startup"));
	theUtils->FileSession().Delete(KTestFile);
	wait->Start();
	watcher->Start(wait->iStatus);
	test(WaitForTestFile());
	theUtils->Complete();

	theUtils->Start(_L("Watcher death"));
	watcher->Cancel();
	CActiveScheduler::Start();
	test(wait->iStatus.Int() == KErrCancel);
	theUtils->Complete();

	theUtils->Start(_L("Watcher restart"));
	test(theUtils->FileSession().Delete(KTestFile) == KErrNone);
	wait->Start();
	watcher->Start(wait->iStatus);
	test(WaitForTestFile());
	theUtils->Complete();

	// Close the watcher
	theUtils->Start(_L("Closing watchers"));
	watcher->Cancel();
	CActiveScheduler::Start();
	test(wait->iStatus.Int() == KErrCancel);
	theUtils->Complete();

	CleanupStack::PopAndDestroy(2); // watcher, wait
	}

void SetupL()
	{
	CActiveScheduler::Install(new(ELeave)CActiveScheduler);
	theUtils = CWatcherTestUtils::NewL(test);
	}

void Close(TInt aRet)
	{
	if (theUtils)
		{
		if (!aRet)
			theUtils->TestHarnessCompleted();
		else
			theUtils->TestHarnessFailed(aRet);
		}
	test(aRet == KErrNone);

	delete theUtils;
	delete CActiveScheduler::Current();
	}

void DoMainL()
	{
	SetupL();
	DoTestsL();
	}

GLDEF_C TInt E32Main()
	{       
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TRAPD(ret, DoMainL());
	Close(ret);
	test.Close();
	delete cleanup;      
	__UHEAP_MARKEND;
	return(KErrNone);
	}

EXPORT_C TInt WinsMain()
	{
#if defined(__WINS__)
	E32Main();
#endif
	return KErrNone;
	}

