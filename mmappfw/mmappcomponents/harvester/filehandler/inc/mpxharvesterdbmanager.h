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
* Description:  Harvester database manager
*
*/


#ifndef CMPXHARVESTERDBMANAGER_H
#define CMPXHARVESTERDBMANAGER_H

#include <e32base.h>
#include <d32dbms.h>
#include "mpxharvesterdb.h"

/**
 *  CMPXHarvesterDatabaseManager
 *
 *  Harvester database manager
 *
 *  @lib harvesterfilehandler.lib
 *  @since S60 3.0
 */
NONSHARABLE_CLASS( CMPXHarvesterDatabaseManager ) : public CBase
    {

public:

    /**
    * Two phased constructor
    * @param aFs file session
    * @return CMPXHarvesterDatabasemanager session
    */
    static CMPXHarvesterDatabaseManager* NewL( RFs& aFs );

    /**
    * Virtual destructor
    */
    virtual ~CMPXHarvesterDatabaseManager();

    /**
    * Open All databases
    */
    TInt OpenAllDatabasesL();

    /**
    * Open a particular database
    * @param TDriveNumber aDrive
    */
    void OpenDatabaseL( TDriveNumber aDrive );

    /**
    * Close all databases
    */
    void CloseAllDatabases();
    
    /**
    * Close all databases on mass storage drives
    */
    void CloseMassStorageDatabases();

    /**
    * Close a particular DB
    * @param TDriveNumber the Drive
    */
    void CloseDatabase( TDriveNumber aDrive );
    

    /**
    * Get a particular database
    * @param TDriveNumber the Drive
    */
    CMPXHarvesterDB& GetDatabaseL( TDriveNumber aDrive );
	
	/**
    * Abruptly close a particular DB without trying to access it
    * To be used when drive is no more accessible
    * @param TDriveNumber the Drive
    */
    void DropDatabase( TDriveNumber aDrive );
    
    /**
     * Return whether database is open on the specified drive
     */
    TBool DatabaseIsOpen( TDriveNumber aDrive );


    /**
    * Return the number of databases
    * @return number of databases
    */
    TInt Count();

    /**
    * Get the n'th database
    * @param aDb index to the database
    * @return CMPXHarvesterDB reference
    */
    CMPXHarvesterDB& GetDatabaseL( TInt aDb );

    /**
    * Recreate all databases
    * Deletes old ones and re-creates
    */
    void RecreateDatabases();
    
    /**
    * Begin transaction on all databases
    */
    void BeginL();
    
    /**
    * Commit transaction on all databases
    */
    void CommitL();
    
    /**
    * Rollbacks the current transaction on all databases
    */
    void Rollback();
    
    /**
     * Checks if the spefified drive is a remove drive
     */      
    TBool IsRemoteDrive(TDriveNumber aDrive);

#ifdef __RAMDISK_PERF_ENABLE
    /**
    * Copy all databases from RAM disk back to normal drive, E, F,...
    * 
    * @return none
    * Leaves if DB is not usable after the operation
    */
    void CopyDBsFromRamL(); 

    /**
    * Copy all databases to RAM disk from normal drive, E, F,...
    * 
    * @return none
    * Leaves if DB is not usable after the operation
    */
    void CopyDBsToRamL(TBool aMtpMode = EFalse);

    /**
     * Update all databases from RAM drive
     */
    //void UpdateDBsFromRamL( TInt aCount );

    /**
     * If Ram disk is low, copy dbs from ram.
     */
    void EnsureRamSpaceL();
#endif //__RAMDISK_PERF_ENABLE

private: // new functions
	
	 /**
	  * Find database index in the internal table
	  * Return KErrNotFound if not found
	  */
   TInt FindDatabaseIndex ( TDriveNumber aDrive );
   
	 /**
	  * Return whether drive exists and is local
	  */
   TBool IsLocalDrive( TDriveNumber aDrive );

    
#ifdef __RAMDISK_PERF_ENABLE
    /**
    * Find available RAMDISK
    */
    TInt GetRAMDiskPath();

    /**
    * Check if disk is available to copy.
    *
    * @return ETrue if there is enough space, EFalse otherwise
    */
    TBool IsRamDiskSpaceAvailable();
    
    /**
     * Copy database to ram drive
     */
    void DoCopyDBToRamL(TDriveUnit aDriveUnit);
    
    /**
    * Copy database from RAM disk
    */
    TInt DoCopyDBFromRam(TDriveUnit aDriveUnit); 

    /**
    * To block a disk space so that it can gurantee for a write back from RAM disk
    *
    * @return KErrNone if the dummy file is created successfully
    */
    TInt BlockDiskSpaceL( TDriveUnit aDrive, TInt aOrigDbSize );

    /**
    * To calculate necessary file size of the dummy file
    *
    * @return TInt64 estimated file size
    */
    TInt64 CalculateInitalDummyDBSizeL( TVolumeInfo& aVol, TInt aOrigDbSize );

    /**
    * Remove dummy file
    *
    * @param aDrive Drive
    */
    void RemoveDummyFile( TDriveNumber aDrive );
    
    /**
     * Update the database from ram drive.
     */
    //void DoUpdateDBFromRamL( TDriveUnit aDriveUnit );
    
    /**
     * Generate the harvester db path and name.
     */
    TFileName GenerateHarvesterDbName( TDriveUnit aDriveUnit, TBool aForRamDrive = EFalse );
    
    /**
     * Generate the dummy db path and name.
     */
    TFileName GenerateDummyDbName( TDriveUnit aDriveUnit );

    /**
     * Check if Ram disk is low.
     */
    TBool IsRamDiskLow();
    
    /**
     * Sum up the total size in bytes of the databases.
     * 
     * @param aSize - On return, the total size of the databases.
     * @param aRamDrive - if True, will sum up dbs on Ram Drive, if False, will sum up dbs on other drives.
     * @return TInt System error.
     */
    TInt GetTotalDatabasesSize(TInt& aSize, TBool aRamDrive);

#endif  // __RAMDISK_PERF_ENABLE

private:

    /**
    * Private constructor
    */
    CMPXHarvesterDatabaseManager( RFs& aFs );

    /**
    * Second phase constructor
    */
    void ConstructL();

private: // data
    RDbs                             iDBSession;
    RPointerArray<CMPXHarvesterDB>   iDatabases;

    RFs&                             iFs;   // Not Owned
    TBool                            iEMMC; //Has system an internal drive
#ifdef __RAMDISK_PERF_ENABLE
    // Defined for RAM disk performance
    TBool                   iRAMDiskPerfEnabled;  // flag to indicate RAM disk feature is enabled from cenrep.
    TUint64                 iMaximumAllowedRAMDiskSpaceToCopy; // max size in megabytes allowed for RAM copying.
    TFileName               iRAMFolder;
    TDriveNumber            iRAMDrive;
    //TInt                    iUpdateCount;
    TBool                   iMtpMode;
    //TInt                    iMtpAddCount;
#endif //__RAMDISK_PERF_ENABLE
    };


#endif // CMPXHARVESTERDBMANAGER_H
