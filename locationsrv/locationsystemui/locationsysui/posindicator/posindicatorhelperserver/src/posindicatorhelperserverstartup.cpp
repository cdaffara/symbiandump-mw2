/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Position indicator server start up
*
*/

#include <e32base.h>
#include <e32std.h>
#include "posindicatorservercore.h"
#include "posindicatorserverconst.h"
#include "posindicatorlogger.h"

// ---------------------------------------------------------------------------
// Starts the server core
// ---------------------------------------------------------------------------
//
void StartServerCoreL()
    {
    FUNC("StartServerCoreL");
    User::LeaveIfError(RThread().RenameMe(KPosIndHelperSrvName));
    
    // Install the scheduler and start the LBT server
    CActiveScheduler* scheduler = new ( ELeave ) CActiveScheduler;
    CleanupStack::PushL( scheduler );
    CActiveScheduler::Install( scheduler );
    
    CPosIndicatorServerCore* server = CPosIndicatorServerCore::NewLC();
    
    // This starts the position indicator server
    server->StartL(KPosIndHelperSrvName);
    
    // Finish rendezvous with launching process
    RProcess::Rendezvous( KErrNone );
    
    CActiveScheduler::Start();
    CleanupStack::PopAndDestroy( 2, scheduler );
    }


GLDEF_C TInt E32Main()
    {
    FUNC("E32Main");
    __UHEAP_MARK;

    CTrapCleanup* cleanupStack = CTrapCleanup::New();

    // The ret is initialized to KErrNoMemory so that the function returns 
    // KErrNoMemory if the cleanupstack allocation fails.
    TInt ret = KErrNoMemory;
    if (cleanupStack)
        {
        TRAP(ret, StartServerCoreL());
        delete cleanupStack;
        }
        
    __UHEAP_MARKEND;
    return ret;
    }

