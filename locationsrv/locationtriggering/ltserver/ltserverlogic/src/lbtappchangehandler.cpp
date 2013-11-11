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
* Description:  This file defines the app removal handler
*
*/


// INCLUDE FILES
#include <apgcli.h>
#include <apaid.h>
#include <apgnotif.h>
#include <driveinfo.h>

#include "lbttriggerfilterbyattribute.h"
#include "lbtappchangehandler.h"
#include "lbtcontainertriggerfilter.h"
#include "lbtcontainerupdatefilter.h"
#include "lbtcontainer.h"
#include "lbtcleanuphandler.h"
#include "lbtserverlogictriggerchangeobserver.h"
#include "lbtlogger.h"

// CONSTANTS
_LIT(KMMCErrorChar, "<"); //if MMC card char retrieval failed, it is assigned.
_LIT(KCharColon, ":"); 

// LOCAL ENUMS

enum
	{
	EOpNone,
	EOpRegiseteredForInitialListPoplulation,
	EOpRegiseteredForListPoplulation,
	EOpRemovalTriggerDeletion,
	EOpDeletionStartupProcessTriggers,
	EOpRemovalListingStartupProcessTriggers,
	EOpRemovalDeletingStartupProcessTriggers,
	EOpRemovalTriggerInvalidation,
	EOpMMCRemovalListingTriggers,
	EOpMMCRemovalListingTriggerForStartupProcess,
	EOpMMCInsertionListingTriggers,
	EOpMMCInsertionListingTriggerForStartupProcess,
	EOpRequestValidityChange	
	};


// LOCAL STRUCTURES
struct TLbtAppInfo
	{
	TFileName iFullName;
	TUid iUid;
	};

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtAppRemovalHandler::NewL
// ---------------------------------------------------------------------------
//
CLbtAppChangeHandler* CLbtAppChangeHandler::NewL(CLbtContainer& aContainer, CLbtCleanupHandler& aCleanupHandler,
                                                 MLbtServerLogicTriggerChangeObserver& aTriggerChangeObserver)
	{
	FUNC_ENTER("CLbtAppChangeHandler::NewL");
	CLbtAppChangeHandler* self = new (ELeave) CLbtAppChangeHandler(aContainer,aCleanupHandler,
	                                                               aTriggerChangeObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
	CleanupStack::Pop( self );  
	return self;
	}

// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::ConstructL
// ---------------------------------------------------------------------------
//
void CLbtAppChangeHandler::ConstructL()
	{
	FUNC_ENTER("CLbtAppChangeHandler::ConstructL");
	User::LeaveIfError(iAppArcSession.Connect());
	MemoryCardChar();
	}

// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::StartListeningL
// ---------------------------------------------------------------------------
//
void CLbtAppChangeHandler::StartListeningL()
	{
	FUNC_ENTER("CLbtAppChangeHandler::StartListeningL");
	if(!iAppListNotifier)
		{
		iAppListNotifier = CApaAppListNotifier::NewL(this, CActive::EPriorityStandard);
		
		iAppArcSession.RegisterListPopulationCompleteObserver(iStatus);
		iOperation = EOpRegiseteredForInitialListPoplulation;
		SetActive();
		}
	}

// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::FindElementInArray
// ---------------------------------------------------------------------------
//
TInt CLbtAppChangeHandler::FindElementInArray(RArray<TLbtAppInfo>& aArray, TLbtAppInfo& aElement)
	{
	FUNC_ENTER("CLbtAppChangeHandler::FindElementInArray");
	// Parse the array to find out if the element is present
	for(TInt i=0;i<aArray.Count();++i)
		{
		if(aArray[i].iUid == aElement.iUid)
			{
			return i;
			}
		}
	return KErrNotFound;
	}

// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::HandleAppListRemovalEvent
// ---------------------------------------------------------------------------
//
void CLbtAppChangeHandler::HandleAppListRemovalEvent(RArray<TLbtAppInfo>& aNewAppList)
	{
	FUNC_ENTER("CLbtAppChangeHandler::HandleAppListRemovalEvent");
	
	RArray<TUid> appRemovalList;
	TBool appsInMMC = EFalse;
	
	/// Check which apps is un-installed/removed
	for(TInt i=0;i<iCurrentInstalledApps.Count();++i)
		{
		TInt index = FindElementInArray(aNewAppList, iCurrentInstalledApps[i]);
		
		if(KErrNotFound == index)
			{
			// This application has be uninstalled/removed
			TLbtAppInfo appInfo = iCurrentInstalledApps[i];
			
			// The removed application resided in the MMC card
			TInt error = iAppList.Append( appInfo );
			if( error != KErrNone )
			    {
                LOG1("Failed to append appInfo:%d",error);
                return;
			    }
						
			// Check if the application resides in MMC
			if( appInfo.iFullName.FindC(iMMCDriveChar) >= 0 )
				{
				appsInMMC = ETrue;				
				}
			}
		}
	
	if( iAppList.Count() == 0 )
		{
		iAppList.Reset();
		LOG("No application found removed");
		return;
		}
	
	TInt error = KErrNone;
	if( appsInMMC )
		{
		TRAP(error, HandleMMCApplicationRemovalL() );
		}
	else
		{
		TRAP(error, HandleApplicationUninstallationL());
		}
	
	if( error != KErrNone )
		{
		iOperation = EOpNone;
		}
	}

// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::HandleAppListAdditionEvent
// ---------------------------------------------------------------------------
//
void CLbtAppChangeHandler::HandleAppListAdditionEvent(RArray<TLbtAppInfo>& aNewAppList)
	{
	FUNC_ENTER("CLbtAppChangeHandler::HandleAppListAdditionEvent");
	// Check which all app(s) have been added back
	for(TInt i=0;i<aNewAppList.Count();++i)
		{
		TInt index = FindElementInArray(iCurrentInstalledApps, aNewAppList[i]);		
		
		if(KErrNotFound == index)
			{
			// This application has been added
			TLbtAppInfo appInfo = aNewAppList[i];
			
			if( appInfo.iFullName.FindC( iMMCDriveChar ) >= 0 )
				{				
				// Append to the array only if the application resides on the MMC
				TInt error = iAppList.Append( appInfo );
				if( error != KErrNone )
				    {
                    LOG1("Failed to append appInfo to the array:%d",error);
                    return;
				    }
				}
			}
		}
	if(iAppList.Count() > 0)
		{
		TRAPD(error, SetValidTriggersOfApplicationsL());
		
		if(error != KErrNone)
			{
			ERROR("SetValidTriggersOfApplicationsL error = %d", error);
			iAppList.Close();
			iOperation = EOpNone;
			}
		}
	}
	
// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::SetValidTriggersOfApplicationsL
// ---------------------------------------------------------------------------
//
void CLbtAppChangeHandler::SetValidTriggersOfApplicationsL()
	{
	FUNC_ENTER("CLbtAppChangeHandler::SetValidTriggersOfApplicationsL");
	// First do a list triggers to get the list of triggers owned by that application
	iOperation = EOpMMCInsertionListingTriggers;
	
	CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
	CleanupStack::PushL(listOptions);
	
	CLbtTriggerFilterByAttribute* attributeFilter = 
	                                CLbtTriggerFilterByAttribute::NewL();
	CleanupStack::PushL(attributeFilter);
	attributeFilter->AddTriggerTypeL(CLbtTriggerEntry::ETypeStartup);
	
	listOptions->SetFilter(attributeFilter);
			
	// Retreive only the trigger id of the trigger
	TLbtTriggerAttributeFieldsMask mask = CLbtTriggerEntry::EAttributeId;
	listOptions->SetRetrievedFields(mask, 0);
	
	CLbtContainerFilter* containerFilter = CLbtContainerFilter::NewL();
	CleanupStack::PushL(containerFilter);
	
	for(TInt i=0;i<iAppList.Count();++i)
		{
		containerFilter->AddOwnerSidInFilterL(iAppList[i].iUid); 
		}
	
	CLbtContainerListOptions* containerListOptions = CLbtContainerListOptions::NewL(listOptions, containerFilter);
	CleanupStack::PushL(containerListOptions);

	CleanupStack::Pop(4); // containerListOptions, containerFilter, attributeFilter & listOptions
			
	iContainer.ListTriggers(containerListOptions, iTriggerEntries, iOpId, iStatus);
	SetActive();
	}
	
// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::HandleAppListEvent
// ---------------------------------------------------------------------------
//
void CLbtAppChangeHandler::HandleAppListEvent(TInt /*aEvent*/)
	{
	FUNC_ENTER("CLbtAppChangeHandler::HandleAppListEvent");
	if( iOperation == EOpNone && !IsActive() )
		{
		iOperation = EOpRegiseteredForListPoplulation;
		iAppArcSession.RegisterListPopulationCompleteObserver(iStatus);
		SetActive();
		return;
		}
	iOperation = EOpNone;
	RArray<TLbtAppInfo> newAppList;
	
	// First get the set of installed applications in the system
	iAppArcSession.GetAllApps();
	
	TApaAppInfo appInfo;
	TInt error = iAppArcSession.GetNextApp(appInfo);
	
	while(KErrNone == error)
		{
		TLbtAppInfo lbtAppInfo;
        lbtAppInfo.iUid = appInfo.iUid;
        lbtAppInfo.iFullName.Zero();
        lbtAppInfo.iFullName.Copy( appInfo.iFullName );

		TRAPD(err, newAppList.AppendL(lbtAppInfo));
		if(err != KErrNone)
			{
			break;
			}
		error = iAppArcSession.GetNextApp(appInfo);
		}
	
	LOG1("newAppList.Count():%d",newAppList.Count());
	LOG1("iCurrentInstalledApps.Count():%d",iCurrentInstalledApps.Count());
	
	if(newAppList.Count() > iCurrentInstalledApps.Count())
		{
		HandleAppListAdditionEvent(newAppList);
		}
	else
		{
		LOG("newAppList.Count() < iCurrentInstalledApps.Count()");
		HandleAppListRemovalEvent(newAppList);
		}
		
	iCurrentInstalledApps.Reset();
	
	for( TInt i=0;i<newAppList.Count();i++ )
	    {
	    TInt error = iCurrentInstalledApps.Append( newAppList[i] );
	    if( error != KErrNone )
	        {
            LOG1("Failed to append new applist to the array:%d",error);
            break;
	        }
	    }
	newAppList.Close();
	}

// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::StopListening
// ---------------------------------------------------------------------------
//
void CLbtAppChangeHandler::StopListening()
	{
	FUNC_ENTER("CLbtAppChangeHandler::StopListening");
	delete iAppListNotifier;
	iAppListNotifier = NULL;
	iCurrentInstalledApps.Close();
	}

// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::HandleMMCApplicationRemovalL
// ---------------------------------------------------------------------------
//
void CLbtAppChangeHandler::HandleMMCApplicationRemovalL()
	{
	FUNC_ENTER("CLbtAppChangeHandler::HandleMMCApplicationRemovalL");
	
	if( IsMemoryCardPresent() )
		{
		// If memory card is present it means to say that the application
		// has been un-installed
		HandleApplicationUninstallationL( );
		}
	else 
		{
		// MMC card has been removed. Hence invalidate all the triggers that 
		// belong to the application.
		
		// First do a list triggers to get the list of triggers owned by that application
		iOperation = EOpMMCRemovalListingTriggers;
		
		CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
		CleanupStack::PushL(listOptions);
		
		CLbtTriggerFilterByAttribute* attributeFilter = 
		                                CLbtTriggerFilterByAttribute::NewL();
		CleanupStack::PushL(attributeFilter);
		attributeFilter->AddTriggerTypeL(CLbtTriggerEntry::ETypeStartup);
		
		listOptions->SetFilter(attributeFilter);
				
		// Retreive only the trigger id of the trigger
		TLbtTriggerAttributeFieldsMask mask = CLbtTriggerEntry::EAttributeId;
		listOptions->SetRetrievedFields(mask, 0);
		
		CLbtContainerFilter* containerFilter = CLbtContainerFilter::NewL();
		CleanupStack::PushL(containerFilter);
		
		for(TInt i=0;i<iAppList.Count();++i)
			{
			containerFilter->AddOwnerSidInFilterL(iAppList[i].iUid); 
			}
		
		CLbtContainerListOptions* containerListOptions = CLbtContainerListOptions::NewL(listOptions, containerFilter);
		CleanupStack::PushL(containerListOptions);

		CleanupStack::Pop(4); // containerListOptions, containerFilter, attributeFilter & listOptions
				
		iContainer.ListTriggers(containerListOptions, iTriggerEntries, iOpId, iStatus);
		SetActive();		
		}
	}

// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::ListTriggersOfStartupProcessL
// ---------------------------------------------------------------------------
//
void CLbtAppChangeHandler::ListTriggersOfStartupProcessL()
	{
	FUNC_ENTER("CLbtAppChangeHandler::ListTriggersOfStartupProcessL");
	CLbtListTriggerOptions* listOptions = CLbtListTriggerOptions::NewL();
	CleanupStack::PushL(listOptions);
	
	CLbtTriggerFilterByAttribute* attributeFilter = 
	                                CLbtTriggerFilterByAttribute::NewL();
	CleanupStack::PushL(attributeFilter);
	attributeFilter->AddTriggerTypeL(CLbtTriggerEntry::ETypeStartup);
	
	listOptions->SetFilter(attributeFilter);
			
	// Retreive only the trigger id of the trigger
	TLbtTriggerAttributeFieldsMask mask = CLbtTriggerEntry::EAttributeId;
	listOptions->SetRetrievedFields(mask, 0);
	
	CLbtContainerFilter* containerFilter = CLbtContainerFilter::NewL();
	CleanupStack::PushL(containerFilter);
	
	for(TInt i=0;i<iAppList.Count();++i)
		{
		containerFilter->AddStartupProcessInFilterL( iAppList[i].iFullName );
		}
	
	CLbtContainerListOptions* containerListOptions = CLbtContainerListOptions::NewL(listOptions, containerFilter);
	CleanupStack::PushL(containerListOptions);

	CleanupStack::Pop(4); // containerListOptions, containerFilter, attributeFilter & listOptions
			
	iContainer.ListTriggers(containerListOptions, iTriggerEntries, iOpId, iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::DeleteTriggersOfStartupProcessL
// ---------------------------------------------------------------------------
//
void CLbtAppChangeHandler::DeleteTriggersOfStartupProcessL()
	{
	FUNC_ENTER("CLbtAppChangeHandler::DeleteTriggersOfStartupProcessL");
	CLbtContainerUpdateFilter* updateOptions = CLbtContainerUpdateFilter::NewL();
	CleanupStack::PushL(updateOptions);
	
	CLbtTriggerFilterByAttribute* attributeFilter = 
	                                CLbtTriggerFilterByAttribute::NewL();
	CleanupStack::PushL(attributeFilter);
	attributeFilter->AddTriggerTypeL(CLbtTriggerEntry::ETypeStartup);
	
	updateOptions->SetTrigInfoFilter(attributeFilter);
	
	CLbtContainerFilter* containerFilter = CLbtContainerFilter::NewL();
	CleanupStack::PushL(containerFilter);
	
	for(TInt i=0;i<iAppList.Count();++i)
		{
		containerFilter->AddStartupProcessInFilterL( iAppList[i].iFullName );
		}
	updateOptions->SetContainerFilter(containerFilter);

	CleanupStack::Pop(3); // containerFilter, attributeFilter & updateOptions
	
	iContainer.DeleteTriggers(updateOptions, iOpId, iStatus);
	SetActive();
	}


// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::IsMemoryCardPresent
// ---------------------------------------------------------------------------
//
TBool CLbtAppChangeHandler::IsMemoryCardPresent()
	{
	FUNC_ENTER("CLbtAppChangeHandler::IsMemoryCardPresent");
	
	//retrieve memory card identifier
    TInt drive;    
    TInt err = DriveInfo::GetDefaultDrive(
           DriveInfo::EDefaultRemovableMassStorage, drive); 
    if( err != KErrNone )
            return EFalse;
    
    //connect to file server
    RFs fs;
    err = fs.Connect();
    
    if(err != KErrNone)
        {
        return EFalse;
        }
    
    //check if memory card is present
    TUint status;
    err = DriveInfo::GetDriveStatus( fs, drive, status );
    fs.Close();
    
    if( err != KErrNone )
        return EFalse;
    return ( status & DriveInfo::EDrivePresent );
  
	}

// ---------------------------------------------------------------------------
// void CLbtAppChangeHandler::MemoryCardChar()
// ---------------------------------------------------------------------------
//
void CLbtAppChangeHandler::MemoryCardChar()
    {
    TInt drive;    
    TInt err = DriveInfo::GetDefaultDrive(
            DriveInfo::EDefaultRemovableMassStorage, drive); 
    //if memory card is not supported
    if( err!=KErrNone )
        {
        iMMCDriveChar.Copy(KMMCErrorChar);
        return;
        }
    
    //retrieve memory card character
    RFs fs;
    err = fs.Connect();
    if( err==KErrNone )
        {
        TChar memChar;
        fs.DriveToChar(drive,memChar);        
        iMMCDriveChar.Zero();
        iMMCDriveChar.Append( memChar );
        }
    else
        {
        iMMCDriveChar.Copy(KMMCErrorChar);
        }
    iMMCDriveChar.Append(KCharColon);
    fs.Close();
    }

// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::RunL
// ---------------------------------------------------------------------------
//
void CLbtAppChangeHandler::RunL()
	{
	FUNC_ENTER("CLbtAppChangeHandler::RunL");
	switch(iOperation)
		{
		case EOpRegiseteredForInitialListPoplulation:
			{
			// First get the set of installed applications in the system
			iAppArcSession.GetAllApps();
			
			TApaAppInfo appInfo;
			TInt error = iAppArcSession.GetNextApp(appInfo);
			
			while(KErrNone == error)
				{
		        TLbtAppInfo lbtAppInfo;
		        lbtAppInfo.iUid = appInfo.iUid;
		        lbtAppInfo.iFullName.Zero();
		        lbtAppInfo.iFullName.Copy( appInfo.iFullName );

				iCurrentInstalledApps.AppendL(lbtAppInfo);
				error = iAppArcSession.GetNextApp(appInfo);
				}
			iOperation = EOpNone;
			break;
			}
		case EOpRegiseteredForListPoplulation:
			{
			HandleAppListEvent(KErrNone);
			break;
			}
		case EOpRemovalTriggerDeletion:
			{
			// Here the observer is set to the get the information pertaining
            // triggers that were deleted.
            TLbtTriggerEventMask eventMask = 0;
            eventMask|= CLbtContainer::ELbtConTriggerDeleted;
            iContainer.SetChangeObserver( this,eventMask );
            
			// Delete triggers for which the un-installed application is the 
			// startup process
			iOperation = EOpDeletionStartupProcessTriggers;
			DeleteTriggersOfStartupProcessL();
			break;
			}
		case EOpRemovalListingStartupProcessTriggers:
			{
			// Here the observer is set to the get the information pertaining
            // triggers that were deleted.
            TLbtTriggerEventMask eventMask = 0;
            eventMask|= CLbtContainer::ELbtConTriggerDeleted;
            iContainer.SetChangeObserver( this,eventMask );
            
			CLbtContainerFilter* containerFilter = CLbtContainerFilter::NewL();
			CleanupStack::PushL(containerFilter);
			
			for(TInt i=0;i<iAppList.Count();++i)
				{
				containerFilter->AddOwnerSidInFilterL(iAppList[i].iUid);
				}
			
			CLbtTriggerFilterByAttribute* filterByAttribute = 
						CLbtTriggerFilterByAttribute::NewL();
			CleanupStack::PushL(filterByAttribute);
			filterByAttribute->AddTriggerTypeL(CLbtTriggerEntry::ETypeStartup);
			
			CLbtContainerUpdateFilter* updateFilter = CLbtContainerUpdateFilter::NewL(filterByAttribute ,containerFilter);
			
			iContainer.DeleteTriggers(updateFilter, iOpId, iStatus);
			SetActive();
			iOperation = EOpRemovalDeletingStartupProcessTriggers;
			break;
			}
		case EOpRemovalDeletingStartupProcessTriggers:
		    {
		    // Here the observer is set to the get the information pertaining
            // triggers that were deleted.
            TLbtTriggerEventMask eventMask = 0;
            eventMask|= CLbtContainer::ELbtConTriggerDeleted;
            iContainer.SetChangeObserver( this,eventMask );
		    break;
		    }
			
		case EOpMMCInsertionListingTriggers:			
		case EOpMMCRemovalListingTriggers:
			{
			// Listing Triggers of startup process
			LOG("Listing Triggers Over");
			
			if( iOperation == EOpMMCInsertionListingTriggers )
				{
				iOperation = EOpMMCInsertionListingTriggerForStartupProcess;
				}
			else
				{
				iOperation = EOpMMCRemovalListingTriggerForStartupProcess;
				}
		
			ListTriggersOfStartupProcessL();
			iAppList.Close();
			LOG("Listing Triggers for startup Process");
			break;
			}
		case EOpMMCRemovalListingTriggerForStartupProcess:
		case EOpMMCInsertionListingTriggerForStartupProcess:
			{
			LOG("Listing Trigger For startup Process Done");
			LOG1("iStatus = %d", iStatus.Int());
			LOG1("Triggers Count = %d", iTriggerEntries.Count());
			if(KErrNone == iStatus.Int())
				{
				// The list operation is a success. Hence invalidate the triggers now
				RArray<TLbtTriggerId> triggerIds;
				
				for(TInt i=0;i<iTriggerEntries.Count();++i)
					{
					triggerIds.AppendL(iTriggerEntries[i]->TriggerEntry()->Id());
					}
				
				TLbtTriggerDynamicInfo::TLbtTriggerValidity validity;
				
				if(iOperation == EOpMMCRemovalListingTriggerForStartupProcess)
					{
					validity = TLbtTriggerDynamicInfo::EInvalid;
					iCleanupHandler.AddTriggersForCleanupL( triggerIds );
					}
				else
					{
					validity = TLbtTriggerDynamicInfo::EValid;
					iCleanupHandler.RemoveTriggersFromCleanupL( triggerIds );
					}
			    
				// update validity of the triggers
				iContainer.UpdateTriggersValidity( validity,
												   triggerIds,
												   iOpId,
												   iStatus );
				SetActive();				
				iOperation = EOpRequestValidityChange;
				triggerIds.Close();
				iTriggerEntries.ResetAndDestroy();
				}
			else
				{
				// Listing is not successful. Hence dont do anything
				iOperation = EOpNone;
				iTriggerEntries.ResetAndDestroy();
				iAppList.Reset();
				}
			break;
			}
		case EOpRequestValidityChange:
		    {
		    // Here the observer is set to the get the information pertaining
            // triggers that were changed to invalid
            TLbtTriggerEventMask eventMask = 0;
            eventMask|= CLbtContainer::ELbtConTriggerValidityFieldChanged;
            iContainer.SetChangeObserver( this,eventMask );
            break;
		    }
		default:
			{
			// Operation complete
			iTriggerEntries.ResetAndDestroy();
			iAppList.Close();
			iOperation = EOpNone;
			break;			
			}
		}
	}

// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::RunError
// ---------------------------------------------------------------------------
//
TInt CLbtAppChangeHandler::RunError(TInt aError)
	{
	FUNC_ENTER("CLbtAppChangeHandler::RunError");	
	iTriggerEntries.ResetAndDestroy();
	iAppList.Reset();
	ERROR("CLbtAppChangeHandler::RunError:%d", aError)
	return KErrNone;
	}


// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::DoCancel
// ---------------------------------------------------------------------------
//
void CLbtAppChangeHandler::DoCancel()
	{
	FUNC_ENTER("CLbtAppChangeHandler::DoCancel");
	if(IsActive())
		{
		switch(iOperation)
			{
			case EOpRegiseteredForInitialListPoplulation:
				{
				iAppArcSession.CancelListPopulationCompleteObserver();
				break;
				}
			default:
				{
				iContainer.CancelAsyncOperation(iOpId);
				break;
				}
			}
		}
	}

// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::HandleApplicationUninstallationL
// ---------------------------------------------------------------------------
//
void CLbtAppChangeHandler::HandleApplicationUninstallationL( )
	{
	FUNC_ENTER("CLbtAppChangeHandler::HandleApplicationUninstallationL");
	// Since the application has been un-installed all the triggers that belong
	// to the application has to be deleted
	CLbtContainerFilter* containerFilter = CLbtContainerFilter::NewL();
	CleanupStack::PushL(containerFilter);
	
	for(TInt i=0;i<iAppList.Count();++i)
		{
		containerFilter->AddOwnerSidInFilterL(iAppList[i].iUid);
		}
	
	CLbtTriggerFilterByAttribute* filterByAttribute = 
				CLbtTriggerFilterByAttribute::NewL();
	CleanupStack::PushL(filterByAttribute);
	filterByAttribute->AddTriggerTypeL(CLbtTriggerEntry::ETypeStartup);
	
	CLbtContainerUpdateFilter* updateFilter = CLbtContainerUpdateFilter::NewL(filterByAttribute ,containerFilter);
	
	iContainer.DeleteTriggers(updateFilter, iOpId, iStatus);
	SetActive();
	iOperation = EOpRemovalTriggerDeletion;
	CleanupStack::Pop(2); //filterByAttribute and containerFilter. Container takes ownership
	}
	
// ---------------------------------------------------------------------------
// Default Constructor
// ---------------------------------------------------------------------------
//
CLbtAppChangeHandler::CLbtAppChangeHandler(CLbtContainer& aContainer, CLbtCleanupHandler& aCleanupHandler,
                                           MLbtServerLogicTriggerChangeObserver& aTriggerChangeObserver):
					   CActive(EPriorityStandard),
					   iContainer(aContainer),
					   iCleanupHandler(aCleanupHandler),
					   iTriggerChangeObserver(aTriggerChangeObserver)
	{
	CActiveScheduler::Add(this);
	}

// ---------------------------------------------------------------------------
// CLbtAppChangeHandler::TriggerStoreChanged
// ---------------------------------------------------------------------------
//
void CLbtAppChangeHandler::TriggerStoreChanged( RArray<TLbtTriggerId>& aTriggerIds,
                                               TLbtTriggerEventMask aEventMask,
                                               TLbtContainerChangedAreaType /*aAreaType*/,
                                               RArray<TUid>& aManagerUids )
    {
    // Remove observer
    iContainer.RemoveObserver( this );
    iTriggerChangeObserver.HandleTriggersChange( aTriggerIds,aManagerUids,aEventMask );
    }

// ---------------------------------------------------------------------------
// Default Destructor
// ---------------------------------------------------------------------------
//
CLbtAppChangeHandler::~CLbtAppChangeHandler()
	{
	FUNC_ENTER("CLbtAppChangeHandler::~CLbtAppChangeHandler");
	if( IsActive() )
	    {
	    Cancel();
	    iContainer.RemoveObserver( this );
	    }
	iAppArcSession.Close();
	delete iAppListNotifier;
	iCurrentInstalledApps.Close();
	iTriggerEntries.ResetAndDestroy();
	iAppList.Close();
	}	
// end of file

