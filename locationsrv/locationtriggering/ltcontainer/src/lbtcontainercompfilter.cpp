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
* Description: Implementation of composite filter class
*
*/


#include <s32strm.h>
#include <lbttriggerfilterbyattribute.h>
#include <lbttriggerfiltercomposite.h>
#include "lbtcontainerfilterbase.h"
#include "lbtcontainertriggerfilter.h"
#include "lbtcontainercompfilter.h"
#include "lbtcontainerareafilter.h"
#include "lbtcontainerattrfilter.h"
#include "lbtcontainerutilities.h"
#include "lbtlogger.h"



// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtContainerCompFilter::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CLbtContainerCompFilter* CLbtContainerCompFilter::NewL(CLbtTriggerFilterBase* aTrigFilter,CLbtContainerFilter* aContFilter)
    {
    FUNC_ENTER("CLbtContainerCompFilter::NewL");
  	CLbtContainerCompFilter* self = new( ELeave ) CLbtContainerCompFilter;
    CleanupStack::PushL( self );
    self->ConstructL(aTrigFilter,aContFilter);
	CleanupStack::Pop( self );    
    return self;
    }

void CLbtContainerCompFilter::ProcessFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested  )
	{
	FUNC_ENTER("CLbtContainerCompFilter::ProcessFilter");
	TInt isFilterPresent = 0;
	TBool isEntryRequested = EFalse;
	CLbtContainerFilter* contFilter = ContFilter();
	if(contFilter)
		{
		ProcessContFilter(aEntry,isFilterPresent,isEntryRequested);
		// if containerFilter attributes itself are not satisfied, return..
		// no use of processing further.
		
		if(isFilterPresent && !isEntryRequested)
			{
			aIsFilterPresent = isFilterPresent;
			aIsEntryRequested = isEntryRequested;
			return;
			}
		}
	for(TInt i=0;i<iFilterArray.Count();i++)
		{
		//Process for all the filters present in the composite filter
		iFilterArray[i]->ProcessFilter(aEntry,isFilterPresent,isEntryRequested);	
		// if any filter returs false, stop processing since API currenty only supports
		//AND composition.
		if(isFilterPresent && !isEntryRequested)
			break;
		}
	aIsFilterPresent = isFilterPresent;
	aIsEntryRequested = isEntryRequested;
	}

void CLbtContainerCompFilter::ProcessContFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested)
	{
	FUNC_ENTER("CLbtContainerCompFilter::ProcessContFilter");
	CLbtExtendedTriggerInfo* contExtInfo=aEntry->ExtendedTriggerInfo(); 
	TInt isFilterPresent = 0;
	TBool isEntryRequested = EFalse;
	TInt i;
	if(contExtInfo!=NULL)
             	{
                 if( iHystRadiusArray.Count()>0)  
                    {
                    isFilterPresent++;
                    isEntryRequested=EFalse;
                    for(i=0;i<iHystRadiusArray.Count();i++)
                	    {
                	    if(contExtInfo->HysteresisRadius()==iHystRadiusArray[i])
                    	    {
                    	    
                            isEntryRequested=ETrue;
                    	    }
                        
                	    }
                    }
                     
                    
                    if((isFilterPresent>0 && isEntryRequested  ) ||  isFilterPresent==0 )  
                        {
                        	if(iTriggerFiredArray.Count()>0)
                        		{
                        isFilterPresent++;
                        isEntryRequested=EFalse;
                        for(i=0;i<iTriggerFiredArray.Count();i++)
                    	    {
                    	    if(contExtInfo->IsTriggerFired()==iTriggerFiredArray[i])
                    		    {
                                isEntryRequested=ETrue;
                        
                    		    }
                   		    }
                        }
                      }
                        
                     if((isFilterPresent>0 && isEntryRequested && iSidArray.Count()>0) || (iSidArray.Count()>0 && isFilterPresent==0 ))  
                        {
                        isFilterPresent++;
                        isEntryRequested=EFalse;
                        for(i=0;i<iSidArray.Count();i++)
                    	    {
                    	    if(contExtInfo->OwnerSid()==iSidArray[i])
                        	    {
                        	    
                                isEntryRequested=ETrue;
                        	    }
                            
                    	    }
                        }     
                    
                    if((isFilterPresent>0 && isEntryRequested && iTriggerFireOnCreationArray.Count()>0) || (iTriggerFireOnCreationArray.Count()>0 && isFilterPresent==0 ))  
                        {
                        isFilterPresent++;
                        isEntryRequested=EFalse;
                        for(i=0;i<iTriggerFireOnCreationArray.Count();i++)
                    	    {
                    	    if(contExtInfo->IsTriggerFireOnCreation()==iTriggerFireOnCreationArray[i])
                        	    {
                        	    
                                isEntryRequested=ETrue;
                        	    }
                            
                    	    }
                        }  
                    
                    
                    } // end of if(contExtInfo!=NULL)
       aIsFilterPresent = isFilterPresent;
       aIsEntryRequested = isEntryRequested;	
	}
	
void CLbtContainerCompFilter::RetrieveFilterDataL()
	{	
	FUNC_ENTER("CLbtContainerCompFilter::RetrieveFilterDataL");
	CLbtContainerFilter* contFilter = ContFilter();
	CLbtTriggerFilterBase* trigFilter = TrigFilter();
	CLbtTriggerFilterComposite* compFilter = static_cast<CLbtTriggerFilterComposite*>(trigFilter);
	
	if(contFilter)
       {
        contFilter->GetSidArrayL(&iSidArray) ;
        contFilter->GetHystRadiusArrayL(&iHystRadiusArray); 
        contFilter->GetRectAreaArrayL(&iTriggerRectAreaArray) ;
        contFilter->GetTriggerFiredArrayL(&iTriggerFiredArray) ;
        contFilter->GetTriggerFireOnCreationArrayL( &iTriggerFireOnCreationArray );
        
        }
    if(compFilter)
        {
        TInt numOfFilters = compFilter->NumberOfFilters();
        for(TInt i = 0;i< numOfFilters;i++)
        	{
        	CLbtTriggerFilterBase* filter = LbtContainerUtilities::CloneFilterL(compFilter->GetFilter(i));
        	CleanupStack::PushL(filter);
        	switch(filter->Type())
        		{
    		    case CLbtTriggerFilterBase::EFilterByAttribute:
    				{
    				CLbtContainerAttrFilter* attrFilter = CLbtContainerAttrFilter::NewL(filter,NULL);
    				CleanupStack::PushL( attrFilter );
					iFilterArray.AppendL(attrFilter);
					CleanupStack::Pop( attrFilter );
					break;
    				}
    			case CLbtTriggerFilterBase::EFilterByArea:
    				{
    				CLbtContainerAreaFilter* areaFilter = CLbtContainerAreaFilter::NewL(filter,NULL);
    				CleanupStack::PushL( areaFilter );
    				iFilterArray.AppendL(areaFilter);
    				CleanupStack::Pop( areaFilter );
    				break;
    				}
    			case CLbtTriggerFilterBase::EFilterComposite:
    				{
    				CLbtContainerCompFilter* compFilter = CLbtContainerCompFilter::NewL(filter,NULL);
    				CleanupStack::PushL( compFilter );
    				iFilterArray.AppendL(compFilter);
    				CleanupStack::Pop( compFilter );
    				break;
    				}
        		}
        	CleanupStack::Pop(filter); // ownership taken by the container filters
        	}        
        }    
	}

CLbtContainerCompFilter::CLbtContainerCompFilter()
	{
	
	}


CLbtContainerCompFilter::~CLbtContainerCompFilter()
	{
	FUNC_ENTER("CLbtContainerCompFilter::~CLbtContainerCompFilter");
	iFilterArray.ResetAndDestroy();
	iHystRadiusArray.Close();
	iTriggerRectAreaArray.Close();
	iTriggerFiredArray.Close();
	iSidArray.Close();
	iTriggerFireOnCreationArray.Close();
	}
 
void CLbtContainerCompFilter::ConstructL(CLbtTriggerFilterBase* aTrigFilter,CLbtContainerFilter* aContFilter)
	{
	FUNC_ENTER("CLbtContainerCompFilter::ConstructL");
	SetTrigFilter(aTrigFilter);
	SetContFilter(aContFilter);
	RetrieveFilterDataL();
	}

