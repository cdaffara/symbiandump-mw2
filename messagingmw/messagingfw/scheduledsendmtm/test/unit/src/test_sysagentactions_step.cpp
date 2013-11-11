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

#include "test_sysagentactions_step.h"
#include <msvsysagentaction.h>
#include <msvschedulesend.h>
#include <msvapi.h>
#include <msvuids.h>
#include <msvids.h>

#include <e32property.h>

#ifdef __EXE__
//The Dll:: methods are not available when building an EXE.
//In order to use the same code base for EXEs, we provide 
//the implementation ourselves. Note that this is not a per
//thread pointer, so don't run the test steps concurrently.
static TAny* gTlsData;
TInt Dll::SetTls(TAny* aPtr)
	{
	gTlsData = aPtr;
	return KErrNone;
	}
TAny* Dll::Tls()
	{
	return gTlsData;
	}	
#endif

#define INIT_STUFF_FROM_TLS CTlsBunchOfStuff*_stuff=(CTlsBunchOfStuff*)Dll::Tls()

CTlsBunchOfStuff::CTlsBunchOfStuff()
	:CArrayFixFlat<TMsvSysAgentConditionExt>(KMsvSchsendArrayGrowth)
	{
	}

	
/**
Test Step class implementation
*/		
CMsvSysAgentActionsStep::CMsvSysAgentActionsStep()
	{
	SetTestStepName(KMsvSysAgentActionsStep);
	}

TVerdict CMsvSysAgentActionsStep::doTestStepPreambleL()
	{
	__UHEAP_MARK;
	iConditionMet = ETrue;
	iExpectedErrorActionIndex = -1;
	
	iScheduler = new (ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);
	
	iSession = CMsvSession::OpenSyncL(*this);
	
	iActions = new (ELeave) CMsvSysAgentActions();
	iStuff = new (ELeave) CTlsBunchOfStuff();
	Dll::SetTls(iStuff);
	SetConfigurationL();
	return TestStepResult();
	}

TVerdict CMsvSysAgentActionsStep::doTestStepPostambleL()
	{
	/* Delete the P&S entry only if it is defined locally */
	TInt count = iDefinedValue.Count();
	while(count--)
		{
		TInt localUid = 0;
		TInt testState;
		//Get the Uid
		localUid = iActions->At(count).iCondition.iVariable.iUid;
		if(KErrNone == RProperty::Get(KUidSystemCategory, localUid, testState))
 			{
 			// Delete P&S variables
 			User::LeaveIfError(RProperty::Delete(KUidSystemCategory, localUid));
 			}
		}
	iDefinedValue.Reset();
	delete iActions;
	delete iStuff;
	delete iSession;
	CActiveScheduler::Install(NULL);
	delete iScheduler;
	__UHEAP_MARKEND;
	return TestStepResult();
	}
			
TVerdict CMsvSysAgentActionsStep::doTestStepL()
	{
	
	//test ConditionsMetL method
	TMsvSendErrorAction expectedErrorAction;
	if(iExpectedErrorActionIndex == -1)
		{
		expectedErrorAction = iActions->iDefault;
		}
	else
		{
		expectedErrorAction = iActions->At(iExpectedErrorActionIndex).iErrorAction;
		}
		
	TMsvSendErrorAction errorAction;
	TBool conditionMet = iActions->ConditionsMetL(errorAction);
	if(conditionMet != iConditionMet ||	!CompareErrorAction(errorAction, expectedErrorAction))
		{
		INFO_PRINTF1(_L("CMsvSysAgentActionsStep: ConditionMetL Failed"));
		SetTestStepResult(EFail);
		}
	
	return TestStepResult();
	}	

void CMsvSysAgentActionsStep::CompareResult(CMsvSysAgentActions* aActions, TBool aCheckIndividual)
	{
	if(iActions->Count() != aActions->Count() || 
		!CompareErrorAction(iActions->iDefault, aActions->iDefault))
		{
		INFO_PRINTF1(_L("CMsvSysAgentActionsStep: [StoreL/RestoreL] default error action or condition actions count mismatch"));
		SetTestStepResult(EFail);
		}
	else if(aCheckIndividual)
		{
		TInt count = iActions->Count();
		while(count--)
			{
			if(!CompareConditionAction(iActions->At(count), aActions->At(count)))
				{
				INFO_PRINTF1(_L("CMsvSysAgentActionsStep: [StoreL/RestoreL] condition action mismatch"));
				SetTestStepResult(EFail);
				break;
				}
			}
		}
	}
		
		
TBool CMsvSysAgentActionsStep::CompareErrorAction(const TMsvSendErrorAction& aAction1, const TMsvSendErrorAction& aAction2)
	{
	if(aAction1.iAction != aAction2.iAction ||
		aAction1.iError != aAction2.iError ||
		aAction1.iRetries != aAction2.iRetries ||
		aAction1.iRetrySpacing != aAction2.iRetrySpacing ||
		aAction1.MaxRetries() != aAction2.MaxRetries())
		{
		return EFalse;
		}
	return ETrue;	
	}
	 	
TBool CMsvSysAgentActionsStep::CompareConditionAction(const TMsvSysAgentConditionAction& aCondAction1, const TMsvSysAgentConditionAction& aCondAction2)
	{
	if(aCondAction1.iUseDefaultSysAgentAction != aCondAction2.iUseDefaultSysAgentAction ||
		!CompareErrorAction(aCondAction1.iErrorAction, aCondAction2.iErrorAction) ||
		aCondAction1.iCondition.iVariable != aCondAction2.iCondition.iVariable ||
		aCondAction1.iCondition.iState != aCondAction2.iCondition.iState ||
		aCondAction1.iCondition.iType != aCondAction2.iCondition.iType)
		{
		return EFalse;
		}
	return ETrue;	
	}

	
void CMsvSysAgentActionsStep::HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*)
	{
	//do nothing
	}

_LIT(KCfgConditionActionTotal, "ConditionActionTotal");
_LIT(KCfgDefaultErrorActionIndex, "DefaultErrorActionIndex");
_LIT(KCfgSystemAgentConnectResult, "SystemAgentConnectResult");
_LIT(KCfgExpectedErrorActionIndex, "ExpectedErrorActionIndex");


_LIT(KCfgAgentCondition, "AgentCondition");
_LIT(KCfgAgentConditionVariable, ".Variable");
_LIT(KCfgAgentConditionState, ".State");
_LIT(KCfgAgentConditionType, ".Type");
_LIT(KCfgAgentConditionMet, ".Met");

_LIT(KCfgErrorAction, "ErrorAction");
_LIT(KCfgErrorActionError, ".Error");
_LIT(KCfgErrorActionAction, ".Action");
_LIT(KCfgErrorActionRetries, ".Retries");
_LIT(KCfgErrorActionRetrySpacing, ".RetrySpacing");
_LIT(KCfgErrorActionMaxRetries, ".MaxRetries");

_LIT(KCfgConditionActionUseDefault, "UseDefaultErrorAction");
const TInt KMaxSectionLength = 100;

void CMsvSysAgentActionsStep::SetConfigurationL()
	{	
	TBuf<KMaxSectionLength> sectionName;	
	TInt count = 0;
	TInt index = 0;
	if(GetIntFromConfig(ConfigSection(), KCfgConditionActionTotal, count))
		{
		INFO_PRINTF2(_L("CMsvSysAgentActionsStep: Actions Total: [%d]."), count);
		}
	if(GetIntFromConfig(ConfigSection(), KCfgDefaultErrorActionIndex, index))
		{
		INFO_PRINTF2(_L("CMsvSysAgentActionsStep: Default error action index: [%d]."), index);
		}
	if(GetIntFromConfig(ConfigSection(), KCfgSystemAgentConnectResult, iStuff->iConnectResult))
		{
		INFO_PRINTF2(_L("CMsvSysAgentActionsStep: System Agent Connect Result: [%d]."), iStuff->iConnectResult);
		}
	if(GetIntFromConfig(ConfigSection(), KCfgExpectedErrorActionIndex, iExpectedErrorActionIndex))
		{
		INFO_PRINTF2(_L("CMsvSysAgentActionsStep: Expected Error Action Index: [%d]."), iExpectedErrorActionIndex);
		}		
	
	TInt i=0;	
	while(i < count)
		{
		TMsvSysAgentConditionExt condition; 
		sectionName.Copy(KCfgAgentCondition);
		sectionName.AppendNum(i);
		TInt pos = sectionName.Length();
		sectionName.Append(KCfgAgentConditionVariable);
		TInt variable = 0;

		if(GetHexFromConfig(ConfigSection(), sectionName, variable))
			{
			INFO_PRINTF3(_L("CMsvSysAgentActionsStep: Section Name: [%S], [%d]."), &sectionName, variable);
			}
		
		condition.iVariable.iUid = variable;

		sectionName.Replace(pos, KCfgAgentConditionVariable().Length(), KCfgAgentConditionState);	
		if(GetIntFromConfig(ConfigSection(), sectionName, condition.iState))
			{
			INFO_PRINTF3(_L("CMsvSysAgentActionsStep: Section Name: [%S], [%d]."), &sectionName, condition.iState);
			}
		
		variable = 0;
		sectionName.Replace(pos, KCfgAgentConditionState().Length(), KCfgAgentConditionType);
		if(GetIntFromConfig(ConfigSection(), sectionName, variable))
			{
			INFO_PRINTF3(_L("CMsvSysAgentActionsStep: Section Name: [%S], [%d]."), &sectionName, variable);
			}
		//ESysAgentEquals=32, but EMsvSchSendEquals start with 0
		condition.iType = static_cast<TMsvCondition::TMsvSchSendConditionType>(variable - 32);

		
		sectionName.Replace(pos, KCfgAgentConditionType().Length(), KCfgAgentConditionMet);
		if(GetBoolFromConfig(ConfigSection(), sectionName, condition.iMet))
			{
			INFO_PRINTF3(_L("CMsvSysAgentActionsStep: Section Name: [%S], [%d]."), &sectionName, condition.iMet);
			}
		iConditionMet &= condition.iMet;


		/* Define an entry in P&S only if the variable is not defined before.
		   Change the state in the P&S entry if iMet is not true*/
		   
		TInt state = condition.iState;

		//Testing for a changed condition
		if(!condition.iMet)
			state = 1;	
			
		// check P&S variables
		TInt testState;
		if(KErrNone != RProperty::Get(KUidSystemCategory, condition.iVariable.iUid, testState))
 			{
 			// create P&S variables
 			RProperty::Define(KUidSystemCategory, condition.iVariable.iUid, RProperty::EInt);
 			User::LeaveIfError(iDefinedValue.Append(ETrue));
 			}

		if(KErrNone != RProperty::Set(KUidSystemCategory, condition.iVariable.iUid, state))
 			{
 			INFO_PRINTF1(_L("CMsvSysAgentActionsStep: SetConfigurationL Failed"));
 			}

		
		TMsvSendErrorAction action;
		sectionName.Copy(KCfgErrorAction);
		sectionName.AppendNum(i);
		pos = sectionName.Length();
		sectionName.Append(KCfgErrorActionError);
		if(GetIntFromConfig(ConfigSection(), sectionName, action.iError))
			{
			INFO_PRINTF3(_L("CMsvSysAgentActionsStep: Section Name: [%S], [%d]."), &sectionName, action.iError);
			}
		
		variable = 0;
		sectionName.Replace(pos, KCfgErrorActionError().Length(), KCfgErrorActionAction);
		if(GetIntFromConfig(ConfigSection(), sectionName, variable))
			{
			INFO_PRINTF3(_L("CMsvSysAgentActionsStep: Section Name: [%S], [%d]."), &sectionName, variable);
			}
		action.iAction = static_cast<TMsvSendAction>(variable);	
		
		variable = 0;
		sectionName.Replace(pos, KCfgErrorActionAction().Length(), KCfgErrorActionRetries);			
		if(GetIntFromConfig(ConfigSection(), sectionName, variable))
			{
			INFO_PRINTF3(_L("CMsvSysAgentActionsStep: Section Name: [%S], [%d]."), &sectionName, variable);
			}
		action.iRetries = static_cast<TMsvSendRetries>(variable);
		
		variable = 0;
		sectionName.Replace(pos, KCfgErrorActionRetries().Length(), KCfgErrorActionRetrySpacing);	
		if(GetIntFromConfig(ConfigSection(), sectionName, variable))
			{
			INFO_PRINTF3(_L("CMsvSysAgentActionsStep: Section Name: [%S], [%d]."), &sectionName, variable);
			}
		action.iRetrySpacing = static_cast<TMsvSendRetrySpacing>(variable);
		
		sectionName.Replace(pos, KCfgErrorActionRetrySpacing().Length(), KCfgErrorActionMaxRetries);
		TInt maxRetry = 0;
		if(GetIntFromConfig(ConfigSection(), sectionName, maxRetry))
			{
			INFO_PRINTF3(_L("CMsvSysAgentActionsStep: Section Name: [%S], [%d]."), &sectionName,maxRetry);
			}
		action.SetMaxRetries((TInt16)maxRetry);
		
		TMsvSysAgentConditionAction conditionAction;
		conditionAction.iCondition = condition;
		conditionAction.iErrorAction = action;									
		sectionName.Copy(KCfgConditionActionUseDefault);
		sectionName.AppendNum(i);
		if(GetBoolFromConfig(ConfigSection(), sectionName, conditionAction.iUseDefaultSysAgentAction))
			{
			INFO_PRINTF3(_L("CMsvSysAgentActionsStep: Section Name: [%S], [%d]."), &sectionName, conditionAction.iUseDefaultSysAgentAction);
			}
		iActions->AppendL(conditionAction);
		iStuff->AppendL(condition);

		if(i == index)
			{
			iActions->iDefault = action;
			}
		++i;					 					
		}
		
	}
	

