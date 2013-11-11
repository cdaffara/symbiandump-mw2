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
// cwatcherlauncher.cpp
//

#include "cwatcherlauncher.h"

#include <ecom/ecom.h>

#include <watcher.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include "cwatcher.h"
#endif

const TInt KWatcherMaxStartCount = 5;

CWatcherLauncher* CWatcherLauncher::NewL(const TDesC& aWatcherName, TUid aWatcherUid, RFs& aFs, CWatcherLog& aLog)
	{
	CWatcherLauncher* self = new (ELeave) CWatcherLauncher(aWatcherUid, aFs, aLog);
	CleanupStack::PushL(self);
	self->ConstructL(aWatcherName);
	CleanupStack::Pop(self);
	return self;
	}
	
CWatcherLauncher::CWatcherLauncher(TUid aWatcherUid, RFs& aFs, CWatcherLog& aLog)
: CActive(CActive::EPriorityStandard), iWatcherUid(aWatcherUid), iFs(aFs), iLog(aLog)
	{
	CActiveScheduler::Add(this);	
	}
	
void CWatcherLauncher::ConstructL(const TDesC& aWatcherName)
	{
	// Create timer for use in RunError(). Closed if RunL() doesn't leave
	User::LeaveIfError(iTimer.CreateLocal());

	// iWatcherName is used for logging only. Deleted if RunL() doesn't leave
	iWatcherName = aWatcherName.AllocL();

	// Defer launching of the watcher to RunL()
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
	}

CWatcherLauncher::~CWatcherLauncher()
	{
	Cancel();
	
	delete iWatcher;
	delete iWatcherName;
	iTimer.Close();
	
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}
	
// methods from CActive

void CWatcherLauncher::RunL()
	{
	iLog.Printf(_L("Watcher %S starting..."), iWatcherName);
	
	// Create the new watcher object - 	get the instantiation of the active 
	// object form the ECOM server.
	TWatcherParams params(iFs, iLog);
	iWatcher = reinterpret_cast<CActive*>
						(REComSession::CreateImplementationL(
														iWatcherUid,
														iDtor_ID_Key,
														&params));

	// Members no longer required...
	iTimer.Close();
	delete iWatcherName;
	iWatcherName = NULL;
	}
	
void CWatcherLauncher::DoCancel()
	{
	iTimer.Cancel();
	}
	
TInt CWatcherLauncher::RunError(TInt aError)
	{
	iLog.Printf(_L("Watcher %S construction error: %d [startCount=%d]"), iWatcherName, aError, iStartCount);

	// Restart the watcher if iStartCount < KWatcherMaxStartCount
	if( iStartCount < KWatcherMaxStartCount )
		{
		++iStartCount;
		iLog.Printf(_L("Watcher %S restarting in %d secs"), iWatcherName, KWatcherDelay/1000000);
		iTimer.After(iStatus, KWatcherDelay);
		SetActive();
		}
	else
		{
		iLog.Printf(_L("Watcher %S permanent fail"), iWatcherName);
		iTimer.Close();
		}
	return KErrNone;
	}
