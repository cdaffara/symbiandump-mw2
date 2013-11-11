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
// @file
// This is the framework class implementation for the Test Msg Server.
// 
//

/* User includes */
#include "T_MsgServer.h"
#include "T_MsgSharedDataBase.h"

#include "mobilitytestpropertyvalue.h"
#include "mobilitytestpropertymap.h"
#include "tmobilitytestpropertymapaccess.h"

/* Header files of Test Steps */
#include "T_StartUp.h"
#include "T_ShutDown.h"
#include "T_RecordCurrentTime.h"

/* epoc includes */
#include <miutset.h>

#include <e32const.h>

/**
  Function : CT_MsgServer
  Description : Constructor
  @return : N/A
*/
CT_MsgServer::CT_MsgServer()
:	CTestServer()
,	iSharedDataBase(NULL)
,	iActiveScheduler(NULL)
	{
	
	//Raise process priority to allow complete event logging.
	RProcess me;
    me.SetPriority(TProcessPriority(450));
    User::SetPriorityControl(EFalse);
    
	}




/**
  Function : ~CT_MsgServer
  Description : Destructor
  @return : N/A
*/
CT_MsgServer::~CT_MsgServer()
	{
	}



/**
  Function : StartupServerL
  Description : Creates and installs the active scheduler
  				Creates an object of the Shared Data class
  @return : N/A
*/
void CT_MsgServer::StartupServerL()
	{
	/* Creates the active scheduler, shared data */
	if (iActiveScheduler==NULL)
		{
		/* Installs Active Scheduler */
		iActiveScheduler=new(ELeave) CActiveScheduler;
		CActiveScheduler::Install(iActiveScheduler);
		}

	if (iSharedDataBase==NULL)
		{
		/* Creates the shared data in the re-used thread.*/
		iSharedDataBase = NewSharedDataL();
		}

#ifdef __MOBILITY_TEST_FRAMEWORK
	if (iPropertyWatcher==NULL)
		{
		iPropertyWatcher = CMsgTestPropertyWatcher::NewL(KPropertyCatMsgTestUtilityServer2, EMyPropertyCounter, *this);
		}
#endif //__MOBILITY_TEST_FRAMEWORK
	}



/**
  Function : ShutdownServer
  Description : Deletes the active scheduler and the created shared data object
  @return : N/A
*/
void CT_MsgServer::ShutdownServer()
	{
	if (NULL!=iSharedDataBase)
		{
		delete iSharedDataBase;
		iSharedDataBase=NULL;
		}
	if (NULL!=iActiveScheduler)
		{
		delete iActiveScheduler;
		iActiveScheduler = NULL;			
		}

	if (NULL!=iPropertyWatcher)
		{
		delete iPropertyWatcher;
		iPropertyWatcher = NULL;			
		}
	}


/**
  Function : CreateTestStep
  Description : This method is run in a seperate thread which is re-used 
  				when the -SharedData flag is set.
  @return : A pointer to the CTestStep class
*/
CTestStep* CT_MsgServer::CreateTestStep(const TDesC& aStepName)
	{
	CTestStep* testStep = NULL;

	TRAPD(err,testStep = CreateTestStepL(aStepName));
	if(err)
		{
		ERR_PRINTF1(_L("CT_MsgServer::CreateTestStep() left!"));
		}
	return testStep;
	}



/**
  Function : CreateTestStepL
  Description : This method creates the startup and shut down test steps
  @return : A pointer to the CTestStep class
*/
CTestStep* CT_MsgServer::CreateTestStepL(const TDesC& aStepName)
	{
	CTestStep*	testStep = NULL;

	/* This must be called as the first test step */
	if(aStepName == KStartUp)					
		{
		StartupServerL();
		testStep = new(ELeave) CT_MsgStartUp();
		}
	/* This must be called as the last test step */
	else if(aStepName == KShutDown)				
		{
		ShutdownServer();
		testStep = new(ELeave) CT_MsgShutDown();
		}
	else if(aStepName == KRecordCurrentTime)
		{
		testStep = new(ELeave) CT_MsgRecordCurrentTime();	
		}

	else
		{
		ERR_PRINTF1(_L("CT_MsgServer::CreateTestStep() - Step not found"));	
		}
	return testStep;
	}


#ifdef __MOBILITY_TEST_FRAMEWORK
void CT_MsgServer::HandleEventL(RProperty& aProperty, TUint /*aKey*/)
#else //__MOBILITY_TEST_FRAMEWORK
void CT_MsgServer::HandleEventL(RProperty& /*aProperty*/, TUint /*aKey*/)
#endif //__MOBILITY_TEST_FRAMEWORK
	{
#ifdef __MOBILITY_TEST_FRAMEWORK

	TInt propertyVal = 0;
	User::LeaveIfError(aProperty.Get(propertyVal));

	const TDesC& name = Name(); //Server name.


	const TMsgTestPropertyMapEntry* propertyMap = TMobilityTestPropertyMapAccess::Get(propertyVal);
	if(propertyMap == NULL)
		{
		ERR_PRINTF1(_L("CT_MsgServer::HandleEventL() - Event not found"));
		}
	else
		{
		TBufC8<80> buf8((const unsigned char*)(propertyMap->name));
		TBuf<80> buf;
		buf.Copy(buf8);
		INFO_PRINTF4(_L("%S EVENT [%d %S]"), &name, propertyVal, &buf);
		}
#endif //__MOBILITY_TEST_FRAMEWORK
	}
