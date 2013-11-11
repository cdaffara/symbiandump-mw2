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

#include "epos_comasuplsessiontest.h"
#include "epos_comasuplpostesterlogger.h"
#include "epos_comasupltesthandler.h"
#include "epos_comasuplgetsuplinfomanager.h"

/** Constant used for logging information about test*/
_LIT8(KSessionTestComplete, "Info: Session Test Complete.");

/** Constant used for logging information about test*/
_LIT8(KSessionTestAborted, "Error: Session  Test Aborted");

/** Constant used for logging information about test*/
_LIT8(KReqBeforeSessionInitialize, "Info: Requesting GetSuplInfo before \
Initializing POS session.");

/** Constant used for logging information about test*/
_LIT8(KPositionReqBeforeSessionInitialize, "Info: Requesting GetPositionL\
 before initializing POS session.");

/** Constant used for logging information about test*/
_LIT8(KGetSuplInfoSessionEndRequest, "Info: GetPositionL requested after \
Session End.");

/** Constant used for logging information about test*/
_LIT8(KGetSuplInfoWarning, "Warning: POS session is Not initialized.\
GetSuplInfoL() completed with KErrNone.");

/** Constant used for logging information about test*/
_LIT8(KPosInactiveInfo, "Info: GetSuplInfoL() completed with status \
KErrOMASuplPosInActive");

/** Constant used for logging information about test*/
_LIT8(KGetSuplInfoInfo, "Info: GetSuplInfoL() completed with status \
KErrNone.");

/** Constant used for logging information about test*/
_LIT8(KGetPositionInfo, "Info: GetPositionL() completed with status ");

/** Constant used for logging information about test*/
_LIT8(KGetPositionWarn, "Warning: GetPositionL() completed with status ");

/** Constant used for logging information about test*/
_LIT8(KGetSuplError, "Error: GetSuplInfoL() completed with status ");

/** Constant used for logging information about test*/
_LIT8(KGetPositionError, "Error: GetPositionL() completed with status ");

/** Constant used for logging information about test*/
_LIT8(KGetPositionInfoErr, "Error: Could not provide value to some \
mandatory fields. GetPositionL() completed with status ");

/** Constant used for logging information about test*/
_LIT8(KRequestAfterSessionInit, "Info: Requesting GetSuplInfo after \
Initializing POS session.");

/** Constant used for logging information about test*/
_LIT8(KGetPositionSessionInitRequest, "Info: Requesting for GetPositionL().\
 Session is initialized.");

/** Constant used for logging information about test*/
_LIT8(KPositionEndSessionRequest, "Info: Requesting for GetPositionL() \
 after ending POS Session.");

/** Constant used for logging information about test*/
_LIT8(KEndSession, "Info: Ending POS Session.");

/** Constant used for logging information about test*/
_LIT8(KEndSessionComplete, "Info: Ending POS Session complete.");

/** Constant used for logging information about test*/
_LIT8(KExistingSessionTest, "Info: Running test on an existing session.");

// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
COMASuplSessionTest* COMASuplSessionTest::NewL(
								COMASuplPosHandlerBase& aPosHandler
							, COMASuplPosTesterCategory* aCallBack )
	{
	COMASuplSessionTest* self = new( ELeave )COMASuplSessionTest( 
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
COMASuplSessionTest::COMASuplSessionTest(
							COMASuplPosHandlerBase& aPosHandler
							, COMASuplPosTesterCategory* aCallBack )
							:COMASuplGetSuplInfoManager(aPosHandler, aCallBack)
	{
	iSessionState = ESessionUninitialized;
	iExistingSessionTestDone = EFalse;
	CActiveScheduler::Add( this );
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplSessionTest::ConstructL()
	{
	iRequestList = COMASuplInfoRequestList::NewL();
	iPosition = COMASuplPosition::NewL();
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplSessionTest::~COMASuplSessionTest()
	{
	delete iPosition;
	}

// ---------------------------------------------------------------------------
// This function is called by test handler. It is entry point of execution
// of a test case.
// ---------------------------------------------------------------------------
//
void COMASuplSessionTest::StartTestingL()
	{
	if(iCallBack)
		{
		iLogger->WriteTestHeader(KSessionTest, iTestNo);
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
void COMASuplSessionTest::RunL()
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
			iNextInSeq = EGetSuplInfoSessionNotInitRequest;
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EGetSuplInfoSessionNotInitRequest:
			{
			iInfo++;
			iLogger->WriteLine(KReqBeforeSessionInitialize, iTestNo);
			iNextInSeq = EGetSuplInfoSessionNotInitRequestComplete;
			GetSuplInfoRequestL();
			break;
			}
		case EGetSuplInfoSessionNotInitRequestComplete:
			{
			iNextInSeq = EGetPositionSessionNotInitRequest;
			GetSuplInfoRequestCompleteL();
			__ASSERT_ALWAYS( !IsActive(), User::Panic(KPanicSetActive, 1));
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EGetPositionSessionNotInitRequest:
			{
			iNextInSeq = EGetPositionSessionNotInitRequestComplete;
			iInfo++;
			iLogger->WriteLine(KPositionReqBeforeSessionInitialize, iTestNo);
			GetPositionRequestL();
			break;
			}
		case EGetPositionSessionNotInitRequestComplete:
			{
			iNextInSeq = EInitializeSessionRequest;
			GetPositionRequestComplete();
			__ASSERT_ALWAYS( !IsActive(), User::Panic(KPanicSetActive, 1));
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EInitializeSessionRequest:
			{
			iNextInSeq = EinitializeSessionRequestComplete;
			iInfo++;
			iLogger->WriteLine(KInitializingSession, iTestNo);
			InitializeSessionRequestL();
			break;
			}
		case EinitializeSessionRequestComplete:
			{
			iNextInSeq = EGetSuplInfoSessionInitRequest;
			InitializeSessionRequestCompleteL();
			break;
			}
		case EGetSuplInfoSessionInitRequest:
			{
			iNextInSeq = EGetSuplInfoSessionInitRequestComplete;
			//log "Info: Requesting for GetSuplInfoL(). Session is initialized."
			iInfo++;
			iLogger->WriteLine(KRequestAfterSessionInit, iTestNo);
			GetSuplInfoRequestL();
			break;
			}
		case EGetSuplInfoSessionInitRequestComplete:
			{
			iNextInSeq = EGetPositionSessionInitRequest;
			GetSuplInfoRequestCompleteL();
			__ASSERT_ALWAYS( !IsActive(), User::Panic(KPanicSetActive, 1));
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EGetPositionSessionInitRequest:
			{
			iNextInSeq = EGetPositionSessionInitRequestComplete;
			iInfo++;
			iLogger->WriteLine(KGetPositionSessionInitRequest, iTestNo);
			GetPositionRequestL();
			break;
			}
		case EGetPositionSessionInitRequestComplete:
			{
			iNextInSeq = EEndSessionRequest;
			GetPositionRequestComplete();
			__ASSERT_ALWAYS( !IsActive(), User::Panic(KPanicSetActive, 1));
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EEndSessionRequest:
			{
			iNextInSeq = EEndSessionRequestComplete;
			//log "Info: Ending POS session."
			iInfo++;
			iLogger->WriteLine(KEndSession, iTestNo);
			EndSession();
			__ASSERT_ALWAYS( !IsActive(), User::Panic(KPanicSetActive, 1));
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EEndSessionRequestComplete:
			{
			iNextInSeq = EGetSuplInfoSessionEndRequest;
			//log "Info: POS Session Ended."
			iInfo++;
			iLogger->WriteLine(KEndSessionComplete, iTestNo);
			__ASSERT_ALWAYS( !IsActive(), User::Panic(KPanicSetActive, 1));
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EGetSuplInfoSessionEndRequest:
			{
			iNextInSeq = EGetSuplInfoSessionEndRequestComplete;
			//log "Requesting for GetSuplInfoL(). Session is ended."
			iInfo++;
			iLogger->WriteLine(KGetSuplInfoSessionEndRequest, iTestNo);
			GetSuplInfoRequestL();
			break;
			}
		case EGetSuplInfoSessionEndRequestComplete:
			{
			iNextInSeq = EGetPositionSessionEndRequest;
			GetSuplInfoRequestCompleteL();
			__ASSERT_ALWAYS( !IsActive(), User::Panic(KPanicSetActive, 1));
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EGetPositionSessionEndRequest:
			{
			iNextInSeq = EGetPositionSessionEndRequestComplete;
			iInfo++;
			iLogger->WriteLine(KPositionEndSessionRequest, iTestNo);
			GetPositionRequestL();
			break;
			}
		case EGetPositionSessionEndRequestComplete:
			{
			iNextInSeq = EExistingSessionTest;
			GetPositionRequestComplete();
			__ASSERT_ALWAYS( !IsActive(), User::Panic(KPanicSetActive, 1));
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case EExistingSessionTest:
			{
			if(iExistingSessionTestDone)
				{
				iNextInSeq = ETestingComplete;
				}
			else
				{
				iExistingSessionTestDone = ETrue;
				iInfo++;
				iLogger->WriteLine(KExistingSessionTest, iTestNo);
				iNextInSeq = EInitializeSessionRequest;
				}
			__ASSERT_ALWAYS( !IsActive(), User::Panic(KPanicSetActive, 1));
			DummyRequest(iStatus);
			SetActive();
			break;
			}
		case ETestingComplete:
			{
			TestingComplete(KSessionTestComplete);
			break;
			}
		}
	}

// ---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void COMASuplSessionTest::DoCancel()
	{
	if(EGetSuplInfoSessionNotInitRequestComplete == iNextInSeq
		|| EGetSuplInfoSessionInitRequestComplete == iNextInSeq
		||EGetSuplInfoSessionEndRequestComplete == iNextInSeq)
		{
		//Cancel GetSuplInfoL()
		iSessionBase->CancelSuplInfoRequest();
		}
	if(EGetPositionSessionNotInitRequestComplete == iNextInSeq
	 || EGetPositionSessionInitRequestComplete == iNextInSeq
	 || EGetPositionSessionEndRequestComplete == iNextInSeq)
		{
		// Cancel GetPositionL()
		iSessionBase->CancelGetPosition();
		}
	if(EinitializeSessionRequestComplete == iNextInSeq)
		{
		//Cancel session initialization
		iSessionBase->CancelInitialize();
		}
	}

// ---------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL().
// ---------------------------------------------------------------------------
//
TInt  COMASuplSessionTest::RunError( TInt aError )
	{
	if(EStartTest == iNextInSeq)
		{
		iError++;
		iLogger->WriteStatus(KLeaveInCreateNewSession, aError, iTestNo);
		}
	else if(EGetSuplInfoSessionNotInitRequestComplete == iNextInSeq
		|| EGetSuplInfoSessionInitRequestComplete == iNextInSeq
		||EGetSuplInfoSessionEndRequestComplete == iNextInSeq)
		{
		//Cancel GetSuplInfoL()
		iError++;
		iLogger->WriteStatus(KLeaveInGetSuplInfo, aError, iTestNo);
		}
	else if(EGetPositionSessionNotInitRequestComplete == iNextInSeq
	 || EGetPositionSessionInitRequestComplete == iNextInSeq
	 || EGetPositionSessionEndRequestComplete == iNextInSeq)
		{
		// Cancel GetPositionL()
		iError++;
		iLogger->WriteStatus(KLeaveInGetPosition, aError, iTestNo);
		}
	else if(EinitializeSessionRequestComplete == iNextInSeq)
		{
		//Cancel session initialization
		iError++;
		iLogger->WriteStatus(KLeaveInSessionInit, aError, iTestNo);
		}
	TestingAborted(KSessionTestAborted);
	return KErrNone;
	}
	
// ---------------------------------------------------------------------------
// Requests for GetSuplInfoL.
// ---------------------------------------------------------------------------
//
void COMASuplSessionTest::GetSuplInfoRequestL()
	{
	ResetLists();
	TRAPD(err, CreateRequestListL());
	if(KErrNone != err)
		{
		iError++;
		iLogger->WriteStatus(KLeaveRequestListCreation(), err, iTestNo);
		TestingAborted(KSessionTestAborted);
		return;
		}
	__ASSERT_ALWAYS( !IsActive(), User::Panic(KPanicSetActive, 1));
	iSessionBase->GetSuplInfoL(iRequestList, iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Logs the completion status of GetsuplinfoL.
// ---------------------------------------------------------------------------
//
void COMASuplSessionTest::GetSuplInfoRequestCompleteL()
	{
	TInt diff = -1;
	if(!MatchLists(diff))
		{
		iError++;
		iLogger->WriteStatus(KMatchError, diff, iTestNo, &KNULL());
		}

	switch (iStatus.Int())
		{
		case KErrNone:
			{
			if(ESessionInitialized == iSessionState)
				{
				//info
				iInfo++;
				iLogger->WriteLine(KGetSuplInfoInfo, iTestNo);
				}
			else 
				{
				//warning
				iWarning++;
				iLogger->WriteLine(KGetSuplInfoWarning, iTestNo);
				}
			iLogger->LogRequestList(*iRequestList);
			break;
			}
		case KErrOMASuplPosInActive:
			{
			if(ESessionInitialized == iSessionState)
				{
				//warning
				iWarning++;
				iLogger->WriteLine(_L8("Warning: GetSuplInfo completed with status KErrNone. Session is initialized."), iTestNo);
				}
			else 
				{
				//info
				iInfo++;
				iLogger->WriteLine(KPosInactiveInfo, iTestNo);
				}
			break;
			}
		case KErrOMASuplParamNotSet:
			{
			//warning
			iWarning++;
			iLogger->WriteLine(_L8("Warning: Parameter not set."), iTestNo);
			iLogger->LogRequestList(*iRequestList);
			break;
			}
		default:
			{
			//Error
			iWarning++;
			iLogger->WriteStatus(KGetSuplError(), iStatus.Int(), iTestNo
												, &KNULL());
			break;
			}
		}
	}

// ---------------------------------------------------------------------------
// Initializes the POS session.
// ---------------------------------------------------------------------------
//
void COMASuplSessionTest::InitializeSessionRequestL()
	{
	__ASSERT_ALWAYS( !IsActive(), User::Panic(KPanicSetActive, 1));
	// Set the status as session is initialized.
	iSessionState = ESessionInitialized;
	iSessionBase->InitializeL(iRequestID, iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Logs the completion status of session initialization.
// Leaves if session initialization is not complete with KErrNone.
// ---------------------------------------------------------------------------
//
void COMASuplSessionTest::InitializeSessionRequestCompleteL()
	{
	if(iStatus != KErrNone)
		{
		iError++;
		iLogger->WriteStatus(KError, iStatus.Int(), iTestNo, &KSessionInitResult());
		TestingAborted(KSessionTestAborted);
		return;
		}
	iInfo++;
	iLogger->WriteLine(KSessionInitialized, iTestNo);
	__ASSERT_ALWAYS(!IsActive(), User::Panic(KPanicSetActive, 1));
	DummyRequest(iStatus);
	SetActive();
	}

void COMASuplSessionTest::ResetPositionL()
	{
	delete iPosition;
	iPosition = NULL;
	iPosition = COMASuplPosition::NewL();
	}
// ---------------------------------------------------------------------------
// Requests for GetPositionL().
// ---------------------------------------------------------------------------
//
void COMASuplSessionTest::GetPositionRequestL()
	{
	TRAPD(err, ResetPositionL());
	if(KErrNone != err)
		{
		iError++;
		iLogger->WriteStatus(KLeaveResettingPosition(), err, iTestNo);
		TestingAborted(KSessionTestAborted);
		return;
		}
	__ASSERT_ALWAYS(!IsActive(), User::Panic(KPanicSetActive, 1));
	iSessionBase->GetPositionL(iStatus, iPosition);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Logs the completion status of GetPositionL().
// ---------------------------------------------------------------------------
//
void COMASuplSessionTest::GetPositionRequestComplete()
	{
	TInt diff = -1;
	if(!MatchLists(diff))
		{
		iError++;
		iLogger->WriteStatus(KMatchError, diff, iTestNo, &KNULL());
		}
	switch (iStatus.Int())
		{
		case KErrNone:
			{
			if(iSessionState != ESessionInitialized)
				{
				//warning
				iWarning++;
				iLogger->WriteStatus(KGetPositionWarn, iStatus.Int()
										, iTestNo, &KNULL());
				}
			else
				{
				iInfo++;
				iLogger->WriteStatus(KGetPositionInfo, iStatus.Int()
														, iTestNo, &KNULL());
				}
			break;
			}
		case KErrOMASuplPosInfo:
			{
			//Error
			iError++;
			iLogger->WriteStatus(KGetPositionInfoErr, iStatus.Int()
														, iTestNo, &KNULL());
			break;
			}
		case KErrOMASuplPosInActive:
			{
			if(iSessionState != ESessionInitialized)
				{
				//info
				iInfo++;
				iLogger->WriteStatus(KGetPositionInfo, iStatus.Int()
													, iTestNo, &KNULL());
				}
			else
				{
				//warning
				iWarning++;
				iLogger->WriteStatus(KGetPositionWarn, iStatus.Int()
													, iTestNo, &KNULL());
				}
			break;
			}
		default:
			{
			iError++;
			iLogger->WriteStatus(KGetPositionError, iStatus.Int()
													, iTestNo, &KNULL());
			break;
			}
		}
	iLogger->LogSuplPosition(iPosition);
	}

// ---------------------------------------------------------------------------
// Ends a POS Session.
// ---------------------------------------------------------------------------
//
void COMASuplSessionTest::EndSession()
	{
	__ASSERT_ALWAYS(!IsActive(), User::Panic(KPanicSetActive, 1));
	iSessionState = ESessionEnded;
	iSessionBase->SessionEnd();
	}
