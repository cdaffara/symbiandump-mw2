/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvester server commands*
*/


#ifndef __HARVESTERCOMMON_H__
#define __HARVESTERCOMMON_H__

#include <harvestereventenum.h>
#include <e32msgqueue.h>
#include <f32file.h>

enum THarvesterService
{
    EResumeHarvester,
    EPauseHarvester,
    EHarvestFile,
    ERegisterProcessOrigin,
    EUnregisterProcessOrigin,
	ERegisterHarvestComplete,
    EUnregisterHarvestComplete,
    ERegisterHarvesterEvent,
    EUnregisterHarvesterEvent,
    EGetLastObserverId,

#ifdef _DEBUG
    EStopHarvester,
#endif

    EHarvestFileWithUID
};

/* Server version */
const TUint KHarvesterServerMajorVersion = 0;
const TUint KHarvesterServerMinorVersion = 1;
const TUint KHarvesterServerBuildVersion = 1;

/**
 * Harvester event status which is sent from 
 * harvester server to harvester client.
 */
struct THarvesterEventNotification
	{
	TUint iObserverId;
	HarvesterEventObserverType iObserverType;
	HarvesterEventState iCurrentState;
	TUint iItemsLeft;
	};

typedef RMsgQueue<THarvesterEventNotification> THarvesterEventQueue;

/**
 * Common harvester event observer infomation.
 * 
 * Used for deliver client side information to server side during 
 * registering observers and store observer information in server side.
 */
struct THarvesterEventObserverInfo
	{
	// client fills
	TInt iQueueHandle;
	TUint iObserverId;
	TInt iObserverType;
	TInt iNotificationInterval;

	// server fills
	TUint iProcessUid;
	THarvesterEventQueue* iQueuePtr; // not owned
	TUint iDelta;
	};

	
_LIT(KHarvesterOomQueue, "mdsoommsgqueue");

enum TMdsOomStatus
	{
	EMdsOomFreeRam,
	EMdsOomMemoryGood
	};

const TInt KHarvesterGetRootDirFlags = EDirsFirst;
const TInt KHarvesterGetDirFlags = ESortByDate | EAscending | EDirsFirst;

_LIT( KExludedThumbPath, "_PAlbTN");
_LIT( KExludedMediaArtPath, ".mediaartlocal");

const TInt KOriginIgnored = 255;
const TInt KOriginFastHarvest = 254;
const TInt KOriginIgnoreAttribsChanged = 253;

const CActive::TPriority KHarvesterPriorityContextEngine = CActive::EPriorityUserInput;
const CActive::TPriority KHarvesterPriorityMonitorPlugin = CActive::EPriorityUserInput;
const CActive::TPriority KHarvesterPriorityComposerPlugin = CActive::EPriorityStandard;
const CActive::TPriority KHarvesterPriorityHarvestingPlugin = CActive::EPriorityStandard;

// Between Standard and UserInput to allow components to react to important events correctly
const TInt KHarvesterCustomImportantPriority = 5;
// Higher than MonitorPlugin priority but lower than High. Highest priority within Harvester
const TInt KHarvesterServerHighPriority = 11;

#endif


