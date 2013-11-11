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



#ifndef CMPXCOLLECTIONCOMMANDDEFS_H
#define CMPXCOLLECTIONCOMMANDDEFS_H

#include <e32base.h>
#include <mpxattribute.h>

/**
 * Content ID identifying sync. command to retrieve URIs of the medias
 * from the path for file deletion. If a client is required to delete
 * the associated file before removing the media from the collection,
 * this command can be used. A list of URIs associated with the path
 * will be returned. If the given path ends with a media or medias in
 * a particular playlist, the returned URI array will contain 0 element
 * since client won't want to delete the associated song files in this
 * case.
 */
const TInt KMPXCommandIdCollectionRetrieveUriForDeletion = 0x10282947;
/**
 *  Attributes supported for KMPXCommandIdCollectionRetrieveUriForDeletion
 */
/**
 * CMPXCollectionPath, collection path (in/out param).
 * The collection path to be used for searching. If the path ends
 * at a particular artist/album/genre/composer, the path will be
 * expended to include all the songs under category selected.
 */
const TMPXAttributeData KMPXCommandCollectionRetrievePath = {
                        KMPXCommandIdCollectionRetrieveUriForDeletion, 0x01};
/**
 *  TInt, error code (out param)
 */
const TMPXAttributeData KMPXCommandCollectionRetrieveUriError = {
                        KMPXCommandIdCollectionRetrieveUriForDeletion, 0x02};
/**
 *  CDesCArray, an array of URIs associated with the given
 *  collection path. (out param)
 */
const TMPXAttributeData KMPXCommandCollectionRetrieveMediaUriArray = {
                        KMPXCommandIdCollectionRetrieveUriForDeletion, 0x04};

/** 
 *  Attributes supported for KMPXCommandIdCollectionRetrieveUriForDeletion
 *  @deprecated Please use constants above
 */
enum
    {
    /**
     * CMPXCollectionPath, collection path (in/out param).
     * The collection path to be used for searching. If the path ends
     * at a particular artist/album/genre/composer, the path will be
     * expended to include all the songs under category selected.
     */
    EMPXCommandCollectionRetrievePath = 0x01,

    /**
     *  TInt, error code (out param)
     */
    EMPXCommandCollectionRetrieveUriError = 0x02,

    /**
     *  CDesCArray, an array of URIs associated with the given
     *  collection path. (out param)
     */
    EMPXCommandCollectionRetrieveMediaUriArray = 0x04
    };

/**
 * Content ID identifying incremental sync. Remove command
 */
const TInt KMPXCommandIdCollectionRemove = 0x101FFC49;
/**
 *  Attributes supported for KMPXCommandIdCollectionRemove
 */
/**
 * CMPXCollectionPath, collection path (in/out param). The path to
 * remove from the collection. Upon return, an updated path is returned
 * so it can be used for the next iteration.
 */
const TMPXAttributeData KMPXCommandCollectionRemovePath = {
                                    KMPXCommandIdCollectionRemove, 0x01};
/**
 * TInt, indicates how many media to remove in one command (in param)
 * 0 or a negative value means to delete all in one go
 */
const TMPXAttributeData KMPXCommandCollectionRemoveMediaCount = {
                                    KMPXCommandIdCollectionRemove, 0x02};
/**
 *  TInt, error code (out param)
 */
const TMPXAttributeData KMPXCommandCollectionRemoveError = {
                                    KMPXCommandIdCollectionRemove, 0x04};
/**
 *  TBool, completed (out param), indicates whether the client should
 *  make further removal request
 */
const TMPXAttributeData KMPXCommandCollectionRemoveCompleted = {
                                    KMPXCommandIdCollectionRemove, 0x08};

/**
 * TBool, indicate if we should supporess collection change messages
 * Changes will be added to EMPXCommandCollectionChangeMsgs
 */
const TMPXAttributeData KMPXCommandCollectionRemoveSuppressMsgs = {
                                   KMPXCommandIdCollectionRemove,  0x10 };

/**
 * CMPXMessageArray to store collection messages
 */
const TMPXAttributeData KMPXCommandCollectionChangeMsgs = {
                                   KMPXCommandIdCollectionRemove, 0x20 };

/** 
 *  Attributes supported for KMPXCommandIdCollectionRemove
 *  @deprecated please use constants above
 */
enum
    {
    /**
     * CMPXCollectionPath, collection path (in/out param). The path to
     * remove from the collection. Upon return, an updated path is returned
     * so it can be used for the next iteration.
     */
    EMPXCommandCollectionRemovePath = 0x01,

    /**
     * TInt, indicates how many media to remove in one command (in param)
     * 0 or a negative value means to delete all in one go
     */
    EMPXCommandCollectionRemoveMediaCount = 0x02,

    /**
     *  TInt, error code (out param)
     */
    EMPXCommandCollectionRemoveError = 0x04,

    /**
     *  TBool, completed (out param), indicates whether the client should
     *  make further removal request
     */
    EMPXCommandCollectionRemoveCompleted = 0x08,

    /**
     * TBool, indicate if we should supporess collection change messages
     * Changes will be added to EMPXCommandCollectionChangeMsgs
     */
    EMPXCommandCollectionSuppressMsgs = 0x10,

    /**
     * CMPXMessageArray to store collection messages
     */
    EMPXCommandCollectionChangeMsgs = 0x20
    };

/**
 * Content ID identifying sync. Remove Media command. The identified media
 * will be deleted from the collection database instead of being marked as
 * deleted if client explictly requests to delete the records; otherwise,
 * whether the records will be deleted or marked as deleted will be based
 * on other factors (e.g. whether MTP is supported, if so, the 1st X records
 * will be marked as deleted; thereafter, they will be deleted).
 */
const TInt KMPXCommandIdCollectionRemoveMedia = 0x10282949;
/**
 *  Attributes supported for KMPXCommandIdCollectionRemoveMedia
 */
/**
 * CMPXMedia, media to be removed (in param)
 */
const TMPXAttributeData KMPXCommandCollectionRemoveMedia = {
                                    KMPXCommandIdCollectionRemoveMedia, 0x01};
/**
 * TBool, indicates whether to delete the records associated
 * with the given media (in param)
 */
const TMPXAttributeData KMPXCommandCollectionRemoveMediaDeleteRecord = {
                                    KMPXCommandIdCollectionRemoveMedia, 0x02};
/**
 *  TInt, error code (out param)
 */
const TMPXAttributeData KMPXCommandCollectionRemoveMediaError = {
                                    KMPXCommandIdCollectionRemoveMedia, 0x04};


/**
 *  Attributes supported for KMPXCommandIdCollectionRemoveMedia
 *
 *  @deprecated please use constant above
 */
enum
    {
    /**
     * CMPXMedia, media to be removed (in param)
     */
    EMPXCommandCollectionRemoveMedia = 0x01,

    /**
     * TBool, indicates whether to delete the records associated
     * with the given media (in param)
     */
    EMPXCommandCollectionRemoveMediaDeleteRecord = 0x02,

    /**
     *  TInt, error code (out param)
     */
    EMPXCommandCollectionRemoveMediaError = 0x04
    };

/**
 * Content ID identifying sync. command to clean up medias that have
 * been marked as deleted.
 */
const TInt KMPXCommandIdCollectionCleanupDeletedMedias = 0x10282948;
/**
 *  Attributes supported for KMPXCommandIdCollectionCleanupDeletedMedias
 */
const TMPXAttributeData KMPXCommandCollectionCleanupError = {
                            KMPXCommandIdCollectionCleanupDeletedMedias, 0x01};

/**
 *  Attributes supported for KMPXCommandIdCollectionCleanupDeletedMedias
 *
 *  @deprecated please use constants above
 */
enum
    {
    /**
     *  TInt, error code (out param)
     */
    EMPXCommandCollectionCleanupError = 0x01
    };

/**
 * Remove all unused data before starting delete. 
 */
const TInt KMPXCommandIdCollectionPrepareDelete= 0x10282999;
/**
 *  Attributes supported for KMPXCommandIdCollectionPrepareDelete
 */  

/**
 * CMPXCollectionPath, collection path (in/out param). The path to
 * remove from the collection.
 */    
const TMPXAttributeData KMPXCommandCollectionPrepareRemovePath = { KMPXCommandIdCollectionPrepareDelete, 0x01 };

/**
 * ContentID for completing the delete operation
 */
const TUint32 KMPXCommandIdCollectionCompleteDelete = 0x10282951;

// CMPXMessageArray containing change messages
const TMPXAttributeData KMPXCommandCollectionDeleteMsgArray = { KMPXCommandIdCollectionCompleteDelete, 0x01 };
// TBool compact database or not
const TMPXAttributeData KMPXCommandCollectionDeleteCompactDb = { KMPXCommandIdCollectionCompleteDelete, 0x02 };

/**
 *  Unique command ids for extensible commands
 */
const TUint32 KMPXCommandIdCollectionSelect = 0x10282940;      // Focused index must be
                                                               // passed in attribute KMPXCommandIdCollection,
                                                               // EMPXCollectionCommandIndex

// Content id for Collection commands (see above)
const TInt KMPXCommandContentIdCollection = 0x10282941;
const TMPXAttributeData KMPXCommandCollectionSelectIndex = { KMPXCommandContentIdCollection, 0x02 };
enum  // Attribute ids for KMPXCommandContentIdCollection
    {
    /**
     * Index, TInt
     */
    EMPXCollectionCommandIndex = 0x02
    };

/**
 * Unique command ids for add media to collection
 */
const TInt KMPXCommandIdCollectionAdd = 0x10282962;
const TMPXAttributeData KMPXCommandColAddMedia = { KMPXCommandIdCollectionAdd, 0x01 };  // CMPXMedia to add
const TMPXAttributeData KMPXCommandColAddRtnId = { KMPXCommandIdCollectionAdd, 0x02 };  // TMPXItemId on return

/**
 * Unique command ids for executing SQL statements to collection
 */
const TInt KMPXCommandIdCollectionExecSql = 0x02ABBADC;
const TMPXAttributeData KMPXCommandColSqlFile = { KMPXCommandIdCollectionExecSql, 0x01 };  // Text for SQL file name
/**
 * Unique command ids for update media in collection
 */
const TInt KMPXCommandIdCollectionSet = 0x10282963;
const TMPXAttributeData KMPXCommandColSetMedia = { KMPXCommandIdCollectionSet, 0x01 };  // CMPXMedia to set.
                                                                                        // The media can be an Item or contain an Item array

/*******************************************************************************
 * Reorder a song in a playlist
 *
 ******************************************************************************/
const TInt KMPXCommandIdReorderPlaylist = 0x10282975;

/**
 *  Attributes supported for KMPXCommandIdReorderPlaylist
 */

/**
 *  TMPXItemId, Id of the playlist to be reordered
 */
const TMPXAttributeData KMPXCommandReorderPlaylistId = { KMPXCommandIdReorderPlaylist, 0x01 };

/**
 *  TMPXItemId, Id of the song in the playlist to be reordered
 */
const TMPXAttributeData KMPXCommandReorderSongId = { KMPXCommandIdReorderPlaylist, 0x02 };

/**
 *  TUint, original ordinal of the song in the playlist
 */
const TMPXAttributeData KMPXCommandReorderOriginalOrdinal = { KMPXCommandIdReorderPlaylist, 0x04 };

/**
 *  TUint, new ordinal of the song in the playlist
 */
const TMPXAttributeData KMPXCommandReorderNewOrdinal = { KMPXCommandIdReorderPlaylist, 0x08 };


/*******************************************************************************
 * Update the last refreshed time for a collection plugin
 *
 ******************************************************************************/
const TInt KMPXCommandIdUpdateRefreshTime = 0x10282976;


/*******************************************************************************
 * Get total collection records count
 *
 ******************************************************************************/
const TInt KMPXCommandCollectionGetCount = 0x1028297C;

/**
 *  Attributes supported for KMPXCommandCollectionGetCount
 */

/**
 * TInt, drive number
 */
const TMPXAttributeData KMPXCommandCollectionCountDrive = {KMPXCommandCollectionGetCount, 0x01};


enum EMPXCollectionCountEnum
    {
    /**
     * count for all records in a music table or podcast table to be retrieved
     */
    EMPXCollectionCountTrack = 1,

    /**
     * count for all records in a playlist table to be retrieved
     */
    EMPXCollectionCountPlaylist = 2,

    /**
     *  count for total music and .m3u playlists records to be retrieved
     */
    EMPXCollectionCountTotal = 3
    };

/**
 * TInt, what content should be retrieved: music, playlist or both, see EMPXCollectionCountEnum
 */
const TMPXAttributeData KMPXCommandCollectionCountTable = {KMPXCommandCollectionGetCount, 0x02};

/**
 * TInt, collection count (out param).
 */
const TMPXAttributeData KMPXCommandCollectionCountValue = {KMPXCommandCollectionGetCount, 0x04};


/*******************************************************************************
 * Get uri list for songs and playlists for the given drive from music db
 *
 ******************************************************************************/
const TInt KMPXCommandCollectionGetURIs = 0x1028297D;

/**
 *  Attributes supported for KMPXCommandCollectionGetURIs
 */

/**
 * TInt, drive number
 */
const TMPXAttributeData KMPXCommandCollectionURIDrive = {KMPXCommandCollectionGetURIs, 0x01};

enum EMPXCollectionURIEnum
    {
    /**
     * requests URIs for records in a music or podcast table
     */
    EMPXCollectionURITrack = 1,

    /**
     * requests URIs for .m3u records in a playlist table
     */
    EMPXCollectionURIPlaylist = 2
    };

/**
 * TInt, what content should be retrieved: music or playlist, see EMPXCollectionURIEnum
 */
const TMPXAttributeData KMPXCommandCollectionURITable = {KMPXCommandCollectionGetCount, 0x02};

/**
 * TInt, number of records to retrieve
 */
const TMPXAttributeData KMPXCommandCollectionURIRecords = {KMPXCommandCollectionGetURIs, 0x04};

/**
 * TInt, ID of the record to use as a 'bottom' in SELECT
 */
const TMPXAttributeData KMPXCommandCollectionURIFromID = {KMPXCommandCollectionGetURIs, 0x08};

/**
 * CDesCArray, an array of URIs associated with the given drive (out param)
 */
const TMPXAttributeData KMPXCommandCollectionURIList = {KMPXCommandCollectionGetURIs, 0x16};

/**
 * TUint32, an ID of last retrieved URI for the array above  (out param)
 * assume that music table and playlist table are both indexed by unique ID
 * note: Symbian does not recommend to use OFFSET and instead recommend item from index
 */
const TMPXAttributeData KMPXCommandCollectionURILastID = {KMPXCommandCollectionGetURIs, 0x32};


#endif // CMPXCOLLECTIONCOMMANDDEFS_H
