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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// SetSchSendSettings
// [Action Parameters]
// CMsvSession& Session    <input>: Reference to the session.
// TMsvId       ServiceId  <input>: Value of the SMS service id.
// TInt         Index      <input>: Selects what defaults file to use
// [Action Description]
// Modifies the settings related to schedule send (namely, system agent condition action array, 
// CMsvScheduleSettings), of the specified SMS service to match the ones specified on a file.
// [APIs Used]
// CMsvEntry::EditStoreL
// CSmsSettings::StoreL
// CMsvStore::CommitL
// __ACTION_INFO_END__
// 
//

#include <msvapi.h>
#include <msvsysagentaction.h>
#include <msvschedulesettings.h>


#include <csmsaccount.h>
#include <msvoffpeaktime.h>


#include "CMtfTestActionSetSchSendSettings.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "TMtfTestActionUtilsSchSendScripts.h"


CMtfTestAction* CMtfTestActionSetSchSendSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetSchSendSettings* self = new (ELeave) CMtfTestActionSetSchSendSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

CMtfTestActionSetSchSendSettings::CMtfTestActionSetSchSendSettings(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


void CMtfTestActionSetSchSendSettings::ExecuteActionL()
	{


	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetSchSendSettings);

	// PARAMS 0 & 1 are ignored for the plat sec test action - they should be removed eventually
	TInt paramDefaultIndex = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));			
	// Get the settings file name from the test case
	const TPtrC settingsFile = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfSmsSettings, paramDefaultIndex);
	CMsvScheduleSettings* scheduleSettings = CMsvScheduleSettings::NewLC();
	TMtfTestActionUtilsSchSendScripts::ReadScheduleSettingsFromConfigurationFileL(TestCase(), settingsFile, *scheduleSettings);

	CMsvOffPeakTimes* offPeakTimes = new (ELeave) CMsvOffPeakTimes;
	CleanupStack::PushL(offPeakTimes);	
	// TODO ? - add some actions to offpeak times using TMsvOffPeakTime API
	// not done in pre __MESSAGING_API_V2__ testaction
	
	CMsvSendErrorActions* sendErrorActions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(sendErrorActions);
	// TODO ? - add some send error actions using TMsvSendErrorAction API
	// not done in pre __MESSAGING_API_V2__ testaction
	
	CMsvSysAgentActions* sysAgentActions = new (ELeave) CMsvSysAgentActions;
	CleanupStack::PushL(sysAgentActions);
	TMtfTestActionUtilsSchSendScripts::ReadScheduleSettingsFromConfigurationFileL(TestCase(), settingsFile, *sysAgentActions);
	// TODO ? - add some sys agent actions using TMsvSysAgentConditionAction API
	// not done in pre __MESSAGING_API_V2__ testaction
	
	// save the settings to the central repository 
	CSmsAccount *smsAccount = CSmsAccount::NewLC();
	smsAccount->SaveSettingsL( *scheduleSettings, *offPeakTimes, *sendErrorActions, *sysAgentActions);

	CleanupStack::PopAndDestroy(5, scheduleSettings); // scheduleSettings, offPeakTimes, sendErrorActions, sysAgentActions, smsAccount

	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetSchSendSettings);
	TestCase().ActionCompletedL(*this);

	}

