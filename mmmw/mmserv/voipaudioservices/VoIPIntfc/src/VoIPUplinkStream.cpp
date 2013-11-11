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
 * Description:  Uplink stream APIs.
 *
 */

#include <voipuplinkstream.h>
#include "VoIPUplinkStreamImpl.h"

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStream::~CVoIPAudioUplinkStream
// ---------------------------------------------------------------------------
//
EXPORT_C CVoIPAudioUplinkStream::~CVoIPAudioUplinkStream()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStream::CVoIPAudioUplinkStream
// ---------------------------------------------------------------------------
//
CVoIPAudioUplinkStream::CVoIPAudioUplinkStream()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStream::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPAudioUplinkStream::ConstructL(
        CVoIPAudioUplinkStreamImpl* aVoIPAudioUplinkStreamImp)
    {
    iVoIPAudioUplinkStreamImpl = aVoIPAudioUplinkStreamImp;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStream::Open
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioUplinkStream::Open(MVoIPUplinkObserver& aObserver)
    {
    TInt err = iVoIPAudioUplinkStreamImpl->Open(aObserver);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStream::GetVersion
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioUplinkStream::GetVersion(TVersion& aVersion)
    {
    TInt err = iVoIPAudioUplinkStreamImpl->GetVersion(aVersion);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStream::GetMaxGain
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioUplinkStream::GetMaxGain(TInt& aGain)
    {
    TInt err = iVoIPAudioUplinkStreamImpl->GetMaxGain(aGain);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStream::SetGain
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioUplinkStream::SetGain(TInt aGain)
    {
    TInt err = iVoIPAudioUplinkStreamImpl->SetGain(aGain);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStream::GetGain
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioUplinkStream::GetGain(TInt& aGain)
    {
    TInt err = iVoIPAudioUplinkStreamImpl->GetGain(aGain);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStream::SetFormat
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioUplinkStream::SetFormat(TVoIPCodecFormat aFormat,
        CVoIPFormatIntfc*& aIntfc)
    {
    TRAPD(err, iVoIPAudioUplinkStreamImpl->SetFormatL(aFormat, aIntfc));
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStream::Start
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioUplinkStream::Start()
    {
    TInt err = iVoIPAudioUplinkStreamImpl->Start();
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStream::
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioUplinkStream::Stop()
    {
    TInt err = iVoIPAudioUplinkStreamImpl->Stop();
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStream::Close
// ---------------------------------------------------------------------------
//
EXPORT_C void CVoIPAudioUplinkStream::Close()
    {
    iVoIPAudioUplinkStreamImpl->Close();
    }

// ---------------------------------------------------------------------------
// CVoIPAudioUplinkStream::BufferEmptied
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioUplinkStream::BufferEmptied(CVoIPDataBuffer* aBuffer)
    {
    TInt err = iVoIPAudioUplinkStreamImpl->BufferEmptied(aBuffer);
    return err;
    }

// End of file
