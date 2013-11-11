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
#include "lbtlistasyncoperation.h"
#include <lbtsessiontrigger.h>
#include <lbtstartuptrigger.h>
#include <lbttriggerentry.h>
#include <lbttriggerconditionbase.h>
#include <lbttriggerconditionarea.h>
#include <lbtgeoareabase.h>
#include <lbtgeocircle.h>
#include <lbtgeorect.h>
#include "lbtcontainertriggerentry.h"
#include "lbtserver.h"
#include "lbtsecuritypolicy.h"
#include "lbtlogger.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtListAsyncOperation::NewL(
//    CLbtTriggerEntry* aEntry,
//    TLbtTriggerDynamicInfo* aDynInfo,
//    TLbtDerivedTriggerInfo* aDerivedInfo)
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CLbtListAsyncOperation* CLbtListAsyncOperation::NewL( CLbtContainerListOptions* aFilter,
													  RPointerArray<CLbtContainerTriggerEntry>& aTriggers,
													  TLbtSecurityPolicy& aSecurityPolicy,
													  TRequestStatus& aClientStatus )
    {
    CLbtListAsyncOperation* self = new(ELeave) CLbtListAsyncOperation( aSecurityPolicy );
	CleanupStack::PushL( self );
    self->ConstructL(aFilter,aTriggers,aClientStatus);
    CleanupStack::Pop( self ); 
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtListAsyncOperation::NewL
// ---------------------------------------------------------------------------
//
CLbtListAsyncOperation* CLbtListAsyncOperation::NewL( const RArray<TLbtTriggerId>& aTriggerIds,
												      RPointerArray<CLbtContainerTriggerEntry>& aTriggers,
												      TLbtSecurityPolicy& aSecurityPolicy,
												      TRequestStatus& aClientStatus )
	{
    CLbtListAsyncOperation* self = new(ELeave) CLbtListAsyncOperation(aSecurityPolicy);
	CleanupStack::PushL( self );
    self->ConstructL(aTriggerIds, aTriggers, aClientStatus);
    CleanupStack::Pop( self ); 
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtListAsyncOperation::FreeArray
// ---------------------------------------------------------------------------
//    
void CLbtListAsyncOperation::FreeArray()
 	{
 	if( iTriggers )
 		{
 		iTriggers->Reset();
 		} 	
 	}

// ---------------------------------------------------------------------------
// CLbtListAsyncOperation::CancelListOperation
// ---------------------------------------------------------------------------
//
void CLbtListAsyncOperation::CancelListOperation()
  	{
  	iCancel = ETrue;	
  	}

// ---------------------------------------------------------------------------
// CLbtListAsyncOperation::GetAOOperationType
// ---------------------------------------------------------------------------
//
TContainerAOOperation CLbtListAsyncOperation::GetAOOperationType()
	{
	return EOpListTriggers;
	}

// ---------------------------------------------------------------------------
// CLbtListAsyncOperation::StartAOOperationL
// ---------------------------------------------------------------------------
//
void CLbtListAsyncOperation::StartAOOperationL(MLbtTriggerStore *aStore,TRequestStatus &aStatus)
    {
    FUNC_ENTER("CLbtListAsyncOperation::StartAOOperationL");
    /* Add asynchronous version of list triggers in the ram and db triggers manager*/
    aStatus = KRequestPending;
    TInt error = KErrNotFound;
    if( iOperationType == EOperationList )
    	{
    	TRAP(error, aStore->ListTriggersL( iFilter,
	    								   *iTriggers, 
	    								   iSecurityPolicy,
	    								   aStatus));
    	}
    else
    	{
    	TRAP( error , aStore->GetTriggersL( iTriggerIds,
    										*iTriggers,
    										iSecurityPolicy,
    										aStatus ) );
    	}
    
    if(error != KErrNone )
    	{
    	TRequestStatus *status = &aStatus;
    	User::RequestComplete(status, error);
    	}
    }

// ---------------------------------------------------------------------------
// CLbtListAsyncOperation::GetStatus
// ---------------------------------------------------------------------------
//
TRequestStatus* CLbtListAsyncOperation::GetStatus()
    {
    return iClientStatus;
    }

// ---------------------------------------------------------------------------
// CLbtListAsyncOperation::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtListAsyncOperation::ConstructL( CLbtContainerListOptions* aFilter,RPointerArray < CLbtContainerTriggerEntry >& aTriggers,TRequestStatus& aClientStatus)  
    {
    iOperationType = EOperationList;
    iFilter = aFilter;
    iTriggers = &aTriggers;
    iClientStatus = &aClientStatus;
    }

// ---------------------------------------------------------------------------
// CLbtListAsyncOperation::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtListAsyncOperation::ConstructL( const RArray<TLbtTriggerId>& aTriggerIds,
									     RPointerArray < CLbtContainerTriggerEntry >& aTriggers,
									     TRequestStatus& aClientStatus )
	{
	iOperationType = EOperationGetTriggers;
	iTriggers = &aTriggers;
    iClientStatus = &aClientStatus;
    for(TInt i=0;i<aTriggerIds.Count();++i)
    	{
    	iTriggerIds.AppendL( aTriggerIds[i] );
    	}
	}


// ---------------------------------------------------------------------------
// CLbtListAsyncOperation::GetArrayCount
// ---------------------------------------------------------------------------
//  
TInt  CLbtListAsyncOperation::GetArrayCount()
    {
    return (iTriggers->Count());
    }

// ---------------------------------------------------------------------------
// CLbtListAsyncOperation::GetTriggerList
// ---------------------------------------------------------------------------
//
RPointerArray<CLbtContainerTriggerEntry>* CLbtListAsyncOperation::GetTriggerList()
	{
	return iTriggers;
	}

// ---------------------------------------------------------------------------
// CLbtListAsyncOperation::~CLbtListAsyncOperation()
// ---------------------------------------------------------------------------
//
CLbtListAsyncOperation::~CLbtListAsyncOperation()
    {
    iTriggerIds.Close();
    delete iFilter;
    }

// ---------------------------------------------------------------------------
// Constructor
//
// ---------------------------------------------------------------------------
//
CLbtListAsyncOperation::CLbtListAsyncOperation(TLbtSecurityPolicy& aSecurityPolicy): 
						iFilter(NULL),
						iClientStatus(NULL),
						iTriggers(NULL),
						iSecurityPolicy(aSecurityPolicy)
    {   

    }
    
// end of file
