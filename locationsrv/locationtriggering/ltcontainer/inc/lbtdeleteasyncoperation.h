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


#ifndef C_LBT_DELETE_ASYNCOPERATION_H
#define C_LBT_DELETE_ASYNCOPERATION_H


#include <lbtcommon.h>
#include <lbttriggerdynamicinfo.h>
#include "lbtcontainerupdatefilter.h"
#include "lbtcontainerAOOperation.h"
#include "lbtsecuritypolicy.h"
       	
/**
 * This is the  class used hold the data sent by the client in case of 
 * asynchronous delete operations 
 */       	
NONSHARABLE_CLASS( CLbtDeleteAsyncOperation ) : public CLbtContainerAOOperation
    {
public:
  
    
        
     /**
     * Constructs a new instance of container trigger entry object.
     *
     * @return The new instance of the container trigger entry object.
     */
     static CLbtDeleteAsyncOperation* NewL( CLbtContainerUpdateFilter* aFilter,
     										TRequestStatus& aClientStatus,
     										TLbtSecurityPolicy aSecurityPolicy );
    
        
     virtual void StartAOOperationL(MLbtTriggerStore* aStore,TRequestStatus &aStatus) ;
    
     virtual TRequestStatus* GetStatus();
    
    /**
     * Destructor
     */
     virtual ~CLbtDeleteAsyncOperation();
	
private:
    /**
     * Constructor.
     */
 	  CLbtDeleteAsyncOperation(TLbtSecurityPolicy aSecurityPolicy);
                    
      void ConstructL(CLbtContainerUpdateFilter* aFilter,TRequestStatus& aClientStatus);     
      
      TContainerAOOperation GetAOOperationType();     
       
private:    
    TRequestStatus* iClientStatus;
    
    CLbtContainerUpdateFilter* iFilter;
    
    TLbtSecurityPolicy iSecurityPolicy;
    };


#endif // C_LBT_DELETE_ASYNCOPERATION_H
