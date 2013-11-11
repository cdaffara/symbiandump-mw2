// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
*/

#include "CInitSimTsyTestStep.h"

#include <simtsy.h>
#include <commsdattypesv1_1.h>
#include <etel.h>
#include <etelmm.h>

using namespace CommsDat;
using namespace SharedData;

const TInt KMsvTestUidPhonePwrValue=0x100052C5;
const TInt KUidCmtPowerValue=0x100052D9;

enum TSACmtPower
	{	
	ECmtOn,		//< Specifies that the power is on
	ECmtOff		//< Specifies that the power is off
	};

enum TMsvTestPhoneStatus
	{
	EMsvTestPhoneOff = 0,
	EMsvTestPhoneOn
	};
/**
 Constructor to set test step name
*/
CInitSimTsyTestStep::CInitSimTsyTestStep(MSharedData& aObj)
:CBasicTestStep(aObj)
	{
	SetTestStepName(KInitSimTsyTestStep);
	}


CInitSimTsyTestStep::~CInitSimTsyTestStep()
	{
	}
TVerdict CInitSimTsyTestStep::doTestStepL()
	{
	TName tsy;
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KMsvTestUidPhonePwrValue, EMsvTestPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidCmtPowerValue, ECmtOn));

	//Initialize TSY using the System Agent
	TInt testNumber=0; // Input from ini

	SetTestStepResult(EPass);
	
	_LIT(KTestNumber,"TestNumber");
	if ( !GetIntFromConfig(ConfigSection(),KTestNumber,testNumber))
		{
		ERR_PRINTF1(_L("Time"));
		User::Leave(KErrNotReady);
		}

	//Set to number which needs to be used
	TInt ret = RProperty::Define(KUidPSSimTsyCategory, KPSSimTsyTestNumber, RProperty::EInt);
	if ( (ret==KErrNone) || (ret== KErrAlreadyExists) )
		{
		//Attach to the property so that you can track any chnages to it.
		RProperty property;
		ret=property.Attach(KUidPSSimTsyCategory, KPSSimTsyTestNumber);
		if ( ret==KErrNone )
			{
			//Set the new test number and wait until the change is in effect.
			TRequestStatus	status;
			property.Subscribe(status);
			ret=property.Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testNumber);
			if ( ret==KErrNone )
				{
				User::WaitForRequest(status);
				ret=status.Int();
				if ( ret != KErrNone )
					{
					SetTestStepResult(EFail);
					INFO_PRINTF2(_L("request complete error=%d"), ret);
					}
				}
			else
				{
				SetTestStepResult(EFail);
				INFO_PRINTF2(_L("RProperty::Set error=%d"), ret);
				}
			property.Close();
			}
		else
			{
			SetTestStepResult(EFail);
			INFO_PRINTF2(_L("RProperty::Attach error=%d"), ret);
			}
		}
	else
		{
		SetTestStepResult(EFail);
		INFO_PRINTF2(_L("RProperty::Define error=%d"), ret);
		}

	CMDBSession* dbSession = CMDBSession::NewLC(CMDBSession::LatestVersion());
	
	CMDBRecordSet<CCDGlobalSettingsRecord> globalSettingsRecord(KCDTIdGlobalSettingsRecord);
	TRAPD(err, globalSettingsRecord.LoadL(*dbSession));
	if(err != KErrNone)
		{
		User::Leave(KErrNotFound);				
		}
		
	CCDModemBearerRecord *modemRecord = static_cast<CCDModemBearerRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdModemBearerRecord));		
	CleanupStack::PushL(modemRecord);	
	modemRecord->SetRecordId(((CCDGlobalSettingsRecord*)globalSettingsRecord.iRecords[0])->iModemForPhoneServicesAndSMS); 
	modemRecord->LoadL(*dbSession);
	tsy = modemRecord->iTsyName;
	INFO_PRINTF2(_L("Name of Tsy is %S"), &tsy);
	CleanupStack::PopAndDestroy(2); //dbSession, modemRecord

	RTelServer server;
	User::LeaveIfError(server.Connect());
	CleanupClosePushL(server);
	User::LeaveIfError(server.LoadPhoneModule(tsy));

	// Find the phone corresponding to this TSY and open a number of handles on it
	TInt numPhones;
	User::LeaveIfError(server.EnumeratePhones(numPhones));
	RPhone phone;

	while (numPhones--)
		{
		TName phoneTsy;
		User::LeaveIfError(server.GetTsyName(numPhones,phoneTsy));
		if (phoneTsy.CompareF(tsy)==KErrNone)
			{
			RTelServer::TPhoneInfo info;
			User::LeaveIfError(server.GetPhoneInfo(numPhones,info));
			User::LeaveIfError(phone.Open(server,info.iName));
			CleanupClosePushL(phone);
			const TInt err = phone.Initialise();
			User::LeaveIfError(err);
			CleanupStack::PopAndDestroy(&phone);
			break;
			}
		}

	CleanupStack::PopAndDestroy(&server);
	
	INFO_PRINTF1(_L("Successfully initialised the Sim Tsy"));
	return TestStepResult();
	}
