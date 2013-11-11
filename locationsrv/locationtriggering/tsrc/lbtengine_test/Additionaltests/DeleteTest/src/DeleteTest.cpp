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


#include "t_triggerfireobserver.h"	 
void DeletetriggerL();
GLDEF_C	 TInt E32Main()
	 {
	 CTrapCleanup* cleanup=CTrapCleanup::New(); // get clean-up stack
	 if (!CActiveScheduler::Current())
		{
		   TRAP_IGNORE(CActiveScheduler* scheduler=new(ELeave) CActiveScheduler;CActiveScheduler::Install(scheduler););
		   
		}
	 TRAP_IGNORE(DeletetriggerL());
	 RProcess::Rendezvous(KErrNone);
    return 0;
  }
  
  void DeletetriggerL()
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
	lbt.DeleteTriggerL(triggerid);
	TInt result=0;
//	CLbtTriggerInfo* trigEntry=NULL;
	TRAPD(error1, lbt.GetTriggerLC(triggerid);CleanupStack::PopAndDestroy(););
	CleanupStack::PopAndDestroy(3,&lbtserver);
	if(error1==KErrNotFound)
	{
		result= KErrNone;
	
	}
		
	else
	{
		result= -99;
	
	}
	   
			// Read policy is always pass and write device data capability
	// is required to write to the status information P&S key
	property.Define(DeletetestUid,
									   KLbttesttriggerid,
									   RProperty::EInt,
									   EReadPolicyAlwaysPass,
									   TSecurityPolicy(ECapabilityWriteDeviceData) );
	

	User::LeaveIfError( property.Attach(DeletetestUid, 
										KLbttesttriggerid) );
										
	User::LeaveIfError(property.Set(
		        DeletetestUid, 
		        KLbttesttriggerid,
		        result));
  }