// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Author	Julia Blumin 
// Date		Aug 1999
// 
//

#include <e32test.h>
#include <e32hal.h>
#include <f32fsys.h>
#include <s32file.h>
#include <barsc.h>

#include <e32uid.h>

//#include <prsreg.h>
//#include <smdef.h>
#include "NbsWatcher.h"		

#include <es_sock.h>
#include <e32test.h>
#include "gsmubuf.h"
#include "gsmumsg.h"
#include "smsuaddr.h"
#include "smsustrm.h"
#include "MsvTestUtilsBase.h"
#include "BioTestUtils.h"

#include "NbsSocketWatcher.h"



#include "cmstd.h"

_LIT(KTelephoneNumber,"+447747065440");
//_LIT(KTelephoneNumber,"+447771656004");
//_LIT(KTelephoneNumber,"+44802258765");
_LIT(KServiceCenterAddress,"+44385016005");

RSocketServ gSS;
RSocket gSocket;

//----------------------------------------------------------------------------------------
TFileName theLogName;
RFile theLog;
RFs theFs;
RTest gTest(_L("NBSWatcher Test Harness"));
//----------------------------------------------------------------------------------------
RFs gTestDirRFs;
_LIT(kDrv,"C:");
_LIT(kTestBioMsgPathFresh,"C:\\test\\bio\\Fresh\\");
_LIT(kTestBioMsgPathStale,"C:\\test\\bio\\Stale\\");
//----------------------------------------------------------------------------------------


const TUid KTestUid			= {0x10001270};
const TUid KTestUid1		= {0x10220108};
const TUid KTestUid2		= {0x10220109};


void TestScheduler::Error( TInt anError ) const
	{
	User::Leave( anError );
	}


//----------------------------------------------------------------------------------------
LOCAL_C void doMainL()
//----------------------------------------------------------------------------------------
	{
	CActiveScheduler* sch = new(ELeave)CActiveScheduler;
	CleanupStack::PushL(sch);

	CActiveScheduler::Install(sch);

	// Start the active scheduler if necessary
	CWatcherWait* wait = CWatcherWait::NewLC();

	CTestTimer* timer = CTestTimer::NewL();
	CleanupStack::PushL(timer);

	CWatcherArray* watchers = CWatcherArray::NewL();

	if (watchers->Count() > 0)
		{
		wait->Start();
		timer->AfterReq(1000000, wait->iStatus);
		CActiveScheduler::Start();
		}

	delete watchers;

	// Tidy up
	CleanupStack::PopAndDestroy(3); //wait, sch, timer

/*	TInt aTestNumber = 1;
	InitTestUtils();

	gTest.Start(_L("Create/Destroy Socket Watcher"));
	CreateDestroySocketWatcher(aTestNumber++);
	
	gTest.Next(_L("Start Watchers"));
	
	//SendMessages(aTestNumber++);

	StartWatchers(aTestNumber++);
	
	CloseTestUtils();*/
	}



//----------------------------------------------------------------------------------------
GLDEF_C TInt E32Main()
//----------------------------------------------------------------------------------------
	{	
__UHEAP_MARK;	
	CTrapCleanup* theCleanup = CTrapCleanup::New();
	TRAPD(ret,doMainL());		
	gTest(ret==KErrNone);
	delete theCleanup;	
	
__UHEAP_MARKEND;
	return(KErrNone);
	}
