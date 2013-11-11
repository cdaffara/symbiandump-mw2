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
* Description:  Scans metadata for new files, updated files
*
*/


#ifndef CMPXMETADATASCANNER_H
#define CMPXMETADATASCANNER_H

#include <e32base.h>
#include <mpxmetadataextractorobserver.h>

class CMPXMedia;
class CMPXMediaArray;
class CMPXCollectionType;
class CMPXMetadataExtractor;
class MMPXMetadataScanObserver;
class MMPXFileScanStateObserver;
 
enum TExtractType
    {
    ENewFiles = 0,
    EModFiles = 1,
    EMaxFile  = 2
    };

/**
 *  CMPXMetadataScanner
 *
 *  Extracts metadata from a list of files
 *
 *  @lib mpxfilehandler.lib
 *  @since S60 3.0
 */
NONSHARABLE_CLASS( CMPXMetadataScanner ): public CActive,
                                          public MMPXMetadataExtractorObserver
    {
public:

    /**
    * Two phased constructor
    * @param aFs file server session
    * @param aAppArc apparc session
    * @param aTypesAry supported file types array
    * @param aObs metadata scanning observer
    * @param aStateObs scanning state observer
    * @return new instance of CMPXMetadataScanner
    */
    static CMPXMetadataScanner* NewL( RFs& aFs, 
                                      RApaLsSession& aAppArc,
                                      RPointerArray<CMPXCollectionType>& aTypesAry,
                                      MMPXMetadataScanObserver& aObs,
                                      MMPXFileScanStateObserver& aStateObs );

    /**
    * Two phased constructor
    * @param aFs file server session
    * @param aAppArc apparc session
    * @param aTypesAry supported file types array
    * @param aObs metadata scanning observer
    * @param aStateObs scanning state observer
    * @return new instance of CMPXMetadataScanner
    */
    static CMPXMetadataScanner* NewLC( RFs& aFs,
                                       RApaLsSession& aAppArc, 
                                       RPointerArray<CMPXCollectionType>& aTypesAry,
                                       MMPXMetadataScanObserver& aObs,
                                       MMPXFileScanStateObserver& aStateObs );

    /**
    * Virtual destructor
    */
    virtual ~CMPXMetadataScanner();

public: // New Functions 

    /**
    * Frees all memory allocated
    */
    void Reset();
    
    /**
    * Extracts metadata for the files in the file list
    */
    void Start();

    /**
    * Stops extraction of metadata for the files in the file list
    */
    void Stop();

    /**
    * Adds a new file to the scanning list
    * @param aFile file path to add to scan
    */
    void AddNewFileToScanL( const TDesC& aFile );
    
    /**
    * Adds a modified file to the scanning list
    * @param aFile file path to add to scan
    */
    void AddModifiedFileToScanL( const TDesC& aFile );
    
    /**
    * Extract the Media properties for a file
    * @param aFile file path to extract metadata
    * @return CMPXMedia* media for the file, ownership transferred
    */
    CMPXMedia* ExtractFileL( const TDesC& aFile );
    
protected: // From base class
    
    /**
    * From CActive
    */
    void DoCancel();
    
    /**
    * From CActive
    */
    void RunL();
    
    /**
     * From MMPXMetadataExtractorObserver
     */
    void HandleCreateMediaComplete( CMPXMedia* aMedia, TInt aError );

private: // New Functions
    
    /**
    * Extract metadata from a few files
    */
    void DoExtractL();
    
    /**
     * Get source array
     */
    RPointerArray<HBufC>* GetSource();
    
    /**
     * Is metadata scanner done
     */
    TBool IsDone();
    
    /**
     * Run again
     */
    void RunAgain();
    
    /**
     * Add metadata to collection
     */
    void AddToCollectionL();
    
    /**
     * Complete metadata scanner
     */
    void MetadataScannerComplete( TInt aError );
        
private:

    /**
    * Private Constructor
    * @param aObs metadata scanning observer
    * @param aStateObs scanning state observer
    */
    CMPXMetadataScanner( MMPXMetadataScanObserver& aObs,
                         MMPXFileScanStateObserver& aStateObs );
    
    /**
    * 2nd phase constructor
    * @param aAppArc apparc session
    * @param aTypesAry supported file types array
    */
    void ConstructL( RFs& aFs, RApaLsSession& aAppArc,
                     RPointerArray<CMPXCollectionType>& aTypesAry );

private: // data
    RPointerArray<HBufC>  iNewFiles;
    RPointerArray<HBufC>  iModifiedFiles;
 
    CMPXMediaArray* iTargetProps;
    
    TBool iExtracting;          // Are we extracting
    TInt  iExtractType; // What are we extracting
    TInt  iAryPos;              // Current array position
    
    CMPXMetadataExtractor* iExtractor;  // Metadata Utilities wrapper
    
    MMPXMetadataScanObserver& iObserver;
    MMPXFileScanStateObserver& iStateObserver;
    };

#endif // CMPXMETADATASCANNER_H
