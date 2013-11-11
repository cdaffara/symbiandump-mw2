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
* Description: Provides static functions which handles server startup.
*
*
*/


// INCLUDE FILES
#include "EPos_PosLmServerStartup.h"
#include "EPos_CPosLmServer.h"
#include "EPos_PosLmServerCommon.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PosLmServerStartup::StartServer
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt PosLmServerStartup::StartServer()
    {
    __UHEAP_MARK;

    CTrapCleanup* cleanupStack = CTrapCleanup::New();

    TInt ret = KErrNoMemory;

    if (cleanupStack)
        {
        TRAP(ret, StartServerL());
        delete cleanupStack;
        }

    __UHEAP_MARKEND;

    return ret;
    }

// -----------------------------------------------------------------------------
// PosLmServerStartup::StartServerL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmServerStartup::StartServerL()
    {
    // Rename thread to aid debugging
    User::LeaveIfError(User::RenameThread(KPosLandmarksServerName));

    // start scheduler and server
    CActiveScheduler* scheduler = new(ELeave) CActiveScheduler;
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);

    CPosLmServer* server = CPosLmServer::NewL();
    CleanupStack::PushL(server);

    CleanupStack::Pop(2, scheduler); // server

    RProcess::Rendezvous(KErrNone);

    CActiveScheduler::Start();

    delete server;
    delete scheduler;
    }

// -----------------------------------------------------------------------------
// E32Main
// Server process entry-point
// Recover the startup parameters and run the server
// -----------------------------------------------------------------------------
//
TInt E32Main()
    {
    return PosLmServerStartup::StartServer();
    }

//  End of File
