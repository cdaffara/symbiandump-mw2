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

#include <voipformatintfc.h>
#include "VoIPFormatIntfcImpl.h"

// ---------------------------------------------------------------------------
// CVoIPFormatIntfc::~CVoIPFormatIntfc
// ---------------------------------------------------------------------------
//
EXPORT_C CVoIPFormatIntfc::~CVoIPFormatIntfc()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPFormatIntfc::CVoIPFormatIntfc
// ---------------------------------------------------------------------------
//
CVoIPFormatIntfc::CVoIPFormatIntfc()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPFormatIntfc::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPFormatIntfc::ConstructL(CVoIPFormatIntfcImpl* aFormatIntfcImpl)
    {
    iVoIPFormatIntfcImpl = aFormatIntfcImpl;
    }

// ---------------------------------------------------------------------------
// CVoIPFormatIntfc::SetObserver
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPFormatIntfc::SetObserver(MVoIPFormatObserver& aObserver)
    {
    TInt err = iVoIPFormatIntfcImpl->SetObserver(aObserver);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPFormatIntfc::GetSupportedBitRates
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPFormatIntfc::GetSupportedBitRates(RArray<TUint>& aArray)
    {
    TInt err = iVoIPFormatIntfcImpl->GetSupportedBitRates(aArray);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPFormatIntfc::SetBitRate
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPFormatIntfc::SetBitRate(TUint aBitrate)
    {
    TInt err = iVoIPFormatIntfcImpl->SetBitRate(aBitrate);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPFormatIntfc::GetBitRate
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPFormatIntfc::GetBitRate(TUint& aBitrate)
    {
    TInt err = iVoIPFormatIntfcImpl->GetBitRate(aBitrate);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPFormatIntfc::SetVAD
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPFormatIntfc::SetVAD(TBool aVad)
    {
    TInt err = iVoIPFormatIntfcImpl->SetVAD(aVad);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPFormatIntfc::GetVAD
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPFormatIntfc::GetVAD(TBool& aVad)
    {
    TInt err = iVoIPFormatIntfcImpl->GetVAD(aVad);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPFormatIntfc::FrameModeRequiredForErrorConcealment
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPFormatIntfc::FrameModeRequiredForErrorConcealment(
        TBool& aMode)
    {
    TInt err = iVoIPFormatIntfcImpl->FrameModeRequiredForErrorConcealment(
            aMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPFormatIntfc::SetFrameMode
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPFormatIntfc::SetFrameMode(TBool aMode)
    {
    TInt err = iVoIPFormatIntfcImpl->SetFrameMode(aMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPFormatIntfc::GetFrameMode
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPFormatIntfc::GetFrameMode(TBool& aMode)
    {
    TInt err = iVoIPFormatIntfcImpl->GetFrameMode(aMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPFormatIntfc::ConcealErrorForNextBuffer
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPFormatIntfc::ConcealErrorForNextBuffer()
    {
    TInt err = iVoIPFormatIntfcImpl->ConcealErrorForNextBuffer();
    return err;
    }

// End of file
