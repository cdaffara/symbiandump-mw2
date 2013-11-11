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
* attribute values of a trigger.
*
*/


#include "lbttriggerfilterbyattribute.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtTriggerFilterByAttribute* CLbtTriggerFilterByAttribute::NewL()
    {
   	CLbtTriggerFilterByAttribute* self = CLbtTriggerFilterByAttribute::NewLC();
	CleanupStack::Pop( self );  
	return self;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::NewLC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C  CLbtTriggerFilterByAttribute* CLbtTriggerFilterByAttribute::NewLC()
    {
   	CLbtTriggerFilterByAttribute* self = new( ELeave ) CLbtTriggerFilterByAttribute;
    CleanupStack::PushL( self );
    self->ConstructL();
	return self;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::ResetFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
 EXPORT_C void CLbtTriggerFilterByAttribute::ResetFilter()
    {
 	iTriggerTypeArray.Reset();
 	iTriggerStateArray.Reset();
 	iTriggerValidityArray.Reset();
 	iManagerUiArray.Reset();
 	iIdArray.Reset();
    }
// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::AddTriggerTypeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerFilterByAttribute::AddTriggerTypeL( 
            CLbtTriggerEntry::TType aType )
    {
	iTriggerTypeArray.Append(aType);    
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::IsTriggerTypeInFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CLbtTriggerFilterByAttribute::IsTriggerTypeInFilter( 
            CLbtTriggerEntry::TType aType ) const  
    {
    TInt countObj=iTriggerTypeArray.Count();
 		if(countObj>0)
 		{
		TInt ret=iTriggerTypeArray.Find(aType);
		if(ret==KErrNotFound)
			return EFalse;
		else
			return ETrue;
 		}
 	else
 		return EFalse;	
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::AddTriggerStateL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerFilterByAttribute::AddTriggerStateL( 
            CLbtTriggerEntry::TLbtTriggerState aState )
            
    {
	iTriggerStateArray.Append(aState);
    }
                
// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::IsTriggerStateInFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CLbtTriggerFilterByAttribute::IsTriggerStateInFilter( 
            CLbtTriggerEntry::TLbtTriggerState aState ) const
    {
    TInt countObj=iTriggerStateArray.Count();
 	if(countObj>0)
 		{
 		TInt ret=iTriggerStateArray.Find(aState);
 		if(ret==KErrNotFound)
 			return EFalse;
 		else
 			return ETrue;
 		}
 	else
 		return EFalse;	
    }  

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByArea::Type()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C  CLbtTriggerFilterBase::TFilterType CLbtTriggerFilterByAttribute::Type() const
    {
    return CLbtTriggerFilterBase::EFilterByAttribute;
    }   

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::AddTriggerValidityL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
 EXPORT_C void CLbtTriggerFilterByAttribute::AddTriggerValidityL( 
            TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity )
    {
    iTriggerValidityArray.Append(aValidity);
    }           
   
// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::IsTriggerValidityStatusInFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                     
EXPORT_C TBool CLbtTriggerFilterByAttribute::IsTriggerValidityStatusInFilter( 
            TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity ) const
    {
    TInt countObj=iTriggerValidityArray.Count();
 	if(countObj>0)
 		{
		TInt ret=iTriggerValidityArray.Find(aValidity);
		if(ret==KErrNotFound)
			return EFalse;
		else
			return ETrue;
 		}
 	else
 		return EFalse;
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::AddTriggerManagerUiL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtTriggerFilterByAttribute::AddTriggerManagerUiL(
            TUid aUid )
    {
	iManagerUiArray.Append(aUid);
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::IsTriggerManagerUiInFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CLbtTriggerFilterByAttribute::IsTriggerManagerUiInFilter(
            TUid aUid ) const
    {
	TInt countObj=iManagerUiArray.Count();
	if(countObj>0)
	    {
		TInt ret=iManagerUiArray.Find(aUid);
		if(ret==KErrNotFound)
			return EFalse;
		else
			return ETrue;
	    }
 	else
 		return EFalse;            	
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::AddTriggerIdL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerFilterByAttribute::AddTriggerIdL(
            TLbtTriggerId aId )
    {
	iIdArray.Append(aId);
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::GetStateArrayL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerFilterByAttribute::GetStateArrayL( RArray <CLbtTriggerEntry::TLbtTriggerState>& aTriggerStateArray) const
    {
    aTriggerStateArray.Reset();
    TInt count = iTriggerStateArray.Count();
    for(TInt i=0; i<count; i++)
        {
        User::LeaveIfError(aTriggerStateArray.Append(iTriggerStateArray[i]));
        }
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::GetTypeArrayL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerFilterByAttribute::GetTypeArrayL( RArray <CLbtTriggerEntry::TType>& aTriggerTypeArray) const
    {
    aTriggerTypeArray.Reset();
    TInt count = iTriggerTypeArray.Count();
    for(TInt i=0; i<count; i++)
        {
        User::LeaveIfError(aTriggerTypeArray.Append(iTriggerTypeArray[i]));
        }
    }
// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::GetValidityArrayL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerFilterByAttribute::GetValidityArrayL( RArray < TLbtTriggerDynamicInfo::TLbtTriggerValidity >& aTriggerValidityArray) const
    {
    aTriggerValidityArray.Reset();
    TInt count = iTriggerValidityArray.Count();
    for(TInt i=0; i<count; i++)
        {
        User::LeaveIfError(aTriggerValidityArray.Append(iTriggerValidityArray[i]));
        }
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::GetManagerUiArrayL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerFilterByAttribute::GetManagerUiArrayL( RArray < TUid >& aManagerUiArray) const
    {
    aManagerUiArray.Reset();
    TInt count = iManagerUiArray.Count();
    for(TInt i=0; i<count; i++)
        {
        User::LeaveIfError(aManagerUiArray.Append(iManagerUiArray[i]));
        }
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::GetTriggerIdArrayL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerFilterByAttribute::GetTriggerIdArrayL( RArray < TLbtTriggerId >& aIdArray) const
    {
    aIdArray.Reset();
    TInt count = iIdArray.Count();
    for(TInt i=0; i<count; i++)
        {
        User::LeaveIfError(aIdArray.Append(iIdArray[i]));
        }
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::IsTriggerIdInFilter()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CLbtTriggerFilterByAttribute::IsTriggerIdInFilter(
            TLbtTriggerId aId ) const
    {
	TInt countObj=iIdArray.Count();
	if(countObj>0)
	    {
		TInt ret=iIdArray.Find(aId);
		if(ret==KErrNotFound)
			return EFalse;
		else
			return ETrue;
	    }
 	else
 		return EFalse;    
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::DoInternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerFilterByAttribute::DoInternalizeL(RReadStream& aStream) 
    {
	TInt numTypeFilter=aStream.ReadInt8L();
	TInt ctr=numTypeFilter;
	while(ctr>0)
	    {
		TInt typeFilter=aStream.ReadInt8L();
		switch(typeFilter)
		    {
			case 1:  iTriggerTypeArray.Append(CLbtTriggerEntry::ETypeSession);
					break;
					
			case 2: iTriggerTypeArray.Append(CLbtTriggerEntry::ETypeStartup);
					break;
		    }
		ctr--;
	    }
	TInt numStateFilter=aStream.ReadInt8L();	
	ctr=numStateFilter;
	
	while(ctr>0)
	    {
		TInt stateFilter=aStream.ReadInt8L();
		switch(stateFilter)
		    {
			case 1:iTriggerStateArray.Append(CLbtTriggerEntry::EStateEnabled);
					break;
			
			case 2:iTriggerStateArray.Append(CLbtTriggerEntry::EStateDisabled);
					break;
			}
		ctr--;
	    }
	
	TInt numValidityFilter=aStream.ReadInt8L();
	ctr=numValidityFilter;
	
	while(ctr>0)
	    {
		TInt validityFilter=aStream.ReadInt8L();
		switch(validityFilter)
		    {
			case 1:iTriggerValidityArray.Append(TLbtTriggerDynamicInfo::EValid);
					break;
			
			case 2:iTriggerValidityArray.Append(TLbtTriggerDynamicInfo::EInvalid);
					break;
		    }
	    ctr--;	
	    }
	
	TInt numMgrUi=aStream.ReadInt8L();
	ctr=numMgrUi;
	while(ctr>0)
	    {
		TUid mgrUid;
		mgrUid.iUid=aStream.ReadInt32L();
		iManagerUiArray.Append(mgrUid);
		ctr--;	
	    }
	
	TInt numTrigId=aStream.ReadInt8L();
	ctr=numTrigId;
	
	while(ctr>0)
	    {
		TLbtTriggerId trigId=aStream.ReadInt32L();
		iIdArray.Append(trigId);
		ctr--;
	    }
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::DoExternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtTriggerFilterByAttribute::DoExternalizeL(RWriteStream& aStream) const
    {
	TInt numTypeFilter=iTriggerTypeArray.Count();
	TInt ctr=0;
	
	aStream.WriteInt8L(numTypeFilter);
	while(ctr<numTypeFilter)
	    {
		switch(iTriggerTypeArray[ctr])
		    {
			case CLbtTriggerEntry::ETypeSession:aStream.WriteInt8L(1);
							  break;
			
			case CLbtTriggerEntry::ETypeStartup:aStream.WriteInt8L(2);
							  break;
		    }
		ctr++;
	    }
	
	ctr=0;
	
	TInt numStateFilter=iTriggerStateArray.Count();
	
	aStream.WriteInt8L(numStateFilter);
	
	while(ctr<numStateFilter)
	    {
		switch(iTriggerStateArray[ctr])
		    {
			case CLbtTriggerEntry::EStateEnabled: aStream.WriteInt8L(1);
						   break;
			
			case CLbtTriggerEntry::EStateDisabled: aStream.WriteInt8L(2);
							break;
		    }
		ctr++;
	}
	
	ctr=0;
	TInt numValidityFilter=iTriggerValidityArray.Count();
	
	aStream.WriteInt8L(numValidityFilter);
	
	while(ctr<numValidityFilter)
	    {
		switch(iTriggerValidityArray[ctr])
		    {
			case TLbtTriggerDynamicInfo::EValid:aStream.WriteInt8L(1);
						   break;
			
			case TLbtTriggerDynamicInfo::EInvalid:aStream.WriteInt8L(2);
							break;
		    }
		ctr++;
	    }
	
	ctr=0;
	TInt numMgrUi=iManagerUiArray.Count();	
	
	aStream.WriteInt8L(numMgrUi);
	
	while(ctr<numMgrUi)
	    {
		aStream.WriteInt32L(iManagerUiArray[ctr].iUid)	;
		ctr++;
	    }
	
	ctr=0;
	TInt numTrigId=iIdArray.Count();
	aStream.WriteInt8L(numTrigId);
	
	while(ctr<numTrigId)
	    {
		aStream.WriteInt32L(iIdArray[ctr]);
		ctr++;
	    }
	}

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::CLbtTriggerFilterByAttribute()
// Default constructor
// ---------------------------------------------------------------------------
//   	
EXPORT_C CLbtTriggerFilterByAttribute::CLbtTriggerFilterByAttribute()
    {
  	
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::~CLbtTriggerFilterByAttribute()
// Destructor
// ---------------------------------------------------------------------------
//  
EXPORT_C CLbtTriggerFilterByAttribute::~CLbtTriggerFilterByAttribute()
    {
  	iTriggerTypeArray.Close();
  	iTriggerStateArray.Close();
  	iTriggerValidityArray.Close();
  	iManagerUiArray.Close();
  	iIdArray.Close();
    }

// ---------------------------------------------------------------------------
// CLbtTriggerFilterByAttribute::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CLbtTriggerFilterByAttribute::ConstructL()
    {
  	
    }
