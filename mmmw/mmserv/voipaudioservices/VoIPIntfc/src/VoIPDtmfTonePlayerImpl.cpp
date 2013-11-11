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
 * Description:  VOIP Audio Services
 *
 */

#include <e32base.h>
#include <voipuplinkstream.h>
#include <voipdownlinkstream.h>
#include "VoIPAudioSession.h"
#include "VoIPDtmfTonePlayerImpl.h"

// ---------------------------------------------------------------------------
// CDTMFTonePlayerImpl::NewL
// ---------------------------------------------------------------------------
//
CDTMFTonePlayerImpl* CDTMFTonePlayerImpl::NewL()
    {
    CDTMFTonePlayerImpl* self = new (ELeave) CDTMFTonePlayerImpl();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayerImpl::~CDTMFTonePlayerImpl
// ---------------------------------------------------------------------------
//
CDTMFTonePlayerImpl::~CDTMFTonePlayerImpl()
    {
    Close();

    if (iMsgQComHandler)
        {
        iMsgQComHandler->Cancel();
        delete iMsgQComHandler;
        }

    if (iMsgComQueue.Handle() > 0)
        {
        iMsgComQueue.Close();
        }
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayerImpl::CDTMFTonePlayerImpl
// ---------------------------------------------------------------------------
//
CDTMFTonePlayerImpl::CDTMFTonePlayerImpl()
    {
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayerImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CDTMFTonePlayerImpl::ConstructL()
    {
    // Pass ownership to the parent class
    CDTMFTonePlayer::ConstructL(this);
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayerImpl::Open
// ---------------------------------------------------------------------------
//
TInt CDTMFTonePlayerImpl::Open(MDTMFToneObserver& aObserver)
    {
    TInt err = KErrNotReady;
    iObserver = &aObserver;

    TRAP(err, StartMsgQueueL());
    if (iVoIPAudioSession && err == KErrNone)
        {
        err = iVoIPAudioSession->OpenDTMFTonePlayer();
        }

    return err;
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayerImpl::Play
// ---------------------------------------------------------------------------
//
TInt CDTMFTonePlayerImpl::Play(const TPtr aTones)
    {
    TInt err = KErrNotReady;

    if (iVoIPAudioSession)
        {
        err = iVoIPAudioSession->PlayDTMFTone(aTones);
        }

    return err;
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayerImpl::Stop
// ---------------------------------------------------------------------------
//
TInt CDTMFTonePlayerImpl::Stop()
    {
    TInt err = KErrNotReady;

    if (iVoIPAudioSession)
        {
        err = iVoIPAudioSession->StopDTMFTonePlay();
        }

    return err;
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayerImpl::Close
// ---------------------------------------------------------------------------
//
void CDTMFTonePlayerImpl::Close()
    {
    if (iVoIPAudioSession)
        {
        iVoIPAudioSession->CloseDTMFTonePlayer();
        }
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayerImpl::AddVoIPAudioUplinkStream
// ---------------------------------------------------------------------------
//
TInt CDTMFTonePlayerImpl::AddVoIPAudioUplinkStream(
        CVoIPAudioUplinkStream& /*aStream*/, RVoIPAudioSession* aSession)
    {
    TInt err = KErrNone;
    iVoIPAudioSession = aSession;

    if (!iVoIPAudioSession)
        {
        err = KErrNotFound;
        }
    else
        {
        iStreamType = EUplink;
        }

    return err;
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayerImpl::RemoveVoIPAudioUplinkStream
// ---------------------------------------------------------------------------
//
TInt CDTMFTonePlayerImpl::RemoveVoIPAudioUplinkStream(
        CVoIPAudioUplinkStream& /*aStream*/)
    {
    iVoIPAudioSession = NULL;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayerImpl::AddVoIPAudioDownlinkStream
// ---------------------------------------------------------------------------
//
TInt CDTMFTonePlayerImpl::AddVoIPAudioDownlinkStream(
        CVoIPAudioDownlinkStream& /*aStream*/, RVoIPAudioSession* aSession)
    {
    TInt err = KErrNone;
    iVoIPAudioSession = aSession;

    if (!iVoIPAudioSession)
        {
        err = KErrNotFound;
        }
    else
        {
        iStreamType = EDownlink;
        }

    return err;
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayerImpl::RemoveVoIPAudioDownlinkStream
// ---------------------------------------------------------------------------
//
TInt CDTMFTonePlayerImpl::RemoveVoIPAudioDownlinkStream(
        CVoIPAudioDownlinkStream& /*aStream*/)
    {
    iVoIPAudioSession = NULL;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayerImpl::StrartMsgQueueL
//
// ---------------------------------------------------------------------------
//
void CDTMFTonePlayerImpl::StartMsgQueueL()
    {
    TInt err = KErrNone;

    if (iMsgComQueue.Handle() <= 0)
        {
        if (iStreamType == EDownlink)
            {
            err = iMsgComQueue.CreateGlobal(KDTMFToneComQueueDNL,
                    KVoIPMsgComQSlots);
            }
        else
            {
            err = iMsgComQueue.CreateGlobal(KDTMFToneComQueueUPL,
                    KVoIPMsgComQSlots);
            }

        User::LeaveIfError(err);
        }

    ReceiveMsgQComHandlerEventsL();
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayerImpl::ReceiveMsgQComHandlerEventsL
// Starts message queue handler (A/O) to monitor server side events
// ---------------------------------------------------------------------------
//
void CDTMFTonePlayerImpl::ReceiveMsgQComHandlerEventsL()
    {
    if (iMsgQComHandler)
        {
        iMsgQComHandler->Cancel();
        }
    else
        {
        iMsgQComHandler = CQueueHandler::NewL(this, &iMsgComQueue);
        }

    iMsgQComHandler->Start();
    }

// ======== CALLBACK FUNCTIONS ========


// ---------------------------------------------------------------------------
// CDTMFTonePlayerImpl::Event
//
// From MQueueHandlerObserver
// ---------------------------------------------------------------------------
//
void CDTMFTonePlayerImpl::Event(TInt aEventType, TInt aError)
    {
    iObserver->Event(*this, aEventType, aError);
    }

// End of file
