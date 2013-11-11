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
* Description:  Monitors for media removal events
*
*/


#ifndef CMPXMEDIAREMOVALMONITOR_H
#define CMPXMEDIAREMOVALMONITOR_H

#include <e32base.h>
#include <disknotifyhandler.h>
#include "mpxsystemeventobserver.h"

/**
 *  Class to monitors for File System dismount events
 * 
 *  The monitor emits events to MMPXSystemEventObserver
 * 
 *     EDiskDismountEvent whenever a drive is about to be dismounted (see RFs::NotifyDismount).
 *     EDiskRemovedEvent  whenever a specified drive has been dismounted or removed
 *     EDiskInsertedEvent whenever a specified drive has been inserted
 *
 *  The observer is expected to prepare the drive for dismounting during the EDiskDismountEvent.
 *
 *  Dismount monitoring is automatically subscribed for all non-remote drives.
 *  Inserted/Removed monitor is subscribed only for the drive given in the constructor.
 *
 *  @lib Harvester
 *  @since S60 3.0
 */
class CMPXMediaRemovalMonitor : public CBase, 
                                public MDiskNotifyHandlerCallback
    {

public:

    /**
    * Two-phase constructor
    * @param aDrive drive to monitor for insertions and removals
    * @param aFs file server session
    * @param aObserver observer to the event
    */
    static CMPXMediaRemovalMonitor* NewL( TInt aDrive, 
                                          RFs& aFs,
                                          MMPXSystemEventObserver& aObserver );
    /**
    * Two-phase constructor
    * @param aDrive drive to monitor
    * @param aFs file server session
    * @param aObserver observer to the event
    */
    static CMPXMediaRemovalMonitor* NewLC( TInt aDrive, 
                                           RFs& aFs,
                                           MMPXSystemEventObserver& aObserver );

    /**
    * Virtual destructor
    */
    virtual ~CMPXMediaRemovalMonitor();
    
    /**
    * Check the status of the monitored disks
    *
    * This check should be done whenever there is a risk that we got
    * a NotifyDismount even though the drives actually weren't dismounted.
    * 
    */
    void CheckDriveStatus();

protected: // From Base Class

    /**
    * From MDiskNotifyHandlerCallback
    */
    void HandleNotifyDismount( TInt aError, const TDismountEvent& aEvent );
    void HandleNotifyDisk( TInt aError, const TDiskEvent& aEvent );

private:

    /**
    *  C++ constructor
    * @param aDrive drive to monitor
    * @param aFs file server session
    * @param aObserver observer to the event
    */
    CMPXMediaRemovalMonitor( TInt aDrive,
                             RFs& aFs,  
                             MMPXSystemEventObserver& aObserver );

    void ConstructL();

private: // data
    
    CDiskNotifyHandler*  iDiskNotifyHandler;  // Disk notification handler
    TInt iDrive;        // Drive that is being monitored
    RFs& iFs;           // File Session, not owned
    TBool iDiskRemoved; // Is the disk inserted
    
    /*
    * Observer interface to callback to an observer
    */
    MMPXSystemEventObserver& iObserver;
    };

#endif // CMPXMEDIAREMOVALMONITOR_H