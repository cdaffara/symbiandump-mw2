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
* Description:  Tests GetSuplInfoL in various scenarios.
*
*/

#include <epos_comasuplposhandlerbase.h>
#include <epos_comasuplpossessionbase.h>
#include <epos_comasuplinforequestlist.h>
#include <epos_comasuplinforequestlist.h>
#include <epos_comasuplreqasstdata.h>
#include <epos_comasuplsetcapabilities.h>
#include <epos_comasuplposition.h>
#include <epos_comasuplpospayload.h>
#include <epos_comasuplvelocity.h>
#include <epos_eomasuplposerrors.h>
#include <ecom/ecom.h>
#include "epos_comasuplpostesterlogger.h"
#include "epos_comasupltesthandler.h"
#include "epos_comasuplgetsuplinfotest.h"
#include "epos_comasuplpostestercategory.h"
#include "epos_comasuplpossessioncreationtest.h"
#include "epos_comasuplgetsuplinfomanager.h"

/** Constant used for logging information about test*/
_LIT8(KGetSuplInfoTestComplete, "Info: GetSuplInfoL Test Complete.");

/** Constant used for logging information about test*/
_LIT8(KGetSuplInfoTestAborted, "Error: GetSuplInfoL Test Aborted");

_LIT8(KReqBeforeSessionInitialize, "Info: Requesting GetSuplInfo before \
Initializing POS session.");

_LIT8(KEndSessionRequest, "Info: GetSuplInfoL requested after \
Session End.");

_LIT8(KPosInactiveWarning, "Warning: POS session is not initialized.\
GetSuplInfoL() completed with error code: ");

_LIT8(KEndSessionWarning, "Warning: After End of POS session \
GetSuplInfoL() completed with error code: ");

_LIT8(KPosInactiveInfo, "Info: GetSuplInfoL() completed with status \
KErrOMASuplPosInActive");

_LIT8(KGetSuplInfoInfo, "Info: GetSuplInfoL() completed with status \
KErrNone.");

_LIT8(KRequestAfterSessionInit, "Info: Requesting GetSuplInfo after \
Initializing POS session.");

// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
COMASuplGetSuplInfoTest* COMASuplGetSuplInfoTest::NewL(
								COMASuplPosHandlerBase& aPosHandler
								, COMASuplPosTesterCategory* aCallBack )
	{
	COMASuplGetSuplInfoTest* self = new( ELeave )COMASuplGetSuplInfoTest(
												 aPosHandler, aCallBack );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
COMASuplGetSuplInfoTest::COMASuplGetSuplInfoTest(
							COMASuplPosHandlerBase& aPosHandler 
						, COMASuplPosTesterCategory* aCallBack)
						:COMASuplGetSuplInfoManager(aPosHandler, aCallBack)
	{
	CActiveScheduler::Add( this );
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoTest::ConstructL()
	{
	iRequestList = COMASuplInfoRequestList::NewL();
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplGetSuplInfoTest::~COMASuplGetSuplInfoTest()
	{
	}

// ---------------------------------------------------------------------------
// Ends a POS Session.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoTest::EndSession()
	{
	iInfo++;
	iLogger->WriteLine(KSessionEnd, iTestNo);
	iSessionBase->SessionEnd();
	}

// ---------------------------------------------------------------------------
// This function is called by test handler. It is entry point of execution
// of a test case.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoTest::StartTestingL()
	{
	if(iCallBack)
		{
		iLogger->WriteTestHeader(KGetSuplInfoTest, iTestNo);
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
void COMASuplGetSuplInfoTest::RunL()
	{
	switch( iNextInSeq )
		{
		case EStartTest:
			{
			iLogger->GetRequestStartTime();
			TRAPD( err, iSessionBase = iPosHandler.CreateNewSessionL( this ) );
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
			iNextInSeq = ERequestBeforeInitialize;
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case ERequestBeforeInitialize:
			{
			ProcessRequestBeforeInitializeL();
			break;
			}
		case ERequestBeforeInitializeComplete:
			{
			ProcessRequestBeforeInitializeCompleteL();
			break;
			}
		case EInitializeSession:
			{
			ProcessInitializeSessionL();
			break;
			}
		case EInitializationComplete:
			{
			ProcessInitializationCompleteL();
			break;
			}
		case ERequestGetSuplInfo:
			{
			ProcessRequestGetSuplInfoL();
			break;
			}
		case EGetSuplInfoLRequestComplete:
			{
			ProcessGetSuplInfoComplete();
			break;
			}
		case EEndSessionAndRequestGetsuplInfo:
			{
			ProcessEndSessionAndRequestGetsuplInfoL();
			break;
			}
		case EEndSessionRequestComplete:
			{
			ProcessEndSessionRequestComplete();
			break;
			}
		case ETestingComplete:
			{
			TestingComplete(KGetSuplInfoTestComplete);
			break;
			}
		default: 
			{
			User::Leave(KErrNotFound);
			}
		}
	}

// ---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoTest::DoCancel()
	{
	if(iNextInSeq == ERequestBeforeInitializeComplete
		|| iNextInSeq == EGetSuplInfoLRequestComplete
		|| iNextInSeq == EEndSessionRequestComplete)
		{
		iSessionBase->CancelSuplInfoRequest();
		}
	else if(iNextInSeq == EInitializationComplete )
		{
		iSessionBase->CancelInitialize();
		}
	}

// ---------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL().
// ---------------------------------------------------------------------------
//
TInt  COMASuplGetSuplInfoTest::RunError( TInt aError )
	{
	
	if(iNextInSeq == EInitializationComplete )
		{
		iError++;
		iLogger->WriteStatus(KLeaveInSessionInit, aError, iTestNo);
		}
	else if(iNextInSeq == EStartTest)
		{
		iError++;
		iLogger->WriteStatus(KLeaveInCreateNewSession, aError, iTestNo);
		}
	else
		{
		iError++;
		iLogger->WriteStatus(KLeaveOccured, aError, iTestNo);
		}
	TestingAborted(KGetSuplInfoTestAborted);
	return KErrNone;
	}
	
// ---------------------------------------------------------------------------
// Requests for GetSuplInfoL before initializing session.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoTest::ProcessRequestBeforeInitializeL()
	{
	iNextInSeq = ERequestBeforeInitializeComplete;
	iInfo++;
	iLogger->WriteLine(KReqBeforeSessionInitialize, iTestNo);
	__ASSERT_ALWAYS( !IsActive(), User::Panic(KPanicSetActive, 1));
	iSessionBase->GetSuplInfoL(iRequestList, iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Logs the completion status of GetsuplinfoL.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoTest::ProcessRequestBeforeInitializeCompleteL()
	{
	iNextInSeq = EInitializeSession;
	TInt diff = -1;
	if(!MatchLists(diff))
		{
		iError++;
		iLogger->WriteStatus(KMatchError, diff, iTestNo, &KNULL());
		}
	if(iStatus != KErrOMASuplPosInActive)
		{
		iWarning++;
		iLogger->WriteStatus(KPosInactiveWarning, iStatus.Int(), iTestNo
																, &KNULL());
		iLogger->LogRequestList(*iRequestList);
		}
	else
		{
		iInfo++;
		iLogger->WriteLine(KPosInactiveInfo, iTestNo);
		}
	__ASSERT_ALWAYS( !IsActive(), User::Panic(KPanicSetActive, 1));
	DummyRequest(iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Initializes the POS session.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoTest::ProcessInitializeSessionL()
	{
	iNextInSeq = EInitializationComplete;
	iInfo++;
	iLogger->WriteLine(KInitializingSession, iTestNo);
	__ASSERT_ALWAYS( !IsActive(), User::Panic(KPanicSetActive, 1));
	iSessionBase->InitializeL(iRequestID, iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Logs the completion status of session initialization.
// Leaves if session initialization is not complete with KErrNone.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoTest::ProcessInitializationCompleteL()
	{
	if(iStatus != KErrNone)
		{
		iError++;
		iLogger->WriteStatus(KError, iStatus.Int(), iTestNo, &KReqCompWith());
		TestingAborted(KGetSuplInfoTestAborted);
		return;
		}
	iNextInSeq = ERequestGetSuplInfo;
	iInfo++;
	iLogger->WriteLine(KSessionInitialized, iTestNo);
	__ASSERT_ALWAYS(!IsActive(), User::Panic(KPanicSetActive, 1));
	DummyRequest(iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Requests for GetSuplInfoL().
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoTest::ProcessRequestGetSuplInfoL()
	{
	iNextInSeq = EGetSuplInfoLRequestComplete;
	TRAPD(err, CreateRequestListL());
	if(KErrNone != err)
		{
		iError++;
		iLogger->WriteStatus(KLeaveRequestListCreation(), err, iTestNo);
		TestingAborted(KGetSuplInfoTestAborted);
		return;
		}
	__ASSERT_ALWAYS(!IsActive(), User::Panic(KPanicSetActive, 1));
	iInfo++;
	iLogger->WriteLine(KRequestAfterSessionInit, iTestNo);
	iSessionBase->GetSuplInfoL(iRequestList, iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Logs the completion status of GetSplInfoL().
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoTest::ProcessGetSuplInfoComplete()
	{
	TInt diff = -1;
	if(!MatchLists(diff))
		{
		iError++;
		iLogger->WriteStatus(KMatchError, diff, iTestNo, &KNULL());
		}
	iNextInSeq = EEndSessionAndRequestGetsuplInfo;
	if(iStatus != KErrNone)
		{
		iWarning++;
		iLogger->WriteStatus(KPosInactiveWarning, iStatus.Int(), iTestNo
															, &KNULL());
		}
	else
		{
		iInfo++;
		iLogger->WriteLine(KGetSuplInfoInfo, iTestNo);
		}
	iLogger->LogRequestList(*iRequestList);
	__ASSERT_ALWAYS(!IsActive(), User::Panic(KPanicSetActive, 1));
	DummyRequest(iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Ends session and requests for GetSuplInfoL().
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoTest::ProcessEndSessionAndRequestGetsuplInfoL()
	{
	EndSession();
	//Recreate request list
	TRAPD(err, CreateRequestListL());
	if(KErrNone != err)
		{
		iError++;
		iLogger->WriteStatus(KLeaveRequestListCreation(), err, iTestNo);
		TestingAborted(KGetSuplInfoTestAborted);
		return;
		}
	iNextInSeq = EEndSessionRequestComplete;
	__ASSERT_ALWAYS(!IsActive(), User::Panic(KPanicSetActive, 1));
	iInfo++;
	iLogger->WriteLine(KEndSessionRequest, iTestNo);
	iSessionBase->GetSuplInfoL( iRequestList, iStatus );
	SetActive();
	}

// ---------------------------------------------------------------------------
// Logs completion status of GetSuplInfoL requested after 
// ending the session.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoTest::ProcessEndSessionRequestComplete()
	{
	TInt diff = -1;
	if(!MatchLists(diff))
		{
		iError++;
		iLogger->WriteStatus(KMatchError, diff, iTestNo, &KNULL());
		}
	iNextInSeq = ETestingComplete;
	if(iStatus != KErrOMASuplPosInActive)
		{
		iWarning++;
		iLogger->WriteStatus(KEndSessionWarning, iStatus.Int(), iTestNo
														, &KNULL());
		}
	else
		{
		iInfo++;
		iLogger->WriteLine(KPosInactiveInfo, iTestNo);
		}
	//Logging the request list.
	iLogger->LogRequestList(*iRequestList);
	__ASSERT_ALWAYS(!IsActive(), User::Panic(KPanicSetActive, 1));
	DummyRequest(iStatus);
	SetActive();
	}
