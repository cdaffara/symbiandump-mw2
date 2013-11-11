// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Internet Service Provider Settings
// 
//

#ifndef __ISPP_H__
#define __ISPP_H__
#include <cdbcols.h>
#include <ipaddr.h>

#include <commsdat.h>
#include <commsdattypesv1_1.h>
using namespace CommsDat;
#include <cdblen.h> 

class CIspParser : public CBase
/**
@internalComponent
@released
*/
	{
public:
	static CIspParser* NewLC(TInt aSmsType);
	static CIspParser* NewL(TInt aSmsType);
	~CIspParser();

	void CheckMandatoryFieldsL(CParsedFieldCollection& aIacpFields);

	void ParseL(CParsedFieldCollection& aIacpFields);
	void ProcessL(CIacSettingsParser::TIacpConnectionPreferenceFlag& aConnectionPreference);
	TUint32	IapEntryId();

private:
	CIspParser(TInt aSmsType);
	void ConstructL();
	
	void WriteToDialOutISPTableL();
	void WriteToDialOutIAPTableL();
	void WriteToPreferencesTableL(TCommDbConnectionDirection aDirection,CIacSettingsParser::TIacpConnectionPreferenceFlag& aConnectionPreference);
	void WriteToProxiesTableL();

	void SetIapIdsL();
	void GetIapIdL(const TDesC& aTableName, TUint32& aId);
	TPtrC LimitStringSize(const TDesC& aString);
	TPtrC8 LimitStringSize(const TDesC8& aString);

	void SetGlobalIapL(const TDesC& aTable, TUint32 aIAP);

private:
	TInt	iSmsType; //Basic or Extended IAP
	//Global Setting data
	TUint32 iIapIsp;
	TUint32 iProxyIsp;
	TUint32 iIapModem;
	TUint32 iIapLocation;
	TUint32 iIapChargeCard;

	//Basic IAP data (Dial Out ISP)
	HBufC*	iName;
	HBufC*  iDefaultTelNum;
	HBufC*  iLoginName;
	TBool	iLoginPrompt;
	HBufC*  iLoginPass;
	HBufC8*	iModemInit;
	CIpAddress*  iAddr;
	CIpAddress*  iDNS1;
	CIpAddress*  iDNS2;
	CIpAddress*  iNetMask;
	CIpAddress*  iGateway;

	//Extended IAP data (Proxies Table)
	CIpAddress*	iProxyExeptions;	//SMS_PROXY_EXCEPTIONS
	CIpAddress*	iProxyServerName;	//SMS_PROXY_SERVER_NAME
	TUint32	iProxyPortNum;		//SMS_PROXY_PORT_NUMBER
	CIpAddress*	iSecureProxySrv;	//SMS_SECURE_PROXY
	TUint32	iSecurePortNum;	//SMS_SECURE_PORT
	TBool	iSecureProxy;
	HBufC*	iLoginCustomisation;	//SMS_LOGIN_CUSTOMISATION 
	TBool   iSWCompressionEnabled; //SMS_ENABLE_SW_COMP //PPP compression "Y"|"N"

	TBool	iNoExistingIpAddr;
	TBool	iNoExistingDNS;

	TUint32	iDialOutIap;

	CMDBSession*		iDbSession;
	TUint32 iRecordId;

	};
#endif  
