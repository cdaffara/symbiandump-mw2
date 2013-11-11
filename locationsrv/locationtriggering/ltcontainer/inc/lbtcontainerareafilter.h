/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class for attribute filtering 
*
*/


#ifndef C_LBT_CONTAINER_AREA_FILTER_H
#define C_LBT_CONTAINER_AREA_FILTER_H

#include "lbtcontainerfilterbase.h"
#include "lbtcontainerextendedtriggerinfo.h"

class CLbtGeoAreaBase;
class CLbtGeoCell;
       	
/* This is the  base class for performing all filtering related operations in the contianer */
       	
NONSHARABLE_CLASS( CLbtContainerAreaFilter ) : public CLbtContainerFilterBase
    {
public:
  
        
      void ProcessFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested) ;
      
      void RetrieveFilterDataL();
     
    /**
     * Destructor
     */
      ~CLbtContainerAreaFilter();
        

         /**
     * Constructs a new instance of container trigger entry object.
     *
     * @return The new instance of the container trigger entry object.
     */
     static CLbtContainerAreaFilter* NewL(CLbtTriggerFilterBase* aTrigFilter,CLbtContainerFilter* aContFilter);
      CLbtContainerAreaFilter();
        
	
private:
    
    void ConstructL(CLbtTriggerFilterBase* aTrigFilter,CLbtContainerFilter* aContFilter);          
    
    void ProcessCircleFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested) ;   
    
    void ProcessRectFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested) ;   
    
    void ProcessContFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested  );
    void ProcessCellFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested);
    void ProcessHybridFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested  );
    
    void CompareCells( CLbtGeoCell* aCellToBeMatched,  CLbtGeoCell* aCellFilter, TInt& aIsFilterPresent,TBool& aIsEntryRequested );
private:
    
   	RArray<TReal> iHystRadiusArray;
	RArray<CLbtExtendedTriggerInfo::TLbtTriggerRectArea> iTriggerRectAreaArray;
	RArray<TBool> iTriggerFiredArray;
	RArray<TSecureId> iSidArray;
	RArray<TBool> iTriggerFireOnCreationArray;
	CLbtGeoAreaBase* iArea; 
    };


#endif // C_LBT_CONTAINER_AREA_FILTER_H
