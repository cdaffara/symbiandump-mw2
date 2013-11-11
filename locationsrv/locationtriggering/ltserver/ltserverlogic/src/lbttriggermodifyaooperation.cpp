/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active Object operation for serving server logic requests
*				 for modification or deletion of triggers.
*
*/



// INCLUDE FILES
#include <lbtstartuptrigger.h>
#include <lbtsessiontrigger.h>
#include <lbtstartuptrigger.h>
#include <lbttriggerfilterbase.h>
#include <lbttriggerfilterbyattribute.h>
#include <lbttriggerfilterbyarea.h>
#include <lbttriggerfiltercomposite.h>
#include <lbttriggerconditionbase.h>
#include <lbttriggerconditionarea.h>
#include <lbtgeocircle.h>
#include <s32mem.h>
#include <e32cmn.h>
#include "lbttriggermodifyaooperation.h"
#include "lbtaoobserver.h"
#include "lbtcontainer.h"
#include "lbtglobal.h"
#include "lbtserverconsts.h"
#include "lbtcontainerextendedtriggerinfo.h"
#include "lbtcontainerupdatefilter.h"
#include "lbtcontainertriggerfilter.h"
#include "lbtserverlogicbase.h"
#include "lbtsettingsmanager.h"
#include "lbtlogger.h"

// ===================== MEMBER FUNCTIONS =====================

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
CLbtTriggerModifyAOOperation* CLbtTriggerModifyAOOperation::NewL( MLbtAOObserver& aObserver,
															      const RMessage2& aMessage,
															      CLbtContainer& aContainer,
															      TInt aClientType,
				    							                  CLbtSettingsManager& aSettingsManager )
	{
	FUNC_ENTER("CLbtTriggerModifyAOOperation::NewL");
	CLbtTriggerModifyAOOperation* self = 
					new (ELeave) CLbtTriggerModifyAOOperation(aObserver, aMessage, aContainer, aClientType,aSettingsManager);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::~CLbtTriggerModifyAOOperation
// Destructor
// ---------------------------------------------------------------------------
//
CLbtTriggerModifyAOOperation::~CLbtTriggerModifyAOOperation()
	{
	FUNC_ENTER("CLbtTriggerModifyAOOperation::~CLbtTriggerModifyAOOperation");
	// If the operation is not complete yet then complete the 
	// message with server busy
	iContainer.RemoveObserver(this);
	if(IsActive())
		{
		Cancel();
		}
	delete iContainerEntry;
	iUpdatedTriggerList.ResetAndDestroy();
	}

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::CLbtTriggerModifyAOOperation
// C++ Default constructor
// ---------------------------------------------------------------------------
//
CLbtTriggerModifyAOOperation::CLbtTriggerModifyAOOperation( MLbtAOObserver& aObserver,
										 				    const RMessage2& aMessage,
										 				    CLbtContainer& aContainer,
										 				    TInt aClientType,
										 				    CLbtSettingsManager& aSettingsManager )
	: CLbtAOOperationBase(aObserver, aMessage, aContainer, aClientType),
	  iState(ENone),iSettingsManager( aSettingsManager )
	{
	FUNC_ENTER("CLbtTriggerModifyAOOperation::CLbtTriggerModifyAOOperation");
	}

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtTriggerModifyAOOperation::ConstructL()
	{
	FUNC_ENTER("CLbtTriggerModifyAOOperation::ConstructL");
	CActiveScheduler::Add(this);
	iTriggerId = KLbtNullTriggerId;
	}

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::RunL
// ---------------------------------------------------------------------------
//
void CLbtTriggerModifyAOOperation::RunL()
	{
	FUNC_ENTER("CLbtTriggerModifyAOOperation::RunL");
	switch ( iState )
	    {
	    case EListTriggers:
	        {
	        if ( iStatus.Int() == KErrNone &&
	             iUpdatedTriggerList.Count() )
	            {
	            UpdateTriggersL();
	            }
	        else
	            {
	            LbtGlobal::RequestComplete(iMessage, iStatus.Int());
	            iObserver.HandleOperationClosureL(this, iStatus.Int());
	            }
	        break;
	        }
	    case EUpdateTriggers: 
	        {
	        iState = ENone;
	        // Complete the request with the return code of container
        	LbtGlobal::RequestComplete(iMessage, iStatus.Int());
        	iObserver.HandleOperationClosureL(this, iStatus.Int());
        	break;
	        }
	    default:
	        break;
	    }
	}

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::DoCancel
// ---------------------------------------------------------------------------
//
void CLbtTriggerModifyAOOperation::DoCancel()
	{
	FUNC_ENTER("CLbtTriggerModifyAOOperation::DoCancel");
	// Cancel container notifications
	iContainer.CancelAsyncOperation(iAOIdentificationNum);
    if( iStatus.Int() == KLbtErrPartial )
      {
      LbtGlobal::RequestComplete(iMessage,KErrCancel);
      }
    else
      {
      LbtGlobal::RequestComplete(iMessage, iStatus.Int());
      }
	}

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::StartOperationL
// ---------------------------------------------------------------------------
//
void CLbtTriggerModifyAOOperation::StartOperationL()
	{
	FUNC_ENTER("CLbtTriggerModifyAOOperation::StartOperationL");
	/* The Update Operation involves the following steps,
	 * 1. First List Triggers has to be executed. This is necessary for 
	 *    obtaining the owner SID of the trigger / triggers on whom the operation
	 *    is to be performed. This is also needed to process the notification
	 *    requests from clients and management ui.
	 *
	 * 2. The actual modification operation is then performed.
	 *
	 * 3. The pending Notification requests are performed.
	 *
	 * Following is a table of possible combination of actions
	 * 
	 *  Client Type       Operation Requested             Operations Performed
	 *
	 *  Client Library           update                           list, modify
	 *  Client Library           delete                              modify
	 *
	 * Management Library    Update state of 1 trigger            list, modify
	 * Management Library    Update other param of own trigger    list, modify
	 * Management Library    Update other param of other trigger  none
	 * Management Library    Update state of many triggers        list, modify
	 * Management Library    Delete 1 trigger                     list, delete
	 * Management Library    Delete many triggers                 list, delete
	 */
    if ( CLbtServerLogicBase::TLbtClientLibrary == iClientType &&
         ELbtUpdateTrigger != iFunction )
        {
        // No need for list triggers in this case.
        TRAPD(error , UpdateTriggersL());
        
        if( error != KErrNone )
        	{
        	iObserver.HandleOperationClosureL(this, error);
        	}
        }
    else
        {
        ListTriggersL();
        }
	}

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::CancelModifyOperation
// ---------------------------------------------------------------------------
//
void CLbtTriggerModifyAOOperation::CancelModifyOperation()
	{
	FUNC_ENTER("CLbtTriggerModifyAOOperation::CancelModifyOperation");
	Cancel();
	
	// HandleOperationClosureL might leave either when it tries to unload strategy
	// or during notification. Nothing can be done when this happens. Hence it will 
	// be ignored.
    if( iStatus.Int() == KLbtErrPartial )
       {
       LOG("Partial completion");  
       TRAP_IGNORE( iObserver.HandleOperationClosureL( this,KErrNone ) );
       }
    else
       {
       TRAP_IGNORE( iObserver.HandleOperationClosureL( this,iStatus.Int() ) );
       }
    }
	
// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::GetTriggerId
// ---------------------------------------------------------------------------
//
TLbtTriggerId CLbtTriggerModifyAOOperation::GetTriggerId()
	{
	FUNC_ENTER("CLbtTriggerModifyAOOperation::GetTriggerId");
	return iTriggerId;
	}

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::ListTriggersL
// ---------------------------------------------------------------------------
//
void CLbtTriggerModifyAOOperation::ListTriggersL()
    {
    FUNC_ENTER("CLbtTriggerModifyAOOperation::ListTriggersL");
    // The List Triggers operation is called for 2 reasons
    //
    // 1. The Container UpdateTrigger method does not take a filter. Hence the
    //    check to ensure that only the owner can modify its triggers requires
    //    a call to List Triggers. During Listing we add the owner SID also to
    //    the filter. If the List Trigger call returns any triggers then the 
    //    update operation can be executed. 
    //
    // 2. The Notification requires informaiton all triggers that have been 
    //    modified and the owners of the triggers as well. This information
    //    is not available form the IPC. The IPC contains only the application
    //    that requests the modification. This could be through the LT 
    //    management API, in which case that will not indicate the owner of the
    //    trigger.

	CLbtContainerListOptions* containerOptions = CLbtContainerListOptions::NewL();
	CleanupStack::PushL(containerOptions);

	CLbtListTriggerOptions* options = CLbtListTriggerOptions::NewL();
	CleanupStack::PushL(options);
    TLbtSecurityPolicy securityPolicy;
    switch( iFunction )
        {
		case ELbtUpdateTrigger:
		    {
		    CLbtTriggerEntry* trigger = 
		        InternalizeTriggerFromIpcLC( iMessage, KParamTriggerEntry );

            // Add a filter for the trigger Id.
			CLbtTriggerFilterByAttribute* filter = 
			                CLbtTriggerFilterByAttribute::NewLC();
			filter->AddTriggerIdL( trigger->Id() );
            options->SetFilter( filter );
			CleanupStack::Pop(filter);
            CleanupStack::PopAndDestroy( trigger );

            // Only the update trigger requires this check. All other client API
            // requests are directly executed since the owner SID check can be done
            // through filters.
            // For all other requests from management API, the check is not 
            // required since the management API has the permission to execute
            // the request on any trigger in the system.

            if ( CLbtServerLogicBase::TLbtClientLibrary == iClientType ||
                 (CLbtServerLogicBase::TLbtManagementLibrary == iClientType &&
                 CLbtTriggerEntry::EAttributeState != iMessage.Int0()) )
                {
    			securityPolicy.SetOwnerSecureId( iMessage.SecureId() );
                }
		    break;
		    }
		case ELbtDeleteTrigger:
		case ELbtSetTriggerState:
		    {
			TPckg<TLbtTriggerId> id(iTriggerId);
			LbtGlobal::Read(iMessage, KParamTriggerId, id);
			// Create a filter for the trigger Id
			CLbtTriggerFilterByAttribute* filter = 
			                CLbtTriggerFilterByAttribute::NewLC();
			filter->AddTriggerIdL( iTriggerId );
            options->SetFilter( filter );
			CleanupStack::Pop(filter);
			break;
		    }
		case ELbtDeleteTriggers:
		case ELbtSetTriggersState:
		    {
			CLbtTriggerFilterBase* filter = 
			    InternalizeFilterFromIpcLC( iMessage, KParamFilter );
            options->SetFilter( filter );
			CleanupStack::Pop(filter);
			break;
		    }
        }

    containerOptions->SetListOptions( options );
	CleanupStack::Pop( options );
	
	containerOptions->SetDataMask(CLbtTriggerEntry::EAttributeId | 
	    CLbtContainerTriggerEntry::EContainerAttributeOwnerSid);
	CleanupStack::Pop(containerOptions);    
						
	// List the triggers that require modification.
	iContainer.ListTriggers(containerOptions, iUpdatedTriggerList,iAOIdentificationNum,iStatus,securityPolicy);
    iState = EListTriggers;
	SetActive();
    }

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::CheckEnabledTriggerCap()
// ---------------------------------------------------------------------------
//
TBool CLbtTriggerModifyAOOperation::CheckEnabledTriggerCap()
	{
	TUint enabledValidTriggerCount = iContainer.GetCountOfEnabledAndValidTriggers();
	
	if( iSettingsManager.EnabledValidTriggerCount() > enabledValidTriggerCount )
		{
		// More triggers can be created. Return true
		return ETrue;
		}
	return EFalse;
	}

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::UpdateTriggersL()
// ---------------------------------------------------------------------------
//
void CLbtTriggerModifyAOOperation::UpdateTriggersL()
    {
    FUNC_ENTER("CLbtTriggerModifyAOOperation::UpdateTriggersL");
	switch(iFunction)
		{
		case ELbtUpdateTrigger:
			{
			HandleUpdateTriggerStateOpL();
			break;
			}
		
		case ELbtSetTriggerState:
		case ELbtSetTriggersState:
			{
			HandleModifyTriggerStateOpL();
			break;
			}
		case ELbtDeleteTrigger:
		case ELbtDeleteTriggers:
			{
			HandleDeleteTriggerOpL();
			break;
			}
		default:
			{
			User::Leave( KErrNotSupported );
			break;
			}
		}
    iState = EUpdateTriggers;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::HandleUpdateTriggerStateOpL()
// ---------------------------------------------------------------------------
//		
void CLbtTriggerModifyAOOperation::HandleUpdateTriggerStateOpL()
    {
    // Create the trigger entry object based on the type of trigger
    TLbtTriggerDataMask dataMask = 0;
	CLbtTriggerEntry* trigger = 
	    InternalizeTriggerFromIpcLC( iMessage, KParamTriggerEntry );
	if( iMessage.Int0() & CLbtTriggerEntry::EAttributeState )
		{
		if( trigger->State() == CLbtTriggerEntry::EStateEnabled)
			{
			if( !CheckEnabledTriggerCap() )
				{						
				CleanupStack::PopAndDestroy( trigger );
				LbtGlobal::RequestComplete(iMessage, KLbtErrMaxEnabledValidTriggersLimitExceeded);
				iObserver.HandleOperationClosureL(this, KLbtErrMaxEnabledValidTriggersLimitExceeded);
				return;
				}
			}
		}
	if( iMessage.Int0() & CLbtTriggerEntry::EAttributeCondition)
	    {
	    CLbtStartupTrigger* triggerEntry = static_cast<CLbtStartupTrigger*>
	                                                    ( trigger );
	      
	    
	    // Check whether the trigger radius is less than minimum 
	    // trigger radius.
	    if(triggerEntry->GetCondition()->Type() == CLbtTriggerConditionBase::ETriggerConditionArea)
    		{
    		// Get the condition area base
    		CLbtTriggerConditionArea* conditionArea = 
    				static_cast<CLbtTriggerConditionArea*>(triggerEntry->GetCondition());
    		
    		// Check if the area is a circle
    		if(conditionArea->TriggerArea()->Type() == CLbtGeoAreaBase::ECircle)
    			{
    			CLbtGeoCircle* circle = static_cast<CLbtGeoCircle*>(conditionArea->TriggerArea());
    			
    			// If the triggering area specified is less than the minimum triggering area
    			// the return KErrTriggerAreaTooSmall
    			if(circle->Radius() < iSettingsManager.MinimumTriggeringArea())
    				{
    				CleanupStack::PopAndDestroy( trigger );
    				LbtGlobal::RequestComplete(iMessage, KErrTriggerAreaTooSmall);
    				iObserver.HandleOperationClosureL(this, KErrTriggerAreaTooSmall);
    				return;
    				}
                }
    		}
	    }
	    
	LOG1("Trigger updated:%d",trigger->Id());
	LOG1("State of updated trigger:%d",trigger->State());    
	
	
	CLbtExtendedTriggerInfo* extendedTriggerInfo = 
	                                            CLbtExtendedTriggerInfo::NewL();
	CleanupStack::PushL( extendedTriggerInfo );
	            
    if( iFireOnUpdate == ELbtTrue )
        {
        extendedTriggerInfo->SetTriggerFireOnCreation( ETrue );
        }
    else
        {
        extendedTriggerInfo->SetTriggerFireOnCreation( EFalse );
        }
    dataMask|= CLbtContainerTriggerEntry::EContainerAttributeIsTriggerFireOnCreation;
	
	// Create the container trigger entry for the update operation
	iContainerEntry = CLbtContainerTriggerEntry::NewL(
	                                        trigger, NULL, extendedTriggerInfo );
	
	
	
	// Pop trigger since ownership is transferred to iContainerEntry
    CleanupStack::Pop( 2 );// extendedTriggerInfo,trigger
    
    // Register for container change event observer
    TLbtTriggerEventMask eventMask = 0;
    eventMask|= CLbtContainer::ELbtConTriggerNameFieldChanged|
                CLbtContainer::ELbtConTriggerStateFieldChanged|
                CLbtContainer::ELbtConTriggerRequestorFieldChanged|
                CLbtContainer::ELbtConTriggerManagerUiFieldChanged|        
                CLbtContainer::ELbtConTriggerConditionFieldChanged|
                CLbtContainer::ELbtConTriggerStartupFieldChanged;
                
    iContainer.SetChangeObserver( this ,eventMask );
    
    TLbtSecurityPolicy securityPolicy;
	// Change the state of the trigger owned by only the requesting 
	// client in case this is a req through LT client API.
	if ( iClientType == CLbtServerLogicBase::TLbtClientLibrary )
	    {
        securityPolicy.SetOwnerSecureId( iMessage.SecureId() );
	    }
    
	// The attribute field mask for updated fields is 
	// stores in IPC slot 0 ( 3 param).
	iContainer.UpdateTriggerL(*iContainerEntry, 
	                          dataMask,
	                          iMessage.Int0(),iAOIdentificationNum,
	                          iStatus,securityPolicy);
	SetActive();
    }

    
// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::HandleModifyTriggerStateOpL()
// ---------------------------------------------------------------------------
//
void CLbtTriggerModifyAOOperation::HandleModifyTriggerStateOpL()
    {
    FUNC_ENTER("CLbtTriggerModifyAOOperation::HandleModifyTriggerStateOpL");
	CLbtTriggerFilterBase* filter = NULL;
	// coverity[var_decl : FALSE] 
	CLbtTriggerEntry::TLbtTriggerState triggerState;
	
	switch ( iFunction )
	    {
	    case ELbtSetTriggerState:
    	    {
    		// Read trigger id from IPC msg and create a filter
    		TPckg<TLbtTriggerId> id( iTriggerId );
    		LbtGlobal::Read( iMessage, KParamTriggerId, id );
    		
    		TPckg<TLbtFireOnUpdate> fireUpdatePckg( iFireOnUpdate );
    		LbtGlobal::Read( iMessage, KParamFireOnUpdate, fireUpdatePckg );
    		filter = CLbtTriggerFilterByAttribute::NewLC();
    		static_cast<CLbtTriggerFilterByAttribute *>(filter)->AddTriggerIdL( iTriggerId );
    		
    		TPckg<CLbtTriggerEntry::TLbtTriggerState> state(triggerState);
    		LbtGlobal::Read(iMessage, KTriggerState, state);
    		break;
    	    }
	    case ELbtSetTriggersState:
	        {
	        TLbtTriggerStateInfo triggerStateInfo;
	        TPckg<TLbtTriggerStateInfo> triggerStateInfoPckg(triggerStateInfo);
	        LbtGlobal::Read(iMessage,KParamTriggerStateInfo,triggerStateInfoPckg);
	        triggerState = triggerStateInfo.iState;
	        iFireOnUpdate = triggerStateInfo.iFireOnUpdate;
    	    filter = InternalizeFilterFromIpcLC( iMessage, KParamFilter );
    		break;
	        }
	    }

	CLbtContainerUpdateFilter* updOptions = 
        CLbtContainerUpdateFilter::NewL( filter, NULL );
	
	// Since ownership of filter is transferred to updOptions, 
	// pop it from the cleanup stack.
	CleanupStack::Pop( filter );

    TLbtSecurityPolicy securityPolicy;
	// Change the state of the trigger owned by only the requesting 
	// client in case this is a req through LT client API.
	if ( iClientType == CLbtServerLogicBase::TLbtClientLibrary )
	    {
        securityPolicy.SetOwnerSecureId( iMessage.SecureId() );
	    }
	
	// Register for container change event observer
    TLbtTriggerEventMask eventMask = 0;
    eventMask|= CLbtContainer::ELbtConTriggerStateFieldChanged;
                
    iContainer.SetChangeObserver( this ,eventMask );
    
	// Update the trigger state in container
	iContainer.UpdateTriggersState(triggerState, updOptions,iAOIdentificationNum,iFireOnUpdate,
									 iStatus,securityPolicy);
	SetActive();
    }

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::HandleDeleteTriggerOpL()
// ---------------------------------------------------------------------------
//
void CLbtTriggerModifyAOOperation::HandleDeleteTriggerOpL()
    {
    FUNC_ENTER("CLbtTriggerModifyAOOperation::HandleDeleteTriggerOpL");
	CLbtTriggerFilterBase* filter = NULL;
	switch( iFunction )
	    {
	    case ELbtDeleteTrigger:
	        {
    		TPckg<TLbtTriggerId> id(iTriggerId);
    		// Read the trigger id to be deleted
    		LbtGlobal::Read(iMessage, KParamTriggerId, id);
    		
    		// Create a Filter based on the trigger Id.
    		filter = CLbtTriggerFilterByAttribute::NewLC();
    		static_cast<CLbtTriggerFilterByAttribute *>(filter)->AddTriggerIdL(iTriggerId);
            break;	        
	        }
	    case ELbtDeleteTriggers:
	        {
            filter = InternalizeFilterFromIpcLC( iMessage, KParamFilter );
            break;	        
	        }
	    }

	CLbtContainerUpdateFilter* updOptions = 
    			    CLbtContainerUpdateFilter::NewL(filter,
    			                                    NULL);
	// Since ownership of filter is transferred to updOptions, 
	// pop it from the cleanup stack.
	CleanupStack::Pop( filter );

    TLbtSecurityPolicy securityPolicy;
    // Add the SID of the requester client as a filter.
    // This prevents the client from changing the state of any trigger
    // that does not belong to it.
	if ( iClientType == CLbtServerLogicBase::TLbtClientLibrary )
	    {
        securityPolicy.SetOwnerSecureId( iMessage.SecureId() );
	    }
    // Register for container change event observer
    TLbtTriggerEventMask eventMask = 0;
    eventMask|= CLbtContainer::ELbtConTriggerDeleted;
                
    iContainer.SetChangeObserver( this ,eventMask );
    
	// Delete trigger from container
	iContainer.DeleteTriggers(updOptions,iAOIdentificationNum,iStatus,securityPolicy);
	SetActive();
    }

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::GetUpdatedTriggerList
// ---------------------------------------------------------------------------
//
RPointerArray<CLbtContainerTriggerEntry>& CLbtTriggerModifyAOOperation::GetUpdatedTriggerList()
	{
	FUNC_ENTER("CLbtTriggerModifyAOOperation::GetUpdatedTriggerList");
	return iUpdatedTriggerList;
	}

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::CreateTriggerEntryLC()
// ---------------------------------------------------------------------------
//
CLbtTriggerEntry* CLbtTriggerModifyAOOperation::CreateTriggerEntryLC( TInt aType )
    {
    FUNC_ENTER("CLbtTriggerModifyAOOperation::CreateTriggerEntryLC");
    CLbtTriggerEntry* trigger = NULL;
    switch( aType )
        {
        case CLbtTriggerEntry::ETypeSession:
    		trigger = CLbtSessionTrigger::NewLC();
    		break;
        case CLbtTriggerEntry::ETypeStartup:
    		trigger = CLbtStartupTrigger::NewLC();
    		break;
        default:
            User::Leave( KErrArgument );
        }
	return trigger;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::CreateTriggerFilterLC()
// ---------------------------------------------------------------------------
//
CLbtTriggerFilterBase* CLbtTriggerModifyAOOperation::CreateTriggerFilterLC( TInt aType )
    {
    FUNC_ENTER("CLbtTriggerModifyAOOperation::CreateTriggerFilterLC");
    CLbtTriggerFilterBase* filter = NULL;
	switch( aType )
		{
		case CLbtTriggerFilterBase::EFilterByAttribute:
			{
			filter = CLbtTriggerFilterByAttribute::NewLC();
			break;
			}	
		case CLbtTriggerFilterBase::EFilterByArea:
			{
			filter = CLbtTriggerFilterByArea::NewLC();
			break;
			}
		case CLbtTriggerFilterBase::EFilterComposite:
			{
			filter = CLbtTriggerFilterComposite::NewLC();
			break;
			}
		default:
		    User::Leave(KErrArgument);
		}
    return filter;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::InternalizeFilterFromIpcLC()
// ---------------------------------------------------------------------------
//
CLbtTriggerFilterBase* CLbtTriggerModifyAOOperation::InternalizeFilterFromIpcLC( 
    const RMessage2& aMessage, 
    const TInt aIpcSlot )
    {
    FUNC_ENTER("CLbtTriggerModifyAOOperation::InternalizeFilterFromIpcLC");
	CLbtTriggerFilterBase* filter = CreateTriggerFilterLC( aMessage.Int0() );
				
	// Copy the filter object descriptor from the client IPC message
	HBufC8* filterBuffer = LbtGlobal::CopyClientBuffer8LC( aMessage, aIpcSlot );
	
	if( filterBuffer == NULL )
		{
		CleanupStack::PopAndDestroy( filter );
		User::Leave(KErrBadDescriptor);
		}
		
	// Read the buffer into a stream
	RDesReadStream stream( *filterBuffer );
	CleanupClosePushL( stream );
	filter->InternalizeL( stream );
	CleanupStack::PopAndDestroy(2, filterBuffer); // filterBuffer,stream
	return filter;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerModifyAOOperation::InternalizeTriggerFromIpcLC()
// ---------------------------------------------------------------------------
//
CLbtTriggerEntry* CLbtTriggerModifyAOOperation::InternalizeTriggerFromIpcLC( 
    const RMessage2& aMessage, 
    const TInt aIpcSlot )
    {
    FUNC_ENTER("CLbtTriggerModifyAOOperation::InternalizeTriggerFromIpcLC");
    // Create the trigger entry object based on the type of trigger
    
    TLbtTriggerUpdationInfo triggerUpdateInfo;
    TPckg<TLbtTriggerUpdationInfo> triggerUpdateInfoPckg( triggerUpdateInfo );
    LbtGlobal::Read( aMessage,KParamTriggerUpdationInfo,triggerUpdateInfoPckg );
    iFireOnUpdate = triggerUpdateInfo.iFireOnUpdate;
	CLbtTriggerEntry* trigger = CreateTriggerEntryLC( triggerUpdateInfo.iTriggerType );
	
	// Copy the trigger entry object descriptor from the client IPC message
	HBufC8* triggerEntryBuffer = LbtGlobal::CopyClientBuffer8LC( aMessage, aIpcSlot );
	
	if( triggerEntryBuffer == NULL )
		{
		CleanupStack::PopAndDestroy(trigger);
		User::Leave(KErrBadDescriptor);
		}
	
	// Create a Read Stream over the descriptor
	RDesReadStream stream(*triggerEntryBuffer);
	CleanupClosePushL(stream);
	// Internalize the trigger entry class since it is a "C" class
	trigger->InternalizeL(stream);	
	CleanupStack::PopAndDestroy(2, triggerEntryBuffer); //triggerEntryBuffer, stream
    return trigger;
    }


// end of file

