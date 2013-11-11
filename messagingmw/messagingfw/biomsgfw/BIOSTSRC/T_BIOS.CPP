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

#include <msvruids.h>
#include <e32test.h>
#include <f32fsys.h>
#include <s32file.h>
#include <e32uid.h>
#include <msvreg.h>
#include <mtsr.h>

#include <msvids.h>
#include <bioscmds.h>
// BIF file database
#include <biodb.h>

#include <biouids.h>		// Defines for use in Bio messaging
#include <bitsids.h>		// Uids for use in Test Harnesses

#include <bsp.h>

#include "biotestutils.h"

#include <txtrich.h>		//	rich text stuff
#include <txtfmlyr.h>		//	para format gubbins..

// Compiler flag to switch on Heap Failure Testing
#define _HEAP_FAILURE_TEST_

RTest	test(_L("BioServer MTM Testrig"));
LOCAL_D	CTrapCleanup* trapcleanup;
LOCAL_D CActiveScheduler* activescheduler;
#ifdef _DEBUG
LOCAL_D TInt failCount1;
#endif

enum{
	ENbssReadingFiles,
	ENbssTestParseProcess,
	ENbssTestParse,
	ENbssTestProcess,
	ENbssTestCopyTo,
	ENbssTestDelete,
	ENbssTestMoveTo,
	ENbssTestCopyFrom,
	ENbssTestMoveFrom,
	ENbssTestMoveWithin,
	ENbssTestCreate,
	ENbssTestChange,
	ENbssTestParsing			// to allow heap testing to continue when
								// thread has left with KErrNoMemory
	};

// create our own active scheduler class

class CExampleScheduler : public CActiveScheduler
	{
public:
	void Error (TInt aError) const;
	};

void CExampleScheduler::Error(TInt /*anError*/) const
	{
	CActiveScheduler::Stop();
	}

class CTestNbssMtm : public CActive
	{
public:
	static CTestNbssMtm* NewLC();
	static CTestNbssMtm* NewL();
	void ConstructL();

	  // Destruction
	~CTestNbssMtm();

	  // Issue request
	void StartL();
	CBaseServerMtm*	Mtm();
	  // Cancel request.
	  // Defined as pure virtual by CActive;
	  // implementation provided by this class.
	void DoCancel();

	  // Service completed request.
	  // Defined as pure virtual by CActive;
	  // implementation provided by this class,
	void RunL();
	void DoRunL();

	//  Sets the active scheduler going
    void QueueOperationAsync(TInt aErr);
	
protected:
	// message creation code
	void InitialiseTesterL();
	void SetMessageType(const TDesC& aFileName);
	// mtm test functions
	void DoCreateParserL();
	void DoParseAgainL();
	void DoProcessAgainL();
	void DoCopyToL();
	void DoCopyFromL();
	void DoMoveToL();
	void DoMoveFromL();
	void DoMoveWithinL();
	void DoDeleteL();
	void DoCreateL();
	void DoChangeL();

	void ProcessError(TInt& aError);

protected:
	  // Data members defined by this class
	CBaseServerMtm*				iNbssServerMtm;
	CMsvEntrySelection*			iSelection;
	CMsvEntrySelection*			iMsvSelection;
	TInt						iState;

	CDir*						iDir;

	RFs							iFs;

	CBioTestUtils*				iBioTestUtils;

	TInt						iNumFiles;
	TInt						iFilesProcessed;
	TInt						iMessagesCreated;
	TInt						iTestCtr;
	CArrayPtrSeg<CParsedField>* iTestParsedFieldArray;
	TBool						iTestsSuccessful;

private:
	CTestNbssMtm();
	};

//
// CTestRefresh CActive object
//
CTestNbssMtm::CTestNbssMtm() // construct low-priority active object
	: CActive(-5)
	{
	}

CTestNbssMtm  *CTestNbssMtm::NewLC()
	{
	CTestNbssMtm* self=new (ELeave) CTestNbssMtm();
	
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CTestNbssMtm  *CTestNbssMtm::NewL()
	{
	CTestNbssMtm* self=NewLC();
	CleanupStack::Pop();
	return self;
	}

void CTestNbssMtm::ConstructL()
	{
	// connect to file system & create a BIO database
	User::LeaveIfError(iFs.Connect());

	iBioTestUtils = CBioTestUtils::NewL(test,ETuGoServerSide);

	iBioTestUtils->CreateServicesL();

	iBioTestUtils->InstantiateServerMtmsL();

	iNbssServerMtm = iBioTestUtils->iBioServerMtm;

	CActiveScheduler::Add(this); // add to active scheduler

	// create an entry selection
	iSelection= new (ELeave) CMsvEntrySelection;

	iNumFiles=0;
	iFilesProcessed=0;
	iMessagesCreated=0;
	iTestsSuccessful = ETrue;

	test.Console()->ClearScreen(); // get rid of menu
	}
 
CTestNbssMtm::~CTestNbssMtm()
	{ 
	Cancel(); // make sure we're cancelled
	
	delete iSelection;
	delete iMsvSelection;
	delete iBioTestUtils;
	delete iDir;
	
	iFs.Close();

	if(iTestParsedFieldArray)
		{
		iTestParsedFieldArray->ResetAndDestroy();
		delete iTestParsedFieldArray;
		}
	}



void  CTestNbssMtm::DoCancel()
	{
	iNbssServerMtm->Cancel();
	CActiveScheduler::Stop();
	}


CBaseServerMtm*	CTestNbssMtm::Mtm()
	{
	return iNbssServerMtm;
	}


void CTestNbssMtm::StartL()
	{
	test.Next(_L("CTestNbssMtm::Start()\n"));
	Cancel();
	iState=ENbssReadingFiles;
#if defined _HEAP_FAILURE_TEST_
	TBool finished=EFalse;
#ifdef _DEBUG
	TInt failCount=0;
	failCount1=0;
#endif
	while (!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAPD(error,InitialiseTesterL());
		if (error==KErrNone)
			{
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(error==KErrNoMemory);
			__UHEAP_RESET;
			}
		}
#else
	InitialiseTesterL();
#endif
	QueueOperationAsync(KErrNone);
	}



void CTestNbssMtm::QueueOperationAsync(TInt aErr)
    {
    TRequestStatus*  pS = &iStatus;
	iStatus = KRequestPending;
    User::RequestComplete(pS, aErr);
	SetActive();
    }

void CTestNbssMtm::InitialiseTesterL()
	{
	// Get the NB if we don't need this two step process, use SetSmsServiceL().
	iBioTestUtils->TestStart(iTestCtr);
	iBioTestUtils->SetBIOServiceIdL();
	// Empty the Inbox
	iBioTestUtils->EmptyInboxMessagesL();
	iMsvSelection = iBioTestUtils->GenerateMessagesL();
	}

void CTestNbssMtm::DoCreateParserL()
	{
	TBuf8<1>dummy;
	// add service entry and msg entry to selection passed to MTM
	iSelection->Reset();
	iSelection->AppendL((*iMsvSelection)[iFilesProcessed]);

#if defined _HEAP_FAILURE_TEST_
	TBool finished=EFalse;
	while (!finished)
		{
		__UHEAP_FAILNEXT(failCount1++);
		TRAPD(error,iNbssServerMtm->StartCommandL(*iSelection, KBiosMtmParseThenProcess, dummy, iStatus));
		if (error==KErrNone)
			{
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(error==KErrNoMemory);
			__UHEAP_RESET;
			}
		}
#else
	iNbssServerMtm->StartCommandL(*iSelection, KBiosMtmParseThenProcess, dummy, iStatus);
#endif
	}

void CTestNbssMtm::DoParseAgainL()
	{
	TBuf8<1>dummy;
	iState = ENbssTestParse;
	// add service entry and msg entry to selection passed to MTM
	iSelection->Reset();

	iSelection->AppendL((*iMsvSelection)[iFilesProcessed]);

#if defined _HEAP_FAILURE_TEST_
	TBool finished=EFalse;

#ifdef _DEBUG
	TInt failCount=0;
#endif

	while (!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAPD(error,iNbssServerMtm->StartCommandL(*iSelection, KBiosMtmParse, dummy, iStatus));
		if (error==KErrNone)
			{
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(error==KErrNoMemory);
			__UHEAP_RESET;
			}
		}
#else
	iNbssServerMtm->StartCommandL(*iSelection, KBiosMtmParse, dummy, iStatus);
#endif
	iState = ENbssTestParse;
	}

void CTestNbssMtm::DoProcessAgainL()
	{
	TBuf8<1>dummy;
	iState = ENbssTestProcess;
	// add service entry and msg entry to selection passed to MTM
	iSelection->Reset();
	iSelection->AppendL((*iMsvSelection)[iFilesProcessed]);

#if defined _HEAP_FAILURE_TEST_
	TBool finished=EFalse;

#ifdef _DEBUG
	TInt failCount=0;
#endif

	while (!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAPD(error,iNbssServerMtm->StartCommandL(*iSelection, KBiosMtmProcess, dummy, iStatus));
		if (error==KErrNone)
			{
			finished=ETrue;
			__UHEAP_RESET;
			}
		else 
			{
			test(error==KErrNoMemory);
			__UHEAP_RESET;
			}
		}
#else
	iNbssServerMtm->StartCommandL(*iSelection, KBiosMtmProcess, dummy, iStatus);
#endif
	iState = ENbssTestProcess;
	}

void CTestNbssMtm::DoCopyToL()
	{
	iNbssServerMtm->CopyToLocalL(*iSelection, KMsvGlobalInBoxIndexEntryId, iStatus);
	}

void  CTestNbssMtm::DoCopyFromL()
	{
	iNbssServerMtm->CopyFromLocalL(*iSelection,KMsvGlobalInBoxIndexEntryId, iStatus);
	}

void CTestNbssMtm::DoDeleteL()
	{
	iNbssServerMtm->MoveFromLocalL(*iSelection,KMsvGlobalInBoxIndexEntryId, iStatus);
	}

void CTestNbssMtm::DoMoveToL()
	{
	iNbssServerMtm->MoveFromLocalL(*iSelection,KMsvGlobalInBoxIndexEntryId, iStatus);
	}

void CTestNbssMtm::DoMoveFromL()
	{
	iNbssServerMtm->MoveFromLocalL(*iSelection,KMsvGlobalInBoxIndexEntryId, iStatus);
	}

void CTestNbssMtm::DoMoveWithinL()
	{
	iNbssServerMtm->MoveWithinServiceL(*iSelection,KMsvGlobalInBoxIndexEntryId, iStatus);
	}

void CTestNbssMtm::DoCreateL()
	{
	TMsvEntry dudEntry;
	iNbssServerMtm->CreateL(dudEntry, iStatus);
	}

// test create
void CTestNbssMtm::DoChangeL()
	{
	TMsvEntry dudEntry;
	iNbssServerMtm->ChangeL(dudEntry, iStatus);
	}

void CTestNbssMtm::RunL()
	{
	TRAPD(error,DoRunL());
	if(error!=KErrNone)
		{
		iBioTestUtils->TestFinish(iTestCtr,error);
		CActiveScheduler::Stop();
		}
	}

void CTestNbssMtm::DoRunL()
	{
	iTestCtr++;
	TInt error=iStatus.Int();

	if (iState == ENbssTestParsing)
		{
		// heap testing sets the active state to KErrNoMemory. In such a case
		// resume testing from the current count of failCount1
		if (error == KErrNone || error == KErrNotSupported)
			{
			// test has complete ok - move to next state
			iBioTestUtils->LogCurrentMessageTypeL((*iMsvSelection)[iFilesProcessed]);
			iState = ENbssTestParseProcess;
			}
		else
			test (error==KErrNoMemory);
		}
	else
		{
		ProcessError(error);
		iBioTestUtils->TestFinish(iTestCtr,error);
		}
	TBool quit=EFalse;

	while (!quit)
		{
		switch(iState)
			{
		case ENbssReadingFiles:
			// for messaging api v2 the bio server mtm is null. The following code allows
			// the test harness to run and pass without breaking the nightly builds, but
			// since this test harness is redundant, it should be removed.
			iBioTestUtils->TestStart(iTestCtr,_L("Dummy test for null bio server MTM"));
			iBioTestUtils->TestHarnessCompleted();
			CActiveScheduler::Stop();
			quit=ETrue;
			break;
		case ENbssTestParseProcess:
			iBioTestUtils->TestStart(iTestCtr,_L("re-parse"));
			DoParseAgainL();
			SetActive();
			quit=ETrue;
			break;
		case ENbssTestParse:
			iBioTestUtils->TestStart(iTestCtr,_L("re-process"));
			DoProcessAgainL();
			SetActive();
			quit=ETrue;
			break;
		case ENbssTestParsing:
			DoCreateParserL();
			SetActive();
			quit=ETrue;
			break;

		case ENbssTestProcess:
			iFilesProcessed++;
			if(iFilesProcessed == iMessagesCreated)
				{
				if(iTestsSuccessful)
					iBioTestUtils->TestHarnessCompleted();
				CActiveScheduler::Stop();
				quit=ETrue;
				}
			else
				{
				iBioTestUtils->TestStart(iTestCtr,_L("Call parse and process"));
			#ifdef _DEBUG				
				failCount1=0;
			#endif				
				iState=ENbssTestParsing;
				}
			break;
			}
		}
	}

void CTestNbssMtm::ProcessError(TInt& aError)
	{
	if(aError==KErrNotSupported)
		{
		aError = KErrNone;
		iBioTestUtils->WriteComment(_L("Unsupported operation"));
		}
	else if (aError == -521)
		{
		aError = KErrNone;
		iBioTestUtils->WriteComment(_L("Re-processing not performed. Script can only be added to once."));
		}
	else if((aError <= -500 && aError >= -521) || (aError <= -600 && aError >= -614))
		{
		iBioTestUtils->WriteComment(_L("Corrupt or missing Bio-data"));
		}
	else if(aError != KErrNone)
		iTestsSuccessful = EFalse;
	}

LOCAL_C void testSetup()
//
//	Start the various bits and pieces of the comms server
//
	{
	trapcleanup=CTrapCleanup::New();
	test(trapcleanup!=NULL);
	activescheduler = new CActiveScheduler;
	test(activescheduler!=NULL);
	CActiveScheduler::Install(activescheduler);
	}

LOCAL_C void testUnsetup()
//
//	Start the various bits and pieces of the comms server
//
	{
	delete activescheduler;
	delete trapcleanup;
	}


LOCAL_C void doMainL()
//
// Test SMMS.
//
    {
	CTestNbssMtm* nbssTester = CTestNbssMtm::NewL();
	nbssTester->StartL();

	CActiveScheduler::Start();
	
	delete nbssTester;
    }

GLDEF_C TInt E32Main()
	{
 	test.Title();
	test.Start(_L("Testing..."));
	__UHEAP_MARK;

	testSetup();

	TRAPD(error,doMainL());

	testUnsetup();

	test (error==KErrNone);

	test.Console()->Printf(_L("Done, press any key\n"));

	__UHEAP_MARKEND;
	test.End();
	return KErrNone;
	}

