/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  sync collection and harvester db contents
*
*/


#ifndef CMPXDBSYNCHRONIZER_H
#define CMPXDBSYNCHRONIZER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class RFs;
class MMPXDbSyncObserver;
class MMPXCollectionUtility;
class CMPXHarvesterDatabaseManager;
class CMPXHarvesterDatabaseTable;

/**
 *  CMPXDbSynchronizer
 *
 *  Sync collection and harvester databases
 *
 *  @lib mpxfilehandler
 *  @since S60 3.0
 */
NONSHARABLE_CLASS( CMPXDbSynchronizer ) : public CActive
    {
    enum ESyncState
        {
        ESyncStopped,
        ESyncMusic,
        ESyncPlaylist,
        ESyncPodcast
        };

public:    
    /**
    * Two-phased constructor
    * @param aObs synchronization observer
    * @param aDbMng reference to the database manager
    * @param aMusic reference to the music collection uid
    * @param aPodcast reference to the podcast collection uid
    * @param aFs file session
    */
    static CMPXDbSynchronizer* NewL(MMPXDbSyncObserver& aStateObs,
                                    CMPXHarvesterDatabaseManager& aDbMng,
                                    const TUid& aMusic,
                                    const TUid& aPodcast,
                                    RFs& aFs, 
                                    TBool aDisablePodcast );

    /**
    * Virtual destructor
    */
    virtual ~CMPXDbSynchronizer();

    /**
    * Check databases on these drives for sync issues, result within a callback to observer
    * @param aDrives the drives to be used
    * @param aColUtil collection utility reference
    */
    void Synchronize(RArray<TInt>& aDbIndexes, MMPXCollectionUtility* aColUtil);

protected: // From Base Class
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
    */
    TInt RunError(TInt aError);    

private: // New Functions
    /**
    * Sync a portion of data to avoid blocking thread for long
    * @return ETrue if there is more data to process
    *         EFalse if the sync process is complete
    */
    TBool DoSynchronizeStepL();
    
    /**
    * Handles what to do when synchronization is done
    * @param aErr error to return to the client
    */
    void DoSynchronizeComplete(TInt aErr);

    //Helper functions used inside DoSynchronizeStepL
    void DoMusicTableSyncL(CMPXHarvesterDatabaseTable& aTable);
    void DoPlaylistTableSyncL(CMPXHarvesterDatabaseTable& aTable);
    void DoPodcastTableSyncL(CMPXHarvesterDatabaseTable& aTable);
    //Misc helper functions for internal use
    inline void CompleteSelf();
    inline void GetTableCountL(TInt aDbId, TInt aTable);
    inline TInt CopyTableRecordsL(CMPXHarvesterDatabaseTable& aTable,
                                  TInt aColDbId, TInt aColTable);
    
private:
    /**
    * Private constructor
    * @param aObs synchronization observer
    * @param aDbMng reference to the database manager
    * @param aMusic reference to the music collection uid
    * @param aPodcast reference to the podcast collection uid
    * @param aFs file session
    */
    CMPXDbSynchronizer(MMPXDbSyncObserver& aObs,
                       CMPXHarvesterDatabaseManager& aDbMng,
                       const TUid& aMusic,
                       const TUid& aPodcast,
                       RFs& aFs,
                       TBool aDisablePodcast );

    /**
    * 2nd Phase constructor
    */
    void ConstructL();

private: // data
    MMPXDbSyncObserver&    iSyncObserver;
    CMPXHarvesterDatabaseManager& iDBManager; // Not owned
    MMPXCollectionUtility* iColUtil; //Not owned
    const TUid&  iMusicUid;
    const TUid&  iPodcastUid;
    RFs&  iFs; // Not owned

    // Databases on these drives to be synchronized
    RArray<TInt>           iDbDrives;

    ESyncState             iSyncState;
    TInt                   iCurDrive;
    TInt                   iDbRecTotalCount;
    TInt                   iCurDbRecCount;
    TInt                   iLastID;
    TBool                  iDisablePodcasting;
    };

#endif // CMPXDBSYNCHRONIZER_H
