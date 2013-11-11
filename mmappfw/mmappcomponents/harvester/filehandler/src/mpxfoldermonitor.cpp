/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Monitors a folder in the file system for file changes
*
*/


#include <e32base.h>
#include <pathinfo.h>
#include <driveinfo.h>
#include <mpxlog.h>
#include "mpxfoldermonitor.h"
#include "mpxfileadditionobserver.h"
#include "mpxfoldermonitorobserver.h"

// CONSTANTS
_LIT( KMPXMusicPath, "\\Music\\");


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CMPXFolderMonitor::CMPXFolderMonitor( MMPXFolderMonitorObserver& aObserver,
                                      RFs& aFs ) : CActive( EPriorityNormal ),
                                                   iObserver( aObserver ),
                                                   iFs( aFs )

    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// Second Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXFolderMonitor::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// Two-phased constructor
// ---------------------------------------------------------------------------
//
CMPXFolderMonitor* CMPXFolderMonitor::NewL( MMPXFolderMonitorObserver& aObserver,
                                            RFs& aFs )
    {
    CMPXFolderMonitor* self = new( ELeave ) CMPXFolderMonitor( aObserver, aFs );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXFolderMonitor::~CMPXFolderMonitor()
    {
    Cancel();
    delete iFolderName;
    }

// ---------------------------------------------------------------------------
// Starts monitoring a particular drive
// ---------------------------------------------------------------------------
//
void CMPXFolderMonitor::StartL( TDriveNumber aDrive )
    {
    MPX_DEBUG1("CMPXFolderMonitor::Start <---");

    // What is the drive name?
    delete iFolderName;
    iFolderName = NULL;

    switch( aDrive )
        {
        case EDriveC:
            {
            // Use the default path for the phone memory
            iFolderName =
                PathInfo::PhoneMemoryRootPath().AllocL();
            break;
            }
        default:
            {
            // Ensure that the drive is available
            // Get the drive status
            TUint driveStatus(0);
            const TInt err = DriveInfo::GetDriveStatus(
                iFs, aDrive, driveStatus );

            if ( ( err == KErrNone ) &&
                (driveStatus & DriveInfo::EDriveUserVisible ) )
                {
                // Use the music path for the drive
                iFolderName = HBufC::NewL(KMaxFileName);
                TPtr folderPtr( iFolderName->Des() );
                User::LeaveIfError(
                    PathInfo::GetRootPath( folderPtr, aDrive ) );
                folderPtr.Append( KMPXMusicPath() );
                }
            else
                {
                TChar driveChar;
                User::LeaveIfError(
                    iFs.DriveToChar( aDrive, driveChar ) );
                MPX_DEBUG2("CMPXFolderMonitor::Start: Drive %c: not supported!",
                    (TUint)driveChar );
                User::Leave( KErrNotSupported );
                }
            break;
            }
        }
    MPX_DEBUG2("CMPXFolderMonitor::Start: Use %S path", iFolderName);

    // Start listening
    //
    TNotifyType notType = ENotifyFile;
    iFs.NotifyChange( notType, iStatus, *iFolderName );
    SetActive();

    MPX_DEBUG1("CMPXFolderMonitor::Start --->");
    }

// ---------------------------------------------------------------------------
// Start monitoring a particular folder
// ---------------------------------------------------------------------------
//
void CMPXFolderMonitor::StartL( const TDesC& aFolder )
    {
    MPX_DEBUG1("CMPXFolderMonitor::StartL <---");

    // Copy folder
    //
    delete iFolderName;
    iFolderName = NULL;
    iFolderName = aFolder.AllocL();

    // Start listening
    //
    TNotifyType notType = ENotifyFile;
    iFs.NotifyChange( notType, iStatus, *iFolderName );
    SetActive();

    MPX_DEBUG1("CMPXFolderMonitor::StartL --->");
    }

// ---------------------------------------------------------------------------
// RunL callback
// ---------------------------------------------------------------------------
//
void CMPXFolderMonitor::RunL()
    {
    MPX_DEBUG1("CMPXFolderMonitor::RunL <---");

    // Tell the observer that this folder has changed
    // It should go and scan it for files?
    iObserver.HandleDirectoryChangedL( *iFolderName );

    // Listen again
    TNotifyType notType(ENotifyFile);
    iFs.NotifyChange( notType, iStatus, *iFolderName );
    SetActive();
    MPX_DEBUG1("CMPXFolderMonitor::RunL --->");
    }

// ---------------------------------------------------------------------------
// Handle Cancelling
// ---------------------------------------------------------------------------
//
void CMPXFolderMonitor::DoCancel()
    {
    MPX_DEBUG1("CMPXFolderMonitor::DoCancel <---");

    // Stop monitoring
    iFs.NotifyChangeCancel();
    }

// ----------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL()
// ----------------------------------------------------------------------------
//
TInt CMPXFolderMonitor::RunError(TInt aError)
    {
    MPX_DEBUG2("CMPXFolderMonitor::RunError(%d)", aError );
    (void)aError;

    // Listen again
    TNotifyType notType(ENotifyFile);
    iFs.NotifyChange( notType, iStatus, *iFolderName );
    SetActive();

    return KErrNone;
    }
