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
* Description:  Extracts metadata from a file
*
*/


#ifndef CMPXMETADATAEXTRACTOR_H
#define CMPXMETADATAEXTRACTOR_H

#include <thumbnailmanager.h>
#include <thumbnailmanagerobserver.h>

// FORWARD DECLARATION
class CMPXMedia;
class CMPXCollectionPath;
class CMPXFileInfoUtility;
class CMPXDrmMediaUtility;
class MMPXMetadataExtractorObserver;
class CMetaDataUtility;
class CMetaDataFieldContainer;

/**
 *  Extracts metadata from a file.
 *
 *  @lib mpxfilehandler.lib
 */
class CMPXMetadataExtractor : public CBase,
                              public MThumbnailManagerObserver
    {
    
public:
    /*
    * Two-Phased Constructor.
    	*@since 3.2.3
    */
    IMPORT_C static CMPXMetadataExtractor* NewL( RFs& aFs, 
                                                 RApaLsSession& aAppArc,
                                                 RPointerArray<CMPXCollectionType>& aTypes );

    /*
    * Virtual Destructor.
    	*@since 3.2.3
    */
    virtual ~CMPXMetadataExtractor();

public: // New Functions
    
    /*
    * Create a media object for a file.
    * @since 3.2.3
    * @param aFile a fullpath to the file.
    * @param aNewProperty on return a new media property will be created.
    * @param aMetadataOnly extract metadata only or not. Default EFalse
    */
    IMPORT_C void CreateMediaL( const TDesC& aFile,
                                CMPXMedia*& aNewProperty,
                                TBool aMetadataOnly = EFalse );

    /**
     * Extract album art from a file and add to thumbnail manager.
     * @since 5.0
     * @param aMedia MPX Media object to add album art attribute.
     * @return error id 
     */
    IMPORT_C TInt ExtractAlbumArtL( CMPXMedia* aMedia );
    
    /*
    * Create a media object for a file. This is a asynchronous function.
    * This function will leave with KErrAbort if still processing last request.
    * Callback function is HandleCreateMediaComplete()
    * @since 9.2
    * @param aFile a fullpath to the file.
    * @param aObs Metadata Extractor Observer
    * @param aMetadataOnly extract metadata only or not. Default EFalse
    */
    IMPORT_C void CreateMediaAsyncL( const TDesC& aFile,
                                     MMPXMetadataExtractorObserver* aObs,
                                     TBool aMetadataOnly = EFalse );

    /**
     * Cancel request.  This will empty the task array and stop the wait loop. This
     *                  will cause the CreateMediaL() to finish more quickly. Also,
     *                  all outstanding thumbnail requests are cancelled.
     * @since 9.2
     */
    IMPORT_C void CancelRequest();
    
private: // New Functions:
    
    /**
    * Set default media properties.
    * @since 3.2.3
    * @param aMediaProp media properties to update
    */
    void SetDefaultL( CMPXMedia& aMediaProp );
    
    /**
    * Function to go through the metadata entries.
    * @since 3.2.3
    */
    void SetMediaPropertiesL();
    
    /**
    * Sets other media properties not found from metadata util.
    * @since 3.2.3
    */
    void SetExtMediaPropertiesL();   
                                
    /**
    * Checks to see if a container type is supported.
    * @since 3.2.3
    * @param aFile file path.
    * @param aIndex index to the supported types array.
    * @return index to the mimetype
    */ 
    TInt SupportedContainerTypeL( const TDesC& aFile, TInt aIndex );

    // From MThumbnailManagerObserver 
    /**
    * Preview thumbnail generation or loading is complete.
     * @since S60 v5.0
     * @param aThumbnail     An object representing the resulting thumbnail.
     * @param aId            Request ID for the operation
    */
    void ThumbnailPreviewReady(
        MThumbnailData& aThumbnail, TThumbnailRequestId aId );
    
    /**
     * Final thumbnail bitmap generation or loading is complete.
     * @since S60 v5.0
     * @param aError         Error code.
     * @param aThumbnail     An object representing the resulting thumbnail.
     * @param aId            Request ID for the operation.
    */
    void ThumbnailReady(
        TInt aError, 
        MThumbnailData& aThumbnail, TThumbnailRequestId aId );

    /**
    * Stop the wait loop.
    */
    void StopWaitLoop();

    /**
     * Cancel timer. 
     */
    void CancelTimeoutTimer();
    
    /**
    * Callback when the timer times out.
    */
    static TInt TimeoutTimerCallback(TAny* aPtr);

    /**
     * Add album art to media object.
     * @since 5.0
     * @param aMedia
     */
    void AddMediaAlbumArtL( CMPXMedia& aMedia,
                            const TDesC& aFile );
    
    /**
     * Check if can send request to TNM or not.
     * @since 5.0
     * 
     */                     
    void CheckBeforeSendRequest();
    
    /**
     * Cancel all outstanding thumbnail requests
     * @since 9.2
     * 
     */                     
    void CancelAllThumbnailRequests();
    
    /**
     * Create media and set default data.
     * @since 9.2
     * 
     */                     
    void DoCreateMediaL();
    
    /**
     * Execute task at index 0.
     * @since 9.2
     * 
     */                     
    void ExecuteTaskL();
    
    /**
     * Cancel task timer. 
     */
    void CancelTaskTimer();
    
    /**
    * Callback when the task timer expires.
    */
    static TInt TaskTimerCallback(TAny* aPtr);

    /**
     * Populate task array
     * @since 9.2
     */
    void AddTasksL();
    
    /**
     * Opens the file
     * @since 9.2
     * @return system error
     */
    TInt OpenFile();
    
    /**
     * Handle task timer expired
     * @since 9.2 
     */
    void HandleTaskTimerExpired();
    
    /**
     * Clean up
     * @since 9.2 
     */
    void CleanUp();
        
    /**
     * Sets drm media properties
     * @since 9.2 
     */
    void SetDrmMediaPropertiesL(const CMetaDataFieldContainer& aMetaCont);

    
private:

    /**
    * Private Two-Phased constuctor.
    * @since 3.2.3
    * @param aFs file path.
    * @param aAppArc address for the session application information.
    * @param array of supported types
    */ 
    CMPXMetadataExtractor( RFs& aFs, RApaLsSession& aAppArc,
                           RPointerArray<CMPXCollectionType>& aTypes );

    void ConstructL();

private: // data
    enum EMetadataExtractorTasks
        {
        ETaskCreateMedia,
        ETaskAddMetadata,
        ETaskAddExtMetadata,
        ETaskAddAlbumArt,
        ETaskCheckBeforeSend
        };
    
    CMetaDataUtility*    iMetadataUtility;  // extract metadata from file
    CMPXDrmMediaUtility* iDrmMediaUtility;  // extra drm data from file
    CMPXFileInfoUtility* iFileInfoUtil;     // extract duration/bitrate etc from file
    RFs&                 iFs;
    RApaLsSession&       iAppArc;
    RPointerArray<CMPXCollectionType>& iSupportedTypes;  // not owned
    
    CThumbnailManager*    iTNManager;
    CActiveSchedulerWait* iTNSyncWait;  // wait loop use to sync thumbnail
    CPeriodic*            iTimer; // backup timer to stop wait loop
    RArray<TThumbnailRequestId>     iArrayTNRequestId;
    RArray<EMetadataExtractorTasks> iArrayTasks;
    TBool                           iCancelled;
    CPeriodic*                      iTaskTimer; // timer for task execution
    MMPXMetadataExtractorObserver*  iObs; // metadata extractor obserer
    TFileName                       iFileName;
    RFile                           iFile;
    CMPXMedia*                      iMedia;  // ownership transferred
    TBool                           iMetadataOnly;
    TInt                            iFileOpenError;
    };

#endif // CMPXMETADATAEXTRACTOR_H
