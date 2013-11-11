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
#include <voipformatintfc.h>
#include <voipg711encoderintfc.h>
#include "VoIPG711EncoderIntfcImpl.h"

// ---------------------------------------------------------------------------
// CVoIPG711EncoderIntfc::~CVoIPG711EncoderIntfc
// ---------------------------------------------------------------------------
//
EXPORT_C CVoIPG711EncoderIntfc::~CVoIPG711EncoderIntfc()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPG711EncoderIntfc::CVoIPG711EncoderIntfc
// ---------------------------------------------------------------------------
//
CVoIPG711EncoderIntfc::CVoIPG711EncoderIntfc()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPG711EncoderIntfc::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPG711EncoderIntfc::ConstructL(
        CVoIPG711EncoderIntfcImpl* aFormatIntfcImpl)
    {
    iFormatIntfcImpl = aFormatIntfcImpl;
    CVoIPFormatIntfc::ConstructL(iFormatIntfcImpl);
    }

// ---------------------------------------------------------------------------
// CVoIPG711EncoderIntfc::SetMode
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPG711EncoderIntfc::SetMode(TG711CodecMode aMode)
    {
    TInt err = iFormatIntfcImpl->SetMode(aMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPG711EncoderIntfc::GetMode
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPG711EncoderIntfc::GetMode(TG711CodecMode& aMode)
    {
    TInt err = iFormatIntfcImpl->GetMode(aMode);
    return err;
    }

// End of file
