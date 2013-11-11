/*
 * Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include <voipdtmftoneplayer.h>
#include "VoIPAudioSession.h"
#include "VoIPDownlinkStreamImpl.h"
#include "VoIPUplinkStreamImpl.h"
#include "VoIPDtmfTonePlayerImpl.h"

// ---------------------------------------------------------------------------
// CDTMFTonePlayer::~CDTMFTonePlayer
// ---------------------------------------------------------------------------
//
EXPORT_C CDTMFTonePlayer::~CDTMFTonePlayer()
    {
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayer::CDTMFTonePlayer
// ---------------------------------------------------------------------------
//
CDTMFTonePlayer::CDTMFTonePlayer()
    {
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayer::ConstructL
// ---------------------------------------------------------------------------
//
void CDTMFTonePlayer::ConstructL(CDTMFTonePlayerImpl* aDTMFTonePlayerImpl)
    {
    iDTMFTonePlayerImpl = aDTMFTonePlayerImpl;
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayer::Open
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CDTMFTonePlayer::Open(MDTMFToneObserver& aObserver)
    {
    TInt err = iDTMFTonePlayerImpl->Open(aObserver);
    return err;
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayer::Play
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CDTMFTonePlayer::Play(const TPtr aTones)
    {
    TInt err = iDTMFTonePlayerImpl->Play(aTones);
    return err;
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayer::Stop
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CDTMFTonePlayer::Stop()
    {
    TInt err = iDTMFTonePlayerImpl->Stop();
    return err;
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayer::Close
// ---------------------------------------------------------------------------
//
EXPORT_C void CDTMFTonePlayer::Close()
    {
    iDTMFTonePlayerImpl->Close();
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayer::AddVoIPAudioUplinkStream
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CDTMFTonePlayer::AddVoIPAudioUplinkStream(
        CVoIPAudioUplinkStream& aStream)
    {
    TInt err = KErrArgument;
    if (&aStream)
        {
        RVoIPAudioSession* session =
                aStream.iVoIPAudioUplinkStreamImpl->GetSession();
        err = iDTMFTonePlayerImpl->AddVoIPAudioUplinkStream(aStream, session);
        }
    return err;
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayer::RemoveVoIPAudioUplinkStream
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CDTMFTonePlayer::RemoveVoIPAudioUplinkStream(
        CVoIPAudioUplinkStream& aStream)
    {
    TInt err = iDTMFTonePlayerImpl->RemoveVoIPAudioUplinkStream(aStream);
    return err;
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayer::AddVoIPAudioDownlinkStream
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CDTMFTonePlayer::AddVoIPAudioDownlinkStream(
        CVoIPAudioDownlinkStream& aStream)
    {
    TInt err = KErrArgument;
    if (&aStream)
        {
        RVoIPAudioSession* session =
                aStream.iVoIPAudioDownlinkStreamImpl->GetSession();
        err = iDTMFTonePlayerImpl->AddVoIPAudioDownlinkStream(aStream, session);
        }
    return err;
    }

// ---------------------------------------------------------------------------
// CDTMFTonePlayer::RemoveVoIPAudioDownlinkStream
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CDTMFTonePlayer::RemoveVoIPAudioDownlinkStream(
        CVoIPAudioDownlinkStream& aStream)
    {
    TInt err = iDTMFTonePlayerImpl->RemoveVoIPAudioDownlinkStream(aStream);
    return err;
    }

// End of file
