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
*                of triggers in RAM Structures.
*
*/


#ifndef C_LBTCONTAINER_RAMTRIGGERS_MANAGER_H
#define C_LBTCONTAINER_RAMTRIGGERS_MANAGER_H

// system includes
#include <e32base.h>

// user includes
#include "lbttriggerstoreinterface.h"

class CLbtRamTriggerIdTree;
class MLbtTriggerStoreObserver;
class CLbtContainerFilterBase;

/**
 *  This class provides an abstraction to store and retrieve triggers
 *  from RAM Structures.
 *
 *  It internally utilizes RAM memory and stores the trigger information
 *  in a efficient data structure.
 */
NONSHARABLE_CLASS( CLbtRamTriggersManager ) : public CActive,
                                              public MLbtTriggerStore
    {
public:
    enum TOperation
        {
        EOperationNone,
        EOperationCreate,
        EOperationUpdateTrigger,
        EOperationDelete,
        EOperationUpdatingState,
        EOperationListing,
        EOperationGetTriggers
        };
    
    /**
     * The Symbian 2 phase constructor.
     * This method creates a new instance of CLbtRamTriggersManager
     *
     * @return An instance of this class
     */
    static CLbtRamTriggersManager* NewL( );

    /**
     * Destructor.
     */
    virtual ~CLbtRamTriggersManager();
    
     /**
     * This method returns the list of triggers modified for a given operation
     * @param[out] aIdArray Contains the list of triggers modified for a given operation. 
     */
     void TriggersModified(RArray<TLbtTriggerModifiedInfo>& aArray);
     
	 TInt GetCountOfEnabledAndValidTriggers();

protected: // from base class MLbtTriggerStore
    /**
     * This method returns the types of triggers that are supported in this
     * trigger store.
     *
     * @return a bitmask of the types of triggers supported by a trigger 
     * store.
     */
    virtual TTriggerTypeMask SupportedTriggerTypes();
    
    void CreateTriggerL( CLbtContainerTriggerEntry &aEntry,
                    			 TRequestStatus& aStatus);
                    			 
	void GetTriggersL( RArray<TLbtTriggerId>& aTriggerIds,
				       RPointerArray<CLbtContainerTriggerEntry>& aTriggers, 
				       TLbtSecurityPolicy& aSecurityPolicy,
				       TRequestStatus &aStatus );

    void ListTriggersL( CLbtContainerListOptions* aFilter,
						RPointerArray<CLbtContainerTriggerEntry>& aTriggers,
						TLbtSecurityPolicy& aSecurityPolicy,
						TRequestStatus &aStatus );

    void FilterByAreaL( CLbtContainerListOptions* aFilter,
    					RPointerArray<CLbtContainerTriggerEntry>& aTriggers,
    					TLbtSecurityPolicy& aSecurityPolicy );

    void FilterByAttributeL( CLbtContainerListOptions* aFilter,
    						 RPointerArray<CLbtContainerTriggerEntry>& aTriggers,
    						 TLbtSecurityPolicy& aSecurityPolicy );
   
    
    void UpdateTriggerL( CLbtContainerTriggerEntry& aEntry,
						 TLbtTriggerDataMask aDataMask,
						 TLbtTriggerAttributeFieldsMask aAttrMask,
						 TLbtSecurityPolicy& aSecurityPolicy,
						 TRequestStatus& aStatus);    
    
    void UpdateTriggersStateL( CLbtTriggerEntry::TLbtTriggerState aState,
			                   CLbtContainerUpdateFilter* aFilter,
			                   TLbtFireOnUpdate aFireOnUpdate,
			                   TLbtSecurityPolicy& aSecurityPolicy,
			                   TRequestStatus& aStatus);
                    
    void UpdateTriggersValidityL( TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity,
			                      RArray<TLbtTriggerId>& aTriggerIds,
			                      TLbtSecurityPolicy& aSecurityPolicy,
			                      TRequestStatus& aStatus );

	void UpdateTriggerFiredStateL( RArray<TLbtTriggerId>& aTriggerIds,
								   TBool aFireBool,
			                       TRequestStatus& aStatus );
    
    void DeleteTriggersL( CLbtContainerUpdateFilter* aFilter,
						  TLbtSecurityPolicy& aSecurityPolicy,
						  TRequestStatus& aStatus );            

    void DeleteTriggerL(TLbtTriggerId aTriggerId);

    void FilterCompositeL( CLbtContainerListOptions* aFilter,
    					   RPointerArray<CLbtContainerTriggerEntry>& aTriggers,
    					   TLbtSecurityPolicy& aSecurityPolicy );
    
    void SetTimeTillCompaction(TTime /*aTime*/);
    
    /**
	 * Used to check if the trigger store is busy
	 *
	 * @return ETrue if the store is busy EFalse otherwise
	 */
    TBool IsStoreBusy();
    
    void SetStoreChangeObserver(MLbtTriggerStoreObserver* aObserver);
    
    TInt CancelCurrentOperation();
 

private:
    void AppendTriggerInfo(CLbtContainerTriggerEntry* aEntry);
    
    void SelfComplete();
    
    void CompleteClientRequest(TInt aError);
    
    void HandleUpdateTriggerEventL();
    
    void HandleListTriggerEventL();
    
    void HandleDeleteTriggersEventL();
    
    void HandleUpdateTriggersStateEventL();
    
    void HandleGetTriggersEventL();
    
private: // From CActive
    void RunL();
    TInt RunError(TInt aError);
    void DoCancel();    
    
private:
    /**
     * C++ constructor
     */
    CLbtRamTriggersManager();

    /**
     * Symbian 2nd phase of construction
     */
    void ConstructL();

    /**
     * C++ Copy Constructor
     * The private copy constructor prevents the usage of copy constructors
     * with this class.
     */
    CLbtRamTriggersManager(const CLbtRamTriggersManager& aRamTriggersManager);
    
    /**
     * Overload equality operator
     * The private overload of the equality operator prevents the usage of
     * equality operator with this class.
     */
    CLbtRamTriggersManager& operator =(const CLbtRamTriggersManager& aRamTriggersManager);
    
    CLbtRamTriggerIdTree* iTriggerIdTree;   
    
    RArray<TLbtTriggerModifiedInfo> iIdArray;
    
    TInt iCountOfEnabledAndValidTrigger;
    
    // Data member to hold the current operation underway
    TOperation iCurrentOperation;    
    
    // The client status
    TRequestStatus* iClientStatus;
    
    // Data member to hold the trigger id which is under operation
    TLbtTriggerId iCurrentTriggerId;
    
    // Clients security policy
    TLbtSecurityPolicy iClientSecurityPolicy;
    
    // Pointer to clients trigger array
    RPointerArray<CLbtContainerTriggerEntry>* iClientTriggerArray;
    
    // Copy of client provided filter base
    CLbtContainerFilterBase* iFilterBase;
    
    // Client provided list options filter
    CLbtContainerListOptions* iFilter;
    
    // Iterator into modification array
    TInt iIterator;
    
    // Trigger state
    CLbtTriggerEntry::TLbtTriggerState iState;
    
    // Fire on update flag
    TLbtFireOnUpdate iFireOnUpdate;
    
    // Container trigger entry
    CLbtContainerTriggerEntry* iClientTriggerEntry;
    
    TLbtTriggerDataMask  iDataMask;
    
    TLbtTriggerAttributeFieldsMask iAttrMask;
    };


#endif // C_LBTCONTAINER_RAMTRIGGERS_MANAGER_H
