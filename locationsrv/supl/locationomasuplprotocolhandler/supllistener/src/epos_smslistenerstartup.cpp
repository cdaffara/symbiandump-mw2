/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   SMS Listener Startup
*
*/


#include <featmgr.h>

#include "epos_comasuplsmslistenerbase.hrh"


    #include "epos_comasuplgsmsmslistener.h"


_LIT(KSuplListener, "SUPLListener");

void StartSmsListenerL()
	{
	User::LeaveIfError(RThread().RenameMe(KSuplListener));

    FeatureManager::InitializeLibL();
	TBool fsupport;
	fsupport = FeatureManager::FeatureSupported(1583);
	FeatureManager::UnInitializeLib();
	if (!fsupport)
        {
        User::Leave(KErrNotSupported);
        }

	// Start scheduler and server
	CActiveScheduler* scheduler = new ( ELeave ) CActiveScheduler;
	CleanupStack::PushL( scheduler );
	CActiveScheduler::Install( scheduler );

    
        COMASuplSmsListenerBase* recr = COMASuplGsmSmsListener::NewL();
    
    
	CleanupStack::PushL( recr );
	recr->ListenForSMSL();

	RProcess::Rendezvous( KErrNone );
	
	CleanupStack::Pop( recr );
	CleanupStack::Pop( scheduler );

    CActiveScheduler::Start();
	delete recr;
	delete scheduler;
	}


TInt E32Main()
	{
	__UHEAP_MARK;

	CTrapCleanup* cleanupStack = CTrapCleanup::New();

	TInt ret = KErrNoMemory;
	if (cleanupStack)
		{
		TRAP(ret, StartSmsListenerL());
		delete cleanupStack;
		}

	__UHEAP_MARKEND;

	return ret;
	}
