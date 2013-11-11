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
#include <lbttriggerchangeeventnotifier.h>
#include <lbttriggerchangeeventobserver.h>


#include "t_triggerfireobserver.h"	 
void NotificationTestL();
GLDEF_C	 TInt E32Main()
	 {
	 CTrapCleanup* cleanup=CTrapCleanup::New(); // get clean-up stack
	 if (!CActiveScheduler::Current())
		{
		   TRAP_IGNORE(CActiveScheduler* scheduler=new(ELeave) CActiveScheduler;CActiveScheduler::Install(scheduler););
		   
		}
	 TRAP_IGNORE(NotificationTestL());

	 RProcess::Rendezvous(KErrNone);
    return 0;
  }
  
  void NotificationTestL()
  {
  		RLbtServer server;
	    RLbt lbt1;
	    
	    User::LeaveIfError(server.Connect());
	    CleanupClosePushL( server );
	    
	    User::LeaveIfError(lbt1.Open(server));
	    CleanupClosePushL( lbt1 );
	    
	    TCoordinate coordinate(0,0);
	    
	    CTriggerFireObserver* notifier1= CTriggerFireObserver::NewL( lbt1,coordinate);
	    CleanupStack::PushL( notifier1 );
	    //notifier->After(50000000); 
	    
	    CActiveSchedulerWait* wait1=new(ELeave)CActiveSchedulerWait;
//	    TLbtTriggerChangeEvent   aEvent ;
	    	    
	    notifier1->NotifyTriggerChangeEventL(lbt1,wait1);
	    wait1->Start( );
	   
	   
	    	CleanupStack::PopAndDestroy(notifier1);
		    CleanupStack::PopAndDestroy(&lbt1);
		    CleanupStack::PopAndDestroy(&server);
//		    delete wait1;
//		    delete wait2;
//		    return KErrNone;
  }