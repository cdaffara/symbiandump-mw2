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

#include <centralrepository.h>
#include <s32mem.h>

#include <tmsvschedulesettingsutils.h>
#include <msvschedulesettings.h>
#include <msvoffpeaktime.h>
#include <msvsenderroraction.h>
#include <msvsysagentaction.h>


const TUint32 KIncrementSettingList = 0x00000010;
const TInt KMaxVariableIntervals = 4096;
const TInt KMaxSettingList = 256;
const TInt KSysAgentActionsVersion = 1;
const TInt KScheduleSettingsVersion = 1;
const TInt KOffPeakTimesVersion = 1;
const TInt KOffPeakTimeVersion = 1;
const TInt KSendErrorActionVersion = 1;
const TInt KSendErrorActionsVersion = 1;
const TInt KSysAgentConditionActionVersion = 1;



/**
Saves Schedule Settings to the Central Repository.
This function must be used within a CenRep Transaction.

@param	aScheduleSettings
Schedule settings

@param	aRepository
Repository to save settings to.
*/
EXPORT_C void TMsvScheduleSettingsUtils::SaveScheduleSettingsL(const CMsvScheduleSettings& aScheduleSettings, CRepository& aRepository)
	{
	aRepository.Set(EScheduleVersionId, KScheduleSettingsVersion);	
	aRepository.Set(ESchedulePriorityId, aScheduleSettings.Priority());
	aRepository.Set(EScheduleValidityPeriodId, aScheduleSettings.ValidityPeriod().Int());
	aRepository.Set(EScheduleIntervalTypeId, static_cast<TInt>(aScheduleSettings.IntervalType()));
	aRepository.Set(EScheduleLatencyId, aScheduleSettings.Latency().Int());
	aRepository.Set(EScheduleLongIntervalId, aScheduleSettings.LongInterval().Int());
	aRepository.Set(EScheduleShortIntervalId, aScheduleSettings.ShortInterval().Int());
	aRepository.Set(ESchedulePendingConditionsTimeoutId, aScheduleSettings.PendingConditionsTimeout().Int());

	TInt count = aScheduleSettings.VariableIntervals().Count();
	__ASSERT_ALWAYS(count<=KMaxVariableIntervals, User::Leave(KErrOverflow));	
	aRepository.Set(EScheduleVariableIntervalsCountId, count);
	
	TUint32 variableIntervalId = EScheduleVariableIntervals;
	for (TInt counter = 0; counter < count; ++counter)
		{
		aRepository.Set(variableIntervalId, aScheduleSettings.VariableIntervals().At(counter).Int());
		++variableIntervalId;
		}
	}

/**
Saves Off Peak Times to CenRep.
This function must be used within a CenRep Transaction.

@param	aOffPeakTimes
Off Peak Times

@param	aRepository
Repository to save settings to.
*/
EXPORT_C void TMsvScheduleSettingsUtils::SaveOffPeakSettingsL(const CMsvOffPeakTimes& aOffPeakTimes, CRepository& aRepository)
	{

	aRepository.Set(EOffPeakTimesVersionId, KOffPeakTimesVersion);

	TInt count = aOffPeakTimes.Count();
	__ASSERT_ALWAYS(count<=KMaxSettingList, User::Leave(KErrOverflow));		
	aRepository.Set(EOffPeakTimesCountId, count);

	TUint32 offPeakTimeId = EOffPeakTimes;
	for(TInt curTime = 0; curTime < count; ++curTime)
		{	
		aRepository.Set(offPeakTimeId + EOffPeakVersion, KOffPeakTimeVersion);		
		aRepository.Set(offPeakTimeId + EOffPeakDay, static_cast<TInt>(aOffPeakTimes.At(curTime).Day()));
		aRepository.Set(offPeakTimeId + EOffPeakHour, aOffPeakTimes.At(curTime).Hour());
		aRepository.Set(offPeakTimeId + EOffPeakMinute, aOffPeakTimes.At(curTime).Minute());
		aRepository.Set(offPeakTimeId + EOffPeakValidityPeriod, aOffPeakTimes.At(curTime).ValidityPeriod().Int());
		
		offPeakTimeId += KIncrementSettingList;
		}	
	}

/**
Saves Send Error Action to CenRep.
This function must be used within a CenRep Transaction.

@param	aErrorActionId
CenRep Id

@param	aErrorAction
Send Error Action

@param	aRepository
Repository to save settings to.
*/
void TMsvScheduleSettingsUtils::SaveSendErrorActionL(TUint32 aErrorActionId, const TMsvSendErrorAction& aErrorAction, CRepository& aRepository)
	{
	aRepository.Set(aErrorActionId + ESendErrorVersion, KSendErrorActionVersion);			
	aRepository.Set(aErrorActionId + ESendErrorError, aErrorAction.iError);		
	aRepository.Set(aErrorActionId + ESendErrorMaxRetries, static_cast<TInt>(aErrorAction.MaxRetries()));		
	aRepository.Set(aErrorActionId + ESendErrorRetries, static_cast<TInt>(aErrorAction.iRetries));		
	aRepository.Set(aErrorActionId + ESendErrorAction,  static_cast<TInt>(aErrorAction.iAction));		
	aRepository.Set(aErrorActionId + ESendErrorRetrySpacing,  static_cast<TInt>(aErrorAction.iRetrySpacing));		
	}

/**
Saves Send Error Actions to CenRep.
This function must be used within a CenRep Transaction.

@param	aErrorActions
Send Error Actions

@param	aRepository
Repository to save settings to.
*/
EXPORT_C void TMsvScheduleSettingsUtils::SaveSendErrorSettingsL(const CMsvSendErrorActions& aErrorActions, CRepository& aRepository)
	{

	aRepository.Set(ESendErrorActionsVersionId, KSendErrorActionsVersion);

	// Default error actions	
	SaveSendErrorActionL(ESendErrorDefaultAction, aErrorActions.Default(), aRepository);

	// List of error actions
	TInt count = aErrorActions.Errors().Count();
	__ASSERT_ALWAYS(count<=KMaxSettingList, User::Leave(KErrOverflow));		
	aRepository.Set(ESendErrorActionsCountId, count);

	TUint32 errorActionId = ESendErrorActions;
	for (TInt counter = 0; counter < count; ++counter)
		{
		SaveSendErrorActionL(errorActionId, aErrorActions.Errors().At(counter), aRepository);							
		errorActionId += KIncrementSettingList;
		}
	}

/**
Saves System Agent Actions to CenRep.
This function must be used within a CenRep Transaction.

@param	aSysAgentActions
System Agent Actions

@param	aRepository
Repository to save settings to.
*/
EXPORT_C void TMsvScheduleSettingsUtils::SaveSysAgentSettingsL(const CMsvSysAgentActions& aSysAgentActions, CRepository& aRepository)
	{

	aRepository.Set(ESysAgentActionsVersionId, KSysAgentActionsVersion);
		
	// Default send error actions	
	SaveSendErrorActionL(ESysAgentDefaultSendErrorAction, aSysAgentActions.iDefault, aRepository);
		
	// Sys Agent Settings		
	TInt count = aSysAgentActions.Count();
	__ASSERT_ALWAYS(count<=KMaxSettingList, User::Leave(KErrOverflow));		
	aRepository.Set(ESysAgentActionsCountId, count);
	
	// Save a list of System Agent Actions and Send Error Actions
	TUint32 sysAgentId = ESysAgentActions;
	TUint32 errorActionId = ESysAgentSendErrorActions;
	while (count--)
		{
		aRepository.Set(sysAgentId + ESysAgentVersion, KSysAgentConditionActionVersion);					
		aRepository.Set(sysAgentId + ESysAgentVariable, static_cast<TInt>(aSysAgentActions.At(count).iCondition.iVariable.iUid));			
		aRepository.Set(sysAgentId + ESysAgentState, aSysAgentActions.At(count).iCondition.iState);					
		aRepository.Set(sysAgentId + ESysAgentType, static_cast<TInt>(aSysAgentActions.At(count).iCondition.iType));					
		aRepository.Set(sysAgentId + ESysAgentDefault, aSysAgentActions.At(count).iUseDefaultSysAgentAction);					
 		sysAgentId += KIncrementSettingList;
		
		// Error actions
		SaveSendErrorActionL(errorActionId, aSysAgentActions.At(count).iErrorAction, aRepository);							
		errorActionId += KIncrementSettingList;
		}	
	}

/**
Loads Schedule Settings from CenRep.

@param	aScheduleSettings
Schedule settings

@param	aRepository
Repository to Load settings from.
*/
EXPORT_C void TMsvScheduleSettingsUtils::LoadScheduleSettingsL(CMsvScheduleSettings& aScheduleSettings, CRepository& aRepository)
	{
	aScheduleSettings.Reset();

	TInt temp = 0;

	User::LeaveIfError(aRepository.Get(ESchedulePriorityId, temp));
	aScheduleSettings.SetPriority(temp);
		
	User::LeaveIfError(aRepository.Get(EScheduleValidityPeriodId, temp));
	aScheduleSettings.SetValidityPeriod(static_cast<TTimeIntervalMinutes>(temp));
		
	User::LeaveIfError(aRepository.Get(EScheduleIntervalTypeId, temp));
	aScheduleSettings.SetIntervalType(static_cast<TIntervalType>(temp));
		
	User::LeaveIfError(aRepository.Get(EScheduleLatencyId, temp));
	aScheduleSettings.SetLatency(static_cast<TTimeIntervalMicroSeconds32>(temp));

	User::LeaveIfError(aRepository.Get(EScheduleLongIntervalId, temp));
	aScheduleSettings.SetLongInterval(static_cast<TTimeIntervalSeconds>(temp));

	User::LeaveIfError(aRepository.Get(EScheduleShortIntervalId, temp));
	aScheduleSettings.SetShortInterval(static_cast<TTimeIntervalSeconds>(temp));

	User::LeaveIfError(aRepository.Get(ESchedulePendingConditionsTimeoutId, temp));
	aScheduleSettings.SetPendingConditionsTimeout(static_cast<TTimeIntervalMinutes>(temp));

	User::LeaveIfError(aRepository.Get(EScheduleVariableIntervalsCountId, temp));
	TInt count = temp;

	if (count>0)
		{		
		TUint32 variableIntervalId = EScheduleVariableIntervals;
	    CArrayFixFlat<TTimeIntervalSeconds>* intervals = new(ELeave)CArrayFixFlat<TTimeIntervalSeconds>(count);    
	    CleanupStack::PushL(intervals);
		for (TInt cur = 0; cur < count; ++cur)
			{
			User::LeaveIfError(aRepository.Get(variableIntervalId, temp));
			intervals->AppendL(static_cast<TTimeIntervalSeconds>(temp));

			++variableIntervalId;
			}
			
		if (intervals->Count()>0)
			{		
	   		aScheduleSettings.SetVariableIntervalsL(*intervals);
			}
		CleanupStack::PopAndDestroy(intervals);	
		}
	}

/**
Loads Off Peak Times from CenRep.

@param	aOffPeakTimes
Off Peak Times

@param	aRepository
Repository to Load settings from.
*/
EXPORT_C void TMsvScheduleSettingsUtils::LoadOffPeakSettingsL(CMsvOffPeakTimes& aOffPeakTimes, CRepository& aRepository)
	{
	aOffPeakTimes.Reset();

	TInt temp = 0;
	
	User::LeaveIfError(aRepository.Get(EOffPeakTimesCountId, temp));
	TInt count = temp;
		
	TUint32 settingId = 0;	
	TUint32 offPeakTimeId = EOffPeakTimes;
	TMsvOffPeakTime opTime;
	for (TInt curTime = 0; curTime < count; ++curTime)
		{			
		opTime.Reset();
		settingId = offPeakTimeId + EOffPeakDay;
		User::LeaveIfError(aRepository.Get(settingId, temp));
		opTime.SetDay(static_cast<TDay>(temp));

		settingId = offPeakTimeId + EOffPeakHour;
		User::LeaveIfError(aRepository.Get(settingId, temp));
		opTime.SetHour(temp);

		settingId = offPeakTimeId + EOffPeakMinute;
		User::LeaveIfError(aRepository.Get(settingId, temp));
		opTime.SetMinute(temp);

		settingId = offPeakTimeId + EOffPeakValidityPeriod;
		User::LeaveIfError(aRepository.Get(settingId, temp));
		opTime.SetValidityPeriod(static_cast<TTimeIntervalMinutes>(temp));
									
		aOffPeakTimes.AppendL(opTime);			
		offPeakTimeId += KIncrementSettingList;
		}
	}

/**
Loads a Send Error Action from CenRep.

@param	aErrorActionId
CenRep Id

@param	aErrorAction
Send Error Action

@param	aRepository
Repository to Load settings from.
*/
void TMsvScheduleSettingsUtils::LoadSendErrorActionL(TUint32 aErrorActionId, TMsvSendErrorAction& aErrorAction, CRepository& aRepository)
	{
	aErrorAction.Reset();

	TInt temp = 0;
	TUint32 settingId = aErrorActionId + ESendErrorError;
	User::LeaveIfError(aRepository.Get(settingId, temp));
	aErrorAction.iError = temp;
		
	settingId = aErrorActionId + ESendErrorMaxRetries;
	User::LeaveIfError(aRepository.Get(settingId, temp));
	aErrorAction.SetMaxRetries(static_cast<TInt16>(temp));

	settingId = aErrorActionId + ESendErrorRetries;
	User::LeaveIfError(aRepository.Get(settingId, temp));
	aErrorAction.iRetries = static_cast<TMsvSendRetries>(temp);

	settingId = aErrorActionId + ESendErrorAction;
	User::LeaveIfError(aRepository.Get(settingId, temp));
	aErrorAction.iAction = static_cast<TMsvSendAction>(temp);

	settingId = aErrorActionId + ESendErrorRetrySpacing;
	User::LeaveIfError(aRepository.Get(settingId, temp));
	aErrorAction.iRetrySpacing = static_cast<TMsvSendRetrySpacing>(temp);
	}

/**
Loads Send Error Actions from CenRep.

@param	aErrorActions
Send Error Actions

@param	aRepository
Repository to Load settings from.
*/
EXPORT_C void TMsvScheduleSettingsUtils::LoadSendErrorSettingsL(CMsvSendErrorActions& aErrorActions, CRepository& aRepository)
	{
	aErrorActions.Reset();
	
	TInt temp = 0;
		
	TMsvSendErrorAction action;
	LoadSendErrorActionL(ESendErrorDefaultAction, action, aRepository);
	aErrorActions.SetDefault(action);

	User::LeaveIfError(aRepository.Get(ESendErrorActionsCountId, temp));	
	TInt count = temp;
		
	TUint32 errorActionId = ESendErrorActions;
	for (TInt counter = 0; counter < count; ++counter)
		{
		LoadSendErrorActionL(errorActionId, action, aRepository);
		aErrorActions.AddSendErrorActionL(action);		
		errorActionId += KIncrementSettingList;
		}
	}

/**
Loads System Agent Actions from CenRep.

@param	aSysAgentActions
System Agent Actions

@param	aRepository
Repository to Load settings from.
*/
EXPORT_C void TMsvScheduleSettingsUtils::LoadSysAgentSettingsL(CMsvSysAgentActions& aSysAgentActions, CRepository& aRepository)
	{
	aSysAgentActions.Reset();
	
	TInt temp = 0;
	
	LoadSendErrorActionL(ESysAgentDefaultSendErrorAction, aSysAgentActions.iDefault, aRepository);

	User::LeaveIfError(aRepository.Get(ESysAgentActionsCountId, temp));	
	TInt count = temp;
		
	TUint32 settingId = 0;
	TMsvSysAgentConditionAction condAction;
	TUint32 sysAgentId = ESysAgentActions;
	TUint32 errorActionId = ESysAgentSendErrorActions;
	while (count--)
		{
		settingId = sysAgentId + ESysAgentVariable;	
		User::LeaveIfError(aRepository.Get(settingId, temp));
		condAction.iCondition.iVariable.iUid = temp;

		settingId = sysAgentId + ESysAgentState;
		User::LeaveIfError(aRepository.Get(settingId, condAction.iCondition.iState));	

		settingId = sysAgentId + ESysAgentType;
		User::LeaveIfError(aRepository.Get(settingId, temp));
		condAction.iCondition.iType = static_cast<TMsvCondition::TMsvSchSendConditionType>(temp);

		settingId = sysAgentId + ESysAgentDefault;
		User::LeaveIfError(aRepository.Get(settingId, condAction.iUseDefaultSysAgentAction));

		// Error actions
		LoadSendErrorActionL(errorActionId, condAction.iErrorAction, aRepository);							
						
		aSysAgentActions.AppendL(condAction);			
		sysAgentId += KIncrementSettingList;
		errorActionId += KIncrementSettingList;
		}
	}
