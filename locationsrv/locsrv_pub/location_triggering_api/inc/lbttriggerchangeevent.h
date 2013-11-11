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
* Description:  Data class for trigger change event.
*
*/


#ifndef LBTTRIGGERCHANGEEVENT_H
#define LBTTRIGGERCHANGEEVENT_H

#include <e32std.h>
#include <lbtcommon.h>

/**
 * Specifies the type of trigger change event.
 *
 * If there is no adequate event type to describe the change,
 * @p ELbtTriggerChangeEventMultiple will be used. This event type is also 
 * used for mass operations, e.g. delete several triggers, in order to bundle
 * the events. In this case, multiple trigger items might have been changed.
 * 
 * Note, no changing event is issued if only the distance from the trigger
 * to the latest acquired location is changed.
 *
 * @since S60 5.1
 */
enum TLbtTriggerChangeEventType
    {
    /**
     * An event related to changes to multiple triggers have occurred.
     * This event is issued if more than one trigger has been added, modified 
     * or deleted.
     */
    ELbtTriggerChangeEventMultiple = 1,
    /**
     * A new trigger has been created.
     */
    ELbtTriggerChangeEventCreated = 2,
    /**
     * A trigger has been deleted.
     */
    ELbtTriggerChangeEventDeleted = 3,
    /**
     * One or more trigger entry's attributes have been updated,
     * or the trigger's validity status is changed.
     */
    ELbtTriggerChangeEventUpdated = 4,
    };

/**
 * Structure defines detailed information of a triggering change event.
 *
 * @since S60 5.1
 */
struct TLbtTriggerChangeEvent
    {
    /**
     * Type of the event.
     */
    TLbtTriggerChangeEventType iEventType;
    
    /**
     * ID of a trigger. The ID indicates that the event is associated with
     * one trigger item. If the change is not associated with exactly one
     * item, item ID will be set to @p KLbtNullTriggerId.
     * For instance in the case of @p ELbtTriggerChangeEventMultiple event.
     */
    TLbtTriggerId iTriggerId;
    
    /**
     * For future use
     */
    TUint8 iUnused[8];
    };


#endif // LBTTRIGGERCHANGEEVENT_H
