// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "GprsBearerData.h"
#include <wappdef.h>
#include <ipaddr.h>
#include <etelpckt.h>

_LIT(KNullIpAddress, "0.0.0.0");
_LIT(KPPP, "ppp");

CWapGprsBearerData*  CWapGprsBearerData::NewLC()
	{	
	CWapGprsBearerData*  self = new (ELeave) CWapGprsBearerData();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CWapGprsBearerData*  CWapGprsBearerData::NewL()
	{
	CWapGprsBearerData*  self;
	self = CWapGprsBearerData::NewLC();
	CleanupStack::Pop();
	return self;
	}

void CWapGprsBearerData::ConstructL()
	{
	// Mandatory fields

	iIFName = KPPP().AllocL();

	iPDPType = RPacketContext::EPdpTypeIPv4;
	iPDPAddress = KNullIpAddress().AllocL();
	iIPDnsAddrFromServer = ETrue;
	iPlainTextAuthDisabled = EFalse;

	iGatewayAddr = CIpAddress::NewL();
	iIpAddrFromServer = ETrue;
	}

CWapGprsBearerData::~CWapGprsBearerData()
	{
	delete iPDPAddress;
	delete iIFName;
	delete iGatewayAddr;
	}
