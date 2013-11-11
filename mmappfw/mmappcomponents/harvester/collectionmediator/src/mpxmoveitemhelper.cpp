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
* Description:  Helper class to move items to different collections
*
*/


#include <e32base.h>
#include <mpxmedia.h>
#include <mpxmediaarray.h>
#include <mpxcollectionutility.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediaaudiodefs.h>
#include <mpxmediacontainerdefs.h>
#include <mpxmediamusicdefs.h>
#include <mpxcollectionplugin.hrh>
#include <mpxplaybackutility.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxcollectioncommanddefs.h>

#include "mpxmediatorobserver.h"
#include "mpxmoveobserver.h"
#include "mpxmoveitemhelper.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CMPXMoveItemHelper::CMPXMoveItemHelper( MMPXMoveItemObserver* aObs )
                                                          : iObs(aObs),
                                                            iMoveState(EIdle )
    {
    }


// ---------------------------------------------------------------------------
// 2nd phased constructor
// ---------------------------------------------------------------------------
//
void CMPXMoveItemHelper::ConstructL()
    {
    iCollection = MMPXCollectionUtility::NewL( NULL );

    RArray<TUid> ary;
    CleanupClosePushL( ary );

    ary.AppendL( TUid::Uid(EMPXCollectionPluginPodCast) );
    iPodCastCollectionID = iCollection->CollectionIDL( ary.Array() );

    ary.Reset();
    ary.AppendL( TUid::Uid(EMPXCollectionPluginMusic) );
    iMusicCollectionID = iCollection->CollectionIDL( ary.Array() );

    CleanupStack::PopAndDestroy( &ary );
    }


// ---------------------------------------------------------------------------
// Two phase constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXMoveItemHelper* CMPXMoveItemHelper::NewL( MMPXMoveItemObserver* aObs )
    {
    CMPXMoveItemHelper* self = CMPXMoveItemHelper::NewLC(aObs);
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two phased constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXMoveItemHelper* CMPXMoveItemHelper::NewLC( MMPXMoveItemObserver* aObs )
    {
    CMPXMoveItemHelper* self = new( ELeave ) CMPXMoveItemHelper(aObs);
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// virtual destructor
// ---------------------------------------------------------------------------
//
CMPXMoveItemHelper::~CMPXMoveItemHelper()
    {
    if( iCollection )
        {
        iCollection->Close();
        }
    }

// ---------------------------------------------------------------------------
// Move an item synchronously
// Three things to do to move an item
// 1: Remove item from old db
// 2: Udpdate media with new collection id
// 3: Add item into new db
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXMoveItemHelper::MoveItemL( CMPXMedia& aMedia, TUid aNewCollection )
    {
    MMPXPlaybackUtility* pbUtil = MMPXPlaybackUtility::UtilityL( KPbModeActivePlayer );
    CleanupClosePushL(*pbUtil);

    // Media as a container or item
    //
    TMPXGeneralType type =
         aMedia.ValueTObjectL<TMPXGeneralType>( TMPXAttribute( KMPXMediaIdGeneral,
                                                               EMPXMediaGeneralType ) );

    if ( type == EMPXGroup )
        {
        const CMPXMediaArray* array = aMedia.Value<CMPXMediaArray>(
                                TMPXAttribute( KMPXMediaIdContainer,
                                               EMPXMediaArrayContents ) );
        if( !array )
            {
            User::Leave( KErrNoMemory );
            }

        TInt count( array->Count() );
        if( count == 0 )
            {
            User::Leave( KErrNotFound );
            }
        for( TInt i=0; i<count; ++i )
            {
            CMPXMedia* entry = array->AtL(i);

            // Notify the playback utility before removing the media item
            if(entry->IsSupported(KMPXMediaGeneralId))
                {
                TMPXItemId mediaId(entry->ValueTObjectL<TMPXItemId>(KMPXMediaGeneralId));
                TRAP_IGNORE(pbUtil->CommandL( EPbCmdCloseItem, mediaId));
                }

            TUid oldCollection = entry->ValueTObjectL<TUid>(TMPXAttribute(KMPXMediaIdGeneral,
                                                                          EMPXMediaGeneralCollectionId));

            DoRemoveL( entry, oldCollection );

            entry->SetTObjectValueL<TUid>(TMPXAttribute(KMPXMediaIdGeneral,
                                                       EMPXMediaGeneralCollectionId),
                                         aNewCollection);

            UpdateMediaForMoveL( *entry, oldCollection, aNewCollection );
            DoAddL( entry, aNewCollection );
            }
        }
    else
        {
        TUid oldCollection = aMedia.ValueTObjectL<TUid>(TMPXAttribute(KMPXMediaIdGeneral,
                                                                      EMPXMediaGeneralCollectionId));

        aMedia.SetTObjectValueL<TUid>(TMPXAttribute(KMPXMediaIdGeneral,
                                                    EMPXMediaGeneralCollectionId),
                                      aNewCollection);

        if(aMedia.IsSupported(KMPXMediaGeneralId))
            {
            TMPXItemId mediaId(aMedia.ValueTObjectL<TMPXItemId>(KMPXMediaGeneralId));
            TRAP_IGNORE(pbUtil->CommandL( EPbCmdCloseItem, mediaId));
            }

        DoRemoveL( &aMedia, oldCollection );

        UpdateMediaForMoveL( aMedia, oldCollection, aNewCollection );
        DoAddL( &aMedia, aNewCollection );
        }

    CleanupStack::PopAndDestroy(pbUtil);
    }

// ---------------------------------------------------------------------------
// Fetch an item then move it async
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXMoveItemHelper::FetchAndMoveItemL( const CMPXMedia& aMedia,
                                                     TUid aNewCollection )
    {
    ASSERT( iMoveState == EIdle );
    ASSERT( iObs );
    // Fetch item from collection asynchronously
    //
    // Look for the item, and fetch all relavant info
    //
    RArray<TMPXAttribute> atts;
    CleanupClosePushL( atts );
    atts.Append(KMPXMediaGeneralAll);
    atts.Append(KMPXMediaAudioAudioAll);
    atts.Append(KMPXMediaMusicAll);

    iCollection->Collection().FindAllL( aMedia, atts.Array(), *this );
    CleanupStack::PopAndDestroy( &atts );
    iMoveState = EFind;
    iMoveTarget = aNewCollection;
    }

// ---------------------------------------------------------------------------
// Fetch an item then move it sync
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXMoveItemHelper::FetchAndMoveItemSyncL( const CMPXMedia& aMedia,
                                                TUid aNewCollection )
    {
    // Look for the item, and fetch all relavant info
    //
    RArray<TMPXAttribute> atts;
    CleanupClosePushL( atts );
    atts.Append(KMPXMediaGeneralAll);
    atts.Append(KMPXMediaAudioAudioAll);
    atts.Append(KMPXMediaMusicAll);

    CMPXMedia* result = iCollection->Collection().FindAllL( aMedia,
                                                            atts.Array() );
    CleanupStack::PopAndDestroy( &atts );
    CleanupStack::PushL( result );

    // Move the item as normal
    MoveItemL( *result, aNewCollection );
    CleanupStack::PopAndDestroy( result );
    }

// ---------------------------------------------------------------------------
// Updates a media object for collection specific detail
// ---------------------------------------------------------------------------
//
void CMPXMoveItemHelper::UpdateMediaForMoveL( CMPXMedia& aMedia,
                                              TUid& aOldCollection,
                                              TUid& aNewCollection )
    {
    // If we are moving from podcast collection -> local audio
    //
    if( aNewCollection == iMusicCollectionID &&
        aOldCollection == iPodCastCollectionID )
        {
        aMedia.SetTObjectValueL(TMPXAttribute(KMPXMediaIdGeneral,
                                               EMPXMediaGeneralCategory),
                                 EMPXSong);
        }
    // Move from local collection -> podcast
    //
    else if( aOldCollection == iMusicCollectionID &&
             aNewCollection == iPodCastCollectionID )
        {
        aMedia.SetTObjectValueL(TMPXAttribute(KMPXMediaIdGeneral,
                                         EMPXMediaGeneralCategory),
                                 EMPXPodcast);
        }
    } //lint !e961

// ---------------------------------------------------------------------------
// Add an item to the collection
// ---------------------------------------------------------------------------
//
void CMPXMoveItemHelper::DoAddL( CMPXMedia* aMedia, TUid aCollectionId )
    {
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );

    cmd->SetTObjectValueL( KMPXCommandGeneralId, KMPXCommandIdCollectionAdd );
    cmd->SetTObjectValueL( KMPXCommandGeneralDoSync, ETrue );
    cmd->SetTObjectValueL( KMPXCommandGeneralCollectionId, aCollectionId.iUid );
    cmd->SetCObjectValueL<CMPXMedia>( KMPXCommandColAddMedia, aMedia );

    iCollection->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );
    }

// ---------------------------------------------------------------------------
// Set an item to the collection
// ---------------------------------------------------------------------------
//
void CMPXMoveItemHelper::DoRemoveL( CMPXMedia* aMedia, TUid aCollectionId )
    {
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );

    cmd->SetTObjectValueL( KMPXCommandGeneralId, KMPXCommandIdCollectionRemoveMedia );
    cmd->SetTObjectValueL( KMPXCommandGeneralDoSync, ETrue );
    cmd->SetTObjectValueL( KMPXCommandCollectionRemoveMediaDeleteRecord, ETrue );
    cmd->SetTObjectValueL( KMPXCommandGeneralCollectionId, aCollectionId.iUid );
    cmd->SetCObjectValueL<CMPXMedia>( KMPXCommandCollectionRemoveMedia, aMedia );

    iCollection->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );
    }

// ---------------------------------------------------------------------------
// Handle find all callback
// ---------------------------------------------------------------------------
//
void CMPXMoveItemHelper::HandleFindAllL( const CMPXMedia& aResults,
                                         TBool /*aComplete*/,TInt aError)
    {
    // Make sure the mode is correct
    ASSERT( iMoveState == EFind );

    TInt err ( aError );
    if( err == KErrNone )
        {
        TRAP( err, DoHandleFindAllL( aResults ) );
        }

    // Find all is the most time consuming part.
    //
    iObs->HandleMoveCompleteL( err );
    iMoveState = EIdle;
    }

// ---------------------------------------------------------------------------
// Handle find all callback
// ---------------------------------------------------------------------------
//
void CMPXMoveItemHelper::DoHandleFindAllL( const CMPXMedia& aResult )
    {
    // Need to make a tmp copy because we have to modify collection id
    //
    CMPXMedia* tmp = CMPXMedia::NewL();
    CleanupStack::PushL( tmp );
    *tmp = aResult;
    MoveItemL( *tmp, iMoveTarget );
    CleanupStack::PopAndDestroy( tmp );
    }

// End of file
