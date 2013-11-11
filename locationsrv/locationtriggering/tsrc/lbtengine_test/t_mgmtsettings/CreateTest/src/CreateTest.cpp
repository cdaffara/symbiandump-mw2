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
* Description:  CreateTest.cpp
*
*/


#include <e32property.h>

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
void createtriggerL();
GLDEF_C	 TInt E32Main()
	 {
	 CTrapCleanup* cleanup=CTrapCleanup::New(); // get clean-up stack
	 if (!CActiveScheduler::Current())
		{
		   TRAP_IGNORE(CActiveScheduler* scheduler=new(ELeave) CActiveScheduler;CActiveScheduler::Install(scheduler););
		   
		}
	 TRAP_IGNORE(createtriggerL());
	 RProcess::Rendezvous(KErrNone);
    return 0;
  }
  
  void createtriggerL()
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
    _LIT( KMyTriggerHandlingProcessName, "About.exe" );
    
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
    //set Requestor     
    CRequestorBase::TRequestorType ReqType=CRequestorBase::ERequestorUnknown;
	CRequestorBase::_TRequestorFormat ReqFormat=CRequestorBase::EFormatUnknown;
	TBuf<KLbtMaxNameLength> ReqData=_L("");
	trig->SetRequestorL(ReqType,ReqFormat,ReqData);
	
    trig->SetCondition(condition); // ownership transferred to object

    TLbtTriggerId trigId;
     trig->SetManagerUi(aManagerUi);   
        
    CTriggerFireObserver* notifier= CTriggerFireObserver::NewL( lbt,coordinate );
    CleanupStack::PushL( notifier );
    
    CActiveSchedulerWait* wait=new(ELeave)CActiveSchedulerWait;
        
    notifier->CreateTriggers( lbt,*trig,trigId,EFalse,wait );
    wait->Start( );	
    RProperty property;
	CleanupClosePushL(property);
	
	//Allow all to ready from status information
	_LIT_SECURITY_POLICY_PASS(EReadPolicyAlwaysPass);
	//Delete the property if already exists
	/*
	 User::LeaveIfError(property.Delete(
        KPSUidTriggerIdInfo, 
        KLbttesttriggerid 
        ));*/
	
	// Read policy is always pass and write device data capability
	// is required to write to the status information P&S key
	property.Define(KPSUidTriggerIdInfo,
									   KLbttesttriggerid,
									   RProperty::EInt,
									   EReadPolicyAlwaysPass,
									   TSecurityPolicy(ECapabilityWriteDeviceData) );
	

	User::LeaveIfError( property.Attach(KPSUidTriggerIdInfo, 
										KLbttesttriggerid) );
										
	User::LeaveIfError( property.Set(KPSUidTriggerIdInfo, 
									 KLbttesttriggerid, 
									 trigId));
	CleanupStack::PopAndDestroy(5,&lbtserver);
}
