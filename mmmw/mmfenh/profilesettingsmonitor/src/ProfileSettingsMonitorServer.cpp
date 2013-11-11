/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains implementation of ProfileSettingsMonitorServer.
*
*/


#include "MmfProfileSettingsMonitorClientServer.h"
#include "ProfileSettingsMonitorServer.h"
#include "ProfileSettingsMonitorServerImpl.h"

CProfileSettingsMonitorServer* CProfileSettingsMonitorServer::NewL()
	{
	CProfileSettingsMonitorServer* s = new(ELeave) CProfileSettingsMonitorServer();
	CleanupStack::PushL(s);
	s->ConstructL();
	CleanupStack::Pop();
	return s;
	}

CProfileSettingsMonitorServer::CProfileSettingsMonitorServer()
    :   CServer2(EPriorityStandard)
	{
	}

void CProfileSettingsMonitorServer::ConstructL()
	{
	// Call base class to Start server
	iServerImpl = CProfileSettingsMonitorServerImpl::NewL();
	StartL(KNullDesC);
	}

CProfileSettingsMonitorServer::~CProfileSettingsMonitorServer()
	{
	}

CSession2* CProfileSettingsMonitorServer::NewSessionL(const TVersion& /*aVersion*/,const RMessage2& /*aMessage*/) const
	{
	    User::Leave(KErrNotSupported);
	    return NULL;
	}

static void StartProfileSettingsMonitorServerL()
// Perform all server initialisation, in particular creation of the
// scheduler and server and then run the scheduler
//
	{
	// Naming the server thread after the server helps to debug panics
	User::LeaveIfError(User::RenameThread(KProfileSettingsMonitorServerName));

	// Create and install the active scheduler we need
	CActiveScheduler* scheduler = new(ELeave)CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	//create the server & leave it on the cleanupstack
	CleanupStack::PushL(CProfileSettingsMonitorServer::NewL());

	// Initialisation complete, now signal the client
	RProcess::Rendezvous(KErrNone);
	
	// Start the scheduler and wait for client requests
	CActiveScheduler::Start();

    //now exiting the server so cleanup
	CleanupStack::PopAndDestroy(2);//scheduler and server
	}

TInt E32Main()
//
// Server process entry-point
// Recover the startup parameters and run the server
//
	{
	__UHEAP_MARK;
	//
	CTrapCleanup* cleanup=CTrapCleanup::New();
	TInt r=KErrNoMemory;
	if (cleanup)
		{
		TRAP(r,StartProfileSettingsMonitorServerL());
		delete cleanup;
		}
	//
	__UHEAP_MARKEND;
	return r;
	}
	
