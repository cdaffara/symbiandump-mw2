/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Executable module to start the process and load the RadioServer.
*
*/



// INCLUDE FILES
#include 	<e32base.h>
#include    "RadioServer.h"
#include	"RadioClientServer.h"

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RunServerL
// Perform all server initialisation, in particular creation of the
// scheduler and server and then run the scheduler
// -----------------------------------------------------------------------------
//
static void RunServerL()
	{
	// Naming the server thread after the server helps to debug panics
	User::LeaveIfError(User::RenameThread(KRadioServerName));

	// Create and install the active scheduler we need
	CActiveScheduler* scheduler = new(ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	// Create the server (leave it on the cleanup stack)
	CRadioServer* server = CRadioServer::NewL();
	CleanupStack::PushL(server);
	
	// Initialisation complete, now signal the client
	RProcess::Rendezvous(KErrNone);

	// Start the scheduler and wait for client requests
	CActiveScheduler::Start();

	// Ready to exit.
	// Cleanup the scheduler and server
	CleanupStack::PopAndDestroy(2, scheduler);
	
	REComSession::FinalClose();
	}

// -----------------------------------------------------------------------------
// E32Main
// Entry point for the server
// -----------------------------------------------------------------------------
//
TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	TInt r = KErrNoMemory;
	if ( cleanup )
		{
		TRAP(r, RunServerL());
		delete cleanup;
		}
	__UHEAP_MARKEND;
	return r;
	}

//  End of File
