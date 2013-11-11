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
* Description:  Tests GetPositionL() in various scenarios.
*
*/

#include <e32base.h>
#include <epos_comasuplposhandlerbase.h>
#include <epos_comasuplpossessionbase.h>
#include <epos_comasuplposition.h>
#include <epos_eomasuplposerrors.h>

#include "epos_comasuplpositiontest.h"
#include "epos_comasupltesthandler.h"
#include "epos_comasuplpostesterlogger.h"


// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
COMASuplPositionTest* COMASuplPositionTest::NewL(COMASuplPosHandlerBase*
									aPosHandler
									, COMASuplPosTesterCategory* aCallBack)
	{
	COMASuplPositionTest* self = new(ELeave)COMASuplPositionTest(aPosHandler
																, aCallBack);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplPositionTest::~COMASuplPositionTest()
	{
	delete iPosition;
	delete iSessionBase;
	}

COMASuplPositionTest::COMASuplPositionTest(COMASuplPosHandlerBase* 
										aPosHandler
									, COMASuplPosTesterCategory* aCallBack)
									: COMASuplPosTesterCategory(aCallBack)
									, iPosHandler(*aPosHandler)
	{
	
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplPositionTest::ConstructL()
	{
	CActiveScheduler::Add(this);
	}

void COMASuplPositionTest::InitializePositionL()
	{
	delete iPosition;
	iPosition = NULL;
	iPosition = COMASuplPosition::NewL();
	}

void COMASuplPositionTest::RequestForPositionL()
	{
	TRAPD(err, InitializePositionL());
	if(KErrNone != err)
		{
		iError++;
		iLogger->WriteStatus(KLeaveResettingPosition(), err, iTestNo);
		TestingAborted(KPositionTestAborted);
		return;
		}
	__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
	iSessionBase->GetPositionL(iStatus, iPosition);
	SetActive();
	}

// ---------------------------------------------------------------------------
// This function is called by test handler. It is entry point of execution
// of a test case.
// ---------------------------------------------------------------------------
//
void COMASuplPositionTest::StartTestingL()
	{
	if(iCallBack)
		{
		iLogger->WriteTestHeader(KPositionTest, iTestNo);
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
void COMASuplPositionTest::RunL()
	{
	switch(iNextInSeq)
		{
		case EStartTest:
			{
			iLogger->GetRequestStartTime();
			TRAPD( err, iSessionBase = iPosHandler.CreateNewSessionL(this));
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
			iNextInSeq = EGetPositionBeforeInitialize;
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EGetPositionBeforeInitialize:
			{
			iNextInSeq = EGetPositionCompleteBeforeInitialize;
			iInfo++;
			iLogger->WriteLine(KGetPositionLInfo, iTestNo);
			RequestForPositionL();
			break;
			}

		case EGetPositionCompleteBeforeInitialize:
			{
			iNextInSeq = EInitializeSession;
			if(iStatus == KErrOMASuplPosInActive)
				{
				iInfo++;
				iLogger->WriteStatus(
						_L8("Info: GetPositionL completed with ")
						, iStatus.Int(), iTestNo);
				}
			else
				{
				iWarning++;
				iLogger->WriteStatus(
						_L8("Warning: GetPositionL completed with ")
						, iStatus.Int(), iTestNo);
				}
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			DummyRequest(iStatus);
			SetActive();
			break;
			}

		case EInitializeSession:
			{
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iNextInSeq = EGetPositionAfterInitialize;
			iInfo++;
			iLogger->WriteLine(KInitializingSession);
			
			iSessionBase->InitializeL(iRequestID, iStatus);
			SetActive();
			break;
			}

		case EGetPositionAfterInitialize:
			{
			if(iStatus != KErrNone)
				{
				iError++;
				iLogger->WriteStatus(KError, iStatus.Int(), iTestNo, &KReqCompWith());
				TestingAborted(KPositionTestAborted);
				return;
				}
			else
				{
				iInfo++;
				iLogger->WriteStatus(KInfo, KErrNone, iTestNo, &KReqCompWith());
				}
			iNextInSeq = EGetPositionCompleteAfterInitialize;
			iInfo++;
			iLogger->WriteLine(KGetPositionLSessionInitInfo, iTestNo);
			RequestForPositionL();
			break;
			}
		case EGetPositionCompleteAfterInitialize:
			{
			iNextInSeq = EEndSessionAndGetPosition;
			if(iStatus != KErrNone)
				{
				iWarning++;
				iLogger->WriteStatus(
						_L8("Warning: GetPositionL completed with ")
						, iStatus.Int(), iTestNo);
				}
			else
				{
				iInfo++;
				iLogger->WriteStatus(
						_L8("Info: GetPositionL completed with ")
						, iStatus.Int(), iTestNo);
				}
			iLogger->LogSuplPosition(iPosition);
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EEndSessionAndGetPosition:
			{
			iNextInSeq = EGetPositionCompleteAfterEndSession;
			iInfo++;
			iLogger->WriteLine(KEndingSession, iTestNo);
			iSessionBase->SessionEnd();
			iInfo++;
			iLogger->WriteLine(KGetPositionLSessionEndInfo, iTestNo);
			RequestForPositionL();
			break;
			}
		case EGetPositionCompleteAfterEndSession:
			{
			if(iStatus == KErrOMASuplPosInActive)
				{
				iInfo++;
				iLogger->WriteStatus(KAfterEndSessionI, iStatus.Int()
													, iTestNo);
				}
			else
				{
				iWarning++;
				iLogger->WriteStatus(KAfterEndSessionW, iStatus.Int()
													, iTestNo);
				}
			iLogger->LogSuplPosition(iPosition);
			TestingComplete(KPositionTestComplete);
			break;
			}
		}
	}

// ---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void COMASuplPositionTest::DoCancel()
	{
	if(EGetPositionCompleteBeforeInitialize == iNextInSeq
	|| EGetPositionCompleteAfterInitialize == iNextInSeq
	|| EGetPositionCompleteAfterEndSession == iNextInSeq)
		{
		//Cancel position
		iSessionBase->CancelGetPosition();
		return;
		}
	if(EGetPositionAfterInitialize == iNextInSeq)
		{
		//Cancel sessionInit
		iSessionBase->CancelInitialize();
		return;
		}
	}

// ---------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL().
// ---------------------------------------------------------------------------
//
TInt COMASuplPositionTest::RunError(TInt aError)
	{
	if(EStartTest ==  iNextInSeq)
		{
		iError++;
		iLogger->WriteStatus(KLeaveInCreateNewSession, aError, iTestNo);
		}
	if(EGetPositionCompleteBeforeInitialize == iNextInSeq
	|| EGetPositionCompleteAfterInitialize == iNextInSeq
	|| EGetPositionCompleteAfterEndSession == iNextInSeq)
		{
		//Leave in Position
		iError++;
		iLogger->WriteStatus(KLeaveInGetPosition, aError, iTestNo);
		}
	if(EGetPositionAfterInitialize == iNextInSeq)
		{
		//Leave in Session Initialization
		iError++;
		iLogger->WriteStatus(KLeaveInSessionInit, aError, iTestNo);
		}
	TestingAborted(KPositionTestAborted);
	return KErrNone;
	}
