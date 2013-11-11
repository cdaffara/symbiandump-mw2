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

//
#include "biotestutils.h"
#include <smtpset.h>
#include <pop3set.h>
#include <iapprefs.h>
#include <imapset.h>
#include <smutset.h>
//
#include "IACPERR.H"
#include "IACPDEF.H"

#include <cemailaccounts.h>
#include <csmsaccount.h>
#include "tmsvbioinfo.h"

#define BIO_MSG_ENTRY_PARSED     1 // Set entry .iMtmData3 to 1 to indicate that the entry has store i.e parsed,externalised
#define BIO_MSG_ENTRY_PROCESSED	 2 // Set TMsvEntry iMtmData3 to 2 to indicate that a message has been processed

//SMS configuration file paths
_LIT(SMS_MAIL_FILE,		"c:\\test\\bio\\iacp\\sms_mail.txt");

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
	EDisplay,  //Display entry data
	EDisplayMailService, //Display mail service entry created
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
		void GetMessageBodyL();
		void DisplayEntryDataL();
		void DisplayMailServiceL();
		void ReadSmtpServiceL();
		void ReadPop3ServiceL();
		void ReadImap4ServiceL();
		void ReadSmsServiceL();
		void PrintSmtpSettings(CImSmtpSettings*		aSmtpSettings);
		void PrintPop3Settings(CImPop3Settings*		aSmtpSettings);
		void PrintImap4Settings(CImImap4Settings*	aImap4Settings);
		void PrintSmsSettings(CSmsSettings*		aSmsSettings);

		void RequestComplete(TRequestStatus& aStatus,TInt aCompletion);
		void DoCancel();
		void RunL();

	private:
		TInt iState;
		CBaseScriptParser2*	iParser;
		CBioTestUtils*		iBioTestUtils;
		TBuf<64>            iIspName;
		TMsvId				iEntryId;       //ID of sms entry
		HBufC* iSmsMsg;			//sms body build dependent		
		TInt			iParseFailCount;//heap testing (Parsing state)
		TInt			iProcessFailCount;//heap testing (processing state)
		CEmailAccounts* iEmailAccounts;
		CSmsAccount*	iSmsAccount;
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

	iEmailAccounts = CEmailAccounts::NewL();
	iSmsAccount = CSmsAccount::NewL();

	TInt error = KErrNone;
	TBool finished = EFalse;
#ifdef _DEBUG
	TInt failCount = 0;
#endif
	while(!finished)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAPD(error,(iParser = iBioTestUtils->CreateParserTypeL(KUidBIOInternetAccessPointMsg)));
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

	
	TRAP(error,(iEntryId= iBioTestUtils->CreateBIOEntryFromFileL(SMS_MAIL_FILE, EBioIapSettingsMessage)));
	if(error)
		{
		iBioTestUtils->Printf(_L("Failed to Create Bio entry from %S\n"), &SMS_MAIL_FILE);
		iBioTestUtils->Printf(_L("error= %d\n"), error);
		User::Leave(error);
		}
	else
		iBioTestUtils->Printf(_L("Created Bio entry with ID=0x%x from %S"),iEntryId, &SMS_MAIL_FILE);
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
	delete iParser;				//	The order of these two deletes is important.
	
	delete iEmailAccounts;
	delete iSmsAccount;
	}

void CTestIacp::GetMessageBodyL()
	{
	delete iSmsMsg;
	iSmsMsg = NULL;
	iSmsMsg= iBioTestUtils->MessageBodyL(iEntryId).AllocL();
	}

void CTestIacp::StartL(TInt aCmd)
	{
	TInt error = KErrNone;
	TBool finished = EFalse;
	iState=aCmd;

	switch (iState)
		{
		case EParse:
			iBioTestUtils->Printf(_L("Parsing...\n"));
			iBioTestUtils->SetEntryL(iEntryId); //clients have to set the right context before calling ParseL() 
			GetMessageBodyL();

			while(!finished)
				{
				__UHEAP_FAILNEXT(iParseFailCount++);
				// 
				TRAP(error, iParser->ParseL(iStatus,iSmsMsg->Des()));
				if (error == KErrNone)
					{
					__UHEAP_RESET;
					finished = ETrue;
					SetActive();
					}
				else
					{
					test(error == KErrNoMemory);
					__UHEAP_RESET;
					}		
				}
				
			break;

		case EProcess:
			iBioTestUtils->Printf(_L("Processing...\n"));
			iBioTestUtils->SetEntryL(iEntryId); //clients have to set the right context before calling ProcessL() 
			while(!finished)
				{
				__UHEAP_FAILNEXT(iProcessFailCount++);
				// 
				TRAP(error, iParser->ProcessL(iStatus)); //internalise and process
				if (error == KErrNone)
					{
					__UHEAP_RESET;
					finished = ETrue;
					SetActive();
					}
				else
					{
					test(error == KErrNoMemory);
					__UHEAP_RESET;
					}		
				}
			break;

		case EDisplay:
            DisplayEntryDataL();
			RequestComplete(iStatus,KErrNone);
            SetActive();
			break;

		case EDisplayMailService:
			DisplayMailServiceL();
			RequestComplete(iStatus,KErrNone);
            SetActive();
			break;
		default:
			break;
		}
	}

void CTestIacp::DoCancel()
	{
	}

void CTestIacp::RequestComplete(TRequestStatus& aStatus,TInt aCompletion)
	{
	TRequestStatus* statusPtr=&aStatus;
	User::RequestComplete(statusPtr,aCompletion);
	}

void CTestIacp::RunL()
	{
	TInt result=iStatus.Int();

	//heap test failure
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
		DisplayErrorReason(result, *iBioTestUtils);
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
			StartL(EDisplayMailService);
			break;
		case EDisplayMailService:
			DisplayErrorReason(result, *iBioTestUtils);
			iParser->Cancel();
			CActiveScheduler::Stop();
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
	iBioTestUtils->Printf(_L("Entry Id:	      0x%x\n"), iEntryId);
    iBioTestUtils->Printf(_L("iServiceId:     0x%x\n"), entry.iServiceId);
    iBioTestUtils->Printf(_L("iRelatedId:     0x%x\n"), entry.iRelatedId);
    iBioTestUtils->Printf(_L("iType:          0x%x\n"), entry.iType);
    iBioTestUtils->Printf(_L("iMtm:           0x%x\n"), entry.iMtm);
    TBuf<128> dateStr;
    entry.iDate.FormatL(dateStr, (_L("%D%M%Y%/0%1%/1%2%/2%3%/3")));
    iBioTestUtils->Printf(_L("iDate:         %S\n"),&dateStr);

    iBioTestUtils->Printf(_L("iSize:          %d\n"), entry.iSize);
    iBioTestUtils->Printf(_L("iError:         %d\n"), entry.iError);
    iBioTestUtils->Printf(_L("iBioType:		0x%x\n"), entry.iBioType);
    iBioTestUtils->Printf(_L("iMtmData1:      %d\n"), entry.MtmData1());
    iBioTestUtils->Printf(_L("iMtmData2:      %d\n"), entry.MtmData2());
    iBioTestUtils->Printf(_L("iMtmData3:      %d\n"), entry.MtmData3());
    iBioTestUtils->Printf(_L("iDescription:   %S\n"),&entry.iDescription);
    iBioTestUtils->Printf(_L("iDetails:       %S\n"),&entry.iDetails);

    iBioTestUtils->ClearScreen();

	if (iBioTestUtils->Entry().MtmData3() != BIO_MSG_ENTRY_PARSED &&
		iBioTestUtils->Entry().MtmData3() != BIO_MSG_ENTRY_PROCESSED)
		{
		iBioTestUtils->WriteComment(_L("BioMg has not been parsed"));
		User::Leave(KErrGeneral);
		}

	iBioTestUtils->Printf(_L("===== Extracted Fields =====\n"));
	iBioTestUtils->LogExtractedFieldsL(iEntryId);
    }

void CTestIacp::DisplayMailServiceL()
	{
	TMsvId smtpEntryId= KMsvNullIndexEntryId;
	TMsvId smtpRelatedId= KMsvNullIndexEntryId;
	TMsvId pop3EntryId= KMsvNullIndexEntryId;
	TMsvId pop3RelatedId= KMsvNullIndexEntryId;
	TMsvId imap4EntryId= KMsvNullIndexEntryId;
	TMsvId imap4RelatedId= KMsvNullIndexEntryId;
	TMsvId smsEntryId	=	KMsvNullIndexEntryId;

	//Get Isp Name from parsed fields
	CArrayPtrSeg<CParsedField>* parsedFieldArray= &(iBioTestUtils->ParsedFieldArray());

	//first set iIspName 
	for (TInt i = 0; i < parsedFieldArray->Count(); i++)
		if(parsedFieldArray->At(i)->FieldName().CompareF(SMS_ISP_M_NAME)==0)
			iIspName = parsedFieldArray->At(i)->FieldValue();

	// Get list of children IDs
	iBioTestUtils->SetEntryL(KMsvRootIndexEntryId);
	CMsvEntrySelection *msvSelection=iBioTestUtils->ChildrenWithTypeLC(KUidMsvServiceEntry);
	
	TMsvEntry entry;
	TInt count = msvSelection->Count(); 
	for (TInt j = 0; j < count; j++)
		{
		// set context to service entry
		iBioTestUtils->SetEntryL((*msvSelection)[j]);
		entry = iBioTestUtils->Entry(); 
		if (entry.iType == KUidMsvServiceEntry)
		    if(entry.iDetails.Length() == iIspName.Length())
				if(entry.iDetails.CompareF(iIspName)==0 )
					{
					if(entry.iMtm == KUidMsgTypeSMTP)
						{
						smtpEntryId= entry.Id();
						smtpRelatedId= entry.iRelatedId;
						ReadSmtpServiceL();
						}
					else if(entry.iMtm == KUidMsgTypePOP3)
						{
						pop3EntryId= entry.Id();
						pop3RelatedId= entry.iRelatedId;
						ReadPop3ServiceL();
						}
					else if(entry.iMtm == KUidMsgTypeIMAP4)
						{
						imap4EntryId= entry.Id();
						imap4RelatedId= entry.iRelatedId;
						ReadImap4ServiceL();
						}
					else if(entry.iMtm == KUidMsgTypeSMS)
						{
						imap4EntryId= entry.Id();
						ReadSmsServiceL();
						}
					}
		}
	CleanupStack::PopAndDestroy();//msvSelection

	iBioTestUtils->WriteComment(_L("\r\n================ Service Entry IDs and Related IDs ================"));
	TBuf<64> tempBuf;
	tempBuf.Format(_L("Smtp service entry Id:\t%d"), smtpEntryId);
	iBioTestUtils->WriteComment(tempBuf);
	tempBuf.Format(_L("Smtp service entry Related Id:\t%d"), smtpRelatedId);
	iBioTestUtils->WriteComment(tempBuf);

	tempBuf.Format(_L("Pop3 service entry Id:\t%d"), pop3EntryId);
	iBioTestUtils->WriteComment(tempBuf);
	tempBuf.Format(_L("Pop3 service entry Related Id:\t%d"), pop3RelatedId);
	iBioTestUtils->WriteComment(tempBuf);

	tempBuf.Format(_L("Imap4 service entry Id:\t%d"), imap4EntryId);
	iBioTestUtils->WriteComment(tempBuf);
	tempBuf.Format(_L("Imap4 service entry Related Id:\t%d"), imap4RelatedId);
	iBioTestUtils->WriteComment(tempBuf);

	tempBuf.Format(_L("Sms service entry Id:\t%d"), smsEntryId);
	iBioTestUtils->WriteComment(tempBuf);
	}

void CTestIacp::ReadSmtpServiceL()
	{
	//iEntry should point to smtp service entry
	CImSmtpSettings* smtpSettings=new(ELeave) CImSmtpSettings();	
	CleanupStack::PushL(smtpSettings);
	
	CImIAPPreferences* smtpIapPreferences = CImIAPPreferences::NewLC();
	
	TSmtpAccount smtpAccount;
 	iEmailAccounts->GetSmtpAccountL(iBioTestUtils->iMsvEntry->EntryId(), smtpAccount);
 	iEmailAccounts->LoadSmtpSettingsL(smtpAccount, *smtpSettings);
 	iEmailAccounts->LoadSmtpIapSettingsL(smtpAccount, *smtpIapPreferences);

	PrintSmtpSettings(smtpSettings);
	CleanupStack::PopAndDestroy(2, smtpSettings); // smtpIapPreferences, smtpSettings
	}

void CTestIacp::ReadPop3ServiceL()
	{
	//iEntry should point to po3 service entry
	CImPop3Settings* pop3Settings=new(ELeave) CImPop3Settings();
	CleanupStack::PushL(pop3Settings);
	
	CImIAPPreferences* pop3IapPreferences = CImIAPPreferences::NewLC();
	
	TPopAccount popAccount;
 	iEmailAccounts->GetPopAccountL(iBioTestUtils->iMsvEntry->EntryId(), popAccount);
 	iEmailAccounts->LoadPopSettingsL(popAccount, *pop3Settings);
 	iEmailAccounts->LoadPopIapSettingsL(popAccount, *pop3IapPreferences);

	PrintPop3Settings(pop3Settings);
	CleanupStack::PopAndDestroy(2, pop3Settings); // pop3IapPreferences, smtpSettings
	}

void CTestIacp::ReadImap4ServiceL()
	{
	//iEntry should point to imap4 service entry
	CImImap4Settings* imap4Settings=new(ELeave) CImImap4Settings();	
	CleanupStack::PushL(imap4Settings);
	
	CImIAPPreferences* imap4IapPreferences = CImIAPPreferences::NewLC();
	
	TImapAccount imapAccount;
 	iEmailAccounts->GetImapAccountL(iBioTestUtils->iMsvEntry->EntryId(), imapAccount);
 	iEmailAccounts->LoadImapSettingsL(imapAccount, *imap4Settings);
 	iEmailAccounts->LoadImapIapSettingsL(imapAccount, *imap4IapPreferences);
	
	PrintImap4Settings(imap4Settings);
	CleanupStack::PopAndDestroy(2, imap4Settings); // imap4IapPreferences, smtpSettings
	}

void CTestIacp::ReadSmsServiceL()
	{
	//iEntry should point to imap4 service entry
	CSmsSettings* smsSettings=CSmsSettings::NewL();
	CleanupStack::PushL(smsSettings);
	iSmsAccount->LoadSettingsL(*smsSettings);
	PrintSmsSettings(smsSettings);
	CleanupStack::PopAndDestroy(smsSettings);
	}

void CTestIacp::PrintSmtpSettings(CImSmtpSettings* aSmtpSettings)
	{
	iBioTestUtils->ClearScreen();
    //test.Console()->SetPos(0,0);

	iBioTestUtils->Printf(_L("====== SMTP Service entry=====\n"));
	TBuf<256> tempBuf;	  //  temp logging buffer
	tempBuf = aSmtpSettings->ServerAddress();
	iBioTestUtils->Printf(_L("ServerAddress: %S\n"), &tempBuf);
	tempBuf = aSmtpSettings->EmailAddress();
	iBioTestUtils->Printf(_L("EmailAddress: %S\n"), &tempBuf);
	iBioTestUtils->Printf(_L("Port: %d\n"), aSmtpSettings->Port());
	tempBuf=aSmtpSettings->EmailAlias();
	iBioTestUtils->Printf(_L("EmailAlias: %S\n"), &tempBuf);
	tempBuf=aSmtpSettings->ReplyToAddress();
	iBioTestUtils->Printf(_L("ReplyToAddress: %S\n"), &tempBuf);
	tempBuf=aSmtpSettings->ReceiptAddress();
	iBioTestUtils->Printf(_L("ReceiptAddress: %S\n"), &tempBuf);
	iBioTestUtils->Printf(_L("BodyEncoding: %d\n"), aSmtpSettings->BodyEncoding());
	}

void CTestIacp::PrintPop3Settings(CImPop3Settings* aSmtpSettings)
	{
	iBioTestUtils->ClearScreen();
    //test.Console()->SetPos(0,0);

	iBioTestUtils->Printf(_L("====== POP3 Service entry=====\n"));
	TBuf<256> tempBuf;	  //  temp logging buffer
	//LoginName and Password are stored as 8 bit Descriptors so we need to convert them 
	// into 16 bit Unicode to be able to display them on the console
	tempBuf.Copy(aSmtpSettings->LoginName());
	iBioTestUtils->Printf(_L("LoginName: %S\n"), &tempBuf);
	tempBuf.Copy(aSmtpSettings->Password());
	iBioTestUtils->Printf(_L("Password: %S\n"), &tempBuf);
	tempBuf = aSmtpSettings->ServerAddress();
	iBioTestUtils->Printf(_L("ServerAddress: %S\n"), &tempBuf);
	iBioTestUtils->Printf(_L("Port: %d\n"),		aSmtpSettings->Port());
	iBioTestUtils->Printf(_L("Apop: %d\n"),		aSmtpSettings->Apop());
	iBioTestUtils->Printf(_L("AutoSendOnConnect: %d\n"),  aSmtpSettings->AutoSendOnConnect());
	}

void CTestIacp::PrintImap4Settings(	CImImap4Settings* aSmtpSettings)
	{
	iBioTestUtils->ClearScreen();

	iBioTestUtils->Printf(_L("====== IMAP4 Service entry=====\n"));
	TBuf<256> tempBuf;	  //  temp logging buffer
	//LoginName and Password are stored as 8 bit Descriptors so we need to convert them 
	// into 16 bit Unicode to be able to display them on the console
	tempBuf.Copy(aSmtpSettings->LoginName());
	iBioTestUtils->Printf(_L("LoginName: %S\n"), &tempBuf);
	tempBuf.Copy(aSmtpSettings->Password());
	iBioTestUtils->Printf(_L("Password: %S\n"), &tempBuf);
	tempBuf=aSmtpSettings->ServerAddress();
	iBioTestUtils->Printf(_L("ServerAddress: %S\n"), &tempBuf);	
	iBioTestUtils->Printf(_L("Port: %d\n"),  	aSmtpSettings->Port());	
	tempBuf.Copy(aSmtpSettings->FolderPath());
	iBioTestUtils->Printf(_L("FolderPath: %S\n"), &tempBuf);

	iBioTestUtils->Printf(_L("PathSeparator: %c\n"),		aSmtpSettings->PathSeparator());	
	iBioTestUtils->Printf(_L("Synchronise: %d\n"),			aSmtpSettings->Synchronise());
	iBioTestUtils->Printf(_L("Subscribe: %d\n"),			aSmtpSettings->Subscribe());
	iBioTestUtils->Printf(_L("AutoSendOnConnect: %d\n"),	aSmtpSettings->AutoSendOnConnect());	
	iBioTestUtils->Printf(_L("DisconnectedUserMode: %d\n"), aSmtpSettings->DisconnectedUserMode());
	}

void CTestIacp::PrintSmsSettings(	CSmsSettings* aSmsSettings)
	{
	iBioTestUtils->ClearScreen();

	iBioTestUtils->Printf(_L("====== Sms Service entry=====\n"));
	//LoginName and Password are stored as 8 bit Descriptors so we need to convert them 
	// into 16 bit Unicode to be able to display them on the console
	iBioTestUtils->Printf(_L("Number of service centre addresses: %d\n"), aSmsSettings->ServiceCenterCount());
	for(TInt i = 0; i < aSmsSettings->ServiceCenterCount();i++)
		{
		CSmsServiceCenter& number = aSmsSettings->GetServiceCenter(i);
		iBioTestUtils->Printf(_L("	Address %d\n"),i);
		TPtrC ptr;
		ptr.Set(number.Name());
		iBioTestUtils->Printf(_L("		Name %S\n"), &ptr);
		ptr.Set(number.Address());
		iBioTestUtils->Printf(_L("		Address %S\n"), &ptr);
		}
	}

LOCAL_C void doMainL()
	{
	//create a scheduler
	exampleScheduler = new (ELeave) CExampleScheduler;
	CleanupStack::PushL( exampleScheduler );
	CActiveScheduler::Install( exampleScheduler );
	
	CBioTestUtils* BioTestUtils= CBioTestUtils::NewL(test, ETuGoClientSide | ETuDeleteService | ETuCreateService);
	BioTestUtils->WriteComment(_L("Msv Client Side Created"));
	CleanupStack::PushL(BioTestUtils);

	CTestIacp* testParser = CTestIacp::NewL(BioTestUtils);
	CleanupStack::PushL( testParser );
	BioTestUtils->WriteComment(_L("CTestIacp Object Created."));

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
	if (error) test.Printf(_L("Completed with return code %d"),error);
	delete cleanup;
	__UHEAP_MARKEND;
	test.Close();
	test.End();
	return KErrNone;
	}
