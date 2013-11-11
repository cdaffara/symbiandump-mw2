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
* Description:    Test module to implement Test cases found during review
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
#include <lbttriggerinfo.h>


#include "t_triggerfireobserver.h"	 
void ModifytriggerL();
GLDEF_C	 TInt E32Main()
	 {
	 CTrapCleanup* cleanup=CTrapCleanup::New(); // get clean-up stack
	 if (!CActiveScheduler::Current())
		{
		   TRAP_IGNORE(CActiveScheduler* scheduler=new(ELeave) CActiveScheduler;CActiveScheduler::Install(scheduler););
		   
		}
	 TRAP_IGNORE(ModifytriggerL());
	 RProcess::Rendezvous(KErrNone);
    return 0;
  }
  
  void ModifytriggerL()
  {
  	RLbtServer lbtserver;
 	 RLbt lbt;
 	 
 	 User::LeaveIfError( lbtserver.Connect() );
     CleanupClosePushL( lbtserver );
 	 User::LeaveIfError( lbt.Open( lbtserver ) );
 	 CleanupClosePushL( lbt );
 	 
    RProperty property;
	CleanupClosePushL(property);
	TInt triggerid=0;
	User::LeaveIfError( property.Get(KPSUid, 
										KLbttriggerid,
										 triggerid));
	
	//Allow all to ready from status information
	_LIT_SECURITY_POLICY_PASS(EReadPolicyAlwaysPass);
	//Delete the property if already exists
	/*
	 User::LeaveIfError(property.Delete(
        KPSUidTriggerIdInfo, 
        KLbttesttriggerid 
        ));*/
	//**************New Trigger Entry**************/ 
    CLbtStartupTrigger* newtrig = CLbtStartupTrigger::NewL();
    newtrig->SetNameL(_L("TriggerTest"));
    newtrig->SetState(CLbtTriggerEntry::EStateDisabled);
    newtrig->SetCommandLineL(_L("Commandline2" ));
    TCoordinate newcoordinate(0,0);
    
    CLbtGeoCircle* newcircle=CLbtGeoCircle::NewL(newcoordinate,500);
    CleanupStack::PushL( newcircle );
         
    // ownership of circle object transferred to the condition object
    CLbtTriggerConditionArea* newcondition=CLbtTriggerConditionArea::NewL(
                                                newcircle,
                                                CLbtTriggerConditionArea::EFireOnEnter);
        
    CleanupStack::Pop( newcircle );
    
    newtrig->SetCondition(newcondition); // ownership transferred to object
//	trigId = 1;
    newtrig->SetId(triggerid);
   
    /***************************End*******************/
    lbt.UpdateTriggerL(*newtrig,CLbtTriggerEntry::EAttributeName
    |CLbtTriggerEntry::EAttributeState
    |CLbtTriggerEntry::EAttributeCondition
    |CLbtTriggerEntry::EAttributeStartUpCommandLine,
    ELbtTrue
    );
   // lbt.UpdateTriggerL(*newtrig);
    
	TInt result=0;
	CLbtTriggerInfo *Triginfo = CLbtTriggerInfo::NewL();
    Triginfo = lbt.GetTriggerLC(triggerid);
    CLbtTriggerEntry *TrigEntry = Triginfo->TriggerEntry();
    
    TLbtTriggerDynamicInfo * dyninfo = Triginfo -> DynInfo() ;
    //Check the name of the trigger
    TDesC Name2 =TrigEntry->Name(); 
    if(TrigEntry->Name()!=newtrig->Name()) 
    {
    	result= -99;
    }
    //Check the state of the trigger
    CLbtTriggerEntry::TLbtTriggerState State2 = TrigEntry->State() ;  
    if(State2 !=newtrig->State())
    {
    	result= -99;
    }
    //Check type of the trigger
    TInt Type2 = TrigEntry ->Type();
    if(Type2 != newtrig->Type())
    {
    	result= -99;
    }
    //Check Condition of the trigger
	CLbtTriggerConditionArea* Condition2 = static_cast <CLbtTriggerConditionArea*>(TrigEntry->GetCondition());
	CLbtTriggerConditionArea::TDirection direction;
	direction = Condition2->Direction();
	if(direction!=CLbtTriggerConditionArea::EFireOnEnter)
	{
		result= -99;
	}
	CLbtGeoAreaBase* area = Condition2->TriggerArea();
	if(area ->Type()!=CLbtGeoAreaBase::ECircle )
	{
		result= -99;
	}
	
	CLbtGeoCircle* circle2 = static_cast <CLbtGeoCircle*>(area);
	TCoordinate center = newcircle->Center();
	TCoordinate center2 = circle2->Center();
	TReal32 distance;
	center2.Distance(center,distance);
	if(distance!=0)
	{
		result= -99;
	}
	TReal radius = circle2->Radius();
	if(radius != newcircle->Radius())
	{
		result= -99;
	}
	if((static_cast <CLbtStartupTrigger*>(TrigEntry))->CommandLine()!=newtrig->CommandLine()) 
    {
    	result= -99;
    }
	
	   
			// Read policy is always pass and write device data capability
	// is required to write to the status information P&S key
	property.Define(ModifytestUid,
									   KLbttesttriggerid,
									   RProperty::EInt,
									   EReadPolicyAlwaysPass,
									   TSecurityPolicy(ECapabilityWriteDeviceData) );
	

	User::LeaveIfError( property.Attach(ModifytestUid, 
										KLbttesttriggerid) );
										
	User::LeaveIfError(property.Set(
		        ModifytestUid, 
		        KLbttesttriggerid,
		        result));
		
	
    CleanupStack::PopAndDestroy(4,&lbtserver);
    
  }