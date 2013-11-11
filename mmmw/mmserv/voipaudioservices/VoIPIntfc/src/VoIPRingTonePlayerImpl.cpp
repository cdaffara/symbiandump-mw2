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
 * Description: VOIP Audio Service
 *
 */

#include <e32base.h>
#include "VoIPAudioSession.h"
#include "VoIPRingTonePlayerImpl.h"

// ---------------------------------------------------------------------------
// CRingTonePlayerImpl::NewL
// ---------------------------------------------------------------------------
//
CRingTonePlayerImpl* CRingTonePlayerImpl::NewL()
    {
    CRingTonePlayerImpl* self = new (ELeave) CRingTonePlayerImpl();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CRingTonePlayerImpl::~CRingTonePlayerImpl
// ---------------------------------------------------------------------------
//
CRingTonePlayerImpl::~CRingTonePlayerImpl()
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

    if (iVoIPAudioSession.Handle() > 0)
        {
        iVoIPAudioSession.Close();
        }
    }

// ---------------------------------------------------------------------------
// CRingTonePlayerImpl::CRingTonePlayerImpl
// ---------------------------------------------------------------------------
//
CRingTonePlayerImpl::CRingTonePlayerImpl()
    {
    }

// ---------------------------------------------------------------------------
// CRingTonePlayerImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CRingTonePlayerImpl::ConstructL()
    {
    // Pass ownership to the parent class
    CRingTonePlayer::ConstructL(this);

    TInt err = iVoIPAudioSession.Connect();
    User::LeaveIfError(err);
    }

// ---------------------------------------------------------------------------
// CRingTonePlayerImpl::OpenL
//
// Gets ring tone from the Profiles
// ---------------------------------------------------------------------------
//
void CRingTonePlayerImpl::OpenL(MRingToneObserver& aObserver)
    {
    iObserver = &aObserver;
    StartMsgQueueL();

    TInt err = iVoIPAudioSession.OpenRingTonePlayer();
    User::LeaveIfError(err);
    }

// ---------------------------------------------------------------------------
// CRingTonePlayerImpl::OpenL
//
// Plays ring tone from file
// ---------------------------------------------------------------------------
//
void CRingTonePlayerImpl::OpenL(MRingToneObserver& aObserver, TPtr aFileName)
    {
    iObserver = &aObserver;
    StartMsgQueueL();
    TInt err = iVoIPAudioSession.OpenRingTonePlayer(aFileName);
    User::LeaveIfError(err);
    }

// ---------------------------------------------------------------------------
// CRingTonePlayerImpl::OpenL
//
// Plays ring tone from file handle
// ---------------------------------------------------------------------------
//
void CRingTonePlayerImpl::OpenL(MRingToneObserver& aObserver,
        RFile& aFileHandle)
    {
    iObserver = &aObserver;
    StartMsgQueueL();
    TInt err = iVoIPAudioSession.OpenRingTonePlayer(aFileHandle);
    User::LeaveIfError(err);
    }

// ---------------------------------------------------------------------------
// CRingTonePlayerImpl::OpenL
//
// Plays ring tone from a descriptor string based on its MIME type
// ---------------------------------------------------------------------------
//
void CRingTonePlayerImpl::OpenL(MRingToneObserver& aObserver, TPtr8 aDesTone,
        TPtr8 /*aMimeType*/)
    {
    iObserver = &aObserver;
    StartMsgQueueL();

    TInt size = aDesTone.Size();

    if (size <= 0)
        {
        User::Leave(KErrArgument);
        }
    else
        {
        HBufC8* tone = HBufC8::NewLC(size);
        TPtr8 ptr = tone->Des();
        ptr.Copy(aDesTone);
        TInt err = iVoIPAudioSession.OpenRingTonePlayer(*tone);
        CleanupStack::PopAndDestroy(tone);
        User::LeaveIfError(err); //return error via trap mechanism
        }
    }

// ---------------------------------------------------------------------------
// CRingTonePlayerImpl::Play
// ---------------------------------------------------------------------------
//
TInt CRingTonePlayerImpl::Play()
    {
    TInt err = iVoIPAudioSession.PlayRingTone();
    return err;
    }

// ---------------------------------------------------------------------------
// CRingTonePlayerImpl::Pause
// ---------------------------------------------------------------------------
//
TInt CRingTonePlayerImpl::Pause()
    {
    TInt err = iVoIPAudioSession.PauseRingTone();
    return err;
    }

// ---------------------------------------------------------------------------
// CRingTonePlayerImpl::Resume
// ---------------------------------------------------------------------------
//
TInt CRingTonePlayerImpl::Resume()
    {
    TInt err = iVoIPAudioSession.ResumeRingTone();
    return err;
    }

// ---------------------------------------------------------------------------
// CRingTonePlayerImpl::Stop
// ---------------------------------------------------------------------------
//
TInt CRingTonePlayerImpl::Stop()
    {
    TInt err = iVoIPAudioSession.StopRingTone();
    return err;
    }

// ---------------------------------------------------------------------------
// CRingTonePlayerImpl::Close
// ---------------------------------------------------------------------------
//
void CRingTonePlayerImpl::Close()
    {
    iVoIPAudioSession.CloseRingTonePlayer();
    }

// ---------------------------------------------------------------------------
// CRingTonePlayerImpl::StrartMsgQueueL
//
// ---------------------------------------------------------------------------
//
void CRingTonePlayerImpl::StartMsgQueueL()
    {
    if (iMsgComQueue.Handle() <= 0)
        {
        TInt err = iMsgComQueue.CreateGlobal(KRingToneComQueue,
                KVoIPMsgComQSlots);
        User::LeaveIfError(err);
        }

    ReceiveMsgQComHandlerEventsL();
    }

// ---------------------------------------------------------------------------
// CRingTonePlayerImpl::ReceiveMsgQComHandlerEventsL
// Starts message queue handler (A/O) to monitor server side events
// ---------------------------------------------------------------------------
//
void CRingTonePlayerImpl::ReceiveMsgQComHandlerEventsL()
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
// CRingTonePlayerImpl::Event
//
// From MQueueHandlerObserver
// ---------------------------------------------------------------------------
//
void CRingTonePlayerImpl::Event(TInt aEventType, TInt aError)
    {
    iObserver->Event(*this, aEventType, aError);
    }

// End of file
