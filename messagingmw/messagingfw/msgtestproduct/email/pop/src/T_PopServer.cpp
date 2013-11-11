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
#include "T_PopServer.h"
#include "T_MsgSharedDataPop.h"

// Header files of Test Steps POP
#include "T_CreatePopAccount.h"
#include "T_DeletePopAccount.h"
#include "T_VerifyPopAccountsCount.h"
#include "T_ModifyPopSettings.h"
#include "T_ComparePopSettings.h"
#include "T_VerfiyPopSettings.h"
#include "T_ConnectPop3Server.h"
#include "T_DisConnectPop3Server.h"
#include "T_ComparePopEmailMsgs.h"
#include "T_CopyPopSelection.h"
#include "T_PopInvokeAsyncFunction.h"
#include "T_DeleteAllPopChildren.h"
#include "T_CheckChildrenCountPop.h"
#include "T_StartRamUsageTimer.h"
#include "T_StopRamUsageTimer.h"
#include "T_CheckPop3AccessPoint.h"
#include "T_CreatePopAccountWithoutSmtp.h"
#include "T_ModifyPopSettings2.h"
#include "T_CheckPopSNAPSetting.h"
#include "T_RemovePopSNAPSetting.h"
#include "T_CheckPopBMSetting.h"
#include "t_cpop3clientmtm.h"
#include "t_verifyheaders.h"

/**
MainL()
This is the main function which installs the active scheduler and
creates an object of the Email server
*/
LOCAL_C void MainL()
/**
Secure variant
Much simpler, uses the new Rendezvous() call to sync with the client
*/
	{
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);

	CActiveScheduler*	sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CT_MsgPopServer* server = NULL;

	// Create the CTestServer derived Email server
	TRAPD(err, server = CT_MsgPopServer::NewL());
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
It is the entry point for POP testserver

@return
Returns KErrNone on suceed otherwise the error code
*/
GLDEF_C TInt E32Main()
/**
@return
Standard Epoc error code on process exit
Secure variant only
Process entry point. Called by client using RProcess API
*/
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
Constructs a CT_MsgPopServer object.
Uses two phase construction and leaves nothing on the CleanupStack.

@leave KErrNoMemory
@return
Created object of type CT_MsgPopServer
*/
CT_MsgPopServer* CT_MsgPopServer::NewL()
	{
	CT_MsgPopServer*	server=new(ELeave) CT_MsgPopServer();
	CleanupStack::PushL(server);
//	server->StartL(KPopServer);
	server->ConstructL(KPopServer);
	CleanupStack::Pop(server);
	return server;
	}


/**
CT_MsgPopServer()
Intializes iSharedDataPOP with NULL
*/
CT_MsgPopServer::CT_MsgPopServer()
:	iSharedDataPOP(NULL)
	{
	}

/**
NewSharedDataL()
Constructs a CT_MsgSharedDataPop object dynamically and assigns it's address to iSharedDataPOP

@return
An object of CT_MsgSharedDataPop
*/
CT_MsgSharedDataBase* CT_MsgPopServer::NewSharedDataL()
	{
	return iSharedDataPOP = CT_MsgSharedDataPop::NewL();
	}


/**
CreateTestStepL()
Creates the test steps based on the name read from the script file

@param aStepName
The name of the test step to be created

@return
The CTestStep object created
*/
CTestStep* CT_MsgPopServer::CreateTestStepL(const TDesC& aStepName)
	{
	CTestStep* testStep = NULL;

	if(aStepName == KVerifyPopSettings)
		{
		testStep = new(ELeave) CT_MsgVerifyPopSettings(*iSharedDataPOP);
		}
	else if(aStepName == KDeleteAllPopChildren)
		{
		testStep = CT_MsgDeleteAllPopChildren::NewL(*iSharedDataPOP);
		}
	else if(aStepName == KCopyPopSelection)
		{
		testStep = CT_MsgCopyPopSelection::NewL(*iSharedDataPOP);
		}
	else if(aStepName == KCreatePopAccount)
		{
		testStep = new(ELeave) CT_MsgCreatePopAccount(*iSharedDataPOP);
		}
	else if(aStepName == KModifyPopSettings)
		{
		testStep = new(ELeave) CT_MsgModifyPopSettings(*iSharedDataPOP);
		}
	else if(aStepName == KComparePopSettings)
		{
		testStep = new(ELeave) CT_MsgComparePopSettings(*iSharedDataPOP);
		}
	else if(aStepName == KDeletePopAccount)
		{
		testStep = new(ELeave) CT_MsgDeletePopAccount(*iSharedDataPOP);
		}
	else if(aStepName == KVerifyPopAccountsCount)
		{
		testStep = new(ELeave) CT_MsgVerifyPopAccountsCount(*iSharedDataPOP);
		}
	else if(aStepName == KPopInvokeAsyncFunction)
		{
		testStep = CT_MsgPopInvokeAsyncFunction::NewL(*iSharedDataPOP);
		}
	else if(aStepName == KCheckChildrenCountPop)
		{
		testStep = new(ELeave) CT_MsgCheckChildrenCountPop(*iSharedDataPOP);
		}
	else if(aStepName == KComparePopEmailMsgs)
		{
		testStep = CT_MsgComparePopEmailMsgs::NewL(*iSharedDataPOP);
		}
	else if(aStepName == KConnectPop3Server)
		{
		testStep = CT_MsgConnectPop3Server::NewL(*iSharedDataPOP);
		}
	else if(aStepName == KDisConnectPop3Server)
		{
		testStep = CT_MsgDisConnectPop3Server::NewL(*iSharedDataPOP);
		}
	else if(aStepName == KStartRamUsageTimer)
 		{
 		testStep = new(ELeave) CT_StartRamUsageTimer(*iSharedDataPOP);
 		}
 	else if(aStepName == KStopRamUsageTimer)
 		{
 		testStep = new(ELeave) CT_StopRamUsageTimer(*iSharedDataPOP);
 		}
 	else if(aStepName == KCheckPop3AccessPoint)
 		{
 		testStep = new(ELeave) CT_MsgCheckPop3AccessPoint(*iSharedDataPOP);
 		}
 	else if(aStepName == KCreatePopAccountWithoutSmtp)
 		{
 		testStep = new(ELeave) CT_MsgCreatePopAccountWithoutSmtp(*iSharedDataPOP);
 		}
 	else if(aStepName == KModifyPopSettings2)
 		{
 		testStep = new(ELeave) CT_MsgModifyPopSettings2(*iSharedDataPOP);
 		}
 	else if(aStepName == KCheckPopSNAPSetting)
 		{
 		testStep = new(ELeave) CT_MsgCheckPopSNAPSetting(*iSharedDataPOP);
 		}
 	else if(aStepName == KRemovePopSNAPSetting)
 		{
 		testStep = new(ELeave) CT_MsgRemovePopSNAPSetting(*iSharedDataPOP);
 		}
 	else if(aStepName == KCheckPopBMSetting)
 		{
 		testStep = new(ELeave) CT_MsgCheckPopBMSetting(*iSharedDataPOP);
 		}
 	else if(aStepName == KUnitTestCPop3ClientMtm)
 		{
 		testStep = new(ELeave) CT_CPop3ClientMtm(*iSharedDataPOP);
 		}
 	else if(aStepName == KVerifyHeaders)
 		{
 		testStep = new(ELeave) CT_MsgVerifyHeaders(*iSharedDataPOP);
 		}
	else
		{
		testStep = CT_MsgServer::CreateTestStepL(aStepName);
		}

	return testStep;
	}

