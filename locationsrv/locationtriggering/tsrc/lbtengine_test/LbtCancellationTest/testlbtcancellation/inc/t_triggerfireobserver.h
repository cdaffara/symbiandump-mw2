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
* Description:    Test module to implement Cancellation of asynchronous requests related test cases
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

#include <lbtmanager.h> 
#include <lbttriggeringsystemsettings.h>
#include <lbttriggeringsystemmanagementsettings.h>
#include <lbttriggeringsystemstatus.h>
  	

  // FORWARD DECLARATIONS
  
  class RLbt; 
  class CLbtTriggerFiringEventNotifier;
  class CLbtTriggerEntry;
  class CLbtTriggerInfo;
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
        void DeleteTriggers(RLbt& aLbt,CLbtTriggerFilterBase* aFilter,CActiveSchedulerWait* wait);                   
        void DeleteTriggers(RLbt& aLbt,RArray<TLbtTriggerId>& aTriggerArray,CActiveSchedulerWait* aWait);
        void SetTriggersState(RLbt& aLbt,CLbtTriggerFilterBase* aFilter,CLbtTriggerEntry::TLbtTriggerState aState,CActiveSchedulerWait* aWait);
        void GetTriggersList(RLbt& aLbt, RPointerArray < CLbtTriggerInfo >& aTriggerList,
        			CActiveSchedulerWait* aWait ,CLbtListTriggerOptions* aListOptions=NULL) ;  
        void ListTriggerIds(RLbt& aLbt,RArray < TLbtTriggerId >& aTriggerIdArray,CActiveSchedulerWait* aWait); 
           
       void ListTriggerIdsList(RLbt& aLbt, RArray < TLbtTriggerId >& aTriggerIdList,
            CActiveSchedulerWait* aWait,CLbtListTriggerOptions* aListOptions=NULL);                          
        void CreateIterator(RLbt& aLbt,CActiveSchedulerWait* aWait,CLbtListTriggerOptions* aListOptions=NULL);    
        TInt GetFireCount( ) ; 
        void Setwait(CActiveSchedulerWait* aWait  ) ;
        void NotifyTriggerFired(RLbt& aLbt, TLbtTriggerFireInfo& aFireInfo,CActiveSchedulerWait* aWait);
        TLbtTriggerFireInfo GetFiredTrigger( ) ;    
        void GetTriggers(RLbt& aLbt,RPointerArray < CLbtTriggerInfo >& aTriggerIdArray,CActiveSchedulerWait* aWait);          
        void CreateGetTriggerIterator(RLbt& aLbt,CActiveSchedulerWait* aWait);
   	    void NotifyTriggerChangeEvent(RLbt& aLbt,TLbtTriggerChangeEvent& aEvent,CActiveSchedulerWait* aWait);
        void RunL( );
        void NotifyTriggeringSystemSettingChange(RLbt& aLbt,TLbtTriggeringSystemSettings& aSettings,CActiveSchedulerWait* aWait);
        void UpdateTrigger(RLbt& aLbt,CLbtTriggerEntry& aTrigger,TLbtTriggerAttributeFieldsMask aFieldMask,CActiveSchedulerWait* aWait,
            TLbtFireOnUpdate aFireOnUpdate) ;     
        void NotifyTriggeringSettingsChange( RLbt& aLbt,
                                             TLbtTriggeringSystemSettings &  aSettings,
                                             TLbtTriggeringSystemSettings dummySettings, 
                                             CActiveSchedulerWait* aWait = NULL
                                           ); 
        void NotifyTriggeringSettingsChange( RLbtManager& aLbtmanager,
                                                    TLbtTriggeringSystemManagementSettings &  aSettings,
                                                     TLbtTriggeringSystemSettings dummySettings, 
                                                     CActiveSchedulerWait* aWait = NULL
                                                   ); 
        void NotifyTriggeringSystemStatusChange( RLbtManager& aLbtmanager,CActiveSchedulerWait* aWait );
	    void DoCancel( );
		
	//	  RTimer iTimer;
        TRequestStatus iWaitStatus;
        TInt iTriggerFireCount ;  
    private:

        /**
        * C++ default constructor.
        */
        CTriggerFireObserver( TCoordinate& aCoordinate,RLbt& aLbt );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( RLbt& aLbt );
        
    private:
    
        CLbtTriggerFiringEventNotifier* iNotifier;
        TCoordinate iCoordinate;
        CActiveSchedulerWait* iWait;  
        RLbt& iLbt;
        
        TLbtTriggerFireInfo iFireInfo;
        
      
       

    };

#endif      // T_TRIGGERFIREOBSERVER_H

// End of File
