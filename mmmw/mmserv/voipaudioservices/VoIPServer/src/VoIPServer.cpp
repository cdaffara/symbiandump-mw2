/*
 * Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Implements VoIP server object.
 *
 */

#include <e32svr.h>
#include <e32uid.h>
#include <e32capability.h>
#include "debugtracemacros.h"
#include "VoIPECallEventHandler.h"
#include "VoIPServerShutdown.h"
#include "VoIPServerSession.h"
#include "VoIPServer.h"

// CONSTANTS
const TInt KShutDownDelayTime = 2000000; // 2 sec delay time
const TInt KNumOfSessions = 3;

// -----------------------------------------------------------------------------
// CVoIPAudioServer::NewL
//
// -----------------------------------------------------------------------------
//
CVoIPAudioServer* CVoIPAudioServer::NewL()
    {
    CVoIPAudioServer* self = new (ELeave) CVoIPAudioServer;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServer::CVoIPAudioServer
//
// -----------------------------------------------------------------------------
//
CVoIPAudioServer::CVoIPAudioServer() :
    CServer2(CActive::EPriorityHigh, ESharableSessions),
    iSession(0)
    {
    iECallInProgress = EFalse;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServer::~CVoIPAudioServer
//
// -----------------------------------------------------------------------------
//
CVoIPAudioServer::~CVoIPAudioServer()
    {
    TRACE_PRN_FN_ENT;
    CancelECallEventNotifier();
    delete iShutdownTimer;
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServer::NewSessionL
//
// -----------------------------------------------------------------------------
//
CSession2* CVoIPAudioServer::NewSessionL(const TVersion& aVersion,
        const RMessage2& aMessage) const
    {
    TRACE_PRN_FN_ENT;

    // Block session creation during active emergency call
    if (iECallInProgress)
        {
        User::Leave(KErrPermissionDenied);
        }

    // Block access to audio device when MultimediaDD capability missing
    if (!aMessage.HasCapability(ECapabilityMultimediaDD))
        {
        User::Leave(KErrPermissionDenied);
        }

    // Limit number of sessions created by the server.
    if (iSession >= KNumOfSessions)
        {
        User::Leave(KErrPermissionDenied);
        }

    const TVersion version(KVoIPServMajorVersionNumber,
            KVoIPServMinorVersionNumber, KVoIPServBuildVersionNumber);

    if (!User::QueryVersionSupported(version, aVersion))
        {
        User::Leave(KErrNotSupported);
        }

    CVoIPAudioServerSession* session = CVoIPAudioServerSession::NewL(
            *((CVoIPAudioServer*) this));

    TRACE_PRN_FN_EXT;
    return session;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServer::ConstructL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServer::ConstructL()
    {
    TRACE_PRN_FN_ENT;

    iShutdownTimer = CDelayVoIPServerShutDown::NewL();
    StartL(KVoIPServerName);
    RThread().SetPriority(EPriorityRealTime);
    StartECallEventNotifierL();

    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServer::AddSession
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServer::AddSession()
    {
    iSession++;
    // If shutdown timer is active, cancel it here.
    iShutdownTimer->Cancel();

    TRACE_PRN_N1(_L("VoIP->SRV: AddSession->Active Sessions: [%d]"), iSession);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServer::DropSession
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServer::DropSession()
    {
    iSession--;

    // If session count is zero, launch shutdown timer here
    if (iSession == 0)
        {
        iShutdownTimer->SetDelay(TTimeIntervalMicroSeconds32(
                KShutDownDelayTime));
        }

    TRACE_PRN_N1(_L("VoIP->DNL: DropSession->Active Sessions: [%d]"), iSession);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServer::SessionCount
//
// -----------------------------------------------------------------------------
//
TInt CVoIPAudioServer::SessionCount()
    {
    return iSession;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServer::SetDnLinkSession
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServer::SetDnLinkSession(CVoIPAudioServerSession* aSession,
        TBool aStatus)
    {
    if (aStatus)
        {
        iDnlinkSession = aSession;
        }
    else
        {
        iDnlinkSession = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServer::SetUpLinkSession
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServer::SetUpLinkSession(CVoIPAudioServerSession* aSession,
        TBool aStatus)
    {
    if (aStatus)
        {
        iUplinkSession = aSession;
        }
    else
        {
        iUplinkSession = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServer::HasDnLinkSession
//
// -----------------------------------------------------------------------------
//
TBool CVoIPAudioServer::HasDnLinkSession()
    {
    TBool status = EFalse;
    if (iDnlinkSession)
        {
        status = ETrue;
        }
    return status;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServer::HasUpLinkSession
//
// -----------------------------------------------------------------------------
//
TBool CVoIPAudioServer::HasUpLinkSession()
    {
    TBool status = EFalse;
    if (iUplinkSession)
        {
        status = ETrue;
        }
    return status;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServer::StartECallEventNotifierL
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServer::StartECallEventNotifierL()
    {
    if (!iECallHandler)
        {
        iECallHandler = CVoIPECallEventHandler::NewL(
                (const_cast<CVoIPAudioServer*> (this)));
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServer::CancelECallEventNotifier
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServer::CancelECallEventNotifier()
    {
    delete iECallHandler;
    iECallHandler = NULL;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServer::NotifyECallEvent
//
// -----------------------------------------------------------------------------
//
void CVoIPAudioServer::NotifyECallEvent(TInt aECall)
    {
    iECallInProgress = aECall;

    if (iECallInProgress)
        {
        if (HasDnLinkSession())
            {
            iDnlinkSession->CloseDownlink();
            }
        if (HasUpLinkSession())
            {
            iUplinkSession->CloseUplink();
            }
        }
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServer::RunServerL
//
// -----------------------------------------------------------------------------
//
static void RunServerL()
    {
    TRACE_PRN_N(_L("VoIP->RunServerL"));

    // Create and install the active scheduler we need
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);

    // Create the server (leave it on the cleanup stack)
    CVoIPAudioServer* server = CVoIPAudioServer::NewL();

    // Initialisation complete, now signal the client
    RProcess::Rendezvous(KErrNone);

    // Start the scheduler and wait for client requests
    CActiveScheduler::Start();

    // Ready to exit.
    // Cleanup scheduler and delete the server
    CleanupStack::PopAndDestroy(scheduler);
    delete server;

    TRACE_PRN_N(_L("VoIP->RunServerL - VoIP server closed"));
    }

// -----------------------------------------------------------------------------
// E32Main
// Entry point for the server
// -----------------------------------------------------------------------------
//
TInt E32Main()
    {
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    TInt r = KErrNoMemory;
    if (cleanup)
        {
        TRAP(r, RunServerL());
        delete cleanup;
        }
    __UHEAP_MARKEND;
    return r;
    }

// End of file
