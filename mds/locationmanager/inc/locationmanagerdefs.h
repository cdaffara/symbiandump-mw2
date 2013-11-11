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
* Description:  Definition file for Location Manager.
*
*/

#ifndef __LOCATIONMANAGERDEFS_H__
#define __LOCATIONMANAGERDEFS_H__

_LIT( KLocServerFileName, "locationmanagerserver.exe");
_LIT( KLocServerName,     "LocationManagerServer" );

const TInt KLocationManagerServerMinor = 0;
const TInt KLocationManagerServerMajor = 1;
const TInt KLocationManagerServerBuild = 0;

const TInt KSessionSlotCount = -1; // use shared on-demand-pool

enum TLocCommands // Server commands.
    {
    ELocManStartTrail,
    ELocManStopTrail,
    ELocManTrailState,
    ELocManNotifyTrailStateChange,
    ELocManCancelNotification,
    ELocManRetrieveLoc,
    ELocManCurrentLoc,
    ELocManCancelCurrentLoc,
    ELocManCreateLocationObject,
    ELocManLocationSnapshot,
    ELocManRemoveLocationObject,
    ELocManCopyLocationDataById,
    ELocManCopyLocationDataByUri,
    ELocManEditLocationObject,
    ELocManStartTrackLog,
    ELocManStopTrackLog,
    ELocManGetTrackLogStatus,
    ELocManDeleteTrackLog,
    ELocManIsTrackLogRecording,
    ELocManTrackLogName,
    ELocManRegObserver,
    ELocManGetCaptureSetting,
    ELocManCancelTrackLogNotify,
    ELocManTagPending,
    ELocManStartGeoTaging,
    ELocManCancelTagPendingReq,
    ELocManCancelGeoTaggingReq
    };

/** Message argument indices */
enum TArgumentIndices
    {
    EArgumentTime = 0,
    EArgumentLocationData,
    EArgumentState
    };

const TInt KLocManagerSessionPriority = 5;

#endif // __LOCATIONMANAGERDEFS_H__

// End of file.
