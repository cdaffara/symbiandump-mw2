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
// Test harness for Internet Access Configuration Parser
// 
//

#include "biotestutils.h"
//
#include "IACPERR.H"
#include "IACPDEF.H"
#include "tmsvbioinfo.h"

//
//SMS configuration files path
_LIT(IACP_TEST_MSG_PATH,	"c:\\test\\bio\\iacp\\");

//forward reference
class CExampleScheduler;

//global declarations
LOCAL_D CExampleScheduler *exampleScheduler;
LOCAL_D RTest test(_L("IACP Test Harness"));


//
//-- CExampleScheduler --
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

LOCAL_C void DisplayErrorReason(TInt& aReason,CBioTestUtils* aBioTestUtils)
	{
	if(aReason==KErrNone)
		{
		aBioTestUtils->TestHarnessCompleted();
		return;
		}

	TBuf<256> tempBuf;
	switch (aReason)
		{
		case KErrCancel:
			tempBuf= _L("\r\n-->Session cancelled");
			break;
		case KErrEof:
			tempBuf= _L("\r\n-->KErrEof");
			break;
		case KErrNoMemory:
			tempBuf= _L("\r\n-->KErrNoMemory");
			break;
		case KErrDisconnected:
			tempBuf= _L("\r\n-->KErrDisconnected");
			break;
		case KErrAccessDenied:
			tempBuf= _L("\r\n-->KErrAccessDenied");
			break;
		case KBspInvalidMessage:
			tempBuf= _L("\r\n-->Error: Invalid SMS Message. For Internet Access Configuration,\r\nSMS Header Field should be //SIAP11 ");
			break;
		case KIacpUnknownSmsType:
			tempBuf=_L("\r\n-->Error:SMS 1st Left Token does not start with niether M  nor I ");
			break;
		case KIacpBIOMsgTypeNotSupported:
			tempBuf= _L("\r\n-->this Bio msg is not suppported");
			break;
		case KIacpMandatoryDataNotSet:
			tempBuf= _L("\r\n-->Error: Mandatory data missing in SMS message.");
			break;
		case KIacpUnknownMailProtocol:
			tempBuf= _L("\r\n-->Error: Unknown Mail Protocol Not a Pop3/Imap4");
			break;
		case KIacpErrRightToken:
			tempBuf= _L("\r\n-->Error: improper right token  i.e not equal t/f (True/False)");
			break;
		case KIacpErrLeftToken:
			tempBuf=_L("\r\n-->Error:SMS Left Tokens should start with 'M' for Mailbox account configuration\r\n and with 'I' for Internet service configuration");
			break;
		case KIacpErrSmsDataNotParsed:
			tempBuf= _L("\r\n-->sms data should be parsed before processing");
			break;
		case KIacpErrSmsDataNotRestored:
			tempBuf= _L("\r\n-->Error: sms data should be parsed before commiting (KIacpErrSmsDataNotRestored)");
			break;
		case KIacpScriptErrISPNotFound:
			tempBuf= _L("\r\n-->Internet Service not found in Dial Out DB");
			break;
		case KIacpErrScriptNotDefined: 
			tempBuf= _L("\r\n-->Sript not included in sms");
			break;
		case KIacpErrScriptNotFoundInDB:
			tempBuf= _L("\r\n-->There is no script in DB to append to.");
			break;
		case KIacpErrScriptAlreadyAdd:
			tempBuf= _L("\r\n-->Script cannot be add more than ounce for each Bio Msg");
			break;
		default:
			tempBuf.Format(_L("\r\n-->Error !!!= %dview"), aReason );
			break;
		}

	aBioTestUtils->Printf(tempBuf);			  
	aBioTestUtils->TestHarnessFailed(aReason);
	}

//--------------------------------------
//
//-- CTestIacp --
//

enum TSessionState
	{
	EParse,  // parsing is done at ParseL() stage. and data are externalised.
	EProcess, //parsed data are internalised, then commited
	EReparse,
	EReProcess,
	EDisplay,  //Display entry data
	};

class CTestIacp : public CActive 
	{
	public:
		~CTestIacp();
		static  CTestIacp * NewL(CBioTestUtils* aBioTestUtils);
		void StartL(TInt aCmd);

	public:
		CTestIacp(CBioTestUtils* aBioTestUtils);
		void ConstructL();

		void CreateParserL(); //sets 'iParser' to point to the created iacp parser
		void GetMessageBodyL();
		
		void ParseMsgWithHeapTest();
		void ProcessMsgWithHeapTest();

		void DisplayEntryDataL();
		void DisplayFinalSummary();

		void RequestComplete(TRequestStatus& aStatus,TInt aCompletion);
		void DoCancel();
		void RunL();

	private:
		TInt iState;
		CBioTestUtils*		iBioTestUtils;
		CBaseScriptParser2*	iParser;

		HBufC* iSmsMsg;			//sms body build dependent		
		TMsvId						iEntryId;       //ID of sms entry
		CMsvEntrySelection*			iIacpSelection;
		TInt						iIacpMsgCtr;
		TInt						iFailedToProcess;

		TInt			iParseFailCount;//heap testing (Parsing state)
		TInt			iProcessFailCount;//heap testing (processing state)
	};


//
CTestIacp::CTestIacp(CBioTestUtils* aBioTestUtils)
:CActive(EPriorityStandard),iBioTestUtils(aBioTestUtils)
	{
	}

CTestIacp* CTestIacp::NewL(CBioTestUtils* aBioTestUtils)
	{
	CTestIacp* self = new(ELeave) CTestIacp(aBioTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();  //self 
	return self;
	}

void CTestIacp::ConstructL()
	{
	iBioTestUtils->WriteComment(_L("CTestIacp::ConstructL()."));
	CreateParserL();

	iIacpSelection=	iBioTestUtils->GenerateMessagesL(IACP_TEST_MSG_PATH);
	CActiveScheduler::Add(this);
	}

CTestIacp::~CTestIacp()
	{
	Cancel();

	delete iIacpSelection;
	delete iSmsMsg;
	delete iParser;
	}

void CTestIacp::CreateParserL()
	{
	TInt error = KErrNone;
	TBool finished = EFalse;

#ifdef _DEBUG
	TInt failCount = 0;
#endif

	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error,(iParser = iBioTestUtils->CreateParserTypeL(KUidBIOInternetAccessPointMsg)));
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			iBioTestUtils->WriteComment(_L("Created IACP Parser object"));
			finished = ETrue;
			__UHEAP_RESET;
			}
		// Handle error
		else
			{
			test(error == KErrNoMemory);
			__UHEAP_RESET;
			}
		}

	}

void CTestIacp::GetMessageBodyL()
	{
	delete iSmsMsg;
	iSmsMsg = NULL;
	iSmsMsg= iBioTestUtils->MessageBodyL(iEntryId).AllocL();
	}

void CTestIacp::StartL(TInt aCmd)
	{
	iState=aCmd;
	iEntryId = (*iIacpSelection)[iIacpMsgCtr];

	switch (iState)
		{
		case EParse:
			iBioTestUtils->Printf(_L("Parsing...\n"));
			iBioTestUtils->SetEntryL(iEntryId); //clients have to set the right context before calling ParseL() 
			GetMessageBodyL();
			ParseMsgWithHeapTest();
			break;

		case EProcess:
			iBioTestUtils->Printf(_L("Processing...\n"));
			iBioTestUtils->SetEntryL(iEntryId); //clients have to set the right context before calling CommitL() 
			ProcessMsgWithHeapTest();
			break;

		case EReparse:
			{
			delete iParser;
			CreateParserL();
			iBioTestUtils->Printf(_L("Re-parsing...\n"));
			iBioTestUtils->SetEntryL(iEntryId); //clients have to set the right context before calling CommitL() 
			iParser->ParseL(iStatus,iSmsMsg->Des());
			SetActive();
			//ParseMsgWithHeapTest();
			}
			break;

		case EReProcess:
			{
			delete iParser;
			CreateParserL();
			iBioTestUtils->Printf(_L("Re-Processing...\n"));
			iBioTestUtils->SetEntryL(iEntryId); //clients have to set the right context before calling CommitL() 
         	iParser->ProcessL(iStatus); //internalise and commit
 			SetActive();
			//ProcessMsgWithHeapTest();
			}
			break;

		case EDisplay:
            DisplayEntryDataL();
			RequestComplete(iStatus,KErrNone);
            SetActive();
			break;

		default:
			break;
		}
	}

void CTestIacp::ParseMsgWithHeapTest()
	{
	TInt error = KErrNone;
	TBool finished = EFalse;
	
	while(!finished)
		{
	//	__UHEAP_FAILNEXT(iParseFailCount++);
		// 
		TRAP(error, iParser->ParseL(iStatus,iSmsMsg->Des())); 
		if (error == KErrNone)
			{
	//		__UHEAP_RESET;
			finished = ETrue;
			SetActive();
			}
		else
			{
			test(error == KErrNoMemory);
	//		__UHEAP_RESET;
			}		
		}
			
	}

void CTestIacp::ProcessMsgWithHeapTest()
	{
	TInt error = KErrNone;
	TBool finished = EFalse;
	while(!finished)
		{
	//	__UHEAP_FAILNEXT(iProcessFailCount++);
		// 
		TRAP(error, iParser->ProcessL(iStatus)); //internalise and process
		if (error == KErrNone)
			{
	//		__UHEAP_RESET;
			finished = ETrue;
			SetActive();
			}
		else
			{
			test(error == KErrNoMemory);
	//		__UHEAP_RESET;
			}		
		}
	}

void CTestIacp::DoCancel()
	{
	iBioTestUtils->WriteComment(_L("DoCancel()"));
	}

void CTestIacp::RequestComplete(TRequestStatus& aStatus,TInt aCompletion)
	{
	TRequestStatus* statusPtr=&aStatus;
	User::RequestComplete(statusPtr,aCompletion);
	}

void CTestIacp::RunL()
	{
	TInt result=iStatus.Int();

	//heap test faillure
	if( result == KErrNoMemory)
		{
		if(iState==EParse)
			{
			iParseFailCount++;
			StartL(EParse);
			return;
			}
		else if(iState==EProcess)
			{
			iProcessFailCount++;
			StartL(EProcess);
			return;
			}
		else if(iState==EReparse)
			{
			iParseFailCount++;
			StartL(EReparse);
			}
		else if(iState==EReProcess)
			{
			iProcessFailCount++;
			StartL(EReProcess);
			}
		}

	if ( result== KIacpErrSmsDataNotParsed)
		{
		iBioTestUtils->ClearScreen();
		iBioTestUtils->Printf(_L("Error: Processing before parsing SMS.\n"));
		iBioTestUtils->Printf(_L("Press any key to continue parsing\n\n"));
		}

	switch (iState)
		{
		case EParse:
			iParseFailCount=0;
			iProcessFailCount=0;	
			iBioTestUtils->WriteComment(_L("Parsed Ok."));
			if(result==KErrNone)
				StartL(EProcess);
			else
				{
				iFailedToProcess++;
				iIacpMsgCtr++;
				if(iIacpMsgCtr < iIacpSelection->Count())
					{
					StartL(EParse);
					}
				else
					{
					DisplayFinalSummary();
					DisplayErrorReason(result,iBioTestUtils);
					iParser->Cancel();
					CActiveScheduler::Stop();
					}
				}
			break;

		case EProcess:
			iParseFailCount=0;
			iBioTestUtils->WriteComment(_L("Processed Ok."));
			StartL(EReparse);
			break;

		case EReparse:
			iProcessFailCount=0;		
			iBioTestUtils->WriteComment(_L("Re-Parsed Ok."));
			StartL(EReProcess);
			break;

		case EReProcess:
			iBioTestUtils->WriteComment(_L("Re-Processed Ok."));
			StartL(EDisplay);
			break;

		case EDisplay:
			iParseFailCount=0;
			iProcessFailCount=0;
			iBioTestUtils->TestFinish(iIacpMsgCtr);
			iIacpMsgCtr++;
			if(iIacpMsgCtr < iIacpSelection->Count())
				{
				iBioTestUtils->TestStart(iIacpMsgCtr);
				StartL(EParse);
				}
			else
				{
				DisplayFinalSummary();
				DisplayErrorReason(result,iBioTestUtils);
				iParser->Cancel();
				CActiveScheduler::Stop();
				}
			break;

		default:
			break;
		}
	}


void CTestIacp::DisplayEntryDataL()
    {
    iBioTestUtils->SetEntryL(iEntryId);

    iBioTestUtils->ClearScreen();
    //test.Console()->SetPos(0,0);

    TMsvEntry entry = iBioTestUtils->Entry();
    iBioTestUtils->Printf(_L("===== MESSAGE DETAILS =====\n"));
	iBioTestUtils->Printf(_L("Id:			  0x%x\n"), iEntryId);
    iBioTestUtils->Printf(_L("iServiceId:     0x%x\n"), entry.iServiceId);
    iBioTestUtils->Printf(_L("iRelatedId:     0x%x\n"), entry.iRelatedId);
    iBioTestUtils->Printf(_L("iType:          0x%x\n"), entry.iType);
    iBioTestUtils->Printf(_L("iMtm:           0x%x\n"), entry.iMtm);
    TBuf<128> dateStr;
    entry.iDate.FormatL(dateStr, (_L("%D%M%Y%/0%1%/1%2%/2%3%/3")));
    iBioTestUtils->Printf(_L("iDate:          %S\n"), &dateStr);

    iBioTestUtils->Printf(_L("iSize:          %d\n"), entry.iSize);
    iBioTestUtils->Printf(_L("iError:         %d\n"), entry.iError);
    iBioTestUtils->Printf(_L("iBioType:		0x%x\n"), entry.iBioType);
    iBioTestUtils->Printf(_L("iMtmData1:      %d\n"), entry.MtmData1());
    iBioTestUtils->Printf(_L("iMtmData2:      %d\n"), entry.MtmData2());
    iBioTestUtils->Printf(_L("iMtmData3:      %d\n"), entry.MtmData3());
    iBioTestUtils->Printf(_L("iDescription:  %S\n"), &entry.iDescription);
    iBioTestUtils->Printf(_L("iDetails:      %S\n "), &entry.iDetails);

    iBioTestUtils->ClearScreen();
    //test.Console()->SetPos(0,0);


	if(iBioTestUtils->Entry().MtmData3() ==0)
		{
		iBioTestUtils->WriteComment(_L("BioMg has not been parsed or processed"));
		User::Leave(KErrGeneral);
		}

	iBioTestUtils->Printf(_L("===== Extracted Fields =====\n"));
	iBioTestUtils->LogExtractedFieldsL(iEntryId);

    }

void CTestIacp::DisplayFinalSummary()
	{
	iBioTestUtils->Printf(_L("\nFinal summary\n"));
	iBioTestUtils->Printf(_L("%d messages processed\n"), iIacpMsgCtr);
	iBioTestUtils->Printf(_L("%d failed\n"), iFailedToProcess);
	}

LOCAL_C void doMainL()
	{
	//create a scheduler
	exampleScheduler = new (ELeave) CExampleScheduler;
	CleanupStack::PushL( exampleScheduler );
	CActiveScheduler::Install( exampleScheduler );

	CBioTestUtils* BioTestUtils = CBioTestUtils::NewL(test,ETuGoClientSide);
	CleanupStack::PushL(BioTestUtils);

	BioTestUtils->WriteComment(_L("Msv Server Side Created"));

	CTestIacp* testParser = CTestIacp::NewL(BioTestUtils);
	CleanupStack::PushL( testParser );
	test(testParser != NULL);
	BioTestUtils->WriteComment(_L("CTestIacp Object Created."));
	
	BioTestUtils->TestStart(0);
	testParser->StartL(EParse);
	
	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(3);    // testParser, BioTestUtils, exampleScheduler

	testParser=NULL;
	BioTestUtils=NULL;
	exampleScheduler = NULL;
	}

GLDEF_C TInt E32Main()
	{
	test.Title();
	test.Start(_L("IACP Test Harness"));
	__UHEAP_MARK;
	CTrapCleanup* cleanup=CTrapCleanup::New();
	test(cleanup!=NULL);
	TRAPD(error,doMainL());
	if (error) 
		test.Printf(_L("Completed with return code %d"),error);
	delete cleanup;

	__UHEAP_MARKEND;
	test.Close();
	test.End();
	return KErrNone;
	}
