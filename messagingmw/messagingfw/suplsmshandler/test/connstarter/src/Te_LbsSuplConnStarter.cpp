// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// The SUPL Ethernet Connection Starter plug-in class. 
// 
//

#include <ecom/implementationproxy.h>

#include "Te_LbsSuplConnStarter.h"

const TImplementationProxy ImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY(0x10283762, CTe_LbsSuplConnStarter::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}


CTe_LbsSuplConnStarter* CTe_LbsSuplConnStarter::NewL(TAny* /*aWatcherParams*/)
	{
	CTe_LbsSuplConnStarter* self= new (ELeave) CTe_LbsSuplConnStarter();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}
	
CTe_LbsSuplConnStarter::~CTe_LbsSuplConnStarter()
	{
	Cancel();
	iConnection.Close();
	iSocketServ.Close();
	}
	
CTe_LbsSuplConnStarter::CTe_LbsSuplConnStarter() : CActive(EPriorityStandard), iState(EDisconnected)
	{
	CActiveScheduler::Add(this);
	}

void CTe_LbsSuplConnStarter::ConstructL()
	{
	User::LeaveIfError(iSocketServ.Connect());
	User::LeaveIfError(iConnection.Open(iSocketServ));
	
	StartConnection();
	}

void CTe_LbsSuplConnStarter::RunL()
	{
	__ASSERT_ALWAYS(iState==EConnecting, User::Invariant());
	__ASSERT_ALWAYS(iStatus==KErrNone, User::Invariant());
	iState = EConnected;
	}

void CTe_LbsSuplConnStarter::DoCancel()
	{
	//intentionally left blank here
	}

void CTe_LbsSuplConnStarter::StartConnection()
	{
	iConnection.Start(iStatus);
	SetActive();
	iState = EConnecting;
	}



