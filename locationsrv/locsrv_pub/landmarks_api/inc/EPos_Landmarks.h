/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Common declarations for Landmarks API
*
*/


#ifndef LANDMARKS_H
#define LANDMARKS_H

#include <e32std.h>

/**
*  Identifies landmarks and landmark categories in a landmark database.
*
*  @since S60 3.0
*/
typedef TUint32 TPosLmItemId;

/**
*  Identifies global categories.
*
*  @since S60 3.0
*/
typedef TUint16 TPosLmGlobalCategory;

/**
*  Identifies collection data.
*
*  @since S60 3.0
*/
enum TPosLmCollectionDataId
    {
    EPosLmCollDataNone = 0                              /**<
        Null collection data. */,
    EPosLmCollDataCollectionName = 1                    /**<
        Collection name. */,
    EPosLmCollDataCollectionDescription = 2             /**<
        Collection description. */,

    EPosLmCollDataContentSpecificDataBegin = 0x8000     /**<
        Start of content specific collection meta data. */,
    EPosLmCollDataLast = KMaxTUint16                    /**<
        Last available collection data id. */
    };

/**
*  Specifies what type of event has occured.
*
*  If there is no adequate event type to describe the DB change,
*  @p EPosLmEventUnknownChanges, @p EPosLmEventLandmarkUnknownChanges or
*  @p EPosLmEventCategoryUnknownChanges will be specified. This event type
*  will also be used for mass operations, e.g. delete several landmarks, in
*  order to bundle the events. This means that it is possible that multiple
*  items have been changed. In this case, all landmarks and/or category
*  information should be reread.
*
*  @since S60 3.0
*/
enum TPosLmEventType
    {
    EPosLmEventUnknownChanges = 0  /**< Unknown change event. */,

    EPosLmEventNewDefaultDatabaseLocation = 10  /**<
        This event is received if the default landmark database location is
        changed. The client has to open a new @ref CPosLandmarkDatabase handle
        to access the new default database. */,

    EPosLmEventMediaRemoved = 11  /**<
        This event is received if the media where the database is stored is
        removed. After this, the database cannot be accessed. If the media
        is inserted again, the database must still be reopened by the
        client. */,

    EPosLmEventLandmarkUnknownChanges = 100 /**<
        Unknown change event concerning only landmarks. */,

    EPosLmEventLandmarkCreated = 101 /**<
        A new landmark has been created. */,
    EPosLmEventLandmarkDeleted = 102 /**<
        A landmark has been deleted. */,
    EPosLmEventLandmarkUpdated = 103 /**<
        A landmark has been updated. */,

    EPosLmEventCategoryUnknownChanges  = 200 /**<
        Unknown change event concerning only landmark categories. */,

    EPosLmEventCategoryCreated = 201 /**<
        A new landmark category has been created. */,
    EPosLmEventCategoryDeleted = 202 /**<
        A landmark category has been deleted. */,
    EPosLmEventCategoryUpdated = 203 /**<
        A landmark category has been updated. */
    };

/**
*  Struct for landmark database change event details.
*
*  @since S60 3.0
*/
struct TPosLmEvent
    {
    TPosLmEventType iEventType;        /**< Type of event. */
    TPosLmItemId iLandmarkItemId;
    /**< ID of an involved database item. This ID indicates that the event is
         associated with one database item. If the change is not associated
         with exactly one item, item ID will be set to @p KPosLmNullItemId.
         For instance this ID will be set to @p KPosLmNullItemId for
         @p EPosLmEventUnknownChanges events. */
    TUint8 iUnused[8];                  /**< For future use. */
    };


// CONSTANTS
const TUint32 KPosLmNullItemId = 0;
const TUint16 KPosLmNullGlobalCategory = 0;

const TInt KPosLmIconMaskNotUsed = -1;

const TInt KPosLmMaxTextFieldLength = 255;
const TInt KPosLmMaxDescriptionLength = 4095;
const TInt KPosLmMaxCategoryNameLength = 124;

const TUint KPosLastParsedLandmark = KMaxTUint32;

// Landmark specific error codes
// The 20 error codes in the range -30351 to -30370 is allocated for Landmarks
// subsystem.
const TInt KLandmarksErrorBase = -30351;
const TInt KErrPosLmNotInitialized = KLandmarksErrorBase;
const TInt KErrPosLmUnknownFormat = KLandmarksErrorBase - 1;

const TInt KPosLmOperationNotComplete = 1001;

// Database secure format
_LIT(KPosLmDbSecureFormat, "secure[101FE978]");

// Database secure policy UID
const TUid KPosLmDbSecureUid = { 0x101FE978 };

// Global functions

/**
* Releases all globally allocated landmark resources.
*
* When using some landmark services, e.g. @ref CPosLandmarkDatabase,
* @ref CPosLandmarkParser and @ref CPosLandmarkEncoder, resources are allocated
* globally. To release these resources, @ref ReleaseLandmarkResources must be
* called. The safest way to do this is to always call this function from the
* client's destructor if the client uses landmark services.
*
* This function can be called any number of times.
*
* The function has an anonymous TAny pointer parameter so that the function
* can be used as a cleanup function for @p TCleanupItem. The pointer is not
* used.
*
* @since S60 3.0
*/
IMPORT_C void ReleaseLandmarkResources(TAny* = NULL);

#endif      // LANDMARKS_H


