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
* Description:  Meta data Access Wrapper
*
*/


#ifndef CMMMTPDPMETADATAACCESSWRAPPER_H
#define CMMMTPDPMETADATAACCESSWRAPPER_H

#include <mpxmediageneraldefs.h>
#include <badesca.h>

// forward declacration
class CMmMtpDpMetadataMpxAccess;
class CMmMtpDpMetadataVideoAccess;

class RFs;
class MMTPType;
class CMPXMediaArray;
class CMPXMedia;
class MMTPDataProviderFramework;
class CMTPObjectMetaData;

class CMmMtpDpMetadataAccessWrapper : public CBase
    {
public:
    static CMmMtpDpMetadataAccessWrapper* NewL( MMTPDataProviderFramework& aFramework );

    /**
    * Destructor
    */
    ~CMmMtpDpMetadataAccessWrapper();

public:
    /**
    * Get all abstract medias from MPX database in the assigned store
    * @param aStoreRoot, specify in which drive abstract medias are stored
    * @param aAbstractMedias, return result array
    * @param aCategory, indicate the category of abstract medias
    */
    IMPORT_C void GetAllAbstractMediaL( const TDesC& aStoreRoot,
        CDesCArray& aAbstractMedias,
        TMPXGeneralCategory aCategory );

    /**
    * Get all references of specified abstract medias
    * @param aAbstractMedia, specify of which reference should be get
    * @param aReferences, return result array which stored handles of all references
    */
    IMPORT_C void GetAllReferenceL( const CMTPObjectMetaData& aRefOwner,
        CDesCArray& aReferences );

    /**
    * Add object (music, video, playlist and abstract album) info to DB
    * @param aFullFileName, full file name of file
    * @return void
    */
    IMPORT_C void AddObjectL( const CMTPObjectMetaData& aObject );

    /**
    * Set abstract media to DB
    * @param aObject,
    * @param aRefFileArray, a array to store the full file name of media files
    * @return void
    */
    IMPORT_C void SetReferenceL( const CMTPObjectMetaData& aObject,
        CDesCArray& aRefFileArray );

    /**
    * Gets a piece of metadata from the collection
    * @param aPropCode, specify property code of aMediaProp
    * @param aNewData, object property value which will be get from
    *    aObjectMetaData
    * @param aObjectMetaData, owner of the property which should be
    *    inserted or updated into database
    * @return void
    */
    IMPORT_C void GetObjectMetadataValueL( const TUint16 aPropCode,
        MMTPType& aNewData,
        const CMTPObjectMetaData& aObjectMetaData );

    /**
    * Sets a piece of metadata in the collection
    * @param aPropCode, specify property code of aMediaProp
    * @param aNewData, object property value which will be set into
    *     aObjectMetaData
    * @param aObjectMetaData, owner of the property which should be
    *     inserted or updated into database
    * @return void
    */
    void SetObjectMetadataValueL( const TUint16 aPropCode,
        const MMTPType& aNewData,
        const CMTPObjectMetaData& aObjectMetaData );

    /**
    * Renames the file part of a record in the collection database
    * @param aOldObject, object need to be renamed
    * @param aNewFileName, new file name need update
    * @return void
    */
    IMPORT_C void RenameObjectL( const CMTPObjectMetaData& aOldObject, const TDesC& aNewFileName );

    /**
    * Deletes metadata information associated with the object
    * @param aObject, the object need to be deleted
    */
    IMPORT_C void DeleteObjectL( const CMTPObjectMetaData& aObject );

    /**
    * Set current  drive info
    * @param aStorageRoot, current drive info
    */
    void SetStorageRootL( const TDesC& aStorageRoot );

    /*
    * set image specific properties specific to videos
    * @param aFullFileName, the fullFileName of object
    * @param aWidth,  the width of an object in pixels to set
    * @parem aHeight,  the height of an object in pixels to set
    */
    void SetImageObjPropL( const CMTPObjectMetaData& aObject,
        const TUint32 aWidth,
        const TUint32 aHeight );

    /*
    * get image specific properties specific to videos
    * @param aFullFileName, the fullFileName of object
    * @param aWidth,  the width of an object in pixels to get
    * @parem aHeight,  the height of an object in pixels to get
    */
    void GetImageObjPropL( const CMTPObjectMetaData& aObject,
        TUint32& aWidth,
        TUint32& aHeight );

    /**
    * Get Modified object from DB
    * @param aStorageRoot, the root path of storage
    * @param arrayCount, array count
    * @param aRefFileArray, a array to store the full file name of media files
    * @return void
    */
    IMPORT_C void GetModifiedContentL( const TDesC& aStorageRoot,
        TInt& arrayCount,
        CDesCArray& aRefFileArray );

    /**
    * Updated Music DB
    * @param aStorageRoot, the root path of storage
    * @return void
    */
    IMPORT_C void UpdateMusicCollectionL( const TDesC& aStorageRoot );

    /*
    * Called when the MTP session is initialised
    */
    void OpenSessionL();

    /*
    * clean up db resource especially for video dp
    */
    void CloseSessionL();

    /**
    * Cleanup database
    */
    IMPORT_C void CleanupDatabaseL();

    /**
    * if the playlsit exist in the MPX DB
    * @param aSuid, the suid of playlsit
    */
    TBool IsExistL( const TDesC& aSuid );

    // related to dummy files
    /**
    * Add one dummy file to dummy files array
    */
    IMPORT_C void AddDummyFileL( const TDesC& aDummyFileName );

    /**
    * Delete one dummy file from dummy files array
    */
    IMPORT_C void DeleteDummyFile( const TDesC& aDummyFileName );

    /**
    * Create a Dummy File from the virtual playlist URI
    * @param aPlaylistName, specify the filename of the dummy file
    */
    IMPORT_C void CreateDummyFile( const TDesC& aPlaylistName );

    TMPXGeneralCategory Category( const CMTPObjectMetaData& aObject );

private:

    CMmMtpDpMetadataAccessWrapper( MMTPDataProviderFramework& aFramework );

    void ConstructL();

    TMPXGeneralCategory ContainerCategoryL( const TDesC& aFullFileName );

    /**
    * Remove all dummy file of which format is "pla", and leave the "m3u"
    */
    void RemoveDummyFiles();

private:
    CMmMtpDpMetadataMpxAccess* iMmMtpDpMetadataMpxAccess;
    CMmMtpDpMetadataVideoAccess* iMmMtpDpMetadataVideoAccess;

    MMTPDataProviderFramework& iFramework;
    RFs& iFs;    // should not remove this member data!!!

    TInt iOpenCount;    // introduce to fix CollectionHelper Flush problem

    CDesCArray* iAbstractMediaArray;

    };

#endif // CMMMTPDPMETADATAACCESSWRAPPER_H
