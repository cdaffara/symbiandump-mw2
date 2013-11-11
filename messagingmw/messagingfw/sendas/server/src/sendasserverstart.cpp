// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "csendasserver.h"
#include "sendasservername.h"


void StartSendAsServerL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	// create the server (leave it on the cleanup stack)
	CSendAsServer::NewLC();

	// naming the server thread after the server helps to debug panics
	User::LeaveIfError(User::RenameProcess(KSendAsServerName));

	// initialisation complete, now signal the client.
	RProcess::Rendezvous(KErrNone);

	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(2, scheduler);	// scheduler, server
	}

TInt E32Main()
	{
	__UHEAP_MARK;

	CTrapCleanup* cleanup=CTrapCleanup::New();
	TInt r=KErrNoMemory;
	if (cleanup)
		{
		TRAP(r, StartSendAsServerL());
		delete cleanup;
		}	
		
	__UHEAP_MARKEND;
	return r;
	}

