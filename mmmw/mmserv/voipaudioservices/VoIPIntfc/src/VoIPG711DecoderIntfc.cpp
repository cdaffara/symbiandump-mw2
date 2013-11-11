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

#include <voipg711decoderintfc.h>
#include "VoIPFormatIntfcImpl.h"
#include "VoIPG711DecoderIntfcImpl.h"

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfc::~CVoIPG711DecoderIntfc
// ---------------------------------------------------------------------------
//
EXPORT_C CVoIPG711DecoderIntfc::~CVoIPG711DecoderIntfc()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfc::CVoIPG711DecoderIntfc
// ---------------------------------------------------------------------------
//
CVoIPG711DecoderIntfc::CVoIPG711DecoderIntfc()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfc::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPG711DecoderIntfc::ConstructL(
        CVoIPG711DecoderIntfcImpl* aFormatIntfcImpl)
    {
    iFormatIntfcImpl = aFormatIntfcImpl;
    CVoIPFormatIntfc::ConstructL(iFormatIntfcImpl);
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfc::SetMode
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPG711DecoderIntfc::SetMode(TG711CodecMode aMode)
    {
    TInt err = iFormatIntfcImpl->SetMode(aMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfc::GetMode
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPG711DecoderIntfc::GetMode(TG711CodecMode& aMode)
    {
    TInt err = iFormatIntfcImpl->GetMode(aMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfc::SetCNG
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPG711DecoderIntfc::SetCNG(TBool aCng)
    {
    TInt err = iFormatIntfcImpl->SetCNG(aCng);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfc::GetCNG
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPG711DecoderIntfc::GetCNG(TBool& aCng)
    {
    TInt err = iFormatIntfcImpl->GetCNG(aCng);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfc::SetPLC
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPG711DecoderIntfc::SetPLC(TBool aPlc)
    {
    TInt err = iFormatIntfcImpl->SetPLC(aPlc);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPG711DecoderIntfc::GetPLC
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPG711DecoderIntfc::GetPLC(TBool& aPlc)
    {
    TInt err = iFormatIntfcImpl->GetPLC(aPlc);
    return err;
    }

// End of file
