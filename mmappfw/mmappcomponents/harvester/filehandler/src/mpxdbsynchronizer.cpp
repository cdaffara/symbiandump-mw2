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
* Description:  Class for synchronization collection and harvester databases
*
*/

#include <e32base.h>
#include <mpxlog.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxcollectioncommanddefs.h>
#include <mpxcollectionutility.h>

#include "mpxcommand.h"
#include "mpxharvesterdbmanager.h"
#include "mpxharvesterdbtable.h"
#include "mpxdbsynchronizer.h"
#include "mpxdbsyncobserver.h"

#include <mpxcollectionplugin.hrh>
#include <centralrepository.h>            // to get podcasting cenrep key
// CONSTANTS
const TInt KRecStepCount = 100;

// ======== LOCAL FUNCTIONS ========
    
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CMPXDbSynchronizer::CMPXDbSynchronizer(MMPXDbSyncObserver& aObserver,
                                       CMPXHarvesterDatabaseManager& aDbMng,
                                       const TUid& aMusic,
                                       const TUid& aPodcast,
                                       RFs& aFs,
                                       TBool aDisablePodcast )
        : CActive(EPriorityNull), //using the same priority as folder scanner
        iSyncObserver(aObserver),
        iDBManager(aDbMng),
        iColUtil(NULL),
        iMusicUid(aMusic),
        iPodcastUid(aPodcast),
        iFs(aFs),
        iSyncState(ESyncStopped),
        iCurDrive(-1),
        iDbRecTotalCount(0),
        iCurDbRecCount(0),
        iLastID(0),
        iDisablePodcasting( aDisablePodcast )
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXDbSynchronizer::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// Two Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXDbSynchronizer* CMPXDbSynchronizer::NewL(MMPXDbSyncObserver& aObserver,
                                             CMPXHarvesterDatabaseManager& aDbMng,
                                             const TUid& aMusic,
                                             const TUid& aPodcast, 
                                             RFs& aFs,
                                             TBool aDisablePodcast )
    {
    CMPXDbSynchronizer* self = new(ELeave) CMPXDbSynchronizer(aObserver,aDbMng,aMusic,
                                                              aPodcast,aFs,aDisablePodcast);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXDbSynchronizer::~CMPXDbSynchronizer()
    {
    Cancel();
    iDbDrives.Close();
    }
    
// ---------------------------------------------------------------------------
// Scans a list of drives for files
// ---------------------------------------------------------------------------
//
void CMPXDbSynchronizer::Synchronize(RArray<TInt>& aDrives, MMPXCollectionUtility* aColUtil)
    {
    MPX_DEBUG1("---> CMPXDbSynchronizer::Synchronize");
    ASSERT(aColUtil != NULL); //internal call, should be always ok
    
    iColUtil = aColUtil;
    
    //check if synchronization is already active then ignore request
    if(iSyncState == ESyncStopped)
        {
        TInt dbCount(iDBManager.Count());
        TInt count(aDrives.Count());
        
        if(dbCount >= count && count != 0)
            {
            for(TInt i=0; i<count; ++i)
                {
                iDbDrives.Append(aDrives[i]);
                }

            iSyncState = ESyncMusic;
            
            // start synchronization cycle
            CompleteSelf();
            }
        else
            {
            iSyncObserver.HandleSynchronizationComplete(KErrArgument);
            }
        }
    else
        {
        iSyncObserver.HandleSynchronizationComplete(KErrInUse);
        }
    
    MPX_DEBUG1("<--- CMPXDbSynchronizer::Synchronize");
    }

// ---------------------------------------------------------------------------
// Sync next portion of data
// ---------------------------------------------------------------------------
//
TBool CMPXDbSynchronizer::DoSynchronizeStepL()
    {
    MPX_DEBUG1("---> CMPXDbSynchronizer::DoSynchronizeStepL");

    TBool ret(EFalse);

    if(iDbDrives.Count() != 0)
        {
        TBool clear(EFalse);
        //first time for a database on this drive
        if(iCurDrive == -1)
            {
            iCurDrive = iDbDrives[0];
            clear = ETrue;
            }

        CMPXHarvesterDB& db = iDBManager.GetDatabaseL((TDriveNumber)iCurDrive);
        CMPXHarvesterDatabaseTable* table = db.OpenAllFilesTableL();
        CleanupStack::PushL(table);

        if(clear)
            {
            db.RemoveAllFilesL();
            iDbRecTotalCount = 0;
            iCurDbRecCount = 0;
            iLastID = 0;
            }

        switch(iSyncState)
            {
            case ESyncMusic: //now getting music table content from curr drive's db
                {
                DoMusicTableSyncL(*table);
                break;
                }
            case ESyncPlaylist: //now getting playlist table content (music db)
                {
                DoPlaylistTableSyncL(*table);
                break;
                }
            case ESyncPodcast: //now getting podcast table content (podcast db)
                {
                DoPodcastTableSyncL(*table);
                break;
                }
            default:
                {
                User::Leave(KErrAbort);
                }
            }

        CleanupStack::PopAndDestroy(table);
        }
    else
        {
        ret = ETrue; //sync complete
        }
    
    MPX_DEBUG1("<--- CMPXDbSynchronizer::SetupNextDriveToScanL return");
    return ret;
    }
    
// ---------------------------------------------------------------------------
// Handle when synchronization is complete
// ---------------------------------------------------------------------------
//
void CMPXDbSynchronizer::DoSynchronizeComplete(TInt aErr)
    {
    MPX_DEBUG1("---> CMPXDbSynchronizer::DoSynchronizeComplete");
    
    iDbDrives.Reset(); // Reset drives
    iSyncState = ESyncStopped;

    iCurDrive = -1;
    iDbRecTotalCount = 0;
    iCurDbRecCount = 0;
    
    // Notify observer
    iSyncObserver.HandleSynchronizationComplete(aErr);
    
    MPX_DEBUG1("<--- CMPXDbSynchronizer::DoSynchronizeComplete");
    }
     
// ---------------------------------------------------------------------------
// From CActive
// ---------------------------------------------------------------------------
//
void CMPXDbSynchronizer::RunL()
    {
    MPX_DEBUG1("---> CMPXDbSynchronizer::RunL");
    
    TBool done(EFalse);
    done = DoSynchronizeStepL();
     
    if(done)
        {
        DoSynchronizeComplete(KErrNone);
        }
    else // if( !done )
        {
        MPX_DEBUG1("CMPXDbSynchronizer::RunL -- Schedule next run");
        CompleteSelf();
        }
    MPX_DEBUG1("<--- CMPXDbSynchronizer::RunL");
    }

// ---------------------------------------------------------------------------
// From CActive
// ---------------------------------------------------------------------------
//
void CMPXDbSynchronizer::DoCancel()
    {
    ASSERT(iSyncState != ESyncStopped);
    DoSynchronizeComplete(KErrCancel);
    }
    
// ----------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL()
// ----------------------------------------------------------------------------
//
TInt CMPXDbSynchronizer::RunError(TInt aError)
    {
    MPX_DEBUG2("CMPXDbSynchronizer::RunError(%d)", aError );
    
    DoSynchronizeComplete(aError);
    
    return KErrNone;
    }

//Helper functions implementation
void CMPXDbSynchronizer::DoMusicTableSyncL(CMPXHarvesterDatabaseTable& aTable)
    {
    if(iDbRecTotalCount == 0)
        {
        //get total number of records from music db for music table
        GetTableCountL(iMusicUid.iUid,EMPXCollectionCountTrack);
        if (iDbRecTotalCount == 0)
            {
            iSyncState = ESyncPlaylist;
            }
        }
    else
        {
        //get number of records from music table
        TInt count = CopyTableRecordsL(aTable,iMusicUid.iUid,EMPXCollectionURITrack);
        
        //continue with music or switch to playlist
        if(iCurDbRecCount >= iDbRecTotalCount || count == 0)
            {
            iDbRecTotalCount = 0;
            iCurDbRecCount = 0;
            iLastID = 0;
            iSyncState = ESyncPlaylist;
            }
        }
    }

void CMPXDbSynchronizer::DoPlaylistTableSyncL(CMPXHarvesterDatabaseTable& aTable)
    {
    if(iDbRecTotalCount == 0)
        {
        //get total number of records from music db for playlist table
        GetTableCountL(iMusicUid.iUid,EMPXCollectionCountPlaylist);
        if (iDbRecTotalCount == 0)
            {
            iSyncState = ESyncPodcast;
            }
        }
    else
        {
        //get number of records from playlist table
        TInt count = CopyTableRecordsL(aTable,iMusicUid.iUid,EMPXCollectionURIPlaylist);

        //continue with music or switch to playlist
        if(iCurDbRecCount >= iDbRecTotalCount || count == 0)
            {
            iDbRecTotalCount = 0;
            iCurDbRecCount = 0;
            iLastID = 0;
            iSyncState = ESyncPodcast;
            }
        }
    }

void CMPXDbSynchronizer::DoPodcastTableSyncL(CMPXHarvesterDatabaseTable& aTable)
    {
    if( iDisablePodcasting )
        {
        iDbDrives.Remove(0);
        iCurDrive = -1;
        iSyncState = ESyncMusic;
        }
    else
        {
        if(iDbRecTotalCount == 0)
            {
            //get total number of records from podcast db for podcast table
            GetTableCountL(iPodcastUid.iUid,EMPXCollectionCountTrack);
            if (iDbRecTotalCount == 0)
                {
                iDbDrives.Remove(0);
                iCurDrive = -1;
                iSyncState = ESyncMusic;
                }
            }
        else
            {
            //get number of records from podcast table
            TInt count = CopyTableRecordsL(aTable,iPodcastUid.iUid,EMPXCollectionURITrack);
        
            //continue with music or switch to playlist
            if(iCurDbRecCount >= iDbRecTotalCount || count == 0)
                {
                iDbDrives.Remove(0);
                iCurDrive = -1;
                iSyncState = ESyncMusic;
                }
            }
        }
    }

void CMPXDbSynchronizer::CompleteSelf()
    {
    iStatus = KRequestPending;
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );        
    }

void CMPXDbSynchronizer::GetTableCountL(TInt aDbId, TInt aTable)
    {
    //get total number of records from given Db for a given table
    CMPXCommand* cmdCount = CMPXMedia::NewL();
    CleanupStack::PushL(cmdCount);
    cmdCount->SetTObjectValueL<TMPXCommandId>(KMPXCommandGeneralId, KMPXCommandCollectionGetCount);
    cmdCount->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmdCount->SetTObjectValueL<TInt>(KMPXCommandGeneralCollectionId, aDbId);
    cmdCount->SetTObjectValueL<TInt>(KMPXCommandCollectionCountTable, aTable);
    cmdCount->SetTObjectValueL<TInt>(KMPXCommandCollectionCountDrive, iCurDrive);
    
    iColUtil->Collection().CommandL(*cmdCount);

    // returned command should contain count
    if (!cmdCount->IsSupported(KMPXCommandCollectionCountValue))
        {
        User::Leave(KErrAbort);
        }

    iDbRecTotalCount = cmdCount->ValueTObjectL<TInt>(KMPXCommandCollectionCountValue);
    
    CleanupStack::PopAndDestroy(cmdCount);
    }

TInt CMPXDbSynchronizer::CopyTableRecordsL(CMPXHarvesterDatabaseTable& aTable,
                                           TInt aColDbId, TInt aColTable)
    {
    //get number of records from given db and table
    CMPXCommand* cmdUri = CMPXMedia::NewL();
    CleanupStack::PushL(cmdUri);
    cmdUri->SetTObjectValueL<TMPXCommandId>(KMPXCommandGeneralId, KMPXCommandCollectionGetURIs);
    cmdUri->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmdUri->SetTObjectValueL<TInt>(KMPXCommandGeneralCollectionId, aColDbId);
    cmdUri->SetTObjectValueL<TInt>(KMPXCommandCollectionURIDrive, iCurDrive);
    cmdUri->SetTObjectValueL<TInt>(KMPXCommandCollectionURITable, aColTable);
    cmdUri->SetTObjectValueL<TInt>(KMPXCommandCollectionURIRecords, KRecStepCount); //number of records
    cmdUri->SetTObjectValueL<TInt>(KMPXCommandCollectionURIFromID, iLastID); //from last record

    // send sync retrieve count command
    iColUtil->Collection().CommandL(*cmdUri);

    // return command should contain Uri list and last item ID
    if (!cmdUri->IsSupported(KMPXCommandCollectionURIList) && 
        !cmdUri->IsSupported(KMPXCommandCollectionURILastID) )
        {
        User::Leave(KErrAbort);
        }

    //populate harvester db using received values;
    // retrieve the list of URIs
    CDesCArray* files = cmdUri->ValueNoNewLCObjectL<CDesCArray>(KMPXCommandCollectionURIList);
    CleanupStack::PushL(files);

    TInt uriCount = files->Count();
    for(TInt j=0; j<uriCount; ++j)
        {
        TTime lastModTime(0);
        TPtrC file((*files)[j]);
        //we need to add all files to harvester db to be able to remove non-existing later
        iFs.Modified(file, lastModTime);
        TParsePtrC parse(file);
        //if we leave here we it's ok to keep in harvester db what we managed to add
        aTable.AddItemL(parse.DriveAndPath(), parse.NameAndExt(),
                        lastModTime, aColDbId, 0); //drm is ok to be 0 here
        }

    iCurDbRecCount += uriCount;
    CleanupStack::PopAndDestroy(files);
    iLastID = cmdUri->ValueTObjectL<TInt>(KMPXCommandCollectionURILastID);
    CleanupStack::PopAndDestroy(cmdUri);
    
    return uriCount;
    }
