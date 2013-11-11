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
* Description:  Defines the cenrep keys of triggering settigs API
*
*/


#ifndef LBTTRIGGERINGSETTINGSKEYS_H
#define LBTTRIGGERINGSETTINGSKEYS_H

// CONSTANTS

// ENUMERATION FOR CENTRAL REPOSITORY KEYS
enum TCenRepKeys
	{
	ELbtOptimizationLevel = 0x00000001,
	ELbtModuleId = 0x00000002,
	ELbtMinUpdateInterval = 0x00000003,
	ELbtMinUpdateIntervalOnGpsFailure = 0x00000004,
	ELbtMaxUserSpeed = 0x00000005,
	ELbtTriggeringMechanismState = 0x00000006,
	ELbtMinimumTriggerArea = 0x00000007,
	ELbtTrackModeInterval = 0x00000008,
	ELbtLocRequestTimeOut = 0x00000009,
	ELbtMinHysteresisArea = 0x0000000A,
	ELbtDaysForCleanup = 0x0000000B,
	ELbtSleepTime  = 0x0000000C,
	ELbtMaxActiveTriggers = 0x0000000D,
	ELbtNppModuleId = 0x0000000E
	};

// CENTRAL REPOSITORY UID
const TInt KCRUidLocationTriggeringSettings(0x1028312F);




#endif // LBTTRIGGERINGSETTINGSKEYS_H