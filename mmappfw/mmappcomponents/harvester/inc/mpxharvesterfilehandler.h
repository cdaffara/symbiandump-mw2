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
* Description:  File handler class to handle file scanning related events
*
*/


#ifndef CMPXHARVESTERFILEHANDLER_H
#define CMPXHARVESTERFILEHANDLER_H

#include <e32base.h>
#include <f32file.h>
#include <bamdesca.h>
#include <mpxharvestercommon.h>

// Foward Declaration
class CMPXMedia;

/**
 *  File handler class to handle file scanning related events
 *
 *  @lib mpxfilehandler.lib
 *  @since S60 3.0
 */
class CMPXHarvesterFileHandler : public CBase

    {
public:

    /**
    * Two-Phased Constructor
    * @param aFs file server session
    */
    IMPORT_C static CMPXHarvesterFileHandler* NewL( RFs& aFs );

    /**
    * Virtual destructor
    */
    virtual ~CMPXHarvesterFileHandler();

    /**
    * Scan all drives for files
    */
    virtual void ScanL() = 0;

    /**
    * Cancel the scanning operation
    */
    virtual void CancelScan() = 0;

    /**
    * Handles a system event
    * @param aEvent, the event
    * @param aData, event specific data
    */
    virtual void HandleSystemEventL( TSystemEvent aEvent,
                                     TInt aData ) = 0;

    /**
    * Add a file to the db
    * @param aPath File to add
    * @return CMPXMedia object for the item pointed to by aPath
    *         ownership transferred
    */
    virtual CMPXMedia* AddFileL( const TDesC& aPath ) = 0;

    /**
    * Add a file to the db
    * @param aMediaProp Media Property to add
    * @return TInt collection ID for the item added
    */
    virtual TInt AddFileL( CMPXMedia& aMediaProp ) = 0;

    /**
    * Remove a file from the db
    * @param aPath, File to remove
    * @return Collection ID of the removed file
    */
    virtual TInt RemoveFileL( const TDesC& aPath, TBool aEndTransaction ) = 0;

    /**
    * Remove multiple files from the db
    * @param aArray, array of files to remove
    */
    virtual void RemoveFilesL( const MDesCArray& aFilePaths ) = 0;

    /**
    * Remove all files from the db
    */
    virtual void RemoveAllFilesL() = 0;

    /**
    * Update a file with a new collection and time stamp
    * @param aFile, file path
    * @param aCollection, Collection ID (UID)
    */
    virtual void UpdateFileL( const TDesC& aFile, TInt aCollection ) = 0;

    /**
    * Rename a file
    * @param aOldPath, the existing file path
    * @param aNewPath, the new file path
    * @param aCollection, Collection ID (UID)
    */
    virtual void RenameFileL( const TDesC& aOldPath, const TDesC& aNewPath, TInt aCollection ) = 0;

    /**
    * Find the associated collection ID with a file
    * @return TInt containing the associated collection ID for a file
    */
    virtual TInt FindCollectionIdL( const TDesC& aFile ) = 0;

    /**
    * Recreate the harvester database
    * (To handle corruption )
    */
    virtual void RecreateDatabases() = 0;

    /**
    * Close harvester database transaction
    */
    virtual void CloseTransactionL() = 0;

    /**
    * Get a media object for the file
    * @param aPath File to add
    * @return CMPXMedia object for the item pointed to by aPath
    *         ownership transferred
    */
    virtual CMPXMedia* GetMediaForFileL( const TDesC& aPath ) = 0;

    /**
    * Get Collection Uid for the file
    * @param aMediaProp Media Property to add
    * @return TInt collection ID for the item added
    */
    virtual TInt GetColUidForFileL( const TDesC& aPath ) = 0;
    };

#endif // CMPXHARVESTERFILEHANDLER_H
