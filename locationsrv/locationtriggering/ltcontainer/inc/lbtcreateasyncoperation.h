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


#ifndef C_LBT_CREATE_ASYNCOPERATION_H
#define C_LBT_CREATE_ASYNCOPERATION_H


#include <lbtcommon.h>
#include <lbttriggerdynamicinfo.h>

#include "lbtcontainertriggerentry.h"
#include "lbtcontainerAOOperation.h"
   

       	
/* This is the  class used hold the data sent by the client in case of 
 * asynchronous operations */
       	
NONSHARABLE_CLASS( CLbtCreateAsyncOperation ) : public CLbtContainerAOOperation
    {
public:
  
    
        
     /**
     * Constructs a new instance of container trigger entry object.
     *
     * @return The new instance of the container trigger entry object.
     */
     static CLbtCreateAsyncOperation* NewL(CLbtContainerTriggerEntry* aEntry,TRequestStatus& aClientStatus);
    
    
        
     virtual void StartAOOperationL(MLbtTriggerStore* aStore,TRequestStatus &aStatus) ;
    
     virtual TRequestStatus* GetStatus();
    
     TInt GetArrayCount();
     void GetTriggerList(RPointerArray<CLbtContainerTriggerEntry>* aTriggers);
    /**
     * Destructor
     */
     virtual ~CLbtCreateAsyncOperation();

  
     const CLbtContainerTriggerEntry* ContainerTriggerEntry();
         
	
private:
    /**
     * Constructor.
     */
 	  CLbtCreateAsyncOperation();
                    
      void ConstructL(CLbtContainerTriggerEntry* aEntry,TRequestStatus& aClientStatus);
      
      TContainerAOOperation GetAOOperationType();
       
private:
    // Pointer to the trigger which has to be created. Pointer just contains
    // pointer to the trigger and does not own it. It should not delete it
    CLbtContainerTriggerEntry* iEntry;
    
    TRequestStatus* iClientStatus;
     
     
                     
    };


#endif // C_LBT_LIST_ASYNCOPERATION_H
