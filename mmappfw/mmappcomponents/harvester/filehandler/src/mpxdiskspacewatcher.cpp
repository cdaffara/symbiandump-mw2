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
* Description:  Low disk space watcher
*
*/


#include <e32base.h>
#include <mpxlog.h>
#include "mpxdiskspacewatcher.h"
#include "mpxdiskspacewatcherobserver.h"

// CONSTANTS
const TInt KLowDiskThreshold  = 131072 * 4; // 128k *4

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CMPXDiskSpaceWatcher::CMPXDiskSpaceWatcher( RFs& aFs, TInt aDrive,
                                            MMPXDiskSpaceObserver& aObs  )
                                           : CActive( CActive::EPriorityHigh ),
                                             iFs( aFs ),
                                             iDriveToMonitor( aDrive ),
                                             iObs( aObs )

    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CMPXDiskSpaceWatcher::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXDiskSpaceWatcher* CMPXDiskSpaceWatcher::NewL( RFs& aFs, TInt aDrive,
                                                  MMPXDiskSpaceObserver& aObs  )
    {
    CMPXDiskSpaceWatcher* self = new( ELeave ) CMPXDiskSpaceWatcher( aFs,
                                                                     aDrive,
                                                                     aObs );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// destructor
// ---------------------------------------------------------------------------
//
CMPXDiskSpaceWatcher::~CMPXDiskSpaceWatcher()
    {
    Cancel();
    }

// ---------------------------------------------------------------------------
// Start the watcher
// ---------------------------------------------------------------------------
//
void CMPXDiskSpaceWatcher::StartL()
    {
    TVolumeInfo volInfo;
    TInt err = iFs.Volume( volInfo, iDriveToMonitor );

    // Only start if the disk is actually working
    //
    if (!IsActive() && err == KErrNone )
        {
        MPX_DEBUG2("CMPXDiskSpaceWatcher::Start monitor drive %d",
                   iDriveToMonitor);
        iFs.NotifyDiskSpace( KLowDiskThreshold, iDriveToMonitor, iStatus );  //lint !e747
        SetActive();
        }
    }

// ---------------------------------------------------------------------------
// Check if the disk is low on memory
// ---------------------------------------------------------------------------
//
TBool CMPXDiskSpaceWatcher::IsLowOnDisk()
    {
    TVolumeInfo volInfo;
    TInt err = iFs.Volume( volInfo, iDriveToMonitor );

    MPX_DEBUG3("CMPXDiskSpaceWatcher::IsLowOnDisk disk %i free %i",
               iDriveToMonitor, volInfo.iFree );

    // Can only trust disk space amount if we could get volume info
    //
    if( (volInfo.iFree < KLowDiskThreshold) && err == KErrNone )
        {
        MPX_DEBUG1("LOW DISK SPACE");
        return ETrue;
        }
    else
        {
        MPX_DEBUG1("DISK SPACE OK");
        return EFalse;
        }

    }

// ---------------------------------------------------------------------------
// Gets the currently monitored drive number
// ---------------------------------------------------------------------------
//
TInt CMPXDiskSpaceWatcher::CurrentDrive()
    {
    return iDriveToMonitor;
    }

// ---------------------------------------------------------------------------
// Cancel the disk notification
// ---------------------------------------------------------------------------
//
void CMPXDiskSpaceWatcher::DoCancel()
    {
    // Cancel the disk space monitoring
    iFs.NotifyDiskSpaceCancel();
    }

// ---------------------------------------------------------------------------
// Active Object callback
// ---------------------------------------------------------------------------
//
void CMPXDiskSpaceWatcher::RunL()
    {
    // Callback to observer about which drive is low on disk space
    //Check whether the current disk is low on disk space in Mass Storage mode.
    //    
    if (IsLowOnDisk())
        {
        iObs.HandleLowDiskEvent( iDriveToMonitor );
        }
    }

// END OF FILE
