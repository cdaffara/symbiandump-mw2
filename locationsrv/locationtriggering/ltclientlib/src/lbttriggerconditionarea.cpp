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
* Description: Implementation of data class for trigger 
* condition based on the geographical area and movement of 
* the terminal
*
*/


#include "lbttriggerconditionbase.h"
#include "lbttriggerconditionarea.h"
#include "lbtgeocircle.h"
#include "lbtgeoareabase.h"
#include "lbtgeocell.h"
#include "lbtgeorect.h"
#include "lbtgeohybrid.h"
#include "lbterrors.h"


// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::CLbtTriggerConditionArea()
// Default constructor
// ---------------------------------------------------------------------------
//
CLbtTriggerConditionArea::CLbtTriggerConditionArea()
    {
	
	}

// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::~CLbtTriggerConditionArea()
// Destructor
// ---------------------------------------------------------------------------
//
CLbtTriggerConditionArea::~CLbtTriggerConditionArea()
    {
    if( iArea )
        {
        delete iArea;
        iArea = NULL;
        }
    }

// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtTriggerConditionArea* CLbtTriggerConditionArea::NewL( 
            CLbtGeoAreaBase* aArea,
            TDirection aDirection )
    {
	CLbtTriggerConditionArea* self = new( ELeave ) CLbtTriggerConditionArea;	
	CleanupStack::PushL( self );
	self->ConstructL(aArea,aDirection);
	CleanupStack::Pop( self );  
	return self;
    }
            
// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
EXPORT_C  CLbtTriggerConditionArea* CLbtTriggerConditionArea::NewL( )
    {
	CLbtTriggerConditionArea* self = new( ELeave ) CLbtTriggerConditionArea;	
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );  
	return self;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::NewLC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtTriggerConditionArea* CLbtTriggerConditionArea::NewLC( )
    {
	CLbtTriggerConditionArea* self = new( ELeave ) CLbtTriggerConditionArea;	
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::Direction()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtTriggerConditionArea::TDirection CLbtTriggerConditionArea::Direction() const
    {
 	return iDirection;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::SetDirection()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C void CLbtTriggerConditionArea::SetDirection( 
            TDirection aDirection )
    {
  	iDirection=aDirection;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::TriggerArea()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
EXPORT_C CLbtGeoAreaBase* CLbtTriggerConditionArea::TriggerArea() const
    {
 	return iArea;
    }   
// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::SetTriggerArea()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtTriggerConditionArea::SetTriggerArea( 
            CLbtGeoAreaBase* aArea )
    {
 	CLbtGeoAreaBase::TGeoAreaType areaType;
 	areaType=aArea->Type();
 	if(areaType ==CLbtGeoAreaBase::ERectangular)
 			User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
 	if( iArea )
 	    {
 	    delete iArea;
 	    iArea = NULL;
 	    }
 	iArea=aArea;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
// 
void CLbtTriggerConditionArea::ConstructL()
    {
    iArea = NULL;
 	iDirection=EFireOnEnter;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
// 
void CLbtTriggerConditionArea::ConstructL(CLbtGeoAreaBase* aArea,
            TDirection aDirection)
    {
 	CLbtGeoAreaBase::TGeoAreaType areaType;
 	areaType=aArea->Type();
 	if(areaType ==CLbtGeoAreaBase::ERectangular)
 			User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
 	iArea=aArea;
 	iDirection=aDirection;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::Type()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
CLbtTriggerConditionBase::TType CLbtTriggerConditionArea::Type() const
    {
 	return CLbtTriggerConditionBase::ETriggerConditionArea;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::DoInternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
void CLbtTriggerConditionArea::DoInternalizeL( RReadStream& aStream )
    {
 	TInt areaType=aStream.ReadInt8L();
 	if(areaType==1)
 	    {
 		CLbtGeoCircle* area= CLbtGeoCircle::NewL();
 		area->InternalizeL(aStream);
 		SetTriggerArea(area);
 		}
 	else if(areaType==2)
 	    {
 		CLbtGeoRect* area=CLbtGeoRect::NewL();
 		area->InternalizeL(aStream);
 		SetTriggerArea(area);
 	    }
 	else if( areaType==3 )
 	    {
 	    CLbtGeoCell* area=CLbtGeoCell::NewL();
 		area->InternalizeL(aStream);
 		SetTriggerArea(area);
 	    }
 	else if( areaType==4)
        {
        CLbtGeoHybrid* area=CLbtGeoHybrid::NewL();
        area->InternalizeL(aStream);
        SetTriggerArea(area);
        }
 	
 	TInt direction;
 	direction=aStream.ReadInt16L();
	if(direction==1)
	 iDirection=EFireOnEnter;
	
	else if(direction==2)
	 iDirection=EFireOnExit;
 	}

// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::DoExternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
void CLbtTriggerConditionArea:: DoExternalizeL( RWriteStream& aStream ) const
    {
 	CLbtGeoAreaBase::TGeoAreaType areaType;
 	
 	if(iArea)
 	    {
 	    areaType=iArea->Type();
 	    if(areaType==CLbtGeoAreaBase::ECircle)
 		    aStream.WriteInt8L(1);
 	    else if( areaType==CLbtGeoAreaBase::ERectangular )
 		    aStream.WriteInt8L(2);
 	    else if( areaType==CLbtGeoAreaBase::ECellular )
 	        aStream.WriteInt8L(3);
 	        
 	    else if( areaType==CLbtGeoAreaBase::EHybrid )
 	       aStream.WriteInt8L(4); 
 	    iArea->ExternalizeL(aStream);
 	    }
 	else
        {
        aStream.WriteInt8L(0);
        }
             	
 	aStream.WriteInt16L(iDirection);
    }

