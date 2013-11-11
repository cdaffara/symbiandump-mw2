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
* Description:  Folder scanning class to scan files in the file system
*
*/

#include <e32base.h>
#include <mpxlog.h>
#include "mpxfolderscanner.h"
#include "mpxfileadditionobserver.h"
#include "mpxfilescanstateobserver.h"
#include "mpxfhcommon.h"

// CONSTANTS
const TInt KFileNumBreakCount = 5;
_LIT( KTxtBackSlash, "\\" );

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// RArray compare function to compare strings
// ---------------------------------------------------------------------------
//
static TInt CompareString(const TPath& aFirst,
                          const TPath& aSecond)
    {
    return aFirst.Compare( aSecond );
    }
    
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CMPXFolderScanner::CMPXFolderScanner( MMPXFileAdditionObserver& aObserver,
                                      MMPXFileScanStateObserver& aStateObs,
                                      RFs& aFs  ) : CActive(EPriorityNull),
                                                  iObserver( aObserver ),
                                                  iStateObserver( aStateObs ),
                                                  iFs( aFs )
                                                    
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXFolderScanner::ConstructL()
    {

    }

// ---------------------------------------------------------------------------
// Two Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXFolderScanner* CMPXFolderScanner::NewL( MMPXFileAdditionObserver& aObserver,
                                            MMPXFileScanStateObserver& aStateObs,
                                            RFs& aFs )
    {
    CMPXFolderScanner* self = new( ELeave ) CMPXFolderScanner( aObserver, 
                                                               aStateObs,
                                                               aFs );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXFolderScanner::~CMPXFolderScanner()
    {
    Cancel();
    iDirQueue.ResetAndDestroy();
    iDrivesToScan.Close();
    }
    
// ---------------------------------------------------------------------------
// Scans a list of drives for files
// ---------------------------------------------------------------------------
//
void CMPXFolderScanner::ScanL( RArray<TPath>& aDrives )
    {
    MPX_DEBUG1("CMPXFolderScanner::ScanL <---");
    
    // Copy all the other drives we want to scan
    //
    TInt count( aDrives.Count() );
    MPX_DEBUG2("CMPXFolderScanner::ScanL aDrives %d",count);
    for( TInt i=0; i<count; ++i )
        {
        // Check if we are already scanning this drive
        TInt found( iDrivesToScan.FindInOrder( aDrives[i], CompareString ) ); 
        if( found == KErrNotFound )
            {
            iDrivesToScan.Append( aDrives[i] );
            }
        }
    
    // If we were already scanning, don't do it again
    //
    if( !iScanning )
        {
        // Setup the next drive to scan
        //
        if( !SetupNextDriveToScanL() )
            {
            // Kick off the scanning
            iCurDirQueueEntry = iDirQueue[ 0 ];
            iCurFullPath = iCurDirQueueEntry->iFullPath;
            ReadDirEntry();

            // We've started scanning
            iScanning = ETrue;
            }
        else
            {
            // Nothing to scan
            DoScanCompleteL(KErrNone);    
            }        
        }
    MPX_DEBUG1("CMPXFolderScanner::ScanL --->");
    }

// ---------------------------------------------------------------------------
// Reads dir entries asynchronously
// ---------------------------------------------------------------------------
//
void CMPXFolderScanner::ReadDirEntry()
    {
    ASSERT( !IsActive() );
    iCurDirQueueEntry->iDir.Read( iCurDirQueueEntry->iEntryArray, iStatus );
    SetActive();
    }

// ---------------------------------------------------------------------------
// Continue Scanning for more files
// ---------------------------------------------------------------------------
//
TBool CMPXFolderScanner::DoScanL()
    {
    MPX_DEBUG1("CMPXFolderScanner::DoScanL <---");
    TBool done (EFalse);
    TBool blocked ( EFalse );

    // read successfully
    if ( iStatus == KErrNone || iStatus == KErrEof )
        {
        TBuf<KMaxFileName> buffer;
        const TEntry* entry = NULL;
        TInt numEntries( iCurDirQueueEntry->iEntryArray.Count() );

        // process the entry one by one
        while ( iCurDirQueueEntry->iPos < numEntries )
            {
            entry = iCurDirQueueEntry->NextEntry();
            buffer.Zero();

            // Generates the full name of the entry
            buffer.Append( *iCurFullPath );
            buffer.Append( entry->iName );

            if ( entry->IsDir() ) // entry is a directory
                {
                buffer.Append( KTxtBackSlash );

                blocked = iObserver.IsPathBlockedL( buffer );

                if ( !blocked )
                    {
                    CDirQueueEntry* newEntry = CDirQueueEntry::NewL( buffer );
                    TInt err = newEntry->iDir.Open( iFs,
                                                    buffer,
                                             KEntryAttNormal | KEntryAttDir );
                    if ( err == KErrNone )
                        {
                        CDirQueueEntry::PushL( iDirQueue, newEntry );
                        }
                    else
                        {
                        delete newEntry;
                        }
                    }
                }
            else // entry is a file
                {
                TInt index = iObserver.IsMediaFileL( buffer );
                if( KErrNotFound != index )
                    {
                    iObserver.HandleFileAdditionL( buffer, index );
                    }
                }
            if ( iCurDirQueueEntry->iPos % KFileNumBreakCount == 0 )
                {
                return done;
                }
            }
        }

    // this dir has other entries to read
    if ( iStatus == KErrNone )
        {
        iCurDirQueueEntry->ResetPosition();
        ReadDirEntry();
        }

    // there is nothing to read or some error has occured during reading,
    // try to move to next dir
    else
        {
        CDirQueueEntry::PopAndDestroy( iDirQueue );
        if ( iDirQueue.Count() || !SetupNextDriveToScanL() )
            {
            iCurDirQueueEntry = iDirQueue[ 0 ];
            iCurFullPath = iCurDirQueueEntry->iFullPath;
            ReadDirEntry();
            }
        else // there is nothing to scan
            {
            done = ETrue;
            }
        }
        
    MPX_DEBUG1("CMPXFolderScanner::DoScanL --->");
    return done;
    }

// ---------------------------------------------------------------------------
// Setup the object to scan the next directory
// ---------------------------------------------------------------------------
//
TBool CMPXFolderScanner::SetupNextDriveToScanL()
    {
    MPX_DEBUG1("CMPXFolderScanner::SetupNextDriveToScanL <---");
    
    TBool done(EFalse);
    TBool blocked(EFalse);
    // Scan next drive
    while( iDrivesToScan.Count() && !iDirQueue.Count() )
        {
        TPath path = iDrivesToScan[0];

        MPX_DEBUG1( "CMPXFolderScanner::SetupNextDriveToScanL path blocked?" );
        blocked = iObserver.IsPathBlockedL( path );
        MPX_DEBUG2( "CMPXFolderScanner::SetupNextDriveToScanL path blocked %i",
                    blocked );

        // If there was something to scan
        if( !blocked )
            {
            CDirQueueEntry* newEntry = CDirQueueEntry::NewL( path );
            TInt err = newEntry->iDir.Open( iFs,
                                            path,
                                            KEntryAttNormal | KEntryAttDir );
            if ( err == KErrNone )
                {
                CDirQueueEntry::PushL( iDirQueue, newEntry );
                // Inform Observer of the new drive that we are scanning
                iObserver.HandleOpenDriveL( ::ExtractDrive(iDrivesToScan[0]),
                                            iDrivesToScan[0] );
                }
            else
                {
                delete newEntry;
                }
            }

        // Remove the 0th element
        iDrivesToScan.Remove(0);
        iDrivesToScan.Compress();

        }

    if ( !iDirQueue.Count() )
        {
        done = ETrue;
        }
    
    
    MPX_DEBUG1("CMPXFolderScanner::SetupNextDriveToScanL --->");
    return done;
    }
    
// ---------------------------------------------------------------------------
// Handle when scanning is complete
// ---------------------------------------------------------------------------
//
void CMPXFolderScanner::DoScanCompleteL( TInt aErr )
    {
    MPX_DEBUG1("CMPXFolderScanner::DoScanCompleteL <---");
    
    // Reset all arrays and data
    iDrivesToScan.Reset();
    iDirQueue.ResetAndDestroy();

    // All done!
    iScanning = EFalse;

    // Callback to observer
    iStateObserver.HandleScanStateCompleteL( MMPXFileScanStateObserver::EScanFiles, 
                                              aErr );
    
    MPX_DEBUG1("CMPXFolderScanner::DoScanCompleteL --->");
    }
     
// ---------------------------------------------------------------------------
// From CActive
// ---------------------------------------------------------------------------
//
void CMPXFolderScanner::RunL()
    {
    MPX_DEBUG1("CMPXFolderScanner::RunL <---");
    
    // Do more scanning
    TBool done(EFalse);
    TRAPD( err, done = DoScanL() );   
     
    // We are all done
    //
    if( KErrNone != err || done )
        {
        DoScanCompleteL( err );
        }
    else if ( iCurDirQueueEntry->iPos ) // if( !done )
        {
        MPX_DEBUG1("CMPXFolderScanner::RunL -- Run again");
        iStatus = KRequestPending;
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        }
    }

// ---------------------------------------------------------------------------
// From CActive
// ---------------------------------------------------------------------------
//
void CMPXFolderScanner::DoCancel()
    {
    if( iScanning )
        {
        // Callback to observer with the partial list?
        TRAP_IGNORE( DoScanCompleteL( KErrCancel ) );
        }
    }
    
// ----------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL()
// ----------------------------------------------------------------------------
//
TInt CMPXFolderScanner::RunError(TInt aError)
    {
    MPX_DEBUG2("CMPXFolderScanner::RunError(%d)", aError );
    
    TRAP_IGNORE( DoScanCompleteL( aError ) );
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Two Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXFolderScanner::CDirQueueEntry* CMPXFolderScanner::CDirQueueEntry::NewL(
                                                    const TDesC& aFullPath )
    {
    CDirQueueEntry* self = new ( ELeave ) CDirQueueEntry;
    CleanupStack::PushL( self );
    self->ConstructL( aFullPath );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXFolderScanner::CDirQueueEntry::~CDirQueueEntry()
    {
    iDir.Close();
    delete iFullPath;
    }

// ---------------------------------------------------------------------------
// Push a dir entry into a dir entry queue
// ---------------------------------------------------------------------------
//
void CMPXFolderScanner::CDirQueueEntry::PushL(
                                     RPointerArray<CDirQueueEntry>& aDirQueue,
                                     CDirQueueEntry* aDirEntry )
    {
    aDirQueue.AppendL( aDirEntry );
    }

// ---------------------------------------------------------------------------
// Pop and destroy a dir entry from a dir entry queue
// ---------------------------------------------------------------------------
//
void CMPXFolderScanner::CDirQueueEntry::PopAndDestroy(
                                  RPointerArray< CDirQueueEntry >& aDirQueue )
    {
    CDirQueueEntry* entry = aDirQueue[ 0 ];
    delete entry;
    aDirQueue.Remove( 0 );
    }

// ---------------------------------------------------------------------------
// CMPXFolderScanner::CDirQueueEntry::NextEntry
// ---------------------------------------------------------------------------
//
const TEntry* CMPXFolderScanner::CDirQueueEntry::NextEntry()
    {
    const TEntry* entry = NULL;
    if ( iPos >= 0 && iPos < iEntryArray.Count() )
        {
        entry = &iEntryArray[ iPos++ ];
        }
    return entry;
    }

// ---------------------------------------------------------------------------
// Reset the current position of entry array
// ---------------------------------------------------------------------------
//
void CMPXFolderScanner::CDirQueueEntry::ResetPosition()
    {
    iPos = 0;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CMPXFolderScanner::CDirQueueEntry::CDirQueueEntry()
    {
    // Do nothing
    }

// ---------------------------------------------------------------------------
// 2nd Phase Contructor
// ---------------------------------------------------------------------------
//
void CMPXFolderScanner::CDirQueueEntry::ConstructL( const TDesC& aFullPath )
    {
    iFullPath = aFullPath.AllocL();
    }
