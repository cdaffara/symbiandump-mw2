// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Test harness for Compact Business Card Parser
// 
//

//Create a buffer containing an sms message with the relevant field
//then pass it by reference to the parser dll
#include <e32uid.h>
#include <e32test.h>
#include <e32cons.h>
#include <s32file.h>
#include <e32base.h>

#include <biouids.h>

#include "biotestutils.h"

#define KTestCbcpPriority    (1)    //Change this....

#define KCompactBusinessCardTextFilePath  _L("c:\\test\\bio\\cbcp\\")

//

//forward reference
class CExampleScheduler;
LOCAL_C void DisplayErrorReason(TInt& aReason);

LOCAL_D RTest test(_L("CBCP Test Harness"));

    //
    // CExampleScheduler //
    //

class CExampleScheduler : public CActiveScheduler
    {
    public:
    void Error(TInt aError) const;
    };

// CExampleScheduler error reporting function

void CExampleScheduler::Error(TInt anError) const
    {
    CActiveScheduler::Stop();
    test.Printf(_L("\n Error reported, reason=%d\n"),anError);
    }
// end CExampleScheduler::Error(TInt) const

//global declarations
LOCAL_D CExampleScheduler *exampleScheduler;

 
    //
    // CTestCbcp: INTERFACE //
    //

class CTestCbcp : public CActive
    {
    public:
        ~CTestCbcp();
        CTestCbcp();

        static CTestCbcp* NewL();
        void ConstructL();
        void Start(TInt aCmd);
        void RunL();
        void DoCancel();
		void RequestComplete(TRequestStatus& aStatus,TInt aCompletion);

        enum TSessionState
            {
            EParse,
            EProcess,
			EReparse,
			EDisplay,  //Display entry data
            };

    private:
        TInt						iState;
		TInt						iMsgCtr;
		TInt						iMsgCount;

		TInt						iTestCtr;

        CBaseScriptParser2*			iParser;
		CMsvEntrySelection*			iMsvSelection;

        CArrayPtrSeg<CParsedField>* iTestParsedFieldArray;
		CBioTestUtils*				iBioTestUtils;
		HBufC*						iMsgBuffer;
    };



    //
    // CTestCbcp: IMPLEMENTATION //
    //


CTestCbcp* CTestCbcp::NewL()
    {
    CTestCbcp* self = new (ELeave) CTestCbcp();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();  //self
    return self;
    }
// end CTestCbcp::NewL(CMsvServerEntry*)


void CTestCbcp::ConstructL()
    {
	// create test utilities object & go server side
	iBioTestUtils = CBioTestUtils::NewL(test,ETuGoClientSide);

	// create some messages
	iMsvSelection = iBioTestUtils->GenerateMessagesL(KCompactBusinessCardTextFilePath);
	iMsgCount = iMsvSelection->Count();

	// create the parser
	TBool finished = EFalse;

#ifdef _DEBUG
	TInt failCount = 0;
#endif

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		// 
	
		TRAPD(error, iParser = iBioTestUtils->CreateParserTypeL(KUidBIOBusinessCardMsg););
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			finished = ETrue;
			__UHEAP_RESET;
			}
		// Handle error
		else
			{
			// Check if error is out of memory
			test(error == KErrNoMemory);
			__UHEAP_RESET;
			}
		}

    CActiveScheduler::Add(this);
    }
// end CTestCbcp::ConstructL()


void CTestCbcp::Start(TInt aCmd)
    {
	TInt error = KErrNone;
	TBool finished = EFalse;

#ifdef _DEBUG
	TInt failCount = 0;
#endif

    iState=aCmd;

    switch (iState)
        {
        case EParse:
			test.Printf(_L("Parsing...\n"));
			iBioTestUtils->TestStart(iTestCtr,_L("Parsing"));
			delete iMsgBuffer;
			iMsgBuffer = iBioTestUtils->MessageBodyL((*iMsvSelection)[iMsgCtr]).AllocL();

			while(!finished)
				{
				__UHEAP_FAILNEXT(failCount++);
				// 
				TRAP(error, iParser->ParseL(iStatus,*iMsgBuffer));
				if (error == KErrNone)
					{
					__UHEAP_RESET;
					finished = ETrue;
					SetActive();
					}
				// Handle error
				else
					{
					// Check if error is out of memory or a specific fax rendering error
					test(error == KErrNoMemory);
					__UHEAP_RESET;
					}
				}
            
			break;

        case EProcess:
			test.Printf(_L("process...(This is not supported in Business Card Parser)\n"));
			iBioTestUtils->TestStart(iTestCtr,_L("Calling process"));

			while(!finished)
				{
				__UHEAP_FAILNEXT(failCount++);
				// 
				TRAP(error, iParser->ProcessL(iStatus));
				if (error == KErrNone)
					{
					__UHEAP_RESET;
					finished = ETrue;
					SetActive();
					}
				// Handle error
				else
					{
					// Check if error is out of memory or a specific fax rendering error
					test(error == KErrNoMemory);
					__UHEAP_RESET;
					}
				}
            break;

		case EReparse:
			test.Printf(_L("Reparsing...\n"));
			iBioTestUtils->TestStart(iTestCtr,_L("Re-Parsing"));
			while(!finished)
				{
				__UHEAP_FAILNEXT(failCount++);
				// 
				TRAP(error, iParser->ParseL(iStatus,*iMsgBuffer));
				if (error == KErrNone)
					{
					__UHEAP_RESET;
					finished = ETrue;
					SetActive();
					}
				// Handle error
				else
					{
					// Check if error is out of memory or a specific fax rendering error
					test(error == KErrNoMemory);
					__UHEAP_RESET;
					}
				}
            break;

		case EDisplay:
			iBioTestUtils->TestStart(iTestCtr,_L("Printing parsed fields"));
			TRAP(error,iBioTestUtils->LogExtractedFieldsL((*iMsvSelection)[iMsgCtr]));
			RequestComplete(iStatus,error);
            SetActive();
			break;
  
		default:
            break;
        }
    }
// end CTestCbcp::StartL(TInt)


void CTestCbcp::RunL()
    {
    TInt result=iStatus.Int();

    if (result != KErrNone && result != KErrNotSupported)
        {
        test.Console()->ClearScreen();
        test.Console()->SetPos(0,0);
        DisplayErrorReason(result);
		iBioTestUtils->TestHarnessFailed(result);
        CActiveScheduler::Stop();
        return;
        }
	
	if(iState==EProcess)
		iBioTestUtils->WriteComment(_L("ProcessL not supported"));
	iBioTestUtils->TestFinish(iTestCtr);
	iTestCtr++;
    switch (iState)
        {
        case EParse:
            Start(EProcess);
            break;
 
		case EProcess:
			Start(EReparse);
			break;

		case EReparse:
			Start(EDisplay);
			break;

		case EDisplay:
			{
			if(++iMsgCtr < iMsgCount)
				Start(EParse);
			else
				{
				test.Console()->ClearScreen();
				test.Console()->SetPos(0,0);
				DisplayErrorReason(result);
				iParser->Cancel();
				iBioTestUtils->TestHarnessCompleted();
				CActiveScheduler::Stop();
				}
			}
			break;

        default:
            break;
        }
    }
// end CTestCbcp::RunL()


void CTestCbcp::DoCancel()
    {
    }
// end CTestCbcp::DoCancel()

void CTestCbcp::RequestComplete(TRequestStatus& aStatus,TInt aCompletion)
	{
	TRequestStatus* statusPtr=&aStatus;
	User::RequestComplete(statusPtr,aCompletion);
	}
 
CTestCbcp::CTestCbcp()
:CActive(KTestCbcpPriority)
    {
    }
// end CTestCbcp::CTestCbcp()

CTestCbcp::~CTestCbcp()
    {
    Cancel();

	delete iParser;
	delete iMsvSelection;
	delete iBioTestUtils;
	delete iMsgBuffer;

	if(iTestParsedFieldArray)
		{
		iTestParsedFieldArray->ResetAndDestroy();
		delete iTestParsedFieldArray;
		iTestParsedFieldArray=NULL;
		}
    }
// end CTestCbcp::~CTestCbcp()

LOCAL_C void DisplayErrorReason(TInt& aReason)
    {
    switch (aReason)
        {
       case KErrNone:
            test.Printf(_L("Session completed successfully"));
            break;
        case KErrCancel:
            test.Printf(_L("Session cancelled"));
            break;
        case KErrEof:
            test.Printf(_L("KErrEof"));
            break;
        case KErrNoMemory:
            test.Printf(_L("No memory"));
            break;
        case KErrDisconnected:
            test.Printf(_L(" KErrDisconnected"));
            break;
        case KErrAccessDenied:
            test.Printf(_L("KErrAccessDenied"));
            break;
        case KErrNotSupported:
            test.Printf(_L("Function Not Supported, Error = %d\n"), aReason);
            break;
        default:
            test.Printf(_L("Error !!!= %d\n"), aReason );
            break;
        }
    }

LOCAL_C void doMainL()
    {

    //create a scheduler
    exampleScheduler = new (ELeave) CExampleScheduler;
    CleanupStack::PushL( exampleScheduler );
    CActiveScheduler::Install( exampleScheduler );

    CTestCbcp* testParser = CTestCbcp::NewL();
    CleanupStack::PushL(testParser);

    test(testParser != NULL);

    testParser->Start(CTestCbcp::EParse);
    CActiveScheduler::Start();

    CleanupStack::PopAndDestroy(2); // testParser, exampleScheduler
    }
// end doMainL()


GLDEF_C TInt E32Main()
    {
    test.Title();
    test.Start(_L("ENP Test Harness"));
    __UHEAP_MARK;
    CTrapCleanup* cleanup=CTrapCleanup::New();
    test(cleanup!=NULL);
    TRAPD(error,doMainL());
    if (error) test.Printf(_L("Completed with return code %d"),error);
    delete cleanup;
    __UHEAP_MARKEND;
    test.Close();
    test.End();
    return KErrNone;
    }
// end E32Main()
