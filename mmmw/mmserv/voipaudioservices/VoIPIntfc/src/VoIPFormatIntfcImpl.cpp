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
#include "VoIPAudioSession.h"
#include "VoIPFormatIntfcImpl.h"

// -----------------------------------------------------------------------------
// CVoIPFormatIntfcImpl::~CVoIPFormatIntfcImpl
// -----------------------------------------------------------------------------
//
CVoIPFormatIntfcImpl::~CVoIPFormatIntfcImpl()
    {
    }

// -----------------------------------------------------------------------------
// CVoIPFormatIntfcImpl::CVoIPFormatIntfcImpl
// -----------------------------------------------------------------------------
//
CVoIPFormatIntfcImpl::CVoIPFormatIntfcImpl()
    {
    }

// -----------------------------------------------------------------------------
// CVoIPFormatIntfcImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CVoIPFormatIntfcImpl::ConstructL(RVoIPAudioSession* aVoIPAudioSession)
    {
    iVoIPAudioSession = aVoIPAudioSession;
    }

// -----------------------------------------------------------------------------
// CVoIPFormatIntfcImpl::SetObserver
// -----------------------------------------------------------------------------
//
TInt CVoIPFormatIntfcImpl::SetObserver(MVoIPFormatObserver& aObserver)
    {
    iObserver = &aObserver;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CVoIPFormatIntfcImpl::GetSupportedBitRates
// -----------------------------------------------------------------------------
//
TInt CVoIPFormatIntfcImpl::GetSupportedBitRates(RArray<TUint>& aArray)
    {
    TRAPD(err, iVoIPAudioSession->GetSupportedBitRatesL(aArray));
    return err;
    }

// -----------------------------------------------------------------------------
// CVoIPFormatIntfcImpl::SetBitRate
// -----------------------------------------------------------------------------
//
TInt CVoIPFormatIntfcImpl::SetBitRate(TUint aBitrate)
    {
    TInt err = iVoIPAudioSession->SetBitRate(aBitrate);
    return err;
    }

// -----------------------------------------------------------------------------
// CVoIPFormatIntfcImpl::GetBitRate
// -----------------------------------------------------------------------------
//
TInt CVoIPFormatIntfcImpl::GetBitRate(TUint& aBitrate)
    {
    TInt err = iVoIPAudioSession->GetBitRate(aBitrate);
    return err;
    }

// -----------------------------------------------------------------------------
// CVoIPFormatIntfcImpl::SetVAD
// -----------------------------------------------------------------------------
//
TInt CVoIPFormatIntfcImpl::SetVAD(TBool aVad)
    {
    TInt err = iVoIPAudioSession->SetVAD(aVad);
    return err;
    }

// -----------------------------------------------------------------------------
// CVoIPFormatIntfcImpl::GetVAD
// -----------------------------------------------------------------------------
//
TInt CVoIPFormatIntfcImpl::GetVAD(TBool& aVad)
    {
    TInt err = iVoIPAudioSession->GetVAD(aVad);
    return err;
    }

// -----------------------------------------------------------------------------
// CVoIPFormatIntfcImpl::FrameModeRequiredForErrorConcealment
// -----------------------------------------------------------------------------
//
TInt CVoIPFormatIntfcImpl::FrameModeRequiredForErrorConcealment(TBool& aMode)
    {
    TInt err = iVoIPAudioSession->FrameModeRequiredForEC(aMode);
    return err;
    }

// -----------------------------------------------------------------------------
// CVoIPFormatIntfcImpl::SetFrameMode
// -----------------------------------------------------------------------------
//
TInt CVoIPFormatIntfcImpl::SetFrameMode(TBool aMode)
    {
    TInt err = iVoIPAudioSession->SetFrameMode(aMode);
    return err;
    }

// -----------------------------------------------------------------------------
// CVoIPFormatIntfcImpl::GetFrameMode
// -----------------------------------------------------------------------------
//
TInt CVoIPFormatIntfcImpl::GetFrameMode(TBool& aMode)
    {
    TInt err = iVoIPAudioSession->GetFrameMode(aMode);
    return err;
    }

// -----------------------------------------------------------------------------
// CVoIPFormatIntfcImpl::ConcealErrorForNextBuffer
// -----------------------------------------------------------------------------
//
TInt CVoIPFormatIntfcImpl::ConcealErrorForNextBuffer()
    {
    TInt err = iVoIPAudioSession->ConcealErrorForNextBuffer();
    return err;
    }

// End of file
