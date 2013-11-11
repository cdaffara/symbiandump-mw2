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
#include "gprsp.h"
#include "IACPDEF.H"
#include "IACPERR.H"

//#include "t_tabdef.h"


#include <ipaddr.h>

_LIT(KGPRSDefaultIpAddress,		"0.0.0.0");

CGprsParser::CGprsParser()
	{
	}

CGprsParser* CGprsParser::NewLC()
	{
	CGprsParser* self=new (ELeave) CGprsParser();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CGprsParser* CGprsParser::NewL()
	{
	CGprsParser* self=CGprsParser::NewLC();
	CleanupStack::Pop();
	return self;
	}

void CGprsParser::ConstructL()
	{
	iLoginPrompt=ETrue;		//Mandatory Data
	iDnsFromServer= ETrue;

	iPdpAddress = CIpAddress::NewL(KGPRSDefaultIpAddress);

	iDbSession = CMDBSession::NewL(CMDBSession::LatestVersion());
	}

CGprsParser::~CGprsParser()
	{

	delete iName;
	delete iApn;
	delete iPdpAddress;
	delete iIfName;
	delete iLoginName;
	delete iLoginPass;
	delete iAddr;
	delete iNetMask;
	delete iDNS1;
	delete iDNS2;

	delete iDbSession;
	}


void CGprsParser::CheckMandatoryFieldsL(CParsedFieldCollection& aIacpFields)
	{
	if (aIacpFields.GetFieldValue(GPRSS_APN).Length()==0)
		User::Leave(KIacpMandatoryDataNotSet);
	}

void CGprsParser::ParseL(CParsedFieldCollection& aIacpFields)
	{
	TPtrC aFieldValueBuf;

	if (aIacpFields.GetFieldValueAndLength(GPRSS_NAME, aFieldValueBuf) != 0)
		{
		delete iName;
		iName = NULL;
		iName = aFieldValueBuf.AllocL();
		}

	if (aIacpFields.GetFieldValueAndLength(GPRSS_APN, aFieldValueBuf) != 0)
		{
		delete iApn;
		iApn = NULL;
		iApn= aFieldValueBuf.AllocL();
		}

	if (aIacpFields.GetFieldValueAndLength(GPRSS_PDP_TYPE, aFieldValueBuf) != 0)
		{
		aIacpFields.GetTUint32NumL(aFieldValueBuf,iPdpType);
		}

	if (aIacpFields.GetFieldValueAndLength(GPRSS_PDP_ADDRESS, aFieldValueBuf) != 0)
		{
		delete iPdpAddress;
		iPdpAddress = NULL;
		iPdpAddress = CIpAddress::NewL(aFieldValueBuf);
		}

	if (aIacpFields.GetFieldValueAndLength(GPRSS_IF_NAME, aFieldValueBuf) != 0)
		{
		delete iIfName;
		iIfName = NULL;
		iIfName= CIpAddress::NewL(aFieldValueBuf);
		}

	if (aIacpFields.GetFieldValueAndLength(GPRSS_LOGIN_NAME, aFieldValueBuf) != 0)
		{
		delete iLoginName;
		iLoginName = NULL;
		iLoginName= aFieldValueBuf.AllocL();
		}

	if (aIacpFields.GetFieldValueAndLength(GPRSS_LOGIN_PASS, aFieldValueBuf) != 0)
		{
		delete iLoginPass;
		iLoginPass = NULL;
		iLoginPass= aFieldValueBuf.AllocL();
		}

	if (aIacpFields.GetFieldValueAndLength(SMS_PROMPT_FOR_LOGIN, aFieldValueBuf) != 0)
		aIacpFields.GetTBoolL(aFieldValueBuf,iLoginPrompt);

	else if((iLoginName) || (iLoginPass) )
		iLoginPrompt=EFalse; // if login Name or password is specified then this should be set to EFalse

	if (aIacpFields.GetFieldValueAndLength(GPRSS_IP_ADDR, aFieldValueBuf) != 0)
		{
		delete iAddr;
		iAddr = NULL;
		iAddr = CIpAddress::NewL(aFieldValueBuf);
		}

	if (aIacpFields.GetFieldValueAndLength(GPRSS_IP_NETMASK, aFieldValueBuf) != 0)
		{
		delete iNetMask;
		iNetMask = NULL;
		iNetMask = CIpAddress::NewL(aFieldValueBuf);
		}

	if (aIacpFields.GetFieldValueAndLength(GPRSS_IP_NAME_SERVER1, aFieldValueBuf) != 0)
		{
		delete iDNS1;
		iDNS1 = NULL;
		iDNS1 = CIpAddress::NewL(aFieldValueBuf);
		}

	if (aIacpFields.GetFieldValueAndLength(GPRSS_IP_NAME_SERVER2, aFieldValueBuf) != 0)	
		{
		delete iDNS2;
		iDNS2 = NULL;
		iDNS2 = CIpAddress::NewL(aFieldValueBuf);
		}
	}

void CGprsParser::ProcessL(CIacSettingsParser::TIacpConnectionPreferenceFlag& aConnectionPreference)
	{
	iNoExistingDNS	  = ETrue;
	
	
	WriteToGPRSTableL(EOutgoing);
	WriteToGPRSTableL(EIncoming);

	WriteToIAPTableL();

	if(aConnectionPreference == CIacSettingsParser::EIacpAttempCreateAsFirstRank ||
		aConnectionPreference == CIacSettingsParser::EIacpAttempCreateAsSecondRank)
		CreatePreferencesTableL(ECommDbConnectionDirectionOutgoing,aConnectionPreference);	

	}

void CGprsParser::WriteToIAPTableL()
	{
	CCDIAPRecord* iapRecord = static_cast<CCDIAPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
	CleanupStack::PushL(iapRecord);
	iapRecord->iRecordName.SetMaxLengthL(iName->Length());
	iapRecord->iRecordName = iName->Des();
	TBool recordExist = FALSE;
	if(!(iapRecord->FindL(*iDbSession)))
		{
		iapRecord->SetRecordId(KCDNewRecordRequest);		
		}
	else
		{
		recordExist = TRUE;
		}
	iapRecord->iServiceType.SetMaxLengthL(((TPtrC)KCDTypeNameOutgoingWCDMA).Length());
	iapRecord->iServiceType = TPtrC(KCDTypeNameOutgoingWCDMA);
	iapRecord->iService =  GetIapIdL(TPtrC(KCDTypeNameOutgoingWCDMA));
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
	CleanupStack::PopAndDestroy(iapRecord);

	}

void CGprsParser::CreatePreferencesTableL(TCommDbConnectionDirection aDirection, CIacSettingsParser::TIacpConnectionPreferenceFlag& aConnectionPreference)
	{
	CCDConnectionPrefsRecord* connectionPrefsRecord = static_cast<CCDConnectionPrefsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdConnectionPrefsRecord));
	CleanupStack::PushL(connectionPrefsRecord);
	connectionPrefsRecord->iRanking = aConnectionPreference;
	connectionPrefsRecord->iDirection = aDirection;
	connectionPrefsRecord->iDialogPref = ECommDbDialogPrefPrompt;
	connectionPrefsRecord->iBearerSet  = ECommDbBearerWcdma;
	connectionPrefsRecord->SetRecordId(iRecordId);
	if ( connectionPrefsRecord->FindL(*iDbSession) )
		{
		aConnectionPreference = CIacSettingsParser::EIacpDoNotCreatePreference;
		}
	else
		{
		connectionPrefsRecord->StoreL(*iDbSession);
		}
	CleanupStack::PopAndDestroy(connectionPrefsRecord);
	}


//
// Get COMMDB_ID from table that matches the COMMDB_NAME extracted from BioMsg
//
TUint32 CGprsParser::GetIapIdL(const TDesC& aTableName)
	{
	const TPtrC KNameFieldToken(iName->Des());


	TUint32 ret = 0;
	if (aTableName.CompareF(TPtrC(KCDTypeNameOutgoingWCDMA)) == KErrNone)
		{
		CCDOutgoingGprsRecord* gprsRecord = static_cast<CCDOutgoingGprsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdOutgoingGprsRecord));	
		CleanupStack::PushL(gprsRecord);
		gprsRecord->iRecordName.SetMaxLengthL(KNameFieldToken.Length());
		gprsRecord->iRecordName =  KNameFieldToken;	
		if (gprsRecord->FindL(*iDbSession))
			{		
			ret = gprsRecord->iRecordTag.RecordId();
			}
		else
			{
			User::Leave(KErrNotFound);
			}
		CleanupStack::PopAndDestroy(gprsRecord);	
		}	
	return ret;
	}

void CGprsParser::WriteToGPRSTableL(TDirection aDir)
	{
	CCDWCDMAPacketServiceRecord* gprsRecord = NULL;
	if (aDir == EOutgoing)
		{
		gprsRecord = static_cast<CCDOutgoingGprsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdOutgoingGprsRecord));	
		}
	else
		{
		gprsRecord = static_cast<CCDIncomingGprsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIncomingGprsRecord));	
		}
	CleanupStack::PushL(gprsRecord);
	gprsRecord->iRecordName.SetMaxLengthL(iName->Length());
	gprsRecord->iRecordName = iName->Des();
	TBool recordExist = FALSE;
	if(!(gprsRecord->FindL(*iDbSession)))
		{
		gprsRecord->SetRecordId(KCDNewRecordRequest);		
		}
	else
		{
		recordExist = TRUE;
		}
	gprsRecord->iGPRSAPN.SetMaxLengthL(iApn->Length());
	gprsRecord->iGPRSAPN = *iApn;
	gprsRecord->iGPRSPDPType = iPdpType;
	gprsRecord->iGPRSPDPAddress.SetMaxLengthL(KCommsDbSvrMaxFieldLength);
	gprsRecord->iGPRSPDPAddress = iPdpAddress->Addr();
	gprsRecord->iGPRSIfPromptForAuth = iLoginPrompt;
	if(iLoginName != NULL)
		{
		gprsRecord->iGPRSIfAuthName.SetMaxLengthL(iLoginName->Length());
		gprsRecord->iGPRSIfAuthName = *iLoginName;			
		}
	if(iLoginPass != NULL)
		{
		gprsRecord->iGPRSIfAuthPass.SetMaxLengthL(iLoginPass->Length());
		gprsRecord->iGPRSIfAuthPass = *iLoginPass;
		}
	if(iNetMask != NULL)
		{
		gprsRecord->iGPRSIPNetMask.SetMaxLengthL(KCommsDbSvrMaxFieldLength);
		gprsRecord->iGPRSIPNetMask = iNetMask->Addr();	
		}
	if(iDNS1 != NULL)
		{
		gprsRecord->iGPRSIPNameServer1.SetMaxLengthL(KCommsDbSvrMaxFieldLength);
		gprsRecord->iGPRSIPNameServer1 = iDNS1->Addr();	
		}		
	if(iDNS2 != NULL)
		{
		gprsRecord->iGPRSIPNameServer2.SetMaxLengthL(KCommsDbSvrMaxFieldLength);
		gprsRecord->iGPRSIPNameServer2 = iDNS2->Addr();	
		}		
	if(!iDNS1 && !iDNS2 && iNoExistingDNS)
		{
		gprsRecord->iGPRSIPDNSAddrFromServer = EFalse;		
		}
	else
		{
		gprsRecord->iGPRSIPDNSAddrFromServer = ETrue;		
		}		
	gprsRecord->iGPRSDisablePlainTextAuth = EFalse;
	if (!recordExist)
		{
		gprsRecord->StoreL(*iDbSession);
		}
	else 
		{
		gprsRecord->ModifyL(*iDbSession);	
		}
	iNoExistingDNS = gprsRecord->iGPRSIPDNSAddrFromServer;
	CleanupStack::PopAndDestroy(gprsRecord);			
	}

TPtrC CGprsParser::LimitStringSize(const TDesC& aString)
	{
	if (aString.Length() < KCommsDbSvrMaxFieldLength)
		return aString;
	else
		return aString.Left(KCommsDbSvrMaxFieldLength);
	}

TPtrC8 CGprsParser::LimitStringSize(const TDesC8& aString)
	{
	if (aString.Length() < KCommsDbSvrMaxFieldLength)
		return aString;
	else
		return aString.Left(KCommsDbSvrMaxFieldLength);
	}
