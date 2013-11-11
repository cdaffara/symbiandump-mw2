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
* Description:  Harvester utility library
*
*/


#ifndef MMPXHARVESTERUTILITY_H
#define MMPXHARVESTERUTILITY_H

#include <bamdesca.h>

// FOWARD DECLARATION
class CMPXMedia;
class TMPXAttribute;
class MMPXHarvesterUtilityObserver;

/**
 *  Harvester Utility
 *
 *  Utility class to the harvester server
 *
 *  @lib mpxharvesterutility.lib
 */
class MMPXHarvesterUtility
    {

public: // New Functions

    /**
     *  Initiates a scan for new files on all drives.
     *  All files are added to the collection.
     *  Scan results are broadcasted through the collection framework.
     *
     *  @since S60 3.0
     */
    virtual void ScanL() = 0;

    /**
     *  Cancels a scan to refresh the db.
     *
     *  @since S60 3.0
     */
    virtual void CancelScanL() = 0;

    /**
     * Shutdown the server.
     *
     * @since S60 3.0
     * @note test function only!! shuts down the harvester server immediately
     */
    virtual void ShutdownL() = 0;

    /**
     * Add a single song.
     * This method is asynchronous and will call back via the observer interface.
     *
     * @since S60 3.0
     * @note S60 metadata utility is REALLY slow, so this has to be async
     * @param aFilePath path to the song to be added
     * @param aObs observer to the add file event
     */
    virtual void AddFileL( const TDesC& aFilePath, MMPXHarvesterUtilityObserver* aObs ) = 0;

    /**
     * Add a media property.
     *
     * @since S60 3.0
     * @param aMedia media to add
     * @return collection ID for this media
     */
    virtual TInt AddFileL( CMPXMedia*& aMedia ) = 0;

    /**
     * Remove a single song.
     *
     * @since S60 3.0
     * @param aFilePath path to the song to be removed
     * @return collection ID for the item
     */
    virtual TInt RemoveFileL( const TDesC& aFilePath ) = 0;

    /**
     * Remove a list of songs.
     *
     * @since S60 3.0
     * @param aArray list of songs to remove
     */
    virtual void RemoveFilesL( const MDesCArray& aFilesArray ) = 0;

    /**
     * Remove all songs from the harvester.
     *
     * @since S60 3.0
     */
    virtual void RemoveAllFilesL() = 0;

    /**
     * Recreate all databases (deletes old).
     * Used to handle db corruption by deleting and re-starting.
     *
     * @since S60 3.0
     */
    virtual void RecreateDatabasesL() = 0;

    /**
     * Remove a single song.
     * This Method is asynchronous and will call back to observer interface.
     *
     * @since S60 3.0
     * @param aFilePath path to the song to be removed
     * @param aObs observer to the delete event
     */
    virtual void DeleteFileL( const TDesC& aFilePath, MMPXHarvesterUtilityObserver* aObs ) = 0;

    /**
     * Remove a list of songs.
     * This method is asynchronous and will call back to observer interface.
     *
     * @since S60 3.0
     * @param aArray list of songs to remove
     * @param aObs observer to the delete event
     */
    virtual void DeleteFilesL( const MDesCArray& aArray, MMPXHarvesterUtilityObserver* aObs ) = 0;

    /**
     * Remove a list of songs.
     * This method is asynchronous and will call back to observer interface.
     *
     * @since S60 3.0
     * @param aArray list of songs to remove
     */
    virtual void DeleteFilesL( const MDesCArray& aArray ) = 0;

    /**
     * Update the collection id column in the harvester for a media.
     *
     * @since S60 3.0
     * @param aMedia media object containing the item to updates
     */
    virtual void UpdateFileL( CMPXMedia*& aProperty ) = 0;

    /**
     * Rename file for the given media.
     *
     * @since S60 3.0
     * @param aMedia media to be renamed
     * @return new URI of the renamed file
     */
    virtual HBufC* RenameFileLC( const CMPXMedia& aMedia ) = 0;

    /**
     * Rename file.
     *
     * @since S60 3.0
     * @param aOldUri existing URI
     * @param aNewUri new URI
     * @param aCollection collection Id of the collection the file belongs to
     */
    virtual void RenameFileL( const TDesC& aOldUri,
                              const TDesC& aNewUri,
                              TInt aCollection ) = 0;

    /**
     * Finds the collection ID based on a File name.
     *
     * @since S60 3.0
     * @param aFile file name to lookup
     * @return the collection ID
     */
    virtual TInt FindCollectionIdL( const TDesC& aFile ) = 0;

    /**
     * Export a Media property as a playlist (Asynchronous).
     * Note that this utility does not have a task queue,
     * The user can ONLY execute 1 async op at a time!
     * This method is asynchronous and will call back via the observer interface.
     *
     * @since S60 3.0
     * @param aProp CMPXMedia object to contain the "list" of items
     * @param aPath path and filename
     * @param aPlaylistType playlist type, default to M3U
     * @param aObs observer for async callback
     */
    virtual void ExportPlaylistL( const CMPXMedia& aProp,
                                  const TDesC& aPath,
                                  TInt aPlaylistType,
                                  MMPXHarvesterUtilityObserver* aObs ) = 0;

    /**
     * Import a playlist from a file (Asynchronous).
     * This method is asynchronous and will call back via the observer interface.
     *
     * @since S60 3.0
     */
    virtual void ImportPlaylistL( const TDesC& aFilename,
                                  MMPXHarvesterUtilityObserver* aObs ) = 0;

    /**
     * Imports a "file" in general (Async).
     * File is not added to any databases.
     *
     * @since S60 3.0
     * @param aFileName file to import
     * @param aObs observer for the import event
     */
    virtual void ImportFileL( const TDesC& aFilename,
                              MMPXHarvesterUtilityObserver* aObs ) = 0;

    /**
     * Query the required attributes for tracks for the specified playlist type.
     *
     * @since S60 3.0
     * @param aPlaylistType playlist type for this query
     * @param aRequiredAttributes out parameter, an array of required attributes
     *        of the specified playlist type
     */
    virtual void RequiredAttributesL( TInt aPlaylistType,
                              RArray<TMPXAttribute>& aRequiredAttributes ) = 0;

    /**
     * Query the optional attributes for tracks for the specified playlist type.
     *
     * @since S60 3.0
     * @param aPlaylistType playlist type for this query
     * @param aOptionalAttributes out parameter, an array of optional attributes
     *        of the specified playlist type
     */
    virtual void OptionalAttributesL( TInt aPlaylistType,
                              RArray<TMPXAttribute>& aOptionalAttributes ) = 0;

    /*
     * Query the file extension of playlist of the specified type.
     *
     * @since S60 3.0
     * @param aPlaylistType playlist type for this query
     * @return playlist file extension of the specified playlist type,
     *         this includes the period. ownership transferred
     */
    virtual HBufC* PlaylistFileExtensionLC( TInt aPlaylistType ) = 0;

    /**
     * Determines whether the given file is a playlist from available
     * playlist plugins currently in the system. The existence of the
     * file is not part of this validation process.
     *
     * An unsupported playlist file is tested as EFalse. But after the
     * appropriate playlist plugin has been installed, the client can
     * retest it and ETrue will be returned.
     *
     * @since S60 3.0
     * @param aUri URI of the media to be tested
     * @return ETrue if it's a playlist; otherwise EFalse.
     */
    virtual TBool IsPlaylistL( const TDesC& aUri ) = 0;

    /**
     * Sends a message to the harvester server to poll for any
     * system events. If there is a sytem event happening,
     * It will notify the rest of the MPX framework.
     * This is used for process start up to query for any on-going events.
     *
     * @since S60 3.0
     */
    virtual void CheckForSystemEventsL() = 0;

    /**
     * Closes the utility and deletes the object.
     *
     * @since S60 3.0
     */
    virtual void Close() = 0;

    /**
     * Get a media object for the file.
     * This method is asynchronous and will call back via the observer interface.
     *
     * @since S60 3.0
     * @note S60 metadata utility is REALLY slow, so this has to be async
     * @param aFilePath path to the song to be added
     * @param aObs observer to the add file event
     */
    virtual void GetMediaForFileL( const TDesC& aFilePath,
                                   MMPXHarvesterUtilityObserver* aObs ) = 0;

    /**
     * Get a collection ID for the file.
     *
     * @since S60 3.0
     * @param aMedia media to add
     * @return collection ID for this media
     */
    virtual TInt GetColUidForFileL( const TDesC& aFilePath ) = 0;

    /**
     * Remove a list of songs.
     * This method is asynchronous and will call back to observer interface.
     * Method is not pure virtual for backward compatibility.
     * 
     * @since S60 3.2.3
     * @param aArray list of songs to remove
     * @param aEndTransaction ETrue to end current database transaction 
     */
    virtual void DeleteFilesL( const MDesCArray& /*aArray*/, TBool /*aEndTransaction*/ ){};

    /**
     * Close the transaction.
     * This method is synchronous.
     * Method is not pure virtual for backward compatibility.
     * 
     * @since S60 3.2.3
     */
    virtual void CloseTransactionL(){};

    };

/**
 *  Utility Factory class to create the utility
 *
 *  @lib harvester utility
 */
class CMPXHarvesterFactory : CBase
    {
public:
    /**
     * Factory function to create the utility
     *
     * @since S60 3.0
     */
    IMPORT_C static MMPXHarvesterUtility* NewL();
    };

#endif // MMPXHARVESTERUTILITY_H
