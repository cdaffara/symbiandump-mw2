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
* Description:  File handler class to handle all file related requests
*
*/


#ifndef CMPXHARVESTERFILEHANDLERIMP_H
#define CMPXHARVESTERFILEHANDLERIMP_H

#include <f32file.h>
#include <badesca.h>
#include <apgcli.h>
#include <mpxattribute.h>
#include <DRMEventObserver.h>

#include "mpxfilescanstateobserver.h"
#include "mpxdbsyncobserver.h"
#include "mpxfileadditionobserver.h"
#include "mpxharvesterfilehandler.h"
#include "mpxmetadatascanobserver.h"
#include "mpxplaylistscanobserver.h"
#include "mpxmediatorobserver.h"
#include "mpxdiskspacewatcher.h"
#include "mpxdiskspacewatcherobserver.h"
#include "mpxfoldermonitor.h"
#include "mpxfoldermonitorobserver.h"
#include "mpxbrokenlinkcleanupobserver.h"

// FORWARD DECLARATIONS
class CMPXFolderScanner;
class CMPXDbSynchronizer;
class CMPXMetadataScanner;
class CMPXPlaylistScanner;
class CMPXBrokenLinkCleanup;
class CMPXDiskSpaceWatcher;
class CMPXFolderMonitor;
class CMPXHarvesterDatabaseManager;
class CMPXHarvesterDB;
class CMPXHarvesterDatabaseTable;
class CMPXHarvesterDbItem;
class CMPXCollectionType;
class MMPXCollectionUtility;
class MMPXCollection;
class CDRMNotifier;

/**
 *  File handler class to take care of all file related operations
 *
 *  @lib mpxfilehandler
 *  @since S60 3.0
 */
NONSHARABLE_CLASS( CMPXHarvesterFileHandlerImp ) : public CMPXHarvesterFileHandler,
                                                   public MMPXFileScanStateObserver,
                                                   public MMPXDbSyncObserver,
                                                   public MMPXFileAdditionObserver,
                                                   public MMPXPlaylistScanObserver,
                                                   public MMPXMetadataScanObserver,
                                                   public MMPXMediatorObserver,
                                                   public MMPXDiskSpaceObserver,
                                                   public MMPXFolderMonitorObserver,
                                                   public MMPXBrokenLinkObserver,
                                                   public MDRMEventObserver
    {
public:

    /**
    * Two-Phased Constructor
    * @param aFs file server session
    */
    static CMPXHarvesterFileHandlerImp* NewL( RFs& aFs );

    /**
    * Virtual destructor
    */
    virtual ~CMPXHarvesterFileHandlerImp();

protected: // From Base Class

    /**
    * From CMPXHarvesterFileHandler
    */
    void ScanL();

    /**
    * From CMPXHarvesterFileHandler
    */
    void CancelScan();

    /**
    * From CMPXHarvesterFileHandler
    */
    void HandleSystemEventL( TSystemEvent aEvent,
                             TInt aData );

    /**
    * From CMPXHarvesterFileHandler
    */
    CMPXMedia* AddFileL( const TDesC& aPath );

    /**
    * From CMPXHarvesterFileHandler
    */
    TInt AddFileL( CMPXMedia& aMediaProp );

    /**
    * From CMPXHarvesterFileHandler
    */
    TInt RemoveFileL( const TDesC& aPath, TBool aEndTransaction );

    /**
    * From CMPXHarvesterFileHandler
    */
    void RemoveFilesL( const MDesCArray& aFilePaths );

    /**
    * From CMPXHarvesterFileHandler
    */
    void RemoveAllFilesL();

    /**
    * From CMPXHarvesterFileHandler
    */
    void UpdateFileL( const TDesC& aFile, TInt aCollection );

    /**
    * From CMPXHarvesterFileHandler
    */
    void RenameFileL( const TDesC& aOldPath, const TDesC& aNewPath, TInt aCollection );

    /**
    * From CMPXHarvesterFileHandler
    */
    TInt FindCollectionIdL( const TDesC& aFile );

    /**
    * From CMPXHarvesterFileHandler
    */
    void RecreateDatabases();

    /**
    * From CMPXHarvesterFileHandler
    */
    void CloseTransactionL();

    /**
    * Get a media object for the file
    * @param aPath File to add
    * @return CMPXMedia object for the item pointed to by aPath
    *         ownership transferred
    */
    virtual CMPXMedia* GetMediaForFileL( const TDesC& aPath );

    /**
    * Get Collection Uid for the file
    * @param aMediaProp Media Property to add
    * @return TInt collection ID for the item added
    */
    virtual TInt GetColUidForFileL( const TDesC& aPath );

    /**
    * From MMPXFileScanStateObserver
    */
    void HandleScanStateCompleteL( TScanState aState, TInt aErr );

    /**
    * From MMPXDbSyncObserver
    */
    void HandleSynchronizationComplete( TInt aErr );

    /**
    * From MMPXFileAdditionObserver
    */
    void HandleFileAdditionL( const TDesC& aFileName,
                              TInt aColIndex,
                              TBool aPlaylist = EFalse  );   //lint !e1735

    /**
    * From MMPXFileAdditionObserver
    */
    void HandleOpenDriveL( TDriveNumber aDrive, const TDesC& aFolder );

    /**
    * From MMPXFIleAdditionObserver
    */
    void HandleDirectoryChangedL( const TDesC& aPath );

    /**
    * From MMPXFileAdditionObserver
    */
    TInt IsMediaFileL( const TDesC& aFile );

    /**
    * From MMPXFileAdditionObserver
    */
    TInt IsPlaylistFileL( const TDesC& aFile );

    /**
    * From MMPXFileAdditionObserver
    */
    TBool IsPathBlockedL( const TDesC& aPath );

    /**
    * From MMPXMetadataScanObserver
    */
    void AddFilesToCollectionL( CMPXMediaArray& aMediaArray );

    /**
    * From MMPXMetadataScanObserver
    */
    void UpdatesFilesInCollectionL( CMPXMediaArray& aMediaArray );

    /**
    * From MMPXMediatorObserver
    */
    void HandleMediatorPathUpdatedL( CMPXMedia*& aProperty,
                                     TUid  aOldPath );

    /**
    * From MMPXPlaylistScanObserver
    */
    void AddPlaylistToCollectionL( CMPXMediaArray& aMediaArray );

    /**
    * From MMPXPlaylistScanObserver
    */
    void UpdatePlaylistToCollectionL( CMPXMediaArray& aMediaArray );

    /**
    * From MMPXDiskSpaceObserver
    */
    void HandleLowDiskEvent( TInt aDrive );

    /**
    * From MMPXBrokenLinkObserver
    */
    void HandleBrokenLinkL( MDesCArray& aFileArray,
                            RArray<TInt>& aColId ,
                            RPointerArray<CMPXHarvesterDB>& aDb,
                            TInt aCount );
                                     
    /**
    * From MDRMEventObserver
    */
    void HandleEventL( MDRMEvent* aEvent );
    
private: // New Functions

    /**
    * Parses cenrep string (KHarvesterScanPathKey)
    * to get a list of scan paths
    */
    void ParseScanPathL();

    /**
	 * Read cenrep file for paths	
     * @param aScanPath fetched from cenrep dirs to scan
     * @param aBlockPath fetched from cenrep dirs not to scan 
     */
    void FetchPathsFromCenrepL( TDes16& aScanPath, TDes16& aBlockPath );
    
    /**
    * Parse cenrep string (KHarvesterContainerKey)
    * for a list of containers
    */
    void ParseContainersL();

    /**
    * Parses the auto scan cenrep key
    * for a list of folders to automatically scan
    */
    void ParseAutoScanL();
    
    /**
     * Refreshes scan drives so that non-existing 
     * drives are not scanned.
	 * Used add back drives that were take out because of dismount
     */
    void RefreshScanDrivesL();

    /***
    * Resets the current scan directory and frees memory
    */
    void Reset();

    /**
    * Handles a new item
    * @param aFileName new file to add
    */
    void HandleNewItemL( const TDesC& aFileName );

    /**
    * Handles broken items
    */
    void HandleBrokenItemsL();

    /**
    * Handles updated items
    * @param aFile file to update
    */
    void HandleUpdatedItemL( const TDesC& aFile );

    /**
    * Resolves the collection UID for a file
    * @param aFile file and path to a file for resolution
    * @return TInt collection ID on return
    */
    TInt CollectionForExtensionL( const TDesC& aFile );

    /**
    * Sets up the internal DB variables for a path
    * @param aPath path to open a view to the database
    */
    void OpenDBForPathL( const TDesC& aPath );

    /**
    * Final step for the refresh operation
    * @param aErr error code to complete with
    */
    void DoCompleteRefreshL( TInt aErr );

    /**
    * Perform a command on the collection
    * @param aCommandId Id of the command
    * @param aMediaAttribute TMPXAttriubte for the media payload
    * @param aCollectionId collection to perform the command on
    * @param aMedia the Media for the command
    * @param aSync Sync operation or not. (Default ETrue)
    */
    void DoCommandL( TInt aCommandId,
                     TUid aCollectionId,
                     const TMPXAttribute& aMediaAttribute,
                     CMPXMedia* aMedia = NULL,
                     TBool aSync = ETrue );

    /**
    * Remove an item to a collection
    * @param aMedia, media to add
    * @param aCollectionId, collection UID
    */
    void DoRemoveL( CMPXMedia* aMedia, TUid aCollectionId );

    /**
    * Verifies if harvester and collection databases are in sync by comparing the number of records
    * if records counts are different delete harvester db and repopulate it from collection
    * @return TBool ETrue if synchronization is started, EFalse otherwise
    */
    TBool CheckDbInSyncL();

    /**
     * Get a list of files with DRM flag set from the Harvester DB.
     * @return list of files
     */
    RPointerArray<CMPXHarvesterDbItem>* GetDrmFilesL();

public:

    /**
    * Delayed callback function
    */
    static TInt Callback( TAny* aPtr );

    /**
    * Perform the auto scan
    */
    TBool DoAutoScanL();

private:

    /**
    * Private Constructor
    */
    CMPXHarvesterFileHandlerImp( RFs& aFs );

    /**
    * 2nd Phased Constructor
    */
    void ConstructL();

    //helper function used inside CheckDbInSyncL
    inline TInt GetTrackCountL(TDriveNumber aDrive,TInt aColDbId, TInt aColTable);

private: // data
    // File scanning related
    CMPXFolderScanner*                iFolderScanner;
    CMPXDbSynchronizer*               iDbSynchronizer;
    RPointerArray<CMPXDiskSpaceWatcher> iDiskMonitors;
    RPointerArray<CMPXFolderMonitor>  iFolderMonitors;  // Multiple drives
    RPointerArray<CMPXCollectionType> iSupportedTypes;
    CDesCArray*                       iContainerTypes;
    RArray<TPath>                     iConfiguredDrivesToScan;
    RArray<TPath>                     iDrivesToScan;
    RArray<TPath>                     iFilteredDrivesToScan;
    RArray<TPath>                     iPathsToBlock;
    TBool                             iOutOfDisk;
    TBool                             iFilteredOutOfDisk;

    // Metadata related
    CMPXMetadataScanner*              iMetadataScanner;
    CMPXPlaylistScanner*              iPlaylistScanner;
    CMPXBrokenLinkCleanup*            iBrokenLink;

    // Session to the collection
    // Only valid during a scan
    //
    MMPXCollectionUtility*            iCollectionUtil;

    // Database related
    CMPXHarvesterDatabaseManager*    iDBManager; // DB manager

    CMPXHarvesterDB*                 iCurDB;     // Current db reference
    CMPXHarvesterDatabaseTable*      iCurTable;  // owned, must delete
    RPointerArray<CMPXHarvesterDbItem>*    iCurList; // owned, must delete

    // Misc
    RFs& iFs;
    RApaLsSession iAppArc;
    TInt iAddedCount;

    TBool                            iRefreshing; // Are we currently refreshing
    TBool                            iSynchronizing; // Are we currently synchronizing
    TInt                             iRefreshCount; // refresh reference count
    CPeriodic*                       iIdle;       // Delayed callback
    RArray<TPath>                    iAutoScanPaths; // list of auto scanning paths

    TUid                             iMusicCollectionId;
    TUid                             iPodcastCollectionId;

    TBool                            iDisablePodcasting;
    TInt                             iCurUSBEvent;
    
    // DRM
    CDRMNotifier*                    iDrmNotifier;
    };

#endif // CMPXHARVESTERFILEHANDLERIMP_H
