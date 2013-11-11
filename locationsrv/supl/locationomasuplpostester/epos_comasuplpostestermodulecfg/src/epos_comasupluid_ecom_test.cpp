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
* Description:  Tests loading of correct implementation.
*
*/

#include <ecom/ecom.h>
#include <ecom/implementationinformation.h>
#include <epos_comasuplposhandlerbase.h>

#include "epos_comasupluid_ecom_test.h"
#include "epos_comasuplpostesterlogger.h"
#include "epos_comasupltesthandler.h"


// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
COMASuplUid_ECom_Test* COMASuplUid_ECom_Test::NewL(
							CImplementationInformation& aImplInfo
							, COMASuplPosTesterCategory* aCallBack)
	{
	COMASuplUid_ECom_Test* self = new(ELeave)COMASuplUid_ECom_Test(
									aImplInfo, aCallBack);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplUid_ECom_Test::~COMASuplUid_ECom_Test()
	{
	Cancel();
	}

// ---------------------------------------------------------------------------
// This function is called by test handler. It is entry point of execution
// of a test case.
// ---------------------------------------------------------------------------
//
void COMASuplUid_ECom_Test::StartTestingL()
	{
	if(iCallBack)
		{
		iLogger->WriteTestHeader(KUid_ECom_Test, iTestNo);
		}
	__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
	DummyRequest(iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Handles an active object's request completion event.
// ---------------------------------------------------------------------------
//
void COMASuplUid_ECom_Test::RunL()
	{
	TInt err = KErrNone;
	COMASuplPosHandlerBase* posHandler = NULL;
	TRAP(err, posHandler = COMASuplPosHandlerBase::NewL(iImplInfo->ImplementationUid()));
	if(KErrNone != err)
		{
		iError++;
		iLogger->WriteLine(KHandlerCreateFail, iTestNo);
		User::Leave(err);
		}
		
	//Get Uid of loaded Impl
	TUid implUid = posHandler->ImplementationUID();
	
	delete posHandler;
	//Match it with the one loaded
	if(implUid == iImplInfo->ImplementationUid())
		{
		//OK
		iInfo++;
		iLogger->WriteLine(_L8("Info: Loaded Uid And Fetched Uid matched."), iTestNo);
		}
	else
		{
		//error
		iError++;
		iLogger->WriteLine(_L8("Error: Loaded Uid And Fetched Uid DO NOT match."), iTestNo);
		TestingAborted(KUid_ECom_TestAborted);
		}
	//Testing complete
	TestingComplete(KUid_ECom_TestComplete);
	}

// ---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void COMASuplUid_ECom_Test::DoCancel()
	{
	}

// ---------------------------------------------------------------------------
// Handles leave from RunL()
// ---------------------------------------------------------------------------
//	
TInt COMASuplUid_ECom_Test::RunError(TInt /*aError*/)
	{
	TestingAborted(KUid_ECom_TestAborted);
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// Parameterized constructor
// ---------------------------------------------------------------------------
//
COMASuplUid_ECom_Test::COMASuplUid_ECom_Test(
								CImplementationInformation& aImplInfo
								, COMASuplPosTesterCategory* aCallBack)
								:COMASuplPosTesterCategory(aCallBack)
	{
	iImplInfo = &aImplInfo;
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplUid_ECom_Test::ConstructL()
	{
	CActiveScheduler::Add(this);
	}
