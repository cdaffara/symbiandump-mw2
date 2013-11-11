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
* Description:     Test module to implement Update trigger related test cases
*
*/






#ifndef T_TRIGGERFIREOBSERVER_H
#define T_TRIGGERFIREOBSERVER_H

  //  INCLUDES

  #include <lbtcommon.h>
  #include <lbttriggerfiringeventobserver.h>
  #include <lbttriggerfiringeventnotifier.h>
  #include <lbtsessiontrigger.h>
  #include <lbtstartuptrigger.h>
  #include <lbttriggerentry.h>		
  	#include <lbtgeocell.h>

  // FORWARD DECLARATIONS
  
  class RLbt; 
  class CLbtTriggerFiringEventNotifier;
  class CLbtTriggerEntry;
  class CLbtTriggerInfo;
//  class CLBTOperation;
  const TUid KPSUidTriggerIdInfo = { 0xA0000196 };  
  const TUint32 KLbttesttriggerid = 0x00000001; 
  
  class CTriggerFireObserver: public CTimer,MLbtTriggerFiringEventObserver
    
    {
    
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CTriggerFireObserver* NewL( RLbt& aLbt,TCoordinate& aCoordinate );

        /**
        * Destructor.
        */
        ~CTriggerFireObserver();
        
        
        void TriggerFiredL( const TLbtTriggerFireInfo& aFireInfo );
        
        void StartNotification( CActiveSchedulerWait* aWait );
        
        void CancelNotification( );
        
        void CreateTrigger( );
        
        void CheckPositionInfoL( const TLbtTriggerFireInfo& aFireInfo );
        
               
        void CreateTriggers( RLbt& aLbt,
                             CLbtTriggerEntry& aEntry,
                             TLbtTriggerId& aId,
                             TBool aFireOnCreation,
                             CActiveSchedulerWait* aWait
                           );
                           
        void GetTriggersList(RLbt& aLbt, RPointerArray < CLbtTriggerInfo >& aTriggerList,
           CActiveSchedulerWait* aWait ,CLbtListTriggerOptions* aListOptions=NULL) ;  
       void ListTriggerIdsList(RLbt& aLbt, RArray < TLbtTriggerId >& aTriggerIdList,
            CActiveSchedulerWait* aWait,CLbtListTriggerOptions* aListOptions=NULL);                          
        void UpdateTriggersState(RLbt& aLbt, CLbtTriggerEntry::TLbtTriggerState aState, 
            CActiveSchedulerWait* aWait,TLbtFireOnUpdate aFireOnUpdate,CLbtTriggerFilterBase *  aFilter=NULL) ;
       void UpdateTrigger(RLbt& aLbt,CLbtTriggerEntry& aTrigger,TLbtTriggerAttributeFieldsMask aFieldMask,CActiveSchedulerWait* aWait,
            TLbtFireOnUpdate aFireOnUpdate) ;     
        TInt GetFireCount( ) ; 
        
        TLbtTriggerFireInfo GetFiredTrigger( ) ;               
        
        void RunL( );
        
	    void DoCancel( );
//		CLBTOperation * CreateGeoAreaForCurrentLocationL(CLbtGeoAreaBase::TGeoAreaType aTechType,CLbtGeoAreaBase** aArea,CActiveSchedulerWait* aWait);

	//	  RTimer iTimer;
        TRequestStatus iWaitStatus;
        TInt iTriggerFireCount ;  
    private:

        /**
        * C++ default constructor.
        */
        CTriggerFireObserver( TCoordinate& aCoordinate );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( RLbt& aLbt );
        
    private:
    
        CLbtTriggerFiringEventNotifier* iNotifier;
        TCoordinate iCoordinate;
        CActiveSchedulerWait* iWait;  
        
        
        TLbtTriggerFireInfo iFireInfo;
        
      
       

    };

#endif      // T_TRIGGERFIREOBSERVER_H

// End of File
