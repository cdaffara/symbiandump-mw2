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
* the additional attributes  of a trigger
*
*/


#include "lbtcontainertriggerfilter.h"
#include "lbtlogger.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtContainerFilter::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtContainerFilter* CLbtContainerFilter::NewL()
    {
   	CLbtContainerFilter* self = new( ELeave ) CLbtContainerFilter;
    CleanupStack::PushL( self );
    self->ConstructL();
	CleanupStack::Pop( self );  
	return self;
    }

// ---------------------------------------------------------------------------
// CLbtContainerFilter::NewL()
// ---------------------------------------------------------------------------
//
CLbtContainerFilter* CLbtContainerFilter::NewL(CLbtContainerFilter* aFilter)
	{
	CLbtContainerFilter* self = new( ELeave ) CLbtContainerFilter;
    CleanupStack::PushL( self );
    self->ConstructL(aFilter);
	CleanupStack::Pop( self );  
	return self;
	}


// ---------------------------------------------------------------------------
// Constructor for CLbtContainerFilter
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                     
EXPORT_C CLbtContainerFilter::~CLbtContainerFilter()
    {
    FUNC_ENTER("CLbtContainerFilter::~CLbtContainerFilter");
    iHystRadiusArray.Close();
    iTriggerRectAreaArray.Close();
    iTriggerFiredArray.Close();
    iSidArray.Close();
    iTriggerFireOnCreationArray.Close();
    }


// ---------------------------------------------------------------------------
// CLbtContainerFilter::AddTriggerTypeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainerFilter::AddHysteresisRadiusInFilter( 
    TReal aRadius )
    {
	TInt error = iHystRadiusArray.Append(aRadius);
	if( error != KErrNone )
	    {
        LOG1("Failed to append radius to the array:%d",error);
	    }
    }


// ---------------------------------------------------------------------------
// CLbtContainerFilter::IsTriggerTypeInFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//

EXPORT_C TBool CLbtContainerFilter::IsHysteresisRadiusInFilter( 
    TReal aRadius) const  
    {
    FUNC_ENTER("CLbtContainerFilter::IsHysteresisRadiusInFilter");
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
// CLbtContainerFilter::AddTriggerStateL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainerFilter::AddRectTrigAreaInFilterL( 
    CLbtExtendedTriggerInfo::TLbtTriggerRectArea aArea )
    {
	iTriggerRectAreaArray.AppendL(aArea);
    }


// ---------------------------------------------------------------------------
// CLbtContainerFilter::IsTriggerStateInFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CLbtContainerFilter::IsRectTriggerAreaInFilter( 
    CLbtExtendedTriggerInfo::TLbtTriggerRectArea aArea ) const
    {
    FUNC_ENTER("CLbtContainerFilter::IsRectTriggerAreaInFilter");
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
// CLbtContainerFilter::AddTriggerValidityL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainerFilter::AddFiredInfoInFilterL( 
    TBool aFired)
    {
    iTriggerFiredArray.AppendL(aFired);
    }           
   
// ---------------------------------------------------------------------------
// CLbtContainerFilter::IsTriggerValidityStatusInFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                     
EXPORT_C TBool CLbtContainerFilter::IsTriggerFiredInfoInFilter( 
    TBool aFired) const
            
    {
    FUNC_ENTER("CLbtContainerFilter::IsTriggerFiredInfoInFilter");
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
// CLbtContainerFilter::AddOwnerSidInFilterL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                     
EXPORT_C void CLbtContainerFilter::AddOwnerSidInFilterL( 
    TSecureId aSid )
    {
    iSidArray.AppendL( aSid );
    }

            
// ---------------------------------------------------------------------------
// CLbtContainerFilter::IsOwnerSidInFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                     
EXPORT_C TBool CLbtContainerFilter::IsOwnerSidInFilter( 
    TSecureId aSid ) const
    {
    FUNC_ENTER("CLbtContainerFilter::IsOwnerSidInFilter");
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
// CLbtContainerFilter::AddTriggerFireOnCreationInFilterL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CLbtContainerFilter::AddTriggerFireOnCreationInFilterL( 
        TBool aFireOnCreation )
	{
	iTriggerFireOnCreationArray.AppendL( aFireOnCreation );	
	}

// ---------------------------------------------------------------------------
// CLbtContainerFilter::IsTriggerFireOnCreationInFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CLbtContainerFilter::IsTriggerFireOnCreationInFilter( 
        TBool aFireOnCreation ) const
	{
	FUNC_ENTER("CLbtContainerFilter::IsTriggerFireOnCreationInFilter");
	TInt countObj = iTriggerFireOnCreationArray.Count();
	if( countObj > 0 )
    	{
		TInt ret = iTriggerFireOnCreationArray.Find( aFireOnCreation );
		if ( ret != KErrNotFound )
		    {
			return ETrue;
		    }
    	}
	return EFalse;	
	}    
	
// ---------------------------------------------------------------------------
// CLbtContainerFilter::GetHystRadiusArrayL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtContainerFilter::GetHystRadiusArrayL(RArray<TReal>* aHystRadiusArray)
    {
    FUNC_ENTER("CLbtContainerFilter::GetHystRadiusArrayL");
    aHystRadiusArray->Reset();
    TInt count = iHystRadiusArray.Count();
    for(TInt i=0; i<count; i++)
        {
        User::LeaveIfError(aHystRadiusArray->Append(iHystRadiusArray[i]));
        }
    }

// ---------------------------------------------------------------------------
// CLbtContainerFilter::GetRectAreaArrayL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CLbtContainerFilter::GetRectAreaArrayL(RArray<CLbtExtendedTriggerInfo::TLbtTriggerRectArea>* aTriggerRectAreaArray)
    {
    FUNC_ENTER("CLbtContainerFilter::GetRectAreaArrayL");
    aTriggerRectAreaArray->Reset();
    TInt count = iTriggerRectAreaArray.Count();
    for(TInt i=0; i<count; i++)
        {
        User::LeaveIfError(aTriggerRectAreaArray->Append(iTriggerRectAreaArray[i]));
        }
    }
    
// ---------------------------------------------------------------------------
// CLbtContainerFilter::GetTriggerFiredArrayL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C void CLbtContainerFilter::GetTriggerFiredArrayL (RArray < TBool >* aTriggerFiredArray)
    {
    FUNC_ENTER("CLbtContainerFilter::GetTriggerFiredArrayL");
    aTriggerFiredArray->Reset();
    TInt count = iTriggerFiredArray.Count();
    for(TInt i=0; i<count; i++)
        {
        User::LeaveIfError(aTriggerFiredArray->Append(iTriggerFiredArray[i]));
        }
    }

// ---------------------------------------------------------------------------
// CLbtContainerFilter::GetSidArrayL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtContainerFilter::GetSidArrayL(RArray< TSecureId >* aSidArray)
    {
    FUNC_ENTER("CLbtContainerFilter::GetSidArrayL");
    aSidArray->Reset();
    TInt count = iSidArray.Count();
    for(TInt i=0; i<count; i++)
        {
        User::LeaveIfError(aSidArray->Append(iSidArray[i]));
        }
    }

// ---------------------------------------------------------------------------
// CLbtContainerFilter::GetTriggerFireOnCreationArrayL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CLbtContainerFilter::GetTriggerFireOnCreationArrayL(RArray< TBool >* aTriggerFireOnCreationArray)
    {
    FUNC_ENTER("CLbtContainerFilter::GetTriggerFireOnCreationArrayL");
    aTriggerFireOnCreationArray->Reset();
    TInt count = iTriggerFireOnCreationArray.Count();
    for(TInt i=0; i<count; i++)
        {
        User::LeaveIfError(aTriggerFireOnCreationArray->Append(iTriggerFireOnCreationArray[i]));
        }
    }

// ---------------------------------------------------------------------------
// CLbtContainerFilter::AddStartupProcessInFilterL
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CLbtContainerFilter::AddStartupProcessInFilterL( TFileName& aFileName )
	{
	iStartupProcessArray.AppendL( aFileName );
	}

// ---------------------------------------------------------------------------
// CLbtContainerFilter::IsStartupProcessInFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CLbtContainerFilter::IsStartupProcessInFilter( TFileName& aFileName )
	{
	FUNC_ENTER("CLbtContainerFilter::IsStartupProcessInFilter");
	for(TInt i=0;i<iStartupProcessArray.Count();++i)
		{
		if(iStartupProcessArray[i].Compare(aFileName) == 0)
			{
			return ETrue;
			}
		}
	return EFalse;
	}

// ---------------------------------------------------------------------------
// CLbtContainerFilter::GetStartupProcessArrayL
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CLbtContainerFilter::GetStartupProcessArrayL(RArray<TFileName>* aStartupProcessArray)
	{
	FUNC_ENTER("CLbtContainerFilter::GetStartupProcessArrayL");
    aStartupProcessArray->Reset();
    TInt count = iStartupProcessArray.Count();
    for(TInt i=0; i<count; i++)
        {
        User::LeaveIfError(aStartupProcessArray->Append(iStartupProcessArray[i]));
        }
	}

// ---------------------------------------------------------------------------
// Constructor for CLbtContainerFilter
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                     
CLbtContainerFilter::CLbtContainerFilter()
    {
    // Nothing to do here
    }

  
// ---------------------------------------------------------------------------
// Constructor for CLbtContainerFilter
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                     
void CLbtContainerFilter::ConstructL()
    {
    // Nothing to do here
    }


// ---------------------------------------------------------------------------
// Constructor for CLbtContainerFilter
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                     
void CLbtContainerFilter::ConstructL(CLbtContainerFilter* aFilter)
    {
    FUNC_ENTER("CLbtContainerFilter::ConstructL");
    
    if(!aFilter)
    	{
    	return;
    	}

    TInt i;
    // Copy hysterisis array
    RArray<TReal> hystArray;
    aFilter->GetHystRadiusArrayL(&hystArray);
    
    for(i=0;i<hystArray.Count();++i)
    	{
    	iHystRadiusArray.AppendL(hystArray[i]);
    	}
    hystArray.Close();
    
    // Copy trigger rect array
    RArray<CLbtExtendedTriggerInfo::TLbtTriggerRectArea> triggerRectAreaArray;
    aFilter->GetRectAreaArrayL(&triggerRectAreaArray);
    
    for(i=0;i<triggerRectAreaArray.Count();++i)
    	{
    	iTriggerRectAreaArray.AppendL(triggerRectAreaArray[i]);
    	}
    triggerRectAreaArray.Close();
    
    // Copy trigger fired array
    RArray<TBool> triggerFiredArray;
    aFilter->GetTriggerFiredArrayL(&triggerFiredArray);
    
    for(i=0;i<triggerFiredArray.Count();++i)
    	{
    	iTriggerFiredArray.AppendL(triggerFiredArray[i]);
    	}
    triggerFiredArray.Close();
    
    // Copy SID array
    RArray<TSecureId> sidArray;
    aFilter->GetSidArrayL(&sidArray);
    
    for(i=0;i<sidArray.Count();++i)
    	{
    	iSidArray.AppendL(sidArray[i]);
    	}
    sidArray.Close();
    
    // Copy fire on creation array
    RArray<TBool> triggerFireOnCreationArray;
    aFilter->GetTriggerFireOnCreationArrayL(&triggerFireOnCreationArray);
    
    for(i=0;i<triggerFireOnCreationArray.Count();++i)
    	{
    	iTriggerFireOnCreationArray.AppendL(triggerFireOnCreationArray[i]);
    	}
    triggerFireOnCreationArray.Close();
    
    // Copy the startup process array
    RArray<TFileName> triggerstartupProcessArray;
    aFilter->GetStartupProcessArrayL(&triggerstartupProcessArray);
    
    for(i=0;i<triggerstartupProcessArray.Count();++i)
    	{
    	iStartupProcessArray.AppendL(triggerstartupProcessArray[i]);
    	}
    triggerstartupProcessArray.Close();
    }


// end of file

