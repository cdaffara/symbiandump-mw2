// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This is the test server for all LbsSuplSmsTrigger unit tests.
// 
//

/**
 @file
 @internalComponent
 @prototype
*/
#include <lbs/lbsadmin.h>

#include "lbssystemcontroller.h"

#include "Te_LbsSuplSmsTriggerServer.h"
#include "Te_LbsSuplSmsTriggerIntegStep.h"

/** The UID of the SUPL Sms Trigger unit test suite dll*/
const TUid  KLbsSuplPushSuiteUid = {0x10283755};

/** The string name of the test suite */
_LIT(KServerName,"Te_LbsSuplSmsTriggerSuite");

/**
Static factory method for creating an instance of the CTe_LbsSuplSmsTriggerSuite class.

@return An instance of the class. The calling application becomes the
owner of the returned instance and is responsible its disposal.

@leave If a error happens, it leaves with one of the system error codes.
*/
CTe_LbsSuplSmsTriggerSuite* CTe_LbsSuplSmsTriggerSuite::NewL()
	{
	CTe_LbsSuplSmsTriggerSuite* server = new (ELeave) CTe_LbsSuplSmsTriggerSuite();
	CleanupStack::PushL(server);
	server->ConstructL();
	CleanupStack::Pop(server);
	return server;
	}


/**
2nd phase constructor. Calls the base class method passing the name of the suite.

@leave If a error happens, it leaves with one of the system error codes.
*/
void CTe_LbsSuplSmsTriggerSuite::ConstructL()
	{
	//ConstructL of the base class
	CTestServer::ConstructL(KServerName);
	
	CLbsAdmin* adminApi = CLbsAdmin::NewL();
	CleanupStack::PushL(adminApi);
	User::LeaveIfError(adminApi->Set(KLbsSettingLogger, CLbsAdmin::ELoggerOff));
	CleanupStack::PopAndDestroy();
	
	//We start LBS system here to define the P&S properties  
	RLbsSystemController lbsSysController;
	lbsSysController.OpenL(KLbsSuplPushSuiteUid);
	CleanupClosePushL(lbsSysController);
		
	//Start the LBS sub-system if it is not running
	if(lbsSysController.RequestSystemStartup()!= ELbsStartupSuccess)
		{
		User::Leave(KErrNotReady);
		}
	//We do not need Lbs to be running - not to interfere with our tests
	User::LeaveIfError(lbsSysController.RequestCompleteCloseDown());
		
	CleanupStack::PopAndDestroy(&lbsSysController);	
	}


/**
Secure variant. Much simpler, uses the new Rendezvous() call to sync with the client.

@leave If a error happens, it leaves with one of the system error codes.
*/
LOCAL_C void MainL()
	{
	// Leave the hooks in for platform security
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().DataCaging(RProcess::ESecureApiOn);
#endif
	CActiveScheduler* sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	
	CTe_LbsSuplSmsTriggerSuite* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CTe_LbsSuplSmsTriggerSuite::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	
	delete server;
	delete sched;
	}


/**
Secure variant only. Process entry point. Called by client using RProcess API.
 
@return - Standard Epoc error code on process exit.
*/
GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAPD(err,MainL());
	delete cleanup;
	__UHEAP_MARKEND;
	return err;
    }

/**
Overrides the pure virtual CTestServer::CreateTestStep. Creates a test step by its name.

@return - A CTestStep derived instance or NULL if error or there is no a test step with the name
specified.

@see CTestServer::CreateTestStep
*/
CTestStep* CTe_LbsSuplSmsTriggerSuite::CreateTestStep(const TDesC& aStepName)
	{
	CTestStep* step = NULL;
	TRAPD(err, step = CreateTestStepL(aStepName));
	if(err!=KErrNone)
		{
		return NULL;
		}
	return step;
	}

/**
The leaving version of the CTe_LbsSuplSmsTriggerSuiteCreateTestStep. The leaving new operator is used 
to initialize the test step object with zeroes.

@return - A CTestStep derived instance or NULL if error or there is no a test step with the name
specified.

@leave KErrNoMemory if not enough memory to create the test step.

@see CTe_LbsSuplSmsTriggerSuite::CreateTestStep
*/
CTestStep* CTe_LbsSuplSmsTriggerSuite::CreateTestStepL(const TDesC& aStepName)
	{
	if(aStepName==KLbsSuplSmsTriggerIntegStep)
    	{
    	return new(ELeave) CTe_LbsSuplSmsTriggerIntegStep();
    	}
	return NULL;
	}
