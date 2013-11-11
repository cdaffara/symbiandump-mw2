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
* Description:  Controls the execution of multiple session test.
*
*/

#include <ecom/implementationinformation.h>
#include "epos_comasuplmultiplesessiontest.h"
#include "epos_comasupltesthandler.h"
#include "epos_comasuplpostesterlogger.h"

#include "epos_comasuplpositionvelocitytest.h"
#include "epos_comasuplgetsuplinfocanceltest.h"
#include "epos_comasuplgetsuplinfotest.h"
#include "epos_comasuplmsgfilereadertest.h"
#include "epos_comasuplpossessioncreationtest.h"
#include "epos_comasuplpositioncanceltest.h"
#include "epos_comasuplcancelinitializetest.h"
#include "epos_comasuplpositiontest.h"
#include "epos_comasuplsessiontest.h"

_LIT8(KMultipleCanceled, "Info: Multiple Session Test Canceled.");
_LIT8(KMultipleComplete, "Info: Multiple Session test complete.");
const TInt KPositionReqCount = 2;

// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
COMASuplMultipleSessionTest* COMASuplMultipleSessionTest::NewL(
									COMASuplPosHandlerBase* aPosHandler
									, CImplementationInformation& aImplInfo
									, TInt aSessionCount
									, COMASuplPosTesterCategory* aCallBack)
	{
	COMASuplMultipleSessionTest* self = new(ELeave)
				COMASuplMultipleSessionTest(aPosHandler, aImplInfo
									, aSessionCount, aCallBack);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplMultipleSessionTest::~COMASuplMultipleSessionTest()
	{
	iTestList.ResetAndDestroy();
	iTestList.Close();
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::ConstructL()
	{
	AddCancelInitializeTestL();
	AddPosSessionCreationTestL();
	AddGetSuplInfoTestL();
	AddGetSuplInfoCancelTestL();
	
	AddMsgFileReaderTestL();
	AddPositionVelocityTestL();
	AddPositionCancelTestL();
	AddPositionTestL();
	AddSessionTestL();
	}

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
COMASuplMultipleSessionTest::COMASuplMultipleSessionTest(
							COMASuplPosHandlerBase* aPosHandler
							, CImplementationInformation& aImplInfo
							, TInt aSessionCount
							, COMASuplPosTesterCategory* aCallBack)
							: COMASuplPosTesterCategory(aCallBack)
							, iPosHandler(*aPosHandler)
							, iImplInfo(aImplInfo)
							, iSessionCount(aSessionCount)
							, iTestExecuted(0)
							, iTotalTests(0)
	{
	
	}

// ---------------------------------------------------------------------------
// Initializes all test class objects
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::InitializeL(COMASuplTestHandler* aTestHandler
						, COMASuplPosTesterLogger& aLogger, TInt /*aTestNo*/)
	{
	iTestHandler = aTestHandler;
	iLogger = &aLogger;
	TInt listSize = iTestList.Count();
	//Initialize the list
	for(TInt count = 0; count < listSize; count++)
		{
		//Initialize
		iTestList[count]->InitializeL(iTestHandler, *iLogger, count + 1);
		}
	}

// ---------------------------------------------------------------------------
// Starts testing of test classes one by one.
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::StartTestingL()
	{
	TInt listSize = iTestList.Count();
	for(TInt count = 0; count < listSize; count++)
		{
		//Create objects. Append to list.
		iTestList[count]->StartTestingL();
		}
	}

// ---------------------------------------------------------------------------
// Handles an active object's request completion event.
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::RunL()
	{
	
	}

// ---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::DoCancel()
	{
	
	}

// ---------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL().
// ---------------------------------------------------------------------------
//
TInt COMASuplMultipleSessionTest::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// If user selects to cancel tests, all tests are canceled.
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::TestingCancelled(const TDesC8& /*aInfo*/)
	{
	iTestExecuted++;
	Cancel();
	iTestHandler->Cancel();
	iTestingStatus = ETestCancelled;
	TInt listSize = iTestList.Count();
	for(TInt sessionNo = 0; sessionNo < listSize; sessionNo++)
		{
		iTestList[sessionNo]->Cancel();
		}
	CActiveScheduler::Stop();
	iInfo++;
	iLogger->WriteLine(KMultipleCanceled, iTestNo);
	}

// ---------------------------------------------------------------------------
// Logs about testing completion of a session
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::TestingComplete(const TDesC8& aInfo)
	{
	iTestExecuted++;
	iInfo++;
	iLogger->WriteLine(aInfo, iTestNo);
	if(iTestExecuted == iTotalTests)
		{
		iTestHandler->Cancel();
		Cancel();
		CActiveScheduler::Stop();
		iTestingStatus = ETestComplete;
		iInfo++;
		iLogger->WriteLine(KMultipleComplete);
		}
	}

// ---------------------------------------------------------------------------
// Logs that testing has been aborted for a session.
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::TestingAborted(const TDesC8& aError)
	{
	iTestExecuted++;
	iError++;
	iLogger->WriteLine(aError);
	if(iTestExecuted == iTotalTests)
		{
		Cancel();
		iTestHandler->Cancel();
		iTestingStatus = ETestAborted;
		CActiveScheduler::Stop();
		iInfo++;
		iLogger->WriteLine(KMultipleComplete);
		}
	}

// ---------------------------------------------------------------------------
// Adds GetSuplInfoCancelTest to the list of tests to be executed.
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::AddGetSuplInfoCancelTestL()
	{
	for(TInt count = 0; count < iSessionCount; count++)
		{
		//Create objects. Append to list.
		COMASuplGetSuplInfoCancelTest* test
						 = COMASuplGetSuplInfoCancelTest::NewL(
							iPosHandler, this);
		CleanupStack::PushL(test);
		test->CreateRequestListL(1, 1, 1, 1, 1);
		iTotalTests++;
		COMASuplPosTesterCategory* testCategory = static_cast<COMASuplPosTesterCategory*>(test);		
		iTestList.Append(testCategory);
		CleanupStack::Pop(test);
		}
	}

// ---------------------------------------------------------------------------
// Adds GetSuplInfoTest to the list of tests to be executed.
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::AddGetSuplInfoTestL()
	{
	for(TInt count = 0; count < iSessionCount; count++)
		{
		//Create objects. Append to list.
		COMASuplGetSuplInfoTest* test
						 = COMASuplGetSuplInfoTest::NewL(
							iPosHandler, this);
		CleanupStack::PushL(test);
		test->CreateRequestListL(1, 1, 1, 1, 1);
		iTotalTests++;
		COMASuplPosTesterCategory* testCategory = static_cast<COMASuplPosTesterCategory*>(test);		
		iTestList.Append(testCategory);
		CleanupStack::Pop(test);
		}
	}

// ---------------------------------------------------------------------------
// Adds MsgFileReaderTest to the list of tests to be executed.
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::AddMsgFileReaderTestL()
	{
	TFileName msgFile;//Default message file will be selected
	for(TInt count = 0; count < iSessionCount; count++)
		{
		//Create objects. Append to list.
		COMASuplPosTesterCategory* test
						 =  COMASuplMsgFileReaderTest::NewL(msgFile, this);
		
		iTotalTests++;		
		iTestList.Append(test);
		}
	}

// ---------------------------------------------------------------------------
// Adds PosSessionCreationTest to the list of tests to be executed.
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::AddPosSessionCreationTestL()
	{
	for(TInt count = 0; count < iSessionCount; count++)
		{
		//Create objects. Append to list.
		COMASuplPosTesterCategory* test =  COMASuplPosSessionCreationTest
											::NewL(iImplInfo, this);
		iTotalTests++;		
		iTestList.Append(test);
		}
	}

// ---------------------------------------------------------------------------
// Adds PositionVelocityTest to the list of tests to be executed.
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::AddPositionVelocityTestL()
	{
	TFileName fileName;
	for(TInt count = 0; count < iSessionCount; count++)
		{
		//Create objects. Append to list.
		COMASuplPosTesterCategory* test
						 = COMASuplPositionVelocityTest::NewL(
							&iPosHandler, KPositionReqCount, fileName, this);
		iTotalTests++;		
		iTestList.Append(test);
		}
	
	}

// ---------------------------------------------------------------------------
// Adds PositionCancelTest to the list of tests to be executed.
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::AddPositionCancelTestL()
	{
	TFileName fileName;
	for(TInt count = 0; count < iSessionCount; count++)
		{
		//Create objects. Append to list.
		COMASuplPosTesterCategory* test
						 = COMASuplPositionCancelTest::NewL(
							&iPosHandler, KPositionReqCount, fileName, this);
		iTotalTests++;		
		iTestList.Append(test);
		}
	
	}

// ---------------------------------------------------------------------------
// Adds CancelInitializeTest to the list of tests to be executed.
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::AddCancelInitializeTestL()
	{
	for(TInt count = 0; count < iSessionCount; count++)
		{
		//Create objects. Append to list.
		COMASuplPosTesterCategory* test
						 = COMASuplCancelInitializeTest::NewL(
						 			iImplInfo.ImplementationUid()
									, this);
		iTotalTests++;		
		iTestList.Append(test);
		}
	
	}

// ---------------------------------------------------------------------------
// Adds PositionTest to the list of tests to be executed.
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::AddPositionTestL()
	{
	for(TInt count = 0; count < iSessionCount; count++)
		{
		//Create objects. Append to list.
		COMASuplPosTesterCategory* test = COMASuplPositionTest::NewL(
													&iPosHandler, this);
		iTotalTests++;		
		iTestList.Append(test);
		}
	
	}

// ---------------------------------------------------------------------------
// Adds SessionTest to the list of tests to be executed.
// ---------------------------------------------------------------------------
//
void COMASuplMultipleSessionTest::AddSessionTestL()
	{
	for(TInt count = 0; count < iSessionCount; count++)
		{
		//Create objects. Append to list.
		COMASuplSessionTest* test = COMASuplSessionTest::NewL(
													iPosHandler, this);
		CleanupStack::PushL(test);
		test->CreateRequestListL(1, 1, 1, 1, 1);
		iTotalTests++;
		COMASuplPosTesterCategory* testCategory = static_cast<COMASuplPosTesterCategory*>(test);		
		iTestList.Append(testCategory);
		CleanupStack::Pop(test);
		}
	}
