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

#include <e32base.h>
#include <e32std.h>

class CWapSmsBearerData:public CBase
/**
@internalComponent
@released
*/
	{
	public:
		static CWapSmsBearerData* NewL();
		static CWapSmsBearerData* NewLC();
	public:
		TPtrC	iGatewayAddr;	// WAP_GATEWAY_ADDRESS - SMS Proxy address 
		TPtrC	iWapServiceAddr;// WAP_SERVICE_CENTRE_ADDRESS  - SMS service address
		TPtrC	iPort;			// Used to set WAP_SECURITY & WAP_WSP_OPTION in  WAP_SMS_BEARER table
	private:
		void Construct();

	};
