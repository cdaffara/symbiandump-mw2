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
* Description:  Tests creation of single POS session.
*
*/


#include <ecom/ecom.h>
#include <ecom/implementationinformation.h>
#include <epos_comasuplposhandlerbase.h>
#include <epos_comasuplpossessionbase.h>

#include "epos_comasuplpossessioncreationtest.h"
#include "epos_comasuplpostesterlogger.h"
#include "epos_comasupltesthandler.h"

// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
COMASuplPosSessionCreationTest* COMASuplPosSessionCreationTest
		::NewL(CImplementationInformation& aImplInfo
				, COMASuplPosTesterCategory* aCallBack)
	{
	COMASuplPosSessionCreationTest* self 
		= new(ELeave)COMASuplPosSessionCreationTest(aImplInfo, aCallBack);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplPosSessionCreationTest::~COMASuplPosSessionCreationTest()
	{
	Cancel();
	delete iSessionBase;
	delete iPosHandler;
	}

// ---------------------------------------------------------------------------
// This function is called by test handler. It is entry point of execution
// of a test case.
// ---------------------------------------------------------------------------
//
void COMASuplPosSessionCreationTest::StartTestingL()
	{
	if(iCallBack)
		{
		iLogger->WriteTestHeader(KPosSessionCreationTest, iTestNo);
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
void COMASuplPosSessionCreationTest::RunL()
	{
	switch(iNextInSeq)
		{
		case EStartTest:
			{
			TInt err = KErrNone;
			TRAP(err, iPosHandler = COMASuplPosHandlerBase::NewL(iImplInfo->ImplementationUid()));
			if(KErrNone != err)
				{
				iError++;
				iLogger->WriteLine(KCreatingSessionFail, iTestNo);
				User::Leave(err);
				}
			iInfo++;
			iLogger->WriteLine(_L8("Info: PosHandler created."), iTestNo);
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iNextInSeq = EInitializeHandler;
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EInitializeHandler:
			{
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iNextInSeq = ECreatePosSession;
			iInfo++;
			iLogger->WriteLine(KInitializingHandler, iTestNo);
			iPosHandler->InitializeL(iStatus);
			SetActive();
			break;
			}
		case ECreatePosSession:
			{
			iNextInSeq = EInitializeSession;
			if(iStatus != KErrNone)
				{
				iError++;
				//Could not initialize POS handler
				iLogger->WriteStatus(KError, iStatus.Int(), iTestNo
													, &KReqCompWith());
				//Abort testing
				TestingAborted(KPosSessionCreationTestAborted);
				return;
				}
			iInfo++;
			iLogger->WriteLine(_L8("Info: PosHandler Initialized."), iTestNo);
			iLogger->GetRequestStartTime();
			TRAPD(err, iSessionBase = iPosHandler->CreateNewSessionL(
													iSuplObserver));
			if(KErrNone != err)
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
			
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EInitializeSession:
			{
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iNextInSeq = EPosSessionTestComplete;
			//initializing POS session
			iInfo++;
			iLogger->WriteLine(KInitializingSession, iTestNo);
			iSessionBase->InitializeL(iRequestID, iStatus);
			SetActive();		
			break;
			}
		case EPosSessionTestComplete:
			{
			if(iStatus == KErrNone)
				{
				iInfo++;
				iLogger->WriteLine(KSessionInitialized, iTestNo);
				TestingComplete(KPosSessionCreationTestComplete);
				}
			else
				{
				iError++;
				//Could not initialize POS session
				iLogger->WriteStatus(KError, iStatus.Int(), iTestNo
													, &KReqCompWith());
				//Abort testing
				TestingAborted(KPosSessionCreationTestAborted);
				}
			break;
			}
		}
	}

// ---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void COMASuplPosSessionCreationTest::DoCancel()
	{
	if(ECreatePosSession == iNextInSeq)
		{
		iPosHandler->CancelInitialize();
		}
	
	if(EPosSessionTestComplete == iNextInSeq && iSessionBase)
		{
		iSessionBase->CancelInitialize();
		}
	}

// ---------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL().
// ---------------------------------------------------------------------------
//
TInt COMASuplPosSessionCreationTest::RunError(TInt aError)
	{
	switch(iNextInSeq)
		{
		case EStartTest:
			{
			iError++;
			iLogger->WriteStatus(KLeaveInHandlerCreation, aError, iTestNo);
			break;
			}
		case ECreatePosSession:
			{
			iError++;
			iLogger->WriteStatus(KLeaveInHandlerInit, aError, iTestNo);
			break;
			}
		case EInitializeSession:
			{
			iError++;
			iLogger->WriteStatus(KLeaveInCreateNewSession, aError, iTestNo);
			break;
			}
		case EPosSessionTestComplete:
			{
			iError++;
			iLogger->WriteStatus(KLeaveInSessionInit, aError, iTestNo);
			break;
			}
		}
	TestingAborted(KPosSessionCreationTestAborted);
	return KErrNone;
	}

COMASuplPosSessionCreationTest::COMASuplPosSessionCreationTest(
								CImplementationInformation& aImplInfo
								, COMASuplPosTesterCategory* aCallBack)
								:COMASuplPosTesterCategory(aCallBack)
	{
	iImplInfo = &aImplInfo;
	iNextInSeq = EPosSessionTestComplete;
	iRequestID = 0;
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplPosSessionCreationTest::ConstructL()
	{
	CActiveScheduler::Add(this);
	iSuplObserver = this;
	}
