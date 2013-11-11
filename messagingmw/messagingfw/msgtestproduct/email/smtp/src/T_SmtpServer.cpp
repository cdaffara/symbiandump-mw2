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
//

// User includes
#include "T_MsgServer.h"
#include "T_SmtpServer.h"
#include "T_MsgSharedDataSmtp.h"

// Header files of Test Steps SMTP
#include "T_CreateSmtpMessageFromEmailFile.h"
#include "T_SendSmtpMessage.h"
#include "T_SmtpSendEmail.h"
#include "T_StartRamUsageTimer.h"
#include "T_StopRamUsageTimer.h"
#include "t_smtpcreatenewmessagewithplainbodytext.h"
#include "T_SmtpCreateForwardMessage.h"
#include "T_SmtpCreateReplyMessage.h"
#include "T_RetrievePlainBodyTextAndCompare.h"
#include "T_CreateSmtpMobilityAccount.h"
#include "T_CreateSmtpAccount.h"
#include "T_CheckSmtpSNAPSetting.h"
#include "T_ModifySmtpSettings.h"
#include "T_RemoveSmtpSNAPSetting.h"
#include "T_CheckSmtpBMSetting.h"

/**
MainL()
This is the main function which installs the 
active scheduler and creates an object of the Email server
*/
LOCAL_C void MainL()
/**
 * Secure variant
 * Much simpler, uses the new Rendezvous() call to sync with the client
 */
	{
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);

	CActiveScheduler*	sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CT_MsgSmtpServer* server = NULL;

	// Create the CTestServer derived Email server
	TRAPD(err, server = CT_MsgSmtpServer::NewL());
	if ( err == KErrNone )
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	server=NULL;
	delete sched;
	sched=NULL;
	}


/**
E32Main()
This function is the entry point for T_MsgSmtpServer test server

@return - Standard Epoc error code on process exit
Secure variant only
Process entry point. Called by client using RProcess API
*/
GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAP_IGNORE(MainL());	
	delete cleanup;
	cleanup=NULL;
	__UHEAP_MARKEND;
	
	return KErrNone;
    }
    


/**
NewL()
Static factory constructor. Uses two phase 
construction.

@leave KErrNoMemory
@return
A pointer to the newly created CT_MsgSmtpServer object.
*/
CT_MsgSmtpServer* CT_MsgSmtpServer::NewL()
	{
	CT_MsgSmtpServer* server=new(ELeave) CT_MsgSmtpServer();
	CleanupStack::PushL(server);
//	server->StartL(KSmtpServer);
	server->ConstructL(KSmtpServer);
	CleanupStack::Pop(server);
	return server;
	}


/**
CT_MsgSmtpServer()
Constructor
*/
CT_MsgSmtpServer::CT_MsgSmtpServer()
:	iSharedDataSMTP(NULL)
	{
	}


/**
CreateSharedDataL()
Creates an object of Shared Data

@return
Returns a CT_MsgSharedDataBase object that would be shared across
the test steps creted by the T_MsgSmtpServer test server.
*/
CT_MsgSharedDataBase* CT_MsgSmtpServer::NewSharedDataL()
	{
	return iSharedDataSMTP = CT_MsgSharedDataSmtp::NewL();
	}
		
			
/**
CreateTestStep()
Creates the test steps based on the name read from the script file

@param	aStepName
The name of the test step to be created

@return
The CTestStep object created
*/ 
CTestStep* CT_MsgSmtpServer::CreateTestStepL(const TDesC& aStepName)
	{
	CTestStep* testStep = NULL;

	 if(aStepName == KCreateSmtpMessageFromEmailFile)
		{
		 testStep = CT_MsgCreateSmtpMessageFromEmailFile::NewL(*iSharedDataSMTP);
		}
	else if(aStepName == KSendSmtpMessage)
		{
		testStep = CT_MsgSendSmtpMessage::NewL(*iSharedDataSMTP);
		}
	else if(aStepName == KSmtpSendEmail)
		{
		testStep = CT_MsgSmtpSendEmail::NewL(*iSharedDataSMTP);
		}
	else if(aStepName == KStartRamUsageTimer)
		{
		testStep = new(ELeave) CT_StartRamUsageTimer(*iSharedDataSMTP);
		}
	else if(aStepName == KStopRamUsageTimer)
		{
		testStep = new(ELeave) CT_StopRamUsageTimer(*iSharedDataSMTP);
		}
	else if(aStepName == KSmtpCreateNewMessageWithPlainBodyText)
		{
		testStep = CT_SmtpCreateNewMessageWithPlainBodyText::NewL(*iSharedDataSMTP);
		}
	else if(aStepName == KSmtpCreateForwadMessage)
		{
		testStep = CT_SmtpCreateForwadMessage::NewL(*iSharedDataSMTP);
		}
	else if(aStepName == KSmtpCreateReplyMessage)
		{
		testStep = CT_SmtpCreateReplyMessage::NewL(*iSharedDataSMTP);
		}
	else if(aStepName == KRetrievePlainBodyTextAndCompare)
		{
		testStep = CT_RetrievePlainBodyTextAndCompare::NewL(*iSharedDataSMTP);
		}
	else if(aStepName == KCreateSmtpMobilityAccount)
		{
		testStep = CT_MsgCreateSmtpMobilityAccount::NewL(*iSharedDataSMTP);
		}
	else if(aStepName == KCreateSmtpAccount)
		{
		testStep = new(ELeave) CT_MsgCreateSmtpAccount(*iSharedDataSMTP);
		}
	else if(aStepName == KCheckSmtpSNAPSetting)
		{
		testStep = new(ELeave) CT_MsgCheckSmtpSNAPSetting(*iSharedDataSMTP);
		}
	else if(aStepName == KModifySmtpSettings)
		{
		testStep = new(ELeave) CT_MsgModifySmtpSettings(*iSharedDataSMTP);
		}
	else if(aStepName == KRemoveSmtpSNAPSetting)
		{
		testStep = new(ELeave) CT_MsgRemoveSmtpSNAPSetting(*iSharedDataSMTP);
		}
	else if(aStepName == KCheckSmtpBMSetting)
		{
		testStep = new(ELeave) CT_MsgCheckSmtpBMSetting(*iSharedDataSMTP);
		}
	else
		{
		testStep = CT_MsgServer::CreateTestStepL(aStepName);
		}
	return testStep;
	}
