// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
*/
 
#include "CSchSendScriptUtils.h"
#include "CConfigFileParserUtils.h"

#include <msvsysagentaction.h>
#include <msvschedulesettings.h>
#include <e32property.h>

_LIT(KCfgConditionActionTotal, "ConditionActionTotal");
_LIT(KCfgDefaultErrorActionIndex, "DefaultErrorActionIndex");

_LIT(KCfgAgentCondition, "AgentCondition");
_LIT(KCfgAgentConditionVariable, ".Variable");
_LIT(KCfgAgentConditionState, ".State");
_LIT(KCfgAgentConditionType, ".Type");

_LIT(KCfgConditionErrorAction, "ConditionErrorAction");

_LIT(KCfgErrorActionError, ".Error");
_LIT(KCfgErrorActionAction, ".Action");
_LIT(KCfgErrorActionRetries, ".Retries");
_LIT(KCfgErrorActionRetrySpacing, ".RetrySpacing");
_LIT(KCfgErrorActionMaxRetries, ".MaxRetries");
_LIT(KCfgConditionActionUseDefault, "UseDefaultErrorAction");

_LIT(KCfgPendingConditionTimeOut, "PendingConditionTimeOut");

_LIT(KCfgValidityPeriod, "ValidityPeriod");
_LIT(KCfgIntervalType, "IntervalType");
_LIT(KCfgShortInterval, "ShortInterval");
_LIT(KCfgLongInterval, "LongInterval");
_LIT(KCfgLatency, "Latency");
_LIT(KCfgVariableIntervalCount, "VariableIntervalCount");
_LIT(KCfgVariableInterval, "VariableInterval");

/**
The method reads the CMsvSysAgentActions settings from the configuration file.

@param aSettingsFile		The configuration file name
@param aSysAgentActions		A reference to the CMsvSysAgentActions object which is to be set from the configuration file.

@internalTechnology
*/
void CSchSendScriptUtils::ReadScheduleSettingsFromConfigurationFileL(const TDesC& aSettingsFile, CMsvSysAgentActions& aSysAgentActions)
	{
	CConfigFileParserUtils* scriptFileParser = CConfigFileParserUtils::NewL(aSettingsFile);
	CleanupStack::PushL(scriptFileParser);

	TInt count = 0;
	TInt defaultIndex = 0;

	// Total number of Condition Actions
	scriptFileParser->GetFieldAsInteger(KCfgConditionActionTotal, count);
				
	// Default Error Action Index
	scriptFileParser->GetFieldAsInteger(KCfgDefaultErrorActionIndex, defaultIndex);
			
	TInt i=0;
	TBuf<100> sectionName;	
	while(i < count)
		{
		
		TMsvCondition condition; 

		sectionName.Copy(KCfgAgentCondition);
		sectionName.AppendNum(i);
		TInt pos = sectionName.Length();
		sectionName.Append(KCfgAgentConditionVariable);
		TInt variable = 0;
		scriptFileParser->GetFieldAsInteger(sectionName, variable);

		condition.iVariable.iUid = variable;
			
		sectionName.Replace(pos, KCfgAgentConditionVariable().Length(), KCfgAgentConditionState);	
		scriptFileParser->GetFieldAsInteger(sectionName, condition.iState);
			
		variable = 0;
		sectionName.Replace(pos, KCfgAgentConditionState().Length(), KCfgAgentConditionType);
		scriptFileParser->GetFieldAsInteger(sectionName, variable);

		variable -= 32;
		condition.iType = static_cast<TMsvCondition::TMsvSchSendConditionType>(variable); 
			
		TMsvSendErrorAction action;
		sectionName.Copy(KCfgConditionErrorAction);
		sectionName.AppendNum(i);
		pos = sectionName.Length();
		sectionName.Append(KCfgErrorActionError);
		scriptFileParser->GetFieldAsInteger(sectionName, action.iError);
			
		variable = 0;
		sectionName.Replace(pos, KCfgErrorActionError().Length(), KCfgErrorActionAction);
		scriptFileParser->GetFieldAsInteger(sectionName, variable);
		action.iAction = static_cast<TMsvSendAction>(variable);	
		
		variable = 0;
		sectionName.Replace(pos, KCfgErrorActionAction().Length(), KCfgErrorActionRetries);			
		scriptFileParser->GetFieldAsInteger(sectionName, variable);
		action.iRetries = static_cast<TMsvSendRetries>(variable);
		
		variable = 0;
		sectionName.Replace(pos, KCfgErrorActionRetries().Length(), KCfgErrorActionRetrySpacing);	
		scriptFileParser->GetFieldAsInteger(sectionName, variable);
		action.iRetrySpacing = static_cast<TMsvSendRetrySpacing>(variable);
		
		variable = 0;
		sectionName.Replace(pos, KCfgErrorActionRetrySpacing().Length(), KCfgErrorActionMaxRetries);
		scriptFileParser->GetFieldAsInteger(sectionName, variable);
		action.SetMaxRetries( static_cast<TInt16>(variable) );

		TMsvSysAgentConditionAction conditionAction;
		conditionAction.iCondition = condition;
		conditionAction.iErrorAction = action;	 						
		
		sectionName.Copy(KCfgConditionActionUseDefault);
		sectionName.AppendNum(i);
		scriptFileParser->GetFieldAsInteger(sectionName, conditionAction.iUseDefaultSysAgentAction);		
		
		aSysAgentActions.AppendL(conditionAction);
	
		if(i == defaultIndex)
			{
			aSysAgentActions.iDefault = action;
			}
		++i;					 					
		}
	CleanupStack::PopAndDestroy(scriptFileParser);
	}

/**
The method reads the CMsvScheduleSettings settings from the configuration file.

@param aSettingsFile		The configuration file name
@param aScheduleSettings	A reference to the CMsvScheduleSettings object which is to be set from the configuration file.

@internalTechnology
*/	
void CSchSendScriptUtils::ReadScheduleSettingsFromConfigurationFileL(const TDesC& aSettingsFile, CMsvScheduleSettings& aScheduleSettings)
	{
	CConfigFileParserUtils* scriptFileParser = CConfigFileParserUtils::NewL(aSettingsFile);
	CleanupStack::PushL(scriptFileParser);
	
	TInt variable = 0;
	TTimeIntervalMinutes minutes;
	TTimeIntervalSeconds seconds;
	TTimeIntervalMicroSeconds32 microSeconds;
	if(scriptFileParser->GetFieldAsInteger(KCfgValidityPeriod, variable) == KErrNone)
		{
		minutes = variable;
		aScheduleSettings.SetValidityPeriod(minutes);
		}
		
	variable = 0;
	if(scriptFileParser->GetFieldAsInteger(KCfgIntervalType, variable) == KErrNone)
		{
		TIntervalType intervalType = static_cast<TIntervalType>(variable);
		aScheduleSettings.SetIntervalType(intervalType);
		}
	
	variable = 0;
	if(scriptFileParser->GetFieldAsInteger(KCfgShortInterval, variable) == KErrNone)
		{
		seconds = variable;
		aScheduleSettings.SetShortInterval(seconds);
		}
	
	variable = 0;
	if(scriptFileParser->GetFieldAsInteger(KCfgLongInterval, variable) == KErrNone)
		{
		seconds = variable;
		aScheduleSettings.SetLongInterval(seconds);
		}
	
	variable = 0;
	if(scriptFileParser->GetFieldAsInteger(KCfgLatency, variable) == KErrNone)
		{
		microSeconds = variable;
		aScheduleSettings.SetLatency(microSeconds);
		}
					
	CArrayFixFlat<TTimeIntervalSeconds>* variableIntervals = new (ELeave) CArrayFixFlat<TTimeIntervalSeconds>(1);
	CleanupStack::PushL(variableIntervals);
	
	TInt count = 0;
	scriptFileParser->GetFieldAsInteger(KCfgVariableIntervalCount, count);
	TBuf<100> sectionName;	
	TInt i=0;
	while(i < count)
		{
		sectionName.Copy(KCfgVariableInterval);
		sectionName.AppendNum(i);		
		scriptFileParser->GetFieldAsInteger(sectionName, variable);
		seconds = variable;
		variableIntervals->AppendL(seconds);
		++i;
		}	
	
	aScheduleSettings.SetVariableIntervalsL(*variableIntervals);
	CleanupStack::PopAndDestroy(variableIntervals);
	
	variable = 0;	
	if(scriptFileParser->GetFieldAsInteger(KCfgPendingConditionTimeOut, variable) == KErrNone)
		{
		minutes = variable;
		aScheduleSettings.SetPendingConditionsTimeout(minutes);
		}
	
	CleanupStack::PopAndDestroy(scriptFileParser);
	}

