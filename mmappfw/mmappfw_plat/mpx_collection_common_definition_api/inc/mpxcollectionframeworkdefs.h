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
* Description:  Common definition
*
*/


#ifndef MPXCOLLECTIONFRAMEWORKDEFS_H
#define MPXCOLLECTIONFRAMEWORKDEFS_H


#include <mpxcommonframeworkdefs.h>

// UID definition for collection mode
// For application collection mode, application UID can be used
//
const TUid KMcModeDefault = {0x1028292C};
const TUid KMcModeIsolated = {0x1028292D};
const TUid KMcModePlaylist = {0x1028292E}; // special mode used by playlist
                                           // isolated, noshareable

/**
 * This is SPECIFIC to the music db plugin and is a reference implementation.
 *
 * EMcItemDeleted, EMcItemInserted and EMcItemModified will be called back when
 * an item is deleted, added or modified via RemoveL(), AddL() and SetL()
 *
 * When user adds a new playlist through AddL, only EMcPlaylistInserted message
 * will be sent with the Id of the playlist added.
 *
 * When user adds a track to a saved playlist through AddL, only EMcPlaylistModified
 * message will be sent with the Id of the playlist modified.
 *
 * When user removes a track from the collection, the client will receive
 * EMcItemDeleted for the track deleted from the collection and
 * EMcPlaylistModified for the modified playlist to indicate which playlists
 * have been updated as result of this track removal.
 *
 * When user removes a playlist from the collection, the client will receive
 * EMcItemDeleted for the playlist removed from the collection.
 *
 * When user removes a track from the playlist, the client will receive
 * EMcPlaylistModified for the playlist the track is removed from.
 *
 * When user renames a playlist, EMcPlaylistModified is called back with the
 * new Id of the renamed playlist.
 *
 * When MTP renames a song, it will receive EMcItemModified for the track and
 * EMcPlaylistModified for the playlist because the URI of one of the songs in
 * the playlist has been changed.
 *
 * When a playlist is updated (overwrite its contents or reordering tracks in
 * the playlist), EMcPlaylistModified is sent.
 *
 * This will be replaced by KMPXMessageIdItemChanged message
 *
 * @deprecated TO BE REMOVED IN INCREMENT 9 RELEASE
 */

enum TMPXCollectionChangeType
    {
    EMcItemDeleted,      // item deleted, including removing playlist itself
    EMcItemInserted,     // item added
    EMcItemModified,     // item modified
    EMcPlaylistInserted, // new playlist added
    EMcPlaylistModified  // Existing playlist content: track added to /removed from playlist or playlist being overwritten
    };

/*
 * Simple structure to hold collection change event
 */
NONSHARABLE_STRUCT(TMPXCollectionChangeEvent)
    {
    TUid iCollectionId;
    TMPXItemId iItemId;
    TMPXCollectionChangeType iChangeType;
    };

enum TMPXCollectionPathChangeType
    {
    EMcPathChangedByOpen = 0,  // path changed due to after OpenL called
    EMcPathChangedByCollectionChange    // path changed due to collection content change
    };

enum TMPXCollectionOpenEntry
    {
    EMcItemOpened = 0,
    EMcContainerOpened
    };

enum TMPXCollectionType
    {
    EMcUnknown = 0x00,
    EMcLocal = 0x01,
    EMcRemote = 0x02
    };

enum TMPXCollectionCommand
    {
    EMcCmdClose,
    EMcCmdRefresh,
    EMcCmdRemoveAll,  // delete all items from db,
                     // aData = collection id to remove all
    EMcCloseCollection,
    EMcReOpenCollection,
    EMcRefreshStarted,
    EMcRefreshEnded,
    EMcCmdReCreateDB,
    EMcCmdSelect, // aData = index of focused item
    EMcCmdDbCorrupted,
    EMcCmdCollectionInit,
    EMcCmdCollectionResyn,
    EMcCmdMtpStart,
    EMcCmdMtpEnd
    };

enum TMPXCollectionBroadCastMsg
    {
    // This should be a 1:1 mapping to harvester events
    // for easy conversion
    // check mpxharvestercommon.h
    //
    EMcMsgFormatStart,
    EMcMsgFormatEnd,
    EMcMsgDiskRemoved,
    EMcMsgDiskInserted,
    EMcMsgUSBMassStorageStart,
    EMcMsgUSBMassStorageEnd,
    EMcMsgUSBMTPStart,
    EMcMsgUSBMTPEnd,
    EMcMsgUSBMTPNotActive,
    EMcMsgSystemEventMax,
    EMcMsgRefreshStart,
    EMcMsgRefreshEnd
    };

// Server function codes
enum TMPXCollectionServerOp
    {
    EMcsGetNextMessage,        // 0 Get next message, must be 0
    EMcsCancelGetMessage,      // 1 Cancel get message, must be 1
    EMcsSetMode,               // 2 Must be called before any following op
    EMcsOpen,                  // 3
    EMcsOpenIndex,             // 4
    EMcsOpenPath,              // 5
    EMcsGetUid,                // 6 Get uid of collection opened
    EMcsGetPath,               // 7
    EMcsBack,                  // 8
    EMcsGetSyncBuffer,         // 9 Read buffer of last sync request
    EMcsGetAsyncBuffer,        // 10 Read buffer of last async request
    EMcsMedia,                 // 11 media for current path
    EMcsMediaByPath,           // 12 media by path.
    EMcsAddItem,               // 13 Add an item
    EMcsRemovePath,            // 14 Remove a collection path
    EMcsRemoveItem,            // 15 Remove a CMPXMedia
    EMcsSetMedia,              // 16 Set a media properties
    EMcsFindAll,               // 17 Find command
    EMcsNotifyEvent,           // 18 Notify an event
    EMcsGetSupportedTypes,     // 19 Get supported Types
    EMcsCancelRequest,         // 20
    EMcsGetCapabilities,       // 21 Find the capabilities of loaded plugin
    EMcsCollectionID,          // 22 Get Collection implementation ID
    EMcsCommand,               // 23 Depricated: send a command to the server, with TInt parameter
    EMcsSetMediaAsync,         // 24 SetMediaL, Asynchronus SetL()
    EMcsCommandExt,            // 25 Send a command to the server, with CMPXMedia-based parameter
    EMcsFilter,                // 26 Get the current filter
    EMcsSetFilter,             // 27 Set the filter
    EMcsOpenByUids,            // 28 Open a list of plugins which match uids.
    EMcsCollectionIdLookup,    // 29 Collection id lookup
    EMcsServerOpEnd            // 30 End of operation,
    };

// Define Collection capability as an int, can expand later
typedef TInt TCollectionCapability;
enum
    {
    EMcSearch = 0x01,            // Supports search
    EMcSearchWildCard = 0x02,    // Supports wildcard search
    EMcNotModifiable = 0x04      // Unmodifiable collection (ie UPNP)
    };

//to indicate object type
const TInt KMPXPathUpdated = 2;
const TInt KMPXCollectionPath = 3;
const TInt KMPXCollectionEntries = 4;
const TInt KMPXCollectionMedia = 5;

// Abstract UID definition for a "collection"
// The collection server can resolve the real collection UID based on this
//
const TUid KLocalCollection = {0x101FFB30};
const TUid KPodcastCollection = {0x101FFB31};
const TUid KInMemoryCollection = {0x101FFCD7};

#endif // MPXCOLLECTIONFRAMEWORKDEFS_H

