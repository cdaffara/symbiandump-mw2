/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Low disk space observer for harvester server
*
*/


#ifndef MDSHARVESTERDISKSPACEOBSERVERAO_H
#define MDSHARVESTERDISKSPACEOBSERVERAO_H

// INCLUDE FILES
#include <e32base.h>
#include <f32file.h>

/**
* MMdSHarvesterDiskSpaceObserver
* Observer interface for a disk space notifier.
*/
class MMdSHarvesterDiskSpaceObserver
    {
    public :
        enum TDiskSpaceDirection
            {
            /** Disk space is larger than threshold level */
            EMore,

            /** Disk space is smaller than threshold level */
            ELess
            };

        /**
         * Called to notify the observer that disk space has crossed the specified threshold value.
         *
         * @param aCrossDirection threshold cross direction
         */
        virtual void HandleDiskSpaceNotificationL(TDiskSpaceDirection aDiskSpaceDirection) = 0;
    };

/**
* CMSDiskSpaceNotifierAO.
* A disk space notifier class
*/
class CMdSHarvesterDiskspaceObserverAO : public CActive
    {
    public:
        enum TDiskSpaceNotifierState
            {
            ENormal,
            EIterate
            };

    public : // Constructors and destructors
        /**
         * Constructs a disk space notifier implementation.
         *
         * @param aThreshold minimum free disk space threshold level in bytes
         * @param aFilename filename which defines monitored drive's number
         * @return  metadata server implementation
         */
        static CMdSHarvesterDiskspaceObserverAO* NewL(
            MMdSHarvesterDiskSpaceObserver& aObserver, 
            TInt64 aThreshold, const TDesC& aFilename);

        /**
         * Constructs a disk space notifier implementation and leaves it
         * in the cleanup stack.
         *
         * @param aThreshold minimum free disk space threshold level in bytes
         * @param aFilename filename which defines monitored drive's number
         * @return  metadata server implementation
         */
        static CMdSHarvesterDiskspaceObserverAO* NewLC(        
            MMdSHarvesterDiskSpaceObserver& aObserver, 
            TInt64 aThreshold, const TDesC& aFilename);

        /**
        * Destructor.
        */
        virtual ~CMdSHarvesterDiskspaceObserverAO();

    protected: // Functions from base classes
        /**
         * From CActive
         * Callback function.
         * Invoked to handle responses from the server.
         */
        void RunL();

        /**
         * From CActive
         * Handles errors that occur during notifying the observer.
         */
        TInt RunError(TInt aError);

        /**
         * From CActive
         * Cancels any outstanding operation.
         */
        void DoCancel();

    private: // Constructors and destructors

        /**
         * constructor
         */
        CMdSHarvesterDiskspaceObserverAO(
            MMdSHarvesterDiskSpaceObserver& aObserver,
            TInt64 aThreshold, TDriveNumber aDrive);

        /**
         * 2nd phase constructor
         * @param aThreshold minimum free disk space threshold level in bytes
         * @param aDrive monitored drive's number
         */
        void ConstructL();

    private: // New methods

        void StartNotifier();

        static TDriveNumber GetDriveNumberL( const TDesC& aFilename );

    private: // Data

        MMdSHarvesterDiskSpaceObserver& iObserver;
        
        RFs iFileServerSession;
        
        const TInt64 iThreshold;
        
        const TDriveNumber iDrive;
        
        TDiskSpaceNotifierState iState;
        
        TInt iIterationCount;
    };

#endif // MDSHARVESTERDISKSPACEOBSERVERAO_H

// End of File

