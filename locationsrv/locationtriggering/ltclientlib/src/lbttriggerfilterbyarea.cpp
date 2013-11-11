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
* Description: Implementation of class representing filter based on
* geographical area.
*
*/


#include "lbttriggerfilterbyarea.h"
#include "lbttriggerfilterbase.h"
#include "lbtgeocircle.h"
#include "lbtgeorect.h"
#include "lbtgeocell.h"
#include "lbtgeohybrid.h"


// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtTriggerFilterByArea* CLbtTriggerFilterByArea::NewL(CLbtGeoAreaBase* aArea)
    {
   	CLbtTriggerFilterByArea* self = new( ELeave ) CLbtTriggerFilterByArea;
    CleanupStack::PushL( self );
    self->ConstructL( aArea);
	CleanupStack::Pop( self );  
	return self;
    }
    
// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C  CLbtTriggerFilterByArea* CLbtTriggerFilterByArea::NewL()
    {
	CLbtTriggerFilterByArea* self = new( ELeave ) CLbtTriggerFilterByArea;
    CleanupStack::PushL( self );
    //self->ConstructL( aArea);
	CleanupStack::Pop( self );  
	return self;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::NewLC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C  CLbtTriggerFilterByArea* CLbtTriggerFilterByArea::NewLC()
    {
	CLbtTriggerFilterByArea* self = new( ELeave ) CLbtTriggerFilterByArea;
    CleanupStack::PushL( self );
    //self->ConstructL( aArea);
	return self;
    }
    
// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::Type()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C  CLbtTriggerFilterBase::TFilterType CLbtTriggerFilterByArea::Type() const
    {
    return CLbtTriggerFilterBase::EFilterByArea;
    }   

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::Area()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C CLbtGeoAreaBase* CLbtTriggerFilterByArea::Area()
    {
    return iGeoArea;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::SetArea()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtTriggerFilterByArea::SetArea( CLbtGeoAreaBase* aArea )
    {
    if( iGeoArea )
        {
        delete iGeoArea;
        iGeoArea = NULL;
        }
    iGeoArea=aArea;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::~CLbtTriggerFilterByArea()
// Destructor
// ---------------------------------------------------------------------------
// 
CLbtTriggerFilterByArea::~CLbtTriggerFilterByArea()
    {
    if( iGeoArea )
        {
        delete iGeoArea;
        iGeoArea = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtTriggerFilterByArea::ConstructL(CLbtGeoAreaBase* aArea)
    {
	iGeoArea=aArea;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::DoInternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C void CLbtTriggerFilterByArea::DoInternalizeL( RReadStream& aStream ) 
    {
	TInt areaType=aStream.ReadInt8L();
 	if( areaType==1 )
 	    {
 		CLbtGeoCircle* area= CLbtGeoCircle::NewL();
 		area->InternalizeL(aStream);
 		iGeoArea=area;
 	    }
 	else if( areaType==2 )
 	    {
 		CLbtGeoRect* area=CLbtGeoRect::NewL();
 		area->InternalizeL(aStream);
 		iGeoArea=area;
 	    }
 	else if( areaType==3 )
 	    {
 	    CLbtGeoCell* area= CLbtGeoCell::NewL();
 		area->InternalizeL(aStream);
 		iGeoArea=area;
 	    }
    else if( areaType==4 )
        {
        CLbtGeoHybrid* area= CLbtGeoHybrid::NewL();
        area->InternalizeL(aStream);
        iGeoArea=area;
        }

    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::DoExternalizeL()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtTriggerFilterByArea::DoExternalizeL( RWriteStream& aStream ) const
    {
	CLbtGeoAreaBase::TGeoAreaType areaType;
 	areaType=iGeoArea->Type();
 		
 	if(areaType==CLbtGeoAreaBase::ECircle)
 	    aStream.WriteInt8L(1);
 	else if( areaType==CLbtGeoAreaBase::ERectangular )
 	    aStream.WriteInt8L(2);
 	else if( areaType==CLbtGeoAreaBase::ECellular )
 	    aStream.WriteInt8L(3);
 	else if( areaType==CLbtGeoAreaBase::EHybrid )
        aStream.WriteInt8L(4);
 	iGeoArea->ExternalizeL(aStream);
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::CLbtTriggerFilterByArea()
// Default constructor
// ---------------------------------------------------------------------------
// 
CLbtTriggerFilterByArea::CLbtTriggerFilterByArea()
    {
	iGeoArea = NULL;
    }
