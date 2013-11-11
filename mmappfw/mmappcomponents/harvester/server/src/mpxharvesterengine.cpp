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
* Description:  Implementation file for the Harvester Engine
*
*/


#include <e32std.h>
#include <driveinfo.h>
#include <mpxlog.h>
#include <mpxmedia.h>
#include <mpxcollectionutility.h>
#include <mpxcollectionmessage.h>
#include <mpxtaskqueue.h>
#include <mpxplaylistengine.h>
#include <mpxplaylistpluginhandler.h>
#include <mpxcmn.h>
#include <mpxmediageneraldefs.h>
#include <mpxplaybackutility.h>
#include <mpxcommandgeneraldefs.h>

// Harvester includes
#include "mpxharvestercommon.h"
#include "mpxharvesterengine.h"
#include "mpxfsformatmonitor.h"
#include "mpxmediaremovalmonitor.h"
#include "mpxconnectioneventhandler.h"
#include "mpxharvesterfilehandler.h"
#include "mpxharvesterengineobserver.h"
#include "mpxhvsmsg.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterEngine::CMPXHarvesterEngine() : iDiskOpActive( EFalse )
    {
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CMPXHarvesterEngine::~CMPXHarvesterEngine()
    {
    MPX_DEBUG1("CMPXHarvesterEngine::~CMPXHarvesterEngine <---");

    delete iFormatMonitor;
    delete iMediaRemovalMonitor;
    delete iConnectionMonitor;

    delete iFileHandler;
    iFsSession.Close();

    if( iTaskQueue )
        {
        iTaskQueue->Reset();
        }
    delete iTaskQueue;
    delete iPlaylistEngine;

    if( iTempCollectionUtil )
        {
        iTempCollectionUtil->Close();
        }
    }

// ---------------------------------------------------------------------------
// 2nd Phase Constructor`
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::ConstructL()
    {
    MPX_DEBUG1("CMPXHarvesterEngine::ConstructL <---");

    User::LeaveIfError(iFsSession.Connect());

    iFsSession.ShareProtected();

    // Format handler for Removable Drive
    iFormatMonitor = CMPXFsFormatMonitor::NewL( *this );

    // MMC Removal monitor for Removable Drive
    TInt removableDrive( EDriveE );
    User::LeaveIfError( DriveInfo::GetDefaultDrive(
        DriveInfo::EDefaultRemovableMassStorage,
        removableDrive ) );
    iMediaRemovalMonitor = CMPXMediaRemovalMonitor::NewL(
        removableDrive, iFsSession, *this );

    // Connection Event monitor
    iConnectionMonitor = CMPXConnectionEventHandler::NewL( *this );


    // File handler to handle file related events
    iFileHandler = CMPXHarvesterFileHandler::NewL( iFsSession );

    // Active task queue for async requests
    iTaskQueue = CMPXActiveTaskQueue::NewL();

    // Playlist Engine for export/import playlist
    iPlaylistEngine = CMPXPlaylistEngine::NewL( *this );

    MPX_DEBUG1("CMPXHarvesterEngine::ConstructL --->");
    }

// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterEngine* CMPXHarvesterEngine::NewL()
    {
    CMPXHarvesterEngine* self = new(ELeave) CMPXHarvesterEngine();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Start scanning all drives
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::ScanL()
    {
    iFileHandler->ScanL();
    }

// ---------------------------------------------------------------------------
// Cancel scanning
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::CancelScan()
    {
    iFileHandler->CancelScan();
    }

// ---------------------------------------------------------------------------
// Add a file to the db by file name
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::AddFileL( RHvsMsg* aMsg, const TDesC& aFile,
                                    MMPXHarvesterEngineObserver* aObs )
    {
    // If it is a playlist, get playlist engine to extract,
    // if it is a normal media file, we get file handler to extract
    //
    if( iPlaylistEngine->IsPlaylistL( aFile ) )
        {
        iTaskQueue->AddTaskL( aMsg->Op(), aObs, this, 0, NULL, aMsg);
        }
    else
        {
        CMPXMedia* media(NULL);
        TRAPD( err,
               media = iFileHandler->AddFileL( aFile )
             );
        CleanupStack::PushL( media );
        aObs->HandleImportCompleteL( media, err, aMsg );
        CleanupStack::PopAndDestroy( media );
        }
    }

// ---------------------------------------------------------------------------
// Add a file to the db (with media properties)
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterEngine::AddFileL( CMPXMedia*& aMedia )
    {
    return iFileHandler->AddFileL( *aMedia );
    }

// ---------------------------------------------------------------------------
// Create a media object for the file
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::GetMediaL( RHvsMsg* aMsg, const TDesC& aFile,
                                    MMPXHarvesterEngineObserver* aObs )
    {
    // If it is a playlist, get playlist engine to extract,
    // if it is a normal media file, we get file handler to extract
    //
    if( iPlaylistEngine->IsPlaylistL( aFile ) )
        {
        iTaskQueue->AddTaskL( aMsg->Op(), aObs, this, 0, NULL, aMsg);
        }
    else
        {
        CMPXMedia* media(NULL);
        TRAPD(err, media = iFileHandler->GetMediaForFileL(aFile));
        CleanupStack::PushL( media );
        aObs->HandleImportCompleteL( media, err, aMsg );
        CleanupStack::PopAndDestroy( media );
        }
    }

// ---------------------------------------------------------------------------
// Get collection id for the file
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterEngine::GetColUidL( const TDesC& aFile )
    {
    return iFileHandler->GetColUidForFileL(aFile);
    }

// ---------------------------------------------------------------------------
// Remove a single file
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterEngine::RemoveFileL( const TDesC& aFile, TBool aDelete, TBool aEndTransaction )
    {
    // Delete this file? If so, delete it from the file system, if a problem
    // occurs deleting this file, the correlating entry in the harvester db
    // is not removed.
    //
    if( aDelete )
        {
		MPX_PERF_START( MPX_PERF_FS_DELETE );
        TInt err = iFsSession.Delete( aFile );
        MPX_PERF_END( MPX_PERF_FS_DELETE );
        if (err == KErrAccessDenied)
            {
            iFsSession.SetAtt(aFile,KEntryAttNormal,KEntryAttReadOnly);
            err = iFsSession.Delete(aFile);
            }
        User::LeaveIfError( err );
        }

	MPX_PERF_START( MPX_PERF_HARV_DB_DELETE );
    TInt r = iFileHandler->RemoveFileL( aFile, aEndTransaction );
    MPX_PERF_END( MPX_PERF_HARV_DB_DELETE );
    return r;
    }

// ---------------------------------------------------------------------------
// Remove Multiple files
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::RemoveMultipleFilesL( const MDesCArray& aArray,
                                                TBool aDelete, TBool aEndTransaction)
    {
    //
    // delete the file from the file system before updating harvester database.
    // delete one file from the file system and update that file in the harvester
    // database one by one; otherwise, file system and database will be out of
    // sync
    //
    if( aDelete )
        {
        TInt c = aArray.MdcaCount();
        for( TInt i=0; i<c; ++i )
            {
            User::LeaveIfError(RemoveFileL(aArray.MdcaPoint(i), aDelete, aEndTransaction));
            }
        }
    else
        {
        iFileHandler->RemoveFilesL( aArray );
        }
    }

// ---------------------------------------------------------------------------
// Remove all files
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::RemoveAllFilesL()
    {
    iFileHandler->RemoveAllFilesL();
    }

// ---------------------------------------------------------------------------
// Update a particular file
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::UpdateFileL( const TDesC& aFile, TInt aCollection )
    {
    MPX_DEBUG1("CMPXHarvesterEngine::UpdateFileL <---");
    iFileHandler->UpdateFileL( aFile, aCollection );
    }

// ---------------------------------------------------------------------------
// Update a particular file
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::RenameFileL( const TDesC& aOldPath,
                                       const TDesC& aNewPath,
                                       TInt aCollection )
    {
    MPX_DEBUG1("CMPXHarvesterEngine::RenameFileL <---");
    iFileHandler->RenameFileL( aOldPath, aNewPath, aCollection );
    }

// ---------------------------------------------------------------------------
// Export a playlist, asynchronous
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::ExportPlaylistL(
                                            RHvsMsg* aMessage,
                                            MMPXHarvesterEngineObserver* aObs )
    {
    MPX_DEBUG1("CMPXHarvesterEngine::HandleExportPlaylistL <---");
    iTaskQueue->AddTaskL( aMessage->Op(), aObs, this, 0, NULL, aMessage);
    }

// ---------------------------------------------------------------------------
// Find the collection id
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterEngine::FindCollectionIdL( const TDesC& aFile )
    {
    MPX_DEBUG1("CMPXHarvesterEngine::FindCollectionIDL <---");
    return iFileHandler->FindCollectionIdL( aFile );
    }

// ---------------------------------------------------------------------------
// Import a playlist
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::ImportPlaylistL(
                                            RHvsMsg* aMessage,
                                            MMPXHarvesterEngineObserver* aObs )
    {
    MPX_DEBUG1("CMPXHarvesterEngine::ImportPlaylistL <---");
    iTaskQueue->AddTaskL( aMessage->Op(), aObs, this, 0, NULL, aMessage);
    }

// ---------------------------------------------------------------------------
// Import a file
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::ImportFileL( RHvsMsg* aMessage,
                                       MMPXHarvesterEngineObserver* aObs )
    {
    MPX_DEBUG1("CMPXHarvesterEngine::ImportFileL <---");
    iTaskQueue->AddTaskL( aMessage->Op(), aObs, this, 0, NULL, aMessage);
    }

// ---------------------------------------------------------------------------
// Query the required attributes for tracks for the specified playlist type
// ---------------------------------------------------------------------------
const TArray<TMPXAttribute> CMPXHarvesterEngine::RequiredAttributesL(TInt aPlaylistType) const
    {
    iPlaylistEngine->PlaylistPluginHandler().SelectPlaylistPluginL(aPlaylistType);

    CMPXPlaylistPlugin* playlistPlugin = iPlaylistEngine->PlaylistPluginHandler().Plugin();
    if (!playlistPlugin)
        {
        User::Leave(KErrNotSupported);
        }
    return playlistPlugin->RequiredAttributes();
    }

// ---------------------------------------------------------------------------
// Query the optional attributes for tracks for the specified playlist type
// ---------------------------------------------------------------------------
const TArray<TMPXAttribute> CMPXHarvesterEngine::OptionalAttributesL(TInt aPlaylistType) const
    {
    iPlaylistEngine->PlaylistPluginHandler().SelectPlaylistPluginL(aPlaylistType);

    CMPXPlaylistPlugin* playlistPlugin = iPlaylistEngine->PlaylistPluginHandler().Plugin();
    if (!playlistPlugin)
        {
        User::Leave(KErrNotSupported);
        }
    return playlistPlugin->OptionalAttributes();
    }

// ---------------------------------------------------------------------------
// Query the optional attributes for tracks for the specified playlist type
// ---------------------------------------------------------------------------
const TDesC& CMPXHarvesterEngine::PlaylistFileExtensionL(TInt aPlaylistType) const
    {
    iPlaylistEngine->PlaylistPluginHandler().SelectPlaylistPluginL(aPlaylistType);

    CMPXPlaylistPlugin* playlistPlugin = iPlaylistEngine->PlaylistPluginHandler().Plugin();
    if (!playlistPlugin)
        {
        User::Leave(KErrNotSupported);
        }
    return playlistPlugin->FileExtension();
    }

// ---------------------------------------------------------------------------
// Determines if the given file is a playlist
// ---------------------------------------------------------------------------
//
TBool CMPXHarvesterEngine::IsPlaylistL( const TDesC& aFile )
    {
    MPX_DEBUG1("CMPXHarvesterEngine::IsPlaylistL <---");
    return iPlaylistEngine->IsPlaylistL( aFile );
    }

// ---------------------------------------------------------------------------
// Determines if the given file is a playlist
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::RecreateDatabases()
    {
    MPX_DEBUG1("CMPXHarvesterEngine::RecreateDatabasesL");
    iFileHandler->RecreateDatabases();
    }

// ---------------------------------------------------------------------------
// Checks for any system events
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::CheckForSystemEvents()
    {
    MPX_DEBUG1("CMPXHarvesterEngine::CheckForSystemEventsL()");

    // Check for formatting, mtp and usb
    // No need to check for disk inserted or not because we
    // wont be able to open the db anyways.
    //
    iFormatMonitor->PollStatus();
    iConnectionMonitor->PollStatus();
    }

// ---------------------------------------------------------------------------
// Handling system events
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::HandleSystemEventL( TSystemEvent aEvent,
                                              TInt aData )
    {
    MPX_DEBUG2("CMPXHarvesterEngine::HandleSystemEventL %i <---", aEvent);
    
    if( !iTempCollectionUtil )
        {
        iTempCollectionUtil = MMPXCollectionUtility::NewL( NULL, KMcModeDefault );
        }
        
    // Must close collections ASAP in case drives may dismount soon       
    TRAP_IGNORE( 
        if (aEvent == EUSBMassStorageStartEvent)
            {
            DoStopPlaybackL();
            iTempCollectionUtil->Collection().CommandL ( EMcCloseCollection, -1 ); 
            iFileHandler->HandleSystemEventL ( EDiskDismountEvent, -1 );
            }
        else if ( aEvent == EDiskDismountEvent )
            {
            DoStopPlaybackL();
            iTempCollectionUtil->Collection().CommandL ( EMcCloseCollection, aData );
            }
        );

    // The engine is a delegator, it sends the events to
    // different classes to do the actual work
    TRAPD( openErr, iFileHandler->HandleSystemEventL( aEvent, aData ) );

    // Avoid Message queue already exist problem
    //
    TBool notify(ETrue);
    switch( aEvent )
        {
        case EDiskDismountEvent:
            {
            notify=EFalse;
            break;
            }
        case EUSBMassStorageStartEvent:
            {
            iDiskOpActive = ETrue;
            }
            break;
        case EFormatStartEvent:
        case EUSBMTPStartEvent:           // deliberate fall through
        case EDiskInsertedEvent:          // deliberate fall through
        case EDiskRemovedEvent:           // deliberate fall through
            {
            iDiskOpActive = ETrue;
            TRAP_IGNORE( DoStopPlaybackL() );
            }
            break;
        }

    // Send a message to the collection server about the event
    //
    if( notify )
        {
        TRAP_IGNORE(
               iTempCollectionUtil->Collection().NotifyL( (TMPXCollectionBroadCastMsg)aEvent,
                                                          aData )
             );
        }

    // Avoid Message queue already exist problem
    //
    switch( aEvent )
        {
        case EFormatEndEvent:
            {
            // On some, the formatting app remounts
            // the disk before it is writable, so
            // we have to "fake" the insert event back after the
            // format is actually done
            //
            if( openErr == KErrNone )
                {
                HandleSystemEventL( EDiskInsertedEvent, aData );
                }
            }                            // deliberate fall through
        case EUSBMassStorageEndEvent:    // deliberate fall through
        case EUSBMTPEndEvent:            // deliberate fall through
            iDiskOpActive = EFalse;
        default: //lint !e616 !e825
            if( iTempCollectionUtil && !iDiskOpActive )
                {
                iTempCollectionUtil->Close();
                iTempCollectionUtil = NULL;
                }
        }
        
    if ( aEvent == EUSBMassStorageEndEvent )
        {
        // In some cases visit to USB Mass Storage mode can be so brief
        // that drives are actually not dismounted at all even though we
        // get a NotifyDismount event. In such cases we need to re-issue
        // the NotifyDismount requests.
        iMediaRemovalMonitor->CheckDriveStatus();
        }
    }

// ---------------------------------------------------------------------------
// Execute task queue events
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::ExecuteTask(TInt aTask,
                                      TInt /*aParamData*/,
                                      TAny* aPtrData,
                                      const CBufBase& /*aBuf*/,
                                      TAny* aCallback,
                                      CBase* /*aCObject1*/,
                                      CBase* /*aCObject2*/)
    {
    MPX_DEBUG1("CMPXHarvesterEngine::ExecuteTask <---");

    // Execute the next task in the queue
    //
    //
    TInt err( KErrNone );
    switch( aTask )
        {
        case EHvsServerGetMedia:
        case EHvsServerAddFile:
        case EHvsServerImportPlaylist:  // deliberate fall through
            TRAP( err, DoImportPlaylistL( (RHvsMsg*) aPtrData ) );
            break;

        case EHvsServerExportPlaylist:
            TRAP( err, DoExportPlaylistL( (RHvsMsg*) aPtrData ) );
            break;
        case EHvsServerImportFile:
            TRAP( err, DoImportFileL( (RHvsMsg*) aPtrData ) );
            break;
        default:
        break;
        }

    // If we failed, we complete with error.
    //
    if( err != KErrNone )
        {
        MPX_DEBUG2("CMPXHarvesterEngine::ExecuteTask err %i", err );
        MMPXHarvesterEngineObserver* obs =
                                     (MMPXHarvesterEngineObserver*) aCallback;
        obs->CompleteTask( (RHvsMsg*) aPtrData, err );
        iTaskQueue->CompleteTask();
        }
    }

// ----------------------------------------------------------------------------
// Indicates that a task was terminated with an error
// ----------------------------------------------------------------------------
//
void CMPXHarvesterEngine::HandleTaskError(
    TInt /* aTask */,
    TAny* /*aPtrData*/,
    TAny* /*aCallback*/,
    TInt /* aError */)
    {
    // Do Nothing
    }

// ---------------------------------------------------------------------------
// Handle the completion of importing a playlist
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::HandlePlaylistL( CMPXMedia* aPlaylist,
                                           const TInt aError,
                                           const TBool /*aCompleted*/ )
    {
    MPX_DEBUG2("CMPXHarvesterEngine::HandlePlaylistL err %i", aError );
    TInt err( aError );
    if( iCurAsyncOp != EHvsServerImportPlaylist && iCurAsyncOp != EHvsServerAddFile &&
        iCurAsyncOp != EHvsServerImportFile  && iCurAsyncOp != EHvsServerGetMedia)
        {
        err = KErrArgument;
        }

    if( aPlaylist )
        {
        CleanupStack::PushL( aPlaylist );
        }

    // If we were adding a file, we need to add it to file handler as well
    if( iCurAsyncOp == EHvsServerAddFile && aPlaylist )
        {
        iFileHandler->AddFileL( *aPlaylist );
        }

    // Complete the message and tell the client
    //
    MMPXHarvesterEngineObserver* obs = (MMPXHarvesterEngineObserver*)
                                                       iTaskQueue->Callback();
    RHvsMsg* msg = (RHvsMsg*) iTaskQueue->PtrData();
    if( err == KErrNone )
        {
        obs->HandleImportCompleteL( aPlaylist, err, msg );
        }
    else
        {
        obs->HandleImportCompleteL( NULL, err, msg );
        }
    iTaskQueue->CompleteTask();

    if( aPlaylist )
        {
        CleanupStack::PopAndDestroy( aPlaylist );
        }
    }

// ---------------------------------------------------------------------------
// Handle the completion of exporting a playlist
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::HandlePlaylistL(const TDesC& aPlaylistUri,
                                          const TInt aError)
    {
    MPX_DEBUG2("CMPXHarvesterEngine::HandlePlaylistL err %i", aError );

    TInt err( aError );
    if( iCurAsyncOp != EHvsServerExportPlaylist )
        {
        err = KErrArgument;
        }

    // Complete the message and tell the client
    //
    MMPXHarvesterEngineObserver* obs = (MMPXHarvesterEngineObserver*)
                                                       iTaskQueue->Callback();
    RHvsMsg* msg = (RHvsMsg*) iTaskQueue->PtrData();
    if ( err == KErrNone )
        {
        CMPXMedia* media(NULL);
        ::NewFromMessageL<CMPXMedia>( msg->Message(), 0, media );
        CleanupStack::PushL( media );

        // set playlist uri
        media->SetTextValueL(
            TMPXAttribute(KMPXMediaIdGeneral, EMPXMediaGeneralUri),
            aPlaylistUri);

        obs->HandleExportCompleteL( media, err, msg );
        CleanupStack::PopAndDestroy( media );
        }
    else
        {
        obs->HandleExportCompleteL( NULL, err, msg );
        }
    iTaskQueue->CompleteTask();
    }

// ---------------------------------------------------------------------------
// Does the actual work for exporting a playlist
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::DoExportPlaylistL( RHvsMsg* aMsg )
    {
    MPX_DEBUG1("CMPXHarvesterEngine::DoExportPlaylistL  <---");
    iCurAsyncOp = EHvsServerExportPlaylist;

    // Unpack the message
    //
    CMPXMedia* media(NULL);
    ::NewFromMessageL<CMPXMedia>( aMsg->Message(), 0, media );
    CleanupStack::PushL( media );

    HBufC* path = HBufC::NewLC( aMsg->Message().GetDesLength(1) );
    TPtr ptr = path->Des();
    aMsg->Message().Read( 1, ptr );

    TPckgBuf<TInt> t;
    aMsg->Message().Read(2, t);
    TInt plType = t();

    // Find the playlist plugin
    CMPXPlaylistPluginHandler& plHandler =
                                      iPlaylistEngine->PlaylistPluginHandler();
    plHandler.SelectPlaylistPluginL( plType );

    // Call playlist engine
    //
    iPlaylistEngine->ExternalizePlaylistL( *media, *path );

    // Cleanup
    CleanupStack::PopAndDestroy( 2, media );
    MPX_DEBUG1("CMPXHarvesterEngine::DoExportPlaylistL  --->");
    }

// ---------------------------------------------------------------------------
// Does the actual work for importing a playlist
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::DoImportPlaylistL( RHvsMsg* aMsg )
    {
    MPX_DEBUG1("CMPXHarvesterEngine::DoImportPlaylistL  <---");
    iCurAsyncOp = aMsg->Op();

    // Unpack the message
    //
    HBufC* filename = HBufC::NewLC( aMsg->Message().GetDesLength(0) );
    TPtr ptr = filename->Des();
    aMsg->Message().Read(0, ptr );

    // Call playlist engine
    //
    iPlaylistEngine->InternalizePlaylistL( *filename );

    // Cleanup
    CleanupStack::PopAndDestroy( filename );
    MPX_DEBUG1("CMPXHarvesterEngine::DoImportPlaylistL  --->");
    }

// ---------------------------------------------------------------------------
// Does the actual work for importing a file
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::DoImportFileL( RHvsMsg* aMsg )
    {
    MPX_DEBUG1("CMPXHarvesterEngine::DoImportFileL  <---");
    iCurAsyncOp = aMsg->Op();

    // Unpack the message
    //
    HBufC* filename = HBufC::NewLC( aMsg->Message().GetDesLength(0) );
    TPtr ptr = filename->Des();
    aMsg->Message().Read(0, ptr );

    if( iPlaylistEngine->IsPlaylistL( *filename ) )
        {
        // Call playlist engine
        iPlaylistEngine->InternalizePlaylistL( *filename );
        }
    else
        {
        // Import file details
        CMPXMedia* media = CMPXMedia::NewL();
        CleanupStack::PushL( media );
        media->SetTObjectValueL( TMPXAttribute(KMPXMediaIdGeneral,
                                               EMPXMediaGeneralType),
                                 EMPXItem );
        media->SetTObjectValueL( TMPXAttribute(KMPXMediaIdGeneral,
                                               EMPXMediaGeneralCategory),
                                 EMPXSong );

        media->SetTextValueL( TMPXAttribute(KMPXMediaIdGeneral,
                                               EMPXMediaGeneralUri),
                              *filename );

        // Callback to observer
        MMPXHarvesterEngineObserver* obs = (MMPXHarvesterEngineObserver*)
                                            iTaskQueue->Callback();
        obs->HandleImportCompleteL( media, KErrNone, aMsg );
        CleanupStack::PopAndDestroy( media );

        // Task is completed for file case
        iTaskQueue->CompleteTask();
        }

    // Cleanup
    CleanupStack::PopAndDestroy( filename );
    MPX_DEBUG1("CMPXHarvesterEngine::DoImportFileL  --->");
    }

// ---------------------------------------------------------------------------
// Stop playback
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::DoStopPlaybackL()
    {
    MMPXPlaybackUtility* pbUtil = MMPXPlaybackUtility::UtilityL( KPbModeActivePlayer );
    CleanupClosePushL( *pbUtil );

    CMPXCommand* closeCmd = CMPXCommand::NewL();
    CleanupStack::PushL( closeCmd );

    // Send a stop command
    closeCmd->SetTObjectValueL(KMPXCommandGeneralId, KMPXCommandIdPlaybackGeneral);
    closeCmd->SetTObjectValueL(KMPXCommandGeneralDoSync, ETrue);
    closeCmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralType,EPbCmdStop);
    closeCmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralNoBuffer, ETrue);
    pbUtil->CommandL( *closeCmd );

    // Send a clear buffer command
    closeCmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralType,EPbCmdClearKeyBuffer);
    pbUtil->CommandL( *closeCmd );
    CleanupStack::PopAndDestroy( closeCmd );

    CleanupStack::Pop( pbUtil );
    pbUtil->Close();
    }

// ---------------------------------------------------------------------------
// Close the database transaction
// ---------------------------------------------------------------------------
//
void CMPXHarvesterEngine::CloseTransactionL()
    {
    iFileHandler->CloseTransactionL();
    }

// END OF FILE

