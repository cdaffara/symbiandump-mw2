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
* Description:  Scans folders for media files
*
*/


#ifndef CMPXFOLDERSCANNER_H
#define CMPXFOLDERSCANNER_H

#include <e32base.h>
#include <f32file.h>
#include <badesca.h>

// FORWARD DECLARATIONS
class MMPXFileAdditionObserver;
class MMPXFileScanStateObserver;
 
/**
 *  CMPXFolderScanner
 *
 *  Scans a particular drive for files
 *
 *  @lib mpxfilehandler
 *  @since S60 3.0
 */
NONSHARABLE_CLASS( CMPXFolderScanner ) : public CActive
    {

public:

    /**
    * Two-phased constructor
    * @param aObserver file addition observer
    * @param aStateObserver scanning state observer
    * @param aFs file session
    */
    static CMPXFolderScanner* NewL( MMPXFileAdditionObserver& aObserver,
                                    MMPXFileScanStateObserver& aStateObs,
                                    RFs& aFs );

    /**
    * Virtual destructor
    */
    virtual ~CMPXFolderScanner();

    /**
    * Scan a particular drive for files
    * @param aDrive the drive to scan
    */
    void ScanL( RArray<TPath>& aDrive );

protected: // From Base Class

    /**
    * From CActive
    */
    void RunL();
    
    /**
    * From CActive
    */
    void DoCancel();
    
    /**
    *  From CActive
    */
    TInt RunError(TInt aError);    

private: // New Functions

    /**
    * Continues the scanning procedure
    * @return ETrue if it is done, EFalse if there is more work to do
    */
    TBool DoScanL();    
    
    /**
    * Setup the object to scan the next drive
    * @return ETrue if there are more folders to scan
    *         EFalse if the scanning process is complete
    */
    TBool SetupNextDriveToScanL();
    
    /**
    * Handles what to do when scanning is done
    * @param aErr error to return to the client
    */
    void DoScanCompleteL( TInt aErr );

    /**
    * Execute asynchronous reading RDir
    */
    void ReadDirEntry();

private:

    /**
    * Private constructor
    * @param aObserver file addition observer
    * @param aStateObserver scanning state observer
    * @param aFs file session
    */
    CMPXFolderScanner( MMPXFileAdditionObserver& aObserver,
                       MMPXFileScanStateObserver& aStateObs,
                       RFs& aFs );

    /**
    * 2nd Phase constructor
    */
    void ConstructL();

private: // data

    /**
     * An entry delegates a scanning folder
     */
    class CDirQueueEntry : public CBase
        {
        public:

            /**
             * Push a scanning folder into a directory queue
             * @param aDirQueue directory queue
             * @param aDirEntry entry delegates a scanning folder
             */
            static void PushL( RPointerArray< CDirQueueEntry >& aDirQueue,
                               CDirQueueEntry* aDirEntry );

            /**
             * Pop and destroy a scanning folder from a directory queue
             * @param aDirQueue directory queue
             */
            static void PopAndDestroy( RPointerArray< CDirQueueEntry >& aDirQueue );

            /**
             * Two-phased constructor
             * @param aFullPath full path of this folder
             */
            static CDirQueueEntry* NewL( const TDesC& aFullPath );

            /**
             * Virtual destructor
             */
            virtual ~CDirQueueEntry();

            /**
             * Get next entry of this folder
             * @return next entry of this folder,
             *         NULL if no next entry
             */
            const TEntry* NextEntry();

            /**
             * Reset the position of the EntryArray
             */
            void ResetPosition();

        private:

            /**
             * Private constructor
             */
            CDirQueueEntry();

            /**
             * 2nd phase constructor
             * @param full path of this folder
             */
            void ConstructL( const TDesC& aFullPath );

        public: //data

            /**
             * Full path of this folder
             * Owned
             */
            HBufC* iFullPath;

            /**
             * Current position of EntryArray
             */
            TInt iPos;

            /**
             * RDir object of this folder
             * Owned
             */
            RDir iDir;

            /**
             * EntryArray of this folder
             */
            TEntryArray iEntryArray;
        };

    MMPXFileAdditionObserver&     iObserver; 
    MMPXFileScanStateObserver&    iStateObserver; 
    
    RFs&  iFs; // Not owned
    
    // Droves or folders that we want to scan
    RArray<TPath>                 iDrivesToScan;
    
    // Flag to indicate if we are scanning or not
    TBool                         iScanning;
    
    // Scanner objects
    RPointerArray< CDirQueueEntry > iDirQueue; // Owned
    CDirQueueEntry*                 iCurDirQueueEntry; // Not Owned
    HBufC*                          iCurFullPath; // Not Owned
    };

#endif // CMPXFOLDERSCANNER_H
