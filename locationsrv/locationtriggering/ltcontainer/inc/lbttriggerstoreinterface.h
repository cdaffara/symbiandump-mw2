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
* Description:  This file defines the interface which the trigger stores must
*                implement.
*
*/


#ifndef M_TRIGGERSTORE_INTERFACE_H
#define M_TRIGGERSTORE_INTERFACE_H

#include <lbtlisttriggeroptions.h>
#include <lbttriggerdynamicinfo.h>
#include "lbtcontainertriggerentry.h"
#include "lbtcontainerlistoptions.h"
#include "lbtcontainerupdatefilter.h"
#include "lbtsecuritypolicy.h"

// FORWARD DECLARATION
class MLbtTriggerStoreObserver;

/**
 * This typedef defines a bitmask of the triggers supported by 
 * a trigger store. It is a bitmask of the CLbtTriggerEntry::TType enumerator.
 */
typedef TInt32 TTriggerTypeMask;

/**
 *  This M class defines the interface for the trigger stores.
 *
 *  The Trigger Store Managers must implement this interface to
 *  provide the store functionality.
 */
class MLbtTriggerStore
    {
public: // Methods of the interface.

	/**
	 * Structure used to supply data on notifing observers of any trigger 
	 * store modification operation
	 *
	 */
	struct TLbtTriggerModifiedInfo
		{
		// The id of the trigger that is modified
		TLbtTriggerId iTriggerId;
		
		// The area type of the trigger that is modified
		CLbtGeoAreaBase::TGeoAreaType iAreaType;
		
		// Uid of manager ui
		TUid iManagerUi;
		
		// Uid of the owner
		TUid iOwner;
		
		// Uid of the startup process
		TUid iStartupProcess;
		};
	
    /**
     * This method returns the types of triggers that are supported in this
     * trigger store.
     *
     * @return a bitmask of the types of triggers supported by a trigger 
     * store.
     */
    virtual TTriggerTypeMask SupportedTriggerTypes() = 0;
    
	/**
     * Creates a trigger in the trigger store in a asynchronous fashion. 
     * Once the creation is complete the client is signalled by completing
     * its active object. The request staus of the active object will hold
     * the error code which informs the client of the status of the creation.
     *
     * @param aEntry reference to the trigger entry object that contains the 
     *        trigger data which has to be created.
     *
     * @param aStatus the clients request status
     */    
    virtual void CreateTriggerL( CLbtContainerTriggerEntry &aEntry,
                    		     TRequestStatus& aStatus ) = 0;	
	
	/** 
	 * Function should append the information of the trigger that has been modfied
	 * by the latest operation
	 *
	 * @param aArray the array of the client to which the trigger info should be appended
	 */
	virtual void TriggersModified(RArray<TLbtTriggerModifiedInfo>& aArray) = 0;
	
	/**
	 * Used to set the time till when compaction is preferred. Over the time specified
	 * compaction should not happed in ideal cases. As and when compaction happens this
	 * time is check to see that compaction does not over shoot this time interval
	 *
	 * @param aTime the time untill which compaction can proceed without effecting
	 *        the system
	 */ 
	virtual void SetTimeTillCompaction(TTime aTime) = 0;
	
	
	/**
	 * Used to retreive a specified list of triggers from the store. This triggers
	 * are specifed using an array of trigger ids.
	 *
	 * @param aTriggerIds the array of trigger ids which need to be retreived
	 *
	 * @param aTriggers reference to the pointer array into which the trigger are 
     *        retreived from the trigger store
     *
     * @param aStatus the clients active status
     *
     * @param aSecurityPolicy the security policy that the triggers have to satisfy
	 */
	virtual void GetTriggersL( RArray<TLbtTriggerId>& aTriggerIds,
							   RPointerArray<CLbtContainerTriggerEntry>& aTriggers, 
    						   TLbtSecurityPolicy& aSecurityPolicy,
    						   TRequestStatus &aStatus ) = 0;
    
    /**
     * Used to list trigger based on list options. The list options contains a filter
     * based on which the trigger will be listed. The listing is done asynchronously
     * 
     * @param aFilter the container list options which contains the filter based on
     *        which trigger are filtered in the trigger store
     *
     * @param aTriggers refernce to the pointer array into which the trigger are 
     *        retreived from the trigger store and stored
     *
     * @param aStatus the clients active status
     */
    virtual void ListTriggersL( CLbtContainerListOptions* aFilter, 
    							RPointerArray<CLbtContainerTriggerEntry>& aTriggers, 
    							TLbtSecurityPolicy& aSecurityPolicy,
    							TRequestStatus &aStatus ) = 0;    
    
    /**
     * Updates the contents of a trigger in the trigger store based on masks. The trigger 
     * is searched in the trigger store based on the trigger id supplied in the trigger 
     * entry object. The masks define the trigger data members that need to be updated. 
     * This request is processed asychronously.
     *
     * @param aEntry reference to the trigger entry object that contains the 
     *        trigger data which has to be created.
     *
     * @param aDataMask the data mask based on which the contents in the triggers dynamic
     *        info is updated. The masks are defined by enumeration
     *        TLbtTriggerDynamicInfo::TLbtDynamicInfoAttribute
     * 
     * @param aAttrMask the attribute mask based on which the atrributes of the trigger
     *        are updated. The masks are defined by enumeration
     *        CLbtTriggerEntry::TAttribute
     *
     * @param aStatus the clients active status
     */
    virtual void UpdateTriggerL( CLbtContainerTriggerEntry& aEntry,
    						     TLbtTriggerDataMask aDataMask,
    						     TLbtTriggerAttributeFieldsMask aAttrMask,
    						     TLbtSecurityPolicy& aSecurityPolicy,
    						     TRequestStatus& aStatus) = 0;
    /**
     * Updates the state of multiple triggers based on container filter. This request 
     * is processed asychronously.
     *
     * @param aState the state to which the triggers that fall in the filter needs to 
     *               be to updated to
     * @param aFilter the filter based on which the triggers are selected to be updated
     * @param aStatus the clients active status
     */
    virtual void  UpdateTriggersStateL( CLbtTriggerEntry::TLbtTriggerState aState,
					                    CLbtContainerUpdateFilter* aFilter,
					                    TLbtFireOnUpdate aFireOnUpdate,
					                    TLbtSecurityPolicy& aSecurityPolicy,
					                    TRequestStatus& aStatus ) = 0;
	
	/**
     * Updates the validity of multiple triggers specified by trigger ids.
     *
     * @param aValidity the validity to which the triggers have to be changed to
     * @param aTriggerIds the array of trigger ids of which the validity have to be 
     *        changed
     * @param aStatus the clients active status
     */
    virtual void UpdateTriggersValidityL( TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity,
					                      RArray<TLbtTriggerId>& aTriggerIds,
					                      TLbtSecurityPolicy& aSecurityPolicy,
					                      TRequestStatus& aStatus) = 0;

	/**
	 * Updates the fired state of multiple triggers specified by the trigger ids
	 *
	 * @param aTriggerIds the array of trigger ids of which the validity have to be 
     *        changed
     * @param aFireBool the state to which the fired attributes has to be changed to
     * @param aStatus the clients active status
	 */
	virtual void UpdateTriggerFiredStateL( RArray<TLbtTriggerId>& aTriggerIds,
										   TBool aFireBool,
					                       TRequestStatus& aStatus) = 0;

	/**
     * Deletes multiple triggers based container update filter. All the trigger in the store
     * that satisfy the supplied filter are deleted from the store.
     *
     * @param aFilter the filter based on which the triggers are selected to be updated
     * @param aStatus the clients active status
     */
    virtual void DeleteTriggersL( CLbtContainerUpdateFilter* aFilter, 
    							  TLbtSecurityPolicy& aSecurityPolicy,
    						 	  TRequestStatus& aStatus) = 0;            
    
    /**
     * Deletes a single trigger which matches the trigger id supplied
     *
     * @param aTriggerId the id of the trigger which needs to be deleted
     */
    virtual void DeleteTriggerL( TLbtTriggerId aTriggerId ) = 0;
    
    /**
     * Returns the count of all the enabled and valid triggers in the store.
     *
     * @return the number of enabled and valid triggers
     */
    virtual TInt GetCountOfEnabledAndValidTriggers() =0;
    
    /**
     * Method to check if the trigger store is busy. The store could be busy due to many
     * reasons with the mail reason being compaction.
     *
     * @return ETrue if the store is busy, EFalse otherwise
     */
    virtual TBool IsStoreBusy() = 0;    
    
    /**
     * Used to set the trigger store observer. The set observer will be notifed if any updation
     * operation is preformed on the trigger store. The modification include creation, updation 
     * or deletion of any trigger
     *
     * @param pointer to the observed that needs to be notified when the store is updated
     */
    virtual void SetStoreChangeObserver(MLbtTriggerStoreObserver* aObserver) = 0;
    
    /**
     * The implementation of this method should cancel any ongoing async 
     * operation
     */
    virtual TInt CancelCurrentOperation() = 0;
    
    /**
     * pure virtual destructor
     */
     virtual ~MLbtTriggerStore() {};    
    };


#endif // M_TRIGGERSTORE_INTERFACE_H

// end of file


