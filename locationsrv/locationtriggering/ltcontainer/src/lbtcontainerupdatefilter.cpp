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

#include "lbtcontainerupdatefilter.h"
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
// CLbtContainerUpdateFilter::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtContainerUpdateFilter* CLbtContainerUpdateFilter::NewL()
    {
	CLbtContainerUpdateFilter* self=new( ELeave ) CLbtContainerUpdateFilter;
	CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self;
    }


// ---------------------------------------------------------------------------
// CLbtContainerUpdateFilter::NewL(
//    CLbtTriggerEntry* aEntry,
//    TLbtTriggerDynamicInfo* aDynInfo,
//    TLbtDerivedTriggerInfo* aDerivedInfo)
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtContainerUpdateFilter* CLbtContainerUpdateFilter::NewL(
    CLbtTriggerFilterBase* aTrigInfoFilter,
    CLbtContainerFilter* aContFilter )
    {
	CLbtContainerUpdateFilter* self=new( ELeave ) CLbtContainerUpdateFilter;
	CleanupStack::PushL( self );
    self->ConstructL(aTrigInfoFilter,aContFilter);
    CleanupStack::Pop( self ); 
    return self;
    }


// ---------------------------------------------------------------------------
// CLbtContainerListOptions::~CLbtContainerListOptions()
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtContainerUpdateFilter::~CLbtContainerUpdateFilter()
    {
    delete iContFilter;
    delete iTrigInfoFilter;
    }


// ---------------------------------------------------------------------------
// CLbtContainerListOptions::TriggerEntry()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtTriggerFilterBase* CLbtContainerUpdateFilter::TrigInfoFilter()  
    {
	return iTrigInfoFilter;
    }


// ---------------------------------------------------------------------------
// CLbtContainerListOptions::SetTriggerEntry()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainerUpdateFilter::SetTrigInfoFilter( 
    CLbtTriggerFilterBase* aTrigInfoFilter )
    {
    if ( iTrigInfoFilter )
        {
        delete iTrigInfoFilter;
        }
	iTrigInfoFilter = aTrigInfoFilter;
    }



  
// ---------------------------------------------------------------------------
// CLbtContainerListOptions::DerivedInfo()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtContainerFilter* CLbtContainerUpdateFilter::ContainerFilter()  
    {
	return iContFilter;
    }


// ---------------------------------------------------------------------------
// CLbtContainerListOptions::SetDerivedInfo()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainerUpdateFilter::SetContainerFilter( 
    CLbtContainerFilter* aContFilter )
    {
    if ( iContFilter )
        {
        delete iContFilter;
        }
	iContFilter = aContFilter;
    }


EXPORT_C TLbtTriggerDataMask CLbtContainerUpdateFilter::DataMask()
    {
    return iMask;
    }
     
EXPORT_C void CLbtContainerUpdateFilter::SetDataMask( TLbtTriggerDataMask aMask)
    {
    iMask=aMask;
    }
// ---------------------------------------------------------------------------
// Constructor
//
// ---------------------------------------------------------------------------
//
CLbtContainerUpdateFilter::CLbtContainerUpdateFilter()
    {
    iContFilter = NULL;
    iTrigInfoFilter = NULL;
    }

// ---------------------------------------------------------------------------
// Clone method
//
// ---------------------------------------------------------------------------
//
void CLbtContainerUpdateFilter::CloneL(CLbtContainerUpdateFilter& /*aEntry*/)
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
void CLbtContainerUpdateFilter::ConstructL()
    {
    ConstructL(NULL,NULL);	
    }


// ---------------------------------------------------------------------------
// Symbian 2nd phase of Construction
//
// ---------------------------------------------------------------------------
//
void CLbtContainerUpdateFilter::ConstructL(
    CLbtTriggerFilterBase* aTrigInfoFilter,
    CLbtContainerFilter* aContFilter )
    {
    iTrigInfoFilter = aTrigInfoFilter;
    iContFilter = aContFilter;
    }

