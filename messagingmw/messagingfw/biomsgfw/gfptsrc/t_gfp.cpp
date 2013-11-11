// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Test harness for Generic File Parser (ie vCard, vCalendar, Op. logo, Ringing Tones)
// 
//

//Create a buffer containing an sms message with the relevant field
//then pass it by reference to the parser dll
#include <e32uid.h>
#include <e32test.h>
#include <e32cons.h>
#include <s32file.h>
#include <e32base.h>

#include "Testenv.h"

#include "REGPSDLL.H"

#include "biotestutils.h"

#define BIO_MSG_ENTRY_PARSED     1 // Set entry .iMtmData3 to 1 to indicate that the entry has store i.e parsed,externalised

#define KTestEnpPriority    (1)    //Change this....

#define KGenericParserTextFilePath  _L("c:\\test\\bio\\gfp\\")

//

//forward reference
class CExampleScheduler;
LOCAL_C void DisplayErrorReason(TInt& aReason);
 
//global declaration
LOCAL_D RTest test(_L("GFP Test Harness"));

    //
    // CExampleScheduler //
    //

class CExampleScheduler : public CActiveScheduler
    {
    public:
    void Error (TInt aError) const;
    };

void CExampleScheduler::Error(TInt anError) const
    {
    CActiveScheduler::Stop();
    test.Printf(_L("\nLeave signalled, reason=%d\n"),anError);
    }
// end CExampleScheduler::Error(TInt) const

 //global declaration
LOCAL_D CExampleScheduler *exampleScheduler;
 
    //
    // CTestGfp: INTERFACE //
    //

class CTestGfp : public CActive
    {
    public:
        ~CTestGfp();
        CTestGfp();

        static CTestGfp* NewL();
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
		TBool						iAllTestsSucessful;
    };



    //
    // CTestGfp: IMPLEMENTATION //
    //


CTestGfp* CTestGfp::NewL()
    {
    CTestGfp* self = new (ELeave) CTestGfp();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();  //self
    return self;
    }
// end CTestGfp::NewL(CMsvServerEntry*)


void CTestGfp::ConstructL()
    {
	// create test utilities object & go server side
	iBioTestUtils = CBioTestUtils::NewL(test, ETuGoClientSide);
	iBioTestUtils->GoClientSideL();

	// create some messages
	iMsvSelection = iBioTestUtils->GenerateMessagesL(KGenericParserTextFilePath);
	iMsgCount = iMsvSelection->Count();
	iAllTestsSucessful = ETrue;
    CActiveScheduler::Add(this);
    }
// end CTestGfp::ConstructL()


void CTestGfp::Start(TInt aCmd)
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
			// create the parser
			if(iParser)
				delete iParser;

			while(!finished)
				{
				__UHEAP_FAILNEXT(failCount++);
				// 
				TRAP(error, iParser = iBioTestUtils->CreateParserL((*iMsvSelection)[iMsgCtr]););
				if (error == KErrNone)
					{
					__UHEAP_RESET;
					finished = ETrue;
					__UHEAP_RESET;
					}
				// Handle error
				else
					{
					// Check if error is out of memory or a specific fax rendering error
					test(error == KErrNoMemory);
					__UHEAP_RESET;
					}
				}

			iMsgBuffer = iBioTestUtils->MessageBodyL((*iMsvSelection)[iMsgCtr]).AllocL();
			finished = EFalse;
	#ifdef _DEBUG			
			failCount = 0;
	#endif			
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
			test.Printf(_L("process...(This is not supported in Generic File Parser)\n"));
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
		default:
            break;
        }
    }
// end CTestGfp::StartL(TInt)


void CTestGfp::RunL()
    {
    TInt result=iStatus.Int();

    if (result != KErrNone && result != KErrNotSupported)
        {
		iAllTestsSucessful = EFalse;
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
			if(++iMsgCtr < iMsgCount)
				Start(EParse);
			else
				{
				test.Console()->ClearScreen();
				test.Console()->SetPos(0,0);
				DisplayErrorReason(result);
				iParser->Cancel();
				if(iAllTestsSucessful)
					iBioTestUtils->TestHarnessCompleted();
				CActiveScheduler::Stop();
				}
			break;

        default:
            break;
        }
    }
// end CTestGfp::RunL()


void CTestGfp::DoCancel()
    {
    }
// end CTestGfp::DoCancel()

CTestGfp::CTestGfp()
:CActive(KTestEnpPriority)
    {
    }
// end CTestGfp::CTestGfp()


CTestGfp::~CTestGfp()
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
// end CTestGfp::~CTestGfp()

void CTestGfp::RequestComplete(TRequestStatus& aStatus,TInt aCompletion)
	{
	TRequestStatus* statusPtr=&aStatus;
	User::RequestComplete(statusPtr,aCompletion);
	}
 
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

    CTestGfp* testParser = CTestGfp::NewL();
    CleanupStack::PushL(testParser);

    test(testParser != NULL);

    testParser->Start(CTestGfp::EParse);
    CActiveScheduler::Start();

    CleanupStack::PopAndDestroy(2); // testParser, exampleScheduler, sEntry ,theServer
    }
// end doMainL()


GLDEF_C TInt E32Main()
    {
    test.Title();
    test.Start(_L("GFP Test Harness"));
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
