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
// Test harness for Internet Access Configuration Parser (Internet Database settings)
// 
//

#include <commdb.h>
#include "BioTestUtils.h"
//
#include "iacperr.h"
#include "iacpdef.h"

#define BIO_MSG_ENTRY_PARSED	 1 // Set entry .iMtmData3 to 1 to indicate that the entry has store i.e parsed,externalised
#define BIO_MSG_ENTRY_PROCESSED	 2 // Set TMsvEntry iMtmData3 to 2 to indicate that a message has been processed

//SMS configuration file paths
_LIT(SMS_ISP_FILE,		"c:\\test\\bio\\iacp\\sms_isp.txt");

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

LOCAL_C void DisplayErrorReason(TInt& aReason, CBioTestUtils& aBioTestUtils)
	{
	if(aReason==KErrNone)
		{
		aBioTestUtils.TestHarnessCompleted();
		return;
		}

	//failure cases
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
			tempBuf.Format(_L("\r\n-->Error !!!= %d"), aReason );
			break;
		}

	aBioTestUtils.Printf(tempBuf);			  
	aBioTestUtils.TestHarnessFailed(aReason);
	}


//--------------------------------------
//
//-- CTestIacp --
//
enum TSessionState
	{
	EParse,  // parsing is done at ParseL() stage. and data are externalised.
	EProcess, //parsed data are internalised, then commited
	EDisplay,  //Display/Log entry data
	};

class CTestIacp : public CActive 
	{
	public:
		~CTestIacp();
		static  CTestIacp* NewL(CBioTestUtils* aBioTestUtils);
		void StartL(TInt aCmd);

	public:
		CTestIacp(CBioTestUtils* aBioTestUtils);
		void ConstructL();

		void RequestComplete(TRequestStatus& aStatus,TInt aCompletion);
		void DoCancel();
		void RunL();

		//logging utility functions...
		void GetMessageBodyL();
		void DisplayEntryDataL();
		void LogCommsDbSettingsL();
		void LogCommsDbDialOutIapL();
		void LogCommsDbDialInIapL();
		void LogCommsDbDialOutIspL();
		void LogCommsDbDialInIspL();
		void LogProxiesL();
		void LogCommsDbModemSettingsL();
		void LogCommsDbLocationSettingsL();
		void LogCommsDbChargeCardSettingsL();
		void LogCommsDbGlobalSettings();

	private:
		TInt iState;
		CBaseScriptParser2*	iParser;

		CBioTestUtils*		iBioTestUtils;
		TMsvId				iEntryId;       //ID of BioMsg entry created
		HBufC*				iIspName;
		TUint32				iProxyIsp;
		CCommsDatabase*		iCommDb;

		HBufC* iSmsMsg;			//sms body build dependent		
		TInt			iParseFailCount;//heap testing (Parsing state)
		TInt			iProcessFailCount;//heap testing (processing state)
	};


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
	
	iCommDb=CCommsDatabase::NewL(EDatabaseTypeIAP);
	iBioTestUtils->WriteComment(_L("Created CCommsDatabase Object."));


	TBool finished = EFalse;
#ifdef _DEBUG
//	TInt failCount = 0;
#endif
//	while(!finished)
//		{
//		__UHEAP_FAILNEXT(failCount++);
		TRAPD(error,(iParser = iBioTestUtils->CreateParserTypeL(KUidBIOInternetAccessPointMsg)));
		if (error == KErrNone)
			{
//			__UHEAP_RESET;
			iBioTestUtils->WriteComment(_L("Created IACP Parser object"));
			finished = ETrue;
//			__UHEAP_RESET;
			}
		// Handle error
		else
			{
//			test(error == KErrNoMemory);
//			__UHEAP_RESET;
			}
//		}

/*
	TRAPD(error, (iParser = iBioTestUtils->CreateParserTypeL(KUidBIOInternetAccessPointMsg)));
	if(error)
		{
		iBioTestUtils->WriteComment(_L("Failed to Create IACP Parser object"));
		iBioTestUtils->Printf(_L("error= %d\n"), error);
		User::Leave(error);
		}
	else
		iBioTestUtils->WriteComment(_L("Created IACP Parser object"));
*/
	
	TRAPD(err,(iEntryId= iBioTestUtils->CreateBIOEntryFromFileL(SMS_ISP_FILE, EBioIapSettingsMessage)));
	if(err)
		{
		iBioTestUtils->Printf(_L("Failed to Create Bio entry from %S\n"), &SMS_ISP_FILE);
		iBioTestUtils->Printf(_L("error= %d\n"), err);
		User::Leave(err);
		}
	else
		iBioTestUtils->Printf(_L("Created Bio entry with ID=0x%x from %S"),iEntryId, &SMS_ISP_FILE);

/*
	error = KErrNone;
	finished = EFalse;
	failCount = 0;
	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);

		TRAP(error,(iEntryId= iBioTestUtils->CreateBIOEntryFromFileL(SMS_MAIL_FILE, EBioIapSettingsMessage)));
		if (error == KErrNone)
			{
			__UHEAP_RESET;
			iBioTestUtils->Printf(_L("Created Bio entry with ID=0x%x from %S"),iEntryId, &SMS_MAIL_FILE);
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
*/
	CActiveScheduler::Add(this);
	}

CTestIacp::~CTestIacp()
	{
	Cancel();
	delete iSmsMsg;
 	delete iParser;
	delete iCommDb;
	delete iIspName;
	}

void CTestIacp::GetMessageBodyL()
	{
	delete iSmsMsg;
	iSmsMsg = NULL;
	iSmsMsg= iBioTestUtils->MessageBodyL(iEntryId).AllocL();
	}

void CTestIacp::StartL(TInt aCmd)
	{
	iBioTestUtils->WriteComment(_L("StartL()"));
	iState=aCmd;
	
	TInt error = KErrNone;
//	TBool finished = EFalse;

	switch (iState)
		{
		case EParse:
			iBioTestUtils->Printf(_L("Parsing...\n"));
			iBioTestUtils->SetEntryL(iEntryId); //clients have to set the right context before calling ParseL() 
			//iParser->ParseL(iStatus, iBioTestUtils->MessageBodyL(iEntryId)); //parse and externalise
			//SetActive();
			GetMessageBodyL();

//			while(!finished)
//				{
//				__UHEAP_FAILNEXT(iParseFailCount++);
				// 
				TRAP(error, iParser->ParseL(iStatus,iSmsMsg->Des())); 
				if (error == KErrNone)
					{
//					__UHEAP_RESET;
//					finished = ETrue;
					SetActive();
					}
				else
					{
//					test(error == KErrNoMemory);
//					__UHEAP_RESET;
					}		
//				}
			break;

		case EProcess:
			iBioTestUtils->Printf(_L("Committing...\n"));
			iBioTestUtils->SetEntryL(iEntryId); //clients have to set the right context before calling ProcessL() 
         	//iParser->ProcessL(iStatus); //internalise and process
 			//SetActive();
//			while(!finished)
//				{
//				__UHEAP_FAILNEXT(iProcessFailCount++);
				// 
				TRAP(error, iParser->ProcessL(iStatus)); //internalise and process
				if (error == KErrNone)
					{
//					__UHEAP_RESET;
//					finished = ETrue;
					SetActive();
					}
				else
					{
					test(error == KErrNoMemory);
//					__UHEAP_RESET;
					}		
//				}
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

void CTestIacp::DoCancel()
	{
	iBioTestUtils->WriteComment(_L("DoCancel()"));
	}

void CTestIacp::RequestComplete(TRequestStatus& aStatus,TInt aCompletion)
	{
	iBioTestUtils->WriteComment(_L("RequestComplete()"));
	TRequestStatus* statusPtr=&aStatus;
	User::RequestComplete(statusPtr,aCompletion);
	}

void CTestIacp::RunL()
	{
	iBioTestUtils->WriteComment(_L("RunL()"));
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
		}

	if ( result!=KErrNone && result!=KIacpErrSmsDataNotRestored) 
		{
		iBioTestUtils->ClearScreen();
		//test.Console()->SetPos(0,0);
		DisplayErrorReason(result,*iBioTestUtils);
		CActiveScheduler::Stop();
		return;
		}
	
	switch (iState)
		{
		case EParse:
			iBioTestUtils->WriteComment(_L("Parsed Ok."));
			StartL(EProcess);
			break;
		case EProcess:
			iBioTestUtils->WriteComment(_L("Processed Ok."));
			StartL(EDisplay);
			break;
		case EDisplay:

			DisplayErrorReason(result,*iBioTestUtils);
			iParser->Cancel();
			CActiveScheduler::Stop();
			break;
		default:
			break;
		}
	}

void CTestIacp::DisplayEntryDataL()
    {
	iBioTestUtils->WriteComment(_L("DisplayEntryDataL()"));	

	iBioTestUtils->ClearScreen();
    //test.Console()->SetPos(0,0);

	iBioTestUtils->SetEntryL(iEntryId);
    TMsvEntry entry = iBioTestUtils->Entry();
	iBioTestUtils->Printf(_L("===== BIO-MSG Entry Details =====\n"));
	iBioTestUtils->Printf(_L("Id:		0x%x\n"), iEntryId);
    iBioTestUtils->Printf(_L("iServiceId:     0x%x\n"), entry.iServiceId);
	iBioTestUtils->Printf(_L("iRelatedId:     0x%x\n"), entry.iRelatedId);

    iBioTestUtils->Printf(_L("iType:          0x%x\n"), entry.iType);
    iBioTestUtils->Printf(_L("iMtm:        0x%x\n"), entry.iMtm);
    TBuf<128> dateStr;
    entry.iDate.FormatL(dateStr, (_L("%D%M%Y%/0%1%/1%2%/2%3%/3")));
    iBioTestUtils->Printf(_L("iDate:         %S\n"),&dateStr);
    iBioTestUtils->Printf(_L("iSize:          %d\n"), entry.iSize);
    iBioTestUtils->Printf(_L("iError:         %d\n"), entry.iError);
    iBioTestUtils->Printf(_L("iBioType:		0x%x\n"), entry.iBioType);
    iBioTestUtils->Printf(_L("iMtmData1:      %d\n"), entry.iMtmData1);
    iBioTestUtils->Printf(_L("iMtmData2:      %d\n"), entry.iMtmData2);
    iBioTestUtils->Printf(_L("iMtmData3:      %d\n"), entry.iMtmData3);
    iBioTestUtils->Printf(_L("iDescription:   %S\n"),&entry.iDescription);
    iBioTestUtils->Printf(_L("iDetails:       %S\n"),&entry.iDetails);

    iBioTestUtils->ClearScreen();

	if (iBioTestUtils->Entry().iMtmData3 != BIO_MSG_ENTRY_PARSED &&
		iBioTestUtils->Entry().iMtmData3 != BIO_MSG_ENTRY_PROCESSED)
		{
		iBioTestUtils->WriteComment(_L("BioMg has not been parsed"));
		User::Leave(KErrGeneral);
		}

	iBioTestUtils->Printf(_L("===== Extracted Fields =====\n"));
	iBioTestUtils->LogExtractedFieldsL(iEntryId);
	//Get Isp Name from parsed fields
	CArrayPtrSeg<CParsedField>* parsedFieldArray= &(iBioTestUtils->ParsedFieldArray());

	for (TInt j = 0; j < parsedFieldArray->Count(); j++)
		{
		if(parsedFieldArray->At(j)->FieldName().CompareF(SMS_ISP_I_NAME) == 0 ||
				parsedFieldArray->At(j)->FieldName().CompareF(SMS_SCRIPT_NAME) == 0)
				{
				delete iIspName;
				iIspName= parsedFieldArray->At(j)->FieldValue().AllocL();
				}
		}

	LogCommsDbSettingsL();	
    }

void CTestIacp::LogCommsDbSettingsL()
	{
	iBioTestUtils->WriteComment(_L("LogCommsDbSettings()"));
	if(!iIspName)
		{
		iBioTestUtils->WriteComment(_L("ISP Name Not found in parsed data"));
		User::Leave(KErrNotFound);
		}

	LogCommsDbDialOutIapL();
	LogCommsDbDialInIapL();
	LogCommsDbDialOutIspL();
	LogCommsDbDialInIspL();
	LogProxiesL();
	LogCommsDbModemSettingsL();
	LogCommsDbLocationSettingsL();
	LogCommsDbChargeCardSettingsL();
	LogCommsDbGlobalSettings();
	}

void CTestIacp::LogCommsDbDialOutIapL()
	{
	iBioTestUtils->WriteComment(_L("\r\n============= DIAL OUT IAP TABLE ============"));
	TBuf<1024> tempBuf;
	TUint32 uval=0ul;
	CCommsDbTableView* view=iCommDb->OpenViewMatchingTextLC(TPtrC(IAP),TPtrC(COMMDB_NAME),iIspName->Des());
	TInt ret =view->GotoFirstRecord();
	if(ret== KErrNotFound) 
		{
		tempBuf = iIspName->Des();
		tempBuf.Append(_L(" record not found in IAP table"));
		iBioTestUtils->WriteComment(tempBuf);
		CleanupStack::PopAndDestroy(); //view
		return;
		}

	view->ReadTextL(TPtrC(COMMDB_NAME),		tempBuf);
	tempBuf.Insert(0, _L("COMMDB_NAME:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadUintL(TPtrC(COMMDB_ID),uval);
	tempBuf.Format(_L("COMMDB_ID:\t%d"), uval);
	iBioTestUtils->WriteComment(tempBuf);


	view->ReadUintL(TPtrC(IAP_ISP),			uval);
	tempBuf.Format(_L("IAP_ISP:\t%d"), uval);
	iBioTestUtils->WriteComment(tempBuf);

	//Taken of read IAP_MODEM and IAP_LOCATION as they are no longer valid fields in commdb

/*
	view->ReadUintL(TPtrC(IAP_CHARGECARD),	uval);
	tempBuf.Format(_L("IAP_CHARGECARD:\t%d"), uval);
	iBioTestUtils->WriteComment(tempBuf);
*/
	
	CleanupStack::PopAndDestroy(); //view
	}

void CTestIacp::LogCommsDbDialInIapL()
	{
	iBioTestUtils->WriteComment(_L("\r\n============= DIAL IN IAP TABLE ============"));
	TBuf<1024> tempBuf;
	TUint32 uval=0ul;
	CCommsDbTableView* view=iCommDb->OpenViewMatchingTextLC(TPtrC(DIAL_IN_IAP),TPtrC(COMMDB_NAME),iIspName->Des());
	TInt ret =view->GotoFirstRecord();
	if(ret== KErrNotFound) 
		{
		tempBuf = iIspName->Des();
		tempBuf.Append(_L(" record not found in DIAL_IN_IAP table"));
		iBioTestUtils->WriteComment(tempBuf);
		CleanupStack::PopAndDestroy(); //view
		return;
		}

	view->ReadTextL(TPtrC(COMMDB_NAME),		tempBuf);
	tempBuf.Insert(0, _L("COMMDB_NAME:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadUintL(TPtrC(COMMDB_ID),uval);
	tempBuf.Format(_L("COMMDB_ID:\t%d"), uval);
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadUintL(TPtrC(IAP_ISP),uval);
	tempBuf.Format(_L("IAP_ISP:\t%d"), uval);
	iBioTestUtils->WriteComment(tempBuf);

	//Taken out reading of IAP_MODEM test as it is not part of commdb anymore

	CleanupStack::PopAndDestroy(); //view
	}

void CTestIacp::LogCommsDbDialOutIspL()
	{
	iBioTestUtils->WriteComment(_L("\r\n============= DIAL OUT ISP TABLE ============"));
	TBuf<1024> tempBuf;
	TBool boolVal;
	TUint32 uval=0ul;
	CCommsDbTableView* view=iCommDb->OpenViewMatchingTextLC(TPtrC(DIAL_OUT_ISP),TPtrC(COMMDB_NAME),iIspName->Des());
	TInt ret =view->GotoFirstRecord();
	if(ret== KErrNotFound) 
		{
		tempBuf = iIspName->Des();
		tempBuf.Append(_L(" record not found in DIAL_OUT_ISP table"));
		iBioTestUtils->WriteComment(tempBuf);
		CleanupStack::PopAndDestroy(); //view
		return;
		}

	view->ReadTextL(TPtrC(COMMDB_NAME),	tempBuf);
	tempBuf.Insert(0, _L("COMMDB_NAME:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadUintL(TPtrC(COMMDB_ID),uval);
	iProxyIsp=uval;
	tempBuf.Format(_L("COMMDB_ID:\t%d"), uval);
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadBoolL(TPtrC(ISP_DIAL_RESOLUTION),	boolVal);
	tempBuf.Format(_L("ISP_DIAL_RESOLUTION:\t%d"), boolVal);
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadTextL(TPtrC(ISP_DEFAULT_TEL_NUM),	tempBuf);
	tempBuf.Insert(0, _L("ISP_DEFAULT_TEL_NUM:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadBoolL(TPtrC(ISP_PROMPT_FOR_LOGIN), boolVal);
	tempBuf.Format(_L("ISP_PROMPT_FOR_LOGIN:\t%d"), boolVal);
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadBoolL(TPtrC(ISP_USE_LOGIN_SCRIPT), boolVal);	//EFalse by default
	tempBuf.Format(_L("ISP_USE_LOGIN_SCRIPT:\t%d"), boolVal);
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadTextL(TPtrC(ISP_LOGIN_NAME), tempBuf);
	tempBuf.Insert(0, _L("ISP_LOGIN_NAME:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadTextL(TPtrC(ISP_LOGIN_PASS), tempBuf);
	tempBuf.Insert(0, _L("ISP_LOGIN_PASS:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadBoolL(TPtrC(ISP_DISPLAY_PCT), boolVal);
	tempBuf.Format(_L("ISP_DISPLAY_PCT:\t%d"), boolVal);
	iBioTestUtils->WriteComment(tempBuf);

/*
	view->ReadTextL(TPtrC(ISP_IF_NAME), tempBuf);
	tempBuf.Insert(0, _L("ISP_IF_NAME:\t"));
	iBioTestUtils->WriteComment(tempBuf);
*/

	view->ReadTextL(TPtrC(ISP_IF_PARAMS), tempBuf);
	tempBuf.Insert(0, _L("ISP_IF_PARAMS:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadTextL(TPtrC(ISP_IF_NETWORKS), tempBuf);
	tempBuf.Insert(0, _L("ISP_IF_NETWORKS:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadBoolL(TPtrC(ISP_IF_PROMPT_FOR_AUTH), boolVal);
	tempBuf.Format(_L("ISP_IF_PROMPT_FOR_AUTH:\t%d"), boolVal);
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadTextL(TPtrC(ISP_IF_AUTH_NAME), tempBuf);
	tempBuf.Insert(0, _L("ISP_IF_AUTH_NAME:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadTextL(TPtrC(ISP_IF_AUTH_PASS), tempBuf);
	tempBuf.Insert(0, _L("ISP_IF_AUTH_PASS:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadBoolL(TPtrC(ISP_IF_CALLBACK_ENABLED), boolVal);
	tempBuf.Format(_L("ISP_IF_CALLBACK_ENABLED:\t%d"), boolVal);
	iBioTestUtils->WriteComment(tempBuf);

//	view->ReadUintL(TPtrC(ISP_IF_CALLBACK_TYPE), uval);
//	tempBuf.Format(_L("ISP_IF_CALLBACK_TYPE:\t%d"), uval);
//	iBioTestUtils->WriteComment(tempBuf);

	TBuf8<256> tempBuf8;
	view->ReadTextL(TPtrC(ISP_IF_CALLBACK_INFO), tempBuf8);
	tempBuf.Copy(tempBuf8);
	tempBuf.Insert(0, _L("ISP_IF_CALLBACK_INFO:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadBoolL(TPtrC(ISP_IP_ADDR_FROM_SERVER),	boolVal);
	tempBuf.Format(_L("ISP_IP_ADDR_FROM_SERVER:\t%d"), boolVal);
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadTextL(TPtrC(ISP_IP_ADDR),	tempBuf);
	tempBuf.Insert(0, _L("ISP_IP_ADDR:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadTextL(TPtrC(ISP_IP_NETMASK), tempBuf);
	tempBuf.Insert(0, _L("ISP_IP_NETMASK:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadTextL(TPtrC(ISP_IP_GATEWAY), tempBuf);
	tempBuf.Insert(0, _L("ISP_IP_GATEWAY:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadBoolL(TPtrC(ISP_IP_DNS_ADDR_FROM_SERVER),	boolVal);
	tempBuf.Format(_L("ISP_IP_DNS_ADDR_FROM_SERVER:\t%d"), boolVal);
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadTextL(TPtrC(ISP_IP_NAME_SERVER1), tempBuf);	
	tempBuf.Insert(0, _L("ISP_IP_NAME_SERVER1:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadTextL(TPtrC(ISP_IP_NAME_SERVER2),	tempBuf);
	tempBuf.Insert(0, _L("ISP_IP_NAME_SERVER2:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadBoolL(TPtrC(ISP_ENABLE_IP_HEADER_COMP),	boolVal);
	tempBuf.Format(_L("ISP_ENABLE_IP_HEADER_COMP:\t%d"), boolVal);
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadBoolL(TPtrC(ISP_DISABLE_PLAIN_TEXT_AUTH),	boolVal);
	tempBuf.Format(_L("ISP_DISABLE_PLAIN_TEXT_AUTH:\t%d"), boolVal);
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadBoolL(TPtrC(ISP_ENABLE_SW_COMP),	boolVal);
	tempBuf.Format(_L("ISP_ENABLE_SW_COMP:\t%d"), boolVal);
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadTextL(TPtrC(ISP_DESCRIPTION),	tempBuf);
	tempBuf.Insert(0, _L("ISP_DESCRIPTION:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadTextL(TPtrC(ISP_INIT_STRING),	tempBuf8);
	tempBuf.Copy(tempBuf8);
	tempBuf.Insert(0, _L("ISP_INIT_STRING:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	HBufC* existingScript=0;
	existingScript= view->ReadLongTextLC(TPtrC(ISP_LOGIN_SCRIPT));
	iBioTestUtils->WriteComment(_L("ISP_LOGIN_SCRIPT: "));
	if(existingScript->Length() > 0)
		{
		//Log script (64 chars at time)
		TPtrC ptr= existingScript->Des();
		TInt scriptLength = existingScript->Length();
		TInt BufSize=64;
		TInt count=0;
		while(count <= scriptLength)
			{
			if(scriptLength-count > BufSize )
				tempBuf= ptr.Mid(count,BufSize);
			else
				tempBuf=ptr.Mid(count,scriptLength-count);

			iBioTestUtils->WriteComment(tempBuf);
			count+=BufSize;
			}
		}

	CleanupStack::PopAndDestroy(2); // existingScript, view
	}

void CTestIacp::LogCommsDbDialInIspL()
	{
	iBioTestUtils->WriteComment(_L("\r\n============= DIAL IN ISP TABLE ============"));
	TBuf<1024> tempBuf;
	TUint32 uval=0ul;
	CCommsDbTableView* view=iCommDb->OpenViewMatchingTextLC(TPtrC(DIAL_IN_ISP),TPtrC(COMMDB_NAME),iIspName->Des());
	TInt ret =view->GotoFirstRecord();
	if(ret== KErrNotFound) 
		{
		tempBuf = iIspName->Des();
		tempBuf.Append(_L(" record not found in DIAL_IN_ISP table!!!"));
		iBioTestUtils->WriteComment(tempBuf);
		CleanupStack::PopAndDestroy(); //view
		return;
		}

	view->ReadTextL(TPtrC(COMMDB_NAME),		tempBuf);
	tempBuf.Insert(0, _L("COMMDB_NAME:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadUintL(TPtrC(COMMDB_ID),uval);
	tempBuf.Format(_L("COMMDB_ID:\t%d"), uval);
	iBioTestUtils->WriteComment(tempBuf);

	CleanupStack::PopAndDestroy(); //view
	}

void CTestIacp::LogProxiesL()
	{
	iBioTestUtils->WriteComment(_L("\r\n============= PROXIES TABLE ============"));
	TBuf<1024> tempBuf;
	TUint32 uval=0ul;
	TBool boolVal;
	CCommsDbTableView* view=iCommDb->OpenViewMatchingUintLC(TPtrC(PROXIES),TPtrC(PROXY_ISP),iProxyIsp);
	TInt ret =view->GotoFirstRecord();
	if(ret== KErrNotFound) 
		{
		tempBuf = iIspName->Des();
		tempBuf.Append(_L(" record not found in PROXIES table!!!"));
		iBioTestUtils->WriteComment(tempBuf);
		CleanupStack::PopAndDestroy(); //view
		return;
		}

//	view->ReadTextL(TPtrC(COMMDB_NAME),		tempBuf);
//	tempBuf.Insert(0, _L("COMMDB_NAME:\t"));
//	iBioTestUtils->WriteComment(tempBuf);

	view->ReadUintL(TPtrC(COMMDB_ID),uval);
	tempBuf.Format(_L("COMMDB_ID:\t%d"), uval);
	iBioTestUtils->WriteComment(tempBuf);
	
	view->ReadUintL(TPtrC(PROXY_ISP),uval);
	tempBuf.Format(_L("PROXY_ISP:\t%d"), uval);
	iBioTestUtils->WriteComment(tempBuf);

	HBufC* proxyServerName=0;
	proxyServerName=view->ReadLongTextLC(TPtrC(PROXY_SERVER_NAME));
	iBioTestUtils->WriteComment(_L("PROXY_SERVER_NAME: "));
	if(proxyServerName->Length() > 0)
		{
		//Log script (64 chars at time)
		TPtrC ptr= proxyServerName->Des();
		TInt proxyLength = proxyServerName->Length();
		TInt BufSize=64;
		TInt count=0;
		while(count <= proxyLength)
			{
			if(proxyLength-count > BufSize )
				tempBuf= ptr.Mid(count,BufSize);
			else
				tempBuf=ptr.Mid(count,proxyLength-count);

			iBioTestUtils->WriteComment(tempBuf);
			count+=BufSize;
			}
		}

	view->ReadBoolL(TPtrC(PROXY_USE_PROXY_SERVER),	boolVal);
	tempBuf.Format(_L("PROXY_USE_PROXY_SERVER:\t%d"), boolVal);
	iBioTestUtils->WriteComment(tempBuf);


	view->ReadTextL(TPtrC(PROXY_PROTOCOL_NAME),tempBuf);
	tempBuf.Insert(0, _L("PROXY_PROTOCOL_NAME:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadUintL(TPtrC(PROXY_PORT_NUMBER),uval);
	tempBuf.Format(_L("PROXY_PORT_NUMBER:\t%d"), uval);
	iBioTestUtils->WriteComment(tempBuf);
	
	HBufC* proxyExeptions=0;	
	proxyExeptions=view->ReadLongTextLC(TPtrC(PROXY_EXCEPTIONS));
	iBioTestUtils->WriteComment(_L("PROXY_EXCEPTIONS: "));
	if(proxyExeptions->Length() > 0)
		{
		//Log script (64 chars at time)
		TPtrC ptr= proxyExeptions->Des();
		TInt proxyLength = proxyExeptions->Length();
		TInt BufSize=64;
		TInt count=0;
		while(count <= proxyLength)
			{
			if(proxyLength-count > BufSize )
				tempBuf= ptr.Mid(count,BufSize);
			else
				tempBuf=ptr.Mid(count,proxyLength-count);

			iBioTestUtils->WriteComment(tempBuf);
			count+=BufSize;
			}
		}

	CleanupStack::PopAndDestroy(3); // proxyExeptions,proxyServerName, view
	}
void CTestIacp::LogCommsDbModemSettingsL()
	{
	iBioTestUtils->WriteComment(_L("\r\n============= MODEM TABLE ============"));
	TBuf<1024> tempBuf;
	TUint32 uval=0ul;
	CCommsDbTableView* view=iCommDb->OpenViewMatchingTextLC(TPtrC(MODEM),TPtrC(COMMDB_NAME),iIspName->Des());
	TInt ret =view->GotoFirstRecord();
	if(ret== KErrNotFound) 
		{
		tempBuf = iIspName->Des();
		tempBuf.Append(_L(" record not found in MODEM table!!!"));
		iBioTestUtils->WriteComment(tempBuf);
		CleanupStack::PopAndDestroy(); //view
		return;
		}

	view->ReadTextL(TPtrC(COMMDB_NAME),		tempBuf);
	tempBuf.Insert(0, _L("COMMDB_NAME:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadUintL(TPtrC(COMMDB_ID),uval);
	tempBuf.Format(_L("COMMDB_ID:\t%d"), uval);
	iBioTestUtils->WriteComment(tempBuf);

	CleanupStack::PopAndDestroy(); //view
	}

void CTestIacp::LogCommsDbLocationSettingsL()
	{
	iBioTestUtils->WriteComment(_L("\r\n============= LOCATION TABLE ============"));
	TBuf<1024> tempBuf;
	TUint32 uval=0ul;
	CCommsDbTableView* view=iCommDb->OpenViewMatchingTextLC(TPtrC(LOCATION),TPtrC(COMMDB_NAME),iIspName->Des());
	TInt ret =view->GotoFirstRecord();
	if(ret== KErrNotFound) 
		{
		tempBuf = iIspName->Des();
		tempBuf.Append(_L(" record not found in LOCATION table!!!"));
		iBioTestUtils->WriteComment(tempBuf);
		CleanupStack::PopAndDestroy(); //view
		return;
		}

	view->ReadTextL(TPtrC(COMMDB_NAME),		tempBuf);
	tempBuf.Insert(0, _L("COMMDB_NAME:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadUintL(TPtrC(COMMDB_ID),uval);
	tempBuf.Format(_L("COMMDB_ID:\t%d"), uval);
	iBioTestUtils->WriteComment(tempBuf);

	CleanupStack::PopAndDestroy(); //view
	}

void CTestIacp::LogCommsDbChargeCardSettingsL()
	{
	iBioTestUtils->WriteComment(_L("\r\n============= CHARGECARD TABLE ============"));
	TBuf<1024> tempBuf;
	TUint32 uval=0ul;
	CCommsDbTableView* view=iCommDb->OpenViewMatchingTextLC(TPtrC(CHARGECARD),TPtrC(COMMDB_NAME),iIspName->Des());
	TInt ret =view->GotoFirstRecord();
	if(ret== KErrNotFound) 
		{
		tempBuf = iIspName->Des();
		tempBuf.Append(_L(" record not found in CHARGECARD table!!!"));
		iBioTestUtils->WriteComment(tempBuf);
		CleanupStack::PopAndDestroy(); //view
		return;
		}

	view->ReadTextL(TPtrC(COMMDB_NAME),		tempBuf);
	tempBuf.Insert(0, _L("COMMDB_NAME:\t"));
	iBioTestUtils->WriteComment(tempBuf);

	view->ReadUintL(TPtrC(COMMDB_ID),uval);
	tempBuf.Format(_L("COMMDB_ID:\t%d"), uval);
	iBioTestUtils->WriteComment(tempBuf);

	CleanupStack::PopAndDestroy(); //view
	}

void CTestIacp::LogCommsDbGlobalSettings()
	{
	}

LOCAL_C void doMainL()
	{
	//create a scheduler
	exampleScheduler = new (ELeave) CExampleScheduler;
	CleanupStack::PushL( exampleScheduler );
	CActiveScheduler::Install( exampleScheduler );

	CBioTestUtils* BioTestUtils = CBioTestUtils::NewL(test,ETuGoClientSide | ETuDeleteService | ETuCreateService);
	CleanupStack::PushL(BioTestUtils);

	BioTestUtils->GoClientSideL();
	BioTestUtils->WriteComment(_L("Msv Client Side Created"));

	CTestIacp* testParser = CTestIacp::NewL(BioTestUtils);
	CleanupStack::PushL( testParser );
	test(testParser!=NULL);
	BioTestUtils->WriteComment(_L("CTestIacp Object Created."));

	testParser->StartL(EParse);

	CActiveScheduler::Start();

 	CleanupStack::PopAndDestroy(3);  // testParser, BioTestUtils,exampleScheduler 

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
