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

#if !defined(__IP_BEARER_DATA_H__)
#define __IP_BEARER_DATA_H__

#include <e32base.h>
#include <e32std.h>
#include <cdbcols.h>

class CIpAddress;

class CWapIpBearerData:public CBase
/**
@internalComponent
@released
*/
	{
	public:
		TUint				iIapId;		  // Entry in IAP DIALOUT table
 		CIpAddress*			iGatewayAddr; // WAP_GATEWAY_ADDRESS - IP Proxy address 
		TPtrC				iPort;		  // Used to set WAP_SECURITY & WAP_WSP_OPTION in WAP_IP_BEARER table
		TPtrC	iProxyAuthName;				// Moved from CIspTableData
		TPtrC	iProxyAuthSecret;			//  "	  "     "
		TBool   iProxyUseAutoLogin;

	public:
		static CWapIpBearerData* NewL();
		static CWapIpBearerData* NewLC();
		~CWapIpBearerData();
	private:
		void ConstructL();
	};

#endif
