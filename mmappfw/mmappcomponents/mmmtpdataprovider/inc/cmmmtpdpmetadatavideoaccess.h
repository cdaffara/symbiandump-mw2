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
* Description:  Meta data video access
*
*/


#ifndef CMMMTPDPMETADATAVIDEOACCESS_H
#define CMMMTPDPMETADATAVIDEOACCESS_H

#include <e32base.h>
#include <f32file.h>
#include <s32file.h>    // for CPermanentFileStore
#include <d32dbms.h>

/*
* Mapping property code to db column name
*/
typedef struct
    {
    TMTPObjectPropertyCode iPropCode;
    const TDesC& iDbColumnName;
    } TColumnNameTable;

class CMmMtpDpMetadataVideoAccess : public CBase
    {
public:
    static CMmMtpDpMetadataVideoAccess* NewL( RFs& aRfs );

    /**
    * Destructor
    */
    ~CMmMtpDpMetadataVideoAccess();

    /*
    * Called when the MTP session is initialised
    */
    void OpenSessionL();

    /*
    * Cleanup db and notify CFL
    */
    void CloseSessionL();

    /*
    * Set root for db to manipulate relevant ".db" file
    */
    void SetStorageRootL( const TDesC& aStorageRoot );

    /**
    * Adds Video info to Video DB
    * @param aFullFileName, full file name of Media file
    */
    void AddVideoL( const TDesC& aFullFileName );

    /*
    * Gets a piece of metadata from permanent store
    * @aPropCode
    * @aNewData, output param, store the value get from db
    * @aObjectMetaData, source to get properties
    */
    void GetObjectMetadataValueL( const TUint16 aPropCode,
        MMTPType& aNewData,
        const CMTPObjectMetaData& aObjectMetaData );

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
            const CMTPObjectMetaData& aObjectMetaData );

    /*
    * Rename the record acording to full file name
    * @aOldFileName, specify the object which need to be changed
    * @aNewFileName, specify the new name which will be changed into
    */
    void RenameRecordL( const TDesC& aOldFileName, const TDesC& aNewFileName );

    /**
    * Remove the file from DB
    * @param aFullFileName, the file to remove
    */
    void DeleteRecordL( const TDesC& aFullFileName );

    /**
    * Remove all records from DB
    */
    void DeleteAllRecordsL();    // Removes all metadata from the database

    /**
    * set image specific properties specific to videos
    * @param aFullFileName, the fullFileName of object
    * @param aWidth,  the width of an object in pixels to set
    * @parem aHeight,  the height of an object in pixels to set
    */
    void SetImageObjPropL( const TDesC& aFullFileName,
        const TUint32 aWidth,
        const TUint32 aHeight );

    /**
    * get image specific properties specific to videos
    * @param aFullFileName, the fullFileName of object
    * @param aWidth,  the width of an object in pixels to get
    * @parem aHeight,  the height of an object in pixels to get
    */
    void GetImageObjPropL( const TDesC& aFullFileName,
            TUint32& aWidth,
            TUint32& aHeight );

    /**
    * Cleanup Database
    */
    void CleanupDatabaseL();

private:

    enum TMtpDbState
        {
        ENoRecord,
        ERecordRead,
        ERecordWrite,
        EFailedWrite
        };

    CMmMtpDpMetadataVideoAccess( RFs& aRfs );
    void ConstructL();

    TInt OpenDatabase();
    void OpenDatabaseL();
    void CreateDatabaseTablesL();
    void CompactDbIfNecessaryL();
    void CleanupDbIfNecessaryL();

    void IdentifyDeletedFilesL();

    HBufC* ReadLongTextL( const TDesC& aColumn);
    void WriteLongTextL( const TDesC& aColumn, const TDesC& aValue);

    void ExecuteQueryL( const TDesC& aSelectThese,
    const TDesC& aFromTable,
    const TDesC& aColumnToMatch = KNullDesC,
    const TDesC& aMatchCriteria = KNullDesC,
    const TBool aIfNot = EFalse,
    const TBool aNeedQuote = ETrue );

    void SetRecordL( const TDesC& aFullFileName, TMtpDbState aState ); // Updates the iRecord to the selected file
    void SetMetadataL( const TUint16 aPropCode, const MMTPType& aNewData );

    inline TBool IsDatabaseOpened()
        {
        return iDbOpened;
        }

    const TDesC& ColumnNameFromPropCodeL( const TUint16 aPropCode );

    TBool FileExists( const TDesC& aFullFileName );

private: // Data

    RFs iRfs;

    CPermanentFileStore* iFileStore;
    TMtpDbState iDbState;
    TFileName iCurrentFileName;

    RDbs iDbsSession;
    RDbView iRecordSet;
    HBufC* iQueryText;
    CDbColSet* iColSet;
    RDbStoreDatabase iDatabase;
    TBool iDbOpened;
    TInt iRecordCount;  // Used to compact the database every KMtpCompactInterval
    TInt iStoreNum;

    };

#endif // CMMMTPDPMETADATAVIDEOACCESS_H
