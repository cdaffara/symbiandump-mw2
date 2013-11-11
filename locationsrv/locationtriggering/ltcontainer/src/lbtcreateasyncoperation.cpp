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
#include "lbtcreateasyncoperation.h"
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
// CLbtCreateAsyncOperation::NewL(
//    CLbtTriggerEntry* aEntry,
//    TLbtTriggerDynamicInfo* aDynInfo,
//    TLbtDerivedTriggerInfo* aDerivedInfo)
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CLbtCreateAsyncOperation* CLbtCreateAsyncOperation::NewL(CLbtContainerTriggerEntry* aEntry,TRequestStatus& aClientStatus)
    {
    CLbtCreateAsyncOperation* self=new( ELeave ) CLbtCreateAsyncOperation;
	CleanupStack::PushL( self );
    self->ConstructL(aEntry,aClientStatus);
    CleanupStack::Pop( self ); 
    return self;
    }    

// ---------------------------------------------------------------------------
// CLbtCreateAsyncOperation::StartAOOperationL
// ---------------------------------------------------------------------------
//
void CLbtCreateAsyncOperation::StartAOOperationL(MLbtTriggerStore* aStore,TRequestStatus &aStatus)
    {
    FUNC_ENTER("CLbtCreateAsyncOperation::StartAOOperationL");
    aStatus = KRequestPending;
    CLbtTriggerEntry* trigger = iEntry->TriggerEntry();
	if ( aStore->SupportedTriggerTypes() & trigger->Type() )
		{
		TRAPD(error, aStore->CreateTriggerL(*iEntry,aStatus));
		
		if(error != KErrNone && aStatus == KRequestPending)
			{
			TRequestStatus* status = &aStatus;
			User::RequestComplete(status, error);
			}
		}
	else 
		{
		TRequestStatus *status = &aStatus;
		User::RequestComplete(status, KErrNotFound); 
		}           
    }

// ---------------------------------------------------------------------------
// CLbtCreateAsyncOperation::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtCreateAsyncOperation::ConstructL( CLbtContainerTriggerEntry* aEntry,TRequestStatus& aClientStatus)  
    {
	iEntry = aEntry;  
    iClientStatus = &aClientStatus;    
    }

// ---------------------------------------------------------------------------
// CLbtCreateAsyncOperation::TriggerEntry
// ---------------------------------------------------------------------------
//
const CLbtContainerTriggerEntry* CLbtCreateAsyncOperation::ContainerTriggerEntry()
	{
	return iEntry;
	}

// ---------------------------------------------------------------------------
// CLbtCreateAsyncOperation::GetStatus
// ---------------------------------------------------------------------------
//	
TRequestStatus* CLbtCreateAsyncOperation::GetStatus()
    {
    return iClientStatus;
    }

// ---------------------------------------------------------------------------
// CLbtCreateAsyncOperation::~CLbtCreateAsyncOperation()
// ---------------------------------------------------------------------------
//
 CLbtCreateAsyncOperation::~CLbtCreateAsyncOperation()
    {
   
    }

// ---------------------------------------------------------------------------
// CLbtCreateAsyncOperation::GetAOOperationType
// ---------------------------------------------------------------------------
//
TContainerAOOperation CLbtCreateAsyncOperation::GetAOOperationType()
	{
	return EOpCreateTrigger;
	}

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CLbtCreateAsyncOperation::CLbtCreateAsyncOperation()
    {   

    }

// end of file

