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

#include <e32def.h>
#include <e32keys.h>
#include "epos_comasupltesthandler.h"
#include "epos_comasuplposhandlercreationtest.h"
#include "epos_comasuplpostesterlogger.h"

// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
COMASuplTestHandler* COMASuplTestHandler::NewL(CConsoleBase& aConsole)
	{
	COMASuplTestHandler* self = new(ELeave)COMASuplTestHandler(aConsole);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplTestHandler::~COMASuplTestHandler()
	{
	Cancel();
	iConsole.ClearScreen();
	}

// ---------------------------------------------------------------------------
// Provides object of observer to handle test states.
// ---------------------------------------------------------------------------
//
void COMASuplTestHandler::InitializeL(MOMASuplTestObserver* aCancelObserver)
	{
	iCancelObserver = aCancelObserver;
	}	

// ---------------------------------------------------------------------------
// All the initializations are done and test is ready for execution.
// This function is called to start any test.
// ---------------------------------------------------------------------------
//	
void COMASuplTestHandler::StartTest()
	{
	iConsole.ClearScreen();
	iConsole.Printf(_L("Press Cancel Key to cancel Testing"));
	iTestStarted = EFalse;
	Cancel();
	if(!IsActive())
		{
		DummyRequest(iStatus);
		SetActive();
		}
	CActiveScheduler::Start();
	}

// ---------------------------------------------------------------------------
// Handles an active object's request completion event.
// ---------------------------------------------------------------------------
//
void COMASuplTestHandler::RunL()
	{
	if(iStatus == KErrNone)
		{
		if(iStatus == KErrNone)
			{
			//Get Key code & Process it. User::Request()
			TKeyCode keyPressed = iConsole.KeyCode();
			if(EKeyBackspace == keyPressed)
				{
				iCancelObserver->TestingCancelled(KUserCancelsTest);
				}
			else
				{
				__ASSERT_ALWAYS(!IsActive(), User::Panic(_L("Already active"), 1));
				iConsole.Read(iStatus);
				SetActive();
				if(!iTestStarted)
					{
					iTestStarted = ETrue;
					TRAPD(err, iCancelObserver->StartTestingL());
					if(err != KErrNone)
						{
						User::Leave(err);
						}
					}		
				}
			}
		}
	else
		{
		User::Leave(KErrNotFound);
		}
	}

// ---------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
// ---------------------------------------------------------------------------
//
void COMASuplTestHandler::DoCancel()
	{
	iConsole.ReadCancel();
	}

// ---------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL().
// ---------------------------------------------------------------------------
//
TInt COMASuplTestHandler::RunError( TInt /*aError*/)
	{
	iConsole.ReadCancel();
	iCancelObserver->TestingAborted(KTestAborted);
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// Parameterized constructor
// ---------------------------------------------------------------------------
//	
COMASuplTestHandler::COMASuplTestHandler(CConsoleBase& aConsole):CActive(EPriorityStandard)
							,iConsole(aConsole)
	{
	iTestStarted = EFalse;
	iStatus = KRequestPending;
	CActiveScheduler::Add(this);
	}

// ---------------------------------------------------------------------------
// Issues a dummy asynchronous request. It is used to execute cases in RunL.
// ---------------------------------------------------------------------------
//
void COMASuplTestHandler::DummyRequest( TRequestStatus& aStatus )
	{
	aStatus = KRequestPending;
	TRequestStatus* status = &aStatus;
	User::RequestComplete( status, KErrNone );
	}
