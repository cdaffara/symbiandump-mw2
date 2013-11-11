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
* Description:  This header file describes the class that handles the storage
*                of triggers in database.
*
*/


#ifndef C_LBTCONTAINER_DBTRIGGERS_MANAGER_H
#define C_LBTCONTAINER_DBTRIGGERS_MANAGER_H

// system includes
#include <e32base.h>
#include <d32dbms.h>

// user includes
#include "lbttriggerstoreinterface.h"

// forward declaration
class CLbtDbOperationAO;
class MLbtTriggerStoreObserver;
class CLbtContainerFilterBase;

/**
 *  This class provides an abstraction to store and retrieve triggers
 *  from a symbian database.
 *
 *  It internally utilizes the database client side API to store and
 *  retrieve information from the database. It also contians a cache
 *  mechanism to provide for faster access to the information from the
 *  database.
 */
NONSHARABLE_CLASS( CLbtDbTriggersManager ) : public CActive,
                                             public MLbtTriggerStore
    {
     enum TDbOperation
        {
        EDbOpNone = 0,
        EDbOpCreateTrigger,
        EDbOpGetTriggers,
        EDbOpListTriggers,
        EDbOpUpdateTrigger,        
        EDbOpUpdateTriggersState,
        EDbOpUpdateTriggersValidity,
        EDbOpUpdateTriggers,
        EDbOpDeleteTriggers
        };
     
     enum TOperationState
         {
         EOpStateNone,
         EOpStateQuery,
         EOpStateIterating,
         EOpCompaction
         };

public:
    /**
     * The Symbian 2 phase constructor.
     * This method creates a new instance of CLbtDbTriggersManager
     *
     * @return An instance of this class
     */
    static CLbtDbTriggersManager* NewL( );

    /**
     * Destructor.
     */
    virtual ~CLbtDbTriggersManager();

    /**
     * This method signals the completion of an asynchronous SQL Query
     * Operation on the database
     * @return any symbian error code.
     */
    void DbSqlOperationCompletedL(TInt error);

    /**
     * This method signals the completion of an asynchronous
     * Operation on the database like database compaction or recovery.
     * @return any symbian error code.
     */
    void DbIncrementalOperationCompleted(TInt error);
    
    /**
     * This method returns the list of triggers modified for a given operation
     * @param[out] aIdArray Contains the list of triggers modified for a given operation. 
     */
     void TriggersModified(RArray<TLbtTriggerModifiedInfo>& aArray);
     
     TInt GetCountOfEnabledAndValidTriggers();

protected: // from base class MLbtTriggerStore

    TTriggerTypeMask SupportedTriggerTypes();
    
	void CreateTriggerL( CLbtContainerTriggerEntry &aEntry,
        				 TRequestStatus& aStatus );

	void GetTriggersL( RArray<TLbtTriggerId>& aTriggerIds,
					   RPointerArray<CLbtContainerTriggerEntry>& aTriggers, 
					   TLbtSecurityPolicy& aSecurityPolicy,
					   TRequestStatus &aStatus );       
    void ListTriggersL( CLbtContainerListOptions* aFilter,
						RPointerArray<CLbtContainerTriggerEntry>& aTriggers, 
						TLbtSecurityPolicy& aSecurityPolicy,
						TRequestStatus &aStatus);
    
    void UpdateTriggerL( CLbtContainerTriggerEntry& aEntry,
						 TLbtTriggerDataMask aDataMask,
						 TLbtTriggerAttributeFieldsMask aAttrMask,
						 TLbtSecurityPolicy& aSecurityPolicy,
						 TRequestStatus& aStatus );
    
    void UpdateTriggersStateL( CLbtTriggerEntry::TLbtTriggerState aState,
			                   CLbtContainerUpdateFilter* aFilter,
			                   TLbtFireOnUpdate aFireOnUpdate,
			                   TLbtSecurityPolicy& aSecurityPolicy,
			                   TRequestStatus& aStatus);
                    
    void UpdateTriggersValidityL( TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity,
				                  RArray<TLbtTriggerId>& aTriggerIds,
				                  TLbtSecurityPolicy& aSecurityPolicy,
				                  TRequestStatus& aStatus);

	void UpdateTriggerFiredStateL( RArray<TLbtTriggerId>& aTriggerIds,
								   TBool aFireBool,
			                       TRequestStatus& aStatus );                    
    
    void DeleteTriggersL( CLbtContainerUpdateFilter* /*aFilter*/,
						  TLbtSecurityPolicy& aSecurityPolicy,
						  TRequestStatus& /*aStatus*/ );

    TBool IsStoreBusy();

    void SetStoreChangeObserver(MLbtTriggerStoreObserver* aObserver);    
    TInt CancelCurrentOperation();    
    
private: // From CActive
    void RunL();
    TInt RunError(TInt aError);
    void DoCancel();
    
private: // Other private functions    
	CLbtContainerTriggerEntry* RetrieveTriggerEntryFromDbL( RDbView& aMainView );
	void DeleteTriggerL(TLbtTriggerId aTriggerId);	
	void AppendTriggerInfo(CLbtContainerTriggerEntry* aEntry);	
	void SetTimeTillCompaction(TTime aTime);	
	void RemoveTriggerConditionFromDb( TLbtTriggerId aTriggerId, 
                                       CLbtGeoAreaBase::TGeoAreaType aAreaType );	
	void AddGeoCellIntoDbL( CLbtTriggerEntry* aTriggerEntry );	
	void AddGeoCircleIntoDbL( CLbtTriggerEntry* aTriggerEntry );	
	void AddGeoHybridIntoDbL( CLbtTriggerEntry* aTriggerEntry );
	void SelfComplete();
	
private: // All active asynch operation handlers    
    /**
     * Handles trigger creation async event
     */
    void HandleTriggerCreationEventL();
    void HandleTriggersDeletionEventL();
    void HandleListTriggersEventL();
    void HandleTriggerStateUpdationEventL();
    void HandleTriggerUpdationEventL();
    void HandleGetTriggersEventL();
    
private:
    /**
     * C++ constructor
     */
    CLbtDbTriggersManager();
   
    /**
     * Symbian 2nd phase of construction
     */
    void ConstructL();

    /**
     * C++ Copy Constructor
     * The private copy constructor prevents the usage of copy constructors
     * with this class.
     */
    CLbtDbTriggersManager(const CLbtDbTriggersManager& aDbTriggersManager);
    
    /**
     * Overload equality operator
     * The private overload of the equality operator prevents the usage of
     * equality operator with this class.
     */
    CLbtDbTriggersManager& operator =(
        const CLbtDbTriggersManager& aDbTriggersManager);

    /** 
     * Create the Table in the Database 
     */
    void CreateTablesInDbL();
    
    /**
     * Helper method to create a trigger entry in the Db View
     */
    void CreateTriggerInViewL();
    
    void ProcessListTriggers( CLbtContainerTriggerEntry* aEntry,
    						  TLbtTriggerAttributeFieldsMask aAttrMask,
    						  TLbtTriggerDynamicInfoFieldsMask aDynInfoMask,
    						  TLbtTriggerDataMask aDataMask );
    
    /**
     * Checks if the DB requires compaction and does the compaction
     */
     void CompactDbIfRequiredL();     
     
     /**
      * 
      */
     void CompleteClientRequest( TInt aError );     
     
     /**
      * 
      */
     void ResetCurrentStates();
     
     /**
      * Prepares view for list operation. The view is prepared
      * such that all the tuples required for the list operation
      * is prepared
      */
     void PrepareViewForListingL( RDbView& aView );
     
     /**
      * Prepare view for the common db such that only the tuples that 
      * match the trigger ids supplied are present in the view.
      */
     void PrepareViewForTriggersL( const RArray<TLbtTriggerId>& aTriggerIds, RDbView& aView );
     
     /**
      * 
      */
     TInt AsyncPrepareViewForTriggers( const RArray<TLbtTriggerId>& aTriggerIds, RDbView& aView );
     
     /**
      * Prepares the main view for creation of a trigger
      */
     TInt AsyncPrepareViewOfCommonTable();     
     
     /**
      * Checks if the filter supplied matches the type of triggers stored in
      * the database
      *
      * @return ETrue if the filter matches for DB, EFalse otherwise
      */ 
     TBool IsFilterForDBTriggersL( CLbtTriggerFilterBase* aFilter );
     
     /**
      * Creates Cell triggers table in DB
      */
     void CreateCellTriggersTableL( );
     
     /**
      * Creates Co-ordinate triggers table in DB
      */
     void CreateCoordinateTriggersTableL();
     
     /**
      * Creates triggers common attribute table in DB
      */
     void CreateCommonTriggerAttributesTableL();     
     /**
      * Creates triggers common attribute table in DB
      */
     void CreateHybridTriggersTableL();
     
private:
    // File Server Client Side handle
    RFs iFs;
    
    // The Database Handle
    RDbNamedDatabase iDb;
    
    // The operation executioner
    CLbtDbOperationAO* iDbOperation;
    
    // SQL query to be executed
    HBufC* iSqlQuery;
    
    /** The current operation being executed */
    TDbOperation iCurrentOperation;
    
    // The state of the current operation being handled
    TOperationState iOperationState;
    
    /** 
     * The Database view which provides the interface to execute 
     * the db operations 
     */
    RDbView iView;        
    
    /** 
     * The trigger entry on which the create trigger operation is performed.
     * This object is not owned by container and should NOT BE deleted by 
     * container
     */
    CLbtContainerTriggerEntry* iEntry;
    
    /** The status variable used for a create trigger request */
    TRequestStatus* iClientReqStatus;

    /** 
     * Pointer to CLbtContainerListOptions used in List Triggers operation 
     * This is a reference parameter and is valid only when the 
     * List Triggers operation is being executed.
     * Db Triggers Manager should NEVER DELETE this member variable.
     */
    CLbtContainerListOptions* iFilter;    
    
    /**
     * Owned by container
     */
    CLbtContainerFilterBase* iFilterBase;
    
    /**
     * 
     */
    TLbtSecurityPolicy iClientSecurityPolicy;
    
    /** 
     * Pointer to array of CLbtContainerTriggerEntry.
     * This is a reference to the array which is filled up
     * with the triggers to be returned to CLbtContainer class. 
     * This object should not NEVER BE deleted by container
     */
    RPointerArray<CLbtContainerTriggerEntry>* iTriggers;
    
    // Array to hold of list of trigger ids modified by an operation
    RArray<TLbtTriggerModifiedInfo> iIdArray;
    
    // Count of enabled and valid trigger in the db
    TInt iCountOfEnabledAndValidTrigger;
    
    // The time which can be provided untill compaction can proceed
    TTime iTimeTillCompaction;
    
    // Refernce to trigger store observer
    MLbtTriggerStoreObserver* iObserver;
    
    // Flag to check if the db has been opened successfully
    TBool iIsDbOpened;    
    
    //It is used to store new state of the trigger to be modified.
    // It is used in RunL to do the actual modification
    CLbtTriggerEntry::TLbtTriggerState iState;
    
    //It is used to store new fireonupdate flag of the trigger to be modified.
    // It is used in RunL to do the actual modification.
    TLbtFireOnUpdate iFireOnUpdate;
    
    // It is used to store datamask of trigger to be modified.
    // It is used in RunL to do the actual modification of trigger.
    TLbtTriggerDataMask iDataMask;
    
   // It is used to store attrmask of trigger to be modified.
   // It is used in RunL to do the actual modification.
    TLbtTriggerAttributeFieldsMask iAttrMask;
    
    // It is used to store id of triggers to be retrived.
    // It is used in RunL to do the actual modification.
    RArray<TLbtTriggerId>* iTriggerIds;
    };


#endif // C_LBTCONTAINER_DBTRIGGERS_MANAGER_H
