/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of helper class used when listing 
* entries from location triggering server.
*
*/


#include "lbtlisttriggeroptions.h"
#include "lbttriggerfilterbase.h"
#include "lbttriggerfilterbyattribute.h"
#include "lbttriggerfilterbyarea.h"
#include "lbttriggerfiltercomposite.h"

// ---------------------------------------------------------------------------
// CLbtListTriggerOptions::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtListTriggerOptions* CLbtListTriggerOptions::NewL()
    {
   	CLbtListTriggerOptions* self = new( ELeave ) CLbtListTriggerOptions;
    CleanupStack::PushL( self );
    self->ConstructL();
	CleanupStack::Pop( self );  
	return self;
    }

// ---------------------------------------------------------------------------
// CLbtListTriggerOptions::NewLC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtListTriggerOptions* CLbtListTriggerOptions::NewLC()
    {
    CLbtListTriggerOptions* self = new( ELeave ) CLbtListTriggerOptions;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtListTriggerOptions::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtListTriggerOptions::ConstructL()
    {
 	iTriigerAttributeFieldsMask=KLbtTriggerAttributeFieldsAll;
 	iDynInfoFieldsMask=KLbtTriggerDynInfoFieldsAll;
 	iSorting=ELbtNoSorting;
 	iFilter=NULL;
    }
    
// ---------------------------------------------------------------------------
// CLbtListTriggerOptions::GetRetrievedFields( 
//            TLbtTriggerAttributeFieldsMask &aEntryMask,
//            TLbtTriggerDynamicInfoFieldsMask &aDynInfoMask ) 
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtListTriggerOptions::GetRetrievedFields( 
            TLbtTriggerAttributeFieldsMask &aEntryMask,
            TLbtTriggerDynamicInfoFieldsMask &aDynInfoMask ) const

    {
    aDynInfoMask=iDynInfoFieldsMask;
    aEntryMask=iTriigerAttributeFieldsMask;
    }
    
// ---------------------------------------------------------------------------
// CLbtListTriggerOptions::SetRetrievedFields( 
//            TLbtTriggerAttributeFieldsMask aEntryMask,
//            TLbtTriggerDynamicInfoFieldsMask aDynInfoMask = KLbtTriggerDynInfoFieldsAll ) 
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//

EXPORT_C void CLbtListTriggerOptions::SetRetrievedFields( 
            TLbtTriggerAttributeFieldsMask aEntryMask, /* check if the default param has to be mentioned here*/
           TLbtTriggerDynamicInfoFieldsMask aDynInfoMask )
    {
    iTriigerAttributeFieldsMask=aEntryMask;
    iDynInfoFieldsMask=aDynInfoMask;
    }
     
// ---------------------------------------------------------------------------
// CLbtListTriggerOptions::SortingOption()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtListTriggerOptions::TLbtListTriggerSorting CLbtListTriggerOptions::SortingOption() const
    {
    return iSorting;
    }
    
// ---------------------------------------------------------------------------
// CLbtListTriggerOptions::SetSortingOption()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtListTriggerOptions::SetSortingOption( TLbtListTriggerSorting aOption )
    {
    iSorting=aOption;
    }

// ---------------------------------------------------------------------------
// CLbtListTriggerOptions::SetFilterL( CLbtTriggerFilterBase* aFilter )
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtListTriggerOptions::SetFilter( 
            CLbtTriggerFilterBase* aFilter )
    {
	iFilter=aFilter;
    }	
    
// ---------------------------------------------------------------------------
// CLbtListTriggerOptions::Filter( )
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtTriggerFilterBase* CLbtListTriggerOptions:: Filter() 
    {
	return iFilter;
    }        

// ---------------------------------------------------------------------------
// CLbtListTriggerOptions::InternalizeL( )
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtListTriggerOptions::InternalizeL( RReadStream& aStream )
    {
	iTriigerAttributeFieldsMask=aStream.ReadUint32L();
	iDynInfoFieldsMask=aStream.ReadUint32L();
	TInt filterType;
	filterType=aStream.ReadInt8L();
	switch(filterType)
	    {
		case 1: iFilter=CLbtTriggerFilterByAttribute::NewL();
				iFilter->InternalizeL(aStream);
				break;
		case 2: iFilter=CLbtTriggerFilterByArea::NewL();
				iFilter->InternalizeL(aStream);
				break;
				
		case 3: iFilter=CLbtTriggerFilterComposite::NewL();
				iFilter->InternalizeL(aStream);
				break;
		}
	TInt sortOption=aStream.ReadInt8L();
	
	switch(sortOption)
	    {
		case 1: iSorting= CLbtListTriggerOptions::ELbtNoSorting;
													break;
		case 2: iSorting= CLbtListTriggerOptions::ELbtTriggerNameAscending;
																break;
																
		case 3: iSorting= CLbtListTriggerOptions::ELbtTriggerNameDescending ;
																 break;
																 
		case 4: iSorting= CLbtListTriggerOptions::ELbtDistanceToLatestLocationAscending ;
		}
    }

// ---------------------------------------------------------------------------
// CLbtListTriggerOptions::ExternalizeL( )
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtListTriggerOptions::ExternalizeL( RWriteStream& aStream ) const
    {
	aStream.WriteUint32L(iTriigerAttributeFieldsMask);
	aStream.WriteUint32L(iDynInfoFieldsMask);
	
	if(iFilter)
		{
	    CLbtTriggerFilterBase::TFilterType filterType=iFilter->Type();
	
	    switch(filterType)
	        {
		    case CLbtTriggerFilterBase::EFilterByAttribute: aStream.WriteInt8L(1);
														break;	
		    case CLbtTriggerFilterBase::EFilterByArea: aStream.WriteInt8L(2);
												   break;
		    case CLbtTriggerFilterBase::EFilterComposite: aStream.WriteInt8L(3);
													  break;
	        default: aStream.WriteInt8L(0);
	        }
	    iFilter->ExternalizeL(aStream);    
		}	
	else
	    {
	    aStream.WriteInt8L(0);
	    }		
	
	switch(iSorting)
	    {
		case CLbtListTriggerOptions::ELbtNoSorting: aStream.WriteInt8L(1);
													break;
		case CLbtListTriggerOptions::ELbtTriggerNameAscending : aStream.WriteInt8L(2);
																break;
																
		case CLbtListTriggerOptions::ELbtTriggerNameDescending : aStream.WriteInt8L(3);
																 break;
																 
		case CLbtListTriggerOptions::ELbtDistanceToLatestLocationAscending :aStream.WriteInt8L(4);
		}
	
	aStream.WriteInt8L(iSorting);
    }

// ---------------------------------------------------------------------------
// CLbtListTriggerOptions::CLbtListTriggerOptions()
// Default constructor
// ---------------------------------------------------------------------------
// 
CLbtListTriggerOptions::CLbtListTriggerOptions()
    {
	
    }

// ---------------------------------------------------------------------------
// CLbtListTriggerOptions::~CLbtListTriggerOptions()
// Destructor
// ---------------------------------------------------------------------------
// 
EXPORT_C CLbtListTriggerOptions::~CLbtListTriggerOptions()
    {
	
    }

