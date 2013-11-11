// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <msvuids.h>
#include <msvids.h>
#include "test_schedulesettings_step.h"
#include <msvoffpeaktime.h>
#include <msvsysagentaction.h>
#include <csmsaccount.h>

	
/**
CMsvScheduleSettingsStep
*/
CMsvScheduleSettingsStep::CMsvScheduleSettingsStep()
	{
	SetTestStepName(KMsvScheduleSettingsStep);
	}

void CMsvScheduleSettingsStep::HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*)
	{
	//do nothing
	}
	
TVerdict CMsvScheduleSettingsStep::doTestStepPreambleL()
	{
	__UHEAP_MARK;
	iScheduler = new (ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);
	
	iSession = CMsvSession::OpenSyncL(*this);
	
	iScheduleSettings = CMsvScheduleSettings::NewL();
	
	iPriority		= CMsvScheduleSettings::EDefaultPriority;
	iValidityPeriod = CMsvScheduleSettings::EDefaultValidityPeriod;
	iIntervalType	= (TIntervalType) (CMsvScheduleSettings::EDefaultIntervalType);

	iShortInterval	= CMsvScheduleSettings::EDefaultShortInterval;
	iLongInterval	= CMsvScheduleSettings::EDefaultLongInterval;

	iLatency		= CMsvScheduleSettings::EDefaultLatency;
	iPendingConditionsTimeout = CMsvScheduleSettings::EDefaultPendingConditionsTimeout;

	iVariableIntervals = new (ELeave) CArrayFixFlat<TTimeIntervalSeconds>(1);
	
	return TestStepResult();
	}	

TVerdict CMsvScheduleSettingsStep::doTestStepL()
	{
	//test constructor 
	if(!TestGetValues(iScheduleSettings))
		{
		SetTestStepResult(EFail);
		INFO_PRINTF1(_L("CMsvScheduleSettings: Default values wrong!"));
		}
	
	//test setters and getters
	TestSetValuesL();
	if(!TestGetValues(iScheduleSettings))
		{
		SetTestStepResult(EFail);
		INFO_PRINTF1(_L("CMsvScheduleSettings: setter/getter wrong!"));
		}
	
	//test store and restore
	TestStoreRestoreL();
	
	//test reset
	TestReset();
	
	return TestStepResult();
	}	

TVerdict CMsvScheduleSettingsStep::doTestStepPostambleL()
	{
	delete iVariableIntervals;
	delete iScheduleSettings;
	delete iSession;
	CActiveScheduler::Install(NULL);
	delete iScheduler;
	__UHEAP_MARKEND;
	return TestStepResult();
	}
	
TBool CMsvScheduleSettingsStep::TestGetValues(const CMsvScheduleSettings* aScheduleSettings)
	{
	TBool result = ETrue;
	
	const CArrayFixFlat<TTimeIntervalSeconds>& variableIntervals = aScheduleSettings->VariableIntervals();
	if(
		aScheduleSettings->Priority() != iPriority ||
		aScheduleSettings->ValidityPeriod().Int() != iValidityPeriod.Int() ||
		aScheduleSettings->IntervalType() != iIntervalType ||
		aScheduleSettings->LongInterval().Int() != iLongInterval.Int() ||
		aScheduleSettings->ShortInterval().Int() != iShortInterval.Int() ||
		aScheduleSettings->Latency().Int() != iLatency.Int() ||
		aScheduleSettings->PendingConditionsTimeout().Int() != iPendingConditionsTimeout.Int() ||
		variableIntervals.Count() != iVariableIntervals->Count())
		{
		result = EFalse;
		}
	
	TInt count = variableIntervals.Count();
	while(result && count--)
		{
		if(variableIntervals.At(count).Int() != iVariableIntervals->At(count).Int())
			{
			result = EFalse;
			}
		}		
	return result;		
	}

void CMsvScheduleSettingsStep::TestSetValuesL()
	{
	iPriority = 1;
	iScheduleSettings->SetPriority(iPriority);
	iValidityPeriod = 6;
	iScheduleSettings->SetValidityPeriod(iValidityPeriod);
	iIntervalType = EDaily;
	iScheduleSettings->SetIntervalType(iIntervalType);
	iLongInterval = 7;
	iScheduleSettings->SetLongInterval(iLongInterval);
	iShortInterval = 8;
	iScheduleSettings->SetShortInterval(iShortInterval);
	iLatency = 9;
	iScheduleSettings->SetLatency(iLatency);
	iPendingConditionsTimeout = 10;
	iScheduleSettings->SetPendingConditionsTimeout(iPendingConditionsTimeout);

	iVariableIntervals->AppendL(2);
	iVariableIntervals->AppendL(3);
	iVariableIntervals->AppendL(4);
	iScheduleSettings->SetVariableIntervalsL(*iVariableIntervals);
	}

void CMsvScheduleSettingsStep::TestStoreRestoreL()
	{
	// access sms account settings
	CSmsAccount* account = CSmsAccount::NewLC();

	CMsvScheduleSettings* scheduleSettings = CMsvScheduleSettings::NewL();
	CleanupStack::PushL(scheduleSettings);
	CMsvOffPeakTimes* offPeakTimes = new(ELeave) CMsvOffPeakTimes;
	CleanupStack::PushL(offPeakTimes);
	CMsvSendErrorActions* errorActions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(errorActions);
	CMsvSysAgentActions* sysAgentActions = new(ELeave) CMsvSysAgentActions;
	CleanupStack::PushL(sysAgentActions);

	// load existing settings
	TRAPD(error, account->LoadSettingsL(*scheduleSettings, *offPeakTimes, *errorActions, *sysAgentActions));
	
	// save these settings
	account->SaveSettingsL(*iScheduleSettings, *offPeakTimes, *errorActions, *sysAgentActions);

	// try reload these settings
	CMsvScheduleSettings* testScheduleSettings = CMsvScheduleSettings::NewL();
	CleanupStack::PushL(testScheduleSettings);
	account->LoadSettingsL(*testScheduleSettings, *offPeakTimes, *errorActions, *sysAgentActions);

	// test settings
	if (!TestGetValues(testScheduleSettings))
		{
		INFO_PRINTF1(_L("CMsvScheduleSettings: Store/Restore wrong!"));
		SetTestStepResult(EFail);
		}

	// save original settings
	if (!error)
		{
		account->SaveSettingsL(*scheduleSettings, *offPeakTimes, *errorActions, *sysAgentActions);
		}
	
	CleanupStack::PopAndDestroy(6, account); // testScheduleSettings, sysAgentActions, errorActions, offPeakTimes, scheduleSettings, account
	}

	
void CMsvScheduleSettingsStep::TestReset()
	{
	iScheduleSettings->Reset();
	iPriority		= CMsvScheduleSettings::EDefaultPriority;
	iValidityPeriod = CMsvScheduleSettings::EDefaultValidityPeriod;
	iIntervalType	= (TIntervalType) (CMsvScheduleSettings::EDefaultIntervalType);

	iShortInterval	= CMsvScheduleSettings::EDefaultShortInterval;
	iLongInterval	= CMsvScheduleSettings::EDefaultLongInterval;

	iLatency		= CMsvScheduleSettings::EDefaultLatency;
	iPendingConditionsTimeout = CMsvScheduleSettings::EDefaultPendingConditionsTimeout;

	iVariableIntervals->Reset();
	
	if(!TestGetValues(iScheduleSettings))
		{
		INFO_PRINTF1(_L("CMsvScheduleSettings: Reset wrong!"));
		SetTestStepResult(EFail);
		}
	}		
	

