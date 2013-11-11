/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Responsible for settings and retreiving all location 
*				 triggering settings
*
*/


// INCLUDE FILES
#include <centralrepository.h>
#include "lbtsettingsmanager.h"
#include "lbttriggeringsettingskeys.h"
#include "lbtlogger.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtSettingsManager::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
CLbtSettingsManager* CLbtSettingsManager::NewL()
	{
	FUNC_ENTER("CLbtSettingsManager::NewL");
	CLbtSettingsManager* self = new (ELeave) CLbtSettingsManager();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtSettingsManager::ConstructL()
	{
	FUNC_ENTER("CLbtSettingsManager::ConstructL");
	// Create a repository object
	iRepository = CRepository::NewL(TUid::Uid(KCRUidLocationTriggeringSettings));
	
	// Get all the values and store it in RAM
	iRepository->Get(ELbtOptimizationLevel, iOptimizationLevel);
	iRepository->Get(ELbtModuleId, iModuleId);	
	iRepository->Get(ELbtMinUpdateInterval, iMinUpdateInterval);
	iRepository->Get(ELbtMinUpdateIntervalOnGpsFailure, iMinUpdateIntervalOnGpsFailure);
	iRepository->Get(ELbtMaxUserSpeed, iMaxUserSpeed);
	iRepository->Get(ELbtTriggeringMechanismState, iTriggeringMechanismState);
	iRepository->Get(ELbtMinimumTriggerArea, iMinimumTriggerArea);
	iRepository->Get(ELbtTrackModeInterval, iTrackModeInterval);
	iRepository->Get(ELbtLocRequestTimeOut, iLocRequestTimeOut);
	iRepository->Get(ELbtMinHysteresisArea, iMinHysteresisArea);
	iRepository->Get(ELbtDaysForCleanup, iDaysForCleanup);
	iRepository->Get(ELbtMaxActiveTriggers, iEnabledValidTriggerCount);
	iRepository->Get(ELbtNppModuleId, iNppModuleId);
	
	LOG1("iOptimizationLevel = %d", iOptimizationLevel);
	LOG1("iModuleId = %d", iModuleId);
	LOG1("iMinUpdateInterval = %d", iMinUpdateInterval);
	LOG1("iMinUpdateIntervalOnGpsFailure = %d", iMinUpdateIntervalOnGpsFailure);
	LOG1("iMaxUserSpeed = %d", iMaxUserSpeed);
	LOG1("iTriggeringMechanismState = %d", iTriggeringMechanismState);
	LOG1("iMinimumTriggerArea = %d", iMinimumTriggerArea);
	LOG1("iTrackModeInterval = %d", iTrackModeInterval);
	LOG1("iLocRequestTimeOut = %d", iLocRequestTimeOut);
	LOG1("iMinHysteresisArea = %d", iMinHysteresisArea);
	LOG1("iDaysForCleanup = %d", iDaysForCleanup);
	LOG1("iEnabledValidTriggerCount = %d", iEnabledValidTriggerCount);
	LOG1("iNppModuleId = %d", iNppModuleId);
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::~CLbtSettingsManager
// ---------------------------------------------------------------------------
//
CLbtSettingsManager::~CLbtSettingsManager()
	{
	FUNC_ENTER("CLbtSettingsManager::~CLbtSettingsManager");
	delete iRepository;
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::SetOptimizationLevel
// ---------------------------------------------------------------------------
//
void CLbtSettingsManager::SetOptimizationLevel(TInt aOptimizationLevel)
	{
	FUNC_ENTER("CLbtSettingsManager::SetOptimizationLevel");
	TInt retVal = iRepository->Set(ELbtOptimizationLevel, aOptimizationLevel);
	if(retVal == KErrNone)
		{
		iOptimizationLevel = aOptimizationLevel;
		}
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::GetOptimizationLevel
// ---------------------------------------------------------------------------
//
TInt CLbtSettingsManager::GetOptimizationLevel()
	{
	FUNC_ENTER("CLbtSettingsManager::GetOptimizationLevel");
	return iOptimizationLevel;
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::SetModuleId
// ---------------------------------------------------------------------------
//
void CLbtSettingsManager::SetModuleId(TPositionModuleId aModuleId)
	{
	FUNC_ENTER("CLbtSettingsManager::SetModuleId");
	TInt uid = aModuleId.iUid;
	TInt retVal = iRepository->Set(ELbtModuleId, uid);
	if(retVal == KErrNone)
		{
		iModuleId = aModuleId.iUid;
		}
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::GetModuleId
// ---------------------------------------------------------------------------
//
TPositionModuleId CLbtSettingsManager::GetModuleId()
	{
	FUNC_ENTER("CLbtSettingsManager::GetModuleId");
	return TUid::Uid(iModuleId);
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::GetNppModuleId
// ---------------------------------------------------------------------------
//
TPositionModuleId CLbtSettingsManager::GetNppModuleId()
	{
	FUNC_ENTER("CLbtSettingsManager::GetNppModuleId");
	return TUid::Uid(iNppModuleId);
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::SetMinimumUpdateInterval
// ---------------------------------------------------------------------------
//
void CLbtSettingsManager::SetMinimumUpdateInterval(TInt aUpdateInterval)
	{
	FUNC_ENTER("CLbtSettingsManager::SetMinimumUpdateInterval");
	TInt retVal = iRepository->Set(ELbtMinUpdateInterval, aUpdateInterval);
	if(retVal == KErrNone)
		{
		iMinUpdateInterval = aUpdateInterval;
		}
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::GetMinimumUpdateInterval
// ---------------------------------------------------------------------------
//
TInt CLbtSettingsManager::GetMinimumUpdateInterval()
	{
	FUNC_ENTER("CLbtSettingsManager::GetMinimumUpdateInterval");
	return iMinUpdateInterval;
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::SetMinimumUpdateIntervalOnGpsFailure
// ---------------------------------------------------------------------------
//
void CLbtSettingsManager::SetMinimumUpdateIntervalOnGpsFailure(TInt aUpdateInterval)
	{
	FUNC_ENTER("CLbtSettingsManager::SetMinimumUpdateIntervalOnGpsFailure");
	TInt retVal = iRepository->Set(ELbtMinUpdateIntervalOnGpsFailure, aUpdateInterval);
	if(retVal == KErrNone)
		{
		iMinUpdateIntervalOnGpsFailure = aUpdateInterval;
		}
	}


// ---------------------------------------------------------------------------
// CLbtSettingsManager::GetMinimumUpdateIntervalOnGpsFailure
// ---------------------------------------------------------------------------
//
TInt CLbtSettingsManager::GetMinimumUpdateIntervalOnGpsFailure()
	{
	FUNC_ENTER("CLbtSettingsManager::GetMinimumUpdateIntervalOnGpsFailure");
	return iMinUpdateIntervalOnGpsFailure;
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::SetMaximumUserSpeed
// ---------------------------------------------------------------------------
//
void CLbtSettingsManager::SetMaximumUserSpeed(TInt aSpeed)
	{
	FUNC_ENTER("CLbtSettingsManager::SetMaximumUserSpeed");
	TInt retVal = iRepository->Set(ELbtMaxUserSpeed, aSpeed);
	if(retVal == KErrNone)
		{
		iMaxUserSpeed = aSpeed;
		}
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::GetMaximumUserSpeed
// ---------------------------------------------------------------------------
//
TInt CLbtSettingsManager::GetMaximumUserSpeed()
	{
	FUNC_ENTER("CLbtSettingsManager::GetMaximumUserSpeed");
	return iMaxUserSpeed;
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::SetTriggeringMechanismState
// ---------------------------------------------------------------------------
//
void CLbtSettingsManager::SetTriggeringMechanismState(TLbtTriggeringMechanismState aState)
	{
	FUNC_ENTER("CLbtSettingsManager::SetTriggeringMechanismState");
	TInt retVal = iRepository->Set(ELbtTriggeringMechanismState, aState);
	if(retVal == KErrNone)
		{
		iTriggeringMechanismState = aState;
		}
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::GetTriggeringMechanismState
// ---------------------------------------------------------------------------
//
TInt CLbtSettingsManager::GetTriggeringMechanismState()
	{
	FUNC_ENTER("CLbtSettingsManager::GetTriggeringMechanismState");
	return iTriggeringMechanismState;
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::MinimumTriggeringArea
// ---------------------------------------------------------------------------
//
TInt CLbtSettingsManager::MinimumTriggeringArea()
	{
	FUNC_ENTER("CLbtSettingsManager::MinimumTriggeringArea");
	return iMinimumTriggerArea;
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::TrackingModeInterval
// ---------------------------------------------------------------------------
//
TInt CLbtSettingsManager::TrackingModeInterval()
	{
	FUNC_ENTER("CLbtSettingsManager::TrackingModeInterval");
	return iTrackModeInterval;
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::GetLocationRequestTimeOut
// ---------------------------------------------------------------------------
//
TInt CLbtSettingsManager::GetLocationRequestTimeOut()
	{
	FUNC_ENTER("CLbtSettingsManager::GetLocationRequestTimeOut");
	return iLocRequestTimeOut;
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::MinimumHysteresisArea
// ---------------------------------------------------------------------------
//
TInt CLbtSettingsManager::MinimumHysteresisArea()
	{
	FUNC_ENTER("CLbtSettingsManager::MinimumHysteresisArea");
	return iMinHysteresisArea;
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::NumberOfDaysForCleanup
// ---------------------------------------------------------------------------
//
TInt CLbtSettingsManager::NumberOfDaysForCleanup()
	{
	FUNC_ENTER("CLbtSettingsManager::NumberOfDaysForCleanup");
	return iDaysForCleanup;
	}

// ---------------------------------------------------------------------------
// CLbtSettingsManager::NumberOfDaysForCleanup
// ---------------------------------------------------------------------------
//
TInt CLbtSettingsManager::EnabledValidTriggerCount()
	{
	return iEnabledValidTriggerCount;
	}

// end of file


