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
#include "VoIPG711DecoderIntfcImpl.h"

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfcImpl::NewL
// ---------------------------------------------------------------------------
//
CVoIPG711DecoderIntfcImpl* CVoIPG711DecoderIntfcImpl::NewL(
        CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl)
    {
    CVoIPG711DecoderIntfcImpl* self = new (ELeave) CVoIPG711DecoderIntfcImpl();
    CleanupStack::PushL(self);
    self->ConstructL(aDnLinkStreamImpl);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfcImpl::~CVoIPG711DecoderIntfcImpl
// ---------------------------------------------------------------------------
//
CVoIPG711DecoderIntfcImpl::~CVoIPG711DecoderIntfcImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfcImpl::CVoIPG711DecoderIntfcImpl
// ---------------------------------------------------------------------------
//
CVoIPG711DecoderIntfcImpl::CVoIPG711DecoderIntfcImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfcImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPG711DecoderIntfcImpl::ConstructL(
        CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl)
    {
    iVoIPAudioSession = aDnLinkStreamImpl->GetSession();
    CVoIPFormatIntfcImpl::ConstructL(iVoIPAudioSession);
    CVoIPG711DecoderIntfc::ConstructL(this);
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfcImpl::SetMode
// ---------------------------------------------------------------------------
//
TInt CVoIPG711DecoderIntfcImpl::SetMode(
        CVoIPFormatIntfc::TG711CodecMode aMode)
    {
    TInt err = iVoIPAudioSession->SetMode(aMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfcImpl::GetMode
// ---------------------------------------------------------------------------
//
TInt CVoIPG711DecoderIntfcImpl::GetMode(
        CVoIPFormatIntfc::TG711CodecMode& aMode)
    {
    TInt err = iVoIPAudioSession->GetMode(aMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfcImpl::SetCNG
// ---------------------------------------------------------------------------
//
TInt CVoIPG711DecoderIntfcImpl::SetCNG(TBool aCng)
    {
    TInt err = iVoIPAudioSession->SetCNG(aCng);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfcImpl::GetCNG
// ---------------------------------------------------------------------------
//
TInt CVoIPG711DecoderIntfcImpl::GetCNG(TBool& aCng)
    {
    TInt err = iVoIPAudioSession->GetCNG(aCng);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfcImpl::SetPLC
// ---------------------------------------------------------------------------
//
TInt CVoIPG711DecoderIntfcImpl::SetPLC(TBool aPlc)
    {
    TInt err = iVoIPAudioSession->SetPLC(aPlc);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfcImpl::GetPLC
// ---------------------------------------------------------------------------
//
TInt CVoIPG711DecoderIntfcImpl::GetPLC(TBool& aPlc)
    {
    TInt err = iVoIPAudioSession->GetPLC(aPlc);
    return err;
    }

// End of file
