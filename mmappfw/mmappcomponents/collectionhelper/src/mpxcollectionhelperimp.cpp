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
* Description:  Main collection helper interface
*
*/


#include <e32base.h>
#include <eikenv.h>
#include <badesca.h>
#include <mpxcollectionpath.h>
#include <mpxmedia.h>
#include <mpxmediaarray.h>
#include <mpxcollectionutility.h>
#include <mpxharvesterutility.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediamusicdefs.h>
#include <mpxmediacontainerdefs.h>
#include <mpxuser.h>
#include <mpxcollectionplugin.hrh>
#include <mpxcommandgeneraldefs.h>
#include <mpxcollectioncommanddefs.h>

#include <mpxlog.h>
#include "mpxcollectionhelperfactory.h"
#include "mpxcollectionhelperimp.h"
#include "mpxcollectionhelpercommon.h"
#include "mpxcollectionmediator.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CMPXCollectionHelperImp::CMPXCollectionHelperImp()
    : iTNManager(NULL)
    {
    }


// ---------------------------------------------------------------------------
// 2nd Phased Constructor
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::ConstructL()
    {
    iHvsUtility = CMPXHarvesterFactory::NewL();
    iCollectionUtil = MMPXCollectionUtility::NewL();
    iMediator = CMPXCollectionMediator::NewL( iCollectionUtil->Collection(),
                                              this );
    
    RArray<TUid> ary;
    CleanupClosePushL( ary );
    ary.AppendL( TUid::Uid(EMPXCollectionPluginMusic) );
    iMusicCollectionId = iCollectionUtil->CollectionIDL( ary.Array() );
    CleanupStack::PopAndDestroy( &ary );
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXCollectionHelperImp* CMPXCollectionHelperImp::NewL()
    {
    CMPXCollectionHelperImp* self = new( ELeave ) CMPXCollectionHelperImp();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXCollectionHelperImp* CMPXCollectionHelperImp::NewLC()
    {
    CMPXCollectionHelperImp* self = new( ELeave ) CMPXCollectionHelperImp();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Virtual Destructor
// ---------------------------------------------------------------------------
//
CMPXCollectionHelperImp::~CMPXCollectionHelperImp()
    {
    if( iHvsUtility )
        {
        iHvsUtility->Close();
        }

    if( iCollectionUtil )
        {
        iCollectionUtil->Close();
        }
    delete iMediator;
    
    delete iTNManager;
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
// Add a File to the collection
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::AddL( const TDesC& aFile,
                                    MMPXCollectionHelperObserver* aObserver )
    {
    MPX_FUNC("CMPXCollectionHelperImp::AddL");
    MPX_DEBUG2("aFile %S", &aFile);

    // Since this is a file, we must get the harvester to parse the file
    // On callback, add the file to collection
    //
    iObserver = aObserver;
    iHvsUtility->GetMediaForFileL(aFile,this);
    }

// ---------------------------------------------------------------------------
// Add a Media Property to the collection
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::AddL( CMPXMedia* aMedia )
    {
    MPX_DEBUG1("CMPXCollectionHelperImp::AddL <--");

    //
    // leave if the given media doesn't contain the following attributes
    //
    if (!aMedia->IsSupported(KMPXMediaGeneralType) ||
        !aMedia->IsSupported(KMPXMediaGeneralCategory) ||
        !aMedia->IsSupported(KMPXMediaGeneralUri))
        {
        User::Leave( KErrArgument );
        }

    const TDesC& path = aMedia->ValueText(KMPXMediaGeneralUri);

    //
    // For now, the default collection to add this playlist is
    // EMPXCollectionPluginMusic. This will be revised later when there is a
    // generic way of resolving collections aside from using file extension
    // or UID.
    //
#ifdef ABSTRACTAUDIOALBUM_INCLUDED
    if ( aMedia->ValueTObjectL<TMPXGeneralType>(KMPXMediaGeneralType) == EMPXItem &&
         (aMedia->ValueTObjectL<TMPXGeneralCategory>(KMPXMediaGeneralCategory) == EMPXPlaylist ||
         aMedia->ValueTObjectL<TMPXGeneralCategory>(KMPXMediaGeneralCategory) == EMPXAbstractAlbum))
#else
    if ( aMedia->ValueTObjectL<TMPXGeneralType>(KMPXMediaGeneralType) == EMPXItem &&
         aMedia->ValueTObjectL<TMPXGeneralCategory>(KMPXMediaGeneralCategory) == EMPXPlaylist )
#endif
        {
        aMedia->SetTObjectValueL<TUid>( KMPXMediaGeneralCollectionId,
                                        iMusicCollectionId );
        }

    //
    // Adding a song
    //
    else
        {
        TInt colUid = iHvsUtility->GetColUidForFileL(path);

        MPX_DEBUG2("CMPXCollectionHelperImp::AddL: collection id = %i",colUid);
        // Add to collection, make sure we set the collection ID
        //
        aMedia->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId,
                                       TUid::Uid( colUid ) );
        }

    MPX_DEBUG1("CMPXCollectionHelperImp::AddL: calling DoAddToCollectionL( aMedia )");
    TRAPD(err,DoAddToCollectionL( aMedia ));
    MPX_DEBUG2("CMPXCollectionHelperImp::AddL: DoAddToCollectionL( aMedia ); called error %d", err);

    //
    // When MTP adds a playlist, the playlist content is represented by the
    // given CMPXMedia and a 0-length .pla file is added to the file system
    // for syncing with WMP. The URI of the playlist CMPXMedia will contain
    // .pla extension. This file will not be added to the harvester database
    // because there is no pla playlist plugin supplied and harvester does
    // not scan .pla files. It is intentional not to have a pla playlist
    // plugin to prevent users from transferring real .pla files to the
    // device's file system through USB and have them shown with music icons
    // when they cannot be understood by the system.
    //
    if (err == KErrNone &&
        aMedia->ValueTObjectL<TMPXGeneralCategory>(KMPXMediaGeneralCategory) != EMPXPlaylist)
        {
        // Add to harvester
        iHvsUtility->AddFileL(aMedia);
        MPX_DEBUG2("    successfully registered %S with harvester", &path);
        }

    MPX_DEBUG1("CMPXCollectionHelperImp::AddL -->");
    }

// ---------------------------------------------------------------------------
// Remove a file from the collection
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::RemoveL( const TDesC& aFile,
                                       TMPXGeneralCategory aItemCat )
    {
    MPX_FUNC("CMPXCollectionHelperImp::RemoveL");
    MPX_DEBUG3("aFile %S, category %d", &aFile, aItemCat);

    if (aItemCat != EMPXSong && aItemCat != EMPXPlaylist)
        {
        User::Leave(KErrArgument);
        }

    //
    // ask harvester to remove the file if any of the following is true:
    // 1) removing a song
    // 2) removing a playlist scanned through file system.
    //
    //    There are 3 types of playlists. The ones created from the device
    //    are virtual playlists which have file extension .vir. There are
    //    no physical playlist files associated with them; hence not
    //    registered with harvester. For virtual playlists, IsPlaylistL
    //    will return EFalse because there is not a playlist plugin that
    //    deals with .vir playlist files.
    //
    //    The ones synced from PC through MTP have file extension .pla.
    //    There are 0-byte .pla files associated with them but they
    //    are not registered with harvester either. IsPlaylistL will also
    //    return EFalse because there is not a playlist plugin that deals
    //    with .pla.
    //
    //    The 3rd type of playlists is .m3u on the file system. They are
    //    added to the collection through file scan and registered with
    //    harvester. IsPlaylistL will return ETrue.
    //
    //    For now virtual playlists and synced playlists are assumed to
    //    be in the music collection for now until there is a generic way
    //    resolving collections aside from using file extension or UID.
    //
    TInt collection(iMusicCollectionId.iUid);
    if (aItemCat == EMPXSong || iHvsUtility->IsPlaylistL(aFile))
        {
        // Remove from the harvester
        collection = iHvsUtility->RemoveFileL( aFile );
        }

    // Remove from the collection
    // Construct a CMPXMedia object with URI, collection id, type and category
    //
    RArray<TInt> contID;
    CleanupClosePushL( contID );
    contID.AppendL( KMPXMediaIdGeneral );
    CMPXMedia* media = CMPXMedia::NewL( contID.Array() );
    CleanupStack::PushL( media );

    media->SetTextValueL( KMPXMediaGeneralUri, aFile );
    media->SetTObjectValueL<TUid>( KMPXMediaGeneralCollectionId, TUid::Uid( collection ) );
    media->SetTObjectValueL( KMPXMediaGeneralType, EMPXItem );
    media->SetTObjectValueL( KMPXMediaGeneralCategory, aItemCat );

    // Remove the item via CMPXCommand
    //
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL( KMPXCommandGeneralCollectionId,
                           collection );
    cmd->SetTObjectValueL( KMPXCommandGeneralId,
                           KMPXCommandIdCollectionRemoveMedia );
    cmd->SetTObjectValueL( KMPXCommandGeneralDoSync, ETrue );
    cmd->SetCObjectValueL( KMPXCommandCollectionRemoveMedia, media );
    cmd->SetTObjectValueL( KMPXCommandCollectionRemoveMediaDeleteRecord,
                           ETrue );
    iCollectionUtil->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );

    CleanupStack::PopAndDestroy( media );
    CleanupStack::PopAndDestroy( &contID );
    }

// ---------------------------------------------------------------------------
// Remove all files from harvester, music and podcast db
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::RemoveAllL()
    {
    MPX_FUNC("CMPXCollectionHelperImp::RemoveAllL");

    // Remove all from harvester
    iHvsUtility->RemoveAllFilesL();

    // Remove all from music collection
    iCollectionUtil->Collection().CommandL( EMcCmdRemoveAll,
                                            iMusicCollectionId.iUid );

    // Remove all from podcast collection
    //iCollectionUtil->Collection().CommandL( EMcCmdRemoveAll,
    //                                        KPodCastCollecionID);
    }

// ---------------------------------------------------------------------------
// Delete all records marked as deleted
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::CleanupDeletedMediasL()
    {
    User::Leave(KErrNotSupported);
    }

// ---------------------------------------------------------------------------
// update the media property based on file path
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::SetL( CMPXMedia*& aMedia )
    {
    MPX_FUNC("CMPXCollectionHelperImp::SetL");

    //
    // leave if the given media doesn't contain the following attributes
    //
    if (!aMedia->IsSupported(KMPXMediaGeneralType) ||
        !aMedia->IsSupported(KMPXMediaGeneralCategory) ||
        !aMedia->IsSupported(KMPXMediaGeneralUri))
        {
        User::Leave( KErrArgument );
        }

    //
    // leave if not updating a song or a playlist
    //
    TMPXGeneralCategory category =
        aMedia->ValueTObjectL<TMPXGeneralCategory>(KMPXMediaGeneralCategory);
#ifdef ABSTRACTAUDIOALBUM_INCLUDED
    if ((aMedia->ValueTObjectL<TMPXGeneralType>(KMPXMediaGeneralType) != EMPXItem) ||
        (category != EMPXSong && category != EMPXPlaylist && category != EMPXAbstractAlbum))
#else
    if ((aMedia->ValueTObjectL<TMPXGeneralType>(KMPXMediaGeneralType) != EMPXItem) ||
        (category != EMPXSong && category != EMPXPlaylist))
#endif
        {
        User::Leave( KErrArgument );
        }

    //
    // Playlists synced from MTP were not added to harvester's database;
    // hence we cannot look up the collection Id through harvester. These
    // playlists contain .pla extension when there is no pla playlist
    // plugin, so we cannot resolve the collection plugins by extensions
    // either. For now, the default collection to add this playlist is
    // EMPXCollectionPluginMusic for playlists. This will be revised later
    // when there is a generic way of resolving collections aside from using
    // file extension or UID. Same for virtual playlists.
    //
    // Fetch collection id from harvester for songs or scanned playlist
    // files.
    //
    TInt col(iMusicCollectionId.iUid);
    const TDesC& path = aMedia->ValueText(KMPXMediaGeneralUri);

    // Fetch collection id from harvester
    if (category == EMPXSong || iHvsUtility->IsPlaylistL(path))
        {
        col = iHvsUtility->FindCollectionIdL( path );
        }

    // Proxy to collection after we get the correct collection ID
    //
    aMedia->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId,
                                   TUid::Uid( col ) );

    // update collection
    iMediator->SetItemL( aMedia );
    }

// ---------------------------------------------------------------------------
// rename a media object
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::RenameL( const TDesC& aOldUri,
                                       const TDesC& aNewUri,
                                       TMPXGeneralCategory aItemCat )
    {
    MPX_FUNC("CMPXCollectionHelperImp::RenameL");
    MPX_DEBUG3("aOldUri = %S, aNewUri = %S", &aOldUri, &aNewUri);

    if (aItemCat != EMPXSong && aItemCat != EMPXPlaylist && aItemCat != EMPXAbstractAlbum)
        {
        User::Leave(KErrArgument);
        }

    // find the media using the old URI
    RArray<TMPXAttribute> attributes;
    CleanupClosePushL( attributes );
    attributes.AppendL(KMPXMediaGeneralId);
    attributes.AppendL(KMPXMediaMusicAlbumArtFileName);

    CMPXMedia* media = GetL(aOldUri, attributes.Array(), aItemCat);
    CleanupStack::PopAndDestroy(&attributes);
    CleanupStack::PushL(media);

    const TDesC& fileName(media->ValueText(KMPXMediaMusicAlbumArtFileName));
    
    // the songs have embedded albumart.
    if(fileName.CompareF(aOldUri) == 0)
        {
        // change the Art filename to the new Uri
        media->SetTextValueL(KMPXMediaMusicAlbumArtFileName, aNewUri);
        
        // Rename the thumbnail
        TRAPD(err, RenameThumbnailL(aOldUri, aNewUri));
        if(KErrNone != err)
            {
            MPX_DEBUG2("Thumbnail renames failed. Err: %d", err);
            }
        }

    // change file path to the new file path
    media->SetTextValueL(KMPXMediaGeneralUri, aNewUri);

    // ask harvester to rename the file if any of the following is true:
    // 1) renaming a song
    // 2) renaming a playlist scanned through file system.
    //
    //    There are 3 types of playlists. The ones created from the device
    //    are virtual playlists which have file extension .vir. There are
    //    no physical playlist files associated with them; hence not
    //    registered with harvester. For virtual playlists, IsPlaylistL
    //    will return EFalse because there is not a playlist plugin that
    //    deals with .vir playlist files.
    //
    //    The ones synced from PC through MTP have file extension .pla.
    //    There are 0-byte .pla files associated with them but they
    //    are not registered with harvester either. IsPlaylistL will also
    //    return EFalse because there is not a playlist plugin that deals
    //    with .pla.
    //
    //    The 3rd type of playlists is .m3u on the file system. They are
    //    added to the collection through file scan and registered with
    //    harvester. IsPlaylistL will return ETrue.
    if (aItemCat == EMPXSong || iHvsUtility->IsPlaylistL(aOldUri))
        {
        const TUid& collection =
             media->ValueTObjectL<TUid>(KMPXMediaGeneralCollectionId);
        iHvsUtility->RenameFileL( aOldUri, aNewUri, collection.iUid );
        }

    // Update collection via CMPXCommand
    //
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL( KMPXCommandGeneralId, KMPXCommandIdCollectionSet );
    TUid colId (media->ValueTObjectL<TUid>(KMPXMediaGeneralCollectionId) );
    cmd->SetTObjectValueL( KMPXCommandGeneralCollectionId, colId.iUid );
    cmd->SetTObjectValueL( KMPXCommandGeneralDoSync, ETrue );
    cmd->SetCObjectValueL( KMPXCommandColSetMedia, media );
    iCollectionUtil->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );

    CleanupStack::PopAndDestroy(media);
    }

// ---------------------------------------------------------------------------
// Retrieve a media object based on file path
// ---------------------------------------------------------------------------
//
CMPXMedia* CMPXCollectionHelperImp::GetL( const TDesC& aFile,
                                          const TArray<TMPXAttribute>& aAttrs,
                                          TMPXGeneralCategory aItemCat )
    {
    MPX_FUNC("CMPXCollectionHelperImp::GetL");
    MPX_DEBUG2("aFile %S", &aFile);
#ifdef ABSTRACTAUDIOALBUM_INCLUDED
    if (aItemCat != EMPXSong && aItemCat != EMPXPlaylist && aItemCat != EMPXAbstractAlbum)
#else
    if (aItemCat != EMPXSong && aItemCat != EMPXPlaylist)
#endif
        {
        User::Leave(KErrArgument);
        }

    // Playlists synced through MTP contains .pla extension and there
    // is no pla playlist plugin. We can no longer reply on harvester
    // utility's IsPlaylistL to determine if this is a playlist.
    // For now, the default collection to get this playlist is
    // EMPXCollectionPluginMusic for playlists. This will be revised
    // later when there is a generic way of resolving collections aside
    // from using file extension or UID. Same for virtual playlists.
    //
    // Find the collection Id from harvester for songs or playlists
    // scanned from the file system
    TInt col(iMusicCollectionId.iUid);
    if (aItemCat == EMPXSong || iHvsUtility->IsPlaylistL(aFile))
        {
        col = iHvsUtility->FindCollectionIdL( aFile );
        }

    // Search criteria
    //
    CMPXMedia* search = CMPXMedia::NewL();
    CleanupStack::PushL( search );
    search->SetTObjectValueL( KMPXMediaGeneralType, EMPXItem );
    search->SetTObjectValueL( KMPXMediaGeneralCategory, aItemCat );
    search->SetTObjectValueL<TUid>( KMPXMediaGeneralCollectionId, TUid::Uid(col) );
    search->SetTextValueL( KMPXMediaGeneralUri, aFile );

    // Search in synch mode
    //
    CMPXMedia* result = iCollectionUtil->Collection().FindAllL( *search, aAttrs );
    CleanupStack::PopAndDestroy( search );
    CleanupStack::PushL(result);

    const CMPXMediaArray* ary(NULL);
    if( result->IsSupported( KMPXMediaArrayContents ) )
      {
      ary = result->Value<CMPXMediaArray>(KMPXMediaArrayContents);
      if( !ary )
          {
          User::Leave(KErrNoMemory);
          }
      }

    CMPXMedia* foundMedia(NULL);

    if ( ary && ary->Count() )
        {
        foundMedia = CMPXMedia::NewL(*ary->AtL(0));
        CleanupStack::PushL(foundMedia);
        foundMedia->SetTObjectValueL<TUid>( KMPXMediaGeneralCollectionId, TUid::Uid(col) );
        CleanupStack::Pop(foundMedia);
        }
    else
        {
        User::Leave(KErrNotFound);    
        }
    
    CleanupStack::PopAndDestroy(result);
    return foundMedia;
    }

// ---------------------------------------------------------------------------
// Gets the attributes of some media object
// ---------------------------------------------------------------------------
//
const CMPXMedia& CMPXCollectionHelperImp::GetL( const TDesC& /*aFile*/,
                                                TMPXGeneralCategory /*aItemCat*/ )
    {
    User::Leave(KErrNotSupported);

    /* avoid compilation warnings */
    CMPXMedia* dummy(NULL);
    return *dummy;
    }

// ---------------------------------------------------------------------------
// Find function to return a list of songs
// ---------------------------------------------------------------------------
//
CMPXMedia* CMPXCollectionHelperImp::FindAllL( CMPXMedia& aCriteria,
                                              const TArray<TMPXAttribute>& aAttrs )
    {
    MPX_FUNC("CMPXCollectionHelperImp::FindAllL");

    // Search criteria, add the collection to search from
    //
    aCriteria.SetTObjectValueL<TUid>( KMPXMediaGeneralCollectionId,
                                      iMusicCollectionId );

    // Search in synch mode
    //
    return iCollectionUtil->Collection().FindAllL( aCriteria, aAttrs );
    }

// ---------------------------------------------------------------------------
// Handles completion of playlist export
// to-do: this should be changed to HandlePlaylistExportCompleted
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::HandlePlaylistExportCompletedL( CMPXMedia* aMedia,
                                                              TInt /*aErr*/ )
    {
    // Not used
    delete aMedia;
    }

// ---------------------------------------------------------------------------
// From MMPXHarvesterUtilityObserver
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::HandlePlaylistImportCompletedL( CMPXMedia* aMedia,
                                                              TInt /*aErr*/ )
    {
    // Not used
    delete aMedia;
    }

// ---------------------------------------------------------------------------
// From MMPXHarvesterUtilityObserver
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::HandleFileAddCompletedL( CMPXMedia* aMedia,
                                                       TInt aErr )
    {
    MPX_FUNC("CMPXCollectionHelperImp::HandleFileAddCompletedL");
    MPX_DEBUG2("err = %d", aErr);

    CleanupStack::PushL( aMedia );

    // Add to collection
    //
    TInt err( aErr );
    if( aErr == KErrNone )
        {
        TRAP( err, DoAddToCollectionL( aMedia ) );
        }

    // Callback to the observer
    //
    if( iObserver )
        {
        iObserver->HandleAddFileCompleteL( err );
        iObserver = NULL;
        }
    CleanupStack::PopAndDestroy( aMedia );
    }

// ---------------------------------------------------------------------------
// From MMPXHarvesterUtilityObserver
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::HandleDeleteCompletedL( TInt /*aErr*/ )
    {
    // not used.
    }

// ---------------------------------------------------------------------------
// From MMPXHarvesterUtilityObserver
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::HandleFileImportCompletedL( CMPXMedia* aMedia,
                                                         TInt /*aErr*/  )
    {
    delete aMedia;
    // not used
    }

// ---------------------------------------------------------------------------
// From MMPXHarvesterUtilityObserver
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::HandleFileGetMediaCompletedL( CMPXMedia* aMedia,
                                                         TInt aErr  )
    {
    MPX_FUNC("CMPXCollectionHelperImp::HandleFileGetMediaCompletedL");
    MPX_DEBUG2("err = %d", aErr);

    CleanupStack::PushL( aMedia );

    // Add to collection
    TInt err(aErr);
    if( err == KErrNone )
        {
        if (!aMedia->IsSupported(KMPXMediaGeneralCollectionId))
            {
            if (aMedia->IsSupported(KMPXMediaGeneralUri))
                {
                TRAP(err,
                     aMedia->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId,
                        TUid::Uid(iHvsUtility->GetColUidForFileL(aMedia->ValueText(KMPXMediaGeneralUri))));
                    );
                }
            else
                {
                err = KErrArgument;
                }
            }
        }
    if ( err == KErrNone )
        {
        TRAP(err, DoAddToCollectionL( aMedia ));
        }
    if( err == KErrNone )
        {
        // Add to harvester
        TRAP(err,iHvsUtility->AddFileL(aMedia));
        }

    // Callback to the observer
    //
    if( iObserver )
        {
        iObserver->HandleAddFileCompleteL( err );
        iObserver = NULL;
        }
    CleanupStack::PopAndDestroy( aMedia );
    }

// ---------------------------------------------------------------------------
// From MMPXMediatorObserver
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::HandleMediatorPathUpdatedL( CMPXMedia*& aMedia,
                                                          TUid  /*aOldPath*/ )
    {
    iHvsUtility->UpdateFileL( aMedia );
    }

// ---------------------------------------------------------------------------
// Add a file to the collection through the mediator
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::DoAddToCollectionL( CMPXMedia* aMedia )
    {
    iMediator->AddItemL( aMedia );
    }

// ---------------------------------------------------------------------------
// Frees this object
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::Close()
    {
    delete this;
    }

// ---------------------------------------------------------------------------
// Rename the thumbnail through TNM
// ---------------------------------------------------------------------------
//
void CMPXCollectionHelperImp::RenameThumbnailL( const TDesC& aOldUri, 
                                         const TDesC& aNewUri )
    {
    MPX_FUNC("CMPXCollectionHelperImp::RenameThumbnailL");
    
    // Create Thumbnail Manager instance when use first time.
    if(NULL == iTNManager)
        {      
        iTNManager = CThumbnailManager::NewL( *this ); 
        }
    
    // Rename thumbnail
    iTNManager->RenameThumbnailsL(aOldUri, aNewUri, 0);
    }

// ---------------------------------------------------------------------------
// CMPXDbAbstractAlbum::ThumbnailReady
// Callback but not used here
// ---------------------------------------------------------------------------
void CMPXCollectionHelperImp::ThumbnailPreviewReady(
        MThumbnailData& /*aThumbnail*/, TThumbnailRequestId /*aId*/ )
    {
    }


// ---------------------------------------------------------------------------
// CMPXDbAbstractAlbum::ThumbnailReady
// Callback but not used here
// ---------------------------------------------------------------------------
void CMPXCollectionHelperImp::ThumbnailReady( TInt /*aError*/,
        MThumbnailData& /*aThumbnail*/, TThumbnailRequestId /*aId*/ )
    {
    }
// End of file
