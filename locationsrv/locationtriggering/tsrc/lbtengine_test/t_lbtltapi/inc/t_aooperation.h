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
* Description:    Test module to implement Test cases for C,T Classes
*
*/





#ifndef T_AOOPERATION_H
#define T_AOOPERATION_H

  //  INCLUDES

#include <e32base.h>

#include <lbtcommon.h>
#include <lbttriggerentry.h> 
  
  // FORWARD DECLARATIONS
 
  class CLbtTriggerFilterBase;
  class CLbtListTriggerOptions;
  class CLbtTriggerInfo;
  class RLbt;
   
  const TUid KPSUidTriggerIdInfo = { 0xA0000196 };  
  const TUint32 KLbttesttriggerid = 0x00000001;  
  
  class CAoOperation: public CActive
    
    {
    
    public:  // Constructors and destructor

        enum TRequestType
            {
            	ESetTriggersState=0,
            	EListTriggerIds,
            	EGetTriggers
            	
            };
        
        
        /**
        * Two-phased constructor.
        */
        static CAoOperation* NewL();

              
        /**
        * Destructor.
        */
        ~CAoOperation();
                      
        
         void RunL( );
        
	     void DoCancel( );
	    
	     void SetRequestType( TRequestType aRequestType);
	     
	     void SetTriggerState( RLbt& aLbt,
	                           CLbtTriggerEntry::TLbtTriggerState aState,
	                           CLbtTriggerFilterBase* aFilter,
	                           CActiveSchedulerWait* aWait=NULL   );   

        void ListTriggerIds( RLbt& aLbt,
                             RArray < TLbtTriggerId >& aTriggerIdList, 
                             CLbtListTriggerOptions* aListOptions,
                             CActiveSchedulerWait* aWait=NULL
                           ); 
                           
        void  GetTriggers(   RLbt& aLbt,
                             RPointerArray < CLbtTriggerInfo >& aTriggerList,
                             CLbtListTriggerOptions* aListOptions,
                             CActiveSchedulerWait* aWait=NULL
                          );                    
    
    
    public:
            TInt  iCompletionCode;
            
                
    private:

        /**
        * C++ default constructor.
        */
        CAoOperation( );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( );
        
    private:
    
         CActiveSchedulerWait* iWait;
         TRequestType iRequestType;
        
    };

#endif      // T_AOOPERATION_H

// End of File
