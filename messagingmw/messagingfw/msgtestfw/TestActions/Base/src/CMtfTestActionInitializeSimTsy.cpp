// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// InitializeSimTsy
// [Action Parameters]
// TInt		Section		<input>: Section of the SimTsy configuration file to use.
// HBufC	TsyName	    <input-optional>: Name of the Tsy to use. Defaults to "SIM".
// [Action Description]
// Initializes SimTsy with the configuration section specified.
// The name of the TSY to use can be specified using the second parameter.
// [APIs Used]
// __ACTION_INFO_END__
// 
//


#include <e32base.h>
#include <e32property.h>
#include <simtsy.h>
#include <etelmm.h>
#include <es_ini.h>
#include <commsdattypesv1_1.h>
#include <commsdat.h>
using namespace CommsDat;
#include "CMtfTestActionInitializeSimTsy.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


const TInt KMsvTestUidPhonePwrValue=0x100052C5;
enum TMsvTestPhoneStatus
	{
	EMsvTestPhoneOff = 0,
	EMsvTestPhoneOn
	};

	
CMtfTestAction* CMtfTestActionInitializeSimTsy::NewL(CMtfTestCase& aTestCase, CMtfTestActionParameters* aActionParameters)
 	{
 	CMtfTestActionInitializeSimTsy* self = new (ELeave) CMtfTestActionInitializeSimTsy(aTestCase);
 	CleanupStack::PushL(self);
 	self->ConstructL(aActionParameters);
 	CleanupStack::Pop(self);
 	return self;
 	}
 
CMtfTestActionInitializeSimTsy::CMtfTestActionInitializeSimTsy(CMtfTestCase& aTestCase)
 	: CMtfSynchronousTestAction(aTestCase)
 	{
 	}

void CMtfTestActionInitializeSimTsy::ExecuteActionL()
	{
	TestCase().Logger().Write(_L("CMtfTestActionInitializeSimTsy::ExecuteActionL IN"));
	_LIT(KDefaultTsyName, "SIM"); 
	TPtrC defaultTsyName(KDefaultTsyName);
	HBufC* defaultTsyNameBuf = defaultTsyName.AllocLC();
	TInt testNumber = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0));
	HBufC* tsyName = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(1), defaultTsyNameBuf);
	
	InitializeSimTsyL(testNumber, tsyName);
	
	CMDBSession* dbSession = CMDBSession::NewL(CMDBSession::LatestVersion());
	CleanupStack::PushL(dbSession);
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
	modemRecord->iTsyName.SetMaxLengthL(tsyName->Des().Length());
	modemRecord->iTsyName = tsyName->Des();	
	modemRecord->ModifyL(*dbSession);
	CleanupStack::PopAndDestroy(3, defaultTsyNameBuf);  //tsyName, dbSession, modemRecord 
	
	TestCase().ActionCompletedL(*this);
	TestCase().Logger().Write(_L("CMtfTestActionInitializeSimTsy::ExecuteActionL OUT"));
	}


void CMtfTestActionInitializeSimTsy::InitializeSimTsyL(TInt aTestNumber, HBufC* aTsyName)
	{
	//Initialize TSY using the System Agent
	TInt testState;
	if(KErrNone != RProperty::Get(KUidPSSimTsyCategory, KPSSimTsyTestNumber, testState))
		{
		User::LeaveIfError(RProperty::Define(KUidPSSimTsyCategory, KPSSimTsyTestNumber, RProperty::EInt));
		}
	TestCase().Logger().WriteFormat(_L("CMtfTestActionInitializeSimTsy::ExecuteActionL Setting test number to %d"), aTestNumber);
	User::LeaveIfError(RProperty::Set(KUidPSSimTsyCategory, KPSSimTsyTestNumber, aTestNumber));
	if(KErrNone != RProperty::Get(KUidSystemCategory, KMsvTestUidPhonePwrValue, testState))
		{
		User::LeaveIfError(RProperty::Define(KUidSystemCategory, KMsvTestUidPhonePwrValue, RProperty::EInt));
		}
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KMsvTestUidPhonePwrValue, EMsvTestPhoneOn));

	User::LeaveIfError(RProperty::Get(KUidPSSimTsyCategory, KPSSimTsyTestNumber, aTestNumber));
	TestCase().Logger().WriteFormat(_L("CMtfTestActionInitializeSimTsy::ExecuteActionL Got test number %d"), aTestNumber);

	RTelServer etelServer;
	TInt err = etelServer.Connect();
	// Fail the Test Case if the connection is not established
	if (err != KErrNone)
		{
		TestCase().ERR_PRINTF2(_L("Connection with the eTelServer server is failed with error %d "), err);
		TestCase().SetTestStepResult(EFail);
		}
	User::LeaveIfError(etelServer.LoadPhoneModule(aTsyName->Des()));

	// Find the phone corresponding to this TSY and open a number of handles on it
	TInt numPhones;
	User::LeaveIfError(etelServer.EnumeratePhones(numPhones));	
	TBool found=EFalse;
	
	RMobilePhone iPhone;
	while (!found && numPhones--)
		{
		TName phoneTsy;
		User::LeaveIfError(etelServer.GetTsyName(numPhones,phoneTsy));
		if (phoneTsy.CompareF(aTsyName->Des())==KErrNone)
			{
			found = ETrue;
			RTelServer::TPhoneInfo info;
			User::LeaveIfError(etelServer.GetPhoneInfo(numPhones,info));
			CleanupClosePushL(iPhone);
			User::LeaveIfError(iPhone.Open(etelServer,info.iName));
			User::LeaveIfError(iPhone.Initialise());
			CleanupStack::PopAndDestroy(&iPhone);
			}
		}
	StoreParameterL<RTelServer>(TestCase(),etelServer,ActionParameters().Parameter(2));
	}
	
