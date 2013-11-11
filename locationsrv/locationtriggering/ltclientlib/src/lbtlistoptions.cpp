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


#include "lbtlistoptions.h"
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
 void CLbtListTriggerOptions::ConstructL()
 {
 	
 	
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
EXPORT_C void CLbtListTriggerOptions::SetFilterL( 
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
           
CLbtListTriggerOptions::CLbtListTriggerOptions()
{
	
}

