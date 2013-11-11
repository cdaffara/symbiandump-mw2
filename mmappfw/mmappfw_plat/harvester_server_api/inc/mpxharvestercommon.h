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
* Description:  Common declarations
*
*/

#ifndef MPX_HARVESTER_COMMON_H
#define MPX_HARVESTER_COMMON_H

// Constants
const TInt KBatchCommit = 100;  // # changes to DB before commit.


// Supported server ops
enum TMPXHarvesterServerOp
    {
    EHvsServerGetBuffer,      // Get the current buffer stored in the session
    EHvsServerScan,           // Initiate a scan of the FS
    EHvsServerCancelScan,     // Cancel a scan
    EHvsServerShutdown,       // Shutdown the server
    EHvsServerAddFile,        // Add an item to the harvester db
    EHvsServerAddFileMedia,   // Add an item based on a media
    EHvsServerRemoveFile,     // Remove an item from harvester db
    EHvsServerRemoveFiles,    // Remove multiple items from the harvester db
    EHvsServerRemoveAllFiles, // Remove all songs from harvester db
    EHvsServerUpdateFile,     // Update a file with new time stamp and collection
    EHvsServerRenameFile,     // Rename a file
    EHvsServerExportPlaylist, // Export a playlist
    EHvsServerImportPlaylist, // Import a playlist
    EHvsServerFindColID,      // Find the collection ID for an item
    EHvsServerReqAttr,        // Query the required attributes for tracks for a playlist type
    EHvsServerOptAttr,        // Query the optional attributes for tracks for a playlist type
    EHvsServerPlaylistFileExt,// Get file extension of a playlist plugin
    EHvsServerIsPlaylist,     // Test if a file is a playlist
    EHvsServerImportFile,     // Import a "file"
    EHvsServerReCreateDB,     // Delete the old databases and re-create from fresh
    EHvsServerCheckSystemEvent, // Check for system events and broadcast if any are active
    EHvsServerGetMedia,       // Get a media for the file
    EHvsServerGetColUid,      // Get a collection id for the file
    EHvsServerCloseTransaction, // Close the database transaction
    EHvsServerOpCount
    };

// System events
enum TSystemEvent
    {
    // This should be a 1:1 mapping to TMPXCollectionBroadCastMsg events
    // for easy conversion
    // check mpxcollectionframeworkdefs.h
    //
    EFormatStartEvent,         // Start of a formatting event
    EFormatEndEvent,           // End of a formatting event
    EDiskRemovedEvent,         // Disk removed event
    EDiskInsertedEvent,        // Disk Inserted event
    EUSBMassStorageStartEvent, // USB Mass storage started event
    EUSBMassStorageEndEvent,   // USB Mass storage ended event
    EUSBMTPStartEvent,         // MTP start event
    EUSBMTPEndEvent,           // MTP end event
    EUSBMTPNotActiveEvent,     // MTP selected but not active
    ESystemEventMax,           // End of event enum
    EDiskDismountEvent         // Disk dismount notification (MPX Harvester internal)
    };

#endif // MPX_HARVESTER_COMMON_H

