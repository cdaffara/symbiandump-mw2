/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Defines all constants used by the Radio Server and its clients.
*
*/



#ifndef RADIOCLIENTSERVER_H
#define RADIOCLIENTSERVER_H

//  INCLUDES
#include <e32std.h>
#include "RadioServerData.h"

// CONSTANTS & DATA TYPES
_LIT(KRadioServerName, "RadioServer");
_LIT(KRadioServerImg,  "RadioServer");		// DLL/EXE name

const TUid KRadioServerUid3 = {0x10207A7E};

// Version numbers
const TUint KRadioServerVersionMajor = 1;
const TUint KRadioServerVersionMinor = 0;
const TUint KRadioServerVersionBuild = 0;

// Interface ID and destination handle
const TUid KRadioServerInterfaceUid = {0x10207A98};
const TInt KRadioServerObjectHandle = -3;

/**
* RadioServer Publish & Subscribe property sub-key
*/
enum TRadioServerPsKey
	{
	ERadioServPsPlayerState		=	10,
	ERadioServPsAntennaStatus,
	ERadioServPsOfflineMode,
	ERadioServPsTransmitterStatus,
	ERadioServPsFrequency,
	ERadioServPsFrequencyRange,
	ERadioServPsForceMonoReception,
	ERadioServPsSquelch,
	ERadioServPsVolume,
	ERadioServPsMuteStatus,
	ERadioServPsBalance,
	ERadioServPsRdsSignalStatus,
	ERadioServPsAutoSwitchStatus,
	ERadioServPsProgrammeIdentification,
	ERadioServPsProgrammeType,
	ERadioServPsProgrammeService,
	ERadioServPsRadioText,
	ERadioServPsClockTime,
	ERadioServPsTrafficAnnouncementStatus,
	ERadioServPsAfSearchBegin,
	ERadioServPsAfSearchEnd,
	ERadioServPsRadioTextPlusObjects
	};

/**
* Server OP codes
*/
enum TRadioServerRequest
	{
	ERadioServNone,
// Tuner
	ERadioServRequestTunerControl,				// 1
	ERadioServGetTunerCapabilities,
	ERadioServEnableTunerInOfflineMode,
	ERadioServSetFrequencyRange,
	ERadioServGetFrequencyRange,				// 5
	ERadioServSetFrequency,
	ERadioServGetFrequency,
	ERadioServStationSeek,
	ERadioServGetSignalStrength,
	ERadioServGetMaxSignalStrength,				// 10
	ERadioServGetStereoMode,
	ERadioServForceMonoReception,
	ERadioServGetForceMonoReception,
	ERadioServSetSquelch,
	ERadioServGetSquelch,						// 15
// Player
	ERadioServGetPlayerState,
	ERadioServPlay,
	ERadioServStop,
	ERadioServGetMaxVolume,
	ERadioServSetVolume,						// 20
	ERadioServGetVolume,
	ERadioServSetVolumeRamp,
	ERadioServSetMute,
	ERadioServGetMuteStatus,
	ERadioServSetBalance,						// 25
	ERadioServGetBalance,
// RDS
	ERadioServGetRdsCapabilities,
	ERadioServGetRdsSignalStatus,
	ERadioServNotifyRdsDataChange,
	ERadioServCancelNotifyRdsDataChange,		// 30
	ERadioServNotifyRadioTextPlusChange,
	ERadioServCancelNotifyRadioTextPlusChange,
	ERadioServSetAutomaticSwitching,
	ERadioServGetAutomaticSwitching,
	ERadioServCancelAFSearch,					// 35
	ERadioServStationSeekByPTY,
	ERadioServStationSeekByTA,
	ERadioServStationSeekByTP,
	ERadioServGetFreqByPTY,
	ERadioServGetFreqByTA,						// 40
	ERadioServGetPSByPTY,
	ERadioServGetPSByTA,
	ERadioServGetProgrammeIdentification,
	ERadioServGetProgrammeType,
	ERadioServGetProgrammeService,				// 45
	ERadioServGetRadioText,
	ERadioServGetRadioTextPlus,
	ERadioServGetClockTime,
	ERadioServGetTrafficAnnouncementStatus,
	ERadioServGetTrafficProgrammeStatus,		// 50
// Internal
	ERadioServAddPrimaryClient,
	ERadioServRemovePrimaryClient,
	ERadioServCancel
	};

#endif      // RADIOCLIENTSERVER_H

// End of File
