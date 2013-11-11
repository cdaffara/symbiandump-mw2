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
* Description:  Constants and data types used by RLbt client.
*
*/


#ifndef LBTCOMMON_H
#define LBTCOMMON_H

#include <e32std.h>
#include <lbspositioninfo.h>
#include <lbtgeoareabase.h>

/**
 *  Trigger identity data type.
 *
 *  @since S60 5.1
 */
typedef TUint32 TLbtTriggerId;

/**
 *  Defines the null trigger ID. 
 * 
 *  @since S60 5.1 
 */
const TLbtTriggerId KLbtNullTriggerId = 0;

/**
 * Defines maximum length of the trigger name.
 *
 * @since S60 5.1
 */
const TInt KLbtMaxNameLength = 256;

/**
 * Error code used when a client application tries 
 * to create a trigger with trigger area smaller than
 * the system's minimum size of trigger area.
 *
 *  @since S60 5.1
 */
const TInt KErrTriggerAreaTooSmall = -20000;

/**
 * Error code used when a client application tries to create start-up
 * triggers beyond the limit specified by the system 
 *
 * @since S60 5.1
 */
const TInt KLbtErrMaxTriggerLimitExceeded = -20001;

/**
 * Error code used when a client application tries to create triggers beyond
 * the total number of enabled and valid triggers limit define by the system
 *
 * @since S60 5.1
 */
const TInt KLbtErrMaxEnabledValidTriggersLimitExceeded = -20002;

/**
 * Trigger entry attribute fields mask. 
 *
 * @see CLbtTriggerEntry::TAttribute
 * 
 * @since S60 5.1
 */
typedef TUint32 TLbtTriggerAttributeFieldsMask;

/**
 * Defines all attributes of a trigger entry are specified.
 *
 * @since S60 5.1
 */
const TLbtTriggerAttributeFieldsMask KLbtTriggerAttributeFieldsAll = 0xffffffff;

/**
 * Trigger dynamic information fields mask.
 *
 * @see TLbtTriggerDynamicInfo::TLbtDynamicInfoAttribute
 *
 * @since S60 5.1
 */
typedef TUint32 TLbtTriggerDynamicInfoFieldsMask;

/**
 * Defines all attributes of the trigger's dynamic information are specified.
 *
 * @since S60 5.1
 */
const TLbtTriggerDynamicInfoFieldsMask KLbtTriggerDynInfoFieldsAll = 0xffffffff;

/**
 * Defines the default time interval after which the trigger is set effective by the Location Triggering Server. 
 *
 * @since S60 5.1
 */
const TInt KLbtDefaultTimeToRearm = 600;

/**
 * Defines the maximum time interval after which the trigger is set effective by the Location Triggering Server. 
 *
 * @since S60 5.1
 */
const TInt KLbtMaxTimeToRearm = 2147483647;


/**
 * Defines the minimum time interval after which the trigger is set effective by the Location Triggering Server. 
 *
 * @since S60 5.1
 */
const TInt KLbtMinTimeToRearm = 0;


/**
 * Structure defines detailed information of a trigger firing event.
 *
 * @since S60 5.1
 */
struct TLbtTriggerFireInfo
    {
    /**
     * Type of trigger fired.
     */
    CLbtGeoAreaBase::TGeoAreaType iAreaType;
    	
    /**
     * The identity of the fired trigger.
     */
    TLbtTriggerId iTriggerId;

    /**
     * Position information of when the trigger was fired. Only
     * basic position information(class TPositionInfo) is returned by Location 
     * Triggering Server.
     */
    TPositionInfo iFiredPositionInfo;
    
    /**
     * For future use.
     */
    TUint8 iUnused[8];
    };

/**
 * The status of the Location triggering supervision mechanism ( on/off ).
 *
 * @since S60 5.1
 */
enum TLbtTriggeringMechanismState
    {
    ETriggeringMechanismOn = 1,   ///<Location triggering supervision mechanism is on
    ETriggeringMechanismOff = 2   ///<Location triggering supervision mechanism is off
    };

/**
 * The values defined in the  enum determine if a particular trigger would be fired soon 
 * after an update operation if the terminal is found to be inside the triggering area 
 * (entry type trigger) or outside a triggering area (exit type trigger). 
 */
enum TLbtFireOnUpdate
    {
    ELbtTrue,
    ELbtFalse,
    };
   
#endif //LBTCOMMON_H
