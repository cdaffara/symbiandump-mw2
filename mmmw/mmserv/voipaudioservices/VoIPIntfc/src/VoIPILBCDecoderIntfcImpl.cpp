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
#include "VoIPDownlinkStreamImpl.h"
#include "VoIPILBCDecoderIntfcImpl.h"

// ---------------------------------------------------------------------------
// CVoIPILBCDecoderIntfcImpl::NewL
// ---------------------------------------------------------------------------
//
CVoIPILBCDecoderIntfcImpl* CVoIPILBCDecoderIntfcImpl::NewL(
        CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl)
    {
    CVoIPILBCDecoderIntfcImpl* self = new (ELeave) CVoIPILBCDecoderIntfcImpl();
    CleanupStack::PushL(self);
    self->ConstructL(aDnLinkStreamImpl);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVoIPILBCDecoderIntfcImpl::~CVoIPILBCDecoderIntfcImpl
// ---------------------------------------------------------------------------
//
CVoIPILBCDecoderIntfcImpl::~CVoIPILBCDecoderIntfcImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPILBCDecoderIntfcImpl::CVoIPILBCDecoderIntfcImpl
// ---------------------------------------------------------------------------
//
CVoIPILBCDecoderIntfcImpl::CVoIPILBCDecoderIntfcImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPILBCDecoderIntfcImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPILBCDecoderIntfcImpl::ConstructL(
        CVoIPAudioDownlinkStreamImpl* aDnLinkStreamImpl)
    {
    iVoIPAudioSession = aDnLinkStreamImpl->GetSession();
    CVoIPFormatIntfcImpl::ConstructL(iVoIPAudioSession);
    CVoIPILBCDecoderIntfc::ConstructL(this);
    }

// ---------------------------------------------------------------------------
// CVoIPILBCDecoderIntfcImpl::SetMode
// ---------------------------------------------------------------------------
//
TInt CVoIPILBCDecoderIntfcImpl::SetMode(TILBCCodecMode aMode)
    {
    TInt err = iVoIPAudioSession->SetMode(aMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPILBCDecoderIntfcImpl::GetMode
// ---------------------------------------------------------------------------
//
TInt CVoIPILBCDecoderIntfcImpl::GetMode(TILBCCodecMode& aMode)
    {
    TInt err = iVoIPAudioSession->GetMode(aMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPILBCDecoderIntfcImpl::SetCNG
// ---------------------------------------------------------------------------
//
TInt CVoIPILBCDecoderIntfcImpl::SetCNG(TBool aCng)
    {
    TInt err = iVoIPAudioSession->SetCNG(aCng);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPILBCDecoderIntfcImpl::GetCNG
// ---------------------------------------------------------------------------
//
TInt CVoIPILBCDecoderIntfcImpl::GetCNG(TBool& aCng)
    {
    TInt err = iVoIPAudioSession->GetCNG(aCng);
    return err;
    }

// End of file
