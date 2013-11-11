/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SATEngine Subsystem PubSub uid and keys declaration.
*
*/


#ifndef SATINTERNALPSKEYS_H
#define SATINTERNALPSKEYS_H

// CONSTANTS

// SAT Internal Configuration API

/* 
 * The UID for internal use only. Using process secure uid 0x1000A833 
 * which is define in satserver.mmp
 */
const TUid  KPSUidSatServerInternal = { 0x1000A833 }; 

/*
 * Indicates the occurred End Key action. Used to determine whether to fetch 
 * icons from NAA syncronously or asynchronously. When the value is set to 1
 * it indicates that the End Key is pressed and synchronous mode must be used.
 *
 * Default value: 0
 */
const TUint32 KSatAppClosedUsingEndKey( 0x00000032 );

/**
 * Set Up Idle Mode Text. Value is a unicode string with length 0 to 242.
 *
 * Default value: N/A
 **/
const TUint32 KSatIdleModeText( 0x00000001 );

/**
 * Set Up Idle Mode Text Icon Id. Possible values are -1 to 255.
 * ID Determined independently of SAT.
 *
 * Default value: N/A
 */
const TUint32 KSatIdleModeTextIconId( 0x00000002 );

/**
 * Set Up Idle Mode Text Icon qualifier.
 *
 * Default value: N/A
 */
const TUint32 KSatIdleModeTextIconQualifier( 0x00000004 );

/**
 * Possible enumerations values for KSatAppClosedUsingEndKey.
 */
enum TSatAppTerminatedEventValue
    {
    KSatAppTerminatedNormal,
    KSatAppTerminatedUsingEndKey
    };

/**
 * Possible enumerations values for KSatIdleModeTextIconQualifier.
 */
enum TSatIdleModeTextIconQualifierValue
    {
    KSatIdleIconQInit = 0x00,
    KSatIdleIconQNoIcon = 0x01,
    KSatIdleIconQSelfExplanatory = 0x02,
    KSatIdleIconQNotSelfExplanatory = 0x04
    };

#endif // SATINTERNALPSKEYS_H

// End of File
