/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#include <voipjitterbufferintfc.h>
#include "VoIPJitterBufferIntfcImpl.h"

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfc::~CVoIPJitterBufferIntfc
// ---------------------------------------------------------------------------
//
EXPORT_C CVoIPJitterBufferIntfc::~CVoIPJitterBufferIntfc()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfc::CVoIPJitterBufferIntfc
// ---------------------------------------------------------------------------
//
CVoIPJitterBufferIntfc::CVoIPJitterBufferIntfc()
    {
    }

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfc::ConstructL
// ---------------------------------------------------------------------------
//
void CVoIPJitterBufferIntfc::ConstructL(
        CVoIPJitterBufferIntfcImpl* aVoIPJitterBufferIntfcImpl)
    {
    iVoIPJitterBufferIntfcImpl = aVoIPJitterBufferIntfcImpl;
    }

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfc::SetObserver
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPJitterBufferIntfc::SetObserver(
        MVoIPJitterBufferObserver& aObserver)
    {
    TInt err = iVoIPJitterBufferIntfcImpl->SetObserver(aObserver);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfc::ConfigureJitterBuffer
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPJitterBufferIntfc::ConfigureJitterBuffer(
        const TVoIPJBConfig& aJbConfig)
    {
    TInt err = iVoIPJitterBufferIntfcImpl->ConfigureJitterBuffer(aJbConfig);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfc::ResetJitterBuffer
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPJitterBufferIntfc::ResetJitterBuffer(TBool aPlayTone)
    {
    TInt err = iVoIPJitterBufferIntfcImpl->ResetJitterBuffer(aPlayTone);
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfc::DelayDown
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPJitterBufferIntfc::DelayDown()
    {
    TInt err = iVoIPJitterBufferIntfcImpl->DelayDown();
    return err;
    }

// ---------------------------------------------------------------------------
// CVoIPJitterBufferIntfc::DelayUp
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoIPJitterBufferIntfc::DelayUp()
    {
    TInt err = iVoIPJitterBufferIntfcImpl->DelayUp();
    return err;
    }

// End of file
