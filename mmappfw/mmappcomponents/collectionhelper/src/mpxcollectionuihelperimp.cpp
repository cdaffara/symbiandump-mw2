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
* Description:  Collection Ui Helper implementation
*
*/


#include <e32base.h>
#include <s32file.h>
#include <mpxmedia.h>
#include <mpxcollectionplaylist.h>
#include <mpxmediaarray.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediacontainerdefs.h>
#include <mpxuser.h>
#include <mpxcmn.h>
#include <mpxcollectionutility.h>
#include <mpxcollectiontype.h>
#include <mpxcollectionmediator.h>
#include <mpxplaylistenginedefs.h>
#include <mpxlog.h>
#include <mpxcollectionmessage.h>
#include <mpxtaskqueue.h>
#include <mpxcollectionplugin.hrh>
#include <mpxharvesterutility.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxcollectioncommanddefs.h>
#include <UsbWatcherInternalPSKeys.h>
#include <usbpersonalityids.h>
#include <mtpprivatepskeys.h>

#include "mpxcollectionuihelperobserver.h"
#include "mpxcollectionuihelperimp.h"
#include "mpxcollectionhelpercommon.h"
#include "mpxdeletehelper.h"

// CONSTANTS
const TInt KMPXAllSongsViewIndex = 0;
const TInt KMPXPlaylistViewIndex = 1;
const TInt KMPXRecentlyPlayedIndex = 0x20000000;
const TInt KMPXMostPlayedIndex = 0x20000001;
const TInt KMPXRecentlyAddedIndex = 0x20000002;

_LIT( KPathVirtualPlaylistHandling, "openpath.dat");

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Private Constructor
// ---------------------------------------------------------------------------
//
CMPXCollectionUiHelperImp::CMPXCollectionUiHelperImp()
    {
    }


// ---------------------------------------------------------------------------
// 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::ConstructL(const TUid& aModeId)
    {
    iCollection = MMPXCollectionUtility::NewL(this,aModeId);
    iMediator = CMPXCollectionMediator::NewL( iCollection->Collection(),
                                              this );
    iHarvester = CMPXHarvesterFactory::NewL();
    iTaskQueue = CMPXActiveTaskQueue::NewL();
    iDeleteHelper = CMPXDeleteHelper::NewL(*iCollection, *iHarvester, *this);
    
    iRemainder = 0;
    iTotalChunkNumber = 0;
    iChunkNumber = 0;
    iChunkSize = 0;
    iArrayIndex = 0;
    iRefCount = 1;
    iModeId = aModeId;
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXCollectionUiHelperImp* CMPXCollectionUiHelperImp::NewL(const TUid& aModeId)
    {
    CMPXCollectionUiHelperImp* self = CMPXCollectionUiHelperImp::NewLC(aModeId);
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXCollectionUiHelperImp* CMPXCollectionUiHelperImp::NewLC(const TUid& aModeId)
    {

	CMPXCollectionUiHelperImp* self(NULL);

    if ( aModeId == KMcModeDefault )
        {
    	self = reinterpret_cast<CMPXCollectionUiHelperImp*>(Dll::Tls());
    	if ( !self )
            {
            self = new( ELeave ) CMPXCollectionUiHelperImp();
            CleanupStack::PushL( self );
			self->ConstructL(aModeId);
            Dll::SetTls( self );
            }
        else
            {
            self->iRefCount++;
            CleanupStack::PushL( self );
            }

		return self;
        }
    else
		{
		self = new( ELeave ) CMPXCollectionUiHelperImp();
		CleanupStack::PushL( self );
		self->ConstructL(aModeId);
		return self;
		}
    }

// ---------------------------------------------------------------------------
// Virtual Destructor
// ---------------------------------------------------------------------------
//
CMPXCollectionUiHelperImp::~CMPXCollectionUiHelperImp()
    {
    delete iMediator;
    if( iCollection )
        {
        iCollection->Close();
        }
    if( iHarvester )
        {
        iHarvester->Close();
        }
    delete iTaskQueue;
    delete iMedia;
    delete iDeleteHelper;
    
    delete iInputMedia;
    }


// ---------------------------------------------------------------------------
// Issue collection initialization command to collection framework.
// This is necessary when collection helper serves as an intermediator
// between collection framework and Podcast App or MTP, etc.
// There is, currently, no way to check the merge status of the collections
// through the collection utility API.  Therefore, collectionhelper issue
// an EMcCmdCollectionInit(or merge) command before any collectionhelper
// functionality, which expects collection is in valid state, is excuted.
// ---------------------------------------------------------------------------
//

// ---------------------------------------------------------------------------
// Add a Media to the collection
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::AddL( const CMPXMedia& aMedia,
                                MMPXCHelperObserver* aObserver )
    {
    if (!aObserver)
        {
        User::Leave( KErrArgument );
        }

    //
    // leave if the given media doesn't contain the following attributes
    //
    if (!aMedia.IsSupported(KMPXMediaGeneralType) ||
        !aMedia.IsSupported(KMPXMediaGeneralCategory))
        {
        User::Leave( KErrArgument );
        }

    //
    // leave if the given media isn't a playlist (i.e. type must be EMPXItem and category
    // must be EMPXPlaylist
    //
    TMPXGeneralType mediaType =
         aMedia.ValueTObjectL<TMPXGeneralType>(KMPXMediaGeneralType);

    TMPXGeneralCategory mediaCategory =
         aMedia.ValueTObjectL<TMPXGeneralCategory>(KMPXMediaGeneralCategory);

    //
    // currently only supporting playlists
    //
    if ( mediaType != EMPXItem ||
         mediaCategory != EMPXPlaylist )
        {
        User::Leave( KErrNotSupported );
        }

    //
    // leave if the media doesn't contain mandatory attributes for adding a playlist
    // or adding (appending) tracks to an existing playlist.
    // Required attributes for adding a new playlist are:
    //  type, category, array contents, array counts, title, and path for playlist
    // Required attributes for adding tracks to an existing playlist are:
    //  type, category, array contents, array counts, and playlist Id
    //
    if (!aMedia.IsSupported(KMPXMediaArrayContents) ||
        !aMedia.IsSupported(KMPXMediaArrayCount))
        {
        User::Leave( KErrArgument );
        }

    TInt task(ETaskNone);
    if (!aMedia.IsSupported(KMPXMediaGeneralId))
        {
        if (!aMedia.IsSupported(KMPXMediaGeneralTitle) ||
            !aMedia.IsSupported(KMPXMediaGeneralUri))
            {
            User::Leave( KErrArgument );
            }
        else
            {
            task = ETaskAddMedia;
            }
        }
    else
        {
        task = ETaskAppendMedia;
        }

    //
    // externalize parameters
    //
    CBufBase* taskParam(NULL);
    ::CreateBufferL<CMPXMedia>( aMedia, taskParam );
    CleanupStack::PushL(taskParam);

    // Increase reference count for task queue
    CMPXMedia* ref = CMPXMedia::NewL( aMedia );
    CleanupStack::PushL(ref);

    //
    // add request to the task queue
    //
    iTaskQueue->AddTaskL( task,
                          aObserver,   // callback when task completed
                          this,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,        // Ptr data
                          ref );       // ownership transferred

    CleanupStack::Pop(2, taskParam);   // ref & task Param
    }

// ---------------------------------------------------------------------------
// Increment Addition
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::IncAddL( const CMPXMedia& aMedia,
                                         MMPXCHelperObserver* aObserver,
                                         const TInt aSize )
    {
    if (!aObserver)
        {
        User::Leave( KErrArgument );
        }
    
    //
    // leave if the given media doesn't contain the following attributes
    //
    if (!aMedia.IsSupported(KMPXMediaGeneralType) ||
        !aMedia.IsSupported(KMPXMediaGeneralCategory))
        {
        User::Leave( KErrArgument );
        }
        
    //
    // leave if the given media isn't a playlist (i.e. type must be EMPXItem and category
    // must be EMPXPlaylist
    //
    TMPXGeneralType mediaType =
         aMedia.ValueTObjectL<TMPXGeneralType>(KMPXMediaGeneralType);

    TMPXGeneralCategory mediaCategory =
         aMedia.ValueTObjectL<TMPXGeneralCategory>(KMPXMediaGeneralCategory);

    //
    // currently only supporting playlists
    //
    if ( mediaType != EMPXItem ||
         mediaCategory != EMPXPlaylist )
        {
        User::Leave( KErrNotSupported );
        }

    //
    // leave if the media doesn't contain mandatory attributes for adding a playlist
    // or adding (appending) tracks to an existing playlist.
    // Required attributes for adding a new playlist are:
    //  type, category, array contents, array counts, title, and path for playlist
    // Required attributes for adding tracks to an existing playlist are:
    //  type, category, array contents, array counts, and playlist Id
    //
    if (!aMedia.IsSupported(KMPXMediaArrayContents) ||
        !aMedia.IsSupported(KMPXMediaArrayCount))
        {
        User::Leave( KErrArgument );
        }

    TInt task(ETaskNone);
    if (!aMedia.IsSupported(KMPXMediaGeneralId))
        {
        if (!aMedia.IsSupported(KMPXMediaGeneralTitle) ||
            !aMedia.IsSupported(KMPXMediaGeneralUri))
            {
            User::Leave( KErrArgument );
            }
        else
            {
            task = ETaskAddMedia;
            }
        }
    else
        {
        task = ETaskAppendMedia;
        }

    // get tracks from input media
    // since using Value instead of ValueCObject, no Push/Pop
    CMPXMediaArray* tracksArray = 
        aMedia.Value<CMPXMediaArray>( KMPXMediaArrayContents );
    
    // use incremental add only if number of songs > aSize
    // this is needed as IncAdd does not deal w/ the case when tracksArray->Count() == aSize
    if( tracksArray->Count() <= aSize || aSize <= 0 )
        {
        //
        // externalize parameters
        //
        CBufBase* taskParam(NULL);
        ::CreateBufferL<CMPXMedia>( aMedia, taskParam );
        CleanupStack::PushL(taskParam);
    
        // Increase reference count for task queue
        CMPXMedia* ref = CMPXMedia::NewL( aMedia );
        CleanupStack::PushL(ref);
        
        //
        // add request to the task queue
        //
        iTaskQueue->AddTaskL( task,
                              aObserver,   // callback when task completed
                              this,        // task queue observer
                              0,           // Integer paramter, not used
                              taskParam,   // task queue assumes ownership of taskParam
                              NULL,        // Ptr data 
                              ref );       // ownership transferred
                              
        CleanupStack::Pop(2, taskParam);   // ref & task Param
        }
    else // Inc Add
        { 
        // calculate number of songs in the last chunk   
        iRemainder = tracksArray->Count() % aSize;
        
        // calculate the number of iterations/chunks needed to divide the songs
        // each chunk contians aSize number of songs
        if( iRemainder == 0 )
            {
            iTotalChunkNumber = tracksArray->Count() / aSize;
            }
        else
            {
            iTotalChunkNumber = tracksArray->Count() / aSize + 1;
            }
        
        iChunkNumber = 0;  // initialize chunk number
        iChunkSize = aSize;
        
        // need this in the case multiple Inc Add is called (if something leaves and
        // iInputMedia was not cleaned up properly)
        if( iInputMedia )
            {
            delete iInputMedia;
            iInputMedia = NULL;
            }
        
        // save needed info for subsequent processing
        iInputMedia = CMPXMedia::CopyL(aMedia);
        iArrayIndex = 0;  // index on tracksArray of the input media
        
        for( int i = 0; i < iTotalChunkNumber; i++ )
            {
            if( task ==  ETaskAddMedia )
                {
                // schedule the adding requests to the task queue
                iTaskQueue->AddTaskL( ETaskIncAddMedia,
                                      aObserver,   // callback when task completed
                                      this );        // task queue observer
                }
            else  // ETaskAppendMedia
                {
                // schedule the appending requests to the task queue
                iTaskQueue->AddTaskL( ETaskIncAppendMedia,
                                      aObserver,   // callback when task completed
                                      this );        // task queue observer
                }
            }
        }
    
    }




// ---------------------------------------------------------------------------
// Update a single media property
// Checks for podcast changes in gendre
// Checks for "PodCast" gendre
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::SetL( CMPXMedia*& aMedia,
                                      MMPXCHelperObserver* aObserver )
    {
    MPX_FUNC("CMPXCollectionUiHelperImp::SetL");

    if (!aObserver)
        {
        User::Leave( KErrArgument );
        }

    //
    // leave if the given media doesn't contain the following attributes
    //
    if (!aMedia->IsSupported(KMPXMediaGeneralType) ||
        !aMedia->IsSupported(KMPXMediaGeneralCategory))
        {
        User::Leave( KErrArgument );
        }

    //
    // externalize parameters
    //
    CBufBase* taskParam(NULL);
    ::CreateBufferL<CMPXMedia>( *aMedia, taskParam );
    CleanupStack::PushL(taskParam);

    //
    // add request to the task queue
    //
    CMPXMedia* ref = CMPXMedia::NewL( *aMedia );  // ownership transferred
    CleanupStack::PushL(ref);

    iTaskQueue->AddTaskL( ETaskSetMedia,
                          aObserver,   // callback when task completed
                          this,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,        // ptr data
                          ref );       // keep reference count alive

    CleanupStack::Pop(2, taskParam);   // ref & taskParam
    }

// ---------------------------------------------------------------------------
// Update a list of medias, checks for podcast changes in genre
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::SetL( TArray<CMPXMedia*>& aMediaArray,
                                CMPXCollectionPath& aPath,
                                RArray<TInt>& aIndices )
    {
    iMediator->SetItemL( aMediaArray, aPath, aIndices );
    }

// ---------------------------------------------------------------------------
// rename a media
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::RenameL( const CMPXMedia& aMedia,
                                         MMPXCHelperObserver* aObserver )
    {
    if (!aObserver)
        {
        User::Leave( KErrArgument );
        }

    //
    // leave if the given media doesn't contain the following attributes
    //
    if (!aMedia.IsSupported(KMPXMediaGeneralType) ||
        !aMedia.IsSupported(KMPXMediaGeneralCategory))
        {
        User::Leave( KErrArgument );
        }

    //
    // leave if the given media isn't a playlist
    //
    TMPXGeneralType mediaType =
         aMedia.ValueTObjectL<TMPXGeneralType>(KMPXMediaGeneralType);

    TMPXGeneralCategory mediaCategory =
         aMedia.ValueTObjectL<TMPXGeneralCategory>(KMPXMediaGeneralCategory);

    //
    // currently only supporting playlists
    //
    if ( mediaType != EMPXItem ||
         mediaCategory != EMPXPlaylist )
        {
        User::Leave( KErrNotSupported );
        }

    //
    // leave if the media doesn't contain mandatory attributes for renaming a playlist
    //
    if (!aMedia.IsSupported(KMPXMediaGeneralTitle) ||
        !aMedia.IsSupported(KMPXMediaGeneralId))
        {
        User::Leave( KErrArgument );
        }

    //
    // externalize parameters
    //
    CBufBase* taskParam(NULL);
    ::CreateBufferL<CMPXMedia>( aMedia, taskParam );
    CleanupStack::PushL(taskParam);

    // Keep media alive
    CMPXMedia* ref = CMPXMedia::NewL( aMedia );
    CleanupStack::PushL(ref);

    //
    // add request to the task queue
    //
    iTaskQueue->AddTaskL( ETaskRenameMedia,
                          aObserver,   // callback when task completed
                          this,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,        // ptr data
                          ref );       // ownership transferred

    CleanupStack::Pop(2, taskParam);   // ref & taskParam
    }

// ---------------------------------------------------------------------------
// Delete a collection path and the files
// associated with each leaf node in the path
// 1: Removes the file from Collection db
// 2: Deletes the file from FS
// 3: Removes the file from Harvester db
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::DeleteL( CMPXCollectionPath& aPath,
                                         MMPXCHelperObserver* aObs )
    {
    if (!aObs)
        {
        User::Leave(KErrArgument);
        }

    MPX_DEBUG_PATH(aPath);

    //
    // externalize parameters
    //
    CBufBase* taskParam(NULL);
    ::CreateBufferL<CMPXCollectionPath>( aPath, taskParam );
    CleanupStack::PushL(taskParam);

    //
    // add request to the task queue
    //
    iTaskQueue->AddTaskL( ETaskRemoveMedia,
                          aObs,        // callback when task completed
                          this,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam);  // task queue assumes ownership of taskParam

    CleanupStack::Pop(taskParam);
    }

// ---------------------------------------------------------------------------
// Move an item from one collection to another
// 0: Convert enum to collection lookup UID
// 1: Removes the item from the old collection
// 2: Adds the item to the new collection using mediator
//    (Mediator sets genre "Podcast" to "Unknown" if we move from Music -> Podcast
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::MoveL( CMPXMedia*& aMedia,
                                       TUid aNewCollection,
                                       MMPXCHelperObserver* aObs )
    {
    if (!aMedia->IsSupported(KMPXMediaGeneralCollectionId))
        {
        User::Leave(KErrArgument);
        }

    // Lookup the collection
    //
    RArray<TUid> ary;
    CleanupClosePushL( ary );
    ary.AppendL( aNewCollection );
    TUid newuid = iCollection->CollectionIDL( ary.Array() );
    CleanupStack::PopAndDestroy( &ary );

    // Move with mediator
    //
    const TUid& olduid = aMedia->ValueTObjectL<TUid>( KMPXMediaGeneralCollectionId );

    iHelperObserver = aObs;
    if( !aObs )
        {
        iMediator->MoveItemL( aMedia, olduid, newuid );
        }
    else
        {
        // Should i add this to the task queue? need to think about it
        iMediator->MoveItemL( aMedia, olduid, newuid, this );
        }

    // Notify harvester of the explicit move in collection
    //
    aMedia->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, newuid);
    iHarvester->UpdateFileL( aMedia );
    }

// ---------------------------------------------------------------------------
// Open the collection in embedded mode for a media
// Cannot cleanup aObs because UPnP is using this.
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::OpenL( const TUid& aHostId, CMPXMedia& aMedia,
                                       MMPXCHelperEmbeddedOpenObserver* /*aObs*/,
                                       TInt aPluginInfo )
    {
    // Grab the in memory collection plugin UID
    // aPluginInfo provides additional resolution to find the plugin
    //
    RArray<TUid> ary;
    CleanupClosePushL( ary );
    ary.AppendL( TUid::Uid(EMPXCollectionPluginHidden) );
    ary.AppendL( TUid::Uid(EMPXCollectionPluginTemporary) );
    if( aPluginInfo )
        {
        ary.AppendL( TUid::Uid(aPluginInfo) );
        }

    TUid inMemCollection = iCollection->CollectionIDL( ary.Array() );
    CleanupStack::PopAndDestroy( &ary );

    // First step is to add this media to the in memory plugin
    // Set the item id to be the host ID
    //
    aMedia.SetTObjectValueL( KMPXMediaGeneralCollectionId, inMemCollection );
    aMedia.SetTObjectValueL<TMPXItemId>( KMPXMediaGeneralId, aHostId.iUid );

    // Add the media via CMPXCommand
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL( KMPXCommandGeneralId,
                           KMPXCommandIdCollectionAdd );
    cmd->SetTObjectValueL( KMPXCommandGeneralDoSync,
                           ETrue );
    cmd->SetTObjectValueL( KMPXCommandGeneralCollectionId,
                           inMemCollection.iUid );
    cmd->SetCObjectValueL( KMPXCommandColAddMedia,
                           &aMedia );
    iCollection->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );

    // Second Step is to construct collection path
    // | collection id | host id |
    //
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( inMemCollection.iUid );
    path->AppendL( aHostId.iUid );

    // Last step is to open this path
    //
    iCollection->Collection().OpenL( *path );
    CleanupStack::PopAndDestroy( path );
    }

// ---------------------------------------------------------------------------
// Open the collection in embedded mode for a file handle
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::OpenL( const TUid& aHostId, const TDesC& aFile,
                                       MMPXCHelperEmbeddedOpenObserver* aObs,
                                       TInt aPluginInfo )
    {

    // Open in embedded mode, no database merge necessary
    if( !aObs )
        {
        User::LeaveIfError( KErrArgument );
        }

    iEmbeddedModeID = aHostId;
    iEmbeddedPluginInfo = aPluginInfo;
    iOpenObserver = aObs;

    iHarvester->ImportFileL( aFile, this );
    }

// ---------------------------------------------------------------------------
// Open the collection in embedded mode for a file handle
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::OpenL( RFile& aFile,
                                       MMPXCHelperEmbeddedOpenObserver* aObs )
    {
    MPX_FUNC( "CMPXCollectionUiHelperImp::OpenL (file handle, observer)" );
    // Open in embedded mode, no database merge necessary
    if( !aObs )
        {
        User::Leave( KErrArgument );
        }

    iOpenObserver = aObs;
    TMPXGeneralCategory category( EMPXNoCategory );

    CFileStore* store( NULL );
    MPX_TRAPD( error, store = CDirectFileStore::FromL( aFile ); );

    if ( error == KErrNone )
        {
        CleanupStack::PushL( store );

        RStoreReadStream stream;
        stream.OpenLC( *store, store->Root() );

        // Recreate the collection path that we were browsing
        CMPXCollectionPath* collectionPath = CMPXCollectionPath::NewL( stream );
        CleanupStack::PopAndDestroy( 2, store ); // stream, store
        CleanupStack::PushL( collectionPath );

        MPX_DEBUG_PATH( *collectionPath );

        if ( collectionPath->Levels() != 3 ||
            ( collectionPath->Id(1) != KMPXAllSongsViewIndex &&
             collectionPath->Id(1) != KMPXPlaylistViewIndex ) )
            {
            error = KErrNotSupported;
            }
        else
            {
            category = collectionPath->Id( 1 ) ==
                KMPXAllSongsViewIndex ? EMPXSong : EMPXPlaylist;
            TRAP( error, iCollection->Collection().OpenL( *collectionPath ) );
            }
        CleanupStack::PopAndDestroy( collectionPath );
        }

    // Callback to observer for err handling
    ( ( MMPXCHelperEmbeddedOpenObserver* )iOpenObserver )->HandleEmbeddedOpenL( error, category );
    }

// ---------------------------------------------------------------------------
// Queries about the playlist file extension of the currently selected playlist
// plugin
// ---------------------------------------------------------------------------
HBufC* CMPXCollectionUiHelperImp::PlaylistFileExtensionLC()
    {
    // default to M3U playlist type for now
    return iHarvester->PlaylistFileExtensionLC(EMPXPlaylistTypeM3U);
    }

// ---------------------------------------------------------------------------
// Export a playlist
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::ExportPlaylistL(TMPXItemId aPlaylistId,
                                                const TDesC& aDestinationDriveAndPath,
                                                MMPXCHelperObserver* aObs)
    {
    if (!aObs)
        {
        User::Leave(KErrArgument);
        }

    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL(media);

    media->SetTObjectValueL<TMPXItemId>(KMPXMediaGeneralId, aPlaylistId);
    media->SetTextValueL(KMPXMediaGeneralUri, aDestinationDriveAndPath);

    //
    // externalize parameters
    //
    CBufBase* taskParam(NULL);
    ::CreateBufferL<CMPXMedia>( *media, taskParam );
    CleanupStack::PushL(taskParam);

    //
    // add request to the task queue
    //
    iTaskQueue->AddTaskL( ETaskExportPlaylist,
                          aObs,        // callback when task completed
                          this,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,
                          media );     // keep media alive, ownership transferred

    CleanupStack::Pop(2, media);       // taskParam & media
    }

// ---------------------------------------------------------------------------
// Reorder a song in a playlist
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::ReorderPlaylistL(const TMPXItemId& aPlaylistId,
                                                 const TMPXItemId& aSongId,
                                                 TUint aOriginalOrdinal,
                                                 TUint aNewOrdinal,
                                                 MMPXCHelperObserver* aObs)
    {
    if (!aObs)
        {
        User::Leave(KErrArgument);
        }

    CMPXMedia* command = CMPXMedia::NewL();
    CleanupStack::PushL(command);

    command->SetTObjectValueL<TMPXItemId>(KMPXCommandReorderPlaylistId, aPlaylistId);
    command->SetTObjectValueL<TMPXItemId>(KMPXCommandReorderSongId, aSongId);
    command->SetTObjectValueL<TUint>(KMPXCommandReorderOriginalOrdinal, aOriginalOrdinal);
    command->SetTObjectValueL<TUint>(KMPXCommandReorderNewOrdinal, aNewOrdinal);

    //
    // externalize parameters
    //
    CBufBase* taskParam(NULL);
    ::CreateBufferL<CMPXMedia>( *command, taskParam );
    CleanupStack::PushL(taskParam);

    //
    // add request to the task queue
    //
    iTaskQueue->AddTaskL( ETaskReorderPlaylist,
                          aObs,        // callback when task completed
                          this,        // task queue observer
                          0,           // Integer paramter, not used
                          taskParam,   // task queue assumes ownership of taskParam
                          NULL,
                          command );   // keep media alive, ownership transferred

    CleanupStack::Pop(2, command);       // taskParam & media
    }

// ---------------------------------------------------------------------------
// Creates a default collection path to the music menu
// ---------------------------------------------------------------------------
//
CMPXCollectionPath* CMPXCollectionUiHelperImp::MusicMenuPathL()
    {
    // Get top level path only, no database merge necessary

    // Convert the Virtual UID to the real UID
    RArray<TUid> ary;
    CleanupClosePushL( ary );
    ary.AppendL( TUid::Uid(EMPXCollectionPluginMusic) );
    TUid musicCollection = iCollection->CollectionIDL( ary.Array() );
    CleanupStack::PopAndDestroy( &ary );

    // Second Step is to construct collection path
    // | collection id |
    //
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( musicCollection.iUid );
    CleanupStack::Pop( path );

    return path;
    }

// ---------------------------------------------------------------------------
// Creates a default collection path to the podcast menu
// ---------------------------------------------------------------------------
//
CMPXCollectionPath* CMPXCollectionUiHelperImp::PodCastMenuPathL()
    {
    // Get top level path only, no database merge necessary

    // Convert the Virtual UID to the real UID
    RArray<TUid> ary;
    CleanupClosePushL( ary );
    ary.AppendL( TUid::Uid(EMPXCollectionPluginPodCast) );
    TUid pcCollection = iCollection->CollectionIDL( ary.Array() );
    CleanupStack::PopAndDestroy( &ary );

    // Second Step is to construct collection path
    // | collection id |
    //
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( pcCollection.iUid );
    CleanupStack::Pop( path );

    return path;
    }

// ---------------------------------------------------------------------------
// Creates a default collection path to the music all songs
// ---------------------------------------------------------------------------
//
CMPXCollectionPath* CMPXCollectionUiHelperImp::MusicAllSongsPathL()
    {
    CMPXCollectionPath* path = MusicMenuPathL();
    CleanupStack::PushL( path );
    path->AppendL( KMPXAllSongsViewIndex );
    CleanupStack::Pop( path );

    return path;
    }

// ---------------------------------------------------------------------------
// Creates a default collection path to the music playlist
// ---------------------------------------------------------------------------
//
CMPXCollectionPath* CMPXCollectionUiHelperImp::MusicPlaylistPathL()
    {
    CMPXCollectionPath* path = MusicMenuPathL();
    CleanupStack::PushL( path );
    path->AppendL( KMPXPlaylistViewIndex );
    CleanupStack::Pop( path );

    return path;
    }

// ---------------------------------------------------------------------------
// Creates a default collection path to the music auto playlist
// ---------------------------------------------------------------------------
//
CMPXCollectionPath* CMPXCollectionUiHelperImp::MusicPlaylistPathL(TMPXAutoPlaylistType aPlaylistType)
    {
    CMPXCollectionPath* path = MusicPlaylistPathL();
    CleanupStack::PushL( path );
    if ( aPlaylistType == EMPXRecentlyPlayedPlaylist )
        {
        path->AppendL( KMPXRecentlyPlayedIndex );
        }
    else if ( aPlaylistType == EMPXMostPlayedPlaylist )
        {
        path->AppendL( KMPXMostPlayedIndex );
        }
    else if ( aPlaylistType == EMPXRecentlyAddedPlaylist)
        {
        path->AppendL( KMPXRecentlyAddedIndex );
        }
    CleanupStack::Pop( path );

    return path;
	}

// ---------------------------------------------------------------------------
// Creates a default collection playlist hard coded to all songs of
// music database
// ---------------------------------------------------------------------------
//
CMPXCollectionPath* CMPXCollectionUiHelperImp::CreateDefaultPlaylistPathLC()
    {
    // Get top level path only, no database merge necessary

    // Convert the Virtual UID to the real UID
    RArray<TUid> ary;
    CleanupClosePushL( ary );
    ary.AppendL( TUid::Uid(EMPXCollectionPluginMusic) );
    TUid musicCollection = iCollection->CollectionIDL( ary.Array() );
    CleanupStack::PopAndDestroy( &ary );

    // Second Step is to construct collection path
    // | collection id | all songs id |
    //
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( musicCollection.iUid );
    path->AppendL( 0 );  // MAGIC, all songs, cannot have dependency to plugin in MPXMusicPlayer

    return path;
    }

// ---------------------------------------------------------------------------
// Returns the file name used for virtual playlist handling
// ---------------------------------------------------------------------------
//
HBufC* CMPXCollectionUiHelperImp::ExternalCollectionPathHandlingFileNameLC()
    {
    MPX_FUNC( "CMPXCollectionUiHelperImp::ExternalCollectionPathHandlingFileNameLC" );
    return KPathVirtualPlaylistHandling().AllocLC();
    }

// ---------------------------------------------------------------------------
// Cancel current op
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::Cancel()
    {
    if( iTask == ETaskRemoveMedia )
        {
        iDeleteHelper->Cancel();
        }
    else
        {
        // currently only used by incremental add
        iTaskQueue->CancelRequests();
        
        if( iTask == ETaskIncAddMedia || iTask == ETaskIncAppendMedia )
            {
            // complete task from scheduler
            iTaskQueue->CompleteTask();
            }
        
        // clean up iInputMedia
        if( iInputMedia )
            {
            delete iInputMedia;
            iInputMedia = NULL;
            }
        }
    }
    
// ---------------------------------------------------------------------------
// Frees this object
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::Close()
    {
    if ( iModeId == KMcModeDefault )
        {
        ASSERT( iRefCount > 0 );
        if ( --iRefCount == 0 )
            {
            // last client released
            CMPXCollectionUiHelperImp* s = reinterpret_cast<CMPXCollectionUiHelperImp*>( Dll::Tls() );
            if ( s && s == this )
                {
                delete this;
                Dll::SetTls( NULL );
                }
            }
        }
    else
        {
        delete this;
        }
    }

// ---------------------------------------------------------------------------
// Test if the specified title already exists for the category
// ---------------------------------------------------------------------------
//
TBool CMPXCollectionUiHelperImp::TitleExistsL(TMPXGeneralCategory aCategory, 
                                              const TDesC& aTitle,
                                              TMPXGeneralType aType)
    {
    CMPXMedia* query = CMPXMedia::NewL();
    CleanupStack::PushL(query);
    query->SetTObjectValueL(KMPXMediaGeneralType, aType);
    query->SetTObjectValueL(KMPXMediaGeneralCategory, aCategory);
    query->SetTextValueL(KMPXMediaGeneralTitle, aTitle);

    // Look up collection UID and set to criteria
    //
    RArray<TUid> ary;
    CleanupClosePushL( ary );
    ary.AppendL( TUid::Uid(EMPXCollectionPluginMusic) );
    TUid musicCollection = iCollection->CollectionIDL( ary.Array() );
    CleanupStack::PopAndDestroy( &ary );

    query->SetTObjectValueL<TUid>( KMPXMediaGeneralCollectionId, musicCollection );

    RArray<TMPXAttribute> attributes;
    CleanupClosePushL(attributes);
    attributes.AppendL(KMPXMediaGeneralId);

    CMPXMedia* result =
        iCollection->Collection().FindAllL(*query, attributes.Array());
    CleanupStack::PopAndDestroy(&attributes);
    CleanupStack::PopAndDestroy(query);
    CleanupStack::PushL(result);

    TInt count = result->ValueTObjectL<TInt>(KMPXMediaArrayCount);
    CleanupStack::PopAndDestroy(result);

    return count;
    }

// ---------------------------------------------------------------------------
// Updates the harvester with the up to date collection <-> file mapping
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::HandleMediatorPathUpdatedL( CMPXMedia*& aProperty,
                                                      TUid  /*aOldPath*/ )
    {
    iHarvester->UpdateFileL( aProperty );
    }

// ---------------------------------------------------------------------------
// Handles completion of playlist export
// to-do: this should be changed to HandlePlaylistExportCompleted
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::HandlePlaylistExportCompletedL( CMPXMedia* aMedia, TInt aErr )
    {
    TRAPD(error, DoHandlePlaylistExportCompletedL(aMedia, aErr));

    CompleteTask(iTask, aErr==KErrNone?error:aErr);
    }

// ---------------------------------------------------------------------------
// Handles completion of playlist export
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::DoHandlePlaylistExportCompletedL( CMPXMedia* aMedia, TInt aErr )
    {
    if (aErr == KErrNone)
        {
        CleanupStack::PushL(aMedia);
        iMedia->SetTextValueL(
            KMPXMediaGeneralUri, aMedia->ValueText(KMPXMediaGeneralUri));
        CleanupStack::PopAndDestroy(aMedia);
        }
    }

// ---------------------------------------------------------------------------
// Handles completion of playlist import
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::HandlePlaylistImportCompletedL( CMPXMedia* aMedia, TInt aErr )
    {
    if (aErr == KErrNone)
        {
        delete aMedia;
        }
    // Should never come here
    ASSERT(0);
    }

// ---------------------------------------------------------------------------
// Handle asynchronous file addition by file name
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::HandleFileAddCompletedL( CMPXMedia* aMedia, TInt aErr )
    {
    // to-do
    if (aErr == KErrNone)
        {
        delete aMedia;
        }
    // Should never come here
    ASSERT(0);
    }

// ---------------------------------------------------------------------------
// Handle file delete completion
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::HandleDeleteCompletedL( TInt /*aErr*/ )
    {
    }

// ---------------------------------------------------------------------------
// From MMPXHarvesterUtilityObserver
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::HandleFileImportCompletedL( CMPXMedia* aMedia,
                                                           TInt aErr  )
    {
    // Only do this if we are embedded mode
    //
    CleanupStack::PushL( aMedia );
    if( iEmbeddedModeID.iUid )
        {
        TInt err( aErr );
        TMPXGeneralCategory cat( EMPXNoCategory );
        if( aErr == KErrNone )
            {
            TRAP( err, OpenL( iEmbeddedModeID,
                              *aMedia,
                              (MMPXCHelperEmbeddedOpenObserver*)iOpenObserver ,
                              iEmbeddedPluginInfo ) );
            cat = aMedia->ValueTObjectL<TMPXGeneralCategory>(KMPXMediaGeneralCategory);
            }

        // Callback to observer for err handling
        ((MMPXCHelperEmbeddedOpenObserver*)iOpenObserver)->HandleEmbeddedOpenL( err, cat );
        }
    CleanupStack::PopAndDestroy( aMedia );
    }

// ---------------------------------------------------------------------------
// From MMPXHarvesterUtilityObserver
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::HandleFileGetMediaCompletedL( CMPXMedia* /*aMedia*/,
                                                              TInt /*aErr*/ )
    {
    // Not used
    ASSERT(0);
    }

// ---------------------------------------------------------------------------
// From MMPXCollectionMediaObserver
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::HandleCollectionMediaL(const CMPXMedia& /*aMedia*/,
                                                       TInt /*aError*/)
    {
    // Not used
    ASSERT(0);
    }

// ---------------------------------------------------------------------------
// From MMPXCollectionObserver
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::HandleCollectionMessage(CMPXMessage* aMsg, TInt aErr)
    {
    iDeleteHelper->HandleCollectionMessage( aMsg, aErr );
    }

// ---------------------------------------------------------------------------
// From MMPXCollectionObserver
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::HandleOpenL(const CMPXMedia& /*aEntries*/, TInt /*aIndex*/, TBool /*aComplete*/, TInt /*aError*/)
    {
    // Not used
    ASSERT(0);
    }

// ---------------------------------------------------------------------------
// From MMPXCollectionObserver
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::HandleOpenL(const CMPXCollectionPlaylist& /*aPlaylist*/, TInt /*aError*/)
    {
    // Not used
    ASSERT(0);
    }

// ---------------------------------------------------------------------------
// From MMPXCollectionObserver
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::HandleCommandComplete(CMPXCommand* aCommandResult, TInt aError)
    {
    MPX_DEBUG3("CMPXCollectionUiHelperImp::HandleCommandComplete iTask=%d, aError=%d", 
        iTask, aError);

    if( iTask == ETaskIncAddMedia && iInputMedia )
        {
        if ( iChunkNumber == 0 )
            {
            // save playlistId in input media & use it for subsequent appending operations
            if( aCommandResult )
                {
                TMPXItemId playlistId = 
                    aCommandResult->ValueTObjectL<TMPXItemId>(KMPXCommandColAddRtnId);
                    
                iInputMedia->SetTObjectValueL<TMPXItemId>(KMPXMediaGeneralId, playlistId);
                }
            
            iChunkNumber++;  // move on to next chunk
            
            CompleteTask(iTask, aError);
            }
        else if ( iChunkNumber == iTotalChunkNumber-1 )  // last chunk
            {
            CompleteTask(ETaskIncFinish, aError);  // finish inc task
            }
        else // intermedia chunks
            {
            iChunkNumber++;  // move on to next chunk
            
            CompleteTask(iTask, aError);
            }
        }
    else if( iTask == ETaskIncAppendMedia && iInputMedia )
        {
        // last chunk
        // for the case that there is only one chunk (first & last chunk at the same 
        // time), Inc Add is not used
        if( iChunkNumber == iTotalChunkNumber-1 )
            {
            // update input media as well
            FillInPlaylistDetailsL(*iInputMedia);
            CompleteTask(ETaskIncFinish, aError);        
            }
        else // intermediate chunks, including first chunk
            {       
            iChunkNumber++;
            CompleteTask(iTask, aError);
            }
        }
    }

// ----------------------------------------------------------------------------
// Handles completion of moving an object, just a proxy
// ----------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::HandleMoveCompleteL( TInt aErr )
    {
    if( iHelperObserver )
        {
        iHelperObserver->HandleOperationCompleteL( EMoveOp, aErr, NULL );
        }
    }

// ----------------------------------------------------------------------------
// Handles completion of removing a collection path
// ----------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::HandleOperationCompleteL( TCHelperOperation aOperation,
                                                          TInt aErr,
                                                          void* aArgument )
    {
    if (aOperation == EDeleteOp && iTask == ETaskRemoveMedia)
        {
        CompleteTask(iTask, aErr);
        }
    else if ( aOperation == EDeleteStatusOp )
        {
        iHelperObserver->HandleOperationCompleteL( aOperation, aErr, aArgument );
        }
    }

// ----------------------------------------------------------------------------
// Execute an async task
// ----------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::ExecuteTask(
    TInt      aTask,
    TInt      /*aParamData*/,
    TAny*     /*aPtrData*/,
    const     CBufBase& aBuf,
    TAny*     aCallback,
    CBase*    /*aCObject1*/,
    CBase*    /*aCObject2*/)
    {
    TRAPD(err, ExecuteTaskL(aTask, aBuf, aCallback));
    if (err != KErrNone)
        {
        CompleteTask(aTask, err);
        }
    }

// ----------------------------------------------------------------------------
// Indicates that a task was terminated with an error
// ----------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::HandleTaskError(
    TInt /* aTask */,
    TAny* /*aPtrData*/,
    TAny* /*aCallback*/,
    TInt /* aError */)
    {
    // Do nothing
    }

// ----------------------------------------------------------------------------
// Execute an async task. Leaves when an error is encountered.
// ----------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::ExecuteTaskL(TInt aTask, const CBufBase& aBuf, TAny* aCallback)
    {
    iTask = aTask;
    MPX_DEBUG2("CMPXCollectionUiHelperImp::ExecuteTaskL, aTask = %d", aTask);
    
    // cenrep key need to be checked whether USB cable is connected in MTP/Combined Mode
    // prevent any general add, general delete, and any moficiation of playlist
    if ((iTask == ETaskAddMedia)
            || (iTask == ETaskAppendMedia)
            || (iTask == ETaskIncAddMedia)
            || (iTask == ETaskIncAppendMedia)
            || (iTask == ETaskRemoveMedia)
            || (iTask == ETaskReorderPlaylist)
            || (iTask == ETaskRenameMedia))
        {
        TInt usbStatus;
        RProperty::Get(KPSUidUsbWatcher, KUsbWatcherSelectedPersonality, usbStatus);
        
        TInt mtpStatus = EMtpPSStatusUninitialized;
        RProperty::Get( KMtpPSUid, KMtpPSStatus, mtpStatus);

        MPX_DEBUG2("CMPXCollectionViewHgImp::ConstructL, mtpstatus = %d", mtpStatus);

        if ( (mtpStatus != EMtpPSStatusUninitialized)
                || (usbStatus == KUsbPersonalityIdMS) )
            {
            MPX_DEBUG1("MTP/USB is active, Leave with KErrLocked");
            // need to call back even if it leaves here
            iHelperObserver = (MMPXCHelperObserver*)aCallback;
            CompleteTask( aTask, KErrLocked );
            //User::Leave(KErrLocked);
            if ((iTaskQueue->Task() != ETaskIncAddMedia)&&(iTaskQueue->Task() != ETaskIncAppendMedia))
                {
                iIncAdding = EFalse;
                }
            return;
            }
        }
    
    switch( aTask )
        {
        case ETaskAddMedia:  // fall through
        case ETaskAppendMedia:
            {
            delete iMedia;
            iMedia = NULL;
            ::NewFromBufferL<CMPXMedia>( aBuf, iMedia );
            iHelperObserver = (MMPXCHelperObserver*)aCallback;
            DoAddL();
            }
            break;
            
        case ETaskIncAddMedia:
            {
            iHelperObserver = (MMPXCHelperObserver*)aCallback;
            DoIncAddMediaL();
            }
            break;
            
        case ETaskIncAppendMedia:
            {
            iHelperObserver = (MMPXCHelperObserver*)aCallback;
            DoIncAppendMediaL();
            }
            break;
                
        case ETaskSetMedia:
            {
            delete iMedia;
            iMedia = NULL;
            ::NewFromBufferL<CMPXMedia>( aBuf, iMedia );
            iHelperObserver = (MMPXCHelperObserver*)aCallback;
            DoSetL();
            }
            break;

        case ETaskRenameMedia:
            {
            delete iMedia;
            iMedia = NULL;
            ::NewFromBufferL<CMPXMedia>( aBuf, iMedia );
            iHelperObserver = (MMPXCHelperObserver*)aCallback;
            DoRenameL();
            }
            break;

        case ETaskRemoveMedia:
            {
            CMPXCollectionPath* path(NULL);
            ::NewFromBufferL<CMPXCollectionPath>( aBuf, path );
            CleanupStack::PushL(path);
            MPX_DEBUG_PATH(*path);

            iHelperObserver = (MMPXCHelperObserver*) aCallback;
            iDeleteHelper->DeleteL(*path);
            CleanupStack::PopAndDestroy(path);
            }
            break;

        case ETaskExportPlaylist:
            {
            delete iMedia;
            iMedia = NULL;

            CMPXMedia* params(NULL);
            ::NewFromBufferL<CMPXMedia>( aBuf, params );
            CleanupStack::PushL(params);

            iHelperObserver = (MMPXCHelperObserver*)aCallback;

            DoExportPlaylistL(
                params->ValueTObjectL<TMPXItemId>(KMPXMediaGeneralId),
                params->ValueText(KMPXMediaGeneralUri));

            CleanupStack::PopAndDestroy(params);
            }
            break;

        case ETaskReorderPlaylist:
            {
            delete iMedia;
            iMedia = NULL;

            CMPXCommand* params(NULL);
            ::NewFromBufferL<CMPXCommand>( aBuf, params );
            CleanupStack::PushL(params);

            iHelperObserver = (MMPXCHelperObserver*)aCallback;

            DoReorderPlaylistL(*params);

            CleanupStack::PopAndDestroy(params);
            }
            break;

        //
        // command not supported
        //
        default:
            {
            User::Leave( KErrNotSupported );
            }
        }
    }

// ----------------------------------------------------------------------------
// Completes the currently executing task
// ----------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::CompleteTask(TInt aTask, TInt aError)
    {
    MPX_FUNC("CMPXCollectionUiHelperImp::CompleteTask");

    MPX_DEBUG3("task %d, error %d", aTask, aError);

    TRAP_IGNORE(NotifyClientL(aTask, aError));

    if ( iTask != ETaskNone )
        {
        // clean up data set during processing of the current task
        Cleanup();

        // Complete current task and move on to the next
        iTaskQueue->CompleteTask();
        }
    }

// ----------------------------------------------------------------------------
// Notifies the client of the completion of a collection helper request
// ----------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::NotifyClientL(TInt aTask, TInt aError)
    {
    MPX_DEBUG3("CMPXCollectionUiHelperImp::NotifyClientL(task %d, error %d)", aTask, aError);

    switch (aTask)
        {
        case ETaskAddMedia:  // fall through
        case ETaskAppendMedia:
            {
            if ( aError >= KErrNone )
                {
                // make a copy of iMedia to hand over to the client which takes ownership
                // of the copied media. if we failed to make a copy, client is not notified.
                iHelperObserver->HandleOperationCompleteL(EAddOp,
                                                          KErrNone,
                                                          CMPXMedia::NewL(*iMedia) );
                }
            else
                {
                iHelperObserver->HandleOperationCompleteL( EAddOp,
                                                           aError,
                                                           NULL );
                }
            }
            break;
        case ETaskIncAddMedia:
        case ETaskIncAppendMedia: 
            {
            if ( KErrLocked == aError )
                {
                if( EFalse == iIncAdding )
                    {
                    iIncAdding = ETrue;
                    iHelperObserver->HandleOperationCompleteL( EAddOp, aError, NULL );
                    }
                }
            }
            break;
        case ETaskIncFinish:
            {
            if ( aError >= KErrNone )
                {
                // input media is handed over to observer
                iHelperObserver->HandleOperationCompleteL(EAddOp, 
                                                          KErrNone, 
                                                          CMPXMedia::NewL(*iInputMedia) );                            
                }
            else
                {
                iHelperObserver->HandleOperationCompleteL( EAddOp,
                                                           aError,
                                                           NULL );
                }
            // clean up input media
            delete iInputMedia;
            iInputMedia = NULL;
            }
            break;
            
        case ETaskSetMedia:
            {
            if ( aError >= KErrNone )
                {
                // make a copy of iMedia to hand over to the client which takes ownership
                // of the copied media. if we failed to make a copy, client is not notified.
                iHelperObserver->HandleOperationCompleteL( ESetOp,
                                                           KErrNone,
                                                           CMPXMedia::NewL(*iMedia) );
                }
            else
                {
                iHelperObserver->HandleOperationCompleteL( ESetOp,
                                                           aError,
                                                           NULL );
                }
            }
            break;

        case ETaskRenameMedia:
            {
            if ( aError >= KErrNone )
                {
                // make a copy of iMedia to hand over to the client which takes ownership
                // of the copied media. if we failed to make a copy, client is not notified.
                iHelperObserver->HandleOperationCompleteL( ERenameOp,
                                                           KErrNone,
                                                           CMPXMedia::NewL(*iMedia) );
                }
            else
                {
                iHelperObserver->HandleOperationCompleteL( ERenameOp,
                                                           aError,
                                                           NULL );
                }
            }
            break;

        case ETaskRemoveMedia:
            {
            iHelperObserver->HandleOperationCompleteL( EDeleteOp,
                                                       aError >= KErrNone? KErrNone : aError,
                                                       NULL );
            }
            break;

        case ETaskExportPlaylist:
            {
            HBufC* buf( NULL );
            if( aError == KErrNone )
                {
                const TDesC& uri =
                    iMedia->ValueText(KMPXMediaGeneralUri);
                buf = uri.AllocL();
                }

            // client assumes ownership of HBufC copied from title
            iHelperObserver->HandleOperationCompleteL(EExportPlaylistOp, aError, buf);
            }
            break;

        case ETaskReorderPlaylist:
            {
            iHelperObserver->HandleOperationCompleteL( EReorderPlaylistOp,
                                                       aError >= KErrNone? KErrNone : aError,
                                                       NULL );
            }
            break;

        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// Cleanup async task variables
// ----------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::Cleanup()
    {
    MPX_DEBUG1("-->CMPXCollectionUiHelperImp::Cleanup");
    iTask = ETaskNone;
    iHelperObserver = NULL;
    delete iMedia;
    iMedia = NULL;
    MPX_DEBUG1("<--CMPXCollectionUiHelperImp::Cleanup");
    }

// ---------------------------------------------------------------------------
// Add a Media to the collection
//
// For playlists:
// 1) add to collection
// 2) when successfully added to collection, AddItemL returns; otherwise,
//    AddItemL leaves.
// 3) complete task upon successfully addition to the collection. If a leave
//    occurs, it's trapped by ExecuteTask and task is completed by ExecuteTask
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::DoAddL()
    {
    // creating a new playlist
    if (iTask == ETaskAddMedia)
        {
        // find the collection Id
        HBufC* playlistExtension = PlaylistFileExtensionLC();

        TInt collectionId = FindCollectionIdL(*playlistExtension);

        // Add to collection, make sure we set the collection ID.
        iMedia->SetTObjectValueL<TUid>(
            KMPXMediaGeneralCollectionId, TUid::Uid( collectionId ));

        CleanupStack::PopAndDestroy(playlistExtension);
        }

    // adding/appending tracks to a saved playlist
    else
        {
        FillInPlaylistDetailsL(*iMedia);
        }

    // when playlist is successfully added to the collection, its
    // KMPXMediaGeneralId is automatically updated by mediator
    iMediator->AddItemL( iMedia );
    
    CompleteTask(iTask, KErrNone);
    }

// ---------------------------------------------------------------------------
// Add media to the collection incrementally
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::DoIncAddMediaL()
    {
    /***** include only aSize/iChunkSize number of songs *****/
    MPX_DEBUG5("CMPXCollectionUiHelperImp::DoIncAddMediaL (%d, %d, %d, %d)", 
        iChunkNumber, iChunkSize, iTotalChunkNumber, iRemainder);
    // copy media
    CMPXMedia* media = CMPXMedia::CopyL(*iInputMedia);
    CleanupStack::PushL(media);
    
    CMPXMediaArray* tracksArray = iInputMedia->Value<CMPXMediaArray>( KMPXMediaArrayContents );  
    
    CMPXMediaArray* cArray = CMPXMediaArray::NewL();
    CleanupStack::PushL(cArray);
       
    // last interation/chunk
    // for the case when aSize/iChunkSize evenly devides number of songs, the else clause takes care of it
    if( (iChunkNumber == iTotalChunkNumber-1) && iRemainder != 0 )
        {
        // add remaining songs to the array
        for(int j = 0; j < iRemainder; j++ )
            {
            cArray->AppendL(*tracksArray->AtL(iArrayIndex));
            iArrayIndex++;
            }                   
        }
    else
        {
        // adding next aSize/iChunkSize elements
        for(int j = 0; j < iChunkSize; j++)
            {
            cArray->AppendL(*tracksArray->AtL(iArrayIndex));
            iArrayIndex++;
            }
        }
    
    // update media w/ new array
    media->SetCObjectValueL(KMPXMediaArrayContents, cArray);
    media->SetTObjectValueL(KMPXMediaArrayCount, cArray->Count() );

    
    /***** logic to see which chunk this code is in *****/
    
    // first chunk
    // for the case that there is only one chunk (first & last chunk at the same 
    // time), Inc Add is not used
    if ( iChunkNumber == 0 )
        {
        // find the collection Id
        HBufC* playlistExtension = PlaylistFileExtensionLC();
        
        TInt collectionId = FindCollectionIdL(*playlistExtension);                
    
        // Add to collection, make sure we set the collection ID.
        media->SetTObjectValueL<TUid>(
            KMPXMediaGeneralCollectionId, TUid::Uid( collectionId ));
        
        // save collectionId for adding subsequent chunks
        iInputMedia->SetTObjectValueL<TUid>(
                KMPXMediaGeneralCollectionId, TUid::Uid( collectionId ));
        
        CleanupStack::PopAndDestroy(playlistExtension); 

        iMediator->AddItemAsyncL( media );  // this creates the new playlist
        }
    else if ( iChunkNumber == iTotalChunkNumber-1 )  // last chunk
        {
        // get saved collection id from input media & set it in current media
        TUid id = iInputMedia->ValueTObjectL<TUid>(KMPXMediaGeneralCollectionId);
        media->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, id);
 
        // get playlistId from input media & set it in current media
        TMPXItemId playlistId = iInputMedia->ValueTObjectL<TMPXItemId>(KMPXMediaGeneralId);
        media->SetTObjectValueL<TMPXItemId>(KMPXMediaGeneralId, playlistId);
        
        iMediator->AddItemAsyncL( media );
        }
    else // intermedia chunks
        {
        // get saved collection id from input media & set it in current media
        TUid id = iInputMedia->ValueTObjectL<TUid>(KMPXMediaGeneralCollectionId);
        media->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, id);
        
        // get playlistId from original media & set it in current media
        TMPXItemId playlistId = iInputMedia->ValueTObjectL<TMPXItemId>(KMPXMediaGeneralId);
        media->SetTObjectValueL<TMPXItemId>(KMPXMediaGeneralId, playlistId);

        iMediator->AddItemAsyncL( media );
        }
    
    CleanupStack::PopAndDestroy(cArray);
    CleanupStack::PopAndDestroy(media);
    }

// ---------------------------------------------------------------------------
// Append media to the collection incrementally
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::DoIncAppendMediaL()
    {
    /***** include only aSize/iChunkSize number of songs *****/

    // copy media
    CMPXMedia* media = CMPXMedia::CopyL(*iInputMedia);
    CleanupStack::PushL(media);
    
    CMPXMediaArray* tracksArray = iInputMedia->Value<CMPXMediaArray>( KMPXMediaArrayContents );  
    
    CMPXMediaArray* cArray = CMPXMediaArray::NewL();
    CleanupStack::PushL(cArray);
       
    // last interation/chunk
    // for the case when aSize/iChunkSize evenly devides number of tracks, the else clause takes care of it
    if( (iChunkNumber == iTotalChunkNumber-1) && iRemainder != 0 )
        {
        for(int j = 0; j < iRemainder; j++ )
            {
            cArray->AppendL(*tracksArray->AtL(iArrayIndex));
            iArrayIndex++;
            }                   
        }
    else
        {
        // adding next aSize/iChunkSize elements
        for(int j = 0; j < iChunkSize; j++)
            {
            cArray->AppendL(*tracksArray->AtL(iArrayIndex));
            iArrayIndex++;
            }
        }
    
    // update media w/ new array
    media->SetCObjectValueL(KMPXMediaArrayContents, cArray);
    media->SetTObjectValueL(KMPXMediaArrayCount, cArray->Count() );
   
    
    /***** logic to see which chunk this code is in *****/

    // update media then append to playlist
    FillInPlaylistDetailsL(*media);
    iMediator->AddItemAsyncL( media );

    CleanupStack::PopAndDestroy(cArray);
    CleanupStack::PopAndDestroy(media);
    }

// ---------------------------------------------------------------------------
// Update a media in the collection
//
// For updating (overwriting) a playlist, or updating a song/artist/album/genre/
// composer:
// 1) update collection
// 2) when successfully updated the collection, SetItemL is returned; otherwise
//    a leave occurs.
// 3) complete task upon successfully updating the collection. If a leave
//    occurs, it's trapped by ExecuteTask and task is completed by ExecuteTask
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::DoSetL()
    {
    MPX_FUNC("CMPXCollectionUiHelperImp::DoSetL");

    // fill in playlist details
    TMPXGeneralCategory category =
        iMedia->ValueTObjectL<TMPXGeneralCategory>(KMPXMediaGeneralCategory);
    if ( category == EMPXPlaylist )
        {
        FillInPlaylistDetailsL(*iMedia);
        }

    // find the collection Id
    if (!iMedia->IsSupported(KMPXMediaGeneralCollectionId) &&
        iMedia->IsSupported(KMPXMediaGeneralUri))
        {
        TInt collectionId =
            FindCollectionIdL(iMedia->ValueText(KMPXMediaGeneralUri));

        if (collectionId)
            {
            iMedia->SetTObjectValueL<TUid>(
                KMPXMediaGeneralCollectionId , TUid::Uid(collectionId));
            }
        }

    // update collection.
    iMediator->SetItemL( iMedia );

    CompleteTask(iTask, KErrNone);
    }

// ---------------------------------------------------------------------------
// Rename a media in the collection
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::DoRenameL()
    {
    // find the collection Id of the playlist from playlist Id
    // iMedia isn't used as the search criteria because it
    // contains the new title of the playlist; calling
    // FillInPlaylistDetailsL(*iMedia) will result in playlist
    // not found.
    if (!iMedia->IsSupported(KMPXMediaGeneralCollectionId))
        {
        CMPXMedia* media = CMPXMedia::NewL();
        CleanupStack::PushL(media);

        media->SetTObjectValueL(KMPXMediaGeneralType, EMPXItem);
        media->SetTObjectValueL(KMPXMediaGeneralCategory,EMPXPlaylist);
        media->SetTObjectValueL(KMPXMediaGeneralId, iMedia->ValueTObjectL<TMPXItemId>(KMPXMediaGeneralId));

        FillInPlaylistDetailsL(*media);

        if (media->IsSupported(KMPXMediaGeneralCollectionId))
            {
            iMedia->SetTObjectValueL<TUid>(
                KMPXMediaGeneralCollectionId, media->ValueTObjectL<TUid>(KMPXMediaGeneralCollectionId));
            }
        else
            {
            User::Leave(KErrNotFound);
            }
        CleanupStack::PopAndDestroy(media);
        }

    // Update collection via command
    //
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL( KMPXCommandGeneralId, KMPXCommandIdCollectionSet );
    cmd->SetTObjectValueL( KMPXCommandGeneralDoSync, ETrue );
    TUid colId = iMedia->ValueTObjectL<TUid>(KMPXMediaGeneralCollectionId);
    cmd->SetTObjectValueL( KMPXCommandGeneralCollectionId, colId.iUid );
    cmd->SetCObjectValueL<CMPXMedia>( KMPXCommandColSetMedia, iMedia );

    iCollection->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );

    CompleteTask(iTask, KErrNone);
    }

// ---------------------------------------------------------------------------
// Export a playlist
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::DoExportPlaylistL(TMPXItemId aPlaylistId,
                                                  const TDesC& aDestinationDriveAndPath)
    {
    //
    // fill in playlist info
    //
    iMedia = CMPXMedia::NewL();

    iMedia->SetTObjectValueL(KMPXMediaGeneralType, EMPXItem);
    iMedia->SetTObjectValueL(KMPXMediaGeneralCategory, EMPXPlaylist);
    iMedia->SetTObjectValueL<TMPXItemId>(KMPXMediaGeneralId, aPlaylistId);

    FillInPlaylistDetailsL(*iMedia);

    //
    // find songs that are in the specified playlist
    //
    CMPXMedia* songQuery = CMPXMedia::NewL();
    CleanupStack::PushL(songQuery);
    songQuery->SetTObjectValueL(KMPXMediaGeneralType, EMPXGroup);
    songQuery->SetTObjectValueL(KMPXMediaGeneralCategory, EMPXSong);
    songQuery->SetTObjectValueL(KMPXMediaGeneralId, aPlaylistId);

    RArray<TMPXAttribute> songAttributes;
    CleanupClosePushL(songAttributes);
    iHarvester->RequiredAttributesL( EMPXPlaylistTypeM3U, songAttributes );
    songAttributes.AppendL(KMPXMediaGeneralTitle);

    CMPXMedia* songQueryResult =
        iCollection->Collection().FindAllL(*songQuery, songAttributes.Array());
    CleanupStack::PopAndDestroy(&songAttributes);
    CleanupStack::PopAndDestroy(songQuery);
    CleanupStack::PushL(songQueryResult);

    // medias contains tracks currently in the playlist
    const CMPXMediaArray* medias =
        songQueryResult->Value<CMPXMediaArray>(KMPXMediaArrayContents);
    if( !medias )
        {
        User::Leave( KErrNoMemory );
        }

    // add media array to iMedia
    iMedia->SetCObjectValueL(
        KMPXMediaArrayContents, const_cast<CMPXMediaArray*>(medias));
    iMedia->SetTObjectValueL(
        KMPXMediaArrayCount, medias->Count());

    CleanupStack::PopAndDestroy(songQueryResult);

    // export playlist, default to M3U file type. When ExportPlaylistL completes,
    // HandlePlaylistExportCompletedL will be called
    iHarvester->ExportPlaylistL( *iMedia, aDestinationDriveAndPath, EMPXPlaylistTypeM3U, this );
    }

// ---------------------------------------------------------------------------
// Reorder a song in a playlist
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::DoReorderPlaylistL(CMPXCommand& aCommand)
    {
    // Update collection via command
    //
    aCommand.SetTObjectValueL( KMPXCommandGeneralId, KMPXCommandIdReorderPlaylist );
    aCommand.SetTObjectValueL( KMPXCommandGeneralDoSync, ETrue );

    iCollection->Collection().CommandL( aCommand );

    CompleteTask(iTask, KErrNone);
    }

// ---------------------------------------------------------------------------
// retrieve information for the required attributes
// ---------------------------------------------------------------------------
//
void CMPXCollectionUiHelperImp::FillInPlaylistDetailsL(CMPXMedia& aMedia)
    {
    MPX_FUNC("CMPXCollectionUiHelperImp::FillInPlaylistDetailsL");

    //
    // retrieve info about the playlist itself
    //
    RArray<TMPXAttribute> playlistAttributes;
    CleanupClosePushL(playlistAttributes);
    playlistAttributes.AppendL(KMPXMediaGeneralId);
    playlistAttributes.AppendL(KMPXMediaGeneralTitle);
    playlistAttributes.AppendL(KMPXMediaGeneralUri);
    playlistAttributes.AppendL(KMPXMediaGeneralCollectionId);

    CMPXMedia* playlistSearchResult =
        iCollection->Collection().FindAllL(aMedia, playlistAttributes.Array());
    CleanupStack::PopAndDestroy(&playlistAttributes);
    CleanupStack::PushL(playlistSearchResult);

    const CMPXMediaArray* results =
        playlistSearchResult->Value<CMPXMediaArray>(KMPXMediaArrayContents);
    if( !results )
        {
        User::Leave( KErrNoMemory );
        }

    if ( results->Count() != 1 )
        {
        User::Leave(KErrArgument);
        }

    MPX_DEBUG1("playlist found");

    aMedia.SetTObjectValueL(
        KMPXMediaGeneralId, results->AtL(0)->ValueTObjectL<TMPXItemId>(KMPXMediaGeneralId));
    aMedia.SetTextValueL(
        KMPXMediaGeneralTitle, results->AtL(0)->ValueText(KMPXMediaGeneralTitle));
    aMedia.SetTextValueL(
        KMPXMediaGeneralUri, results->AtL(0)->ValueText(KMPXMediaGeneralUri));
    aMedia.SetTObjectValueL(
        KMPXMediaGeneralCollectionId, results->AtL(0)->ValueTObjectL<TUid>(KMPXMediaGeneralCollectionId));

    CleanupStack::PopAndDestroy(playlistSearchResult);
    }

// ---------------------------------------------------------------------------
// retrieve collection from URI
// ResolvePlugin should be able to resolve the plugin without
// the client looking for the collection Id
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionUiHelperImp::FindCollectionIdL(const TDesC& aUri)
    {
    MPX_FUNC("CMPXCollectionUiHelperImp::FindCollectionIdL");

    TInt collectionId(KErrNotFound);

    TParsePtrC parser( aUri );

    RPointerArray<CMPXCollectionType> collectionType;
    iCollection->Collection().GetSupportedTypesL(collectionType);

    TInt index(KErrNotFound);
    TInt count( collectionType.Count() );

    for (TInt i = 0; i < count; i++)
        {
        const CDesCArray& extensions = collectionType[i]->Extensions();

        if (extensions.FindIsq(parser.Ext(), index) == 0)
            {
            collectionId = collectionType[i]->Uid().iUid;
            break;
            }
        }

    collectionType.ResetAndDestroy();

    MPX_DEBUG3("Uid 0x%x for %S", collectionId, &aUri);

    return collectionId;
    }

// End of file
