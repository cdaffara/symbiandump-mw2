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
* Description:  Abstract base class for filtering operations
*
*/


#ifndef C_LBT_CONTAINER_FILTER_BASE_H
#define C_LBT_CONTAINER_FILTER_BASE_H

class CLbtTriggerFilterBase;
class CLbtContainerFilter;
class CLbtContainerTriggerEntry;

   

       	
/* This is the  base class for performing all filtering related operations in the contianer */
       	
NONSHARABLE_CLASS( CLbtContainerFilterBase ) : public CBase
    {
public:
  
        
     virtual void ProcessFilter(CLbtContainerTriggerEntry* aEntry, TInt& aIsFilterPresent, TBool& aIsEntryRequested) = 0;
     
      void SetTrigFilter(CLbtTriggerFilterBase* aTrigFilter);
     
      void SetContFilter(CLbtContainerFilter*  aContFilter);
     
      CLbtContainerFilter* ContFilter();
     
      CLbtTriggerFilterBase* TrigFilter();
   
      CLbtContainerFilterBase( CLbtContainerFilterBase& aEntry);
      
      CLbtContainerFilterBase()
        {
        
        }
        
      ~CLbtContainerFilterBase();  

private:
    
    CLbtTriggerFilterBase* iTrigFilter;
    CLbtContainerFilter*	  iContFilter;
    };


#endif // C_LBT_CONTAINER_FILTER_BASE_H
