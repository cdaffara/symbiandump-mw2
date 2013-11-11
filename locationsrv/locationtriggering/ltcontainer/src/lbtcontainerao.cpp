/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  location triggering server client interface
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <lbtstartuptrigger.h>
#include "lbtcontainerao.h"
#include "lbtlisttriggerobserver.h"
#include "lbtupdateasyncoperation.h"
#include "lbtlistasyncoperation.h"
#include "lbtdeleteasyncoperation.h"
#include "lbtcreateasyncoperation.h"
#include "lbtdbtriggersmanager.h"
#include "lbtramtriggersmanager.h"
#include "lbttriggerchangeobserver.h"
#include "lbtcontainer.h"
#include "lbttriggerconditionarea.h"
#include "lbttriggerstoreinterface.h"
#include "lbtcontainerextendedtriggerinfo.h"
#include "lbtlogger.h"





// ================= LOCAL FUNCTIONS ========================
 
// ================= MEMBER FUNCTIONS =======================

// ------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ------------------------------------------------------------------
//

CLbtContainerAO::CLbtContainerAO(RPointerArray<MLbtTriggerStore>& aTriggerStores,
                                 RArray<CLbtContainer::TLbtTriggerStoreChangeObserver>& aObservers ):
								 CActive(EPriorityStandard),
								 iClientStatus(NULL),
								 iAsyncRequest(EOpNone),
								 iTriggerStores(aTriggerStores),
								 iTriggerStoreCtr (0),
								 iObservers(aObservers)
    
    {   
    CActiveScheduler::Add(this);    
    }


// ------------------------------------------------------------------
// CLbtContainerAO::ConstructL.
// ------------------------------------------------------------------
//
void CLbtContainerAO::ConstructL()
    {
    for(TInt i=0;i<iTriggerStores.Count();++i)
    	{
    	iTriggerStores[i]->SetStoreChangeObserver(this);
    	}	
    }


// ------------------------------------------------------------------
// CLbtContainerAO::NewL
// ------------------------------------------------------------------
//
CLbtContainerAO* CLbtContainerAO::NewL(RPointerArray<MLbtTriggerStore>& aTriggerStores,
                                       RArray<CLbtContainer::TLbtTriggerStoreChangeObserver>& aObservers )
    {
    FUNC_ENTER("CLbtContainerAO::NewL");
    CLbtContainerAO* self = new (ELeave) CLbtContainerAO(aTriggerStores,aObservers);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
   

// ------------------------------------------------------------------
// CLbtContainerAO::~CLbtContainerAO
// ------------------------------------------------------------------
//
CLbtContainerAO::~CLbtContainerAO()
    {
    FUNC_ENTER("CLbtContainerAO::~CLbtContainerAO");
	if(IsActive())      
		{
		Cancel();
		}	
	iAsyncOpQueue.ResetAndDestroy();
    }

// ------------------------------------------------------------------
// CLbtContainerAO::AddAsyncOpToQueue
// ------------------------------------------------------------------
//
void CLbtContainerAO::AddAsyncOpToQueue( CLbtContainerAOOperation* aAsyncOp)
    {
    FUNC_ENTER("CLbtContainerAO::AddAsyncOpToQueue");
    // Append to the queue of asynchronous requests.
    if(aAsyncOp)
        {
        iAsyncOpQueue.Append(aAsyncOp);
        // if AO is not active, start serving the current request.
        StartNextAsyncOperation();
        }        
    }

// ------------------------------------------------------------------
// CLbtContainerAO::StartNextAsyncOperation
// ------------------------------------------------------------------
//
void CLbtContainerAO::StartNextAsyncOperation()
    {
    FUNC_ENTER("CLbtContainerAO::StartNextAsyncOperation");
    // Check if any of the trigger stores are busy
    for(TInt i=0;i<iTriggerStores.Count();++i)
    	{
    	if(iTriggerStores[i]->IsStoreBusy())
    		{
    		return;
    		}
    	}

    if(!IsActive())
        {
        if(iTriggerStores.Count() > 0)
            {
            iTriggerStoreCtr = -1; 
            iAsyncOpQueue[0]->SetPrevReturnCode(KErrNotReady);
            iAsyncOpQueue[0]->SetCurrentReturnCode(KErrNotReady);
            iAsyncRequest = iAsyncOpQueue[0]->GetAOOperationType();
            }
        else
            {
            TRequestStatus* status = iAsyncOpQueue[0]->GetStatus();
            User::RequestComplete(status,KErrNotSupported);
            return;
            }
        iStatus = KRequestPending;    
        TRequestStatus* status = &iStatus;    
        SetActive();
        /* Do a Self Complete so that the RunL() gets scheduled */
        User::RequestComplete(status,KErrNone);
        }
    }

// ------------------------------------------------------------------
// CLbtContainerAO::GetTriggersL
// ------------------------------------------------------------------
//
void CLbtContainerAO::GetTriggersL( const RArray<TLbtTriggerId>& aTriggerIds,
								    RPointerArray < CLbtContainerTriggerEntry >& aTriggers,
								    TInt& aOpId,
								    TRequestStatus& aStatus,
								    TLbtSecurityPolicy aSecurityPolicy )
	{
	FUNC_ENTER("CLbtContainerAO::GetTriggers");
	CLbtListAsyncOperation* listop = CLbtListAsyncOperation::NewL( aTriggerIds,
    															   aTriggers,
    															   aSecurityPolicy,
    															   aStatus);
    listop->SetOpCode( aOpId );
    AddAsyncOpToQueue( listop );
	}

// ------------------------------------------------------------------
// CLbtContainerAO::ListTriggersL
// ------------------------------------------------------------------
//
void CLbtContainerAO::ListTriggersL( CLbtContainerListOptions* aFilter,
    								 RPointerArray < CLbtContainerTriggerEntry >& aTriggers,
								     TInt aOpCode,
								     TRequestStatus& aStatus,
								     TLbtSecurityPolicy aSecurityPolicy )
    {
    FUNC_ENTER("CLbtContainerAO::ListTriggersL");
    iSortingOption = aFilter->ListOptions()->SortingOption();
    CLbtListAsyncOperation* listop = CLbtListAsyncOperation::NewL( aFilter,
    															   aTriggers,
    															   aSecurityPolicy,
    															   aStatus);
    listop->SetOpCode(aOpCode);
    AddAsyncOpToQueue(listop);
    }

// ------------------------------------------------------------------
// CLbtContainerAO::UpdateTriggerL
// ------------------------------------------------------------------
//    
void CLbtContainerAO::UpdateTriggerL( CLbtContainerTriggerEntry& aEntry,
        							  TLbtTriggerDataMask aDataMask,
        							  TLbtTriggerAttributeFieldsMask aAttrMask,
        							  TInt aOpCode,
        							  TRequestStatus& aStatus,
        							  TLbtSecurityPolicy aSecurityPolicy )
    {
    FUNC_ENTER("CLbtContainerAO::UpdateTriggerL");
    iDataMask = aDataMask;
    iAttrMask = aAttrMask;
    CLbtUpdateAsyncOperation* updateop = CLbtUpdateAsyncOperation::NewL( aEntry,
    																	 aDataMask,
    																	 aAttrMask,
    																	 aStatus,
    																	 aSecurityPolicy,
    																	 EOpUpdateTrigger);
    updateop->SetOpCode(aOpCode);
    AddAsyncOpToQueue(updateop);
    }

// ------------------------------------------------------------------
// CLbtContainerAO::UpdateTriggerL
// ------------------------------------------------------------------
//
void CLbtContainerAO::UpdateTriggerFiredStateL( RArray<TLbtTriggerId>& aTriggerIds,
											    TBool aFireBool,
											    TInt aOpCode,
					                    	    TRequestStatus& aStatus )
	{
	FUNC_ENTER("CLbtContainerAO::UpdateTriggerL - Filter");
	aStatus = KRequestPending;
    CLbtUpdateAsyncOperation* updateop = CLbtUpdateAsyncOperation::NewL( aTriggerIds, 
    																	 aStatus,
    																	 aFireBool,
    																	 KLbtNullSecurity,
    																	 EOpUpdateTriggersFiredState );
    updateop->SetOpCode(aOpCode);
    AddAsyncOpToQueue(updateop);
	}
        
// ------------------------------------------------------------------
// CLbtContainerAO::UpdateTriggersStateL
// ------------------------------------------------------------------
//
void  CLbtContainerAO::UpdateTriggersStateL( CLbtTriggerEntry::TLbtTriggerState aState,
            								 CLbtContainerUpdateFilter* aFilter,
            								 TInt aOpCode,
            								 TLbtFireOnUpdate aFireOnUpdate,
            								 TRequestStatus& aStatus,
            								 TLbtSecurityPolicy aSecurityPolicy ) 
    {
    FUNC_ENTER("CLbtContainerAO::UpdateTriggersStateL");
    aStatus = KRequestPending;
    CLbtUpdateAsyncOperation* updateop = CLbtUpdateAsyncOperation::NewL( aState,
    																	 aFilter,
    																	 aFireOnUpdate,
    																	 aStatus,    																	 
    																	 aSecurityPolicy,
    																	 EOpUpdateTriggerState);
    updateop->SetOpCode(aOpCode);
    AddAsyncOpToQueue(updateop);
    }
 
 
// ------------------------------------------------------------------
// CLbtContainerAO::UpdateTriggersValidityL
// ------------------------------------------------------------------
//            
void CLbtContainerAO::UpdateTriggersValidityL( TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity,
            								   RArray <TLbtTriggerId>& aTriggerIds,
            								   TInt aOpCode,
            								   TRequestStatus& aStatus,
            								   TLbtSecurityPolicy aSecurityPolicy ) 
            
    {
    FUNC_ENTER("CLbtContainerAO::UpdateTriggersValidityL");
    CLbtUpdateAsyncOperation* updateop = CLbtUpdateAsyncOperation::NewL( aValidity,
    																	 aTriggerIds,
    																	 aStatus,
    																	 aSecurityPolicy,
    																	 EOpUpdateTriggersValidity);
    updateop->SetOpCode(aOpCode);
    AddAsyncOpToQueue(updateop);
    }
    
    
// ------------------------------------------------------------------
// CLbtContainerAO::DeleteTriggersL
// ------------------------------------------------------------------
//    
void CLbtContainerAO::DeleteTriggersL( CLbtContainerUpdateFilter* aFilter,
									   TInt aOpCode,
									   TRequestStatus& aStatus,
									   TLbtSecurityPolicy aSecurityPolicy )
    {
    FUNC_ENTER("CLbtContainerAO::DeleteTriggersL");
    CLbtDeleteAsyncOperation* deleteop = CLbtDeleteAsyncOperation::NewL(aFilter, aStatus, aSecurityPolicy);
    deleteop->SetOpCode(aOpCode);
    AddAsyncOpToQueue(deleteop);
    }

// ------------------------------------------------------------------
// CLbtContainerAO::CreateTriggerL
// ------------------------------------------------------------------
//    
void CLbtContainerAO::CreateTriggerL(CLbtContainerTriggerEntry* aEntry,TInt aOpCode,TRequestStatus& aStatus) 
 	{
 	FUNC_ENTER("CLbtContainerAO::CreateTriggerL");
 	CLbtCreateAsyncOperation* createop = CLbtCreateAsyncOperation::NewL(aEntry,aStatus);
 	createop->SetOpCode(aOpCode);
 	AddAsyncOpToQueue(createop);	
 	}

// ---------------------------------------------------------
// CLbtContainerAO::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtContainerAO::RunL()
    {
    FUNC_ENTER("CLbtContainerAO::RunL");
   	TInt error=iStatus.Int();
   	
   	if( error == KErrCancel )
   		{
   		if( iAsyncOpQueue.Count() > 0 )
			{
			StartNextAsyncOperation();
			}
		return;
   		}
   	
   	if(iTriggerStoreCtr < iTriggerStores.Count() - 1)
   	    {
        iTriggerStoreCtr++;
        if(iTriggerStoreCtr == 0)
            {             
            iAsyncOpQueue[0]->StartAOOperationL( iTriggerStores[iTriggerStoreCtr],iStatus); 
            SetActive();
            }
        else if(iTriggerStoreCtr == 1)
            {
            if(error == KErrNone || error == KErrNotFound)
                {
                iAsyncOpQueue[0]->SetCurrentReturnCode(error);                
                iAsyncOpQueue[0]->StartAOOperationL( iTriggerStores[iTriggerStoreCtr],iStatus);
                SetActive();                
                }
            else 
                {
                CompleteAsyncRequest(error);
                if(iAsyncOpQueue.Count()>0)
   	                {
           	        StartNextAsyncOperation();
                    }
                }
            }
        else if(iTriggerStoreCtr>1)
            {
            TInt prevError = iAsyncOpQueue[0]->GetPrevReturnCode();
            TInt currError = iAsyncOpQueue[0]->GetCurrentReturnCode();
            
            /* if previous error is KErrNotFound and current is KErrNone
             * then make previous as KErrNone 
             * else  if prev is KErrNone, that means that the operation has
             * succeeded in atleast one trigger store so,
             * let the prev code remain KErrNone
             * even if the currErr is KErrNotFound
             */
            if((prevError == KErrNotFound || prevError == KErrNotReady) && (currError == KErrNone || currError == KErrNotReady))
                {
                iAsyncOpQueue[0]->SetPrevReturnCode(currError);
                }
            
            /* Now once the prev code is modified, set the current code
             * to the one this operation returned.
             */
            
            iAsyncOpQueue[0]->SetCurrentReturnCode(error);
            TInt prevCode,currCode;
            prevCode = iAsyncOpQueue[0]->GetCurrentReturnCode();
            currCode = iAsyncOpQueue[0]->GetPrevReturnCode();
            
            /* Start the operation with the next trigger store only if
             * both the prevCode and CurrCode  are KErrNone or KErrNotFound
             * else complete the request with the error code
             */
            if(((prevCode == KErrNotFound) ||( prevCode == KErrNone) || (prevCode == KErrNotReady))&&(currCode == KErrNotFound || currCode == KErrNone))
                {
                 if(iTriggerStoreCtr<iTriggerStores.Count())
                    {
                    iAsyncOpQueue[0]->StartAOOperationL( iTriggerStores[iTriggerStoreCtr],iStatus);
                    SetActive();   
                    }
                  else
                    {
                    CompleteAsyncRequest(currCode);
                    if(iAsyncOpQueue.Count()>0)
   	                    {
               	        StartNextAsyncOperation();
                        }
                    } 
                 
                }
            else
                {
                CompleteAsyncRequest(currCode);
                 if(iAsyncOpQueue.Count()>0)
   	                {
           	        StartNextAsyncOperation();
                    }
                }
            }
   	    }
   	else
   	    {
        TInt prevCode = iAsyncOpQueue[0]->GetPrevReturnCode();
        TInt currCode = iAsyncOpQueue[0]->GetCurrentReturnCode();    
        
        if(prevCode == KErrNone || currCode == KErrNone || error == KErrNone)
            {
            CompleteAsyncRequest(KErrNone);
            }
        else 
            {
            CompleteAsyncRequest(error);
            }   
             
   	    if(iAsyncOpQueue.Count()>0)
   	        {
   	        StartNextAsyncOperation();
   	        }
   	    }    
    }

// ------------------------------------------------------------------
// CLbtContainerAO::CompleteAsyncRequest
// ------------------------------------------------------------------
//
void CLbtContainerAO::CompleteAsyncRequest(TInt aErr)
    {
    FUNC_ENTER("CLbtContainerAO::CompleteAsyncRequest");
    CLbtContainerAOOperation* asyncObjPtr = iAsyncOpQueue[0];    
                
    if( (iAsyncRequest == EOpCreateTrigger) && 
        (aErr == KErrNone) )
		{
		CLbtCreateAsyncOperation* createop = static_cast <CLbtCreateAsyncOperation*> (asyncObjPtr);
		
		RArray<TLbtTriggerId> triggerIds;
		CLbtContainerTriggerEntry* entry = const_cast<CLbtContainerTriggerEntry*>(createop->ContainerTriggerEntry());
   		TInt error = triggerIds.Append(entry->TriggerEntry()->Id());
   		if( error != KErrNone )
   		    {
            LOG1("Failed to append trigger ids to array:%d",error); 
   		    }
   		
   		TLbtTriggerEventMask eventMask;
   		
   		// Append the manager uids and owner uids into the array
   		RArray<TUid> managerui;
   		error = managerui.Append( entry->TriggerEntry()->ManagerUi() );
        if( error != KErrNone )
            {
            LOG1("Failed to append trigger ids to array:%d",error); 
            }
   		
   		TUid ownerUid;
   		ownerUid.iUid = (TInt)( entry->ExtendedTriggerInfo()->OwnerSid().iId);
   		managerui.Append( ownerUid );
   		
   		if( entry->TriggerEntry()->Type() == CLbtTriggerEntry::ETypeStartup )
   			{
   			CLbtStartupTrigger* startupTrigger = static_cast<CLbtStartupTrigger*>(entry->TriggerEntry());
			TSecureId secureId;
			TBuf16<KMaxFileName> fileName;
			startupTrigger->GetProcessId(fileName, secureId);
			TUid startupUid;
			startupUid.iUid = (TInt)(secureId.iId);
			
			if( startupUid != KNullUid )
				{				
				managerui.Append( startupUid );
				}
   			}   		
   
   		// Give the observer callback through interface
		for( TInt i = 0; i<iObservers.Count(); i++ )
			{
			TInt isRequested = EFalse;	
			ProcessFilter(isRequested,iObservers[i].iEventMask,eventMask);
			if(isRequested)
				{
				MLbtContainerChangeEventObserver::TLbtContainerChangedAreaType changeType = 
										MLbtContainerChangeEventObserver::ETypeUnKnown;
				const CLbtTriggerConditionArea* conditionBase = 
						static_cast<const CLbtTriggerConditionArea*>(entry->TriggerEntry()->GetCondition());				
				if(conditionBase)
					{
					CLbtGeoAreaBase* areaBase = conditionBase->TriggerArea();
					
					if(areaBase)
						{
						CLbtGeoAreaBase::TGeoAreaType areaType = areaBase->Type();
					
						if(areaType == CLbtGeoAreaBase::ECircle)
							{
							changeType = MLbtContainerChangeEventObserver::ETypeCircularTriggerChange;
							}
						else if(areaType == CLbtGeoAreaBase::ECellular)
							{
							changeType = MLbtContainerChangeEventObserver::ETypeCellularTriggerChange;
							}
						else
							{
							changeType = MLbtContainerChangeEventObserver::ETypeRectangularTriggerChange;
							}
						}					
					}				
								
				//Ownership of triggersModified array is transferred to the client
		    	iObservers[i].iObserver->TriggerStoreChanged(triggerIds, CLbtContainer::ELbtConTriggerCreated, changeType, managerui);
		    	triggerIds.Reset();		    	
				}
			}
		triggerIds.Close();
		managerui.Close();
    	}

    if( EOpListTriggers == iAsyncRequest )
	    {
	    if( aErr == KErrNone)
	        {
	        RPointerArray<CLbtContainerTriggerEntry>* triggers = NULL;
	        CLbtListAsyncOperation* listAsync = static_cast<CLbtListAsyncOperation*> (asyncObjPtr);
            triggers = listAsync->GetTriggerList();

            // sort the list using sorting options            
            if( triggers )
                {
                SortTriggers(*triggers);        
                }
	        }
	    else
	        {
            CLbtListAsyncOperation* listOp = static_cast<CLbtListAsyncOperation*>(asyncObjPtr);
            RPointerArray<CLbtContainerTriggerEntry>* triggers = listOp->GetTriggerList();
            triggers->ResetAndDestroy();
	        }
	     }
    
    // Now,notify the trigger store change observers.
     if( EOpListTriggers != iAsyncRequest &&
         EOpCreateTrigger != iAsyncRequest )
		 {
		 NotifyListeners();	
		 }
     
    TRequestStatus* status = asyncObjPtr->GetStatus();
    // Sorting is completed,complete the client request.
    User::RequestComplete(status,aErr);     

	iAsyncOpQueue.Remove(0);	
	iAsyncRequest = EOpNone;
    delete asyncObjPtr;    
    }

// ---------------------------------------------------------
// CLbtContainerAO::RunError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CLbtContainerAO::RunError(TInt /*aError*/)
    {
    FUNC_ENTER("CLbtContainerAO::RunError");
    return KErrNone;
    }

// ---------------------------------------------------------
// CLbtContainerAO::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtContainerAO::DoCancel()
    {
    FUNC_ENTER("CLbtContainerAO::DoCancel");
	CLbtContainerAOOperation* cancelObj = iAsyncOpQueue[0];
	// Cancel current operation of trigger store
	
	TInt error = KErrCancel;
	if( iTriggerStoreCtr!= -1 )
	    {
	    error = iTriggerStores[iTriggerStoreCtr]->CancelCurrentOperation();
	    }
	CompleteAsyncRequest( error );	
	if(iAsyncOpQueue.Count()>0)
        {
        StartNextAsyncOperation();
        }
    }

// ---------------------------------------------------------
// CLbtContainerAO::NotifyListeners
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtContainerAO::NotifyListeners()
	{
	FUNC_ENTER("CLbtContainerAO::NotifyListeners");
	TLbtTriggerEventMask eventMask;
		
	CLbtRamTriggersManager* ramTriggerMgr = NULL;
	CLbtDbTriggersManager* dbTriggerMgr = NULL;
	
	RArray<MLbtTriggerStore::TLbtTriggerModifiedInfo> triggerInfoArray;
	
	// Get the list of triggers which were modified for a  given operation
	for( TInt i = 0; i<iTriggerStores.Count(); i++ )
		{
		if( CLbtTriggerEntry::ETypeSession == iTriggerStores[i]->SupportedTriggerTypes() )
			{
			ramTriggerMgr = static_cast<CLbtRamTriggersManager*> (iTriggerStores[i]);
			ramTriggerMgr->TriggersModified(triggerInfoArray);	
			}
		else if( CLbtTriggerEntry::ETypeStartup == iTriggerStores[i]->SupportedTriggerTypes() )
			{
			dbTriggerMgr = static_cast<CLbtDbTriggersManager*> (iTriggerStores[i]);
			dbTriggerMgr->TriggersModified(triggerInfoArray);		
			}	
		}
		
	if(triggerInfoArray.Count() <= 0)
		{
		triggerInfoArray.Close();
		// If no triggers are modified then no notification required
		return;
		}

	TInt changedAreaType = MLbtContainerChangeEventObserver::ETypeUnKnown;
	for(TInt i=0;i<triggerInfoArray.Count();++i)
		{
		if(triggerInfoArray[i].iAreaType == CLbtGeoAreaBase::ECircle)
			{
			changedAreaType = changedAreaType | MLbtContainerChangeEventObserver::ETypeCircularTriggerChange;
			}
		else if(triggerInfoArray[i].iAreaType == CLbtGeoAreaBase::ECellular)
			{
			changedAreaType = changedAreaType | MLbtContainerChangeEventObserver::ETypeCellularTriggerChange;
			}
		else if(triggerInfoArray[i].iAreaType == CLbtGeoAreaBase::ERectangular)
			{
			changedAreaType = changedAreaType | MLbtContainerChangeEventObserver::ETypeRectangularTriggerChange;
			}
		}

	RArray<TLbtTriggerId> triggersModified;
	RArray<TUid> managerUis;
	for(TInt i=0;i<triggerInfoArray.Count();++i)
		{
		TInt error = triggersModified.Append(triggerInfoArray[i].iTriggerId);
		if( error != KErrNone)
		    {
            LOG1("Failed to append modified triggers:%d",error);
		    }
		
		if( managerUis.Find(triggerInfoArray[i].iManagerUi) == KErrNotFound )
			{
			// Appen both manager ui and owner into manager uis
			managerUis.Append(triggerInfoArray[i].iManagerUi);			
			}
			
		if( managerUis.Find(triggerInfoArray[i].iOwner) == KErrNotFound )
			{
			managerUis.Append(triggerInfoArray[i].iOwner);
			}
		
		if( managerUis.Find(triggerInfoArray[i].iStartupProcess) == KErrNotFound &&
		    triggerInfoArray[i].iStartupProcess !=  KNullUid )
			{
			managerUis.Append(triggerInfoArray[i].iStartupProcess);
			}
		}

	TBool isRequested = EFalse;
	// Give the observer callback through interface
	for( TInt i = 0; i<iObservers.Count(); i++ )
		{
		isRequested = EFalse;	
		ProcessFilter(isRequested,iObservers[i].iEventMask,eventMask);
		if(isRequested)
			{
			MLbtContainerChangeEventObserver::TLbtContainerChangedAreaType areaType = 
					static_cast<MLbtContainerChangeEventObserver::TLbtContainerChangedAreaType>(changedAreaType);
			//Ownership of triggersModified array is transferred to the client
		    iObservers[i].iObserver->TriggerStoreChanged( triggersModified, 
		    											  eventMask, 
		    											  areaType,
		    											  managerUis );			
			}
		}
	triggersModified.Close();
	triggerInfoArray.Close();
	managerUis.Close();
	}

// ---------------------------------------------------------
// CLbtContainerAO::ProcessFilter
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLbtContainerAO::ProcessFilter(TBool& aIsRequested,TLbtTriggerEventMask& aRequestedMask,TLbtTriggerEventMask& aEventMask)
	{
	FUNC_ENTER("CLbtContainerAO::ProcessFilter");
	aEventMask = 0;
	if( EOpUpdateTrigger == iAsyncRequest )
		{
		// Depending on the bitmask,form trigger change event
		if( iAttrMask & CLbtTriggerEntry::EAttributeName )
			{
			aEventMask = aEventMask | CLbtContainer::ELbtConTriggerNameFieldChanged;
			if((aRequestedMask&CLbtContainer::ELbtConTriggerNameFieldChanged) == CLbtContainer::ELbtConTriggerNameFieldChanged)
				{
				aIsRequested = ETrue;
				}	
			}
		
		if( iAttrMask & CLbtTriggerEntry::EAttributeState )
			{
			aEventMask = aEventMask | CLbtContainer::ELbtConTriggerStateFieldChanged;
			if((aRequestedMask&CLbtContainer::ELbtConTriggerStateFieldChanged) == CLbtContainer::ELbtConTriggerStateFieldChanged)
				{
				aIsRequested = ETrue;
				}
			}
		
		if( iAttrMask & CLbtTriggerEntry::EAttributeCondition )
			{
			aEventMask = aEventMask | CLbtContainer::ELbtConTriggerConditionFieldChanged;
			if((aRequestedMask&CLbtContainer::ELbtConTriggerConditionFieldChanged) == CLbtContainer::ELbtConTriggerConditionFieldChanged)
				{
				aIsRequested = ETrue;
				}	
			}
		
		if( iAttrMask & CLbtTriggerEntry::EAttributeStartUpProcessId )
			{
			aEventMask = aEventMask | CLbtContainer::ELbtConTriggerStartupFieldChanged;
			if((aRequestedMask&CLbtContainer::ELbtConTriggerStartupFieldChanged) == CLbtContainer::ELbtConTriggerStartupFieldChanged)
				{
				aIsRequested = ETrue;
				}	
			}
		
		if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeRectTriggerArea )
			{
			aEventMask = aEventMask | CLbtContainer::ELbtConTriggerRectAreaFieldChanged;
			if((aRequestedMask&CLbtContainer::ELbtConTriggerRectAreaFieldChanged) == CLbtContainer::ELbtConTriggerRectAreaFieldChanged)
				{
				aIsRequested = ETrue;
				}	
			}
	
		if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeHysteresisRadius )
			{
			aEventMask = aEventMask | CLbtContainer::ELbtConTriggerHysteresisFieldChanged;
			if((aRequestedMask&CLbtContainer::ELbtConTriggerHysteresisFieldChanged) == CLbtContainer::ELbtConTriggerHysteresisFieldChanged)
				{
				aIsRequested = ETrue;
				}	
			}
		
		if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeIsFired )
			{
			aEventMask = aEventMask | CLbtContainer::ELbtConTriggerFiredFieldChanged;
			if((aRequestedMask&CLbtContainer::ELbtConTriggerFiredFieldChanged) == CLbtContainer::ELbtConTriggerFiredFieldChanged)
				{
				aIsRequested = ETrue;
				}	
			}
		
		if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeStrategyData )
			{
			aEventMask = aEventMask | CLbtContainer::ELbtConTriggerStrategyDataFieldChanged;
			if((aRequestedMask&CLbtContainer::ELbtConTriggerStrategyDataFieldChanged) == CLbtContainer::ELbtConTriggerStrategyDataFieldChanged)
				{
				aIsRequested = ETrue;
				}		
			}
		
		if( iDataMask & CLbtContainerTriggerEntry::EContainerDynInfoAttributeValidity )
			{
			aEventMask = aEventMask | CLbtContainer::ELbtConTriggerValidityFieldChanged;
			if((aRequestedMask&CLbtContainer::ELbtConTriggerValidityFieldChanged) == CLbtContainer::ELbtConTriggerValidityFieldChanged)
				{
				aIsRequested = ETrue;
				}
			}
		
		if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeIsTriggerFireOnCreation )
			{
			aEventMask = aEventMask | CLbtContainer::ELbtConTriggerFireOnCreationFieldChanged;
			if((aRequestedMask&CLbtContainer::ELbtConTriggerFireOnCreationFieldChanged) == CLbtContainer::ELbtConTriggerFireOnCreationFieldChanged)
				{
				aIsRequested = ETrue;
				}
			}                
		}
	else if( EOpUpdateTriggerState == iAsyncRequest )
		{
		//form trigger change event
		aEventMask = CLbtContainer::ELbtConTriggerStateFieldChanged;	
		if( aRequestedMask & CLbtContainer::ELbtConTriggerStateFieldChanged )
			{
			aIsRequested = ETrue;
			}	
		}
	else if( EOpUpdateTriggersValidity == iAsyncRequest )
		{
		// form trigger change event
		aEventMask = CLbtContainer::ELbtConTriggerValidityFieldChanged;
		if( aRequestedMask & CLbtContainer::ELbtConTriggerValidityFieldChanged )
			{
			aIsRequested = ETrue;
			}			
		}
	else if( EOpDeleteTriggers == iAsyncRequest )
		{
		// form trigger change event
		aEventMask = CLbtContainer::ELbtConTriggerDeleted;	
		if( aRequestedMask & CLbtContainer::ELbtConTriggerDeleted )
			{
			aIsRequested = ETrue;
			}			
		}
	else if( EOpCreateTrigger == iAsyncRequest )
		{		
		if( aRequestedMask & CLbtContainer::ELbtConTriggerCreated )
			{
			aEventMask = CLbtContainer::ELbtConTriggerCreated;
			aIsRequested = ETrue;
			}
		
		if( aRequestedMask & CLbtContainer::ELbtConTriggerEnabledAndValidCreated )
			{
			// Get the currently running ao operation object and check if the trigger is enabled and valid
			CLbtCreateAsyncOperation* operation = static_cast<CLbtCreateAsyncOperation*>(iAsyncOpQueue[0]);
			CLbtContainerTriggerEntry* entry = const_cast<CLbtContainerTriggerEntry*>(operation->ContainerTriggerEntry());
			
			if( entry->TriggerEntry()->State() == CLbtTriggerEntry::EStateEnabled && 
				entry->DynInfo()->iValidity == TLbtTriggerDynamicInfo::EValid)
				{
				// The trigger is enabled and valid, hence notify
				aEventMask = CLbtContainer::ELbtConTriggerEnabledAndValidCreated;
				aIsRequested = ETrue;
				}			
			}
		}
	}
 
// ------------------------------------------------------------------
// CLbtContainerAO::SortTriggers
// ------------------------------------------------------------------
//
void CLbtContainerAO::SortTriggers(RPointerArray<CLbtContainerTriggerEntry>& aTriggers)
	 {
	 FUNC_ENTER("CLbtContainerAO::SortTriggers");
	 if(CLbtListTriggerOptions::ELbtNoSorting == iSortingOption)
		 {
		 //No sorting
		 return;
		 }
	 else if(CLbtListTriggerOptions::ELbtTriggerNameAscending == iSortingOption)
		 {
		 //sort by name ascending
		 SortByName(aTriggers);
		 }
	else if(CLbtListTriggerOptions::ELbtTriggerNameDescending == iSortingOption)
		{
		//sort by name ascending
		// reverse the array
		SortByName(aTriggers);
		ReverseArray(aTriggers);		
		}
	else if(CLbtListTriggerOptions::ELbtDistanceToLatestLocationAscending == iSortingOption)
		{
		//sort by distance ascending
		SortByDistance(aTriggers);	
		}		
	 }

// ------------------------------------------------------------------
// CLbtContainerAO::SortByName
// ------------------------------------------------------------------
//
void CLbtContainerAO::SortByName(RPointerArray<CLbtContainerTriggerEntry>& aTriggers)
	{
	FUNC_ENTER("CLbtContainerAO::SortByName");
    TBool isFinished=EFalse;
    CLbtContainerTriggerEntry* temp=NULL;

    while(!isFinished)
	    {
	    isFinished=ETrue;
	    for(TInt i = 1 ;i <= aTriggers.Count()-1 ; i++)
		    {
		    if( aTriggers[i]->TriggerEntry()->Name() < aTriggers[i-1]->TriggerEntry()->Name() )	
			    {
			    temp=aTriggers[i-1];
			    aTriggers[i-1]=aTriggers[i];
			    aTriggers[i]=temp;
			    isFinished=EFalse;
			    }
	        }	       
	    }
	}
    
// ------------------------------------------------------------------
// CLbtContainerAO::SortByDistance
// ------------------------------------------------------------------
//        
void CLbtContainerAO::SortByDistance(RPointerArray<CLbtContainerTriggerEntry>& aTriggers)
	{
	FUNC_ENTER("CLbtContainerAO::SortByDistance");
	TBool isFinished=EFalse;
    CLbtContainerTriggerEntry* temp=NULL;

    while(!isFinished)
	    {
	    isFinished=ETrue;
	    for(TInt i = 1 ;i <= aTriggers.Count()-1 ; i++)
		    {
		    if( aTriggers[i]->DynInfo()->iDistanceToLatestLocation < aTriggers[i-1]->DynInfo()->iDistanceToLatestLocation )	
			    {
			    temp=aTriggers[i-1];
			    aTriggers[i-1]=aTriggers[i];
			    aTriggers[i]=temp;
			    isFinished=EFalse;
			    }
	        }	       
	    }
    }

// ------------------------------------------------------------------
// CLbtContainerAO::ReverseArray
// ------------------------------------------------------------------
//
void CLbtContainerAO::ReverseArray(RPointerArray<CLbtContainerTriggerEntry>& aTriggers)
	{
	FUNC_ENTER("CLbtContainerAO::ReverseArray");
	int i=0;
	int j=aTriggers.Count()-1;
	CLbtContainerTriggerEntry* temp=NULL;
	
	while(i < j)
		{
		temp=aTriggers[i];
		aTriggers[i]=aTriggers[j];
		aTriggers[j]=temp;
		i++;
		j--;
		}
	}

// ------------------------------------------------------------------
// CLbtContainerAO::CancelAsyncRequest
// ------------------------------------------------------------------
//
void CLbtContainerAO::CancelAsyncRequest(TInt aOpId)
	{
	FUNC_ENTER("CLbtContainerAO::CancelAsyncRequest");
	
	if(iAsyncOpQueue.Count() > 0)
		{
		if(iAsyncOpQueue[0]->GetOpCode() == aOpId)
			{
			// Currently running operation
			if( IsActive() )
				{
				Cancel();
				}
			else
				{
				CLbtContainerAOOperation* cancelObj = iAsyncOpQueue[0];
				iAsyncOpQueue.Remove(0);
				if( iAsyncRequest == EOpListTriggers )
					{
					CLbtListAsyncOperation* listOp = static_cast<CLbtListAsyncOperation*> (cancelObj);
					listOp->FreeArray();
					}
				TRequestStatus* status = cancelObj->GetStatus();
				User::RequestComplete(status, KErrCancel);
				delete cancelObj;
				if(iAsyncOpQueue.Count()>0)
			        {
			        StartNextAsyncOperation();
			        }
				}	
			}
		else
			{
			for(TInt i=1;i < iAsyncOpQueue.Count(); ++i)
				{
				if(iAsyncOpQueue[i]->GetOpCode() == aOpId)
					{
					CLbtContainerAOOperation* cancelObj = iAsyncOpQueue[i];
					iAsyncOpQueue.Remove(i);
					TRequestStatus* status = cancelObj->GetStatus();
					User::RequestComplete(status,KErrCancel);
					delete cancelObj;
					break;
					}
				}			
			}
		}	
	}

// ------------------------------------------------------------------
// CLbtContainerAO::NotifyTriggerStoreReadyToServe
// ------------------------------------------------------------------
//
void CLbtContainerAO::NotifyTriggerStoreReadyToServe()
	{
	FUNC_ENTER("CLbtContainerAO::NotifyTriggerStoreReadyToServe");
	if(iAsyncOpQueue.Count() > 0)
	    {
	    StartNextAsyncOperation();
	    }
	}

//  End of File
