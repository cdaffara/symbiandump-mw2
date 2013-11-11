// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
    
#include "starter.h"

LOCAL_C void StartAppsL();
CActiveScheduler* TheScheduler;

#if defined(__WINS__)
//
EXPORT_C TInt EntryPoint(TAny* /*aCmdLine*/)
//
#else
//
GLDEF_C TInt E32Main()
#endif
	{
	
	__UHEAP_MARK; // mark heap state
    
	CTrapCleanup* TheTrapCleanup=CTrapCleanup::New();
	TRAPD(error, StartAppsL());
    delete TheTrapCleanup;
	
	__UHEAP_MARKEND; // check no memory leak
	
	return(error);
	}


LOCAL_C void StartAppsL()
	{
	TheScheduler=new(ELeave) CActiveScheduler();
	CleanupStack::PushL(TheScheduler);
	CActiveScheduler::Install(TheScheduler);

	CStarter *starter = CStarter::NewL();
	CleanupStack::PushL(starter);

	CActiveScheduler::Add(starter);
	starter->Start();
	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(2); // starter,TheScheduler
	}
