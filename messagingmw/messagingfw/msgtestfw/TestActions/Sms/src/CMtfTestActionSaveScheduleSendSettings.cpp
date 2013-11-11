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
// SaveScheduleSendSettings
// [Action Parameters]
// configFileIndex	<input>:	Configuration file index
// ScheduleSettings	<output>:	CMsvScheduleSettings
// OffPeakTimes		<output>:	CMsvOffPeakTimes
// ErrorActions		<output>:	CMsvSendErrorActions
// SysAgentActions	<output>:	CMsvSysAgentActions
// [Action Description]
// SaveScheduleSendSettings Test Action is intended to modify and save existing Schedule Send settings 
// to CenRep using configuration settings file.  The Test Action creates the CMsvScheduleSettings, 
// CMsvOffPeakTimes, CMsvSendErrorActions and CMsvSysAgentActions object, parses the settings file and 
// then sets the member variables of the objects to the corresponding value stored in the settings file.  
// These objects are then used to save the settings into the Central Repository.  It is assumed that the 
// settings for Schedule send settings Off Peak Times, Send Error Actions and Agent Actions will be 
// present in the same ini file.
// [APIs Used]	
// CSMSAccount::SaveSettingsL
// CMsvScheduleSettings::SetPriority
// CMsvScheduleSettings::SetValidityPeriod
// CMsvScheduleSettings::SetIntervalType
// CMsvScheduleSettings::SetLatency
// CMsvScheduleSettings::SetLongInterval
// CMsvScheduleSettings::SetShortInterval
// __ACTION_INFO_END__
// 
//



#include <msvsenderroraction.h>
#include <msvsysagentaction.h>
#include <msvoffpeaktime.h>
#include <msvschedulesettings.h>
#include <csmsaccount.h>

#include "CMtfTestCase.h"
#include "CMtfTestActionSaveScheduleSendSettings.h"
#include "TMtfTestActionUtilsSchSendScripts.h"
#include "CMtfTestActionParameters.h"



/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSaveScheduleSendSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSaveScheduleSendSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSaveScheduleSendSettings* self = new (ELeave) CMtfTestActionSaveScheduleSendSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSaveScheduleSendSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSaveScheduleSendSettings::~CMtfTestActionSaveScheduleSendSettings()
	{
	}
	
/**
  Function : ~CMtfTestActionSaveScheduleSendSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSaveScheduleSendSettings::CMtfTestActionSaveScheduleSendSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ExecuteActionL
  Description : Entry point for the this test action in the test framework
  @internalTechnology
  @param : none
  @return : void
  @pre none 
  @post none
*/
void CMtfTestActionSaveScheduleSendSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSaveScheduleSendSettings);
	CSmsAccount* smsAccount = CSmsAccount::NewL();
	CleanupStack::PushL(smsAccount);
	
	CMsvSysAgentActions* sysAgentActions = new (ELeave) CMsvSysAgentActions;
	CleanupStack::PushL(sysAgentActions);
	
	CMsvSendErrorActions* errorActions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(errorActions);
	
	CMsvOffPeakTimes* offPeakTimes = new (ELeave) CMsvOffPeakTimes;
	CleanupStack::PushL(offPeakTimes);
	
	CMsvScheduleSettings* scheduleSettings = CMsvScheduleSettings::NewL();
	CleanupStack::PushL(scheduleSettings);
	
	// Get index for schedule settings config file, see CMtfTestServer.cpp
	TInt configFileIndex = ObtainValueParameterL<TInt>(TestCase(), ActionParameters().Parameter(0));
	
	// Get the settings file name from the test case
	const TPtrC settingsFile = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfSmsSettings, configFileIndex);
	
	// Read SMS settings from config file
	TMtfTestActionUtilsSchSendScripts::ReadScheduleSettingsFromConfigurationFileL(TestCase(), settingsFile, *scheduleSettings);

	// Save settings to central repository
	smsAccount->SaveSettingsL(*scheduleSettings, *offPeakTimes, *errorActions, *sysAgentActions);
	
	// Set output parameters
	StoreParameterL<CMsvScheduleSettings>(TestCase(), *scheduleSettings, ActionParameters().Parameter(1));
	CleanupStack::Pop(scheduleSettings);
	
	StoreParameterL<CMsvOffPeakTimes>(TestCase(), *offPeakTimes, ActionParameters().Parameter(2));
	CleanupStack::Pop(offPeakTimes);
	
	StoreParameterL<CMsvSendErrorActions>(TestCase(), *errorActions, ActionParameters().Parameter(3));
	CleanupStack::Pop(errorActions);
	
	StoreParameterL<CMsvSysAgentActions>(TestCase(), *sysAgentActions, ActionParameters().Parameter(4));
	CleanupStack::Pop(sysAgentActions);
	
	CleanupStack::PopAndDestroy(smsAccount);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSaveScheduleSendSettings);
	TestCase().ActionCompletedL(*this);
	}


