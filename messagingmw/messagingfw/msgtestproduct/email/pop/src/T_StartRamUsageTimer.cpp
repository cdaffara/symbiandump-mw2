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
// StartRamUsageTimer
// [Paramaters]
// IntervalTimeInMicroSecs			<input>	: The interval between events generated
// after the initial delay, in microseconds
// Starts periodical measurement of RAM memory used and updates the information kept
// in POP testserver.
// [APIs Used]
// 
//

// User includes 
#include "T_StartRamUsageTimer.h"
#include "T_MsgSharedDataPop.h"

// Epoc includes
#include <hal.h>
#include <hal_data.h>

// Literals used
_LIT(KIntervalTimeInMicroSecs, "IntervalTimeInMicroSecs");

/**
  Function 		: CT_StartRamUsageTimer
  Description 	: Constructor
  @return 		: none
*/
CT_StartRamUsageTimer::CT_StartRamUsageTimer(CT_MsgSharedDataPop& aSharedDataPOP)
:CT_MsgAsyncStepPOP(aSharedDataPOP)
	{
	SetTestStepName(KStartRamUsageTimer);
	}

/**
  Function 		: ~CT_StartRamUsageTimer
  Description 	: Destructor
  @return 		: N/A
*/
CT_StartRamUsageTimer::~CT_StartRamUsageTimer()
	{}
	

/**
  Function 		: doTestStepL
  Description 	: Generate a periodic Timer event and handle the RAM usage callback function
  @return 		: TVerdict Test result
*/
TVerdict CT_StartRamUsageTimer::doTestStepL()
	{
	INFO_PRINTF1(_L("TestStep :StartRamUsageTimer"));
	TInt intervalTime = 0;	
	if(!GetIntFromConfig(ConfigSection(),KIntervalTimeInMicroSecs,intervalTime))
		{
		ERR_PRINTF1(_L("Interval time is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		iSharedDataPOP.iPeriodic->Start(intervalTime, intervalTime, TCallBack(RamUsage,this));
		}
	return TestStepResult();	
	}

/**
  Function 		: RamUsage
  Description 	: Call non-static method DoRamUsage
  @return 		: TInt
*/
TInt CT_StartRamUsageTimer::RamUsage(TAny* aObject)
	{
	// Cast, and call non-static function
	((CT_StartRamUsageTimer *)aObject)->DoRamUsage();	
	return 1;	
	}

/**
  Function 		: DoRamUsage
  Description 	: Calculate the RAM usage value and percentage at a particular instance 
  @return 		: void
*/	
void CT_StartRamUsageTimer::DoRamUsage()
	{
	
	TInt fullRamValue;
	TInt ramFreeValue;
	
	HAL 	data;
	HALData haldata;
	
	data.Get(haldata.EMemoryRAM, fullRamValue);
	data.Get(haldata.EMemoryRAMFree, ramFreeValue);	
	
	TInt ramUsage = ((fullRamValue - ramFreeValue)/(1024));
	
	if ( iSharedDataPOP.iMark != EFalse )
		{
		iSharedDataPOP.iStartRamValue = ramUsage;
		iSharedDataPOP.iMinRamValue = ramUsage;
		iSharedDataPOP.iMaxRamValue = ramUsage;
		iSharedDataPOP.iMark = EFalse;
		}
	
	if ( iSharedDataPOP.iMinRamValue > ramUsage )
		{
		iSharedDataPOP.iMinRamValue = ramUsage;
		}
		
	if ( iSharedDataPOP.iMaxRamValue < ramUsage )
		{
		iSharedDataPOP.iMaxRamValue = ramUsage;
		}
	
	iSharedDataPOP.iEndRamValue = ramUsage;
	}
	

	
/**
  Function 		: ProgressL
  Description 	: Displays the progress information of the asynchronous operation
  @return 		: void
*/
void CT_StartRamUsageTimer::ProgressL(TBool /*aFinal*/)
	{}

/**
  Function 		: CancelStep
  Description 	: Cancels the asynchronous operation
  @return 		: void
*/
void CT_StartRamUsageTimer::CancelStep()
	{}
