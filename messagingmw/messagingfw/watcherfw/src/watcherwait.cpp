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
// watcherwait.cpp
//

#include <watcher.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include "cwatcher.h"
#endif

EXPORT_C CWatcherWait* CWatcherWait::NewLC(TInt aPriority)
	{
	CWatcherWait* self = new(ELeave)CWatcherWait(aPriority);
	CleanupStack::PushL(self);
	return self;
	}

CWatcherWait::CWatcherWait(TInt aPriority)
: CActive(aPriority)
	{
	CActiveScheduler::Add(this);
	}

EXPORT_C CWatcherWait::~CWatcherWait()
	{
	Cancel();
	}

EXPORT_C void CWatcherWait::Start()
	{
	iStatus = KRequestPending;
	SetActive();
	}

void CWatcherWait::RunL()
	{
	CActiveScheduler::Stop();
	}

void CWatcherWait::DoCancel()
	{
	TRequestStatus* s=&iStatus;
	User::RequestComplete(s, KErrCancel);
	}
