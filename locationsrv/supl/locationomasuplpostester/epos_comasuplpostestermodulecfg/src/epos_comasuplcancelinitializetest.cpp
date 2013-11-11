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
* Description:  Tests the behaviour of InitializeL and CancelInitialize.
*
*/

#include <e32base.h>
#include <epos_comasuplposhandlerbase.h>
#include <epos_comasuplpossessionbase.h>

#include "epos_comasuplcancelinitializetest.h"
#include "epos_comasupltesthandler.h"
#include "epos_comasuplpostesterlogger.h"

// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
COMASuplCancelInitializeTest* COMASuplCancelInitializeTest
									::NewL(const TUid& aUid
									, COMASuplPosTesterCategory* aCallBack)
	{
	COMASuplCancelInitializeTest* self = new(ELeave)
							COMASuplCancelInitializeTest(aUid, aCallBack);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplCancelInitializeTest::~COMASuplCancelInitializeTest()
	{
	delete iPosHandler;
	delete iSessionBase;
	}

COMASuplCancelInitializeTest::COMASuplCancelInitializeTest(const TUid& aUid
									, COMASuplPosTesterCategory* aCallBack)
							: COMASuplPosTesterCategory(aCallBack) 
							, iUid(aUid)
							
	{
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplCancelInitializeTest::ConstructL()
	{
	CActiveScheduler::Add(this);
	}


// ---------------------------------------------------------------------------
// This function is called by test handler. It is entry point of execution
// of a test case.
// ---------------------------------------------------------------------------
//
void COMASuplCancelInitializeTest::StartTestingL()
	{
	if(iCallBack)
		{
		iLogger->WriteTestHeader(KCancelInitializeTest, iTestNo);
		}
	iNextInSeq = EStartTest;
	__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
	DummyRequest(iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Handles an active object's request completion event.
// ---------------------------------------------------------------------------
//
void COMASuplCancelInitializeTest::RunL()
	{
	switch(iNextInSeq)
		{
		case EStartTest:
			{
			iLogger->GetRequestStartTime();
			TRAPD(handlerErr, iPosHandler = COMASuplPosHandlerBase::NewL(iUid));
			if(KErrNone != handlerErr)
				{
				iError++;
				iLogger->WriteLine(KHandlerCreateFail, iTestNo);
				User::Leave(handlerErr);
				}
			iInfo++;
			iLogger->LogExecutionTime(KExecHandlerNewL);
			
			//Cancel handler
			iInfo++;
			iLogger->WriteLine(_L8("Info: Canceling POS Handler initialization before giving Initialization request."), iTestNo);
			iPosHandler->CancelInitialize();
			iInfo++;
			iLogger->WriteLine(_L8("Info: Cancellation Done."), iTestNo);
			iNextInSeq = EInitializeAndCancel;
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EInitializeAndCancel:
			{
			//initialize Handler and cancel
			iInfo++;
			iLogger->WriteLine(_L8("Info: Initializing POS handler with an outstanding cancel request."), iTestNo);
			iNextInSeq = EHandlerInitAndCancelComplete;
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iPosHandler->InitializeL(iStatus);
			SetActive();
			iPosHandler->CancelInitialize();
			break;
			}
		case EHandlerInitAndCancelComplete:
			{
			if(iStatus == KErrNone)
				{
				//Request completed even with an outstsnding cancellation 
				//request.
				iWarning++;
				iLogger->WriteStatus(KWarning, KErrNone, iTestNo
														, &KReqCompWith());
				__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
				iNextInSeq = ECreateNewSession;
				DummyRequest(iStatus);
				SetActive();
				}
			else
				{
				if(KErrCancel == iStatus.Int())
					{
					iInfo++;
					iLogger->WriteStatus(KInfo, iStatus.Int(), iTestNo
														, &KReqCompWith());
					}
				else
					{
					iError++;
					iLogger->WriteStatus(KError, iStatus.Int(), iTestNo
														, &KReqCompWith());
					}
				//PosHandler initialization failed
				//Now Initialize the pos handler.
				iNextInSeq = EHandlerInitComplete;
				__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
				iInfo++;
				iLogger->WriteLine(KInitializingHandler, iTestNo);
				iPosHandler->InitializeL(iStatus);
				SetActive();
				}
			
			break;
			}
		case EHandlerInitComplete:
			{
			if(iStatus == KErrNone)
				{
				iInfo++;
				iLogger->WriteStatus(KInfo, KErrNone, iTestNo
														, &KReqCompWith());
				__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
				iNextInSeq = ECreateNewSession;
				DummyRequest(iStatus);
				SetActive();
				}
			else
				{
				iError++;
				iLogger->WriteStatus(KError, KErrNone, iTestNo
														, &KReqCompWith());
				TestingAborted(KCancelInitializeTestAborted);
				}
		
			break;
			}
		case ECreateNewSession:
			{
			iLogger->GetRequestStartTime();
			TRAPD( err, iSessionBase = iPosHandler->CreateNewSessionL(this));
			if( KErrNone != err )
				{
				iError++;
				iLogger->WriteLine(KCreatingSessionFail, iTestNo);
				User::Leave(err);
				}
			iInfo++;
			iLogger->LogExecutionTime(KExecCreateNewSessionL);
			iInfo++;
			iLogger->WriteLine(KSessionCreated, iTestNo);
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iNextInSeq = ECancelBeforeInitializingSession;
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case ECancelBeforeInitializingSession:
			{
			iInfo++;
			iLogger->WriteLine(_L8("Info: Canceling POS Session initialize before initialize request."), iTestNo);
			iSessionBase->CancelInitialize();
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iNextInSeq = EInitializeSessionAndCancel;
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EInitializeSessionAndCancel:
			{
			iInfo++;
			iLogger->WriteLine(_L8("Info: Initializing POS Session with an outstanding POS Session Cancel Request."), iTestNo);
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iNextInSeq = ESessionInitializationComplete;
			iSessionBase->InitializeL(iRequestID, iStatus);
			SetActive();
			iSessionBase->CancelInitialize();
			break;
			}
		case ESessionInitializationComplete:
			{
			if(iStatus == KErrCancel)
				{
				iInfo++;
				iLogger->WriteLine(KInitializeInfo, iTestNo);
				}
			else
				{
				iWarning++;
				iLogger->WriteLine(KInitializeWarning, iTestNo);
				}
			TestingComplete(KCancelInitializeTestComplete);
			break;
			}
		default:
			{
			TestingAborted(KCancelInitializeTestAborted);
			break;
			}
		}
	}
	
// ---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void COMASuplCancelInitializeTest::DoCancel()
	{
	if(EHandlerInitComplete == iNextInSeq)
		{
		//POS handler init cancel
		iPosHandler->CancelInitialize();
		}
	}

// ---------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL().
// ---------------------------------------------------------------------------
//
TInt COMASuplCancelInitializeTest::RunError( TInt aError)
	{
	iError++;
	iLogger->WriteStatus(KLeaveOccured, aError, iTestNo);
	TestingAborted(KCancelInitializeTestAborted);
	return KErrNone;
	}
