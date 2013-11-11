/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Sim change handler class definition
*
*/


#include <e32property.h> 
#include <commdb.h>
#include <startupdomainpskeys.h>
#include "lbtgeocell.h"
#include "lbttriggerfilterbyarea.h"
#include "lbtlisttriggeroptions.h"
#include "lbtcontainerlistoptions.h"
#include "lbtcontainer.h"
#include "lbtsessiontrigger.h"
#include "lbtstartuptrigger.h"
#include "lbttriggerconditionbase.h"
#include "lbttriggerconditionarea.h"
#include "lbtlisttriggeroptions.h"
#include "lbtsimchangehandler.h"
#include "lbtcleanuphandler.h"
#include "lbtserverlogictriggerchangeobserver.h"
#include "lbtlogger.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtSimChangeHandler::NewL
// ---------------------------------------------------------------------------
//
CLbtSimChangeHandler* CLbtSimChangeHandler::NewL( CLbtContainer& aContainer,
                                                  CLbtCleanupHandler& aCleanupHandler,
                                                  MLbtServerLogicTriggerChangeObserver& aTriggerChangeObserver )
    {
    FUNC_ENTER("CLbtSimChangeHandler::NewL");
    CLbtSimChangeHandler* self = new( ELeave ) CLbtSimChangeHandler( aContainer,aCleanupHandler,aTriggerChangeObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtSimChangeHandler::CLbtSimChangeHandler
// ---------------------------------------------------------------------------
//   
CLbtSimChangeHandler::CLbtSimChangeHandler( CLbtContainer& aContainer,CLbtCleanupHandler& aCleanupHandler,
                                            MLbtServerLogicTriggerChangeObserver& aTriggerChangeObserver):
                                            CActive( EPriorityNormal ),iContainer( aContainer ),
                                            iCleanupHandler( aCleanupHandler ),
                                            iTriggerChangeObserver( aTriggerChangeObserver ),
                                            iNwInfo(),iNwInfoPckg(iNwInfo), iLocArea()
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// CLbtSimChangeHandler::~CLbtSimChangeHandler
// ---------------------------------------------------------------------------
//   
CLbtSimChangeHandler::~CLbtSimChangeHandler()
    {
    FUNC_ENTER("CLbtSimChangeHandler::~CLbtSimChangeHandler");
    if( IsActive() )
        {
        Cancel();
        iContainer.RemoveObserver( this );
        }
        
    iSimProperty.Close();
    iMPhone.Close();
    iTelServer.Close();    
    iInvalidTriggers.Close();
    iValidTriggers.Close();    
    iTriggerList.ResetAndDestroy();    
    }

// ---------------------------------------------------------------------------
// CLbtSimChangeHandler::ConstructL
// ---------------------------------------------------------------------------
//   
void CLbtSimChangeHandler::ConstructL()
    {
    InitialiseEtelL();
    iState = EStateIdle;
    User::LeaveIfError( iSimProperty.Attach( KPSUidStartup, 
                                             KPSSimChanged, EOwnerThread ) );
    }
    
// ---------------------------------------------------------------------------
// CLbtSimChangeHandler::ConstructL
// ---------------------------------------------------------------------------
//   
void CLbtSimChangeHandler::StartToListenL()
    {
    FUNC_ENTER("CLbtSimChangeHandler::StartToListenL");
    TInt simChanged = 0;
    User::LeaveIfError( iSimProperty.Get( simChanged ) );
    
    if( simChanged == ESimChanged || simChanged == ESimNotChanged )
        {
        iState = EStateSimChangeEvent;
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status,KErrNone ); 
        }
    else if( simChanged == ESimChangedUninitialized  )
        {
        iState = EStateGetNetworkInfo;
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status,KErrNone ); 
        }
    else
        {
        SubscribeForSimChange();
        }   
    }
    
// ---------------------------------------------------------------------------
// CLbtSimChangeHandler::RunL
// ---------------------------------------------------------------------------
//      
void CLbtSimChangeHandler::RunL()
    {
    FUNC_ENTER("CLbtSimChangeHandler::RunL");
    if( KErrNone == iStatus.Int() )
        {
        TLbtTriggerEventMask eventMask = 0;
        switch( iState )
            {
            case EStateSimChangeEvent:
                GetCurrentNetworkInfo();
                break;
            
            case EStateGetNetworkInfo:
                ListCellTriggersL();
                break; 
                    
            case EStateListTriggers:
                EvaluateTriggers();
                break;    
            
            case EStateEvaluateTriggers:
                InvalidateDifferentNwTriggersL();
                break;
             
            case EStateInvalidateTriggers:
                // Here the observer is set to the get the information pertaining
                // triggers that were changed to invalid
                eventMask|= CLbtContainer::ELbtConTriggerValidityFieldChanged;
                iContainer.SetChangeObserver( this,eventMask );
                ValidateCurrentNwTriggersL();
                break;
                
            case EStateValidateTriggers:
                // Here the observer is set to the get the information pertaining
                // triggers that were changed to invalid
                eventMask|= CLbtContainer::ELbtConTriggerValidityFieldChanged;
                iContainer.SetChangeObserver( this,eventMask );
                SubscribeForSimChange();
                break;
            
            default:
                break;        
            }
            
        }
    }

// ---------------------------------------------------------------------------
// CLbtSimChangeHandler::RunError
// ---------------------------------------------------------------------------
//      
TInt CLbtSimChangeHandler::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CLbtSimChangeHandler::GetCurrentNetworkInfo
// ---------------------------------------------------------------------------
//   
void CLbtSimChangeHandler::GetCurrentNetworkInfo()
    {
    FUNC_ENTER("CLbtSimChangeHandler::GetCurrentNetworkInfo");
    TInt error = iMPhone.GetCurrentMode( iNetworkMode );
    if( KErrNone == error )
        {
        iMPhone.GetCurrentNetwork( iStatus,iNwInfoPckg,iLocArea );
        iState = EStateGetNetworkInfo;
        SetActive();
        }
    }

// ---------------------------------------------------------------------------
// CLbtSimChangeHandler::ListCellTriggers
// ---------------------------------------------------------------------------
//   
void CLbtSimChangeHandler::ListCellTriggersL()
    {
    FUNC_ENTER("CLbtSimChangeHandler::ListCellTriggers");
    CLbtGeoCell* geoCell = CLbtGeoCell::NewLC();
                    
    CLbtTriggerFilterByArea* filter = CLbtTriggerFilterByArea::NewL();    
    filter->SetArea( geoCell );
    CleanupStack::Pop( geoCell );//Ownership is transfered to filter
    CleanupStack::PushL( filter );
    
    CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();    
    listOptions->SetFilter( filter );
    CleanupStack::Pop( filter ); //Ownership is transfered to listOptions
    CleanupStack::PushL( listOptions );    
    
    // We need only trigger Id and condition
    TLbtTriggerAttributeFieldsMask attributeMask = 0;
    attributeMask|= CLbtTriggerEntry::EAttributeId|
                    CLbtTriggerEntry::EAttributeCondition;
    listOptions->SetRetrievedFields( attributeMask , 0 );
    
    CLbtContainerListOptions* contianerListOptions = 
                                CLbtContainerListOptions::NewL( listOptions,NULL );
                                
    iTriggerList.Reset();                            
    iContainer.ListTriggers( contianerListOptions,iTriggerList,iAoOpcode,iStatus );
    iState = EStateListTriggers;
    SetActive();
    
    CleanupStack::Pop( listOptions );//Ownership is transfered to container list options
    }

// ---------------------------------------------------------------------------
// CLbtSimChangeHandler::EvaluateTriggers
// ---------------------------------------------------------------------------
//   
void CLbtSimChangeHandler::EvaluateTriggers()
    {
    FUNC_ENTER("CLbtSimChangeHandler::EvaluateTriggers");    
    // If the count is zero,register again for sim change notification
    if( !iTriggerList.Count() )
        {
        SubscribeForSimChange();
        return;
        }
        
    // Reset the valid and invalid trigger arrays
    iInvalidTriggers.Reset();
    iValidTriggers.Reset();
    
    // Iterate through trigger list and mark the trigger that doesnt belong to this network
    for( TInt i=0;i<iTriggerList.Count();i++ )
        {
        CLbtTriggerEntry* triggerEntry = iTriggerList[i]->TriggerEntry();
        
        // Create the trigger entry object based on the type of trigger
    	CLbtTriggerEntry* trigger;
    	if( triggerEntry->Type() == CLbtTriggerEntry::ETypeStartup )
    		{
    		trigger = static_cast<CLbtStartupTrigger*>( triggerEntry );
    		}
    	else
    		{
    		trigger = static_cast<CLbtSessionTrigger*>( triggerEntry );
    		}
        
        if( trigger->GetCondition()->Type() == CLbtTriggerConditionBase::ETriggerConditionArea )
    		{
    		// Get the condition area base
    		CLbtTriggerConditionArea* conditionArea = 
    				static_cast<CLbtTriggerConditionArea*>(trigger->GetCondition());
    		
    		// Check if the area is a cell
    		if( conditionArea->TriggerArea()->Type() == CLbtGeoAreaBase::ECellular )
    			{
    			CLbtGeoCell* cellularArea = static_cast<CLbtGeoCell*>(conditionArea->TriggerArea());
    			
    			TInt ctNetworkCode, trNetworkCode;
                TLex lex( iNwInfo.iNetworkId );
                lex.Val( ctNetworkCode );

                lex.Assign( cellularArea->NetworkIdentityCode() );
                lex.Val( trNetworkCode );
                
                TInt error;
                // If the network type,country code and network code of trigger 
                // matches with current cell reading, add it to invalid trigger array
                if( trNetworkCode != ctNetworkCode )
                    {
                    error = iInvalidTriggers.Append( trigger->Id() );
                    if( error != KErrNone )
                        {
                        LOG1("Failed to append trigger id:%d",error);
                        return;
                        }
                    }
                else if( trNetworkCode == ctNetworkCode  )
                    {
                    error = iValidTriggers.Append( trigger->Id() );
                    if( error != KErrNone )
                        {
                        LOG1("Failed to append trigger id:%d",error);
                        return;
                        }
                    }
    			}
    		}
        }
    iState = EStateEvaluateTriggers; 
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status,KErrNone );    
    }

// ---------------------------------------------------------------------------
// CLbtSimChangeHandler::InvalidateTriggers
// ---------------------------------------------------------------------------
//   
void CLbtSimChangeHandler::InvalidateDifferentNwTriggersL()
    {
    FUNC_ENTER("CLbtSimChangeHandler::InvalidateDifferentNwTriggers");
    // Change the validity of these triggers to InValid
    LOG("Invalidating Current N/W Triggers");
    iState = EStateInvalidateTriggers; 
    SetActive();
    if( iInvalidTriggers.Count() )
    	{
	    iContainer.UpdateTriggersValidity( TLbtTriggerDynamicInfo::EInvalid,
                                           iInvalidTriggers,
                                           iAoOpcode,
                                           iStatus );
		iCleanupHandler.AddTriggersForCleanupL( iInvalidTriggers );
    	}
    else    
    	{
    	TRequestStatus* status = &iStatus;
    	User::RequestComplete(status, KErrNone);
    	}
    }

// ---------------------------------------------------------------------------
// CLbtSimChangeHandler::ValidateTriggers
// ---------------------------------------------------------------------------
//   
void CLbtSimChangeHandler::ValidateCurrentNwTriggersL()
    {
    FUNC_ENTER("CLbtSimChangeHandler::ValidateCurrentNwTriggers");
    LOG("Validating Current N/W Triggers");    
    // Change the validity of these triggers to Valid
    iState = EStateValidateTriggers; 
    SetActive();
    if( iValidTriggers.Count() )
    	{
        iContainer.UpdateTriggersValidity( TLbtTriggerDynamicInfo::EValid,
                                           iValidTriggers,
                                           iAoOpcode,
                                           iStatus );
        iCleanupHandler.RemoveTriggersFromCleanupL( iValidTriggers );
    	}
    else
    	{
    	TRequestStatus* status = &iStatus;
    	User::RequestComplete(status, KErrNone);
    	}    
    }
    
    
// ---------------------------------------------------------------------------
// CLbtSimChangeHandler::SubscribeForSimChange
// ---------------------------------------------------------------------------
//   
void CLbtSimChangeHandler::SubscribeForSimChange()
    {
    FUNC_ENTER("CLbtSimChangeHandler::SubscribeForSimChange");
    iSimProperty.Subscribe( iStatus );
    iState = EStateSimChangeEvent;
    SetActive();   
    }

// ---------------------------------------------------------------------------
// CLbtSimChangeHandler::DoCancel
// ---------------------------------------------------------------------------
//   
void CLbtSimChangeHandler::DoCancel()
    {
    FUNC_ENTER("CLbtSimChangeHandler::DoCancel");
    switch( iState )
        {
        case EStateSimChangeEvent:
            iSimProperty.Cancel();
            break;
        
        case EStateGetNetworkInfo:
            iMPhone.CancelAsyncRequest( EMobilePhoneGetCurrentNetwork );
            break;
        
        case EStateListTriggers:
        case EStateInvalidateTriggers:
        case EStateValidateTriggers:
            iContainer.CancelAsyncOperation( iAoOpcode );
            break;
        
        default:
            break;          
        }
    }

// ---------------------------------------------------------------------------
// CLbtSimChangeHandler::TriggerStoreChanged
// ---------------------------------------------------------------------------
//
void CLbtSimChangeHandler::TriggerStoreChanged( RArray<TLbtTriggerId>& aTriggerIds,
                                               TLbtTriggerEventMask aEventMask,
                                               TLbtContainerChangedAreaType /*aAreaType*/,
                                               RArray<TUid>& aManagerUids )
    {
    // Remove observer
    iContainer.RemoveObserver( this );
    iTriggerChangeObserver.HandleTriggersChange( aTriggerIds,aManagerUids,aEventMask );
    }

// ----------------------------------------------------------------------------
// CLbtSimChangeHandler::InitialiseEtelL
// ----------------------------------------------------------------------------
//
void CLbtSimChangeHandler::InitialiseEtelL()
    {
    FUNC_ENTER("CLbtSimChangeHandler::InitialiseEtelL");
    CCommsDatabase* const db = CCommsDatabase::NewL(ETrue); 
	CleanupStack::PushL(db); 

	TUint32 modemId = 0; 
	db->GetGlobalSettingL(TPtrC(MODEM_PHONE_SERVICES_SMS), modemId); 
	CCommsDbTableView* const view =
	    db->OpenViewMatchingUintLC(TPtrC(MODEM), TPtrC(COMMDB_ID), modemId); 

	TInt err = view->GotoFirstRecord();
	if(err != KErrNone)
	    {
        User::Leave(err);
	    }

    HBufC* nameBuf = NULL;
    nameBuf = view->ReadLongTextLC(TPtrC(MODEM_TSY_NAME));

    User::LeaveIfError(iTelServer.Connect());
	err = iTelServer.LoadPhoneModule(*nameBuf);
	
	if(err != KErrNone)
	    {
        User::Leave(err);
	    }
		    
	// For the phone information
	RTelServer::TPhoneInfo info;

	iTelServer.GetPhoneInfo(0, info); 
	err = iMPhone.Open(iTelServer, info.iName);

	if(err != KErrNone)
	    {
        User::Leave(err);
	    }
   
	CleanupStack::PopAndDestroy(3); // nameBuf, view and db 
	}
    
//end of file

