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
//

#include <f32file.h>
#include <watcher.h>
#include "testwatcher.h"

#define KTestDelay 5000000
_LIT(KTestFile, "c:\\ECOMTESTWATCHER");

//**********************************
// Globals
//**********************************


//**********************************
// CTestWatcher
//**********************************

// static

#include <ecom/implementationproxy.h>

const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(0x10008D9D, CTestWatcher::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}

CTestWatcher* CTestWatcher::NewL(TAny* aWatcherParams)
	{
	TWatcherParams* params = reinterpret_cast<TWatcherParams*>(aWatcherParams);
	CTestWatcher* self = new (ELeave) CTestWatcher(EPriorityStandard, params->iFs, params->iLog);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CTestWatcher::CTestWatcher(TInt aPriority, RFs& aFs, CWatcherLog& aLog)
: CTimer(aPriority), iLog(aLog), iFs(aFs)
	{
	CActiveScheduler::Add(this);
	}

CTestWatcher::~CTestWatcher()
	{
	Cancel();
	}

void CTestWatcher::ConstructL()
	{
	TRAPD(err, CTimer::ConstructL(); After(KTestDelay));
	RDebug::Print(_L("Constructed test watcher with error %d"), err);
	User::LeaveIfError(err);
	}

void CTestWatcher::RunL()
	{
	iLog.Printf(_L("EcomTestWatcher: Creating test file"));

	RFile file;
	TInt err = file.Replace(iFs, KTestFile, EFileShareExclusive);

	if (err)
		err = file.Create(iFs, KTestFile, EFileShareExclusive);

	if (!err)
		{
		file.Write(_L8("Hello\n"));
		file.Flush();
		file.Close();
		}

	After(KTestDelay);
	}

