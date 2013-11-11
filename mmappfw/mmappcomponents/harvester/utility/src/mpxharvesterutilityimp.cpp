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
* Description:  Harvester utility implementation
*
*/


#include <e32base.h>
#include <f32file.h>
#include <mpxharvesterserverdefs.h>
#include <mpxcmn.h>

#include <mpxmedia.h>
#include <mpxmediageneraldefs.h>
#include <mpxcollectionpath.h>
#include <mpxlog.h>
#include "mpxharvesterutilityobserver.h"
#include "mpxharvesterutilityimp.h"


const TInt KMaxPlaylistExtensionLength = 10;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Private Constructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterUtilityImp::CMPXHarvesterUtilityImp() : CActive( EPriorityNormal ),
                                               iCurAsyncOp( EHvsServerOpCount )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// 2nd-Phased Constructor
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::ConstructL()
    {
    TInt err = iSession.Connect(KMPXHarvesterServerName,
                                KMPXHarvesterServerImg,
                                TVersion(KMPXHarvesterServerMajorVersionNumber,
                                         KMPXHarvesterServerMinorVersionNumber,
                                         KMPXHarvesterServerBuildVersionNumber));
    User::LeaveIfError( err );
    }

// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterUtilityImp* CMPXHarvesterUtilityImp::NewL()
    {
    CMPXHarvesterUtilityImp* self = new( ELeave ) CMPXHarvesterUtilityImp();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Virtual Destructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterUtilityImp::~CMPXHarvesterUtilityImp()
    {
    Cancel();

    delete iBuffer;
    iSession.Close();
    delete iCurTask;
	delete iFile;
    }

// ---------------------------------------------------------------------------
// Scan Function
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::ScanL()
    {
    iSession.SendReceiveL( EHvsServerScan );
    }

// ---------------------------------------------------------------------------
// Cancel Scan Function
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::CancelScanL()
    {
    iSession.SendReceiveL( EHvsServerCancelScan );
    }

// ---------------------------------------------------------------------------
// Shut down the server function
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::ShutdownL()
    {
    iSession.SendReceiveL( EHvsServerShutdown );
    }

// ---------------------------------------------------------------------------
// Add a song to the harvester
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::AddFileL( const TDesC& aFilePath,
                                        MMPXHarvesterUtilityObserver* aObs )
    {
    ASSERT( !iCurTask );
    HBufC* path = aFilePath.AllocLC();
    iCurTask = new(ELeave) CHvsUtilityTask( NULL, path,
                                            NULL, NULL );
    CleanupStack::Pop( path );
    iSession.SendReceive( EHvsServerAddFile, TIpcArgs( &iCurTask->Arg2() ), iStatus );

    // Set Async op
    iCurAsyncOp = EHvsServerAddFile;
    iObserver = aObs;
    SetActive();
    }

// ---------------------------------------------------------------------------
// Add a song to the harvester
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterUtilityImp::AddFileL( CMPXMedia*& aProperty )
    {
    CBufBase* buf( NULL );
    CreateBufferL<CMPXMedia>( *aProperty, buf );
    CleanupStack::PushL( buf );
    TPtr8 p = buf->Ptr(0);
    TInt r = iSession.SendReceiveL( EHvsServerAddFileMedia, TIpcArgs( &p ) );
    CleanupStack::PopAndDestroy( buf );
    return r;
    }

// ---------------------------------------------------------------------------
// Remove a song from the harvester
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterUtilityImp::RemoveFileL( const TDesC& aFilePath )
    {
    TPckgBuf<TInt> del(EFalse);
    return iSession.SendReceiveL( EHvsServerRemoveFile, TIpcArgs( &aFilePath,
                                                                  &del ) );
    }

// ---------------------------------------------------------------------------
// Remove multiple songs from the harvester
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::RemoveFilesL( const MDesCArray& aArray )
    {
    CBufBase* buf( NULL );
    MPXUser::CreateBufferL( &aArray, buf );
    CleanupStack::PushL( buf );
    // IPC args
    TPtr8 p=buf->Ptr(0);
    TPckgBuf<TInt> del(EFalse);
    iSession.SendReceiveL( EHvsServerRemoveFiles,
                           TIpcArgs( &p, &del ) );
    CleanupStack::PopAndDestroy( buf );
    }

// ---------------------------------------------------------------------------
// Remove all songs from the harvester
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::RemoveAllFilesL()
    {
    iSession.SendReceiveL( EHvsServerRemoveAllFiles );
    }

// ---------------------------------------------------------------------------
// Recreate the harvester database
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::RecreateDatabasesL()
    {
    iSession.SendReceiveL( EHvsServerReCreateDB );
    }

// ---------------------------------------------------------------------------
// Remove a song and delete it from the file system
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::DeleteFileL( const TDesC& aFilePath,
                                        MMPXHarvesterUtilityObserver* aObs  )
    {
    ASSERT( !iCurTask );

    HBufC* path = aFilePath.AllocLC();
    iCurTask = new(ELeave) CHvsUtilityTask( NULL, path,
                                            NULL, ETrue );
    CleanupStack::Pop( path );

    iSession.SendReceive( EHvsServerRemoveFile, TIpcArgs( &iCurTask->Arg2(),
                          &iCurTask->Arg4() ), iStatus );

    // Set Async op
    iCurAsyncOp = EHvsServerRemoveFile;
    iObserver = aObs;
    SetActive();
    }

// ---------------------------------------------------------------------------
// Remove and delete a list of files from the file system
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::DeleteFilesL( const MDesCArray& aArray,
                                         MMPXHarvesterUtilityObserver* aObs  )
    {
    ASSERT( !iCurTask );

    CBufBase* buf( NULL );
    MPXUser::CreateBufferL( &aArray, buf );
    CleanupStack::PushL( buf );
    iCurTask = new(ELeave) CHvsUtilityTask( buf, NULL,
                                            NULL, ETrue );
    CleanupStack::Pop( buf );

    iSession.SendReceive( EHvsServerRemoveFiles,
                          TIpcArgs( &iCurTask->Arg1(), &iCurTask->Arg4() ), iStatus );
    // Set Async op
    iCurAsyncOp = EHvsServerRemoveFiles;
    iObserver = aObs;
    SetActive();
    }

// ---------------------------------------------------------------------------
// Remove and delete a list of files from the file system
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::DeleteFilesL( const MDesCArray& aArray )
    {
    DeleteFilesL( aArray, ETrue );
    }

// ---------------------------------------------------------------------------
// Remove and delete a list of files from the file system
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::DeleteFilesL( const MDesCArray& aArray, TBool aEndTransaction )
    {
    CBufBase* buf( NULL );
    MPXUser::CreateBufferL( &aArray, buf );
    CleanupStack::PushL( buf );
    // IPC args
    TPtr8 p=buf->Ptr(0);
    TPckgBuf<TInt> del(ETrue);
    TPckgBuf<TInt> transaction(aEndTransaction);
    iSession.SendReceiveL( EHvsServerRemoveFiles,
                           TIpcArgs( &p, &del , &transaction ) );
    CleanupStack::PopAndDestroy( buf );
    }

// ---------------------------------------------------------------------------
// Close the database transaction
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::CloseTransactionL()
    {
    iSession.SendReceiveL( EHvsServerCloseTransaction );
    }

// ---------------------------------------------------------------------------
// Remove and delete a file from the file system
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::UpdateFileL( CMPXMedia*& aProperty )
    {
    TMPXAttribute att( KMPXMediaIdGeneral, EMPXMediaGeneralUri );
    const TDesC& filepath = aProperty->ValueText( att );

    att = TMPXAttribute( KMPXMediaIdGeneral, EMPXMediaGeneralCollectionId );
    const TUid& collection = aProperty->ValueTObjectL<TUid>( att );

    TPckgBuf<TInt>  arg2( collection.iUid );
    iSession.SendReceiveL( EHvsServerUpdateFile, TIpcArgs(&filepath, &arg2) );
    }

// ---------------------------------------------------------------------------
// Rename file for the media
// ---------------------------------------------------------------------------
//
HBufC* CMPXHarvesterUtilityImp::RenameFileLC( const CMPXMedia& aMedia )
    {
    TMPXGeneralCategory category =
         aMedia.ValueTObjectL<TMPXGeneralCategory>(
            TMPXAttribute(KMPXMediaIdGeneral,EMPXMediaGeneralCategory));
    if ( category != EMPXPlaylist )
        {
        User::Leave( KErrNotSupported );
        }

    const TDesC& title =
        aMedia.ValueText(
            TMPXAttribute( KMPXMediaIdGeneral, EMPXMediaGeneralTitle ) );

    const TDesC& oldPath =
        aMedia.ValueText(
            TMPXAttribute( KMPXMediaIdGeneral, EMPXMediaGeneralUri ) );

    // determine the new file path from the existing file path and the given title
    TParsePtrC parser( oldPath );
    HBufC* newPath = HBufC::NewLC( parser.DriveAndPath().Length() +
                                   title.Length() + parser.Ext().Length() );
    TPtr ptr = newPath->Des();
    ptr.Append( parser.DriveAndPath() );
    ptr.Append( title );
    ptr.Append( parser.Ext() );

    // get collection Id
    const TUid& collection =
         aMedia.ValueTObjectL<TUid>(TMPXAttribute(KMPXMediaIdGeneral,EMPXMediaGeneralCollectionId));

    RenameFileL(oldPath, *newPath, collection.iUid);

    return newPath;
    }

// ---------------------------------------------------------------------------
// Rename file
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::RenameFileL( const TDesC& aOldUri,
                                        const TDesC& aNewUri,
                                        TInt aCollection )
    {
    TPckgBuf<TInt> collectionId( aCollection );

    iSession.SendReceiveL( EHvsServerRenameFile, TIpcArgs(&aOldUri, &aNewUri, &collectionId) );
    }

// ---------------------------------------------------------------------------
// Find the collection ID for a file
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterUtilityImp::FindCollectionIdL( const TDesC& aFile )
    {
    return iSession.SendReceiveL( EHvsServerFindColID, TIpcArgs(&aFile) );
    }

// ---------------------------------------------------------------------------
// Query the required attributes for tracks for the specified playlist type
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::RequiredAttributesL(TInt aPlaylistType,
                                               RArray<TMPXAttribute>& aRequiredAttributes)
    {
    TPckgBuf<TInt> playlistType( aPlaylistType );

    TInt size = iSession.SendReceiveL( EHvsServerReqAttr, TIpcArgs( &playlistType) );

    // Transfer results from server
    // lint doesn't understand the template ::CreateBuffer
    //
    ::TransferBufferFromServerL( iSession, EHvsServerGetBuffer, size, iBuffer );
    ::CreateFromBufferL<TMPXAttribute>( *iBuffer, aRequiredAttributes ); //lint !e1024 !e1703 !e1514 !e64
    }

// ---------------------------------------------------------------------------
// Query the optional attributes for tracks for the specified playlist type
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::OptionalAttributesL(TInt aPlaylistType,
                                               RArray<TMPXAttribute>& aOptionalAttributes)
    {
    TPckgBuf<TInt> playlistType( aPlaylistType );

    TInt size = iSession.SendReceiveL( EHvsServerOptAttr, TIpcArgs( &playlistType) );

    // Transfer results from server
    //
    ::TransferBufferFromServerL( iSession, EHvsServerGetBuffer, size, iBuffer );
    ::CreateFromBufferL<TMPXAttribute>( *iBuffer, aOptionalAttributes ); //lint !e1024 !e1703 !e1514 !e64
    }

// ---------------------------------------------------------------------------
// Queries about the playlist file extension of the currently selected playlist
// plugin
// ---------------------------------------------------------------------------
HBufC* CMPXHarvesterUtilityImp::PlaylistFileExtensionLC(TInt aPlaylistType)
    {
    TPckgBuf<TInt> playlistType( aPlaylistType );

    HBufC* fileExtension = HBufC::NewLC(KMaxPlaylistExtensionLength);
    TPtr extension = fileExtension->Des();

    User::LeaveIfError(
        iSession.SendReceiveL( EHvsServerPlaylistFileExt, TIpcArgs(&playlistType, &extension)));

    return fileExtension;
    }

// ---------------------------------------------------------------------------
// Determines if the given file is a playlist
// ---------------------------------------------------------------------------
//
TBool CMPXHarvesterUtilityImp::IsPlaylistL( const TDesC& aUri )
    {
    return iSession.SendReceiveL( EHvsServerIsPlaylist, TIpcArgs(&aUri) );
    }

// ---------------------------------------------------------------------------
// Tells the harvester to poll for system events
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::CheckForSystemEventsL()
    {
    User::LeaveIfError( iSession.SendReceiveL( EHvsServerCheckSystemEvent ) );
    }


// ---------------------------------------------------------------------------
// Closes the harvester utility
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::Close()
    {
    delete this;
    }

// ---------------------------------------------------------------------------
// Get a media object for the file
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::GetMediaForFileL( const TDesC& aFilePath,
                                        MMPXHarvesterUtilityObserver* aObs )
    {
    ASSERT( !iCurTask );
    HBufC* path = aFilePath.AllocLC();
    iCurTask = new(ELeave) CHvsUtilityTask( NULL, path,
                                            NULL, NULL );
    CleanupStack::Pop( path );
    iSession.SendReceive( EHvsServerGetMedia, TIpcArgs( &iCurTask->Arg2() ), iStatus );

    // Set Async op
    iCurAsyncOp = EHvsServerGetMedia;
    iObserver = aObs;
    SetActive();
    }

// ---------------------------------------------------------------------------
// Get a collection ID for the file
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterUtilityImp::GetColUidForFileL( const TDesC& aFilePath )
    {
    return iSession.SendReceiveL( EHvsServerGetColUid, TIpcArgs(&aFilePath) );
    }

// ---------------------------------------------------------------------------
// Export a playlist to a file
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::ExportPlaylistL( const CMPXMedia& aProp,
                                            const TDesC& aPath,
                                            TInt aPlaylistType,
                                            MMPXHarvesterUtilityObserver* aObs )
    {
    if( iCurAsyncOp != EHvsServerOpCount )
        {
        User::Leave( KErrNotReady );
        }

    // Construct the IPC Args
    CBufBase* buf( NULL );
    CreateBufferL<CMPXMedia>( aProp, buf );
    CleanupStack::PushL( buf );
    HBufC* path = aPath.AllocLC();
    CMPXMedia* copy = CMPXMedia::NewL( aProp );
    CleanupStack::PushL( copy );
    iCurTask = new(ELeave) CHvsUtilityTask( buf, path,
                                            NULL, aPlaylistType, copy );  // ownership xfer
    CleanupStack::Pop( 3, buf );

    // Send the IPC
    //
    iSession.SendReceive( EHvsServerExportPlaylist,
                          TIpcArgs( &iCurTask->Arg1(), &iCurTask->Arg2(),
                                    &iCurTask->Arg4() ),
                          iStatus );

    iCurAsyncOp = EHvsServerExportPlaylist;
    iObserver = aObs;
    SetActive();
    }

// ---------------------------------------------------------------------------
// Import a playlist from a file
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::ImportPlaylistL( const TDesC& aFilename,
                                            MMPXHarvesterUtilityObserver* aObs )
    {
    if( iCurAsyncOp != EHvsServerOpCount )
        {
        User::Leave( KErrNotReady );
        }

    HBufC* file = aFilename.AllocL();
    iCurTask = new(ELeave) CHvsUtilityTask( NULL, file );
    iSession.SendReceive( EHvsServerImportPlaylist,
                          TIpcArgs( &iCurTask->Arg2() ),
                          iStatus );

    iCurAsyncOp = EHvsServerImportPlaylist;
    iObserver = aObs;
    SetActive();
    }

// ---------------------------------------------------------------------------
// Import a file, async
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::ImportFileL( const TDesC& aFilename,
                                        MMPXHarvesterUtilityObserver* aObs )
    {
    if( iCurAsyncOp != EHvsServerOpCount )
        {
        User::Leave( KErrNotReady );
        }
    if( iFile != NULL )
    	{
    	delete iFile;
    	iFile = NULL;
    	}
    iFile = aFilename.AllocL();
    HBufC* file = aFilename.AllocL();
    iCurTask = new(ELeave) CHvsUtilityTask( NULL, file );
    iSession.SendReceive( EHvsServerImportFile,
                          TIpcArgs( iFile ),
                          iStatus );

    iCurAsyncOp = EHvsServerImportFile;
    iObserver = aObs;
    SetActive();
    }

// ---------------------------------------------------------------------------
// RunL() from CActive
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::RunL()
    {
    // Try again in case server is down for IAD
    if (iStatus == KErrDied || iStatus == KErrServerTerminated)
        {
        MPX_DEBUG1("-->CMPXHarvesterUtilityImp::RunL Reconnecting session for IAD");
        
        // attempt to bring the server back up
        if (iSession.Reconnect() != KErrNone)
            {
            MPX_DEBUG1("-->CMPXHarvesterUtilityImp::RunL Reconnect Failed");
            }
        }
    
    // Clear status needs to be here because user can launch a second
    // async op within the callback!
    //
    delete iCurTask;
    iCurTask = NULL;

    // Cache these variables as locals because they need to be used
    //
    MMPXHarvesterUtilityObserver*  curObserver = iObserver;
    iObserver = NULL;
    TInt curTask = iCurAsyncOp;
    iCurAsyncOp = EHvsServerOpCount;


    switch( curTask )
        {
        case EHvsServerAddFile:
            {
            TInt size = iStatus.Int();
            CMPXMedia* prop( NULL );
            if( size > 0 )
                {
                ::TransferBufferFromServerL(iSession, EHvsServerGetBuffer,
                                            size, iBuffer);
                ::NewFromBufferL<CMPXMedia>( *iBuffer, prop );
                delete iBuffer;
                iBuffer = NULL;
                }
            curObserver->HandleFileAddCompletedL( prop, size<0?size:KErrNone ) ;
            break;
            }
        case EHvsServerRemoveFile:
        case EHvsServerRemoveFiles:  // deliberate fall through
            {
            TInt err( iStatus.Int() );
            curObserver->HandleDeleteCompletedL( err>0?KErrNone:err );
            break;
            }
        case EHvsServerExportPlaylist:
            {
            TInt size = iStatus.Int();
            CMPXMedia* prop( NULL );
            if( size > 0 )
                {
                ::TransferBufferFromServerL(iSession, EHvsServerGetBuffer,
                                            size, iBuffer);
                ::NewFromBufferL<CMPXMedia>( *iBuffer, prop );
                delete iBuffer;
                iBuffer = NULL;
                }
            // observer takes ownership of prop
            curObserver->HandlePlaylistExportCompletedL( prop, size<0?size:KErrNone );
            break;
            }
        case EHvsServerImportPlaylist:
            {
            TInt size = iStatus.Int();
            CMPXMedia* prop(NULL);
            if( size > 0 )
                {
                ::TransferBufferFromServerL(iSession, EHvsServerGetBuffer,
                                            size, iBuffer);
                ::NewFromBufferL<CMPXMedia>( *iBuffer, prop );
                delete iBuffer;
                iBuffer = NULL;
                }
            curObserver->HandlePlaylistImportCompletedL( prop,
                                                       size<0?size:KErrNone );
            break;
            }
        case EHvsServerImportFile:
            {
            TInt size = iStatus.Int();
            CMPXMedia* prop(NULL);
            if( size > 0 )
                {
                ::TransferBufferFromServerL(iSession, EHvsServerGetBuffer,
                                            size, iBuffer);
                ::NewFromBufferL<CMPXMedia>( *iBuffer, prop );
                delete iBuffer;
                iBuffer = NULL;
                }
            curObserver->HandleFileImportCompletedL( prop, size<0?size:KErrNone );
            break;
            }
        case EHvsServerGetMedia:
            {
            TInt size = iStatus.Int();
            CMPXMedia* prop( NULL );
            if( size > 0 )
                {
                ::TransferBufferFromServerL(iSession, EHvsServerGetBuffer,
                                            size, iBuffer);
                ::NewFromBufferL<CMPXMedia>( *iBuffer, prop );
                delete iBuffer;
                iBuffer = NULL;
                }
            curObserver->HandleFileGetMediaCompletedL( prop, size<0?size:KErrNone ) ;
            break;
            }
        default:
            {
            ASSERT(0);
            }
        }
    }

// ---------------------------------------------------------------------------
// Cancels the current async op
// ---------------------------------------------------------------------------
//
void CMPXHarvesterUtilityImp::DoCancel()
    {
    // Do Nothing
    }

// ----------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL()
// ----------------------------------------------------------------------------
//
TInt CMPXHarvesterUtilityImp::RunError(TInt aError)
    {
    MPX_DEBUG2("CMPXHarvesterUtilityImp::RunError(%d)", aError );
    (void) aError;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Utility class to cache async requests
// ---------------------------------------------------------------------------
//
CMPXHarvesterUtilityImp::CHvsUtilityTask::CHvsUtilityTask( CBufBase* arg1,
                                                        HBufC* arg2,
                                                        HBufC* arg3,
                                                        TInt arg4,
                                                        CMPXMedia* arg5 )
    {
    iArg1 = arg1;
    iArg2 = arg2;
    iArg3 = arg3;
    iArg4 = TPckgBuf<TInt>(arg4);
    iArg5 = arg5;

    if( iArg1 )
        {
        iPtr1.Set(iArg1->Ptr(0));
        }
    if( iArg2 )
        {
        iPtr2.Set(iArg2->Des());
        }
    if( iArg3 )
        {
        iPtr3.Set(iArg3->Des());
        }
    }

// ---------------------------------------------------------------------------
// Default destructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterUtilityImp::CHvsUtilityTask::~CHvsUtilityTask()
    {
    delete iArg1;
    delete iArg2;
    delete iArg3;
    delete iArg5;
    }

// ---------------------------------------------------------------------------
// Access each argument
// ---------------------------------------------------------------------------
//
const TDesC8& CMPXHarvesterUtilityImp::CHvsUtilityTask::Arg1()
    {
    return iPtr1;
    }

// ---------------------------------------------------------------------------
// Access each argument
// ---------------------------------------------------------------------------
//
const TDesC&  CMPXHarvesterUtilityImp::CHvsUtilityTask::Arg2()
    {
    return iPtr2;
    }

// ---------------------------------------------------------------------------
// Access each argument
// ---------------------------------------------------------------------------
//
const TDesC&  CMPXHarvesterUtilityImp::CHvsUtilityTask::Arg3()
    {
    return iPtr3;
    }

// ---------------------------------------------------------------------------
// Access each argument
// ---------------------------------------------------------------------------
//
const TDesC8&  CMPXHarvesterUtilityImp::CHvsUtilityTask::Arg4()
    {
    return iArg4;
    }

// ---------------------------------------------------------------------------
// Access each argument
// ---------------------------------------------------------------------------
//
const CMPXMedia& CMPXHarvesterUtilityImp::CHvsUtilityTask::Arg5()
    {
    return *iArg5;
    }

// END OF FILE
