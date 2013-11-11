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


#ifndef C_LBT_CONTAINER_AOOPERATION_H
#define C_LBT_CONTAINER_AOOPERATION_H

#include "lbttriggerstoreinterface.h"

// Enumeration for type of AO operation
enum TContainerAOOperation
    {
    EOpNone = 0,
    EOpCreateTrigger,
    EOpListTriggers,
    EOpUpdateTrigger,
    EOpUpdateTriggersFiredState,
    EOpUpdateTriggerState,
    EOpUpdateTriggersValidity,
    EOpDeleteTriggers
    };

       	
/**
 * This is the  class used hold the data sent by the client in case of 
 * asynchronous operations 
 *
 */       	
NONSHARABLE_CLASS( CLbtContainerAOOperation ) : public CBase
    {
public:  
     /**
      *
      */
     virtual void StartAOOperationL(MLbtTriggerStore* aStore,TRequestStatus &aStatus) = 0;
    
     /**
      *
      */
     virtual TRequestStatus* GetStatus() = 0;
     
     /**
      * Destructor
      */
     virtual ~CLbtContainerAOOperation() {}        

	 /**
      *
      */
     void SetPrevReturnCode(TInt aErr);
     
     /**
      *
      */
     TInt GetPrevReturnCode();
     
     /**
      *
      */
     void SetCurrentReturnCode(TInt aErr);
     
     /**
      *
      */
     TInt GetCurrentReturnCode();
     
     /**
      *
      */
     void SetOpCode(TInt aOpCode);
     
     /**
      *
      */
     TInt GetOpCode();
     
     /**
      *
      */
     virtual TContainerAOOperation GetAOOperationType() = 0;
   
   	 /**
      *
      */
     CLbtContainerAOOperation( CLbtContainerAOOperation& aEntry);
     
     /**
      *
      */
     CLbtContainerAOOperation();
     	
private:
    /**
     * Constructor.
     */
   
    TInt iPrevRetCode;
    
    TInt iCurrRetCode;
    
    TInt iOpCode;
    };


#endif // C_LBT_CONTAINER_AOOPERATION_H
