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
// CompareScheduleSendSettings
// [Action Parameters]
// ScheduleSettings1	<input>:	CMsvScheduleSettings
// OffPeakTimes1		<input>:	CMsvOffPeakTimes
// ErrorActions1		<input>:	CMsvSendErrorActions
// SysAgentActions1		<input>:	CMsvSysAgentActions
// ScheduleSettings2	<input>:	CMsvScheduleSettings
// OffPeakTimes2		<input>:	CMsvOffPeakTimes
// ErrorActions2		<input>:	CMsvSendErrorActions
// SysAgentActions2		<input>:	CMsvSysAgentActions
// [Action Description]
// CompareScheduleSendsettings Test Action is intended to compare one Schedule Send settings 
// values with other Schedule Send settings values.
// [APIs Used]	
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionCompareScheduleSendSettings.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvschedulesettings.h>
#include <msvoffpeaktime.h>
#include <msvsenderroraction.h>
#include <msvsysagentaction.h>

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionCompareScheduleSendSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionCompareScheduleSendSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCompareScheduleSendSettings* self = new (ELeave) CMtfTestActionCompareScheduleSendSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionCompareScheduleSendSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionCompareScheduleSendSettings::CMtfTestActionCompareScheduleSendSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionCompareScheduleSendSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionCompareScheduleSendSettings::~CMtfTestActionCompareScheduleSendSettings()
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
void CMtfTestActionCompareScheduleSendSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCompareScheduleSendSettings);
	CMsvScheduleSettings* scheduleSettings1 = ObtainParameterReferenceL<CMsvScheduleSettings>(TestCase(), ActionParameters().Parameter(0));
	CMsvScheduleSettings* scheduleSettings2 = ObtainParameterReferenceL<CMsvScheduleSettings>(TestCase(), ActionParameters().Parameter(1));
	CMsvOffPeakTimes* offPeakTimes1 = ObtainParameterReferenceL<CMsvOffPeakTimes>(TestCase(), ActionParameters().Parameter(2));
	CMsvOffPeakTimes* offPeakTimes2 = ObtainParameterReferenceL<CMsvOffPeakTimes>(TestCase(), ActionParameters().Parameter(3));
	CMsvSendErrorActions* sendErrorActions1 = ObtainParameterReferenceL<CMsvSendErrorActions>(TestCase(), ActionParameters().Parameter(4));
	CMsvSendErrorActions* sendErrorActions2 = ObtainParameterReferenceL<CMsvSendErrorActions>(TestCase(), ActionParameters().Parameter(5));
	CMsvSysAgentActions* sysAgentActions1 = ObtainParameterReferenceL<CMsvSysAgentActions>(TestCase(), ActionParameters().Parameter(6));
	CMsvSysAgentActions* sysAgentActions2 = ObtainParameterReferenceL<CMsvSysAgentActions>(TestCase(), ActionParameters().Parameter(7));
	
	if (scheduleSettings1->Priority() != scheduleSettings2->Priority() )
		{
		TestCase().ERR_PRINTF1(_L("Schedule Settings Priority() does not match !"));
		TestCase().SetTestStepResult(EFail);		
		}
	
	else if( scheduleSettings1->ValidityPeriod() != scheduleSettings2->ValidityPeriod() )
		{
		TestCase().ERR_PRINTF1(_L("Schedule Settings ValidityPeriod() does not match !"));
		TestCase().SetTestStepResult(EFail);		
		}
	
	else if( scheduleSettings1->IntervalType() != scheduleSettings2->IntervalType() )
		{
		TestCase().ERR_PRINTF1(_L("Schedule Settings IntervalType()  does not match !"));
		TestCase().SetTestStepResult(EFail);				
		}
	else if( scheduleSettings1->Latency() != scheduleSettings2->Latency() )
		{
		TestCase().ERR_PRINTF1(_L("Schedule Settings Latency()  does not match !"));
		TestCase().SetTestStepResult(EFail);						
		}
	else if( scheduleSettings1->LongInterval() != scheduleSettings2->LongInterval() )
		{
		TestCase().ERR_PRINTF1(_L("Schedule Settings LongInterval()  does not match !"));
		TestCase().SetTestStepResult(EFail);								
		}
	else if( scheduleSettings1->ShortInterval() != scheduleSettings2->ShortInterval() )
		{
		TestCase().ERR_PRINTF1(_L("Schedule Settings ShortInterval()  does not match !"));
		TestCase().SetTestStepResult(EFail);										
		}
	else if ( scheduleSettings1->PendingConditionsTimeout() != scheduleSettings2->PendingConditionsTimeout() )
		{
		TestCase().ERR_PRINTF1(_L("Schedule Settings PendingConditionsTimeout()  does not match !"));
		TestCase().SetTestStepResult(EFail);												
		}
	else if(  offPeakTimes1->Count() != offPeakTimes2->Count() )
		{
		TestCase().ERR_PRINTF1(_L("Off peak times Count()  does not match !"));
		TestCase().SetTestStepResult(EFail);														
		}
	else if( sendErrorActions1->Default().iError != sendErrorActions2->Default().iError )
		{
		TestCase().ERR_PRINTF1(_L("SendError Actions iError does not match !"));
		TestCase().SetTestStepResult(EFail);																
		}
	else if( sendErrorActions1->Default().iAction != sendErrorActions2->Default().iAction )
		{
		TestCase().ERR_PRINTF1(_L("SendError Actions iAction does not match !"));
		TestCase().SetTestStepResult(EFail);																		
		}
	else if( sendErrorActions1->Default().iRetries != sendErrorActions2->Default().iRetries )
		{
		TestCase().ERR_PRINTF1(_L("SendError Actions iRetries does not match !"));
		TestCase().SetTestStepResult(EFail);																		
		}
		
	else if( sendErrorActions1->Default().iRetrySpacing != sendErrorActions2->Default().iRetrySpacing )
		{
		TestCase().ERR_PRINTF1(_L("SendError Actions iRetrySpacing does not match !"));
		TestCase().SetTestStepResult(EFail);																				
		}
		
	else if( sysAgentActions1->Count() != sysAgentActions2->Count() )
		{
		// failed
		TestCase().ERR_PRINTF1(_L("sysAgentActions1->Count() does not match !"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		// compare variable intervals
		TInt count1 = scheduleSettings1->VariableIntervals().Count();
		TInt count2 = scheduleSettings2->VariableIntervals().Count();
		if (count1 != count2)
			{
			// failed
			TestCase().ERR_PRINTF1(_L("Schedule Settings objects do not match !"));
			TestCase().SetTestStepResult(EFail);
			}
		else
			{
			// compare the actual values
			for (TInt i = 0; i < count1; i++)
				{
				if (scheduleSettings1->VariableIntervals()[i] != scheduleSettings2->VariableIntervals()[i])
					{
					// failed
					TestCase().ERR_PRINTF1(_L("Schedule Settings objects do not match !"));
					TestCase().SetTestStepResult(EFail);
					}
				}
			}
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCompareScheduleSendSettings);
	TestCase().ActionCompletedL(*this);
	}


