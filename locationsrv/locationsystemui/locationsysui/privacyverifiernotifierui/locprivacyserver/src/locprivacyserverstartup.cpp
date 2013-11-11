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
* Description:  Implementation for server startup .
*
*/

// INCLUDE FILES
#include "locprivacyserverstartup.h"
#include "locprivacyserver.h"
#include "locprivacycommon.h"
#include "locutilsdebug.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// LocPrivacyServerStartup::StartServer
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt LocPrivacyServerStartup::StartServer()
    {
    LOCUTILSDEBUG( "+LocPrivacyServerStartup::StartServer" )
    __UHEAP_MARK;

    CTrapCleanup* cleanupStack = CTrapCleanup::New();

    TInt ret = KErrNoMemory;

    if (cleanupStack)
        {
        TRAP(ret, StartServerL());
        delete cleanupStack;
        }

    __UHEAP_MARKEND;

    LOCUTILSDEBUG( "-LocPrivacyServerStartup::StartServer" )
    return ret;
    }

// -----------------------------------------------------------------------------
// LocPrivacyServerStartup::StartServerL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void LocPrivacyServerStartup::StartServerL()
    {
    LOCUTILSDEBUG( "+LocPrivacyServerStartup::StartServerL" )
    // Rename thread to aid debugging
    User::LeaveIfError(User::RenameThread(KLocPrivacyServerName));

    // start scheduler and server
    CActiveScheduler* scheduler = new(ELeave) CActiveScheduler;
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);

    CLocPrivacyServer* server = CLocPrivacyServer::NewL();
    CleanupStack::PushL(server);

    CleanupStack::Pop(2, scheduler); // server

    RProcess::Rendezvous(KErrNone);

    CActiveScheduler::Start();

    delete server;
    delete scheduler;
   LOCUTILSDEBUG( "-LocPrivacyServerStartup::StartServerL" )
    }

// -----------------------------------------------------------------------------
// E32Main
// Server process entry-point
// Recover the startup parameters and run the server
// -----------------------------------------------------------------------------
//
TInt E32Main()
    {
    return LocPrivacyServerStartup::StartServer();
    }

//  End of File
