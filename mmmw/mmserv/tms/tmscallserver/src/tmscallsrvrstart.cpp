/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Telephony Multimedia Service
 *
 */

#include <ecom/ecom.h>
#include "tmscallserver.h"
#include "tmscallserverstartparam.h"
#include "tmsutility.h"

using namespace TMS;

_LIT(KTMSCallServerName, "TMSCallServer-");

// -----------------------------------------------------------------------------
// TMSCallServer::StartThread
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TMSCallServer::StartThread(TAny* aParams)
    {
    TRACE_PRN_FN_ENT;

    TInt err = KErrNone;
    __UHEAP_MARK;
    TMSCallServerStartParam* start =
            reinterpret_cast<TMSCallServerStartParam*> (aParams);
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if (!cleanup)
        {
        err = KErrNoMemory;
        }
    else
        {
        TRAP(err, StartThreadL(*start));
        }
    delete cleanup;

    REComSession::FinalClose();
    __UHEAP_MARKEND;

    TRACE_PRN_FN_EXT;
    return err;
    }

// -----------------------------------------------------------------------------
// TMSCallServer::StartThreadL
//
// -----------------------------------------------------------------------------
//
void TMSCallServer::StartThreadL(TMSCallServerStartParam& aStart)
    {
    TRACE_PRN_FN_ENT;

    CActiveScheduler* sched = new (ELeave) CActiveScheduler;
    CleanupStack::PushL(sched);

    CActiveScheduler::Install(sched);
    TMSCallServer* server = TMSCallServer::NewL(aStart.iTMSServer);
    CleanupStack::PushL(server);

    //Rename tmscall server name
    RThread tmscallServerThread;
    TThreadId threadId;
    TName name;
    name.Append(KTMSCallServerName);
    threadId = tmscallServerThread.Id();
    name.AppendNum(threadId.Id(), EHex);
    //We are ignoring the error code returned from User::RenameThread
    //as it is not important here, may be for profiling
    User::RenameThread(name);

    aStart.iTMSCallServerHandle = server->Server();
    // Sync with the client and enter the active scheduler
    RThread::Rendezvous(KErrNone);
    sched->Start();

    CleanupStack::PopAndDestroy(server); // server
    CleanupStack::PopAndDestroy(sched); // sched

    TRACE_PRN_FN_EXT;
    }

// End of file
