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
* Description:  Tests creation and initialization of POS Handler.
*
*/

#include <epos_comasuplposhandlerbase.h>
#include <epos_comasuplpossessionbase.h>
#include <ecom/ecom.h>

#include "epos_comasuplposhandlercreationtest.h"
#include "epos_comasupltesthandler.h"
#include "epos_comasuplpostesterlogger.h"

// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
COMASuplPosHandlerCreationTest* COMASuplPosHandlerCreationTest::NewL(
								const TUid& aUid
								, COMASuplPosTesterCategory* aCallBack)
	{
	COMASuplPosHandlerCreationTest* self 
						= new(ELeave)COMASuplPosHandlerCreationTest(
														aUid, aCallBack);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplPosHandlerCreationTest::~COMASuplPosHandlerCreationTest()
	{
	Cancel();
	delete iPosHandler;
	}

// ---------------------------------------------------------------------------
// This function is called by test handler. It is entry point of execution
// of a test case.
// ---------------------------------------------------------------------------
//
void COMASuplPosHandlerCreationTest::StartTestingL()
	{
	if(iCallBack)
		{
		iLogger->WriteTestHeader(KPosHandlerCreationTest, iTestNo);
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
void COMASuplPosHandlerCreationTest::RunL()
	{
	switch(iNextInSeq)
		{
		case EStartTest:
			{
			TInt err = KErrNone;
			iLogger->GetRequestStartTime();
			TRAP(err, iPosHandler = COMASuplPosHandlerBase::NewL(iUid));
			if(KErrNone != err)
				{
				iError++;
				iLogger->WriteLine(KHandlerCreateFail, iTestNo);
				User::Leave(err);
				}
			iInfo++;
			iLogger->LogExecutionTime(_L8("Info: Execution Time of COMASuplPosHandlerBase::NewL()"));
			iInfo++;
			iLogger->WriteLine(_L8("Info: PosHandler Created."), iTestNo);
			iNextInSeq = EInitializeHandler;
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			DummyRequest(iStatus);
		 	SetActive();
		 	break;
			}
		case EInitializeHandler:
			{
			iNextInSeq = EInitComplete;
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iInfo++;
			iLogger->WriteLine(KInitializingHandler, iTestNo);
		 	iPosHandler->InitializeL(iStatus);
		 	SetActive();
			break;
			}
		case EInitComplete:
			{
			if(iStatus == KErrNone)
				{
				iInfo++;
				iLogger->WriteLine(KInitializedHandler, iTestNo);
				TestingComplete(KPosHandlerCreationComplete);	
				}
			else
				{
				iError++;
				iLogger->WriteStatus(KError, iStatus.Int(), iTestNo
														, &KReqCompWith());
				TestingAborted(KPosHandlerCreationAborted);
				}
			}
		}
	}

// ---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void COMASuplPosHandlerCreationTest::DoCancel()
	{
	iPosHandler->CancelInitialize();
	}

// ---------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL().
// ---------------------------------------------------------------------------
//
TInt COMASuplPosHandlerCreationTest::RunError(TInt aError)
	{
	switch(iNextInSeq)
		{
		case EStartTest:
			{
			iError++;
			iLogger->WriteStatus(KLeaveInHandlerCreation, aError, iTestNo);
			break;
			}
		case EInitComplete:
			{
			iError++;
			iLogger->WriteStatus(KLeaveInHandlerInit, aError, iTestNo);
			break;
			}
		}
	
	TestingAborted(KPosHandlerCreationAborted);
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplPosHandlerCreationTest::ConstructL()
	{
	CActiveScheduler::Add(this);
	}

// ---------------------------------------------------------------------------
// Stores Uid of implementation to be tested.
// ---------------------------------------------------------------------------
//	
COMASuplPosHandlerCreationTest::COMASuplPosHandlerCreationTest(
								const TUid& aUid
								, COMASuplPosTesterCategory* aCallBack)
								:COMASuplPosTesterCategory(aCallBack)
	{
	iUid = aUid;
	}
