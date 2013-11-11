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

#include "ctestactivewatcher.h"

#include <implementationproxy.h>
#include <watcher.h>

const TInt KDelay = 10000000;

const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(0x10008D9D,	CTestActiveWatcher::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}

CTestActiveWatcher* CTestActiveWatcher::NewL(TAny* aWatcherParams)
	{
	CActiveWatcher::TWatcherParams* params = reinterpret_cast<CActiveWatcher::TWatcherParams*>(aWatcherParams);
	CTestActiveWatcher* self = new (ELeave)	CTestActiveWatcher(params->iFs, params->iLog);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CTestActiveWatcher::CTestActiveWatcher(RFs& aFs, CWatcherLog& aLog)
: CActiveWatcher(CActive::EPriorityStandard, aFs, aLog)
	{
	CActiveScheduler::Add(this);
	}
	
void CTestActiveWatcher::ConstructL()
	{
	User::LeaveIfError(iTimer.CreateLocal());

	iTimer.After(iStatus, KDelay);
	SetActive();
	}
	
CTestActiveWatcher::~CTestActiveWatcher()
	{
	Cancel();	
	}
	
void CTestActiveWatcher::RunL()
	{
	iLog.Printf(_L("Watcher TestWatcher starting..."));

	User::Invariant();
	}
	
void CTestActiveWatcher::DoCancel()
	{
	iTimer.Cancel();
	}
	
