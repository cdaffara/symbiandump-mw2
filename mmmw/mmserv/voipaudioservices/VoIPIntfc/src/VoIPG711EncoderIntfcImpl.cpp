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

#include "VoIPAudioSession.h"
#include "VoIPUplinkStreamImpl.h"
#include "VoIPG711EncoderIntfcImpl.h"

// ---------------------------------------------------------------------------
// CVoIPG711EncoderIntfcImpl::NewL
// ---------------------------------------------------------------------------
//
CVoIPG711EncoderIntfcImpl* CVoIPG711EncoderIntfcImpl::NewL(
        CVoIPAudioUplinkStreamImpl* aUpLinkStreamImpl)
    {
    CVoIPG711EncoderIntfcImpl* self =
            new (ELeave) CVoIPG711EncoderIntfcImpl();
    CleanupStack::PushL(self);
    self->ConstructL(aUpLinkStreamImpl);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVoIPG711EncoderIntfcImpl::~CVoIPG711EncoderIntfcImpl
// ---------------------------------------------------------------------------
//
CVoIPG711EncoderIntfcImpl::~CVoIPG711EncoderIntfcImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPG711EncoderIntfcImpl::CVoIPG711EncoderIntfcImpl
// ---------------------------------------------------------------------------
//
CVoIPG711EncoderIntfcImpl::CVoIPG711EncoderIntfcImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPG711EncoderIntfcImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPG711EncoderIntfcImpl::ConstructL(
        CVoIPAudioUplinkStreamImpl* aUpLinkStreamImpl)
    {
    iVoIPAudioSession = aUpLinkStreamImpl->GetSession();
    CVoIPFormatIntfcImpl::ConstructL(iVoIPAudioSession);
    CVoIPG711EncoderIntfc::ConstructL(this);
    }

// ---------------------------------------------------------------------------
// CVoIPG711EncoderIntfcImpl::SetMode
// ---------------------------------------------------------------------------
//
TInt CVoIPG711EncoderIntfcImpl::SetMode(
        CVoIPFormatIntfc::TG711CodecMode aMode)
    {
    TInt err = iVoIPAudioSession->SetMode(aMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPG711EncoderIntfcImpl::GetMode
// ---------------------------------------------------------------------------
//
TInt CVoIPG711EncoderIntfcImpl::GetMode(
        CVoIPFormatIntfc::TG711CodecMode& aMode)
    {
    TInt err = iVoIPAudioSession->GetMode(aMode);
    return err;
    }

// End of file
