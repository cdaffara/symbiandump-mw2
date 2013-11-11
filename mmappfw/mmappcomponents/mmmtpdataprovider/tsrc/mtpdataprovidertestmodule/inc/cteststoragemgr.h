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


#ifndef CTESTSTORAGEMGR_H
#define CTESTSTORAGEMGR_H

#include <stiflogger.h>
#include <e32base.h>
#include <f32file.h>
#include <mtp/mmtpstoragemgr.h>

class CTestStorageMgr :
        public CBase,
        public MMTPStorageMgr
    {
public:
    static CTestStorageMgr* NewL();
    ~CTestStorageMgr();
    
    static TUint LogicalStorageNumber( TUint32 aStorageId );
    static TUint LogicalStorageOwner( TUint32 aStorageId );
    static TUint PhysicalStorageNumber( TUint32 aStorageId );
    static TUint PhysicalStorageOwner( TUint32 aStorageId );
    
    void SetDefaultStorageId( TUint32 aStorageId );
    void SetDriveMappingL( TDriveNumber aDriveNumber, TUint32 aStorageId );
    void SetFrameworkId( TUint aDataProviderId );
public: // From MMTPStorageMgr
    TUint32 AllocateLogicalStorageIdL( TUint aDataProviderId,
            TDriveNumber aDriveNumber, const CMTPStorageMetaData& aStorage );
    TUint32 AllocateLogicalStorageIdL( TUint aDataProviderId,
            TUint32 aPhysicalStorageId, const CMTPStorageMetaData& aStorage );
    TUint32 AllocatePhysicalStorageIdL( TUint aDataProviderId,
            const CMTPStorageMetaData& aStorage );
    TInt DeallocateLogicalStorageId( TUint aDataProviderId,
            TUint32 aLogicalStorageId );
    void DeallocateLogicalStorageIds( TUint aDataProviderId,
            TUint32 aPhysicalStorageId );
    TInt DeallocatePhysicalStorageId( TUint aDataProviderId,
            TUint32 aPhysicalStorageId );
    TUint32 DefaultStorageId() const;
    TInt DriveNumber( TUint32 aStorageId ) const;
    TInt32 FrameworkStorageId( TDriveNumber aDriveNumber ) const;
    void GetAvailableDrivesL( RArray<TDriveNumber>& aDrives ) const;
    void GetLogicalStoragesL( const TMTPStorageMgrQueryParams& aParams,
            RPointerArray<const CMTPStorageMetaData>& aStorages ) const;
    void GetPhysicalStoragesL( const TMTPStorageMgrQueryParams& aParams,
            RPointerArray<const CMTPStorageMetaData>& aStorages ) const;
    TUint32 LogicalStorageId( TUint32 aStorageId ) const;
    TInt32 LogicalStorageId( const TDesC& aStorageSuid ) const;
    TInt32 PhysicalStorageId( TDriveNumber aDriveNumber ) const;
    TUint32 PhysicalStorageId( TUint32 aStorageId ) const;
    const CMTPStorageMetaData& StorageL( TUint32 aStorageId ) const;
    TUint32 StorageId( TUint32 aPhysicalStorageId,
            TUint32 aLogicalStorageId ) const;
    TBool ValidStorageId( TUint32 aStorageId ) const;
    CMTPTypeString* VolumeIdL( TUint aDataProviderId, TUint32 aStorageId,
            const TDesC& aVolumeIdSuffix ) const;
private:
    CTestStorageMgr();
    void ConstructL();
    
    TInt32 AllocateLogicalStorageId( TUint aDataProviderId,
            TUint32 aPhysicalStorageId );
    TInt32 AllocatePhysicalStorageId( TUint aDataProviderId );
    TUint32 EncodeLogicalStorageId( TUint32 aPhysicalStorageId,
            TUint aDataProviderId, TUint aStorageNumber );
    TUint32 EncodeLogicalStorageNumber( TUint aStorageNumber );
    TUint32 EncodeLogicalStorageOwner( TUint aDataProviderId );
    TUint32 EncodePhysicalStorageId( TUint aDataProviderId,
            TUint aStorageNumber );
    TUint32 EncodePhysicalStorageNumber( TUint aStorageNumber );
    TUint32 EncodePhysicalStorageOwner( TUint aDataProviderId );
    void RemoveLogicalStorageL( TUint aIdx );
    CMTPStorageMetaData& StorageMetaDataL( TUint32 aStorageId );
    
    static TBool StorageKeyMatchSuid( const TDesC* aSuid,
            const CMTPStorageMetaData& aStorage );
    static TInt StorageOrder( const CMTPStorageMetaData& aL,
            const CMTPStorageMetaData& aR );
    static TInt StorageOrder( const TUint32* aKey,
            const CMTPStorageMetaData& aStorage );
    
private: // owned data
    /**
    The default MTP storage ID.
    */
    TUint32 iDefaultStorageId;
    
    /**
    The framework storages (drives) owning data provider ID.
    */
    TInt iFrameworkId;

    /**
    The Symbian OS drive number to default MTP StorageID mapping table. This 
    StorageID may either be the physical MTP StorageID or the default logical 
    MTP StorageID.
    */
    TFixedArray<TInt, KMaxDrives> iMapDriveToStorage;
    
    /**
    The physical storage numbers allocation table. Indexed by physical storage 
    owner data provider ID.
    */
    RArray<TUint> iPhysicalStorageNumbers;
    
    /**
    The storages table.
    */
    RPointerArray<CMTPStorageMetaData> iStorages;
    
    RFs iFs;
    static CStifLogger* iLog;
    };

#endif // CTESTSTORAGEMGR_H
