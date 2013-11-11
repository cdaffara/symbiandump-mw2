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
* Description:  Harvester internal database
*
*/


#ifndef CMPXHARVESTERDB_H
#define CMPXHARVESTERDB_H

#include <d32dbms.h>
#include <s32file.h>

// FORWARD DECLARATIONS
class CMPXHarvesterDatabaseTable;

// ENUMS
#ifdef __RAMDISK_PERF_ENABLE
// enum for database state
enum TDbState
    {
    EDbClose,
    EDbOpen,
    EDbInTransaction
    };
#endif // __RAMDISK_PERF_ENABLE

/**
 *  Database class for the Harvester Component
 *
 *  @lib mpxfilehandler.lib
 *  @since S60 3.0
 */
NONSHARABLE_CLASS( CMPXHarvesterDB ) : public CBase
    {

public:

    /**
    * Two-phased constructor
    * @param aDrive drive of the database
    * @param aFs file session
    * @param aEMMC whether the system has an internal drive 
    * @return new instance of CMPXHarvesterDB
    */
    static CMPXHarvesterDB* NewL( TDriveNumber aDrive, RFs& aFs, TBool aEMMC = EFalse );

    /**
    * Virtual destructor
    */
    virtual ~CMPXHarvesterDB();

    /**
    * Open the database
    * @return State of the opened database: KErrNone or KErrCorrupt
    * Leaves if cannot open the database
    */
    TInt OpenL();

    /**
    * Close the database
    */
    void Close();

    /**
    * Gets what drive this db object represents
    * @return TDriveNumber
    */
    TDriveNumber GetDbDrive();

    /**
    * Creates a table to open all files
    * @return CMPXHarvesterDatabaseTable instance, ownership x-fer
    */
    CMPXHarvesterDatabaseTable* OpenAllFilesTableL();

    /**
    * Creates a table to open to a specific directory
    * @return CMPXHarvesterDatabaseTable instance, ownership x-fer
    */
    CMPXHarvesterDatabaseTable* OpenDirectoryL( const TDesC& aDir );

    /**
    * Creates a table to open to a specific file
    * @return CMPXHarvesterDatabaseTable instance, ownership x-fer
    */
    CMPXHarvesterDatabaseTable* OpenFileL( const TDesC& aFile );

    /**
    * Creates a table to open to all files with DRM flag set
    * @return CMPXHarvesterDatabaseTable instance, ownership x-fer
    */
    CMPXHarvesterDatabaseTable* OpenDrmFileL();
    
    /**
    * Delete a file using SQL
    * @param aFile file to remove
    */
    void DeleteFileL( const TDesC& aFile );

    /**
    * Removes ALL files from this db
    */
    void RemoveAllFilesL();

    /**
    * Deletes the physical database file
    * Call Close() before calling delete!
    * If Close() isn't called KErrInUse will be returned
    * @return error for the operation
    */
    TInt DeleteDatabase();

    /**
    * Retrieve the unique ID of this db
    * The unique id is set as the volume id of the drive from
    * which the database was created
    */
    TUint UniqueIdL();

    /**
    * Count ALL files from this db
    * @return count for the operation
    */
    TInt CountAllFilesL();
    
    /**
    * Begin transaction on this db
    */
    void BeginL();

    /**
    * Check if the db is in transaction
    */
    TBool InTransaction();
    
    /**
    * Commit all changes in the transaction on this db
    */
    void CommitL();
    
    /**
    * Rollbacks the current transaction
    */
    void Rollback();
    
#ifdef __RAMDISK_PERF_ENABLE
    /**
     * Set RAM drive info
     */
    void SetRamDriveInfo(TDriveNumber aDrive, TBool aUseRamDrive);
    
    /**
     * Get UseRamDrive
     */
    TBool IsUseRamDrive();
    
    /**
     * Get the state of the database.
     */
    TDbState GetDbState();
    
    /**
     * Set the state of the database.
     */
    void SetDbStateL( TDbState aState );
#endif // __RAMDISK_PERF_ENABLE
    
private: // private functions

    /**
    * Create a database
    */
    void CreateDBL();

    /**
    * Opens a database
    * @return State of the opened database: KErrNone or KErrCorrupt
    * Leaves if cannot open the database
    */
    TInt OpenDBL();

    /**
    * Replace quotes
    * @param aSrc source
    * @param aTrg destination
    */
    void FindAndReplaceSingleQuote(const TDesC& aSrc,
                                   TDes& aTrg);

    /**
     * Generate the database name.
     * @return the database file name.
     */
    TFileName GenerateDbName();
    
private:

    /**
    * Default constructor
    */
    CMPXHarvesterDB( TDriveNumber aDrive, RFs& aFs, TBool aEMMC );

    /**
    * 2nd phase constructor
    */
    void ConstructL();

private: // data
    TDriveNumber      iDrive;
    RFs&              iFs;
    TBool             iEMMC;      //Has system an internal drive
    // Internal database
    CFileStore*       iStore;
    RDbStoreDatabase* iDatabase;  // Local single client db
    TBool             iDBOpen;    // Is the db open and ready
#ifdef __RAMDISK_PERF_ENABLE
    TDriveNumber      iRamDrive;
    TBool             iUseRamDrive;
#endif // __RAMDISK_PERF_ENABLE
    };

#endif // CMPXHARVESTERDB_H
