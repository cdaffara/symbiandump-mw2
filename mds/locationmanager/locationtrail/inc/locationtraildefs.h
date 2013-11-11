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
* Description:  Definition file for Location Trail.
*
*/


#ifndef __LOCATIONTRAILDEFS_H__
#define __LOCATIONTRAILDEFS_H__

// Update interval
const TInt KUpdateInterval = 10000000;  // 10 seconds.
// Update time out
const TInt KUpdateTimeOut  = 20000000; // 20 seconds.
// Max age
const TInt KMaxAge         = 800000;   // 0.8 second

const TInt KMaxTrailLength = 480000000; // 8 min.

const TInt KMaximumIntervalSeconds = KUpdateTimeOut / 1000000; // 20 seconds

const TInt KCurrentLocTimeoutCount = 48; // 48 * 10 sec -> 8 min timeout.

const TInt KTrackLogBufSize = 10;

const TInt KLocationTrailShutdownDelay = 30;

const TInt KLocationTrailRemappingCheckDelay = 15;

const TInt KLocationTrailRemapShutdownDelay = 320;

const TInt KLocationDelta = 25;

const TInt KRemappingTime = 480;

const TInt KFirstInterval = 1000000; // 1 second

const TInt KFirstTimeOut = 1500000; // 1.5 second

//The name of the requestor
_LIT( KRequestor,         "Location Trail" );

// GPS tag ids.
const TInt KGPSVersion      = 0x0000;
const TInt KGPSLatitudeRef  = 0x0001;
const TInt KGPSLatitude     = 0x0002;

const TInt KGPSLongitudeRef = 0x0003;
const TInt KGPSLongitude    = 0x0004;

const TInt KGPSAltitudeRef  = 0x0005;
const TInt KGPSAltitude     = 0x0006;

const TInt KGPSTimeStamp    = 0x0007;
const TInt KGPSDateStamp    = 0x001d;


const TInt KMaxTagSize = 25;
const TInt KMinutes    = 60;   // Minutes.
const TInt KSeconds    = 6000; // Seconds * 100.
const TReal KRounding  = 0.5;  

_LIT8( KDateStampStr, "%04d:%02d:%02d" );  

_LIT8( KLatitudeStr, "%04S%04S%04S%04S%04S%04S" );

_LIT8( KNorthStr, "N" );  // North
_LIT8( KSouthStr, "S" );  // South
_LIT8( KEastStr,  "E" );  // East
_LIT8( KWestStr,  "W" );  // West

/**
 * UID for using Central Repository 
 * location managers related data.
 */ 
const TUid KRepositoryUid = { 0x200071BE };

/**
 * Key for reading interval value from 
 * Central repository.
 */ 
const TUint32 KIntervalKey = 0x00000001;

/**
 * Key for reading trail length value from 
 * Central repository.
 */ 
const TUint32 KTrailLengthKey = 0x00000002;

/**
 * Key for reading location trail timeout value from 
 * Central repository.
 */
const TUint32 KLocationTrailShutdownTimer = 0x00000003;

/**
 * Key for reading tracklog buffer size from 
 * Central repository.
 */
const TUint32 KTrackLogSizeKey = 0x00000004;

/**
 * Key for reading distance for using existing location object from 
 * Central repository.
 */
const TUint32 KLocationDeltaKey = 0x00000005;

/**
 * Key for reading distance for using existing location object from 
 * Central repository.
 */
const TUint32 KRemappingTimeKey = 0x00000006;

/**
 * Key for reading location trail timeout value from 
 * Central repository if items are still to be remapped.
 */
const TUint32 KLocationTrailRemapShutdownTimer = 0x00000007;

#endif // __LOCATIONTRAILDEFS_H__

// End of file.
