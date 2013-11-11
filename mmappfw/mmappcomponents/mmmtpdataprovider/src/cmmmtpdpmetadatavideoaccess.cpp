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
 * Description:  Meta data Video access
 *
 */

#include <ContentListingFactory.h>
#include <MCLFContentListingEngine.h>
#include <driveinfo.h>
#include <pathinfo.h>
#include <bautils.h>

// from Symbian MTP
#include <mtp/tmtptypeuint16.h>
#include <mtp/tmtptypeuint32.h>
#include <mtp/tmtptypeuint64.h>
#include <mtp/tmtptypeuint128.h>
#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypearray.h>
#include <mtp/cmtpobjectmetadata.h>
#include <mtp/mmtpdataproviderframework.h>
#include <mtp/mmtpreferencemgr.h>
#include <mtp/mtpprotocolconstants.h>

#include "cmmmtpdpmetadatavideoaccess.h"
#include "mmmtpdplogger.h"
#include "mmmtpdputility.h"
#include "tmmmtpdppanic.h"
#include "mmmtpvideodbdefs.h"
#include "tobjectdescription.h"

_LIT( KMTPNoBackupFolder, "nobackup\\" );

#ifdef _DEBUG
static const TInt KMtpMaxStringDescLength = KMtpMaxStringLength - 1;
#endif

_LIT( KEmptyText, "" );
// Database interface
_LIT( KSelect, "SELECT " );
_LIT( KAllColumns, "*" );
_LIT( KFrom, " FROM " );
_LIT( KWhere, " WHERE " );
_LIT( KNot, " NOT " );
_LIT( KEquals, " = " );
_LIT( KTrue, "1" );

const TInt KMaxQueryLength = 512;
const TInt KMtpCompactInterval = 50;  // Compact every ....

CMmMtpDpMetadataVideoAccess* CMmMtpDpMetadataVideoAccess::NewL( RFs& aRfs )
    {
    CMmMtpDpMetadataVideoAccess* me = new( ELeave ) CMmMtpDpMetadataVideoAccess( aRfs );
    CleanupStack::PushL( me );
    me->ConstructL();
    CleanupStack::Pop( me );

    return me;
    }

CMmMtpDpMetadataVideoAccess::CMmMtpDpMetadataVideoAccess( RFs& aRfs ) :
    iRfs( aRfs ),
    iDbState( ENoRecord ),
    iDbOpened( EFalse )
    {
    // Do nothing
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::ConstructL
// Second-phase
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::ConstructL()
    {
    iQueryText = HBufC::NewL( KMaxQueryLength );

    User::LeaveIfError( iDbsSession.Connect() );

    TInt err = DriveInfo::GetDefaultDrive( DriveInfo::EDefaultPhoneMemory, iStoreNum );
    PRINT1( _L( "MM MTP <> CMmMtpDpMetadataVideoAccess::ConstructL, EDefaultPhoneMemory err = %d" ), err );
    User::LeaveIfError( err );

    err = OpenDatabase();

    if ( KErrNone != err )
        {
        PRINT1( _L( "CMmMtpDpMetadataVideoAccess::ConstructL OpenDatabase err = %d" ), err );
        }
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::OpenDatabase
// Open data base
// ---------------------------------------------------------------------------
//
TInt CMmMtpDpMetadataVideoAccess::OpenDatabase()
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::OpenDatabase" ) );

    TFileName dbFileName;
    TDriveUnit dbDrive( iStoreNum );
    iRfs.PrivatePath( dbFileName );
    dbFileName.Insert( 0, dbDrive.Name() );
    dbFileName.Append( KMTPNoBackupFolder );
    dbFileName.Append( KMtpVideoDb );
    TInt err = iRfs.MkDirAll( dbFileName );

    if ( err == KErrNone || err == KErrAlreadyExists )
        {
        TRAP( err, iFileStore = CPermanentFileStore::OpenL( iRfs, dbFileName, EFileRead | EFileWrite ) );

        if ( err == KErrNone )
            TRAP( err, iFileStore->SetTypeL( iFileStore->Layout() ) );

        if ( err == KErrNone )
            {
            TRAP( err, iDatabase.OpenL( iFileStore, iFileStore->Root() ) );
            PRINT1( _L( "MM MTP <> OpenDatabase RDbNamedDatabase::OpenL, err = %d" ), err );
            }

        if ( err != KErrNone )
            {
            iRfs.Delete( dbFileName );   // delete first before creating a new one
            TRAP( err, iFileStore = CPermanentFileStore::CreateL( iRfs, dbFileName, EFileRead | EFileWrite ) );
            PRINT1( _L( "MM MTP <> OpenDatabase RDbNamedDatabase::CreateL, err = %d" ), err );

            if ( err == KErrNone )
                {
                TRAP( err, iFileStore->SetTypeL( iFileStore->Layout() ) );

                TStreamId streamId = 0;

                if ( err == KErrNone )
                    TRAP( err, streamId = iDatabase.CreateL( iFileStore ) );

                if ( err == KErrNone )
                    TRAP( err, iFileStore->SetRootL( streamId ) );

                if ( err == KErrNone )
                    TRAP( err, CreateDatabaseTablesL() );

                if ( KErrNone != err )
                    {
                    iDatabase.Close();
                    iRfs.Delete( dbFileName );
                    }
                }
            }
        }

    if ( err == KErrNone )
        {
        iDbOpened = ETrue;
        }

    PRINT1( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::OpenDatabase err = %d" ), err );
    return err;
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::OpenDatabaseL
// Open data base
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::OpenDatabaseL()
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::OpenDatabaseL" ) );
    if ( OpenDatabase() != KErrNone )
        User::Leave( KErrGeneral );
    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::OpenDatabaseL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::~CMmMtpDpMetadataVideoAccess
// Destructor
// ---------------------------------------------------------------------------
//
CMmMtpDpMetadataVideoAccess::~CMmMtpDpMetadataVideoAccess()
    {
    delete iQueryText;
    delete iColSet;
    iRecordSet.Close();
    iDatabase.Close();
    iDbsSession.Close();
    delete iFileStore;
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::CreateDatabaseTablesL
// Case where a new memory card is used and the player has not been opened
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::CreateDatabaseTablesL()
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::CreateDatabaseTablesL" ) );
    // Video table
    RBuf formatBuf;
    User::LeaveIfError( formatBuf.Create( 5 * KMaxQueryLength ) );
    CleanupClosePushL( formatBuf ); // + foramtBuf

    formatBuf.Append( KMtpVideoStartCreateTable );
    formatBuf.Append( KMtpVideoTable );
    formatBuf.Append( KMtpVideoOpenBracket );

    formatBuf.Append( KMtpVideoLocation );
    formatBuf.Append( KMtpVideoLocationType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoName );
    formatBuf.Append( KMtpVideoNameType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoArtist );
    formatBuf.Append( KMtpVideoArtistType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoTrack );
    formatBuf.Append( KMtpVideoTrackType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoGenre );
    formatBuf.Append( KMtpVideoGenreType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoAlbumName );
    formatBuf.Append( KMtpVideoAlbumNameType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoComposer );
    formatBuf.Append( KMtpVideoComposerType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoOrigReleaseDate );
    formatBuf.Append( KMtpVideoOrigReleaseDateType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoComment );
    formatBuf.Append( KMtpVideoCommentType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoWidth );
    formatBuf.Append( KMtpVideoWidthType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoHeight );
    formatBuf.Append( KMtpVideoHeightType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoDuration );
    formatBuf.Append( KMtpVideoDurationType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoSampleRate );
    formatBuf.Append( KMtpVideoSampleRateType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoNumberOfChannels );
    formatBuf.Append( KMtpVideoNumberOfChannelsType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoAudioCodec );
    formatBuf.Append( KMtpVideoAudioCodecType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoAudioBitrate );
    formatBuf.Append( KMtpVideoAudioBitrateType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoVideoCodec );
    formatBuf.Append( KMtpVideoVideoCodecType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoVideoBitrate );
    formatBuf.Append( KMtpVideoVideoBitrateType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoFramesPer1000Sec );
    formatBuf.Append( KMtpVideoFramesPer1000SecType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoKeyFrameDistance );
    formatBuf.Append( KMtpVideoKeyFrameDistanceType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoScanType );
    formatBuf.Append( KMtpVideoScanTypeType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoEncodingProfile );
    formatBuf.Append( KMtpVideoEncodingProfileType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoParentalRating );
    formatBuf.Append( KMtpVideoParentalRatingType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoDRM );
    formatBuf.Append( KMtpVideoDRMType );
    formatBuf.Append( KMtpVideoCommaSign );

    formatBuf.Append( KMtpVideoDeleted );
    formatBuf.Append( KMtpVideoDeletedType );
    formatBuf.Append( KMtpVideoCloseBracket );

    // Execute the SQL statement.
    User::LeaveIfError( iDatabase.Execute( formatBuf ) );
    PRINT( _L( "MM MTP <> Video Table Created" ) );

    CleanupStack::PopAndDestroy( &formatBuf ); // - foramtBuf
    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::CreateDatabaseTablesL" ) );
    }

// For performance, better to regularly compact the MC database.
void CMmMtpDpMetadataVideoAccess::CompactDbIfNecessaryL()
    {
    iRecordCount++;

    if ( iRecordCount > KMtpCompactInterval )
        {
        iRecordCount = 0;

        PRINT( _L( "MM MTP <> Compacting database file..." ) );

        if ( IsDatabaseOpened() )
            User::LeaveIfError( iDatabase.Compact() );
        }
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::CleanupDbIfNecessaryL
// Cleanup Database
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::CleanupDbIfNecessaryL()
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::CleanupDbIfNecessaryL" ) );

    ExecuteQueryL( KAllColumns, KMtpVideoTable, KMtpVideoDeleted, KTrue, EFalse, EFalse );
    PRINT1( _L( "MM MTP <> CleanupDbIfNecessaryL Database deleted count = %d" ), iRecordSet.CountL() );

    for ( iRecordSet.FirstL(); iRecordSet.AtRow(); iRecordSet.NextL() )
        {
#ifdef _DEBUG
        HBufC* data = ReadLongTextL( KMtpVideoLocation );

        PRINT1( _L( "MM MTP <> CleanupDbIfNecessaryL removing %S from database" ), data );
        delete data;
        data = NULL;
#endif
        iRecordSet.DeleteL();
        }

    delete iColSet;
    iColSet = NULL;
    iRecordSet.Close();
    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::CleanupDbIfNecessaryL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::IdentifyDeletedFilesL
// Identify deleted files
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::IdentifyDeletedFilesL()
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::IdentifyDeletedFilesL" ) );

    ExecuteQueryL( KAllColumns, KMtpVideoTable, KMtpVideoLocation, KNullDesC, ETrue );
    PRINT1( _L( "MM MTP <> CleanupDbIfNecessaryL Database total count = %d" ), iRecordSet.CountL() );

    for ( iRecordSet.FirstL(); iRecordSet.AtRow(); iRecordSet.NextL() )
        {
        HBufC* data = ReadLongTextL( KMtpVideoLocation );
        CleanupStack::PushL( data );

        if ( !FileExists( *data ) )
            {
            iRecordSet.UpdateL();
            PRINT1( _L( "MM MTP <> IdentifyDeletedFilesL marking %S as deleted" ), data );

            TDbColNo num = iColSet->ColNo( KMtpVideoDeleted );

            iRecordSet.SetColL( num, 1 );
            iRecordSet.PutL();
            }

        CleanupStack::PopAndDestroy( data );
        }

    delete iColSet;
    iColSet = NULL;
    iRecordSet.Close();

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::IdentifyDeletedFilesL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::ReadLongTextL
// Read from Data base
// ---------------------------------------------------------------------------
//
HBufC* CMmMtpDpMetadataVideoAccess::ReadLongTextL( const TDesC& aColumn )
    {
    TDbColNo num = iColSet->ColNo( aColumn );

    // The row must have previously been read into the rowset
    // using RDbRowSet::GetL().
    iRecordSet.GetL();

    TInt len = iRecordSet.ColLength( num );

    HBufC* buf = HBufC::NewLC( len ); // + buf
    TPtr value( buf->Des() );

    if ( len > 0 )
        {
        RDbColReadStream strm;
        strm.OpenLC( iRecordSet, num );
        strm.ReadL( value, len );
        strm.Close();
        CleanupStack::PopAndDestroy( &strm );
        }
    else
        {
        value.SetLength( 0 );
        }

    CleanupStack::Pop( buf ); // - buf
    PRINT2( _L( "MM MTP <> ReadLongTextL Metadata value for %S is \"%S\"" ), &aColumn, buf );
    return buf;
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::WriteLongTextL
// Utility to write to the database
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::WriteLongTextL( const TDesC& aColumn,
    const TDesC& aValue )
    {
    PRINT2( _L( "MM MTP <> WriteLongTextL Metadata value for %S is \"%S\"" ), &aColumn, &aValue );
    TDbColNo num = iColSet->ColNo( aColumn );
    RDbColWriteStream strm;
    strm.OpenLC( iRecordSet, num );
    strm.WriteL( aValue );
    strm.Close();
    CleanupStack::PopAndDestroy( &strm );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::ExecuteQueryL
// Executes a query on the database and sets the cursor at the start of the recordset
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::ExecuteQueryL( const TDesC& aSelectThese,
    const TDesC& aFromTable,
    const TDesC& aColumnToMatch,
    const TDesC& aMatchCriteria,
    const TBool aIfNot,
    const TBool aNeedQuotes )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::ExecuteQueryL" ) );

    delete iColSet;
    iColSet = NULL;
    iRecordSet.Close();
    //__ASSERT_DEBUG(!iColSet, _MTP_PANIC(KMtpPrtPncCat, ENullPointer)); // Must have iColSet == NULL

    // Make sure any quotes in the aMatchCriteria are doubled...
    // coverity[incorrect_multiplication]
    // coverity[buffer_alloc]
    HBufC* matchText = HBufC::NewLC( 2 * aMatchCriteria.Length() );
    TPtr pMatchText( matchText->Des() );
    TInt srcLen = aMatchCriteria.Length();
    TPtrC ch;

    for ( TInt i = 0; i < srcLen; ++i )
        {
        ch.Set( &aMatchCriteria[i], 1 );
        pMatchText.Append( ch );

        if ( ch.CompareF( KMtpVideoSingleQuote ) == 0 )
            {
            pMatchText.Append( ch );
            }
        }

    TPtr query( iQueryText->Des() );
    query.Zero();
    query.Append( KSelect );
    query.Append( aSelectThese );
    query.Append( KFrom );
    query.Append( aFromTable );

    if ( aColumnToMatch.Length() != 0 )
        {
        query.Append( KWhere );

        if ( aIfNot )
            query.Append( KNot );

        query.Append( aColumnToMatch );
        query.Append( KEquals );

        if ( aNeedQuotes )
            query.Append( KMtpVideoSingleQuote );

        query.Append( *matchText );

        if ( aNeedQuotes )
            query.Append( KMtpVideoSingleQuote );
        }

    TDbQuery dbQuery( query );
    PRINT1( _L( "MM MTP <> ExecuteQueryL Query is \"%S\"" ), &query );

    // Execute the query
    User::LeaveIfError( iRecordSet.Prepare( iDatabase, dbQuery ) );
    User::LeaveIfError( iRecordSet.EvaluateAll() );
    iRecordSet.FirstL();
    iColSet = iRecordSet.ColSetL();
    CleanupStack::PopAndDestroy( matchText );

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::ExecuteQueryL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::OpenSessionL
// Called when the MTP session is initialised
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::OpenSessionL()
    {
    // Do nothing
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::CloseSessionL
// Called when the MTP session is closed
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::CloseSessionL()
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::CloseSessionL" ) );

    //open database if not opened
    if ( !IsDatabaseOpened() )
        return;

    // Make sure the db is commited
    SetRecordL( KNullDesC, ENoRecord );

    CleanupDbIfNecessaryL();

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::CloseSessionL" ) )
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::AddVideoL
// Adds video info to the database
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::AddVideoL( const TDesC& aFullFileName )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::AddVideoL" ) );

    //open database if not opened
    if ( !IsDatabaseOpened() )
        OpenDatabaseL();

    // Put any outstanding changes to the DB
    SetRecordL( KNullDesC, ENoRecord );
    TDbColNo num = 0;

    // Does a record already exist for this playlist?
    ExecuteQueryL( KAllColumns, KMtpVideoTable, KMtpVideoLocation, aFullFileName );

    if ( iRecordSet.CountL() == 0 )   // No record...
        {
        PRINT1( _L( "MM MTP <> AddVideoL file %S is adding to collection" ), &aFullFileName );

        // Create the record
        iRecordSet.InsertL();

        // Defaults
        num = iColSet->ColNo( KMtpVideoLocation );
        iRecordSet.SetColL( num, aFullFileName );

        num = iColSet->ColNo( KMtpVideoName );
        TParsePtrC parser( aFullFileName );
        iRecordSet.SetColL( num, parser.Name() ); // Default name is the filename.

        num = iColSet->ColNo( KMtpVideoArtist );
        iRecordSet.SetColL( num, KNullDesC );

        num = iColSet->ColNo( KMtpVideoTrack );
        iRecordSet.SetColL( num, 0 );

        num = iColSet->ColNo( KMtpVideoGenre );
        iRecordSet.SetColL( num, KNullDesC );

        num = iColSet->ColNo( KMtpVideoAlbumName );
        iRecordSet.SetColL( num, KNullDesC );

        num = iColSet->ColNo( KMtpVideoComposer );
        iRecordSet.SetColL( num, KNullDesC );

        num = iColSet->ColNo( KMtpVideoOrigReleaseDate );
        iRecordSet.SetColL( num, KNullDesC );

        num = iColSet->ColNo( KMtpVideoComment );
        iRecordSet.SetColL( num, KNullDesC );

        num = iColSet->ColNo( KMtpVideoWidth );
        iRecordSet.SetColL( num, 0 );

        num = iColSet->ColNo( KMtpVideoHeight );
        iRecordSet.SetColL( num, 0 );

        num = iColSet->ColNo( KMtpVideoDuration );
        iRecordSet.SetColL( num, 0 );

        num = iColSet->ColNo( KMtpVideoSampleRate );
        iRecordSet.SetColL( num, 0 );

        num = iColSet->ColNo( KMtpVideoNumberOfChannels );
        iRecordSet.SetColL( num, 0 );

        num = iColSet->ColNo( KMtpVideoAudioCodec );
        iRecordSet.SetColL( num, 0 );

        num = iColSet->ColNo( KMtpVideoAudioBitrate );
        iRecordSet.SetColL( num, 0 );

        num = iColSet->ColNo( KMtpVideoVideoCodec );
        iRecordSet.SetColL( num, 0 );

        num = iColSet->ColNo( KMtpVideoVideoBitrate );
        iRecordSet.SetColL( num, 0 );

        num = iColSet->ColNo( KMtpVideoFramesPer1000Sec );
        iRecordSet.SetColL( num, 0 );

        num = iColSet->ColNo( KMtpVideoKeyFrameDistance );
        iRecordSet.SetColL( num, 0 );

        num = iColSet->ColNo( KMtpVideoScanType );
        iRecordSet.SetColL( num, 0 );

        num = iColSet->ColNo( KMtpVideoEncodingProfile );
        iRecordSet.SetColL( num, KNullDesC );

        num = iColSet->ColNo( KMtpVideoParentalRating );
        iRecordSet.SetColL( num, KNullDesC );

        num = iColSet->ColNo( KMtpVideoDRM );
        iRecordSet.SetColL( num, 0 );
        }
    else
        {
        PRINT1( _L( "MM MTP <> AddVideoL file %S is found in the collection" ), &aFullFileName );

        iRecordSet.FirstL();
        iRecordSet.UpdateL();
        }

    // reset deleted bit
    num = iColSet->ColNo( KMtpVideoDeleted );
    iRecordSet.SetColL( num, 0 );

    // Commit the record
    TRAPD( err, iRecordSet.PutL() );

    if ( KErrNone != err )
        {
        PRINT1( _L( "MM MTP <> AddVideoL Failed to add record to database with code %d" ), err );
        iRecordSet.Cancel();
        User::Leave( err );
        }

    delete iColSet;
    iColSet = NULL;
    iRecordSet.Close();
    CompactDbIfNecessaryL();
    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::AddVideoL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::GetObjectMetadataValueL
//  Gets a piece of metadata from the collection
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::GetObjectMetadataValueL( const TUint16 aPropCode,
    MMTPType& aNewData,
    const CMTPObjectMetaData& aObjectMetaData )
    {
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::GetObjectMetadataValue aPropCode = 0x%x" ), aPropCode );

    //open database if not opened
    if ( !IsDatabaseOpened() )
        OpenDatabaseL();

    // File Path
    const TDesC& suid = aObjectMetaData.DesC( CMTPObjectMetaData::ESuid );
    SetRecordL( suid, ERecordRead );

    HBufC* data = NULL;
    TDbColNo num;
    TUint32 uInt32 = 0;
    TUint16 uInt16 = 0;

    switch ( aPropCode )
        {
        case EMTPObjectPropCodeName:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeName-MD" ) );
            data = ReadLongTextL( KMtpVideoName );
            }
            break;

        case EMTPObjectPropCodeArtist:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeArtist-MD" ) );
            data = ReadLongTextL( KMtpVideoArtist );
            }
            break;

        case EMTPObjectPropCodeTrack:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeTrack-MD" ) );
            num = iColSet->ColNo( KMtpVideoTrack );
            uInt16 = iRecordSet.ColUint16( num );
            if ( EMTPTypeUINT16 == aNewData.Type() )
                {
                ( ( TMTPTypeUint16 & ) aNewData ).Set( uInt16 );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeGenre:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeGenre-MD" ) );
            data = ReadLongTextL( KMtpVideoGenre );
            }
            break;

        case EMTPObjectPropCodeAlbumName:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeAlbumName-MD" ) );
            data = ReadLongTextL( KMtpVideoAlbumName );
            }
            break;

        case EMTPObjectPropCodeComposer:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeComposer-MD" ) );
            data = ReadLongTextL( KMtpVideoComposer );
            }
            break;

        case EMTPObjectPropCodeOriginalReleaseDate:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeOriginalReleaseDate-MD" ) );
            data = ReadLongTextL( KMtpVideoOrigReleaseDate );
            }
            break;

        case EMTPObjectPropCodeDescription:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeDescription-MD" ) );
            data = ReadLongTextL( KMtpVideoComment );
            CleanupStack::PushL( data ); // + data

            TInt len = data->Length();
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::GetObjectMetadataValue len = %d" ),len );
            for ( TInt i = 0; i < len; i++ )
                ( ( CMTPTypeArray& ) aNewData ).AppendUintL( ( *data )[i] );

            CleanupStack::PopAndDestroy( data ); // - data
            data = NULL;
            }
            break;

        case EMTPObjectPropCodeWidth:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeWidth-MD" ) );
            num = iColSet->ColNo( KMtpVideoWidth );
            uInt32 = iRecordSet.ColUint32( num );
            if ( EMTPTypeUINT32 == aNewData.Type() )
                {
                ( ( TMTPTypeUint32 & ) aNewData ).Set( uInt32 );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeHeight:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeHeight-MD" ) );
            num = iColSet->ColNo( KMtpVideoHeight );
            uInt32 = iRecordSet.ColUint32( num );
            if ( EMTPTypeUINT32 == aNewData.Type() )
                {
                ( ( TMTPTypeUint32 & ) aNewData ).Set( uInt32 );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeDuration:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeDuration-MD" ) );
            num = iColSet->ColNo( KMtpVideoDuration );
            uInt32 = iRecordSet.ColUint32( num );
            if ( EMTPTypeUINT32 == aNewData.Type() )
                {
                ( ( TMTPTypeUint32 & ) aNewData ).Set( uInt32 );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeSampleRate:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeSampleRate-MD" ) );
            num = iColSet->ColNo( KMtpVideoSampleRate );
            uInt32 = iRecordSet.ColUint32( num );
            if ( EMTPTypeUINT32 == aNewData.Type() )
                {
                ( ( TMTPTypeUint32 & ) aNewData ).Set( uInt32 );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeNumberOfChannels:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeNumberOfChannels-MD" ) );
            num = iColSet->ColNo( KMtpVideoNumberOfChannels );
            uInt16 = iRecordSet.ColUint16( num );
            if ( EMTPTypeUINT16 == aNewData.Type() )
                {
                ( ( TMTPTypeUint16 & ) aNewData ).Set( uInt16 );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeAudioWAVECodec:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeAudioWAVECodec-MD" ) );
            num = iColSet->ColNo( KMtpVideoAudioCodec );
            uInt32 = iRecordSet.ColUint32( num );
            if ( EMTPTypeUINT32 == aNewData.Type() )
                {
                ( ( TMTPTypeUint32 & ) aNewData ).Set( uInt32 );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeAudioBitRate:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeAudioBitRate-MD" ) );
            num = iColSet->ColNo( KMtpVideoAudioBitrate );
            uInt32 = iRecordSet.ColUint32( num );
            if ( EMTPTypeUINT32 == aNewData.Type() )
                {
                ( ( TMTPTypeUint32 & ) aNewData ).Set( uInt32 );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeVideoFourCCCodec:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeVideoFourCCCodec-MD" ) );
            num = iColSet->ColNo( KMtpVideoVideoCodec );
            uInt32 = iRecordSet.ColUint32( num );
            if ( EMTPTypeUINT32 == aNewData.Type() )
                {
                ( ( TMTPTypeUint32 & ) aNewData ).Set( uInt32 );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeVideoBitRate:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeVideoBitRate-MD" ) );
            num = iColSet->ColNo( KMtpVideoVideoBitrate );
            uInt32 = iRecordSet.ColUint32( num );
            if ( EMTPTypeUINT32 == aNewData.Type() )
                {
                ( ( TMTPTypeUint32 & ) aNewData ).Set( uInt32 );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeFramesPerThousandSeconds:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeFramesPerThousandSeconds-MD" ) );
            num = iColSet->ColNo( KMtpVideoFramesPer1000Sec );
            uInt32 = iRecordSet.ColUint32( num );
            if ( EMTPTypeUINT32 == aNewData.Type() )
                {
                ( ( TMTPTypeUint32 & ) aNewData ).Set( uInt32 );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeKeyFrameDistance:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeKeyFrameDistance-MD" ) );
            num = iColSet->ColNo( KMtpVideoKeyFrameDistance );
            uInt32 = iRecordSet.ColUint32( num );
            if ( EMTPTypeUINT32 == aNewData.Type() )
                {
                ( ( TMTPTypeUint32 & ) aNewData ).Set( uInt32 );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeScanType:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeScanType-MD" ) );
            num = iColSet->ColNo( KMtpVideoScanType );
            uInt16 = iRecordSet.ColUint16( num );
            if ( EMTPTypeUINT16 == aNewData.Type() )
                {
                ( ( TMTPTypeUint16 & ) aNewData ).Set( uInt16 );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        case EMTPObjectPropCodeEncodingProfile:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeEncodingProfile-MD" ) );
            data = ReadLongTextL( KMtpVideoEncodingProfile );
            }
            break;

        case EMTPObjectPropCodeParentalRating:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeParentalRating-MD" ) );
            data = ReadLongTextL( KMtpVideoParentalRating );
            }
            break;

        case EMTPObjectPropCodeDRMStatus:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeDRMStatus-MD" ) );
            num = iColSet->ColNo( KMtpVideoDRM );
            uInt16 = iRecordSet.ColUint16( num );
            if ( EMTPTypeUINT16 == aNewData.Type() )
                {
                ( ( TMTPTypeUint16 & ) aNewData ).Set( uInt16 );
                }
            else
                {
                User::Leave( KErrArgument );
                }
            }
            break;

        default:
            {
            User::Leave( KErrNotSupported );
            }
            break;
        }

    // Pack the info to aNewData
    if ( data )
        {
        CleanupStack::PushL( data ); // + data
#ifdef _DEBUG
        if ( data->Length() > KMtpMaxStringDescLength )   // Have to concatenate for MTP
            {
            PRINT1( _L( "MM MTP <> Descriptor will be concatenated from length %d to KMtpMaxStringLength" ), data->Length() );
            }
#endif // _DEBUG

        if ( EMTPTypeString == aNewData.Type() )
            {
            ( ( CMTPTypeString& ) aNewData ).SetL( *data );
            }
        else
            {
            User::Leave( KErrArgument );
            }
        CleanupStack::PopAndDestroy( data ); // - data
        }

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::GetObjectMetadataValue" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::SetObjectMetadataValueL
// Sets a piece of metadata in the collection
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::SetObjectMetadataValueL( const TUint16 aPropCode,
    const MMTPType& aNewData,
    const CMTPObjectMetaData& aObjectMetaData )
    {
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::SetObjectMetadataValueL aPropCode = 0x%x" ), aPropCode );

    if ( !IsDatabaseOpened() )
        OpenDatabaseL();

    // Get file path
    TPtrC suid( aObjectMetaData.DesC( CMTPObjectMetaData::ESuid ) );
    SetRecordL( suid, ERecordWrite );
    SetMetadataL( aPropCode, aNewData );

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::SetObjectMetadataValueL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::SetMetadataL
// Set meta data.
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::SetMetadataL( const TUint16 aObjPropCode,
    const MMTPType& aNewData )
    {
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::SetMetadataL property Code = 0x%x" ), aObjPropCode );

    TBuf<KMtpMaxStringLength> data;
    TDbColNo num;
    TMTPTypeUint16 uint16Data;
    TMTPTypeUint32 uint32Data;
    CMTPTypeArray* desData = NULL;

    TPtrC colName( ColumnNameFromPropCodeL( aObjPropCode ) );
    switch ( aObjPropCode )
        {
        case EMTPObjectPropCodeName:
        case EMTPObjectPropCodeArtist:
        case EMTPObjectPropCodeGenre:
        case EMTPObjectPropCodeAlbumName:
        case EMTPObjectPropCodeComposer:
        case EMTPObjectPropCodeOriginalReleaseDate:
        case EMTPObjectPropCodeEncodingProfile:
        case EMTPObjectPropCodeParentalRating:
            {
            if ( EMTPTypeString != aNewData.Type() )
                {
                User::Leave( KErrArgument );
                }
            TPtrC string( ( ( CMTPTypeString& ) aNewData ).StringChars() );
            WriteLongTextL( colName, string );
            PRINT2( _L( "MM MTP <> CMmMtpDpMetadataVideoAccess::SetMetadataL string = %S, length = %d" ), &string, string.Length() );
            }
            break;

        case EMTPObjectPropCodeDescription:
            {
            PRINT( _L( "MM MTP <> EMTPObjectPropCodeDescription-MD" ) );
#ifdef __MUSIC_ID_SUPPORT
            // WriteMusicIdsL(*longString);
#else
            desData = CMTPTypeArray::NewLC( EMTPTypeAUINT16 ); // + desData
            MMTPType::CopyL( aNewData, *desData );
            TUint length = desData->NumElements();
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::SetMetadataValueL length = %d" ),
                length );
            if ( length != 0 )
                {
                length = ( length < KMTPMaxDescriptionLen ) ? length : KMTPMaxDescriptionLen;
                HBufC* text = HBufC::NewLC( length );    // + text
                TPtr ptr = text->Des();

                for ( TUint i = 0; i < length; i++ )
                    ptr.Append( desData->ElementUint( i ) );
                PRINT1( _L( "MM MTP <> CMmMtpDpMetadataMpxAccess::SetMetadataValueL text = %S" ),
                    text );

                WriteLongTextL( KMtpVideoComment, *text );
                CleanupStack::PopAndDestroy( text );    // - text
                }
            else
                {
                WriteLongTextL( KMtpVideoComment, KEmptyText );
                }
            CleanupStack::PopAndDestroy( desData ); // - desData
#endif //__MUSIC_ID_SUPPORT
            }
            break;

        case EMTPObjectPropCodeWidth:
        case EMTPObjectPropCodeHeight:
        case EMTPObjectPropCodeDuration:
        case EMTPObjectPropCodeSampleRate:
        case EMTPObjectPropCodeAudioWAVECodec:
        case EMTPObjectPropCodeAudioBitRate:
        case EMTPObjectPropCodeVideoFourCCCodec:
        case EMTPObjectPropCodeVideoBitRate:
        case EMTPObjectPropCodeFramesPerThousandSeconds:
        case EMTPObjectPropCodeKeyFrameDistance:
            {
            if ( EMTPTypeUINT32 != aNewData.Type() )
                {
                User::Leave( KErrArgument );
                }
            num = iColSet->ColNo( colName );
            iRecordSet.SetColL( num, ( ( TMTPTypeUint32& ) aNewData ).Value() );
            }
            break;

        case EMTPObjectPropCodeTrack:
        case EMTPObjectPropCodeNumberOfChannels:
        case EMTPObjectPropCodeScanType:
        case EMTPObjectPropCodeDRMStatus:
            {
            if ( EMTPTypeUINT16 != aNewData.Type() )
                {
                User::Leave( KErrArgument );
                }
            num = iColSet->ColNo( colName );
            iRecordSet.SetColL( num, ( ( TMTPTypeUint16& ) aNewData ).Value() );
            }
            break;

        default:
            {
            User::Leave( KErrNotSupported );
            }
            break;
        }

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::SetMetadataL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::RenameRecordL
// Renames the file part of a record in the collection database.
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::RenameRecordL( const TDesC& aOldFileName,
    const TDesC& aNewFileName )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::RenameRecordL()" ) );

    //open database if not opened
    if ( !IsDatabaseOpened() )
        OpenDatabaseL();

    SetRecordL( aOldFileName, ERecordWrite ); // Open the record to write
    TDbColNo num = iColSet->ColNo( KMtpVideoLocation );
    iRecordSet.SetColL( num, aNewFileName );

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::RenameRecordL()" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::DeleteRecordL
// Deletes metadata information associated with the object
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::DeleteRecordL( const TDesC& aFullFileName )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::DeleteRecordL" ) );

    //open database if not opened
    if ( !IsDatabaseOpened() )
        OpenDatabaseL();

    SetRecordL( KNullDesC, ENoRecord ); // Commit any other changes to the DB

    TUint format = MmMtpDpUtility::FormatFromFilename( aFullFileName );

    if ( MmMtpDpUtility::HasMetadata( format ) )
        {
        ExecuteQueryL( KMtpVideoLocation,
            KMtpVideoTable,
            KMtpVideoLocation,
            aFullFileName );

        for ( iRecordSet.FirstL(); iRecordSet.AtRow(); iRecordSet.NextL() )
            {
            iRecordSet.DeleteL();
            PRINT( _L( "MM MTP <> CMmMtpDpMetadataVideoAccess::DeleteRecordL Record deleted" ) );
            }

        delete iColSet;
        iColSet = NULL;
        iRecordSet.Close();
        }
    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::DeleteRecordL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::DeleteAllRecordsL
// Empties the database - used by the FormatStore command
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::DeleteAllRecordsL()
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::DeleteAllRecordsL()" ) );

    //open database if not opened
    if ( !IsDatabaseOpened() )
        OpenDatabaseL();

    SetRecordL( KNullDesC, ENoRecord );   // Commit any other changes to the DB

    PRINT( _L( "MM MTP <> Deleting music files" ) );
    ExecuteQueryL( KAllColumns, KMtpVideoTable );

    for ( iRecordSet.FirstL(); iRecordSet.AtRow(); iRecordSet.NextL() )
        {
        iRecordSet.DeleteL();
        }

    delete iColSet;
    iColSet = NULL;
    iRecordSet.Close();

    iDatabase.Compact();
    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::DeleteAllRecordsL()" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::SetRecordL
// Set Record
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::SetRecordL( const TDesC& aFullFileName,
    TMtpDbState aState )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::SetRecordL()" ) );

    if ( ( aState == iDbState )
        && ( 0 == aFullFileName.Compare( iCurrentFileName ) ) )    // Already have it
        {
        if ( iDbState == ERecordRead )
            iRecordSet.GetL();

        PRINT1( _L( "MM MTP <> Cached recordset, filename is '%S'" ), &iCurrentFileName );
        return;
        }

    iCurrentFileName.Copy( aFullFileName );
    PRINT1( _L( "MM MTP <> CMmMtpDpMetadataVideoAccess::iCurrentFileName set to '%S'" ), &iCurrentFileName );

    // We arrive here, if a new view is required.  First, need to deal with the old one.
    switch ( iDbState )
        {
        case EFailedWrite:
            // We wrote to the DB, but the op failed
            PRINT( _L( "MM MTP <> SetRecordL EFailedWrite" ) );
            iRecordSet.Cancel();

        case ERecordWrite:              // intentionally fallthrough
            // We wrote some stuff to the db, now needs comitting
            PRINT( _L( "MM MTP <> SetRecordL ERecordWrite" ) );
            TRAPD( err, iRecordSet.PutL() );

            if ( KErrNone != err )
                {
                iRecordSet.Cancel();
                User::Leave( err );
                }
            //lint -fallthrough
        case ENoRecord:                 // intentionally fallthrough
            // We need to just close the open record
            PRINT( _L( "MM MTP <> SetRecordL ENoRecord" ) );
            //lint -fallthrough
        case ERecordRead:               // intentionally fallthrough
            // We need to open a row for reading
            PRINT( _L( "MM MTP <> SetRecordL ERecordRead" ) );
            //lint -fallthrough
        default:                        // intentionally fallthrough
            PRINT( _L( "MM MTP <> SetRecordL Closing recordset" ) );
            delete iColSet;
            iColSet = NULL;
            iRecordSet.Close();
            iDbState = ENoRecord;
            break;
        }

    if ( aState == ENoRecord ) // We are closing the session if this is the case
        {
        PRINT( _L( "MM MTP <> SetRecordL Record closed, no request to open" ) );
        return;
        }

    // if we have a music format, we can open the database...
    TUint16 format = MmMtpDpUtility::FormatFromFilename( iCurrentFileName );
    PRINT1( _L( "MM MTP <> SetRecordL format = 0x%x" ), format );

    if ( MmMtpDpUtility::HasMetadata( format ) )
        {
        // Get the record
        ExecuteQueryL( KAllColumns, KMtpVideoTable, KMtpVideoLocation, aFullFileName );

        if ( iRecordSet.CountL() == 0 ) // No match...
            {
            iRecordSet.Close();
            PRINT( _L( "MM MTP <> Recordset is empty! Bailing out..." ) );
            delete iColSet;
            iColSet = NULL;
            iRecordSet.Close();
            iDbState = ENoRecord;
            User::Leave( KErrNotFound );  // Have an object on the disk, but no metadata
            }

        // Check that we only have one record, not many...
        if ( iRecordSet.CountL() > 1 )
            {
            //LOGTEXT2("METADATA DATABASE ERROR: %d entries for the same file name!!", iRecordSet.CountL());
            User::Leave( KErrCorrupt );
            }

        if ( aState == ERecordWrite )
            {
            iRecordSet.UpdateL();    // Lock the row
            PRINT( _L( "MM MTP <> Row locked" ) );
            }
        else if ( aState == ERecordRead )
            {
            iRecordSet.GetL();
            }

        // Update the internal state
        iDbState = aState;
        }
    else
        User::Leave( KErrNotFound );

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::SetRecordL()" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::SetImageObjPropL
// set image specific properties specific to videos
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::SetImageObjPropL( const TDesC& aFullFileName,
    const TUint32 aWidth,
    const TUint32 aHeight )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::SetImageObjPropL()" ) );

    if ( !IsDatabaseOpened() )
        OpenDatabaseL();

    TDbColNo num = 0;
    TInt err = KErrNone;

    SetRecordL( aFullFileName, ERecordWrite );
    num = iColSet->ColNo( KMtpVideoWidth );
    TRAP( err, iRecordSet.SetColL( num, aWidth ) );

    if ( err != KErrNone )
        {
        SetRecordL( aFullFileName, EFailedWrite );
        User::Leave( err );
        }

    num = iColSet->ColNo( KMtpVideoHeight );
    TRAP( err, iRecordSet.SetColL( num, aHeight ) );

    if ( err != KErrNone )
        {
        SetRecordL( aFullFileName, EFailedWrite );
        User::Leave( err );
        }
    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::SetImageObjPropL()" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataVideoAccess::GetImageObjPropL
// Get image specific properties specific to videos
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataVideoAccess::GetImageObjPropL( const TDesC& aFullFileName,
    TUint32& aWidth,
    TUint32& aHeight )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::GetImageObjPropL()" ) );

    if ( !IsDatabaseOpened() )
        OpenDatabaseL();

    TDbColNo num = 0;

    SetRecordL( aFullFileName, ERecordRead );

    num = iColSet->ColNo( KMtpVideoWidth );
    aWidth = iRecordSet.ColUint32( num );

    num = iColSet->ColNo( KMtpVideoHeight );
    aHeight = iRecordSet.ColUint32( num );

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::GetImageObjPropL()" ) );
    }

const TDesC& CMmMtpDpMetadataVideoAccess::ColumnNameFromPropCodeL( const TUint16 aPropCode )
    {
    const TColumnNameTable KColumnTable[] =
        {
            { EMTPObjectPropCodeName,                    KMtpVideoName },
            { EMTPObjectPropCodeArtist,                  KMtpVideoArtist },
            { EMTPObjectPropCodeTrack,                   KMtpVideoTrack },
            { EMTPObjectPropCodeGenre,                   KMtpVideoGenre },
            { EMTPObjectPropCodeAlbumName,               KMtpVideoAlbumName },
            { EMTPObjectPropCodeComposer,                KMtpVideoComposer },
            { EMTPObjectPropCodeOriginalReleaseDate,     KMtpVideoOrigReleaseDate },
            { EMTPObjectPropCodeDescription,             KMtpVideoComment },
            { EMTPObjectPropCodeWidth,                   KMtpVideoWidth },
            { EMTPObjectPropCodeHeight,                  KMtpVideoHeight },
            { EMTPObjectPropCodeDuration,                KMtpVideoDuration },
            { EMTPObjectPropCodeSampleRate,              KMtpVideoSampleRate },
            { EMTPObjectPropCodeNumberOfChannels,        KMtpVideoNumberOfChannels },
            { EMTPObjectPropCodeAudioBitRate,            KMtpVideoAudioBitrate },
            { EMTPObjectPropCodeVideoFourCCCodec,        KMtpVideoVideoCodec },
            { EMTPObjectPropCodeVideoBitRate,            KMtpVideoVideoBitrate },
            { EMTPObjectPropCodeFramesPerThousandSeconds,KMtpVideoFramesPer1000Sec },
            { EMTPObjectPropCodeKeyFrameDistance,        KMtpVideoKeyFrameDistance },
            { EMTPObjectPropCodeScanType,                KMtpVideoScanType },
            { EMTPObjectPropCodeEncodingProfile,         KMtpVideoEncodingProfile },
            { EMTPObjectPropCodeParentalRating,          KMtpVideoParentalRating },
            { EMTPObjectPropCodeAudioWAVECodec,          KMtpVideoAudioCodec },
            { EMTPObjectPropCodeDRMStatus,               KMtpVideoDRM },
        };

    TInt i = 0;
    TInt count = sizeof( KColumnTable ) / sizeof( KColumnTable[0] );
    while ( ( KColumnTable[i].iPropCode != aPropCode ) && ( i < count ) )
        {
        i++;
        }

    if ( i == count )
        {
        // Not supported by MPX, shouldn't call this function
        PRINT( _L( "MM MTP <> CMmMtpDpMetadataVideoAccess::ColumnNameFromPropCodeL NOT SUPPORTED ATTRIBUTE" ) );
        User::Leave( KErrNotSupported );
        }
    return KColumnTable[i].iDbColumnName;
    }

void CMmMtpDpMetadataVideoAccess::SetStorageRootL( const TDesC& aStorageRoot )
    {
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::SetStorageRoot aStoreRoot = %S" ), &aStorageRoot );

    // get the drive number
    TParsePtrC pathParser( aStorageRoot );
    TChar driveChar( pathParser.Drive()[0] );

    User::LeaveIfError( RFs::CharToDrive( driveChar, iStoreNum ) );
    PRINT1( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::SetStorageRoot drive number = %d" ), iStoreNum );
    }

void CMmMtpDpMetadataVideoAccess::CleanupDatabaseL()
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataVideoAccess::CleanupDatabaseL" ) );

    //open database if not opened
    if ( !IsDatabaseOpened() )
        {
        return;
        }

    // Make sure the db is commited
    SetRecordL( KNullDesC, ENoRecord );

    IdentifyDeletedFilesL();

    CleanupDbIfNecessaryL();

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataVideoAccess::CleanupDatabaseL" ) )
    }

TBool CMmMtpDpMetadataVideoAccess::FileExists( const TDesC& aFullFileName )
    {
    return BaflUtils::FileExists( iRfs, aFullFileName );
    }

// end of file
