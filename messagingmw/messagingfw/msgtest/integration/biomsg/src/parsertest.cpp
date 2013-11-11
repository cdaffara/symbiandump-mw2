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

#include "parsertest.h"
#include <biocmtm.h>
#include <bioscmds.h>
#include <bitsids.h>

#include <bsp.h>
#include "iacp.h"

#include "iaptest.h"
//#include "wapptest.h"

const TUid KUidMsgTypeSmartMessage  = {0x10001262};

TestUiTimer* TestUiTimer::NewLC(CSmsTestUtils& aTestUtils)
	{
	TestUiTimer* self = new(ELeave) TestUiTimer(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

TestUiTimer::TestUiTimer(CSmsTestUtils& aTestUtils)
	: CTimer(5), iSmsTest(aTestUtils)
	{
	period = KPeriod;
	}

TestUiTimer::~TestUiTimer()
	{
	delete iOperation;
	}

void TestUiTimer::ConstructL()
	{
	CTimer::ConstructL();
	CActiveScheduler::Add(this);
	}

void TestUiTimer::IssueRequest()
	{
	After(period);
	}

void TestUiTimer::SetOperation(CMsvOperation *aOperation)
	{
	if(iOperation)
		{
		delete iOperation;
		iOperation=NULL;
		}
	iOperation = aOperation;
	}

void TestUiTimer::DoCancel()
	{
	CTimer::DoCancel();
	}

void TestUiTimer::RunL()
	{
	// display the current progress
	if(iOperation!=NULL)
		{
		TBioProgress temp;	
		TPckgC<TBioProgress> paramPack(temp);

		const TDesC8& progBuf = iOperation->ProgressL();	
		paramPack.Set(progBuf);
//		TBioProgress progress=paramPack();	

//		iSmsTest.Printf(TRefByValue<const TDesC>_L("   Operation: %d"),progress.iBioState);
//		iSmsTest.Printf(TRefByValue<const TDesC>_L("   Progress:  Error: %d\n"),
//					progress.iErrorCode);
		}
	IssueRequest();
	};

CParserTest::~CParserTest()
	{
	delete iBioClientMtm;

	iBioMessages = NULL;
	delete iBioMessages;

	delete iSelection;
	delete iTimer;

	delete iIapTester;
//	delete iWapTester;

	delete iClientMtmRegistry;
	}


CParserTest* CParserTest::NewL(CSmsTestUtils& aTestUtils)
	{
	CParserTest* self = new (ELeave) CParserTest(aTestUtils);
	CleanupStack::PushL(self);

	self->ConstructL();

	CleanupStack::Pop();
	return self;
	}



CParserTest::CParserTest(CSmsTestUtils& aTestUtils)
: iCurrentMessageNum(-1),iSmsTest(aTestUtils)
	{
	}

void CParserTest::StartL(CMsvEntrySelection* aReceived)
	{
	iCurrentTest = 0;
	iAllTestsSuccessful = ETrue;
	
	iSmsTest.TestStart(++iCurrentTest, _L("GetBioMessages"));

	iBioMessages = aReceived;

	TInt numMessages = iBioMessages->Count();
	// 
	iCurrentTest++;

	for(TInt i =0; i < numMessages; i++)
		{
		iSelection->Reset();
		iSelection->AppendL((*iBioMessages)[i]);
		MsvEntry().SetEntryL((*iBioMessages)[i]);
		iSmsTest.Printf(_L("\nMessage type %d\n"),MsvEntry().Entry().Id());
		iSmsTest.TestStart(iCurrentTest,_L("Parse and process"));
		TRAPD(error,CreateParserL());

		ProcessError(error);
		if(error == KErrNone)
			{
			iSmsTest.TestFinish(iCurrentTest);
			iCurrentTest++;
			iSmsTest.TestStart(iCurrentTest,_L("Reparse"));
			TRAP(error,ReParseL());
			ProcessError(error);
			iSmsTest.TestFinish(iCurrentTest,error);
			if(error == KErrNone)
				{
				iCurrentTest++;
				iSmsTest.TestStart(iCurrentTest,_L("Reprocess"));
				TRAP(error,ReProcessL());
				ProcessError(error);
				if(error==KErrNone)
					{
					iSmsTest.TestFinish(iCurrentTest);
					TRAP(error,CheckMessageTypeL());
					}
				else
					{
					iSmsTest.TestFinish(iCurrentTest,error);
					iAllTestsSuccessful = EFalse;
					}
				}
			}
		else
			{
			iSmsTest.TestFinish(iCurrentTest,error);
			iAllTestsSuccessful = EFalse;
			}
		}
	iSmsTest.TestFinish(iCurrentTest, KErrNone);
	}

void CParserTest::ConstructL()
	{
	iSelection = new(ELeave)CMsvEntrySelection();
	// create our mtm
	iClientMtmRegistry = CClientMtmRegistry::NewL(Session());
	iBioClientMtm = (CBIOClientMtm*) iClientMtmRegistry->NewMtmL(KUidMsgTypeSmartMessage);

	iTimer = TestUiTimer::NewLC(iSmsTest);
	CleanupStack::Pop();	// iTimer
	}

void CParserTest::CreateParserL()
	{
	TBuf8<1> aParameter;
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->iStatus = KRequestPending;

	CMsvOperation* operation = iBioClientMtm->InvokeAsyncFunctionL( KBiosMtmParseThenProcess, *iSelection, aParameter, wait->iStatus);
	
	iTimer->SetOperation(operation);
	iTimer->IssueRequest();
	
	wait->Start();
	CActiveScheduler::Start();

	TInt error = operation->iStatus.Int();
	iTimer->Cancel();
	CleanupStack::PopAndDestroy();	// wait
	if(error !=KErrNone)
		User::Leave(error);
	}

void CParserTest::ReParseL()
	{
	TBuf8<1> parameter;

	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->iStatus = KRequestPending;

	CMsvOperation* operation = iBioClientMtm->InvokeAsyncFunctionL( KBiosMtmParse, *iSelection, parameter, wait->iStatus);

	iTimer->SetOperation(operation);
	iTimer->IssueRequest();

	wait->Start();
	CActiveScheduler::Start();
	iTimer->Cancel();
	TInt error = operation->iStatus.Int();
	
	CleanupStack::PopAndDestroy();	// wait
	if(error !=KErrNone)
		User::Leave(error);
	}

void CParserTest::ReProcessL()
	{
	TBuf8<1> aParameter;
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->iStatus = KRequestPending;

	CMsvOperation* operation = iBioClientMtm->InvokeAsyncFunctionL( KBiosMtmProcess, *iSelection, aParameter, wait->iStatus);
	
	iTimer->SetOperation(operation);
	iTimer->IssueRequest();
	
	wait->Start();
	CActiveScheduler::Start();

	TInt error = operation->iStatus.Int();
	iTimer->Cancel();
	
	CleanupStack::PopAndDestroy();	// wait
	if(error !=KErrNone)
		User::Leave(error);
	}

void CParserTest::ProcessError(TInt& aError)
	{
	if(aError==KErrNotSupported)
		{
		aError = KErrNone;
		iSmsTest.WriteComment(_L("Unsupported operation"));
		}
	else if((aError <= -500 && aError >= -521) || (aError <= -600 && aError >= -614))
		{
		aError = KErrNone;
		iSmsTest.WriteComment(_L("Corrupt or missing Bio-data"));
		}
	}

void CParserTest::CheckMessageTypeL()
	{
	// get type
	MsvEntry().SetEntryL((*iSelection)[0]);
	TMsvEntry entry = MsvEntry().Entry();
	if(entry.iBioType == KUidBIOInternetAccessPointMsg)
		{
		CIacSettingsParser::TSmsType type = CIacSettingsParser::TSmsType(entry.MtmData1());
		if(type == CIacSettingsParser::EBasicMailIAP)
			{
			TestInternetAccessL();
			}
		}
	else if(entry.iBioType == KUidBIOWAPAccessPointMsg)
		{
//		TestWapAccessL();
		}


	}

void CParserTest::TestInternetAccessL()
	{
	CInternetAccessTester* iapTester = CInternetAccessTester::NewL(iSmsTest);
	CleanupStack::PushL(iapTester);
	iapTester->StartL(MsvEntry().Entry().Id());	

	CleanupStack::PopAndDestroy();	// iapTester
	}

void CParserTest::TestWapAccessL()
	{
//	CWapAccessTester* wapTester = CWapAccessTester::NewL(iSmsTest);
//	CleanupStack::PushL(wapTester);
//	wapTester->StartL(MsvEntry().Entry().Id());
//	CleanupStack::PopAndDestroy();	// wapTester
	}
