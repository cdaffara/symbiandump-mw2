// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This is the header file for CT_MsgUtilsSendEmail
// 
//

/**
 @file
*/


// User includes
#include "T_UtilsSendEmail.h"


// epoc includes
#include <iapprefs.h>


// Literals for the Smtp commands
_LIT8(KHeloString, "HELO\r\n");
_LIT8(KMailFrom, "MAIL FROM:");
_LIT8(KRcptTo, "RCPT TO:");
_LIT8(KDataString, "DATA\r\n");
_LIT8(KEndData, "\r\n.\r\n");
_LIT8(KQuitString, "QUIT\r\n");
_LIT8(KCrlfString, "\r\n");


/**
NewL()
Static factory constructor. Uses two phase 
construction.

@param aTestStep
@leave KErrNoMemory
@return
A pointer to the newly created CT_MsgUtilsSendEmail object.
*/
CT_MsgUtilsSendEmail* CT_MsgUtilsSendEmail::NewL(CTestStep& aTestStep)
   {
	CT_MsgUtilsSendEmail* self = new (ELeave) CT_MsgUtilsSendEmail(aTestStep);
	return self;
	}


/**
CT_MsgUtilsSendEmail()
Constructor

@param aTestStep
*/
CT_MsgUtilsSendEmail::CT_MsgUtilsSendEmail(CTestStep& aTestStep)
:	CActive(EPriorityStandard),iTestStep(aTestStep)
	{
	CActiveScheduler::Add(this);
	}


/**
~CT_MsgUtilsSendEmail()
Destructor
*/
CT_MsgUtilsSendEmail::~CT_MsgUtilsSendEmail()
	{
	iFile.Close();
	iFs.Close();
	delete iImSocket;
	iImSocket=NULL;
	delete iIapPrefs;
	iIapPrefs=NULL;
	}


/**
StartL()
Deletes the children entries of the specified parent

@param aSender
@param aReceipient
@param aMailFile
@param aServerIP
@param aStatus
*/
void CT_MsgUtilsSendEmail::StartL(TPtrC aSender,TPtrC aReceipient,TPtrC aMailFile,
											TPtrC aServerIP,TRequestStatus& aStatus)
	{
	iRequestStatus = &aStatus;
	aStatus = KRequestPending;
	
	iSender.Set(aSender);
	iReceipient.Set(aReceipient);
	
	// Connects the client to the file server.
	iFs.Connect();

	// Opens an existing file reading and a share mode 
	// If the file does not already exist, an error is returned.
	iFile.Open(iFs, (TDesC&)aMailFile, EFileShareAny|EFileRead);

	// Uses two phase construction and leaves nothing on the CleanupStack
	iImSocket = CImTextServerSession::NewL();

	iIapPrefs = CImIAPPreferences::NewLC();
	CleanupStack::Pop();

	// Queue a connect assuming the socket is successfully opened.
	iImSocket->QueueConnectL(iStatus, (TDesC&)aServerIP, 25, *iIapPrefs, ETrue);
	
	iSendState = ESmtpConnecting;
	SetActive();
	}

	
/**
DoCancel()
CActive derived function to handle Cancel requests
*/
void CT_MsgUtilsSendEmail::DoCancel()
	{
	iImSocket->Cancel();
	}


/**
RunL()
CActive derived function to handle the state machine

@panic Unkown receive state!
@panic Unkown send state!
*/
void CT_MsgUtilsSendEmail::RunL()
	{
	// handle send state
	switch (iSendState)
		{
		case ESmtpNotConnected:
			{
			// Send completed.
			User::RequestComplete(iRequestStatus,KErrNone);
			break;
			}
		case ESmtpConnecting:
			{	
			// Connection attempt completed.
			if (iStatus != KErrNone)
				{
				// error
				iTestStep.ERR_PRINTF2(_L("CT_MsgUtilsSendEmail: failed to connect: [%d]."), iStatus.Int());
				iTestStep.SetTestStepResult(EFail);
				User::Leave(iStatus.Int());
				}
			
				// ok successful connect, next send HELO
				iSendState = ESmtpSendHELO;
				iImSocket->Send(iStatus, KHeloString());
				SetActive();
				break;
			}
		case ESmtpSendHELO:
			{	
			// HELO was sent
			if (iStatus != KErrNone)
				{
				// error
				iTestStep.ERR_PRINTF2(_L("CT_MsgUtilsSendEmail: failed to send HELO: [%d]."), iStatus.Int());
				iTestStep.SetTestStepResult(EFail);
				User::Leave(iStatus.Int());
				}
			
				// ok we sent HELO, now receive reply
				iSendState = ESmtpReceiveReply;
				iRecvState = ESmtpHeloReply;
				iImSocket->QueueReceiveNextTextLine(iStatus);
				SetActive();
				break;
			}
		case ESmtpSendMailFrom:
			{	
			// MAIL FROM was sent.
			if (iStatus != KErrNone)
				{
				// error
				iTestStep.ERR_PRINTF2(_L("CT_MsgUtilsSendEmail: failed to send MAIL FROM: [%d]."), iStatus.Int());
				iTestStep.SetTestStepResult(EFail);
				User::Leave(iStatus.Int());
				}
			
				// ok we sent MAIL FROM, now receive reply
				iSendState = ESmtpReceiveReply;
				iRecvState = ESmtpFromReply;
				iImSocket->QueueReceiveNextTextLine(iStatus);
				SetActive();
				break;
			}
		case ESmtpSendRcptTo:
			{	
			// RCPT TO was sent.
			if (iStatus != KErrNone)
				{
				// error
				iTestStep.ERR_PRINTF2(_L("CT_MsgUtilsSendEmail: failed to send RCPT TO: [%d]."), iStatus.Int());
				iTestStep.SetTestStepResult(EFail);
				User::Leave(iStatus.Int());
				}
			
				// ok we sent RCPT TO, now receive reply
				iSendState = ESmtpReceiveReply;
				iRecvState = ESmtpRcptReply;
				iImSocket->QueueReceiveNextTextLine(iStatus);
				SetActive();
				break;
			}
		case ESmtpSendData:
			{	
			// DATA was sent.
			if (iStatus != KErrNone)
				{
				// error
				iTestStep.ERR_PRINTF2(_L("CT_MsgUtilsSendEmail: failed to send DATA: [%d]."), iStatus.Int());
				iTestStep.SetTestStepResult(EFail);
				User::Leave(iStatus.Int());
				}
			
				// ok we sent DATA, now receive reply
				iSendState = ESmtpReceiveReply;
				iRecvState = ESmtpDataReply;
				iImSocket->QueueReceiveNextTextLine(iStatus);
				SetActive();
				break;
			}
		case ESmtpSendContent:
			{	
			// Content was sent.
			// Read and send file. Send file in chunks of 100 bytes.
			TBuf8<100> buffer;
			User::LeaveIfError(iFile.Read(buffer, 100));
			if (buffer.Length() == 0)
				{
				// ok, end of file. send end of email.
				iSendState = ESmtpSendEndData;
				iImSocket->Send(iStatus, KEndData());
				}
			else
				{
				// we still have data in file. read and send.
				iSendState = ESmtpSendContent;
				iImSocket->Send(iStatus, buffer);
				}
				SetActive();
				break;
			}
		case ESmtpSendEndData:
			{	
			// end of data was sent.
			if (iStatus != KErrNone)
				{
				// error
				iTestStep.ERR_PRINTF2(_L("CT_MsgUtilsSendEmail: failed to send end-of-data marker: [%d]."), iStatus.Int());
				iTestStep.SetTestStepResult(EFail);
				User::Leave(iStatus.Int());
				}
			
			// ok we sent end-of-data, now receive reply
			iSendState = ESmtpReceiveReply;
			iRecvState = ESmtpContentReply;
			iImSocket->QueueReceiveNextTextLine(iStatus);
			SetActive();
			break;
			}
		case ESmtpSendLogout:
			{
			iSendState = ESmtpReceiveReply;
			iRecvState = ESmtpLogoutReply;
			iImSocket->QueueReceiveNextTextLine(iStatus);
			SetActive();
			break;
			}
		case ESmtpReceiveReply:
			{
			if (iStatus != KErrNone)
				{
				// error
				iTestStep.ERR_PRINTF3(_L("CT_MsgUtilsSendEmail: failed to receive reply: error [%d] recvstate [%d]."), iStatus.Int(), iRecvState);
				iTestStep.SetTestStepResult(EFail);
				User::Leave(iStatus.Int());
				}
			
			// ok, read and print out the reply
			TBuf8<50> buffer;
			TBuf<50> buffer16;
			TImLineType replyLine = iImSocket->GetCurrentTextLine(buffer);
			buffer16.Copy(buffer);
			iTestStep.INFO_PRINTF1(buffer16);
			
			if (replyLine == EBufferTooSmall)
				{
				while (replyLine != ECRLFTerminated)
					{
					replyLine = iImSocket->GetCurrentTextLine(buffer);
					buffer16.Copy(buffer);
					iTestStep.INFO_PRINTF1(buffer16);
					}
				}
		
			// handle reply state
			switch (iRecvState)
				{
				case ESmtpHeloReply:
					{
					// send MAIL FROM
					iSendState = ESmtpSendMailFrom;
					TBuf8<100> from;
					from.Append(KMailFrom);
					from.Append(iSender);
					from.Append(KCrlfString);
					iImSocket->Send(iStatus, from);
					break;
					}
				case ESmtpFromReply:
					{
					// send RCPT TO
					iSendState = ESmtpSendRcptTo;
					TBuf8<100> to;
					to.Append(KRcptTo);
					to.Append(iReceipient);
					to.Append(KCrlfString);
					iImSocket->Send(iStatus, to);
					break;
					}
				case ESmtpRcptReply:
					{
						// send DATA
					iSendState = ESmtpSendData;
					iImSocket->Send(iStatus, KDataString());
					break;
					}
				case ESmtpDataReply:
					{
					// send content
					iSendState = ESmtpSendContent;
					TBuf8<100> buffer;
					User::LeaveIfError(iFile.Read(buffer, 100));
					if (buffer.Length() == 0)
						{
						// ok, end of file. since this is first read it would mean the file
						// is empty! we should maybe leave here or??
						iSendState = ESmtpSendEndData;
						iImSocket->Send(iStatus, KEndData());
						}
					else
						{
						// we still have data in file
						iSendState = ESmtpSendContent;
						iImSocket->Send(iStatus, buffer);
						}
						break;
					}
				case ESmtpContentReply:
					{
					// send QUIT
					iSendState = ESmtpSendLogout;
					iImSocket->Send(iStatus, KQuitString());
					break;
					}
				case ESmtpLogoutReply:
					{
					//iTestStep.INFO_PRINTF1(_L("LOGOUT"));
					iSendState = ESmtpNotConnected;
					iImSocket->Disconnect(iStatus);
					break;
					}
				default:
					{
					User::Panic(_L("Unkown receive state!"), iRecvState);
					break;
					}
				}// End switch (iRecvState)
			
				SetActive();
				break;
			}
		default:
			{
			User::Panic(_L("Unkown send state!"), iSendState);
			break;
			}
		} // End switch (iSendState)
	}

