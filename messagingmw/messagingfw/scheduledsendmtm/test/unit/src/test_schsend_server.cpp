// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "test_schsend_server.h"
#include "test_schedulepackage_step.h"
#include "test_scheduledentry_step.h"
#include "test_offpeaktime_step.h"
#include "test_schedulesettings_step.h"
#include "test_sysagentactions_step.h"
#include "test_erroractions_step.h"


_LIT(KServerName,"Test_SchSend_Server");
/**
Called inside the MainL() function to create and start the
CTestServer derived server.

@return Instance of the test server
*/
CTest_SchSend_Server* CTest_SchSend_Server::NewL()
	{
	CTest_SchSend_Server * server = new (ELeave) CTest_SchSend_Server();
	CleanupStack::PushL(server);
	// CServer base class call
	server->StartL(KServerName);
	CleanupStack::Pop(server);
	return server;
	}

// EKA2 much simpler
// Just an E32Main and a MainL()
LOCAL_C void MainL()
	{
	// Leave the hooks in for platform security
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
#endif
	CActiveScheduler* sched= new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CTest_SchSend_Server* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CTest_SchSend_Server::NewL());
	if(err == KErrNone)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	}


// Only a DLL on emulator for typhoon and earlier

/**
@return Standard Epoc error code on exit
*/
GLDEF_C TInt E32Main()
	{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAPD(err,MainL());
	delete cleanup;
	return KErrNone;
    }

// Create a thread in the calling process
// Emulator typhoon and earlier

/**
Implementation of CTestServer pure virtual

@param	aStepName	The test step name to run 
@return  A CTestStep derived instance
*/
CTestStep* CTest_SchSend_Server::CreateTestStep(const TDesC& aStepName)
	{
	CTestStep* testStep = NULL;
	if(aStepName == KMsvSysAgentActionsStep)
		testStep = new CMsvSysAgentActionsStep();
	else if(aStepName == KMsvScheduledEntryStep)
		testStep = new CMsvScheduledEntryStep();	
	else if(aStepName == KMsvScheduleSettingsStep)
		testStep = new CMsvScheduleSettingsStep();
	else if(aStepName == KMsvSchedulePackageStep)
		testStep = new CMsvSchedulePackageStep();
	else if(aStepName == KMsvOffPeakTimeStep)
		testStep = new CMsvOffPeakTimeStep();
	else if(aStepName == KMsvOffPeakTimeArrayStep)
		testStep = new CMsvOffPeakTimeArrayStep();	
	else if(aStepName == KMsvSendErrorActionsStep)
		testStep = new CMsvSendErrorActionsStep();			
	return testStep;
	}
