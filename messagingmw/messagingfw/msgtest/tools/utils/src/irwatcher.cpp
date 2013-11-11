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

#include <watcher.h>
#include <e32property.h>
#include "msvtestutils.h"
#include <ir_sock.h>
#include "irwatcher.h"


//**********************************
// Globals
//**********************************



#include <ecom/implementationproxy.h>

const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(0x10008C68, CIrWatcher::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}

//**********************************
// CIrWatcher
//**********************************

// static 
CIrWatcher* CIrWatcher::NewL(TAny* aWatcherParams)
	{
	TWatcherParams* params = reinterpret_cast<TWatcherParams*>(aWatcherParams);
	CIrWatcher* self = new (ELeave) CIrWatcher(EPriorityStandard, params->iLog);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CIrWatcher::CIrWatcher(TInt aPriority, CWatcherLog& aLog)
	: CActive(aPriority), iLog(aLog)
	{
	CActiveScheduler::Add(this);
	}

CIrWatcher::~CIrWatcher()
	{
	iProperty.Close();
	Cancel();
	}

void CIrWatcher::ConstructL()
	{
	User::LeaveIfError(iProperty.Attach(KIrdaPropertyCategory, KIrdaStatus));
	iProperty.Subscribe(iStatus);
	// Start the watcher
	iLog.Printf(_L("IrWatcher: Starting watcher"));
	SetActive();
	}

void CIrWatcher::DisplayState(TInt aState)
	{
	switch(aState)
		{
		
		case TIrdaStatusCodes::EIrLoaded:
			iLog.Printf(_L("IrWatcher: Loaded"));
			break;

		case TIrdaStatusCodes::EIrDiscoveredPeer:
			iLog.Printf(_L("IrWatcher: Start Discovery"));
			break;

		case TIrdaStatusCodes::EIrLostPeer:
			iLog.Printf(_L("IrWatcher: End Discovery"));
			break;

		case TIrdaStatusCodes::EIrConnected:
			iLog.Printf(_L("IrWatcher: Connected"));
			break;

		case TIrdaStatusCodes::EIrBlocked:
			iLog.Printf(_L("IrWatcher: Blocked"));
			break;

		case TIrdaStatusCodes::EIrDisconnected:
			iLog.Printf(_L("IrWatcher: Disconnected"));
			break;

		case TIrdaStatusCodes::EIrUnloaded:
			iLog.Printf(_L("IrWatcher: Unloaded"));
			break;

		default:
			iLog.Printf(_L("IrWatcher: Unknown"));
			break;
		};
	}

void CIrWatcher::RunL()
	{
	TInt state;
	iProperty.Subscribe(iStatus);
	iProperty.Get(state);
	DisplayState(state);
 	SetActive();
	}

void CIrWatcher::DoCancel()
	{
	iLog.Printf(_L("IrWatcher: DoCancel"));
	iProperty.Cancel();
	}


