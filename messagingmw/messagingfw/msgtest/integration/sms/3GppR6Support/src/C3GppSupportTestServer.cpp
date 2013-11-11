// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
*/

// EPOC includes
#include "C3GppSupportTestServer.h"
#include "CDeleteChildrenTestStep.h"
#include "CInitSimTsyTestStep.h"
#include "CCloseSimTsyTestStep.h"
#include "CAddIndicationTestStep.h"
#include "CCreateSmsMessageTestStep.h"
#include "CSendSmsTestStep.h"
#include "CVerifyIndicationTestStep.h"
#include "CCreateServiceTestStep.h"
#include "CCreateReplyTestStep.h"
#include "CVerifyRecipientTestStep.h"

#include "MsgingUtils.h"

const TTimeIntervalMicroSeconds32 KWaitForWatchersToStart = 20000000;
const TTimeIntervalMicroSeconds32 KWaitForWatchersToStartDisplayInterval = 5000000;

_LIT(KWatcherExe, "z:\\system\\libs\\watcher.exe");

using namespace SharedData;

LOCAL_C void MainL()
/**
 * Secure variant
 * Much simpler, uses the new Rendezvous() call to sync with the client
 */
	{
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
#endif
	CActiveScheduler*	sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CleanupStack::PushL(sched);
	CActiveScheduler::Install(sched);
	C3GppSupportTestServer*	server = NULL;

// Create the CTestServer derived server
	TRAPD(err,server = C3GppSupportTestServer::NewL());
	CleanupStack::PushL(server);
	if ( err==KErrNone )
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	CleanupStack::PopAndDestroy(2,sched);
	}

GLDEF_C TInt E32Main()
/**
 * @return - Standard Epoc error code on process exit
 * Secure variant only
 * Process entry point. Called by client using RProcess API
 */
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
#if (defined TRAP_IGNORE)
	TRAP_IGNORE(MainL());
#else
	TRAPD(err,MainL());
#endif
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
    }
    
void C3GppSupportTestServer::ConstructL()
	{
	iEntryObserver = CEntryCmdHandler::NewL();
	
	//Creating Watchers
	TInt	ret=iWatcher.Create(KWatcherExe, KNullDesC);
	switch ( ret )
		{
	case KErrNone:
		iWatcher.Resume();
		iCreateFlag = ETrue;
	case KErrAlreadyExists:
		break;
	default:
		User::Leave(ret);
		}	
	}

C3GppSupportTestServer* C3GppSupportTestServer::NewL()
	{
	C3GppSupportTestServer*	server=new(ELeave) C3GppSupportTestServer();
	CleanupStack::PushL(server);
	server->ConstructL();
	server->StartL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}

C3GppSupportTestServer::C3GppSupportTestServer()
:	CTestServer(), iCreateFlag(EFalse)
	{
	}


C3GppSupportTestServer::~C3GppSupportTestServer()
	{
	delete iEntryObserver;
	
	//Killing watchers
	if (iCreateFlag)
		iWatcher.Kill(KErrNone);

	iWatcher.Close();
	}

// this creates returns a test step
CTestStep* C3GppSupportTestServer::CreateTestStep(const TDesC& aStepName)
	{
	CTestStep* testStep = NULL;
	TRAPD(err, testStep = CreateTestL(aStepName));
	
	return testStep;
	}

CTestStep* C3GppSupportTestServer::CreateTestL(const TDesC& aStepName)
	{
	CTestStep* testStep = NULL;
	
	if(aStepName == KDeleteChildrenTestStep)
		{
		testStep = new(ELeave) CDeleteChildrenTestStep(*this);
		}

	if(aStepName == KInitSimTsyTestStep)
		{
		testStep = new(ELeave) CInitSimTsyTestStep(*this);
		}
	
	if(aStepName == KCloseSimTsyTestStep)
		{
		testStep = new(ELeave) CCloseSimTsyTestStep(*this);
		}
	
	if(aStepName == KAddIndicationTestStep)
		{
		testStep = new(ELeave) CAddIndicationTestStep(*this);
		}

	if(aStepName == KCreateSmsMessageTestStep)
		{
		testStep = new(ELeave) CCreateSmsMessageTestStep(*this);
		}

	if(aStepName == KSendSmsTestStep)
		{
		testStep = new(ELeave) CSendSmsTestStep(*this);
		}

	if(aStepName == KVerifyIndicationTestStep)
		{
		testStep = new(ELeave) CVerifyIndicationTestStep(*this);
		}

	if(aStepName == KCreateServiceTestStep)
		{
		testStep = new(ELeave) CCreateServiceTestStep(*this);
		}
	
	if(aStepName == KCreateReplyTestStep)
		{
		testStep = new(ELeave) CCreateReplyTestStep(*this);
		}
		
	if(aStepName == KVerifyRecipientTestStep)
		{
		testStep = new(ELeave) CVerifyRecipientTestStep(*this);
		}
		
	return testStep;	
	}

MMsvEntryObserver* C3GppSupportTestServer::GetObserverHandle()
	{
	return iEntryObserver;
	}

