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
* Description:  mediator class to handle music specific collection requirements
*
*/


#include <e32base.h>
#include <bautils.h>
#include <mpxlog.h>
#include <mpxcollectionutility.h>
#include <mpxcollectionpath.h>
#include <mpxmedia.h>
#include <mpxmediaarray.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediaaudiodefs.h>
#include <mpxmediamusicdefs.h>
#include <mpxmediacontainerdefs.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxcollectioncommanddefs.h>
#include <mpxcollectionplugin.hrh>
#include <centralrepository.h>            // to get podcasting cenrep key

#include <data_caging_path_literals.hrh>
#include <mpxmediator.rsg>
#include "mpxcollectionmediator.h"
#include "mpxmediatorobserver.h"
#include "mpxmoveitemhelper.h"

// CONSTANTS
_LIT( KPodCastPath, "\\podcasts\\");

_LIT( KMediatorResource, "mpxmediator.rsc" );

const TUid KCRUIDHarvesterFeatures  = { 0x101FFCD2 };
const TInt KDisablePodcasting = 5;

// ---------------------------------------------------------------------------
// Default Constructor
// ---------------------------------------------------------------------------
//
CMPXCollectionMediator::CMPXCollectionMediator( MMPXCollection& aColUtil,
                                                MMPXMediatorObserver* aObs ) :
                                                iObserver( aObs )
    {
    iColUtil = &aColUtil;
    }

// ---------------------------------------------------------------------------
// Default Constructor
// ---------------------------------------------------------------------------
//
CMPXCollectionMediator::CMPXCollectionMediator()
    {

    }

// ---------------------------------------------------------------------------
// 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXCollectionMediator::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );

    TParse parse;
    TFileName resFile;
    parse.Set( KMediatorResource, &KDC_APP_RESOURCE_DIR, NULL );
    resFile.Copy(parse.FullName());
    User::LeaveIfError( MPXUser::CompleteWithDllPath( resFile ) );

    BaflUtils::NearestLanguageFile(iFs, resFile);
    RResourceFile resourceFile;
    resourceFile.OpenL(iFs, resFile);
    resourceFile.ConfirmSignatureL(0);  // magic

    TResourceReader resReader;

    // Get the first podcast buf
    HBufC8* buf = resourceFile.AllocReadLC( R_GENRE_PODCAST );
    resReader.SetBuffer( buf );
    iPodCastBuf = resReader.ReadHBufCL();
    CleanupStack::PopAndDestroy( buf );
    buf = NULL;

    // Get the second podcast buf
    buf = resourceFile.AllocReadLC( R_GENRE_PODCAST_ENGLISH );
    resReader.SetBuffer( buf );
    iPodCastEngBuf = resReader.ReadHBufCL();
    CleanupStack::PopAndDestroy( buf );

    // Get the unknwon podcast buf
    buf = resourceFile.AllocReadLC( R_GENRE_UNKNOWN );
    resReader.SetBuffer( buf );
    iUnknownBuf = resReader.ReadHBufCL();
    CleanupStack::PopAndDestroy( buf );

    // Fetch the real implementation ID for collection plugins
    RArray<TUid> ary;
    CleanupClosePushL( ary );

    MMPXCollectionUtility* temp = MMPXCollectionUtility::NewL();
    CleanupClosePushL( *temp );
    ary.AppendL( TUid::Uid(EMPXCollectionPluginPodCast) );
    iPodCastCollectionID = temp->CollectionIDL( ary.Array() );

    ary.Reset();
    ary.AppendL( TUid::Uid(EMPXCollectionPluginMusic) );
    iMusicCollectionID = temp->CollectionIDL( ary.Array() );
    CleanupStack::PopAndDestroy( temp );

    CleanupStack::PopAndDestroy( &ary );

    resourceFile.Close();
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionMediator* CMPXCollectionMediator::NewL(
                                                    MMPXCollection& aColUtil,
                                                    MMPXMediatorObserver* aObs )
    {
    CMPXCollectionMediator* self = CMPXCollectionMediator::NewLC( aColUtil,
                                                                  aObs );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionMediator* CMPXCollectionMediator::NewL()
    {
    CMPXCollectionMediator* self = new( ELeave ) CMPXCollectionMediator();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionMediator* CMPXCollectionMediator::NewLC(
                                                    MMPXCollection& aColUtil,
                                                    MMPXMediatorObserver* aObs )
    {
    CMPXCollectionMediator* self = new( ELeave ) CMPXCollectionMediator
                                                                 ( aColUtil,
                                                                   aObs );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXCollectionMediator::~CMPXCollectionMediator()
    {
    iFs.Close();

    delete iPodCastBuf;
    delete iPodCastEngBuf;
    delete iUnknownBuf;
    delete iMoveHelper;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionMediator::AddItemL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionMediator::AddItemL( CMPXMedia*& aMedia )
    {
    ASSERT( iColUtil );

    // If this is a podcast, change the collection id
    if( IsPodcastL( *aMedia ) )
        {
        UpdatePathToPodcastL( *aMedia );
        }

    // Add it to the collection
    CMPXCommand* cmd = CMPXMedia::NewL();
    CleanupStack::PushL( cmd );

    cmd->SetTObjectValueL(KMPXCommandGeneralId, KMPXCommandIdCollectionAdd );
    cmd->SetTObjectValueL(KMPXCommandGeneralDoSync, ETrue );
    cmd->SetCObjectValueL(KMPXCommandColAddMedia, aMedia); // copied

    if (aMedia->IsSupported(KMPXMediaGeneralCollectionId))
        {
        TUid collectionId = aMedia->ValueTObjectL<TUid>(KMPXMediaGeneralCollectionId);
        cmd->SetTObjectValueL(KMPXCommandGeneralCollectionId, collectionId.iUid);
        }
    else
        {
        User::Leave( KErrArgument );
        }

    iColUtil->CommandL(*cmd);

    TMPXItemId mediaId = cmd->ValueTObjectL<TMPXItemId>(KMPXCommandColAddRtnId);
    CleanupStack::PopAndDestroy(cmd);

    // put mediaId in aMedia
    aMedia->SetTObjectValueL<TMPXItemId>(KMPXMediaGeneralId, mediaId);
    }

// ---------------------------------------------------------------------------
// CMPXCollectionMediator::AddItemAsyncL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionMediator::AddItemAsyncL( CMPXMedia*& aMedia )
    {
    ASSERT( iColUtil );

    // If this is a podcast, change the collection id
    if( IsPodcastL( *aMedia ) )
        {
        UpdatePathToPodcastL( *aMedia );
        }

    // Add it to the collection
    CMPXCommand* cmd = CMPXMedia::NewL();
    CleanupStack::PushL( cmd );

    cmd->SetTObjectValueL(KMPXCommandGeneralId, KMPXCommandIdCollectionAdd );
    cmd->SetCObjectValueL(KMPXCommandColAddMedia, aMedia); // copied

    if (aMedia->IsSupported(KMPXMediaGeneralCollectionId))
        {
        TUid collectionId = aMedia->ValueTObjectL<TUid>(KMPXMediaGeneralCollectionId);
        cmd->SetTObjectValueL(KMPXCommandGeneralCollectionId, collectionId.iUid);
        }
    else
        {
        User::Leave( KErrArgument );
        }

    iColUtil->CommandL(*cmd);

    CleanupStack::PopAndDestroy(cmd);
    }

// ---------------------------------------------------------------------------
// CMPXCollectionMediator::AddItemL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionMediator::AddItemL( CMPXMediaArray& aMPArray )
    {
    ASSERT( iColUtil );

    // Check each media property and update as needed
    //
    CMPXMediaArray* musicArray = CMPXMediaArray::NewL();
    CleanupStack::PushL( musicArray );
    CMPXMediaArray* podcastArray = CMPXMediaArray::NewL();
    CleanupStack::PushL( podcastArray );

    TInt count( aMPArray.Count() );
    for( TInt i=0; i<count; ++i )
        {
        CMPXMedia* curMP = aMPArray.AtL(i);

        if( IsPodcastL( *curMP ) )
            {
            // Update path to be podcast and notify observer (if needed)
            //
            if( !curMP->IsSupported(KMPXMediaGeneralCollectionId) )
                {
                User::Leave( KErrArgument );
                }

            const TUid& oldCol = curMP->ValueTObjectL<TUid>( KMPXMediaGeneralCollectionId );

            UpdatePathToPodcastL( *curMP );
            if( iObserver )
                {
                TRAP_IGNORE(iObserver->HandleMediatorPathUpdatedL( curMP, oldCol ) );
                }

            // Add this to podcast collection
            curMP->SetTObjectValueL<TUid>( KMPXMediaGeneralCollectionId,
                                           iPodCastCollectionID );
            // Make a copy reference
            podcastArray->AppendL(*curMP);
            }
        else
            {
            // Make a copy reference
            musicArray->AppendL(*curMP);
            }
        }
    // Package it as a CMPXMediaContainer class, then send to collection
    //
    if( musicArray->Count() )
        {
        RArray<TInt> contId;
        CleanupClosePushL( contId );
        contId.AppendL( KMPXMediaIdGeneral );
        contId.AppendL( KMPXMediaIdContainer );

        CMPXMedia* container = CMPXMedia::NewL( contId.Array() );
        CleanupStack::PushL( container );
        container->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType,
                                                      EMPXGroup );
        container->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaGeneralCategory,
                                                          EMPXCollection );

        container->SetCObjectValueL<CMPXMediaArray>( KMPXMediaArrayContents,
                                                     musicArray );
        container->SetTObjectValueL<TInt>(KMPXMediaArrayCount, musicArray->Count() );

        // Add remainder to music collection
        DoAddToCollectionL( container, iMusicCollectionID );
        CleanupStack::PopAndDestroy( container );
        CleanupStack::PopAndDestroy( &contId );
        contId.Close();
        }
    if( podcastArray->Count() )
        {
        RArray<TInt> contId;
        CleanupClosePushL( contId );
        contId.AppendL( KMPXMediaIdGeneral );
        contId.AppendL( KMPXMediaIdContainer );

        CMPXMedia* container = CMPXMedia::NewL( contId.Array() );
        CleanupStack::PushL( container );
        container->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType,
                                                      EMPXGroup );
        container->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaGeneralCategory,
                                                          EMPXCollection );

        container->SetCObjectValueL<CMPXMediaArray>( KMPXMediaArrayContents,
                                                     podcastArray );
        container->SetTObjectValueL<TInt>(KMPXMediaArrayCount, podcastArray->Count() );

        // Add remainder to podcast collection
        DoAddToCollectionL( container, iPodCastCollectionID );
        CleanupStack::PopAndDestroy( container );
        CleanupStack::PopAndDestroy( &contId );
        contId.Close();
        }
    CleanupStack::PopAndDestroy( podcastArray );
    CleanupStack::PopAndDestroy( musicArray );
    }

// ---------------------------------------------------------------------------
// CMPXCollectionMediator::SetItemL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionMediator::SetItemL( CMPXMediaArray& aMPArray )
    {
    ASSERT( iColUtil );

    // Set each item
    //
    TInt count( aMPArray.Count() );
    for( TInt i=0; i<count; ++i )
        {
        CMPXMedia* prop = aMPArray.AtL(i);
        SetItemL( prop );
        }
    }

// ---------------------------------------------------------------------------
// CMPXCollectionMediator::SetItemL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionMediator::SetItemL( CMPXMedia*& aMedia )
    {
    ASSERT( iColUtil );

    // UI is only going to pass fields that were modified
    //
    if( IsPodcastL( *aMedia ) )
        {
        // Convert the collection id to "KPodCastCollection" UID
        //
        const TUid& oldCol = aMedia->ValueTObjectL<TUid>( KMPXMediaGeneralCollectionId );

        // No need to move it is already in podcast
        if( oldCol != iPodCastCollectionID )
            {
            TRAPD( err, DoMoveItemL( aMedia, iPodCastCollectionID ) );

            if( err == KErrNone )
                {
                if( iObserver )
                    {
                    iObserver->HandleMediatorPathUpdatedL( aMedia, oldCol );
                    }
                }
            else
                {
                DoSetToCollectionL( aMedia, iPodCastCollectionID );
                }
            }
        else
            {
            DoSetToCollectionL( aMedia, iPodCastCollectionID );
            }
        }
    else
        {
        // Update as usual
        //
        DoSetToCollectionL( aMedia,
                            aMedia->ValueTObjectL<TUid>(KMPXMediaGeneralCollectionId) );
        }
    }

// ---------------------------------------------------------------------------
// CMPXCollectionMediator::SetItemL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionMediator::SetItemL(
                                       TArray<CMPXMedia*>& /*aArray*/,
                                       CMPXCollectionPath& /*aPath*/,
                                       RArray<TInt>& /*aIndices*/ )
    {
    // Not necessary for Increment 8 timeframe
    ASSERT( iColUtil );
    ASSERT( 0 );
    }

// ---------------------------------------------------------------------------
// CMPXCollectionMediator::CheckItemL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionMediator::CheckItemL( CMPXMedia& aMedia )
    {
    MPX_DEBUG1( "CMPXCollectionMediator::CheckItemL <---" );
    if( IsPodcastL( aMedia ) )
        {
        MPX_DEBUG1( "CMPXCollectionMediator::CheckItemL - changing collection id" );
        UpdatePathToPodcastL( aMedia );
        }
    }

// ---------------------------------------------------------------------------
// CMPXCollectionMediator::MoveItemL()
// ---------------------------------------------------------------------------
//
EXPORT_C  void CMPXCollectionMediator::MoveItemL(
                                        CMPXMedia*& aMedia,
                                        TUid  aOldCollection,
                                        TUid  aNewCollection )
    {
    ASSERT( iColUtil );

    // If we are moving from podcast collection -> local audio, we strip the
    // podcast genre
    //
    if( aNewCollection == iMusicCollectionID &&
        aOldCollection == iPodCastCollectionID )
        {
        aMedia->SetTextValueL(KMPXMediaMusicGenre,
                              *iUnknownBuf );
        DoSetToCollectionL( aMedia, iPodCastCollectionID );
        }

    // Do the actual move operation
    //
    DoMoveItemL( aMedia, aNewCollection );
    }

// ---------------------------------------------------------------------------
// CMPXCollectionMediator::MoveItemL()
// ---------------------------------------------------------------------------
//
EXPORT_C  void CMPXCollectionMediator::MoveItemL(
                                        CMPXMedia*& aMedia,
                                        TUid  aOldCollection,
                                        TUid  aNewCollection,
                                        MMPXMoveItemObserver* aObs  )
    {
    ASSERT( iColUtil );

    // If we are moving from podcast collection -> local audio, we strip the
    // podcast genre
    //
    if( aNewCollection == iMusicCollectionID &&
        aOldCollection == iPodCastCollectionID )
        {
        aMedia->SetTextValueL(KMPXMediaMusicGenre,
                              *iUnknownBuf );
        DoSetToCollectionL( aMedia, iPodCastCollectionID );
        }

    // Do the actual move operation
    //
    delete iMoveHelper;
    iMoveHelper = NULL;
    iMoveHelper = CMPXMoveItemHelper::NewL(aObs);
    iMoveHelper->FetchAndMoveItemL( *aMedia, aNewCollection );
    }

// ---------------------------------------------------------------------------
// CMPXCollectionMediator::IsPodcast()
// ---------------------------------------------------------------------------
//
TBool CMPXCollectionMediator::IsPodcastL( CMPXMedia& aMedia )
    {
    MPX_DEBUG1( "CMPXCollectionMediator::IsPodcast <---" );

    ASSERT(aMedia.IsSupported(KMPXMediaGeneralCategory));

    TBool isPodCast( EFalse );

    // Get podcasting key
    TBool disablePodcast( EFalse );
    CRepository* repository = CRepository::NewL( KCRUIDHarvesterFeatures );
    repository->Get( KDisablePodcasting, disablePodcast );
    delete repository;

    if ( !disablePodcast )
        {
        const TDesC& genre = aMedia.ValueText(KMPXMediaMusicGenre);
        const TDesC& path = aMedia.ValueText(KMPXMediaGeneralUri);
        if ( aMedia.ValueTObjectL<TMPXGeneralCategory>( KMPXMediaGeneralCategory ) != EMPXPlaylist )
            {
            if( genre.Length() )
                {
                // Make everything lower case for easy comparison
                //
                HBufC* buf = genre.AllocL();
                TPtr ptr = buf->Des();
                ptr.LowerCase();

                if( !ptr.CompareF( *iPodCastBuf ) ||
                    !ptr.CompareF( *iPodCastEngBuf ) )
                    {
                    isPodCast = ETrue;
                    }
                delete buf;
                }
            if( path.Length() ) // also check podcast
                {
                HBufC* buf = path.AllocL();
                TPtr ptr = buf->Des();
                ptr.LowerCase();

                if( KErrNotFound != buf->Find( KPodCastPath ) )
                    {
                    isPodCast = ETrue;
                    }
                delete buf;
                }
            }
        }
    MPX_DEBUG2( "CMPXCollectionMediator::IsPodcast %i --->", isPodCast );

    return isPodCast;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionMediator::UpdatePathToPodcastL()
// ---------------------------------------------------------------------------
//
void CMPXCollectionMediator::UpdatePathToPodcastL( CMPXMedia& aMedia )
    {
    aMedia.SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId,
                                  iPodCastCollectionID );
    }

// ---------------------------------------------------------------------------
// CMPXCollectionMediator::DoMoveItemL()
// ---------------------------------------------------------------------------
//
void CMPXCollectionMediator::DoMoveItemL( CMPXMedia*& aMedia,
                                          TUid  aNewCollection )
    {
    CMPXMoveItemHelper* mHelper = CMPXMoveItemHelper::NewL(NULL);
    CleanupStack::PushL( mHelper );
    mHelper->FetchAndMoveItemSyncL( *aMedia, aNewCollection );
    CleanupStack::PopAndDestroy( mHelper );
    }

// ---------------------------------------------------------------------------
// Add an item to the collection
// ---------------------------------------------------------------------------
//
void CMPXCollectionMediator::DoAddToCollectionL( CMPXMedia* aMedia,
                                                 TUid aCollectionId )
    {
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );

    cmd->SetTObjectValueL( KMPXCommandGeneralId, KMPXCommandIdCollectionAdd );
    cmd->SetTObjectValueL( KMPXCommandGeneralDoSync, ETrue );
    cmd->SetTObjectValueL( KMPXCommandGeneralCollectionId, aCollectionId.iUid );
    cmd->SetCObjectValueL<CMPXMedia>( KMPXCommandColAddMedia, aMedia );

    iColUtil->CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );
    }

// ---------------------------------------------------------------------------
// Set an item to the collection
// ---------------------------------------------------------------------------
//
void CMPXCollectionMediator::DoSetToCollectionL( CMPXMedia* aMedia,
                                                 TUid aCollectionId )
    {
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );

    cmd->SetTObjectValueL( KMPXCommandGeneralId, KMPXCommandIdCollectionSet );
    cmd->SetTObjectValueL( KMPXCommandGeneralDoSync, ETrue );
    cmd->SetTObjectValueL( KMPXCommandGeneralCollectionId, aCollectionId.iUid );
    cmd->SetCObjectValueL<CMPXMedia>( KMPXCommandColSetMedia, aMedia );

    iColUtil->CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );
    }

// END OF FILE

