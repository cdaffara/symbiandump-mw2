/*
* Copyright (c) 2006,2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  LBT Server Startup
*
*/


// INCLUDE FILES
#include <e32std.h>
#include <e32base.h>
#include "lbtservercore.h"
#include "lbtserverconsts.h"
#include "lbtlogger.h"


// ---------------------------------------------------------------------------
// Starts the server core
// ---------------------------------------------------------------------------
//
void StartServerCoreL()
	{
	User::LeaveIfError(RThread().RenameMe(KLbtServerCoreName));
	
	// Install the scheduler and start the LBT server
	CActiveScheduler* scheduler = new ( ELeave ) CActiveScheduler;
	CleanupStack::PushL( scheduler );
	CActiveScheduler::Install( scheduler );
	
	CLbtServerCore* server = CLbtServerCore::NewLC();
	
	// This starts the LBT Server
	server->StartL(KLbtServerCoreName);
	
	// Finish rendezvous with launching process
	RProcess::Rendezvous( KErrNone );
	
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy( 2, scheduler );
	}

// ---------------------------------------------------------------------------
// Entry function of the exe
// ---------------------------------------------------------------------------
//
TInt E32Main()
	{
	__UHEAP_MARK;

	CTrapCleanup* cleanupStack = CTrapCleanup::New();

    // The ret is initialized to KErrNoMemory so that the function returns 
    // KErrNoMemory if the cleanupstack allocation fails.
	TInt ret = KErrNoMemory;
	if (cleanupStack)
		{
		TRAP(ret, StartServerCoreL());
		delete cleanupStack;
		}
		
	STOPLOGGING;

	__UHEAP_MARKEND;

	return ret;
	}

// end of file
