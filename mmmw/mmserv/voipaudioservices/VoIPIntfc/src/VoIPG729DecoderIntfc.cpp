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
#include <voipg729decoderintfc.h>
#include "VoIPG729DecoderIntfcImpl.h"

// ---------------------------------------------------------------------------
// CVoIPG729DecoderIntfc::~CVoIPG729DecoderIntfc
// ---------------------------------------------------------------------------
//
EXPORT_C CVoIPG729DecoderIntfc::~CVoIPG729DecoderIntfc()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPG729DecoderIntfc::CVoIPG729DecoderIntfc
// ---------------------------------------------------------------------------
//
CVoIPG729DecoderIntfc::CVoIPG729DecoderIntfc()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPG729DecoderIntfc::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPG729DecoderIntfc::ConstructL(
        CVoIPG729DecoderIntfcImpl* aFormatIntfcImpl)
    {
    iFormatIntfcImpl = aFormatIntfcImpl;
    CVoIPFormatIntfc::ConstructL(iFormatIntfcImpl);
    }

// ---------------------------------------------------------------------------
// CVoIPG729DecoderIntfc::BadLsfNextBuffer
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPG729DecoderIntfc::BadLsfNextBuffer()
    {
    TInt err = iFormatIntfcImpl->BadLsfNextBuffer();
    return err;
    }

// End of file
