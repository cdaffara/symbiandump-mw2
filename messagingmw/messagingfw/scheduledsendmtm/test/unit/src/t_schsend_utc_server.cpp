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

#include "t_schsend_utc_server.h"
#include "t_schsend_utc_scheduledentry_step.h"
#include "t_schsend_utc_rescheduletime_1_step.h"
#include "t_schsend_utc_rescheduletime_2_step.h"
#include "t_schsend_utc_rescheduleconditions_1_step.h"
#include "t_schsend_utc_rescheduleconditions_2_step.h"
#include "t_schsend_utc_rescheduleconditions_3_step.h"
#include "t_schsend_utc_reschedulemixed_1_step.h"
#include "t_schsend_utc_failed_1_step.h"
#include "t_schsend_utc_failed_2_step.h"
 
_LIT(KServerName,"t_schsend_utc_server");

CSchSendUTCServer* CSchSendUTCServer::NewL()
	{
	CSchSendUTCServer * server = new (ELeave) CSchSendUTCServer();
	CleanupStack::PushL(server);
	server-> ConstructL(KServerName);

	CleanupStack::Pop(server);
	return server;
	}
	
LOCAL_C void MainL()
	{
	CActiveScheduler* sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CSchSendUTCServer* server = NULL;
	// Create the CTestServer derived server
	TRAPD(err,server = CSchSendUTCServer::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	server = NULL;
	delete sched;
	}


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
    
RFs& CSchSendUTCServer::Fs()
	{
	return iFs;
	};	

CTestStep* CSchSendUTCServer::CreateTestStep(const TDesC& aStepName)
	{
		
	CTestStep* testStep = NULL; 	
	
	if(aStepName == KScheduledEntryStep)
		testStep = new CScheduledEntryStep();
		
	if(aStepName == KRescheduleTime1Step)
		testStep = new CRescheduleTime1Step();
	
	if(aStepName == KRescheduleTime2Step)
		testStep = new CRescheduleTime2Step();
	
	if(aStepName == KRescheduleConditions1Step)
		testStep = new CRescheduleConditions1Step();
	
	if(aStepName == KRescheduleConditions2Step)
		testStep = new CRescheduleConditions2Step();
		
	if(aStepName == KRescheduleConditions3Step)
		testStep = new CRescheduleConditions3Step();
	
	if(aStepName == KRescheduleMixed1Step)
		testStep = new CRescheduleMixed1Step();
	
	if(aStepName == KFailed1Step)
		testStep = new CFailed1Step();
	
	if(aStepName == KFailed2Step)
		testStep = new CFailed2Step();
	
	
	return testStep;
	}
