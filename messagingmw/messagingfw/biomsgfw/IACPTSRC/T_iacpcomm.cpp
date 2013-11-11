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
// This test harness processes an Internet Access Configuration Message
// and uses the resulting database entry to perform a connection to 
// the POP3 and SMTP servers.
// The parser creates the service entries from the files iacp*.txt
// placed into the data directory c:\biomsg\iacpcomm. If more
// than one entry is placed into the directory the harness uses the
// first instance as the target POP3/SMTP provider.
// The harness contructs and sends any files of the form rfc*.txt
// with the data directory to the SMTP service, before connecting to 
// the POP3 service to download mail.
// The test harness does not support heap fail testing
// Based on t_iacp.cpp amd t_pops.cpp
// 
//

#include "biotestutils.h"
#include "emailtestutils.h"
//
#include "IACPERR.H"
#include "IACPDEF.H"
#include <commdb.h>
#include <popsmtm.h>
#include <etel.h>
#include "tmsvbioinfo.h"
//
//SMS configuration files path
_LIT(IACP_TEST_MSG_PATH,	"c:\\test\\bio\\iacpcomm\\");

#define BIO_MSG_ENTRY_PARSED	 1 // Set entry .iMtmData3 to 1 to indicate that the entry has store i.e parsed,externalised


// forward references
class CExampleScheduler;
class CTestIacp;

//global declarations
LOCAL_D CExampleScheduler *exampleScheduler;
LOCAL_D RTest test(_L("IACPCOMM Test Harness"));

// Uncommment the following to provide prompting on the Windows platform
// #if defined(_DEBUG)
// #define PROMPTUSER test.Printf(_L("\nPress any key: ")); test.Getch();
// #else
// #define PROMPTUSER
// #endif
#define PROMPTUSER

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
	PROMPTUSER
	}

//--------------------------------------
//
//-- CTestIacp --
//

enum TSessionState
	{
	EParse,			// parsing is done at ParseL() stage. and data are externalised.
	EProcess,		// parsed data are internalised, then commited
	EInitSMTPMTM,	// initialise the send test utils for SMTP
	ESendSMTP,		// prep outgoing message and connect to the SMTP service
					// and send them
	EInitPOP3MTM,	// initialise the send test utils for SMTP
	EConnectPOP3,	// connect to the access point setup by the IACP entry
	EReceivePOP3,	// pull back any messages on the service
	EDisconnectPOP3,// and disconnect 
	EFinished		// Time to pack up
	};

class CTestIacp : public CActive 
	{
	public:
		~CTestIacp();
		static  CTestIacp * NewL(CBioTestUtils* aBioTestUtils);
		void StartL(TSessionState aCmd);
		CTestIacp(CBioTestUtils* aBioTestUtils);
		void DoCancel();
		void RunL();
		TBool DisplaySummary();
		TInt  Error();

	private:
		void RequestComplete(TRequestStatus& aStatus,TInt aCompletion);
		void ConstructL();
		void CreateParserL(); //sets 'iParser' to point to the created iacp parser
		void GetMessageBodyL();
		void DisplayEntryDataL();
		void ParseMsgL();
		void ProcessMsgL();
		void InitSMTPMTML();
		void InitPOP3MTML();
		void SendSMTPL();
		void ConnectPOP3L();
		void DisconnectPOP3L();
		void DoRunL();
		void SetIAPL(TMsvId aService);
		void deleteParser();
		void CheckSendStatus();
		TBool CheckRecvStatus();
		void LogCommsError(TInt aError);
		void LogParseError(TInt& aReason);
		void GetDefaultConnectionL();
		void ReceivePOP3L();

	private:
		TSessionState		iState;
		TInt				iError;

		// for managing the connections to the pop3 and smtp servers
		CEmailTestUtils*	iSendUtils;
		TMsvId				iPop3service;	// IACP added entry
		TMsvId				iSmtpservice;	// as above
		CMsvEntrySelection* iMsvSelection;
		TUint32				iIap;		// what commdb record we use for connecting out

		// for managing the input IACP data
		CBioTestUtils*		iBioTestUtils;
		CBaseScriptParser2*	iParser;

		HBufC*				iSmsMsg;		//sms body build dependent		
		TMsvId				iEntryId;       //ID of sms entry
		CMsvEntrySelection*	iIacpSelection;	//the collection of iacp*.txt entries

		// status
		TBool				iParsedMsg;
		TBool				iConnectedToSMTP;
		TInt				iMsgsSent;
		TBool				iConnectedToPOP3;
		TInt				iMsgsReceived;
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

	// for selecting message store items
	iMsvSelection = new (ELeave) CMsvEntrySelection;

	iIacpSelection=	iBioTestUtils->GenerateMessagesL(IACP_TEST_MSG_PATH);

	CActiveScheduler::Add(this);
	}

CTestIacp::~CTestIacp()
	{
	Cancel();

	deleteParser();
	delete iIacpSelection;
	delete iSmsMsg;
	delete iSendUtils;
	delete iMsvSelection;
	}

void CTestIacp::CreateParserL()
	{
	TRAPD(error,(iParser = iBioTestUtils->CreateParserTypeL(KUidBIOInternetAccessPointMsg)));
	if (error == KErrNone)
		{
		iBioTestUtils->WriteComment(_L("Created IACP Parser object"));
		}
	else
		{
		LogParseError(error);
		User::Leave(error);
		}	
	}

void CTestIacp::GetMessageBodyL()
	{
	// get a memory version of the message created by the test utils
	delete iSmsMsg;
	iSmsMsg = NULL;
	iSmsMsg = iBioTestUtils->MessageBodyL(iEntryId).AllocL();
	}

TInt CTestIacp::Error()
	{
	return iError;
	}

void CTestIacp::StartL(TSessionState aCmd)
	{
	iState=aCmd;
	iEntryId = (*iIacpSelection)[0];

	switch (iState)
		{
		case EParse:
			test.Printf(_L("Parsing...\n"));
			iBioTestUtils->SetEntryL(iEntryId); //clients have to set the right context before calling ParseL() 
			GetMessageBodyL();
			ParseMsgL();
			break;

		case EProcess:
			test.Printf(_L("Processing...\n"));
			iBioTestUtils->SetEntryL(iEntryId); //clients have to set the right context before calling CommitL() 
			ProcessMsgL();
			break;

		case EInitSMTPMTM:
			InitSMTPMTML();
			RequestComplete(iStatus,KErrNone);
            SetActive();
			break;

		case EInitPOP3MTM:
			InitPOP3MTML();
			RequestComplete(iStatus,KErrNone);
            SetActive();
			break;

		case ESendSMTP:
			SendSMTPL();
			break;

		case EConnectPOP3:
			ConnectPOP3L();
			break;

		case EReceivePOP3:
			ReceivePOP3L();
			break;

		case EDisconnectPOP3:
			DisconnectPOP3L();
			break;

		default:
			break;
		}
	}

void CTestIacp::RunL()
	{
	TRAP(iError, DoRunL());
	if (iError != KErrNone || iState == EFinished)
		{
		CActiveScheduler::Stop();
		}
	}

void CTestIacp::DoRunL()
	{
	TInt result=iStatus.Int();

	switch (iState)
		{
		case EParse:
			if(result==KErrNone)
				{
				iBioTestUtils->WriteComment(_L("Parsed Ok"));
				StartL(EProcess);
				}
			else
				{
				LogParseError(result);
				iParser->Cancel();
				iState=EFinished;
				}
			break;

		case EProcess:
			if(result==KErrNone)
				{
				iBioTestUtils->WriteComment(_L("Processed IACP Ok"));
				iParsedMsg = ETrue;
				//StartL(EInitSMTPMTM);
				StartL(EInitPOP3MTM);
				}
			else
				{
				LogParseError(result);
				iParser->Cancel();
				iState=EFinished;
				}
			break;

		case EInitSMTPMTM:
			iSendUtils->WriteComment(_L("Initialised SMTP MTM Ok"));
			StartL(ESendSMTP);	
			break;

		case ESendSMTP:
			CheckSendStatus();
			if (iConnectedToSMTP)
				{
				StartL(EInitPOP3MTM);
				}
			else
				{
				iState=EFinished;
				}
			break;

		case EInitPOP3MTM:
			iSendUtils->WriteComment(_L("Initialised POP3 MTM Ok"));
			StartL(EConnectPOP3);
			break;

		case EConnectPOP3:
			if (CheckRecvStatus())
				{
				iConnectedToPOP3 = ETrue;
				iSendUtils->WriteComment(_L("Connected POP3 server Ok"));
				// count number or entries in remote service
				User::LeaveIfError(iSendUtils->iServerEntry->SetEntry(iPop3service));
				User::LeaveIfError(iSendUtils->iServerEntry->GetChildren(*iMsvSelection));

				if (iMsvSelection->Count() > 0)
					{
					StartL(EReceivePOP3);
					}
				else
					{
					StartL(EDisconnectPOP3);
					}
				}
			else
				{
				iState=EFinished;
				}
			break;

		case EReceivePOP3:
			if (CheckRecvStatus())
				{
				iSendUtils->WriteComment(_L("Retrieved from POP3 Ok"));
				iMsgsReceived = iMsvSelection->Count();
				}
			StartL(EDisconnectPOP3);
			break;

		case EDisconnectPOP3:
			iSendUtils->WriteComment(_L("Disconnected POP3 server Ok"));
			iState = EFinished;
			break;

		default:
			break;
		}
	}

void CTestIacp::ParseMsgL()
	{
	iBioTestUtils->WriteComment(_L("Creating IACP Parser object"));
	TRAPD(error, iParser->ParseL(iStatus,iSmsMsg->Des())); 
	if (error == KErrNone)
		{
		iBioTestUtils->WriteComment(_L("Created IACP Parser object"));
		SetActive();
		}
	else
		{
		LogParseError(error);
		User::Leave(error);
		}	
	}

void CTestIacp::ProcessMsgL()
	{
	iBioTestUtils->WriteComment(_L("Starting processing..."));

	DisplayEntryDataL();

	TRAPD(error, iParser->ProcessL(iStatus)); //internalise and process
	if (error == KErrNone)
		{
		iBioTestUtils->WriteComment(_L("Process started ok"));
		SetActive();
		}
	else
		{
		LogParseError(error);
		User::Leave(error);
		}	
	}

void CTestIacp::deleteParser()
	{
	delete iParser;
	iParser = 0;
	delete iBioTestUtils;
	iBioTestUtils = 0;
	}

// setup the objects for connecting to the POP3 server
void CTestIacp::InitPOP3MTML()
	{
	// close down iBioTestUtils instance since it shares a single message server
	// which causes iSendUtils creation to fail

	// first delete the parser to allow iBioTestUtils to be deleted
	// without panicking
	deleteParser();

	delete iSendUtils;
	iSendUtils = 0;

	// now setup the send utils
	iSendUtils = CEmailTestUtils::NewL(test);
	iSendUtils->WriteComment(_L("Initialised POP3 Send Utils"));

	iSendUtils->GoServerSideL();
	iSendUtils->CreatePopServerMtmRegL();

	// Load the DLLs
	iSendUtils->CreateRegistryObjectAndControlL();
	iSendUtils->InstallPopMtmGroupL();

	iSendUtils->FindExistingServicesL();
	iSendUtils->InstantiatePopServerMtmL();

	// processing of message should have added these entries
	TRAPD(error, (iPop3service = iSendUtils->FindPopServiceL()));
	if (error != KErrNone)
		{
		iSendUtils->WriteComment(_L("Could not locate POP3 service entry"));
		User::Leave(error);
		}

	GetDefaultConnectionL();

	SetIAPL(iPop3service);
	}

// setup the objects for connecting to the SMTP server
void CTestIacp::InitSMTPMTML()
	{
	deleteParser();

	// now setup the send utils
	iSendUtils = CEmailTestUtils::NewL(test);
	iSendUtils->WriteComment(_L("Initialised SMTP Send Utils"));

	iSendUtils->GoServerSideL();
	iSendUtils->CreateSmtpServerMtmRegL();

	// Load the DLLs
	iSendUtils->CreateRegistryObjectAndControlL();
	iSendUtils->InstallSmtpMtmGroupL();

	iSendUtils->FindExistingServicesL();
	iSendUtils->InstantiateSmtpServerMtmL();

	// processing of message should have added these entries
	TRAPD(error, (iSmtpservice = iSendUtils->FindSmtpServiceL()));
	if (error != KErrNone)
		{
		iSendUtils->WriteComment(_L("Could not locate SMTP service entry"));
		User::Leave(error);
		}

	GetDefaultConnectionL();

	SetIAPL(iSmtpservice);
	}

void CTestIacp::ConnectPOP3L()
	{
	iMsvSelection->ResizeL(0);
	iMsvSelection->AppendL(iPop3service);

	iSendUtils->WriteComment(_L("Connecting to POP3 server..."));

	test.Printf(_L("Connecting to POP3 server...\n"));

	TBuf8<1> aParameter;

	iSendUtils->iPopServerMtm->StartCommandL(*iMsvSelection, KPOP3MTMConnect, aParameter, iStatus);

	SetActive();
	}

void CTestIacp::ReceivePOP3L()
	{
	TBuf<60> outputBuf;

	outputBuf.Format(_L("%d messages in the inbox"), iMsvSelection->Count());
	iSendUtils->WriteComment(outputBuf);

	outputBuf.Format(_L("%d messages in the inbox\n"), iMsvSelection->Count());
	test.Printf(outputBuf);
	
	iSendUtils->WriteComment(_L("Retrieving messages..."));

	test.Printf(_L("Retrieving messages...\n"));

	iSendUtils->iPopServerMtm->CopyToLocalL(*iMsvSelection, KMsvGlobalInBoxIndexEntryId, iStatus);

	SetActive();
	}

void CTestIacp::DisconnectPOP3L()
	{
	iSendUtils->WriteComment(_L("Disconnecting from POP3 server"));

	test.Printf(_L("Disconnecting from POP3 server...\n"));

	TBuf8<1> aParameter;

	iSendUtils->iPopServerMtm->StartCommandL(*iMsvSelection, KPOP3MTMDisconnect, aParameter, iStatus);

	SetActive();
	}

void CTestIacp::SendSMTPL()
	{
	test.Printf(_L("Creating SMTP messages...\n"));

	iSendUtils->CreateMessageFilesL(iSmtpservice,KMsvGlobalOutBoxIndexEntryId,IACP_TEST_MSG_PATH);

	// see how many we have created
	iSendUtils->iServerEntry->SetEntry(KMsvGlobalOutBoxIndexEntryId);
	User::LeaveIfError(iSendUtils->iServerEntry->GetChildren(*iMsvSelection));

	TInt count=iMsvSelection->Count();
	if (count > 0)
		{
		TBuf<60> outputBuf;

		outputBuf.Format(_L("%d messages in the outbox"), count);
		iSendUtils->WriteComment(outputBuf);

		outputBuf.Format(_L("\n%d messages in the outbox\n"), count);
		test.Printf(outputBuf);

		test.Printf(_L("Sending SMTP messages...\n"));

		// begin the send
		iSendUtils->iSmtpServerMtm->CopyFromLocalL(*iMsvSelection,iSmtpservice, iStatus);
		SetActive();
		}
	else
		{
		iSendUtils->WriteComment(_L("No messages to send in the outbox"));
		iState=EFinished;
		}
	}

//

void CTestIacp::GetDefaultConnectionL()
	{
	// determine what default record has been set as the preferred
	// connection by the IACP parser when processing the message
	CCommsDatabase* db = CCommsDatabase::NewL(EDatabaseTypeUnspecified);
	CleanupStack::PushL(db);
	TBuf<60> outputBuf;

	//db->GetGlobalSettingL(TPtrC(IAP),iIap);
	
	//GetGlobalSettingL() above is now replaced by the connection preferences table below
	CCommsDbConnectionPrefTableView* view;
	view = db->OpenConnectionPrefTableViewOnRankLC(ECommDbConnectionDirectionOutgoing, 1/*rank 1 is like a default value*/ );

	view->GotoFirstRecord(); //table with rank 1 only has one record
	CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref connPref;
	view->ReadConnectionPreferenceL(connPref);
	iIap = connPref.iBearer.iIapId;
	//END of retrieve setting from connection Pref Table

	CleanupStack::PopAndDestroy(2, db);

	outputBuf.Format(_L("Using IAP record %d for connection"), iIap);
	iSendUtils->WriteComment(outputBuf);
	}

void CTestIacp::SetIAPL(TMsvId aService)
	{
	// set the service with the preferred IAP comms record
	iSendUtils->iServerEntry->SetEntry(aService);
	TMsvEntry entry = iSendUtils->iServerEntry->Entry();
	entry.SetMtmData1(iIap);
	User::LeaveIfError(iSendUtils->iServerEntry->ChangeEntry(entry));
	}

void CTestIacp::DoCancel()
	{
	test.Printf(_L("DoCancel()"));
	}

void CTestIacp::RequestComplete(TRequestStatus& aStatus,TInt aCompletion)
	{
	TRequestStatus* statusPtr=&aStatus;
	User::RequestComplete(statusPtr,aCompletion);
	}


void CTestIacp::DisplayEntryDataL()
    {
    iBioTestUtils->SetEntryL(iEntryId);

	/*
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
    iBioTestUtils->Printf(_L("iMtmData1:      %d\n"), entry.iMtmData1);
    iBioTestUtils->Printf(_L("iMtmData2:      %d\n"), entry.iMtmData2);
    iBioTestUtils->Printf(_L("iMtmData3:      %d\n"), entry.iMtmData3);
    iBioTestUtils->Printf(_L("iDescription:  %S\n"), &entry.iDescription);
    iBioTestUtils->Printf(_L("iDetails:      %S\n "), &entry.iDetails);

	PROMPTUSER
	*/

    iBioTestUtils->ClearScreen();
    //test.Console()->SetPos(0,0);

	if(iBioTestUtils->Entry().MtmData3() != BIO_MSG_ENTRY_PARSED)
		{
		iBioTestUtils->WriteComment(_L("BioMg has not been parsed"));
		User::Leave(KErrGeneral);
		}
	iBioTestUtils->LogExtractedFieldsL(iEntryId);
    }

TBool CTestIacp::DisplaySummary()
	{
	TBuf<128> result;

	test.Printf(_L("\nFinal summary\n"));
	if (iBioTestUtils != 0)
		iBioTestUtils->WriteComment(_L("Final summary"));
	else
		iSendUtils->WriteComment(_L("Final summary"));
	result = iParsedMsg ? _L("TRUE") : _L("FALSE");

	TBuf<120> logMsg;

	logMsg.Format(_L("Parsed IACP message      : %S\n"), &result);
	test.Printf(logMsg);

	if (iBioTestUtils != 0)
		iBioTestUtils->WriteComment(logMsg);
	else
		iSendUtils->WriteComment(logMsg);

	result = iConnectedToSMTP ? _L("TRUE") : _L("FALSE");
	logMsg.Format(_L("Connected to SMTP server : %S\n"), &result);
	test.Printf(logMsg);
	if (iBioTestUtils != 0)
		iBioTestUtils->WriteComment(logMsg);
	else
		iSendUtils->WriteComment(logMsg);

	if (iConnectedToSMTP)
		{
		logMsg.Format(_L("    Sent %d messages\n"), iMsgsSent);
		test.Printf(logMsg);
		if (iBioTestUtils != 0)
			iBioTestUtils->WriteComment(logMsg);
		else
			iSendUtils->WriteComment(logMsg);
		}

	result = iConnectedToPOP3 ? _L("TRUE") : _L("FALSE");
	logMsg.Format(_L("Connected to POP3 server : %S\n"), &result);
	test.Printf(logMsg);
	if (iBioTestUtils != 0)
		iBioTestUtils->WriteComment(logMsg);
	else
		iSendUtils->WriteComment(logMsg);

	if (iConnectedToPOP3)
		{
		logMsg.Format(_L("    Received %d messages\n"), iMsgsReceived);
		test.Printf(logMsg);
		if (iBioTestUtils != 0)
			iBioTestUtils->WriteComment(logMsg);
		else
			iSendUtils->WriteComment(logMsg);
		}

	return iParsedMsg && iConnectedToSMTP && iConnectedToPOP3;
	}

void CTestIacp::CheckSendStatus()
	{
 	TImSmtpProgress temp;	
	TPckgC<TImSmtpProgress> paramPack(temp);

	const TDesC8& progBuf = iSendUtils->iSmtpServerMtm->Progress();
	paramPack.Set(progBuf);
	TImSmtpProgress progress=paramPack();	

	TBuf<128> logString;
	//  State of Smtp session?
	TInt state = progress.Status();
	switch(state)
		{
		case EMsgOutboxProgressWaiting:
			iSendUtils->WriteComment(_L("Waiting to start"));
			break;

		case EMsgOutboxProgressDone:
			if (progress.Error() == 0)
				iSendUtils->WriteComment(_L("Connection closed OK"));
			else
				LogCommsError(progress.Error());

			logString.Format(_L("%d messages: Sent %d, failed to send %d, didn't attempt to send %d.\n"), 
				progress.SendTotal(),
				progress.Sent(),
				progress.FailedToSend(),
				progress.NotSent());
			iSendUtils->WriteComment(logString);
			if (progress.Sent() > 0)
				{
				iConnectedToSMTP=ETrue;
				iMsgsSent=progress.Sent();
				}
			break;

		case EMsgOutboxProgressConnecting:
			logString.Format(_L("Connecting to SMTP server. Sending %d messages"),
				progress.SendTotal());
			iSendUtils->WriteComment(logString);
			break;

		case EMsgOutboxProgressSending:
			logString.Format(_L("Sending message No.%d/%d. Transmitted %d bytes of %d"),
				progress.MsgNo()+1,
				progress.SendTotal(),
				progress.iSendFileProgress.iBytesSent,
				progress.iSendFileProgress.iBytesToSend);
			iSendUtils->WriteComment(logString);
			break;
		}
	}

TBool CTestIacp::CheckRecvStatus()
	{
	TBool status = EFalse;

	// Get the progress
	TPop3Progress prog;	
	TPckgC<TPop3Progress> paramPack(prog);
	paramPack.Set(iSendUtils->iPopServerMtm->Progress());
	TPop3Progress progress=paramPack();

	// Check that the reported progress is EPopConnecting
	if (progress.iPop3Progress == TPop3Progress::EPopConnecting
	||	progress.iPop3Progress == TPop3Progress::EPopRefreshing)
		{
		if (progress.iErrorCode == 0)
			{
			status = ETrue;
			}
		else
			{
			LogCommsError(progress.iErrorCode);
			}
		}

	return status;
	}

void CTestIacp::LogCommsError(TInt aError)
	{
	TBuf<100> errorMsg;

	switch (aError)
	{
	case KErrEtelNotCallOwner:
		errorMsg=_L("Comms error: Not call owner");
		break;
	case KErrEtelDuplicatePhoneName:
		errorMsg=_L("Duplicate phone name");
		break;

	case KErrEtelAlreadyCallOwner:
		errorMsg=_L("Already call owner");
		break;

	case KErrEtelNoCarrier:
		errorMsg=_L("No carrier");
		break;

	case KErrEtelBusyDetected:
		errorMsg=_L("Busy detected");
		break;

	case KErrEtelNoClientInterestedInThisCall:
		errorMsg=_L("No client interested in call");
		break;

	case KErrEtelInitialisationFailure:
		errorMsg=_L("Initialisation failure");
		break;

	case KErrEtelCallNotActive:
		errorMsg=_L("Call not active");
		break;

	case KErrEtelNoAnswer:
		errorMsg=_L("No answer");
		break;

	case KErrEtelNoDialTone:
		errorMsg=_L("No dial tone");
		break;

	case KErrEtelConfigPortFailure:
		errorMsg=_L("Config port failure");
		break;

	case KErrEtelFaxChunkNotCreated:
		errorMsg=_L("Fax chunk not created");
		break;

	case KErrEtelNotFaxOwner:
		errorMsg=_L("Not fax owner");
		break;

	case KErrEtelPortNotLoanedToClient:
		errorMsg=_L("Port not loaned to client");
		break;

	case KErrEtelWrongModemType:
		errorMsg=_L("Wrong modem type");
		break;

	case KErrEtelUnknownModemCapability:
		errorMsg=_L("Unknown modem capability");
		break;

	case KErrEtelAnswerAlreadyOutstanding:
		errorMsg=_L("Answer already outstanding");
		break;
	case KErrEtelModemNotDetected:
		errorMsg=_L("Modem not detected");
		break;

	case KErrEtelModemSettingsCorrupt:
		errorMsg=_L("Modem settings corrupt");
		break;

	case KPop3InvalidUser:
		errorMsg=_L("Invalid POP3 user");
		break;

	case KPop3InvalidLogin:
		errorMsg=_L("Invalid POP3 login");
		break;

	case KPop3CannotConnect:
		errorMsg=_L("Cannot connect to POP3 server");
		break;

	case KPop3CannotCreateApopLogonString:
		errorMsg=_L("Cannot create Apop logon string");
		break;

	case KPop3ProblemWithRemotePopServer:
		errorMsg=_L("Problem with remote POP server");
		break;

	case KPop3CannotOpenServiceEntry:
		errorMsg=_L("Cannot open POP3 service entry");
		break;

	case KPop3CannotSetRequiredFolderContext:
		errorMsg=_L("Cannot set required folder context");
		break;

	case KPop3InvalidApopLogin:
		errorMsg=_L("Invalid Apop login");
		break;

	case KPopTopError:
		errorMsg=_L("POP TOP Error");
		break;

	default:
		errorMsg.Format(_L("Unknown error %d"), aError);
		break;
		}

	TBuf<120> logMsg;

	logMsg.Format(_L("Connection closed with comms error (%S)"), &errorMsg);
	iSendUtils->WriteComment(logMsg);

	logMsg.Format(_L("Connection closed with comms error (%S)\n"), &errorMsg);
	test.Printf(logMsg);			  
	}

void CTestIacp::LogParseError(TInt& aReason)
	{
	TBuf<256> tempBuf;
	switch (aReason)
		{
		case KErrCancel:
			tempBuf= _L("Session cancelled");
			break;
		case KErrEof:
			tempBuf= _L("KErrEof");
			break;
		case KErrNoMemory:
			tempBuf= _L("KErrNoMemory");
			break;
		case KErrDisconnected:
			tempBuf= _L("KErrDisconnected");
			break;
		case KErrAccessDenied:
			tempBuf= _L("KErrAccessDenied");
			break;
		case KBspInvalidMessage:
			tempBuf= _L("Error: Invalid SMS Message. For Internet Access Configuration,SMS Header Field should be //SIAP11 ");
			break;
		case KIacpUnknownSmsType:
			tempBuf=_L("Error:SMS 1st Left Token does not start with niether M nor I");
			break;
		case KIacpBIOMsgTypeNotSupported:
			tempBuf= _L("this Bio msg is not suppported");
			break;
		case KIacpMandatoryDataNotSet:
			tempBuf= _L("Error: Mandatory data missing in SMS message");
			break;
		case KIacpUnknownMailProtocol:
			tempBuf= _L("Error: Unknown Mail Protocol Not a Pop3/Imap4");
			break;
		case KIacpErrRightToken:
			tempBuf= _L("Error: improper right token  i.e not equal t/f (True/False)");
			break;
		case KIacpErrLeftToken:
			tempBuf=_L("Error:SMS Left Tokens should start with 'M' for Mailbox account configuration\r\n and with 'I' for Internet service configuration");
			break;
		case KIacpErrSmsDataNotParsed:
			tempBuf= _L("sms data should be parsed before processing");
			break;
		case KIacpErrSmsDataNotRestored:
			tempBuf= _L("Error: sms data should be parsed before commiting (KIacpErrSmsDataNotRestored)");
			break;
		case KIacpScriptErrISPNotFound:
			tempBuf= _L("Internet Service not found in Dial Out DB");
			break;
		case KIacpErrScriptNotDefined: 
			tempBuf= _L("Script not included in sms");
			break;
		case KIacpErrScriptNotFoundInDB:
			tempBuf= _L("There is no script in DB to append to");
			break;
		case KIacpErrScriptAlreadyAdd:
			tempBuf= _L("Script cannot be add more than ounce for each Bio Msg");
			break;
		default:
			tempBuf.Format(_L("Unknown error %d"), aReason);
			break;
		}

	TBuf<300> logMsg;

	logMsg.Format(_L("Parse error (%S)"), &tempBuf);
	iBioTestUtils->WriteComment(logMsg);

	logMsg.Format(_L("Parse error (%S)\n"), &tempBuf);
	test.Printf(logMsg);			  
	}

LOCAL_C void doMainL()
	{
	// create a scheduler
	exampleScheduler = new (ELeave) CExampleScheduler;
	CleanupStack::PushL(exampleScheduler);
	CActiveScheduler::Install(exampleScheduler);

	// initialise the bio test utils
	CBioTestUtils* BioTestUtils = CBioTestUtils::NewL(test,ETuCleanMessageFolder|ETuGoClientSide);
	BioTestUtils->GoClientSideL();
	BioTestUtils->WriteComment(_L("Msv Client Side Created"));

	// create a IACP parser
	CTestIacp* testParser = CTestIacp::NewL(BioTestUtils);
	CleanupStack::PushL(testParser);
	test(testParser != NULL);

	BioTestUtils->WriteComment(_L("CTestIacp Object Created."));

	// begin the tests
	BioTestUtils->TestStart(1);
	testParser->StartL(EParse);
	
	CActiveScheduler::Start();

	if (testParser->DisplaySummary())
		{
		BioTestUtils->TestHarnessCompleted();
		}
	else
		{
		BioTestUtils->TestHarnessFailed(testParser->Error());
		}

	CleanupStack::PopAndDestroy(2);   // testParser, exampleScheduler

	PROMPTUSER
	}

GLDEF_C TInt E32Main()
	{
	test.Title();
	test.Start(_L("IACPCOMM Test Harness"));
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

