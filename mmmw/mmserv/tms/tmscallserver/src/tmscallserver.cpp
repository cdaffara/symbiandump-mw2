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

#include "tmscallserver.h"
#include "tmsdelaytimer.h"
#include "tmsutility.h"
#include "tmscallsession.h"

using namespace TMS;

const TInt KShutDownDelay = 0; //0 sec

// -----------------------------------------------------------------------------
// TMSCallServer::TMSCallServer
//
// -----------------------------------------------------------------------------
//
TMSCallServer::TMSCallServer(TMSServer* aTMSServer) :
    CServer2(CActive::EPriorityHigh, EGlobalSharableSessions),
    iTMSServer(aTMSServer),
    iDelayTimer(NULL),
    iSessionCount(0)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallServer::~TMSCallServer
//
// -----------------------------------------------------------------------------
//
TMSCallServer::~TMSCallServer()
    {
    TRACE_PRN_FN_ENT;
    if (iDelayTimer)
        {
        iDelayTimer->Cancel();
        delete iDelayTimer;
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallServer::NewL
//
// -----------------------------------------------------------------------------
//
TMSCallServer* TMSCallServer::NewL(TMSServer* aTMSServer)
    {
    TRACE_PRN_FN_ENT;
    TMSCallServer* self = new (ELeave) TMSCallServer(aTMSServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    TRACE_PRN_FN_EXT;
    return self;
    }

// -----------------------------------------------------------------------------
// TMSCallServer::ConstructL
//
// -----------------------------------------------------------------------------
//
void TMSCallServer::ConstructL()
    {
    TRACE_PRN_FN_ENT;
    // Call base class to Start server
    StartL(KNullDesC);
    iDelayTimer = TMSCallDelayTimer::NewL();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallServer::AddSession
//
// -----------------------------------------------------------------------------
//
void TMSCallServer::AddSession()
    {
    TRACE_PRN_FN_ENT;
    iSessionCount++;
    iDelayTimer->Cancel();
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallServer::DropSession
//
// -----------------------------------------------------------------------------
//
void TMSCallServer::DropSession()
    {
    TRACE_PRN_FN_ENT;
    iSessionCount--;
    if (iSessionCount == 0)
        {
        iDelayTimer->SetDelay(TTimeIntervalMicroSeconds32(KShutDownDelay));
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSCallServer::NewSessionL
//
// -----------------------------------------------------------------------------
//
CSession2* TMSCallServer::NewSessionL(const TVersion& aVersion,
        const RMessage2& /*aMessage*/) const
    {
    TRACE_PRN_FN_ENT;
    const TVersion version(KTMSCallServerMajorVersionNumber,
                           KTMSCallServerMinorVersionNumber,
                           KTMSCallServerBuildVersionNumber);

    if (!User::QueryVersionSupported(version, aVersion))
        {
        User::Leave(KErrNotSupported);
        }

    TMSCallSession* session =
        TMSCallSession::NewL(*(const_cast<TMSCallServer*> (this)));
    TRACE_PRN_FN_EXT;
    return session;
    }

// -----------------------------------------------------------------------------
// TMSCallServer::TerminateServer
//
// -----------------------------------------------------------------------------
//
void TMSCallServer::TerminateServer()
    {
    TMSCallSession *session;
    iSessionIter.SetToFirst();

    for (;;)
        {
        session = static_cast<TMSCallSession*>(iSessionIter++);
        if (!session)
            {
            break;
            }
        delete session;
        }
    }

// End of file
