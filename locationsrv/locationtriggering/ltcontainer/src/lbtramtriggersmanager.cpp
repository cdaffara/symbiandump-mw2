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
* Description:  This header file describes the class that handles the storage
*                of triggers in RAM Structures.
*
*/


#include <lbttriggerfilterbyattribute.h>
#include <lbtsessiontrigger.h>
#include <lbtstartuptrigger.h>
#include <lbttriggerfilterbyarea.h>
#include <lbtgeorect.h>
#include <lbttriggerfiltercomposite.h>
#include <lbtgeocircle.h>
#include <lbttriggerconditionarea.h>
#include <lbtgeocell.h>
#include <lbtgeohybrid.h>
#include "lbttriggerstoreobserver.h"
#include "lbtramtriggersmanager.h"
#include "lbtramtriggeridtree.h"
#include "lbtcontainerextendedtriggerinfo.h"
#include "lbtcontainertriggerfilter.h"
#include "lbtcontainerattrfilter.h"
#include "lbtcontainerareafilter.h"
#include "lbtcontainercompfilter.h"
#include "lbtcontainerutilities.h"
#include "lbtserverconsts.h"
#include "lbtlogger.h"


// CONSTANTS
const TInt KStepCount(2); // The step for batch processing of triggers


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// The Symbian 2 phase constructor
// ---------------------------------------------------------------------------
//
CLbtRamTriggersManager* CLbtRamTriggersManager::NewL( )
    {
    CLbtRamTriggersManager* self = new( ELeave ) CLbtRamTriggersManager;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CLbtRamTriggersManager::~CLbtRamTriggersManager()
    {
    delete iTriggerIdTree;
    delete iFilterBase;
    iIdArray.Close();
    }

// ---------------------------------------------------------------------------
// CLbtRamTriggersManager::TriggersModified
// ---------------------------------------------------------------------------
//
void CLbtRamTriggersManager::TriggersModified(RArray<TLbtTriggerModifiedInfo>& aArray)
	 {
	 for( TInt i = 0;i<iIdArray.Count();i++ )
		 {
		 TInt error = aArray.Append(iIdArray[i]);
		 if( error != KErrNone )
		     {
             LOG1("Failed to append modified triggers:%d",error);
             return;
		     }
	     }
	 iIdArray.Reset();
	 }	
	 
// ---------------------------------------------------------------------------
// Specify the types of triggers supported by the RAM Trigger Store
// ---------------------------------------------------------------------------
//
TTriggerTypeMask CLbtRamTriggersManager::SupportedTriggerTypes()
    {
    return ( CLbtTriggerEntry::ETypeSession );
    }

// ---------------------------------------------------------------------------
// CLbtRamTriggersManager::CreateTriggerL
// ---------------------------------------------------------------------------
//
void CLbtRamTriggersManager::CreateTriggerL( CLbtContainerTriggerEntry &aEntry,
                    					     TRequestStatus& aStatus )
 	{
 	FUNC_ENTER("CLbtRamTriggersManager::CreateTriggerL");
 	aStatus = KRequestPending;
 	// Save client request objects
    iClientStatus = &aStatus;
    iCurrentTriggerId = aEntry.TriggerEntry()->Id();
    iCurrentOperation = EOperationCreate;
    
 	CLbtContainerTriggerEntry* tEntry = new(ELeave) CLbtContainerTriggerEntry(aEntry);
 	
 	TRAPD(error,iTriggerIdTree->AddToTreeL(tEntry));
 	
    if( error != KErrNone )
        {
        delete tEntry;
        CompleteClientRequest( error );
        }
    else
        {
        // Reliquish control to serve cancel request
        SelfComplete();
        }           
 	}
 	
// ---------------------------------------------------------------------------
// CreateEntryBasedOnType
// ---------------------------------------------------------------------------
//
CLbtTriggerEntry* CreateEntryBasedOnType(CLbtTriggerEntry* aEntry) 	
    {
     CLbtTriggerEntry* tEntry = NULL;
     switch(aEntry->Type())
        {
         case CLbtTriggerEntry::ETypeSession: 
         	{
         	TRAP_IGNORE(tEntry = CLbtSessionTrigger::NewL());
         	break;
         	}
         case CLbtTriggerEntry::ETypeStartup:
         	{
         	TRAP_IGNORE(tEntry = CLbtStartupTrigger::NewL());
         	break;
         	}
        }
    return tEntry; 
    }
 

// ------------------------------------------------------------      
// CLbtRamTriggersManager::GetTriggersL
// ------------------------------------------------------------
//
void CLbtRamTriggersManager::GetTriggersL( RArray<TLbtTriggerId>& aTriggerIds,
										   RPointerArray<CLbtContainerTriggerEntry>& aTriggers, 
										   TLbtSecurityPolicy& aSecurityPolicy,
										   TRequestStatus &aStatus )
	{
	FUNC_ENTER("CLbtRamTriggersManager::GetTriggersL");
	iClientStatus = &aStatus;
    
    if( !aTriggerIds.Count() || 
    	iTriggerIdTree->IsEmpty() )
        {
        CompleteClientRequest(KErrNotFound);
        return;
        }
    
    // Copy trigger id array
    for( TInt i=0;i<aTriggerIds.Count();i++ )
        {
        TLbtTriggerModifiedInfo triggerInfo;
        triggerInfo.iTriggerId = aTriggerIds[i];
        iIdArray.AppendL( triggerInfo );
        }
    iIterator = 0;    
    iCurrentOperation = EOperationGetTriggers;
    iClientSecurityPolicy = aSecurityPolicy;
    iClientTriggerArray = &aTriggers;
    SelfComplete();
    }

// ------------------------------------------------------------      
// CLbtRamTriggersManager::HandleGetTriggersEventL
// ------------------------------------------------------------
//
void CLbtRamTriggersManager::HandleGetTriggersEventL()
    {
    TInt count = 0;
    while( (iIterator < iIdArray.Count()) && 
           (count < KStepCount) )
       {
       CLbtContainerTriggerEntry *entry = iTriggerIdTree->FindEntryInTreeL( iIdArray[iIterator].iTriggerId );
       if( entry )
           {
           if( LbtContainerUtilities::RunSecurityPolicy( entry, iClientSecurityPolicy ) )
               {
               CLbtContainerTriggerEntry* clientEntry = new (ELeave) CLbtContainerTriggerEntry( *entry );
               iClientTriggerArray->Append( clientEntry );
               }
           }
       ++iIterator;
       ++count;
       }
    
    if( iIterator >= iIdArray.Count() )
        {
        // All triggers iteration done. Complete request                
        CompleteClientRequest( KErrNone );
        }
    else
        {
        SelfComplete();                
        }            
    }

// ------------------------------------------------------------      
// CLbtRamTriggersManager::ListTriggersL
// ------------------------------------------------------------
//
void CLbtRamTriggersManager::ListTriggersL( CLbtContainerListOptions* aFilter,
										    RPointerArray<CLbtContainerTriggerEntry>& aTriggers,
										    TLbtSecurityPolicy& aSecurityPolicy,
										    TRequestStatus &aStatus )
   {
   FUNC_ENTER("CLbtRamTriggersManager::ListTriggersL");
   iClientStatus = &aStatus;
    
   if(aFilter == NULL)
       {
       CompleteClientRequest( KErrArgument );
       return;
       }
   
   if( iTriggerIdTree->IsEmpty() )
    	{
    	CompleteClientRequest( KErrNotFound );
        return;
    	}

   // Store client data 
   iFilter = aFilter;   
   iClientSecurityPolicy = aSecurityPolicy;
   iClientTriggerArray = &aTriggers;
   iCurrentOperation = EOperationListing;
   
   iFilterBase = LbtContainerUtilities::GetContainerFilterFromListOptionsLC(aFilter);
   CleanupStack::Pop(1); // iFilterBase

   // Set the iterator to point to the first entry in the tree
   iTriggerIdTree->SetIteratorL();
   
   SelfComplete();
   }


// ---------------------------------------------------------------------------
// CLbtRamTriggersManager::HandleListTriggerEventL
// ---------------------------------------------------------------------------
void CLbtRamTriggersManager::HandleListTriggerEventL()
    {    
    CLbtContainerTriggerEntry* tEntry = NULL;
    
    TInt count = 0;
    while( count < KStepCount )
        {
        tEntry = iTriggerIdTree->GetNextEntryL();
        
        if( tEntry == NULL )
            break;
        
        CLbtListTriggerOptions* listOptions = iFilter->ListOptions();
        CLbtExtendedTriggerInfo* contExtInfo = tEntry->ExtendedTriggerInfo();   
        TLbtTriggerDynamicInfoFieldsMask dynInfoMask;
        TLbtTriggerAttributeFieldsMask attrMask;
        listOptions->GetRetrievedFields(attrMask,dynInfoMask);
        TLbtTriggerDataMask dataMask = iFilter->DataMask();
        ++count;
        if( !LbtContainerUtilities::RunSecurityPolicy( tEntry, iClientSecurityPolicy ) )
            {
            continue;
            }
        
        CLbtContainerTriggerEntry* entry = NULL;
        CLbtTriggerEntry* clientEntry = NULL;       
        CLbtExtendedTriggerInfo* clientExtInfo = NULL;
        CLbtTriggerEntry* trigEntry = tEntry->TriggerEntry();
        TInt isFilterPresent = 0;
        TBool isEntryRequested = EFalse;
        iFilterBase->ProcessFilter(tEntry, isFilterPresent, isEntryRequested);
        
        if(isFilterPresent > 0 && isEntryRequested)
            {
            entry = CLbtContainerTriggerEntry::NewL();
            iClientTriggerArray->Append( entry );

            clientEntry = CLbtSessionTrigger::NewL();
            entry->SetTriggerEntry( clientEntry );

            if( attrMask & CLbtTriggerEntry::EAttributeId )
                {
                clientEntry->SetId(trigEntry->Id());
                }              
                
            if( attrMask & CLbtTriggerEntry::EAttributeName )
                {
                clientEntry->SetNameL(trigEntry->Name());
                }
                
            if( attrMask & CLbtTriggerEntry::EAttributeState )
                {           
                clientEntry->SetState(trigEntry->State());
                }
            
            if( attrMask & CLbtTriggerEntry::EAttributeManagerUi )
                {               
                clientEntry->SetManagerUi(trigEntry->ManagerUi());              
                }
            
            if( attrMask & CLbtTriggerEntry::EAttributeCondition )
                {
                CLbtTriggerConditionArea* cond = static_cast<CLbtTriggerConditionArea*>(trigEntry->GetCondition());
                // Condition area to be sent back to client
                CLbtTriggerConditionArea* condArea=CLbtTriggerConditionArea::NewLC();
                CLbtGeoAreaBase* area = LbtContainerUtilities::CopyGeoAreaL( cond->TriggerArea() );
                condArea->SetTriggerArea( area );
                condArea->SetDirection( cond->Direction() );
                CleanupStack::Pop( condArea );
                clientEntry->SetCondition( condArea );
                }

            TLbtTriggerDynamicInfo* info = new TLbtTriggerDynamicInfo;
            TBool isDynInfoPresent=EFalse;
                
            if( dynInfoMask & TLbtTriggerDynamicInfo::EValidityStatus )
                {
                isDynInfoPresent=ETrue;
                info->iValidity=tEntry->DynInfo()->iValidity;
                }
            
            if( dynInfoMask & TLbtTriggerDynamicInfo::EDistanceToLatestLocation )
                {               
                isDynInfoPresent=ETrue; 
                info->iDistanceToLatestLocation=tEntry->DynInfo()->iDistanceToLatestLocation;
                }
            
            if( dynInfoMask & TLbtTriggerDynamicInfo::EFiredLocality )
                {
                isDynInfoPresent=ETrue; 
                info->iFiredLocality=tEntry->DynInfo()->iFiredLocality;
                }
                    
            if(isDynInfoPresent)
                {
                entry->SetDynInfo(info);
                }
            else
                {
                delete info;
                }
            
            
            if( dataMask & CLbtContainerTriggerEntry::EContainerAttributeHysteresisRadius )
                {
                if(clientExtInfo == NULL)
                    {
                    clientExtInfo=CLbtExtendedTriggerInfo::NewL();
                    }
                clientExtInfo->SetHysteresisRadius(contExtInfo->HysteresisRadius());
                }
                        
            if( dataMask & CLbtContainerTriggerEntry::EContainerAttributeIsFired )
                {
                if(clientExtInfo == NULL)
                    {
                    clientExtInfo = CLbtExtendedTriggerInfo::NewL();
                    }
                clientExtInfo->SetTriggerFiredState(contExtInfo->IsTriggerFired());
                }
                
            if( dataMask & CLbtContainerTriggerEntry::EContainerAttributeOwnerSid )
                {
                if(clientExtInfo == NULL)
                    {
                    clientExtInfo = CLbtExtendedTriggerInfo::NewL();
                    }                  
                clientExtInfo->SetOwnerSid(contExtInfo->OwnerSid()); 
                }
                    
            if( dataMask & CLbtContainerTriggerEntry::EContainerAttributeRectTriggerArea )
                {
                if(clientExtInfo == NULL)
                    {
                    clientExtInfo = CLbtExtendedTriggerInfo::NewL();
                    }
                clientExtInfo->SetTriggerRectangleArea(contExtInfo->TriggerReactangleArea()); 
                }   
                
            if( dataMask & CLbtContainerTriggerEntry::EContainerAttributeStrategyData )
                {
                if(clientExtInfo == NULL)                   
                    {
                    clientExtInfo = CLbtExtendedTriggerInfo::NewL();
                    }
                clientExtInfo->SetStrategyDataL(contExtInfo->StategyData()); 
                }
                
            if( dataMask & CLbtContainerTriggerEntry::EContainerAttributeIsTriggerFireOnCreation )
                {
                if(clientExtInfo == NULL)
                    {
                    clientExtInfo = CLbtExtendedTriggerInfo::NewL();
                    }
                clientExtInfo->SetTriggerFireOnCreation( contExtInfo->IsTriggerFireOnCreation()); 
                }   
                        
            if( dataMask & CLbtContainerTriggerEntry::EContainerAttributeFireInfo )
                {
                if(clientExtInfo == NULL)                   
                    {
                    clientExtInfo = CLbtExtendedTriggerInfo::NewL();
                    }
                clientExtInfo->SetFiredInfo( contExtInfo->GetFiredInfo()); 
                }
            
            if(clientExtInfo!=NULL)
                {
                entry->SetExtendedTriggerInfo(clientExtInfo);
                }
            }
        }
    
    if( tEntry == NULL )
        {
        if( iClientTriggerArray->Count() == 0 )
            {
            CompleteClientRequest( KErrNotFound );
            }
        else
            {
            CompleteClientRequest( KErrNone );
            }
        }
    else
        {
        SelfComplete();
        }
    }


// ---------------------------------------------------------------------------
// CLbtRamTriggersManager::AppendTriggerInfo
// ---------------------------------------------------------------------------
void CLbtRamTriggersManager::AppendTriggerInfo(CLbtContainerTriggerEntry* aEntry)
	{
	FUNC_ENTER("CLbtRamTriggersManager::AppendTriggerInfo");
	const CLbtTriggerConditionArea* conditionBase = 
			static_cast<const CLbtTriggerConditionArea*>(aEntry->TriggerEntry()->GetCondition());
	CLbtGeoAreaBase* areaBase = conditionBase->TriggerArea();
		    
    TLbtTriggerModifiedInfo info;
    info.iTriggerId = aEntry->TriggerEntry()->Id();
    info.iAreaType = areaBase->Type();
    info.iManagerUi = aEntry->TriggerEntry()->ManagerUi();
    
    TSecureId sid = aEntry->ExtendedTriggerInfo()->OwnerSid();
    TUid ownerUid;
    ownerUid.iUid = (TInt)(sid.iId);
    info.iOwner = ownerUid;
    
    CLbtTriggerEntry* triggerEntry = aEntry->TriggerEntry();	
	if( triggerEntry->Type() == CLbtTriggerEntry::ETypeStartup)
		{
		CLbtStartupTrigger* startupTrigger = static_cast<CLbtStartupTrigger*>(triggerEntry);
		TSecureId secureId;
		TBuf16<KMaxFileName> fileName;
		startupTrigger->GetProcessId(fileName, secureId);
		TUid startupUid;
		startupUid.iUid = (TInt)(sid.iId);
		info.iStartupProcess = startupUid;
		}
	else
		{
		info.iStartupProcess = KNullUid;
		}
    
    TInt error = iIdArray.Append(info);
    if( error != KErrNone )
        {
        LOG1("Failed to appenf info to the array:%d",error);
        }
	}


// ---------------------------------------------------------------------------
// CLbtRamTriggersManager::SelfComplete
// ---------------------------------------------------------------------------
void CLbtRamTriggersManager::SelfComplete()
    {
    FUNC_ENTER("CLbtRamTriggersManager::SelfComplete");
    iStatus = KRequestPending;
    TRequestStatus* status = &iStatus;    
    SetActive();
    User::RequestComplete(status, KErrNone);    
    }

// ---------------------------------------------------------------------------
// CLbtRamTriggersManager::RunL
// ---------------------------------------------------------------------------
void CLbtRamTriggersManager::RunL()
    {
    FUNC_ENTER("CLbtRamTriggersManager::RunL");
    switch( iCurrentOperation )
        {
        case EOperationCreate:
            {
            // See if the trigger is a enabled and valid trigger
            CLbtContainerTriggerEntry *entry = iTriggerIdTree->FindEntryInTreeL( iCurrentTriggerId );
            CLbtTriggerEntry* trigger = entry->TriggerEntry();
            if((trigger->State() == CLbtTriggerEntry::EStateEnabled) &&
               (entry->DynInfo()->iValidity == TLbtTriggerDynamicInfo::EValid))
                {
                iCountOfEnabledAndValidTrigger++;
                }
            CompleteClientRequest( KErrNone );
            break;
            }
        case EOperationUpdateTrigger:
            {
            HandleUpdateTriggerEventL();
            break;
            }
        case EOperationDelete:
            {
            HandleDeleteTriggersEventL();
            break;
            }
        case EOperationUpdatingState:
            {
            HandleUpdateTriggersStateEventL();
            break;
            }
    
        case EOperationListing:
            {
            HandleListTriggerEventL();
            break;
            }
            
        case EOperationGetTriggers:
            {
            HandleGetTriggersEventL();
            break;
            }
    
        default:
            {
            CompleteClientRequest( KErrGeneral );
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CLbtRamTriggersManager::CompleteClientRequest
// ---------------------------------------------------------------------------
void CLbtRamTriggersManager::CompleteClientRequest(TInt aError)
    {
    FUNC_ENTER("CLbtRamTriggersManager::CompleteClientRequest");
    if( iClientStatus )
        User::RequestComplete( iClientStatus, aError );
    iCurrentOperation = EOperationNone;
    iClientStatus = NULL;
    iCurrentTriggerId = 0;    
    delete iFilterBase;
    iFilterBase = NULL;    
    iIterator = 0;
    iClientTriggerEntry = NULL;
    }

// ---------------------------------------------------------------------------
// CLbtRamTriggersManager::RunError
// ---------------------------------------------------------------------------
TInt CLbtRamTriggersManager::RunError( TInt aError  )
    {
    FUNC_ENTER("CLbtRamTriggersManager::RunError");
    CompleteClientRequest( aError );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CLbtRamTriggersManager::DoCancel
// ---------------------------------------------------------------------------
void CLbtRamTriggersManager::DoCancel()
    {
    FUNC_ENTER("CLbtRamTriggersManager::DoCancel");
    // Nothing to do here. Only self complete events
    }


// ---------------------------------------------------------------------------
// CLbtRamTriggersManager::UpdateTriggerL
// ---------------------------------------------------------------------------
void CLbtRamTriggersManager::UpdateTriggerL( CLbtContainerTriggerEntry& aEntry,
    										 TLbtTriggerDataMask aDataMask,
    										 TLbtTriggerAttributeFieldsMask aAttrMask,
    										 TLbtSecurityPolicy& aSecurityPolicy,
    										 TRequestStatus& aStatus )
    {
    FUNC_ENTER("CLbtRamTriggersManager::UpdateTriggerL");
    iClientStatus = &aStatus;   
    iClientTriggerEntry = &aEntry;
    iClientSecurityPolicy = aSecurityPolicy;
    iDataMask = aDataMask;
    iAttrMask = aAttrMask;
    iIdArray.Reset();
    
    iCurrentOperation = EOperationUpdateTrigger;
    SelfComplete();
    }

// ---------------------------------------------------------- 
// CLbtRamTriggersManager::HandleUpdateTriggerEventL
// ----------------------------------------------------------
//
void CLbtRamTriggersManager::HandleUpdateTriggerEventL()
    {
    // entry retrieved from the tree
    CLbtContainerTriggerEntry* entry = NULL;
    entry = iTriggerIdTree->FindEntryInTreeL( iClientTriggerEntry->TriggerEntry()->Id() );
    
    if( entry==NULL )
        {
        CompleteClientRequest(KErrNotFound);
        return;
        }
    
    if( !LbtContainerUtilities::RunSecurityPolicy( entry, iClientSecurityPolicy ) )
        {
        CompleteClientRequest(KErrAccessDenied);
        return;
        }    
    
    // Trigger Entry supplied by the client
    CLbtTriggerEntry* trigEntry = iClientTriggerEntry->TriggerEntry();
    CLbtExtendedTriggerInfo* trigExtInfo = iClientTriggerEntry->ExtendedTriggerInfo();
    TLbtTriggerDynamicInfo* trigDynInfo = iClientTriggerEntry->DynInfo();
    
    // Append the modification information
    MLbtTriggerStore::TLbtTriggerModifiedInfo info;
    info.iTriggerId = entry->TriggerEntry()->Id();   
    TSecureId sid = entry->ExtendedTriggerInfo()->OwnerSid();    
    info.iOwner.iUid= (TInt)(sid.iId);
    iIdArray.AppendL(info);

    /* contTrigEntry is the trigger entry which is a part of 
     * the container trigger entry retrieved from the tree 
     */     
    CLbtSessionTrigger* contTrigEntry = NULL;
    if( entry->TriggerEntry() )
        {
        contTrigEntry = static_cast <CLbtSessionTrigger*>(entry->TriggerEntry());
        
        CLbtTriggerConditionArea* condArea = static_cast <CLbtTriggerConditionArea*>(contTrigEntry->GetCondition());
        CLbtGeoAreaBase* geoArea = condArea->TriggerArea();    
        info.iAreaType = geoArea->Type();
        }
    else
        {
        contTrigEntry=CLbtSessionTrigger::NewL();
        entry->SetTriggerEntry( contTrigEntry );
        }
    
    /* Contextinfo is the extended information present in the entry 
     * retrieved from the tree 
     */
    CLbtExtendedTriggerInfo* contExtInfo = entry->ExtendedTriggerInfo();
    TLbtTriggerDynamicInfo* contDynInfo =  entry->DynInfo();
    
    if(trigEntry!=NULL)
        {
        if( iAttrMask & CLbtTriggerEntry::EAttributeId )
            {
             contTrigEntry->SetId(trigEntry->Id());
            }
                
        if( iAttrMask & CLbtTriggerEntry::EAttributeName )
            {
            contTrigEntry->SetNameL(trigEntry->Name()); 
            }
                            
        if( iAttrMask & CLbtTriggerEntry::EAttributeState )
            {
            contTrigEntry->SetState(trigEntry->State());        
            }
        
        if( iAttrMask & CLbtTriggerEntry::EAttributeRearmTime )
            {
            contTrigEntry->SetTimeToRearm((trigEntry->TimeToRearm()));
            }
                            
        if( iAttrMask & CLbtTriggerEntry::EAttributeRequestor )
            {
            //clientEntry->SetRequestorL(trigEntry->GetRequestor());        
            }
                                
        if( iAttrMask & CLbtTriggerEntry::EAttributeManagerUi )
            {
            contTrigEntry->SetManagerUi(trigEntry->ManagerUi());        
            }
                    
        if( iAttrMask & CLbtTriggerEntry::EAttributeCondition )
            {
            CLbtTriggerConditionArea* condArea = static_cast <CLbtTriggerConditionArea*>(contTrigEntry->GetCondition());
            CLbtGeoAreaBase* geoArea = condArea->TriggerArea();
            CLbtTriggerConditionArea* cond=static_cast <CLbtTriggerConditionArea*> (trigEntry->GetCondition());
            
            if(geoArea->Type() == CLbtGeoAreaBase::ECircle)
                {
                // Update the condition for cicular trigger specified in the trigger entry parameter
                CLbtGeoCircle* containerCircle = static_cast<CLbtGeoCircle*>(geoArea);          
                CLbtGeoCircle* geoCircle= static_cast <CLbtGeoCircle*> (cond->TriggerArea());
                
                containerCircle->SetCenter(geoCircle->Center());
                containerCircle->SetRadius(geoCircle->Radius());
                }
            else if(geoArea->Type() == CLbtGeoAreaBase::ECellular)
                {
                CLbtGeoCell* containerCell = static_cast<CLbtGeoCell*>(geoArea);
                CLbtGeoCell* geoCell = static_cast <CLbtGeoCell*> (cond->TriggerArea());
                
                containerCell->SetNetworkType(geoCell->NetworkType());
                containerCell->SetNetworkCountryCode(geoCell->NetworkCountryCode());
                containerCell->SetNetworkIdentityCode(geoCell->NetworkIdentityCode());
                containerCell->SetLocationAreaCode(geoCell->LocationAreaCode());
                containerCell->SetCellId(geoCell->CellId());
                }
            else if(geoArea->Type() == CLbtGeoAreaBase::EHybrid)
                {
                CLbtGeoHybrid* containerHybrid = static_cast<CLbtGeoHybrid*>(geoArea);
                CLbtGeoHybrid* geoHybrid = static_cast <CLbtGeoHybrid*>( cond->TriggerArea() );
                RPointerArray<CLbtGeoAreaBase> newHybridArea;
                CleanupClosePushL( newHybridArea );
                
                const RPointerArray<CLbtGeoAreaBase>& hybridArray = geoHybrid->HybridArea();
                for(TInt i=0;i<hybridArray.Count();++i)
                    {
                    CLbtGeoAreaBase* geoAreaToArray = NULL;
                    switch( hybridArray[i]->Type() )
                        {
                        case CLbtGeoAreaBase::ECircle:                              
                            {
                            // Update the condition for cicular trigger specified in the trigger entry parameter
                            CLbtGeoCircle* containerCircle = CLbtGeoCircle::NewL();           
                            CLbtGeoCircle* geoCircle= static_cast<CLbtGeoCircle*> ( hybridArray[i] );
                            
                            containerCircle->SetCenter(geoCircle->Center());
                            containerCircle->SetRadius(geoCircle->Radius());
                            
                            /*
                            // First clear the present additional geo area info array
                            RPointerArray<CLbtGeoAreaBase>& array = containerCircle->GetAdditionalGeoAreaInfo();
                            array.ResetAndDestroy();
                            
                            // copy from provided geo circle
                            array = geoCircle->GetAdditionalGeoAreaInfo();
                            for(TInt i=0;i<array.Count();++i)
                                {
                                containerCircle->SetAdditionalGeoAreaInfo( array[i] );
                                }
                            geoAreaToArray = containerCircle;
                            */
                            break;
                            }
                        case CLbtGeoAreaBase::ECellular:
                            {
                            CLbtGeoCell* containerCell = CLbtGeoCell::NewL();
                            CLbtGeoCell* geoCell = static_cast<CLbtGeoCell*> ( hybridArray[i] );
                            
                            containerCell->SetNetworkType(geoCell->NetworkType());
                            containerCell->SetNetworkCountryCode(geoCell->NetworkCountryCode());
                            containerCell->SetNetworkIdentityCode(geoCell->NetworkIdentityCode());
                            containerCell->SetLocationAreaCode(geoCell->LocationAreaCode());
                            containerCell->SetCellId(geoCell->CellId());
                            geoAreaToArray = containerCell;
                            break;
                            }                           
                        }
                   CleanupStack::PushL( geoAreaToArray );
                   newHybridArea.AppendL( geoAreaToArray );
                   CleanupStack::Pop( geoAreaToArray );
                   }                
                // The new hybrid array is ready. Now set the array into the geo area Hybrid
                containerHybrid->SetHybridArea( newHybridArea );
                CleanupStack::Pop(1); //newHybridArea
                newHybridArea.Close();                 
                }  
            
            CLbtTriggerConditionArea* newCond = 
                static_cast <CLbtTriggerConditionArea*>(trigEntry->GetCondition());
            CLbtTriggerConditionArea::TDirection newTriggerDirection =
                newCond->Direction();
            
            CLbtTriggerConditionArea* OldCond = 
                static_cast <CLbtTriggerConditionArea*>(contTrigEntry->GetCondition());
            CLbtTriggerConditionArea::TDirection currentTriggerDirection =
                OldCond->Direction();
            
            if( newTriggerDirection != currentTriggerDirection )
                {
                OldCond->SetDirection( newTriggerDirection );
                }
            }
        }
                           
    if(trigExtInfo!=NULL)
        {
        if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeHysteresisRadius )
            {
            if(contExtInfo==NULL)
                {
                contExtInfo=CLbtExtendedTriggerInfo::NewL();
                entry->SetExtendedTriggerInfo( contExtInfo );
                }
            contExtInfo->SetHysteresisRadius(trigExtInfo->HysteresisRadius());
            }
                        
        if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeIsFired )
            {
            if(contExtInfo==NULL)
                {
                contExtInfo=CLbtExtendedTriggerInfo::NewL();
                entry->SetExtendedTriggerInfo( contExtInfo );
                }   
            contExtInfo->SetTriggerFiredState(trigExtInfo->IsTriggerFired());
            }
            
        if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeFireInfo )
            {
            if(contExtInfo==NULL)
                {
                contExtInfo=CLbtExtendedTriggerInfo::NewL();
                entry->SetExtendedTriggerInfo( contExtInfo );
                }   
            contExtInfo->SetFiredInfo( trigExtInfo->GetFiredInfo() );
            }
            
        if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeOwnerSid )
            {
            if(contExtInfo==NULL)
                {
                contExtInfo=CLbtExtendedTriggerInfo::NewL();
                entry->SetExtendedTriggerInfo( contExtInfo );
                }
            contExtInfo->SetOwnerSid(trigExtInfo->OwnerSid()); 
            }
                    
        if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeRectTriggerArea )
            {
            if(contExtInfo==NULL)
                {
                contExtInfo=CLbtExtendedTriggerInfo::NewL();
                entry->SetExtendedTriggerInfo( contExtInfo );
                }
            contExtInfo->SetTriggerRectangleArea(trigExtInfo->TriggerReactangleArea()); 
            }   
                        
        if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeStrategyData )
            {
            if(contExtInfo==NULL)
                {
                contExtInfo=CLbtExtendedTriggerInfo::NewL();
                entry->SetExtendedTriggerInfo( contExtInfo );
                }
            contExtInfo->SetStrategyDataL(trigExtInfo->StategyData()); 
            }

        if( iDataMask & CLbtContainerTriggerEntry::EContainerAttributeIsTriggerFireOnCreation )
            {
            if(contExtInfo==NULL)
                {
                contExtInfo=CLbtExtendedTriggerInfo::NewL();
                entry->SetExtendedTriggerInfo( contExtInfo );
                }
            contExtInfo->SetTriggerFireOnCreation(trigExtInfo->IsTriggerFireOnCreation()); 
            }   
        }
        
    if(trigDynInfo!=NULL)
        {
        if( iDataMask & CLbtContainerTriggerEntry::EContainerDynInfoAttributeValidity )
            {
            if(contDynInfo == NULL)
                {
                contDynInfo = new TLbtTriggerDynamicInfo;
                entry->SetDynInfo( contDynInfo );
                }
            contDynInfo->iValidity = trigDynInfo->iValidity;    
            }
        }
            
    CompleteClientRequest( KErrNone );
    }


// ---------------------------------------------------------- 
// CLbtRamTriggersManager::UpdateTriggersState
// ----------------------------------------------------------
//
void CLbtRamTriggersManager::UpdateTriggersStateL( CLbtTriggerEntry::TLbtTriggerState aState,
                    							   CLbtContainerUpdateFilter* aFilter,
                    							   TLbtFireOnUpdate aFireOnUpdate,
                    							   TLbtSecurityPolicy& aSecurityPolicy,
                    							   TRequestStatus& aStatus )
    {
    FUNC_ENTER("CLbtRamTriggersManager::UpdateTriggersState");
    iClientStatus = &aStatus;
    
    if(aFilter==NULL)
        {
        CompleteClientRequest( KErrArgument );
        return;
        }
    
    /* Set the iterator to the beginning of the tree */
    //Set the iterator to the beginning of the tree
    iTriggerIdTree->SetIteratorL();
    iCurrentOperation = EOperationUpdatingState;
    iIterator = 0;  
    iState = aState;
    iFireOnUpdate = aFireOnUpdate;  
    
    /* Retrieve the first entry of the tree */
    CLbtContainerTriggerEntry* tEntry = NULL;
    tEntry=iTriggerIdTree->GetNextEntryL();
    
    if(tEntry == NULL)
        {
        CompleteClientRequest( KErrNotFound );    
        return;
        }
    
    CLbtContainerFilterBase* filter = LbtContainerUtilities::GetContainerFilterFromUpdateFilterLC(aFilter);    
    iIdArray.Reset();
    
    while(tEntry)
    	{
    	if( LbtContainerUtilities::RunSecurityPolicy( tEntry, aSecurityPolicy ) )
    		{
    		TInt isFilterPresent = 0;
			TBool isEntryRequested = EFalse;		
		
			filter->ProcessFilter(tEntry,isFilterPresent,isEntryRequested);
			if(isFilterPresent>0 && isEntryRequested)
				{
				AppendTriggerInfo(tEntry);
				}
    		}		
		tEntry=iTriggerIdTree->GetNextEntryL();		
    	}

    CleanupStack::PopAndDestroy(filter);
    if(iIdArray.Count() == 0)
        {
        CompleteClientRequest( KErrNotFound );                
        }
    else
        {
        SelfComplete();
        }       
    }

// ---------------------------------------------------------- 
// CLbtRamTriggersManager::HandleUpdateTriggersStateEventL
// ----------------------------------------------------------
//
void CLbtRamTriggersManager::HandleUpdateTriggersStateEventL()
    {
    TInt count = 0;
    while( (iIterator < iIdArray.Count()) && 
           (count < KStepCount) )
       {
       CLbtContainerTriggerEntry *trigEntry = iTriggerIdTree->FindEntryInTreeL( iIdArray[iIterator].iTriggerId );
       CLbtTriggerEntry* trigger = trigEntry->TriggerEntry();
       
       if( trigger->State() == CLbtTriggerEntry::EStateDisabled && 
           iState == CLbtTriggerEntry::EStateEnabled && 
           trigEntry->DynInfo()->iValidity == TLbtTriggerDynamicInfo::EValid )
           {
           iCountOfEnabledAndValidTrigger++;
           }
       else if( trigger->State() == CLbtTriggerEntry::EStateEnabled && 
               iState == CLbtTriggerEntry::EStateDisabled && 
               trigEntry->DynInfo()->iValidity == TLbtTriggerDynamicInfo::EValid)
           {
           iCountOfEnabledAndValidTrigger--;
           }
       trigger->SetState(iState);   
       if( iFireOnUpdate == ELbtTrue )
           {
           CLbtExtendedTriggerInfo* extInfo = trigEntry->ExtendedTriggerInfo();
           extInfo->SetTriggerFireOnCreation( ETrue );
           }
       else 
           {
           CLbtExtendedTriggerInfo* extInfo = trigEntry->ExtendedTriggerInfo();
           extInfo->SetTriggerFireOnCreation( EFalse );
           }                  
       ++iIterator;
       ++count;
       }
    
    if( iIterator >= iIdArray.Count() )
        {
        // All triggers iteration done. Complete request                
        CompleteClientRequest( KErrNone );
        }
    else
        {
        SelfComplete();                
        }            
    }

//-----------------------------------------------------                    
// CLbtRamTriggersManager::UpdateTriggersValidity
//-----------------------------------------------------
//
void CLbtRamTriggersManager::UpdateTriggersValidityL( TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity,
                    							 	  RArray<TLbtTriggerId>& aTriggerIds,
                    							 	  TLbtSecurityPolicy& aSecurityPolicy,
                    							 	  TRequestStatus& aStatus) 
    {
    FUNC_ENTER("CLbtRamTriggersManager::UpdateTriggersValidity");
    aStatus = KRequestPending;
    TRequestStatus* status = &aStatus;
    iIdArray.Reset();
    
    if( iTriggerIdTree->IsEmpty() ||
        !aTriggerIds.Count() )
    	{
    	User::RequestComplete(status, KErrNotFound);
        return;
    	}
    
    for(TInt i=0;i<aTriggerIds.Count();i++)
        {
        CLbtContainerTriggerEntry* entry = NULL;
        entry = iTriggerIdTree->FindEntryInTreeL(aTriggerIds[i]);
        if(entry == NULL)
        	{
        	// This trigger is not found. Continue to see if we can find other triggers
        	continue;
        	}
        
        if( !LbtContainerUtilities::RunSecurityPolicy( entry, aSecurityPolicy ) )
        	{
        	// Security check did not pass. Goto next trigger
        	continue;
        	}
            	
        if(entry != NULL)
            {
            if( entry->DynInfo()->iValidity == TLbtTriggerDynamicInfo::EInvalid &&
	    	    aValidity == TLbtTriggerDynamicInfo::EValid)
	    		{
	    		iCountOfEnabledAndValidTrigger++;
	    		}
	    	else if(entry->DynInfo()->iValidity == TLbtTriggerDynamicInfo::EValid &&
	    	    aValidity == TLbtTriggerDynamicInfo::EInvalid)
	    	    {
	    	    iCountOfEnabledAndValidTrigger--;
	    	    }

            if(entry->DynInfo() != NULL)
                {
                entry->DynInfo()->iValidity = aValidity;
                }
            else
                {
                TLbtTriggerDynamicInfo * info = new TLbtTriggerDynamicInfo;
                info->iValidity = aValidity;
                entry->SetDynInfo(info);                
                }    
            AppendTriggerInfo(entry);
            }
        }
    User::RequestComplete(status,KErrNone);    
    }

// --------------------------------------------------------------------------------------------
// CLbtRamTriggersManager::UpdateTriggerFiredStateL
// --------------------------------------------------------------------------------------------
//    
void CLbtRamTriggersManager::UpdateTriggerFiredStateL( RArray<TLbtTriggerId>& aTriggerIds,
													   TBool aFireBool,
								                       TRequestStatus& aStatus )
	{
	FUNC_ENTER("CLbtRamTriggersManager::UpdateTriggerFiredStateL");
	TBool found = EFalse;
    aStatus = KRequestPending;
    TRequestStatus* status = &aStatus;
    
    if( aTriggerIds.Count() == 0 ||
    	iTriggerIdTree->IsEmpty() )
    	{
    	User::RequestComplete(status, KErrNotFound);
    	return;
    	}
    
    iIdArray.Reset();
    
    for(TInt i=0;i<aTriggerIds.Count();i++)
        {
        CLbtContainerTriggerEntry* entry = NULL;
        entry = iTriggerIdTree->FindEntryInTreeL(aTriggerIds[i]);
        if(entry == NULL)
        	{
        	// This trigger is not found. Continue to see if we can find other triggers
        	continue;
        	}
            
        CLbtExtendedTriggerInfo* extInfo = entry->ExtendedTriggerInfo();
        if(extInfo != NULL)
        	{
        	extInfo->SetTriggerFiredState(aFireBool);
        	found = ETrue;
        	}
        }
            
    TInt error = KErrNone;
    if( !found )
    	{
    	error = KErrNotFound;
    	}
    User::RequestComplete(status, error);
	}

//------------------------------------------------------
// CLbtRamTriggersManager::DeleteTriggers
//------------------------------------------------------
//
void CLbtRamTriggersManager::DeleteTriggersL( CLbtContainerUpdateFilter* aFilter,
											  TLbtSecurityPolicy& aSecurityPolicy,
											  TRequestStatus& aStatus )
    {
    FUNC_ENTER("CLbtRamTriggersManager::DeleteTriggers");
    iIdArray.Reset();
    iClientStatus = &aStatus;
 
    if(aFilter==NULL)
        {
        User::RequestComplete(iClientStatus,KErrArgument);        
        return;
        }
    
    if( iTriggerIdTree->IsEmpty() )
    	{
    	User::RequestComplete(iClientStatus, KErrNotFound);
        return;
    	}

    /* Set the iterator to the beginning of the tree */
    iTriggerIdTree->SetIteratorL();
    iCurrentOperation = EOperationDelete;
    iIterator = 0;  
    
    iFilterBase = LbtContainerUtilities::GetContainerFilterFromUpdateFilterLC(aFilter);
    CleanupStack::Pop(1); // iFilterBase
    
    /* Retrieve the first entry of the tree */    
    CLbtContainerTriggerEntry* tEntry=iTriggerIdTree->GetNextEntryL();
        
    while( tEntry )
        {       
        if( LbtContainerUtilities::RunSecurityPolicy( tEntry, aSecurityPolicy ) )
            {
            TInt isFilterPresent = 0;
            TBool isEntryRequested = ETrue;
            iFilterBase->ProcessFilter(tEntry,isFilterPresent,isEntryRequested);
            if(isFilterPresent > 0 && isEntryRequested)
                {
                AppendTriggerInfo(tEntry);              
                }
            }
        tEntry = iTriggerIdTree->GetNextEntryL();
        }
    
    if(iIdArray.Count() == 0)
        {
        CompleteClientRequest( KErrNotFound );                
        }
    else
        {
        SelfComplete();
        }          
    }   

// ---------------------------------------------------------------------------
// CLbtRamTriggersManager::HandleDeleteTrigger()
// ---------------------------------------------------------------------------
//
void CLbtRamTriggersManager::HandleDeleteTriggersEventL()
    {
    TInt count = 0;
    while( (iIterator < iIdArray.Count()) && 
           (count < KStepCount) )
       {
       CLbtContainerTriggerEntry *entry = iTriggerIdTree->FindEntryInTreeL( iIdArray[iIterator].iTriggerId );
       CLbtTriggerEntry* trigger = entry->TriggerEntry();
       if( trigger->State() == CLbtTriggerEntry::EStateEnabled && 
           entry->DynInfo()->iValidity == TLbtTriggerDynamicInfo::EValid )
           {
           iCountOfEnabledAndValidTrigger--;
           }
       DeleteTriggerL( iIdArray[iIterator].iTriggerId );
       ++iIterator;
       ++count;
       }
    
    if( iIterator >= iIdArray.Count() )
        {
        // All triggers iteration done. Complete request                
        CompleteClientRequest( KErrNone );
        }
    else
        {
        SelfComplete();                
        }
    }

// ---------------------------------------------------------------------------
// CLbtRamTriggersManager::DeleteTriggerL
// ---------------------------------------------------------------------------
//
void CLbtRamTriggersManager::DeleteTriggerL(TLbtTriggerId aTriggerId)
    {
    FUNC_ENTER("CLbtRamTriggersManager::DeleteTriggerL");

	CLbtContainerTriggerEntry* triggerEntry = iTriggerIdTree->FindEntryInTreeL(aTriggerId);
	if(triggerEntry == NULL)
		{
		User::Leave(KErrNotFound);
		}

	CLbtGeoAreaBase::TGeoAreaType areaType = CLbtGeoAreaBase::ECircle;
	if(triggerEntry->TriggerEntry()->GetCondition()->Type() == CLbtTriggerConditionBase::ETriggerConditionArea)
		{
		// Get the condition area base
		CLbtTriggerConditionArea* conditionArea = 
				static_cast<CLbtTriggerConditionArea*>(triggerEntry->TriggerEntry()->GetCondition());
		areaType = conditionArea->TriggerArea()->Type();
		}
	
	if(iIdArray.Count() == 0)
    	{
    	MLbtTriggerStore::TLbtTriggerModifiedInfo info;
    	info.iTriggerId = aTriggerId;
    	info.iAreaType = areaType;
    	iIdArray.AppendL(info);
    	}
    iTriggerIdTree->DeleteFromTreeL(aTriggerId);
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CLbtRamTriggersManager::CLbtRamTriggersManager():CActive( EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    iCountOfEnabledAndValidTrigger=0;
    iIdArray.Reset();
    }


// ---------------------------------------------------------------------------
// CLbtRamTriggersManager::GetCountOfEnabledAndValidTriggers
// ---------------------------------------------------------------------------
//
TInt CLbtRamTriggersManager::GetCountOfEnabledAndValidTriggers()
	{
	return iCountOfEnabledAndValidTrigger;
	}

// ---------------------------------------------------------------------------
// The 2nd phase Symbian Constructor
// ---------------------------------------------------------------------------
//
void CLbtRamTriggersManager::ConstructL()
    {
	iTriggerIdTree=CLbtRamTriggerIdTree::NewL();	
    }

//---------------------------------------------------------------------------
// CLbtDbTriggersManager::SetTimeTillCompaction
//---------------------------------------------------------------------------
//
void CLbtRamTriggersManager::SetTimeTillCompaction(TTime /*aTime*/)
	{
	// Do nothing here. No compaction required
	}

//---------------------------------------------------------------------------
// CLbtRamTriggersManager::IsStoreBusy
//---------------------------------------------------------------------------
//
TBool CLbtRamTriggersManager::IsStoreBusy()
	{
	return EFalse;
	}

//---------------------------------------------------------------------------
// CLbtRamTriggersManager::SetStoreChangeObserver
//---------------------------------------------------------------------------
//
void CLbtRamTriggersManager::SetStoreChangeObserver(MLbtTriggerStoreObserver* /*aObserver*/)
	{
	// Do nothing
	}

//---------------------------------------------------------------------------
// CLbtRamTriggersManager::CancelCurrentOperation
//---------------------------------------------------------------------------
//
TInt CLbtRamTriggersManager::CancelCurrentOperation()
    {
    FUNC_ENTER("CLbtRamTriggersManager::CancelCurrentOperation");
    TInt error = KErrCancel;
    switch( iCurrentOperation )
        {
        case EOperationCreate:
            {
            TRAP_IGNORE( iTriggerIdTree->DeleteFromTreeL( iCurrentTriggerId ));
            break;
            }
        case EOperationDelete:
            {
            // This information is required to update client about trigger change event.
            if( iIterator > 0 &&
                iIterator < iIdArray.Count() )
                {
                error = KLbtErrPartial;
                }
            break;
            }
        case EOperationUpdatingState:
            {
            // This information is required to update client about trigger change event.
            if( iIterator > 0 &&
                iIterator < iIdArray.Count() )
                {
                error = KLbtErrPartial;
                }
            }
        case EOperationListing:
        default:
            {
            break;
            }
        }
    Cancel();
    CompleteClientRequest( KErrCancel );
    return error;
    }
