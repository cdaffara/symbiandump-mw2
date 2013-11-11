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

#if !defined(__GPRS_BEARER_DATA_H__)
#define __GPRS_BEARER_DATA_H__

#include <e32base.h>
#include <e32std.h>

class CIpAddress;

class CWapGprsBearerData:public CBase
/**
@internalComponent
@released
*/
	{
	public:
	
		static	CWapGprsBearerData* NewL();
		static	CWapGprsBearerData* NewLC();
		~CWapGprsBearerData();
		
	public:
		
		// Read from XML message (must be present in it)
		TPtrC	iAccessPointName;
	
		// Values for these two must be invented
		// they are mandatory but will not be in the XML message
		TUint32 iPDPType;
		HBufC*	iPDPAddress;
		
		// These two are set to "PPP" and ETrue
		// they are mandatory but are not in the XML message or the default table
		HBufC*	iIFName;
		TBool	iIFPromptForAuth;

		// Read from XML message
		// Not mandatory
		TPtrC	iLoginName;
		TPtrC	iLoginPass;
		CIpAddress* iGatewayAddr;

		// Mandatory, but not in the XML message or the default table
		// set to ETrue
		TBool	iIPDnsAddrFromServer;
		TBool	iPlainTextAuthDisabled;
		TBool   iIpAddrFromServer;
		TUint	iGprsCommDbRecordId;
		
	private:
		void ConstructL();
		CWapGprsBearerData(){}
	};

#endif
