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
 * Description:  Downlink stream APIs.
 *
 */

#include <voipdatabuffer.h>
#include <voipdownlinkstream.h>
#include "VoIPDownlinkStreamImpl.h"

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStream::~CVoIPAudioDownlinkStream
// ---------------------------------------------------------------------------
//
EXPORT_C CVoIPAudioDownlinkStream::~CVoIPAudioDownlinkStream()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStream::CVoIPAudioDownlinkStream
// ---------------------------------------------------------------------------
//
CVoIPAudioDownlinkStream::CVoIPAudioDownlinkStream()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStream::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPAudioDownlinkStream::ConstructL(
        CVoIPAudioDownlinkStreamImpl* aVoIPAudioDownlinkStreamImpl)
    {
    iVoIPAudioDownlinkStreamImpl = aVoIPAudioDownlinkStreamImpl;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStream::Open
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioDownlinkStream::Open(MVoIPDownlinkObserver& aObserver)
    {
    TInt err = iVoIPAudioDownlinkStreamImpl->Open(aObserver);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStream::GetVersion
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioDownlinkStream::GetVersion(TVersion& aVersion)
    {
    TInt err = iVoIPAudioDownlinkStreamImpl->GetVersion(aVersion);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStream::GetMaxVolume
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioDownlinkStream::GetMaxVolume(TInt& aVolume)
    {
    TInt err = iVoIPAudioDownlinkStreamImpl->GetMaxVolume(aVolume);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStream::SetVolume
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioDownlinkStream::SetVolume(TInt aVolume)
    {
    TInt err = iVoIPAudioDownlinkStreamImpl->SetVolume(aVolume);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStream::GetVolume
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioDownlinkStream::GetVolume(TInt& aVolume)
    {
    TInt err = iVoIPAudioDownlinkStreamImpl->GetVolume(aVolume);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStream::SetFormat
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioDownlinkStream::SetFormat(TVoIPCodecFormat aFormat,
        CVoIPFormatIntfc*& aIntfc)
    {
    TRAPD(err, iVoIPAudioDownlinkStreamImpl->SetFormatL(aFormat, aIntfc));
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStream::SetAudioDevice
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioDownlinkStream::SetAudioDevice(
        TVoIPOutputDevice aDevice)
    {
    TInt err = iVoIPAudioDownlinkStreamImpl->SetAudioDevice(aDevice);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStream::GetAudioDevice
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioDownlinkStream::GetAudioDevice(
        TVoIPOutputDevice& aDevice)
    {
    TInt err = iVoIPAudioDownlinkStreamImpl->GetAudioDevice(aDevice);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStream::Start
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioDownlinkStream::Start()
    {
    TInt err = iVoIPAudioDownlinkStreamImpl->Start();
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStream::Stop
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioDownlinkStream::Stop()
    {
    TInt err = iVoIPAudioDownlinkStreamImpl->Stop();
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStream::Close
// ---------------------------------------------------------------------------
//
EXPORT_C void CVoIPAudioDownlinkStream::Close()
    {
    iVoIPAudioDownlinkStreamImpl->Close();
    }

// ---------------------------------------------------------------------------
// CVoIPAudioDownlinkStream::BufferFilled
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPAudioDownlinkStream::BufferFilled(CVoIPDataBuffer* aBuffer)
    {
    TInt err = iVoIPAudioDownlinkStreamImpl->BufferFilled(aBuffer);
    return err;
    }

// End of file
