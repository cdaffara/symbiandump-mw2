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
* Description: Implementation of class representing a composite 
* filter
*
*/


#include "lbttriggerfiltercomposite.h"
#include "lbttriggerfilterbase.h"
#include "lbttriggerfilterbyarea.h"
#include "lbttriggerfilterbyattribute.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//

EXPORT_C  CLbtTriggerFilterComposite* CLbtTriggerFilterComposite::NewL(TCompositeType aType)
    {
   	CLbtTriggerFilterComposite* self = CLbtTriggerFilterComposite::NewLC( aType );
	CleanupStack::Pop( self );  
	return self;
    }


// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//

EXPORT_C  CLbtTriggerFilterComposite* CLbtTriggerFilterComposite::NewLC(TCompositeType /*aType*/)
    {
   	CLbtTriggerFilterComposite* self = new( ELeave ) CLbtTriggerFilterComposite;
    CleanupStack::PushL( self );
    self->ConstructL( );
	return self;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::Type()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C  CLbtTriggerFilterBase::TFilterType CLbtTriggerFilterComposite::Type() const
    {
    return CLbtTriggerFilterBase::EFilterComposite;
    }   

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::AddFilterL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtTriggerFilterComposite::AddFilterL( CLbtTriggerFilterBase* aFilter )
    {
	iFilters.Append(aFilter);
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::NumberOfFilters()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CLbtTriggerFilterComposite::NumberOfFilters() const
    {
	return iFilters.Count();
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::GetFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C CLbtTriggerFilterBase* CLbtTriggerFilterComposite::GetFilter( 
            TInt aIndex )
    {
	return iFilters[aIndex];
    }
 
// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::RemoveFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
 EXPORT_C void CLbtTriggerFilterComposite::RemoveFilter( 
            TInt aIndex )
    {
	iFilters.Remove(aIndex);
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::~CLbtTriggerFilterByArea()
// Destructor
// ---------------------------------------------------------------------------
//  
CLbtTriggerFilterComposite::~CLbtTriggerFilterComposite()
    {   
    for(TInt i=0;i<iFilters.Count();i++)
        {
        delete iFilters[i];
        }
    iFilters.Close();
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::Reset()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
 EXPORT_C void CLbtTriggerFilterComposite::Reset()
    {
 	iFilters.Reset();
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterComposite::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtTriggerFilterComposite::ConstructL()
    {
    
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::DoInternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
void CLbtTriggerFilterComposite::DoInternalizeL( RReadStream& aStream ) 
    {
	TInt numFilters=aStream.ReadInt8L();
	TInt index=numFilters;
	
    while(index>0)
	    {
		CLbtTriggerFilterBase *filter=NULL;
	
		TInt filterType=aStream.ReadInt8L();
		switch (filterType)
		    {
			case 1:filter=CLbtTriggerFilterByAttribute::NewL();
					break;
			case 2:filter=CLbtTriggerFilterByArea::NewL();	
					break;
			case 3:filter=CLbtTriggerFilterComposite::NewL();
					break;
		    }
		filter->InternalizeL(aStream);
		iFilters.Append(filter);
		index--;
	    }
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::DoExternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
void CLbtTriggerFilterComposite::DoExternalizeL(RWriteStream& aStream) const
    {
	TInt numFilters=iFilters.Count();
	TInt index=0;
	
	aStream.WriteInt8L(numFilters);
	while(index<numFilters)
	    {
		CLbtTriggerFilterBase::TFilterType filterType=iFilters[index]->Type();
		switch(filterType)
		    {
			case CLbtTriggerFilterBase::EFilterByAttribute:aStream.WriteInt8L(1);
														   break;
			
			case CLbtTriggerFilterBase::EFilterByArea:aStream.WriteInt8L(2);
													  break;
													  			
			case CLbtTriggerFilterBase::EFilterComposite:aStream.WriteInt8L(3);
														 break;
	        }
		
	    iFilters[index]->ExternalizeL(aStream);
	    index++;
	    }
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterComposite::CLbtTriggerFilterComposite()
// Default constructor
// ---------------------------------------------------------------------------
//
CLbtTriggerFilterComposite::CLbtTriggerFilterComposite()
    {
	
    }

