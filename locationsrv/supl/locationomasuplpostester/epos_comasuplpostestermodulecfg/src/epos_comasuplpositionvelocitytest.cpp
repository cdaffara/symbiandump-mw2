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
* Description:  Handles test execution and monitors user input.
*
*/

#include <e32base.h>
#include <f32file.h>
#include <epos_comasuplposhandlerbase.h>
#include <epos_comasuplpossessionbase.h>
#include <epos_comasuplinforequestlist.h>
#include <epos_comasuplinforequestlist.h>
#include <epos_comasuplposition.h>
#include <epos_comasuplpospayload.h>
#include <epos_comasuplvelocity.h>
#include <epos_eomasuplposerrors.h>
#include <stiflogger.h>

#include "epos_comasuplpositionvelocitytest.h"
#include "epos_comasupltesthandler.h"
#include "epos_comasuplpostestercategory.h"
#include "epos_comasuplpostesterlogger.h"
#include "epos_comasuplmessagefilereader.h"
#include "epos_comasuplpositionvelocitymanager.h"

// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
COMASuplPositionVelocityTest* COMASuplPositionVelocityTest::NewL(
							COMASuplPosHandlerBase* aPosHandler
							, TInt aGetPositionReqCount
							, TFileName& aFileName
							, COMASuplPosTesterCategory* aCallBack)
	{
	COMASuplPositionVelocityTest* self = new(ELeave)
							COMASuplPositionVelocityTest(aPosHandler
							, aCallBack
							, aGetPositionReqCount
							, aFileName);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplPositionVelocityTest::~COMASuplPositionVelocityTest()
	{
	//delete iMsgFileReader;
	}

COMASuplPositionVelocityTest::COMASuplPositionVelocityTest(
							COMASuplPosHandlerBase* aPosHandler
							, COMASuplPosTesterCategory* aCallBack
							, TInt aGetPositionReqCount
							, TFileName& aFileName)
							:COMASuplPositionVelocityManager(aCallBack
										, aFileName)
	{
	iPosHandler = aPosHandler;
	iMaxGetPositionReqCount = aGetPositionReqCount;
	iGetPositionRequested = 0;
	CActiveScheduler::Add(this);
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityTest::ConstructL()
	{
	iPreviousPosition = NULL;
	}

// ---------------------------------------------------------------------------
// This function is called by test handler. It is entry point of execution
// of a test case.
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityTest::StartTestingL()
	{
	if(iCallBack)
		{
		iLogger->WriteTestHeader(KPositionVelocityTest, iTestNo);
		}
	iNextInSeq = EStartTest;
	__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
	DummyRequest(iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Provides implementation required for cancellation of testing
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityTest::TestingCancelled(const TDesC8& /*aInfo*/)
	{
	if(iCallBack)
		{
		HBufC8* infoBuf = NULL;
		TRAPD(err, infoBuf = HBufC8::NewL(KPositionVelocityTestCancel().Length() + KExtraBuffer));
		if(err != KErrNone)
			{
			iCallBack->TestingCancelled(KPositionVelocityTestCancel);
			}
		else
			{
			TPtr8 info = infoBuf->Des();
			info.Copy(KPositionVelocityTestCancel);
			info.Append(KSessionId);
			info.AppendNum(iTestNo);
			iCallBack->TestingCancelled(info);
			}
		delete infoBuf;
		iCallBack->UpdateSessionTestSummary(iInfo, iWarning, iError);
		}
	else
		{
		Cancel();
		iTestHandler->Cancel();
		iTestingStatus = ETestCancelled;
		CActiveScheduler::Stop();
		iInfo++;
		iLogger->WriteLine(KPositionVelocityTestCancel, iTestNo);
		}
	}

// ---------------------------------------------------------------------------
//Provides implementation required for completion of testing
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityTest::TestingComplete(const TDesC8& /*aInfo*/)
	{
	if(iCallBack)
		{
		HBufC8* infoBuf = NULL;
		TRAPD(err, infoBuf = HBufC8::NewL(KPositionVelocityTestComplete().Length() + KExtraBuffer));
		if(err != KErrNone)
			{
			iCallBack->TestingComplete(KPositionVelocityTestComplete);
			}
		else
			{
			TPtr8 info = infoBuf->Des();
			info.Copy(KPositionVelocityTestComplete);
			info.Append(KSessionId);
			info.AppendNum(iTestNo);
			iCallBack->TestingComplete(info);
			}
		delete infoBuf;
		iCallBack->UpdateSessionTestSummary(iInfo, iWarning, iError);
		}
	else
		{
		iTestHandler->Cancel();
		Cancel();
		CActiveScheduler::Stop();
		iTestingStatus = ETestComplete;
		iInfo++;
		iLogger->WriteLine(KPositionVelocityTestComplete, iTestNo);
		}
	
	}

// ---------------------------------------------------------------------------
//Provides implementation required for aborting testing
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityTest::TestingAborted(const TDesC8& /*aInfo*/)
	{
	if(iCallBack)
		{
		HBufC8* infoBuf = NULL;
		TRAPD(err, infoBuf = HBufC8::NewL(KPositionVelocityTestAborted().Length() + KExtraBuffer));
		if(err != KErrNone)
			{
			iCallBack->TestingAborted(KPositionVelocityTestAborted);
			}
		else
			{
			TPtr8 info = infoBuf->Des();
			info.Copy(KPositionVelocityTestAborted);
			info.Append(KSessionId);
			info.AppendNum(iTestNo);
			iCallBack->TestingAborted(info);
			}
		delete infoBuf;
		iCallBack->UpdateSessionTestSummary(iInfo, iWarning, iError);
		}
	else
		{
		Cancel();
		iTestHandler->Cancel();
		iTestingStatus = ETestAborted;
		CActiveScheduler::Stop();
		iError++;
		iLogger->WriteLine(KPositionVelocityTestAborted, iTestNo);
		}
	}

// ---------------------------------------------------------------------------
// Handles an active object's request completion event.
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityTest::RunL()
	{
	switch( iNextInSeq )
		{
		case EStartTest:
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
			iMsgInfoList = &(iMsgFileReader->GetMessageListL());
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iNextInSeq = ERequestToInitializeSession;
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case ERequestToInitializeSession:
			{
			iInfo++;
			iLogger->WriteLine(KInitializingSession, iTestNo);
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iNextInSeq = ERequestToInitializeSessionComplete;
			iSessionBase->InitializeL(iRequestID, iStatus);
			SetActive();
			break;
			}
			
		case ERequestToInitializeSessionComplete:
			{
			if(iStatus != KErrNone)
				{
				iError++;
				iLogger->WriteStatus(KError, iStatus.Int(), iTestNo
												, &KSessionInitResult());
				TestingAborted(KPositionVelocityTestAborted);
				return;
				}
			iInfo++;
			iLogger->WriteLine(KSessionInitialized, iTestNo);	
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iNextInSeq = EProcessInstructions;
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EProcessInstructions:
			{
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			ProcessNextInstructionL();
			break;
			}
		case ERequestForInfo:
			{
			iNextInSeq = ERequestForInfoComplete;
			RequestForInfoL();
			SetActive();
			break;
			}
		case ERequestForInfoComplete:
			{
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			ProcessRequestCompleteL(iStatus);
			break;
			}
		case ERequestForPosition:
			{
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			RequestForPositionL(iStatus);
			break;
			}
		case ERequestForPositionComplete:
			{
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			ProcessGetPositionCompleteL(iStatus);
			break;
			}
		}
	}

// ---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityTest::DoCancel()
	{
	if(ERequestToInitializeSessionComplete == iNextInSeq)
		{
		iSessionBase->CancelInitialize();
		}

	if(ERequestForInfoComplete == iNextInSeq)
		{
		//Cancel GetSuplInfoL()
		iSessionBase->CancelSuplInfoRequest();
		}

	if(ERequestForPositionComplete == iNextInSeq)
		{
		//Cancel GetPositionL())
		iSessionBase->CancelGetPosition();
		}
	}

// ---------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL().
// ---------------------------------------------------------------------------
//
TInt  COMASuplPositionVelocityTest::RunError( TInt aError )
	{
	if(ERequestToInitializeSessionComplete == iNextInSeq)
		{
		iError++;
		iLogger->WriteStatus(KLeaveInSessionInit, aError, iTestNo);
		}
	else if(ERequestForInfoComplete == iNextInSeq)
		{
		iError++;
		iLogger->WriteStatus(KLeaveInGetSuplInfo, aError, iTestNo);
		}
	else if(ERequestForPositionComplete == iNextInSeq)
		{
		iError++;
		iLogger->WriteStatus(KLeaveInGetPosition, aError, iTestNo);
		}
	else
		{
		iError++;
		iLogger->WriteStatus(KLeaveOccured, aError, iTestNo);
		}
	TestingAborted(KPositionVelocityTestAborted);
	return KErrNone;
	}
