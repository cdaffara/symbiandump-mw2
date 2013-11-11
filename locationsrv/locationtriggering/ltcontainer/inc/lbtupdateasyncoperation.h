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
* Description:  Abstract base class for location triggering entries
*
*/


#ifndef C_LBT_UPDATE_ASYNCOPERATION_H
#define C_LBT_UPDATE_ASYNCOPERATION_H


#include <lbtcommon.h>
#include <lbttriggerdynamicinfo.h>

#include "lbtcontainerupdatefilter.h"
#include "lbtcontainerAOOperation.h"
#include "lbttriggerstoreinterface.h"   
#include "lbtsecuritypolicy.h"

       	
/* This is the  class used hold the data sent by the client in case of 
 * asynchronous operations */
       	
NONSHARABLE_CLASS( CLbtUpdateAsyncOperation ) : public CLbtContainerAOOperation
    {
public:
        
     /**
     * Constructs a new instance of container trigger entry object.
     *
     * @return The new instance of the container trigger entry object.
     */
     static CLbtUpdateAsyncOperation* NewL( CLbtContainerTriggerEntry& aEntry,
     										TLbtTriggerDataMask aDataMask,
     										TLbtTriggerAttributeFieldsMask aAttrMask,
     										TRequestStatus& aClientStatus,
     										TLbtSecurityPolicy aSecurityPolicy,
     										TContainerAOOperation aUpdateOpType);
    
     static CLbtUpdateAsyncOperation* NewL( CLbtTriggerEntry::TLbtTriggerState aState,
     										CLbtContainerUpdateFilter* aFilter,
     										TLbtFireOnUpdate aFireOnUpdate,
     										TRequestStatus& aClientStatus,     										
     										TLbtSecurityPolicy aSecurityPolicy,
     										TContainerAOOperation aUpdateOpType);
    
     static CLbtUpdateAsyncOperation* NewL( TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity,
     										RArray<TLbtTriggerId> &aTriggerIds,
     										TRequestStatus& aClientStatus,
     										TLbtSecurityPolicy aSecurityPolicy,
     										TContainerAOOperation aUpdateOpType);
     
     static CLbtUpdateAsyncOperation* NewL( RArray<TLbtTriggerId>& aTriggerIds,
								            TRequestStatus& aClientStatus,
								            TBool aFireBool,
								            TLbtSecurityPolicy aSecurityPolicy,
								            TContainerAOOperation aUpdateOpType );
    
        
   	 virtual void StartAOOperationL(MLbtTriggerStore *aStore, TRequestStatus &aStatus) ;
    
     virtual TRequestStatus* GetStatus();
    
    /**
     * Destructor
     */
     virtual ~CLbtUpdateAsyncOperation();

  
   
         
	
private:
    /**
     * Constructor.
     */
 	  CLbtUpdateAsyncOperation(TLbtSecurityPolicy aSecurityPolicy);
                    
      void ConstructL(); 
      
      void ConstructL( CLbtContainerTriggerEntry& aEntry,
					   TLbtTriggerDataMask aDataMask,
					   TLbtTriggerAttributeFieldsMask aAttrMask,
					   TRequestStatus& aClientStatus,
					   TContainerAOOperation aUpdateOpType);
     
     void ConstructL( CLbtTriggerEntry::TLbtTriggerState aState,
				      CLbtContainerUpdateFilter* aFilter,
				      TLbtFireOnUpdate aFireOnUpdate,
				      TRequestStatus& aClientStatus,
				      TContainerAOOperation aUpdateOpType);
     
     void ConstructL( TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity,
				      RArray<TLbtTriggerId>& aTriggerIds,
				      TRequestStatus& aClientStatus,
				      TContainerAOOperation aUpdateOpType);  

	 void ConstructL( RArray<TLbtTriggerId>& aTriggerIds,
		              TRequestStatus& aClientStatus,
		              TBool aFireBool,
		              TContainerAOOperation aUpdateOpType );

	 TContainerAOOperation GetAOOperationType();
       
private:
    
    /**
     * Pointer to trigger entry which needs to be updated.
     * This pointer is not owner by container and hence should
     * not be deleted
     */
    CLbtContainerTriggerEntry* iEntry;
    
    TLbtTriggerDataMask iDataMask;
    
    TLbtTriggerAttributeFieldsMask iAttrMask;
   
    CLbtTriggerEntry::TLbtTriggerState iState;
    
    CLbtContainerUpdateFilter* iFilter;
            
    TLbtTriggerDynamicInfo::TLbtTriggerValidity iValidity;
    
    RArray<TLbtTriggerId> iTriggerIds;
            
    TRequestStatus* iClientStatus;
     
    TContainerAOOperation iUpdateOpType;
    
    TBool iFireBool;
    
    TLbtFireOnUpdate iFireOnUpdate;
    
    // Security policy
    TLbtSecurityPolicy iSecurityPolicy;
    };


#endif // C_LBT_CONTAINER_AOOPERATION_H
