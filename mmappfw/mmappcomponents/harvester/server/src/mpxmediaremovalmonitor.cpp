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
* Description:  Monitors for Media removal
*
*/


#include <e32base.h>
#include <f32file.h>
#include <driveinfo.h>
#include <mpxlog.h>
#include "mpxmediaremovalmonitor.h"

// ---------------------------------------------------------------------------
// C++ Constructor
// ---------------------------------------------------------------------------
//
CMPXMediaRemovalMonitor::CMPXMediaRemovalMonitor
                  ( TInt aDrive, RFs& aFs, MMPXSystemEventObserver& aObserver ) 
                                                       : iDrive( aDrive ),
                                                         iFs( aFs ),
                                                         iDiskRemoved( EFalse ),
                                                         iObserver( aObserver )
                                                         
    {
    }


// ---------------------------------------------------------------------------
// 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXMediaRemovalMonitor::ConstructL()
    {
    MPX_FUNC("CMPXMediaRemovalMonitor::ConstructL");    	
    iDiskNotifyHandler = CDiskNotifyHandler::NewL(*this, iFs);
    CheckDriveStatus();
    
    iDiskNotifyHandler->NotifyDisk();
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXMediaRemovalMonitor* CMPXMediaRemovalMonitor::NewL
                ( TInt aDrive, RFs& aFs, MMPXSystemEventObserver& aObserver )
    {
    CMPXMediaRemovalMonitor* self
                               = CMPXMediaRemovalMonitor::NewLC( aDrive,
                                                                 aFs,
                                                                 aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXMediaRemovalMonitor* CMPXMediaRemovalMonitor::NewLC
               ( TInt aDrive, RFs& aFs, MMPXSystemEventObserver& aObserver )
    {
    CMPXMediaRemovalMonitor* self = 
                          new( ELeave ) CMPXMediaRemovalMonitor( aDrive,
                                                                 aFs,
                                                                 aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXMediaRemovalMonitor::~CMPXMediaRemovalMonitor()
    {
    delete iDiskNotifyHandler;
    }
    
void CMPXMediaRemovalMonitor::CheckDriveStatus()
    {
    TDriveList driveList;
    TInt driveCount(0);
    TInt error = DriveInfo::GetUserVisibleDrives( iFs, driveList, driveCount );
    if ( error != KErrNone )
        {
        MPX_DEBUG2("CMPXMediaRemovalMonitor::CheckDriveStatus GetUserVisibleDrives failed, error %d", error);
        return;
        }
    for( TInt driveNum = EDriveA; driveNum < EDriveZ; driveNum++ )
        {
        if ( driveNum != EDriveC && driveList[driveNum] )
            {
            TDriveInfo driveInfo;
            if (iFs.Drive(driveInfo, driveNum) == KErrNone)
               {
               if ( (driveInfo.iType != EMediaNotPresent) && ! (driveInfo.iDriveAtt & KDriveAttRemote) )
                   {
                   TInt error = iDiskNotifyHandler->NotifyDismount( driveNum );
                   MPX_DEBUG3("CMPXMediaRemovalMonitor::CheckDriveStatus monitoring drive %d for dismounts, error %d", driveNum, error);
                   }
                }
            if ( driveNum == iDrive )
                {
                iDiskRemoved = driveInfo.iType == EMediaNotPresent;
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// Callback when disk state has changed
// ---------------------------------------------------------------------------
//
void CMPXMediaRemovalMonitor::HandleNotifyDisk( TInt aError, const TDiskEvent& aEvent )
    {
    MPX_DEBUG4("-->CMPXMediaRemovalMonitor::HandleNotifyDisk aError=%d event=%d drive=%d", aError, aEvent.iType, aEvent.iDrive);
	  if ( aError == KErrNone )
	      {
	      if ( aEvent.iType == EDiskRemoved
             || aEvent.iType == EDiskStatusChanged && aEvent.iInfo.iType == EMediaNotPresent )
	          {
	          if ( aEvent.iDrive == iDrive && !iDiskRemoved )
                {
                iObserver.HandleSystemEventL( EDiskRemovedEvent, iDrive );
                iDiskRemoved = ETrue;
                }
        
	          }
	      else if ( ( aEvent.iType == EDiskAdded || aEvent.iType == EDiskStatusChanged ) 
                  && aEvent.iInfo.iType != EMediaNotPresent )
	          {
            if ( aEvent.iDrive == iDrive 
            	   && iDiskRemoved 
                 && ( aEvent.iInfo.iMediaAtt & ( KMediaAttLockable|KMediaAttLocked|KMediaAttHasPassword ) ) 
                    != ( KMediaAttLockable|KMediaAttLocked|KMediaAttHasPassword ) )
                {
                iObserver.HandleSystemEventL( EDiskInsertedEvent, iDrive );
                iDiskRemoved = EFalse;
                }
                
            if ( !( aEvent.iInfo.iDriveAtt & KDriveAttRemote ) ) 
                {
                TInt error = iDiskNotifyHandler->NotifyDismount( aEvent.iDrive );
                MPX_DEBUG3("CMPXMediaRemovalMonitor::HandleNotifyDisk monitoring drive %d for dismounts, error %d", aEvent.iDrive, error);
                }
	          }
        }
            
    MPX_DEBUG1("<--CMPXMediaRemovalMonitor::HandleNotifyDisk");
    }   

    
// ---------------------------------------------------------------------------
// Callback when disk is about to be dismounted
// ---------------------------------------------------------------------------
//
void CMPXMediaRemovalMonitor::HandleNotifyDismount( TInt aError, const TDismountEvent& aEvent )
    {
    MPX_DEBUG3("-->CMPXMediaRemovalMonitor::HandleNotifyDismount aError=%d drive=%d", aError, aEvent.iDrive);
    if (aError == KErrNone)
        {
        TRAP_IGNORE( iObserver.HandleSystemEventL( EDiskDismountEvent, aEvent.iDrive ) );
        TInt error = iDiskNotifyHandler->AllowDismount( aEvent.iDrive );
        MPX_DEBUG3("CMPXMediaRemovalMonitor::HandleNotifyDismount allowed dismount of drive %d, error %d", aEvent.iDrive, error);
        }
    MPX_DEBUG1("<--CMPXMediaRemovalMonitor::HandleNotifyDismount");
    }
