// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file 
// [TestStep Name]
// StopRamUsageTimer
// [Paramaters]
// EventStartCode			<input>	: Start event code which maps to the RAM usage value
// when tacking of RAM usage got started.
// EventStopCode			<input> : Stop event code which maps to the RAM usage value
// when tacking of RAM usage got stopped.
// Stops periodical measurement of RAM memory used and logs the RAM usage information
// [APIs Used]
// 
//

// User includes
#include "T_StopRamUsageTimer.h"
#include <t_utilsenumconverter.h>
#include <t_testinstrumentation.h>


// Literals used
_LIT(KEventStartCode, "EventStartCode");
_LIT(KEventStopCode, "EventStopCode");
/**
  Function 		: CT_StopRamUsageTimer
  Description 	: Constructor
  @return 		: none
*/
CT_StopRamUsageTimer::CT_StopRamUsageTimer(CT_MsgSharedDataPop& aSharedDataPOP)
:CT_MsgAsyncStepPOP(aSharedDataPOP)
	{
	SetTestStepName(KStopRamUsageTimer);
	}
	
/**
  Function 		: ~CT_StopRamUsageTimer
  Description 	: Destructor
  @return 		: N/A
*/
CT_StopRamUsageTimer::~CT_StopRamUsageTimer()
	{}
	

/**
  Function 		: doTestStepL
  Description 	: Stop the periodic Timer
  @return 		: TVerdict Test result
*/
TVerdict CT_StopRamUsageTimer::doTestStepL()
	{
	INFO_PRINTF1(_L("TestStep :StopRamUsageTimer"));
	TPtrC startCodeFromIni;
	if(!GetStringFromConfig(ConfigSection(), KEventStartCode, startCodeFromIni))
		{
		ERR_PRINTF1(_L("Event Start code is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		TInstrumentationPoint starteventcode = CT_MsgUtilsEnumConverter::ConvertDesToEventCode(startCodeFromIni);
		
		TPtrC stopCodeFromIni;
		if(!GetStringFromConfig(ConfigSection(), KEventStopCode, stopCodeFromIni))
			{
			ERR_PRINTF1(_L("Event Stop code is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			TInstrumentationPoint stopeventcode = CT_MsgUtilsEnumConverter::ConvertDesToEventCode(stopCodeFromIni);
			iSharedDataPOP.iPeriodic->Cancel();

			INFO_PRINTF2(_L("Minimum Ram usage value is %d KB "), iSharedDataPOP.iMinRamValue);
			INFO_PRINTF2(_L("Maximum Ram usage value is %d KB "), iSharedDataPOP.iMaxRamValue);
			INFO_PRINTF2(_L("Start Ram usage value is %d KB "), iSharedDataPOP.iStartRamValue);
			INFO_PRINTF2(_L("End Ram usage value is %d KB "), iSharedDataPOP.iEndRamValue);

			TInt maxRamUsage = ( iSharedDataPOP.iMaxRamValue - iSharedDataPOP.iStartRamValue );
			TInt extremeRamUsage = ( iSharedDataPOP.iMaxRamValue - iSharedDataPOP.iMinRamValue );
			TInt actualRamUsage = ( iSharedDataPOP.iEndRamValue - iSharedDataPOP.iStartRamValue );
			
			INFO_PRINTF2(_L("Maximum(Mx-St) Ram usage diff value is %d KB "), maxRamUsage );
			INFO_PRINTF2(_L("Extreme(Mx-Mn) Ram usage diff value is %d KB "), extremeRamUsage );
			INFO_PRINTF2(_L("Actual(En-St) Ram usage diff value is %d KB "), actualRamUsage );
			if( starteventcode || stopeventcode )
				{
				INFO_PRINTF5(_L("EVENT_LOG_INFORMATION,%d,%d,%d,%d"),0,0,starteventcode,iSharedDataPOP.iStartRamValue);
				INFO_PRINTF5(_L("EVENT_LOG_INFORMATION,%d,%d,%d,%d"),0,0,stopeventcode,iSharedDataPOP.iMaxRamValue);
				}
			else
				{
				ERR_PRINTF1(_L("**ERROR**: Bad Event code. Check t_testinstrumentation.h for valid event code"));					
				}	
			}		
		}
	return TestStepResult();
	}

/**
  Function 		: ProgressL
  Description 	: Displays the progress information of the asynchronous operation
  @return 		: void
*/
void CT_StopRamUsageTimer::ProgressL(TBool /*aFinal*/)
	{}

/**
  Function 		: CancelStep
  Description 	: Cancels the asynchronous operation
  @return 		: void
*/
void CT_StopRamUsageTimer::CancelStep()
	{}
