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


#ifndef C_LBT_LIST_ASYNCOPERATION_H
#define C_LBT_LIST_ASYNCOPERATION_H


#include <lbtcommon.h>
#include <lbttriggerdynamicinfo.h>
#include "lbtcontainerlistoptions.h"
#include "lbtcontainerAOOperation.h"
#include "lbtsecuritypolicy.h"
   

       	
/* This is the  class used hold the data sent by the client in case of 
 * asynchronous operations */
       	
NONSHARABLE_CLASS( CLbtListAsyncOperation ) : public CLbtContainerAOOperation
    {
    
public:        
     /**
      * Constructs a new instance of container trigger entry object. 
      * This NewL should be used for List triggers.
      *
      * @return The new instance of the container trigger entry object.
      */
     static CLbtListAsyncOperation* NewL( CLbtContainerListOptions* aFilter,
     									  RPointerArray<CLbtContainerTriggerEntry>& iTriggers,
     									  TLbtSecurityPolicy& aSecurityPolicy,
     									  TRequestStatus& aClientStatus );    
	 /**
      * Constructs a new instance of container trigger entry object.
      * This should be used for retreiving triggers based on trigger ids
      *
      * @return The new instance of the container trigger entry object.
      */
	 static CLbtListAsyncOperation* NewL( const RArray<TLbtTriggerId>& aTriggerIds,
									      RPointerArray<CLbtContainerTriggerEntry>& aTriggers,
									      TLbtSecurityPolicy& aSecurityPolicy,
									      TRequestStatus& aClientStatus );
        
     virtual void StartAOOperationL(MLbtTriggerStore* aStore,TRequestStatus &aStatus) ;
    
     virtual TRequestStatus* GetStatus();
	 
     TInt GetArrayCount();
     
     void FreeArray();
     
     /** 
      *
      */
     RPointerArray<CLbtContainerTriggerEntry>* GetTriggerList();
     
     /**
     * Destructor
     */
     virtual ~CLbtListAsyncOperation();

  
   void CancelListOperation();
         
	
private:
    /**
     * Constructor.
     */
 	  CLbtListAsyncOperation( TLbtSecurityPolicy& aSecurityPolicy );
                    
      void ConstructL( CLbtContainerListOptions* aFilter,
      				   RPointerArray<CLbtContainerTriggerEntry>& aTriggers,
      				   TRequestStatus& aClientStatus);

	  void ConstructL( const RArray<TLbtTriggerId>& aTriggerIds,
					   RPointerArray < CLbtContainerTriggerEntry >& aTriggers,
					   TRequestStatus& aClientStatus );
      
      TContainerAOOperation GetAOOperationType();
       
private:
    
    CLbtContainerListOptions* iFilter;

    TRequestStatus* iClientStatus;
     
    //Pointer to the pointer array into which the listed trigger should be present
    RPointerArray<CLbtContainerTriggerEntry>* iTriggers;
    
    RArray<TLbtTriggerId> iTriggerIds;
    
    TLbtSecurityPolicy iSecurityPolicy;
    
    enum TLbtOperationType
    	{
    	EOperationNone,
    	EOperationList,
    	EOperationGetTriggers
    	};
    
    // The operation type for which this object has been created
    TLbtOperationType iOperationType;
    
    TBool iCancel;                     
    };


#endif // C_LBT_LIST_ASYNCOPERATION_H
