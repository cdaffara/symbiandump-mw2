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

#ifndef __TEST_SYSAGENTACTIONS_STEP_H__
#define __TEST_SYSAGENTACTIONS_STEP_H__

#include <test/testexecutestepbase.h>


#include <msvapi.h>
#include <msvsysagentaction.h>

/**
@internalTechnology
*/
class TMsvSysAgentConditionExt : public TMsvCondition 
	{
public:
	TBool iMet;
	};

/**
@internalTechnology	
*/
class CTlsBunchOfStuff : public CArrayFixFlat<TMsvSysAgentConditionExt>
	{
public:
	CTlsBunchOfStuff();
	TInt iConnectResult;
	};


_LIT(KMsvSysAgentActionsStep, "MsvSysAgentActionsStep");
	
/**
Test class CMsvSysAgentActions

@internalTechnology
*/
class CMsvSysAgentActionsStep : public CTestStep, public MMsvSessionObserver
	{
public:
	CMsvSysAgentActionsStep();
	TInt GetStates(const CArrayFixFlat<TUid>& aUids, CArrayFixFlat<TInt>& aStates);
	//methods from CTestStep
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
	
	//method from MMsvSessionObserver
	virtual void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny*, TAny*);	
	
private:
	void SetConfigurationL();
	void CompareResult(CMsvSysAgentActions* aActions, TBool aCheckIndividual = EFalse);
	TBool CompareErrorAction(const TMsvSendErrorAction& aAction1, const TMsvSendErrorAction& aAction2); 	
	TBool CompareConditionAction(const TMsvSysAgentConditionAction& aCondAction1, const TMsvSysAgentConditionAction& aCondAction2);
	
private:
	CMsvSysAgentActions* iActions;
	CTlsBunchOfStuff* 	iStuff;
	CActiveScheduler* 	iScheduler;
	CMsvSession* 		iSession;
	TInt 				iExpectedErrorActionIndex;	
	TBool				iConditionMet;
	RArray<TBool> iDefinedValue;
	};

#endif
