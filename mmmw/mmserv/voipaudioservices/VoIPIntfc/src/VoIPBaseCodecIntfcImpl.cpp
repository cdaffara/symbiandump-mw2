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
 * Description: VOIP Audio Services
 *
 */

#include "VoIPAudioSession.h"
#include "VoIPDownlinkStreamImpl.h"
#include "VoIPUplinkStreamImpl.h"
#include "VoIPBaseCodecIntfcImpl.h"

// ---------------------------------------------------------------------------
// CVoIPBaseCodecIntfcImpl::NewL
// ---------------------------------------------------------------------------
//
CVoIPBaseCodecIntfcImpl* CVoIPBaseCodecIntfcImpl::NewL(
        CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl)
    {
    CVoIPBaseCodecIntfcImpl* self = new (ELeave) CVoIPBaseCodecIntfcImpl();
    CleanupStack::PushL(self);
    self->ConstructL(aDnLinkStreamImpl);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVoIPBaseCodecIntfcImpl::NewL
// ---------------------------------------------------------------------------
//
CVoIPBaseCodecIntfcImpl* CVoIPBaseCodecIntfcImpl::NewL(
        CVoIPAudioUplinkStreamImpl* aUpLinkStreamImpl)
    {
    CVoIPBaseCodecIntfcImpl* self = new (ELeave) CVoIPBaseCodecIntfcImpl();
    CleanupStack::PushL(self);
    self->ConstructL(aUpLinkStreamImpl);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVoIPBaseCodecIntfcImpl::~CVoIPBaseCodecIntfcImpl
// ---------------------------------------------------------------------------
//
CVoIPBaseCodecIntfcImpl::~CVoIPBaseCodecIntfcImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPBaseCodecIntfcImpl::CVoIPBaseCodecIntfcImpl
// ---------------------------------------------------------------------------
//
CVoIPBaseCodecIntfcImpl::CVoIPBaseCodecIntfcImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPBaseCodecIntfcImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPBaseCodecIntfcImpl::ConstructL(
        CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl)
    {
    iVoIPAudioSession = aDnLinkStreamImpl->GetSession();
    CVoIPFormatIntfcImpl::ConstructL(iVoIPAudioSession);
    CVoIPFormatIntfc::ConstructL(this);
    }

// ---------------------------------------------------------------------------
// CVoIPBaseCodecIntfcImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPBaseCodecIntfcImpl::ConstructL(
        CVoIPAudioUplinkStreamImpl* aUpLinkStreamImpl)
    {
    iVoIPAudioSession = aUpLinkStreamImpl->GetSession();
    CVoIPFormatIntfcImpl::ConstructL(iVoIPAudioSession);
    CVoIPFormatIntfc::ConstructL(this);
    }

// End of file
