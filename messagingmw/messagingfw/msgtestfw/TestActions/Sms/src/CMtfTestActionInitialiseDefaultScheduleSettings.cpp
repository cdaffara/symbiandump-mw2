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
// InitialiseDefaultScheduleSettings
// [Action Parameters]
// ScheduleSettings		output		CMsvScheduleSettings
// OffPeakTimes			output		CMsvOffPeakTimes
// ErrorActions			output		CMsvSendErrorActions
// SysAgentActions			output		CMsvSysAgentActions
// [Action Description]
// InitialiseDefaultScheduleSettings Test Action is intended to set the member 
// variables of CMsvScheduleSettings, CMsvOffPeakTimes, CMsvSendErrorActions, 
// CMsvSysAgentActions classes with the default values.
// [APIs Used]	
// CSMSAccount::InitialiseDefaultScheduleSettingsL 
// __ACTION_INFO_END__
// 
//

#include <msvsenderroraction.h>
#include <msvsysagentaction.h>
#include <msvoffpeaktime.h>
#include <msvschedulesettings.h>
#include <csmsaccount.h>

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionInitialiseDefaultScheduleSettings.h"




/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionInitialiseDefaultScheduleSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionInitialiseDefaultScheduleSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionInitialiseDefaultScheduleSettings* self = new (ELeave) CMtfTestActionInitialiseDefaultScheduleSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionInitialiseDefaultScheduleSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
	
CMtfTestActionInitialiseDefaultScheduleSettings::CMtfTestActionInitialiseDefaultScheduleSettings(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionInitialiseDefaultScheduleSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionInitialiseDefaultScheduleSettings::~CMtfTestActionInitialiseDefaultScheduleSettings()
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
void CMtfTestActionInitialiseDefaultScheduleSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionInitialiseDefaultScheduleSettings);
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
	
	smsAccount->InitialiseDefaultSettingsL( *scheduleSettings, *offPeakTimes, *sendErrorActions, *sysAgtActions );
		
	StoreParameterL<CMsvScheduleSettings>(TestCase(), *scheduleSettings, ActionParameters().Parameter(0));
	CleanupStack::Pop(scheduleSettings);
	
	StoreParameterL<CMsvOffPeakTimes>(TestCase(), *offPeakTimes, ActionParameters().Parameter(1));
	CleanupStack::Pop(offPeakTimes);
	
	StoreParameterL<CMsvSendErrorActions>(TestCase(), *sendErrorActions, ActionParameters().Parameter(2));
	CleanupStack::Pop(sendErrorActions);

//	CleanupStack::PopAndDestroy(sendErrorActions);


	
	StoreParameterL<CMsvSysAgentActions>(TestCase(), *sysAgtActions, ActionParameters().Parameter(3));
	CleanupStack::Pop(sysAgtActions);
	
//	CleanupStack::PopAndDestroy(sysAgtActions);
//
	
	CleanupStack::PopAndDestroy( smsAccount );
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionInitialiseDefaultScheduleSettings );
	TestCase().ActionCompletedL(*this);
	
	}


