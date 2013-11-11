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

#include "IspTableData.h"
#include <cdbcols.h>
#include <ipaddr.h>

_LIT(KPPP, "PPP");
_LIT(KIP, "ip,ip6");


CIspTableData* CIspTableData::NewL()
	{
	CIspTableData* self;
	self = new (ELeave) CIspTableData;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CIspTableData::ConstructL()
	{
	// Initialise the member variables

	iISPType=EIspTypeWAPOnly;
	iDialResolution=ETrue;			//Mandatory Data
	iUseScript=EFalse;				//Mandatory Data
	iPromptForLoginDetails=ETrue; 	//Mandatory Data
	iDisplayPCT=EFalse;
	
	iIFName = NULL;
	iIFName = KPPP().AllocL();		//Mandatory Data

	iIFNetworks=NULL;
	iIFNetworks = KIP().AllocL();


	iIFPromptForAuthentication =EFalse;	//Mandatory Data
	iNumAuthAttempts=10;
	iCallBackEnabled=EFalse;
	iAddrFromServer = ETrue;			//Mandatory Data
	iDNSFromServer  = ETrue;			//Mandatory Data
	iHeaderCompressionEnabled = EFalse; //Don't compress headers
	iPlainTextAuthDisabled = EFalse; 
	iSwCompressionEnabled = EFalse;	
	iCallSpeed = RMobileCall::ESpeedAutobauding;
	iCallDataProtocol = RMobileCall::EProtocolV34;

	iInOutAddress = CIpAddress::NewL();
	iNetmask = CIpAddress::NewL();
	iGateway = CIpAddress::NewL();
	
	iDNS1 = CIpAddress::NewL();
	iDNS2 = CIpAddress::NewL();
	}


CIspTableData::~CIspTableData()
	{
	delete iIFName;
	delete iIFNetworks;
	delete iInOutAddress;
	delete iNetmask;
	delete iGateway;
	delete iDNS1;
	delete iDNS2;
	}
