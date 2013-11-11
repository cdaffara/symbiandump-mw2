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

#include <e32base.h>
#include <voipilbcdecoderintfc.h>
#include "VoIPILBCDecoderIntfcImpl.h"

// ---------------------------------------------------------------------------
// CVoIPILBCDecoderIntfc::~CVoIPILBCDecoderIntfc
// ---------------------------------------------------------------------------
//
EXPORT_C CVoIPILBCDecoderIntfc::~CVoIPILBCDecoderIntfc()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPILBCDecoderIntfc::CVoIPILBCDecoderIntfc
// ---------------------------------------------------------------------------
//
CVoIPILBCDecoderIntfc::CVoIPILBCDecoderIntfc()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPILBCDecoderIntfc::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPILBCDecoderIntfc::ConstructL(
        CVoIPILBCDecoderIntfcImpl* aFormatIntfcImpl)
    {
    iFormatIntfcImpl = aFormatIntfcImpl;
    CVoIPFormatIntfc::ConstructL(iFormatIntfcImpl);
    }

// ---------------------------------------------------------------------------
// CVoIPILBCDecoderIntfc::SetMode
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPILBCDecoderIntfc::SetMode(
        CVoIPFormatIntfc::TILBCCodecMode aMode)
    {
    TInt err = iFormatIntfcImpl->SetMode(aMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPILBCDecoderIntfc::GetMode
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPILBCDecoderIntfc::GetMode(
        CVoIPFormatIntfc::TILBCCodecMode& aMode)
    {
    TInt err = iFormatIntfcImpl->GetMode(aMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPILBCDecoderIntfc::SetCNG
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPILBCDecoderIntfc::SetCNG(TBool aCng)
    {
    TInt err = iFormatIntfcImpl->SetCNG(aCng);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPILBCDecoderIntfc::GetCNG
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPILBCDecoderIntfc::GetCNG(TBool& aCng)
    {
    TInt err = iFormatIntfcImpl->GetCNG(aCng);
    return err;
    }

// End of file
