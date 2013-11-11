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

#ifndef __TEST_SCHEDULESETTINGS_STEP_H__
#define __TEST_SCHEDULESETTINGS_STEP_H__

#include <test/testexecutestepbase.h>
#include <msvschedulesend.h>
#include <msvschedulesettings.h>
#include <msvapi.h>

 

_LIT(KMsvScheduleSettingsStep, "MsvScheduleSettingsStep");

/**	
Testing the class CMsvScheduleSettings

@internalTechnology	
*/
class CMsvScheduleSettingsStep : public CTestStep, public MMsvSessionObserver
	{
public:
	CMsvScheduleSettingsStep();
	
	//methods from CTestStep
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
	
	//method from MMsvSessionObserver
	virtual void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny*, TAny*);		

private:
	TBool	TestGetValues(const CMsvScheduleSettings* aScheduleSettings);	
	void	TestSetValuesL();
	void	TestReset();
	void	TestStoreRestoreL();
	
private:	
	CMsvScheduleSettings* iScheduleSettings;
	CActiveScheduler* 	iScheduler;
	CMsvSession* 		iSession;
	
	TInt					iPriority;
	TTimeIntervalMinutes	iValidityPeriod;
	TIntervalType			iIntervalType;	
	TTimeIntervalMicroSeconds32 iLatency;		
	TTimeIntervalMinutes	iPendingConditionsTimeout;

	TTimeIntervalSeconds iLongInterval;
	TTimeIntervalSeconds iShortInterval;
	CArrayFixFlat<TTimeIntervalSeconds>* iVariableIntervals;
	};
	

#endif
