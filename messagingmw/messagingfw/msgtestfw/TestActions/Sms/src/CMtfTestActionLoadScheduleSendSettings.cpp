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
// LoadScheduleSendSettings
// [Action Parameters]
// scheduleSettings		output		CMsvScheduleSettings 
// offPeakTimes			output		CMsvOffPeakTimes
// errorActions			output 		CMsvSendErrorActions
// sysAgentActions		output		CMsvSysAgentActions
// [Action Description]
// LoadScheduleSendSettings Test Action is intended to retrieve Schedule Send 
// settings from the central repository.
// [APIs Used]	
// CSMSAccount::LoadSettingsL
// __ACTION_INFO_END__
// 
//

#include <msvsenderroraction.h>
#include <msvsysagentaction.h>
#include <msvoffpeaktime.h>
#include <msvschedulesettings.h>
#include <csmsaccount.h>

#include "CMtfTestActionLoadScheduleSendSettings.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"



/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionLoadScheduleSendSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionLoadScheduleSendSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionLoadScheduleSendSettings* self = new (ELeave) CMtfTestActionLoadScheduleSendSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionLoadScheduleSendSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionLoadScheduleSendSettings::CMtfTestActionLoadScheduleSendSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionLoadSmsSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionLoadScheduleSendSettings::~CMtfTestActionLoadScheduleSendSettings()
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
void CMtfTestActionLoadScheduleSendSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionLoadScheduleSendSettings);
	CSmsAccount	*smsAccount = CSmsAccount::NewL();
	CleanupStack::PushL( smsAccount );
	
	CMsvSysAgentActions *sysAgtActions = new (ELeave) CMsvSysAgentActions;
	CleanupStack::PushL( sysAgtActions );
	
	CMsvSendErrorActions *sendErrorActions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(sendErrorActions);
	
	CMsvOffPeakTimes *offPeakTimes = new (ELeave) CMsvOffPeakTimes;
	CleanupStack::PushL( offPeakTimes );
	
	CMsvScheduleSettings *scheduleSettings = CMsvScheduleSettings::NewL();
	CleanupStack::PushL( scheduleSettings );
	
	smsAccount->LoadSettingsL( *scheduleSettings, *offPeakTimes, *sendErrorActions, *sysAgtActions );
		
	StoreParameterL<CMsvScheduleSettings>(TestCase(), *scheduleSettings, ActionParameters().Parameter(0));
	CleanupStack::Pop(scheduleSettings);
	
	StoreParameterL<CMsvOffPeakTimes>(TestCase(), *offPeakTimes, ActionParameters().Parameter(1));
	CleanupStack::Pop(offPeakTimes);
	
	StoreParameterL<CMsvSendErrorActions>(TestCase(), *sendErrorActions, ActionParameters().Parameter(2));
	CleanupStack::Pop(sendErrorActions);
	
	StoreParameterL<CMsvSysAgentActions>(TestCase(), *sysAgtActions, ActionParameters().Parameter(3));
	CleanupStack::Pop(sysAgtActions);
	
	CleanupStack::PopAndDestroy(smsAccount);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionLoadScheduleSendSettings );
	TestCase().ActionCompletedL(*this);

	}


