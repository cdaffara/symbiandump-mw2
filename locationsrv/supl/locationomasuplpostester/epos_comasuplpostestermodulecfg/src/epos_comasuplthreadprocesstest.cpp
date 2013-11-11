/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Tests for thread creation & Process priority.
*
*/

#include <stiflogger.h>
#include "epos_comasuplthreadprocesstest.h"
#include "epos_comasuplpostesterlogger.h"

/** Constant used for logging name of test*/
_LIT8(KThreadTest, "---------------Thread and Process Test---------------");

/** Constant used for logging name of test*/
_LIT8(KThreadTestComplete, "Info: Thread and Process Test complete.");

_LIT8(KThreadsNotKilled, "Error: Number of threads not killed: ");

_LIT8(KExtraThreadsKilled, "Error: Number of extra threads killed: ");

_LIT8(KSameThreadCount, "Info: Number of threads created = \
Number of threads killed.");

_LIT8(KPriorityChanged, "Error: Priority of the process is changed.");

_LIT8(KPriorityNotChanged, "Info: Priority of the process is not changed.");	

// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
COMASuplThreadProcessTest* COMASuplThreadProcessTest::NewL(
										COMASuplPosTesterCategory* aCallBack )
	{
	COMASuplThreadProcessTest* self = 
							new(ELeave)COMASuplThreadProcessTest(aCallBack);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplThreadProcessTest::~COMASuplThreadProcessTest()
	{
	
	}

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
COMASuplThreadProcessTest::COMASuplThreadProcessTest(
							COMASuplPosTesterCategory* aCallBack )
							: COMASuplPosTesterCategory( aCallBack )
	{
	
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplThreadProcessTest::ConstructL()
	{
	
	}

// ---------------------------------------------------------------------------
// This function is called by test handler. It is entry point of execution
// of a test case.
// ---------------------------------------------------------------------------
//
void COMASuplThreadProcessTest::StartTestingL()
	{
	}

// ---------------------------------------------------------------------------
// Handles an active object's request completion event.
// ---------------------------------------------------------------------------
//
void COMASuplThreadProcessTest::RunL()
	{
	}

// ---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void COMASuplThreadProcessTest::DoCancel()
	{
	
	}

// ---------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL().
// ---------------------------------------------------------------------------
//
TInt COMASuplThreadProcessTest::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}


// ---------------------------------------------------------------------------
// Gets number of threads, name of process and process priority.
// ---------------------------------------------------------------------------
//
void COMASuplThreadProcessTest::ReadStatusL(TInt& aThreadCount
											, TProcessPriority& aPriority
											, TFullName& aProcessName)
	{
	RProcess process;
	CleanupClosePushL(process);
	aProcessName = process.FullName();
	CountThreadsL(aProcessName, aThreadCount);
	aPriority = process.Priority();
	CleanupStack::PopAndDestroy(&process);// removing process
	}

// ---------------------------------------------------------------------------
// Counts number of threads associated with a process.
// ---------------------------------------------------------------------------
//
void COMASuplThreadProcessTest::CountThreadsL(const TFullName& aProcessName
											, TInt& aThreadCount)
	{
	aThreadCount = 0;
	HBufC* searchString = NULL;
	TFullName threadName;
	PrepareSearchStringL(searchString, aProcessName);
	TFindThread findThread(*searchString);
	
	while(KErrNone == findThread.Next(threadName))
		{
		aThreadCount++;
		}
	delete searchString;
	}

// ---------------------------------------------------------------------------
// Prepares a search string from the name of process.
// ---------------------------------------------------------------------------
//
void COMASuplThreadProcessTest::PrepareSearchStringL(HBufC*& aSearchString
								, const TFullName& aProcessName)
	{
	delete aSearchString;
	aSearchString = HBufC::NewL(aProcessName.Length() + 2);
	TPtr stringPtr = aSearchString->Des();
	_LIT(KStar, "*");
	stringPtr.Copy(KStar);
	stringPtr.Append(aProcessName);
	stringPtr.Append(KStar);
	}

// ---------------------------------------------------------------------------
// Reads the number of threads, process priority & name of process 
// before executing tests.
// ---------------------------------------------------------------------------
//
void COMASuplThreadProcessTest::StatusBeforeTestsL()
	{
	ReadStatusL(iThreadCountBeforeTests, iProcessPriorityBeforeTests
				, iProcessNameBeforeTests);
	}
// ---------------------------------------------------------------------------
// Reads the number of threads, process priority & name of process 
// after executing tests.
// ---------------------------------------------------------------------------
//
void COMASuplThreadProcessTest::StatusAfterTestsL()
	{
	ReadStatusL(iThreadCountAfterTests, iProcessPriorityAfterTests
				, iProcessNameAfterTests);
	}

// ---------------------------------------------------------------------------
// Matches the number of threads, process priority & name of process 
// obtained before and after executing tests and logd the info.
// ---------------------------------------------------------------------------
//
TBool COMASuplThreadProcessTest::MatchAndLogStatus()
	{
	iLogger->WriteTestHeader(KThreadTest, iTestNo);
	TBool result = ETrue;
	if(iThreadCountAfterTests > iThreadCountBeforeTests)
		{
		iError++;
		iLogger->WriteStatus(KThreadsNotKilled,
				 iThreadCountAfterTests - iThreadCountBeforeTests
				 								, iTestNo, &KNULL());
		result = EFalse;
		}
	else if(iThreadCountAfterTests < iThreadCountBeforeTests)
		{
		iError++;
		iLogger->WriteStatus(KExtraThreadsKilled,
				 iThreadCountBeforeTests - iThreadCountAfterTests
				 								, iTestNo, &KNULL());
		result = EFalse;
		}
		else
			{
			iInfo++;
			iLogger->WriteLine(KSameThreadCount, iTestNo);
			}
	if(iProcessPriorityBeforeTests != iProcessPriorityAfterTests)
		{
		iError++;
		iLogger->WriteLine(KPriorityChanged, iTestNo);
		result = EFalse;
		}
	else
		{
		iInfo++;
		iLogger->WriteLine(KPriorityNotChanged, iTestNo);
		}
	iInfo++;
	iLogger->WriteLine(KThreadTestComplete, iTestNo);
	return result;
	}

