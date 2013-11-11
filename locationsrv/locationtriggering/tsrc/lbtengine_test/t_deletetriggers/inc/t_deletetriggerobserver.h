/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Test module to implement Delete trigger related test cases
*
*/





#ifndef T_DELETETRIGGEROBSERVER_H
#define T_DELETETRIGGEROBSERVER_H

  //  INCLUDES
  #include <e32base.h>
  #include <lbtcommon.h> 
  

  // FORWARD DECLARATIONS
  
      
  class RLbt;
  class CLbtTriggerFilterBase;
 
  
  class CDeleteTriggerObserver: public CActive
    
    {
    
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CDeleteTriggerObserver* NewL();

              
        /**
        * Destructor.
        */
        ~CDeleteTriggerObserver();
        
        
        
        
        void RunL( );
        
	    void DoCancel( );
	    
	    void DeleteFilteredTriggers( RLbt& aLbt,
	                                 CLbtTriggerFilterBase* aFilter,
	                                 CActiveSchedulerWait* aWait 
	                               );
	    
	    void DeleteTriggersById( RLbt& aLbt,
	                             const RArray< TLbtTriggerId >& aTriggerIdList,
	                             CActiveSchedulerWait* aWait 
	                           );
	                           
	                           
	    void DeleteAllTriggers( RLbt& aLbt,
	                         CActiveSchedulerWait* aWait 
	                       );
	    
	    TInt CompletionCode();
	    


    public:
           
    
    private:

        /**
        * C++ default constructor.
        */
        CDeleteTriggerObserver( );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( );
        
    private:
         
        CActiveSchedulerWait* iWait;
        TInt  iCompletionCode; 
       

    };

#endif      // T_DELETETRIGGEROBSERVER_H

// End of File
