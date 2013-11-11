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

#include "VoIPAudioSession.h"
#include "VoIPDownlinkStreamImpl.h"
#include "VoIPG729DecoderIntfcImpl.h"

// ---------------------------------------------------------------------------
// CVoIPG729DecoderIntfcImpl::NewL
// ---------------------------------------------------------------------------
//
CVoIPG729DecoderIntfcImpl* CVoIPG729DecoderIntfcImpl::NewL(
        CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl)
    {
    CVoIPG729DecoderIntfcImpl* self = new (ELeave) CVoIPG729DecoderIntfcImpl();
    CleanupStack::PushL(self);
    self->ConstructL(aDnLinkStreamImpl);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVoIPG729DecoderIntfcImpl::~CVoIPG729DecoderIntfcImpl
// ---------------------------------------------------------------------------
//
CVoIPG729DecoderIntfcImpl::~CVoIPG729DecoderIntfcImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPG729DecoderIntfcImpl::CVoIPG729DecoderIntfcImpl
// ---------------------------------------------------------------------------
//
CVoIPG729DecoderIntfcImpl::CVoIPG729DecoderIntfcImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPG729DecoderIntfcImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPG729DecoderIntfcImpl::ConstructL(
        CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl)
    {
    iVoIPAudioSession = aDnLinkStreamImpl->GetSession();
    CVoIPFormatIntfcImpl::ConstructL(iVoIPAudioSession);
    CVoIPG729DecoderIntfc::ConstructL(this);
    }

// ---------------------------------------------------------------------------
// CVoIPG729DecoderIntfcImpl::BadLsfNextBuffer
// ---------------------------------------------------------------------------
//
TInt CVoIPG729DecoderIntfcImpl::BadLsfNextBuffer()
    {
    TInt err = iVoIPAudioSession->BadLsfNextBuffer();
    return err;
    }

// End of file
