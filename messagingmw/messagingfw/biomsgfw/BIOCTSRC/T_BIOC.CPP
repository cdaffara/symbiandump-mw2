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

#include "TESTENV.H"
#include <e32test.h>
#include <e32uid.h>

#include <msvruids.h>
#include <msvids.h>
#include <msvuids.h>

#include <mtmdef.h>

#include <txtrich.h>                            //      rich text stuff
#include <txtfmlyr.h>                           //      para format gubbins..

#include <bsp.h>
#include "biotestutils.h"



//
class TestScheduler : public CActiveScheduler
	{
	public:
	void Error (TInt aError) const;
	};

//
void TestScheduler::Error(TInt /*anError*/) const
	{
	CActiveScheduler::Stop();
	}


// local variables etc //

LOCAL_D RTest test(_L("BIOC.dll Testrig"));
LOCAL_D CTrapCleanup* theCleanup;
LOCAL_D RFs theFs;      
LOCAL_D TestScheduler* scheduler;

LOCAL_D TMsvSelectionOrdering   msvSelectionOrdering;

#define KPeriod 10000   // period of timer

//

//
class TestUiTimer : public CTimer
	{
public:
	static TestUiTimer* NewLC();
	~TestUiTimer();

	void RunL();
	void DoCancel();
	void ConstructL();
	void IssueRequest();
	void SetOperation(CMsvOperation*);
	TTimeIntervalMicroSeconds32 period;
protected:
	TestUiTimer();
	
	CMsvOperation*                          iOperation;
	};

//
TestUiTimer* TestUiTimer::NewLC()
	{
	TestUiTimer* self = new(ELeave) TestUiTimer();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

TestUiTimer::TestUiTimer()
	: CTimer(5)
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
//      delete iOperation;
//      iOperation = NULL;
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
		TBioProgress progress=paramPack();      

		test.Console()->SetPos(0, 13);
		test.Printf(TRefByValue<const TDesC>_L("   Operation: %d"),progress.iBioState);
		test.Printf(TRefByValue<const TDesC>_L("   Progress:  Error: %d\n"),
					progress.iErrorCode);

		}
	IssueRequest();
	};


//
class CNbscTester : public CBase
	{
public:
	  // Construction
	static CNbscTester* NewLC();

	  // Destruction
	~CNbscTester();

	  // Issue request
	void StartL();

	void TestFindBodyL(const TDesC& aText);
	void TestFindSubjectL(const TDesC& aText);

private:
	CNbscTester();
	void ConstructL();
	// functions
	void CreateParserL();
	void ReParseL();
	void ReProcessL();
	void ProcessError(TInt& aError);
	
	 // Data members defined by this class
	CMsvEntrySelection* iSelection;
	CMsvEntrySelection*     iMsvSelection;  // selection of test messages
	CBioTestUtils*          iBioTestUtils;
	CBIOClientMtm*          iBioClientMtm;

	TUint iMenuState;
	TUint iPrevState;

	TUint iParseState;
	TestUiTimer* iTimer;

	TInt            iNumMessages;
	TInt            iMsgCtr;
	TInt            iTestCtr;
	TBool           iAllTestsSuccessful;
	enum
		{
		EParserIdle,
		EParserCreate,
		EParserParse,
		EParserCommit
		};
	};

//
CNbscTester* CNbscTester::NewLC()
	{
	CNbscTester* self = new(ELeave) CNbscTester();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CNbscTester::CNbscTester()
	{
	}

void CNbscTester::ConstructL()
	{
	// create test utilities
	iBioTestUtils = CBioTestUtils::NewL(test, ETuGoClientSide | ETuCleanMessageFolder);

	test.Printf(_L("Creating services"));
	iBioTestUtils->CreateServicesL();

	User::After(2500000);
	
	iBioTestUtils->TestStart(1,_L("Instant client"));
	TRAPD(error,iBioTestUtils->InstantiateClientMtmsL(););
	iBioTestUtils->TestFinish(1,error);
	if(error)
		User::Leave(error);

	iBioClientMtm = iBioTestUtils->iBioClientMtm;
	
	iSelection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(iSelection);
	CleanupStack::Pop();    //iSelection
	iTimer = TestUiTimer::NewLC();
	CleanupStack::Pop();    // iTimer
	iParseState = EParserCreate;
	iAllTestsSuccessful = ETrue;
	}

CNbscTester::~CNbscTester()
	{
	delete iSelection;
	delete iMsvSelection;
	delete iTimer;
	delete iBioTestUtils;
	}

void CNbscTester::StartL()
    {
	iBioTestUtils->TestStart(iTestCtr,_L("Generate Test Messages"));
	iMsvSelection = iBioTestUtils->GenerateMessagesL();
	iNumMessages = iMsvSelection->Count();
	iMsgCtr = 0;
	// 
	iTestCtr++;

	for(TInt i =0; i < iNumMessages; i++)
		{
		iSelection->Reset();

		iSelection->AppendL((*iMsvSelection)[i]);

		iBioTestUtils->TestStart(iTestCtr,_L("Parse and process"));
		TRAPD(error,CreateParserL());
		ProcessError(error);
		if(error == KErrNone)
			{
			iBioTestUtils->TestFinish(iTestCtr);
			iTestCtr++;
			iBioTestUtils->TestStart(iTestCtr,_L("Reparse"));
			TRAP(error,ReParseL());
			ProcessError(error);
			iBioTestUtils->TestFinish(iTestCtr,error);
			if(error == KErrNone)
				{
				iTestCtr++;
				iBioTestUtils->TestStart(iTestCtr,_L("Reprocess"));
				TRAP(error,ReProcessL());
				ProcessError(error);
				if(error==KErrNone)
					iBioTestUtils->TestFinish(iTestCtr);
				else
					{
					iBioTestUtils->TestFinish(iTestCtr,error);
					iAllTestsSuccessful = EFalse;
					}
				}
			}
		else
			{
			iBioTestUtils->TestFinish(iTestCtr,error);
			iAllTestsSuccessful = EFalse;
			}

		iTestCtr++;
		iBioTestUtils->TestStart(iTestCtr,_L("test find in body"));
		TRAP(error,TestFindBodyL(_L("yashar")));
		iBioTestUtils->TestFinish(iTestCtr,error);

		iTestCtr++;
		iBioTestUtils->TestStart(iTestCtr,_L("test find in subject"));
		TRAP(error,TestFindSubjectL(_L("Configuration")));
		iBioTestUtils->TestFinish(iTestCtr,error);
		}
	if(iAllTestsSuccessful)
		iBioTestUtils->TestHarnessCompleted();
    }

void CNbscTester::ProcessError(TInt& aError)
	{
	if(aError==KErrNotSupported)
		{
		aError = KErrNone;
		iBioTestUtils->WriteComment(_L("Unsupported operation"));
		}
	else if((aError <= -500 && aError >= -521) || (aError <= -600 && aError >= -614))
		{
		aError = KErrNone;
		iBioTestUtils->WriteComment(_L("Corrupt or missing Bio-data"));
		}
	}

//
// Try a get a parser
//
void CNbscTester::CreateParserL()
	{
	TBuf8<1> aParameter;
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->iStatus = KRequestPending;
	wait->Start();
	
	CMsvOperation* operation = iBioClientMtm->InvokeAsyncFunctionL( KBiosMtmParseThenProcess, *iSelection, aParameter, wait->iStatus);
	
	iTimer->SetOperation(operation);
	iTimer->IssueRequest();
	
	CActiveScheduler::Start();

	TInt error = operation->iStatus.Int();
	iTimer->Cancel();
	CleanupStack::PopAndDestroy();  // wait
	if(error !=KErrNone)
		User::Leave(error);
	}

//
// Interpret the message
//
void CNbscTester::ReParseL()
	{
	TBuf8<1> parameter;

	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->iStatus = KRequestPending;
	wait->Start();
	
	CMsvOperation* operation = iBioClientMtm->InvokeAsyncFunctionL( KBiosMtmParse, *iSelection, parameter, wait->iStatus);

	iTimer->SetOperation(operation);
	iTimer->IssueRequest();

	CActiveScheduler::Start();
	iTimer->Cancel();
	TInt error = operation->iStatus.Int();
	
	CleanupStack::PopAndDestroy();  // wait
	if(error !=KErrNone)
		User::Leave(error);
	}

//
// Act on the changes
// 
void CNbscTester::ReProcessL()
	{
	TBuf8<1> aParameter;
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->iStatus = KRequestPending;
	wait->Start();

	CMsvOperation* operation = iBioClientMtm->InvokeAsyncFunctionL(KBiosMtmProcess, *iSelection, aParameter, wait->iStatus);
	
	iTimer->SetOperation(operation);
	iTimer->IssueRequest();
	
	CActiveScheduler::Start();

	TInt error = operation->iStatus.Int();
	iTimer->Cancel();
	
	CleanupStack::PopAndDestroy();  // wait
	if(error !=KErrNone)
		User::Leave(error);
	}

void CNbscTester::TestFindBodyL(const TDesC& aText)
	{
	iBioClientMtm->SwitchCurrentEntryL((*iSelection)[0]);
	iBioClientMtm->LoadMessageL();
	TMsvPartList retPart =  iBioClientMtm->Find(aText,KMsvMessagePartBody);
	if(retPart&KMsvMessagePartBody)
		iBioTestUtils->WriteComment(_L("Found in body text"));
	else
		iBioTestUtils->WriteComment(_L("Not found in body text"));
	
	TRAP_IGNORE(iBioClientMtm->SaveMessageL());
	TRequestStatus st;
	TMsvId s =0;
	TRAP_IGNORE(iBioClientMtm->ReplyL(s,retPart,st));
	TRAP_IGNORE(iBioClientMtm->ForwardL(s,retPart,st));
	iBioClientMtm->ValidateMessage(retPart);
	
	const TBufC<1> yy;
	TRAP_IGNORE(iBioClientMtm->AddAddresseeL(yy) );
	TRAP_IGNORE(iBioClientMtm->AddAddresseeL(yy ,yy) );
	TInt ii = 0;
	iBioClientMtm->RemoveAddressee(ii);
	TBuf8<1> yy1;
	const CMsvEntrySelection* selection1 = new(ELeave)CMsvEntrySelection();
	TRAP_IGNORE(iBioClientMtm->InvokeSyncFunctionL( ii,*selection1, yy1));
	
	TUid parserUid =TUid::Uid(10);
	iBioClientMtm->QueryCapability(parserUid,ii);
	delete selection1;
	}

void CNbscTester::TestFindSubjectL(const TDesC& aText)
	{
	iBioClientMtm->LoadMessageL();
	TMsvPartList retPart = iBioClientMtm->Find(aText,KMsvMessagePartDescription);
	if(retPart&KMsvMessagePartDescription)
		iBioTestUtils->WriteComment(_L("Found in Subject"));
	else
		iBioTestUtils->WriteComment(_L("Not found in sbject"));
	}

LOCAL_C void doMainL()
	{
	scheduler = new (ELeave) TestScheduler;
	CActiveScheduler::Install( scheduler );

	User::LeaveIfError(theFs.Connect());
	theFs.SetSessionPath(_L("C:\\"));

	test.Console()->SetPos(0, 16);

	CNbscTester* activeConsole = CNbscTester::NewLC();
	CleanupStack::Pop();
	activeConsole->StartL();        //Start the active console
	// Cleanup exit
	delete activeConsole;

	theFs.Close( );
	delete scheduler;       

	}


GLDEF_C TInt E32Main()
	{       
	__UHEAP_MARK;
	test.Start(_L("T_BIOC - Test Harness"));
	theCleanup=CTrapCleanup::New();
	TRAPD(ret,doMainL());           
	test(ret==KErrNone);
	delete theCleanup;      
	test.Console()->SetPos(0, 13);
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return(KErrNone);
	}
