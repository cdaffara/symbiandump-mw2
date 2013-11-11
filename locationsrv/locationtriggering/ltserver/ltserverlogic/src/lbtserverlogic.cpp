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
* Description:  LBT Sever Logic
*
*/


// INCLUDE FILES
#include <ecom.h>
#include <e32base.h>
#include <implementationproxy.h>
#include <e32property.h>
#include <lbtstatuspskeys.h>
#include <lbttriggeringsystemsettings.h>
#include <lbterrors.h>
#include "lbtcontainertriggerfilter.h"
#include "lbtserverlogic.h"
#include "lbtserverconsts.h"
#include "lbtcreatetriggeraooperation.h"
#include "lbtstrategybase.h"
#include "lbttriggerfirehandler.h"
#include "lbtbackuprestorelistener.h"
#include "lbtcontainer.h"
#include "lbtglobal.h"
#include "lbtstratergycontainer.h"
#include "lbtlistaooperation.h"
#include "lbtsettingsmanager.h"
#include "lbtcontainertriggerentry.h"
#include "lbttriggermodifyaooperation.h"
#include "lbtnotificationmap.h"
#include "lbttriggerchangeevent.h"
#include "lbtcontainerupdatefilter.h"
#include "lbttriggerfilterbyattribute.h"
#include "lbtlogger.h"
#include "lbtdeletesessiontriggers.h"
#include "lbtappchangehandler.h"
#include "lbtcleanuphandler.h"
#include "lbtsimchangehandler.h"
#include "lbtlogger.h"

// ===================== LOCAL FUNCTIONS =====================

const TImplementationProxy ImplementationTable[] = 
	{
    IMPLEMENTATION_PROXY_ENTRY( 0x1028312C, CLbtServerLogic::NewL ) 
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );
    return ImplementationTable;
    }


TInt OrderTriggerEntryBySid(
    const CLbtContainerTriggerEntry& aLhs, 
    const CLbtContainerTriggerEntry& aRhs)
	{
	CLbtContainerTriggerEntry& lhs = const_cast<CLbtContainerTriggerEntry&>(aLhs);
	CLbtContainerTriggerEntry& rhs = const_cast<CLbtContainerTriggerEntry&>(aRhs);
	
	if(lhs.ExtendedTriggerInfo()->OwnerSid() < rhs.ExtendedTriggerInfo()->OwnerSid())
		{
		return -1;
		}
	else if(lhs.ExtendedTriggerInfo()->OwnerSid() > rhs.ExtendedTriggerInfo()->OwnerSid())
		{
		return 1;
		}
	else
		{
		return 0;
		}
	}

// ===================== MEMBER FUNCTIONS =====================

// ---------------------------------------------------------------------------
// CLbtServerLogic::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
CLbtServerLogic* CLbtServerLogic::NewL()
	{
    LBT_TRACE(KLbtLogVerbose|KLbtLogServerLogic,__FILE__, __LINE__, "Entering NewL" );
	CLbtServerLogic* self = new ( ELeave ) CLbtServerLogic;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    LBT_TRACE(KLbtLogVerbose|KLbtLogServerLogic,__FILE__, __LINE__, "Returning from NewL" );
    return self;
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::~CLbtServerLogic
// Destructor
// ---------------------------------------------------------------------------
//
CLbtServerLogic::~CLbtServerLogic()
	{
	FUNC_ENTER("CLbtServerLogic::~CLbtServerLogic");
    // Cancel all outstanding operations.		
	CancelAllOperations();

	delete iBackupRestoreListener;
	delete iStratergyContainer;
	delete iSettingsManager;
	delete iFireHandler;
	delete iAppChangeHandler;
	delete iNotificationMap;
	delete iCleanupHandler;
    delete iSimChangeHandler;	
	iAOOArray.ResetAndDestroy();
	iDeleteSessionTriggersArray.ResetAndDestroy();
	CLbtContainer::Destroy();
	iProperty.Close();
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::CLbtServerLogic
// ---------------------------------------------------------------------------
//
CLbtServerLogic::CLbtServerLogic() : iContainer(NULL)
	{
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::ServiceL
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::ServiceL(const RMessage2 &aMessage, TSubSessionType aType)
	{
	FUNC_ENTER("CLbtServerLogic::ServiceL");
    LBT_TRACE(KLbtLogVerbose|KLbtLogServerLogic,__FILE__, __LINE__, "Received IPC function = %d client = %x", aMessage.Function(), aMessage.SecureId().iId );
	// If backup or restore operation is in progress then return server busy
	if(iBackupRestoreListener->IsBackupRestoreOperationInProgress())
		{
		LbtGlobal::RequestComplete(aMessage, KErrServerBusy);
		return;
		}	
		
	/**
	 * All IPC messages which requires notification will be 
	 * stored and serviced by server logic. Other messages will
	 * be serviced by AOOperation objects
	 */ 
	switch(aMessage.Function())
		{
		/**
		 * The below are all notification requests. Hence insert the 
		 * message into the message array and notify the requestor  
		 * when the event occurs	
		 */
		case ELbtNotifyTriggerChangeEvent:
		case ELbtNotifyTriggerFired:
		case ELbtNotifyNearestTriggerChange:
		case ELbtNotifyTriggeringSysSettingChange:
		case ELbtNotifyTriggeringSysStatusChange:
			{
			TInt retValue=ValidateChangeNotification(aMessage);
			
			if(retValue!=KErrNone)
				{
				// If validation failed then complete the request with the appropriate error code
				LbtGlobal::RequestComplete(aMessage, retValue);				
				break;
				}
			InsertIntoNotificationMapL(aMessage, aType);			
			break;
			}
		case ELbtCnclNotifyTriggerChangeEvent:		
			{
			TInt retVal = RemoveFromNotificationMap(aMessage, ELbtNotifyTriggerChangeEvent);
			LbtGlobal::RequestComplete(aMessage, retVal);
			break;
			}
		case ELbtCnclNotifyTriggerFired:
			{
			TInt retVal = RemoveFromNotificationMap(aMessage, ELbtNotifyTriggerFired);
			LbtGlobal::RequestComplete(aMessage, retVal);			
			break;
			}
		case ELbtCnclNotifyNearestTriggerChange:
			{
			TInt retVal = RemoveFromNotificationMap(aMessage, ELbtNotifyNearestTriggerChange);
			LbtGlobal::RequestComplete(aMessage, retVal);			
			break;
			}
		case ELbtCnclNotifyTriggeringSysStatusChange:
			{
			TInt retVal = RemoveFromNotificationMap(aMessage, ELbtNotifyTriggeringSysStatusChange);
			LbtGlobal::RequestComplete(aMessage, retVal);			
			break;
			}
		case ELbtCnclNotifyTriggeringSysSettingChange:
			{
			TInt retVal = RemoveFromNotificationMap(aMessage, ELbtNotifyTriggeringSysSettingChange);
			LbtGlobal::RequestComplete(aMessage, retVal);			
			break;
			}
		case ELbtCreateTrigger: // create trigger
			{
			// Validate the IPC message and the trigger for create trigger
			TInt retVal = ValidateCreateTriggerMessage(aMessage);
			
			if( retVal != KErrNone )
				{
				// If validation failed then complete the request with the appropriate error code
				LbtGlobal::RequestComplete(aMessage, retVal);				
				break;
				}
				
			// If capability check is successful then create the aooperation object
			CLbtCreateTriggerAOOperation* operation = 
						CLbtCreateTriggerAOOperation::NewL(*this, 
														   aMessage, 
														   *iContainer, 
														   aType,
														   *iSettingsManager);
			CleanupStack::PushL( operation );
			iAOOArray.AppendL(operation);
			CleanupStack::Pop( operation );
			operation->StartOperationL();
			break;
			}
		case ELbtGetTriggers:
		case ELbtGetFiredTriggers:
		case ELbtListTriggerIds:
			{
			TInt retValue=ValidateGeneralOperation(aMessage);
			
			if(retValue!=KErrNone)
				{
				// If validation failed then complete the request with the appropriate error code
				LbtGlobal::RequestComplete(aMessage, retValue);				
				break;
				}
			for(TInt i = 0; i < iAOOArray.Count(); ++i)
				{
				CLbtAOOperationBase* operation = iAOOArray[i];				
				if(operation->GetSecureId() == aMessage.SecureId() &&
				   operation->GetSubSessionHandle() == aMessage.Int3() &&
				   ( iAOOArray[i]->GetFunction() == ELbtGetTriggersBufferSize ||
    			     iAOOArray[i]->GetFunction() == EGetFiredTriggersCount ||
    			     iAOOArray[i]->GetFunction() == ELbtGetListTriggerIdsSize ) )
					{
					// The session id and the client of the message are the 
					// same and since a sub-session can have just one outstanding
					// asynchronous service request, the AO object obtained must be
					// the listAO operation object to service this request
					CLbtListAOOperation* listOperation = static_cast<CLbtListAOOperation*>(operation);
					listOperation->ServiceMessageL(aMessage);
					break;
					}
				}
			break;
			}
		case ELbtGetTriggersBufferSize:
		case EGetFiredTriggersCount:
		case ELbtGetListTriggerIdsSize:
			{
			TInt retValue=ValidateGeneralOperation(aMessage);
			
			if(retValue!=KErrNone)
				{
				// If validation failed then complete the request with the appropriate error code
				LbtGlobal::RequestComplete(aMessage, retValue);				
				break;
				}
			CLbtListAOOperation* operation = 
						CLbtListAOOperation::NewL(*this, aMessage, *iContainer, aType);
			CleanupStack::PushL( operation );
		    iAOOArray.AppendL(operation);
		    CleanupStack::Pop( operation );
			operation->StartOperationL();
			break;
			}
		case ELbtUpdateTrigger:
		case ELbtSetTriggerState:
		case ELbtSetTriggersState:
		case ELbtDeleteTrigger:
		case ELbtDeleteTriggers:
			{
			TInt retValue=ValidateGeneralOperation(aMessage);
			
			if(retValue!=KErrNone)
				{
				// If validation failed then complete the request with the appropriate error code
				LbtGlobal::RequestComplete(aMessage, retValue);				
				break;
				}
			
			CLbtTriggerModifyAOOperation* operation = 
						CLbtTriggerModifyAOOperation::NewL(*this, aMessage, *iContainer, aType,*iSettingsManager);
			CleanupStack::PushL( operation );
		    iAOOArray.AppendL(operation);
		    CleanupStack::Pop( operation );
			operation->StartOperationL();
			break;
			}
		case ELbtGetTriggeringSysSetting:
			{
			TInt retValue=ValidateGetTriggeringSysSetting(aMessage);
			
			if(retValue!=KErrNone)
				{
				// If validation failed then complete the request with the appropriate error code
				LbtGlobal::RequestComplete(aMessage, retValue);				
				break;
				}
				
			TInt retVal = GetTriggeringSystemSettings( aMessage, aType );

			if(retVal == KErrNone)
				{
				LbtGlobal::RequestComplete(aMessage, KErrNone);
				}
			break;
			}
			
		case ELbtSetTriggeringSettings:
			{
			TInt retValue=ValidateSetTriggeringSetting(aMessage);
			
			if(retValue!=KErrNone)
				{
				// If validation failed then complete the request with the appropriate error code
				LbtGlobal::RequestComplete(aMessage, retValue);				
				break;
				}
				
        	// The request can only be through the LT management API.
        	if ( TLbtManagementLibrary != aType ) 
			    {
			    // This is not a management library. Hence return access denied.
    		    aMessage.Complete( KErrAccessDenied );
			    }
            else
                {
        	    TLbtTriggeringSystemManagementSettings settings;
        		TPckg<TLbtTriggeringSystemManagementSettings> pckgSettings(settings);
                // In case of error in LbtGlobal::Read then the client is already
                // panic'ed hence there is no need to complete the message again.
    			if( !LbtGlobal::Read(aMessage, KParamSettings, pckgSettings) )
    				{
                    // KParamMask slot contains the mask of the settings to be changed.
                    SetTriggeringSystemSettings( settings, aMessage.Int0() );
        		    aMessage.Complete( KErrNone );
    				}
                }
			// TODO: Handle triggering settings change request from management library
			//HandleNotificationOperationsL(aMessage);
			break;
			}
		case ELbtGetTriggeringSysStatus:
	        {
	        TInt retValue=ValidateGetTriggeringSysStatus(aMessage);
			
			if(retValue!=KErrNone)
				{
				// If validation failed then complete the request with the appropriate error code
				LbtGlobal::RequestComplete(aMessage, retValue);				
				break;
				}
				
			if ( TLbtManagementLibrary != aType ) 
			    {
			    // This is not a management library. Hence return access denied.
    		    aMessage.Complete( KErrAccessDenied );
			    }
		    
		    TLbtTriggeringSystemStatus systemStatus;
		    GetTriggeringSysStatus( systemStatus );
		    
		    TPckg<TLbtTriggeringSystemStatus> systemStatusPckg( systemStatus );
		    
		    TInt error = LbtGlobal::Write(aMessage, KParamStatus, systemStatusPckg); 
		    LbtGlobal::RequestComplete(aMessage, error);
		    
            break;
	        }
	        
	    case ELbtCancelCreateTrigger:
	    	{
	    	if(!iAOOArray.Count())
	    		{
	    		LbtGlobal::RequestComplete(aMessage,KErrNotFound);	
	    		break;
	    		}
	    	for(TInt i=0;i<iAOOArray.Count();i++)
	    		{
	    		if((iAOOArray[i]->GetSecureId()==aMessage.SecureId()) &&
	    			(iAOOArray[i]->GetFunction()==ELbtCreateTrigger) &&
	    			(iAOOArray[i]->GetSubSessionHandle()==aMessage.Int3()))
	    			{
	    			CLbtCreateTriggerAOOperation* createTriggerAOOperation=
	    							static_cast<CLbtCreateTriggerAOOperation*>
	    							(iAOOArray[i]);
	    			createTriggerAOOperation->CancelCreateTrigger();	
	    			LbtGlobal::RequestComplete(aMessage,KErrNone);
	    			return;	
	    			}
	    		}
	    	LbtGlobal::RequestComplete(aMessage,KErrNotFound);
			break;	    
	    	}
	    		
		case ELbtCancelDeleteTriggers:
		    {
		    if(!iAOOArray.Count())
                {
                LbtGlobal::RequestComplete(aMessage,KErrNotFound);  
                break;
                }
            for(TInt i=0;i<iAOOArray.Count();i++)
                {
                if((iAOOArray[i]->GetSecureId()==aMessage.SecureId())&& 
                    (iAOOArray[i]->GetSubSessionHandle()==aMessage.Int3()) &&
                    (iAOOArray[i]->GetFunction()==ELbtDeleteTriggers))
                    {
                    CLbtTriggerModifyAOOperation* triggerModifyAOOperation=
                                    static_cast<CLbtTriggerModifyAOOperation*>
                                    (iAOOArray[i]);
                    triggerModifyAOOperation->CancelModifyOperation();  
                    LbtGlobal::RequestComplete(aMessage,KErrNone); 
                    return;
                    }
                }
            LbtGlobal::RequestComplete(aMessage,KErrNotFound);  
            break;      
            }
		case ELbtCancelSetTriggersState:
		    {
		    if(!iAOOArray.Count())
                {
                LbtGlobal::RequestComplete(aMessage,KErrNotFound);  
                break;
                }
            for(TInt i=0;i<iAOOArray.Count();i++)
                {
                if((iAOOArray[i]->GetSecureId()==aMessage.SecureId())&& 
                    (iAOOArray[i]->GetSubSessionHandle()==aMessage.Int3()) &&
                    (iAOOArray[i]->GetFunction()==ELbtSetTriggersState))
                    {
                    CLbtTriggerModifyAOOperation* triggerModifyAOOperation=
                                    static_cast<CLbtTriggerModifyAOOperation*>
                                    (iAOOArray[i]);
                    triggerModifyAOOperation->CancelModifyOperation();  
                    LbtGlobal::RequestComplete(aMessage,KErrNone); 
                    return;
                    }
                }
            LbtGlobal::RequestComplete(aMessage,KErrNotFound);  
            break;      
            }
		case ELbtCancelUpdateTrigger:    
	    	{
	    	if(!iAOOArray.Count())
	    		{
	    		LbtGlobal::RequestComplete(aMessage,KErrNotFound);	
	    		break;
	    		}
	    	for(TInt i=0;i<iAOOArray.Count();i++)
	    		{
	    		if((iAOOArray[i]->GetSecureId()==aMessage.SecureId())&& 
	    		    (iAOOArray[i]->GetSubSessionHandle()==aMessage.Int3()) &&
	    			(iAOOArray[i]->GetFunction()==ELbtUpdateTrigger ))
	    			{
	    			CLbtTriggerModifyAOOperation* triggerModifyAOOperation=
	    							static_cast<CLbtTriggerModifyAOOperation*>
	    							(iAOOArray[i]);
	    			triggerModifyAOOperation->CancelModifyOperation();	
                    LbtGlobal::RequestComplete(aMessage,KErrNone); 
                    return;
	    			}
	    		}
	    	LbtGlobal::RequestComplete(aMessage,KErrNotFound);	
			break;	 	
	    	} 
	    
		case ELbtCancelListTriggerIds:
		    {
		    if(!iAOOArray.Count())
                {
                LbtGlobal::RequestComplete(aMessage,KErrNotFound);  
                break;
                }
            for(TInt i=0;i<iAOOArray.Count();i++)
                {
                if((iAOOArray[i]->GetSecureId()==aMessage.SecureId()) &&
                    (iAOOArray[i]->GetSubSessionHandle()==aMessage.Int3()) &&
                    (iAOOArray[i]->GetFunction()==ELbtGetListTriggerIdsSize))
                    {
                    CLbtListAOOperation* listAOOperation=
                                    static_cast<CLbtListAOOperation*>
                                    (iAOOArray[i]);
                    listAOOperation->CancelListOperation(); 
                    LbtGlobal::RequestComplete(aMessage,KErrNone); 
                    return;
                    }
                }
            LbtGlobal::RequestComplete(aMessage,KErrNotFound);  
            break;   
            }
		case ELbtCancelGetTriggers:
	    	{
	    	if(!iAOOArray.Count())
	    		{
	    		LbtGlobal::RequestComplete(aMessage,KErrNotFound);	
	    		break;
	    		}
	    	for(TInt i=0;i<iAOOArray.Count();i++)
	    		{
	    		if((iAOOArray[i]->GetSecureId()==aMessage.SecureId()) &&
	    			(iAOOArray[i]->GetSubSessionHandle()==aMessage.Int3()) &&
	    			(iAOOArray[i]->GetFunction()==ELbtGetTriggersBufferSize))
	    			{
	    			CLbtListAOOperation* listAOOperation=
	    							static_cast<CLbtListAOOperation*>
	    							(iAOOArray[i]);
	    			listAOOperation->CancelListOperation();	
                    LbtGlobal::RequestComplete(aMessage,KErrNone); 
                    return;
	    			}
	    		}
	    	LbtGlobal::RequestComplete(aMessage,KErrNotFound);	
			break;	 
	    	}
	    	
		case ELbtCancelAll:
			{
			RMessage2 message;
		    while( !iNotificationMap->Retreive( message,aMessage.Int3(),aMessage.SecureId() ) )
		           {
	               LbtGlobal::RequestComplete(message, KErrCancel);
		           }
			if(!iAOOArray.Count())
	    		{
	    		LbtGlobal::RequestComplete(aMessage,KErrNotFound);	
	    		break;
	    		}
			for(TInt i=0;i<iAOOArray.Count();i++)
				{
				if(iAOOArray[i]->GetSecureId()==aMessage.SecureId() &&
					(iAOOArray[i]->GetSubSessionHandle()==aMessage.Int3()))
					{
					switch(iAOOArray[i]->GetFunction())
						{
						case ELbtCreateTrigger:
							{
							CLbtCreateTriggerAOOperation* createTriggerAOOperation=
	    							static_cast<CLbtCreateTriggerAOOperation*>
	    							(iAOOArray[i]);
	    					createTriggerAOOperation->CancelCreateTrigger();
	    					break;	
							}
						case ELbtDeleteTriggers:
						case ELbtSetTriggersState:
						case ELbtUpdateTrigger:    
							{
							CLbtTriggerModifyAOOperation* triggerModifyAOOperation=
	    							static_cast<CLbtTriggerModifyAOOperation*>
	    							(iAOOArray[i]);
	    					triggerModifyAOOperation->CancelModifyOperation();
	    					break;
							}
						case ELbtGetListTriggerIdsSize:
						case ELbtGetTriggersBufferSize: 
							{
							CLbtListAOOperation* listAOOperation=
	    							static_cast<CLbtListAOOperation*>
	    							(iAOOArray[i]);
	    					listAOOperation->CancelListOperation();	
	    					break;
							}
						default:
							{
							break;
							}	
						}
					}
				}
			LbtGlobal::RequestComplete(aMessage,KErrNone);		
			break;	
			} 
		default:
			{
			break;
			}
		}
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::LoadOrUnloadStrategyPluginL
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::LoadOrUnloadStrategyPluginL()
	{
	FUNC_ENTER("CLbtServerLogic::LoadOrUnloadStrategyPluginL");
	// Load stratergy if it is not already loaded and if there are valid
	// and enabled triggers in the system
	if(!iStratergyContainer->Stratergy() &&  // if stratergy not already loaded
		iContainer->GetCountOfEnabledAndValidTriggers() && // if there are enabled and valid triggers
		( iSettingsManager->GetTriggeringMechanismState() == ETriggeringMechanismOn )&&
		!iBackupRestoreListener->IsBackupRestoreOperationInProgress()) // if backup or restore operation is not in progress
		{
        LBT_TRACE(KLbtLogVerbose|KLbtLogServerLogic,__FILE__, __LINE__, "Loading Strategy plugin");
		iStratergyContainer->LoadStratergyL(this);
		iStratergyContainer->Stratergy()->StartSupervision();	
		iStrategyDynamicInfo.iLocationAcquisitionStatus = ELocationAcquisitionActive;
		NotifyTriggeringSystemStatusChange();
		}
	
	else if(!iContainer->GetCountOfEnabledAndValidTriggers())
		{
		// If there are no triggers in the system to be supervised then
		// unload stratergy
        LBT_TRACE(KLbtLogVerbose|KLbtLogServerLogic,__FILE__, __LINE__, "Unloading Strategy plugin");
		iStratergyContainer->UnloadStratergy();
		iStrategyDynamicInfo.iLocationAcquisitionStatus = ELocationAcquisitionInactive;
		NotifyTriggeringSystemStatusChange();
		} 
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::HandleSubSessionClosure
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::HandleSubSessionClosure(const TInt aSubSessionId, const CSession2* aSession)
	{
	FUNC_ENTER("CLbtServerLogic::HandleSubSessionClosure");
    LBT_TRACE(KLbtLogVerbose|KLbtLogServerLogic,__FILE__, __LINE__, "HandleSubSessionClosure called");
	// Since the sub-session is being closed remove all the messages
	// from the message array
	iNotificationMap->CompleteMessagesOfSubsession(aSession, aSubSessionId, KErrDisconnected);

	// Stop all the AOOperation object pertaining to the 
	// sub-session that is being closed. 
	for(TInt i = iAOOArray.Count() - 1; i >= 0 ; --i)
		{
		CLbtAOOperationBase* operation = iAOOArray[i];
		if(operation->GetSubSessionHandle() == aSubSessionId && 
		   operation->GetSession() == aSession )
			{
			iAOOArray.Remove(i);
			delete operation;
			}
		}
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::HandleSessionClosureL
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::HandleSessionClosureL(const TSecureId aSecureId)
	{
	FUNC_ENTER("CLbtServerLogic::HandleSessionClosureL");
    LBT_TRACE(KLbtLogVerbose|KLbtLogServerLogic,__FILE__, __LINE__, "Session closed by client %x", aSecureId.iId );
	// Inform container to delete all the sessions triggers of 
	// the client with secure id aSecureId.	
	CLbtContainerFilter* filter = CLbtContainerFilter::NewL();
	CleanupStack::PushL(filter);
	filter->AddOwnerSidInFilterL(aSecureId);
	
	CLbtTriggerFilterByAttribute* filterByAttribute = 
				CLbtTriggerFilterByAttribute::NewL();
	CleanupStack::PushL(filterByAttribute);
	filterByAttribute->AddTriggerTypeL(CLbtTriggerEntry::ETypeSession);
	
	CLbtContainerUpdateFilter* containerFilter = 
							CLbtContainerUpdateFilter::NewL(filterByAttribute, filter);
	CleanupStack::PushL(containerFilter);
	
	CLbtDeleteSessionTriggers* deleteSessionTriggers=
							CLbtDeleteSessionTriggers::NewL(*this,*iContainer,*iNotificationMap);
	CleanupStack::PushL( deleteSessionTriggers );
	iDeleteSessionTriggersArray.AppendL(deleteSessionTriggers);
	CleanupStack::Pop( deleteSessionTriggers );
							
	deleteSessionTriggers->DeleteSessionTriggers(containerFilter);
	iNotificationMap->RemoveAllClientMessages(aSecureId);
	CleanupStack::Pop(containerFilter);
	CleanupStack::Pop(filterByAttribute);
	CleanupStack::Pop(filter);						
	}
	

// ---------------------------------------------------------------------------
// CLbtServerLogic::HandleDeleteSessionTriggersClosureL
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::HandleDeleteSessionTriggersClosureL(CLbtDeleteSessionTriggers* aDeleteSessionTriggers)
	{
	FUNC_ENTER("CLbtServerLogic::HandleDeleteSessionTriggersClosureL");
	TInt index = iDeleteSessionTriggersArray.Find(aDeleteSessionTriggers);
	if(index != KErrNotFound)
		{
		iDeleteSessionTriggersArray.Remove(index);
		}
	delete aDeleteSessionTriggers;
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::HandleOperationClosureL
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::HandleOperationClosureL(CLbtAOOperationBase* aOperation, TInt aStatus)
	{
	FUNC_ENTER("CLbtServerLogic::HandleOperationClosureL");
	switch(aOperation->GetFunction())
		{
		case ELbtCreateTrigger:
		case ELbtUpdateTrigger:
		case ELbtSetTriggerState:
		case ELbtSetTriggersState:
		case ELbtDeleteTrigger:
		case ELbtDeleteTriggers:
			{
			LoadOrUnloadStrategyPluginL();
			if(aStatus == KErrNone)
				{
				// Send the active object operation object and the notification type
				HandleNotificationOperationsL(aOperation, ELbtNotifyTriggerChangeEvent);
				}
			break;
			}
		default:
			{
			break;
			}
		}

	// Check which AOOperation object has notified. Delete the	
	// object and remove it from the aooperation array.
	TInt index = iAOOArray.Find(aOperation);
	if(index != KErrNotFound)
		{
		iAOOArray.Remove(index);
		}
	delete aOperation;
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::HandleBackupRestoreOperationL
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::HandleBackupRestoreOperationL(TBRState aState)
	{
	FUNC_ENTER("CLbtServerLogic::HandleBackupRestoreOperationL");
	if(aState == MLbtBackupRestoreObserver::EStarted)
		{
		// The logic to handle backup / restore operation is as follows.
		// 1. Cancel All outstanding requests from clients first. Since the AO 
		//    operations will be using container, this should be done first. 
		//    This will also close all pending IPC messages with KErrServerBusy.
		// 2. Then unload the strategy implementation.
		// 3. Delete the Container instance now.

        // Cancel all outstanding operations.		
		CancelAllOperations();
		
		// Delete all handlers that use server logics container instance
		delete iFireHandler;
		iFireHandler = NULL;
		
		delete iSimChangeHandler;
		iSimChangeHandler = NULL;
		
		delete iAppChangeHandler;
		iAppChangeHandler = NULL;
		
		delete iCleanupHandler;
		iCleanupHandler = NULL;
		
		// If backup or restore operation has started then unload strategy.
		iStratergyContainer->UnloadStratergy();
		// Destroy the container instance. This will close the open Database
		// file so that it can be backed up or restored.
		CLbtContainer::Destroy();
		iContainer = NULL;
		}
	else
		{
		// Backup and restore operation finished.
		
		// Load the Container First since it is required to determine whether
		// strategy should be loaded or not.
		iContainer = CLbtContainer::NewL();
		
		// Load all handlers that use container same instance
		iFireHandler = CLbtTriggerFireHandler::NewL(*iNotificationMap, *iContainer);
		
		iCleanupHandler = CLbtCleanupHandler::NewL( *iContainer );
		
		iSimChangeHandler = CLbtSimChangeHandler::NewL( *iContainer,*iCleanupHandler,*this );
		iSimChangeHandler->StartToListenL();
		
		iAppChangeHandler = CLbtAppChangeHandler::NewL(*iContainer, *iCleanupHandler,*this);
		iAppChangeHandler->StartListeningL();				
		
		// Load Strategy if there are triggers to be monitored.
		LoadOrUnloadStrategyPluginL();
		}
	}


// ---------------------------------------------------------------------------
// CLbtServerLogic::TriggerFiredL
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::TriggerFiredL( CLbtGeoAreaBase::TGeoAreaType aAreaType,
                                     TLbtTriggerId aId, const TPositionInfo& aPosInfo )
	{
	FUNC_ENTER("CLbtServerLogic::TriggerFiredL");
	/**
	 * Create the trigger fire handler. The trigger fire handler is an active 
	 * object and does the firing operation asynchronously since firing in
	 * this function will block stratergy's normal operation
	 */
	TLbtTriggerFireInfo triggerFireInfo;
	triggerFireInfo.iAreaType = aAreaType;
	triggerFireInfo.iTriggerId = aId;
	triggerFireInfo.iFiredPositionInfo = aPosInfo;
	iFireHandler->FireTriggerL(triggerFireInfo);
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::SetTriggeringSupervisionDynamicInfo
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::SetTriggeringSupervisionDynamicInfo( 
    const TLbtStrategySupervisionDynamicInfo& aStatus )
	{
	FUNC_ENTER("CLbtServerLogic::SetTriggeringSupervisionDynamicInfo");
    iStrategyDynamicInfo = aStatus;
    NotifyTriggeringSystemStatusChange();
    }

// ---------------------------------------------------------------------------
// CLbtServerLogic::GetTriggeringSupervisionSettings
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::GetTriggeringSupervisionSettings( TLbtStrategyTriggeringSupervisionSettings& aSettings )
	{
	FUNC_ENTER("CLbtServerLogic::GetTriggeringSupervisionSettings");
	aSettings.iGpsTrackingModeInterval = iSettingsManager->GetMinimumUpdateInterval();
	aSettings.iLocationRequestTimeout = iSettingsManager->GetLocationRequestTimeOut();
	aSettings.iMaximumUserSpeed = iSettingsManager->GetMaximumUserSpeed();
	aSettings.iMinimumLocationUpdateInterval = iSettingsManager->GetMinimumUpdateInterval();
	aSettings.iMinimumLocationUpdateIntervalWithoutGps = iSettingsManager->GetMinimumUpdateIntervalOnGpsFailure();
	aSettings.iPositioningTechnology = iSettingsManager->GetModuleId();	
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::ConstructL()
	{
	FUNC_ENTER("CLbtServerLogic::ConstructL");
	iBackupRestoreListener = CLbtBackupRestoreListener::NewL(*this);
	iBackupRestoreListener->StartToListenL();
	iContainer = CLbtContainer::NewL();
	iStratergyContainer = CLbtStratergyContainer::NewL();
	iSettingsManager = CLbtSettingsManager::NewL();
	iNotificationMap = CLbtNotificationMap::NewL();
	iFireHandler = CLbtTriggerFireHandler::NewL(*iNotificationMap, *iContainer);
	iStrategyDynamicInfo.iLocationAcquisitionStatus = ELocationAcquisitionNotReady;
	iCleanupHandler = CLbtCleanupHandler::NewL( *iContainer );
	iSimChangeHandler = CLbtSimChangeHandler::NewL( *iContainer,*iCleanupHandler,*this );
	iSimChangeHandler->StartToListenL();
	// App change handler to handle application uninstallation and MMC card removal
	iAppChangeHandler = CLbtAppChangeHandler::NewL(*iContainer, *iCleanupHandler,*this);
	iAppChangeHandler->StartListeningL();
	LoadOrUnloadStrategyPluginL();
	User::LeaveIfError( iProperty.Attach(KPSUidLbtStatusInformation, 
                                         KLbtLocationTriggeringSupervisionStatus,
	                                     EOwnerThread) );
	
	if( iSettingsManager->GetTriggeringMechanismState() == ETriggeringMechanismOff )
        {
        // Change the status information to supervision not active
        iProperty.Set(KPSUidLbtStatusInformation, 
                      KLbtLocationTriggeringSupervisionStatus, 
                      ELbtLocationTriggeringSupervisionOff);
        }
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::CancelAllOperations
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::CancelAllOperations()
	{
	FUNC_ENTER("CLbtServerLogic::CancelAllOperations");
	
	if(iNotificationMap)
		{
		// Inform all outstanding IPC messages that the server is busy with some other operation
		iNotificationMap->Reset(KErrServerBusy);
		}	
	
	// Destroy all the AOO operation objects
	iAOOArray.ResetAndDestroy();		
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::RemoveFromNotificationMap
// ---------------------------------------------------------------------------
//
TInt CLbtServerLogic::RemoveFromNotificationMap(const RMessage2& aMessage, TInt aServiceId)
	{
	FUNC_ENTER("CLbtServerLogic::RemoveFromNotificationMap");
	RMessage2 msg;
	TInt retVal = iNotificationMap->Retreive(msg,
											 aMessage.Int3(), 
											 aMessage.SecureId(), 
											 aServiceId, 
											 aMessage.Session());
	if(retVal == KErrNone)
		{
		LbtGlobal::RequestComplete(msg, KErrCancel);
		}
	return retVal;
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::InsertIntoNotificationMapL
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::InsertIntoNotificationMapL(
    const RMessage2& aMessage,
    TSubSessionType aType)	
	{
	FUNC_ENTER("CLbtServerLogic::InsertIntoNotificationMapL");
	// Valid request. Insert in sorted order into to the message array	
	iNotificationMap->InsertL(aMessage, aType);
	}
	
// ---------------------------------------------------------------------------
// CLbtServerLogic::ValidateChangeNotification
// ---------------------------------------------------------------------------
//
TInt CLbtServerLogic::ValidateChangeNotification(const RMessage2& aMessage)
	{
	FUNC_ENTER("CLbtServerLogic::ValidateChangeNotification");
	if( !aMessage.HasCapability(ECapabilityLocation) ||
	    !aMessage.HasCapability(ECapabilityReadDeviceData)||
	    !aMessage.HasCapability(ECapabilityReadUserData))
		{
		return KErrPermissionDenied;
		}

	return KErrNone;
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::ValidateGeneralOperation
// ---------------------------------------------------------------------------
//
TInt CLbtServerLogic::ValidateGeneralOperation(const RMessage2& aMessage)
	{
	FUNC_ENTER("CLbtServerLogic::ValidateGeneralOperation");
	if( !aMessage.HasCapability(ECapabilityLocation))
		{
		return KErrPermissionDenied;
		}

	return KErrNone;
	}
	
// ---------------------------------------------------------------------------
// CLbtServerLogic::ValidateCreateTriggerMessage
// ---------------------------------------------------------------------------
//
TInt CLbtServerLogic::ValidateCreateTriggerMessage(const RMessage2& aMessage)
	{
	FUNC_ENTER("CLbtServerLogic::ValidateCreateTriggerMessage");
	// The server should check for the following parameters
	// 1. capability checks for security.
		if( !aMessage.HasCapability(ECapabilityLocation) ||
	    !aMessage.HasCapability(ECapabilityWriteUserData) )
		{
		return KErrPermissionDenied;
		}

	return KErrNone;
	}	
// ---------------------------------------------------------------------------
// CLbtServerLogic::ValidateGetTriggeringSysSetting
// ---------------------------------------------------------------------------
//
TInt CLbtServerLogic::ValidateGetTriggeringSysSetting(const RMessage2& aMessage)
	{
	FUNC_ENTER("CLbtServerLogic::ValidateGetTriggeringSysSetting");
	if( !aMessage.HasCapability(ECapabilityLocation)||
		!aMessage.HasCapability(ECapabilityReadDeviceData))
		{
		return KErrPermissionDenied;
		}

	return KErrNone;
	}
	
// ---------------------------------------------------------------------------
// CLbtServerLogic::ValidateSetTriggeringSetting
// ---------------------------------------------------------------------------
//
TInt CLbtServerLogic::ValidateSetTriggeringSetting(const RMessage2& aMessage)
	{
	FUNC_ENTER("CLbtServerLogic::ValidateSetTriggeringSetting");
	if( !aMessage.HasCapability(ECapabilityLocation)||
		!aMessage.HasCapability(ECapabilityWriteDeviceData))
		{
		return KErrPermissionDenied;
		}

	return KErrNone;
	}		
	
	
// ---------------------------------------------------------------------------
// CLbtServerLogic::ValidateGetTriggeringSysStatus	
// ---------------------------------------------------------------------------
//
TInt CLbtServerLogic::ValidateGetTriggeringSysStatus(const RMessage2& aMessage)
	{
	FUNC_ENTER("CLbtServerLogic::ValidateGetTriggeringSysStatus");
	if( !aMessage.HasCapability(ECapabilityLocation)||
		!aMessage.HasCapability(ECapabilityReadDeviceData))
		{
		return KErrPermissionDenied;
		}

	return KErrNone;
	}
	


// ---------------------------------------------------------------------------
// CLbtServerLogic::IsNotificationRequest
// ---------------------------------------------------------------------------
//
TBool CLbtServerLogic::IsNotificationRequest(TInt aServiceId)
	{
	FUNC_ENTER("CLbtServerLogic::IsNotificationRequest");
	if(aServiceId >= ELbtNotifyTriggerFired &&
	   aServiceId <= ELbtNotifyTriggeringSysSettingChange)
		{
		return ETrue;
		}
	return EFalse;
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::HandleNotificationOperationsL
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::HandleNotificationOperationsL(
    CLbtAOOperationBase* aOperation, 
    TInt aServiceId)
	{
	FUNC_ENTER("CLbtServerLogic::HandleNotificationOperationsL");
	// This method handles all notifications. The IPC messsage has actually
	// been compleated and hence the client side buffers should NOT be accessed 
	// through this message object.
			
	// First get the set of registered messages for notification for notification map
	
	// 2 types of operations
	// 1. Client side -- Use the secureId in iMessage
	// 2. Management Side -- Use the ownerSid retrieved by list triggers.

	// For each secureId now retrieve all messages for the same secureId and serviceId.
	// Also retrieve all notif msgs from management API.
	// respond to the pending messages.
	
    if ( aOperation->GetFunction() == ELbtCreateTrigger ||
         aOperation->GetClientType() == CLbtServerLogicBase::TLbtClientLibrary )
        {
        HandleSingleNotificationOperationsL( aOperation, aServiceId );
        }
    else
        {
        HandleMultipleNotificationOperationsL( aOperation, aServiceId );
        }
	}

// ---------------------------------------------------------------------------
// CLbtServerLogic::HandleSingleNotificationOperationsL
// ---------------------------------------------------------------------------
//	
void CLbtServerLogic::HandleSingleNotificationOperationsL(
    CLbtAOOperationBase* aOperation, 
    TInt aServiceId)
    {
    FUNC_ENTER("CLbtServerLogic::HandleSingleNotificationOperationsL");
	RArray<RMessage2> array;
	RMessage2 message;
	
	RArray<TUid> managerUidArray =aOperation->GetManagerArray();
	for( TInt i=0;i<managerUidArray.Count();i++ )
	    {
	    TSecureId secureId(managerUidArray[i]);
    	// Retrieve all client session notification messages.
    	while(!iNotificationMap->Retreive(message, secureId, aServiceId) )
    		{
    		array.AppendL(message);
    		}
		}

    // Retrieve all management notification messages.
	while(!iNotificationMap->Retreive(message, 
	                                  aServiceId,
	                                  CLbtServerLogicBase::TLbtManagementLibrary))
		{
		array.AppendL(message);
		}

	if(array.Count() == 0)
		{
		array.Close();
		return;
		}
	
	// Populate the change event structure to send the information to the client based
	// on the message for which the notification needs to be made
	// coverity[var_decl : FALSE] 
	TLbtTriggerChangeEvent event;
	
	switch(aOperation->GetFunction())
		{
		case ELbtCreateTrigger:
			{
			event.iEventType = ELbtTriggerChangeEventCreated;
			// Get the trigger id from the create trigger AO object
			CLbtCreateTriggerAOOperation* operation = 
						static_cast<CLbtCreateTriggerAOOperation*>(aOperation);
        	event.iTriggerId = operation->GetTriggerId();
			break;
			}
		case ELbtSetTriggerState:
		case ELbtUpdateTrigger:
			{
			// Just one trigger has been updated
			event.iEventType = ELbtTriggerChangeEventUpdated;
			CLbtTriggerModifyAOOperation* operation = 
						static_cast<CLbtTriggerModifyAOOperation*>(aOperation);
			event.iTriggerId = operation->GetTriggerId();
			break;
			}		
		case ELbtDeleteTrigger:
            {
			event.iEventType = ELbtTriggerChangeEventDeleted;
			CLbtTriggerModifyAOOperation* operation = 
						static_cast<CLbtTriggerModifyAOOperation*>(aOperation);
			event.iTriggerId = operation->GetTriggerId();
            break;
            }
		case ELbtSetTriggersState:			
		case ELbtDeleteTriggers:
			{
			// Multiple triggers have been updated
			event.iEventType = ELbtTriggerChangeEventMultiple;
			break;
			}
		default:
			{
			break;
			}
		}
	
	// Respond with the change structure on all registered IPC's
	for(TInt i=0;i<array.Count();++i)
		{
		TPckgBuf<TLbtTriggerChangeEvent> changeEvent(event);
		if(LbtGlobal::Write(array[i], KParamChangeEvent, changeEvent) == KErrNone)
			{
			LbtGlobal::RequestComplete(array[i], KErrNone);
			}
		}
	
	array.Close();
    }
	
// ---------------------------------------------------------------------------
// CLbtServerLogic::HandleMultipleNotificationOperationsL
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::HandleMultipleNotificationOperationsL(
    CLbtAOOperationBase* aOperation, 
    TInt aServiceId)
    {
    FUNC_ENTER("CLbtServerLogic::HandleMultipleNotificationOperationsL");
	CLbtTriggerModifyAOOperation* operation = 
		static_cast<CLbtTriggerModifyAOOperation*>(aOperation);
		// list of triggers that were modified.
	RPointerArray<CLbtContainerTriggerEntry>& list = 
	    operation->GetUpdatedTriggerList();

	RArray<RMessage2> array;
	RMessage2 message;
	// coverity[var_decl : FALSE] 
	TLbtTriggerChangeEvent event;
	
    // sort by SID the retrieved list of triggers that were modified.
    TLinearOrder<CLbtContainerTriggerEntry> TriggerSidOrderPredicate(OrderTriggerEntryBySid);
    list.Sort(TriggerSidOrderPredicate);
    
    TInt multipleOps = 0;
    TInt totalTriggersModified = list.Count();
        
    for( TInt i = totalTriggersModified - 1; i >= 0 ; --i  )
        {
        const TLbtTriggerId& id = list[i]->TriggerEntry()->Id();
        const TSecureId& sid = list[i]->ExtendedTriggerInfo()->OwnerSid();
        // Using ternary operator here to check if i is zero becoz then list[i -1] will
        // be crossing the array boundary.
        const TSecureId& prevSid = i ? list[i - 1]->ExtendedTriggerInfo()->OwnerSid() : TSecureId(0);
        
        if ( sid == prevSid )
            {
            // The modification is on the same client as the previous one.
            // Rather than sending the notification collate them into single event.
            multipleOps++;
            continue;
            }
    	else
    	    {
    	    // The next trigger in list belongs to a different client.
    	    // So reply back on the notification now.
        	// retrieve all notification messages of this client.
        	
        	while( !iNotificationMap->Retreive(message, sid, aServiceId) )
        		{
        		array.AppendL(message);
        		}

            SetNotificationEventType(aOperation, event);
            
            if ( multipleOps )
                {
    			event.iEventType = ELbtTriggerChangeEventMultiple;
                }

        	event.iTriggerId = id;
        	// Respond with the change structure on all registered IPC's
        	for(TInt j = array.Count(); j ; --j)
        		{
        		TPckgBuf<TLbtTriggerChangeEvent> changeEvent(event);
        		if(LbtGlobal::Write(array[j - 1], KParamChangeEvent, changeEvent) == KErrNone)
        			{
        			LbtGlobal::RequestComplete(array[j - 1], KErrNone);
        			}
        		}
        	array.Close();

    	    multipleOps = 0;
    	    }
        }

    // Retrieve all management Notification messages
    // Retrieve all management notification messages.
	while(!iNotificationMap->Retreive(message,
	                                  aServiceId,
	                                  CLbtServerLogicBase::TLbtManagementLibrary))
		{
		array.Append(message);
		}

    if ( totalTriggersModified > 1 )
        {
        event.iEventType = ELbtTriggerChangeEventMultiple;
        event.iTriggerId = 0;
        }
        
	// Respond with the change structure on all registered IPC's
	for(TInt j = array.Count(); j ; --j)
		{
		TPckgBuf<TLbtTriggerChangeEvent> changeEvent(event);
		if(LbtGlobal::Write(array[j - 1], KParamChangeEvent, changeEvent) == KErrNone)
			{
			LbtGlobal::RequestComplete(array[j - 1], KErrNone);
			}
		}
	array.Close();
    }



void CLbtServerLogic::SetNotificationEventType( 
    CLbtAOOperationBase* aOperation, 
    TLbtTriggerChangeEvent& aEvent )
    {
    FUNC_ENTER("CLbtServerLogic::SetNotificationEventType");
	switch(aOperation->GetFunction())
		{
		case ELbtCreateTrigger:
			{
			aEvent.iEventType = ELbtTriggerChangeEventCreated;
			break;
			}
		case ELbtSetTriggerState:
		case ELbtUpdateTrigger:
			{
			// Just one trigger has been updated
			aEvent.iEventType = ELbtTriggerChangeEventUpdated;
			break;
			}		
		case ELbtDeleteTrigger:
            {
			aEvent.iEventType = ELbtTriggerChangeEventDeleted;
            break;
            }
		case ELbtSetTriggersState:			
		case ELbtDeleteTriggers:
		default:
			{
			// Multiple triggers have been updated
			aEvent.iEventType = ELbtTriggerChangeEventMultiple;
			break;
			}
		}
    }

TInt CLbtServerLogic::GetTriggeringSystemSettings(
    const RMessage2& aMessage, 
    TSubSessionType aClientType )
    {
    FUNC_ENTER("CLbtServerLogic::GetTriggeringSystemSettings");
	// The request can be either through LT client API or LT management API.
	// and the handling has to be slightly different depending on the client type.
    switch ( aClientType )
        {
        case TLbtManagementLibrary:
    	    {
    	    // This is the req from the LT client API, Hence return only
    	    // the min trigger area and triggering engine state ( On/Off ).
    	    TLbtTriggeringSystemManagementSettings settings;

    		settings.SetMinimumTriggerAreaSize(
    		    iSettingsManager->MinimumTriggeringArea());

    		TLbtTriggeringMechanismState state = 
    			static_cast<TLbtTriggeringMechanismState>(
			    iSettingsManager->GetTriggeringMechanismState());
    		settings.SetTriggeringMechanismState(state);

    		settings.SetMinimumLocationUpdateInterval( 
    		    iSettingsManager->GetMinimumUpdateInterval() );

            settings.SetMinimumLocationUpdateIntervalWhenGpsFails( 
                iSettingsManager->GetMinimumUpdateIntervalOnGpsFailure() );

            settings.SetUsedPositioningModule( 
                iSettingsManager->GetModuleId() );
            
            settings.SetMaximumUserSpeed( 
                iSettingsManager->GetMaximumUserSpeed() );

    		TPckg<TLbtTriggeringSystemManagementSettings> pckgSettings(settings);
    		return LbtGlobal::Write(aMessage, KParamSettings, pckgSettings);
    	    }
    	    

        case TLbtClientLibrary:
    	    {
    	    // This is the req from the LT client API, Hence return only
    	    // the min trigger area and triggering engine state ( On/Off ).
    		TLbtTriggeringSystemSettings settings;

    		settings.SetMinimumTriggerAreaSize(
    		    iSettingsManager->MinimumTriggeringArea() );

    		TLbtTriggeringMechanismState state = 
				static_cast<TLbtTriggeringMechanismState>(
				iSettingsManager->GetTriggeringMechanismState());

    		settings.SetTriggeringMechanismState(state);

    		TPckg<TLbtTriggeringSystemSettings> pckgSettings(settings);
    		return LbtGlobal::Write(aMessage, KParamSettings, pckgSettings);
    	    }
    	default :
            // This is a bug in server implementation. There is no other
            // session type as of now. Hence treating this as a LT API
            // client.
            
            // Note : Fall through is intended
            break;
    	    
        }
    return KErrNotSupported;
    }

void CLbtServerLogic::GetTriggeringSysStatus( TLbtTriggeringSystemStatus& aSystemStatus )
    {
    FUNC_ENTER("CLbtServerLogic::GetTriggeringSysStatus");
    aSystemStatus.iLocationAcquisitionStatus = iStrategyDynamicInfo.iLocationAcquisitionStatus;
    
    aSystemStatus.iPositioningMethod = iSettingsManager->GetModuleId();
    
    //aSystemStatus.iPositioningMethodName
    
    TPosition position;
    iStrategyDynamicInfo.iLatestAcquiredPosInfo.GetPosition( position );
    aSystemStatus.iLatestLocationUpdate = position.Time();
    }


void CLbtServerLogic::SetTriggeringSystemSettings(
    TLbtTriggeringSystemManagementSettings& aSettings,
    TLbtManagementSettingsMask aSettingsMask )
    {
    FUNC_ENTER("CLbtServerLogic::SetTriggeringSystemSettings");
    if ( aSettingsMask & EMinimumLocationUpdateInterval )
        {
        iSettingsManager->SetMinimumUpdateInterval( aSettings.MinimumLocationUpdateInterval() );
        }
    if ( aSettingsMask & EMinimumLocationUpdateIntervalWhenGpsFails )
        {
        iSettingsManager->SetMinimumUpdateIntervalOnGpsFailure( aSettings.MinimumLocationUpdateIntervalWhenGpsFails() );
        }
    if ( aSettingsMask & EUsedPositioningmodule )
        {
        iSettingsManager->SetModuleId( aSettings.UsedPositioningModule() );
        }
    if ( aSettingsMask & EMaximumUserSpeed )
        {
        iSettingsManager->SetMaximumUserSpeed( aSettings.MaximumUserSpeed() );
        }
    if ( aSettingsMask & ETriggeringMechanismState )
        {
        iSettingsManager->SetTriggeringMechanismState( aSettings.TriggeringMechanismState() );
        }
    
    
    if( aSettings.TriggeringMechanismState() == ETriggeringMechanismOff )
        {
        if( iStratergyContainer->Stratergy() )
            {
            iStratergyContainer->UnloadStratergy();
            }
        iStrategyDynamicInfo.iLocationAcquisitionStatus = ELocationAcquisitionOff;
        
        // Change the status information to supervision off
        iProperty.Set(KPSUidLbtStatusInformation, 
                                         KLbtLocationTriggeringSupervisionStatus, 
                                         ELbtLocationTriggeringSupervisionOff);
                                         
        NotifyTriggeringSystemStatusChange();
        }
    else
        {
        if(!iStratergyContainer->Stratergy() && 
		   iContainer->GetCountOfEnabledAndValidTriggers() && 
		   !iBackupRestoreListener->IsBackupRestoreOperationInProgress())
		    {
            TRAPD(error,iStratergyContainer->LoadStratergyL(this));
		    if( KErrNone == error )
		        {
		        iStratergyContainer->Stratergy()->StartSupervision();
		        iStrategyDynamicInfo.iLocationAcquisitionStatus = ELocationAcquisitionActive;
		        NotifyTriggeringSystemStatusChange();
		        }
		    }
        else
            {
            // Change the status information to supervision off
            iProperty.Set(KPSUidLbtStatusInformation, 
                      KLbtLocationTriggeringSupervisionStatus, 
                      ELbtLocationTriggeringSupervisionNotActive);
            }
        }    
        
    // ToDo : the management header does not have this optimization level. Hence fix in the management lib
    // iSettingsManager->SetOptimizationLevel(   );

    // Inform Strategy that the Triggering Engine Settings have changed.
    // These settings values might affect the behaviour of the supervision algorithm.
    CLbtStrategyBase* strategy = iStratergyContainer->Stratergy();
    if ( strategy )
        {
        strategy->TriggeringSupervisionSettingsChanged();
        }
        
    //ToDo : we also need to notify clients that the triggering settings has changed.
    NotifyTriggeringSystemSettingChange(aSettingsMask);   
    }

void CLbtServerLogic::NotifyTriggeringSystemSettingChange(TLbtManagementSettingsMask aSettingsMask )
	{
	FUNC_ENTER("CLbtServerLogic::NotifyTriggeringSystemSettingChange");
	RArray<RMessage2> messageArray;
	RMessage2 message;
	if((aSettingsMask & ETriggeringMechanismState))
		{
		while( !iNotificationMap->Retreive(message,ELbtNotifyTriggeringSysSettingChange,
										CLbtServerLogicBase::TLbtClientLibrary))
			{
			TInt error = messageArray.Append( message );
			if( error != KErrNone )
			    {
                LOG1("Failed to append message to the array:%d",error);
                break;
			    }
			}
		if(messageArray.Count())
			{
			TLbtTriggeringSystemSettings triggeringSystemSettings;
			if(iSettingsManager->GetTriggeringMechanismState()==1)	
				{
				triggeringSystemSettings.SetTriggeringMechanismState(ETriggeringMechanismOn);
				}
			else
				{
				triggeringSystemSettings.SetTriggeringMechanismState(ETriggeringMechanismOff);
				}	
			triggeringSystemSettings.SetMinimumTriggerAreaSize(
								iSettingsManager->MinimumTriggeringArea());	
			TPckg<TLbtTriggeringSystemSettings> pckSystemSettings(triggeringSystemSettings);
			for(TInt i=messageArray.Count();i>0;i--)
				{
				LbtGlobal::Write(messageArray[i-1],0,pckSystemSettings);
				LbtGlobal::RequestComplete(messageArray[i-1],KErrNone);
				}
			}
		}
	messageArray.Reset();
	while( !iNotificationMap->Retreive(message,ELbtNotifyTriggeringSysSettingChange,
									CLbtServerLogicBase::TLbtManagementLibrary))
		{
		messageArray.Append( message );
		}
	if(messageArray.Count())
		{
		TLbtTriggeringSystemManagementSettings triggeringSystemManagementSettings;
		if(iSettingsManager->GetTriggeringMechanismState()==1)	
			{
			triggeringSystemManagementSettings.SetTriggeringMechanismState(ETriggeringMechanismOn);
			}
		else
			{
			triggeringSystemManagementSettings.SetTriggeringMechanismState(ETriggeringMechanismOff);
			}	
		triggeringSystemManagementSettings.SetMinimumTriggerAreaSize(
							iSettingsManager->MinimumTriggeringArea());	
		triggeringSystemManagementSettings.SetMinimumLocationUpdateInterval(
							iSettingsManager->GetMinimumUpdateInterval());	
		triggeringSystemManagementSettings.SetMinimumLocationUpdateIntervalWhenGpsFails(
							iSettingsManager->GetMinimumUpdateIntervalOnGpsFailure());
		triggeringSystemManagementSettings.SetUsedPositioningModule(
							iSettingsManager->GetModuleId());
		triggeringSystemManagementSettings.SetMaximumUserSpeed(
							iSettingsManager->GetMaximumUserSpeed());															
		TPckg<TLbtTriggeringSystemManagementSettings> pckSystemManagementSettings(
										triggeringSystemManagementSettings);
		for(TInt i=messageArray.Count();i>0;i--)
			{
			LbtGlobal::Write(messageArray[i-1],0,pckSystemManagementSettings);
			LbtGlobal::RequestComplete(messageArray[i-1],KErrNone);
			}
		}
	messageArray.Close();	
	}
	
void CLbtServerLogic::NotifyTriggeringSystemStatusChange()
    {
    FUNC_ENTER("CLbtServerLogic::NotifyTriggeringSystemStatusChange");
    RArray<RMessage2> messageArray;
	RMessage2 message;
	
	while( !iNotificationMap->Retreive(message,ELbtNotifyTriggeringSysStatusChange,
										CLbtServerLogicBase::TLbtClientLibrary))
			{
			TInt error = messageArray.Append( message );
			if( error != KErrNone )
			    {
                LOG1("Failed to append to the array:%d",error);
                break;
			    }
			}
	if( messageArray.Count() )		
	    {
	    for(TInt i=messageArray.Count();i>0;i--)
			{
			LbtGlobal::RequestComplete(messageArray[i-1],KErrNotSupported);
			}
	    messageArray.Reset();
	    }
	
	while( !iNotificationMap->Retreive(message,ELbtNotifyTriggeringSysStatusChange,
										CLbtServerLogicBase::TLbtManagementLibrary))
			{
			TInt error = messageArray.Append( message );
			if( error != KErrNone )
                {
                LOG1("Failed to append to the array:%d",error);
                break;
                }
			}    
    if( messageArray.Count() )
        {
        for(TInt i=messageArray.Count();i>0;i--)
			{
			LbtGlobal::RequestComplete(messageArray[i-1],KErrNone);
			}
        }
    messageArray.Close();    
    }

// ---------------------------------------------------------------------------
// CLbtServerLogic::HandleTriggersChange
// ---------------------------------------------------------------------------
//
void CLbtServerLogic::HandleTriggersChange( RArray<TLbtTriggerId>& aTriggerIds,
                                            RArray<TUid>& aManagerUids,
                                            TLbtTriggerEventMask aEventMask )
    {
    FUNC_ENTER("CLbtServerLogic::HandleContainerTriggerChange");
    RMessage2 message;
    
    // coverity[var_decl : FALSE] 
    TLbtTriggerChangeEvent event;
    // If there is change in the single trigger, set the trigger id else
    // update KLbtNullTriggerId for trigger id
    if( aTriggerIds.Count() == 1)
        {
        event.iTriggerId = aTriggerIds[0];
        }
    else
        {
        event.iTriggerId = KLbtNullTriggerId;
        }
    
    // Check for type of notification
    if( aEventMask & CLbtContainer::ELbtConTriggerDeleted &&
        aEventMask & CLbtContainer::ELbtConTriggerValidityFieldChanged )
        {
        event.iEventType = ELbtTriggerChangeEventMultiple;
        }
    else if( aEventMask & CLbtContainer::ELbtConTriggerDeleted )
        {
        event.iEventType = ELbtTriggerChangeEventDeleted;
        }
    else if ( aEventMask & CLbtContainer::ELbtConTriggerValidityFieldChanged )
        {
        event.iEventType = ELbtTriggerChangeEventUpdated;
        }
    TPckgBuf<TLbtTriggerChangeEvent> changeEvent(event);

    for( TInt i=0;i<aManagerUids.Count();i++ )
        {
        TSecureId secureId(aManagerUids[i]);
        // Retrieve all client session trigger change notification messages.
        while(!iNotificationMap->Retreive(message, secureId, ELbtNotifyTriggerChangeEvent) )
            {
            if(LbtGlobal::Write(message, KParamChangeEvent, changeEvent) == KErrNone)
                {
                LbtGlobal::RequestComplete(message, KErrNone);
                }
            }
        }
    
    // Retrieve all management library notification messages
    while( !iNotificationMap->Retreive( message,ELbtNotifyTriggerChangeEvent,
                                   CLbtServerLogicBase::TLbtManagementLibrary ) )
           {
           if(LbtGlobal::Write(message, KParamChangeEvent, changeEvent) == KErrNone)
               {
               LbtGlobal::RequestComplete(message, KErrNone);
               }
           }
    
    
    TRAP_IGNORE(LoadOrUnloadStrategyPluginL());
    }
// end of file
