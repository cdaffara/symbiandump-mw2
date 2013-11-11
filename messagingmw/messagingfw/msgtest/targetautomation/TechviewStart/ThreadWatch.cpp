// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ThreadWatch.h"
#include "Starter.h"

/**
 * class CThreadWatcher
 **/
CThreadWatcher::~CThreadWatcher()
	{
	Cancel();
	}

CThreadWatcher::CThreadWatcher(TInt appType, TFullName aFullName, CStarter* aOwner, TUint32 aAppUid, TBool aViewless)
	: CActive(EPriorityStandard), iAppType(appType), iFullName(aFullName), iOwner(aOwner), iAppUid(aAppUid), iViewless(aViewless)
	{}
	
void CThreadWatcher::ConstructL(const TThreadId aThreadId)
	{
	iMonitoringThread = ETrue;
	User::LeaveIfError(iThread.Open(aThreadId)); // Queue logon request
	iThread.Logon(iStatus);
	CActiveScheduler::Add(this);
	SetActive(); // Tell scheduler we're active
	}

CThreadWatcher* CThreadWatcher::NewL(TInt appType, const TThreadId aThreadId, TFullName aFullName, CStarter* aOwner, TUint32 aAppUid, TBool aViewless)
	{
	CThreadWatcher* self = new (ELeave) CThreadWatcher(appType, aFullName, aOwner, aAppUid, aViewless);
	CleanupStack::PushL(self);
	self->ConstructL(aThreadId);
	CleanupStack::Pop(); // self;
	return self;
	}

void CThreadWatcher::DoCancel()

	{
	iThread.LogonCancel(iStatus);
	iThread.Close();
	}

void CThreadWatcher::RunL()
	{
	iThread.Close();
	if(iMonitoringThread) 
		{
		TRAPD(err, RestartThreadL()); //ignore error
		}
	}

void CThreadWatcher::RestartThreadL()
	{
	TThreadId threadId;
	iOwner->RestartMonitoredThreadL(iAppType, threadId, iFullName, iAppUid, iViewless);
	User::LeaveIfError(iThread.Open(threadId)); // Queue logon request
	iThread.Logon(iStatus);
	iStatus = KRequestPending;
	SetActive(); // Tell scheduler we're active
	}

