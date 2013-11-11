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
* Description:  This file implements the Location triggering container
*                interface
*
*/


#include <lbtlisttriggeroptions.h>
#include <e32math.h>

#include "lbtcontainer.h"
#include "lbttriggerstoreinterface.h"
#include "lbttriggerstorerepository.h"
#include "lbttriggeridgenerator.h"
#include "lbtcontainerao.h"
#include "lbtcontainerupdatefilter.h"
#include "lbttriggerchangeobserver.h"
#include "lbtlogger.h"

// ========= Static member variable initialization =================

// Initalize the member variable for shared Container instance.
CLbtContainer* CLbtContainer::iContainerInstance = NULL;

// Initialize the reference count for the container instance.
TInt CLbtContainer::iRefCount = 0;


// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Comparison Algorithm for 2 objects of type TLbtTriggerStoreChangeObserver
// The idea is that the 2 TLbtTriggerStoreChangeObserver structures are same 
// if they have the same observer object.
// The event mask is not required in the comparison.
// ---------------------------------------------------------------------------
//
TBool ObserverCompare(
    const CLbtContainer::TLbtTriggerStoreChangeObserver& lhs, 
    const CLbtContainer::TLbtTriggerStoreChangeObserver& rhs)
    {
    if ( lhs.iObserver == rhs.iObserver )
        {
        return ETrue;
        }
    return EFalse;
    }

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// The Symbian 2 phase constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtContainer* CLbtContainer::NewL()
    {
    FUNC_ENTER("CLbtContainer::NewL");
    // If an instance doesn't exist then create the Container object.
    if ( !iContainerInstance )
        {
        CLbtContainer* container = new( ELeave ) CLbtContainer();

        CleanupStack::PushL( container );
        container->ConstructL();
        CleanupStack::Pop( container );
        iContainerInstance = container;
        }
    
    // Increment the reference count
    iRefCount++;
    return iContainerInstance;
    }
    

// ---------------------------------------------------------------------------
// Destroys the Container Instance
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainer::Destroy()
    {
    FUNC_ENTER("CLbtContainer::Destroy");
    // Check that the Container Instance exists
    if ( iContainerInstance )
        {
        iRefCount--;
        // If the Reference Count is 0 then delete the container Instance.
        if ( !iRefCount )
            {
            delete iContainerInstance;
            iContainerInstance = NULL;
            }
        }
    }


// ---------------------------------------------------------------------------
// Create a Trigger
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainer::CreateTrigger(
    CLbtContainerTriggerEntry& aEntry,
    TInt& aOpId,
    TRequestStatus& aStatus,
    TLbtSecurityPolicy /*aSecurityPolicy*/)
    {
    FUNC_ENTER("CLbtContainer::CreateTrigger");

    // Allocate the Trigger ID First
    CLbtTriggerEntry* trigger = aEntry.TriggerEntry();
   
	if(iTrigId == 0)
		{
		iTrigId=iTrigIdGenerator->GetTriggerId();
		}

	trigger->SetId(++iTrigId);
	iTrigIdGenerator->SetTriggerId(iTrigId);	
	aOpId = GenerateRandomOpCode();
	aStatus=KRequestPending;   
	TRAPD(err, iContainerAO->CreateTriggerL(&aEntry,aOpId,aStatus)  ) ;

	if(err != KErrNone && aStatus == KRequestPending)
		{
		TRequestStatus* status = &aStatus;
	  	User::RequestComplete( status, err );	
		}
    }

// ---------------------------------------------------------------------------
// CancelAsyncOperation
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainer::CancelAsyncOperation(TInt aOpId)
    {
    iContainerAO->CancelAsyncRequest(aOpId);
    }

// ---------------------------------------------------------------------------
// CLbtContainer::GetTriggers
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainer::GetTriggers( const RArray<TLbtTriggerId>& aTriggerIds,
			    						  RPointerArray < CLbtContainerTriggerEntry >& aTriggers,
			    						  TInt& aOpId,
			    						  TRequestStatus& aStatus,
			    						  TLbtSecurityPolicy aSecurityPolicy )
	{
	FUNC_ENTER("CLbtContainer::GetTriggers");
	aOpId = GenerateRandomOpCode();
    aStatus=KRequestPending;
    
    TRAPD(err,iContainerAO->GetTriggersL( aTriggerIds, 
    									  aTriggers, 
    									  aOpId, 
    									  aStatus,
    									  aSecurityPolicy ));
    if(err != KErrNone && aStatus == KRequestPending)
    	{
    	TRequestStatus* status = &aStatus;
        User::RequestComplete( status, err );	
    	}
	}

// ---------------------------------------------------------------------------
// CLbtContainer::UpdateTriggerFiredState
// ---------------------------------------------------------------------------
//    
EXPORT_C void CLbtContainer::UpdateTriggerFiredState( RArray<TLbtTriggerId>& aTriggerIds,
							                		  TInt& aOpId,
							                		  TBool aFireBool,
						                    		  TRequestStatus& aStatus )
    {
    FUNC_ENTER("CLbtContainer::UpdateTriggerFiredState");
    aOpId = GenerateRandomOpCode();
    aStatus=KRequestPending;
    
    TRAPD(err,iContainerAO->UpdateTriggerFiredStateL(aTriggerIds, aFireBool, aOpId, aStatus));
    if(err != KErrNone && aStatus == KRequestPending)
    	{
    	TRequestStatus* status = &aStatus;
        User::RequestComplete( status, err );	
    	}
    }
    
// ---------------------------------------------------------------------------
// UpdateTriggerL
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainer::UpdateTriggerL( CLbtContainerTriggerEntry& aEntry,
    										 TLbtTriggerDataMask aDataMask,
    										 TLbtTriggerAttributeFieldsMask aAttrMask,
    										 TInt& aOpId,
    										 TRequestStatus& aStatus,
    										 TLbtSecurityPolicy aSecurityPolicy )
    {
    FUNC_ENTER("CLbtContainer::UpdateTriggerL");
    aOpId = GenerateRandomOpCode();
    aStatus=KRequestPending;
    
    TRAPD( err, iContainerAO->UpdateTriggerL( aEntry,
    									      aDataMask,
    									      aAttrMask,
    									      aOpId,
    									      aStatus,
    									      aSecurityPolicy) );
    if(err != KErrNone && aStatus == KRequestPending)
    	{
    	TRequestStatus* status = &aStatus;
        User::RequestComplete( status, err );	
    	}
    }    
    
    
// ---------------------------------------------------------------------------
// UpdateTriggersState
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainer::UpdateTriggersState( CLbtTriggerEntry::TLbtTriggerState aState,
												  CLbtContainerUpdateFilter* aFilter,
												  TInt& aOpId,
												  TLbtFireOnUpdate aFireOnUpdate,
												  TRequestStatus& aStatus,
												  TLbtSecurityPolicy aSecurityPolicy )
    {
    FUNC_ENTER("CLbtContainer::UpdateTriggersState");
    aOpId = GenerateRandomOpCode();
    aStatus=KRequestPending;
    
    TRAPD(err, iContainerAO->UpdateTriggersStateL( aState,
    											   aFilter,
    											   aOpId,
    											   aFireOnUpdate,
    											   aStatus,
    											   aSecurityPolicy ));
    if(err != KErrNone && aStatus == KRequestPending)
    	{
    	TRequestStatus* status = &aStatus;
        User::RequestComplete( status, err );	
    	}
    }

    
// ---------------------------------------------------------------------------
// UpdateTriggersValidity
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainer::UpdateTriggersValidity( TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity,
												     RArray <TLbtTriggerId>& aTriggerIds,
												     TInt& aOpId,
												     TRequestStatus& aStatus,
												     TLbtSecurityPolicy aSecurityPolicy )
    {
    FUNC_ENTER("CLbtContainer::UpdateTriggersValidity");
    aOpId = GenerateRandomOpCode();
    aStatus=KRequestPending;
   
    TRAPD(err, iContainerAO->UpdateTriggersValidityL( aValidity,
    											 	  aTriggerIds,
    											 	  aOpId,
    											 	  aStatus,
    											 	  aSecurityPolicy ));
    if(err != KErrNone && aStatus == KRequestPending)
    	{
    	TRequestStatus* status = &aStatus;
        User::RequestComplete( status, err );	
    	}
    }

// ---------------------------------------------------------------------------
// DeleteTriggers
// ---------------------------------------------------------------------------
//    
EXPORT_C void CLbtContainer::DeleteTriggers(
    CLbtContainerUpdateFilter* aFilter,
    TInt& aOpId,
    TRequestStatus& aStatus,
    TLbtSecurityPolicy aSecurityPolicy)
    {
    FUNC_ENTER("CLbtContainer::DeleteTriggers")
    aOpId = GenerateRandomOpCode();
    aStatus=KRequestPending;
    
    TRAPD(err, iContainerAO->DeleteTriggersL( aFilter,
    		 								  aOpId,
    										  aStatus,
    										  aSecurityPolicy ));
    if(err != KErrNone && aStatus == KRequestPending)
    	{
    	TRequestStatus* status = &aStatus;
        User::RequestComplete( status, err );	
    	}
    }

// ---------------------------------------------------------------------------
// CLbtContainer::ListTriggers
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainer::ListTriggers(
	CLbtContainerListOptions* aListOptions,
    RPointerArray < CLbtContainerTriggerEntry >& aTriggers,
    TInt& aOpId,
    TRequestStatus& aStatus,
    TLbtSecurityPolicy aSecurityPolicy)
    {
    FUNC_ENTER("CLbtContainer::ListTriggers");
    aOpId = GenerateRandomOpCode();
    aStatus=KRequestPending;
    
    TRAPD(err, iContainerAO->ListTriggersL( aListOptions,
    									    aTriggers,
    									    aOpId,
    									    aStatus,
    									    aSecurityPolicy ));
    if(err != KErrNone && aStatus == KRequestPending)
    	{
    	TRequestStatus* status = &aStatus;
        User::RequestComplete( status, err );	
    	}
    }


    
// ---------------------------------------------------------------------------
// SetChangeObserver
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainer::SetChangeObserver(
    MLbtContainerChangeEventObserver* aObserver,
    TLbtTriggerEventMask aEventMask)
    {
    FUNC_ENTER("CLbtContainer::SetChangeObserver");
    TLbtTriggerStoreChangeObserver obsvr;
    obsvr.iObserver = aObserver;
    obsvr.iEventMask = aEventMask;
    
    TIdentityRelation<TLbtTriggerStoreChangeObserver> compareAlgoirthm(ObserverCompare);
    
    if ( KErrNotFound == iObservers.Find(obsvr, compareAlgoirthm) )
        {
        TInt error = iObservers.Append( obsvr );
        if( error != KErrNone )
            {
            LOG1("Failed to add observer to the array:%d",error);
            }
        }
    }

// ---------------------------------------------------------------------------
// RemoveObserver
// ---------------------------------------------------------------------------
//

EXPORT_C void CLbtContainer::RemoveObserver(MLbtContainerChangeEventObserver* aObserver)
    {
    FUNC_ENTER("CLbtContainer::RemoveObserver");
    TLbtTriggerStoreChangeObserver obsvr;
    obsvr.iObserver = aObserver;
    
    TIdentityRelation<TLbtTriggerStoreChangeObserver> compareAlgoirthm(ObserverCompare);
    
    TInt index = iObservers.Find( obsvr, compareAlgoirthm );
    if ( KErrNotFound != index )
        {
        iObservers.Remove( index );
        }
    }

// ---------------------------------------------------------------------------
// GetCountOfEnabledAndValidTriggers
// ---------------------------------------------------------------------------
//

EXPORT_C TInt CLbtContainer::GetCountOfEnabledAndValidTriggers()
    {
    TInt count=0;
    count = iTriggerStores[0]->GetCountOfEnabledAndValidTriggers() +
            iTriggerStores[1]->GetCountOfEnabledAndValidTriggers();
    return count;
    }

// ---------------------------------------------------------------------------
// CLbtContainer::SetTimeTillCompaction
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainer::SetTimeTillCompaction(TTime aTime)
	{
	for(TInt i=0;i<iTriggerStores.Count();++i)
		{
		iTriggerStores[i]->SetTimeTillCompaction(aTime);
		}
	}

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CLbtContainer::CLbtContainer()
    {
    // Nothing to do here
    }


// ---------------------------------------------------------------------------
// The 2nd phase Symbian Constructor
// ---------------------------------------------------------------------------
//
void CLbtContainer::ConstructL()
    {
    FUNC_ENTER("CLbtContainer::ConstructL");
    LbtTriggerStoreFactory::CreateTriggerStoresL(iTriggerStores);
    iTrigIdGenerator = CLbtTriggerIdGenerator::NewL();
    iTrigId=0;
    iContainerAO = CLbtContainerAO::NewL(iTriggerStores,iObservers);    
    iRandNumRef = 100;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CLbtContainer::~CLbtContainer()
    {
    FUNC_ENTER("CLbtContainer::~CLbtContainer");
    iTriggerStores.ResetAndDestroy();
    
    // We don't take ownership of the observers. Hence we just need to release
    // the resources of the array.
    iObservers.Close();
    
    delete iContainerAO;
    delete iTrigIdGenerator;    
    }

// ---------------------------------------------------------------------------
// CLbtContainer::GenerateRandomOpCode
// ---------------------------------------------------------------------------
//
TInt CLbtContainer::GenerateRandomOpCode()
	{
	TInt num = Math::Rand(iRandNumRef);
	return num;
	}
