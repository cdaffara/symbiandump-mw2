/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:   Server startup code
*
*/



// INCLUDE FILES
#include <e32base.h>
#include <centralrepository.h>

#include "epos_startsuplserver.h"
#include "epos_csuplserver.h"

//#ifdef _DEBUG
#include "epos_csuplglobal.h"
//#endif

_LIT(KSuplServerStartupSemaphoreName, "SuplServerStartupSem");
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// StartSuplServerL
// Server startup routine
// ---------------------------------------------------------
//
void StartSuplServerL()
	{
	// Rename thread to aid debugging
	User::LeaveIfError(RThread().RenameMe(KSuplServerName));

	RSemaphore startupSemaphore;
	// Start scheduler and server
	CActiveScheduler* scheduler = new(ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

//#ifdef _DEBUG
    SuplGlobal::SetupTraceL();
    _LIT(KTraceFileName, "SUPL_GW::epos_suplserverstartup.cpp");
    _LIT(KTracePointStart, "SuplServerStartup");
    SuplGlobal::Trace(KTracePointStart, KTraceFileName, __LINE__);
//#endif
    CSuplServer* server = NULL;
	//TInt err;
	TRAPD(err, server = CSuplServer::NewL());
	
	if (err != KErrNone)
	{
		if ( startupSemaphore.OpenGlobal( KSuplServerStartupSemaphoreName ) == KErrNone 
       )
        {
        //Signal the client:The server might have started up successfully or not
        startupSemaphore.Signal(KMaxClients);
        }

    startupSemaphore.Close();
    
    _LIT(KTracePointStop, "SuplServerShutdown");
    SuplGlobal::Trace(KTracePointStop, KTraceFileName, __LINE__);
    SuplGlobal::TearDownTrace();
		
		User::Leave(err);
	}
	CleanupStack::PushL(server);

	// Signal that we've started
	RProcess::Rendezvous(KErrNone);
	
	CleanupStack::Pop(server);
	CleanupStack::Pop(scheduler);

	// Close semaphore since the startup is now ready
	//CleanupStack::PopAndDestroy(&startingSemaphore);
   
    // Signal to semaphore that server startup is completed
	
    if ( startupSemaphore.OpenGlobal( KSuplServerStartupSemaphoreName ) == KErrNone 
       )
        {
        //Signal the client:The server might have started up successfully or not
        startupSemaphore.Signal(KMaxClients);
        }

    startupSemaphore.Close();
    
    CActiveScheduler::Start();
	delete server;
	delete scheduler;
	
//#ifdef _DEBUG
    _LIT(KTracePointStop, "SuplServerShutdown");
    SuplGlobal::Trace(KTracePointStop, KTraceFileName, __LINE__);
    SuplGlobal::TearDownTrace();
//#endif

	}

//
// ---------------------------------------------------------
// E32Dll
// Server process entry-point
// Recover the startup parameters and run the server
// ---------------------------------------------------------
TInt E32Main()
	{
	__UHEAP_MARK;

	CTrapCleanup* cleanupStack = CTrapCleanup::New();

	TInt ret = KErrNoMemory;
	if (cleanupStack)
		{
		TRAP(ret, StartSuplServerL());
		delete cleanupStack;
		}

	__UHEAP_MARKEND;

	return ret;
	}

//  End of File
