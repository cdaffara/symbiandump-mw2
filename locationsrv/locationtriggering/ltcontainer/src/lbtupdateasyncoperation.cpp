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
* Description:  This file defines the container trigger entry class
*
*/


#include <lbttriggerdynamicinfo.h>
#include <lbtsessiontrigger.h>
#include <lbtstartuptrigger.h>
#include <lbttriggerentry.h>
#include <lbttriggerconditionbase.h>
#include <lbttriggerconditionarea.h>
#include <lbtgeoareabase.h>
#include <lbtgeocircle.h>
#include <lbtgeorect.h>
#include "lbtcontainertriggerentry.h"
#include "lbtupdateasyncoperation.h"
#include "lbtsecuritypolicy.h"
#include "lbtlogger.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CLbtUpdateAsyncOperation::NewL(
//    CLbtTriggerEntry* aEntry,
//    TLbtTriggerDynamicInfo* aDynInfo,
//    TLbtDerivedTriggerInfo* aDerivedInfo)
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CLbtUpdateAsyncOperation* CLbtUpdateAsyncOperation::NewL(
     CLbtContainerTriggerEntry& aEntry,
     TLbtTriggerDataMask aDataMask,
     TLbtTriggerAttributeFieldsMask aAttrMask,
     TRequestStatus& aClientStatus,
     TLbtSecurityPolicy aSecurityPolicy,
     TContainerAOOperation aUpdateOpType)
    {
	CLbtUpdateAsyncOperation* self=new( ELeave ) CLbtUpdateAsyncOperation(aSecurityPolicy);
	CleanupStack::PushL( self );
    self->ConstructL(aEntry,aDataMask,aAttrMask,aClientStatus,aUpdateOpType);
    CleanupStack::Pop( self ); 
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtUpdateAsyncOperation::NewL()
// ---------------------------------------------------------------------------
//
CLbtUpdateAsyncOperation* CLbtUpdateAsyncOperation::NewL( RArray<TLbtTriggerId>& aTriggerIds,
								                    	  TRequestStatus& aClientStatus,
								                    	  TBool aFireBool,
								                    	  TLbtSecurityPolicy aSecurityPolicy,
								                    	  TContainerAOOperation aUpdateOpType )
	{
	CLbtUpdateAsyncOperation* self=new( ELeave ) CLbtUpdateAsyncOperation(aSecurityPolicy);
	CleanupStack::PushL( self );
    self->ConstructL(aTriggerIds, aClientStatus, aFireBool, aUpdateOpType);
    CleanupStack::Pop( self ); 
    return self;
	}

// ---------------------------------------------------------------------------
// CLbtUpdateAsyncOperation::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//

  CLbtUpdateAsyncOperation* CLbtUpdateAsyncOperation::NewL(
     CLbtTriggerEntry::TLbtTriggerState aState,
     CLbtContainerUpdateFilter* aFilter,
     TLbtFireOnUpdate aFireOnUpdate,
     TRequestStatus& aClientStatus,
     TLbtSecurityPolicy aSecurityPolicy,
     TContainerAOOperation aUpdateOpType)
    {
	CLbtUpdateAsyncOperation* self = new( ELeave ) CLbtUpdateAsyncOperation(aSecurityPolicy);
	CleanupStack::PushL( self );
    self->ConstructL( aState,
                      aFilter,
                      aFireOnUpdate,
                      aClientStatus,
                      aUpdateOpType);
    CleanupStack::Pop( self ); 
    return self;
    }
    
// ---------------------------------------------------------------------------
// CLbtUpdateAsyncOperation::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
  CLbtUpdateAsyncOperation* CLbtUpdateAsyncOperation::NewL( TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity,
														    RArray<TLbtTriggerId> &aTriggerIds,
														    TRequestStatus& aClientStatus,
														    TLbtSecurityPolicy aSecurityPolicy,
														    TContainerAOOperation aUpdateOpType )
    {
	CLbtUpdateAsyncOperation* self=new( ELeave ) CLbtUpdateAsyncOperation(aSecurityPolicy);
	CleanupStack::PushL( self );
    self->ConstructL(aValidity,aTriggerIds,aClientStatus,aUpdateOpType);
    CleanupStack::Pop( self ); 
    return self;
    }
// ---------------------------------------------------------------------------
// CLbtUpdateAsyncOperation::ConstructL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
void CLbtUpdateAsyncOperation::ConstructL( CLbtContainerTriggerEntry& aEntry,
									       TLbtTriggerDataMask aDataMask,
									       TLbtTriggerAttributeFieldsMask aAttrMask,
									       TRequestStatus& aClientStatus,
									       TContainerAOOperation aUpdateOpType )
    {
    iEntry = &aEntry;
    iDataMask = aDataMask;
    iAttrMask = aAttrMask;
    iClientStatus = &aClientStatus;
    iUpdateOpType = aUpdateOpType;
    }

// ---------------------------------------------------------------------------
// CLbtUpdateAsyncOperation::ConstructL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//      
void CLbtUpdateAsyncOperation::ConstructL( CLbtTriggerEntry::TLbtTriggerState aState,
									       CLbtContainerUpdateFilter* aFilter,
									       TLbtFireOnUpdate aFireOnUpdate,
									       TRequestStatus& aClientStatus,
									       TContainerAOOperation aUpdateOpType ) 
     
    {
    iState = aState;
    iFilter = aFilter;
    iClientStatus = &aClientStatus;
    iUpdateOpType = aUpdateOpType;
    iFireOnUpdate = aFireOnUpdate;
    }

// ---------------------------------------------------------------------------
// CLbtUpdateAsyncOperation::ConstructL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//      
void CLbtUpdateAsyncOperation::ConstructL( TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity,
										   RArray <TLbtTriggerId>& aTriggerIds,
										   TRequestStatus& aClientStatus,
										   TContainerAOOperation aUpdateOpType)  
    {
    FUNC_ENTER("CLbtUpdateAsyncOperation::ConstructL");
    for(TInt i=0;i<aTriggerIds.Count();++i)
    	{
    	iTriggerIds.AppendL(aTriggerIds[i]);
    	}
    iValidity = aValidity;
    iClientStatus = &aClientStatus;
    iUpdateOpType = aUpdateOpType;
    }

// ---------------------------------------------------------------------------
// CLbtUpdateAsyncOperation::ConstructL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//      
void CLbtUpdateAsyncOperation::ConstructL( RArray<TLbtTriggerId>& aTriggerIds,
				                    	   TRequestStatus& aClientStatus,
				                    	   TBool aFireBool,
				                    	   TContainerAOOperation aUpdateOpType )
	{
	FUNC_ENTER("CLbtUpdateAsyncOperation::ConstructL");
	for(TInt i=0;i<aTriggerIds.Count();++i)
    	{
    	iTriggerIds.AppendL(aTriggerIds[i]);
    	}
	iClientStatus = &aClientStatus;
	iUpdateOpType = aUpdateOpType;
	iFireBool = aFireBool;
	}

// ---------------------------------------------------------------------------
// CLbtUpdateAsyncOperation::StartAOOperation()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CLbtUpdateAsyncOperation::StartAOOperationL(MLbtTriggerStore* aStore, TRequestStatus &aStatus) 
    {
    FUNC_ENTER("CLbtUpdateAsyncOperation::StartAOOperationL");
    aStatus = KRequestPending;
    switch(iUpdateOpType)
    	{
    	case EOpUpdateTrigger:
    		{
    		TRAPD(error,aStore->UpdateTriggerL( *iEntry,
    											iDataMask,
    											iAttrMask,
    											iSecurityPolicy,
    											aStatus));
        
	        if(error != KErrNone && aStatus == KRequestPending)
	            {
	        	TRequestStatus* status = &aStatus;
	            User::RequestComplete(status, error);
	            }
    		break;
    		}
    	case EOpUpdateTriggerState:
    		{
    		TRAPD(error, aStore->UpdateTriggersStateL( iState,
    												   iFilter,
    												   iFireOnUpdate,
    												   iSecurityPolicy,
    												   aStatus));

    		if(error != KErrNone && aStatus == KRequestPending)
    			{
    			TRequestStatus* status = &aStatus;
    			User::RequestComplete(status, error);
    			}
    		break;
    		}
    	case EOpUpdateTriggersValidity:
    		{
    		TRAPD(error ,aStore->UpdateTriggersValidityL( iValidity,
    													  iTriggerIds,
    													  iSecurityPolicy,
    													  aStatus));
    		if(error != KErrNone && aStatus == KRequestPending)
    			{
    			TRequestStatus* status = &aStatus;
    			User::RequestComplete(status, error);
    			}
    		break;
    		}
    	case EOpUpdateTriggersFiredState:
    		{
    		TRAPD(error, aStore->UpdateTriggerFiredStateL(iTriggerIds, iFireBool, aStatus));

    		if(error != KErrNotFound && aStatus == KRequestPending)
    			{
    			TRequestStatus* status = &aStatus;
	            User::RequestComplete(status, error);
    			}
    		break;
    		}
    	default:
    		{
    		TRequestStatus* status = &aStatus;
	        User::RequestComplete(status, KErrArgument);
    		break;
    		}
    	}    
    }

// ---------------------------------------------------------------------------
// CLbtUpdateAsyncOperation::~CLbtUpdateAsyncOperation()
// ---------------------------------------------------------------------------
//
TContainerAOOperation CLbtUpdateAsyncOperation::GetAOOperationType()    
	{
	return iUpdateOpType;
	}
  
// ---------------------------------------------------------------------------
// CLbtUpdateAsyncOperation::GetStatus()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
  TRequestStatus* CLbtUpdateAsyncOperation::GetStatus()
    {
    return iClientStatus;
    }

// ---------------------------------------------------------------------------
// CLbtUpdateAsyncOperation::~CLbtUpdateAsyncOperation()
// ---------------------------------------------------------------------------
//
 CLbtUpdateAsyncOperation::~CLbtUpdateAsyncOperation()
    {
    iTriggerIds.Close();
    delete iFilter; 
    }

// ---------------------------------------------------------------------------
// Constructor
//
// ---------------------------------------------------------------------------
//
CLbtUpdateAsyncOperation::CLbtUpdateAsyncOperation(TLbtSecurityPolicy aSecurityPolicy): 
							iEntry(NULL),
							iFilter(NULL),
							iClientStatus(NULL),
							iSecurityPolicy(aSecurityPolicy)
    {
   

    }

// end of file
