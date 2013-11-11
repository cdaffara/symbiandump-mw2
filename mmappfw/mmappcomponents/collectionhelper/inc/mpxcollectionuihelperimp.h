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
* Description:  Collection Ui helper implementation
*
*/


#ifndef CMPX_COLLECTION_UI_HELPER_H
#define CMPX_COLLECTION_UI_HELPER_H

#include <mpxcollectionobserver.h>
#include <mpxcollectionpath.h>
#include <mpxtaskqueueobserver.h>
#include <mpxcollectionuihelperobserver.h>
#include "mpxcollectionuihelper.h"
#include "mpxmediatorobserver.h"
#include "mpxmoveobserver.h"
#include "mpxharvesterutilityobserver.h"

// FOWARD DECLARATION
class MMPXCollectionUtility;
class CMPXCollectionMediator;
class MMPXHarvesterUtility;
class CMPXMedia;
class TMPXCollectionMessage;
class CMPXActiveTaskQueue;
class CMPXDeleteHelper;

/**
 *  CMPXCollectionUiHelper
 *
 *  Implementation of the collection ui helper interface
 *  for system specific changes
 *
 *  @lib collectionhelper.lib
 *  @since S60 3.0
 */
NONSHARABLE_CLASS( CMPXCollectionUiHelperImp ): public CBase,
                                          public MMPXCollectionUiHelper,
                                          public MMPXMediatorObserver,
                                          public MMPXHarvesterUtilityObserver,
                                          public MMPXCollectionObserver,
                                          public MMPXTaskQueueObserver,
                                          public MMPXMoveItemObserver,
                                          public MMPXCHelperObserver

    {
public:

    /**
    * Standard 2-phased constructor
    * @param aModeId collection mode id. see mpxcollectionutility.h
    *        for details.
    * @return new instance of the collection ui helper
    */
    static CMPXCollectionUiHelperImp* NewL(const TUid& aModeId);

    /**
    * Standard 2-phased constructor
    * @param aModeId collection mode id. see mpxcollectionutility.h
    *        for details.
    * @return new instance of the collection ui helper
    */
    static CMPXCollectionUiHelperImp* NewLC(const TUid& aModeId);

    /**
    * Virtual Destructor
    */
    virtual ~CMPXCollectionUiHelperImp();

protected:

    /**
    * From MMPXCollectionUiHelper
    *
    * This is an async call to create a playlist or add songs to
    * a saved playlist. In the case of creating a new playlist,
    * no physical playlist files will be created. This type of
    * playlists is referred to as virtual playlists. In the case
    * of adding songs to a saved playlist, if the playlist
    * corresponds to a playlist file, the playlist file is NOT
    * updated.
    *
    * Upon completion of this request, HandleAddCompletedL is called.
    *
    * To add a playlist to the collection, the media provided MUST
    * contain the following attributes:
    *    KMPXMediaGeneralType:
    *       must be EMPXItem
    *
    *    KMPXMediaGeneralCategory:
    *       must be EMPXPlaylist
    *
    *    KMPXMediaGeneralUri:
    *       This is the path where the playlist should be located.
    *       e.g. c:\data\playlists\ or e:\playlists\. When the playlist
    *       is successfully added to the collection, its URI will be
    *       generated. If client is interested its URI, client can
    *       perform a search based on the returned Id of the playlist.
    *       Currently only supporting local path, i.e. the path provided
    *       must contain starts with a drive letter.
    *
    *    KMPXMediaGeneralTitle:
    *       Title of the playlist is NOT enforced be unique within the
    *       music collection.
    *
    *    KMPXMediaArrayCount:
    *    KMPXMediaArrayContents:
    *       This playlist can be an empty playlist. If this is the case,
    *       KMPXMediaArrayContents will be a CMPXMediaArray that doesn't
    *       contain any media and KMPXMediaArrayCount will be 0.
    *
    *       Each CMPXMedia contained in the media array represents
    *       a song. Each song must contain the following attributes:
    *           KMPXMediaGeneralType:
    *               must be EMPXItem
    *
    *           KMPXMediaGeneralCategory:
    *               must be EMPXSong
    *
    *       For songs that already exist in the collection, client
    *       must provide either KMPXMediaGeneralId or KMPXMediaGeneralUri.
    *
    *       For songs that do not exist in the collection, client must
    *       provide KMPXMediaGeneralUri and its metadata. These songs
    *       will be added to the music collection when the playlist is
    *       added. These songs added to the collection are NOT registered
    *       with Harvester; hence, during next scan/refresh, these songs
    *       will be scanned and updated in the collection unnecessarily
    *       and cause scan/refresh performance degradation. If any
    *       metadata of these songs is changed in the collection before
    *       next scan/refresh, it will be lost after scan/refresh.
    *
    *       WARNING:
    *       Client must add the songs to the collection prior to adding
    *       the playlist.
    *
    *    WARNING:
    *    This playlist is NOT registered with Harvester. If client
    *    creates a physical playlist file, Harvester will re-parse
    *    and update the playlist in the collection during the next
    *    scan/refresh unnecessarily and cause scan/refresh performance
    *    degradation. If this playlist is edited, the physical
    *    playlist file is NOT updated to reflect the changes.
    *
    *    If the playlist is successfully added to the collection,
    *    the media returned in the callback will contain the Id
    *    of the playlist.
    *
    * To add songs to a saved playlist, the media provided MUST
    * contain the following attributes:
    *    KMPXMediaGeneralType:
    *       must be EMPXItem
    *
    *    KMPXMediaGeneralCategory:
    *       must be EMPXPlaylist
    *
    *    This playlist must already exist in the collection, client
    *    must provide either KMPXMediaGeneralId or KMPXMediaGeneralUri
    *    for locating which playlist to add the songs to.
    *
    *    KMPXMediaArrayCount:
    *       indicates the number of songs to be added to the playlist
    *
    *    KMPXMediaArrayContents:
    *       This contains an array of songs to be added/appended
    *       to the playlist. Each media in the array represents
    *       a song. Each song must contain the following attributes:
    *           KMPXMediaGeneralType:
    *               must be EMPXItem
    *
    *           KMPXMediaGeneralCategory:
    *               must be EMPXSong
    *
    *       These songs must already exist in the collection. Client
    *       must provide either KMPXMediaGeneralId or KMPXMediaGeneralUri.
    *
    * This method will leave with KErrArgument if any mandatory
    * is missing.
    *
    * @param aMedia, a media to be added to the system
    * @param aObserver, an observer for callback when
    *        operation is compelte. Leaves with KErrArgument if
    *        observer provided is NULL. The Id of playlist will
    *        be accessible via the KMPXMediaGeneralId of the media
    *        returned in the callback.
    *
    */
    void AddL( const CMPXMedia& aMedia,
               MMPXCHelperObserver* aObserver );
   
    
    /**
    * From MMPXCollectionUiHelper    
    *
    * This is exactly the same as AddL except that songs in aMedia
    * is added incrementally (in chunks).
    * It should accept the same input and the same output as if AddL
    * is used.
    * @param aMedia, a media to be added to the system
    * @param aObserver, an observer for callback when
    *        operation is compelte. Leaves with KErrArgument if
    *        observer provided is NULL. The Id of playlist will
    *        be accessible via the KMPXMediaGeneralId of the media
    *        returned in the callback.
    * @param aSize, the number of songs to be added in each chunk
    */
    void IncAddL( const CMPXMedia& aMedia,
                  MMPXCHelperObserver* aObserver,
                  const TInt aSize );

    /**
    * From MMPXCollectionUiHelper
    *
    * Asynchronously updates a media in the collection. For performance
    * consideration, it is recommended that client only sets the attribute
    * that needs to be updated.
    *
    * @param aMedia, media to be updated.
    *
    *        To update a song in the collection, the media provided MUST
    *        contain the following attributes:
    *           KMPXMediaGeneralType:
    *               must be EMPXItem
    *
    *           KMPXMediaGeneralCategory:
    *               must be EMPXSong
    *
    *           KMPXMediaGeneralId or KMPXMediaGeneralUri:
    *               for identifying which song to update
    *
    *        Additionally, this media should provide one or more of the
    *        following attributes for update:
    *               KMPXMediaGeneralTitle
    *               KMPXMediaGeneralComment
    *               KMPXMediaGeneralSynchronized
    *               KMPXMediaGeneralDeleted
    *               KMPXMediaGeneralModified
    *               KMPXMediaGeneralCopyright
    *               KMPXMediaGeneralDuration
    *               KMPXMediaGeneralFlags
    *               KMPXMediaGeneralPlayCount
    *               KMPXMediaGeneralLastPlaybackTime
    *           KMPXMediaIdMusic:
    *               KMPXMediaMusicArtist
    *               KMPXMediaMusicAlbum
    *               KMPXMediaMusicAlbumTrack
    *               KMPXMediaMusicGenre
    *               KMPXMediaMusicComposer
    *               KMPXMediaMusicYear
    *               KMPXMediaMusicRating
    *               KMPXMediaMusicAlbumArtFileName
    *               KMPXMediaMusicURL
    *           KMPXMediaIdAudio:
    *               KMPXMediaAudioSamplerate
    *               KMPXMediaAudioBitrate
    *               KMPXMediaAudioNumberOfChannels
    *           KMPXMediaIdDrm:
    *               KMPXMediaDrmType
    *           KMPXMediaIdMTP:
    *               KMPXMediaMTPDrmStatus
    *
    *        To update a playlist in the collection, the media provided MUST
    *        contain the following attributes:
    *           KMPXMediaGeneralType:
    *               must be EMPXItem
    *
    *           KMPXMediaGeneralCategory:
    *               must be EMPXPlaylist
    *
    *           KMPXMediaGeneralId or KMPXMediaGeneralUri:
    *               for identifying which playlist to update
    *
    *        If the media contains KMPXMediaArrayContents attribute, the
    *        content of the playlist will be overwritten with the new media
    *        array. If this attribute contains a CMPXMediaArray of 0 element,
    *        all songs in this playlist will be removed from the playlist
    *        (songs will remain in the collection and file system).
    *        e.g.
    *        1) "Playlist" originally contains "Song1", "Song2", and "Song3".
    *           If KMPXMediaArrayContents contains a CMPXMediaArray that
    *           contains "Song3" and "Song4", "Playlist" will be updated to
    *           contain "Song3" and "Song4".
    *        2) "Playlist" originally contains "Song1", "Song2", and "Song3".
    *           If KMPXMediaArrayContents contains a CMPXMediaArray that
    *           contains 0 element, "Playlist" will be updated so that it
    *           doesn't contain any songs. "Song1", "Song2", and "Song3"
    *           will remain in the collection.
    *
    *       If the media contains any of the following attributes, the
    *       playlist will be updated accordingly:
    *               KMPXMediaGeneralTitle
    *               KMPXMediaGeneralSynchronized
    *               KMPXMediaGeneralDeleted (DEPRECATED)
    *               KMPXMediaGeneralModified (DEPRECATED)
    *
    *       NOTE: If a client wishes to change the playlist title and modify
    *             its contents (i.e. the songs it includes), client needs to
    *             perform two separate SetL operations; one to update the
    *             playlist title providing KMPXMediaGeneralTitle attribute
    *             and the other to update its contents by providing
    *             KMPXMediaArrayContents.
    *
    * @param aObserver, an observer for callback when operation is complete.
    *        KErrArgument if NULL.
    */
    void SetL( CMPXMedia*& aMedia,
               MMPXCHelperObserver* aObserver );

    /**
    * From MMPXCollectionUiHelper
    *
    * Asynchronously updates a list of medias in the collection. This is
    * NOT currently implemented.
    *
    * @param aMediaArray, list of medias
    * @param aPath, collection path
    * @param aIndices, array of indicies to collection path
    */
    void SetL( TArray<CMPXMedia*>& aMediaArray,
               CMPXCollectionPath& aPath,
               RArray<TInt>& aIndices);

    /**
    * From MMPXCollectionUiHelper
    *
    * Asynchronously renames a media. This method will be DEPRECATED as client
    * can use SetL to update the title of a song or a playlist.
    *
    * @param aMedia media to be renamed
    *
    *        Currently only supporting renaming of a playlist. This media
    *        MUST contain the following attributes:
    *           KMPXMediaGeneralType:
    *               must be EMPXItem
    *
    *           KMPXMediaGeneralCategory:
    *               must be EMPXPlaylist
    *
    *           KMPXMediaGeneralId:
    *               for identifying which playlist to rename
    *
    *           KMPXMediaGeneralTitle:
    *               new title of the playlist
    *
    *        This method will leave with KErrArgument if any mandatory is
    *        missing. URI of the playlist remains unchanged when its title
    *        is changed.
    *
    * @param aObserver an observer for callback when operation is complete.
    *        KErrArgument if NULL.
    */
    void RenameL( const CMPXMedia& aMedia,
                  MMPXCHelperObserver* aObserver );

    /**
    * From MMPXCollectionUiHelper
    *
    * Asynchronously deletes a media or medias based on the collection path.
    * If this collection path refers to multiple medias, the referred medias
    * are deleted one by one. If the media correlates to a file in the file
    * system, the correlating file is deleted from the file system. If the
    * file is in use, operation will proceed to the next media and upon
    * operation completion, KErrInUse is returned via callback.
    * If the file has already been deleted from the file system, operation
    * will also proceed to the next media as normal.
    *
    * This operation is cancelable via Cancel(),
    *
    * @param aPath collection path to the media to be removed
    * @param aObserver an observer for callback when operation is complete.
    *        KErrArgument if NULL.
    */
    void DeleteL( CMPXCollectionPath& aPath,
                  MMPXCHelperObserver* aObserver );

    /**
    * From MMPXCollectionUiHelper
    *
    * Move an item from one collection to another
    *
    * @param aMedia media to be moved.
    * @param aNewCollection uid of the collection defined in collectionplugins.hrh
    * @param aObserver observer for the event. if aObs is NULL, sync, not NULL async.
    */
    void MoveL( CMPXMedia*& aMedia,
                TUid aNewCollection,
                MMPXCHelperObserver* aObserver = NULL );  //lint !e1735

    /**
    * From MMPXCollectionUiHelper
    * Open the collection in embedded mode with a Media Object
    * @param aHostId Host Process UID to identify this embedded instance
    * @param aMedia Media Object to open
    * @param aObserver observer to handle errors
    * @param aPluginInfo additional argument to pass to the plugin resolver
    */
    void OpenL( const TUid& aHostId, CMPXMedia& aMedia,
                MMPXCHelperEmbeddedOpenObserver* aObserver,
                TInt aPluginInfo = 0 );  //lint !e1735

    /**
    * From MMPXCollectionUiHelper
    * Open the collection in embedded mode with a File Handle
    * @param aHostId Host Process UID to identify this embedded instance
    * @param aFile File handle that we have to process
    * @param aObserver observer to handle errors

    */
    void OpenL( const TUid& aHostId, const TDesC& aFile,
                MMPXCHelperEmbeddedOpenObserver* aObserver,
                TInt aPluginInfo = 0 );  //lint !e1735

    /**
    * From MMPXCollectionUiHelper
    * Open the collection in embedded mode with a File Handle
    * @param aFile File handle that we have to process
    * @param aObserver, observer to the open operation
    */
    void OpenL( RFile& aFile, MMPXCHelperEmbeddedOpenObserver* aObserver );

    /**
    * From MMPXCollectionUiHelper
    * Queries about the playlist file extension of the currently selected playlist
    * plugin
    * @return file extension of the playlist type which includes the period
    */
    HBufC* PlaylistFileExtensionLC();

    /**
    * From MMPXCollectionUiHelper
    * Export the specified playlist to a file
    * @param aPlaylistId, id of the playlist to be exported
    * @param aDestinationDriveAndPath, specifies where the playlist file should
    *        be created. If the file already exists, it will be overwritten.
    * @param aObs observer for the callback. URI of the exported playlist
    *        will be returned as a HBufC* which client takes over the
    *        ownership
    */
    void ExportPlaylistL(TMPXItemId aPlaylistId,
                         const TDesC& aDestinationDriveAndPath,
                         MMPXCHelperObserver* aObserver);

    /**
    * From MMPXCollectionUiHelper
    * Reorder a song in the playlist
    * @param aPlaylistId id of the playlist which contains the song to be reordered
    * @param aSongId id of the song to be reordered
    * @param aOriginalOrdinal the original ordinal of the song within the playlist.
    *        Ordinal starts from 0.
    * @param aNewOrdinal the new ordinal of the song within the playlist. Ordinal
    *        starts from 0.
    * @param aObs observer for the callback
    */
    void ReorderPlaylistL(const TMPXItemId& aPlaylistId,
                          const TMPXItemId& aSongId,
                          TUint aOriginalOrdinal,
                          TUint aNewOrdinal,
                          MMPXCHelperObserver* aObserver);

    /**
    * From MMPXCollectionUiHelper
    * Gets the collection path to the main music menu
    * Call OpenL() with this path to open music menu
    * @return CMPXCollectionPath*, ownership transferred
    */
    CMPXCollectionPath* MusicMenuPathL();

    /**
    * From MMPXCollectionUiHelper
    * Gets the collection path to the podcast menu
    * Call OpenL() with this path to open podcast menu
    * @return CMPXCollectionPath*, ownership transferred
    */
    CMPXCollectionPath* PodCastMenuPathL();

    /**
    * From MMPXCollectionUiHelper
    * Gets the collection path to the main all songs view
    * Call OpenL() with this path to open music all songs view
    * @return CMPXCollectionPath*, ownership transferred
    */
    CMPXCollectionPath* MusicAllSongsPathL();

    /**
    * From MMPXCollectionUiHelper
    * Gets the collection path to the music playlist
    * Call OpenL() with this path to open music playlist menu
    * @return CMPXCollectionPath*, ownership transferred
    */
    CMPXCollectionPath* MusicPlaylistPathL();

    /**
    * From MMPXCollectionUiHelper
    * Gets the collection path to the music playlist
    * Call OpenL() with this path to open music playlist menu
    * @param aPlaylistType type of auto playlist
    * @return CMPXCollectionPath*, ownership transferred
    */
    CMPXCollectionPath* MusicPlaylistPathL(TMPXAutoPlaylistType aPlaylistType);

    /**
    * From MMPXCollectionUiHelper
    * Creates a default playlist path to open. Hardcoded to Music Collection / All songs
    * @return CollectionPath*, ownership transferred.
    */
    CMPXCollectionPath* CreateDefaultPlaylistPathLC();

    /**
    * From MMPXCollectionUiHelper
    * Cancels current async request
    * NOTE: Currently only Cancelling DeleteL() is supported
    */
    void Cancel();

    /**
    * From MMPXCollectionUiHelper
    * Frees this object
    */
    void Close();

    /**
    * From MMPXCollectionUiHelper
    * Test if the specified title exists in the specified category
    * @param aCategory specifies the category for testing
    * @param aTitle specifies the title for testing
    * @return ETrue if the specified title exists in the specified
    *         category; otherwise, EFalse
    */
    TBool TitleExistsL( TMPXGeneralCategory aCategory, 
                        const TDesC& aTitle, TMPXGeneralType aType = EMPXItem );

    /**
    * From MMPXCollectionUiHelper
    * Returns the file name used for virtual playlist handling
    * @return the file name to be used to externalize collection path for the playlist
    *         that needs to be handled. ownership transfered
    */
    HBufC* ExternalCollectionPathHandlingFileNameLC();

private: // from base class

    /**
    * From MMPXMediatorObserver
    * @param aMedia, properties of the object
    * @param aOldPath, old collection path
    */
    void HandleMediatorPathUpdatedL( CMPXMedia*& aMedia,
                                     TUid  aOldPath  );

    /**
    * From MMPXHarvesterUtilityObserver
    * Handles completion of playlist export
    * @param aMedia, media that has been exported to a playlist file
    * @param aErr, error encountered during playlist export
    */
    void HandlePlaylistExportCompletedL( CMPXMedia* aMedia, TInt aErr );

    /**
    * From MMPXHarvesterUtilityObserver
    * Handles completion of playlist import
    * @param aMedia, media imported from a playlist file.
    * @param aErr, error encountered during playlist import
    */
    void HandlePlaylistImportCompletedL( CMPXMedia* aMedia, TInt aErr );

    /**
    * From MMPXHarvesterUtilityObserver
    * Handle asynchronous file addition by file name
    * @param aMedia, media added to the system
    * @param aErr, error encountered during AddL operation
    */
    void HandleFileAddCompletedL( CMPXMedia* aMedia, TInt aErr );

    /**
    * From MMPXHarvesterUtilityObserver
    * Handle asynchronous file addition by file name
    * @param aMedia, media imported from file
    * @param aErr, error encountered during the operation
    */
    void HandleFileImportCompletedL( CMPXMedia* aMedia, TInt aErr  );

    /**
    * From MMPXHarvesterUtilityObserver
    * Handle file delete completion
    * @param aErr, error encountered during delete operation
    */
    void HandleDeleteCompletedL( TInt aErr );

    /**
    * From MMPXHarvesterUtilityObserver
    * Handle media retrieving completion
    * @param aErr, error encountered during delete operation
    */
    void HandleFileGetMediaCompletedL( CMPXMedia* aMedia, TInt aErr  );

    /**
    * From MMPXCollectionMediaObserver
    *  Handle extended media properties
    *
    *  @param aMedia media
    *  @param aError error code
    */
    void HandleCollectionMediaL(const CMPXMedia& aMedia,
                                TInt aError);

    /**
    * From MMPXCollectionObserver
    *  Handle collection message
    *  @param aMessage collection message
    *  @param aErr error
    */
    void HandleCollectionMessage(CMPXMessage* aMsg, TInt aErr);

    /**
    * From MMPXCollectionObserver
    *  Handles the collection entries being opened. Typically called
    *  when client has Open()'d a folder
    *
    *  @param aEntries collection entries opened
    *  @param aIndex focused entry
    *  @param aComplete ETrue no more entries. EFalse more entries
    *                   expected
    *  @param aError error code
    */
    void HandleOpenL(const CMPXMedia& aEntries,
                     TInt aIndex,TBool aComplete,TInt aError);

    /**
    * From MMPXCollectionObserver
    *  Handles the item being opened. Typically called
    *  when client has Open()'d an item. Client typically responds by
    *  'playing' the item via the playlist
    *
    *  @param aPlaylist collection playlist
    *  @param aError error code
    */
    void HandleOpenL(const CMPXCollectionPlaylist& aPlaylist,TInt aError);

    /**
    * From MMPXCollectionObserver
    *  Handles completion of a asynchronous command.
    *
    *  @param aCommandResult result of the command, NULL if error
    *  @param aError error code    
    */
    virtual void HandleCommandComplete(CMPXCommand* aCommandResult, 
                                       TInt aError);

    /**
    * From MMPXMoveItemObserver
    * @param aErr, error for the operation
    */
    void HandleMoveCompleteL( TInt aErr );

    /**
    * From MMPXCHelperObserver
    *  Handle callback for delete operation
    *  @param aOperation operation type that is completed
    *  @param aErr error code for the operation
    *  @param aArgument a pointer to any data to retun
    */
    void HandleOperationCompleteL( TCHelperOperation aOperation,
                                   TInt aErr,
                                   void* aArgument );

    /**
    * From MMPXTaskQueueObserver
    * Execute a task
    * @param aTask task number
    * @param aParamData, parameter
    * @param aPtrData, any object
    * @param aBuf, buffer containing externalized parameters for the task.
    * @param aCallback call back function pointer
    * @param aCObject1 object 1 owned by task queue
    * @param aCObject2 object 2 owned by task queue
    */
    void ExecuteTask(TInt aTask,
                     TInt aParamData,
                     TAny* aPtrData,
                     const CBufBase& aBuf,
                     TAny* aCallback,
                     CBase* aCObject1,
                     CBase* aCObject2);

    /**
    * @see MMPXTaskQueueObserver
    */
    void HandleTaskError(
        TInt aTask,
        TAny* aPtrData,
        TAny* aCallback,
        TInt aError);

private:

    /**
    * Constructor
    */
    CMPXCollectionUiHelperImp();

    /**
    * 2nd-phase constructor
    *  @param aModeId collection mode id
    */
    void ConstructL(const TUid& aModeId);

   /**
    * Execute a task
    * @param aTask task number
    * @param aBuf, externalized parameters for the task
    * @param aCallback, callback for the task
    */
    void ExecuteTaskL(TInt aTask, const CBufBase& aBuf, TAny* aCallback);

    /**
    * Completes the current task
    *
    * @param aTask task number
    * @param aError error code
    */
    void CompleteTask(TInt aTask, TInt aError);

    /**
    * Notify client of the task results
    *
    * @param aTask task number
    * @param aError error code
    */
    void NotifyClientL(TInt aTask, TInt aError);

    /**
    * Cleanup before processing the next task
    */
    void Cleanup();

    /**
    * Performs AddL task from the task queue
    */
    void DoAddL();
             
    /**
    * Performs IncAddMedialL task from the task queue
    */
    void DoIncAddMediaL();
    
    /**
    * Performs IncAppendMedialL task from the task queue
    */
    void DoIncAppendMediaL();
    
    /**
    * Performs SetL task from the task queue
    */
    void DoSetL();

    /**
    * Performs RenameL task from the task queue
    */
    void DoRenameL();

    /**
    * Export the specified playlist to a file
    * @param aPlaylistId, id of the playlist to be exported
    * @param aDestinationDriveAndPath, specifies where the playlist file should
    *        be created. If the file already exists, it will be overwritten.
    */
    void DoExportPlaylistL(TMPXItemId aPlaylistId,
                           const TDesC& aDestinationDriveAndPath);

    /**
    * Reorder a song in the playlist
    * @param aCommand command to reorder a song in the playlist
    */
    void DoReorderPlaylistL(CMPXCommand& aCommand);

    /**
    * Fill in details for the playlist itself for playlist export
    * @param aMedia, a playlist media whose information is to be
    *        filled.
    */
    void FillInPlaylistDetailsL(CMPXMedia& aMedia);

    /**
    * Handles completion of playlist export
    * @param aMedia, media that has been exported to a playlist file
    * @param aErr, error encountered during playlist export
    */
    void DoHandlePlaylistExportCompletedL( CMPXMedia* aMedia, TInt aErr );

    /**
    *  Looks for collection Id from the given URI through its extension.
    *  This method is able to determine the collection Id without the media
    *  being in the harvester's database.
    *  @param aUri URI of a media
    *  @return collection Id. KErrNotFound if unable to find a collection
    *          that supports the type of extension
    */
    TInt FindCollectionIdL(const TDesC& aUri);


private:

    // tasks
    enum TMPXCollectionUiHelperTask
        {
        ETaskNone,
        ETaskAddMedia,
        ETaskAppendMedia,
        ETaskSetMedia,
        ETaskRemoveMedia,
        ETaskRenameMedia,
        ETaskExportPlaylist,
        ETaskReorderPlaylist,
        ETaskIncAddMedia,
        ETaskIncAppendMedia,
        ETaskIncFinish
        };

private: // data

    MMPXCollectionUtility*              iCollection;
    CMPXCollectionMediator*             iMediator;
    MMPXHarvesterUtility*               iHarvester;
    CMPXActiveTaskQueue*                iTaskQueue;
    TInt                                iTask;

    CMPXDeleteHelper*                   iDeleteHelper;

    // parameters for async commands
    CMPXMedia*                          iMedia;
    
    // parameters used for incremental adding of songs
    CMPXMedia*                          iInputMedia;
    TInt                                iRemainder;
    TInt                                iTotalChunkNumber;
    TInt                                iChunkNumber;  // used as ID of current chunk
    TInt                                iChunkSize;
    TInt                                iArrayIndex;

    
    MMPXCHelperEmbeddedOpenObserver*    iOpenObserver; // not owned, transient

    MMPXCHelperObserver*                iHelperObserver;  // not owned, transient
    // Embedded Mode handling
    TUid                                iEmbeddedModeID;
    TInt                                iEmbeddedPluginInfo;
    TBool                               iInitialized;  // collection db merging
    TBool                               iIncAdding;  // Incremental adding guard
    TInt 								iRefCount;
    TUid                                iModeId;
    };

#endif // CMPX_COLLECTION_UI_HELPER_H
