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
* Description:  Database Class to the harvester db
*
*/


#include <e32base.h>
#include <mpxlog.h>
#include <bautils.h>
#include "mpxharvesterdb.h"
#include "mpxdbcommon.h"
#include "mpxharvesterdbtable.h"

// unlikely to have 50 quotes in a string
const TInt KAdditionalStringLength = 50;

// ---------------------------------------------------------------------------
// C++ Constructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterDB::CMPXHarvesterDB( TDriveNumber aDrive, RFs& aFs, TBool aEMMC  ) :
                                  iDrive( aDrive),
                                  iFs( aFs ),
                                  iEMMC( aEMMC )	  
#ifdef __RAMDISK_PERF_ENABLE
                                  ,iRamDrive(aDrive),
                                  iUseRamDrive(EFalse)
#endif // __RAMDISK_PERF_ENABLE
    {
    }


// ---------------------------------------------------------------------------
// 2nd Phase constructor
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDB::ConstructL()
    {
    // Nothing, have to call "OpenL()"
    }

// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterDB* CMPXHarvesterDB::NewL( TDriveNumber aDrive, RFs& aFs, TBool aEMMC )
    {
    CMPXHarvesterDB* self = new( ELeave ) CMPXHarvesterDB( aDrive, aFs, aEMMC );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterDB::~CMPXHarvesterDB()
    {
    Close();
    }

// ---------------------------------------------------------------------------
// Open a database
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterDB::OpenL()
    {
    MPX_DEBUG1("-->CMPXHarvesterDB::OpenL");

    // There is no need to re-open if it was already open
    if( iDBOpen )
        {
        MPX_DEBUG1("<--CMPXHarvesterDB::OpenL rtn=0 (already open)");
        return KErrNone;
        }

    TInt rtn( KErrNone );
    TFileName fileName = GenerateDbName();
    TParsePtr fileParser( fileName );
    TFileName filePath = fileParser.DriveAndPath();
    // Make sure Path exists; allow to leave if can't create the path
    BaflUtils::EnsurePathExistsL(iFs, filePath);

    // Try to open the stream
    TRAPD( err,
        iStore = CPermanentFileStore::OpenL(iFs, fileName ,EFileRead|EFileWrite);
        );

    // DB doesn't exist or error, replace it and recreate DB
    //
    if( err )
        {
        MPX_DEBUG2("CMPXHarvesterDB::OpenL -- New database %i", err);
        iStore = CPermanentFileStore::ReplaceL(iFs, fileName ,EFileRead|EFileWrite);
        iStore->SetTypeL(iStore->Layout());
        CreateDBL();
        // If the open stream error was not found, that is fine
        // because it is a new db, other errors means the stream
        // is corrupted
        //
        rtn = err!=KErrNotFound ? KErrCorrupt : KErrNone;
        }
    else
        {
        MPX_DEBUG1("CMPXHarvesterDB::OpenL -- Opening database" );
        rtn = OpenDBL();
        }
    iDBOpen = ETrue;

    // Check volume Id
    //
    TVolumeInfo volInfo;
    iFs.Volume(volInfo, iDrive);
    TUint volId(volInfo.iUniqueID);

    TUint uniqueId(0);
    TRAPD( idErr, uniqueId = UniqueIdL() );
    if( idErr != KErrNone )
        {
        // Delete the database because this is not readable
        MPX_DEBUG2("CMPXHarvesterDB::OpenL -- Deleting unreadable DB %i", idErr);
        Close();
        User::LeaveIfError(DeleteDatabase());
        rtn = KErrCorrupt;
        }
    else if( volId != uniqueId )
        {
        // Recreate if volId doesn't match
        //
        MPX_DEBUG1("CMPXHarvesterDB::OpenL unmatched unique ID");
        Close();
        User::LeaveIfError(DeleteDatabase());
        rtn = OpenL();
        }
    
    MPX_DEBUG2("<--CMPXHarvesterDB::OpenL rtn=%d", rtn);
    return rtn;
    }

// ---------------------------------------------------------------------------
// Close a database
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDB::Close()
    {
    MPX_DEBUG1("CMPXHarvesterDB::Close <---");

    // Close the database and close the stream
    if( iDatabase )
        {
        iDatabase->Close();
        delete iDatabase;
        iDatabase = NULL;
        }

    delete iStore;
    iStore = NULL;

    iDBOpen = EFalse;
    MPX_DEBUG1("CMPXHarvesterDB::Close --->");
    }

// ---------------------------------------------------------------------------
// Create a new database
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDB::CreateDBL()
    {
    MPX_DEBUG1("CMPXHarvesterDB::CreateDBL <---");

    // remove old databases before creating/replacing new database
    TFileName fileName = GenerateDbName();
    TParsePtr fileParser( fileName );
    TFileName filePath = fileParser.DriveAndPath();
#ifdef __RAMDISK_PERF_ENABLE
    if ( iUseRamDrive )
        {
        TDriveUnit drive( iDrive );
        filePath.Append(drive.Name()[0]);
        filePath.Append(KHarvesterDBPattern);
        }
    else
        {
#endif // __RAMDISK_PERF_ENABLE
        filePath.Append( KHarvesterDBPattern );
#ifdef __RAMDISK_PERF_ENABLE
        }
#endif // __RAMDISK_PERF_ENABLE
    
    CFileMan* fileManager = CFileMan::NewL(iFs);
    TInt ret = fileManager->Delete(filePath); 
    delete fileManager;
    fileManager = NULL;


    //Create the database
    //
    iDatabase = new( ELeave ) RDbStoreDatabase();
    TStreamId id(0);
    id = iDatabase->CreateL( iStore );

    iStore->SetRootL(id);
    iStore->CommitL();

    // Define the columns
    // "Create table files ( PathName, FileName, LastModTime, Col ID )"
    //
    TBuf<KDbMaxTableCreationSQLLength> query;
    query.Append( KStartCreateTable );
    query.Append( KHarvesterMainTable );

    query.Append(KOpenBracket);

    query.Append(KHarPathName);
    query.Append(KHarPathNameType);
    query.Append(KCommaSign);

    query.Append(KHarFileName);
    query.Append(KHarFileNameType);
    query.Append(KCommaSign);

    query.Append(KHarLastModTime);
    query.Append(KHarLastModTimeType);
    query.Append(KCommaSign);

    query.Append(KHarCollectionDB);
    query.Append(KHarColDBIDType);
    query.Append(KCommaSign);

    query.Append(KHarItemDRM);
    query.Append(KHarItemDRMType);

    query.Append(KCloseBracket);

    // Execute the query
    User::LeaveIfError( iDatabase->Execute( query ) );

    // Add an auxiliary table with the volume id
    // This is to make sure the user doesn't copy a db from another phone
    // "Create table aux { volumeid }"
    //
    query = KNullDesC;
    query.Append( KStartCreateTable );
    query.Append( KHarvesterAuxTable);

    query.Append(KOpenBracket);
    query.Append(KAuxVolumeId);
    query.Append(KAuxVolumeIdType);
    query.Append(KCloseBracket);

    User::LeaveIfError( iDatabase->Execute( query ) );

    // Default aux volume id entry
    //
    TVolumeInfo volInfo;
    iFs.Volume(volInfo, iDrive);
    TUint volId(volInfo.iUniqueID);

    query = KNullDesC;
    query.AppendFormat(KAuxVolumeIdQuery, volId);
    User::LeaveIfError( iDatabase->Execute( query ) );
    MPX_DEBUG2("CMPXHarvesterDB::CreateDBL 4a %S", &query);

    MPX_DEBUG1("CMPXHarvesterDB::CreateDBL --->");
    }

// ---------------------------------------------------------------------------
// Open an existing database
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterDB::OpenDBL()
    {
    MPX_DEBUG1("CMPXHarvesterDB::OpenDBL <---");
    TInt rtn( KErrNone );

    // Open the database
    if( !iDBOpen )
        {
        iDatabase = new( ELeave ) RDbStoreDatabase(); //lint !e423

        // Unable to open db, so try to re-create
        //
        TRAPD(err, iDatabase->OpenL( iStore, iStore->Root() ) );
        if( err != KErrNone )
            {
            MPX_DEBUG2("CMPXHarvesterDB::OpenDBL RDbStoreDatabase::OpenL error %d", err);
            delete iDatabase;
            iDatabase = NULL;
            CreateDBL();
            rtn = KErrCorrupt;
            }
        }

    MPX_DEBUG1("CMPXHarvesterDB::OpenDBL --->");
    return rtn;
    }

// ---------------------------------------------------------------------------
// Get the current drive
// ---------------------------------------------------------------------------
//
TDriveNumber CMPXHarvesterDB::GetDbDrive()
    {
    return iDrive;
    }

// ---------------------------------------------------------------------------
// Creates a table to all items
// ---------------------------------------------------------------------------
//
CMPXHarvesterDatabaseTable* CMPXHarvesterDB::OpenAllFilesTableL()
    {
    // Database isn't opened, leave
    if( iDBOpen == EFalse )
        {
        User::Leave(KErrNotReady);
        }

    // Open the table
    CMPXHarvesterDatabaseTable* table =
                               CMPXHarvesterDatabaseTable::NewLC( *iDatabase );
    table->OpenAllFilesTableL();
    CleanupStack::Pop( table );
    return table;
    }

// ---------------------------------------------------------------------------
// Creates a table to a directory
// ---------------------------------------------------------------------------
//
CMPXHarvesterDatabaseTable* CMPXHarvesterDB::OpenDirectoryL( const TDesC& aDir )
    {
    // Database isn't opened, leave
    if( iDBOpen == EFalse )
        {
        User::Leave(KErrNotReady);
        }

    // Open the table
    CMPXHarvesterDatabaseTable* table =
                               CMPXHarvesterDatabaseTable::NewLC( *iDatabase );
    table->OpenDirectoryL( aDir );
    CleanupStack::Pop( table );
    return table;
    }

// ---------------------------------------------------------------------------
// Creates a table to a specific song
// ---------------------------------------------------------------------------
//
CMPXHarvesterDatabaseTable* CMPXHarvesterDB::OpenFileL( const TDesC& aFile )
    {
    // Database isn't opened, leave
    if( iDBOpen == EFalse )
        {
        User::Leave(KErrNotReady);
        }

    // Open the table
    CMPXHarvesterDatabaseTable* table =
                               CMPXHarvesterDatabaseTable::NewLC( *iDatabase );
    table->OpenItemL( aFile );
    CleanupStack::Pop( table );
    return table;
    }

// ---------------------------------------------------------------------------
// Creates a table to open to all files with DRM flag set
// ---------------------------------------------------------------------------
//    
CMPXHarvesterDatabaseTable* CMPXHarvesterDB::OpenDrmFileL()
    {
    // Database isn't opened, leave 
    if( iDBOpen == EFalse )
        {
        User::Leave(KErrNotReady);
        }
    
    // Open the table    
    CMPXHarvesterDatabaseTable* table = 
                               CMPXHarvesterDatabaseTable::NewLC( *iDatabase );
    table->OpenDrmL();                            
    CleanupStack::Pop( table );
    return table;
    }

// ---------------------------------------------------------------------------
// Delete a particular file from the database
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDB::DeleteFileL( const TDesC& aFile )
    {
    HBufC* buf = HBufC::NewLC( aFile.Length() + KAdditionalStringLength );
    TPtr ptr = buf->Des();

    TParsePtrC parse( aFile );
    TBuf<KDbMaxTableCreationSQLLength> query;
    query += KDelete;
    query += KFrom;
    query += KHarvesterMainTable;
    query += KWhere;
    query += KHarFileName;
    query += KEquals;
    query += KItemBracket;
    ptr.Copy( KNullDesC );
    FindAndReplaceSingleQuote( parse.NameAndExt(), ptr );
    query += ptr;
    query += KItemBracket;
    query += KAnd;
    query += KHarPathName;
    query += KEquals;
    query += KItemBracket;
    ptr.Copy( KNullDesC );
    FindAndReplaceSingleQuote( parse.DriveAndPath(), ptr );
    query += ptr;
    query += KItemBracket;

    // Execute
    User::LeaveIfError( iDatabase->Execute( query, EDbCompareFolded ) );

    CleanupStack::PopAndDestroy( buf );
    }

// ---------------------------------------------------------------------------
// Removes all files from this database
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDB::RemoveAllFilesL()
    {

    // Only delete all if db is opened (ie media present)
    //
    if( iDBOpen )
        {
        // SQL Statement is "DELETE FROM 'tablename'"
        //
        TBuf<KDbMaxTableCreationSQLLength> query;
        query += KDelete;
        query += KFrom;
        query += KHarvesterMainTable;
        User::LeaveIfError( iDatabase->Execute( query ) );

    #ifdef __PRINTDB__
        CMPXHarvesterDatabaseTable* curTable = OpenAllFilesTableL();
        if( curTable )
            {
            CleanupStack::PushL( curTable );
            curTable->PrintItemsInTableL();
            CleanupStack::PopAndDestroy( curTable );
            }
    #endif //__PRINTDB__

        }
    }

// ---------------------------------------------------------------------------
// Deletes the database file, the user should call Close() before hand
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterDB::DeleteDatabase()
    {
    TFileName fileName = GenerateDbName();
    return iFs.Delete( fileName );
    }

// ---------------------------------------------------------------------------
// Read the unique id of the database file
// ---------------------------------------------------------------------------
//
TUint CMPXHarvesterDB::UniqueIdL()
    {
    TBuf<KDbMaxTableCreationSQLLength> query;
    query.Append( KSelectAll );
    query.Append( KHarvesterAuxTable );

    RDbView view;
    User::LeaveIfError( view.Prepare( *iDatabase, query ) );
    User::LeaveIfError( view.Evaluate() );

    TBool found = view.FirstL();
    if( !found )
        {
        User::Leave( KErrCorrupt );
        }

    view.GetL();
    TUint id = view.ColUint(KHarAuxVolumeIdColumn);
    view.Close();
    return id;
    }

// ---------------------------------------------------------------------------
// Counts all files from this database
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterDB::CountAllFilesL()
    {
    TInt count = 0;
    // Only count if db is opened (ie media present)
    if( iDBOpen )
        {
        // Open the table
        CMPXHarvesterDatabaseTable* table = CMPXHarvesterDatabaseTable::NewLC( *iDatabase );
        table->OpenAllFilesTableL();
        count = table->CountL();
        CleanupStack::PopAndDestroy(table);
        }
    return count;
    }

// ---------------------------------------------------------------------------
// Fixes single quote query issues
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDB::FindAndReplaceSingleQuote(const TDesC& aSrc,
                                                TDes& aTrg)
    {
    TPtrC ch;

    TInt srcLen = aSrc.Length();

    for (TInt i = 0; i < srcLen; ++i)
        {
        ch.Set(&aSrc[i], 1);
        aTrg.Append(ch);
        if ( ch.CompareF(_L("'")) == 0)
            {
            aTrg.Append(ch);
            }
        }
    }

// ---------------------------------------------------------------------------
// Begin transaction on this database
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDB::BeginL()
    {
    TInt err = iDatabase->Begin();
    
    if(err != KErrNone)
        {
        User::Leave(err);
        }
    }

// ---------------------------------------------------------------------------
// Check if the database is in transaction
// ---------------------------------------------------------------------------
//
TBool CMPXHarvesterDB::InTransaction()
    {
    MPX_DEBUG1("CMPXHarvesterDB::InTransaction <---");
    TBool inTrans = EFalse;
    if ( iDatabase != NULL )
        {
        inTrans = iDatabase->InTransaction();
        }
    MPX_DEBUG1("CMPXHarvesterDB::InTransaction --->");
    return inTrans;
    }

// ---------------------------------------------------------------------------
// Commit all changes in the transaction on this database
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDB::CommitL()
    {
    MPX_DEBUG1("CMPXHarvesterDB::CommitL <---");
    if ( iDatabase != NULL )
        {
        TInt err = iDatabase->Commit();
        
        if(err != KErrNone)
            {
            User::Leave(err);
            }
        }
    }

// ---------------------------------------------------------------------------
// Rollbacks the current transaction
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDB::Rollback()
    {
    iDatabase->Rollback();
    }

#ifdef __RAMDISK_PERF_ENABLE
// ---------------------------------------------------------------------------
// Set RAM drive info
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDB::SetRamDriveInfo(TDriveNumber aDrive, TBool aUseRamDrive)
    {
    MPX_FUNC("CMPXHarvesterDB::SetRamDriveInfo");
    iRamDrive = aDrive;
    iUseRamDrive = aUseRamDrive;
    }

// ---------------------------------------------------------------------------
// Get UseRamDrive
// ---------------------------------------------------------------------------
//
TBool CMPXHarvesterDB::IsUseRamDrive()
    {
    MPX_FUNC("CMPXHarvesterDB::IsUseRamDrive");
    return iUseRamDrive;
    }

// ---------------------------------------------------------------------------
// Get the state of the database.
// ---------------------------------------------------------------------------
//
TDbState CMPXHarvesterDB::GetDbState()
    {
    MPX_FUNC("CMPXHarvesterDB::GetDbState");
    TDbState state = EDbClose;
    if ( InTransaction() )
        {
        state = EDbInTransaction;
        }
    else if ( iDBOpen )
        {
        state = EDbOpen;
        }
    
    MPX_DEBUG2("CMPXHarvesterDB::GetDbState state = %d", state );
    return state;
    }

// ---------------------------------------------------------------------------
// Set the state of the database.
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDB::SetDbStateL( TDbState aState )
    {
    MPX_FUNC("CMPXHarvesterDB::SetDbState");
    MPX_DEBUG2("CMPXHarvesterDB::SetDbState state = %d", aState );
    
    switch( aState )
        {
        case EDbClose:
            {
            if ( InTransaction() )
                {
                CommitL();
                }
            Close();
            break;
            }
        case EDbOpen:
            {
            if ( InTransaction() )
                {
                CommitL();
                }
            OpenL();
            break;
            }
        case EDbInTransaction:
            {
            if ( !InTransaction() )
                {
                OpenL();
                BeginL();
                }
            break;
            }
        default:
            {
            // should never get here
            User::Leave(KErrNotSupported);
            }
        }
    }

#endif // __RAMDISK_PERF_ENABLE

// ---------------------------------------------------------------------------
// Generate the database name
// ---------------------------------------------------------------------------
//
TFileName CMPXHarvesterDB::GenerateDbName()
    {
    MPX_FUNC("CMPXHarvesterDB::GenerateDbName");
    TFileName fileName;
#ifdef __RAMDISK_PERF_ENABLE
    if ( iUseRamDrive )
        {
        TDriveUnit ramDrive( iRamDrive );
        fileName.Append( ramDrive.Name() );
        fileName.Append( KHarvesterDBPath );
        TDriveUnit drive( iDrive );
        fileName.Append(drive.Name()[0]);
    
        //Use different name for Dbs if the system has an internal drive vs. MMC-only.
        //Since hard-coded drive letters in the Thumbnail URIs
        //So Dbs are not interchangeable between an internal drive system and MMC-only system.		
        if ( iEMMC )
           fileName.Append( KHarvesterDBNameEMMC );
        else
           fileName.Append( KHarvesterDBName );
        }
    else
        {
#endif // __RAMDISK_PERF_ENABLE
        TDriveUnit drive( iDrive );
        fileName.Append( drive.Name() );
        fileName.Append( KHarvesterDBPath );
        if ( iEMMC )
           fileName.Append( KHarvesterDBNameEMMC );
        else
           fileName.Append( KHarvesterDBName );
#ifdef __RAMDISK_PERF_ENABLE
        }
#endif // __RAMDISK_PERF_ENABLE
    MPX_DEBUG2("CMPXHarvesterDB::GenerateDbName file name = %S", &fileName );
    return fileName;
    }

// End of file
