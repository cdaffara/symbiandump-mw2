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

#if !defined(__ISP_TABLE_DATA_H__)
#define __ISP_TABLE_DATA_H__

#include <e32base.h>
#include <e32std.h>
#include <etelmm.h>

class CIpAddress;

// Mapping object for an ISP Dial Out record - acts as as glorified struct.
class CIspTableData: public CBase
/**
@internalComponent
@released
*/
	{
	public:
		TInt	iWapEntryID;  // Id of partner WAP Table object in array of WAP table objects
		TPtrC	iISPDescription;			//ISP_DESCRIPTION
		TUint	iISPType;					//ISP_TYPE
		TPtrC	iDefaultPhoneNum;			//ISP_DEFAULT_TEL_NUM
		TBool	iDialResolution;			//ISP_DIAL_RESOLUTION
		TBool	iUseScript;					//ISP_USE_LOGIN_SCRIPT
		TPtrC	iLoginScript;
		TBool	iPromptForLoginDetails;		//ISP_PROMPT_FOR_LOGIN
		TPtrC	iLoginName;					//ISP_LOGIN_NAME
		TPtrC	iLoginPass;					//ISP_LOGIN_PASS
		TBool	iDisplayPCT;				//ISP_DISPLAY_PCT
		HBufC*	iIFName;					//ISP_IF_NAME
		TPtrC	iIFParams;					//ISP_IF_PARAMS
		HBufC*	iIFNetworks;				//ISP_IF_NETWORKS
		TBool	iIFPromptForAuthentication; //ISP_IF_PROMPT_FOR_AUTHENTICATION
		TUint	iNumAuthAttempts;			//ISP_AUTH_RETRIES
		TBool	iCallBackEnabled;			//ISP_IF_CALLBACK_ENABLED
		TUint	iCallBackType;				//ISP_IF_CALLBACK_TYPE
		TPtrC	iCallBackInfo;				//ISP_IF_CALLBACK_INFO
		TBool	iAddrFromServer;			//ISP_IP_ADDR_FROM_SERVER
		CIpAddress*	iInOutAddress;				//ISP_IP_ADDR
		CIpAddress*	iNetmask;					//ISP_IP_NETMASK
		CIpAddress*	iGateway;					//ISP_IP_GATEWAY
		TBool	iDNSFromServer;				//ISP_IP_DNS_ADDR_FROM_SERVER
		CIpAddress*	iDNS1;						//ISP_IP_NAME_SERVER1
		CIpAddress*	iDNS2;						//ISP_IP_NAME_SERVER2
		TBool	iHeaderCompressionEnabled;  //ISP_ENABLE_IP_HEADER_COMP
		TBool	iEnableLCPExtensions;		//ISP_ENABLE_LCP_EXTENSIONS
		TBool	iPlainTextAuthDisabled;		//ISP_DISABLE_PLAIN_TEXT_AUTH
		TBool	iSwCompressionEnabled;		//ISP_ENABLE_SW_COMP
		//NB don't bother with ISP_BEARER_TYPE - no info in message to set this
		
		RMobileCall::TMobileCallDataSpeed iCallSpeed;	// Need both CSD_CALLSPEED & CSD_CALLTYPE to set ISP_BEARER_SPEED!!
		TInt	iISPCommDbRecordId;				// Id of the record in CommDb - used to set IAP_SERVICE field
		RMobileCall::TMobileCallDataProtocol iCallDataProtocol;


	public:
		static	CIspTableData* NewL();
		~CIspTableData();
	private:
		void ConstructL();	 // Initialise variables.
	};

#endif
