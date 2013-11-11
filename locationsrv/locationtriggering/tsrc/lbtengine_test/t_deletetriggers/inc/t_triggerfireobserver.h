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





#ifndef T_TRIGGERFIREOBSERVER_H
#define T_TRIGGERFIREOBSERVER_H

  //  INCLUDES

  #include <lbtcommon.h>
  #include <lbttriggerfiringeventobserver.h>
  #include <lbttriggeringsystemsettings.h>
	#include <lbt.h> 
	#include <lbttriggeringsystemsettings.h>
	#include <lbttriggeringsystemmanagementsettings.h>
	#include <lbttriggeringsystemstatus.h>


  // FORWARD DECLARATIONS
  
  class RLbt; 
  class CLbtTriggerFiringEventNotifier;
  class CLbtTriggerEntry;
  
     
  
  class CTriggerFireObserver: public CTimer,MLbtTriggerFiringEventObserver
    
    {
    
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CTriggerFireObserver* NewL( RLbt& aLbt,
                                           TCoordinate aCoordinate,
                                           TInt aRadiusInMetres );

              
        /**
        * Destructor.
        */
        ~CTriggerFireObserver();
        
        
        void TriggerFiredL( const TLbtTriggerFireInfo& aFireInfo );
        
        void StartNotification( CActiveSchedulerWait* aWait=NULL );
        
        void CancelNotification( );
        
        void CreateTrigger( );
        
        void CheckPositionInfoL( const TLbtTriggerFireInfo& aFireInfo );
        
               
        void CreateTriggers( RLbt& aLbt,
                             CLbtTriggerEntry& aEntry,
                             TLbtTriggerId& aId,
                             TBool aFireOnCreation,
                             CActiveSchedulerWait* aWait
                           );
        
        void NotifyTriggeringSettingsChange( RLbt& aLbt,
                                             TLbtTriggeringSystemSettings &  aSettings,
                                             TLbtTriggeringSystemSettings dummySettings, 
                                             CActiveSchedulerWait* aWait = NULL
                                           ); 
                                           
      /*  void NotifyTriggeringSettingsChange( RLbtManager& aLbt,
                                             TLbtTriggeringSystemManagementSettings &  aSettings,
                                             TLbtTriggeringSystemManagementSettings dummySettings, 
                                             CActiveSchedulerWait* aWait = NULL
                                           );          */                            

        
        void CancelNotifyTriggeringSettingsChange( RLbt& aLbt );
        
        void DeleteAllTriggers( RLbt& aLbt,CActiveSchedulerWait* aWait);
        void CreateIterator(RLbt& aLbt,CActiveSchedulerWait* aWait,CLbtListTriggerOptions* aListOptions=NULL);    
        
        void RunL( );
        
	    void DoCancel( );


    public:
           TBool iTriggerFired;
           TInt  iCompletionCode;
    
    private:

        /**
        * C++ default constructor.
        */
        CTriggerFireObserver( TCoordinate aCoordinate,TInt aRadiusInMetres );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( RLbt& aLbt );
        
    private:
    
        CLbtTriggerFiringEventNotifier* iNotifier;
        CActiveSchedulerWait* iWait;
        
        TLbtTriggeringSystemSettings  iSettings;
        
        TCoordinate iCoordinate; 
        TReal32     iRadiusInMetres;
        TBool       iSettingsChangeReqIssued;     
       

    };

#endif      // T_TRIGGERFIREOBSERVER_H

// End of File
