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
* Description:  This header file describes the triggering container interface
*
*/


#ifndef C_LBTCONTAINER_H
#define C_LBTCONTAINER_H

#include <e32base.h>
#include <lbttriggerfiltercomposite.h>
#include <lbttriggerdynamicinfo.h>
#include <lbtlisttriggeroptions.h>

#include "lbtcontainertriggerentry.h"
#include "lbtlisttriggerobserver.h"
#include "lbtcontainerlistoptions.h"
#include "lbtsecuritypolicy.h"


// forward declaration
class MLbtTriggerStore; // Trigger Store Interface
class MLbtContainerChangeEventObserver; // Client Observer Interface
class CLbtListTriggerOptions;
class CLbtTriggerIdGenerator;
class CLbtContainerAO;
class CLbtContainerUpdateFilter;

/**
 * The data type defines a bitmask of the TLbtTriggerChangeEvent enumerator.
 * It is used to subscribe to specific trigger store change events.
 */
typedef TInt TLbtTriggerEventMask;
    
/**
 *  This class provides the inteface to access the functionality of the
 *  Location Triggering Container.
 *
 *  The CLbtContainer is a singleton class. The same instance is shared by all
 *  the components in the subsystem.
 *
 *  The inteface provides the following functionality,
 *  - Create a Session or Startup trigger
 *  - Modify the static or dynamic information of a trigger
 *  - Delete one or more triggers based on filters.
 *  - Retrieve information on one or more triggers based on filters.
 *
 *  @lib lbtcontainer.lib
 *  @since S60 v4.0
 */
NONSHARABLE_CLASS( CLbtContainer ) : public CBase
    {
public:
    /**
     * enumerator to specify the different trigger store events .
     */
    enum TLbtTriggerChangeEvent
        {
        ELbtConTriggerCreated = 0x0001, // A Trigger is added to trigger store
        ELbtConTriggerDeleted = 0x0002, // A Trigger is deleted from trigger store
        ELbtConTriggerNameFieldChanged = 0x0004, // Trigger Name Field has changed
        ELbtConTriggerStateFieldChanged = 0x0008, // Trigger has been enabled or disabled
        ELbtConTriggerRequestorFieldChanged = 0x0010, // Trigger requestor Field has changed
        ELbtConTriggerManagerUiFieldChanged = 0x0020, // Trigger Manager Ui has changed
        ELbtConTriggerConditionFieldChanged = 0x0040, // Trigger Condition has changed
        ELbtConTriggerStartupFieldChanged = 0x0080, // Trigger Startup Field has changed
        ELbtConTriggerHysteresisFieldChanged = 0x0100, // Trigger Hysteresis Field has Changed
        ELbtConTriggerRectAreaFieldChanged = 0x0200, // Trigger Rectangular area field has changed
        ELbtConTriggerFiredFieldChanged = 0x0400, // Trigger Fired field has changed
        ELbtConTriggerStrategyDataFieldChanged = 0x0800, // Trigger strategy data field has changed
        ELbtConTriggerValidityFieldChanged = 0x1000, // Trigger validity field has changed
        ELbtConTriggerFireOnCreationFieldChanged = 0x2000, // Trigger fire on creation field has changed
        ELbtConTriggerEnabledAndValidCreated = 0x4000 // For startegy. Enabled and valid trigger created
        };
        
public:
    /**
     * The Symbian 2 phase constructor.
     * This method does not create new instances of the container. It creates
     * an instance the first time and then returns the same instance.
     *
     * @return An instance of this class
     */
    IMPORT_C static CLbtContainer* NewL();

    /**
     * Destroy the Container Instance.
     * Since this class is a singleton class, this does not necessarily 
     * destroy the Container instance. 
     */

    IMPORT_C static void Destroy();
    
public:    
    /**
     * Create a trigger in the Container. Both session and startup triggers
     * are created using this method. This is an synchronous method call.
     *
     * @param[in] aEntry This parameter contains trigger information.
     * 
     * @param[out] aStatus Contains the error code once the operation is 
     * completed. The value can be,
     * - any Symbian error code.
     * - KErrNoMemory if the trigger was not created because of low memory
     * - KErrDiskFull if the startup trigger was not created because of
     * low disk space.
     */
    IMPORT_C void CreateTrigger(
                    CLbtContainerTriggerEntry &aEntry,TInt& aOpId,
                    TRequestStatus& aStatus,
                    TLbtSecurityPolicy aSecurityPolicy = KLbtNullSecurity );                    
                    
                    
    /** 
     * Cancel Async Operation operation in the container
     *
     */    
    IMPORT_C void CancelAsyncOperation(TInt aOpId);
    
    /**
     * Retreives the specified triggers from the database. Both session and startup 
     * triggers are retreived by this API. The triggers are retreived asynchronously
     *
     * @param[in] aTriggerIds the trigger ids which have to be retreived 
     * @param[out] the container trigger entries that hold the trigger information
     * @param[out] aStatus Contains the error code once the operation is 
     * completed.
     * @param[out] aOpId the operation identifier
     * @param[in] The security policy which the retreived triggers have to satisfy
     */
    IMPORT_C void GetTriggers( const RArray<TLbtTriggerId>& aTriggerIds,
    						   RPointerArray < CLbtContainerTriggerEntry >& aTriggers,
    						   TInt& aOpId,
    						   TRequestStatus& aStatus,
    						   TLbtSecurityPolicy aSecurityPolicy = KLbtNullSecurity );
    
    
    /**
     * Modify the trigger fire state.The fires state of both session and 
     * startup triggers can be modified using this method.
     *
     * @param[in] aTriggerIds an array containing the trigger ids whose
     * fired state needs to be updated
     *
     * @param[out] aOpId the operation identifier
     *
     * @param[in] aFireBool the state to which the fires state has to be updated
     *
     * @param[in] aStatus the request status of the client 
     */
	IMPORT_C void UpdateTriggerFiredState( RArray<TLbtTriggerId>& aTriggerIds,
				                		   TInt& aOpId,
				                		   TBool aFireBool,
			                    		   TRequestStatus& aStatus );


	/**
	 * Modifies the fields of a particular trigger in container. The fields
	 * of both session and startup triggers can be changed using this method.
	 *
	 * @param[in] aEntry refrence to the trigger entry whose fields have to be 
	 * changed
	 *
	 * @param[in] aDataMask the mask of the data fields that have to be changed
	 *
	 * @param[in] aAttrMask the mask of the attribute fields that have to be 
	 * changed
	 *
	 * @param[out] aOpId the operation identifier
	 *
	 * @param[in] aStatus the request status of the client 
	 *
	 * @leave KErrNoMemory The trigger was not updated because of low memory
     * @leave KErrDiskFull The trigger was not updated because of
     * low disk space. This happens when the commandline or the name of
     * the trigger is updated and there is no disk space for the update.
     *
     * @note The Update Trigger potentially allows modification for any 
     * trigger attribute except the trigger ID. The Container does not perform
     * any checks regarding non modifiable attributes.
	 */
	
	IMPORT_C void UpdateTriggerL( CLbtContainerTriggerEntry& aEntry,
    							  TLbtTriggerDataMask aDataMask,
    							  TLbtTriggerAttributeFieldsMask aAttrMask,
    							  TInt& aOpId,
    							  TRequestStatus& aStatus,
                    			  TLbtSecurityPolicy aSecurityPolicy = KLbtNullSecurity );
    /**
     * Modify the state of triggers in the Container. The state of both 
     * session and startup triggers can be modified using this method.
     * 
     * @param[in] aState This parameter specifies the state to which the trigger
     * should be set to. 
     *
     * @param[in] aFilter This parameter specifies the triggers for which the state
     * has to be changed. 
     * <b> The Container will take ownership of the filter paramter.</b>
     *
     * @param[out] aStatus Contains the error code once the operation is 
     * completed. The value can be any Symbian error code.
     *
     * @note The Update Trigger potentially allows modification for any 
     * trigger attribute except the trigger ID. The Container does not perform
     * any checks regarding non modifiable attributes.
     *
     * @note The Filter can specify a list of trigger Ids. Hence using this 
     * method it is possible to enable or disable a list of triggers.
     */
	 IMPORT_C void UpdateTriggersState(
	                CLbtTriggerEntry::TLbtTriggerState aState,
                    CLbtContainerUpdateFilter* aFilter,
                    TInt& aOpId,
                    TLbtFireOnUpdate aFireOnUpdate,
                    TRequestStatus& aStatus,
                    TLbtSecurityPolicy aSecurityPolicy = KLbtNullSecurity );
                    
  
       
    /**
     * Modify the validity of triggers in the Container. The validity attribute
     * of both session and startup triggers can be modified using this method.
     * 
     * @param[in] aValidity This parameter specifies the validity to which the trigger
     * should be set to.
     *
     * @param[in] aFilter This parameter specifies the triggers for which
     * the state has to be changed.
     * <b> The Container will take ownership of the filter paramter.</b>
     *
     * @param[out] aStatus Contains the error code once the operation is 
     * completed.The value can be any Symbian error code.
     *
     * @note The Update Trigger potentially allows modification for any 
     * trigger attribute except the trigger ID. The Container does not perform
     * any checks regarding non modifiable attributes.
     *
     * @note The Filter can specify a list of trigger Ids. Hence using this 
     * method it is possible to set a list of triggers to valid or invalid.
     */
	IMPORT_C void UpdateTriggersValidity(
                    TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity,
                    RArray <TLbtTriggerId>& aTriggerIds,
                    TInt& aOpId,
                    TRequestStatus& aStatus,
                    TLbtSecurityPolicy aSecurityPolicy = KLbtNullSecurity );
                    

    /**
     * Asynchronous delete triggers in the Container based on a filter. 
     * Both session and startup triggers can be deleted using this method.
     * 
     * @param[in] aFilter This parameter specifies the triggers which need to
     * be deleted.
     * <b> The Container will take ownership of the filter paramter.</b>
     *
     * @param[out] aStatus Contains the error code once the operation is 
     * completed.The value can be,
     * - KErrNotFound if the filter does not match any trigger
     * - any other Symbian error code.
     *
     * @note The Filter can specify a list of trigger Ids. Hence using this 
     * method it is possible to delete a list of triggers.
     */
	IMPORT_C void DeleteTriggers(
	                CLbtContainerUpdateFilter* aFilter,
	                TInt& aOpId,
	                TRequestStatus &aStatus,
                    TLbtSecurityPolicy aSecurityPolicy = KLbtNullSecurity );
	
    /**
     * List triggers in the Container. The session and startup triggers
     * can be deleted using this method.
     * 
     * @param[in] aListOptions Specifies the options used for listing 
     * triggers. Default value is NULL, which will retrieve all triggers
     * owned by the client application.
     * <b> The Container will take ownership of the list options paramter.
     * </b>
     *
     * @param[out] aTriggers An Array of trigger entry objects. It contains 
     * the triggers that match the specified criteria. The ownership of the 
     * trigger entry objects in the array is transferred to the client.
     *
     * @param[in] aMask Specifies the attributes that need to be modified.
     * The mask is a bitmask of the enumerators defined in 
     * #CLbtContainerTriggerEntry::TLbtContainerTriggerAttribute
     *
     * @param[out] aStatus Contains the error code once the operation is 
     * completed.The value can be,
     * - KErrNotFound if the filter does not match any trigger
     * - any other Symbian error code.
     *
     * @note The Filter can specify a list of trigger Ids. Hence using this 
     * method it is possible to delete a list of triggers.
     */
	IMPORT_C void ListTriggers(
	                CLbtContainerListOptions* aListOptions,
	                RPointerArray < CLbtContainerTriggerEntry >& aTriggers,
	                TInt& aOpId,
	                TRequestStatus &aStatus,
                    TLbtSecurityPolicy aSecurityPolicy = KLbtNullSecurity );
	 
    /**
     * Register for notifications when the container trigger store changes.
     * The parameters specify the events for which the observer can choose to get
     * notification.
     *
     * @param[in] aObserver This parameter specifies the observer for the events.
     * The observer will be notified when ever a change occurs in the Container.
     * The container does not take ownership of this parameter.
     * @param[in] aEventMask This parameter specifies the specific events for which
     * the client wants to obtain notifications
     *
     * @note the client should implement the observer methods to obtain the 
     * events. 
     */
	IMPORT_C void SetChangeObserver(
	                MLbtContainerChangeEventObserver* aObserver,
	                TLbtTriggerEventMask aEventMask);

    /**
     * De-register request for container trigger store change notification.
     *
     * @param[in] aObserver This parameter specifies the observer that has to 
     * be de-registered. The Container doesn't take onwership of this parameter.
     *
     * @note The observer will be not notified when a change occurs in the
     * Container in future.
     */
	IMPORT_C void RemoveObserver(MLbtContainerChangeEventObserver* aObserver);


	/* Get the Count of Enabled and Valid Triggers */
	IMPORT_C TInt GetCountOfEnabledAndValidTriggers();
	
	/**
	 * Used to set the time till when compaction is preferred. Over the time specified
	 * compaction should not happed in ideal cases. As and when compaction happens this
	 * time is check to see that compaction does not over shoot this time interval
	 *
	 * @param aTime the time untill which compaction can proceed without effecting
	 *        the system
	 */ 
	IMPORT_C void SetTimeTillCompaction(TTime aTime);
	

public:
    /**
     * Internal structure for maintaining observers
     */     
    struct TLbtTriggerStoreChangeObserver
        {
        MLbtContainerChangeEventObserver* iObserver;
        TLbtTriggerEventMask iEventMask;
        };
        
private:
	/**
	 * Generates op code to identify requests
	 *
	 * @return TInt the opcode
	 */ 
	TInt GenerateRandomOpCode();

    /**
     * C++ constructor
     */
    CLbtContainer();

    /**
     * Symbian 2nd phase of construction
     */
    void ConstructL();

    /**
     * Destructor.
     */
    virtual ~CLbtContainer();
    
    /**
     * C++ Copy Constructor
     * The private copy constructor prevents the usage of copy constructors
     * with this class.
     */
    CLbtContainer(const CLbtContainer& aContainer);
    
    /**
     * Overload equality operator
     * The private overload of the equality operator prevents the usage of
     * equality operator with this class.
     */
    CLbtContainer& operator =(const CLbtContainer& aContainer);
    

private: // data members
    /**
     * Pointer to an instance of this class.
     * Owns
     */
    static CLbtContainer* iContainerInstance;

    /**
     * variable to maintain usage reference count for the shared instance of 
     * this class.
     */
    static TInt iRefCount;
    
    /**
     * Array of Pointers to Trigger Store Managers.
     * Owns
     */
    RPointerArray<MLbtTriggerStore> iTriggerStores;
    
    /**
     * Array of Trigger Store Change Observers.
     * Owns
     */
    RArray<TLbtTriggerStoreChangeObserver> iObservers;
    
    // For generating trigger ids
    TLbtTriggerId iTrigId;
    
    // Pointer to an object of trigger id generator
    CLbtTriggerIdGenerator *iTrigIdGenerator;
    
    // Pointer to an object of Container active object
    CLbtContainerAO* iContainerAO;
    
    // For random number generator
    TInt64 iRandNumRef;
    };


#endif //  C_lbtTAINER_H
