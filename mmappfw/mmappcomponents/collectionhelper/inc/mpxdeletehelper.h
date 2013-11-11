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
* Description:  Delete helper
*
*/


#ifndef C_MPX_DELETE_HELPER_H
#define C_MPX_DELETE_HELPER_H

#include <e32base.h>
#include <badesca.h>
#include <thumbnailmanager.h>
#include <thumbnailmanagerobserver.h>
#include <mpxcollectionobserver.h>
#include <mpxharvesterutilityobserver.h>
#include <mpxmessage2.h>
#include <usbman.h>

// FORWARD DECLARATIONS
class MMPXCollectionUtility;
class MMPXPlaybackUtility;
class MMPXHarvesterUtility;
class MMPXCHelperObserver;
class CMPXCollectionPath;


/**
 *  Delete Helper
 *
 *  @lib mpxcollectionhelper.lib
 *  @since S60 3.0
 */
NONSHARABLE_CLASS( CMPXDeleteHelper ) : public CActive,
                            public MThumbnailManagerObserver
    {
public:

    /**
    * Two-Phased Constructor
    * @param aCollectionUtility reference to a collection utility
    * @param aHarvesterUtility reference to a harvester utility
    * @param aObserver observer to callback upon completion of the operation
    * @return new instance of CMPXDeleteHelper
    */
    static CMPXDeleteHelper* NewL(MMPXCollectionUtility& aCollectionUtility,
                                  MMPXHarvesterUtility& aHarvesterUtility,
                                  MMPXCHelperObserver& aObserver);

    /**
    * Virtual Destructor
    */
    virtual ~CMPXDeleteHelper();

public: // New functions

    /**
    * Delete all items in a collection path
    * @param aPath, collection path to delete
    */
    void DeleteL( CMPXCollectionPath& aPath );

    /**
    * Handles any collection message callbacks from collection helper
    */
    void HandleCollectionMessage(CMPXMessage* aMessage, TInt aErr);

    /**
     * Stop deleting
     * 
     */
    void Stop();
    
private:

    /**
    *  From CActive
    *  Handles an active object's request completion event
    */
    void RunL();

    /**
    * From CActive
    * Implements cancellation of an outstanding request.
    */
    void DoCancel();

    /**
    * Performs incremental delete operation
    */
    void DoTaskStep();

    /**
    * Performs incremental delete operation
    */
    void DoTaskStepL();

    /**
    * Completes delete operation request and notifies the client
    */
    void CompleteDelete( TInt aErr );

    /**
    * Finish the delete operation
    * @param aErr error
    */
    void DoCompleteDeleteL( TInt aErr );

    /**
    * Resets members to get ready for the next client request
    */
    void Reset();

    /**
    * Retrieves a list of file URIs associated with the path
    */
    void RetrieveFileListL();

    /**
     * Perfoms cleanup of all unused data before delete
     */
     void StartDeleteL();
    
    /**
    * Deletes one file from the file system then removes it from the
    * collection
    */
    void DeleteL();

    /**
    * Deletes the file from the file system and harvester's database
    * @return ETrue if the file has been deleted from the file system;
    *         otherwise EFalse.
    */
    TBool DeleteFileL();
	
    /**
     * Connect to usbman.
     */
    void ConnectUsbMan();

    // MThumbnailManagerObserver for getting thumbnails
    void ThumbnailPreviewReady(
        MThumbnailData& aThumbnail, TThumbnailRequestId aId );
        
    void ThumbnailReady(
        TInt aError, 
        MThumbnailData& aThumbnail, TThumbnailRequestId aId );
    
private:

    /**
    * Constructor
    * @param aCollectionUtility reference to a collection utility
    * @param aHarvesterUtility reference to a harvester utility
    * @param aObserver observer to callback upon completion of the operation
    */
    CMPXDeleteHelper(MMPXCollectionUtility& aCollectionUtility,
                     MMPXHarvesterUtility& aHarvesterUtility,
                     MMPXCHelperObserver& aObserver);

    /**
    * 2nd phase constructor
    */
    void ConstructL();

 private:

    /*
    * state for deleting one item
    */
    enum TMPXDeleteState
        {
        EMPXIdle,
        EMPXInitDelete,
        EMPXPreparation,
        EMPXDelete
        };

private: // data
    MMPXCollectionUtility&    iCollectionUtil;
    MMPXHarvesterUtility&     iHarvester;
    MMPXCHelperObserver&      iObserver;

    MMPXPlaybackUtility*      iPbUtil;

    CMPXCollectionPath*       iDeletePath;
    CDesCArray*               iFiles;

    TBool                     iMoreToDo;
    TBool                     iHadInUse;
    TBool                     iCancelled;

    TMPXDeleteState           iState;

    CMPXMessageArray*         iMessageArray;
    TInt                      iItemsCount;
    TInt                      iRetrievedItemsCount;
    TInt                      iDeletePercent;
    TInt                      iDeleteIncFactor;
    TInt                      iDeleteCount;
    RFs                       iFs;           // FsSession (owned)
    CThumbnailManager*        iTNManager;
    RUsb                      iUsbMan;
    TBool                     iUsbManConnected;

    };

#endif // C_MPX_DELETE_HELPER_H
