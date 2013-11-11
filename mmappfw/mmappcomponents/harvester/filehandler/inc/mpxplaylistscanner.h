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
* Description:  Class to handle playlist parsing
*
*/


#ifndef CMPXPLAYLISTSCANNER_H
#define CMPXPLAYLISTSCANNER_H

#include <e32base.h>
#include <mpxplaylistengineobserver.h>

// FOWARD DECLARATIONS
class MMPXPlaylistScanObserver;
class MMPXFileScanStateObserver;
class CMPXMedia;
class CMPXMediaArray;
class CMPXPlaylistEngine;
class CMPXCollectionType; 

/**
 *  CMPXPlaylistScanner
 *  Playlist extraction class
 *
 *  @lib mpxfilehandler.lib
 *  @since S60 v3.0
 */
NONSHARABLE_CLASS( CMPXPlaylistScanner ): public CBase,
                                          public MMPXPlaylistEngineObserver
    {

public:

    /**
    * Two-phase constructor
    * @param aObs observer to the playlist scan
    * @param aStateObs observer to state changes in scanning
    * @param aTypes supported collection files
    */
    static CMPXPlaylistScanner* NewL( MMPXPlaylistScanObserver& aObs,
                                      MMPXFileScanStateObserver& aStateObs,
                                      RPointerArray<CMPXCollectionType>& aTypes );

    /**
    * Destructor
    */
    virtual ~CMPXPlaylistScanner();

public: 
    
    /**
    * Initiates the scanning of the list of playlists 
    */
    void ScanL(); 
    
    /**
    * Cancels scanning
    */
    void Cancel();
    
    /**
    * Resets any internal variables
    */
    void Reset();
    
    /**
    * Add a file to the list for scanning
    * @param aFile file to add to the new scan list
    */
    void AddPlaylistToScanL( const TDesC& aFile );
    
    /**
    * Add a file to the list for update
    * @param aFile file to add to the update scan list
    */
    void AddUpdatedPlaylistToScanL( const TDesC& aFile );
    
    /**
    * Is aFile a playlist file?
    * @param aFile, file to check
    * @return ETrue if the file is a playlist, EFalse otherwise
    */
    TBool IsPlaylistFileL( const TDesC& aFile );
     
protected:

    /**
    * From MMPXPlaylistEngineObserver
    */
    void HandlePlaylistL(CMPXMedia* aPlaylist,
                         const TInt aError,
                         const TBool aCompleted );

    /**
    * From MMPXPlaylistEngineObserver
    */
    void HandlePlaylistL(const TDesC& aPlaylistUri,
                         const TInt aError);    

private:

    /**
    * Adds/Updates files to the collection
    */
    void CompleteFilesL();
        
private:

    /**
    * Constructor
    * @param aObs observer to the playlist scan
    * @param aStateObs observer to state changes in scanning
    * @param aTypes supported collection files
    */
    CMPXPlaylistScanner( MMPXPlaylistScanObserver& aObs,
                         MMPXFileScanStateObserver& aStateObs,
                         RPointerArray<CMPXCollectionType>& aTypes );
    
    /**
    * 2nd Phase constructor
    */
    void ConstructL();

private: // data
    MMPXPlaylistScanObserver&  iObserver;
    MMPXFileScanStateObserver& iStateObserver; 
    TBool                      iExtracting;            // is this object busy?
    TInt                       iNewPlPending;
    TInt                       iUpdatePlPending;
    
    CDesCArray*                iPlList;  // list of playlist files to scan
    CDesCArray*                iPlUpdateList; // list of updated playlists;
    
    CMPXMediaArray* iPropArray;
    CMPXMediaArray* iUpdatePropArray;
    
    CMPXPlaylistEngine*        iPlEngine;
    RPointerArray<CMPXCollectionType>& iSupportedTypes;  // not owned
    };


#endif // CMPXPLAYLISTSCANNER_H
