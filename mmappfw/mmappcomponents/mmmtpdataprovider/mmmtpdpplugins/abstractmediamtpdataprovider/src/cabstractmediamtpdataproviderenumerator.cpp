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
* Description:  Enumerator objects
*
*/


#include <bautils.h>
#include <mtp/cmtpobjectmetadata.h>
#include <mtp/mmtpdataproviderframework.h>
#include <mtp/mmtpobjectmgr.h>
#include <mtp/mmtpstoragemgr.h>
#include <mtp/mmtpreferencemgr.h>
#include <mtp/tmtptypeuint32.h>
#include <mpxmediaarray.h>
#include <mpxmedia.h>

#include "cabstractmediamtpdataproviderenumerator.h"
#include "cabstractmediamtpdataprovider.h"
#include "mmmtpdplogger.h"
#include "mmmtpdputility.h"
#include "cmmmtpdpmetadataaccesswrapper.h"


const TInt KMTPDriveGranularity = 5;
const TInt KAbstractMediaDpArrayGranularity = 2;

_LIT( KPlaylistFilePath, "Playlists\\" );

#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
_LIT( KMpxGetAllAbstractMedia, "MpxGetAllAbstractMedia" );
_LIT( KObjectManagerObjectUid, "ObjectManagerObjectUid" );
_LIT( KObjectManagerInsert, "ObjectManagerInsert" );
_LIT( KResetObjectFormatSubCode, "ResetObjectFormatSubCode" );
#endif

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderEnumerator::NewL
// Two phase constructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderEnumerator* CAbstractMediaMtpDataProviderEnumerator::NewL( MMTPDataProviderFramework& aFramework,
    CAbstractMediaMtpDataProvider& aDataProvider )
    {
    PRINT( _L( "MM MTP => CAbstractMediaMtpDataProviderEnumerator::NewL" ) );
    CAbstractMediaMtpDataProviderEnumerator* self = new ( ELeave ) CAbstractMediaMtpDataProviderEnumerator( aFramework,
        aDataProvider );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    PRINT( _L( "MM MTP <= CAbstractMediaMtpDataProviderEnumerator::NewL" ) );
    return self;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderEnumerator::CAbstractMediaMtpDataProviderEnumerator
// Standard C++ Constructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderEnumerator::CAbstractMediaMtpDataProviderEnumerator( MMTPDataProviderFramework& aFramework,
    CAbstractMediaMtpDataProvider& aDataProvider ) :
    CActive( EPriorityLow ),
    iFramework( aFramework ),
    iObjectMgr( aFramework.ObjectMgr() ),
    iDataProviderId( aFramework.DataProviderId() ),
    iDataProvider( aDataProvider ),
    iStorages( KAbstractMediaDpArrayGranularity ),
    iAbstractMedias( NULL ),
    iCount( 0 ),
    iCurrentIndex( 0 ),
    iEnumState ( EEnumNone ),
    iResetCount( 0 )
    {
    PRINT1( _L( "MM MTP <> CAbstractMediaMtpDataProviderEnumerator::CAbstractMediaMtpDataProviderEnumerator, iDataProviderId = %d" ), iDataProviderId );
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderEnumerator::ConstructL
//
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderEnumerator::ConstructL()
    {
    CActiveScheduler::Add( this );

#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
    iPerfLog = CMmMtpDpPerfLog::NewL( _L( "CAbstractMediaMtpDataProviderEnumerator" ) );
#endif
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderEnumerator::~CAbstractMediaMtpDataProviderEnumerator
// destructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderEnumerator::~CAbstractMediaMtpDataProviderEnumerator()
    {
    PRINT( _L( "MM MTP => CAbstractMediaMtpDataProviderEnumerator::~CAbstractMediaMtpDataProviderEnumerator" ) );

    Cancel();
    iStorages.Close();

    delete iAbstractMedias;
    iAbstractMedias = NULL;

#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
    delete iPerfLog;
#endif // _DEBUG
    PRINT( _L( "MM MTP <= CAbstractMediaMtpDataProviderEnumerator::~CAbstractMediaMtpDataProviderEnumerator" ) );
    }

// -----------------------------------------------------------------------------
// AbstractMediaDpMtpEnumerator::StartL
// Kick off the enumeration on the specified storage
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderEnumerator::StartL( TUint32 aStorageId )
    {
    PRINT1( _L( "MM MTP => CAbstractMediaMtpDataProviderEnumerator::StartL aStorageId = 0x%x" ), aStorageId );

    MMTPStorageMgr& storageMgr( iFramework.StorageMgr() );
    if ( aStorageId == KMTPStorageAll )
        {
        // Retrieve the available logical StorageIDs
        RPointerArray<const CMTPStorageMetaData> storages;
        CleanupClosePushL( storages );      // + storages
        TMTPStorageMgrQueryParams params( KNullDesC,
            CMTPStorageMetaData::ESystemTypeDefaultFileSystem );

        storageMgr.GetLogicalStoragesL( params, storages );

        // Construct the StorageIDs list.
        for ( TInt i = 0; i < storages.Count(); i++ )
            {
            iStorages.AppendL( storages[i]->Uint(
                CMTPStorageMetaData::EStorageId ) );
            }
        CleanupStack::PopAndDestroy( &storages );   // - storages
        }
    else if ( aStorageId != KMTPNotSpecified32
            && storageMgr.ValidStorageId( aStorageId ) )
        {
        const CMTPStorageMetaData& storage(
             storageMgr.StorageL( aStorageId ) );
        if ( storage.Uint( CMTPStorageMetaData::EStorageSystemType ) ==
                CMTPStorageMetaData::ESystemTypeDefaultFileSystem )
            {
            if ( storageMgr.LogicalStorageId( aStorageId ) != KMTPNotSpecified32 )
                {
                // Logical StorageID.
                iStorages.AppendL( aStorageId );
                }
            else
                {
                // Physical StorageID. Enumerate all eligible logical storages.
                const RArray<TUint>& logicalIds( storage.UintArray(
                    CMTPStorageMetaData::EStorageLogicalIds ) );

                TInt countLogicalIds = logicalIds.Count();

                for ( TInt i = 0; i < countLogicalIds; i++ )
                    {
                    iStorages.AppendL( logicalIds[i] );
                    }
                }
            }
        }

    // keep in mind for notification when enumeration complete
    iStorageId = aStorageId;
    iEnumState = EEnumNone;
    ScanNextL();

    PRINT( _L( "MM MTP <= CAbstractMediaMtpDataProviderEnumerator::StartL" ) );
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderEnumerator::InitStorageL
// Initialize iAbstractMedia, iCount and iCurrentIndex according to iStorages and iEnumState
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderEnumerator::InitStorageL()
    {
    __ASSERT_DEBUG(iStorages.Count() > 0 && iEnumState < EEnumCount, User::Invariant() );
    PRINT2( _L( "MM MTP => CAbstractMediaMtpDataProviderEnumerator::InitStorageL storageId = 0x%x, state=%d" ), iStorages[0], iEnumState );
    const CMTPStorageMetaData& storage(
        iFramework.StorageMgr().StorageL( iStorages[0] ) );

    __ASSERT_DEBUG( ( storage.Uint( CMTPStorageMetaData::EStorageSystemType ) ==
        CMTPStorageMetaData::ESystemTypeDefaultFileSystem ), User::Invariant() );

    TPtrC root( storage.DesC( CMTPStorageMetaData::EStorageSuid ) );
    PRINT1( _L( "MM MTP <> CAbstractMediaMtpDataProviderEnumerator::InitStorageL StorageSuid = %S" ), &root );

    if ( iEnumState == EEnumPlaylist )
        {
        // created by windows media player, or else return responsecode is Access denied
        // Create playlist directory if it does not exist
        // NOTE: Only playlist need to create directory here, for the dummy files
        HBufC* tempBuf = HBufC::NewLC( KMaxFileName );  // + tempBuf
        TPtr folder = tempBuf->Des();
        folder.Zero();
        folder.Append( root );
        folder.Append( KPlaylistFilePath );
        TBool ret = BaflUtils::FileExists( iFramework.Fs(), folder );
        PRINT2( _L( "MM MTP <> CAbstractMediaMtpDataProviderEnumerator::InitStorageL ret = %d, folder = %S" ), ret, &folder );
        if( !ret )
            {
            TInt err = iFramework.Fs().MkDirAll( folder );
            PRINT2( _L( "MM MTP <> CAbstractMediaMtpDataProviderEnumerator::InitStorageL Creating folder (%S) returned error %d" ), tempBuf, err );

            // add this new folder to framework metadata DB
            CMTPObjectMetaData* object = CMTPObjectMetaData::NewLC( 0, // Dev Dp hard code
                    EMTPFormatCodeAssociation,
                    iStorages[0],
                    folder );  // + object
            object->SetUint( CMTPObjectMetaData::EParentHandle, KMTPHandleNoParent );
            object->SetUint( CMTPObjectMetaData::EFormatSubCode, EMTPAssociationTypeGenericFolder );
            object->SetDesCL( CMTPObjectMetaData::EName, KPlaylistFilePath().Left( KPlaylistFilePath().Length() - 1 ) );
            PERFLOGSTART( KObjectManagerInsert );
            iObjectMgr.InsertObjectL( *object );
            PERFLOGSTOP( KObjectManagerInsert );
            CleanupStack::PopAndDestroy( object ); // - object
            }
        CleanupStack::PopAndDestroy( tempBuf ); // - tempBuf
        }

    // find all abstract medias stored in MPX
    delete iAbstractMedias;
    iAbstractMedias = NULL;
    iAbstractMedias = new( ELeave ) CDesCArrayFlat( KMTPDriveGranularity );
    TMPXGeneralCategory category = ( iEnumState == EEnumPlaylist ) ? EMPXPlaylist : EMPXAbstractAlbum;
    PERFLOGSTART( KMpxGetAllAbstractMedia );
    TRAPD( err, iDataProvider.GetWrapperL().GetAllAbstractMediaL( root, *iAbstractMedias, category  ) );
    PERFLOGSTOP( KMpxGetAllAbstractMedia );

    if ( iAbstractMedias != NULL && err == KErrNone )
        {
        iCount = iAbstractMedias->Count();
        }
    else
        {
        iCount = 0;
        }
    iCurrentIndex = 0;

    PRINT( _L( "MM MTP <= CAbstractMediaMtpDataProviderEnumerator::InitStorageL" ) );
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderEnumerator::ScanNextL
// Proceed to next abstract media item
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderEnumerator::ScanNextL()
    {
    PRINT( _L( "MM MTP => CAbstractMediaMtpDataProviderEnumerator::ScanNextL" ) );

    if ( iCurrentIndex >= iCount )
        {
        iEnumState++;
        }

    if ( iEnumState >= EEnumCount )
        {
        iEnumState = EEnumPlaylist;
        iResetCount = 0;
        
        PERFLOGSTART( KResetObjectFormatSubCode );
        ResetObjectFormatSubCodeL( iStorages[0] );
        PERFLOGSTOP( KResetObjectFormatSubCode );
        
        PRINT2( _L( "MM MTP <> ResetObjectFormatSubCodeL, storage = 0x%x, iResetCount = %d" ), iStorages[0], iResetCount );
        iStorages.Remove( 0 );
        }

    if ( iStorages.Count() > 0 )
        {
        if( iCurrentIndex >= iCount )
            {
            InitStorageL();
            }

        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, iStatus.Int() );
        SetActive();
        }
    else
        {
        // We are done
        PRINT( _L( "MM MTP <> Objects enumeration completed" ) );
        delete iAbstractMedias;
        iAbstractMedias = NULL;
        iStorages.Reset();
        SignalCompleteL( iDataProvider );
        }

    PRINT( _L( "MM MTP <= CAbstractMediaMtpDataProviderEnumerator::ScanNextL" ) );   
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderEnumerator::RunL
// NOTE: preserve for performance improvement
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderEnumerator::RunL()
    {
    PRINT( _L( "MM MTP => CAbstractMediaMtpDataProviderEnumerator::RunL" ) );
    if ( iCurrentIndex < iCount )
        {
        PRINT2( _L( "MM MTP <> Current storage is still being scanned, current index = %d, total AbstractMedia count = %d" ),
            iCurrentIndex,
            iCount );

        // Insert all abstract medias into handle db of framework
        iCurrentIndex++;    // Increase the index first in case of leave
        TPtrC entrySuid( ( *iAbstractMedias )[iCurrentIndex-1] );

        if ( iEnumState == EEnumAbstractAlbum )
            {
            // The abstract album may be removed, check it first
            TBool ret = BaflUtils::FileExists( iFramework.Fs(), entrySuid );
            PRINT2( _L( "MM MTP <> BaflUtils::FileExists( RFs,%S ) ret = %d" ), &entrySuid, ret );
            }
        else // EEnumPlaylist
            {
            // Add an object to the object store for DB based playlist
            AddEntryL( entrySuid );
            }

        // Getting all reference during internal enumeration takes too long, query later on demand
        }

    ScanNextL();

    PRINT( _L( "MM MTP <= CAbstractMediaMtpDataProviderEnumerator::RunL" ) );
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderEnumerator::RunError
// NOTE: preserve for performance improvement
// -----------------------------------------------------------------------------
//
TInt CAbstractMediaMtpDataProviderEnumerator::RunError( TInt aError )
    {
    if ( aError != KErrNone )
        PRINT1( _L( "MM MTP <> CAbstractMediaMtpDataProviderEnumerator::RunError with error %d" ), aError );

    TRAP_IGNORE( SignalCompleteL( iDataProvider ) );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderEnumerator::DoCancel()
// Cancel the enumeration process
// NOTE: preserve for performance improvement
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderEnumerator::DoCancel()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderEnumerator::SignalCompleteL
// Called when the enumeration is completed
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderEnumerator::SignalCompleteL( MMTPEnumerationCallback& aCallback,
    TInt aError )
    {
    PRINT( _L( "MM MTP <> CAbstractMediaMtpDataProviderEnumerator::SignalCompleteL" ) );
    // Enumeration completed on this drive
    aCallback.NotifyEnumerationCompleteL( iStorageId, aError );
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderEnumerator::AddEntryL
// Add a file entry to the object store
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderEnumerator::AddEntryL( const TDesC& aSuid )
    {
    PRINT1( _L("MM MTP => CAbstractMediaMtpDataProviderEnumerator::AddEntryL AbstractMedia name = %S"), &aSuid );

    TMTPFormatCode format = MmMtpDpUtility::FormatFromFilename( aSuid );
    CMTPObjectMetaData* object = CMTPObjectMetaData::NewLC( iDataProviderId,
        format,
        iStorages[0],
        aSuid );  // + object

    TParsePtrC parser( aSuid );
    PERFLOGSTART( KObjectManagerObjectUid );
    TUint32 parentHandle = iFramework.ObjectMgr().HandleL( parser.DriveAndPath() );
    PERFLOGSTOP( KObjectManagerObjectUid );

    // MTP FW returns KMTPHandleNone for parentHandle in two situations:
    // 1. The path doesn't exist
    // 2. The object is under root directory
    if ( parentHandle == KMTPHandleNone )
        {
        if ( BaflUtils::PathExists( iFramework.Fs(), parser.DriveAndPath() ) )
            {
            parentHandle = KMTPHandleNoParent;
            }
        else
            {
            iDataProvider.GetWrapperL().DeleteObjectL( *object );
            }
        }

    if ( parentHandle != KMTPHandleNone )
        {
        // Handle .m3u playlist like abstract playlist in two situations:
        // 1. The .m3u file is not on the file system.
        // 2. The .m3u file is hidden and 0 size
        TBool add = ETrue;
        if ( format == EMTPFormatCodeM3UPlaylist )
            {
            TEntry entry;
            TInt err = iFramework.Fs().Entry( aSuid, entry );
            add = ( err != KErrNone) || ( entry.iSize == 0 && entry.IsHidden() );
            }

        if ( add )
            {
            object->SetUint( CMTPObjectMetaData::EParentHandle, parentHandle );

            PERFLOGSTART( KObjectManagerInsert );
            iObjectMgr.InsertObjectL( *object );
            PERFLOGSTOP( KObjectManagerInsert );

            //Create the dummy file on file system
            iDataProvider.GetWrapperL().CreateDummyFile( aSuid );
            // remember the abstract media file for clean up
            iDataProvider.GetWrapperL().AddDummyFileL( aSuid );
            }
        }

    CleanupStack::PopAndDestroy( object );// - object

    PRINT( _L( "MM MTP <= CAbstractMediaMtpDataProviderEnumerator::AddEntryL" ) );
    }

void CAbstractMediaMtpDataProviderEnumerator::ResetObjectFormatSubCodeL( TUint32 aStorageId )
    {
    PRINT1( _L( "MM MTP => CAbstractMediaMtpDataProviderEnumerator::ResetObjectFormatSubCodeL, StorageId = 0x%x" ),
        aStorageId );
    RMTPObjectMgrQueryContext context;
    RArray<TUint> handles;
    CleanupClosePushL( context ); // + context
    CleanupClosePushL( handles ); // + handles
    
    // search all handles in desired storage with desired DP id
    TMTPObjectMgrQueryParams params( aStorageId, KMTPFormatsAll, KMTPHandleNone, iFramework.DataProviderId() );

    do
        {
        iObjectMgr.GetObjectHandlesL( params, context, handles );

        for ( TInt i = 0; i < handles.Count(); i++ )
            {
            CMTPObjectMetaData* objectInfo = CMTPObjectMetaData::NewLC(); // + objectInfo
            
            iResetCount++;
            iObjectMgr.ObjectL( handles[i], *objectInfo );
            // EMTPSubFormatCodeUnknown means references of the object haven't been insert into reference db of fw.
            objectInfo->SetUint( CMTPObjectMetaData::EFormatSubCode, EMTPSubFormatCodeUnknown );
            iObjectMgr.ModifyObjectL( *objectInfo );
            CleanupStack::PopAndDestroy( objectInfo ); // - objectInfo
            }
        }
    while ( !context.QueryComplete() );

    CleanupStack::PopAndDestroy( &handles ); // - handles
    CleanupStack::PopAndDestroy( &context ); // - context

    PRINT( _L( "MM MTP <= CAbstractMediaMtpDataProviderEnumerator::ResetObjectFormatSubCodeL" ) );
    }

//end of file
