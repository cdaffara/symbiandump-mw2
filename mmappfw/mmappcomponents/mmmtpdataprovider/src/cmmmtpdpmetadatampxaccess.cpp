/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Meta data Mpx access
*
*/


// from Symbian MTP
#include <mtp/tmtptypeuint8.h>
#include <mtp/tmtptypeuint16.h>
#include <mtp/tmtptypeuint32.h>
#include <mtp/tmtptypeuint64.h>
#include <mtp/tmtptypeuint128.h>
#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypearray.h>
#include <mtp/mmtpdataproviderframework.h>
#include <mtp/mtpprotocolconstants.h>
// from MPX
#include <mpxmedia.h>
#include <mpxmediaarray.h>
#include <mpxmediamusicdefs.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediaaudiodefs.h>
#include <mpxmediamtpdefs.h>
#include <mpxmediacontainerdefs.h>
#include <mpxcollectionhelperfactory.h>
#include <mpxcollectionhelper.h>
#include <pathinfo.h>

#include "cmmmtpdpmetadatampxaccess.h"
#include "mmmtpdplogger.h"
#include "mmmtpdputility.h"
#include "tmmmtpdppanic.h"
#include "tobjectdescription.h"

static const TInt KMtpInvalidSongID = 0x1FFFFFFF;
static const TInt KMtpChannelMono = 1;
static const TInt KMtpChannelStereo = 2;


_LIT( KMtpDateTimeConnector, "T" );
_LIT( KEmptyText, "" );

#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
_LIT( KMpxCollectionNewL, "MpxCollectionNewL" );
_LIT( KMpxCollectionAddL, "MpxCollectionAddL" );
_LIT( KMpxCollectionGetL, "MpxCollectionGetL" );
_LIT( KMpxCollectionSetL, "MpxCollectionSetL" );
_LIT( KMpxCollectionSetReferenceL, "MpxCollectionSetReferenceL" );
_LIT( KMpxCollectionGetAbstractMedia, "MpxCollectionGetAbstractMedia" );
_LIT( KMpxCollectionGetReference, "MpxCollectionGetReference" );
_LIT( KMpxCollectionFindAllLValidate, "MpxCollectionValidate" );
_LIT( KMpxCollectionFindAllLBeforeAdd, "MpxCollectionFindAllLBeforeAdd" );
_LIT( KMpxCollectionFindAllLModified, "MpxCollectionFindAllLModified" );
_LIT( KMpxCollectionCleanupDeletedMediasL, "KMpxCollectionCleanupDeletedMediasL" );
_LIT( KSetMetadataValue, "SetMetadataValueL" );
#endif

CMmMtpDpMetadataMpxAccess* CMmMtpDpMetadataMpxAccess::NewL( RFs& aRfs )
    {
    CMmMtpDpMetadataMpxAccess* self = new( ELeave ) CMmMtpDpMetadataMpxAccess( aRfs );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

CMmMtpDpMetadataMpxAccess::CMmMtpDpMetadataMpxAccess( RFs& aRfs ) :
    iRfs( aRfs ),
    iSampleData( NULL ),
    iHasCleanUp( EFalse )
    {
    // Do nothing
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::ConstructL
// Second-phase
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataMpxAccess::ConstructL()
    {
    // for performance measurement purpose
#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
    iPerfLog = CMmMtpDpPerfLog::NewL( _L( "CMmMtpDpMetadataMpxAccess" ) );
#endif
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::~CMmMtpDpMetadataMpxAccess
// Destructor
// ---------------------------------------------------------------------------
//
CMmMtpDpMetadataMpxAccess::~CMmMtpDpMetadataMpxAccess()
    {
    if ( iCollectionHelper != NULL )
        {
        iCollectionHelper->Close();
        iCollectionHelper = NULL;
        }

    delete iSampleData;

    // for performance measurement purpose
#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
    delete iPerfLog;
#endif
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::OpenSession
// This is introduced to fix CollectionHelper Flush problem
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataMpxAccess::OpenSession()
    {
    // do nothing
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::CloseSession
// This is introduced to fix CollectionHelper Flush problem
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataMpxAccess::CloseSession()
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::CloseSession" ) );

    // flush cache
    if ( iCollectionHelper != NULL )
        {
        PRINT( _L( "MM MTP <> Delete & Close CollectionHelper" ) );
        iCollectionHelper->Close();
        iCollectionHelper = NULL;
        }

    iHasCleanUp = EFalse;  // must reset, as class is not always destoryed
    PRINT( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::CloseSession" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::GetObjectMetadataValueL
// Gets a piece of metadata from the collection
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataMpxAccess::GetObjectMetadataValueL( const TUint16 aPropCode,
    MMTPType& aNewData,
    const TDesC& aFullFileName,
    TMPXGeneralCategory aCategory )
    {
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::GetObjectMetadataValueL aPropCode = 0x%x" ), aPropCode );

    PERFLOGSTART( KMpxCollectionGetL );
    const CMPXMedia& media = CollectionHelperL()->GetL( aFullFileName, aCategory );
    PERFLOGSTOP( KMpxCollectionGetL );

    TMPXAttributeData attrib( MpxAttribFromPropL( aPropCode ) );
    TBool isSupported = media.IsSupported( attrib );
    PRINT1(_L( "MM MTP <> CMmMtpDpMetadataMpxAccess::GetObjectMetadataValueL isSupported = %d" ), isSupported );

    if ( aPropCode != EMTPObjectPropCodeOriginalReleaseDate
        && aPropCode != EMTPObjectPropCodeDRMStatus
        && !isSupported )
        {
        User::Leave( KErrNotSupported );
        }

    switch ( aPropCode )
        {
        case EMTPObjectPropCodeName:
        case EMTPObjectPropCodeArtist:
        case EMTPObjectPropCodeAlbumName:
        case EMTPObjectPropCodeComposer:
        case EMTPObjectPropCodeGenre:
        case EMTPObjectPropCodeAlbumArtist:
            {
            if ( EMTPTypeString == aNewData.Type() )
                {
                ( ( CMTPTypeString& ) aNewData ).SetL( media.ValueText( attrib ) );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeDescription:
            {
            HBufC* data;
            data = media.ValueText( KMPXMediaGeneralComment ).AllocLC(); // + data
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::GetObjectMetadataValue data = %S" ), data );
            if ( EMTPTypeAUINT16 == aNewData.Type() )
                {
                TInt len = data->Length();
                PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::GetObjectMetadataValue len = %d" ),len );
                if( len != 0 )
                    {
                    for( TInt i = 0; i < len; i++ )
                        ( ( CMTPTypeArray& ) aNewData ).AppendUintL( (*data)[i] );
                    }
                }
            else
                {
                User::Leave( KErrArgument );
                }
            CleanupStack::PopAndDestroy( data ); // - data
            }
            break;

        case EMTPObjectPropCodeDateModified:
        case EMTPObjectPropCodeDateAdded:
        case EMTPObjectPropCodeDateCreated:
            {
            TTime time( *media.Value<TInt64> ( attrib ) );
            TBuf<KMtpMaxDateTimeStringLength> timeStr;
            time.FormatL( timeStr, KMtpDateTimeFormat );

            if ( EMTPTypeString == aNewData.Type() )
                {
                ( ( CMTPTypeString & ) aNewData ).SetL( timeStr );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::GetObjectMetadataValue - get DC4E DateAdded = %Ld" ), time.Int64() );
            }
            break;

        case EMTPObjectPropCodeDuration:
        case EMTPObjectPropCodeAudioBitRate:
        case EMTPObjectPropCodeSampleRate:
        case EMTPObjectPropCodeAudioWAVECodec:
            {
            if ( EMTPTypeUINT32 == aNewData.Type() )
                {
                ( ( TMTPTypeUint32& ) aNewData ).Set( *media.Value<TUint32>( attrib ) );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeTrack:
            {
            TLex lex( media.ValueText( attrib ) );
            TUint16 uint16( 0 );
            lex.Val( uint16, EDecimal );

            if ( EMTPTypeUINT16 == aNewData.Type() )
                {
                ( ( TMTPTypeUint16 & ) aNewData ).Set( uint16 );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::GetObjectMetadataValue - get DC8B Track = %d" ), uint16 );
            }
            break;

        case EMTPObjectPropCodeOriginalReleaseDate:
            {
            // Compose DateTime string in format YYYYMMDDTHHMMSS
            TBuf<KMtpMaxDateTimeStringLength> dateTime;
            dateTime.Zero();

            // NOTE: Handled specially, shouldn't leave like other property, following S60
            if ( !isSupported )
                {
                PRINT( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::GetObjectMetadataValue 0xDC99 date hasn't been set before" ) );
                dateTime.Copy( KEmptyText );
                }
            else
                {
                TTime time = *media.Value<TInt64> ( attrib );
#ifdef _DEBUG
                RDebug::Print( _L( "MM MTP <> GetObjectMetadataValue 0xDC99 time = %Ld, Year = %d, Month = %d, Day = %d, Hour = %d, Minute = %d, Second = %d" ),
                    time.Int64(),
                    time.DateTime().Year(),
                    time.DateTime().Month(),
                    time.DateTime().Day(),
                    time.DateTime().Hour(),
                    time.DateTime().Minute(),
                    time.DateTime().Second() );
#endif  // _DEBUG
                // Compose DateTime string in format YYYYMMDDTHHMMSS
                dateTime.AppendNumFixedWidth( time.DateTime().Year(),
                    EDecimal,
                    4 );
                dateTime.AppendNumFixedWidth( time.DateTime().Month() + 1,
                    EDecimal,
                    2 );
                dateTime.AppendNumFixedWidth( time.DateTime().Day() + 1,
                    EDecimal,
                    2 );
                dateTime.Append( KMtpDateTimeConnector );
                dateTime.AppendNumFixedWidth( time.DateTime().Hour(),
                    EDecimal,
                    2 );
                dateTime.AppendNumFixedWidth( time.DateTime().Minute(),
                    EDecimal, 2 );
                dateTime.AppendNumFixedWidth( time.DateTime().Second(),
                    EDecimal, 2 );

                PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::GetObjectMetadataValue 0xDC99 dateString = %S" ), &dateTime );
                }
            if ( EMTPTypeString == aNewData.Type() )
                {
                ( ( CMTPTypeString & ) aNewData ).SetL( dateTime );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeNumberOfChannels:
        case EMTPObjectPropCodeDRMStatus:
            if ( !isSupported)
                {
                PRINT( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::GetObjectMetadataValue 0xDC9D date hasn't been set before" ) );
                TInt16 zeroValue = 0x0;
                ( ( TMTPTypeUint16 & ) aNewData ).Set( zeroValue );
                }
            else
                {
                if (EMTPTypeUINT16 == aNewData.Type() )
                    {
                    ( ( TMTPTypeUint16 & ) aNewData ).Set( *media.Value<TUint16>(attrib) );
                    }
                else
                    {
                    User::Leave(KErrArgument);
                    }
                }
            break;

        default:
            {
            User::Leave( KErrNotSupported );
            }
            break;
        }

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::GetObjectMetadataValueL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::CollectionHelperL
// Returns the collection helper
// ---------------------------------------------------------------------------
//
MMPXCollectionHelper* CMmMtpDpMetadataMpxAccess::CollectionHelperL()
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::CollectionHelperL()" ) );

    // This is for the case when drive is completely full but Music DB is not
    // yet created. NewCollectionCachedHelperL() will attempt to create a
    // DB file on the spot when MPX Collection helper is accessed. For the
    // case of Music DB creation failure, it will be handled by MTP server
    // as a General Error
    if ( iCollectionHelper == NULL )
        {
        PERFLOGSTART( KMpxCollectionNewL );
        iCollectionHelper = CMPXCollectionHelperFactory::NewCollectionCachedHelperL();
        PERFLOGSTOP( KMpxCollectionNewL );

        // Do a search for a song ID that does not exist
        // This is to validate the presence of the media database.
        RArray<TInt> contentIDs;
        CleanupClosePushL( contentIDs ); // + contentIDs
        contentIDs.AppendL( KMPXMediaIdGeneral );

        CMPXMedia* searchMedia = CMPXMedia::NewL( contentIDs.Array() );
        CleanupStack::PopAndDestroy( &contentIDs ); // - contentIDs
        CleanupStack::PushL( searchMedia ); // + searchMedia

        searchMedia->SetTObjectValueL( KMPXMediaGeneralType, EMPXItem );
        searchMedia->SetTObjectValueL( KMPXMediaGeneralCategory, EMPXSong );
        searchMedia->SetTObjectValueL<TMPXItemId>( KMPXMediaGeneralId,
            KMtpInvalidSongID );
        if ( iStoreRoot.Length() > 0 )
            {
            searchMedia->SetTextValueL( KMPXMediaGeneralDrive, iStoreRoot );
            }

        RArray<TMPXAttribute> songAttributes;
        CleanupClosePushL( songAttributes ); // + songAttributes
        songAttributes.AppendL( KMPXMediaGeneralId );

        CMPXMedia* foundMedia = NULL;

        PERFLOGSTART( KMpxCollectionFindAllLValidate );
        TRAPD( err, foundMedia = iCollectionHelper->FindAllL(
            *searchMedia,
            songAttributes.Array() ) );
        PERFLOGSTOP( KMpxCollectionFindAllLValidate );

        CleanupStack::PopAndDestroy( &songAttributes ); // - songAttributes
        CleanupStack::PopAndDestroy( searchMedia ); // - searchMedia

        CleanupStack::PushL( foundMedia ); // + foundMedia
        if ( err != KErrNone )
            {
            PRINT1( _L("MM MTP <> CMmMtpDpMetadataMpxAccess::CollectionHelperL() Had err (%d) accessing the Music Database!!!"), err );
            // Delete the collection helper for now
            iCollectionHelper->Close();
            iCollectionHelper = NULL;
            User::Leave( KErrGeneral );
            }
        else
            {
            PRINT( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::CollectionHelperL() Music Database successfully created and validated!!!" ) );
            }

        CleanupStack::PopAndDestroy( foundMedia ); // - foundMedia
        }

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::CollectionHelperL()" ) );
    return iCollectionHelper;
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::FindWMPMediaLC
// Finds Media with specific WMP values
// ---------------------------------------------------------------------------
//
CMPXMedia* CMmMtpDpMetadataMpxAccess::FindWMPMediaLC( TMPXAttributeData aWMPMediaID,
    TBool aFlag )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::FindWMPMediaLC" ) );

    CMPXMedia* searchMedia = CMPXMedia::NewL();
    CleanupStack::PushL( searchMedia ); // + searchMeida

    searchMedia->SetTObjectValueL( KMPXMediaGeneralType, EMPXGroup );
    searchMedia->SetTObjectValueL( KMPXMediaGeneralCategory, EMPXSong );
    searchMedia->SetTObjectValueL<TBool>( aWMPMediaID, aFlag );
    searchMedia->SetTextValueL( KMPXMediaGeneralDrive, iStoreRoot );

    RArray<TMPXAttribute> songAttributes;
    CleanupClosePushL( songAttributes ); // + songAttributes
    songAttributes.AppendL( KMPXMediaGeneralUri );

    PRINT( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::FindWMPMediaLC searchMedia setup with no problems" ) );

    PERFLOGSTART(KMpxCollectionFindAllLModified);
    CMPXMedia* foundMedia = CollectionHelperL()->FindAllL( *searchMedia,
        songAttributes.Array() );
    PERFLOGSTOP(KMpxCollectionFindAllLModified);
    PRINT( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::FindWMPMediaLC foundMedia assigned from FindAllL" ) );

    CleanupStack::PopAndDestroy( &songAttributes ); // - songAttributes
    CleanupStack::PopAndDestroy( searchMedia ); // - searchMedia
    CleanupStack::PushL( foundMedia ); // + foundMedia

    if ( !foundMedia->IsSupported( KMPXMediaArrayCount ) )
        {
        User::Leave( KErrNotSupported );
        }

#ifdef _DEBUG
    TInt foundItemCount = *foundMedia->Value<TInt>( KMPXMediaArrayCount );
    PRINT1( _L( "MM MTP <> %d Media Objects found in the WMP search" ), foundItemCount );
#endif

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::FindWMPMediaLC" ) );
    return foundMedia;
    }

// ---------------------------------------------------------------------------
// Update the Sync flag for those not synchronized, Update the Modified flag for those have been modified, and delete the stale
// records for files that have been deleted.
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataMpxAccess::UpdateMusicCollectionL()
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::UpdateMusicCollectionL" ) );
    CMPXMedia* foundMedia= NULL;
    TInt foundItemCount( 0 );

    // We Should Consider this!!!
    //        if (iWmpRoundTripUsed)  // Only update values if they've been read by the PC...
    //            {
    // Change flag on acquired content so they won't be reported twice

    //when thousands of the file are being copied in with mass storage, and MTP is connected/disconnected for the first time
    //updateing the sync bit cause a serious performance issue issue
    //since our DP is not presistent, and we are not depedns on KMPXMediaGeneralSynchronized flag to tell whether a file is newly added or not
    //reseting this flag is not needed in reality, comment out for now
    /*foundMedia = FindWMPMediaLC( KMPXMediaGeneralSynchronized, EFalse );// + foundMedia

    if ( !foundMedia->IsSupported( KMPXMediaArrayCount ) )
        {
        User::Leave( KErrNotSupported );
        }

    foundItemCount = *foundMedia->Value<TInt>( KMPXMediaArrayCount );

    PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::UpdateMusicCollectionL found %d Media Objects" ), foundItemCount );

    if ( foundItemCount )
        {
        if ( !foundMedia->IsSupported( KMPXMediaArrayContents ) )
            {
            User::Leave( KErrNotSupported );
            }

        const CMPXMediaArray* foundArray = foundMedia->Value<CMPXMediaArray>(
            KMPXMediaArrayContents );

        for ( TInt j = 0; j < foundItemCount; j++ )
            {
            CMPXMedia* media = CMPXMedia::NewL( *(*foundArray)[j] );
            CleanupStack::PushL( media );
            media->SetTObjectValueL<TBool>(
                KMPXMediaGeneralSynchronized,
                ETrue );
            // Update the song's metadata with the media object
            PERFLOGSTART(KMpxCollectionSetL);
            CollectionHelperL()->SetL( media );
            PERFLOGSTOP(KMpxCollectionSetL);

            CleanupStack::PopAndDestroy( media );
            }
        }

    CleanupStack::PopAndDestroy( foundMedia );// - foundMedia*/

    //we are indeed depends on KMPXMediaGeneralModified for modified content to be report correctly
    //hence it should be reset everytime to ensure integrity
    // Change flag for files with updated metadata
    PRINT( _L( "MM MTP <> Updating the mod bit for files..." ) );

    foundMedia = FindWMPMediaLC( KMPXMediaGeneralModified, ETrue ); // + foundMedia

    if ( !foundMedia->IsSupported( KMPXMediaArrayCount ) )
        {
        User::Leave( KErrNotSupported );
        }

    foundItemCount = *foundMedia->Value<TInt>( KMPXMediaArrayCount );

    PRINT1( _L( "MM MTP <> CMtpMpxMetadataAccess::UpdateMusicCollectionL found %d Media Objects" ), foundItemCount );

    if ( foundItemCount )
        {
        if ( !foundMedia->IsSupported( KMPXMediaArrayContents ) )
            {
            User::Leave( KErrNotSupported );
            }

        const CMPXMediaArray* foundArray2 = foundMedia->Value<CMPXMediaArray>(
            KMPXMediaArrayContents );

        for ( TInt j = 0; j < foundItemCount; j++ )
            {
            CMPXMedia* media = CMPXMedia::NewL( *(*foundArray2)[j] );
            CleanupStack::PushL( media ); // + media
            media->SetTObjectValueL<TBool>(
                KMPXMediaGeneralModified,
                EFalse );
            // Update the song's metadata with the media object
            PERFLOGSTART( KMpxCollectionSetL );
            CollectionHelperL()->SetL( media );
            PERFLOGSTOP( KMpxCollectionSetL );

            CleanupStack::PopAndDestroy( media ); // - media
            }
        }

    CleanupStack::PopAndDestroy( foundMedia ); // - foundMedia

    //although as a non-presistent DP, we are not depending on KMPXMediaGeneralDeleted to report deleted file,
    //however, mark as deleted entry should be cleanup to improve music db performance
    // Delete stale records from the audio collection, for files that have been deleted.
    // Records are deleted everytime in CloseSession to improve the audio database performance
    // as there is NOT a separate database for deleted files.
    PRINT( _L( "MM MTP <> Deleting metadata for deleted files" ) );

    if ( !iHasCleanUp )
        {
        PERFLOGSTART( KMpxCollectionCleanupDeletedMediasL );
        CollectionHelperL()->CleanupDeletedMediasL();
        PERFLOGSTOP( KMpxCollectionCleanupDeletedMediasL );

        iHasCleanUp = ETrue;
        }

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::UpdateMusicCollectionL" ) );
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::RemoveL
// Remove the file from MPX DB
// -----------------------------------------------------------------------------
//
void CMmMtpDpMetadataMpxAccess::DeleteObjectL( const TDesC& aFullFileName,
    TMPXGeneralCategory aCategory )
    {
    TRAPD( err, CollectionHelperL()->RemoveL( aFullFileName, aCategory ) );

    if ( err == KErrNotFound )
        {
        // Ignore songs not found in MPX DB
        PRINT1( _L( "MM MTP <> DeleteObjectL deletion failed: %S not found" ), &aFullFileName );
        }
    else if ( err != KErrNone )
        {
        User::Leave( err );
        }
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::RenameRecordL
// Rename the filename onto MPX DB
// -----------------------------------------------------------------------------
//
void CMmMtpDpMetadataMpxAccess::RenameObjectL( const TDesC& aOldFileName,
    const TDesC& aNewFileName,
    TMPXGeneralCategory aCategory )
    {
    PRINT2( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::RenameObjectL aOldFileName = %S, aNewFileName = %S" ),
            &aOldFileName,
            &aNewFileName );

    TInt err = KErrNone;
    TRAP( err, CollectionHelperL()->RenameL( aOldFileName, aNewFileName, aCategory ) );

    if ( KErrNotFound == err )
        {
        PRINT1( _L( "MM MTP <> Not found the %S in the MPX DB" ), &aOldFileName );
        }
    else if ( KErrNone != err )
        {
        User::Leave( err );
        }
    PRINT( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::RenameObjectL" ) );
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::SetObjectMetadataValueL
// Updated object metadata in MPX database
// -----------------------------------------------------------------------------
void CMmMtpDpMetadataMpxAccess::SetObjectMetadataValueL( const TUint16 aPropCode,
    const MMTPType& aNewData,
    const TDesC& aFullFileName,
    TMPXGeneralCategory aCategory )
    {
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::SetObjectMetadataValueL aPropCode = 0x%x" ), aPropCode );

    RArray<TInt> contentIDs;
    CleanupClosePushL( contentIDs ); // + contentIDs

    contentIDs.AppendL( KMPXMediaIdGeneral );

    if ( aCategory == EMPXSong )
        {
        contentIDs.AppendL( KMPXMediaIdAudio );
        contentIDs.AppendL( KMPXMediaIdMusic );
        contentIDs.AppendL( KMPXMediaIdMTP );
        }

    CMPXMedia* media = CMPXMedia::NewL( contentIDs.Array() );
    CleanupStack::PushL( media ); // + media

    media->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaGeneralCategory, aCategory );

    // MPXMedia default types
    media->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType, EMPXItem );

    TParsePtrC parse( aFullFileName );
    media->SetTextValueL( KMPXMediaGeneralUri, aFullFileName );
    media->SetTextValueL( KMPXMediaGeneralDrive, parse.Drive() );
    media->SetTObjectValueL( KMPXMediaGeneralModified, EFalse );

    PERFLOGSTART( KSetMetadataValue );
    SetMetadataValueL( aPropCode, aNewData, *media );
    PERFLOGSTOP( KSetMetadataValue );

    // Update the song's metadata with the media object
    PERFLOGSTART( KMpxCollectionSetL );
    CollectionHelperL()->SetL( media );
    PERFLOGSTOP( KMpxCollectionSetL );

    delete iSampleData;
    iSampleData = NULL;

    CleanupStack::PopAndDestroy( 2, &contentIDs ); // - media, contentIDs

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::SetObjectMetadataValueL" ) );
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::SetMetadataValueL
// private, Set property value into MPX object according to property code
// -----------------------------------------------------------------------------
void CMmMtpDpMetadataMpxAccess::SetMetadataValueL( const TUint16 aPropCode,
    const MMTPType& aNewData,
    CMPXMedia& aMediaProp )
    {
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::SetMetadataValueL aPropCode = 0x%x" ), aPropCode );
    CMTPTypeArray* desData = NULL;
    TMTPTypeUint16 uint16Data;
    TMTPTypeUint32 uint32Data;

    TMPXAttributeData attrib( MpxAttribFromPropL( aPropCode ) );

    switch ( aPropCode )
        {
        case EMTPObjectPropCodeName:
        case EMTPObjectPropCodeArtist:
        case EMTPObjectPropCodeGenre:
        case EMTPObjectPropCodeAlbumName:
        case EMTPObjectPropCodeComposer:
        case EMTPObjectPropCodeAlbumArtist:
            {
            const CMTPTypeString& textData = static_cast<const CMTPTypeString&>( aNewData );
            HBufC* string = textData.StringChars().AllocLC();    // + string
            PRINT2( _L( "MM MTP <> SetMetadataValueL des.ptr() = 0x%x, string length = %d" ),string->Des().Ptr(), string->Des().Length() );
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::SetMetadataValueL string = %S" ), string );
            aMediaProp.SetTextValueL( attrib, string->Des() );
            CleanupStack::PopAndDestroy( string );               // - string
            }
            break;

        case EMTPObjectPropCodeTrack:
            {
            MMTPType::CopyL( aNewData, uint16Data );
            TBuf<KMtpMaxStringLength> data;
            data.AppendNum( uint16Data.Value() );
            aMediaProp.SetTextValueL( KMPXMediaMusicAlbumTrack, data );
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::SetMetadataValueL Track = %S" ), &data );
            }
            break;

        case EMTPObjectPropCodeNumberOfChannels:
            {
            MMTPType::CopyL( aNewData, uint16Data );
            // Device currently supports types 1(mono) & 2(Stereo) only
            if ( ( uint16Data.Value() != KMtpChannelMono )
                && ( uint16Data.Value() != KMtpChannelStereo )
                && ( uint16Data.Value() != 0 ) )// 0 not used
                {
                PRINT( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::SetMetadataValueL NumberOfChannels NOT SUPPORT!" ) );
                User::Leave( KErrNotSupported );
                }
            aMediaProp.SetTObjectValueL<TUint32>( KMPXMediaAudioNumberOfChannels,
                uint16Data.Value() );
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::SetMetadataValueL NumberOfChannels = %u" ), uint16Data.Value() );
            }
            break;

        case EMTPObjectPropCodeSampleRate:
        case EMTPObjectPropCodeAudioWAVECodec:
        case EMTPObjectPropCodeAudioBitRate:
        case EMTPObjectPropCodeDuration:
            {
            MMTPType::CopyL( aNewData, uint32Data );
            aMediaProp.SetTObjectValueL<TUint32>( attrib, uint32Data.Value() );
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::SetMetadataValueL uint32 value = %u" ), uint32Data.Value() );
            }
            break;

        case EMTPObjectPropCodeOriginalReleaseDate:
            {
            const CMTPTypeString& textData = static_cast<const CMTPTypeString&>( aNewData );

            TBuf<KMtpMaxStringLength> data;
            data.Copy( textData.StringChars().Left( KMtpMaxDateTimeStringLength ) );
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::SetMetadataValueL 0xDC99 date = %S" ),
                &data );
            if ( data.Length() < KMtpMaxDateTimeStringLength )
                {
                PRINT( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::SetMetadataValueL 0xDC99 date string is too short" ) );
                break;
                }

            TLex dateBuf( data.Left( 4 ) );
            TInt year;
            dateBuf.Val( year );

            dateBuf = data.Mid( 4, 2 );
            TInt month;
            dateBuf.Val( month );

            dateBuf = data.Mid( 6, 2 );
            TInt day;
            dateBuf.Val( day );

            dateBuf = data.Mid( 9, 2 );
            TInt hour;
            dateBuf.Val( hour );

            dateBuf = data.Mid( 11, 2 );
            TInt minute;
            dateBuf.Val( minute );

            dateBuf = data.Mid( 13, 2 );
            TInt second;
            dateBuf.Val( second );

#ifdef _DEBUG
            RDebug::Print( _L( "MM MTP <> SetMetadataValueL 0xDC99 dateTime Year = %d, Month = %d, Day = %d, Hour = %d, Minute = %d, Second = %d" ),
                year, month, day, hour, minute, second );
#endif // _DEBUG
            TDateTime dateTime;
            if ( ( month > 0 && month < 13 )
                && ( day > 0 && day < 32 )
                && ( hour >= 0 && hour < 60 )
                && ( minute >= 0 && minute < 60 )
                && ( second >= 0 && second < 60 ) )
                {
                // microsecond is ignored because MPX doesn't support it, following s60
                dateTime.Set( year,
                    TMonth( --month ),
                    --day,
                    hour,
                    minute,
                    second,
                    0 );
                }
            else
                {
                // date string syntax is wrong
                User::Leave( KErrGeneral );
                }
            TTime time( dateTime );
#ifdef _DEBUG
            RDebug::Print( _L( "MM MTP <> SetMetadataValueL 0xDC99 time = %Ld, Year = %d, Month = %d, Day = %d, Hour = %d, Minute = %d, Second = %d"),
                time.Int64(),
                time.DateTime().Year(),
                time.DateTime().Month(),
                time.DateTime().Day(),
                time.DateTime().Hour(),
                time.DateTime().Minute(),
                time.DateTime().Second() );
#endif // _DEBUG
            aMediaProp.SetTObjectValueL( KMPXMediaMusicYear, time.Int64() );
            }
            break;

        case EMTPObjectPropCodeDescription:
            {
#ifdef __MUSIC_ID_SUPPORT
            // WriteMusicIdsL(*longString);
#else
            desData = CMTPTypeArray::NewLC( EMTPTypeAUINT16 ); // + desData
            MMTPType::CopyL( aNewData, *desData );
            TUint length = desData->NumElements();
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::SetMetadataValueL length = %d" ),
                length );

            length = ( length < KMTPMaxDescriptionLen ) ? length : KMTPMaxDescriptionLen;
            HBufC* text = HBufC::NewLC( length );    // + text
            TPtr ptr = text->Des();

            for ( TUint i = 0; i < length; i++ )
                ptr.Append( desData->ElementUint( i ) );
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::SetMetadataValueL text = %S" ),
                text );
            aMediaProp.SetTextValueL( KMPXMediaGeneralComment, text->Des() );
            CleanupStack::PopAndDestroy( text );    // - text

            CleanupStack::PopAndDestroy( desData ); // - desData
#endif //__MUSIC_ID_SUPPORT
            }
            break;

        case EMTPObjectPropCodeDRMStatus:
            {
            MMTPType::CopyL( aNewData, uint16Data );
            aMediaProp.SetTObjectValueL<TUint16>( attrib, uint16Data.Value() );
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::SetMetadataValueL DRM Status uint16 value = %u" ), uint16Data.Value() );
            }
            break;

        case EMTPObjectPropCodeRepresentativeSampleData:
            {
            const CMTPTypeArray& mtpTypeArray = static_cast<const CMTPTypeArray&>( aNewData );
            TUint32 numElements = mtpTypeArray.NumElements();
            const TDesC& suid = aMediaProp.ValueText( KMPXMediaGeneralUri );
            PRINT2( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::SetMetadataValueL SampleData numElements = %d, suid = %S" ), numElements, &suid );

            delete iSampleData;
            iSampleData = NULL;

            iSampleData = HBufC8::NewL( numElements );
            TPtr8 samplePtr = iSampleData->Des();
            mtpTypeArray.ToDes( samplePtr );
            RFile sampleFile;
            CleanupClosePushL( sampleFile );
            User::LeaveIfError( sampleFile.Replace( iRfs, suid, EFileWrite ) );
            User::LeaveIfError( sampleFile.Write( samplePtr ) );
            aMediaProp.SetTObjectValueL<TInt>( KMPXMediaMTPSampleData, (TInt)iSampleData );
            aMediaProp.SetTObjectValueL<TBool>( attrib, ETrue );
            CleanupStack::PopAndDestroy( &sampleFile );
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::SetMetadataValueL SampleData is ready = %u" ), 1 );
            }
            break;

        default:
            {
            User::Leave( KErrNotSupported );
            }
            break;
        }

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::SetMetadataValueL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::AddSongL
// Adds song info to the database
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataMpxAccess::AddSongL( const TDesC& aFullFileName )
    {
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::AddSongL aFullFileName = %S" ), &aFullFileName );

    // Does a record already exist for this song?
    RArray<TInt> contentIDs;
    CleanupClosePushL( contentIDs ); // + contentIDs
    contentIDs.AppendL( KMPXMediaIdGeneral );

    CMPXMedia* searchMedia = CMPXMedia::NewL( contentIDs.Array() );
    CleanupStack::PopAndDestroy( &contentIDs ); // - contentIDs
    CleanupStack::PushL( searchMedia ); // + searchMedia

    searchMedia->SetTObjectValueL( KMPXMediaGeneralType, EMPXItem );
    searchMedia->SetTObjectValueL( KMPXMediaGeneralCategory, EMPXSong );
    searchMedia->SetTextValueL( KMPXMediaGeneralUri, aFullFileName );

    TParsePtrC parse( aFullFileName );
    searchMedia->SetTextValueL( KMPXMediaGeneralDrive, parse.Drive() );

    RArray<TMPXAttribute> songAttributes;
    CleanupClosePushL( songAttributes ); // + songAttributes
    songAttributes.AppendL( KMPXMediaGeneralUri );

    PRINT( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::AddSongL searchMedia setup with no problems" ) );

    PERFLOGSTART( KMpxCollectionFindAllLBeforeAdd );
    CMPXMedia* foundMedia = CollectionHelperL()->FindAllL( *searchMedia,
        songAttributes.Array() );
    PERFLOGSTOP( KMpxCollectionFindAllLBeforeAdd );

    CleanupStack::PopAndDestroy( &songAttributes ); // - songAttributes
    CleanupStack::PopAndDestroy( searchMedia ); // - searchMedia

    CleanupStack::PushL( foundMedia ); // + foundMedia

    if ( !foundMedia->IsSupported( KMPXMediaArrayCount ) )
        {
        PRINT( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::AddSongL Not supported KMPXMediaArrayCount" ) );
        User::Leave( KErrNotSupported );
        }

    TInt foundItemCount = *foundMedia->Value<TInt>( KMPXMediaArrayCount );

    PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::AddSongL %d Media Objects found in the WMP search" ), foundItemCount );

    if ( foundItemCount > 1 )
        {
        PRINT( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::AddSongL Multiple medias already exist in the collection. Error!!!" ) );
        }
    else
        {
        // Create media properties for the song
        PRINT( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::AddSongL Create media properties" ) );
        RArray<TInt> contentIDs;
        CleanupClosePushL( contentIDs ) ;    // + contentIDs
        contentIDs.AppendL( KMPXMediaIdGeneral );
        contentIDs.AppendL( KMPXMediaIdAudio );
        contentIDs.AppendL( KMPXMediaIdMusic );
        contentIDs.AppendL( KMPXMediaIdMTP );

        CMPXMedia* media = CMPXMedia::NewL( contentIDs.Array() );
        CleanupStack::PopAndDestroy( &contentIDs ); // - contentIDs
        CleanupStack::PushL( media ); // + media

        // MPXMedia default types
        media->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType,
            EMPXItem );
        media->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaGeneralCategory,
            EMPXSong );
        // File Path
        //
        media->SetTextValueL( KMPXMediaGeneralUri, aFullFileName );
        media->SetTextValueL( KMPXMediaGeneralDrive, parse.Drive() );

        if ( foundItemCount == 0 )
            {
            // Set default Metadata
            SetDefaultL( *media );
            PRINT( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::AddSongL Default values set to Media" ) );
            }

        // Update MPX WMP Roundtrip Metadata of the media object
        media->SetTObjectValueL<TBool>( KMPXMediaGeneralDeleted, EFalse );
        media->SetTObjectValueL<TBool>( KMPXMediaGeneralModified, EFalse );
        media->SetTObjectValueL<TBool>( KMPXMediaGeneralSynchronized, ETrue );
        media->SetTObjectValueL<TUint>( KMPXMediaGeneralFlags,
            KMPXMediaGeneralFlagsIsInvalid | KMPXMediaGeneralFlagsIsCorrupted );

        if ( foundItemCount == 0 )
            {
            PERFLOGSTART( KMpxCollectionAddL );
            CollectionHelperL()->AddL( media );
            PERFLOGSTOP( KMpxCollectionAddL );

            PRINT( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::AddSongL Media added into collection" ) );
            }
        else
            {
            PERFLOGSTART( KMpxCollectionSetL );
            CollectionHelperL()->SetL( media );
            PERFLOGSTOP( KMpxCollectionSetL );

            PRINT( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::AddSongL Media metadata updated in collection" ) );
            }

        CleanupStack::PopAndDestroy( media ); // - media
        }

    CleanupStack::PopAndDestroy( foundMedia ); // - foundMedia

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::AddSongL" ) );
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::AddAbstractMediaL
// Adds abstract media to Mpx DB
// -----------------------------------------------------------------------------
//
void CMmMtpDpMetadataMpxAccess::AddAbstractMediaL( const TDesC& aFullFileName, TMPXGeneralCategory aCategory )
    {
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::AddAbstractMediaL aFullFileName = %S" ), &aFullFileName );

    // Does a record already exist for this AbstractMedia?
    RArray<TInt> contentIDs;
    CleanupClosePushL( contentIDs ); // + contentIDs
    contentIDs.AppendL( KMPXMediaIdGeneral );

    CMPXMedia* searchMedia = CMPXMedia::NewL( contentIDs.Array() );
    CleanupStack::PopAndDestroy( &contentIDs ); // - contentIDs
    CleanupStack::PushL( searchMedia ); // + searchMedia

    searchMedia->SetTObjectValueL( KMPXMediaGeneralType, EMPXItem );
    searchMedia->SetTObjectValueL( KMPXMediaGeneralCategory, aCategory );
    searchMedia->SetTextValueL( KMPXMediaGeneralUri, aFullFileName );

    RArray<TMPXAttribute> abstractMediaAttributes;
    CleanupClosePushL( abstractMediaAttributes ); // + abstractMediaAttributes
    abstractMediaAttributes.AppendL( KMPXMediaGeneralId );
    abstractMediaAttributes.AppendL( KMPXMediaGeneralTitle );
    abstractMediaAttributes.AppendL( KMPXMediaGeneralUri );

    PERFLOGSTART( KMpxCollectionFindAllLBeforeAdd );
    CMPXMedia* foundMedia = CollectionHelperL()->FindAllL( *searchMedia,
        abstractMediaAttributes.Array() );
    PERFLOGSTOP( KMpxCollectionFindAllLBeforeAdd );

    CleanupStack::PopAndDestroy( &abstractMediaAttributes ); // - abstractMediaAttributes
    CleanupStack::PopAndDestroy( searchMedia ); // - searchMedia
    CleanupStack::PushL( foundMedia ); // + foundMedia

    if ( !foundMedia->IsSupported( KMPXMediaArrayCount ) )
        User::Leave( KErrNotSupported );

    TInt foundItemCount = *foundMedia->Value<TInt>( KMPXMediaArrayCount );

    if ( foundItemCount != 0 )
        {
        PRINT( _L( "MM MTP <> Abstract Media already exists in the collection" ) );
        }
    else
        {
        // Creat media properties for the abstractMedia
        PRINT( _L( "MM MTP <> Create abstract media properties" ) );
        RArray<TInt> contentIDs;
        CleanupClosePushL( contentIDs ); // + contentIDs
        contentIDs.AppendL( KMPXMediaIdGeneral );

        CMPXMedia* media = CMPXMedia::NewL( contentIDs.Array() );
        CleanupStack::PopAndDestroy( &contentIDs ); // - contentIDs
        CleanupStack::PushL( media ); // + media

        CMPXMediaArray* abstractMediaArray = CMPXMediaArray::NewL();
        CleanupStack::PushL( abstractMediaArray ); // + abstractMediaArray;

        // MPXMedia default types
        media->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType,
            EMPXItem );
        media->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaGeneralCategory,
            aCategory );
        // File Path
        //
        media->SetTextValueL( KMPXMediaGeneralUri, aFullFileName );

        TParsePtrC parse( aFullFileName );
        media->SetTextValueL( KMPXMediaGeneralDrive, parse.Drive() );
        media->SetTextValueL( KMPXMediaGeneralTitle, parse.Name() );
        media->SetTObjectValueL<TBool>( KMPXMediaGeneralSynchronized, ETrue );
        media->SetCObjectValueL( KMPXMediaArrayContents, abstractMediaArray );
        media->SetTObjectValueL( KMPXMediaArrayCount, abstractMediaArray->Count() );
        if ( aCategory == EMPXAbstractAlbum )
            {
            media->SetTObjectValueL( KMPXMediaMTPSampleDataFlag, EFalse );
            }

        PERFLOGSTART( KMpxCollectionAddL );
        CollectionHelperL()->AddL( media );
        PERFLOGSTOP( KMpxCollectionAddL );

        // Clear the array
        CleanupStack::PopAndDestroy( abstractMediaArray ); // - abstractMediaArray

        CleanupStack::PopAndDestroy( media ); // - media
        }

    CleanupStack::PopAndDestroy( foundMedia ); // - foundMedia

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::AddAbstractMediaL" ) );
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::SetReferenceL
// Set abstract media to DB
// -----------------------------------------------------------------------------
//
void CMmMtpDpMetadataMpxAccess::SetReferenceL( const TDesC& aRefOwnerName,
    CDesCArray& aRefFileArray,
    TMPXGeneralCategory aCategory )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::SetReferenceL" ) );
    CMPXMediaArray* abstractMediaArray = CMPXMediaArray::NewL();
    CleanupStack::PushL( abstractMediaArray ); // + abstractMediaArray

    TUint count = aRefFileArray.Count();
    for ( TUint j = 0; j < count; j++ )
        {
        // Creat media properties for the song
        RArray<TInt> contentIDs;
        CleanupClosePushL( contentIDs ); // + contentIDs
        contentIDs.AppendL( KMPXMediaIdGeneral );
        contentIDs.AppendL( KMPXMediaIdAudio );
        contentIDs.AppendL( KMPXMediaIdMusic );
        contentIDs.AppendL( KMPXMediaIdMTP );

        CMPXMedia* media = CMPXMedia::NewL( contentIDs.Array() );
        CleanupStack::PopAndDestroy( &contentIDs ); // - contentIDs
        CleanupStack::PushL( media ); // + media

        // MPXMedia default types
        media->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType,
            EMPXItem );
        media->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaGeneralCategory,
            EMPXSong );

        media->SetTextValueL( KMPXMediaGeneralUri, aRefFileArray[j] );
        // TODO: need to confirm that should set drive letter or storage root path.
        TParsePtrC parse( aRefFileArray[j] );
        media->SetTextValueL( KMPXMediaGeneralDrive, parse.Drive() );
        media->SetTObjectValueL( KMPXMediaGeneralModified, EFalse );

        // Add media into array contents
        abstractMediaArray->AppendL( media );

        CleanupStack::Pop( media ); // - media
        }

    RArray<TInt> contentIDs;
    CleanupClosePushL( contentIDs ); // + contentIDs
    contentIDs.AppendL( KMPXMediaIdGeneral );

    CMPXMedia* abstractMedia = CMPXMedia::NewL( contentIDs.Array() );
    CleanupStack::PopAndDestroy( &contentIDs ); // - contentIDs
    CleanupStack::PushL( abstractMedia ); // + abstractMedia

    abstractMedia->SetTObjectValueL( KMPXMediaGeneralType, EMPXItem );
    abstractMedia->SetTObjectValueL( KMPXMediaGeneralCategory, aCategory );
    abstractMedia->SetTextValueL( KMPXMediaGeneralUri, aRefOwnerName );
    TParsePtrC parse( aRefOwnerName );
    abstractMedia->SetTextValueL( KMPXMediaGeneralDrive, parse.Drive() );
    abstractMedia->SetTObjectValueL<TBool>( KMPXMediaGeneralSynchronized,
        ETrue );
    abstractMedia->SetCObjectValueL( KMPXMediaArrayContents, abstractMediaArray );
    abstractMedia->SetTObjectValueL( KMPXMediaArrayCount,
        abstractMediaArray->Count() );

    // Update the duplicate abstractMedia(s) with the new abstractMedia array
    PERFLOGSTART( KMpxCollectionSetReferenceL );
    CollectionHelperL()->SetL( abstractMedia );
    PERFLOGSTOP( KMpxCollectionSetReferenceL );

    CleanupStack::PopAndDestroy( abstractMedia ); // - abstractMedia
    CleanupStack::PopAndDestroy( abstractMediaArray ); // - abstractMediaArray

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::SetReferenceL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::SetStorageRootL
// Set storage root of the MPX DB
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataMpxAccess::SetStorageRootL( const TDesC& aStorageRoot )
    {
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::SetStorageRoot aStoreRoot = %S" ), &aStorageRoot );

    // get the drive number
    TParsePtrC pathParser( aStorageRoot );
    TChar driveChar( pathParser.Drive()[0] );

    TInt driveNumber;
    User::LeaveIfError( RFs::CharToDrive( driveChar, driveNumber ) );
    PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::SetStorageRoot drive number = %d" ), driveNumber );

    // get root path
    User::LeaveIfError( PathInfo::GetRootPath( iStoreRoot, driveNumber ) );

    PRINT1( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::SetStorageRoot root path: %S" ), &iStoreRoot );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::SetDefaultL
// Sets all of the default media properties
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataMpxAccess::SetDefaultL( CMPXMedia& aMediaProp )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::SetDefaultL" ) );
    // Title
    aMediaProp.SetTextValueL( KMPXMediaGeneralTitle, KNullDesC );
    // Comment
    aMediaProp.SetTextValueL( KMPXMediaGeneralComment, KNullDesC );
    // Artist
    aMediaProp.SetTextValueL( KMPXMediaMusicArtist, KNullDesC );
    // Album
    aMediaProp.SetTextValueL( KMPXMediaMusicAlbum, KNullDesC );
    // Year
    //aMediaProp.SetTextValueL(KMPXMediaMusicYear, KNullDesC);  // should never set year to KNullDesC, it is a TInt64 value
    // Track
    aMediaProp.SetTextValueL( KMPXMediaMusicAlbumTrack, KNullDesC );
    // Genre
    aMediaProp.SetTextValueL( KMPXMediaMusicGenre, KNullDesC );
    // Composer
    aMediaProp.SetTextValueL( KMPXMediaMusicComposer, KNullDesC );
    // Album Artist
    aMediaProp.SetTextValueL( KMPXMediaMusicAlbumArtist, KNullDesC );
    // URL
    aMediaProp.SetTextValueL( KMPXMediaMusicURL, KNullDesC );

    // add below for P4S failed case, that get metadata from cached CMPXMedia object
    // DateAdded
    TTime time;
    time.HomeTime();
    aMediaProp.SetTObjectValueL( KMPXMediaGeneralDate, time.Int64() );
    // SampleRate
    aMediaProp.SetTObjectValueL<TUint32>( KMPXMediaAudioSamplerate, 0 );
    // AudioBitrate
    aMediaProp.SetTObjectValueL<TUint32>( KMPXMediaAudioBitrate, 0 );
    // Duration
    aMediaProp.SetTObjectValueL<TInt>( KMPXMediaGeneralDuration, 0 );
    // DrmStatus
    aMediaProp.SetTObjectValueL<TUint16>( KMPXMediaMTPDrmStatus, 0 );
    // NumberOfChannels
    aMediaProp.SetTObjectValueL<TUint32>( KMPXMediaAudioNumberOfChannels, 0 );
    // AudioCodec
    aMediaProp.SetTObjectValueL<TUint32>( KMPXMediaAudioAudioCodec, 0 );

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::SetDefaultL" ) );
    }

TMPXAttributeData CMmMtpDpMetadataMpxAccess::MpxAttribFromPropL( const TUint16 aPropCode )
    {
    const TMetadataTable KMetadataTable[] =
        {
            { EMTPObjectPropCodeName,                    KMPXMediaGeneralTitle },
            { EMTPObjectPropCodeArtist,                  KMPXMediaMusicArtist },
            { EMTPObjectPropCodeAlbumName,               KMPXMediaMusicAlbum },
            { EMTPObjectPropCodeDateModified,            KMPXMediaGeneralDate },
            { EMTPObjectPropCodeDateAdded,               KMPXMediaGeneralDate },
            { EMTPObjectPropCodeDuration,                KMPXMediaGeneralDuration },
            { EMTPObjectPropCodeTrack,                   KMPXMediaMusicAlbumTrack },
            { EMTPObjectPropCodeComposer,                KMPXMediaMusicComposer },
            { EMTPObjectPropCodeOriginalReleaseDate,     KMPXMediaMusicYear },
            { EMTPObjectPropCodeGenre,                   KMPXMediaMusicGenre },
            { EMTPObjectPropCodeDRMStatus,               KMPXMediaMTPDrmStatus },
            { EMTPObjectPropCodeDescription,             KMPXMediaGeneralComment },
            { EMTPObjectPropCodeNumberOfChannels,        KMPXMediaAudioNumberOfChannels },
            { EMTPObjectPropCodeAudioBitRate,            KMPXMediaAudioBitrate },
            { EMTPObjectPropCodeSampleRate,              KMPXMediaAudioSamplerate },
            { EMTPObjectPropCodeAudioWAVECodec,          KMPXMediaAudioAudioCodec },
            { EMTPObjectPropCodeAlbumArtist,             KMPXMediaMusicAlbumArtist },
            { EMTPObjectPropCodeRepresentativeSampleData,KMPXMediaMTPSampleDataFlag }
        };

    TInt i = 0;
    TInt count = sizeof( KMetadataTable ) / sizeof( KMetadataTable[0] );
    while ( ( KMetadataTable[i].iPropCode != aPropCode )
        && ( i < count ) )
        {
        i++;
        }

    if ( i == count )
        {
        // Not supported by MPX, shouldn't call this function
        PRINT( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::MpxAttribFromPropL NOT SUPPORTED ATTRIBUTE" ) );
        User::Leave( KErrNotSupported );
        }

    return KMetadataTable[i].iMpxAttrib;
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::GetAllAbstractMediaL
// Get all abstract medias from MPX database in the assigned store
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataMpxAccess::GetAllAbstractMediaL( const TDesC& aStoreRoot,
    CDesCArray& aAbstractMedias,
    TMPXGeneralCategory aCategory )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::GetAllAbstractMediaL" ) );

    SetStorageRootL( aStoreRoot );

    RArray<TInt> contentIDs;
    CleanupClosePushL( contentIDs ); // + contentIDs
    contentIDs.AppendL( KMPXMediaIdGeneral );

    CMPXMedia* searchMedia = CMPXMedia::NewL( contentIDs.Array() );
    CleanupStack::PopAndDestroy( &contentIDs ); // - contentIDs
    CleanupStack::PushL( searchMedia ); // + searchMedia

    searchMedia->SetTObjectValueL( KMPXMediaGeneralType, EMPXItem );
    searchMedia->SetTObjectValueL( KMPXMediaGeneralCategory, aCategory );
    searchMedia->SetTextValueL( KMPXMediaGeneralDrive, iStoreRoot );

    RArray<TMPXAttribute> abstractMediaAttributes;
    CleanupClosePushL( abstractMediaAttributes ); // + abstractMediaAttributes
    abstractMediaAttributes.AppendL( KMPXMediaGeneralId );
    abstractMediaAttributes.AppendL( KMPXMediaGeneralTitle );

    abstractMediaAttributes.AppendL( KMPXMediaGeneralUri );

    PERFLOGSTART( KMpxCollectionGetAbstractMedia );
    CMPXMedia* foundMedia = CollectionHelperL()->FindAllL( *searchMedia,
        abstractMediaAttributes.Array() );
    PERFLOGSTOP( KMpxCollectionGetAbstractMedia );

    CleanupStack::PopAndDestroy( &abstractMediaAttributes ); // - abstractMediaAttributes
    CleanupStack::PopAndDestroy( searchMedia ); // - searchMedia
    CleanupStack::PushL( foundMedia ); // + foundMedia

    if ( !foundMedia->IsSupported( KMPXMediaArrayCount ) )
        {
        User::Leave( KErrNotSupported );
        }

    TInt count = *foundMedia->Value<TInt>( KMPXMediaArrayCount );

    PRINT1( _L("MM MTP <> CMmMtpDpMetadataMpxAccess::GetAllAbstractMediaL [%d] abstractMedias found in Database"), count );

    if ( count > 0 )
        {
        if ( !foundMedia->IsSupported( KMPXMediaArrayContents ) )
            {
            User::Leave( KErrNotSupported );
            }

        CMPXMediaArray* mediaArray = foundMedia->Value<CMPXMediaArray> ( KMPXMediaArrayContents );
        for ( TInt i = 0; i < count; i++ )
            aAbstractMedias.AppendL( ( *mediaArray )[i]->ValueText( KMPXMediaGeneralUri ) );
        }

    CleanupStack::PopAndDestroy( foundMedia ); // - foundMedia

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::GetAllAbstractMediaL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::GetAllReferenceL
// Get all references of specified playlist
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataMpxAccess::GetAllReferenceL( const TDesC& aRefOwnerSuid,
    TMPXGeneralCategory aCategory,
    CDesCArray& aReferences )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::GetAllReferenceL" ) );

    // 1. Get CMPXMedia object by the reference owner suid
    RArray<TMPXAttribute> attributes;
    CleanupClosePushL( attributes ); // + attributes
    attributes.AppendL( KMPXMediaGeneralId );
    attributes.AppendL( KMPXMediaGeneralTitle );
    attributes.AppendL( KMPXMediaGeneralUri );

    CMPXMedia* refOwner = CollectionHelperL()->GetL( aRefOwnerSuid,
        attributes.Array(),
        aCategory );

    __ASSERT_DEBUG( refOwner, Panic( EMmMTPDpObjectNull ) );

    CleanupStack::PushL( refOwner ); // + refOwner

    // 2. Extract the general id from the found object
    if ( !refOwner->IsSupported( KMPXMediaGeneralId ) )
        PRINT( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::GetAllReferenceL Before GeneralId" ) );

    TUint32 abstractMediaId = *( *refOwner ).Value<TMPXItemId>( KMPXMediaGeneralId );
    PRINT( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::GetAllReferenceL After GeneralId" ) );

    CleanupStack::PopAndDestroy( refOwner );    // - refOwner
    CleanupStack::PopAndDestroy( &attributes ); // - attributes

    // find the media object that contains a list of songs in the playlist
    RArray<TInt> contentIDs;
    CleanupClosePushL( contentIDs ); // + contentIDs
    contentIDs.AppendL( KMPXMediaIdGeneral );

    CMPXMedia* searchMedia = CMPXMedia::NewL( contentIDs.Array() );
    CleanupStack::PopAndDestroy( &contentIDs ); // - contentIDs
    CleanupStack::PushL( searchMedia ); // + searchMedia

    searchMedia->SetTObjectValueL( KMPXMediaGeneralType, EMPXGroup );
    searchMedia->SetTObjectValueL( KMPXMediaGeneralCategory, EMPXSong );
    searchMedia->SetTObjectValueL<TMPXItemId>( KMPXMediaGeneralId, abstractMediaId );

    RArray<TMPXAttribute> songAttributes;
    CleanupClosePushL( songAttributes ); // + songAttributes
    songAttributes.AppendL( KMPXMediaGeneralId );
    songAttributes.AppendL( KMPXMediaGeneralUri );

    PERFLOGSTART( KMpxCollectionGetReference );
    CMPXMedia* foundMedia = CollectionHelperL()->FindAllL( *searchMedia,
        songAttributes.Array() );
    PERFLOGSTOP( KMpxCollectionGetReference );

    CleanupStack::PopAndDestroy( &songAttributes ); // - songAttributes
    CleanupStack::PopAndDestroy( searchMedia ); // - searchMedia
    CleanupStack::PushL( foundMedia ); // + foundMedia

    if ( !foundMedia->IsSupported( KMPXMediaArrayCount ) )
        {
        User::Leave( KErrNotSupported );
        }

    // Number of references
    TInt numOfRefs = *foundMedia->Value<TInt> ( KMPXMediaArrayCount );
    if ( numOfRefs > 0 )
        {
        if ( !foundMedia->IsSupported( KMPXMediaArrayContents ) )
            {
            User::Leave( KErrNotSupported );
            }

        const CMPXMediaArray* refArray = foundMedia->Value<CMPXMediaArray> (
            KMPXMediaArrayContents );

        // Import the references
        for ( TInt i = 0; i < numOfRefs; i++ )
            {
            const CMPXMedia* refMedia = ( *refArray )[i];

            if ( !refMedia->IsSupported( KMPXMediaGeneralUri ) )
                {
                User::Leave( KErrNotSupported );
                }

            // may replace the following 3 statements into the following:
            // AppendL(refMedia->ValueText(KMPXMediaGeneralUri));
            HBufC* musicFileLocation = refMedia->ValueText( KMPXMediaGeneralUri ).AllocLC();    // + musicFileLocation
            aReferences.AppendL( *musicFileLocation );

            PRINT1( _L("MM MTP <> CMmMtpDpMetadataMpxAccess::GetAllReferenceL, [%S] found from MPX db"), musicFileLocation );

            CleanupStack::PopAndDestroy( musicFileLocation ); // - musicFileLocation
            }
        }

    CleanupStack::PopAndDestroy( foundMedia ); // - foundMedia

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::GetAllReferenceL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::GetModifiedContentL
// Get modified content
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataMpxAccess::GetModifiedContentL( TInt& arrayCount,
    CDesCArray& aModifiedcontent )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::GetModifiedContentL" ) );
    CMPXMedia* foundMedia;
    TInt foundItemCount( 0 );
    HBufC* musicFileLocation( NULL );

     // Modified content
    PRINT( _L( "MM MTP <> Modified contents are:" ) );

    foundMedia = FindWMPMediaLC( KMPXMediaGeneralModified, ETrue ); // + foundMedia
    foundItemCount = *foundMedia->Value<TInt>( KMPXMediaArrayCount );

    PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::GetModifiedContentL() found %d Media Objects" ), foundItemCount );

    if ( foundItemCount )
        {
        if ( !foundMedia->IsSupported( KMPXMediaArrayContents ) )
            {
            User::Leave( KErrNotSupported );
            }

        const CMPXMediaArray* foundArray = foundMedia->Value<CMPXMediaArray>( KMPXMediaArrayContents );

        for ( TInt j = 0; j < foundItemCount; j++ )
            {
            CMPXMedia* media = CMPXMedia::NewL( *(*foundArray)[j] );
            CleanupStack::PushL( media ); // + media

            if ( !media->IsSupported( KMPXMediaGeneralUri ) )
                {
                User::Leave( KErrNotSupported );
                }

            musicFileLocation = media->ValueText( KMPXMediaGeneralUri ).AllocLC(); // + musicFileLoaction

            aModifiedcontent.AppendL( *musicFileLocation );
            arrayCount++;
            PRINT1( _L("arrayCount = %d"), arrayCount);

            CleanupStack::PopAndDestroy( musicFileLocation ); // - musicFileLocation
            CleanupStack::PopAndDestroy( media ); // - media
            }
        }

    CleanupStack::PopAndDestroy( foundMedia ); // + foundMedia
    PRINT( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::GetModifiedContentL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::IsExistL
// if the playlsit exist in the MPX DB
// ---------------------------------------------------------------------------
//
TBool CMmMtpDpMetadataMpxAccess::IsExistL( const TDesC& aSuid )
    {
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataMpxAccess::IsExist aSuid(%S)" ), &aSuid );
    RArray<TInt> contentIDs;
    CleanupClosePushL( contentIDs ); // + contentIDs
    contentIDs.AppendL( KMPXMediaIdGeneral );

    CMPXMedia* searchMedia = CMPXMedia::NewL( contentIDs.Array() );
    CleanupStack::PopAndDestroy( &contentIDs ); // - contentIDs
    CleanupStack::PushL( searchMedia ); // + searchMedia

    searchMedia->SetTObjectValueL( KMPXMediaGeneralType, EMPXItem );
    searchMedia->SetTObjectValueL( KMPXMediaGeneralCategory, EMPXPlaylist );
    searchMedia->SetTextValueL( KMPXMediaGeneralUri, aSuid );

    TParsePtrC parse( aSuid );
    searchMedia->SetTextValueL( KMPXMediaGeneralDrive, parse.Drive() );

    RArray<TMPXAttribute> playlistAttributes;
    CleanupClosePushL( playlistAttributes ); // + playlistAttributes
    playlistAttributes.AppendL( KMPXMediaGeneralId );
    playlistAttributes.AppendL( KMPXMediaGeneralTitle );
    playlistAttributes.AppendL( KMPXMediaGeneralUri );

    CMPXMedia* foundMedia = CollectionHelperL()->FindAllL( *searchMedia,
        playlistAttributes.Array() );

    CleanupStack::PopAndDestroy( &playlistAttributes ); // - playlistAttributes
    CleanupStack::PopAndDestroy( searchMedia ); // - searchMedia
    CleanupStack::PushL( foundMedia ); // + foundMedia

    if ( !foundMedia->IsSupported( KMPXMediaArrayCount ) )
        User::Leave( KErrNotSupported );

    TInt foundItemCount = *foundMedia->Value<TInt>( KMPXMediaArrayCount );

    CleanupStack::PopAndDestroy( foundMedia ); // - foundMedia

    PRINT1( _L( "MM MTP <= CMmMtpDpMetadataMpxAccess::IsExist foundItemCount(%d)" ), foundItemCount );
    return ( foundItemCount > 0 ? ETrue : EFalse );
    }

//end of file
