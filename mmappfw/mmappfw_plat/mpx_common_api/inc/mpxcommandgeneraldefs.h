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
* Description:  Definitions for command type
*
*/



#ifndef CMPXCOMMANDGENERALDEFS_H
#define CMPXCOMMANDGENERALDEFS_H


#include <e32base.h>
#include <mpxattribute.h>

/**
* Content ID identifying general category of content provided
* in the command object and associated attributes. This should
* be supported by ALL command objects as a common base
*/
const TInt KMPXCommandContentIdGeneral = 0x101FFC48;

/**
*  Attributes supported for KMPXCommandIdGeneral
*/
/**
*  TMPXCommandId, *unique* id of the command
*/
const TMPXAttributeData KMPXCommandGeneralId = {KMPXCommandContentIdGeneral, 0x01};
/**
*  Optional: Request to block until command is complete
*  TBool value
*/
const TMPXAttributeData KMPXCommandGeneralDoSync = {KMPXCommandContentIdGeneral, 0x02};
/**
 * Optional: TInt, Collection id
 */
const TMPXAttributeData KMPXCommandGeneralCollectionId = {KMPXCommandContentIdGeneral, 0x04};
/**
 * Optional: Unique id from client
 */
const TMPXAttributeData KMPXCommandGeneralSessionId = {KMPXCommandContentIdGeneral, 0x08};
/**
 * Optional: Collection path of source ids
 */
const TMPXAttributeData KMPXCommandGeneralSourceIds = {KMPXCommandContentIdGeneral, 0x10};
/**
 * Optional: Descriptor, Source uri
 */
const TMPXAttributeData KMPXCommandGeneralSourceUri = {KMPXCommandContentIdGeneral, 0x20};
/**
 * Optional: CMPXCollectionPath, Target ids
 */
const TMPXAttributeData KMPXCommandGeneralTargetIds = {KMPXCommandContentIdGeneral, 0x40};


/*******************************************************************************
 * ContentID for media command
 *
 ******************************************************************************/
const TInt KMPXCommandContentIdMedia = 0x10282974;

/**
 *  Attributes supported for KMPXCommandConentIdMedia
 */
/**
 * CMPXAttributeSpecs, attribute specs for the media request
 */
const TMPXAttributeData KMPXCommandMediaAttributeSpecs = { KMPXCommandContentIdMedia, 0x01 };

/**
 * TInt, index to the current item, for multiple selection
 */
const TMPXAttributeData KMPXCommandMediaIndex = { KMPXCommandContentIdMedia, 0x02 };

/**
 * TDesC, externalized version of a TMPXAttribute array
 */
const TMPXAttributeData KMPXCommandMediaAttribute = { KMPXCommandContentIdMedia, 0x04 };

/**
 * CMPXFilter, filter for the media request
 */
const TMPXAttributeData KMPXCommandMediaFilter = { KMPXCommandContentIdMedia, 0x08 };

/**
 * TCapabilitySet, set of capabilities
 */
const TMPXAttributeData KMPXCommandMediaCapbilitySet = { KMPXCommandContentIdMedia, 0x10 };

/*******************************************************************************
 * Subscription commands
 *
 ******************************************************************************/

/**
 *  Add Subscription command
 */
const TInt KMPXCommandSubscriptionAdd = 0x10282977;

/**
 *  Add subscription attributes
 */
const TMPXAttributeData KMPXCommandSubscriptionAddItems = { KMPXCommandSubscriptionAdd, 0x01 };

/**
 *  Remove subscription command
 */
const TInt KMPXCommandSubscriptionRemove = 0x10282978;

/**
 *  Remove subscription attributes
 */
const TMPXAttributeData KMPXCommandSubscriptionRemoveItems = { KMPXCommandSubscriptionRemove, 0x01 };

/**
 *  Remove all subscriptions command
 */
const TInt KMPXCommandSubscriptionRemoveAll = 0x10282979;



/** 
*  Attributes supported for KMPXCommandIdGeneral.
*
*  @deprecated Use the above consts instead
*/
enum
    {
    /**
    *  TMPXCommandId, *unique* id of the command
    */
    EMPXCommandGeneralId = 0x01,

    /**
    *  Optional: Request to block until command is complete
    *  TBool value
    */
    EMPXCommandGeneralDoSync = 0x02,

    /**
     * Optional: TInt, Collection id
     */
    EMPXCommandGeneralCollectionId = 0x04,

    /**
     * Optional: Unique id from client
     */
    EMPXCommandGeneralSessionId = 0x08,

    /**
     * Optional: Array of Source ids
     */
    EMPXCommandGeneralSourceIds = 0x10,

    /**
     * Optional: Descriptor, Source uri
     */
    EMPXCommandGeneralSourceUri = 0x20
    };

#endif // CMPXCOMMANDGENERALDEFS_H
