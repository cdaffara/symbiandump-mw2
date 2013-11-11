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
* Description:  Encapsulates db operations on a table
*
*/


#ifndef CMPXHARVESTERDBTABLE_H
#define CMPXHARVESTERDBTABLE_H

#include <e32base.h>
#include <d32dbms.h>

//#define __PRINTDB__

// Forward Declarations
//
class CMPXHarvesterDbItem;
       
/**
 *  CMPXHarvesterDatabaseTable
 *  Encapsulates operations on the database
 *
 *  @lib mpxfilehandler.lib
 *  @since S60 v3.0
 */
NONSHARABLE_CLASS( CMPXHarvesterDatabaseTable ): public CBase
    {

public:

    /**
    * Two-Phased Constructor
    * @param aDb database to create the table from
    * @return CMPXHarvesterDatabaseTable instance
    */
    static CMPXHarvesterDatabaseTable* NewLC( RDbStoreDatabase& aDB );
    
    /**
    * Virtual Destructor
    */
    virtual ~CMPXHarvesterDatabaseTable();

public: 

    /**
    * Open a table to list all files
    */
    void OpenAllFilesTableL();
    
    /**
    * Opens a table to a specific directory
    * @param aDirectory directory to list the files
    */ 
    void OpenDirectoryL( const TDesC& aDirectory );
    
    /**
    * Open a table to a specific song
    * @param aSongName song to open
    */
    void OpenItemL( const TDesC& aSongName );
    
    /**
    * Open a table to list of files with DRM flag set.
    * @param aSongName song to open
    */
    void OpenDrmL();
    
    /**
    * Add an item
    * @param aPath path of the file
    * @param aFileName file to add
    * @param aLastModTime last modified time
    * @param aDRM is the file a drm file
    */
    void AddItemL( const TDesC& aPath, const TDesC& aFileName, 
                   const TTime aLastModTime, const TInt aColId,
                   const TBool aDRM );
    
    /**
    * Updates an item
    * @param aPath, file path
    * @paran aLastModTime, last modified time
    * @param aColID, collection id
    * @param aDrm, is this file drm'd?
    * @param aNewTransaction create new transaction for operation
    */
    void UpdateItemL( const TDesC& aPath, const TTime aLastModTime,
                      const TInt aColID, const TBool aDRM, 
                      TBool aNewTransaction = ETrue);
    
    /**
    * Updates an item (don't care about drm)
    * @paran aLastModTime, last modified time
    * @param aColID, collection id
    * @param aNewTransaction create new transaction for operation
    * @param aPath, path of the item
    */
    void UpdateItemL( const TTime aLastModTime,
                      const TInt aColID = KNullUid.iUid,
                      TBool aNewTransaction = ETrue,
                      const TDesC& aPath = KNullDesC );
                      
    /**
    * Delete the current song
    */
    void DeleteItemL(TBool aEndTransaction);
    
    /**
    * Return the count of the current view
    */
    TInt CountL();
    
    /**
    * Generates a RPointerArray to represent the table
    * @return pointer array containing the list of items
    */
    RPointerArray<CMPXHarvesterDbItem>* CreateTableRepresentationL();
    
#ifdef __PRINTDB__
    /**
    * Debug function to print out all of the elements
    */
    void PrintItemsInTableL();
#endif

private: // New Functions
    
    /**
    * Read a long text string from db at the specified column
    * @param aColumn
    * @return string left on cleanup stack
    */
    HBufC* ReadLongTextLC( TInt aColumn );
    
    /**
    * Replaces single quotes with two single quotes
    * @param aSrc source descriptor
    * @param aTrg destination descriptor
    */
    void FindAndReplaceSingleQuote(const TDesC& aSrc, TDes& aTrg);
         
private:

    /**
    * Private Constructor
    * @param aDb database to create the table from
    */
    CMPXHarvesterDatabaseTable( RDbStoreDatabase& aDB );

private: 

    // Enumeration of what state this table is
    //
    enum TTableState
        {
        EInvalidState,
        EAllItems,   // Listing all items
        EDirectory,  // Listing only a directory
        EFile,       // Listing only a file
        EDrm         // Listing files with DRM flag
        };
     
private: // data
    RDbStoreDatabase&  iDb;
    RDbView            iView;
    TTableState        iState;
    };

#endif // CMPXHARVESTERDBTABLE_H
