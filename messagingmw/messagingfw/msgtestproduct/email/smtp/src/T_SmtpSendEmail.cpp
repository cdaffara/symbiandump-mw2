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
// [TestStep Name]
// SmtpSendEmail
// [Parameters]
// sender			:	User name of email sender
// recipient		:	User name of email recipient
// mailFile		:	Path of the meta data file to check
// serverIP 		:	IP address of SMTP server
// This test step will send an email from <sender> to <recipient> with the contents
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
// 
//

/**
 @file
*/


//User includes
#include <t_utilsconfigfilemachinename.h>
#include "T_UtilsSendEmail.h"
#include "T_SmtpSendEmail.h"


// Literals Used
_LIT(KSender,"Sender");
_LIT(KRecipient,"Recipient");
_LIT(KMailFile,"MailFile");
_LIT(KServerIP,"ServerIP");


/**
CT_MsgSmtpSendEmail()
Constructor

@param aSharedDataSMTP
*/
CT_MsgSmtpSendEmail::CT_MsgSmtpSendEmail(CT_MsgSharedDataSmtp& aSharedDataSMTP)
:	CT_MsgAsyncStepSMTP(aSharedDataSMTP)
	{
	SetTestStepName(KSmtpSendEmail);
	}

/**
NewL()
Static factory constructor. Uses two phase 
construction.

@param aSharedDataSMTP
@leave KErrNoMemory
@return
A pointer to the newly created CT_MsgSendSmtpMessage object.
*/
CT_MsgSmtpSendEmail* CT_MsgSmtpSendEmail::NewL(CT_MsgSharedDataSmtp& aSharedDataSMTP)
	{
	CT_MsgSmtpSendEmail* self = new(ELeave) CT_MsgSmtpSendEmail(aSharedDataSMTP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 // Call CT_AsyncStep::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}


/**
~CT_MsgSmtpSendEmail()
Destructor
*/
CT_MsgSmtpSendEmail::~CT_MsgSmtpSendEmail()
	{
	if(iSendEmail!=NULL)
		{
		delete iSendEmail;
		iSendEmail = NULL;	
		}

	if(iMachineNameFile!=NULL)
		{
		delete iMachineNameFile;
		iMachineNameFile = NULL;
		}
	}


/**
ProgressL()

*/
void CT_MsgSmtpSendEmail::ProgressL(TBool bFinal)
	{
	//	TODO
	if ( bFinal )
		{
		//	Display final progress iSendEmail
		}
	else
		{
		//	Display current progress iSendEmail
		}
	}

/**
CancelStep()

*/
void CT_MsgSmtpSendEmail::CancelStep()
	{
	//	TODO cancel iSendEmail
	}

	
/**
doTestStepL()
Reads the parent folder name, the message subject from the ini file
Searches for the message under the specified folder, and sends the message

@return
Returns the test step result
*/
TVerdict CT_MsgSmtpSendEmail::doTestStepL()
	{
	INFO_PRINTF1( _L("Test Step :  Smtp Send Email") );
	TPtrC8	string8Ptr; 
	TPtrC	stringPtr = KNone();
	// Gets the machine name of the system
	if(iMachineNameFile==NULL)
	{
	iMachineNameFile = CT_MsgUtilsConfigFileMachineName::NewL(stringPtr);
	}

	TPtrC8	machineName(iMachineNameFile->MachineName());

	// Read the username of the email sender from the ini file
	TPtrC	sender;
	if(!GetStringFromConfig( ConfigSection(), KSender, sender))
		{
		ERR_PRINTF1(_L("Sender is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		// Read the username of the email recipient from the ini file
		TPtrC	recipient;
		if( !GetStringFromConfig(ConfigSection(),KRecipient,recipient ))
			{
			ERR_PRINTF1(_L("Recipient is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			// Read the path of the mailfile from the ini file
			TPtrC	mailFile;
			if( !GetStringFromConfig(ConfigSection(),KMailFile,mailFile ))
				{
				ERR_PRINTF1(_L("MailFile is not specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				// Read the server IP address from the ini file
				TPtrC	serverIP;
				if( !GetStringFromConfig(ConfigSection(),KServerIP,serverIP ))
					{
					ERR_PRINTF1(_L("ServerIP is not specified"));
					SetTestStepResult(EFail);
					}
				else
					{
					if(iSendEmail==NULL)
						{
						iSendEmail = CT_MsgUtilsSendEmail::NewL(*this);
						}

					CT_MsgActive&	active=Active();
					iSendEmail->StartL(sender,recipient,mailFile,serverIP, active.iStatus);
					active.Activate();
					CActiveScheduler::Start();
					User::LeaveIfError(active.Result());
					}
				}
			}
		}

	return TestStepResult();
	}
