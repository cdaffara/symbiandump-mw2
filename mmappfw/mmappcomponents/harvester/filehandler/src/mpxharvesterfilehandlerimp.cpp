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
* Description:  Handles all file related activities
*  Version     : %version: da1mmcf#72.1.14.2.4.1.4.1.2.5.4 % << Don't touch! Updated by Synergy at check-out.
*
*/


#include <e32base.h>
#include <f32file.h>
#include <centralrepository.h>
#include <pathinfo.h>
#include <driveinfo.h>

#include <mpxlog.h>
#include <mpxharvestercommon.h>
#include <mpxmedia.h>
#include <mpxmediaarray.h>
#include <mpxcollectiontype.h>
#include <mpxcollectionutility.h>
#include <mpxcollectionpath.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediacontainerdefs.h>

#include <mpxdrmmediautility.h>
#include <mpxmediadrmdefs.h>
#include <mpxcollectionplugin.hrh>
#include <mpxcollectionmediator.h>

#include <mpxcommandgeneraldefs.h>
#include <mpxcollectioncommanddefs.h>
#include <DRMNotifier.h>
#include <DRMEventAddRemove.h>
#include <Oma2Agent.h>
#include <caf/caferr.h>
#include <caf/content.h>
#include <caf/data.h>
#include <mtpprivatepskeys.h>

#include "mpxharvesterfilehandlerimp.h"
#include "mpxfolderscanner.h"
#include "mpxdbsynchronizer.h"
#include "mpxfoldermonitor.h"
#include "mpxmetadatascanner.h"
#include "mpxplaylistscanner.h"
#include "mpxharvesterdbmanager.h"
#include "mpxharvesterdbtable.h"
#include "mpxharvesterdbitem.h"
#include "mpxfhcommon.h"
#include "mpxbrokenlinkcleanup.h"

// ============ CONSTANTS ==========
_LIT( KDefaultScanPath, "C:\\DATA\\|E:\\|F:\\" );
_LIT( KDefaultBlockPath, "\\SYS\\|\\PRIVATE\\|\\SYSTEM\\|\\CITIES\\|\\SOUNDS\\SIMPLE\\|\\GAMES\\|\\RESOURCE\\|\\IMAGES\\"); 
_LIT( KDefaultContainers, ".odf|.dcf|.asf|.m4a|.mp4" );
_LIT( KDefaultAutoScanFolder, "C:\\data\\sounds\\digital\\|E:\\sounds\\digital\\");
const TUid KCRUIDHarvesterFeatures  = { 0x101FFCD2 };
const TUid KMusicPlayerUid = {0x102072C3};
const TInt KHarvesterScanPathKey = 1;
const TInt KHarvesterBlockPathKey = 2;
const TInt KHarvesterContainerKey = 3;
const TInt KAutoScanDirectoryKey = 4;
const TInt KDisablePodcasting = 5;
const TInt KAutoScanDelay = 10000000;  // 10 second delay for rights to arrive
const TInt KAutoScanAfter = 10000000;

// ==========LOCAL FUNCTIONS ========
static void CleanupArray( TAny* item )
    {
    ((RPointerArray<CMPXHarvesterDbItem>*) item )->ResetAndDestroy();
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ Constructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterFileHandlerImp::CMPXHarvesterFileHandlerImp( RFs& aFs ) :
        iFs( aFs ),
        iSynchronizing(EFalse),
        iDisablePodcasting(EFalse)
    {
    }


// ---------------------------------------------------------------------------
// 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::ConstructL()
    {
    // Folder monitoring related,
    //
    User::LeaveIfError( iAppArc.Connect() );
    iFolderScanner = CMPXFolderScanner::NewL( *this, *this, iFs );
    iMetadataScanner = CMPXMetadataScanner::NewL( iFs,
                                                  iAppArc,
                                                  iSupportedTypes,
                                                  *this, *this );
    iPlaylistScanner = CMPXPlaylistScanner::NewL( *this,
                                                  *this,
                                                  iSupportedTypes );

    iBrokenLink = CMPXBrokenLinkCleanup::NewL( *this, *this );
    
    // Database related
    //
    iDBManager = CMPXHarvesterDatabaseManager::NewL( iFs );
    
    // List of watchers for different drives
    //
    TDriveList driveList;
    TInt driveCount(0);
    User::LeaveIfError( DriveInfo::GetUserVisibleDrives(
           iFs, driveList, driveCount ) );

    for( TInt driveNum = EDriveA; driveNum <= EDriveZ; driveNum++ )
        {
        if ( driveList[driveNum] && (!iDBManager->IsRemoteDrive(static_cast<TDriveNumber>(driveNum))))
            {
            CMPXDiskSpaceWatcher* dw = CMPXDiskSpaceWatcher::NewL(
                    iFs, static_cast<TDriveNumber>(driveNum), *this );
            CleanupStack::PushL( dw );
            iDiskMonitors.AppendL( dw );
            CleanupStack::Pop( dw );
            }
        }

    TInt openerr = iDBManager->OpenAllDatabasesL();
    
    // Temporary collection utility
    //
    MMPXCollectionUtility* colUtil =
                           MMPXCollectionUtility::NewL( NULL, KMcModeDefault );
    CleanupStack::PushL( colUtil );

    // Get the collection UIDs
    RArray<TUid> ary;
    CleanupClosePushL( ary );
    ary.AppendL( TUid::Uid(EMPXCollectionPluginMusic) );
    iMusicCollectionId = colUtil->CollectionIDL( ary.Array() );
    ary.Reset();
    ary.AppendL( TUid::Uid(EMPXCollectionPluginPodCast) );
    iPodcastCollectionId = colUtil->CollectionIDL( ary.Array() );
    CleanupStack::PopAndDestroy( &ary );

    // If harvester db was corrupted, mark podcast and music db as corrupt
    if( openerr == KErrCorrupt )
        {
        colUtil->Collection().CommandL(EMcCmdDbCorrupted, iMusicCollectionId.iUid );
        colUtil->Collection().CommandL(EMcCmdDbCorrupted, iPodcastCollectionId.iUid );
        }

    // Get the list of supported types from the collection
    //
    colUtil->Collection().GetSupportedTypesL( iSupportedTypes );
    CleanupStack::Pop( colUtil );
    colUtil->Close();

    // Get the scan drives from cenrep.
    //
    ParseScanPathL();
    RefreshScanDrivesL();

    // Get the list of container types
    iContainerTypes = new(ELeave) CDesCArrayFlat(2);  // granularity
    ParseContainersL();

    // Get the list of automatic scanned folders
    ParseAutoScanL();
    iIdle = CPeriodic::NewL( CActive::EPriorityLow );

    // Get the podcasting enabled / disabled flag
    //
    CRepository* cenrep(NULL);
    TRAPD( err, cenrep = CRepository::NewL( KCRUIDHarvesterFeatures ) );
    if( err == KErrNone )
        {
        cenrep->Get( KDisablePodcasting, iDisablePodcasting );
        delete cenrep;
        }
    else
        {
        iDisablePodcasting = EFalse;
        }

    // Create the database synchronizer
    iDbSynchronizer = CMPXDbSynchronizer::NewL(*this,*iDBManager,iMusicCollectionId,
                                               iPodcastCollectionId,iFs, iDisablePodcasting);

    // Create DRM Notifier and register for AddRemove event
    iDrmNotifier = CDRMNotifier::NewL();
    iDrmNotifier->RegisterEventObserverL( *this, KEventAddRemove );
    iCurUSBEvent = KErrNotFound;
    }

// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterFileHandlerImp* CMPXHarvesterFileHandlerImp::NewL( RFs& aFs )
    {
    CMPXHarvesterFileHandlerImp* self =
                                  new( ELeave ) CMPXHarvesterFileHandlerImp(aFs);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterFileHandlerImp::~CMPXHarvesterFileHandlerImp()
    {
    MPX_DEBUG1("~CMPXHarvesterFileHandlerImp <---");
    delete iDbSynchronizer;
    delete iFolderScanner;
    delete iMetadataScanner;
    delete iPlaylistScanner;
    delete iBrokenLink;

    iDiskMonitors.ResetAndDestroy();
    iDiskMonitors.Close();

    iFolderMonitors.ResetAndDestroy();
    iFolderMonitors.Close();


    iSupportedTypes.ResetAndDestroy();
    iSupportedTypes.Close();

    if( iContainerTypes )
        {
        iContainerTypes->Reset();
        }
    delete iContainerTypes;

    iFilteredDrivesToScan.Close();
    iDrivesToScan.Close();
    iConfiguredDrivesToScan.Close();
    iPathsToBlock.Close();

    // Cleans up the scanning tables and arrays
    Reset();

    delete iDBManager;

    if( iCollectionUtil )
        {
        iCollectionUtil->Close();
        }

    iAppArc.Close();

    iAutoScanPaths.Reset();
    iAutoScanPaths.Close();
    delete iIdle;
    
    if(iDrmNotifier)
        {
        TRAP_IGNORE( iDrmNotifier->UnRegisterEventObserverL( *this, KEventAddRemove ) );
        delete iDrmNotifier;
        }
    MPX_DEBUG1("~CMPXHarvesterFileHandlerImp --->");
    }

// ---------------------------------------------------------------------------
// Scans all drives in the list of interested drives
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::ScanL()
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::ScanL <---");

    // Reset all previous states
    CancelScan();
    Reset();

    // Construct the collection utility
    if( iCollectionUtil )
        {
        iCollectionUtil->Close();
        iCollectionUtil = NULL;
        }
    iCollectionUtil = MMPXCollectionUtility::NewL( NULL, KMusicPlayerUid );
    
    // cenrep key need to be checked whether MTP is connected
    // to prevent refresh
    TInt mtpStatus = EMtpPSStatusUninitialized;
    RProperty::Get( KMtpPSUid, KMtpPSStatus, mtpStatus);
        
    MPX_DEBUG2("CMPXCollectionViewHgImp::ConstructL, mtpstatus = %d", mtpStatus);

    if (mtpStatus != EMtpPSStatusUninitialized)
        {
        MPX_DEBUG1("MTP is active, Leave with KErrLocked");
        // need to call back even if it leaves here
        iCollectionUtil->Collection().NotifyL( EMcMsgRefreshEnd, KErrLocked );
        //User::Leave(KErrLocked);
        return;
        }
   
    iCollectionUtil->Collection().NotifyL( EMcMsgRefreshStart, KErrNone );

    // Begin transaction on databases
    iDBManager->BeginL();
    
    //Remove out of disk space drives from scanned drives list
    iFilteredDrivesToScan.Reset();
    iFilteredOutOfDisk = EFalse;
    CopyArrayL(iDrivesToScan.Array(),iFilteredDrivesToScan);

    iOutOfDisk = EFalse;
    TInt driveCount (iDiskMonitors.Count());
    TBool outOfDisk(EFalse);
    for( TInt driveIndex = 0; driveIndex < driveCount; ++driveIndex )
        {
        //Check if the current drive is low on disk
        outOfDisk = iDiskMonitors[driveIndex]->IsLowOnDisk();
        iDiskMonitors[driveIndex]->StartL();

        if(outOfDisk)
            {
            TInt count( iFilteredDrivesToScan.Count() );
            TInt index(0);
            TInt currentDriveNumber = iDiskMonitors[driveIndex]->CurrentDrive();
            while(index < count)
                {
                // Remove current drive from the scanned drives list
                TParse fileNameParser;
                User::LeaveIfError(fileNameParser.Set(iFilteredDrivesToScan[index],NULL,NULL));
                TFileName driveName(fileNameParser.Drive());
                TInt driveNumber = TDriveUnit( driveName );
                if (currentDriveNumber == driveNumber)
                    {
                    iFilteredDrivesToScan.Remove(index);
                    iFilteredOutOfDisk = ETrue;
                    count--;
                    }
                else
                    {
                    index++;
                    }
                }
            iDBManager->CloseDatabase(static_cast<TDriveNumber>(currentDriveNumber));
            }
        }

    iSynchronizing = CheckDbInSyncL();

    if(!iSynchronizing)
        {
        // Start the scanning process
        iRefreshCount++;
        iFolderScanner->ScanL( iFilteredDrivesToScan );
        iMetadataScanner->Reset();
        iBrokenLink->Reset();
        iPlaylistScanner->Reset();
        iIdle->Cancel();

        // Keep a count of how many we added to synchronize the number
        iAddedCount = 0;
        iRefreshing = ETrue;
        }

    if (iFilteredDrivesToScan.Count() == 0 )
        {
        iOutOfDisk = ETrue;
        MPX_DEBUG1("CMPXHarvesterFileHandlerImp::ScanL -- cancelling scan");
        // Cancel the scan in this case, will goto HandleScanStateCompleteL()
        CancelScan();
        }
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::ScanL --->");
    }

// ---------------------------------------------------------------------------
// Cancels Scanning
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::CancelScan()
    {
    iDbSynchronizer->Cancel();
    iFolderScanner->Cancel();
    iBrokenLink->Cancel();
    iMetadataScanner->Stop();
    iPlaylistScanner->Cancel();
    }

// ---------------------------------------------------------------------------
// Handles a system event
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::HandleSystemEventL( TSystemEvent aEvent,
                                                      TInt aData )
    {
    MPX_DEBUG3("CMPXHarvesterFileHandlerImp::HandleSystemEventL %i, drive %d", aEvent, aData);
    // How to handle each event
    //
    // 1: Format and eject, we stop scanning and close only the mmc db
    // 2: Format end and disk insert we reopen db and scan for new files
    // 3: USB start we stop scan and close all db
    // 4: USB end we re-open all db and scan for new files
    // 5: MTP start we stop monitoring for new files (no dismount)
    // 6: MTP end we re-open all db, files added already, restart monitor
    // 7: Disk dismount: stop scanning, close the dismounting DB
    //
    // Get all visible drives
    TDriveList driveList;
    TInt driveCount(0);
    TPtrC drivePresent(_L("present"));
    TPtrC driveNotPresent(_L("not present"));
    TPtrC driveInUse(_L("in use"));
    TPtrC driveAvailable(_L("available"));
    TPtrC driveFormatted(_L("formatted"));
    TPtrC driveNotFormatted(_L("not formatted"));
    User::LeaveIfError( DriveInfo::GetUserVisibleDrives(
           iFs, driveList, driveCount ) );
    MPX_DEBUG2 ("CMPXHarvesterFileHandlerImp::HandleSystemEventL - driveCount = %d", driveCount);

    for( TInt driveNum = EDriveA; driveNum <= EDriveZ; driveNum++ )
        {
        if (driveList[driveNum])
            {
            // Get the drive status
            TUint driveStatus(0);
            User::LeaveIfError( DriveInfo::GetDriveStatus(
                iFs, driveNum, driveStatus ) );
            MPX_DEBUG3 ("CMPXHarvesterFileHandlerImp::HandleSystemEventL - drive %d status=0x%x", driveNum, driveStatus);
            TChar driveChar;
            User::LeaveIfError(
                iFs.DriveToChar( driveNum, driveChar ) );
            MPX_DEBUG5 ("CMPXHarvesterFileHandlerImp::HandleSystemEventL - drive %c: is %S, %S and %S",
                (TUint)driveChar,
                (driveStatus&DriveInfo::EDrivePresent)?&drivePresent:&driveNotPresent,
                (driveStatus&DriveInfo::EDriveInUse)?&driveInUse:&driveAvailable,
                (driveStatus&DriveInfo::EDriveFormatted)?&driveFormatted:&driveNotFormatted);
            }
        }

    switch( aEvent )
        {
        case EFormatStartEvent:
            {
            MPX_DEBUG2("Disk Format start event, drive %d", aData);
            iIdle->Cancel();
            CancelScan();
            iDBManager->CloseDatabase( (TDriveNumber) aData );
            RefreshScanDrivesL();
            break;
            }
        case EDiskRemovedEvent:
            {
            MPX_DEBUG2("Disk Removed event, drive %d", aData);
            iIdle->Cancel();
            CancelScan();
            iDBManager->DropDatabase ( TDriveNumber( aData ) );
            RefreshScanDrivesL();
            break;
            }
        case EFormatEndEvent:
            {
            MPX_DEBUG2("Disk Format end event, drive %d", aData);
            CancelScan();
            iDBManager->OpenDatabaseL( (TDriveNumber) aData );
            RefreshScanDrivesL();
            break;
            }
        case EDiskInsertedEvent:
            {
            MPX_DEBUG2("Disk Insert event %d", aData);
            CancelScan();
            iDBManager->OpenDatabaseL( (TDriveNumber) aData );
            RefreshScanDrivesL();
            break;
            }
        case EUSBMassStorageStartEvent:
            {
            if (iCurUSBEvent == EUSBMassStorageStartEvent)
                {
                break;
                }            
            iIdle->Cancel();
            iDBManager->CloseMassStorageDatabases();
            RefreshScanDrivesL();
            CancelScan();
            iCurUSBEvent = EUSBMassStorageStartEvent;
            break;
            }
        case EUSBMassStorageEndEvent:
            {
            iDBManager->OpenAllDatabasesL();
            RefreshScanDrivesL();
            iCurUSBEvent = EUSBMassStorageEndEvent;
            break;
            }
        case EUSBMTPNotActiveEvent:
            {
            if (iCurUSBEvent == EUSBMTPNotActiveEvent)
            	{
            	break;
            	}
            if ( iRefreshing )
                {
                // Notify clients that refresh is cancelled.
                iCollectionUtil->Collection().NotifyL( EMcMsgRefreshEnd, KErrLocked );
                }
            CancelScan();
            iCurUSBEvent = EUSBMTPNotActiveEvent;
            break;
            }
        case EUSBMTPStartEvent:
            {
            CancelScan();
            iCurUSBEvent = EUSBMTPStartEvent;
#ifdef __RAMDISK_PERF_ENABLE
            iDBManager->CopyDBsToRamL(ETrue);
#endif //__RAMDISK_PERF_ENABLE
            break;
            }
        case EUSBMTPEndEvent:
            {
            iCurUSBEvent = EUSBMTPEndEvent;
#ifdef __RAMDISK_PERF_ENABLE
            // copy dbs from ram drive
            iDBManager->CopyDBsFromRamL();
#endif //__RAMDISK_PERF_ENABLE
            break;
            }
        case EDiskDismountEvent:
            {
            MPX_DEBUG2("Disk dismount notification, drive %d", aData);
            iIdle->Cancel();
            if ( aData < 0 )
                {
                iDBManager->CloseMassStorageDatabases();
                }
            else
                {
                iDBManager->CloseDatabase( (TDriveNumber) aData );
                }
            RefreshScanDrivesL();
            CancelScan();
            break;
            }
        default:
            {
            MPX_DEBUG1("CCMPXHarvesterFileHandlerImp::HandleSystemEventL Unknown system event!");
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// Add a file to the harvester db
// ---------------------------------------------------------------------------
//
CMPXMedia* CMPXHarvesterFileHandlerImp::AddFileL( const TDesC& aPath )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::AddFileL <---");

    // Create the media properties and add as usual.
    //
    CMPXMedia* prop = iMetadataScanner->ExtractFileL( aPath );
    CleanupStack::PushL( prop );
    AddFileL( *prop );
    CleanupStack::Pop( prop );

    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::AddFileL --->");
    return prop;
    }

// ---------------------------------------------------------------------------
// Add a file to the harvester db
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterFileHandlerImp::AddFileL( CMPXMedia& aMediaProp )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::AddFileL <---");

    // Parse
    TPtrC path = aMediaProp.ValueText( TMPXAttribute( KMPXMediaIdGeneral,
                                                       EMPXMediaGeneralUri) );
    TInt r(0);
    // Make sure we have a "file"
    if( path.Compare(KNullDesC) )
        {
        // Last modified time
        TTime lastModTime;
        iFs.Modified( path, lastModTime );

        // Collection UID,
        CMPXCollectionMediator* mediator = CMPXCollectionMediator::NewL();
        CleanupStack::PushL( mediator );
        mediator->CheckItemL( aMediaProp );
        CleanupStack::PopAndDestroy( mediator );

        TUid col( KNullUid );
        if( aMediaProp.IsSupported( TMPXAttribute( KMPXMediaIdGeneral,
                                                    EMPXMediaGeneralCollectionId ) ) )
            {
            col = aMediaProp.ValueTObjectL<TUid>( TMPXAttribute(
                                                  KMPXMediaIdGeneral,
                                                  EMPXMediaGeneralCollectionId ) );
            }
        MPX_DEBUG2("CMPXHarvesterFileHandlerImp::AddFileL aMediaProp->Value<TUid> col = %i", col.iUid);
        // Collection not set yet
        if( col.iUid == 0 )
            {
            TInt index = IsMediaFileL( path );
            if( index >= KErrNone )
                {
                MPX_DEBUG2(_L("CMPXHarvesterFileHandlerImp::AddFileL - count: %i"),iSupportedTypes.Count());
                col = iSupportedTypes[index]->Uid();
                MPX_DEBUG2(_L("CMPXHarvesterFileHandlerImp::AddFileL Selected Collection %i"), col.iUid);
                aMediaProp.SetTObjectValueL<TUid>( TMPXAttribute(
                                                    KMPXMediaIdGeneral,
                                                    EMPXMediaGeneralCollectionId ),
                                                    col );
                }
            }

        // drm
        TBool drm(EFalse);
        if( aMediaProp.IsSupported( TMPXAttribute(KMPXMediaIdDrm,
                                                   EMPXMediaDrmProtected) ) )
            {
            if( aMediaProp.ValueTObjectL<TBool>(TMPXAttribute(KMPXMediaIdDrm,
                                                 EMPXMediaDrmProtected)) )
                {
                TInt rights( EMPXDrmRightsFull );

                if( aMediaProp.IsSupported( TMPXAttribute(KMPXMediaIdDrm,
                                                           EMPXMediaDrmRightsStatus) ) )
                    {
                    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::AddFiles -- getting rights");
                    rights = aMediaProp.ValueTObjectL<TInt>( TMPXAttribute(KMPXMediaIdDrm,
                                                                     EMPXMediaDrmRightsStatus) );
                    }
                MPX_DEBUG2("CMPXHarvesterFileHandlerImp::AddFiles -- rights %i", rights);
                if( rights == EMPXDrmRightsMissing ||
                    rights == EMPXDrmRightsExpired )
                    {
                    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::AddFiles -- drm");
                    drm = ETrue;
                    }
                }
            }
        if( aMediaProp.IsSupported(KMPXMediaGeneralFlags) )
            {
            MPX_DEBUG1("CMPXHarvesterFileHandlerImp::AddFiles -- db flags");
            TUint dbflags( aMediaProp.ValueTObjectL<TUint>(KMPXMediaGeneralFlags) );
            if( dbflags&KMPXMediaGeneralFlagsIsDrmLicenceInvalid )
                {
                MPX_DEBUG1("CMPXHarvesterFileHandlerImp::AddFiles -- db flags drm invalid");
                drm = ETrue;
                }
            }
        // Add to database
#ifdef __RAMDISK_PERF_ENABLE
        // EnsureRamSpaceL will copy dbs from ram if ram space is low or dbs exceeded max space.
        iDBManager->EnsureRamSpaceL();
#endif // __RAMDISK_PERF_ENABLE
        CMPXHarvesterDB& db = iDBManager->GetDatabaseL( ::ExtractDrive( path ) );
        CMPXHarvesterDatabaseTable* table = db.OpenFileL( path );
        CleanupStack::PushL( table );

        // Don't add something we already have
        //
        if( table->CountL() == 0 )
            {
            TParsePtrC parse( path );
            table->AddItemL( parse.DriveAndPath(), parse.NameAndExt(),
                             lastModTime, col.iUid, drm );
            }
        CleanupStack::PopAndDestroy( table );

        // Return the collection that it should belong to.
        r = col.iUid;
#ifdef __RAMDISK_PERF_ENABLE
        // This feature will be turned on in the second phase.
        //TRAP_IGNORE( iDBManager->UpdateDBsFromRamL(0) );
#endif // __RAMDISK_PERF_ENABLE
        }
    else
        {
        // No file path, leave KErrArgument!
        User::Leave( KErrArgument );
        }

    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::AddFileL --->");
    return r;
    }

// ---------------------------------------------------------------------------
// Remove a file from the harvester db
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterFileHandlerImp::RemoveFileL( const TDesC& aPath, TBool aEndTransaction )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::RemoveFileL <---");
    TInt r(0);

    // Open the db
#ifdef __RAMDISK_PERF_ENABLE
    // EnsureRamSpaceL will copy dbs from ram if ram space is low or dbs exceeded max space.
    iDBManager->EnsureRamSpaceL();
#endif // __RAMDISK_PERF_ENABLE
    CMPXHarvesterDB& db = iDBManager->GetDatabaseL( ::ExtractDrive(aPath) );
	MPX_PERF_START( MPX_PERF_HARV_DB_DELETE_SUB1 );
    CMPXHarvesterDatabaseTable* table = db.OpenFileL( aPath );
    MPX_PERF_END( MPX_PERF_HARV_DB_DELETE_SUB1 );

    CleanupStack::PushL( table );

    // Create a cached copy of the db
	MPX_PERF_START( MPX_PERF_HARV_DB_DELETE_SUB2 );
    RPointerArray<CMPXHarvesterDbItem>* ary =
                                           table->CreateTableRepresentationL();
    TCleanupItem cleanup( CleanupArray, ary );
    CleanupStack::PushL( cleanup );
    MPX_PERF_END( MPX_PERF_HARV_DB_DELETE_SUB2 );

    // Delete the item from db
    MPX_PERF_START( MPX_PERF_HARV_DB_DELETE_SUB3 );
    TRAPD( err, table->DeleteItemL(aEndTransaction) );
    MPX_PERF_END( MPX_PERF_HARV_DB_DELETE_SUB3 );

    // If delete was successful, that means the item existed
    // find the collection db id for return
    //
    if( err == KErrNone )
        {
        CMPXHarvesterDbItem* item = (*ary)[0];
        if( item != NULL )
            {
            r = item->iColId;
            }
        }
    else
        {
        r=err;
        }
    CleanupStack::Pop( ary );
    ary->ResetAndDestroy();
    delete ary;
    CleanupStack::PopAndDestroy( table );

    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::RemoveFileL --->");
    return r;
    }

// ---------------------------------------------------------------------------
// Update the properties of a file
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::UpdateFileL( const TDesC& aFile, TInt aCollection )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::UpdateFileL <---");

    OpenDBForPathL( aFile );

     // Last modified time
    TTime lastModTime;
    iFs.Modified( aFile, lastModTime );

    iCurTable->UpdateItemL( lastModTime, aCollection,!iRefreshing );
    Reset();
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::UpdateFileL --->");
    }

// ---------------------------------------------------------------------------
// Rename a file
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::RenameFileL( const TDesC& aOldPath,
                                               const TDesC& aNewPath,
                                               TInt aCollection )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::RenameFileL <---");

    // update harvester database
    OpenDBForPathL( aOldPath );

     // Last modified time
    TTime lastModTime;
    iFs.Modified( aNewPath, lastModTime );

    iCurTable->UpdateItemL( lastModTime, aCollection, !iRefreshing, aNewPath );
    Reset();

    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::RenameFileL --->");
    }

// ---------------------------------------------------------------------------
// Finds the associated collection id for a file
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterFileHandlerImp::FindCollectionIdL( const TDesC& aFile )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::FindCollectionIdL <---");
    TInt r(0);

    // Open the db
    CMPXHarvesterDB& db = iDBManager->GetDatabaseL( ::ExtractDrive(aFile) );
    CMPXHarvesterDatabaseTable* table = db.OpenFileL( aFile );
    CleanupStack::PushL( table );

    // Create a cached copy of the db
    RPointerArray<CMPXHarvesterDbItem>* ary =
                                           table->CreateTableRepresentationL();
    if( ary->Count() > 0 )
        {
        CMPXHarvesterDbItem* item = (*ary)[0];  // not owned
        r = item->iColId;
        }
    else
        {
        r = KErrNotFound;
        }
    ary->ResetAndDestroy();
    delete ary;
    CleanupStack::PopAndDestroy( table );

    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::FindCollectionIdL --->");
    return r;
    }


// ---------------------------------------------------------------------------
// Re-create all databases
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::RecreateDatabases()
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::RecreateDatabases <--");
    iDBManager->RecreateDatabases();
    TRAP_IGNORE(RefreshScanDrivesL());
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::RecreateDatabases -->");
    }

// ---------------------------------------------------------------------------
// Close database transaction
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::CloseTransactionL()
    {
    iDBManager->CommitL();
    }

// ---------------------------------------------------------------------------
// Get a media object for the file
// ---------------------------------------------------------------------------
//
CMPXMedia* CMPXHarvesterFileHandlerImp::GetMediaForFileL( const TDesC& aPath )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::GetMediaForFileL <---");

    // Create the media properties
    //
    CMPXMedia* prop = iMetadataScanner->ExtractFileL( aPath );

    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::GetMediaForFileL --->");
    return prop;
    }

// ---------------------------------------------------------------------------
// Get Collection Uid for the file
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterFileHandlerImp::GetColUidForFileL( const TDesC& aPath )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::GetColUidForFileL <---");

    if(aPath == KNullDesC)
        {
        // No file path, leave KErrArgument!
        User::Leave( KErrArgument );
        }

    TInt ret(0);
    TInt index = IsMediaFileL( aPath );
    if( index >= KErrNone )
        {
        ret = iSupportedTypes[index]->Uid().iUid;
        }

    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::GetColUidForFileL --->");
    return ret;
    }

// ---------------------------------------------------------------------------
// Remove multiple files from the harvester db
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::RemoveFilesL( const MDesCArray& aFilePaths )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::RemoveFilesL <---");

    TInt count( aFilePaths.MdcaCount() );
    TBool endTransaction(EFalse);
    for( TInt i=0; i<count; ++i )
        {
        if ( (i == count-1) || (i%KBatchCommit == 0) )
            {
            endTransaction = ETrue;
            }
        User::LeaveIfError( RemoveFileL( aFilePaths.MdcaPoint(i), endTransaction ) );
        }

    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::RemoveFilesL --->");
    }

// ---------------------------------------------------------------------------
// Remove all files from the harvester db
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::RemoveAllFilesL()
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::RemoveAllFilesL <---");
    TInt dbCount( iDBManager->Count() );
    for( TInt i=0; i< dbCount; ++i )
        {
        CMPXHarvesterDB& db = iDBManager->GetDatabaseL( i );
        db.RemoveAllFilesL();
        }
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::RemoveAllFilesL --->");
    }

// ---------------------------------------------------------------------------
// Handles a state change event
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::HandleScanStateCompleteL( TScanState aState,
                                                            TInt aErr )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::HandleScanStateCompleteL <---");

    // Cleanup associated with each state
    //
    switch( aState )
        {
        case EScanFiles:
            {
#ifdef __PRINTDB__
            if( iCurTable )
                iCurTable->PrintItemsInTableL();
#endif //__PRINTDB__

            // Look for removed items
            // Do not look for removed files if there was an error
            if( aErr == KErrNone )
                {
                HandleBrokenItemsL();
                }
            Reset();
            break;
            }
        case ECleanupBrokenLink:
            {
            iBrokenLink->Reset();
            break;
            }
        case EScanPlaylists:
            {
            iPlaylistScanner->Reset();
            break;
            }
        case EScanMetadata:
            {
            iMetadataScanner->Reset();
            break;
            }
        default:
            break;
        }

    // Next state handling
    //
    if( KErrNone == aErr )
        {
        switch( aState )
            {
            case EScanFiles:
                {
                MPX_DEBUG1("Start Broken Link");
                iBrokenLink->Start();
                break;
                }
            case ECleanupBrokenLink:
                {
#ifdef __RAMDISK_PERF_ENABLE
                // copy dbs to ram drive
                iDBManager->CopyDBsToRamL();
#endif //__RAMDISK_PERF_ENABLE
    
                MPX_DEBUG1("Start Metadata Scan");
                iMetadataScanner->Start();
                break;
                }
            case EScanMetadata:
                {
                MPX_DEBUG1("Start Metadata Scan");
                iPlaylistScanner->ScanL();
                break;
                }
            case EScanPlaylists:
                {
                MPX_DEBUG1("Scan complete");
                
                TRAPD(err, DoCompleteRefreshL( KErrNone ));
                
                // handle DoCompleteRefreshL leave
                if( err != KErrNone )
                    {
                    // finish refresh, close waitnote
                    iCollectionUtil->Collection().NotifyL( EMcMsgRefreshEnd, err );
                    }
                
                // Reset all scan states
                Reset();
                break;
                }
            default:
                break;
            }
        }
    else
        {
        // Error occured, check for out of disk
        //
        TInt error = iOutOfDisk ? KErrDiskFull : aErr;
        TRAPD(err, DoCompleteRefreshL( error ));
        
        // handle DoCompleteRefreshL leave
        if( err != KErrNone )
            {
            // finish refresh, close waitnote
            iCollectionUtil->Collection().NotifyL( EMcMsgRefreshEnd, err );
            }
        }
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::HandleScanStateCompleteL <---");
    }

// ---------------------------------------------------------------------------
// Handles a state change event
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::HandleSynchronizationComplete( TInt aErr )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::HandleSynchronizationComplete enter");

    iSynchronizing = EFalse;

    if(aErr == KErrNone)
        {
        // Start the scanning process
        iRefreshCount++;
        // Keep a count of how many we added to syncrhonize the number
        iAddedCount = 0;
        iRefreshing = ETrue;

        TRAPD(err,iFolderScanner->ScanL( iFilteredDrivesToScan ));
        if(err == KErrNone)
            {
            iMetadataScanner->Reset();
            iBrokenLink->Reset();
            iPlaylistScanner->Reset();
            iIdle->Cancel();
            }
        else
            {
            TRAPD(error, DoCompleteRefreshL( err ));
            
            // handle DoCompleteRefreshL leave
            if( error != KErrNone )
                {
                // finish refresh, close waitnote
                TRAP_IGNORE( iCollectionUtil->Collection().NotifyL( EMcMsgRefreshEnd, error ) );
                }
            }        
        }
    else
        {
        iRefreshCount++;
        iRefreshing = ETrue;
        TInt error = iOutOfDisk ? KErrDiskFull : aErr;
        TRAPD(err, DoCompleteRefreshL( error ));
        
        // handle DoCompleteRefreshL leave
        if( err != KErrNone )
            {
            // finish refresh, close waitnote
            TRAP_IGNORE( iCollectionUtil->Collection().NotifyL( EMcMsgRefreshEnd, err ) );
            }
        }
    MPX_DEBUG1("<--- CMPXHarvesterFileHandlerImp::HandleSynchronizationCompleteL exit");
    }

// ---------------------------------------------------------------------------
// Handle adding a single file
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::HandleFileAdditionL( const TDesC& aFileName,
                                                       TInt /*aColIndex*/,
                                                       TBool /*aPlaylist*/ )
    {
    MPX_DEBUG2("CMPXHarvesterFileHandlerImp::HandleFileAdditionL %S<---", &aFileName );

    // Find the item in the array
    //
    CMPXHarvesterDbItem tmp;
    tmp.iFile = aFileName.AllocL();

    // Last modified time
    //
    TTime lastModTime;
    iFs.Modified( aFileName, lastModTime );

    // This is very inefficient, should hash.
    //
    if( iCurList )
        {
        TInt index = iCurList->FindInOrder( &tmp, CMPXHarvesterDbItem::Compare );
        if( index != KErrNotFound )
            {
            MPX_DEBUG1("CMPXHarvesterFileHandlerImp::HandleFileAdditionL found file");

            // Check modified time stamp to see if it was modified
            // Also check files that did not have rights, see if we have rights now
            //
            CMPXHarvesterDbItem* item = (*iCurList)[index];
            if( item->iLastModifiedTime != lastModTime || item->iDrm )
                {
                MPX_DEBUG1("Rescanning a file because of modified or drm");
                HandleUpdatedItemL( aFileName );
                }

            // If found, we remove it from the list, items left should be deleted
            //
            delete item;
            iCurList->Remove( index );
            }
        else  // new file
            {
            HandleNewItemL( aFileName );
            }
        }
    else
        {
        MPX_DEBUG1("CMPXHarvesterFileHandlerImp::HandleFileAdditionL Scan cancelled");
        }

    delete tmp.iFile;
    tmp.iFile = NULL;

    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::HandleFileAdditionL --->");
    }

// ---------------------------------------------------------------------------
// Handle a change in the file system, could be a file added/removed
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::HandleDirectoryChangedL( const TDesC& aPath )
    {
    MPX_DEBUG2("--->CMPXHarvesterFileHandlerImp::HandleDirectoryChangedL path=%S", &aPath);
    // Delay the scanning for a few seconds so the files are finished
    // copying. If already active, means we just append onto the list
    // But don't scan if there is no DB == drive does not exist any more
    TParsePtrC parse( aPath );
    TDriveUnit drive ( parse.Drive() );
    if ( iDBManager->DatabaseIsOpen ((TDriveNumber) (TInt) drive) )
        {
        MPX_DEBUG1("CMPXHarvesterFileHandlerImp::HandleDirectoryChangedL adding in iAutoScanPaths");
        iAutoScanPaths.AppendL( aPath );
        if( !iIdle->IsActive() )
            {
            TCallBack cb( Callback, this );
            iIdle->Start( TTimeIntervalMicroSeconds32( KAutoScanDelay ),
                          TTimeIntervalMicroSeconds32( KAutoScanAfter ),
                          cb );
            }
        }
    }

// ---------------------------------------------------------------------------
// Handles opening the drive
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::HandleOpenDriveL( TDriveNumber aDrive,
                                                    const TDesC& aFolder )
    {
    MPX_DEBUG3("--->CMPXHarvesterFileHandlerImp::HandleOpenDriveL drive=%d, folder=%S", aDrive, &aFolder);
#ifdef __PRINTDB__
    if( iCurTable )
        iCurTable->PrintItemsInTableL();
#endif //__PRINTDB__

    // Look for broken files
    HandleBrokenItemsL();

    // Delete previous table and open the next one
    Reset();
#ifdef __RAMDISK_PERF_ENABLE
    // EnsureRamSpaceL will copy dbs from ram if ram space is low or dbs exceeded max space.
    iDBManager->EnsureRamSpaceL();
#endif // __RAMDISK_PERF_ENABLE
    iCurDB = &iDBManager->GetDatabaseL( aDrive );
    if( iDrivesToScan.Find( aFolder ) != KErrNotFound )
        {
        iCurTable = iCurDB->OpenAllFilesTableL();
        }
    else
        {
        iCurTable = iCurDB->OpenDirectoryL( aFolder );
        }
    iCurList = iCurTable->CreateTableRepresentationL();
    MPX_DEBUG1("<--CMPXHarvesterFileHandlerImp::HandleOpenDriveL");
    }

// ---------------------------------------------------------------------------
// Is this a media file we are interested in
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterFileHandlerImp::IsMediaFileL( const TDesC& aFile )
    {
    MPX_DEBUG2("CMPXHarvesterFileHandlerImp::IsMediaFileL %S <---", &aFile);

    TParsePtrC parse( aFile );
    TInt index(KErrNotFound);
    TInt count( iSupportedTypes.Count() );
    for (TInt i=0; i <count; ++i)
        {
        TInt index2(KErrNotFound);
        const CDesCArray& exts = iSupportedTypes[i]->Extensions();
        if (!exts.FindIsq(parse.Ext(), index2))
            { // found
            index = i;
            break;
            }
        }

    if( index == KErrNotFound )
        {
        index = IsPlaylistFileL( aFile ) ? ETrue : KErrNotFound;
        }

    MPX_DEBUG2("CMPXHarvesterFileHandlerImp::IsMediaFileL %i --->", index);
    return index;
    }

// ---------------------------------------------------------------------------
// Is this a playlist file we are interested in
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterFileHandlerImp::IsPlaylistFileL( const TDesC& aFile )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::IsPlaylistFileL <---");
    return iPlaylistScanner->IsPlaylistFileL( aFile );
    }

// ---------------------------------------------------------------------------
// Checks if this path is in the blocked list
// ---------------------------------------------------------------------------
//
TBool CMPXHarvesterFileHandlerImp::IsPathBlockedL( const TDesC& aPath )
    {
    TInt count( iPathsToBlock.Count() );
    TBool isBlocked(EFalse);
    HBufC* buf = aPath.AllocLC();
    TPtr ptr = buf->Des();
    ptr.UpperCase();
    for( TInt i=0; i<count; ++i )
        {
        if( ptr.Find( iPathsToBlock[i] ) != KErrNotFound )
            {
            isBlocked = ETrue;
            break;
            }
        }
    CleanupStack::PopAndDestroy( buf );
    return isBlocked;
    }

// ---------------------------------------------------------------------------
// Adds multiple files to the collection
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::AddFilesToCollectionL(
                                                CMPXMediaArray& aMediaArray )
    {
    MPX_DEBUG1("MPXHarvesterFileHandlerImp::AddFilesToCollectionL <---");

    // Add to collection db
    CMPXCollectionMediator* mediator =
                CMPXCollectionMediator::NewL( iCollectionUtil->Collection(),
                                              this );
    CleanupStack::PushL( mediator );
    TRAPD( addErr, mediator->AddItemL( aMediaArray ) );
    CleanupStack::PopAndDestroy( mediator );

    // Now we add them to the harvester db
    //
    if( addErr == KErrNone )
        {
        TInt count = aMediaArray.Count();
        for( TInt i=0; i<count; ++i )
            {
            const TDesC& path = aMediaArray.AtL(i)->ValueText( TMPXAttribute(
                                                                KMPXMediaIdGeneral,
                                                                EMPXMediaGeneralUri ) );
            OpenDBForPathL( path );

            // Collection
            const TUid& collection = aMediaArray.AtL(i)->ValueTObjectL<TUid>(
                                   TMPXAttribute( KMPXMediaIdGeneral,
                                                  EMPXMediaGeneralCollectionId ) );

            // Last modified time
            TTime lastModTime;
            iFs.Modified( path, lastModTime );

            // drm
            TBool drm(EFalse);
            if( aMediaArray[i]->IsSupported( TMPXAttribute(KMPXMediaIdDrm,
                                                     EMPXMediaDrmProtected) ) &&
                aMediaArray.AtL(i)->ValueTObjectL<TBool>(TMPXAttribute(KMPXMediaIdDrm,
                                                     EMPXMediaDrmProtected)) )
                {
                TInt rights = EMPXDrmRightsFull;
                if( aMediaArray[i]->IsSupported( TMPXAttribute(KMPXMediaIdDrm,
                                                               EMPXMediaDrmRightsStatus)) )
                    {
                    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::AddFilesToCollectionL -- getting rights");
                    rights =  aMediaArray.AtL(i)->ValueTObjectL<TInt>(
                              TMPXAttribute(KMPXMediaIdDrm, EMPXMediaDrmRightsStatus) );
                    }
                if( rights == EMPXDrmRightsMissing ||
                    rights == EMPXDrmRightsExpired )
                    {
                    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::AddFilesToCollectionL -- rights missing/expired");
                    drm = ETrue;
                    }
                }

            TParsePtrC parse( path );
            iCurTable->AddItemL( parse.DriveAndPath(),
                                 parse.NameAndExt(),
                                 lastModTime, collection.iUid, drm );

            iAddedCount++;
            }
#ifdef __RAMDISK_PERF_ENABLE
        // This feature will be turned on in the second phase.
        //TRAP_IGNORE( iDBManager->UpdateDBsFromRamL(iAddedCount) );
#endif // __RAMDISK_PERF_ENABLE
        }
    else if ( addErr == KErrDiskFull )
        {
        User::Leave( KErrDiskFull );
        }

    MPX_DEBUG1("MPXHarvesterFileHandlerImp::AddFilesToCollectionL --->");
    }

// ---------------------------------------------------------------------------
// Updates some files to the collection
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::UpdatesFilesInCollectionL(
                                                 CMPXMediaArray& aMediaArray )
    {
    MPX_DEBUG1("MPXHarvesterFileHandlerImp::UpdatesFilesInCollectionL <---");

    // Do not update files which still do not have rights
    //
    TInt c( aMediaArray.Count() );
    for( TInt i=0; i<c; ++i )
        {
        TInt rights = EMPXDrmRightsFull;

        if( aMediaArray.AtL(i)->IsSupported( KMPXMediaDrmRightsStatus ) )
            {
            rights = aMediaArray.AtL(i)->ValueTObjectL<TInt>( KMPXMediaDrmRightsStatus );
            }
        if( rights == EMPXDrmRightsMissing )
            {
            aMediaArray.Remove(i);
            i--;
            c--;
            }
        }

    // Update collection db
    CMPXCollectionMediator* mediator =
                CMPXCollectionMediator::NewL( iCollectionUtil->Collection(),
                                              this );
    CleanupStack::PushL( mediator );
    TRAPD( setErr , mediator->SetItemL( aMediaArray ) );
    CleanupStack::PopAndDestroy( mediator );

    // Now we update them to the harvester db
    //
    if( setErr == KErrNone )
        {
        TInt count( aMediaArray.Count() );
        for( TInt i=0; i<count; ++i )
            {
            const TDesC& filepath = aMediaArray.AtL(i)->ValueText( KMPXMediaGeneralUri );
            OpenDBForPathL( filepath );

            // Collection
            TUid collection = aMediaArray.AtL(i)->ValueTObjectL<TUid>( KMPXMediaGeneralCollectionId );
            // Last modified time
            TTime lastModTime;
            iFs.Modified( filepath, lastModTime );

            // drm
            TBool drm(EFalse);
            if( aMediaArray.AtL(i)->IsSupported(KMPXMediaDrmProtected)  &&
                aMediaArray.AtL(i)->ValueTObjectL<TBool>(KMPXMediaDrmProtected) )
                {
                TInt rights = EMPXDrmRightsFull;

                if( aMediaArray.AtL(i)->IsSupported(KMPXMediaDrmRightsStatus) )
                    {
                    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::UpdateFilesToCollectionL -- getting rights");
                    rights = aMediaArray.AtL(i)->ValueTObjectL<TInt>(KMPXMediaDrmRightsStatus);
                    }
                if( rights == EMPXDrmRightsMissing ||
                    rights == EMPXDrmRightsExpired )
                    {
                    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::UpdateFilesToCollectionL -- updating rights");
                    drm = ETrue;
                    }
                }

            iCurTable->UpdateItemL( filepath, lastModTime, collection.iUid, drm, !iRefreshing );
            }
        }
    else if ( setErr == KErrDiskFull )
        {
        User::Leave( KErrDiskFull );
        }

    MPX_DEBUG1("MPXHarvesterFileHandlerImp::UpdatesFilesInCollectionL --->");
    }

// ---------------------------------------------------------------------------
// Updates the db based on mediator decisions
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::HandleMediatorPathUpdatedL(
                                                         CMPXMedia*& aProperty,
                                                         TUid  /*aOldPath*/  )
    {
    MPX_DEBUG1("MPXHarvesterFileHandlerImp::UpdatesFilesInCollectionL <---");

    // Update harvester database based on mediator changes
    //
    const TDesC& filepath = aProperty->ValueText( KMPXMediaGeneralUri );
    OpenDBForPathL( filepath );

    // Collection
    //
    TUid colUid = aProperty->ValueTObjectL<TUid>( KMPXMediaGeneralCollectionId );

    // Last modified time
    //
    TTime lastModTime;
    iFs.Modified( filepath, lastModTime );

    // Update DB, only if the record existed. No record means this is a new
    // entry
    if( iCurTable->CountL() )
        {
        iCurTable->UpdateItemL( lastModTime, colUid.iUid, !iRefreshing );
        }

    MPX_DEBUG1("MPXHarvesterFileHandlerImp::UpdatesFilesInCollectionL <---");
    }

// ---------------------------------------------------------------------------
// Add playlists to the collection
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::AddPlaylistToCollectionL(
                                                 CMPXMediaArray& aMediaArray )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::AddPlaylistToCollectionL <---");

    //
    // add to collection one by one to avoid the following scenario:
    // 2 playlists to add to the collection. The 1st one is successfully
    // added but the 2nd playlist isn't. When AddL leaves, it does not
    // indicate which one is successfully added and which one isn't.
    // As a result, the successfully added playlist isn't added to
    // harvester database and during the next scan, it will be picked
    // up again and added to the database with auto-numbered title.
    //
    TInt count( aMediaArray.Count() );
    for (TInt i=0; i<count; ++i)
        {
        CMPXMedia* media = aMediaArray.AtL(i);

        TRAPD(addErr, DoCommandL( KMPXCommandIdCollectionAdd,
                                  media->ValueTObjectL<TUid>(KMPXMediaGeneralCollectionId),
                                  TMPXAttribute(KMPXCommandColAddMedia),
                                  media,
                                  ETrue  ) );

        // Now we add the item to the harvester db
        //
        if( addErr == KErrNone )
            {
            const TDesC& path =
                media->ValueText( TMPXAttribute( KMPXMediaIdGeneral, EMPXMediaGeneralUri ) );
            OpenDBForPathL( path );

            // Collection
            TUid collection =
                media->ValueTObjectL<TUid>( TMPXAttribute( KMPXMediaIdGeneral, EMPXMediaGeneralCollectionId ) );

            // Last modified time
            TTime lastModTime;
            iFs.Modified( path, lastModTime );

            TParsePtrC parse( path );
            iCurTable->AddItemL( parse.DriveAndPath(),
                                 parse.NameAndExt(),
                                 lastModTime, collection.iUid, EFalse );
            iAddedCount++;
            }
        else if ( addErr == KErrDiskFull )
            {
            User::Leave( KErrDiskFull );
            }
        }
#ifdef __RAMDISK_PERF_ENABLE
        // This feature will be turned on in the second phase.
        //TRAP_IGNORE( iDBManager->UpdateDBsFromRamL(iAddedCount) );
#endif // __RAMDISK_PERF_ENABLE

    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::AddPlaylistToCollectionL --->");
    }

// ---------------------------------------------------------------------------
// Update Playlists in the collection
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::UpdatePlaylistToCollectionL(
                                                 CMPXMediaArray& aMediaArray )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::UpdatePlaylistToCollectionL <---");

    // Update the collection
    //
    TInt count( aMediaArray.Count() );
    for (TInt i=0; i<count; ++i)
        {
        CMPXMedia* media = aMediaArray.AtL(i);

        TRAPD( setErr, DoCommandL( KMPXCommandIdCollectionSet,
                                   media->ValueTObjectL<TUid>(KMPXMediaGeneralCollectionId),
                                   KMPXCommandColSetMedia,
                                   media,
                                   ETrue ) );

        // Now we update them to the harvester db
        //
        if( setErr == KErrNone )
            {
            const TDesC& filepath =
                media->ValueText( TMPXAttribute( KMPXMediaIdGeneral, EMPXMediaGeneralUri ) );
            OpenDBForPathL( filepath );

            // Collection
            const TUid& collection =
                media->ValueTObjectL<TUid>( TMPXAttribute( KMPXMediaIdGeneral, EMPXMediaGeneralCollectionId ) );

            // Last modified time
            TTime lastModTime;
            iFs.Modified( filepath, lastModTime );

            iCurTable->UpdateItemL( filepath, lastModTime, collection.iUid, EFalse, !iRefreshing);
            }
        else if ( setErr == KErrDiskFull )
            {
            User::Leave( KErrDiskFull );
            }
        }

    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::UpdatePlaylistToCollectionL --->");
    }

// ---------------------------------------------------------------------------
// Handle low disk events
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::HandleLowDiskEvent( TInt /*aDrive*/ )
    {
    // Cancel the scanning process, set low disk flag to true
    iOutOfDisk = ETrue;
    CancelScan();
    }

// ---------------------------------------------------------------------------
// Handle Broken Links
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::HandleBrokenLinkL( MDesCArray& aFileArray,
                                                     RArray<TInt>& aColIds ,
                                                     RPointerArray<CMPXHarvesterDB>& aDbs,
                                                     TInt aCount )
    {
    // Common local variables
    //
    RArray<TInt> contId;
    CleanupClosePushL( contId );
    contId.AppendL( KMPXMediaIdGeneral );
    contId.AppendL( KMPXMediaIdContainer );

    RArray<TInt> itemId;
    CleanupClosePushL( itemId );
    itemId.AppendL( KMPXMediaIdGeneral );

    // Remove from Collection db
    //
    CMPXMediaArray* musicArray = CMPXMediaArray::NewL();
    CleanupStack::PushL( musicArray );
    CMPXMediaArray* podcastArray = CMPXMediaArray::NewL();
    CleanupStack::PushL( podcastArray );

    for(TInt j=0; j<aCount; ++j )
        {
        TPtrC uri(aFileArray.MdcaPoint(j));
        TUid uid = TUid::Uid( aColIds[j] );

        MPX_DEBUG1("CMPXHarvesterFileHandlerImp::HandleBrokenLinkL -- \
                    Want to remove an item");
        MPX_DEBUG2("%S", &uri);
        if (!(IsPlaylistFileL( uri )))
            {
            // Construct a CMPXMedia to represent the deleted object
            CMPXMedia* media = CMPXMedia::NewL( itemId.Array() );
            CleanupStack::PushL( media );
            media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType,
                                                     EMPXItem);
            media->SetTextValueL( KMPXMediaGeneralUri,
                                  uri );
            media->SetTObjectValueL<TMPXGeneralCategory>(KMPXMediaGeneralCategory,
                                                         EMPXSong );
            media->SetTObjectValueL<TUid>( KMPXMediaGeneralCollectionId,
                                           uid );
            if( uid == iMusicCollectionId )
                {
                musicArray->AppendL( media );
                }
            else
                {
                podcastArray->AppendL( media );
                }
            CleanupStack::Pop( media );  // ownership xfer

            // Cleanup harvester DB
            aDbs[j]->DeleteFileL( uri );
            }
        else
            {
            OpenDBForPathL( uri );

            //  Last modified time
            TTime lastModTime;
            lastModTime.HomeTime();

            iCurTable->UpdateItemL(uri, lastModTime, uid.iUid, EFalse);
            }
        }

    //  Package the media array and send to collection
    //
    if( musicArray->Count() )
        {
        CMPXMedia* rootMedia = CMPXMedia::NewL( contId.Array() );
        CleanupStack::PushL( rootMedia );
        rootMedia->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType, EMPXGroup );
        rootMedia->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaGeneralCategory, EMPXCollection );
        rootMedia->SetCObjectValueL<CMPXMediaArray>( KMPXMediaArrayContents, musicArray );
        rootMedia->SetTObjectValueL<TInt>( KMPXMediaArrayCount, musicArray->Count() );
        DoRemoveL( rootMedia, iMusicCollectionId );
        CleanupStack::PopAndDestroy( rootMedia );
        }
    if( podcastArray->Count() )
        {
        CMPXMedia* rootMedia = CMPXMedia::NewL( contId.Array() );
        CleanupStack::PushL( rootMedia );
        rootMedia->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType, EMPXGroup );
        rootMedia->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaGeneralCategory, EMPXCollection );
        rootMedia->SetCObjectValueL<CMPXMediaArray>( KMPXMediaArrayContents, podcastArray );
        rootMedia->SetTObjectValueL<TInt>( KMPXMediaArrayCount, podcastArray->Count() );
        DoRemoveL( rootMedia, iPodcastCollectionId );
        CleanupStack::PopAndDestroy( rootMedia );
        }

    // Cleanup
    //
    CleanupStack::PopAndDestroy( podcastArray );
    CleanupStack::PopAndDestroy( musicArray );

    contId.Reset();
    itemId.Reset();
    CleanupStack::PopAndDestroy( &itemId );
    CleanupStack::PopAndDestroy( &contId );
    }

// ---------------------------------------------------------------------------
// Parses cenrep setting for scan paths
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::ParseScanPathL()
    {
    // Gets the string from cenrep, use default if leave
    // string should be in the form 'item1' 'item2'
    //
    TBuf<255> scanPath;
    TBuf<255> blockPath;
    TRAPD( err, FetchPathsFromCenrepL(scanPath,blockPath) );
    if( err )
        {
        scanPath = KDefaultScanPath;
        blockPath = KDefaultBlockPath;
        }

    MPX_DEBUG2("ParseScanPathL scanPaths: %S", &scanPath);
    MPX_DEBUG2("ParseScanPathL blockPaths: %S", &blockPath);
    ::ExtractTokensL( scanPath, iConfiguredDrivesToScan );
    ::ExtractTokensL( blockPath, iPathsToBlock );
    }

// ---------------------------------------------------------------------------
// Fetches scan - and block - paths from cenrep 
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::FetchPathsFromCenrepL( TDes16& aScanPath, 
                                                         TDes16& aBlockPath )
    {
    MPX_FUNC("CMPXHarvesterFileHandlerImp::FetchPathsFromCenrepL");
    CRepository* cenrep( CRepository::NewLC( KCRUIDHarvesterFeatures ) );
    User::LeaveIfError( cenrep->Get( KHarvesterScanPathKey, aScanPath ) );
    User::LeaveIfError( cenrep->Get( KHarvesterBlockPathKey, aBlockPath ) );
    CleanupStack::PopAndDestroy(cenrep);
    }

// ---------------------------------------------------------------------------
// Parses cenrep setting for container file types
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::ParseContainersL()
    {
    TBuf<255> containers;
    CRepository* cenrep( NULL );
    TRAPD( err, cenrep = CRepository::NewL( KCRUIDHarvesterFeatures ) );
    if( err == KErrNone )
        {
        cenrep->Get( KHarvesterContainerKey, containers );
        delete cenrep;
        }
    else
        {
        containers = KDefaultContainers;
        }

    MPX_DEBUG2("Container types: %S", &containers);
    ::ExtractTokensL( containers, *iContainerTypes);
    }

// ---------------------------------------------------------------------------
// Parses cenrep setting for automatic scan folders
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::ParseAutoScanL()
    {
    // Make sure we don't insert duplicates
    iFolderMonitors.ResetAndDestroy();

    TBuf<255> folders;
    CRepository* cenrep( NULL );
    TRAPD( err, cenrep = CRepository::NewL( KCRUIDHarvesterFeatures ) );
    if( err == KErrNone )
        {
        err = cenrep->Get( KAutoScanDirectoryKey, folders );
        delete cenrep;
        }
    if( err != KErrNone )
        {
        folders = KDefaultAutoScanFolder;
        }

    MPX_DEBUG2("AutoScanFolders: %S", &folders);

    CDesCArrayFlat* array = new(ELeave) CDesCArrayFlat(2);
    CleanupStack::PushL( array );
    ::ExtractTokensL( folders, *array);

    TInt count( array->Count() );
    for( TInt i=0; i<count; ++i )
        {
        CMPXFolderMonitor* monitor = CMPXFolderMonitor::NewL( *this, iFs );
        CleanupStack::PushL( monitor );
        iFolderMonitors.AppendL( monitor );  // ownership xfer
        CleanupStack::Pop( monitor );
        monitor->StartL( array->MdcaPoint(i) );
        }
    CleanupStack::PopAndDestroy( array );
    }

// ---------------------------------------------------------------------------
// Refreshes scan drives
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::RefreshScanDrivesL()
    {
    iDrivesToScan.Reset();
    for (TInt i = 0; i < iConfiguredDrivesToScan.Count(); ++i)
        {
        const TDesC& path = iConfiguredDrivesToScan[i];
        TParsePtrC parse(path);
        TDriveUnit drive(parse.Drive());
        if ( iDBManager->DatabaseIsOpen( (TDriveNumber)(TInt) drive) )
            {
            iDrivesToScan.AppendL(path);
            }
        }
    }
    
// ---------------------------------------------------------------------------
// Resets the scanning table and array
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::Reset()
    {
    delete iCurTable;
    iCurTable = NULL;

    if( iCurList )
        {
        iCurList->ResetAndDestroy();
        delete iCurList;
        iCurList = NULL;
        }
    iCurDB = NULL; // not owned
    }

// ---------------------------------------------------------------------------
// Handles a new file added to the collection
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::HandleNewItemL( const TDesC& aFileName )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::HandleNewItemL new file");

    // Add it to the database
    //
    if( IsPlaylistFileL( aFileName ) )
        {
        iPlaylistScanner->AddPlaylistToScanL( aFileName );
        }
    else
        {
        iMetadataScanner->AddNewFileToScanL( aFileName );
        }
    }

// ---------------------------------------------------------------------------
// Handles broken items
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::HandleBrokenItemsL()
    {
    // Add the item to the broken links list
    //
    if( iCurList )
        {
        TInt count( iCurList->Count() );
        for( TInt i=0; i<count; ++i )
            {
            TPtrC filename = (*iCurList)[i]->iFile->Des();
            TInt colId = (*iCurList)[i]->iColId;
            iBrokenLink->AddBrokenLinkL( filename, colId, iCurDB );
            }
        }
    }

// ---------------------------------------------------------------------------
// Handles Updated items
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::HandleUpdatedItemL( const TDesC& aFile )
    {
    TBool playlist = IsPlaylistFileL( aFile );
    if( playlist )
        {
        iPlaylistScanner->AddUpdatedPlaylistToScanL( aFile );
        }
    else
        {
        iMetadataScanner->AddModifiedFileToScanL( aFile );
        }
    }

// ---------------------------------------------------------------------------
// Finds the associated collection for a file
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterFileHandlerImp::CollectionForExtensionL( const TDesC& aFile )
    {
    TInt index = IsMediaFileL( aFile );
    TInt val(0);
    if( KErrNotFound != index )
        {
        const CMPXCollectionType& type = *iSupportedTypes[index];
        val = type.Uid().iUid;
        }
    return val;
    }

// ---------------------------------------------------------------------------
// Setup the internal variables when opening a path
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::OpenDBForPathL( const TDesC& aPath )
    {
    Reset();
    TDriveNumber num = ::ExtractDrive( aPath );
#ifdef __RAMDISK_PERF_ENABLE
    // EnsureRamSpaceL will copy dbs from ram if ram space is low or dbs exceeded max space.
    iDBManager->EnsureRamSpaceL();
#endif // __RAMDISK_PERF_ENABLE
    iCurDB = &iDBManager->GetDatabaseL( num );
    iCurTable = iCurDB->OpenFileL( aPath );
    }

// ---------------------------------------------------------------------------
// Complete a refreshing event
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::DoCompleteRefreshL( TInt aErr )

    {
    // If no error or cancel, return the final number of items added
    MPX_DEBUG2("Scan error %i", aErr );
    
    if( aErr == KErrNone || aErr == KErrCancel )
        {
        // Commit the changes on databases in transaction
        iDBManager->CommitL();
        }
    else
        {
        // Rollback the changes on databases in transaction
        iDBManager->Rollback();
        }

#ifdef __RAMDISK_PERF_ENABLE
    // copy dbs from ram drive
    iDBManager->CopyDBsFromRamL();
#endif //__RAMDISK_PERF_ENABLE

    // Reopen databases (in case we removed them for out of disk drives before scan)
    for (TInt i = 0, j = 0; i < iDrivesToScan.Count(); ++i)
        {
        if ( j < iFilteredDrivesToScan.Count() && ! iDrivesToScan[i].Compare( iFilteredDrivesToScan[j] ) )
            {
             ++j;
            }
        else
            {
            TParsePtrC fileNameParser ( iDrivesToScan[i] );
            TDriveUnit drive ( fileNameParser.Drive() );
            TRAP_IGNORE( iDBManager->OpenDatabaseL( TDriveNumber ( (TInt) drive ) ) );
            }
        }

    if( aErr == KErrNone || aErr == KErrCancel )
        {
        aErr = iAddedCount;
        }
    
    if ( iFilteredOutOfDisk )
        {
        aErr = KErrDiskFull; 
        }

    if( iRefreshing )
        {
        iCollectionUtil->Collection().NotifyL( EMcMsgRefreshEnd, aErr );
        }
    iRefreshCount--;
    if ( iRefreshCount < 0 )
        {
        iRefreshCount = 0;
        }
    if( iCollectionUtil && (iRefreshCount == 0) )
        {
        iCollectionUtil->Close();
        iCollectionUtil = NULL;
        }
    iRefreshing = EFalse;

    // Cancel disk monitors
    TInt c (iDiskMonitors.Count());
    iOutOfDisk = EFalse;

    for( TInt i=0; i<c; ++i )
        {
        iDiskMonitors[i]->Cancel();
        }
    }

// ---------------------------------------------------------------------------
// Issue a command to the collection
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::DoCommandL( TInt aCommandId,
                                              TUid aCollectionId,
                                              const TMPXAttribute& aMediaAttribute,
                                              CMPXMedia* aMedia,
                                              TBool aSync )
    {
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );

    cmd->SetTObjectValueL( KMPXCommandGeneralId, aCommandId );
    cmd->SetTObjectValueL( KMPXCommandGeneralCollectionId, aCollectionId.iUid );
    cmd->SetTObjectValueL( KMPXCommandGeneralDoSync, aSync );
    if( aMedia )
        {
        cmd->SetCObjectValueL<CMPXMedia>( aMediaAttribute, aMedia );
        }


    iCollectionUtil->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );
    }

// ---------------------------------------------------------------------------
// Remove an item from the collection
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::DoRemoveL( CMPXMedia* aMedia, TUid aCollectionId )
    {
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );

    cmd->SetTObjectValueL( KMPXCommandGeneralId, KMPXCommandIdCollectionRemoveMedia );
    cmd->SetTObjectValueL( KMPXCommandGeneralDoSync, ETrue );
    cmd->SetTObjectValueL( KMPXCommandCollectionRemoveMediaDeleteRecord, ETrue );
    cmd->SetTObjectValueL( KMPXCommandGeneralCollectionId, aCollectionId.iUid );
    cmd->SetCObjectValueL<CMPXMedia>( KMPXCommandCollectionRemoveMedia, aMedia );

    iCollectionUtil->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );
    }

// ---------------------------------------------------------------------------
// Check DB logical synchronization
// ---------------------------------------------------------------------------
//
TBool CMPXHarvesterFileHandlerImp::CheckDbInSyncL()
    {
    ASSERT(iCollectionUtil != NULL);
    TBool needsync(EFalse);
    RArray<TInt> dbDrives;

    TInt dbCount(iDBManager->Count());
    for(TInt i=0; i < dbCount; ++i)
        {
        TInt harv_count = 0; //records count from harvester database
        TInt coll_count = 0; //records count from collection database

        CMPXHarvesterDB& db = iDBManager->GetDatabaseL(i);
        TDriveNumber drive = db.GetDbDrive();
        harv_count = db.CountAllFilesL();

        //get count from music db
        coll_count = GetTrackCountL(drive,iMusicCollectionId.iUid,EMPXCollectionCountTotal);

        if ( !iDisablePodcasting )
            {
            //get count from podcast db
            coll_count += GetTrackCountL(drive,iPodcastCollectionId.iUid,EMPXCollectionCountTrack);
            }

        if(harv_count != coll_count)
            {
            needsync = ETrue;
            dbDrives.Append(drive);
            }
        }

    if(needsync)
        {
        iDbSynchronizer->Synchronize(dbDrives,iCollectionUtil);
        }
    dbDrives.Close();

    return needsync;
    }

// ---------------------------------------------------------------------------
// Idle callback from CPerioidic
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterFileHandlerImp::Callback( TAny* aPtr )
    {
    TBool rtn(EFalse);
    TRAP_IGNORE( rtn = ((CMPXHarvesterFileHandlerImp*)aPtr)->DoAutoScanL() ); //lint !e665
    return rtn; // done
    }

// ---------------------------------------------------------------------------
// Perform the automatic scan event
// ---------------------------------------------------------------------------
//
TBool CMPXHarvesterFileHandlerImp::DoAutoScanL()
    {
    TBool again(EFalse);
    // Make sure to create a collection utility instance if it has not been
    //
    if( !iCollectionUtil )
        {
        iCollectionUtil = MMPXCollectionUtility::NewL( NULL, KMusicPlayerUid );
        }

    if( !iRefreshing && !iSynchronizing )
        {
        // Add a path to scan
        iRefreshCount++;
        CancelScan();
        Reset();
        // ensure you don't try to scan paths that are on closed drives
        for (TInt i = 0; i < iAutoScanPaths.Count(); )
            {
            TParsePtr parse(iAutoScanPaths[i]);
            TDriveUnit drive(parse.Drive());
            if ( iDBManager->DatabaseIsOpen( (TDriveNumber) (TInt) drive ) )
                {
                ++i;
                }
            else
                {
                iAutoScanPaths.Remove(i);
                }
            }
        if ( iAutoScanPaths.Count() )
            {
            iFolderScanner->ScanL( iAutoScanPaths );
            }

        // Cleanup
        iAutoScanPaths.Reset();
        iIdle->Cancel();
        }
    else
        {
        again = ETrue;
        }

    return again;
    }

// ---------------------------------------------------------------------------
// Get track count for given table in the db
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterFileHandlerImp::GetTrackCountL(TDriveNumber aDrive,TInt aColDbId, TInt aColTable)
    {
    TInt count(0);

    //get count from music db
    CMPXCommand* cmdCountM = CMPXMedia::NewL();
    CleanupStack::PushL(cmdCountM);
    cmdCountM->SetTObjectValueL<TMPXCommandId>(KMPXCommandGeneralId, KMPXCommandCollectionGetCount);
    cmdCountM->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmdCountM->SetTObjectValueL<TInt>(KMPXCommandGeneralCollectionId, aColDbId);
    cmdCountM->SetTObjectValueL<TInt>(KMPXCommandCollectionCountDrive, aDrive);
    cmdCountM->SetTObjectValueL<TInt>(KMPXCommandCollectionCountTable, aColTable);

    iCollectionUtil->Collection().CommandL(*cmdCountM);

    // returned command should contain count
    if (!cmdCountM->IsSupported(KMPXCommandCollectionCountValue))
        {
        User::Leave(KErrAbort);
        }

    count = cmdCountM->ValueTObjectL<TInt>(KMPXCommandCollectionCountValue);
    CleanupStack::PopAndDestroy(cmdCountM);

    return count;
    }

// ---------------------------------------------------------------------------
// Event callback from MDRMEventObserver
// ---------------------------------------------------------------------------
//
void CMPXHarvesterFileHandlerImp::HandleEventL( MDRMEvent* aEvent )
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::HandleEventL <---");
    CDRMEventAddRemove *event  = reinterpret_cast<CDRMEventAddRemove*>(aEvent);
 
    if( event->Status() == ERightsObjectRecieved ) 
        {
        HBufC8 *url = event->GetContentIDL();
        CleanupStack::PushL(url);
        // Convert 8 bit data to 16 bit.
        TBufC<ContentAccess::KMaxCafUniqueId> rightsCid;
        TPtr cidPtr( rightsCid.Des() );
        cidPtr.Copy( url->Des() );
        
        // get list of files with DRM flag set
        RPointerArray<CMPXHarvesterDbItem>* list = GetDrmFilesL();
        CleanupStack::PushL( list );

        // match incoming rights to the correct file
        TBool found = EFalse;
        for ( TInt i=0; i<list->Count(); i++ )
            {
            CMPXHarvesterDbItem* item = (*list)[i];
            CContent* content = CContent::NewL( item->iFile->Des() );
            CleanupStack::PushL( content );
            CData* data = content->OpenContentL( EPeek );
            CleanupStack::PushL( data );
            
            // Get content ID from file
            TInt err = KErrNone;
            TPtr cid( NULL, 0 );
            HBufC* uniqueId( HBufC::NewLC( ContentAccess::KMaxCafUniqueId ) );
            cid.Set( uniqueId->Des() );
            err = data->GetStringAttribute( ContentAccess::EContentID, cid );
            if ( (err == KErrNone ) && (cid.Compare( cidPtr ) == 0) )
                {
                // Add file for metadata scanning
                iMetadataScanner->AddModifiedFileToScanL( item->iFile->Des() );
                found = ETrue;
                }
            CleanupStack::PopAndDestroy( 3 );  // uniqueId, data, content
            }
        
        // Start metadata scanning if match found
        if ( found )
            {
            if( !iCollectionUtil )
                {
                iCollectionUtil = MMPXCollectionUtility::NewL( NULL, KMusicPlayerUid );
                }
            iMetadataScanner->Start();
            }
        list->ResetAndDestroy();
        CleanupStack::PopAndDestroy( list );
        CleanupStack::PopAndDestroy( url );
        }
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::HandleEventL --->");
    }

// ---------------------------------------------------------------------------
// Get a list of files with DRM flag set from the Harvester DB.
// ---------------------------------------------------------------------------
//
RPointerArray<CMPXHarvesterDbItem>* CMPXHarvesterFileHandlerImp::GetDrmFilesL()
    {
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::GetDrmFiles <---");
    CMPXHarvesterDB*             db=NULL;     // Current db reference
    CMPXHarvesterDatabaseTable*  table=NULL;  // owned, must delete
    RPointerArray<CMPXHarvesterDbItem>* tempList=NULL;
    RPointerArray<CMPXHarvesterDbItem>* drmFileList = new (ELeave) RPointerArray<CMPXHarvesterDbItem>;
    
    CleanupStack::PushL( drmFileList );

    TDriveList driveList;
    TInt driveCount(0);

    // Get all visible drives
    if ( KErrNone == DriveInfo::GetUserVisibleDrives(
    		iFs, driveList, driveCount ) )
    	{
        for( TInt driveNum = EDriveA; driveNum <= EDriveZ; driveNum++ )
            {
            if ( driveList[driveNum] )
                {
            	TDriveNumber drive = (TDriveNumber)driveNum;
                // Get DRM files from database
                db = &iDBManager->GetDatabaseL( drive );
                table = db->OpenDrmFileL(); 
                CleanupStack::PushL( table );
                tempList = table->CreateTableRepresentationL();
                CleanupStack::PushL( tempList );
                // copy content to drm file list
                for ( TInt i=0; i<tempList->Count(); i++ )
                    {
                    drmFileList->AppendL( (*tempList)[i] );
                    }
                // reset
                tempList->Reset();
                CleanupStack::PopAndDestroy( tempList );
                CleanupStack::PopAndDestroy( table );
                }
            }
    	}
    CleanupStack::Pop( drmFileList );
    MPX_DEBUG1("CMPXHarvesterFileHandlerImp::GetDrmFiles --->");
    return drmFileList;
    }

// END OF FILE
