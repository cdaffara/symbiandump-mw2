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
* Description:  MPX Low Disk Space Watcher class
*
*/


#ifndef CMPXDISKWATCHER_H
#define CMPXDISKWATCHER_H

#include <f32file.h>

class MMPXDiskSpaceObserver;

/**
 *  Low disk space watcher for monitoring disk space
 *
 *  @lib mpxfilehandler.lib
 *  @since S60 3.0
 */
NONSHARABLE_CLASS( CMPXDiskSpaceWatcher ) : public CActive
    {
public:

    /**
    * Two-phase constructor
    * @param aFs File system session
    * @param aDrive aDrive to monitor
    * @param aObs Observer to the monitor class
    */
    static CMPXDiskSpaceWatcher* NewL( RFs& aFs, TInt aDrive,
                                       MMPXDiskSpaceObserver& aObs );

    /**
    * destructor
    */
    virtual ~CMPXDiskSpaceWatcher();

    /**
    * Start monitoring for low disk events
    */
    void StartL();

    /**
    * Check if we are low on disk space
    * @return ETrue if low on disk
    */
    TBool IsLowOnDisk();

    /**
     * Gets the currently monitored drive number
     *
     * @return Currectly monitored drive number
     */
     TInt CurrentDrive();

protected:

    /**
    * Handle cancelling of active object request
    */
    void DoCancel();

    /**
    * Handle a change in event
    */
    void RunL();

private:

    /**
    * Default Constructor
    */
    CMPXDiskSpaceWatcher( RFs& aFs, TInt aDrive,
                          MMPXDiskSpaceObserver& aObs );

    /**
    * Standard two-phase constructor
    */
    void ConstructL();

private: // data
    RFs& iFs;             // Not owned
    TInt iDriveToMonitor; // Drive to monitor
    MMPXDiskSpaceObserver& iObs;
    };

#endif //CMPXDISKWATCHER_H
