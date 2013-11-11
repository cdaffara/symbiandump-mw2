// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "msvtestutilsbase.h"
#include <watcher.h>

_LIT(KWatcherExe, "z:\\system\\libs\\watcher.exe");

EXPORT_C CTestUtilsWatcherStarter* CTestUtilsWatcherStarter::NewL(TRequestStatus& aStatus, TTimeIntervalMicroSeconds32 aWaitToStart, TInt aPriority)
	{
	CTestUtilsWatcherStarter* self = NewL(aPriority);
	CleanupStack::PushL(self);

	self->ConstructL(aStatus, aWaitToStart);

	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CTestUtilsWatcherStarter* CTestUtilsWatcherStarter::NewL(TInt aPriority)
	{
	CTestUtilsWatcherStarter* self = new (ELeave) CTestUtilsWatcherStarter(aPriority);
	CleanupStack::PushL(self);

	self->ConstructL();

	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CTestUtilsWatcherStarter::~CTestUtilsWatcherStarter()
	{
	Cancel();
	iTimer.Close();
	if (iConstructed)
		iWatcher.Kill(KErrNone);

	iWatcher.Close();
	}

CTestUtilsWatcherStarter::CTestUtilsWatcherStarter(TInt aPriority)
: CActive(aPriority)
	{
	CActiveScheduler::Add(this);
	}

void CTestUtilsWatcherStarter::RunL()
	{
	//Completes when timer has elapsed
	User::RequestComplete(iObserver, iStatus.Int());
	}

void CTestUtilsWatcherStarter::DoCancel()
	{
	iTimer.Cancel();
	User::RequestComplete(iObserver, KErrCancel);
	}

void CTestUtilsWatcherStarter::ConstructL()
	{
	User::LeaveIfError(iWatcher.Create(KWatcherExe, KNullDesC));
	iWatcher.Resume();
	iConstructed = ETrue;
	}

void CTestUtilsWatcherStarter::ConstructL(TRequestStatus& aStatus, TTimeIntervalMicroSeconds32 aWaitToStart)
	{
	ConstructL();
	User::LeaveIfError(iTimer.CreateLocal());
	iTimer.After(iStatus, aWaitToStart);
	iObserver = &aStatus;
	SetActive();
	}


