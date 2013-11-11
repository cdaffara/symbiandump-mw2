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

#ifndef __TEST_OFFPEAKTIME_STEP_H__
#define __TEST_OFFPEAKTIME_STEP_H__

#include <test/testexecutestepbase.h>
#include <msvschedulesend.h>
#include <msvoffpeaktime.h>
#include <msvapi.h>

_LIT(KMsvOffPeakTimeStep, "MsvOffPeakTimeStep");
	
/**
Testing the class TMsvOffPeakTime

@internalTechnology
*/
class CMsvOffPeakTimeStep : public CTestStep
	{
public:
	CMsvOffPeakTimeStep();
	
	//methods from CTestStep
	virtual TVerdict doTestStepL();	
	
private:
	void TestConstructor();	
	void TestGetterSetters();
	void TestNextTimeInclusive();
	TBool TestValues();	
private:
	TDay iDay;
	TInt8 iHour;						
	TInt8 iMinute;							
	TTimeIntervalMinutes iValidityPeriod;
	TMsvOffPeakTime	iOffPeakTime;	
	};

	
_LIT(KMsvOffPeakTimeArrayStep, "MsvOffPeakTimeArrayStep");

/**
Testing the class CMsvOffPeakTimes

@internalTechnology
*/
class CMsvOffPeakTimeArrayStep : public CTestStep,  public MMsvSessionObserver
	{
public:
	CMsvOffPeakTimeArrayStep();
	
	//methods from MMsvSessionObserver
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*);
	
	//methods from CTestStep
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
	
private:
	TBool TestOffPeaks(const CMsvOffPeakTimes& aOffPeaks);
	TBool TestOffPeakTime(const TMsvOffPeakTime& source, const TMsvOffPeakTime& dest);
	void TestNextOffPeakTime();
private:
	CActiveScheduler* 	iScheduler;
	CMsvSession* 		iSession;
	CMsvOffPeakTimes*	iOffPeakTimes;		
	};
#endif
