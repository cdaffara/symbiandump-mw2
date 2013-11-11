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
* Description:  Parent class of all test classes.
*
*/


#include <epos_momasuplobserver.h>
#include "epos_comasuplpostestercategory.h"
#include "epos_comasupltesthandler.h"
#include "epos_comasuplpostesterlogger.h"

_LIT8(KTerminateSession, "Error: Calling MOMASuplObserver::TerminateSession()");
_LIT8(KSuplVersion, "Info: Calling MOMASuplObserver::SuplVersion()");

// ---------------------------------------------------------------------------
// Default Constructor
// ---------------------------------------------------------------------------
//
COMASuplPosTesterCategory::COMASuplPosTesterCategory()
								:CActive(EPriorityStandard)
								, iTestNo(0)
	{
	}

// ---------------------------------------------------------------------------
// Parameterized Constructor
// ---------------------------------------------------------------------------
//
COMASuplPosTesterCategory::COMASuplPosTesterCategory(
								COMASuplPosTesterCategory* aCallBack)
								:CActive(EPriorityStandard)
								, iCallBack(aCallBack)
								, iTestNo(0)
	{
	}

// ---------------------------------------------------------------------------
// Initializes COMASuplPosTesterCategory. Must be called before
// starting test.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterCategory::InitializeL(COMASuplTestHandler* aTestHandler
								, COMASuplPosTesterLogger& aLogger
								, TInt aTestNo)
	{
	iTestHandler = aTestHandler;
	iLogger = &aLogger;
	iTestNo = aTestNo;
	}

// ---------------------------------------------------------------------------
// Provides default implementation required for cancellation of testing
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterCategory::TestingCancelled(const TDesC8& aInfo)
	{
	if(iCallBack)
		{
		HBufC8* infoBuf = NULL;
		TRAPD(err, infoBuf = HBufC8::NewL(aInfo.Length() + KExtraBuffer));
		if(err != KErrNone)
			{
			iCallBack->TestingCancelled(aInfo);
			}
		else
			{
			TPtr8 info = infoBuf->Des();
			info.Copy(aInfo);
			info.Append(KSessionId);
			info.AppendNum(iTestNo);
			iCallBack->TestingComplete(info);
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
		iLogger->WriteLine(aInfo, iTestNo);
		}
	
	}

// ---------------------------------------------------------------------------
//Provides default implementation required for completion of testing
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterCategory::TestingComplete(const TDesC8& aInfo)
	{	
	if(iCallBack)
		{
		HBufC8* infoBuf = NULL;
		TRAPD(err, infoBuf = HBufC8::NewL(aInfo.Length() + KExtraBuffer));
		if(err != KErrNone)
			{
			iCallBack->TestingComplete(aInfo);
			}
		else
			{
			TPtr8 info = infoBuf->Des();
			info.Copy(aInfo);
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
		iLogger->WriteLine(aInfo, iTestNo);
		}
	}

// ---------------------------------------------------------------------------
//Provides default implementation required for aborting testing
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterCategory::TestingAborted(const TDesC8& aError)
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
		iLogger->WriteLine(aError, iTestNo);
		}
	}

// ---------------------------------------------------------------------------
//Gets the status of test conducted
// ---------------------------------------------------------------------------
//
TTestingStatus COMASuplPosTesterCategory::GetTestingStatus()
	{
	return iTestingStatus;
	}

// ---------------------------------------------------------------------------
//Gets the number of warning, info and error messages generated during test.
// ---------------------------------------------------------------------------
//		
void COMASuplPosTesterCategory::GetTestSummary(TInt& aInfo, TInt& aWarning, TInt& aError)
	{
	aInfo = iInfo;
	aWarning = iWarning;
	aError = iError;
	}

// ---------------------------------------------------------------------------
//Gets the number of warning, info and error messages generated during test.
//This method is used to get summary during multiple session test.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterCategory::UpdateSessionTestSummary(TInt& aInfo
									, TInt& aWarning, TInt& aError)
	{
	iInfo += aInfo;
	iWarning += aWarning;
	iError += aError;
	}

// ---------------------------------------------------------------------------
//Default implementation of MOMASuplObserver::SuplVersion()
// ---------------------------------------------------------------------------
//
TReal COMASuplPosTesterCategory::SuplVersion()
	{
	iInfo++;
	iLogger->WriteLine(KSuplVersion, iTestNo);
	return 0;
	}

// ---------------------------------------------------------------------------
//Default implementation of MOMASuplObserver::TerminateSession()
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterCategory::TerminateSession()
	{
	iError++;
	iLogger->WriteLine(KTerminateSession, iTestNo);
	}

// ---------------------------------------------------------------------------
// Issues a dummy asynchronous request. It is used to execute cases in RunL.
// ---------------------------------------------------------------------------
//
void COMASuplPosTesterCategory::DummyRequest(TRequestStatus& aStatus)
	{
	aStatus = KRequestPending;
	TRequestStatus* status = &aStatus;
	User::RequestComplete( status, KErrNone );
	}
