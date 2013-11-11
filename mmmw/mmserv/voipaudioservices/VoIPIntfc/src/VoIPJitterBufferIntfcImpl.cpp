/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
#include "VoIPAudioSession.h"
#include "VoIPJitterBufferIntfcImpl.h"

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfcImpl::NewL
// ---------------------------------------------------------------------------
//
CVoIPJitterBufferIntfcImpl* CVoIPJitterBufferIntfcImpl::NewL(
        RVoIPAudioSession* aVoIPAudioSession)
    {
    CVoIPJitterBufferIntfcImpl* self =
            new (ELeave) CVoIPJitterBufferIntfcImpl();
    CleanupStack::PushL(self);
    self->ConstructL(aVoIPAudioSession);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfcImpl::~CVoIPJitterBufferIntfcImpl
// ---------------------------------------------------------------------------
//
CVoIPJitterBufferIntfcImpl::~CVoIPJitterBufferIntfcImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfcImpl::CVoIPJitterBufferIntfcImpl
// ---------------------------------------------------------------------------
//
CVoIPJitterBufferIntfcImpl::CVoIPJitterBufferIntfcImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfcImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPJitterBufferIntfcImpl::ConstructL(
        RVoIPAudioSession* aVoIPAudioSession)
    {
    CVoIPJitterBufferIntfc::ConstructL(this);
    iVoIPAudioSession = aVoIPAudioSession;
    }

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfcImpl::SetObserver
// ---------------------------------------------------------------------------
//
TInt CVoIPJitterBufferIntfcImpl::SetObserver(
        MVoIPJitterBufferObserver& aObserver)
    {
    iObserver = &aObserver;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfcImpl::ConfigureJitterBuffer
// ---------------------------------------------------------------------------
//
TInt CVoIPJitterBufferIntfcImpl::ConfigureJitterBuffer(
        const TVoIPJBConfig& aJbConfig)
    {
    TInt err = iVoIPAudioSession->ConfigureJitterBuffer(aJbConfig);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfcImpl::ResetJitterBuffer
// ---------------------------------------------------------------------------
//
TInt CVoIPJitterBufferIntfcImpl::ResetJitterBuffer(TBool aPlayTone)
    {
    TInt err = iVoIPAudioSession->ResetJitterBuffer(aPlayTone);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfcImpl::DelayDown
// ---------------------------------------------------------------------------
//
TInt CVoIPJitterBufferIntfcImpl::DelayDown()
    {
    TInt err = iVoIPAudioSession->DelayDown();
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfcImpl::DelayUp
// ---------------------------------------------------------------------------
//
TInt CVoIPJitterBufferIntfcImpl::DelayUp()
    {
    TInt err = iVoIPAudioSession->DelayUp();
    return err;
    }

#ifdef __FEATURE_NOT_SUPPORTED__
// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfcImpl::SendEventToObserver
// ---------------------------------------------------------------------------
//
void CVoIPJitterBufferIntfcImpl::SendEventToObserver(TInt aEvent)
    {
    iObserver->Event(*this, aEvent);
    }
#endif //__FEATURE_NOT_SUPPORTED__


// End of file
