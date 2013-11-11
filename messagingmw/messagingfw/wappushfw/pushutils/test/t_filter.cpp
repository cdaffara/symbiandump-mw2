// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//


    
#include <e32std.h>
#include <e32test.h>
#include <pi.h>
#include <wapengstd.h>

#include <push/pushmessage.h>
#include "wappushfilter.h"

RTest test(_L("Push Filter Test"));


LOCAL_C createSchedulerLC()
	{
	
	CActiveScheduler* scheduler=new(ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	// install the sceduler
	CActiveScheduler::Install(scheduler);
	}


LOCAL_C doTestsL()
	{
	test.Start(_L("@SYMTestCaseID IWS-WAPBROWSER-PUSHUTILS-T_FILTER-0001 Start Push Filter Test"));
			
	createSchedulerLC();

	TMatchParams pluginParams;
	const TDesC& name = KWapPushFilterPluginName;
	pluginParams.SetName(name);
	pluginParams.SetPartialMatch(ETrue);

	// Request the plugin - this will leave if the plug-in can't be found
	CPluginOwner* filterPluginOwner = NULL;
	TRAPD(err,filterPluginOwner = CPluginBase::CreatePluginOwnerL(KUidWapPushFilterPlugin,pluginParams));
	if (err == EWapErrPluginNotFound)
		test.Printf(_L("No filter plugin found"));
	else 
		{
		
		User::LeaveIfError(err);
		CleanupStack::PushL(filterPluginOwner);

		CWapPushMessageFilter& pushfilter = STATIC_CAST(CWapPushMessageFilter&, filterPluginOwner->Plugin());
	
	// do some tests here
		HBufC8* dummy = _L8("dummy").AllocLC();
		HBufC8* dummy2 = _L8("dummy2").AllocLC();

		CPushMessage* msg1 = CPushMessage::NewL(dummy,dummy2); // dummy message
		CleanupStack::Pop(2, dummy);// dummy, dummy2
		
		TBool result = EFalse;
		TRequestStatus status;
		pushfilter.FilterMessage(*msg1,status,result);
		User::WaitForRequest(status);

		delete msg1;
		CleanupStack::PopAndDestroy(); // filterPlugin
		}



	CleanupStack::PopAndDestroy(); // ActiveScheduler

	test.End();
	test.Close();
	}

GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;
	CTrapCleanup*	theCleanup = CTrapCleanup::New();
	

	TRAPD(ret,doTestsL());		
	test(ret==KErrNone);

	delete theCleanup;	
	__UHEAP_MARKEND;
	return(KErrNone);
	}
