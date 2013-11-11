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
#include <lbtsessiontrigger.h>
#include <lbtstartuptrigger.h>
#include <lbttriggerentry.h>
#include <lbttriggerconditionbase.h>
#include <lbttriggerconditionarea.h>
#include <lbtgeoareabase.h>
#include <lbtgeocircle.h>
#include <lbtgeorect.h>
#include <lbtgeocell.h>
#include <lbtgeohybrid.h>
#include "lbtcontainertriggerentry.h"
#include "lbtcontainerextendedtriggerinfo.h"
#include "lbtcontainerutilities.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtContainerTriggerEntry::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtContainerTriggerEntry* CLbtContainerTriggerEntry::NewL()
    {
	CLbtContainerTriggerEntry* self=new( ELeave ) CLbtContainerTriggerEntry;
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self;
    }


// ---------------------------------------------------------------------------
// CLbtContainerTriggerEntry::NewL(
//    CLbtTriggerEntry* aEntry,
//    TLbtTriggerDynamicInfo* aDynInfo,
//    TLbtDerivedTriggerInfo* aDerivedInfo)
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtContainerTriggerEntry* CLbtContainerTriggerEntry::NewL(
    CLbtTriggerEntry* aEntry,
    TLbtTriggerDynamicInfo* aDynInfo,
    CLbtExtendedTriggerInfo* aExtendedTriggerInfo)
    {
	CLbtContainerTriggerEntry* self=new( ELeave ) CLbtContainerTriggerEntry;
	CleanupStack::PushL( self );
    self->ConstructL(aEntry,aDynInfo,aExtendedTriggerInfo);
    CleanupStack::Pop( self ); 
    return self;
    }


// ---------------------------------------------------------------------------
// CLbtContainerTriggerEntry::~CLbtContainerTriggerEntry()
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtContainerTriggerEntry::~CLbtContainerTriggerEntry()
    {
    delete iExtendedTriggerInfo;
    }


// ---------------------------------------------------------------------------
// CLbtContainerTriggerEntry::DerivedInfo()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtExtendedTriggerInfo* CLbtContainerTriggerEntry::ExtendedTriggerInfo()  
    {
	return iExtendedTriggerInfo;
    }


// ---------------------------------------------------------------------------
// CLbtContainerTriggerEntry::SetDerivedInfo()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainerTriggerEntry::SetExtendedTriggerInfo( 
    CLbtExtendedTriggerInfo* aExtendedTriggerInfo )
    {
    if ( iExtendedTriggerInfo )
        {
        delete iExtendedTriggerInfo;
        }
	iExtendedTriggerInfo = aExtendedTriggerInfo;
    }

// ---------------------------------------------------------------------------
// Constructor
//
// ---------------------------------------------------------------------------
//
CLbtContainerTriggerEntry::CLbtContainerTriggerEntry() : iExtendedTriggerInfo(NULL)
    {
    SetTriggerEntry(NULL);
    SetDynInfo(NULL);
    InitializeStrategyData();
    }

// ---------------------------------------------------------------------------
// Copy Constructor
//
// ---------------------------------------------------------------------------
//
CLbtContainerTriggerEntry::CLbtContainerTriggerEntry(
    CLbtContainerTriggerEntry& aEntry) : iExtendedTriggerInfo(NULL)
    
	{
	SetTriggerEntry(NULL);
    SetDynInfo(NULL);
    InitializeStrategyData();
	// ToDo : Ignoring the error as of now. Will be changed once the clone 
	// methods are created in client Library.
	TRAP_IGNORE(CloneL(aEntry));
	}	

// ---------------------------------------------------------------------------
// CLbtContainerTriggerEntry::InitializeStrategyData
// ---------------------------------------------------------------------------
//
void CLbtContainerTriggerEntry::InitializeStrategyData()
	{
	iStrategyData.iFireTime = 0;
	iStrategyData.iActivityStatus = EActive;
	iStrategyData.iBoundaryCondition = 0;
	iStrategyData.iVisitedCell[0] = 0;
	iStrategyData.iVisitedCell[1] = 0;
	
	CLbtTriggerEntry* triggerEntry = TriggerEntry();
	if( triggerEntry )
	    {	    
	    // Initialise hybrid data
	    iStrategyData.iHybridState = EHybridNone;
	    
	    if( triggerEntry->GetCondition() )
	        {
	        if( triggerEntry->GetCondition()->Type() == CLbtTriggerConditionBase::ETriggerConditionArea )
	            {
	            // Get the condition area base
	            CLbtTriggerConditionArea* conditionArea = 
	                     static_cast<CLbtTriggerConditionArea*>(triggerEntry->GetCondition());
	             
	            if( conditionArea->TriggerArea() )
	                {
	                // Check if the area is a circle
                    if(conditionArea->TriggerArea()->Type() == CLbtGeoAreaBase::ECircle)
                        {
                        CLbtGeoCircle* circle = static_cast<CLbtGeoCircle*>(conditionArea->TriggerArea());
                        RPointerArray<CLbtGeoAreaBase> additionalArea = circle->GetAdditionalGeoAreaInfo();
                        if( additionalArea.Count() )
                            {
                            iStrategyData.iHybridState = EHybridInActive;
                            }
                        }
	                }	            
	            }
	        }	    
	    }
	}


// ---------------------------------------------------------------------------
// Clone method
//
// ---------------------------------------------------------------------------
//
void CLbtContainerTriggerEntry::CloneL(CLbtContainerTriggerEntry& aEntry)
    {
	CLbtTriggerEntry::TType trigType;
	trigType=aEntry.TriggerEntry()->Type();
	CLbtTriggerEntry* trigEntry=aEntry.TriggerEntry();
	CLbtTriggerConditionBase* trigCondition= trigEntry->GetCondition();
	TLbtTriggerDynamicInfo* dynInfo=aEntry.DynInfo();
	CLbtExtendedTriggerInfo* extInfo=aEntry.ExtendedTriggerInfo();
	CLbtTriggerEntry* entry=NULL;
	
	if(trigType==CLbtTriggerEntry::ETypeSession)
		{
		entry=CLbtSessionTrigger::NewL();
		entry->SetId(trigEntry->Id());
		entry->SetNameL(trigEntry->Name());
		entry->SetManagerUi(trigEntry->ManagerUi());
		entry->SetState(trigEntry->State());
		entry->SetTimeToRearm( trigEntry->TimeToRearm() );
		if(trigCondition->Type() == 
		    CLbtTriggerConditionBase::ETriggerConditionArea)
			{
			CLbtTriggerConditionArea* cond = 
			    (CLbtTriggerConditionArea*)trigCondition;
			CLbtTriggerConditionArea* trigCondArea = 
			                        CLbtTriggerConditionArea::NewL();
			
			CLbtGeoAreaBase* trigArea = NULL;
			trigCondArea->SetDirection(cond->Direction());
			CLbtGeoAreaBase* areaBase=cond->TriggerArea();
			
			trigArea = LbtContainerUtilities::CopyGeoAreaL( areaBase );			
			trigCondArea->SetTriggerArea(trigArea);
		    entry->SetCondition(trigCondArea);
		    SetTriggerEntry(entry);
		    }
  
		}
	else
		{
		// We use clone only for session triggers.
		}
		
    if(dynInfo!=NULL)
        {
		TLbtTriggerDynamicInfo* dynInfoPtr=new TLbtTriggerDynamicInfo;
		dynInfoPtr->iValidity=dynInfo->iValidity;
		dynInfoPtr->iDistanceToLatestLocation=dynInfo->iDistanceToLatestLocation;
		SetDynInfo(dynInfoPtr);
        }
	
	if(extInfo!=NULL)
	    {
		iExtendedTriggerInfo = CLbtExtendedTriggerInfo::NewL();	    
		iExtendedTriggerInfo->SetTriggerRectangleArea(extInfo->TriggerReactangleArea()) ;
		iExtendedTriggerInfo->SetHysteresisRadius(extInfo->HysteresisRadius());
		iExtendedTriggerInfo->SetTriggerFiredState(extInfo->IsTriggerFired());
		iExtendedTriggerInfo->SetOwnerSid(extInfo->OwnerSid());
		iExtendedTriggerInfo->SetTriggerFireOnCreation(extInfo->IsTriggerFireOnCreation());
	    }
    }
// ---------------------------------------------------------------------------
// Symbian 2nd phase of Construction
//
// ---------------------------------------------------------------------------
//
void CLbtContainerTriggerEntry::ConstructL()
    {
    ConstructL(NULL,NULL,NULL);    
    }


// ---------------------------------------------------------------------------
// Symbian 2nd phase of Construction
//
// ---------------------------------------------------------------------------
//
void CLbtContainerTriggerEntry::ConstructL( CLbtTriggerEntry* aEntry,
										    TLbtTriggerDynamicInfo* aDynInfo,
										    CLbtExtendedTriggerInfo* aExtendedTriggerInfo )
    {
    SetTriggerEntry(aEntry);
    SetDynInfo(aDynInfo);
	iExtendedTriggerInfo = aExtendedTriggerInfo;

	// Initialize to default values 
    iStrategyData.iActivityStatus = EActive;    
    }

// ---------------------------------------------------------------------------
// CLbtContainerTriggerEntry::SetStrategyData
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainerTriggerEntry::SetStrategyData( TStrategyData& aStartegyData )
    {
    iStrategyData = aStartegyData;
    }
    
// ---------------------------------------------------------------------------
// CLbtContainerTriggerEntry::StrategyData
// ---------------------------------------------------------------------------
//
EXPORT_C TStrategyData CLbtContainerTriggerEntry::StrategyData()
    {
    return iStrategyData;
    }    
// end of file

