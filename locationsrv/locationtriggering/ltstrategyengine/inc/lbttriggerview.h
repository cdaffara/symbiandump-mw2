/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class definition of Trigger View
*
*/


#ifndef C_LBTTRIGGERVIEW_H
#define C_LBTTRIGGERVIEW_H


#include <e32cmn.h>
#include <e32base.h>

#include "lbttriggerkey.h"
#include "lbttriggerviewobserver.h"
#include "lbtcontainer.h"
#include "lbttriggerfilterbyattribute.h"
#include "lbtcontainerextendedtriggerinfo.h"
#include "lbttriggerstatusobserver.h"
#include "lbtstrategycommon.h"
#include "lbtgeoareabase.h"

/**
 * CLbtTriggerView is a container class that stores the list of triggers to be
 * supervised. An instance of this class is instantiated by CLbtStrategyEngine
 * The list of triggers is updated based on the location information obtained
 * by the strategy engine.
 */
NONSHARABLE_CLASS( CLbtTriggerView ) : public CActive
	
    {

public:

    /**
     * Forward declaration
     */
    class TTriggerViewIter;

    /**
     * Factory function that instantiates an object of CLbtTriggerView
     */        
    static CLbtTriggerView* NewL( MLbtTriggerViewObserver* aObserver );

    /**
     * Checks for refresh requests that arrive from container
     */
    void CheckRefreshRequests();
     
    /**
     * Updates the list of triggers, to be used by strategy engine for 
     * supervision, based on the location information 
     */        
    void Refresh( TPositionInfo& aPosInfo, 
                  TLbtTriggerEventMask aEventMask,
                  RArray<TLbtTriggerId>& aTriggerID );
                        
    /**
     * Retrieves a list of triggers that were modified when a call to Refresh
     * is made
     */        
    void GetModifiedTriggers( RArray< TLbtTriggerId >& aList );
    
    /**
     * Retrieves triggers based on the specified ID
     */
    TBool GetTriggerByID( TLbtTriggerId aID, 
                                    CLbtContainerTriggerEntry*& aTriggerInfo );
                                    
    /**
     * Determines whether the view is empty
     */
    TBool IsEmpty();
    
    /**
     * Initializes the observer to monitor changes in container
     */
    void SetContainerObserver( MLbtContainerChangeEventObserver* aObserver );
    
    /**
     * Initializes observer to monitor trigger fire events
     */
    void SetTriggerStatusObserver( MLbtTriggerStatusObsrvr* aObserver );
    
    /**
     * Updates trigger information in the container
     */
    void UpdateTriggerInfo( CLbtGeoAreaBase::TGeoAreaType aAreaType,
                            TPositionInfo aPosInfo, 
                            CLbtContainerTriggerEntry* aTrigger, 
                            TLbtTriggerDataMask aDataMask );
                            
    /**
     * Checks for changes in nearest trigger information provided by strategy
     * engine. This method is invoked by strategy engine during supervision
     * to determine any changes in the nearest triggers.
     */
    void DetermineNearestTriggerChanges( RArray< TNearestTrigger >* aNearestTriggers );
    
    /**
     * Removes the observer to monitor changes in container
     */
    void RemContainerObserver();

    /**
     * Sets the internal trigger state to invalid. This method is called only 
     * when internal errors occur while operating on the Btree or when updates
     * from the container are erroneous
     */
    void InvalidateTrigger( TLbtTriggerId aKey );
    
        
    /**
     *
     */
    inline TInt CellTriggerCount() const;
    
    /**
     *
     */
    inline TInt CoordTriggerCount() const;
      
    /**
     * Returns an instance of the iterator used to parse through the trigger
     * information stored in the tree
     */
    TTriggerViewIter BeginL( CLbtGeoAreaBase::TGeoAreaType aPrimaryTriggerArea );
    
    
    /**
     * Returns an instance of the iterator used to parse through the trigger
     * information stored in the tree
     */
    TTriggerViewIter BeginL(CLbtGeoAreaBase::TGeoAreaType aPrimaryTriggerArea,
                          CLbtGeoAreaBase::TGeoAreaType aSecondaryTriggerArea);
    
    /**
     * Returns an instance of the iterator used to parse through the trigger
     * information stored in the tree based on the IDs specified
     */
    TTriggerViewIter Begin( RArray< TLbtTriggerId >* aID,
                            CLbtGeoAreaBase::TGeoAreaType aPrimaryTriggerArea );

    /**
     * Destructor
     */        
    ~CLbtTriggerView();
    
public:

    /**
     * Trigger information that is stored in the BTree
     */
    typedef struct TTrigger
        
        {
        
        TLbtTriggerId                               key;
        
        TBool                                       validity;
        
        CLbtGeoAreaBase::TGeoAreaType               type;
        
        CLbtContainerTriggerEntry*                  info;        
        
        };   
        
    /**
     * An iterator class that is used to access trigger information present in 
     * trigger view. An instance of this class is used by strategy engine when 
     * accessing triggers successively during supervision
     */
    class TTriggerViewIter
        
        {
        
        public:
        
            /**
             * Default Constructor
             */
            TTriggerViewIter();
            
            /**
             * Constructor to initialize the handle to the Btree object
             */ 
            TTriggerViewIter( 
                        TBtreeFix< TTrigger, TLbtTriggerId >* aTriggerList,
                        CLbtGeoAreaBase::TGeoAreaType aPrimaryTriggerArea );
            
            /**
             * Constructor to initialize the handle to the Btree object
             */ 
            TTriggerViewIter( 
                        TBtreeFix< TTrigger, TLbtTriggerId >* aTriggerList,
                        CLbtGeoAreaBase::TGeoAreaType aPrimaryTriggerArea,
                        CLbtGeoAreaBase::TGeoAreaType aSecondaryTriggerArea );
            
            /**
             * Constructor to initialize the handle to the Btree object based 
             * on the specified IDs
             */
            TTriggerViewIter( 
                    TBtreeFix< TTrigger, TLbtTriggerId >* aTriggerList,
                                RArray< TLbtTriggerId >* aTriggerIDs,
                                CLbtGeoAreaBase::TGeoAreaType aPrimaryTriggerArea );
            
            /**
             * Constructor to initialize the handle to the Btree object based 
             * on the specified IDs
             */
            TTriggerViewIter( 
                    TBtreeFix< TTrigger, TLbtTriggerId >* aTriggerList,
                                RArray< TLbtTriggerId >* aTriggerIDs,
                                CLbtGeoAreaBase::TGeoAreaType aPrimaryTriggerArea,
                                CLbtGeoAreaBase::TGeoAreaType aSecondaryTriggerArea );

            /**
             * Resets the iterator to point to the trigger element at the 
             * begining of the tree
             */
            TInt Reset();
            
            /**
             * Retrieves trigger information successively from the tree
             */
            TBool NextL( CLbtContainerTriggerEntry*& aTriggerInfo );
            
            
        private:
        
            /**
             * Index into the array of trigger IDs
             */
            TInt                                    iIndex;
            
            /**
             * TBtree Iterator
             */
            TBtreeMark                              iPos;
            
            /**
             * Flag to check if the last element in the Btree is reached
             */
            TBool                                   iLast;

            /**
             * Handle to the Btree object created by trigger view. Ownership
             * not acquired
             */
            TBtreeFix< TTrigger, TLbtTriggerId >*   iTree;
            
            /**
             * Array of trigger IDs. Ownership not acquired
             */
            RArray< TLbtTriggerId >*                iTrgArray;
            
            /**
             * Primary area object that needs to be retrieved
             */
            CLbtGeoAreaBase::TGeoAreaType            iPrimaryAreaType;
            
            /**
             * Secondary area object that needs to be retrieved
             */
            CLbtGeoAreaBase::TGeoAreaType            iSecondaryAreaType;
            
        };  // TTriggerViewIter
        
private:

    /**
     * Default constructor
     */        
    CLbtTriggerView();

    /**
     * 2nd phase constuctor for instantiating member variables
     */
    void ConstructL( MLbtTriggerViewObserver* aObserver );
    
    /**
     * Initializes the Btree with the information obtained from container
     */
    void InitializeList();
    
    /**
     * Processes trigger modification requests from the list
     */
    void ProcessRefreshRequests();
    
    /**
     * Processes update requests from strategy engine
     */
     void ProcessUpdateRequests();
    
    /**
     * Makes an entry in the tree for the new trigger created in the system
     */
    void HandleTriggerCreationEvent();
    
    /**
     * Changes the entry in the tree in accordance to the modification to the
     * trigger information in the system
     */
    void HandleTriggerModificationEvent();
    
    /**
     * Removes the trigger entry from the tree
     */
    void HandleTriggerDeletionEvent( RArray< TLbtTriggerId >* aIDs );
    
    /**
     * Compares two trigger ID values. This method is used for sorting triggers
     * within an array
     */
    static TInt CompareByID(const TLbtTriggerId& src, const TLbtTriggerId& dst );

    /**
     * Retrieves triggers based on specific conditions. The method retrieves 
     * all triggers whose IDs are specified in the array (if provided) 
     */
    void GetTriggers( RArray< TLbtTriggerId >* aIDs = NULL );
    
    /**
     * Lists all the enabled and valid triggers present in the system
     */
    void ListEnabledAndValidTriggersL();
    
    /**
     * Method used to access the BTree instance to add entries. Also maintains 
     * the count of specific trigger types being inserted.
     */    
    void AddToTreeL( TLbtTriggerId aKey, CLbtContainerTriggerEntry* aEntry );

    /**
     * Method used to access the BTree instance to delete entries. Also 
     * maintains the count of specific trigger types being deleted.
     */    
    TBool DeleteFromTreeL( TLbtTriggerId aKey );
    
    /**
     * Find an entry in the tree based on the trigger Id
     */
    TBool FindInTreeL( TLbtTriggerId aKey, TTrigger& aEntry );
    
    /**
     * Completes the iRequestStatus with KErrNone 
     */
    void SetEvent();
    
private: // from base class CActive

    /**
     * Request completion event handler
     */
    void RunL();
    
    /**
     * Called to cancel outanding requests
     */        
    void DoCancel();
    
private:

    /**
     * States of trigger view
     */
    typedef enum TState
    
        {
        
        EInitialize = 1,
        
        ERefreshView,
        
        EProcessTriggerList,
        
        ERefreshContainer,
        
        EIdle
        
        };

    /**
     * Specifies current state of view
     */
    TState                                          iState;

    /**
     * A handle to the implementation of MTriggerObserver interface to report
     * a successful refresh operation completion
     */
    MLbtTriggerViewObserver*                        iObserver;
    
    /**
     * Handle to the container observer
     */
    MLbtContainerChangeEventObserver*               iContainerObserver;
    
    /**
     * Handle to Trigger Status observer
     */
    MLbtTriggerStatusObsrvr*                        iTriggerStatusObserver;
    
    /**
     * List of triggers that are to be supervised
     * This array only consists of pointers to trigger instances stored
     * in the container.
     */
    RPointerArray< CLbtContainerTriggerEntry >      iList;
    
    /**
     * Structure to contain the parameters used for processing requests to 
     * refresh trigger view
     */
    struct TRefRequestParam
    
        {
        
        TPositionInfo                               posInfo;
        
        TLbtTriggerEventMask                        eventMask;
        
        RArray< TLbtTriggerId >*                    triggerID;
        
        };
        
    /**
     * Structure that defines the parameters used for processing requests to
     * update container
     */
     struct TUpdateRequestParam
        
        {
        CLbtGeoAreaBase::TGeoAreaType               areaType;
        
        TPositionInfo                               posInfo;
        
        CLbtContainerTriggerEntry*                  trigger;
        
        TLbtTriggerDataMask                         dataMask;
        
        };
        
    /**
     *
     */
     TInt                                           iRequestID;
    
    /**
     *
     */
    TUint8                                          iRefReqCounter;
    
    /**
     *
     */
    TUint8                                          iUpdateReqCounter;
    
    /**
     *
     */
    TInt                                            iCellTriggerCount;
    
    /**
     *
     */
    TInt                                            iCoordTriggerCount;
    
    /**
     *
     */
    TBool                                           iRequestInitiated;
    
    /**
     *
     */
    TBool                                           iDelayedInitialization;
     
    /**
     * List of TRefRequestParam(s) identifying request(s) to refresh trigger 
     * view
     */
    RArray< TRefRequestParam >                      iRefRequestList;
    
    /**
     * List of TUpdateRequestParam(s) identifying request(s) to update container
     * These trigger updates are provided by strategy engine during supervision
     */
    RArray< TUpdateRequestParam >                   iUpdateRequestList;

    /**
     * List of modified triggers, if they exist
     */
    RArray< TLbtTriggerId >                         iModifiedTrgList;
        
    /**
     * Allocates memory to be used by the BTree to store trigger information
     */
    CMemPagePool*                                   iPool;
    
    /**
     * An instance of TBtreeFix to contain the list of enabled and valid 
     * triggers in the system
     */
    TBtreeFix< TTrigger, TLbtTriggerId >*           iTriggerList;
    
    /**
     * Instance of TLbtTriggerKey to manage key values to access trigger 
     * information stored in the Btree
     */
    TLbtTriggerKey                                  iKey;
    
    /**
     * Handle to the trigger container
     */
    CLbtContainer*                                  iTrgContainer;
    
    };


/**
 *
 */
inline TInt CLbtTriggerView::CellTriggerCount() const
    {
    return iCellTriggerCount;
    }


/**
 *
 */
inline TInt CLbtTriggerView::CoordTriggerCount() const
    {
    return iCoordTriggerCount;
    }


#endif	// C_LBTTRIGGERVIEW_H
