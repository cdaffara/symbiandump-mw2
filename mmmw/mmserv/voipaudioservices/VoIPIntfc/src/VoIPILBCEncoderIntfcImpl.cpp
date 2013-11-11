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
#include "VoIPILBCEncoderIntfcImpl.h"

// ---------------------------------------------------------------------------
// CVoIPILBCEncoderIntfcImpl::NewL
// ---------------------------------------------------------------------------
//
CVoIPILBCEncoderIntfcImpl* CVoIPILBCEncoderIntfcImpl::NewL(
        CVoIPAudioUplinkStreamImpl* aUpLinkStreamImpl)
    {
    CVoIPILBCEncoderIntfcImpl* self = new (ELeave) CVoIPILBCEncoderIntfcImpl();
    CleanupStack::PushL(self);
    self->ConstructL(aUpLinkStreamImpl);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CVoIPILBCEncoderIntfcImpl::~CVoIPILBCEncoderIntfcImpl
// ---------------------------------------------------------------------------
//
CVoIPILBCEncoderIntfcImpl::~CVoIPILBCEncoderIntfcImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPILBCEncoderIntfcImpl::CVoIPILBCEncoderIntfcImpl
// ---------------------------------------------------------------------------
//
CVoIPILBCEncoderIntfcImpl::CVoIPILBCEncoderIntfcImpl()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPILBCEncoderIntfcImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPILBCEncoderIntfcImpl::ConstructL(
        CVoIPAudioUplinkStreamImpl* aUpLinkStreamImpl)
    {
    iVoIPAudioSession = aUpLinkStreamImpl->GetSession();
    CVoIPFormatIntfcImpl::ConstructL(iVoIPAudioSession);
    CVoIPILBCEncoderIntfc::ConstructL(this);
    }

// ---------------------------------------------------------------------------
// CVoIPILBCEncoderIntfcImpl::SetMode
// ---------------------------------------------------------------------------
//
TInt CVoIPILBCEncoderIntfcImpl::SetMode(TILBCCodecMode aMode)
    {
    TInt err = iVoIPAudioSession->SetMode(aMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPILBCEncoderIntfcImpl::GetMode
// ---------------------------------------------------------------------------
//
TInt CVoIPILBCEncoderIntfcImpl::GetMode(TILBCCodecMode& aMode)
    {
    TInt err = iVoIPAudioSession->GetMode(aMode);
    return err;
    }

// End of file
