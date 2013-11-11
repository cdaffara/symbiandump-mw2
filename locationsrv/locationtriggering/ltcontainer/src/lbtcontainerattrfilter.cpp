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
* Description: Implementation of container attribute filter class
*
*/


#include <s32strm.h>
#include <lbttriggerfilterbyattribute.h>
#include <lbttriggerentry.h>
#include <lbtstartuptrigger.h>
#include "lbtcontainerfilterbase.h"
#include "lbtcontainertriggerfilter.h"
#include "lbtcontainerattrfilter.h"
#include "lbtlogger.h"

// CONSTANTS
_LIT(KFileExtensionExe,".exe");

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtContainerAttrFilter::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CLbtContainerAttrFilter* CLbtContainerAttrFilter::NewL(CLbtTriggerFilterBase* aTrigFilter,CLbtContainerFilter* aContFilter)
    {
    FUNC_ENTER("CLbtContainerAttrFilter::NewL");
  	CLbtContainerAttrFilter* self = new( ELeave ) CLbtContainerAttrFilter;
    CleanupStack::PushL( self );
    self->ConstructL(aTrigFilter,aContFilter);
	CleanupStack::Pop( self );    
    return self;
    }

void CLbtContainerAttrFilter::ProcessFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested  )
	{
	FUNC_ENTER("CLbtContainerAttrFilter::ProcessFilter");
	TInt isFilterPresent = 0;
	TBool isEntryRequested=EFalse;
	if(aEntry)
		{
		CLbtTriggerEntry* trigEntry=aEntry->TriggerEntry();
        CLbtExtendedTriggerInfo* contExtInfo=aEntry->ExtendedTriggerInfo();
        TInt i=0;
        if(trigEntry!=NULL)
        	{
            if( iTriggerStateArray.Count()>0 )
            	{
                 isFilterPresent++;
                 for(i=0;i<iTriggerStateArray.Count();i++)
 	                {
                    if(trigEntry->State()==iTriggerStateArray[i])
                        {
                        isEntryRequested=ETrue;
                        break;
                        }
                    }
                }
            if( (isFilterPresent>0 && isEntryRequested ) || 
                (isFilterPresent==0) )
                {
                	if(iTriggerValidityArray.Count()>0)
                		{
                isFilterPresent++;
                isEntryRequested=EFalse;
                for(i=0;i<iTriggerValidityArray.Count();i++)
                   {
                   if(aEntry->DynInfo()->iValidity == iTriggerValidityArray[i])
                      {
                      isEntryRequested=ETrue;
                      }                
                   }
                }
              }
                   
            if((isFilterPresent>0 && isEntryRequested && iTriggerTypeArray.Count()>0) || (iTriggerTypeArray.Count()>0 && isFilterPresent==0))
                {
                 isFilterPresent++;
                 isEntryRequested=EFalse;
                 for(i=0;i<iTriggerTypeArray.Count();i++)
                    {
                    if(trigEntry->Type()==iTriggerTypeArray[i])
                       {
                       isEntryRequested=ETrue;
                       }
                
                    }
                 }
                   
            if((isFilterPresent>0 && isEntryRequested && iIdArray.Count()>0) || (iIdArray.Count()>0 && isFilterPresent==0 ))
                {
                isFilterPresent++;
                isEntryRequested=EFalse;
                for(i=0;i<iIdArray.Count();i++)
                    {
                    if(trigEntry->Id()==iIdArray[i])
                        {
                        isEntryRequested=ETrue;
                        }
                    }

                 }
                    
            if((isFilterPresent>0 && isEntryRequested && iManagerUiArray.Count()>0) || (iManagerUiArray.Count()>0 && isFilterPresent==0 )) 
                {
                isFilterPresent++;
                isEntryRequested=EFalse;
                for(i=0;i<iManagerUiArray.Count();i++)
                    {
                    if(trigEntry->ManagerUi()==iManagerUiArray[i])
                        {
                        isEntryRequested=ETrue;
                        }
                    }
                }
             
            if((isFilterPresent>0 && isEntryRequested && iStartupProcessArray.Count()>0) || (iStartupProcessArray.Count()>0 && isFilterPresent==0 )) 
                {
                // This filter applies to only startup triggers
                if( trigEntry->Type() == CLbtTriggerEntry::ETypeStartup )
                	{
                	CLbtStartupTrigger* startupTrigger = static_cast<CLbtStartupTrigger*>(trigEntry);
                	isFilterPresent++;
	                isEntryRequested=EFalse;
	                
	                TFileName fileName;
                	TSecureId secureId;
	                startupTrigger->GetProcessId(fileName, secureId);
					
					// Append the EXE extension
	                if (fileName.LocateReverse('.') == KErrNotFound)
						{
						if (fileName.Length() + 4 > KMaxFileName)
							return;
						fileName.Append( KFileExtensionExe );
						}

					LOGSTRING( "Trigger File Name : \"%S\" ", &fileName );
	                for(i=0;i<iStartupProcessArray.Count();i++)
	                    {
						LOGSTRING( "Filter startup name: \"%S\" ", &iStartupProcessArray[i] );
	                    if( FilesIdentical(fileName, iStartupProcessArray[i] ) )
	                    	{
	                    	LOG("Files Are Identical");
	                    	isEntryRequested=ETrue;
	                    	break;
	                    	}
	                    }
                	}                
                }
                
           }// end of if(trigEntry!=NULL)
              
             if(contExtInfo!=NULL)
             	{
                 if((isFilterPresent>0 && isEntryRequested && iHystRadiusArray.Count()>0) || (iHystRadiusArray.Count()>0 && isFilterPresent==0 ))  
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
                     
                    
                    if((isFilterPresent>0 && isEntryRequested && iTriggerFiredArray.Count()>0) || (iTriggerFiredArray.Count()>0 && isFilterPresent==0 ))  
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
        
		}//end of if(aEntry)
	aIsFilterPresent = isFilterPresent;
	aIsEntryRequested = isEntryRequested;
	}

//--------------------------------------------------------------------------
// CLbtContainerAttrFilter::DoesFileContainePathInfo
//--------------------------------------------------------------------------
//
TBool CLbtContainerAttrFilter::DoesFileContainePathInfo( TFileName& aFileName )
	{
	return (aFileName.LocateReverse('\\') != KErrNotFound);
	}

//--------------------------------------------------------------------------
// CLbtContainerAttrFilter::FilesIdentical
//--------------------------------------------------------------------------
//
TBool CLbtContainerAttrFilter::FilesIdentical( TFileName& aTriggerFileName, TFileName& aSystemFileName )
	{
	FUNC_ENTER("CLbtContainerAttrFilter::FilesIdentical");
	
	if( DoesFileContainePathInfo(aTriggerFileName) )
		{
		LOG("aTriggerFileName containes path info");		
		return ( aSystemFileName.CompareC(aTriggerFileName) != KErrNotFound );
		}
	
	TFileName triggerFileName;
	triggerFileName.Zero();
	triggerFileName.Copy(aTriggerFileName);
	triggerFileName.Insert(0,_L("\\"));
	
	LOGSTRING( "triggerFileName : \"%S\" ", &triggerFileName );
	LOGSTRING( "aSystemFileName: \"%S\" ", &aSystemFileName );
	
	return ( aSystemFileName.FindC( aTriggerFileName ) != KErrNotFound );	
	}

	
void CLbtContainerAttrFilter::RetrieveFilterDataL()
	{	
	FUNC_ENTER("CLbtContainerAttrFilter::RetrieveFilterDataL");
	CLbtContainerFilter* contFilter = ContFilter();
	CLbtTriggerFilterBase* trigFilter = TrigFilter();
	CLbtTriggerFilterByAttribute* attrFilter = static_cast<CLbtTriggerFilterByAttribute*>(trigFilter);
	if(contFilter)
       {
       contFilter->GetSidArrayL(&iSidArray) ;
       contFilter->GetHystRadiusArrayL(&iHystRadiusArray); 
       contFilter->GetRectAreaArrayL(&iTriggerRectAreaArray) ;
       contFilter->GetTriggerFiredArrayL(&iTriggerFiredArray) ;
       contFilter->GetTriggerFireOnCreationArrayL( &iTriggerFireOnCreationArray );
       contFilter->GetStartupProcessArrayL( &iStartupProcessArray );
       }

    if(attrFilter)
        {
        attrFilter->GetStateArrayL(iTriggerStateArray); 
        attrFilter->GetTypeArrayL(iTriggerTypeArray) ;
        attrFilter->GetValidityArrayL(iTriggerValidityArray) ;
        attrFilter->GetManagerUiArrayL(iManagerUiArray) ;
        attrFilter->GetTriggerIdArrayL(iIdArray) ; 
        }    
	}

CLbtContainerAttrFilter::CLbtContainerAttrFilter()
	{
	
	}


CLbtContainerAttrFilter::~CLbtContainerAttrFilter()
	{
	FUNC_ENTER("CLbtContainerAttrFilter::~CLbtContainerAttrFilter");
	iHystRadiusArray.Close();
	iTriggerRectAreaArray.Close();
	iTriggerFiredArray.Close();
	iSidArray.Close();
	iTriggerFireOnCreationArray.Close();
	iTriggerTypeArray.Close();
	iTriggerStateArray.Close();
	iTriggerValidityArray.Close();
	iManagerUiArray.Close();
	iIdArray.Close();
	iStartupProcessArray.Close();
	}

void CLbtContainerAttrFilter::ConstructL(CLbtTriggerFilterBase* aTrigFilter,CLbtContainerFilter* aContFilter)
	{
	FUNC_ENTER("CLbtContainerAttrFilter::ConstructL");
	SetTrigFilter(aTrigFilter);
	SetContFilter(aContFilter);
	RetrieveFilterDataL();
	}

