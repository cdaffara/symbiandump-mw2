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
* Description: Implementation of class for filter based on the 
* attributes value of a trigger.
*
*/


#include "lbtcontainertriggerfilterbyattribute.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtContainerFilterByAttribute::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtContainerFilterByAttribute* CLbtContainerFilterByAttribute::NewL()
    {
   	CLbtContainerFilterByAttribute* self = new( ELeave ) CLbtContainerFilterByAttribute;
    CleanupStack::PushL( self );
    self->ConstructL();
	CleanupStack::Pop( self );  
	return self;
    }


// ---------------------------------------------------------------------------
// Constructor for CLbtContainerFilterByAttribute
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                     
EXPORT_C CLbtContainerFilterByAttribute::~CLbtContainerFilterByAttribute()
    {
    iHystRadiusArray.Reset();
    iTriggerRectAreaArray.Reset();
    iTriggerFiredArray.Reset();
    iSidArray.Reset();
    }


// ---------------------------------------------------------------------------
// CLbtContainerFilterByAttribute::AddTriggerTypeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainerFilterByAttribute::AddHysteresisRadiusInFilter( 
    TReal aRadius )
    {
	iHystRadiusArray.Append(aRadius);
    }


// ---------------------------------------------------------------------------
// CLbtContainerFilterByAttribute::IsTriggerTypeInFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//

EXPORT_C TBool CLbtContainerFilterByAttribute::IsHysteresisRadiusInFilter( 
    TReal aRadius) const  
    {
    TInt countObj=iHystRadiusArray.Count();
	if ( countObj > 0 )
    	{
		TInt ret = iHystRadiusArray.Find( aRadius );
		if ( ret != KErrNotFound )
		    {
			return ETrue;
		    }
    	}
	return EFalse;	
    }


// ---------------------------------------------------------------------------
// CLbtContainerFilterByAttribute::AddTriggerStateL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainerFilterByAttribute::AddRectTrigAreaInFilterL( 
    CLbtExtendedTriggerInfo::TLbtTriggerRectArea aArea )
    {
	iTriggerRectAreaArray.Append(aArea);
    }


// ---------------------------------------------------------------------------
// CLbtContainerFilterByAttribute::IsTriggerStateInFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CLbtContainerFilterByAttribute::IsRectTriggerAreaInFilter( 
    CLbtExtendedTriggerInfo::TLbtTriggerRectArea aArea ) const
    {
	TInt countObj = iTriggerRectAreaArray.Count();
	if ( countObj > 0 )
    	{
    	TInt ret = iTriggerRectAreaArray.Find( aArea );
		if ( ret != KErrNotFound )
		    {
			return ETrue;
		    }
    	}
	return EFalse;	
    }  

 

// ---------------------------------------------------------------------------
// CLbtContainerFilterByAttribute::AddTriggerValidityL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainerFilterByAttribute::AddFiredInfoInFilterL( 
    TBool aFired)
    {
    iTriggerFiredArray.Append(aFired);
    }           
   
// ---------------------------------------------------------------------------
// CLbtContainerFilterByAttribute::IsTriggerValidityStatusInFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                     
EXPORT_C TBool CLbtContainerFilterByAttribute::IsTriggerFiredInfoInFilter( 
    TBool aFired) const
            
    {
    TInt countObj = iTriggerFiredArray.Count();
	if( countObj > 0 )
    	{
		TInt ret = iTriggerFiredArray.Find( aFired );
		if ( ret != KErrNotFound )
		    {
			return ETrue;
		    }
    	}
	return EFalse;	
    }
            

// ---------------------------------------------------------------------------
// CLbtContainerFilterByAttribute::AddOwnerSidInFilterL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                     
EXPORT_C void CLbtContainerFilterByAttribute::AddOwnerSidInFilterL( 
    TSecureId aSid )
    {
    iSidArray.AppendL( aSid );
    }

            
// ---------------------------------------------------------------------------
// CLbtContainerFilterByAttribute::IsOwnerSidInFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                     
EXPORT_C TBool CLbtContainerFilterByAttribute::IsOwnerSidInFilter( 
    TSecureId aSid ) const
    {
    TInt countObj = iSidArray.Count();
	if( countObj > 0 )
    	{
		TInt ret = iSidArray.Find( aSid );
		if ( ret != KErrNotFound )
		    {
			return ETrue;
		    }
    	}
	return EFalse;	
    } 
 

// ---------------------------------------------------------------------------
// Constructor for CLbtContainerFilterByAttribute
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                     
CLbtContainerFilterByAttribute::CLbtContainerFilterByAttribute()
    {
    // Nothing to do here
    }

  
// ---------------------------------------------------------------------------
// Constructor for CLbtContainerFilterByAttribute
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                     
void CLbtContainerFilterByAttribute::ConstructL()
    {
    // Nothing to do here
    }
