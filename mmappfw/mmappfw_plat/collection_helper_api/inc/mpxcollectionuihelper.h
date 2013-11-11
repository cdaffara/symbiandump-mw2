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
* Description:  Collection Ui Helper interface class
*
*/

#ifndef MMPXCOLLECTIONUIHELPER_H
#define MMPXCOLLECTIONUIHELPER_H

#include <mpxmediageneraldefs.h>
#include <mpxcommonframeworkdefs.h>
#include <mpxmediageneraldefs.h>

// FORWARD DECLARATIONS
class CMPXCollectionPath;
class CMPXCollectionPlaylist;
class CMPXMedia;
class MMPXCHelperObserver;
class MMPXCHelperEmbeddedOpenObserver;

/**
 *  Abstract class for collection ui helper. Performs operation on playlist object. 
 *  Enables client to add, rename, etc a song to a playlist or add a particular property.
 *
 *  @lib mpxcollectionhelper.lib
 */
NONSHARABLE_CLASS( MMPXCollectionUiHelper )
    {
public:

    /**  Auto playlist type */
    enum TMPXAutoPlaylistType
        {
        EMPXRecentlyPlayedPlaylist,
		EMPXMostPlayedPlaylist,
        EMPXRecentlyAddedPlaylist
        };

    /**
    * Add a media. Currently only adding a playlist is
    * supported.
	* @since S60 3.2.3
    * @param aMedia media to be added to the
    *        system. media type and category
    *        are mandatory attributes. Depending
    *        on the operation additional attributes
    *        may be required.
    * @param aObserver observer to callback on completion 
    *        of the request
    */
    virtual void AddL( const CMPXMedia& aMedia,
                       MMPXCHelperObserver* aObserver ) = 0;

    /**
    * Update a file with the particular media property.
	* @since S60 3.2.3
    * @param aMedia media to update.
    * @param aObserver observer to callback on completion 
    *        of the request
    */
    virtual void SetL( CMPXMedia*& aMedia,
                       MMPXCHelperObserver* aObserver ) = 0;

    /**
    * Updates a list of properties.
	* @since S60 3.2.3
    * @param aMediaArray list of media items.
    * @param aPath collection path.
    * @param aIndices an array of indicides to a collection path.
    * @note this is not implemented because there is no use case
    */
    virtual void SetL( TArray<CMPXMedia*>& aMediaArray,
                       CMPXCollectionPath& aPath,
                       RArray<TInt>& aIndices) = 0;

    /**
    * Rename a media.
	* @since S60 3.2.3
    * @param aMedia media to be renamed.
    * @param aObserver an observer for callback when
    *        operation is complete
    */
    virtual void RenameL( const CMPXMedia& aMedia,
                          MMPXCHelperObserver* aObserver ) = 0;

    /**
    * Delete a file based on the collection path.
	* @since S60 3.2.3
    * @param aPath collection path with selections to delete.
    * @param aObserver observer callback when operation is complete
    */
    virtual void DeleteL( CMPXCollectionPath& aPath,
                          MMPXCHelperObserver* aObserver ) = 0;

    /**
    * Move an item from one collection to another.
	* @since S60 3.2.3
    * @param aMedia media for the item to delete.
    * @param aNewCollection collection uid defined in collectionplugin.hrh.
    * @param aObserver observer for the event
    */
    virtual void MoveL( CMPXMedia*& aMedia,
                        TUid aNewCollection,
                        MMPXCHelperObserver* aObserver = NULL ) = 0;  //lint !e1735
    /**
    * Open the collection in embedded mode with a Media Object.
	* @since S60 3.2.3
    * @param aUid Host Process UID to identify this embedded instance.
    * @param aMedia Media Object to open.
    * @param aObserver observer to the open operation.
    * @param aPluginInfo additional argument to pass to the plugin resolver
    */
    virtual void OpenL( const TUid& aHostId, CMPXMedia& aMedia,
                        MMPXCHelperEmbeddedOpenObserver* aObserver,
                        TInt aPluginInfo = 0 ) = 0; //lint !e1735

    /**
    * Open the collection in embedded mode with a File Handle.
	* @since S60 3.2.3
    * @param aHostId Host Process UID to identify this embedded instance.
    * @param aFile File handle that we have to process.
    * @param aObserver observer to the open operation.
    * @param aPluginInfo additional argument to pass to the plugin resolver
    */
    virtual void OpenL( const TUid& aHostId, const TDesC& aFile,
                        MMPXCHelperEmbeddedOpenObserver* aObserver,
                        TInt aPluginInfo = 0 ) = 0; //lint !e1735

    /**
    * Queries about the playlist file extension of the currently selected playlist
    * plugin.
	* @since S60 3.2.3
    * @return File extension of the playlist type which includes the period
    *         ownership transferred
    */
    virtual HBufC* PlaylistFileExtensionLC() = 0;

    /**
    * Export the specified playlist to a file.
	* @since S60 3.2.3
    * @param aPlaylistId id of the playlist to be exported.
    * @param aDestinationDriveAndPath, specifies where the playlist file should
    *        be created. If the file already exists, it will be overwritten.    
    * @param aObserver observer for the callback. URI of the exported playlist
    *        will be returned as a HBufC* which client takes over the
    *        ownership.
    * @leave KErrArgument leave if the playlist object does not exist
    */
    virtual void ExportPlaylistL(TMPXItemId aPlaylistId,
                                 const TDesC& aDestinationDriveAndPath,
                                 MMPXCHelperObserver* aObserver) = 0;


    /**
    * Reorder a song in the playlist.
	* @since S60 3.2.3
    * @param aPlaylistId id of the playlist which contains the song to be reordered.
    * @param aSongId id of the song to be reordered.
    * @param aOriginalOrdinal the original ordinal of the song within the playlist.
    *        Ordinal starts from 0.
    * @param aNewOrdinal the new ordinal of the song within the playlist. Ordinal
    *        starts from 0.
    * @param aObs observer for the callback.
    * @leave KErrArgument leave if the playlist object does not exist
    */
    virtual void ReorderPlaylistL(const TMPXItemId& aPlaylistId,
                                  const TMPXItemId& aSongId,
                                  TUint aOriginalOrdinal,
                                  TUint aNewOrdinal,
                                  MMPXCHelperObserver* aObserver) = 0;

    /**
    * Gets the collection path to the main music menu.
    * Call OpenL() with this path to open music menu.
	* @since S60 3.2.3
    * @return CMPXCollectionPath* ownership transferred
    */
    virtual CMPXCollectionPath* MusicMenuPathL() = 0;

    /**
    * Gets the collection path to the podcast menu.
    * Call OpenL() with this path to open podcast menu.
	* @since S60 3.2.3
    * @return CMPXCollectionPath* ownership transferred
    */
    virtual CMPXCollectionPath* PodCastMenuPathL() = 0;

    /**
    * Gets the collection path to the main all songs view.
    * Call OpenL() with this path to open music all songs view.
	* @since S60 3.2.3
    * @return CMPXCollectionPath* ownership transferred
    */
    virtual CMPXCollectionPath* MusicAllSongsPathL() = 0;

    /**
    * Gets the collection path to the music playlist view.
    * Call OpenL() with this path to open music playlist menu.
	* @since S60 3.2.3
    * @return CMPXCollectionPath* ownership transferred
    */
    virtual CMPXCollectionPath* MusicPlaylistPathL() = 0;

    /**
    * Gets the collection path to the music playlist view
    * Call OpenL() with this path to open music playlist menu
    * @since S60 5.0
    * @param aPlaylistType type of auto playlist
    * @return CMPXCollectionPath*, ownership transferred
    */
    virtual CMPXCollectionPath* MusicPlaylistPathL( TMPXAutoPlaylistType aPlaylistType ) = 0;

    /**
    * Creates a default playlist path to open. Hardcoded to Music Collection / All songs.
	* @since S60 3.2.3
    * @return CollectionPath* ownership transferred
    */
    virtual CMPXCollectionPath* CreateDefaultPlaylistPathLC() = 0;

    /**
    * Cancels any async requests if any.
	* @since S60 3.2.3
    * @note Only DeleteL() is supported at the moment
    */
    virtual void Cancel() = 0;

    /**
    * Free this object.
	* @since S60 3.2.3
    */
    virtual void Close() = 0;

    /**
    * Test if the specified title exists in the specified category.
	* @since S60 3.2.3
    * @param aCategory specifies the category for testing.
    * @param aTitle specifies the title for testing.
    * @return ETrue if the specified title exists in the specified
    *         category; otherwise EFalse
    */
    virtual TBool TitleExistsL( TMPXGeneralCategory aCategory,
                                const TDesC& aTitle,			
                              	TMPXGeneralType aType = EMPXItem ) = 0;

    /**
    * Open the collection in embedded mode with a File Handle.
	* @since S60 3.2.3
    * @param aFile File handle that we have to process.
    * @param aObserver observer to the open operation
    */
    virtual void OpenL( RFile& aFile,
                        MMPXCHelperEmbeddedOpenObserver* aObserver ) = 0;

    /**
    * Returns the file name used for virtual playlist handling.
	* @since S60 3.2.3
    * @return The file name to be used to externalize collection path for the playlist
    *         that needs to be handled. Ownership transfered.
    */
    virtual HBufC* ExternalCollectionPathHandlingFileNameLC() = 0;
    
    /**
    * Add a media. This is exactly the same as AddL except
    * the songs in the media are added incrementally (in chunks).
    * This function is not pure virtual in case some clients did 
    * not implement this (binary compatibility).
    * @since S60 3.2.3
    * @param aMedia media to be added to the
    *        system. Media type and category
    *        are mandatory attributes. Depending
    *        on the operation additional attributes
    *        may be required.
    * @param aObserver observer to callback on completion 
    *        of the request.
    * @param aSize number of songs to add in each chunk
    */
    virtual void IncAddL( const CMPXMedia& /*aMedia*/,
                          MMPXCHelperObserver* /*aObserver*/,
                          const TInt /*aSize*/ ) {};
    };

#endif // MMPXCOLLECTIONUIHELPER_H
