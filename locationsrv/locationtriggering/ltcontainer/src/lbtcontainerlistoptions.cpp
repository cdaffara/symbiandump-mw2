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

#include "lbtcontainerlistoptions.h"
#include "lbtcontainertriggerfilter.h"
#include <lbtsessiontrigger.h>
#include <lbtstartuptrigger.h>
#include <lbttriggerentry.h>
#include <lbttriggerconditionbase.h>
#include <lbttriggerconditionarea.h>
#include <lbtgeoareabase.h>
#include <lbtgeocircle.h>
#include <lbtgeorect.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtContainerListOptions::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtContainerListOptions* CLbtContainerListOptions::NewL()
    {
	CLbtContainerListOptions* self=new( ELeave ) CLbtContainerListOptions;
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self;
    }


// ---------------------------------------------------------------------------
// CLbtContainerListOptions::NewL(
//    CLbtTriggerEntry* aEntry,
//    TLbtTriggerDynamicInfo* aDynInfo,
//    TLbtDerivedTriggerInfo* aDerivedInfo)
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtContainerListOptions* CLbtContainerListOptions::NewL(
    CLbtListTriggerOptions* aListOptions,
    CLbtContainerFilter* aFilter)
    {
	CLbtContainerListOptions* self=new( ELeave ) CLbtContainerListOptions;
	CleanupStack::PushL( self );
    self->ConstructL(aListOptions,aFilter);
    CleanupStack::Pop( self ); 
    return self;
    }


// ---------------------------------------------------------------------------
// CLbtContainerListOptions::~CLbtContainerListOptions()
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtContainerListOptions::~CLbtContainerListOptions()
    {
    // Ownership of filter base is not take by list options.
    // Hence delete the filter base
    if( iListOptions )
    	{
    	CLbtTriggerFilterBase* filter = iListOptions->Filter();
    	delete filter;
    	}
    delete iListOptions;
    delete iFilter;
    }


// ---------------------------------------------------------------------------
// CLbtContainerListOptions::TriggerEntry()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtListTriggerOptions* CLbtContainerListOptions::ListOptions()  
    {
	return iListOptions;
    }


// ---------------------------------------------------------------------------
// CLbtContainerListOptions::SetTriggerEntry()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainerListOptions::SetListOptions( 
    CLbtListTriggerOptions* aListOptions )
    {
    if ( iListOptions )
        {
        delete iListOptions;
        }
	iListOptions = aListOptions;
    }



  
// ---------------------------------------------------------------------------
// CLbtContainerListOptions::DerivedInfo()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtContainerFilter* CLbtContainerListOptions::ContainerFilter()  
    {
	return iFilter;
    }


// ---------------------------------------------------------------------------
// CLbtContainerListOptions::SetDerivedInfo()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainerListOptions::SetContainerFilter( 
    CLbtContainerFilter* aFilter )
    {
    if ( iFilter )
        {
        delete iFilter;
        }
	iFilter = aFilter;
    }


EXPORT_C TLbtTriggerDataMask CLbtContainerListOptions::DataMask()
    {
    return iMask;
    }
     
EXPORT_C void CLbtContainerListOptions::SetDataMask( TLbtTriggerDataMask aMask)
    {
    iMask=aMask;
    }
// ---------------------------------------------------------------------------
// Constructor
//
// ---------------------------------------------------------------------------
//
CLbtContainerListOptions::CLbtContainerListOptions()
    {
    iListOptions = NULL;
    iFilter = NULL;
    }

// ---------------------------------------------------------------------------
// Clone method
//
// ---------------------------------------------------------------------------
//
void CLbtContainerListOptions::CloneL(CLbtContainerListOptions& /*aEntry*/)
    {
/*	CLbtTriggerEntry::TType trigType;
	trigType=aEntry.TriggerEntry()->Type();
	CLbtTriggerEntry* trigEntry=aEntry.TriggerEntry();
	CLbtTriggerConditionBase* trigCondition= trigEntry->GetCondition();
	TLbtTriggerDynamicInfo* dynInfo=aEntry.DynInfo();
	CLbtExtendedTriggerInfo* extInfo=aEntry.ExtendedTriggerInfo();
	CLbtGeoAreaBase* trigArea;
	
	if(trigType==CLbtTriggerEntry::ETypeSession)
		{
		iEntry=CLbtSessionTrigger::NewL();
		iEntry->SetId(trigEntry->Id());
		iEntry->SetNameL(trigEntry->Name());
		iEntry->SetManagerUi(trigEntry->ManagerUi());
		iEntry->SetState(trigEntry->State());
		if(trigCondition->Type() == 
		    CLbtTriggerConditionBase::ETriggerConditionArea)
			{
			CLbtTriggerConditionArea* cond = 
			    (CLbtTriggerConditionArea*)trigCondition;
			CLbtTriggerConditionArea* trigCondArea = 
			                        CLbtTriggerConditionArea::NewL();
			trigCondArea->SetDirection(cond->Direction());
			CLbtGeoAreaBase* areaBase=cond->TriggerArea();
			if( areaBase->Type() == CLbtGeoAreaBase::ECircle )
				{
				CLbtGeoCircle* areaCircle=(CLbtGeoCircle*) areaBase;
				trigArea=CLbtGeoCircle::NewL(areaCircle->Center(),
				                             areaCircle->Radius());
				trigCondArea->SetTriggerArea(trigArea);
				}
			else if(areaBase->Type()==CLbtGeoAreaBase::ERectangular)
				{
				CLbtGeoRect* areaRect=(CLbtGeoRect*)areaBase;
				TReal southLat,northLat,eastLong,westLong;
				areaRect->GetBorder(southLat,northLat,westLong,eastLong);
				 trigArea=CLbtGeoRect::NewL(southLat,
				                            northLat,
				                            westLong,
				                            eastLong);
				trigCondArea->SetTriggerArea(trigArea);
			    }
		    iEntry->SetCondition(trigCondArea);
		    }
		}
	else
		{
		iEntry=CLbtStartupTrigger::NewL();
		}
	iDynInfo=new TLbtTriggerDynamicInfo;
	iDynInfo->iValidity=dynInfo->iValidity;
	iDynInfo->iActivityStatus=dynInfo->iActivityStatus;
	iDynInfo->iDistanceToLatestLocation=dynInfo->iDistanceToLatestLocation;
	
	iExtendedTriggerInfo = CLbtExtendedTriggerInfo::NewL();
    
	iExtendedTriggerInfo->SetTriggerRectangleArea(extInfo->TriggerReactangleArea()) ;
	iExtendedTriggerInfo->SetHysteresisRadius(extInfo->HysteresisRadius());
	iExtendedTriggerInfo->SetTriggerFiredState(extInfo->IsTriggerFired());
//	iExtendedTriggerInfo->SetStrategyDataL(extInfo->StategyData());
	iExtendedTriggerInfo->SetOwnerSid(extInfo->OwnerSid());*/
    }
// ---------------------------------------------------------------------------
// Symbian 2nd phase of Construction
//
// ---------------------------------------------------------------------------
//
void CLbtContainerListOptions::ConstructL()
    {
    ConstructL(NULL,NULL);	
    }


// ---------------------------------------------------------------------------
// Symbian 2nd phase of Construction
//
// ---------------------------------------------------------------------------
//
void CLbtContainerListOptions::ConstructL(
    CLbtListTriggerOptions* aListOptions,
    CLbtContainerFilter* aFilter)
    {
    iListOptions = aListOptions;
    iFilter = aFilter;
    }

