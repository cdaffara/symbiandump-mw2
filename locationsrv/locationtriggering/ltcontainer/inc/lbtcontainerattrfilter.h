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


#ifndef C_LBT_CONTAINER_ATTR_FILTER_H
#define C_LBT_CONTAINER_ATTR_FILTER_H

#include "lbtcontainerfilterbase.h"


   

       	
/* This is the  base class for performing all filtering related operations in the contianer */
       	
NONSHARABLE_CLASS( CLbtContainerAttrFilter ) : public CLbtContainerFilterBase
    {
public:
  
        
      void ProcessFilter(CLbtContainerTriggerEntry* aEntry,TInt& aIsFilterPresent,TBool& aIsEntryRequested) ;
      
      void RetrieveFilterDataL();
     
    /**
     * Destructor
     */
      ~CLbtContainerAttrFilter();
        

         /**
     * Constructs a new instance of container trigger entry object.
     *
     * @return The new instance of the container trigger entry object.
     */
     static CLbtContainerAttrFilter* NewL(CLbtTriggerFilterBase* aTrigFilter,CLbtContainerFilter* aContFilter);
      CLbtContainerAttrFilter();
        
	
private:
    
    void ConstructL(CLbtTriggerFilterBase* aTrigFilter,CLbtContainerFilter* aContFilter);
    TBool FilesIdentical( TFileName& aTriggerFileName, TFileName& aSystemFileName );
    TBool DoesFileContainePathInfo( TFileName& aFileName );
       
private:
    
   	RArray<TReal> iHystRadiusArray;
	RArray<CLbtExtendedTriggerInfo::TLbtTriggerRectArea> iTriggerRectAreaArray;
	RArray<TBool> iTriggerFiredArray;
	RArray<TSecureId> iSidArray;
	RArray<TBool> iTriggerFireOnCreationArray;
	RArray<CLbtTriggerEntry::TType> iTriggerTypeArray;
    RArray<CLbtTriggerEntry::TLbtTriggerState> iTriggerStateArray;
    RArray<TLbtTriggerDynamicInfo::TLbtTriggerValidity> iTriggerValidityArray;
    RArray<TUid> iManagerUiArray;
    RArray<TLbtTriggerId > iIdArray; 
    RArray<TFileName> iStartupProcessArray;
    };


#endif // C_LBT_CONTAINER_ATTR_FILTER_H
