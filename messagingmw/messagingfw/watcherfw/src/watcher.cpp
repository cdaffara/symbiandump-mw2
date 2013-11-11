// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// watcher.cpp
//

#include <basched.h>
#include <watcher.h>
#include "watcherdef.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include "cwatcher.h"
#endif

#include "cwatcherssastartupmgr.h"

#pragma warning	(disable : 4702)

_LIT(KThreadName1, "WatcherMainThread%d");
_LIT(KThreadName2, "WatcherThread");
const TInt KMinWatcherHeapSize = 0x8000;
const TInt KMaxWatcherHeapSize = 0x200000;


void DoLaunchWatchersL()
	{
	// Create scheduler
	CBaActiveScheduler* scheduler = new(ELeave)CBaActiveScheduler;
	CleanupStack::PushL(scheduler);

	// Install scheduler
	CActiveScheduler::Install(scheduler);

	// Launch the start-up manager that will launch the watchers in the 
 	// correct start-up state
	CWatcherSSAStartupMgr* startupMgr = CWatcherSSAStartupMgr::NewL();
	CleanupStack::PushL(startupMgr);

	// The System Starter is waiting for us (start_method = EWaitForStart)
	// so we rendez-vous so it can start the next process in the SSC
	RProcess::Rendezvous(KErrNone);

	// We start the scheduler even if there are no watchers (vs previous
	// behaviour) as the SSA manager is also an active object
	CActiveScheduler::Start();
	
	// Tidy up
	CleanupStack::PopAndDestroy(2, scheduler);
	}

static TInt DoLaunchThread(TAny*)
	{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	__ASSERT_ALWAYS(cleanup, Panic(ENoCleanup));
	TRAP_IGNORE(DoLaunchWatchersL());		
	delete cleanup;
	return 0;
	}

//**********************************
// CWatcher
//**********************************

EXPORT_C CWatcher* CWatcher::NewL(TInt aPriority)
	{
	return new(ELeave)CWatcher(aPriority);
	}

EXPORT_C CWatcher::~CWatcher()
	{
	Cancel();
	}

EXPORT_C void CWatcher::Start(TRequestStatus& aStatus)
	{
	// Try and create the watcher thread
	iStatus = KRequestPending;

	// Make sure the watching thread has a unique name
	TName threadName;
	threadName.Format(KThreadName1, iLaunchCount++);

#ifdef __X86GCC__
	// Some watchers overflow their stack on this platform when using KDefaultStackSize
	TInt error = iThread.Create(threadName, DoLaunchThread, 2*KDefaultStackSize, KMinWatcherHeapSize, KMaxWatcherHeapSize, NULL, EOwnerThread);
#else	
	TInt error = iThread.Create(threadName, DoLaunchThread, KDefaultStackSize, KMinWatcherHeapSize, KMaxWatcherHeapSize, NULL, EOwnerThread);
#endif // __X86GCC__
	if (!error)
		{
		// Complete the request when the thread dies
		iThread.Logon(iStatus);
		iThread.Resume();
		}
	else
		{
		// Complete ourselves with the error
		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, error);
		}

	aStatus = KRequestPending;
	iObserver = &aStatus;
	SetActive();
	}

CWatcher::CWatcher(TInt aPriority)
: CActive(aPriority)
	{
	CActiveScheduler::Add(this);
	}

void CWatcher::DoCancel()
	{
	// Kill off the thread
	iThread.Kill(KErrCancel);
	iThread.Close();

	User::RequestComplete(iObserver, KErrCancel);
	}

void CWatcher::RunL()
	{
	iThread.Close();

	// Wait a bit
	User::After(KWatcherDelay);
	User::RequestComplete(iObserver, iStatus.Int());
	}

//**********************************
// Watcher Exe
//**********************************

LOCAL_C void DoStartL()
	{
	User::LeaveIfError(User::RenameThread(KThreadName2));

	CActiveScheduler::Install(new(ELeave)CActiveScheduler);
	CleanupStack::PushL(CActiveScheduler::Current());
	CWatcherWait* wait = CWatcherWait::NewLC();

	// The watchers should always be running
	CWatcher* watcher = CWatcher::NewL(CActive::EPriorityStandard);
	FOREVER
		{
		wait->Start();
		watcher->Start(wait->iStatus);
		CActiveScheduler::Start();
		}

	// The following statements are never going to execute due to the endless loop above.
	// They have been commented out to keep the compiler happy.
	//delete watcher;
	//CleanupStack::PopAndDestroy(4); // wait, CActiveScheduler, library, semaphore
	}

GLDEF_C TInt E32Main()
	{       
	__UHEAP_MARK;
	CTrapCleanup* cleanup=CTrapCleanup::New();
	TRAP_IGNORE(DoStartL());          
	delete cleanup;      
	__UHEAP_MARKEND;
	return(KErrNone);
	}

EXPORT_C TInt WinsMain()
	{
	return KErrNone;
	}


#pragma warning	(default : 4702)
