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
* Description:  Storage manager for test module dummy mtp implementation
*
*/


#include <bautils.h>
#include <mtp/cmtptypestring.h>
#include <mtp/mtpdatatypeconstants.h>
#include <mtp/mtpprotocolconstants.h>

#include "cteststoragemgr.h"
#include "logging.h"

#ifdef LOGGER
#undef LOGGER
#endif
#define LOGGER CTestStorageMgr::iLog

// StorageID bit manipulation patterns.
static const TUint32    KLogicalIdMask(0x0000FFFF);
static const TUint32    KPhysicalIdMask(0xFFFF0000);

static const TUint      KLogicalNumberMask(0x000000FF);
static const TUint      KLogicalOwnerShift(8);
static const TUint      KPhysicalNumberShift(16);
static const TUint      KPhysicalOwnerShift(24);
static const TUint8     KMaxOwnedStorages(0xFF);

CStifLogger* CTestStorageMgr::iLog = NULL;

/**
MTP data provider framework storage manager factory method.
@return A pointer to an MTP data provider framework storage manager. Ownership 
IS transfered.
@leave One of the system wide error codes, if a processing failure occurs.
*/
CTestStorageMgr* CTestStorageMgr::NewL()
    {
    TFileName logFileName;
    logFileName.Copy( KMtpDataProviderTestModuleLogFile );
    
    if ( CTestStorageMgr::iLog == NULL )
        {
        CTestStorageMgr::iLog = CStifLogger::NewL(
                KMtpDataProviderTestModuleLogPath, 
                logFileName,
                CStifLogger::ETxt,
                TRACE_TARGET,
                ETrue, ETrue, ETrue, EFalse, ETrue );
        }
    PRINTF0( ">CTestStorageMgr::NewL" );
    CTestStorageMgr* self = new( ELeave ) CTestStorageMgr();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    PRINTF0( "<CTestStorageMgr::NewL" );
    return self;
    }

/**
Destructor.
*/
CTestStorageMgr::~CTestStorageMgr()
    {
    PRINTF0( ">CTestStorageMgr::~CTestStorageMgr()" );
    iFs.Close();
    iPhysicalStorageNumbers.Reset();
    iStorages.ResetAndDestroy();
    PRINTF0( "<CTestStorageMgr::~CTestStorageMgr()" );
    if ( CTestStorageMgr::iLog != NULL )
        {
        delete CTestStorageMgr::iLog;
        }
    }

/**
Extracts the storage number of the logical storage ID encoded in the specified
StorageID.
@param aStorageId The storage ID.
@return The storage number.
*/
TUint CTestStorageMgr::LogicalStorageNumber( TUint32 aStorageId ) 
    {
    PRINTF1( ">CTestStorageMgr::LogicalStorageNumber aStorageId = 0x%x", aStorageId );
    TUint number = aStorageId & KLogicalNumberMask;
    PRINTF1( "<CTestStorageMgr::LogicalStorageNumber number = 0x%x", number );
    return number;
    }

/**
Extracts the ID of the data provider responsible for the logical storage ID 
encoded in the specified StorageID.
@param aStorageId The storage ID.
@return The data provider owner ID.
*/    
TUint CTestStorageMgr::LogicalStorageOwner( TUint32 aStorageId ) 
    {
    PRINTF1( ">CTestStorageMgr::LogicalStorageOwner aStorageId = 0x%x", aStorageId );
    TUint owner = ( aStorageId & KLogicalIdMask ) >> KLogicalOwnerShift;
    PRINTF1( "<CTestStorageMgr::LogicalStorageOwner owner = 0x%x", owner );
    return owner;
    }

/**
Extracts the storage number of the physical storage ID encoded in the specified
StorageID.
@param aStorageId The storage ID.
@return The storage number.
*/
TUint CTestStorageMgr::PhysicalStorageNumber( TUint32 aStorageId ) 
    {
    PRINTF1( ">CTestStorageMgr::PhysicalStorageNumber aStorageId = 0x%x", aStorageId );
    TUint number = ( aStorageId & KPhysicalIdMask ) >> KPhysicalNumberShift;
    PRINTF1( "<CTestStorageMgr::PhysicalStorageNumber number = 0x%x", number );
    return number;
    }
    
/**
Extracts the ID of the data provider responsible for the physical storage ID 
encoded in the specified StorageID.
@param aStorageId The storage ID.
@return The data provider owner ID.
*/
TUint CTestStorageMgr::PhysicalStorageOwner( TUint32 aStorageId ) 
    {
    PRINTF1( ">CTestStorageMgr::PhysicalStorageOwner aStorageId = 0x%x", aStorageId );
    TUint owner = ( aStorageId & KPhysicalIdMask ) >> KPhysicalOwnerShift;
    PRINTF1( "<CTestStorageMgr::PhysicalStorageOwner owner = 0x%x", owner );
    return owner;
    }

/**
Sets the default MTP StorageID. This should be set once at start up and not 
subsequently changed.
@param aStorageId The system default MTP StorageID.
@panic USER 0, in debug builds only, if the default StorageID is set more than
once.
*/    
void CTestStorageMgr::SetDefaultStorageId( TUint32 aStorageId )
    {
    PRINTF1( "<>CTestStorageMgr::SetDefaultStorageId aStorageId = 0x%x", aStorageId );
    iDefaultStorageId = aStorageId;
    }

/**
Creates a mapping between the specified Symbian OS drive number and MTP 
StorageID.
@param aDriveNumber The Symbian OS drive number.
@param aStorageId The MTP StorageID.
@leave One of the sysem wide error codes, if a processing failure occurs.
*/
void CTestStorageMgr::SetDriveMappingL( TDriveNumber aDriveNumber, TUint32 aStorageId )
    {
    PRINTF2( ">CTestStorageMgr::SetDriveMappingL aDriveNumber = %d aStorageId = 0x%x", aDriveNumber, aStorageId );
    iMapDriveToStorage[aDriveNumber] = aStorageId;
    PRINTF0( "<CTestStorageMgr::SetDriveMappingL" );
    }

/**
Sets the framework storages owner identifier. This should be set once at start 
up and not subsequently changed.
@param aDataProviderId The framework storages owner identifier.
@panic USER 0, in debug builds only, if the framework storages owner identifier
is set more than once.
*/    
void CTestStorageMgr::SetFrameworkId( TUint aDataProviderId )
    {
    PRINTF1( ">CTestStorageMgr::SetFrameworkId aDataProviderId = %d", aDataProviderId );
    __ASSERT_DEBUG( ( iFrameworkId == KErrNotFound ), User::Invariant() );
    iFrameworkId = aDataProviderId;
    PRINTF0( "<CTestStorageMgr::SetFrameworkId" );
    }

TUint32 CTestStorageMgr::AllocateLogicalStorageIdL( TUint aDataProviderId, 
        TDriveNumber aDriveNumber, const CMTPStorageMetaData& aStorage )
    {
    PRINTF2( ">CTestStorageMgr::AllocateLogicalStorageIdL aDataProviderId = %u aDriveNumber = %d", aDataProviderId, aDriveNumber );
    TUint id( AllocateLogicalStorageIdL( aDataProviderId, 
            PhysicalStorageId( aDriveNumber ), aStorage ) );
    PRINTF1( "<CTestStorageMgr::AllocateLogicalStorageIdL id = 0x%x", id );
    return id;
    }

TUint32 CTestStorageMgr::AllocateLogicalStorageIdL( TUint aDataProviderId,
        TUint32 aPhysicalStorageId, const CMTPStorageMetaData& aStorage )
    {
    PRINTF2( ">CTestStorageMgr::AllocateLogicalStorageIdL aDataProviderId = %u aPhysicalStorageId = 0x%x", aDataProviderId, aPhysicalStorageId );
    // Resolve the physical storage.
    CMTPStorageMetaData& physical( StorageMetaDataL( aPhysicalStorageId ) );
    // Validate the SUID and storage type.
    if ( iStorages.Find( aStorage.DesC( CMTPStorageMetaData::EStorageSuid ),
            StorageKeyMatchSuid ) != KErrNotFound )
        {
        // SUID is not unique.
        User::Leave( KErrAlreadyExists );
        }
    else if ( aStorage.Uint( CMTPStorageMetaData::EStorageSystemType ) !=
              physical.Uint( CMTPStorageMetaData::EStorageSystemType ) )
        {
        // Physical/logical storage type mis-match.
        User::Leave( KErrArgument );
        }
    else if ( aStorage.Uint( CMTPStorageMetaData::EStorageSystemType ) ==
            CMTPStorageMetaData::ESystemTypeDefaultFileSystem )
        {
        // Validate that the SUID path exists.
        if ( !BaflUtils::PathExists( iFs,
                aStorage.DesC( CMTPStorageMetaData::EStorageSuid ) ) )
            {
            User::Leave( KErrPathNotFound );
            }
        
        // Validate that the SUID path corresponds to the physical storage drive.
        TInt storageDrive( DriveNumber( aPhysicalStorageId ) );
        TParse p;
        User::LeaveIfError( p.Set( aStorage.DesC(
                CMTPStorageMetaData::EStorageSuid ), NULL, NULL ) );
        TInt suidDrive( 0 );
        User::LeaveIfError( iFs.CharToDrive( TChar( p.Drive()[0] ), 
                suidDrive ) );
        if ( suidDrive != storageDrive )
            {
            // SUID path/physical storage drive mis-match.
            User::Leave( KErrArgument );
            }
        }
    
    // Allocate a logical StorageId.
    TInt32 id( AllocateLogicalStorageId( aDataProviderId,
            aPhysicalStorageId ) );
    User::LeaveIfError( id );
    
    // Create the logical storage meta-data.
    CMTPStorageMetaData* logical( CMTPStorageMetaData::NewLC( aStorage ) );
    logical->SetUint( CMTPStorageMetaData::EStorageId, id );
    
    // Store the logical storage meta-data.
    iStorages.InsertInOrderL( logical, StorageOrder );
    CleanupStack::Pop( logical );
    
    // Associate the logical and physical storages.
    RArray<TUint> logicals;
    CleanupClosePushL( logicals );
    physical.GetUintArrayL( CMTPStorageMetaData::EStorageLogicalIds,
            logicals );
    logicals.InsertInOrderL( id );
    physical.SetUintArrayL( CMTPStorageMetaData::EStorageLogicalIds,
            logicals );
    CleanupStack::PopAndDestroy( &logicals );
    
    PRINTF1( "<CTestStorageMgr::AllocateLogicalStorageIdL id = 0x%x", id );
    return id;
    }

TUint32 CTestStorageMgr::AllocatePhysicalStorageIdL( TUint aDataProviderId,
        const CMTPStorageMetaData& aStorage )
    {
    PRINTF1( ">CTestStorageMgr::AllocatePhysicalStorageIdL aDataProviderId = %u", aDataProviderId );
    // Validate the SUID.
    if ( iStorages.Find( aStorage.DesC( CMTPStorageMetaData::EStorageSuid ),
            StorageKeyMatchSuid ) != KErrNotFound )
        {
        // SUID is not unique.
        User::Leave( KErrAlreadyExists );
        }
    
    // Allocate a physical StorageId.
    TInt32 id( AllocatePhysicalStorageId( aDataProviderId ) );
    User::LeaveIfError( id );
    
    // Create the physical storage meta-data.
    CMTPStorageMetaData* physical( CMTPStorageMetaData::NewLC( aStorage ) );
    const RArray<TUint> noStorages;
    physical->SetUint( CMTPStorageMetaData::EStorageId, id );
    physical->SetUintArrayL( CMTPStorageMetaData::EStorageLogicalIds, 
            noStorages );
    
    // Store the physical storage meta-data.
    iStorages.InsertInOrderL( physical, StorageOrder );
    CleanupStack::Pop( physical );
    
    PRINTF1( "<CTestStorageMgr::AllocatePhysicalStorageIdL id = 0x%x", id );
    return id;
    }

TInt CTestStorageMgr::DeallocateLogicalStorageId( TUint aDataProviderId, 
        TUint32 aLogicalStorageId )
    {
    PRINTF2( ">CTestStorageMgr::DeallocateLogicalStorageId aDataProviderId = %d aLogicalStorageId = 0x%x", aDataProviderId, aLogicalStorageId );
    TInt result = KErrArgument;
    
    // Validate the StorageID.
    if ( LogicalStorageId( aLogicalStorageId ) )
        {
        result = iStorages.FindInOrder( aLogicalStorageId, StorageOrder );
        if ( result != KErrNotFound )
            {
            // Validate the storage owner.
            if ( LogicalStorageOwner( iStorages[result]->
                    Uint( CMTPStorageMetaData::EStorageId ) ) !=
                    aDataProviderId )
                {
                result = KErrAccessDenied;
                }
            else
                {
                TRAP( result, RemoveLogicalStorageL( result ) );
                }
            }
        }
    PRINTF1( "<CTestStorageMgr::DeallocateLogicalStorageId result = %d", result );
    return result;
    }

void CTestStorageMgr::DeallocateLogicalStorageIds( TUint aDataProviderId,
        TUint32 aPhysicalStorageId )
    {
    PRINTF2( ">CTestStorageMgr::DeallocateLogicalStorageIds aDataProviderId = %d aPhysicalStorageId = 0x%x", aDataProviderId, aPhysicalStorageId );
    TInt result = iStorages.FindInOrder( aPhysicalStorageId, StorageOrder );
    if ( result != KErrNotFound )
        {
        const RArray<TUint>& logicals( iStorages[result]->
                UintArray( CMTPStorageMetaData::EStorageLogicalIds ) );
        TUint count = logicals.Count();
        while ( count )
            {
            const TUint KIdx = count - 1;
            if ( LogicalStorageOwner( logicals[KIdx] ) == aDataProviderId )
                {
                DeallocateLogicalStorageId( aDataProviderId, logicals[KIdx] );
                }
            count--;
            }
        }
    PRINTF0( "<CTestStorageMgr::DeallocateLogicalStorageIds" );
    }

TInt CTestStorageMgr::DeallocatePhysicalStorageId( TUint aDataProviderId,
        TUint32 aPhysicalStorageId )
    {
    PRINTF2( ">CTestStorageMgr::DeallocatePhysicalStorageId aDataProviderId = %u aPhysicalStorageId = 0x%x", aDataProviderId, aPhysicalStorageId );
    TInt result = KErrArgument;
    
    // Validate the StorageID.
    if ( !LogicalStorageId( aPhysicalStorageId ) )
        {
        result = iStorages.FindInOrder( aPhysicalStorageId, StorageOrder );
        if ( result != KErrNotFound )
            {
            // Validate the storage owner.
            if ( PhysicalStorageOwner( iStorages[result]->
                    Uint( CMTPStorageMetaData::EStorageId ) ) !=
                    aDataProviderId )
                {
                result = KErrAccessDenied;
                }
            else
                {
                // Deallocate all associated logical storages.
                const RArray<TUint>& logicals( iStorages[result]->
                        UintArray( CMTPStorageMetaData::EStorageLogicalIds ) );
                TUint count = logicals.Count();
                while ( count )
                    {
                    const TUint KIdx = --count;
                    DeallocateLogicalStorageId( aDataProviderId,
                            logicals[KIdx] );
                    }
                
                // Delete the storage.
                delete iStorages[result];
                iStorages.Remove( result );
                }
            }
        }
    PRINTF1( "<CTestStorageMgr::DeallocatePhysicalStorageId result = %d", result );
    return result;
    }

TUint32 CTestStorageMgr::DefaultStorageId() const
    {
    PRINTF1( "<>CTestStorageMgr::DefaultStorageId iDefaultStorageId = 0x%x", iDefaultStorageId );
    return iDefaultStorageId;
    }

TInt CTestStorageMgr::DriveNumber( TUint32 aStorageId ) const
    {
    PRINTF1( ">CTestStorageMgr::DriveNumber aStorageId = 0x%x", aStorageId );
    TInt drive = EDriveC; // return C by default
    if ( PhysicalStorageOwner( aStorageId ) == iFrameworkId )
        {
        const TUint32 KPhysicalId = PhysicalStorageId( aStorageId );
        const TUint KCount = iMapDriveToStorage.Count();
        for ( TUint i = 0; ( i < KCount ) && ( drive == KErrNotFound ); i++ )
            {
            if ( PhysicalStorageId( iMapDriveToStorage[i] ) == KPhysicalId )
                {
                drive = i;
                }
            }
        }
    PRINTF1( "<CTestStorageMgr::DriveNumber drive = %d", drive );
    return drive;
    }

TInt32 CTestStorageMgr::FrameworkStorageId( TDriveNumber aDriveNumber ) const
    {
    PRINTF1( ">CTestStorageMgr::FrameworkStorageId aDriveNumber = %d", aDriveNumber );
    TInt32 result = KErrNotFound;
    TInt32 id = iMapDriveToStorage[aDriveNumber];
    if ( ( id != KErrNotFound ) && ( LogicalStorageId( id ) ) )
        {
        result = id;
        }
    PRINTF1( "<CTestStorageMgr::FrameworkStorageId id = 0x%x", result );
    return result;
    }

 void CTestStorageMgr::GetAvailableDrivesL(
         RArray<TDriveNumber>& aDrives ) const
    {
    CleanupClosePushL( aDrives ); 
    PRINTF0( ">CTestStorageMgr::GetAvailableDrivesL" );
    aDrives.Reset();
    for ( TUint i = 0; ( i < iMapDriveToStorage.Count() ); i++ )
        {
        if ( iMapDriveToStorage[i] != KErrNotFound )
            {
            PRINTV1( "Appending drive %d", i );
            aDrives.AppendL( static_cast<TDriveNumber>( i ) );
            }
        }
    PRINTF0( "<CTestStorageMgr::GetAvailableDrivesL" );
    CleanupStack::Pop(); 
    }

void CTestStorageMgr::GetLogicalStoragesL(
        const TMTPStorageMgrQueryParams& aParams,
        RPointerArray<const CMTPStorageMetaData>& aStorages ) const
    {
    PRINTF2( ">CTestStorageMgr::GetLogicalStoragesL suid = %S system type = %d", &aParams.StorageSuid(), aParams.StorageSystemType() );
    aStorages.Reset();
    const TBool KAllStorages = ( aParams.StorageSuid() == KNullDesC );
    const TBool KAllStorageSystemTypes = ( aParams.StorageSystemType() ==
            CMTPStorageMetaData::ESystemTypeUndefined );
    const TUint KCount = iStorages.Count();
    for ( TUint i = 0; i < KCount; i++ )
        {
        const CMTPStorageMetaData& storage = *iStorages[i];
        if ( ( ( KAllStorages ) ||
               ( storage.DesC( CMTPStorageMetaData::EStorageSuid ) ==
                       aParams.StorageSuid() ) ) &&
             ( ( KAllStorageSystemTypes) ||
               ( storage.Uint( CMTPStorageMetaData::EStorageSystemType ) ==
                         aParams.StorageSystemType() ) ) &&
             ( LogicalStorageId(
                     storage.Uint( CMTPStorageMetaData::EStorageId ) ) ) )
            {
            PRINTV1( "Appending storage 0x%x", storage.Uint( CMTPStorageMetaData::EStorageId ) );
            aStorages.AppendL( iStorages[i] );
            }
        }
    PRINTF0( "<CTestStorageMgr::GetLogicalStoragesL" );
    }

void CTestStorageMgr::GetPhysicalStoragesL( 
        const TMTPStorageMgrQueryParams& aParams, 
        RPointerArray<const CMTPStorageMetaData>& aStorages ) const
    {
    PRINTF2( ">CTestStorageMgr::GetPhysicalStoragesL suid = %S system type = %d", &aParams.StorageSuid(), aParams.StorageSystemType() );
    aStorages.Reset();
    const TBool KAllStorages = ( aParams.StorageSuid() == KNullDesC );
    const TBool KAllStorageSystemTypes = ( aParams.StorageSystemType() ==
            CMTPStorageMetaData::ESystemTypeUndefined );
    const TUint KCount = iStorages.Count();
    for ( TUint i = 0; i < KCount; i++ )
        {
        const CMTPStorageMetaData& storage = *iStorages[i];
        if ( ( ( KAllStorages ) ||
               ( storage.DesC( CMTPStorageMetaData::EStorageSuid ) ==
                       aParams.StorageSuid() ) ) &&
             ( ( KAllStorageSystemTypes ) ||
               ( storage.Uint( CMTPStorageMetaData::EStorageSystemType ) ==
                       aParams.StorageSystemType() ) ) &&
             ( !LogicalStorageId( 
                     storage.Uint( CMTPStorageMetaData::EStorageId ) ) ) )
            {
            PRINTV1( "Appending storage 0x%x", storage.Uint( CMTPStorageMetaData::EStorageId ) );
            aStorages.AppendL( iStorages[i] );
            }
        }
    PRINTF0( "<CTestStorageMgr::GetPhysicalStoragesL" );
    }

TUint32 CTestStorageMgr::LogicalStorageId( TUint32 aStorageId ) const
    {
    PRINTF1( ">CTestStorageMgr::LogicalStorageId aStorageId = 0x%x", aStorageId );
    TUint32 id = 1;
    PRINTF1( "<CTestStorageMgr::LogicalStorageId id = 0x%x", id );
    return id;
    }

TInt32 CTestStorageMgr::LogicalStorageId( const TDesC& aStorageSuid ) const
    {
    PRINTF1( ">CTestStorageMgr::LogicalStorageId aStorageSuid = %S", &aStorageSuid );
    TInt32 id = KErrNotFound;
    TInt idx = iStorages.Find( aStorageSuid, StorageKeyMatchSuid );
    if ( idx != KErrNotFound )
        {
        id = iStorages[ idx ]->Uint( CMTPStorageMetaData::EStorageId );
        if ( !LogicalStorageId( id ) )
            {
            id = KErrNotFound;
            }
        }
    PRINTF1( "<CTestStorageMgr::LogicalStorageId id = 0x%x", id );
    return id;
    }

TInt32 CTestStorageMgr::PhysicalStorageId( TDriveNumber aDriveNumber ) const
    {
    PRINTF1( ">CTestStorageMgr::PhysicalStorageId aDriveNumber = %d", aDriveNumber );
    TInt32 storageId = iMapDriveToStorage[aDriveNumber];
    if ( storageId != KErrNotFound )
        {
        storageId = PhysicalStorageId( storageId );
        }
    PRINTF1( "<CTestStorageMgr::PhysicalStorageId storageId = 0x%x", storageId );
    return storageId;
    }

TUint32 CTestStorageMgr::PhysicalStorageId( TUint32 aStorageId ) const
    {
    PRINTF1( ">CTestStorageMgr::PhysicalStorageId aStorageId = 0x%x", aStorageId );
    TUint32 id = ( aStorageId & KPhysicalIdMask );
    PRINTF1( "<CTestStorageMgr::PhysicalStorageId id = 0x%x", id );
    return id;
    }

const CMTPStorageMetaData& CTestStorageMgr::StorageL( 
        TUint32 aStorageId ) const
    {
    PRINTF1( ">CTestStorageMgr::StorageL aStorageId = 0x%x", aStorageId );
    TInt idx = iStorages.FindInOrder( aStorageId, StorageOrder );
    User::LeaveIfError( idx );
    PRINTF1( "<CTestStorageMgr::StorageL idx = %d", idx );
    return *iStorages[idx];
    }

TUint32 CTestStorageMgr::StorageId( TUint32 aPhysicalStorageId, 
        TUint32 aLogicalStorageId ) const
    {
    PRINTF2( ">CTestStorageMgr::StorageId aPhysicalStorageId = 0x%x aLogicalStorageId = 0x%x", aPhysicalStorageId, aLogicalStorageId );
    TUint32 id = ( aPhysicalStorageId | aLogicalStorageId );
    PRINTF1( "<CTestStorageMgr::StorageId id = 0x%x", id );
    return id;
    }

TBool CTestStorageMgr::ValidStorageId( TUint32 aStorageId ) const
    {
    PRINTF1( ">CTestStorageMgr::ValidStorageId aStorageId = 0x%x", aStorageId );
    TBool valid = 
            iStorages.FindInOrder( aStorageId, StorageOrder ) != KErrNotFound;
    PRINTF1( "<CTestStorageMgr::ValidStorageId valid = %d", valid );
    return valid;
    }

CMTPTypeString* CTestStorageMgr::VolumeIdL( TUint aDataProviderId,
        TUint32 aStorageId, const TDesC& aVolumeIdSuffix ) const
    {
    PRINTF3( ">CTestStorageMgr::VolumeIdL aDataProviderId = %u aStorageId = 0x%x aVolumeIdSuffix = %S", aDataProviderId, aStorageId, &aVolumeIdSuffix );
    // Validate the StorageId.
    TUint owner = LogicalStorageId( aStorageId ) ?
            LogicalStorageOwner( aStorageId ) :
            PhysicalStorageOwner( aStorageId );
    if ( !ValidStorageId( aStorageId ) )
        {
        User::Leave( KErrNotFound );
        }
    else if ( aDataProviderId != owner )
        {
        User::Leave( KErrAccessDenied );
        }
    
    // Generate a unique volume ID.
    RBuf16 buffer;
    buffer.CreateL( KMTPMaxStringCharactersLength );
    CleanupClosePushL( buffer );
    buffer.Format( _L( "%08X" ), aStorageId );
    
    if ( aVolumeIdSuffix.Length() != 0 )
        {
        // Append the separator and suffix, truncating if necessary.
        buffer.Append( _L( "-" ) );
        buffer.Append( aVolumeIdSuffix.Left( KMTPMaxStringCharactersLength - 
                buffer.Length() ) );
        }
    
    CMTPTypeString* volumeId = CMTPTypeString::NewL( buffer );
    CleanupStack::PopAndDestroy( &buffer );
    
    PRINTF1( "<CTestStorageMgr::VolumeIdL volumeId = %S", volumeId );
    return volumeId;
    }

/**
Constructor.
*/
CTestStorageMgr::CTestStorageMgr() :
        iFrameworkId( KErrNotFound )
    {
    // implementation not required
    }
    
/**
Second phase constructor.
@leave One of the system wide error code, if a processing failure occurs.
*/
void CTestStorageMgr::ConstructL()
    {
    PRINTF0( ">CTestStorageMgr::ConstructL" );
    
    iFs.Connect();
    for ( TUint i = 0; i < KMaxDrives; i++ )
        {
        iMapDriveToStorage[i] = KErrNotFound;
        }
    PRINTF0( "<CTestStorageMgr::ConstructL" );
    }

/**
Allocates a new 32-bit logical StorageId for the storage owner as a partition 
of the specified physical MTP StorageID.
@param aDataProviderId The storage owner data provider identifier.
@param aPhysicalStorageId The physical MTP StorageID.
@return The new logical StorageId.
@return KErrNotFound, if the specified physical MTP StorageID does not exist
@return KErrOverflow, if the maximum number of storages would be exceeded.
*/
TInt32 CTestStorageMgr::AllocateLogicalStorageId( TUint aDataProviderId,
        TUint32 aPhysicalStorageId )
    {
    PRINTF2( ">CTestStorageMgr::AllocateLogicalStorageId aDataProviderId = %d aPhysicalStorageId = 0x%x", aDataProviderId, aPhysicalStorageId );
    TInt result = iStorages.FindInOrder( aPhysicalStorageId, StorageOrder );
    if ( result != KErrNotFound )
        {
        // Scan for the first available storage number.
        const RArray<TUint>& logicalIds( iStorages[result]->
                UintArray( CMTPStorageMetaData::EStorageLogicalIds ) );
        TUint num = 1;
        do
            {
            result = EncodeLogicalStorageId( aPhysicalStorageId,
                    aDataProviderId, num );
            }
        while ( ( logicalIds.FindInOrder( result ) != KErrNotFound ) &&
                ( ++num <= KMaxOwnedStorages ) );
        
        if ( num >= KMaxOwnedStorages )
            {
            result = KErrOverflow;
            }
        }
    PRINTF1( "<CTestStorageMgr::AllocateLogicalStorageId result = %d", result );
    return result;
    }

/**
Allocates a new 32-bit physical StorageId for the storage owner.
@param aDataProviderId The storage owner data provider identifier.
@return The new physical StorageId.
@return KErrOverflow, if the maximum number of storages would be exceeded.
@return One of the system wide error code, if a processing failure occurs.
*/
TInt32 CTestStorageMgr::AllocatePhysicalStorageId( TUint aDataProviderId )
    {
    PRINTF1( ">CTestStorageMgr::AllocatePhysicalStorageId aDataProviderId = %d", aDataProviderId );
    TInt32 result = KErrNone;
    while ( ( iPhysicalStorageNumbers.Count() < ( aDataProviderId + 1 ) ) && 
            ( result == KErrNone ) )
        {
        result = iPhysicalStorageNumbers.Append( 0 );
        }
    
    if ( result == KErrNone )
        {
        if ( iPhysicalStorageNumbers[aDataProviderId] < KMaxOwnedStorages )
            {
            result = EncodePhysicalStorageId( aDataProviderId,
                    ++iPhysicalStorageNumbers[aDataProviderId] );
            }
        else
            {
            result = KErrOverflow;
            }
        }
    PRINTF1( "<CTestStorageMgr::AllocatePhysicalStorageId result = %d", result );
    return result;
    }

/**
Encodes the specified physical MTP StorageID, data provider identifier, and 
storage number as a fully formed MTP StorageID.
@param aPhysicalStorageId The physical MTP StorageID.
@param aDataProviderId The data provider identifier.
@param aStorageNumber The storage number.
@return The fully formed MTP StorageID.
*/  
TUint32 CTestStorageMgr::EncodeLogicalStorageId( TUint32 aPhysicalStorageId,
        TUint aDataProviderId, TUint aStorageNumber )
    {
    PRINTF3( ">CTestStorageMgr::EncodeLogicalStorageId aPhysicalStorageId = 0x%x aDataProviderId = %d aStorageNumber = %d", aPhysicalStorageId, aDataProviderId, aStorageNumber );
    TUint32 id = StorageId( aPhysicalStorageId,
            EncodeLogicalStorageOwner( aDataProviderId) | 
            EncodeLogicalStorageNumber( aStorageNumber ) );
    PRINTF1( "<CTestStorageMgr::EncodeLogicalStorageId id = 0x%x", id );
    return id;
    }

/**
Encodes the storage identifier as the logical storage number in a fully formed 
MTP StorageID.
@param aStorageNumber The storage number.
@return The encoded logical storage number.
*/  
TUint32 CTestStorageMgr::EncodeLogicalStorageNumber( TUint aStorageNumber )
    {
    PRINTF1( "<>CTestStorageMgr::EncodeLogicalStorageNumber aStorageNumber = 0x%x", aStorageNumber );
    return aStorageNumber;
    }

/**
Encodes the specified data provider identifier as the logical storage owner 
in a fully formed MTP StorageID.
@param aDataProviderId The data provider identifier.
@return The encoded logical storage owner.
*/
TUint32 CTestStorageMgr::EncodeLogicalStorageOwner( TUint aDataProviderId )
    {
    PRINTF1( ">CTestStorageMgr::EncodeLogicalStorageOwner aDataProviderId = %d", aDataProviderId );
    TUint32 owner = ( aDataProviderId << KLogicalOwnerShift );
    PRINTF1( "<CTestStorageMgr::EncodeLogicalStorageOwner with result 0x%x", owner );
    return owner;
    }
    
/**
Encodes the specified data provider identifier and storage number as an  
physical MTP StorageID.
@param aDataProviderId The data provider identifier.
@param aStorageNumber The storage number.
@return The encoded physical MTP StorageID.
*/  
TUint32 CTestStorageMgr::EncodePhysicalStorageId( TUint aDataProviderId,
        TUint aStorageNumber )
    {
    PRINTF2( ">CTestStorageMgr::EncodePhysicalStorageId aDataProviderId = %d, aStorageNumber = %d", aDataProviderId, aStorageNumber );
    TUint32 id = EncodePhysicalStorageOwner( aDataProviderId ) |
            EncodePhysicalStorageNumber( aStorageNumber );
    PRINTF1( "<CTestStorageMgr::EncodePhysicalStorageId id = 0x%x", id );
    return id;
    }

/**
Encodes the storage identifier as the physical storage number in a fully formed 
MTP StorageID.
@param aStorageNumber The storage number.
@return The encoded physical storage number.
*/  
TUint32 CTestStorageMgr::EncodePhysicalStorageNumber( TUint aStorageNumber )
    {
    PRINTF1( ">CTestStorageMgr::EncodePhysicalStorageNumber aStorageNumber = 0x%x", aStorageNumber );
    TUint32 number = ( aStorageNumber << KPhysicalNumberShift );
    PRINTF1( "<CTestStorageMgr::EncodePhysicalStorageNumber number = 0x%x", number );
    return number;
    }

/**
Encodes the specified data provider identifier as the physical storage owner 
in a fully formed MTP StorageID.
@param aDataProviderId The data provider identifier.
@return The encoded physical storage owner.
*/
TUint32 CTestStorageMgr::EncodePhysicalStorageOwner( TUint aDataProviderId )
    {
    PRINTF1( ">CTestStorageMgr::EncodePhysicalStorageOwner aDataProviderId = 0x%x", aDataProviderId );
    TUint32 owner = ( aDataProviderId << KPhysicalOwnerShift );
    PRINTF1( "<CTestStorageMgr::EncodePhysicalStorageOwner owner = 0x%x", owner );
    return owner;
    }

/**
Removes the logical storages table entry at the specified index.
@param aIdx The storages table index.
@leave One of the system wide error codes, if a processing failure occurs.
*/
void CTestStorageMgr::RemoveLogicalStorageL( TUint aIdx )
    {
    PRINTF1( ">CTestStorageMgr::RemoveLogicalStorageL aIdx = %u", aIdx );
    TUint32 id = iStorages[aIdx]->Uint( CMTPStorageMetaData::EStorageId );
    PRINTV1( "Removing storage with id 0x%x", id );
    // Disassociate the logical and physical storages.
    CMTPStorageMetaData& physical( StorageMetaDataL( 
            PhysicalStorageId( id ) ) );
    RArray<TUint> logicals;
    CleanupClosePushL( logicals );
    physical.GetUintArrayL( CMTPStorageMetaData::EStorageLogicalIds,
            logicals );
    logicals.Remove( logicals.FindInOrderL( id ) );
    physical.SetUintArrayL( CMTPStorageMetaData::EStorageLogicalIds,
            logicals );
    CleanupStack::PopAndDestroy( &logicals );
    
    // Delete the storage.
    delete iStorages[aIdx];
    iStorages.Remove( aIdx );
    PRINTF0( "<CTestStorageMgr::RemoveLogicalStorageL" );
    }

/**
Provides a non-const reference to the storage meta-data for the specified 
logical MTP StorageID.
@param aStorageId The physical or fully formed logical MTP StorageID.
@leave KErrNotFound if the specified StorageID does not exist.
@leave One of the system wide error codes, if a processing failure occurs.
*/
CMTPStorageMetaData& CTestStorageMgr::StorageMetaDataL( TUint32 aStorageId )
    {
    PRINTF1( ">CTestStorageMgr::StorageMetaDataL aStorageId = 0x%x", aStorageId );
    TInt idx = iStorages.FindInOrder( aStorageId, StorageOrder );
    User::LeaveIfError( idx );
    PRINTF1( "<CTestStorageMgr::StorageMetaDataL idx = %d", idx );
    return *iStorages[idx];
    }

/**
Implements a storage key match identity relation using 
@see CMTPStorageMetaData::EStorageSuid.
@param aSuid The storage SUID key value.
@param aStorage The storage meta-data.
@return ETrue if the storage matches the key relation, otherwise EFalse.
*/ 
TBool CTestStorageMgr::StorageKeyMatchSuid( const TDesC* aSuid,
        const CMTPStorageMetaData& aStorage )
    {
    //PRINTF2( ">StorageKeyMatchSuid aSuid = %S storage id = 0x%x", &aSuid, aStorage.Uint( CMTPStorageMetaData::EStorageId ) );
    TBool match = ( *aSuid ==
            aStorage.DesC( CMTPStorageMetaData::EStorageSuid ) );
    PRINTF1( "<CTestStorageMgr::StorageKeyMatchSuid match = %d", match );
    return match;
    }
    
/**
Implements an @see TLinearOrder function for @see CMTPStorageMetaData objects 
based on relative @see CMTPStorageMetaData::EStorageId.
@param aL The first object instance.
@param aR The second object instance.
@return Zero, if the two objects are equal; A negative value, if the first 
object is less than the second, or; A positive value, if the first object is 
greater than the second.
*/
TInt CTestStorageMgr::StorageOrder( const CMTPStorageMetaData& aL,
        const CMTPStorageMetaData& aR )
    {
    PRINTF2( ">CTestStorageMgr::StorageOrder left id = 0x%x right id = 0x%x", aL.Uint( CMTPStorageMetaData::EStorageId ), aR.Uint( CMTPStorageMetaData::EStorageId ) );
    TInt order = aL.Uint( CMTPStorageMetaData::EStorageId ) -
                 aR.Uint( CMTPStorageMetaData::EStorageId );
    PRINTF1( "<CTestStorageMgr::StorageOrder order = %d", order );
    return order;
    }

/**
Implements an @see CMTPStorageMetaData::EStorageId key order function.
@param aKey The key value.
@param aR The storage meta-data.
@return Zero, if the two objects are equal; A negative value, if the first 
object is less than the second, or; A positive value, if the first object is 
greater than the second.
*/
TInt CTestStorageMgr::StorageOrder( const TUint32* aKey, const CMTPStorageMetaData& aStorage )
    {
    PRINTF2( ">CTestStorageMgr::StorageOrder aKey = 0x%x storage id = 0x%x", *aKey, aStorage.Uint( CMTPStorageMetaData::EStorageId ) );
    TInt order = *aKey - aStorage.Uint( CMTPStorageMetaData::EStorageId );
    PRINTF1( "<CTestStorageMgr::StorageOrder order = %d", order );
    return order;
    }
