// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include "emailsmoke.h"
#include "testframeutils.h"
#include "emailsmokeparse.h"
#include "emailsmokecommands.h"
#include "emailtestutils.h"


//
//
// CBaseTestHarness
//

CBaseTestHarness::~CBaseTestHarness()
	{
	iStateList->ResetAndDestroy();
	delete iStateList;
	}

void CBaseTestHarness::StartL(TRequestStatus& aStatus)
	{
	TTestDebugInfo debugInfo = GetDebugInfo();
	HBufC* startString = debugInfo.TestStartStringLC();
	LogComment(*startString);
	TestConsole().Printf(*startString);
	TestConsole().Printf(_L("\n"));

	CleanupStack::PopAndDestroy(startString);
	iCurrentState = 0;
	iReportStatus = &aStatus;
	
	if (iCurrentState < iStateList->Count())
		{
		TTestDebugInfo moreInfo=(*iStateList)[iCurrentState]->GetDebugInfo();
		HBufC* debugString=moreInfo.TestStartStringLC();
		TestConsole().Printf(*debugString);
		TestConsole().Printf(_L("\r"));
		CleanupStack::PopAndDestroy(debugString);

		(*iStateList)[iCurrentState]->StartL(iStatus);
		}

	aStatus = KRequestPending;
	SetActive();
	}


void CBaseTestHarness::AddStateL(MBaseTestState* aTestState, TTestDebugInfo aDebugInfo)
	{
	aTestState->SetDebugInfo(aDebugInfo);
	TRAPD(err, iStateList->AppendL(aTestState));
	if (err != KErrNone)
		// If the state can't be added then delete it.
		// We do this because we have taken ownership of the state.
		{
		delete aTestState;
		User::Leave(err);
		}
	}


// From CActive
void CBaseTestHarness::RunL()
	{
	if (iStatus == KErrNone)
		{
		if (++iCurrentState < iStateList->Count())
			{
			TTestDebugInfo moreInfo=(*iStateList)[iCurrentState]->GetDebugInfo();
			HBufC* debugString=moreInfo.TestCompleteStringLC();
			TestConsole().Printf(*debugString);
			TestConsole().Printf(_L("\r"));
			CleanupStack::PopAndDestroy(debugString);

			(*iStateList)[iCurrentState]->StartL(iStatus);
			SetActive();
			}
		else
			{
			TTestDebugInfo debugInfo = GetDebugInfo();
			HBufC* testCompleted = debugInfo.TestCompleteStringLC();
			LogComment(*testCompleted);

			CleanupStack::PopAndDestroy(testCompleted);
			User::RequestComplete(iReportStatus, iStatus.Int());
			}
		}
	else
		{
		// Pass the debug info back from the child state and log the error
		TTestDebugInfo debugInfo = (*iStateList)[iCurrentState]->GetDebugInfo();
		SetDebugInfo(debugInfo);
		HBufC* errorText = debugInfo.TestFailedStringLC(iStatus.Int());
		LogComment(*errorText);
		TestConsole().Printf(*errorText);
		TestConsole().Printf(_L("\r"));
							  
		CleanupStack::PopAndDestroy(errorText);
		User::RequestComplete(iReportStatus, iStatus.Int());
		}
	}

void CBaseTestHarness::DoCancel()
	{
	}

void CBaseTestHarness::LogComment(const TDesC& /* aError */)
	{
	}


TInt CBaseTestHarness::RunError(TInt aError)
	{
	// Pass the debug info back from the child state and log the error
	TTestDebugInfo debugInfo = (*iStateList)[iCurrentState]->GetDebugInfo();
	SetDebugInfo(debugInfo);
	HBufC* errorText=NULL;
	TRAPD(ret,errorText= debugInfo.TestFailedStringLC(aError));
	if (ret!=KErrNone)
		return ret;
	TRAP(ret,LogComment(*errorText)); //LogComentL doesn't leave although it has L.  
	TestConsole().Printf(*errorText);
	TestConsole().Printf(_L("\r"));

	CleanupStack::PopAndDestroy(errorText);
	User::RequestComplete(iReportStatus, aError);
	return KErrNone;
	}

void CBaseTestHarness::ConstructL()
	{
	CActiveScheduler::Add(this);
	iStateList = new (ELeave) CArrayPtrFlat<MBaseTestState>(10);
	}

CBaseTestHarness::CBaseTestHarness(RTest& aTest) : CActive(EPriorityHigh), iTest(aTest)
	{
	}

RTest& CBaseTestHarness::TestConsole()
	{
	return iTest;
	}

//
//
// CMainTestHarness
//

CMainTestHarness* CMainTestHarness::NewL(RTest& aTest)
	{
	CMainTestHarness* self = new (ELeave) CMainTestHarness(aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CMainTestHarness::ConstructL()
	{
	CBaseTestHarness::ConstructL();
	}

CMainTestHarness::CMainTestHarness(RTest& aTest) : CBaseTestHarness(aTest)
	{
	}

void CMainTestHarness::StartL(TRequestStatus& aStatus)
	{
	CBaseTestHarness::StartL(aStatus);
	}


//
//
// CMsvClientTest
//


CMsvClientTest* CMsvClientTest::NewL(CEmailTestUtils& aTestUtils, RTest& aTest)
	{
	CMsvClientTest* self = new (ELeave) CMsvClientTest(aTestUtils, aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CMsvClientTest* CMsvClientTest::NewL(CEmailTestUtils* aTestUtils, RTest& aTest)
	{
	CMsvClientTest* self = new (ELeave) CMsvClientTest(aTestUtils, aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CMsvClientTest::~CMsvClientTest()
	{
	Cancel();
	delete iCurrentSelection;
	delete iOperation;
	}

void CMsvClientTest::DoCancel()
	{
	if (iOperation)
		{
		iOperation->Cancel();
		}
	}

void CMsvClientTest::SetCurrentOperation(CMsvOperation* aOperation)
	{
	delete iOperation;
	iOperation = aOperation;
	}


CMsvOperation& CMsvClientTest::CurrentOperation()
	{
	return *iOperation;
	}

void CMsvClientTest::ConstructL()
	{
	CBaseTestHarness::ConstructL();
	iCurrentSelection = new (ELeave) CMsvEntrySelection;
	}

CMsvClientTest::CMsvClientTest(CEmailTestUtils& aMsvTestUtils, RTest& aTest) 
: CBaseTestHarness(aTest), 
  iMsvTestUtils(aMsvTestUtils)
	{
	iOperation = NULL;
	}

CMsvClientTest::CMsvClientTest(CEmailTestUtils* aMsvTestUtils, RTest& aTest) 
: CBaseTestHarness(aTest), 
  iMsvTestUtils(*aMsvTestUtils),
  iOwnedMsvTestUtils(aMsvTestUtils)
	{
	iOperation = NULL;
	}

CEmailTestUtils& CMsvClientTest::MsvTestUtils()
	{
	return iMsvTestUtils;
	}


//
//
// CEmailClientTest
//

CEmailClientTest* CEmailClientTest::NewL(CEmailTestUtils* aTestUtils, RTest& aTest)
	{
	CEmailClientTest* self = new (ELeave) CEmailClientTest(aTestUtils, aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CEmailClientTest* CEmailClientTest::NewL(CEmailTestUtils& aTestUtils, RTest& aTest)
	{
	CEmailClientTest* self = new (ELeave) CEmailClientTest(aTestUtils, aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CEmailClientTest::~CEmailClientTest()
	{
	delete iOwnedEmailTestUtils;
	}

void CEmailClientTest::ConstructL()
	{
	CMsvClientTest::ConstructL();
	}

CEmailTestUtils& CEmailClientTest::EmailTestUtils()
	{
	return iEmailTestUtils;
	}

CEmailClientTest::CEmailClientTest(CEmailTestUtils& aTestUtils, RTest& aTest) 
: CMsvClientTest(aTestUtils, aTest), 
  iEmailTestUtils(aTestUtils)
	{
	}

CEmailClientTest::CEmailClientTest(CEmailTestUtils* aTestUtils, RTest& aTest) 
: CMsvClientTest(*aTestUtils, aTest), 
  iEmailTestUtils(*aTestUtils),
  iOwnedEmailTestUtils(aTestUtils)
	{
	}

void CEmailClientTest::LogComment(const TDesC& aError)
	{
	iEmailTestUtils.WriteComment(aError);
	}

