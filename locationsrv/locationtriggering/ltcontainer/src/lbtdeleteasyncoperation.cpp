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
#include "lbtdeleteasyncoperation.h"
#include <lbtsessiontrigger.h>
#include <lbtstartuptrigger.h>
#include <lbttriggerentry.h>
#include <lbttriggerconditionbase.h>
#include <lbttriggerconditionarea.h>
#include <lbtgeoareabase.h>
#include <lbtgeocircle.h>
#include <lbtgeorect.h>
#include "lbtcontainertriggerentry.h"
#include "lbtlogger.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CLbtDeleteAsyncOperation::NewL(
//    CLbtTriggerEntry* aEntry,
//    TLbtTriggerDynamicInfo* aDynInfo,
//    TLbtDerivedTriggerInfo* aDerivedInfo)
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CLbtDeleteAsyncOperation* CLbtDeleteAsyncOperation::NewL( CLbtContainerUpdateFilter* aFilter,
														  TRequestStatus& aClientStatus,
														  TLbtSecurityPolicy aSecurityPolicy)
    {
    CLbtDeleteAsyncOperation* self = new(ELeave) CLbtDeleteAsyncOperation(aSecurityPolicy);
	CleanupStack::PushL( self );
    self->ConstructL(aFilter,aClientStatus);
    CleanupStack::Pop( self ); 
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtDeleteAsyncOperation::StartAOOperationL
// ---------------------------------------------------------------------------
//
void CLbtDeleteAsyncOperation::StartAOOperationL(MLbtTriggerStore* aStore,TRequestStatus &aStatus)
    {
    FUNC_ENTER("CLbtDeleteAsyncOperation::StartAOOperationL");
    aStatus = KRequestPending;   
    TRAPD(error, aStore->DeleteTriggersL(iFilter, iSecurityPolicy, aStatus));
    
    if(error != KErrNone && aStatus == KRequestPending)
    	{
    	TRequestStatus* status = &aStatus;
    	User::RequestComplete(status, error);
    	}
    }

// ---------------------------------------------------------------------------
// CLbtDeleteAsyncOperation::GetAOOperationType
// ---------------------------------------------------------------------------
//
TContainerAOOperation CLbtDeleteAsyncOperation::GetAOOperationType()
	{
	return EOpDeleteTriggers;
	}

// ---------------------------------------------------------------------------
// CLbtDeleteAsyncOperation::ConstructL()
// ---------------------------------------------------------------------------
//
void CLbtDeleteAsyncOperation::ConstructL( CLbtContainerUpdateFilter* aFilter,TRequestStatus& aClientStatus)  
    {
    iFilter = aFilter;
    iClientStatus = &aClientStatus;
    }
    
// ---------------------------------------------------------------------------
// CLbtDeleteAsyncOperation::GetStatus()
// ---------------------------------------------------------------------------
//
TRequestStatus* CLbtDeleteAsyncOperation::GetStatus()
    {
    return iClientStatus;
    }
    
// ---------------------------------------------------------------------------
// CLbtDeleteAsyncOperation::~CLbtDeleteAsyncOperation()
// ---------------------------------------------------------------------------
//
 CLbtDeleteAsyncOperation::~CLbtDeleteAsyncOperation()
    {
    delete iFilter;
    }

// ---------------------------------------------------------------------------
// Constructor
//
// ---------------------------------------------------------------------------
//
CLbtDeleteAsyncOperation::CLbtDeleteAsyncOperation(TLbtSecurityPolicy aSecurityPolicy): 
								iClientStatus(NULL),
								iFilter(NULL),
								iSecurityPolicy(aSecurityPolicy)
    {   

    }

// end of file


