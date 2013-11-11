// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// __ACTION_INFO_BEGIN__ 
// [Action Name]
// DrmSendEmail
// [Action Parameters]
// sender			input			HBufC8, user name of email sender
// recipient		input			HBufC8, user name of email recipient
// mailFile			input			HBufC, path and name of the meta data file to check
// serverIP 		input			HBufC, IP address of SMTP server
// [Action Description]
// This test action will send an email from <sender> to <recipient> with the contents
// of <mailfile> as message body using the SMTP server specified by <serverIP>.
// [APIs Used]	
// RFile::Open
// RFile::Read
// CImTextServerSession::QueueConnectL
// CImTextServerSession::Send
// CImTextServerSession::QueueReceiveNextTextLine
// CImTextServerSession::GetCurrentTextLine
// CImTextServerSession::Disconnect
// CImIAPPreferences::NewLC
// __ACTION_INFO_END__
//

//! @file

#include "CMtfTestActionSmtpSendEmail.h"

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionUtilsConfigFileMachineName.h"

#include <iapprefs.h>


_LIT8(KHeloString, "HELO\r\n");
_LIT8(KMailFrom, "MAIL FROM:");
_LIT8(KRcptTo, "RCPT TO:");
_LIT8(KDataString, "DATA\r\n");
_LIT8(KEndData, "\r\n.\r\n");
_LIT8(KQuitString, "QUIT\r\n");
_LIT8(KCrlfString, "\r\n");

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSmtpSendEmail object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSmtpSendEmail::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
{
	CMtfTestActionSmtpSendEmail* self = new (ELeave) CMtfTestActionSmtpSendEmail(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
}

/**
  Function : CMtfTestActionSmtpSendEmail
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSmtpSendEmail::CMtfTestActionSmtpSendEmail(CMtfTestCase& aTestCase)
 : CMtfTestAction(aTestCase)
{
}
	
/**
  Function : ~CMtfTestActionSmtpSendEmail
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSmtpSendEmail::~CMtfTestActionSmtpSendEmail()
{
	iFile.Close();
	iFs.Close();
	delete iImSocket;
	delete iIapPrefs;
}

/**
  Function : ExecuteActionL
  Description : Entry point for the this test action in the test framework
  @internalTechnology
  @param : none
  @return : void
  @pre none 
  @post none
*/
void CMtfTestActionSmtpSendEmail::ExecuteActionL()
{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSmtpSendEmail);
	CActiveScheduler::Add(this);
	HBufC* serverIP = ObtainParameterReferenceL<HBufC>(TestCase(), ActionParameters().Parameter(3));

	HBufC* emailAddressExt = ObtainParameterReferenceL<HBufC>(TestCase(), ActionParameters().Parameter(4), (*serverIP).Alloc());
	
	CMtfTestActionUtilsConfigFileMachineName* machineNameFile = CMtfTestActionUtilsConfigFileMachineName::NewLC(*emailAddressExt);

	TPtrC machineNameEmail(machineNameFile->MachineNameEmail());
	iSender = ObtainParameterReferenceL<HBufC>(TestCase(), ActionParameters().Parameter(0), machineNameEmail.Alloc());
	iRecipient = ObtainParameterReferenceL<HBufC>(TestCase(), ActionParameters().Parameter(1), machineNameEmail.Alloc());
	
	HBufC* mailFile = ObtainParameterReferenceL<HBufC>(TestCase(), ActionParameters().Parameter(2));
	
	iFs.Connect();
	iFile.Open(iFs, *mailFile, EFileShareAny|EFileRead);
	
	iImSocket = CImTextServerSession::NewL();
	iIapPrefs = CImIAPPreferences::NewLC();
	CleanupStack::Pop();
	
	iImSocket->QueueConnectL(iStatus, serverIP->Des(), 25, *iIapPrefs, ETrue);
	
	CleanupStack::PopAndDestroy(machineNameFile);
	
	iSendState = ESmtpConnecting;
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSmtpSendEmail);
}

/**
  Function : DoCancel
  Description : CActive derived function to handle Cancel requests
  @internalTechnology
  @param : none
  @return : void
  @pre none 
  @post none
*/
void CMtfTestActionSmtpSendEmail::DoCancel()
{
	iImSocket->Cancel();
}

/**
  Function : RunL
  Description : CActive derived function to handle the state machine
  @internalTechnology
  @param : none
  @return : void
  @pre none 
  @post none
*/
void CMtfTestActionSmtpSendEmail::RunL()
{
	// handle send state
	switch (iSendState)
	{
		case ESmtpNotConnected:
		{	// ok, we're done. send completed.
			TestCase().ActionCompletedL(*this);
			break;
		}
		case ESmtpConnecting:
		{	// connection attempt completed.
			if (iStatus != KErrNone)
			{
				// error
				TestCase().ERR_PRINTF2(_L("CMtfTestActionSmtpSendEmail: failed to connect: [%d]."), iStatus.Int());
				TestCase().SetTestStepResult(EFail);
				User::Leave(iStatus.Int());
			}
			
			// ok successful connect, next send HELO
			iSendState = ESmtpSendHELO;
			iImSocket->Send(iStatus, KHeloString());
			SetActive();
			break;
		}
		case ESmtpSendHELO:
		{	// HELO was sent
			if (iStatus != KErrNone)
			{
				// error
				TestCase().ERR_PRINTF2(_L("CMtfTestActionSmtpSendEmail: failed to send HELO: [%d]."), iStatus.Int());
				TestCase().SetTestStepResult(EFail);
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
		{	// MAIL FROM was sent.
			if (iStatus != KErrNone)
			{
				// error
				TestCase().ERR_PRINTF2(_L("CMtfTestActionSmtpSendEmail: failed to send MAIL FROM: [%d]."), iStatus.Int());
				TestCase().SetTestStepResult(EFail);
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
		{	// RCPT TO was sent.
			if (iStatus != KErrNone)
			{
				// error
				TestCase().ERR_PRINTF2(_L("CMtfTestActionSmtpSendEmail: failed to send RCPT TO: [%d]."), iStatus.Int());
				TestCase().SetTestStepResult(EFail);
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
		{	// DATA was sent.
			if (iStatus != KErrNone)
			{
				// error
				TestCase().ERR_PRINTF2(_L("CMtfTestActionSmtpSendEmail: failed to send DATA: [%d]."), iStatus.Int());
				TestCase().SetTestStepResult(EFail);
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
		{	//content was sent.
			// read and send file. send file in chunks of 100 bytes.
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
		{	// end of data was sent.
			if (iStatus != KErrNone)
			{
				// error
				TestCase().ERR_PRINTF2(_L("CMtfTestActionSmtpSendEmail: failed to send end-of-data marker: [%d]."), iStatus.Int());
				TestCase().SetTestStepResult(EFail);
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
				TestCase().ERR_PRINTF3(_L("CMtfTestActionSmtpSendEmail: failed to receive reply: error [%d] recvstate [%d]."), iStatus.Int(), iRecvState);
				TestCase().SetTestStepResult(EFail);
				User::Leave(iStatus.Int());
			}
			
			// ok, read and print out the reply
			TBuf8<50> buffer;
			TBuf<50> buffer16;
			TImLineType replyLine = iImSocket->GetCurrentTextLine(buffer);
			buffer16.Copy(buffer);
			TestCase().INFO_PRINTF1(buffer16);
			
			if (replyLine == EBufferTooSmall)
			{
				while (replyLine != ECRLFTerminated)
				{
					replyLine = iImSocket->GetCurrentTextLine(buffer);
					buffer16.Copy(buffer);
					TestCase().INFO_PRINTF1(buffer16);
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
					from.Append(*iSender);
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
					to.Append(*iRecipient);
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
					iSendState = ESmtpNotConnected;
					iImSocket->Disconnect(iStatus);
					break;
				}
				default:
				{
					User::Panic(_L("Unkown receive state!"), iRecvState);
					break;
				}
			} // End switch (iRecvState)
			
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
