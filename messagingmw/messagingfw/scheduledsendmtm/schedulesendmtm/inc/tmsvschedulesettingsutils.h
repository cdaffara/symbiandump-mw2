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

#ifndef __TMSVSCHEDULESETTINGSUTILS_H__
#define __TMSVSCHEDULESETTINGSUTILS_H__

class CRepository;
class CMsvScheduleSettings;
class CMsvOffPeakTimes;
class CMsvSendErrorActions;
class CMsvSysAgentActions;
class TMsvSendErrorAction;


/**
Schedule Send settings utility class. 

Provides functionality to load/save Schedule Send settings in Central Repository.

@publishedPartner
@released
*/
class TMsvScheduleSettingsUtils
	{
public:
	IMPORT_C static void SaveScheduleSettingsL(const CMsvScheduleSettings& aScheduleSettings, CRepository& aRepository);
	IMPORT_C static void SaveOffPeakSettingsL(const CMsvOffPeakTimes& aOffPeakTimes, CRepository& aRepository);
	IMPORT_C static void SaveSendErrorSettingsL(const CMsvSendErrorActions& aErrorActions, CRepository& aRepository);
	IMPORT_C static void SaveSysAgentSettingsL(const CMsvSysAgentActions& aSysAgentActions, CRepository& aRepository);

	IMPORT_C static void LoadScheduleSettingsL(CMsvScheduleSettings& aScheduleSettings, CRepository& aRepository);
	IMPORT_C static void LoadOffPeakSettingsL(CMsvOffPeakTimes& aOffPeakTimes, CRepository& aRepository);
	IMPORT_C static void LoadSendErrorSettingsL(CMsvSendErrorActions& aErrorActions, CRepository& aRepository);
	IMPORT_C static void LoadSysAgentSettingsL(CMsvSysAgentActions& aSysAgentActions, CRepository& aRepository);

	/** 
	Schedule Send Setting Types. 
	
	CenRep ID 0x10000 to 0x90000 is reserved for Schedule Send Settings. 

	@publishedPartner
	@released
	*/
	enum TMsvScheduleSendSettingTypes
		{
		EScheduleSettings			= 0x00010000,
		EOffPeakTimeSettings		= 0x00020000,
		ESendErrorActionSettings	= 0x00030000,
		ESystemAgentActionSettings	= 0x00040000
		};
		
private:
		
	enum TMsvScheduleSettingsCenRepId
		{
		// Schedule Settings		
		EScheduleVersionId					= 0x00010000,
		ESchedulePriorityId					= 0x00010001,
		EScheduleValidityPeriodId			= 0x00010002,
		EScheduleIntervalTypeId				= 0x00010003,
		EScheduleLatencyId					= 0x00010004,
		EScheduleLongIntervalId	   			= 0x00010005,     	
		EScheduleShortIntervalId			= 0x00010006,
		EScheduleVariableIntervalsCountId	= 0x00010007,
		ESchedulePendingConditionsTimeoutId	= 0x00010008,
		
		// List of Variable Intervals
		EScheduleVariableIntervals			= 0x00011000
		};

	enum TMsvOffPeakSettingsCenRepId
		{
		// Off Peak Time settings		
		EOffPeakTimesVersionId		= 0x00020000,
		EOffPeakTimesCountId		= 0x00020001,

		// List of Off Peak Times
		EOffPeakTimes				= 0x00021000,
		
		// Off Peak Time fields
		EOffPeakVersion				= 0x00000000,
		EOffPeakDay					= 0x00000001,
		EOffPeakHour				= 0x00000002,
		EOffPeakMinute	   			= 0x00000003,     	
		EOffPeakValidityPeriod 		= 0x00000004
		};

	enum TMsvSendErrorActionsCenRepId
		{
		// Send Error Action settings		
		ESendErrorActionsVersionId		= 0x00030000,
		ESendErrorActionsCountId		= 0x00030001,

		// Default Send Error Action
		ESendErrorDefaultAction			= 0x00031000,

		// List of Send Error Actions
		ESendErrorActions				= 0x00032000,
		
		// Send Error Action fields
		ESendErrorVersion				= 0x00000000,
		ESendErrorError					= 0x00000001,
		ESendErrorMaxRetries			= 0x00000002,     	
		ESendErrorRetries				= 0x00000003,
		ESendErrorAction				= 0x00000004,
		ESendErrorRetrySpacing			= 0x00000005
		};

	enum TMsvSysAgentActionsCenRepId
		{
		// System Agent Action settings		
		ESysAgentActionsVersionId		= 0x00040000,
		ESysAgentActionsCountId			= 0x00040001,

		// Default Send Error Action
		ESysAgentDefaultSendErrorAction = 0x00041000,

		// List of System Agent Actions
		ESysAgentActions				= 0x00042000,

		// List of Send Error Actions
		ESysAgentSendErrorActions		= 0x00043000,
		
		// System Agent Action fields
		ESysAgentVersion				= 0x00000000,
		ESysAgentVariable				= 0x00000001,
		ESysAgentState					= 0x00000002,     	
		ESysAgentType					= 0x00000003,
		ESysAgentDefault				= 0x00000004
		};
			
	static void SaveSendErrorActionL(TUint32 aErrorActionId, const TMsvSendErrorAction& aErrorAction, CRepository& aRepository);
	static void LoadSendErrorActionL(TUint32 aErrorActionId, TMsvSendErrorAction& aErrorAction, CRepository& aRepository);
	};

#endif
