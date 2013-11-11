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

#include <s32mem.h>
#include <msvuids.h>
#include <msvids.h>
#include "test_offpeaktime_step.h"

/**	
CMsvOffPeakTimeStep	
*/
CMsvOffPeakTimeStep::CMsvOffPeakTimeStep()
	{
	SetTestStepName(KMsvOffPeakTimeStep);
	}

TVerdict CMsvOffPeakTimeStep::doTestStepL()
	{
	TestConstructor();
	TestGetterSetters();
	TestNextTimeInclusive();
	return TestStepResult();
	}

void CMsvOffPeakTimeStep::TestConstructor()
	{
	iDay = EMonday;
	iHour = 0;
	iMinute = 0;
	iValidityPeriod = (TTimeIntervalMinutes)0;	
	iOffPeakTime = TMsvOffPeakTime();	
	if(!TestValues())
		{
		INFO_PRINTF1(_L("TMsvOffPeakTime:: default constructor failed"));
		SetTestStepResult(EFail);
		}
	
	iDay = ETuesday;
	iHour = 1;
	iMinute = 2;
	iValidityPeriod = (TTimeIntervalMinutes)3;	
	iOffPeakTime = TMsvOffPeakTime(iDay, iHour, iMinute, iValidityPeriod);
	if(!TestValues())
		{
		INFO_PRINTF1(_L("TMsvOffPeakTime:: constructor failed"));
		SetTestStepResult(EFail);
		}
	}

void CMsvOffPeakTimeStep::TestGetterSetters()
	{
	iOffPeakTime.SetDay(EThursday);
	iOffPeakTime.SetHour(3);
	iOffPeakTime.SetMinute(3);
	iOffPeakTime.SetValidityPeriod((TTimeIntervalMinutes)4);
	iDay = EThursday;
	iHour = 3;
	iMinute = 3;
	iValidityPeriod = (TTimeIntervalMinutes)4;
	if(!TestValues())
		{
		INFO_PRINTF1(_L("TMsvOffPeakTime:: getter/setters failed"));
		SetTestStepResult(EFail);
		}
	}
	

void CMsvOffPeakTimeStep::TestNextTimeInclusive()
	{
	TDateTime currentDateTime(2004, EApril, 7, iHour, iMinute, 10, 0);
	
	TTime currentTime(currentDateTime);
	TTime nextTime = iOffPeakTime.NextTimeInclusive(currentTime);
	TDateTime nextDateTime = nextTime.DateTime();
	if(nextDateTime.Day() != 7 || nextDateTime.Hour() != iHour ||
		nextDateTime.Minute() != iMinute)
		{
		INFO_PRINTF1(_L("TMsvOffPeakTime:: NextTimeInclusive failed"));
		SetTestStepResult(EFail);
		}
	
	currentDateTime.SetHour(iHour-1);
	currentTime = currentDateTime;
	nextTime = iOffPeakTime.NextTimeInclusive(currentTime);
	nextDateTime = nextTime.DateTime();
	if(nextDateTime.Day() != 7 || nextDateTime.Hour() != iHour ||
		nextDateTime.Minute() != iMinute)
		{
		INFO_PRINTF1(_L("TMsvOffPeakTime:: NextTimeInclusive failed"));
		SetTestStepResult(EFail);
		}
	
	currentDateTime.SetDay(6);
	currentTime = currentDateTime;
	nextTime = iOffPeakTime.NextTimeInclusive(currentTime);
	 nextDateTime = nextTime.DateTime();
	if(nextDateTime.Day() != 7 || nextDateTime.Hour() != iHour ||
		nextDateTime.Minute() != iMinute)
		{
		INFO_PRINTF1(_L("TMsvOffPeakTime:: NextTimeInclusive failed"));
		SetTestStepResult(EFail);
		}
	
	currentDateTime.SetDay(9);
	currentTime = currentDateTime;
	nextTime = iOffPeakTime.NextTimeInclusive(currentTime);
	nextDateTime = nextTime.DateTime();
	if(nextDateTime.Day() != 14 || nextDateTime.Hour() != iHour ||
		nextDateTime.Minute() != iMinute)
		{
		INFO_PRINTF1(_L("TMsvOffPeakTime:: NextTimeInclusive failed"));
		SetTestStepResult(EFail);
		}		
	
	}
	
TBool CMsvOffPeakTimeStep::TestValues()
	{
	if(iOffPeakTime.Day() != iDay ||
		iOffPeakTime.Minute() != iMinute ||
		iOffPeakTime.Hour() != iHour ||
		iOffPeakTime.ValidityPeriod() != iValidityPeriod)
		{
		return EFalse;
		}
	return ETrue;			
	}


/**
CMsvOffPeakTimeArrayStep
*/		
CMsvOffPeakTimeArrayStep::CMsvOffPeakTimeArrayStep()
	{
	SetTestStepName(KMsvOffPeakTimeArrayStep);
	}

void CMsvOffPeakTimeArrayStep::HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*)
	{
	//do nothing
	}
	
TVerdict CMsvOffPeakTimeArrayStep::doTestStepPreambleL()	
	{
	__UHEAP_MARK;
	iScheduler = new (ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);
	iSession = CMsvSession::OpenSyncL(*this);	
	
	iOffPeakTimes = new (ELeave) CMsvOffPeakTimes();
	TMsvOffPeakTime opTime(EWednesday, 12, 0, 100);
	iOffPeakTimes->AppendL(opTime);
	
	opTime.SetDay(EFriday);
	opTime.SetHour(10);
	iOffPeakTimes->AppendL(opTime);
	return TestStepResult();
	}

TVerdict CMsvOffPeakTimeArrayStep::doTestStepPostambleL()	
	{
	delete iOffPeakTimes;
	delete iSession;
	CActiveScheduler::Install(NULL);
	delete iScheduler;
	__UHEAP_MARKEND;
	return TestStepResult();
	}
			
TVerdict CMsvOffPeakTimeArrayStep::doTestStepL()	
	{
	TestNextOffPeakTime();
	return TestStepResult();
	}


TBool CMsvOffPeakTimeArrayStep::TestOffPeaks(const CMsvOffPeakTimes& aOffPeaks)
	{
	TInt count = iOffPeakTimes->Count();
	if(count != aOffPeaks.Count())
		{
		return EFalse;
		}
		
	while(count--)
		{
		if(!TestOffPeakTime(iOffPeakTimes->At(count), aOffPeaks.At(count)))
			{
			return EFalse;
			}
		}	
	return ETrue;
	}	

TBool CMsvOffPeakTimeArrayStep::TestOffPeakTime(const TMsvOffPeakTime&	source, const TMsvOffPeakTime& dest)
	{
	if(source.Day() != dest.Day() ||
		source.Hour() != dest.Hour() ||
		source.Minute() != dest.Minute() ||
		source.ValidityPeriod() != dest.ValidityPeriod())
		{
		return EFalse;
		}
	return ETrue;	
	}
	
void CMsvOffPeakTimeArrayStep::TestNextOffPeakTime()	
	{
	TDateTime currentDateTime(2004, EApril, 7, 5, 5, 10, 0);
	TTime currentTime = currentDateTime;
	TMsvOffPeakTime nextOffPeak;
	TTime nextTime;
	
	if(iOffPeakTimes->GetNextOffPeakTime(currentTime, nextOffPeak, nextTime) != KErrNone)
		{
		INFO_PRINTF1(_L("CMsvOffPeakTimes:: GetNextOffPeakTime failed"));
		SetTestStepResult(EFail);
		}
	if(!TestOffPeakTime(nextOffPeak, iOffPeakTimes->At(1)))
		{
		INFO_PRINTF1(_L("CMsvOffPeakTimes:: GetNextOffPeakTime failed"));
		SetTestStepResult(EFail);
		}
	
	TDateTime nextDateTime = nextTime.DateTime();
	if(nextDateTime.Day() != 8 || nextDateTime.Hour() != 10 ||
		nextDateTime.Minute() != 0)
		{
		INFO_PRINTF1(_L("CMsvOffPeakTimes:: GetNextOffPeakTime failed"));
		SetTestStepResult(EFail);		
		}
	}
