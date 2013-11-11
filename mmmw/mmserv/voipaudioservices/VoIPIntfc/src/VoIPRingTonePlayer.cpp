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

#include <e32base.h>
#include <voipringtoneplayer.h>
#include "VoIPRingTonePlayerImpl.h"

// ---------------------------------------------------------------------------
// CRingTonePlayer::~CRingTonePlayer
// ---------------------------------------------------------------------------
//
EXPORT_C CRingTonePlayer::~CRingTonePlayer()
    {
    }

// ---------------------------------------------------------------------------
// CRingTonePlayer::CRingTonePlayer
// ---------------------------------------------------------------------------
//
CRingTonePlayer::CRingTonePlayer()
    {
    }

// ---------------------------------------------------------------------------
// CRingTonePlayer::ConstructL
// ---------------------------------------------------------------------------
//
void CRingTonePlayer::ConstructL(CRingTonePlayerImpl* aRingTonePlayerImpl)
    {
    iRingTonePlayerImpl = aRingTonePlayerImpl;
    }

// ---------------------------------------------------------------------------
// CRingTonePlayer::Open
//
// Gets ring tone from the Profiles
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRingTonePlayer::Open(MRingToneObserver& aObserver)
    {
    TRAPD(err, iRingTonePlayerImpl->OpenL(aObserver));
    return err;
    }

// ---------------------------------------------------------------------------
// CRingTonePlayer::Open
//
// Plays ring tone from file
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRingTonePlayer::Open(MRingToneObserver& aObserver,
        TPtr aFileName)
    {
    TRAPD(err, iRingTonePlayerImpl->OpenL(aObserver, aFileName));
    return err;
    }

// ---------------------------------------------------------------------------
// CRingTonePlayer::Open
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRingTonePlayer::Open(MRingToneObserver& aObserver,
        RFile& aFileHandle)
    {
    TRAPD(err, iRingTonePlayerImpl->OpenL(aObserver, aFileHandle));
    return err;
    }

// ---------------------------------------------------------------------------
// CRingTonePlayer::Open
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRingTonePlayer::Open(MRingToneObserver& aObserver,
        TPtr8 aDesTone, TPtr8 aMimeType)
    {
    TRAPD(err, iRingTonePlayerImpl->OpenL(aObserver, aDesTone, aMimeType));
    return err;
    }

// ---------------------------------------------------------------------------
// CRingTonePlayer::Play
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRingTonePlayer::Play()
    {
    TInt err = iRingTonePlayerImpl->Play();
    return err;
    }

// ---------------------------------------------------------------------------
// CRingTonePlayer::Pause
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRingTonePlayer::Pause()
    {
    TInt err = iRingTonePlayerImpl->Pause();
    return err;
    }

// ---------------------------------------------------------------------------
// CRingTonePlayer::Resume
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRingTonePlayer::Resume()
    {
    TInt err = iRingTonePlayerImpl->Resume();
    return err;
    }

// ---------------------------------------------------------------------------
// CRingTonePlayer::Stop
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRingTonePlayer::Stop()
    {
    TInt err = iRingTonePlayerImpl->Stop();
    return err;
    }

// ---------------------------------------------------------------------------
// CRingTonePlayer::Close
// ---------------------------------------------------------------------------
//
EXPORT_C void CRingTonePlayer::Close()
    {
    iRingTonePlayerImpl->Close();
    }

// End of file
