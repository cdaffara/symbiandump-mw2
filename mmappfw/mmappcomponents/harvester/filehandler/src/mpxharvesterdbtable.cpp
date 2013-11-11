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
* Description:  Encapsulates operations on a table in the db
*
*/


#include <e32base.h>
#include <mpxlog.h>
#include "mpxharvesterdbtable.h"
#include "mpxdbcommon.h"
#include "mpxharvesterdbitem.h"

const TInt KStringLength = 300;

// ---------------------------------------------------------------------------
// Constructor 
// ---------------------------------------------------------------------------
//
CMPXHarvesterDatabaseTable::CMPXHarvesterDatabaseTable( RDbStoreDatabase& aDb )
                                                                  : iDb( aDb ),
                                                        iState( EInvalidState )
    {
    }

// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterDatabaseTable* CMPXHarvesterDatabaseTable::NewLC( 
                                                       RDbStoreDatabase& aDb )
    {
    CMPXHarvesterDatabaseTable* self = 
                               new( ELeave ) CMPXHarvesterDatabaseTable( aDb );
    CleanupStack::PushL( self );
    return self;
    }


// ---------------------------------------------------------------------------
// destructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterDatabaseTable::~CMPXHarvesterDatabaseTable()
    {
    iView.Close();
    }

// ---------------------------------------------------------------------------
// Open the Table to list all files in the db
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseTable::OpenAllFilesTableL()
    {
    MPX_DEBUG1("CMPXHarvesterDatabaseTable::OpenAllFilesTableL <---");
    
    // Close old view just in case
    iView.Close();
    
    // Open all files query
    // select * from files
    //
    TBuf<KDbMaxTableCreationSQLLength> query;
    query.Append( KSelectAll );
    query.Append( KHarvesterMainTable );
    
    // Open the view
    User::LeaveIfError( iView.Prepare( iDb, query ) );
    User::LeaveIfError( iView.Evaluate() );
    
    iState = EAllItems;
    
    MPX_DEBUG1("CMPXHarvesterDatabaseTable::OpenAllFilesTableL --->");
    } 
       
// ---------------------------------------------------------------------------
// Open the Table to a specific directory ( drive:\\path )
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseTable::OpenDirectoryL( const TDesC& aDirectory )
    {
    MPX_DEBUG1("CMPXHarvesterDatabaseTable::OpenDirectoryL <---");
    
    // Close old view just in case
    iView.Close();
    
    HBufC* buffer = HBufC::NewLC(KStringLength); // 255 file path + 45 for single quotes
                                                 // magic, this would break if we have >
                                                 // 45 single quotes in a file name.
    TParsePtrC parse( aDirectory );
    TPtr ptr = buffer->Des(); 
    FindAndReplaceSingleQuote( parse.DriveAndPath(), ptr );
    
    // Open all files query
    // select * from files where filepath='aDirectory'
    //
    TBuf<KDbMaxTableCreationSQLLength> query;
    query += KSelectAll;
    query += KHarvesterMainTable;
    query += KWhere;
    query += KHarPathName;
    query += KLike; 
    query += KItemBracket;
    query += ptr;
    query += KWildcard;
    query += KItemBracket;
    
    // Open the view
    TDbQuery sqlQuery( query, EDbCompareFolded ); 
    User::LeaveIfError( iView.Prepare( iDb, sqlQuery ) );
    User::LeaveIfError( iView.Evaluate() );
    
    iState = EDirectory;
    CleanupStack::PopAndDestroy(buffer);
    MPX_DEBUG1("CMPXHarvesterDatabaseTable::OpenDirectoryL --->");
    } 
       
// ---------------------------------------------------------------------------
// Open the Table to a specific song view
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseTable::OpenItemL( const TDesC& aSongName )
    {
    MPX_DEBUG1("CMPXHarvesterDatabaseTable::OpenItemL <---");
    
    // Close old view just in case
    iView.Close();
    
    HBufC* buffer = HBufC::NewLC(KStringLength); // 255 file path + 45 for single quotes
                                                 // magic, this would break if we have > 45 single quotes in a file name.
    TPtr ptr = buffer->Des();
    // Open all files query
    // select * from files where path='path' AND filename='aSongName'
    //
    TParsePtrC parse( aSongName );
    TBuf<KDbMaxTableCreationSQLLength> query;
    query += KSelectAll;
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
    
    // Open the view
    TDbQuery sqlQuery( query, EDbCompareFolded ); 
    User::LeaveIfError( iView.Prepare( iDb, sqlQuery ) );
    User::LeaveIfError( iView.Evaluate() );
    
    iState = EFile;
    CleanupStack::PopAndDestroy( buffer );
    MPX_DEBUG1("CMPXHarvesterDatabaseTable::OpenItemL --->");
    } 
       
// ---------------------------------------------------------------------------
// Open a table to list of files with DRM flag set.
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseTable::OpenDrmL()
    {
    MPX_DEBUG1("CMPXHarvesterDatabaseTable::OpenDrmL <---");
    
    // Close old view just in case
    iView.Close();
    
    HBufC* buffer = HBufC::NewLC(KStringLength); // 255 file path + 45 for single quotes
                                                 // magic, this would break if we have > 45 single quotes in a file name.
    TPtr ptr = buffer->Des();
    // Open all files query
    // select * from files where path='path' AND filename='aSongName'
    //
    TBuf<KDbMaxTableCreationSQLLength> query;
    query += KSelectAll;
    query += KHarvesterMainTable;
    query += KWhere;
    query += KHarItemDRM;
    query += KEquals;
    query += KOn;
    
    // Open the view
    TDbQuery sqlQuery( query, EDbCompareFolded ); 
    User::LeaveIfError( iView.Prepare( iDb, sqlQuery ) );
    User::LeaveIfError( iView.Evaluate() );
    
    iState = EDrm;
    CleanupStack::PopAndDestroy( buffer );
    MPX_DEBUG1("CMPXHarvesterDatabaseTable::OpenDrmL --->");
    } 
       
// ---------------------------------------------------------------------------
// Delete the item pointed to at the current row
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseTable::DeleteItemL(TBool aEndTransaction)
    {
    MPX_DEBUG1("CMPXHarvesterDatabaseTable::DeleteItemL <---");

#ifdef __PRINTDB__     
    PrintItemsInTableL();
#endif //__PRINTDB__
        
    if (!iDb.InTransaction())
        {
        iDb.Begin();
        }
    // Safe row update
    TBool found = iView.FirstL();
    if( !found )
        {
        if ( aEndTransaction )
            {
            iDb.Commit();
            }
        User::Leave(KErrNotFound);    
        }
    iView.GetL();
    
    TRAPD( err, iView.DeleteL() );
    if( err != KErrNone )
        {
        iView.Cancel();
        if ( aEndTransaction )
            {
            iDb.Commit();
            }
        User::Leave( err );
        }
    if ( aEndTransaction )
        {
        iDb.Commit();
        }
    // DB Compaction
    struct RDbDatabase::TSize size = iDb.Size();
    const TInt waterMark = 60;
    if (size.iUsage <= waterMark)
        {
        MPX_DEBUG1( "Compacting DB" );
        iDb.Compact();
        }
        
    MPX_DEBUG1("CMPXHarvesterDatabaseTable::DeleteItemL --->");
    }  

// ---------------------------------------------------------------------------
// Return the number of rows in the current view
// ---------------------------------------------------------------------------
//
TInt CMPXHarvesterDatabaseTable::CountL()
    {
    return iView.CountL();
    }
    
// ---------------------------------------------------------------------------
// Add an item to the table
// ---------------------------------------------------------------------------
//
void CMPXHarvesterDatabaseTable::AddItemL( const TDesC& aPath, 
                                           const TDesC& aFileName, 
                                           const TTime aLastModTime,
                                           const TInt  aColId,
                                           const TBool aDRM )
    {
    MPX_DEBUG1("CMPXHarvesterDatabaseTable::AddItemL <---");
    
    // Add the item into the db
    iView.InsertL();
    iView.SetColL( KHarPathNameColumn, aPath );
    iView.SetColL( KHarFileNameColumn, aFileName );
    iView.SetColL( KHarModTimeColumn,  aLastModTime );
    iView.SetColL( KHarColDBIDColumn,  aColId );
    iView.SetColL( KHarDRMColumn, (TUint) aDRM );
    
    // Safe update to database
    TRAPD( err, iView.PutL() );
    if( KErrNone != err )
        {
        iView.Cancel();
        User::Leave( err );
        }
   
#ifdef __PRINTDB__     
    PrintItemsInTableL();
#endif //__PRINTDB__
 
    MPX_DEBUG1("CMPXHarvesterDatabaseTable::AddItemL --->");
    }
    
// ---------------------------------------------------------------------------
// Update the current row with the updated info
// ---------------------------------------------------------------------------
//    
void CMPXHarvesterDatabaseTable::UpdateItemL( const TDesC& /*aPath*/, 
                                              const TTime aLastModTime,
                                              const TInt  aColId,
                                              const TBool aDrm,
                                              TBool aNewTransaction )    
    {
    // Safe row update
    //
    TBool newTransaction = aNewTransaction && !iDb.InTransaction();
    if ( newTransaction )
        {
        iDb.Begin();
        }
    TBool valid = iView.FirstL();
    if( valid )
        {
        iView.UpdateL();
        iView.SetColL( KHarModTimeColumn, aLastModTime );
        iView.SetColL( KHarColDBIDColumn, aColId );
        iView.SetColL( KHarDRMColumn, (TUint) aDrm );
        }
    else
        {
        if( newTransaction )
            {
            iDb.Commit();
            }
        User::Leave( KErrNotFound );    
        }
        
    TRAPD( err, iView.PutL() );
    if( err != KErrNone )
        {
        iView.Cancel();
        if ( newTransaction )
            {
            iDb.Commit();
            }
        User::Leave( err );
        }
    if ( newTransaction )
        {
        iDb.Commit();
        }
              
    // DB Compaction
    struct RDbDatabase::TSize size = iDb.Size();
    const TInt waterMark = 60;
    if (size.iUsage <= waterMark)
        {
        if ( newTransaction )
            {
            MPX_DEBUG1( "Compacting DB" );
            iDb.Compact();
            }
        }
    }

// ---------------------------------------------------------------------------
// Update the current row with the updated info
// ---------------------------------------------------------------------------
//    
void CMPXHarvesterDatabaseTable::UpdateItemL( const TTime aLastModTime,
                                              const TInt  aColId,
                                              TBool aNewTransaction,
                                              const TDesC& aPath )    
    {
    // Safe row update
    //
    TBool newTransaction = aNewTransaction && !iDb.InTransaction();
    if( iView.FirstL() )
        {
        if ( newTransaction )
            {
            iDb.Begin();
            }
        iView.UpdateL();
        if ( aPath != KNullDesC )
            {
            TParsePtrC parser( aPath );
            iView.SetColL( KHarPathNameColumn, parser.DriveAndPath() );
            iView.SetColL( KHarFileNameColumn, parser.NameAndExt() );        
            }
        iView.SetColL( KHarModTimeColumn, aLastModTime );
        if ( aColId != KNullUid.iUid )
            {
            iView.SetColL( KHarColDBIDColumn, aColId );
            }
        }
    else
        {
        User::Leave( KErrNotFound );    
        }
        
    TRAPD( err, iView.PutL() );
    if( err != KErrNone )
        {
        iView.Cancel();
        if ( newTransaction )
            {
            iDb.Commit();
            }
        
        User::Leave( err );
        }
    if ( newTransaction )
        {
        iDb.Commit();
        }

    // DB Compaction
    struct RDbDatabase::TSize size = iDb.Size();
    const TInt waterMark = 60;
    if (size.iUsage <= waterMark)
        {
        if ( newTransaction )
            {
            MPX_DEBUG1( "Compacting DB" );
            iDb.Compact();
            }
        }
    }

// ---------------------------------------------------------------------------
// Local function to cleanup an array
// ---------------------------------------------------------------------------
//
static void CleanupArray( TAny* item )
    {
    ((RPointerArray<CMPXHarvesterDbItem>*) item )->ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// Create an array representing the table that is generated
// ---------------------------------------------------------------------------
//     
RPointerArray<CMPXHarvesterDbItem>* CMPXHarvesterDatabaseTable::CreateTableRepresentationL()
    {
    MPX_DEBUG1("CMPXHarvesterDatabaseTable::CreateTableRepresentationL <---");
    
    RPointerArray<CMPXHarvesterDbItem>* array = 
                              new (ELeave) RPointerArray<CMPXHarvesterDbItem>;
    CleanupStack::PushL( TCleanupItem( CleanupArray, array) );
    
    for (iView.FirstL();iView.AtRow();iView.NextL())
        {
        // Retrieve the row
        iView.GetL();
        
        CMPXHarvesterDbItem* item = new(ELeave) CMPXHarvesterDbItem();
        CleanupStack::PushL( item );
        
        // Setup the item
        HBufC* filepath = ReadLongTextLC( KHarPathNameColumn );
        HBufC* songname = ReadLongTextLC( KHarFileNameColumn );
        item->iFile = HBufC::NewL( filepath->Length() + songname->Length() );
        TPtr buf = item->iFile->Des();
        buf.Append( *filepath );
        buf.Append( *songname );
        CleanupStack::PopAndDestroy( 2, filepath );
        
        item->iLastModifiedTime = iView.ColTime( KHarModTimeColumn );
        item->iColId = iView.ColInt( KHarColDBIDColumn );
        item->iDrm   = iView.ColUint( KHarDRMColumn );
        
        User::LeaveIfError( array->InsertInOrderAllowRepeats( item, 
                            CMPXHarvesterDbItem::Compare ) );
        CleanupStack::Pop( item );
        }
  
    CleanupStack::Pop();  // cleanup
    MPX_DEBUG1("CMPXHarvesterDatabaseTable::CreateTableRepresentationL --->");
    return array;
    }
    
// ---------------------------------------------------------------------------
// Read out the text at the current row and at a specific column
// ---------------------------------------------------------------------------
// 
HBufC* CMPXHarvesterDatabaseTable::ReadLongTextLC( TInt aColumn )
    {
    TInt len = iView.ColLength( aColumn );

    HBufC* buf = HBufC::NewLC(len);
    TPtr value( buf->Des() );
    if ( len>0 )
        {
        RDbColReadStream strm;
        strm.OpenLC(iView, aColumn );
        strm.ReadL( value, len );
        strm.Close();
        CleanupStack::PopAndDestroy( &strm );
        }
    return buf;
    }

// ---------------------------------------------------------------------------
// Fixes single quote query issues
// ---------------------------------------------------------------------------
// 
void CMPXHarvesterDatabaseTable::FindAndReplaceSingleQuote(const TDesC& aSrc,
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

#ifdef __PRINTDB__ 
// ---------------------------------------------------------------------------
// Print out all items in the current table
// ---------------------------------------------------------------------------
//  
void CMPXHarvesterDatabaseTable::PrintItemsInTableL()
    {
    for (iView.FirstL();iView.AtRow();iView.NextL())
        {
        // Retrieve the row
        iView.GetL();
        
        // Print out each row
        HBufC* filepath = ReadLongTextLC( KHarPathNameColumn );
        HBufC* songname = ReadLongTextLC( KHarFileNameColumn );
        TTime time = iView.ColTime( KHarModTimeColumn );
        MPX_DEBUG4("Filepath: %S Filename:%S Time %i", filepath, songname, time.Int64() );
        CleanupStack::PopAndDestroy( 2, filepath );
        }
  }   
#endif // __PRINTDB__

