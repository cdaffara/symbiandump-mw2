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


#include <mtp/mmtpdataproviderframework.h>
#include <mtp/mmtpobjectmgr.h>
#include <mtp/mmtpstoragemgr.h>
#include <mtp/cmtpobjectmetadata.h>

#include "cmediamtpdataproviderenumerator.h"
#include "cmediamtpdataprovider.h"
#include "mmmtpdplogger.h"
#include "cmmmtpdpmetadataaccesswrapper.h"

const TInt KMTPDriveGranularity = 5;
const TInt KMediaDpArrayGranularity = 2;

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderEnumerator::NewL
// Two phase constructor
// -----------------------------------------------------------------------------
//
CMediaMtpDataProviderEnumerator* CMediaMtpDataProviderEnumerator::NewL( MMTPDataProviderFramework& aFramework,
    CMediaMtpDataProvider& aDataProvider )
    {
    PRINT( _L( "MM MTP => CMediaMtpDataProviderEnumerator::NewL" ) );

    CMediaMtpDataProviderEnumerator* self = new ( ELeave ) CMediaMtpDataProviderEnumerator( aFramework, aDataProvider );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    PRINT( _L( "MM MTP <= CMediaMtpDataProviderEnumerator::NewL" ) );
    return self;
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderEnumerator::CMediaMtpDataProviderEnumerator
// Standard C++ Constructor
// -----------------------------------------------------------------------------
//
CMediaMtpDataProviderEnumerator::CMediaMtpDataProviderEnumerator( MMTPDataProviderFramework& aFramework,
    CMediaMtpDataProvider& aDataProvider ) :
        CActive( EPriorityLow ), // EPriorityStandard ? sure?
        iFramework( aFramework ),
        iDataProvider( aDataProvider ),
        iStorages( KMediaDpArrayGranularity )
    {
    PRINT1( _L( "MM MTP <> CMediaMtpDataProviderEnumerator::CMediaMtpDataProviderEnumerator, DataProviderId = %d" ), iFramework.DataProviderId() );
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderEnumerator::~CMediaMtpDataProviderEnumerator
// destructor
// -----------------------------------------------------------------------------
//
CMediaMtpDataProviderEnumerator::~CMediaMtpDataProviderEnumerator()
    {
    PRINT( _L( "MM MTP => CMediaMtpDataProviderEnumerator::~CMediaMtpDataProviderEnumerator" ) );
    Cancel();
    iStorages.Close();

#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
    delete iPerfLog;
#endif
    PRINT( _L( "MM MTP <= CMediaMtpDataProviderEnumerator::~CMediaMtpDataProviderEnumerator" ) );
    }

// -----------------------------------------------------------------------------
// MediaMtpDataProviderEnumerator::StartL
// Kick off the enumeration on the specified storage
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProviderEnumerator::StartL( TUint32 aStorageId )
    {
    PRINT1( _L( "MM MTP => CMediaMtpDataProviderEnumerator::StartL aStorageId = 0x%x" ), aStorageId );

    MMTPStorageMgr& storageMgr( iFramework.StorageMgr() );
    if ( aStorageId == KMTPStorageAll )
        {
        // Retrieve the available logical StorageIDs
        RPointerArray<const CMTPStorageMetaData> storages;
        CleanupClosePushL( storages ); // + storages
        TMTPStorageMgrQueryParams params( KNullDesC,
            CMTPStorageMetaData::ESystemTypeDefaultFileSystem );

        storageMgr.GetLogicalStoragesL( params, storages );

        // Construct the StorageIDs list.
        for ( TInt i = 0; i < storages.Count(); i++ )
            {
            iStorages.AppendL( storages[i]->Uint( CMTPStorageMetaData::EStorageId ) );
            }
        CleanupStack::PopAndDestroy( &storages ); // - storages
        }
    else if ( aStorageId != KMTPNotSpecified32 
            && storageMgr.ValidStorageId( aStorageId ) )
        {
        const CMTPStorageMetaData& storage( storageMgr.StorageL( aStorageId ) );

        if ( storage.Uint( CMTPStorageMetaData::EStorageSystemType )
            == CMTPStorageMetaData::ESystemTypeDefaultFileSystem )
            {
            if ( storageMgr.LogicalStorageId( aStorageId ) )
                {
                // Logical StorageID.
                iStorages.AppendL( aStorageId );
                }
            else
                {
                // Physical StorageID. Enumerate all eligible logical storages.
                const RArray<TUint>& logicalIds( storage.UintArray( CMTPStorageMetaData::EStorageLogicalIds ) );

                for ( TInt i = 0; i < logicalIds.Count(); i++ )
                    {
                    iStorages.AppendL( logicalIds[i] );
                    }
                }
            }
        }

    iStorageId = aStorageId;
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, iStatus.Int() );
    SetActive();

    PRINT( _L( "MM MTP <= CMediaMtpDataProviderEnumerator::StartL" ) );
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderEnumerator::DoCancel()
// Cancel the enumeration process
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProviderEnumerator::DoCancel()
    {
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderEnumerator::RunL
// Update the mpx db flag for round trip
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProviderEnumerator::RunL()
    {
    PRINT1( _L( "MM MTP => CMediaMtpDataProviderEnumerator::RunL iStorages.Count() = %d" ), iStorages.Count() );

    if ( iStorages.Count() > 0 )
        {
        const CMTPStorageMetaData& storage( iFramework.StorageMgr().StorageL( iStorages[0] ) );
        TPtrC root( storage.DesC( CMTPStorageMetaData::EStorageSuid ) );
        GetModifiedContentL( root );
        TRAPD( err, iDataProvider.GetWrapperL().UpdateMusicCollectionL( root ) );
        if ( err != KErrNone )
            {
            PRINT1( _L("MM MTP <> CMediaMtpDataProviderEnumerator::RunL, UpdateMusicCollectionL err =%d "), err );            
            }
        iStorages.Remove( 0 );
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, iStatus.Int() );
        SetActive();
        }
    else
        {
        iStorages.Reset();
        SignalCompleteL( iDataProvider );
        }
    PRINT( _L( "MM MTP <= CMediaMtpDataProviderEnumerator::RunL" ) );
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderEnumerator::RunError
//
// -----------------------------------------------------------------------------
//
TInt CMediaMtpDataProviderEnumerator::RunError( TInt aError )
    {
    if ( aError != KErrNone )
        PRINT1( _L( "MM MTP <> CMediaMtpDataProviderEnumerator::RunError with error %d" ), aError );

    TRAP_IGNORE( SignalCompleteL( iDataProvider ) );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderEnumerator::ConstructL
//
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProviderEnumerator::ConstructL()
    {
    CActiveScheduler::Add( this );

#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
    iPerfLog = CMmMtpDpPerfLog::NewL( _L( "MediaMtpDataProviderEnumerator" ) );
#endif
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderEnumerator::SignalCompleteL
// Called when the enumeration is completed
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProviderEnumerator::SignalCompleteL( MMTPEnumerationCallback& aCallback,
    TInt aError/* = KErrNone*/)
    {
    // Enumeration completed on this drive
    aCallback.NotifyEnumerationCompleteL( iStorageId, aError );
    }

// -----------------------------------------------------------------------------
// CMediaDpMtpEnumerator::GetModifiedContentL()
// Get Modified Content and report to Framework
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProviderEnumerator::GetModifiedContentL( const TDesC& aStorageRoot )
    {
    PRINT( _L( "MM MTP => CMediaMtpDataProviderEnumerator::GetModifiedContentL" ) );
    TInt arrayCount = 0;
    CDesCArray* modifiedContents = new ( ELeave ) CDesCArrayFlat( KMTPDriveGranularity );
    CleanupStack::PushL( modifiedContents ); // + modifiedContents

    TRAPD( err, iDataProvider.GetWrapperL().GetModifiedContentL( aStorageRoot, arrayCount, *modifiedContents ) );
    if ( err != KErrNone )
        PRINT1( _L("MM MTP <> GetModifiedContentL err =%d "), err );   

    if ( arrayCount > 0 )
        {
        CMTPObjectMetaData* object = CMTPObjectMetaData::NewLC();   // + object
        for ( TInt i = 0; i < arrayCount; i++ )
            {
            if ( iFramework.ObjectMgr().ObjectL( ( *modifiedContents )[i], *object ) )
                {
                object->SetUint( CMTPObjectMetaData::EObjectMetaDataUpdate , 1 );
                iFramework.ObjectMgr().ModifyObjectL( *object );
                }
            }

        CleanupStack::PopAndDestroy( object );  // - object
        }

    CleanupStack::PopAndDestroy( modifiedContents ); // - modifiedContents

    PRINT( _L( "MM MTP <= CMediaMtpDataProviderEnumerator::GetModifiedContentL" ) );
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderEnumerator::SessionClosedL
//
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProviderEnumerator::SessionClosedL()
    {
    iDataProvider.GetWrapperL().CleanupDatabaseL();
    }

//end of file
