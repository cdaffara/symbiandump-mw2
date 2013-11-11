/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Harvester Utility Implementation class
*
*/


#ifndef CMPXHARVESTERUTILITYIMP_H
#define CMPXHARVESTERUTILITYIMP_H

#include <mpxsession.h>
#include <mpxharvestercommon.h>
#include "mpxharvesterutility.h"

// Foward Declaration
class MMPXHarvesterUtilityObserver;

/**
 *  Harvester Utility, RSession owner
 *
 *  Session class to the harvester server
 *
 *  @lib mpxharvesterutility.lib
 *  @since S60 v3.0
 */
NONSHARABLE_CLASS( CMPXHarvesterUtilityImp ): public CActive,
                                           public MMPXHarvesterUtility
    {

public:

    /**
    * Two Phase constructor
    * @return CMPXHarvesterUtilityImp instance
    */
    static CMPXHarvesterUtilityImp* NewL();

    /**
    * destructor
    */
    virtual ~CMPXHarvesterUtilityImp();

protected:

    /*
    * From MMPXHarvesterUtility
    * Initiates a scan to refresh the database
    */
    void ScanL();

    /*
    * From MMPXHarvesterUtility
    * Cancels a scan to refresh the db
    */
    void CancelScanL();

    /*
    * From MMPXHarvesterUtility
    * Shutdown the server
    */
    void ShutdownL();

    /**
    * From MMPXHarvesterUtility
    * Add a single song
    * @param aFilePath path to the song to be added
    * @param aObs observer to the AddFileL() event
    */
    void AddFileL( const TDesC& aFilePath, MMPXHarvesterUtilityObserver* aObs );

    /**
    * From MMPXHarvesterUtility
    * Add a media object
    * @param aMedia, media object to add
    * @return, collection ID for this media property
    */
    TInt AddFileL( CMPXMedia*& aMedia );

    /**
    * From MMPXHarvesterUtility
    * Remove a single song
    * @param aFilePath, path to the item to be removed
    * @return, Collection ID for the item
    */
    TInt RemoveFileL( const TDesC& aFilePath );

    /**
    * From MMPXHarvesterUtility
    * Remove a list of songs
    * @param aFilesArray, list of songs to remove
    */
    void RemoveFilesL( const MDesCArray& aFilesArray );

    /**
    * From MMPXHarvesterUtility
    * Remove all songs from the harvester
    */
    void RemoveAllFilesL();

    /**
    * From MMPXHarvesterUtility
    * Remove all databases and re-create the db to handle corruption
    */
    void RecreateDatabasesL();

    /**
    * From MMPXHarvesterUtility
    * Remove a single song
    * @param aFilePath, path to the item to be removed
    * @param aObs observer to the delete operation
    */
    void DeleteFileL( const TDesC& aFilePath, MMPXHarvesterUtilityObserver* aObs  );

    /**
    * From MMPXHarvesterUtility
    * Remove a list of songs
    * @param aArray, list of songs to remove
    * @param aObs observer to the delete operation
    */
    void DeleteFilesL( const MDesCArray& aArray, MMPXHarvesterUtilityObserver* aObs );

    /**
    * From MMPXHarvesterUtility
    * Remove a list of songs
    * @param aArray, list of songs to remove
    */
    void DeleteFilesL( const MDesCArray& aArray );

    /**
    * From MMPXHarvesterUtility
    * Remove a list of songs
    * @param aArray, list of songs to remove
    * @param aEndTransaction ETrue to end current database transaction
    */
    void DeleteFilesL( const MDesCArray& aArray, TBool aEndTransaction );

    /**
     * From MMPXHarvesterUtility
     * Close the transaction.
     */
    void CloseTransactionL();

    /**
    * From MMPXHarvesterUtility
    * Update the collection id column in the harvester for a media
    * @param aMedia media object containing the item to update
    */
    void UpdateFileL( CMPXMedia*& aMedia );

    /**
    * From MMPXHarvesterUtility
    * Rename file for the given media. This method only supports
    * playlist renaming.
    * @param aMedia media to be renamed. this media must contain
    *        the new title of the playlist, the existing uri of
    *        the playlist
    * @return new URI of the renamed playlist media, ownership transferred
    */
    HBufC* RenameFileLC( const CMPXMedia& aMedia );

    /**
    * Rename file
    * @param aOldUri, existing URI
    * @param aNewUri, new URI
    * @param aCollection, collection Id of the collection the file belongs to
    */
    void RenameFileL( const TDesC& aOldUri,
                      const TDesC& aNewUri,
                      TInt aCollection );

    /*
    * From MMPXHarvesterUtility
    * Finds the collection ID based on a File name
    * @param aFile, file name to lookup
    * @return the collection ID
    */
    TInt FindCollectionIdL( const TDesC& aFile );

    /**
    * From MMPXHarvesterUtility
    * Export a Media property as a playlist
    * @param aProp CMPXMedia object to contain the "list" of items
    * @param aPath, path and filename
    * @param aPlaylistType, playlist type, default to M3U
    * @param aObs, observer for async callback
    */
    void ExportPlaylistL( const CMPXMedia& aProp,
                          const TDesC& aPath,
                          TInt aPlaylistType,
                          MMPXHarvesterUtilityObserver* aObs );

    /**
    * From MMPXHarvesterUtility
    * Import a playlist from a file
    * The user can ONLY execute 1 async op at a time!
    */
    void ImportPlaylistL( const TDesC& aFilename,
                          MMPXHarvesterUtilityObserver* aObs );

    /**
    * From MMPXHarvesterUtility
    * Imports a "file" in general
    * @param aFileName, file to import
    * @param aObs, observer for the import event
    */
    void ImportFileL( const TDesC& aFilename,
                      MMPXHarvesterUtilityObserver* aObs );

    /**
    * From MMPXHarvesterUtility
    * Query the required attributes for tracks for the specified playlist type
    * @param aPlaylistType playlist type for this query
    * @param aRequiredAttributes out parameter, an array of required attributes
    *        of the specified playlist type
    */
    void RequiredAttributesL( TInt aPlaylistType,
                              RArray<TMPXAttribute>& aRequiredAttributes );

    /**
    * From MMPXHarvesterUtility
    * Query the optional attributes for tracks for the specified playlist type
    * @param aPlaylistType playlist type for this query
    * @param aOptionalAttributes out parameter, an array of optional attributes
    *        of the specified playlist type
    */
    void OptionalAttributesL( TInt aPlaylistType,
                              RArray<TMPXAttribute>& aOptionalAttributes );


    /*
    * From MMPXHarvesterUtility
    * Query the file extension of the playlist of the specified type
    * @param aPlaylistType playlist type for this query
    * @return playlist file extension of the specified playlist type,
    *         this includes the period. Ownership transferred
    */
    HBufC* PlaylistFileExtensionLC( TInt aPlaylistType );

    /**
    * From MMPXHarvesterUtility
    * Determines whether the given file is a playlist.
    *
    * @param aUri URI of the media to be tested
    * @return ETrue if it's a playlist; otherwise EFalse.
    */
    TBool IsPlaylistL( const TDesC& aUri );

    /**
    * From MMPXHarvesterUtility
    * Sends a message to the harvester server to poll for any
    * system events. If there is a sytem event happening,
    * It will notify the rest of the MPX framework
    */
    void CheckForSystemEventsL();

    /**
    * From MMPXHarvesterUtility
    * Deletes the harvester utility and free the implementation
    */
    void Close();

    /**
    * Get a media object for the file
    * This method is asynchronous and will call back via the observer interface
    * @note S60 metadata utility is REALLY slow, so this has to be async
    * @param aFilePath, path to the song to be added
    * @param aObs observer to the add file event
    */
    void GetMediaForFileL( const TDesC& aFilePath, MMPXHarvesterUtilityObserver* aObs );

    /**
    * Get a collection ID for the file
    * @param aMedia media to add
    * @return collection ID for this media
    */
    TInt GetColUidForFileL( const TDesC& aFilePath );

protected:

    /**
    * From CActive
    */
    void RunL();

    /**
    * From CActive
    */
    void DoCancel();

    /**
    *  From CActive
    *  Handles a leave occurring in the request completion event handler RunL()
    *
    *  @param aError error code
    *  @return system error code
    */
    TInt RunError(TInt aError);

private:

    /**
    * Constructor
    */
    CMPXHarvesterUtilityImp();

    /**
    * 2nd phase constructor
    */
    void ConstructL();

    /**
    * Class to maintain the life time of async requests
    */
    NONSHARABLE_CLASS( CHvsUtilityTask ) : public CBase
        {
        public:
            CHvsUtilityTask( CBufBase* arg1=NULL, HBufC* arg2=NULL, HBufC* arg3=NULL,
                             TInt arg4=0, CMPXMedia* arg5=NULL );
            virtual ~CHvsUtilityTask();
            const TDesC8& Arg1();
            const TDesC&  Arg2();
            const TDesC&  Arg3();
            const TDesC8&  Arg4();
            const CMPXMedia& Arg5();

        private:
            CBufBase* iArg1;
            HBufC*    iArg2;
            HBufC*    iArg3;
            TPckgBuf<TInt> iArg4;

            TPtrC8    iPtr1;
            TPtrC     iPtr2;
            TPtrC     iPtr3;
            CMPXMedia* iArg5;
        };

private: // data
    RMPXSession  iSession; // Session
    CBufBase*    iBuffer;

    // Callback observer for async ops.
    //
    MMPXHarvesterUtilityObserver*  iObserver;
    TMPXHarvesterServerOp          iCurAsyncOp;
    CHvsUtilityTask*               iCurTask;
    HBufC* iFile;
    };


#endif // CMPXHarvesterUtilityImp_H
