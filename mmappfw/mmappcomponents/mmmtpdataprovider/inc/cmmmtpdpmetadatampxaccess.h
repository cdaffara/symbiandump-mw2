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

#ifndef CMMMTPDPMETADATAMPXACCESS_H
#define CMMMTPDPMETADATAMPXACCESS_H

#include <badesca.h>

#include "cmmmtpdpperflog.h"

// forward declacration
class CMPXMedia;
class CMTPObjectMetaData;
class MMPXCollectionHelper;
class MMTPDataProviderFramework;
class MMTPType;
class TMPXAttributeData;

const TInt KStorageRootMaxLength = 10;

typedef struct
    {
    TMTPObjectPropertyCode iPropCode;
    TMPXAttributeData iMpxAttrib;
    } TMetadataTable;

/**
 * Controls access to the music MPX store database.  Allows access to the
 * information about any of the music stored in any of the MPX db's on any
 * of the drives. Also supports deletions and insertions into the db.
 */
class CMmMtpDpMetadataMpxAccess : public CBase
    {
public:

    static CMmMtpDpMetadataMpxAccess* NewL( RFs& aRfs );

    /**
     * Destructor
     */
    ~CMmMtpDpMetadataMpxAccess();

public:
    /**
     * OpenSession, introduce to fix CollectionHelper Flush problem
     */
    void OpenSession();

    /**
     * CloseSession, introduce to fix CollectionHelper Flush problem
     */
    void CloseSession();

    /**
     * Get all abstract medias from MPX database in the assigned store
     * @param aStoreRoot, specify in which drive abstract medias are stored
     * @param aAbstractMedias, return result array
     * @param aCategory, indicate the category of abstract medias
     */
    void GetAllAbstractMediaL( const TDesC& aStoreRoot,
        CDesCArray& aAbstractMedias,
        TMPXGeneralCategory aCategory );

    /**
     * Get all references of specified abstract media
     * @param aAbstractMedia, specify of which reference should be get
     * @param aReferences, return result array which stored handles of all references
     */
    void GetAllReferenceL( const TDesC& aRefOwnerSuid,
        TMPXGeneralCategory aCategory,
        CDesCArray& aReferences );

    /**
     * Adds Songs info to Mpx DB
     * @param aFullFileName, full file name of Media file
     */
    void AddSongL( const TDesC& aFullFileName );

    /**
     * Adds abstract media to Mpx DB
     * @param aFullFileName, full file name of abstract media file
     * @param aCategory, indicate the category of abstract media
     */
    void AddAbstractMediaL( const TDesC& aFullFileName,
        TMPXGeneralCategory aCategory );

    /**
     * Set abstract media to DB
     * @param aRefOwnerName, full file name of abstract media file
     * @param aRefFileArray, a array to store the full file name of media files
     * @param aCategory, indicate the category of abstract media
     */
    void SetReferenceL( const TDesC& aRefOwnerName,
        CDesCArray& aRefFileArray,
        TMPXGeneralCategory aCategory );

    void GetObjectMetadataValueL( const TUint16 aPropCode,
        MMTPType& aNewData,
        const TDesC& aFullFileName,
        TMPXGeneralCategory aCategory );

    /**
     * Set object property and updated object in DB according property code,
     * only for update, not for creation
     * @param aPropCode, property code of aObjectMetaData
     * @param aNewData, object property value which will be set into
     *     aObjectMetaData
     * @param aObjectMetaData, owner of the property which should be
     *     updated into database
     */
    void SetObjectMetadataValueL( const TUint16 aPropCode,
        const MMTPType& aNewData,
        const TDesC& aFullFileName,
        TMPXGeneralCategory aCategory );

    CMPXMedia* FindWMPMediaLC( TMPXAttributeData aWMPMediaID,
        TBool aFlag );

    /**
     * Update the Sync flag for those not synchronized, Update the Modified
     * flag for those have been modified, and delete the stale records for
     * files that have been deleted.
     */
    void UpdateMusicCollectionL();

    /**
     * Remove the file from MPX DB
     * @param aFullFileName, the file to remove
     * @param aCategory, the category of the file
     */
    void DeleteObjectL( const TDesC& aFullFileName,
        TMPXGeneralCategory aCategory );

    /**
     * Rename the filename onto MPX DB
     * @param aOldFileName, the old file to rename
     * @param aNewFileName, the new file name
     * @param aCategory, the category of object
     */
    void RenameObjectL( const TDesC& aOldFileName,
        const TDesC& aNewFileName,
        TMPXGeneralCategory aCategory );

    /**
     * Set current  drive info
     * @param aStorageRoot, current drive info
     */
    void SetStorageRootL( const TDesC& aStorageRoot );

    /**
     * Get modified content
     * @param arrayCount, count of modified content
     * @param aModifiedcontent, modified files name list
     */
    void GetModifiedContentL( TInt& arrayCount,
        CDesCArray& aModifiedcontent );

    /**
     * if the playlsit exist in the MPX DB
     * @param aSuid, the suid of playlsit
     */
    TBool IsExistL( const TDesC& aSuid );

private:
    CMmMtpDpMetadataMpxAccess( RFs& aFs );

    void ConstructL();

    void SetDefaultL( CMPXMedia& aMediaProp );

    /**
     * Set property value into MPX object according to property code,
     * only used by member functions of this class.
     * @param aPropCode, specify property code of aMediaProp
     * @param aNewData, object property value which will be set into
     *     aObjectMetaData
     * @param aObjectMetaData, owner of the property which should be
     *     inserted or updated into database
     */
    void SetMetadataValueL( const TUint16 aPropCode,
        const MMTPType& aNewData,
        CMPXMedia& aMediaProp );

    TMPXAttributeData MpxAttribFromPropL( const TUint16 aPropCode );

    MMPXCollectionHelper* CollectionHelperL();

private:
    RFs& iRfs;

    TBuf<KStorageRootMaxLength> iStoreRoot;

    MMPXCollectionHelper* iCollectionHelper;
    HBufC8* iSampleData;

    TBool iHasCleanUp;

#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
    CMmMtpDpPerfLog* iPerfLog;
#endif

    };

#endif // CMMMTPDPMETADATAMPXACCESS_H
