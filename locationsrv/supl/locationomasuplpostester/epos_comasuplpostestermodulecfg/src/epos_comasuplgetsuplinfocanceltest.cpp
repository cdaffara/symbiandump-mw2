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
* Description:  Tests cancellation of GetSuplInfoL()
*
*/


#include <epos_comasuplposhandlerbase.h>
#include <epos_comasuplpossessionbase.h>
#include <epos_comasuplinforequestlist.h>
#include <epos_comasuplinforequestlist.h>
#include <epos_comasuplreqasstdata.h>
#include <epos_comasuplinforequestlist.h>
#include <epos_comasuplsetcapabilities.h>
#include <epos_comasuplposition.h>
#include <epos_comasuplpospayload.h>
#include <epos_comasuplvelocity.h>
#include <epos_eomasuplposerrors.h>
#include <ecom/ecom.h>
#include "epos_comasuplpostesterlogger.h"
#include "epos_comasupltesthandler.h"

#include "epos_comasuplgetsuplinfocanceltest.h"
#include "epos_comasuplpossessioncreationtest.h"
#include "epos_comasuplgetsuplinfomanager.h"


_LIT8(CancelNoInitialize, "Info: Calling Cancel of GetSuplInfoL before \
Initializing session and calling GetSuplInfoL.");

_LIT8(CancelNoGetSuplInfo, "Info: Calling Cancel of GetSuplInfoL before \
calling GetSuplInfoL.");

_LIT8(CancelGetSuplInfo, "Info: Calling Cancel of GetSuplInfoL after calling\
 GetSuplInfoL.");

_LIT8(CancelGetSuplInfoAgain, "Info: Calling Cancel of GetSuplInfoL after \
canceling GetSuplInfoL.");

_LIT8(CancelAfterEndSession, "Info: Calling Cancel of GetSuplInfoL after \
ending session.");

_LIT8(KCancelSuccess, "Info: GetSuplInfoL() canceled successfully.");

_LIT8(KCompNoneW, "Warning:GetSuplInfoL completed with KErrNone.");
_LIT8(KCompInActive,
	"Warning: GetSuplInfoL completed with KErrOMASuplPosInActive.");

_LIT8(KCompPosInfoW,
		"Warning:GetSuplInfoL completed with KErrOMASuplPosInfo.");

// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//	
COMASuplGetSuplInfoCancelTest* COMASuplGetSuplInfoCancelTest::NewL(
								COMASuplPosHandlerBase& aPosHandler
								, COMASuplPosTesterCategory* aCallBack )
	{
	COMASuplGetSuplInfoCancelTest* self 
					= new( ELeave )COMASuplGetSuplInfoCancelTest(
							aPosHandler
							, aCallBack);
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
COMASuplGetSuplInfoCancelTest::COMASuplGetSuplInfoCancelTest(
							COMASuplPosHandlerBase& aPosHandler
							, COMASuplPosTesterCategory* aCallBack )
						:COMASuplGetSuplInfoManager(aPosHandler, aCallBack)
							
	{
	CActiveScheduler::Add( this );
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoCancelTest::ConstructL()
	{
	iRequestList = COMASuplInfoRequestList::NewL();
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplGetSuplInfoCancelTest::~COMASuplGetSuplInfoCancelTest()
	{
	
	}

// ---------------------------------------------------------------------------
// Ends a POS Session.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoCancelTest::EndSession(const TDesC8& aInfo)
	{
	iInfo++;
	iLogger->WriteLine(aInfo, iTestNo);
	iSessionBase->SessionEnd();
	}

// ---------------------------------------------------------------------------
// CancelSuplInfoRequest() is called without giving any GetSuplInfoL request.
// It is used for canceling GetSuplInfoL when request is not given.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoCancelTest::CancelWithoutRequest( TRequestStatus& aStatus, 
											TExecutionSequence aNextinSeq)
	{
	CancelGetInfoRequest(CancelNoGetSuplInfo);
	iInfo++;
	iLogger->WriteLine( _L8( "Info: Cancellation without Request." ) , iTestNo);
	iNextInSeq = aNextinSeq;
	DummyRequest( aStatus );
	}

// ---------------------------------------------------------------------------
// CancelSuplInfoRequest() is called without giving any GetSuplInfoL request.
// It is used for canceling GetSuplInfoL only when request is given.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoCancelTest::CancelGetInfoRequest(const TDesC8& aInfo)
	{
	iInfo++;
	iLogger->WriteLine(aInfo, iTestNo);
	iSessionBase->CancelSuplInfoRequest();
	}

// ---------------------------------------------------------------------------
// This function is called by test handler. It is entry point of execution
// of a test case.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoCancelTest::StartTestingL()
	{
	if(iCallBack)
		{
		iLogger->WriteTestHeader(KGetSuplInfoCancelTest, iTestNo);
		}
	iNextInSeq = EStartTest;
	__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
	DummyRequest(iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Writes log based on completion status of GetSuplInfoL().
// ---------------------------------------------------------------------------
//

void COMASuplGetSuplInfoCancelTest::ProcessRequestComplete()
	{
	switch(iStatus.Int())
		{
		case KErrOMASuplPosInfo:
			{
			iWarning++;
			iLogger->WriteLine(KCompPosInfoW, iTestNo);
			break;
			}
		case KErrOMASuplPosInActive:
			{
			iWarning++;
			iLogger->WriteLine(KCompInActive, iTestNo);
			break;
			}
		case KErrNone:
			{
			iWarning++;
			iLogger->WriteLine(KCompNoneW, iTestNo);
			break;
			}
		case KErrCancel:
			{
			iInfo++;
			iLogger->WriteLine(KCancelSuccess, iTestNo);
			break;
			}
		default:
			{
			iError++;
			iLogger->WriteStatus(_L8( "Error:GetSuplInfoL \
completed with " ), iStatus.Int(), iTestNo);
			break;
			}
		}
	iLogger->LogRequestList(*iRequestList);
	}

// ---------------------------------------------------------------------------
// Handles an active object's request completion event.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoCancelTest::RunL()
	{
	switch( iNextInSeq )
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
			//Cancel the request before initializing the session and giving
			//any request.
			CancelGetInfoRequest(CancelNoInitialize);
			iNextInSeq = EInitializeSession;
			iInfo++;
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			DummyRequest( iStatus );
			SetActive();
			break;
			}
		case EInitializeSession:
			{
			iNextInSeq = EInitializationComplete;
			iInfo++;
			iLogger->WriteLine(KInitializingSession, iTestNo);
			__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
			iSessionBase->InitializeL( iRequestID, iStatus );
			SetActive();
			break;
			}
		case EInitializationComplete:
			{
			ProcessInitComplete();
			break;
			}
		case ERequestGetSuplInfoAndCancel:
			{
			ProcessGetSuplInfoAndCancelL();
			break;
			}
		case EGetSuplInfoLRequestComplete:
			{
			ProcessGetSuplInfoRequestComplete();
			break;
			}
		case EEndSessionAndCancel:
			{
			ProcessEndSessionAndCancel();
			break;
			}
		case ETestingComplete:
			{
			TestingComplete(KGetSuplInfoCancelTestComplete);
			break;
			}
		default:
			{
			User::Leave(KErrGeneral);
			}
		}
	}
	
// ---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoCancelTest::DoCancel()
	{
	if( iNextInSeq == EInitializationComplete)
		{
		iSessionBase->CancelInitialize();
		}
	}

// ---------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL().
// ---------------------------------------------------------------------------
//
TInt COMASuplGetSuplInfoCancelTest::RunError( TInt aError )
	{
	if(EStartTest == iNextInSeq)
		{
		//Leave in initialization
		iError++;
		iLogger->WriteStatus(KLeaveInCreateNewSession, aError, iTestNo);
		}
	if(EInitializationComplete == iNextInSeq)
		{
		//Leave in initialization
		iError++;
		iLogger->WriteStatus(KLeaveInSessionInit, aError, iTestNo);
		}
	if(EGetSuplInfoLRequestComplete == iNextInSeq)
		{
		//Leave in GetSuplInfoL
		iError++;
		iLogger->WriteStatus(KLeaveInGetSuplInfo, aError, iTestNo);
		}
	TestingAborted(KGetSuplInfoCancelTestAborted);
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// If Session initialization completes with KErrNone, calls CancelGetSuplInfo
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoCancelTest::ProcessInitComplete()
	{
	if(iStatus == KErrNone)
		{
		iInfo++;
		iLogger->WriteLine(KSessionInitialized, iTestNo);
		__ASSERT_ALWAYS( !IsActive(),
						User::Panic( KPanicSetActive, 1 ) );
		CancelWithoutRequest(iStatus, ERequestGetSuplInfoAndCancel );
		SetActive();
		}
	else
		{
		iError++;
		iLogger->WriteStatus(KError, iStatus.Int(), iTestNo, &KSessionInitResult());
		TestingAborted(KGetSuplInfoCancelTestAborted);
		}				
	}


// ---------------------------------------------------------------------------
// Requests for GetSuplInfoL() and immediately cancels it.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoCancelTest::ProcessGetSuplInfoAndCancelL()
	{
	__ASSERT_ALWAYS( !IsActive(), User::Panic( KPanicSetActive, 1 ) );
	iNextInSeq = EGetSuplInfoLRequestComplete;
	iInfo++;
	iLogger->WriteLine(_L8("Info: Requesting GetSuplInfoL"), iTestNo);
	iSessionBase->GetSuplInfoL( iRequestList, iStatus );
	SetActive();
	CancelGetInfoRequest(CancelGetSuplInfo);
	}

// ---------------------------------------------------------------------------
// When GetSuplInfoL completes this function is executed. It provides 
// information about completion status of GetSuplInfoL.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoCancelTest::ProcessGetSuplInfoRequestComplete()
	{
	TInt diff = -1;
	if(!MatchLists(diff))
		{
		iError++;
		iLogger->WriteStatus(KMatchError, diff, iTestNo, &KNULL());
		}
	ProcessRequestComplete();
	CancelGetInfoRequest(CancelGetSuplInfoAgain);
	iNextInSeq = EEndSessionAndCancel;
	__ASSERT_ALWAYS( !IsActive(), User::Panic( KPanicSetActive, 1 ) );
	DummyRequest( iStatus );
	SetActive();
	}

// ---------------------------------------------------------------------------
// Ends session specified by iSessionBase and requests for CancelGetsuplInfo.
// ---------------------------------------------------------------------------
//
void COMASuplGetSuplInfoCancelTest::ProcessEndSessionAndCancel()
	{
	EndSession(KSessionEnd);
	CancelGetInfoRequest(CancelAfterEndSession);
	__ASSERT_ALWAYS( !IsActive(), User::Panic( KPanicSetActive, 1 ) );
	iNextInSeq = ETestingComplete;
	DummyRequest( iStatus );
	SetActive();
	}
