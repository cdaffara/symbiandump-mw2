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
* Description:  Harvester Engine to handle file and system related events
*
*/


#ifndef CHARVESTERENGINE_H
#define CHARVESTERENGINE_H

#include <e32base.h>
#include <f32file.h>
#include <bamdesca.h>
#include <mpxcollectionobserver.h>
#include <mpxtaskqueueobserver.h>
#include <mpxplaylistengineobserver.h>
#include "mpxsystemeventobserver.h"

// FORWARD DECLARATIONS
class CMPXHarvesterFileHandler;
class CMPXFsFormatMonitor;
class CMPXMediaRemovalMonitor;
class CMPXConnectionEventHandler;
class CMPXMedia;
class CMPXActiveTaskQueue;
class CMPXPlaylistEngine;
class MMPXHarvesterEngineObserver;
class MMPXCollectionUtility;
class RHvsMsg;

/**
 *  Harvester engine to monitor for file system events
 *
 *  @lib mpxharvesterserver.exe
 *  @since S60 S60_version 3.0
 */
NONSHARABLE_CLASS( CMPXHarvesterEngine ) : public CBase,
                                           public MMPXSystemEventObserver,
                                           public MMPXTaskQueueObserver,
                                           public MMPXPlaylistEngineObserver
    {
public:

    /**
    * Two-phase constructor
    */
    static CMPXHarvesterEngine* NewL();

    /**
    * Virtual destructor
    */
    virtual ~CMPXHarvesterEngine();

public:  // New Functions

    /**
    * Initiates a scan of the file system
    */
    void ScanL();

    /**
    * Cancels a scan of the file system
    */
    void CancelScan();

    /**
    * Add a song to the harvester db, asyncrhonous
    * @param aMsg Message struct for the add file
    * @param aFile path to the file
    * @param aObs observer to the add event,
    *        callback with CompleteTask() on completion
    */
    void AddFileL( RHvsMsg* aMsg, const TDesC& aFile, MMPXHarvesterEngineObserver* aObs );

    /**
    * Add a song to the harvester db
    * @param aMedia, media to add
    * @return collection ID
    */
    TInt AddFileL( CMPXMedia*& aMedia );

    /**
    * Get media for song, asyncrhonous
    * @param aMsg Message struct for the add file
    * @param aFile path to the file
    * @param aObs observer to the add event,
    *        callback with CompleteTask() on completion
    */
    void GetMediaL( RHvsMsg* aMsg, const TDesC& aFile, MMPXHarvesterEngineObserver* aObs );

    /**
    * Get Collection id for the song
    * @param aFile path to the file
    * @return collection ID
    */
    TInt GetColUidL( const TDesC& aFile );

    /**
    * Remove a song from the harvester db
    * @param aFile path to the file
    * @param aDelete, do we want to delete the file from FS
    * @return the Collection ID of the removed file
    */
    TInt RemoveFileL( const TDesC& aFile, TBool aDelete, TBool aEndTransaction=ETrue);

    /**
    * Removes multiple songs from the harvester db
    * @param aArray list of files to delete
    * @param aDelete, do we want to delete the file from FS
    */
    void RemoveMultipleFilesL( const MDesCArray& aArray, TBool aDelete, TBool aEndTransaction );

    /**
    * Remove all songs from the harvester db
    */
    void RemoveAllFilesL();

    /**
    * Update a file
    * @param aFile, File name
    * @param aCollection, Collection ID
    */
    void UpdateFileL( const TDesC& aFile, TInt aCollection );

    /**
    * Rename a file
    * @param aOldPath original file path
    * @param aNewPath new file path
    * @param aCollection Collection ID
    */
    void RenameFileL( const TDesC& aOldPath, const TDesC& aNewPath, TInt aCollection );

    /**
    * Finds the collection ID based on a File name
    * @param aFile, file name to lookup
    * @return the collection ID
    */
    TInt FindCollectionIdL( const TDesC& aFile );

    /**
    * Exports a playlist based on the message
    * @param aMessage containing the task
    * @param aObs observer to the export event
    *        callback to HandleImportCompleteL() on completion
    */
    void ExportPlaylistL( RHvsMsg* aMessage,
                          MMPXHarvesterEngineObserver* aObs );

    /**
    * Import a playlist based on the message
    * @param aMessage containing the task
    * @param aObs observer callback
    *             callback to HandleExportCompleteL() on completion
    */
    void ImportPlaylistL( RHvsMsg* aMessage,
                          MMPXHarvesterEngineObserver* aObs );

    /**
    * Async method to import a file (playlist or media )
    * @param aMessage containing the task
    * @param aObs, observer callback
    */
    void ImportFileL( RHvsMsg* aMessage,
                      MMPXHarvesterEngineObserver* aObs );

    /**
    * Query the required attributes for tracks for the specified playlist type
    * @param aPlaylistType typ of playlist to check for required attriubtes
    * @return list of required attributes for a playlist
    */
    const TArray<TMPXAttribute> RequiredAttributesL(TInt aPlaylistType) const;

    /**
    * Query the optional attributes for tracks for the specified playlist type
    * @param aPlaylistType typ of playlist to check for optional attriubtes
    * @return list of required attributes for a playlist
    */
    const TArray<TMPXAttribute> OptionalAttributesL(TInt aPlaylistType) const;

    /**
    * Query about the file extension the playlist plugin for the given type
    * supports
    * @return file extension which includes the period
    */
    const TDesC& PlaylistFileExtensionL(TInt aPlaylistType) const;

    /**
    * Determines if the given file is a playlist
    * @param aFile file path of the file to be tested
    * @return ETrue if it's a playlist otherwise EFalse
    */
    TBool IsPlaylistL( const TDesC& aFile );

    /**
    * Recreate the harvester database if corruption is detected
    * note: all dbs are re-created for accuracy and simplicity
    */
    void RecreateDatabases();

    /**
    * Polls for system events
    * If an event occurs, it will br broadcasted to the rest of the MPX framework
    */
    void CheckForSystemEvents();

    /**
    * Close the database transaction
    */
    void CloseTransactionL();

protected:  // From Base Class

    /*
    * From MMPXSystemEventObserver
    */
    void HandleSystemEventL( TSystemEvent aEvent, TInt aData );

    /*
    * From MMPXTaskQueueObserver
    */
    void ExecuteTask(TInt aTask,
                     TInt aParamData,
                     TAny* aPtrData,
                     const CBufBase& aBuf,
                     TAny* aCallback,
                     CBase* aCObject1,
                     CBase* aCObject2);

    /*
    * From MMPXTaskQueueObserver
    */
    void HandleTaskError(
        TInt aTask,
        TAny* aPtrData,
        TAny* aCallback,
        TInt aError);

     /*
     * From MMPXPlaylistEngineObserver
     */
     void HandlePlaylistL(CMPXMedia* aPlaylist,
                          const TInt aError,
                          const TBool aCompleted );

     /*
     * From MMPXPlaylistEngineObserver
     */
     void HandlePlaylistL(const TDesC& aPlaylistUri,
                          const TInt aError);

private:  // New functions

    /**
    * Handles exporting a playlist
    * @param aMsg, RMessage to handle
    */
    void DoExportPlaylistL( RHvsMsg* aMsg );

    /**
    * Handles importing a playlist
    * @param aMsg, RMessage to handle
    */
    void DoImportPlaylistL( RHvsMsg* aMsg );

    /**
    * Handles importing a file
    * @param aMsg, RMessage to handle
    */
    void DoImportFileL( RHvsMsg* aMsg );

    /**
    * Stop playback synchronously
    */
    void DoStopPlaybackL();

private:

    /**
    *  C++ constructor
    */
    CMPXHarvesterEngine();

    /**
    * 2nd-phase constructor
    */
    void ConstructL();

private: // data
    RFs   iFsSession;    // Session to File System

    CMPXHarvesterFileHandler* iFileHandler;

    // Move to RPointerArrays of monitors in the future
    CMPXFsFormatMonitor*      iFormatMonitor;
    CMPXMediaRemovalMonitor*  iMediaRemovalMonitor;
    CMPXConnectionEventHandler*      iConnectionMonitor;

    // Internal task queue for async operations
    CMPXActiveTaskQueue*      iTaskQueue;

    // Playlist engine
    CMPXPlaylistEngine*       iPlaylistEngine;

    // Async op details
    TMPXHarvesterServerOp     iCurAsyncOp;

    // Temporary variable, avoid msg queue already exist problems
    MMPXCollectionUtility*    iTempCollectionUtil;
    TBool                     iDiskOpActive;
    };

#endif // CHARVESTERENGINE_H
