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
* Description: Implementation of container area filter class
*
*/


#include <s32strm.h>
#include <lbttriggerfilterbyarea.h>
#include <lbtgeoareabase.h>
#include <lbttriggerconditionarea.h>
#include <lbtgeocircle.h>
#include <lbtgeorect.h>
#include <lbtgeocell.h>
#include <lbtgeohybrid.h>
#include "lbtcontainerfilterbase.h"
#include "lbtcontainertriggerfilter.h"
#include "lbtcontainerareafilter.h"
#include "lbtlogger.h"




// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtContainerAttrFilter::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CLbtContainerAreaFilter* CLbtContainerAreaFilter::NewL(CLbtTriggerFilterBase* aTrigFilter,CLbtContainerFilter* aContFilter)
    {
    FUNC_ENTER("CLbtContainerAreaFilter::NewL");
  	CLbtContainerAreaFilter* self = new( ELeave ) CLbtContainerAreaFilter;
    CleanupStack::PushL( self );
    self->ConstructL(aTrigFilter,aContFilter);
	CleanupStack::Pop( self );    
    return self;
    }

void CLbtContainerAreaFilter::ProcessFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested  )
	{
	FUNC_ENTER("CLbtContainerAreaFilter::ProcessFilter");
	ProcessContFilter( aEntry,aIsFilterPresent, aIsEntryRequested);
	
	if((aIsFilterPresent>0 && aIsEntryRequested)||(aIsFilterPresent == 0))
		{
		CLbtGeoAreaBase::TGeoAreaType type = iArea->Type();
		switch( type )
		    {
		    case CLbtGeoAreaBase::ECircle:
		        {
		        ProcessCircleFilter(aEntry,aIsFilterPresent,aIsEntryRequested);
		        }
		        break;
          case CLbtGeoAreaBase::ERectangular:
                {
                ProcessRectFilter(aEntry,aIsFilterPresent,aIsEntryRequested);       
                }
                break;
          case CLbtGeoAreaBase::ECellular:
               {
               ProcessCellFilter(aEntry,aIsFilterPresent,aIsEntryRequested);
               }
               break;
          case CLbtGeoAreaBase::EHybrid:
               {
               ProcessHybridFilter(aEntry,aIsFilterPresent,aIsEntryRequested);
               }
               break;
          default:
              {
              break;
              }
		    }
		}
	}
	
void CLbtContainerAreaFilter::ProcessCellFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested  )
	{
	FUNC_ENTER("CLbtContainerAreaFilter::ProcessCellFilter");
	
	CLbtGeoCell* cellArea=static_cast<CLbtGeoCell*>(iArea);
        
    // If the filter does not have a valid network mode then return ETrue
    //case only for simcard change from server
    //from client side, it is restricted
    if(cellArea->NetworkType() == RMobilePhone::ENetworkModeUnknown)
        {
        aIsFilterPresent++;
        aIsEntryRequested = ETrue;
        return;
        }
    
	if(aEntry!=NULL)
	    {	    
	    CLbtTriggerEntry* trigEntry=aEntry->TriggerEntry();
	    if(!trigEntry)
	    	{
	    	aIsFilterPresent = 0;
	    	return;
	    	}
	    CLbtTriggerConditionBase* condBase=trigEntry->GetCondition();
	    if(!condBase)
	    	{
	    	aIsFilterPresent = 0;
	    	return;
	    	}
	    	
	    CLbtTriggerConditionArea* trigCond = static_cast<CLbtTriggerConditionArea*> (condBase);
	    
	    
	    CLbtGeoAreaBase::TGeoAreaType trigAreaType =  trigCond->TriggerArea()->Type();
	    
	    //if other than cell/hybrid
	    if( trigAreaType != CLbtGeoAreaBase::ECellular && trigAreaType != CLbtGeoAreaBase::EHybrid )
	    	{
	    	// The area type is not the same. 
	    	aIsFilterPresent++;
			aIsEntryRequested = EFalse;
			return;
	    	}	    	
	    
	    //if cell
	    if( trigAreaType == CLbtGeoAreaBase::ECellular )
	        { 
	        CLbtGeoCell* trigArea=static_cast<CLbtGeoCell*>(trigCond->TriggerArea());
            CompareCells( trigArea,cellArea, aIsFilterPresent, aIsEntryRequested );
            return;
	        }
	    //hybrid
	    CLbtGeoHybrid* hybridArea=static_cast<CLbtGeoHybrid*>(trigCond->TriggerArea());
	    const RPointerArray<CLbtGeoAreaBase>& trigAreaArray = hybridArea->HybridArea();
        TInt cnt = trigAreaArray.Count();
        TInt index;
        CLbtGeoCell* trigArea = NULL;
        for(index=0;index<cnt;index++)
            {
            //Current support is only for cellular hybrid triggers
            if( trigAreaArray[index]->Type() != CLbtGeoAreaBase::ECellular )
                {
                continue;
                }
            trigArea = static_cast<CLbtGeoCell*> (trigAreaArray[index]);
            CompareCells( trigArea,cellArea, aIsFilterPresent, aIsEntryRequested );
            //if atleast one cell matches, return, else goto next value
            if( aIsEntryRequested ) return;
            }
	    }// end of if(aEntry!=NULL)
	}

void CLbtContainerAreaFilter::ProcessHybridFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested  )
    {
    FUNC_ENTER("CLbtContainerAreaFilter::ProcessHybridFilter");
    if(aEntry!=NULL)
        {       
        CLbtTriggerEntry* trigEntry=aEntry->TriggerEntry();
        if(!trigEntry)
           {
           aIsFilterPresent = 0;
           return;
           }
        CLbtTriggerConditionBase* condBase=trigEntry->GetCondition();
        if(!condBase)
           {
           aIsFilterPresent = 0;
           return;
           }
           
        CLbtTriggerConditionArea* trigCond = static_cast<CLbtTriggerConditionArea*> (condBase);
       
       
        CLbtGeoAreaBase::TGeoAreaType trigAreaType =  trigCond->TriggerArea()->Type();
       
        //if other than cell/hybrid
        if( trigAreaType != CLbtGeoAreaBase::ECellular && trigAreaType != CLbtGeoAreaBase::EHybrid )
           {
           // The area type is not the same. 
           aIsFilterPresent++;
           aIsEntryRequested = EFalse;
           return;
           }
       
        //start filtering
        CLbtGeoHybrid* hybridArea = static_cast<CLbtGeoHybrid*>(iArea);
        const RPointerArray<CLbtGeoAreaBase>& hybridAreaArray = hybridArea->HybridArea();
        TInt cnt = hybridAreaArray.Count();
        TInt index;
        CLbtGeoCell* hybridCellArea = NULL;
        CLbtGeoCell* trigCellArea = NULL;
        for(index=0;index<cnt;index++)
            {      
            //Current support is only for cellular hybrid triggers -check for dest
            if( hybridAreaArray[index]->Type() != CLbtGeoAreaBase::ECellular )
                {
                continue;
                }    
            hybridCellArea = static_cast<CLbtGeoCell*> (hybridAreaArray[index]);
            
            if( trigAreaType == CLbtGeoAreaBase::ECellular ) //source
                {   
                trigCellArea=static_cast<CLbtGeoCell*>(trigCond->TriggerArea());                
                CompareCells( trigCellArea,hybridCellArea, aIsFilterPresent, aIsEntryRequested );
                if(  aIsFilterPresent > 0 && aIsEntryRequested  ) return;
                }
            else
                {
                CLbtGeoHybrid* trigHybridArea=static_cast<CLbtGeoHybrid*>(trigCond->TriggerArea());
                const RPointerArray<CLbtGeoAreaBase>& trigHybridAreaArray = trigHybridArea->HybridArea();
                TInt count = trigHybridAreaArray.Count();
                TInt indexTrig;                
                for(indexTrig=0;indexTrig<count;indexTrig++)
                    {
                    //Current support is only for cellular hybrid triggers -check for source
                   if( trigHybridAreaArray[indexTrig]->Type() != CLbtGeoAreaBase::ECellular )
                       {
                       continue;
                       }                   
                   trigCellArea = static_cast<CLbtGeoCell*> (trigHybridAreaArray[indexTrig]);
                   CompareCells( trigCellArea, hybridCellArea, aIsFilterPresent, aIsEntryRequested );
                   //if atleast one cell matches, return, else goto next value
                   if( aIsFilterPresent > 0 && aIsEntryRequested ) 
                       {
                       return;
                       }
                    } //inner for
                }//else  trigAreaType == CLbtGeoAreaBase::ECellular
            }   //outer for     
        }// end of if(aEntry!=NULL)
    }
    
void CLbtContainerAreaFilter::ProcessCircleFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested  )
	{
	FUNC_ENTER("CLbtContainerAreaFilter::ProcessCircleFilter");
	// Add cicular filtering here.	
	if(aEntry!=NULL)
	    {	    
	    CLbtTriggerEntry* trigEntry=aEntry->TriggerEntry();
        if(!trigEntry)
        	{
        	//CHECK HOW TO HANDLE THIS SCENARIO
        	aIsFilterPresent = 0;
        	return;
        	}

        CLbtTriggerConditionBase* condBase=trigEntry->GetCondition();
        if(!condBase)
        	{
        	//CHECK HOW TO HANDLE THIS SCENARIO
        	aIsFilterPresent = 0;
        	return;
        	}

        CLbtTriggerConditionArea* trigCond=(CLbtTriggerConditionArea*)condBase;
        CLbtGeoCircle* trigArea=static_cast<CLbtGeoCircle*>(trigCond->TriggerArea());
        if( trigArea->Type() != CLbtGeoAreaBase::ECircle )
	    	{
	    	// The area type is not the same. 
	    	aIsFilterPresent++;
			aIsEntryRequested = EFalse;
			return;
	    	}	    	
        
    	CLbtGeoCircle* circArea=static_cast<CLbtGeoCircle*>(iArea);
    	TCoordinate filterCenter=circArea->Center();
    	TCoordinate trigCenter=trigArea->Center();
    	TReal filterRadius=circArea->Radius();
        TReal trigRadius=trigArea->Radius();
    	TReal32 distance;
    	filterCenter.Distance(trigCenter,distance);
    	
    	/* If the distance between the trigger center and the filter center
    	 * is less than the radius of the filter circle
    	 * and if this distance + the radius of the trigger
    	 * is less than the radius of the filter 
    	 * then the given trigger lies within the specified area
    	 * and will be returned back to the client
    	 */
    	if((distance<filterRadius) && ((distance+trigRadius)<=filterRadius))
    	    {
    	    aIsFilterPresent++ ;
    	    aIsEntryRequested = ETrue;
    	    }
    	else
    		{
    		aIsEntryRequested = EFalse;
    		}
	    }
	}
	
void CLbtContainerAreaFilter::ProcessRectFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested  )
	{
	FUNC_ENTER("CLbtContainerAreaFilter::ProcessRectFilter");
	// Add rectangular filtering here.
	if(aEntry != NULL)
		{
		CLbtGeoRect* rectArea=static_cast<CLbtGeoRect*>(iArea);
    	TReal northLat,southLat,eastLong,westLong;
    	rectArea->GetBorder(southLat,northLat,westLong,eastLong);
    	CLbtExtendedTriggerInfo* contExtInfo = aEntry->ExtendedTriggerInfo();
    	CLbtExtendedTriggerInfo::TLbtTriggerRectArea trigArea=contExtInfo->TriggerReactangleArea();
    	if( trigArea.iTrigAreaNorthLat <= northLat && trigArea.iTrigAreaSouthLat >= southLat &&
    	    trigArea.iTrigAreaEastLong <= eastLong && trigArea.iTrigAreaWestLong >= westLong )
    		{
    		aIsFilterPresent++;
    		aIsEntryRequested = ETrue;
    		}	
		}
		
	}	
	
void CLbtContainerAreaFilter::ProcessContFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested  )
	{
	FUNC_ENTER("CLbtContainerAreaFilter::ProcessContFilter");
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
                     
                    
                    if((isFilterPresent>0 && isEntryRequested ) || ( isFilterPresent==0 ))  
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
                    
                    
                    }
                     
                    // end of if(contExtInfo!=NULL)
       aIsFilterPresent = isFilterPresent;
       aIsEntryRequested = isEntryRequested;
	}
void CLbtContainerAreaFilter::RetrieveFilterDataL()
	{
	FUNC_ENTER("CLbtContainerAreaFilter::RetrieveFilterDataL");
	CLbtContainerFilter* contFilter = ContFilter();
	CLbtTriggerFilterBase* trigFilter = TrigFilter();
	CLbtTriggerFilterByArea* areaFilter = static_cast<CLbtTriggerFilterByArea*>(trigFilter);
    iArea = areaFilter->Area();
	
	if(contFilter)
       {
        contFilter->GetSidArrayL(&iSidArray) ;
        contFilter->GetHystRadiusArrayL(&iHystRadiusArray); 
        contFilter->GetRectAreaArrayL(&iTriggerRectAreaArray) ;
        contFilter->GetTriggerFiredArrayL(&iTriggerFiredArray) ;
        contFilter->GetTriggerFireOnCreationArrayL( &iTriggerFireOnCreationArray );
        }
       
	}

void CLbtContainerAreaFilter::CompareCells( CLbtGeoCell* aCellToBeMatched,  CLbtGeoCell* aCellFilter, TInt& aIsFilterPresent,TBool& aIsEntryRequested )
    { 
    if(aCellToBeMatched->NetworkType() != aCellFilter->NetworkType())
        {
        aIsFilterPresent++;
        aIsEntryRequested = EFalse;
        return ;
        }
    else
        {
        aIsFilterPresent++;
        TLex lex( aCellFilter->NetworkCountryCode() );
        TUint netCountryCode;
        lex.Val( netCountryCode );
        
        TLex lexTrig( aCellToBeMatched->NetworkCountryCode() );
        TUint trigNetCountryCode;
        lexTrig.Val( trigNetCountryCode );
        
        if(netCountryCode == 0 )
            {
            aIsEntryRequested=ETrue;
            return;
            }
        else
            {
            if(netCountryCode != trigNetCountryCode)                
                {
                aIsFilterPresent++;
                aIsEntryRequested = EFalse;
                return;
                }
                
            if(netCountryCode == trigNetCountryCode)
                {
                aIsEntryRequested = ETrue;
                aIsFilterPresent++;
                TLex idcode( aCellFilter->NetworkIdentityCode() );
                TUint netIdCode;
                idcode.Val( netIdCode );
                
                TLex trigIdcode( aCellToBeMatched->NetworkIdentityCode() );
                TUint trigNetIdCode;
                trigIdcode.Val( trigNetIdCode );    
                if(netIdCode == 0 )    
                    {
                    aIsEntryRequested = ETrue;
                    return;
                    }
                else
                    {
                    if(netIdCode != trigNetIdCode)
                        {
                        aIsFilterPresent++;
                        aIsEntryRequested = EFalse;
                        return;
                        }
                    if(netIdCode == trigNetIdCode)  
                        {   
                        aIsFilterPresent++;
                        aIsEntryRequested = EFalse;
                        if(aCellFilter->LocationAreaCode() == 0)   
                            {
                            aIsEntryRequested = ETrue;
                            return;
                            }
                        else
                            {
                            if(aCellFilter->LocationAreaCode() != aCellToBeMatched->LocationAreaCode())
                                {
                                aIsFilterPresent++;
                                aIsEntryRequested = EFalse;
                                return;
                                }   
                            if(aCellFilter->LocationAreaCode() == aCellToBeMatched->LocationAreaCode())
                                {
                                aIsFilterPresent++;
                                aIsEntryRequested = ETrue;
                                if(aCellFilter->CellId() == 0)
                                    {
                                    aIsEntryRequested = ETrue;
                                    return ;
                                    }
                                else if(aCellFilter->CellId() != aCellToBeMatched->CellId())
                                    {
                                    aIsFilterPresent++;
                                    aIsEntryRequested = EFalse;
                                    return ;
                                    }                                 
                                }// if(cellArea->LocationAreaCode() == trigArea->LocationAreaCode())    
                            } // end of else if(cellArea->LocationAreaCode() == 0)                       
                        }// end of if(netIdCode == trigNetIdCode)                       
                    }// end of else     if(netIdCode == 0 )                 
                } //end of  if(netCountryCode == trigNetCountryCode)    
            }// end of else after if(netCountryCode == 0 )
        }// end of if(trigArea->NetworkType() == cellArea->NetworkType())
    }

CLbtContainerAreaFilter::CLbtContainerAreaFilter()
	{
	
	}


CLbtContainerAreaFilter::~CLbtContainerAreaFilter()
	{
	FUNC_ENTER("CLbtContainerAreaFilter::~CLbtContainerAreaFilter");
	iHystRadiusArray.Close();
	iTriggerRectAreaArray.Close();
	iTriggerFiredArray.Close();
	iSidArray.Close();
	iTriggerFireOnCreationArray.Close();
	
	}
 
void CLbtContainerAreaFilter::ConstructL(CLbtTriggerFilterBase* aTrigFilter,CLbtContainerFilter* aContFilter)
	{
	FUNC_ENTER("CLbtContainerAreaFilter::ConstructL");
	SetTrigFilter(aTrigFilter);
	SetContFilter(aContFilter);
	RetrieveFilterDataL();
	}

