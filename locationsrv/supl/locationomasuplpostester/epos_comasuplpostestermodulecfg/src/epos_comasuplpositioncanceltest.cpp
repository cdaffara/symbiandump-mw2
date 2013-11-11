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
* Description:  Tests for GetPositionL request cancellation.
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

#include "epos_comasuplpositioncanceltest.h"

#include "epos_comasupltesthandler.h"
#include "epos_comasuplpostestercategory.h"
#include "epos_comasuplpostesterlogger.h"
#include "epos_comasuplmessagefilereader.h"
#include "epos_comasuplpositionvelocitymanager.h"

/** Constant used for logging.*/
_LIT8(KGetPosCodeW, 
"Warning: After cancellation, GetPositionL completed with error code.");

/** Constant used for logging.*/
_LIT8(KGetPosCodeI, 
"Info: After cancellation, GetPositionL completed with error code.");

/** Constant used for logging.*/
_LIT8(KCancelRequestBeforeRequest, "Info: Canceling Position request before \
giving Position request");

// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
COMASuplPositionCancelTest* COMASuplPositionCancelTest::NewL(
							COMASuplPosHandlerBase* 
							aPosHandler
							, TInt aGetPositionReqCount
							, TFileName& aFileName
							, COMASuplPosTesterCategory* aCallBack)
	{
	COMASuplPositionCancelTest* self = new(ELeave)
							COMASuplPositionCancelTest(aPosHandler
							, aGetPositionReqCount
							, aFileName
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
COMASuplPositionCancelTest::~COMASuplPositionCancelTest()
	{
	//delete iMsgFileReader;
	}

COMASuplPositionCancelTest::COMASuplPositionCancelTest(
							COMASuplPosHandlerBase* aPosHandler
							, TInt aGetPositionReqCount
							, TFileName& aFileName
							, COMASuplPosTesterCategory* aCallBack)
							:COMASuplPositionVelocityManager(aCallBack
											, aFileName)
	{
	iPosHandler = aPosHandler;
	iMaxGetPositionReqCount = aGetPositionReqCount;
	iGetPositionRequested = 0;
	iCancelRequested = EFalse;
	CActiveScheduler::Add(this);
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplPositionCancelTest::ConstructL()
	{
	iPreviousPosition = NULL;
	}

// ---------------------------------------------------------------------------
// This function is called by test handler. It is entry point of execution
// of a test case.
// ---------------------------------------------------------------------------
//
void COMASuplPositionCancelTest::StartTestingL()
	{
	if(iCallBack)
		{
		iLogger->WriteTestHeader(KPositionCancelTest, iTestNo);
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
void COMASuplPositionCancelTest::TestingCancelled(const TDesC8& /*aInfo*/)
	{
	if(iCallBack)
		{
		HBufC8* infoBuf = NULL;
		TRAPD(err, infoBuf = HBufC8::NewL(KPositionCancelTestCancel().Length() + KExtraBuffer));
		if(err != KErrNone)
			{
			iCallBack->TestingCancelled(KPositionCancelTestCancel);
			}
		else
			{
			TPtr8 info = infoBuf->Des();
			info.Copy(KPositionCancelTestCancel);
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
		iLogger->WriteLine(KPositionCancelTestCancel, iTestNo);
		}
	}

// ---------------------------------------------------------------------------
//Provides implementation required for completion of testing
// ---------------------------------------------------------------------------
//
void COMASuplPositionCancelTest::TestingComplete(const TDesC8& /*aInfo*/)
	{
	if(iCallBack)
		{
		HBufC8* infoBuf = NULL;
		TRAPD(err, infoBuf = HBufC8::NewL(KPositionCancelTestComplete().Length() + KExtraBuffer));
		if(err != KErrNone)
			{
			iCallBack->TestingComplete(KPositionCancelTestComplete);
			}
		else
			{
			TPtr8 info = infoBuf->Des();
			info.Copy(KPositionCancelTestComplete);
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
		iTestingStatus = ETestComplete;
		CActiveScheduler::Stop();
		iInfo++;
		iLogger->WriteLine(KPositionCancelTestComplete, iTestNo);
		}
	}

// ---------------------------------------------------------------------------
//Provides implementation required for aborting testing
// ---------------------------------------------------------------------------
//
void COMASuplPositionCancelTest::TestingAborted(const TDesC8& aError)
	{
	if(iCallBack)
		{
		HBufC8* infoBuf = NULL;
		TRAPD(err, infoBuf = HBufC8::NewL(aError.Length() + KExtraBuffer));
		if(err != KErrNone)
			{
			iCallBack->TestingAborted(aError);
			}
		else
			{
			TPtr8 info = infoBuf->Des();
			info.Copy(aError);
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
		iLogger->WriteLine(KPositionCancelTestAborted, iTestNo);
		}
	}

// ---------------------------------------------------------------------------
// Handles an active object's request completion event.
// ---------------------------------------------------------------------------
//
void COMASuplPositionCancelTest::RunL()
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
				iLogger->WriteLine(KSessionInitializeFail, iTestNo);
				TestingAborted(KPositionCancelTestAborted);
				return;
				}
			iInfo++;
			iLogger->WriteLine(KSessionInitialized, iTestNo);
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iNextInSeq = ECancelBeforeGivingRequest;
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case ECancelBeforeGivingRequest:
			{
			iInfo++;
			iLogger->WriteLine(KCancelRequestBeforeRequest, iTestNo);
			CancelGetPositionRequest();
			iInfo++;
			iLogger->WriteLine(_L8("Info: Canceled before giving any request."), iTestNo);
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iNextInSeq = EStartMsgTransaction;
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EStartMsgTransaction:
			{
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			ProcessNextInstructionL();
			break;
			}
		case ERequestForInfo:
			{
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iNextInSeq = ERequestForInfoComplete;
			RequestForInfoL();
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
			CancelGetPositionRequest();
			CancellationRequested();
			break;
			}
		case ERequestForPositionComplete:
			{
			if(iCancelRequested)
				{
				if(iStatus != KErrCancel)
					{
					iWarning++;
					iLogger->WriteStatus(KGetPosCodeW
								, iStatus.Int(), iTestNo, &KNULL());
					}
				else
					{
					iInfo++;
					iLogger->WriteStatus(KGetPosCodeI
								, iStatus.Int(), iTestNo, &KNULL());
					}
				TestingComplete(KPositionCancelTestComplete);
				break;
				}
			else
				{
				__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
				TInt err = ProcessGetPositionCompleteL(iStatus);
				if(err == KErrNone)
					{
					CancellationRequested();
					CancelGetPositionRequest();
					}
				}
			break;
			}
		}//End of switch
	}


// ---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void COMASuplPositionCancelTest::DoCancel()
	{
	if(ERequestToInitializeSessionComplete == iNextInSeq)
		{
		// Cancel Session Initialization
		iSessionBase->CancelInitialize();
		}
	 if(ERequestForInfoComplete == iNextInSeq)
	 	{
	 	//Cancel GetSuplInfoL()
	 	iSessionBase->CancelSuplInfoRequest();
	 	}
		
	if(ERequestForPositionComplete  == iNextInSeq)
		{
		//Cancel GetPositionL
		iSessionBase->CancelGetPosition();
		}
	}

// ---------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL().
// ---------------------------------------------------------------------------
//
TInt  COMASuplPositionCancelTest::RunError( TInt aError )
	{
	iError++;
	iLogger->WriteStatus(KLeaveOccured, aError, iTestNo);
	TestingAborted(KPositionCancelTestAborted);
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// Cancels GetPositionL().
// ---------------------------------------------------------------------------
//
void COMASuplPositionCancelTest::CancelGetPositionRequest()
	{
	iInfo++;
	iLogger->WriteLine(_L8("Info: Canceling position request."), iTestNo);
	iSessionBase->CancelGetPosition();
	}

// ---------------------------------------------------------------------------
// Sets iCancelRequested to ETrue.
// ---------------------------------------------------------------------------
//
void COMASuplPositionCancelTest::CancellationRequested()
	{
	iCancelRequested = ETrue;
	}

