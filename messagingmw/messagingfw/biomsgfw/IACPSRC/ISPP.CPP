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

#include "BSP.H"
#include "IACP.H"
#include "ISPP.H"
#include "IACPDEF.H"
#include "IACPERR.H"

_LIT(KIacpProxyProtocolName, "HTTP");
_LIT(KCharOne,"1");
//
// Constructor
//
CIspParser::CIspParser(TInt aSmsType)
:iSmsType(aSmsType)
	{
	__ASSERT_DEBUG((iSmsType==CIacSettingsParser::EBasicIAP ||iSmsType==CIacSettingsParser::EBasicMailIAP ||
		iSmsType==CIacSettingsParser::EExtendedIAP),
					User::Panic(KIACP, KIacpUnknownSmsType));
	}

//
// Factory fns
//
CIspParser* CIspParser::NewLC(TInt aSmsType)
	{
	CIspParser* self=new (ELeave) CIspParser(aSmsType);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CIspParser* CIspParser::NewL(TInt aSmsType)
	{
	CIspParser* self=CIspParser::NewLC(aSmsType);
	CleanupStack::Pop();
	return self;
	}

//
// 2nd stage of construction
//
void CIspParser::ConstructL()
	{
	iLoginPrompt=ETrue;		//Mandatory Data
	iSecureProxy=EFalse;
	iDbSession = CMDBSession::NewL(CMDBSession::LatestVersion());					

	iAddr = CIpAddress::NewL();
	iDNS1 = CIpAddress::NewL();
	iDNS2 = CIpAddress::NewL();
	iNetMask = CIpAddress::NewL();
	iGateway = CIpAddress::NewL();

	iProxyExeptions = CIpAddress::NewL();
	iProxyServerName = CIpAddress::NewL();
	iSecureProxySrv = CIpAddress::NewL();
	}

//
// Destruction
//
CIspParser::~CIspParser()
	{
	delete iName;
	delete iDefaultTelNum;
	delete iLoginName;
	delete iLoginPass;
	delete iAddr;
	delete iNetMask;
	delete iGateway;
	delete iDNS1;
	delete iDNS2;
	delete iModemInit;

	//Extended IAP data (Proxies Table)
	delete iProxyExeptions;
	delete iProxyServerName;
	delete iSecureProxySrv;
	delete iLoginCustomisation;
	
	delete iDbSession;
	}


void CIspParser::CheckMandatoryFieldsL(CParsedFieldCollection& aIacpFields)
	{
	if (aIacpFields.GetFieldValue(SMS_ISP_I_NAME).Length()==0 )
		User::Leave(KIacpMandatoryDataNotSet);
	}


//
// 
//
void CIspParser::ParseL(CParsedFieldCollection& aIacpFields)
	{
	TPtrC aFieldValueBuf;

	if (aIacpFields.GetFieldValueAndLength(SMS_ISP_I_NAME, aFieldValueBuf) != 0)
		{
		delete iName;
		iName = NULL;
		iName = aFieldValueBuf.AllocL();
		}

	if (aIacpFields.GetFieldValueAndLength(SMS_DEFAULT_TEL_NUM, aFieldValueBuf) != 0)
		{
		delete iDefaultTelNum;
		iDefaultTelNum = NULL;
		iDefaultTelNum= aFieldValueBuf.AllocL();
		}

	if (aIacpFields.GetFieldValueAndLength(SMS_LOGIN_NAME, aFieldValueBuf) != 0)
		{
		delete iLoginName;
		iLoginName = NULL;
		iLoginName= aFieldValueBuf.AllocL();
		}

	if (aIacpFields.FieldNameExists(SMS_LOGIN_PASS))
		{
		//should recheck for -	if (aIacpFields.GetFieldValueAndLength(SMS_LOGIN_PASS, aFieldValueBuf) != 0)
		//to set it if it exists if it doesn't it needs resetting
		if (aIacpFields.GetFieldValueAndLength(SMS_LOGIN_PASS, aFieldValueBuf))
			{
			delete iLoginPass;
			iLoginPass = NULL;
			iLoginPass= aFieldValueBuf.AllocL();		
			}		
		}

	if (aIacpFields.GetFieldValueAndLength(SMS_PROMPT_FOR_LOGIN, aFieldValueBuf) != 0)
		aIacpFields.GetTBoolL(aFieldValueBuf,iLoginPrompt);
	else if((iLoginName) && (iLoginPass) ) 
		iLoginPrompt=EFalse; // if both the login Name and password are specified then this should be set to EFalse

	if (aIacpFields.GetFieldValueAndLength(SMS_IP_ADDR, aFieldValueBuf) != 0)
		{
		iAddr->SetAddrL(aFieldValueBuf);
		}

	if (aIacpFields.GetFieldValueAndLength(SMS_IP_NETMASK, aFieldValueBuf) != 0)
		{
		iNetMask->SetAddrL(aFieldValueBuf);
		}

	if (aIacpFields.GetFieldValueAndLength(SMS_IP_GATEWAY, aFieldValueBuf) != 0)
		{
		iGateway->SetAddrL(aFieldValueBuf);
		}

	if (aIacpFields.GetFieldValueAndLength(SMS_IP_NAME_SERVER1, aFieldValueBuf) != 0)
		{
		iDNS1->SetAddrL(aFieldValueBuf);
		}

	if (aIacpFields.GetFieldValueAndLength(SMS_IP_NAME_SERVER2, aFieldValueBuf) != 0)	
		{
		iDNS2->SetAddrL(aFieldValueBuf);
		}

	if (aIacpFields.GetFieldValueAndLength(SMS_INIT_STRING, aFieldValueBuf) != 0)
		{
		delete iModemInit;
		iModemInit = NULL;
		iModemInit = HBufC8::NewL(aFieldValueBuf.Length());
		iModemInit->Des().Copy(aFieldValueBuf);
		}

	//Extended IAP
	if (aIacpFields.GetFieldValueAndLength(SMS_PROXY_EXCEPTIONS, aFieldValueBuf) != 0)
		{
		iProxyExeptions->SetAddrL(aFieldValueBuf);
		}

	if (aIacpFields.GetFieldValueAndLength(SMS_PROXY_PORT_NUMBER, aFieldValueBuf) != 0)
		aIacpFields.GetTUint32NumL(aFieldValueBuf,iProxyPortNum);

	if (aIacpFields.GetFieldValueAndLength(SMS_PROXY_SERVER_NAME, aFieldValueBuf) != 0)
		{
		iProxyServerName->SetAddrL(aFieldValueBuf);
		iSecureProxy=EFalse;
		}

	if (aIacpFields.GetFieldValueAndLength(SMS_LOGIN_CUSTOMISATION, aFieldValueBuf) != 0)
		{
		delete iLoginCustomisation;
		iLoginCustomisation = NULL;
		iLoginCustomisation= aFieldValueBuf.AllocL();
		}

	if (aIacpFields.GetFieldValueAndLength(SMS_ENABLE_SW_COMP, aFieldValueBuf) != 0)
		aIacpFields.GetTBoolL(aFieldValueBuf, iSWCompressionEnabled);  //goes to ISP -Dial out Table fields

	if (aIacpFields.GetFieldValueAndLength(SMS_SECURE_PROXY, aFieldValueBuf) != 0)
		{
		iSecureProxySrv->SetAddrL(aFieldValueBuf);
		iSecureProxy=ETrue;
		}

	if (aIacpFields.GetFieldValueAndLength(SMS_SECURE_PORT, aFieldValueBuf) != 0)
		aIacpFields.GetTUint32NumL(aFieldValueBuf,iSecurePortNum);
	}
//
//
//
void CIspParser::ProcessL(CIacSettingsParser::TIacpConnectionPreferenceFlag& aConnectionPreference)
	{
	iNoExistingIpAddr = ETrue;
	iNoExistingDNS	  = ETrue;
	WriteToDialOutISPTableL();
	SetIapIdsL();	
	if(iSmsType== CIacSettingsParser::EExtendedIAP)
		{
		WriteToProxiesTableL();	
		}
	WriteToDialOutIAPTableL();
	SetGlobalIapL(TPtrC(KCDTypeNameIAP), iRecordId);
	iDialOutIap = iRecordId;
	
	if(aConnectionPreference == CIacSettingsParser::EIacpAttempCreateAsFirstRank ||
		aConnectionPreference == CIacSettingsParser::EIacpAttempCreateAsSecondRank)
		{
		WriteToPreferencesTableL(ECommDbConnectionDirectionOutgoing,aConnectionPreference);
		}			
	}
void CIspParser::SetGlobalIapL(const TDesC& aTable, TUint32 aIAP)
	{
	CCDConnectionPrefsRecord* connectionPrefsRecord = static_cast<CCDConnectionPrefsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdConnectionPrefsRecord));	
	CleanupStack::PushL(connectionPrefsRecord);
	connectionPrefsRecord->iDirection = ECommDbConnectionDirectionOutgoing;
	connectionPrefsRecord->iRanking = 1;	//rank 1 is like a default value
	TBool recExists = connectionPrefsRecord->FindL(*iDbSession);
	if (recExists)
		{
		TInt intValue = 	connectionPrefsRecord->iDefaultIAP;
		if(aTable.CompareF(TPtrC(KCDTypeNameIAP)) && intValue != aIAP)
			{
			CCDIAPRecord* iapRecord = static_cast<CCDIAPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
			CleanupStack::PushL(iapRecord);
			iapRecord->iRecordTag = intValue;
			recExists = iapRecord->FindL(*iDbSession);			
			CleanupStack::PopAndDestroy(iapRecord);		
			}
		// If there are no IAP record
		if(!recExists)
			{
			connectionPrefsRecord->iDefaultIAP = aIAP;	
			connectionPrefsRecord->ModifyL(*iDbSession);
			}		
		}
	// If there are no ConnPref 
	else
		{
		connectionPrefsRecord->iDefaultIAP = aIAP;	
		connectionPrefsRecord->StoreL(*iDbSession);
		}	
	CleanupStack::PopAndDestroy(connectionPrefsRecord);

	}

//
// Id of IAP record entry
//
TUint32	CIspParser::IapEntryId()
	{
	return iDialOutIap;
	}
//
//
//
void CIspParser::SetIapIdsL()
	{
	TUint32 id=0;
	GetIapIdL(TPtrC(KCDTypeNameDialOutISP), id);
	iIapIsp=id;
	iProxyIsp=id;
	}

//
// Get COMMDB_ID from table that matches the COMMDB_NAME extracted from BioMsg
//
void CIspParser::GetIapIdL(const TDesC& aTableName, TUint32& aId)
	{
  	const TPtrC KNameFieldToken(iName->Des());
	if (aTableName.CompareF(TPtrC(KCDTypeNameDialOutISP)) == KErrNone)
		{
		CCDDialOutISPRecord* dialOutIspRecord = static_cast<CCDDialOutISPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdDialOutISPRecord));
		CleanupStack::PushL(dialOutIspRecord);
		dialOutIspRecord->iRecordName.SetMaxLengthL(KNameFieldToken.Length());
		dialOutIspRecord->iRecordName =  KNameFieldToken;
		TBool err = dialOutIspRecord->FindL(*iDbSession);	
		if (err)
			{		
			aId = dialOutIspRecord->iRecordTag.RecordId();
			}
		else
			{
			User::Leave(err);
			}
		CleanupStack::PopAndDestroy(dialOutIspRecord);		
		}
	}
	

void CIspParser::WriteToDialOutISPTableL()
	{
	CCDDialOutISPRecord* dialOutIspRecord = static_cast<CCDDialOutISPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdDialOutISPRecord));
	CleanupStack::PushL(dialOutIspRecord);
	(dialOutIspRecord->iRecordName).SetMaxLengthL(iName->Length());	
	dialOutIspRecord->iRecordName = *iName;
	TBool recordExist = FALSE;
	if(!(dialOutIspRecord->FindL(*iDbSession)))
		{
		dialOutIspRecord->SetRecordId(KCDNewRecordRequest);		
		}
	else
		{
		recordExist = TRUE;
		}
	dialOutIspRecord->iDialResolution = ETrue;
	if(iDefaultTelNum)
		{
		(dialOutIspRecord->iDefaultTelNum).SetMaxLengthL(iDefaultTelNum->Length());
		dialOutIspRecord->iDefaultTelNum = LimitStringSize(iDefaultTelNum->Des());
		}
	dialOutIspRecord->iPromptForLogin = iLoginPrompt;

	TBool manualCustomisation = EFalse;
	if(iLoginCustomisation)
		{
		// If Ilgn field is found in the BIO message check whether
		// it is '1', in which case place TRUE into the CommsDat field.
		// Currently it is not possible to handle a script name being
		// specified in the Ilgn field of the BIO message because CommsDat
		// will only accept a boolean value for ISP_USE_LOGIN_SCRIPT.
		// This may need to be changed in the future depending on requirements.

		manualCustomisation = (iLoginCustomisation->Compare(KCharOne) == 0);
		}

	dialOutIspRecord->iUseLoginScript = manualCustomisation;
	if (iLoginName)
		{
		(dialOutIspRecord->iLoginName).SetMaxLengthL(iLoginName->Length());
		dialOutIspRecord->iLoginName = LimitStringSize(iLoginName->Des());
		(dialOutIspRecord->iIfAuthName).SetMaxLengthL(iLoginName->Length());
		dialOutIspRecord->iIfAuthName = LimitStringSize(iLoginName->Des());	
		}
	if(iLoginPass)
		{
		(dialOutIspRecord->iLoginPass).SetMaxLengthL(iLoginPass->Length());
		dialOutIspRecord->iLoginPass = LimitStringSize(iLoginPass->Des());
		(dialOutIspRecord->iIfAuthPass).SetMaxLengthL(iLoginPass->Length());
		dialOutIspRecord->iIfAuthPass = LimitStringSize(iLoginPass->Des());		
		}
		
	dialOutIspRecord->iDisplayPct = EFalse;
	dialOutIspRecord->iIfPromptForAuth = EFalse;
	dialOutIspRecord->iIfCallbackEnabled = EFalse;
	(dialOutIspRecord->iIpAddr).SetMaxLengthL(KCommsDbSvrMaxFieldLength);
	dialOutIspRecord->iIpAddr = iAddr->Addr();
	dialOutIspRecord->iIpAddrFromServer = EFalse;
	dialOutIspRecord->iIpAddrFromServer = ETrue;
	if(iNetMask)
		{
		(dialOutIspRecord->iIpNetMask).SetMaxLengthL(KCommsDbSvrMaxFieldLength);
		dialOutIspRecord->iIpNetMask = LimitStringSize(iNetMask->Addr());
		}
	if(iGateway)
		{
		(dialOutIspRecord->iIpGateway).SetMaxLengthL(KCommsDbSvrMaxFieldLength);
		dialOutIspRecord->iIpGateway = LimitStringSize(iGateway->Addr());
		}
	TBool dns1Specified = (iDNS1->Addr().Compare(KNullDesC) != 0);
	TBool dns2Specified = (iDNS2->Addr().Compare(KNullDesC) != 0);
	if(dns1Specified)
		{
		(dialOutIspRecord->iIpNameServer1).SetMaxLengthL(KCommsDbSvrMaxFieldLength);
		dialOutIspRecord->iIpNameServer1 = iDNS1->Addr();
		}
	if(dns2Specified)
		{
		(dialOutIspRecord->iIpNameServer2).SetMaxLengthL(KCommsDbSvrMaxFieldLength);
		dialOutIspRecord->iIpNameServer2 = LimitStringSize(iDNS2->Addr());	
		}	
	if(dns1Specified || dns2Specified || !iNoExistingDNS)
		{
		dialOutIspRecord->iIpDnsAddrFromServer = EFalse;	
		}		
	else
		{
		dialOutIspRecord->iIpDnsAddrFromServer = ETrue;	
		}		
	dialOutIspRecord->iEnableIpHeaderComp = EFalse;
	dialOutIspRecord->iDisablePlainTextAuth = EFalse;
	dialOutIspRecord->iEnableSwComp = iSWCompressionEnabled;
	dialOutIspRecord->iIfNetworks = KIacpIfNetworks;
	if(iModemInit)
		{
		(dialOutIspRecord->iInitString).SetMaxLengthL(KCommsDbSvrMaxFieldLength);
		dialOutIspRecord->iInitString = iModemInit->Des();	
		}
	if(iIapChargeCard)
		{
		dialOutIspRecord->iChargecard = iIapChargeCard;	
		}
		
	if (!recordExist)
		{
		dialOutIspRecord->StoreL(*iDbSession);
		}
	else 
		{
		dialOutIspRecord->ModifyL(*iDbSession);	
		}
	CleanupStack::PopAndDestroy(dialOutIspRecord);
	}

void CIspParser::WriteToDialOutIAPTableL()
	{
	CCDIAPRecord* iapRecord = static_cast<CCDIAPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
	CleanupStack::PushL(iapRecord);

	(iapRecord->iRecordName).SetMaxLengthL(iName->Length());	
	iapRecord->iRecordName = *iName;
	TBool recordExist = FALSE;
	if(!(iapRecord->FindL(*iDbSession)))
		{
		iapRecord->SetRecordId(KCDNewRecordRequest);		
		}
	else
		{
		recordExist = TRUE;
		}
	iapRecord->iServiceType.SetMaxLengthL(((TPtrC)KCDTypeNameDialOutISP).Length());
	iapRecord->iServiceType = TPtrC(KCDTypeNameDialOutISP);
	iapRecord->iService =  iIapIsp; 
	iapRecord->iBearerType.SetMaxLengthL(((TPtrC)KCDTypeNameModemBearer).Length());
	iapRecord->iBearerType = TPtrC(KCDTypeNameModemBearer);
	//there are 7 fields in IAP table which cannot have null values, and so when creating an IAP record we were only filling in one of the fields which meant we were submitting a record with lots of null values which can't be handled.
	iapRecord->iBearer = 2;
	iapRecord->iNetwork = 1;
	iapRecord->iNetworkWeighting = 0;
	iapRecord->iLocation = 4;
	if (!recordExist)
		{
		iapRecord->StoreL(*iDbSession);
		}
	else 
		{
		iapRecord->ModifyL(*iDbSession);	
		}
	iRecordId = iapRecord->iRecordTag.RecordId();	
	CleanupStack::PopAndDestroy(iapRecord);
	}
void CIspParser::WriteToPreferencesTableL(TCommDbConnectionDirection aDirection,CIacSettingsParser::TIacpConnectionPreferenceFlag& aConnectionPreference)
	{
	// create a new connection prefs record
	CCDConnectionPrefsRecord* connectionPrefsRecord = static_cast<CCDConnectionPrefsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdConnectionPrefsRecord));
	CleanupStack::PushL(connectionPrefsRecord);
	connectionPrefsRecord->iRanking = aConnectionPreference;
	connectionPrefsRecord->iDirection = aDirection;
	connectionPrefsRecord->iDialogPref = ECommDbDialogPrefPrompt;
	connectionPrefsRecord->iBearerSet  = ECommDbBearerWcdma;
	connectionPrefsRecord->SetRecordId(KCDNewRecordRequest);
	if(!(connectionPrefsRecord->FindL(*iDbSession)))
		{
		connectionPrefsRecord->StoreL(*iDbSession);		
		}
	else
		{
		aConnectionPreference = CIacSettingsParser::EIacpDoNotCreatePreference;
		}
	CleanupStack::PopAndDestroy(connectionPrefsRecord);	
	}
void CIspParser::WriteToProxiesTableL()
	{
	CCDProxiesRecord *proxiesRecord = static_cast<CCDProxiesRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdProxiesRecord));
	CleanupStack::PushL(proxiesRecord);
	proxiesRecord->iService = iProxyIsp;
	TBool recordExist = FALSE;
	if(!(proxiesRecord->FindL(*iDbSession)))
		{
		proxiesRecord->SetRecordId(KCDNewRecordRequest);		
		}
	else
		{
		recordExist = TRUE;
		}
	if(iSecureProxy)
		{
		if(iSecureProxySrv)
			{
			(proxiesRecord->iServerName).SetMaxLengthL(KCommsDbSvrMaxFieldLength);
			proxiesRecord->iServerName = iSecureProxySrv->Addr();		
			}
		if(iSecurePortNum)
			{
			proxiesRecord->iPortNumber = iSecurePortNum;		
			}
		(proxiesRecord->iProtocolName).SetMaxLengthL(KCommsDbSvrMaxFieldLength);		
		proxiesRecord->iProtocolName = KIacpProxyProtocolName;
		
		}
	else
		{
		if(iProxyServerName)
			{
			(proxiesRecord->iServerName).SetMaxLengthL(KCommsDbSvrMaxFieldLength);
			proxiesRecord->iServerName = iProxyServerName->Addr();		
			}
		proxiesRecord->iPortNumber = iProxyPortNum;
		proxiesRecord->iProtocolName = KIacpProxyProtocolName;
		}

	if(iProxyExeptions)
		{
		(proxiesRecord->iExceptions).SetMaxLengthL(KCommsDbSvrMaxFieldLength);
		proxiesRecord->iExceptions = iProxyExeptions->Addr();		
		}		
	proxiesRecord->iUseProxyServer = EFalse;

	if (!recordExist)
		{
		proxiesRecord->StoreL(*iDbSession);
		}
	else
		{
		proxiesRecord->ModifyL(*iDbSession);	
		}
	CleanupStack::PopAndDestroy(proxiesRecord);	
	}


TPtrC CIspParser::LimitStringSize(const TDesC& aString)
	{
	if (aString.Length() < KCommsDbSvrMaxFieldLength)
		return aString;
	else
		return aString.Left(KCommsDbSvrMaxFieldLength);
	}

TPtrC8 CIspParser::LimitStringSize(const TDesC8& aString)
	{
	if (aString.Length() < KCommsDbSvrMaxFieldLength)
		return aString;
	else
		return aString.Left(KCommsDbSvrMaxFieldLength);
	}
