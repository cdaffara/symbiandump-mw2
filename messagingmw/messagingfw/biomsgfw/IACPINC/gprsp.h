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

#ifndef __GPRSP_H__
#define __GPRSP_H__

#include <commsdat.h>
#include <commsdattypesv1_1.h>
#include <cdblen.h>
using namespace CommsDat;

class CIpAddress;

class CGprsParser : public CBase
/**
@internalComponent
@released
*/
	{
public:
	static CGprsParser* NewLC();
	static CGprsParser* NewL();
	~CGprsParser();
	
	void CheckMandatoryFieldsL(CParsedFieldCollection& aIacpFields);

	void ParseL(CParsedFieldCollection& aIacpFields);
	void ProcessL(CIacSettingsParser::TIacpConnectionPreferenceFlag& aConnectionPreference);

private:

	enum TDirection{ EOutgoing, EIncoming };
	CGprsParser();
	void ConstructL();

	//commit functions
	void CreateProxiesTableL(const TDesC& aTableName);

	void WriteToGPRSTableL(TDirection aDir);
	void WriteToIAPTableL();

	void CreatePreferencesTableL(TCommDbConnectionDirection aDirection, CIacSettingsParser::TIacpConnectionPreferenceFlag& aConnectionPreference);	
	TUint32 GetIapIdL(const TDesC& aTableName);
	TPtrC LimitStringSize(const TDesC& aString);
	TPtrC8 LimitStringSize(const TDesC8& aString);

private:
	HBufC*							iName;
	HBufC*							iApn;
	TUint32							iPdpType;
	CIpAddress*						iPdpAddress;
	CIpAddress*						iIfName;
	HBufC*  						iLoginName;
	TBool							iLoginPrompt;
	HBufC*  						iLoginPass;
	CIpAddress* 					iAddr;
	TBool							iDnsFromServer;
	CIpAddress*						iDNS1;
	CIpAddress*						iDNS2;
	CIpAddress*						iNetMask;

	TBool 							iNoExistingIpAddr;
	TBool 							iNoExistingDNS;

	//commit data
	CMDBSession*					iDbSession;
	TUint32 						iRecordId;
	};
#endif  
