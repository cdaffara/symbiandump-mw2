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
* Description:  Incremental OpenL() utility
*
*/


#ifndef C_CMPXCOLLECTIONOPENUTILITY_H
#define C_CMPXCOLLECTIONOPENUTILITY_H

#include <mpxcollectionobserver.h>
#include <mpxcollectionframeworkdefs.h>
#include <mpxcollectionopenlresultdef.h>

// FORWARD DECLARTIONS
class MMPXCollectionUtility;
class CMPXCollectionPath;

/**
 *  CMPXCollectionOpenUtility
 *
 *  Incremental OpenL() utility to open data in chunks
 *
 *  @lib mpxcollectionutility.lib
 */
class CMPXCollectionOpenUtility  : public CBase, 
                                   public MMPXCollectionObserver
    {
public:

    /**
     * Two phased constructor.
     *
     * @since S60 5.0
     * @param aObs observer
     * @param aMode mode of the utility 
     */
    IMPORT_C static CMPXCollectionOpenUtility* NewL( MMPXCollectionObserver* aObs, TUid aMode = KMcModeDefault );

    /**
     * virtual destructor.
     *
     * @since S60 5.0
     */
    virtual ~CMPXCollectionOpenUtility();

public:  // Enumerations
    /**
     * Direction of the fetch
     */
    enum TDirection
        {
        EFetchNormal,
        EFetchUp,
        EFetchDown
        };
        
public:  // Public functions
    
    /**
     * Start the incremental read operation.
     *
     * @since S60 5.0
     * @param aAttrs array of attributes
     * @param aChunkSize size of each chunk to fetch
     * @param aOffset offset to start fetching from
     * @param aDirection fetching direction, normal, up or down
     * @param aKeyAttribute key attribute to speed up open operation
     */
    IMPORT_C void StartL( TArray<TMPXAttribute> aAttrs,
                          TInt aChunkSize,
                          TInt aOffset = 0,
                          TDirection aDirection = EFetchNormal,
                          TMPXAttribute aKeyAttribute = KMPXMediaNullAttribute );
    
    /**
     * Start the incremental read operation.
     *
     * @since S60 5.0
     * @param aPath path to have to open to
     * @param aAttrs array of attributes
     * @param aChunkSize size of each chunk to fetch
     * @param aOffset offset to start fetching from
     * @param aDirection fetching direction, normal, up or down
     * @param aKeyAttribute key attribute to speed up open operation
     */
    IMPORT_C void StartL( const CMPXCollectionPath& aPath,
                          TArray<TMPXAttribute> aAttrs,
                          TInt aChunkSize,
                          TInt aOffset = 0,
                          TDirection aDirection = EFetchNormal,
                          TMPXAttribute aKeyAttribute = KMPXMediaNullAttribute );

    /**
     * Cancel the incremental reading procedure.
     *
     * @since S60 5.0
     */
    IMPORT_C void Stop();
    
    /**
     * Set the fetching direction.
     *
     * @since S60 5.0
     * @param aDirection a direction to start fetching from
     */
    IMPORT_C void SetDirection( TDirection aDirection );
    
    /**
     * Set the fetching delay in between subsequent fetches.
     *
     * @since S60 5.0
     * @param aDelay delay in microseconds, default is 0
     */
    IMPORT_C void SetDelay( TInt aDelay );
    
    /**
     * Returns a copy of the collection path.
     *
     * @since S60 5.0
     * @return CMPXCollectionpath* ownership transferred
     */
    IMPORT_C CMPXCollectionPath* PathL();

protected: // From Base Class
    
    /**
     * From MMPXCollectionObserver
     *
     * @since S60 5.0
     */
    void HandleCollectionMessage(CMPXMessage* aMsg, TInt aErr); 

    /**
     * From MMPXCollectionObserver
     *
     * @since S60 5.0
     */
    void HandleOpenL(const CMPXMedia& aEntries,
                     TInt aIndex,TBool aComplete,TInt aError);
    
    /**
     * From MMPXCollectionObserver
     *
     * @since S60 5.0
     */
    void HandleOpenL(const CMPXCollectionPlaylist& aPlaylist,TInt aError);
    
    /**
     * From MMPXCollectionObserver
     *
     * @since S60 5.0
     */
    void HandleCommandComplete(CMPXCommand* aCommandResult, 
                               TInt aError);
    
    /**
     * From MMPXCollectionObserver
     *
     * @since S60 5.0
     */
    void HandleCollectionMediaL(const CMPXMedia& aMedia, 
                                TInt aError);

private:
    /**
     * Handle collection messages.
     *
     * @since S60 5.0
     * @param aMsg CMPXMessage to handle 
     */
    void DoHandleCollectionMessageL( const CMPXMessage& aMsg );
    
    /**
     * Handle command complete.
     *
     * @since S60 5.0
     * @param aCommand result of the command
     * @param aError any errors
     */
    void DoHandleCommandCompleteL(CMPXCommand& aCommandResult, 
                                  TInt aError);
    /**
     * Fetch Count step.
     *
     * @since S60 5.0
     */ 
    void DoFetchCountL();
    
    /**
     * Handle completion of count step.
     *
     * @since S60 5.0
     * @param aMedia media returned from the collection
     * @param aIndex item focus from the collection
     * @param aError error of the operation
     */
    void DoHandleCountL(const CMPXMedia& aMedia, 
                        TInt aIndex, 
                        TInt aError);
                        
    /**
     * Fetch items step.
     *
     * @since S60 5.0
     */
    void DoFetchItemsL();
    
    /**
     * Handle completion of fetch item step.
     *
     * @since S60 5.0
     * @param aMedia media returned from the collection
     * @param aIndex item focus from the collection
     * @param aError error of the operation 
     * @leave IfNull
     */
    void DoHandleFetchItemsL( const CMPXMedia& aMedia,
                              TInt aIndex, 
                              TInt aError);
    
    /**
     * Run the next step.
     *
     * @since S60 5.0
     */    
    void RunNext();

    /**
     * Callback function for the periodic timer.
     *
     * @since S60 5.0
     */
    static TInt Callback( TAny* aAny );
    
     /**
      * Handle callback from the timer.
      *
      * @since S60 5.0
      */
    void HandleCallbackL();
        
    /**
     * Setup the chunks.
     *
	 	 * @since S60 5.0
     * @param aCount total number of items
     */
    void DoSetupChunksL( TInt aCount );
    
    /**
     * Sort ascending.
     *
     * @since S60 5.0
     * @param aSkipFirst do not order the first item
     */
    void DoSortAscend( TBool aSkipFirst = EFalse );
    
    /**
     * Sort decending do not order the first item.
     *
     * @since S60 5.0
     * @param aSkipFirst do not order the first item
     */
    void DoSortDescend( TBool aSkipFirst = EFalse );
    
    /**
     * Sort normal.
     *
     * @since S60 5.0
     * @param aSkipFirst do not order the first item
     */
    void DoSortNormalL( TBool aSkipFirst = EFalse );
    
    /**
     * Compact the task list given partial results may have been fetched.
     *
     * @since S60 5.0
     * @param aMedia media containing the fetched sections
     */
    void DoCompactTaskListL( const CMPXMedia& aMedia );
    
    /**
     * Send a command to the collection to update selection index.
     *
     * @since S60 5.0
     */
    void DoSelectIndexL();
    
private:
  
    /**
     * Constructor.
     *
     * @since S60 5.0
     * @param aObs observer
     * @param aMode mode of the utility
     */
    CMPXCollectionOpenUtility( MMPXCollectionObserver* aObs, TUid aMode );

    /**
     * 2nd phase constructor.
     *
     * @since S60 5.0
     */
    void ConstructL();

private:

    /**
     * Enum fo rthe different states of this object
     */
    enum TFetchingStep
        {
        EFetchPath,
        EFetchCount,
        EFetchItems,
        EFetchCommand,
        EFetchNone
        };

private: // data
    CPeriodic*             iTimer;
    MMPXCollectionUtility* iCollection;

    TDirection             iFetchDirection;
    TMPXOpenDataBlock      iFetchInfo;
    TMPXAttribute          iKeyAttribute;
    TInt                   iFetchDelay; 
    
    TFetchingStep          iFetchStep;
    MMPXCollectionObserver* iObs; 
    RArray<TMPXOpenDataBlock> iIncrementalChunks;
    
    CMPXMedia*             iMedia;  // handle to the current data retrieved from handleopen
    CMPXMediaArray*        iData;   // not owned, media array inside of iMedia
    CMPXCollectionPath*    iPath;   // Current working path
    
    TUid                   iMode;
    TBool                  iFirstOpen;
    TBool                  iSelecting;
    };

#endif // C_CMPXCOLLECTIONOPENUTILITY_H
