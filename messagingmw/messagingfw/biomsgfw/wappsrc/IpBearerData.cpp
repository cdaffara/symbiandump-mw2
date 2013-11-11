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

#include "IpBearerData.h"
#include <ipaddr.h>

CWapIpBearerData*  CWapIpBearerData::NewLC()
	{	
	CWapIpBearerData*  self = new (ELeave) CWapIpBearerData;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CWapIpBearerData*  CWapIpBearerData::NewL()
	{
	CWapIpBearerData*  self;
	self = CWapIpBearerData::NewLC();
	CleanupStack::Pop();
	return self;
	}

void CWapIpBearerData::ConstructL()
	{
	iIapId = 0;
	iGatewayAddr = CIpAddress::NewL();
	}

CWapIpBearerData::~CWapIpBearerData()
	{
	delete iGatewayAddr;
	}
