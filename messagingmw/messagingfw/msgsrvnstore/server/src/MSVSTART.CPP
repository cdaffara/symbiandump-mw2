// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32std.h>
#include <basched.h>

#include "MSVSTD.H"
#include "MSERVER.H"
#include "MSVSERV.H"
#include "MSVPANIC.H"
#include "MSVENTRY.H"

_LIT(KMsvServerPanicString, "MSGS");

//**********************************
// Single exported function
//**********************************

EXPORT_C TInt StartMessageServer(TAny* /*anArg*/)
//
// The message server thread function
//
	{
	TBuf<32> name(KMsvServerName);
		RProcess().RenameMe(name);
	User::RenameThread(name);
	__UHEAP_MARK;

	CTrapCleanup* cleanup=CTrapCleanup::New();
	
	if(!cleanup) 
    	{ 
   		return KErrNoMemory; 
        } 


	CBaActiveScheduler *pA=new CBaActiveScheduler;
	__ASSERT_ALWAYS(pA!=NULL,PanicServer(EMainSchedulerError1));
	CActiveScheduler::Install(pA);

	CMsvServer* server=NULL;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TRAPD(err, server = CMsvServer::NewL(ETrue));
#else
	TRAPD(err, server = CMsvServer::NewL(EFalse));
#endif

	if( err != KErrNone )
		return err;
	
	RProcess::Rendezvous(KErrNone);

	CActiveScheduler::Start();

	// finished
	delete server;
	CActiveScheduler::Install(NULL);
	delete pA;
	delete cleanup;


	__UHEAP_MARKEND;

	return KErrNone;
	}

// Create Message Server and return a server entry owned by caller
// This is all that is required to test server Mtm's
EXPORT_C CServer2* CreateMessageServerL(CMsvServerEntry*& aServerEntry)
	{
	// Create the server - in debug mode (doesn't load mailinit or observers
	// and load the index synchronously)
	CMsvServer* server = CMsvServer::NewL(ETrue);
	CleanupStack::PushL(server);

	// Return the server entry
	aServerEntry = CMsvServerEntry::NewL(*server, KMsvNullIndexEntryId);
	CleanupStack::Pop(); // server
	return server;
	}

//**********************************
//Global functions
//**********************************


GLDEF_C void PanicServer(TMsvServerPanic aPanic)
//
// Panic indicating a server fault
//
	{
	User::Panic(KMsvServerPanicString, aPanic);
	}






