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
#include <voipilbcencoderintfc.h>
#include "VoIPILBCEncoderIntfcImpl.h"

// ---------------------------------------------------------------------------
// CVoIPILBCEncoderIntfc::~CVoIPILBCEncoderIntfc
// ---------------------------------------------------------------------------
//
EXPORT_C CVoIPILBCEncoderIntfc::~CVoIPILBCEncoderIntfc()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPILBCEncoderIntfc::CVoIPILBCEncoderIntfc
// ---------------------------------------------------------------------------
//
CVoIPILBCEncoderIntfc::CVoIPILBCEncoderIntfc()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPILBCEncoderIntfc::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPILBCEncoderIntfc::ConstructL(
        CVoIPILBCEncoderIntfcImpl* aFormatIntfcImpl)
    {
    iFormatIntfcImpl = aFormatIntfcImpl;
    CVoIPFormatIntfc::ConstructL(iFormatIntfcImpl);
    }

// ---------------------------------------------------------------------------
// CVoIPILBCEncoderIntfc::SetMode
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPILBCEncoderIntfc::SetMode(TILBCCodecMode aMode)
    {
    TInt err = iFormatIntfcImpl->SetMode(aMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPILBCEncoderIntfc::GetMode
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPILBCEncoderIntfc::GetMode(TILBCCodecMode& aMode)
    {
    TInt err = iFormatIntfcImpl->GetMode(aMode);
    return err;
    }

// End of file
