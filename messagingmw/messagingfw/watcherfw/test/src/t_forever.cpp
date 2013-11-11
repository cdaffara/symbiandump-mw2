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
// t_forever.cpp
//

#include <watcher.h>
#include "cwatcher.h"

LOCAL_C void DoStartL()
	{
	CActiveScheduler::Install(new(ELeave)CActiveScheduler);
	CWatcherWait* wait = CWatcherWait::NewLC();
		
	CWatcher* watcher = CWatcher::NewL(CActive::EPriorityStandard);
	FOREVER
		{
		wait->Start();
		watcher->Start(wait->iStatus);
		CActiveScheduler::Start();
		}
	delete watcher;
	
	CleanupStack::PopAndDestroy(); // wait
	delete CActiveScheduler::Current();
	}

GLDEF_C TInt E32Main()
	{       
	__UHEAP_MARK;
	CTrapCleanup* theCleanup=CTrapCleanup::New();
	TRAPD(ret,DoStartL());          
	delete theCleanup;      
	__UHEAP_MARKEND;
	return(KErrNone);
	}

EXPORT_C TInt WinsMain()
	{
#if defined(__WINS__)
	E32Main();
#endif
	return KErrNone;
	}

