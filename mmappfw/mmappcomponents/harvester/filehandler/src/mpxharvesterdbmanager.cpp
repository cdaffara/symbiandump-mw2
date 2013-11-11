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


#include <e32base.h>
#include <pathinfo.h>
#include <driveinfo.h>

#ifdef __RAMDISK_PERF_ENABLE
#include <centralrepository.h>
#include <bautils.h>  
#include <mpxharvestercrkeys.h>
#include "mpxdbcommon.h"
#endif //__RAMDISK_PERF_ENABLE

#include <mpxlog.h>
#include "mpxharvesterdbmanager.h"
#include "mpxharvesterdb.h"
#include "mpxharvesterdbtable.h"

// CONSTANTS
#ifdef __RAMDISK_PERF_ENABLE
_LIT( KHarvesterDummyDBName, "harvesterDummy.dat" );
const TInt64 KMPMegaByte = 1048576;
const TInt64 KMPEstimatedSongInBytes = KMPMegaByte * 2; 
const TInt KMPEstimatedSizePerDBEntry = KMaxFileName; // worst scenario, can be lower if needed
//const TInt KUpdateDBCount = 200;
const TInt KMPMinimumRAMSizeToRun = 6 * KMPMegaByte;
#endif //__RAMDISK_PERF_ENABLE


// ---------------------------------------------------------------------------
// C++ Constructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterDatabaseManager::CMPXHarvesterDatabaseManager( RFs& aFs ):
        iFs( aFs ),
        iEMMC( EFalse )
#ifdef __RAMDISK_PERF_ENABLE
        ,iRAMDiskPerfEnabled(EFalse),
        iMaximumAllowedRAMDiskSpaceToCopy(0),
        //iUpdateCount(0),
        iMtpMode(EFalse)
        //iMtpAddCount(0)
#endif //__RAMDISK_PERF_ENABLE
    {

    }


// ---------------------------------------------------------------------------
// 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseManager::ConstructL()
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::ConstructL");
    User::LeaveIfError( iDBSession.Connect() );

    //Find out if the system has an internal drive (eMMC)
    TDriveInfo driveInfo;
    if( iFs.Drive( driveInfo, EDriveE ) == KErrNone )
    	{
        if ( driveInfo.iDriveAtt & KDriveAttInternal )
        	iEMMC = ETrue;
    	}
#ifdef __RAMDISK_PERF_ENABLE
    TInt temp;
    CRepository* repository = CRepository::NewLC( KCRUIDMpxHarvesterFeatures );
    MPX_DEBUG1("CMPXHarvesterDatabaseManager::ConstructL got repository");        
    User::LeaveIfError( repository->Get( KMpxHarvesterEnableRamDisk, temp ));
    iRAMDiskPerfEnabled = temp;
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::ConstructL KMpxHarvesterEnableRamDisk %d", iRAMDiskPerfEnabled);        
    
    User::LeaveIfError( repository->Get( KMpxHarvesterMaxAllowedRamDiskSpace, temp) );
    iMaximumAllowedRAMDiskSpaceToCopy = temp * KMPMegaByte;
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::ConstructL KMpxHarvesterMaxAllowedRamDiskSpace %Lu", iMaximumAllowedRAMDiskSpaceToCopy);        
    CleanupStack::PopAndDestroy(repository);
            
    if ( iRAMDiskPerfEnabled )
        {
        MPX_DEBUG1("CMPXHarvesterDatabaseManager::ConstructL RAMDisk performance is enabled.");
        MPX_DEBUG2("CMPXHarvesterDatabaseManager::ConstructL RAMDisk iMaximumAllowedRAMDiskSpaceToCopy=%Lu", iMaximumAllowedRAMDiskSpaceToCopy);
        if ( GetRAMDiskPath() != KErrNone )
            {
            // Error finding ram drive, disable ram disk
            iRAMDiskPerfEnabled = EFalse;
            }
        }
    else
        {
        MPX_DEBUG2("CMPXHarvesterDatabaseManager::ConstructL RAMDisk performance is NOT enabled iRAMDiskPerfEnabled=%d", iRAMDiskPerfEnabled);
        MPX_DEBUG2("CMPXHarvesterDatabaseManager::ConstructL RAMDisk iMaximumAllowedRAMDiskSpaceToCopy=%Lu", iMaximumAllowedRAMDiskSpaceToCopy);
        }
#endif //__RAMDISK_PERF_ENABLE
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterDatabaseManager* CMPXHarvesterDatabaseManager::NewL( RFs& aFs )
    {
    CMPXHarvesterDatabaseManager* self =
                             new( ELeave ) CMPXHarvesterDatabaseManager( aFs );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterDatabaseManager::~CMPXHarvesterDatabaseManager()
    {
#ifdef __RAMDISK_PERF_ENABLE
    TInt count(iDatabases.Count());
    for (TInt i = 0; i < count; ++i)
        {
        RemoveDummyFile(iDatabases[i]->GetDbDrive());
        }
#endif // __RAMDISK_PERF_ENABLE
    iDatabases.ResetAndDestroy();
    iDatabases.Close();
    iDBSession.Close();
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::OpenAllDatabasesL
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterDatabaseManager::OpenAllDatabasesL()
    {
    MPX_DEBUG1("CMPXHarvesterDatabaseManager::OpenAllDatabasesL <---");

    TInt rtn (KErrNone);

    // Open drives we are interested in.
    //
    TDriveList driveList;
    TInt driveCount(0);
    User::LeaveIfError( DriveInfo::GetUserVisibleDrives(
           iFs, driveList, driveCount ) );

    for( TInt driveNum = EDriveA; driveNum <= EDriveZ; driveNum++ )
        {
        if (driveList[driveNum] && IsLocalDrive(static_cast<TDriveNumber>(driveNum)))
            {
            MPX_DEBUG2("CMPXHarvesterDatabaseManager::OpenAllDatabasesL: opening database in drive %d", driveNum);
            TInt index = FindDatabaseIndex ( (TDriveNumber) driveNum );
            if ( index == KErrNotFound )
                {
                MPX_DEBUG1("CMPXHarvesterDatabaseManager::OpenAllDatabasesL: re-creating database");
                CMPXHarvesterDB* dB = CMPXHarvesterDB::NewL(
                    static_cast<TDriveNumber>(driveNum), iFs, iEMMC );
                CleanupStack::PushL( dB );
                TRAPD(openError, rtn |= dB->OpenL() );  //lint !e665
                if(openError == KErrNone)
                    {
                     iDatabases.AppendL( dB );
                     CleanupStack::Pop( dB );
                    }
                else
                    {
                    MPX_DEBUG2("CMPXHarvesterDatabaseManager::OpenAllDatabasesL: opening failed, error=%d, removing database", openError);
                    CleanupStack::PopAndDestroy( dB );
                    }
                }
            else
                {
                CMPXHarvesterDB* dB = iDatabases[index];
                TRAPD(openError, rtn |= dB->OpenL() );   //lint !e665
                if(openError != KErrNone)
                    {
                    MPX_DEBUG2("CMPXHarvesterDatabaseManager::OpenAllDatabasesL: opening failed, error=%d, removing database", openError);
                    iDatabases.Remove ( index );
                    delete dB;
                    }
                }
            }
        }
    MPX_DEBUG1("CMPXHarvesterDatabaseManager::OpenAllDatabasesL --->");  //lint !e961
    return rtn;
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::OpenDatabaseL
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseManager::OpenDatabaseL( TDriveNumber aDrive )
    {
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::OpenDatabaseL %d <---", aDrive);

    if ( ! IsLocalDrive( aDrive ) )
        {
        MPX_DEBUG1("CMPXHarvesterDatabaseManager::OpenDatabaseL drive not available -->");
        return;
        }

    CMPXHarvesterDB * db = NULL;
    TInt index = FindDatabaseIndex ( aDrive );
    if ( index == KErrNotFound )
        {
        index = iDatabases.Count();
        db = CMPXHarvesterDB::NewL( aDrive, iFs, iEMMC );
        CleanupStack::PushL( db );
        iDatabases.AppendL( db );
        CleanupStack::Pop( db );
        }
    else
        {
        db = iDatabases[index];
        }

    // TRAPD(openError, rtn |= dB->OpenL() );  //lint !e665
    TRAPD( openError, db->OpenL() );
    if( openError != KErrNone )
        {
        MPX_DEBUG2("CMPXHarvesterDatabaseManager::OpenAllDatabasesL: opening failed, error=%d", openError);
        iDatabases.Remove(index);
        delete db;
        }
    MPX_DEBUG1("CMPXHarvesterDatabaseManager::OpenDatabaseL --->");
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::CloseAllDatabases
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseManager::CloseAllDatabases()
    {
    // Close all databases for shutdown
    iDatabases.ResetAndDestroy();
    }


// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::CloseMassStorageDatabases
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseManager::CloseMassStorageDatabases()
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::CloseMassStorageDatabases");
    for (TInt i = 0; i < iDatabases.Count();)
        {
        CMPXHarvesterDB * db =iDatabases [i];
        TDriveNumber drive = db->GetDbDrive();
        if ( drive != EDriveC )
            {
            MPX_DEBUG2("CMPXHarvesterDatabaseManager::CloseMassStorageDatabases closing DB on drive %d", drive);
        	  db->Close();
#ifdef __RAMDISK_PERF_ENABLE                
            if( iRAMDiskPerfEnabled && db->IsUseRamDrive() )
                {
                MPX_DEBUG1("CMPXHarvesterDatabaseManager::CloseDatabase DB is on RAM");
                db->SetRamDriveInfo( iRAMDrive, EFalse ); 
                TInt err = DoCopyDBFromRam (drive);
                if ( err )
                    {
                    MPX_DEBUG2("CMPXHarvesterDatabaseManager::CloseDatabase DB copy error=%d", err);                
                    RemoveDummyFile( drive );
                    }
                }
#endif
            delete db;
            iDatabases.Remove(i);
            }
        else
            {
            ++i;
            }
        }
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::CloseDatabase
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseManager::CloseDatabase( TDriveNumber aDrive )
    {
    MPX_DEBUG2("-->CMPXHarvesterDatabaseManager::CloseDatabase drive %d", aDrive );
    TInt index = FindDatabaseIndex( aDrive );
    if ( index != KErrNotFound )
        {
        CMPXHarvesterDB * db =iDatabases[index];
        db->Close();
#ifdef __RAMDISK_PERF_ENABLE                
        if( iRAMDiskPerfEnabled && db->IsUseRamDrive() )
            {
            MPX_DEBUG1("CMPXHarvesterDatabaseManager::CloseDatabase DB is on RAM");
            db->SetRamDriveInfo( iRAMDrive, EFalse ); 
            TInt err = DoCopyDBFromRam(aDrive);
            if ( err )
                {
                MPX_DEBUG2("CMPXHarvesterDatabaseManager::CloseDatabase DB copy error=%d", err);                
                RemoveDummyFile( aDrive );
                }
            }
#endif
        delete db;
        iDatabases.Remove(index);
        }
    MPX_DEBUG1("<--CMPXHarvesterDatabaseManager::CloseDatabase");
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::DropDatabase
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseManager::DropDatabase( TDriveNumber aDrive )
    {
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::DropDatabase drive %d <---", aDrive );
    TInt index = FindDatabaseIndex( aDrive );
    if ( index != KErrNotFound )
        {
        CMPXHarvesterDB * db =iDatabases[index];
#ifdef __RAMDISK_PERF_ENABLE                
        if( iRAMDiskPerfEnabled && db->IsUseRamDrive() )
            {
            MPX_DEBUG1("CMPXHarvesterDatabaseManager::DropDatabase DB is on RAM");
            db->Close();
            // delete db on ram drive.
            TFileName src = GenerateHarvesterDbName( TDriveUnit(aDrive), ETrue );
            BaflUtils::DeleteFile(iFs, src);
            }
#endif
        delete db;
        iDatabases.Remove(index);
        }
    MPX_DEBUG1("CMPXHarvesterDatabaseManager::DropDatabase --->");
    }
// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::FindDatabaseIndex
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterDatabaseManager::FindDatabaseIndex ( TDriveNumber aDrive )
    {
    TInt count = iDatabases.Count();
    for( TInt i=0; i<count; ++i )
        {
        CMPXHarvesterDB* db = (CMPXHarvesterDB*) iDatabases[i];
        if( db->GetDbDrive() == aDrive )
            {
            MPX_DEBUG3("CMPXHarvesterDatabaseManager::FindDatabaseIndex drive=%d returns index %d ", aDrive, i);
            return i;
            }
        }
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::FindDatabaseIndex drive=%d returns KErrNotFound", aDrive);
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::DataaseIsOpen
// ---------------------------------------------------------------------------
//
TBool CMPXHarvesterDatabaseManager::DatabaseIsOpen( TDriveNumber aDrive )
    {
    return FindDatabaseIndex( aDrive ) != KErrNotFound;
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::GetDatabase
// ---------------------------------------------------------------------------
//
CMPXHarvesterDB& CMPXHarvesterDatabaseManager::GetDatabaseL( TDriveNumber aDrive )
    {
    TInt index = FindDatabaseIndex( aDrive );
    User::LeaveIfError (index); // Not found, so we leave
    return *iDatabases[index];
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::Count
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterDatabaseManager::Count()
    {
    return iDatabases.Count();
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::GetDatabaseL
// ---------------------------------------------------------------------------
//
CMPXHarvesterDB& CMPXHarvesterDatabaseManager::GetDatabaseL( TInt aDb )
    {
    if( aDb > iDatabases.Count() )
        {
        User::Leave(KErrNotFound/*KErrArgument*/);
        }
    return *iDatabases[aDb];
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::RecreateDatabase
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseManager::RecreateDatabases()
    {
    MPX_DEBUG1("CMPXHarvesterDatabaseManager::RecreateDatabases <--");
    for( TInt i=0; i < iDatabases.Count(); )
        {
        // Close db, delete and recreate
        //
        MPX_DEBUG2("RecreateDatabasesL() -- %i", i);
        CMPXHarvesterDB* cur = iDatabases[i];
        cur->Close();
        cur->DeleteDatabase();
        // trap leave just in case 1 db had err
        //
        TRAPD( openError, cur->OpenL() );
        if( openError != KErrNone )
            {
            MPX_DEBUG2("CMPXHarvesterDatabaseManager::RecreateDatabases: opening failed, error=%d", openError);
            iDatabases.Remove(i);
            delete cur;
            }
        else 
            {
            ++i;
            }
        }
    }


// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::IsRemoteDrive
// ---------------------------------------------------------------------------
//
TBool CMPXHarvesterDatabaseManager::IsRemoteDrive(TDriveNumber aDrive)
    {
    TDriveInfo driveInfo;
    TBool isRemoteDrive(EFalse);
    if (iFs.Drive(driveInfo, aDrive) == KErrNone)
        {
        isRemoteDrive = driveInfo.iDriveAtt & KDriveAttRemote;
        }
    return isRemoteDrive;
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::IsLocalDrive
// ---------------------------------------------------------------------------
//
TBool CMPXHarvesterDatabaseManager::IsLocalDrive( TDriveNumber aDrive )
    {
    TDriveInfo driveInfo;
    return (iFs.Drive ( driveInfo, aDrive) == KErrNone )
           && driveInfo.iType != EMediaNotPresent 
           && ! (driveInfo.iDriveAtt & KDriveAttRemote);
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::BeginL
// ---------------------------------------------------------------------------
// 
void CMPXHarvesterDatabaseManager::BeginL()
    {
    MPX_DEBUG1("CMPXHarvesterDatabaseManager::BeginL <--");
    TInt count( iDatabases.Count() );
    for( TInt i=0; i<count; ++i )
        {
        CMPXHarvesterDB* db = (CMPXHarvesterDB*) iDatabases[i];
        if(!db->InTransaction())
             {
        TRAPD(err, db->BeginL());
        User::LeaveIfError(err);
             }
        }
    MPX_DEBUG1("CMPXHarvesterDatabaseManager::BeginL -->");
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::CommitL
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseManager::CommitL()
    {
    MPX_DEBUG1("CMPXHarvesterDatabaseManager::CommitL <--");
    TInt count( iDatabases.Count() );
    for( TInt i=0; i<count; ++i )
        {
        CMPXHarvesterDB* db = (CMPXHarvesterDB*) iDatabases[i];
        
        // commit only on databases that are in transaction
        if(db->InTransaction())
            {
            TRAPD(err, db->CommitL());
            User::LeaveIfError(err);
            }
        }
    MPX_DEBUG1("CMPXHarvesterDatabaseManager::CommitL -->");
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::Rollback
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseManager::Rollback()
    {
    MPX_DEBUG1("CMPXHarvesterDatabaseManager::Rollback <--");
    TInt count( iDatabases.Count() );
    for( TInt i=0; i<count; ++i )
        {
        CMPXHarvesterDB* db = (CMPXHarvesterDB*) iDatabases[i];
        
        // rollback only on databases that are in transaction
        if(db->InTransaction())
            {
            db->Rollback();
            }
        }      
    MPX_DEBUG1("CMPXHarvesterDatabaseManager::Rollback -->");
    }

#ifdef __RAMDISK_PERF_ENABLE
// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::CopyDBsToRamL
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseManager::CopyDBsToRamL( TBool aMtpMode )
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::CopyDBsToRamL");
    iMtpMode = aMtpMode;
    //iMtpAddCount = 0;
    //iUpdateCount = 0;
    
    if( iRAMDiskPerfEnabled )
        {
        // Check for low ram disk space.
        if ( !IsRamDiskSpaceAvailable() || IsRamDiskLow() )
            {
            return;
            }
        
        // Check if we are over the allowed ram space.
        TInt dbSize=0;
        TInt err = GetTotalDatabasesSize(dbSize, EFalse);
        if ( err || (dbSize > iMaximumAllowedRAMDiskSpaceToCopy) )
            {
            MPX_DEBUG2("CMPXDbManager::CopyDBsToRamL Over the allowed Ram disk limit %Lu", iMaximumAllowedRAMDiskSpaceToCopy );
            return;
            }
    
        TInt count(iDatabases.Count());
        MPX_DEBUG2("CMPXHarvesterDatabaseManager::CopyDBsToRamL() iDatabaseHandles.Count()=%d", count);
        for (TInt i = 0; i < count; ++i)
            {
            TDriveUnit drive( iDatabases[i]->GetDbDrive() );
            // Don't copy db on C drive.
            if ( (TInt)drive == EDriveC )
                {
                MPX_DEBUG1("CMPXHarvesterDatabaseManager::CopyDBsToRamL() Drive C: skipped.");
                continue;
                }
            
            // Check if database is in RAM drive.
            if ( iDatabases[i]->IsUseRamDrive())
                {
                MPX_DEBUG2("CMPXHarvesterDatabaseManager::CopyDBsToRamL Drive %d is already in Ram Drive.", (TInt)drive);
                continue;
                }
            
            // Get the db state in order to restore it later.
            TDbState dbState = iDatabases[i]->GetDbState();
            iDatabases[i]->SetDbStateL( EDbClose );
            TRAPD(err, DoCopyDBToRamL( drive ));
            if ( err != KErrNone )
                {
                MPX_DEBUG2("CMPXHarvesterDatabaseManager::CopyDBsToRamL error=%d", err);
                // delete dummy file
                RemoveDummyFile( (TDriveNumber)(TInt)drive );
                
                // delete db in ram drive
                TFileName ramDb = GenerateHarvesterDbName( drive, ETrue );
                TInt delErr = BaflUtils::DeleteFile(iFs, ramDb);
                MPX_DEBUG3("CMPXHarvesterDatabaseManager::CopyDBsToRamL db on ram drive deleted file=%S, err=%d", &ramDb, delErr);
                
                iDatabases[i]->SetRamDriveInfo( iRAMDrive, EFalse);
                }
            // Restore the db state.
            iDatabases[i]->SetDbStateL( dbState );
            }
        }
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::DoCopyDBToRamL
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseManager::DoCopyDBToRamL(TDriveUnit aDriveUnit)
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::DoCopyDBToRamL");
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::DoCopyDBToRamL Database from drive %d", (TInt)aDriveUnit );
    TFileName dst;
    TFileName src;
    dst.Zero();
    dst.Append(iRAMFolder);
    BaflUtils::EnsurePathExistsL( iFs, dst );
    src = GenerateHarvesterDbName( aDriveUnit );
    MPX_DEBUG2("RAMDisk src path=%S", &src);
    dst = GenerateHarvesterDbName( aDriveUnit, ETrue );
    MPX_DEBUG2("RAMDisk dst path=%S", &dst);
    TEntry entry;
    iFs.Entry( src, entry );
    User::LeaveIfError( BlockDiskSpaceL( aDriveUnit, entry.iSize ));
    User::LeaveIfError( BaflUtils::CopyFile(iFs, src, dst ) );
    MPX_DEBUG1("CMPXHarvesterDatabaseManager::DoCopyDBToRamL Copy to Ram Drive Successful.");
    GetDatabaseL( (TDriveNumber) (TInt)aDriveUnit ).SetRamDriveInfo(iRAMDrive, ETrue);
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::CopyDBsFromRamL
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseManager::CopyDBsFromRamL()
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::CopyDBsFromRamL");
    // Stop monitoring ram drive for low disk space.
    iMtpMode = EFalse;
    //iMtpAddCount = 0;
    //iUpdateCount = 0;

    if( iRAMDiskPerfEnabled )
       {
        // Should not leave until all the databases have been copied from RAM drive. 
        TInt leaveError = KErrNone;
        TInt count(iDatabases.Count());
        for (TInt i = 0; i < count; ++i)
            {
            TInt err = KErrNone;
            TDriveUnit drive( iDatabases[i]->GetDbDrive() );
            // Check if database is in RAM drive.
            if ( !iDatabases[i]->IsUseRamDrive())
                {
                MPX_DEBUG2("CMPXHarvesterDatabaseManager::CopyDBsFromRamL Drive %d is NOT in Ram Drive.", (TInt)drive);
                continue;
                }

            // Set the UseRamDrive bool to False.
            iDatabases[i]->SetRamDriveInfo( iRAMDrive, EFalse );
            // Get the db state in order to restore it later.
            TDbState dbState = iDatabases[i]->GetDbState();
            TRAP( err, iDatabases[i]->SetDbStateL(EDbClose) );
            if ( err == KErrNone )
                {
                err = DoCopyDBFromRam(drive);
                }
            else
                {
                // Error closing db on RAM drive, try to delete it.
                TFileName ramDB;
                ramDB = GenerateHarvesterDbName( drive, ETrue );
                BaflUtils::DeleteFile(iFs, ramDB);
                }
            
            if ( err )
                {
                MPX_DEBUG2("CMPXHarvesterDatabaseManager::CopyDBsFromRamL copy error=%d", err);                
                //anyting wrong, delete the temp file.
                RemoveDummyFile( (TDriveNumber)(TInt)drive );
                }

            // Restore the db state.
            TRAPD( error, iDatabases[i]->SetDbStateL( dbState ) );
            if ( error && !leaveError )
                {
                leaveError = error;
                }
            }
        User::LeaveIfError( leaveError );
        }
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::DoCopyDBFromRam
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterDatabaseManager::DoCopyDBFromRam(TDriveUnit aDriveUnit)
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::DoCopyDBFromRam");
    TFileName dst;
    TFileName src;
    TInt err = KErrNone;
    
    dst = GenerateHarvesterDbName( aDriveUnit );
    src = GenerateHarvesterDbName( aDriveUnit, ETrue );
    MPX_DEBUG3("CMPXHarvesterDatabaseManager::DoCopyDBFromRam from %S to %S", &src, &dst );

    TFileName dummyDbFileName = GenerateDummyDbName( aDriveUnit ); 

    //Copy Db from RAM to replace dummy file
    err = BaflUtils::CopyFile(iFs, src, dummyDbFileName);
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::DoCopyDBFromRam database copied from ram drive err=%d.", err);
    
    // delete db on ram drive.
    TInt delErr = BaflUtils::DeleteFile(iFs, src);
    MPX_DEBUG3("CMPXHarvesterDatabaseManager::DoCopyDBFromRam db on ram drive deleted file=%S, err=%d", &src, delErr);

    // Make sure we del db from ram drive before returning.
    if (err != KErrNone)
        {
        return err;
        }
    
    err = iFs.Replace(dummyDbFileName, dst);
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::DoCopyDBFromRam dummy file replaced, err=%d.", err);
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::GetRAMDiskPath
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterDatabaseManager::GetRAMDiskPath()
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::GetRAMDiskPath");
    TDriveList driveList;
    TBool ramDriveFound = EFalse;
    iRAMFolder.Zero();
    
    TInt error = iFs.DriveList( driveList );
    if ( error == KErrNone )
        {
        for ( TInt i = 0; i < driveList.Length(); i++ )
            {
            if ( driveList[i] != 0 )
                {
                TDriveInfo info;
                TInt err = iFs.Drive( info, i ); 
                if ( !err && info.iType == EMediaRam )
                    {
                    TDriveUnit driveUnit( i );
                    iRAMDrive = (TDriveNumber)i;
                    iRAMFolder.Append(driveUnit.Name());
                    iRAMFolder.Append(KHarvesterDBPath);
                    ramDriveFound = ETrue;
                    MPX_DEBUG2("RAMDisk path=%S", &iRAMFolder);
                    break;
                    }
                }
            }
        
        // Check if ram drive is found.
        if ( !ramDriveFound )
            {
            error = KErrNotFound;
            }
        }
    MPX_DEBUG2("CMPXDbManager::GetRAMDiskPath Get DriveList error=%d", error);
    return error;
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::IsRamDiskSpaceAvailable
// ---------------------------------------------------------------------------
//
TBool CMPXHarvesterDatabaseManager::IsRamDiskSpaceAvailable()
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::IsRamDiskSpaceAvailable");
    TVolumeInfo vol;
    TInt err = iFs.Volume( vol, iRAMDrive );
    if ( err != KErrNone )
        {
        MPX_DEBUG2("CMPXHarvesterDatabaseManager::IsRamDiskSpaceAvailable Disk Not available to use. Error = %d", err);
        return EFalse;
        }
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::IsRamDiskSpaceAvailable Free in bytes =%Lu", vol.iFree);
    if ( vol.iFree > KMPMinimumRAMSizeToRun )
        {
        MPX_DEBUG1("CMPXHarvesterDatabaseManager::IsRamDiskSpaceAvailable Ok to copy");
        return ETrue;
        }
        
    MPX_DEBUG1("CMPXHarvesterDatabaseManager::IsRamDiskSpaceAvailable NOT Ok to copy");
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::BlockDiskSpaceL
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterDatabaseManager::BlockDiskSpaceL( TDriveUnit aDrive, TInt aOrigDbSize )
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::BlockDiskSpaceL");
    
    // if current DB size can not fit in RAM, abort now
    TVolumeInfo vol;
    TInt err = KErrNone;
    err = iFs.Volume( vol, iRAMDrive );
    if ( err )
        {
        return err;
        }
    
    if ( vol.iFree < aOrigDbSize + KMPMinimumRAMSizeToRun )
        {
        MPX_DEBUG1("CMPXHarvesterDatabaseManager::BlockDiskSpaceL Not enough space on ram drive." );
        return KErrDiskFull;
        }

    MPX_DEBUG2("CMPXHarvesterDatabaseManager::BlockDiskSpaceL %d", (TInt)aDrive );
    err = iFs.Volume( vol, (TInt)aDrive );
    if ( err )
        {
        return err;
        }
    
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::BlockDiskSpaceL Disk total free space in bytes =%Lu", vol.iFree);
    TInt64 blockingSize( CalculateInitalDummyDBSizeL( vol, aOrigDbSize ));
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::BlockDiskSpaceL Disk blocking size =%Lu", blockingSize);
    // Pad blockingSize by 1Mb so we don't use all the free disk space.
    if ( vol.iFree > (blockingSize + 1*KMPMegaByte) )
        {
        TFileName dummyDbFileName = GenerateDummyDbName( aDrive ); 
        MPX_DEBUG2("CMPXHarvesterDatabaseManager::BlockDiskSpaceL  Dummy db file name=%S", &dummyDbFileName);
        
        RFile dummyDb;
        err = dummyDb.Replace( iFs, dummyDbFileName, EFileWrite );
        if (err != KErrNone) 
            {
            MPX_DEBUG2("CMPXDbManager::BlockDiskSpaceL Can't open dummy file %d", err);
            return err;
            }
        
        err = dummyDb.SetSize( blockingSize );
        // Close the dummy RFile
        dummyDb.Close();
        if ( err )
            {
            MPX_DEBUG2("CMPXDbManager::BlockDiskSpaceL Can't resize dummy file %d", err);
            return err;
            }
        }
    else
        {
        MPX_DEBUG1("CMPXHarvesterDatabaseManager::BlockDiskSpaceL - Not enough disk space for dummy file.");
        err = KErrDiskFull;
        }
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::CalculateInitalDummyDBSizeL
// ---------------------------------------------------------------------------
//
TInt64 CMPXHarvesterDatabaseManager::CalculateInitalDummyDBSizeL( TVolumeInfo& aVol, TInt aOrigDbSize )
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::CalculateInitalDummyDBSizeL");
    TInt64 size;
    
    if ( iMtpMode )
        {
        MPX_DEBUG2("CMPXHarvesterDatabaseManager::CalculateInitalDummyDBSize aVol.iFree=%Lu", aVol.iFree );
        TInt64 totalNumOfSongsCanFit = aVol.iFree / KMPEstimatedSongInBytes;
        MPX_DEBUG2("CMPXHarvesterDatabaseManager::CalculateInitalDummyDBSize (MTP) totalNumOfSongsCanFit=%Lu", totalNumOfSongsCanFit );
        TInt64 estimatedSize = totalNumOfSongsCanFit * (TInt64) KMPEstimatedSizePerDBEntry + aOrigDbSize;
        MPX_DEBUG2("CMPXHarvesterDatabaseManager::CalculateInitalDummyDBSize (MTP) estimated DB size from calculation=%Lu", estimatedSize );
        if ( estimatedSize > iMaximumAllowedRAMDiskSpaceToCopy )
            {
            size = iMaximumAllowedRAMDiskSpaceToCopy;
            }
        else
            {
            size = estimatedSize;
            }
        }
    else
        {
        TInt64 totalNumOfSongsCanFit = aVol.iSize / KMPEstimatedSongInBytes;
        MPX_DEBUG2("CMPXHarvesterDatabaseManager::CalculateInitalDummyDBSize totalNumOfSongsCanFit=%Lu", totalNumOfSongsCanFit );
        TInt64 estimatedSize = totalNumOfSongsCanFit * (TInt64) KMPEstimatedSizePerDBEntry + aOrigDbSize;
        MPX_DEBUG2("CMPXHarvesterDatabaseManager::CalculateInitalDummyDBSize estimated DB size from calculation=%Lu", estimatedSize );
        if ( estimatedSize > iMaximumAllowedRAMDiskSpaceToCopy )
            {
            size = iMaximumAllowedRAMDiskSpaceToCopy;
            }
        else
            {
            size = estimatedSize;
            }
        }

    MPX_DEBUG2("CMPXHarvesterDatabaseManager::CalculateInitalDummyDBSize Dummy DB size=%Lu", size );
    return size;
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::RemoveDummyFile
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseManager::RemoveDummyFile( TDriveNumber aDrive )
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::RemoveDummyFile");
    
    TFileName file = GenerateDummyDbName(TDriveUnit(aDrive));
    
    if ( (file.Length() > 0) &&
         (BaflUtils::FileExists(iFs, file)) )
        {
        BaflUtils::DeleteFile(iFs, file);
        }
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::UpdateDBsFromRamL
// ---------------------------------------------------------------------------
//
/*void CMPXHarvesterDatabaseManager::UpdateDBsFromRamL( TInt aCount )
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::UpdateDBsFromRamL");
    TBool updateDb = EFalse;
    
    // In MTP mode, aCount is invalid, songs are added one at a time.
    if ( iMtpMode )
        {
        iMtpAddCount++;
        MPX_DEBUG2("CMPXHarvesterDatabaseManager::UpdateDBsFromRamL iMtpAddCount=%d", iMtpAddCount);
        if ( (iMtpAddCount - iUpdateCount) > KUpdateDBCount )
            {
            updateDb = ETrue;
            iUpdateCount = iMtpAddCount;
            }
        }
    else
        {
        MPX_DEBUG2("CMPXHarvesterDatabaseManager::UpdateDBsFromRamL aCount=%d", aCount);
        if ( (aCount - iUpdateCount) > KUpdateDBCount )
            {
            updateDb = ETrue;
            iUpdateCount = aCount;
            }
        }
    
    if ( updateDb )
        {
        CommitL();
        TInt count(iDatabases.Count());
        for (TInt i = 0; i < count; ++i)
            {
            TDriveUnit drive( iDatabases[i]->GetDbDrive() );
            // Check if database is not in RAM drive.
            if ( !iDatabases[i]->IsUseRamDrive() )
                {
                MPX_DEBUG2("CMPXHarvesterDatabaseManager::UpdateDBsFromRamL Drive %d is not in Ram Drive.", (TInt)drive);
                continue;
                }

            iDatabases[i]->Close();
            // update the database from ram drive.
            TRAP_IGNORE( DoUpdateDBFromRamL( drive ) );
            // 
            iDatabases[i]->OpenL();
            }
        BeginL();
        }
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::DoUpdateDBFromRamL
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseManager::DoUpdateDBFromRamL(TDriveUnit aDriveUnit)
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::DoUpdateDBFromRamL");
    
    TFileName dst;
    TFileName src;
    
    dst = GenerateHarvesterDbName( aDriveUnit );
    src = GenerateHarvesterDbName( aDriveUnit, ETrue );
    MPX_DEBUG3("CMPXHarvesterDatabaseManager::DoUpdateDBFromRamL from %S to %S", &src, &dst );

    //Copy Db from RAM to real db file
    TInt err = BaflUtils::CopyFile(iFs, src, dst);
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::DoUpdateDBFromRamL database copied from ram drive err=%d.", err);
    }*/

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::GenerateHarvesterDbName
// ---------------------------------------------------------------------------
//
TFileName CMPXHarvesterDatabaseManager::GenerateHarvesterDbName( TDriveUnit aDriveUnit, TBool aForRamDrive )
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::GenerateHarvesterDbName");
    TFileName name;
    name.Zero();
    
    if ( aForRamDrive )
        {
        name.Append(iRAMFolder);
        name.Append(aDriveUnit.Name()[0]);

        //Use different name for Dbs if the system has an internal drive vs. MMC-only.
        //Since hard-coded drive letters in the Thumbnail URIs
        //So Dbs are not interchangeable between an internal drive system and MMC-only system.
        if ( iEMMC )
            name.Append( KHarvesterDBNameEMMC );
        else
            name.Append( KHarvesterDBName );
        }
    else
        {
        name.Append(aDriveUnit.Name());
        name.Append(KHarvesterDBPath);
        if ( iEMMC )
            name.Append( KHarvesterDBNameEMMC );
        else
            name.Append( KHarvesterDBName );
        }
    
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::GenerateHarvesterDbName name = %S", &name );
    return name;
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::GenerateDummyDbName
// ---------------------------------------------------------------------------
//
TFileName CMPXHarvesterDatabaseManager::GenerateDummyDbName( TDriveUnit aDriveUnit )
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::GenerateDummyDbName");
    TFileName name;
    name.Zero();
    name.Append(aDriveUnit.Name());
    name.Append(KHarvesterDBPath);
    name.Append(KHarvesterDummyDBName);
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::GenerateDummyDbName name = %S", &name );
    return name;
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::EnsureRamSpaceL
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseManager::EnsureRamSpaceL()
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::EnsureRamSpaceL()");
    
    // Ram disk disabled, no need to continue.
    if( !iRAMDiskPerfEnabled )
        {
        MPX_DEBUG1("CMPXHarvesterDatabaseManager::EnsureRamSpaceL RAM Disk Disabled.");
        return;
        }

    // Check if any DBs are in RAM drive.
    TInt count(iDatabases.Count());
    TBool ramDriveInUse = EFalse;
    for (TInt i = 0; i < count; ++i)
        {
        // Check if database is in RAM drive.
        if ( iDatabases[i]->IsUseRamDrive() )
            {
            ramDriveInUse = ETrue;
            break;
            }
        }
    
    // DBs not in ram drive, no need to continue.
    if ( !ramDriveInUse )
        {
        MPX_DEBUG1("CMPXHarvesterDatabaseManager::EnsureRamSpaceL No DBs in RAM drive.");
        return;
        }
        
    // Check if low on Ram disk.
    if ( IsRamDiskLow() )
        {
        // RAM not enough, copy back to normal drive and continue to harvest.
        MPX_DEBUG1("CMPXHarvesterDatabaseManager::EnsureRamSpaceL RAM diskspace is full, copy dbs back.");
        CopyDBsFromRamL();
        }
    else
        {
        TInt size=0;
        TInt err = GetTotalDatabasesSize(size, ETrue);
        if ( err || (size > iMaximumAllowedRAMDiskSpaceToCopy) )
            {
            // Databases using too much RAM space, copy back to normal drive and continue to harvest.
            if ( err )
                {
                MPX_DEBUG2("CMPXHarvesterDatabaseManager::EnsureRamSpaceL Get DBs Size Err = %d, copy dbs back.", err);
                }
            else
                {
                MPX_DEBUG2("CMPXHarvesterDatabaseManager::EnsureRamSpaceL DBs using too much RAM space size = %d, copy dbs back.", size);
                }
            CopyDBsFromRamL();
            }
        }
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::IsRamDiskLow
// ---------------------------------------------------------------------------
//
TBool CMPXHarvesterDatabaseManager::IsRamDiskLow()
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::IsRamDiskLow()");
    TBool low = EFalse;
    TVolumeInfo vol;
    TInt errRAM = iFs.Volume( vol, iRAMDrive );
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::IsRamDiskLow Get vol err: %d", errRAM);
    MPX_DEBUG2("CMPXHarvesterDatabaseManager::IsRamDiskLow Free RAM Disk Space: %Lu", vol.iFree);
    if ( errRAM == KErrNone && vol.iFree < KMPMinimumRAMSizeToRun )
        {
        low = ETrue;
        }
    return low;
    }

// ---------------------------------------------------------------------------
// CMPXHarvesterDatabaseManager::GetTotalDatabasesSize
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterDatabaseManager::GetTotalDatabasesSize(TInt& aSize, TBool aRamDrive)
    {
    MPX_FUNC("CMPXHarvesterDatabaseManager::GetTotalDatabasesSize");
    TInt err = KErrNone;
    TInt size=0;
    TInt count(iDatabases.Count());
    for ( TInt i = 0; i < count ; ++i )
        {
        TDriveUnit drive( iDatabases[i]->GetDbDrive() );
        // Ignore C drive
        if ( (TInt)drive == EDriveC )
            {
            continue;
            }
        // Generate database name.
        TFileName dbFilename = GenerateHarvesterDbName(drive, aRamDrive);
        MPX_DEBUG2("CMPXHarvesterDatabaseManager::GetTotalDatabasesSize - Database name = %S", &dbFilename);
        TEntry entry;
        err = iFs.Entry( dbFilename, entry );
        if (err == KErrNotFound || err == KErrNotReady)
            {
            MPX_DEBUG3("CMPXHarvesterDatabaseManager::GetTotalDatabasesSize - Ignored %S, err = %d", &dbFilename, err);
            err = KErrNone;
            continue;
            }
        if ( err != KErrNone )
            {
            break;
            }
        MPX_DEBUG3("CMPXHarvesterDatabaseManager::GetTotalDatabasesSize - Size of Db %S = %d", &dbFilename, entry.iSize);
        // sum up size
        size += entry.iSize;
        }
    aSize = size;
    MPX_DEBUG3("CMPXHarvesterDatabaseManager::GetTotalDatabasesSize - Total Size of Dbs = %d, err = %d", size, err);
    return err;
    }

#endif // __RAMDISK_PERF_ENABLE

// END OF FILE

