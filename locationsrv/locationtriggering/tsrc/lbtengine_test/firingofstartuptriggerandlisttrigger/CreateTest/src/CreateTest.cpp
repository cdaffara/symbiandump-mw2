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
* Description:    Test module to implement Firing of startup trigger and Listing of trigger related test cases
*
*/



#include <lbtcommon.h>
#include <lbterrors.h>
#include <lbtgeoareabase.h>
#include <lbtgeocircle.h>
#include <lbtgeorect.h>
#include <lbtlisttriggeroptions.h>
#include <lbtsessiontrigger.h>
#include <lbtstartuptrigger.h>
#include <lbtserver.h>
#include <lbttriggerchangeevent.h>
#include <lbttriggerchangeeventnotifier.h>
#include <lbttriggerchangeeventobserver.h>
#include <lbttriggerconditionarea.h>
#include <lbttriggerconditionbase.h>
#include <lbttriggerdynamicinfo.h>
#include <lbttriggerentry.h>

#include "t_triggerfireobserver.h"	 
TInt createtriggerL();
GLDEF_C	 TInt E32Main()
	 {
	 CTrapCleanup* cleanup=CTrapCleanup::New(); // get clean-up stack
	 if (!CActiveScheduler::Current())
		{
		   TRAP_IGNORE(CActiveScheduler* scheduler=new(ELeave) CActiveScheduler;
		   CActiveScheduler::Install(scheduler););
		}
	 TLbtTriggerId trigId = KLbtNullTriggerId;
	 TRAP_IGNORE(trigId = createtriggerL());
	 RProcess::Rendezvous(trigId);
    return 0;
  }
  
  TInt createtriggerL()
  {
  	RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
 	
 	 
	  //Construct a startup trigger
    CLbtStartupTrigger* trig = CLbtStartupTrigger::NewL();
    
    //Push to cleanup stack
    CleanupStack::PushL( trig );
    
    // Set Name
    trig->SetNameL(_L("Trigger1"));
    _LIT( KMyTriggerHandlingProcessName, "LbtReference.exe" );
    
    TSecureId secureid;
    trig->SetProcessId(KMyTriggerHandlingProcessName,secureid);
       
    // set condition
    
    TCoordinate coordinate(62.5285,23.9385);
    
    CLbtGeoCircle* circle=CLbtGeoCircle::NewL(coordinate,1000);
    CleanupStack::PushL( circle );
    
    TUid aManagerUi = TUid::Uid(0x87654321);     
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* condition=CLbtTriggerConditionArea::NewL(
                                                circle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( circle );
    
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );	
    CleanupStack::PopAndDestroy(4,&lbtserver);
    return trigId;
  }