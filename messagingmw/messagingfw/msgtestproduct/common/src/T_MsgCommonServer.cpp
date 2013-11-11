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

/* User includes */
#include "T_MsgServer.h"
#include "T_MsgCommonServer.h"
#include "T_MsgSharedDataCommon.h"


/* Header files for test steps */
#include "T_InitNtRas.h"
#include "T_ResetMessageStore.h"
#include "T_CheckChildrenCount.h"

#include "T_MsgEventHandlerAdd.h"
#include "T_MsgEventHandlerInit.h"
#include "T_MsgEventHandlerCheck.h"
#include "t_msgsearchentry.h"
#include "t_changehometime.h"
#include "t_msgclientsearch.h"
#include "t_cmsvsession.h"
#include "t_cmsventry.h"

#include "t_deletemessageselection.h"
#include "t_createsearchsortquery.h"
#include "t_searchsortbyqueryobject.h"
#include "t_searchsortresultbyqueryid.h"
#include "t_modifyindexentryflags.h"
#include "t_verifysearchresult.h"
#include "t_verifysortresult.h"
#include "t_msgsearchperformance.h"
#include "t_msgperfsearchusingexisting.h"
#include "t_msgperfsearchusing1667qo.h"
#include "t_createperfsearchsortquery.h"
#include "t_msgperfsearchsortusingqi.h"
#include "t_verifyheaders.h"

/**
  MainL()
  Description :		This is the main function which installs the 
					active scheduler and creates an object of the Common server
  @return none
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
	CT_MsgCommonServer* server = NULL;

	/* Creates the CTestServer derived Common server */
	TRAPD(err, server = CT_MsgCommonServer::NewL());
	if ( err == KErrNone )
		{
		/* Syncs with the client and enters the active scheduler */
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	
	/* Deletes the server and active scheduler object */
	delete server;
	server=NULL;
	delete sched;
	sched=NULL;
	}




/**
  E32Main()
  Description :	It is the entry point for the server
  @return Standard Epoc error code on process exit
*/
GLDEF_C TInt E32Main()
/**
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
	TRAP_IGNORE(MainL());	
	delete cleanup;
	cleanup=NULL;
	__UHEAP_MARKEND;
	
	return KErrNone;
    }



/**
  Function : NewL
  Description : Creates an object of CT_MsgCommonServer and 
			    Starts the server.
  @return : A pointer of type CT_MsgCommonServer
  @leave  : KErrNoMemory if	sufficient memory is not available
*/
CT_MsgCommonServer* CT_MsgCommonServer::NewL()
	{
	CT_MsgCommonServer* server=new(ELeave) CT_MsgCommonServer();
	CleanupStack::PushL(server);
	server->StartL(KMsgCommonServer);
	CleanupStack::Pop(server);
	return server;
	}


/**
  Function : CT_MsgCommonServer
  Description : Constructor
  @return : N/A
*/
CT_MsgCommonServer::CT_MsgCommonServer()
:	CT_MsgServer()
,	iSharedDataCommon(NULL)
	{	
	}


/**
  Function : NewSharedDataL
  Description : Creates the object of the shared data class
  @return : A pointer to CT_MsgSharedDataBase
  @leave  : KErrNoMemory if	sufficient memory is not available
*/
CT_MsgSharedDataBase* CT_MsgCommonServer::NewSharedDataL()
	{
	return iSharedDataCommon = CT_MsgSharedDataCommon::NewL();
	}


/**
  Function : CreateTestStepL
  Description : Creates the test steps based on the test step name read 
				from the script file
  @param :	aStepName		The name of the test step to be created
  @return : A pointer to the CTestStep object created
*/ 
CTestStep* CT_MsgCommonServer::CreateTestStepL(const TDesC& aStepName)
	{
	CTestStep* testStep = NULL;

	if(aStepName == KCheckChildrenCount)
		{
		testStep = new(ELeave) CT_MsgCheckChildrenCount(*iSharedDataCommon);
		}
	else if(aStepName == KResetMessageStore)
		{
		testStep = CT_MsgResetMessageStore::NewL(*iSharedDataCommon);
		}
	else if(aStepName == KInitNtRas)
		{
		testStep = new(ELeave) CT_MsgInitNtRas(*iSharedDataCommon);
		}
//	else if(aStepName == KMsgBlank)
//		{
//		testStep = CT_MsgBlank::NewL(*iSharedDataCommon);
//		}		
//	else if(aStepName == KMsgSyncBlank)
//		{
//		testStep = new(ELeave) CT_MsgSyncBlank(*iSharedDataCommon);
//		}
	else if(aStepName == KMsgEventHandlerAdd)
		{
		testStep = new(ELeave) CT_MsgEventHandlerAdd(*iSharedDataCommon);
		}
	else if(aStepName == KMsgEventHandlerInit)
		{
		testStep = new(ELeave) CT_MsgEventHandlerInit(*iSharedDataCommon);
		}
	else if(aStepName == KMsgEventHandlerCheck)
		{
		testStep = new(ELeave) CT_MsgEventHandlerCheck(*iSharedDataCommon);
		}	
	else if(aStepName == KSearchEntry)
		{
		testStep = CT_MsgSearchEntry::NewL(*iSharedDataCommon);
		}	
	else if(aStepName == KClientSearch)
		{
		testStep = CT_MsgClientSearch::NewL(*iSharedDataCommon);
		}	
	else if(aStepName == KChangeHomeTime)
		{
		testStep = new(ELeave) CT_MsgChangeHomeTime(*iSharedDataCommon);
		}
	else if(aStepName == KDeleteMessageSelection)
		{
		testStep = CT_MsgDeleteMessageSelection::NewL(*iSharedDataCommon);
		}
	else if(aStepName == KCreateSearchSortQuery)
		{
		testStep = new(ELeave) CT_MsgCreateSearchSortQuery(*iSharedDataCommon);
		}
	else if(aStepName == KSearchSortByQueryObject)
		{
		testStep = CT_MsgSearchSortByQueryObject::NewL(*iSharedDataCommon);
		}
	else if(aStepName == KSearchSortResultByQueryId)
		{
		testStep = CT_MsgSearchSortResultByQueryId::NewL(*iSharedDataCommon);
		}
	else if(aStepName == KModifyIndexEntryFlags)
		{
		testStep = new(ELeave) CT_MsgModifyIndexEntryFlags(*iSharedDataCommon);
		}
	else if(aStepName == KVerifySearchResult)
		{
		testStep = new(ELeave) CT_MsgVerifySearchResult(*iSharedDataCommon);
		}
	else if(aStepName == KVerifySortResult)
		{
		testStep = new(ELeave) CT_MsgVerifySortResult(*iSharedDataCommon);
		}
	else if(aStepName == KSearchPerformance)
		{
		testStep = CT_MsgSearchPerformance::NewL(*iSharedDataCommon);
		}	
	else if(aStepName == KPerfSearchUsingExisting)
		{
		testStep = CT_MsgPerfSearchUsingExisting::NewL(*iSharedDataCommon);
		}
	else if(aStepName == KPerfSearchUsing1667QO)
		{
		testStep = CT_MsgPerfSearchUsing1667QO::NewL(*iSharedDataCommon);
		}
	else if(aStepName == KCreatePerfSearchSortQuery)
		{
		testStep = new(ELeave) CT_MsgCreatePerfSearchSortQuery(*iSharedDataCommon);
		}
	else if(aStepName == KPerfSearchSortUsingQI)
		{
		testStep = CT_MsgPerfSearchSortUsingQI::NewL(*iSharedDataCommon);
		}
	else if(aStepName == KUnitTestCMsvSession)
		{
		testStep = new (ELeave)CT_CMsvSession(*iSharedDataCommon);
		}
	else if(aStepName == KUnitTestCMsvEntry)
		{
		testStep = new (ELeave)CT_CMsvEntry(*iSharedDataCommon);
		}
	else if(aStepName == KVerifyHeaders)
		{
		testStep = new (ELeave)CT_MsgVerifyHeaders(*iSharedDataCommon);
		}
	/* 
	 * If the test step names do not match then call the base class
	 * CreateTestStepL
	 */
    else
		{
		testStep=CT_MsgServer::CreateTestStepL(aStepName);
		}
	return testStep;
	}
